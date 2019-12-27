

#include "stdafx.h"
#include "ScanTrac.h"
#include "SerialPortReview.h"
#include "SerialPortModBus.h"
#include "SerialPortGuardMaster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CSerialPortGuardMaster, CWinThread)

#ifdef CompileInHTTPClientOperationsGuardian
	extern CString vGlobalOGLocationString;
	extern CString vGlobalOGLimitString;
	extern WORD vGlobalOGErrorCode;
#endif
extern BYTE vGlobalScanTracType;


CSerialPortGuardMaster::CSerialPortGuardMaster(void)
{
	vGuardMasterGaveFirmwareError = false;
	vGuardMasterFirmWareVersion = 0;
	vGuardMasterFaultLEDData = 0;
	vOldGuardMasterFaultLEDData = 0;
	vGuardMasterRawInterlockData = 0;
	vOldGuardMasterRawInterlockData = 0;
	vDetectedValidGuardMasterDevice = false;
	vGuardMasterInterlockFaults = 0;
	//vGuardMasterOuputData = 0;
	//vOldGuardMasterOuputData = 0;
	for (BYTE TempLoop = 0; TempLoop < 6; TempLoop++)
	{
		vGuardMasterFaultsData[TempLoop] = 0;
		vOldGuardMasterFaultsData[TempLoop] = 0;
	}
	vGuardMasterHardwareFaultsData = 0;
	vOldGuardMasterHardwareFaultsData = 0;
	vBackgroundFaultCount = 0;
	vBackgroundCount = 0;
}

CSerialPortGuardMaster::~CSerialPortGuardMaster(void)
{
}

BEGIN_MESSAGE_MAP(CSerialPortGuardMaster, CWinThread)
	//{{AFX_MSG_MAP(CSerialPortGuardMaster)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSerialPortGuardMaster::DecodeMessage()
{
	//get interlock status and set states
	BYTE TempBuffer[120];
	WORD TempIndex = vReceiveDataTail;

	vHasReceivedCommunication = true;
	WORD TempMessageByteCount = 0;
	while ((ReceiveDataCount()) && (TempMessageByteCount < 100))
	{
		TempBuffer[TempMessageByteCount] = vReceiveDataBuffer[TempIndex];
		TempMessageByteCount++;
		TempIndex++;
		if (TempIndex >= cReceiveBufferSize)
			TempIndex = 0;

		if (vReceiveDataTail < cReceiveBufferSize - 1)
			vReceiveDataTail++;
		else
			vReceiveDataTail = 0;
	}

	vWaitingForReply = 0;

	//if (TempBuffer[0] > 127) //msbit is set, so got an error code
	//{
	//	make error types
	//	ReportErrorMessage("Guard Master Serial Data Error", cEMailInspx, 32000);
	//}
	//else
		
	if ((vLastCommandSent == cReadFirmwareVersion) && (TempMessageByteCount > 1))
	{
		if (TempBuffer[2] == 4) //got correct number of bytes of data for LED status request
		{
			vGuardMasterFirmWareVersion = (((WORD)TempBuffer[4]) << 8) + TempBuffer[3];

			if (!vDetectedValidGuardMasterDevice)
			if ((TempBuffer[4] > 7) && (TempBuffer[4] < 12))
			{
				vDetectedValidGuardMasterDevice = true;
				vLocalConfigurationData->vDetectedValidGuardMasterDevice = true;
				ReportErrorMessage("Detected Guard Master Interlock System", cEMailInspx,0);
			}
			else
			{
				if ((TempBuffer[3] > 7) && (TempBuffer[3] < 12)) //brh not sure if need?
					vDetectedValidGuardMasterDevice = true;

				if (!vGuardMasterGaveFirmwareError)
				{
					ReportErrorMessage("Detected Guard Master Interlock System but firmware version wrong: " + dtoa(TempBuffer[4], 0), cEMailInspx,32000);
					vGuardMasterGaveFirmwareError = true;
				}
			}
		}
		else
		{
			if (!vGuardMasterGaveFirmwareError)
			{
				ReportErrorMessage("Guard Master Serial Data Error in Read Firmware Version", cEMailInspx, 32000);
				vGuardMasterGaveFirmwareError = true;
			}
		}
	}
	else
	if ((vLastCommandSent == cReadFaultLED) && (TempMessageByteCount > 1))
	{
		if (TempBuffer[2] == 1) //got correct number of bytes of data for LED status request
		{
			vGuardMasterFaultLEDData = TempBuffer[3];

			if (vOldGuardMasterFaultLEDData != vGuardMasterFaultLEDData)
			{
				vOldGuardMasterFaultLEDData = vGuardMasterFaultLEDData;
				ReportErrorMessage("Guard Master Major Fault LED Data changed: " + ByteToHex(vOldGuardMasterFaultLEDData), cEMailInspx,0);

				if (vOldGuardMasterFaultLEDData & 1)
				{
					ReportErrorMessage("Guard Master Major Fault, LED on Solid, code 1", cEMailMaintenance,0);
					ReportErrorMessage("Guard Master Major Fault, LED on Solid, code 1", cEMailInspx,0);
				}

				if (vOldGuardMasterFaultLEDData & 0x20)
				{
					ReportErrorMessage("Guard Master Major Fault, LED on Solid, code 0x20", cEMailMaintenance,0);
					ReportErrorMessage("Guard Master Major Fault, LED on Solid, code 0x20", cEMailInspx,0);
				}

				if (vOldGuardMasterFaultLEDData & 2)
				{
					ReportErrorMessage("Guard Master Minor Fault, LED blinking, code 2", cEMailMaintenance,0);
					ReportErrorMessage("Guard Master Minor Fault, LED blinking, code 2", cEMailInspx,0);
				}

				if (vOldGuardMasterFaultLEDData & 0x10)
				{
					ReportErrorMessage("Guard Master Minor Fault, LED blinking, code 0x10", cEMailMaintenance,0);
					ReportErrorMessage("Guard Master Minor Fault, LED blinking, code 0x10", cEMailInspx,0);
				}
			}
		}
		else
			ReportErrorMessage("Guard Master Serial Data Error in Read Fault LED", cEMailInspx, 32000);
	}
	else
	if ((vLastCommandSent == cReadHardwareFaults) && (TempMessageByteCount > 1))
	{
		if (TempBuffer[1] == 1) //got correct number of bytes of data for interlock status request
		{
			//must set 4 bits of interlock status to say safe to run X-Rays
			vGuardMasterHardwareFaultsData = TempBuffer[2];

			if (vOldGuardMasterHardwareFaultsData != vGuardMasterHardwareFaultsData)
			{
				vOldGuardMasterHardwareFaultsData = vGuardMasterHardwareFaultsData;

				if (vOldGuardMasterHardwareFaultsData & 1)
					ReportErrorMessage("Guard Master Interlock System Error: Processor HW fault", cEMailMaintenance,0);

				if (vOldGuardMasterHardwareFaultsData & 2)
					ReportErrorMessage("Guard Master Interlock System Error: Safety Input HW fault", cEMailMaintenance,0);

				if (vOldGuardMasterHardwareFaultsData & 4)
					ReportErrorMessage("Guard Master Interlock System Error: Safety Output HW fault", cEMailMaintenance,0);

				if (vOldGuardMasterHardwareFaultsData & 8)
					ReportErrorMessage("Guard Master Interlock System Error: Power supply fault / Main transistor fault", cEMailMaintenance,0);

				if (vOldGuardMasterHardwareFaultsData & 0x10)
					ReportErrorMessage("Guard Master Interlock System Error: Communication fault", cEMailMaintenance,0);

				if (vOldGuardMasterHardwareFaultsData & 0x20)
					ReportErrorMessage("Guard Master Interlock System Error: Configuration fault (wrong revision, invalid configuration", cEMailMaintenance,0);

				if (vOldGuardMasterHardwareFaultsData & 0x40)
					ReportErrorMessage("Guard Master Interlock System Error: Time out (Clock monitoring)", cEMailMaintenance,0);

				if (vOldGuardMasterHardwareFaultsData & 0x80)
					ReportErrorMessage("Guard Master Interlock System Error: Plug-in fault", cEMailMaintenance,0);

				if (vOldGuardMasterHardwareFaultsData == 0)
					ReportErrorMessage("Guard Master Interlock System Hardware Error Cleared", cEMailMaintenance,0);
			}
		}
		else
			ReportErrorMessage("Guard Master Serial Data Error in Read Hardware Faults", cEMailInspx, 32000);
	}
	else
	if ((vLastCommandSent == cReadFaults) && (TempMessageByteCount > 1))
	{
		if (TempBuffer[2] == 15) //got correct number of bytes of data for interlock status request
		{
			//must set 4 bits of interlock status to say safe to run X-Rays
			//vGuardMasterFaultsData[0] = (TempBuffer[5] << 16) + (TempBuffer[4] << 8) + TempBuffer[3];
			//vGuardMasterFaultsData[1] = (TempBuffer[8] << 16) + (TempBuffer[7] << 8) + TempBuffer[6];
			//vGuardMasterFaultsData[2] = (TempBuffer[11] << 16) + (TempBuffer[10] << 8) + TempBuffer[9];
			//vGuardMasterFaultsData[3] = (TempBuffer[14] << 16) + (TempBuffer[13] << 8) + TempBuffer[12];
			//vGuardMasterFaultsData[4] = (TempBuffer[17] << 16) + (TempBuffer[16] << 8) + TempBuffer[15];
			vGuardMasterFaultsData[0] = TempBuffer[3];
			vGuardMasterFaultsData[1] = TempBuffer[6];
			vGuardMasterFaultsData[2] = TempBuffer[9];
			vGuardMasterFaultsData[3] = TempBuffer[12];
			vGuardMasterFaultsData[4] = TempBuffer[15];

			if (vGuardMasterFaultsData[0] != vOldGuardMasterFaultsData[0]) //output changed
			{
				vOldGuardMasterFaultsData[0] = vGuardMasterFaultsData[0];
				ReportErrorMessage("GuardMaster Interlock Bits 1: " + ByteToHex(TempBuffer[5]) + " " +ByteToHex(TempBuffer[4]) + " " +ByteToHex(TempBuffer[3]), cWriteToLog, 0);
			}
			if (vGuardMasterFaultsData[1] != vOldGuardMasterFaultsData[1]) //output changed
			{
				vOldGuardMasterFaultsData[1] = vGuardMasterFaultsData[1];
				ReportErrorMessage("GuardMaster Interlock Bits 2: " + ByteToHex(TempBuffer[8]) + " " +ByteToHex(TempBuffer[7]) + " " +ByteToHex(TempBuffer[6]), cWriteToLog, 0);
			}
			if (vGuardMasterFaultsData[2] != vOldGuardMasterFaultsData[2]) //output changed
			{
				vOldGuardMasterFaultsData[2] = vGuardMasterFaultsData[2];
				ReportErrorMessage("GuardMaster Interlock Bits 3: " + ByteToHex(TempBuffer[11]) + " " +ByteToHex(TempBuffer[10]) + " " +ByteToHex(TempBuffer[9]), cWriteToLog, 0);
			}
			if (vGuardMasterFaultsData[3] != vOldGuardMasterFaultsData[3]) //output changed
			{
				vOldGuardMasterFaultsData[3] = vGuardMasterFaultsData[3];
				ReportErrorMessage("GuardMaster Interlock Bits 4: " + ByteToHex(TempBuffer[14]) + " " +ByteToHex(TempBuffer[13]) + " " +ByteToHex(TempBuffer[12]), cWriteToLog, 0);
			}
			if (vGuardMasterFaultsData[4] != vOldGuardMasterFaultsData[4]) //output changed
			{
				vOldGuardMasterFaultsData[4] = vGuardMasterFaultsData[4];
				ReportErrorMessage("GuardMaster Output Bits 5: " + ByteToHex(TempBuffer[17]) + " " +ByteToHex(TempBuffer[16]) + " " +ByteToHex(TempBuffer[15]), cWriteToLog, 0);
			}
		}
		else
		if (vDetectedValidGuardMasterDevice)
			ReportErrorMessage("Guard Master Serial Data Error in Read Faults", cEMailInspx, 32000);
	}
	//else
	//if ((vLastCommandSent == cReadOutputState) && (TempMessageByteCount > 1))
	//{
	//	if (TempBuffer[2] == 1) //got correct number of bytes of data for interlock status request
	//	{
	//		//must set 4 bits of interlock status to say safe to run X-Rays
	//		if (!vDetectedValidGuardMasterDevice)
	//		{
	//			vDetectedValidGuardMasterDevice = true;
	//			ReportErrorMessage("Detected Guard Master Interlock System", cAction,0);
	//		}
	//		vGuardMasterOuputData = TempBuffer[3];

	//		if (vGuardMasterOuputData != vOldGuardMasterOuputData) //output changed
	//		{
	//			vOldGuardMasterOuputData = vGuardMasterOuputData;
	//		}
	//	}
	//}
	else
	if ((vLastCommandSent == cSendResetCommand) && (TempMessageByteCount > 1))
	{
		if ((TempBuffer[1] == 5) && (TempBuffer[3] == 1)) //got correct wrote to for reset
		{
		}
		else
			ReportErrorMessage("Guard Master Serial Data Error in Send Reset Command", cEMailInspx, 32000);
	}
	else
	if ((vLastCommandSent == cReadInterLockState) && (TempMessageByteCount > 1))
	{
		if (TempBuffer[2] == 3) //got correct number of bytes of data for interlock status request
		{
			//must set 4 bits of interlock status to say safe to run X-Rays
			vGuardMasterRawInterlockData = (TempBuffer[5] << 16) + (TempBuffer[4] << 8) + TempBuffer[3];

			if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
			{ //these devices don't have side conveyor cover inputs, so set those inputs to OK
				vGuardMasterRawInterlockData = vGuardMasterRawInterlockData | cLeftConveyorCoversMaskBitA | cRightConveyorCoversMaskBitA | cLeftConveyorCoversMaskBitB | cRightConveyorCoversMaskBitB;
			}

			if (vGuardMasterRawInterlockData != vOldGuardMasterRawInterlockData) //interlocks changed
			{
				if (!vGuardMasterRawInterlockData)
					ReportErrorMessage("Guard Master - ALL Interlocks are OPEN", cEMailInspx, 0);

				CString TempString1;
				CString TempString;
				if ((vLocalSystemData->vLastXRayCommandOn) || (vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
					TempString1.LoadString(IDS_XRaysShutDown);

				//first input on Guard Master is Front Door
				if (((vGuardMasterRawInterlockData & cFrontDoorMaskBitA) && (!(vGuardMasterRawInterlockData & cFrontDoorMaskBitB))) || //if interlock fault state where one bit is active and the other is not
					((!(vGuardMasterRawInterlockData & cFrontDoorMaskBitA)) && (vGuardMasterRawInterlockData & cFrontDoorMaskBitB)))
				{
					if (!(((vOldGuardMasterRawInterlockData & cFrontDoorMaskBitA) && (!(vOldGuardMasterRawInterlockData & cFrontDoorMaskBitB))) || //if previous status did not have a fault, give message
						((!(vOldGuardMasterRawInterlockData & cFrontDoorMaskBitA)) && (vOldGuardMasterRawInterlockData & cFrontDoorMaskBitB))))
					{
						if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
							(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
						{
							ReportErrorMessage("Interlock Circuit FAULT Cabinet Door", cEMailMaintenance, 0);
							ReportErrorMessage("Interlock Circuit FAULT Cabinet Door", cEMailInspx, 0);
						}
						else
						{
							ReportErrorMessage("Interlock Circuit FAULT Front Door", cEMailMaintenance, 0);
							ReportErrorMessage("Interlock Circuit FAULT Front Door", cEMailInspx, 0);
						}
						vGuardMasterInterlockFaults = vGuardMasterInterlockFaults | cFrontDoorMaskBitA;
					}
				}
				else
				if (((vOldGuardMasterRawInterlockData & cFrontDoorMaskBitA) && (!(vOldGuardMasterRawInterlockData & cFrontDoorMaskBitB))) || //if interlock fault state where one bit is active and the other is not on previous read
					((!(vOldGuardMasterRawInterlockData & cFrontDoorMaskBitA)) && (vOldGuardMasterRawInterlockData & cFrontDoorMaskBitB)))
				{
					if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
						(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
					{
						ReportErrorMessage("Interlock Circuit FAULT Cabinet Door corrected", cEMailMaintenance, 0);

						if ((vGuardMasterFaultsData[0]) || (vGuardMasterFaultsData[1]) || (vGuardMasterFaultsData[2]) || (vGuardMasterFaultsData[3]) || (vGuardMasterFaultsData[4]))
							ReportErrorMessage("Open and Close Cabinet Door to Reset Interlock System", cEMailMaintenance, 0);
					}
					else
					{
						ReportErrorMessage("Interlock Circuit FAULT Front Door corrected", cEMailMaintenance, 0);

						if ((vGuardMasterFaultsData[0]) || (vGuardMasterFaultsData[1]) || (vGuardMasterFaultsData[2]) || (vGuardMasterFaultsData[3]) || (vGuardMasterFaultsData[4]))
							ReportErrorMessage("Open and Close Front Door to Reset Interlock System", cEMailMaintenance, 0);
					}
					vGuardMasterInterlockFaults = vGuardMasterInterlockFaults & (!cFrontDoorMaskBitA);
				}

				if (((vGuardMasterRawInterlockData & cFrontDoorMaskBits) != cFrontDoorMaskBits) && //if front door was just opened log event
					((vOldGuardMasterRawInterlockData & cFrontDoorMaskBits) == cFrontDoorMaskBits))
				{
					if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
						(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
						ReportErrorMessage("Operator Opened Cabinet Door", cUserAction, 0);
					else
						ReportErrorMessage("Operator Opened Front Door", cUserAction, 0);

					if ((vLocalSystemData->vLastXRayCommandOn) || (vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
					{//"F/R Door Interlock Broken - X-Rays Shut Down"
						::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,1,1);

						if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
							(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
							TempString.LoadString(IDS_UpperDoorInterlockBroken);
						else
							TempString.LoadString(IDS_FRDoorInterlockBroken);
						ReportErrorMessage(TempString + " - " + TempString1, cError,14);
						vLocalSystemData->vLastXRayCommandOn = false;
#ifdef CompileInHTTPClientOperationsGuardian
						if (vLocalConfigurationData->vEnableOperationsGuardian)
						{
							if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
								(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
							{
								vGlobalOGLocationString = "front_door";
								vGlobalOGErrorCode = 202;
							}
							else
							{
								vGlobalOGLocationString = "cabinet_door";
								vGlobalOGErrorCode = 206;
							}
							::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageInterlockBroken, cOGSevereMessageInterlockBroken);
						}
#endif
					}
				}
				else
				if (((vGuardMasterRawInterlockData & cFrontDoorMaskBits) == cFrontDoorMaskBits) && //if fron door was just opened log event
					((vOldGuardMasterRawInterlockData & cFrontDoorMaskBits) != cFrontDoorMaskBits))
				{
					if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
						(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
						ReportErrorMessage("Operator Closed Cabinet Door", cUserAction, 0);
					else
						ReportErrorMessage("Operator Closed Front Door", cUserAction, 0);
				}

				//2nd input on Guard Master is Back Door
				if (((vGuardMasterRawInterlockData & cBackDoorMaskBitA) && (!(vGuardMasterRawInterlockData & cBackDoorMaskBitB))) || //if interlock fault state where one bit is active and the other is not
					((!(vGuardMasterRawInterlockData & cBackDoorMaskBitA)) && (vGuardMasterRawInterlockData & cBackDoorMaskBitB)))
				{
					if (!(((vOldGuardMasterRawInterlockData & cBackDoorMaskBitA) && (!(vOldGuardMasterRawInterlockData & cBackDoorMaskBitB))) || //if previous status did not have a fault, give message
						((!(vOldGuardMasterRawInterlockData & cBackDoorMaskBitA)) && (vOldGuardMasterRawInterlockData & cBackDoorMaskBitB))))
					{
						if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
							(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
						{
							ReportErrorMessage("Interlock Circuit FAULT Center Conveyor Door", cEMailMaintenance, 0);
							ReportErrorMessage("Interlock Circuit FAULT Center Conveyor Door", cEMailInspx, 0);
						}
						else
						{
							ReportErrorMessage("Interlock Circuit FAULT Back Door", cEMailMaintenance, 0);
							ReportErrorMessage("Interlock Circuit FAULT Back Door", cEMailInspx, 0);
						}
						vGuardMasterInterlockFaults = vGuardMasterInterlockFaults | cBackDoorMaskBitA;
					}
				}
				else //no fault currently
				if (((vOldGuardMasterRawInterlockData & cBackDoorMaskBitA) && (!(vOldGuardMasterRawInterlockData & cBackDoorMaskBitB))) || //if interlock fault state where one bit is active and the other is not on previous read
					((!(vOldGuardMasterRawInterlockData & cBackDoorMaskBitA)) && (vOldGuardMasterRawInterlockData & cBackDoorMaskBitB)))
				{
					if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
						(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
						ReportErrorMessage("Interlock Circuit FAULT Center Conveyor Door corrected", cEMailMaintenance, 0);
					else
						ReportErrorMessage("Interlock Circuit FAULT Back Door corrected", cEMailMaintenance, 0);
					vGuardMasterInterlockFaults = vGuardMasterInterlockFaults & (!cBackDoorMaskBitA);

					if ((vGuardMasterFaultsData[0]) || (vGuardMasterFaultsData[1]) || (vGuardMasterFaultsData[2]) || (vGuardMasterFaultsData[3]) || (vGuardMasterFaultsData[4]))
					{
						if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
							(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
							ReportErrorMessage("Open and Close Center Conveyor Door to Reset Interlock System", cEMailMaintenance, 0);
						else
							ReportErrorMessage("Open and Close Back Door to Reset Interlock System", cEMailMaintenance, 0);
					}
				}

				if (((vGuardMasterRawInterlockData & cBackDoorMaskBits) != cBackDoorMaskBits) && //if front door was just opened log event
					((vOldGuardMasterRawInterlockData & cBackDoorMaskBits) == cBackDoorMaskBits))
				{
					if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
						(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
						ReportErrorMessage("Operator Opened Center Conveyor Door", cUserAction, 0);
					else
						ReportErrorMessage("Operator Opened Rear Door", cUserAction, 0);

					if ((vLocalSystemData->vLastXRayCommandOn) || (vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
					{
						::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,1,1);

						if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
							(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
							TempString.LoadString(IDS_LowerDoorInterlockBroken);
						else
							TempString.LoadString(IDS_RearDoorOpened);
						ReportErrorMessage(TempString + " - " + TempString1, cError,13);
#ifdef CompileInHTTPClientOperationsGuardian
						if (vLocalConfigurationData->vEnableOperationsGuardian)
						{
							if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
								(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
							{
								vGlobalOGLocationString = "Conveyor_door";
								vGlobalOGErrorCode = 207;
							}
							else
							{
								vGlobalOGLocationString = "rear_door";
								vGlobalOGErrorCode = 203;
							}
							::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageInterlockBroken, cOGSevereMessageInterlockBroken);
						}
#endif
					}
				}
				else
				if (((vGuardMasterRawInterlockData & cBackDoorMaskBits) == cBackDoorMaskBits) && //if fron door was just opened log event
					((vOldGuardMasterRawInterlockData & cBackDoorMaskBits) != cBackDoorMaskBits))
				{
					if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
						(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
						ReportErrorMessage("Operator Closed Center Conveyor Door", cUserAction, 0);
					else
						ReportErrorMessage("Operator Closed Rear Door", cUserAction, 0);
				}

				////3rd input on Guard Master is E-Stop
				if (((vGuardMasterRawInterlockData & cEStopMaskBitA) && (!(vGuardMasterRawInterlockData & cEStopMaskBitB))) || //if interlock fault state where one bit is active and the other is not
					((!(vGuardMasterRawInterlockData & cEStopMaskBitA)) && (vGuardMasterRawInterlockData & cEStopMaskBitB)))
				{
					if (!(((vOldGuardMasterRawInterlockData & cEStopMaskBitA) && (!(vOldGuardMasterRawInterlockData & cEStopMaskBitB))) || //if previous status did not have a fault, give message
						((!(vOldGuardMasterRawInterlockData & cEStopMaskBitA)) && (vOldGuardMasterRawInterlockData & cEStopMaskBitB))))
					{
						ReportErrorMessage("Interlock Circuit FAULT E-Stop", cEMailMaintenance, 0);
						ReportErrorMessage("Interlock Circuit FAULT E-Stop", cEMailInspx, 0);
						vGuardMasterInterlockFaults = vGuardMasterInterlockFaults | cEStopMaskBitA;
					}
				}
				else //no fault currently
				if (((vOldGuardMasterRawInterlockData & cEStopMaskBitA) && (!(vOldGuardMasterRawInterlockData & cEStopMaskBitB))) || //if interlock fault state where one bit is active and the other is not on previous read
					((!(vOldGuardMasterRawInterlockData & cEStopMaskBitA)) && (vOldGuardMasterRawInterlockData & cEStopMaskBitB)))
				{
					ReportErrorMessage("Interlock Circuit FAULT E-Stop corrected", cEMailMaintenance, 0);
					vGuardMasterInterlockFaults = vGuardMasterInterlockFaults & (!cEStopMaskBitA);

					if ((vGuardMasterFaultsData[0]) || (vGuardMasterFaultsData[1]) || (vGuardMasterFaultsData[2]) || (vGuardMasterFaultsData[3]) || (vGuardMasterFaultsData[4]))
						ReportErrorMessage("Push In and Pull Out E-Stop to Reset Interlock System", cEMailMaintenance, 0);
				}

				if (((vGuardMasterRawInterlockData & cEStopMaskBits) != cEStopMaskBits) && //if fron door was just opened log event
					((vOldGuardMasterRawInterlockData & cEStopMaskBits) == cEStopMaskBits))
				{
					ReportErrorMessage("Operator Pushed In E-Stop to disabled state", cUserAction, 0);
					if ((vLocalSystemData->vLastXRayCommandOn) || (vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
					{//"E-Stop Shut Off - X-Rays Shut Down"
						::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						TempString.LoadString(IDS_EStopShutOff);
						ReportErrorMessage(TempString + " - " + TempString1, cError,16);
#ifdef CompileInHTTPClientOperationsGuardian
						if (vLocalConfigurationData->vEnableOperationsGuardian)
						{
							vGlobalOGLocationString = "estop";
							vGlobalOGErrorCode = 200;
							::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageInterlockBroken, cOGSevereMessageInterlockBroken);
						}
#endif
					}
				}
				else
				if (((vGuardMasterRawInterlockData & cEStopMaskBits) == cEStopMaskBits) && //if fron door was just opened log event
					((vOldGuardMasterRawInterlockData & cEStopMaskBits) != cEStopMaskBits))
				{
					ReportErrorMessage("Operator Pulled out E-Stop to OK state", cUserAction, 0);
				}

				//bit 9 of vLocalSystemData->vSourceInterLocks is e-stop are closed, so set their status
				if ((vGuardMasterRawInterlockData & cEStopMaskBits) == cEStopMaskBits)  //front and read door closed
					vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x0200; //set status to front and read door closed
				else
					vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks & 0xFDFF; //set status to front and read door open

				////4rd input on Guard Master is Key Switch  Only one input, so no fault condition
				//if (((vGuardMasterRawInterlockData & cKeySwitchMaskBitA) && (!(vGuardMasterRawInterlockData & cKeySwitchMaskBitB))) || //if interlock fault state where one bit is active and the other is not
				//	((!(vGuardMasterRawInterlockData & cKeySwitchMaskBitA)) && (vGuardMasterRawInterlockData & cKeySwitchMaskBitB)))
				//{
				//	if (!(((vOldGuardMasterRawInterlockData & cKeySwitchMaskBitA) && (!(vOldGuardMasterRawInterlockData & cKeySwitchMaskBitB))) || //if previous status did not have a fault, give message
				//		((!(vOldGuardMasterRawInterlockData & cKeySwitchMaskBitA)) && (vOldGuardMasterRawInterlockData & cKeySwitchMaskBitB))))
				//	{
				//		ReportErrorMessage("Interlock Circuit FAULT E-Stop", cEMailMaintenance, 0);
				//		vGuardMasterInterlockFaults = vGuardMasterInterlockFaults | cKeySwitchMaskBitA;
				//	}
				//}
				//else //no fault currently
				//if (((vOldGuardMasterRawInterlockData & cKeySwitchMaskBitA) && (!(vOldGuardMasterRawInterlockData & cKeySwitchMaskBitB))) || //if interlock fault state where one bit is active and the other is not on previous read
				//	((!(vOldGuardMasterRawInterlockData & cKeySwitchMaskBitA)) && (vOldGuardMasterRawInterlockData & cKeySwitchMaskBitB)))
				//{
				//	ReportErrorMessage("Interlock Circuit FAULT E-Stop corrected", cEMailMaintenance, 0);
				//	vGuardMasterInterlockFaults = vGuardMasterInterlockFaults & (!cKeySwitchMaskBitA);

				//	if ((vGuardMasterFaultsData[0]) || (vGuardMasterFaultsData[1]) || (vGuardMasterFaultsData[2]) || (vGuardMasterFaultsData[3]) || (vGuardMasterFaultsData[4]))
				//		ReportErrorMessage("Turn Off and back On Key Switch to Reset Interlock System", cEMailMaintenance, 0);
				//}

				if (((vGuardMasterRawInterlockData & cKeySwitchMaskBits) != cKeySwitchMaskBits) && //if key switch was just turned off log event
					((vOldGuardMasterRawInterlockData & cKeySwitchMaskBits) == cKeySwitchMaskBits))
				{
					ReportErrorMessage("Operator Turned Off Key Switch", cUserAction, 0);
					if ((vLocalSystemData->vLastXRayCommandOn) || (vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
					{
						::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						TempString.LoadString(IDS_KeySwitchTurnedOff);
						ReportErrorMessage(TempString + " - " + TempString1, cError,15);
#ifdef CompileInHTTPClientOperationsGuardian
						if (vLocalConfigurationData->vEnableOperationsGuardian)
						{
							vGlobalOGLocationString = "key_switch";
							vGlobalOGErrorCode = 201;
							::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageInterlockBroken, cOGSevereMessageInterlockBroken);
						}
#endif
					}
				}
				else
				if (((vGuardMasterRawInterlockData & cKeySwitchMaskBits) == cKeySwitchMaskBits) && //if fron door was just opened log event
					((vOldGuardMasterRawInterlockData & cKeySwitchMaskBits) != cKeySwitchMaskBits))
				{
					ReportErrorMessage("Operator Turned On Key Switch", cUserAction, 0);
				}

				//bit 8 of vLocalSystemData->vSourceInterLocks is Key Switch are closed, so set their status
				if ((vGuardMasterRawInterlockData & cKeySwitchMaskBits) == cKeySwitchMaskBits)  //front and read door closed
					vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x0100; //set status to front and read door closed
				else
					vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks & 0xFEFF; //set status to front and read door open

				////5th input on Guard Master is X-Ray Detector Shields (up to 3 in series)
				//if (((vGuardMasterRawInterlockData & cDetectorXRayShieldsMaskBitA) && (!(vGuardMasterRawInterlockData & cDetectorXRayShieldsMaskBitB))) || //if interlock fault state where one bit is active and the other is not
				//	((!(vGuardMasterRawInterlockData & cDetectorXRayShieldsMaskBitA)) && (vGuardMasterRawInterlockData & cDetectorXRayShieldsMaskBitB)))
				//{
				//	if (!(((vOldGuardMasterRawInterlockData & cDetectorXRayShieldsMaskBitA) && (!(vOldGuardMasterRawInterlockData & cDetectorXRayShieldsMaskBitB))) || //if previous status did not have a fault, give message
				//		((!(vOldGuardMasterRawInterlockData & cDetectorXRayShieldsMaskBitA)) && (vOldGuardMasterRawInterlockData & cDetectorXRayShieldsMaskBitB))))
				//	{
				//		ReportErrorMessage("Interlock Circuit FAULT X-Ray Detector Shield", cEMailMaintenance, 0);
				//		vGuardMasterInterlockFaults = vGuardMasterInterlockFaults | cDetectorXRayShieldsMaskBitA;
				//	}
				//}
				//else
				//if (((vOldGuardMasterRawInterlockData & cDetectorXRayShieldsMaskBitA) && (!(vOldGuardMasterRawInterlockData & cDetectorXRayShieldsMaskBitB))) || //if interlock fault state where one bit is active and the other is not on previous read
				//	((!(vOldGuardMasterRawInterlockData & cDetectorXRayShieldsMaskBitA)) && (vOldGuardMasterRawInterlockData & cDetectorXRayShieldsMaskBitB)))
				//{
				//	ReportErrorMessage("Interlock Circuit FAULT X-Ray Detector Shield corrected", cEMailMaintenance, 0);
				//	vGuardMasterInterlockFaults = vGuardMasterInterlockFaults & (!cDetectorXRayShieldsMaskBitA);

				//	if ((vGuardMasterFaultsData[0]) || (vGuardMasterFaultsData[1]) || (vGuardMasterFaultsData[2]) || (vGuardMasterFaultsData[3]) || (vGuardMasterFaultsData[4]))
				//		ReportErrorMessage("Remove and Install Detector Shield to Reset Interlock System", cEMailMaintenance, 0);
				//}

				//if (((vGuardMasterRawInterlockData & cDetectorXRayShieldsMaskBits) != cDetectorXRayShieldsMaskBits) && //if fron door was just opened log event
				//	((vOldGuardMasterRawInterlockData & cDetectorXRayShieldsMaskBits) == cDetectorXRayShieldsMaskBits))
				//{
				//	ReportErrorMessage("Operator Removed X-Ray Detector Shield", cUserAction, 0);
				//}
				//else
				//if (((vGuardMasterRawInterlockData & cDetectorXRayShieldsMaskBits) == cDetectorXRayShieldsMaskBits) && //if fron door was just opened log event
				//	((vOldGuardMasterRawInterlockData & cDetectorXRayShieldsMaskBits) != cDetectorXRayShieldsMaskBits))
				//{
				//	ReportErrorMessage("Operator Installed X-Ray Detector Shield", cUserAction, 0);
				//}

				//6th input on Guard Master is Tube Cover (on top downs it is the bottom door and detector shields
				if (((vGuardMasterRawInterlockData & cTubeCoverMaskBitA) && (!(vGuardMasterRawInterlockData & cTubeCoverMaskBitB))) || //if interlock fault state where one bit is active and the other is not
					((!(vGuardMasterRawInterlockData & cTubeCoverMaskBitA)) && (vGuardMasterRawInterlockData & cTubeCoverMaskBitB)))
				{
					if (!(((vOldGuardMasterRawInterlockData & cTubeCoverMaskBitA) && (!(vOldGuardMasterRawInterlockData & cTubeCoverMaskBitB))) || //if previous status did not have a fault, give message
						((!(vOldGuardMasterRawInterlockData & cTubeCoverMaskBitA)) && (vOldGuardMasterRawInterlockData & cTubeCoverMaskBitB))))
					{
						if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
							(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
						{
							ReportErrorMessage("Interlock Circuit FAULT Detector Door/Detector Shields", cEMailMaintenance, 0);
							ReportErrorMessage("Interlock Circuit FAULT Detector Door/Detector Shields", cEMailInspx, 0);
						}
						else
						{
							ReportErrorMessage("Interlock Circuit FAULT X-Ray Tube Cover", cEMailMaintenance, 0);
							ReportErrorMessage("Interlock Circuit FAULT X-Ray Tube Cover", cEMailInspx, 0);
						}
						vGuardMasterInterlockFaults = vGuardMasterInterlockFaults | cTubeCoverMaskBitA;
					}
				}
				else //no fault currently
				if (((vOldGuardMasterRawInterlockData & cTubeCoverMaskBitA) && (!(vOldGuardMasterRawInterlockData & cTubeCoverMaskBitB))) || //if interlock fault state where one bit is active and the other is not on previous read
					((!(vOldGuardMasterRawInterlockData & cTubeCoverMaskBitA)) && (vOldGuardMasterRawInterlockData & cTubeCoverMaskBitB)))
				{
					if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
						(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
					{
						ReportErrorMessage("Interlock Circuit FAULT Detector Door/Detector Shields corrected", cEMailMaintenance, 0);

						if ((vGuardMasterFaultsData[0]) || (vGuardMasterFaultsData[1]) || (vGuardMasterFaultsData[2]) || (vGuardMasterFaultsData[3]) || (vGuardMasterFaultsData[4]))
							ReportErrorMessage("Open and Close Detector Door to Reset Interlock System", cEMailMaintenance, 0);
					}
					else
					{
						ReportErrorMessage("Interlock Circuit FAULT X-Ray Tube Cover corrected", cEMailMaintenance, 0);

						if ((vGuardMasterFaultsData[0]) || (vGuardMasterFaultsData[1]) || (vGuardMasterFaultsData[2]) || (vGuardMasterFaultsData[3]) || (vGuardMasterFaultsData[4]))
							ReportErrorMessage("Remove and Install Tube Cover to Reset Interlock System", cEMailMaintenance, 0);
					}
					vGuardMasterInterlockFaults = vGuardMasterInterlockFaults & (!cTubeCoverMaskBitA);
				}

				if (((vGuardMasterRawInterlockData & cTubeCoverMaskBits) != cTubeCoverMaskBits) && //if fron door was just opened log event
					((vOldGuardMasterRawInterlockData & cTubeCoverMaskBits) == cTubeCoverMaskBits))
				{
					if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
						(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
					{
						ReportErrorMessage("Operator Opened Detector Door", cUserAction, 0);

					}
					else
					{
						ReportErrorMessage("Operator Removed X-Ray Tube Cover", cUserAction, 0);

					}
				}
				else
				if (((vGuardMasterRawInterlockData & cTubeCoverMaskBits) == cTubeCoverMaskBits) && //if fron door was just opened log event
					((vOldGuardMasterRawInterlockData & cTubeCoverMaskBits) != cTubeCoverMaskBits))
				{
					if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
						(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
					{
						ReportErrorMessage("Operator Closed Detector Door", cUserAction, 0);

					}
					else
					{
						ReportErrorMessage("Operator Installed X-Ray Tube Cover", cUserAction, 0);

					}
				}

				//7th input on Guard Master is Left Conveyor Cover
				if (((vGuardMasterRawInterlockData & cLeftConveyorCoversMaskBitA) && (!(vGuardMasterRawInterlockData & cLeftConveyorCoversMaskBitB))) || //if interlock fault state where one bit is active and the other is not
					((!(vGuardMasterRawInterlockData & cLeftConveyorCoversMaskBitA)) && (vGuardMasterRawInterlockData & cLeftConveyorCoversMaskBitB)))
				{
					if (!(((vOldGuardMasterRawInterlockData & cLeftConveyorCoversMaskBitA) && (!(vOldGuardMasterRawInterlockData & cLeftConveyorCoversMaskBitB))) || //if previous status did not have a fault, give message
						((!(vOldGuardMasterRawInterlockData & cLeftConveyorCoversMaskBitA)) && (vOldGuardMasterRawInterlockData & cLeftConveyorCoversMaskBitB))))
					{
						ReportErrorMessage("Interlock Circuit FAULT Left Conveyor Door", cEMailMaintenance, 0);
						ReportErrorMessage("Interlock Circuit FAULT Left Conveyor Door", cEMailInspx, 0);
						vGuardMasterInterlockFaults = vGuardMasterInterlockFaults | cLeftConveyorCoversMaskBitA;
					}
				}
				else
				if (((vOldGuardMasterRawInterlockData & cLeftConveyorCoversMaskBitA) && (!(vOldGuardMasterRawInterlockData & cLeftConveyorCoversMaskBitB))) || //if interlock fault state where one bit is active and the other is not on previous read
					((!(vOldGuardMasterRawInterlockData & cLeftConveyorCoversMaskBitA)) && (vOldGuardMasterRawInterlockData & cLeftConveyorCoversMaskBitB)))
				{
					//if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
					//	(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
					//{
					//	ReportErrorMessage("Interlock Circuit FAULT Left Tunnel Curtain corrected", cEMailMaintenance, 0);

					//	if ((vGuardMasterFaultsData[0]) || (vGuardMasterFaultsData[1]) || (vGuardMasterFaultsData[2]) || (vGuardMasterFaultsData[3]) || (vGuardMasterFaultsData[4]))
					//		ReportErrorMessage("Remove and Install Left Tunnel Curtain to Reset Interlock System", cEMailMaintenance, 0);
					//}
					//else
					//{
						ReportErrorMessage("Interlock Circuit FAULT Left Conveyor Door corrected", cEMailMaintenance, 0);

						if ((vGuardMasterFaultsData[0]) || (vGuardMasterFaultsData[1]) || (vGuardMasterFaultsData[2]) || (vGuardMasterFaultsData[3]) || (vGuardMasterFaultsData[4]))
							ReportErrorMessage("Open and Close Left Conveyor Door to Reset Interlock System", cEMailMaintenance, 0);
					//}
					vGuardMasterInterlockFaults = vGuardMasterInterlockFaults & (!cLeftConveyorCoversMaskBitA);
				}

				if (((vGuardMasterRawInterlockData & cLeftConveyorCoversMaskBits) != cLeftConveyorCoversMaskBits) && //if fron door was just opened log event
					((vOldGuardMasterRawInterlockData & cLeftConveyorCoversMaskBits) == cLeftConveyorCoversMaskBits))
				{
					//if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
					//	(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
					//	ReportErrorMessage("Operator Removed Left Tunnel Curtain", cUserAction, 0);
					//else
						ReportErrorMessage("Operator Opened Left Conveyor Door", cUserAction, 0);
					if ((vLocalSystemData->vLastXRayCommandOn) || (vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
					{
						::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						TempString.LoadString(IDS_ConveyorCoverOpened);
						ReportErrorMessage(TempString + " - " + TempString1, cError,15);
#ifdef CompileInHTTPClientOperationsGuardian
						if (vLocalConfigurationData->vEnableOperationsGuardian)
						{
							vGlobalOGLocationString = "conveyor_cover";
							vGlobalOGErrorCode = 204;
							::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageInterlockBroken, cOGSevereMessageInterlockBroken);
						}
#endif
					}
				}
				else
				if (((vGuardMasterRawInterlockData & cLeftConveyorCoversMaskBits) == cLeftConveyorCoversMaskBits) && //if fron door was just opened log event
					((vOldGuardMasterRawInterlockData & cLeftConveyorCoversMaskBits) != cLeftConveyorCoversMaskBits))
				{
					//if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
					//	(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
					//	ReportErrorMessage("Operator Installed Left Tunnel Curtain", cUserAction, 0);
					//else
						ReportErrorMessage("Operator Closed Left Conveyor Door", cUserAction, 0);
				}

				//8th input on Guard Master is Right Conveyor Cover
				if (((vGuardMasterRawInterlockData & cRightConveyorCoversMaskBitA) && (!(vGuardMasterRawInterlockData & cRightConveyorCoversMaskBitB))) || //if interlock fault state where one bit is active and the other is not
					((!(vGuardMasterRawInterlockData & cRightConveyorCoversMaskBitA)) && (vGuardMasterRawInterlockData & cRightConveyorCoversMaskBitB)))
				{
					if (!(((vOldGuardMasterRawInterlockData & cRightConveyorCoversMaskBitA) && (!(vOldGuardMasterRawInterlockData & cRightConveyorCoversMaskBitB))) || //if previous status did not have a fault, give message
						((!(vOldGuardMasterRawInterlockData & cRightConveyorCoversMaskBitA)) && (vOldGuardMasterRawInterlockData & cRightConveyorCoversMaskBitB))))
					{
						//if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
						//	(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
						//	ReportErrorMessage("Interlock Circuit FAULT Left Tunnel Curtain", cEMailMaintenance, 0);
						//else
							ReportErrorMessage("Interlock Circuit FAULT Right Conveyor Door", cEMailMaintenance, 0);
							ReportErrorMessage("Interlock Circuit FAULT Right Conveyor Door", cEMailInspx, 0);
						vGuardMasterInterlockFaults = vGuardMasterInterlockFaults | cRightConveyorCoversMaskBitA;
					}
				}
				else //no fault currently
				if (((vOldGuardMasterRawInterlockData & cRightConveyorCoversMaskBitA) && (!(vOldGuardMasterRawInterlockData & cRightConveyorCoversMaskBitB))) || //if interlock fault state where one bit is active and the other is not on previous read
					((!(vOldGuardMasterRawInterlockData & cRightConveyorCoversMaskBitA)) && (vOldGuardMasterRawInterlockData & cRightConveyorCoversMaskBitB)))
				{
					//if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
					//	(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
					//{
					//	ReportErrorMessage("Interlock Circuit FAULT Right Tunnel Curtain corrected", cEMailMaintenance, 0);

					//	if ((vGuardMasterFaultsData[0]) || (vGuardMasterFaultsData[1]) || (vGuardMasterFaultsData[2]) || (vGuardMasterFaultsData[3]) || (vGuardMasterFaultsData[4]))
					//		ReportErrorMessage("Remove and Install Right Tunnel Curtain to Reset Interlock System", cEMailMaintenance, 0);
					//}
					//else
					//{
						ReportErrorMessage("Interlock Circuit FAULT Right Conveyor Door corrected", cEMailMaintenance, 0);

						if ((vGuardMasterFaultsData[0]) || (vGuardMasterFaultsData[1]) || (vGuardMasterFaultsData[2]) || (vGuardMasterFaultsData[3]) || (vGuardMasterFaultsData[4]))
							ReportErrorMessage("Open and Close Right Conveyor Door to Reset Interlock System", cEMailMaintenance, 0);
					//}
					vGuardMasterInterlockFaults = vGuardMasterInterlockFaults & (!cRightConveyorCoversMaskBitA);
				}

				if (((vGuardMasterRawInterlockData & cRightConveyorCoversMaskBits) != cRightConveyorCoversMaskBits) && //if fron door was just opened log event
					((vOldGuardMasterRawInterlockData & cRightConveyorCoversMaskBits) == cRightConveyorCoversMaskBits))
				{
					//if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
					//	(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
					//	ReportErrorMessage("Operator Removed Right Tunnel Curtain", cUserAction, 0);
					//else
						ReportErrorMessage("Operator Opened Right Conveyor Door", cUserAction, 0);
					if ((vLocalSystemData->vLastXRayCommandOn) || (vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
					{
						::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
						TempString.LoadString(IDS_ConveyorCoverOpened);
						ReportErrorMessage(TempString + " - " + TempString1, cError,15);
#ifdef CompileInHTTPClientOperationsGuardian
						if (vLocalConfigurationData->vEnableOperationsGuardian)
						{
							vGlobalOGLocationString = "conveyor_cover";
							vGlobalOGErrorCode = 204;
							::PostThreadMessage(vGlobalCallingThreadID, cOGSevereMessage, cOGSevereMessageInterlockBroken, cOGSevereMessageInterlockBroken);
						}
#endif
					}
				}
				else
				if (((vGuardMasterRawInterlockData & cRightConveyorCoversMaskBits) == cRightConveyorCoversMaskBits) && //if fron door was just opened log event
					((vOldGuardMasterRawInterlockData & cRightConveyorCoversMaskBits) != cRightConveyorCoversMaskBits))
				{
					//if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
					//	(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
					//	ReportErrorMessage("Operator Installed Right Tunnel Curtain", cUserAction, 0);
					//else
						ReportErrorMessage("Operator Closed Right Conveyor Door", cUserAction, 0);
				}

				////9th input on Guard Master is Curtains
				//if (((vGuardMasterRawInterlockData & cCurtainsMaskBitA) && (!(vGuardMasterRawInterlockData & cCurtainsMaskBitB))) || //if interlock fault state where one bit is active and the other is not
				//	((!(vGuardMasterRawInterlockData & cCurtainsMaskBitA)) && (vGuardMasterRawInterlockData & cCurtainsMaskBitB)))
				//{
				//	if (!(((vOldGuardMasterRawInterlockData & cCurtainsMaskBitA) && (!(vOldGuardMasterRawInterlockData & cCurtainsMaskBitB))) || //if previous status did not have a fault, give message
				//		((!(vOldGuardMasterRawInterlockData & cCurtainsMaskBitA)) && (vOldGuardMasterRawInterlockData & cCurtainsMaskBitB))))
				//	{
				//		if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
				//			(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
				//			ReportErrorMessage("Interlock Circuit FAULT in Spare Interlock", cEMailMaintenance, 0);
				//		else
				//			ReportErrorMessage("Interlock Circuit FAULT X-Ray Curtains", cEMailMaintenance, 0);
				//		vGuardMasterInterlockFaults = vGuardMasterInterlockFaults | cCurtainsMaskBitA;
				//	}
				//}
				//else //no fault currently
				//if (((vOldGuardMasterRawInterlockData & cCurtainsMaskBitA) && (!(vOldGuardMasterRawInterlockData & cCurtainsMaskBitB))) || //if interlock fault state where one bit is active and the other is not on previous read
				//	((!(vOldGuardMasterRawInterlockData & cCurtainsMaskBitA)) && (vOldGuardMasterRawInterlockData & cCurtainsMaskBitB)))
				//{
				//	if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
				//		(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
				//		ReportErrorMessage("Interlock Circuit FAULT in Spare Interlock corrected", cEMailMaintenance, 0);
				//	else
				//		ReportErrorMessage("Interlock Circuit FAULT X-Ray Curtains corrected", cEMailMaintenance, 0);
				//	vGuardMasterInterlockFaults = vGuardMasterInterlockFaults & (!cCurtainsMaskBitA);
				//}

				//if (((vGuardMasterRawInterlockData & cCurtainsMaskBits) != cCurtainsMaskBits) && //if fron door was just opened log event
				//	((vOldGuardMasterRawInterlockData & cCurtainsMaskBits) == cCurtainsMaskBits))
				//{
				//	if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
				//		(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
				//		ReportErrorMessage("Spare Interlock Opened", cUserAction, 0);
				//	else
				//		ReportErrorMessage("Operator Removed X-Ray Curtain", cUserAction, 0);
				//	if ((vLocalSystemData->vLastXRayCommandOn) || (vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
				//	{
				//		::PostThreadMessage(vGlobalCallingThreadID,cEmergencyShutXRaysDownMessage,0,0);
				//		TempString.LoadString(IDS_ConveyorCoverOpened);
				//		ReportErrorMessage(TempString + " - " + TempString1, cError,15);
				//	}
				//}
				//else
				//if (((vGuardMasterRawInterlockData & cCurtainsMaskBits) == cCurtainsMaskBits) && //if fron door was just opened log event
				//	((vOldGuardMasterRawInterlockData & cCurtainsMaskBits) != cCurtainsMaskBits))
				//{
				//	if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
				//		(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
				//		ReportErrorMessage("Spare Interlock Closed", cUserAction, 0);
				//	else
				//		ReportErrorMessage("Operator Installed X-Ray Curtain", cUserAction, 0);
				//}

				//bit 1 and 2 of vLocalSystemData->vSourceInterLocks is Front and Back door are closed, so set their status
				if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) || (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
				{
					if (((vGuardMasterRawInterlockData & cFrontDoorMaskBits) == cFrontDoorMaskBits) && ((vGuardMasterRawInterlockData & cTubeCoverMaskBits) == cTubeCoverMaskBits))  //Upper and bottom door underneath are close or not
						vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x1000; //set status to front and read door closed
					else
						vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks & 0xEFFF; //set status to front and read door open

					if (((vGuardMasterRawInterlockData & cBackDoorMaskBits) == cBackDoorMaskBits) && ((vGuardMasterRawInterlockData & cLeftConveyorCoversMaskBits) == cLeftConveyorCoversMaskBits) &&
						((vGuardMasterRawInterlockData & cRightConveyorCoversMaskBits) == cRightConveyorCoversMaskBits))  //lower front door closed and two side doors
						vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x0800; //set status to front and read door closed
					else
						vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks & 0xF7FF; //set status to front and read door open
				}
				else
				{
					if (((vGuardMasterRawInterlockData & cFrontDoorMaskBits) == cFrontDoorMaskBits) && ((vGuardMasterRawInterlockData & cBackDoorMaskBits) == cBackDoorMaskBits))  //front and read door closed
						vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x1000; //set status to front and read door closed
					else
						vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks & 0xEFFF; //set status to front and read door open

					if (((vGuardMasterRawInterlockData & cTubeCoverMaskBits) == cTubeCoverMaskBits) &&
						((vGuardMasterRawInterlockData & cLeftConveyorCoversMaskBits) == cLeftConveyorCoversMaskBits) && ((vGuardMasterRawInterlockData & cRightConveyorCoversMaskBits) == cRightConveyorCoversMaskBits))  //front and read door closed
						vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x0800; //set status to side covers closed
					else
						vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks & 0xF7FF; //set status to side covers open
				}

				vOldGuardMasterRawInterlockData = vGuardMasterRawInterlockData;

				SetRedStatusIndicator();
			}
		}
		else
			ReportErrorMessage("Guard Master Serial Data Error in Read Interlock Status", cEMailInspx, 32000);
			//WORD TempPreviousInterlockValue = vLocalSystemData->vSourceInterLocks;

			////uController does not send Conveyor position for this change on digital line
			//TempPreviousInterlockValue = vLocalSystemData->vPreviousInterlockValue;
			//vLocalSystemData->vPreviousInterlockValue = vLocalSystemData->vSourceInterLocks;

			//if (!vLocalConfigurationData->vSimulateLightControlBoard)
			//	vLocalSystemData->vRawInterlockInput = (TempBuffer[2] << 8) + TempBuffer[3];

			//vLocalSystemData->vSourceInterLocks = vLocalSystemData->vRawInterlockInput;

			////set coolant flow OK since 50 watt and TDs have no coolant
			//if ((vLocalConfigurationData->vScanTracType == cBriosoScanTrac) || (vLocalConfigurationData->vScanTracType == cAtempoScanTrac) ||
			//	(vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) || (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) ||
			//	(vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac))
			//	vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x0400;

			//if ((!vLocalSystemData->vFPGAVersion9Point1OrHigher) || (vGlobalFPGAVersion10Point0OrHigher))
			//{
			//	vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x0040;//set power indicator as set as wire is not connected before 9.1 or after 10.0
			//}
			//if (!vLocalSystemData->vFPGAVersion8Point7OrHigher)
			//{
			//	vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x0060;//set spellman interlock and power as set as wire is not connected before 8.7
			//}

			////upstreamsignal
			//if (vLocalConfigurationData->vUseDownStreamSignalThatStopsConveyor == cUseDownStream) //if not using down stream signal, set active
			//{
			//	WORD TempOldDownStream = vLocalSystemData->vPreviousInterlockValue & 0x2000;
			//	WORD TempCurrentDownStream = vLocalSystemData->vSourceInterLocks & 0x2000;

			//	if (TempCurrentDownStream)
			//		vLocalSystemData->vDownStreamSignalSaysConveyorOn = true;
			//	else
			//		vLocalSystemData->vDownStreamSignalSaysConveyorOn = false;

			//	if (TempOldDownStream != TempCurrentDownStream)
			//	{
			//		//11/18/2009
			//		if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))
			//		if (TempCurrentDownStream)  //conveyor starting, turn on simulated encoder
			//			::PostThreadMessage(vGlobalCallingThreadID,cDownStreamSignalChangedMessage,1,1);
			//		else
			//			::PostThreadMessage(vGlobalCallingThreadID,cDownStreamSignalChangedMessage,0,0);
			//	}
			//}
			//else
			//{
			//	//pipe flow on input bit not used by software, only used by FPGA
			//	vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks | 0x2000);
			//}

			//if (vGlobalDXMXRayPort)
			//{
			//	if (vGlobalDXMXRayPort->vInterLockStatus)
			//		vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks | 0x0020);//set bit 6, the x-ray interlock bit
			//	else
			//		vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks & 0xFFDF); //clear bit 6, the x-ray interlock bit

			//	vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x0010;  //set Spellman XLF fault to OK
			//}
			//else
			//if (vGlobalMonoBlockXRayPort)
			//{
			//	if (vGlobalMonoBlockXRayPort->vInterLockStatus)
			//		vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks | 0x0020);//set bit 6, the x-ray interlock bit
			//	else
			//		vLocalSystemData->vSourceInterLocks = (vLocalSystemData->vSourceInterLocks & 0xFFDF); //clear bit 6, the x-ray interlock bit

			//	vLocalSystemData->vSourceInterLocks = vLocalSystemData->vSourceInterLocks | 0x0010;  //set Spellman XLF fault to OK

			//}
			//::PostThreadMessage(vGlobalCallingThreadID,cCheckInterlocksIn200MSMessage,0,0);
			//if (vLocalSystemData->vLogFile.vLogSerialData)
			//	vLocalSystemData->vLogFile.WriteToLogFile("Interlock Changed: " + ByteToHex((BYTE)(vLocalSystemData->vSourceInterLocks >> 8)) + ", " +
			//		ByteToHex((BYTE)(vLocalSystemData->vSourceInterLocks)),cDebugMessage);

			//TempTime = CIFCOS::GetSystimeMillisecs();

			//if (TempPreviousInterlockValue != vLocalSystemData->vPreviousInterlockValue)
			//if ((TempTime < vLocalSystemData->vLastInterlockChangedTime + 200) &&
			//	(TempTime + 200 > vLocalSystemData->vLastInterlockChangedTime))  //if < 200 milliseconds since interlocks changed
			//{
			//	if (TempPreviousInterlockValue == vLocalSystemData->vSourceInterLocks)  //if just changed back to what it was
			//	{
			//		vLocalSystemData->vInterlock200mSecChangeCount++;
			//		if (vLocalSystemData->vInterlock200mSecChangeCount == 1)
			//			ReportErrorMessage("Interlock Changed Less Than 200mSec: " +
			//				ByteToHex((BYTE)(vLocalSystemData->vPreviousInterlockValue >> 8)) + ", " +
			//				ByteToHex((BYTE)(vLocalSystemData->vPreviousInterlockValue)) + " -> " +
			//				ByteToHex((BYTE)(vLocalSystemData->vSourceInterLocks >> 8)) + ", " +
			//				ByteToHex((BYTE)(vLocalSystemData->vSourceInterLocks)), cWriteToLog,0);
			//	}
			//}

			//vLocalSystemData->vLastInterlockChangedTime = TempTime;

			//if (vLocalSystemData->vSourceInterLocks & 0x8000) //Encoder Error Indicator
			//{
			//	if (vGlobalFPGAVersion15point42orAbove)
			//	if (!vLocalSystemData->vGaveEncoderErrorThisRun)
			//	if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
			//	if ((!vLocalConfigurationData->vContinuousFeedEncoderRate) && (!vLocalConfigurationData->vContinuousFeedContainerRate))
			//	if (!vLocalSystemData->vFPGASimulateEncoderRate)
			//	if (vLocalSystemData->vActualEncoderRate < vLocalSystemData->vMaximumEncoderRate)
			//	if (vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
			//	{
			//		ReportErrorMessage("Encoder Faulty Indicator From FPGA", cWriteToLog, 0);
			//		if (vLocalSystemData->vEncoderFaultyCount < 250)
			//			vLocalSystemData->vEncoderFaultyCount++;

			//		if (vLocalSystemData->vEncoderFaultyCount > 40)
			//		{
			//			ReportErrorMessage("Encoder Faulty - Irregular Pulses - Replace Encoder", cEMailMaintenance, 0);
			//			ReportErrorMessage("Encoder Faulty - Irregular Pulses - Replace Encoder", cEMailInspx, 0);
			//			vLocalSystemData->vDisplayingAnEncoderError = true;
			//			vLocalSystemData->vGaveEncoderErrorThisRun = true;
			//			if (vMainWindowPointer)
			//			{
			//				vMainWindowPointer->SetInterlockStatus();
			//				vMainWindowPointer->UpdateClearYellowMessageButton();
			//			}
			//		}
			//	}
			//}

			////b	rh test
			////ReportErrorMessage("Interlock Changed: " + ByteToHex((BYTE)(vLocalSystemData->vSourceInterLocks >> 8)) + ", " +
			////		ByteToHex((BYTE)(vLocalSystemData->vSourceInterLocks)),cError,0);
			//CheckInterlocks(true);

	}
	else
		ReportErrorMessage("GuardMaster Received response, but none expected", cEMailInspx, 32000);
}

void CSerialPortGuardMaster::DoBackGroundWork()
{
	if (vWaitingForReply)
	{
		if (vCommunicationsTimeOutCounter < 200)
		vCommunicationsTimeOutCounter++;

		if (vCommunicationsTimeOutCounter >= 10)
		{
			vWaitingForReply = false; //lost comm, but retry
			vCommunicationsTimeOutCounter = 0;
			if (!vNoCommunications) //if was communicating, flag that we lost com as last command was not answered
			{
				vNoCommunications = true;
				if (vDetectedValidGuardMasterDevice)
				{
					ReportErrorMessage("Lost Communication with " + vComPortString + " Guard Master Interlock System", cError,0);
					ReportErrorMessage("Lost Communication with " + vComPortString + " Guard Master Interlock System", cEMailInspx,0);
					if (vMainWindowPointer)
						vMainWindowPointer->SetInterlockStatus();
				}
				else
				{
					ReportErrorMessage("Did Not Detect Guard Master Interlock System-using uC Board", cAction,0);
				}
			}
		}
	}
	//SendGetInterlockStatusCommand();

	if (!vWaitingForReply)
	if (vBackgroundCount > 0)
	{
		vBackgroundCount = 0;
		SendGetInterlockStatusCommand();
	}
	else
	{
		vBackgroundCount++;
		vBackgroundFaultCount++;

		if (vBackgroundFaultCount > 3)
			vBackgroundFaultCount = 0;

		if (vBackgroundFaultCount == 3)
			SendGetFirmwareVersionCommand();
		else
		if (vBackgroundFaultCount == 2)
			SendGetFaultsCommand();
		else
		if (vBackgroundFaultCount == 1)
			SendGetHardwareFaultsCommand();
		else
			SendGetFaultLEDCommand();
	}


	//if (vBackgroundCount > 0)
	//{
	//	vBackgroundCount = 0;
	//	SendGetInterlockStatusCommand();
	//}
	//else
	//{
	//	vBackgroundCount++;
	//	SendGetInterlockOutputCommand();
	//}
}


void CSerialPortGuardMaster::SendGetDeviceIDCommand()
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
	if (vDetectedValidGuardMasterDevice)
		vLocalSystemData->vLogFile.WriteToLogFile("Send Guard Master Get ID Command", cDebugMessage);

	tSerialCommand TempCommand;
	TempCommand[0] = 1;
	TempCommand[1] = 11; //Read Input Register command
	SendSerialCommand(TempCommand, 2);
	Sleep(cSendCommandSleepTime);
}

//void CSerialPortGuardMaster::SendGetInterlockOutputStateCommand()
//{
//	//get interlock inputs
//	tSerialCommand TempCommand;
//	TempCommand[0] = 1; //address device
//	TempCommand[1] = 1; //Read Coils Input command
//	TempCommand[2] = 0x01; //MSB of Register Address 361 = 0x0110
//	TempCommand[3] = 0x10; //LSB of Register Address = 0x10
//	TempCommand[4] = 0;	//msb of quantity of coils to read = 0
//	TempCommand[5] = 0x16; //Quantity of Coils to read = 22
//	SendSerialCommand(TempCommand, 6);
//	vLastCommandSent = cReadInterLockState;
//	Sleep(cSendCommandSleepTime);
//}

void CSerialPortGuardMaster::SendClearFaultCommand()
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
	if (vDetectedValidGuardMasterDevice)
		vLocalSystemData->vLogFile.WriteToLogFile("Send Guard Master Clear Fault Command", cDebugMessage);

	tSerialCommand TempCommand;
	TempCommand[0] = 1;
	TempCommand[1] = 5; //Write Bit command
	TempCommand[2] = 8; //Use I/O 9 as the reset fault control
	TempCommand[3] = 1; //activate retrigger sequence
	SendSerialCommand(TempCommand, 3);
	Sleep(cSendCommandSleepTime);
	Sleep(250);

	TempCommand[0] = 1;
	TempCommand[1] = 5; //Write Bit command
	TempCommand[2] = 8; //Use I/O 9 as the reset fault control
	TempCommand[3] = 0; //clear retrigger sequence
	SendSerialCommand(TempCommand, 3);
}

void CSerialPortGuardMaster::SendGetInterlockStatusCommand()
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
	if (vDetectedValidGuardMasterDevice)
		vLocalSystemData->vLogFile.WriteToLogFile("Send Guard Master Get Interlock Data Command", cDebugMessage);

	//get interlock inputs
	tSerialCommand TempCommand;
	TempCommand[0] = 1; //address device
	TempCommand[1] = 1; //Read Coils Input command
	TempCommand[2] = 0x01; //MSB of Register Address 272 = 0x0110
	TempCommand[3] = 0x10; //LSB of Register Address = 0x10
	TempCommand[4] = 0;	//msb of quantity of coils to read = 0
	TempCommand[5] = 0x16; //Quantity of Coils to read = 22
	SendSerialCommand(TempCommand, 6);
	vLastCommandSent = cReadInterLockState;
	Sleep(cSendCommandSleepTime);
}

void CSerialPortGuardMaster::SendResetToGuardMaster()
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
	if (vDetectedValidGuardMasterDevice)
		vLocalSystemData->vLogFile.WriteToLogFile("Send Guard Master Reset Command", cDebugMessage);

	//get interlock inputs
	tSerialCommand TempCommand;
	TempCommand[0] = 1; //address device
	TempCommand[1] = 5; //Write Coils Input command
	TempCommand[2] = 0x00; //MSB of Register Address 0 = 0
	TempCommand[3] = 0x01; //LSB of Register Address = 0
	TempCommand[4] = 0xFF;	//write all 1s to address high
	TempCommand[5] = 0x00; //write all 1s to address Low
	SendSerialCommand(TempCommand, 6);
	vLastCommandSent = cSendResetCommand;
	Sleep(cSendCommandSleepTime + 225);

	TempCommand[0] = 1; //address device
	TempCommand[1] = 5; //Write Coils Input command
	TempCommand[2] = 0x00; //MSB of Register Address 0 = 0
	TempCommand[3] = 0x01; //LSB of Register Address = 0
	TempCommand[4] = 0x00;	//write all 1s to address high
	TempCommand[5] = 0x00; //write all 1s to address Low
	SendSerialCommand(TempCommand, 6);
	vLastCommandSent = cSendResetCommand;
	Sleep(cSendCommandSleepTime);
}

void CSerialPortGuardMaster::SendGetFaultsCommand()
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
	if (vDetectedValidGuardMasterDevice)
		vLocalSystemData->vLogFile.WriteToLogFile("Send Guard Master Get Faults Command", cDebugMessage);

	//get faults read into vGuardMasterFaultsData
	tSerialCommand TempCommand;
	TempCommand[0] = 1; //address device
	TempCommand[1] = 1; //Read Coils Input command
	TempCommand[2] = 0x01; //MSB of Register Address 393 = 0x0189
	TempCommand[3] = 0x08; //LSB of Register Address = 0x10
	TempCommand[4] = 0;	//msb of quantity of coils to read = 0
	TempCommand[5] = 118; //Quantity of Coils to read = 120 =  0x78
	SendSerialCommand(TempCommand, 6);
	vLastCommandSent = cReadFaults;
	Sleep(cSendCommandSleepTime);
}

void CSerialPortGuardMaster::SendGetFaultLEDCommand()
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
	if (vDetectedValidGuardMasterDevice)
		vLocalSystemData->vLogFile.WriteToLogFile("Send Guard Master Get Fault LED Command", cDebugMessage);

	//get faults read into vGuardMasterFaultsData
	tSerialCommand TempCommand;
	TempCommand[0] = 1; //address device
	TempCommand[1] = 1; //Read Coils Input command
	TempCommand[2] = 0x02; //MSB of Register Address 522 = 0x020A
	TempCommand[3] = 0x0A; //LSB of Register Address = 0x10
	TempCommand[4] = 0;	//msb of quantity of coils to read = 0
	TempCommand[5] = 2; //Quantity of Coils to read = 2 =  0x2
	SendSerialCommand(TempCommand, 6);
	vLastCommandSent = cReadFaultLED;
	Sleep(cSendCommandSleepTime);
}

void CSerialPortGuardMaster::SendGetFirmwareVersionCommand()
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
	if (vDetectedValidGuardMasterDevice)
		vLocalSystemData->vLogFile.WriteToLogFile("Send Guard Master Get Firmware Version Command", cDebugMessage);

	//get faults read into vGuardMasterFaultsData
	tSerialCommand TempCommand;
	TempCommand[0] = 1; //address device
	TempCommand[1] = 3; //Read Holding Register command
	TempCommand[2] = 0x07; //MSB of Register Address 2003 = 0x07D3
	TempCommand[3] = 0xD2; //LSB of Register Address = 0x10
	TempCommand[4] = 0;	//msb of quantity of coils to read = 0
	TempCommand[5] = 2; //Quantity of Registers to read = 2 =  0x2
	SendSerialCommand(TempCommand, 6);
	vLastCommandSent = cReadFirmwareVersion;
	Sleep(cSendCommandSleepTime);
}

void CSerialPortGuardMaster::SendGetHardwareFaultsCommand()
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
	if (vDetectedValidGuardMasterDevice)
		vLocalSystemData->vLogFile.WriteToLogFile("Send Guard Master Get Hardware Faults Command", cDebugMessage);

	//get faults read into vGuardMasterFaultsData
	tSerialCommand TempCommand;
	TempCommand[0] = 1; //address device
	TempCommand[1] = 1; //Read Coils Input command
	TempCommand[2] = 0x01; //MSB of Register Address 265 = 0x0109
	TempCommand[3] = 0x09; //LSB of Register Address = 0x10
	TempCommand[4] = 0;	//msb of quantity of coils to read = 0
	TempCommand[5] = 0; //Quantity of Coils to read = 8 =  8
	SendSerialCommand(TempCommand, 6);
	vLastCommandSent = cReadHardwareFaults;
	Sleep(cSendCommandSleepTime);
}



//void CSerialPortGuardMaster::SendGetInterlockOutputCommand()
//{
	//if (vLocalSystemData->vLogFile.vLogSerialData)
	//	vLocalSystemData->vLogFile.WriteToLogFile("Send Guard Master Get Interlock Output Command", cDebugMessage);

//	//get interlock inputs
//	tSerialCommand TempCommand;
//	TempCommand[0] = 1; //address device
//	TempCommand[1] = 1; //Read Coils Input command
//	TempCommand[2] = 0x01; //MSB of Register Address 305 = 0x0131
//	TempCommand[3] = 0x31; //LSB of Register Address = 0x10
//	TempCommand[4] = 0;	//msb of quantity of coils to read = 0
//	TempCommand[5] = 0x8; //Quantity of Coils to read = 8
//	SendSerialCommand(TempCommand, 6);
//	vLastCommandSent = cReadOutputState;
//	Sleep(cSendCommandSleepTime);
//}
//
void CSerialPortGuardMaster::SetRedStatusIndicator()
{
	vLocalSystemData->vOpenInterlock = "";

	if ((vGuardMasterRawInterlockData & cFrontDoorMaskBits) != cFrontDoorMaskBits)  //front and read door closed
	{//"F/R Door Interlock Broken
		if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
			(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
			vLocalSystemData->vOpenInterlock.LoadString(IDS_TopDoorOpen);
		else
			vLocalSystemData->vOpenInterlock.LoadString(IDS_FrontDoorOpen);
	}
	else
	if ((vGuardMasterRawInterlockData & cBackDoorMaskBits) != cBackDoorMaskBits) //if fron door was just opened log event
	{
		if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
			(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
			vLocalSystemData->vOpenInterlock.LoadString(IDS_LowerFrontDoor);
		else
			vLocalSystemData->vOpenInterlock.LoadString(IDS_RearDoorOpen);
	}
	else
	if ((vGuardMasterRawInterlockData & cEStopMaskBits) != cEStopMaskBits)//if fron door was just opened log event
		vLocalSystemData->vOpenInterlock.LoadString(IDS_EStop);
	else
	if ((vGuardMasterRawInterlockData & cKeySwitchMaskBits) != cKeySwitchMaskBits)//if fron door was just opened log event
		vLocalSystemData->vOpenInterlock.LoadString(IDS_KeySwitch);
	else
	//if ((vGuardMasterRawInterlockData & cDetectorXRayShieldsMaskBits) != cDetectorXRayShieldsMaskBits)//if fron door was just opened log event
	//	vLocalSystemData->vOpenInterlock.LoadString(IDS_XRayDetectorShield);
	//else
	if ((vGuardMasterRawInterlockData & cTubeCoverMaskBits) != cTubeCoverMaskBits) //if fron door was just opened log event
	{
		if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
			(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
			vLocalSystemData->vOpenInterlock.LoadString(IDS_BottomDoor);
		else
			vLocalSystemData->vOpenInterlock.LoadString(IDS_XRayTubeCover);
	}
	else
	if ((vGuardMasterRawInterlockData & cLeftConveyorCoversMaskBits) != cLeftConveyorCoversMaskBits) //if fron door was just opened log event
	{
		//if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
		//	(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
		//	vLocalSystemData->vOpenInterlock.LoadString(IDS_LTunnelCurtains);
		//else
			vLocalSystemData->vOpenInterlock.LoadString(IDS_LConveyorCover);
	}
	else
	if ((vGuardMasterRawInterlockData & cRightConveyorCoversMaskBits) != cRightConveyorCoversMaskBits)
	{
		//if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
		//	(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
		//	vLocalSystemData->vOpenInterlock.LoadString(IDS_RTunnelCurtains);
		//else
			vLocalSystemData->vOpenInterlock.LoadString(IDS_RConveyorCover);
	}
	else
	if (!(vLocalSystemData->vSourceInterLocks & 0x0400))
	{
		if (vLocalConfigurationData->vUseExternalInterlock)
			vLocalSystemData->vOpenInterlock.LoadString(IDS_ExternalInterlock);
		else
			vLocalSystemData->vOpenInterlock.LoadString(IDS_CoolantFault);
	}
	else
	if (!(vLocalSystemData->vSourceInterLocks & 0x0010))
		vLocalSystemData->vOpenInterlock.LoadString(IDS_XRayFault);
	else
	if (!(vLocalSystemData->vSourceInterLocks & 0x0020))
		vLocalSystemData->vOpenInterlock.LoadString(IDS_HVPSInterlock);
	else
	if (vLocalSystemData->vPosteduCVoltageProblemMessage)
		vLocalSystemData->vOpenInterlock.LoadString(IDS_PowerSupplyProblem);
	else
	if (vLocalSystemData->vAirConditionerFailed)
		vLocalSystemData->vOpenInterlock.LoadString(IDS_ACFailed);
	else
	if (vLocalSystemData->vWeightTrendingAlarm)
		vLocalSystemData->vOpenInterlock.LoadString(IDS_WeightTrend);
	else
	if ((vLocalSystemData->vCheckWeighOffsetCalibrationNeeded) && (vLocalSystemData->vSystemRunMode == cRunningSystemMode))
		vLocalSystemData->vOpenInterlock.LoadString(IDS_CalibrateWeight);
	else
	if (vLocalSystemData->vShutterFailedLastTest)
		vLocalSystemData->vOpenInterlock.LoadString(IDS_ShutterFailed);
	else
	if (vLocalSystemData->vProductHasAPoorLearn)
		vLocalSystemData->vOpenInterlock.LoadString(IDS_PoorLearn);
	else
	//if (vLocalSystemData->vHadABrownOut)
	//	vLocalSystemData->vOpenInterlock.LoadString(IDS_BrownOut);
	//else
	//if (vLocalSystemData->vHadAnACPowerDropOut)
	//	vLocalSystemData->vOpenInterlock.LoadString(IDS_ACDropOut);
	//else
	if (vLocalSystemData->vHadAPowerMoniterMessageError % 16)	//if flags 1, 2, 4, 8 are on
		vLocalSystemData->vOpenInterlock.LoadString(IDS_ACPowerCriticalError);
	else
	if  (vLocalSystemData->vGivenTemperatureSensorWarning == cTemperatureSensorWarningRedBoxSent)
		vLocalSystemData->vOpenInterlock = "Temp Sensor";
	else
	if (vLocalSystemData->vDetectorCalibrationNeeded)
		vLocalSystemData->vOpenInterlock.LoadString(IDS_DetectorCalibrationNeeded);
	else
	if (!(vLocalSystemData->vSourceInterLocks & 0x0001)) //red lamp
		vLocalSystemData->vOpenInterlock.LoadString(IDS_RedLight);
	else
	if (!(vLocalSystemData->vSourceInterLocks & 0x0002)) //yellow lamp
		vLocalSystemData->vOpenInterlock.LoadString(IDS_YellowLight);
	else
	if (!(vLocalSystemData->vSourceInterLocks & 0x0004)) //blue lamp OK
		vLocalSystemData->vOpenInterlock.LoadString(IDS_BlueLight);

	if (vMainWindowPointer)
		vMainWindowPointer->SetInterlockStatus();
}
