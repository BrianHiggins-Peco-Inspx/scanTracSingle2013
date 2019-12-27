//////////////////////////////////////////////////////////////////////
//ScanTrac Side View Source File
// StartCheckDisk.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "string.h"
#include "stdio.h"
#include "afx.h"
#include "afxwin.h"
#include "process.h"
#include "scantrac.h"
#include "scantracdlg.h"
#include "SelectItemDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern bool vGlobalRunningCheckDisk;

CString vOutputTextString;

// Want to setup file to run check disk, return success(true) or failure(false)
BOOL WriteFilesToRunCheckDisk(int WhichHardDiskDrive) 
{
	CString TempCString;
	bool bCheckDiskOkay = true;

	// if this is Windows XP Embedded, check bit for check disk enable is set correctly in registry
	//kjh 7/14/2010 read registry value
	CRegKey TempRegistryKey;
	DWORD dwType = REG_DWORD;

	long TempErrorCode = TempRegistryKey.Open(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Services\\RegFilter")); 
	if (TempErrorCode == ERROR_SUCCESS)
	{
		DWORD TempValueField;
		CString KeyName = "Start";
		TempErrorCode = TempRegistryKey.QueryDWORDValue(KeyName, TempValueField);
		if ((TempErrorCode == ERROR_SUCCESS) && (TempValueField != 2))
		{
			TempValueField = 2;
			TempErrorCode = TempRegistryKey.SetDWORDValue(KeyName,TempValueField);
		}
	}
	TempRegistryKey.Close();
	// end registry check for password for Teamviewer kjh,



	//send to cmd.exe the cmd: chkdsk c: /f <AnswerYes.txt\n in the batch file
	TRY
	{
		// delete previous batch file and rewrite, easier for d: or c: questions
		HANDLE hBatchFileCheckDiskInfo = CreateFile(_T("\\ScanTracLogs\\RunChkDskNoDelay.bat"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ |FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hBatchFileCheckDiskInfo != INVALID_HANDLE_VALUE) // returns invalid handle if file exists, don't need ((ErrorMsg = GetLastError()) != ERROR_FILE_EXISTS))
		{	
			CString BufferToWrite;
			DWORD BytesWritten = 0; 
			if (WhichHardDiskDrive != 2)	// if drive selected is not d:, scan c:
			{					
				BufferToWrite = "chkdsk c: /f <c:\\ScanTracLogs\\AnswerYes.txt";
			}
			else
			{
				vGlobalRunningCheckDisk = true;
				BufferToWrite = "chkdsk d: /f <c:\\ScanTracLogs\\AnswerYes.txt";
			}
			
			CW2A TempStringToWrite(BufferToWrite);

			WriteFile(hBatchFileCheckDiskInfo, TempStringToWrite, BufferToWrite.GetLength(), &BytesWritten, NULL);
			if (BytesWritten != BufferToWrite.GetLength())
			{
				bCheckDiskOkay = false;
			}
		}
		CloseHandle(hBatchFileCheckDiskInfo);


		if (bCheckDiskOkay == true)
		{
			//check if yes file exists, if not write it
			//If CreateFile() successfully creates the file, a valid HANDLE to that Win32 file object is returned; otherwise, INVALID_HANDLE_VALUE is returned
			HANDLE hBatchFileVerify = CreateFile(_T("\\ScanTracLogs\\AnswerYes.txt"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ |FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

			if (hBatchFileVerify != INVALID_HANDLE_VALUE) // returns invalid handle if file exists, don't need ((ErrorMsg = GetLastError()) != ERROR_FILE_EXISTS))
			{	// if file does not exist, write it
				CString BufferToWrite = "yy\n";
				DWORD BytesWritten = 0; 
			
				CW2A TempStringToWrite(BufferToWrite);
				WriteFile(hBatchFileVerify, TempStringToWrite, BufferToWrite.GetLength(), &BytesWritten, NULL);
				if (BytesWritten != BufferToWrite.GetLength())
				{
					bCheckDiskOkay = false;
				}		
			}
			CloseHandle(hBatchFileVerify);
		}

		if (bCheckDiskOkay == true)
		{
			Sleep(1);		//release thread for a moment
			int TempError = _spawnl(_P_NOWAIT, "C:\\ScanTracLogs\\RunChkDskNoDelay.bat", "C:\\ScanTracLogs\\RunChkDskNoDelay.bat", NULL);

			if (errno)
			{
					LPVOID ErrorMsgBuf;
					vOutputTextString.Format(_T("Could Not Run Check Disk Batch File: Error %d.  \n"),errno);

					FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,NULL, GetLastError(), 
												MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &ErrorMsgBuf,0,NULL);

					vOutputTextString = vOutputTextString + (LPTSTR) ErrorMsgBuf;

					LocalFree(ErrorMsgBuf);
			}
			else
			{
					Sleep(1);
			}

		}
	}	// end try
	CATCH_ALL (TempFileException);
	{
		ReportErrorMessage("Failed to Setup CheckDisk correctly", cEMailInspx, 32000);
	}

	END_CATCH_ALL

	return (bCheckDiskOkay);
}