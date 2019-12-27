//ScanTrac Side View Source File
// SpellmanDXMXRaySourcePort.cpp: implementation of the CSpellmanDXMXRaySourcePort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScanTrac.h"
#include "SpellmanDXMXRaySourcePort.h"
#include "SerialPortBase.h"
#include "SerialPort.h"  //for RampingPortMessage
#include "SerialPortReview.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef CompileInHTTPClientOperationsGuardian
	extern CString vGlobalOGLocationString;
	extern CString vGlobalOGLimitString;
	extern WORD vGlobalOGErrorCode;
#endif
extern DWORD vGlobalCallingThreadID;
extern CSerialPort *vGlobaluCSerialPort;
extern int vGlobalSerialPortNumberXRaySource;
extern int vGlobalXrayNoCommMessageRcvdEver;

IMPLEMENT_DYNCREATE(CSpellmanDXMXRaySourcePort, CSerialPortBase) //, CWinThread)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpellmanDXMXRaySourcePort::CSpellmanDXMXRaySourcePort()
{
	vEveryOther = false;
	vMinimumCurrent = 0;
  vRetriesAfterNoAnswer = 0;
	vCurrentMaximumScale = 1.388; //only used on 100 Watt MonoBlock
	vVoltageMaximumScale = 88.89; //only used on 100 Watt MonoBlock

	vTemperature = 0;
	vTemperatureMaximum = 0;
	vTemperatureMinimum = 0xFFFEE;
	vOldTemperature = -2;
	vOldTemperatureMaximum = -1;
	vOldTemperatureMinimum = 0xFFFF;
	vHVPSType = "DXM";
	vControllingAnMNX = false;
	vSoftwareVersion = "";
	vHardwareVersion = "";
	vWebserverVersion = "";
	vModelNumber = "";
	vComPortString = "";
	vComPortNumber = vGlobalSerialPortNumberXRaySource;	// was = 3
	vNextBackGroundTask = 0;
	vLatestResponse = "";
	vNoCommunications = true;
	v15VoltMonitor = 0;
	vHVPSTemperatureMonitor = 0;
	vFilamentCurrentFeedbackMonitor = 0;
	vFilamentVoltageFeedbackMonitor = 0;
	vFilamentCurrentMin = 9999999;
	vFilamentCurrentMax = 0;
	vOldFilamentCurrentMin = 9999999;
	vOldFilamentCurrentMax = 0;

	vMultplierTemperatureMonitor = 0;
	vHaveNewFilamentFeedbackMonitorReading = 0;
	vFilamentPreHeatMonitor = 0;
	vRemoteMode = false;
	vInterLockStatus = true;
	ClearCommandQueue();
	vMaybeSendPortMessage = false;
	ReportComNotDefaultDone = false;
}

CSpellmanDXMXRaySourcePort::~CSpellmanDXMXRaySourcePort()
{

}

CString CSpellmanDXMXRaySourcePort::GetNumberStringFromReply()
{
	WORD TempLength = vLatestResponse.GetLength();
	CString TempString = "";
	TempString = "";
	WORD TempIndex = vReceiveDataTail;

	if (TempLength > 7)
	for (WORD TempLoop = 4; TempLoop < 8; TempLoop++)
	{
		if ((vLatestResponse.GetAt(TempLoop) >= 0x30) && (vLatestResponse.GetAt(TempLoop) <= 0x39))
		{
			CString TempChar(vLatestResponse.GetAt(TempLoop));
			TempString = TempString + TempChar;
		}
		else
		if (vLatestResponse.GetAt(TempLoop) == cComma)
			TempLoop = TempLength;
	}
	return TempString;
}

CString CSpellmanDXMXRaySourcePort::GetNthNumberStringFromReply(BYTE TempWhichNumber)
{
	WORD TempLength = vLatestResponse.GetLength();
	CString TempString = "";
	TempString = "";
	WORD TempIndex = vReceiveDataTail;

	BYTE TempPosition = 0;
	for (BYTE TempLoop = 0; TempLoop < TempWhichNumber; TempLoop++)
	{
		TempPosition = vLatestResponse.Find(_T(","),TempPosition);
		TempPosition++;
	}

	if (TempLength > 7)
	for (WORD TempLoop = TempPosition; TempLoop < TempPosition + 4; TempLoop++)
	if (TempLoop < TempLength)
	{
		if ((vLatestResponse.GetAt(TempLoop) >= 0x30) && (vLatestResponse.GetAt(TempLoop) <= 0x39))
		{
			CString TempChar(vLatestResponse.GetAt(TempLoop));
			TempString = TempString + TempChar;
		}
		else
		if (vLatestResponse.GetAt(TempLoop) == cComma)
			TempLoop = 255;
	}
	return TempString;
}

//CString CSpellmanDXMXRaySourcePort::GetHexNumberStringFromReply()
//{
//	WORD TempLength = vLatestResponse.GetLength();
//	CString TempString = "";
//	TempString = "";
//	WORD TempIndex = vReceiveDataTail;
//
//	if (TempLength > 5)
//	for (WORD TempLoop = 5; TempLoop < TempLength; TempLoop++)
//	{
//		if ((vLatestResponse.GetAt(TempLoop) >= 0x30) && (vLatestResponse.GetAt(TempLoop) <= 0x39))
//		{
//			CString TempChar(vLatestResponse.GetAt(TempLoop));
//			TempString = TempString + TempChar;
//		}
//		else
//		if ((vLatestResponse.GetAt(TempLoop) >= 0x41) && (vLatestResponse.GetAt(TempLoop) <= 0x46))
//		{
//			CString TempChar(vLatestResponse.GetAt(TempLoop));
//			TempString = TempString + TempChar;
//		}
//		else
//		if (vLatestResponse.GetAt(TempLoop) == cComma)
//			TempLoop = TempLength;
//	}
//	return TempString;
//}
//

bool CSpellmanDXMXRaySourcePort::SerialMessageComplete()
{
	vLatestResponse = "";
	bool TempDone = false;
	while (!TempDone)
	{
		WORD TempCount = ReceiveDataCount();
		if (TempCount)  
		{
			WORD TempIndex = vReceiveDataTail;
			for (WORD TempLoop = 0; TempLoop < TempCount; TempLoop++)
			{
				//CString TempChar(vReceiveDataBuffer[TempIndex]);
				//BYTE TempByte = BYTE(TempChar.GetAt(0));
				vLatestResponse = vLatestResponse + (char)vReceiveDataBuffer[TempIndex];
				//11/23/2009
				if ((vReceiveDataBuffer[TempIndex] == cSpellmanETX) || ((vReceiveDataBuffer[TempIndex] == cSTX) && (TempLoop)))
					/*
				CString TempChar(vReceiveDataBuffer[TempIndex]);
				BYTE TempByte = BYTE(TempChar.GetAt(0));
				vLatestResponse = vLatestResponse + TempChar;
				if (vReceiveDataBuffer[TempIndex] == cSpellmanETX)
				*/
				{
					TempDone = true;
					vWaitingForReply = false;
					TempLoop = TempCount + 1;
					if (vNoCommunications)
					{
						vNoCommunications = false;
						vRetriesAfterNoAnswer = 0;
						ReportErrorMessage("Established Communication with " + vHVPSType + " HVPS.", cAction,0);
						//ReportErrorMessage("Established Communication with DXM HVPS", cEMailInspx,32001);
						::PostThreadMessage(vGlobalCallingThreadID,cInitializeDXMMessage,0,0);
					}
					return true;
				}
				if (TempIndex < cReceiveBufferSize - 1)
					TempIndex++;
				else
					TempIndex = 0;
			}
			if (!TempDone)//if did not get valid message, and had enough bytes
			{
				BYTE TempMaximumExpectedRecievedDataLength = 20;
				if (vControllingAnMNX)
					TempMaximumExpectedRecievedDataLength = 50;

				if (TempCount > TempMaximumExpectedRecievedDataLength)
				{
					vLatestResponse = "";
					if ((!vNoCommunications) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
					{
						CString TempString = ByteToHex(vReceiveDataBuffer[vReceiveDataTail]);
						ReportErrorMessage("Error " + vComPortString + ":No ETX - Removing one Byte: " + TempString, cWriteToLog, 0);
					}
			
					if (vReceiveDataTail < cReceiveBufferSize - 1)
						vReceiveDataTail++;
					else
						vReceiveDataTail = 0;
				}					
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

void CSpellmanDXMXRaySourcePort::RemoveMessageFromBuffer() 
{
	WORD TempCount = ReceiveDataCount();
	if (TempCount)
	{
		if (vReceiveDataBuffer[vReceiveDataTail] == cSTX)
		{  //have STX, so remove message
			for (WORD TempLoop = 0; TempLoop < TempCount; TempLoop++)
			{
				if (vReceiveDataBuffer[vReceiveDataTail] == cSpellmanETX)
					TempLoop = TempCount;
				vReceiveDataBuffer[vReceiveDataTail] = 0;  //clear the buffer so don't confuse old ETX

				if (vReceiveDataTail < cReceiveBufferSize - 1)
					vReceiveDataTail++;
				else
					vReceiveDataTail = 0;
			}
		}
		else
		{  //don't have STX, so remove one byte
			vReceiveDataBuffer[vReceiveDataTail] = 0;  //clear the buffer so don't confuse old ETX
			if (vReceiveDataTail < cReceiveBufferSize - 1)
				vReceiveDataTail++;
			else
				vReceiveDataTail = 0;
		}
	}
}

void CSpellmanDXMXRaySourcePort::ShowSerialData()  //data received
{
	if ((vLocalSystemData->vLogFile.vLogSerialData) || (vLocalSystemData->vLogFile.vShowingSerialDataOnScreen))
	{
		WORD TempLength = ReceiveDataCount();
		CString TempString = "";
		TempString = "";
		BYTE TempByte = 0;
		bool TempShowASCII = true;

		WORD TempIndex = vReceiveDataTail;
		if (TempLength)
		for (WORD TempLoop = 0; TempLoop < TempLength; TempLoop++)
		{
			TempByte = vReceiveDataBuffer[TempIndex];
			if (TempByte == cSTX)
				TempString = TempString + "<STX>";
			else
			if (TempByte == cSpellmanETX)
			{
				TempString = TempString + "<ETX>";
				TempLoop = TempLength;
			}
			else
			if (TempShowASCII)
				TempString = TempString + (char)TempByte;
			else
				TempString = TempString + "<" + ByteToHex(TempByte) + ">";

			if (TempLoop == TempLength - 3)
				TempShowASCII = false;  //show the checksum in hex

			if (TempIndex < cReceiveBufferSize - 1)
				TempIndex++;
			else
				TempIndex = 0;
		}
		TempString = vComPortString + ":" + TempString;
		vLocalSystemData->vLogFile.WriteToLogFile(TempString,cInputMessage);
	}
}

void CSpellmanDXMXRaySourcePort::DecodeMessage()
{
	WORD TempReturnValue = 0;
	CString TempReturnNumberString = "";

	tXRayReply TempResponse;
	WORD TempLength = ReceiveDataCount();
	WORD TempIndex = vReceiveDataTail;

	vHasReceivedCommunication = true;

	if (TempLength)
	if (TempLength > 50)
	{
		TempLength = 50;
		if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
			ReportErrorMessage(vHVPSType + " Error Reply too long: " + dtoa(TempLength,0), cEMailInspx, 32000);
	}

	for (WORD TempLoop = 0; TempLoop < TempLength; TempLoop++)
	{
		TempResponse[TempLoop] = vReceiveDataBuffer[TempIndex];
		if (TempIndex < cReceiveBufferSize - 1)
			TempIndex++;
		else
			TempIndex = 0;
	}

	//WORD TempLength = vLatestResponse.GetLength();
	if (TempLength == 0)
		ReportErrorMessage("Error - Latest response blank: " + vComPortString, cEMailInspx, 32000);
	else
	{
		//tXRayReply TempResponse;
		//for (BYTE TempLoop = 0; TempLoop < TempLength; TempLoop++)
		//	TempResponse[TempLoop] = vLatestResponse.GetAt(TempLoop);

		if (TempResponse[0] != cSTX)
		{
			if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
				ReportErrorMessage(vComPortString + "Error - No STX at start of message: " + (char)TempResponse[0], cWriteToLog, 0);
			TempIndex++;

			vWaitingForReply = 0;
			vLastCommandSent = 0;
			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Error No STX, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		if (TempResponse[TempLength - 1] != cSpellmanETX)
		{
			if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
				ReportErrorMessage("Error - No ETX at end of message: " + vComPortString, cWriteToLog, 0); 

			vWaitingForReply = 0;
			vLastCommandSent = 0;
			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Error Message has No ETX, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		if ((TempResponse[1] == '9') && (TempResponse[2] == '9') && (TempResponse[3] == cComma))
		{
			vGlobalXrayNoCommMessageRcvdEver = cYesCommSeen;
			if (vGlobalPortHereOK[vGlobalSerialPortNumberXRaySource] != cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER)	
			{
				vGlobalPortHereOK[vGlobalSerialPortNumberXRaySource] = cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER;
				vMaybeSendPortMessage = true;
			}
			if (TempResponse[4] != '$')
			ReportErrorMessage("Error - Remote Mode Not Acked: " + vComPortString, cEMailInspx, 32000);

			vWaitingForReply = 0;
			vLastCommandSent = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Decoded Message from 99, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		if ((TempResponse[1] == '3') && (TempResponse[2] == '1') && (TempResponse[3] == cComma))
		{
			if (TempResponse[4] != '$')
			ReportErrorMessage("Error - Clear Faults Not Acked: " + vComPortString, cEMailInspx, 32000);

			vWaitingForReply = 0;
			vLastCommandSent = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Decoded Message from 31, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		if ((TempResponse[1] == '9') && (TempResponse[2] == '8') && (TempResponse[3] == cComma))
		{
			if (TempResponse[4] != '$')
			ReportErrorMessage("Error - HVPS ON/OFF Not Acked: " + vComPortString, cEMailInspx, 32000);

			vWaitingForReply = 0;
			vLastCommandSent = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Decoded Message from 98, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		if ((TempResponse[1] == '1') && (TempResponse[2] == '0') && (TempResponse[3] == cComma))
		{
			if (TempResponse[4] != '$')
			ReportErrorMessage("Error - HVPS Set Voltage Not Acked: " + vComPortString, cEMailInspx, 32000);

			vWaitingForReply = 0;
			vLastCommandSent = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Decoded Message from 10, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		if ((TempResponse[1] == '1') && (TempResponse[2] == '1') && (TempResponse[3] == cComma))
		{
			if (TempResponse[4] != '$')
			ReportErrorMessage("Error - HVPS Set Current Not Acked: " + vComPortString, 
				cEMailInspx, 32000);

			vWaitingForReply = 0;
			vLastCommandSent = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Decoded Message from 11, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		if ((TempResponse[1] == '1') && (TempResponse[2] == '2') && (TempResponse[3] == cComma))
		{
			if (TempResponse[4] != '$')
			ReportErrorMessage("Error - HVPS Set Filament Limit Not Acked: " + vComPortString, cEMailInspx, 32000);

			vWaitingForReply = 0;
			vLastCommandSent = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Decoded Message from 12, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		if ((TempResponse[1] == '1') && (TempResponse[2] == '3') && (TempResponse[3] == cComma))
		{
			if (TempResponse[4] != '$')
			ReportErrorMessage("Error - HVPS Set Pre-heat Not Acked: " + vComPortString, cEMailInspx, 32000);

			vWaitingForReply = 0;
			vLastCommandSent = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Decoded Message from 13, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		if ((TempResponse[1] == '2') && (TempResponse[2] == '3') && (TempResponse[3] == cComma))
		{ //reply with software version number
			vCommunicationsTimeOutCounter = 0;
			vGlobalXrayNoCommMessageRcvdEver = cYesCommSeen;
			if (vGlobalPortHereOK[vGlobalSerialPortNumberXRaySource] != cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER)	
			{
				vGlobalPortHereOK[vGlobalSerialPortNumberXRaySource] = cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER;
				vMaybeSendPortMessage = true;
			}
			bool TempStringGood = true;
			for (BYTE TempLoop = 4; TempLoop < 16; TempLoop++)
			if ((TempResponse[TempLoop] < 32) || (TempResponse[TempLoop] > 126))
				TempStringGood = false;

			if (TempStringGood)
			{
				vSoftwareVersion = (char)TempResponse[4];
				for (BYTE TempLoop = 5; TempLoop < 16; TempLoop++)
					vSoftwareVersion = vSoftwareVersion + (char)TempResponse[TempLoop];

				ReportErrorMessage("Information - Read DXM Software Version: " + vSoftwareVersion, cAction, 0);
			}
			else
			{
				vSoftwareVersion = vHVPSType + " SWV: Bad Data:";
				for (BYTE TempLoop = 4; TempLoop < 16; TempLoop++)
					vSoftwareVersion = vSoftwareVersion + " 0x" + ByteToHex(TempResponse[TempLoop]);

				ReportErrorMessage("Error - " + vSoftwareVersion, cWriteToLog,0);
			}

			vWaitingForReply = 0;
			vLastCommandSent = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Decoded Message from 23, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		if ((TempResponse[1] == '2') && (TempResponse[2] == '4') && (TempResponse[3] == cComma))
		{ //reply with Hardware version number
			vCommunicationsTimeOutCounter = 0;
			vGlobalXrayNoCommMessageRcvdEver = cYesCommSeen;
			if (vGlobalPortHereOK[vGlobalSerialPortNumberXRaySource] != cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER)	
			{
				vGlobalPortHereOK[vGlobalSerialPortNumberXRaySource] = cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER;
				vMaybeSendPortMessage = true;
			}
			bool TempStringGood = true;
			for (BYTE TempLoop = 4; TempLoop < 7; TempLoop++)
			if ((TempResponse[TempLoop] < 32) || (TempResponse[TempLoop] > 126))
				TempStringGood = false;

			if (TempStringGood)
			{
				vHardwareVersion = (char)TempResponse[4];
				for (BYTE TempLoop = 5; TempLoop < 7; TempLoop++)
					vHardwareVersion = vHardwareVersion + (char)TempResponse[TempLoop];

				ReportErrorMessage("Information - Read DXM Hardware Version: " + vHardwareVersion, cAction, 0);
			}
			else
			{
				vHardwareVersion = vHVPSType + " HW Version: Bad Data:";
				for (BYTE TempLoop = 4; TempLoop < 7; TempLoop++)
					vHardwareVersion = vHardwareVersion + " 0x" + ByteToHex(TempResponse[TempLoop]);

				ReportErrorMessage("Error - " + vHardwareVersion, cWriteToLog, 0);
			}

			vWaitingForReply = 0;
			vLastCommandSent = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Decoded Message from 24, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		if ((TempResponse[1] == '2') && (TempResponse[2] == '5') && (TempResponse[3] == cComma))
		{ //reply with Webserver version number
			vCommunicationsTimeOutCounter = 0;
			vGlobalXrayNoCommMessageRcvdEver = cYesCommSeen;
			if (vGlobalPortHereOK[vGlobalSerialPortNumberXRaySource] != cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER)	
			{
				vGlobalPortHereOK[vGlobalSerialPortNumberXRaySource] = cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER;
				vMaybeSendPortMessage = true;
			}
			bool TempStringGood = true;
			for (BYTE TempLoop = 4; TempLoop < 16; TempLoop++)
			if ((TempResponse[TempLoop] < 32) || (TempResponse[TempLoop] > 126))
				TempStringGood = false;

			if (TempStringGood)
			{
				vWebserverVersion = (char)TempResponse[4];
				for (BYTE TempLoop = 5; TempLoop < 16; TempLoop++)
					vWebserverVersion = vWebserverVersion + (char)TempResponse[TempLoop];

				ReportErrorMessage("Information - Read DXM Web Server Version: " + vWebserverVersion, cAction, 0);
			}
			else
			{
				vWebserverVersion = vHVPSType + " Web: Bad Data:";
				for (BYTE TempLoop = 4; TempLoop < 16; TempLoop++)
					vWebserverVersion = vWebserverVersion + " 0x" + ByteToHex(TempResponse[TempLoop]);

				ReportErrorMessage("Error - " + vWebserverVersion, cWriteToLog, 0);
			}

			vWaitingForReply = 0;
			vLastCommandSent = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Decoded Message from 25, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		if ((TempResponse[1] == '2') && (TempResponse[2] == '6') && (TempResponse[3] == cComma))
		{ //reply with Model number
			vCommunicationsTimeOutCounter = 0;
			vGlobalXrayNoCommMessageRcvdEver = cYesCommSeen;
			if (vGlobalPortHereOK[vGlobalSerialPortNumberXRaySource] != cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER)
			{
				vGlobalPortHereOK[vGlobalSerialPortNumberXRaySource] = cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER;
				vMaybeSendPortMessage = true;
			}
			bool TempModelNumberGood = true;
			if (TempLength > 9)
			{
				for (BYTE TempLoop = 4; TempLoop < TempLength - 3; TempLoop++)
				if ((TempResponse[TempLoop] < 32) || (TempResponse[TempLoop] > 126))
					TempModelNumberGood = false;

				if (TempModelNumberGood)
				{
					vModelNumber = (char)TempResponse[4];
					for (BYTE TempLoop = 5; TempLoop < TempLength - 3; TempLoop++)
						vModelNumber = vModelNumber + (char)TempResponse[TempLoop];
				}
				else
				{
					vModelNumber = vHVPSType + " Model: Bad Data:";
					for (BYTE TempLoop = 4; TempLoop < TempLength - 3; TempLoop++)
						vModelNumber = vModelNumber + " 0x" + ByteToHex(TempResponse[TempLoop]);

					ReportErrorMessage("Error - " + vModelNumber, cWriteToLog,0);
				}

				//if (vModelNumber.Find("X3547") != -1)
				//{
				//	if (TempLength > 13)
				//		vModelNumber = "Spellman DXM>S/N:" + vModelNumber;
				//	else
				//		vModelNumber = "Spellman DXM>" + vModelNumber;
				//}

				int TempPosition = vModelNumber.Find(_T("MNX"),0);
				if (TempPosition >= 0)
				{
					vControllingAnMNX = true;
					vHVPSType = "MNX";
					vRemoteMode = true;
					if (!((vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac)))
					//if not a 50 watt system, but has NMX, then give error
						ReportErrorMessage("MNX-Piccolo Power Supply Detected, but DXM-Sideview Setting Enabled.", cError,0);
				}
				else
				{
					vControllingAnMNX = false;
					if ((vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac))
						ReportErrorMessage("DXM-Sideview Power Supply Detected, but MNX-Piccolo Setting Enabled.", cError,0);
				}
			}
			vWaitingForReply = 0;
			vLastCommandSent = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Decoded Message from 26, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();

			CompleteInitialization();
		}
		else
		if ((TempResponse[1] == '2') && (TempResponse[2] == '2') && (TempResponse[3] == cComma))
		{  //latched status data
			vWaitingForReply = 0;
			vLastCommandSent = 0;

			vCommunicationsTimeOutCounter = 0;
			if (!vLocalConfigurationData->vSimulateLightControlBoard) //must comment out to test DXM in the office
			if (TempResponse[4] == '1')
				vLocalSystemData->vXRayOnNotice = true;
			else
				vLocalSystemData->vXRayOnNotice = false;

			if (vControllingAnMNX)
			{
				if (vLocalConfigurationData->vSimulateLightControlBoard) //must comment out to test DXM in the office
					TempResponse[6] = 0;
				if (TempResponse[6] == '1')  //interlock open = 1
				{
					if (vInterLockStatus)
					{
						//if (vLocalSystemData->vSourceRampedUp)
						//if (!vLocalSystemData->vShuttingDownXRays)
						//{
						//	::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
						//	//::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						//	//ReportErrorMessage("Error - X-Ray Source Interlock Fault - Inspection Stopped", cEMailMaintenance,9);
						//	//ReportErrorMessage("Error - X-Ray Source Interlock Fault - Inspection Stopped", cEMailInspx,9);
						//}

						vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks & 0xFFDF); //clear bit 6, the x-ray interlock bit
						vInterLockStatus = false;
						::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
					}
				}
				else
				{
					if (!vInterLockStatus)
					{
						vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks | 0x0020);//set bit 6, the x-ray interlock bit
						vInterLockStatus = true;
						::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
					}
				}
			}

			if (TempResponse[8] == '1')  //got a fault of some kind
			{
				GetXRaySourceFaultData();
				Sleep(10);
				ClearXRayFault();
			}
					
			if (!vControllingAnMNX)
			{
				if (TempResponse[10] == '1')
					vRemoteMode = true;
				else
				if (vRemoteMode == 1)
				{
					vRemoteMode = false;
					ReportErrorMessage(vHVPSType + " Not in Remote.  Send Initalize", cAction,0);
					::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
#ifdef CompileInHTTPClientOperationsGuardian
					if (vLocalConfigurationData->vEnableOperationsGuardian)
					{
						vGlobalOGLocationString = "XRay";
						vGlobalOGErrorCode = 109;
						::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageXRayFault, cOGSevereMessageXRayFault);
					}
#endif
					::PostThreadMessage(vGlobalCallingThreadID,cInitializeDXMMessage,0,0);
					if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
					{
						ReportErrorMessage("Error - A/C Line Power Dropout (from DXM), Inspection Stopped.", cCritical,0);
						ReportErrorMessage("Error - A/C Line Power Dropout(from DXM), Inspection Stopped.", cError,0);
						ReportErrorMessage("Error - DXM in Local (power problem), Inspection Stopped.", cEMailInspx,32009);
					}
					else
					{
						//ReportErrorMessage("Error - A/C Line Power Dropout(from DXM).", cError,0);
						ReportErrorMessage("Error - DXM in Local (power problem).", cEMailInspx,32000);
					}
				}
			}

			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Decoded Message from 22, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		if ((TempResponse[1] == '5') && (TempResponse[2] == '5') && (TempResponse[3] == cComma))
		{  //latched status data
			vCommunicationsTimeOutCounter = 0;
			if (!vControllingAnMNX)
			{
				if (vLocalConfigurationData->vSimulateLightControlBoard) //must comment out to test DXM in the office
					TempResponse[4] = 0;

				if (TempResponse[4] == '1')  //interlock open = 1
				{
					if (vInterLockStatus)
					{
						if (vLocalSystemData->vSourceRampedUp)
						if (!vLocalSystemData->vShuttingDownXRays)
						{
							::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
							//::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
							//ReportErrorMessage("Error - X-Ray Source Interlock Fault - Inspection Stopped", cEMailMaintenance,9);
							//ReportErrorMessage("Error - X-Ray Source Interlock Fault - Inspection Stopped", cEMailInspx,9);
						}

						vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks & 0xFFDF); //clear bit 6, the x-ray interlock bit
						vInterLockStatus = false;
						::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
						//if (vGlobaluCSerialPort)
						//	vGlobaluCSerialPort->CheckInterlocks();
					}
				}
				else
				{
					if (!vInterLockStatus)
					{
						vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks | 0x0020);//set bit 6, the x-ray interlock bit
						vInterLockStatus = true;
						//vLocalSystemData->vHVPSInterlockDelay = 0;
						::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
						//if (vGlobaluCSerialPort)
						//	vGlobaluCSerialPort->CheckInterlocks();
					}
				}
			}

			vWaitingForReply = 0;
			vLastCommandSent = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Decoded Message from 55, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		if ((TempResponse[1] == '6') && (TempResponse[2] == '8') && (TempResponse[3] == cComma))
		{  //latched Fault data
			vWaitingForReply = 0;
			vLastCommandSent = 0;
			vCommunicationsTimeOutCounter = 0;
			if (TempResponse[4] == '1')
			{
				ClearXRayFault();
				if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
				{
					::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
					ReportErrorMessage(vHVPSType + " Error-Arc Fault - Inspection Stopped", cError,9);
					ReportErrorMessage(vHVPSType + " Error-Arc Fault - Inspection Stopped", cEMailInspx,9);
#ifdef CompileInHTTPClientOperationsGuardian
					if (vLocalConfigurationData->vEnableOperationsGuardian)
					{
						vGlobalOGLocationString = "XRay";
						vGlobalOGErrorCode = 103;
						::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageXRayFault, cOGSevereMessageXRayFault);
					}
#endif
				}
			}
			if (TempResponse[6] == '1')
			{
				ClearXRayFault();
				if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
				{
					::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);

					ReportErrorMessage(vHVPSType + " Error-Too Hot-Inspection Stopped. Inside Temp: " + ConvertToDegreesCString(vLocalSystemData->vTemperatureAirInside,
						vLocalConfigurationData->vTemperatureAirInsideAdjust), cError,9);

					ReportErrorMessage(vHVPSType + " Error-Too Hot-Inspection Stopped. Inside Temp: " + ConvertToDegreesCString(vLocalSystemData->vTemperatureAirInside,
						vLocalConfigurationData->vTemperatureAirInsideAdjust), cEMailInspx,9);
#ifdef CompileInHTTPClientOperationsGuardian
					if (vLocalConfigurationData->vEnableOperationsGuardian)
					{
						vGlobalOGLocationString = "xray";
						vGlobalOGErrorCode = 300;
						::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageTemperatureFault, cOGSevereMessageTemperatureFault);
					}
#endif
				}
			}
			if (TempResponse[8] == '1')
			{
				ClearXRayFault();
				if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
				{
					::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
					ReportErrorMessage(vHVPSType + " Error-Over Voltage-Inspection Stopped", cEMailInspx,9);
					ReportErrorMessage(vHVPSType + " Error-Over Voltage-Inspection Stopped", cError,9);
#ifdef CompileInHTTPClientOperationsGuardian
					if (vLocalConfigurationData->vEnableOperationsGuardian)
					{
						vGlobalOGLocationString = "XRay";
						vGlobalOGErrorCode = 107;
						::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageXRayFault, cOGSevereMessageXRayFault);
					}
#endif
				}
			}
			if (TempResponse[10] == '1')
			{
				ClearXRayFault();
				if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
				{
					::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
					ReportErrorMessage(vHVPSType + " Error - X-Ray Source Under Voltage", cEMailInspx,9);
					ReportErrorMessage(vHVPSType + " Error - X-Ray Source Under Voltage", cError,9);
#ifdef CompileInHTTPClientOperationsGuardian
					if (vLocalConfigurationData->vEnableOperationsGuardian)
					{
						vGlobalOGLocationString = "XRay";
						vGlobalOGErrorCode = 108;
						::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageXRayFault, cOGSevereMessageXRayFault);
					}
#endif
				}
			}
			if (TempResponse[12] == '1')
			{
				ClearXRayFault();
				if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
				{
					::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
					ReportErrorMessage(vHVPSType + " Error - X-Ray Source Over Current", cEMailInspx,9);
					ReportErrorMessage(vHVPSType + " Error - X-Ray Source Over Current", cError,9);
#ifdef CompileInHTTPClientOperationsGuardian
					if (vLocalConfigurationData->vEnableOperationsGuardian)
					{
						vGlobalOGLocationString = "XRay";
						vGlobalOGErrorCode = 102;
						::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageXRayFault, cOGSevereMessageXRayFault);
					}
#endif
				}
			}
			if (TempResponse[14] == '1')
			{
				ClearXRayFault();
				if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
				{
					double TempCurrentSetting = vLocalSystemData->vRampXRayCurrentAmount;
					if (vComPortNumber == 5)
						TempCurrentSetting = vLocalSystemData->vRampXRayCurrentAmount;

					if (vLocalSystemData->vXRayOnNotice)
					if (!vLocalSystemData->vSourceErrorReported)
					if (TempCurrentSetting > .5)
					{
						vLocalSystemData->vSourceErrorReported = true;
						ReportErrorMessage(vHVPSType + " Error-Under Current. Setting: " + dtoa(TempCurrentSetting, 2) + ", Reading: " + dtoa(vLocalSystemData->vSourceCurrent,2), cEMailInspx, 9);

						::PostThreadMessage(vGlobalCallingThreadID,cCheckHVPSVoltageSoonMessage,0,0);
					}
				}
			}

			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Decoded Message from 68, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		if ((TempResponse[1] == '6') && (TempResponse[2] == '0') && (TempResponse[3] == cComma))
		{  //HV Voltage
			vCommunicationsTimeOutCounter = 0;
			TempReturnNumberString = GetNumberStringFromReply();
			TempReturnValue = _wtoi(TempReturnNumberString);

			vLocalSystemData->vSourceVoltage = (double)TempReturnValue * 70.0 / 4095.0 / 6.0;

			if (vLocalSystemData->vSourceVoltage < .05)
				vLocalSystemData->vSourceVoltage = 0;

			if (vLocalConfigurationData->vSimulateLightControlBoard) //must comment out to test DXM in the office
			{
				if (vLocalSystemData->vLastXRayCommandOn)
					vLocalSystemData->vSourceVoltage = vLocalSystemData->vRampXRayVoltageAmount;
				else
					vLocalSystemData->vSourceVoltage = 0;
			}

			VerifySourceWithinTolerance();

			vWaitingForReply = 0;
			vLastCommandSent = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Decoded Message from 60, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		if ((TempResponse[1] == '6') && (TempResponse[2] == '1') && (TempResponse[3] == cComma))
		{  //HV Current
			vCommunicationsTimeOutCounter = 0;
			TempReturnNumberString = GetNumberStringFromReply();
			TempReturnValue = _wtoi(TempReturnNumberString);

			vLocalSystemData->vSourceCurrent = 	(double)TempReturnValue * 8.55 / 4095.0;
			if (vLocalSystemData->vSourceCurrent < .01)
				vLocalSystemData->vSourceCurrent = 0;

			if (vLocalConfigurationData->vSimulateLightControlBoard) //must comment out to test DXM in the office
			{
				if (vLocalSystemData->vLastXRayCommandOn)
					vLocalSystemData->vSourceCurrent = vLocalSystemData->vRampXRayCurrentAmount;
				else
					vLocalSystemData->vSourceCurrent = 0;
			}

			vWaitingForReply = 0;
			vLastCommandSent = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Decoded Message from 61, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		if ((TempResponse[1] == '6') && (TempResponse[2] == '2') && (TempResponse[3] == cComma))
		{  //Filament feedback
			vCommunicationsTimeOutCounter = 0;
			TempReturnNumberString = GetNumberStringFromReply();
			TempReturnValue = _wtoi(TempReturnNumberString);

			vFilamentCurrentFeedbackMonitor = 	(double)TempReturnValue / 4095.0 * 5.0;
			if (vHaveNewFilamentFeedbackMonitorReading < 250)
				vHaveNewFilamentFeedbackMonitorReading++;

			if (vFilamentCurrentMin > vFilamentCurrentFeedbackMonitor)
				vFilamentCurrentMin = vFilamentCurrentFeedbackMonitor;
			if (vFilamentCurrentMax < vFilamentCurrentFeedbackMonitor)
				vFilamentCurrentMax = vFilamentCurrentFeedbackMonitor;

			vWaitingForReply = 0;
			vLastCommandSent = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Decoded Message from 62, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		if ((TempResponse[1] == '6') && (TempResponse[2] == '4') && (TempResponse[3] == cComma))
		{  //Filament Preheat
			vCommunicationsTimeOutCounter = 0;
			TempReturnNumberString = GetNumberStringFromReply();
			TempReturnValue = _wtoi(TempReturnNumberString);

			vFilamentPreHeatMonitor = 	(double)TempReturnValue;

			vWaitingForReply = 0;
			vLastCommandSent = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Decoded Message from 64, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		if ((TempResponse[1] == '6') && (TempResponse[2] == '5') && (TempResponse[3] == cComma))
		{  //-15 volt monitor
			vCommunicationsTimeOutCounter = 0;
			TempReturnNumberString = GetNumberStringFromReply();
			TempReturnValue = _wtoi(TempReturnNumberString);
			
			v15VoltMonitor = 	((double)TempReturnValue / 4095.0 * 37.469); //convert from the code to volts for DXM

			vWaitingForReply = 0;
			vLastCommandSent = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Decoded Message from 65, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		if ((TempResponse[1] == '7') && (TempResponse[2] == '6') && (TempResponse[3] == cComma))
		{  //latched status data
			TempReturnNumberString = GetNumberStringFromReply();
			vCommunicationsTimeOutCounter = 0;

			vWaitingForReply = 0;
			vLastCommandSent = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Decoded Message from 76, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		if ((TempResponse[1] == '2') && (TempResponse[2] == '0') && (TempResponse[3] == cComma)) //MNX reply to ADC Montitor readouts
		{
			vCommunicationsTimeOutCounter = 0;
			TempReturnNumberString = GetNthNumberStringFromReply(1);
			TempReturnValue = _wtoi(TempReturnNumberString);
			vHVPSTemperatureMonitor = (double)TempReturnValue;

			TempReturnNumberString = GetNthNumberStringFromReply(2);
			TempReturnValue = _wtoi(TempReturnNumberString);
			v15VoltMonitor = 	(((double)TempReturnValue / 4095) * 43);  //on MNX supply is actually 24 Volts

			TempReturnNumberString = GetNthNumberStringFromReply(3);
			TempReturnValue = _wtoi(TempReturnNumberString);
			//vLocalSystemData->vSourceVoltage = (double)TempReturnValue * 50.0 / 4095.0 / 6.0;
			vLocalSystemData->vSourceVoltage = (double)TempReturnValue * 12.21 / 1000.0 / 6.0;

			TempReturnNumberString = GetNthNumberStringFromReply(4);
			TempReturnValue = _wtoi(TempReturnNumberString);
			//vLocalSystemData->vSourceCurrent = (double)TempReturnValue * 2.0 / 4095.0;
			vLocalSystemData->vSourceCurrent = (double)TempReturnValue * 0.0005861;//convert code units to mA
			VerifySourceWithinTolerance();

			if (vLocalConfigurationData->vSimulateLightControlBoard) //must comment out to test DXM in the office
			{
				if (vLocalSystemData->vLastXRayCommandOn)
				{
					vLocalSystemData->vSourceVoltage = vLocalSystemData->vRampXRayVoltageAmount;
					vLocalSystemData->vSourceCurrent = vLocalSystemData->vRampXRayCurrentAmount;
				}
				else
				{
					vLocalSystemData->vSourceVoltage = 0;
					vLocalSystemData->vSourceCurrent = 0;
				}
			}

			TempReturnNumberString = GetNthNumberStringFromReply(5);
			TempReturnValue = _wtoi(TempReturnNumberString);
			//vFilamentCurrentFeedbackMonitor = (double)TempReturnValue;
			vFilamentCurrentFeedbackMonitor = (double)TempReturnValue * 0.000879; //convert code units to mA
			if (vHaveNewFilamentFeedbackMonitorReading < 250)
				vHaveNewFilamentFeedbackMonitorReading++;

			TempReturnNumberString = GetNthNumberStringFromReply(6);
			TempReturnValue = _wtoi(TempReturnNumberString);
			vFilamentVoltageFeedbackMonitor = (double)TempReturnValue * 0.001343;//convert code units to Volts

			if (vFilamentCurrentFeedbackMonitor < vFilamentCurrentMin)
				vFilamentCurrentMin = vFilamentCurrentFeedbackMonitor;
			if (vFilamentCurrentFeedbackMonitor > vFilamentCurrentMax)
				vFilamentCurrentMax = vFilamentCurrentFeedbackMonitor;

			TempReturnNumberString = GetNthNumberStringFromReply(7);
			TempReturnValue = _wtoi(TempReturnNumberString);
			vMultplierTemperatureMonitor = (double)TempReturnValue;

			vWaitingForReply = 0;
			vLastCommandSent = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Decoded Message from 20, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		if ((TempResponse[1] == '9') && (TempResponse[2] == '5'))
		{  //Reply to filament limit
			vWaitingForReply = 0;
			vLastCommandSent = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempCString;
				TempCString.Format(_T("Com%d-DXM-Decoded Message from 95, Pop off next command"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
		else
		{
			ReportErrorMessage(vHVPSType + " Error-Unknown Reply: " + vLatestResponse, cEMailInspx, 32000);
		}
		if ((vMaybeSendPortMessage == true) && (ReportComNotDefaultDone == false))
		{
			vMaybeSendPortMessage = false;
			ReportComNotDefaultDone = true;
			if (vComPortNumber != cDEFAULT_XRAY_SERIAL_PORT_NUMBER)
			{
				CString TempAString;
				TempAString.Format(_T("Error-XRay Com Port %d, Default Com 3"),vGlobalSerialPortNumberXRaySource);
				ReportErrorMessage(TempAString, cEMailInspx,32000);
//				ReportErrorMessage(TempAString, cError,0);
			}
		}
	}
	vLatestResponse = "";
}

void CSpellmanDXMXRaySourcePort::VerifySourceWithinTolerance()
{
	// if source is on check voltage and current not high
	if (vLocalSystemData->vSourceRampedUp)
	if (!vLocalSystemData->vShuttingDownXRays)
	if (vLocalSystemData->vSystemRunMode != cCalibratingDetectorOffsetMode)
	if (!vLocalSystemData->vAdjustingSourceSettings)
	//if (!vLocalConfigurationData->vSimulateLightControlBoard)
	if (vLocalSystemData->vRampXRayVoltageAmount > 19 / 6)
	if (vLocalSystemData->vRampXRayCurrentAmount > .2)
	{
		if (vLocalSystemData->vSourceVoltage > vLocalSystemData->vSourceVoltageMax)
			vLocalSystemData->vSourceVoltageMax = vLocalSystemData->vSourceVoltage;
		if (vLocalSystemData->vSourceVoltage < vLocalSystemData->vSourceVoltageMin)
			vLocalSystemData->vSourceVoltageMin = vLocalSystemData->vSourceVoltage;
		if (vLocalSystemData->vSourceCurrent > vLocalSystemData->vSourceCurrentMax)
			vLocalSystemData->vSourceCurrentMax = vLocalSystemData->vSourceCurrent;
		if (vLocalSystemData->vSourceCurrent < vLocalSystemData->vSourceCurrentMin)
			vLocalSystemData->vSourceCurrentMin = vLocalSystemData->vSourceCurrent;

		double DeltaV = (vLocalSystemData->vSourceVoltage - vLocalSystemData->vRampXRayVoltageAmount) * 6;
		double DeltaC = (vLocalSystemData->vSourceCurrent - (vLocalSystemData->vRampXRayCurrentAmount));
		if (DeltaV < 0)
			DeltaV = - DeltaV;
		if (DeltaC < 0)
			DeltaC = - DeltaC;

		double TempSourceVoltageTolerance = .1 * vLocalSystemData->vSourceVoltage * 6;
		double TempSourceCurrentTolerance = .1 * vLocalSystemData->vSourceCurrent * 6;
		if (TempSourceCurrentTolerance < .25)
			TempSourceCurrentTolerance = .25;

		if ((DeltaV > TempSourceVoltageTolerance) || (DeltaC > TempSourceCurrentTolerance))
		{
			//shut down the source
			if (!vLocalConfigurationData->vSimulateLightControlBoard)
			{
				if (!vLocalSystemData->vSourceErrorReported)
				if (vLocalSystemData->vSystemRunMode == cConditionXRayTubeMode)
				{
					vLocalSystemData->vXRayTubeConditionGlitchCount++;
					if (DeltaV > TempSourceVoltageTolerance)
					{
						ReportErrorMessage("Notice-X-Ray/Supply glitch: " + dtoa(vLocalSystemData->vXRayTubeConditionGlitchCount,0) + ", Delta Voltage: " + dtoa(DeltaV,2) + ", Delta Current: " + dtoa(DeltaC,2), cEMailMaintenance,0);
						ReportErrorMessage("Notice-X-Ray/Supply glitch: " + dtoa(vLocalSystemData->vXRayTubeConditionGlitchCount,0) + ", Delta Voltage: " + dtoa(DeltaV,2) + ", Delta Current: " + dtoa(DeltaC,2), cEMailInspx, 32000);
					}
					vLocalSystemData->vSourceErrorReported = true;
					vLocalSystemData->vXRayTubeConditionCurrentStep = -1;//set so will start over
					vLocalSystemData->vXRayTubeConditionStepTimeLeft = 1;//set so next second will do next step

					if (vLocalSystemData->vXRayTubeConditionGlitchCount > 2)
					{
						if (DeltaV > TempSourceVoltageTolerance)
						{
							ReportErrorMessage("Error-X-Ray Tube " + vLocalSystemData->vConditionString + " Failed, Call Service", cError,0);
							ReportErrorMessage("Error-X-Ray Tube " + vLocalSystemData->vConditionString + " Failed, Call Service", cEMailInspx, 32000);
						}
						vLocalSystemData->vXRayTubeConditionCurrentStep = 12;//set step so will end
					}

				}
				else
				//if (!vLocalSystemData->vSourceErrorReported)
				{
					//if (vLocalConfigurationData->vAutoXRaySourceOffEnabled)
					//	::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);

					vLocalSystemData->vSourceErrorReported = true;
					if ((DeltaV > TempSourceVoltageTolerance) && (vLocalSystemData->vSourceVoltage > vLocalSystemData->vRampXRayVoltageAmount))
					{
						//ReportErrorMessage("Error-X-Ray voltage too high.", cEMailMaintenance,0);
						ReportErrorMessage("Error-X-Ray voltage too high. Voltage: " + dtoa(vLocalSystemData->vSourceVoltage * 6,2) + " Current: " + dtoa(vLocalSystemData->vSourceCurrent,2),cEMailInspx,32000);

						::PostThreadMessage(vGlobalCallingThreadID,cCheckHVPSVoltageSoonMessage,0,0);
#ifdef CompileInHTTPClientOperationsGuardian
						if (vLocalConfigurationData->vEnableOperationsGuardian)
						{
							vGlobalOGLocationString = "XRay";
							vGlobalOGErrorCode = 107;
							::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageXRayFault, cOGSevereMessageXRayFault);
						}
#endif
					}
					else
					if ((DeltaC > TempSourceCurrentTolerance) && 
						(vLocalSystemData->vSourceCurrent > 
							vLocalSystemData->vRampXRayCurrentAmount))
					{
						//ReportErrorMessage("Error-X-Ray current too high.", cEMailMaintenance,0);
						ReportErrorMessage("Error-X-Ray current too high. Voltage: " + dtoa(vLocalSystemData->vSourceVoltage * 6,2) + " Current: " + dtoa(vLocalSystemData->vSourceCurrent,2),cEMailInspx,32000);
						::PostThreadMessage(vGlobalCallingThreadID,cCheckHVPSVoltageSoonMessage,0,0);
					}
					else
					if (DeltaV > TempSourceVoltageTolerance)
					{
						//ReportErrorMessage("Error-X-Ray voltage too low.", cEMailMaintenance,0);
						ReportErrorMessage("Error-X-Ray voltage too low. Voltage: " + dtoa(vLocalSystemData->vSourceVoltage * 6,2) + " Current: " + dtoa(vLocalSystemData->vSourceCurrent,2),cEMailInspx,32000);
						::PostThreadMessage(vGlobalCallingThreadID,cCheckHVPSVoltageSoonMessage,0,0);
					}
					else
					if (DeltaC > TempSourceCurrentTolerance)
					{
						//ReportErrorMessage("Error-X-Ray current too low.", cEMailMaintenance,0);
						ReportErrorMessage("Error-X-Ray current too low. Voltage: " + dtoa(vLocalSystemData->vSourceVoltage * 6,2) + " Current: " + dtoa(vLocalSystemData->vSourceCurrent,2),cEMailInspx,32000);

						::PostThreadMessage(vGlobalCallingThreadID,cCheckHVPSVoltageSoonMessage,0,0);

						/*
						if ((vLocalSystemData->vSourceCurrent < .1) && 
							(vLocalSystemData->vRampXRayCurrentAmount > .5) &&
							(vLocalSystemData->vRampXRayVoltageAmount > 19 / 6) && 
							(vLocalSystemData->vSourceVoltage > 19 / 6) &&
							(DeltaV < 5))
						{
							ReportErrorMessage("Error-No X-Ray Current", cEMailInspx,32000);
						}
						*/
					}
				}
			}
		}
		else
		if (vLocalSystemData->vSourceErrorReported)
		{
			vLocalSystemData->vSourceErrorReported = false;
			ReportErrorMessage("Notice-X-Ray source normal after event",cEMailInspx,32000);
			//ReportErrorMessage("Notice-X-Ray source normal after event",cError,0);
		}
	}
}

void CSpellmanDXMXRaySourcePort::TurnOnOffXRays(BYTE TempOn)
{ 
	if ((TempOn) && ((vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac)) && 
		(!vControllingAnMNX))
		ReportErrorMessage(vHVPSType + "-Sideview Power Supply Detected, but MNX-Piccolo Setting Enabled.", cError,0);
	else
	if ((TempOn) && (!((vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac))) && 
		(vControllingAnMNX))
		ReportErrorMessage("MNX-Piccolo Power Supply Detected, but Solo ScanTrac Setting Enabled.", cError,0);
	else
	{
		tXRayCommand TempCommand;
		TempCommand[1] = (BYTE)'9';
		if (vControllingAnMNX) //MNX is command 99, DXM is command 98
			TempCommand[2] = (BYTE)'9';
		else
			TempCommand[2] = (BYTE)'8';
		TempCommand[3] = cComma;
		TempCommand[4] = (BYTE)'0';
		if (TempOn)
			TempCommand[4] = (BYTE)'1';
		SendXRayCommand(TempCommand, 4, 98);
	}
}

void CSpellmanDXMXRaySourcePort::TurnOnXRayWatchDog()
{
	/*
	TempCommand[1] = (BYTE)'A';
	TempCommand[2] = (BYTE)'0';
	TempCommand[3] = (BYTE)'B';
	TempCommand[4] = (BYTE)'0';
	TempCommand[5] = (BYTE)'0';
	TempCommand[6] = (BYTE)'1';
	TempCommand[7] = (BYTE)'0';
	SendXRayCommand(TempCommand, 7, 0);
	*/
}

void CSpellmanDXMXRaySourcePort::TickleXRayWatchDog()
{
	/*
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'A';
	TempCommand[2] = (BYTE)'0';
	TempCommand[3] = (BYTE)'A';
	TempCommand[4] = (BYTE)'0';
	TempCommand[5] = (BYTE)'0';
	TempCommand[6] = (BYTE)'0';
	TempCommand[7] = (BYTE)'0';
	if (vLocalSystemData->vLogFile.vLogSyncData)
		SendXRayCommand(TempCommand, 7, 0);
		*/
}

void CSpellmanDXMXRaySourcePort::GetXRaySourceFaultData()
{
	tXRayCommand TempCommand;
	if (vControllingAnMNX)
	{
		TempCommand[1] = (BYTE)'2';
		TempCommand[2] = (BYTE)'2';
	}
	else
	{
		TempCommand[1] = (BYTE)'6';
		TempCommand[2] = (BYTE)'8';
	}
	SendXRayCommand(TempCommand, 2, 68);
}

void CSpellmanDXMXRaySourcePort::ClearXRayFault()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'3';
	TempCommand[2] = (BYTE)'1';
	SendXRayCommand(TempCommand, 2, 31);
}

void CSpellmanDXMXRaySourcePort::SetXRayVoltage(double TempVoltage)
{
	//input is 0 to 10 representing 0 to 60 kV
	//output is 0 to 4095 representing 0 to 70 kV
	if (TempVoltage > 12)
		TempVoltage = 12;
	double TempCorrectedVoltage = 0;
	if (vControllingAnMNX)
		TempCorrectedVoltage = (TempVoltage * 6 * 1000.0) / 12.21; //MNX  12.21 Volts per step in output code, convert 0 to 10 to 0 to 50,000
	else
		TempCorrectedVoltage = TempVoltage / 10.0 * 60.0 / 70.0 * 4095.0; //DXM

	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'1';
	TempCommand[2] = (BYTE)'0';
	TempCommand[3] = cComma;
	TempCommand[4] = (BYTE)'0';
	TempCommand[5] = (BYTE)'0';
	TempCommand[6] = (BYTE)'0';
	TempCommand[7] = (BYTE)'0';
	CString TempString = dtoa(TempCorrectedVoltage,0);
	BYTE TempLength = TempString.GetLength();
	for (BYTE TempLoop = 0; TempLoop < TempLength; TempLoop++)
		TempCommand[8 - TempLength + TempLoop] = (BYTE)TempString.GetAt(TempLoop);
	SendXRayCommand(TempCommand, 7, 10);
}

void CSpellmanDXMXRaySourcePort::SetFilamentLimitToDefault()
{
	tXRayCommand TempCommand;
	if (vControllingAnMNX)
	{
		TempCommand[1] = (BYTE)'1';
		TempCommand[2] = (BYTE)'3';
		TempCommand[3] = cComma;
		TempCommand[4] = (BYTE)'1';
		TempCommand[5] = (BYTE)'3';
		TempCommand[6] = (BYTE)'5';
		TempCommand[7] = (BYTE)'1';
		SendXRayCommand(TempCommand, 7, 12);
	}
	else
	{
		TempCommand[1] = (BYTE)'1';
		TempCommand[2] = (BYTE)'2';
		TempCommand[3] = cComma;
		TempCommand[4] = (BYTE)'4';
		TempCommand[5] = (BYTE)'0';
		TempCommand[6] = (BYTE)'9';
		TempCommand[7] = (BYTE)'5';
		SendXRayCommand(TempCommand, 7, 12);
	}
}

void CSpellmanDXMXRaySourcePort::SetFilamentLimitToZero()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'1';
	if (vControllingAnMNX)
		TempCommand[2] = (BYTE)'3';
	else
		TempCommand[2] = (BYTE)'2';
	TempCommand[3] = cComma;
	TempCommand[4] = (BYTE)'0';
	TempCommand[5] = (BYTE)'0';
	TempCommand[6] = (BYTE)'0';
	TempCommand[7] = (BYTE)'0';
	SendXRayCommand(TempCommand, 7, 12);
}

void CSpellmanDXMXRaySourcePort::SetXRayCurrent(double TempCurrent)
{
	//current value is scale of 0 to 10 for 0 to 10 milli amps
	//need to send value of 0 to 4095 for 0 to 8.56 milli amps
	if (TempCurrent > 10)
		TempCurrent = 10;
	double TempCorrectedCurrent = 0;
	if (vControllingAnMNX)
	{
		if (TempCurrent > 1.99)
			TempCurrent = 1.99;
		TempCorrectedCurrent = TempCurrent / .000488; //MNX  488 uA per step in output code
	}
	else
		TempCorrectedCurrent = TempCurrent / 10.0 / 8.55 * 10.0 * 4095.0; //DXM TempCurrent 1 = 1 mA

	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'1';
	TempCommand[2] = (BYTE)'1';
	TempCommand[3] = cComma;
	TempCommand[4] = (BYTE)'0';
	TempCommand[5] = (BYTE)'0';
	TempCommand[6] = (BYTE)'0';
	TempCommand[7] = (BYTE)'0';
	CString TempString = dtoa(TempCorrectedCurrent,0);
	BYTE TempLength = TempString.GetLength();
	for (BYTE TempLoop = 0; TempLoop < TempLength; TempLoop++)
		TempCommand[8 - TempLength + TempLoop] = (BYTE)TempString.GetAt(TempLoop);
	SendXRayCommand(TempCommand, 7, 11);
}

void CSpellmanDXMXRaySourcePort::SetXRayFilamentLimit(double TempLimit)
{
	//current value is scale of 0 to 1 for 0 to 5 amps
	//need to send value of 0 to 4095 for 0 to 5 amps
	if (TempLimit > 1)
		TempLimit = 1;
	double TempCorrectedLimit = TempLimit * 4095.0;
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'1';
	if (vControllingAnMNX)  //MNX command 13, DXM command 12
		TempCommand[2] = (BYTE)'3';
	else
		TempCommand[2] = (BYTE)'2';
	TempCommand[3] = cComma;
	TempCommand[4] = (BYTE)'0';
	TempCommand[5] = (BYTE)'0';
	TempCommand[6] = (BYTE)'0';
	TempCommand[7] = (BYTE)'0';
	CString TempString = dtoa(TempCorrectedLimit,0);
	BYTE TempLength = TempString.GetLength();
	for (BYTE TempLoop = 0; TempLoop < TempLength; TempLoop++)
		TempCommand[8 - TempLength + TempLoop] = (BYTE)TempString.GetAt(TempLoop);
	SendXRayCommand(TempCommand, 7, 12);
}

void CSpellmanDXMXRaySourcePort::SetXRayFilamentPreHeat(double TempPreHeat)
{
	//current value is scale of 0 to 1 for 0 to 2.5 amps
	//need to send value of 0 to 4095 for 0 to 2.5 amps
	if (TempPreHeat > 1)
		TempPreHeat = 1;
	double TempCorrectedPreHeat = TempPreHeat * 4095.0;
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'1';
	if (vControllingAnMNX)  //MNX command 12, DXM command 3
		TempCommand[2] = (BYTE)'2';
	else
		TempCommand[2] = (BYTE)'3';
	TempCommand[3] = cComma;
	TempCommand[4] = (BYTE)'0';
	TempCommand[5] = (BYTE)'0';
	TempCommand[6] = (BYTE)'0';
	TempCommand[7] = (BYTE)'0';
	CString TempString = dtoa(TempCorrectedPreHeat,0);
	BYTE TempLength = TempString.GetLength();
	for (BYTE TempLoop = 0; TempLoop < TempLength; TempLoop++)
		TempCommand[8 - TempLength + TempLoop] = (BYTE)TempString.GetAt(TempLoop);
	SendXRayCommand(TempCommand, 7, 13);
}


void CSpellmanDXMXRaySourcePort::GetXRayVoltage()
{
	tXRayCommand TempCommand;
	if (vControllingAnMNX)  //MNX read out generic Analog Monitor Status data on 7 channels, DXM read -15 V Monitor
	{
		TempCommand[1] = (BYTE)'2';
		TempCommand[2] = (BYTE)'0';
	}
	else
	{
		TempCommand[1] = (BYTE)'6';
		TempCommand[2] = (BYTE)'0';
	}
	SendXRayCommand(TempCommand, 2, 60);
}

void CSpellmanDXMXRaySourcePort::GetXRayCurrent()
{
	tXRayCommand TempCommand;
	if (vControllingAnMNX)  //MNX read out generic Analog Monitor Status data on 7 channels, DXM read -15 V Monitor
	{
		TempCommand[1] = (BYTE)'2';
		TempCommand[2] = (BYTE)'0';
	}
	else
	{
		TempCommand[1] = (BYTE)'6';
		TempCommand[2] = (BYTE)'1';
	}
	SendXRayCommand(TempCommand, 2, 61);
}

void CSpellmanDXMXRaySourcePort::GetXRayStatus()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'2';
	TempCommand[2] = (BYTE)'2';
	SendXRayCommand(TempCommand, 2, 22);
}

void CSpellmanDXMXRaySourcePort::GetFilamentFeedback()
{
	tXRayCommand TempCommand;
	if (vControllingAnMNX)  //MNX read out generic Analog Monitor Status data on 7 channels, DXM read specific Monitor
	{
		TempCommand[1] = (BYTE)'2';
		TempCommand[2] = (BYTE)'0';
	}
	else
	{
		TempCommand[1] = (BYTE)'6';
		TempCommand[2] = (BYTE)'2';
	}
	SendXRayCommand(TempCommand, 2, 62);
}
/* this command is not implemented in the DXM
void CSpellmanDXMXRaySourcePort::GetFilamentPreHeat()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'6';
	TempCommand[2] = (BYTE)'4';
	SendXRayCommand(TempCommand, 2, 64);
}
*/
void CSpellmanDXMXRaySourcePort::Get15VoltMonitor()
{
	tXRayCommand TempCommand;
	if (vControllingAnMNX)  //MNX read out generic Analog Monitor Status data on 7 channels, DXM read -15 V Monitor
	{
		TempCommand[1] = (BYTE)'2';
		TempCommand[2] = (BYTE)'0';
	}
	else
	{
		TempCommand[1] = (BYTE)'6';
		TempCommand[2] = (BYTE)'5';
	}

	SendXRayCommand(TempCommand, 2, 65);
}

void CSpellmanDXMXRaySourcePort::GetInterlockStatus()
{
	tXRayCommand TempCommand;
	if (vControllingAnMNX)
	{
		TempCommand[1] = (BYTE)'2';
		TempCommand[2] = (BYTE)'2';
	}
	else
	{
		TempCommand[1] = (BYTE)'5';
		TempCommand[2] = (BYTE)'5';
	}
	SendXRayCommand(TempCommand, 2, 55);
}

void CSpellmanDXMXRaySourcePort::GetSoftwareVersion()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'2';
	TempCommand[2] = (BYTE)'3';
	SendXRayCommand(TempCommand, 2, 23);
}

void CSpellmanDXMXRaySourcePort::GetHardwareVersion()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'2';
	TempCommand[2] = (BYTE)'4';
	SendXRayCommand(TempCommand, 2, 24);
}

void CSpellmanDXMXRaySourcePort::GetWebServerVersion()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'2';
	TempCommand[2] = (BYTE)'5';
	SendXRayCommand(TempCommand, 2, 25);
}

void CSpellmanDXMXRaySourcePort::GetModelNumber()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'2';
	TempCommand[2] = (BYTE)'6';
	SendXRayCommand(TempCommand, 2, 26);
}

void CSpellmanDXMXRaySourcePort::SetInRemoteMode(bool TempRemote)
{
	if (!vControllingAnMNX) //DXM has local/remote mode, MNX is always in remote mode
	{
		tXRayCommand TempCommand;
		TempCommand[1] = (BYTE)'9';
		TempCommand[2] = (BYTE)'9';
		TempCommand[3] = (BYTE)',';
		if (TempRemote)
			TempCommand[4] = (BYTE)'1';
		else
			TempCommand[4] = (BYTE)'0';
		SendXRayCommand(TempCommand, 4, 99);
	}
}

void CSpellmanDXMXRaySourcePort::DoBackGroundWork()
{
	//called at 10 hz
	if (vWaitingForReply)
	{
		if (!vLocalConfigurationData->vSimulateLightControlBoard) //comment out to test DXM in the office
		if (vLocalSystemData->vACPowerOn)
		{
			if (vWaitingForReply < 250)
				vWaitingForReply++;
			if (vWaitingForReply == 20)
			{
				ReportErrorMessage(vHVPSType + " Timeout waiting for reply, try next: " + vComPortString, cWriteToLog, 0);
				vWaitingForReply = 0;
				vLastCommandSent = 0;
				PopFirstCommandOffQueue();
				if (!vNoCommunications)
				if (vRetriesAfterNoAnswer < 250)
					vRetriesAfterNoAnswer++;
			}

			if (vWaitingForReply > 2)
			if (!vNoCommunications)
			if (vLocalSystemData->vACPowerOn)
			if (!vLocalConfigurationData->vSimulateLightControlBoard) //must comment out to test monoblock in the office
			{
				WORD TempLength = ReceiveDataCount();
				if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
					ReportErrorMessage("Waiting for HVPS Reply: " + dtoa(vWaitingForReply,0) + " Bytes RCVD: " + dtoa(TempLength, 0), cWriteToLog, 0);
				if (TempLength)
					ProcessSerialMessage();
			}
		}
	}
	else
	if (vNextBackGroundTask == 0)
	{
		GetFilamentFeedback();
		vNextBackGroundTask++;
	}
	else
	if (vNextBackGroundTask == 1)
	{
		GetXRayCurrent();
		vNextBackGroundTask++;
	}
	else
	if (vNextBackGroundTask == 2)
	{
		GetXRayVoltage();
		vNextBackGroundTask++;
	}
	else
	if (vNextBackGroundTask == 3)
	{
		GetXRayStatus();
		vNextBackGroundTask = 4;
	}
	else
	if (vNextBackGroundTask == 4)
	{
		Get15VoltMonitor();
		vNextBackGroundTask = 5;
	}
	else
	if (vNextBackGroundTask == 5)
	{
		GetInterlockStatus();
		vNextBackGroundTask = 6;
	}
	else
	if (vNextBackGroundTask == 6)
	{
		GetXRaySourceFaultData();
		vNextBackGroundTask = 7;
	}
	else
		vNextBackGroundTask = 0;
}

void CSpellmanDXMXRaySourcePort::SendXRayCommand(tXRayCommand TempData, BYTE TempNumberOfBytes, BYTE TempCommandSending)
{
	if (vWaitingForReply)
	{
		if (!vNoCommunications)
		{
			BYTE TempIndex = GetFirstEmptyCommandQueueSpot();
			if (TempIndex < 0xFF)
			{
				for (BYTE TempLoop = 1; TempLoop <= TempNumberOfBytes;TempLoop++)
					vCommandQueue[TempIndex][TempLoop] = TempData[TempLoop];
				vCommandQueue[TempIndex][0] = TempNumberOfBytes;
				vCommandQueueCommand[TempIndex] = TempCommandSending;

				if (vLocalSystemData->vLogFile.vLogSerialData)
				{
					CString CStringATemp;
					CStringATemp.Format(_T("-Com%d-Queue up command: %d"),vGlobalSerialPortNumberXRaySource,TempCommandSending);
					vLocalSystemData->vLogFile.WriteToLogFile(this->vHVPSType + CStringATemp, cDebugMessage);
				}
			}
			else
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString CStringATemp;
				CStringATemp.Format(_T("-Error-Com%d-Waiting for Reply and Queue Full, ignoring command: %d"),vGlobalSerialPortNumberXRaySource,TempCommandSending);
				vLocalSystemData->vLogFile.WriteToLogFile(this->vHVPSType +  CStringATemp, cDebugMessage);
			}
		}
	}
	else
	if (TempNumberOfBytes < 17)
	{
		if (vLocalSystemData->vLogFile.vLogSerialData)
		{
			CString CStringATemp;
			CStringATemp.Format(_T("-Com%d-Send command: %d"),vGlobalSerialPortNumberXRaySource,TempCommandSending);
			vLocalSystemData->vLogFile.WriteToLogFile(this->vHVPSType + CStringATemp, cDebugMessage);
		}

		vWaitingForReply = 1;
		tXRayCommand TempCommand;
		TempCommand[0] = cSTX;
		for (BYTE TempLoop = 1; TempLoop <= TempNumberOfBytes; TempLoop++)
			TempCommand[TempLoop] = TempData[TempLoop];
		TempCommand[TempNumberOfBytes + 1] = (BYTE)',';
		TempCommand[TempNumberOfBytes + 2] = CalculateCheckSum(TempCommand, TempNumberOfBytes);
		TempCommand[TempNumberOfBytes + 3] = cSpellmanETX;

		BYTE TempCommandLength = TempNumberOfBytes + 4;
		if (vPortOpen)
		{
			DWORD TempErrorCode = 0;
			DWORD TempBytesWritten = 0;
			int TempDidIt = 0;
			bool TempShowASCII = true;

			TempDidIt = WriteFile(vWinPortHandle, &TempCommand, TempCommandLength, &TempBytesWritten, &vWriteOverlappedStructure);
 			TempErrorCode = GetLastError(); 
			if ((TempErrorCode == ERROR_IO_PENDING) || (TempDidIt))
			{
				vHasTransmittedCommunication = true;
				CString TempString("");
				TempString = "";
				//Log to file if configured to
				if ((vLocalSystemData->vLogFile.vLogSerialData) || (vLocalSystemData->vLogFile.vShowingSerialDataOnScreen))
				{
					//add the decimal values of the string on the end
					for (BYTE TempLoop = 0; TempLoop < TempCommandLength; TempLoop++)
					{
						BYTE TempByte = TempCommand[TempLoop];
						if (TempByte == cSTX)
							TempString = TempString + "<STX>";
						else
						if (TempByte == cSpellmanETX)
							TempString = TempString + "<ETX>";
						else
						if (TempByte == cComma)
						{
							TempString = TempString + ",";
						}
						else
						if (TempShowASCII)
							TempString = TempString + (char)TempByte;
						else
							TempString = TempString + "<" + ByteToHex(TempByte) + ">";

						if (TempLoop == TempCommandLength - 3)
							TempShowASCII = false;  //show the checksum in hex
					}
					TempString = vComPortString + TempString;
					vLocalSystemData->vLogFile.WriteToLogFile(TempString,cOutputMessage);
				}
			}
			else
			{
				TempErrorCode = GetLastError();
				CString vTempString2 = "\n\n\nCom Port " + vComPortString + " Write Error " + dtoa(TempErrorCode, 0);
				
				if (TempErrorCode == ERROR_INVALID_HANDLE)
					vTempString2 = "Port " + vComPortString + " Write Error: Invalid Handle, Error 6";
				if (TempErrorCode == ERROR_ACCESS_DENIED)
					vTempString2 = "Port " + vComPortString + " Write Error: Access Denied, Error 5";
				if (TempErrorCode == ERROR_FILE_NOT_FOUND)
					vTempString2 = "Port " + vComPortString + " Write Error: Device Not Found, Error 2";
				if (TempErrorCode == ERROR_INVALID_PARAMETER)
					vTempString2 = "Port " + vComPortString + " Write Error: Invalid Parameter, Error 87";
					
				ReportErrorMessage(vTempString2, cEMailInspx, 32000);
				/*
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = vTempString2;
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
				*/
			}
		}
	}
	else
		if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
			ReportErrorMessage(vHVPSType + " Command Too Long", cEMailInspx, 32000);
}

BYTE CSpellmanDXMXRaySourcePort::CalculateCheckSum(tXRayCommand TempCommand, BYTE TempNumberOfBytes)
{
	int TempSum = 0;
	for (BYTE TempLoop = 1; TempLoop <= TempNumberOfBytes + 1; TempLoop++)
	  TempSum = TempSum + TempCommand[TempLoop]; // sum up bytes

	TempSum = 256 - TempSum;//Take Two's Compliment
	TempSum = 127 & TempSum; //only allow bottom 7 bits
	TempSum = 0x40 | TempSum; //set bit six so get values 0x40 to 0x7f
	return (BYTE)TempSum;
}

WORD CSpellmanDXMXRaySourcePort::ReceiveDataCount()
{
	WORD TempCount = CSerialPortBase::ReceiveDataCount();
	WORD TempFinalCount = TempCount;
	if (TempCount)
	{
		WORD TempIndex = vReceiveDataTail;

		for (WORD TempLoop = 0; TempLoop < TempCount - 1; TempLoop++)
		{
			if (TempLoop > 1)
			if (vReceiveDataBuffer[TempIndex] == cSpellmanETX)
			{
				TempFinalCount = TempLoop + 1;
				TempLoop = TempCount;
			}
			if (TempIndex < cReceiveBufferSize - 1)
				TempIndex++;
			else
				TempIndex = 0;
		}
	}
	return TempFinalCount;
}

BYTE CSpellmanDXMXRaySourcePort::GetFirstEmptyCommandQueueSpot()
{
	BYTE TempIndex = 0;
	for (BYTE TempLoop = 0; TempLoop < 20; TempLoop++)
	if (!vCommandQueue[TempLoop][0])
	{
		TempIndex = TempLoop;
		TempLoop = 20;
	}
	if (vCommandQueue[TempIndex][0])
		TempIndex = 0xFF;
	return TempIndex;
}

void CSpellmanDXMXRaySourcePort::ClearCommandQueue()
{
	for (BYTE TempLoop = 0; TempLoop < 20; TempLoop++)
	{
		for (BYTE TempLoopA = 0; TempLoopA < 20; TempLoopA++)
			vCommandQueue[TempLoop][TempLoopA] = 0;

		vCommandQueueCommand[TempLoop] = 0;
	}
}

void CSpellmanDXMXRaySourcePort::PopFirstCommandOffQueue()
{
	//if there are any commands on the command queue
	if (vCommandQueue[0][0])
	if ((!vWaitingForReply) && (!vLastCommandSent))
	{
		//find number of commands on queue so know how many to move down
		BYTE TempNumberOfCommandsOnQueue = 0;
		for (BYTE TempLoop = 0; TempLoop < 20; TempLoop++)
		if (vCommandQueue[TempLoop][0])
			TempNumberOfCommandsOnQueue++;
		else
			TempLoop = 20;

		if (vLocalSystemData->vLogFile.vLogSerialData)
		{
			CString CStringATemp;
			CStringATemp.Format(_T("-Com%d-Pop off command: %d"),vGlobalSerialPortNumberXRaySource,vCommandQueueCommand[0]);
			vLocalSystemData->vLogFile.WriteToLogFile(this->vHVPSType + CStringATemp, cDebugMessage);
		}

		SendXRayCommand(vCommandQueue[0], vCommandQueue[0][0], vCommandQueueCommand[0]);

		if (TempNumberOfCommandsOnQueue)
		{
			//move each command down one in queue, so next one to send is in position zero
			if (TempNumberOfCommandsOnQueue > 1)
			{
				BYTE TempIndex = 0;
				for (BYTE TempLoop = 0; TempLoop < TempNumberOfCommandsOnQueue - 1; TempLoop++)
				{
					for (BYTE TempLoopA = 0; TempLoopA < 20; TempLoopA++)
						vCommandQueue[TempLoop][TempLoopA] = vCommandQueue[TempLoop + 1][TempLoopA];

					vCommandQueueCommand[TempLoop] = vCommandQueueCommand[TempLoop + 1];
				}
			}

			//clear memory location of last command in queue as moved it down one already
			BYTE TempIndex = TempNumberOfCommandsOnQueue - 1;
			for (BYTE TempLoop = 0; TempLoop < 20; TempLoop++)
				vCommandQueue[TempIndex][TempLoop] = 0;

			vCommandQueueCommand[TempIndex] = 0;
		}
	}
}

void CSpellmanDXMXRaySourcePort::CompleteInitialization()
{ //got model number before, so know if it is a DXM or MNX
	GetHardwareVersion();

	SetInRemoteMode(true);

	GetSoftwareVersion();

	GetWebServerVersion();

	if (vControllingAnMNX) //MNX is command 99, DXM is command 98
		SetXRayFilamentLimit(3.3 / 10);// 3.3 Amp out of 3.5 Amp limit MNX, (protocol range is to 10 Volts = 10 Amps)
	else
		SetXRayFilamentLimit(1); //vLocalSystemData->vDXMFilamentLimit);// 1 = full scale, or 4.6Amp limit DXM, 

	if (vControllingAnMNX) //MNX is command 99, DXM is command 98
		SetXRayFilamentPreHeat(1.0 / 10);//MNX = 1 Amp out of 2.5 Amp Preheat (protocol range is to 10 Volts = 10 Amps)
	else
		SetXRayFilamentPreHeat(0.5 / 2.5);//DXM = .2 = .5 Amp Preheat out of 2.5 Amp
}

void CSpellmanDXMXRaySourcePort::GetTemperature()
{
	//dxm CAN NOT DO. 100 W MonoBlock can do
}
	
void CSpellmanDXMXRaySourcePort::GetVoltageScale()
{
	//dxm CAN NOT DO. 100 W MonoBlock can do
}
	
void CSpellmanDXMXRaySourcePort::GetCurrentScale()
{
	//dxm CAN NOT DO. 100 W MonoBlock can do
}

void CSpellmanDXMXRaySourcePort::EnableWatchDog(bool TempEnable)
{
	//dxm CAN NOT DO. 100 W MonoBlock can do
}

void CSpellmanDXMXRaySourcePort::TickleWatchDog()
{
	//dxm CAN NOT DO. 100 W MonoBlock can do
}

void CSpellmanDXMXRaySourcePort::DisableWatchDogTimer()
{
	//dxm CAN NOT DO. 100 W MonoBlock can do
}
