//ScanTrac Side View Source File
// XRaySource.cpp: implementation of the CXRaySource class.
//Controls the Spellman Model 3492, 210 Watt Monoblock for the TD
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "math.h"
#include "XRaySource.h"
#include "ScanTrac.h"
#include "SerialPort.h"
#include "SelectItemDialog.h"
#include "SystemConfigurationData.h"
#include "YesNoDialog.h"
#include "ImageAquisitionThread.h"

const BYTE cSemiColon = 0x3B;
const BYTE cSTX = 0x02;
const BYTE cCarriageReturn = 0x0D;
const BYTE cLineFeed = 0x0A;

extern DWORD vGlobalCallingThreadID;
extern CProduct *vGlobalCurrentProduct;
extern CImageAquisitionThread *vGlobalImageAquisitionThread;

IMPLEMENT_DYNCREATE(CXRaySource, CSerialPortBase) //, CWinThread)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXRaySource::CXRaySource()
{
	vNextBackGroundTask = 0;
	vLatestResponse = "";
	vModelNumber = "Spellman 3492 Monoblock";
	vHadLowCurrent = false;
	vHadLowVoltage = false;
	vInterLockStatus = true;
	vNoCommunications = true;
}

CXRaySource::~CXRaySource()
{

}

CString CXRaySource::GetNumberStringFromReply()
{
	WORD TempLength = vLatestResponse.GetLength();
	CString TempString = "";
	TempString = "";
	WORD TempIndex = vReceiveDataTail;

	if (TempLength > 5)
	for (WORD TempLoop = 5; TempLoop < TempLength; TempLoop++)
	{
		if ((vLatestResponse.GetAt(TempLoop) >= 0x30) && (vLatestResponse.GetAt(TempLoop) <= 0x39))
		{
			CString TempChar(vLatestResponse.GetAt(TempLoop));
			TempString = TempString + TempChar;
		}
		else
		if (vLatestResponse.GetAt(TempLoop) == '$')
			TempLoop = TempLength;
	}
	return TempString;
}

CString CXRaySource::GetHexNumberStringFromReply()
{
	WORD TempLength = vLatestResponse.GetLength();
	CString TempString = "";
	TempString = "";
	WORD TempIndex = vReceiveDataTail;

	if (TempLength > 5)
	for (WORD TempLoop = 5; TempLoop < TempLength; TempLoop++)
	{
		if ((vLatestResponse.GetAt(TempLoop) >= 0x30) && (vLatestResponse.GetAt(TempLoop) <= 0x39))
		{
			CString TempChar(vLatestResponse.GetAt(TempLoop));
			TempString = TempString + TempChar;
		}
		else
		if ((vLatestResponse.GetAt(TempLoop) >= 0x41) && (vLatestResponse.GetAt(TempLoop) <= 0x46))
		{
			CString TempChar(vLatestResponse.GetAt(TempLoop));
			TempString = TempString + TempChar;
		}
		else
		if (vLatestResponse.GetAt(TempLoop) == '$')
			TempLoop = TempLength;
	}
	return TempString;
}


bool CXRaySource::SerialMessageComplete()
{
	vLatestResponse = "";
	bool TempDone = false;
	while (!TempDone)
	{
		WORD TempCount = ReceiveDataCount();
		if (ReceiveDataCount() >= 12)  
		{
			if (vNoCommunications)
			{
				vNoCommunications = false;
				ReportErrorMessage("Established Communication with MonoBlock HVPS.", cAction,0);
				//::PostThreadMessage(vGlobalCallingThreadID,cInitializeDXMMessage,0,0);
			}

			WORD TempIndex = vReceiveDataTail;
			for (WORD TempLoop = 0; TempLoop < TempCount; TempLoop++)
			{
				CString TempChar = (char)(vReceiveDataBuffer[TempIndex]);
				BYTE TempByte = BYTE(TempChar.GetAt(0));
				vLatestResponse = vLatestResponse + TempChar;
				if (vReceiveDataBuffer[TempIndex] == cCarriageReturn)  //carriage return
				{
					TempDone = true;
					vWaitingForReply = false;
					TempLoop = TempCount + 1;
					return true;
				}
				if (TempIndex >= cReceiveBufferSize)
					TempIndex = 0;
				else
					TempIndex++;
			}
			if (!TempDone)
				vLatestResponse = "";
			if (!TempDone)//if did not get valid message, and had enough bytes
			{
				if (!vNoCommunications)
				{
					CString TempString = ByteToHex(vReceiveDataBuffer[vReceiveDataTail]);
					ReportErrorMessage("Error " + vComPortString + ":No ETX - Removing one Byte: " + TempString, cWriteToLog, 0);
				}
		
				vReceiveDataTail++; //must have wrong byte, pop one byte off and throw away
				if (vReceiveDataTail >= cReceiveBufferSize)
					vReceiveDataTail = vReceiveDataTail - cReceiveBufferSize;
				if (ReceiveDataCount() < cuControllerMessageLength)
				{
					TempDone = true;
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

void CXRaySource::RemoveMessageFromBuffer() 
{
	WORD TempCount = ReceiveDataCount();
	if (TempCount)
	{
		if (vReceiveDataBuffer[vReceiveDataTail] == '!')
		{  //have STX, so remove message
			for (WORD TempLoop = 0; TempLoop < TempCount; TempLoop++)
			{
				if (vReceiveDataTail < cReceiveBufferSize - 1)
					vReceiveDataTail++;
				else
					vReceiveDataTail = 0;
				if (vReceiveDataBuffer[vReceiveDataTail] == cCarriageReturn)
				{
					TempLoop = TempCount;
					if (vReceiveDataTail < cReceiveBufferSize - 1)
						vReceiveDataTail++;
					else
						vReceiveDataTail = 0;
				}
			}
		}
		else
		{  //don't have STX, so remove one byte
			if (vReceiveDataTail < cReceiveBufferSize - 1)
				vReceiveDataTail++;
			else
				vReceiveDataTail = 0;
		}
	}
}

void CXRaySource::ShowSerialData()  //data received
{
	if ((vLocalSystemData->vLogFile.vLogSerialData) ||
		(vLocalSystemData->vLogFile.vShowingSerialDataOnScreen))
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
				TempString = TempString + "STX";
			else
			if (TempByte == cCarriageReturn)
				TempString = TempString + "CR";
			else
			if (TempByte == cLineFeed)
				TempString = TempString + "LF";
			else
			if (TempByte == 0x20)
				TempString = TempString + "_";
			else
			if (TempByte == cSemiColon)
			{
				TempString = TempString + ";";
				TempShowASCII = false;
			}
			else
			if (TempShowASCII)
				TempString = TempString + (char)TempByte;
			else
				TempString = TempString + ByteToHex(TempByte);

			if (TempLoop == 0)
				TempString = TempString + ";";
			else
				TempString = TempString + ":";

			if (vReceiveDataBuffer[TempIndex] == cCarriageReturn)  
				TempLoop = TempLength;

			if (TempIndex < cReceiveBufferSize - 1)
				TempIndex++;
			else
				TempIndex = 0;
		}
		TempString = vComPortString + ":" + TempString;
		vLocalSystemData->vLogFile.WriteToLogFile(TempString,cInputMessage);
	}
}

void CXRaySource::DecodeMessage()
{
	vCommunicationsTimeOutCounter = 0;
	WORD TempReturnValue = 0;
	CString TempReturnNumberString = "";
	WORD TempLength = vLatestResponse.GetLength();
	
	vHasReceivedCommunication = true;
	
	if (TempLength == 0)
		ReportErrorMessage("Error - Latest response blank: " + vComPortString, cEMailInspx, 32000);
	else
	{
		tXRayCommand TempResponse;
		for (BYTE TempLoop = 0; TempLoop < 12; TempLoop++)
		{
			if (TempLoop < TempLength)
				TempResponse[TempLoop] = (BYTE)vLatestResponse.GetAt(TempLoop);
			else
				TempResponse[TempLoop] = 0;
		}

		if ((TempResponse[0] != '!') || (TempResponse[1] != '!'))
			ReportErrorMessage("Error - No STX at start of message: " + vComPortString, cWriteToLog, 0);
		else
		if ((TempResponse[9] != '$') || (TempResponse[10] != '$'))
			ReportErrorMessage("Error - No ETX at end of message: " + vComPortString, cEMailInspx, 32000);
		else
		if (TempResponse[11] != cCarriageReturn)
			ReportErrorMessage("Error - No carriage return at end of message: " + vComPortString, cEMailInspx, 32000);
		else
		if ((TempResponse[2] == 'D') && (TempResponse[3] == '4') && (TempResponse[4] == '0'))
		{  //latched status data
//			vGlobalXrayNoCommMessageRcvdEver = cYesCommSeen;
			TempReturnNumberString = GetHexNumberStringFromReply();
			
			TempReturnValue = HexToWord(TempReturnNumberString);

			if (vLocalConfigurationData->vSimulateLightControlBoard) //indicate interlock is OK if simulating interlocks and x-rays
				 TempReturnValue = TempReturnValue & 0xFF7F;

			if (!vLocalConfigurationData->vSimulateLightControlBoard) //indicate interlock is OK if simulating interlocks and x-rays
			if (TempReturnValue & 0xFFFE) //0x2FCE mask off on bit so only look at fault bits
			//if (vLocalSystemData->vSourceRampedUp)
			//if (!vLocalSystemData->vShuttingDownXRays)
			//if (!vLocalSystemData->vSourceErrorReported)
			//if (vLocalSystemData->vXRayOnNotice)
			{
				ClearXRayFault();
				if (TempReturnValue & 0x0002) 
				{
					if (vLocalSystemData->vXRayOnNotice)
					{
						vHadLowCurrent = 1;
						vHadLowVoltage = false;
						::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
						//::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						//ReportErrorMessage("MonoBlock Error - X-Ray Source Low Current, shut down.", cEMailMaintenance,9);
						//ReportErrorMessage("MonoBlock Error - X-Ray Source Low Current, shut down.", cEMailInspx,9);
					}
				}
				if (TempReturnValue & 0x0004) 
				{
					if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
					{
						vHadLowVoltage = 2;
						vHadLowCurrent = false;
						::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
						//::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						//ReportErrorMessage("MonoBlock Error - X-Ray Source Low Voltage, shut down.", cEMailMaintenance,9);
						//ReportErrorMessage("MonoBlock Error - X-Ray Source Low Voltage, shut down.", cEMailInspx,9);
					}
					else
						ReportErrorMessage("MonoBlock Error - X-Ray Source Under Voltage", cWriteToLog, 0);
				}
				if (TempReturnValue & 0x0008) 
				{
					if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
					{
						::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						ReportErrorMessage("MonoBlock Error - X-Ray Source Over Voltage - Inspection Stopped", cError,9);
						ReportErrorMessage("MonoBlock Error - X-Ray Source Over Voltage - Inspection Stopped", cEMailInspx,9);
					}
					else
						ReportErrorMessage("MonoBlock Error - X-Ray Source Over Voltage", cEMailMaintenance, 0);
				}
				if (TempReturnValue & 0x0040) 
				{
					if (vLocalSystemData->vXRayOnNotice)
					if (vLocalSystemData->vSourceRampedUp)
					{
						::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						ReportErrorMessage("Error - MonoBlock Lost Comm-WD, Inspection Stopped.", cError,9);
						ReportErrorMessage("MonoBlock Error - X-Ray Watch Dog Timeout - Inspection Stopped", cEMailInspx,9);
					}
					else
						ReportErrorMessage("MonoBlock Error - X-Ray Watch Dog Timeout", cEMailInspx,9);
				}
				if (TempReturnValue & 0x0080) 
				{
					if (vLocalSystemData->vSourceRampedUp)
					if (!vLocalSystemData->vShuttingDownXRays)
					{
						vHadLowCurrent = false;
						vHadLowVoltage = false;
						::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
						//::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						//ReportErrorMessage("Error - X-Ray Source Interlock Fault, shut down.", cEMailMaintenance,9);
						//ReportErrorMessage("Error - X-Ray Source Interlock Fault, shut down.", cEMailInspx,9);
					}
				}

				if (TempReturnValue & 0x0100) 
				{
					if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
					{
						::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						ReportErrorMessage("MonoBlock Error - Current Loop Fault - Inspection Stopped", cError,9);
						ReportErrorMessage("MonoBlock Error - Current Loop Fault - Inspection Stopped", cEMailInspx,9);
					}
					else
						ReportErrorMessage("MonoBlock Error - Current Loop Fault", cEMailInspx, 0);
				}
				if (TempReturnValue & 0x0200) 
				{
					if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
					{
						::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						ReportErrorMessage("MonoBlock Error -  Voltage Loop Fault - Inspection Stopped", cError,9);
						ReportErrorMessage("MonoBlock Error -  Voltage Loop Fault - Inspection Stopped", cEMailInspx,9);
					}
					else
						ReportErrorMessage("MonoBlock Error - Voltage Loop Fault", cEMailMaintenance, 0);
				}
				if (TempReturnValue & 0x0400) 
				{
					if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
					{
						::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						ReportErrorMessage("MonoBlock Error - Arc Fault - Inspection Stopped", cError,9);
						ReportErrorMessage("MonoBlock Error - Arc Fault - Inspection Stopped", cEMailInspx,9);
					}
					else
						ReportErrorMessage("MonoBlock Error - Arc Fault", cEMailMaintenance, 0);
				}
				if (TempReturnValue & 0x0800) 
				{
					if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
					{
						::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);

						ReportErrorMessage("MonoBlock Error - Over Temp - Inspection Stopped-Inside Temp: " + ConvertToDegreesCString(vLocalSystemData->vTemperatureAirInside,
							vLocalConfigurationData->vTemperatureAirInsideAdjust), cError,9);

						ReportErrorMessage("MonoBlock Error - Over Temp - Inspection Stopped-Inside Temp: " + ConvertToDegreesCString(vLocalSystemData->vTemperatureAirInside,
							vLocalConfigurationData->vTemperatureAirInsideAdjust), cEMailInspx,9);
					}
					else
						ReportErrorMessage("MonoBlock Error - Over Temp-Inside Temp: " + ConvertToDegreesCString(vLocalSystemData->vTemperatureAirInside,
							vLocalConfigurationData->vTemperatureAirInsideAdjust), cEMailMaintenance, 0);
				}
				if (TempReturnValue & 0x2000) 
				{
					if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
					{
						::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						ReportErrorMessage("MonoBlock Error - High Current - Inspection Stopped", cError,9);
						ReportErrorMessage("MonoBlock Error - High Current - Inspection Stopped", cEMailInspx,9);
					}
					else
						ReportErrorMessage("MonoBlock Error - High Current", cEMailMaintenance, 0);
				}
				if (TempReturnValue & 0x4000) 
				{
					if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
					{
						::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						ReportErrorMessage("MonoBlock Error - Under/Over Power - Inspection Stopped", cError,9);
						ReportErrorMessage("MonoBlock Error - Under/Over Power - Inspection Stopped", cEMailInspx,9);
					}
					else
						ReportErrorMessage("MonoBlock Error - Under/Over Power", cEMailMaintenance, 0);
				}
				if (TempReturnValue & 0x9030)
				{
					if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
					{
						::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						ReportErrorMessage("MonoBlock Error - Unknown Fault - Inspection Stopped" + WordToHex(TempReturnValue), cError,9);
						ReportErrorMessage("MonoBlock Error - Unknown Fault - Inspection Stopped" + WordToHex(TempReturnValue), cEMailInspx,9);
					}
					else
						ReportErrorMessage("MonoBlock Error - Unknown Fault: " + WordToHex(TempReturnValue), cEMailMaintenance, 0);
				}
			}
			if (!vLocalConfigurationData->vSimulateLightControlBoard) //indicate interlock is OK if simulating interlocks and x-rays
			if (TempReturnValue & 0x0080)
			{
				if (vInterLockStatus)
				{
					if (vLocalSystemData->vSourceRampedUp)
					if (!vLocalSystemData->vShuttingDownXRays)
					{
						::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
						//::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						//ReportErrorMessage("MonoBlock Error - X-Ray Source Interlock Fault - Inspection Stopped", cEMailMaintenance,9);
						//ReportErrorMessage("MonoBlock Error - X-Ray Source Interlock Fault - Inspection Stopped", cEMailInspx,9);
					}

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
					//vLocalSystemData->vHVPSInterlockDelay = 0;
					::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
				}
			}
			if (!vLocalConfigurationData->vSimulateLightControlBoard)
				vLocalSystemData->vXRayOnNotice = TempReturnValue & 0x0001;
		}
		else
		if ((TempResponse[2] == 'D') && (TempResponse[3] == '4') && (TempResponse[4] == '2'))
		{  //HV Voltage
//			vGlobalXrayNoCommMessageRcvdEver = cYesCommSeen;
			TempReturnNumberString = GetNumberStringFromReply();
			TempReturnValue = _wtoi(TempReturnNumberString);

			//if (vLocalSystemData->vNumberOfVoltageSamples < cNumberOfVoltageSamples)
			//	vLocalSystemData->vNumberOfVoltageSamples++;
			//vLocalSystemData->vSourceVoltageReading[vLocalSystemData->vXRayStatusIndex] = 
			vLocalSystemData->vSourceVoltage = (double)TempReturnValue / 60.0;//code sent is 10 times higher than voltage, we store as 1/6th so code 10 = 60KV

			VerifySourceWithinTolerance();
		}
		else
		if ((TempResponse[2] == 'D') && (TempResponse[3] == '4') && (TempResponse[4] == '3'))
		{  //HV Current
//			vGlobalXrayNoCommMessageRcvdEver = cYesCommSeen;
			TempReturnNumberString = GetNumberStringFromReply();
			TempReturnValue = _wtoi(TempReturnNumberString);

			vLocalSystemData->vSourceCurrent = 	(double)TempReturnValue / 1000.0;
			VerifySourceWithinTolerance();
		}
		else
		if ((TempResponse[2] == 'C') && (TempResponse[3] == 'C') && (TempResponse[4] == 'C'))
		{  //retransmit last command
//			vGlobalXrayNoCommMessageRcvdEver = cYesCommSeen;
		}
	}
}

void CXRaySource::SendXRaySourceSettings(double TempVoltage, double TempCurrent)
{
	vLocalSystemData->vLastXRayCurrentSetTime = CIFCOS::GetSystimeMillisecs();
	//Send X-Ray Voltage
	if (TempVoltage)
	{
		vLocalSystemData->vSourceVoltageMin = 254;
		vLocalSystemData->vSourceVoltageMax = 0;
	}
	double TempDouble = TempVoltage * 6;

	if (TempDouble > vLocalConfigurationData->vMaximumVoltage)
		TempDouble = vLocalConfigurationData->vMaximumVoltage;

	if ((TempVoltage) || (TempCurrent))
	{
		double TempSourceVoltageTolerance = .1 * vLocalSystemData->vRampXRayVoltageAmount;

		vLocalSystemData->vSourceVoltageMinRedLine = vLocalSystemData->vRampXRayVoltageAmount - TempSourceVoltageTolerance;
		vLocalSystemData->vSourceVoltageMaxRedLine = vLocalSystemData->vRampXRayVoltageAmount + TempSourceVoltageTolerance;

		if (vLocalSystemData->vSourceVoltageMinRedLine < 0)
			vLocalSystemData->vSourceVoltageMinRedLine = 0;
		if (vLocalSystemData->vSourceVoltageMaxRedLine > vLocalConfigurationData->vMaximumVoltage + TempSourceVoltageTolerance)
			vLocalSystemData->vSourceVoltageMaxRedLine = vLocalConfigurationData->vMaximumVoltage + TempSourceVoltageTolerance;

		vLocalSystemData->vRampXRayVoltageAmount = TempDouble / 6;
	}
	vLocalSystemData->vSourceRampedUp = false;

	ReportErrorMessage("Set X-Ray Source Voltage to: " + dtoa(TempDouble,2),cWriteToLog,0);
	
	if (!vLocalConfigurationData->vDemoMode)
	if (!vLocalConfigurationData->vSimulateLightControlBoard)
	//DON'T	turn x-rays on if in demo mode
		SetXRayVoltage(vLocalSystemData->vRampXRayVoltageAmount);
	Sleep(cSendCommandSleepTime);


	if ((!TempCurrent) && (TempVoltage))
		::PostThreadMessage(vGlobalCallingThreadID,cRampingSourceMessage,0,0);
	if ((TempCurrent) && (TempVoltage))
		SendSourceCurrentToUController(TempCurrent);
}

void CXRaySource::SendSourceCurrentToUController(double TempCurrent)
{
	//Send X-Ray Current
	if (TempCurrent < .1)
		TempCurrent = .1;

	vLocalSystemData->vLastXRayCurrentSetTime = CIFCOS::GetSystimeMillisecs();

	if (TempCurrent)
	{
		vLocalSystemData->vSourceCurrentMin = 100;
		vLocalSystemData->vSourceCurrentMax = 0;
	}

	double TempDouble = TempCurrent;

	double TempMaxValue = GetMaximumCurrentFromPower(vLocalSystemData->vRampXRayVoltageAmount, vLocalConfigurationData->vMaximumPower);
	if (TempMaxValue > vLocalConfigurationData->vMaximumCurrent)
		TempMaxValue = vLocalConfigurationData->vMaximumCurrent;

	if (TempDouble > TempMaxValue)
		TempDouble = TempMaxValue;

//	vLocalSystemData->vSourceCurrentSetting = TempDouble;

	ReportErrorMessage("Set X-Ray Source Current to: " + dtoa(TempDouble, 2),cWriteToLog,0);

	if (!vLocalConfigurationData->vDemoMode)
	if (!vLocalConfigurationData->vSimulateLightControlBoard)
	//DON'T	turn x-rays on if in demo mode
		SetXRayCurrent(TempDouble);
	
	if (TempDouble)
	{
		vLocalSystemData->vRampXRayCurrentAmount = TempDouble;
		double TempSourceCurrentTolerance = .1 * TempDouble;
		if (TempSourceCurrentTolerance < .25)
			TempSourceCurrentTolerance = .25;

		vLocalSystemData->vSourceCurrentMinRedLine = TempDouble - TempSourceCurrentTolerance;
		vLocalSystemData->vSourceCurrentMaxRedLine = TempDouble + TempSourceCurrentTolerance;

		if (vLocalSystemData->vSourceCurrentMinRedLine < 0)
			vLocalSystemData->vSourceCurrentMinRedLine = 0;
		if (vLocalSystemData->vSourceCurrentMaxRedLine > TempDouble + TempSourceCurrentTolerance)
			vLocalSystemData->vSourceCurrentMaxRedLine = TempDouble + TempSourceCurrentTolerance;
	}

	vLocalSystemData->vSourceRampedUp = false;
	if (TempDouble)
		::PostThreadMessage(vGlobalCallingThreadID,cRampingSourceMessage,0,0);
	Sleep(cSendCommandSleepTime);
}

void CXRaySource::VerifySourceWithinTolerance()
{
	// if source is on check voltage and current not high
	if (vLocalSystemData->vSourceRampedUp)
	if (!vLocalSystemData->vShuttingDownXRays)
	if (vLocalSystemData->vSystemRunMode != cCalibratingDetectorOffsetMode)
	if (vLocalSystemData->vSystemRunMode != cCalibratingDetectorEdgesMode)
	if (!vLocalSystemData->vAdjustingSourceSettings)
	//if (!vLocalConfigurationData->vSimulateLightControlBoard)
	{
		if (vLocalSystemData->vSourceVoltage > vLocalSystemData->vSourceVoltageMax)
			vLocalSystemData->vSourceVoltageMax = vLocalSystemData->vSourceVoltage;
		if (vLocalSystemData->vSourceVoltage < vLocalSystemData->vSourceVoltageMin)
			vLocalSystemData->vSourceVoltageMin = vLocalSystemData->vSourceVoltage;
		if (vLocalSystemData->vSourceCurrent > vLocalSystemData->vSourceCurrentMax)
			vLocalSystemData->vSourceCurrentMax = vLocalSystemData->vSourceCurrent;
		if (vLocalSystemData->vSourceCurrent < vLocalSystemData->vSourceCurrentMin)
			vLocalSystemData->vSourceCurrentMin = vLocalSystemData->vSourceCurrent;

		double DeltaV = 0;
		if (vGlobalCurrentProduct)
			DeltaV = (vLocalSystemData->vSourceVoltage - vLocalSystemData->vRampXRayVoltageAmount);
		double DeltaC = (vLocalSystemData->vSourceCurrent - (vLocalSystemData->vRampXRayCurrentAmount));

		double TempSourceVoltageTolerance = .1 * vLocalSystemData->vSourceVoltage;
		if (TempSourceVoltageTolerance < 6)
			TempSourceVoltageTolerance = 6;
		double TempSourceCurrentTolerance = .1 * vLocalSystemData->vSourceCurrent;
		if (TempSourceCurrentTolerance < .25)
			TempSourceCurrentTolerance = .25;

		if ((DeltaV > TempSourceVoltageTolerance) || (DeltaC > TempSourceCurrentTolerance) || 
			(DeltaV < -TempSourceVoltageTolerance) || (DeltaC < -TempSourceCurrentTolerance))
		{
			//shut down the source
			if (!vLocalConfigurationData->vSimulateLightControlBoard)
			if ((DeltaV < -TempSourceVoltageTolerance) || (DeltaC < -TempSourceCurrentTolerance))
			{
				if (vLocalSystemData->vSystemRunMode == cConditionXRayTubeMode)
				{
					ReportErrorMessage("Notice-X-Ray/Supply glitch: " + dtoa(vLocalSystemData->vXRayTubeConditionGlitchCount,0) + ", Delta Voltage: " +
						dtoa(DeltaV,2) + ", Delta Current: " + dtoa(DeltaC,2), cEMailInspx, 32000);

					if (DeltaV > TempSourceVoltageTolerance)
					{
						ReportErrorMessage("Notice-X-Ray/Supply glitch: " + dtoa(vLocalSystemData->vXRayTubeConditionGlitchCount,0) + ", Delta Voltage: " +
							dtoa(DeltaV,2) + ", Delta Current: " + dtoa(DeltaC,2), cEMailMaintenance,0);

						ReportErrorMessage("Notice-X-Ray/Supply glitch: " + dtoa(vLocalSystemData->vXRayTubeConditionGlitchCount,0) + ", Delta Voltage: " +
							dtoa(DeltaV,2) + ", Delta Current: " + dtoa(DeltaC,2), cEMailInspx, 32000);
					}
					vLocalSystemData->vXRayTubeConditionCurrentStep = -1;//set so will start over
					vLocalSystemData->vXRayTubeConditionStepTimeLeft = 1;//set so next second will do next step
					vLocalSystemData->vXRayTubeConditionGlitchCount++;

					if (vLocalSystemData->vXRayTubeConditionGlitchCount > 2)
					{
						if (DeltaV > TempSourceVoltageTolerance)
						{
							ReportErrorMessage("Error-X-Ray Tube Conditioning Failed, Check Connectors", cEMailMaintenance,0);
							ReportErrorMessage("Error-X-Ray Tube Conditioning Failed, Check Connectors", cEMailInspx, 32000);
						}
						vLocalSystemData->vXRayTubeConditionCurrentStep = 12;//set step so will end
					}

				}
				else
				if (!vLocalSystemData->vSourceErrorReported)
				{
					vLocalSystemData->vSourceErrorReported = true;
					if (DeltaV > TempSourceVoltageTolerance)
					{
						ReportErrorMessage("Error-X-Ray voltage too high.", cError,32000);
						ReportErrorMessage("Error-X-Ray voltage too high. Voltage: " + dtoa(vLocalSystemData->vSourceVoltage * 6, 1) + " Current: " + 
							dtoa(vLocalSystemData->vSourceCurrent, 2),cEMailInspx,32000);
						::PostThreadMessage(vGlobalCallingThreadID,cCheckHVPSVoltageSoonMessage,0,0);
					}
					else
					if (DeltaC > TempSourceCurrentTolerance)
					{
						ReportErrorMessage("Error-X-Ray current too high.", cError,32000);
						ReportErrorMessage("Error-X-Ray current too high. Voltage: " + dtoa(vLocalSystemData->vSourceVoltage * 6,1) + " Current: " + 
							dtoa(vLocalSystemData->vSourceCurrent, 2),cEMailInspx,32000);
						::PostThreadMessage(vGlobalCallingThreadID,cCheckHVPSVoltageSoonMessage,0,0);
					}
					else
					if (DeltaV < -TempSourceVoltageTolerance)
					{
						vHadLowCurrent = false;
						vHadLowVoltage = 2;
						::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
					}
					else
					if (DeltaC < -TempSourceCurrentTolerance)
					{
						vHadLowCurrent = 2;
						vHadLowVoltage = false;
						::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
					}
				}
			}
		}
	}
}

void CXRaySource::TurnOnOffXRays(BYTE TempOn)
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'A';
	TempCommand[2] = (BYTE)'0';
	TempCommand[3] = (BYTE)'0';
	TempCommand[4] = (BYTE)'0';
	TempCommand[5] = (BYTE)'0';
	TempCommand[6] = (BYTE)'0';
	TempCommand[7] = (BYTE)'0';
	if (TempOn)
	if (!vLocalConfigurationData->vDemoMode)
	if (!vLocalConfigurationData->vSimulateLightControlBoard)
		TempCommand[3] = (BYTE)'1';
	SendXRayCommand(TempCommand,7);
}

void CXRaySource::TurnOnXRayWatchDog()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'A';
	TempCommand[2] = (BYTE)'0';
	TempCommand[3] = (BYTE)'B';
	TempCommand[4] = (BYTE)'0';
	TempCommand[5] = (BYTE)'0';
	TempCommand[6] = (BYTE)'1';
	TempCommand[7] = (BYTE)'0';
	SendXRayCommand(TempCommand,7);
}

void CXRaySource::TickleXRayWatchDog()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'A';
	TempCommand[2] = (BYTE)'0';
	TempCommand[3] = (BYTE)'A';
	TempCommand[4] = (BYTE)'0';
	TempCommand[5] = (BYTE)'0';
	TempCommand[6] = (BYTE)'0';
	TempCommand[7] = (BYTE)'0';
	if (vLocalSystemData->vLogFile.vLogSyncData)
		SendXRayCommand(TempCommand,7);
}

void CXRaySource::ClearXRayFault()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'A';
	TempCommand[2] = (BYTE)'0';
	TempCommand[3] = (BYTE)'7';
	TempCommand[4] = (BYTE)'0';
	TempCommand[5] = (BYTE)'0';
	TempCommand[6] = (BYTE)'0';
	TempCommand[7] = (BYTE)'0';
	SendXRayCommand(TempCommand,7);
}

void CXRaySource::SetXRayVoltage(double TempVoltage)
{
	if (TempVoltage > vLocalConfigurationData->vMaximumVoltage / 6)
		TempVoltage = vLocalConfigurationData->vMaximumVoltage / 6;

	if (TempVoltage) //don't send zero as will give an error under power
	{
		tXRayCommand TempCommand;
		TempCommand[1] = (BYTE)'A';
		TempCommand[2] = (BYTE)'0';
		TempCommand[3] = (BYTE)'2';
		TempCommand[4] = (BYTE)'0';
		TempCommand[5] = (BYTE)'0';
		TempCommand[6] = (BYTE)'0';
		TempCommand[7] = (BYTE)'0';
		double TempCode = TempVoltage * 60; //times 6 as ours is 1/6 th of value or 10 = 60KV, times 10 as the code to send is in tenths of KV
		if (TempCode > 800)
			TempCode = 800;
		CString TempString = dtoa(TempCode,0);
		BYTE TempLength = TempString.GetLength();
		if (!vLocalConfigurationData->vDemoMode)
		if (!vLocalConfigurationData->vSimulateLightControlBoard)
		for (BYTE TempLoop = 0; TempLoop < TempLength; TempLoop++)
			TempCommand[8 - TempLength + TempLoop] = (BYTE)TempString.GetAt(TempLoop);
		SendXRayCommand(TempCommand,7);
	}
}

void CXRaySource::SetXRayCurrent(double TempCurrent)
{
	if (TempCurrent > vLocalConfigurationData->vMaximumCurrent)
		TempCurrent = vLocalConfigurationData->vMaximumCurrent;

	if (TempCurrent < .1) //mono block has a minimum current of .1
		TempCurrent = .1;
	if (TempCurrent) //don't send zero as will give an error under power
	{
		if (TempCurrent < 0.1)
			TempCurrent = 0.1;
		tXRayCommand TempCommand;
		TempCommand[1] = (BYTE)'A';
		TempCommand[2] = (BYTE)'0';
		TempCommand[3] = (BYTE)'3';
		TempCommand[4] = (BYTE)'0';
		TempCommand[5] = (BYTE)'0';
		TempCommand[6] = (BYTE)'0';
		TempCommand[7] = (BYTE)'0';
		double TempCode = TempCurrent * 1000; //code is for Thousanths of milli-amps, so 6mA = code 6000
		if (TempCode > 6000)
			TempCode = 6000;
		CString TempString = dtoa(TempCode,0);
		BYTE TempLength = TempString.GetLength();
		if (!vLocalConfigurationData->vDemoMode)
		if (!vLocalConfigurationData->vSimulateLightControlBoard)
		for (BYTE TempLoop = 0; TempLoop < TempLength; TempLoop++)
			TempCommand[8 - TempLength + TempLoop] = (BYTE)TempString.GetAt(TempLoop);
		SendXRayCommand(TempCommand,7);
	}
}

void CXRaySource::GetXRayVoltage()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'A';
	TempCommand[2] = (BYTE)'0';
	TempCommand[3] = (BYTE)'4';
	TempCommand[4] = (BYTE)'0';
	TempCommand[5] = (BYTE)'0';
	TempCommand[6] = (BYTE)'0';
	TempCommand[7] = (BYTE)'0';
	SendXRayCommand(TempCommand,7);
}

void CXRaySource::GetXRayCurrent()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'A';
	TempCommand[2] = (BYTE)'0';
	TempCommand[3] = (BYTE)'5';
	TempCommand[4] = (BYTE)'0';
	TempCommand[5] = (BYTE)'0';
	TempCommand[6] = (BYTE)'0';
	TempCommand[7] = (BYTE)'0';
	SendXRayCommand(TempCommand,7);
}

void CXRaySource::GetXRayStatus()
{
	tXRayCommand TempCommand;
	TempCommand[1] = (BYTE)'A';
	TempCommand[2] = (BYTE)'0';
	TempCommand[3] = (BYTE)'6';
	TempCommand[4] = (BYTE)'0';
	TempCommand[5] = (BYTE)'0';
	TempCommand[6] = (BYTE)'0';
	TempCommand[7] = (BYTE)'0';
	SendXRayCommand(TempCommand,7);
}

void CXRaySource::DoBackGroundWork()
{
	if (vNextBackGroundTask == 0)
	{
		GetXRayCurrent();
		vNextBackGroundTask++;
	}
	else
	if (vNextBackGroundTask == 1)
	{
		GetXRayVoltage();
		vNextBackGroundTask++;
	}
	else
	if (vNextBackGroundTask == 2)
	{
		GetXRayStatus();
		vNextBackGroundTask = 0;
	}
	else
		vNextBackGroundTask = 0;
}

void CXRaySource::SendXRayCommand(tXRayCommand TempData, BYTE TempNumberOfBytes)
{
	tXRayCommand TempCommand;
	TempCommand[0] = (BYTE)'!';
	TempCommand[1] = (BYTE)'!';
	for (BYTE TempLoop = 1; TempLoop <= TempNumberOfBytes; TempLoop++)
	  TempCommand[TempLoop + 1] = TempData[TempLoop];
	TempCommand[TempNumberOfBytes + 2] = (BYTE)'$';
	TempCommand[TempNumberOfBytes + 3] = (BYTE)'$';
	TempCommand[TempNumberOfBytes + 4] = cCarriageReturn;

	vHasTransmittedCommunication = true;

	BYTE TempCommandLength = TempNumberOfBytes + 5;
	if (vPortOpen)
	{
		DWORD TempErrorCode = 0;
		DWORD TempBytesWritten = 0;
		int TempDidIt = 0;
		bool TempShowASCII = true;

		TempDidIt = WriteFile(vWinPortHandle,&TempCommand,
			TempCommandLength,&TempBytesWritten,
			&vWriteOverlappedStructure);
 		TempErrorCode = GetLastError(); 
		if ((TempErrorCode == ERROR_IO_PENDING) ||
			(TempDidIt))
		{
			vWaitingForReply = true;
			CString TempString("");
			TempString = "";
			//Log to file if configured to
			if ((vLocalSystemData->vLogFile.vLogSerialData) ||
				(vLocalSystemData->vLogFile.vShowingSerialDataOnScreen))
			{
				//add the decimal values of the string on the end
				for (BYTE TempLoop = 0; TempLoop < TempCommandLength; TempLoop++)
				{
					BYTE TempByte = TempCommand[TempLoop];
					if (TempByte == cSTX)
						TempString = TempString + "STX";
					else
					if (TempByte == 0x0D)
						TempString = TempString + "CR";
					else
					if (TempByte == cLineFeed)
						TempString = TempString + "LF";
					else
					if (TempByte == 0x20)
						TempString = TempString + "_";
					else
					if (TempByte == cSemiColon)
					{
						TempString = TempString + ";";
						TempShowASCII = false;
					}
					else
					if (TempShowASCII)
						TempString = TempString + (char)TempByte;
					else
						TempString = TempString + ByteToHex(TempByte);

					if (TempLoop == 0)
						TempString = TempString + ";";
					else
						TempString = TempString + ":";
				}
				TempString = vComPortString + ":" + TempString;
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
		Sleep(10);
	}
}


