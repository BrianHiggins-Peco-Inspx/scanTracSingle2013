

#include "stdafx.h"
#include "ScanTrac.h"
#include "SerialPortReview.h"
#include "SerialPortModBus.h"
#include "SerialPortPowerFlex40.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CSerialPortPowerFlex40, CWinThread)


CSerialPortPowerFlex40::CSerialPortPowerFlex40(void)
{
	vBackgroundCount = 0;
	vCurrentSpeed = 500; //500 / 10 = 50 hz
	vSpeedMonitor = 0;
	vDetectedValidPowerFlex40Device = false;
	vDirectionIsReverse = false;
	vPowerFlex40Status = 0;
	vPowerFlex40FaultCodes = 0;
	ClearCommandQueue();
}


CSerialPortPowerFlex40::~CSerialPortPowerFlex40(void)
{
}

void CSerialPortPowerFlex40::DecodeMessage()
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
	//	ReportErrorMessage("PowerFlex40 Serial Data Error", cEMailInspx, 32000);
	//	vWaitingForReply = 0;
	//}
	//else
//enum {cNoPowerFlex40Command, cPowerFlex40ReadStatus, cPowerFlex40StartCommand, cPowerFlex40StopCommand, cPowerFlex40SetSpeedCommand, cPowerFlex40GetSpeed, cPowerFlex40ReadFaultCodes, 
//	cPowerFlex40RemoteControlCommand, cPowerFlex40ButtonControlCommand};
	
	if ((vLastCommandSent == cPowerFlex40ReadStatus) && (TempMessageByteCount > 1))
	{
		//if (TempBuffer[2] == 4) //got correct number of bytes of data for LED status request
		{
			if (!vDetectedValidPowerFlex40Device)
			{
				vDetectedValidPowerFlex40Device = true;
				ReportErrorMessage("Detected PowerFlex40 Motor Controller", cAction,0);
			}
			vPowerFlex40Status = (((WORD)TempBuffer[4]) << 8) + TempBuffer[3];
			vWaitingForReply = 0;
		}
	}
	else
	if ((vLastCommandSent == cPowerFlex40ReadFaultCodes) && (TempMessageByteCount > 1))
	{
		//if (TempBuffer[2] == 4) //got correct number of bytes of data for LED status request
		{
			if (!vDetectedValidPowerFlex40Device)
			{
				vDetectedValidPowerFlex40Device = true;
				ReportErrorMessage("Detected PowerFlex40 Motor Controller", cAction,0);
			}
			vPowerFlex40FaultCodes = (((WORD)TempBuffer[4]) << 8) + TempBuffer[3];
			vWaitingForReply = 0;
		}
	}
	else
	if ((vLastCommandSent == cPowerFlex40GetSpeed) && (TempMessageByteCount > 1))
	{
		//if (TempBuffer[2] == 4) //got correct number of bytes of data for LED status request
		{
			if (!vDetectedValidPowerFlex40Device)
			{
				vDetectedValidPowerFlex40Device = true;
				ReportErrorMessage("Detected PowerFlex40 Motor Controller", cAction,0);
			}
			vSpeedMonitor = (((WORD)TempBuffer[4]) << 8) + TempBuffer[3];
			vWaitingForReply = 0;
		}
	}
	//
	//if (!vWaitingForReply)
	//if (vCommandQueueu[0] != cNoPowerFlex40Command)
	//	SendNextCommandInQueue();
}

void CSerialPortPowerFlex40::DoBackGroundWork()
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
				if (vDetectedValidPowerFlex40Device)
				{
					ReportErrorMessage("Lost Communication with " + vComPortString + " PowerFlex40 Conveyor Controller", cError,0);
					ReportErrorMessage("Lost Communication with " + vComPortString + " PowerFlex40 Conveyor Controller", cEMailInspx,0);
					if (vMainWindowPointer)
						vMainWindowPointer->SetInterlockStatus();
				}
				else
				{
					ReportErrorMessage("Did Not Detect PowerFlex40 Conveyor Controller-using manual control", cAction,0);
				}
			}
		}
	}


	if (!vWaitingForReply)
	{
		if (vCommandQueueu[0] != cNoPowerFlex40Command)
			SendNextCommandInQueue();
		else
		{
			switch (vBackgroundCount)
			{
				case 1: PutCommandOnQueueToSend(cPowerFlex40ReadStatus); break;
				case 2: PutCommandOnQueueToSend(cPowerFlex40ReadFaultCodes); break;
				default: vBackgroundCount = 0; PutCommandOnQueueToSend(cPowerFlex40GetSpeed); break;
			}
			vBackgroundCount++;
		}
	}
}

void CSerialPortPowerFlex40::ClearCommandQueue()
{
	for (BYTE TempLoop = 0; TempLoop < cMaximumCommandsInQueue; TempLoop++)
		vCommandQueueu[TempLoop] = 0;
}

void CSerialPortPowerFlex40::PutCommandOnQueueToSend(BYTE TempCommand)
{
	BYTE TempQueueIndex = 0;
	while ((vCommandQueueu[TempQueueIndex] != cNoPowerFlex40Command) && (TempQueueIndex < cMaximumCommandsInQueue - 1))
		TempQueueIndex++;

	if ((TempQueueIndex < cMaximumCommandsInQueue) && (vCommandQueueu[TempQueueIndex] == cNoPowerFlex40Command))
	{
		vCommandQueueu[TempQueueIndex] = TempCommand;
		if (!vWaitingForReply)
			SendNextCommandInQueue();
	}
}

void CSerialPortPowerFlex40::SendNextCommandInQueue()
{
	if (!vWaitingForReply)
	if (vCommandQueueu[0] != cNoPowerFlex40Command)
	{
		switch (vCommandQueueu[0])
		{
			case cPowerFlex40ReadStatus: SendGetStatusCommand(); break;
			case cPowerFlex40StartCommand: 
				if (vDirectionIsReverse)
					SendStartGoingBackwardCommand(); 
				else
					SendStartGoingForwardCommand(); 
			break;
			case cPowerFlex40StopCommand: SendStopCommand(); break;
			case cPowerFlex40SetSpeedCommand: SendSpeedCommand(); break;
			case cPowerFlex40GetSpeed: SendGetSpeed(); break;
			case cPowerFlex40ReadFaultCodes: SendGetDeviceErrorCode(); break;
		}
		//move all commands down 1 in queue
		BYTE TempQueueIndex = 0;
		while ((vCommandQueueu[TempQueueIndex + 1] != cNoPowerFlex40Command) && (TempQueueIndex + 1 < cMaximumCommandsInQueue))
		{
			vCommandQueueu[TempQueueIndex] = vCommandQueueu[TempQueueIndex + 1];
			TempQueueIndex++;
		}
		vCommandQueueu[TempQueueIndex] = cNoPowerFlex40Command;
	}
}

void CSerialPortPowerFlex40::StartStopConveyor(bool TempStart)
{
	PutCommandOnQueueToSend(cPowerFlex40RemoteControlCommand);

	if (TempStart)
		PutCommandOnQueueToSend(cPowerFlex40StartCommand);
	else
		PutCommandOnQueueToSend(cPowerFlex40StopCommand);

	PutCommandOnQueueToSend(cPowerFlex40ButtonControlCommand);
}

void CSerialPortPowerFlex40::SetSpeed(WORD TempSpeed)
{
	vCurrentSpeed = TempSpeed;
	PutCommandOnQueueToSend(cPowerFlex40SetSpeedCommand);
}

void CSerialPortPowerFlex40::SendInitialization()
{
	//tSerialCommand TempCommand;
	//TempCommand[0] = 100; //Address of PowerFlex40 on ModBus
	//TempCommand[1] = 6; //Write to Register command
	//TempCommand[2] = 0; //control speed from serial port
	//TempCommand[3] = 38; //control speed from serial port
	//SendSerialCommand(TempCommand, 4);
	//vNumberOfCharactersToExpectInReply = 6;
	//Sleep(cSendCommandSleepTime);
}

void CSerialPortPowerFlex40::SendSpeedCommand()
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Send PowerFlex40 Set Speed: " + dtoa(vCurrentSpeed / 10, 1), cDebugMessage);

	tSerialCommand TempCommand;
	//set speed value
	TempCommand[0] = 100; //Address of PowerFlex40 on ModBus
	TempCommand[1] = 6; //Write to Register command
	TempCommand[2] = 0x20; //Address of Speed Register MSB = decimal 8193
	TempCommand[3] = 0x01; //Address of Speed Register LSB
	TempCommand[4] = (BYTE)(vCurrentSpeed >> 8); //control Run/Stop from serial port, Data MSB
	TempCommand[5] = (BYTE)(vCurrentSpeed); //control Run/Stop from serial port, Data LSB 5 = serial port control
	vLastCommandSent = cPowerFlex40SetSpeedCommand;
	vNumberOfCharactersToExpectInReply = 8;
	SendSerialCommand(TempCommand, 6);
	Sleep(cSendCommandSleepTime);
}

void CSerialPortPowerFlex40::SendGetSpeed()
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Send PowerFlex40 Get Speed", cDebugMessage);

	tSerialCommand TempCommand;
	//set speed value
	TempCommand[0] = 100; //Address of PowerFlex40 on ModBus
	TempCommand[1] = 3; //Read from Register command
	TempCommand[2] = 0x20; //Address of Speed Register MSB = decimal 8193
	TempCommand[3] = 0x01; //Address of Speed Register LSB
	vLastCommandSent = cPowerFlex40GetSpeed;
	vNumberOfCharactersToExpectInReply = 6;
	SendSerialCommand(TempCommand, 4);
	Sleep(cSendCommandSleepTime);
}

void CSerialPortPowerFlex40::SendGetDeviceErrorCode()
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Send PowerFlex40 Get Faults", cDebugMessage);

	tSerialCommand TempCommand;
	TempCommand[0] = 100; //Address of PowerFlex40 on ModBus
	TempCommand[1] = 3; //Read Register command
	TempCommand[2] = 0x21; //Address to read Error Codes MSB Decimal 8449
	TempCommand[3] = 0x01; //Address to read Error Codes LSB
	vLastCommandSent = cPowerFlex40ReadFaultCodes;
	vNumberOfCharactersToExpectInReply = 6;
	SendSerialCommand(TempCommand, 4);
	Sleep(cSendCommandSleepTime);
}

void CSerialPortPowerFlex40::SendGetStatusCommand()
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Send PowerFlex40 Get Status", cDebugMessage);

	tSerialCommand TempCommand;
	TempCommand[0] = 100; //Address of PowerFlex40 on ModBus
	TempCommand[1] = 3; //Read Register command
	TempCommand[2] = 0x21; //Address to read Status MSB, decimal 8448
	TempCommand[3] = 0x00; //Address to read Status LSB
	vLastCommandSent = cPowerFlex40ReadStatus;
	vNumberOfCharactersToExpectInReply = 6;
	SendSerialCommand(TempCommand, 4);
	Sleep(cSendCommandSleepTime);
}
void CSerialPortPowerFlex40::SendStartGoingForwardCommand()
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Send PowerFlex40 Start Forward", cDebugMessage);

	tSerialCommand TempCommand;
	TempCommand[0] = 100; //Address of PowerFlex40 on ModBus
	TempCommand[1] = 6; //Write to Register command
	TempCommand[2] = 0x20; //Address to write command to MSB, decimal 8192 to control Logic Command
	TempCommand[3] = 0x00; //Address to write command to LSB
	TempCommand[4] = 0; //control Run/Stop from serial port, Data MSB
	TempCommand[5] = 0x12; //control Run/Stop from serial port, Data LSB 1 = 3 wire control. 0x22 = backwards, 0x12 = forwards
	vLastCommandSent = cPowerFlex40StartCommand;
	vNumberOfCharactersToExpectInReply = 8;
	SendSerialCommand(TempCommand, 6);
	Sleep(cSendCommandSleepTime);
}

void CSerialPortPowerFlex40::SendStartGoingBackwardCommand()
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Send PowerFlex40 Start Reverse", cDebugMessage);

	tSerialCommand TempCommand;
	TempCommand[0] = 100; //Address of PowerFlex40 on ModBus
	TempCommand[1] = 6; //Write to Register command
	TempCommand[2] = 0x20; //Address to write command to MSB, decimal 8192 to control Logic Command
	TempCommand[3] = 0x00; //Address to write command to LSB
	TempCommand[4] = 0; //control Run/Stop from serial port, Data MSB
	TempCommand[5] = 0x22; //control Run/Stop from serial port, Data LSB 1 = 3 wire control. 0x22 = backwards, 0x12 = forwards
	vLastCommandSent = cPowerFlex40StartCommand;
	vNumberOfCharactersToExpectInReply = 8;
	SendSerialCommand(TempCommand, 6);
	Sleep(cSendCommandSleepTime);
}

void CSerialPortPowerFlex40::SendRemoteStartCommand()
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Send PowerFlex40 Remote Start Enable", cDebugMessage);

	tSerialCommand TempCommand;
	//first must enable command from serial
	TempCommand[0] = 100; //Address of PowerFlex40 on ModBus
	TempCommand[1] = 6; //Write to Register command
	TempCommand[2] = 0; //control Run/Stop from serial port, Address MSB
	TempCommand[3] = 36; //control Run/Stop from serial port, Address LSB
	TempCommand[4] = 0; //control Run/Stop from serial port, Data MSB
	TempCommand[5] = 5; //control Run/Stop from serial port, Data LSB 5 = serial port control
	vLastCommandSent = cPowerFlex40RemoteControlCommand;
	vNumberOfCharactersToExpectInReply = 8;
	SendSerialCommand(TempCommand, 6);
	Sleep(cSendCommandSleepTime + 250);
}

void CSerialPortPowerFlex40::SendStartButtonStartControlCommand()
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Send PowerFlex40 Button Start Enable", cDebugMessage);

	tSerialCommand TempCommand;
	//last must enable start/stop from 3 wire switch
	TempCommand[0] = 100; //Address of PowerFlex40 on ModBus
	TempCommand[1] = 1; //Write to Register command
	TempCommand[2] = 0; //control Run/Stop from serial port, Address MSB
	TempCommand[3] = 36; //control Run/Stop from serial port, Address LSB
	TempCommand[4] = 0; //control Run/Stop from serial port, Data MSB
	TempCommand[5] = 1; //control Run/Stop from serial port, Data LSB 1 = 3 wire control
	vLastCommandSent = cPowerFlex40ButtonControlCommand;
	vNumberOfCharactersToExpectInReply = 8;
	SendSerialCommand(TempCommand, 6);
	Sleep(cSendCommandSleepTime);
}

void CSerialPortPowerFlex40::SendStopCommand()
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Send PowerFlex40 Stop", cDebugMessage);

	tSerialCommand TempCommand;
	TempCommand[0] = 100; //Address of PowerFlex40 on ModBus
	TempCommand[1] = 6; //Write to Register command
	TempCommand[2] = 0x20; //Address to write command to MSB, decimal 8192 to control Logic Command
	TempCommand[3] = 0x00; //Address to write command to LSB
	TempCommand[4] = 0; //control Run/Stop from serial port, Data MSB
	TempCommand[5] = 0x01; //control Run/Stop from serial port, Data LSB 1 = 3 wire control. 0x22 = backwards, 0x12 = forwards 0x01 = stop
	vLastCommandSent = cPowerFlex40StopCommand;
	vNumberOfCharactersToExpectInReply = 8;
	SendSerialCommand(TempCommand, 6);
	Sleep(cSendCommandSleepTime);
}

