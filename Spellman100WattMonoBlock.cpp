//ScanTrac Side View Source File
// Spellman100WattMonoBlock.cpp: implementation of the Spellman100WattMonoBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScanTrac.h"
#include "Spellman100WattMonoBlock.h"
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
extern int vGlobalSerialPortNumberXRaySource;
extern int vGlobalXrayNoCommMessageRcvdEver;
//extern CSerialPort *vGlobaluCSerialPort;

IMPLEMENT_DYNCREATE(CSpellman100WattMonoBlock, CSpellmanDXMXRaySourcePort) //, CWinThread)

CSpellman100WattMonoBlock::CSpellman100WattMonoBlock(void)
{
	vMinimumCurrent = 0.15;
	vHVPSType = "XRB100W";
	vRemoteMode = true;
	vLastTickleTime = 0;
	vWatchDogEnabled = false;
	vFoundXrayPort = false;
	vWaitingForReply = false;
}


CSpellman100WattMonoBlock::~CSpellman100WattMonoBlock(void)
{
}

void CSpellman100WattMonoBlock::SendXRayCommand(tXRayCommand TempData, BYTE TempNumberOfBytes, BYTE TempCommandSending)
{
	if (TempCommandSending == cTickleWatchDog)
	{
		vLastTickleTime = CTime::GetCurrentTime();
	}

	if ((vWaitingForReply) || (vLastCommandSent))
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

				if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
				{
					CString TempString;
					TempString.Format(_T("-Com%d-100W-Queue up command: "), vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(this->vHVPSType + TempString + dtoa(TempCommandSending, 0), cDebugMessage);
				}
			}
			else
			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("-Error-Com%d-100W-Waiting for Reply and Queue Full, ignoring command: "), vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(this->vHVPSType + TempString + dtoa(TempCommandSending, 0), cDebugMessage);
			}
		}
	}
	else
	if (TempNumberOfBytes < 17)
	{
		vWaitingForReply = 1;
		if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
		{
			CString TempString;
			TempString.Format(_T("-Com%d-100W-Send command: "),vGlobalSerialPortNumberXRaySource);
			vLocalSystemData->vLogFile.WriteToLogFile(this->vHVPSType + TempString + dtoa(TempCommandSending, 0), cDebugMessage);
		}

		if (TempCommandSending > cSetMAMax)
		{
			vLastCommandSent = cNoCommand;
			if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
				ReportErrorMessage("XRB100W MonoBlock Command Programming Error", cEMailInspx, 32000);
		}
		else
		{
			vLastCommandSent = TempCommandSending;

		tXRayCommand TempCommand;
		TempCommand[0] = cSTX;
		for (BYTE TempLoop = 1; TempLoop <= TempNumberOfBytes; TempLoop++)
			TempCommand[TempLoop] = TempData[TempLoop];
		TempCommand[TempNumberOfBytes + 1] = (BYTE)';';
		TempCommand[TempNumberOfBytes + 2] = CalculateCheckSum(TempCommand, TempNumberOfBytes);
		TempCommand[TempNumberOfBytes + 3] = cCarriageReturn;
		TempCommand[TempNumberOfBytes + 4] = cLineFeed;

		BYTE TempCommandLength = TempNumberOfBytes + 5;
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
				if (((vLocalSystemData->vLogFile.vLogSerialData) || (vLocalSystemData->vLogFile.vShowingSerialDataOnScreen)) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
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
						if (TempByte == cSemiColon)
						{
							TempString = TempString + ";";
						}
						else
						if (TempByte == cCarriageReturn)
						{
							TempString = TempString + "<CR>";
						}
						else
						if (TempByte == cLineFeed)
						{
							TempString = TempString + "<LF>";
						}
						else
						if (TempShowASCII)
							TempString = TempString + (char)TempByte;
						else
							TempString = TempString + "<" + ByteToHex(TempByte) + ">";

						if (TempLoop == TempCommandLength - 4)
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
			}
		}
	}
	}
	else
	{
		if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
			ReportErrorMessage(vHVPSType + " Command Too Long", cEMailInspx, 32000);
	}
}

void CSpellman100WattMonoBlock::SetXRayVoltage(double TempVoltage)
{
	//input is 0 to 10 representing 0 to 60 kV
	//output is 0 to 4095 representing 0 to 70 kV
	if (vVoltageMaximumScale <= 0)
		vVoltageMaximumScale = 70;
	if (TempVoltage > 14)
		TempVoltage = 14; // 12 = 82 KV
	double TempCorrectedVoltage = 0;
	TempCorrectedVoltage = TempVoltage * 6.0 / vVoltageMaximumScale * 4095.0; //100 Watt Mono Block map code from KV / 6 to 80 KV

	if (TempCorrectedVoltage > 3685) //3685 is 90% of max
		TempCorrectedVoltage = 3685;
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
	CString TempString = dtoa(TempCorrectedVoltage,0);
	BYTE TempLength = TempString.GetLength();
	for (BYTE TempLoop = 0; TempLoop < TempLength; TempLoop++)
		TempCommand[10 - TempLength + TempLoop] = (BYTE)TempString.GetAt(TempLoop);
	if (!vLocalConfigurationData->vSimulateLightControlBoard)
		SendXRayCommand(TempCommand, 9, cSetKV);
	//Sleep(1000);
}

void CSpellman100WattMonoBlock::TurnOnOffXRays(BYTE TempOn)
{ 
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'E';
	TempCommand[2] = (BYTE)'N';
	TempCommand[3] = (BYTE)'B';
	TempCommand[4] = (BYTE)'L';
	TempCommand[5] = cSpace;
	TempCommand[6] = (BYTE)'0';
	if (TempOn)
		TempCommand[6] = (BYTE)'1';
	if (!vLocalConfigurationData->vSimulateLightControlBoard)
		SendXRayCommand(TempCommand, 6, cSetOn);

	bool TempEnable = false;
	if (TempOn)
		TempEnable = true;

	//EnableWatchDog(TempEnable); //Spellman XRB-80 100 watt monoblock timeout is 10 seconds
}

void CSpellman100WattMonoBlock::GetXRaySourceFaultData()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'F';
	TempCommand[2] = (BYTE)'L';
	TempCommand[3] = (BYTE)'T';

	SendXRayCommand(TempCommand, 3, cGetFaults);
}

void CSpellman100WattMonoBlock::ClearXRayFault()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'C';
	TempCommand[2] = (BYTE)'L';
	TempCommand[3] = (BYTE)'R';
	SendXRayCommand(TempCommand, 3, cResetFaults);
}

void CSpellman100WattMonoBlock::SetFilamentLimitToDefault()
{
}

void CSpellman100WattMonoBlock::SetFilamentLimitToZero()
{
}

void CSpellman100WattMonoBlock::SetXRayCurrent(double TempCurrent)
{
	//current value is scale of 0 to 10 for 0 to 10 milli amps
	//need to send value of 0 to 4095 for 0 to 90% of 2 milli amps or vCurrentMaximumScale
	if (vCurrentMaximumScale <= 0)
		vCurrentMaximumScale = 2;
	if (TempCurrent > vCurrentMaximumScale * .9)  //limit is 90% of max scale
		TempCurrent = vCurrentMaximumScale * .9; //2 maximum for XRB100W MonoBlock

	if (vLocalSystemData->vRampXRayVoltageAmount) //if voltage is non zero, maintain a minimum current
		if (TempCurrent < vMinimumCurrent)  //150 uA is smallest current allowed above zero
	{
		//if (vLocalSystemData->vXRayOnNotice)
		//	ReportErrorMessage("100W MonoBlock current: " + dtoa(TempCurrent, 4) + " setting to " + dtoa(vMinimumCurrent, 2), cWriteToLog, 0); //for testing
		ReportErrorMessage("100W MonoBlock current: " + dtoa(TempCurrent, 4) + " below Minimum, setting to " + dtoa(vMinimumCurrent, 2), cWriteToLog, 0); //for testing
		TempCurrent = vMinimumCurrent;
	}

	double TempCorrectedCurrent = TempCurrent  / vCurrentMaximumScale  * 4095.0;

	if (TempCorrectedCurrent > 3685) //3685 is 90% of max
		TempCorrectedCurrent = 3685;

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
	CString TempString = dtoa(TempCorrectedCurrent,0);
	BYTE TempLength = TempString.GetLength();
	for (BYTE TempLoop = 0; TempLoop < TempLength; TempLoop++)
		TempCommand[10 - TempLength + TempLoop] = (BYTE)TempString.GetAt(TempLoop);
	if (!vLocalConfigurationData->vSimulateLightControlBoard)
		SendXRayCommand(TempCommand, 9, cSetMA);
}

void CSpellman100WattMonoBlock::SetXRayFilamentLimit(double TempLimit)
{
}

void CSpellman100WattMonoBlock::SetXRayFilamentPreHeat(double TempPreHeat)
{
}

void CSpellman100WattMonoBlock::GetXRayVoltage()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'V';
	TempCommand[2] = (BYTE)'M';
	TempCommand[3] = (BYTE)'O';
	TempCommand[4] = (BYTE)'N';
	SendXRayCommand(TempCommand, 4, cGetKV);
}

void CSpellman100WattMonoBlock::GetXRayCurrent()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'I';
	TempCommand[2] = (BYTE)'M';
	TempCommand[3] = (BYTE)'O';
	TempCommand[4] = (BYTE)'N';
	SendXRayCommand(TempCommand, 4, cGetMA);
}

void CSpellman100WattMonoBlock::GetXRayStatus()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'S';
	TempCommand[2] = (BYTE)'T';
	TempCommand[3] = (BYTE)'A';
	TempCommand[4] = (BYTE)'T';
	SendXRayCommand(TempCommand, 4, cGetOnStatus);
}

void CSpellman100WattMonoBlock::GetFilamentFeedback()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'F';
	TempCommand[2] = (BYTE)'M';
	TempCommand[3] = (BYTE)'O';
	TempCommand[4] = (BYTE)'N';
	SendXRayCommand(TempCommand, 4, cGetFilament);
}

void CSpellman100WattMonoBlock::Get15VoltMonitor()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'L';
	TempCommand[2] = (BYTE)'V';
	TempCommand[3] = (BYTE)'P';
	TempCommand[4] = (BYTE)'S';
	SendXRayCommand(TempCommand, 4, cGet15V);
}

void CSpellman100WattMonoBlock::GetInterlockStatus()
{
//This is in the Get Faults for the XRB100W MonoBlock
}

void CSpellman100WattMonoBlock::GetSoftwareVersion()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'F';
	TempCommand[2] = (BYTE)'R';
	TempCommand[3] = (BYTE)'E';
	TempCommand[4] = (BYTE)'V';
	SendXRayCommand(TempCommand, 4, cGetSWVer);
}

void CSpellman100WattMonoBlock::GetHardwareVersion()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'H';
	TempCommand[2] = (BYTE)'W';
	TempCommand[3] = (BYTE)'V';
	TempCommand[4] = (BYTE)'R';
	SendXRayCommand(TempCommand, 4, cGetHWVer);
}

void CSpellman100WattMonoBlock::GetWebServerVersion() //Is Software Build Version for XRB100W MonoBlock
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'S';
	TempCommand[2] = (BYTE)'O';
	TempCommand[3] = (BYTE)'F';
	TempCommand[4] = (BYTE)'T';
	SendXRayCommand(TempCommand, 4, cGetSWBuild);
}

void CSpellman100WattMonoBlock::GetModelNumber()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'M';
	TempCommand[2] = (BYTE)'O';
	TempCommand[3] = (BYTE)'D';
	TempCommand[4] = (BYTE)'R';
	SendXRayCommand(TempCommand, 4, cGetModel);
}

void CSpellman100WattMonoBlock::SetInRemoteMode(bool TempRemote)
{
 //Not used on XRB100W MonoBlock
}

WORD CSpellman100WattMonoBlock::ReceiveDataCount()
{
	WORD TempCount = CSerialPortBase::ReceiveDataCount();
	WORD TempFinalCount = TempCount;
	if (TempCount > 5)
	{
		WORD TempIndex = vReceiveDataTail;
		WORD TempPreviousIndex = TempIndex;

		for (WORD TempLoop = 0; TempLoop < TempCount - 1; TempLoop++)
		{
			if (TempLoop > 1)
			if ((vReceiveDataBuffer[TempPreviousIndex] == cCarriageReturn) && (vReceiveDataBuffer[TempIndex] == cLineFeed))
			{
				TempFinalCount = TempLoop + 1;
				TempIndex++;
				TempLoop = TempCount;
			}
			TempPreviousIndex = TempIndex;
			if (TempIndex < cReceiveBufferSize - 1)
				TempIndex++;
			else
				TempIndex = 0;
		}
	}
	return TempFinalCount;
}

void CSpellman100WattMonoBlock::GetTemperature()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'T';
	TempCommand[2] = (BYTE)'E';
	TempCommand[3] = (BYTE)'M';
	TempCommand[4] = (BYTE)'P';
	SendXRayCommand(TempCommand, 4, cGetTemp);
}

bool CSpellman100WattMonoBlock::SerialMessageComplete()
{
	vLatestResponse = "";
	bool TempDone = false;
	while (!TempDone)
	{
		WORD TempCount = ReceiveDataCount();
		if (TempCount >= 1)  //5 is the shortest reply
		{
			if (vReceiveDataBuffer[vReceiveDataTail] != cSTX)
			{
				if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
					ReportErrorMessage(vHVPSType + " First Byte not STX, Throw away: 0x" + ByteToHex(vReceiveDataBuffer[vReceiveDataTail]), cEMailInspx,32000);

				if (vReceiveDataTail < cReceiveBufferSize - 1)
					vReceiveDataTail++;
				else
					vReceiveDataTail = 0;

				TempCount = ReceiveDataCount();
			}
		}
		
		if (TempCount >= 5)  //5 is the shortest reply
		{
			WORD TempIndex = vReceiveDataTail;
			WORD TempPreviousIndex = vReceiveDataTail;
			for (WORD TempLoop = 0; TempLoop < TempCount; TempLoop++)
			{
				vLatestResponse = vLatestResponse + (char)vReceiveDataBuffer[TempIndex];
				if ((vReceiveDataBuffer[TempPreviousIndex] == cCarriageReturn) && (vReceiveDataBuffer[TempIndex] == cLineFeed))// || ((vReceiveDataBuffer[TempIndex] == cSTX) && (TempLoop)))
				{
					TempDone = true;
					vWaitingForReply = false;
					TempLoop = TempCount + 1;
					if (vNoCommunications)
					{
						vNoCommunications = false;
						ReportErrorMessage("Established Communication with " + vHVPSType + " HVPS.", cAction,0);
						::PostThreadMessage(vGlobalCallingThreadID,cInitializeDXMMessage,0,0);
					}
					return true;
				}
				TempPreviousIndex = TempIndex;
				if (TempIndex < cReceiveBufferSize - 1)
					TempIndex++;
				else
					TempIndex = 0;
			}
			if (!TempDone)//if did not get valid message, and had enough bytes
			{
				BYTE TempMaximumExpectedRecievedDataLength = 20;

				if (TempCount > TempMaximumExpectedRecievedDataLength)
				{
					vLatestResponse = "";
					if (!vNoCommunications)
					{
						if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
						{
							CString TempString = ByteToHex(vReceiveDataBuffer[vReceiveDataTail]);
							ReportErrorMessage("Error " + vComPortString + ":No CR,LF - Removing one Byte: " + TempString, cWriteToLog, 0);
						}
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

void CSpellman100WattMonoBlock::RemoveMessageFromBuffer() 
{
	WORD TempCount = ReceiveDataCount();
	if (TempCount)
	{
		WORD TempPreviousIndex = vReceiveDataTail;
		if (vReceiveDataBuffer[vReceiveDataTail] == cSTX)
		{  //have STX, so remove message
			for (WORD TempLoop = 0; TempLoop < TempCount; TempLoop++)
			{
				if ((vReceiveDataBuffer[TempPreviousIndex] == cCarriageReturn) && (vReceiveDataBuffer[vReceiveDataTail] == cLineFeed))
					TempLoop = TempCount;
				vReceiveDataBuffer[vReceiveDataTail] = 0;  //clear the buffer so don't confuse old ETX

				TempPreviousIndex = vReceiveDataTail;
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

//enum {cNoCommand, cGetKV, cGetMA, cGetFilament, cGetFaults, cGetOnStatus, cGetSWVer, cGetModel, cGetHWVer, cGetSWBuild, cGet15V, cGetTemp,
//	cSetKV, cSetMA, cSetOn, cResetFaults}

void CSpellman100WattMonoBlock::DecodeMessage()
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
					TempCString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply NOT OK for SetKV"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
			}
			else
			{
				if (vLocalSystemData->vLogFile.vLogSerialData) 
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply OK for SetKV"),vGlobalSerialPortNumberXRaySource);
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
					TempCString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply NOT OK for SetMA"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
			}
			else
			{
				if (vLocalSystemData->vLogFile.vLogSerialData) 
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply OK for SetMA"),vGlobalSerialPortNumberXRaySource);
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
					TempCString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply NOT OK for TurnOn/Off"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
			}
			else
			{
				if (vLocalSystemData->vLogFile.vLogSerialData) 
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply OK for TurnOn/Off"),vGlobalSerialPortNumberXRaySource);
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
					TempCString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply NOT OK for ResetFaults"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
			}
			else
			{
				if (vLocalSystemData->vLogFile.vLogSerialData)
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply OK for ResetFaults"),vGlobalSerialPortNumberXRaySource);
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
					TempCString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply NOT OK for Enable/Disable Watch Dog"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
			}
			else
			{
				if (vLocalSystemData->vLogFile.vLogSerialData) 
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply OK for Enable/Disable Watch Dog"),vGlobalSerialPortNumberXRaySource);
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
					TempCString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply NOT OK for Tickle Watch Dog"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
			}
			else
			{
				if (vLocalSystemData->vLogFile.vLogSerialData) 
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply OK for Tickle Watch Dog"),vGlobalSerialPortNumberXRaySource);
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
			for (BYTE TempLoop = 2; TempLoop < 3; TempLoop++) // 1 character.  actualy get like V3.6//up to 11 characters
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
					TempCString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply OK for GetSWVer: "),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString + vSoftwareVersion, cDebugMessage);
				}
				vFoundXrayPort = true;
			}
			else
			{
				vSoftwareVersion = "SWV:";
				for (BYTE TempLoop = 1; TempLoop < 11; TempLoop++)
					vSoftwareVersion = vSoftwareVersion + (char)(TempResponse[TempLoop]);

				if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Replyfor GetSWVer"),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
				}
				if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
					ReportErrorMessage("Error - " + vSoftwareVersion, cEMailMaintenance,0);
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
					TempCString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply OK for GetHWVer: "),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString + vHardwareVersion, cDebugMessage);
				}
				vFoundXrayPort = true;
			}
			else
			{
				vHardwareVersion = vHVPSType + " HW Version: Bad Data:";
				for (BYTE TempLoop = 1; TempLoop < 3; TempLoop++)
					vHardwareVersion = vHardwareVersion + " 0x" + ByteToHex(TempResponse[TempLoop]);

				if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply NOT OK for GetHWVer"),vGlobalSerialPortNumberXRaySource);
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
					TempCString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply OK for GetSWBuild: "),vGlobalSerialPortNumberXRaySource);
					vLocalSystemData->vLogFile.WriteToLogFile(TempCString + vWebserverVersion, cDebugMessage);
				}
				vFoundXrayPort = true;
			}
			else
			{
				vWebserverVersion = vHVPSType + " Web: Bad Data:";
				for (BYTE TempLoop = 1; TempLoop < 5; TempLoop++)
					vWebserverVersion = vWebserverVersion + " 0x" + ByteToHex(TempResponse[TempLoop]);

				if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
				{
					CString TempCString;
					TempCString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply NOT OK for GetSWBuild"),vGlobalSerialPortNumberXRaySource);
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

					if (vLocalSystemData->vLogFile.vLogSerialData)
					{
						CString TempCString;
						TempCString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply OK for GetModel: "),vGlobalSerialPortNumberXRaySource);
						vLocalSystemData->vLogFile.WriteToLogFile(TempCString + vModelNumber, cDebugMessage);
					}
					vFoundXrayPort = true;
				}
				else
				{
					vModelNumber = vHVPSType + " Model: Bad Data:";
					for (BYTE TempLoop = 1; TempLoop < TempLength - 4; TempLoop++)
						vModelNumber = vModelNumber + " 0x" + ByteToHex(TempResponse[TempLoop]);

					if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
					{
						CString TempCString;
						TempCString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply NOT OK for GetModel"),vGlobalSerialPortNumberXRaySource);
						vLocalSystemData->vLogFile.WriteToLogFile(TempCString, cDebugMessage);
					}
					if (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen)
						ReportErrorMessage("Error - " + vModelNumber, cEMailInspx,0);
				}

				if (vModelNumber.Find(_T("X3547")) != -1)
				{
					if (TempLength > 13)
						vModelNumber = "Spellman XRB100W>S/N:" + vModelNumber;
					else
						vModelNumber = "Spellman XRB100W>" + vModelNumber;
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
					TempString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply OK for GetStatus: X-Rays On"),vGlobalSerialPortNumberXRaySource);
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
					TempString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply OK for GetStatus: X-Rays Off"),vGlobalSerialPortNumberXRaySource);
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
				TempString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply for GetFaults"),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempString, cDebugMessage);
			}

			if (TempResponse[1] == '1')
			{
				if (!vLocalSystemData->vXRayOnNotice) // if the X-Rays have gone off due to 3 arcs in 10 seconds
				{
					ClearXRayFault();
					//if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
					{
						::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						ReportErrorMessage(vHVPSType + " Error - X-Ray Source Arc Fault - Inspection Stopped", cCritical,9);
						ReportErrorMessage(vHVPSType + " Error - X-Ray Source Arc Fault - Inspection Stopped", cError,9);
						ReportErrorMessage(vHVPSType + " Error - Multiple X-Ray Source Arc Faults - Inspection Stopped", cEMailInspx,32000);
#ifdef CompileInHTTPClientOperationsGuardian
						if (vLocalConfigurationData->vEnableOperationsGuardian)
						{
							vGlobalOGLocationString = "XRay";
							vGlobalOGErrorCode = 103;
							::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageXRayFault, cOGSevereMessageXRayFault);
						}
#endif
					}
					//else
					//	ReportErrorMessage(vHVPSType + " Notice - X-Ray Source Arc Fault - X-Rays were turning off", cWriteToLog,9);
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
					::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);

					ReportErrorMessage(vHVPSType + " Error - X-Ray Source Too Hot. Inspection Stopped-Inside Temp: " + ConvertToDegreesCString(vLocalSystemData->vTemperatureAirInside,
						vLocalConfigurationData->vTemperatureAirInsideAdjust) + ", MonoBlock Temp: " + dtoa(vTemperature, 0), cError,9);

					ReportErrorMessage(vHVPSType + " Error - X-Ray Source Too Hot. Inspection Stopped-Inside Temp: " + ConvertToDegreesCString(vLocalSystemData->vTemperatureAirInside,
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
				else
					ReportErrorMessage(vHVPSType + " Error - X-Ray Source Too Hot. Not Inspecting-Inside Temp: " + ConvertToDegreesCString(vLocalSystemData->vTemperatureAirInside,
						vLocalConfigurationData->vTemperatureAirInsideAdjust), cEMailInspx,9);

				vFoundXrayPort = true;
			}
			if (TempResponse[3] == '1')
			{
				ClearXRayFault();
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
					ReportErrorMessage(vHVPSType + " Notice - X-Ray Source Over Voltage-Inspection had just stopped", cWriteToLog,9);
			}
			if (TempResponse[4] == '1')
			{
				ClearXRayFault();
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
#ifdef CompileInHTTPClientOperationsGuardian
					if (vLocalConfigurationData->vEnableOperationsGuardian)
					{
						vGlobalOGLocationString = "XRay";
						vGlobalOGErrorCode = 108;
						::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageXRayFault, cOGSevereMessageXRayFault);
					}
#endif
				}
				else
					ReportErrorMessage(vHVPSType + " Notice - X-Ray Source Under Voltage-Inspection had just stopped", cWriteToLog,9);
			}
			if (TempResponse[5] == '1')
			{
				ClearXRayFault();
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
#ifdef CompileInHTTPClientOperationsGuardian
					if (vLocalConfigurationData->vEnableOperationsGuardian)
					{
						vGlobalOGLocationString = "XRay";
						vGlobalOGErrorCode = 102;
						::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageXRayFault, cOGSevereMessageXRayFault);
					}
#endif
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
				ClearXRayFault();
				if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) && (vLocalSystemData->vXRayOnNotice) && (!vLocalSystemData->vSourceErrorReported))
				{
					vLocalSystemData->vSourceErrorReported = true;
					ReportErrorMessage(vHVPSType + " Error - Watch Dog Timeout-Inspection Stopped", cEMailInspx,9);
					ReportErrorMessage(vHVPSType + " Error - Lost Communications-WD-Inspection Stopped", cError,9);

					::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
#ifdef CompileInHTTPClientOperationsGuardian
					if (vLocalConfigurationData->vEnableOperationsGuardian)
					{
						vGlobalOGLocationString = "XRay";
						vGlobalOGErrorCode = 106;
						::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageXRayFault, cOGSevereMessageXRayFault);
					}
#endif
				}
				else
					ReportErrorMessage(vHVPSType + " Notice - Watch Dog Timeout, but not inspecting", cWriteToLog,0);
				vFoundXrayPort = true;
			}
			if (TempResponse[8] == '1')
			{
				if (!vLocalConfigurationData->vSimulateLightControlBoard) //don't say interlock open if simulating X-Rays
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
#ifdef CompileInHTTPClientOperationsGuardian
					if (vLocalConfigurationData->vEnableOperationsGuardian)
					{
						vGlobalOGLocationString = "XRay";
						vGlobalOGErrorCode = 111;
						::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageXRayFault, cOGSevereMessageXRayFault);
					}
#endif
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
				TempString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply for GetTemp: "),vGlobalSerialPortNumberXRaySource);
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
#ifdef CompileInHTTPClientOperationsGuardian
					if (vLocalConfigurationData->vEnableOperationsGuardian)
					{
						vGlobalOGLocationString = "xray";
						vGlobalOGErrorCode = 300;
						vGlobalOGLimitString = dtoa(vLocalConfigurationData->vMaximumMonoBlockTemperature, 0);
						::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageTemperatureFault, cOGSevereMessageTemperatureFault);
					}
#endif
				}
				else
					ReportErrorMessage(vHVPSType + " is Too Hot-Notice-Not Inspecting", cWriteToLog,0);
			}

			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-XRB100W MonoBlock-Converted Reply for GetTemp: "),vGlobalSerialPortNumberXRaySource);
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
				TempString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply for GetKV: "),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempString + TempReturnNumberString, cDebugMessage);
			}

			TempReturnValue = _wtoi(TempReturnNumberString);

			vLocalSystemData->vSourceVoltage = (double)TempReturnValue * vVoltageMaximumScale / 4095.0 / 6.0;

			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-XRB100W MonoBlock-Converted Reply for GetKV: "),vGlobalSerialPortNumberXRaySource);
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
				TempString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply for GetMA: "),vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempString + TempReturnNumberString, cDebugMessage);
			}

			TempReturnValue = _wtoi(TempReturnNumberString);

			vLocalSystemData->vSourceCurrent = 	(double)TempReturnValue * vCurrentMaximumScale / 4095.0;
			if (vLocalSystemData->vSourceCurrent < .01)
				vLocalSystemData->vSourceCurrent = 0;

			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-XRB100W MonoBlock-Converted Reply for GetMA: ") ,vGlobalSerialPortNumberXRaySource);
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
				TempString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply for GetVoltageScale: "), vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempString + TempReturnNumberString, cDebugMessage);
			}
			TempReturnValue = _wtoi(TempReturnNumberString);

			vVoltageMaximumScale = (double)TempReturnValue / 100.0;
			if (vVoltageMaximumScale == 70)
				vVoltageMaximumScale = vVoltageMaximumScale / .9;

			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-XRB100W MonoBlock-Converted Reply for GetVoltageScale: "), vGlobalSerialPortNumberXRaySource);
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
				TempString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply for GetCurrentScale: "), vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempString + TempReturnNumberString, cDebugMessage);
			}
			TempReturnValue = _wtoi(TempReturnNumberString);

			vCurrentMaximumScale = 	(double)TempReturnValue / 1000.0;
			if (vCurrentMaximumScale == 1.25)
				vCurrentMaximumScale = vCurrentMaximumScale / .9;

			
			double TempMaximumCurrent = vCurrentMaximumScale * 0.9; //for the 100 Watt XRB, the maximum current allowed is 10% less than the Scale
			if (vLocalConfigurationData->vMaximumCurrent > TempMaximumCurrent)
			{
				ReportErrorMessage(vHVPSType + " Maximum Current of: " + dtoa(vLocalConfigurationData->vMaximumCurrent, 2) + " was too high.  Set to: " + dtoa(TempMaximumCurrent, 2), cWriteToLog,0);
				vLocalConfigurationData->vMaximumCurrent = TempMaximumCurrent;
			}

			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-XRB100W MonoBlock-Converted Reply for GetCurrentScale: "), vGlobalSerialPortNumberXRaySource);
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
				TempString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply for GetFilament: "), vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempString + TempReturnNumberString, cDebugMessage);
			}
			TempReturnValue = _wtoi(TempReturnNumberString);

			vFilamentCurrentFeedbackMonitor = 	(double)TempReturnValue * vCurrentMaximumScale / 4095.0;
			if (vHaveNewFilamentFeedbackMonitorReading < 250)
				vHaveNewFilamentFeedbackMonitorReading++;

			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-XRB100W MonoBlock-Converted Reply for GetFilament: "), vGlobalSerialPortNumberXRaySource);
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
				TempString.Format(_T("Com%d-XRB100W MonoBlock-Decoded Reply for Get15V: "), vGlobalSerialPortNumberXRaySource);
				vLocalSystemData->vLogFile.WriteToLogFile(TempString + TempReturnNumberString, cDebugMessage);
			}
			TempReturnValue = _wtoi(TempReturnNumberString);

			v15VoltMonitor = (3972 -(double)TempReturnValue) * 0.006224; //XRB100W

			if ((vLocalSystemData->vLogFile.vLogSerialData) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
			{
				CString TempString;
				TempString.Format(_T("Com%d-XRB100W MonoBlock-Converted Reply for Get15V: ") ,vGlobalSerialPortNumberXRaySource);
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
				TempString.Format(_T("Com%d-XRB100W-Error-Unknown Reply: "), vGlobalSerialPortNumberXRaySource);
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

CString CSpellman100WattMonoBlock::GetNumberStringFromReply()
{
	WORD TempLength = vLatestResponse.GetLength();
	CString TempString = "";
	TempString = "";
	WORD TempIndex = vReceiveDataTail;

	if (TempLength > 4)
	for (WORD TempLoop = 1; TempLoop < 6; TempLoop++)
	{
		if ((vLatestResponse.GetAt(TempLoop) >= 0x30) && (vLatestResponse.GetAt(TempLoop) <= 0x39))
		{
			CString TempChar(vLatestResponse.GetAt(TempLoop));
			TempString = TempString + TempChar;
		}
		else
		if (vLatestResponse.GetAt(TempLoop) == cSemiColon)
			TempLoop = TempLength;
	}
	return TempString;
}

void CSpellman100WattMonoBlock::ShowSerialData()  //data received
{
	if (((vLocalSystemData->vLogFile.vLogSerialData) || (vLocalSystemData->vLogFile.vShowingSerialDataOnScreen)) && (vGlobalXrayNoCommMessageRcvdEver == cYesCommSeen))
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
			if (TempByte == cCarriageReturn)
			{
				TempString = TempString + "<CR>";
			}
			else
			if (TempByte == cLineFeed)
			{
				TempString = TempString + "<LF>";
			}
			else
			if (TempShowASCII)
				TempString = TempString + (char)TempByte;
			else
				TempString = TempString + "<" + ByteToHex(TempByte) + ">";

			if (TempLoop == TempLength - 4)
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

void CSpellman100WattMonoBlock::GetVoltageScale()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'S';
	TempCommand[2] = (BYTE)'L';
	TempCommand[3] = (BYTE)'V';
	TempCommand[4] = (BYTE)'R';
	SendXRayCommand(TempCommand, 4, cGetVoltageScale);
}
	
void CSpellman100WattMonoBlock::GetCurrentScale()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'S';
	TempCommand[2] = (BYTE)'L';
	TempCommand[3] = (BYTE)'I';
	TempCommand[4] = (BYTE)'R';
	SendXRayCommand(TempCommand, 4, cGetCurrentScale);
}

void CSpellman100WattMonoBlock::EnableWatchDog(bool TempEnable)//Spellman XRB-80 100 watt monoblock timeout is 10 seconds
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'W';
	TempCommand[2] = (BYTE)'D';
	TempCommand[3] = (BYTE)'T';
	TempCommand[4] = (BYTE)'E';
	TempCommand[5] = (BYTE)' ';
	if (TempEnable)
		TempCommand[6] = (BYTE)'1';
	else
		TempCommand[6] = (BYTE)'0';
	SendXRayCommand(TempCommand, 6, cSetEnableWatchDog);
}

void CSpellman100WattMonoBlock::TickleWatchDog() //Spellman XRB-80 100 watt monoblock timeout is 10 seconds
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'W';
	TempCommand[2] = (BYTE)'D';
	TempCommand[3] = (BYTE)'T';
	TempCommand[4] = (BYTE)'T';
	SendXRayCommand(TempCommand, 4, cTickleWatchDog);
	vWatchDogEnabled = true;
}

void CSpellman100WattMonoBlock::DisableWatchDogTimer()
{
	vWatchDogEnabled = false;
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'W';
	TempCommand[2] = (BYTE)'D';
	TempCommand[3] = (BYTE)'T';
	TempCommand[4] = (BYTE)'E';
	TempCommand[5] = (BYTE)' ';
	TempCommand[6] = (BYTE)'0';
	SendXRayCommand(TempCommand, 6, cSetEnableWatchDog);
}

void CSpellman100WattMonoBlock::DoBackGroundWork()
{
	//called at 10 hz
	if (vWaitingForReply)
	{
		//if (!vLocalConfigurationData->vSimulateLightControlBoard)
		if (vLocalSystemData->vACPowerOn)
		{
			if (vWaitingForReply < 250)
				vWaitingForReply++;
			if (vWaitingForReply == 20)
			{
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
	if (vNextBackGroundTask == 7)
	{
		GetTemperature(); //only supported on 100Watt MonoBlock
		vNextBackGroundTask = 0;
	}
	else
		vNextBackGroundTask = 0;
}



BOOL CSpellman100WattMonoBlock::OnIdle(LONG lCount)
{
	// TODO: Add your specialized code here and/or call the base class
	return CSpellmanDXMXRaySourcePort::OnIdle(lCount);
}


int CSpellman100WattMonoBlock::Run()
{
	// TODO: Add your specialized code here and/or call the base class
	return CSpellmanDXMXRaySourcePort::Run();
}


BOOL CSpellman100WattMonoBlock::PumpMessage()
{
	// TODO: Add your specialized code here and/or call the base class

	return CSpellmanDXMXRaySourcePort::PumpMessage();
}
