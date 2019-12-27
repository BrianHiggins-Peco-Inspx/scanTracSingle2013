//ScanTrac Side View Source File
// NoticeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "ScanTracDlg.h"
#include "NoticeDialog.h"
#include "SystemConfigurationData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CNoticeDialog, CDialog)

extern WORD vGlobalDisplaySizeH;
extern WORD vGlobalDisplaySizeV;
extern double vGlobalPercentToResizeY;
extern CScanTracSystemRunningData *vGlobalRunningData;

/////////////////////////////////////////////////////////////////////////////
// CNoticeDialog dialog
CNoticeDialog::CNoticeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNoticeDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNoticeDialog)
	//}}AFX_DATA_INIT
	vPlaceAtTopOfScreen = false;
	vSetWindowActiveTimerHandle = 9938;
	vCloseOnTimer = true;
	vCloseOnBrightnessLoopLock = false;
	vAutoClose = 0;
	vStep = 0;
	vDisplayCount = 0;
	vYellowBrush = ::CreateSolidBrush(cYellow);
	vLightYellowBrush = ::CreateSolidBrush(cLightYellow);
	vGreenBrush = ::CreateSolidBrush(cGreen);
	vLightGreenBrush = ::CreateSolidBrush(cLightGreen);
	vRedBrush = ::CreateSolidBrush(cRed);
	vLightRedBrush = ::CreateSolidBrush(cLightRed);
	vBlueBrush = ::CreateSolidBrush(cBlue);
	vLightBlueBrush = ::CreateSolidBrush(cLightBlue);
	vVeryLightBlueBrush = ::CreateSolidBrush(cVeryLightBlue);
	vButtonColorBrush = ::CreateSolidBrush(cButtonColor);
	vDisplayTimerHandle = 2345;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_InformationGraphic);
	vLocalCWndCollection.Add(&m_CheckMarkGraphic);
	vLocalCWndCollection.Add(&m_ExclaimationGraphic);
	vLocalCWndCollection.Add(&m_ErrorGraphic);
	vLocalCWndCollection.Add(&m_InspxLogo);
	vLocalCWndCollection.Add(&m_SplashScreenLogo);
	vLocalCWndCollection.Add(&m_NoticeTextButtonControl);
	vLocalCWndCollection.Add(&m_MessageTypeStaticTextControl);
	vLocalCWndCollection.Add(&m_AnyKeyToContinueStaticText);
}

CNoticeDialog::~CNoticeDialog()
{
	DeleteObject(vRedBrush);
	DeleteObject(vLightRedBrush);
	DeleteObject(vYellowBrush);
	DeleteObject(vLightYellowBrush);
	DeleteObject(vBlueBrush);
	DeleteObject(vLightBlueBrush);
	DeleteObject(vVeryLightBlueBrush);
	DeleteObject(vGreenBrush);
	DeleteObject(vLightGreenBrush);
	DeleteObject(vButtonColorBrush);
}

void CNoticeDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	SetupMenu(vLocalCWndCollection);

	// TODO: Add your message handler code here
	SetDlgItemText(IDC_NoticeTextButton,vNoticeText);
	if (vAutoClose)
		SetDlgItemText(IDC_AnyKeyToContinueStaticText, _T(""));

	this->SetWindowText(_T("Notice"));

	CString TempText("");
	if ((vAutoClose) || (vType == cVersionMessage))
		TempText = "";
	else
		TempText.LoadString(IDS_PressAnyKeyToContinue);
	SetDlgItemText(IDC_AnyKeyToContinueStaticText,TempText);
	if (vType == cVersionMessage)
	{
		m_SplashScreenLogo.LoadPngImageFile(_T("SplashScreenLogo.png"));
		m_InspxLogo.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_MessageTypeStaticText, _T(""));
		this->SetWindowText(_T("Version"));
	}
	else
	if (vType == cNoTypeMessage)
	{
		m_InspxLogo.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_AnyKeyToContinueStaticText, _T(""));
		SetDlgItemText(IDC_MessageTypeStaticText, _T(""));
	}
	else
	if (vType == cWarningMessage)
	{
		if (ResizeXCoor(1) >= 2)
		{
			vGraphicBitmap.DeleteObject();
			vGraphicBitmap.LoadBitmap(IDB_ExclaimationGraphic4xBigger);
			m_ExclaimationGraphic.SetBitmap(vGraphicBitmap);
		}
		else if (ResizeXCoor(10) >= 15)
		{
			vGraphicBitmap.DeleteObject();
			vGraphicBitmap.LoadBitmap(IDB_ExclaimationGraphic2xBigger);
			m_ExclaimationGraphic.SetBitmap(vGraphicBitmap);
		}
		m_ExclaimationGraphic.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_Warning);
		SetDlgItemText(IDC_MessageTypeStaticText,TempText);
		this->SetWindowText(TempText);
	}
	else
		
	if (vType == cSafetyMessage)
	{
		m_ErrorGraphic.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_SafetyViolation);
		SetDlgItemText(IDC_MessageTypeStaticText,TempText);
		this->SetWindowText(TempText);
	}
	else
	if (vType == cErrorMessage)
	{
		if (ResizeXCoor(1) >= 2)
		{
			vGraphicBitmap.DeleteObject();
			vGraphicBitmap.LoadBitmap(IDB_ErrorGraphic4xBigger);
			m_ErrorGraphic.SetBitmap(vGraphicBitmap);
		}
		else if (ResizeXCoor(10) >= 15)
		{
			vGraphicBitmap.DeleteObject();
			vGraphicBitmap.LoadBitmap(IDB_ErrorGraphic2xBigger);
			m_ErrorGraphic.SetBitmap(vGraphicBitmap);
		}
		m_ErrorGraphic.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_Error);
		SetDlgItemText(IDC_MessageTypeStaticText,TempText);
		this->SetWindowText(TempText);
	}
	else
	if (vType == cInformationMessage)
	{
		if (ResizeXCoor(1) >= 2)
		{
			vGraphicBitmap.DeleteObject();
			vGraphicBitmap.LoadBitmap(IDB_InformationGraphic4xBigger);
			bool TempSuccess = vGraphicBitmap.LoadBitmap(IDB_InformationGraphic4xBigger) != 0;
			TempSuccess++;
			m_InformationGraphic.SetBitmap(vGraphicBitmap);
		}
		else if (ResizeXCoor(10) >= 15)
		{
			vGraphicBitmap.DeleteObject();
			vGraphicBitmap.LoadBitmap(IDB_InformationGraphic2xBigger);
			m_InformationGraphic.SetBitmap(vGraphicBitmap);
		}
		m_InformationGraphic.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_Information);
		SetDlgItemText(IDC_MessageTypeStaticText,TempText);
		this->SetWindowText(TempText);
	}
	else
	if (vType == cInstructionsMessage)
	{
		if (ResizeXCoor(1) >= 2)
		{
			vGraphicBitmap.DeleteObject();
			vGraphicBitmap.LoadBitmap(IDB_CheckMarkGraphic4xBigger);
			m_CheckMarkGraphic.SetBitmap(vGraphicBitmap);
		}
		else if (ResizeXCoor(10) >= 15)
		{
			vGraphicBitmap.DeleteObject();
			vGraphicBitmap.LoadBitmap(IDB_CheckMarkGraphic2xBigger);
			m_CheckMarkGraphic.SetBitmap(vGraphicBitmap);
		}
		m_CheckMarkGraphic.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_Instructions);
		SetDlgItemText(IDC_MessageTypeStaticText,TempText);
		this->SetWindowText(TempText);
	}
	else
	if (vType == cNoticeMessage)
	{
		if (ResizeXCoor(1) >= 2)
		{
			vGraphicBitmap.DeleteObject();
			vGraphicBitmap.LoadBitmap(IDB_CheckMarkGraphic4xBigger);
			m_CheckMarkGraphic.SetBitmap(vGraphicBitmap);
		}
		else if (ResizeXCoor(10) >= 15)
		{
			vGraphicBitmap.DeleteObject();
			vGraphicBitmap.LoadBitmap(IDB_CheckMarkGraphic2xBigger);
			m_CheckMarkGraphic.SetBitmap(vGraphicBitmap);
		}
		m_CheckMarkGraphic.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_Notice);
		SetDlgItemText(IDC_MessageTypeStaticText,TempText);
		this->SetWindowText(TempText);
	}
	else
	//if (vType == cInformationMessage)
	{
		if (ResizeXCoor(1) >= 2)
		{
			vGraphicBitmap.DeleteObject();
			vGraphicBitmap.LoadBitmap(IDB_CheckMarkGraphic4xBigger);
			m_CheckMarkGraphic.SetBitmap(vGraphicBitmap);
		}
		else if (ResizeXCoor(10) >= 15)
		{
			vGraphicBitmap.DeleteObject();
			vGraphicBitmap.LoadBitmap(IDB_CheckMarkGraphic2xBigger);
			m_CheckMarkGraphic.SetBitmap(vGraphicBitmap);
		}
		m_CheckMarkGraphic.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_MessageTypeStaticText, _T(""));
		this->SetWindowText(_T("Information"));
	}
	if (vAutoClose)
	{
		this->SetWindowText(_T("Loading"));
		UpdateDisplay();
	}
	if (vType == cVersionMessage)
		OnPaint();

	CDialog::OnShowWindow(bShow, nStatus);

	if (vPlaceAtTopOfScreen)
		this->SetWindowPos(NULL,ResizeXCoor(75),ResizeYCoor(5),ResizeXCoor(490),ResizeYCoor(215),SWP_NOZORDER);

	int TimerResult = SetTimer(vSetWindowActiveTimerHandle,500,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-Startup Timer Failed",cEMailInspx,32000);
}

void CNoticeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNoticeDialog)
	DDX_Control(pDX, IDC_InformationGraphic, m_InformationGraphic);
	DDX_Control(pDX, IDC_CheckMarkGraphic, m_CheckMarkGraphic);
	DDX_Control(pDX, IDC_ExclaimationGraphic, m_ExclaimationGraphic);
	DDX_Control(pDX, IDC_ErrorGraphic, m_ErrorGraphic);
	DDX_Control(pDX, IDC_InspxLogo, m_InspxLogo);
	DDX_Control(pDX, IDC_SplashScreenLogo, m_SplashScreenLogo);
	DDX_Control(pDX, IDC_NoticeTextButton, m_NoticeTextButtonControl);
	DDX_Control(pDX, IDC_MessageTypeStaticText, m_MessageTypeStaticTextControl);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_AnyKeyToContinueStaticText, m_AnyKeyToContinueStaticText);
}


BEGIN_MESSAGE_MAP(CNoticeDialog, CDialog)
	//{{AFX_MSG_MAP(CNoticeDialog)
	ON_WM_LBUTTONUP()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_NoticeTextButton, OnNoticeTextButton)
	ON_WM_KEYUP()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNoticeDialog message handlers

void CNoticeDialog::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	//CDialog::OnLButtonUp(nFlags, point);
	//if ((vType == cVersionMessage) || (!vAutoClose))
	//	CDialog::EndDialog(true);

}

void CNoticeDialog::OnNoticeTextButton() 
{
	// TODO: Add your control notification handler code here
	if ((vType == cVersionMessage) || (!vAutoClose))
		CDialog::EndDialog(true);
}

void CNoticeDialog::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	//CDialog::OnKeyUp(nChar, nRepCnt,nFlags);
	//if ((vType == cVersionMessage) || (!vAutoClose))
	//	CDialog::EndDialog(true);
}

BOOL CNoticeDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == 27) // Escape
			::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
		else
			OnNoticeTextButton();
		return true;  //true indicates it has been handled, so do not process
	}
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		OnNoticeTextButton();
		return true;  //true indicates it has been handled, so do not process
	}
	if (pMsg->message == cDensityLoopLockedClearMessage)
	if (vCloseOnBrightnessLoopLock)
	{
		//EndModalLoop(0);
		EndDialog(true);
	}

	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CNoticeDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		// TODO: Change any attributes of the DC here
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	pDC->SetBkMode(TRANSPARENT);
	if (pWnd->GetDlgCtrlID() == IDC_NoticeTextButton)
	{
		SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_NoticeTextButtonControl, 5);
		if ((vType == cErrorMessage) || (vType == cSafetyMessage))
			return vRedBrush;
		else
		if ((vType == cWarningMessage) || (vType == cStartupInfo))
			return vYellowBrush;
		else
		if ((vType == cNoTypeMessage) || (vType == cVersionMessage))
			return vLightGreenBrush;
		else
			return vLightBlueBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_AnyKeyToContinueStaticText)
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_AnyKeyToContinueStaticText, 5);
	if (pWnd->GetDlgCtrlID() == IDC_MessageTypeStaticText)
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_MessageTypeStaticTextControl, 6);
	return hbr;
}

void CNoticeDialog::UpdateDisplay() 
{
	if (vCloseOnTimer)
	{
		int TempTimerResult = SetTimer(vDisplayTimerHandle,vAutoClose,NULL);
		if (!TempTimerResult)
			ReportErrorMessage("Error-Display Timer Failed",cEMailInspx,32000);
	}
	else
	{
		int TempTimerResult = SetTimer(vDisplayTimerHandle,1000,NULL);
		if (!TempTimerResult)
			ReportErrorMessage("Error-Display Timer Failed",cEMailInspx,32000);
	}
}

void CNoticeDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CNoticeDialog::NextStep() 
{
	vStep++;
	m_MessageTypeStaticTextControl.Invalidate();
}

void CNoticeDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == vSetWindowActiveTimerHandle)
	{
		KillTimer(vSetWindowActiveTimerHandle);

 		//this->ShowWindow(SW_SHOW);
		this->SetForegroundWindow();
		this->BringWindowToTop();
		this->SetActiveWindow();
		CWnd *TempWindow = this->SetFocus();
		//test code 2 lines below
		//if (TempWindow != this)
		//	ReportErrorMessage("CNotice did not have focus", cAction, 0);
	}
	else
	if (nIDEvent == vDisplayTimerHandle)
  {
		if (vCloseOnTimer)
		{
			KillTimer(vDisplayTimerHandle);
			//EndModalLoop(0);
			EndDialog(true);
		}
		else
		{
			wchar_t TempChars[100];
			vDisplayCount++;
			if (vDisplayCount > 30)
			{
				vDisplayCount = 0;
				SetDlgItemText(IDC_AnyKeyToContinueStaticText, _T("."));
			}
			GetDlgItemText(IDC_AnyKeyToContinueStaticText, TempChars, 100);
			CString TempText(TempChars);
			TempText = TempText + ".";
			SetDlgItemText(IDC_AnyKeyToContinueStaticText,TempText);
			/*//br htest
			switch (vStep)
			{
				case 1:
					TempText = TempText + "1";
				break;
				case 2:
					TempText = TempText + "2";
				break;
				case 3:
					TempText = TempText + "3";
				break;
				case 4:
					TempText = TempText + "4";
				break;
				case 5:
					TempText = TempText + "5";
				break;
				case 6:
					TempText = TempText + "6";
				break;
				case 7:
					TempText = TempText + "7";
				break;
				case 8:
					TempText = TempText + "8";
				break;
				default:
					TempText = TempText + ".";
			}
			*/
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CNoticeDialog::SetupMenu(tCWndCollection& TempCWndCollection)
{
	RECT TempRectCoor;
	int TempCenterX=320; 
	int TempCenterY=240;
	int TempHalfHeight = 0;
	int TempHalfWidth = 0;
	int TempUpperCornerX = 300;
	int TempUpperCornerY = 200;
	int TempOffsetFromUpperCornerX = 0;
	int TempOffsetFromUpperCornerY = 0;
		
	for (WORD TempLoop = 0; TempLoop < TempCWndCollection.GetCount(); TempLoop++)
	{
		TempCWndCollection[TempLoop]->GetWindowRect(&TempRectCoor);
		TempHalfWidth = (TempRectCoor.right-TempRectCoor.left)/2;
		TempHalfHeight = (TempRectCoor.bottom-TempRectCoor.top)/2;
		if ( TempLoop == 0 ) //if it is 'this'  //calculate center
		{
			TempCenterX = TempRectCoor.right - TempHalfWidth; 
			TempCenterY = TempRectCoor.bottom - TempHalfHeight;

			TempHalfWidth = ResizeXCoor(TempHalfWidth);
			TempHalfHeight = ResizeYCoor(TempHalfHeight);
		
			TempCWndCollection[TempLoop]->SetWindowPos(NULL,TempCenterX - TempHalfWidth,
					TempCenterY - TempHalfHeight,
					TempHalfWidth*2,
					TempHalfHeight*2, NULL);

			TempUpperCornerX = TempCenterX - TempHalfWidth;
			TempUpperCornerY = TempCenterY - TempHalfHeight;
		}
		else
		{
			TempOffsetFromUpperCornerX = TempRectCoor.left - TempUpperCornerX;  //center of screen - center of TempCWnd
			TempOffsetFromUpperCornerY = TempRectCoor.top - TempUpperCornerY;	
			TempOffsetFromUpperCornerX = ResizeXCoor(TempOffsetFromUpperCornerX);
			TempOffsetFromUpperCornerY = ResizeYCoor(TempOffsetFromUpperCornerY);

			TempHalfWidth = ResizeXCoor(TempHalfWidth);
			TempHalfHeight = ResizeYCoor(TempHalfHeight);

			TempCWndCollection[TempLoop]->SetWindowPos(NULL,TempOffsetFromUpperCornerX,
					TempOffsetFromUpperCornerY,
					TempHalfWidth*2,
					TempHalfHeight*2, NULL);
		}
	}
}

void CNoticeDialog::UpdateText()
{
	SetDlgItemText(IDC_NoticeTextButton, vNoticeText);
}
