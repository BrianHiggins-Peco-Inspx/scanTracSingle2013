//ScanTrac Side View Source File
// ACPowerMoniter.cpp: implementation of the CACPowerMoniter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "scantrac.h"
#include "ACPowerMoniter.h"
#include "SerialPortBase.h"
#include "SerialPortReview.h"
//#include "SerialPort.h"
//#include "SpellmanDXMXRaySourcePort.h" //for cEmergencyShutXRaysDownMessage

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const BYTE cSemiColon = 0x3B;
const BYTE cSTX = 0x02;
const BYTE cCarriageReturn = 0x0D;
const BYTE cLineFeed = 0x0A;
const BYTE cPowerTronicsETX = 3;
const BYTE cComma = 0x2c;

IMPLEMENT_DYNCREATE(CACPowerMoniter, CSerialPortBase) //, CWinThread)


//extern DWORD vGlobalCallingThreadID;
//extern CSerialPort *vGlobaluCSerialPort;
extern int vGlobalSerialPortNumberACPowerMonitor;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CACPowerMoniter::CACPowerMoniter()
{
	//10/26/2009
	vLastError = "";
	vOldHour = "";
	for (WORD TempLoop = 0; TempLoop < 16; TempLoop++)
		vACErrorCounter[TempLoop] = 0;

	vComPortString = "Com AC Power Monitor";
	vComPortNumber = vGlobalSerialPortNumberACPowerMonitor;	//was 1
	vLatestResponse = "";
	vACVoltMonitor = 0;
	vLogACPowerMoniterData = true;
	vLength;
	vFilePosition;
	vErrorsOccuredWait = 0;
	//vPowerFailure = -1;
	//vPowerError = -1;
	vGaveACVoltageWarning = false;
	vCurrentACInputVoltage = 0;
	vCurrentACInputVoltageMinimum = 500;
	vCurrentACInputVoltageMaximum = 0;
	vDataIndex = 0;
	vGaveBelow108VoltsError = 0;
	vGaveBrownOutError = 0;

	for (int TempIndex = 0; TempIndex < 250; TempIndex++)
		vResponse[TempIndex] = 0;

	for (int TempIndex = 0; TempIndex < cNumberOfACVoltageSamples; TempIndex++)
		vACInputVoltageData[TempIndex] = 0;

	vCommunicationsACPowerMonitorDataCount = 0;
	SyncFromACPowerMonitorSeen = 0;
	vTestACPowerMoniterExists = cNoCommSeenEver;
#ifdef _DEBUG
	vTestACPowerMoniterExists = cYesCommSeen;
#endif
	
	ReportComNotDefaultDone = false;
}

CACPowerMoniter::~CACPowerMoniter()
{
	IACPowerMoniterLogFile.~CACPowerMoniterLogFile();
	IErrorACPowerMoniterLogFile.~CACPowerMoniterLogFile();
}

void CACPowerMoniter::InitializeACPowerMoniter()
{
	OpenACPowerMoniterLogFiles();
//	GetNextAC
}

CString CACPowerMoniter::ReadACPowerMoniterLogFiles()
{
	return IErrorACPowerMoniterLogFile.GetACPowerMoniterErrorLogData(cToday);
}

void CACPowerMoniter::OpenACPowerMoniterLogFiles()
{
	IACPowerMoniterLogFile.SetACPowerMoniterFormat(true, cSTANDARDLOG);
	IErrorACPowerMoniterLogFile.SetACPowerMoniterFormat(true, cERRORLOG);
	//opening All log
	if (IACPowerMoniterLogFile.SetupLogFileName())
	{
		if (!IErrorACPowerMoniterLogFile.SetupLogFileName())
		{
			ReportErrorMessage("Error-Could Not Open AC Power Monitor Log File", cEMailInspx,32000); 
			#ifdef _DEBUG
				//afxDump << "Configuration file could not be opened " << TempFileException->m_cause << "\n";
				afxDump << " Tried to open AC Power Monitor Log and Failed "; 
			#endif
		}
	}
}

bool CACPowerMoniter::SerialMessageComplete()
{
	vLatestResponse = "";
	bool TempDone = false;
	
	for (WORD TempLoop = 0; TempLoop < 250; TempLoop++)
		vResponse[TempLoop]=0;

	WORD TempCount = ReceiveDataCount();
	vCommunicationsACPowerMonitorDataCount += TempCount;
	if (TempCount)  
	{
		WORD TempIndex = vReceiveDataTail;
		//increment Temp Index until start at start of text
		while (vReceiveDataBuffer[TempIndex] != cSTX)
		{
			if (TempIndex < cReceiveBufferSize - 1)
				TempIndex++;
			else
				TempIndex = 0;
		}
			vReceiveDataTail = TempIndex;
		for (WORD TempLoop = 0; TempLoop <= TempCount; TempLoop++)
		{
			vLatestResponse = vLatestResponse + (char)vReceiveDataBuffer[TempIndex];
			if (TempLoop < 250)
				vResponse[TempLoop] = vReceiveDataBuffer[TempIndex];

			//this is a fix, it doesn't always seem to get the 3 end character.
			if ((vReceiveDataBuffer[TempIndex] == 2) && (TempLoop != 0))
			{ 
				if (TempLoop < 250)
					vResponse[TempLoop] = 0;
				int tempx = 2;
				vLength = TempLoop;
				return true;
			}
			if ((BYTE)vReceiveDataBuffer[TempIndex] == (BYTE)cPowerTronicsETX)
			{
				vLength = TempLoop+1;
//				if (vNoCommunications)
//				{
//					vNoCommunications = false;
//					ReportErrorMessage("Established Communication with AC Power Monitor, but No Communications", cAction,0);
//				}					
				return true;
			}

			if (TempIndex < cReceiveBufferSize - 1)
				TempIndex++;
			else
				TempIndex = 0;
		}
		
		vLatestResponse = "";				
		return false;
	}
	else
		return false;
}

void CACPowerMoniter::RemoveMessageFromBuffer() 
{
	WORD TempCount = ReceiveDataCount();
	if (TempCount)
	{
		if (vReceiveDataBuffer[vReceiveDataTail] == cSTX)
		{  //have STX, so remove message
			for (WORD TempLoop = 0; TempLoop < TempCount; TempLoop++)
			{
				if (vReceiveDataBuffer[vReceiveDataTail] == cPowerTronicsETX)
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

void CACPowerMoniter::ShowSerialData()  //log data
{
	if (vLogACPowerMoniterData) 
	{
		WORD TempLength = ReceiveDataCount();
		CString TempString = "";
		TempString = "";
		BYTE TempByte = 0;
		WORD TempIndex = vReceiveDataTail;

		// #ifdef _DEBUG
			//afxDump << "Configuration file could not be opened " << TempFileException->m_cause << "\n";
	//		afxDump << "This is a test"; 
		//
//			#endif

		while (TempLength)
		{
			TempByte = vReceiveDataBuffer[TempIndex];
		//	TempString = TempString + (char)TempByte;
			TempLength--;
			TempIndex++;
		}
	//	for debug only
		//TempString = vComPortString + ":" + TempString;
		//#ifdef _DEBUG
			//afxDump << "Configuration file could not be opened " << TempFileException->m_cause << "\n";
//				afxDump << TempString; 
		//	#endif
	//		IACPowerMoniterLogFile.WriteToLogFile(TempString,cInputMessage);
		//for debug only
	}
}

//called by doBackgroundWork if IErrorACPowerMoniterLogFile.vDoResolveErrors == true
void CACPowerMoniter::ResolveErrors()
{
	bool TempReturnAfterResolving = false;
	CString TempCurrentTime = "", TempCurrentDay = "";
	//IErrorACPowerMoniterLogFile.vNumOfErrors;
	//IErrorACPowerMoniterLogFile.vErrorStrings;
	
	//figures out which message to show on screen
	/*
	if ((vPowerFailure != -1) && (vPowerFailure < 50)) // if a power failure occures (-1 means no power failure), and is in bounds (array is 50 long), then use the power failure message
	{
		if (vPowerFailure > IErrorACPowerMoniterLogFile.vNumOfErrors)
			vPowerFailure = 0;
		TempErrorMessageString = IErrorACPowerMoniterLogFile.vErrorStrings[vPowerFailure]; 
	}
	else 
	if ((vPowerError != -1) && (vPowerError < 50))	//if a power error message occures (-1 means no power error), and is in bounds of the array, use that power error message
	{
		if (vPowerError > IErrorACPowerMoniterLogFile.vNumOfErrors)
			vPowerError = 0;
		TempErrorMessageString = IErrorACPowerMoniterLogFile.vErrorStrings[vPowerError]; 
	}
	else	//else use the first message that occured
		//TempErrorMessageString = IErrorACPowerMoniterLogFile.vErrorStrings[0]; 
		*/

	if (vLastError != "")	//if that message exists continue
	{
/*		//calculate the time between the time stamp on it and now
		TempCurrentTime = CTime::GetCurrentTime().Format("%H%M%S");

		CString TempHoursOff = dtoa(_wtoi(TempCurrentTime.Mid(0,2)) - _wtoi(TempErrorMessageString.Mid(0,2)));
		CString TempMinutesOff = dtoa(_wtoi(TempCurrentTime.Mid(2,2)) - _wtoi(TempErrorMessageString.Mid(3,2)));
		
		CString TempSecondsOff = dtoa(_wtoi(TempCurrentTime.Mid(4,2)) - _wtoi(TempErrorMessageString.Mid(6,2)) - 2); //minus two for wait algorithm to collect all errors in a row.
		if(_wtoi(TempSecondsOff) < 0)
		{
			TempMinutesOff = dtoa(_wtoi(TempMinutesOff) - 1);
			TempSecondsOff = dtoa(_wtoi(TempSecondsOff) + 60);
		}
		if (_wtoi(TempMinutesOff) < 0)
		{
			TempHoursOff = dtoa(_wtoi(TempHoursOff)-1);
			TempMinutesOff = dtoa(_wtoi(TempMinutesOff) + 60);
		}
		if (TempHoursOff.GetLength() < 2)
			TempHoursOff = "0" + TempHoursOff;
		if (TempMinutesOff.GetLength() < 2)
			TempMinutesOff = "0" + TempMinutesOff;
		if (TempSecondsOff.GetLength() < 2)
			TempSecondsOff = "0" + TempSecondsOff;

		int TempTimeOff = _wtoi(TempHoursOff + TempMinutesOff + TempSecondsOff);	//temp time off is the result of the calculation [currentTime]-[messageTimeStamp] 
		
		if (vPowerFailure == -1)	//if it was not a power failure that occured
		{
			if (TempTimeOff > 3)	//and the message was recieved over three seconds ago
			{
				if (vPowerError == -1)	//and if it was not a power error that occured
				{
					if (IErrorACPowerMoniterLogFile.vGaveInspxMessageOverflowWarning)	//if there were too many messages so that it overflowed, just show the first message
					{
						vPowerFailure = 0;
				//		IErrorACPowerMoniterLogFile.vErrorStrings[48] 
				//			= TempErrorMessageString.Mid(0,9) + "Event - Power Turned Off" + (char)10;
					}
					else
					{		//make own power failure message, if there was not an overflow of error messages
						vPowerFailure = IErrorACPowerMoniterLogFile.vNumOfErrors;

						if (IErrorACPowerMoniterLogFile.vNumOfErrors < 50)  
						{
							ErrorACPowerMoniterLogFile.vErrorStrings[IErrorACPowerMoniterLogFile.vNumOfErrors] 
								= TempErrorMessageString.Mid(0,9) + "Event - Power Turned Off" + (char)10;
							IErrorACPowerMoniterLogFile.vNumOfErrors++;
						}
					}
				}
			}
			else //and the message occured less than three seconds ago (not a power failure message)
				TempReturnAfterResolving = true;
		}
		if (vPowerFailure == -1)	//if there was no power Failure
		{
			if (vPowerError == -1)	//and no power error
				vPowerFailure = 0;	//use the first message in the array as the message that is visible, vpowerfailure is an index at this point
			else
				vPowerFailure = vPowerError; //if there was a power error, let power error message index transfer to power failure variable
		}


		//int TempLoop = IErrorACPowerMoniterLogFile.vNumOfErrors;

		//if (TempLoop > 49)
		//	TempLoop = 49;
		//for (i = 0; i < TempLoop; i++)	//loop #of error messages times
		//{
		//	if ((i != vPowerFailure) && (i < 50))	//put a slash infront of all messages that you don't want to show
		//		IErrorACPowerMoniterLogFile.vErrorStrings[i] = "/" + IErrorACPowerMoniterLogFile.vErrorStrings[i];
		//}
		if (vPowerError != -1)
			TempErrorMessageString = " Power was low";
		else
			TempErrorMessageString = " Power was off";

		vPowerFailure = -1;
		vPowerError = -1;
*/

		IErrorACPowerMoniterLogFile.WriteToLogFile(vLastError, cResolveErrorMessages);

		if (TempReturnAfterResolving == true)
		{
			if (vGaveACVoltageWarning == 1)
			{
				vGaveACVoltageWarning = 2;
				ReportErrorMessage("AC Voltage Error, Check AC Voltage Problems Log", cEMailMaintenance,0);
			}
			return;
		}

/*		if (TempTimeOff == 0 )
			IErrorACPowerMoniterLogFile.WriteToLogFile(TempErrorMessageString + " for less than 1 second.", cErrorPowerOnMessage);
		else 
		if (TempTimeOff < 200)
		{
			IErrorACPowerMoniterLogFile.WriteToLogFile(TempErrorMessageString + " for " + TempHoursOff + " hours, "+ TempMinutesOff 
				+ " minutes, " + TempSecondsOff + " seconds.", cErrorPowerOnMessage);
		}
		else
			IErrorACPowerMoniterLogFile.WriteToLogFile(TempErrorMessageString + " for " + TempHoursOff + " hours, "+ TempMinutesOff 
				+ " minutes, " + TempSecondsOff + " seconds.", cInputMessage);
*/	}
	else	//if the message doesn't exist, write the error messages to the log file
		IErrorACPowerMoniterLogFile.WriteToLogFile(vLastError, cResolveErrorMessages);

	if (IACPowerMoniterLogFile.vTimeOffMessage != "")
	{
		IErrorACPowerMoniterLogFile.WriteToLogFile(IACPowerMoniterLogFile.vTimeOffMessage, cErrorPowerOnMessage);
		IACPowerMoniterLogFile.vTimeOffMessage = "";
	}

	//handle yellow messages
	if (vGaveACVoltageWarning == 1)
	{
		vGaveACVoltageWarning = 2;
		ReportErrorMessage("AC Voltage Error, Check AC Voltage Problems Log",cEMailMaintenance,0);
	}
	vLastError = "";
	IACPowerMoniterLogFile.vNumOfErrors = 0;
}

//puts message or message data into appropriate array to be handled later
void CACPowerMoniter::DecodeMessage()
{
	vCommunicationsTimeOutCounter = 0;
	WORD TempReturnValue = 0;
	CString TempReturnNumberString = "";
	int i;

	int TempVoltageLevel = 0;
	WORD TempIndex = vReceiveDataTail;
	CString TempDebugString = "";
	CString TempResponseString = "";
	CString TempVoltageString = "";
	vHasReceivedCommunication = true;

	if (vLength > 50)
	{
		ReportErrorMessage("Error - AC Power Monitor Event Message too long: " + dtoa(vLength,0), cEMailInspx, 32000);
		vLength = 50;
	}

	for (WORD TempLoop = 0; TempLoop < vLength; TempLoop++)
	{
//		if((vResponse[TempLoop]==2) && (TempLoop != 0))
//			int Tempx = 2;

		if ((TempLoop != 0) && (TempLoop != vLength - 1)  && (TempLoop != vLength - 3) && (TempLoop != vLength - 2))
		if (TempLoop < 250)
			TempResponseString = TempResponseString + (char)vResponse[TempLoop];
	}
	//	TempDebugString = TempDebugString+TempResponseString;

	//WORD TempLength = vLatestResponse.GetLength();
	if (vLength == 0)
		ReportErrorMessage("Error - AC Power Monitor latest response blank: " + vComPortString, cEMailInspx, 32000);
	else
	{
		if (vResponse[0] != cSTX)
			ReportErrorMessage("Error - No STX at start of message: " + vComPortString, cWriteToLog, 0);
//		else
//		if (vResponse[vLength - 1] != cPowerTronicsETX)
//			ReportErrorMessage("Error - No ETX at end of message: " + vComPortString, 
//				cEMailInspx, 32000);
//		;
//		else
		if (vResponse[1] == 'A') //ACV Current Voltage
		{
			if ((SyncFromACPowerMonitorSeen > 2) && (vGlobalPortHereOK[vGlobalSerialPortNumberACPowerMonitor] != cCURRENT_COMM_AC_POWER_MONITOR_SERIAL_PORT_NUMBER))
			{
				vGlobalPortHereOK[vGlobalSerialPortNumberACPowerMonitor] = cCURRENT_COMM_AC_POWER_MONITOR_SERIAL_PORT_NUMBER;
				vTestACPowerMoniterExists = cYesCommSeen;
				if ((vComPortNumber != cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER) && (ReportComNotDefaultDone == false))
				{
					ReportComNotDefaultDone = true;
					CString TempAString;
					TempAString.Format(_T("Error-AC Power Monitor Com Port %d, Default Com 4"),vGlobalSerialPortNumberACPowerMonitor);
					ReportErrorMessage(TempAString, cEMailInspx,32000);
//					ReportErrorMessage(TempAString, cError,0);
				}
			}
			if (SyncFromACPowerMonitorSeen < 100)
				SyncFromACPowerMonitorSeen++;
			//get voltage level from bytes 5, 6, 7 in message in vResponse
			for (i = 5; i < 8; i++)
			{
				if ((vResponse[i] > 57) || (vResponse[i] < 48))
					continue;

				TempVoltageLevel = TempVoltageLevel * 10;
				TempVoltageLevel = TempVoltageLevel + vResponse[i] - 48;
			}

			//set current, minimum, and maximum voltage levels
			int TempOldVoltageReading = vCurrentACInputVoltage;
			vCurrentACInputVoltage = TempVoltageLevel + vLocalConfigurationData->vACPowerMonitorOffset;

			if (vCurrentACInputVoltageMinimum > vCurrentACInputVoltage)
				vCurrentACInputVoltageMinimum = vCurrentACInputVoltage;

			if (vCurrentACInputVoltageMaximum < vCurrentACInputVoltage)
				vCurrentACInputVoltageMaximum = vCurrentACInputVoltage;

			//put voltage level into array of voltage levels
			vACInputVoltageData[vDataIndex] = (BYTE)vCurrentACInputVoltage;

			//increase the index of the circular buffer array
			if (vDataIndex < cNumberOfACVoltageSamples - 1)
				vDataIndex++;
			else
				vDataIndex = 0;

			BYTE TempSAGCount = 0;
			for (int TempIndex = 0; TempIndex < cNumberOfACVoltageSamples; TempIndex++)
			if ((vACInputVoltageData[TempIndex] > 1) && (((vACInputVoltageData[TempIndex] < 106) && (!vLocalConfigurationData->vUse101VACFor10SecondsShutDownLimit)) || (vACInputVoltageData[TempIndex] < 101)))
				TempSAGCount++;
			
			if (TempSAGCount > 19)  //if voltage below 106 for 10 seconds of last 30 samples, shut off X-Rays.  New power monitors sampe 2 times per second
			{
				if ((!vGaveBrownOutError) || ((vLocalSystemData->vXRayOnNotice) && (vLocalConfigurationData->vAutoXRaySourceOffEnabled == 1)))
				{
					vGaveBrownOutError = 1200;
					vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 0x1000;
					vLocalSystemData->vHadABrownOut = true;
					//10/26/2009
					vACErrorCounter[12]++;

					for (int TempIndex = 0; TempIndex < cNumberOfACVoltageSamples; TempIndex++)
						vACInputVoltageData[TempIndex] = 0;

					//if (!(vLocalConfigurationData->vEnableACMessages & 0x1000))
					{
						//vLocalSystemData->vOpenInterlock.LoadString(IDS_BrownOut);
						
						CString TempVoltageString = "106";
						if (vLocalConfigurationData->vUse101VACFor10SecondsShutDownLimit)
							TempVoltageString = "101";

						if ((vLocalConfigurationData->vAutoXRaySourceOffEnabled == 1) && (vLocalSystemData->vXRayOnNotice))
						{
							::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
							ReportErrorMessage("AC LINE VOLTAGE TOO LOW,\nInspection Stopped\nVoltage below " + TempVoltageString + " VAC for 10 Seconds",cCritical,0);
							ReportErrorMessage("Error-AC Input Voltage Brownout, < " + TempVoltageString + " for 10 Sec. Inspection Stopped",cError,0);
							ReportErrorMessage("Error-AC Input Voltage Brownout, < " + TempVoltageString + " for 10 Sec. Inspection Stopped",cEMailInspx,32000);

							IErrorACPowerMoniterLogFile.WriteToLogFile("Event - AC Input Voltage Brownout < " + TempVoltageString + " VAC 10 sec. Inspection Stopped", cErrorPowerOnMessage);
						}
						else
						if (!(vLocalConfigurationData->vEnableACMessages & 0x1000))
						{
							ReportErrorMessage("AC LINE VOLTAGE TOO LOW,\n\nVoltage below " + TempVoltageString + " VAC for 10 Seconds",cCritical,0);
							ReportErrorMessage("AC Input Voltage Brownout, < " + TempVoltageString + " for 10 Sec.",cError,0);
							ReportErrorMessage("AC Input Voltage Brownout, < " + TempVoltageString + " for 10 Sec.",cEMailInspx,32000);

							IErrorACPowerMoniterLogFile.WriteToLogFile("Event - AC Input Voltage Brownout < " + TempVoltageString + " VAC 10 seconds", cErrorPowerOnMessage);
						
							if (vGaveACVoltageWarning == 0)
							{
								vGaveACVoltageWarning = 1;
							}
						}
					}
				}
			}
			else
			if (vGaveBrownOutError)
				vGaveBrownOutError--;

			//if (!vGaveBrownOutError)
			if ((TempOldVoltageReading) && (((TempOldVoltageReading < 106) && (!vLocalConfigurationData->vUse101VACFor10SecondsShutDownLimit)) || (TempOldVoltageReading < 101)) &&  //if previous reading below 108
				(vCurrentACInputVoltage) && (((vCurrentACInputVoltage < 106) && (!vLocalConfigurationData->vUse101VACFor10SecondsShutDownLimit)) || (vCurrentACInputVoltage < 101)))  //and current reading below 108, put error message in log
			{
				if (!vGaveBelow108VoltsError)
				if (IErrorACPowerMoniterLogFile.vNumOfErrors < 50)
				{
					if (vLocalConfigurationData->vUse101VACFor10SecondsShutDownLimit)
						IErrorACPowerMoniterLogFile.WriteToLogFile("Event - AC Input Voltage below 101 V", cErrorPowerOnMessage);
					else
						IErrorACPowerMoniterLogFile.WriteToLogFile("Event - AC Input Voltage below 106 V", cErrorPowerOnMessage);

					vGaveBelow108VoltsError = 1200;  //will give error every 20 minutes if still occuring
				}
			}
			else
			if (vGaveBelow108VoltsError)
				vGaveBelow108VoltsError--;


			if ((vCurrentACInputVoltage > -1) && (vCurrentACInputVoltage < 550))
			{
				CString TempSpaceString = " ";
				if (vCurrentACInputVoltage < 100)
					TempSpaceString = "  ";
				else 
				if (vCurrentACInputVoltage < 10)
					TempSpaceString = "   ";
				IACPowerMoniterLogFile.WriteToLogFile(TempSpaceString + dtoa((double)vCurrentACInputVoltage,0), cInputMessage);
			}
			#ifdef _DEBUG
			//afxDump << "Configuration file could not be opened " << TempFileException->m_cause << "\n";
//				afxDump << vCurrentACInputVoltage << ". ";
			#endif

		}
		else
		if (vResponse[1] == 'E') //event occured
		{
			if (!vErrorsOccuredWait) //10/26/2009
				vErrorsOccuredWait = 30;
	//		TempResponseString = TempResponseString + "Caught Event Occured "; 
			#ifdef _DEBUG
			//afxDump << "Configuration file could not be opened " << TempFileException->m_cause << "10";
	//			afxDump << "Event Occured ";
			#endif

			CString TempString1 = "",TempString2 = "";
			bool TempErrorMessageIsNew = false;

			if (vResponse[9] == '>')	//message is an impulse event old style power moniter
			{
				//10/26/2009
					vLocalSystemData->vOldStylePowerMoniter = true;

				if (vLastError.GetLength() == 0)
				{
					vLastError = " " + TempResponseString;
					IErrorACPowerMoniterLogFile.vNumOfErrors = 1;
				}

				if (vResponse[11] == '1') //'Event - > 100V Impulse on [Hot/Neut.]'
				{
					if (vResponse[27] == 'H')//100V Hot
					{
						vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 0x40;
						//10/26/2009
						vACErrorCounter[6]++;

						//if (vGaveACVoltageWarning == 0)
						//if (!(vLocalConfigurationData->vEnableACMessages & 0x40))
						//	vGaveACVoltageWarning = 1;
					}
					else
					{//100 v Neutral
						vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 0x400;
						//10/26/2009
						vACErrorCounter[10]++;

						//if (vGaveACVoltageWarning == 0)
						//if (!(vLocalConfigurationData->vEnableACMessages & 0x400))
						//	vGaveACVoltageWarning = 1;
					}
				}
				else 
				if (vResponse[13] == '0') //'Event - > 500V Impulse on [Hot/Neut.]'
				{
					if (vResponse[27] == 'H')//500V Hot
					{
						vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 0x80;
						//10/26/2009
						vACErrorCounter[7]++;

						//if (vGaveACVoltageWarning == 0)
						//if (!(vLocalConfigurationData->vEnableACMessages & 0x80))
						//	vGaveACVoltageWarning = 1;
					}
					else
					{//500V Neutral
						vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 0x800;
						//10/26/2009
						vACErrorCounter[11]++;

						//if (vGaveACVoltageWarning == 0)
						//if (!(vLocalConfigurationData->vEnableACMessages & 0x800))
						//	vGaveACVoltageWarning = 1;
					}
				}
				else 
				if (vResponse[12] == '0') //'Event - > 50V Impulse on [Hot/Neut.]'
				{
					if (vResponse[26] == 'H') //50V Hot
					{
						vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 0x20;
						//10/26/2009
						vACErrorCounter[5]++;

						//if (vGaveACVoltageWarning == 0)
						//if (!(vLocalConfigurationData->vEnableACMessages & 0x20)) //0x20
						//	vGaveACVoltageWarning = 1;
					}
					else
					{//50V Neutral
						vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 0x200;
						//10/26/2009
						vACErrorCounter[9]++;

						//if (vGaveACVoltageWarning == 0)
						//if (!(vLocalConfigurationData->vEnableACMessages & 0x200))
						//	vGaveACVoltageWarning = 1;
					}
				}
				else	//'Event - > 5V Impulse on [Hot/Neut.]'
				if (vResponse[25] == 'H') //5V Hot
				{
					vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 0x10;
					//10/26/2009
					vACErrorCounter[4]++;

					//if (vGaveACVoltageWarning == 0)
					//if (!(vLocalConfigurationData->vEnableACMessages & 0x10))
					//	vGaveACVoltageWarning = 1;
				}
				else
				{//5V Neutral
					vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 0x100;
					//10/26/2009
					vACErrorCounter[8]++;

					//if (vGaveACVoltageWarning == 0)
					//if (!(vLocalConfigurationData->vEnableACMessages & 0x100))
					//	vGaveACVoltageWarning = 1;
				}
			}
			else
			if (vResponse[8] == '>')	//message is an impulse event new style power moniter
			{
				if (vLocalSystemData->vOldStylePowerMoniter)
					vLocalSystemData->vOldStylePowerMoniter = false;
				//10/26/2009
				if (vLastError.GetLength() == 0)
				{
					vLastError = " " + TempResponseString;
					IErrorACPowerMoniterLogFile.vNumOfErrors = 1;
				}

				if (vResponse[10] == '1') 
				{
					if (vResponse[26] == 'H')		//'Event - > 100V Impulse on Hot'
					{
						//10/26/2009
						vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 0x40;
						vACErrorCounter[6]++;
						
						//if (vGaveACVoltageWarning == 0)
						//if (!(vLocalConfigurationData->vEnableACMessages & 0x40))
						//{
						//	vGaveACVoltageWarning = 1;
						//}
					}
					else		//'Event - > 10V Impulse on Neut.'
					if (vResponse[11] == '0')
					{
						//10/26/2009
						vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 0x400;
						vACErrorCounter[10]++;

						//if (vGaveACVoltageWarning == 0)
						//if (!(vLocalConfigurationData->vEnableACMessages & 0x400))
						//{
						//	vGaveACVoltageWarning = 1;
						//}
					}
					else	//'Event - > 1V Impulse on Neut.'
					{
						//10/26/2009
						vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 0x100;
						vACErrorCounter[8]++;

						//if (vGaveACVoltageWarning == 0)
						//if (!(vLocalConfigurationData->vEnableACMessages & 0x100))
						//{
						//	vGaveACVoltageWarning = 1;
						//}
					}
				}
				else 
				if (vResponse[12] == '0') //'Event - > 500V Impulse on Hot'
				{
					if (vResponse[25] == 'H')//500V Hot
					{
						//10/26/2009
						vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 0x80;
						vACErrorCounter[7]++;

						//if (vGaveACVoltageWarning == 0)
						//if (!(vLocalConfigurationData->vEnableACMessages & 0x80))
						//{
						//	vGaveACVoltageWarning = 1;
						//}
					}
				}
				else 
				if (vResponse[11] == '0') 
				{
					if (vResponse[10] == '5')	//'Event - > 50V Impulse on [Hot/Neut.]'
					{
						if (vResponse[25] == 'H') //50V Hot
						{
							//10/26/2009
							vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 0x20;
							vACErrorCounter[5]++;

							//if (vGaveACVoltageWarning == 0)
							//if (!(vLocalConfigurationData->vEnableACMessages & 0x20)) //0x20
							//{
							//	vGaveACVoltageWarning = 1;
							//}
						}
						else		//50V Neutral
						{	
							//10/26/2009
							vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 0x800;
							vACErrorCounter[11]++;

							//if (vGaveACVoltageWarning == 0)
							//if (!(vLocalConfigurationData->vEnableACMessages & 0x800))
							//{
							//	vGaveACVoltageWarning = 1;
							//}
						}
					}
					else	////'Event - > 20V Impulse on Hot'
					{
						//10/26/2009
						vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 0x10;
						vACErrorCounter[4]++;

						//if (vGaveACVoltageWarning == 0)
						//if (!(vLocalConfigurationData->vEnableACMessages & 0x10)) //0x10
						//{
						//	vGaveACVoltageWarning = 1;
						//}
					}
				}
				else	
				{
					if (vResponse[10] == '5')	//'Event - > 5V Impulse on Neut.'
					{
						//10/26/2009
						vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 0x200;
						vACErrorCounter[9]++;

						//if (vGaveACVoltageWarning == 0)
						//if (!(vLocalConfigurationData->vEnableACMessages & 0x200))
						//{
						//	vGaveACVoltageWarning = 1;
						//}
					}
				}
			}
			else
			if ((vResponse[24] == '<') && (vResponse[26] == '9'))//message is Event - AC Voltage Sag < 95V
				//((vResponse[9] == 'A') && (vResponse[12] == 'P')))//message is Event - AC Power Dropout
			{
				vACErrorCounter[1]++;
				vLastError = " " + TempResponseString;
				IErrorACPowerMoniterLogFile.vNumOfErrors = 1;
				if (!vErrorsOccuredWait) //10/26/2009
					vErrorsOccuredWait = 30;
				if ((!(vLocalSystemData->vHadAPowerMoniterMessageError & 2)) ||//if flag is not already set
					((vLocalConfigurationData->vAutoXRaySourceOffEnabled == 1) && (vLocalSystemData->vXRayOnNotice)) ||
					(!vGaveACVoltageWarning))  //or have not given any messages this run
				{
					vGaveBrownOutError = 1200;
					vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 2;

					vLocalSystemData->vHadABrownOut = true;
					if ((!(vLocalConfigurationData->vEnableACMessages & 2)) || //enable bit is active low
						((vLocalConfigurationData->vAutoXRaySourceOffEnabled == 1) && (vLocalSystemData->vXRayOnNotice)))
					{
						//TempErrorMessageIsNew = true;
						//vLocalSystemData->vOpenInterlock.LoadString(IDS_BrownOut);

						if ((vLocalConfigurationData->vAutoXRaySourceOffEnabled == 1) && (vLocalSystemData->vXRayOnNotice))
						{
							//TempString1 = "AC LINE VOLTAGE TOO LOW,\nInspection Stopped\nVoltage Below 95VAC";
							//TempString2 = "AC Input Voltage Brownout < 95 VAC - Inspection Stopped";
							TempString1 = "AC LINE VOLTAGE TOO LOW,\n\nVoltage Below 95VAC";
							TempString2 = "AC Input Voltage Brownout < 95 VAC";
						}
						else
						{
							TempString1 = "AC LINE VOLTAGE TOO LOW,\nVoltage Below 95VAC";
							TempString2 = "AC Input Voltage Brownout,  < 95 VAC";
						}
						if (vGaveACVoltageWarning == 0)
							vGaveACVoltageWarning = 1;

						ReportErrorMessage(TempString2,cError,32000);
						ReportErrorMessage(TempString2,cEMailInspx,32000);
						ReportErrorMessage(TempString1,cCritical,0);

						IErrorACPowerMoniterLogFile.WriteToLogFile(TempString2, cErrorPowerOnMessage);
					}
				}
			}
			else
			if (((vResponse[26] == '>') && (vResponse[28] == '1')))	//16:11:18 Event - AC Voltage Surge > 135V
			{
				vACErrorCounter[2]++;
				vLastError = " " + TempResponseString;
				IErrorACPowerMoniterLogFile.vNumOfErrors = 1;
				if (!vErrorsOccuredWait) //10/26/2009
					vErrorsOccuredWait = 30;
				if ((!(vLocalSystemData->vHadAPowerMoniterMessageError & 4)) || //if have not given this message yet
					((vLocalConfigurationData->vAutoXRaySourceOffEnabled == 1) && (vLocalSystemData->vXRayOnNotice)) ||
					(!vGaveACVoltageWarning))  //or have not given any messages this run
				{
					vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 4;

					if ((!(vLocalConfigurationData->vEnableACMessages & 4)) ||	//if enabled
						((vLocalConfigurationData->vAutoXRaySourceOffEnabled == 1) && (vLocalSystemData->vXRayOnNotice)))
					{
						TempErrorMessageIsNew = true;
						if ((vLocalConfigurationData->vAutoXRaySourceOffEnabled == 1) && (vLocalSystemData->vXRayOnNotice))
						{
							TempString1 = "AC LINE VOLTAGE TOO HIGH,\nInspection Stopped\nVoltage Above 135VAC";
							TempString2 = "AC Input Voltage Surge > 135 VAC - Inspection Stopped";
						}
						else
						{
							TempString1 = "AC LINE VOLTAGE TOO HIGH,\nVoltage Above 135VAC";
							TempString2 = "AC Input Voltage Surge, > 135 VAC";
						}
						if (vGaveACVoltageWarning == 0)
							vGaveACVoltageWarning = 1;
					}
				}
			}
			else
			if (((vResponse[9] == 'A') && (vResponse[12] == 'P')))		//16:24:12 Event - AC Power Dropout
			{
				vACErrorCounter[0]++;
				vLastError = " " + TempResponseString;
				IErrorACPowerMoniterLogFile.vNumOfErrors = 1;
				if (!vErrorsOccuredWait) //10/26/2009
					vErrorsOccuredWait = 30;
				if ((!(vLocalSystemData->vHadAPowerMoniterMessageError & 1)) ||	//if flag is not already set
					((vLocalConfigurationData->vAutoXRaySourceOffEnabled == 1) && (vLocalSystemData->vXRayOnNotice)) ||
					(!vGaveACVoltageWarning))  //or have not given any messages this run
				{
					vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 1;

					if ((!(vLocalConfigurationData->vEnableACMessages & 1)) ||	//if enabled
						((vLocalConfigurationData->vAutoXRaySourceOffEnabled == 1) && (vLocalSystemData->vXRayOnNotice)))
					{
						TempErrorMessageIsNew = true;
						if ((vLocalConfigurationData->vAutoXRaySourceOffEnabled == 1) && (vLocalSystemData->vXRayOnNotice))
						{
							TempString1 = "AC LINE VOLTAGE TOO LOW,\nInspection Stopped\nVoltage Power Dropout";
							TempString2 = "AC Input Voltage Dropout - Inspection Stopped";
						}
						else
						{
							TempString1 = "AC LINE VOLTAGE TOO LOW,\nVoltage Power Dropout.";
							TempString2 = "AC Input Voltage Dropout.";
						}
						if (vGaveACVoltageWarning == 0)
							vGaveACVoltageWarning = 1;
					}
				}
			}
			else
			if (((vResponse[9] == 'H') && (vResponse[13] == 'N')))	//		Event - Hot/Neutral Reversed
			{
				vACErrorCounter[3]++;
				vLastError = " " + TempResponseString;
				IErrorACPowerMoniterLogFile.vNumOfErrors = 1;
				if (!vErrorsOccuredWait) //10/26/2009
					vErrorsOccuredWait = 30;
				if ((!(vLocalSystemData->vHadAPowerMoniterMessageError & 8))	|| //if flag is not already set
					((vLocalConfigurationData->vAutoXRaySourceOffEnabled == 1) && (vLocalSystemData->vXRayOnNotice)) ||
					(!vGaveACVoltageWarning))  //or have not given any messages this run
				{
					vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 8;

					if ((!(vLocalConfigurationData->vEnableACMessages & 8)) ||	//if enabled
						((vLocalConfigurationData->vAutoXRaySourceOffEnabled == 1) && (vLocalSystemData->vXRayOnNotice)))
					{
						TempErrorMessageIsNew = true;
						if ((vLocalConfigurationData->vAutoXRaySourceOffEnabled == 1) && (vLocalSystemData->vXRayOnNotice))
						{
							TempString1 = "AC LINE VOLTAGE REVERSAL,\nInspection Stopped\nVoltage Hot and Neutral Reversed";
							TempString2 = "AC Input Voltage Hot and Neutral Reversed - Inspection Stopped";
						}
						else
						{
							TempString1 = "AC LINE VOLTAGE REVERSAL,\nVoltage Hot and Neutral Reversed";
							TempString2 = "AC Input Voltage Hot and Neutral Reversed.";
						}
						if (vGaveACVoltageWarning == 0)
							vGaveACVoltageWarning = 1;
					}
				}				
			}
			else
			if (((vResponse[9] == 'H') && (vResponse[14] == 'F')))	//		Event - High Frequency Noise
			{
				vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 0x4000;
				vACErrorCounter[14]++;
				//just record event happened
//				vLastError = " " + TempResponseString;
//				IErrorACPowerMoniterLogFile.vNumOfErrors = 1;

//				if (!vErrorsOccuredWait) //10/26/2009
//					vErrorsOccuredWait = 30;

//				if (vGaveACVoltageWarning == 0)
//				if (!(vLocalConfigurationData->vEnableACMessages & 0x4000))
//					vGaveACVoltageWarning = 1;
			}
			else
			if (((vResponse[9] == 'O') && (vResponse[14] == 'G')))	//		Event - Open Ground Detected
			{
				//10/26/2009
				vACErrorCounter[13]++;
				vLastError = " " + TempResponseString;
				IErrorACPowerMoniterLogFile.vNumOfErrors = 1;

				if (!vErrorsOccuredWait) //10/26/2009
					vErrorsOccuredWait = 30;
				if ((!(vLocalSystemData->vHadAPowerMoniterMessageError & 0x2000)) ||	//if flag is not already set
					((vLocalConfigurationData->vAutoXRaySourceOffEnabled == 1) && (vLocalSystemData->vXRayOnNotice)) ||
					(!vGaveACVoltageWarning))  //or have not given any messages this run
				{
					vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError | 0x2000;

					TempErrorMessageIsNew = true;
					if ((!(vLocalConfigurationData->vEnableACMessages & 0x2000)) ||	//if enabled
						((vLocalConfigurationData->vAutoXRaySourceOffEnabled == 1) && (vLocalSystemData->vXRayOnNotice)))
					{
						TempString1 = "Open Ground\nInspection Stopped";
						TempString2 = "Open Ground - Inspection Stopped";
					}
					else
					{
						TempString1 = "Open Ground.";
						TempString2 = "Open Ground.";
					}
					if (vGaveACVoltageWarning == 0)
					if (!(vLocalConfigurationData->vEnableACMessages & 0x2000))
						vGaveACVoltageWarning = 1;
				}
			}
			else
			{
				bool TempInvalidData = false;
				for (WORD TempLoop = 0; TempLoop < vLength; TempLoop++)
				if (vResponse[TempLoop] > 0x7E)
					TempInvalidData = true;

				if (TempInvalidData)
					ReportErrorMessage("AC Power Monitor Error-Unknown Reply with Invalid data: " + vLatestResponse, cWriteToLog, 0);
				else
					ReportErrorMessage("AC Power Monitor Error-Unknown Reply: " + vLatestResponse, cEMailInspx, 32000);
			}

			if (TempErrorMessageIsNew)
			{
				ReportErrorMessage(TempString2,cError,32000);
				ReportErrorMessage(TempString2,cEMailInspx,32000);
				ReportErrorMessage(TempString1,cCritical,0);

				IErrorACPowerMoniterLogFile.WriteToLogFile(TempString2, cErrorPowerOnMessage);
			
				if ((vLocalConfigurationData->vAutoXRaySourceOffEnabled == 1) && (vLocalSystemData->vXRayOnNotice))
					::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
			}

				/*
			if (vResponse[9] == 'A')	//16:24:12 Event - AC Power Dropout <-example message, where A is the 9th character
			{
	//			TempResponseString = TempResponseString + "Caught Error - AC Power Dropout\n\r"+ (char)10;//line feed
//				ReportErrorMessage("Error - AC Power Dropout", cEMailInspx, 32000);
				if (vPowerFailure == -1)
				{
					if (vResponse[12] == 'P')	//16:24:12 Event - AC Power Dropout <-example message, where P is the 12th character
						vPowerFailure = IErrorACPowerMoniterLogFile.vNumOfErrors;

					else
					if (vResponse[12] == 'V')	//16:11:18 Event - AC Voltage Surge > 135V
					if (vPowerError == -1)
						vPowerError = IErrorACPowerMoniterLogFile.vNumOfErrors;
				}
			}
			else 
			{
				for (i = 11; i < 15; i++)
				{
					TempVoltageLevel = TempVoltageLevel + vResponse[i] - 48;
					if (vResponse[i+1] == 'V')
						break;
					else
						TempVoltageLevel = TempVoltageLevel * 10;
				}
				if ((TempVoltageLevel > -1) && (TempVoltageLevel < 550))
					TempVoltageString = dtoa((double)TempVoltageLevel,0);

				if (TempVoltageLevel < 100)
				{
					TempVoltageString = " " + TempVoltageString;
					if (TempVoltageLevel < 10)
						TempVoltageString = " " + TempVoltageString;
				}
				//	IACPowerMoniterLogFile.WriteToLogFile( " " + TempVoltageString, cInputMessage);
				//TempResponseString = TempResponseString + " caught low power = " + dtoa((double)TempVoltageLevel) + " " + (char)10 + (char)13;

//				#ifdef _DEBUG
					//afxDump << "Configuration file could not be opened " << TempFileException->m_cause << "10";
//				afxDump << "\n\r" << TempVoltageLevel << ". ";
//				#endif
			
			}
				*/
			//IErrorACPowerMoniterLogFile.WriteToLogFile(" " + TempResponseString, cError);
			//IErrorACPowerMoniterLogFile.WriteToLogFile( TempResponseString + " Latest Response: "+ vLatestResponse, cInputMessage);
		}
		else 
		{
			ReportErrorMessage("AC Power Monitor Error-Unknown Reply: " + vLatestResponse, cEMailInspx, 32000);
		}
	}
	vLatestResponse = "";

}

void CACPowerMoniter::DoBackGroundWork()  //called about 10 hertz
{
	if (vErrorsOccuredWait)
	{
		vErrorsOccuredWait--;
		if (vErrorsOccuredWait == 0)
			IErrorACPowerMoniterLogFile.vDoResolveErrors = true;
	}

	if (IErrorACPowerMoniterLogFile.vDoResolveErrors == true)
	{
		ResolveErrors();
		IErrorACPowerMoniterLogFile.vDoResolveErrors = false;
	}
	//10/26/2009
	CTime TempTime = CTime::GetCurrentTime();
	CString TempHour = TempTime.Format("%H");

	if (vOldHour != TempHour)
	{
		vOldHour = TempHour;
		for (BYTE TempLoop = 0; TempLoop < 16; TempLoop ++)
		{
			if (vACErrorCounter[TempLoop] > 1)
			{
				CString TempErrorString = "Unknown";
				switch (TempLoop)
				{
					case 0: TempErrorString = "AC Power Dropout"; break;
					case 1: TempErrorString = "AC Voltage Sag < 95V"; break;
					case 2: TempErrorString = "AC Voltage Surge > 135V"; break;
					case 3: TempErrorString = "Hot-Neutral Reversed"; break;
					case 4: TempErrorString = "5V Impulse on Hot"; break;
					case 5: TempErrorString = "50V Impulse on Hot"; break;
					case 6: TempErrorString = "100V Impulse on Hot"; break;
					case 7: TempErrorString = "500V Impulse on Hot"; break;
					case 8: TempErrorString = "5V Impulse on Neutral"; break;
					case 9: TempErrorString = "50V Impulse on Neutral"; break;
					case 10: TempErrorString = "100V Impulse on Neutral"; break;
					case 11: TempErrorString = "500V Impulse on Neutral"; break;
					case 12: 
							if (vLocalConfigurationData->vUse101VACFor10SecondsShutDownLimit)
								TempErrorString = "Voltage below 101 VAC"; 
							else
								TempErrorString = "Voltage below 106 VAC"; 
					break;
					case 13: TempErrorString = "Open Ground Detected"; break;
					//case 14: TempErrorString = ""; break;
					//case 15: TempErrorString = ""; break;
				}
				IErrorACPowerMoniterLogFile.WriteToLogFile(" In last hour had " + dtoa(vACErrorCounter[TempLoop],0) + 
					" occurences of " + TempErrorString, cErrorPowerOnMessage);
			}
			vACErrorCounter[TempLoop] = 0;
		}
	}

	IACPowerMoniterLogFile.LogFileObjectBackground();
	IErrorACPowerMoniterLogFile.LogFileObjectBackground();
}

