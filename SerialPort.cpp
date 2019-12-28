//ScanTrac Side View Source File
// SerialPort.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "SerialPortReview.h"
#include "SerialPort.h"
#include "SelectItemDialog.h"
#include "SystemConfigurationData.h"
//#include "YesNoDialog.h" //can't ask users questions in this thread
#include "math.h"  //for sqrt
#include "ImageAquisitionThread.h"
#include "SpellmanDXMXRaySourcePort.h"
#include "XRaySource.h"
#include "SerialPortGuardMaster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef CompileInHTTPClientOperationsGuardian
	extern CString vGlobalOGLocationString;
	extern CString vGlobalOGLimitString;
	extern WORD vGlobalOGErrorCode;
#endif
extern CSerialPortGuardMaster *vGlobalGuardMaster;
extern 	unsigned int vGlobalTrackSensors;
extern BYTE vGlobalEjectorConfirmMask[cNumberOfEjectors];
extern BYTE vGlobalScanTracType;
extern BYTE vGlobalInAutoSize;
extern bool vGlobalFPGAVersion15Point31OrHigher;
extern bool vGlobalFPGAVersion15Point34OrHigher;
extern bool vGlobalFPGAVersion15Point2OrHigher;
extern bool vGlobalFPGAVersion15point42orAbove;
extern bool vGlobalFPGAVersion16point0orAbove;
extern bool vGlobalFPGAVersion16point7orAbove;
extern bool vGlobalFPGAVersion17point0orAbove;
extern bool vGlobalFPGAVersion18point0orAbove;
extern bool vGlobalFPGAVersion11Point0OrHigher;
extern bool vGlobalFPGAVersion10Point87OrHigher;
extern bool vGlobalFPGAVersion10Point0OrHigher;
extern CProduct *vGlobalCurrentProduct;
extern CImageAquisitionThread *vGlobalImageAquisitionThread;
extern CXRaySource *vGlobalMonoBlockXRayPort;
extern CSpellmanDXMXRaySourcePort *vGlobalDXMXRayPort;
extern double vGlobalPixelsPerUnit;
extern int vGlobalSerialPortNumberMicroController;
extern int vGlobalFirstMicroControllerCommCheck;
//extern CCriticalSection vGlobalSerialPortSendLock;
/////////////////////////////////////////////////////////////////////////////
// CSerialPort

IMPLEMENT_DYNCREATE(CSerialPort, CWinThread)

CSerialPort::CSerialPort()
{
	//vGaveBodyTriggerActiveTooLongError = false;
	//vBeltPositionBodyTriggerWentActive = 0;
	vGotBodyTriggerActiveMessage = false;
	vPreviousSeamerObjectPulseTime = 0;
	vNumberOfExternalDetectors = 0;
	vWaitingForContainerTriggerEnd = false;
	vBeltPositionToEnableTriggerAfterTemporaryDisable = 0;
	vBeltPositionDisabledTrigger = 0;
	vMainWindowPointer = NULL;
	vLastRecievedDataTime = 0;
	vOldSimulatingFastestEncoder = false;
	vGaveEncoderWarning = false;
	vComPortString = "";
	vPreviousAnalogChannelReadMSB = 0;
	vOldActualEncoderRate = 0;
	vReceiveCount = 0;
	vSendCount = 0;
	vSendingContinuous = false;
	vPreviousContainer = NULL;
	vCurrentContainer = NULL;

//	vComPortString = "";	//set above
	vComPortNumber = vGlobalSerialPortNumberMicroController;
	vPortOpen = false;
	vReadPortDataError = false;
	vReceiveDataHead = 0; 
	vReceiveDataTail = 0;
	vLostMessageCounter = 0;
	m_bAutoDelete = true;
	vLastCommTestStatusOK = true;

	//initialize Discover Mode Variables to zero
	for (BYTE TempLoop = 0; TempLoop < cNumberOfDiscoverSamples; TempLoop++)
	{
		vDiscoverModeDataIsActive[TempLoop] = false;
		vDiscoverModeDataBeltPosition[TempLoop] = 0;
	}
	vDiscoverModeDataIndex = 0;
	vDiscoverModeDataOldIndex = 0;
	vDiscoverModeOn = false;
	vLastDiscoverModeBits = 0xFF;

	vFlashingLights = false;
	SyncFromMicroControllerSeen = 0;
	ReportComNotDefaultDone = false;
}

CSerialPort::~CSerialPort()
{
}

BOOL CSerialPort::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return true;
}

int CSerialPort::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSerialPort, CWinThread)
	//{{AFX_MSG_MAP(CSerialPort)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialPort message handlers
void CSerialPort::RemoveMessageFromBuffer() 
{
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
}

WORD CSerialPort::ReceiveDataCount()
{
	if (vReceiveDataHead == vReceiveDataTail)
		return 0;
	else
	if (vReceiveDataHead > vReceiveDataTail)
		return vReceiveDataHead - vReceiveDataTail;
	else
		return cReceiveBufferSize - vReceiveDataTail + vReceiveDataHead;
}

void CSerialPort::ClearReceiver() 
{
  if (vPortOpen)
	{
		PurgeComm(vWinPortHandle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	}
}

void CSerialPort::ClearReceiveBuffer() 
{
	vReceiveDataTail = vReceiveDataHead;
}

void CSerialPort::ClearPort() 
{
	PurgeComm(vWinPortHandle, PURGE_RXABORT | PURGE_RXCLEAR);
}
void CSerialPort::ShowSerialData()  //data received
{
	if ((vLocalSystemData->vLogFile.vLogSerialData) ||
		(vLocalSystemData->vLogFile.vShowingSerialDataOnScreen))
	{
		WORD TempLength = ReceiveDataCount();
		if (TempLength > 5)
			TempLength = 5;

		CString TempString = "";
		TempString = "";
		BYTE TempByte = 0;
		if (TempLength>0)
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
				TempIndex++;
				if (TempIndex >= cReceiveBufferSize)
					TempIndex = 0;
			}
			//if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile(TempString,cInputMessage);
		}
	}
}

BOOL CSerialPort::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == cSerialReceivedDataMessage)
	{
		ProcessSerialMessage();
		return true;
	}
	else
	if (pMsg->message == cSerialInputOverflowMessage)
	{
		ReportErrorMessage("Error " + vComPortString + " Serial Input Overflow A, Bytes: " + dtoa(pMsg->wParam,0) + ", port: " + dtoa(pMsg->lParam, 0), cEMailInspx, 32000);

		return true;
	}

	return CWinThread::PreTranslateMessage(pMsg);
}

void CSerialPort::ProcessSerialMessage()
{
	while (SerialMessageComplete())
	{
		//log and show data on diagnostics screen if should
		ShowSerialData();
		//decode message
		DecodeMessage();

		RemoveMessageFromBuffer();

		double TempTime = CIFCOS::GetSystimeMillisecs();
		if (TempTime > vLastRecievedDataTime + 1000)
		if (vLastRecievedDataTime)
		if (vMainWindowPointer)
		if (!vMainWindowPointer->vDisplayingVersion)
		if (!vLocalConfigurationData->vSimulateLightControlBoard)
		if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
		{
			ReportErrorMessage("Error > 1 Second gap in recieved data",cEMailInspx, 32000);

			if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
			if (vLocalSystemData->vContainerCollection->GetCount()) //if  containers in tunnel
			if (!vBeltPositionToEnableTriggerAfterTemporaryDisable)
				TemporarilyDisableContainerTrigger();
		}
		else
			ReportErrorMessage("Error > 1 Second gap in recieved data",cWriteToLog,0);
		vLastRecievedDataTime = TempTime;
	}
}

void CSerialPort::DecodeMessage()
{
	double TempEndTime = 0;
	double TempStartTime = 0;
	WORD TempWatchDog = 0;
	BYTE TempBuffer[4];
	WORD TempSum1,TempSum2,TempSum3, TempWord;
	int TempSum;
	int TempNumberOfSamples = 0;
	CString TempString1("");
	CString TempString = "";
	TempString = "";
	WORD TempIndex = vReceiveDataTail;

	vHasReceivedCommunication = true;

	for (BYTE TempLoop = 0; TempLoop < 4; TempLoop++)
	{
		TempBuffer[TempLoop] = vReceiveDataBuffer[TempIndex];
		TempIndex++;
		if (TempIndex >= cReceiveBufferSize)
			TempIndex = 0;
	}
	WORD TempBeltPosition = TempBuffer[1];
	TempBeltPosition = (TempBeltPosition << 8) + TempBuffer[2];
//	CString TempErrorString("");
	double TempTime;
	WORD TempPreviousInterlockValue;
	switch (TempBuffer[0])
	{
		case 0x21: //eject Notification
			if (vLocalSystemData->vOutstandingEjects)
				vLocalSystemData->vOutstandingEjects--;

			ProcessEjectNotification(TempBeltPosition, TempBuffer[3]);
			ProcessBeltPosition(TempBeltPosition,false);

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("Got Eject Notification at: " + dtoa(TempBeltPosition,0), cDebugMessage);
			//ReportErrorMessage("Got Eject Notification at: " + dtoa(TempBeltPosition,0),cWriteToLog,0);
		break;
		case 0x22: //Change in Digital Line State 1
			//uController does not send Conveyor position for this change on digital line
			if ((TempBuffer[2] == 0) && (TempBuffer[3] == 0))  //ignore for version 18 board as after reboot it outputs one with all zeros
				ReportErrorMessage("Interlock Status 22 is all zeros, It is ignored", cWriteToLog, 0);
			else
			{
				TempPreviousInterlockValue = vLocalSystemData->vPreviousInterlockValue;
				vLocalSystemData->vPreviousInterlockValue = vLocalSystemData->vSourceInterLocks;

				if (!vLocalConfigurationData->vSimulateLightControlBoard)
					vLocalSystemData->vRawInterlockInput = (TempBuffer[2] << 8) + TempBuffer[3];

				if ((vGlobalGuardMaster) && (vGlobalGuardMaster->vDetectedValidGuardMasterDevice))  //using Guard Master for Interlocks, so don't get interlocks from uC board
				{//still read coolant flow, light stack from uC board
					vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks & 0x1B00) | (vLocalSystemData->vRawInterlockInput & 0xE4FF);
				}
				else
				{ 
					vLocalSystemData->vSourceInterLocks = vLocalSystemData->vRawInterlockInput;
				}

				//set coolant flow OK since 50 watt and TDs have no coolant 
				if ((vLocalConfigurationData->vScanTracType == cBriosoScanTrac) || (vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || 
					(vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) || (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) ||
					(vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac))
				if (!vLocalConfigurationData->vUseExternalInterlock)
					vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x0400;

				if ((!vLocalSystemData->vFPGAVersion9Point1OrHigher) || (vGlobalFPGAVersion10Point0OrHigher))
				{
					vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x0040;//set power indicator as set as wire is not connected before 9.1 or after 10.0
				}
				if (!vLocalSystemData->vFPGAVersion8Point7OrHigher)
				{
					vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x0060;//set spellman interlock and power as set as wire is not connected before 8.7
				}

				//upstreamsignal
				if (vLocalConfigurationData->vUseDownStreamSignalThatStopsConveyor == cUseDownStream) //if not using down stream signal, set active
				{
					WORD TempOldDownStream = vLocalSystemData->vPreviousInterlockValue & 0x2000;
					WORD TempCurrentDownStream = vLocalSystemData->vSourceInterLocks & 0x2000;

					if (TempCurrentDownStream)
						vLocalSystemData->vDownStreamSignalSaysConveyorOn = true;
					else
						vLocalSystemData->vDownStreamSignalSaysConveyorOn = false;

					if (TempOldDownStream != TempCurrentDownStream)
					{
						//11/18/2009
						if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))
						if (TempCurrentDownStream)  //conveyor starting, turn on simulated encoder
							::PostThreadMessage(vGlobalCallingThreadID,cDownStreamSignalChangedMessage,1,1);
						else
							::PostThreadMessage(vGlobalCallingThreadID,cDownStreamSignalChangedMessage,0,0);
					}
				}
				else
				{
					//pipe flow on input bit not used by software, only used by FPGA
					vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks | 0x2000);
				}

				if (vGlobalDXMXRayPort)
				{
					if (vGlobalDXMXRayPort->vInterLockStatus)
						vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks | 0x0020);//set bit 6, the x-ray interlock bit
					else
						vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks & 0xFFDF); //clear bit 6, the x-ray interlock bit

					vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x0010;  //set Spellman XLF fault to OK
				}
				else
				if (vGlobalMonoBlockXRayPort)
				{
					if (vGlobalMonoBlockXRayPort->vInterLockStatus)
						vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks | 0x0020);//set bit 6, the x-ray interlock bit
					else
						vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks & 0xFFDF); //clear bit 6, the x-ray interlock bit

					vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x0010;  //set Spellman XLF fault to OK

				}
				//added for KJH Office Testing
				else
				if (vLocalConfigurationData->vHVPSType == cXLFHVPS)
					vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x0020;  //Spellman XLF so set interlock OK

				::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("Interlock Changed: " + ByteToHex((BYTE)(vLocalSystemData->vSourceInterLocks >> 8)) + ", " + 
						ByteToHex((BYTE)(vLocalSystemData->vSourceInterLocks)),cDebugMessage);

				TempTime = CIFCOS::GetSystimeMillisecs();

				if (TempPreviousInterlockValue != vLocalSystemData->vPreviousInterlockValue)
				if ((TempTime < vLocalSystemData->vLastInterlockChangedTime + 200) && 
					(TempTime + 200 > vLocalSystemData->vLastInterlockChangedTime))  //if < 200 milliseconds since interlocks changed
				{
					if (TempPreviousInterlockValue == vLocalSystemData->vSourceInterLocks)  //if just changed back to what it was
					{
						vLocalSystemData->vInterlock200mSecChangeCount++;
						if (vLocalSystemData->vInterlock200mSecChangeCount == 1)
							ReportErrorMessage("Interlock Changed Less Than 200mSec: " + 
								ByteToHex((BYTE)(vLocalSystemData->vPreviousInterlockValue >> 8)) + ", " + 
								ByteToHex((BYTE)(vLocalSystemData->vPreviousInterlockValue)) + " -> " + 
								ByteToHex((BYTE)(vLocalSystemData->vSourceInterLocks >> 8)) + ", " + 
								ByteToHex((BYTE)(vLocalSystemData->vSourceInterLocks)), cWriteToLog,0);
					}
				}

				vLocalSystemData->vLastInterlockChangedTime = TempTime;

				if (vLocalSystemData->vSourceInterLocks & 0x8000) //Encoder Error Indicator
				{
					if (vGlobalFPGAVersion15point42orAbove)
					if (!vLocalSystemData->vGaveEncoderErrorThisRun)
					if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)  
					if ((!vLocalConfigurationData->vContinuousFeedEncoderRate) && (!vLocalConfigurationData->vContinuousFeedContainerRate))
					if (!vLocalSystemData->vFPGASimulateEncoderRate)
					if (vLocalSystemData->vActualEncoderRate < vLocalSystemData->vMaximumEncoderRate)
					if (vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
					{
						ReportErrorMessage("Encoder Faulty Indicator From FPGA", cWriteToLog, 0);
						if (vLocalSystemData->vEncoderFaultyCount < 250)
							vLocalSystemData->vEncoderFaultyCount++;

						if (vLocalSystemData->vEncoderFaultyCount > 40)
						{
							ReportErrorMessage("Encoder Faulty - Irregular Pulses - Replace Encoder", cEMailMaintenance, 0);
							ReportErrorMessage("Encoder Faulty - Irregular Pulses - Replace Encoder", cEMailInspx, 0);
							vLocalSystemData->vDisplayingAnEncoderError = true;
							vLocalSystemData->vGaveEncoderErrorThisRun = true;
							if (vMainWindowPointer)
							{
								vMainWindowPointer->SetInterlockStatus();
								vMainWindowPointer->UpdateClearYellowMessageButton();
							}
						}
					}
				}
				if (vLocalSystemData->vPreviousInterlockValue != vLocalSystemData->vSourceInterLocks)
				{
					if (!((vGlobalGuardMaster) && (vGlobalGuardMaster->vDetectedValidGuardMasterDevice)))  //NOT using Guard Master for Interlocks, so use interlocks from uC board
					{
						if (((vLocalSystemData->vPreviousInterlockValue & 0x1000) == 0x1000) && ((vLocalSystemData->vSourceInterLocks & 0x1000) == 0x0000))
						{
							if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
								(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
								ReportErrorMessage("Cabinet/conveyor Door Open", cUserAction, 0); 
							else
								ReportErrorMessage("Front/Rear Door Open", cUserAction, 0); 
						}
						if (((vLocalSystemData->vPreviousInterlockValue & 0x1000) == 0x0000) && ((vLocalSystemData->vSourceInterLocks & 0x1000) == 0x1000))
						{
							if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
								(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
								ReportErrorMessage("Cabinet and conveyor Door Closed", cUserAction, 0); 
							else
								ReportErrorMessage("Front and Rear Door Closed", cUserAction, 0); 
						}

						if (((vLocalSystemData->vPreviousInterlockValue & 0x0800) == 0x0800) && ((vLocalSystemData->vSourceInterLocks & 0x0800) == 0x0000))
						{
							if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
								(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
								ReportErrorMessage("Conveyor Side Door(s) Open", cUserAction, 0); 
							else
								ReportErrorMessage("Conveyor Side Cover(s) Open", cUserAction, 0); 
						}
						if (((vLocalSystemData->vPreviousInterlockValue & 0x0800) == 0x0000) && ((vLocalSystemData->vSourceInterLocks & 0x0800) == 0x0800))
						{
							if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
								(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
								ReportErrorMessage("Conveyor Side Doors Closed", cUserAction, 0); 
							else
								ReportErrorMessage("Conveyor Side Covers Closed", cUserAction, 0); 
						}

						if (((vLocalSystemData->vPreviousInterlockValue & 0x0200) == 0x0200) && ((vLocalSystemData->vSourceInterLocks & 0x0200) == 0x0000))
						{
								ReportErrorMessage("E-Stop pushed in - stopped", cUserAction, 0); 
						}
						if (((vLocalSystemData->vPreviousInterlockValue & 0x0200) == 0x0000) && ((vLocalSystemData->vSourceInterLocks & 0x0200) == 0x0200))
						{
								ReportErrorMessage("E-Stop pulled out - OK", cUserAction, 0); 
						}

						if (((vLocalSystemData->vPreviousInterlockValue & 0x0100) == 0x0100) && ((vLocalSystemData->vSourceInterLocks & 0x0100) == 0x0000))
						{
								ReportErrorMessage("Key Switch Turned Off", cUserAction, 0); 
						}
						if (((vLocalSystemData->vPreviousInterlockValue & 0x0100) == 0x0000) && ((vLocalSystemData->vSourceInterLocks & 0x0100) == 0x0100))
						{
								ReportErrorMessage("Key Switch Turned On", cUserAction, 0); 
						}
					}
				}


				//b	rh test
				//ReportErrorMessage("Interlock Changed: " + ByteToHex((BYTE)(vLocalSystemData->vSourceInterLocks >> 8)) + ", " + 
				//		ByteToHex((BYTE)(vLocalSystemData->vSourceInterLocks)),cError,0);
				CheckInterlocks(true);
			}

		break;
		case 0x23: //Disable Auxiliary Detector bit
							//Auxiliary Detector input 1
							//Auxiliary Detector input 2
							//Auxiliary Detector input 3
							//Auxiliary Detector input 4
							//Auxiliary Detector input 5
							//Eject Confirmation 1
							//Eject Confirmation 2
							//Eject Confirmation 3      (LSB)
			TempStartTime = CIFCOS::GetSystimeMicrosecs();

			vLocalSystemData->vRawExternalDetectorInput = TempBuffer[3];
			HandleExternalDetectorLineChange(TempBeltPosition, vLocalSystemData->vRawExternalDetectorInput);
			if (vLocalSystemData->vKeepExternalDetectorsEnabled)
				ProcessBeltPosition(TempBeltPosition,false);

			TempEndTime = CIFCOS::GetSystimeMicrosecs();
			TempEndTime = TempEndTime - TempStartTime;
			if (vLocalSystemData->vMaximumExternalDetectorTime < TempEndTime)
				vLocalSystemData->vMaximumExternalDetectorTime = TempEndTime;
		break;
		case 0x24: //Change in Digital Line State 3
			//got a Container trigger change
							//Container Trigger 1               (MSB)
							//Container Trigger 2
							//Container Trigger 3
							//Container Trigger 4
							//Container Trigger 5
							//Spare 1 
							//Spare 2 
							//Spare 3 
			//need to calculate the Conveyor rate if don't have one.
			ProcessBeltPosition(TempBeltPosition,!vLocalSystemData->vActualEncoderRate);
			vLocalSystemData->vRawBodyTriggerInput = TempBuffer[3];
			/*
			if ((vLocalSystemData->vActualEncoderRate == 0) &&
				//don't report if first few containers as conveyor starting up
				(vLocalSystemData->vContainerRate > 10))
			{
				if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
				if (vLocalSystemData->vSourceRampedUp)
					ReportErrorMessage("Got Container Trigger But No Encoder, Check Encoder",cError,0);
			}
			else
			*/
			{
				HandleBodyTriggerLineChange(TempBeltPosition,TempBuffer[3]);

				if (vLocalSystemData->vBackupBodyTriggerMask)  //if configured, then check it
					HandleBackupBodyTriggerChange(TempBeltPosition,TempBuffer[3]);
			}
		break;
		case 0x25: //Ended Image Data Frame at Conveyor Position
			ProcessBeltPosition(TempBeltPosition,false);
			/*
			if (vLocalSystemData->vKeepExternalDetectorsEnabled)
				ReportErrorMessage("Ended taking image at: " +
				dtoa(TempBeltPosition,0) + ", " +
				dtoa((TempBeltPosition - vLocalSystemData->vLastBodyTriggerPosition) /
				vGlobalPixelsPerUnit,2) + " " +
				vLocalConfigurationData->vUnitsString,cAction,0);
				*/
			//if (vLocalSystemData->vContainerBeingImaged)
			if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
			if (vLocalSystemData->vSystemRunMode != cCalibratingDetectorOffsetMode)
			{
				vLocalSystemData->vEndImageCounter++;
				WriteToLogFileWithBeltPosition("*Got uC End Image Message: " + 
					dtoa(vLocalSystemData->vEndImageCounter,0),0);
				//vLocalSystemData->vContainerBeingImaged->vGotEndImageNotice = true;
			}
		break;
		case 0x26: //Micro controller ready to run
			vLocalSystemData->vUControllerReady = true;
			//if operator pressed run before it was ready, and is waiting, then run
			if (vLocalSystemData->vWaitingForUControllerReady)
			{
				::PostThreadMessage(vGlobalCallingThreadID,cuControllerReadyToRunMessage,0,0);
			}
		break;
		case 0x27: //body trigger turned off
			ProcessBeltPosition(TempBeltPosition,false);
			if (vLocalSystemData->vKeepExternalDetectorsEnabled)
				ReportErrorMessage("Container Trigger ended at: " + dtoa(TempBeltPosition,0) + ", " +
				dtoa((TempBeltPosition - vLocalSystemData->vLastBodyTriggerPosition) / vGlobalPixelsPerUnit,2) + " " + vLocalConfigurationData->vUnitsString,cAction,0);
			vLocalSystemData->vRawBodyTriggerInput = TempBuffer[3];
			HandleBodyTriggerEnd(TempBeltPosition,TempBuffer[3]);
			if (vLocalSystemData->vBackupBodyTriggerMask)  //if configured, then check it
				HandleBackupBodyTriggerChange(TempBeltPosition,TempBuffer[3]);
		break;
		case 0x28: //Started Image Data Frame at Conveyor Position
			ProcessBeltPosition(TempBeltPosition,false);
			if (vLocalSystemData->vKeepExternalDetectorsEnabled)
				ReportErrorMessage("Started taking image at: " + dtoa(TempBeltPosition,0) + ", " +
				dtoa((TempBeltPosition - vLocalSystemData->vLastBodyTriggerPosition) / vGlobalPixelsPerUnit,2) + " " + vLocalConfigurationData->vUnitsString,cAction,0);
			//if (vLocalSystemData->vLogFile.vLogSerialData)
			//if (vLocalSystemData->vContainerBeingImaged != NULL)
			//{
			//	vLocalSystemData->vLogFile.WriteToLogFile(
			//		"Start of Image - Have Not Ended Previous Image ", cDebugMessage);
			//}
			if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
			if (vLocalSystemData->vSystemRunMode != cCalibratingDetectorOffsetMode)
			{
				vLocalSystemData->vStartImageCounter++;
				WriteToLogFileWithBeltPosition("*Got uC Start Image Message: " + 
					dtoa(vLocalSystemData->vStartImageCounter,0),0);

				if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
				{
					if (vLocalSystemData->vContainerCollection->GetCount() > 0)
					{
					}
					else
					if (vLocalSystemData->vLogFile.vLogSerialData)
					if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
					if ((vGlobalCurrentProduct) && (!vGlobalCurrentProduct->vMultiLaneNumberOfLanes))
						WriteToLogFileWithBeltPosition("Start of Image but no containers", TempBeltPosition);
					vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
				}
				else
					ReportErrorMessage("Error-Serial Thread Lock Failed 1", cEMailInspx,32000);
			}
		break;
		case 0x29: //Disable Auxiliary Detector bit
							//Auxiliary Detector input 1
							//Auxiliary Detector input 2
							//Auxiliary Detector input 3
							//Auxiliary Detector input 4
							//Auxiliary Detector input 5
							//Eject Confirmation 1
							//Eject Confirmation 2
							//Eject Confirmation 3      (LSB)
			TempStartTime = CIFCOS::GetSystimeMicrosecs();

			if (vLocalSystemData->vOutstandingAuxiliaryDetectorEnables)
				vLocalSystemData->vOutstandingAuxiliaryDetectorEnables--;
			vLocalSystemData->vRawExternalDetectorInput = TempBuffer[3];

			HandleExternalDetectorLineChange(TempBeltPosition, vLocalSystemData->vRawExternalDetectorInput);
			TestContainersForEject(TempBeltPosition);

			if (vLocalSystemData->vKeepExternalDetectorsEnabled)
				ProcessBeltPosition(TempBeltPosition,false);

			TempEndTime = CIFCOS::GetSystimeMicrosecs();
			TempEndTime = TempEndTime - TempStartTime;
			if (vLocalSystemData->vMaximumExternalDetectorTime < TempEndTime)
				vLocalSystemData->vMaximumExternalDetectorTime = TempEndTime;
		break;
		case 0x2A: 
		case 0x2B: //Got an eject notice from a resynchornized ejector
			if (vLocalConfigurationData->vResynchronizeEjectors)  //reset the container count
			{
				BYTE TempEjectorIndex = 0;
				if (TempBuffer[0] == 0x2B)
					TempEjectorIndex = 1;

				if (vLocalSystemData->vOutstandingEjects)
					vLocalSystemData->vOutstandingEjects--;

				ProcessResynchronizedEjectNotification(TempBeltPosition, TempBuffer[3]);
				ProcessBeltPosition(TempBeltPosition,false);

				if (vLocalSystemData->vInAutoSetup)
					CalculateDistanceToFirstSensor(TempBeltPosition, TempBuffer[3], TempEjectorIndex);
			
				if (vLocalSystemData->vLogFile.vLogSerialData)
				if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
					WriteToLogFileWithBeltPosition("Resynchronize Eject sensor " + dtoa(TempEjectorIndex + 1, 0) + " counted Ejected container at: " + dtoa(TempBeltPosition,0) + ", cen: " + dtoa(TempBuffer[3], 0), vLocalSystemData->vCurrentBeltPosition);
				else
					WriteToLogFileWithBeltPosition("Resynchronize Eject sensor " + dtoa(TempEjectorIndex + 1, 0) + " count at: " + dtoa(TempBeltPosition,0) + ", cen: " + dtoa(TempBuffer[3], 0), vLocalSystemData->vCurrentBeltPosition);

				vLocalSystemData->vResynchronizeEjectorsCurrentEjectorSensorCount[TempEjectorIndex] = TempBuffer[3];

				if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
				if ((vLocalSystemData->vResynchronizeEjectorsCurrentEjectorSensorCount[TempEjectorIndex] > vLocalSystemData->vResynchronizeEjectorsCurrentTriggerCount) && 
				(!((vLocalSystemData->vResynchronizeEjectorsCurrentEjectorSensorCount[TempEjectorIndex] > 200) && (vLocalSystemData->vResynchronizeEjectorsCurrentTriggerCount < 50)))) // did not just wrap around
				{
					//reset counts if nothing has passed the body trigger yet
					WORD TempNumberOfContainers = vLocalSystemData->vContainerCollection->GetCount();
					if (TempNumberOfContainers == 0)
					{
						ClearEjectorSynchronizationCount();
						if (vLocalSystemData->vLogFile.vLogSerialData)
							WriteToLogFileWithBeltPosition("Reset Resynchronize Eject counters because got ejector sensor pulse, but no containers should be in tunnel", vLocalSystemData->vCurrentBeltPosition);
					}
					else
					{
						if (((TempEjectorIndex == 0) && (vLocalSystemData->vEjectorUsed & 1)) || ((TempEjectorIndex == 1) && (vLocalSystemData->vEjectorUsed & 6))) //if using this ejector on this product
						{
							ReportErrorMessage("Critical error SCE-Resynchronized Ejector is off.\nThe ScanTrac needs a 10 foot gap to clear resynchronizing ejectors.\nTrigger: " + 
								dtoa(vLocalSystemData->vResynchronizeEjectorsCurrentTriggerCount, 0) + " Sensor " + dtoa(TempEjectorIndex + 1, 0) + ": " + 
								dtoa(vLocalSystemData->vResynchronizeEjectorsCurrentEjectorSensorCount[TempEjectorIndex], 0), cCritical, 0);

							ReportErrorMessage("Critical error SCE-Ejector requires a 10 foot gap to synchronize", cError, 0);
						}
					}
				}
				else
				if (vLocalSystemData->vFirstContainerShouldNotTriggerSensorYet)
				{
					//reset counts if nothing has passed the body trigger yet
					WORD TempNumberOfContainers = vLocalSystemData->vContainerCollection->GetCount();
					if (TempNumberOfContainers == 0)
					{
						ClearEjectorSynchronizationCount();
						if (vLocalSystemData->vLogFile.vLogSerialData)
							WriteToLogFileWithBeltPosition("Clear Resynchronize Eject counters because got ejector sensor pulse, but no containers should be in tunnel", vLocalSystemData->vCurrentBeltPosition);
					}
					else
					{
						if (((TempEjectorIndex == 0) && (vLocalSystemData->vEjectorUsed & 1)) || ((TempEjectorIndex == 1) && (vLocalSystemData->vEjectorUsed & 6))) //if using this ejector on this product
						{
							ReportErrorMessage("Critical error PDE-Resynchronized Ejector is off.\nThe ScanTrac needs a 10 foot gap to clear resynchronizing ejectors.\nTrigger: " + 
								dtoa(vLocalSystemData->vResynchronizeEjectorsCurrentTriggerCount, 0) + " Sensor " + dtoa(TempEjectorIndex + 1, 0) + ": " + 
								dtoa(vLocalSystemData->vResynchronizeEjectorsCurrentEjectorSensorCount[TempEjectorIndex], 0), cCritical, 0);

							ReportErrorMessage("Critical error PDE-Ejector requires a 10 foot gap to synchronize", cError, 0);
						}
					}
				}
			}
		break;
		case 0x2F: //Got a reset confirm notice from a resynchornized ejector
			if (vLocalConfigurationData->vResynchronizeEjectors)  //reset the container count
			{
				if (vLocalSystemData->vLogFile.vLogSerialData)
					WriteToLogFileWithBeltPosition("uC confirm reset resynchronize ejector sensor. Old counts. 1: " + dtoa(TempBuffer[1],0) + ", 2: " + dtoa(TempBuffer[2], 0) + ", 3: " + dtoa(TempBuffer[3], 0), vLocalSystemData->vCurrentBeltPosition);
			}
		break;
		case 0x6A: 
		case 0x6B: //Got an from a resynchornized ejector trigger that it counted a container that did not need ejecting
			if (vLocalConfigurationData->vResynchronizeEjectors)
			{
				BYTE TempEjectorIndex = 0;
				if (TempBuffer[0] == 0x6B)
					TempEjectorIndex = 1;

				ProcessBeltPosition(TempBeltPosition,false);

				if (vLocalSystemData->vLogFile.vLogSerialData)
				if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
					WriteToLogFileWithBeltPosition("Resynchronize Eject sensor " + dtoa(TempEjectorIndex + 1, 0) + " counted good container at: " + dtoa(TempBeltPosition,0) + ", cen: " + dtoa(TempBuffer[3], 0), vLocalSystemData->vCurrentBeltPosition);
				else
					WriteToLogFileWithBeltPosition("Resynchronize Eject sensor " + dtoa(TempEjectorIndex + 1, 0) + " count at: " + dtoa(TempBeltPosition,0) + ", cen: " + dtoa(TempBuffer[3], 0), vLocalSystemData->vCurrentBeltPosition);

				if (vLocalSystemData->vInAutoSetup)
					CalculateDistanceToFirstSensor(TempBeltPosition, TempBuffer[3], TempEjectorIndex);

				vLocalSystemData->vResynchronizeEjectorsCurrentEjectorSensorCount[TempEjectorIndex] = TempBuffer[3];

				if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
				if ((vLocalSystemData->vResynchronizeEjectorsCurrentEjectorSensorCount[TempEjectorIndex] > vLocalSystemData->vResynchronizeEjectorsCurrentTriggerCount) && 
					(!((vLocalSystemData->vResynchronizeEjectorsCurrentEjectorSensorCount[TempEjectorIndex] > 200) && (vLocalSystemData->vResynchronizeEjectorsCurrentTriggerCount < 50)))) // did not just wrap around
				{
					//reset counts if nothing has passed the body trigger yet
					WORD TempNumberOfContainers = vLocalSystemData->vContainerCollection->GetCount();
					if (TempNumberOfContainers == 0)
					{
						ClearEjectorSynchronizationCount();
						if (vLocalSystemData->vLogFile.vLogSerialData)
							WriteToLogFileWithBeltPosition("Reset all Resynchronize Eject counters because got ejector sensor pulse, but no containers should be in tunnel", vLocalSystemData->vCurrentBeltPosition);
					}
					else
					{
						if (((TempEjectorIndex == 0) && (vLocalSystemData->vEjectorUsed & 1)) || ((TempEjectorIndex == 1) && (vLocalSystemData->vEjectorUsed & 6))) //if using this ejector on this product
						{
							if (vLocalSystemData->vResynchronizeEjectorsCurrentTriggerCount < vLocalSystemData->vResynchronizeEjectorsCurrentEjectorSensorCount[0])
								ReportErrorMessage("Resynchronize Ejector Error, Sensor " + dtoa(TempEjectorIndex + 1, 0) + " counted more than Trigger: " + dtoa(vLocalSystemData->vResynchronizeEjectorsCurrentTriggerCount, 0) + ", " +
									dtoa(vLocalSystemData->vResynchronizeEjectorsCurrentEjectorSensorCount[TempEjectorIndex], 0), cEMailInspx, 32000);

							ReportErrorMessage("Critical error SC-Resynchronized Ejector is off.\nThe ScanTrac needs a 10 foot gap to clear resynchronizing ejectors.\nTrigger: " + 
								dtoa(vLocalSystemData->vResynchronizeEjectorsCurrentTriggerCount, 0) + " Sensor " + dtoa(TempEjectorIndex + 1, 0) + ": " + 
								dtoa(vLocalSystemData->vResynchronizeEjectorsCurrentEjectorSensorCount[TempEjectorIndex], 0), cCritical, 0);

							ReportErrorMessage("Critical error SC-Ejector requires a 10 foot gap to synchronize", cError, 0);
						}
					}
				}
				else
				if (vLocalSystemData->vFirstContainerShouldNotTriggerSensorYet)
				{
					//reset counts if nothing has passed the body trigger yet
					WORD TempNumberOfContainers = vLocalSystemData->vContainerCollection->GetCount();
					if (TempNumberOfContainers == 0)
					{
						ClearEjectorSynchronizationCount();
						if (vLocalSystemData->vLogFile.vLogSerialData)
							WriteToLogFileWithBeltPosition("Clear all Resynchronize Eject counters because got ejector sensor pulse, but no containers should be in tunnel", vLocalSystemData->vCurrentBeltPosition);
					}
					else
					{
						if (((TempEjectorIndex == 0) && (vLocalSystemData->vEjectorUsed & 1)) || ((TempEjectorIndex == 1) && (vLocalSystemData->vEjectorUsed & 6))) //if using this ejector on this product
						{
							ReportErrorMessage("Critical error PD-Resynchronized Ejector is off.\nThe ScanTrac needs a 10 foot gap to clear resynchronizing ejectors.\nTrigger: " + 
								dtoa(vLocalSystemData->vResynchronizeEjectorsCurrentTriggerCount, 0) + " Sensor " + dtoa(TempEjectorIndex + 1, 0) + ": " + 
								dtoa(vLocalSystemData->vResynchronizeEjectorsCurrentEjectorSensorCount[TempEjectorIndex], 0), cCritical, 0);

							ReportErrorMessage("Critical error PD-Ejector requires a 10 foot gap to synchronize", cError, 0);
						}
					}
				}
			}
		break;
		case 0x30: //reply to request to read a certain address
			TempWord = (TempBuffer[2] << 8) + TempBuffer[3];
			
			if (TempBuffer[1] == 0x76) //only asked for on version 15.31 FPGA and higher
			if (vGlobalFPGAVersion15Point31OrHigher)
			{
				if (vLocalSystemData->vDACErrorCount != TempWord)
				{
					vLocalSystemData->vDACErrorCount = TempWord;
					if (vLocalSystemData->vDACErrorCount)
						ReportErrorMessage("DAC Errors: " + dtoa(vLocalSystemData->vDACErrorCount, 0), cEMailInspx, 32000);
				}
			}
			
			//if (vGlobalFPGAVersion15Point2OrHigher)
			{
				if (TempBuffer[1] == 0x78)
				{//got DAC Channel Back
					WORD TempReturnedValue = ((TempBuffer[2] << 8) + TempBuffer[3]) & 0x0FFF;

					//temp to cause error
					//if ((vLocalSystemData->vDACChannelWrote == 4) && (vLocalSystemData->vVerifyDACErros < 3))
					//	TempReturnedValue = 222;

					ReportErrorMessage("Verify DAC Detector " + dtoa(vLocalSystemData->vDACChannelWrote + 1, 0) +
						": " + dtoa(TempReturnedValue, 0) + ", Set: " + dtoa(vLocalSystemData->vLastProgrammedDACDetectorOffset[vLocalSystemData->vDACChannelWrote], 0), cWriteToLog, 0);

					if (vLocalSystemData->vVerifyDACValues)
					{
						vLocalSystemData->vLatestDACValueRead[vLocalSystemData->vDACChannelWrote] = TempReturnedValue;
						if (vLocalSystemData->vLogFile.vLogSerialData)
							vLocalSystemData->vLogFile.WriteToLogFile("Verify DAC Detector " + dtoa(vLocalSystemData->vDACChannelWrote + 1, 0) +
								": " + dtoa(TempReturnedValue, 0) + ", Set: " + dtoa(vLocalSystemData->vLastProgrammedDACDetectorOffset[vLocalSystemData->vDACChannelWrote], 0),cDebugMessage);

						if (vLocalSystemData->vNeedToVerifyDACValues)
						if (TempReturnedValue != vLocalSystemData->vLastProgrammedDACDetectorOffset[vLocalSystemData->vDACChannelWrote])
						{ //found DAC not correct value, so re-write it
							vLocalSystemData->vVerifyDACErros++;
							if (vLocalSystemData->vVerifyDACErros < 4)
							{
								//vMainWindowPointer->SendAllADCOffsets //Reprogram ADCs
								if (!vLocalSystemData->vGenerateDACVerifyReport)
								{
									ReportErrorMessage("GOT DAC VERIFY ERROR, REPROGRAMMING DACS: " + dtoa(vLocalSystemData->vVerifyDACErros, 0), cEMailInspx, 0);
									vMainWindowPointer->StopDACTest();
									vMainWindowPointer->SendAllDACDetectorOffsets(); //Reprogram DACS
								}
								else
									ReportErrorMessage("Got DAC Verify Error On Manual Test", cAction, 0);
							}
							else
							{
								if (!vLocalSystemData->vGenerateDACVerifyReport)
								{
									if (vGlobalFPGAVersion16point0orAbove)
										ReportErrorMessage("DAC Programming Error - Call Inspx", cEMailMaintenance, 0);
									ReportErrorMessage("GOT DAC VERIFY ERROR, Gave up trying to reprogram DACs: " + dtoa(vLocalSystemData->vVerifyDACErros, 0), cEMailInspx, 0);
								}
								else
									ReportErrorMessage("Got DAC Verify Error On Manual Verify", cAction, 0);
							}
						}
						else
						if ((vLocalSystemData->vDACChannelWrote == vLocalConfigurationData->vNumberOfXRayDetectors)) //finished verify, so turn off
						{
							vLocalSystemData->vNeedToVerifyDACValues = false;
							ReportErrorMessage("Finished DAC Programmed Value Verify, ALL OK", cWriteToLog, 0);
						}
					}
					else
					{
						if (TempReturnedValue == vLocalSystemData->vDACTestValueSent)
						{
							if (vLocalSystemData->vLogFile.vLogSerialData)
								vLocalSystemData->vLogFile.WriteToLogFile("DAC: " + dtoa(vLocalSystemData->vDACChannelWrote + 1, 0) + " Read OK, Value: " + 
									WordToHex(TempReturnedValue) + " (" + dtoa(TempReturnedValue, 0) + ")",cDebugMessage);

							ReportErrorMessage("DAC: " + dtoa(vLocalSystemData->vDACChannelWrote + 1, 0) + " Read OK, Value: " + 
								WordToHex(TempReturnedValue) + " (" + dtoa(TempReturnedValue, 0) + ")", cWriteToLog,0);
						}
						else
						{
							vLocalSystemData->vDACTestErrorCount++;
							if (vLocalSystemData->vLogFile.vLogSerialData)
								vLocalSystemData->vLogFile.WriteToLogFile("DAC: " + dtoa(vLocalSystemData->vDACChannelWrote + 1, 0) + " Read Failed, Expected: " + WordToHex(vLocalSystemData->vDACTestValueSent) + 
								" (" + dtoa(vLocalSystemData->vDACTestValueSent, 0) + ")" + " Got: " + WordToHex(TempReturnedValue) + " (" + dtoa(TempReturnedValue, 0) + ")",cDebugMessage);

							ReportErrorMessage("DAC: " + dtoa(vLocalSystemData->vDACChannelWrote + 1, 0) + " Read Failed, Expected: " + WordToHex(vLocalSystemData->vDACTestValueSent) + " (" + 
								dtoa(vLocalSystemData->vDACTestValueSent, 0) + ")" + " Got: " + WordToHex(TempReturnedValue) + " (" + dtoa(TempReturnedValue, 0) + ")", cEMailMaintenance,0);
						}
					}
					vLocalSystemData->vWaitingForDACValueReturn = false;
				}
				if (TempBuffer[1] == 0x72)
				{//got ADC Channel Back
					//WORD TempReturnedValue = (((TempBuffer[2] << 8) + TempBuffer[3]) & 0x00FF) << 2;//shift 2 bits to left as only get back 8 msBits of 10 bit value
					WORD TempReturnedValue = ((WORD)TempBuffer[3]);
					if (!vGlobalFPGAVersion18point0orAbove)
						TempReturnedValue = TempReturnedValue << 2;//shift 2 bits to left as only get back 8 msBits of 10 bit value

					if (vLocalSystemData->vVerifyADCValues)
					{
						vLocalSystemData->vLatestADCValueRead[vLocalSystemData->vADCChannelWrote] = TempReturnedValue; //shift 2 bits to the left as only get 8 msbits of 10 bits

						if (vLocalSystemData->vLogFile.vLogSerialData)
							vLocalSystemData->vLogFile.WriteToLogFile("Verify ADC Detector " + dtoa(vLocalSystemData->vADCChannelWrote + 1, 0) +
								": " + dtoa(TempReturnedValue, 0) + ", Set: " + dtoa(vLocalSystemData->vLastProgrammedADCDetectorOffset[vLocalSystemData->vADCChannelWrote], 0),cDebugMessage);

						ReportErrorMessage("Verify ADC Detector " + dtoa(vLocalSystemData->vADCChannelWrote + 1, 0) + ": " + dtoa(TempReturnedValue, 0) + ", Set: " + 
							dtoa(vLocalSystemData->vLastProgrammedADCDetectorOffset[vLocalSystemData->vADCChannelWrote], 0), cWriteToLog, 0);
					}
					else
					{
						if (TempReturnedValue == vLocalSystemData->vADCTestValueSent)
						{
							if (vLocalSystemData->vLogFile.vLogSerialData)
								vLocalSystemData->vLogFile.WriteToLogFile("ADC: " + dtoa(vLocalSystemData->vADCChannelWrote + 1, 0) + " Read OK, Value: " + 
								WordToHex(TempReturnedValue) + " (" + dtoa(TempReturnedValue, 0) + ")",cDebugMessage);

							ReportErrorMessage("ADC: " + dtoa(vLocalSystemData->vADCChannelWrote + 1, 0) + " Read OK, Value: " + 
								WordToHex(TempReturnedValue) + " (" + dtoa(TempReturnedValue, 0) + ")", cWriteToLog,0);
						}
						else
						{
							vLocalSystemData->vADCTestErrorCount++;
							if (vLocalSystemData->vLogFile.vLogSerialData)
								vLocalSystemData->vLogFile.WriteToLogFile("ADC: " + dtoa(vLocalSystemData->vADCChannelWrote + 1, 0) + " Read Failed, Expected: " + WordToHex(vLocalSystemData->vADCTestValueSent) + 
								" (" + dtoa(vLocalSystemData->vADCTestValueSent, 0) + ")" + " Got: " + WordToHex(TempReturnedValue) + " (" + dtoa(TempReturnedValue, 0) + ")",cDebugMessage);

							ReportErrorMessage("ADC: " + dtoa(vLocalSystemData->vADCChannelWrote + 1, 0) + " Read Failed, Expected: " + WordToHex(vLocalSystemData->vADCTestValueSent) + 
								" (" + dtoa(vLocalSystemData->vADCTestValueSent, 0) + ")" + " Got: " + WordToHex(TempReturnedValue) + " (" + dtoa(TempReturnedValue, 0) + ")", cEMailMaintenance,0);
						}
					}
					vLocalSystemData->vWaitingForADCValueReturn = false;
				}
			}
			if (vGlobalFPGAVersion10Point0OrHigher)
			{
				if (TempBuffer[1] == 0xFA)
				{//got analog status channel data back
					//must set MSB to MSB of previous read for hardware reading problem, then save this
					//MSB to put in next read
					if (vLocalSystemData->vuCBoardAnalogValueReadCount < 250)
						vLocalSystemData->vuCBoardAnalogValueReadCount++;

					WORD TempCurrentAnalogChannelReadMSB = TempWord & 0x8000;
					//clear this MSB, set previous MSB into this word
					TempWord = (TempWord & 0x7FFF) | vPreviousAnalogChannelReadMSB; 
					vPreviousAnalogChannelReadMSB = TempCurrentAnalogChannelReadMSB;
					if ((TempBuffer[2] == 0xFF) && (TempBuffer[3] == 0xFF) && (!vLocalConfigurationData->vSimulateLightControlBoard)) //don't care if can't read value if simulating, can get invalid code FFFF if no board.
					{
						if (vLocalSystemData->vLogFile.vLogSerialData)
							vLocalSystemData->vLogFile.WriteToLogFile("Analog Status Data Unavailable",cDebugMessage);
					}
					else
					{
						WORD TempCount = 0;
						WORD TempValue = TempWord & 0x0FFF;
						BYTE TempChannel = TempWord >> 12;
						if (vLocalSystemData->vLogFile.vLogSerialData)
						{
							vLocalSystemData->vLogFile.WriteToLogFile("Analog: " + dtoa(TempChannel,0) + " = "+ dtoa(TempValue,0),cDebugMessage);
						}
						double TempTemp = 0;
						if (vLocalSystemData->vuCBoardAnalogValueReadCount > 5)
						switch (TempChannel)
						{
							case 0: //+5V monitor
								if (vLocalSystemData->vuCPoweredUp)
								if (vLocalSystemData->vNumberOfDataSamples < cNumberOfVoltageSamples)
									vLocalSystemData->vNumberOfDataSamples++;

								if (vLocalSystemData->vDataIndex > cNumberOfVoltageSamples - 2)
								{
									vLocalSystemData->vDataIndex = 0;
									vLocalSystemData->vuCPoweredUp = true;
								}
								else
									vLocalSystemData->vDataIndex++;

								vLocalSystemData->v5 = TempValue;

								if (vLocalSystemData->vuCPoweredUp)
								if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
								{
									if (vLocalSystemData->vMin5 > vLocalSystemData->v5)
										vLocalSystemData->vMin5 = vLocalSystemData->v5;
									if (vLocalSystemData->vMax5 < vLocalSystemData->v5)
										vLocalSystemData->vMax5 = vLocalSystemData->v5;
									CheckuCPowerSupplyVoltages();
								}
							break;
							case 1: 
								vLocalSystemData->v12 = TempValue;

								if (vLocalSystemData->vuCPoweredUp)
								if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
								{
									if (vLocalSystemData->vMax12 < vLocalSystemData->v12)
										vLocalSystemData->vMax12 = vLocalSystemData->v12;
									if (vLocalSystemData->vMin12 > vLocalSystemData->v12)
										vLocalSystemData->vMin12 = vLocalSystemData->v12;
								}
							break;
							case 2: 
								vLocalSystemData->vMinus12 = TempValue;

								if (vLocalSystemData->vuCPoweredUp)
								if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
								{
									if (vLocalSystemData->vMaxMinus12 < vLocalSystemData->vMinus12)
										vLocalSystemData->vMaxMinus12 = vLocalSystemData->vMinus12;
									if (vLocalSystemData->vMinMinus12 > vLocalSystemData->vMinus12)
										vLocalSystemData->vMinMinus12 = vLocalSystemData->vMinus12;
								}
							break;
							case 3: 
								TempSum = 0;
								//vLocalSystemData->vDataIndex = (vLocalSystemData->vDataIndex + 1) % cNumberOfVoltageSamples;
								vLocalSystemData->vTemperatureuCData[vLocalSystemData->vDataIndex] = TempValue;

							//	TempErrorString = "vTemperatureuCData array ";


								for (BYTE TempLoop = 0; (BYTE)TempLoop < vLocalSystemData->vNumberOfDataSamples; TempLoop++)
								{
									//if(vLocalSystemData->vTemperatureuCData[TempLoop] != 0)
									//	TempNumberOfSamples++;

							//		TempErrorString = TempErrorString + dtoa(TempLoop) + ") " + dtoa( vLocalSystemData->vTemperatureuCData[TempLoop] ) + ", ";
									TempSum = TempSum + vLocalSystemData->vTemperatureuCData[TempLoop];
								}

								if (vLocalSystemData->vNumberOfDataSamples != 0)
								{
									vLocalSystemData->vTemperatureuC = TempSum / vLocalSystemData->vNumberOfDataSamples;
							//		TempErrorString = TempErrorString + "Final Result = " +dtoa(TempSum)+ " / "+ dtoa(vLocalSystemData->vNumberOfDataSamples)
							//			+ " = " + dtoa(vLocalSystemData->vTemperatureuC);
								}

							//	ReportErrorMessage(TempErrorString, cAction,0);

								if (vLocalConfigurationData->vSimulateLightControlBoard)
									vLocalSystemData->vTemperatureuC = 2026;

								if (vLocalSystemData->vuCPoweredUp)
								if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
								{
									if (vLocalSystemData->vTemperatureuC < vLocalSystemData->vTemperatureuCMaximum)
										vLocalSystemData->vTemperatureuCMaximum = vLocalSystemData->vTemperatureuC;

									if (vLocalSystemData->vTemperatureuC > vLocalSystemData->vTemperatureuCMinimum)
										vLocalSystemData->vTemperatureuCMinimum = vLocalSystemData->vTemperatureuC;

									if (!vLocalSystemData->vGivenTemperatureWarning)
									{
										int TempTemp = ConvertToDegreesC(vLocalSystemData->vTemperatureuC,
											vLocalConfigurationData->vTemperatureuCAdjust);
										if (TempTemp > vLocalConfigurationData->vMaximumuCTemperature)
										{
											if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
											{
												::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
												ReportErrorMessage("Error-uC Board Too Hot - Inspection Stopped-Temperature: " + dtoa(TempTemp, 1) + ", Limit: " + dtoa(vLocalConfigurationData->vMaximumuCTemperature, 0), cError,4);
#ifdef CompileInHTTPClientOperationsGuardian
												if (vLocalConfigurationData->vEnableOperationsGuardian)
												{
													vGlobalOGErrorCode = 303;
													vGlobalOGLocationString = "uC";
													vGlobalOGLimitString = dtoa(vLocalConfigurationData->vMaximumuCTemperature, 0);
													::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageTemperatureFault, cOGSevereMessageTemperatureFault);
												}
#endif
											}
											else
												ReportErrorMessage("Error-uC Board Too Hot-Temperature: " + dtoa(TempTemp, 1) + ", Limit: " + dtoa(vLocalConfigurationData->vMaximumuCTemperature, 0), cEMailMaintenance,4);
											vLocalSystemData->vGivenTemperatureWarning = true;
										}
									}
								}
							break;
							case 4: 
								TempSum = 0;
								vLocalSystemData->vTemperatureAirInsideData[vLocalSystemData->vDataIndex] = TempValue;

								if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
								{
									TempTemp = ConvertToDegreesC(TempValue, vLocalConfigurationData->vTemperatureAirInsideAdjust);
									if (TempTemp <= 0)
									{
										SetBit(vGlobalTrackSensors,cAirInsideTemperature);
										//ReportErrorMessage("Error-Air Inside Temperature Sensor Failed.  Reading:" + dtoa(TempTemp, 0), cWriteToLog, 0);
									}
									else
										ClrBit(vGlobalTrackSensors,cAirInsideTemperature);
								}

								for (BYTE TempLoop = 0; (BYTE)TempLoop < vLocalSystemData->vNumberOfDataSamples; TempLoop++)
								{
									TempSum = TempSum + vLocalSystemData->vTemperatureAirInsideData[TempLoop];
								}

								if (vLocalSystemData->vNumberOfDataSamples != 0)
									vLocalSystemData->vTemperatureAirInside = TempSum / vLocalSystemData->vNumberOfDataSamples;

								if (vLocalConfigurationData->vSimulateLightControlBoard)
									vLocalSystemData->vTemperatureAirInside = 2026;

								if (vLocalSystemData->vuCPoweredUp)
								if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
								{
									if (vLocalSystemData->vTemperatureAirInside < vLocalSystemData->vTemperatureAirMaximum)
										vLocalSystemData->vTemperatureAirMaximum = vLocalSystemData->vTemperatureAirInside;

									if (vLocalSystemData->vTemperatureAirInside > vLocalSystemData->vTemperatureAirMinimum)
										vLocalSystemData->vTemperatureAirMinimum = vLocalSystemData->vTemperatureAirInside;

									int TempAirIn = ConvertToDegreesC(vLocalSystemData->vTemperatureAirInside,
										vLocalConfigurationData->vTemperatureAirInsideAdjust);

									if (!vLocalSystemData->vGivenTemperatureWarning)
									{
										if (TempAirIn > vLocalConfigurationData->vMaximumAirTemperature)
										{
											if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
											{
												::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
												ReportErrorMessage("Error-Inside Air Too Hot - Inspection Stopped: " + dtoa(TempAirIn,0) + ", Limit: " + dtoa(vLocalConfigurationData->vMaximumAirTemperature, 0), cError,6);
#ifdef CompileInHTTPClientOperationsGuardian
												if (vLocalConfigurationData->vEnableOperationsGuardian)
												{
													vGlobalOGErrorCode = 301;
													vGlobalOGLocationString = "inside_air";
													vGlobalOGLimitString = dtoa(vLocalConfigurationData->vMaximumAirTemperature, 0);
													::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageTemperatureFault, cOGSevereMessageTemperatureFault);
												}
#endif
											}
											else
												ReportErrorMessage("Error-Inside Air Too Hot: " + dtoa(TempAirIn,0) + ", Limit: " + dtoa(vLocalConfigurationData->vMaximumAirTemperature, 0), cEMailMaintenance,6);
											vLocalSystemData->vGivenTemperatureWarning = true;
										}
									}

									if (vLocalConfigurationData->vDeltaTemperatureForFilterCloggedError)
									if ((vLocalConfigurationData->vHVPSType != cCheyneyMonoBlock) && (vLocalConfigurationData->vHVPSType != c210WMonoBlockHVPS) && 
										(vLocalConfigurationData->vHVPSType != c100WMonoBlockHVPS) && (vLocalConfigurationData->vHVPSType != cMNXHVPS)) //MNX and 210W MonoBlock dont have coolant or MonoBlock Temp
									if (!vLocalSystemData->vGivenFilterCloggedWarning)
									if (vLocalSystemData->vRunDuration / 1800) //if have been running for at least 1/2 hour
									{
										double TempOutsideTemp = 0;
											//TempTemp is surface temp converted, which is outside temperature
										//if (vGlobalFPGAVersion10Point0OrHigher)
										//This is FPGA Version 10 code only
											TempOutsideTemp = ConvertToDegreesC(vLocalSystemData->vTemperatureAirOutside, vLocalConfigurationData->vTemperatureAirOutsideAdjust);
										//else
										//	TempOutsideTemp = ConvertToDegreesC(
										//		vLocalSystemData->vTemperatureSurface,
										//		vLocalConfigurationData->vTemperatureSurfaceAdjust);

										double TempDeltaTemp = TempAirIn - TempOutsideTemp;

										if (TempDeltaTemp > vLocalConfigurationData->vDeltaTemperatureForFilterCloggedError)
										{
											ReportErrorMessage("Check Air Filter and replace if dirty; check fans", cEMailMaintenance,17);
											vLocalSystemData->vGivenFilterCloggedWarning = true;

											ReportErrorMessage("Clog Check Inside: " + dtoa(TempAirIn,1), cWriteToLog,17); 
											ReportErrorMessage("Clog Check Outside: " + dtoa(TempOutsideTemp,1), cWriteToLog,17); 
											ReportErrorMessage("Clog Check Delta: " + dtoa(TempDeltaTemp,1), cWriteToLog,17); 
										}
									}
									int TempLiquidTemperature = ConvertToDegreesC(vLocalSystemData->vTemperatureLiquid, vLocalConfigurationData->vTemperatureLiquidAdjust);

									double TempDeltaTemp = TempLiquidTemperature - TempAirIn;
									if (vLocalConfigurationData->vDeltaTemperatureForCoolantProblem)
									if ((vLocalConfigurationData->vHVPSType != cCheyneyMonoBlock) && (vLocalConfigurationData->vHVPSType != c210WMonoBlockHVPS) && 
										(vLocalConfigurationData->vHVPSType != c100WMonoBlockHVPS) && (vLocalConfigurationData->vHVPSType != cMNXHVPS)) //MNX and 210W MonoBlock dont have coolant or MonoBlock Temp
									if (TempDeltaTemp > vLocalConfigurationData->vDeltaTemperatureForCoolantProblem)
									if (!vLocalSystemData->vGivenFilterCloggedWarning)
									if (vLocalSystemData->vRunDuration / 1800) //if have been running for at least 1/2 hour
									{
										TempString.LoadString(IDS_CoolingSystemProblemCallInspx);
										ReportErrorMessage(TempString,cError,17);
										vLocalSystemData->vGivenFilterCloggedWarning = true;
										ReportErrorMessage("Coolant Hot Vs Air, Coolant: " + dtoa(TempLiquidTemperature,1) + ", Inside Air: " + dtoa(TempAirIn,1), cEMailInspx,32000); 
									}
								}
							break;
							case 5: 
								TempNumberOfSamples = 0;
								TempSum = 0;
								if (vLocalConfigurationData->vSimulateLightControlBoard) 
									TempValue = 2026;

								if (vGlobalDXMXRayPort) 
								if ((vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac)) //set coolant temp since 50 watt has no coolant 
									TempValue = 2026;

								//set coolant temp since 50 watt has no coolant in TDs
								if ((vLocalConfigurationData->vScanTracType == cBriosoScanTrac) || (vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || 
									(vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) || (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) ||
									(vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac))
									TempValue = 2026;

								vLocalSystemData->vTemperatureLiquidData[vLocalSystemData->vDataIndex] = TempValue;

								if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
								{
									TempTemp = ConvertToDegreesC(TempValue, vLocalConfigurationData->vTemperatureLiquidAdjust);
									if (TempTemp <= 0)
									{
										SetBit(vGlobalTrackSensors,cLiquidTemperature);
										//ReportErrorMessage("Error-Liquid Temperature Sensor Failed.  Reading:" + dtoa(TempTemp, 0), cWriteToLog, 0);
									}
									else
										ClrBit(vGlobalTrackSensors,cLiquidTemperature);
								}

								for (BYTE TempLoop = 0; (BYTE)TempLoop < vLocalSystemData->vNumberOfDataSamples; TempLoop++)
								{
									TempSum = TempSum + vLocalSystemData->vTemperatureLiquidData[TempLoop];
								}

								if (vLocalSystemData->vNumberOfDataSamples != 0)
									vLocalSystemData->vTemperatureLiquid = TempSum/vLocalSystemData->vNumberOfDataSamples;

								if (vLocalSystemData->vuCPoweredUp)
								if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
								{
									if (vLocalSystemData->vTemperatureLiquid < vLocalSystemData->vTemperatureLiquidMaximum)
										vLocalSystemData->vTemperatureLiquidMaximum = vLocalSystemData->vTemperatureLiquid;

									if (vLocalSystemData->vTemperatureLiquid > vLocalSystemData->vTemperatureLiquidMinimum)
										vLocalSystemData->vTemperatureLiquidMinimum = vLocalSystemData->vTemperatureLiquid;

									if (!vLocalSystemData->vGivenTemperatureWarning)
									{
										int TempTemp = ConvertToDegreesC(vLocalSystemData->vTemperatureLiquid,
											vLocalConfigurationData->vTemperatureLiquidAdjust);
										if (TempTemp > vLocalConfigurationData->vMaximumLiquidTemperature)
										{
											if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
											{
												::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
												ReportErrorMessage("Error-X-Ray Source Coolant Too Hot - Inspection Stopped-Temperature: " + dtoa(TempTemp, 1) + ", Limit: " + dtoa(vLocalConfigurationData->vMaximumLiquidTemperature, 0), cError,7);
#ifdef CompileInHTTPClientOperationsGuardian
												if (vLocalConfigurationData->vEnableOperationsGuardian)
												{
													vGlobalOGErrorCode = 309;
													vGlobalOGLocationString = "coolant";
													vGlobalOGLimitString = dtoa(vLocalConfigurationData->vMaximumLiquidTemperature, 0);
													::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageTemperatureFault, cOGSevereMessageTemperatureFault);
												}
#endif
											}
											else
												ReportErrorMessage("Error-X-Ray Source Coolant Too Hot-Temperature: " + dtoa(TempTemp, 1) + ", Limit: " + dtoa(vLocalConfigurationData->vMaximumLiquidTemperature, 0), cEMailMaintenance,7);
											vLocalSystemData->vGivenTemperatureWarning = true;
										}
									}
								}
							break;
							case 6:
								TempNumberOfSamples = 0;
								TempSum = 0;
								vLocalSystemData->vTemperatureAirOutsideData[vLocalSystemData->vDataIndex] = TempValue;

								if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
								{
									TempTemp = ConvertToDegreesC(TempValue, vLocalConfigurationData->vTemperatureAirOutsideAdjust);
									if (TempTemp <= 0)
									{
										SetBit(vGlobalTrackSensors,cAirOutsideTemperature);
										//ReportErrorMessage("Error-Air Outside Temperature Sensor Failed.  Reading:" + dtoa(TempTemp, 0), cWriteToLog, 0);
									}
									else
										ClrBit(vGlobalTrackSensors,cAirOutsideTemperature);
								}

								TempCount = 0;
								for (WORD TempLoop = 0; (BYTE)TempLoop < vLocalSystemData->vNumberOfDataSamples; TempLoop++)
								{
									if (vLocalSystemData->vTemperatureAirOutsideData[TempLoop] > 0)
									{
										TempSum = TempSum + vLocalSystemData->vTemperatureAirOutsideData[TempLoop];
										TempCount++;
									}
								}

								if (TempCount > 0)
									vLocalSystemData->vTemperatureAirOutside = TempSum / TempCount;

								if (vLocalConfigurationData->vSimulateLightControlBoard)
									vLocalSystemData->vTemperatureAirOutside = 2026;

								if (vLocalSystemData->vuCPoweredUp)
								if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
								{
									if (vLocalSystemData->vTemperatureAirOutside < vLocalSystemData->vTemperatureAirOutsideMaximum)
										vLocalSystemData->vTemperatureAirOutsideMaximum = vLocalSystemData->vTemperatureAirOutside;

									if (vLocalSystemData->vTemperatureAirOutside > vLocalSystemData->vTemperatureAirOutsideMinimum)
										vLocalSystemData->vTemperatureAirOutsideMinimum = vLocalSystemData->vTemperatureAirOutside;

									if (!vLocalSystemData->vGivenTemperatureWarning)
									{
										int TempTemp = ConvertToDegreesC(vLocalSystemData->vTemperatureAirOutside,
											vLocalConfigurationData->vTemperatureAirOutsideAdjust);
										if (TempTemp > vLocalConfigurationData->vMaximumTemperatureAirOutside)
										{
											CString TempText(_T(""));
											CString TempText1(_T(""));
											CString TempText2(_T(""));
											if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
											{
												::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
												if (vLocalConfigurationData->vDeltaTemperatureForFilterCloggedError == 0)
												{
													TempText.LoadString(IDS_ErrorAirConditionerAirTooHotIS);
													TempText1.LoadString(IDS_AirConditionerTemp);
												}
												else
												{
													TempText.LoadString(IDS_ErrorOutsideAirTooHotIS);
													TempText1.LoadString(IDS_OutsideTemp);
												}
												TempText2.LoadString(IDS_Limit);
												ReportErrorMessage(TempText + ":" + ConvertToDegreesCString(vLocalSystemData->vTemperatureAirInside, vLocalConfigurationData->vTemperatureAirInsideAdjust) + 
													" " + TempText1 + ": " + dtoa(TempTemp,1) + " " + TempText2 + ": " + dtoa(vLocalConfigurationData->vMaximumTemperatureAirOutside,1), cError,7);
#ifdef CompileInHTTPClientOperationsGuardian
												if (vLocalConfigurationData->vEnableOperationsGuardian)
												{
													vGlobalOGErrorCode = 302;
													vGlobalOGLocationString = "outside_air";
													vGlobalOGLimitString = dtoa(vLocalConfigurationData->vMaximumTemperatureAirOutside, 0);
													::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageTemperatureFault, cOGSevereMessageTemperatureFault);
												}
#endif
											}
											else
											{
												if (vLocalConfigurationData->vDeltaTemperatureForFilterCloggedError == 0)
												{
													TempText.LoadString(IDS_ErrorAirConditionerAirTooHotInsideAir);
													TempText1.LoadString(IDS_AirConditionerTemp);
												}
												else
												{
													TempText.LoadString(IDS_ErrorOutsideAirTooHotInsideAir);
													TempText1.LoadString(IDS_OutsideTemp);
												}
												TempText2.LoadString(IDS_Limit);
												ReportErrorMessage(TempText + ":" +
													ConvertToDegreesCString(vLocalSystemData->vTemperatureAirInside, vLocalConfigurationData->vTemperatureAirInsideAdjust) + 
													" " + TempText1 + ": " + dtoa(TempTemp,1) + " " + TempText2 + ": " + dtoa(vLocalConfigurationData->vMaximumTemperatureAirOutside,1), cEMailMaintenance,7);
											}
											vLocalSystemData->vGivenTemperatureWarning = true;
										}
									}
								}
							break;
							case 7: 
								TempNumberOfSamples = 0;
								TempSum = 0;
								vLocalSystemData->vTemperatureSurfaceData[vLocalSystemData->vDataIndex] = TempValue;

								if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
								{
									TempTemp = ConvertToDegreesC(TempValue, vLocalConfigurationData->vTemperatureSurfaceAdjust);
									if (TempTemp <= 0)
									{
										SetBit(vGlobalTrackSensors,cSurfaceTemperature);
										//ReportErrorMessage("Error-Surface Temperature Sensor Failed.  Reading:" + dtoa(TempTemp, 0), cWriteToLog, 0);
									}
									else
										ClrBit(vGlobalTrackSensors,cSurfaceTemperature);
								}

								for (BYTE TempLoop = 0; (BYTE)TempLoop < vLocalSystemData->vNumberOfDataSamples; TempLoop++)
								{
									TempSum = TempSum + vLocalSystemData->vTemperatureSurfaceData[TempLoop];
								}

								if (vLocalSystemData->vNumberOfDataSamples != 0)
									vLocalSystemData->vTemperatureSurface = TempSum/vLocalSystemData->vNumberOfDataSamples;

								if (vLocalConfigurationData->vSimulateLightControlBoard)
									vLocalSystemData->vTemperatureSurface = 2026;

								if (vLocalSystemData->vuCPoweredUp)
								if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
								{
									if (vLocalSystemData->vTemperatureSurface < vLocalSystemData->vTemperatureSurfaceMaximum)
										vLocalSystemData->vTemperatureSurfaceMaximum = vLocalSystemData->vTemperatureSurface;

									if (vLocalSystemData->vTemperatureSurface > vLocalSystemData->vTemperatureSurfaceMinimum)
										vLocalSystemData->vTemperatureSurfaceMinimum = vLocalSystemData->vTemperatureSurface;

									if (!vLocalSystemData->vGivenTemperatureWarning)
									{
										int TempTemp = ConvertToDegreesC(vLocalSystemData->vTemperatureSurface,
											vLocalConfigurationData->vTemperatureSurfaceAdjust);
										if (TempTemp > vLocalConfigurationData->vMaximumSurfaceTemperature)
										{
											if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
											{
												::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
												ReportErrorMessage("Error-X-Ray Source Surface Too Hot - Inspection Stopped, Inside Air:" +
													ConvertToDegreesCString(vLocalSystemData->vTemperatureAirInside, vLocalConfigurationData->vTemperatureAirInsideAdjust) +
													" X-Ray Temp: " + dtoa(TempTemp,1) + " Limit: " +
													dtoa(vLocalConfigurationData->vMaximumSurfaceTemperature,1), cError,7);
#ifdef CompileInHTTPClientOperationsGuardian
												if (vLocalConfigurationData->vEnableOperationsGuardian)
												{
													vGlobalOGErrorCode = 304;
													vGlobalOGLocationString = "xray_surface";
													vGlobalOGLimitString = dtoa(vLocalConfigurationData->vMaximumSurfaceTemperature, 0);
													::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageTemperatureFault, cOGSevereMessageTemperatureFault);
												}
#endif
											}
											else
												ReportErrorMessage("Error-X-Ray Source Surface Too Hot, Inside Air:" +
													ConvertToDegreesCString(vLocalSystemData->vTemperatureAirInside, vLocalConfigurationData->vTemperatureAirInsideAdjust)
													+ " X-Ray Temp: " + dtoa(TempTemp,1) + " Limit: " +
													dtoa(vLocalConfigurationData->vMaximumSurfaceTemperature,1), cEMailMaintenance,7);
											vLocalSystemData->vGivenTemperatureWarning = true;
										}
									}
								}
							break;
							/*
							case 9: 
								vLocalSystemData->vCoolantPressure = TempValue;
								if (vLocalConfigurationData->vSimulateLightControlBoard)
									vLocalSystemData->vCoolantPressure = 2500;

								if (vLocalSystemData->vuCPoweredUp)
								if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
								{
									if (vLocalSystemData->vCoolantPressure > 
										vLocalSystemData->vCoolantPressureMaximum)
										vLocalSystemData->vCoolantPressureMaximum =
										vLocalSystemData->vCoolantPressure;

									if (vLocalSystemData->vCoolantPressure < 
										vLocalSystemData->vCoolantPressureMinimum)
										vLocalSystemData->vCoolantPressureMinimum =
										vLocalSystemData->vCoolantPressure;

									if (!vLocalSystemData->vGivenTemperatureWarning)
									{
										int TempTemp = ConvertToPressurePSI(vLocalSystemData->vCoolantPressure,0);
										//	vLocalConfigurationData->vCoolantPressureAdjust);
										if (TempTemp < vLocalConfigurationData->vMinimumCoolantPressure)
										{
											if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
											{
												::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
												ReportErrorMessage("Error-Coolant Pressure Too Low - Inspection Stopped", cError,7);
											}
											else
												ReportErrorMessage("Error-Coolant Pressure Too Low", cError,7);
											vLocalSystemData->vGivenTemperatureWarning = true;
										}
										if (TempTemp > vLocalConfigurationData->vMaximumCoolantPressure)
										{
											if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
											{
												::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
												ReportErrorMessage("Error-Coolant Pressure Too High - Inspection Stopped", cError,7);
											}
											else
												ReportErrorMessage("Error-Coolant Pressure Too High", cError,7);
											vLocalSystemData->vGivenTemperatureWarning = true;
										}
									}
								}
							break;
							*/
							case 10: 
								vLocalSystemData->v15 = TempValue; //is IO 24 Volts
								if (vLocalConfigurationData->vSimulateLightControlBoard)
									vLocalSystemData->v15 = cuCNormalVoltageValue;

								if (vLocalSystemData->vuCPoweredUp)
								if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
								{
									if (vLocalSystemData->vMax15 < vLocalSystemData->v15)
										vLocalSystemData->vMax15 = vLocalSystemData->v15;
									if (vLocalSystemData->vMin15 > vLocalSystemData->v15)
										vLocalSystemData->vMin15 = vLocalSystemData->v15;
								}
								//This is FPGA Version 10 code only
								//if (vGlobalFPGAVersion10Point0OrHigher)
								{
									if (vLocalSystemData->v15 < 400)// Version10Point0OrHigher
									{
										if (vLocalSystemData->vACPowerOn)  //report only once
										{
											//TempText.LoadString(IDS_ShuttingDown);
											//vLocalSystemData->vOpenInterlock = TempText;
											ReportErrorMessage("Shutting Down A/C Power Off, 24Volts Off", cAction,0);
//for kjh debug comment out											vLocalSystemData->vACPowerOn = false;
											vLocalSystemData->vACPowerOn = false;
											if (vMainWindowPointer)
											{
												vMainWindowPointer->SetInterlockStatus();

												vMainWindowPointer->GenerateProductionReport(cSelectProductProductionReport);
												vMainWindowPointer->SendProductionReport(cSelectProductProductionReport);
											}

										}
									}
									if (!vLocalSystemData->vACPowerOn)
									if (vLocalSystemData->v15 > 1000)// Version10Point0OrHigher
									{
										//TempText.LoadString(IDS_ShuttingDown);
										ReportErrorMessage("A/C Power Turned On, 24Volts On", cAction,0);
										vLocalSystemData->vACPowerOn = true;
										TurnOnOffYellowLamp(false);
										::PostThreadMessage(vGlobalCallingThreadID,cKillCheckACBeforeYellowMessage,0,0);

										if (vMainWindowPointer)
											vMainWindowPointer->SetInterlockStatus();
									}
								}

							break;
							case 14: //read x-ray current monitor
								if ((!vGlobalDXMXRayPort) && (!vGlobalMonoBlockXRayPort))
								{
									//br ht test code only
									/*
									if (vLocalSystemData->vLastXRayCommandOn)
									{
										TempBuffer[1] = 25.5 * 9.8;
										TempBuffer[2] = 25.5 * 6;
									}
									else
									{
										TempBuffer[1] = 25.5 * .2;
										TempBuffer[2] = 25.5 * .3;
									}
									*/
									vLocalSystemData->vCommunicationsTimeOutCounter = 0;
									if (vLocalSystemData->vNumberOfVoltageSamples < cNumberOfVoltageSamples)
										vLocalSystemData->vNumberOfVoltageSamples++;
									if (vLocalConfigurationData->vDemoMode)
									//br htest
									//if (vLocalConfigurationData->vSimulateLightControlBoard)
									{
										if (vLocalSystemData->vLastXRayCommandOn)
										{
											TempBuffer[1] = (BYTE)(vLocalSystemData->vRampXRayVoltageAmount * 25.5);
											TempBuffer[2] = (BYTE)(vLocalSystemData->vRampXRayCurrentAmount * 25.5);
										}
										else
										{
											TempBuffer[1] = 0;
											TempBuffer[2] = 0;
										}
										TempBuffer[3] = 7;
									}

									if (!vLocalConfigurationData->vCurrentAdjustGain)
										vLocalConfigurationData->vCurrentAdjustGain = 1;

									vLocalSystemData->vSourceCurrent = TempValue / 255.;

									if (vLocalSystemData->vSourceCurrent)
										vLocalSystemData->vSourceCurrent = (vLocalSystemData->vSourceCurrent *
											vLocalConfigurationData->vCurrentAdjustGain) +
											vLocalConfigurationData->vCurrentAdjustOffset;
								}
							break;
							case 15: 
								if ((!vGlobalDXMXRayPort) && (!vGlobalMonoBlockXRayPort))
								{
									//br ht test code only
									/*
									if (vLocalSystemData->vLastXRayCommandOn)
									{
										TempBuffer[1] = 25.5 * 9.8;
										TempBuffer[2] = 25.5 * 6;
									}
									else
									{
										TempBuffer[1] = 25.5 * .2;
										TempBuffer[2] = 25.5 * .3;
									}
									*/
									vLocalSystemData->vCommunicationsTimeOutCounter = 0;
									if (vLocalSystemData->vNumberOfVoltageSamples < cNumberOfVoltageSamples)
										vLocalSystemData->vNumberOfVoltageSamples++;
									if (vLocalConfigurationData->vDemoMode)
									//br htest
									//if (vLocalConfigurationData->vSimulateLightControlBoard)
									{
										if (vLocalSystemData->vLastXRayCommandOn)
										{
											TempBuffer[1] = (BYTE)(vLocalSystemData->vRampXRayVoltageAmount * 25.5);
											TempBuffer[2] = (BYTE)(vLocalSystemData->vRampXRayCurrentAmount * 25.5);
										}
										else
										{
											TempBuffer[1] = 0;
											TempBuffer[2] = 0;
										}
										TempBuffer[3] = 7;
									}

									//vLocalSystemData->vSourceVoltageReading[vLocalSystemData->vXRayStatusIndex] = 
									//	TempValue;
									//vLocalSystemData->vSourceCurrentReading[vLocalSystemData->vXRayStatusIndex] = 
									//	TempBuffer[2];
	/*
									TempSum1 = 0;
									TempSum2 = 0;
									for (TempLoop = 0; TempLoop < vLocalSystemData->vNumberOfVoltageSamples; TempLoop++)
									{
										TempSum1 = TempSum1 + vLocalSystemData->vSourceVoltageReading[TempLoop];
										TempSum2 = TempSum2 + vLocalSystemData->vSourceCurrentReading[TempLoop];
									}
		*/
									if (!vLocalConfigurationData->vVoltageAdjustGain)
										vLocalConfigurationData->vVoltageAdjustGain = 1;

									vLocalSystemData->vSourceVoltage = TempValue / 255.;

									if (vLocalSystemData->vSourceVoltage)
										vLocalSystemData->vSourceVoltage = (vLocalSystemData->vSourceVoltage *
											vLocalConfigurationData->vVoltageAdjustGain) + 
											(vLocalConfigurationData->vVoltageAdjustOffset / 6);

									if (vLocalSystemData->vXRayStatusIndex > cNumberOfVoltageSamples - 2)
										vLocalSystemData->vXRayStatusIndex = 0;
									else
										vLocalSystemData->vXRayStatusIndex++;

									VerifySourceWithinTolerance();
									//if (vLocalSystemData->vHVPSInterlockDelay)
									//if (vLocalSystemData->vOpenInterlock == "")
									//if (!(vLocalSystemData->vSourceInterLocks & 0x0020))
									//{
									//	CString TempString1;
									//	CString TempString;
									//	if (vLocalSystemData->vHVPSInterlockDelay < 250)
									//		vLocalSystemData->vHVPSInterlockDelay++;
									//	if (vLocalSystemData->vHVPSInterlockDelay > 3)
									//	if (vLocalSystemData->vLastXRayCommandOn)
									//	{
									//		::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
									//		//EmergencyShutXRaysDown(0);//"High Voltage Power Supply Interlock - Inspection Stopped"
									//		TempString.LoadString(IDS_HighVoltagePowerSupplyInterlock);
									//		ReportErrorMessage(TempString + " - " + TempString1, cError,15);
									//	}
									//	TempString.LoadString(IDS_HVPSInterlock);
									//	vLocalSystemData->vOpenInterlock = TempString;
									//}
								}
								if (!vLocalSystemData->vuCPoweredUp)
									vLocalSystemData->vuCPoweredUp = true;
							break;
						}
					}
				}
			}
			if (TempBuffer[1] == 0x80)
			{
				if (!vLocalConfigurationData->vSimulateLightControlBoard)
					vLocalSystemData->vRawInterlockInput = (TempBuffer[2] << 8) + TempBuffer[3];

				if ((vGlobalGuardMaster) && (vGlobalGuardMaster->vDetectedValidGuardMasterDevice))  //using Guard Master for Interlocks, so don't get interlocks from uC board
				{//still read coolant flow, light stack from uC board, just get 4 bits from Guard Master bits 0x1B00
					vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks & 0x1B00) | (vLocalSystemData->vRawInterlockInput & 0xE4FF);
				}
				else
				{ 
					vLocalSystemData->vSourceInterLocks = vLocalSystemData->vRawInterlockInput;
				}

				
				//set coolant flow OK since 50 watt and TDs have no coolant 
				if ((vLocalConfigurationData->vScanTracType == cBriosoScanTrac) || (vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || 
					(vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) || (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) ||
					(vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac))
				if (!vLocalConfigurationData->vUseExternalInterlock)
					vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x0400;

				if ((!vLocalSystemData->vFPGAVersion9Point1OrHigher) || (vGlobalFPGAVersion10Point0OrHigher))
				{
					vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x0040;//set power indicator as set as wire is not connected before 9.1
				}

				if (!vLocalSystemData->vFPGAVersion8Point7OrHigher)
				{
					vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x0060;//set spellman interlock and power as set as wire is not connected before 8.7
				}

				if (vLocalConfigurationData->vUseDownStreamSignalThatStopsConveyor != cUseDownStream) //if not using down stream signal, set active
					vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks | 0x2000);
				else
				{
					WORD TempOldDownStream = vLocalSystemData->vPreviousInterlockValue & 0x2000;
					WORD TempCurrentDownStream = vLocalSystemData->vSourceInterLocks & 0x2000;

					if (TempCurrentDownStream)
						vLocalSystemData->vDownStreamSignalSaysConveyorOn = true;
					else
						vLocalSystemData->vDownStreamSignalSaysConveyorOn = false;

					if (TempOldDownStream != TempCurrentDownStream)
					{
						//11/18/2009
						if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) ||
 							(vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))
						if (TempCurrentDownStream)  //conveyor starting, turn on simulated encoder
							::PostThreadMessage(vGlobalCallingThreadID,cDownStreamSignalChangedMessage,1,1);
						else
							::PostThreadMessage(vGlobalCallingThreadID,cDownStreamSignalChangedMessage,0,0);
					}
				}

				if (vGlobalDXMXRayPort)
				{
					if (vGlobalDXMXRayPort->vInterLockStatus)
						vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks | 0x0020);//set bit 6, the x-ray interlock bit
					else
						vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks & 0xFFDF); //clear bit 6, the x-ray interlock bit

					vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x0010;  //set Spellman XLF fault to OK
				}
				else
				if (vGlobalMonoBlockXRayPort)
				{
					if (vGlobalMonoBlockXRayPort->vInterLockStatus)
						vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks | 0x0020);//set bit 6, the x-ray interlock bit
					else
						vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks & 0xFFDF); //clear bit 6, the x-ray interlock bit

					vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x0010;  //set Spellman XLF fault to OK

				}
				//added for KJH Office Testing
				else
				if (vLocalConfigurationData->vHVPSType == cXLFHVPS)
					vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x0020;  //Spellman XLF so set interlock OK

				::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("Interlock Changed30: " + ByteToHex((BYTE)(vLocalSystemData->vSourceInterLocks >> 8)) + ", " + 
						ByteToHex((BYTE)(vLocalSystemData->vSourceInterLocks)),cDebugMessage);

				//b rh test
				//ReportErrorMessage("Interlock Changed2: " + ByteToHex((BYTE)(vLocalSystemData->vSourceInterLocks >> 8)) + ", " + 
				//		ByteToHex((BYTE)(vLocalSystemData->vSourceInterLocks)),cError,0);
				//CheckInterlocks();
			}
			if (TempBuffer[1] == 0x82) //from read port 30
			{
				vLocalSystemData->vRawExternalDetectorInput = TempBuffer[3];

				HandleExternalDetectorLineChange(TempBeltPosition, vLocalSystemData->vRawExternalDetectorInput);
			}
			if (TempBuffer[1] == 0x84)
			{
				vLocalSystemData->vRawBodyTriggerInput = TempBuffer[3];
				HandleBodyTriggerLineChange(TempBeltPosition,TempBuffer[3]);
				if (vLocalSystemData->vBackupBodyTriggerMask)  //if configured, then check it
					HandleBackupBodyTriggerChange(TempBeltPosition,TempBuffer[3]);
			}
			//TD Only, Read Motor Signal
			if (TempBuffer[1] == 0xA8)  //address to ask for motor signal rate  was 0xBC in the TD with IO Board
			if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac))
			if (!vLocalConfigurationData->vUseRealEncoder) //only use if using the motor signal
			if (TempWord < 5000)
			{ //Conveyor Encoder Rate read back
				TempWord = (TempBuffer[2] << 8) + TempBuffer[3];
				vLocalSystemData->vConveyorEncoderRate = TempWord;

				if (vLocalSystemData->vRealConveyorEncoderPulsesCount) //if starting to run so average motor signal to find speed to simulate conveyor
				if (vLocalSystemData->vConveyorEncoderRate > 3)
				{
					if (vLocalSystemData->vSecondsMeasuringRealConveyorEncoder) //6/23/2011
					{
						WORD TempPreviousAverage = (WORD)((vLocalSystemData->vRealConveyorEncoderPulsesCount / vLocalSystemData->vSecondsMeasuringRealConveyorEncoder) + .5);
						if ((TempPreviousAverage > vLocalSystemData->vConveyorEncoderRate + 3) || (TempPreviousAverage + 3 < vLocalSystemData->vConveyorEncoderRate)) //Rate different, throw out old
						{
							vLocalSystemData->vSecondsMeasuringRealConveyorEncoder = 0;
							vLocalSystemData->vRealConveyorEncoderPulsesCount = 1;
						}
					}
					vLocalSystemData->vRealConveyorEncoderPulsesCount = vLocalSystemData->vRealConveyorEncoderPulsesCount + vLocalSystemData->vConveyorEncoderRate;
					vLocalSystemData->vSecondsMeasuringRealConveyorEncoder++;
				}

				//11/18/2009
				if (vLocalSystemData->vOldConveyorEncoderRate != vLocalSystemData->vConveyorEncoderRate)
				{
					if (vLocalSystemData->vConveyorEncoderRate == 0)
					{
						if (vLocalConfigurationData->vUseDownStreamSignalThatStopsConveyor == cUseDownStream)
							::PostThreadMessage(vGlobalCallingThreadID,cDownStreamSignalChangedMessage,0,0);

						//conveyor stopped, so stop simulated encoder
						if (vMainWindowPointer)
						if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))//6/23/2011
							vMainWindowPointer->TurnSimulatedEncoderOff();

						TurnAlarmOnOff(4,false);//turn off system inspecting bit since conveyor stoped

						if (vLocalConfigurationData->vPixelsPerDetector == 128)
							ReportErrorMessage("Conveyor Stopped, Turn Off Encoder 32x", cAction,0);
						else
							ReportErrorMessage("Conveyor Stopped, Turn Off Encoder 16x", cAction,0);

						if (vLocalConfigurationData->vContinuousFeedContainerRate)
							vMainWindowPointer->TurnSimulatedContainersOff();
					}
					else
					if (vLocalSystemData->vOldConveyorEncoderRate == 0)
					{
						if (vLocalConfigurationData->vUseDownStreamSignalThatStopsConveyor == cUseDownStream)
							::PostThreadMessage(vGlobalCallingThreadID,cDownStreamSignalChangedMessage,1,1);

						//conveyor just started, so start simulated encoder
						if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))
						{
							WORD TempRate = vLocalConfigurationData->vLastSimulatedEncoderRateSet * 16;

							if (vLocalConfigurationData->vContinuousFeedEncoderRate)
								TempRate = vLocalConfigurationData->vContinuousFeedEncoderRate;

							//if .4mm detectors, want to set to 32 times
							if (vLocalConfigurationData->vPixelsPerDetector == 128)
							{
								TempRate = TempRate * 2;
								if (!vLocalConfigurationData->vContinuousFeedEncoderRate)
									ReportErrorMessage("Conveyor Started, Set Encoder 32x to: " + dtoa(TempRate,0), cAction,0);
								else
									ReportErrorMessage("Conveyor Started, Set Continuous Feed Encoder to: " + dtoa(TempRate,0), cAction,0);
							}
							else
							{
								if (!vLocalConfigurationData->vContinuousFeedEncoderRate)
									ReportErrorMessage("Conveyor Started, Set Encoder 16x to: " + dtoa(TempRate,0), cAction,0);
								else
									ReportErrorMessage("Conveyor Started, Set Continuous Feed Encoder to: " + dtoa(TempRate,0), cAction,0);
							}
							if (TempRate > vLocalSystemData->vBeltRateMaximum)
								TempRate = (WORD)(vLocalSystemData->vBeltRateMaximum * .95);

							if (vMainWindowPointer)
								vMainWindowPointer->SetMotorSignalSimulatedEncoderRate((WORD)TempRate);

							//enable encoder in the FPGA
							vLocalSystemData->vCurrentConveyorOutput = vLocalSystemData->vCurrentConveyorOutput | 0x01;//enable encoder for simulated encoder

							if (vMainWindowPointer)
								vMainWindowPointer->Send0x9ACommand();

							//KillTimer(vConveyorStartTimerHandle);
							//int TempTimerResult = SetTimer(vConveyorStartTimerHandle,5000,NULL);
							//if (!TempTimerResult)
							//	ReportErrorMessage("Error-vConveyorStart Timer Failed",cWriteToLog,32000);

							if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
							if (vLocalConfigurationData->vEnableEjectors)
							TurnAlarmOnOff(4,true);//turn on system inspecting bit if inspecting and ejectors enabled since conveyor started


							if (vLocalConfigurationData->vContinuousFeedContainerRate)
								vMainWindowPointer->TurnSimulatedContainersOn();
						}
					}
					vLocalSystemData->vOldConveyorEncoderRate = vLocalSystemData->vConveyorEncoderRate;
				}
			}
			else
			if (TempWord == 65535)
			{
				//ReportErrorMessage("Conveyor encoder cannot be read now", cWriteToLog,0);
			}
			else
			if (!vLocalConfigurationData->vSimulateLightControlBoard)
				ReportErrorMessage("Conveyor encoder rate too high", cEMailInspx, 32000);
		break;
		case 0x32: //CommTestEcho
			vLocalSystemData->vLastCommTestBytes[0] = TempBuffer[1];
			vLocalSystemData->vLastCommTestBytes[1] = TempBuffer[2];
			vLocalSystemData->vLastCommTestBytes[2] = TempBuffer[3];
/*
			if ((vLastSent[0] == vLocalSystemData->vLastCommTestBytes[0]) &&
				(vLastSent[1] == vLocalSystemData->vLastCommTestBytes[1]) &&
				(vLastSent[2] == vLocalSystemData->vLastCommTestBytes[2]))
				vLastCommTestStatusOK = true;
			else
				vLastCommTestStatusOK = true;
*/
			if (((BYTE)(vLocalSystemData->vLastCommTestBytes[0] + 1) == (BYTE)(vLocalSystemData->vLastCommTestBytes[1])) &&
				((BYTE)(vLocalSystemData->vLastCommTestBytes[1] + 1) == (BYTE)(vLocalSystemData->vLastCommTestBytes[2])))
			{
				vLastCommTestStatusOK = true;
				vReceiveCount++;
			}
			else
				vLastCommTestStatusOK = false;

			if (vSendingContinuous)
			{
				if ((vReceiveCount % 50) == 0)
				::PostThreadMessage(vGlobalCallingThreadID,cGotTestCommReplyMessage,
					vLastCommTestStatusOK,vLastCommTestStatusOK);
				Sleep(10); //br ht
				SendCommTest();
			}
			else
			::PostThreadMessage(vGlobalCallingThreadID,cGotTestCommReplyMessage,
				vLastCommTestStatusOK,vLastCommTestStatusOK);

		break;
		case 0x36: //uC reset and version 
			vLocalSystemData->vuCMajorVersionNumber = TempBuffer[1];
			vLocalSystemData->vuCMinorVersionNumber = TempBuffer[2];
			//vLocalSystemData->vShuttingDownXRays = true;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				vLocalSystemData->vLogFile.WriteToLogFile("Error-SerialPort:uCReset",cDebugMessage);
			}

			vLocalSystemData->vuCResetCount++;
			//Watch dog caused a reset
			if ((TempBuffer[3] & 1) == 1)
			{
				vLocalSystemData->vuCWatchDogCount++;
				if (vLocalSystemData->vuCWatchDogCount > 1)
				{
					TempString1 = "Error - uController Watch Dog Reset: " + dtoa(vLocalSystemData->vuCWatchDogCount - 1, 0);
					ReportErrorMessage(TempString1, cEMailInspx,32000);
					TempWatchDog = 1;
				}
			}
			else
			if (vLocalSystemData->vuCResetCount > 1)
			{
				ReportErrorMessage("Error - uController Reset", cEMailInspx,32000);
				TempWatchDog = 1;
			}

			::PostThreadMessage(vGlobalCallingThreadID,cuControllerResetMessage,
					TempWatchDog,TempWatchDog);
		break;
		case 0x3C: //uC Queue Overflow Error 
			if (!vLocalSystemData->vGaveQueueOverflowMessage)
			{
				vLocalSystemData->vGaveQueueOverflowMessage = true;
				TempString1 = ByteToHex(TempBuffer[1]) + ", " + ByteToHex(TempBuffer[2]) + ", " + ByteToHex(TempBuffer[3]);
				if ((TempBuffer[1] & 0x04) == 0x04)
					TempString1 = "Error - uController message invalid Ejector number";
				else
				if ((TempBuffer[1] & 0x80) == 0x80)
					TempString1 = "Error - uController message Q2 Stack Overflow";
				else
				if ((TempBuffer[1] & 0x40) == 0x40)
					TempString1 = "Error - uController message FEN On/Off Overflow";
				else
					TempString1 = "Error - uController Queue Overflow Error: " + TempString1;
				ReportErrorMessage(TempString1, cEMailInspx,32000);
			}
		break;
		case 0x3B: //uC version  
			vLocalSystemData->vuCMajorVersionNumber = TempBuffer[1];
			vLocalSystemData->vuCMinorVersionNumber = TempBuffer[2];
		break;
		case 0x38: //FPGA Version, passed self test 
			vLocalSystemData->vFPGAMajorVersionNumber = TempBuffer[1];
			vLocalSystemData->vFPGAMinorVersionNumber = TempBuffer[2];
			vLocalSystemData->vFPGASelfTest = TempBuffer[3];
			if (vLocalSystemData->vFPGASelfTest == 0)
				ReportErrorMessage("Error - FPGA Failed Self Test", cEMailInspx,32000);

			//set all version levels to false, then set the correct ones to true

			vGlobalFPGAVersion18point0orAbove = false;
			vGlobalFPGAVersion17point0orAbove = false;
			vGlobalFPGAVersion16point7orAbove = false;
			vGlobalFPGAVersion16point0orAbove = false;
			vGlobalFPGAVersion10Point0OrHigher = false;
			vGlobalFPGAVersion10Point87OrHigher = false;
			vGlobalFPGAVersion11Point0OrHigher = false;
			vGlobalFPGAVersion15Point2OrHigher = false;
			vGlobalFPGAVersion15Point31OrHigher = false;
			vGlobalFPGAVersion15Point34OrHigher = false;
			vGlobalFPGAVersion15point42orAbove = false;
			vLocalSystemData->vFPGAVersion8Point7OrHigher = false;
			vLocalSystemData->vFPGAVersion9Point0OrHigher = false;
			vLocalSystemData->vFPGAVersion9Point1OrHigher = false;
			vLocalSystemData->vFPGAVersion9Point2OrHigher = false;
			vLocalSystemData->vFPGAVersion10Point7OrHigher = false;
			vLocalSystemData->vFPGAVersion10Point86OrHigher = false;


			if ((vLocalSystemData->vFPGAMajorVersionNumber > 8) || 
				((vLocalSystemData->vFPGAMajorVersionNumber == 8) && (vLocalSystemData->vFPGAMinorVersionNumber > 6)))
				vLocalSystemData->vFPGAVersion8Point7OrHigher = true;

			if (vLocalSystemData->vFPGAMajorVersionNumber > 8)
				vLocalSystemData->vFPGAVersion9Point0OrHigher = true;

			if (vLocalSystemData->vFPGAMajorVersionNumber > 9)
				vLocalSystemData->vFPGAVersion9Point1OrHigher = true;

			if ((vLocalSystemData->vFPGAMajorVersionNumber > 8) && (vLocalSystemData->vFPGAMinorVersionNumber > 0))
				vLocalSystemData->vFPGAVersion9Point1OrHigher = true;

			if (vLocalSystemData->vFPGAMajorVersionNumber > 9)
				vLocalSystemData->vFPGAVersion9Point2OrHigher = true;

			if ((vLocalSystemData->vFPGAMajorVersionNumber > 8) && (vLocalSystemData->vFPGAMinorVersionNumber > 1))
				vLocalSystemData->vFPGAVersion9Point2OrHigher = true;

			if (vLocalSystemData->vFPGAMajorVersionNumber > 10)
			{
				vLocalSystemData->vFPGAVersion10Point7OrHigher = true;
				//vLocalSystemData->vFPGAVersion10Point9OrHigher = true;
				vLocalSystemData->vFPGAVersion10Point86OrHigher = true;
			}

			if ((vLocalSystemData->vFPGAMajorVersionNumber == 10) && (vLocalSystemData->vFPGAMinorVersionNumber >= 86))
			{
				vLocalSystemData->vFPGAVersion10Point86OrHigher = true;
			}

			//Bill release 10.5, then 10.51, then 10.6 then 10.61, then 10.62, then 10.7 then 10.71, then 10.8, then 10.81, then 10.82
			//then 10.83, then 10.84 and will only increment by one from now on.
			if ((vLocalSystemData->vFPGAMajorVersionNumber == 10) && ((vLocalSystemData->vFPGAMinorVersionNumber >= 7) ||
				(vLocalSystemData->vFPGAMajorVersionNumber > 10)))
				vLocalSystemData->vFPGAVersion10Point7OrHigher = true;

			if (vLocalSystemData->vFPGAMajorVersionNumber > 10)
				vGlobalFPGAVersion11Point0OrHigher = true;
			else
			{
				if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
				if (!vLocalConfigurationData->vSimulateLightControlBoard)
					ReportErrorMessage("Error-Case Inspector ScanTrac Requires Version 15 or Higher FPGA", cError, 0);
			}

			if ((vLocalSystemData->vFPGAMajorVersionNumber > 15) || ((vLocalSystemData->vFPGAMajorVersionNumber == 15) && (vLocalSystemData->vFPGAMinorVersionNumber >= 2)))
			{
					vGlobalFPGAVersion15Point2OrHigher = true;
					//ReportErrorMessage("Information-FPGA Verison 15.2 or higher", cWriteToLog, 0);
			}
			//else
			//		ReportErrorMessage("Information-FPGA Verison 15.1 or Lower", cWriteToLog, 0);
			
			if (vLocalSystemData->vFPGAMajorVersionNumber > 15)
				vGlobalFPGAVersion15Point31OrHigher = true;
			else
			{
				if (vLocalSystemData->vFPGAMajorVersionNumber == 15)
				if (vLocalSystemData->vFPGAMinorVersionNumber >= 31)
					vGlobalFPGAVersion15Point31OrHigher = true;
			}
			
			if (vLocalSystemData->vFPGAMajorVersionNumber > 15)
				vGlobalFPGAVersion15Point34OrHigher = true;
			else
			{
				if (vLocalSystemData->vFPGAMajorVersionNumber == 15)
				if (vLocalSystemData->vFPGAMinorVersionNumber >= 34)
					vGlobalFPGAVersion15Point34OrHigher = true;
			}
			
			if (vLocalSystemData->vFPGAMajorVersionNumber > 15)
				vGlobalFPGAVersion15point42orAbove = true;
			else
			{
				if (vLocalSystemData->vFPGAMajorVersionNumber == 15)
				if (vLocalSystemData->vFPGAMinorVersionNumber >= 42)
					vGlobalFPGAVersion15point42orAbove = true;
			}

			if (vLocalSystemData->vFPGAMajorVersionNumber >= 10)
			{
				vGlobalFPGAVersion10Point0OrHigher = true;

				//if (vLocalConfigurationData->vPreAmplifierGain == 2)  //default version 9 is 4
				//	vLocalConfigurationData->vPreAmplifierGain = 1;  //default version 10 is 2

				if (vLocalConfigurationData->vDetectorClockDivider == 20)  //default version 9 is 20
					vLocalConfigurationData->vDetectorClockDivider = 8;  //default version 10 is 8

				if (((vLocalSystemData->vFPGAMajorVersionNumber == 10) && (vLocalSystemData->vFPGAMinorVersionNumber >= 87)) || 
					(vLocalSystemData->vFPGAMajorVersionNumber >= 11))
					vGlobalFPGAVersion10Point87OrHigher = true;
			}
			else
			if ((vLocalSystemData->vFPGAMajorVersionNumber == 8) || (vLocalSystemData->vFPGAMajorVersionNumber == 9)) //if version 8 or 9
			{
				//if (vLocalConfigurationData->vPreAmplifierGain == 1)  //default version 10 is 2
				//	vLocalConfigurationData->vPreAmplifierGain = 2;  //default version 9 is 4

				if (vLocalConfigurationData->vDetectorClockDivider == 8)  //default version 10 is 8
					vLocalConfigurationData->vDetectorClockDivider = 20;  //default version 9 is 20
			}
			if (vLocalSystemData->vFPGAMajorVersionNumber > 15)
				vGlobalFPGAVersion16point0orAbove = true;

			if ((vLocalSystemData->vFPGAMajorVersionNumber > 16) ||
				((vLocalSystemData->vFPGAMajorVersionNumber > 15) && (vLocalSystemData->vFPGAMajorVersionNumber > 6)))
				vGlobalFPGAVersion16point7orAbove = true;

			if (vLocalSystemData->vFPGAMajorVersionNumber > 16)
				vGlobalFPGAVersion17point0orAbove = true;

			if (vLocalSystemData->vFPGAMajorVersionNumber > 17)
				vGlobalFPGAVersion18point0orAbove = true;
			
			if (vLocalSystemData->vFPGAMajorVersionNumber > 18)
			{
				ReportErrorMessage("Error-Invalid FPGA Version from Power Glitch", cError, 0);
				ReportErrorMessage("Please Power Down, Wait 5 minutes, then Turn On", cError, 0);
			}
			
		break;
		case 0x41: //X-Ray source voltage
			if (vLocalConfigurationData->vDemoMode)
				vLocalSystemData->vuCLEDGYR = 7;
			else
				vLocalSystemData->vuCLEDGYR = TempBuffer[3];
			if (vGlobalFPGAVersion10Point0OrHigher) //new board does not test voltages right so always say OK
				vLocalSystemData->vuCLEDGYR = vLocalSystemData->vuCLEDGYR | 1;

		if ((!vGlobalDXMXRayPort) && (!vGlobalMonoBlockXRayPort))
		if (!vGlobalFPGAVersion10Point0OrHigher)
		{
			//br ht test code only
			//vLocalSystemData->vSourceInterLocks = 0xF7FF;
			//CheckInterlocks(true);
			/*
			if (vLocalSystemData->vLastXRayCommandOn)
			{
				TempBuffer[1] = 25.5 * 9.8;
				TempBuffer[2] = 25.5 * 6;
			}
			else
			{
				TempBuffer[1] = 25.5 * .2;
				TempBuffer[2] = 25.5 * .3;
			}
			*/
			vLocalSystemData->vCommunicationsTimeOutCounter = 0;
			if (vLocalSystemData->vNumberOfVoltageSamples < cNumberOfVoltageSamples)
				vLocalSystemData->vNumberOfVoltageSamples++;
			if (vLocalConfigurationData->vDemoMode)
			//br htest
			//if (vLocalConfigurationData->vSimulateLightControlBoard)
			if (vLocalSystemData->vLastXRayCommandOn)
			{
				TempBuffer[1] = (BYTE)(vLocalSystemData->vRampXRayVoltageAmount * 25.5);
				TempBuffer[2] = (BYTE)(vLocalSystemData->vRampXRayCurrentAmount * 25.5);
			}
			else
			{
				TempBuffer[1] = 0;
				TempBuffer[2] = 0;
			}
			TempBuffer[3] = 7;

			{
				vLocalSystemData->vSourceVoltageReading[vLocalSystemData->vXRayStatusIndex] = 
					TempBuffer[1];
				vLocalSystemData->vSourceCurrentReading[vLocalSystemData->vXRayStatusIndex] = 
					TempBuffer[2];

				TempSum1 = 0;
				TempSum2 = 0;
				for (BYTE TempLoop = 0; TempLoop < vLocalSystemData->vNumberOfVoltageSamples; TempLoop++)
				{
					TempSum1 = TempSum1 + vLocalSystemData->vSourceVoltageReading[TempLoop];
					TempSum2 = TempSum2 + vLocalSystemData->vSourceCurrentReading[TempLoop];
				}
				if (!vLocalConfigurationData->vVoltageAdjustGain)
					vLocalConfigurationData->vVoltageAdjustGain = 1;
				if (!vLocalConfigurationData->vCurrentAdjustGain)
					vLocalConfigurationData->vCurrentAdjustGain = 1;
				if (vLocalSystemData->vSourceRampedUp)
				{
					vLocalSystemData->vSourceVoltage = ((TempSum1 / vLocalSystemData->vNumberOfVoltageSamples) / 25.5);
					vLocalSystemData->vSourceCurrent = ((TempSum2 / vLocalSystemData->vNumberOfVoltageSamples) / 25.5);
					if (vLocalSystemData->vSourceVoltage)
						vLocalSystemData->vSourceVoltage = (vLocalSystemData->vSourceVoltage *
							vLocalConfigurationData->vVoltageAdjustGain) + 
							(vLocalConfigurationData->vVoltageAdjustOffset / 6);
					if (vLocalSystemData->vSourceCurrent)
						vLocalSystemData->vSourceCurrent = (vLocalSystemData->vSourceCurrent *
							vLocalConfigurationData->vCurrentAdjustGain) +
							vLocalConfigurationData->vCurrentAdjustOffset;
				}
				else
				{
					vLocalSystemData->vSourceVoltage = (TempBuffer[1] / 25.5);
					if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
						vLocalSystemData->vSourceVoltage = (vLocalSystemData->vSourceVoltage *
							vLocalConfigurationData->vVoltageAdjustGain) + 
							(vLocalConfigurationData->vVoltageAdjustOffset / 6);
					vLocalSystemData->vSourceCurrent = (TempBuffer[2] / 25.5);
					if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
						vLocalSystemData->vSourceCurrent = (vLocalSystemData->vSourceCurrent *
							vLocalConfigurationData->vCurrentAdjustGain) + 
							vLocalConfigurationData->vCurrentAdjustOffset;
				}
			}

			if (vLocalSystemData->vXRayStatusIndex > cNumberOfVoltageSamples - 2)
				vLocalSystemData->vXRayStatusIndex = 0;
			else
				vLocalSystemData->vXRayStatusIndex++;

			VerifySourceWithinTolerance();
			//if (vLocalSystemData->vHVPSInterlockDelay)
			//if (vLocalSystemData->vOpenInterlock == "")
			//if (!(vLocalSystemData->vSourceInterLocks & 0x0020))
			//{
			//	CString TempString1;
			//	CString TempString;
			//	//if (vLocalSystemData->vHVPSInterlockDelay < 250)
			//	//	vLocalSystemData->vHVPSInterlockDelay++;
			//	if (vLocalSystemData->vHVPSInterlockDelay > 3)
			//	if (vLocalSystemData->vLastXRayCommandOn)
			//	{
			//		::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
			//		//EmergencyShutXRaysDown(0);//"High Voltage Power Supply Interlock - Inspection Stopped"
			//		TempString.LoadString(IDS_HighVoltagePowerSupplyInterlock);
			//		ReportErrorMessage(TempString + " - " + TempString1, cError,15);
			//	}
			//	TempString.LoadString(IDS_HVPSInterlock);
			//	vLocalSystemData->vOpenInterlock = TempString;
			//}
		}
		break;
		case 0x45: //uController Power supply voltage
			if (!vGlobalFPGAVersion10Point0OrHigher)
			{
				vLocalSystemData->v5Data[vLocalSystemData->vDataIndex] = TempBuffer[1];
				vLocalSystemData->v33Data[vLocalSystemData->vDataIndex] = TempBuffer[2];
				vLocalSystemData->v25Data[vLocalSystemData->vDataIndex] = TempBuffer[3];
				if (vLocalSystemData->vNumberOfDataSamples < cNumberOfVoltageSamples)
					vLocalSystemData->vNumberOfDataSamples++;
				TempSum1 = 0;
				TempSum2 = 0;
				TempSum3 = 0;
				for (BYTE TempLoop = 0; TempLoop < cNumberOfVoltageSamples; TempLoop++)
				{
					TempSum1 = TempSum1 + vLocalSystemData->v5Data[TempLoop];
					TempSum2 = TempSum2 + vLocalSystemData->v33Data[TempLoop];
					TempSum3 = TempSum3 + vLocalSystemData->v25Data[TempLoop];
				}
				vLocalSystemData->v5 = (BYTE)(TempSum1 / vLocalSystemData->vNumberOfDataSamples);
				vLocalSystemData->v33 = TempSum2 / vLocalSystemData->vNumberOfDataSamples;
				vLocalSystemData->v25 = TempSum3 / vLocalSystemData->vNumberOfDataSamples;

				//br ht temp test
				//vLocalSystemData->v5 = TempBuffer[1];
				//vLocalSystemData->v33 = TempBuffer[2];
				//vLocalSystemData->v25 = TempBuffer[3];
			}
		break;
		case 0x46: //uController Power supply voltage
			if (!vGlobalFPGAVersion10Point0OrHigher)
			{
				vLocalSystemData->v12Data[vLocalSystemData->vDataIndex] = TempBuffer[1];
				vLocalSystemData->v15Data[vLocalSystemData->vDataIndex] = TempBuffer[2];
				vLocalSystemData->vMinus12Data[vLocalSystemData->vDataIndex] = TempBuffer[3];
				TempSum1 = 0;
				TempSum2 = 0;
				TempSum3 = 0;
				for (BYTE TempLoop = 0; TempLoop < cNumberOfVoltageSamples; TempLoop++)
				{
					TempSum1 = TempSum1 + vLocalSystemData->v12Data[TempLoop];
					TempSum2 = TempSum2 + vLocalSystemData->vMinus12Data[TempLoop];
					TempSum3 = TempSum3 + vLocalSystemData->v15Data[TempLoop];
				}
				if (vLocalSystemData->vNumberOfDataSamples)
				{
					vLocalSystemData->v12 = TempSum1 / vLocalSystemData->vNumberOfDataSamples;
					vLocalSystemData->v15 = TempSum2 / vLocalSystemData->vNumberOfDataSamples;
					vLocalSystemData->vMinus12 = TempSum3 / vLocalSystemData->vNumberOfDataSamples;
				}

				//br ht temp test
				//vLocalSystemData->v12 = TempBuffer[1];
				//vLocalSystemData->v15 = TempBuffer[2];
				//vLocalSystemData->vMinus12 = TempBuffer[3];

				CheckuCPowerSupplyVoltages();
			}
		break;
		case 0x47: //uController Temperature monitors
			if (!vGlobalFPGAVersion10Point0OrHigher)
			{
				vLocalSystemData->vTemperatureSurfaceData[vLocalSystemData->vDataIndex] = TempBuffer[1];
				vLocalSystemData->vTemperatureLiquidData[vLocalSystemData->vDataIndex] = TempBuffer[2];
				vLocalSystemData->vTemperatureuCData[vLocalSystemData->vDataIndex] = TempBuffer[3];
				TempSum1 = 0;
				TempSum2 = 0;
				TempSum3 = 0;
				for (BYTE TempLoop = 0; TempLoop < cNumberOfVoltageSamples; TempLoop++)
				{
					TempSum1 = TempSum1 + vLocalSystemData->vTemperatureSurfaceData[TempLoop];
					TempSum2 = TempSum2 + vLocalSystemData->vTemperatureLiquidData[TempLoop];
					TempSum3 = TempSum3 + vLocalSystemData->vTemperatureuCData[TempLoop];
				}
				if (vLocalSystemData->vNumberOfDataSamples)
				{
					vLocalSystemData->vTemperatureSurface = TempSum1 / vLocalSystemData->vNumberOfDataSamples;
					vLocalSystemData->vTemperatureLiquid = TempSum2 / vLocalSystemData->vNumberOfDataSamples;
					vLocalSystemData->vTemperatureuC = TempSum3 / vLocalSystemData->vNumberOfDataSamples;
				}

				if (vLocalConfigurationData->vSimulateLightControlBoard)
				//This code is for less than FPGA Version 10 already
					/*
				if (vGlobalFPGAVersion10Point0OrHigher)
				{
					vLocalSystemData->vTemperatureSurface = cuCNormalVoltageValue;
					vLocalSystemData->vTemperatureLiquid = cuCNormalVoltageValue;
					vLocalSystemData->vTemperatureAirInside = cuCNormalVoltageValue;
					vLocalSystemData->vTemperatureuC = cuCNormalVoltageValue;
					vLocalSystemData->vTemperatureAirOutside = cuCNormalVoltageValue;  
					//vLocalSystemData->vCoolantPressure = cuCNormalVoltageValue;  
				}
				else
				*/
				{
					vLocalSystemData->vTemperatureSurface = 193;
					vLocalSystemData->vTemperatureLiquid = 193;
					vLocalSystemData->vTemperatureAirInside = 193;
					vLocalSystemData->vTemperatureuC = 193;
					vLocalSystemData->vTemperatureAirOutside = 193;  
					//vLocalSystemData->vCoolantPressure = 193; 
				}

				if (vLocalSystemData->vuCPoweredUp)
				if (vLocalSystemData->vNumberOfDataSamples)
				{
					//temperatures values are reversed numerically
					if (vLocalSystemData->vTemperatureSurface <  vLocalSystemData->vTemperatureSurfaceMaximum)
						vLocalSystemData->vTemperatureSurfaceMaximum = vLocalSystemData->vTemperatureSurface;

					//temperatures values are reversed numerically
					if (vLocalSystemData->vTemperatureLiquid < vLocalSystemData->vTemperatureLiquidMaximum)
						vLocalSystemData->vTemperatureLiquidMaximum = vLocalSystemData->vTemperatureLiquid;

					//temperatures values are reversed numerically
					if (vLocalSystemData->vTemperatureuC < vLocalSystemData->vTemperatureuCMaximum)
						vLocalSystemData->vTemperatureuCMaximum = vLocalSystemData->vTemperatureuC;

					//temperatures values are reversed numerically
					if (vLocalSystemData->vTemperatureSurface > vLocalSystemData->vTemperatureSurfaceMinimum)
						vLocalSystemData->vTemperatureSurfaceMinimum = vLocalSystemData->vTemperatureSurface;

					//temperatures values are reversed numerically
					if (vLocalSystemData->vTemperatureLiquid >  vLocalSystemData->vTemperatureLiquidMinimum)
						vLocalSystemData->vTemperatureLiquidMinimum = vLocalSystemData->vTemperatureLiquid;

					//temperatures values are reversed numerically
					if (vLocalSystemData->vTemperatureuC >  vLocalSystemData->vTemperatureuCMinimum)
						vLocalSystemData->vTemperatureuCMinimum = vLocalSystemData->vTemperatureuC;
				}
				//check temperatures, if too hot or too cold, shut down system
				if (!vLocalSystemData->vGivenTemperatureWarning)
				if (vLocalSystemData->vuCPoweredUp)
				{
					TempString1.LoadString(IDS_XRaysShutDown);
					int TempTemp = ConvertToDegreesC(vLocalSystemData->vTemperatureuC,
						vLocalConfigurationData->vTemperatureuCAdjust);
					if (TempTemp > vLocalConfigurationData->vMaximumuCTemperature)
					{
						TempString.LoadString(IDS_DetectorTooHot);
						if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
						{
							::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
							//EmergencyShutXRaysDown(0);//"Detector Too Hot - X-Rays Shut Down"
							ReportErrorMessage(TempString + " - " + TempString1, cError,4);
							ReportErrorMessage(TempString + " - " + TempString1, cEMailInspx,4);
						}
						else
						if (!vLocalSystemData->vGivenTemperatureWarning)
						{
							ReportErrorMessage(TempString, cEMailMaintenance,4);//"Detector Too Hot"
							ReportErrorMessage(TempString, cEMailInspx,4);//"Detector Too Hot"
						}
#ifdef CompileInHTTPClientOperationsGuardian
						if (vLocalConfigurationData->vEnableOperationsGuardian)
						{
							vGlobalOGErrorCode = 303;
							vGlobalOGLocationString = "uC";
							::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageTemperatureFault, cOGSevereMessageTemperatureFault);
						}
#endif
						vLocalSystemData->vGivenTemperatureWarning = true;
					}

					int TempSurfaceTemperature = ConvertToDegreesC(vLocalSystemData->vTemperatureSurface,
						vLocalConfigurationData->vTemperatureSurfaceAdjust);

					if (TempSurfaceTemperature > vLocalConfigurationData->vMaximumSurfaceTemperature)
					{
						TempString.LoadString(IDS_XRaySourceTooHot);
						if (vLocalConfigurationData->vHasOutsideAirTemp)
						{
							if (vLocalConfigurationData->vDeltaTemperatureForFilterCloggedError == 0)
								TempString.LoadString(IDS_AirConditionerAirTooHot);
								else
								TempString.LoadString(IDS_OutsideAirTooHot);
						}
						if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
						{
							::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
							//EmergencyShutXRaysDown(0); //Outside Air Too Hot
							ReportErrorMessage(TempString + " - " + TempString1, cError,7);
							ReportErrorMessage(TempString + " - " + TempString1, cEMailInspx,7);
						}
						else
						if (!vLocalSystemData->vGivenTemperatureWarning)
						{
							ReportErrorMessage(TempString, cEMailMaintenance,7);
							ReportErrorMessage(TempString, cEMailInspx,7);
						}
#ifdef CompileInHTTPClientOperationsGuardian
						if (vLocalConfigurationData->vEnableOperationsGuardian)
						{
							vGlobalOGErrorCode = 302;
							vGlobalOGLocationString = "outside_air";
							::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageTemperatureFault, cOGSevereMessageTemperatureFault);
						}
#endif
						vLocalSystemData->vGivenTemperatureWarning = true;
					}

					int TempLiquidTemperature = ConvertToDegreesC(vLocalSystemData->vTemperatureLiquid, 
						vLocalConfigurationData->vTemperatureLiquidAdjust);

					int TempAirIn = ConvertToDegreesC(vLocalSystemData->vTemperatureAirInside, vLocalConfigurationData->vTemperatureAirInsideAdjust);

					if (vLocalConfigurationData->vDeltaTemperatureForFilterCloggedError)
					if (!vLocalSystemData->vGivenFilterCloggedWarning)
					if (vLocalSystemData->vRunDuration / 1800) //if have been running for at least 1/2 hour
					{
						double TempOutsideTemp = 0;
							//TempTemp is surface temp converted, which is outside temperature
						//FPGA Code below version 10 only
						TempOutsideTemp = ConvertToDegreesC(vLocalSystemData->vTemperatureSurface, vLocalConfigurationData->vTemperatureSurfaceAdjust);

						double TempDeltaTemp = TempAirIn - TempOutsideTemp;

						if (TempDeltaTemp > vLocalConfigurationData->vDeltaTemperatureForFilterCloggedError)
						{
							ReportErrorMessage("Check Air Filter and replace if dirty; check fans", cEMailMaintenance,17);
							vLocalSystemData->vGivenFilterCloggedWarning = true;

							ReportErrorMessage("Clog Check Inside: " + dtoa(TempAirIn,1), cWriteToLog,17); 
							ReportErrorMessage("Clog Check Outside: " + dtoa(TempOutsideTemp,1), cWriteToLog,17); 
							ReportErrorMessage("Clog Check Delta: " + dtoa(TempDeltaTemp,1), cWriteToLog,17); 
						}

						TempDeltaTemp = TempLiquidTemperature - TempAirIn;
						if (vLocalConfigurationData->vDeltaTemperatureForCoolantProblem)
						if ((vLocalConfigurationData->vHVPSType != cCheyneyMonoBlock) && (vLocalConfigurationData->vHVPSType != c210WMonoBlockHVPS) && 
							(vLocalConfigurationData->vHVPSType != c100WMonoBlockHVPS) && (vLocalConfigurationData->vHVPSType != cMNXHVPS)) //MNX and 210W MonoBlock dont have coolant or MonoBlock Temp
						if (TempDeltaTemp > vLocalConfigurationData->vDeltaTemperatureForCoolantProblem)
						if (!vLocalSystemData->vGivenFilterCloggedWarning)
						if (vLocalSystemData->vRunDuration / 1800) //if have been running for at least 1/2 hour
						{
							TempString.LoadString(IDS_CoolingSystemProblemCallInspx);
							ReportErrorMessage(TempString,cError,17);
							vLocalSystemData->vGivenFilterCloggedWarning = true;
							ReportErrorMessage("Check Coolant, Coolant: " + dtoa(TempLiquidTemperature,1) + ", Inside Air: " + dtoa(TempAirIn,1), cEMailInspx,32000); 
						}
					}

					if (TempLiquidTemperature > vLocalConfigurationData->vMaximumLiquidTemperature) 
					{
						TempString.LoadString(IDS_CoolantTooHot);
						if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
						{
							::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
							//EmergencyShutXRaysDown(0);//"X-Ray Coolant Too Hot - X-Rays Shut Down"
							ReportErrorMessage(TempString + " - " + TempString1, cError,5);
							ReportErrorMessage(TempString + " - " + TempString1, cEMailInspx,5);
						}
						else
						if (!vLocalSystemData->vGivenTemperatureWarning)//"X-Ray Coolant Too Hot"
						{
							ReportErrorMessage(TempString, cEMailMaintenance,5);
							ReportErrorMessage(TempString, cEMailInspx,5);
						}
#ifdef CompileInHTTPClientOperationsGuardian
						if (vLocalConfigurationData->vEnableOperationsGuardian)
						{
							vGlobalOGErrorCode = 309;
							vGlobalOGLocationString = "coolant";
							::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageTemperatureFault, cOGSevereMessageTemperatureFault);
						}
#endif
						vLocalSystemData->vGivenTemperatureWarning = true;
					}
				}
			}
		break;
		case 0x48: //uController temp and x-ray source monitors
			if (!vGlobalFPGAVersion10Point0OrHigher)
			{
				vLocalSystemData->vTemperatureAirInsideData[vLocalSystemData->vDataIndex] = TempBuffer[1];
				vLocalSystemData->vSpareMonitorData[vLocalSystemData->vDataIndex] = TempBuffer[3];
				TempSum1 = 0;
				TempSum2 = 0;
				TempSum3 = 0;
				for (BYTE TempLoop = 0; TempLoop < cNumberOfVoltageSamples; TempLoop++)
				{
					TempSum1 = TempSum1 + vLocalSystemData->vTemperatureAirInsideData[TempLoop];
					TempSum3 = TempSum3 + vLocalSystemData->vSpareMonitorData[TempLoop];
				}
				vLocalSystemData->vTemperatureAirInside = TempSum1 / cNumberOfVoltageSamples;
				vLocalSystemData->vSpareMonitor = TempSum3 / cNumberOfVoltageSamples;

				if (vLocalConfigurationData->vSimulateLightControlBoard)
					vLocalSystemData->vTemperatureAirInside = 193;

				if (vLocalSystemData->vuCPoweredUp)
				if (vLocalSystemData->vNumberOfDataSamples)
				{
					//temperatures values are reversed numerically
					if (vLocalSystemData->vTemperatureAirInside < vLocalSystemData->vTemperatureAirMaximum)
						vLocalSystemData->vTemperatureAirMaximum = vLocalSystemData->vTemperatureAirInside;

					//temperatures values are reversed numerically
					if (vLocalSystemData->vTemperatureAirInside > vLocalSystemData->vTemperatureAirMinimum)
						vLocalSystemData->vTemperatureAirMinimum = vLocalSystemData->vTemperatureAirInside;
				}

				if (vLocalSystemData->vDataIndex > cNumberOfVoltageSamples - 2)
				{
					vLocalSystemData->vDataIndex = 0;
					vLocalSystemData->vuCPoweredUp = true;
				}
				else
					vLocalSystemData->vDataIndex++;

				if (!vLocalSystemData->vGivenTemperatureWarning)
				if (vLocalSystemData->vuCPoweredUp)
				{
					int TempTemp = ConvertToDegreesC(vLocalSystemData->vTemperatureAirInside,
						vLocalConfigurationData->vTemperatureAirInsideAdjust);
					if (TempTemp > vLocalConfigurationData->vMaximumAirTemperature)
					{
						TempString.LoadString(IDS_XRaySourceTooHot);
						TempString1.LoadString(IDS_XRaysShutDown);
						if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
						{
							::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
							//EmergencyShutXRaysDown(0);//"Error-Inside Air Too Hot - X-Rays Shut Down"
							ReportErrorMessage(TempString + " - " + TempString1, cError,6);
							ReportErrorMessage(TempString + " - " + TempString1, cEMailInspx,6);
						}
						else//"Error-Inside Air Too Hot"
						{
							ReportErrorMessage(TempString, cEMailMaintenance,6);
							ReportErrorMessage(TempString, cEMailInspx,6);
						}
#ifdef CompileInHTTPClientOperationsGuardian
						if (vLocalConfigurationData->vEnableOperationsGuardian)
						{
							vGlobalOGErrorCode = 301;
							vGlobalOGLocationString = "inside_air";
							::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageTemperatureFault, cOGSevereMessageTemperatureFault);
						}
#endif
						vLocalSystemData->vGivenTemperatureWarning = true;
					}
				}
			}
		break;
		case 0x49: //Conveyor Overspeed Error
			ProcessBeltPosition(TempBeltPosition,false);
		break;
		case 0x54:
			ReportErrorMessage("uC Error, Got Eject Interupt, but eject queue is empty. BP: " + WordToHex(TempBeltPosition), cWriteToLog, 0);
		break;
		case 0x55: //Current Conveyor Position
			vLocalSystemData->vCommunicationsTimeOutCounter = 0;
			vLocalSystemData->vNoCommunicationsWithUController = false;
			vNoCommunications = false;

			if ((!TempBuffer[3]) && (!vLocalSystemData->vuCMainLoopCounter))
			if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
				ReportErrorMessage("Error-uC Main Loop Health: 0", cEMailInspx,32000);

			vLocalSystemData->vuCMainLoopCounter = TempBuffer[3];
			ProcessBeltPosition(TempBeltPosition,true);
		break;
		case 0x56: //Current Conveyor Position
			ProcessBeltPosition(TempBeltPosition,false);
		break;
		case 0x57: //Diagnostic Information like what was just loaded into the comparitor
		case 0x58: //Diagnostic Information like what was just loaded into the comparitor
		case 0x59: //Diagnostic Information like what was just loaded into the comparitor
		break;
		case 0x5A: //Synchronization Message
			//if uC off, then get feed back and false sync messages, so don't use to tell uC is there
			vLocalSystemData->vCommunicationsTimeOutCounter = 0;
			if (SyncFromMicroControllerSeen < 100)
				SyncFromMicroControllerSeen++;
			if ((SyncFromMicroControllerSeen > 2) /*&& (vGlobalFirstMicroControllerCommCheck == cInitializeSearchForSerialPort) || (vGlobalFirstMicroControllerCommCheck == cStartSearchForSerialPort) || (vGlobalFirstMicroControllerCommCheck == cSearchingForNextSerialPort)*/)
			{
				vGlobalFirstMicroControllerCommCheck = cFinishedSearchForNextSerialPort;
				vGlobalPortHereOK[vComPortNumber] = cCURRENT_COMM_MICRO_CONTROLLER_SERIAL_PORT_NUMBER;
				if ((vComPortNumber != cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER) && (ReportComNotDefaultDone == false))
				{
					ReportComNotDefaultDone = true;
					CString TempAString;
					TempAString.Format(_T("Error-uC Com Port %d, Default Com 1"),vGlobalSerialPortNumberMicroController);
					ReportErrorMessage(TempAString, cEMailInspx,32000);
//					ReportErrorMessage(TempAString, cError,0);
				}
			}
		break;
		case 0x5C: //
			if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) && (vLocalSystemData->vITIPCDig->vShowOnlyRealImages))
			{
				::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
				//EmergencyShutXRaysDown(0);
				ReportErrorMessage("Error-uC Stopped Inspection , No SBC Comm for 20 seconds", cEMailInspx,32000);
				ReportErrorMessage("Error-uC Stopped Inspection, No SBC Comm for 20 seconds", cError,0);
#ifdef CompileInHTTPClientOperationsGuardian
				if (vLocalConfigurationData->vEnableOperationsGuardian)
				{
					vGlobalOGErrorCode = 702;
					vGlobalOGLocationString = "uC";
					::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageComFault, cOGSevereMessageComFault);
				}
#endif
			}
			else
				ReportErrorMessage("Error-No SBC Comm for 20 seconds", cEMailInspx,32000);
		break;
		case 0x66: //Diagnostic Information just change the end of frame Conveyor position
		break;
		case 0x68: //uController reporting did not take an image because too close to previous image
			if (!vGlobalInAutoSize)
			if ((vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem) || ((vLocalSystemData->vSystemRunMode == cRunningSystemMode)))
			if (vLocalSystemData->vFPGASimulateEncoderRate == 0)
			if (!vLocalConfigurationData->vContinuousFeedEncoderRate)
			if (!(vLocalSystemData->vCurrentLampStatus & 2)) //if the yellow light is off
			if (vLocalConfigurationData->vBackupDetectionUsingBodyTriggerTimeOut & 0x7F)
			if (vLocalSystemData->vActualEncoderRate == 0)
				ReportErrorMessage("Maintenance-No Encoder signal, Check Encoder", cError,0);
			else
				ReportErrorMessage("Maintenance-Containers too close together, Check for slipping", cEMailMaintenance,0);
		break;
		case 0x67: //Image will start later as previous image will not be done.
		{
			if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
			{
				if (vLocalSystemData->vContainerCollection->GetCount() > 0)
				{
					if (vLocalSystemData->vLogFile.vLogSerialData)
						WriteToLogFileWithBeltPosition("Received Changed Frame Start Position ", TempBeltPosition);
					CContainer *TempContainer = FindContainerAtStartImage(TempBeltPosition);
					if (TempContainer != NULL)
					{
						if (vLocalSystemData->vLogFile.vLogSerialData)
						{
							WriteToLogFileWithBeltPosition("Changed Start Position of Container c: "
								+ dtoa(TempContainer->vBodyTriggerPosition, 0) + " by: " + dtoa(TempBuffer[3], 0), TempContainer->vBodyTriggerPosition);
						}

						//set start of this container to be this many lines later
						TempContainer->vStartImagePosition = BPAdd(TempContainer->vStartImagePosition,(WORD)TempBuffer[3]);
						TempContainer->vNumberOfLinesShort = TempContainer->vNumberOfLinesShort + TempBuffer[3] + 1;

						// 8/27/2001
						if (vPreviousContainer)
							vPreviousContainer->vNumberOfLinesShort = TempBuffer[3] + vPreviousContainer->vNumberOfLinesShort;
					}
					else
					if (vLocalSystemData->vLogFile.vLogSerialData)
						WriteToLogFileWithBeltPosition("Changed Frame Start Position - Can't Find ", BPSubtract(TempBeltPosition,vGlobalCurrentProduct->vBTToImagePosition * vGlobalCurrentProduct->vOverScanMultiplier));
				}
				else
				if (vLocalSystemData->vLogFile.vLogSerialData)
					WriteToLogFileWithBeltPosition("Changed Frame Start Position but no containers", TempBeltPosition);
				vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
			}
			else
				ReportErrorMessage("Error-Serial Thread Lock Failed 2", cEMailInspx,32000);
		}
		break;
		case 0x77: //debug message just for log indicating new start for trigger set.
		break;
		default:
			ReportErrorMessage("Error - Unknown uC Message: " + ByteToHex(TempBuffer[0]), cEMailInspx,32000);
	}
}

void CSerialPort::CheckInterlocks(bool TempJustRecievedValues)
{
	//if get a fault, or Red lamp fault, then shut down source if on
	if (vLocalSystemData->vOldSourceInterLocks != vLocalSystemData->vSourceInterLocks)
	{
		int TempNewInterlocks = vLocalSystemData->vSourceInterLocks;
		//if interlock broken and X-Rays are on then check if should shut down if should and report
		CString TempString1;
		CString TempString;
		TempString1.LoadString(IDS_XRaysShutDown);
		vLocalSystemData->vOpenInterlock = "";


		//check A/C power is still on
		if ((!vGlobalFPGAVersion10Point0OrHigher) && (vLocalSystemData->vFPGAVersion9Point1OrHigher))
		{
			if (!(vLocalSystemData->vSourceInterLocks & 0x0040)) //a/c power off
			{
				if (vLocalSystemData->vOldSourceInterLocks & 0x0040)  //A/C Power was on last check
				{
					vLocalSystemData->vACPowerOn = false;
					if ((vLocalSystemData->vLastXRayCommandOn) || (vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
					{
						::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						ReportErrorMessage(TempString + " - A/C Power Turned Off " + TempString1, cError,28);
						ReportErrorMessage(TempString + " - A/C Power Turned Off " + TempString1, cEMailInspx,32000);
					}
					else
					{
						ReportErrorMessage(TempString + " - A/C Power Turned Off", cAction,0);
					}
					if (vMainWindowPointer)
						vMainWindowPointer->SetInterlockStatus();
#ifdef CompileInHTTPClientOperationsGuardian
					if (vLocalConfigurationData->vEnableOperationsGuardian)
					{
						vGlobalOGErrorCode = 800;
						vGlobalOGLocationString = "power";
						::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessagePowerFault, cOGSevereMessagePowerFault);
					}
#endif
				}
			}
			else //A/C Power On
			if (!(vLocalSystemData->vOldSourceInterLocks & 0x0040))  //A/C power was off last check
			{
				vLocalSystemData->vACPowerOn = true;
				ReportErrorMessage("A/C Power Turned On <10.0", cAction,0);
				TurnOnOffYellowLamp(false);
				::PostThreadMessage(vGlobalCallingThreadID,cKillCheckACBeforeYellowMessage,0,0);

				if (vMainWindowPointer)
					vMainWindowPointer->SetInterlockStatus();
			}
		}
		if (!vLocalSystemData->vOpenInterlock.GetLength())	//kjh here
		{
			if (!(vLocalSystemData->vSourceInterLocks & 0x0400))
			{
				if (vLocalConfigurationData->vUseExternalInterlock)
					TempString.LoadString(IDS_ExternalInterlock);
				else
					TempString.LoadString(IDS_CoolantFlowFault);
				if ((vLocalSystemData->vLastXRayCommandOn) || (vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
				{
					::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
					vLocalSystemData->vLastXRayCommandOn = false;
					//EmergencyShutXRaysDown(0);//"Error-Coolant Flow Fault - X-Rays Shut Down"
					if (vLocalSystemData->vACPowerOn)
					{
						ReportErrorMessage(TempString + " - " + TempString1, cError,8);
						//ReportErrorMessage(TempString + " - " + TempString1, cEMailInspx,8);
					}
#ifdef CompileInHTTPClientOperationsGuardian
					if (vLocalConfigurationData->vEnableOperationsGuardian)
					{
						vGlobalOGErrorCode = 900;
						vGlobalOGLocationString = "coolant_flow";
						::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageTemperatureFault, cOGSevereMessageTemperatureFault);
					}
#endif
				}
				else
				if (!vLocalConfigurationData->vUseExternalInterlock)  //if X-Rays are not on, and just an external interlock, don't need to do anything except change status
				{
					TurnOnOffYellowLamp(true);
					if (vLocalSystemData->vACPowerOn)
					if (!vLocalSystemData->vCheckACBeforeThisYellowMessage.GetLength())
					if ((vGlobalScanTracType != cAtempoScanTrac) && (vGlobalScanTracType != cForteScanTrac) && (vGlobalScanTracType != cAllegroScanTrac) && 
						(vGlobalScanTracType != cCaseInspectorScanTrac) && (vGlobalScanTracType != cBriosoScanTrac) && (vGlobalScanTracType != cLegatoScanTrac) && (vGlobalScanTracType != cPiccoloScanTrac))
					{
						vLocalSystemData->vCheckACBeforeThisYellowMessage = TempString;
						vLocalSystemData->vCheckACBeforeThisYellowMessageTempLevel = cEMailMaintenance;
						vLocalSystemData->vCheckACBeforeThisYellowMessageTempErrorCode = 8;
						::PostThreadMessage(vGlobalCallingThreadID,cCheckACBeforeYellowMessage,0,0);
					}
					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile(TempString,cDebugMessage);
					//ReportErrorMessage(TempString, cEMailMaintenance,8);
					//ReportErrorMessage(TempString, cEMailInspx,8);
				}
				vLocalSystemData->vOpenInterlock = TempString;
			}
			else
			if (!(vLocalSystemData->vSourceInterLocks & 0x1000)) 
			{
				if (vLocalSystemData->vLastXRayCommandOn)
				{//"F/R Door Interlock Broken - X-Rays Shut Down"
					::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,1,1);
#ifdef CompileInHTTPClientOperationsGuardian
					if (vLocalConfigurationData->vEnableOperationsGuardian)
					{
						vGlobalOGErrorCode = 202;
						vGlobalOGLocationString = "front_door";
						::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageInterlockBroken, cOGSevereMessageInterlockBroken);
					}
#endif
					if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
						(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
						TempString.LoadString(IDS_UpperDoorInterlockBroken);
					else
						TempString.LoadString(IDS_FRDoorInterlockBroken);
					ReportErrorMessage(TempString + " - " + TempString1, cError,14);
					vLocalSystemData->vLastXRayCommandOn = false;
				}
				if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
					(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
					TempString.LoadString(IDS_TopDoorOpen);
				else
					TempString.LoadString(IDS_FRDoorOpen);
				vLocalSystemData->vOpenInterlock = TempString;
			}
			else
			if (!(vLocalSystemData->vSourceInterLocks & 0x0800))
			{
				if (vLocalSystemData->vLastXRayCommandOn)// || (vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
				{
					::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,1,1);
#ifdef CompileInHTTPClientOperationsGuardian
					if (vLocalConfigurationData->vEnableOperationsGuardian)
					{
						if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
							(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
						{
							vGlobalOGErrorCode = 207;
							vGlobalOGLocationString = "conveyor_door";
						}
						else
						{
							vGlobalOGErrorCode = 204;
							vGlobalOGLocationString = "conveyor_cover";
						}
						::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageInterlockBroken, cOGSevereMessageInterlockBroken);
					}
#endif
					if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
						(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
						TempString.LoadString(IDS_LowerDoorInterlockBroken);
					else
						TempString.LoadString(IDS_SideDoorInterlockBroken);
					ReportErrorMessage(TempString + " - " + TempString1, cError,13);
				}
				if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
					(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
					TempString.LoadString(IDS_LowerDoor);
				else
					TempString.LoadString(IDS_SDoorOpen);
				vLocalSystemData->vOpenInterlock = TempString;
			}
			else
			if (!(vLocalSystemData->vSourceInterLocks & 0x0200))
			{
				if ((vLocalSystemData->vLastXRayCommandOn) || (vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
				{//"E-Stop Shut Off - X-Rays Shut Down"
					::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
#ifdef CompileInHTTPClientOperationsGuardian
					if (vLocalConfigurationData->vEnableOperationsGuardian)
					{
							vGlobalOGErrorCode = 200;
							vGlobalOGLocationString = "estop";
						::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageInterlockBroken, cOGSevereMessageInterlockBroken);
					}
#endif
					//EmergencyShutXRaysDown(0);
					TempString.LoadString(IDS_EStopShutOff);
					ReportErrorMessage(TempString + " - " + TempString1, cError,16);
				}
				TempString.LoadString(IDS_EStop);
				vLocalSystemData->vOpenInterlock = TempString;
			}
			else
			if (!(vLocalSystemData->vSourceInterLocks & 0x0100))
			{
				if ((vLocalSystemData->vLastXRayCommandOn) || (vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
				{
					::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
#ifdef CompileInHTTPClientOperationsGuardian
					if (vLocalConfigurationData->vEnableOperationsGuardian)
					{
							vGlobalOGErrorCode = 201;
							vGlobalOGLocationString = "key_switch";
						::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageInterlockBroken, cOGSevereMessageInterlockBroken);
					}
#endif
					//EmergencyShutXRaysDown(0);//"Key Switch Turned Off - X-Rays Shut Down"
					TempString.LoadString(IDS_KeySwitchTurnedOff);
					ReportErrorMessage(TempString + " - " + TempString1, cError,15);
				}
				TempString.LoadString(IDS_KeySwitch);
				vLocalSystemData->vOpenInterlock = TempString;
			}
			else
			if (!(vLocalSystemData->vSourceInterLocks & 0x0010))
			{
				TempString.LoadString(IDS_XRaySourceFault);
				if (vLocalSystemData->vLastXRayCommandOn)// || (vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
				{
					if (TempJustRecievedValues)
						::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
					else
					{
						::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						//EmergencyShutXRaysDown(0);//"Error-X-Ray Source Fault - X-Rays Shut Down"
						ReportErrorMessage("Hardware Interlock circuit problem, call service - " + TempString1, cEMailMaintenance,9);
						ReportErrorMessage("Hardware Interlock circuit problem, call service - " + TempString1, cEMailInspx,9);
						ReportErrorMessage(TempString + " - " + TempString1, cError,9);
					}
#ifdef CompileInHTTPClientOperationsGuardian
					if (vLocalConfigurationData->vEnableOperationsGuardian)
					{
							vGlobalOGErrorCode = 208;
							vGlobalOGLocationString = "xray";
						::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageInterlockBroken, cOGSevereMessageInterlockBroken);
					}
#endif
				}
				TempString.LoadString(IDS_XRayFault);
				vLocalSystemData->vOpenInterlock = TempString;
			}
			else
			if (!(vLocalSystemData->vSourceInterLocks & 0x0020))
			{
				TempString.LoadString(IDS_HVPSInterlock);
				if (vLocalSystemData->vLastXRayCommandOn)// || (vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
				{
					if (TempJustRecievedValues)
						::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
					else
					{ //have waited 200 mSec, so take action
						::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						//EmergencyShutXRaysDown(0);//"Error-X-Ray Source Fault - X-Rays Shut Down"
						ReportErrorMessage("HVPS Interlock circuit problem 2, call service - " + TempString1, cError,9);
						ReportErrorMessage(TempString + " - " + TempString1, cEMailInspx,9);
					}
#ifdef CompileInHTTPClientOperationsGuardian
					if (vLocalConfigurationData->vEnableOperationsGuardian)
					{
						vGlobalOGErrorCode = 208;
						vGlobalOGLocationString = "xray";
						::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageInterlockBroken, cOGSevereMessageInterlockBroken);
					}
#endif
				}
				vLocalSystemData->vOpenInterlock.LoadString(IDS_HVPSInterlock);
				//vLocalSystemData->vHVPSInterlockDelay++;
			}
			else
				/*
			if (!(vLocalSystemData->vSourceInterLocks & 0x2000))
			{
				if (vLocalSystemData->vACPowerOn)
				{
					ReportErrorMessage("Error-No Ejector Air Pressure", cEMailMaintenance,0);
					ReportErrorMessage("Error-No Ejector Air Pressure", cEMailInspx, 32000);
				}
				vLocalSystemData->vOpenInterlock = "Ejector Air";
			}
			else
			*/
			{
				//vLocalSystemData->vHVPSInterlockDelay = 0;
				if (vLocalSystemData->vPosteduCVoltageProblemMessage)
				{
					vLocalSystemData->vOpenInterlock.LoadString(IDS_PowerSupplyProblem);
				}
				else
				if (vLocalSystemData->vAirConditionerFailed)
				{
					vLocalSystemData->vOpenInterlock.LoadString(IDS_ACFailed);
				}
				else
				if (vLocalSystemData->vWeightTrendingAlarm)
				{
					vLocalSystemData->vOpenInterlock.LoadString(IDS_WeightTrend);
				}
				else
				if ((vLocalSystemData->vCheckWeighOffsetCalibrationNeeded) && (vLocalSystemData->vSystemRunMode == cRunningSystemMode))
				{
					vLocalSystemData->vOpenInterlock.LoadString(IDS_CalibrateWeight);
				}
				else
				if (vLocalSystemData->vShutterFailedLastTest)
				{
					vLocalSystemData->vOpenInterlock.LoadString(IDS_ShutterFailed);
				}
				else
				if (vLocalSystemData->vProductHasAPoorLearn)
				{
					vLocalSystemData->vOpenInterlock.LoadString(IDS_PoorLearn);
				}
				else
				//if (vLocalSystemData->vHadABrownOut)
				//{
				//	vLocalSystemData->vOpenInterlock.LoadString(IDS_BrownOut);
				//}
				//else
				//if (vLocalSystemData->vHadAnACPowerDropOut)
				//{
				//	vLocalSystemData->vOpenInterlock.LoadString(IDS_ACDropOut);
				//}
				//else
				if (vLocalSystemData->vHadAPowerMoniterMessageError % 16)	//if flags 1, 2, 4, 8 are on
				{
					//vLocalSystemData->vOpenInterlock.LoadString(IDS_ACPowerCriticalError);
				}
				else 
				if  (vLocalSystemData->vGivenTemperatureSensorWarning == cTemperatureSensorWarningRedBoxSent)
				{
					vLocalSystemData->vOpenInterlock = "Temp Sensor";
				}
				else
				if ((vLocalConfigurationData->vEnableOperationsGuardian) && (vLocalSystemData->vOperationsGuardianError))
				{
					vLocalSystemData->vOpenInterlock.LoadString(IDS_GuardianServer);
				}
				else 
				if (vLocalSystemData->vDetectorCalibrationNeeded)
				{
					vLocalSystemData->vOpenInterlock.LoadString(IDS_DetectorCalibrationNeeded);
				}
			}
		}

		if (!(vLocalSystemData->vSourceInterLocks & 0x0001)) //red lamp
		{
			if (vLocalSystemData->vACPowerOn)
				::PostThreadMessage(vGlobalCallingThreadID,cCheckLampsMessage,0,0);
		}
		else
		if (!(vLocalSystemData->vSourceInterLocks & 0x0002)) //yellow lamp
		{
			if (vLocalSystemData->vACPowerOn)
				::PostThreadMessage(vGlobalCallingThreadID,cCheckLampsMessage,0,0);
		}
		else
		if (!(vLocalSystemData->vSourceInterLocks & 0x0004)) //blue lamp OK
		{
			if (vLocalSystemData->vACPowerOn)
				::PostThreadMessage(vGlobalCallingThreadID,cCheckLampsMessage,0,0);
		}
		else
		if  (vLocalSystemData->vDisplayingAnEncoderError)
			vLocalSystemData->vOpenInterlock.LoadString(IDS_EncoderFaulty);
		else
		if ((vGlobalGuardMaster) && (vGlobalGuardMaster->vDetectedValidGuardMasterDevice) && (vGlobalGuardMaster->vNoCommunications))
			vLocalSystemData->vOpenInterlock.LoadString(IDS_NoInterlockComm);
	
		ReportErrorMessage("CI-Interlock Changed: " + WordToHex(vLocalSystemData->vSourceInterLocks), cWriteToLog,0);
		vLocalSystemData->vOldSourceInterLocks = TempNewInterlocks;
	}
}

void CSerialPort::ProcessBeltPosition(WORD TempBeltPosition, bool TempSyncValue)
{
	if (TempSyncValue)
	{
		//in counts per second
		if ((vLocalSystemData->vITIPCDig->vShowOnlyRealImages) || (!vNoCommunications))
		{
			vLocalSystemData->vActualEncoderRate = BPSubtract(TempBeltPosition, vLocalSystemData->vLastEncoderCountToCalculateEncoderRate);
			vLocalSystemData->vLastEncoderCountToCalculateEncoderRate = TempBeltPosition;
		}

		if (vLocalSystemData->vActualEncoderRate > 20000)
			vLocalSystemData->vActualEncoderRate = vPreviousEncoderReading;
		if (vLocalSystemData->vActualEncoderRate < 3)
			vLocalSystemData->vActualEncoderRate = 0;

		if ((vLocalSystemData->vActualEncoderRate) && (!vPreviousEncoderReading))
		{//if last two seconds encoder was zero, assume stopped and clear history
			ClearEncoderHistory();
		}

		if ((vLocalSystemData->vSystemRunMode == cCalibratingDetectorOffsetMode) || (vLocalSystemData->vSimulatingFastestEncoder) || (vOldSimulatingFastestEncoder))
			vLocalSystemData->vEncoderSample[vLocalSystemData->vEncoderSampleIndex] = vPreviousEncoderReading;
		else
		{
			vLocalSystemData->vEncoderSample[vLocalSystemData->vEncoderSampleIndex] = vLocalSystemData->vActualEncoderRate;
			//br htest		(vLocalSystemData->vEncoderSampleIndex / 10) + 1200;

			vPreviousEncoderReading = vLocalSystemData->vActualEncoderRate;
		}
		vLocalSystemData->vEncoderSampleIndex++;
		if (vLocalSystemData->vEncoderSampleIndex >= cNumberOfEncoderSamples)
			vLocalSystemData->vEncoderSampleIndex = 0;

		if (vLocalConfigurationData->vUseRealEncoder) //only use if using a real encoder
		{
			vLocalSystemData->vConveyorEncoderRate = vLocalSystemData->vActualEncoderRate;

			//11/18/2009
			if (vLocalSystemData->vOldConveyorEncoderRate != vLocalSystemData->vConveyorEncoderRate)
			{
				if (vLocalSystemData->vConveyorEncoderRate == 0)
				{
					::PostThreadMessage(vGlobalCallingThreadID,cDownStreamSignalChangedMessage,0,0);
				}
				else
				if (vLocalSystemData->vOldConveyorEncoderRate == 0)
				{
					::PostThreadMessage(vGlobalCallingThreadID,cDownStreamSignalChangedMessage,1,1);
				}
				vLocalSystemData->vOldConveyorEncoderRate = vLocalSystemData->vConveyorEncoderRate;
			}
		}

		//check if encoder varring too much up and down twice in last two minutes
		if (vLocalConfigurationData->vEncoderTolerance)
		{
			bool TempGoingUp = false;
			bool TempGoingDown = false;
			WORD TempIndexToCheck = vLocalSystemData->vEncoderSampleIndex;
			WORD TempNumberOfReversals = 0;
			WORD TempPreviousValue = vLocalSystemData->vEncoderSample[TempIndexToCheck];
			for (WORD TempLoop = 0; TempLoop < cNumberOfEncoderSamples - 1; TempLoop++)
			{
				if (TempIndexToCheck)
					TempIndexToCheck--;
				else
					TempIndexToCheck = cNumberOfEncoderSamples - 1;

				if (vLocalSystemData->vEncoderSample[TempIndexToCheck] > 
					TempPreviousValue + vLocalConfigurationData->vEncoderTolerance)
				{
					TempGoingUp = true;
					if (TempGoingDown)
					{
						TempGoingDown = false;
						TempNumberOfReversals++;
					}
					TempPreviousValue = vLocalSystemData->vEncoderSample[TempIndexToCheck];
				}
				if (vLocalSystemData->vEncoderSample[TempIndexToCheck] + 
					vLocalConfigurationData->vEncoderTolerance < TempPreviousValue)
				{
					TempGoingDown = true;
					if (TempGoingUp)
					{
						TempGoingUp = false;
						TempNumberOfReversals++;
					}
					TempPreviousValue = vLocalSystemData->vEncoderSample[TempIndexToCheck];
				}
			}
			if (vLocalConfigurationData->vNumberOfEncoderRateReversalsForError)
			if (TempNumberOfReversals > 
				vLocalConfigurationData->vNumberOfEncoderRateReversalsForError)
			if (!vGaveEncoderWarning)
			{
				vGaveEncoderWarning = true;
				CString TempText;
				TempText.LoadString(IDS_EncoderMayBeFaultyCheckOrReplace);
				ReportErrorMessage(TempText,cEMailInspx,32000);
				ClearEncoderHistory();
			}
		}
		if (vLocalSystemData->vSimulatingFastestEncoder)
			vOldSimulatingFastestEncoder = 3;
		else 
		if (vOldSimulatingFastestEncoder)
			vOldSimulatingFastestEncoder--;

		//if ((vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem) || 
		//	((vLocalSystemData->vSystemRunMode == cRunningSystemMode) &&
		//	(vLocalSystemData->vSourceRampedUp)))
		//if ((!vLocalSystemData->vShuttingDownXRays) && (vLocalSystemData->vCheckConveyor == 2))
		if (vLocalSystemData->vActualEncoderRate > vLocalSystemData->vActualEncoderRateMaximum)
		if (vLocalSystemData->vActualEncoderRate > 4)
		if ((vLocalSystemData->vSystemRunMode != cCalibratingDetectorOffsetMode) &&
			(!vLocalSystemData->vSimulatingFastestEncoder) && (!vOldSimulatingFastestEncoder))
			vLocalSystemData->vActualEncoderRateMaximum = vLocalSystemData->vActualEncoderRate;

		/*
		if (!vLocalSystemData->vFPGASimulateEncoderRate)
		if (!vLocalConfigurationData->vContinuousFeedEncoderRate)
		if (vOldActualEncoderRate != vLocalSystemData->vActualEncoderRate)
		if (vBeltPositionBodyTriggerWentActive)
		if (vLocalSystemData->vSourceRampedUp)
		if (!vGaveBodyTriggerActiveTooLongError)
		if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
		{			//look and see if body trigger is stuck high
			WORD TempDistanceBodyTriggerActive = BPSubtract(
				TempBeltPosition,vBeltPositionBodyTriggerWentActive);
			if (TempDistanceBodyTriggerActive > 10000)
			{
				ReportErrorMessage("Maintenance-Clean and align the photoeye", 
					cEMailMaintenance,0);
				ReportErrorMessage("Error-Container Trigger Active Too Long", 
					cEMailInspx,32000);
				vGaveBodyTriggerActiveTooLongError = true;
			}
		}
		*/
		if (vLocalSystemData->vUseSamplingFIFO) //clear the FIFO if belt has moved far enough since last item put in it
		if (vLocalConfigurationData->vDistanceFromSeamerObjectToScanTracTrigger)  //have a distance configure
		if (vLocalSystemData->vSamplingFIFOHead[cFillerSamplingType] != vLocalSystemData->vSamplingFIFOTail[cFillerSamplingType]) //something in the FIFO
		{
			int TempItemsLiftInFIFO = vLocalSystemData->vSamplingFIFOHead[cFillerSamplingType] - vLocalSystemData->vSamplingFIFOTail[cFillerSamplingType];
			if (TempItemsLiftInFIFO < 0)
				TempItemsLiftInFIFO = TempItemsLiftInFIFO + vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType];

			WORD TempDistanceSinceLastItemWentIntoFIFO = BPSubtract(vLocalSystemData->vCurrentBeltPosition, vLocalSystemData->vBeltPositionLastPutItemInSamplingFIFO);
			if (TempDistanceSinceLastItemWentIntoFIFO > vLocalConfigurationData->vDistanceFromSeamerObjectToScanTracTrigger * vGlobalPixelsPerUnit)
			{
				vMainWindowPointer->ClearValveMonitoringFIFO();
				vLocalSystemData->vCurrentSampleDataCount[cFillerSamplingType] = 0;
				vLocalSystemData->vCurrentSampleDataCount[cSeamerSamplingType] = 0;
				vLocalSystemData->vPreviousFillerHeadPutInContainer = 0;

		
				ReportErrorMessage("Notice-Sampling FIFO re-synchronized, continue inspecting. FIFO had " + dtoa(TempItemsLiftInFIFO, 0), cWriteToLog, 0);  

				//if (PasswordOK(cTemporaryInspxPassword,false))
				//	ReportErrorMessage("Notice-Sampling FIFO re-synchronized, continue inspecting. FIFO had " + dtoa(TempItemsLiftInFIFO, 0), cEMailMaintenance, 0);  
				//else
				//if (vLocalSystemData->vLogFile.vLogSerialData)
				//	vLocalSystemData->vLogFile.WriteToLogFile("Notice-Sampling FIFO re-synchronized, continue inspecting. FIFO had " + dtoa(TempItemsLiftInFIFO, 0),cDebugMessage);
			}
		}
		else
		{
		//brh temp diags
			WORD TempDistanceSinceLastItemWentIntoFIFO = BPSubtract(vLocalSystemData->vCurrentBeltPosition, vLocalSystemData->vBeltPositionLastPutItemInSamplingFIFO);
			if (vLocalSystemData->vBeltPositionLastPutItemInSamplingFIFO)
			if (TempDistanceSinceLastItemWentIntoFIFO > vLocalConfigurationData->vDistanceFromSeamerObjectToScanTracTrigger * vGlobalPixelsPerUnit)
			{
				vLocalSystemData->vBeltPositionLastPutItemInSamplingFIFO = 0;
				//diagnostics
				ReportErrorMessage("Notice-Gap in Production and Sampling was in sync", cWriteToLog, 0);  
				//if (PasswordOK(cTemporaryInspxPassword,false))
				//	ReportErrorMessage("Notice-Gap in Production and Sampling was in sync", cEMailMaintenance, 0);  
				//else
				//if (vLocalSystemData->vLogFile.vLogSerialData)
				//	vLocalSystemData->vLogFile.WriteToLogFile("Notice-Gap in Production and Sampling was in sync",cDebugMessage);
			}
		}
	}

	if (vOldActualEncoderRate != vLocalSystemData->vActualEncoderRate)
	{
		if (!vLocalSystemData->vActualEncoderRate)
		if (vGlobalImageAquisitionThread)
		if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		{
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("Actual Encoder Stopped",cDebugMessage);
		}
		vOldActualEncoderRate = vLocalSystemData->vActualEncoderRate;
	}

	//if (vLocalSystemData->vCurrentBeltPosition != TempBeltPosition)
	//if (vLocalSystemData->vRadiationDoorManualControlRegister == cRadiationShutterAutoControl)
	//if (vLocalSystemData->vSystemRunMode != cCalibratingDetectorOffsetMode)
	//	SetRadiationDoorManualControl(cRadiationShutterAutoControl);

	//if (!((vLocalConfigurationData->vDemoMode)
	//	|| (vLocalConfigurationData->vSimulateLightControlBoard)))
	if (!vLocalConfigurationData->vDemoMode)
		vLocalSystemData->vCurrentBeltPosition = TempBeltPosition;
	
	if (vLocalConfigurationData->vSimulateLightControlBoard)
	if (!vLocalConfigurationData->vUseRealEncoder) //only use if not using a real encoder
		vLocalSystemData->vConveyorEncoderRate = vLocalSystemData->vActualEncoderRate / 16;

	double TempStartTime1 = CIFCOS::GetSystimeMicrosecs();
	if (vLocalSystemData->vKeepExternalDetectorsEnabled)
		TestContainersForEject(TempBeltPosition);
	CheckContainers(vLocalSystemData->vCurrentBeltPosition);

	double TempEndTime1 = CIFCOS::GetSystimeMicrosecs();
	TempEndTime1 = TempEndTime1 - TempStartTime1;
	if (vLocalSystemData->vMaximumExternalDetectorTime < TempEndTime1)
		vLocalSystemData->vMaximumExternalDetectorTime = TempEndTime1;

	if (vBeltPositionToEnableTriggerAfterTemporaryDisable)  //disabled triggers to clear tunnel to resync containers, is tunnel clear now.
	{
		if (vBeltPositionToEnableTriggerAfterTemporaryDisable < vBeltPositionDisabledTrigger)
		{  //The Timeout position has wraped around from BodyTrigger Position
			if ((vLocalSystemData->vCurrentBeltPosition > vBeltPositionToEnableTriggerAfterTemporaryDisable) && 
				(vLocalSystemData->vCurrentBeltPosition < vBeltPositionDisabledTrigger))
					EnableTriggerAfterTemporaryDisable(true); //tunnel should be clear, re-enable triggers
		}
		else
		{
			if ((vLocalSystemData->vCurrentBeltPosition > vBeltPositionToEnableTriggerAfterTemporaryDisable) || 
				(vLocalSystemData->vCurrentBeltPosition < vBeltPositionDisabledTrigger))
					EnableTriggerAfterTemporaryDisable(true); //tunnel should be clear, re-enable triggers
		}
	}

	if (vLocalSystemData->vFirstContainerShouldNotTriggerSensorYet)
	if (vLocalConfigurationData->vResynchronizeEjectors)  //reset the container count
	if ((vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem) || (vLocalSystemData->vSystemRunMode == cRunningSystemMode))
	{
		if (BPSubtract(vLocalSystemData->vCurrentBeltPosition, vLocalSystemData->vBeltPositionTunnelShouldBeClearTo) < 20000) //current belt position is passed the point sensor 1 should not have trigger from a container in the tunnel
		{
			vLocalSystemData->vFirstContainerShouldNotTriggerSensorYet = false;
			if (vLocalSystemData->vLogFile.vLogSerialData)
				WriteToLogFileWithBeltPosition("Belt has traveled past sensor for Resynchronizing Ejector: " + dtoa(vLocalSystemData->vCurrentBeltPosition, 0), vLocalSystemData->vCurrentBeltPosition);

			//enable ejector resync sensors
			vLocalSystemData->vKeepExternalDetectorsEnabled = true;
			vLocalSystemData->vCurrentExternalDetectorMask = vLocalSystemData->vCurrentExternalDetectorMask | 2; //enable photo eye on Aux Det 2 for ejector 1

			if (vLocalConfigurationData->vEjector[1].vEnabled)
				vLocalSystemData->vCurrentExternalDetectorMask = vLocalSystemData->vCurrentExternalDetectorMask | 0x20; //enable photo eye on Eject Confirmer 1 for ejector 2

			if (vLocalConfigurationData->vResynchronizeEjectors)
				vLocalSystemData->vCurrentAlwaysOnExternalDetectorMask = vLocalSystemData->vCurrentExternalDetectorMask;

			tSerialCommand TempCommand;
			TempCommand[0] = 0x1C;
			TempCommand[1] = 0x02;  //group 2 is Auxiliary Detectors
			TempCommand[2] = 0x00;

			if (vLocalSystemData->vKeepExternalDetectorsEnabled)
				TempCommand[3] = vLocalSystemData->vCurrentExternalDetectorMask;
			else
				TempCommand[3] = vLocalSystemData->vCurrentAlwaysOnExternalDetectorMask;

			SendSerialCommand(TempCommand);
			Sleep(cSendCommandSleepTimeSetup);
		}
	}
}

void CSerialPort::ProcessEjectNotification(WORD TempBeltPosition, BYTE TempEjector)
{
	TempEjector = TempEjector & 0x1F; //mask off upper 3 bits as upper 2 are diverter control and bit 5 is not used

	vLocalSystemData->vLastEjectedPosition = TempBeltPosition;
	vLocalSystemData->vLastEjectedEjector = TempEjector;

	//find which container should have ejected, them mark it.
	if ((vLocalConfigurationData->vEjectContainersNotInspected) &&
		(TempEjector == vLocalConfigurationData->vDiverterEjector) &&
		(vLocalConfigurationData->vEjector[vLocalConfigurationData->vDiverterEjector - 1].vEnabled) &&
		(vLocalConfigurationData->vEjector[vLocalConfigurationData->vDiverterEjector - 1].vDigitalInputLine) &&
		(vLocalSystemData->vSystemRunMode == cRunningSystemMode))
	{//if diverter ejector fired, then turn it on now.
		SendEjectorOnOffCommand(vLocalConfigurationData->vDiverterEjector,1); //turn Diverter on to divert all containers down good shoot
		vLocalSystemData->vDiverterOn = true;
		if (vLocalSystemData->vLogFile.vLogSerialData)
			WriteToLogFileWithBeltPosition("Turn Diverter On",vLocalSystemData->vCurrentBeltPosition);
	}
	else
	{
		if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
		{
			WORD TempContainerCount = vLocalSystemData->vContainerCollection->GetCount();
			if (vLocalSystemData->vContainerCollection->GetCount() > 0)
			{
				POSITION TempIndex = vLocalSystemData->vContainerCollection->GetHeadPosition();
				bool TempDone = false;
				CContainer *TempContainer = vLocalSystemData->vContainerCollection->GetAt(TempIndex);//GetHead();
				//Check Each Containter until get one that was ejected
				while ((TempContainer != NULL) && (!TempDone))
				{
					if (vLocalConfigurationData->vResynchronizeEjectors)
					{
						if (TempContainer->vContainerNumberToEject == TempEjector) //TempEjector is actually the container number to eject
						{
 							TempDone = true;
							if (vLocalSystemData->vInAutoSetup)
							{
								WORD TempBeltOffsetToThisEjector = BPSubtract(TempBeltPosition, TempContainer->vBodyTriggerPosition);
								if (TempBeltOffsetToThisEjector > vGlobalCurrentProduct->vEjectorDistanceFromTriggerInInches[0])
								{
									vGlobalCurrentProduct->vEjectorDistanceFromTriggerInInches[0] = TempBeltOffsetToThisEjector;

									if (vLocalSystemData->vLogFile.vLogSerialData)
										WriteToLogFileWithBeltPosition("Resync Ejectors 1 Last Ejector Position set to: " + dtoa(TempBeltOffsetToThisEjector, 0), vLocalSystemData->vCurrentBeltPosition);
								}
							}
						}
 						else
						{
							//get next container to check
							if (TempIndex)
								TempContainer = vLocalSystemData->vContainerCollection->GetNext(TempIndex);
							else
								TempContainer = NULL;
						}
					}
					else
					{ //normal belt position method of ejector timing
						if ((TempContainer->vEjectPosition[TempEjector - 1] == TempBeltPosition) && ((TempContainer->vEjectorNumber >> (TempEjector - 1)) & 1))
 							TempDone = true;
 						else
						{
							//get next container to check
							if (TempIndex)
								TempContainer = vLocalSystemData->vContainerCollection->GetNext(TempIndex);
							else
								TempContainer = NULL;
						}
					}
				}
				if (TempDone)
				{
					TempContainer->vGotEjectedNotice++;
					//if (TempContainer->vGotEjectedNotice >= TempContainer->vNumberOfEjectors)
					{
						if (!vLocalConfigurationData->vEjector[TempEjector - 1].vConfirmEnabled)
						{  //if got eject notice, and don't have a confirm, indicate this one done
							TempContainer->vDataComplete = TempContainer->vDataComplete + 2;
							if (vLocalSystemData->vLogFile.vLogSerialData)
								WriteToLogFileWithBeltPosition("Processed Eject Notification, Data Complete 2 c: " + dtoa(TempContainer->vBodyTriggerPosition, 0), vLocalSystemData->vCurrentBeltPosition);
							//ReportErrorMessage("Processed Eject Notification, Data Complete 2 c: " + dtoa(TempContainer->vContainerNumber, 0), cWriteToLog,0);
						}
						else
						{
							TempContainer->vDataComplete++;
							if (vLocalSystemData->vLogFile.vLogSerialData)
								WriteToLogFileWithBeltPosition("Processed Eject Notification, Need Confirm for c: " + dtoa(TempContainer->vBodyTriggerPosition, 0), vLocalSystemData->vCurrentBeltPosition);

								//brh for testing only
							if (vLocalConfigurationData->vSimulateLightControlBoard)
								HandleExternalDetectorLineChange(vLocalSystemData->vCurrentBeltPosition + 400, 0x20);

						}
					}
				}
				else
				if ((vLocalSystemData->vDiverterDeactivatePosition[TempEjector]) && (vLocalSystemData->vDiverterDeactivatePosition[TempEjector] == TempBeltPosition))
				{
					vLocalSystemData->vDiverterDeactivatePosition[TempEjector] = 0;
					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("Processed Eject Notification, Diverter: " + dtoa(TempEjector, 0) + " De-activated at: " + dtoa(TempBeltPosition, 0),cDebugMessage);
				}
				else
				if ((TempEjector > 0) && (vLocalSystemData->vDiverterDeactivatePosition[TempEjector - 1]) && (vLocalSystemData->vDiverterDeactivatePosition[TempEjector - 1] == TempBeltPosition))
				{
					vLocalSystemData->vDiverterDeactivatePosition[TempEjector - 1] = 0;
					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("Processed Eject Notification, Diverter: " + dtoa(TempEjector, 0) + " De-activated at: " + dtoa(TempBeltPosition, 0),cDebugMessage);
				}
				else
				if (vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
				if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
				if (!((vGlobalCurrentProduct->vMultiLaneNumberOfLanes) && (vGlobalCurrentProduct->vMultiLaneNumberOfLanes))) //multi lane ejecting adjacent lanes getting adjacent ones, so will get extra
				{
					vLocalSystemData->vProcessingErrorCount++;
					ReportErrorMessage("Processing Error, ProcessEjectNotice, Can't Find Container, Ejector: " + dtoa(TempEjector, 0), cWriteToLog,32000);
					ReportErrorMessage("Ejector " + dtoa(TempEjector, 0) + " Command Too Late 2", cEMailInspx,32000);
					ReportErrorMessage("Also Check Image Width and processing Time.", cWriteToLog,32000);
					ReportErrorMessage("Eject Decision Too Late! Adjust setup or Call Service.", cError, 0);
				}
			}
			else
			if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
			if (vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
			{
				vLocalSystemData->vProcessingErrorCount++;
				ReportErrorMessage("Processing Error, ProcessEjectNotice, No Containers", cEMailInspx,32000);
			}
			vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
		}
		else
			ReportErrorMessage("Error-Serial Thread Lock Failed 3", cEMailInspx,32000);
	}
}

void CSerialPort::ProcessResynchronizedEjectNotification(WORD TempBeltPosition, BYTE TempContainerEjectNumber)
{
	vLocalSystemData->vLastEjectedPosition = TempBeltPosition;

	//find which container should have ejected, them mark it.
	if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
	{
		WORD TempContainerCount = vLocalSystemData->vContainerCollection->GetCount();
		if (vLocalSystemData->vContainerCollection->GetCount() > 0)
		{
			POSITION TempIndex = vLocalSystemData->vContainerCollection->GetHeadPosition();
			bool TempDone = false;
			CContainer *TempContainer = vLocalSystemData->vContainerCollection->GetAt(TempIndex);//GetHead();
			//Check Each Containter until get one that was ejected
			while ((TempContainer != NULL) && (!TempDone))
			{
				if (TempContainer->vContainerNumberToEject == TempContainerEjectNumber) //TempEjector is actually the container number to eject
				{
 					TempDone = true;
					if (vLocalSystemData->vInAutoSetup)
					{
						WORD TempBeltOffsetToThisEjector = BPSubtract(TempBeltPosition, TempContainer->vBodyTriggerPosition);
						if (TempBeltOffsetToThisEjector > vGlobalCurrentProduct->vEjectorDistanceFromTriggerInInches[0])
						{
							vGlobalCurrentProduct->vEjectorDistanceFromTriggerInInches[0] = TempBeltOffsetToThisEjector;

							if (vLocalSystemData->vLogFile.vLogSerialData)
								WriteToLogFileWithBeltPosition("Resync Ejectors 2 Last Ejector Position set to: " + dtoa(TempBeltOffsetToThisEjector, 0), vLocalSystemData->vCurrentBeltPosition);
						}
					}
				}
 				else
				{
					//get next container to check
					if (TempIndex)
						TempContainer = vLocalSystemData->vContainerCollection->GetNext(TempIndex);
					else
						TempContainer = NULL;
				}
			}
			if (TempDone)
			{
				TempContainer->vGotEjectedNotice++;
				//if (TempContainer->vGotEjectedNotice >= TempContainer->vNumberOfEjectors)
				{
					//if (!vLocalConfigurationData->vEjector[TempEjector - 1].vConfirmEnabled)
					{  //if got eject notice, and don't have a confirm, indicate this one done
						TempContainer->vDataComplete = TempContainer->vDataComplete + 2;
						if (vLocalSystemData->vLogFile.vLogSerialData)
							WriteToLogFileWithBeltPosition("Processed Eject Notification r, Data Complete c: " + dtoa(TempContainer->vContainerNumber, 0), TempContainer->vBodyTriggerPosition);
					}
					//else
					//{
					//	TempContainer->vDataComplete++;
					//	if (vLocalSystemData->vLogFile.vLogSerialData)
					//		WriteToLogFileWithBeltPosition("Processed Eject Notification, Need Confirm for c: " + dtoa(TempContainer->vContainerNumber, 0), TempContainer->vBodyTriggerPosition);
					//}
				}
			}
			else
			if (vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
			if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
			{
				vLocalSystemData->vProcessingErrorCount++;
				ReportErrorMessage("Processing Error, ProcessEjectNotice, Can't Find Container", cWriteToLog,32000);
				//ReportErrorMessage("Eject Command Too Late 2", cEMailInspx,32000);
				//ReportErrorMessage("Also Check Image Width and processing Time.", cWriteToLog,32000);
				//ReportErrorMessage("Eject Decision Too Late! Adjust setup or Call Service.", cError, 0);
				//vTooLateToEjectErrorCount
			}
		}
		else
		if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
		if (vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
		{
			vLocalSystemData->vProcessingErrorCount++;
			ReportErrorMessage("Processing Error, ProcessEjectNotice, No Containers", cEMailInspx,32000);
		}
		vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
	}
	else
		ReportErrorMessage("Error-Serial Thread Lock Failed 3", cEMailInspx,32000);
}

void CSerialPort::SendXRayPowerOnOffToUController(BYTE TempOn)
{
	if (TempOn)
	{
		tSerialCommand TempCommand;
		TempCommand[0] = 0x23;
		TempCommand[1] = 0x98;  //FPGA address 0x9008
		TempCommand[2] = 0x00;
		TempCommand[3] = 0x03;  //latch relay enable and Latch On, on Spellman
		if (!vLocalConfigurationData->vDemoMode)
			SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTime);

		Sleep(cSendCommandSleepTime + 700);

		TempCommand[0] = 0x23;
		TempCommand[1] = 0x98;  //FPGA address 0x9008
		TempCommand[2] = 0x00;
		TempCommand[3] = 0x01;  //unlatch relay Latch On, on Spellman
		if (!vLocalConfigurationData->vDemoMode)
			SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTime);
	}
	else
	{
		tSerialCommand TempCommand;
		TempCommand[0] = 0x23;
		TempCommand[1] = 0x98;  //FPGA address 0x9008
		TempCommand[2] = 0x00;
		TempCommand[3] = 0x00;  //un-latch relay On and Off on Spellman, and XR-50
		SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTime);
	}
}

void CSerialPort::HandleExternalDetectorLineChange(WORD TempBeltPosition, BYTE TempDigitalLines)
{
	//if (vLocalSystemData->vKeepExternalDetectorsEnabled)
	//	ReportErrorMessage("Auxiliary Detector changed to: " + ByteToHex(TempDigitalLines) + " At: " + dtoa(TempBeltPosition,0) + ", " +
	//	dtoa((TempBeltPosition - vLocalSystemData->vLastBodyTriggerPosition) / vGlobalPixelsPerUnit,2) + " " + vLocalConfigurationData->vUnitsString, cAction,0);

	//if (TempBeltPosition != vLocalSystemData->vLastExternalDetectorPosition) //|| (TempDigitalLines != vLocalSystemData->vLastExternalDetectorLineState))
	{
		//mask disabled bits out so only active bits can be a one
		TempDigitalLines = (TempDigitalLines & vLocalSystemData->vCurrentExternalDetectorMask);

		TempDigitalLines = (TempDigitalLines ^ vLocalSystemData->vCurrentExternalDetectorActiveLowMask);  //make all bits active high

		BYTE TempBitsActive = TempDigitalLines & (TempDigitalLines ^ vLocalSystemData->vLastExternalDetectorLineState); //TempBitsActive are only the bits that are different and it is always active high

		vLocalSystemData->vLastExternalDetectorLineState = TempDigitalLines;
		vLocalSystemData->vLastExternalDetectorPosition = TempBeltPosition;

		if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
		{
			if (vLocalSystemData->vLogFile.vLogSerialData)
				WriteToLogFileWithBeltPosition("Aux Detector changed. Masked: " + ByteToHex(TempDigitalLines) + ", Active: " + ByteToHex(TempBitsActive),TempBeltPosition);

			if (vDiscoverModeDataIndex) //only record transitions if got a trigger so that index is not zero
			{
				BYTE TempDiscoverModeBits = vLocalSystemData->vLastExternalDetectorLineState & vLocalConfigurationData->vExternalDetector[vDiscoverModeActiveDetectorNumber].vLineBitMask;

				if (vDiscoverModeDataIndex < cNumberOfDiscoverSamples)
				if (vLastDiscoverModeBits != TempDiscoverModeBits)
				{
					vLastDiscoverModeBits = TempDiscoverModeBits;

					if (vDiscoverModeDataIndex < cNumberOfDiscoverSamples)
					{
						vLocalSystemData->vLogFile.WriteToLogFile("Ext. Det. Discover Mode Index: " + dtoa(vDiscoverModeDataIndex),cDebugMessage);
					
						if (TempDiscoverModeBits)
						{
							vDiscoverModeDataIsActive[vDiscoverModeDataIndex] = true;
							if (vLocalSystemData->vLogFile.vLogSerialData)
								WriteToLogFileWithBeltPosition("Ext. Det. Discover Mode setting active = true",TempBeltPosition);
						}
						else
						{
							vDiscoverModeDataIsActive[vDiscoverModeDataIndex] = false;
							if (vLocalSystemData->vLogFile.vLogSerialData)
								WriteToLogFileWithBeltPosition("Ext. Det. Discover Mode setting active = false",TempBeltPosition);
						}
						vDiscoverModeDataBeltPosition[vDiscoverModeDataIndex] = TempBeltPosition;

						if (vLocalSystemData->vLogFile.vLogSerialData)
							WriteToLogFileWithBeltPosition("Ext. Det. Discover Mode setting position = " + dtoa(vDiscoverModeDataBeltPosition[vDiscoverModeDataIndex]),TempBeltPosition);

						if (vDiscoverModeDataIndex < cNumberOfDiscoverSamples - 1)
							vDiscoverModeDataIndex++;
					}
				}
			}
			//if (vLocalSystemData->vLogFile.vLogSerialData)
			//{
			//	WriteToLogFileWithBeltPosition("After Mask changed: " + ByteToHex(TempDigitalLines),TempBeltPosition);
			//}

			BYTE TempBitsActive = TempDigitalLines;
			if (vGlobalCurrentProduct	!= NULL)   //you have a product
			//if (TempDigitalLines)
			//if (vLocalSystemData->vLastExternalDetectorLineState != TempDigitalLines) //if the lines changed
			{
				if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
				{
					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("Test for Eject Confirm/Aux. Det. change, bits: " + ByteToHex(TempBitsActive), cDebugMessage);
					if (vLocalSystemData->vEjectConfirmerUsed) //check for eject confirm
					{
						if (TempBitsActive)
						{
							if (vLocalSystemData->vFirstContainerShouldNotTriggerSensorYet)  //if the aux detector in front of the ejector triggers before enough belt went by, then give warning message
							if (vLocalConfigurationData->vResynchronizeEjectors)  //should not get this as don't enable sensors until ready, but may be a container is in the few inches of not sure area
							if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
							if (TempBitsActive & 1)
							{
								ReportErrorMessage("Critical Problem - Tunnel was not cleared before started inspection so all resynchronizing ejects will eject the wrong container until you make a gap of at least 10 feet to resynchronize", cCritical, 0);
								vLocalSystemData->vEjectorsNotSynchronized = true;
							}

							BYTE TempLoop = 0;
							for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
							if (vLocalConfigurationData->vEjector[TempLoop].vConfirmLineBitMask & TempBitsActive) //if confirmer for this ejector is active, set that it is active
							vLocalSystemData->vEjectConfirmerActive[TempLoop] = 1; //if eject confirmer active more than 5 seconds that eject tray must be full as sensor is blocked

							//find which container waiting for eject confirm, them mark it.
							//WORD TempCount = vLocalSystemData->vContainerCollection->GetCount();
							bool TempDone = false;
							if (vLocalSystemData->vContainerCollection->GetCount() > 0)
							{
								POSITION TempIndex = vLocalSystemData->vContainerCollection->GetHeadPosition();
								CContainer *TempContainer = vLocalSystemData->vContainerCollection->GetAt(TempIndex);//GetHead();

								if (TempIndex)
									TempContainer = vLocalSystemData->vContainerCollection->GetNext(TempIndex);
								else
									TempContainer = NULL;
								//Check Each Containter until get one that was ejected
								while ((TempContainer != NULL) && (!TempDone))
								{
									bool TempDataComplete = (TempContainer->vDataComplete >= (TempContainer->vNumberOfEjectors * 2));

									if ((TempContainer->vGotEjectedNotice) && (!TempDataComplete) && (TempContainer->vEjectorNumber))
									{
										//make sure bit changed is confirm bit for this container's ejector
										//if (vGlobalEjectorConfirmMask[TempContainer->vEjectorNumber - 1])
										//if (TempBitsActive & vGlobalEjectorConfirmMask[TempContainer->vEjectorNumber - 1])
										{
											if (vLocalSystemData->vLogFile.vLogSerialData)
												WriteToLogFileWithBeltPosition("Found Container for Ejector: " + dtoa(TempContainer->vEjectorNumber - 1, 0) + 
												"  Confirm, Data Complete c: " + dtoa(TempContainer->vContainerNumber,0),TempContainer->vBodyTriggerPosition);
													
											//vLocalSystemData->vEjectConfirmerActive[TempContainer->vEjectorNumber - 1] = 1; //if eject confirmer active more than 5 seconds that eject tray must be full as sensor is blocked

											//vLocalSystemData->vEjectorConfirmCount[TempEjectorNumber]++;
											TempContainer->vDataComplete++;
											TempContainer->vGotEjectConfirmedNotice = true;
											//Turn bit just handled off with exclusive or, 
											//and see if any other bits to process
											//TempBitsActive = TempBitsActive ^ vGlobalEjectorConfirmMask[TempContainer->vEjectorNumber - 1];
											//if (!TempBitsActive) //if NO more bits set mark done
												TempDone = true;
										}
									}
									//get next container to check
									if (TempIndex)
										TempContainer = vLocalSystemData->vContainerCollection->GetNext(TempIndex);
									else
										TempContainer = NULL;
								}
							}
						}
						else
						for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
							vLocalSystemData->vEjectConfirmerActive[TempLoop] = 0;
					}
					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("Test for Eject Confirm/Aux. Det. change, bits: " + ByteToHex(TempBitsActive), cDebugMessage);

					if (TempBitsActive & vLocalSystemData->vSampleEjectNextContainerBitMap) 
					{
						TempBitsActive = TempBitsActive ^ vLocalSystemData->vSampleEjectNextContainerBitMap; //clear the bit because we are handling it
						vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSeries[cRandomSamplingType]++;
						ReportErrorMessage("Auxiliary Input to Sample Next Container, Setup to Eject Next Container", cUserAction, 0);
					}

					//SEAMER INDEX AUX DETECTOR
					if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType])
					{
						if (TempBitsActive & vLocalSystemData->vSeamerIndexPulseBitMap) //if got an index pulse from filler
						{
							TempBitsActive = TempBitsActive ^ vLocalSystemData->vSeamerIndexPulseBitMap; //clear the bit because we are handling the index pulse
							HandleSeamerIndexPulse();
						}

						//FILLER POCKET SENSOR (Timing Sensor)
						if (vLocalSystemData->vUseSamplingFIFO)
						if (TempBitsActive & vLocalSystemData->vSeamerPocketSensorBitMap) //a container just left the seamer Object, so increment the current head numbers and put in the FIFOs for filler and seamer
						{
							TempBitsActive = TempBitsActive ^ vLocalSystemData->vSeamerPocketSensorBitMap; //clear the bit because we are handling the index pulse
							if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
								HandlePocketSensorPulse();
						}
					}

					//SEAMER Object AUX DETECTOR  (Object Sensor)
					if (vLocalSystemData->vUseSamplingFIFO)
					{
						if (TempBitsActive & vLocalSystemData->vSeamerObjectPulseBitMap) //a container just left the seamer Object, so increment the current head numbers and put in the FIFOs for filler and seamer
						{
							TempBitsActive = TempBitsActive ^ vLocalSystemData->vSeamerObjectPulseBitMap; //clear the bit because we are handling the index pulse
							if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
								HandleSeamerObjectPulse();
						}

						//FILLER INDEX AUX DETECTOR
						if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
						if (vLocalSystemData->vValveData)
						{
							if (TempBitsActive & vLocalSystemData->vFillerIndexPulseBitMap) //if got an index pulse from filler
							{
								TempBitsActive = TempBitsActive ^ vLocalSystemData->vFillerIndexPulseBitMap; //clear the bit because we are handling the index pulse

								HandleFillerIndexPulse();
							}
						}
					}

					//if (vLocalSystemData->vLogFile.vLogSerialData)
					//	vLocalSystemData->vLogFile.WriteToLogFile("*Sampling-Bits Active after Sampling Index: " + dtoa(TempBitsActive, 0),cDebugMessage);

					if (vLocalSystemData->vEjectConfirmerUsed) //check for eject confirm
					{
						if (TempBitsActive)
						{
							//find which container waiting for eject confirm, them mark it.
							//WORD TempCount = vLocalSystemData->vContainerCollection->GetCount();
							bool TempDone = false;
							if (vLocalSystemData->vContainerCollection->GetCount() > 0)
							{
								if (vLocalSystemData->vLogFile.vLogSerialData)
									vLocalSystemData->vLogFile.WriteToLogFile("Check for eject confirm.  Containers in tunnel: " + dtoa(vLocalSystemData->vContainerCollection->GetCount(), 0),cDebugMessage);

								POSITION TempIndex = vLocalSystemData->vContainerCollection->GetHeadPosition();
								CContainer *TempContainer = vLocalSystemData->vContainerCollection->GetAt(TempIndex);//GetHead();

								if (TempIndex)
									TempContainer = vLocalSystemData->vContainerCollection->GetNext(TempIndex);
								else
									TempContainer = NULL;
								//Check Each Containter until get one that was ejected
								while ((TempContainer != NULL) && (!TempDone))
								{
									bool TempDataComplete = (TempContainer->vDataComplete >= (TempContainer->vNumberOfEjectors * 2));

									if ((TempContainer->vGotEjectedNotice) && (!TempDataComplete) && (!TempContainer->vGotEjectConfirmedNotice) && (TempContainer->vEjectorNumber))
									{
										BYTE TempEjectorLoop = 0;
										//for (BYTE TempEjectorLoop = 0; TempEjectorLoop < cNumberOfEjectors; TempEjectorLoop++)
										{
											//brh temp comment out because beechnut forte 71643 does not confirm on lanes 3 and 4, but does on lanes 1 and 2
											//if (TempContainer->vEjectorNumber & (1 << (TempEjectorLoop - 1))) //found ejector number used to eject this container
											//if (vGlobalEjectorConfirmMask[TempEjectorLoop])
											//if (TempBitsActive & vGlobalEjectorConfirmMask[TempEjectorLoop])
											//if (TempContainer->vEjectPosition[TempEjectorLoop] == TempBeltPosition)
											{
												if (vLocalSystemData->vLogFile.vLogSerialData)
													WriteToLogFileWithBeltPosition("Found Container for Ejector: " + dtoa(TempEjectorLoop + 1, 0) + 
													"  Confirm, Data Complete c: " + dtoa(TempContainer->vBodyTriggerPosition, 0), vLocalSystemData->vCurrentBeltPosition);
													
												vLocalSystemData->vEjectConfirmerActive[TempEjectorLoop] = 1; //if eject confirmer active more than 5 seconds that eject tray must be full as sensor is blocked

												//vLocalSystemData->vEjectorConfirmCount[TempEjectorNumber]++;
												TempContainer->vDataComplete++;
												TempContainer->vGotEjectConfirmedNotice = true;
												//Turn bit just handled off with exclusive or, 
												//and see if any other bits to process
												//TempBitsActive = TempBitsActive ^ vGlobalEjectorConfirmMask[TempEjectorLoop];
												//if (!TempBitsActive) //if NO more bits set mark done
													TempDone = true;
											}
										}
									}
									//get next container to check
									if (TempIndex)
										TempContainer = vLocalSystemData->vContainerCollection->GetNext(TempIndex);
									else
										TempContainer = NULL;
								}
							}

							//set bits active if any are so will count for eject table full
							for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
							if (vGlobalEjectorConfirmMask[TempLoop]) //if have an eject confirmer
							if (TempBitsActive & vGlobalEjectorConfirmMask[TempLoop]) //if the eject confirmer is active
								vLocalSystemData->vEjectConfirmerActive[TempLoop] = 1; //set active so that if eject confirmer active more than 5 seconds that eject tray must be full as sensor is blocked
						}
						else
						for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
							vLocalSystemData->vEjectConfirmerActive[TempLoop] = 0;
					}
					if (vLocalSystemData->vContainerCollection->GetCount() > 0)
					if (vLocalSystemData->vExternalDetectorDecisionEnd)
					if (TempBitsActive != 0) //check for Auxiliary Detectors
					{
						if (vLocalSystemData->vLogFile.vLogSerialData)
						{
							vLocalSystemData->vLogFile.WriteToLogFile("Test for Auxiliary Detector change",cDebugMessage);
						}
						bool TempDone = false;
						POSITION TempIndex = vLocalSystemData->vContainerCollection->GetHeadPosition();
						CContainer *TempContainer = vLocalSystemData->vContainerCollection->GetAt(TempIndex);//GetHead();

						if (TempIndex)
							TempContainer = vLocalSystemData->vContainerCollection->GetNext(TempIndex);
						else
							TempContainer = NULL;
						
						while ((TempContainer != NULL) && (!TempDone))
						{
							//if we have not gotten too the starting point then we are done checking
							//if the start of this container vLocalSystemData->vExternalDetectorDecisionStart
							//is before the TempBeltPosition then stop checking cause all other containers
							//start even later
							if (!IsPastBeltPostion(TempBeltPosition, BPAdd(vLocalSystemData->vExternalDetectorDecisionStart, TempContainer->vBodyTriggerPosition), TempContainer->vBodyTriggerPosition))
							{
								TempDone = true;
								if (vLocalSystemData->vLogFile.vLogSerialData)
									WriteToLogFileWithBeltPosition("HandleExternalDetectorLineChange past start BP: "
										+ dtoa(BPAdd(vLocalSystemData->vExternalDetectorDecisionStart, TempContainer->vBodyTriggerPosition), 0),	TempContainer->vBodyTriggerPosition);
							}
									
							if (!TempDone)
							for (BYTE TempLoop = 0; TempLoop < vLocalSystemData->vLastExternalDetectorUsed; TempLoop++)
							if (vGlobalCurrentProduct->vExternalDetectorEnable[TempLoop])
							if ((vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled) || (vLocalSystemData->vKeepExternalDetectorsEnabled))
							{
								//if this Auxiliary detectors bit mask is one that just went active now
								if ((TempBitsActive & vLocalConfigurationData->vExternalDetector[TempLoop].vLineBitMask))
								{

									if (IsPastBeltPostion(BPAdd(vLocalSystemData->vExternalDetectorDecisionEnd, TempContainer->vBodyTriggerPosition), TempBeltPosition, TempContainer->vBodyTriggerPosition))
									{
										TempDone = true;
										if (vLocalSystemData->vLogFile.vLogSerialData)
											WriteToLogFileWithBeltPosition("HandleExternalDetectorLineChange past End BP: "
												+ dtoa(BPAdd(vLocalSystemData->vExternalDetectorDecisionStart, TempContainer->vBodyTriggerPosition), 0),	TempContainer->vBodyTriggerPosition);
									}
									
									if (vLocalSystemData->vLogFile.vLogSerialData)
										WriteToLogFileWithBeltPosition("HandleExternalDetectorLineChange check Aux. Det: " + dtoa(vLocalConfigurationData->vExternalDetector[TempLoop].vLineBitMask) + 
										" c: " + dtoa(TempContainer->vBodyTriggerPosition, 0), vLocalSystemData->vCurrentBeltPosition);

									if (!TempContainer->vGotExternalDetectorNotice[TempLoop])
									//if this container is in the window active region
									if (IsBeltPositionInWindow(TempBeltPosition, TempContainer->vStartExternalDetectorPosition[TempLoop], TempContainer->vEndExternalDetectorPosition[TempLoop]))
									{
										if (vLocalSystemData->vLogFile.vLogSerialData)
										{
											WriteToLogFileWithBeltPosition("HandleExternalDetectorLineChange In Window", TempContainer->vBodyTriggerPosition);
										}

										vLocalSystemData->vExternalDetectorCount[TempLoop]++;
										TempContainer->vGotExternalDetectorNotice[TempLoop] = true;
										//if should eject, then do it
										if ((vGlobalCurrentProduct->vExternalDetectorMode[TempLoop] == 1) || (vGlobalCurrentProduct->vExternalDetectorMode[TempLoop] == 3))
										if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
										if (!TempContainer->vToBeEjected)
										{  
											if (vLocalSystemData->vLogFile.vLogSerialData)
												WriteToLogFileWithBeltPosition("HandleExternalDetectorLineChange 3 eject Aux. Det: " + dtoa(vLocalConfigurationData->vExternalDetector[TempLoop].vLineBitMask) + 
												" c: " + dtoa(TempContainer->vBodyTriggerPosition, 0), vLocalSystemData->vCurrentBeltPosition);

											EjectContainer(TempLoop + cEjectAuxilaryDetectorOffset, TempContainer, NULL);
											TempContainer->vExternalDetectorsDone = true;
											TempContainer->vToBeEjected = true;


											//save the reject image to display in rejects display
											vLocalSystemData->vITIPCDig->SaveThisRejectInBuffer(TempContainer, TempContainer->vContainerNumber, 
												0, vLocalSystemData->vCountToUseForEjectReason[TempLoop + cEjectAuxilaryDetectorOffset]);
										}
										if (vLocalSystemData->vLogFile.vLogSerialData)
											WriteToLogFileWithBeltPosition("Found Container for Auxiliary Detector: " + dtoa(TempLoop + 1) + " c: " + dtoa(TempContainer->vBodyTriggerPosition, 0), vLocalSystemData->vCurrentBeltPosition);

										//clear the Auxiliary Detector bit cause processed the container it was for
										TempBitsActive = TempBitsActive ^ vLocalConfigurationData->vExternalDetector[TempLoop].vLineBitMask;
										if (TempBitsActive == 0) //if have no more Auxiliary detector bits
											TempDone = true;
									}
								}
							}
							if (!TempDone)
							{//get next container to check
								if (TempIndex)
									TempContainer = vLocalSystemData->vContainerCollection->GetNext(TempIndex);
								else
									TempContainer = NULL;
							}
						}
					}
					/*
					if (TempBitsActive != 0) //did not find where bit changed belong to
					{
						vLocalSystemData->vProcessingErrorCount++;
						ReportErrorMessage("Processing Error, AuxiliaryDetLineChange or EjectConfirm, Can't Find Container", cEMailInspx,32000);
					}
					*/
					if (vLocalSystemData->vDoingBackupDetection)
					{
						for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
						if (vGlobalCurrentProduct->vExternalDetectorMode[TempLoop] == cBackupDetectorMode)
						if (vGlobalCurrentProduct->vExternalDetectorEnable[TempLoop])
						if (vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled)
						{
							//if this Auxiliary detectors bit is inactive, set last backup on indicator to off
							if ((TempDigitalLines & vLocalConfigurationData->vExternalDetector[TempLoop].vLineBitMask) == 0)
							{//Auxiliary Detector bit is inactive
								if (vLocalSystemData->vBackupDetectorOn[TempLoop] != 2)  // 2 = off
								{  //if bit was active, now inactive, record Conveyor position and set inactive
									vLocalSystemData->vLastBackupDetectorChangePosition[TempLoop] = TempBeltPosition;
									vLocalSystemData->vBackupDetectorOn[TempLoop] = 2;
									if (vLocalSystemData->vLogFile.vLogSerialData)
									{
										WriteToLogFileWithBeltPosition("Backup Cleared", TempBeltPosition);
									}
								}
							}
							else  //bit is active
							if (vLocalSystemData->vBackupDetectorOn[TempLoop] != 1)  // 1 = 0n  0 = not set
							{//last backup on indicator is off so set it on
								vLocalSystemData->vLastBackupDetectorChangePosition[TempLoop] = TempBeltPosition;
								vLocalSystemData->vBackupDetectorOn[TempLoop] = 1;
								if (vLocalSystemData->vLogFile.vLogSerialData)
								{
									WriteToLogFileWithBeltPosition("Backup Occured", TempBeltPosition);
								}
							}
						}
					}
					vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
				}
				else
					ReportErrorMessage("Error-Serial Thread Lock Failed 5", cEMailInspx,32000);
			}
		}
	}
}

void CSerialPort::TestContainersForEject(WORD TempCurrentBeltPosition)
{
	//check if Auxiliary detectors at end of range, so telling you status
	if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
	if (vLocalSystemData->vExternalDetectorDecisionEnd != 0)  //how far after BT is last aux. det.
	if (vLocalSystemData->vContainerCollection->GetCount() > 0)
	{
		if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
		{
			POSITION TempIndex = vLocalSystemData->vContainerCollection->GetHeadPosition();
			CContainer *TempContainer = vLocalSystemData->vContainerCollection->GetAt(TempIndex);//GetHead();
			if (TempIndex)
				TempContainer = vLocalSystemData->vContainerCollection->GetNext(TempIndex);
			else
				TempContainer = NULL;
			//Check Each Containter 
			while (TempContainer != NULL)
			{
				if (!TempContainer->vExternalDetectorsDone)
				if (!TempContainer->vToBeEjected)
				{//have Auxiliary detectors to analyze
					if (vLocalSystemData->vLogFile.vLogSerialData)
						WriteToLogFileWithBeltPosition("TestContainersForEject check Aux. Det. c: " + dtoa(TempContainer->vBodyTriggerPosition, 0), vLocalSystemData->vCurrentBeltPosition);

					if (IsPastBeltPostion(TempCurrentBeltPosition + 1, BPAdd(vLocalSystemData->vExternalDetectorDecisionEnd,TempContainer->vBodyTriggerPosition), TempContainer->vBodyTriggerPosition))
					{
						for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
						{
							//if got Auxiliary detector active, and reject when active is the mode
							if ((vGlobalCurrentProduct->vExternalDetectorEnable[TempLoop]) &&
								(vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled))
							{
								if (!TempContainer->vToBeEjected)
								if ((TempContainer->vGotExternalDetectorNotice[TempLoop]) &&
									((vGlobalCurrentProduct->vExternalDetectorMode[TempLoop] == 1) ||
									(vGlobalCurrentProduct->vExternalDetectorMode[TempLoop] == 3)))
								if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
								{
									EjectContainer(TempLoop + cEjectAuxilaryDetectorOffset,TempContainer, NULL);
									TempContainer->vExternalDetectorsDone = true;
									TempContainer->vToBeEjected = true;

									//save the reject image to display in rejects display
									vLocalSystemData->vITIPCDig->SaveThisRejectInBuffer(TempContainer, TempContainer->vContainerNumber, 
										0, vLocalSystemData->vCountToUseForEjectReason[TempLoop + cEjectAuxilaryDetectorOffset]);
								}
								//figure out if passed all Auxiliary detectors and if should eject or not
								if (!TempContainer->vToBeEjected)
								//if didn't get a detector, and mode is accept if get detector, then reject
								if ((!TempContainer->vGotExternalDetectorNotice[TempLoop]) &&
										((vGlobalCurrentProduct->vExternalDetectorMode[TempLoop] == 2) ||
										(vGlobalCurrentProduct->vExternalDetectorMode[TempLoop] == 4)))
								if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
								{
									EjectContainer(TempLoop + cEjectAuxilaryDetectorOffset,TempContainer, NULL);
									TempContainer->vExternalDetectorsDone = true;
									TempContainer->vToBeEjected = true;

									//save the reject image to display in rejects display
									vLocalSystemData->vITIPCDig->SaveThisRejectInBuffer(TempContainer, TempContainer->vContainerNumber, 
										0, vLocalSystemData->vCountToUseForEjectReason[TempLoop + cEjectAuxilaryDetectorOffset]);
								}
							}
						}
						TempContainer->vExternalDetectorsDone = true;
						if (vLocalSystemData->vLogFile.vLogSerialData)
						{
							WriteToLogFileWithBeltPosition("TestContainersForEject done checking Aux. Det. c: " + dtoa(TempContainer->vBodyTriggerPosition, 0), vLocalSystemData->vCurrentBeltPosition);
						}
					}	
					else
						TempIndex = 0; //stop checking containers once find one not past the end point
				}
				//get next container to check
				if (TempIndex)
					TempContainer = vLocalSystemData->vContainerCollection->GetNext(TempIndex);
				else
					TempContainer = NULL;
			}
			vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
		}
		else
			ReportErrorMessage("Error-Serial Thread Lock Failed 6", cEMailInspx,32000);
	}
	else
	if (vLocalSystemData->vLogFile.vLogSerialData)
	if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
	if (!vLocalSystemData->vKeepExternalDetectorsEnabled)
		WriteToLogFileWithBeltPosition("Error - TestContainersForEject No Containers ",0);
}


void CSerialPort::HandleBackupBodyTriggerChange(WORD TempBeltPosition, 
	BYTE TempBodyTriggerLines)
{

	if (vGlobalCurrentProduct	!= NULL)
	{
		//toggle active low bits so all bits are active high
		vLocalSystemData->vBackupBodyTriggerLines = TempBodyTriggerLines ^ vLocalSystemData->vCurrentBodyTriggerActiveLowMask;
		//mask disabled bits out so only active bits can be a one
		vLocalSystemData->vBackupBodyTriggerLines = 
			vLocalSystemData->vBackupBodyTriggerLines & vLocalSystemData->vBackupBodyTriggerMask;
		if (vLocalSystemData->vBackupBodyTriggerLines != vLocalSystemData->vOldBackupBodyTriggerLines)
		{
			vLocalSystemData->vOldBackupBodyTriggerLines = vLocalSystemData->vBackupBodyTriggerLines;			
			if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
			if (vLocalSystemData->vBackupBodyTriggerLines != 0)
			{
				vLocalSystemData->vBackupBodyTriggersCount++;
				if (vLocalSystemData->vBackupBodyTriggerVerifyFlag < 10)
						vLocalSystemData->vBackupBodyTriggerVerifyFlag++;
				if (vLocalSystemData->vBackupBodyTriggerVerifyFlag == 3)
					ReportErrorMessage("Error-Container Trigger Sensor may not be working properly", cEMailInspx,32000);
			}
		}
	}
}

void CSerialPort::HandleBodyTriggerLineChange(WORD TempBeltPosition, BYTE TempBodyTriggerLines)
{
	vGotBodyTriggerActiveMessage = true;
	vLocalSystemData->vBodyTriggerLines = 0;
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("HandleBodyTriggerLineChange vBodyTriggerLines = 0",cDebugMessage);

	if (vGlobalCurrentProduct	!= NULL)
	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		vLocalSystemData->vLastBodyTriggerPosition = TempBeltPosition;
	else
	{
		if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))
			vLocalSystemData->vTriggerCounter++;
		if (vLocalSystemData->vLogFile.vLogSerialData)
		{
			WriteToLogFileWithBeltPosition("*New Trigger: " + dtoa(vLocalSystemData->vTriggerCounter,0), 0);
		}
		//br ht
		if (vLocalSystemData->vShuttingDownXRays)
		{
			if (vLocalSystemData->vSystemRunMode != cCalibratingDetectorOffsetMode)
				vLocalSystemData->vLogFile.WriteToLogFile("Trigger when shutting down",cDebugMessage);
		}
		else
		//comment out as if temporarily change voltage or current while running, don't want error
		//if ((!vLocalSystemData->vSourceRampedUp) &&
		//	(vLocalSystemData->vSystemRunMode != cAutoSetupRunningSystem))
		//	vLocalSystemData->vLogFile.WriteToLogFile("Trigger when source not ramped up: " + 
		//		dtoa(vLocalSystemData->vSystemRunMode,0),cDebugMessage);
		//else
		{
			//toggle active low bits so all bits are active high
			vLocalSystemData->vBodyTriggerLines = TempBodyTriggerLines ^ vLocalSystemData->vCurrentBodyTriggerActiveLowMask;
			//mask disabled bits out so only active bits can be a one
			vLocalSystemData->vBodyTriggerLines = 
				vLocalSystemData->vBodyTriggerLines & vLocalSystemData->vCurrentBodyTriggerMask;

			if ((vLocalSystemData->vOldBodyTriggerLines != vLocalSystemData->vBodyTriggerLines) ||
				(vLocalSystemData->vOnlyOneBodyTriggerEnabled))
			{
				if (vMainWindowPointer)
					vMainWindowPointer->vBodyTriggerActiveTimer = 0;
				vLocalSystemData->vOldBodyTriggerLines = vLocalSystemData->vBodyTriggerLines;
				//if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
				//{
				//	vBeltPositionBodyTriggerWentActive = TempBeltPosition;
				//}
				//else
				//	vBeltPositionBodyTriggerWentActive = 0;

				if (vLocalSystemData->vKeepExternalDetectorsEnabled)
					ReportErrorMessage("Container Trigger changed to: " + ByteToHex(TempBodyTriggerLines) + " At: " + dtoa(TempBeltPosition,0),cAction,0);

				if (vDiscoverModeOn)
				{
					if (vDiscoverModeDataIndex) //got a trigger, and have data, so clear old container's data to take data for this container
					{
						for (BYTE TempLoop = 0; TempLoop < cNumberOfDiscoverSamples; TempLoop++)
						{
							vDiscoverModeDataIsActive[TempLoop] = false;
							vDiscoverModeDataBeltPosition[TempLoop] = 0;
						}
						vDiscoverModeDataIndex = 0;
					}
					vDiscoverModeDataBeltPosition[vDiscoverModeDataIndex] = TempBeltPosition;
					BYTE TempDiscoverModeBits = vLocalSystemData->vLastExternalDetectorLineState & vLocalConfigurationData->vExternalDetector[vDiscoverModeActiveDetectorNumber].vLineBitMask;

					if (!vLocalConfigurationData->vExternalDetector[vDiscoverModeActiveDetectorNumber].vActiveHigh)  //invert if active low
						TempDiscoverModeBits = !TempDiscoverModeBits;

					if (TempDiscoverModeBits)
						vDiscoverModeDataIsActive[vDiscoverModeDataIndex] = true;
					else
						vDiscoverModeDataIsActive[vDiscoverModeDataIndex] = false;

					vLastDiscoverModeBits = TempDiscoverModeBits;

					if (vDiscoverModeDataIsActive[vDiscoverModeDataIndex])
						vLocalSystemData->vLogFile.WriteToLogFile("Ext. Det. Active Discover New Trigger: " + dtoa(vDiscoverModeDataBeltPosition[vDiscoverModeDataIndex],0),cDebugMessage);
					else
						vLocalSystemData->vLogFile.WriteToLogFile("Ext. Det. Inactive Discover New Trigger: " + dtoa(vDiscoverModeDataBeltPosition[vDiscoverModeDataIndex],0),cDebugMessage);
					vDiscoverModeDataIndex = 1;
				}

				if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
				{
					vLocalSystemData->vMultiLaneLastTriggerPosition = TempBeltPosition;
					vLocalSystemData->vBodyTriggersCount++;
					if (vLocalSystemData->vLogFile.vLogSerialData)
						WriteToLogFileWithBeltPosition("Multi Lane-Got Image Trigger: " + dtoa(vLocalSystemData->vBodyTriggersCount + 1, 0), TempBeltPosition);
				}
				else
				if (vLocalSystemData->vBodyTriggerLines != 0)
				{
					vWaitingForContainerTriggerEnd = true;
					if (vLocalSystemData->vLogFile.vLogSerialData)
						WriteToLogFileWithBeltPosition("Got Container Trigger: " + dtoa(vLocalSystemData->vBodyTriggersCount + 1, 0), TempBeltPosition);

					//if backup body trigger configured
					if (vLocalSystemData->vBackupBodyTriggerMask)
					{
						if (vLocalSystemData->vBackupBodyTriggerVerifyFlag > -10)
							vLocalSystemData->vBackupBodyTriggerVerifyFlag--;
						if (vLocalSystemData->vBackupBodyTriggerVerifyFlag == -3) //did not get backup so may have failed
							ReportErrorMessage("Error-Backup Container Trigger Sensor May Be Bad", cEMailInspx,32000);
					}

					CContainer *TempContainer = NULL;
					//create new container object, and intitialize
					if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))
					{
						if (!vLocalSystemData->vShuttingDownXRays)
						{
							vLocalSystemData->vBodyTriggersCount++;
							if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
							{
								vLocalSystemData->vValidBodyTriggersCount++;
								TempContainer = AddNewContainer(TempBeltPosition);
								TempContainer->vRealImage = true;

								if (vLocalConfigurationData->vResynchronizeEjectors)
								{
									vLocalSystemData->vResynchronizeEjectorsCurrentTriggerCount++;

									TempContainer->vContainerNumberToEject = vLocalSystemData->vResynchronizeEjectorsCurrentTriggerCount;
									if (vLocalSystemData->vLogFile.vLogSerialData)
										WriteToLogFileWithBeltPosition("Trigger Resynchronized cen: " + dtoa(TempContainer->vContainerNumberToEject, 0),	TempContainer->vBodyTriggerPosition);
								}
								TempContainer = vLocalSystemData->vContainerCollection->GetTail();
								vPreviousContainer = vCurrentContainer;
								vCurrentContainer = TempContainer;

								if (TempContainer)
									AddedNewContainer_FillInData(TempContainer, TempBeltPosition);
								vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
							}

							if (vLocalSystemData->vReCalibrationTimerRunning)
							if (!vLocalSystemData->vAutoCalibrateWhileRunning)
								::PostThreadMessage(vGlobalCallingThreadID,cKillCalibrationTimerMessage,0,0);
						}
						else
							ReportErrorMessage("Error-Serial Thread Lock Failed 7", cEMailInspx,32000);
					}
					else
					if (vLocalSystemData->vLogFile.vLogSerialData)
						WriteToLogFileWithBeltPosition("Got Container Trigger but not running, so throw away", TempBeltPosition);
				}
				else
				if (vLocalSystemData->vCurrentBodyTriggerMask)  //1/14/2002
					ReportErrorMessage("Error - Got Container Trigger Message, but no trigger bit set", cWriteToLog,32000);

				vLocalSystemData->vLastBodyTriggerPosition = TempBeltPosition;
				vLocalSystemData->vLastBodyTriggerLineState = TempBodyTriggerLines;

				if (vLocalSystemData->vKeepExternalDetectorsEnabled)
					ReportErrorMessage("Last trigger position at: " + dtoa(TempBeltPosition,0),cAction,0);
			}
		}
	}
}

void CSerialPort::HandleBodyTriggerEnd(WORD TempBeltPosition, BYTE TempBodyTriggerLines)
{
	vLocalSystemData->vBodyTriggerLines = 0;
	if (vGlobalCurrentProduct)
	if (!vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
	{
		if (!vGotBodyTriggerActiveMessage)
			ReportErrorMessage("Possible error-Got end of body trigger without beginning of body trigger message", cWriteToLog, 0);

		vGotBodyTriggerActiveMessage = false;
		if (vLocalSystemData->vActualEncoderRate == 0)
		{
			if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
			if (vLocalSystemData->vSourceRampedUp)
				ReportErrorMessage("Got Container Trigger End But No Encoder, Check Encoder",cWriteToLog,0);
		}
		//else //br h change 2/14/2007 may cause get containers out of sync
		if (vLocalSystemData->vBodyTriggerWidthSamples < cMaximumBodyTriggerWidthData)
			vLocalSystemData->vBodyTriggerWidthSamples++;
		if (vGlobalCurrentProduct	!= NULL)
		if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		{
			vLocalSystemData->vLastBodyTriggerLength = BPSubtract(TempBeltPosition, vLocalSystemData->vLastBodyTriggerPosition);
			if (vLocalSystemData->vTestingTrigger)
			{
				vLocalSystemData->vGoodCountTotal++;
				vLocalSystemData->vTotalContainers++;
				vLocalSystemData->vImproveLearnTotalContainers++;
			}
		}
		else
		{
			if (!vLocalSystemData->vShuttingDownXRays)
			{
				//toggle active low bits so all bits are active high
				vLocalSystemData->vBodyTriggerLines = TempBodyTriggerLines ^ vLocalSystemData->vCurrentBodyTriggerActiveLowMask;
				//mask disabled bits out so only active bits can be a one
				vLocalSystemData->vBodyTriggerLines = vLocalSystemData->vBodyTriggerLines & vLocalSystemData->vCurrentBodyTriggerMask;

				if (vWaitingForContainerTriggerEnd)
				//if (vLocalSystemData->vOldBodyTriggerLines != vLocalSystemData->vBodyTriggerLines)
				{
					vLocalSystemData->vOldBodyTriggerLines = vLocalSystemData->vBodyTriggerLines;
					vWaitingForContainerTriggerEnd = false;
					if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
					if (vLocalSystemData->vBodyTriggerLines == 0)
					{
						vLocalSystemData->vLastBodyTriggerLength = BPSubtract(TempBeltPosition, vLocalSystemData->vLastBodyTriggerPosition);
						vLocalSystemData->vLastBodyTriggerLineState = TempBodyTriggerLines;
						if (vLocalSystemData->vLogFile.vLogSerialData)
						{
							CString TempString("0");
							TRY
							{
								if (vCurrentContainer)
									TempString = dtoa(vCurrentContainer->vContainerNumber, 0);
							}
							CATCH_ALL(TempException)
							{
								ReportErrorMessage("Error-vCurrentContainer Invalid Pointer",cWriteToLog,0);
							}
							END_CATCH_ALL

							WriteToLogFileWithBeltPosition("Got Container Trigger End, Cont. c: " + TempString + " Length: " + dtoa(vLocalSystemData->vLastBodyTriggerLength, 0), 0);
						}
					}
				}
			}
		}
		vLocalSystemData->vTriggerEndCounter++;
		WriteToLogFileWithBeltPosition("*Got Container Trigger End: " + dtoa(vLocalSystemData->vTriggerEndCounter,0),0);

		if (vLocalSystemData->vLastBodyTriggerLength > 4000)
			vLocalSystemData->vLastBodyTriggerLength = 0;

		if (vLocalSystemData->vBodyTriggerWidthSamples > 0)
		if (vLocalSystemData->vLastBodyTriggerLength)
		{
			if (vLocalSystemData->vLogFile.vLogSerialData)
				WriteToLogFileWithBeltPosition("Saved Trigger Length in Array: " + dtoa(vLocalSystemData->vLastBodyTriggerLength,0),0);
			vLocalSystemData->vBodyTriggerWidthData[vLocalSystemData->vBodyTriggerWidthIndex] = vLocalSystemData->vLastBodyTriggerLength;
			if (vLocalSystemData->vBodyTriggerWidthIndex + 1 >= cMaximumBodyTriggerWidthData)
				vLocalSystemData->vBodyTriggerWidthIndex = 0;
			else
				vLocalSystemData->vBodyTriggerWidthIndex++;

			//only want to average sample that are more than 14, as less than 14 is a glitch
			WORD TempNumberOfBodyTriggerSamples = 0;
			double *TempSource = &vLocalSystemData->vBodyTriggerWidthData[0];
			WORD TempMaximum = (WORD)(vLocalSystemData->vBodyTriggerWidthAverage * 1.6);
			for (WORD TempLoop = 0; TempLoop < vLocalSystemData->vBodyTriggerWidthSamples; TempLoop++)
			{
				if ((!TempMaximum) || (*TempSource < TempMaximum))
				if (*TempSource > 14)
					TempNumberOfBodyTriggerSamples++;
				TempSource++;
			}
			//find variance and average of body trigger data
			if (TempNumberOfBodyTriggerSamples > 5)
			{
					vLocalSystemData->vBodyTriggerWidthStandardDeviation = CalculateStandardDeviation(vLocalSystemData->vBodyTriggerWidthData, vLocalSystemData->vBodyTriggerWidthSamples, &vLocalSystemData->vBodyTriggerWidthAverage);
			}
		}
	}
}

CContainer *CSerialPort::FindContainerAtStartImage(WORD TempBeltPosition)
{
	WORD TempCount = vLocalSystemData->vContainerCollection->GetCount();
	bool TempDone = false;
	CContainer *TempContainer = NULL;
	if (vLocalSystemData->vContainerCollection->GetCount() > 0)
	{
		POSITION TempIndex = vLocalSystemData->vContainerCollection->GetHeadPosition();
		TempContainer = vLocalSystemData->vContainerCollection->GetAt(TempIndex);//GetHead();
		//Check Each Containter until get one before timeout
		while ((TempContainer != NULL) && (!TempDone))
		{
			if (TempContainer->vStartImagePosition == TempBeltPosition)
				TempDone = true;
			else
			{
				//get next container to check
				if (TempIndex)
					TempContainer = vLocalSystemData->vContainerCollection->GetNext(TempIndex);
				else
					TempContainer = NULL;
			}
		}  // while not done
	}  //end if have containers
	if (!TempDone)
		return NULL;
	else
		return TempContainer;
}  //end function

void CSerialPort::VerifySourceWithinTolerance()
{
	// if source is on check voltage and current not high
	if (vLocalSystemData->vSourceRampedUp)
	if (!vLocalSystemData->vShuttingDownXRays)
	if (vLocalSystemData->vSystemRunMode != cCalibratingDetectorOffsetMode)
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

		if ((vLocalSystemData->vSourceVoltage > vLocalSystemData->vSourceVoltageMaxRedLine) || 
			(vLocalSystemData->vSourceCurrent > vLocalSystemData->vSourceCurrentMaxRedLine) || 
			(vLocalSystemData->vSourceVoltage < vLocalSystemData->vSourceVoltageMinRedLine) || 
			(vLocalSystemData->vSourceCurrent < vLocalSystemData->vSourceCurrentMinRedLine))
		{
			//shut down the source
			if (!vLocalConfigurationData->vSimulateLightControlBoard)
			{
				if (!vLocalSystemData->vSourceErrorReported)
				if (vLocalSystemData->vSystemRunMode == cConditionXRayTubeMode)
				{
					vLocalSystemData->vXRayTubeConditionGlitchCount++;
					if (vLocalSystemData->vSourceVoltage < vLocalSystemData->vSourceVoltageMinRedLine)
					{
						ReportErrorMessage("Notice-X-Ray/Supply glitch: " + dtoa(vLocalSystemData->vXRayTubeConditionGlitchCount,0) + ", Voltage: " +
							dtoa(vLocalSystemData->vSourceVoltage,2) + ", Current: " + dtoa(vLocalSystemData->vSourceCurrent,2), cEMailMaintenance,0);
						ReportErrorMessage("Notice-X-Ray/Supply glitch: " + dtoa(vLocalSystemData->vXRayTubeConditionGlitchCount,0) + ", Voltage: " +
							dtoa(vLocalSystemData->vSourceVoltage,2) + ", Current: " + dtoa(vLocalSystemData->vSourceCurrent,2), cEMailInspx, 32000);
					}
					else
					if (vLocalSystemData->vSourceCurrent < vLocalSystemData->vSourceCurrentMinRedLine)
					{
						if (vLocalSystemData->vSourceCurrent < .2)
						{
							ReportErrorMessage("Error: No X-Ray Current", cError,0);
							ReportErrorMessage("Error: No X-Ray Current", cEMailInspx, 32000);
						}
						else
						{
							ReportErrorMessage("Error: Low Current", cError,0);
							ReportErrorMessage("Error: Low Current", cEMailInspx, 32000);
						}
					}

					vLocalSystemData->vSourceErrorReported = false;
					vLocalSystemData->vXRayTubeConditionCurrentStep = -1;//set so will start over
					vLocalSystemData->vXRayTubeConditionStepTimeLeft = 1;//set so next second will do next step

					if (vLocalSystemData->vXRayTubeConditionGlitchCount > 2)
					{
						if (vLocalSystemData->vSourceVoltage > vLocalSystemData->vSourceVoltageMaxRedLine)
						{
							ReportErrorMessage("Error-X-Ray Tube Conditioning Failed, Check Connectors", cError,0);
							ReportErrorMessage("Error-X-Ray Tube Conditioning Failed, Check Connectors", cEMailInspx, 32000);
						}
						vLocalSystemData->vXRayTubeConditionCurrentStep = 12;//set step so will end
					}
				}
				else
				//if (!vLocalSystemData->vSourceErrorReported)
				{
					/*
					if (vLocalConfigurationData->vAutoXRaySourceOffEnabled)
						::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						//EmergencyShutXRaysDown(0);
						*/

					vLocalSystemData->vSourceErrorReported = true;
					if (vLocalSystemData->vSourceVoltage > vLocalSystemData->vSourceVoltageMaxRedLine)
					{
						//ReportErrorMessage("Error-X-Ray voltage too high.", cEMailMaintenance,32000);
						ReportErrorMessage("Error-X-Ray voltage too high. Voltage: " + dtoa(vLocalSystemData->vSourceVoltage * 6,2) + " Current: " + dtoa(vLocalSystemData->vSourceCurrent,2),cWriteToLog,0);
						::PostThreadMessage(vGlobalCallingThreadID,cCheckHVPSVoltageSoonMessage,0,0);
					}
					else
					if (vLocalSystemData->vSourceCurrent > vLocalSystemData->vSourceCurrentMaxRedLine)
					{
						//ReportErrorMessage("Error-X-Ray current too high.", cEMailMaintenance,32000);
						ReportErrorMessage("Error-X-Ray current too high. Voltage: " + dtoa(vLocalSystemData->vSourceVoltage * 6,2) + " Current: " + dtoa(vLocalSystemData->vSourceCurrent,2),cWriteToLog,0);
						::PostThreadMessage(vGlobalCallingThreadID,cCheckHVPSVoltageSoonMessage,0,0);
					}
					else
					if (vLocalSystemData->vSourceVoltage < vLocalSystemData->vSourceVoltageMinRedLine)
					{
						//ReportErrorMessage("Error-X-Ray voltage too low.", cEMailMaintenance,32000);
						ReportErrorMessage("Error-X-Ray voltage too low. Voltage: " + dtoa(vLocalSystemData->vSourceVoltage * 6,2) + " Current: " + dtoa(vLocalSystemData->vSourceCurrent,2),cWriteToLog,0);
						::PostThreadMessage(vGlobalCallingThreadID,cCheckHVPSVoltageSoonMessage,0,0);
					}
					else
					if (vLocalSystemData->vSourceCurrent < vLocalSystemData->vSourceCurrentMinRedLine)
					{
						//ReportErrorMessage("Error-X-Ray current too low.", cEMailMaintenance,32000);
						ReportErrorMessage("Error-X-Ray current too low. Voltage: " + dtoa(vLocalSystemData->vSourceVoltage * 6,2) + " Current: " + dtoa(vLocalSystemData->vSourceCurrent,2),cWriteToLog,0);
						::PostThreadMessage(vGlobalCallingThreadID,cCheckHVPSVoltageSoonMessage,0,0);
					}
					/*
					double DeltaV = (vLocalSystemData->vSourceVoltage - 
							vLocalSystemData->vRampXRayVoltageAmount) * 6;
					if (DeltaV < 0)
						DeltaV = - DeltaV;

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
		else
		if (vLocalSystemData->vSourceErrorReported)
		{
			vLocalSystemData->vSourceErrorReported = false;
			ReportErrorMessage("Notice-X-Ray source normal after event",cEMailInspx,32000);
			//ReportErrorMessage("Notice-X-Ray source normal after event",cError,0);
		}
	}
}

void CSerialPort::CheckuCPowerSupplyVoltages()
{
	bool TempVoltageOK = true;
	if (vGlobalFPGAVersion10Point0OrHigher)
	{
		if ((vLocalSystemData->v5 > cuCHighVoltageValue) ||
			(vLocalSystemData->v5 < cuCLowVoltageValue) ||
			//(vLocalSystemData->v33 > 215) ||
			//(vLocalSystemData->v33 < 170) ||
			//(vLocalSystemData->v25 > 215) ||
			//(vLocalSystemData->v25 < 170) ||
			(vLocalSystemData->v12 > cuCHighVoltageValue) ||
			(vLocalSystemData->v12 < cuCLowVoltageValue) ||
			(vLocalSystemData->v15 > cuCHighVoltageValue) ||  //is 12v IO
			(vLocalSystemData->v15 < cuCLowVoltageValue) ||
			(vLocalSystemData->vMinus12 > cuCHighVoltageValue) ||
			(vLocalSystemData->vMinus12 < cuCLowVoltageValue))
			TempVoltageOK = false;
	}
	else
	{
		if ((vLocalSystemData->v5 > 215) ||
			(vLocalSystemData->v5 < 170) ||
			(vLocalSystemData->v33 > 215) ||
			(vLocalSystemData->v33 < 170) ||
			(vLocalSystemData->v25 > 215) ||
			(vLocalSystemData->v25 < 170) ||
			(vLocalSystemData->v12 > 215) ||
			(vLocalSystemData->v12 < 170) ||
			(vLocalSystemData->v15 > 215) ||  //is 12v IO
			(vLocalSystemData->v15 < 170) ||
			(vLocalSystemData->vMinus12 > 215) ||
			(vLocalSystemData->vMinus12 < 170))
			TempVoltageOK = false;
	}

	if (!TempVoltageOK)  //voltage problem and have not posted message yet
	{
		if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
		if (vLocalSystemData->vACPowerOn)
		{
			if (vLocalSystemData->vPosteduCVoltageProblemMessage < 250)
			{
				vLocalSystemData->vPosteduCVoltageProblemMessage++;
				if (vLocalSystemData->vPosteduCVoltageProblemMessage == 20)
					ReportErrorMessage("Error-Micro Controller power supply voltage problem", cEMailInspx,32000);
			}
			/*
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = 
				"\nMicro Controller power supply voltage problem\n\nSee Diagnostics - uController Status for details";
			TempNoticeDialog.vType = cWarningMessage;
			TempNoticeDialog.DoModal();
			*/
		}
	}
	else
		//clear posted problem flag so report next one
		vLocalSystemData->vPosteduCVoltageProblemMessage = false;

	if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
	{
		if (vLocalSystemData->vMax5 < vLocalSystemData->v5)
			vLocalSystemData->vMax5 = vLocalSystemData->v5;
		if (vLocalSystemData->vMax33 < vLocalSystemData->v33)
			vLocalSystemData->vMax33 = vLocalSystemData->v33;
		if (vLocalSystemData->vMax25 < vLocalSystemData->v25)
			vLocalSystemData->vMax25 = vLocalSystemData->v25;
		if (vLocalSystemData->vMax12 < vLocalSystemData->v12)
			vLocalSystemData->vMax12 = vLocalSystemData->v12;
		if (vLocalSystemData->vMax15 < vLocalSystemData->v15)
			vLocalSystemData->vMax15 = vLocalSystemData->v15;
		if (vLocalSystemData->vMaxMinus12 < vLocalSystemData->vMinus12)
			vLocalSystemData->vMaxMinus12 = vLocalSystemData->vMinus12;

		if (vLocalSystemData->vMin5 > vLocalSystemData->v5)
			vLocalSystemData->vMin5 = vLocalSystemData->v5;
		if (vLocalSystemData->vMin33 > vLocalSystemData->v33)
			vLocalSystemData->vMin33 = vLocalSystemData->v33;
		if (vLocalSystemData->vMin25 > vLocalSystemData->v25)
			vLocalSystemData->vMin25 = vLocalSystemData->v25;
		if (vLocalSystemData->vMin12 > vLocalSystemData->v12)
			vLocalSystemData->vMin12 = vLocalSystemData->v12;
		if (vLocalSystemData->vMin15 > vLocalSystemData->v15)
			vLocalSystemData->vMin15 = vLocalSystemData->v15;
		if (vLocalSystemData->vMinMinus12 > vLocalSystemData->vMinus12)
			vLocalSystemData->vMinMinus12 = vLocalSystemData->vMinus12;
	}
}

void CSerialPort::CheckContainers(UINT TempBTPosition)
{ //Check containers to see if passed end of Conveyor marker then tally them up
	if (vLocalSystemData->vDoingBackupDetection)
	{
		if (vLocalSystemData->vHaveABackup)
		{
			if (vLocalSystemData->vBackupDetectorOn[vLocalSystemData->vHaveABackup - 1] != 1) 
				//not on
			if (BPSubtract(TempBTPosition, vLocalSystemData->vLastBackupDetectorChangePosition[vLocalSystemData->vHaveABackup - 1]) > (cBackupDetectorStartFactor * vGlobalCurrentProduct->vImageWidthPosition)) 
			{  //backup is now clear
				TurnCanStopOnOff(vLocalSystemData->vBackupCanStop,false);
				vLocalSystemData->vHaveABackup = false;
				::PostThreadMessage(vGlobalCallingThreadID,cBackupChangedMessage,0,0);
			}
		}
		else
		{  //don't have a backup condition yet, so see if one has occured
			for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
			if (vGlobalCurrentProduct->vExternalDetectorMode[TempLoop] == cBackupDetectorMode)
			if (vLocalSystemData->vBackupDetectorOn[TempLoop] == 1)
			if (!vLocalSystemData->vHaveABackup)
			if (BPSubtract(TempBTPosition, vLocalSystemData->vLastBackupDetectorChangePosition[TempLoop]) > (cBackupDetectorStopFactor * vGlobalCurrentProduct->vImageWidthPosition)) 
			{//backup occured
				vLocalSystemData->vBackupCanStop = vGlobalCurrentProduct->vExternalDetectorEjector[TempLoop] + 1;
				if (vLocalSystemData->vBackupCanStop > cNumberOfCanStops)
					vLocalSystemData->vBackupCanStop = 1;
				TurnCanStopOnOff(vLocalSystemData->vBackupCanStop,true);
				vLocalSystemData->vHaveABackup = TempLoop + 1;
				CString TempString("1");
				if (vLocalSystemData->vBackupCanStop == 2)
					TempString = "2";
				ReportErrorMessage("Container Backup Detected on Conveyor, Stop" + TempString, cEMailMaintenance,0);
				::PostThreadMessage(vGlobalCallingThreadID,cBackupChangedMessage,1,vLocalSystemData->vBackupCanStop);
			}
		}
	}

	if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
	{
		WORD TempContainerCount = vLocalSystemData->vContainerCollection->GetCount();

		if (TempContainerCount > 0)
		{
			bool TempDone = false;
			CContainer *TempContainer = vLocalSystemData->vContainerCollection->GetHead();
			//Check and process Each Containter until get one before timeout, then stop checking
			while ((TempContainer != NULL) && (!TempDone))
			{
				TempDone = true;

				if (TempContainer->vTimeOutPosition < TempContainer->vBodyTriggerPosition)
				{  //The Timeout position has wraped around from BodyTrigger Position
					if ((TempBTPosition > TempContainer->vTimeOutPosition) && (TempBTPosition < TempContainer->vBodyTriggerPosition))
					{
						TempDone = false; // container passed end
						//ReportErrorMessage("Remove Container, passed end2 c: " + dtoa(TempContainer->vContainerNumber, 0), cWriteToLog, 0);
						//ReportErrorMessage("Container Trigger Position end2 c: " + dtoa(TempContainer->vBodyTriggerPosition, 0), cWriteToLog, 0);
						//ReportErrorMessage("Container End Position end2 c: " + dtoa(TempContainer->vTimeOutPosition, 0), cWriteToLog, 0);
						//ReportErrorMessage("Current Position end2 c: " + dtoa(TempBTPosition, 0), cWriteToLog, 0);
					}
				}
				else
				{
					if ((TempBTPosition > TempContainer->vTimeOutPosition) || (TempBTPosition < TempContainer->vBodyTriggerPosition))
					{
						TempDone = false; // container passed end
						//ReportErrorMessage("Remove Container, passed end1 c: " + dtoa(TempContainer->vContainerNumber, 0), cWriteToLog, 0);
						//ReportErrorMessage("Container Trigger Position end2 c: " + dtoa(TempContainer->vBodyTriggerPosition, 0), cWriteToLog, 0);
						//ReportErrorMessage("Container End Position end2 c: " + dtoa(TempContainer->vTimeOutPosition, 0), cWriteToLog, 0);
						//ReportErrorMessage("Current Position end2 c: " + dtoa(TempBTPosition, 0), cWriteToLog, 0);
					}
				}	
				if (TempDone)
				//if ((!vLocalSystemData->vITIPCDig->vShowOnlyRealImages) && (vLocalSystemData->vNoCommunicationsWithUController))
				if (!vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
				if (TempContainer->vImageAnalyzed)
				{  //simulating images, so mark done
					TempDone = false;
					TempContainer->vDataComplete = 2 * TempContainer->vNumberOfEjectors;
					TempContainer->vExternalDetectorsDone = true;  
					if (TempContainer->vToBeEjected)
						TempContainer->vGotEjectedNotice = TempContainer->vNumberOfEjectors;
				}

				if (!TempDone)  //not done checking containers cause found a container that exited
				{ //container has exited coveyor area
					if (vLocalSystemData->vLogFile.vLogSerialData)
					{
						if ((!TempContainer->vImageReceived) && (vLocalSystemData->vSystemRunMode == cRunningSystemMode))
						{
							WriteToLogFileWithBeltPosition("Error-Image Not Received c: " + dtoa(TempContainer->vBodyTriggerPosition, 0), vLocalSystemData->vCurrentBeltPosition);
						}
						else
						{
							if ((!TempContainer->vImageAnalyzed) && (vLocalSystemData->vSystemRunMode == cRunningSystemMode))
								WriteToLogFileWithBeltPosition("Error-Image Not Analyzed c: " + dtoa(TempContainer->vBodyTriggerPosition, 0), vLocalSystemData->vCurrentBeltPosition);
							else
							if ((!TempContainer->vExternalDetectorsDone) && (vLocalSystemData->vExternalDetectorDecisionEnd != 0))
								WriteToLogFileWithBeltPosition("Auxiliary Detector Not Done c: " + dtoa(TempContainer->vBodyTriggerPosition, 0), vLocalSystemData->vCurrentBeltPosition);
						}
					}
					if (!TempContainer->vToBeEjected) 
					{
						if((!TempContainer->vImageAnalyzed) || (!TempContainer->vImageReceived) || ((!TempContainer->vExternalDetectorsDone) && 
							(vLocalSystemData->vExternalDetectorDecisionEnd != 0)))
						{
							if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
							if (vLocalSystemData->vRampXRayStep == 0)
							if (vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
							if (!vLocalSystemData->vShuttingDownXRays)
							if (!vGlobalInAutoSize)
							{
								//did not analyze image or ext det
								vLocalSystemData->vIncompleteDataCount++;
								if (!TempContainer->vImageReceived)
								{
									ReportErrorMessage("Error, Image Not Received c: " + dtoa(TempContainer->vBodyTriggerPosition, 0), cEMailInspx,32000);
									vLocalSystemData->vLVDSErrorCount++;

									//need to disable body trigger until tunnel empty cause lost image, so out of sync with images
									if (TempContainer->vRealImage)
									if (vLocalSystemData->vLVDSErrorCount == 1)
									if (vLocalSystemData->vContainerCollection->GetCount() > 1) //if more containers in tunnel than this one
										TemporarilyDisableContainerTrigger();

									if (vLocalSystemData->vNoImageCounter < 250)
										vLocalSystemData->vNoImageCounter++;
									if (vLocalSystemData->vNoImageCounter == 10)
									if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) && (!vLocalSystemData->vRunningDiagnosticsMode)) //shut down if inspecting
									{
										::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
#ifdef CompileInHTTPClientOperationsGuardian
									if (vLocalConfigurationData->vEnableOperationsGuardian)
									{
										vGlobalOGErrorCode = 500;
										::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageImageFault, cOGSevereMessageImageFault);
									}
#endif
										ReportErrorMessage("No Image, Call Service, Inspection Stopped",cEMailInspx, 32000);
										ReportErrorMessage("No Image, Call Service, Inspection Stopped",cError, 0);
									}
								}
								else
								if (!TempContainer->vImageAnalyzed)
								{
									ReportErrorMessage("Error, Image Not Analyzed c: " + dtoa(TempContainer->vBodyTriggerPosition, 0), cEMailInspx,32000);
									vLocalSystemData->vLVDSErrorCount++;
								}
								else
								if ((vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] == 0) && (vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType]))
									ReportErrorMessage("Error, No Auxiliary Detector Data: " + dtoa(TempContainer->vBodyTriggerPosition,0), cEMailInspx,32000);
							}
							if (vGlobalInAutoSize)
							if (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem) 
							if (!vLocalConfigurationData->vSimulateLightControlBoard)
							{ //in Autosize, so probably ejector position too short to get the image, so need to increase ejector posisition.
								if ((!TempContainer->vImageReceived) || (!TempContainer->vImageAnalyzed))// || 
//									((!TempContainer->vExternalDetectorsDone) && (vLocalSystemData->vExternalDetectorDecisionEnd != 0)))
								if (vGlobalCurrentProduct->vEndOfLineTimeOut < 80)
									if (vLocalSystemData->vNeedToAdjustEjector == 0)
										vLocalSystemData->vNeedToAdjustEjector = 1;
							}
						}
						else
						{
							vLocalSystemData->vNoImageCounter = 0;
							if ((TempContainer->vContainerNumber % 1000) == 5) //% is mod function, reset error count every 1000 containers
								vLocalSystemData->vLVDSErrorCount = 0;
						}
					}
					else
					if (!TempContainer->vToBeEjectedButDisabled)
					{ //TempContainer->vToBeEjected
						bool TempDataComplete = (TempContainer->vDataComplete >= (TempContainer->vNumberOfEjectors * 2));
						if (!TempDataComplete)
						{
							if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
							if ((vLocalSystemData->vITIPCDig->vShowOnlyRealImages) || (!vLocalSystemData->vNoCommunicationsWithUController))  //not testing or running show demo
							if (TempContainer->vRealImage) //|| (!vLocalSystemData->vNoCommunicationsWithUController))
							{
								//did not get confirm on eject, or eject notice
								//vLocalSystemData->vIncompleteDataCount++;
								//vLocalSystemData->vTotalUnConfirmed++;

								if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
								{
									if (((TempContainer->vDataComplete < ((TempContainer->vNumberOfEjectors * 2) - 1))) && (!vLocalConfigurationData->vEjector[0].vConfirmEnabled))
									{
										ReportErrorMessage("Error, Did not get Eject Notification. Check ejector position. " + vLocalSystemData->vContainerString + ": " + dtoa(TempContainer->vBodyTriggerPosition, 0), cWriteToLog,32000);

										//9/11/2009
										ReportErrorMessage("Also Check Image Width and processing Time.", cWriteToLog,32000);

										ReportErrorMessage("Eject Decision Too Late 2! Adjust setup or Call Service.", cError, 0);

										ReportErrorMessage("Eject Command Too Late 2, Container Trigger: " + dtoa(TempContainer->vBodyTriggerPosition, 0), cEMailInspx,32000);

										vLocalSystemData->vTooLateToEjectErrorCount++;
										ReportErrorMessage("Eject Decision Too Late 2!", cError, 0);
										if (!vLocalSystemData->vEjectTooLateMessageGivenThisRun)
											ReportErrorMessage("Eject Decision Too Late 2!\n Change in Setup Required Immediately. \nShorten Image, or Reduce Processing Time, or call service.", cCritical, 0);

										vLocalSystemData->vEjectTooLateMessageGivenThisRun = true;
										//::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
									}
									else
									if (!TempContainer->vGotEjectConfirmedNotice)
										ReportErrorMessage("Error, Did not get Eject Confirm: " + dtoa(TempContainer->vBodyTriggerPosition, 0), cError, 0);
								}
							}
						}
					}
					if (!TempContainer->vTheImage)
					{
						if (TempContainer->vRealImage)
							ReportErrorMessage("TempContainer->vTheImage point=Null in CheckContainers: " + dtoa(TempContainer->vBodyTriggerPosition, 0), cEMailInspx, 0);
					}
					else
							//don't count images if calibrating offset
					if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem) || (!TempContainer->vRealImage))
					if (TempContainer->vTheImage->vContainerEjectReason != cTestEject)
					{
						if (vMainWindowPointer->vGaveBodyTriggerJammedMessage == true)
						{
							if (TempContainer->vTheImage->vContainerEjectReason == 255)
							{
								vMainWindowPointer->vGaveBodyTriggerJammedMessage = false;
								ReportErrorMessage("Backup cleared, inspection normal", cEMailMaintenance, 0);
							}
						}

						if ((TempContainer->vToBeEjected) || (TempContainer->vToBeEjectedButDisabled) || (TempContainer->vTheImage->vContainerEjectReason < 255))
						{
							TallyEject(TempContainer);
						}
						else  //not ejected so must be good
							//don't count images if calibrating offset
						if (vLocalSystemData->vSystemRunMode != cCalibratingDetectorOffsetMode)
						if (vLocalSystemData->vSystemRunMode != cConditionXRayTubeMode)
						if (vLocalSystemData->vSystemRunMode != cTestShutterMode)
						{
							if (TempContainer->vImageAnalyzed)
							{
								if (TempContainer->vTheImage->vContainerEjectReason == 255)  //This excludes wrong widths and slips
								{
									if ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && ((vGlobalCurrentProduct) && (!vGlobalCurrentProduct->vBulkProductMode)))
										vLocalSystemData->vGoodCountTotal++;
									else
									{//only count a container as a good container if dark enough something was in the image
										if (TempContainer->vImageBrightness < cImageBrightnessThreshold)
											vLocalSystemData->vGoodCountTotal++;  
									}
								}
							}
							vLocalSystemData->vTotalContainers++;
							vLocalSystemData->vImproveLearnTotalContainers++;
						}

						if (TempContainer->vRealImage)
							vLocalSystemData->vRealImageCountTotal++;

						//if x out of the last y containers were bad, then sound an alarm output
						if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
						if (vGlobalCurrentProduct)
						for (WORD TempLoop = 0; TempLoop < 2; TempLoop++)
						{
							if ((!vLocalSystemData->vGaveTooManyRejectsAlarm[TempLoop]) && (vGlobalCurrentProduct->vTooManyRejectsAlarmX[TempLoop]))  //if configured
							{
								DWORD TempBitVector = 1;
								for (BYTE TempBitLoop = 0; TempBitLoop < (TempContainer->vTheImage->vContainerEjectReason - 1); TempBitLoop++)
									TempBitVector = TempBitVector * 2;
										
								TempBitVector = (TempBitVector & (vGlobalCurrentProduct->vInspectionAlarmSettings[TempLoop]));
								
								//was if ((TempContainer->vToBeEjected) && (TempContainer->vTheImage->vContainerEjectReason != cTestEject) && (TempBitVector > 0))
								if ((TempContainer->vToBeEjected) && (TempContainer->vTheImage->vContainerEjectReason < cEjectSlippedContainer) && (TempBitVector > 0))
								{
									vLocalSystemData->vContainerEjectData[vLocalSystemData->vTooManyRejectsIndex[TempLoop]][TempLoop] = true;
									WORD TempRejectsCount = 0;
									for (WORD TempIndexLoop = 0; TempIndexLoop < vGlobalCurrentProduct->vTooManyRejectsAlarmY[TempLoop]; TempIndexLoop++)
									if (vLocalSystemData->vContainerEjectData[TempIndexLoop][TempLoop])
										TempRejectsCount++;

									if (TempRejectsCount >= vGlobalCurrentProduct->vTooManyRejectsAlarmX[TempLoop])
									if (vLocalConfigurationData->vEnableEjectors)
									{
										if (vLocalConfigurationData->vAlarmEjectorReEnableX)
										if (!vLocalConfigurationData->vEjectContainersNotInspected)
										//if (TempContainer->vTheImage->vContainerEjectReason == 255)
										{
											vLocalSystemData->vHaveAutoDisabledEjectors = true;
											for (WORD TempIndexLoop = 0; TempIndexLoop < vLocalConfigurationData->vAlarmEjectorReEnableY; TempIndexLoop++)
												vLocalSystemData->vContainerEjectData[TempIndexLoop][cAlarmEjectorReEnableIndex] = true;
											vLocalSystemData->vTooManyRejectsIndex[cAlarmEjectorReEnableIndex] = 0;
										}
										vLocalSystemData->vGaveTooManyRejectsAlarm[TempLoop] = true;

										if ((!vLocalConfigurationData->vUseXofYAlarmOutputForUnconfirmedEject) || (TempLoop == 0)) //don't turn on opto output if configured to be used by unconfirmed eject
											TurnAlarmOnOff(TempLoop + 1,true);//turn on x of y alarm bit

										for (WORD TempIndexLoop = 0; TempIndexLoop < vGlobalCurrentProduct->vTooManyRejectsAlarmY[TempLoop]; TempIndexLoop++)
											vLocalSystemData->vContainerEjectData[TempIndexLoop][TempLoop] = false;

										CString TempAlarmString;
										TempAlarmString.LoadString(IDS_Alarm);
										CString TempRejectsOutOfString;
										TempRejectsOutOfString.LoadString(IDS_RejectsOutOf);
										CString TempEjectorsDisabledString;
										TempEjectorsDisabledString.LoadString(IDS_EjectorsDisabled);
										if (vGlobalCurrentProduct->vTooManyRejectsAlarmDisableEjectors[TempLoop])
										{
											if (!vLocalConfigurationData->vEjectContainersNotInspected)
											{
												::PostThreadMessage(vGlobalCallingThreadID,cDisableEjectorsMessage,TempLoop,TempLoop);
												ReportErrorMessage(TempAlarmString + " " + dtoa(TempLoop + 1, 0) + ",  " + dtoa(vGlobalCurrentProduct->vTooManyRejectsAlarmX[TempLoop], 0) + " " + 
													TempRejectsOutOfString + " " + dtoa(vGlobalCurrentProduct->vTooManyRejectsAlarmY[TempLoop], 0) + ", " + TempEjectorsDisabledString, cEMailPlantOperations,11 + TempLoop);
											}
											else
											{
												//::PostThreadMessage(vGlobalCallingThreadID,cStopRunningMessage,TempLoop,TempLoop);
												if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
													::PostThreadMessage(vGlobalCallingThreadID, cuControllerStopRunningMessage,0,0);
												ReportErrorMessage(TempAlarmString + " " + dtoa(TempLoop + 1, 0) + ",  " + dtoa(vGlobalCurrentProduct->vTooManyRejectsAlarmX[TempLoop], 0) + " " + 
													TempRejectsOutOfString + " " + dtoa(vGlobalCurrentProduct->vTooManyRejectsAlarmY[TempLoop], 0) + ", Inspection Stopped", cEMailPlantOperations,11 + TempLoop);
											}
										}
										else
										ReportErrorMessage(TempAlarmString + " " + dtoa(TempLoop + 1, 0) + ",  " + dtoa(vGlobalCurrentProduct->vTooManyRejectsAlarmX[TempLoop], 0) + " " + 
											TempRejectsOutOfString + " " + dtoa(vGlobalCurrentProduct->vTooManyRejectsAlarmY[TempLoop], 0), cEMailPlantOperations,11 + TempLoop);
									}
								}
								else
									vLocalSystemData->vContainerEjectData[vLocalSystemData->vTooManyRejectsIndex[TempLoop]][TempLoop] = false;
							}
							else
								vLocalSystemData->vContainerEjectData[vLocalSystemData->vTooManyRejectsIndex[TempLoop]][TempLoop] = false;

							vLocalSystemData->vTooManyRejectsIndex[TempLoop]++;
							if (vLocalSystemData->vTooManyRejectsIndex[TempLoop] >= vGlobalCurrentProduct->vTooManyRejectsAlarmY[TempLoop])
								vLocalSystemData->vTooManyRejectsIndex[TempLoop] = 0;
						}

						if (vLocalSystemData->vHaveAutoDisabledEjectors)
						{
							if (TempContainer->vToBeEjected)
								vLocalSystemData->vContainerEjectData[vLocalSystemData->vTooManyRejectsIndex[cAlarmEjectorReEnableIndex]] [cAlarmEjectorReEnableIndex] = true;
							else
							{
								vLocalSystemData->vContainerEjectData[vLocalSystemData->vTooManyRejectsIndex[cAlarmEjectorReEnableIndex]] [cAlarmEjectorReEnableIndex] = false;
								BYTE TempGoodContainerCount = 0;
								for (WORD TempIndexLoop = 0; TempIndexLoop < vLocalConfigurationData->vAlarmEjectorReEnableY; TempIndexLoop++)
								if (!vLocalSystemData->vContainerEjectData[TempIndexLoop] [cAlarmEjectorReEnableIndex])
									TempGoodContainerCount++;

								if (TempGoodContainerCount >= vLocalConfigurationData->vAlarmEjectorReEnableX)
								if (!vLocalConfigurationData->vEnableEjectors)
								{
									::PostThreadMessage(vGlobalCallingThreadID,cEnableEjectorsMessage,0,0);
									vLocalSystemData->vHaveAutoDisabledEjectors = false;
									TurnAlarmOnOff(1,false);//turn off x of y alarm bit
									TurnAlarmOnOff(2,false);//turn off x of y alarm bit
									CString TempString = "-";
									TempString.LoadString(IDS_InspectingOKReenabledEjectorsAfterAlarm);
									ReportErrorMessage(TempString,//"Inspecting OK, Re-enabled Ejectors After Alarm",
										cEMailPlantOperations,18);
									for (WORD TempLoop = 0; TempLoop < 2; TempLoop++)
										vLocalSystemData->vGaveTooManyRejectsAlarm[TempLoop] = false;
								}
							}
							vLocalSystemData->vTooManyRejectsIndex[cAlarmEjectorReEnableIndex]++;
							if (vLocalSystemData->vTooManyRejectsIndex[cAlarmEjectorReEnableIndex] >= vLocalConfigurationData->vAlarmEjectorReEnableY)
								vLocalSystemData->vTooManyRejectsIndex[cAlarmEjectorReEnableIndex] = 0;
						}
					}

					RemoveContainerFromCollection();

					if (vBeltPositionToEnableTriggerAfterTemporaryDisable)  //disabled triggers to clear tunnel to resync containers, is tunnel clear now.
						EnableTriggerAfterTemporaryDisable(false); //if tunnel is clear, re-enable triggers

					//get next container to check
					if (vLocalSystemData->vContainerCollection->GetCount() > 0)
						TempContainer = vLocalSystemData->vContainerCollection->GetHead();
					else
						TempDone = true;
				} //if not done
			}  // while not done
		}
		vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
	}  //end if have containers
	else
		ReportErrorMessage("Error-Serial Thread Lock Failed 8", cEMailInspx,32000);
}  //end function

void CSerialPort::SendSerialCommand(tSerialCommand TheData)
{
	tSerialCommand vCommand;
	for (BYTE TempLoop = 0; TempLoop < cuControllerMessageLength - 1; TempLoop++)
		vCommand[TempLoop] = TheData[TempLoop];
	vCommand[cuControllerMessageLength - 1] = (CHAR)cETX;
	if (vPortOpen)
	{
		DWORD TempErrorCode = 0;
		DWORD TempBytesWritten = 0;
		int TempDidIt = 0;

		TempDidIt = WriteFile(vWinPortHandle,&vCommand,
			cuControllerMessageLength,&TempBytesWritten,
			&vWriteOverlappedStructure);
 		TempErrorCode = GetLastError(); 
		if ((TempErrorCode == ERROR_IO_PENDING) || (TempDidIt))
		{
			vWaitingForReply = true;
			vHasTransmittedCommunication = true;
			CString TempString("-");
			TempString = "";
			//Log to file if configured to
			if ((vLocalSystemData->vLogFile.vLogSerialData) ||
				(vLocalSystemData->vLogFile.vShowingSerialDataOnScreen))
			{
				//add the decimal values of the string on the end
				for (BYTE TempLoop = 0; TempLoop < cuControllerMessageLength; TempLoop++)
				{
					TempString = TempString + ByteToHex(vCommand[TempLoop]);
					if (TempLoop == 0)
						TempString = TempString + ";";
					else
						TempString = TempString + ":";
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
	
void CSerialPort::SetXRayVoltage(double TempVoltage)
{
	//Send X-Ray Voltage
	double TempVoltageCode = TempVoltage  * 6553.5;
	WORD TempWord = (WORD)(TempVoltageCode);

	tSerialCommand TempCommand;
	TempCommand[0] = 0x4A;
	TempCommand[1] = (BYTE)(TempWord >> 8);
	TempCommand[2] = (BYTE)(TempWord);
	TempCommand[3] = 0;
	SendSerialCommand(TempCommand);
	Sleep(cSendCommandSleepTime);
}

void CSerialPort::SetXRayCurrent(double TempCurrent)
{
	double TempCurrentCode = TempCurrent  * 6553.5;
	WORD TempWord = (WORD)(TempCurrentCode);

	tSerialCommand TempCommand;
	TempCommand[0] = 0x4B;
	TempCommand[1] = (BYTE)(TempWord >> 8);
	TempCommand[2] = (BYTE)(TempWord);
	TempCommand[3] = 0;
	SendSerialCommand(TempCommand);
	Sleep(cSendCommandSleepTime);
}

bool CSerialPort::IsBeltPositionInWindow(WORD TempPosition, WORD TempStart, WORD TempEnd)
{
	if (TempStart <= TempEnd)
	{
		if ((TempPosition >= TempStart) && (TempPosition <= TempEnd))
			return true;
		else
			return false;
	}
	else
	{
		if ((TempPosition >= TempStart) || (TempPosition <= TempEnd))
			return true;
		else
			return false;
	}
}

void CSerialPort::EjectContainer(BYTE TempReason, CContainer *TempContainer, CInspection *TempInspection)
{
	//EjectReason 0 to vGlobalCurrentProduct->vNumberOfInspections are major inspections

	//EjectReason vGlobalCurrentProduct->vNumberOfInspections to 
	//  vGlobalCurrentProduct->vNumberOfInspections + 
	//  vLocalSystemData->vNumberOfShapeInspections
	//	are shape inspections

	//EjectReason vGlobalCurrentProduct->vNumberOfInspections + 
	//  vLocalSystemData->vNumberOfShapeInspections to 
	//	vGlobalCurrentProduct->vNumberOfInspections + 
	//  vLocalSystemData->vNumberOfShapeInspections +
	//  vGlobalCurrentProduct->vNumberOfAuxiliaryDetectors are Auxiliary detectors
	if (TempContainer->vEjectorNumber == 0)
		TempContainer->vEjectorNumber = 1;

	if (TempContainer)
	if (!TempContainer->vTheImage)
		ReportErrorMessage("TempContainer->vTheImage point=Null in EjectContainer: " + dtoa(TempContainer->vBodyTriggerPosition, 0), cEMailInspx, 0);
	else
	if (!TempContainer->vTheImage->vPassContainer)
	{
		if ((!TempInspection) || (TempInspection->vEnable))
		{
			if ((vLocalSystemData->vXRayOnNotice) || (TempReason == cTestEject)) //in AutoSetup timing ejectors don't have X-Rays on, so don't inspect and eject except for test ejects
				TempContainer->vToBeEjected = true;
		}

		if (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem)
		{
			TempContainer->vTheImage->vContainerEjectReason = TempReason;
			TempContainer->vToBeEjected = false;
			TempContainer->vToBeEjectedButDisabled = true;

			if (TempInspection)
			{
				TempContainer->vTheImage->vContainerEjectInspection = TempReason - cEjectInspectionOffset;
			}
			else
				TempContainer->vTheImage->vContainerEjectInspection = 255;

			if (TempInspection)
			for (BYTE TempLoop = 0; TempLoop < vGlobalCurrentProduct->vNumberOfInspections; TempLoop++)
			if (TempContainer->vTheImage->vRejectedForThisInspection[TempLoop])
				vLocalSystemData->vXRayCriteriaCountTotal[vLocalSystemData->vCountToUseForEjectReason[cEjectInspectionOffset + TempLoop]]++;
		}
		else
		if ((TempInspection) && (!TempInspection->vEjector))
		{
			TempContainer->vTheImage->vContainerEjectReason = TempReason;
			TempContainer->vTheImage->vContainerEjectInspection = TempReason - cEjectInspectionOffset;
			vLocalSystemData->vXRayCriteriaCountButDisabled[TempReason]++;
			ReportErrorMessage("Error-No Ejector for Inspection",cEMailInspx,32000);
		}
		else
		if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (!TempContainer->vRealImage))
		{
			DWORD TempEjector = 1;
			if (TempInspection)
				TempEjector = vGlobalCurrentProduct->vInspection[TempReason - cEjectInspectionOffset]->vEjector;
			else
			if (TempReason == cTestEject)
				TempEjector = vLocalSystemData->vTestEjectNextContainer;
			else
			if (TempReason < cEjectAuxilaryDetectorOffset)
				TempEjector = vLocalConfigurationData->vShapeHandlingEjectors;//Shape handling inspection
			else
			if (TempReason <= cMaximumRejectReason)
				TempEjector = vGlobalCurrentProduct->vExternalDetectorEjector[TempReason - cEjectAuxilaryDetectorOffset];//Auxiliary Detector

			vLocalSystemData->vRejectsDuringLearn++;
			TempContainer->vTheImage->vContainerEjectReason = TempReason;
			if (((vLocalConfigurationData->vEnableEjectors) || (TempReason == cTestEject)) && ((!TempInspection) || (TempInspection->vEnable)))  //note ejectors numbers are 1 to 32, so must subtract one to index dwell time
			{
				TempContainer->vToBeEjected = true;
				TempContainer->vToBeEjectedButDisabled = false;
			}
			else
			{
				TempContainer->vToBeEjected = false;
				TempContainer->vToBeEjectedButDisabled = true;
			}

			if (TempInspection)
			{
				TempContainer->vTheImage->vContainerEjectInspection = TempReason - cEjectInspectionOffset;
			}
			else
				TempContainer->vTheImage->vContainerEjectInspection = 255;

			{
				TempContainer->vTheImage->vContainerEjectReason = TempReason;

				if (TempInspection)
				{
					TempContainer->vEjectorNumber = TempInspection->vEjector;

					if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes) //if multi lane, change the ejector for the lane
					{
						if (vLocalSystemData->vLogFile.vLogSerialData)
							WriteToLogFileWithBeltPosition("Multi Lane, original ejector bit map: " + dtoa(TempContainer->vEjectorNumber,0) + ", Ej Offset: " + dtoa(TempContainer->vTheImage->vMultiLaneEjectorOffset, 0), TempContainer->vBodyTriggerPosition);
						TempContainer->vEjectorNumber = TempContainer->vEjectorNumber << (TempContainer->vTheImage->vMultiLaneLaneNumber - 1);  //lane number starts at zero, so Lane 1 adds zero to ejector number, shift bit for each ejector number to increase
						//TempContainer->vEjectorNumber = TempContainer->vEjectorNumber + TempContainer->vTheImage->vMultiLaneLaneNumber - 1;  //lane number starts at zero, so Lane 1 adds zero to ejector number, shift bit for each ejector number to increase
						if (vLocalSystemData->vLogFile.vLogSerialData)
							WriteToLogFileWithBeltPosition("Multi Lane: " + dtoa(TempContainer->vTheImage->vMultiLaneLaneNumber,0) + 
								" Change to Ejector bit map:" + dtoa(TempContainer->vEjectorNumber,0), TempContainer->vBodyTriggerPosition);
					}
				}
				else
				if (TempReason == cTestEject)
				{
					if (vLocalSystemData->vTestEjectNextContainer)
						TempContainer->vEjectorNumber = vLocalSystemData->vTestEjectNextContainer;
					else
						TempContainer->vEjectorNumber = 1;

					if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes) //if multi lane, change the ejector for the lane
					{
						TempContainer->vEjectorNumber = 1;
						if (vGlobalCurrentProduct->vInspection[0])
							TempContainer->vEjectorNumber = vGlobalCurrentProduct->vInspection[0]->vEjector;

						if (vLocalSystemData->vLogFile.vLogSerialData)
							WriteToLogFileWithBeltPosition("Multi Lane, original ejector bit map: " + dtoa(TempContainer->vEjectorNumber,0) + ", Ej Offset: " + dtoa(TempContainer->vTheImage->vMultiLaneEjectorOffset, 0), TempContainer->vBodyTriggerPosition);
						TempContainer->vEjectorNumber = TempContainer->vEjectorNumber << (TempContainer->vTheImage->vMultiLaneLaneNumber - 1);  //lane number starts at zero, so Lane 1 adds zero to ejector number, shift bit for each ejector number to increase
						if (vLocalSystemData->vLogFile.vLogSerialData)
							WriteToLogFileWithBeltPosition("Multi Lane: " + dtoa(TempContainer->vTheImage->vMultiLaneLaneNumber,0) + 
								" Change to Ejector bit map:" + dtoa(TempContainer->vEjectorNumber,0), TempContainer->vBodyTriggerPosition);
					}

					if (vLocalSystemData->vEject4InARowCount)
						vLocalSystemData->vEject4InARowCount--;
				}
				else
				if (TempReason < cEjectAuxilaryDetectorOffset)
				{ //Shape handling inspection
					TempContainer->vEjectorNumber = vLocalConfigurationData->vShapeHandlingEjectors;
				}
				else
				if (TempReason <= cMaximumRejectReason)
				{  //Auxiliary Detector
					TempContainer->vEjectorNumber = vGlobalCurrentProduct->vExternalDetectorEjector[TempReason - cEjectAuxilaryDetectorOffset];

					if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes) //if multi lane, change the ejector for the lane
					{
						if (vLocalSystemData->vLogFile.vLogSerialData)
							WriteToLogFileWithBeltPosition("Multi Lane, original ejector bit map: " + dtoa(TempContainer->vEjectorNumber,0) + ", Ej Offset: " + dtoa(TempContainer->vTheImage->vMultiLaneEjectorOffset, 0), TempContainer->vBodyTriggerPosition);
						TempContainer->vEjectorNumber = TempContainer->vEjectorNumber << (TempContainer->vTheImage->vMultiLaneLaneNumber - 1);  //lane number starts at zero, so Lane 1 adds zero to ejector number, shift bit for each ejector number to increase
						if (vLocalSystemData->vLogFile.vLogSerialData)
							WriteToLogFileWithBeltPosition("Multi Lane: " + dtoa(TempContainer->vTheImage->vMultiLaneLaneNumber,0) + 
								" Change to Ejector bit map:" + dtoa(TempContainer->vEjectorNumber,0), TempContainer->vBodyTriggerPosition);
					}

					::PostThreadMessage(vGlobalCallingThreadID,cAuxiliaryDetectorEjectMessage, TempReason,	TempReason - cEjectAuxilaryDetectorOffset);
				}

				if (!TempContainer->vEjectorNumber) //is an improve learn as has no ejector
				{
					//TempContainer->vToBeEjected = true;
					TempContainer->vDataComplete = true;
					TempContainer->vToBeEjectedButDisabled = true;
					//vLocalSystemData->vITIPCDig->vLastImproveLearn = true;
					ReportErrorMessage("Check To " + vLocalSystemData->vCriteriaString[vLocalSystemData->vCountToUseForEjectReason[TempReason]] + 
							" " + vLocalSystemData->vContainerString + ": " + dtoa(TempContainer->vContainerNumber,0), cReject,0);
				}
				else
				//if ((!TempInspection) || (TempInspection->vEnable))
				//if ejectors are disabled, eject anyway as FPGA will block eject, but flash blue light
				if ((!TempInspection) || (TempInspection->vEnable))
				for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
				if (TempContainer->vEjectorNumber & (1 << TempLoop))
				{
					TempContainer->vNumberOfEjectors++;
					TempContainer->vEjectPosition[TempLoop] = BPAdd(vGlobalCurrentProduct->vEjectorDelayBeltPosition[TempLoop], TempContainer->vBodyTriggerPosition);

					//on Multi Lane, add the number of encoder pulses into the raw image the container started to correct for where in the image the container is
					if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes) //if multi lane, change the position for the eject by the distance in the image the object started
					if (TempContainer->vTheImage->vMultiLaneEjectorOffset)
					{
						TempContainer->vEjectPosition[TempLoop] = BPAdd(TempContainer->vEjectPosition[TempLoop], TempContainer->vTheImage->vMultiLaneEjectorOffset);

						if (vLocalSystemData->vLogFile.vLogSerialData)
							WriteToLogFileWithBeltPosition("Multi Lane Eject Offset, add: " + dtoa(TempContainer->vTheImage->vMultiLaneEjectorOffset,0) + " to Eject Position", TempContainer->vBodyTriggerPosition);
					}
				
					if (vGlobalCurrentProduct->vEjectorResponseTime[TempLoop])
					{
						TempContainer->vEjectPosition[TempLoop] = BPSubtract(TempContainer->vEjectPosition[TempLoop], (WORD)(vLocalSystemData->vActualEncoderRate * 
							vGlobalCurrentProduct->vEjectorResponseTime[TempLoop]) + vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust);
					}
					else
					{
						TempContainer->vEjectPosition[TempLoop] = BPSubtract(TempContainer->vEjectPosition[TempLoop], vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust);
					}

					if (vLocalSystemData->vLogFile.vLogSerialData)
					if (vLocalConfigurationData->vEnableEjectors)
					if (!TempInspection)
						WriteToLogFileWithBeltPosition("Ejected Container Reason: " + dtoa(vLocalSystemData->vCountToUseForEjectReason[TempContainer->vTheImage->vContainerEjectReason],0) + 
							" Ejector:" + dtoa(TempLoop + 1,0) + " " + vLocalSystemData->vCriteriaString[vLocalSystemData->vCountToUseForEjectReason[TempContainer->vTheImage->vContainerEjectReason]], TempContainer->vBodyTriggerPosition);
					else
						WriteToLogFileWithBeltPosition("Ejected Container because Inspection: " + dtoa(vLocalSystemData->vCountToUseForEjectReason[TempContainer->vTheImage->vContainerEjectReason],0) + 
							" Ejector:" + dtoa(TempLoop + 1,0) + " " + vLocalSystemData->vCriteriaString[vLocalSystemData->vCountToUseForEjectReason[TempContainer->vTheImage->vContainerEjectReason]], TempContainer->vBodyTriggerPosition);

					WORD TempDelta = TempContainer->vEjectPosition[TempLoop] - vLocalSystemData->vCurrentBeltPosition - ((WORD)(0.002 * vLocalSystemData->vBeltRate)); 
					//TempDelta is Conveyor positions before need to eject, add 2 milliseconds times the belt Conveyor
					//to give time to execute command
					if (TempDelta < 0xC000)
					{
						//test
						//WriteToLogFileWithBeltPosition("Eject Delta OK: " + dtoa(TempDelta,0), TempContainer->vBodyTriggerPosition);

						//don't tell uC to eject if simulated images
						if ((!TempContainer->vRealImage) && (vLocalSystemData->vNoCommunicationsWithUController))
						{//simulating, so simulate got confirmation
							WriteToLogFileWithBeltPosition("Simulated image so don't send eject command", TempContainer->vBodyTriggerPosition);
							TempContainer->vToBeEjected = true;
							TempContainer->vGotEjectedNotice = TempContainer->vNumberOfEjectors;
							TempContainer->vGotEjectConfirmedNotice = true;
							TempContainer->vDataComplete = 2 * TempContainer->vNumberOfEjectors;

							CString TempString = "None";
							TempString = dtoa(TempContainer->vContainerNumber, 0);
							if ((vLocalConfigurationData->vEnableEjectors) || ((vGlobalCurrentProduct->vEjectorDwellTime[TempLoop] == 0) && (TempContainer->vTheImage->vContainerEjectReason > vGlobalCurrentProduct->vNumberOfInspections))) //sampling using a diverter
								ReportErrorMessage("Ejected " + vLocalSystemData->vCriteriaString[vLocalSystemData->vCountToUseForEjectReason[TempReason]] + " " + vLocalSystemData->vContainerString + ": " + TempString, cReject, 0);
							else
								ReportErrorMessage("Did not eject " + vLocalSystemData->vCriteriaString[vLocalSystemData->vCountToUseForEjectReason[TempReason]] + ", ejectors disabled, c: " + TempString, cReject, 0);
						}
						//removed 9/16/2016
						//else
						//if (!vLocalSystemData->vConveyorEncoderRate)
						//{
						//	ReportErrorMessage("Error - Did not Eject as conveyor not moving: " + dtoa(TempContainer->vContainerNumber,0),cWriteToLog,0);
						//	TempContainer->vToBeEjected = false;
						//}
						else
						if (vLocalConfigurationData->vEnableEjectors)
						{
							if (!vLocalSystemData->vActualEncoderRate)
							//if (!vLocalSystemData->vConveyorEncoderRate)
								ReportErrorMessage("Warning-Conveyor not moving before eject: " + dtoa(TempContainer->vContainerNumber,0),cError,0);

							if (vLocalSystemData->vOutstandingEjects < cMaximumOutstandingEjects)
							{
								TempContainer->vToBeEjected = true;
								vLocalSystemData->vOutstandingEjects++;
								if (vLocalConfigurationData->vResynchronizeEjectors)
								{
									int TempWaitTime = (int)vGlobalCurrentProduct->vResyncTriggerToEjectTime[TempContainer->vEjectorNumber - 1];  //ejector number is one based
									if (vGlobalCurrentProduct->vResyncTriggerToEjectTime[TempContainer->vEjectorNumber - 1])
									{
										int TempCorrection = (int)(vGlobalCurrentProduct->vEjectorResponseTime[TempContainer->vEjectorNumber - 1] * vLocalSystemData->vActualEncoderRate);
										if (TempWaitTime > TempCorrection)
											TempWaitTime = TempWaitTime - TempCorrection;
										else
											TempWaitTime = 0;
									}
										//temp for hardware limitation, should be able to go to zero, but firmware has a bug or something, it does not fire at all if less than 3
									if (TempWaitTime < 3)
										TempWaitTime = 3;
									SendResynchronizedEjectCommand(TempContainer->vContainerNumberToEject, TempContainer->vEjectorNumber, (WORD)TempWaitTime);

									if (vLocalSystemData->vLogFile.vLogSerialData)
										WriteToLogFileWithBeltPosition("Send Resynchronized Eject Command, c: " + dtoa(TempContainer->vContainerNumber, 0) + ", cen: " + dtoa(TempContainer->vContainerNumberToEject, 0) + " Ejector: " +
											dtoa(TempLoop + 1, 0) + " Wait Time: " + dtoa(TempWaitTime, 0), TempContainer->vBodyTriggerPosition);
								}
								else

								if (TempContainer->vEjectorNumber)
								{
									CString TempString = "None";
									TempString = dtoa(TempContainer->vContainerNumber, 0);

									if ((vLocalConfigurationData->vEnableEjectors) || ((vGlobalCurrentProduct->vEjectorDwellTime[TempLoop] == 0) && (TempContainer->vTheImage->vContainerEjectReason > vGlobalCurrentProduct->vNumberOfInspections))) //sampling using a diverter
									{
										if (vGlobalCurrentProduct->vEjectorDwellTime[TempLoop] == 0) //&& (TempContainer->vTheImage->vContainerEjectReason > vGlobalCurrentProduct->vNumberOfInspections))
												ReportErrorMessage("Diverted " + vLocalSystemData->vCriteriaString[vLocalSystemData->vCountToUseForEjectReason[TempReason]] + " " + vLocalSystemData->vContainerString + ": " + TempString, cReject,0);
										else
										if (TempContainer->vEjectorNumber)
												ReportErrorMessage("Ejected " + vLocalSystemData->vCriteriaString[vLocalSystemData->vCountToUseForEjectReason[TempReason]] + " " + vLocalSystemData->vContainerString + ": " + TempString, cReject,0);
										else
												ReportErrorMessage("Check For " + vLocalSystemData->vCriteriaString[vLocalSystemData->vCountToUseForEjectReason[TempReason]] + " " + vLocalSystemData->vContainerString + ": " + TempString, cReject,0);
									}
									else
									{
										ReportErrorMessage("Did not eject " + vLocalSystemData->vCriteriaString[vLocalSystemData->vCountToUseForEjectReason[TempReason]] + ", ejectors disabled, c: " + TempString, cReject,0);
									}
								}

								if (vGlobalCurrentProduct->vEjectorDwellTime[TempLoop] == 0) //&& (TempContainer->vTheImage->vContainerEjectReason > vGlobalCurrentProduct->vNumberOfInspections)) //sampling using a diverter
								{
									if (vLocalSystemData->vDiverterActive[TempLoop])
									{
										TempContainer->vToBeEjected = true;
										TempContainer->vGotEjectedNotice = TempContainer->vNumberOfEjectors;
										TempContainer->vGotEjectConfirmedNotice = true;
										TempContainer->vDataComplete = 2 * TempContainer->vNumberOfEjectors;//since not ejecting as diverter in correct position, mark container complete
										ReportErrorMessage("Diverter: " + dtoa(vLocalConfigurationData->vEjector[TempLoop].vDigitalInputLine,0) + " already active no action needed at BP: " + dtoa(TempContainer->vEjectPosition[TempLoop], 0),cWriteToLog,0);
									}
									else
									{
										BYTE TempEjector = vLocalConfigurationData->vEjector[TempLoop].vDigitalInputLine;
										//don't use as Shuttleworth ejector is pulse driven, not level drive, pulse to open it to eject postion, TempEjector = TempEjector | 0x80; //turn Diverter On to Sample 
										vLocalSystemData->vDiverterActive[TempLoop] = true;
										if (TempContainer->vEjectPosition[TempLoop] == 0)
											TempContainer->vEjectPosition[TempLoop] = 0xFFFF;

										SendEjectCommand(TempEjector, TempContainer->vEjectPosition[TempLoop]);

										ReportErrorMessage("Activate Diverter: " + dtoa(vLocalConfigurationData->vEjector[TempLoop].vDigitalInputLine,0) + " at BP: " + dtoa(TempContainer->vEjectPosition[TempLoop], 0),cAction,0);
									}
								}
								else
								{
									SendEjectCommand(vLocalConfigurationData->vEjector[TempLoop].vDigitalInputLine,TempContainer->vEjectPosition[TempLoop]);
									if (vLocalSystemData->vLogFile.vLogSerialData)
									{
										CString TempString = dtoa(TempLoop + 1, 0);
										CString TempString1 = dtoa(TempContainer->vEjectPosition[TempLoop], 0);
										CString TempString2 = dtoa(TempContainer->vBodyTriggerPosition, 0);
										WriteToLogFileWithBeltPosition("Send Eject Command, c: " + TempString2 + " Ejector: " + TempString + " at: " + TempString1, TempContainer->vBodyTriggerPosition);
									}
									if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
									if (vGlobalCurrentProduct->vMultiLaneEjectAdjacentLanes) //want to eject the lane on either side of this lane too
									{
										if (TempContainer->vTheImage->vMultiLaneLaneNumber > 1) //if higher than ejector 1, activate ejector on lane before
										{
											SendEjectCommand(vLocalConfigurationData->vEjector[TempLoop - 1].vDigitalInputLine, TempContainer->vEjectPosition[TempLoop]);
											if (vLocalSystemData->vLogFile.vLogSerialData)
											{
												CString TempString = dtoa(TempLoop, 0);
												CString TempString1 = dtoa(TempContainer->vEjectPosition[TempLoop], 0);
												CString TempString2 = dtoa(TempContainer->vBodyTriggerPosition, 0);
												WriteToLogFileWithBeltPosition("Send Eject Command lane before, c: " + TempString2 + " Ejector: " + TempString + " at: " + TempString1, TempContainer->vBodyTriggerPosition);
											}
										}

										if (TempContainer->vTheImage->vMultiLaneLaneNumber < vGlobalCurrentProduct->vMultiLaneNumberOfLanes) //if not last lane, activate next lanes ejector
										{
											SendEjectCommand(vLocalConfigurationData->vEjector[TempLoop + 1].vDigitalInputLine, TempContainer->vEjectPosition[TempLoop]);
											if (vLocalSystemData->vLogFile.vLogSerialData)
											{
												CString TempString = dtoa(TempLoop + 2, 0);
												CString TempString1 = dtoa(TempContainer->vEjectPosition[TempLoop], 0);
												CString TempString2 = dtoa(TempContainer->vBodyTriggerPosition, 0);
												WriteToLogFileWithBeltPosition("Send Eject Command lane after, c: " + TempString2 + " Ejector: " + TempString + " at: " + TempString1, TempContainer->vBodyTriggerPosition);
											}
										}
									}
								}
							}
							else
							{
								if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
								{
									ReportErrorMessage("Error-Did Not Eject as Eject Queue Full, " + vLocalSystemData->vContainerString + ": " + dtoa(TempContainer->vContainerNumber,0),cError,0);
									vLocalSystemData->vProcessingErrorCount++;
								}
							}
						}
					}
					else
					{
						WriteToLogFileWithBeltPosition("Ejected Too Late", TempContainer->vBodyTriggerPosition);

						if ((vLocalSystemData->vITIPCDig->vShowOnlyRealImages) || (!vLocalSystemData->vNoCommunicationsWithUController)) //if have a real image, or communicating to a real uC board
						if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
						if ((vLocalConfigurationData->vEnableEjectors) && ((!TempInspection) || (TempInspection->vEnable)))
						if (TempContainer->vRealImage)
						{
							TempContainer->vToBeEjected = true;

							ReportErrorMessage("Cur BP: " + dtoa(vLocalSystemData->vCurrentBeltPosition, 0) + " Needed: " + dtoa(TempContainer->vEjectPosition[TempLoop], 0) + 
								" Conveyor Rate: " + dtoa(vLocalSystemData->vBeltRate,0) + "Hz",cEMailInspx,32000);

							ReportErrorMessage( "Error-Too Late to Eject 1 cont: " + dtoa(TempContainer->vBodyTriggerPosition,0) + ", Check ejector:" + dtoa(TempLoop + 1,0) + " position",cEMailInspx,32000);
							vLocalSystemData->vTooLateToEjectErrorCount++;

							ReportErrorMessage("Eject Decision Too Late 1!", cError, 0);
							if (!vLocalSystemData->vEjectTooLateMessageGivenThisRun)
								ReportErrorMessage("Eject Decision Too Late 1!\nChange in Setup Required Immediately. \nShorten Image, or Reduce Processing Time, or call service", cCritical, 0);

							vLocalSystemData->vEjectTooLateMessageGivenThisRun = true;
						}
					}
				}
			}
		}
	}
	else
	if (vLocalSystemData->vITIPCDig->vHaveRealImage)
	if (vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
	if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
	if (vLocalSystemData->vFPGASimulateEncoderRate == 0)
	if (!vLocalConfigurationData->vContinuousFeedEncoderRate)
		ReportErrorMessage("Error-Eject Container No Current Container ",cEMailInspx,32000);

		//brh temp for testing
		if (vLocalConfigurationData->vSimulateLightControlBoard)
			ProcessEjectNotification(TempContainer->vEjectPosition[2], 3);

}

bool CSerialPort::IsPastBeltPostion(WORD TempCurrentBeltPosition, WORD TempDecisionPoint, 
	WORD TempBodyTrigger)
{
	if (TempBodyTrigger <= TempDecisionPoint)
	{
		if ((TempCurrentBeltPosition >= TempDecisionPoint) || 
			(TempCurrentBeltPosition < TempBodyTrigger))
			return true;
		else
			return false;
	}
	else
	{
		if ((TempCurrentBeltPosition >= TempDecisionPoint) && 
			(TempCurrentBeltPosition < TempBodyTrigger))
			return true;
		else
			return false;
	}
}

CContainer* CSerialPort::AddNewContainer(UINT TempBTPosition)
{
	CContainer *TempContainer = NULL;
	if (vGlobalCurrentProduct)
	if (ThereIsEnoughMemory(sizeof(CContainer), "New Container"))
	{
		TempContainer = new CContainer;
		if (TempContainer)
		{
			//ReportErrorMessage("New Container Created", cWriteToLog, 0);

			DWORD TempMemoryMargin = 2500000;
			//create memory for new reject and copy data to it
			//TempContainer->vTheImages = vLocalSystemData->vITIPCDig->CreateCImageDataObject(0, true, vLocalSystemData->vITIPCDig->vDisplayBufferSizeInPixels);

			TempContainer->vBodyTriggerPosition = TempBTPosition;

			TempContainer->vTimeOutPosition = BPAdd(vGlobalCurrentProduct->vEndOfLineTimeOut * vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier, TempBTPosition);

			if (vLocalConfigurationData->vResynchronizeEjectors)
			if (vLocalSystemData->vInAutoSetup)
			if (TempContainer->vTimeOutPosition < 15 * 12 * vGlobalPixelsPerUnit) //if end of line is less than 15 feet in setup mode, set to 12 feet to find resynch ejectors
				TempContainer->vTimeOutPosition = BPAdd((WORD)(15 * 12 * vGlobalPixelsPerUnit), TempBTPosition);


			if (vLocalConfigurationData->vResynchronizeEjectors)
			if (vLocalSystemData->vInAutoSetup)
			if (TempContainer->vTimeOutPosition < 200 * vGlobalPixelsPerUnit)
				TempContainer->vTimeOutPosition = (WORD)(200 * vGlobalPixelsPerUnit);

			TempContainer->vStartImagePosition = BPAdd(vGlobalCurrentProduct->vBTToImagePosition, TempBTPosition);

			TempContainer->vEndImagePosition = BPAdd(vGlobalCurrentProduct->vImageWidthPosition, TempContainer->vStartImagePosition);

			//br ht for testing only, next line
			//TempContainer->vEjectPosition[0] = BPAdd(vGlobalCurrentProduct->
			//	vEjectorDistanceFromTriggerInPixels[0], TempBTPosition);

			for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
			{
				if (vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled)
				{
					if (vGlobalCurrentProduct->vExternalDetectorEnable[TempLoop])
					{
						TempContainer->vStartExternalDetectorPosition[TempLoop] = BPAdd(vGlobalCurrentProduct->vExternalDetectorWindowStartPosition[TempLoop], TempBTPosition);
						if (vGlobalCurrentProduct->vExternalDetectorWindowEndPosition[TempLoop] > 0)
							TempContainer->vEndExternalDetectorPosition[TempLoop] = BPAdd(vGlobalCurrentProduct->vExternalDetectorWindowEndPosition[TempLoop], TempBTPosition);
						else
							TempContainer->vEndExternalDetectorPosition[TempLoop] = TempContainer->vStartExternalDetectorPosition[TempLoop];
					}
				}
			}
			vLocalSystemData->vContainerCollection->AddTail(TempContainer);
		}
	}
	return TempContainer;
}

void CSerialPort::SendDigitalLineInterruptEnableDisable(BYTE TempLine, BYTE TempEnable, WORD TempPosition)
{  //only enable and disables first 7 bits of auxiliary detectors
	if ((!vLocalSystemData->vKeepExternalDetectorsEnabled) ||
		(TempEnable))
	{ //don't send disabled if keep detectors enabled.
		if ((!TempEnable) ||  //if enabling and have room for both enable and disable, then send
			(vLocalSystemData->vOutstandingAuxiliaryDetectorEnables + 1 < 
			cMaximumOutstandingAuxiliaryDetectorEnables))
		{
			vLocalSystemData->vOutstandingAuxiliaryDetectorEnables++;
			tSerialCommand TempCommand;
			if (TempEnable)
				TempCommand[0] = 0x12;	//Enable
			else
				TempCommand[0] = 0x13;	//Disable
			TempCommand[1] = (BYTE)(TempPosition >> 8);
			TempCommand[2] = (BYTE)TempPosition;
			//this is which bit number to enable or disable
			//TempLine is zero based in uC, is one based in SBC so subtract one
			TempCommand[3] = TempLine - 1; 
			SendSerialCommand(TempCommand);
			Sleep(cSendCommandSleepTime);
		}
		else
		{
			ReportErrorMessage("Error-Auxiliary Detector Queue Full, Missed Inspection",cWriteToLog,32000);
		}
	}
}

void CSerialPort::TallyEject(CContainer *TempContainer)
{		
	if (TempContainer)
	if ((TempContainer->vToBeEjected) || (TempContainer->vToBeEjectedButDisabled))
	{
		if (!TempContainer->vTheImage)
			ReportErrorMessage("TempContainer->vTheImage point=Null in TallyEject: " + dtoa(TempContainer->vBodyTriggerPosition, 0), cEMailInspx, 0);
		else
		if (TempContainer->vEjectorNumber)
		{
			if (TempContainer->vToBeEjectedButDisabled)
				vLocalSystemData->vMissedEjectsBecauseDisabledCount++;
			else
			{
				vLocalSystemData->vEjectCountTotal++;
			}
			vLocalSystemData->vTotalContainers++;
			vLocalSystemData->vImproveLearnTotalContainers++;
			vLocalSystemData->vRejectCountTotal++;

			if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
			if (TempContainer->vTheImage->vMultiLaneLaneNumber)
			{
				if (TempContainer->vToBeEjectedButDisabled)
					vLocalSystemData->vMultiLaneTotalDisabledEjectsCounts[TempContainer->vTheImage->vMultiLaneLaneNumber - 1]++;
				else
					vLocalSystemData->vMultiLaneTotalEjectsCounts[TempContainer->vTheImage->vMultiLaneLaneNumber - 1]++;

				//if (TempContainer->vTheImage->vContainerEjectReason <= vGlobalCurrentProduct->vNumberOfInspections)
				//if (TempContainer->vTheImage->vContainerEjectReason)
				//if (TempContainer->vToBeEjectedButDisabled)
				//	vLocalSystemData->vMultiLaneInspectionDisabledCounts[TempContainer->vTheImage->vMultiLaneLaneNumber - 1][TempContainer->vTheImage->vContainerEjectReason - 1]++;
				//else
				//	vLocalSystemData->vMultiLaneInspectionCounts[TempContainer->vTheImage->vMultiLaneLaneNumber - 1][TempContainer->vTheImage->vContainerEjectReason - 1]++;

				for (BYTE TempInspectionLoop = 0; TempInspectionLoop < vGlobalCurrentProduct->vNumberOfInspections; TempInspectionLoop++)
				if (TempContainer->vTheImage->vRejectedForThisInspection[TempInspectionLoop])
				if (TempContainer->vToBeEjectedButDisabled)
					vLocalSystemData->vMultiLaneInspectionDisabledCounts[TempContainer->vTheImage->vMultiLaneLaneNumber - 1][TempInspectionLoop]++;
				else
					vLocalSystemData->vMultiLaneInspectionCounts[TempContainer->vTheImage->vMultiLaneLaneNumber - 1][TempInspectionLoop]++;
			}

			if (TempContainer->vTheImage->vContainerEjectReason > cMaximumRejectReason)
			{
				vLocalSystemData->vProcessingErrorCount++;
				ReportErrorMessage("Processing Error, Tally Eject, Bad Eject Reason", cWriteToLog,32000);
			}
			//else
			//if (TempContainer->vToBeEjectedButDisabled)
			//	vLocalSystemData->vXRayCriteriaCountButDisabled[vLocalSystemData->vCountToUseForEjectReason[TempContainer->vTheImage->vContainerEjectReason]]++;
			//else
			//	vLocalSystemData->vXRayCriteriaCount[vLocalSystemData->vCountToUseForEjectReason[TempContainer->vTheImage->vContainerEjectReason]]++;

			//vLocalSystemData->vXRayCriteriaCountTotal[vLocalSystemData->vCountToUseForEjectReason[TempContainer->vTheImage->vContainerEjectReason]]++;

			//wrong shape, so tally counter
			//if ((TempContainer->vTheImage->vContainerEjectReason >= cEjectSlippedContainer) &&
			//	(TempContainer->vTheImage->vContainerEjectReason < cEjectAuxilaryDetectorOffset))
			//	vLocalSystemData->vImageReferenceErrorCount++;

			for (BYTE TempLoop = 0; TempLoop < vGlobalCurrentProduct->vNumberOfInspections; TempLoop++)
			if (TempContainer->vTheImage->vRejectedForThisInspection[TempLoop])
			{
				if (vLocalConfigurationData->vEnableEjectors)
					vLocalSystemData->vXRayCriteriaCount[vLocalSystemData->vCountToUseForEjectReason[cEjectInspectionOffset + TempLoop]]++;
				else
					vLocalSystemData->vXRayCriteriaCountButDisabled[vLocalSystemData->vCountToUseForEjectReason[cEjectInspectionOffset + TempLoop]]++;

				vLocalSystemData->vXRayCriteriaCountTotal[vLocalSystemData->vCountToUseForEjectReason[cEjectInspectionOffset + TempLoop]]++;
			}
			if (TempContainer->vTheImage->vContainerEjectReason >= cEjectAuxilaryDetectorOffset)
			{
				if (TempContainer->vTheImage->vRejectDisabled)
					vLocalSystemData->vXRayCriteriaCountButDisabled[vLocalSystemData->vCountToUseForEjectReason[TempContainer->vTheImage->vContainerEjectReason]]++;
				else
					vLocalSystemData->vXRayCriteriaCount[vLocalSystemData->vCountToUseForEjectReason[TempContainer->vTheImage->vContainerEjectReason]]++;
				vLocalSystemData->vXRayCriteriaCountTotal[vLocalSystemData->vCountToUseForEjectReason[TempContainer->vTheImage->vContainerEjectReason]]++;
			}
			if (!TempContainer->vToBeEjectedButDisabled)
			for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
			{
				//if (!TempContainer->vGotEjectedNotice)
				//	ReportErrorMessage("Error-Did not get Eject Notice from uC",cWriteToLog,32000);
				//for each ejector, if this container is to be ejected by it
				if (TempContainer->vEjectorNumber & (1 << TempLoop))
				if (vLocalConfigurationData->vEjector[TempLoop].vEnabled)
				{
					vLocalSystemData->vEjectorCount[TempLoop]++;
					if (vLocalConfigurationData->vEjector[TempLoop].vConfirmEnabled) 
					{
						if (TempContainer->vGotEjectConfirmedNotice)
						{
							vLocalSystemData->vEjectorConfirmCount[TempLoop]++;
							vLocalSystemData->vTotalConfirmed++;
						}
						else
						{
							if (vLocalConfigurationData->vStopRunningOnUnConfirmedEject)
							{
								if (vLocalConfigurationData->vDiverterEjector)
									SendEjectorOnOffCommand(vLocalConfigurationData->vDiverterEjector,0);
								vLocalSystemData->vDiverterOn = false;
								if (vLocalSystemData->vLogFile.vLogSerialData)
									WriteToLogFileWithBeltPosition("Turn Diverter Off", vLocalSystemData->vCurrentBeltPosition);
								::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
#ifdef CompileInHTTPClientOperationsGuardian
								if (vLocalConfigurationData->vEnableOperationsGuardian)
								{
									vGlobalOGErrorCode = 401;
									vGlobalOGLocationString = "ejector";
									::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageEjectorFault, cOGSevereMessageEjectorFault);
								}
#endif
								ReportErrorMessage("Error-Eject Not Confirmed, Stopped Inspection", cError,0);
								//ReportErrorMessage("Error-Eject Not Confirmed, Stopped Inspection", cEMailInspx, 32000);
							}
							vLocalSystemData->vTotalUnConfirmed++;
							
							//got unconfirmed eject, activate alarm two if unconfirmed eject alarm is set
							if (vLocalConfigurationData->vUseXofYAlarmOutputForUnconfirmedEject)
							{
								TurnAlarmOnOff(2,true);
							}
							
							ReportErrorMessage("Error - Eject Not Confirmed, " + vLocalSystemData->vContainerString + ": " + dtoa(TempContainer->vBodyTriggerPosition,0), cError,0);
							//ReportErrorMessage("Error - Eject Not Confirmed, " + vLocalSystemData->vContainerString + ": " + 
							//	dtoa(TempContainer->vContainerNumber,0), cEMailInspx, 32000);
						}
					}
				}
				else
				{
					vLocalSystemData->vProcessingErrorCount++;
					ReportErrorMessage("Error - Can't Eject, Ejector Disabled: " + 
						vLocalConfigurationData->vEjector[TempLoop].vName, cEMailInspx, 32000);
				}
			}
		}
		else
		{
			if (TempContainer->vToBeEjectedButDisabled)
			if (TempContainer->vTheImage)
			{
				if (TempContainer->vTheImage->vContainerEjectReason > cMaximumRejectReason)
				{
					vLocalSystemData->vProcessingErrorCount++;
					ReportErrorMessage("Processing Error, Tally Eject, Bad Eject Reason", cWriteToLog,32000);
				}
				else //improve learn inspection will not have ejector assigned
					vLocalSystemData->vXRayCriteriaCountButDisabled[
						vLocalSystemData->vCountToUseForEjectReason[TempContainer->vTheImage->vContainerEjectReason]]++;

				vLocalSystemData->vXRayCriteriaCountTotal[vLocalSystemData->vCountToUseForEjectReason[TempContainer->vTheImage->vContainerEjectReason]]++;

				if (vLocalSystemData->vSystemRunMode != cCalibratingDetectorOffsetMode)
				if (vLocalSystemData->vSystemRunMode != cConditionXRayTubeMode)
				{
					vLocalSystemData->vGoodCountTotal++;
					vLocalSystemData->vTotalContainers++;
					vLocalSystemData->vImproveLearnTotalContainers++;
				}
				if (TempContainer->vRealImage)
					vLocalSystemData->vRealImageCountTotal++;
			}
			else
			{
				vLocalSystemData->vProcessingErrorCount++;
				ReportErrorMessage("Error-Bad Ejector Number", cEMailInspx,32000);
			}
		}
	}
}

void CSerialPort::RemoveContainerFromCollection()
{
	WORD TempNumberOfContainers = vLocalSystemData->vContainerCollection->GetCount();
	if (TempNumberOfContainers > 0)
	{
		CContainer *TempContainer = vLocalSystemData->vContainerCollection->GetHead();
		if (vLocalSystemData->vLogFile.vLogSerialData)
		if (!vLocalSystemData->vSimulating)
		{
			if (vLocalConfigurationData->vResynchronizeEjectors)
				WriteToLogFileWithBeltPosition("Removed Resync Container " + dtoa(TempContainer->vContainerNumber, 0) + ", cen: " + dtoa(TempContainer->vContainerNumberToEject, 0), TempContainer->vBodyTriggerPosition);
			else
				WriteToLogFileWithBeltPosition("Removed Container " + dtoa(TempContainer->vContainerNumber, 0), TempContainer->vBodyTriggerPosition);
		}

		TempContainer = vLocalSystemData->vContainerCollection->RemoveHead();
		delete TempContainer;
		if (TempNumberOfContainers == 1)
		{
			vLocalSystemData->vOutstandingEjects = 0;
			vLocalSystemData->vOutstandingAuxiliaryDetectorEnables = 0;

			if (vLocalConfigurationData->vResynchronizeEjectors)  //reset the container count because there are no more containers in the tunnel
			{
				ClearEjectorSynchronizationCount();
				if (vLocalSystemData->vLogFile.vLogSerialData)
					WriteToLogFileWithBeltPosition("Reset Resynchronize because last container exited the tunnel", vLocalSystemData->vCurrentBeltPosition);
			}
		}
	}
}

void CSerialPort::SendEjectCommand(BYTE TempEjector, WORD TempPosition)
{
	if ((TempEjector < 1) || (TempEjector & 0x20))//|| (TempEjector > 32)) //if zero or 5th bit set, something wrong.  Bit 6 and 7 turn diverter off and on
		ReportErrorMessage("Error-Bad Ejector Number : " + dtoa(TempEjector, 0),cEMailInspx,32000);
	else
	{
		tSerialCommand TempCommand;
		TempCommand[0] = 0x11;
		TempCommand[1] = (BYTE)(TempPosition >> 8);
		TempCommand[2] = (BYTE)TempPosition;
		TempCommand[3] = TempEjector;
		SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTime);
	}
}

void CSerialPort::SendResynchronizedEjectCommand(BYTE TempContainerNumber, BYTE TempEjector, WORD TempDelay)
{
	if ((TempEjector < 1) || (TempEjector > cNumberOfEjectors))
		ReportErrorMessage("Error-Bad Ejector Number",cEMailInspx,32000);
	else
	{
		tSerialCommand TempCommand;

		if (TempEjector == 1)
			TempCommand[0] = 0x2C;
		else
			TempCommand[0] = 0x2D;

		TempCommand[1] = TempContainerNumber;

		TempCommand[2] = ((BYTE)(TempEjector << 4)) + ((BYTE)((TempDelay >> 8) & 0x0F));  //upper 4 bits are ejector number, lower 4 bits are MSBits of delay between photo eye and ejecting
		TempCommand[3] = (BYTE)TempDelay;
		SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTime);
	}
}

void CSerialPort::WriteToLogFileWithBeltPosition(CString TempString, WORD TempContainerPosition)
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
	{
		TempString = TempString + " cont: " + dtoa(TempContainerPosition, 0) + " at: " + dtoa(vLocalSystemData->vCurrentBeltPosition, 0);
		vLocalSystemData->vLogFile.WriteToLogFile(TempString,cDebugMessage);
	}
}
/*
void CSerialPort::EndTheThread()
{
	//AfxEndThread(0);
	//return(0);
}
*/
void CSerialPort::SetSimulatedEncoderRate(WORD TempRate)
{
	//must do when start to select real encoder
	if (TempRate > vLocalSystemData->vBeltRateMaximum)
		TempRate = (WORD)(vLocalSystemData->vBeltRateMaximum * .95);

	ReportErrorMessage("4. Set Simulated Encoder to: " + dtoa(TempRate),cAction,0);

	tSerialCommand TempCommand;
	BYTE TempUpperWord = TempRate >> 8;
	BYTE TempLowerWord = (BYTE)TempRate;
	TempCommand[0] = 0x34;
	TempCommand[1] = TempUpperWord;
	TempCommand[2] = TempLowerWord;
	TempCommand[3] = 0;
	SendSerialCommand(TempCommand);
	Sleep(cSendCommandSleepTime);
}

void CSerialPort::SendCommTest()
{
	tSerialCommand TempCommand;
	TempCommand[0] = 0x31;
	TempCommand[1] = vTestByte;
	vLastSent[0] = vTestByte;
	vTestByte++;
	TempCommand[2] = vTestByte;
	vLastSent[1] = vTestByte;
	vTestByte++;
	TempCommand[3] = vTestByte;
	vLastSent[2] = vTestByte;
	vTestByte++;
	SendSerialCommand(TempCommand);
	Sleep(cSendCommandSleepTime);
	vSendCount++;
	if (vSendingContinuous)
	{
		if ((vSendCount % 50) == 0)
			::PostThreadMessage(vGlobalCallingThreadID,cSentCommTestMessage,
				vLastCommTestStatusOK,vLastCommTestStatusOK);
	}
	else
		::PostThreadMessage(vGlobalCallingThreadID,cSentCommTestMessage,
			vLastCommTestStatusOK,vLastCommTestStatusOK);
}

void CSerialPort::SendEjectorOnOffCommand(DWORD TempEjectorNumber,BYTE TempOn)
{
	tSerialCommand TempCommand;
	TempCommand[0] = 0x23;  //write to FPGA Address
	if (TempEjectorNumber > 16)
		TempCommand[1] = 0xC9;  //FPGA address 0xC808 for ejector 17 to 30
	else
		TempCommand[1] = 0x9C;  //FPGA address 0x900C

	DWORD TempBitMapMask = 1 << (TempEjectorNumber - 1);

	if (TempOn)
	{
		vLocalSystemData->vCurrentEjectorOnStatus = vLocalSystemData->vCurrentEjectorOnStatus | TempBitMapMask;
		//if (TempEjectorNumber == 2)
		//	vLocalSystemData->vCurrentEjectorOnStatus = vLocalSystemData->vCurrentEjectorOnStatus | 2;
		//else
		//if (TempEjectorNumber == 3)
		//	vLocalSystemData->vCurrentEjectorOnStatus = vLocalSystemData->vCurrentEjectorOnStatus | 4;
		//else  //TempAlarmNumber == 1
		//	vLocalSystemData->vCurrentEjectorOnStatus = vLocalSystemData->vCurrentEjectorOnStatus | 1;
	}
	else
	{
		TempBitMapMask = ~TempBitMapMask; //bitwise invert
		vLocalSystemData->vCurrentEjectorOnStatus = vLocalSystemData->vCurrentEjectorOnStatus & TempBitMapMask;
		//if (TempEjectorNumber == 2)
		//	vLocalSystemData->vCurrentEjectorOnStatus = vLocalSystemData->vCurrentEjectorOnStatus & 0xFD;
		//else
		//if (TempEjectorNumber == 3)
		//	vLocalSystemData->vCurrentEjectorOnStatus = vLocalSystemData->vCurrentEjectorOnStatus & 0xFB;
		//else  //TempAlarmNumber == 1
		//	vLocalSystemData->vCurrentEjectorOnStatus = vLocalSystemData->vCurrentEjectorOnStatus & 0xFE;
	}
	if (TempEjectorNumber > 16)
	{
		TempCommand[2] = (BYTE)(vLocalSystemData->vCurrentEjectorOnStatus >> 24);
		TempCommand[3] = (BYTE)(vLocalSystemData->vCurrentEjectorOnStatus >> 16);
	}
	else
	{
		TempCommand[2] = (BYTE)(vLocalSystemData->vCurrentEjectorOnStatus >> 8);
		TempCommand[3] = (BYTE)vLocalSystemData->vCurrentEjectorOnStatus;
	}

	if ((TempEjectorNumber < 2) || (TempEjectorNumber > 3) || ((vGlobalCurrentProduct) && (vGlobalCurrentProduct->vEjectorDwellTime[TempEjectorNumber - 2] > 0))) //normal ejector
	{
		SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTime);
	}
	else
	{
		//need to send a pulse to the next ejector to get it to extend for Shuttleworth Conveyor, this is only used if ejectors are disabled, stoped inspecting, or select a product
		if (!TempOn)
		{
			if (TempEjectorNumber == 3)
				vLocalSystemData->vCurrentEjectorOnStatus = vLocalSystemData->vCurrentEjectorOnStatus | 4;
			else  
				vLocalSystemData->vCurrentEjectorOnStatus = vLocalSystemData->vCurrentEjectorOnStatus | 2;
			TempCommand[3] = (BYTE)vLocalSystemData->vCurrentEjectorOnStatus;
			SendSerialCommand(TempCommand);
			Sleep(cSendCommandSleepTime);
			::PostThreadMessage(vGlobalCallingThreadID,cTurnOffEjectorMessage,TempEjectorNumber,TempEjectorNumber);

			ReportErrorMessage("Started Pulse to De-activate Diverter: " + dtoa(TempEjectorNumber, 0), cAction, 0);
			Sleep(100); //must wait for pulse to do work as will kill pulse when ejectors are disabled
		}
	}
}

void CSerialPort::TurnOnOffYellowLamp(BYTE TempOn) 
{
	//BYTE TempTurnOn = 0;
	//if (TempOn)
	//	TempTurnOn = 2;
	//BYTE TempIsOn = vLocalSystemData->vCurrentLampStatus & 2;
	//if (TempIsOn ^ TempTurnOn)  //xor so only if different, send command
	{
		tSerialCommand TempCommand;
		TempCommand[0] = 0x23;  //write to FPGA Address
		TempCommand[1] = 0xC8;  //FPGA Address 0xC008
		TempCommand[2] = 0;
		if (TempOn)
			vLocalSystemData->vCurrentLampStatus = vLocalSystemData->vCurrentLampStatus | 2;
		else
			vLocalSystemData->vCurrentLampStatus = vLocalSystemData->vCurrentLampStatus & 0xFD;
		TempCommand[3] = vLocalSystemData->vCurrentLampStatus;
		if (!vFlashingLights)
			SendSerialCommand(TempCommand);
	}
	//if (TempOn)
	//	ReportErrorMessage("Turn Yellow Lamp On", cWriteToLog,0);
	//else
	//	ReportErrorMessage("Turn Yellow Lamp Off", cWriteToLog,0);
	//if (vFlashingLights)
	//	ReportErrorMessage("Flashing Lights, Did not change lights, just state", cWriteToLog,0);
	/*
	BYTE TempTurnOn = 0;
	if (TempOn)
		TempTurnOn = 2;
	BYTE TempIsOn = vLocalSystemData->vCurrentLampStatus & 2;
	//if (TempIsOn ^ TempTurnOn)  //xor so only if different, send command
	{
		tSerialCommand TempCommand;
		TempCommand[0] = 0x23;  //write to FPGA Address
		TempCommand[1] = 0xC8;  //FPGA Address 0xC008
		TempCommand[2] = 0;
		if (TempOn)
			vLocalSystemData->vCurrentLampStatus = vLocalSystemData->vCurrentLampStatus | 2;
		else
			vLocalSystemData->vCurrentLampStatus = vLocalSystemData->vCurrentLampStatus & 0xFD;
		TempCommand[3] = vLocalSystemData->vCurrentLampStatus;
		if (!vFlashingLights)
			SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTime);
	}
	*/
}

void CSerialPort::TurnOnOffRedLamp(BYTE TempOn) 
{
	//BYTE TempTurnOn = 0;
	//if (TempOn)
	//	TempTurnOn = 1;
	//BYTE TempIsOn = vLocalSystemData->vCurrentLampStatus & 1;
	//if (TempIsOn ^ TempTurnOn)  //xor so only if different, send command
	//{
		tSerialCommand TempCommand;
		TempCommand[0] = 0x23;  //write to FPGA Address
		TempCommand[1] = 0xC8;  //FPGA Address 0xC008
		TempCommand[2] = 0;
		if (TempOn)
			vLocalSystemData->vCurrentLampStatus = vLocalSystemData->vCurrentLampStatus | 1;
		else
			vLocalSystemData->vCurrentLampStatus = vLocalSystemData->vCurrentLampStatus & 0xFE;
		TempCommand[3] = vLocalSystemData->vCurrentLampStatus;
		if (!vFlashingLights)
			SendSerialCommand(TempCommand);
	//}
	//if (TempOn)
	//	ReportErrorMessage("Turn Red Lamp On", cWriteToLog,0);
	//else
	//	ReportErrorMessage("Turn Red Lamp Off", cWriteToLog,0);
	//if (vFlashingLights)
	//	ReportErrorMessage("Flashing Lights, Did not change lights, just state", cWriteToLog,0);
	/*
	BYTE TempTurnOn = 0;
	if (TempOn)
		TempTurnOn = 1;
	BYTE TempIsOn = vLocalSystemData->vCurrentLampStatus & 1;
	//if (TempIsOn ^ TempTurnOn)  //xor so only if different, send command
	{
		tSerialCommand TempCommand;
		TempCommand[0] = 0x23;  //write to FPGA Address
		TempCommand[1] = 0xC8;  //FPGA Address 0xC008
		TempCommand[2] = 0;
		if (TempOn)
			vLocalSystemData->vCurrentLampStatus = vLocalSystemData->vCurrentLampStatus | 1;
		else
			vLocalSystemData->vCurrentLampStatus = vLocalSystemData->vCurrentLampStatus & 0xFE;
		TempCommand[3] = vLocalSystemData->vCurrentLampStatus;
		if (!vFlashingLights)
			SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTime);
	}
	*/
}

void CSerialPort::TurnOnOffBlueLamp(BYTE TempOn) 
{
	//BYTE TempTurnOn = 0;
	//if (TempOn)
	//	TempTurnOn = 4;
	//BYTE TempIsOn = vLocalSystemData->vCurrentLampStatus & 4;
	//if (TempIsOn ^ TempTurnOn)  //xor so only if different, send command
	{
		tSerialCommand TempCommand;
		TempCommand[0] = 0x23;  //write to FPGA Address
		TempCommand[1] = 0xC8;  //FPGA Address 0xC008
		TempCommand[2] = 0;
		if (TempOn)
			vLocalSystemData->vCurrentLampStatus = vLocalSystemData->vCurrentLampStatus | 4;
		else
			vLocalSystemData->vCurrentLampStatus = vLocalSystemData->vCurrentLampStatus & 0xFB;
		TempCommand[3] = vLocalSystemData->vCurrentLampStatus;
		if (!vFlashingLights)
			SendSerialCommand(TempCommand);
	}
	//if (TempOn)
	//	ReportErrorMessage("Turn Blue Lamp On", cWriteToLog,0);
	//else
	//	ReportErrorMessage("Turn Blue Lamp Off", cWriteToLog,0);
	//if (vFlashingLights)
	//	ReportErrorMessage("Flashing Lights, Did not change lights, just state", cWriteToLog,0);
	/*
	BYTE TempTurnOn = 0;
	if (TempOn)
		TempTurnOn = 4;
	BYTE TempIsOn = vLocalSystemData->vCurrentLampStatus & 4;
	//if (TempIsOn ^ TempTurnOn)  //xor so only if different, send command
	{
		tSerialCommand TempCommand;
		TempCommand[0] = 0x23;  //write to FPGA Address
		TempCommand[1] = 0xC8;  //FPGA Address 0xC008
		TempCommand[2] = 0;
		if (TempOn)
			vLocalSystemData->vCurrentLampStatus = vLocalSystemData->vCurrentLampStatus | 4;
		else
			vLocalSystemData->vCurrentLampStatus = vLocalSystemData->vCurrentLampStatus & 0xFB;
		TempCommand[3] = vLocalSystemData->vCurrentLampStatus;
		if (!vFlashingLights)
			SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTime);
	}
	*/
}

void CSerialPort::EnableDisableEjectors(BYTE TempEnable)
{
	//check for any diverters active as must deactivate
	if (!TempEnable)
	//if fpga version high enough
	{
		//for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors - 1; TempLoop++)
		for (BYTE TempLoop = 0; TempLoop < 1; TempLoop++)
		if (vLocalSystemData->vDiverterActive[TempLoop])
		//if ((vMainWindowPointer) && (vMainWindowPointer->vEjectorNumberToTurnOff == 0))
		{
			vLocalSystemData->vDiverterActive[TempLoop] = false;
			SendEjectorOnOffCommand(TempLoop + 2, 0); 
			ReportErrorMessage("De-activate Diverter: " + dtoa(TempLoop + 2, 0) + " now because ejectors were disabled", cAction, 0);
		}
	}

	tSerialCommand TempCommand;
	TempCommand[0] = 0x23;  //write to FPGA Address
	TempCommand[1] = 0xCA;  //FPGA Address 0xC00A
	TempCommand[2] = 0;
	if (TempEnable)
		TempCommand[3] = 1;
	else
		TempCommand[3] = 0;
	SendSerialCommand(TempCommand);
	Sleep(cSendCommandSleepTime);

	if (TempEnable)
	{
		if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) ||
			(vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))  //turn on while in setup so conveyor or pump runs if interlocked
			TurnAlarmOnOff(4,true);//turn on system inspecting bit
	}
	else
	{
		if (vLocalSystemData->vSystemRunMode != cAutoSetupRunningSystem)  //turn on while in setup so conveyor or pump runs if interlocked
			TurnAlarmOnOff(4,false);//turn off system inspecting bit
	}
}

void CSerialPort::TurnAlarmOnOff(BYTE TempAlarmNumber,bool TempOn)
{
	tSerialCommand TempCommand;
	TempCommand[0] = 0x23;  //write to FPGA Address
	TempCommand[1] = 0xC4;  //FPGA Address 0xC004
	TempCommand[2] = 0;
	if (TempOn)
	{
		if (TempAlarmNumber == 1)
			vLocalSystemData->vCurrentAlarmStatus = vLocalSystemData->vCurrentAlarmStatus | 1;  //x of y Alarm 1
		else  
		if (TempAlarmNumber == 2)
			vLocalSystemData->vCurrentAlarmStatus = vLocalSystemData->vCurrentAlarmStatus | 2;  //x of y Alarm 2
		else
		if (TempAlarmNumber == 3)
			vLocalSystemData->vCurrentAlarmStatus = vLocalSystemData->vCurrentAlarmStatus | 4; //Critical Alarm
		else
		if (TempAlarmNumber == 4)
			vLocalSystemData->vCurrentAlarmStatus = vLocalSystemData->vCurrentAlarmStatus | 8; //System Inspecting

		//5 is a spare Opto output module 3 output 4 was Shutter Control in initial design, but never used
	}
	else
	{
		if (TempAlarmNumber == 1)
			vLocalSystemData->vCurrentAlarmStatus = vLocalSystemData->vCurrentAlarmStatus & 0xF8;
		else  
		if (TempAlarmNumber == 2)
			vLocalSystemData->vCurrentAlarmStatus = vLocalSystemData->vCurrentAlarmStatus & 0xF8;
		else
		if (TempAlarmNumber == 3)
			vLocalSystemData->vCurrentAlarmStatus = vLocalSystemData->vCurrentAlarmStatus & 0xF8;
		else
		if (TempAlarmNumber == 4)
			vLocalSystemData->vCurrentAlarmStatus = vLocalSystemData->vCurrentAlarmStatus & 0xF7;
	}
	TempCommand[3] = vLocalSystemData->vCurrentAlarmStatus;
	SendSerialCommand(TempCommand);
	Sleep(cSendCommandSleepTime);
}

void CSerialPort::TurnCanStopOnOff(DWORD TempCanStopNumber,bool TempOn)
{
	tSerialCommand TempCommand;
	TempCommand[0] = 0x23;  //write to FPGA Address
	TempCommand[1] = 0xCC;  //FPGA Address 0xC00C
	TempCommand[2] = 0;

	DWORD TempBitMapMask = 1 << (TempCanStopNumber - 1);

	if (TempOn)
	{
		vLocalSystemData->vCurrentCanStopStatus = vLocalSystemData->vCurrentCanStopStatus | TempBitMapMask;
		//if (TempCanStopNumber == 1)
		//	vLocalSystemData->vCurrentCanStopStatus = vLocalSystemData->vCurrentCanStopStatus | 1;
		//else  
		//if (TempCanStopNumber == 2)
		//	vLocalSystemData->vCurrentCanStopStatus = vLocalSystemData->vCurrentCanStopStatus | 2;
		//else
		//if (TempCanStopNumber == 3)
		//	vLocalSystemData->vCurrentCanStopStatus = vLocalSystemData->vCurrentCanStopStatus | 4;
	}
	else
	{
		TempBitMapMask = ~TempBitMapMask; //bitwise invert
		vLocalSystemData->vCurrentCanStopStatus = vLocalSystemData->vCurrentCanStopStatus & TempBitMapMask;

		//if (TempCanStopNumber == 1)
		//	vLocalSystemData->vCurrentCanStopStatus = vLocalSystemData->vCurrentCanStopStatus & 0xFE;
		//else  
		//if (TempCanStopNumber == 2)
		//	vLocalSystemData->vCurrentCanStopStatus = vLocalSystemData->vCurrentCanStopStatus & 0xFD;
		//else
		//if (TempCanStopNumber == 3)
		//	vLocalSystemData->vCurrentCanStopStatus = vLocalSystemData->vCurrentCanStopStatus & 0xFB;
	}
	TempCommand[3] = (BYTE)vLocalSystemData->vCurrentCanStopStatus;
	SendSerialCommand(TempCommand);
	Sleep(cSendCommandSleepTime);
}

void CSerialPort::SendEncoderDividerTouC()
{
	WORD TempEncoderDivider = (BYTE)vLocalConfigurationData->vEncoderDivider;
	vGlobalPixelsPerUnit = vLocalConfigurationData->vPixelsPerUnit;

	if (vGlobalCurrentProduct)
	{
		if (vGlobalCurrentProduct->vOverScanMultiplier > 1)
		{
			if (vLocalConfigurationData->vEncoderDivider % 2 != 0)// % is mod function  //if not divisible by 2, then can't do
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\nYou cannot use Overscan because the\nEncoder Divider factor is not divisible by two.\nYou must re-calibrate the encoder with the divider divisible by 2.";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
			else
			if (TempEncoderDivider >= vGlobalCurrentProduct->vOverScanMultiplier)
				TempEncoderDivider = (BYTE)(((double)TempEncoderDivider / vGlobalCurrentProduct->vOverScanMultiplier) + .5);
		}

		
		if (vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors)
		{
			if (vLocalConfigurationData->vEncoderDividerForPoint4mmMode)
			{
				TempEncoderDivider = vLocalConfigurationData->vEncoderDividerForPoint4mmMode;
				if (vLocalConfigurationData->vPixelsPerUnitForPoint4mmMode)
					vGlobalPixelsPerUnit = vLocalConfigurationData->vPixelsPerUnitForPoint4mmMode;
				else
					vGlobalPixelsPerUnit = vLocalConfigurationData->vPixelsPerUnit * 2;
				
			}
			else
			{
				if (TempEncoderDivider % 2 != 0)// % is mod function  //if not divisible by 2, then can't do
				{
					//CNoticeDialog TempNoticeDialog;
					//TempNoticeDialog.vNoticeText = "\nX-Scan .4 mm Mode Detectors can not scan\nat fast rate because the\nEncoder Divider factor is not divisible by two.\nYou must re-calibrate the encoder with the divider divisible by 2.";
					//TempNoticeDialog.vType = cErrorMessage;
					//TempNoticeDialog.DoModal();
				}
				else
				if (TempEncoderDivider >= 2)
				{
					TempEncoderDivider = (BYTE)(((double)TempEncoderDivider / 2.0) + .5);
					vGlobalPixelsPerUnit = vLocalConfigurationData->vPixelsPerUnit * 2;
				}
			}
		}
	}
	vLocalSystemData->vCurrentEncoderDivider = TempEncoderDivider;

	tSerialCommand TempCommand;
	TempCommand[0] = 0x4F;
	TempCommand[1] = (BYTE)TempEncoderDivider;
	TempCommand[2] = 0;
	TempCommand[3] = 0;
	SendSerialCommand(TempCommand);
	Sleep(cSendCommandSleepTime);
}

void CSerialPort::SendDACDetectorOffset(BYTE TempDetectorNumber)
{
	BYTE TempDetector = TempDetectorNumber;
	if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
		TempDetector = TempDetector % 9;

	BYTE TempDetectorAddress = 0x6A;
	//combine system detector offset for this detector with the product offset and limit
	double TempTotalOffset = vLocalSystemData->vDACDetectorOffset[TempDetectorNumber];
	if (TempTotalOffset > 100)
		TempTotalOffset = 100;
	if (TempTotalOffset < -100)
		TempTotalOffset = -100;

	int TempMaximumOffsetValue = 2048;
	//if (cNumberOfOffsetBits == 12)
	//	TempMaximumOffsetValue = 4096;

	//if (vGlobalFPGAVersion10Point0OrHigher) //FPGA 9.2 Uses 11 bits by default, can't be less than 11 bits
	//{
	//	if (cNumberOfOffsetBits == 10)
	//		TempMaximumOffsetValue = 1024;
	//	if (cNumberOfOffsetBits == 9)
	//		TempMaximumOffsetValue = 512;
	//}

	int TempOffsetWord = (WORD)((TempTotalOffset /  100.0) * TempMaximumOffsetValue);
	TempOffsetWord = TempOffsetWord + vLocalSystemData->vDriftCompensationDACOffset;

	if (TempOffsetWord < 0)
		TempOffsetWord = 0;
	if (TempOffsetWord > TempMaximumOffsetValue)
		TempOffsetWord = TempMaximumOffsetValue;

	BYTE TempUpperByte = (BYTE)((TempOffsetWord >> 8) | 0xC0);
	BYTE TempLowerByte = (BYTE)TempOffsetWord;

	vLocalSystemData->vLastProgrammedDACDetectorOffset[TempDetectorNumber] = TempOffsetWord;
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Send DAC-Offset: " + dtoa(vLocalSystemData->vDACDetectorOffset[TempDetectorNumber],2) + " Detector: " + dtoa(TempDetectorNumber + 1,0),cDebugMessage);

	if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
	{

		switch (TempDetectorNumber)
		{
			case 0: StartMiddleBeamControl(); TempDetectorAddress = 0x24; break;
			case 1: TempDetectorAddress = 0x22; break;//uC Board back 3 detectors
			case 2: TempDetectorAddress = 0x20; break;
			case 3: TempDetectorAddress = 0x24; break;
			case 4: TempDetectorAddress = 0x22; break;
			case 5: TempDetectorAddress = 0x20; break;//for CaseInspector, center 3 detectors on daughter board

			case 6: TempDetectorAddress = 0x24; break;
			case 7: TempDetectorAddress = 0x22; break;
			case 8: TempDetectorAddress = 0x20; break; //for CaseInspector, front 3 detectors on daughter board

			//case 9: StartMiddleBeamControl(); TempDetectorAddress = 0x26; break;
			//case 10: TempDetectorAddress = 0x68; break;//uC Board back 3 detectors
			//case 11: TempDetectorAddress = 0x6A; break;
			//case 12: TempDetectorAddress = 0x26; break;//for CaseInspector, center 3 detectors on daughter board
			//case 13: TempDetectorAddress = 0x68; break;
			//case 14: TempDetectorAddress = 0x6A; break;

			//case 15: TempDetectorAddress = 0x26; break; //for CaseInspector, front 3 detectors on daughter board
			//case 16: TempDetectorAddress = 0x68; break;
			//case 17: TempDetectorAddress = 0x6A; break;
		}

		tSerialCommand TempCommand;
		TempCommand[0] = 0x23;

		{
			if (TempDetectorNumber > 5)
				TempCommand[1] = 0xDA;  //uC Board
			else
				TempCommand[1] = 0xAA; //daughter board in front of forte and case inpectors
		}

		TempCommand[2] = 0x00;
		TempCommand[3] = TempDetectorAddress;
		SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTime);

		TempCommand[0] = 0x23;
		if (TempDetectorNumber > 5)
			TempCommand[1] = 0xDC; 
		else
			TempCommand[1] = 0xAC; //daughter board in front of forte and case inpectors

		TempCommand[2] = TempUpperByte;
		TempCommand[3] = TempLowerByte;
		SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTime);
		//ReportErrorMessage("DAC:" + dtoa(TempDetectorNumber + 1,0) + " Offset:" + dtoa(TempTotalOffset,2) + " Command Value = " + dtoa(TempUpperByte,0) + "," + dtoa(TempLowerByte,0), cWriteToLog,0);

		if (TempDetectorNumber == 2)
			EndMiddleBeamControl();
	}
	else
	{ //not a case inspector
		if (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)
		{
			switch (TempDetectorNumber)
			{
				case 8: TempDetectorAddress = 0x24; break; //grand daughter top T2
				case 7: TempDetectorAddress = 0x22; break;
				case 6: TempDetectorAddress = 0x20; break; //grand daughter bottom
				case 5: TempDetectorAddress = 0x6A; break; //uC top
				case 4: TempDetectorAddress = 0x68; break;
				case 3: TempDetectorAddress = 0x26; break;
				case 2: TempDetectorAddress = 0x24; break; 
				case 1: TempDetectorAddress = 0x22; break;
				case 0: TempDetectorAddress = 0x20; break; //uC bottom
			}
		}
		else
		if (vLocalConfigurationData->vScanTracType == cAllegroScanTrac)
		{
			switch (TempDetectorNumber)
			{
				case 8: TempDetectorAddress = 0x20; break;
				case 7: TempDetectorAddress = 0x22; break;
				case 6: TempDetectorAddress = 0x24; break;
				case 5: TempDetectorAddress = 0x26; break;
				case 4: TempDetectorAddress = 0x68; break;
				case 3: TempDetectorAddress = 0x6A; break;
				case 2: TempDetectorAddress = 0x20; break; //for Allegro, front 3 detectors on daughter board
				case 1: TempDetectorAddress = 0x22; break;
				case 0: TempDetectorAddress = 0x24; break;
			}
		}
		else
		{
			switch (TempDetectorNumber)
			{
				case 11: TempDetectorAddress = 0x20; break;
				case 10: TempDetectorAddress = 0x22; break;
				case 9: TempDetectorAddress = 0x24; break;
				case 8: TempDetectorAddress = 0x26; break;
				case 7: TempDetectorAddress = 0x68; break;
				case 6: TempDetectorAddress = 0x6A; break;
				case 5: TempDetectorAddress = 0x20; break; //for forte, front 6 detectors on daughter board
				case 4: TempDetectorAddress = 0x22; break;
				case 3: TempDetectorAddress = 0x24; break;
				case 2: TempDetectorAddress = 0x26; break;
				case 1: TempDetectorAddress = 0x68; break;
				case 0: TempDetectorAddress = 0x6A; break;
			}
		}

		tSerialCommand TempCommand;
		TempCommand[0] = 0x23;
		if (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)
		{
			if (TempDetectorNumber < 6)
				TempCommand[1] = 0xDA; //uc Board on bottom
			else
				TempCommand[1] = 0xAA; //grand daughter board on top with only 3 detectors
		}
		else
		if (vLocalConfigurationData->vScanTracType == cAllegroScanTrac)
		{
			if (TempDetectorNumber > 2)
				TempCommand[1] = 0xDA; //uc Board in back
			else
				TempCommand[1] = 0xAA; //daughter board in front with only 3 detectors
		}
		else
		{
			if (vLocalConfigurationData->vNumberOfXRayDetectors == 6)
				TempCommand[1] = 0xDA; //only ucBoard
			else
			if (TempDetectorNumber > 5)
				TempCommand[1] = 0xDA; //uc board  in back
			else
				TempCommand[1] = 0xAA; //daughter board in front
		}
		TempCommand[2] = 0x00;
		TempCommand[3] = TempDetectorAddress;
		SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTime);

		if (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)
		{
			if (TempDetectorNumber < 6)
				TempCommand[1] = 0xDC; //uc Board on bottom
			else
				TempCommand[1] = 0xAC; //grand daughter board on top with only 3 detectors
		}
		else
		if (vLocalConfigurationData->vScanTracType == cAllegroScanTrac)
		{
			if (TempDetectorNumber > 2)
				TempCommand[1] = 0xDC; //uc board in back
			else
				TempCommand[1] = 0xAC; //daughter board in front
		}
		else
		{
			if (vLocalConfigurationData->vNumberOfXRayDetectors == 6)
				TempCommand[1] = 0xDC; //only uc Board
			else
			if (TempDetectorNumber > 5)
				TempCommand[1] = 0xDC; //uc board in back
			else
				TempCommand[1] = 0xAC; //daughter board in front
		}
		TempCommand[2] = TempUpperByte;
		TempCommand[3] = TempLowerByte;
		SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTime);
	}
}

void CSerialPort::ReadDACDetectorOffset(BYTE TempDetectorNumber)
{
	BYTE TempDetectorAddress = 0x6A;
	//WORD TempOffsetWord = (WORD)TempOffset;

	//BYTE TempUpperByte = (BYTE)((TempOffsetWord >> 8) | 0xC0);
	//BYTE TempLowerByte = (BYTE)TempOffsetWord;
	if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
	{
		switch (TempDetectorNumber)
		{
			case 2: TempDetectorAddress = 0x20; break;
			case 1: TempDetectorAddress = 0x22; break;//uC Board back 3 detectors
			case 0: StartMiddleBeamControl(); TempDetectorAddress = 0x24; break;
			case 5: TempDetectorAddress = 0x20; break;//for CaseInspector, center 3 detectors on daughter board
			case 4: TempDetectorAddress = 0x22; break;
			case 3: TempDetectorAddress = 0x24; break;

			case 8: TempDetectorAddress = 0x20; break; //for CaseInspector, front 3 detectors on daughter board
			case 7: TempDetectorAddress = 0x22; break;
			case 6: TempDetectorAddress = 0x24; break;
		}

		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("Reading DAC-Offset Detector: " + dtoa(TempDetectorNumber + 1,0),cDebugMessage);

		tSerialCommand TempCommand;
		TempCommand[0] = 0x23;
		if (TempDetectorNumber > 5)
			TempCommand[1] = 0xDA; //uC board
		else
			TempCommand[1] = 0xAA; //daughter board in front of forte and case inpectors

		TempCommand[2] = 0x00;
		TempCommand[3] = TempDetectorAddress;
		SendSerialCommand(TempCommand);//send command to select DAC chip
		Sleep(cSendCommandSleepTime);

		TempCommand[0] = 0x23;
		if (TempDetectorNumber > 5)
			TempCommand[1] = 0xDC; 
		else
			TempCommand[1] = 0xAC; //daughter card in front

		TempCommand[2] = 0;
		TempCommand[3] = 0;
		SendSerialCommand(TempCommand);
		//Sleep(cSendCommandSleepTime);
		Sleep(cSendCommandSleepTime);
		//ReportErrorMessage("DAC:" + dtoa(TempDetectorNumber + 1,0) + " Offset:" + dtoa(TempOffset,2) + " Command Value = " + dtoa(TempUpperByte,0) + "," + dtoa(TempLowerByte,0), cWriteToLog,0);

		if (TempDetectorNumber == 2)
			EndMiddleBeamControl();
	}
	else
	{ //not a case inspector
		if (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)
		{
			switch (TempDetectorNumber)
			{
				case 8: TempDetectorAddress = 0x24; break; //grand daughter top T2
				case 7: TempDetectorAddress = 0x22; break;
				case 6: TempDetectorAddress = 0x20; break; //grand daughter bottom
				case 5: TempDetectorAddress = 0x6A; break; //uC top
				case 4: TempDetectorAddress = 0x68; break;
				case 3: TempDetectorAddress = 0x26; break;
				case 2: TempDetectorAddress = 0x24; break; 
				case 1: TempDetectorAddress = 0x22; break;
				case 0: TempDetectorAddress = 0x20; break; //uC bottom
			}
		}
		else
		if (vLocalConfigurationData->vScanTracType == cAllegroScanTrac)
		{
			switch (TempDetectorNumber)
			{
				case 8: TempDetectorAddress = 0x20; break;
				case 7: TempDetectorAddress = 0x22; break;
				case 6: TempDetectorAddress = 0x24; break;
				case 5: TempDetectorAddress = 0x26; break;
				case 4: TempDetectorAddress = 0x68; break;
				case 3: TempDetectorAddress = 0x6A; break;
				case 2: TempDetectorAddress = 0x20; break; //for Allegro, front 3 detectors on daughter board
				case 1: TempDetectorAddress = 0x22; break;
				case 0: TempDetectorAddress = 0x24; break;
			}
		}
		else
		{
			switch (TempDetectorNumber)
			{
				case 0: TempDetectorAddress = 0x6A; break;
				case 1: TempDetectorAddress = 0x68; break;
				case 2: TempDetectorAddress = 0x26; break;
				case 3: TempDetectorAddress = 0x24; break;
				case 4: TempDetectorAddress = 0x22; break;
				case 5: TempDetectorAddress = 0x20; break;
				case 6: TempDetectorAddress = 0x6A; break; //for forte, front 6 detectors on daughter board
				case 7: TempDetectorAddress = 0x68; break;
				case 8: TempDetectorAddress = 0x26; break;
				case 9: TempDetectorAddress = 0x24; break;
				case 10: TempDetectorAddress = 0x22; break;
				case 11: TempDetectorAddress = 0x20; break;
			}
		}

		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("Send Read DAC-Offset Detector: " + dtoa(TempDetectorNumber + 1,0),cDebugMessage);

		tSerialCommand TempCommand;
		TempCommand[0] = 0x23;
		if (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)
		{
			if (TempDetectorNumber < 6)
				TempCommand[1] = 0xDA; //uc Board on bottom
			else
				TempCommand[1] = 0xAA; //grand daughter board on top with only 3 detectors
		}
		else
		if (vLocalConfigurationData->vScanTracType == cAllegroScanTrac)
		{
			if (TempDetectorNumber > 2)
				TempCommand[1] = 0xDA; //uc Board in back
			else
				TempCommand[1] = 0xAA; //daughter board in front with only 3 detectors
		}
		else
		{
			if (vLocalConfigurationData->vNumberOfXRayDetectors == 6)
				TempCommand[1] = 0xDA; //pnly has a uc board
			else
			if (TempDetectorNumber > 5)
				TempCommand[1] = 0xDA; //uc Board in back
			else
				TempCommand[1] = 0xAA; //daughter board in front
		}
		TempCommand[2] = 0x00;
		TempCommand[3] = TempDetectorAddress;
		SendSerialCommand(TempCommand);//send command to select DAC chip
		Sleep(cSendCommandSleepTime);

		if (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)
		{
			if (TempDetectorNumber < 6)
				TempCommand[1] = 0xDC; //uc Board on bottom
			else
				TempCommand[1] = 0xAC; //grand daughter board on top with only 3 detectors
		}
		else
		if (vLocalConfigurationData->vScanTracType == cAllegroScanTrac)
		{
			if (TempDetectorNumber > 2)
				TempCommand[1] = 0xDC; //uc Board in back
			else
				TempCommand[1] = 0xAC; //daughter board in front with only 3 detectors
		}
		else
		{
			if (vLocalConfigurationData->vNumberOfXRayDetectors == 6)
				TempCommand[1] = 0xDC; 
			else
			if (TempDetectorNumber > 5)
				TempCommand[1] = 0xDC; //uc Board in back
			else
				TempCommand[1] = 0xAC; //daughter board in front
		}
		TempCommand[2] = 0x80; //data to read the DAC value
		TempCommand[3] = 0x00;
		SendSerialCommand(TempCommand);
		//Sleep(cSendCommandSleepTime);
		Sleep(cCalibrateOffsetSleepAfterCommandTime);
		/*
		ReportErrorMessage("DAC:" + dtoa(TempDetectorNumber + 1,0) + " Offset:" + 
			dtoa(TempOffset,2) + " Command Value = " +
			dtoa(TempUpperByte,0) +
			"," + dtoa(TempLowerByte,0), cWriteToLog,0);
			*/
		TempCommand[0] = 0x30; //read the latest DAC value from the micro controller
		TempCommand[1] = 0x78;
		TempCommand[2] = 0x00;
		TempCommand[3] = 0x00;
		SendSerialCommand(TempCommand);
	vLocalSystemData->vWaitingForDACValueReturn = true;
		//Sleep(cSendCommandSleepTime);
	//Sleep(cSendCommandSleepTime);
	}
}

void CSerialPort::SendAbsoluteDACDetectorOffset(BYTE TempDetectorNumber, double TempOffset)
{
	BYTE TempDetectorAddress = 0x6A;
	WORD TempOffsetWord = (WORD)TempOffset;

	////don't allow over 9 bits
	//if (TempOffsetWord >= 0x200)
	//	TempOffsetWord = TempOffsetWord & 0x1FF;

	BYTE TempUpperByte = (BYTE)((TempOffsetWord >> 8) | 0xC0);
	BYTE TempLowerByte = (BYTE)TempOffsetWord;
	vLocalSystemData->vLastProgrammedDACDetectorOffset[TempDetectorNumber] = TempOffsetWord;
	if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
	{
		switch (TempDetectorNumber)
		{
			case 2: TempDetectorAddress = 0x20; break;
			case 1: TempDetectorAddress = 0x22; break;//uC Board back 3 detectors
			case 0: StartMiddleBeamControl(); TempDetectorAddress = 0x24; break;
			case 5: TempDetectorAddress = 0x20; break;//for CaseInspector, center 3 detectors on daughter board
			case 4: TempDetectorAddress = 0x22; break;
			case 3: TempDetectorAddress = 0x24; break;

			case 8: TempDetectorAddress = 0x20; break; //for CaseInspector, front 3 detectors on daughter board
			case 7: TempDetectorAddress = 0x22; break;
			case 6: TempDetectorAddress = 0x24; break;
		}

		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("Send Abs DAC-Offset: " + dtoa(TempOffset,2) + " Detector: " + dtoa(TempDetectorNumber + 1,0),cDebugMessage);

		tSerialCommand TempCommand;
		TempCommand[0] = 0x23;
		if (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)
		{
			if (TempDetectorNumber < 6)
				TempCommand[1] = 0xDA; //uc Board on bottom
			else
				TempCommand[1] = 0xAA; //grand daughter board on top with only 3 detectors
		}
		else
		if (vLocalConfigurationData->vScanTracType == cAllegroScanTrac)
		{
			if (TempDetectorNumber > 2)
				TempCommand[1] = 0xDA; //uc Board in back
			else
				TempCommand[1] = 0xAA; //daughter board in front with only 3 detectors
		}
		else
		{
			if (TempDetectorNumber > 5)
				TempCommand[1] = 0xDA; //uC board
			else
				TempCommand[1] = 0xAA; //daughter board in front of forte and case inpectors
		}

		TempCommand[2] = 0x00;
		TempCommand[3] = TempDetectorAddress;
		SendSerialCommand(TempCommand);//send command to select DAC chip
		Sleep(cSendCommandSleepTime);

		TempCommand[0] = 0x23;
		if (TempDetectorNumber > 5)
			TempCommand[1] = 0xDC; 
		else
			TempCommand[1] = 0xAC; //daughter card in front

		TempCommand[2] = TempUpperByte;
		TempCommand[3] = TempLowerByte;
		SendSerialCommand(TempCommand);
		//Sleep(cSendCommandSleepTime);
		Sleep(cSendCommandSleepTime);
		//ReportErrorMessage("DAC:" + dtoa(TempDetectorNumber + 1,0) + " Offset:" + dtoa(TempOffset,2) + " Command Value = " + dtoa(TempUpperByte,0) + "," + dtoa(TempLowerByte,0), cWriteToLog,0);

		if (TempDetectorNumber == 2)
			EndMiddleBeamControl();
	}
	else
	{ //not a case inspector
		if (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)
		{
			switch (TempDetectorNumber)
			{
				case 8: TempDetectorAddress = 0x24; break; //grand daughter top T2
				case 7: TempDetectorAddress = 0x22; break;
				case 6: TempDetectorAddress = 0x20; break; //grand daughter bottom
				case 5: TempDetectorAddress = 0x6A; break; //uC top
				case 4: TempDetectorAddress = 0x68; break;
				case 3: TempDetectorAddress = 0x26; break;
				case 2: TempDetectorAddress = 0x24; break; 
				case 1: TempDetectorAddress = 0x22; break;
				case 0: TempDetectorAddress = 0x20; break; //uC bottom
			}
		}
		else
		if (vLocalConfigurationData->vScanTracType == cAllegroScanTrac)
		{
			switch (TempDetectorNumber)
			{
				case 8: TempDetectorAddress = 0x20; break;
				case 7: TempDetectorAddress = 0x22; break;
				case 6: TempDetectorAddress = 0x24; break;
				case 5: TempDetectorAddress = 0x26; break;
				case 4: TempDetectorAddress = 0x68; break;
				case 3: TempDetectorAddress = 0x6A; break;
				case 2: TempDetectorAddress = 0x20; break; //for Allegro, front 3 detectors on daughter board
				case 1: TempDetectorAddress = 0x22; break;
				case 0: TempDetectorAddress = 0x24; break;
			}
		}
		else
		{
			switch (TempDetectorNumber)
			{
				case 0: TempDetectorAddress = 0x6A; break;
				case 1: TempDetectorAddress = 0x68; break;
				case 2: TempDetectorAddress = 0x26; break;
				case 3: TempDetectorAddress = 0x24; break;
				case 4: TempDetectorAddress = 0x22; break;
				case 5: TempDetectorAddress = 0x20; break;
				case 6: TempDetectorAddress = 0x6A; break; //for forte, front 6 detectors on daughter board
				case 7: TempDetectorAddress = 0x68; break;
				case 8: TempDetectorAddress = 0x26; break;
				case 9: TempDetectorAddress = 0x24; break;
				case 10: TempDetectorAddress = 0x22; break;
				case 11: TempDetectorAddress = 0x20; break;
			}
		}

		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("Send Abs DAC-Offset: " + dtoa(TempOffset,2) + " Detector: " + dtoa(TempDetectorNumber + 1,0),cDebugMessage);

		tSerialCommand TempCommand;
		TempCommand[0] = 0x23;
		if (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)
		{
			if (TempDetectorNumber < 6)
				TempCommand[1] = 0xDA; //uc Board on bottom
			else
				TempCommand[1] = 0xAA; //grand daughter board on top with only 3 detectors
		}
		else
		if (vLocalConfigurationData->vScanTracType == cAllegroScanTrac)
		{
			if (TempDetectorNumber > 2)
				TempCommand[1] = 0xDA; //uc Board in back
			else
				TempCommand[1] = 0xAA; //daughter board in front with only 3 detectors
		}
		else
		{
			if (vLocalConfigurationData->vNumberOfXRayDetectors == 6)
				TempCommand[1] = 0xDA; //only a uc board
			else
			if (TempDetectorNumber > 5)
				TempCommand[1] = 0xDA; //uc board in back
			else
				TempCommand[1] = 0xAA; //daughter board in front
		}
		TempCommand[2] = 0x00;
		TempCommand[3] = TempDetectorAddress;
		SendSerialCommand(TempCommand);//send command to select DAC chip
		Sleep(cSendCommandSleepTime);

		TempCommand[0] = 0x23;
		if (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)
		{
			if (TempDetectorNumber < 6)
				TempCommand[1] = 0xDC; //uc Board on bottom
			else
				TempCommand[1] = 0xAC; //grand daughter board on top with only 3 detectors
		}
		else
		if (vLocalConfigurationData->vScanTracType == cAllegroScanTrac)
		{
			if (TempDetectorNumber > 2)
				TempCommand[1] = 0xDC; //uc Board in back
			else
				TempCommand[1] = 0xAC; //daughter board in front with only 3 detectors
		}
		else
		{
			if (vLocalConfigurationData->vNumberOfXRayDetectors == 6)
				TempCommand[1] = 0xDC; 
			else
			if (TempDetectorNumber > 5)
				TempCommand[1] = 0xDC; //daughter board in front
			else
				TempCommand[1] = 0xAC;
		}
		TempCommand[2] = TempUpperByte;
		TempCommand[3] = TempLowerByte;
		SendSerialCommand(TempCommand);
		//Sleep(cSendCommandSleepTime);
		Sleep(cSendCommandSleepTime);
		//ReportErrorMessage("DAC:" + dtoa(TempDetectorNumber + 1,0) + " Offset:" + dtoa(TempOffset,2) + " Command Value = " + dtoa(TempUpperByte,0) + "," + dtoa(TempLowerByte,0), cWriteToLog,0);
	}
}

void CSerialPort::ReadADCDetectorOffset(BYTE TempDetectorNumber)
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Read ADC Offset Detector: " + dtoa(TempDetectorNumber + 1, 0),cDebugMessage);

	tSerialCommand TempCommand;
	if (vGlobalFPGAVersion15Point31OrHigher)
	{  //new way have 6 chips, one for each detector
		//send A/d offset, set to zero since not using the offset in the ADC
		TempCommand[0] = 0x23;
		if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
		{
			switch (TempDetectorNumber)
			{
				case 2: TempCommand[1] = 0xBC; break;  //front detector on 36 inch is center beam from trio
				case 1: TempCommand[1] = 0xD6; break;
				case 0: StartMiddleBeamControl();  TempCommand[1] = 0xDE; break;
				case 5: TempCommand[1] = 0xBC; break; //middle detector daughter card is right beam from trio
				case 4: TempCommand[1] = 0xD6; break;
				case 3: TempCommand[1] = 0xDE; break;  
				case 8: TempCommand[1] = 0xB2; break; //back detector, uc
				case 7: TempCommand[1] = 0xB4; break;
				case 6: TempCommand[1] = 0xB6; break;
			}
		}
		else
		if (vLocalConfigurationData->vNumberOfXRayDetectors == 6)
		{
			switch (TempDetectorNumber)
			{
				case 5: TempCommand[1] = 0xB2; break; //uC Board
				case 4: TempCommand[1] = 0xB4; break;
				case 3: TempCommand[1] = 0xB6; break;
				case 2: TempCommand[1] = 0xF2; break;
				case 1: TempCommand[1] = 0xF4; break;
				case 0: TempCommand[1] = 0xF6; break;  
			}
		}
		else
		if (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)
		{
			switch (TempDetectorNumber)
			{
				case 8: TempCommand[1] = 0xDE; break;//top detector on 6 inch //daughter board T2
				case 7: TempCommand[1] = 0xD6; break;
				case 6: TempCommand[1] = 0xBC; break;  //6" daughter board
				case 5: TempCommand[1] = 0xF6; break;  //top detector on 12 inch //uC Board
				case 4: TempCommand[1] = 0xF4; break;
				case 3: TempCommand[1] = 0xF2; break;
				case 2: TempCommand[1] = 0xB6; break;
				case 1: TempCommand[1] = 0xB4; break;
				case 0: TempCommand[1] = 0xB2; break; //bottom detector//uC Board
			}
		}
		else
		if (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) //Allegro
		{
			switch (TempDetectorNumber)
			{
				case 8: TempCommand[1] = 0xB2; break; //back detector//uC Board
				case 7: TempCommand[1] = 0xB4; break;
				case 6: TempCommand[1] = 0xB6; break;
				case 5: TempCommand[1] = 0xF2; break;
				case 4: TempCommand[1] = 0xF4; break;
				case 3: TempCommand[1] = 0xF6; break;  //front detector on 12 inch //uC Board
				case 2: TempCommand[1] = 0xBC; break;  //6" daughter board
				case 1: TempCommand[1] = 0xD6; break;
				case 0: TempCommand[1] = 0xDE; break;//front detector on 6 inch //daughter board
			}
		}
		else
		if (vLocalConfigurationData->vNumberOfXRayDetectors == 12) //forte
		{
			switch (TempDetectorNumber)
			{
				case 11: TempCommand[1] = 0xB2; break; //back detector//uC Board
				case 10: TempCommand[1] = 0xB4; break;
				case 9: TempCommand[1] = 0xB6; break;
				case 8: TempCommand[1] = 0xF2; break;
				case 7: TempCommand[1] = 0xF4; break;
				case 6: TempCommand[1] = 0xF6; break;  //front detector on 12 inch //uC Board
				case 5: TempCommand[1] = 0xBC; break;  //daughter board
				case 4: TempCommand[1] = 0xD6; break;
				case 3: TempCommand[1] = 0xDE; break;
				case 2: TempCommand[1] = 0xE4; break;
				case 1: TempCommand[1] = 0xE6; break;
				case 0: TempCommand[1] = 0xEA; break;//front detector on 24 inch //daughter board
			}
		}
		if (vGlobalFPGAVersion18point0orAbove)
		{
			TempCommand[2] = 0xD0; //0xD000 puts ADC in Readout mode
			TempCommand[3] = 0x00; 
		}
		else
		{
			TempCommand[2] = 0x04; //0x0401 puts ADC in Readout mode
			TempCommand[3] = 0x01; 
		}
		SendSerialCommand(TempCommand);
		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("Send Readout ADC " + dtoa(TempDetectorNumber + 1,0), cDebugMessage);

		if (!vGlobalFPGAVersion18point0orAbove)
		{
			TempCommand[2] = 0x14; //0x01400 selects ADC offset register for Readout mode (0x0800 is to readout Gain Register)
			TempCommand[3] = 0x00; 
			SendSerialCommand(TempCommand);
		}

		Sleep(cSendCommandSleepTime);

		tSerialCommand TempReadCommand;
		//ReportErrorMessage("ADC:" + dtoa(TempDetectorNumber,0) + " Offset:" + dtoa(TempOffset,2) + " Command Value = " + dtoa(TempUpperByte,0) + "," + dtoa(TempLowerByte,0), cWriteToLog,0);
		TempReadCommand[0] = 0x30; //read the latest ADC value from the micro controller
		TempReadCommand[1] = 0x72;
		TempReadCommand[2] = 0x00;
		TempReadCommand[3] = 0x00;
		SendSerialCommand(TempReadCommand);

		vLocalSystemData->vWaitingForADCValueReturn = true;

		Sleep(cSendCommandSleepTime);

		if (!vGlobalFPGAVersion18point0orAbove)
		{
			TempCommand[2] = 0x04; //0x0400 end Readout Mode in ADC
			TempCommand[3] = 0x0; 
			SendSerialCommand(TempCommand);
		}

		if ((vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) && (TempDetectorNumber == 2))
			EndMiddleBeamControl();
	}
}

void CSerialPort::SetRadiationDoorManualControl(WORD TempControl)
{
	if ((vLocalConfigurationData->vRadiationShutterInSystem) ||
		(TempControl == cRadiationShutterOpen))
	if (vLocalSystemData->vRadiationDoorManualControlCurrentStatus != TempControl)
	{
		vLocalSystemData->vRadiationDoorManualControlCurrentStatus = TempControl;
		tSerialCommand TempCommand;
		TempCommand[0] = 0x23;
		TempCommand[1] = 0xE8;

		//for show demos
		if (vLocalConfigurationData->vSimulateLightControlBoard)
			TempControl  = cRadiationShutterOpen;

		TempCommand[2] = (BYTE)(TempControl >> 8);
		TempCommand[3] = (BYTE)(TempControl);
		SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTime);
		if (TempControl == cRadiationShutterOpen)
			ReportErrorMessage("Open Shutter", cWriteToLog,0);
		else
		if (TempControl == cRadiationShutterAutoControl)
			ReportErrorMessage("Auto Shutter", cWriteToLog,0);
		else
		if (TempControl == cRadiationShutterClose)
			ReportErrorMessage("Close Shutter", cWriteToLog,0);
		else
			ReportErrorMessage("Unknown Shutter", cWriteToLog,0);
	}
}

//void CSerialPort::SetAllADCDetectorOffsetsToZero()
//{
//	for (BYTE TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfXRayDetectors; TempLoop++)
//		vLocalSystemData->vADCDetectorOffset[TempLoop] = 0;
//
//	SendADCOffsets();
//}

//void CSerialPort::SetAllDACDetectorOffsets()
//{
//	BYTE TempLoopCount = vLocalConfigurationData->vNumberOfXRayDetectors;
//	//if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
//	//	TempLoopCount = 18;
//
//	for (BYTE TempLoop = 0; TempLoop < TempLoopCount; TempLoop++)
//		SendDACDetectorOffset(TempLoop); 
//}

void CSerialPort::SendContainerTriggerToImageDelay(CProduct *TempProduct)
{
	BYTE TempActiveHigh = 0;
	if ((vLocalConfigurationData->vBodyTrigger[TempProduct->vBodyTrigger - 1].vActiveHigh) ||
		(vLocalSystemData->vFPGAVersion10Point7OrHigher))  //always active high in 10.7 FPGA or above as invert input if active low
		TempActiveHigh = 0x80;
	WORD TempBTToImagePosition = (TempProduct->vBTToImagePosition);
	tSerialCommand TempCommand;
	TempCommand[0] = 0x18;
	TempCommand[1] = (BYTE)(TempBTToImagePosition >> 8);
	TempCommand[2] = (BYTE)TempBTToImagePosition;
	TempCommand[3] = TempActiveHigh;
	SendSerialCommand(TempCommand);
	Sleep(cSendCommandSleepTime);
}

/*

DAC Offset Programming

Two commands per offset
send 0x23 first then:
			Left						Right
Top 6	0xDA,0,0x20			0xAA,0,0x6A
		5	0xDA,0,0x22			0xAA,0,0x68
		4	0xDA,0,0x24			0xAA,0,0x26
		3	0xDA,0,0x26			0xAA,0,0x24
		2	0xDA,0,0x68			0xAA,0,0x22
Bot	1	0xDA,0,0x6A			0xAA,0,0x20
then send 0x23, 0xDC, Data Value for Left
or send 0x23, 0xAC, Data Value for Right



ADC Offset Programming
One command per offset
send 0x23 first then:

			Left			Right
Top 6	0xB2			0xEA
		5	0xB4			0xE6
		4	0xB6			0xE4
		3	0xF2			0xDE
		2	0xF4			0xD6
Bot	1	0xF6			0xBC

	The Next two bytes are the data value with 0x14 added to upper byte


*/
void CSerialPort::SendADCOffset(BYTE TempWhichDetectorForOffset, int TempOffsetIn)
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Send ADC-Offset: " + dtoa(TempOffsetIn,2) + " Detector: " + dtoa(TempWhichDetectorForOffset,0),cDebugMessage);

	tSerialCommand TempCommand;

	int TempOffset = TempOffsetIn;
	BYTE TempOffsetCode = 0;
	BYTE TempSign = 0;
	BYTE TempGrossOffset = 0;

	if (vGlobalFPGAVersion18point0orAbove)
	{ 
		if (TempOffset < -255)
			TempOffset = -255;
		if (TempOffset > 255)
			TempOffset = 255;
		if (TempOffsetIn < 0)
		{
			TempSign = 1;
			TempOffsetCode = -TempOffsetIn;
		}
		else
			TempOffsetCode = TempOffsetIn;
	}
	else
	{
		if (TempOffset < -700)
			TempOffset = -700;
		if (TempOffset > 500)
			TempOffset = 500;

		if (TempOffset <= -400)
		{
			TempGrossOffset = 3;
			TempOffset = TempOffset + 400;
		}
		else
		if (TempOffset <= -200)
		{
			TempGrossOffset = 2;
			TempOffset = TempOffset + 200;
		}
		else
		if (TempOffset >= 200)
		{
			TempGrossOffset = 1;
			TempOffset = TempOffset + -200;
		}

		if (TempOffset < 0)
		{
			TempSign = 0x80;
			TempOffset = -TempOffset;
		}

		TempOffsetCode = (BYTE)((DWORD)TempOffset * (DWORD)127 / (DWORD)300);
		TempOffsetCode = TempOffsetCode | TempSign;
	}

	if (vGlobalFPGAVersion18point0orAbove)
		vLocalSystemData->vLastProgrammedADCDetectorOffset[TempWhichDetectorForOffset] = TempOffsetIn & 0x1FF; //all 9 LSbits of data can be read back
	else
		vLocalSystemData->vLastProgrammedADCDetectorOffset[TempWhichDetectorForOffset] = TempOffsetCode & 0x3FC; //only 8 bits of 10 bits can be read back, not two lsbits

	if (vGlobalFPGAVersion10Point0OrHigher)
	{  //new way have 6 chips, one for each detector
		//send A/d offset, set to zero since not using the offset in the ADC
		TempCommand[0] = 0x23;
		if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
		{
			switch (TempWhichDetectorForOffset)
			{
				case 2: TempCommand[1] = 0xBC; break;  //front detector on 36 inch is center beam from trio
				case 1: TempCommand[1] = 0xD6; break;
				case 0: StartMiddleBeamControl();  TempCommand[1] = 0xDE; break;
				case 5: TempCommand[1] = 0xBC; break; //middle detector daughter card is right beam from trio
				case 4: TempCommand[1] = 0xD6; break;
				case 3: TempCommand[1] = 0xDE; break;  
				case 8: TempCommand[1] = 0xB2; break; //back detector, uc
				case 7: TempCommand[1] = 0xB4; break;
				case 6: TempCommand[1] = 0xB6; break;

				//case 17: TempCommand[1] = 0xF2; break; //back detector, uc
				//case 16: TempCommand[1] = 0xF4; break;
				//case 15: TempCommand[1] = 0xF6; break;
				//case 14: TempCommand[1] = 0xE4; break; //middle detector daughter card is right beam from trio
				//case 13: TempCommand[1] = 0xE6; break;
				//case 12: TempCommand[1] = 0xEA; break;  
				//case 11: TempCommand[1] = 0xE4; break;  //front detector on 36 inch is center beam from trio
				//case 10: TempCommand[1] = 0xE6; break;
				//case 9: StartMiddleBeamControl(); TempCommand[1] = 0xEA; break;
			}
		}
		else
		if (vLocalConfigurationData->vNumberOfXRayDetectors == 6)
		{
			switch (TempWhichDetectorForOffset)
			{
				case 5: TempCommand[1] = 0xB2; break; //uC Board
				case 4: TempCommand[1] = 0xB4; break;
				case 3: TempCommand[1] = 0xB6; break;
				case 2: TempCommand[1] = 0xF2; break;
				case 1: TempCommand[1] = 0xF4; break;
				case 0: TempCommand[1] = 0xF6; break;  
			}
		}
		else
		if (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)
		{
			switch (TempWhichDetectorForOffset)
			{
				case 8: TempCommand[1] = 0xDE; break;//top detector on 6 inch //daughter board T2
				case 7: TempCommand[1] = 0xD6; break;
				case 6: TempCommand[1] = 0xBC; break;  //6" daughter board
				case 5: TempCommand[1] = 0xF6; break;  //top detector on 12 inch //uC Board
				case 4: TempCommand[1] = 0xF4; break;
				case 3: TempCommand[1] = 0xF2; break;
				case 2: TempCommand[1] = 0xB6; break;
				case 1: TempCommand[1] = 0xB4; break;
				case 0: TempCommand[1] = 0xB2; break; //bottom detector//uC Board
			}
		}
		else
		if (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) //Allegro
		{
			switch (TempWhichDetectorForOffset)
			{
				case 8: TempCommand[1] = 0xB2; break; //back detector//uC Board
				case 7: TempCommand[1] = 0xB4; break;
				case 6: TempCommand[1] = 0xB6; break;
				case 5: TempCommand[1] = 0xF2; break;
				case 4: TempCommand[1] = 0xF4; break;
				case 3: TempCommand[1] = 0xF6; break;  //front detector on 12 inch //uC Board
				case 2: TempCommand[1] = 0xBC; break;  //6" daughter board
				case 1: TempCommand[1] = 0xD6; break;
				case 0: TempCommand[1] = 0xDE; break;//front detector on 6 inch //daughter board
			}
		}
		else
		if (vLocalConfigurationData->vNumberOfXRayDetectors == 12) //forte
		{
			switch (TempWhichDetectorForOffset)
			{
				case 11: TempCommand[1] = 0xB2; break; //back detector//uC Board
				case 10: TempCommand[1] = 0xB4; break;
				case 9: TempCommand[1] = 0xB6; break;
				case 8: TempCommand[1] = 0xF2; break;
				case 7: TempCommand[1] = 0xF4; break;
				case 6: TempCommand[1] = 0xF6; break;  //front detector on 12 inch //uC Board
				case 5: TempCommand[1] = 0xBC; break;  //daughter board
				case 4: TempCommand[1] = 0xD6; break;
				case 3: TempCommand[1] = 0xDE; break;
				case 2: TempCommand[1] = 0xE4; break;
				case 1: TempCommand[1] = 0xE6; break;
				case 0: TempCommand[1] = 0xEA; break;//front detector on 24 inch //daughter board
			}
		}

		if (vGlobalFPGAVersion18point0orAbove)
			TempCommand[2] = 0x50 + TempSign; //address for red offset plus the sign bit
		else
			TempCommand[2] = 0x14 + TempGrossOffset; //address for red offset

		TempCommand[3] = TempOffsetCode; //offset value
		SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTime);

		if ((vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) && (TempWhichDetectorForOffset == 2))
			EndMiddleBeamControl();
	}
	else
	{  //old way had 2 - 3 channel a/d chips
		//New ADC XRD9814
		BYTE TempTheDetector = 5 - TempWhichDetectorForOffset;
		BYTE TempOffsetAddress = 0x14 + ((TempTheDetector % 3) << 2);

		TempCommand[0] = 0x4C;
		TempCommand[1] = ((TempTheDetector) / 3) + 1; //which ADC Chip
		TempCommand[2] = TempOffsetAddress + TempGrossOffset; //address for adc offset
		TempCommand[3] = TempOffsetCode; //offset value
		SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTime);
	}
}

void CSerialPort::SendAbsoluteADCDetectorOffset(BYTE TempWhichDetector, int TempOffsetIn)
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Send Absolute ADC-Offset: " + dtoa(TempOffsetIn,2) + " Detector: " + dtoa(TempWhichDetector + 1,0),cDebugMessage);

	tSerialCommand TempCommand;
	if (vGlobalFPGAVersion18point0orAbove)
		vLocalSystemData->vLastProgrammedADCDetectorOffset[TempWhichDetector] = TempOffsetIn & 0x1FF; //all 9 LSbits of data can be read back
	else
		vLocalSystemData->vLastProgrammedADCDetectorOffset[TempWhichDetector] = TempOffsetIn & 0x3FC; //only 8 bits of 10 bits can be read back, not two lsbits

	if (vGlobalFPGAVersion10Point0OrHigher)
	{  //new way have 6 chips, one for each detector
		//send A/d offset, set to zero since not using the offset in the ADC
		TempCommand[0] = 0x23;
		if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
		{
			switch (TempWhichDetector)
			{
				case 2: TempCommand[1] = 0xBC; break;  //front detector on 36 inch is center beam from trio
				case 1: TempCommand[1] = 0xD6; break;
				case 0: StartMiddleBeamControl();  TempCommand[1] = 0xDE; break;
				case 5: TempCommand[1] = 0xBC; break; //middle detector daughter card is right beam from trio
				case 4: TempCommand[1] = 0xD6; break;
				case 3: TempCommand[1] = 0xDE; break;  
				case 8: TempCommand[1] = 0xB2; break; //back detector, uc
				case 7: TempCommand[1] = 0xB4; break;
				case 6: TempCommand[1] = 0xB6; break;

				//case 17: TempCommand[1] = 0xF2; break; //back detector, uc
				//case 16: TempCommand[1] = 0xF4; break;
				//case 15: TempCommand[1] = 0xF6; break;
				//case 14: TempCommand[1] = 0xE4; break; //middle detector daughter card is right beam from trio
				//case 13: TempCommand[1] = 0xE6; break;
				//case 12: TempCommand[1] = 0xEA; break;  
				//case 11: TempCommand[1] = 0xE4; break;  //front detector on 36 inch is center beam from trio
				//case 10: TempCommand[1] = 0xE6; break;
				//case 9: StartMiddleBeamControl(); TempCommand[1] = 0xEA; break;
			}
		}
		else
		if (vLocalConfigurationData->vNumberOfXRayDetectors == 6)
		{
			switch (TempWhichDetector)
			{
				case 5: TempCommand[1] = 0xB2; break; //uC Board
				case 4: TempCommand[1] = 0xB4; break;
				case 3: TempCommand[1] = 0xB6; break;
				case 2: TempCommand[1] = 0xF2; break;
				case 1: TempCommand[1] = 0xF4; break;
				case 0: TempCommand[1] = 0xF6; break;  
			}
		}
		else
		if (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)
		{
			switch (TempWhichDetector)
			{
				case 8: TempCommand[1] = 0xDE; break;//top detector on 6 inch //daughter board T2
				case 7: TempCommand[1] = 0xD6; break;
				case 6: TempCommand[1] = 0xBC; break;  //6" daughter board
				case 5: TempCommand[1] = 0xF6; break;  //top detector on 12 inch //uC Board
				case 4: TempCommand[1] = 0xF4; break;
				case 3: TempCommand[1] = 0xF2; break;
				case 2: TempCommand[1] = 0xB6; break;
				case 1: TempCommand[1] = 0xB4; break;
				case 0: TempCommand[1] = 0xB2; break; //bottom detector//uC Board
			}
		}
		else
		if (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) //Allegro
		{
			switch (TempWhichDetector)
			{
				case 8: TempCommand[1] = 0xB2; break; //back detector//uC Board
				case 7: TempCommand[1] = 0xB4; break;
				case 6: TempCommand[1] = 0xB6; break;
				case 5: TempCommand[1] = 0xF2; break;
				case 4: TempCommand[1] = 0xF4; break;
				case 3: TempCommand[1] = 0xF6; break;  //front detector on 12 inch //uC Board
				case 2: TempCommand[1] = 0xBC; break;  //6" daughter board
				case 1: TempCommand[1] = 0xD6; break;
				case 0: TempCommand[1] = 0xDE; break;//front detector on 6 inch //daughter board
			}
		}
		else
		if (vLocalConfigurationData->vNumberOfXRayDetectors == 12) //forte
		{
			switch (TempWhichDetector)
			{
				case 11: TempCommand[1] = 0xB2; break; //back detector//uC Board
				case 10: TempCommand[1] = 0xB4; break;
				case 9: TempCommand[1] = 0xB6; break;
				case 8: TempCommand[1] = 0xF2; break;
				case 7: TempCommand[1] = 0xF4; break;
				case 6: TempCommand[1] = 0xF6; break;  //front detector on 12 inch //uC Board
				case 5: TempCommand[1] = 0xBC; break;  //daughter board
				case 4: TempCommand[1] = 0xD6; break;
				case 3: TempCommand[1] = 0xDE; break;
				case 2: TempCommand[1] = 0xE4; break;
				case 1: TempCommand[1] = 0xE6; break;
				case 0: TempCommand[1] = 0xEA; break;//front detector on 24 inch //daughter board
			}
		}

		if (vGlobalFPGAVersion18point0orAbove)
		{
			BYTE TempSign = 0;
			if (TempOffsetIn & 0x100)
				TempSign = 1;

			TempCommand[2] = 0x50 + TempSign; //address for red offset plus the sign bit
		}
		else
			TempCommand[2] = 0x14 + ((TempOffsetIn & 0x3FF) >> 8); //address for red offset

		TempCommand[3] = (BYTE)TempOffsetIn; //offset value
		SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTime);

		if ((vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) && (TempWhichDetector == 2))
			EndMiddleBeamControl();
	}
	else
	{  //old way had 2 - 3 channel a/d chips
		//New ADC XRD9814
		BYTE TempTheDetector = 5 - TempWhichDetector;
		BYTE TempOffsetAddress = 0x14 + ((TempTheDetector % 3) << 2);

		TempCommand[0] = 0x4C;
		TempCommand[1] = ((TempTheDetector) / 3) + 1; //which ADC Chip
		TempCommand[2] = TempOffsetAddress + (TempOffsetIn << 8); //address for adc offset
		TempCommand[3] = (BYTE)TempOffsetIn; //offset value
		SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTime);
	}
}

void CSerialPort::ReadADCAnalogStatusValue()
{
	tSerialCommand TempCommand;
	TempCommand[0] = 0x30;
	TempCommand[1] = 0xFA; //read ADC Data
	TempCommand[2] = 0x00;
	TempCommand[3] = 0x00;
	SendSerialCommand(TempCommand);
	Sleep(cSendCommandSleepTimeSetup);

	if (vAnalogMonitorChannelToRead < 15)
		vAnalogMonitorChannelToRead++;
	else
		vAnalogMonitorChannelToRead = 0;

	WORD TempWord = 0x0835 + (vAnalogMonitorChannelToRead << 6);
	TempWord = TempWord << 4;
	TempCommand[0] = 0x23;
	TempCommand[1] = 0xF8; //Setup status ADC chip AD7490
	TempCommand[2] = (BYTE)(TempWord >> 8);
	TempCommand[3] = (BYTE)(TempWord);
	SendSerialCommand(TempCommand);
	Sleep(cSendCommandSleepTimeSetup);
}

void CSerialPort::ClearEncoderHistory()
{
	for (WORD TempLoop = 0; TempLoop < cNumberOfEncoderSamples; TempLoop++)
		vLocalSystemData->vEncoderSample[TempLoop] = 0;
}

void CSerialPort::TemporarilyDisableContainerTrigger()
{
	if (!vBeltPositionToEnableTriggerAfterTemporaryDisable)
	{
		vBeltPositionDisabledTrigger = vLocalSystemData->vCurrentBeltPosition;

		vBeltPositionToEnableTriggerAfterTemporaryDisable = 
			(WORD)(vLocalSystemData->vCurrentBeltPosition + vGlobalCurrentProduct->vEndOfLineBeltPosition + (12 * vGlobalPixelsPerUnit));

		ReportErrorMessage("*Clearing all containers in the tunnel by disabling the triggers", cEMailInspx,32000);
		ReportErrorMessage("*Disabled at: " + dtoa(vLocalSystemData->vCurrentBeltPosition,0) + ", enable at: " + 
			dtoa(vBeltPositionToEnableTriggerAfterTemporaryDisable,0) + " Containers: " + 
			dtoa(vLocalSystemData->vContainerCollection->GetCount(),0), cEMailInspx,32000);

		tSerialCommand TempCommand;
		TempCommand[0] = 0x1C;
		TempCommand[1] = 0x03;  //send mask for 16 digital lines in #3
		TempCommand[2] = 0x00;
		TempCommand[3] = 0;//vLocalSystemData->vDigitalInputLine3Mask;
		SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTimeSetup);

		TempCommand[1] = 0x04;  //send mask so uC know which bit is body trigger
		TempCommand[2] = 0x00;
		TempCommand[3] = 0;//vLocalSystemData->vCurrentBodyTriggerMask;
		SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTimeSetup);
	}
}

void CSerialPort::EnableTriggerAfterTemporaryDisable(bool TempForceRemoveContainers)
{
	ReportErrorMessage("Check if tunnel clear to Enabled Container Triggers after temporary disable, Containers: " + 
		dtoa(vLocalSystemData->vContainerCollection->GetCount(),0), cWriteToLog,0);

	if (vLocalSystemData->vContainerCollection->GetCount()) //if any containers in tunnel
	if (TempForceRemoveContainers) //if past belt position expected tunnel to be clear
		RemoveContainerFromCollection(); //remove a container

	if (!vLocalSystemData->vContainerCollection->GetCount()) //if no more containers in tunnel
	{
		vBeltPositionToEnableTriggerAfterTemporaryDisable = 0;

			//Send Interrupt Mask for body trigger
		tSerialCommand TempCommand;
		TempCommand[0] = 0x1C;
			//Don't enable encoder interrupt
		TempCommand[1] = 0x04;  //send mask so uC know which bit is body trigger
		TempCommand[2] = 0x00;
		TempCommand[3] = vLocalSystemData->vCurrentBodyTriggerMask;
		SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTimeSetup);

		TempCommand[1] = 0x03;  //send mask for 16 digital lines in #3 , body triggers
		TempCommand[2] = 0x00;
		TempCommand[3] = vLocalSystemData->vDigitalInputLine3Mask;
		SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTimeSetup);

		ReportErrorMessage("*Enabled Container Triggers after temporary disable", cEMailInspx,32000);
		ReportErrorMessage("*Belt Position: " + dtoa(vLocalSystemData->vCurrentBeltPosition, 0), cEMailInspx, 32000);
	}
}

void CSerialPort::ClearEjectorSynchronizationCount()
{
	if (vLocalConfigurationData->vResynchronizeEjectors)
	{
		//if (vLocalSystemData->vTotalContainers)
		//if (vLocalSystemData->vResynchronizeEjectorsCurrentTriggerCount != vLocalSystemData->vResynchronizeEjectorsCurrentEjectorSensorCount[0])
		//	ReportErrorMessage("Reset Resync Counts: T: " + dtoa(vLocalSystemData->vResynchronizeEjectorsCurrentTriggerCount,0) + ", S1: " + dtoa(vLocalSystemData->vResynchronizeEjectorsCurrentEjectorSensorCount[0],0) + ", S2: " +
		//		dtoa(vLocalSystemData->vResynchronizeEjectorsCurrentEjectorSensorCount[1],0), cEMailInspx, 32000);

		tSerialCommand TempCommand;
		TempCommand[0] = 0x2F;
		TempCommand[1] = 0;
		TempCommand[2] = 0;
		TempCommand[3] = 0;
		SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTime);

		vLocalSystemData->vResynchronizeEjectorsCurrentTriggerCount = 0;
		vLocalSystemData->vResynchronizeEjectorsCurrentEjectorSensorCount[0] = 0;
		vLocalSystemData->vResynchronizeEjectorsCurrentEjectorSensorCount[1] = 0;
		vLocalSystemData->vEjectorsNotSynchronized = false;

		if (vLocalSystemData->vLogFile.vLogSerialData)
			WriteToLogFileWithBeltPosition("Cleared the Resynchronizing Ejector Container Count", vLocalSystemData->vCurrentBeltPosition);
	}
}

void CSerialPort::CalculateDistanceToFirstSensor(WORD TempBeltPosition, BYTE TempContainerEjectNumber, BYTE TempEjectorNumber) //TempEjectorNumber is either zero or one
{
	if (vLocalConfigurationData->vResynchronizeEjectors)
	if (vLocalSystemData->vInAutoSetup)
	if (vLocalSystemData->vContainerCollection->GetCount() == 1)
	if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
	if (TempEjectorNumber > 1)
		ReportErrorMessage("CalculateDistanceToFirstSensor TempEjectorNumber too large, software bug would cause crash", cEMailInspx, 32000);
	else
	{
		CContainer *TempContainer = vLocalSystemData->vContainerCollection->GetHead();
		WORD TempBeltOffsetToThisEjector = BPSubtract(TempBeltPosition, TempContainer->vBodyTriggerPosition);

		bool TempEjectorUsed[cNumberOfEjectors];
		for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
			TempEjectorUsed[TempLoop] = false;

		for (BYTE TempLoop = 0; TempLoop < vGlobalCurrentProduct->vNumberOfInspections; TempLoop++)
		{
			if (vGlobalCurrentProduct->vInspection[TempLoop])
			if (vGlobalCurrentProduct->vInspection[TempLoop]->vEjector)
			{
				for (BYTE TempEjectorLoop = 0; TempEjectorLoop < cNumberOfEjectors; TempEjectorLoop++)
				if (vGlobalCurrentProduct->vInspection[TempLoop]->vEjector & (1 << TempEjectorLoop))
					TempEjectorUsed[TempEjectorLoop] = true;
			}
		}

		if (TempEjectorUsed[TempEjectorNumber]) //if this ejector is used, save its distance in the distance to have tunnel cleared on startup
		{
			if (vLocalConfigurationData->vBeltPositionDistancetoEjectorResynchronizationSensor != TempBeltOffsetToThisEjector)
			{

				//ReportErrorMessage("Set Resynchronizing Ejector Distance to Sensor to : " + dtoa(TempBeltOffsetToThisEjector, 2) + " was: " + 
				//dtoa(vLocalConfigurationData->vBeltPositionDistancetoEjectorResynchronizationSensor, 2), cEMailInspx, 32000);

				vLocalConfigurationData->vBeltPositionDistancetoEjectorResynchronizationSensor = TempBeltOffsetToThisEjector; //save the distance to the first sensor
			}
		}

		if (vLocalSystemData->vLogFile.vLogSerialData)
		{
			WriteToLogFileWithBeltPosition("Trigger to Resync Ejector " + dtoa(TempEjectorNumber + 1, 0) + " sensor distance: " + dtoa(TempBeltOffsetToThisEjector / vGlobalPixelsPerUnit, 0) + 
				"\" (" + dtoa(TempBeltOffsetToThisEjector, 0) + " encoder pulses)", vLocalSystemData->vCurrentBeltPosition);
		}

		double TempDistanceInInches = (TempBeltOffsetToThisEjector / vGlobalPixelsPerUnit) + 12.0; 
		if (TempDistanceInInches > 400)
			TempDistanceInInches = 250;

		if (vGlobalCurrentProduct->vEjectorDistanceFromTriggerInInches[TempEjectorNumber] < TempDistanceInInches)
		{
			double TempOldValue = vGlobalCurrentProduct->vEjectorDistanceFromTriggerInInches[TempEjectorNumber];
			if (vLocalSystemData->vLogFile.vLogSerialData)
				WriteToLogFileWithBeltPosition("Resync A Ejector " + dtoa(TempEjectorNumber + 1, 0) + " Position was: " + dtoa(vGlobalCurrentProduct->vEjectorDistanceFromTriggerInInches[TempEjectorNumber], 2) +
					" set to: " + dtoa(TempDistanceInInches, 2), vLocalSystemData->vCurrentBeltPosition);

			vGlobalCurrentProduct->SetEjectorBeltPositionOffset(TempEjectorNumber, (float)TempDistanceInInches);

			//ReportErrorMessage("Set Resynchronizing Ejector Sensor: " + dtoa(TempEjectorNumber + 1, 0) + " to : " + dtoa(TempDistanceInInches, 2) + " was: " + dtoa(TempOldValue, 2), cEMailInspx, 32000);
		}
		else
		if (vGlobalCurrentProduct->vEjectorDistanceFromTriggerInInches[TempEjectorNumber] > TempDistanceInInches + 30) //if more than 20 inches past new location, set to new location
		{
			double TempOldValue = vGlobalCurrentProduct->vEjectorDistanceFromTriggerInInches[TempEjectorNumber];
			if (vLocalSystemData->vLogFile.vLogSerialData)
				WriteToLogFileWithBeltPosition("Resync B Ejector " + dtoa(TempEjectorNumber + 1, 0) + " Position was: " + dtoa(vGlobalCurrentProduct->vEjectorDistanceFromTriggerInInches[TempEjectorNumber], 2) + 
					" set to: " + dtoa(TempDistanceInInches, 2), vLocalSystemData->vCurrentBeltPosition);

			vGlobalCurrentProduct->SetEjectorBeltPositionOffset(TempEjectorNumber, (float)TempDistanceInInches);

			//ReportErrorMessage("Set Resynchronizing Ejector Sensor " + dtoa(TempEjectorNumber + 1, 0) + " to : " + dtoa(TempDistanceInInches, 2) + " was: " + dtoa(TempOldValue, 2), cEMailInspx, 32000);
		}
		else
		{
			if (vLocalSystemData->vLogFile.vLogSerialData)
				WriteToLogFileWithBeltPosition("Resync Ejector " + dtoa(TempEjectorNumber + 1, 0) + " Position is OK at: " + dtoa(vGlobalCurrentProduct->vEjectorDistanceFromTriggerInInches[TempEjectorNumber], 1) + " calculated: " + dtoa(TempDistanceInInches, 0), vLocalSystemData->vCurrentBeltPosition);

			double TempOldValue = vGlobalCurrentProduct->vEjectorDistanceFromTriggerInInches[TempEjectorNumber];
			//ReportErrorMessage("Resynchronizing Ejector Sensor " + dtoa(TempEjectorNumber + 1, 0) + " close enough, left at current value: " + dtoa(TempOldValue, 2), cEMailInspx, 32000);
		}
		double TempOriginalEndOfLineTimeOutInInches = vGlobalCurrentProduct->vEndOfLineTimeOut;
		vGlobalCurrentProduct->CalculateEndOfLineTimeOut();

		//if (TempOriginalEndOfLineTimeOut != vGlobalCurrentProduct->vEndOfLineTimeOut)
		//	ReportErrorMessage("End of Line Timeout changed from  " + dtoa(TempOriginalEndOfLineTimeOut, 2) + " to : " + dtoa(vGlobalCurrentProduct->vEndOfLineTimeOut, 2), cEMailInspx, 32000);

		vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
	}
}
void CSerialPort::DoBackGroundWork()
{
	if (vGlobalFPGAVersion10Point0OrHigher)
		ReadADCAnalogStatusValue();
}

void CSerialPort::StartMiddleBeamControl()
{
	tSerialCommand TempCommand;
	TempCommand[0] = 0x23;
	TempCommand[1] = 0xA6;
	TempCommand[2] = 0; 
	TempCommand[3] = 1;//send 0 to control right image, send 1 to control middle image
	SendSerialCommand(TempCommand);
	Sleep(cSendCommandSleepTime);
}

void CSerialPort::EndMiddleBeamControl()
{
	tSerialCommand TempCommand;
	TempCommand[0] = 0x23;
	TempCommand[1] = 0xA6;
	TempCommand[2] = 0; 
	TempCommand[3] = 0;//send 0 to control right image, send 1 to control middle image
	SendSerialCommand(TempCommand);
	Sleep(cSendCommandSleepTime);
}

void CSerialPort::ReadDACErrorCount()
{
	tSerialCommand TempCommand;
	TempCommand[0] = 0x30;
	TempCommand[1] = 0x76;
	TempCommand[2] = 0; 
	TempCommand[3] = 0;//send 0 to control right image, send 1 to control middle image
	SendSerialCommand(TempCommand);
	Sleep(cSendCommandSleepTime);
}

void CSerialPort::ReadADCErrorCount()
{
}

#ifdef CompileInHTTPClientOperationsGuardian
void CSerialPort::TestOGMessage()
{
		if (vLocalConfigurationData->vEnableOperationsGuardian)
		{
			vGlobalOGLimitString = "55";
			vGlobalOGLocationString = "XRay";
			vGlobalOGErrorCode = 99;
			::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageTemperatureFault, cOGSevereMessageTemperatureFault);
		}
}
#endif

BYTE CSerialPort::GetIndexForThisEncoderRate(WORD TempEncoderRate)
{
	BYTE TempReturn = cNumberOfFillerOffsetSpeeds;

	if (vLocalSystemData->vUseSamplingFIFO)
		TempReturn = 0;
	else
	if	((((double)(100 + vLocalConfigurationData->vEncoderRateTolerance) / 100.0) * (double)vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[0] > TempEncoderRate)
			&& (((double)(100 - vLocalConfigurationData->vEncoderRateTolerance) / 100.0) * (double)vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[0] < TempEncoderRate))
		TempReturn = 0;
	else
	if	((((double)(100 + vLocalConfigurationData->vEncoderRateTolerance) / 100.0) * (double)vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[1] > TempEncoderRate)
			&& (((double)(100 - vLocalConfigurationData->vEncoderRateTolerance) / 100.0) * (double)vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[1] < TempEncoderRate))
		TempReturn = 1;
	else
	if	((((double)(100 + vLocalConfigurationData->vEncoderRateTolerance) / 100.0) * (double)vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[2] > TempEncoderRate)
			&& (((double)(100 - vLocalConfigurationData->vEncoderRateTolerance) / 100.0) * (double)vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[2] < TempEncoderRate))
		TempReturn = 2;
	else
	if	((((double)(100 + vLocalConfigurationData->vEncoderRateTolerance) / 100.0) * (double)vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[3] > TempEncoderRate)
			&& (((double)(100 - vLocalConfigurationData->vEncoderRateTolerance) / 100.0) * (double)vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[3] < TempEncoderRate))
		TempReturn = 3;

	return TempReturn;
}

void CSerialPort::HandleCompleteFillerMonitorDataSet()
{
	if (vLocalSystemData->vCurrentSampleDataCount[cFillerSamplingType]) //got at least one trigger since previous index pulse
	if (vLocalSystemData->vRecievedFirstIndexPulse[cFillerSamplingType])
	{
		//do error checking to be sure index pulse is in correct place to be sure you got entire set of containers (one from each valve)
		int *TempPointerToValveError = vLocalSystemData->vSamplingIndexErrorData[cFillerSamplingType] + vLocalSystemData->vSamplingIndexErrorDataIndex[cFillerSamplingType];

		if (!vLocalSystemData->vITIPCDig->vShowOnlyRealImages) //simulating
			*TempPointerToValveError = 0;
		else
		{
			if ((!vLocalSystemData->vUseSamplingFIFO) && (GetIndexForThisEncoderRate(vLocalSystemData->vActualEncoderRate) == cNumberOfFillerOffsetSpeeds)) //if not running one of configured encoder rates, indicate an error
			{
				*TempPointerToValveError = vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType];
			}
			else
			if (vLocalSystemData->vCurrentSampleDataCount[cFillerSamplingType] == vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]) //got a full set of data
			{
				*TempPointerToValveError = vLocalSystemData->vHaveFillerIndexError;
				//if (vLocalSystemData->vLogFile.vLogSerialData)
				//	vLocalSystemData->vLogFile.WriteToLogFile("HandleCompleteFillerMonitorDataSet, vLocalSystemData->vHaveFillerIndexError : " + dtoa(vLocalSystemData->vHaveFillerIndexError, 0) +
				//	", vLocalSystemData->vCurrentSampleDataCount[cFillerSamplingType]: " + dtoa(vLocalSystemData->vCurrentSampleDataCount[cFillerSamplingType], 0),cDebugMessage);
			}
			else
			{	//positive value is number of extra packages, negative value is number of packages missed
				*TempPointerToValveError = vLocalSystemData->vCurrentSampleDataCount[cFillerSamplingType] - vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; 

				//if (vLocalSystemData->vLogFile.vLogSerialData)
				//	vLocalSystemData->vLogFile.WriteToLogFile("HandleCompleteFillerMonitorDataSet, vLocalSystemData->vCurrentSampleDataCount[cFillerSamplingType]: " + 
				//	dtoa(vLocalSystemData->vCurrentSampleDataCount[cFillerSamplingType], 0),cDebugMessage);
			}

			//brh should we count the zeros as errors or not
			//if (*TempPointerToValveError == 0)
			//{
			//	WORD TempNumberOfZeroData = 0;
			//	double *TempPointerToMostRecentDataJustTook = vLocalSystemData->vHoldValveDataForNextIndexPulse;
			//	for (WORD TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; TempLoop++)
			//	{
			//		if (*TempPointerToMostRecentDataJustTook == 0)
			//			TempNumberOfZeroData++;
			//		TempPointerToMostRecentDataJustTook++;
			//	}
			//	*TempPointerToValveError = TempNumberOfZeroData; //if there are any zero values, then they are missed containers
			//}
		}

		vLocalSystemData->vSamplingIndexErrorDataIndex[cFillerSamplingType]++;
		if (vLocalSystemData->vSamplingIndexErrorDataIndex[cFillerSamplingType] >= cMaximumValveDataRows)
			vLocalSystemData->vSamplingIndexErrorDataIndex[cFillerSamplingType] = 0;
		if (vLocalSystemData->vSamplingIndexErrorDataHave[cFillerSamplingType] < cMaximumValveDataRows)
			vLocalSystemData->vSamplingIndexErrorDataHave[cFillerSamplingType]++;

		//copy the set of data from current to long term data if no index pulse error, current data has offset of Index pulse, so copy over so valve 1 is in first position of permanent data
		if (*TempPointerToValveError == 0)
		{
			//if (*vLocalSystemData->vHoldValveDataForNextIndexPulse == 0)
			//	ReportErrorMessage("Good Filler Index Pulse, but no temporary data to save: " + dtoa(vMainWindowPointer->GetCurrentFillerIndexOffset(), 0), cWriteToLog, 0);
			//else
			{
				double *TempPointerToDataRowToWriteInto = vLocalSystemData->vValveData + (vLocalSystemData->vValveDataRowIndex * vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]); 
				double *TempPointerToCurrentDataJustTook = vLocalSystemData->vHoldValveDataForNextIndexPulse;

				WORD TempCurrentDataIndexOffset = vMainWindowPointer->GetCurrentFillerIndexOffset();
				if (TempCurrentDataIndexOffset > 0)
					TempPointerToCurrentDataJustTook = TempPointerToCurrentDataJustTook + (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] - TempCurrentDataIndexOffset);

				for (BYTE TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; TempLoop++)
				if (TempPointerToDataRowToWriteInto < vLocalSystemData->vValveDataMaximum)
				{
					*TempPointerToDataRowToWriteInto++ = *TempPointerToCurrentDataJustTook++;
					if (TempPointerToCurrentDataJustTook >= vLocalSystemData->vHoldValveDataForNextIndexPulseMaximum) //have read to end of valve set as started at offset, so start back at 0th location
						TempPointerToCurrentDataJustTook = vLocalSystemData->vHoldValveDataForNextIndexPulse;
				}

				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("*Sampling-Valve Monitor Copy Valve Data From Holder For Display: " + dtoa(vLocalSystemData->vValveDataRowIndex, 0) +
					", Offset: " + dtoa(TempCurrentDataIndexOffset, 0) + ", Encoder Rate: " + dtoa(vLocalSystemData->vActualEncoderRate, 0),cDebugMessage);

				if (vLocalSystemData->vNumberValveRowDataHave < cMaximumValveDataRows)
					vLocalSystemData->vNumberValveRowDataHave++;
				vLocalSystemData->vValveDataRowIndex++;
				if (vLocalSystemData->vValveDataRowIndex >= cMaximumValveDataRows)
					vLocalSystemData->vValveDataRowIndex = 0;
			}
			vLocalSystemData->vLastIndexPulseWasCorrect[cFillerSamplingType] = true;
			//find head 1 offset from Index pulse

			BYTE TempFindLowestValue = 0;
			BYTE TempSpeedFindingAt = 0;
			for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
			if (vLocalSystemData->vFindFillerOffset[TempLoop])
			{
				TempFindLowestValue = vLocalSystemData->vFindFillerOffset[TempLoop];
				TempSpeedFindingAt = TempLoop;
			}
			if (TempFindLowestValue)
			{
				double *TempPointerToMostRecentDataJustTook = vLocalSystemData->vHoldValveDataForNextIndexPulse;

				if (vLocalSystemData->vValveMonitorInspection < vGlobalCurrentProduct->vNumberOfInspections)
				if ((vLocalSystemData->vValveMonitorInspection < vGlobalCurrentProduct->vNumberOfInspections) && (vGlobalCurrentProduct) && (vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection]))
				if ((vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection]->vInspectionType == cUnderfillInspection) ||
					(vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection]->vInspectionType == cOverfillInspection))
				{//must swap highest to lowest as fill is opposite in values as weight
					if (TempFindLowestValue == 1) //finding highest value
						TempFindLowestValue = 2; //find lowest value as fill values are opposite of weight values
					else
					if (TempFindLowestValue == 2) //finding highest value
						TempFindLowestValue = 1; //find lowest value as fill values are opposite of weight values
				}
				//find heaviest container (lowest brightness number)
				if (TempFindLowestValue == 1)
				{
					double TempLowestValue = 0xffffff;
					BYTE TempLowestPosition = 0;

					for (BYTE TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; TempLoop++)
					{
						if (*TempPointerToMostRecentDataJustTook < TempLowestValue)
						{
							TempLowestValue = *TempPointerToMostRecentDataJustTook;
							TempLowestPosition = TempLoop;
						}
						TempPointerToMostRecentDataJustTook++;
					}

					vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[TempSpeedFindingAt] = (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] - 
							TempLowestPosition) % vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType];  //the modulus affects the case where highest position is 0, because x % x = 0, offset is 0 not numberOfHeads
											
					vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[TempSpeedFindingAt] = vLocalSystemData->vActualEncoderRate;

					ReportErrorMessage("Info Only-Filler Speed: " + dtoa(TempSpeedFindingAt + 1, 0) + " Offset found to be: " + dtoa(vMainWindowPointer->GetCurrentFillerIndexOffset(), 0) +
						" Encoder Rate: " + dtoa(vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[TempSpeedFindingAt], 0), cUserChanges, 0);
				}
				else
				if (TempFindLowestValue == 2)
				{ //find highest value or lightest container
					double TempHighesttValue = 0;
					BYTE TempHighestPosition = 0;

					for (BYTE TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; TempLoop++)
					{
						if (*TempPointerToMostRecentDataJustTook > TempHighesttValue)
						{
							TempHighesttValue = *TempPointerToMostRecentDataJustTook;
							TempHighestPosition = TempLoop;
						}
						TempPointerToMostRecentDataJustTook++;
					}
					vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[TempSpeedFindingAt] = (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] - 
							TempHighestPosition) % vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType];  //the modulus affects the case where highest position is 0, because x % x = 0, offset is 0 not numberOfHeads
											
					vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[TempSpeedFindingAt] = vLocalSystemData->vActualEncoderRate;

					ReportErrorMessage("Info Only--->Filler Speed: " + dtoa(TempSpeedFindingAt + 1, 0) + " Offset found to be: " + dtoa(vMainWindowPointer->GetCurrentFillerIndexOffset(), 0) +
						" Encoder Rate: " + dtoa(vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[TempSpeedFindingAt], 0), cUserChanges, 0);
				}
				else
				if (TempFindLowestValue == 3)
				{ //find most variable value
					double TempMostVariableValue = 0;
					BYTE TempMostVariablePosition = 0;
					double *TempStandardDieviationValues = vLocalSystemData->vValveStandardDeviations;

					for (BYTE TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; TempLoop++)
					{
						if (*TempStandardDieviationValues > TempMostVariableValue)
						{
							TempMostVariableValue = *TempStandardDieviationValues;
							TempMostVariablePosition = TempLoop;
						}
						TempStandardDieviationValues++;
					}
					vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[TempSpeedFindingAt] = (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] - 
							TempMostVariablePosition) % vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType];  //the modulus affects the case where highest position is 0, because x % x = 0, offset is 0 not numberOfHeads
											
					vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[TempSpeedFindingAt] = vLocalSystemData->vActualEncoderRate;

					ReportErrorMessage("Info Only--->Filler Speed: " + dtoa(TempSpeedFindingAt + 1, 0) + " Offset found to be: " + dtoa(vMainWindowPointer->GetCurrentFillerIndexOffset(), 0) +
						" Encoder Rate: " + dtoa(vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[TempSpeedFindingAt], 0), cUserChanges, 0);
				}
				for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
					vLocalSystemData->vFindFillerOffset[TempLoop] = 0; //clear the flag to find the filler offset as you are done
			}
		}
		else
		{
			vLocalSystemData->vLastIndexPulseWasCorrect[cFillerSamplingType] = false;

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("Filler Index ERROR: " + dtoa(*TempPointerToValveError, 0),cDebugMessage);

			::PostThreadMessage(vGlobalCallingThreadID, cIndexPulseErrorMessage, cFillerSamplingType, cFillerSamplingType);

			if (vLocalSystemData->vSampledOnLastRevolution[cFillerSamplingType])
			{
				ReportErrorMessage("Filler Index ERROR-\n Last sample may be from wrong valve\n Take sample over", cCritical,0);
				vLocalSystemData->vSampledOnLastRevolution[cFillerSamplingType] = false;
			}
		}
	}
	vLocalSystemData->vHaveFillerIndexError = 0;
	vLocalSystemData->vRecievedFirstIndexPulse[cFillerSamplingType] = true;
	ZeroMemory(vLocalSystemData->vHoldValveDataForNextIndexPulse, vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] * sizeof(double));
	vLocalSystemData->vCurrentSampleDataCount[cFillerSamplingType] = 0;
}

void CSerialPort::HandleCompleteSeamerDataSet()
{
	if (vLocalSystemData->vRecievedFirstIndexPulse[cSeamerSamplingType])
	{
		//do error checking to be sure index pulse is in correct place to be sure you got entire set of containers (one from each valve)
		int *TempPointerToSeamerError = vLocalSystemData->vSamplingIndexErrorData[cSeamerSamplingType] + vLocalSystemData->vSamplingIndexErrorDataIndex[cSeamerSamplingType];
		if (GetIndexForThisEncoderRate(vLocalSystemData->vActualEncoderRate) == cNumberOfFillerOffsetSpeeds) //if not running one of configured encoder rates, indicate an error
			*TempPointerToSeamerError = vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType];
		else
		if (vLocalSystemData->vCurrentSampleDataCount[cSeamerSamplingType] == vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType]) //Index should be zero when get the sync pulse as should have written current data, and now point at zeroth position
		{
			*TempPointerToSeamerError = 0;
			vLocalSystemData->vLastIndexPulseWasCorrect[cSeamerSamplingType] = true;
		}
		else
		{	//positive value is number of extra packages, negative value is number of packages missed
			vLocalSystemData->vLastIndexPulseWasCorrect[cSeamerSamplingType] = false;
			*TempPointerToSeamerError = (int)vLocalSystemData->vCurrentSampleDataCount[cSeamerSamplingType] - (int)vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType]; 

			if (*TempPointerToSeamerError < - vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType])
				*TempPointerToSeamerError = *TempPointerToSeamerError + vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType];

			ReportErrorMessage("Seamer Index ERROR B: " + dtoa(*TempPointerToSeamerError, 0), cWriteToLog,0);
			::PostThreadMessage(vGlobalCallingThreadID, cIndexPulseErrorMessage, cSeamerSamplingType, cSeamerSamplingType);

			if (vLocalSystemData->vSampledOnLastRevolution[cSeamerSamplingType])
			{
				ReportErrorMessage("Seamer Index ERROR\n Last sample may be from wrong head\n Take sample over", cCritical,0);
				ReportErrorMessage("Seamer Index ERROR-Last sample may be from wrong head-Take sample over", cError,0);
			}
		}
	}
	vLocalSystemData->vCurrentSampleDataCount[cSeamerSamplingType] = 0;
	vLocalSystemData->vSampledOnLastRevolution[cSeamerSamplingType] = false;

	vLocalSystemData->vSamplingIndexErrorDataIndex[cSeamerSamplingType]++;
	if (vLocalSystemData->vSamplingIndexErrorDataIndex[cSeamerSamplingType] >= cMaximumValveDataRows)
		vLocalSystemData->vSamplingIndexErrorDataIndex[cSeamerSamplingType] = 0;

	if (vLocalSystemData->vSamplingIndexErrorDataHave[cSeamerSamplingType] < cMaximumValveDataRows)
		vLocalSystemData->vSamplingIndexErrorDataHave[cSeamerSamplingType]++;

	//copy the set of data from current to long term data if no index pulse error, current data has offset of Index pulse, so copy over so valve 1 is in first position of permanent data
}

void CSerialPort::HandleSeamerIndexPulse()
{
	if (vLocalSystemData->vSamplingLastProcessTime[cSeamerSamplingType])
	{
		double TempCurrentTime = CIFCOS::GetSystimeMicrosecs();
		vLocalSystemData->vMostRecentTimeBeforeIndexPulse[cSeamerSamplingType] = TempCurrentTime - vLocalSystemData->vSamplingLastProcessTime[cSeamerSamplingType];
	}
	vLocalSystemData->vSamplingLastProcessTime[cSeamerSamplingType] = CIFCOS::GetSystimeMicrosecs();

	if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
	{
		if (vLocalSystemData->vLogFile.vLogSerialData)
		{
			vLocalSystemData->vSamplingPulseLog[cSeamerSamplingType] = vLocalSystemData->vSamplingPulseLog[cSeamerSamplingType] + dtoa(vLocalSystemData->vSamplingPulseIndexCounter[cSeamerSamplingType], 0) + "\nI";

			int TempLength = vLocalSystemData->vSamplingPulseLog[cSeamerSamplingType].GetLength();
			if (TempLength > 30000)
			{
				int TempPosition = vLocalSystemData->vSamplingPulseLog[cSeamerSamplingType].Find(_T("I"), 15000);
				if (TempPosition > 10000)
				{
					vLocalSystemData->vSamplingPulseLog[cSeamerSamplingType].Delete(0, TempPosition - 1);
					vLocalSystemData->vSamplingPulseLog[cSeamerSamplingType] = "Most Recent at Bottom\n" + vLocalSystemData->vSamplingPulseLog[cSeamerSamplingType];
				}
			}

			vLocalSystemData->vSamplingPulseLog[0] = vLocalSystemData->vSamplingPulseLog[0] + dtoa(vLocalSystemData->vSamplingPulseIndexCounter[cSeamerSamplingType], 0) + "\nI";

			TempLength = vLocalSystemData->vSamplingPulseLog[0].GetLength();
			if (TempLength > 30000)
			{
				int TempPosition = vLocalSystemData->vSamplingPulseLog[0].Find(_T("I"), 15000);
				if (TempPosition > 10000)
				{
					vLocalSystemData->vSamplingPulseLog[0].Delete(0, TempPosition - 1);
					vLocalSystemData->vSamplingPulseLog[0] = "Most Recent at Bottom\n" + vLocalSystemData->vSamplingPulseLog[0];
				}
			}
		}
		vLocalSystemData->vSamplingPulseIndexCounter[cSeamerSamplingType]++;

		if (vLocalSystemData->vUseSamplingFIFO)
		{
			if (vLocalSystemData->vCountPutInFIFOThisRevolution[cSeamerSamplingType] == vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType])
				vLocalSystemData->vRecievedFirstIndexPulse[cSeamerSamplingType] = true;

			//got the Index pulse, so reset the counter
			if (vLocalSystemData->vRecievedFirstIndexPulse[cSeamerSamplingType])
			{
				//diagnostics
				if (vLocalSystemData->vCountPutInFIFOThisRevolution[cSeamerSamplingType] == 0)
				{
					if (PasswordOK(cTemporaryInspxPassword,false))
						ReportErrorMessage("Sampling Seamer Index Pulse had no containers this revolution. pulse: " + dtoa(vLocalSystemData->vSamplingPulseIndexCounter[cSeamerSamplingType], 0),cWriteToLog, 0);  
					else
					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("Sampling Seamer Index Pulse had no containers this revolution. pulse: " + dtoa(vLocalSystemData->vSamplingPulseIndexCounter[cSeamerSamplingType], 0), cDebugMessage);
				}
				else
				if (vLocalSystemData->vCountPutInFIFOThisRevolution[cSeamerSamplingType] < vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType])
				{
					if (PasswordOK(cTemporaryInspxPassword,false))
						ReportErrorMessage("Seamer Index/Object sensor detected empty pockets: " + 
							dtoa(vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType] - vLocalSystemData->vCountPutInFIFOThisRevolution[cSeamerSamplingType], 0),cEMailMaintenance, 0);
					else
					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("Seamer Index/Object sensor detected empty pockets: " + 
							dtoa(vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType] - vLocalSystemData->vCountPutInFIFOThisRevolution[cSeamerSamplingType], 0), cDebugMessage);
				}
				else
				if (vLocalSystemData->vCountPutInFIFOThisRevolution[cSeamerSamplingType] > vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType])
				{
					ReportErrorMessage("Seamer Index/Object sensor detected too many pockets: " + 
						dtoa(vLocalSystemData->vCountPutInFIFOThisRevolution[cSeamerSamplingType], 0), cEMailInspx, 0);
				}
				else
					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("*Sampling-Seamer Drum Got Index Pulse correct number of cans. pulse: " + dtoa(vLocalSystemData->vSamplingPulseIndexCounter[cSeamerSamplingType], 0),cDebugMessage);
			}
			else
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("*Sampling-Seamer Drum Got Index Pulse: " + dtoa(vLocalSystemData->vSamplingIndexErrorDataIndex[cSeamerSamplingType], 0),cDebugMessage);

			if (vLocalSystemData->vRecievedFirstIndexPulse[cSeamerSamplingType])
			{
				//do error checking to be sure index pulse is in correct place to be sure you got entire set of containers (one from each valve)
				int *TempPointerToSeamerError = vLocalSystemData->vSamplingIndexErrorData[0] + vLocalSystemData->vSamplingIndexErrorDataIndex[0]; //0 index of data is random sampling, which is not used, so use for putting into FIFO
					*TempPointerToSeamerError = vLocalSystemData->vCountPutInFIFOThisRevolution[cSeamerSamplingType] - vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType];

				if (*TempPointerToSeamerError != 0)//positive value is number of extra packages, negative value is number of packages missed
					vLocalSystemData->vLastIndexPulseWasCorrect[cSeamerSamplingType] = false;
				else
					vLocalSystemData->vLastIndexPulseWasCorrect[cSeamerSamplingType] = true;

				vLocalSystemData->vSamplingIndexErrorDataIndex[0]++;
				if (vLocalSystemData->vSamplingIndexErrorDataIndex[0] >= cMaximumValveDataRows)
					vLocalSystemData->vSamplingIndexErrorDataIndex[0] = 0;

				if (vLocalSystemData->vSamplingIndexErrorDataHave[0] < cMaximumValveDataRows)
					vLocalSystemData->vSamplingIndexErrorDataHave[0]++;
			}
			vLocalSystemData->vCountPutInFIFOThisRevolution[cSeamerSamplingType] = 0;
			vLocalSystemData->vNextHeadToPutInFIFO[cSeamerSamplingType] = 0;
		}
		//without FIFO
		//else
		//if (vLocalSystemData->vRecievedFirstIndexPulse[cSeamerSamplingType])
		//if (vLocalSystemData->vHeadCountAfterIndexPulse[cSeamerSamplingType]) //got at least one trigger since previous index pulse
		//{
		//	HandleCompleteSeamerDataSet(); //Sampling not using FIFO
		//	//do error checking to be sure index pulse is in correct place to be sure you got entire set of containers (one from each valve)
		//	int *TempPointerToSeamerError = vLocalSystemData->vSamplingIndexErrorData[cSeamerSamplingType] + vLocalSystemData->vSamplingIndexErrorDataIndex[cSeamerSamplingType];
		//	if (GetIndexForThisEncoderRate(vLocalSystemData->vActualEncoderRate) == cNumberOfFillerOffsetSpeeds) //if not running one of configured encoder rates, indicate an error
		//		*TempPointerToSeamerError = vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType];
		//	else
		//	if (vLocalSystemData->vNextHeadToProcess[cSeamerSamplingType] == 0) //Index should be zero when get the sync pulse as should have written current data, and now point at zeroth position
		//	{
		//		*TempPointerToSeamerError = 0;
		//		vLocalSystemData->vLastIndexPulseWasCorrect[cSeamerSamplingType] = true;
		//	}
		//	else
		//	{	//positive value is number of extra packages, negative value is number of packages missed
		//		vLocalSystemData->vLastIndexPulseWasCorrect[cSeamerSamplingType] = false;
		//		*TempPointerToSeamerError = -vLocalSystemData->vNextHeadToProcess[cSeamerSamplingType] + vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType]; 

		//	if (*TempPointerToSeamerError < - vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType])
		//			*TempPointerToSeamerError = *TempPointerToSeamerError + vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType];
		//		ReportErrorMessage("Seamer Index ERROR A: " + dtoa(*TempPointerToSeamerError, 0), cWriteToLog,0);
		//		::PostThreadMessage(vGlobalCallingThreadID, cIndexPulseErrorMessage, cSeamerSamplingType, cSeamerSamplingType);

		//		if (vLocalSystemData->vSampledOnLastRevolution[cSeamerSamplingType])
		//		{
		//			ReportErrorMessage("Seamer Index ERROR\n Last sample may be from wrong head\n Take sample over", cCritical,0);
		//		}
		//	}
		//	vLocalSystemData->vSamplingIndexErrorDataIndex[cSeamerSamplingType]++;
		//	if (vLocalSystemData->vSamplingIndexErrorDataIndex[cSeamerSamplingType] >= cMaximumValveDataRows)
		//		vLocalSystemData->vSamplingIndexErrorDataIndex[cSeamerSamplingType] = 0;
		//	if (vLocalSystemData->vSamplingIndexErrorDataHave[cSeamerSamplingType] < cMaximumValveDataRows)
		//		vLocalSystemData->vSamplingIndexErrorDataHave[cSeamerSamplingType]++;

		//	//copy the set of data from current to long term data if no index pulse error, current data has offset of Index pulse, so copy over so valve 1 is in first position of permanent data
		//	vLocalSystemData->vCurrentSampleDataCount[cSeamerSamplingType] = 0;
		//	vLocalSystemData->vSampledOnLastRevolution[cSeamerSamplingType] = false;
		//}

		vLocalSystemData->vHeadCountAfterIndexPulse[cSeamerSamplingType] = 0;
		vLocalSystemData->vTimeToGetSeamerIndexPulse = false;
		//start collecting a new round of valve data
		vLocalSystemData->vRecievedFirstIndexPulse[cSeamerSamplingType] = true;
		vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
	}				
}

void CSerialPort::HandleFillerIndexPulse()
{
	if (vLocalSystemData->vSamplingLastProcessTime[cFillerSamplingType])
	{
		double TempCurrentTime = CIFCOS::GetSystimeMicrosecs();
		vLocalSystemData->vMostRecentTimeBeforeIndexPulse[cFillerSamplingType] = TempCurrentTime - vLocalSystemData->vSamplingLastProcessTime[cFillerSamplingType];
	}
	vLocalSystemData->vSamplingLastProcessTime[cFillerSamplingType] = CIFCOS::GetSystimeMicrosecs();

	if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
	{
		//got the Index pulse, so reset the counter
		vLocalSystemData->vSamplingPulseIndexCounter[cFillerSamplingType]++;
		if (vLocalSystemData->vLogFile.vLogSerialData)
		{
		//brh remove some if too many lines
			vLocalSystemData->vLogFile.WriteToLogFile("*Sampling-Filler Drum Got Index Pulse: "  + dtoa(vLocalSystemData->vSamplingPulseIndexCounter[cFillerSamplingType], 0),cDebugMessage);
			vLocalSystemData->vSamplingPulseLog[cFillerSamplingType] = vLocalSystemData->vSamplingPulseLog[cFillerSamplingType] + dtoa(vLocalSystemData->vSamplingPulseIndexCounter[cFillerSamplingType], 0) + 
			" P=" + dtoa(vLocalSystemData->vSamplingSeamerPocketPulseCounter, 0) + " O=" + dtoa(vLocalSystemData->vSamplingSeamerObjectPulseCounter, 0) + "\nI";

			int TempLength = vLocalSystemData->vSamplingPulseLog[cFillerSamplingType].GetLength();
			if (TempLength > 30000)
			{
				int TempPosition = vLocalSystemData->vSamplingPulseLog[cFillerSamplingType].Find(_T("I"), 15000);
				if (TempPosition > 10000)
				{
					vLocalSystemData->vSamplingPulseLog[cFillerSamplingType].Delete(0, TempPosition - 1);
					vLocalSystemData->vSamplingPulseLog[cFillerSamplingType] = "Most Recent at Bottom\n" + vLocalSystemData->vSamplingPulseLog[cFillerSamplingType];
				}
			}
			vLocalSystemData->vSamplingSeamerObjectPulseCounter = 0;
			vLocalSystemData->vSamplingSeamerPocketPulseCounter = 0;
		}

		if (vLocalSystemData->vUseSamplingFIFO) //In FIFO mode, each index pulse sees if last group was complete and marks error if not, then zeros counters that put head numbers into FIFOs
		{
			if (vLocalSystemData->vCountPutInFIFOThisRevolution[cFillerSamplingType] == vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
			{
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("*Sampling-Filler Drum Got Index Pulse complete rotation",cDebugMessage);
			}
			else
			if (vLocalSystemData->vITIPCDig->vShowOnlyRealImages) // NOT simulating
			{
				if (vLocalSystemData->vRecievedFirstIndexPulse[cFillerSamplingType])
				{
					//diagnostics
					if (vLocalSystemData->vCountPutInFIFOThisRevolution[cFillerSamplingType] == 0)
					{
						if (PasswordOK(cTemporaryInspxPassword,false))
							ReportErrorMessage("Sampling Filler Index Pulse had no containers this revolution",cWriteToLog, 0);
						else
						if (vLocalSystemData->vLogFile.vLogSerialData)
							vLocalSystemData->vLogFile.WriteToLogFile("Sampling Filler Index Pulse had no containers this revolution", cDebugMessage);

						vLocalSystemData->vLastIndexPulseWasCorrect[cFillerSamplingType] = false;
					}
					else
					if (vLocalSystemData->vCountPutInFIFOThisRevolution[cFillerSamplingType] < vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
					{
						//if (PasswordOK(cTemporaryInspxPassword,false))
						//	ReportErrorMessage("Filler Index/Object sensor detected empty pockets: " + 
						//		dtoa(vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] - vLocalSystemData->vCountPutInFIFOThisRevolution[cFillerSamplingType], 0),cEMailMaintenance, 0);
						//else
						if (vLocalSystemData->vLogFile.vLogSerialData)
							vLocalSystemData->vLogFile.WriteToLogFile("Filler Index/Object sensor detected empty pockets: " + 
								dtoa(vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] - vLocalSystemData->vCountPutInFIFOThisRevolution[cFillerSamplingType], 0), cDebugMessage);
						vLocalSystemData->vLastIndexPulseWasCorrect[cFillerSamplingType] = false;
					}
					else
					if (vLocalSystemData->vCountPutInFIFOThisRevolution[cFillerSamplingType] > vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
					{
							ReportErrorMessage("Filler Index/Object sensor detected extra containers: " + 
								dtoa(vLocalSystemData->vCountPutInFIFOThisRevolution[cFillerSamplingType], 0),cEMailInspx, 0);
						vLocalSystemData->vLastIndexPulseWasCorrect[cFillerSamplingType] = false;
					}
					else
						vLocalSystemData->vLastIndexPulseWasCorrect[cFillerSamplingType] = true;
				}
			}

			vLocalSystemData->vTimeToGetIndexPulse = false;
		}
		else //not FIFO Mode
		if (vLocalSystemData->vRecievedFirstIndexPulse[cFillerSamplingType])
		{
			HandleCompleteFillerMonitorDataSet();
			vLocalSystemData->vSampledOnLastRevolution[cFillerSamplingType] = false;
		}

		vLocalSystemData->vHeadCountAfterIndexPulse[cFillerSamplingType] = 0;

		//start collecting a new round of valve data
		vLocalSystemData->vCountPutInFIFOThisRevolution[cFillerSamplingType] = 0;
		vLocalSystemData->vNextHeadToPutInFIFO[cFillerSamplingType] = 0;
		//vLocalSystemData->vRecievedFirstIndexPulse[cFillerSamplingType] = true;
		vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
	}				
}

void CSerialPort::HandlePocketSensorPulse()
{
	for (BYTE TempLoop = 1; TempLoop < 3; TempLoop++) //do for filler and seamer, (not random sampling which is index 0)
	{
		if (vLocalSystemData->vCountPutInFIFOThisRevolution[TempLoop] == 0) //if just got first Object pulse after index pulse, record time
		if (vLocalSystemData->vSamplingLastProcessTime[TempLoop])
		{
			double TempCurrentTime = CIFCOS::GetSystimeMicrosecs();
			vLocalSystemData->vMostRecentTimeAfterIndexPulse[TempLoop] = TempCurrentTime - vLocalSystemData->vSamplingLastProcessTime[TempLoop];
		}
		vLocalSystemData->vSamplingLastProcessTime[TempLoop] = CIFCOS::GetSystimeMicrosecs();
	}

	if (vLocalSystemData->vSeamerPocketSensorBitMap) //if  using pocket sensor to increment the head number going into the FIFO, calculate time
	{
		vLocalSystemData->vLastPocketTime = CIFCOS::GetSystimeMicrosecs();
		vLocalSystemData->vLastTimeObjectToPocketPulse = vLocalSystemData->vLastPocketTime - vLocalSystemData->vLastObjectTime;
	}

	if (vLocalSystemData->vLogFile.vLogSerialData)
	{
		vLocalSystemData->vSamplingPulseLog[0] = vLocalSystemData->vSamplingPulseLog[0] + "P";
		vLocalSystemData->vSamplingSeamerPocketPulseCounter++;
	}

	for (BYTE TempLoop = 1; TempLoop < 3; TempLoop++) //do for filler and seamer, (not random sampling which is index 0)
	if (vLocalSystemData->vSamplingFIFO[TempLoop])
	{
		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vSamplingPulseLog[TempLoop] = vLocalSystemData->vSamplingPulseLog[TempLoop] + "P";

		vLocalSystemData->vNextHeadToPutInFIFO[TempLoop]++;
	}

	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Sampling Seamer Pocket. Next head for FIFO: " + dtoa(vLocalSystemData->vNextHeadToPutInFIFO[cFillerSamplingType], 0) + ", Belt Position: " + dtoa(vLocalSystemData->vCurrentBeltPosition, 0), cDebugMessage);
}

void CSerialPort::HandleSeamerObjectPulse()
{
	if (!vLocalSystemData->vSeamerPocketSensorBitMap)  //if not using a pocket sensor, show time to Object sensor
	{
		for (BYTE TempLoop = 1; TempLoop < 3; TempLoop++) //do for filler and seamer, (not random sampling which is index 0)
		{
			if (vLocalSystemData->vCountPutInFIFOThisRevolution[TempLoop] == 0) //if just got first Object pulse after index pulse, record time
			if (vLocalSystemData->vSamplingLastProcessTime[TempLoop])
			{
				double TempCurrentTime = CIFCOS::GetSystimeMicrosecs();
				vLocalSystemData->vMostRecentTimeAfterIndexPulse[TempLoop] = TempCurrentTime - vLocalSystemData->vSamplingLastProcessTime[TempLoop];
			}
			vLocalSystemData->vSamplingLastProcessTime[TempLoop] = CIFCOS::GetSystimeMicrosecs();
		}
	}

	if (vLocalSystemData->vSeamerPocketSensorBitMap) //if  using pocket sensor to increment the head number going into the FIFO, calculate time
	{
		vLocalSystemData->vLastObjectTime = CIFCOS::GetSystimeMicrosecs();
		vLocalSystemData->vLastTimePocketToObjectPulse = vLocalSystemData->vLastObjectTime - vLocalSystemData->vLastPocketTime;
	}

	if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
	{
		if (vLocalSystemData->vLogFile.vLogSerialData)
		{
			vLocalSystemData->vSamplingPulseLog[0] = vLocalSystemData->vSamplingPulseLog[0] + "O";
			vLocalSystemData->vSamplingSeamerObjectPulseCounter++;
		}

		for (BYTE TempLoop = 1; TempLoop < 3; TempLoop++) //do for filler and seamer, (not random sampling which is index 0)
		if (vLocalSystemData->vSamplingFIFO[TempLoop])
		{
			vLocalSystemData->vCountPutInFIFOThisRevolution[TempLoop]++;

			if (TempLoop == 1)
			{
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("Sampling Seamer Object.  Belt Position: " + dtoa(vLocalSystemData->vCurrentBeltPosition, 0), cDebugMessage);

				double TempTime = CIFCOS::GetSystimeMicrosecs();

				double TempTimeBetweenPulses = TempTime - vPreviousSeamerObjectPulseTime;
				if (TempTimeBetweenPulses < 20)
					ReportErrorMessage("Time between Seamer Object Pulses Very Short: " + dtoa(TempTimeBetweenPulses, 9), cEMailInspx, 32000);
				
				vPreviousSeamerObjectPulseTime = TempTime;
			}

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vSamplingPulseLog[TempLoop] = vLocalSystemData->vSamplingPulseLog[TempLoop] + "O";

			if (vLocalSystemData->vCountPutInFIFOThisRevolution[TempLoop] > vLocalConfigurationData->vNumberOfHeadsToMonitor[TempLoop]) //Index should be zero when get the sync pulse as should have written current data, and now point at zeroth position
			{
				vLocalSystemData->vLastIndexPulseWasCorrect[TempLoop] = false;

				CString TempName = "Filler";
				if (TempLoop == cSeamerSamplingType)
					TempName = "Seamer";

				if (vLocalSystemData->vRecievedFirstIndexPulse[TempLoop])
					ReportErrorMessage("Extra Seamer Object Sensor Pulses, check " + TempName + " sensor", cEMailMaintenance, 0);
			}
			else
			{
				if (vLocalSystemData->vSeamerPocketSensorBitMap == 0) //if not using pocket sensor to increment the head number going into the FIFO, increment it here
					vLocalSystemData->vNextHeadToPutInFIFO[TempLoop]++;
				else
					if (vLocalSystemData->vNextHeadToPutInFIFO[TempLoop] == 0) //if just had an index pulse, and object pulse comes before pocket pulse, then set head to head one
						vLocalSystemData->vNextHeadToPutInFIFO[TempLoop] = 1;

				BYTE *TempFIFOLocationPointer = vLocalSystemData->vSamplingFIFO[TempLoop] + vLocalSystemData->vSamplingFIFOHead[TempLoop];
				*TempFIFOLocationPointer = vLocalSystemData->vNextHeadToPutInFIFO[TempLoop]; //put the correct head number in the FIFO as just got the Seamer Object Pulse

				if (vLocalSystemData->vLogFile.vLogSerialData)
				{
					vLocalSystemData->vLogFile.WriteToLogFile("Sampling Seamer Object.  Add to FIFO " + dtoa(TempLoop, 0) + " Head: " + dtoa(vLocalSystemData->vNextHeadToPutInFIFO[TempLoop], 0) +
						", FIFO Head: " + dtoa(vLocalSystemData->vSamplingFIFOHead[TempLoop], 0), cDebugMessage);
				}

				vLocalSystemData->vSamplingFIFOHead[TempLoop]++;
				if (vLocalSystemData->vSamplingFIFOHead[TempLoop] >= cSamplingFIFOLength)
					vLocalSystemData->vSamplingFIFOHead[TempLoop] = 0;

				vLocalSystemData->vBeltPositionLastPutItemInSamplingFIFO = vLocalSystemData->vCurrentBeltPosition;

				if (vLocalSystemData->vSamplingFIFOHead[TempLoop] == vLocalSystemData->vSamplingFIFOTail[TempLoop])
					ReportErrorMessage("Error-Sampling FIFO overflow: " + dtoa(TempLoop, 0), cEMailInspx, 32000);
			}
		}
		vLocalSystemData->vTimeToGetIndexPulse = true;
		vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
	}				
}

void CSerialPort::AddedNewContainer_FillInData(CContainer *TempContainer, WORD TempBeltPosition)
{
	TempContainer->vContainerNumber = vLocalSystemData->vBodyTriggersCount;
	if (vLocalSystemData->vLogFile.vLogSerialData)
	{
		WriteToLogFileWithBeltPosition("Add new container from BodyTrigger c: " + dtoa(TempContainer->vContainerNumber, 0),	TempContainer->vBodyTriggerPosition);
		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vSamplingPulseLog[0] = vLocalSystemData->vSamplingPulseLog[0] + "S";

	}

	TempContainer->vLastBodyTriggerGapLength = BPSubtract(TempBeltPosition, vLocalSystemData->vLastBodyTriggerPosition);
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Last Body Trigger Gap: " + dtoa(TempContainer->vLastBodyTriggerGapLength,0),cDebugMessage);

	for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
	{
		if (vGlobalCurrentProduct->vExternalDetectorEnable[TempLoop])
		if (vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled)
		if ((vGlobalCurrentProduct->vExternalDetectorMode[TempLoop] > cExternalDetectorDisabledMode) && (vGlobalCurrentProduct->vExternalDetectorMode[TempLoop] < cBackupDetectorMode))
		{
			//if ((vGlobalCurrentProduct->vExternalDetectorMode[TempLoop] == cRejectInWindowMode) ||
			//	(vGlobalCurrentProduct->vExternalDetectorMode[TempLoop] == cAcceptInWindowMode))
			{
				SendDigitalLineInterruptEnableDisable(vLocalConfigurationData->vExternalDetector[TempLoop].vDigitalInputLine, 1, TempContainer->vStartExternalDetectorPosition[TempLoop]);
				Wait100MicroSecondsForCommandSend();
			}

			SendDigitalLineInterruptEnableDisable(vLocalConfigurationData->vExternalDetector[TempLoop].vDigitalInputLine, 0, TempContainer->vEndExternalDetectorPosition[TempLoop]);
		}
	}

	if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]) // record which head this container is from as just got new trigger
	{
		if (vLocalSystemData->vUseSamplingFIFO) //adding a new container to container collection, so put in which head numbers of filler and seamer it is from
		{
			if (vLocalSystemData->vSamplingFIFO[cFillerSamplingType])
			{
				if (!vLocalSystemData->vITIPCDig->vShowOnlyRealImages) //simulating
				{
					vLocalSystemData->vPreviousFillerHeadPutInContainer++;
					if (vLocalSystemData->vPreviousFillerHeadPutInContainer > vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
						vLocalSystemData->vPreviousFillerHeadPutInContainer = 1;

					TempContainer->vHeadNumber[cFillerSamplingType] = vLocalSystemData->vPreviousFillerHeadPutInContainer;
					vLocalSystemData->vLastIndexPulseWasCorrect[cFillerSamplingType] = true;
					vLocalSystemData->vRecievedFirstIndexPulse[cFillerSamplingType] = true;
					vLocalSystemData->vHeadCountAfterIndexPulse[cFillerSamplingType]++;
				}
				else
				if (vLocalSystemData->vSamplingFIFOTail[cFillerSamplingType] == vLocalSystemData->vSamplingFIFOHead[cFillerSamplingType])
				{
					if (vLocalSystemData->vRecievedFirstIndexPulse[cFillerSamplingType])
					{
						ReportErrorMessage("Got ScanTrac Trigger w/o Object Sensor, Check Sensors", cEMailInspx, 0);
						vLocalSystemData->vHaveFillerIndexError = 1;
					}
					vLocalSystemData->vLastIndexPulseWasCorrect[cFillerSamplingType] = false;
					vLocalSystemData->vPreviousFillerHeadPutInContainer = 0;
					TempContainer->vHeadNumber[cFillerSamplingType] = 0;
					vLocalSystemData->vHeadCountAfterIndexPulse[cFillerSamplingType]++;
				}
				else
				{
					BYTE *TempPointerToTail = vLocalSystemData->vSamplingFIFO[cFillerSamplingType] + vLocalSystemData->vSamplingFIFOTail[cFillerSamplingType];
					TempContainer->vHeadNumber[cFillerSamplingType] = *TempPointerToTail;

					if ((vLocalSystemData->vPreviousFillerHeadPutInContainer + 1 != TempContainer->vHeadNumber[cFillerSamplingType]) && 
						(!((vLocalSystemData->vPreviousFillerHeadPutInContainer == vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]) && (TempContainer->vHeadNumber[cFillerSamplingType] == 1))))
					if (vLocalSystemData->vRecievedFirstIndexPulse[cFillerSamplingType])
					{
						//if (PasswordOK(cTemporaryInspxPassword,false))
						//	ReportErrorMessage("Filler Sampling Error next container not correct head number. Previous: " + dtoa(vLocalSystemData->vPreviousFillerHeadPutInContainer, 0) + ", this: " +
						//		dtoa(TempContainer->vHeadNumber[cFillerSamplingType], 0), cError, 0); //brh change to WriteToLog
						//else
						if (vLocalSystemData->vLogFile.vLogSerialData)
							vLocalSystemData->vLogFile.WriteToLogFile("Filler Sampling FIFO Error next container not correct valve number. Previous: " + dtoa(vLocalSystemData->vPreviousFillerHeadPutInContainer, 0) + ", this: " +
								dtoa(TempContainer->vHeadNumber[cFillerSamplingType], 0), cDebugMessage);

						//vLocalSystemData->vHaveFillerIndexError = TempContainer->vHeadNumber[cFillerSamplingType] - vLocalSystemData->vPreviousFillerHeadPutInContainer - 1;

						//if (vLocalSystemData->vHaveFillerIndexError > vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
						//	vLocalSystemData->vHaveFillerIndexError = vLocalSystemData->vHaveFillerIndexError - vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType];
						//if (vLocalSystemData->vHaveFillerIndexError < -vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
						//	vLocalSystemData->vHaveFillerIndexError = vLocalSystemData->vHaveFillerIndexError + vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType];
					}
					if (vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
					{
						vLocalSystemData->vPreviousFillerHeadPutInContainer = TempContainer->vHeadNumber[cFillerSamplingType];

						vLocalSystemData->vHeadCountAfterIndexPulse[cFillerSamplingType]++;
						//vLocalSystemData->vCurrentSampleDataCount[cFillerSamplingType]++; //done when calculate fill weight and put weight in data array

						if (vLocalSystemData->vLogFile.vLogSerialData)
							vLocalSystemData->vLogFile.WriteToLogFile("Sampling new container filler valve: " + dtoa(TempContainer->vHeadNumber[cFillerSamplingType], 0) + ", FIFO tail: " + 
								dtoa(vLocalSystemData->vSamplingFIFOTail[cFillerSamplingType], 0), cDebugMessage);

						*TempPointerToTail = 0; //clear head number so don't use again accidentally
						vLocalSystemData->vSamplingFIFOTail[cFillerSamplingType]++;
						if (vLocalSystemData->vSamplingFIFOTail[cFillerSamplingType] >= cSamplingFIFOLength)
							vLocalSystemData->vSamplingFIFOTail[cFillerSamplingType] = 0;

						if (vLocalSystemData->vSamplingFIFOTail[cFillerSamplingType] == vLocalSystemData->vSamplingFIFOHead[cFillerSamplingType])
						{
							vLocalSystemData->vPreviousFillerHeadPutInContainer = 0; // queue is empty so next head could be anyone

							if (vLocalSystemData->vLogFile.vLogSerialData)
								vLocalSystemData->vLogFile.WriteToLogFile("Sampling filler FIFO empty", cDebugMessage);
						}
					}
				}
			}
			//else
			//if (vLocalSystemData->vLogFile.vLogSerialData)
			//	vLocalSystemData->vLogFile.WriteToLogFile("Sampling Error, got new Container but have not gotten an Index pulse", cDebugMessage);
		}
		else //sampling without the FIFO to  track containers, only works well when ScanTrac is right at Seamer Object
		{
			TempContainer->vHeadNumber[cFillerSamplingType] = vLocalSystemData->vNextHeadToProcess[cFillerSamplingType];
			vLocalSystemData->vHeadCountAfterIndexPulse[cFillerSamplingType]++;
			//vLocalSystemData->vCurrentSampleDataCount[cFillerSamplingType]++; //done when fill value put into array

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("*Sampling: Container: " + dtoa(TempContainer->vBodyTriggerPosition , 0) + " is from valve/head: " + dtoa(vLocalSystemData->vNextHeadToProcess[cFillerSamplingType], 0), cDebugMessage);

			if (vLocalSystemData->vNextHeadToProcess[cFillerSamplingType] == 0) //if just got first trigger after index pulse, record time
			//if (vLocalSystemData->vLastIndexPulseWasCorrect[cFillerSamplingType])
			if (vLocalSystemData->vSamplingLastProcessTime[cFillerSamplingType])
			{
				double TempCurrentTime = CIFCOS::GetSystimeMicrosecs();
				vLocalSystemData->vMostRecentTimeAfterIndexPulse[cFillerSamplingType] = TempCurrentTime - vLocalSystemData->vSamplingLastProcessTime[cFillerSamplingType];
			}
			vLocalSystemData->vSamplingLastProcessTime[cFillerSamplingType] = CIFCOS::GetSystimeMicrosecs();

			vLocalSystemData->vNextHeadToProcess[cFillerSamplingType]++;
			if (vLocalSystemData->vNextHeadToProcess[cFillerSamplingType] >= vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
			{
				vLocalSystemData->vNextHeadToProcess[cFillerSamplingType] = 0;
				vLocalSystemData->vTimeToGetIndexPulse = true;
			}
		}
	}
	//seamer tracking just got a container trigger and added it to collection of containers in the tunnel
	if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType])
	{
		if (vLocalSystemData->vUseSamplingFIFO) //adding a new container to container collection, so put in which head numbers of filler and seamer it is from
		{
			if (vLocalSystemData->vSamplingFIFO[cSeamerSamplingType])
			//if (vLocalSystemData->vRecievedFirstIndexPulse[cSeamerSamplingType])
			{
				if (vLocalSystemData->vSamplingFIFOTail[cSeamerSamplingType] == vLocalSystemData->vSamplingFIFOHead[cSeamerSamplingType])
				{
					if (vLocalSystemData->vRecievedFirstIndexPulse[cSeamerSamplingType])
					if (vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
					{
						ReportErrorMessage("Got ScanTrac Trigger w/o Object Sensor, Check Sensors", cEMailInspx, 0);
						vLocalSystemData->vLastIndexPulseWasCorrect[cSeamerSamplingType] = false;
						vLocalSystemData->vCurrentSampleDataCount[cSeamerSamplingType]++;
					}
				}
				else
				{
					BYTE *TempPointerToTail = vLocalSystemData->vSamplingFIFO[cSeamerSamplingType] + vLocalSystemData->vSamplingFIFOTail[cSeamerSamplingType];

					//when using FIFO, if head number wraps around to 1, or low value, then completed last set of data, save if complete set, or throw away if too many or too few
					vLocalSystemData->vNextHeadToProcess[cSeamerSamplingType] = *TempPointerToTail;
					if (vLocalSystemData->vCurrentSampleDataCount[cSeamerSamplingType]) //had at least one container in last filler rotation
					if ((vLocalSystemData->vNextHeadToProcess[cSeamerSamplingType] < (vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType] / 2)) &&
						(vLocalSystemData->vPreviousHeadProcessed[cSeamerSamplingType] > (vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType] / 2)))  //went from a higher head number to a lower head number, so copy data from temporary memory to permanent memory
					{
						HandleCompleteSeamerDataSet(); //Using Sampling FIFO
					}
					vLocalSystemData->vPreviousHeadProcessed[cSeamerSamplingType] = vLocalSystemData->vNextHeadToProcess[cSeamerSamplingType];

					//now put the head number for the seamer in the container
					TempContainer->vHeadNumber[cSeamerSamplingType] = vLocalSystemData->vNextHeadToProcess[cSeamerSamplingType];
					vLocalSystemData->vHeadCountAfterIndexPulse[cSeamerSamplingType]++;

					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("Sampling New Container Seamer Head: " + dtoa(TempContainer->vHeadNumber[cSeamerSamplingType], 0) + ", FIFO Tail: " + 
							dtoa(vLocalSystemData->vSamplingFIFOTail[cSeamerSamplingType], 0), cDebugMessage);

					*TempPointerToTail = 0; //clear head number so don't use again accidentally
					vLocalSystemData->vSamplingFIFOTail[cSeamerSamplingType]++;
					if (vLocalSystemData->vSamplingFIFOTail[cSeamerSamplingType] >= cSamplingFIFOLength)
						vLocalSystemData->vSamplingFIFOTail[cSeamerSamplingType] = 0;

					if (vLocalSystemData->vLogFile.vLogSerialData)
					if (vLocalSystemData->vSamplingFIFOTail[cSeamerSamplingType] == vLocalSystemData->vSamplingFIFOHead[cSeamerSamplingType])
						vLocalSystemData->vLogFile.WriteToLogFile("Sampling Seamer FIFO Empty", cDebugMessage);

					vLocalSystemData->vCurrentSampleDataCount[cSeamerSamplingType]++;
				}
			}
		}
		else
		{
			TempContainer->vHeadNumber[cSeamerSamplingType] = vLocalSystemData->vNextHeadToProcess[cSeamerSamplingType];
			vLocalSystemData->vHeadCountAfterIndexPulse[cSeamerSamplingType]++;
			vLocalSystemData->vCurrentSampleDataCount[cSeamerSamplingType]++;

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("*Sampling: Container: " + dtoa(TempContainer->vBodyTriggerPosition , 0) + " is from Head: " + dtoa(vLocalSystemData->vNextHeadToProcess[cSeamerSamplingType], 0), cDebugMessage);

			if (vLocalSystemData->vNextHeadToProcess[cSeamerSamplingType] == 0) //if just got first trigger after index pulse, record time
			//if (vLocalSystemData->vLastIndexPulseWasCorrect[cSeamerSamplingType])
			if (vLocalSystemData->vSamplingLastProcessTime[cSeamerSamplingType])
			{
				double TempCurrentTime = CIFCOS::GetSystimeMicrosecs();
				vLocalSystemData->vMostRecentTimeAfterIndexPulse[cSeamerSamplingType] = TempCurrentTime - vLocalSystemData->vSamplingLastProcessTime[cSeamerSamplingType];
			}
			vLocalSystemData->vSamplingLastProcessTime[cSeamerSamplingType] = CIFCOS::GetSystimeMicrosecs();

			vLocalSystemData->vNextHeadToProcess[cSeamerSamplingType]++;

			if (vLocalSystemData->vNextHeadToProcess[cSeamerSamplingType] >= vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType])
			{
				vLocalSystemData->vNextHeadToProcess[cSeamerSamplingType] = 0;
				vLocalSystemData->vTimeToGetSeamerIndexPulse = true;
			}
		}
	}
}