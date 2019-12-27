//ScanTrac Side View Source File
// ListeningSocket.cpp: implementation of the CListeningSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "scantrac.h"
#include "ListeningSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListeningSocket::CListeningSocket(CScanTracDlg *TempMainWindowPointer)
{
	vPointerToMainWindow = TempMainWindowPointer;
}

CListeningSocket::~CListeningSocket()
{

}

void CListeningSocket::OnAccept(int nErrorCode)
{
	CSocket::OnAccept(nErrorCode);
	vPointerToMainWindow->ProcessPendingAccept();
}

#ifdef _DEBUG
void CListeningSocket::AssertValid() const
{
	CSocket::AssertValid();
}

void CListeningSocket::Dump(CDumpContext& TempDumpContext) const
{
	CSocket::Dump(TempDumpContext);
}
#endif //_DEBUG

IMPLEMENT_DYNAMIC(CListeningSocket, CSocket)
