#include "stdafx.h"
#include "ScanTrac.h"
#include "SerialPortBase.h"
#include "SerialPortRFIdeasBadgeReader.h"

extern CString vGlobalBadgeReaderRFIDString;

IMPLEMENT_DYNCREATE(CSerialPortRFIDEASBadgeReader, CWinThread)

CSerialPortRFIDEASBadgeReader::CSerialPortRFIDEASBadgeReader()
{
	vBadgeReadEmployeeCode = "Not Read";
	vBadgeReadFacilityCode = "Not Read";
}

CSerialPortRFIDEASBadgeReader::~CSerialPortRFIDEASBadgeReader()
{
}

void CSerialPortRFIDEASBadgeReader::DecodeMessage()
{
	WORD TempReturnValue = 0;
	CString TempReturnNumberString = "";
	
	if (vCurrentReceiveCount)
	{
		vHasReceivedCommunication = true;
		vWaitingForReply = false;
		vCommunicationsTimeOutCounter = 0;
	}

	tSerialResponse TempResponse;
	if (vCurrentReceiveCount > 30)
		vCurrentReceiveCount = 30;
	WORD TempIndex = vReceiveDataTail;
	for (BYTE TempLoop = 0; TempLoop < vCurrentReceiveCount; TempLoop++)
	{
		TempResponse[TempLoop] = vReceiveDataBuffer[TempIndex];
		TempIndex++;
		if (TempIndex >= cReceiveBufferSize)
			TempIndex = 0;
	}

	if (vCurrentReceiveCount > 15)
		ReportErrorMessage(vComPortString + ">ERROR-reader received message overflow", cEMailInspx, 32000);

	if ((TempResponse[0] >= '0') && (TempResponse[0] <= '9'))
		vGlobalBadgeReaderRFIDString = (wchar_t)TempResponse[0];
	else
		vGlobalBadgeReaderRFIDString = "0";

	for (int TempLoop =  1; TempLoop < vCurrentReceiveCount; TempLoop++)
	{
		if ((TempResponse[TempLoop] >= '0') && (TempResponse[TempLoop] <= '9'))
			vGlobalBadgeReaderRFIDString = vGlobalBadgeReaderRFIDString + (wchar_t)TempResponse[TempLoop];
	}

	//testing only
	//ReportErrorMessage((CString)vGlobalBadgeReaderRFIDString + " - rfid read", cError, 32000); 
	::PostThreadMessage(vGlobalCallingThreadID, cRFIDEASBadgeReadMessage, 0, 0);
}

bool CSerialPortRFIDEASBadgeReader::SerialMessageComplete()
{
	bool TempDone = false;
	while (!TempDone)
	{
		vCurrentReceiveCount = ReceiveDataCount();

		if (vCurrentReceiveCount == 0)
			return false;

		if (vCurrentReceiveCount >= 20)//10 bytes, message is complete
				TempDone = true;

		if (vCurrentReceiveCount >= 6)//look to see if have an EOT, if so, that is a full message
		{
			WORD TempIndex = vReceiveDataTail;
			for (WORD TempLoop = 0; TempLoop < vCurrentReceiveCount; TempLoop++)
			{
				if (vReceiveDataBuffer[TempIndex] == 13) //if get a carrage return it is the end of message
				{
					TempDone = true;
					TempLoop = vCurrentReceiveCount;
				}
				if (TempIndex < cReceiveBufferSize - 1)
					TempIndex++;
				else
					TempIndex = 0;
			}
		}

		if (TempDone)
		{
			if (vNoCommunications)
			{
				vNoCommunications = false;
				ReportErrorMessage("Established Communication with RFIDEAS Badge Reader", cAction,0);
			}
			return true;
		}
		else
		{
			if (vCurrentReceiveCount > 30)
			{
				CString TempString = ByteToHex(vReceiveDataBuffer[vReceiveDataTail]);
				ReportErrorMessage("Error-" + vComPortString + "-No EOT - Removing one Byte: " + TempString, cWriteToLog,0);
				vReceiveDataTail++;
				if (vReceiveDataTail >= cReceiveBufferSize)
					vReceiveDataTail = vReceiveDataTail - cReceiveBufferSize;
				if (ReceiveDataCount() < cuControllerMessageLength)
				{
					TempDone = true;
					return false;
				}
			}
		}
		return false;
	}
	return false;
}


void CSerialPortRFIDEASBadgeReader::DoBackGroundWork()
{
	if (vWaitingForReply)
	if (!vNoCommunications)
	{
		vCommunicationsTimeOutCounter++;
		if (vCommunicationsTimeOutCounter > 50) //5 second timeout because background called at 10 hz
		{				
			vNoCommunications = true;
			ReportErrorMessage("No Reply From Badge Reader", cError, 0);
		}
	}

	if (vBackGroundCount > 9)
		vBackGroundCount = 0;
	else
		vBackGroundCount++;

	if (vBackGroundCount == 0)
	{
		if (!vWaitingForReply)
		{
			//	SendAskForStatus();
		}
		else
		{
			if (vWaitingForReply < 10)
				vWaitingForReply++;
			else
				vWaitingForReply = 0;
		}
	}
}

void CSerialPortRFIDEASBadgeReader::RemoveMessageFromBuffer() 
{
	if (vCurrentReceiveCount)
	{
		for (WORD TempLoop = 0; TempLoop < vCurrentReceiveCount; TempLoop++)
		{
			if (vReceiveDataBuffer[vReceiveDataTail] == 13)
				TempLoop = vCurrentReceiveCount;  //if got an end of message character, exit so don't wipe out next message

			vReceiveDataBuffer[vReceiveDataTail] = 0;  //clear the buffer so don't confuse old ETX

			if (vReceiveDataTail < cReceiveBufferSize - 1)
				vReceiveDataTail++;
			else
				vReceiveDataTail = 0;
		}
		vCurrentReceiveCount = 0;
	}
}

void CSerialPortRFIDEASBadgeReader::ShowSerialData()  //data received
{
	if ((vLocalSystemData->vLogFile.vLogSerialData) || (vLocalSystemData->vLogFile.vShowingSerialDataOnScreen))
	{
		CString TempString = "";
		TempString = vComPortString + ":";
		BYTE TempByte = 0;
		if (vCurrentReceiveCount > 0)
		{
			//add the decimal values of the string on the end
			WORD TempIndex = vReceiveDataTail;
			for (BYTE TempLoop = 0; TempLoop < vCurrentReceiveCount; TempLoop++)
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
				vLocalSystemData->vLogFile.WriteToLogFile(TempString,cInputMessage);
		}
	}
}

