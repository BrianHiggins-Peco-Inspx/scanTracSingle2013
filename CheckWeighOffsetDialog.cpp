// CheckWeighOffsetDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "CheckWeighOffsetDialog.h"
#include "afxdialogex.h"
#include "NumericEntryDialog.h"


// CCheckWeighOffsetDialog dialog

IMPLEMENT_DYNAMIC(CCheckWeighOffsetDialog, CDialogEx)

extern unsigned long vGlobalDialogTitleColor;
extern BYTE vGlobalUseCommaForDecimalPoint;
extern BYTE vGlobalShiftKeyDown;

CCheckWeighOffsetDialog::CCheckWeighOffsetDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckWeighOffsetDialog::IDD, pParent)
{
	vWeightsTakenTimerHandle = 31234;
	vEnteringWeight = false;
	vCheckWeighInspection = NULL;
	vEditString = "";
	vContainersSentSoFar = 0;
	vAverageMeasuredWeightOfContainer = 0;
	for (BYTE TempLoop = 0; TempLoop < 50; TempLoop++)
		vContainerWeights[TempLoop] = 0;

	vCursorPosition = 0;
	vDeleteAll = true;
	vCheckWeighInspectionNumber = 0;
	vMainWindowPointer = NULL;
	vLocalSystemData = NULL;
	vLocalConfigurationData = NULL;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText1);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText2);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText3);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText4);
	vLocalCWndCollection.Add(&m_UnitsString);
	vLocalCWndCollection.Add(&m_EditStaticText);
	vLocalCWndCollection.Add(&m_Key0);
	vLocalCWndCollection.Add(&m_Key1);
	vLocalCWndCollection.Add(&m_Key2);
	vLocalCWndCollection.Add(&m_Key3);
	vLocalCWndCollection.Add(&m_Key4);
	vLocalCWndCollection.Add(&m_Key5);
	vLocalCWndCollection.Add(&m_Key6);
	vLocalCWndCollection.Add(&m_Key7);
	vLocalCWndCollection.Add(&m_Key8);
	vLocalCWndCollection.Add(&m_Key9);
	vLocalCWndCollection.Add(&m_KeyClear);
	vLocalCWndCollection.Add(&m_KeyHelp);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_FullTextDisplay);
	vLocalCWndCollection.Add(&m_FullTextDisplay3);
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
}

CCheckWeighOffsetDialog::~CCheckWeighOffsetDialog()
{
}

void CCheckWeighOffsetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_DialogTitleStaticText3, m_DialogTitleStaticText3);
	DDX_Control(pDX, IDC_DialogTitleStaticText4, m_DialogTitleStaticText4);
	DDX_Control(pDX, IDC_DialogTitleStaticText8, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_EditStaticText, m_EditStaticText);
	DDX_Control(pDX, IDC_Key0, m_Key0);
	DDX_Control(pDX, IDC_Key1, m_Key1);
	DDX_Control(pDX, IDC_Key2, m_Key2);
	DDX_Control(pDX, IDC_Key3, m_Key3);
	DDX_Control(pDX, IDC_Key4, m_Key4);
	DDX_Control(pDX, IDC_Key5, m_Key5);
	DDX_Control(pDX, IDC_Key6, m_Key6);
	DDX_Control(pDX, IDC_Key7, m_Key7);
	DDX_Control(pDX, IDC_Key8, m_Key8);
	DDX_Control(pDX, IDC_Key9, m_Key9);
	DDX_Control(pDX, IDC_KeyClear, m_KeyClear);
	DDX_Control(pDX, IDC_KeyHelp, m_KeyHelp);
	DDX_Control(pDX, IDC_UnitsString, m_UnitsString);
	DDX_Control(pDX, IDC_FullTextDisplay, m_FullTextDisplay);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText1);
	DDX_Control(pDX, IDC_DialogTitleStaticText2, m_DialogTitleStaticText2);
	DDX_Control(pDX, IDC_FullTextDisplay3, m_FullTextDisplay3);
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


BEGIN_MESSAGE_MAP(CCheckWeighOffsetDialog, CDialogEx)
	ON_STN_CLICKED(IDC_Function1Button, &CCheckWeighOffsetDialog::OnFunction1button)
	ON_STN_CLICKED(IDC_Function2Button, &CCheckWeighOffsetDialog::OnFunction2button)
	ON_STN_CLICKED(IDC_Function3Button, &CCheckWeighOffsetDialog::OnFunction3button)
	ON_STN_CLICKED(IDC_Function4Button, &CCheckWeighOffsetDialog::OnFunction4button)
	ON_STN_CLICKED(IDC_Function5Button, &CCheckWeighOffsetDialog::OnFunction5button)
	ON_STN_CLICKED(IDC_SubFunction4Button, &CCheckWeighOffsetDialog::OnSubfunction4button)
	ON_STN_CLICKED(IDC_SubFunction8Button, &CCheckWeighOffsetDialog::OnSubfunction8button)
	ON_STN_CLICKED(IDC_SubFunction1Button, &CCheckWeighOffsetDialog::OnSubfunction1button)
	ON_STN_CLICKED(IDC_SubFunction2Button, &CCheckWeighOffsetDialog::OnSubfunction2button)
	ON_STN_CLICKED(IDC_SubFunction3Button, &CCheckWeighOffsetDialog::OnSubfunction3button)
	ON_STN_CLICKED(IDC_SubFunction5Button, &CCheckWeighOffsetDialog::OnSubfunction5button)
	ON_STN_CLICKED(IDC_SubFunction6Button, &CCheckWeighOffsetDialog::OnSubfunction6button)
	ON_STN_CLICKED(IDC_SubFunction7Button, &CCheckWeighOffsetDialog::OnSubfunction7button)
	ON_STN_CLICKED(IDC_Key1, &CCheckWeighOffsetDialog::OnKey1)
	ON_STN_CLICKED(IDC_Key2, &CCheckWeighOffsetDialog::OnKey2)
	ON_STN_CLICKED(IDC_Key3, &CCheckWeighOffsetDialog::OnKey3)
	ON_STN_CLICKED(IDC_Key4, &CCheckWeighOffsetDialog::OnKey4)
	ON_STN_CLICKED(IDC_Key5, &CCheckWeighOffsetDialog::OnKey5)
	ON_STN_CLICKED(IDC_Key6, &CCheckWeighOffsetDialog::OnKey6)
	ON_STN_CLICKED(IDC_Key7, &CCheckWeighOffsetDialog::OnKey7)
	ON_STN_CLICKED(IDC_Key8, &CCheckWeighOffsetDialog::OnKey8)
	ON_STN_CLICKED(IDC_Key9, &CCheckWeighOffsetDialog::OnKey9)
	ON_STN_CLICKED(IDC_Key0, &CCheckWeighOffsetDialog::OnKey0)
	ON_STN_CLICKED(IDC_KeyClear, &CCheckWeighOffsetDialog::OnKeyclear)
	ON_STN_CLICKED(IDC_KeyHelp, &CCheckWeighOffsetDialog::OnKeyhelp)
	ON_WM_SETTINGCHANGE()
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_DialogTitleStaticText8, &CCheckWeighOffsetDialog::OnDialogtitlestatictext8)
END_MESSAGE_MAP()

void CCheckWeighOffsetDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL, cMainBackgroundLeft, cMainBackgroundTop, cMainBackgroundLength, cMainBackgroundHeight, SWP_NOZORDER);

	m_FullTextDisplay.SetWindowPos(NULL, cMainBackgroundLeft + 50, cMainBackgroundTop + 30, cMainBackgroundLength - 100, (cMainBackgroundHeight / 3) - 20, SWP_NOZORDER);
	m_FullTextDisplay3.SetWindowPos(NULL, cMainBackgroundLeft + 110, cMainBackgroundTop + (cMainBackgroundHeight / 3), cMainBackgroundLength - 200, (cMainBackgroundHeight * 2 / 3) - 20, SWP_NOZORDER);

	vLocalConfigurationData->vNumberOfCheckWeighOffsetSamples = 4;
	UpdateFullTextDisplay();

	vCursorPosition = vEditString.GetLength();

	CString TempText = " ";
	TempText.LoadString(IDS_MainMenu);
	SetDlgItemText(IDC_Function1Button,TempText);
	TempText.LoadString(IDS_SaveAndExit);
	SetDlgItemText(IDC_Function2Button,TempText);
	TempText.LoadString(IDS_Cancel);
	SetDlgItemText(IDC_Function4Button,TempText);

	UpdateDisplay();
	SetNumLock(true);
	SetCursor();
	this->SetWindowText(_T("Cal Weight Offset"));

	if (vCheckWeighInspection->vCheckWeighUnits == "Grams")
		SetDlgItemText(IDC_UnitsString, _T("In Grams.  Zero = no offset"));
	
	::SetupMenu(vLocalCWndCollection);		
}


// CCheckWeighOffsetDialog message handlers


void CCheckWeighOffsetDialog::OnFunction1button()
{
	// User clicked Main Menu Button to exit without saving
	CDialog::EndDialog(10);
}

void CCheckWeighOffsetDialog::OnFunction2button()
{
	//User pressed Save and exit
	if (vEnteringWeight)
	{
		BYTE TempOldCursorPosition = vEditString.Find(_T("&"));
		if (TempOldCursorPosition >= 0)
			vEditString.Delete(TempOldCursorPosition,1);

		if (vEditString == "")
			vEditString = "0";

		CDialog::EndDialog(true);
	}
}



void CCheckWeighOffsetDialog::OnFunction3button()
{
	ProcessKey(cF3);
}


void CCheckWeighOffsetDialog::OnFunction4button()
{
	//Cancel was pressed
	CDialog::OnCancel();
}


void CCheckWeighOffsetDialog::OnFunction5button()
{
	// TODO: Add your control notification handler code here
}


void CCheckWeighOffsetDialog::OnSubfunction1button()
{
	//CNumericEntryDialog INumericEntryDialog;
	//
	////Set dialog box data titles and number value
	//INumericEntryDialog.vEditString = dtoa(vLocalConfigurationData->vNumberOfCheckWeighOffsetSamples,0);

	//INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the number of times to send the measured";
	//INumericEntryDialog.m_DialogTitleStaticText2 = "container to calibrate the weight offset.";
	//INumericEntryDialog.m_DialogTitleStaticText3 = "For Weight Trending Inpection: " + vCheckWeighInspection->vName;
	//INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

	//INumericEntryDialog.m_UnitsString = "Default: 4. Maximum: 50 samples.";
	//INumericEntryDialog.vMaxValue = 50;
	//INumericEntryDialog.vMinValue = 1;
	//INumericEntryDialog.vIntegerOnly = true;
	////Pass control to dialog box and display
	//int nResponse = INumericEntryDialog.DoModal();
	////dialog box is now closed, if user pressed select do this
	////if user pressed cancel, do nothing
	//if (nResponse == IDOK)
	//{
	//	vLocalConfigurationData->vNumberOfCheckWeighOffsetSamples = _wtoi(INumericEntryDialog.vEditString);
	//	UpdateDisplay();
	//}
	//else 
	//if (nResponse == 10)
	//{
	//	//Main Menu button pressed
	//	Sleep(1); //is the equivelent of a yeild statement;
	//	EndDialog(10);
	//}
}


void CCheckWeighOffsetDialog::OnSubfunction2button()
{
	// TODO: Add your control notification handler code here
}


void CCheckWeighOffsetDialog::OnSubfunction3button()
{
	// TODO: Add your control notification handler code here
}


void CCheckWeighOffsetDialog::OnSubfunction4button()
{
	if (vEnteringWeight)
	{
		//Left arrow key pressed
		if (vDeleteAll)
			vCursorPosition = 0;
		if (vCursorPosition > 0)
			vCursorPosition--;
		vDeleteAll = false;
		SetCursor();
	}
}

void CCheckWeighOffsetDialog::OnSubfunction5button()
{
	// TODO: Add your control notification handler code here
}


void CCheckWeighOffsetDialog::OnSubfunction6button()
{
	// TODO: Add your control notification handler code here
}


void CCheckWeighOffsetDialog::OnSubfunction7button()
{
	// TODO: Add your control notification handler code here
}

void CCheckWeighOffsetDialog::OnSubfunction8button()
{
	if (vEnteringWeight)
	{
		//Right Arrow Button pressed
		vCursorPosition++;
		vDeleteAll = false;
		SetCursor();
	}
}



void CCheckWeighOffsetDialog::OnKey1()
{
	ProcessKey(c1);
}


void CCheckWeighOffsetDialog::OnKey2()
{
	ProcessKey(c2);
}


void CCheckWeighOffsetDialog::OnKey3()
{
	ProcessKey(c3);
}


void CCheckWeighOffsetDialog::OnKey4()
{
	ProcessKey(c4);
}


void CCheckWeighOffsetDialog::OnKey5()
{
	ProcessKey(c5);
}


void CCheckWeighOffsetDialog::OnKey6()
{
	ProcessKey(c6);
}


void CCheckWeighOffsetDialog::OnKey7()
{
	ProcessKey(c7);
}


void CCheckWeighOffsetDialog::OnKey8()
{
	ProcessKey(c8);
}


void CCheckWeighOffsetDialog::OnKey9()
{
	ProcessKey(c9);
}


void CCheckWeighOffsetDialog::OnKey0()
{
	ProcessKey(c0);
}


void CCheckWeighOffsetDialog::OnKeyclear()
{
	//if backspace character
	if (vEnteringWeight)
	{
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
}


void CCheckWeighOffsetDialog::OnKeyhelp()
{
	//Help Key Pressed
	PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 0, 0);
}

HBRUSH CCheckWeighOffsetDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	pDC->SetBkMode(TRANSPARENT);

	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText8)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);  
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_FullTextDisplay)
	{
		TextSize = SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_FullTextDisplay, 5);  
		return vLocalSystemData->vLightBlueBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_FullTextDisplay3)
	{
		TextSize = SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_FullTextDisplay3, 5);  
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBlueBrush;
	}

	//if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText1, 5);  
	//	pDC->SetTextColor(cDarkBlue);
	//}

	if (pWnd->GetDlgCtrlID() == IDC_EditStaticText) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EditStaticText, 5);  
		if (vDeleteAll)
		{
			pDC->SetBkColor(cBlue);
			pDC->SetTextColor(cWhite);
		}
		else
			pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vWhiteBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Background) 
	{
		return vLocalSystemData->vLightBlueBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText1, 5);  
		return vLocalSystemData->vLightBlueBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText2) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText2, 5);  
		return vLocalSystemData->vLightBlueBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText3) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText3, 5);  
		return vLocalSystemData->vLightBlueBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText4) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText4, 5);  
		return vLocalSystemData->vLightBlueBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_UnitsString)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_UnitsString, 5);  
		return vLocalSystemData->vLightBlueBrush;
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
		pDC->SetTextColor(cButtonTextColor);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vGreenBrush;
		
		pDC->SetBkMode(TRANSPARENT);
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

void CCheckWeighOffsetDialog::SetCursor()
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
	if (!vDeleteAll)
		vEditString.Insert(vCursorPosition, _T("&"));
	SetDlgItemText(IDC_EditStaticText,vEditString);
	//UpdateData(false);
}

BOOL CCheckWeighOffsetDialog::PreTranslateMessage(MSG* pMsg)
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
	if (pMsg->message == cNewImageReadyToDisplay)
	{
		//if (vLocalSystemData->vITIPCDig->vNextImageToDisplayLock.Lock())
		//{
		//	vContainerWeights[vContainersSentSoFar] = vCheckWeighInspection->ConvertToWeight(vLocalSystemData->vITIPCDig->vNextImageToDisplay->vLastDensityValue[vCheckWeighInspectionNumber]);
		//	vLocalSystemData->vITIPCDig->vNextImageToDisplayLock.Unlock();
		//}
		if (vContainersSentSoFar < 50)
		if (!vEnteringWeight)
		if (vCheckWeighInspection)
		{
			vContainerWeights[vContainersSentSoFar] = vCheckWeighInspection->ConvertToWeight(vCheckWeighInspection->vLastWeight);
			vContainersSentSoFar++;
			//CString TempWeightsString = "";
			//for (BYTE TempLoop = 0; TempLoop < vContainersSentSoFar; TempLoop++)
			//{
			//	vAverageMeasuredWeightOfContainer = vAverageMeasuredWeightOfContainer + vContainerWeights[TempLoop];
			//	if (TempLoop < 6)
			//	{
			//		if (TempLoop)
			//			TempWeightsString = TempWeightsString + ", ";

			//		TempWeightsString = TempWeightsString + dtoa(vContainerWeights[TempLoop], 2);
			//	}
			//}
			//SetDlgItemText(IDC_DialogTitleStaticText1, dtoa(vContainersSentSoFar, 0) + " of " + dtoa(vLocalConfigurationData->vNumberOfCheckWeighOffsetSamples, 0) + " sent.  Average: " + dtoa(vAverageMeasuredWeightOfContainer, 2));
			//SetDlgItemText(IDC_DialogTitleStaticText2, "Weights: " + TempWeightsString);


			if (vContainersSentSoFar == vLocalConfigurationData->vNumberOfCheckWeighOffsetSamples)
			{
				double TempMinimumWeight = 999999999;
				double TempMaximumWeight = 0;

				vAverageMeasuredWeightOfContainer = 0;
				for (BYTE TempLoop = 0; TempLoop < vContainersSentSoFar; TempLoop++)
				{
					if (vContainerWeights[TempLoop] < TempMinimumWeight)
						TempMinimumWeight = vContainerWeights[TempLoop];
					if (vContainerWeights[TempLoop] > TempMaximumWeight)
						TempMaximumWeight = vContainerWeights[TempLoop];

					vAverageMeasuredWeightOfContainer = vAverageMeasuredWeightOfContainer + vContainerWeights[TempLoop];
				}
				vAverageMeasuredWeightOfContainer = vAverageMeasuredWeightOfContainer / vContainersSentSoFar;

				if (TempMinimumWeight * 1.1 < TempMaximumWeight)
				{
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n4 measurements vary by more than 10%\nMust have had a bad reading.\nSend through 4 more times, or Cancel.";
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
					
					vContainersSentSoFar = 0;
					vAverageMeasuredWeightOfContainer = 0;
					for (BYTE TempLoop = 0; TempLoop < 50; TempLoop++)
						vContainerWeights[TempLoop] = 0;
				}
				else
				{
					int TimerResult = SetTimer(vWeightsTakenTimerHandle,2000,NULL);
					if (!TimerResult)
						ReportErrorMessage("Error-vWeightsTakenTimerHandle Timer Failed",cEMailInspx,32000);
				}
			}

			UpdateFullTextDisplay();
		}
		return true;  //true indicates it has been handled, so do not process
	}
	else
	{
		CWnd * TempWindow = NULL;
		// TODO: Add your specialized code here and/or call the base class
		if (pMsg->message == WM_KEYDOWN)
		{
			//get an alpha numeric key press
			if (((pMsg->wParam >= c0) && (pMsg->wParam <= c9)) || (pMsg->wParam == cF3))
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
				//didn't get alphanumeric key press, so reset key press counter
			}
			//check for function keys
			if (vGlobalShiftKeyDown)
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
			else
			switch (pMsg->wParam)
			{
				case 0x6e: // F3:
					OnFunction3button();
				break;
				case 0x0d: // F2:
					OnFunction2button();
				break;
				case 113: // F2:
					//OnSubFunction5button();
				break;
				case 114: // F3:
					//OnSubFunction7button();
				break;
				case 115: // F4:
					//OnSubFunction6button();
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
					//OnSubFunction2button();
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
				//case 109: // - Key - Sub Function 7  Decimal Point:
					//ProcessKey(cF3);  called up above
				//break;
				case 107: // + Key - Sub Function 8:
					//OnFunction5button();
				break;
				case cPageUp: // PageUp:
					OnKeyclear();
				break;
				case 16: // Shift Key
					PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
				break;
				case 36:  //Home key, Screen Capture
					vGlobalShiftKeyDown = true;
					PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
				break;
				case cPageDown: // Help
					OnKeyhelp();
					{
						TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText8);
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
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CCheckWeighOffsetDialog::ProcessKey(BYTE TempKey)
{
	if (vEnteringWeight)
	if (!vGlobalShiftKeyDown)
	{
		//if hitting same key as before, back up one character
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

void CCheckWeighOffsetDialog::StartEnteringWeight()
{
	vEnteringWeight = true;
	m_FullTextDisplay.ShowWindow(SW_HIDE);
	m_FullTextDisplay3.ShowWindow(SW_HIDE);
	m_Function3Button.ShowWindow(SW_SHOW); //show decimal point
	m_Function2Button.ShowWindow(SW_SHOW); //show Save and Exit
	CString TempText = " ";
	TempText.LoadString(IDS_SaveAndExit);
	SetDlgItemText(IDC_Function2Button,TempText);
	TempText.LoadString(IDS_Cancel);
	SetDlgItemText(IDC_Function4Button,TempText);

	m_SubFunction4Button.ShowWindow(SW_SHOW);
	m_SubFunction8Button.ShowWindow(SW_SHOW);

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

	m_EditStaticText.ShowWindow(SW_SHOW);
	m_DialogTitleStaticText4.ShowWindow(SW_SHOW);
	SetDlgItemText(IDC_DialogTitleStaticText4, "Nominal weight: " + dtoa(vCheckWeighInspection->vCheckWeighNominalWeight, 2));
	m_UnitsString.ShowWindow(SW_SHOW);
	
	m_DialogTitleStaticText1.ShowWindow(SW_SHOW);
	SetDlgItemText(IDC_DialogTitleStaticText1, _T("Enter weight of sent container"));
	m_DialogTitleStaticText2.ShowWindow(SW_SHOW);
	SetDlgItemText(IDC_DialogTitleStaticText2, "Calculated average: " + dtoa(vAverageMeasuredWeightOfContainer, 2) + " " + vCheckWeighInspection->vCheckWeighUnits);
	m_DialogTitleStaticText3.ShowWindow(SW_SHOW);
	Invalidate();
}

void CCheckWeighOffsetDialog::UpdateDisplay()
{
	if (vLocalConfigurationData->vNumberOfCheckWeighOffsetSamples < 1)
		vLocalConfigurationData->vNumberOfCheckWeighOffsetSamples = 4;
	SetDlgItemText(IDC_DialogTitleStaticText1, dtoa(vContainersSentSoFar, 0) + " of " + dtoa(vLocalConfigurationData->vNumberOfCheckWeighOffsetSamples, 0) + " sent.");
	SetDlgItemText(IDC_DialogTitleStaticText2, "Weigh a container and send through " + dtoa(vLocalConfigurationData->vNumberOfCheckWeighOffsetSamples, 0) + " times.");
	SetDlgItemText(IDC_DialogTitleStaticText3, "Inspection: " + vCheckWeighInspection->vName);
}

void CCheckWeighOffsetDialog::OnKeyHelp() 
{
	//Help Key Pressed
	PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 24, 0);
}


void CCheckWeighOffsetDialog::UpdateFullTextDisplay()
{
	SetDlgItemText(IDC_FullTextDisplay, "Pass container of known weight " + dtoa(vLocalConfigurationData->vNumberOfCheckWeighOffsetSamples, 0) + " times.");

	CString TempText = "";
	for (BYTE TempLoop = 0; TempLoop < 4; TempLoop++)
	{
		if (vContainerWeights[TempLoop])
			TempText = TempText + "\nWeight " + dtoa(TempLoop + 1, 0) + ": " + dtoa(vContainerWeights[TempLoop], 2);
		else
			TempText = TempText + "\nWeight " + dtoa(TempLoop + 1, 0) + ": --";
	}

	if (vAverageMeasuredWeightOfContainer)
		TempText = TempText + "\nAverage: " + dtoa(vAverageMeasuredWeightOfContainer, 2);

	SetDlgItemText(IDC_FullTextDisplay3, TempText);
	
}


void CCheckWeighOffsetDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
  if (nIDEvent == vWeightsTakenTimerHandle)
  {
		KillTimer(vWeightsTakenTimerHandle);
		StartEnteringWeight();
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CCheckWeighOffsetDialog::OnDialogtitlestatictext8()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
