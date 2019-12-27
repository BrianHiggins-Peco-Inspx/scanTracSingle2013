//ScanTrac Side View Source File
// NumericEntryDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "NumericEntryDialog.h"
#include "NoticeDialog.h"
#include "YesNoDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BYTE vGlobalUseCommaForDecimalPoint;
extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
extern CScanTracSystemRunningData *vGlobalRunningData;
/////////////////////////////////////////////////////////////////////////////
// CNumericEntryDialog dialog


CNumericEntryDialog::CNumericEntryDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNumericEntryDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNumericEntryDialog)
	m_DialogTitleStaticText2 = _T("");
	m_UnitsString = _T("");
	m_DialogTitleStaticText1 = _T("");
	m_DialogTitleStaticText3 = _T("");
	m_DialogTitleStaticText4 = _T("");
	//}}AFX_DATA_INIT
	vWhiteBrush = ::CreateSolidBrush(cWhite);
	vLightBlueBrush = ::CreateSolidBrush(cLightBlue);
	vBlueBrush = ::CreateSolidBrush(cBlue);
	vGreenBrush = ::CreateSolidBrush(cGreen);
	vCursorPosition = 0;
	vIntegerOnly = true;
	vEditString = "";
	vAllowNegative = false;
	vMinValue = 0;
	vMaxValue = 0;
	vDeleteAll = true;
	vShowNextOnButton2 = false;
	vAllowZero = true;
	vAlreadyOpen = false;
	vWarnIfLargerThan4 = false;
}

CNumericEntryDialog::~CNumericEntryDialog()
{
	DeleteObject(vWhiteBrush);
	DeleteObject(vBlueBrush);
	DeleteObject(vLightBlueBrush);
	DeleteObject(vGreenBrush);
}

void CNumericEntryDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

	vDeleteAll = true;
	vCursorPosition = vEditString.GetLength();
	if (vIntegerOnly)
	{
		SetDlgItemText(IDC_Function3Button, _T(""));
		//UpdateData(false);
	}
	else
	{
		m_Function3Button.ShowWindow(SW_SHOW);
	}
	if (vAllowNegative)
	{
		SetDlgItemText(IDC_SubFunction1Button, _T("-"));
		m_SubFunction1Button.ShowWindow(SW_SHOW);
	}
	if (vGlobalUseCommaForDecimalPoint)
		SetDlgItemText(IDC_Function3Button, _T(","));

	CString TempText("");
	TempText.LoadString(IDS_EnterNumberFor);
	SetDlgItemText(IDC_DialogTitleStaticText,TempText);
	TempText.LoadString(IDS_MainMenu);
	SetDlgItemText(IDC_Function1Button,TempText);
	TempText.LoadString(IDS_SaveAndExit);
	if (vShowNextOnButton2)
	{
		m_Function2Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		TempText.LoadString(IDS_Next);
	}
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
	this->SetWindowText(_T("Number"));
	SetupMenu();

	m_SubFunction4Button.ShowNavigationArrow(cLeftArrowBig);
	m_SubFunction8Button.ShowNavigationArrow(cRightArrowBig);
}

void CNumericEntryDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNumericEntryDialog)
	DDX_Control(pDX, IDC_KeyHelp, m_KeyHelp);
	DDX_Control(pDX, IDC_KeyClear, m_KeyClear);
	DDX_Control(pDX, IDC_Key8, m_Key8);
	DDX_Control(pDX, IDC_Key9, m_Key9);
	DDX_Control(pDX, IDC_Key7, m_Key7);
	DDX_Control(pDX, IDC_Key6, m_Key6);
	DDX_Control(pDX, IDC_Key5, m_Key5);
	DDX_Control(pDX, IDC_Key4, m_Key4);
	DDX_Control(pDX, IDC_Key3, m_Key3);
	DDX_Control(pDX, IDC_Key2, m_Key2);
	DDX_Control(pDX, IDC_Key0, m_Key0);
	DDX_Control(pDX, IDC_Key1, m_Key1);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_EditStaticText, m_EditStaticText);
	DDX_Control(pDX, IDC_DialogTitleStaticText2, m_DialogTitleStaticTextBox2);
	DDX_Control(pDX, IDC_UnitsString, m_UnitsStringBox);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticTextBox1);
	DDX_Control(pDX, IDC_DialogTitleStaticText3, m_DialogTitleStaticTextBox3);
	DDX_Control(pDX, IDC_DialogTitleStaticText4, m_DialogTitleStaticTextBox4);
	DDX_Text(pDX, IDC_DialogTitleStaticText2, m_DialogTitleStaticText2);
	DDX_Text(pDX, IDC_UnitsString, m_UnitsString);
	DDX_Text(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText1);
	DDX_Control(pDX, IDC_DialogTitleStaticText, m_DialogTitleStaticText);
	DDX_Text(pDX, IDC_DialogTitleStaticText3, m_DialogTitleStaticText3);
	DDX_Text(pDX, IDC_DialogTitleStaticText4, m_DialogTitleStaticText4);
//	DDX_Control(pDX, IDC_DialogTitleStaticText6, m_DialogTitleStaticTextBox6);
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
}


BEGIN_MESSAGE_MAP(CNumericEntryDialog, CDialog)
	//{{AFX_MSG_MAP(CNumericEntryDialog)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_SubFunction4Button, OnSubFunction4Button)
	ON_BN_CLICKED(IDC_SubFunction3Button, OnSubFunction3Button)
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
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
	ON_BN_CLICKED(IDC_SubFunction1Button, OnSubFunction1Button)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText, &CNumericEntryDialog::OnDialogtitlestatictext7)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNumericEntryDialog message handlers

void CNumericEntryDialog::OnFunction1Button() 
{
	// User clicked Main Menu Button to exit without saving
	CDialog::EndDialog(10);
}

void CNumericEntryDialog::OnFunction2Button() 
{
	//User pressed Save and exit
	BYTE TempLength = vEditString.GetLength();
	if (TempLength > 0)
  {
		BYTE TempOldCursorPosition = vEditString.Find(_T("&"));
		if (TempOldCursorPosition >= 0)
			vEditString.Delete(TempOldCursorPosition,1);

		if (vEditString == "")
			vEditString = "0";

		double TempMaxValue = ATOF(dtoa(vMaxValue,2));
		double TempMinValue = ATOF(dtoa(vMinValue,2));
		double TempData = ATOF(vEditString);
		if ((((TempData < 0) && (!vAllowNegative)) || 
			(TempData > TempMaxValue) || (TempData < -vMaxValue)) ||
			((!vAllowZero) && (TempData == 0)))
		{
			vDeleteAll = true;
			CNoticeDialog TempNoticeDialog;

			CString TempString3 = dtoa(vMinValue,2);
			CString TempString4 = dtoa(vMaxValue,2);
			CString TempText("");
			TempText.LoadString(IDS_ValueMustBeBetween);
			CString TempString1 = TempText + " ";
			TempText.LoadString(IDS_And);
			if (vAllowNegative)
				TempString1 = TempString1 + "-" + TempString4 + " " + TempText + " ";
			else
				TempString1 = TempString1 + TempString3 + " " + TempText + " ";

			CString TempString2(TempString1 + TempString4);
			if ((vAllowZero) && (vMinValue > 0))
				TempString2 = TempString2 + " or 0";
			TempNoticeDialog.vNoticeText = TempString2;
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
			SetCursor();
		}
		else
		if ((TempMinValue) && (TempData < TempMinValue) && (!((vAllowZero) && (TempData == 0))))
		{
			vDeleteAll = true;
			CNoticeDialog TempNoticeDialog;

			CString TempString3 = dtoa(vMinValue,2);
			CString TempString4 = dtoa(vMaxValue,2);
			CString TempText("");
			TempText.LoadString(IDS_ValueMustBeBetween);
			CString TempString1 = TempText + " ";
			TempText.LoadString(IDS_And);
			TempString1 = TempString1 + TempString3 + " " + TempText + " ";

			CString TempString2(TempString1 + TempString4);
			if (vAllowZero)
				TempString2 = TempString2 + " or 0";
			TempNoticeDialog.vNoticeText = TempString2;
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
			SetCursor();
		}
		else
		{
			bool TempOKToExit = true;

			if (vWarnIfLargerThan4)
			{
				if ((TempData < -4) || (TempData > 4))
				{
					CYesNoDialog TempYesNoDialog;
					TempYesNoDialog.vNoticeText = _T("\nNormally this value is near zero\n\nYou should check your settings and value");
					TempYesNoDialog.vYesButtonText = "Save This Value";
					TempYesNoDialog.vNoButtonText = "I Will Recheck";
					TempYesNoDialog.vQuestionType = cWarningQuestion;
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult != IDOK)
						TempOKToExit = false;
				}
			}

			if (TempOKToExit)
				CDialog::EndDialog(true);
		}
  }
	else
	{
		CNoticeDialog TempNoticeDialog;
		CString TempText("");
		TempText.LoadString(IDS_ValueCannotBeBlank);
		TempNoticeDialog.vNoticeText = TempText;
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
}

void CNumericEntryDialog::OnFunction3Button() 
{
	ProcessKey(cF3);
}

void CNumericEntryDialog::OnFunction4Button() 
{
	//Cancel was pressed
	CDialog::OnCancel();
}

void CNumericEntryDialog::OnSubFunction1Button() 
{
	if (vDeleteAll)
	{
		vEditString = " ";
		vDeleteAll = false;
	}
	if (vAllowNegative)
	{
		if (vEditString.GetAt(0) == '-')
		{
			vEditString.Delete(0,1);
			vCursorPosition--;
		}
		else
		{
			if (vEditString == " ")
			{
				vEditString = "";
				vCursorPosition = 0;
			}

			vEditString.Insert(0,'-');
			vCursorPosition++;
		}
		SetCursor();
	}
}

void CNumericEntryDialog::OnSubFunction3Button() 
{
}

void CNumericEntryDialog::OnSubFunction4Button() 
{
	//Left arrow Key pressed
	if (vDeleteAll)
		vCursorPosition = 0;
	if (vCursorPosition > 0)
		vCursorPosition--;
	vDeleteAll = false;
	SetCursor();
}

void CNumericEntryDialog::OnSubFunction8Button() 
{
	//Right Arrow Button pressed
	vCursorPosition++;
	vDeleteAll = false;
	SetCursor();
}

void CNumericEntryDialog::SetCursor()
{

	if (vIntegerOnly)
	{
		BYTE TempOldCursorPosition1 = 0;
		if (vGlobalUseCommaForDecimalPoint)
			TempOldCursorPosition1 = vEditString.Find(_T(","));
		else
			TempOldCursorPosition1 = vEditString.Find(_T("."));
		if (TempOldCursorPosition1 >= 0)
		{
			vEditString.Delete(TempOldCursorPosition1,1);
		}
	}
	BYTE TempOldCursorPosition = vEditString.Find(_T("&"));
	if (TempOldCursorPosition >= 0)
	{
		vEditString.Delete(TempOldCursorPosition,1);
	}
	if (vCursorPosition > vEditString.GetLength())
	{
		vCursorPosition = vEditString.GetLength();
	}
	if (!vDeleteAll)
		vEditString.Insert(vCursorPosition, _T("&"));
	SetDlgItemText(IDC_EditStaticText,vEditString);
	//UpdateData(false);
}

BOOL CNumericEntryDialog::PreTranslateMessage(MSG* pMsg) 
{
	CWnd * TempWindow = NULL;
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
		if (pMsg->wParam == 190) //if pressed . on Keyboard, convert to . on Keypad
			pMsg->wParam = 110;

		if ((pMsg->wParam == 188) && (vGlobalUseCommaForDecimalPoint)) //if pressed , on keyboard, convert to . on keypad
			pMsg->wParam = 110;

		//get an alpha numeric Key press
		if (((pMsg->wParam >= c0) && (pMsg->wParam <= c9)) ||
			((pMsg->wParam == cF3) && (!vIntegerOnly)))
		{
			ProcessKey(pMsg->wParam);
		}
		else
		if ((pMsg->wParam >= 48) && (pMsg->wParam <= 57))
		{
			ProcessKey(pMsg->wParam + 48);
		}
		else
		{
			//didn't get alphanumeric Key press, so reset Key press counter
		}
		//check for function Keys
		if (vGlobalShiftKeyDown)
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
		else
		switch (pMsg->wParam)
		{
			case 0x6e: // F3:
				OnFunction3Button();
			break;
			case 0x0d: // F2:
				OnFunction2Button();
			break;
			case 113: // F2:
				//OnSubFunction5Button();
			break;
			case 114: // F3:
				//OnSubFunction7Button();
			break;
			case 115: // F4:
				//OnSubFunction6Button();
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
			//case 109: // - Key - Sub Function 7  Decimal Point:
				//ProcessKey(cF3);  called up above
			//break;
			case 107: // + Key - Sub Function 8:
				//OnFunction5Button();
			break;
			case cPageUp: // PageUp:
				OnKeyClear();
			break;
			case 16: // Shift Key
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
			break;
			case 36:  //Home Key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
			case cPageDown: // Help
				OnKeyHelp();
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
			break;
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
		}
		return true;  //true indicates it has been handled, so do not process
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CNumericEntryDialog::ProcessKey(BYTE TempKey)
{
	if (!vGlobalShiftKeyDown)
	{
		//if hitting same Key as before, back up one character
		if (vDeleteAll)
		if (((TempKey >= c0) && (TempKey <= c9)) || (TempKey <= cPageUp) || (TempKey <= cF3))
		{
			vEditString = "";
			vDeleteAll = false;
		}

		BYTE TempLength = vEditString.GetLength();
		//make sure string 10 characters
		// so have room to add one more
		CString TempText("");
		if (TempLength > 10)
		{
			CNoticeDialog TempNoticeDialog;
			TempText.LoadString(IDS_10DigitsMaximum);
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			BYTE TempIndex = 0;

			if ((TempKey >= c0) && (TempKey <= c9))
			{
				TempIndex = TempKey - c0;
				vEditString.Insert(vCursorPosition,(CHAR)vNumberKeys[TempIndex]);
				//vEditString.Insert(vCursorPosition,(CHAR)pMsg->wParam);
			}
			else
			if (TempKey == cF3)
			{
				BYTE TempOldCursorPosition = 0;
				if (vGlobalUseCommaForDecimalPoint)
					TempOldCursorPosition = vEditString.Find(_T(","));
				else
					TempOldCursorPosition = vEditString.Find(_T("."));
				if ((TempOldCursorPosition >= 0) && (TempOldCursorPosition < 25))
				{
					CNoticeDialog TempNoticeDialog;

					TempText.LoadString(IDS_Only1DecimalPointAllowed);
					TempNoticeDialog.vNoticeText = TempText;
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}
				else
				{
					if (vGlobalUseCommaForDecimalPoint)
						vEditString.Insert(vCursorPosition, _T(","));
					else
						vEditString.Insert(vCursorPosition, _T("."));
				}
			}
			vCursorPosition++;
			SetCursor();
		}
	}
}

HBRUSH CNumericEntryDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}


  if (pWnd->GetDlgCtrlID() == IDC_EditStaticText) 
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_EditStaticText, 5);
		if (vDeleteAll)
		{
			pDC->SetBkColor(cBlue);
			pDC->SetTextColor(cWhite);
		}
		//if text is highlighted, then show blue background, else
		//make transparent so shows white background
		else
			pDC->SetBkMode(TRANSPARENT);
		return vWhiteBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticTextBox1, 5);
		return vLightBlueBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText2) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticTextBox2, 5);
		return vLightBlueBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText3) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticTextBox3, 5);
		return vLightBlueBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText4) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticTextBox4, 5);
		return vLightBlueBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_UnitsString)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_UnitsStringBox, 5);
		return vLightBlueBrush;
  }
	if ((pWnd->GetDlgCtrlID() == IDC_Background))
	{
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
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);  //5 is medium large
		//if (vInAutoSetup)
		pDC->SetBkMode(TRANSPARENT);
		return vGreenBrush;
		
		pDC->SetBkMode(TRANSPARENT);
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
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Key1, 5);  //Key buttons not function buttons but is two lines
	}
	return hbr;
}

void CNumericEntryDialog::OnKey0() 
{
	ProcessKey(c0);
}

void CNumericEntryDialog::OnKey1() 
{
	ProcessKey(c1);
}

void CNumericEntryDialog::OnKey2() 
{
	ProcessKey(c2);
}

void CNumericEntryDialog::OnKey3() 
{
	ProcessKey(c3);
}

void CNumericEntryDialog::OnKey4() 
{
	ProcessKey(c4);
}

void CNumericEntryDialog::OnKey5() 
{
	ProcessKey(c5);
}

void CNumericEntryDialog::OnKey6() 
{
	ProcessKey(c6);
}

void CNumericEntryDialog::OnKey7() 
{
	ProcessKey(c7);
}

void CNumericEntryDialog::OnKey8() 
{
	ProcessKey(c8);
}

void CNumericEntryDialog::OnKey9() 
{
	ProcessKey(c9);
}

void CNumericEntryDialog::OnKeyClear() 
{
	//if backspace character
	BYTE TempLength = vEditString.GetLength();
	if (vDeleteAll)
		vEditString = "";
	else
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
	//UpdateData(false);
	vDeleteAll = false;
	SetCursor();
}

void CNumericEntryDialog::OnKeyHelp() 
{
	//Help Key Pressed
	PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 24, 0);
}

void CNumericEntryDialog::SetupMenu()
{

	if (!vAlreadyOpen)
	{
		vLocalCWndCollection.Add(this);
		vLocalCWndCollection.Add(&m_DialogTitleStaticText);
		vLocalCWndCollection.Add(&m_DialogTitleStaticTextBox1);
		vLocalCWndCollection.Add(&m_DialogTitleStaticTextBox2);
		vLocalCWndCollection.Add(&m_DialogTitleStaticTextBox3);
		vLocalCWndCollection.Add(&m_DialogTitleStaticTextBox4);
//		vLocalCWndCollection.Add(&m_DialogTitleStaticTextBox6);
		vLocalCWndCollection.Add(&m_Function2Button			 );
		vLocalCWndCollection.Add(&m_SubFunction1Button		 );
		vLocalCWndCollection.Add(&m_Function3Button			 );
		vLocalCWndCollection.Add(&m_KeyHelp					 );
		vLocalCWndCollection.Add(&m_KeyClear				 );
		vLocalCWndCollection.Add(&m_Key8					 );
		vLocalCWndCollection.Add(&m_Key9					 );
		vLocalCWndCollection.Add(&m_Key7					 );
		vLocalCWndCollection.Add(&m_Key6					 );
		vLocalCWndCollection.Add(&m_Key5					 );
		vLocalCWndCollection.Add(&m_Key4					 );
		vLocalCWndCollection.Add(&m_Key3					 );
		vLocalCWndCollection.Add(&m_Key2					 );
		vLocalCWndCollection.Add(&m_Key0					 );
		vLocalCWndCollection.Add(&m_Key1					 );
		vLocalCWndCollection.Add(&m_Background				 );
		vLocalCWndCollection.Add(&m_EditStaticText			 );
		vLocalCWndCollection.Add(&m_Function1Button			 );
		vLocalCWndCollection.Add(&m_SubFunction2Button		 );
		vLocalCWndCollection.Add(&m_SubFunction3Button		 );
		vLocalCWndCollection.Add(&m_SubFunction4Button		 );
		vLocalCWndCollection.Add(&m_SubFunction5Button		 );
		vLocalCWndCollection.Add(&m_SubFunction6Button		 );
		vLocalCWndCollection.Add(&m_SubFunction7Button		 );
		vLocalCWndCollection.Add(&m_SubFunction8Button		 );
		vLocalCWndCollection.Add(&m_UnitsStringBox			 );
		vLocalCWndCollection.Add(&m_Function4Button			 );
		vLocalCWndCollection.Add(&m_Function5Button			 );
		vAlreadyOpen = true;
	}
	::SetupMenu(vLocalCWndCollection);		
				 
	int TempIDCArray[] = {
	  IDC_DialogTitleStaticText1
	, IDC_DialogTitleStaticText2
	, IDC_DialogTitleStaticText3
	, IDC_DialogTitleStaticText4
	, IDC_DialogTitleStaticText6
	, IDC_UnitsString
	};
	CString* TempStringArray[] = {
	  &m_DialogTitleStaticText1
	, &m_DialogTitleStaticText2
	, &m_DialogTitleStaticText3
	, &m_DialogTitleStaticText4
	, &m_DialogTitleStaticText6
	, &m_UnitsString
	};
	for (BYTE TempLoop = 0; TempLoop < 6; TempLoop++)
	{
		SetDlgItemText(TempIDCArray[TempLoop],*TempStringArray[TempLoop]);
	}
	if (m_DialogTitleStaticText6 == "")
	{
		CString TempText("");
		TempText.LoadString(IDS_EnterNumberFor);
		SetDlgItemText(IDC_DialogTitleStaticText6,TempText);
	}

}
void CNumericEntryDialog::OnDialogtitlestatictext7()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
