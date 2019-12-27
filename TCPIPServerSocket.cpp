//Side View ScanTrac, ScanTrac Source File
// TCPIPServerSocket.cpp: implementation of the CTCPIPServerSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "scantrac.h"
#include "TCPIPServerSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTCPIPServerSocket::CTCPIPServerSocket(CScanTracDlg *TempMainWindowPointer)
{
	vMainWindowPointer = TempMainWindowPointer;
	vNumberOfSocketMessageToSend = 0;
	vSocketFileForArchive = NULL;
	vSocketArchiveIn = NULL;
	vSocketArchiveOut = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CTCPIPServerSocket Operations, methods

void CTCPIPServerSocket::Init()
{
	vSocketFileForArchive = new CSocketFile(this);
	vSocketArchiveIn = new CArchive(vSocketFileForArchive,CArchive::load);
	vSocketArchiveOut = new CArchive(vSocketFileForArchive,CArchive::store);
}

void CTCPIPServerSocket::Abort()
{
	if (vSocketArchiveOut != NULL)
	{
		vSocketArchiveOut->Abort();
		delete vSocketArchiveOut;
		vSocketArchiveOut = NULL;
	}
}

void CTCPIPServerSocket::SendSocketMessage(CSocketMessage* TempSocketMessage)
{
	if (vSocketArchiveOut != NULL)
	{
		TempSocketMessage->Serialize(*vSocketArchiveOut);
		vSocketArchiveOut->Flush();
	}
}

void CTCPIPServerSocket::ReceiveMsg(CSocketMessage* TempSocketMessage)
{
	TempSocketMessage->Serialize(*vSocketArchiveIn);
}

/////////////////////////////////////////////////////////////////////////////
// CTCPIPServerSocket Overridable callbacks

void CTCPIPServerSocket::OnReceive(int nErrorCode)
{
	CSocket::OnReceive(nErrorCode);

	vMainWindowPointer->ProcessPendingRead(this);
}

/////////////////////////////////////////////////////////////////////////////
// CSocket Implementation

CTCPIPServerSocket::~CTCPIPServerSocket()
{
	if (vSocketArchiveOut != NULL)
		delete vSocketArchiveOut;

	if (vSocketArchiveIn != NULL)
		delete vSocketArchiveIn;

	if (vSocketFileForArchive != NULL)
		delete vSocketFileForArchive;
}

#ifdef _DEBUG
void CTCPIPServerSocket::AssertValid() const
{
	CSocket::AssertValid();
}

void CTCPIPServerSocket::Dump(CDumpContext& TempDumpContext) const
{
	CSocket::Dump(TempDumpContext);
}
#endif //_DEBUG

IMPLEMENT_DYNAMIC(CTCPIPServerSocket, CSocket)
