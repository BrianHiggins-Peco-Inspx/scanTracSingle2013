//ScanTrac Side View Source File
// LogFile.cpp: implementation of the CLogFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScanTrac.h"
#include "LogFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CCriticalSection vGlobalWriteToLogLock;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogFile::CLogFile()
{
	vLogDirectory = cLogDirectory;
	vSerialDataLogFile = NULL;
	vLogSerialData = false;
	vShowingSerialDataOnScreen = false;
	vSendCount = 0;
	vReceiveCount = 0;
	vLastDataReceived = "None";
	vLastDataSent = "None";
	vLogSyncData = true;
}

CLogFile::~CLogFile()
{
	if (vSerialDataLogFile)
		CloseLogFile();
}

void CLogFile::WriteToLogFile(CString TempStringToWrite, BYTE TempMessageType) 
{
	if (vGlobalWriteToLogLock.Lock())
	{
		CFileException TempFileException;
		CMemoryException TempMemoryException;
		TRY
		{
			BYTE TempSyncMessage = false;

			CTime TempTime;
			TempTime = CTime::GetCurrentTime();
			CString TempTimeString = TempTime.Format("%H:%M:%S");
			double TempStartTime = CIFCOS::GetSystimeMillisecs();
			TempStartTime = TempStartTime / 1000;
			TempStartTime = TempStartTime - ((DWORD)TempStartTime);
			CString TempString1 = dtoa(TempStartTime,3);
			TempString1.Delete(0,1);
			TempTimeString = TempTimeString + TempString1;

			if (TempMessageType == cInputMessage)
			{
				vReceiveCount++;
				TempTimeString = TempTimeString + "<" + dtoa(vReceiveCount, 0) + "<";
				
				TempSyncMessage = !TempStringToWrite.Find( _T("5A;"));
			}
			else
			if (TempMessageType == cOutputMessage)
			{
				vSendCount++;
				TempTimeString = TempTimeString + ">" + dtoa(vSendCount, 0) + ">";
				
				TempSyncMessage = !TempStringToWrite.Find( _T("5A;"));
			}
			else
				TempTimeString = TempTimeString + "-";

			CString TempString = TempTimeString + TempStringToWrite + "\n";

		
			//don't display on screen if sync and configured not to
			if ((vLogSyncData) ||
				(!TempSyncMessage))
			{
				if (vLogSerialData)
				if (vSerialDataLogFile)
				{
					TRY
					{
						CStringBufferObject *TempStringToBuffer = NULL;
						TempStringToBuffer = new CStringBufferObject;
						if (TempStringToBuffer)
						{
							//TempStringToBuffer->vTheStringLength = TempString.GetLength();
							TempStringToBuffer->vTheString = TempString;
							if (vStringCollectionLock.Lock())
							{
								vOutputStringCollection.AddTail(TempStringToBuffer);
								vStringCollectionLock.Unlock();
							}
						}
					}
					CATCH(CMemoryException,TempMemoryException)
					{
					 #ifdef _DEBUG
							afxDump << "Could Not Write Serial Log File, Out of memory"  << "\n";
					 #endif
						//vLogSerialData = false;
					}
					END_CATCH
				}
				if (vShowingSerialDataOnScreen)
				{
					if (TempMessageType == cInputMessage)
					{
						vLastDataReceived = TempString;
						::PostThreadMessage(vGlobalCallingThreadID,cDataReceivedMessage,0,0);
					}
					if (TempMessageType == cOutputMessage)
					{
						vLastDataSent = TempString;
						::PostThreadMessage(vGlobalCallingThreadID,cDataSentMessage,0,0);
					}
				}
			}
		}
		CATCH_ALL(TempFileException)
		{
		 #ifdef _DEBUG
				afxDump << "Could Not Write Serial Log File"  << "\n";
		 #endif
			//vLogSerialData = false;
		}
		END_CATCH_ALL
		//br htest
		//int TempNumberOfFilesFlushed;
		//TempNumberOfFilesFlushed = _flushall();

		vGlobalWriteToLogLock.Unlock();
	}
}

void CLogFile::LogFileObjectBackground() 
{
	if (vStringCollectionLock.Lock())
	{
		if (vSerialDataLogFile)
		{
			ULONGLONG TempLength = vSerialDataLogFile->GetLength();
			if (vSerialDataLogFile->GetLength() > 1000000)
			{
				CloseLogFile();
				OpenLogFile();
			}
			OutputStringsToFile();
		}
		vStringCollectionLock.Unlock();
	}
}

void CLogFile::OutputStringsToFile() 
{  //must lock collection and validate log file open before calling
	CStringBufferObject *TempLastStringObject = NULL;
	while (vOutputStringCollection.GetCount() > 0)
	{
		CStringBufferObject *TempStringObject = vOutputStringCollection.GetHead();
		if (TempStringObject)
		{
			if (TempLastStringObject != TempStringObject)
			{
				if (TempStringObject->vTheString)
				if (vSerialDataLogFile)
				{
					WORD TempLength = TempStringObject->vTheString.GetLength();
					if (TempLength)
					{
						CW2A TempStringToWrite(TempStringObject->vTheString);
						vSerialDataLogFile->Write(TempStringToWrite, TempLength);
					}
				}
				TempLastStringObject = TempStringObject;
				vOutputStringCollection.RemoveHead();
				delete TempStringObject;
			}
			else
			{
				vSerialDataLogFile->Write("Log Error - Pointer",19);
			}
		}
	}
}

void CLogFile::CloseLogFile() 
{
	//Open serial data log file if configured
	CFileException TempFileException;
	//vLogSerialData = false;
	if (vSerialDataLogFile)
	TRY
	{
		//vLogSerialData = false;
		if (vStringCollectionLock.Lock())
		{
			OutputStringsToFile();
			vStringCollectionLock.Unlock();
		}
		vSerialDataLogFile->Close();
		delete vSerialDataLogFile;
		vSerialDataLogFile = NULL;
	}
	CATCH_ALL(TempFileException)
	{
	 #ifdef _DEBUG
			afxDump << "Could Not Close Serial Log File"  << "\n";
	 #endif
	}
	END_CATCH_ALL
}

BYTE CLogFile::OpenLogFile() 
{
	//Open serial data log file if configured
	bool TempDidIt = false;

	double TempTotalBytesAvailable = CheckDiskSpaceFree("C:\\");
	if (TempTotalBytesAvailable > 200000000)  //200 megabytes free
	{
		CTime TempTime;
		TempTime = CTime::GetCurrentTime();
		CString TempTimeString = TempTime.Format("%Y-%m-%d-%H-%M-%S");
		CString TempDirectoryString(vLogDirectory);
		TempTimeString = TempDirectoryString + "ScanTracSerialData" + TempTimeString + ".txt";
		LPCTSTR TempFileName = TempTimeString;
		CFileException TempFileException;
		if (ThereIsEnoughMemory(sizeof(CFile), "Serial Data Log File2"))
		{
			vSendCount = 0;
			vReceiveCount = 0;
			vSerialDataLogFile = new CFile;
			if (vSerialDataLogFile)
			if (vSerialDataLogFile->Open(TempFileName, (CFile::modeCreate | CFile::modeReadWrite), &TempFileException))
			{
				TempDidIt = true;
				vLogSerialData = true;
			}
			else
			{
				delete vSerialDataLogFile;
				vSerialDataLogFile = NULL;
				//could not open serial log file
				#ifdef _DEBUG
					afxDump << "Could Not Open Serial Log File"  << "\n";
				#endif
				ReportErrorMessage("Could Not Write To Event Log",cEMailInspx, 32000);
			}
			CFileStatus TempFileStatus;
			if (vSerialDataLogFile)
			if (!(CFile::GetStatus(TempFileName, TempFileStatus)))
			{
				vSerialDataLogFile = NULL;
				TempDidIt = false;
				//vLogSerialData = false;
				ReportErrorMessage("Could Not Write To Event Log",cEMailInspx, 32000);
			}
		}
	}
	else
		ReportErrorMessage("Low Hard Disk Space, Event Log Not Recorded",cError,31999);
	return TempDidIt;
}


CStringBufferObject::CStringBufferObject()
{
	vTheString = "None";
	//vTheStringLength = 4;
}

CStringBufferObject::~CStringBufferObject()
{

}

