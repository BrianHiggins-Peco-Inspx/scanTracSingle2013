//ScanTrac Side View Source File
// CheyneyMonoBlock.cpp: implementation of the CCheyneyMonoBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScanTrac.h"
#include "SerialPortBase.h"
#include "SerialPort.h"  //for RampingPortMessage
#include "SpellmanDXMXRaySourcePort.h"
#include "Spellman100WattMonoBlock.h"
#include "SerialPortReview.h"
#include "CheyneyMonoBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern DWORD vGlobalCallingThreadID;
extern int vGlobalSerialPortNumberXRaySource;
extern int vGlobalXrayNoCommMessageRcvdEver;

IMPLEMENT_DYNCREATE(CCheyneyMonoBlock, CSpellman100WattMonoBlock) //, CWinThread)

CCheyneyMonoBlock::CCheyneyMonoBlock()
{
	vHVPSType = "MonoBlock";
	vMinimumCurrent = 0;

}

CCheyneyMonoBlock::~CCheyneyMonoBlock()
{
}

void CCheyneyMonoBlock::DecodeMessage()
{
	WORD TempReturnValue = 0;
	CString TempReturnNumberString = "";

	tXRayReply TempResponse;
	WORD TempIndex = vReceiveDataTail;
	WORD TempLength = ReceiveDataCount();

	vHasReceivedCommunication = true;
	vNoCommunications = false;
	vCommunicationsTimeOutCounter = 0;
	vFoundXrayPort = false;

	if (TempLength)
	if (TempLength > 50)
	{
		if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
			ReportErrorMessage(vHVPSType + " Error Reply too long: " + dtoa(TempLength,0), cEMailInspx, 32000);
		TempLength = 50;
	}

	//WORD TempLength = vLatestResponse.GetLength();
	if (TempLength == 0)
	{
		if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
			ReportErrorMessage(vHVPSType + "Error - Latest response blank: " + vComPortString, cEMailInspx, 32000);
	}
	else
	if (vReceiveDataBuffer[TempIndex] != cSTX)
	{
		if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
			ReportErrorMessage(vHVPSType + " Error-Got: 0x" + ByteToHex(vReceiveDataBuffer[TempIndex]) + " Not STX at start of message: " + vComPortString, cWriteToLog, 0);
		TempIndex++;
	}
	else
	{
		for (WORD TempLoop = 0; TempLoop < TempLength; TempLoop++)
		{
			TempResponse[TempLoop] = vReceiveDataBuffer[TempIndex];
			if (TempIndex < cReceiveBufferSize - 1)
				TempIndex++;
			else
				TempIndex = 0;
		}

		if ((TempResponse[TempLength - 1] != cLineFeed) || (TempResponse[TempLength - 2] != cCarriageReturn))
		{
			if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
				ReportErrorMessage(vHVPSType + " Error - No CR,LF at end of message: " + vComPortString, cWriteToLog, 0); 
		}
		else
		if (vLastCommandSent == cNoCommand)
		{
			if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
				ReportErrorMessage(vHVPSType + " Error - Get response, but had not sent command: " + vComPortString, cEMailInspx, 32000);

			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			PopFirstCommandOffQueue();
		}
		else
		if (vLastCommandSent == cSetKV)
		{
			if (TempResponse[1] != ';')
			{
				if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
					ReportErrorMessage(vHVPSType + " Error - Set KV Not Acked: " + vComPortString, cEMailInspx, 32000);
				if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply NOT OK for SetKV"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
			}
			else
			{
				if (vLocalSystemData->vLogFile.vLogSerialData) 
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply OK for SetKV"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
				vFoundXrayPort = true;
			}
			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			PopFirstCommandOffQueue();
		}
		else
		if (vLastCommandSent == cSetKVMax)
		{
			if (TempResponse[1] != ';')
			{
				if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
					ReportErrorMessage(vHVPSType + " Error - Set KV Max Not Acked: " + vComPortString, cEMailInspx, 32000);
				if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply NOT OK for SetKVMax"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
			}
			else
			{
				if (vLocalSystemData->vLogFile.vLogSerialData) 
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply OK for SetKVMax"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
				vFoundXrayPort = true;
			}
			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			PopFirstCommandOffQueue();
		}
		else
		if (vLastCommandSent == cSetMA)
		{
			if (TempResponse[1] != ';')
			{
				if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
					ReportErrorMessage(vHVPSType + " Error - Set MA Not Acked: " + vComPortString, cEMailInspx, 32000);
				if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply NOT OK for SetMA Max"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
			}
			else
			{
				if (vLocalSystemData->vLogFile.vLogSerialData) 
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply OK for SetMA Max"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
				vFoundXrayPort = true;
			}
			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			PopFirstCommandOffQueue();
		}
		else
		if (vLastCommandSent == cSetMAMax)
		{
			if (TempResponse[1] != ';')
			{
				if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
					ReportErrorMessage(vHVPSType + " Error - Set MA Max Not Acked: " + vComPortString, cEMailInspx, 32000);
				if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply NOT OK for SetMA"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
			}
			else
			{
				if (vLocalSystemData->vLogFile.vLogSerialData) 
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply OK for SetMA"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
				vFoundXrayPort = true;
			}
			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			PopFirstCommandOffQueue();
		}
		else
		if (vLastCommandSent == cSetOn)
		{
			if (TempResponse[1] != ';')
			{
				if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
					ReportErrorMessage(vHVPSType + " Error - Turn On/Off Not Acked: " + vComPortString, cEMailInspx, 32000);
				if ((vLocalSystemData->vLogFile.vLogSerialData)  && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply NOT OK for TurnOn/Off"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
			}
			else
			{
				if (vLocalSystemData->vLogFile.vLogSerialData) 
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply OK for TurnOn/Off"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
				vFoundXrayPort = true;
			}
			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			PopFirstCommandOffQueue();
		}
		else
		if (vLastCommandSent == cResetFaults)
		{
			if (TempResponse[1] != ';')
			{
				if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
					ReportErrorMessage(vHVPSType + " Error - Reset Faults Not Acked: " + vComPortString, cEMailInspx, 32000);
				if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply NOT OK for ResetFaults"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
			}
			else
			{
				if (vLocalSystemData->vLogFile.vLogSerialData)
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply OK for ResetFaults"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
				vFoundXrayPort = true;
			}
			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			PopFirstCommandOffQueue();
		}
		else
		if (vLastCommandSent == cSetEnableWatchDog)
		{
			if (TempResponse[1] != ';')
			{
				if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
					ReportErrorMessage(vHVPSType + " Error - Enable/Disable Watch Dog Not Acked: " + vComPortString, cEMailInspx, 32000);
				if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply NOT OK for Enable/Disable Watch Dog"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
			}
			else
			{
				if (vLocalSystemData->vLogFile.vLogSerialData) 
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply OK for Enable/Disable Watch Dog"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
				vFoundXrayPort = true;
			}
			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			PopFirstCommandOffQueue();
		}
		else
		if (vLastCommandSent == cTickleWatchDog)
		{
			if (TempResponse[1] != ';')
			{
				if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
					ReportErrorMessage(vHVPSType + " Error - Tickle Watch Dog Not Acked: " + vComPortString, cEMailInspx, 32000);
				if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply NOT OK for Tickle Watch Dog"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
			}
			else
			{
				if (vLocalSystemData->vLogFile.vLogSerialData) 
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply OK for Tickle Watch Dog"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
				vFoundXrayPort = true;
			}
			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			PopFirstCommandOffQueue();
		}
		else
		if (vLastCommandSent == cGetSWVer)
		{ //reply with software version number
			vCommunicationsTimeOutCounter = 0;
			bool TempStringGood = true;
			for (BYTE TempLoop = 1; TempLoop < 6; TempLoop++) //up to 6 characters
			if ((TempResponse[TempLoop] < 32) || (TempResponse[TempLoop] > 126))
				TempStringGood = false;

			if (TempStringGood)
			{
				vSoftwareVersion = (char)TempResponse[1];
				for (BYTE TempLoop = 2; TempLoop < TempLength - 4; TempLoop++)
				{
					if (TempResponse[TempLoop] == cSemiColon)
						TempLoop = 20;
					else
						vSoftwareVersion = vSoftwareVersion + (char)TempResponse[TempLoop];
				}

				if (vLocalSystemData->vLogFile.vLogSerialData)
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply OK for GetSWVer: "),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString + vSoftwareVersion, cDebugMessage);
				}
				vFoundXrayPort = true;
			}
			else
			{
				//vSoftwareVersion = vHVPSType + " SWV: Bad Data:";
				//for (BYTE TempLoop = 1; TempLoop < 11; TempLoop++)
				//	vSoftwareVersion = vSoftwareVersion + " 0x" + ByteToHex(TempResponse[TempLoop]);
				vSoftwareVersion = "Cheyney";

				//if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
				//{
				//	CString TempCString;
				//	TempCString.Format("Com%d-Cheyney MonoBlock-Decoded Reply NOT OK for GetSWVer",vGlobalSerialPortNumberXRaySource);
				//	vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				//}
				//if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
				//	ReportErrorMessage("Error - " + vSoftwareVersion, cEMailMaintenance,0);
			}
			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			PopFirstCommandOffQueue();
		}
		else
		if (vLastCommandSent == cGetHWVer)
		{ //reply with Hardware version number
			vCommunicationsTimeOutCounter = 0;
			bool TempStringGood = true;
			for (BYTE TempLoop = 1; TempLoop < 3; TempLoop++) //3 CHARACTERS FOR HARDWARE VERSION
			if ((TempResponse[TempLoop] < 32) || (TempResponse[TempLoop] > 126))
				TempStringGood = false;

			if (TempStringGood)
			{
				vHardwareVersion = (char)TempResponse[1];
				for (BYTE TempLoop = 2; TempLoop < TempLength - 4; TempLoop++)
				{
					if (TempResponse[TempLoop] == cSemiColon)
						TempLoop = 7;
					else
						vHardwareVersion = vHardwareVersion + (char)TempResponse[TempLoop];
				}

				if (vLocalSystemData->vLogFile.vLogSerialData)
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply OK for GetHWVer: "),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString + vHardwareVersion, cDebugMessage);
				}
				vFoundXrayPort = true;
			}
			else
			{
				//vHardwareVersion = vHVPSType + " HW Version: Bad Data:";
				//for (BYTE TempLoop = 1; TempLoop < 3; TempLoop++)
				//	vHardwareVersion = vHardwareVersion + " 0x" + ByteToHex(TempResponse[TempLoop]);
				vHardwareVersion = "Cheyney";

				if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply NOT OK for GetHWVer"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
				if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
					ReportErrorMessage("Error - " + vHardwareVersion, cEMailInspx, 32000);
			}
			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			PopFirstCommandOffQueue();
		}
		else
		if (vLastCommandSent == cGetSWBuild)
		{ //reply with SOFTWARE BUILD version number
			vCommunicationsTimeOutCounter = 0;
			bool TempStringGood = true;
			for (BYTE TempLoop = 1; TempLoop < 3; TempLoop++)
			if ((TempResponse[TempLoop] < 32) || (TempResponse[TempLoop] > 126))
				TempStringGood = false;

			if (TempStringGood)
			{
				vWebserverVersion = (char)TempResponse[1];
				for (BYTE TempLoop = 2; TempLoop < TempLength - 4; TempLoop++)
				{
					if (TempResponse[TempLoop] == cSemiColon)
						TempLoop = 20;
					else
					if ((TempResponse[TempLoop] >= 32) && (TempResponse[TempLoop] <= 126))
						vWebserverVersion = vWebserverVersion + (char)TempResponse[TempLoop];
				}

				if (vLocalSystemData->vLogFile.vLogSerialData)
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply OK for GetSWBuild: "),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString + vWebserverVersion, cDebugMessage);
				}
				vFoundXrayPort = true;
			}
			else
			{
				//vWebserverVersion = vHVPSType + " Web: Bad Data:";
				//for (BYTE TempLoop = 1; TempLoop < 5; TempLoop++)
				//	vWebserverVersion = vWebserverVersion + " 0x" + ByteToHex(TempResponse[TempLoop]);
				vWebserverVersion = "Cheyney";

				if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply NOT OK for GetSWBuild"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
				if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
					ReportErrorMessage("Error - " + vWebserverVersion, cEMailMaintenance,0);
			}
			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			PopFirstCommandOffQueue();
		}
		else
		if (vLastCommandSent == cGetModel)
		{ //reply with Model number
			vCommunicationsTimeOutCounter = 0;
			bool TempModelNumberGood = true;
			if (TempLength > 9)
			{
				for (BYTE TempLoop = 1; TempLoop < TempLength - 4; TempLoop++)
				if ((TempResponse[TempLoop] < 32) || (TempResponse[TempLoop] > 126))
					TempModelNumberGood = false;

				if (TempModelNumberGood)
				{
					vModelNumber = (char)TempResponse[1];
					for (BYTE TempLoop = 2; TempLoop < TempLength - 4; TempLoop++)
						vModelNumber = vModelNumber + (char)TempResponse[TempLoop];

					if (vModelNumber == "ABCD1234")
						vModelNumber = "Cheyney";

					if (vLocalSystemData->vLogFile.vLogSerialData)
					{
						CString TempCString;
						TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply OK for GetModel: "),vGlobalSerialPortNumberXRaySource);
						vLocalSystemData->vLogFile.WriteToLogFile(TempCString + vModelNumber, cDebugMessage);
					}
					vFoundXrayPort = true;
				}
				else
				{
					//vModelNumber = vHVPSType + " Model: Bad Data:";
					//for (BYTE TempLoop = 1; TempLoop < TempLength - 4; TempLoop++)
					//	vModelNumber = vModelNumber + " 0x" + ByteToHex(TempResponse[TempLoop]);
					vModelNumber = "Cheyney";

					if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
					{
						CString TempCString;
						TempCString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply NOT OK for GetModel"),vGlobalSerialPortNumberXRaySource);
						vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
					}
					if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
						ReportErrorMessage("Error - " + vModelNumber, cEMailInspx,0);
				}

				if (vModelNumber.Find(_T("X3547")) != -1)
				{
					if (TempLength > 13)
						vModelNumber = "Spellman Cheyney>S/N:" + vModelNumber;
					else
						vModelNumber = "Spellman Cheyney>" + vModelNumber;
					vFoundXrayPort = true;
				}

				int TempPosition = vModelNumber.Find(_T("MNX"),0);
				if (TempPosition >= 0)
				{
					vControllingAnMNX = true;
					vHVPSType = "MNX";
					vRemoteMode = true;
					if (!((vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac)))
					//if not a 50 watt system, but has NMX, then give error
					ReportErrorMessage("MNX-Piccolo Power Supply Detected, but " + vHVPSType + "-Sideview Setting Enabled.", cError,0);
					vFoundXrayPort = true;
				}
				else
				{
					vControllingAnMNX = false;
					if ((vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac))
						ReportErrorMessage(vHVPSType + "-Sideview Power Supply Detected, but MNX-Piccolo Setting Enabled.", cError,0);
				}
			}
			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			PopFirstCommandOffQueue();
			CompleteInitialization();
		}
		else
		if (vLastCommandSent == cGetOnStatus)
		{  //latched status data, X-Rays on or off
			vCommunicationsTimeOutCounter = 0;
			//if (!vLocalConfigurationData->vSimulateLightControlBoard)
			if (TempResponse[1] == '1')
			{
				vLocalSystemData->vXRayOnNotice = true;

				if (vLocalSystemData->vLogFile.vLogSerialData)
				{
					CString TempString;
					TempString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply OK for GetStatus: X-Rays On"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempString, cDebugMessage);
				}
				vFoundXrayPort = true;
			}
			else
			{
				vLocalSystemData->vXRayOnNotice = false;

				if (vLocalSystemData->vLogFile.vLogSerialData)
				{
					CString TempString;
					TempString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply OK for GetStatus: X-Rays Off"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempString, cDebugMessage);
				}
				vFoundXrayPort = true;
			}
			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			PopFirstCommandOffQueue();
		}
		else
		//if ((TempResponse[1] == '5') && (TempResponse[2] == '5') && (TempResponse[3] == cComma))
		//{  //latched status data
		//	vCommunicationsTimeOutCounter = 0;
		//	if (!vControllingAnMNX)
		//	{
		//		if (vLocalConfigurationData->vSimulateLightControlBoard)
		//			TempResponse[4] = 0;

		//		if (TempResponse[4] == '1')  //interlock open = 1
		//		{
		//			if (vInterLockStatus)
		//			{
		//				if (vLocalSystemData->vSourceRampedUp)
		//				if (!vLocalSystemData->vShuttingDownXRays)
		//				{
		//					::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
		//					//::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
		//					//ReportErrorMessage("Error - X-Ray Source Interlock Fault - Inspection Stopped", cEMailMaintenance,9);
		//					//ReportErrorMessage("Error - X-Ray Source Interlock Fault - Inspection Stopped", cEMailInspx,9);
		//				}

		//				vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks & 0xFFDF); //clear bit 6, the x-ray interlock bit
		//				vInterLockStatus = false;
		//				::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
		//				//if (vGlobaluCSerialPort)
		//				//	vGlobaluCSerialPort->CheckInterlocks();
		//			}
		//		}
		//		else
		//		{
		//			if (!vInterLockStatus)
		//			{
		//				vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks | 0x0020);//set bit 6, the x-ray interlock bit
		//				vInterLockStatus = true;
		//				//vLocalSystemData->vHVPSInterlockDelay = 0;
		//				::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
		//				//if (vGlobaluCSerialPort)
		//				//	vGlobaluCSerialPort->CheckInterlocks();
		//			}
		//		}
		//	}
		//}
		//else
		if (vLastCommandSent == cGetFaults)
		{  //latched Fault data
			vCommunicationsTimeOutCounter = 0;

			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply for GetFaults"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempString, cDebugMessage);
			}

			if (TempResponse[1] == '1')
			{
				if (!vLocalSystemData->vXRayOnNotice) // if the X-Rays have gone off due to 3 arcs in 10 seconds
				{
					ClearXRayFault();
					if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
					{
						::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						ReportErrorMessage(vHVPSType + " Error - X-Ray Source Arc Fault - Inspection Stopped", cCritical,9);
						ReportErrorMessage(vHVPSType + " Error - X-Ray Source Arc Fault - Inspection Stopped", cError,9);
						ReportErrorMessage(vHVPSType + " Error - Multiple X-Ray Source Arc Faults - Inspection Stopped", cEMailInspx,32000);
					}
					else
						ReportErrorMessage(vHVPSType + " Notice - X-Ray Source Arc Fault - X-Rays were turning off", cWriteToLog,9);
				}
				else
					ReportErrorMessage(vHVPSType + " Error - X-Ray Source Single Arc Fault-Non fatal", cEMailInspx,9);
				vFoundXrayPort = true;
			}
			if (TempResponse[2] == '1')
			{
				ClearXRayFault();
				if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
				{
					::PostThreadMessage(vGlobalCallingThreadID, cEmergencyShutXRaysDownMessage, 0, 0);

					ReportErrorMessage(vHVPSType + " Error - X-Ray Source Too Hot. Inspection Stopped-Inside Temp: " + ConvertToDegreesCString(vLocalSystemData->vTemperatureAirInside,
						vLocalConfigurationData->vTemperatureAirInsideAdjust), cError,9);

					ReportErrorMessage(vHVPSType + " Error - X-Ray Source Too Hot. Inspection Stopped-Inside Temp: " + ConvertToDegreesCString(vLocalSystemData->vTemperatureAirInside,
						vLocalConfigurationData->vTemperatureAirInsideAdjust), cEMailInspx,9);
				}
				else
					ReportErrorMessage(vHVPSType + " Error - X-Ray Source Too Hot. Not Inspecting-Inside Temp: " + ConvertToDegreesCString(vLocalSystemData->vTemperatureAirInside,
						vLocalConfigurationData->vTemperatureAirInsideAdjust), cEMailInspx,9);
				vFoundXrayPort = true;
			}
			if (TempResponse[3] == '1')
			{
				ClearXRayFault();
				if (!vLocalConfigurationData->vIgnoreCheyneyInterlockStatus)
				if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && (vLocalSystemData->vRampXRayVoltageAmount > 19 / 6) && (vLocalSystemData->vXRayOnNotice) && (!vLocalSystemData->vSourceErrorReported))
				{
					ReportErrorMessage(vHVPSType + " Error - X-Ray Source Over Voltage. Trying Again. KV: " + dtoa(vLocalSystemData->vRampXRayVoltageAmount * 6, 2) , cEMailInspx,32000);
					//ReportErrorMessage(vHVPSType + " Error - X-Ray Source Over Voltage. Trying Again", cError,9);
					vLocalSystemData->vSourceErrorReported = true;
					Sleep(2000);
					TurnOnOffXRays(true);
					ReportErrorMessage(vHVPSType + " Turned back on", cEMailInspx,0);
				}
				else
				if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && (vLocalSystemData->vRampXRayVoltageAmount > 19 / 6) && (vLocalSystemData->vXRayOnNotice))
				{
					::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
					ReportErrorMessage(vHVPSType + " Error - X-Ray Source Over Voltage. Setting: " + dtoa(vLocalSystemData->vRampXRayVoltageAmount, 2) , cEMailInspx,32000);
					ReportErrorMessage(vHVPSType + " Error - X-Ray Source Over Voltage. Inspection Stopped", cError,9);
				}
				else
					ReportErrorMessage(vHVPSType + " Notice - X-Ray Source Over Voltage-Inspection had just stopped", cWriteToLog,9);
			}
			if (TempResponse[4] == '1')
			{
				ClearXRayFault();
				if (!vLocalConfigurationData->vIgnoreCheyneyInterlockStatus)
				if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && (vLocalSystemData->vXRayOnNotice) && (!vLocalSystemData->vSourceErrorReported))
				{
					vLocalSystemData->vSourceErrorReported = true;
					ReportErrorMessage(vHVPSType + " Error - X-Ray Source Under Voltage. Try Again", cEMailInspx,32000);
					//ReportErrorMessage(vHVPSType + " Error - X-Ray Source Under Voltage. Try Again", cError,9);

					Sleep(2000);
					TurnOnOffXRays(true);
					ReportErrorMessage(vHVPSType + " Turned back on", cEMailInspx,0);
				}
				else
				if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && (vLocalSystemData->vXRayOnNotice))
				{
					::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
					ReportErrorMessage(vHVPSType + " Error - X-Ray Source Under Voltage. Inspection Stopped", cEMailInspx,32000);
					ReportErrorMessage(vHVPSType + " Error - X-Ray Source Under Voltage. Inspection Stopped", cError,9);
				}
				else
					ReportErrorMessage(vHVPSType + " Notice - X-Ray Source Under Voltage-Inspection had just stopped", cWriteToLog,9);
			}
			if (TempResponse[5] == '1')
			{
				ClearXRayFault();
				if (!vLocalConfigurationData->vIgnoreCheyneyInterlockStatus)
				if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && (vLocalSystemData->vXRayOnNotice) && (!vLocalSystemData->vSourceErrorReported))
				{
					vLocalSystemData->vSourceErrorReported = true;
					if (vLocalSystemData->vSystemRunMode)
					{
						ReportErrorMessage(vHVPSType + " Error - X-Ray Source Over Current. Try Again", cEMailInspx,32000);
						//ReportErrorMessage(vHVPSType + " Error - X-Ray Source Over Current. Try Again", cError,9);
						Sleep(2000);
						TurnOnOffXRays(true);
						ReportErrorMessage(vHVPSType + " Turned back on", cEMailInspx,0);
					}
				}
				else
				if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) || (vLocalSystemData->vXRayOnNotice))
				{
					::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
					ReportErrorMessage(vHVPSType + " Error - X-Ray Source Over Current. Inspection Stopped", cEMailInspx,32000);
					ReportErrorMessage(vHVPSType + " Error - X-Ray Source Over Current. Inspection Stopped", cError,9);
				}
				else
				{
					ReportErrorMessage(vHVPSType + " Notice - X-Ray Source Over Current-Inspection had just stopped", cWriteToLog,9);
				}
			}
			if (TempResponse[6] == '1')
			{
				ClearXRayFault();
				double TempCurrentSetting = vLocalSystemData->vRampXRayCurrentAmount;
				if (vComPortNumber == 5)
					TempCurrentSetting = vLocalSystemData->vRampXRayCurrentAmount;

				if (!vLocalConfigurationData->vIgnoreCheyneyInterlockStatus)
				if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && (vLocalSystemData->vXRayOnNotice) && (vLocalSystemData->vSourceErrorReported < 5))
				{
					if (TempCurrentSetting > .5)
					{
						vLocalSystemData->vSourceErrorReported++;
						ReportErrorMessage(vHVPSType + " Notice - X-Ray Source Under Current. Setting: " + dtoa(TempCurrentSetting,2) + ", Reading: " + dtoa(vLocalSystemData->vSourceCurrent,2), cEMailInspx,32000);
						ReportErrorMessage(vHVPSType + " Notice - X-Ray Source Under Current. Setting: " + dtoa(TempCurrentSetting,2) + ", Reading: " + dtoa(vLocalSystemData->vSourceCurrent,2), cError,9);
					}
					if (vLocalSystemData->vXRayOnNotice)
					{
						Sleep(2000);
						TurnOnOffXRays(true);
						ReportErrorMessage(vHVPSType + " Turned back on, because sensed Under Current. Setting: " + dtoa(TempCurrentSetting,2) + ", Reading: " + dtoa(vLocalSystemData->vSourceCurrent,2), cEMailInspx,32000);
						ReportErrorMessage(vHVPSType + " Turned back on, because sensed Under Current. Setting: " + dtoa(TempCurrentSetting,2) + ", Reading: " + dtoa(vLocalSystemData->vSourceCurrent,2), cAction,0);
					}
				}
				else
				if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && (vLocalSystemData->vXRayOnNotice) && (vLocalSystemData->vSourceErrorReported < 7))
				{
					if (TempCurrentSetting > .5)
					{
						vLocalSystemData->vSourceErrorReported++;
						ReportErrorMessage(vHVPSType + " Notice - X-Ray Source Under Current. Setting: " + dtoa(TempCurrentSetting,2) + ", Reading: " + dtoa(vLocalSystemData->vSourceCurrent,2), cEMailInspx,32000);
						ReportErrorMessage(vHVPSType + " Notice - X-Ray Source Under Current. Setting: " + dtoa(TempCurrentSetting,2) + ", Reading: " + dtoa(vLocalSystemData->vSourceCurrent,2), cError,9);
					}
				}
				else
					ReportErrorMessage(vHVPSType + " Notice - X-Ray Source Under Current-Inspection had just stopped. Setting: " + dtoa(TempCurrentSetting,2) + ", Reading: " + dtoa(vLocalSystemData->vSourceCurrent,2), cWriteToLog,9);

				vFoundXrayPort = true;
			}
			if (TempResponse[7] == '1')
			{
				ReportErrorMessage(vHVPSType + " Error - Lost Communications-WD-Inspection Stopped", cWriteToLog,9);
				ClearXRayFault();
				if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)// && (!vLocalSystemData->vSourceErrorReported))
				{
					vLocalSystemData->vSourceErrorReported = true;
					ReportErrorMessage(vHVPSType + " Error - Watch Dog Timeout-Inspection Stopped", cEMailInspx,9);
					ReportErrorMessage(vHVPSType + " Error - Lost Communications-WD-Inspection Stopped", cError,9);

					::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
				}
				else
					ReportErrorMessage(vHVPSType + " Notice - Watch Dog Timeout, but not inspecting", cWriteToLog,0);
				vFoundXrayPort = true;
			}
			if (TempResponse[8] == '1')
			{
				if ((!vLocalConfigurationData->vSimulateLightControlBoard) && (!vLocalConfigurationData->vIgnoreCheyneyInterlockStatus)) //don't say interlock open if simulating X-Rays
				if (vInterLockStatus)
				{
					vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks & 0xFFDF); //clear bit 6, the x-ray interlock bit
					vInterLockStatus = false;
					::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
				}
				vFoundXrayPort = true;
			}
			else
			{
				if (!vInterLockStatus)
				{
					vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks | 0x0020); //set bit 6, the x-ray interlock bit
					vInterLockStatus = true;
					::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
				}
			}
			if (TempResponse[9] == '1')
			{
				ClearXRayFault();
				if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) && (vLocalSystemData->vXRayOnNotice) && (!vLocalSystemData->vSourceErrorReported))
				{
					vLocalSystemData->vSourceErrorReported = true;
					ReportErrorMessage(vHVPSType + " Error - Over Power Limit-Inspection Stopped", cEMailInspx,32000);
					ReportErrorMessage(vHVPSType + " Error - Over Power Limit-Inspection Stopped", cError,9);

					::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
				}
				else
					ReportErrorMessage(vHVPSType + " Notice - Over Power Limit.  Inspection had just stopped", cWriteToLog,0);
				vFoundXrayPort = true;
			}
			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			PopFirstCommandOffQueue();
		}
		else
		if (vLastCommandSent == cGetTemp)
		{  //Temperature
			vCommunicationsTimeOutCounter = 0;
			TempReturnNumberString = GetNumberStringFromReply();
			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply for GetTemp: "),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempString + TempReturnNumberString, cDebugMessage);
			}
			TempReturnValue = _wtoi(TempReturnNumberString);

			vTemperature = (double)TempReturnValue * 70.036 / 956;
			if (vTemperature > vTemperatureMaximum)
				vTemperatureMaximum = vTemperature;
			if (vTemperatureMinimum > vTemperature)
				vTemperatureMinimum = vTemperature;

			if ((BYTE)vTemperature > vLocalConfigurationData->vMaximumMonoBlockTemperature) 
			{
				if ((vLocalSystemData->vSourceRampedUp) && (!vLocalSystemData->vShuttingDownXRays) && (vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
				{
					::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
					ReportErrorMessage(vHVPSType + " is Too Hot-Error-Inspection Stopped", cError,9);
					ReportErrorMessage(vHVPSType + " is Too Hot-Error-Inspection Stopped", cEMailInspx,32000);
				}
				else
					ReportErrorMessage(vHVPSType + " is Too Hot-Notice-Not Inspecting", cWriteToLog,0);
			}

			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-Cheyney MonoBlock-Converted Reply for GetTemp: "),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempString + dtoa(vTemperature, 2), cDebugMessage);
			}
			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			PopFirstCommandOffQueue();
		}
		else
		if (vLastCommandSent == cGetKV)
		{  //HV Voltage
			vCommunicationsTimeOutCounter = 0;
			TempReturnNumberString = GetNumberStringFromReply();
			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply for GetKV: "),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempString + TempReturnNumberString, cDebugMessage);
			}

			TempReturnValue = _wtoi(TempReturnNumberString);

			vLocalSystemData->vSourceVoltage = (double)TempReturnValue * vVoltageMaximumScale / 4095.0 / 6.0;

			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-Cheyney MonoBlock-Converted Reply for GetKV: "),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempString + dtoa(vLocalSystemData->vSourceVoltage * 6, 2), cDebugMessage);
			}

			if (vLocalSystemData->vSourceVoltage < .05)
				vLocalSystemData->vSourceVoltage = 0;

			//must comment out to test monoblock in the office
			if (vLocalConfigurationData->vSimulateLightControlBoard)
			{
				if (vLocalSystemData->vLastXRayCommandOn)
					vLocalSystemData->vSourceVoltage = vLocalSystemData->vRampXRayVoltageAmount;
				else
					vLocalSystemData->vSourceVoltage = 0;
			}

			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			PopFirstCommandOffQueue();
			VerifySourceWithinTolerance();
		}
		else
		if (vLastCommandSent == cGetMA)
		{  //HV Current
			vCommunicationsTimeOutCounter = 0;
			TempReturnNumberString = GetNumberStringFromReply();
			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply for GetMA: "),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempString + TempReturnNumberString, cDebugMessage);
			}

			TempReturnValue = _wtoi(TempReturnNumberString);

			vLocalSystemData->vSourceCurrent = 	(double)TempReturnValue * vCurrentMaximumScale / 4095.0;
			if (vLocalSystemData->vSourceCurrent < .01)
				vLocalSystemData->vSourceCurrent = 0;

			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-Cheyney MonoBlock-Converted Reply for GetMA: "), vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempString + dtoa(vLocalSystemData->vSourceCurrent, 2), cDebugMessage);
			}			//must comment out to test monoblock in the office
			if (vLocalConfigurationData->vSimulateLightControlBoard)
			{
				if (vLocalSystemData->vLastXRayCommandOn)
					vLocalSystemData->vSourceCurrent = vLocalSystemData->vRampXRayCurrentAmount;
				else
					vLocalSystemData->vSourceCurrent = 0;
			}
			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			PopFirstCommandOffQueue();
		}
		else
		if (vLastCommandSent == cGetVoltageScale)
		{  //HV Voltage Scale
			vCommunicationsTimeOutCounter = 0;
			TempReturnNumberString = GetNumberStringFromReply();
			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply for GetVoltageScale: "), vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempString + TempReturnNumberString, cDebugMessage);
			}
			TempReturnValue = _wtoi(TempReturnNumberString);

			vVoltageMaximumScale = (double)TempReturnValue / 100.0;

			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-Cheyney MonoBlock-Converted Reply for GetVoltageScale: "),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempString + dtoa(vVoltageMaximumScale, 4), cDebugMessage);
			}
			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			PopFirstCommandOffQueue();
			VerifySourceWithinTolerance();
		}
		else
		if (vLastCommandSent == cGetCurrentScale)
		{  //HV Current Scale
			vCommunicationsTimeOutCounter = 0;
			TempReturnNumberString = GetNumberStringFromReply();
			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply for GetCurrentScale: "),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempString + TempReturnNumberString, cDebugMessage);
			}
			TempReturnValue = _wtoi(TempReturnNumberString);

			if (TempReturnValue > 0)
				vCurrentMaximumScale = 	(double)TempReturnValue / 1000.0;
			else
			{
				vCurrentMaximumScale = 2;
				ReportErrorMessage(vHVPSType + " Ignore Maximum Current of zero from Cheyney Monoblock" , cEMailInspx,0);
			}

			double TempMaximumCurrent = vCurrentMaximumScale;// * 0.9; //for the 100 Watt XRB, the maximum current allowed is 10% less than the Scale
			if (vLocalConfigurationData->vMaximumCurrent > TempMaximumCurrent)
			{
				ReportErrorMessage(vHVPSType + " Maximum Current of: " + dtoa(vLocalConfigurationData->vMaximumCurrent, 2) + " was too high.  Set to: " + dtoa(TempMaximumCurrent, 2), cWriteToLog,0);
				vLocalConfigurationData->vMaximumCurrent = TempMaximumCurrent;
			}

			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-Cheyney MonoBlock-Converted Reply for GetCurrentScale: "),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempString + dtoa(vCurrentMaximumScale, 4), cDebugMessage);
			}
			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			PopFirstCommandOffQueue();
		}
		else
		if (vLastCommandSent == cGetFilament)
		{  //Filament feedback
			vCommunicationsTimeOutCounter = 0;
			TempReturnNumberString = GetNumberStringFromReply();
			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply for GetFilament: "),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempString + TempReturnNumberString, cDebugMessage);
			}
			TempReturnValue = _wtoi(TempReturnNumberString);

			vFilamentCurrentFeedbackMonitor = 	(double)TempReturnValue * vCurrentMaximumScale / 4095.0;
			if (vHaveNewFilamentFeedbackMonitorReading < 250)
				vHaveNewFilamentFeedbackMonitorReading++;

			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-Cheyney MonoBlock-Converted Reply for GetFilament: "),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempString + dtoa(vFilamentCurrentFeedbackMonitor, 2), cDebugMessage);
			}
			if (vFilamentCurrentMin > vFilamentCurrentFeedbackMonitor)
				vFilamentCurrentMin = vFilamentCurrentFeedbackMonitor;
			if (vFilamentCurrentMax < vFilamentCurrentFeedbackMonitor)
				vFilamentCurrentMax = vFilamentCurrentFeedbackMonitor;
			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			PopFirstCommandOffQueue();
		}
		else
		if (vLastCommandSent == cGet15V)
		{  //-15 volt monitor
			vCommunicationsTimeOutCounter = 0;
			TempReturnNumberString = GetNumberStringFromReply();
			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-Cheyney MonoBlock-Decoded Reply for Get15V: "),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempString + TempReturnNumberString, cDebugMessage);
			}
			TempReturnValue = _wtoi(TempReturnNumberString);

			v15VoltMonitor = (3972 -(double)TempReturnValue) * 0.006224; //Cheyney

			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-Cheyney MonoBlock-Converted Reply for Get15V: "),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempString + dtoa(v15VoltMonitor, 2), cDebugMessage);
			}
			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			PopFirstCommandOffQueue();
		}
		else
		{
			if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
				ReportErrorMessage(vHVPSType + " Error-Unknown Reply: " + vLatestResponse, cEMailInspx, 32000);
			vLastCommandSent = cNoCommand;
			vWaitingForReply = 0;
			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-Cheyney-Error-Unknown Reply: "),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(vHVPSType + TempString + vLatestResponse, cDebugMessage);
			}
			PopFirstCommandOffQueue();
		}
	}
	vLatestResponse = "";
	if ((vFoundXrayPort == true) && (vGlobalXrayNoCommMessageRcvdEver != cYesCommSeen))
	{
		vGlobalXrayNoCommMessageRcvdEver = cYesCommSeen;
		if (vGlobalPortHereOK[vGlobalSerialPortNumberXRaySource] != cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER)	
		{
			vGlobalPortHereOK[vGlobalSerialPortNumberXRaySource] = cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER;
			if (vComPortNumber != cDEFAULT_XRAY_SERIAL_PORT_NUMBER)
			{
				CString TempAString;
				TempAString.Format(_T("Error-XRay Com Port %d, Default Com 3"),vGlobalSerialPortNumberXRaySource);
				ReportErrorMessage(TempAString, cEMailInspx,32000);
//				ReportErrorMessage(TempAString, cError,0);
			}
		}
	} 
}

void CCheyneyMonoBlock::SetXRayVoltage(double TempVoltage)
{
	if (vVoltageMaximumScale <= 0)
		vVoltageMaximumScale = 70;
	//input is 0 to 10 representing 0 to 60 kV
	//output is 0 to 4095 representing 0 to 70 kV
	if (TempVoltage > 14)
		TempVoltage = 14; // 12 = 82 KV
	double TempCorrectedVoltage = 0;
	TempCorrectedVoltage = TempVoltage * 6.0 / vVoltageMaximumScale * 4095.0; //100 Watt Mono Block map code from KV / 6 to 80 KV

	if (TempCorrectedVoltage > 4095) //4095 is 100% of max
		TempCorrectedVoltage = 4095;
	//vVoltageMaximumScale is 10% above 80KV.  at 5% from maximum will get over voltage error

	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'V';
	TempCommand[2] = (BYTE)'R';
	TempCommand[3] = (BYTE)'E';
	TempCommand[4] = (BYTE)'F';
	TempCommand[5] = cSpace;
	TempCommand[6] = (BYTE)'0';
	TempCommand[7] = (BYTE)'0';
	TempCommand[8] = (BYTE)'0';
	TempCommand[9] = (BYTE)'0';
	CString TempString = dtoa(TempCorrectedVoltage, 0);
	BYTE TempLength = TempString.GetLength();
	for (BYTE TempLoop = 0; TempLoop < TempLength; TempLoop++)
		TempCommand[10 - TempLength + TempLoop] = (BYTE)(TempString.GetAt(TempLoop));
	if (!vLocalConfigurationData->vSimulateLightControlBoard)
		SendXRayCommand(TempCommand, 9, cSetKV);
}

void CCheyneyMonoBlock::SetXRayCurrent(double TempCurrent)
{
	if (vCurrentMaximumScale <= 0)
		vCurrentMaximumScale = 2;
	//current value is scale of 0 to 10 for 0 to 10 milli amps
	//need to send value of 0 to 4095 for 0 to 90% of 2 milli amps or vCurrentMaximumScale
	double TempCorrectedCurrent = TempCurrent / vCurrentMaximumScale  * 4095.0;

	if (TempCorrectedCurrent > 4095) //4095 is 100% of max
		TempCorrectedCurrent = 4095;

	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'I';
	TempCommand[2] = (BYTE)'R';
	TempCommand[3] = (BYTE)'E';
	TempCommand[4] = (BYTE)'F';
	TempCommand[5] = cSpace;
	TempCommand[6] = (BYTE)'0';
	TempCommand[7] = (BYTE)'0';
	TempCommand[8] = (BYTE)'0';
	TempCommand[9] = (BYTE)'0';
	CString TempString = dtoa(TempCorrectedCurrent, 0);
	BYTE TempLength = TempString.GetLength();
	for (BYTE TempLoop = 0; TempLoop < TempLength; TempLoop++)
		TempCommand[10 - TempLength + TempLoop] = (BYTE)(TempString.GetAt(TempLoop));
	if (!vLocalConfigurationData->vSimulateLightControlBoard)
		SendXRayCommand(TempCommand, 9, cSetMA);
}

void CCheyneyMonoBlock::SetMaximumVoltageSupported(double TempVoltage)
{ //default is 70 kv
	double TempCorrectedVoltage = 70;
	TempCorrectedVoltage = TempVoltage;

	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'V';
	TempCommand[2] = (BYTE)'M';
	TempCommand[3] = (BYTE)'A';
	TempCommand[4] = (BYTE)'X';
	TempCommand[5] = cSpace;
	TempCommand[6] = (BYTE)'0';
	TempCommand[7] = (BYTE)'0';
	TempCommand[8] = (BYTE)'0';
	TempCommand[9] = (BYTE)'0';
	CString TempString = dtoa(TempCorrectedVoltage, 0);
	BYTE TempLength = TempString.GetLength();
	for (BYTE TempLoop = 0; TempLoop < TempLength; TempLoop++)
		TempCommand[10 - TempLength + TempLoop] = (BYTE)(TempString.GetAt(TempLoop));
	if (!vLocalConfigurationData->vSimulateLightControlBoard)
		SendXRayCommand(TempCommand, 9, cSetKVMax);
}


void CCheyneyMonoBlock::SetMaximumCurrentSupported(double TempCurrent)
{ //default is 70 kv
	double TempCorrectedCurrent = 2;
	TempCorrectedCurrent = TempCurrent;
	TempCorrectedCurrent = TempCorrectedCurrent * 10; //send 30 to mean 3.0 mAmps

	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'I';
	TempCommand[2] = (BYTE)'M';
	TempCommand[3] = (BYTE)'A';
	TempCommand[4] = (BYTE)'X';
	TempCommand[5] = cSpace;
	TempCommand[6] = (BYTE)'0';
	TempCommand[7] = (BYTE)'0';
	TempCommand[8] = (BYTE)'0';
	TempCommand[9] = (BYTE)'0';
	CString TempString = dtoa(TempCorrectedCurrent, 0);
	BYTE TempLength = TempString.GetLength();
	for (BYTE TempLoop = 0; TempLoop < TempLength; TempLoop++)
		TempCommand[10 - TempLength + TempLoop] = (BYTE)(TempString.GetAt(TempLoop));
	if (!vLocalConfigurationData->vSimulateLightControlBoard)
		SendXRayCommand(TempCommand, 9, cSetMAMax);
}

void CCheyneyMonoBlock::CompleteInitialization()
{
	SetMaximumVoltageSupported(70);
	if (vLocalConfigurationData->vIgnoreCheyneyInterlockStatus) //is a cheyney 210 w monoblock
		SetMaximumCurrentSupported(3); //210 watt monoblock
	else
		SetMaximumCurrentSupported(2); //100 watt monoblock

	GetCurrentScale();
	GetVoltageScale();

	CSpellman100WattMonoBlock::CompleteInitialization();
}