#pragma once

//WinSock defines and setup
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "SystemConfigurationData.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512




// CModBusTCPIPInterface

class CModBusTCPIPInterface : public CWinThread
{
	DECLARE_DYNCREATE(CModBusTCPIPInterface)
public:
	bool vCloseThread;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;

protected:
	CModBusTCPIPInterface();           // protected constructor used by dynamic creation
	virtual ~CModBusTCPIPInterface();

private:
	void ProcessDataRequest();
	CString GetSocketError();

	SOCKET vListenSocket;
	SOCKET vClientSocket;
	char vReceiveBuffer[DEFAULT_BUFLEN];
	int vReceiveBufferLength = DEFAULT_BUFLEN;
	char vSendBuffer[DEFAULT_BUFLEN];
	int vSendBufferLength = DEFAULT_BUFLEN;

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	void RunTestCode();

protected:
	DECLARE_MESSAGE_MAP()
};


