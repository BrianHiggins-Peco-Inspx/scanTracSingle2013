// SendNextOGMessage.cpp : implementation file
//

#include "stdafx.h"
#ifdef CompileInHTTPClientOperationsGuardian
#include "SendNextOGMessageThread.h"
#include <string.h>
#include <wchar.h>
#include <crtdefs.h>

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

#include <cpprest/json.h> //don't need
#include <iostream> //don't need

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

// CSendNextOGMessageThread
extern BYTE vGlobalUseCommaForDecimalPoint;
extern int vGlobalLastOGServerErrorCode;
extern std::wstring vGlobalOGLastErrorKeyword;
extern std::wstring vGlobalOGLastErrorMessage;
extern DWORD vGlobalOGSendMessageexceptionCount;
extern DWORD vGlobalOGSendMessageErrorCount;
extern DWORD vGlobalOGSendMessageGoodCount;
extern int vGlobalLastOGServerWriteStatusCode;
extern CSendNextOGMessageThread *vGlobalSendNextOGMessageThread;
extern CString vGlobalOperationsGuardianServer;
extern int vGlobalLastOGServerErrorCode;
extern DWORD vGlobalOGMessageSendCounter;
extern WORD vGlobalOGQueueHead;
extern WORD vGlobalOGQueueTail;
extern std::wstring vGlobalOGQueueMessage[cOGQueueLength];
extern std::wstring vGlobalOGQueueKeyWord[cOGQueueLength];
extern CString vGlobalNewOperationsGuardianErrorMessage;

IMPLEMENT_DYNCREATE(CSendNextOGMessageThread, CWinThread)

CSendNextOGMessageThread::CSendNextOGMessageThread()
{
	vShutDown = false;
	//vOGMessageToSend = "-";
	//vOGMessageKeyWord = "-";
}

CSendNextOGMessageThread::~CSendNextOGMessageThread()
{
}

BOOL CSendNextOGMessageThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CSendNextOGMessageThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSendNextOGMessageThread, CWinThread)
END_MESSAGE_MAP()

// CSendNextOGMessageThread message handlers

int CSendNextOGMessageThread::Run()
{
	while (!vShutDown)
	{
		if (vGlobalOGQueueHead != vGlobalOGQueueTail) //tail always points to next message to send, but there are no messages if head and tail the same
		{
			vOGMessageToSend = vGlobalOGQueueMessage[vGlobalOGQueueTail];
			vOGMessageKeyWord = vGlobalOGQueueKeyWord[vGlobalOGQueueTail];

			SendNextMessage();

			vGlobalOGMessageSendCounter++;

			if (vGlobalOGQueueTail < cOGQueueLength - 1)
				vGlobalOGQueueTail++;
			else
				vGlobalOGQueueTail = 0;
		}
		Sleep(50); //wait 50 milliseconds before sending another, so maximum send rate 20 messages per second
	}
	vGlobalSendNextOGMessageThread = NULL;
	return 1;
}

void CSendNextOGMessageThread::SendNextMessage()
{
	std::wstring TempURLToUse = vGlobalOperationsGuardianServer;

	//for testing use
	//TempURLToUse = L"https://og-demo.pecoinspx-internal.com"; 
	//TempURLToUse = L"https://og-qa.pecoinspx-internal.com"; 
	//vOGMessageKeyWord = L"runduration";
	//vOGMessageToSend = L"{ \"current\": " + dtoa(vGlobalOGSendMessageGoodCount + vGlobalOGSendMessageErrorCount + vGlobalOGSendMessageexceptionCount, 0);
	//vOGMessageToSend += L", \"machineID\": \"ST538xx\" }";

	TempURLToUse += L"/api/events"; 
	TempURLToUse += L"/"; 
	TempURLToUse += vOGMessageKeyWord; 
	TempURLToUse += L"?email=david.hazlett@peco-inspx.com&password=password"; 
	//TempURLToUse += L"?email=brian.higgins@peco-inspx.com&password=xrayscando"; 

  auto TempReplyStream = std::make_shared<ostream>();

	std::wstring TempOutputFileName(U("0_OGReply.html")); //don't need.  remove when all working

#ifndef _DEBUG
  // Open stream to output file.
  pplx::task<void> TempSendOGMessageTask = fstream::open_ostream(TempOutputFileName).then([=](ostream outFile)
  {
     *TempReplyStream = outFile; //don't need.  remove when all working

		//web::http::client::http_client_config config;
		//config.set_validate_certificates(false);
		//http_client TempClientToOGServer(TempURLToUse, config);

    // Create http_client to send the message.
		http_client TempClientToOGServer(TempURLToUse);

		//DWORD opt = WINHTTP_OPTION_REDIRECT_POLICY_ALWAYS;
		//WinHttpSetOption(hSession, WINHTTP_OPTION_REDIRECT_POLICY, &opt, sizeof(opt));

		//WinHttpSetOption to set WINHTTP_OPTION_SECURITY_FLAGS option to a desired combination of flags

		//auto client = std::make_shared<web::http::client::http_client>(U("https://localhost:8081/"), config);
		//send the message
		return TempClientToOGServer.request(methods::POST, U(" "), vOGMessageToSend, U("application/json"));
	})

  // Handle response headers arriving.
  .then([=](http_response response)
  {
    //printf("Received response status code:%u\n", response.status_code());
		vGlobalLastOGServerWriteStatusCode = response.status_code();

		if ((vGlobalLastOGServerWriteStatusCode >= 200) && (vGlobalLastOGServerWriteStatusCode <= 202))
			vGlobalOGSendMessageGoodCount++;
		else
		{
			vGlobalOGSendMessageErrorCount++;
		 	vGlobalOGLastErrorKeyword = TempURLToUse;
			vGlobalOGLastErrorMessage = vOGMessageToSend;
			vGlobalLastOGServerErrorCode = vGlobalLastOGServerWriteStatusCode;
		}
		//Operations Guardian Transmission Log File
		WriteToDebugLog(TempURLToUse, vOGMessageToSend, vGlobalLastOGServerWriteStatusCode, response.reason_phrase());

    // Write response body into the file.
    return response.body().read_to_end(TempReplyStream->streambuf());
  })

  // Close the file stream.
  .then([=](size_t)
  {
     return TempReplyStream->close();
  });

  // Wait for all the outstanding I/O to complete and handle any exceptions
  try
  {
     TempSendOGMessageTask.wait();
  }
  catch (const std::exception &e)
  {
		printf("Error exception:%s\n", e.what());

		vGlobalNewOperationsGuardianErrorMessage = e.what();
		std::wstring TempErrorMessage = vGlobalNewOperationsGuardianErrorMessage;
		vGlobalOGLastErrorKeyword = TempURLToUse;
		vGlobalOGLastErrorMessage = vOGMessageToSend;
		vGlobalLastOGServerWriteStatusCode = 0;
		vGlobalLastOGServerErrorCode = 0;

		//int TempPosition = vGlobalNewOperationsGuardianErrorMessage.Find("data is invalid", 0);
		//if (TempPosition > 0)
		//	vGlobalLastOGServerErrorCode = 1;

		//int TempPosition = vGlobalNewOperationsGuardianErrorMessage.Find("12175");
		//if (TempPosition >= 0)
		//{
		//	WINHTTP_CALLBACK_STATUS_SECURE_FAILURE notification in a status callback function. For more information, see WINHTTP_STATUS_CALLBACK.

			//typedef void ( CALLBACK *WINHTTP_STATUS_CALLBACK)(
			//		_In_ HINTERNET hInternet,
			//		_In_ DWORD_PTR dwContext,
			//		_In_ DWORD     dwInternetStatus,
			//		_In_ LPVOID    lpvStatusInformation,
			//		_In_ DWORD     dwStatusInformationLength
			//	);
		//}
		WriteToDebugLog(TempURLToUse, vOGMessageToSend, 0, TempErrorMessage); //L"Exception");

		vGlobalOGSendMessageexceptionCount++;
  }
#else
	vGlobalOGSendMessageErrorCount++;
#endif
}

void CSendNextOGMessageThread::WriteToDebugLog(std::wstring TempURL, std::wstring TempMessage, int TempCode, std::wstring TempResponse)
{
	CFile vOGLogFile;
	CFileException TempFileException;
	LPCTSTR TempFileName = _T("C:\\ScanTracLogs\\OperationsGuardianLog.txt");
	if (vOGLogFile.Open(TempFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyNone , &TempFileException ))
	{
		TRY
		{
			vOGLogFile.SeekToEnd();
		}
		CATCH_ALL(TempFileException)
		{
			//ReportErrorMessage("Failed SeekToEnd of AC Power Monitor Log", cEMailInspx, 32000);
		}
		END_CATCH_ALL

		CFileException TempFileException;
		TRY
		{
			CString TempStringToWrite(TempURL.c_str());

			CString TempLineToWrite = "URL: " + TempStringToWrite + "\n";
			DWORD BytesWritten = 0; 
			CW2A TempOutputString(TempLineToWrite);

			vOGLogFile.Write(TempOutputString, TempLineToWrite.GetLength());

			CString TempOutputString1(TempMessage.c_str());
			TempLineToWrite = "Message: " + TempOutputString1 + "\n";
			BytesWritten = 0; 
			CW2A TempOutputString2(TempLineToWrite);
			vOGLogFile.Write(TempOutputString2, TempLineToWrite.GetLength());

			CTime TempTime;
			TempTime = CTime::GetCurrentTime();
			CString TempTimeString = " ";
			TempTimeString = TempTime.Format("%#I:%M:%S%p");//12 hour time format with am pm

			TempLineToWrite = "Reply Code: " + dtoa(TempCode, 0) + "       Time: " + TempTimeString + "\n";
			BytesWritten = 0; 
			CW2A TempOutputString3(TempLineToWrite);
			vOGLogFile.Write(TempOutputString3, TempLineToWrite.GetLength());

			if (TempResponse.length() > 0)
			{
				CString TempOutputString4(TempResponse.c_str());
				TempLineToWrite = "Reason: " + TempOutputString4;

				if (TempCode == 404)
					TempLineToWrite = TempLineToWrite + "-Maybe ScanTrac Serial Number not in OG Database";

				int TempPosition = TempLineToWrite.Find(_T("data is invalid"), 0);
				if (TempPosition > 0)
					vGlobalLastOGServerErrorCode = 1;

				TempLineToWrite = TempLineToWrite + "\n\n";
				BytesWritten = 0; 
				CW2A TempOutputString5(TempLineToWrite);
				vOGLogFile.Write(TempOutputString5, TempLineToWrite.GetLength());
			}

		}
		CATCH_ALL(TempFileException)
		{
			#ifdef _DEBUG
				afxDump << "Could Not Output AC Strings to File"  << "\n";
			#endif
		}
		END_CATCH_ALL

		vOGLogFile.Close();
	}
}


//std::wstring CSendNextOGMessageThread::ConvertCStringToWString(CString TempString)
//{
//	std::wstring TempResult(TempString);
//
	//WORD TempLength = TempString.GetLength();
	//for (WORD TempLoop = 0; TempLoop < TempLength; TempLoop++)
	//{
	//	char TempChar = TempString.GetAt(TempLoop);
	//	wchar_t TempWChar = TempChar;
	//	TempResult = TempResult + TempWChar;
	//}
//
//	return TempResult;
//}
//
CString CSendNextOGMessageThread::dtoa(double TempData, BYTE TempClipLength)
{
		//Clip Length is number of digits after decimal point
	CString TempString = _T("");
	TempString = _T("");
	if (vGlobalUseCommaForDecimalPoint)
	{
		if ((!(TempData > -.0000001)) && (TempData < 0))
		{
			TempData = - TempData;
			TempString = _T("-");
		}
		if (TempClipLength == 0)
			TempData = TempData + .5;
		else
		if (TempClipLength == 1)
			TempData = TempData + .05;
		else
		if (TempClipLength == 2)
			TempData = TempData + .005;
		else
		if (TempClipLength == 3)
			TempData = TempData + .0005;
		else
		if (TempClipLength == 4)
			TempData = TempData + .00005;
		wchar_t TempChars[20] = _T("");

		__int64 TempInt = (__int64)TempData;
		_i64tow_s(TempInt,TempChars, 18, 10);
		TempString = TempString + TempChars;

		if (TempClipLength > 0)
			TempString = TempString + _T(",");
		
		double TempDouble = TempData - TempInt;
		if (TempDouble != 0)
		{
			for (BYTE TempLoop = 0; TempLoop < TempClipLength; TempLoop++)
			{
				TempInt = (BYTE)(TempDouble * 10);
				TempString = TempString + (char)(TempInt + 48);
				TempDouble = (TempDouble * 10) - TempInt;
			}
			if (TempClipLength > 0)
			while ((TempString.GetLength() > 1) &&
				(TempString.GetAt(TempString.GetLength()-1) == 48))
				TempString.Delete(TempString.GetLength()-1,1);
		}
		if (TempString == _T("-0,")) 
			TempString = _T("0,");
		if (TempString == _T("-0"))
			TempString = _T("0");

		wchar_t TempChar = TempString.GetAt(TempString.GetLength() - 1);
		if (TempChar == ',')
			TempString.Delete(TempString.GetLength() - 1,1);
	}
	else
	{ //english
				//Clip Length is number of digits after decimal point
		if ((!(TempData > -.0000001)) && (TempData < 0))
		{
			TempData = - TempData;
			TempString = _T("-");
		}
		if (TempClipLength == 0)
			TempData = TempData + .5;
		else
		if (TempClipLength == 1)
			TempData = TempData + .05;
		else
		if (TempClipLength == 2)
			TempData = TempData + .005;
		else
		if (TempClipLength == 3)
			TempData = TempData + .0005;
		else
		if (TempClipLength == 4)
			TempData = TempData + .00005;
		wchar_t TempChars[20] = _T("");

		__int64 TempInt = (__int64)TempData;
		_i64tow_s(TempInt,TempChars, 18, 10);
		TempString = TempString + TempChars;

		if (TempClipLength > 0)
			TempString = TempString + _T(".");
		
		double TempDouble = TempData - TempInt;
		if (TempDouble != 0)
		{
			for (BYTE TempLoop = 0; TempLoop < TempClipLength; TempLoop++)
			{
				TempInt = (BYTE)(TempDouble * 10);
				TempString = TempString + (char)(TempInt + 48);
				TempDouble = (TempDouble * 10) - TempInt;
			}
			if (TempClipLength > 0)
			while ((TempString.GetLength() > 1) &&
				(TempString.GetAt(TempString.GetLength()-1) == 48))
				TempString.Delete(TempString.GetLength()-1,1);
		}
		if (TempString == _T("-0.")) 
			TempString = _T("0.");
		if (TempString == _T("-0"))
			TempString = _T("0");

		wchar_t TempChar = TempString.GetAt(TempString.GetLength() - 1);
		if ((TempChar == '.') || ((vGlobalUseCommaForDecimalPoint) && (TempChar == ',')))
			TempString.Delete(TempString.GetLength() - 1,1);
	}
	return TempString;
}

//CString CSendNextOGMessageThread::ConvertWStringToCString(std::wstring TempString)
//{
//	CString TempResult("");
//
//	WORD TempLength = TempString.length();
//	for (WORD TempLoop = 0; TempLoop < TempLength; TempLoop++)
//	{
//		wchar_t TempWChar = TempString[TempLoop];
//		char TempChar = (char)TempWChar;
//		TempResult = TempResult + TempChar;
//	}
//	return TempResult;
//}

#endif
