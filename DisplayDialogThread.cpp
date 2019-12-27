//ScanTrac Side View Source File
// DisplayDialogThread.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "DisplayDialogThread.h"
#include "NoticeDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDisplayDialogThread

IMPLEMENT_DYNCREATE(CDisplayDialogThread, CWinThread)
extern BYTE vGlobalShowMouseCursor;

CDisplayDialogThread::CDisplayDialogThread()
{
	vType = cNoTypeMessage;
	vMessageToDisplay = "";
	vWaitToLoadProductNoticeDialog = NULL;
	m_bAutoDelete = true;
	vDisplaySplashScreenLogo = false;
}

CDisplayDialogThread::~CDisplayDialogThread()
{
}

BOOL CDisplayDialogThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return true;
}

int CDisplayDialogThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CDisplayDialogThread, CWinThread)
	//{{AFX_MSG_MAP(CDisplayDialogThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDisplayDialogThread message handlers

int CDisplayDialogThread::Run() 
{
	// TODO: Add your specialized code here and/or call the base class

	if (!vGlobalShowMouseCursor)
		int TempDisplayCursorCounter = ShowCursor(false);

	HBITMAP hBmp = NULL;
	CString TempText("");
	if (vMessageToDisplay.GetLength())
		TempText = vMessageToDisplay;
	else
		TempText.LoadString(IDS_LoadingProductSetupPleaseWait);

	vWaitToLoadProductNoticeDialog = new CNoticeDialog;
	vWaitToLoadProductNoticeDialog->Create(IDD_NoticeDialog,(CWnd*)this);
	vWaitToLoadProductNoticeDialog->vNoticeText = TempText;
	vWaitToLoadProductNoticeDialog->vType = vType;
	vWaitToLoadProductNoticeDialog->SetDlgItemText(IDC_AnyKeyToContinueStaticText, _T(""));
	vWaitToLoadProductNoticeDialog->vCloseOnTimer = false;

	//if (vDisplaySplashScreenLogo)
	{
		struct stat buf;
		if (stat("SplashScreenLogo.png", &buf) != -1)
		{
			vWaitToLoadProductNoticeDialog->m_SplashScreenLogo.LoadPngImageFile(_T("SplashScreenLogo.png"));
			vWaitToLoadProductNoticeDialog->m_SplashScreenLogo.ShowWindow(SW_SHOW);
			vWaitToLoadProductNoticeDialog->m_SplashScreenLogo.Invalidate(true);
		}
	}

	vWaitToLoadProductNoticeDialog->vAutoClose = 250;
	vWaitToLoadProductNoticeDialog->ShowWindow(SW_SHOW);
	vWaitToLoadProductNoticeDialog->SetForegroundWindow();

	vWaitToLoadProductNoticeDialog->RunModalLoop(0);
	//vWaitToLoadProductNoticeDialog->EndModalLoop(0);
	while (!vWaitToLoadProductNoticeDialog->vCloseOnTimer)
	{
		Sleep(250);
	}

	vWaitToLoadProductNoticeDialog->EndDialog(true);

	delete vWaitToLoadProductNoticeDialog;
	vWaitToLoadProductNoticeDialog = NULL;

	if (hBmp != NULL) DeleteObject(hBmp);

	return 0;
}

void CDisplayDialogThread::ExitDisplayThread()
{
	if (vWaitToLoadProductNoticeDialog)
	{
		vWaitToLoadProductNoticeDialog->vCloseOnTimer = true;
		vWaitToLoadProductNoticeDialog->EndModalLoop(0);
	}
}

void CDisplayDialogThread::NextStep()
{
	vWaitToLoadProductNoticeDialog->vStep++;

	CString TempText = "-";
	if (vMessageToDisplay.GetLength())
		TempText = vMessageToDisplay;
	else
		TempText.LoadString(IDS_LoadingProductSetupPleaseWait);

	int TempPosition = TempText.Find(_T("Event Log"), 0);
	if (TempPosition >= 0)
	{
		TempText = TempText + " file " + dtoa(vWaitToLoadProductNoticeDialog->vStep, 0);
		vWaitToLoadProductNoticeDialog->vNoticeText = TempText;
		vWaitToLoadProductNoticeDialog->UpdateText();
	}
}

