//ScanTrac Side View Source File
// PingTestThread.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "PingTestThread.h"
#include "Ping.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPingTestThread
//	vGlobalPingThreadStatus = NULL;

extern BYTE vGlobalPingStatus;
extern BYTE vGlobalPingReply;
extern CString vGlobalIPDefaultGatewayString;

IMPLEMENT_DYNCREATE(CPingTestThread, CWinThread)

CPingTestThread::CPingTestThread()
{
}

CPingTestThread::~CPingTestThread()
{
}

BOOL CPingTestThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return true;
}

int CPingTestThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CPingTestThread, CWinThread)
	//{{AFX_MSG_MAP(CPingTestThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPingTestThread message handlers
// put IP address to ping in this string -- vIPAddressToPingString
// ping answer in  vIPPingGatewayReply
int CPingTestThread::Run() 
{
	vMainWindowPointer = NULL;
	if (vPingDNS)
		//vIPAddressToPingString = "66.180.96.12";
		vIPAddressToPingString = "8.8.8.8";
	else
		vIPAddressToPingString = vGlobalIPDefaultGatewayString;
	// check for ping
	if ((vIPAddressToPingString != "0.0.0.0 Not Read ...") && (vIPAddressToPingString != "") &&
			(vIPAddressToPingString != "Gateway Not Read    ") && (vIPAddressToPingString != "0.0.0.0"))	
	{
		#ifdef CPING_USE_ICMP
		{
			CPing p1;
			CPingReply pr1;
			if (p1.Ping1(vIPAddressToPingString, pr1)) //do the ping test now
			{
				vGlobalPingReply = cPingResultReply;
			}
			else
			{
				vGlobalPingReply = cPingResultNoReply;
				//sprintf(PingTempString, "Failed in call to ping, GetLastError returns: %d", GetLastError());
			}
		}
		#endif
	}	// end check ping if valid ip address
	else
	{
		// IP address not valid
		vGlobalPingReply = cPingResultNotSent;
	}
	// end ping function

	vGlobalPingStatus = cPingThreadDone;

	//return CWinThread::Run();
	//AfxEndThread(1);
	return 0;
}
