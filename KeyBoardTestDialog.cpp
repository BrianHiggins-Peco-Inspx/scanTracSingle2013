//ScanTrac Side View Source File
// KeyBoardTestDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "KeyBoardTestDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
/////////////////////////////////////////////////////////////////////////////
// CKeyBoardTestDialog dialog


CKeyBoardTestDialog::CKeyBoardTestDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyBoardTestDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyBoardTestDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	vGreenLightTimerHandle = 31353;
	vCurrentKeyIDD = IDC_NoWindow;
	vShowRed = false;
	vShowMultiple = false;
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_LeftRight);
	vLocalCWndCollection.Add(&m_HiddenKey);
	vLocalCWndCollection.Add(&m_KeyHelp);
	vLocalCWndCollection.Add(&m_KeyClear);
	vLocalCWndCollection.Add(&m_UnusedKey);
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
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_ScreenCapture);
	vLocalCWndCollection.Add(&m_NoWindow);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
}

void CKeyBoardTestDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);
	SetNumLock(true);
	
	this->SetWindowText(_T("Keypad Test"));
	::SetupMenu(vLocalCWndCollection);
}

void CKeyBoardTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyBoardTestDialog)
	DDX_Control(pDX, IDC_LeftRight, m_LeftRight);
	DDX_Control(pDX, IDC_ScreenCapture, m_ScreenCapture);
	DDX_Control(pDX, IDC_HiddenKey, m_HiddenKey);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Key1, m_Key1);
	DDX_Control(pDX, IDC_UnusedKey, m_UnusedKey);
	DDX_Control(pDX, IDC_Key2, m_Key2);
	DDX_Control(pDX, IDC_Key3, m_Key3);
	DDX_Control(pDX, IDC_Key4, m_Key4);
	DDX_Control(pDX, IDC_Key5, m_Key5);
	DDX_Control(pDX, IDC_Key6, m_Key6);
	DDX_Control(pDX, IDC_Key7, m_Key7);
	DDX_Control(pDX, IDC_Key8, m_Key8);
	DDX_Control(pDX, IDC_Key9, m_Key9);
	DDX_Control(pDX, IDC_Key0, m_Key0);
	DDX_Control(pDX, IDC_KeyClear, m_KeyClear);
	DDX_Control(pDX, IDC_KeyHelp, m_KeyHelp);
	DDX_Control(pDX, IDC_NoWindow, m_NoWindow);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
}


BEGIN_MESSAGE_MAP(CKeyBoardTestDialog, CDialog)
	//{{AFX_MSG_MAP(CKeyBoardTestDialog)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
	ON_BN_CLICKED(IDC_Function5Button, OnFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction1Button, OnSubFunction1Button)
	ON_BN_CLICKED(IDC_SubFunction2Button, OnSubFunction2Button)
	ON_BN_CLICKED(IDC_SubFunction3Button, OnSubFunction3Button)
	ON_BN_CLICKED(IDC_SubFunction4Button, OnSubFunction4Button)
	ON_BN_CLICKED(IDC_SubFunction5Button, OnSubFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction6Button, OnSubFunction6Button)
	ON_BN_CLICKED(IDC_SubFunction7Button, OnSubFunction7Button)
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_Key1, OnKey1)
	ON_BN_CLICKED(IDC_Key2, OnKey2)
	ON_BN_CLICKED(IDC_Key3, OnKey3)
	ON_BN_CLICKED(IDC_Key4, OnKey4)
	ON_BN_CLICKED(IDC_Key5, OnKey5)
	ON_BN_CLICKED(IDC_Key6, OnKey6)
	ON_BN_CLICKED(IDC_Key7, OnKey7)
	ON_BN_CLICKED(IDC_Key8, OnKey8)
	ON_BN_CLICKED(IDC_Key9, OnKey9)
	ON_BN_CLICKED(IDC_Key0, OnKey0)
	ON_BN_CLICKED(IDC_KeyClear, OnKeyClear)
	ON_BN_CLICKED(IDC_KeyHelp, OnKeyHelp)
	ON_WM_CHAR()
	ON_BN_CLICKED(IDC_ScreenCapture, OnScreenCapture)
	ON_BN_CLICKED(IDC_LeftRight, OnLeftRight)
	ON_BN_CLICKED(IDC_HiddenKey, OnHiddenKey)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_UnusedKey, &CKeyBoardTestDialog::OnUnused)
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CKeyBoardTestDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyBoardTestDialog message handlers

void CKeyBoardTestDialog::OnFunction1Button() 
{
	// Function 1 Button Pressed
	TurnKeyGreen(IDC_Function1Button);
}

void CKeyBoardTestDialog::OnFunction2Button() 
{
	// Function 2 Button Pressed
	TurnKeyGreen(IDC_Function2Button);

	// Save and Exit Button was pressed
	//CDialog::EndDialog(true);
}

void CKeyBoardTestDialog::OnFunction3Button() 
{
	// Function 3 Button Pressed
	TurnKeyGreen(IDC_Function3Button);
}

void CKeyBoardTestDialog::OnFunction4Button() 
{
	// Function 4 Button Pressed
	TurnKeyGreen(IDC_Function4Button);
}

void CKeyBoardTestDialog::OnFunction5Button() 
{
	// Function 5 Button Pressed
	TurnKeyGreen(IDC_Function5Button);
}

void CKeyBoardTestDialog::OnSubFunction1Button() 
{
	// Sub Function 1 Button Pressed
	TurnKeyGreen(IDC_SubFunction1Button);
}

void CKeyBoardTestDialog::OnSubFunction2Button() 
{
	// Sub Function 2 Button Pressed
	TurnKeyGreen(IDC_SubFunction2Button);
}

void CKeyBoardTestDialog::OnSubFunction3Button() 
{
	// Sub Function 3 Button Pressed
	TurnKeyGreen(IDC_SubFunction3Button);
}

void CKeyBoardTestDialog::OnSubFunction4Button() 
{
	// Sub Function 4 Button Pressed
	TurnKeyGreen(IDC_SubFunction4Button);
}

void CKeyBoardTestDialog::OnSubFunction5Button() 
{
	// Sub Function 5 Button Pressed
	TurnKeyGreen(IDC_SubFunction5Button);
}

void CKeyBoardTestDialog::OnSubFunction6Button() 
{
	// Sub Function 6 Button Pressed
	TurnKeyGreen(IDC_SubFunction6Button);
}

void CKeyBoardTestDialog::OnSubFunction7Button() 
{
	// Sub Function 7 Button Pressed
	TurnKeyGreen(IDC_SubFunction7Button);
}

void CKeyBoardTestDialog::OnSubFunction8Button() 
{
	// Sub Function 8 Button Pressed
	TurnKeyGreen(IDC_SubFunction8Button);
}

BOOL CKeyBoardTestDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
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
		/*
		BYTE TestByte = ((BYTE)(pMsg->lParam >> 24)) & 1;
		if (!TestByte)
		if ((pMsg->wParam >= 12) && (pMsg->wParam <= 45))
		{
			BYTE TestChar = (BYTE)(pMsg->lParam >> 16);
			switch (TestChar)
			{
				case 0x52: // 0:
					pMsg->wParam = 96;
				break;
				case 0x4F: // 1:
					pMsg->wParam = 97;
				break;
				case 0x50: // 2:
					pMsg->wParam = 98;
				break;
				case 0x51: // 3:
					pMsg->wParam = 99;
				break;
				case 0x4B: // 4:
					pMsg->wParam = 100;
				break;
				case 0x4C: // 5:
					pMsg->wParam = 101;
				break;
				case 0x4D: // 6:
					pMsg->wParam = 102;
				break;
				case 0x47: // 7:
					pMsg->wParam = 103;
				break;
				case 0x48: // 8:
					pMsg->wParam = 104;
				break;
				case 0x49: // / 9:
					pMsg->wParam = 105;
				break;
			}
		}
		*/
		if (vGlobalShiftKeyDown)
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
		else
		switch (pMsg->wParam)
		{
			case 113: // F2:
				OnSubFunction5Button();
			break;
			case 114: // F3:
				OnSubFunction7Button();
			break;
			case 115: // F4:
				OnSubFunction6Button();
			break;
			case 116: // F5:
				OnSubFunction1Button();
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
				OnSubFunction2Button();
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
				OnFunction3Button();
			break;
			case 107: // + Key - Sub Function 8:
				OnFunction5Button();
			break;
			case cPageUp: // PageUp:
				TurnKeyGreen(IDC_KeyClear);
			break;
			case 16: // Shift Key
				TurnKeyGreen(IDC_HiddenKey);
				//PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
			break;
			case 36:  //Home key, Screen Capture
				TurnKeyGreen(IDC_ScreenCapture);
			break;
			case 35:  //End key, Left/Right for dual beam
				TurnKeyGreen(IDC_LeftRight);
			break;
			case cPageDown: // PageDown:
				TurnKeyGreen(IDC_KeyHelp);
			break;
			case c0: // 0:
				TurnKeyGreen(IDC_Key0);
			break;
			case c1: // 0:
				TurnKeyGreen(IDC_Key1);
			break;
			case c2: // 2:
				TurnKeyGreen(IDC_Key2);
			break;
			case c3: // 3:
				TurnKeyGreen(IDC_Key3);
			break;
			case c4: // 4:
				TurnKeyGreen(IDC_Key4);
			break;
			case c5: // 5:
				TurnKeyGreen(IDC_Key5);
			break;
			case c6: // / 6:
				TurnKeyGreen(IDC_Key6);
			break;
			case c7: // 7:
				TurnKeyGreen(IDC_Key7);
			break;
			case c8: // - Key - 8
				TurnKeyGreen(IDC_Key8);
			break;
			case c9: // + Key - 9
				TurnKeyGreen(IDC_Key9);
			break;
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
			default:
				TurnKeyGreen(IDC_UnusedKey);
				SetDlgItemText(IDC_UnusedKey, "Unused: " + dtoa(pMsg->wParam, 0));
			break;
		}
		return true;  //true indicates it has been handled, so do not process
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CKeyBoardTestDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
		pDC->SetAttribDC(pDC->m_hDC);
	
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_ScreenCapture)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5);  //5 is medium large
		if (pWnd->GetDlgCtrlID() == vCurrentKeyIDD)
		{
			pDC->SetTextColor(cWhite);
			if (vShowRed)
				return vLocalSystemData->vBlueBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_LeftRight)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_LeftRight, 5);	//two line button next to functionbuttons
		pDC->SetBkMode(TRANSPARENT);
		if (pWnd->GetDlgCtrlID() == vCurrentKeyIDD)
		{
			pDC->SetTextColor(cWhite);
			if (vShowRed)
				return vLocalSystemData->vBlueBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_HiddenKey)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_HiddenKey, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (pWnd->GetDlgCtrlID() == vCurrentKeyIDD)
		{
			pDC->SetTextColor(cWhite);
			if (vShowRed)
				return vLocalSystemData->vBlueBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_UnusedKey)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_UnusedKey, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (pWnd->GetDlgCtrlID() == vCurrentKeyIDD)
		{
			pDC->SetTextColor(cWhite);
			if (vShowRed)
				return vLocalSystemData->vBlueBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function1Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5);  //5 is medium large
		if (pWnd->GetDlgCtrlID() == vCurrentKeyIDD)
		{
			pDC->SetTextColor(cWhite);
			if (vShowRed)
				return vLocalSystemData->vBlueBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function2Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);  //5 is medium large
		if (pWnd->GetDlgCtrlID() == vCurrentKeyIDD)
		{
			pDC->SetTextColor(cWhite);
			if (vShowRed)
				return vLocalSystemData->vBlueBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function3Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Button, 5);  //5 is medium large
		if (pWnd->GetDlgCtrlID() == vCurrentKeyIDD)
		{
			pDC->SetTextColor(cWhite);
			if (vShowRed)
				return vLocalSystemData->vBlueBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function4Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function4Button, 5);  //5 is medium large
		if (pWnd->GetDlgCtrlID() == vCurrentKeyIDD)
		{
			pDC->SetTextColor(cWhite);
			if (vShowRed)
				return vLocalSystemData->vBlueBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function5Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		if (pWnd->GetDlgCtrlID() == vCurrentKeyIDD)
		{
			pDC->SetTextColor(cWhite);
			if (vShowRed)
				return vLocalSystemData->vBlueBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		if (pWnd->GetDlgCtrlID() == vCurrentKeyIDD)
		{
			pDC->SetTextColor(cWhite);
			if (vShowRed)
				return vLocalSystemData->vBlueBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		if (pWnd->GetDlgCtrlID() == vCurrentKeyIDD)
		{
			pDC->SetTextColor(cWhite);
			if (vShowRed)
				return vLocalSystemData->vBlueBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
		if (pWnd->GetDlgCtrlID() == vCurrentKeyIDD)
		{
			pDC->SetTextColor(cWhite);
			if (vShowRed)
				return vLocalSystemData->vBlueBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		if (pWnd->GetDlgCtrlID() == vCurrentKeyIDD)
		{
			pDC->SetTextColor(cWhite);
			if (vShowRed)
				return vLocalSystemData->vBlueBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		if (pWnd->GetDlgCtrlID() == vCurrentKeyIDD)
		{
			pDC->SetTextColor(cWhite);
			if (vShowRed)
				return vLocalSystemData->vBlueBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Button) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5);  //5 is medium large
		if (pWnd->GetDlgCtrlID() == vCurrentKeyIDD)
		{
			pDC->SetTextColor(cWhite);
			if (vShowRed)
				return vLocalSystemData->vBlueBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
		if (pWnd->GetDlgCtrlID() == vCurrentKeyIDD)
		{
			pDC->SetTextColor(cWhite);
			if (vShowRed)
				return vLocalSystemData->vBlueBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		if (pWnd->GetDlgCtrlID() == vCurrentKeyIDD)
		{
			pDC->SetTextColor(cWhite);
			if (vShowRed)
				return vLocalSystemData->vBlueBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_KeyHelp)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_KeyHelp, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (pWnd->GetDlgCtrlID() == vCurrentKeyIDD)
		{
			pDC->SetTextColor(cWhite);
			if (vShowRed)
				return vLocalSystemData->vBlueBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_KeyClear)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_KeyClear, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (pWnd->GetDlgCtrlID() == vCurrentKeyIDD)
		{
			pDC->SetTextColor(cWhite);
			if (vShowRed)
				return vLocalSystemData->vBlueBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}
	if ((pWnd->GetDlgCtrlID() == IDC_Key1) || (pWnd->GetDlgCtrlID() == IDC_Key2) || (pWnd->GetDlgCtrlID() == IDC_Key3)
	|| (pWnd->GetDlgCtrlID() == IDC_Key4) || (pWnd->GetDlgCtrlID() == IDC_Key5) || (pWnd->GetDlgCtrlID() == IDC_Key6)
	|| (pWnd->GetDlgCtrlID() == IDC_Key7) || (pWnd->GetDlgCtrlID() == IDC_Key8) || (pWnd->GetDlgCtrlID() == IDC_Key9)
	|| (pWnd->GetDlgCtrlID() == IDC_Key0))
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Key1, 5);  //key buttons not function buttons but is three lines
		pDC->SetBkMode(TRANSPARENT);
		if (pWnd->GetDlgCtrlID() == vCurrentKeyIDD)
		{
			pDC->SetTextColor(cWhite);
			if (vShowRed)
				return vLocalSystemData->vBlueBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}
	return hbr;
}

void CKeyBoardTestDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == vGreenLightTimerHandle)
	{
		bool TempTimeToExit = false;
		if (vCurrentKeyIDD == IDC_Function2Button)
			TempTimeToExit = true;

		bool TempShowHelp = false;
		if (vCurrentKeyIDD == IDC_KeyHelp)
			TempShowHelp = true;

		bool TempPrintScreen = false;
		if (vCurrentKeyIDD == IDC_ScreenCapture)
			TempPrintScreen = true;

		CWnd *TempWindow = CWnd::GetDlgItem(vCurrentKeyIDD);
		vCurrentKeyIDD = IDC_NoWindow;
		KillTimer(vGreenLightTimerHandle);
		if (TempWindow != NULL)
			TempWindow->Invalidate(false);

		if (TempPrintScreen)
		if (!vShowMultiple)
		{
			vGlobalShiftKeyDown = true;
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
		}

		// Save and Exit Button was pressed
		if (TempTimeToExit)
		if (!vShowMultiple)
			CDialog::EndDialog(true);

		if (TempShowHelp)
		if (!vShowMultiple)
			PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 17, 0);
	}
	CDialog::OnTimer(nIDEvent);
}

void CKeyBoardTestDialog::TurnKeyGreen(int TempKeyIDC)
{
	//if a key is on already on, then turn it off
	KillTimer(vGreenLightTimerHandle);
	if (vCurrentKeyIDD == TempKeyIDC)
	{
		if (vShowRed)
			vShowRed = false;
		else
		{
			vShowRed = true;
			vShowMultiple = true;
		}
	}
	else
	{
		vShowRed = false;
		vShowMultiple = false;
	}
	if (vCurrentKeyIDD != IDC_NoWindow)
	{
		OnTimer(vGreenLightTimerHandle);
	}
	//set this key green and turn on timer
	int TempResult = SetTimer(vGreenLightTimerHandle,cGreenTimeDelay,NULL);
	vCurrentKeyIDD = TempKeyIDC;
	if (vCurrentKeyIDD == IDC_LeftRight)
		m_LeftRight.ShowWindow(SW_SHOW);
	
	if (vCurrentKeyIDD == IDC_HiddenKey)
		m_HiddenKey.ShowWindow(SW_SHOW);

	if (vCurrentKeyIDD == IDC_UnusedKey)
		m_UnusedKey.ShowWindow(SW_SHOW);

	CWnd *TempWindow = CWnd::GetDlgItem(vCurrentKeyIDD);
	if (TempWindow)
		TempWindow->Invalidate(false);
}

void CKeyBoardTestDialog::OnKey1() 
{
	TurnKeyGreen(IDC_Key1);
}

void CKeyBoardTestDialog::OnKey2() 
{
	TurnKeyGreen(IDC_Key2);
}

void CKeyBoardTestDialog::OnKey3() 
{
	TurnKeyGreen(IDC_Key3);
}

void CKeyBoardTestDialog::OnKey4() 
{
	TurnKeyGreen(IDC_Key4);
}

void CKeyBoardTestDialog::OnKey5() 
{
	TurnKeyGreen(IDC_Key5);
}

void CKeyBoardTestDialog::OnKey6() 
{
	TurnKeyGreen(IDC_Key6);
}

void CKeyBoardTestDialog::OnKey7() 
{
	TurnKeyGreen(IDC_Key7);
}

void CKeyBoardTestDialog::OnKey8() 
{
	TurnKeyGreen(IDC_Key8);
}

void CKeyBoardTestDialog::OnKey9() 
{
	TurnKeyGreen(IDC_Key9);
}

void CKeyBoardTestDialog::OnKey0() 
{
	TurnKeyGreen(IDC_Key0);
}

void CKeyBoardTestDialog::OnKeyClear() 
{
	TurnKeyGreen(IDC_KeyClear);
}

void CKeyBoardTestDialog::OnKeyHelp() 
{
	TurnKeyGreen(IDC_KeyHelp);
}

void CKeyBoardTestDialog::OnScreenCapture() 
{
	TurnKeyGreen(IDC_ScreenCapture);
}

void CKeyBoardTestDialog::OnLeftRight() 
{
	TurnKeyGreen(IDC_LeftRight);
}

void CKeyBoardTestDialog::OnHiddenKey() 
{
	TurnKeyGreen(IDC_HiddenKey);
}


void CKeyBoardTestDialog::OnUnused()
{
	TurnKeyGreen(IDC_UnusedKey);
}


void CKeyBoardTestDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
