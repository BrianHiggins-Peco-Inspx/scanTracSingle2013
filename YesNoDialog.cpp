//ScanTrac Side View Source File
// YesNoDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "YesNoDialog.h"
#include "NoticeDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
IMPLEMENT_DYNCREATE(CYesNoDialog, CDialog)

extern BYTE vGlobalShiftKeyDown;
extern CScanTracSystemRunningData *vGlobalRunningData;
/////////////////////////////////////////////////////////////////////////////
// CYesNoDialog dialog


CYesNoDialog::CYesNoDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CYesNoDialog::IDD, pParent)
{
	vPlaceAtTopOfScreen = false;
	//{{AFX_DATA_INIT(CYesNoDialog)
	//}}AFX_DATA_INIT
	vLightBlueBrush = ::CreateSolidBrush(cLightBlue);
	vVeryLightBlueBrush = ::CreateSolidBrush(cVeryLightBlue);
	vBlueBrush = ::CreateSolidBrush(cBlue);
	vBrilliantBlueBrush = ::CreateSolidBrush(cBrilliantBlue);
	vLightBrilliantBlueBrush = ::CreateSolidBrush(cLightBrilliantBlue);
	vLightYellowBrush = ::CreateSolidBrush(cLightYellow);
	vYesButtonText = "";
	vNoButtonText = "";
	vGrabFocusTimerHandle = 62134;
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_NoButtonControl);
	vLocalCWndCollection.Add(&m_YesButtonControl);
	vLocalCWndCollection.Add(&m_NoticeTextButtonControl);
	vLocalCWndCollection.Add(&m_MessageTypeStaticTextControl);
	vLocalCWndCollection.Add(&m_CheckMarkGraphic);
}

CYesNoDialog::~CYesNoDialog()
{
	DeleteObject(vLightBlueBrush);
	DeleteObject(vVeryLightBlueBrush);
	DeleteObject(vBlueBrush);
	DeleteObject(vBrilliantBlueBrush);
	DeleteObject(vLightBrilliantBlueBrush);
	DeleteObject(vLightYellowBrush);
}

void CYesNoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYesNoDialog)
	DDX_Control(pDX, IDC_NoButton, m_NoButtonControl);
	DDX_Control(pDX, IDC_YesButton, m_YesButtonControl);
	DDX_Control(pDX, IDC_NoticeTextButton, m_NoticeTextButtonControl);
	DDX_Control(pDX, IDC_MessageTypeStaticText, m_MessageTypeStaticTextControl);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CheckMarkGraphic, m_CheckMarkGraphic);
}


BEGIN_MESSAGE_MAP(CYesNoDialog, CDialog)
	//{{AFX_MSG_MAP(CYesNoDialog)
	ON_BN_CLICKED(IDC_NoButton, OnNoButton)
	ON_BN_CLICKED(IDC_YesButton, OnYesButton)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYesNoDialog message handlers

void CYesNoDialog::OnNoButton() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CYesNoDialog::OnYesButton() 
{
	// TODO: Add your control notification handler code here
	CDialog::EndDialog(true);
	
}

void CYesNoDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	CString TempText("");
	TempText.LoadString(IDS_Yes);
	SetDlgItemText(IDC_YesButton,TempText);
	TempText.LoadString(IDS_No);
	SetDlgItemText(IDC_NoButton,TempText);

	if ((vQuestionType == cConfirmQuestion) || (vQuestionType == cConfirmAbandon) || (vQuestionType == cConfirmSave))
	{
		TempText.LoadString(IDS_Confirm);
		SetDlgItemText(IDC_MessageTypeStaticText,TempText);
		if (vQuestionType == cConfirmAbandon)
		{
			TempText.LoadString(IDS_Abandon);
			SetDlgItemText(IDC_YesButton,TempText);
		}
		else
		if (vQuestionType == cConfirmSave)
		{
			TempText.LoadString(IDS_KeepChanges);
			SetDlgItemText(IDC_YesButton,TempText);
			m_YesButtonControl.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		}
	}
	else
	if (vQuestionType == cWarningQuestion)
		SetDlgItemText(IDC_MessageTypeStaticText, _T("Warning"));
	else
	//if (vQuestionType == cConfirmQuestion)
	{
		TempText.LoadString(IDS_Question);
		SetDlgItemText(IDC_MessageTypeStaticText,TempText);
	}
	SetDlgItemText(IDC_NoticeTextButton,vNoticeText);
	if (vYesButtonText != "")
	{
		SetDlgItemText(IDC_YesButton,vYesButtonText);
		if (vYesButtonText.GetLength() > 11)
			m_YesButtonControl.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		else
			m_YesButtonControl.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
	}
	if (vNoButtonText != "")
	{
		SetDlgItemText(IDC_NoButton,vNoButtonText);
		if (vNoButtonText.GetLength() > 11)
			m_NoButtonControl.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		else
			m_NoButtonControl.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
	}

	CDialog::OnShowWindow(bShow, nStatus);

	if (vPlaceAtTopOfScreen)
		this->SetWindowPos(NULL,75,5,490,219,SWP_NOZORDER);

	this->SetWindowText(_T("Yes No"));
	int TimerResult = SetTimer(vGrabFocusTimerHandle,1000,NULL);  //WAS 100
	if (!TimerResult)
		ReportErrorMessage("Error-GrabFocus Timer Failed",cEMailInspx, 32000);

	this->SetupMenu(vLocalCWndCollection);

	if (ResizeXCoor(1) >= 2)
	{
		vGraphicBitmap.DeleteObject();
		vGraphicBitmap.LoadBitmap(IDB_QuestionGraphic4xBigger);
		m_CheckMarkGraphic.SetBitmap(vGraphicBitmap);
	}
	else if (ResizeXCoor(10) >= 15)
	{
		vGraphicBitmap.DeleteObject();
		vGraphicBitmap.LoadBitmap(IDB_QuestionGraphic2xBigger);
		m_CheckMarkGraphic.SetBitmap(vGraphicBitmap);
	}

}

BOOL CYesNoDialog::PreTranslateMessage(MSG* pMsg) 
{
	CWnd * TempWindow = NULL;
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		int TempKey = pMsg->wParam;
		if (vGlobalShiftKeyDown)
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
		else
		switch (TempKey)
		{
			case 111: // / Key - Sub Function 5:
				OnYesButton();
			break;
			case 106: // * Key - Sub Function 6:
				OnNoButton();
			break;
			case 16: // Shift Key
			case 0x60:  //0 key on KeyPad
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
			break;
			case 36:  //Home key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
			case cPageDown: // Help
				ShowNoHelpAvailable();
				//PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 26, 0);
				//{
				//	TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText);
				//	if (TempWindow)
				//		TempWindow->Invalidate(false);
				//}
			break;
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
		}
		return true;  //true indicates it has been handled, so do not process
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CYesNoDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	pDC->SetTextColor(cBlack);
	pDC->SetBkMode(TRANSPARENT);
	if (pWnd->GetDlgCtrlID() == IDC_MessageTypeStaticText)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MessageTypeStaticTextControl, 6);
	}

  if (pWnd->GetDlgCtrlID() == IDC_NoticeTextButton)
	{
		TextSize = SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_NoticeTextButtonControl, 5);
		if (vQuestionType == cWarningQuestion)
  			return vLightYellowBrush;
		else
  			return vLightBlueBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_YesButton)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_YesButtonControl, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}


	if (pWnd->GetDlgCtrlID() == IDC_NoButton)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_NoButtonControl, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	
	return hbr;
}


void CYesNoDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnTimer(nIDEvent);
  if (nIDEvent == vGrabFocusTimerHandle)  
  {
		//KillTimer(vGrabFocusTimerHandle);
		SetForegroundWindow();
	}
}

void CYesNoDialog::SetupMenu(tCWndCollection& TempCWndCollection)
{
	RECT TempRectCoor;
	int TempCenterX=320; 
	int TempBottomY=480;
	int TempHeight = 0;
	int TempHalfWidth = 0;
	int TempUpperCornerX = 300;
	int TempUpperCornerY = 200;
	int TempOffsetFromUpperCornerX = 0;
	int TempOffsetFromUpperCornerY = 0;
		
	for (WORD TempLoop = 0; TempLoop < TempCWndCollection.GetCount(); TempLoop++)
	{
		TempCWndCollection[TempLoop]->GetWindowRect(&TempRectCoor);
		TempHalfWidth = (TempRectCoor.right-TempRectCoor.left)/2;
		TempHeight = (TempRectCoor.bottom-TempRectCoor.top);
		if ( TempLoop == 0 ) //if it is 'this'  //calculate center
		{
			TempCenterX = vGlobalRunningData->vGlobalMonitorSizeX/2; 
			TempBottomY = vGlobalRunningData->vGlobalMonitorSizeY;

			TempHalfWidth = ResizeXCoor(TempHalfWidth);
			TempHeight = ResizeYCoor(TempHeight);
		
			TempCWndCollection[TempLoop]->SetWindowPos(NULL,TempCenterX - TempHalfWidth,
					TempBottomY - TempHeight,
					TempHalfWidth*2,
					TempHeight, NULL);

			TempUpperCornerX = TempCenterX - TempHalfWidth;
			TempUpperCornerY = TempBottomY - TempHeight;
		}
		
		
		else
		{
			TempOffsetFromUpperCornerX = TempRectCoor.left - TempUpperCornerX;  //center of screen - center of TempCWnd
			TempOffsetFromUpperCornerY = TempRectCoor.top - TempUpperCornerY;	
			TempOffsetFromUpperCornerX = ResizeXCoor(TempOffsetFromUpperCornerX);
			TempOffsetFromUpperCornerY = ResizeYCoor(TempOffsetFromUpperCornerY);

			TempHalfWidth = ResizeXCoor(TempHalfWidth);
			TempHeight = ResizeYCoor(TempHeight);

			TempCWndCollection[TempLoop]->SetWindowPos(NULL,TempOffsetFromUpperCornerX,
					TempOffsetFromUpperCornerY,
					TempHalfWidth*2,
					TempHeight, NULL);
		}
		
	}
/*	RECT TempRectCoor;
	int TempUpperLeftX, TempUpperLeftY, TempLength, TempHeight;
	BYTE TempNumberOfObjectsInCollection = TempCWndCollection.GetSize();
	for (WORD TempLoop = 0; TempLoop < TempNumberOfObjectsInCollection; TempLoop++)
	{
		if (TempLoop == 0) //this
		{
			TempCWndCollection[TempLoop]->GetWindowRect(&TempRectCoor);
			LONG TempCenterLocationX = (vGlobalRunningData->vGlobalMonitorSizeX / 2) - ((TempRectCoor.right - TempRectCoor.left)/2);
			Long TempBottomLocationY = vGlobalRunningData->vGlobalMonitorSizeY - (TempRectCoor.bottom - TempRectCoor.top);
			TempCWndCollection[TempLoop]->SetWindowPos(NULL,TempCenterLocationX, TempBottomLocationY
							,0,0,SWP_NOSIZE);
		}
		TempCWndCollection[TempLoop]->GetWindowRect(&TempRectCoor);
		TempUpperLeftX = (TempRectCoor.left)*(vGlobalRunningData->vGlobalMonitorSizeX)/cOldResolutionX;
		TempUpperLeftY = (TempRectCoor.top)*(vGlobalRunningData->vGlobalMonitorSizeY)/cOldResolutionY;
		TempLength = (TempRectCoor.right-TempRectCoor.left)*(vGlobalRunningData->vGlobalMonitorSizeX)/cOldResolutionX;
		TempHeight = (TempRectCoor.bottom-TempRectCoor.top)*(vGlobalRunningData->vGlobalMonitorSizeY)/cOldResolutionY;
		TempCWndCollection[TempLoop]->SetWindowPos(NULL,TempUpperLeftX, TempUpperLeftY, TempLength, TempHeight,
																						SWP_NOACTIVATE | SWP_NOZORDER);
	}
	*/
}

void CYesNoDialog::ShowNoHelpAvailable()
{
	CNoticeDialog TempNoticeDialog;
	TempNoticeDialog.vNoticeText = "\n\n\nHelp not available in this window.";
	TempNoticeDialog.vType = cErrorMessage;
	TempNoticeDialog.vAutoClose = 3000;
	TempNoticeDialog.DoModal();
}