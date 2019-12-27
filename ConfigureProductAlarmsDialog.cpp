//Scan Trac Solo Source File ScanTrac
// ConfigureProductAlarmsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "ScanTracDlg.h"
#include "afxdialogex.h"
#include "SystemConfigurationData.h"
#include "NumericEntryDialog.h"
#include "ConfigureProductAlarmsDialog.h"

extern double vGlobalPercentToResizeY;
extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
extern CProduct *vGlobalCurrentProduct;
extern CScanTracSystemRunningData *vGlobalRunningData;
/////////////////////////////////////////////////////////////////////////////
// CConfigureProductAlarmsDialog dialog

IMPLEMENT_DYNAMIC(CConfigureProductAlarmsDialog, CDialogEx)

CConfigureProductAlarmsDialog::CConfigureProductAlarmsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigureProductAlarmsDialog::IDD, pParent)
{
	vAlarmNumber = 0;
	vCurrentFirstItemNumberShown = 0;
	vNumberOfItems = 0; //cMaximumNumberOfInspections; // This value is changed later
	WORD TempLoop = 0;
	for (; TempLoop < vGlobalCurrentProduct->vNumberOfInspections ; TempLoop++)
	{
		vItems[TempLoop] = true; 
		vInspectionName[TempLoop] = (vGlobalCurrentProduct->vInspection[TempLoop])->vName;
		vNumberOfItems++;
		vItemEnabled[TempLoop] = false; //This is initialised later in LoadSettings().
	}
	for (; TempLoop < cMaximumNumberOfInspections; TempLoop++)
		vItems[TempLoop] = false; 
	vUseSelectText = false;
	vChangeMade = false;
	vInAutoSetup = 0;
	vCurrentPage = 0;
	vNumberOfItemsSelected = 0;
	vGreenBrush = ::CreateSolidBrush(cGreen);
	vYellowBrush = ::CreateSolidBrush(cYellow);
	vLightGreenBrush = ::CreateSolidBrush(cLightGreen);
	vWhiteBrush = ::CreateSolidBrush(cWhite);
	vAllSelected = false;
	vEjectorsEnabled = false;
	vXValue = 0;
	vYValue = 0;
	vConfigurationSettings = 0;
    vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_XOfYTextBox);
	vLocalCWndCollection.Add(&m_TotalTextbox);
	vLocalCWndCollection.Add(&m_RejectsTextbox);
	vLocalCWndCollection.Add(&m_ofTotalTextbox);

	//put in a different collection so can update each time change
	vLocalDisplayCollection.Add(&m_Item8);
	vLocalDisplayCollection.Add(&m_Item7);
	vLocalDisplayCollection.Add(&m_Item6);
	vLocalDisplayCollection.Add(&m_Item5);
	vLocalDisplayCollection.Add(&m_Item4);
	vLocalDisplayCollection.Add(&m_Item3);
	vLocalDisplayCollection.Add(&m_Item2);
	vLocalDisplayCollection.Add(&m_Item1);
	vLocalDisplayCollection.Add(&m_CheckMarkGraphic8);
	vLocalDisplayCollection.Add(&m_CheckMarkGraphic7);
	vLocalDisplayCollection.Add(&m_CheckMarkGraphic6);
	vLocalDisplayCollection.Add(&m_CheckMarkGraphic5);
	vLocalDisplayCollection.Add(&m_CheckMarkGraphic4);
	vLocalDisplayCollection.Add(&m_CheckMarkGraphic3);
	vLocalDisplayCollection.Add(&m_CheckMarkGraphic2);
	vLocalDisplayCollection.Add(&m_CheckMarkGraphic1);
}

CConfigureProductAlarmsDialog::~CConfigureProductAlarmsDialog()
{
	DeleteObject(vGreenBrush);
	DeleteObject(vYellowBrush);
	DeleteObject(vLightGreenBrush);
	DeleteObject(vWhiteBrush);
}

void CConfigureProductAlarmsDialog::ToggleInspection(BYTE TempInspectionNumber)
{
	vItemEnabled[TempInspectionNumber] = !vItemEnabled[TempInspectionNumber];
	if(vItemEnabled[TempInspectionNumber])
		vNumberOfItemsSelected++;
	else
		vNumberOfItemsSelected--;
	UpdateSettings(TempInspectionNumber);
}

bool CConfigureProductAlarmsDialog::CheckAllSelected()
{
	for (int TempLoop = 0; TempLoop < vNumberOfItems; TempLoop++)
	{
		if (vItemEnabled[TempLoop] == false)
		{
			return false;
		}
	}
	return true;
}

void CConfigureProductAlarmsDialog::LoadSettings(DWORD TempSettings)
{
	if ((TempSettings & cDWORDLastSignificantDigitValue))
	{
		for (BYTE TempLoop = 0; TempLoop < vNumberOfItems; TempLoop++)
		{
			DWORD TempBitVector = 1;
			for (BYTE TempNum = TempLoop; TempNum > 0; TempNum--)
				TempBitVector = TempBitVector * 2;
			if ((TempBitVector & TempSettings) == TempBitVector)
			{
				vConfigurationSettings = (vConfigurationSettings | TempBitVector);
				vItemEnabled[TempLoop] = true;
				vNumberOfItemsSelected++;
			}
		}
	}
	else
	{
		ToggleAllSelected();
	}
	vConfigurationSettings = (vConfigurationSettings | cDWORDLastSignificantDigitValue);
}

void CConfigureProductAlarmsDialog::UpdateSettings(BYTE TempBitNum)
{
	int TempBitVector = 1;
	for (;TempBitNum > 0; TempBitNum--)
		TempBitVector = TempBitVector * 2;
	vConfigurationSettings = vConfigurationSettings ^ TempBitVector;
}

void CConfigureProductAlarmsDialog::ToggleAllSelected()
{
	vAllSelected = !vAllSelected;

	if (vAllSelected)
		vNumberOfItemsSelected = vNumberOfItems;
	else
		vNumberOfItemsSelected = 0;

	for (BYTE TempLoop = 0; TempLoop < vNumberOfItems; TempLoop++)
	{
		if (vItemEnabled[TempLoop] != vAllSelected)
			UpdateSettings(TempLoop);
		vItemEnabled[TempLoop] = vAllSelected;
	}
}

void CConfigureProductAlarmsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
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
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	//DDV_MaxChars(pDX, m_DialogTitleStaticText, 50);
	DDX_Control(pDX, IDC_XOfYText, m_XOfYTextBox);
	DDX_Control(pDX, IDC_TotalTextbox, m_TotalTextbox);
	DDX_Control(pDX, IDC_RejectsTextbox, m_RejectsTextbox);
	DDX_Control(pDX, IDC_ofTotalTextbox, m_ofTotalTextbox);
}


BEGIN_MESSAGE_MAP(CConfigureProductAlarmsDialog, CDialog)
	ON_STN_CLICKED(IDC_Function1Button, &CConfigureProductAlarmsDialog::OnFunction1button)
	ON_STN_CLICKED(IDC_Function2Button, &CConfigureProductAlarmsDialog::OnFunction2button)
	ON_STN_CLICKED(IDC_Function3Button, &CConfigureProductAlarmsDialog::OnFunction3button)
	ON_STN_CLICKED(IDC_Function4Button, &CConfigureProductAlarmsDialog::OnFunction4button)
	ON_STN_CLICKED(IDC_Function5Button, &CConfigureProductAlarmsDialog::OnFunction5button)
	ON_STN_CLICKED(IDC_SubFunction1Button, &CConfigureProductAlarmsDialog::OnSubfunction1button)
	ON_STN_CLICKED(IDC_SubFunction2Button, &CConfigureProductAlarmsDialog::OnSubfunction2button)
	ON_STN_CLICKED(IDC_SubFunction3Button, &CConfigureProductAlarmsDialog::OnSubfunction3button)
	ON_STN_CLICKED(IDC_SubFunction4Button, &CConfigureProductAlarmsDialog::OnSubfunction4button)
	ON_STN_CLICKED(IDC_SubFunction5Button, &CConfigureProductAlarmsDialog::OnSubfunction5button)
	ON_STN_CLICKED(IDC_SubFunction6Button, &CConfigureProductAlarmsDialog::OnSubfunction6button)
	ON_STN_CLICKED(IDC_SubFunction7Button, &CConfigureProductAlarmsDialog::OnSubfunction7button)
	ON_STN_CLICKED(IDC_SubFunction8Button, &CConfigureProductAlarmsDialog::OnSubfunction8button)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CConfigureProductAlarmsDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigureProductAlarmsDialog message handlers

void CConfigureProductAlarmsDialog::OnFunction1button()
{
	// Main Menu Button was pressed
	if (AbortSetupChanges())
		CDialog::EndDialog(10);
}


void CConfigureProductAlarmsDialog::OnFunction2button()
{
	//Save and Quit button was pressed
	//Check for X <= Y
	CDialog::EndDialog(IDOK);
}


void CConfigureProductAlarmsDialog::OnFunction3button()
{
	//Previous button was pressed
	if (vCurrentFirstItemNumberShown < 4)
		return;
	switch (vCurrentPage)
	{
		case 1:
		{
			vCurrentFirstItemNumberShown = 0;
		}
		break;
		default:
		{
			vCurrentFirstItemNumberShown = vCurrentFirstItemNumberShown - 8;
		}
		break;
	}
	vCurrentPage = vCurrentPage - 1;
	UpdateItemDisplay();
}


void CConfigureProductAlarmsDialog::OnFunction4button()
{
	//Cancel button was pressed
	if ((vChangeMade) && (AbortSetupChanges()))
		CDialog::OnCancel();
}


void CConfigureProductAlarmsDialog::OnFunction5button()
{
	switch (vCurrentPage)
	{
		case 0:
		{
			if (vCurrentFirstItemNumberShown + 4 < vNumberOfItems)
			{
				vCurrentFirstItemNumberShown = 4;
				vCurrentPage = vCurrentPage + 1;
			}
		}
		break;
		default:
		{
			if (vCurrentFirstItemNumberShown + 8 < vNumberOfItems)
			{
				vCurrentFirstItemNumberShown = vCurrentFirstItemNumberShown + 8;
				vCurrentPage = vCurrentPage + 1;
			}
		}
		break;
	}
	UpdateItemDisplay();
}

void CConfigureProductAlarmsDialog::OnSubfunction1button()
{
	if (vCurrentPage != 0 && vCurrentFirstItemNumberShown < vNumberOfItems)
	{
		ToggleInspection(vCurrentFirstItemNumberShown);
	}
	else
	{
		CNumericEntryDialog INumericEntryDialog;  
		INumericEntryDialog.vMaxValue = cMaximumEjectDataSize;
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter value for X";
		INumericEntryDialog.m_DialogTitleStaticText2 = "of X of Y Alarm #" + dtoa(vAlarmNumber, 0);
		INumericEntryDialog.m_DialogTitleStaticText3 = "X" ;
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original value: " + dtoa(vXValue, 0);
		INumericEntryDialog.vEditString = dtoa(vXValue, 0);

		int nResponse = INumericEntryDialog.DoModal();//numeric input for x
		if (nResponse == IDOK)
		{
			vXValue = (WORD) ATOF(INumericEntryDialog.vEditString);
			if (vXValue > vYValue)
				vYValue = vXValue;
		}
		else if (nResponse == 10)
		{
			CDialog::EndDialog(10);
		}
	}
	vChangeMade = true;
	UpdateItemDisplay();
}


void CConfigureProductAlarmsDialog::OnSubfunction2button()
{
	if (vCurrentPage != 0 && vCurrentFirstItemNumberShown + 1 < vNumberOfItems)
	{
		ToggleInspection(vCurrentFirstItemNumberShown + 1);
	}
	else
	{
		CNumericEntryDialog INumericEntryDialog;
		INumericEntryDialog.vMaxValue = cMaximumEjectDataSize;
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter value for Y";
		INumericEntryDialog.m_DialogTitleStaticText2 = "of X of Y Alarm #" + dtoa(vAlarmNumber, 0);
		INumericEntryDialog.m_DialogTitleStaticText3 = "Y" ;
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original value: " + dtoa(vYValue, 0);
		INumericEntryDialog.vEditString = dtoa(vYValue, 0);

		int nResponse = INumericEntryDialog.DoModal();
		if (nResponse == IDOK)
		{
			vYValue = (WORD) ATOF(INumericEntryDialog.vEditString);
			if (vYValue < vXValue)
				vXValue = vYValue;
		}
	}
	vChangeMade = true;
	UpdateItemDisplay();
}


void CConfigureProductAlarmsDialog::OnSubfunction3button()
{
	if (vCurrentPage != 0 && vCurrentFirstItemNumberShown + 2 < vNumberOfItems)
		ToggleInspection(vCurrentFirstItemNumberShown + 2);
	else
		ToggleInspection(0);
	vChangeMade = true;
	UpdateItemDisplay();
}


void CConfigureProductAlarmsDialog::OnSubfunction4button()
{
	if (vCurrentPage != 0 && vCurrentFirstItemNumberShown + 3 < vNumberOfItems)
	{
		ToggleInspection(vCurrentFirstItemNumberShown + 3);
	}
	else
		ToggleInspection(1);
	vChangeMade = true;
	UpdateItemDisplay();
}


void CConfigureProductAlarmsDialog::OnSubfunction5button()
{
	if (vCurrentPage != 0 && vCurrentFirstItemNumberShown + 4 < vNumberOfItems)
	{
		ToggleInspection(vCurrentFirstItemNumberShown + 4);
	}
	else
		vEjectorsEnabled = !vEjectorsEnabled;
	vChangeMade = true;
	UpdateItemDisplay();
}


void CConfigureProductAlarmsDialog::OnSubfunction6button()
{
	if (vCurrentPage != 0 && vCurrentFirstItemNumberShown + 5 < vNumberOfItems)
	{
		ToggleInspection(vCurrentFirstItemNumberShown + 5);
	}
	else
	{
		ToggleAllSelected();
	}
	vChangeMade = true;
	UpdateItemDisplay();
}


void CConfigureProductAlarmsDialog::OnSubfunction7button()
{
	if (vCurrentPage != 0 && vCurrentFirstItemNumberShown + 6 < vNumberOfItems)
		ToggleInspection(vCurrentFirstItemNumberShown + 6);
	else
		ToggleInspection(2);
	vChangeMade = true;
	UpdateItemDisplay();
}


void CConfigureProductAlarmsDialog::OnSubfunction8button()
{
	if (vCurrentPage != 0 && vCurrentFirstItemNumberShown + 7 < vNumberOfItems)
		ToggleInspection(vCurrentFirstItemNumberShown + 7);
	else
		ToggleInspection(3);
	vChangeMade = true;
	UpdateItemDisplay();
}

BOOL CConfigureProductAlarmsDialog::PreTranslateMessage(MSG* pMsg)
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
				OnSubfunction5button();
			break;
			case 114: // F3:
				OnSubfunction7button();
			break;
			case 115: // F4:
				OnSubfunction6button();
			break;
			case 116: // F5:
				OnSubfunction1button();
			break;
			case 117: // F6 Key - Function 5:
				OnSubfunction4button();
			break;
			case 118: // F7 Key - Sub Function 1:
				OnFunction1button();
			break;
			case 119: // F8 Key - Sub Function 2:
				OnSubfunction8button();
			break;
			case 120: // F9 Key - Sub Function 3:
				OnSubfunction2button();
			break;
			case 122: // F11 Key - Sub Function 4:
				OnSubfunction3button();
			break;
			case 111: // / Key - Sub Function 5:
				OnFunction2button();
			break;
			case 106: // * Key - Sub Function 6:
				OnFunction4button();
			break;
			case 109: // - Key - Sub Function 7  Decimal Point:
				OnFunction3button();
			break;
			case 107: // + Key - Sub Function 8:
				OnFunction5button();
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
				PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 0, 0);
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
	//End the comment preceding this one
	return CDialog::PreTranslateMessage(pMsg);
}


HBRUSH CConfigureProductAlarmsDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

    if (pWnd->GetDlgCtrlID() == IDC_Item1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item1, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vItemEnabled[0 + vCurrentFirstItemNumberShown] && vCurrentPage > 0)
			return vGreenBrush;
		else if (vXValue == 0 && vCurrentPage == 0)
			return vYellowBrush;
		else if (vCurrentPage == 0)
			return vGreenBrush;
	}
 
    if (pWnd->GetDlgCtrlID() == IDC_Item2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item2, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vItemEnabled[1 + vCurrentFirstItemNumberShown] && vCurrentPage > 0)
			return vGreenBrush;
		else if (vYValue == 0 && vCurrentPage == 0)
			return vYellowBrush;
		else if (vCurrentPage == 0)
			return vGreenBrush;
	}
 
    if (pWnd->GetDlgCtrlID() == IDC_Item3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item3, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vItemEnabled[2 + vCurrentFirstItemNumberShown] && vCurrentPage > 0)
			return vGreenBrush;
		else if (vItemEnabled[0] && vCurrentPage == 0)
			return vGreenBrush;
	}
 
    if (pWnd->GetDlgCtrlID() == IDC_Item4)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item4, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vItemEnabled[3 + vCurrentFirstItemNumberShown] && vCurrentPage > 0)
			return vGreenBrush;
		else if (vItemEnabled[1] && vCurrentPage == 0)
			return vGreenBrush;
	}
 
    if (pWnd->GetDlgCtrlID() == IDC_Item5)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item5, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vItemEnabled[4 + vCurrentFirstItemNumberShown] && vCurrentPage > 0)
			return vGreenBrush;
		else if (vEjectorsEnabled && vCurrentPage == 0)
			return vGreenBrush;
		else if (!vEjectorsEnabled && vCurrentPage == 0)
			return vYellowBrush;
	}
 
    if (pWnd->GetDlgCtrlID() == IDC_Item6)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item6, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vItemEnabled[5 + vCurrentFirstItemNumberShown] && vCurrentPage > 0)
			return vGreenBrush;
		else if (vNumberOfItemsSelected == 0 && vCurrentPage == 0)
			return vYellowBrush;
		else if (vNumberOfItemsSelected == vNumberOfItems && vCurrentPage == 0)
			return vGreenBrush;
	}
 
    if (pWnd->GetDlgCtrlID() == IDC_Item7)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item7, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vItemEnabled[6 + vCurrentFirstItemNumberShown] && vCurrentPage > 0)
			return vGreenBrush;
		else if (vItemEnabled[2] && vCurrentPage == 0)
			return vGreenBrush;
	}
 
    if (pWnd->GetDlgCtrlID() == IDC_Item8)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item8, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vItemEnabled[7 + vCurrentFirstItemNumberShown] && vCurrentPage > 0)
			return vGreenBrush;
		else if (vItemEnabled[3] && vCurrentPage == 0)
			return vGreenBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLightGreenBrush;
    }
	if (pWnd->GetDlgCtrlID() == IDC_RejectsTextbox)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_RejectsTextbox, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLightGreenBrush;
    }
	if (pWnd->GetDlgCtrlID() == IDC_ofTotalTextbox)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ofTotalTextbox, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLightGreenBrush;
    }
	if (pWnd->GetDlgCtrlID() == IDC_TotalTextbox)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TotalTextbox, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLightGreenBrush;
    }
	if (pWnd->GetDlgCtrlID() == IDC_XOfYText)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XOfYTextBox, 5);
		pDC->SetBkMode(TRANSPARENT);
    }

	//if ((pWnd->GetDlgCtrlID() == IDC_Title1) ||
	//	(pWnd->GetDlgCtrlID() == IDC_Title2) ||
	//	(pWnd->GetDlgCtrlID() == IDC_Title4) ||
	//	(pWnd->GetDlgCtrlID() == IDC_Title6) 
	//	(pWnd->GetDlgCtrlID() == IDC_Title8))
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title1, 5);
	//	pDC->SetBkMode(TRANSPARENT);
	//	return vLightGreenBrush;
 //   }

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

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

//void CConfigureProductAlarmsDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
//{
//	//This function is never called. 
//	vXValue = 500; 
//	CDialog::OnShowWindow(bShow, nStatus);
//	this->SetWindowText("Configure Alarms");
//	CDialog::OnShowWindow(bShow, nStatus);
//	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
//		cMainBackgroundTop,cMainBackgroundLength,
//		cMainBackgroundHeight,SWP_NOZORDER);
//	m_Background.ShowWindow(SW_SHOW);
//
//	vChangeMade = false;
//	CString TempText("");
//	if (vInAutoSetup)
//	{
//		m_Function2Button.ShowWindow(SW_HIDE);
//		m_ArrowDown2.ShowWindow(SW_HIDE);
//		m_Function5Button.ShowWindow(SW_SHOW);
//		m_ArrowDown5.ShowWindow(SW_SHOW);
//		m_Function5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
//		TempText.LoadString(IDS_NextAutoSetupStep);
//		SetDlgItemText(IDC_Function5Button,TempText);
//		m_Function4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
//		m_Function4Button.ShowWindow(SW_SHOW);
//		m_ArrowDown4.ShowWindow(SW_SHOW);
//		TempText.LoadString(IDS_PreviousAutoSetup);
//		SetDlgItemText(IDC_Function4Button,TempText);
//	}
//	else
//	{
//		TempText.LoadString(IDS_Exit);
//		SetDlgItemText(IDC_Function2Button,TempText);
//		TempText.LoadString(IDS_Cancel);
//		SetDlgItemText(IDC_Function4Button,TempText);
//	}
//	TempText.LoadString(IDS_MainMenu);
//	SetDlgItemText(IDC_Function1Button,TempText);
//	
//	if (vNumberOfItems > 2)
//	{
//		m_SubFunction3Button.ShowWindow(SW_SHOW);
//		m_ArrowLeft3.ShowWindow(SW_SHOW);
//		m_Item3.ShowWindow(SW_SHOW);
//	}
//	if (vNumberOfItems > 3)
//	{
//		m_SubFunction4Button.ShowWindow(SW_SHOW);
//		m_ArrowLeft4.ShowWindow(SW_SHOW);
//		m_Item4.ShowWindow(SW_SHOW);
//	}
//	if (vNumberOfItems > 4)
//	{
//		m_SubFunction5Button.ShowWindow(SW_SHOW);
//		m_ArrowRight1.ShowWindow(SW_SHOW);
//		m_Item5.ShowWindow(SW_SHOW);
//	}
//	if (vNumberOfItems > 5)
//	{
//		m_SubFunction6Button.ShowWindow(SW_SHOW);
//		m_ArrowRight2.ShowWindow(SW_SHOW);
//		m_Item6.ShowWindow(SW_SHOW);
//	}
//	if (vNumberOfItems > 6)
//	{
//		m_SubFunction7Button.ShowWindow(SW_SHOW);
//		m_ArrowRight3.ShowWindow(SW_SHOW);
//		m_Item7.ShowWindow(SW_SHOW);
//	}
//	if (vNumberOfItems > 7)
//	{
//		m_SubFunction8Button.ShowWindow(SW_SHOW);
//		m_ArrowRight4.ShowWindow(SW_SHOW);
//		m_Item8.ShowWindow(SW_SHOW);
//	}
//	UpdateItemDisplay();
//}

void CConfigureProductAlarmsDialog::UpdateItemDisplay() 
{
	CWnd* pWnd;
	CDC* pDC;
	HWND TempDialogHwnd = this->m_hWnd;

	Invalidate(true); 
	m_Background.ShowWindow(SW_SHOW);
	vAllSelected = CheckAllSelected();
	BYTE TempLoop = 0;
	if (vNumberOfItems == 0)
	{
		m_ofTotalTextbox.ShowWindow(SW_HIDE);
		m_XOfYTextBox.ShowWindow(SW_HIDE);
		m_RejectsTextbox.ShowWindow(SW_HIDE);
		m_TotalTextbox.ShowWindow(SW_HIDE);
	}

	for (; TempLoop < cMaxItemsPerPage && vCurrentPage == 0; TempLoop++)
	{
		CString TempButtonString("");
		CString TempBoxString("");
		switch (TempLoop)
		{
			case 0:
			{
				TempButtonString = "\nSet X";
				char TempCharArray[16];
				_itoa_s(vXValue,TempCharArray,16,10);
				TempBoxString = TempCharArray;
			}
			break;
			case 1:
			{
				TempButtonString = "\nSet Y";
				char TempCharArray[16];
				_itoa_s(vYValue,TempCharArray,16,10);
				TempBoxString = TempCharArray;
			}
			break;
			case 2:
			{
				TempBoxString = vInspectionName[0];
				if (vItemEnabled[0])
				{
					TempButtonString.LoadString(IDS_InspectionEnabled);
					TempButtonString.Append( _T("1"));
				}
				else
				{
					TempButtonString.LoadString(IDS_InspectionDisabled);
					TempButtonString.Append( _T("1"));
				}
			}
			break;
			case 3:
			{
				TempBoxString = vInspectionName[1];
				if (vItemEnabled[1])
				{
					TempButtonString.LoadString(IDS_InspectionEnabled);
					TempButtonString.Append( _T("2"));
				}
				else
				{
					TempButtonString.LoadString(IDS_InspectionDisabled);
					TempButtonString.Append( _T("2"));
				}
			}
			break;
			case 4:
			{
				if (vEjectorsEnabled)
				{
					TempBoxString.LoadString(IDS_DisableEjectorsOnAlarm);	
					TempButtonString = "Yellow Message On Alarm";
				}
				else
				{
					TempBoxString.LoadString(IDS_YellowMessageOnly);
					TempBoxString = TempBoxString + " On Alarm";
					TempButtonString = "Disable Ejectors On Alarm";				
				}
			}
			break;
			case 5:
			{
				TempBoxString.Append(dtoa(vNumberOfItemsSelected, 0));
				TempBoxString.Append(_T(" out of "));
				TempBoxString.Append(dtoa(vNumberOfItems, 0));
				TempBoxString.Append(_T(" inspections selected"));
				if (vAllSelected)
					TempButtonString.LoadString(IDS_DeselectAll);
				else
				{
					CString TempText("");
					TempText.LoadString(IDS_SelectAll);
					TempButtonString = "\n";
					TempButtonString.Append(TempText);
				}
			}
			break;
			case 6:
			{
				TempBoxString = vInspectionName[2];
				if (vItemEnabled[2])
				{
					TempButtonString.LoadString(IDS_InspectionEnabled);
					TempButtonString.Append( _T("3"));
				}
				else
				{
					TempButtonString.LoadString(IDS_InspectionDisabled);
					TempButtonString.Append( _T("3"));
				}
			}
			break;
			case 7:
			{
				TempBoxString = vInspectionName[3];
				if (vItemEnabled[3])
				{
					TempButtonString.LoadString(IDS_InspectionEnabled);
					TempButtonString.Append( _T("4"));
				}
				else
				{
					TempButtonString.LoadString(IDS_InspectionDisabled);
					TempButtonString.Append( _T("4"));
				}
			}
			break;
		}
		switch (TempLoop)
		{
			case 0:
			{
				m_SubFunction1Button.ShowWindow(SW_SHOW);
				m_Item1.SetWindowPos(NULL,110,52,65,28,SWP_NOZORDER);
				m_Item1.ShowWindow(SW_SHOW);
				m_Item1.EnableWindow(true);
				m_CheckMarkGraphic1.ShowWindow(SW_HIDE);
				m_XOfYTextBox.ShowWindow(SW_SHOW);
				m_RejectsTextbox.ShowWindow(SW_SHOW);
				m_TotalTextbox.ShowWindow(SW_SHOW);
				m_ofTotalTextbox.ShowWindow(SW_SHOW);
				m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				SetDlgItemText(IDC_SubFunction1Button,TempButtonString);
				SetDlgItemText(IDC_Item1,TempBoxString);
			}
			break;
			case 1:
			{
				m_SubFunction2Button.ShowWindow(SW_SHOW);
				m_Item2.SetWindowPos(NULL,110,142,65,28,SWP_NOZORDER);
				m_Item2.ShowWindow(SW_SHOW);
				m_Item2.EnableWindow(true);
				m_CheckMarkGraphic2.ShowWindow(SW_HIDE);
				m_SubFunction2Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				SetDlgItemText(IDC_SubFunction2Button,TempButtonString);
				SetDlgItemText(IDC_Item2,TempBoxString);
			}
			break;
			case 2:
			{
				if (vItems[vCurrentFirstItemNumberShown + 0])
				{
					m_SubFunction3Button.ShowWindow(SW_SHOW);
					m_Item3.ShowWindow(SW_SHOW);
					m_Item3.EnableWindow(true);
					if (vItemEnabled[vCurrentFirstItemNumberShown + 0])
						m_CheckMarkGraphic3.ShowWindow(SW_SHOW);
					else
						m_CheckMarkGraphic3.ShowWindow(SW_HIDE);
				}
				else
				{
					m_SubFunction3Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
					m_Item3.ShowWindow(SW_HIDE);
					m_CheckMarkGraphic3.ShowWindow(SW_HIDE);
				}
				
				m_SubFunction3Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				SetDlgItemText(IDC_SubFunction3Button,TempButtonString);
				SetDlgItemText(IDC_Item3,TempBoxString);

				pDC = m_Item3.GetDC();
				pWnd = &m_Item3;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item3, 5) + 30;  //5 is medium large
				m_Item3.SetWindowPos(NULL, 110, 233, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				m_CheckMarkGraphic3.SetWindowPos(NULL, 111 + (int)(TempStringLength / vGlobalPercentToResizeY), 229, vCheckMarkGraphicSizeRect.bottom - vCheckMarkGraphicSizeRect.top, vCheckMarkGraphicSizeRect.right - vCheckMarkGraphicSizeRect.left, SWP_NOZORDER);
			}
			break;
			case 3:
			{
				if (vItems[vCurrentFirstItemNumberShown + 1])
				{
					m_SubFunction4Button.ShowWindow(SW_SHOW);
					m_Item4.ShowWindow(SW_SHOW);
					m_Item4.EnableWindow(true);
					if (vItemEnabled[vCurrentFirstItemNumberShown + 1])
						m_CheckMarkGraphic4.ShowWindow(SW_SHOW);
					else
						m_CheckMarkGraphic4.ShowWindow(SW_HIDE);
				}
				else
				{
					m_SubFunction4Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
					m_Item4.ShowWindow(SW_HIDE);
					m_CheckMarkGraphic4.ShowWindow(SW_HIDE);
				}
				m_SubFunction4Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				SetDlgItemText(IDC_SubFunction4Button,TempButtonString);
				SetDlgItemText(IDC_Item4,TempBoxString);

				pDC = m_Item4.GetDC();
				pWnd = &m_Item4;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item4, 5) + 30;  //5 is medium large
				m_Item4.SetWindowPos(NULL, 110, 323, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				m_CheckMarkGraphic4.SetWindowPos(NULL, 111 + (int)(TempStringLength / vGlobalPercentToResizeY), 319, vCheckMarkGraphicSizeRect.bottom - vCheckMarkGraphicSizeRect.top, vCheckMarkGraphicSizeRect.right - vCheckMarkGraphicSizeRect.left, SWP_NOZORDER);
			}
			break;
			case 4:
			{
				m_SubFunction5Button.ShowWindow(SW_SHOW);
				m_Item5.ShowWindow(SW_SHOW);
				m_Item5.EnableWindow(true);
				if (vEjectorsEnabled)
					m_CheckMarkGraphic5.ShowWindow(SW_SHOW);
				else
					m_CheckMarkGraphic5.ShowWindow(SW_HIDE);
				m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				SetDlgItemText(IDC_SubFunction5Button,TempButtonString);
				SetDlgItemText(IDC_Item5,TempBoxString);

				pDC = m_Item5.GetDC();
				pWnd = &m_Item5;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item5, 5) + 30;  //5 is medium large
				m_Item5.SetWindowPos(NULL, 525 - (int)(TempStringLength / vGlobalPercentToResizeY), 86, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				m_CheckMarkGraphic5.SetWindowPos(NULL, 485 - (int)(TempStringLength / vGlobalPercentToResizeY), 82, vCheckMarkGraphicSizeRect.bottom - vCheckMarkGraphicSizeRect.top, vCheckMarkGraphicSizeRect.right - vCheckMarkGraphicSizeRect.left, SWP_NOZORDER);
			}
			break;
			case 5:
			{
				m_SubFunction6Button.ShowWindow(SW_SHOW);
				m_Item6.ShowWindow(SW_SHOW);
				m_Item6.EnableWindow(true);
				if (vAllSelected)
					m_CheckMarkGraphic6.ShowWindow(SW_SHOW);
				else
					m_CheckMarkGraphic6.ShowWindow(SW_HIDE);

				m_SubFunction6Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				SetDlgItemText(IDC_SubFunction6Button,TempButtonString);
				SetDlgItemText(IDC_Item6,TempBoxString);

				pDC = m_Item6.GetDC();
				pWnd = &m_Item6;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item6, 5) + 30;  //5 is medium large
				m_Item6.SetWindowPos(NULL, 525 - (int)(TempStringLength / vGlobalPercentToResizeY), 176, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				m_CheckMarkGraphic6.SetWindowPos(NULL, 485 - (int)(TempStringLength / vGlobalPercentToResizeY), 172, vCheckMarkGraphicSizeRect.bottom - vCheckMarkGraphicSizeRect.top, vCheckMarkGraphicSizeRect.right - vCheckMarkGraphicSizeRect.left, SWP_NOZORDER);
			}
			break;
			case 6:
			{
				if (vItems[vCurrentFirstItemNumberShown + 2])
				{
					m_SubFunction7Button.ShowWindow(SW_SHOW);
					m_Item7.ShowWindow(SW_SHOW);
					m_Item7.EnableWindow(true);
					if (vItemEnabled[vCurrentFirstItemNumberShown + 2])
						m_CheckMarkGraphic7.ShowWindow(SW_SHOW);
					else
						m_CheckMarkGraphic7.ShowWindow(SW_HIDE);
				}
				else
				{
					m_SubFunction7Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
					m_Item7.ShowWindow(SW_HIDE);
					m_CheckMarkGraphic7.ShowWindow(SW_HIDE);
				}
				m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				SetDlgItemText(IDC_SubFunction7Button,TempButtonString);
				SetDlgItemText(IDC_Item7,TempBoxString);

				pDC = m_Item7.GetDC();
				pWnd = &m_Item7;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item7, 5) + 30;  //5 is medium large
				m_Item7.SetWindowPos(NULL, 525 - (int)(TempStringLength / vGlobalPercentToResizeY), 267, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				m_CheckMarkGraphic7.SetWindowPos(NULL, 485 - (int)(TempStringLength / vGlobalPercentToResizeY), 263, vCheckMarkGraphicSizeRect.bottom - vCheckMarkGraphicSizeRect.top, vCheckMarkGraphicSizeRect.right - vCheckMarkGraphicSizeRect.left, SWP_NOZORDER);
			}
			break;
			case 7:
			{
				if (vItems[vCurrentFirstItemNumberShown + 3])
				{
					m_SubFunction8Button.ShowWindow(SW_SHOW);
					m_Item8.ShowWindow(SW_SHOW);
					m_Item8.EnableWindow(true);
					if (vItemEnabled[vCurrentFirstItemNumberShown + 3])
						m_CheckMarkGraphic8.ShowWindow(SW_SHOW);
					else
						m_CheckMarkGraphic8.ShowWindow(SW_HIDE);
				}
				else
				{
					m_SubFunction8Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
					m_Item8.ShowWindow(SW_HIDE);
					m_CheckMarkGraphic8.ShowWindow(SW_HIDE);
				}
				m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				SetDlgItemText(IDC_SubFunction8Button,TempButtonString);
				SetDlgItemText(IDC_Item8,TempBoxString);

				pDC = m_Item8.GetDC();
				pWnd = &m_Item8;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item8, 5) + 30;  //5 is medium large
				m_Item8.SetWindowPos(NULL, 525 - (int)(TempStringLength / vGlobalPercentToResizeY), 357, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				m_CheckMarkGraphic8.SetWindowPos(NULL, 485 - (int)(TempStringLength / vGlobalPercentToResizeY), 353, vCheckMarkGraphicSizeRect.bottom - vCheckMarkGraphicSizeRect.top, vCheckMarkGraphicSizeRect.right - vCheckMarkGraphicSizeRect.left, SWP_NOZORDER);
			}
			break;
		}
	}
	
	for (; TempLoop + vCurrentFirstItemNumberShown < vNumberOfItems && vCurrentPage > 0; TempLoop++)
	{
		CString TempButtonString("");
		CString TempBoxString("");
		if (vCurrentPage > 0)
		{
			TempBoxString = vInspectionName[TempLoop + vCurrentFirstItemNumberShown];
			if (vItemEnabled[TempLoop + vCurrentFirstItemNumberShown])
			{
				TempButtonString.LoadString(IDS_InspectionEnabled);
				TempButtonString.Append(dtoa(TempLoop+vCurrentFirstItemNumberShown+1, 0));
			}
			else
			{
				TempButtonString.LoadString(IDS_InspectionDisabled);
				TempButtonString.Append(dtoa(TempLoop+vCurrentFirstItemNumberShown+1, 0));
			}
		}
		switch (TempLoop)
		{
			case 0:
			{
				if (vItems[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_SubFunction1Button.ShowWindow(SW_SHOW);
					m_Item1.ShowWindow(SW_SHOW);
					m_Item1.EnableWindow(true);
				}
				else
				{
					m_SubFunction1Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
					m_Item1.ShowWindow(SW_HIDE);
				}
				if (vItemEnabled[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_CheckMarkGraphic1.ShowWindow(SW_SHOW);
				}
				else
				{
					m_CheckMarkGraphic1.ShowWindow(SW_HIDE);
				}
				m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				m_ofTotalTextbox.ShowWindow(SW_HIDE);
				m_XOfYTextBox.ShowWindow(SW_HIDE);
				m_RejectsTextbox.ShowWindow(SW_HIDE);
				m_TotalTextbox.ShowWindow(SW_HIDE);
				SetDlgItemText(IDC_SubFunction1Button,TempButtonString);
				SetDlgItemText(IDC_Item1,TempBoxString);

				pDC = m_Item1.GetDC();
				pWnd = &m_Item1;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item1, 5) + 30;  //5 is medium large
				m_Item1.SetWindowPos(NULL, 110, 52, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				m_CheckMarkGraphic1.SetWindowPos(NULL, 111 + (int)(TempStringLength / vGlobalPercentToResizeY), 48, vCheckMarkGraphicSizeRect.bottom - vCheckMarkGraphicSizeRect.top, vCheckMarkGraphicSizeRect.right - vCheckMarkGraphicSizeRect.left, SWP_NOZORDER);
			}
			break;
			case 1:
			{
				if (vItems[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_SubFunction2Button.ShowWindow(SW_SHOW);
					m_Item2.ShowWindow(SW_SHOW);
					m_Item2.EnableWindow(true);
				}
				else
				{
					m_SubFunction2Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
					m_Item2.ShowWindow(SW_HIDE);
				}
				if (vItemEnabled[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_CheckMarkGraphic2.ShowWindow(SW_SHOW);
				}
				else
				{
					m_CheckMarkGraphic2.ShowWindow(SW_HIDE);
				}
				m_SubFunction2Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				SetDlgItemText(IDC_SubFunction2Button,TempButtonString);
				SetDlgItemText(IDC_Item2,TempBoxString);

				pDC = m_Item2.GetDC();
				pWnd = &m_Item2;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item2, 5) + 30;  //5 is medium large
				m_Item2.SetWindowPos(NULL, 110, 142, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				m_CheckMarkGraphic2.SetWindowPos(NULL, 111 + (int)(TempStringLength / vGlobalPercentToResizeY), 138, vCheckMarkGraphicSizeRect.bottom - vCheckMarkGraphicSizeRect.top, vCheckMarkGraphicSizeRect.right - vCheckMarkGraphicSizeRect.left, SWP_NOZORDER);
			}
			break;
			case 2:
			{
				if (vItems[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_SubFunction3Button.ShowWindow(SW_SHOW);
					m_Item3.ShowWindow(SW_SHOW);
					m_Item3.EnableWindow(true);
				}
				else
				{
					m_SubFunction3Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
					m_Item3.ShowWindow(SW_HIDE);
				}
				if (vItemEnabled[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_CheckMarkGraphic3.ShowWindow(SW_SHOW);
				}
				else
				{
					m_CheckMarkGraphic3.ShowWindow(SW_HIDE);
				}
				m_SubFunction3Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				SetDlgItemText(IDC_SubFunction3Button,TempButtonString);
				SetDlgItemText(IDC_Item3,TempBoxString);

				pDC = m_Item3.GetDC();
				pWnd = &m_Item3;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item3, 5) + 30;  //5 is medium large
				m_Item3.SetWindowPos(NULL, 110, 233, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				m_CheckMarkGraphic3.SetWindowPos(NULL, 111 + (int)(TempStringLength / vGlobalPercentToResizeY), 229, vCheckMarkGraphicSizeRect.bottom - vCheckMarkGraphicSizeRect.top, vCheckMarkGraphicSizeRect.right - vCheckMarkGraphicSizeRect.left, SWP_NOZORDER);
			}
			break;
			case 3:
			{
				if (vItems[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_SubFunction4Button.ShowWindow(SW_SHOW);
					m_Item4.ShowWindow(SW_SHOW);
					m_Item4.EnableWindow(true);
				}
				else
				{
					m_SubFunction4Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
					m_Item4.ShowWindow(SW_HIDE);
				}
				if (vItemEnabled[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_CheckMarkGraphic4.ShowWindow(SW_SHOW);
				}
				else
				{
					m_CheckMarkGraphic4.ShowWindow(SW_HIDE);
				}
				m_SubFunction4Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				SetDlgItemText(IDC_SubFunction4Button,TempButtonString);
				SetDlgItemText(IDC_Item4,TempBoxString);

				pDC = m_Item4.GetDC();
				pWnd = &m_Item4;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item4, 5) + 30;  //5 is medium large
				m_Item4.SetWindowPos(NULL, 110, 323, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				m_CheckMarkGraphic4.SetWindowPos(NULL, 111 + (int)(TempStringLength / vGlobalPercentToResizeY), 319, vCheckMarkGraphicSizeRect.bottom - vCheckMarkGraphicSizeRect.top, vCheckMarkGraphicSizeRect.right - vCheckMarkGraphicSizeRect.left, SWP_NOZORDER);
			}
			break;
			case 4:
			{
				if (vItems[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_SubFunction5Button.ShowWindow(SW_SHOW);
					m_Item5.ShowWindow(SW_SHOW);
					m_Item5.EnableWindow(true);
				}
				else
				{
					m_SubFunction5Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
					m_Item5.ShowWindow(SW_HIDE);
				}
				if (vItemEnabled[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_CheckMarkGraphic5.ShowWindow(SW_SHOW);
				}
				else
				{
					m_CheckMarkGraphic5.ShowWindow(SW_HIDE);
				}
				m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				SetDlgItemText(IDC_SubFunction5Button,TempButtonString);
				SetDlgItemText(IDC_Item5,TempBoxString);

				pDC = m_Item5.GetDC();
				pWnd = &m_Item5;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item5, 5) + 30;  //5 is medium large
				m_Item5.SetWindowPos(NULL, 525 - (int)(TempStringLength / vGlobalPercentToResizeY), 86, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				m_CheckMarkGraphic5.SetWindowPos(NULL, 485 - (int)(TempStringLength / vGlobalPercentToResizeY), 82, vCheckMarkGraphicSizeRect.bottom - vCheckMarkGraphicSizeRect.top, vCheckMarkGraphicSizeRect.right - vCheckMarkGraphicSizeRect.left, SWP_NOZORDER);
			}
			break;
			case 5:
			{
				if (vItems[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_SubFunction6Button.ShowWindow(SW_SHOW);
					m_Item6.ShowWindow(SW_SHOW);
					m_Item6.EnableWindow(true);
				}
				else
				{
					m_SubFunction6Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
					m_Item6.ShowWindow(SW_HIDE);
				}
				if (vItemEnabled[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_CheckMarkGraphic6.ShowWindow(SW_SHOW);
				}
				else
				{
					m_CheckMarkGraphic6.ShowWindow(SW_HIDE);
				}
				m_SubFunction6Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				SetDlgItemText(IDC_SubFunction6Button,TempButtonString);
				SetDlgItemText(IDC_Item6,TempBoxString);

				pDC = m_Item6.GetDC();
				pWnd = &m_Item6;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item6, 5) + 30;  //5 is medium large
				m_Item6.SetWindowPos(NULL, 525 - (int)(TempStringLength / vGlobalPercentToResizeY), 176, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				m_CheckMarkGraphic6.SetWindowPos(NULL, 485 - (int)(TempStringLength / vGlobalPercentToResizeY), 172, vCheckMarkGraphicSizeRect.bottom - vCheckMarkGraphicSizeRect.top, vCheckMarkGraphicSizeRect.right - vCheckMarkGraphicSizeRect.left, SWP_NOZORDER);
			}
			break;
			case 6:
			{
				if (vItems[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_SubFunction7Button.ShowWindow(SW_SHOW);
					m_Item7.ShowWindow(SW_SHOW);
					m_Item7.EnableWindow(true);
				}
				else
				{
					m_SubFunction7Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
					m_Item7.ShowWindow(SW_HIDE);
				}
				if (vItemEnabled[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_CheckMarkGraphic7.ShowWindow(SW_SHOW);
				}
				else
				{
					m_CheckMarkGraphic7.ShowWindow(SW_HIDE);
				}
				m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				SetDlgItemText(IDC_SubFunction7Button,TempButtonString);
				SetDlgItemText(IDC_Item7,TempBoxString);

				pDC = m_Item7.GetDC();
				pWnd = &m_Item7;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item7, 5) + 30;  //5 is medium large
				m_Item7.SetWindowPos(NULL, 525 - (int)(TempStringLength / vGlobalPercentToResizeY), 267, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				m_CheckMarkGraphic7.SetWindowPos(NULL, 485 - (int)(TempStringLength / vGlobalPercentToResizeY), 263, vCheckMarkGraphicSizeRect.bottom - vCheckMarkGraphicSizeRect.top, vCheckMarkGraphicSizeRect.right - vCheckMarkGraphicSizeRect.left, SWP_NOZORDER);
			}
			break;
			case 7:
			{
				if (vItems[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_SubFunction8Button.ShowWindow(SW_SHOW);
					m_Item8.ShowWindow(SW_SHOW);
					m_Item8.EnableWindow(true);
				}
				else
				{
					m_SubFunction8Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
					m_Item8.ShowWindow(SW_HIDE);
				}
				if (vItemEnabled[TempLoop + vCurrentFirstItemNumberShown])
				{
					m_CheckMarkGraphic8.ShowWindow(SW_SHOW);
				}
				else
				{
					m_CheckMarkGraphic8.ShowWindow(SW_HIDE);
				}
				m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				SetDlgItemText(IDC_SubFunction8Button,TempButtonString);
				SetDlgItemText(IDC_Item8,TempBoxString);

				pDC = m_Item8.GetDC();
				pWnd = &m_Item8;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Item8, 5) + 30;  //5 is medium large
				m_Item8.SetWindowPos(NULL, 525 - (int)(TempStringLength / vGlobalPercentToResizeY), 357, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				m_CheckMarkGraphic8.SetWindowPos(NULL, 485 - (int)(TempStringLength / vGlobalPercentToResizeY), 353, vCheckMarkGraphicSizeRect.bottom - vCheckMarkGraphicSizeRect.top, vCheckMarkGraphicSizeRect.right - vCheckMarkGraphicSizeRect.left, SWP_NOZORDER);
			}
			break;
		}
	}
	for (;TempLoop < cMaxItemsPerPage; TempLoop++)
	{
		switch (TempLoop)
		{
			case 0:
			{
				m_SubFunction1Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
				m_Item1.ShowWindow(SW_HIDE);
				m_Item1.EnableWindow(false);
				m_CheckMarkGraphic1.ShowWindow(SW_HIDE);
			}
			break;
			case 1:
			{
				m_SubFunction2Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
				m_Item2.ShowWindow(SW_HIDE);
				m_Item2.EnableWindow(false);
				m_CheckMarkGraphic2.ShowWindow(SW_HIDE);
			}
			break;
			case 2:
			{
				m_SubFunction3Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
				m_Item3.ShowWindow(SW_HIDE);
				m_Item3.EnableWindow(false);
				m_CheckMarkGraphic3.ShowWindow(SW_HIDE);
			}
			break;
			case 3:
			{
				m_SubFunction4Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
				m_Item4.ShowWindow(SW_HIDE);
				m_Item4.EnableWindow(false);
				m_CheckMarkGraphic4.ShowWindow(SW_HIDE);
			}
			break;
			case 4:
			{
				m_SubFunction5Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
				m_Item5.ShowWindow(SW_HIDE);
				m_Item5.EnableWindow(false);
				m_CheckMarkGraphic5.ShowWindow(SW_HIDE);
			}
			break;
			case 5:
			{
				m_SubFunction6Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
				m_Item6.ShowWindow(SW_HIDE);
				m_Item6.EnableWindow(false);
				m_CheckMarkGraphic6.ShowWindow(SW_HIDE);
			}
			break;
			case 6:
			{
				m_SubFunction7Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
				m_Item7.ShowWindow(SW_HIDE);
				m_Item7.EnableWindow(false);
				m_CheckMarkGraphic7.ShowWindow(SW_HIDE);
			}
			break;
			case 7:
			{
				m_SubFunction8Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
				m_Item8.ShowWindow(SW_HIDE);
				m_Item8.EnableWindow(false);
				m_CheckMarkGraphic8.ShowWindow(SW_HIDE);
			}
			break;
		}
	}
	CString TempText("");
	if (((vNumberOfItems > vCurrentFirstItemNumberShown + 8) && vCurrentPage >  0) ||
		((vNumberOfItems > vCurrentFirstItemNumberShown + 4) && vCurrentPage == 0))
	{
		m_Function5Button.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_Next);
		SetDlgItemText(IDC_Function5Button,TempText);
	}
	else
	{
		m_Function5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
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

	TempText.LoadString(IDS_MainMenu);
	SetDlgItemText(IDC_Function1Button,TempText);

	if (vChangeMade)
	{
		TempText.LoadString(IDS_SaveAndExit);
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
	}
	else
		TempText.LoadString(IDS_Exit);
	
	SetDlgItemText(IDC_Function2Button,TempText);

	if (vChangeMade)
	{
		TempText.LoadString(IDS_Cancel);
		SetDlgItemText(IDC_Function4Button,TempText);
		m_Function4Button.ShowWindow(SW_SHOW);
	}
	else
	{
		m_Function4Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());
	}
	::SetupMenu(vLocalDisplayCollection);
}

void CConfigureProductAlarmsDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	this->SetWindowText(_T("Edit Alarms"));
	CString TempTitleText(""); 
	TempTitleText.LoadString(IDS_ConfigureAlarmTitle);
	char TempBuffer[2];
	_itoa_s(vAlarmNumber,TempBuffer,2,10);
	SetDlgItemText(IDC_DialogTitleStaticText1, TempTitleText + " " + TempBuffer + " Settings");	
	m_CheckMarkGraphic1.GetWindowRect(&vCheckMarkGraphicSizeRect);
	::SetupMenu(vLocalCWndCollection);	

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
	UpdateItemDisplay();
}

void CConfigureProductAlarmsDialog::SetupMenu()
{
	for (BYTE TempLoop = 0; TempLoop < vLocalCWndCollection.GetSize(); TempLoop++)
	{
		RECT TempRectCoor;
		vLocalCWndCollection[TempLoop]->GetWindowRect(&TempRectCoor);
		vLocalCWndCollection[TempLoop]->SetWindowPos(NULL,(TempRectCoor.left)*cOldResolutionX/(vGlobalRunningData->vGlobalMonitorSizeX), 
											(TempRectCoor.top)*cOldResolutionY/(vGlobalRunningData->vGlobalMonitorSizeY),
											(TempRectCoor.right-TempRectCoor.left)*cOldResolutionX/(vGlobalRunningData->vGlobalMonitorSizeX),
											(TempRectCoor.bottom-TempRectCoor.top)*cOldResolutionY/(vGlobalRunningData->vGlobalMonitorSizeY),
											SWP_DRAWFRAME | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOZORDER);
	}
}

void CConfigureProductAlarmsDialog::OnDialogtitlestatictext1()
{
	//bring up soft key menu for help, print screen, Image, Clear, and hidden shift key
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
