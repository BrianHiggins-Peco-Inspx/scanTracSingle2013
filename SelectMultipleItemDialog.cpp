//ScanTrac Side View Source File
// SelectMultipleItemDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "SelectMultipleItemDialog.h"

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
// CSelectMultipleItemDialog dialog


CSelectMultipleItemDialog::CSelectMultipleItemDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectMultipleItemDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectMultipleItemDialog)
	m_DialogTitleStaticText = _T("");
	//}}AFX_DATA_INIT
	vOutboardItemMustBeDisabled = false;
	vCurrentFirstItemNumberShown = 0;
	for (WORD TempLoop = 0; TempLoop < cMaxMultipleSelectDialogItems; TempLoop++)
		vItemEnabled[TempLoop] = false;
	vUseSelectText = false;
	vChangeMade = false;
	vInAutoSetup = 0;
	vNumberOfItems = 2;
	vGreenBrush = ::CreateSolidBrush(cGreen);
	vYellowBrush = ::CreateSolidBrush(cYellow);
	vLightGreenBrush = ::CreateSolidBrush(cLightGreen);

	for (WORD TempLoop = 0; TempLoop < cMaxMultipleSelectDialogItems; TempLoop++)
		vItems[TempLoop] = 0;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticTextBox);
	vLocalCWndCollection.Add(&m_Function1Button);

	vLocalDisplayCollection.Add(&m_CheckMarkGraphic8);
	vLocalDisplayCollection.Add(&m_CheckMarkGraphic7);
	vLocalDisplayCollection.Add(&m_CheckMarkGraphic6);
	vLocalDisplayCollection.Add(&m_CheckMarkGraphic5);
	vLocalDisplayCollection.Add(&m_CheckMarkGraphic4);
	vLocalDisplayCollection.Add(&m_CheckMarkGraphic3);
	vLocalDisplayCollection.Add(&m_CheckMarkGraphic2);
	vLocalDisplayCollection.Add(&m_CheckMarkGraphic1);
	vLocalDisplayCollection.Add(&m_Item8);
	vLocalDisplayCollection.Add(&m_Item7);
	vLocalDisplayCollection.Add(&m_Item6);
	vLocalDisplayCollection.Add(&m_Item5);
	vLocalDisplayCollection.Add(&m_Item4);
	vLocalDisplayCollection.Add(&m_Item3);
	vLocalDisplayCollection.Add(&m_Item2);
	vLocalDisplayCollection.Add(&m_Item1);
}

CSelectMultipleItemDialog::~CSelectMultipleItemDialog()
{
	DeleteObject(vGreenBrush);
	DeleteObject(vYellowBrush);
	DeleteObject(vLightGreenBrush);
}

void CSelectMultipleItemDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

	m_CheckMarkGraphic1.GetWindowRect(&TempRect);

	if (ResizeXCoor(1) >= 2)
	{
		vGraphicBitmap.LoadBitmap(IDB_GreenBackgroundCheckMarkGraphic4xBigger);
		m_CheckMarkGraphic1.SetBitmap(vGraphicBitmap);
		m_CheckMarkGraphic2.SetBitmap(vGraphicBitmap);
		m_CheckMarkGraphic3.SetBitmap(vGraphicBitmap);
		m_CheckMarkGraphic4.SetBitmap(vGraphicBitmap);
		m_CheckMarkGraphic5.SetBitmap(vGraphicBitmap);
		m_CheckMarkGraphic6.SetBitmap(vGraphicBitmap);
		m_CheckMarkGraphic7.SetBitmap(vGraphicBitmap);
		m_CheckMarkGraphic8.SetBitmap(vGraphicBitmap);
	}
	else if (ResizeXCoor(10) >= 15)
	{
		vGraphicBitmap.LoadBitmap(IDB_GreenBackgroundCheckMarkGraphic2xBigger);
		m_CheckMarkGraphic1.SetBitmap(vGraphicBitmap);
		m_CheckMarkGraphic2.SetBitmap(vGraphicBitmap);
		m_CheckMarkGraphic3.SetBitmap(vGraphicBitmap);
		m_CheckMarkGraphic4.SetBitmap(vGraphicBitmap);
		m_CheckMarkGraphic5.SetBitmap(vGraphicBitmap);
		m_CheckMarkGraphic6.SetBitmap(vGraphicBitmap);
		m_CheckMarkGraphic7.SetBitmap(vGraphicBitmap);
		m_CheckMarkGraphic8.SetBitmap(vGraphicBitmap);
	}
	vChangeMade = false;
	CString TempText("");
	if (vInAutoSetup)
	{
		m_Function2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());
		m_Function5Button.ShowWindow(SW_SHOW);
		m_Function5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		TempText.LoadString(IDS_NextAutoSetupStep);
		SetDlgItemText(IDC_Function5Button,TempText);
		m_Function4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_Function4Button.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_PreviousAutoSetup);
		SetDlgItemText(IDC_Function4Button,TempText);
	}
	else
	{
		TempText.LoadString(IDS_Exit);
		SetDlgItemText(IDC_Function2Button,TempText);
		TempText.LoadString(IDS_Cancel);
		SetDlgItemText(IDC_Function4Button,TempText);
	}
	if (vNumberOfItems > cMaxMultipleSelectDialogItems)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nThere were too many items to choose from.\nSome were omitted from this list.";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();

		vNumberOfItems = cMaxMultipleSelectDialogItems;
	}
	for (WORD TempLoop = 0; TempLoop < vNumberOfItems; TempLoop++)
		vItemEnabled[TempLoop] = true;
	TempText.LoadString(IDS_MainMenu);
	SetDlgItemText(IDC_Function1Button,TempText);
	
	if (vNumberOfItems > 2)
	{
		m_SubFunction3Button.ShowWindow(SW_SHOW);
		m_Item3.ShowWindow(SW_SHOW);
	}
	if (vNumberOfItems > 3)
	{
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		m_Item4.ShowWindow(SW_SHOW);
	}
	if (vNumberOfItems > 4)
	{
		m_SubFunction5Button.ShowWindow(SW_SHOW);
		m_Item5.ShowWindow(SW_SHOW);
	}
	if (vNumberOfItems > 5)
	{
		m_SubFunction6Button.ShowWindow(SW_SHOW);
		m_Item6.ShowWindow(SW_SHOW);
	}
	if (vNumberOfItems > 6)
	{
		m_SubFunction7Button.ShowWindow(SW_SHOW);
		m_Item7.ShowWindow(SW_SHOW);
	}
	if (vNumberOfItems > 7)
	{
		m_SubFunction8Button.ShowWindow(SW_SHOW);
		m_Item8.ShowWindow(SW_SHOW);
	}
	::SetupMenu(vLocalCWndCollection);
	UpdateItemDisplay();
	this->SetWindowText(_T("Select Multiple Items"));
}

void CSelectMultipleItemDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectMultipleItemDialog)
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_CheckMarkGraphic8, m_CheckMarkGraphic8);
	DDX_Control(pDX, IDC_CheckMarkGraphic7, m_CheckMarkGraphic7);
	DDX_Control(pDX, IDC_CheckMarkGraphic6, m_CheckMarkGraphic6);
	DDX_Control(pDX, IDC_CheckMarkGraphic5, m_CheckMarkGraphic5);
	DDX_Control(pDX, IDC_CheckMarkGraphic4, m_CheckMarkGraphic4);
	DDX_Control(pDX, IDC_CheckMarkGraphic3, m_CheckMarkGraphic3);
	DDX_Control(pDX, IDC_CheckMarkGraphic2, m_CheckMarkGraphic2);
	DDX_Control(pDX, IDC_CheckMarkGraphic1, m_CheckMarkGraphic1);
	DDX_Control(pDX, IDC_Item8, m_Item8);
	DDX_Control(pDX, IDC_Item7, m_Item7);
	DDX_Control(pDX, IDC_Item6, m_Item6);
	DDX_Control(pDX, IDC_Item5, m_Item5);
	DDX_Control(pDX, IDC_Item4, m_Item4);
	DDX_Control(pDX, IDC_Item3, m_Item3);
	DDX_Control(pDX, IDC_Item2, m_Item2);
	DDX_Control(pDX, IDC_Item1, m_Item1);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticTextBox);
	DDX_Text(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDV_MaxChars(pDX, m_DialogTitleStaticText, 50);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
}


BEGIN_MESSAGE_MAP(CSelectMultipleItemDialog, CDialog)
	//{{AFX_MSG_MAP(CSelectMultipleItemDialog)
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
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CSelectMultipleItemDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectMultipleItemDialog message handlers

void CSelectMultipleItemDialog::OnFunction1Button() 
{
	// Main Menu Button was pressed
	if (vInAutoSetup)
	{
		if (AbortSetupChanges())
			CDialog::EndDialog(10);
	}
	else
	//if ((!vChangeMade) || (AbortSetupChanges()))
	{
		CDialog::EndDialog(10);
	}
}

void CSelectMultipleItemDialog::OnFunction2Button() 
{
	if (vInAutoSetup != 1)
	{
		CDialog::EndDialog(true);
	}
}

void CSelectMultipleItemDialog::OnFunction3Button() 
{
	//Previous Group button was pressed
  if (vCurrentFirstItemNumberShown > 1)
  {
		if (vCurrentFirstItemNumberShown > 8)
      vCurrentFirstItemNumberShown = vCurrentFirstItemNumberShown - 8;
		else
			vCurrentFirstItemNumberShown = 0;
		UpdateItemDisplay();
  }	
	this->Invalidate();
}

void CSelectMultipleItemDialog::OnFunction4Button() 
{
	//Cancel was pressed
	if (vInAutoSetup == 1)
		CDialog::EndDialog(true);
	else
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::OnCancel();
}

void CSelectMultipleItemDialog::OnFunction5Button() 
{
	if (vInAutoSetup == 1)
	{
		CDialog::EndDialog(true);
	}
	else
	//next button pressed to display next group of products
	if (vNumberOfItems > vCurrentFirstItemNumberShown + 7)
	if (vCurrentFirstItemNumberShown + 7 < vNumberOfItems)
	{
		//if (vCurrentFirstItemNumberShown + 15 > vNumberOfItems)
		//	vCurrentFirstItemNumberShown = vNumberOfItems - 8;
		//else
			vCurrentFirstItemNumberShown = vCurrentFirstItemNumberShown + 8;
		UpdateItemDisplay();
		this->Invalidate();
	}
}

void CSelectMultipleItemDialog::OnSubFunction1Button() 
{
	if (vNumberOfItems > 0 + vCurrentFirstItemNumberShown)
	{
		if (vItemEnabled[0 + vCurrentFirstItemNumberShown])
		if (vItems[0 + vCurrentFirstItemNumberShown])
		{
			if (!IsThereAnOutwardItemEnabled(0 + vCurrentFirstItemNumberShown))
				vItems[0 + vCurrentFirstItemNumberShown] = false;
		}
		else
		{
			if (!IsThereAnInwardItemDisabled(0 + vCurrentFirstItemNumberShown))
				vItems[0 + vCurrentFirstItemNumberShown] = true;
		}

		SetChangeMade();
		UpdateItemDisplay();
	}
}

void CSelectMultipleItemDialog::OnSubFunction2Button() 
{
	if (vNumberOfItems > 1 + vCurrentFirstItemNumberShown)
	{
		if (vItemEnabled[1 + vCurrentFirstItemNumberShown])
		if (vItems[1 + vCurrentFirstItemNumberShown])
		{
			if (!IsThereAnOutwardItemEnabled(1 + vCurrentFirstItemNumberShown))
				vItems[1 + vCurrentFirstItemNumberShown] = false;
		}
		else
		{
			if (!IsThereAnInwardItemDisabled(1 + vCurrentFirstItemNumberShown))
				vItems[1 + vCurrentFirstItemNumberShown] = true;
		}

		SetChangeMade();
		UpdateItemDisplay();
	}
}

void CSelectMultipleItemDialog::OnSubFunction3Button() 
{
	if (vNumberOfItems > 2 + vCurrentFirstItemNumberShown)
	{
		if (vItemEnabled[2 + vCurrentFirstItemNumberShown])
		if (vItems[2 + vCurrentFirstItemNumberShown])
		{
			if (!IsThereAnOutwardItemEnabled(2 + vCurrentFirstItemNumberShown))
				vItems[2 + vCurrentFirstItemNumberShown] = false;
		}
		else
		{
			if (!IsThereAnInwardItemDisabled(2 + vCurrentFirstItemNumberShown))
				vItems[2 + vCurrentFirstItemNumberShown] = true;
		}

		SetChangeMade();
		UpdateItemDisplay();
	}
}

void CSelectMultipleItemDialog::OnSubFunction4Button() 
{
	if (vNumberOfItems > 3 + vCurrentFirstItemNumberShown)
	{
		if (vItemEnabled[3 + vCurrentFirstItemNumberShown])
		if (vItems[3 + vCurrentFirstItemNumberShown])
		{
			if (!IsThereAnOutwardItemEnabled(3 + vCurrentFirstItemNumberShown))
				vItems[3 + vCurrentFirstItemNumberShown] = false;
		}
		else
		{
			if (!IsThereAnInwardItemDisabled(3 + vCurrentFirstItemNumberShown))
				vItems[3 + vCurrentFirstItemNumberShown] = true;
		}

		SetChangeMade();
		UpdateItemDisplay();
	}
}

void CSelectMultipleItemDialog::OnSubFunction5Button() 
{
	if (vNumberOfItems > 4 + vCurrentFirstItemNumberShown)
	{
		if (vItemEnabled[4 + vCurrentFirstItemNumberShown])
		if (vItems[4 + vCurrentFirstItemNumberShown])
		{
			if (!IsThereAnOutwardItemEnabled(4 + vCurrentFirstItemNumberShown))
				vItems[4 + vCurrentFirstItemNumberShown] = false;
		}
		else
		{
			if (!IsThereAnInwardItemDisabled(4 + vCurrentFirstItemNumberShown))
				vItems[4 + vCurrentFirstItemNumberShown] = true;
		}

		SetChangeMade();
		UpdateItemDisplay();
	}
}

void CSelectMultipleItemDialog::OnSubFunction6Button() 
{
	if (vNumberOfItems > 5 + vCurrentFirstItemNumberShown)
	{
		if (vItemEnabled[5 + vCurrentFirstItemNumberShown])
		if (vItems[5 + vCurrentFirstItemNumberShown])
		{
			if (!IsThereAnOutwardItemEnabled(5 + vCurrentFirstItemNumberShown))
				vItems[5 + vCurrentFirstItemNumberShown] = false;
		}
		else
		{
			if (!IsThereAnInwardItemDisabled(5 + vCurrentFirstItemNumberShown))
				vItems[5 + vCurrentFirstItemNumberShown] = true;
		}

		SetChangeMade();
		UpdateItemDisplay();
	}
}

void CSelectMultipleItemDialog::OnSubFunction7Button() 
{
	if (vNumberOfItems > 6 + vCurrentFirstItemNumberShown)
	{
		if (vItemEnabled[6 + vCurrentFirstItemNumberShown])
		if (vItems[6 + vCurrentFirstItemNumberShown])
		{
			if (!IsThereAnOutwardItemEnabled(6 + vCurrentFirstItemNumberShown))
				vItems[6 + vCurrentFirstItemNumberShown] = false;
		}
		else
		{
			if (!IsThereAnInwardItemDisabled(6 + vCurrentFirstItemNumberShown))
				vItems[6 + vCurrentFirstItemNumberShown] = true;
		}

		SetChangeMade();
		UpdateItemDisplay();
	}
}

void CSelectMultipleItemDialog::OnSubFunction8Button() 
{
	if (vNumberOfItems > 7 + vCurrentFirstItemNumberShown)
	{
		if (vItemEnabled[7 + vCurrentFirstItemNumberShown])
		if (vItems[7 + vCurrentFirstItemNumberShown])
		{
			if (!IsThereAnOutwardItemEnabled(7 + vCurrentFirstItemNumberShown))
				vItems[7 + vCurrentFirstItemNumberShown] = false;
		}
		else
		{
			if (!IsThereAnInwardItemDisabled(7 + vCurrentFirstItemNumberShown))
				vItems[7 + vCurrentFirstItemNumberShown] = true;
		}

		SetChangeMade();
		UpdateItemDisplay();
	}
}

BOOL CSelectMultipleItemDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd * TempWindow = NULL;
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
		int TempKey = pMsg->wParam;
		if (vGlobalShiftKeyDown)
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
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
					cShowHelp, 46, 0);
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

HBRUSH CSelectMultipleItemDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticTextBox, 8);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

  if (pWnd->GetDlgCtrlID() == IDC_Item1)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Item1, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vItems[0 + vCurrentFirstItemNumberShown])
			return vGreenBrush;
	}
 
  if (pWnd->GetDlgCtrlID() == IDC_Item2)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Item2, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vItems[1 + vCurrentFirstItemNumberShown])
			return vGreenBrush;
	}
 
  if (pWnd->GetDlgCtrlID() == IDC_Item3)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Item3, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vItems[2 + vCurrentFirstItemNumberShown])
			return vGreenBrush;
	}
 
  if (pWnd->GetDlgCtrlID() == IDC_Item4)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Item4, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vItems[3 + vCurrentFirstItemNumberShown])
			return vGreenBrush;
	}
 
  if (pWnd->GetDlgCtrlID() == IDC_Item5)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Item5, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vItems[4 + vCurrentFirstItemNumberShown])
			return vGreenBrush;
	}
 
  if (pWnd->GetDlgCtrlID() == IDC_Item6)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Item6, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vItems[5 + vCurrentFirstItemNumberShown])
			return vGreenBrush;
	}
 
  if (pWnd->GetDlgCtrlID() == IDC_Item7)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Item7, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vItems[6 + vCurrentFirstItemNumberShown])
			return vGreenBrush;
	}
 
  if (pWnd->GetDlgCtrlID() == IDC_Item8)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Item8, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vItems[7 + vCurrentFirstItemNumberShown])
			return vGreenBrush;
	}
 
	if (pWnd->GetDlgCtrlID() == IDC_Function5Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		if (vInAutoSetup == 1)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vGreenBrush;
		}
	}

	if ((pWnd->GetDlgCtrlID() == IDC_Background) ||
		(pWnd->GetDlgCtrlID() == IDC_Title1) ||
		(pWnd->GetDlgCtrlID() == IDC_Title2) ||
		(pWnd->GetDlgCtrlID() == IDC_Title4) ||
		(pWnd->GetDlgCtrlID() == IDC_Title6) ||
		(pWnd->GetDlgCtrlID() == IDC_Title8))
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLightGreenBrush;
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
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Button) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	return hbr;
}

void CSelectMultipleItemDialog::SetChangeMade() 
{
	if (!vChangeMade)
	{
		CString TempText("");
		vChangeMade = true;
		if (!vInAutoSetup)
		{
			m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			TempText = "Save and Exit";
			SetDlgItemText(IDC_Function2Button,TempText);
			m_Function4Button.ShowWindow(SW_SHOW);
			TempText.LoadString(IDS_Cancel);
			SetDlgItemText(IDC_Function4Button,TempText);
		}
	}
}

void CSelectMultipleItemDialog::UpdateItemDisplay() 
{
	CWnd* pWnd;
	CDC* pDC;
	HWND TempDialogHwnd = this->m_hWnd;

	BYTE TempNumberOfItemsDisplaying = vNumberOfItems;
	if (TempNumberOfItemsDisplaying > 8)
		TempNumberOfItemsDisplaying = 8;

	for (BYTE TempLoop = 0; TempLoop < TempNumberOfItemsDisplaying; TempLoop++)
	{
		CString TempButtonString("");
		if (vUseSelectText)
			TempButtonString.LoadString(IDS_Select);
		else
			TempButtonString.LoadString(IDS_Enable);
		CString TempBoxString(vItemsNo[TempLoop + vCurrentFirstItemNumberShown]);
		if (vItems[TempLoop + vCurrentFirstItemNumberShown])
		{
			if (vUseSelectText)
				TempButtonString.LoadString(IDS_UnSelect);
			else
				TempButtonString.LoadString(IDS_Disable);
			TempBoxString = vItemsYes[TempLoop + vCurrentFirstItemNumberShown];
		}
		switch (TempLoop)
		{
			case 0:
			{
				if (vItemEnabled[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_SubFunction1Button.ShowWindow(SW_SHOW);
					m_Item1.ShowWindow(SW_SHOW);
				}
				else
				{
					m_SubFunction1Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
					m_Item1.ShowWindow(SW_HIDE);
				}
				if (vItems[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_Item1.EnableWindow(true);
					m_CheckMarkGraphic1.ShowWindow(SW_SHOW);
				}
				else
				{
					m_Item1.EnableWindow(false);
					m_CheckMarkGraphic1.ShowWindow(SW_HIDE);
				}
				SetDlgItemText(IDC_SubFunction1Button,TempButtonString);
				SetDlgItemText(IDC_Item1,TempBoxString);

				pDC = m_Item1.GetDC();
				pWnd = &m_Item1;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item1, 5) + 30;  //5 is medium large
				m_Item1.SetWindowPos(NULL, 110, 52, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				m_CheckMarkGraphic1.SetWindowPos(NULL, 111 + (int)(TempStringLength / vGlobalPercentToResizeY) , 48, TempRect.bottom - TempRect.top, TempRect.right - TempRect.left, SWP_NOZORDER);
			}
			break;
			case 1:
			{
				if (vItemEnabled[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_SubFunction2Button.ShowWindow(SW_SHOW);
					m_Item2.ShowWindow(SW_SHOW);
				}
				else
				{
					m_SubFunction2Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
					m_Item2.ShowWindow(SW_HIDE);
				}
				if (vItems[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_Item2.EnableWindow(true);
					m_CheckMarkGraphic2.ShowWindow(SW_SHOW);
				}
				else
				{
					m_Item2.EnableWindow(false);
					m_CheckMarkGraphic2.ShowWindow(SW_HIDE);
				}
				SetDlgItemText(IDC_SubFunction2Button,TempButtonString);
				SetDlgItemText(IDC_Item2,TempBoxString);

				pDC = m_Item2.GetDC();
				pWnd = &m_Item2;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item2, 5) + 30;  //5 is medium large
				m_Item2.SetWindowPos(NULL, 110, 142, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				m_CheckMarkGraphic2.SetWindowPos(NULL, 111 + (int)(TempStringLength / vGlobalPercentToResizeY), 138, TempRect.bottom - TempRect.top, TempRect.right - TempRect.left, SWP_NOZORDER);
			}
			break;
			case 2:
			{
				if (vItemEnabled[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_SubFunction3Button.ShowWindow(SW_SHOW);
					m_Item3.ShowWindow(SW_SHOW);
				}
				else
				{
					m_SubFunction3Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
					m_Item3.ShowWindow(SW_HIDE);
				}
				if (vItems[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_Item3.EnableWindow(true);
					m_CheckMarkGraphic3.ShowWindow(SW_SHOW);
				}
				else
				{
					m_Item3.EnableWindow(false);
					m_CheckMarkGraphic3.ShowWindow(SW_HIDE);
				}
				SetDlgItemText(IDC_SubFunction3Button,TempButtonString);
				SetDlgItemText(IDC_Item3,TempBoxString);

				pDC = m_Item3.GetDC();
				pWnd = &m_Item3;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item3, 5) + 30;  //5 is medium large
				m_Item3.SetWindowPos(NULL, 110, 233, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				m_CheckMarkGraphic3.SetWindowPos(NULL, 111 + (int)(TempStringLength / vGlobalPercentToResizeY), 229, TempRect.bottom - TempRect.top, TempRect.right - TempRect.left, SWP_NOZORDER);
			}
			break;
			case 3:
			{
				if (vItemEnabled[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_SubFunction4Button.ShowWindow(SW_SHOW);
					m_Item4.ShowWindow(SW_SHOW);
				}
				else
				{
					m_SubFunction4Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
					m_Item4.ShowWindow(SW_HIDE);
				}
				if (vItems[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_Item4.EnableWindow(true);
					m_CheckMarkGraphic4.ShowWindow(SW_SHOW);
				}
				else
				{
					m_Item4.EnableWindow(false);
					m_CheckMarkGraphic4.ShowWindow(SW_HIDE);
				}
				SetDlgItemText(IDC_SubFunction4Button,TempButtonString);
				SetDlgItemText(IDC_Item4,TempBoxString);

				pDC = m_Item4.GetDC();
				pWnd = &m_Item4;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item4, 5) + 30;  //5 is medium large
				m_Item4.SetWindowPos(NULL, 110, 323, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				m_CheckMarkGraphic4.SetWindowPos(NULL, 111 + (int)(TempStringLength / vGlobalPercentToResizeY), 319, TempRect.bottom - TempRect.top, TempRect.right - TempRect.left, SWP_NOZORDER);
			}
			break;
			case 4:
			{
				if (vItemEnabled[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_SubFunction5Button.ShowWindow(SW_SHOW);
					m_Item5.ShowWindow(SW_SHOW);
				}
				else
				{
					m_SubFunction5Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
					m_Item5.ShowWindow(SW_HIDE);
				}
				if (vItems[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_Item5.EnableWindow(true);
					m_CheckMarkGraphic5.ShowWindow(SW_SHOW);
				}
				else
				{
					m_Item5.EnableWindow(false);
					m_CheckMarkGraphic5.ShowWindow(SW_HIDE);
				}
				SetDlgItemText(IDC_SubFunction5Button,TempButtonString);
				SetDlgItemText(IDC_Item5,TempBoxString);

				pDC = m_Item5.GetDC();
				pWnd = &m_Item5;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item5, 5) + 30;  //5 is medium large
				m_Item5.SetWindowPos(NULL, 525 - (int)(TempStringLength / vGlobalPercentToResizeY), 86, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				m_CheckMarkGraphic5.SetWindowPos(NULL, 485 - (int)(TempStringLength / vGlobalPercentToResizeY), 82, TempRect.bottom - TempRect.top, TempRect.right - TempRect.left, SWP_NOZORDER);
			}
			break;
			case 5:
			{
				if (vItemEnabled[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_SubFunction6Button.ShowWindow(SW_SHOW);
					m_Item6.ShowWindow(SW_SHOW);
				}
				else
				{
					m_SubFunction6Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
					m_Item6.ShowWindow(SW_HIDE);
				}
				if (vItems[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_Item6.EnableWindow(true);
					m_CheckMarkGraphic6.ShowWindow(SW_SHOW);
				}
				else
				{
					m_Item6.EnableWindow(false);
					m_CheckMarkGraphic6.ShowWindow(SW_HIDE);
				}
				SetDlgItemText(IDC_SubFunction6Button,TempButtonString);
				SetDlgItemText(IDC_Item6,TempBoxString);

				pDC = m_Item6.GetDC();
				pWnd = &m_Item6;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item6, 5) + 30;  //5 is medium large
				m_Item6.SetWindowPos(NULL, 525 - (int)(TempStringLength / vGlobalPercentToResizeY), 176, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				m_CheckMarkGraphic6.SetWindowPos(NULL, 485 - (int)(TempStringLength / vGlobalPercentToResizeY), 172, TempRect.bottom - TempRect.top, TempRect.right - TempRect.left, SWP_NOZORDER);
			}
			break;
			case 6:
			{
				if (vItemEnabled[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_SubFunction7Button.ShowWindow(SW_SHOW);
					m_Item7.ShowWindow(SW_SHOW);
				}
				else
				{
					m_SubFunction7Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
					m_Item7.ShowWindow(SW_HIDE);
				}
				if (vItems[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_Item7.EnableWindow(true);
					m_CheckMarkGraphic7.ShowWindow(SW_SHOW);
				}
				else
				{
					m_Item7.EnableWindow(false);
					m_CheckMarkGraphic7.ShowWindow(SW_HIDE);
				}
				SetDlgItemText(IDC_SubFunction7Button,TempButtonString);
				SetDlgItemText(IDC_Item7,TempBoxString);

				pDC = m_Item7.GetDC();
				pWnd = &m_Item7;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item7, 5) + 30;  //5 is medium large
				m_Item7.SetWindowPos(NULL, 525 - (int)(TempStringLength / vGlobalPercentToResizeY), 267, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				m_CheckMarkGraphic7.SetWindowPos(NULL, 485 - (int)(TempStringLength / vGlobalPercentToResizeY) ,263, TempRect.bottom - TempRect.top, TempRect.right - TempRect.left, SWP_NOZORDER);
			}
			break;
			case 7:
			{
				if (vItems[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_Item8.EnableWindow(true);
					m_CheckMarkGraphic8.ShowWindow(SW_SHOW);
				}
				else
				{
					m_Item8.EnableWindow(false);
					m_CheckMarkGraphic8.ShowWindow(SW_HIDE);
				}
				if (vItemEnabled[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_SubFunction8Button.ShowWindow(SW_SHOW);
					m_Item8.ShowWindow(SW_SHOW);
				}
				else
				{
					m_SubFunction8Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
					m_Item8.ShowWindow(SW_HIDE);
				}
				SetDlgItemText(IDC_SubFunction8Button,TempButtonString);
				SetDlgItemText(IDC_Item8,TempBoxString);

				pDC = m_Item8.GetDC();
				pWnd = &m_Item8;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item8, 5) + 30;  //5 is medium large
				m_Item8.SetWindowPos(NULL, 525 - (int)(TempStringLength / vGlobalPercentToResizeY), 353, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				m_CheckMarkGraphic8.SetWindowPos(NULL, 485 - (int)(TempStringLength / vGlobalPercentToResizeY), 357, TempRect.bottom - TempRect.top, TempRect.right - TempRect.left, SWP_NOZORDER);
			}
			break;
		}
	}
	CString TempText("");
	if (vNumberOfItems > vCurrentFirstItemNumberShown + 8)
	{
		m_Function5Button.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_Next);
		SetDlgItemText(IDC_Function5Button,TempText);
	}
	else
	{
		if (!vInAutoSetup)
		{
			m_Function5Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
		}
	}

	if (vCurrentFirstItemNumberShown > 1)
	{
		m_Function3Button.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_Previous);
		SetDlgItemText(IDC_Function3Button,TempText);
	}
	else
	{
		m_Function3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
	}
	::SetupMenu(vLocalDisplayCollection);
}

bool CSelectMultipleItemDialog::IsThereAnInwardItemDisabled(BYTE TempCurrentItem)
{
	bool TempReturn = false;
	if (vOutboardItemMustBeDisabled)
	{
		if ((TempCurrentItem + 1) < vNumberOfItems / 2)
		{
			for (BYTE TempLoop = TempCurrentItem + 1; TempLoop < vNumberOfItems / 2; TempLoop++)
			if (!vItems[TempLoop])
				TempReturn = true;
		}
		else
		{
			for (BYTE TempLoop = TempCurrentItem - 1; TempLoop >= vNumberOfItems / 2; TempLoop--)
			if (!vItems[TempLoop])
				TempReturn = true;
		}
		if (TempReturn)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nYou must enable inward detector before you can enable this detector.";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
	}
	return TempReturn;
}

bool CSelectMultipleItemDialog::IsThereAnOutwardItemEnabled(BYTE TempCurrentItem)
{
	bool TempReturn = false;
	if (vOutboardItemMustBeDisabled)
	{
		if ((TempCurrentItem + 1) > vNumberOfItems / 2)
		{
			for (BYTE TempLoop = TempCurrentItem + 1; TempLoop < vNumberOfItems; TempLoop++)
			if (vItems[TempLoop])
				TempReturn = true;
		}
		else
		{
			for (int TempLoop = TempCurrentItem - 1; TempLoop >= 0; TempLoop--)
			if (vItems[TempLoop])
				TempReturn = true;
		}

		if (TempReturn)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nYou must disable outward detector before you can disable this detector.";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
	}
	return TempReturn;
}

void CSelectMultipleItemDialog::OnDialogtitlestatictext1()
{
	//bring up soft key menu for help, print screen, Image, Clear, and hidden shift key
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
