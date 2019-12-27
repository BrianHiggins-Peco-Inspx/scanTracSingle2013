//ScanTrac Side View Source File
// SerialPortBase.cpp: implementation of the CSerialPortBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "scantrac.h"
#include "SerialPortBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CSerialPortBase, CWinThread)

CSerialPortBase::CSerialPortBase()
{
	vUse9600Baud = false;
	vConnectedDeviceVerified = false;
	vParity = cNOPARITY;
	vDeviceName = "Serial Port X";
	vCurrentReceiveCount = 0;
	vLastCommandSent = 0;
	vHasTransmittedCommunication = false;
	vHasReceivedCommunication = false;
	vWaitingForReply = false;
	vNoCommunications = true;
	vOldNoCommunications = true;
	vReceiveCount = 0;
	vSendCount = 0;
	vBaudRate = CBR_115200;
	vComPortString = "";
	vComPortNumber = 0;
	vPortOpen = false;
	vReadPortDataError = false;
	vReceiveDataHead = 0; 
	vReceiveDataTail = 0;
	vLostMessageCounter = 0;
	m_bAutoDelete = true;
	vLastCommTestStatusOK = true;
	vCommunicationsTimeOutCounter = 0;
	vPreviousMilliSeconds = 0;
	vTryNextSerialPort = 0;
}

CSerialPortBase::~CSerialPortBase()
{

}

BOOL CSerialPortBase::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return true;
}

int CSerialPortBase::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSerialPortBase, CWinThread)
	//{{AFX_MSG_MAP(CSerialPortBase)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSerialPortBase::SetBaudRate(int TempBaudRate)
{
	vBaudRate=TempBaudRate;
}

bool CSerialPortBase::OpenPort(BYTE TempPort) 
{
	if (TempPort)
	if (!vPortOpen)
	{
		vWriteOverlappedStructure.Offset = 0;
		vWriteOverlappedStructure.OffsetHigh = 0;
		vWriteOverlappedStructure.hEvent = CreateEvent( NULL,    // no security
                            true,    // explicit reset req
                            false,   // initial event reset
                            NULL ) ; // no name

		vReadOverlappedStructure.Offset = 0;
		vReadOverlappedStructure.OffsetHigh = 0;
		if (TempPort < 10)
			vComPortString.Format(_T("Com%d"),TempPort);
		else		// http://support.microsoft.com/kb/115831 info for TempComPort > Com9, for com10 or greater
			vComPortString.Format(_T("\\\\.\\COM%d"),TempPort);
		CString TempComPort(vComPortString);
		vReadOverlappedStructure.hEvent = CreateEvent( NULL,    // no security
                            true,    // explicit reset req
                            false,   // initial event reset
                            NULL ) ; // no name


		BYTE TempTrial = 0;
		if (vWinPortHandle != INVALID_HANDLE_VALUE)
		{
			ClosePort();
		}
		vWinPortHandle = INVALID_HANDLE_VALUE;
		while ((vWinPortHandle == INVALID_HANDLE_VALUE) && (TempComPort != "") && (!vLocalConfigurationData->vDemoMode))
		{
			vWinPortHandle = CreateFile(
				TempComPort, 
				GENERIC_READ | GENERIC_WRITE,
				0,
				NULL,
				OPEN_EXISTING, 
				FILE_FLAG_OVERLAPPED, 
				NULL);

			if ((vWinPortHandle == INVALID_HANDLE_VALUE) && (TempComPort != "") && (!vLocalConfigurationData->vDemoMode))// did we succeed?
			{
				TempTrial++;
				if (TempTrial > 50)//on com1 try up to 50 seconds to open the port
					TempTrial = 0;

				if (TempPort > 1)  //on comports larger than comport1, just try to open only 5 times.
				if (TempTrial > 5)
					TempTrial = 0;

				if (TempTrial)
				{
					Sleep(1000);
					//ReportErrorMessage("Trying to Open Serial Port: " + TempComPort + ", wait 1 second",cEMailInspx,32000);
				}
				else
				{
						// possibly call GetLastError() to get a hint what failed
						// and terminate (it is useless to continue if we can’t connect to Direct I/O)
					int TempError = GetLastError();
					CString vTempString2 = TempComPort + ": Port Open Error: " + dtoa(TempError, 0);
					if (TempError == ERROR_ACCESS_DENIED)
						vTempString2 = vTempString2 + ": Access Denied";
					if (TempError == ERROR_FILE_NOT_FOUND)
						vTempString2 = vTempString2 + ": Device Not Found";
					if (vComPortNumber != 4)
						ReportErrorMessage(vTempString2,cWriteToLog,0);
					else
						ReportErrorMessage(vTempString2,cEMailInspx,32000);

					TempComPort = "";
				}
			}
		}
		if ((TempComPort != "") && (!vLocalConfigurationData->vDemoMode))
		{

			vPortOpen = true;
			DCB TempDCB;
			GetCommState(vWinPortHandle,&TempDCB);
			TempDCB.BaudRate = vBaudRate;
			//CBR_38400
			//CBR_56000
			//CBR_57600
			//CBR_115200
			//CBR_128000
			//CBR_256000
			TempDCB.fParity = false;
			TempDCB.fDtrControl = DTR_CONTROL_DISABLE;
			TempDCB.ByteSize = 8;
			TempDCB.Parity = cNOPARITY;
			TempDCB.StopBits = ONESTOPBIT;
			//EscapeCommFunction();
			int TempResult = SetCommState(vWinPortHandle,&TempDCB);
			if (!TempResult)
			{
				int TempError = GetLastError();
				CString vTempString2 = "\n\n\nPort Set Parameters Error: " + dtoa(TempError, 0);
				if (TempError == ERROR_INVALID_PARAMETER)
					vTempString2 = vTempString2 + " - Invalid Parameter";
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = vTempString2;
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
			else
			{
				COMMTIMEOUTS TempCommTimout;
				int TempResult = GetCommTimeouts(vWinPortHandle,&TempCommTimout);
				TempCommTimout.ReadIntervalTimeout = 0xFFFFFFFF;//MAXDWORD;
				TempCommTimout.ReadTotalTimeoutConstant = 0;
				TempCommTimout.ReadTotalTimeoutMultiplier = 0;
				TempCommTimout.WriteTotalTimeoutConstant = 0;
				TempCommTimout.WriteTotalTimeoutMultiplier = 0;
				TempResult = SetCommTimeouts(vWinPortHandle,&TempCommTimout);
				if (!TempResult)
				{
					int TempError = GetLastError();
					CString vTempString2 = "\n\n\nPort Timeout Error: " + dtoa(TempError, 0);
					if (TempError == ERROR_INVALID_PARAMETER)
						vTempString2 = vTempString2 + " - Invalid Parameter";
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = vTempString2;
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}
				else
				{
					TempResult = SetCommMask(vWinPortHandle,EV_RXCHAR); // | EV_ERR);
					if (!TempResult)
					{
						int TempError = GetLastError();
						CString vTempString2("\n\n\nPort Comm Mask Error: " + dtoa(TempError, 0));

						if (TempError == ERROR_INVALID_PARAMETER)
							vTempString2 = vTempString2 + " - Invalid Parameter";

						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = vTempString2;
						TempNoticeDialog.vType = cErrorMessage;
						TempNoticeDialog.DoModal();
					}
					else
					{
						//create the thread that reads the com port
						if (ThereIsEnoughMemory(sizeof(CWin32SerialPortThread), "Serial Port Thread"))
						{
							vComPortThreadObject = new CWin32SerialPortThread(vWinPortHandle, 
								&vReceiveDataHead, &vReceiveDataTail, (BYTE *)(&vReceiveDataBuffer),
								GetCurrentThreadId(), &vReadOverlappedStructure, &vReadPortDataError);

							vComPortThreadObject->vSerialPortThreadID = this->m_nThreadID;

							if (!::SetThreadPriority(vComPortThreadObject->vComPortThreadHandle, THREAD_PRIORITY_HIGHEST))
								ReportErrorMessage("Set Thread Prioirty Failed", cEMailInspx, 32000);

							//vComPortThreadObject->vMySerialPortOwner = this;
							vComPortThreadObject->vSerialPortNumber = vComPortNumber;
						}
					}
				}
			}
		}
	}
	return true;
}

void CSerialPortBase::ClosePort() 
{
	if (vPortOpen)
	{
		vComPortThreadObject->CloseSerialPortReadThread();
		if (!CloseHandle(vWinPortHandle))
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nError - Close Port Failed";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			//delete vComPortThreadObject;
			vPortOpen = false;
		}
	}
}

void CSerialPortBase::RemoveMessageFromBuffer() 
{
	/*
	if (ReceiveDataCount() == 5)
		vReceiveDataTail = vReceiveDataHead;
	else
	{
		for (WORD TempLoop = 0; TempLoop < cuControllerMessageLength; TempLoop++)
		{
			if (vReceiveDataTail < cReceiveBufferSize - 1)
				vReceiveDataTail++;
			else
				vReceiveDataTail = 0;
		}
	}
	*/
}

WORD CSerialPortBase::ReceiveDataCount()
{
	if (vReceiveDataHead == vReceiveDataTail)
		return 0;
	else
	if (vReceiveDataHead > vReceiveDataTail)
		return vReceiveDataHead - vReceiveDataTail;
	else
		return cReceiveBufferSize - vReceiveDataTail + vReceiveDataHead;
}

void CSerialPortBase::ClearReceiver() 
{
  if (vPortOpen)
	{
		PurgeComm(vWinPortHandle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	}
}

void CSerialPortBase::ClearReceiveBuffer() 
{
	vReceiveDataTail = vReceiveDataHead;
}

void CSerialPortBase::ClearPort() 
{
	PurgeComm(vWinPortHandle, PURGE_RXABORT | PURGE_RXCLEAR);
}

bool CSerialPortBase::SerialMessageComplete()
{
	bool TempDone = false;
	while (!TempDone)
	{
		vCurrentReceiveCount = ReceiveDataCount();
		//if (vLocalSystemData->vLogFile.vLogSerialData)
		//	vLocalSystemData->vLogFile.WriteToLogFile("RCD: " + dtoa(TempCount,0),cDebugMessage);

		if (vCurrentReceiveCount >= cuControllerMessageLength)
		{
			WORD TempIndex = vReceiveDataTail + cuControllerMessageLength - 1;
			if (TempIndex >= cReceiveBufferSize)
				TempIndex = TempIndex - cReceiveBufferSize;
			if (vReceiveDataBuffer[TempIndex] == cETX)
			{
				TempDone = true;
				vWaitingForReply = false;
				return true;

				if (vNoCommunications)
				{
					vNoCommunications = false;
					ReportErrorMessage("Established Communication with " + vComPortString, cAction,0);
				}
			}
			else
			{
				if (!vLocalSystemData->vNoCommunicationsWithUController)
				{
					CString TempString = ByteToHex(vReceiveDataBuffer[vReceiveDataTail]);
					ReportErrorMessage("Error - No ETX - Removing one Byte: " + TempString, cWriteToLog,0);
					vTryNextSerialPort = 0;
				}
				else 
				{
					if (vTryNextSerialPort < 600)
						vTryNextSerialPort++;
				}
		
				vReceiveDataTail++;
				if (vReceiveDataTail >= cReceiveBufferSize)
					vReceiveDataTail = vReceiveDataTail - cReceiveBufferSize;
				if (vCurrentReceiveCount < cuControllerMessageLength)
				{
					TempDone = true;
					vWaitingForReply = false;
					return false;
				}
			}
		}
		else
		{
			TempDone = true;
			return false;
		}
	}
	return false;
}

void CSerialPortBase::ShowSerialData()  //data received
{
	if ((vLocalSystemData->vLogFile.vLogSerialData) ||
		(vLocalSystemData->vLogFile.vShowingSerialDataOnScreen))
	{
		CString TempString = "";
		TempString = vComPortString + ":";
		BYTE TempByte = 0;
		if (vCurrentReceiveCount > 0)
		{
			//add the decimal values of the string on the end
			WORD TempIndex = vReceiveDataTail;
			for (BYTE TempLoop = 0; TempLoop < 5; TempLoop++)
			{
				TempByte = vReceiveDataBuffer[TempIndex];
				TempString = TempString + ByteToHex(TempByte);
				if (TempLoop == 0)
					TempString = TempString + ";";
				else
					TempString = TempString + ":";
				if (TempIndex < cReceiveBufferSize - 1)
					TempIndex++;
				else
					TempIndex = 0;
			}
			//if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile(TempString,cInputMessage);
		}
	}
}

BOOL CSerialPortBase::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == cSerialReceivedDataMessage)
	{
		ProcessSerialMessage();
		return true;
	}
	if (pMsg->message == cSerialInputOverflowMessage)
	{
		ShowSerialData();
		//ReportErrorMessage("Error " + vComPortString + " Serial Input Overflow",cEMailInspx, 32000);
		ReportErrorMessage("Error " + vComPortString + " Serial Input Overflow B, Bytes: " + dtoa(pMsg->wParam,0) + ", port: " + dtoa(pMsg->lParam, 0), cEMailInspx, 32000);
		ClearReceiveBuffer();
		ClosePort();
		Sleep(5000);
		OpenPort(vComPortNumber);

		return true;
	}

	return CWinThread::PreTranslateMessage(pMsg);
}

void CSerialPortBase::ProcessSerialMessage()
{
	while (SerialMessageComplete())
	{
		//log and show data on diagnostics screen if should
		ShowSerialData();

		//decode message
		DecodeMessage();

		RemoveMessageFromBuffer();
	}
}

void CSerialPortBase::DecodeMessage()
{
}

void CSerialPortBase::DoBackGroundWork()
{
}

void CSerialPortBase::WriteCommandToLogFile(tSerialCommand& cmd, int length)
{
	int TempTotalLength = length;
	CString TempString;
	TempString = "";
	//Log to file if configured to
	if (vLocalSystemData->vLogFile.vLogSerialData)// || (vLocalSystemData->vLogFile.vShowingSerialDataOnScreen))
	{
		TempString = TempString + ">" + vComPortString + ">";

		//add the decimal values of the string on the end
		for (BYTE TempLoop = 0; TempLoop < TempTotalLength; TempLoop++)
		{
			//char TempChar = static_cast<char>(cmd[TempLoop]);
			TempString = TempString + ByteToHex(cmd[TempLoop]) + ":";
		}
		vLocalSystemData->vLogFile.WriteToLogFile(TempString, cOutputMessage);
	}
}
