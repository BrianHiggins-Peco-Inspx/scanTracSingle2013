// ConfigureProductInspectionOptions.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "ConfigureProductInspectionOptions.h"
#include "afxdialogex.h"
#include "NewConfigureProductExternalDetectorDialog.h"
#include "NumericEntryDialog.h"
#include "YesNoDialog.h"


// CConfigureProductInspectionOptions dialog
extern unsigned long vGlobalDialogTitleColor;
extern int vGlobalPasswordLevel;
extern BYTE vGlobalShiftKeyDown;
extern CProduct *vGlobalCurrentProduct;

IMPLEMENT_DYNAMIC(CConfigureProductInspectionOptions, CDialogEx)

CConfigureProductInspectionOptions::CConfigureProductInspectionOptions(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfigureProductInspectionOptions::IDD, pParent)
{
	vChangeMade = false;
	vHaveExternalDetectorsConfigured = false;
	vNumberOfInspectionsRequiredToEject = 1;
	vMultiLaneNumberOfLanes = 0;
	vMultiLaneNumberOfLinesBeforeContainer = 4;
	vMultiLaneEjectAdjacentLanes = 0;
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
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
	vLocalCWndCollection.Add(&m_SubFunction1Display);
	vLocalCWndCollection.Add(&m_SubFunction2Display);
	vLocalCWndCollection.Add(&m_SubFunction4Display);
}

CConfigureProductInspectionOptions::~CConfigureProductInspectionOptions()
{
}

void CConfigureProductInspectionOptions::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	m_Background.SetWindowPos(NULL, cMainBackgroundLeft, cMainBackgroundTop, cMainBackgroundLength, cMainBackgroundHeight, SWP_NOZORDER);

	vNumberOfInspectionsRequiredToEject = vProductEditing->vNumberOfInspectionsRequiredToEject;
	vMultiLaneNumberOfLanes = vProductEditing->vMultiLaneNumberOfLanes;
	vMultiLaneNumberOfLinesBeforeContainer = vProductEditing->vMultiLaneNumberOfLinesBeforeContainer;
	vMultiLaneEjectAdjacentLanes = vProductEditing->vMultiLaneEjectAdjacentLanes;

	CString TempText;
	TempText.LoadString(IDS_MainMenu);
	m_Function1Button.ModifyStyle(NULL, SS_CENTERIMAGE, NULL);
	SetDlgItemText(IDC_Function1Button, TempText);

	SetDlgItemText(IDC_DialogTitleStaticText1,  _T("Setup Product Inspection Options"));
	for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
	if (vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled)
		vHaveExternalDetectorsConfigured = true;

	if (vHaveExternalDetectorsConfigured)
	{
		m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
		m_SubFunction8Button.ShowWindow(SW_SHOW);
		CString TempText = " ";
		TempText.LoadString(IDS_SetupAuxiliaryDetectors);
		SetDlgItemText(IDC_SubFunction8Button, TempText);
	}
	TempText.LoadString(IDS_Exit);
	SetDlgItemText(IDC_Function2Button, TempText);

	this->SetWindowText(_T("Prod Options"));
	SetupMenu(vLocalCWndCollection);
	UpdateDisplay();
}

void CConfigureProductInspectionOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction1Display, m_SubFunction1Display);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction2Display, m_SubFunction2Display);
	//  DDX_Control(pDX, IDC_Function5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction4Display, m_SubFunction4Display);
}


BEGIN_MESSAGE_MAP(CConfigureProductInspectionOptions, CDialogEx)
	ON_STN_CLICKED(IDC_Function1Button, &CConfigureProductInspectionOptions::OnStnClickedFunction1button)
	ON_STN_CLICKED(IDC_Function2Button, &CConfigureProductInspectionOptions::OnStnClickedFunction2button)
	ON_STN_CLICKED(IDC_Function3Button, &CConfigureProductInspectionOptions::OnStnClickedFunction3button)
	ON_STN_CLICKED(IDC_Function4Button, &CConfigureProductInspectionOptions::OnStnClickedFunction4button)
	ON_STN_CLICKED(IDC_Function5Button, &CConfigureProductInspectionOptions::OnStnClickedFunction5button)
	ON_STN_CLICKED(IDC_SubFunction1Button, &CConfigureProductInspectionOptions::OnStnClickedSubfunction1button)
	ON_STN_CLICKED(IDC_SubFunction2Button, &CConfigureProductInspectionOptions::OnStnClickedSubfunction2button)
	ON_STN_CLICKED(IDC_SubFunction3Button, &CConfigureProductInspectionOptions::OnStnClickedSubfunction3button)
	ON_STN_CLICKED(IDC_SubFunction4Button, &CConfigureProductInspectionOptions::OnStnClickedSubfunction4button)
	ON_STN_CLICKED(IDC_SubFunction5Button, &CConfigureProductInspectionOptions::OnStnClickedSubfunction5button)
	ON_STN_CLICKED(IDC_SubFunction6Button, &CConfigureProductInspectionOptions::OnStnClickedSubfunction6button)
	ON_STN_CLICKED(IDC_SubFunction7Button, &CConfigureProductInspectionOptions::OnStnClickedSubfunction7button)
	ON_STN_CLICKED(IDC_SubFunction8Button, &CConfigureProductInspectionOptions::OnStnClickedSubfunction8button)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CConfigureProductInspectionOptions message handlers

void CConfigureProductInspectionOptions::OnStnClickedFunction1button()
{
	if (AbortSetupChanges())
	{
		CDialog::EndDialog(10);
	}
}

void CConfigureProductInspectionOptions::OnStnClickedFunction2button()
{
	//Exit button pressed
	if ((!vChangeMade) || (KeepSetupChanges()))
	{
		if ((vNumberOfInspectionsRequiredToEject == 0) || (vNumberOfInspectionsRequiredToEject > vProductEditing->vNumberOfInspections))
			vNumberOfInspectionsRequiredToEject = 1;

		vProductEditing->vNumberOfInspectionsRequiredToEject = vNumberOfInspectionsRequiredToEject;

		if ((!vProductEditing->vMultiLaneNumberOfLanes) && (vMultiLaneNumberOfLanes))
		if (((vLocalConfigurationData->vBulkProductMode) || ((vGlobalCurrentProduct) && (vGlobalCurrentProduct->vBulkProductMode))))		// not bulk mode
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\nMulti Lane Feature just enabled.\nBulk Mode must be Disabled.";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
		vProductEditing->vMultiLaneNumberOfLanes = vMultiLaneNumberOfLanes;
		vProductEditing->vMultiLaneNumberOfLinesBeforeContainer = vMultiLaneNumberOfLinesBeforeContainer;
		vProductEditing->vMultiLaneEjectAdjacentLanes = vMultiLaneEjectAdjacentLanes;
		CDialog::EndDialog(1);
	}
}

void CConfigureProductInspectionOptions::OnStnClickedFunction3button()
{
	// TODO: Add your control notification handler code here
}

void CConfigureProductInspectionOptions::OnStnClickedFunction4button()
{
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::OnCancel();
}

void CConfigureProductInspectionOptions::OnStnClickedFunction5button()
{
	// TODO: Add your control notification handler code here
}

void CConfigureProductInspectionOptions::OnStnClickedSubfunction1button()
{
	CNumericEntryDialog INumericEntryDialog;
	INumericEntryDialog.vEditString = dtoa(vNumberOfInspectionsRequiredToEject, 0);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the number of Inspections that must ";
	INumericEntryDialog.m_DialogTitleStaticText2 = "find a defect to Eject a container";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Should be 1 normally.  Max is number of inspections";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Default = 1, Original Value: " + INumericEntryDialog.vEditString;

	INumericEntryDialog.m_UnitsString = "Maximum value allowed is the number of inspections: " + dtoa(vProductEditing->vNumberOfInspections, 0);
	INumericEntryDialog.vMaxValue = vProductEditing->vNumberOfInspections;
	INumericEntryDialog.vMinValue = 1;
	INumericEntryDialog.vAllowZero = false;
	INumericEntryDialog.vIntegerOnly = true;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vNumberOfInspectionsRequiredToEject = _wtoi(INumericEntryDialog.vEditString);
		if (vNumberOfInspectionsRequiredToEject < 1)
			vNumberOfInspectionsRequiredToEject = 1;
		UpdateDisplay();
		SetChangeMade();
	}
	else
	if (nResponse == 10)
	{
		//Main Menu button pressed
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CConfigureProductInspectionOptions::OnStnClickedSubfunction2button()
{
}

void CConfigureProductInspectionOptions::OnStnClickedSubfunction3button()
{
	// TODO: Add your control notification handler code here
}

void CConfigureProductInspectionOptions::OnStnClickedSubfunction4button()
{
	if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nYou will need to stop inspecting\nand reload the product if you change\nthe number of lanes inspecting.";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
	CNumericEntryDialog INumericEntryDialog;
	INumericEntryDialog.vEditString = dtoa(vMultiLaneNumberOfLanes, 0);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the number of lanes product";
	INumericEntryDialog.m_DialogTitleStaticText2 = "to be inspected and ejected from.";
	INumericEntryDialog.m_DialogTitleStaticText3 = "0 and 1 = Multi Lane feature disabled.";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Default = 1, Original Value: " + INumericEntryDialog.vEditString;

	INumericEntryDialog.m_UnitsString = "Maximum value: " + dtoa(cMaximumNumberForMultiLane, 0) + " Lanes";
	INumericEntryDialog.vMaxValue = cMaximumNumberForMultiLane;
	INumericEntryDialog.vMinValue = 0;
	INumericEntryDialog.vAllowZero = true;
	INumericEntryDialog.vIntegerOnly = true;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vMultiLaneNumberOfLanes = _wtoi(INumericEntryDialog.vEditString);

		if (vMultiLaneNumberOfLanes)
		{
			INumericEntryDialog.vEditString = dtoa(vMultiLaneNumberOfLinesBeforeContainer, 0);
			INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the number of lines to show before container.";
			INumericEntryDialog.m_DialogTitleStaticText2 = "This can center container more in image.";
			INumericEntryDialog.m_DialogTitleStaticText3 = "A zero will make container at edge of image.";
			INumericEntryDialog.m_DialogTitleStaticText4 = "Default = 4, Original Value: " + INumericEntryDialog.vEditString;
			INumericEntryDialog.m_UnitsString = "Maximum value: " + dtoa(cMaximumNumberForMultiLane, 0) + " Lines";
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vMultiLaneNumberOfLinesBeforeContainer = _wtoi(INumericEntryDialog.vEditString);

				vMultiLaneEjectAdjacentLanes = vProductEditing->vMultiLaneEjectAdjacentLanes;

				CYesNoDialog TempYesNoDialog;
				TempYesNoDialog.vNoticeText = _T("\nDo you want to eject just the lane the contamiant was found in, or the lanes next to it also?");
				TempYesNoDialog.vYesButtonText = "Just found lane";
				TempYesNoDialog.vNoButtonText = "Lanes Next To It Also";
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult == IDOK)
					vMultiLaneEjectAdjacentLanes = false;
				else
					vMultiLaneEjectAdjacentLanes = true;
			}
			else
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
		if (vMultiLaneNumberOfLanes != vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
			vLocalSystemData->vCurrentProductChanged = true;
		UpdateDisplay();
		SetChangeMade();
	}
	else
	if (nResponse == 10)
	{
		//Main Menu button pressed
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CConfigureProductInspectionOptions::OnStnClickedSubfunction5button()
{
	// TODO: Add your control notification handler code here
	vMainWindowPointer->UpdateAllInspectionNames();
}

void CConfigureProductInspectionOptions::OnStnClickedSubfunction6button()
{
	// TODO: Add your control notification handler code here
}

void CConfigureProductInspectionOptions::OnStnClickedSubfunction7button()
{
	// TODO: Add your control notification handler code here
}

void CConfigureProductInspectionOptions::OnStnClickedSubfunction8button()
{
	//setup Auxiliary Detectors
	if (vHaveExternalDetectorsConfigured)
		SetupExternalDetectors(true);
}

BYTE CConfigureProductInspectionOptions::SetupExternalDetectors(bool TempInAutoSetup)
{	//Create dialog box
	if (vHaveExternalDetectorsConfigured)
	{
		CNewConfigureProductExternalDetectorDialog IConfigureProductExternalDetectorDialog;

		//Set dialog box data like how many products there are
		IConfigureProductExternalDetectorDialog.vLocalCurrentProductPointer = vProductEditing;
		IConfigureProductExternalDetectorDialog.vLocalConfigurationData = vLocalConfigurationData;
		IConfigureProductExternalDetectorDialog.vLocalSystemData = vLocalSystemData;
		IConfigureProductExternalDetectorDialog.vInAutoSetup = TempInAutoSetup;
		IConfigureProductExternalDetectorDialog.vMainWindowPointer = vMainWindowPointer;

		//Pass control to dialog box and display
		//IEditProductDialog.vEPLocalNumberOfProducts = vCPLocalNumberOfProducts;
		int nResponse = IConfigureProductExternalDetectorDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if ((nResponse == IDOK) || ((TempInAutoSetup) && (nResponse == 2)))
		{
			// TODO: Place code here to handle when the dialog is accepted
			if (IConfigureProductExternalDetectorDialog.vChangeMade)
				SetChangeMade();
			return nResponse;
		}
		else
		if (nResponse == 10)
		{
			//  Main Menu button was pressed
			OnStnClickedFunction1button();
			return 10;
		}
		else
			return 0;
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nNo Auxiliary Detectors configured in System.";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
		return 1;
	}
}

HBRUSH CConfigureProductInspectionOptions::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);

	// TODO: Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function2Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vGreenBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function1Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5);  //5 is medium large
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
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Display)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5);  //5 is medium large
		//pDC->SetTextColor(cButtonTextColor);
		if (vNumberOfInspectionsRequiredToEject != 1)
			return vLocalSystemData->vYellowBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Display)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Display, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Display)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Display, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
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
	return hbr;
}

BOOL CConfigureProductInspectionOptions::PreTranslateMessage(MSG* pMsg)
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
		if (vGlobalShiftKeyDown)
			PostThreadMessage(vGlobalCallingThreadID, cHiddenKeyPressed, 0, pMsg->wParam);
		else
			switch (pMsg->wParam)
		{
			case 113: // F2:
				OnStnClickedSubfunction5button();
				break;
			case 114: // F3:
				OnStnClickedSubfunction7button();
				break;
			case 115: // F4:
				OnStnClickedSubfunction6button();
				break;
			case 116: // F5:
				OnStnClickedSubfunction1button();
				break;
			case 117: // F6 Key - Function 5:
				OnStnClickedSubfunction4button();
				break;
			case 118: // F7 Key - Sub Function 1:
				OnStnClickedFunction1button();
				break;
			case 119: // F8 Key - Sub Function 2:
				OnStnClickedSubfunction8button();
				break;
			case 120: // F9 Key - Sub Function 3:
				OnStnClickedSubfunction2button();
				break;
			case 122: // F11 Key - Sub Function 4:
				OnStnClickedSubfunction3button();
				break;
			case 111: // / Key - Sub Function 5:
				OnStnClickedFunction2button();
				break;
			case 106: // * Key - Sub Function 6:
				OnStnClickedFunction4button();
				break;
			case 109: // - Key - Sub Function 7  Decimal Point:
				OnStnClickedFunction3button();
				break;
			case 107: // + Key - Sub Function 8:
				OnStnClickedFunction5button();
				break;
			case cPageUp: // Clear, but enter System setup password
			{
				if (vGlobalPasswordLevel)
				{
					PasswordOK(cNoPassword, false);	//todo, search on PasswordOK (cNoPassword)
				}
				else
				{
					PasswordOK(cAnyPasswordToEnter, true); //allow any password to be entered
				}
				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			}
			break;
			case 16: // Shift Key
			case 0x60:  //0 key on KeyPad
				PostThreadMessage(vGlobalCallingThreadID, cHiddenKeyPressed, 1, 0);
				break;
			case 36: // Home Key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID, cHiddenKeyPressed, 0, 0x61);
				break;
			case cPageDown: // Help
				PostThreadMessage(vGlobalCallingThreadID,
					cShowHelp, 9, 0);
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
				break;
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID, cStartChatWindowMessage, 0, 0);
				break;
		}
		return true;  //true indicates it has been handled, so do not process
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CConfigureProductInspectionOptions::SetChangeMade()
{
	if (!vChangeMade)
	{
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
		m_Function4Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_Function2Button, _T("Save\nand Exit"));
		SetDlgItemText(IDC_Function4Button, _T("Cancel"));
		vChangeMade = true;
	}
}

void CConfigureProductInspectionOptions::UpdateDisplay()
{
	SetDlgItemText(IDC_SubFunction1Display, dtoa(vNumberOfInspectionsRequiredToEject, 0));

	CString TempText = "Eject just found lane";
	if (vMultiLaneEjectAdjacentLanes)
		TempText = "Eject Adjacent lanes also";

	SetDlgItemText(IDC_SubFunction4Display, dtoa(vMultiLaneNumberOfLanes, 0) + ", " + dtoa(vMultiLaneNumberOfLinesBeforeContainer, 0) + "\n" + TempText);
}
