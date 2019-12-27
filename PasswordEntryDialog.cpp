//ScanTrac Side View Source File
// PasswordEntryDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "PasswordEntryDialog.h"
#include "NoticeDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern bool vGlobalClosePasswordDialog;
extern CString vGlobalPasswordToEnter;
extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
extern CScanTracSystemRunningData *vGlobalRunningData;
extern CString vGlobalBadgeReaderRFIDString;
/////////////////////////////////////////////////////////////////////////////
// CPasswordEntryDialog dialog


CPasswordEntryDialog::CPasswordEntryDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPasswordEntryDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPasswordEntryDialog)
	m_DialogTitleStaticText1 = _T("");
	m_DialogTitleStaticText2 = _T("");
	//}}AFX_DATA_INIT
	vWhiteBrush = ::CreateSolidBrush(cWhite);
	vLightBlueBrush = ::CreateSolidBrush(cLightBlue);
	vBlueBrush = ::CreateSolidBrush(cBlue);
	vGreenBrush = ::CreateSolidBrush(cGreen);
	vCursorPosition = 0;
  vEditString = "";
	vDisplayString = "";
	vMinimumPasswordLength = 0;
	vLowerPasswordRequiresThisOne = false;
	vCheckIfShouldCloseTimerHandle = 65432;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_KeyHelp);
	vLocalCWndCollection.Add(&m_KeyClear);
	vLocalCWndCollection.Add(&m_Key9);
	vLocalCWndCollection.Add(&m_Key8);
	vLocalCWndCollection.Add(&m_Key7);
	vLocalCWndCollection.Add(&m_Key6);
	vLocalCWndCollection.Add(&m_Key5);
	vLocalCWndCollection.Add(&m_Key4);
	vLocalCWndCollection.Add(&m_Key3);
	vLocalCWndCollection.Add(&m_Key2);
	vLocalCWndCollection.Add(&m_Key1);
	vLocalCWndCollection.Add(&m_Key0);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_DialogTitleStaticTextBox1);
	vLocalCWndCollection.Add(&m_DialogTitleStaticTextBox2);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText7);
	vLocalCWndCollection.Add(&m_EditStaticText);
}

CPasswordEntryDialog::~CPasswordEntryDialog()
{
	DeleteObject(vWhiteBrush);
	DeleteObject(vLightBlueBrush);
	DeleteObject(vBlueBrush);
	DeleteObject(vGreenBrush);
}

void CPasswordEntryDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);
	//vDeleteAll = true;

	vCursorPosition = vEditString.GetLength();

	CString TempText("");
	TempText.LoadString(IDS_EnterPasswordFor);
	SetDlgItemText(IDC_DialogTitleStaticText7,TempText);
	TempText.LoadString(IDS_MainMenu);
	SetDlgItemText(IDC_Function1Button,TempText);
	TempText.LoadString(IDS_PasswordComplete);
	SetDlgItemText(IDC_Function2Button,TempText);
	TempText.LoadString(IDS_Cancel);
	SetDlgItemText(IDC_Function4Button,TempText);

	//if (vGlobalShowMouseCursor)
	//{
		m_Key0.ShowWindow(SW_SHOW);
		m_Key1.ShowWindow(SW_SHOW);
		m_Key2.ShowWindow(SW_SHOW);
		m_Key3.ShowWindow(SW_SHOW);
		m_Key4.ShowWindow(SW_SHOW);
		m_Key5.ShowWindow(SW_SHOW);
		m_Key6.ShowWindow(SW_SHOW);
		m_Key7.ShowWindow(SW_SHOW);
		m_Key8.ShowWindow(SW_SHOW);
		m_Key9.ShowWindow(SW_SHOW);
		m_KeyClear.ShowWindow(SW_SHOW);
		m_KeyHelp.ShowWindow(SW_SHOW);
	//}
	SetNumLock(true);
	SetCursor();
	this->SetWindowText(_T("Password"));
	::SetupMenu(vLocalCWndCollection);

	int TempTimerResult = SetTimer(vCheckIfShouldCloseTimerHandle,500,NULL);
	if (!TempTimerResult)
		ReportErrorMessage("Error-Hold Sent Test Display Timer Failed",cEMailInspx,32000);

	m_SubFunction4Button.ShowNavigationArrow(cLeftArrowBig);
	m_SubFunction8Button.ShowNavigationArrow(cRightArrowBig);
}

void CPasswordEntryDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPasswordEntryDialog)
	DDX_Control(pDX, IDC_KeyHelp, m_KeyHelp);
	DDX_Control(pDX, IDC_KeyClear, m_KeyClear);
	DDX_Control(pDX, IDC_Key9, m_Key9);
	DDX_Control(pDX, IDC_Key8, m_Key8);
	DDX_Control(pDX, IDC_Key7, m_Key7);
	DDX_Control(pDX, IDC_Key6, m_Key6);
	DDX_Control(pDX, IDC_Key5, m_Key5);
	DDX_Control(pDX, IDC_Key4, m_Key4);
	DDX_Control(pDX, IDC_Key3, m_Key3);
	DDX_Control(pDX, IDC_Key2, m_Key2);
	DDX_Control(pDX, IDC_Key1, m_Key1);
	DDX_Control(pDX, IDC_Key0, m_Key0);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Text(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText1);
	DDX_Text(pDX, IDC_DialogTitleStaticText2, m_DialogTitleStaticText2);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticTextBox1);
	DDX_Control(pDX, IDC_DialogTitleStaticText2, m_DialogTitleStaticTextBox2);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_DialogTitleStaticText7, m_DialogTitleStaticText7);
	DDX_Control(pDX, IDC_EditStaticText, m_EditStaticText);
}


BEGIN_MESSAGE_MAP(CPasswordEntryDialog, CDialog)
	//{{AFX_MSG_MAP(CPasswordEntryDialog)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
	ON_BN_CLICKED(IDC_Function5Button, OnFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction3Button, OnSubFunction3Button)
	ON_BN_CLICKED(IDC_SubFunction4Button, OnSubFunction4Button)
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_Key0, OnKey0)
	ON_BN_CLICKED(IDC_Key1, OnKey1)
	ON_BN_CLICKED(IDC_Key2, OnKey2)
	ON_BN_CLICKED(IDC_Key3, OnKey3)
	ON_BN_CLICKED(IDC_Key4, OnKey4)
	ON_BN_CLICKED(IDC_Key5, OnKey5)
	ON_BN_CLICKED(IDC_Key6, OnKey6)
	ON_BN_CLICKED(IDC_Key7, OnKey7)
	ON_BN_CLICKED(IDC_Key8, OnKey8)
	ON_BN_CLICKED(IDC_Key9, OnKey9)
	ON_BN_CLICKED(IDC_KeyClear, OnKeyClear)
	ON_BN_CLICKED(IDC_KeyHelp, OnKeyHelp)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText7, &CPasswordEntryDialog::OnDialogtitlestatictext7)
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPasswordEntryDialog message handlers

void CPasswordEntryDialog::OnFunction1Button() 
{
	// User clicked Main Menu Button to exit without saving
	//CDialog::EndDialog(10);
}

void CPasswordEntryDialog::OnFunction2Button() 
{
	//User pressed Save and exit

	if ((vEditString.GetLength() >= vMinimumPasswordLength + 1) && 
		((!vLowerPasswordRequiresThisOne) || (vEditString.GetLength() > 1)))
	{
		BYTE TempOldCursorPosition = vEditString.Find(_T("&"));
		if (TempOldCursorPosition >= 0)
		{
			vEditString.Delete(TempOldCursorPosition,1);
		}
		CDialog::EndDialog(true);
	}
	else
	{
		if ((vLowerPasswordRequiresThisOne) && (vEditString.GetLength() <= 1))
		{
			CNoticeDialog TempNoticeDialog;
			//TempText.LoadString(IDS_32DigitsMaximum);
			TempNoticeDialog.vNoticeText = "\n\nThis Password can not be blank\nbecause a lower level password is set";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			//TempText.LoadString(IDS_32DigitsMaximum);
			TempNoticeDialog.vNoticeText = "\n\nPassword Too Short\n" + dtoa(vMinimumPasswordLength) +
				" characters minimum";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
}

void CPasswordEntryDialog::OnFunction4Button() 
{
	//Cancel was pressed
	CDialog::OnCancel();
}

void CPasswordEntryDialog::OnFunction5Button() 
{
}

void CPasswordEntryDialog::OnSubFunction3Button() 
{
}

void CPasswordEntryDialog::OnSubFunction4Button() 
{
	//Left arrow key pressed
	//if (vDeleteAll)
	//	vCursorPosition = 0;
	if (vCursorPosition > 0)
		vCursorPosition--;
	//vDeleteAll = false;
	SetCursor();
}

void CPasswordEntryDialog::OnSubFunction8Button() 
{
	//Right Arrow Button pressed
	vCursorPosition++;
	//vDeleteAll = false;
	SetCursor();
}

void CPasswordEntryDialog::SetCursor()
{
	BYTE TempOldCursorPosition = vEditString.Find(_T("&"));
	if (TempOldCursorPosition >= 0)
	{
		vEditString.Delete(TempOldCursorPosition,1);
	}
	if (vCursorPosition > vEditString.GetLength())
	{
		vCursorPosition = vEditString.GetLength();
	}
	//if (!vDeleteAll)
		vEditString.Insert(vCursorPosition, _T("&"));

	TempOldCursorPosition = vDisplayString.Find(_T("&"));
	if (TempOldCursorPosition >= 0)
	{
		vDisplayString.Delete(TempOldCursorPosition,1);
	}
	if (vCursorPosition > vDisplayString.GetLength())
	{
		vCursorPosition = vDisplayString.GetLength();
	}
	//if (!vDeleteAll)
		vDisplayString.Insert(vCursorPosition, _T("&"));
	SetDlgItemText(IDC_EditStaticText,vDisplayString);
}

BOOL CPasswordEntryDialog::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == cUpdateTitleBarMessage)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
		return true;  //true indicates it has been handled, so do not process
	}
	else
	if (pMsg->message == WM_KEYDOWN)
	{
		//get an alpha numeric key press
		if ((pMsg->wParam >=48) && (pMsg->wParam <= 57))
			pMsg->wParam = pMsg->wParam + 48;  //add 48 to change code from upper key board numbers to keypad number codes

		if ((pMsg->wParam >= c0) && (pMsg->wParam <= c9))
		{
			ProcessKey(pMsg->wParam);
		}
		//didn't get alphanumeric key press, so reset key press counter
		//check for function keys
		if (vGlobalShiftKeyDown)
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
		else
		switch (pMsg->wParam)
		{
			case 113: // F2:
				//OnSubFunction5Button();
			break;
			case 0x0d: //user hit enter, so save and exit
				OnFunction2Button();
			break;
			case 114: // F3:
				//OnSubFunction7Button();
			break;
			case 115: // F4:
				//OnSubFunction6Button();
			break;
			case 116: // F5:
				//OnSubFunction1Button();
			break;
			case 117: // F6 Key - Function 5:
				OnSubFunction4Button();
			break;
			case 118: // F7 Key - Sub Function 1:
				OnFunction1Button();
			break;
			case 119: // F8 Key - Sub Function 2:
				OnSubFunction8Button();
			break;
			case 120: // F9 Key - Sub Function 3:
				//OnSubFunction2Button();
			break;
			case 122: // F11 Key - Sub Function 4:
				OnSubFunction3Button();
			break;
			case 111: // / Key - Sub Function 5:
				OnFunction2Button();
			break;
			case 106: // * Key - Sub Function 6:
				OnFunction4Button();
			break;
			case 109: // - Key - Sub Function 7  Decimal Point:
				//OnFunction3Button();
			break;
			case 107: // + Key - Sub Function 8:
				OnFunction5Button();
			break;
			case cPageUp: // PageUp:
				OnKeyClear();
			break;
			case 16: // Shift Key
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
			break;
			case 36:  //Home key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
			case cPageDown: // Help
				//OnKeyHelp();
				OnFunction2Button(); //take this as the end of password key
			break;
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
		}
		return true;  //true indicates it has been handled, so do not process
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CPasswordEntryDialog::ProcessKey(BYTE TempKey) 
{
	if (!vGlobalShiftKeyDown)
	{
		//if hitting same key as before, back up one character
		//if (vDeleteAll)
		//{
		//	vEditString = "";
		//	vDisplayString = "";
		//	vDeleteAll = false;
		//}
		BYTE TempLength = vEditString.GetLength();
		//make sure string 10 characters
		// so have room to add one more
		CString TempText("");
		if (TempLength > 32)
		{
			CNoticeDialog TempNoticeDialog;
			TempText.LoadString(IDS_32DigitsMaximum);
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			BYTE TempIndex = 0;

			if ((TempKey >= c0) && (TempKey <= c9))
			{
				//TempIndex = pMsg->wParam - cChar0 + c0Key;
				//vEditString.Insert(vCursorPosition,(CHAR)vKey[TempIndex]);
				vEditString.Insert(vCursorPosition,(CHAR)TempKey - c0 + cChar0);
				vDisplayString.Insert(vCursorPosition,(CHAR)42);
			}
			vCursorPosition++;
			SetCursor();
			//return true if handled message an no further processing needed
		}
	}
}

HBRUSH CPasswordEntryDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText7)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText7, 8);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function2Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		return vGreenBrush;
  }

  if (pWnd->GetDlgCtrlID() == IDC_EditStaticText) 
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_EditStaticText, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		return vWhiteBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLightBlueBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticTextBox1, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLightBlueBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText2)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticTextBox2, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLightBlueBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function1Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function2Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function3Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function4Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function4Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function5Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Button) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	
	if (pWnd->GetDlgCtrlID() == IDC_KeyHelp)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_KeyHelp, 5);  //5 is medium large
	if (pWnd->GetDlgCtrlID() == IDC_KeyClear)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_KeyClear, 5);  //5 is medium large
	if ((pWnd->GetDlgCtrlID() == IDC_Key1) || (pWnd->GetDlgCtrlID() == IDC_Key2) || (pWnd->GetDlgCtrlID() == IDC_Key3)
	|| (pWnd->GetDlgCtrlID() == IDC_Key4) || (pWnd->GetDlgCtrlID() == IDC_Key5) || (pWnd->GetDlgCtrlID() == IDC_Key6)
	|| (pWnd->GetDlgCtrlID() == IDC_Key7) || (pWnd->GetDlgCtrlID() == IDC_Key8) || (pWnd->GetDlgCtrlID() == IDC_Key9)
	|| (pWnd->GetDlgCtrlID() == IDC_Key0))
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Key1, 5);  //key buttons not function buttons but is two lines
	}
	
	return hbr;
}

void CPasswordEntryDialog::OnKey0() 
{
	ProcessKey(c0);
}

void CPasswordEntryDialog::OnKey1() 
{
	ProcessKey(c1);
}

void CPasswordEntryDialog::OnKey2() 
{
	ProcessKey(c2);
}

void CPasswordEntryDialog::OnKey3() 
{
	ProcessKey(c3);
}

void CPasswordEntryDialog::OnKey4() 
{
	ProcessKey(c4);
}

void CPasswordEntryDialog::OnKey5() 
{
	ProcessKey(c5);
}

void CPasswordEntryDialog::OnKey6() 
{
	ProcessKey(c6);
}

void CPasswordEntryDialog::OnKey7() 
{
	ProcessKey(c7);
}

void CPasswordEntryDialog::OnKey8() 
{
	ProcessKey(c8);
}

void CPasswordEntryDialog::OnKey9() 
{
	ProcessKey(c9);
}

void CPasswordEntryDialog::OnKeyClear() 
{
	//if backspace character
	BYTE TempLength = vEditString.GetLength();
	if (vCursorPosition == 0)
	{
		vEditString.Delete(vCursorPosition + 1,1);
		vDisplayString.Delete(vCursorPosition + 1,1);
	}
	else
	if (vCursorPosition >= TempLength - 1)
	{
		vEditString.Delete(vCursorPosition - 1,1);
		vDisplayString.Delete(vCursorPosition - 1,1);
	}
	else
	{
		vEditString.Delete(vCursorPosition + 1,1);
		vDisplayString.Delete(vCursorPosition + 1,1);
	}
	TempLength = vDisplayString.GetLength();
	SetDlgItemText(IDC_EditStaticText,vDisplayString);
	SetCursor();

/*
	BYTE TempLength = vEditString.GetLength();
	if (vCursorPosition >= TempLength - 1)  // - 1 for the &
	{
		vEditString.Delete(vCursorPosition - 1,1);
		if ((vCursorPosition > 0) && (vCursorPosition < TempLength - 1))
			vCursorPosition--;
	}
	else
	{
		vEditString.Delete(vCursorPosition + 1,1); //vCursorPosition is at the &
		if (vCursorPosition == TempLength)
			vCursorPosition--;
	}
	SetDlgItemText(IDC_EditStaticText,vEditString);
	vDeleteAll = false;
	SetCursor();
	*/
}

void CPasswordEntryDialog::OnKeyHelp() 
{
	// Help Key Pressed
	PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 25, 0);
	//{
	//	CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText7);
	//	if (TempWindow)
	//	 TempWindow->Invalidate(false);
	//}
}


void CPasswordEntryDialog::OnDialogtitlestatictext7()
{
	//BYTE TempHiddenKeys = 0x18;
	//if (!PasswordOK(cTemporaryInspxPassword,false))
	//	TempHiddenKeys = 10;
	//PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}

void CPasswordEntryDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == vCheckIfShouldCloseTimerHandle)
	{
		if (vGlobalClosePasswordDialog)
		{
			vGlobalClosePasswordDialog = false;
			vEditString = vGlobalPasswordToEnter;
			OnFunction2Button();
		}

	}
	CDialog::OnTimer(nIDEvent);
}
