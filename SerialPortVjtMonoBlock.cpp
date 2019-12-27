#include "stdafx.h"
#include "ScanTrac.h"
#include "SerialPortReview.h"
#include "SerialPortModBus.h"
#include "SerialPortGuardMaster.h"
#include "SerialPortVjtMonoBlock.h"
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CSerialPortVjtMonoBlock, CSpellmanDXMXRaySourcePort)

//const BYTE cSendCommandSleepTime = 5;

#ifdef CompileInHTTPClientOperationsGuardian
extern CString vGlobalOGLocationString;
extern CString vGlobalOGLimitString;
extern WORD vGlobalOGErrorCode;
#endif
//extern BYTE vGlobalXRaySourceInterlockInputOpen;
extern BYTE vGlobalScanTracType;
extern CSerialPortGuardMaster *vGlobalGuardMaster;
extern int vGlobalXrayNoCommMessageRcvdEver;
extern int vGlobalSerialPortNumberXRaySource;

CSerialPortVjtMonoBlock* CSerialPortVjtMonoBlock::vMyPointer;
CSemaphore CSerialPortVjtMonoBlock::vSyncSect;
extern CScanTracDlg* vGlobalMainWindowPointer;

////////////////////////////////////////////////
//
// VJT 320w Monoblock hard limits:
//   Tube Current : 0.2mA to 8mA
//   Tube High Voltage : 30kV to 80kV
//
////////////////////////////////////////////////

CSerialPortVjtMonoBlock::CSerialPortVjtMonoBlock(void)
{
	vNextWatchDogEnable = 0;

	vMinimumCurrent = 0.205;
	vLocalSystemData = NULL;

	vEveryOther = false;
	vHVPSType = "VJT";
	vDeviceName = "VJT Monoblock";
	v15VoltMonitor = 15;
	vRemoteMode = true;
	vSetupDataSent = false;
	vDetectedValidBannerDevice = false;
	vLastMessageSent = 0;
	vSentDeviceSetup = false;
	vSerialPortParity = cNOPARITY;
	vParity = cNOPARITY;
	//vConnectedDeviceVerified = false;
	vSentCommLossMessage = false;
	vUse9600Baud = TRUE;
	vBaudRate = CBR_9600;

	// Data initialization:
	vDetectedValidVjtMonoBlockDevice = true;
	vRecoveredFromComLoss = false;
	for (BYTE TempLoop = 0; TempLoop < 10; TempLoop++)
		vPendingCommand[TempLoop] = NO_COMMAND;
	vVoltage = vCurrent = vTemperature = vFilament = 0.0;
	vRegulationFault = vInterlockOpenFault = vCathodeOverVoltageFault = false;
	vAnodeOverVoltageFault = vOverTemperatureFault = vArcFault = false;
	vOverCurrentFault = vPowerLimitFault = vOverVoltageFault = false;

	vSendWatchdogHeartBeat = false;
	vWatchdogStatusOn = true;
	vWatchdogEnabledActive = true;
	vHvStatusOn = false;
	vXraysPoweredOnStatus = false;

	vInterlockFaultReported = false;
	vRegulationFaultReported = false;
	vPowerLimitFaultReported = false;
	vCathodeOverVoltageFaultReported = false;
	vAnodeOverVoltageFaultReported = false;
	vOverTemperatureFaultReported = false;
	vArcFaultReported = false;
	vOverCurrentFaultReported = false;
	vOverVoltageFaultReported = false;

	vXraysOnSent = false;
	vProgramVoltageSent = false;
	vProgramCurrentSent = false;
	vClearFaultsSent = false;
	vXraysOnAcknowledged = false;
	vProgramVoltageAcknowledged = false;
	vProgramCurrentAcknowledged = false;
	vClearFaultsAcknowledged = false;
	vXraysOnAcknowledgedWaitCount = 0;
	vProgramVoltageAcknowledgedWaitCount = 0;
	vProgramCurrentAcknowledgedWaitCount = 0;
	vClearFaultsAcknowledgedWaitCount = 0;
	vFaultPresentCount = 0;
	vInterlockFaultDelayCounter = 0;
	vTemporarilyTurnedOffHVForLowCurrent = false;

	vVoltageToProgram = 0;
	vCurrentToProgram = 0;

	// Thread data initialization:
	vMyPointer = this;
	vThreadStarted = false;
	vWorkerThread = AfxBeginThread(&CSerialPortVjtMonoBlock::ThreadProc, 0, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, (LPSECURITY_ATTRIBUTES)0);
	vWorkerThread->m_bAutoDelete = true;
}

CSerialPortVjtMonoBlock::~CSerialPortVjtMonoBlock(void)
{
	//vEndThisThread = true;
	Sleep(100);
	ClosePort();
}

void CSerialPortVjtMonoBlock::ResetStoredComPortId()
{
	//vGlobalMainWindowPointer->vConfigurationData->vVjtMonoBlockComPort = 0;
}

BEGIN_MESSAGE_MAP(CSerialPortVjtMonoBlock, CWinThread)
	//{{AFX_MSG_MAP(CSerialPortVjtMonoBlock)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSerialPortVjtMonoBlock::DemoModeMockUp()
{
	vPortOpen = true;
	vComPortNumber = 3;
	vConnectedDeviceVerified = true;
	vHasReceivedCommunication = vHasTransmittedCommunication = true;
}

#define MESSAGE_BUFFER_SIZE 160
void CSerialPortVjtMonoBlock::DecodeMessage()
{
	BYTE TempBuffer[MESSAGE_BUFFER_SIZE];
	WORD TempIndex = vReceiveDataTail;

	memset(TempBuffer, 0x0, MESSAGE_BUFFER_SIZE);
	vNoCommunications = false;
	//vLocalSystemData->vHVPSNoComm = false;
	vHasReceivedCommunication = true;
	WORD TempMessageByteCount = 0;
	WORD TempQueueCount = ReceiveDataCount();
	for (WORD TempLoop = 1; TempLoop < TempQueueCount; TempLoop++)
	{
		WORD TempIndex = (vReceiveDataTail + TempLoop) % cReceiveBufferSize; 
	// Filter out the framing characters.
		if (vReceiveDataBuffer[TempIndex] == STX) 
		{ 
			continue; 
		}
		if (vReceiveDataBuffer[TempIndex] == CR) 
			break;

		TempBuffer[TempMessageByteCount] = vReceiveDataBuffer[TempIndex];
		TempMessageByteCount++;
		TempIndex++;
		if (TempIndex >= cReceiveBufferSize)
			TempIndex = 0;

		//if (vReceiveDataTail < cReceiveBufferSize - 1)
		//	vReceiveDataTail++;
		//else
		//	vReceiveDataTail = 0;
	}

	if (TempMessageByteCount > 0)
	{
		ProcessMessageContents(TempBuffer, TempMessageByteCount);
	}

	if (vWaitingForReply)
	{
		vWaitingForReply = 0;
		if (vPendingCommand[0] != NO_COMMAND)
		{
			Sleep(55);
			SendVjtCommand();
		}
	}

	//vLatestResponse = "";
	//if ((vFoundXrayPort == true) && (vGlobalXrayNoCommMessageRcvdEver != cYesCommSeen))
	//{
	//	vGlobalXrayNoCommMessageRcvdEver = cYesCommSeen;
	//	if (vGlobalPortHereOK[vGlobalSerialPortNumberXRaySource] != cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER)	
	//	{
	//		vGlobalPortHereOK[vGlobalSerialPortNumberXRaySource] = cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER;
	//		if (vComPortNumber != cDEFAULT_XRAY_SERIAL_PORT_NUMBER)
	//		{
	//			CString TempAString;
	//			TempAString.Format(_T("Error-XRay Com Port %d, Default Com 3"),vGlobalSerialPortNumberXRaySource);
	//			ReportErrorMessage(TempAString, cEMailInspx,32000);
	//		}
	//	}
	//}
}

void CSerialPortVjtMonoBlock::ProcessMessageContents(BYTE *MessageBuffer, int MessageByteCount)
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		if (vDetectedValidVjtMonoBlockDevice)
			vLocalSystemData->vLogFile.WriteToLogFile("Processing VjtMonoBlock message.", cDebugMessage);

	vCommunicationsTimeOutCounter = 0;
	vGlobalXrayNoCommMessageRcvdEver = cYesCommSeen;

	if (vSentCommLossMessage)
	{
		vSentCommLossMessage = false;
		CString msg = "Restored Serial Comm with: " + vDeviceName + ", " + vComPortString;
		ReportErrorMessage(msg, cError, 0);
		//vLocalSystemData->vHVPSNoComm = false;
		ReportErrorMessage("Restored Communication with " + vDeviceName + " on " + vComPortString, cEMailInspx, 32000);
		vRecoveredFromComLoss = true;
	}

	// For debugging the message contents only.
	//BYTE buffer[MESSAGE_BUFFER_SIZE];
	//memset(buffer, 0x0, MESSAGE_BUFFER_SIZE);
	//for (int indx = 0; indx < MessageByteCount; indx++)
	//	buffer[indx] = MessageBuffer[indx];

	///////////////////////////////////////////////////////////////////////////////
	//  VJT Command Summary:
	//
	//  Voltage Program "VP"
	//	Current Program "CP"
	//	Voltage / Current / Temperature / Filament Monitor "MON"
	//	Fault Clear "CLR"
	//	Report Fault "FLT"
	//	HV Status "STAT"
	//  X-Ray Enable "ENBL"
	//	Watch Dog Timer "WDTE"
	//	Comm Port Echo "FREV"
	//	Watch Dog Enable / Disable "WDOGX"
	//	Watch Dog Status "WSTAT"
	///////////////////////////////////////////////////////////////////////////////

	CString MessageContents(MessageBuffer);

	if ((MessageBuffer != NULL) && (MessageByteCount > 0))
	{
		switch (vLastMessageSent)
		{
		case VOLTAGE_PROGRAM:
			if (MessageContents.Find(_T("VP")) != -1)
			{
				vProgramVoltageSent = false;
				vProgramVoltageAcknowledged = true;
				vFoundXrayPort = vConnectedDeviceVerified = true;
			}

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("VJT Reply from set voltage.", cDebugMessage);

			break;
		case CURRENT_PROGRAM:
			if (MessageContents.Find(_T("CP")) != -1)
			{
				vProgramCurrentSent = false;
				vProgramCurrentAcknowledged = true;
				vFoundXrayPort = vConnectedDeviceVerified = true;
			}

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("VJT Reply from set current.", cDebugMessage);

			break;
		case MONITOR:
			vSyncSect.Lock();
			MessageToArray(MessageContents, vMonitorMessageData);
			vSyncSect.Unlock();

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("VJT got filament monitor.", cDebugMessage);

			break;
		case CLEAR_FAULT:
			if (MessageContents.Find(_T("CLR")) != -1)
			{
				vClearFaultsSent = false;
				vClearFaultsAcknowledged = true;
				vFoundXrayPort = vConnectedDeviceVerified = true;
			}

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("VJT Reply from clear faults.", cDebugMessage);

			break;
		case REPORT_FAULT:
			vSyncSect.Lock();
			MessageToArray(MessageContents, vFaultMessageData);
			vSyncSect.Unlock();

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("VJT Reply from report fault.", cDebugMessage);

			break;
		case HV_STATUS:
			vFoundXrayPort = vConnectedDeviceVerified = true;
			vGlobalXrayNoCommMessageRcvdEver = cYesCommSeen;
			vHvStatusOn = false;
			vXraysOnSent = false;
			vXraysOnAcknowledged = true;
			if (MessageByteCount == 1)
				if (MessageContents.CompareNoCase(_T("1")) == 0)
					vHvStatusOn = true;
				//else
				//{
				//	if (vRecoveredFromComLoss)
				//	{// Need to sunc the system with the actual hardware On Off state.
				//		vRecoveredFromComLoss = false;
				//		if (vLocalSystemData->vXRayOnNotice && !vHvStatusOn)
				//		{// System thinks x-rays are on but they are not.
				//			CString msg("-");
				//			msg = "Lost Communication with";
				//			msg = vComPortString + msg;
				//			vHvStatusOn = true; // Need to unblock the emergency off check.
				//			PostErrorMessage(msg, true);
				//			vHvStatusOn = false;
				//		}
				//	}
				//}

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("VJT Reply from on status.", cDebugMessage);

			break;
		case XRAY_ENABLE:
			if (MessageByteCount == 5)
			{
				vXraysOnAcknowledged = false;
				if (MessageBuffer[4] == 0x31)
				{
					vXraysOnSent = false;
					vXraysOnAcknowledged = true;
				}
			}

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("VJT Reply from turn on/off.", cDebugMessage);

			break;
		case COMM_PORT_ECHO:
			if (MessageByteCount > 2)
				vFoundXrayPort = vConnectedDeviceVerified = true;

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("VJT Reply from echo.", cDebugMessage);

			break;
		case WATCH_DOG_HEARTBEAT:

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("VJT Reply from heartbeat.", cDebugMessage);

			break;
		case WATCH_DOG_ENABLE:
		{
			CString status(MessageContents);

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("VJT Reply from watchdog enable.", cDebugMessage);

		}
		break;
		case WATCH_DOG_STATUS:
			if (MessageContents == "1")
				vWatchdogStatusOn = true;
			else
				vWatchdogStatusOn = false;
			break;

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("VJT Reply from watch dog status.", cDebugMessage);

		default:	
		

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("VJT Reply not processed.", cDebugMessage);

		break;
		}
	}
	vLastMessageSent = 0;
}

void CSerialPortVjtMonoBlock::MessageToArray(CString message, CStringArray& array)
{
	int spaceIndex = message.Find(SP);
	for (; spaceIndex > 0;)
	{
		CString data(message, spaceIndex);
		array.Add(data);
		message = message.Right(message.GetLength() - spaceIndex - 1);
		spaceIndex = message.Find(SP);
		if (spaceIndex < 0 && message.GetLength() > 0)
			array.Add(message);
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Begin SerialPortVjtMonoBlock command implementation.
//
//////////////////////////////////////////////////////////////////////////////////////
int CSerialPortVjtMonoBlock::ComposeVoltageProgramCommand(BYTE data[])
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Send VjtMonoBlock program x-ray voltage.", cDebugMessage);

	int commandIndex = 1;
	data[commandIndex++] = 'V';
	data[commandIndex++] = 'P';
	return commandIndex;
}

int CSerialPortVjtMonoBlock::ComposeCurrentProgramCommand(BYTE data[])
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Send VjtMonoBlock program x-ray current.", cDebugMessage);

	int commandIndex = 1;
	data[commandIndex++] = 'C';
	data[commandIndex++] = 'P';
	return commandIndex;
}

int CSerialPortVjtMonoBlock::ComposeMonitorCommand(BYTE data[])
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Send VjtMonoBlock get monitoring data.", cDebugMessage);

	int commandIndex = 1;
	data[commandIndex++] = 'M';
	data[commandIndex++] = 'O';
	data[commandIndex++] = 'N';
	return commandIndex;
}

int CSerialPortVjtMonoBlock::ComposeClearFaultCommand(BYTE data[])
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Send VjtMonoBlock clear fault.", cDebugMessage);

	int commandIndex = 1;
	data[commandIndex++] = 'C';
	data[commandIndex++] = 'L';
	data[commandIndex++] = 'R';
	return commandIndex;
}

int CSerialPortVjtMonoBlock::ComposeReportFaultCommand(BYTE data[])
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Send VjtMonoBlock get fault data.", cDebugMessage);

	int commandIndex = 1;
	data[commandIndex++] = 'F';
	data[commandIndex++] = 'L';
	data[commandIndex++] = 'T';
	return commandIndex;
}

int CSerialPortVjtMonoBlock::ComposeHvStatusCommand(BYTE data[])
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Send VjtMonoBlock get HV status.", cDebugMessage);

	int commandIndex = 1;
	data[commandIndex++] = 'S';
	data[commandIndex++] = 'T';
	data[commandIndex++] = 'A';
	data[commandIndex++] = 'T';
	return commandIndex;
}

int CSerialPortVjtMonoBlock::ComposeXayEnableCommand(BYTE data[])
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Send VjtMonoBlock enable x-rays.", cDebugMessage);

	int commandIndex = 1;
	data[commandIndex++] = 'E';
	data[commandIndex++] = 'N';
	data[commandIndex++] = 'B';
	data[commandIndex++] = 'L';
	return commandIndex;
}

int CSerialPortVjtMonoBlock::ComposeCommPortEchoCommand(BYTE data[])
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Send VjtMonoBlock get echo request.", cDebugMessage);

	int commandIndex = 1;
	data[commandIndex++] = 'F';
	data[commandIndex++] = 'R';
	data[commandIndex++] = 'E';
	data[commandIndex++] = 'V';
	return commandIndex;
}

int CSerialPortVjtMonoBlock::ComposeWatchDogOkCommand(BYTE data[])
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("Send VjtMonoBlock get watchdog OK.", cDebugMessage);

	int commandIndex = 1;
	data[commandIndex++] = 'W';
	data[commandIndex++] = 'D';
	data[commandIndex++] = 'T';
	data[commandIndex++] = 'E';
	return commandIndex;
}

int CSerialPortVjtMonoBlock::ComposeWatchEnableCommand(BYTE data[])
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Send VjtMonoBlock send watchdog enabled.", cDebugMessage);

	int commandIndex = 1;
	data[commandIndex++] = 'W';
	data[commandIndex++] = 'D';
	data[commandIndex++] = 'O';
	data[commandIndex++] = 'G';
	return commandIndex;
}

int CSerialPortVjtMonoBlock::ComposeWatchDogStatusCommand(BYTE data[])
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Send VjtMonoBlock get watchdog enabled.", cDebugMessage);

	int commandIndex = 1;
	data[commandIndex++] = 'W';
	data[commandIndex++] = 'S';
	data[commandIndex++] = 'T';
	data[commandIndex++] = 'A';
	data[commandIndex++] = 'T';
	return commandIndex;
}

int CSerialPortVjtMonoBlock::ComposeParameterBytes(BYTE data[], double param)
{
	int commandIndex = 3;
	int value6 = ((int)(param) % 10); 
	param = param / 10;
	int value5 = ((int)(param) % 10); 
	param = param / 10;
	int value4 = ((int)(param) % 10); 
	param = param / 10;
	int value3 = ((int)(param) % 10);
	data[commandIndex++] = value3 + 0x30;
	data[commandIndex++] = value4 + 0x30;
	data[commandIndex++] = value5 + 0x30;
	data[commandIndex++] = value6 + 0x30;
	return commandIndex;
}

void CSerialPortVjtMonoBlock::PutVjtCommandInQueue(Constants CommandId, BYTE parameters[])
{
		if (CommandId == XRAY_ENABLE)
		{
			if (parameters[0])
				vXraysPoweredOnStatus = true;
			else
				vXraysPoweredOnStatus = false;
		}
		if (CommandId == WATCH_DOG_ENABLE)
		{
			vNextWatchDogEnable = parameters[0];
			//for debugging
			//ReportErrorMessage("Watch Dog Enable: " + dtoa(vNextWatchDogEnable, 0), cError, 0);
		}
		PutCommandOnQueue(CommandId);
}

void CSerialPortVjtMonoBlock::SendVjtCommand()
{
	if (vPendingCommand[0] != NO_COMMAND)
	{
		if (!vLocalConfigurationData->vSimulateLightControlBoard)
			vWaitingForReply = true;

		int commandIndex = 0;
		tSerialCommand TempCommand;
		TempCommand[commandIndex++] = STX;
		switch (vPendingCommand[0])
		{
		case VOLTAGE_PROGRAM:
			vProgramVoltageAcknowledgedWaitCount = 0;
			vProgramVoltageSent = true;
			vProgramVoltageAcknowledged = false;
			ComposeVoltageProgramCommand(TempCommand);
			commandIndex = ComposeParameterBytes(TempCommand, vVoltageToProgram * 10.0);
			break;
		case CURRENT_PROGRAM:
			vProgramCurrentAcknowledgedWaitCount = 0;
			vProgramCurrentSent = true;
			vProgramCurrentAcknowledged = false;
			ComposeCurrentProgramCommand(TempCommand);
			commandIndex = ComposeParameterBytes(TempCommand, vCurrentToProgram);
			break;
		case MONITOR:
			commandIndex = ComposeMonitorCommand(TempCommand);
			break;
		case CLEAR_FAULT: // This clears all fault conditions except interlock open.
			vClearFaultsAcknowledgedWaitCount = 0;
			vClearFaultsSent = true;
			vClearFaultsAcknowledged = false;
			commandIndex = ComposeClearFaultCommand(TempCommand);
			break;
		case REPORT_FAULT:
			commandIndex = ComposeReportFaultCommand(TempCommand);
			break;
		case HV_STATUS:
			commandIndex = ComposeHvStatusCommand(TempCommand);
			break;
		case XRAY_ENABLE:
			vXraysOnAcknowledgedWaitCount = 0;
			vXraysOnSent = true;
			vXraysOnAcknowledged = false;
			commandIndex = ComposeXayEnableCommand(TempCommand);
			TempCommand[commandIndex] = 0x30 + vXraysPoweredOnStatus;
			commandIndex++;
			break;
		case COMM_PORT_ECHO:
			commandIndex = ComposeCommPortEchoCommand(TempCommand);
			break;
		case WATCH_DOG_HEARTBEAT:
			commandIndex = ComposeWatchDogOkCommand(TempCommand);
			break;
		case WATCH_DOG_ENABLE:
			commandIndex = ComposeWatchEnableCommand(TempCommand);
			TempCommand[commandIndex] = vNextWatchDogEnable;
			commandIndex++;
			break;
		case WATCH_DOG_STATUS:
			commandIndex = ComposeWatchDogStatusCommand(TempCommand);
			break;
		default:	break;
		}
		TempCommand[commandIndex++] = CR;

		vLastMessageSent = vPendingCommand[0];
		if (!vLocalConfigurationData->vDemoMode)
			SendSerialCommand(TempCommand, commandIndex);
		RemoveCommandFromQueue();
				
		if (vLocalConfigurationData->vSimulateLightControlBoard) //OFFICE TESTING
			vLastMessageSent = NO_COMMAND;

		Sleep(cSendCommandSleepTime);
		Sleep(55);
	}
}

void CSerialPortVjtMonoBlock::SendSerialCommand(tSerialCommand TheData, BYTE TempLength)
{
	WORD TempTotalLength = 0;
	tSerialCommand vCommand;

	for (BYTE TempLoop = 0; TempLoop < TempLength; TempLoop++)
		vCommand[TempLoop] = TheData[TempLoop];
	TempTotalLength = TempLength;

	if (vPortOpen)
	{
		DWORD TempErrorCode = 0;
		DWORD TempBytesWritten = 0;
		int TempDidIt = 0;

		TempDidIt = WriteFile(vWinPortHandle, &vCommand, TempTotalLength, &TempBytesWritten, &vWriteOverlappedStructure);
		TempErrorCode = GetLastError();
		if ((TempErrorCode == ERROR_IO_PENDING) || (TempDidIt))
		{
			WriteCommandToLogFile(vCommand, TempTotalLength);

			vHasTransmittedCommunication = true;
		}
		else
		{
			TempErrorCode = GetLastError();
			ClosePort();
			OpenPort(vComPortNumber);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//
// End SerialPortVjtMonoBlock command implementation.
//
//////////////////////////////////////////////////////////////////////////////////////

void CSerialPortVjtMonoBlock::ProcessDataMessages()
{
	int monitorDataCount = (int)vMonitorMessageData.GetCount();
	int faultDataCount = (int)vFaultMessageData.GetCount();

	if (monitorDataCount > 0)
	{
		vVoltage = ATOF(vMonitorMessageData[0]) / 10.0;
		vLocalSystemData->vSourceVoltage = vVoltage / 6;

		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("VJT Voltage Mon: " + dtoa(vVoltage, 4), cDebugMessage);

		if (vLocalSystemData->vSourceRampedUp)
		if (vLocalSystemData->vXRayOnNotice)
		if (!vLocalSystemData->vShuttingDownXRays)
		if (vLocalSystemData->vSystemRunMode != cCalibratingDetectorOffsetMode)
		if (!vLocalSystemData->vAdjustingSourceSettings)
		if (vLocalSystemData->vRampXRayVoltageAmount > 19 / 6)
		if (vLocalSystemData->vRampXRayCurrentAmount > vMinimumCurrent)
		{
			if (vLocalSystemData->vSourceVoltage > vLocalSystemData->vSourceVoltageMax)
				vLocalSystemData->vSourceVoltageMax = vLocalSystemData->vSourceVoltage;

			if (vLocalSystemData->vSourceVoltageMin > vLocalSystemData->vSourceVoltage)
				vLocalSystemData->vSourceVoltageMin = vLocalSystemData->vSourceVoltage;
		}
	}

	if (monitorDataCount > 1)
	{
		vCurrent = ATOF(vMonitorMessageData[1]);

		vLocalSystemData->vSourceCurrent = (vCurrent / 1000.0);
		
		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("VJT Current Mon: " + dtoa(vLocalSystemData->vSourceCurrent, 4), cDebugMessage);

		if (vLocalSystemData->vSourceRampedUp)
		if (vLocalSystemData->vXRayOnNotice)
		if (!vLocalSystemData->vShuttingDownXRays)
		if (vLocalSystemData->vSystemRunMode != cCalibratingDetectorOffsetMode)
		if (!vLocalSystemData->vAdjustingSourceSettings)
		if (vLocalSystemData->vRampXRayVoltageAmount > 19 / 6)
		if (vLocalSystemData->vRampXRayCurrentAmount > 0)
		{
			if (vLocalSystemData->vSourceCurrent > vLocalSystemData->vSourceCurrentMax)
				vLocalSystemData->vSourceCurrentMax = vLocalSystemData->vSourceCurrent;

			if (vLocalSystemData->vSourceCurrentMin > vLocalSystemData->vSourceCurrent)
				vLocalSystemData->vSourceCurrentMin = vLocalSystemData->vSourceCurrent;
		}
	}

		//ReportErrorMessage("VJT Voltage Monitor: " + dtoa(vLocalSystemData->vSourceVoltage, 4) + ", Current: " + dtoa(vLocalSystemData->vSourceCurrentMax, 4), cError, 0);

	if (monitorDataCount > 2)
	{
		vTemperature = ATOF(vMonitorMessageData[2]) / 10.0;

		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("VJT Temperature Mon: " + dtoa(vTemperature, 4), cDebugMessage);

		if (vTemperature > vTemperatureMaximum)
			vTemperatureMaximum = vTemperature;
		if (vTemperatureMinimum > vTemperature)
			vTemperatureMinimum = vTemperature;

		if ((BYTE)vTemperature > vLocalConfigurationData->vMaximumMonoBlockTemperature)
		{
			if ((vLocalSystemData->vSourceRampedUp) && (!vLocalSystemData->vShuttingDownXRays) && (vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
			{
				::PostThreadMessage(vGlobalCallingThreadID, cEmergencyShutXRaysDownMessage, 0, 0);
				CString TempText = "-";
				TempText.LoadStringW(IDS_XRaySourceTooHot);
				ReportErrorMessage(vHVPSType + " " + TempText, cError, 9);
				ReportErrorMessage(vHVPSType + " is Too Hot-Error-Inspection Stopped", cEMailInspx, 32000);
				if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) && (vLocalSystemData->vXRayOnNotice) && (!vLocalSystemData->vSourceErrorReported))
					::PostThreadMessage(vGlobalCallingThreadID, cEmergencyShutXRaysDownMessage, 0, 0);
			}
			else
				ReportErrorMessage(vHVPSType + " is Too Hot-Notice-Not Inspecting", cWriteToLog, 0);
		}
	}

	if (monitorDataCount > 3)
	{
		vFilament = ATOF(vMonitorMessageData[3]);
		vFilamentCurrentFeedbackMonitor = (vFilament / 1000.0);


		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("VJT Filament Mon: " + dtoa(vFilamentCurrentFeedbackMonitor, 4), cDebugMessage);

		if (vFilamentCurrentMin > vFilamentCurrentFeedbackMonitor)
			vFilamentCurrentMin = vFilamentCurrentFeedbackMonitor;
		if (vFilamentCurrentMax < vFilamentCurrentFeedbackMonitor)
			vFilamentCurrentMax = vFilamentCurrentFeedbackMonitor;
	}
	// The documentation had the power limit and interlock fault bits reversed. 
	if (faultDataCount > 0)
	{
		CString val(vFaultMessageData[0]);
		if (val.Compare(_T("0")) == 0)
			//vRegulationFault = false;
			vOverVoltageFault = false;
		else
		{
			if (val.Compare(_T("1")) == 0)
				//vRegulationFault = true;
				vOverVoltageFault = true;
		}
	}
	if (faultDataCount > 1)
	{
		CString val(vFaultMessageData[1]);
		if (val.Compare(_T("0")) == 0)
			vPowerLimitFault = false;
		else
		{
			if (val.Compare(_T("1")) == 0)
				vPowerLimitFault = true;
		}
	}
	if (faultDataCount > 2)
	{
		CString val(vFaultMessageData[2]);
		if (val.Compare(_T("0")) == 0)
			//vCathodeOverVoltageFault = false;
			vOverCurrentFault = false;
		else
		{
			if (val.Compare(_T("1")) == 0)
				//vCathodeOverVoltageFault = true;
				vOverCurrentFault = true;
		}
	}
	if (faultDataCount > 3)
	{
		CString val(vFaultMessageData[3]);
		if (val.Compare(_T("0")) == 0)
			//vAnodeOverVoltageFault = false;
			vArcFault = false;
		else
		{
			if (val.Compare(_T("1")) == 0)
				//vAnodeOverVoltageFault = true;
				vArcFault = true;
		}
	}
	if (faultDataCount > 4)
	{
		CString val(vFaultMessageData[4]);
		if (val.Compare(_T("0")) == 0)
			vOverTemperatureFault = false;
		else
		{
			if (val.Compare(_T("1")) == 0)
				vOverTemperatureFault = true;
		}
	}
	if (faultDataCount > 5)
	{
		CString val(vFaultMessageData[5]);
		if (val.Compare(_T("0")) == 0)
			//vArcFault = false;
			vAnodeOverVoltageFault = false;
		else
		{
			if (val.Compare(_T("1")) == 0)
				//vArcFault = true;
				vAnodeOverVoltageFault = true;
		}
	}
	if (faultDataCount > 6)
	{
		CString val(vFaultMessageData[6]);
		if (val.Compare(_T("0")) == 0)
			//vOverCurrentFault = false;
			vCathodeOverVoltageFault = false;
		else
		{
			if (val.Compare(_T("1")) == 0)
				//vOverCurrentFault = true;
				vCathodeOverVoltageFault = true;
		}
	}
	if (faultDataCount > 7)
	{
		CString val(vFaultMessageData[7]);
		if (val.Compare(_T("0")) == 0)
		{
			//ReportErrorMessage("VJT Interlock input closed", cWriteToLog, 0);
			if (!vInterLockStatus)
			{
				vInterlockOpenFault = false;
				vInterLockStatus = true; 
				vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks | 0x0020); //set bit 6, the x-ray interlock bit
				::PostThreadMessage(vGlobalCallingThreadID, cCheckInterlocksIn200MSMessage, 0, 0);

				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("VJT Interlock changed: " + dtoa(vInterLockStatus, 4), cDebugMessage);
			}
		}
		else
		{
			if (val.Compare(_T("1")) == 0)
			{
				if (vInterLockStatus)
				{
					//ReportErrorMessage("VJT Interlock input opened", cWriteToLog, 0);
					//vGlobalXRaySourceInterlockInputOpen = 10;
					vInterlockOpenFault = true;
					vInterLockStatus = false;
					vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks & 0xFFDF); //clear bit 6, the x-ray interlock bit
					::PostThreadMessage(vGlobalCallingThreadID, cCheckInterlocksIn200MSMessage, 0, 0);

					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("VJT Interlock changed: " + dtoa(vInterLockStatus, 4), cDebugMessage);
				}
			}
		}
	}
	if (faultDataCount > 8)
	{
		CString val(vFaultMessageData[8]);
		if (val.Compare(_T("0")) == 0)
			//vOverVoltageFault = false;
			vRegulationFault = false;
		else
		{
			if (val.Compare(_T("1")) == 0)
				//vOverVoltageFault = true;
				vRegulationFault = true;
		}
	}

	if (monitorDataCount > 0) vMonitorMessageData.RemoveAll();
	if (faultDataCount > 0) vFaultMessageData.RemoveAll();
}

void CSerialPortVjtMonoBlock::SendInitialSetupData()
{
	vSetupDataSent = true;
	PutVjtCommandInQueue(COMM_PORT_ECHO, 0);
	PutVjtCommandInQueue(CLEAR_FAULT, 0);
	//BYTE TempStartWatchDog = 0;
	//PutVjtCommandInQueue(WATCH_DOG_ENABLE, &TempStartWatchDog);
	//Sleep(200);
	// The VJT Monoblock has no programmed voltage or curent on power up, but main program should set
	// and has an initial regulation fault that needs clearing.
	//vCurrentToProgram = 200;
	//vVoltageToProgram = 40;
	//PutCommandOnQueue(CURRENT_PROGRAM);
}

void CSerialPortVjtMonoBlock::DoBackGroundWork()
{
	if (vMyPointer->vLocalConfigurationData->vDemoMode == (BYTE)true)
		DemoModeMockUp();
	else
		if (!vThreadStarted)
		{
			//vThreadPaused = false;
			vThreadStarted = true;
			vWorkerThread->ResumeThread();
		}
}

void CSerialPortVjtMonoBlock::PauseBackGroundWork()
{
	//if (vMyPointer->vLocalConfigurationData->vDemoMode == (BYTE)false)
	//	if (vThreadStarted || !vThreadPaused)
	//	{
	//		vWorkerThread->SuspendThread();
	//		vThreadPaused = true;
	//		if (vConnectedDeviceVerified && !vSentCommLossMessage)
	//		{
	//			vSentCommLossMessage = true;
	//			::PostThreadMessage(vGlobalCallingThreadID, cCheckInterlocksIn200MSMessage, 0, 0);
	//			if (!vLocalConfigurationData->vSimulateLightControlBoard)
	//			{
	//				CString msg = "Lost comm with: " + vDeviceName + ", " + vComPortString);
	//				PostErrorMessage(msg, true);
	//				ReportErrorMessage("Lost Communication with " + vDeviceName + " on " + vComPortString, cEMailInspx, 32000);
	//			}
	//			else
	//				ReportErrorMessage("Lost Communication with " + vDeviceName + " on " + vComPortString, cEMailInspx, 32000);
	//		}
	//	}
}

void CSerialPortVjtMonoBlock::ResumeBackGroundWork()
{
		if (!vThreadStarted)
		{
			VerifyConnectedDevice();
			vThreadStarted = true;
			vWorkerThread->ResumeThread();
		}
}

UINT CSerialPortVjtMonoBlock::ThreadProc(LPVOID param)
{
	for (; 1;) { vMyPointer->DoBackGroundWorkThreadProc(); Sleep(200); }
	return 0;
}

UINT CSerialPortVjtMonoBlock::DoBackGroundWorkThreadProc()
{
	if (vWaitingForReply || vLastMessageSent || vNoCommunications)
	{
		if (vCommunicationsTimeOutCounter < 200)
			vCommunicationsTimeOutCounter++;

		// Time interval setting based on 500ms between calls. 10 seconds window required.
		if (vCommunicationsTimeOutCounter == 10)
		{
			vWaitingForReply = false; //lost comm, but retry
			vLastMessageSent = 0;
			if (vPendingCommand[0] == NO_COMMAND)
				PutVjtCommandInQueue(HV_STATUS,0);

			SendVjtCommand();
		}
		if (vCommunicationsTimeOutCounter >= 25)
		{
			vWaitingForReply = false; //lost comm, but retry
			vCommunicationsTimeOutCounter = 1;
			vLastMessageSent = 0;
			//if (!vNoCommunications) //if was communicating, flag that we lost com as last command was not answered
			{// Leaving the above line of code in to keep resemblence to non-usb serial device code structure.
				vNoCommunications = true;
				//vLocalSystemData->vHVPSNoComm = true;
				::PostThreadMessage(vGlobalCallingThreadID, cCheckInterlocksIn200MSMessage, 0, 0);
				//if (vConnectedDeviceVerified && !vSentCommLossMessage)
				{
					vSentCommLossMessage = true;
					//if (!vLocalConfigurationData->vSimulateLightControlBoard)
					{
						ReportErrorMessage("Lost Communication with VJT, Retry: " + vComPortString, cWriteToLog, 0);
						if (vPendingCommand[0] == NO_COMMAND)
							PutVjtCommandInQueue(HV_STATUS,0);

						SendVjtCommand();

						//CString msg = "Lost Comm with: " + vDeviceName + ", " + vComPortString;
						////vGlobalMainWindowPointer->vSetInterlockStatusEvent.PulseEvent();
						//PostErrorMessage(msg, true);
						//ReportErrorMessage("Lost Communication with " + vDeviceName + " on " + vComPortString, cEMailInspx, 32000);
					}
				}
			}
		}
	}
	else
	if (!vWaitingForReply)
	if (!vNoCommunications)
	if (vPendingCommand[0] == NO_COMMAND)
	{
		vGetStatusDelay++;
		switch (vGetStatusDelay % 15)
		{
			case 1:	PutVjtCommandInQueue(HV_STATUS,0); break;
			case 4:	PutVjtCommandInQueue(MONITOR,0); break;
			case 7:	PutVjtCommandInQueue(REPORT_FAULT,0); break;
			case 10:	PutVjtCommandInQueue(WATCH_DOG_HEARTBEAT,0); break;
			case 13:	PutVjtCommandInQueue(WATCH_DOG_STATUS,0); break;
		}
		//WATCH_DOG_ENABLE = 0x0A, // "WDOGX"
	}

	//else
	//{// Port recovery
	//	//if (vNoCommunications)
	//	//	VerifyConnectedDevice();
	//	//else
	//	{
	//		vCommunicationsTimeOutCounter = 0;

	//		if (vSentCommLossMessage)
	//		{
	//			vSentCommLossMessage = false;
	//			CString msg = "Restored Serial Comm with: " + vDeviceName + ", " + vComPortString;
	//			ReportErrorMessage(msg, cError, 0);
	//			//vLocalSystemData->vHVPSNoComm = false;
	//			ReportErrorMessage("Restored Communication with " + vDeviceName + " on " + vComPortString, cEMailInspx, 32000);
	//			vRecoveredFromComLoss = true;
	//		}
	//	}
	//}

	vSyncSect.Lock();
	ProcessDataMessages();
	vSyncSect.Unlock();

	vDataNotificationEvent.PulseEvent();

	if (!vLastMessageSent)
	{
		if (vConnectedDeviceVerified && !vSetupDataSent)
			SendInitialSetupData();
	}
	ProcessFaultConditions();

	if (!vWaitingForReply)
	if (vPendingCommand[0] != NO_COMMAND)
		SendVjtCommand();

	return 0;
}

bool CSerialPortVjtMonoBlock::SerialMessageComplete()
{
	if (FrameMessageAtHeaderByte())
	{
		//int EtxIndex = vReceiveDataTail;
		//while (!FrameMessageAtTailByte())
		//	return false;
		return true;
	}
	return false;
}

bool CSerialPortVjtMonoBlock::FrameMessageAtHeaderByte()
{
	WORD TempBytesAvailableInQueue = ReceiveDataCount();
	if (TempBytesAvailableInQueue)
	{
		if (TempBytesAvailableInQueue > 2)
		if (vReceiveDataBuffer[vReceiveDataTail] == STX)
		{
			for (WORD TempLoop = 1; TempLoop < TempBytesAvailableInQueue; TempLoop++)
			{
				WORD TempIndex = (vReceiveDataTail + TempLoop) % cReceiveBufferSize; 
				if (vReceiveDataBuffer[TempIndex] == CR)
					return true;
			}
		}

		if (vReceiveDataBuffer[vReceiveDataTail] != STX) //not a start of message, so toss out
		{
			ReportErrorMessage(vComPortString + " byte not STX, throw away: " + ByteToHex(vReceiveDataBuffer[vReceiveDataTail]), cEMailInspx, 32000);
	
			vReceiveDataBuffer[vReceiveDataTail] = 0;  //clear the buffer so don't confuse old ETX

			if (vReceiveDataTail < cReceiveBufferSize - 1)
				vReceiveDataTail++;
			else
				vReceiveDataTail = 0;

		}
	}
	return false;
}

bool CSerialPortVjtMonoBlock::FrameMessageAtTailByte()
{
	int HeadIndex = vReceiveDataTail;
	while (ReceiveDataCount())
	{
		if (vReceiveDataBuffer[HeadIndex] == CR)
		{
			return true;
		}
		if (HeadIndex < cReceiveBufferSize - 1)
			HeadIndex++;
		else
			HeadIndex = 0;
		if (HeadIndex == vReceiveDataHead)
			return false;
	}
	return false;
}

void CSerialPortVjtMonoBlock::VerifyConnectedDevice()
{
	vWaitingForReply = false;
	PutVjtCommandInQueue(COMM_PORT_ECHO, 0);
	//if (!vConnectedDeviceVerified) 
	//	Sleep(500);
}

//void CSerialPortVjtMonoBlock::SendClearFaults()
//{
//	PutCommandOnQueue(CLEAR_FAULT);
//}
//
//bool CSerialPortVjtMonoBlock::GetEnableHeatbeat()
//{
//	return vSendWatchdogHeartBeat;
//}
//
//void CSerialPortVjtMonoBlock::SetEnableHeartbeat(bool enable)
//{
//	if (vWatchdogEnabledActive)
//		vSendWatchdogHeartBeat = enable;
//	else
//		vSendWatchdogHeartBeat = false;
//}
//
//void CSerialPortVjtMonoBlock::SendEnableXrays(bool OnOff)
//{
//	vXraysPoweredOnStatus = OnOff;
//	PutCommandOnQueue(XRAY_ENABLE);
//}

void CSerialPortVjtMonoBlock::SetXRayVoltage(double TempVoltage)
{// Range 30 to 80kV
	if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("Program calls Set voltage in VJT com3.", cDebugMessage);


	double TempVoltageToCheck = TempVoltage * 6;
	//if (TempVoltageToCheck >= 40)
	{
		vVoltageToProgram = TempVoltageToCheck;
		//if (vVoltageToProgram < 40)
		//{
		//	vVoltageToProgram = 40;
		//	if (vHvStatusOn)
		//	if (vLocalSystemData)
		//		vLocalSystemData->vRampXRayVoltageAmount = 40 / 6;
		//		
		//		
			//for debugging
		//		//ReportErrorMessage("Error com3 Temp Set VoltageTo 40 in VJT", cError, 0);
		//}
		if (vVoltageToProgram > 80)
		{
			 vVoltageToProgram = 80;
			if (vHvStatusOn)
			if (vLocalSystemData)
				vLocalSystemData->vRampXRayVoltageAmount = 80;

				//for debugging
				//ReportErrorMessage("Error com3 Temp Set VoltageTo 80 in VJT", cError, 0);
		}
		PutCommandOnQueue(VOLTAGE_PROGRAM);
	}
	//Sleep(500); // 500ms delay needed to decode response.
}

void CSerialPortVjtMonoBlock::SetXRayCurrent(double TempCurrent)
{// Range 200 to 8000uA
	//if (TempCurrent > 0)
	{
		double TempCheckedCurrent = TempCurrent;
		//if (TempCheckedCurrent >= vMinimumCurrent) //monoblock gives a regulation error if start at 0 current and go up from there.
		{
			if (TempCheckedCurrent < vMinimumCurrent) //vMinimumCurrent
			{
				if (!vTemporarilyTurnedOffHVForLowCurrent)
				if (vXraysPoweredOnStatus)
				{
					vTemporarilyTurnedOffHVForLowCurrent = true;
					TurnOnOffXRays(false);
				}
				TempCheckedCurrent = vMinimumCurrent;
				if (vHvStatusOn)
				if (vLocalSystemData)
					vLocalSystemData->vRampXRayCurrentAmount = vMinimumCurrent;
					//for debugging
					//ReportErrorMessage("Error com3 Temp Set CurrentTo .2 in VJT", cError, 0);
			}
			else
			if (vTemporarilyTurnedOffHVForLowCurrent)
			{
				vTemporarilyTurnedOffHVForLowCurrent = false;
				TurnOnOffXRays(true);
			}

			if (TempCheckedCurrent > 2.5)
			{
				TempCheckedCurrent = 2.5;
				if (vHvStatusOn)
				if (vLocalSystemData)
					vLocalSystemData->vRampXRayCurrentAmount = 2.5;
					//for debugging
					//ReportErrorMessage("Error com3 Temp Set CurrentTo 2.5 in VJT", cError, 0);
			}
			vCurrentToProgram = (TempCheckedCurrent * 1000.0);

			//for debugging
			//ReportErrorMessage("VJT Set Current To: " + dtoa(vCurrentToProgram, 4), cError, 0);
			PutCommandOnQueue(CURRENT_PROGRAM);
		}
	}
}

void CSerialPortVjtMonoBlock::TurnOnOffXRays(BYTE TempOn)
{
	if (TempOn == 0x01) 
		vXraysPoweredOnStatus = true;
	else 
		vXraysPoweredOnStatus = false;

	if (TempOn == false)
	if (vTemporarilyTurnedOffHVForLowCurrent)
	if (vLocalSystemData->vXRayOnNotice == false) //if turning off X-Rays and had turned off for low current condition, clear flag
		vTemporarilyTurnedOffHVForLowCurrent = false;

	if (TempOn == 0x01) 
		PutVjtCommandInQueue(CLEAR_FAULT, 0);

	if (((vCurrentToProgram >= vMinimumCurrent * 1000.0 ) && (vVoltageToProgram >= 39.9)) || (!TempOn))
	{
		//for debugging
		//if (TempOn == 0x01) 
		//	ReportErrorMessage("VJT X-Rays On", cError, 0);
		//else 
		//	ReportErrorMessage("VJT X-Rays Off", cError, 0);

		PutCommandOnQueue(XRAY_ENABLE);
	}
		//for debugging
	//else
	//	ReportErrorMessage("VJT Ignore X-Rays On command, V or I low. V: " + dtoa(vVoltageToProgram,4) + ", I: " + dtoa(vCurrentToProgram, 4), cError, 0);
}

void CSerialPortVjtMonoBlock::ProcessFaultConditions()
{
	if (!vNoCommunications && vConnectedDeviceVerified)
	{
		CheckFaultsWithTimeouts();
		CheckMonitoredFaults();
		if (vRegulationFaultReported || vPowerLimitFaultReported || vCathodeOverVoltageFaultReported || vAnodeOverVoltageFaultReported ||
			vOverTemperatureFaultReported || vArcFaultReported || vOverCurrentFaultReported || vOverVoltageFaultReported)
			vFaultPresentCount++;
		else
			vFaultPresentCount = 0;
		if (vFaultPresentCount >= 3)
		{// Faults need to be cleared before x-rays can continue.
			vFaultPresentCount = 0;
			PutCommandOnQueue(CLEAR_FAULT); // Does not clear the interlock open fault.
		}
	}
	else
		ResetFaultConditions();
}

void CSerialPortVjtMonoBlock::ResetFaultConditions()
{// Reset everything on COM loss.
	vXraysOnSent = false;
	vProgramVoltageSent = false;
	vProgramCurrentSent = false;
	vClearFaultsSent = false;
	vXraysOnAcknowledged = false;
	vProgramVoltageAcknowledged = false;
	vProgramCurrentAcknowledged = false;
	vClearFaultsAcknowledged = false;
	vXraysOnAcknowledgedWaitCount = 0;
	vProgramVoltageAcknowledgedWaitCount = 0;
	vProgramCurrentAcknowledgedWaitCount = 0;
	vClearFaultsAcknowledgedWaitCount = 0;
	///////////////////////////////////////////////////
	vInterlockOpenFault = false;
	vInterlockFaultReported = false;
	vRegulationFault = false;
	vRegulationFaultReported = false;
	vPowerLimitFault = false;
	vPowerLimitFaultReported = false;
	vCathodeOverVoltageFault = false;
	vCathodeOverVoltageFaultReported = false;
	vAnodeOverVoltageFault = false;
	vAnodeOverVoltageFaultReported = false;
	vOverTemperatureFault = false;
	vOverTemperatureFaultReported = false;
	vArcFault = false;
	vArcFaultReported = false;
	vOverCurrentFault = false;
	vOverCurrentFaultReported = false;
	vOverVoltageFault = false;
	vOverVoltageFaultReported = false;
}

void CSerialPortVjtMonoBlock::CheckFaultsWithTimeouts()
{// Faults with time outs are critical control commands
// so notify the user if a problem is occurring.
	CheckXraysOnOffAcked();
	CheckProgramVoltageAcked();
	CheckProgramCurrentAcked();
	CheckClearFaultsAcked();
}

void CSerialPortVjtMonoBlock::CheckMonitoredFaults()
{// Monitored faults are set internally by the VJT and 
// will shut down x-rays so notify the user.
	CheckRegulationFault();
	CheckPowerLimitFault();
	CheckCathodeOverVoltageFault();
	CheckAnodeOverVoltageFault();
	CheckOverTemperatureFault();
	CheckArcFault();
	CheckOverCurrentFault();
	CheckOverVoltageFault();
	CheckInterlockOpenFault();
}

///////////////////////////////////////////////////////////////
/// Timed out faults
///////////////////////////////////////////////////////////////
void CSerialPortVjtMonoBlock::CheckXraysOnOffAcked()
{// Check X-Ray command response OK
	CString msgText;
	if (vXraysOnSent && !vXraysOnAcknowledged)
	{
		if (vXraysOnAcknowledgedWaitCount == 3)
		{
			vXraysOnSent = false;
			msgText.Format(_T("Error - VJT XRays ON/OFF Not Acked: Com%d"), vComPortNumber);
			PostErrorMessage(msgText, false);
		}
		else
			vXraysOnAcknowledgedWaitCount++;
	}
}

void CSerialPortVjtMonoBlock::CheckProgramVoltageAcked()
{// Check Program Voltage command response OK
	CString msgText;
	if (vProgramVoltageSent && !vProgramVoltageAcknowledged)
	{
		if (vProgramVoltageAcknowledgedWaitCount == 3)
		{
			vProgramVoltageSent = false;
			msgText.Format(_T("Error - VJT Voltage Programming Not Acked: Com%d"), vComPortNumber);
			PostErrorMessage(msgText, false);
		}
		else
			vProgramVoltageAcknowledgedWaitCount++;
	}
}

void CSerialPortVjtMonoBlock::CheckProgramCurrentAcked()
{// Check Program command response OK
	CString msgText;
	if (vProgramCurrentSent && !vProgramCurrentAcknowledged)
	{
		if (vProgramCurrentAcknowledgedWaitCount == 3)
		{
			vProgramCurrentSent = false;
			msgText.Format(_T("Error - VJT Current Programming Not Acked: Com%d"), vComPortNumber);
			PostErrorMessage(msgText, false);
		}
		else
			vProgramCurrentAcknowledgedWaitCount++;
	}
}

void CSerialPortVjtMonoBlock::CheckClearFaultsAcked()
{// Check Clear Faults command response OK
	CString msgText;
	if (vClearFaultsSent && !vClearFaultsAcknowledged)
	{
		if (vClearFaultsAcknowledgedWaitCount == 3)
		{
			vClearFaultsSent = false;
			msgText.Format(_T("Error - VJT Clear Faults Not Acked: Com%d"), vComPortNumber);
			PostErrorMessage(msgText, false);
		}
		else
			vClearFaultsAcknowledgedWaitCount++;
	}
}

void CSerialPortVjtMonoBlock::ProcessSerialMessage()
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

void CSerialPortVjtMonoBlock::ShowSerialData()  //data received
{
	if (vLocalSystemData->vLogFile.vLogSerialData)// || (vLocalSystemData->vLogFile.vShowingSerialDataOnScreen))
	{
		CString TempString = "";
		TempString = vComPortString + ":";
		BYTE TempByte = 0;
		vCurrentReceiveCount = ReceiveDataCount();
		if (vCurrentReceiveCount > 0)
		{
			//add the decimal values of the string on the end
			WORD TempIndex = vReceiveDataTail;
			for (BYTE TempLoop = 0; TempLoop < 30; TempLoop++)
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
				if (TempByte == 0x0D) break;
			}
			vLocalSystemData->vLogFile.WriteToLogFile(TempString, cInputMessage);
		}
	}
}
///////////////////////////////////////////////////////////////
/// Monitored out faults
///////////////////////////////////////////////////////////////
void CSerialPortVjtMonoBlock::CheckInterlockOpenFault()
{
	CString msgText;
	if (vInterlockOpenFault)
	{
		if (vInterlockFaultDelayCounter < 3)
			vInterlockFaultDelayCounter++;
		if (vInterlockFaultDelayCounter > 2)
		{
			if ((vGlobalGuardMaster->vGuardMasterRawInterlockData & cInterlockOutputBits) == cInterlockOutputBits)
			{
				if (!vInterlockFaultReported)
				{
					vInterlockFaultReported = true;
					//msgText.Format(_T("Error - VJT Interlock Open fault reported: Com%d"), vComPortNumber);
					msgText = "Error-X-Ray Source Interlock Opened-Inspection Stopped";
					if (vHvStatusOn)
						PostErrorMessage(msgText, true);
					if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) && (vLocalSystemData->vXRayOnNotice) && (!vLocalSystemData->vSourceErrorReported))
						::PostThreadMessage(vGlobalCallingThreadID, cEmergencyShutXRaysDownMessage, 0, 0);
				}
			}
		}
	}
	else
	{
		vInterlockFaultDelayCounter = 0;
		vInterlockFaultReported = false;
	}
}

void CSerialPortVjtMonoBlock::CheckRegulationFault()
{
	CString TempText;
	if (vRegulationFault)
	{
		if (!vRegulationFaultReported)
		{
			vRegulationFaultReported = true;
			//msgText.Format(_T("Error - VJT Regulation fault reported: Com%d"), vComPortNumber);
			//TempText.LoadStringW(IDS_ErrorXRaySourceRegulationFaultInspectionStopped);
			PostErrorMessage("Error-X-Ray Source Regulation Fault-Inspection Stopped", true);
			if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) && (vLocalSystemData->vXRayOnNotice) && (!vLocalSystemData->vSourceErrorReported))
				::PostThreadMessage(vGlobalCallingThreadID, cEmergencyShutXRaysDownMessage, 0, 0);
		}
	}
	else
		vRegulationFaultReported = false;
}

void CSerialPortVjtMonoBlock::CheckPowerLimitFault()
{
	CString msgText;
	if (vPowerLimitFault)
	{
		if (!vPowerLimitFaultReported)
		{
			vPowerLimitFaultReported = true;
			//msgText.Format(_T("Error - VJT Power Limit fault reported: Com%d"), vComPortNumber);
			//msgText.LoadStringW(IDS_ErrorOverPowerLimitInspectionStopped);
			PostErrorMessage("Error-X-Ray Source Over Power-Inspection Stopped", true);
			if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) && (vLocalSystemData->vXRayOnNotice) && (!vLocalSystemData->vSourceErrorReported))
				::PostThreadMessage(vGlobalCallingThreadID, cEmergencyShutXRaysDownMessage, 0, 0);
		}
	}
	else
		vPowerLimitFaultReported = false;
}

void CSerialPortVjtMonoBlock::CheckCathodeOverVoltageFault()
{
	CString msgText;
	if (vCathodeOverVoltageFault)
	{
		if (!vCathodeOverVoltageFaultReported)
		{
			vCathodeOverVoltageFaultReported = true;
			//msgText.Format(_T("Error - VJT Cathode Over Voltage fault reported: Com%d"), vComPortNumber);
			//msgText.LoadStringW(IDS_MonoBlockErrorXRaySourceOverVoltageInspectionStopped);
			PostErrorMessage("Error-X-Ray Source Over Voltage-Inspection Stopped", true);
			if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) && (vLocalSystemData->vXRayOnNotice) && (!vLocalSystemData->vSourceErrorReported))
				::PostThreadMessage(vGlobalCallingThreadID, cEmergencyShutXRaysDownMessage, 0, 0);
		}
	}
	else
		vCathodeOverVoltageFaultReported = false;
}

void CSerialPortVjtMonoBlock::CheckAnodeOverVoltageFault()
{
	CString msgText;
	if (vAnodeOverVoltageFault)
	{
		if (!vAnodeOverVoltageFaultReported)
		{
			vAnodeOverVoltageFaultReported = true;
			//msgText.Format(_T("Error - VJT Anode Over Voltage fault reported: Com%d"), vComPortNumber);
			//msgText.LoadStringW(IDS_MonoBlockErrorXRaySourceOverVoltageInspectionStopped);
			PostErrorMessage("Error-X-Ray Source OverVoltage-Inspection Stopped", true);
			if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) && (vLocalSystemData->vXRayOnNotice) && (!vLocalSystemData->vSourceErrorReported))
				::PostThreadMessage(vGlobalCallingThreadID, cEmergencyShutXRaysDownMessage, 0, 0);
		}
	}
	else
		vAnodeOverVoltageFaultReported = false;
}

void CSerialPortVjtMonoBlock::CheckOverTemperatureFault()
{
	CString msgText;
	if (vOverTemperatureFault)
	{
		if (!vOverTemperatureFaultReported)
		{
			vOverTemperatureFaultReported = true;
			//msgText.Format(_T("Error - VJT Over Temperature fault reported: Com%d"), vComPortNumber);
			//msgText.LoadStringW(IDS_MonoBlockErrorOverTempInspectionStopped);
			PostErrorMessage("Error-X-Ray Source Over Temperature-Inspection Stopped", true);
			if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) && (vLocalSystemData->vXRayOnNotice) && (!vLocalSystemData->vSourceErrorReported))
				::PostThreadMessage(vGlobalCallingThreadID, cEmergencyShutXRaysDownMessage, 0, 0);
		}
	}
	else
		vOverTemperatureFaultReported = false;
}

void CSerialPortVjtMonoBlock::CheckArcFault()
{
	CString msgText;
	if (vArcFault)
	{
		//vLocalConfigurationData->LogAnArc();
		if (!vArcFaultReported)
		{
			vArcFaultReported = true;
			//msgText.Format(_T("Error - VJT Arc fault reported: Com%d"), vComPortNumber);
			//msgText.LoadStringW(IDS_ErrorXRaySourceArcFaultInspectionStopped);
			PostErrorMessage("Error-X-Ray Source Arc Fault-Inspection Stopped", true);
			if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) && (vLocalSystemData->vXRayOnNotice) && (!vLocalSystemData->vSourceErrorReported))
				::PostThreadMessage(vGlobalCallingThreadID, cEmergencyShutXRaysDownMessage, 0, 0);
		}
	}
	else
		vArcFaultReported = false;
}

void CSerialPortVjtMonoBlock::CheckOverCurrentFault()
{
	CString msgText;
	if (vOverCurrentFault)
	{
		if (!vOverCurrentFaultReported)
		{
			vOverCurrentFaultReported = true;
			//msgText.Format(_T("Error - VJT Over Current fault reported: Com%d"), vComPortNumber);
			//msgText.LoadStringW(IDS_ErrorXRaySourceOverCurrentInspectionStopped);
			PostErrorMessage("Error-X-Ray Source Over Current-Inspection Stopped", true);
			if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) && (vLocalSystemData->vXRayOnNotice) && (!vLocalSystemData->vSourceErrorReported))
				::PostThreadMessage(vGlobalCallingThreadID, cEmergencyShutXRaysDownMessage, 0, 0);
		}
	}
	else
		vOverCurrentFaultReported = false;
}

void CSerialPortVjtMonoBlock::CheckOverVoltageFault()
{
	CString msgText;
	if (vOverVoltageFault)
	{
		if (!vOverVoltageFaultReported)
		{
			vOverVoltageFaultReported = true;
			//msgText.Format(_T("Error - VJT Over Voltage fault reported: Com%d"), vComPortNumber);
			msgText = "Error-X-Ray Source Over Voltage-Inspection Stopped";
			PostErrorMessage(msgText, true);
			if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) && (vLocalSystemData->vXRayOnNotice) && (!vLocalSystemData->vSourceErrorReported))
				::PostThreadMessage(vGlobalCallingThreadID, cEmergencyShutXRaysDownMessage, 0, 0);
		}
	}
	else
		vOverVoltageFaultReported = false;
}

void CSerialPortVjtMonoBlock::PostErrorMessage(CString msg, bool reportError)
{
	if (reportError && !vLocalConfigurationData->vSimulateLightControlBoard)
	{
		if (!vHvStatusOn)
			ReportErrorMessage(msg, cError, 0);
		else
		{
			if (!vNoCommunications) // Loss of COMs shutdown handles in ScanTracDlg.
				if (!vLocalSystemData->vRunningDiagnosticsMode)
					ShutDownXrays(msg); // Don't post the xray shut down message unless they are on.
				else
				{
					ReportErrorMessage("Run in Diagnostics Mode, so did not stop X-Rays on VJT error", cEMailInspx, 32000);
					ReportErrorMessage(msg, cEMailInspx, 32000);
				}
		}
	}
	//ReportErrorMessage("Lost Communication with " + vDeviceName + " on " + vComPortString, cEMailInspx, 32000);
}

void CSerialPortVjtMonoBlock::ShutDownXrays(CString reason)
{
	//CString TempText = "-";
	//TempText.LoadStringW(IDS_XRaysShutDown);
	ReportErrorMessage(vDeviceName + " " + reason, cError, 0);
	ReportErrorMessage(vDeviceName + " " + reason, cEMailInspx, 32000);
	if (!vLocalConfigurationData->vSimulateLightControlBoard)
		::PostThreadMessage(vGlobalCallingThreadID, cEmergencyShutXRaysDownMessage, 0, 0);
}

void CSerialPortVjtMonoBlock::SetInRemoteMode(bool TempRemote)
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::SetXRayFilamentLimit(double TempLimit)
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::SetXRayFilamentPreHeat(double TempPreHeat)
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::SetFilamentLimitToDefault()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::SetFilamentLimitToZero()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::VerifySourceWithinTolerance()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::SendXRayCommand(tXRayCommand TempData, BYTE TempNumberOfBytes, BYTE TempCommandSending)
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::GetXRayVoltage()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::GetXRayCurrent()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::GetXRayStatus()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::GetXRaySourceFaultData()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::TurnOnXRayWatchDog()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::TickleXRayWatchDog()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::ClearXRayFault()
{// Overridden from Spellman base for safety.
}

BYTE CSerialPortVjtMonoBlock::CalculateCheckSum(tXRayCommand TempCommand, BYTE TempNumberOfBytes)
{// Overridden from Spellman base for safety.
	return 0;
}

void CSerialPortVjtMonoBlock::GetFilamentFeedback()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::Get15VoltMonitor()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::GetInterlockStatus()
{// Overridden from Spellman base for safety.
}

BYTE CSerialPortVjtMonoBlock::GetFirstEmptyCommandQueueSpot()
{// Overridden from Spellman base for safety.
	return 0;
}

void CSerialPortVjtMonoBlock::ClearCommandQueue()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::PopFirstCommandOffQueue()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::GetSoftwareVersion()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::GetHardwareVersion()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::GetWebServerVersion()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::GetModelNumber()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::CompleteInitialization()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::GetTemperature()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::GetVoltageScale()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::GetCurrentScale()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::EnableWatchDog(bool TempEnable)
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::TickleWatchDog()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::DisableWatchDogTimer()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::InitializeDXM()
{// Overridden from Spellman base for safety.
}

void CSerialPortVjtMonoBlock::RemoveMessageFromBuffer() 
{
	WORD TempCount = ReceiveDataCount();
//ReportErrorMessage(vComPortString + " VJT, remove count: " + dtoa(TempCount, 0), cEMailInspx, 32000);
	if (TempCount)
	{
		if (vReceiveDataBuffer[vReceiveDataTail] == STX)
		{  //have STX, so remove message
			for (WORD TempLoop = 0; TempLoop < TempCount; TempLoop++)
			{
				WORD TempIndex = (vReceiveDataTail) % cReceiveBufferSize; 
				if (vReceiveDataBuffer[TempIndex] == CR)
					TempLoop = TempCount;

//ReportErrorMessage(vComPortString + " VJT, remove: " + ByteToHex(vReceiveDataBuffer[vReceiveDataTail]), cEMailInspx, 32000);
				vReceiveDataBuffer[vReceiveDataTail] = 0;  //clear the buffer so don't confuse old ETX

				if (vReceiveDataTail < cReceiveBufferSize - 1)
					vReceiveDataTail++;
				else
					vReceiveDataTail = 0;

				if (vReceiveDataTail == vReceiveDataHead)
					TempLoop = TempCount;
			}
		}
		else
		{  //don't have STX, so remove one byte
			ReportErrorMessage(vComPortString + " byte not STX, remove: " + ByteToHex(vReceiveDataBuffer[vReceiveDataTail]), cEMailInspx, 32000);
			vReceiveDataBuffer[vReceiveDataTail] = 0;  //clear the buffer so don't confuse old ETX
			if (vReceiveDataTail < cReceiveBufferSize - 1)
				vReceiveDataTail++;
			else
				vReceiveDataTail = 0;
		}
	}
}

WORD CSerialPortVjtMonoBlock::ReceiveDataCount()
{
	return CSerialPortBase::ReceiveDataCount();
	//WORD TempCount = CSerialPortBase::ReceiveDataCount();
	//WORD TempFinalCount = TempCount;
	//if (TempCount)
	//{
	//	for (WORD TempLoop = 0; TempLoop < TempCount; TempLoop++)
	//	{
	//		WORD TempIndex = (vReceiveDataTail + TempLoop) % cReceiveBufferSize; 
	//		if (vReceiveDataBuffer[TempIndex] == CR)
	//		{
	//			TempFinalCount = TempLoop + 1;
	//			TempLoop = TempCount;
	//		}
	//	}
	//}
	//return TempFinalCount;
}

void CSerialPortVjtMonoBlock::PutCommandOnQueue(Constants TempCommand)
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("com3 VJT put command on queue: " + dtoa(TempCommand, 0), cDebugMessage);

	if ((vPendingCommand[0] == NO_COMMAND) && (vWaitingForReply))
	{
		vPendingCommand[0] = TempCommand;
		SendVjtCommand();
	}
	else
	for (BYTE TempLoop = 0; TempLoop < 10; TempLoop++)
	{
		vSyncSect.Lock();
		if (vPendingCommand[TempLoop] == NO_COMMAND)
		{
			vPendingCommand[TempLoop] = TempCommand;
			TempLoop = 10;
		}
		vSyncSect.Unlock();
	}
}

void CSerialPortVjtMonoBlock::RemoveCommandFromQueue()
{
	vSyncSect.Lock();
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("com3 VJT remove command from queue: " + dtoa(vPendingCommand[0], 0), cDebugMessage);

	vPendingCommand[0] = NO_COMMAND;

	if (vPendingCommand[1] != NO_COMMAND) //move commands down queue if just sent commad
	for (BYTE TempLoop = 0; TempLoop < 9; TempLoop++)
	{
		vPendingCommand[TempLoop] = vPendingCommand[TempLoop + 1];
		if (vPendingCommand[TempLoop + 1] == NO_COMMAND)
			TempLoop = 10;
	}
	vPendingCommand[9] = NO_COMMAND;
	vSyncSect.Unlock();
}
