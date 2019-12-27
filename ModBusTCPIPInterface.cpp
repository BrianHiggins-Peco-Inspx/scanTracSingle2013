// ModBusTCPIPInterface.cpp : implementation file
//
#include <vector> //for test code


#include "stdafx.h"
#include "ScanTrac.h"
#include "ModBusTCPIPInterface.h"


// CModBusTCPIPInterface

IMPLEMENT_DYNCREATE(CModBusTCPIPInterface, CWinThread)
#define DEFAULT_PORT 502

extern CProduct* vGlobalCurrentProduct;

CModBusTCPIPInterface::CModBusTCPIPInterface()
{
}

CModBusTCPIPInterface::~CModBusTCPIPInterface()
{
}

BOOL CModBusTCPIPInterface::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	vCloseThread = false;
	return TRUE;
}

int CModBusTCPIPInterface::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CModBusTCPIPInterface, CWinThread)
END_MESSAGE_MAP()


// CModBusTCPIPInterface message handlers


int CModBusTCPIPInterface::Run()
{
	vLocalSystemData->vModBusClientConnectionCounts = 0;
	vLocalSystemData->vModBusClientRequestCounts = 0;

	while (!vCloseThread)
	{
		bool TempHadAnError = false;
		WSADATA TempwsaData;
		int iResult = 0;

		vListenSocket = INVALID_SOCKET;
		vClientSocket = INVALID_SOCKET;

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &TempwsaData);
		if (iResult != 0)
		{
			if (iResult == WSASYSNOTREADY)
				ReportErrorMessage("WinSock ModBus TCP/IP Interface WSA Startup failed becuase network not ready", cError, 0);
			else
			if (iResult == WSAVERNOTSUPPORTED)
				ReportErrorMessage("WinSock ModBus TCP/IP Interface WSA Startup failed becuase socket version not supported", cError, 0);
			else
			if (iResult == WSAEPROCLIM)
				ReportErrorMessage("WinSock ModBus TCP/IP Interface WSA Startup failed becuase hit limit on socket tasks", cError, 0);
			else
			if (iResult == WSAEFAULT)
				ReportErrorMessage("WinSock ModBus TCP/IP Interface WSA Startup failed becuase WSA data pointer invalid", cError, 0);
			else
			{
				//printf("WSAStartup failed with error: %d\n", iResult);
				CString TempString = "WinSock ModBus TCP/IP Interface WSA Startup failed with error: " + GetSocketError();
				ReportErrorMessage(TempString, cError, 0);
			}
			//return 1;
			TempHadAnError = true;
		}
		else
		{
			ReportErrorMessage("ModBus TCP/IP initialized version: " + dtoa((BYTE)TempwsaData.wVersion, 0) + "." + dtoa(TempwsaData.wVersion >> 8, 0), cAction, 0);
			//ReportErrorMessage("ModBus TCP/IP initialized High version: " + dtoa((BYTE)TempwsaData.wHighVersion, 0) + "." + dtoa(TempwsaData.wHighVersion >> 8, 0), cError, 0);
			//ReportErrorMessage("ModBus TCP/IP initialized Max Sockets: " + dtoa(TempwsaData.iMaxSockets, 0), cError, 0);
			//ReportErrorMessage("ModBus TCP/IP initialized iMaxUdpDgs: " + dtoa(TempwsaData.iMaxUdpDg, 0), cError, 0);
			////CString TempString(*TempwsaData.lpVendorInfo);
			////ReportErrorMessage("ModBus TCP/IP initialized Vendor info: " + TempString, cError, 0);
			//CString TempString = TempwsaData.szDescription;
			//ReportErrorMessage("ModBus TCP/IP initialized Description: " + TempString, cError, 0);
			//TempString = TempwsaData.szSystemStatus;
			//ReportErrorMessage("ModBus TCP/IP initialized System Status: " + TempString, cError, 0);

			sockaddr_in TempIPAddressForSocket; // = GetLANSocketIPAddressIn();
			TempIPAddressForSocket.sin_addr.s_addr = INADDR_ANY; //Wild card IP address
			TempIPAddressForSocket.sin_family = AF_INET; //Address family
			TempIPAddressForSocket.sin_port = htons((u_short)DEFAULT_PORT); //port to use

			vListenSocket = socket(AF_INET, SOCK_STREAM, 0);
			if (vListenSocket == INVALID_SOCKET)
			{
				CString TempString = "WinSock ModBus TCP/IP socket failed with error: " + GetSocketError();
				ReportErrorMessage(TempString, cError, 0);
				WSACleanup();
				//return 1;
				TempHadAnError = true;
			}
			else
			{
				iResult = bind(vListenSocket, (sockaddr*)&TempIPAddressForSocket, sizeof(TempIPAddressForSocket));
				if (iResult == SOCKET_ERROR)
				{
					DWORD TempErrorCode = WSAGetLastError();
					CString TempString = "WinSock ModBus TCP/IP bind failed with error: " + GetSocketError();
					ReportErrorMessage(TempString, cError, 0);
					closesocket(vListenSocket);
					WSACleanup();
					//return 1;
					TempHadAnError = true;
				}
				else
				{
					vLocalSystemData->vModBusTCPIPAddress = "Socket: " + dtoa(vListenSocket);


					iResult = listen(vListenSocket, SOMAXCONN); //listen to see if a client wants to connect
					if (iResult == SOCKET_ERROR)
					{
						CString TempString = "WinSock ModBus TCP/IP listen failed with error: " + GetSocketError();
						ReportErrorMessage(TempString, cError, 0);
						closesocket(vListenSocket);
						WSACleanup();
						//return 1;
						TempHadAnError = true;
					}
					else
					{
						ReportErrorMessage("ModBus TCP/IP Listening Socket Opened", cAction, 0);
						vLocalSystemData->vModBusSocketOpen = true;

						// Shield does not care about vLocalSystemData->vModBusTCPIPAddress now.
						//vLocalSystemData->vModBusTCPIPAddress = dtoa((BYTE)TempIPAddressForSocket.sa_data[2], 0) + "." + dtoa((BYTE)TempIPAddressForSocket.sa_data[3], 0) + "." +
						//	dtoa((BYTE)TempIPAddressForSocket.sa_data[4], 0) + "." + dtoa((BYTE)TempIPAddressForSocket.sa_data[5], 0);

						// Accept a client socket
						vClientSocket = accept(vListenSocket, NULL, NULL); //continues after client connect
						if (vClientSocket == INVALID_SOCKET)
						{
							CString TempString = "WinSock ModBus TCP/IP accept failed with error: " + GetSocketError();
							ReportErrorMessage(TempString, cError, 0);
							closesocket(vListenSocket);
							WSACleanup();
							vLocalSystemData->vModBusSocketOpen = false;
							//return 1;
							TempHadAnError = true;
						}
						else
						{
							ReportErrorMessage("ModBus Client Connected", cAction, 0);
							vLocalSystemData->vModBusClientConnected = true;
							vLocalSystemData->vModBusClientConnectionCounts++;

							// No longer need server socket out there listening for a client to log in since only need 1 client log in at a time
							closesocket(vListenSocket);

							ProcessDataRequest();

							// shutdown the connection since we're done
							iResult = shutdown(vClientSocket, SD_SEND);
							vLocalSystemData->vModBusClientConnected = false;

							if (vLocalSystemData->vModBusClientConnectionCounts)
								vLocalSystemData->vModBusClientConnectionCounts--;

							if (iResult == SOCKET_ERROR)
							{
								//printf("shutdown failed with error: %d\n", WSAGetLastError());
								CString TempString = "WinSock ModBus TCP/IP shutdown failed with error: " + GetSocketError();
								ReportErrorMessage(TempString, cEMailInspx, 32000);
							}
							closesocket(vClientSocket);
							WSACleanup();
							vLocalSystemData->vModBusSocketOpen = false;
						}
					}
				}
			}
			ReportErrorMessage("ModBus Client Closed Connection", cAction, 0);
			if (TempHadAnError)
			{
				Sleep(250);
				vLocalSystemData->vModBusTCPIPErrorCounter++;
			}
		}
	}
	ReportErrorMessage("Error-ModBus TCP/IP-Must restart Shield software", cError, 0);
	vLocalSystemData->vModBusTCPIPAddress = "Socket Not Open";
	vLocalSystemData->vModBusSocketOpen = false;

	return 0;
}

void CModBusTCPIPInterface::ProcessDataRequest()
{
	// Receive until the peer shuts down the connection
	//while ((!vCloseThread) && (iResult > 0))
	int iResult = 1;
	int iSendResult;

	while (iResult > 0)
	{
		iResult = recv(vClientSocket, vReceiveBuffer, vReceiveBufferLength, 0);
		if (iResult > 0)
		{
			vLocalSystemData->vModBusClientRequestCounts++;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempLogText = "ModBus<Length=" + dtoa(iResult, 0) + "<";
				for (WORD TempLoop = 0; TempLoop < iResult; TempLoop++)
				{
					if (TempLoop)
						TempLogText = TempLogText + ":" + ByteToHex(vReceiveBuffer[TempLoop]);
					else
						TempLogText = TempLogText + ":" + ByteToHex(vReceiveBuffer[TempLoop]);
				}

				//	vLocalSystemData->vLogFile.WriteToLogFile(TempLogText, cDebugMessage);
				ReportErrorMessage(TempLogText, cWriteToLog, 0);
			}


			vSendBufferLength = 0;
			BYTE TempDataRequested = vReceiveBuffer[0];
			if (iResult > 9)
				TempDataRequested = vReceiveBuffer[9];

			//if ((TempDataRequested < 1) || (TempDataRequested > 8) || (TempDataRequested == 3) || (TempDataRequested == 5) || (TempDataRequested == 7))
			//	TempDataRequested = 1;

			BYTE TempNumberOfWordLengthRegistersToRead = vReceiveBuffer[11];
			if ((TempNumberOfWordLengthRegistersToRead < 2) || (TempNumberOfWordLengthRegistersToRead > 512))
				TempNumberOfWordLengthRegistersToRead = 2;

			//code up replies here
			vSendBuffer[vSendBufferLength++] = vReceiveBuffer[0]; //transaction #
			vSendBuffer[vSendBufferLength++] = vReceiveBuffer[1]; //transaction #
			vSendBuffer[vSendBufferLength++] = vReceiveBuffer[2]; //protocol ID = 0 = ModBus
			vSendBuffer[vSendBufferLength++] = vReceiveBuffer[3]; //protocol ID = 0 = ModBus
			vSendBuffer[vSendBufferLength++] = vReceiveBuffer[4]; //MSB of Length
			BYTE TempLengthIndex = vSendBufferLength;
			vSendBuffer[vSendBufferLength++] = 3; //length of data in the reply is the 3 bytes following this, then all the data requested below
			vSendBuffer[vSendBufferLength++] = vReceiveBuffer[6]; //unit ID = 0
			vSendBuffer[vSendBufferLength++] = vReceiveBuffer[7]; //ModBus Function Code = 3 for Read Multiple Registers
			//vSendBuffer[vSendBufferLength++] = vReceiveBuffer[8]; //MSB of Offset into register group
			//vSendBuffer[vSendBufferLength++] = vReceiveBuffer[9];	//LSB of Offset into register group
			//vSendBuffer[vSendBufferLength++] = vReceiveBuffer[10]; //MSB of number of Registers to read
			//vSendBuffer[vSendBufferLength++] = vReceiveBuffer[11]; //LSB of number of Registers to read
			BYTE TempLengthIndex1 = vSendBufferLength;
			vSendBuffer[vSendBufferLength++] = 0;//TempNumberOfWordLengthRegistersToRead * 2; // set the length of the reply data to the LSB of number of Registers to read * 2 because 2 bytes per word

			if (TempDataRequested <= 1) //request for Total Counts
			{
				vSendBuffer[TempLengthIndex] = vSendBuffer[TempLengthIndex] + 4; //Length in words
				vSendBuffer[TempLengthIndex1] = vSendBuffer[TempLengthIndex1] + 4; //Length in bytes

				vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vTotalContainers >> 8);
				vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vTotalContainers);
				vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vTotalContainers >> 24);
				vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vTotalContainers >> 16);
			}

			//if ((TempDataRequested == 3) || ((TempDataRequested == 1) && (TempNumberOfWordLengthRegistersToRead >= 4))) //request for Total Rejects
			if ((TempDataRequested <= 3) && (TempDataRequested + TempNumberOfWordLengthRegistersToRead >= 4)) //request for Total Rejects //address 3
			{
				vSendBuffer[TempLengthIndex] = vSendBuffer[TempLengthIndex] + 4;
				vSendBuffer[TempLengthIndex1] = vSendBuffer[TempLengthIndex1] + 4;

				vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vRejectCountTotal >> 8);
				vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vRejectCountTotal);
				vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vRejectCountTotal >> 24);
				vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vRejectCountTotal >> 16);
			}

			//if ((TempDataRequested == 5) || ((TempDataRequested == 1) && (TempNumberOfWordLengthRegistersToRead >= 6)) || ((TempDataRequested == 3) && (TempNumberOfWordLengthRegistersToRead >= 4))) //request for Total Ejects
			if ((TempDataRequested <= 5) && (TempDataRequested + TempNumberOfWordLengthRegistersToRead >= 6)) //request for Total Ejects //address 5
			{
				vSendBuffer[TempLengthIndex] = vSendBuffer[TempLengthIndex] + 4;
				vSendBuffer[TempLengthIndex1] = vSendBuffer[TempLengthIndex1] + 4;

				vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vEjectCountTotal >> 8);
				vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vEjectCountTotal);
				vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vEjectCountTotal >> 24);
				vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vEjectCountTotal >> 16);
			}
			//if TempDataRequested + TempNumberOfWordLengthRegistersToRead = read this buffer
			//	1 + 8 = 9
			//	3 + 6 = 9
			//	5 + 4 = 9
			//	7 + 2 = 9
			if ((TempDataRequested <= 7) && (TempDataRequested + TempNumberOfWordLengthRegistersToRead >= 8)) //request for Total Good //address 7
			{
				vSendBuffer[TempLengthIndex] = vSendBuffer[TempLengthIndex] + 4;
				vSendBuffer[TempLengthIndex1] = vSendBuffer[TempLengthIndex1] + 4;

				vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vGoodCountTotal >> 8);
				vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vGoodCountTotal);
				vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vGoodCountTotal >> 24);
				vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vGoodCountTotal >> 16);
			}
			//new on 2019 07 30 add inspection counts
			//36 inspections+shape handlings possible times 3 values per, gives 108 values + 4 main counters times 4 bytes per counter = 448. 
			if ((TempDataRequested <= 448) && (TempDataRequested + TempNumberOfWordLengthRegistersToRead > 10)) //request for any Inspection data // Address 9 to 448  
			for (BYTE TempInspectionLoop = 0; TempInspectionLoop < cMaximumNumberOfCountsDisplayed ; TempInspectionLoop++) //cMaximumNumberOfInspections
			{
				if ((TempDataRequested <= (9 + (TempInspectionLoop * 6))) && (TempDataRequested + TempNumberOfWordLengthRegistersToRead >= 10 + (TempInspectionLoop * 6))) //request for Inspection 1, Ejects // address 9
				if (vSendBufferLength < 500)
				{
					vSendBuffer[TempLengthIndex] = vSendBuffer[TempLengthIndex] + 4;
					vSendBuffer[TempLengthIndex1] = vSendBuffer[TempLengthIndex1] + 4;

					//vLocalSystemData->vTotalNumberOfInspections includes all inspections plus shape handling counters like slipped, Aux Detectors, and overlapped
					if (vLocalSystemData->vTotalNumberOfInspections > (TempInspectionLoop)) // vGlobalCurrentProduct->vNumberOfInspections
					{ //send back the number that were ejected for this inspection/item
						vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vXRayCriteriaCount[TempInspectionLoop] >> 8);
						vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vXRayCriteriaCount[TempInspectionLoop]);
						vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vXRayCriteriaCount[TempInspectionLoop] >> 24);
						vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vXRayCriteriaCount[TempInspectionLoop] >> 16);
					}
					else
					{
						long TempAnswer = -1;
						vSendBuffer[vSendBufferLength++] = (BYTE)(TempAnswer >> 8);
						vSendBuffer[vSendBufferLength++] = (BYTE)(TempAnswer);
						vSendBuffer[vSendBufferLength++] = (BYTE)(TempAnswer >> 24);
						vSendBuffer[vSendBufferLength++] = (BYTE)(TempAnswer >> 16);
					}
				}

				if ((TempDataRequested <= (11 + (TempInspectionLoop * 6))) && (TempDataRequested + TempNumberOfWordLengthRegistersToRead >= 12 + (TempInspectionLoop * 6))) //request for Inspection 1, Ejects // address 9
				if (vSendBufferLength < 500)
				{ //send back the number that should have been ejected, but the ejectors were disabled for this inspection/item
					vSendBuffer[TempLengthIndex] = vSendBuffer[TempLengthIndex] + 4;
					vSendBuffer[TempLengthIndex1] = vSendBuffer[TempLengthIndex1] + 4;

					if (vLocalSystemData->vTotalNumberOfInspections > (TempInspectionLoop))
					{
						vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vXRayCriteriaCountButDisabled[TempInspectionLoop] >> 8);
						vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vXRayCriteriaCountButDisabled[TempInspectionLoop]);
						vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vXRayCriteriaCountButDisabled[TempInspectionLoop] >> 24);
						vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vXRayCriteriaCountButDisabled[TempInspectionLoop] >> 16);
					}
					else
					{
						long TempAnswer = -1;
						vSendBuffer[vSendBufferLength++] = (BYTE)(TempAnswer >> 8);
						vSendBuffer[vSendBufferLength++] = (BYTE)(TempAnswer);
						vSendBuffer[vSendBufferLength++] = (BYTE)(TempAnswer >> 24);
						vSendBuffer[vSendBufferLength++] = (BYTE)(TempAnswer >> 16);
					}
				}

				if (vSendBufferLength < 500)
				if (TempDataRequested + TempNumberOfWordLengthRegistersToRead >= 4 + (TempInspectionLoop * 3)) //if request goes up this high then continue to process, otherwise exit inspection loop
				{//send back the Threshold of this inspection
					if ((TempDataRequested <= (13 + (TempInspectionLoop * 6))) && (TempDataRequested + TempNumberOfWordLengthRegistersToRead >= 14 + (TempInspectionLoop * 6))) //request for Inspection 1, Ejects // address 9
					{//send back the Threshold of this inspection
						vSendBuffer[TempLengthIndex] = vSendBuffer[TempLengthIndex] + 4;
						vSendBuffer[TempLengthIndex1] = vSendBuffer[TempLengthIndex1] + 4;

						if ((vGlobalCurrentProduct->vNumberOfInspections > (TempInspectionLoop)) && (vGlobalCurrentProduct->vInspection[TempInspectionLoop]))
						{
							vSendBuffer[vSendBufferLength++] = vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vThreshold;
							vSendBuffer[vSendBufferLength++] = (BYTE)vLocalSystemData->vThresholdSensitivities[TempInspectionLoop];
							vSendBuffer[vSendBufferLength++] = (BYTE)vLocalSystemData->vLearnSensitivities[TempInspectionLoop];
							vSendBuffer[vSendBufferLength++] = (BYTE)vLocalSystemData->vSensitivities[TempInspectionLoop];
						}
						else
						{
							long TempAnswer = -1;
							vSendBuffer[vSendBufferLength++] = (BYTE)(TempAnswer >> 8);
							vSendBuffer[vSendBufferLength++] = (BYTE)(TempAnswer);
							vSendBuffer[vSendBufferLength++] = (BYTE)(TempAnswer >> 24);
							vSendBuffer[vSendBufferLength++] = (BYTE)(TempAnswer >> 16);
						}
					}
				}
				else
					TempInspectionLoop = cMaximumNumberOfCountsDisplayed; //
			}

				//else
				//if ((iResult >= 1) && (TempDataRequested == 5)) //request for Status
				//{
				//	WORD TempConveyorSpeed = vLocalSystemData->vBeltRate / 12;
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vSystemRunMode);
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalConfigurationData->vEnableEjectors);
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(TempConveyorSpeed);
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(TempConveyorSpeed >> 8);
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vContainerRate);
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vContainerRate >> 8);
				//}
				//else //command 5
				//if ((iResult >= 1) && (TempDataRequested == 6)) //request for Current Product Name
				//{
				//	BYTE TempLength = vLocalSystemData->vCurrentProductName.GetLength();
				//	CString TempName = vLocalSystemData->vCurrentProductName;
				//	if (TempLength == 0)
				//	{
				//		TempName = "No Current Product";
				//		TempLength = TempName.GetLength();
				//	}
				//	for (BYTE TempLoop = 0; TempLoop < TempLength; TempLoop++)
				//		vSendBuffer[vSendBufferLength++] = (char)(TempName.GetAt(TempLoop));
				//}
				//else //command 7
				//{
				//	WORD TempConveyorSpeed = vLocalSystemData->vBeltRate / 12;
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vTotalContainers);
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vTotalContainers >> 8);
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vTotalContainers >> 16);
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vTotalContainers >> 24);

				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vRejectCountTotal);
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vRejectCountTotal >> 8);
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vRejectCountTotal >> 16);
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vRejectCountTotal >> 24);

				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vEjectCountTotal);
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vEjectCountTotal >> 8);
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vEjectCountTotal >> 16);
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vEjectCountTotal >> 24);

				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vGoodCountTotal);
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vGoodCountTotal >> 8);
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vGoodCountTotal >> 16);
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vGoodCountTotal >> 24);

				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vSystemRunMode);
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalConfigurationData->vEnableEjectors);
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(TempConveyorSpeed);
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(TempConveyorSpeed >> 8);
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vContainerRate);
				//	vSendBuffer[vSendBufferLength++] = (BYTE)(vLocalSystemData->vContainerRate >> 8);

				//	BYTE TempLength = vLocalSystemData->vCurrentProductName.GetLength();
				//	CString TempName = vLocalSystemData->vCurrentProductName;
				//	if (TempLength == 0)
				//	{
				//		TempName = "No Current Product";
				//		TempLength = TempName.GetLength();
				//	}
				//	for (BYTE TempLoop = 0; TempLoop < TempLength; TempLoop++)
				//		vSendBuffer[vSendBufferLength++] = (char)(TempName.GetAt(TempLoop));
				//}
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempLogText = "ModBus>";
				for (WORD TempLoop = 0; TempLoop < vSendBufferLength; TempLoop++)
					if (TempLoop)
						TempLogText = TempLogText + ":" + ByteToHex(vSendBuffer[TempLoop]);
					else
						TempLogText = TempLogText + ":" + ByteToHex(vSendBuffer[TempLoop]);

				//vLocalSystemData->vLogFile.WriteToLogFile(TempLogText, cDebugMessage);
				ReportErrorMessage(TempLogText, cWriteToLog, 0);
			}

			// Echo the buffer back to the sender
			iSendResult = send(vClientSocket, vSendBuffer, vSendBufferLength, 0);
			if (iSendResult == SOCKET_ERROR)
			{
				CString TempString = "WinSock ModBus TCP/IP send failed with error: " + dtoa(WSAGetLastError(), 0);
				ReportErrorMessage(TempString, cEMailInspx, 32000);
				iResult = 0; //exit the loop reading from this client since it closed, which will close the connection and start listening for a new connection
			}
		}
		else
			if (iResult == 0)
				ReportErrorMessage("WinSock ModBus TCP/IP Connection closing", cWriteToLog, 0); // for debugging, can comment out once working and verified
			else
			{
				int TempErrorCode = WSAGetLastError();
				if (TempErrorCode == 10054)
					ReportErrorMessage("ModBus TCP/IP Client closed connection unexpectantly", cEMailInspx, 32000);
				else
				{
					CString TempString = "WinSock ModBus TCP/IP recv failed with error: " + dtoa(TempErrorCode, 0);
					ReportErrorMessage(TempString, cEMailInspx, 32000);
				}
				iResult = 0; //exit the loop reading from this client since it closed, which will close the connection and start listening for a new connection
			}
		Sleep(50);
	}
}

void CModBusTCPIPInterface::RunTestCode()
{
	//    //-----------------------------------------
	//    // Declare and initialize variables
	//    WSADATA wsaData;
	//    int iResult;
	//    INT iRetval;
	//
	//    DWORD dwRetval;
	//
	//    int i = 1;
	//    
	//    struct addrinfo *result = NULL;
	//    struct addrinfo *ptr = NULL;
	//    struct addrinfo hints;
	//
	//    struct sockaddr_in  *sockaddr_ipv4;
	////    struct sockaddr_in6 *sockaddr_ipv6;
	//    LPSOCKADDR sockaddr_ip;
	//
	//    LPTSTR ipstringbuffer;
	//    DWORD ipbufferlength = 46;
	//		int argc;
	//		char argv[50];
	//
	//		argc = 11;
	//		argv[0] = 'p';
	//
	//    // Validate the parameters
	//    if (argc != 3) {
	//        printf("usage: %s <hostname> <servicename>\n", argv[0]);
	//        printf("       provides protocol-independent translation\n");
	//        printf("       from an ANSI host name to an IP address\n");
	//        printf("%s example usage\n", argv[0]);
	//        printf("   %s www.contoso.com 0\n", argv[0]);
	//       // return 1;
	//    }
	//
	//    // Initialize Winsock
	//    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	//    if (iResult != 0) {
	//        printf("WSAStartup failed: %d\n", iResult);
	//        //return 1;
	//    }
	//
	//    //--------------------------------
	//    // Setup the hints address info structure
	//    // which is passed to the getaddrinfo() function
	//    ZeroMemory( &hints, sizeof(hints) );
	//    hints.ai_family = AF_UNSPEC;
	//    hints.ai_socktype = SOCK_STREAM;
	//    hints.ai_protocol = IPPROTO_TCP;
	//
	//    printf("Calling getaddrinfo with following parameters:\n");
	//    printf("\tnodename = %s\n", argv[1]);
	//    printf("\tservname (or port) = %s\n\n", argv[2]);
	//    
	////--------------------------------
	//// Call getaddrinfo(). If the call succeeds,
	//// the result variable will hold a linked list
	//// of addrinfo structures containing response
	//// information
	//    dwRetval = getaddrinfo(&argv[1], &argv[2], &hints, &result);
	//    if ( dwRetval != 0 ) {
	//        printf("getaddrinfo failed with error: %d\n", dwRetval);
	//        WSACleanup();
	//       // return 1;
	//    }
	//
	//    printf("getaddrinfo returned success\n");
	//    
	//    // Retrieve each address and print out the hex bytes
	//    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {
	//
	//        printf("getaddrinfo response %d\n", i++);
	//        printf("\tFlags: 0x%x\n", ptr->ai_flags);
	//        printf("\tFamily: ");
	//        switch (ptr->ai_family) {
	//            case AF_UNSPEC:
	//                printf("Unspecified\n");
	//                break;
	//            case AF_INET:
	//                printf("AF_INET (IPv4)\n");
	//                sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
	//                printf("\tIPv4 address %s\n",
	//                    inet_ntoa(sockaddr_ipv4->sin_addr) );
	//                break;
	//            case AF_INET6:
	//                printf("AF_INET6 (IPv6)\n");
	//                // the InetNtop function is available on Windows Vista and later
	//                // sockaddr_ipv6 = (struct sockaddr_in6 *) ptr->ai_addr;
	//                // printf("\tIPv6 address %s\n",
	//                //    InetNtop(AF_INET6, &sockaddr_ipv6->sin6_addr, ipstringbuffer, 46) );
	//                
	//                // We use WSAAddressToString since it is supported on Windows XP and later
	//                sockaddr_ip = (LPSOCKADDR) ptr->ai_addr;
	//                // The buffer length is changed by each call to WSAAddresstoString
	//                // So we need to set it for each iteration through the loop for safety
	//                ipbufferlength = 46;
	//                iRetval = WSAAddressToString(sockaddr_ip, (DWORD) ptr->ai_addrlen, NULL, 
	//                    ipstringbuffer, &ipbufferlength );
	//                if (iRetval)
	//                    printf("WSAAddressToString failed with %u\n", WSAGetLastError() );
	//                else    
	//                    printf("\tIPv6 address %s\n", ipstringbuffer);
	//                break;
	//            case AF_NETBIOS:
	//                printf("AF_NETBIOS (NetBIOS)\n");
	//                break;
	//            default:
	//                printf("Other %ld\n", ptr->ai_family);
	//                break;
	//        }
	//        printf("\tSocket type: ");
	//        switch (ptr->ai_socktype) {
	//            case 0:
	//                printf("Unspecified\n");
	//                break;
	//            case SOCK_STREAM:
	//                printf("SOCK_STREAM (stream)\n");
	//                break;
	//            case SOCK_DGRAM:
	//                printf("SOCK_DGRAM (datagram) \n");
	//                break;
	//            case SOCK_RAW:
	//                printf("SOCK_RAW (raw) \n");
	//                break;
	//            case SOCK_RDM:
	//                printf("SOCK_RDM (reliable message datagram)\n");
	//                break;
	//            case SOCK_SEQPACKET:
	//                printf("SOCK_SEQPACKET (pseudo-stream packet)\n");
	//                break;
	//            default:
	//                printf("Other %ld\n", ptr->ai_socktype);
	//                break;
	//        }
	//        printf("\tProtocol: ");
	//        switch (ptr->ai_protocol) {
	//            case 0:
	//                printf("Unspecified\n");
	//                break;
	//            case IPPROTO_TCP:
	//                printf("IPPROTO_TCP (TCP)\n");
	//                break;
	//            case IPPROTO_UDP:
	//                printf("IPPROTO_UDP (UDP) \n");
	//                break;
	//            default:
	//                printf("Other %ld\n", ptr->ai_protocol);
	//                break;
	//        }
	//        printf("\tLength of this sockaddr: %d\n", ptr->ai_addrlen);
	//        printf("\tCanonical name: %s\n", ptr->ai_canonname);
	//    }
	//
	//    freeaddrinfo(result);
	//    WSACleanup();
}


CString CModBusTCPIPInterface::GetSocketError()
{// Localized error lookup for all winsock error codes.
 // From : https://msdn.microsoft.com/en-us/library/windows/desktop/ms740668(v=vs.85).aspx#WSAECONNRESET
 // The following list describes the possible error codes returned by the WSAGetLastError function.
 // Errors are listed in numerical order with the error macro name.

	CString reason("");
	int error = WSAGetLastError();
	switch (error)
	{
	case WSA_INVALID_HANDLE: //6 
		reason = "Specified event object handle is invalid.";
		break;
	case WSA_NOT_ENOUGH_MEMORY: //8 
		reason = "Insufficient memory available.";
		break;
	case WSA_INVALID_PARAMETER: //87 
		reason = "One or more parameters are invalid.";
		break;
	case WSA_OPERATION_ABORTED: //995 
		reason = "Overlapped operation aborted.";
		break;
	case WSA_IO_INCOMPLETE: //996 
		reason = "Overlapped I/O event object not in signaled state.";
		break;
	case WSA_IO_PENDING: //997 
		reason = "Overlapped operations will complete later.";
		break;
	case WSAEINTR: //10004 
		reason = "Interrupted function call.";
		break;
	case WSAEBADF: //10009 
		reason = "File handle is not valid.";
		break;
	case WSAEACCES: //10013 
		reason = "Permission denied.";
		break;
	case WSAEFAULT: //10014 
		reason = "Bad address.";
		break;
	case WSAEINVAL: //10022 
		reason = "Invalid argument.";
		break;
	case WSAEMFILE: //10024 
		reason = "Too many open files.";
		break;
	case WSAEWOULDBLOCK: //10035 
		reason = "Resource temporarily unavailable.";
		break;
	case WSAEINPROGRESS: //10036 
		reason = "Operation now in progress.";
		break;
	case WSAEALREADY: //10037 
		reason = "Operation already in progress.";
		break;
	case WSAENOTSOCK: //10038 
		reason = "Socket operation on nonsocket.";
		break;
	case WSAEDESTADDRREQ: //10039 
		reason = "Destination address required.";
		break;
	case WSAEMSGSIZE: //10040 
		reason = "Message too long.";
		break;
	case WSAEPROTOTYPE: //10041 
		reason = "Protocol wrong type for socket.";
		break;
	case WSAENOPROTOOPT: //10042 
		reason = "Bad protocol option.";
		break;
	case WSAEPROTONOSUPPORT: //10043 
		reason = "Protocol not supported.";
		break;
	case WSAESOCKTNOSUPPORT: //10044 
		reason = "Socket type not supported.";
		break;
	case WSAEOPNOTSUPP: //10045 
		reason = "Operation not supported.";
		break;
	case WSAEPFNOSUPPORT: //10046 
		reason = "Protocol family not supported.";
		break;
	case WSAEAFNOSUPPORT: //10047 
		reason = "Address family not supported by protocol family.";
		break;
	case WSAEADDRINUSE: //10048 
		reason = "Address already in use.";
		break;
	case WSAEADDRNOTAVAIL: //10049 
		reason = "Cannot assign requested address.";
		break;
	case WSAENETDOWN: //10050 
		reason = "Network is down.";
		break;
	case WSAENETUNREACH: //10051 
		reason = "Network is unreachable.";
		break;
	case WSAENETRESET: //10052 
		reason = "Network dropped connection on reset.";
		break;
	case WSAECONNABORTED: //10053 
		reason = "Software caused connection abort.";
		break;
	case WSAECONNRESET: //10054 
		reason = "Connection reset by peer.";
		break;
	case WSAENOBUFS: //10055 
		reason = "No buffer space available.";
		break;
	case WSAEISCONN: //10056 
		reason = "Socket is already connected.";
		break;
	case WSAENOTCONN: //10057 
		reason = "Socket is not connected.";
		break;
	case WSAESHUTDOWN: //10058 
		reason = "Cannot send after socket shutdown.";
		break;
	case WSAETOOMANYREFS: //10059 
		reason = "Too many references.";
		break;
	case WSAETIMEDOUT: //10060 
		reason = "Connection timed out.";
		break;
	case WSAECONNREFUSED: //10061 
		reason = "Connection refused.";
		break;
	case WSAELOOP: //10062 
		reason = "Cannot translate name.";
		break;
	case WSAENAMETOOLONG: //10063 
		reason = "Name too long.";
		break;
	case WSAEHOSTDOWN: //10064 
		reason = "Host is down.";
		break;
	case WSAEHOSTUNREACH: //10065 
		reason = "No route to host.";
		break;
	case WSAENOTEMPTY: //10066 
		reason = "Directory not empty.";
		break;
	case WSAEPROCLIM: //10067 
		reason = "Too many processes.";
		break;
	case WSAEUSERS: //10068 
		reason = "User quota exceeded.";
		break;
	case WSAEDQUOT: //10069 
		reason = "Disk quota exceeded.";
		break;
	case WSAESTALE: //10070 
		reason = "Stale file handle reference.";
		break;
	case WSAEREMOTE: //10071 
		reason = "Item is remote.";
		break;
	case WSASYSNOTREADY: //10091 
		reason = "Network subsystem is unavailable.";
		break;
	case WSAVERNOTSUPPORTED: //10092 
		reason = "Winsock.dll version out of range.";
		break;
	case WSANOTINITIALISED: //10093 
		reason = "Successful WSAStartup not yet performed.";
		break;
	case WSAEDISCON: //10101 
		reason = "Graceful shutdown in progress.";
		break;
	case WSAENOMORE: //10102 
		reason = "No more results.";
		break;
	case WSAECANCELLED: //10103 
		reason = "Call has been canceled.";
		break;
	case WSAEINVALIDPROCTABLE: //10104 
		reason = "Procedure call table is invalid.";
		break;
	case WSAEINVALIDPROVIDER: //10105 
		reason = "Service provider is invalid.";
		break;
	case WSAEPROVIDERFAILEDINIT: //10106 
		reason = "Service provider failed to initialize.";
		break;
	case WSASYSCALLFAILURE: //10107 
		reason = "System call failure.";
		break;
	case WSASERVICE_NOT_FOUND: //10108 
		reason = "Service not found.";
		break;
	case WSATYPE_NOT_FOUND: //10109 
		reason = "Class type not found.";
		break;
	case WSA_E_NO_MORE: //10110 
		reason = "No more results.";
		break;
	case WSA_E_CANCELLED: //10111 
		reason = "Call was canceled.";
		break;
	case WSAEREFUSED: //10112 
		reason = "Database query was refused.";
		break;
	case WSAHOST_NOT_FOUND: //11001 
		reason = "Host not found.";
		break;
	case WSATRY_AGAIN: //11002 
		reason = "Nonauthoritative host not found.";
		break;
	case WSANO_RECOVERY: //11003 
		reason = "This is a nonrecoverable error.";
		break;
	case WSANO_DATA: //11004 
		reason = "Valid name, no data record of requested type.";
		break;
	case WSA_QOS_RECEIVERS: //11005 
		reason = "QoS receivers.";
		break;
	case WSA_QOS_SENDERS: //11006 
		reason = "QoS senders.";
		break;
	case WSA_QOS_NO_SENDERS: //11007 
		reason = "No QoS senders.";
		break;
	case WSA_QOS_NO_RECEIVERS: //11008 
		reason = "QoS no receivers.";
		break;
	case WSA_QOS_REQUEST_CONFIRMED: //11009
		reason = "QoS request confirmed.";
		break;
	case WSA_QOS_ADMISSION_FAILURE: //11010 
		reason = "QoS admission error.";
		break;
	case WSA_QOS_POLICY_FAILURE: //11011 
		reason = "QoS policy failure.";
		break;
	case WSA_QOS_BAD_STYLE: //11012 
		reason = "QoS bad style.";
		break;
	case WSA_QOS_BAD_OBJECT: //11013 
		reason = "QoS bad object.";
		break;
	case WSA_QOS_TRAFFIC_CTRL_ERROR: //11014 
		reason = "QoS traffic control error.";
		break;
	case WSA_QOS_GENERIC_ERROR: //11015 
		reason = "QoS generic error.";
		break;
	case WSA_QOS_ESERVICETYPE: //11016 
		reason = "QoS service type error.";
		break;
	case WSA_QOS_EFLOWSPEC: //11017 
		reason = "QoS flowspec error.";
		break;
	case WSA_QOS_EPROVSPECBUF: //11018 
		reason = "Invalid QoS provider buffer.";
		break;
	case WSA_QOS_EFILTERSTYLE: //11019 
		reason = "Invalid QoS filter style.";
		break;
	case WSA_QOS_EFILTERTYPE: //11020 
		reason = "Invalid QoS filter type.";
		break;
	case WSA_QOS_EFILTERCOUNT: //11021 
		reason = "Incorrect QoS filter count.";
		break;
	case WSA_QOS_EOBJLENGTH: //11022 
		reason = "Invalid QoS object length.";
		break;
	case WSA_QOS_EFLOWCOUNT: //11023 
		reason = "Incorrect QoS flow count.";
		break;
	case WSA_QOS_EUNKOWNPSOBJ: //11024 
		reason = "Unrecognized QoS object.";
		break;
	case WSA_QOS_EPOLICYOBJ: //11025 
		reason = "Invalid QoS policy object.";
		break;
	case WSA_QOS_EFLOWDESC: //11026 
		reason = "Invalid QoS flow descriptor.";
		break;
	case WSA_QOS_EPSFLOWSPEC: //11027 
		reason = "Invalid QoS provider-specific flowspec.";
		break;
	case WSA_QOS_EPSFILTERSPEC: //11028 
		reason = "Invalid QoS provider-specific filterspec.";
		break;
	case WSA_QOS_ESDMODEOBJ: //11029 
		reason = "Invalid QoS shape discard mode object.";
		break;
	case WSA_QOS_ESHAPERATEOBJ: //11030 
		reason = "Invalid QoS shaping rate object.";
		break;
	case WSA_QOS_RESERVED_PETYPE: //11031 
		reason = "Reserved policy QoS element type.";
		break;
	default:
		reason = "Unkown error.";
		break;
	}
	// Message to be sent to Inspx log.
	CString cause;
	cause.Format(_T("Server failed . Winsock error reason: %s\n"), reason);
	return cause;
}