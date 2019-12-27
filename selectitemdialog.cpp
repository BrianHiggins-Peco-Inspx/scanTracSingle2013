//ScanTrac Side View Source File ScanTrac
// SelectItemDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "SelectItemDialog.h"
#include "NoticeDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern double vGlobalPercentToResizeY;
extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
extern CScanTracSystemRunningData *vGlobalRunningData;
/////////////////////////////////////////////////////////////////////////////
// CSelectItemDialog dialog


CSelectItemDialog::CSelectItemDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectItemDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectItemDialog)
	m_DialogTitleStaticText2 = _T("");
	m_DialogTitleStaticText3 = _T("");
	//}}AFX_DATA_INIT
	vMakeTitle3Big = false;
	vMakeTitle3Highlighted = false;
	vSetWindowActiveTimerHandle = 9937;
	for (WORD TempLoop = 0; TempLoop < cMaxSelectDialogItems; TempLoop++)
		vItemEnabled[TempLoop] = true;

	vItemNumberSelected = 0;
	vNumberOfItems = 0;
	vCurrentFirstItemNumberShown = 1;
	vBlueBrush = ::CreateSolidBrush(cBlue);
	vYellowBrush = ::CreateSolidBrush(cYellow);
	vGreenBrush = ::CreateSolidBrush(cGreen);
	vBackgroundColor = cGreen;
	vInAutoSetup = false;
	vShowExit = false;
	vGreenItem = 0xFFFF;
	vYellowItem = 0xFFFF;
	vOkayMessage = "";
	vStayTillDecissionSetup = false;
	vQuickFindCharacter = ' ';

	vSelectDialogType = cItemSelectDialog; //cProductSelectDialog,cHelpIndexSelectDialog};
	m_DialogTitleStaticText4 = _T("");
	m_DialogTitleStaticText5 = _T("");

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_DialogTitleControlStaticText5);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText1);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_DialogTitleStaticTextBox2);
	vLocalCWndCollection.Add(&m_DialogTitleStaticTextBox3);
	vLocalCWndCollection.Add(&m_DialogTitleStaticTextBox4);

	vLocalDisplayCollection.Add(&m_SubFunction8Button);
	vLocalDisplayCollection.Add(&m_SubFunction7Button);
	vLocalDisplayCollection.Add(&m_SubFunction6Button);
	vLocalDisplayCollection.Add(&m_SubFunction5Button);
	vLocalDisplayCollection.Add(&m_SubFunction4Button);
	vLocalDisplayCollection.Add(&m_SubFunction3Button);
	vLocalDisplayCollection.Add(&m_SubFunction2Button);
	vLocalDisplayCollection.Add(&m_SubFunction1Button);
}

CSelectItemDialog::~CSelectItemDialog()
{
	DeleteObject(vBlueBrush);
	DeleteObject(vYellowBrush);
	DeleteObject(vGreenBrush);
	DeleteObject(vBackgroundBrush);
	DeleteObject(vLightBackgroundBrush);
}

void CSelectItemDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if (vSelectDialogType == cHelpIndexSelectDialog)
		vBackgroundColor = cWhite; //cProductSelectDialog,};
	
	vSubFunction1ButtonEnable = true;
	vSubFunction2ButtonEnable = true;
	vSubFunction3ButtonEnable = true;
	vSubFunction4ButtonEnable = true;
	vSubFunction5ButtonEnable = true;
	vSubFunction6ButtonEnable = true;
	vSubFunction7ButtonEnable = true;
	vSubFunction8ButtonEnable = true;
	
	if (vBackgroundColor == cGreen)
	{
		vLightBackgroundBrush = ::CreateSolidBrush(cLightGreen);
		vBackgroundBrush = ::CreateSolidBrush(cGreen);
	}
	else
	if (vBackgroundColor == cYellow)
	{
		vLightBackgroundBrush = ::CreateSolidBrush(cLightYellow);
		vBackgroundBrush = ::CreateSolidBrush(cYellow);
	}
	else
	if (vBackgroundColor == cWhite)
	{
		vLightBackgroundBrush = ::CreateSolidBrush(cWhite);
		vBackgroundBrush = ::CreateSolidBrush(cWhite);
	}
	else
	{
		vLightBackgroundBrush = ::CreateSolidBrush(cLightRed);
		vBackgroundBrush = ::CreateSolidBrush(cRed);
	}
	CString TempText("");
	m_Function4Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
	TempText.LoadString(IDS_Cancel);
	SetDlgItemText(IDC_Function4Button,TempText);
	if (vInAutoSetup)
	{
		m_Function2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());
		//m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		//TempText.LoadString(IDS_NextAutoSetupStep);
		//SetDlgItemText(IDC_Function2Button,TempText);
		m_Function4Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		m_Function4Button.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_Cancel);
		SetDlgItemText(IDC_Function4Button,TempText);
	}
	else 
	if (vShowExit)
	{
		m_Function2Button.ShowWindow(SW_SHOW);
		if (vOkayMessage == "")
		{
			m_Function2Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			TempText.LoadString(IDS_OK);
		}
		else
		{
			m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			TempText = vOkayMessage;
		}
		SetDlgItemText(IDC_Function2Button,TempText);
	}
	TempText.LoadString(IDS_MainMenu);
	SetDlgItemText(IDC_Function1Button,TempText);
	TempText.LoadString(IDS_Previous);
	SetDlgItemText(IDC_Function3Button,TempText);
	TempText.LoadString(IDS_Next);
	SetDlgItemText(IDC_Function5Button,TempText);

	::SetupMenu(vLocalCWndCollection);
	UpdateItemDisplay();
	SetDlgItemText(IDC_DialogTitleStaticText1,vTitleString);
	this->SetWindowText(_T("Select Item"));

	int TimerResult = SetTimer(vSetWindowActiveTimerHandle,3300,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-Startup Timer Failed",cEMailInspx,32000);
	if (m_DialogTitleStaticText5 != _T(""))
	{
		m_DialogTitleControlStaticText5.ShowWindow(SW_SHOW);
	}
}

void CSelectItemDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectItemDialog)
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_DialogTitleStaticText2, m_DialogTitleStaticTextBox2);
	DDX_Control(pDX, IDC_DialogTitleStaticText3, m_DialogTitleStaticTextBox3);
	DDX_Control(pDX, IDC_DialogTitleStaticText4, m_DialogTitleStaticTextBox4);
	DDX_Text(pDX, IDC_DialogTitleStaticText2, m_DialogTitleStaticText2);
	DDX_Text(pDX, IDC_DialogTitleStaticText3, m_DialogTitleStaticText3);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_DialogTitleStaticText4, m_DialogTitleStaticText4);
	DDV_MaxChars(pDX, m_DialogTitleStaticText4, 50);
	DDX_Text(pDX, IDC_DialogTitleStaticText5, m_DialogTitleStaticText5);
	DDV_MaxChars(pDX, m_DialogTitleStaticText5, 50);
	DDX_Control(pDX, IDC_DialogTitleStaticText5, m_DialogTitleControlStaticText5);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText1);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
}


BEGIN_MESSAGE_MAP(CSelectItemDialog, CDialog)
	//{{AFX_MSG_MAP(CSelectItemDialog)
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
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CSelectItemDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectItemDialog message handlers

void CSelectItemDialog::OnFunction1Button() 
{
	// User clicked Main Menu Button to exit without saving
	if (vInAutoSetup)
	{
		if(AbortSetupChanges())	
			CDialog::EndDialog(10);
	}
	else
		CDialog::EndDialog(10);
}

void CSelectItemDialog::OnFunction2Button() 
{
	//Cancel button pressed
	//if (vInAutoSetup)
	//	CDialog::EndDialog(true);
	//else
	if (!vInAutoSetup)
	if (vShowExit)
		CDialog::EndDialog(11);
}

void CSelectItemDialog::OnFunction3Button() 
{
	//Previous Group button was pressed
  if (vCurrentFirstItemNumberShown > 1)
  {
		if (vCurrentFirstItemNumberShown > 8)
      vCurrentFirstItemNumberShown = vCurrentFirstItemNumberShown - 8;
		else
			vCurrentFirstItemNumberShown = 1;
		UpdateItemDisplay();
  }	
}

void CSelectItemDialog::OnFunction4Button() 
{
	//if (vInAutoSetup)  //if in autosetup go to previous step
	{
		CDialog::OnCancel();
	}
}

void CSelectItemDialog::OnFunction5Button() 
{
	//next button pressed to display next group of products
	if (vNumberOfItems > vCurrentFirstItemNumberShown + 7)
	{
		//if (vCurrentFirstItemNumberShown + 15 > vNumberOfItems)
		//	vCurrentFirstItemNumberShown = vNumberOfItems - 7;
		//else
		vCurrentFirstItemNumberShown = vCurrentFirstItemNumberShown + 8;
		UpdateItemDisplay();
	}
}

void CSelectItemDialog::OnSubFunction1Button() 
{
	if (vSubFunction1ButtonEnable == true)
	{
	if (!vStayTillDecissionSetup)
		SelectAndExit(1);
	else
		SelectAndContinue(1);
	}
	else
	{
		ShowCurrentSelectionNotAvailableDialog();
	}
}

void CSelectItemDialog::OnSubFunction2Button() 
{
	if (vSubFunction2ButtonEnable == true)
	{
	if (!vStayTillDecissionSetup)
		SelectAndExit(2);
	else
		SelectAndContinue(2);
	}
	else
	{
		ShowCurrentSelectionNotAvailableDialog();
	}
}

void CSelectItemDialog::OnSubFunction3Button() 
{
	if (vSubFunction3ButtonEnable == true)
	{
	if (!vStayTillDecissionSetup)
		SelectAndExit(3);
	else
		SelectAndContinue(3);
	}
	else
	{
		ShowCurrentSelectionNotAvailableDialog();
	}
}

void CSelectItemDialog::OnSubFunction4Button() 
{
	if (vSubFunction4ButtonEnable == true)
	{
	if (!vStayTillDecissionSetup)
		SelectAndExit(4);
	else
		SelectAndContinue(4);
	}
	else
	{
		ShowCurrentSelectionNotAvailableDialog();
	}
}

void CSelectItemDialog::OnSubFunction5Button() 
{
	if (vSubFunction5ButtonEnable == true)
	{
	if (!vStayTillDecissionSetup)
		SelectAndExit(5);
	else
		SelectAndContinue(5);
	}
	else
	{
		ShowCurrentSelectionNotAvailableDialog();
	}
}

void CSelectItemDialog::OnSubFunction6Button() 
{
	if (vSubFunction6ButtonEnable == true)
	{
	if (!vStayTillDecissionSetup)
		SelectAndExit(6);
	else
		SelectAndContinue(6);
	}
	else
	{
		ShowCurrentSelectionNotAvailableDialog();
	}
}

void CSelectItemDialog::OnSubFunction7Button() 
{
	if (vSubFunction7ButtonEnable == true)
	{
	if (!vStayTillDecissionSetup)
		SelectAndExit(7);
	else
		SelectAndContinue(7);
	}
	else
	{
		ShowCurrentSelectionNotAvailableDialog();
	}
}

void CSelectItemDialog::OnSubFunction8Button() 
{
	if (vSubFunction8ButtonEnable == true)
	{
	if (!vStayTillDecissionSetup)
		SelectAndExit(8);
	else
		SelectAndContinue(8);
	}
	else
	{
		ShowCurrentSelectionNotAvailableDialog();
	}
}

BOOL CSelectItemDialog::PreTranslateMessage(MSG* pMsg) 
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
		CWnd * TempWindow = NULL;
		int TempKey = pMsg->wParam;
		if (vGlobalShiftKeyDown)
		{
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
		}
		else
		if (TempKey != 16)
		if (((TempKey >= c0) && (TempKey <= c9)) || (TempKey == 192))
		{
			switch (TempKey)
			{
				case 192: 
						vQuickFindCharacter = '~'; 
				break;
				case c0: 
					if (vQuickFindCharacter == 'Y')
						vQuickFindCharacter = 'Z'; 
					else
						vQuickFindCharacter = 'Y'; 
				break;
				case c1: 
					if (vQuickFindCharacter == '9')
						vQuickFindCharacter = '0'; 
					else
					if ((vQuickFindCharacter >= '0') && (vQuickFindCharacter < '9'))
						vQuickFindCharacter++; 
					else
						vQuickFindCharacter = '0'; 
				break;
				case c2: 					
					if (vQuickFindCharacter == 'A')
						vQuickFindCharacter = 'B'; 
					else
					if (vQuickFindCharacter == 'B')
						vQuickFindCharacter = 'C'; 
					else
						vQuickFindCharacter = 'A'; 
				break;
				case c3: 					
					if (vQuickFindCharacter == 'D')
						vQuickFindCharacter = 'E'; 
					else
					if (vQuickFindCharacter == 'E')
						vQuickFindCharacter = 'F'; 
					else
						vQuickFindCharacter = 'D';
				break;
				case c4:
					if (vQuickFindCharacter == 'G')
						vQuickFindCharacter = 'H'; 
					else
					if (vQuickFindCharacter == 'H')
						vQuickFindCharacter = 'I'; 
					else
						vQuickFindCharacter = 'G';
				break;
				case c5:
					if (vQuickFindCharacter == 'J')
						vQuickFindCharacter = 'K'; 
					else
					if (vQuickFindCharacter == 'K')
						vQuickFindCharacter = 'L'; 
					else
						vQuickFindCharacter = 'J';
				break;
				case c6:
					if (vQuickFindCharacter == 'M')
						vQuickFindCharacter = 'N'; 
					else
					if (vQuickFindCharacter == 'N')
						vQuickFindCharacter = 'O'; 
					else
						vQuickFindCharacter = 'M';
				break;
				case c7:
					if (vQuickFindCharacter == 'P')
						vQuickFindCharacter = 'Q'; 
					else
					if (vQuickFindCharacter == 'Q')
						vQuickFindCharacter = 'R'; 
					else
						vQuickFindCharacter = 'P';
				break;
				case c8:
					if (vQuickFindCharacter == 'S')
						vQuickFindCharacter = 'T'; 
					else
					if (vQuickFindCharacter == 'T')
						vQuickFindCharacter = 'U'; 
					else
						vQuickFindCharacter = 'S';
				break;
				case c9:
					if (vQuickFindCharacter == 'V')
						vQuickFindCharacter = 'W'; 
					else
					if (vQuickFindCharacter == 'W')
						vQuickFindCharacter = 'X'; 
					else
						vQuickFindCharacter = 'V';
				break;
			}

			//vCurrentFirstItemNumberShown = 3;
			if (vNumberOfItems > 8)
			{
//					ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempTextD;
//					ISelectItemDialog.vNumberOfItems++
				CString TempCurrentItemName = vItems[vCurrentFirstItemNumberShown];
				CString Temp2CurrentCharacter;
				char ShortString[6];
				ShortString[0] = vQuickFindCharacter;
				ShortString[1] = 0;
				Temp2CurrentCharacter = ShortString;
				
				
				
				if (TempCurrentItemName != "")
				{
					while ((vCurrentFirstItemNumberShown + 7 < vNumberOfItems) &&
						(TempCurrentItemName.CompareNoCase(Temp2CurrentCharacter) < 0))
					{
						vCurrentFirstItemNumberShown++;
						if (vCurrentFirstItemNumberShown + 7 < vNumberOfItems)
						{
							TempCurrentItemName = vItems[vCurrentFirstItemNumberShown];
						}
					}
				}
				if (vCurrentFirstItemNumberShown + 7 < vNumberOfItems)
				{
					vCurrentFirstItemNumberShown++;
				}
				else
				{
					vCurrentFirstItemNumberShown = vNumberOfItems - 7;
				}
				UpdateItemDisplay();
			}
		}
		else
		switch (TempKey)
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
			case 16: // Shift Key
			case 0x60:  //0 key on KeyPad
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
			break;
			case 36:  //Home key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
			case cPageDown: // Help
				PostThreadMessage(vGlobalCallingThreadID,
					cShowHelp, 29, 0);
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
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

HBRUSH CSelectItemDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		if (vGreenItem == 0 + vCurrentFirstItemNumberShown)
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction1ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vGreenBrush;
		}
		if (vYellowItem == (0 + vCurrentFirstItemNumberShown))
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction1ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vYellowBrush;
		}
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction1ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		if (vGreenItem == 1 + vCurrentFirstItemNumberShown)
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction2ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vGreenBrush;
		}
		if (vYellowItem == (1 + vCurrentFirstItemNumberShown))
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction2ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vYellowBrush;
		}
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction2ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
		if (vGreenItem == 2 + vCurrentFirstItemNumberShown)
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction3ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vGreenBrush;
		}
		if (vYellowItem == (2 + vCurrentFirstItemNumberShown))
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction3ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vYellowBrush;
		}
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction3ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);
		if (vGreenItem == 3 + vCurrentFirstItemNumberShown)
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction4ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vGreenBrush;
		}
		if (vYellowItem == (3 + vCurrentFirstItemNumberShown))
		{
			if (vSubFunction4ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			pDC->SetTextColor(cButtonTextColor);
			return vYellowBrush;
		}
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction4ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  
		if (vGreenItem == 4 + vCurrentFirstItemNumberShown)
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction5ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vGreenBrush;
		}
		if (vYellowItem == (4 + vCurrentFirstItemNumberShown))
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction5ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vYellowBrush;
		}
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction5ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Button)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5);  
		if (vGreenItem == 5 + vCurrentFirstItemNumberShown)
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction6ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vGreenBrush;
		}
		if (vYellowItem == (5 + vCurrentFirstItemNumberShown))
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction6ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vYellowBrush;
		}
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction6ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);

		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction7ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);

		if (vGreenItem == 6 + vCurrentFirstItemNumberShown)
			return vGreenBrush;
		if (vYellowItem == (6 + vCurrentFirstItemNumberShown))
			return vYellowBrush;
		else
			return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5); 
		if (vGreenItem == 7 + vCurrentFirstItemNumberShown)
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction8ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vGreenBrush;
		}

		if (vYellowItem == (7 + vCurrentFirstItemNumberShown))
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction8ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vYellowBrush;
		}
		else
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction8ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText1, 8);  //5 is medium large
		
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticTextBox2, 5); 
		if (vShowExit)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vYellowBrush;
		}

		pDC->SetBkMode(TRANSPARENT);
		return vLightBackgroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText3) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticTextBox2, 5); 
		if (vMakeTitle3Big)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cGreen);

			return vYellowBrush;
		}
		if (vMakeTitle3Highlighted)
		{
			pDC->SetTextColor(cDarkBlue);
			pDC->SetBkMode(TRANSPARENT);
			return vYellowBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		return vLightBackgroundBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLightBackgroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText5)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticTextBox5, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLightBackgroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText4) 
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticTextBox4, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLightBackgroundBrush;
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
		if (vInAutoSetup)
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
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

	return hbr;
}

void CSelectItemDialog::SelectAndExit(BYTE TempItemNumber) 
{
	if ((TempItemNumber > 0) && 
		(vCurrentFirstItemNumberShown - 1 + TempItemNumber <= vNumberOfItems))
	{
		//figure out what product number is selected by the button press
		if (vItemEnabled[vCurrentFirstItemNumberShown - 2 + TempItemNumber])
		{
			vItemNumberSelected = vCurrentFirstItemNumberShown - 1 + TempItemNumber;
			CDialog::EndDialog(true);
		}
		else
		if (vItems[vCurrentFirstItemNumberShown - 2 + TempItemNumber] == "Void")
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\nTo add a Void inspection\nVoid inspections must be enabled in\nSystem Setup, Setup Image Handling";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
	}
	/*
	else
	{
		CNoticeDialog TempNoticeDialog;
		CString TempText("");
		TempText.LoadString(IDS_NoItemSelected);
		TempNoticeDialog.vNoticeText = TempText;
		TempNoticeDialog.vType = cWarningMessage;
		TempNoticeDialog.DoModal();
	}
	*/
}

void CSelectItemDialog::SelectAndContinue(BYTE TempItemNumber) 
{
	if ((TempItemNumber > 0) && 
		(vCurrentFirstItemNumberShown - 1 + TempItemNumber <= vNumberOfItems))
	{
		//figure out what product number is selected by the button press
		if (vItemEnabled[vCurrentFirstItemNumberShown - 2 + TempItemNumber])
		{
			vItemNumberSelected = vCurrentFirstItemNumberShown - 1 + TempItemNumber;
//			CDialog::EndDialog(true);
			CString TempString = vStringItemSelecting + vItems[TempItemNumber-1];
			SetDlgItemText(IDC_DialogTitleStaticText2,TempString );
			SetDlgItemText(IDC_Function2Button, _T("Save\n and Exit"));
			m_Function2Button.ShowWindow(SW_SHOW);
		}
		else
		if (vItems[vCurrentFirstItemNumberShown - 2 + TempItemNumber] == "Void")
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\nTo add a Void inspection\nVoid inspections must be enabled in\nSystem Setup, Setup Image Handling";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
	}
	/*
	else
	{
		CNoticeDialog TempNoticeDialog;
		CString TempText("");
		TempText.LoadString(IDS_NoItemSelected);
		TempNoticeDialog.vNoticeText = TempText;
		TempNoticeDialog.vType = cWarningMessage;
		TempNoticeDialog.DoModal();
	}
	*/
}

void CSelectItemDialog::UpdateItemDisplay()
{
	CWnd* pWnd;
	CDC* pDC;
	HWND TempDialogHwnd = this->m_hWnd;

	CString TempItemName("");
	CString TempBlankSpace(" ");
	WORD TempLength = 0;
	bool TempEnable = true;
	for (WORD TempLoop = 1; TempLoop < 9;  TempLoop++)
	{
		if (vCurrentFirstItemNumberShown + TempLoop - 2 < vNumberOfItems)
		{
			if (TempLoop > 4)
				TempItemName = vItems[TempLoop + vCurrentFirstItemNumberShown - 2] + TempBlankSpace;
			else
				TempItemName =  TempBlankSpace + vItems[TempLoop + vCurrentFirstItemNumberShown - 2];

			TempLength = TempItemName.GetLength();
		}
		else
			TempLength = 0;

		TempEnable = vItemEnabled[TempLoop + vCurrentFirstItemNumberShown - 2];

		switch (TempLoop)
		{
			case 1:
				if (!TempLength)
				{
					m_SubFunction1Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction1Button.ShowWindow(SW_SHOW);

					SetDlgItemText(IDC_SubFunction1Button,TempItemName);
					vSubFunction1ButtonEnable = TempEnable;	//m_SubFunction1Button.EnableWindow(TempEnable); 		

					pDC = m_SubFunction1Button.GetDC();
					pWnd = &m_SubFunction1Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5) + 30;  //5 is medium large
					if (vYellowItem == (0 + vCurrentFirstItemNumberShown))
						m_SubFunction1Button.SetWindowPos(NULL, 11, 52, (int)(TempStringLength / vGlobalPercentToResizeY), 56, SWP_NOZORDER);
					else
						m_SubFunction1Button.SetWindowPos(NULL, 11, 52, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
			case 2:
				if (!TempLength)
				{
					m_SubFunction2Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction2Button.ShowWindow(SW_SHOW);
					//if (vYellowItem == (1 + vCurrentFirstItemNumberShown))
					//	m_SubFunction2Button.SetWindowPos(NULL,11,142,TempLength,56,SWP_NOZORDER);
					//else
					//	m_SubFunction2Button.SetWindowPos(NULL,11,142,TempLength,28,SWP_NOZORDER);
					SetDlgItemText(IDC_SubFunction2Button,TempItemName);
					vSubFunction2ButtonEnable = TempEnable;	//m_SubFunction2Button.EnableWindow(TempEnable);

					pDC = m_SubFunction2Button.GetDC();
					pWnd = &m_SubFunction2Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5) + 30;  //5 is medium large
					if (vYellowItem == (1 + vCurrentFirstItemNumberShown))
						m_SubFunction2Button.SetWindowPos(NULL, 11, 142, (int)(TempStringLength / vGlobalPercentToResizeY), 56, SWP_NOZORDER);
					else
						m_SubFunction2Button.SetWindowPos(NULL, 11, 142, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
			case 3:
				if (!TempLength)
				{
					m_SubFunction3Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction3Button.ShowWindow(SW_SHOW);
					//if (vYellowItem == (2 + vCurrentFirstItemNumberShown))
					//	m_SubFunction3Button.SetWindowPos(NULL,11,232,TempLength,56,SWP_NOZORDER);
					//else
					//	m_SubFunction3Button.SetWindowPos(NULL,11,232,TempLength,28,SWP_NOZORDER);
					SetDlgItemText(IDC_SubFunction3Button,TempItemName);
					vSubFunction3ButtonEnable = TempEnable;	//m_SubFunction3Button.EnableWindow(TempEnable);

					pDC = m_SubFunction3Button.GetDC();
					pWnd = &m_SubFunction3Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5) + 30;  //5 is medium large
					if (vYellowItem == (2 + vCurrentFirstItemNumberShown))
						m_SubFunction3Button.SetWindowPos(NULL, 11, 232, (int)(TempStringLength / vGlobalPercentToResizeY), 56, SWP_NOZORDER);
					else
						m_SubFunction3Button.SetWindowPos(NULL, 11, 232, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
			case 4:
				if (!TempLength)
				{
					m_SubFunction4Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction4Button.ShowWindow(SW_SHOW);
					//if (vYellowItem == (3 + vCurrentFirstItemNumberShown))
					//	m_SubFunction4Button.SetWindowPos(NULL,11,327,TempLength,56,SWP_NOZORDER);
					//else
					//	m_SubFunction4Button.SetWindowPos(NULL,11,327,TempLength,28,SWP_NOZORDER);
					SetDlgItemText(IDC_SubFunction4Button,TempItemName);
					vSubFunction4ButtonEnable = TempEnable;	//m_SubFunction4Button.EnableWindow(TempEnable);

					pDC = m_SubFunction4Button.GetDC();
					pWnd = &m_SubFunction4Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5) + 30;  //5 is medium large
					if (vYellowItem == (3 + vCurrentFirstItemNumberShown))
						m_SubFunction4Button.SetWindowPos(NULL, 11, 327, (int)(TempStringLength / vGlobalPercentToResizeY), 56, SWP_NOZORDER);
					else
						m_SubFunction4Button.SetWindowPos(NULL, 11, 327, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
			case 5:
				if (!TempLength)
				{
					m_SubFunction5Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction5Button.ShowWindow(SW_SHOW);
					//if (vYellowItem == (4 + vCurrentFirstItemNumberShown))
					//	m_SubFunction5Button.SetWindowPos(NULL,623 - TempLength,79,TempLength,56,SWP_NOZORDER);
					//else
					//	m_SubFunction5Button.SetWindowPos(NULL,623 - TempLength,79,TempLength,28,SWP_NOZORDER);
					SetDlgItemText(IDC_SubFunction5Button,TempItemName);
					vSubFunction5ButtonEnable = TempEnable;	//m_SubFunction5Button.EnableWindow(TempEnable);

					pDC = m_SubFunction5Button.GetDC();
					pWnd = &m_SubFunction5Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5) + 30;  //5 is medium large
					if (vYellowItem == (4 + vCurrentFirstItemNumberShown))
						m_SubFunction5Button.SetWindowPos(NULL, 623 - (int)(TempStringLength / vGlobalPercentToResizeY), 79, (int)(TempStringLength / vGlobalPercentToResizeY), 56, SWP_NOZORDER);
					else
						m_SubFunction5Button.SetWindowPos(NULL, 623 - (int)(TempStringLength / vGlobalPercentToResizeY), 79, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
			case 6:
				if (!TempLength)
				{
					m_SubFunction6Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction6Button.ShowWindow(SW_SHOW);
					//if (vYellowItem == (5 + vCurrentFirstItemNumberShown))
					//	m_SubFunction6Button.SetWindowPos(NULL,623 - TempLength,169,TempLength,56,SWP_NOZORDER);
					//else
					//	m_SubFunction6Button.SetWindowPos(NULL,623 - TempLength,169,TempLength,28,SWP_NOZORDER);
					SetDlgItemText(IDC_SubFunction6Button,TempItemName);
					vSubFunction6ButtonEnable = TempEnable;	//m_SubFunction6Button.EnableWindow(TempEnable);

					pDC = m_SubFunction6Button.GetDC();
					pWnd = &m_SubFunction6Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5) + 30;  //5 is medium large
					if (vYellowItem == (5 + vCurrentFirstItemNumberShown))
						m_SubFunction6Button.SetWindowPos(NULL, 623 - (int)(TempStringLength / vGlobalPercentToResizeY), 169, (int)(TempStringLength / vGlobalPercentToResizeY), 56, SWP_NOZORDER);
					else
						m_SubFunction6Button.SetWindowPos(NULL, 623 - (int)(TempStringLength / vGlobalPercentToResizeY), 169, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
			case 7:
				if (!TempLength)
				{
					m_SubFunction7Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction7Button.ShowWindow(SW_SHOW);
					//if (vYellowItem == (6 + vCurrentFirstItemNumberShown))
					//	m_SubFunction7Button.SetWindowPos(NULL,623 - TempLength,259,TempLength,56,SWP_NOZORDER);
					//else
					//	m_SubFunction7Button.SetWindowPos(NULL,623 - TempLength,259,TempLength,28,SWP_NOZORDER);
					SetDlgItemText(IDC_SubFunction7Button,TempItemName);
					vSubFunction7ButtonEnable = TempEnable;	//m_SubFunction7Button.EnableWindow(TempEnable);

					pDC = m_SubFunction7Button.GetDC();
					pWnd = &m_SubFunction7Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5) + 30;  //5 is medium large
					if (vYellowItem == (6 + vCurrentFirstItemNumberShown))
						m_SubFunction7Button.SetWindowPos(NULL, 623 - (int)(TempStringLength / vGlobalPercentToResizeY), 259, (int)(TempStringLength / vGlobalPercentToResizeY), 56, SWP_NOZORDER);
					else
						m_SubFunction7Button.SetWindowPos(NULL, 623 - (int)(TempStringLength / vGlobalPercentToResizeY), 259, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
			case 8:
				if (!TempLength)
				{
					m_SubFunction8Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction8Button.ShowWindow(SW_SHOW);
					//if (vYellowItem == (7 + vCurrentFirstItemNumberShown))
					//	m_SubFunction8Button.SetWindowPos(NULL,623 - TempLength,354,TempLength,56,SWP_NOZORDER);
					//else
					//	m_SubFunction8Button.SetWindowPos(NULL,623 - TempLength,354,TempLength,28,SWP_NOZORDER);
					SetDlgItemText(IDC_SubFunction8Button,TempItemName);
					vSubFunction8ButtonEnable = TempEnable;	//m_SubFunction8Button.EnableWindow(TempEnable);

					pDC = m_SubFunction8Button.GetDC();
					pWnd = &m_SubFunction8Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5) + 30;  //5 is medium large
					if (vYellowItem == (7 + vCurrentFirstItemNumberShown))
						m_SubFunction8Button.SetWindowPos(NULL, 623 - (int)(TempStringLength / vGlobalPercentToResizeY), 354, (int)(TempStringLength / vGlobalPercentToResizeY), 56, SWP_NOZORDER);
					else
						m_SubFunction8Button.SetWindowPos(NULL, 623 - (int)(TempStringLength / vGlobalPercentToResizeY), 354, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
		}
	}
	if (vNumberOfItems > vCurrentFirstItemNumberShown + 7)
	{
		m_Function5Button.ShowWindow(SW_SHOW);
		//SetDlgItemText(IDC_Function5Button,"Next");
	}
	else
	{
		m_Function5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
		//SetDlgItemText(IDC_Function5Button,"");
	}

	if (vCurrentFirstItemNumberShown > 1)
	{
		m_Function3Button.ShowWindow(SW_SHOW);
		//SetDlgItemText(IDC_Function3Button,"Previous");
	}
	else
	{
		m_Function3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
		//SetDlgItemText(IDC_Function3Button,"");
	}
	::SetupMenu(vLocalDisplayCollection);
}

void CSelectItemDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == vSetWindowActiveTimerHandle)
	{
		KillTimer(vSetWindowActiveTimerHandle);
		this->SetForegroundWindow();
	}
	CDialog::OnTimer(nIDEvent);
}

void CSelectItemDialog::ShowCurrentSelectionNotAvailableDialog()
{
	CNoticeDialog TempNoticeDialog;
	TempNoticeDialog.vNoticeText = "\n\n\nCurrent Selection Not Available";
	TempNoticeDialog.vType = cErrorMessage;
	TempNoticeDialog.DoModal();
}

void CSelectItemDialog::OnDialogtitlestatictext1()
{
	//bring up soft key menu for help, print screen, Image, Clear, and hidden shift key
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
