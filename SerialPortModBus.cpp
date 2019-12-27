//ScanTrac Side View Source File
// CSerialPortModBus.cpp : implementation file
//


#include "stdafx.h"
#include "ScanTrac.h"
#include "SerialPortReview.h"
#include "SerialPortModBus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSerialPortModBus, CWinThread)


CSerialPortModBus::CSerialPortModBus(void)
{
	vMainWindowPointer = NULL;
	vNumberOfCharactersToExpectInReply = 0;
}


CSerialPortModBus::~CSerialPortModBus(void)
{
}

BEGIN_MESSAGE_MAP(CSerialPortModBus, CWinThread)
	//{{AFX_MSG_MAP(CSerialPortModBus)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSerialPortModBus::SendSerialCommand(tSerialCommand TheData, BYTE TempLength)
{
	tSerialCommand vCommand;
	for (BYTE TempLoop = 0; TempLoop < TempLength; TempLoop++)
		vCommand[TempLoop] = TheData[TempLoop];

	WORD TempCRCValue = MakeCRC(TheData, TempLength);
	vCommand[TempLength] = (BYTE)(TempCRCValue);
	vCommand[TempLength + 1] = TempCRCValue >> 8;

	WORD TempTotalLength = TempLength + 2;

	if (vPortOpen)
	{
		DWORD TempErrorCode = 0;
		DWORD TempBytesWritten = 0;
		int TempDidIt = 0;

		TempDidIt = WriteFile(vWinPortHandle, &vCommand, TempTotalLength, &TempBytesWritten, &vWriteOverlappedStructure);
 		TempErrorCode = GetLastError(); 
		if ((TempErrorCode == ERROR_IO_PENDING) || (TempDidIt))
		{
			vWaitingForReply = true;
			vHasTransmittedCommunication = true;
			CString TempString("-");
			TempString = "";
			//Log to file if configured to
			if ((vLocalSystemData->vLogFile.vLogSerialData) || (vLocalSystemData->vLogFile.vShowingSerialDataOnScreen))
			{
				TempString = TempString + ">" + vComPortString + ">";
				
				//add the decimal values of the string on the end
				for (BYTE TempLoop = 0; TempLoop < TempTotalLength; TempLoop++)
				{
					TempString = TempString + ByteToHex(vCommand[TempLoop]) + ":";
				}
				vLocalSystemData->vLogFile.WriteToLogFile(TempString,cOutputMessage);
			}
		}
		else
		{
			TempErrorCode = GetLastError();
			CString vTempString2 = dtoa(TempErrorCode, 0);
			vTempString2 = "Com Port " + vComPortString + " Write Error: " + vTempString2;
				
			if (TempErrorCode == ERROR_NO_SYSTEM_RESOURCES)
				vTempString2 = "Port " + vComPortString + " Write Error: Insufficient System Resources, Error 1450";
			if (TempErrorCode == 1816)
				vTempString2 = "Port " + vComPortString + " Write Error: Insufficient Quota, Error 1816";
			if (TempErrorCode == ERROR_INVALID_HANDLE)
				vTempString2 = "Port " + vComPortString + " Write Error: Invalid Handle, Error 6";
			if (TempErrorCode == ERROR_ACCESS_DENIED)
				vTempString2 = "Port " + vComPortString + " Write Error: Access Denied, Error 5";
			if (TempErrorCode == ERROR_FILE_NOT_FOUND)
				vTempString2 = "Port " + vComPortString + " Write Error: Device Not Found, Error 2";
			if (TempErrorCode == ERROR_INVALID_PARAMETER)
				vTempString2 = "Port " + vComPortString + " Write Error: Invalid Parameter, Error 87";

			ReportErrorMessage(vTempString2, cEMailInspx,32000); 
			//CNoticeDialog TempNoticeDialog;
			//TempNoticeDialog.vNoticeText = vTempString2;
			//TempNoticeDialog.vType = cErrorMessage;
			//TempNoticeDialog.DoModal();

			ClosePort();
			ReportErrorMessage("Auto Closed Serial Port", cWriteToLog,0);
			OpenPort(vComPortNumber);
			ReportErrorMessage("Auto Opened Serial Port", cWriteToLog,0);
		}
	}
}
	
WORD CSerialPortModBus::MakeCRC(tSerialCommand TheData, BYTE TempLength)
{
	WORD TempCRCSum = 0xFFFF;
	for (BYTE TempLoop = 0; TempLoop < TempLength; TempLoop++)
	{
		TempCRCSum = TempCRCSum ^ TheData[TempLoop]; //^ is xor

		BYTE Temp8ShiftCount = 0;
		for (BYTE TempShiftLoop = 0; TempShiftLoop < 8; TempShiftLoop++)
		{
			bool TempNeedToShift = (TempCRCSum & 1);
			TempCRCSum = TempCRCSum >> 1;

			if (TempNeedToShift)
				TempCRCSum = TempCRCSum ^ 0xA001;
			//else
			//	TempCRCSum = TempCRCSum >> 1;
		}
	}
	return TempCRCSum;
}

bool CSerialPortModBus::SerialMessageComplete()
{
	bool TempDone = false;
	while (!TempDone)
	{
		WORD TempCount = ReceiveDataCount();
		if (vNumberOfCharactersToExpectInReply)
		{
			if (TempCount >= vNumberOfCharactersToExpectInReply)
			{
				TempDone = true;
				vWaitingForReply++;
				if (vNoCommunications)
				{
					vNoCommunications = false;
					ReportErrorMessage("Established Communication with " + vComPortString, cAction,0);
					if (vMainWindowPointer)
						vMainWindowPointer->SetInterlockStatus();
				}
				vCommunicationsTimeOutCounter = 0;
				return true;
			}
			else
			{
				TempDone = true;
				return false;
			}
		}
		else
		if (TempCount >= 5)
		{
			WORD TempIndex = vReceiveDataTail + 2;
			if (TempIndex >= cReceiveBufferSize)
				TempIndex = TempIndex - cReceiveBufferSize;

			BYTE TempDataLengthByte = vReceiveDataBuffer[TempIndex];

			if (TempCount >= 5 + TempDataLengthByte)
			{
				TempDone = true;
				vWaitingForReply++;
				if (vNoCommunications)
				{
					vNoCommunications = false;
					ReportErrorMessage("Established Communication with " + vComPortString, cAction,0);
					if (vMainWindowPointer)
						vMainWindowPointer->SetInterlockStatus();
				}
				vCommunicationsTimeOutCounter = 0;
				return true;
			}
			else
			{
				TempDone = true;
				return false;
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

void CSerialPortModBus::ShowSerialData()  //data received
{
	if ((vLocalSystemData->vLogFile.vLogSerialData) || (vLocalSystemData->vLogFile.vShowingSerialDataOnScreen))
	{
		WORD TempLength = ReceiveDataCount();

		CString TempString = "";
		TempString = "";
		BYTE TempByte = 0;
		if (TempLength > 0)
		{
			TempString = TempString + "<" + vComPortString + "<";
			
			//add the decimal values of the string on the end
			WORD TempIndex = vReceiveDataTail;
			for (BYTE TempLoop = 0; TempLoop < TempLength; TempLoop++)
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
				vLocalSystemData->vLogFile.WriteToLogFile(TempString, cInputMessage);
		}
	}
}

void CSerialPortModBus::RemoveMessageFromBuffer() 
{
	//vReceiveDataTail = vReceiveDataHead;
}

