//ScanTrac Side View Source File
// ConfigureProductContaminantReferenceDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "ConfigureProductContaminantReferenceDialog.h"
#include "NumericEntryDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
/////////////////////////////////////////////////////////////////////////////
// CConfigureProductContaminantReferenceDialog dialog


CConfigureProductContaminantReferenceDialog::CConfigureProductContaminantReferenceDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigureProductContaminantReferenceDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigureProductContaminantReferenceDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	vChangeMade = false;
	vInspectionEditing = NULL;
	vReferenceToTop = false;
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_SubFunction5Display);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_ContaminantEjector);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_LeftReference);
	vLocalCWndCollection.Add(&m_ProductNameTitle);
	vLocalCWndCollection.Add(&m_RightReference);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction6Display);
	vLocalCWndCollection.Add(&m_SubFunction2Display);
	vLocalCWndCollection.Add(&m_SubFunction7Display);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_TopReference);
	vReferenceToBottom = false;
}

void CConfigureProductContaminantReferenceDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

	SetDlgItemText(IDC_ProductNameTitle,"Product: " + *(vPointerToProductEditing->GetProductName()));
	
	if (vInspectionEditing)
	{
		m_SubFunction5Button.ShowWindow(SW_SHOW);
		m_SubFunction5Display.ShowWindow(SW_SHOW);
		vReferenceToTop = vInspectionEditing->vReferenceToTop;
		vReferenceToBottom = vInspectionEditing->vReferenceToBottom;
	}

		m_SubFunction2Button.ShowWindow(SW_SHOW);
		m_SubFunction2Display.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction2Button,  _T("Bottom Container Reference"));

		m_SubFunction6Button.ShowWindow(SW_SHOW);
		m_SubFunction6Display.ShowWindow(SW_SHOW);
	UpdateDisplay();
	this->SetWindowText(_T("Inspx-Reference"));
	::SetupMenu(vLocalCWndCollection);
}

void CConfigureProductContaminantReferenceDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigureProductContaminantReferenceDialog)
	DDX_Control(pDX, IDC_SubFunction5Display, m_SubFunction5Display);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ContaminantEjector, m_ContaminantEjector);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_LeftReference, m_LeftReference);
	DDX_Control(pDX, IDC_ProductNameTitle, m_ProductNameTitle);
	DDX_Control(pDX, IDC_RightReference, m_RightReference);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction2Display, m_SubFunction2Display);
	DDX_Control(pDX, IDC_SubFunction7Display, m_SubFunction7Display);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction6Display, m_SubFunction6Display);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_TopReference, m_TopReference);
}


BEGIN_MESSAGE_MAP(CConfigureProductContaminantReferenceDialog, CDialog)
	//{{AFX_MSG_MAP(CConfigureProductContaminantReferenceDialog)
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
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CConfigureProductContaminantReferenceDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigureProductContaminantReferenceDialog message handlers

void CConfigureProductContaminantReferenceDialog::OnFunction1Button() 
{
	// User clicked Main Menu Button to exit without saving
	if ((!vChangeMade) || (AbortSetupChanges()))
	CDialog::EndDialog(10);
}

void CConfigureProductContaminantReferenceDialog::OnFunction2Button() 
{
	//Save and Exit button pressed
	//if ((!vChangeMade) || (KeepSetupChanges()))
	{
		vInspectionEditing->vReferenceToTop = vReferenceToTop;
		vInspectionEditing->vReferenceToBottom = vReferenceToBottom;
		CDialog::EndDialog(true);
	}
}

void CConfigureProductContaminantReferenceDialog::OnFunction3Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CConfigureProductContaminantReferenceDialog::OnFunction4Button() 
{
	// pressed cancel button
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::OnCancel();
}

void CConfigureProductContaminantReferenceDialog::OnFunction5Button() 
{
}

void CConfigureProductContaminantReferenceDialog::OnSubFunction1Button() 
{
	//Top Reference
	//Set dialog box data titles and number value
	CNumericEntryDialog INumericEntryDialog;  
	INumericEntryDialog.vEditString = vReferenceTop;

	INumericEntryDialog.m_DialogTitleStaticText1 = "";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Top Reference to Align Image for Contaminants";
	CString *TempString = vPointerToProductEditing->GetProductName();
	INumericEntryDialog.m_DialogTitleStaticText3 = "Product: " + *TempString;
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + vReferenceTop;

	INumericEntryDialog.m_UnitsString = "Measured in Pixels";// + vLocalConfigurationData->vUnitsString;


	INumericEntryDialog.vMaxValue = vPointerToProductEditing->vProductImageHeightTop * vGlobalPixelsPerUnitInHeight;
	INumericEntryDialog.vIntegerOnly = false;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vReferenceTop = INumericEntryDialog.vEditString;
		UpdateDisplay();
		SetChangeMade();
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		CDialog::EndDialog(10);
	}
}

void CConfigureProductContaminantReferenceDialog::OnSubFunction2Button() 
{
	//Set dialog box data titles and number value
		CNumericEntryDialog INumericEntryDialog;  
		INumericEntryDialog.vEditString = vReferenceBottom;

		INumericEntryDialog.m_DialogTitleStaticText1 = "";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Bottom Reference to Align Image for Contaminants";
		CString *TempString = vPointerToProductEditing->GetProductName();
		INumericEntryDialog.m_DialogTitleStaticText3 = "Product: " + *TempString;
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + vReferenceBottom;

		INumericEntryDialog.m_UnitsString = "Measured in Pixels";// + vLocalConfigurationData->vUnitsString;


		INumericEntryDialog.vMaxValue = _wtoi(vReferenceTop) - 1;
		INumericEntryDialog.vIntegerOnly = false;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vReferenceBottom = INumericEntryDialog.vEditString;
			UpdateDisplay();
			SetChangeMade();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			CDialog::EndDialog(10);
		}
}

void CConfigureProductContaminantReferenceDialog::OnSubFunction3Button() 
{
	//Left Reference
	CNumericEntryDialog INumericEntryDialog;  
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = vReferenceLeft;

	INumericEntryDialog.m_DialogTitleStaticText1 = "";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Left Reference to Align Image for Contaminants";
	CString *TempString = vPointerToProductEditing->GetProductName();
	INumericEntryDialog.m_DialogTitleStaticText3 = "Product: " + *TempString;
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + vReferenceLeft;

	INumericEntryDialog.m_UnitsString = "Measured in Pixels";// + vLocalConfigurationData->vUnitsString;


	INumericEntryDialog.vMaxValue = vPointerToProductEditing->vProductImageWidth * 
		(vGlobalPixelsPerUnit * vPointerToProductEditing->vOverScanMultiplier);
	INumericEntryDialog.vIntegerOnly = false;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vReferenceLeft = INumericEntryDialog.vEditString;
		SetChangeMade();
		UpdateDisplay();
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		CDialog::EndDialog(10);
	}
}

void CConfigureProductContaminantReferenceDialog::OnSubFunction4Button() 
{
	//Right Reference
	CNumericEntryDialog INumericEntryDialog;  
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = vReferenceRight;

	INumericEntryDialog.m_DialogTitleStaticText1 = "";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Right Reference to Align Image for Contaminants";
	CString *TempString = vPointerToProductEditing->GetProductName();
	INumericEntryDialog.m_DialogTitleStaticText3 = "Product: " + *TempString;
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + vReferenceRight;

	INumericEntryDialog.m_UnitsString = "Measured in Pixels";// + vLocalConfigurationData->vUnitsString;


	INumericEntryDialog.vMaxValue = vPointerToProductEditing->vProductImageWidth * 
		vGlobalPixelsPerUnit * vPointerToProductEditing->vOverScanMultiplier;
	INumericEntryDialog.vIntegerOnly = false;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vReferenceRight = INumericEntryDialog.vEditString;
		SetChangeMade();
		UpdateDisplay();
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		CDialog::EndDialog(10);
	}
}

void CConfigureProductContaminantReferenceDialog::OnSubFunction5Button() 
{
	if (vInspectionEditing)
	{
		vReferenceToTop = !vReferenceToTop;
		UpdateDisplay();
		SetChangeMade();
	}
}

void CConfigureProductContaminantReferenceDialog::OnSubFunction6Button() 
{
	if (vInspectionEditing)
	{
		vReferenceToBottom = !vReferenceToBottom;
		UpdateDisplay();
		SetChangeMade();
	}
}

void CConfigureProductContaminantReferenceDialog::OnSubFunction7Button() 
{
	vReferenceROIsToEdges = !vReferenceROIsToEdges;
	UpdateDisplay();
	SetChangeMade();
}

void CConfigureProductContaminantReferenceDialog::OnSubFunction8Button() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CConfigureProductContaminantReferenceDialog::PreTranslateMessage(MSG* pMsg) 
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
					cShowHelp, 40, 0);
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

HBRUSH CConfigureProductContaminantReferenceDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

	if (pWnd->GetDlgCtrlID() == IDC_TopReference)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TopReference, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_LeftReference)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LeftReference, 5);
	if (pWnd->GetDlgCtrlID() == IDC_RightReference)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_RightReference, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Display, 5);
		if (vReferenceToTop)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Display, 5);
		if (vReferenceToBottom)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Display, 5);
		if (vReferenceROIsToEdges)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ProductNameTitle)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ProductNameTitle, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
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
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
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

	return hbr;
}

void CConfigureProductContaminantReferenceDialog::UpdateDisplay() 
{
	SetDlgItemText(IDC_SubFunction2Display, vReferenceBottom);
	//SetDlgItemText(IDC_ToleranceReference,vReferenceTolerance);
	SetDlgItemText(IDC_TopReference,vReferenceTop);
	SetDlgItemText(IDC_LeftReference,vReferenceLeft);
	SetDlgItemText(IDC_RightReference,vReferenceRight);

	if (vReferenceToTop)
	{
		SetDlgItemText(IDC_SubFunction5Button, _T("Don't Reference To Top"));
		SetDlgItemText(IDC_SubFunction5Display, _T("Referencing Inspection To Top"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction5Button, _T("Reference Inspection To Top"));
		SetDlgItemText(IDC_SubFunction5Display, _T("Not Referencing To Top"));
	}

	if (vReferenceToBottom)
	{
		SetDlgItemText(IDC_SubFunction6Button, _T("Don't Reference To Bottom"));
		SetDlgItemText(IDC_SubFunction6Display, _T("Referencing Inspection To Bottom"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction6Button, _T("Reference Inspection To Bottom"));
		SetDlgItemText(IDC_SubFunction6Display, _T("Not Referencing To Bottom"));
	}

	if (vReferenceROIsToEdges)
	{
		SetDlgItemText(IDC_SubFunction7Button, _T("Reference ROIs To Image"));
		SetDlgItemText(IDC_SubFunction7Display, _T("Referencing ROIs to Edges"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction7Button, _T("Reference ROIs to Edges"));
		SetDlgItemText(IDC_SubFunction7Display, _T("Referencing ROI to Image"));
	}
}

void CConfigureProductContaminantReferenceDialog::SetChangeMade() 
{
	if (!vChangeMade)
	{
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_Function4Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_Function2Button, _T("Save\nand Exit"));
		SetDlgItemText(IDC_Function4Button, _T("Cancel"));
		vChangeMade = true;
	}
}



void CConfigureProductContaminantReferenceDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
