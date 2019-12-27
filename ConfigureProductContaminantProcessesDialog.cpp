//ScanTrac Side View Source File
// ConfigureProductContaminantProcessesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "ConfigureProductContaminantProcessesDialog.h"
#include "NumericEntryDialog.h"
#include "AlphaEntryDialog.h"
#include "ConfigureProductContaminantReferenceDialog.h"
#include "SelectItemDialog.h"
#include "ConfigureProductInspectionSettings.h"
#include "Contaminant22EditThresholdsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BYTE vGlobalShiftKeyDown;
extern CProduct *vGlobalCurrentProduct;
extern unsigned long vGlobalDialogTitleColor;
/////////////////////////////////////////////////////////////////////////////
// CConfigureProductContaminantProcessesDialog dialog


CConfigureProductContaminantProcessesDialog::CConfigureProductContaminantProcessesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigureProductContaminantProcessesDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigureProductContaminantProcessesDialog)
	//}}AFX_DATA_INIT
	vChangeMade = false;
	vInspectionEditing = NULL;
    vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_SubFunction5bDisplay);
	vLocalCWndCollection.Add(&m_SubFunction5aDisplay);
	vLocalCWndCollection.Add(&m_SubFunction6bDisplay);
	vLocalCWndCollection.Add(&m_SubFunction6aDisplay);
	vLocalCWndCollection.Add(&m_SubFunction2Display);
	vLocalCWndCollection.Add(&m_SubFunction8Display);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction7Display);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction3Display);
	vLocalCWndCollection.Add(&m_ProcessStatus6);
	vLocalCWndCollection.Add(&m_ProcessStatus5);
	vLocalCWndCollection.Add(&m_ProcessStatus4);
	vLocalCWndCollection.Add(&m_ProcessStatus3);
	vLocalCWndCollection.Add(&m_ProcessStatus2);
	vLocalCWndCollection.Add(&m_ProcessStatus1);
	vLocalCWndCollection.Add(&m_Function5Button);
//	vLocalCWndCollection.Add(&m_ContaminantThreshold);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_ProductNameTitle);
	vLocalCWndCollection.Add(&m_ProductNameTitle2);
	vMaximumHeightAboveBottom = 0;
}

void CConfigureProductContaminantProcessesDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

	vStep1 = "Step 1";
	vStep2 = "Step 2";
	vStep3 = "Step 3";
	vStep4 = "Step 4";
	vStep5 = "Step 5";
	vStep6 = "Step 6";
	vStep7 = "Step 7";
	vStep8 = "Step 8";
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		vStep1 = "Reference";
		vStep2 = "Subtract Average";
		vStep3 = "Add Structure";
		vStep4 = "Add Histogram";
		vStep5 = "Enhance";
		vStep6 = "Dilation";
		vStep7 = "Image Factors";
		vStep8 = "Glass";
		m_SubFunction3Display.ShowWindow(SW_SHOW);
	}
	vThreshold = vInspectionEditing->vThreshold;
	vSize = vInspectionEditing->vSize;
	vMaximumSize = vInspectionEditing->vMaximumSize;
	vMinimumWidth = vInspectionEditing->vMinimumWidth;
	vMaximumHeightAboveBottom = vInspectionEditing->vMaximumHeightAboveBottom;
	vMinimumNumberOfItemsRequired = vInspectionEditing->vMinimumNumberOfItemsRequired;
	vMaximumNumberOfItemsRequired = vInspectionEditing->vMaximumNumberOfItemsRequired;
	vMinimumBrightnessAceptable = (float)vInspectionEditing->vMinimumBrightnessAceptable;

	vSubtractAverage = vInspectionEditing->vSubtractAverage;
	vAddStructure = vInspectionEditing->vAddStructure;
	vAddHistogram = vInspectionEditing->vAddHistogram;
	vDilationCount = vInspectionEditing->vDilationCount;
	vErodeCount = vInspectionEditing->vErodeCount;
	vErodeCountAtStart = vInspectionEditing->vErodeCountAtStart;
	//vOriginalImageFactor = vInspectionEditing->vOriginalImageFactor;
	//vEnhancedImageFactor = vInspectionEditing->vEnhancedImageFactor;
	vDilationCountAtEnd = vInspectionEditing->vDilationCountAtEnd;
	vErodeCountAtEnd = vInspectionEditing->vErodeCountAtEnd;
	//vDoImageFactors = vInspectionEditing->vDoImageFactors;
	vName = vInspectionEditing->vName;
	vEnhanceMethod = vInspectionEditing->vEnhanceMethod;
	vPerformGlassAlgorithm = vInspectionEditing->vPerformGlassAlgorithm;

	if (vInspectionEditing->vInspectionType == cVoidInspection)
	{
		SetDlgItemText(IDC_DialogTitleStaticText1, _T("Setup Product Void Processing"));
	}

	m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	SetDlgItemText(IDC_Function5Button,vStep1);
	SetDlgItemText(IDC_Function3Button, _T("More\nSettings"));

	SetDlgItemText(IDC_ProductNameTitle,"Product: " + *(vPointerToProductEditing->GetProductName()));

	if (vInspectionEditing->vInspectionType == cKernelInspection)
		SetDlgItemText(IDC_SubFunction7Button, _T("Kernel Size In Pixels"));

	if (vInspectionEditing->vInspectionType == cFloodedInspection)
		SetDlgItemText(IDC_SubFunction7Button, _T("Minimum Spout Length"));

	CString TempText = vInspectionEditing->vName + ", Type: " + 
		vInspectionEditing->GetInspectionTypeString();
	SetDlgItemText(IDC_ProductNameTitle2,TempText);

	m_SubFunction2Display.ShowWindow(SW_HIDE);

	if (vInspectionEditing->vInspectionType == cJarBottomContaminantInspection)
	{
		m_SubFunction1Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
		m_ProcessStatus1.ShowWindow(SW_HIDE);

		m_SubFunction2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
		m_ProcessStatus2.ShowWindow(SW_HIDE);

		m_SubFunction3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
		m_ProcessStatus3.ShowWindow(SW_HIDE);
		m_SubFunction3Display.ShowWindow(SW_HIDE);

		m_SubFunction4Button.ShowWindow(SW_SHOW);
		m_ProcessStatus4.ShowWindow(SW_SHOW);

		m_SubFunction5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
		m_ProcessStatus5.ShowWindow(SW_HIDE);
		m_SubFunction5aDisplay.ShowWindow(SW_HIDE);
		m_SubFunction5bDisplay.ShowWindow(SW_HIDE);

		m_SubFunction6Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
		m_ProcessStatus6.ShowWindow(SW_HIDE);
		m_SubFunction6aDisplay.ShowWindow(SW_HIDE);
		m_SubFunction6bDisplay.ShowWindow(SW_HIDE);

		SetDlgItemText(IDC_SubFunction7Button, _T("Maxiumum Pixels Up Allowed"));
	}

	if (vInspectionEditing->vInspectionType == cDripInspection)
		SetDlgItemText(IDC_SubFunction7Button, _T("Min-Max Defect Size-Width"));

	if (vInspectionEditing->vInspectionType == cItemCountInspection)
	{
		m_ProcessStatus1.SetWindowPos(NULL,122,69,100,20,SWP_NOZORDER);
		m_ProcessStatus2.SetWindowPos(NULL,122,157,100,20,SWP_NOZORDER);
		m_ProcessStatus3.SetWindowPos(NULL,122,253,100,20,SWP_NOZORDER);
		m_ProcessStatus4.SetWindowPos(NULL,122,341,100,20,SWP_NOZORDER);
		m_SubFunction7Display.SetWindowPos(NULL,410,253,100,20,SWP_NOZORDER);
		m_SubFunction8Display.SetWindowPos(NULL,410,341,100,20,SWP_NOZORDER);

		m_Function3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
		m_SubFunction2Button.ShowWindow(SW_SHOW);
		m_SubFunction2Display.ShowWindow(SW_SHOW);
		m_ProcessStatus2.ShowWindow(SW_HIDE);
		m_SubFunction3Display.ShowWindow(SW_HIDE);
		m_SubFunction5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
		m_ProcessStatus5.ShowWindow(SW_HIDE);
		m_SubFunction5aDisplay.ShowWindow(SW_HIDE);
		m_SubFunction5bDisplay.ShowWindow(SW_HIDE);
		m_SubFunction6Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
		m_ProcessStatus6.ShowWindow(SW_HIDE);
		m_SubFunction6aDisplay.ShowWindow(SW_HIDE);
		m_SubFunction6bDisplay.ShowWindow(SW_HIDE);
	}
	this->SetWindowText(_T("Params"));
	
	UpdateDisplay();
	::SetupMenu(vLocalCWndCollection);
}

void CConfigureProductContaminantProcessesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigureProductContaminantProcessesDialog)
	DDX_Control(pDX, IDC_DilationThreshold, m_SubFunction5bDisplay);
	DDX_Control(pDX, IDC_DilationCount, m_SubFunction5aDisplay);
	DDX_Control(pDX, IDC_EnhancedImageFactor, m_SubFunction6bDisplay);
	DDX_Control(pDX, IDC_OriginalImageFactor, m_SubFunction6aDisplay);
	DDX_Control(pDX, IDC_StructureAdjustFactor, m_SubFunction2Display);
	DDX_Control(pDX, IDC_SubFunction8Display, m_SubFunction8Display);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction7Display, m_SubFunction7Display);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction3Display, m_SubFunction3Display);
	DDX_Control(pDX, IDC_ProcessStatus6, m_ProcessStatus6);
	DDX_Control(pDX, IDC_ProcessStatus5, m_ProcessStatus5);
	DDX_Control(pDX, IDC_ProcessStatus4, m_ProcessStatus4);
	DDX_Control(pDX, IDC_ProcessStatus3, m_ProcessStatus3);
	DDX_Control(pDX, IDC_ProcessStatus2, m_ProcessStatus2);
	DDX_Control(pDX, IDC_ProcessStatus1, m_ProcessStatus1);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_ProductNameTitle, m_ProductNameTitle);
	DDX_Control(pDX, IDC_ProductNameTitle2, m_ProductNameTitle2);
}


BEGIN_MESSAGE_MAP(CConfigureProductContaminantProcessesDialog, CDialog)
	//{{AFX_MSG_MAP(CConfigureProductContaminantProcessesDialog)
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
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CConfigureProductContaminantProcessesDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigureProductContaminantProcessesDialog message handlers

void CConfigureProductContaminantProcessesDialog::OnFunction1Button() 
{
	// User clicked Main Menu Button to exit without saving
	if ((!vChangeMade) || (vLocalSystemData->vInAutoSetup) || (AbortSetupChanges()))
		CDialog::EndDialog(10);
}

void CConfigureProductContaminantProcessesDialog::OnFunction2Button() 
{
	//Save and Exit button pressed
	if (CheckConfiguredOK())
	{
		vInspectionEditing->vThreshold = vThreshold;
		if (vInspectionEditing->vSize != vSize)
		{
			ReportErrorMessage("Changed " + vInspectionEditing->vName + " Minimum Size from: " + dtoa(vInspectionEditing->vSize, 0) + " to " + dtoa(vSize,0) + " by Super operator", cUserChanges,0);
			vInspectionEditing->vSize = vSize;
		}
		if (vInspectionEditing->vMaximumSize != vMaximumSize)
		{
			ReportErrorMessage("Changed " + vInspectionEditing->vName + " Maximum Size from: " + dtoa(vInspectionEditing->vMaximumSize, 0) + " to " + dtoa(vMaximumSize,0) + " by operator", cUserChanges,0);
			vInspectionEditing->vMaximumSize = vMaximumSize;
		}
		if (vInspectionEditing->vMinimumBrightnessAceptable != vMinimumBrightnessAceptable)
		{
			ReportErrorMessage("Changed " + vInspectionEditing->vName + " Upper Threshold from: " + dtoa(vInspectionEditing->vMinimumBrightnessAceptable, 0) + " to " + dtoa(vMinimumBrightnessAceptable,0) + " by operator", cUserChanges,0);
			vInspectionEditing->vMinimumBrightnessAceptable = vMinimumBrightnessAceptable;
		}
		if (vInspectionEditing->vMinimumWidth != vMinimumWidth)
		{
			ReportErrorMessage("Changed " + vInspectionEditing->vName + " Minimum Width from: " + dtoa(vInspectionEditing->vMinimumWidth, 0) + " to " + dtoa(vMinimumWidth,0) + " by Super operator", cUserChanges,0);
			vInspectionEditing->vMinimumWidth = vMinimumWidth;
		}
		if (vInspectionEditing->vMaximumHeightAboveBottom != vMaximumHeightAboveBottom)
		{
			ReportErrorMessage("Changed " + vInspectionEditing->vName + " Maximum Height From Bottom from: " + dtoa(vInspectionEditing->vMaximumHeightAboveBottom, 0) + " to " + dtoa(vMaximumHeightAboveBottom,0) + " by Super operator", cUserChanges,0);
			vInspectionEditing->vMaximumHeightAboveBottom = vMaximumHeightAboveBottom;
		}
		if (vMinimumNumberOfItemsRequired != vInspectionEditing->vMinimumNumberOfItemsRequired)
		{
			ReportErrorMessage("Changed " + vInspectionEditing->vName + " Minimum Acceptable Number Of Items: " + dtoa(vInspectionEditing->vMinimumNumberOfItemsRequired, 0) + " to " + dtoa(vMinimumNumberOfItemsRequired,0) + " by operator", cUserChanges,0);
			vInspectionEditing->vMinimumNumberOfItemsRequired = vMinimumNumberOfItemsRequired;
		}
		if (vMaximumNumberOfItemsRequired != vInspectionEditing->vMaximumNumberOfItemsRequired)
		{
			ReportErrorMessage("Changed " + vInspectionEditing->vName + " Maximum Acceptable Number Of Items: " + dtoa(vInspectionEditing->vMaximumNumberOfItemsRequired, 0) + " to " + dtoa(vMaximumNumberOfItemsRequired,0) + " by operator", cUserChanges,0);
			vInspectionEditing->vMaximumNumberOfItemsRequired = vMaximumNumberOfItemsRequired;
		}

		vInspectionEditing->vSubtractAverage = vSubtractAverage;
		vInspectionEditing->vAddStructure = vAddStructure;
		vInspectionEditing->vAddHistogram = vAddHistogram;
		vInspectionEditing->vDilationCount = vDilationCount;
		vInspectionEditing->vErodeCount = vErodeCount;
		vInspectionEditing->vErodeCountAtStart = vErodeCountAtStart;
		//vInspectionEditing->vOriginalImageFactor = vOriginalImageFactor;
		//vInspectionEditing->vEnhancedImageFactor = vEnhancedImageFactor;
		vInspectionEditing->vDilationCountAtEnd = vDilationCountAtEnd;
		vInspectionEditing->vErodeCountAtEnd = vErodeCountAtEnd;
		//vInspectionEditing->vDoImageFactors = vDoImageFactors;
		vInspectionEditing->vName = vName;
		vInspectionEditing->vEnhanceMethod = vEnhanceMethod;
		vInspectionEditing->vPerformGlassAlgorithm = vPerformGlassAlgorithm;

		CDialog::EndDialog(true);
	}
}

void CConfigureProductContaminantProcessesDialog::OnFunction3Button() 
{
	if (vInspectionEditing->vInspectionType != cItemCountInspection)
	{
		//Create dialog box
		CConfigureProductInspectionSettings IConfigureProductInspectionSettings;  
	
		//Set dialog box data like how many products there are
		IConfigureProductInspectionSettings.vPointerToProductEditing = vPointerToProductEditing;
		IConfigureProductInspectionSettings.vLocalSystemData = vLocalSystemData;
		IConfigureProductInspectionSettings.vInspectionEditing = vInspectionEditing;


		IConfigureProductInspectionSettings.vLocalConfigurationData = vLocalConfigurationData;
		IConfigureProductInspectionSettings.vLocalSystemData = vLocalSystemData;
		//Pass control to dialog box and display
		//IEditProductDialog.vEPLocalNumberOfProducts = vCPLocalNumberOfProducts;
		int nResponse = IConfigureProductInspectionSettings.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			// TODO: Place code here to handle when the dialog is accepted
			if (IConfigureProductInspectionSettings.vChangeMade)
			{
				//SetChangeMade();
				UpdateDisplay();
			}
		}
		else 
		if (nResponse == 10)
		{
			//  Main Menu button was pressed
			CDialog::EndDialog(10);
		}
	}
}

void CConfigureProductContaminantProcessesDialog::OnFunction4Button() 
{
	// pressed cancel button
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::OnCancel();
}

void CConfigureProductContaminantProcessesDialog::OnFunction5Button() 
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		//setup Contaminant Reference
		//Create dialog box
		CConfigureProductContaminantReferenceDialog IConfigureProductContaminantReferenceDialog;  
	
		//Set dialog box data like how many products there are
		IConfigureProductContaminantReferenceDialog.vPointerToProductEditing = vPointerToProductEditing;
		IConfigureProductContaminantReferenceDialog.vLocalSystemData = vLocalSystemData;
		IConfigureProductContaminantReferenceDialog.vInspectionEditing = vInspectionEditing;

		IConfigureProductContaminantReferenceDialog.vReferenceLeft = dtoa(vPointerToProductEditing->vReferenceLeft,2);
		IConfigureProductContaminantReferenceDialog.vReferenceRight = dtoa(vPointerToProductEditing->vReferenceRight,2);
		IConfigureProductContaminantReferenceDialog.vReferenceTop = dtoa(vPointerToProductEditing->vReferenceTop,2);
		IConfigureProductContaminantReferenceDialog.vReferenceBottom = dtoa(vPointerToProductEditing->vReferenceBottom,2);
		IConfigureProductContaminantReferenceDialog.vReferenceROIsToEdges = vLocalConfigurationData->vReferenceROIsToEdges;

		IConfigureProductContaminantReferenceDialog.vLocalConfigurationData = vLocalConfigurationData;
		IConfigureProductContaminantReferenceDialog.vLocalSystemData = vLocalSystemData;
		//Pass control to dialog box and display
		//IEditProductDialog.vEPLocalNumberOfProducts = vCPLocalNumberOfProducts;
		int nResponse = IConfigureProductContaminantReferenceDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			// TODO: Place code here to handle when the dialog is accepted
			if (IConfigureProductContaminantReferenceDialog.vChangeMade)
			{
				vPointerToProductEditing->vReferenceLeft = (WORD)ATOF(IConfigureProductContaminantReferenceDialog.vReferenceLeft);
				vPointerToProductEditing->vReferenceRight = (WORD)ATOF(IConfigureProductContaminantReferenceDialog.vReferenceRight);
				vPointerToProductEditing->vReferenceWidth = vPointerToProductEditing->vReferenceRight - vPointerToProductEditing->vReferenceLeft;
				vPointerToProductEditing->vReferenceTop = (WORD)ATOF(IConfigureProductContaminantReferenceDialog.vReferenceTop);
				vPointerToProductEditing->vReferenceBottom = (WORD)ATOF(IConfigureProductContaminantReferenceDialog.vReferenceBottom);
				vLocalConfigurationData->vReferenceROIsToEdges = IConfigureProductContaminantReferenceDialog.vReferenceROIsToEdges;

				SetChangeMade();
			}
		}
		else 
		if (nResponse == 10)
		{
			//  Main Menu button was pressed
			CDialog::EndDialog(10);
		}
	}
}

void CConfigureProductContaminantProcessesDialog::OnSubFunction1Button() 
{
	if (vInspectionEditing->vInspectionType == cItemCountInspection)
	{
		CNumericEntryDialog INumericEntryDialog;  
	
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vMinimumBrightnessAceptable, 0);
		INumericEntryDialog.m_DialogTitleStaticText1 = "Threshold pixels must be lighter than";

		INumericEntryDialog.m_DialogTitleStaticText2 = "to be considered part of item";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Product: " + *(vPointerToProductEditing->GetProductName());
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
		INumericEntryDialog.m_UnitsString = "Measured in brightness value, 0 = Disabled";
		INumericEntryDialog.vMaxValue = 255;
		if (vThreshold)
			INumericEntryDialog.vMaxValue = vThreshold;
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.vIntegerOnly = false;

		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vMinimumBrightnessAceptable = (float)(ATOF(INumericEntryDialog.vEditString));
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
	else
	if (vInspectionEditing->vInspectionType != cJarBottomContaminantInspection)
	{
		vSubtractAverage = !vSubtractAverage;
		UpdateDisplay();
		SetChangeMade();
	}
}

void CConfigureProductContaminantProcessesDialog::OnSubFunction2Button() 
{
	if (vInspectionEditing->vInspectionType == cItemCountInspection)
	{
		CNumericEntryDialog INumericEntryDialog;  
	
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vThreshold, 0);
		INumericEntryDialog.m_DialogTitleStaticText1 = "Threshold pixels must be darker than (or equal)";

		INumericEntryDialog.m_DialogTitleStaticText2 = "to be considered part of item";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Product: " + *(vPointerToProductEditing->GetProductName());
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
		INumericEntryDialog.m_UnitsString = "Measured in brightness value, 0 = Disabled";
		INumericEntryDialog.vMaxValue = 255;
		INumericEntryDialog.vMinValue = 0;
		if (vThreshold)
			INumericEntryDialog.vMinValue = vMinimumBrightnessAceptable;
		INumericEntryDialog.vIntegerOnly = false;

		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vThreshold = (BYTE)(ATOF(INumericEntryDialog.vEditString));
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
	else
	if (vInspectionEditing->vInspectionType != cJarBottomContaminantInspection)
	{
		vAddStructure = !vAddStructure;
		UpdateDisplay();
		SetChangeMade();
	}
}

void CConfigureProductContaminantProcessesDialog::OnSubFunction3Button() 
{
	if (vInspectionEditing->vInspectionType == cItemCountInspection)
	{
		//Maximum Acceptable Size button pressed
		CNumericEntryDialog INumericEntryDialog;  
	
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vMinimumNumberOfItemsRequired, 0);
		INumericEntryDialog.m_DialogTitleStaticText1 = "";

		INumericEntryDialog.m_DialogTitleStaticText2 = "Minimum Number of Items to be Acceptable";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Product: " + *(vPointerToProductEditing->GetProductName());
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
		INumericEntryDialog.m_UnitsString = "Count of Items in ROI, 0 = Disabled";
		INumericEntryDialog.vMaxValue = 64000;
		if (vMaximumNumberOfItemsRequired)
			INumericEntryDialog.vMaxValue = vMaximumNumberOfItemsRequired;
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.vIntegerOnly = true;

		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vMinimumNumberOfItemsRequired = _wtoi(INumericEntryDialog.vEditString);
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
	else
	if (vInspectionEditing->vInspectionType != cJarBottomContaminantInspection)
	{
		vAddHistogram = !vAddHistogram;
		if (vAddHistogram)
		{
			if (PasswordOK(cTemporaryInspxPassword,false))
			{
				CNumericEntryDialog INumericEntryDialog;  
				
				//Set dialog box data titles and number value
				INumericEntryDialog.vEditString = dtoa(vPointerToProductEditing->vHistogramThreshold, 0);

				INumericEntryDialog.m_DialogTitleStaticText1 = "";
				INumericEntryDialog.m_DialogTitleStaticText2 = "Threshold for Histogram";
				INumericEntryDialog.m_DialogTitleStaticText3 = "Setting for product: " + *(vPointerToProductEditing->GetProductName());
				INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

				INumericEntryDialog.m_UnitsString = "";
					INumericEntryDialog.vMaxValue = 255;
				INumericEntryDialog.vIntegerOnly = true;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vPointerToProductEditing->vHistogramThreshold = _wtoi(INumericEntryDialog.vEditString);

					//Set dialog box data titles and number value
					INumericEntryDialog.vEditString = dtoa(vPointerToProductEditing->vHistogramLowerThreshold);

					INumericEntryDialog.m_DialogTitleStaticText1 = "";
					INumericEntryDialog.m_DialogTitleStaticText2 = "Lower Threshold of Histogram";
					INumericEntryDialog.m_DialogTitleStaticText3 = "Setting for product: " + *(vPointerToProductEditing->GetProductName());
					INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

					INumericEntryDialog.vMaxValue = 250;
					//Pass control to dialog box and display
					int nResponse = INumericEntryDialog.DoModal();
					if (nResponse == IDOK)
					{
						BYTE TempHistogramDilateTimes = vPointerToProductEditing->vHistogramDilateTimes;
						vPointerToProductEditing->vHistogramLowerThreshold = _wtoi(INumericEntryDialog.vEditString);

						//Set dialog box data titles and number value
						INumericEntryDialog.vEditString = dtoa(vPointerToProductEditing->vHistogramDilateTimes);

						INumericEntryDialog.m_DialogTitleStaticText1 = "";
						INumericEntryDialog.m_DialogTitleStaticText2 = "Number of times to Dilate Histogram";
						INumericEntryDialog.m_DialogTitleStaticText3 = "Setting for product: " + *(vPointerToProductEditing->GetProductName());
						INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

						INumericEntryDialog.vMaxValue = 25;
						//Pass control to dialog box and display
						int nResponse = INumericEntryDialog.DoModal();
						if (nResponse == IDOK)
						{
							vPointerToProductEditing->vHistogramDilateTimes = _wtoi(INumericEntryDialog.vEditString);
							if (TempHistogramDilateTimes != vPointerToProductEditing->vHistogramDilateTimes)
							{
								if ((vLocalSystemData->vITIPCDig->vProductAverageImage) && (vLocalSystemData->vITIPCDig->vProductAverageImage->vGreyImage))
								{
									vLocalSystemData->vITIPCDig->SaveThresholdImageInHistogram(vPointerToProductEditing, vPointerToProductEditing->vHistogramThreshold,
										vLocalSystemData->vITIPCDig->vProductAverageImage->vGreyImage, vPointerToProductEditing->vHistogramDilateTimes, vPointerToProductEditing->vHistogramLowerThreshold);

									vPointerToProductEditing->vSetupHistoryHistogramThreshold = vPointerToProductEditing->vHistogramThreshold;

									vLocalSystemData->vITIPCDig->vShowAverageMaskedWithHistogram = true;
									vLocalSystemData->vCurrentProductChanged = true;
								}
							}
						}
					}
					else 
					if (nResponse == 10)
					{
						//Main Menu button pressed
						CDialog::EndDialog(10);
					}
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					CDialog::EndDialog(10);
				}
			}
		}
		UpdateDisplay();
		SetChangeMade();
	}
}

void CConfigureProductContaminantProcessesDialog::OnSubFunction4Button() 
{
	//Enhance Image Button Was Pressed  Process 1
	if (vInspectionEditing->vInspectionType == cItemCountInspection)
	{
		//Maximum Acceptable Size button pressed
		CNumericEntryDialog INumericEntryDialog;  
	
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vMaximumNumberOfItemsRequired, 0);
		INumericEntryDialog.m_DialogTitleStaticText1 = "";

		INumericEntryDialog.m_DialogTitleStaticText2 = "Maximum Number of Items to be Acceptable";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Product: " + *(vPointerToProductEditing->GetProductName());
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
		INumericEntryDialog.m_UnitsString = "Count of Items in ROI, 0 = Disabled";
		INumericEntryDialog.vMaxValue = 64000;
		INumericEntryDialog.vMinValue = 0;
		if (vMinimumNumberOfItemsRequired)
			INumericEntryDialog.vMinValue = vMinimumNumberOfItemsRequired;
		INumericEntryDialog.vIntegerOnly = true;

		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vMaximumNumberOfItemsRequired = _wtoi(INumericEntryDialog.vEditString);
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
	else
	{
		if (vEnhanceMethod)
		{
			vEnhanceMethod = 0;
			vThreshold = vThreshold / 2;
		}
		else
		{
			//Create dialog box
			CSelectItemDialog ISelectItemDialog;  
			
			//Set dialog box data titles and number value
			ISelectItemDialog.vTitleString = "Select Enhance Method";
			ISelectItemDialog.vNumberOfItems = 12;
			ISelectItemDialog.vBackgroundColor = cGreen;

			ISelectItemDialog.vItems[0] = "SobelX";
			ISelectItemDialog.vItems[1] = "SobelY";
			ISelectItemDialog.vItems[2] = "SobelXY";
			ISelectItemDialog.vItems[3] = "PrewittX";
			ISelectItemDialog.vItems[4] = "PrewittY";
			ISelectItemDialog.vItems[5] = "PrewittXY";
			ISelectItemDialog.vItems[6] = "Erode";
			ISelectItemDialog.vItems[7] = "Convolve 3 X 3";
			ISelectItemDialog.vItems[8] = "Convolve 5 X 5";
			ISelectItemDialog.vItems[9] = "Convolve 5 X 5 (2 times)";
			ISelectItemDialog.vItems[10] = "Convolve 7 X 7";
			ISelectItemDialog.vItems[11] = "Convolve 3 X 3 then SobelXY";

			ISelectItemDialog.m_DialogTitleStaticText2 = "Setup Product: " + *(vPointerToProductEditing->GetProductName());
			ISelectItemDialog.m_DialogTitleStaticText3 = "SobelXY is standard";
			//Pass control to dialog box and display
			int nResponse = ISelectItemDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vEnhanceMethod = ISelectItemDialog.vItemNumberSelected;
				if (vEnhanceMethod)
				if (vInspectionEditing->vInspectionType != cJarBottomContaminantInspection)
				{
					if (vThreshold == 128)
						vThreshold = 255;
					else
					if (vThreshold < 127)
						vThreshold = vThreshold * 2;
				}
			}
		}
		/*
		if (vEnhanceMethod >= cNumberOfEnhanceMethods)
			vEnhanceMethod = 0;
		else
			vEnhanceMethod++;
		*/
		SetChangeMade();
		UpdateDisplay();
	}
}

void CConfigureProductContaminantProcessesDialog::OnSubFunction5Button() 
{
	if (vInspectionEditing->vDoContaminant22Algorithm)
	{
		CContaminant22EditThresholdsDialog IContaminant22EditThresholdsDialog;
		IContaminant22EditThresholdsDialog.vInspectionEditing = vInspectionEditing;
		IContaminant22EditThresholdsDialog.vLocalSystemData = vLocalSystemData;
		IContaminant22EditThresholdsDialog.vLocalConfigurationData = vLocalConfigurationData;
		int nResponse = IContaminant22EditThresholdsDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			CDialog::EndDialog(10);
		}
	}
	else
	//change process 5, dilation
	if (vInspectionEditing->vInspectionType != cItemCountInspection)
	if (vInspectionEditing->vInspectionType != cJarBottomContaminantInspection)
	{
		SetChangeMade();
		{
			CNumericEntryDialog INumericEntryDialog;  
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(vErodeCount, 0);

			INumericEntryDialog.m_DialogTitleStaticText1 = "";
			INumericEntryDialog.m_DialogTitleStaticText2 = "Dilation Count";
			INumericEntryDialog.m_DialogTitleStaticText3 = "Setup Product: " + *(vPointerToProductEditing->GetProductName());
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

			INumericEntryDialog.m_UnitsString = "Default: 0";
			INumericEntryDialog.vMaxValue = 25;
			INumericEntryDialog.vIntegerOnly = true;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vErodeCount = _wtoi(INumericEntryDialog.vEditString);
				//Set dialog box data titles and number value
				INumericEntryDialog.vEditString = dtoa(vDilationCount, 0);

				INumericEntryDialog.m_DialogTitleStaticText1 = "";
				INumericEntryDialog.m_DialogTitleStaticText2 = "Erode Count";
				INumericEntryDialog.m_DialogTitleStaticText3 = "Setup Product: " + *(vPointerToProductEditing->GetProductName());
				INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

				INumericEntryDialog.m_UnitsString = "Default: 0";
				INumericEntryDialog.vMaxValue = 25;
				INumericEntryDialog.vIntegerOnly = true;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vDilationCount = _wtoi(INumericEntryDialog.vEditString);
					SetDlgItemText(IDC_DilationCount,INumericEntryDialog.vEditString);
					INumericEntryDialog.vEditString = dtoa(vErodeCountAtStart, 0);

					INumericEntryDialog.m_DialogTitleStaticText1 = "";
					INumericEntryDialog.m_DialogTitleStaticText2 = "Dilation Count after Erode at Start";
					INumericEntryDialog.m_DialogTitleStaticText3 = "Setup Product: " + *(vPointerToProductEditing->GetProductName());
					INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

					INumericEntryDialog.m_UnitsString = "Default: 0";
					INumericEntryDialog.vMaxValue = 25;
					INumericEntryDialog.vIntegerOnly = true;
					//Pass control to dialog box and display
					int nResponse = INumericEntryDialog.DoModal();
					//dialog box is now closed, if user pressed select do this
					//if user pressed cancel, do nothing
					if (nResponse == IDOK)
					{
						vErodeCountAtStart = _wtoi(INumericEntryDialog.vEditString);
					}
					else 
					if (nResponse == 10)
					{
						//Main Menu button pressed
						CDialog::EndDialog(10);
					}
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					CDialog::EndDialog(10);
				}
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				CDialog::EndDialog(10);
			}
		}
		UpdateDisplay();
	}
}

void CConfigureProductContaminantProcessesDialog::OnSubFunction6Button() 
{
	//change process 6, image factors
	if (vInspectionEditing->vInspectionType != cItemCountInspection)
	if (vInspectionEditing->vInspectionType != cJarBottomContaminantInspection)
	{
		SetChangeMade();
		{
			CNumericEntryDialog INumericEntryDialog;  
			//Set dialog box data titles and number value
			CString TempString = dtoa(vErodeCountAtEnd,2);
			INumericEntryDialog.vEditString = TempString;

			INumericEntryDialog.m_DialogTitleStaticText1 = "";
			INumericEntryDialog.m_DialogTitleStaticText2 = "Dilate Count at end of processing";
			INumericEntryDialog.m_DialogTitleStaticText3 = "Setup Product: " + *(vPointerToProductEditing->GetProductName());
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

			INumericEntryDialog.m_UnitsString = "Default: 5";
			INumericEntryDialog.vMaxValue = 100;
			INumericEntryDialog.vIntegerOnly = true;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vErodeCountAtEnd = (BYTE)_wtoi(INumericEntryDialog.vEditString);
				CString TempString = dtoa(vDilationCountAtEnd,2);
				//Set dialog box data titles and number value
				INumericEntryDialog.vEditString = TempString;

				INumericEntryDialog.m_DialogTitleStaticText1 = "";
				INumericEntryDialog.m_DialogTitleStaticText2 = "Erode Count at end of processing";
				INumericEntryDialog.m_DialogTitleStaticText3 = "Setup Product: " + *(vPointerToProductEditing->GetProductName());
				INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

				INumericEntryDialog.m_UnitsString = "Default: 5";
				INumericEntryDialog.vMaxValue = 100;
				INumericEntryDialog.vIntegerOnly = true;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vDilationCountAtEnd = (BYTE)_wtoi(INumericEntryDialog.vEditString);
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					CDialog::EndDialog(10);
				}
				SetDlgItemText(IDC_DilationCount,INumericEntryDialog.vEditString);
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				CDialog::EndDialog(10);
			}
		}
		UpdateDisplay();
	}
}

void CConfigureProductContaminantProcessesDialog::OnSubFunction7Button() 
{
	//Contaminant Size button pressed
	CNumericEntryDialog INumericEntryDialog;  
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vSize, 0);
	INumericEntryDialog.m_DialogTitleStaticText2 = "";


	if (vInspectionEditing->vInspectionType == cFloodedInspection)
		INumericEntryDialog.m_DialogTitleStaticText1 = "Minimum Spout Length";
	else
	if (vInspectionEditing->vInspectionType == cKernelInspection)
		INumericEntryDialog.m_DialogTitleStaticText1 = "Length of one edge of the kernel";
	else
	if (vInspectionEditing->vInspectionType == cVoidInspection)
		INumericEntryDialog.m_DialogTitleStaticText1 = "Minimum size of Void or missing part";
	else
	if (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)
	{
		INumericEntryDialog.m_DialogTitleStaticText1 = "Minimum acceptable size of rim of lid";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Containers with no lid rim will be ejected";
	}
	else
	if (vInspectionEditing->vInspectionType == cItemCountInspection)
	{
		INumericEntryDialog.m_DialogTitleStaticText1 = "Minimum acceptable size to be an acceptable item";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Containers without correct number of items are ejected";
	}
	else
	if ((vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize))
	{
		INumericEntryDialog.m_DialogTitleStaticText1 = "Minimum acceptable object size or area of interest";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Containers with small object size will be ejected";
	}
	else
	if (vInspectionEditing->vInspectionType == cJarBottomContaminantInspection)
	{
		INumericEntryDialog.m_DialogTitleStaticText1 = "Maxiumum Pixels Up Allowed Before Considered Contaminant";
	}
	else
		INumericEntryDialog.m_DialogTitleStaticText1 = "Minimum size of Contaminant or foreign matter";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Product: " + *(vPointerToProductEditing->GetProductName());
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
	INumericEntryDialog.m_UnitsString = "Measered in number of pixels";
	INumericEntryDialog.vMaxValue = 9000000;
	if ((vInspectionEditing->vInspectionType == cItemCountInspection) && (vMaximumSize))
		INumericEntryDialog.vMaxValue = vMaximumSize;
	if (vInspectionEditing->vInspectionType == cKernelInspection)
		INumericEntryDialog.vMaxValue = 200;
	INumericEntryDialog.vMinValue = 1;
	INumericEntryDialog.vIntegerOnly = true;

	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vSize = _wtoi(INumericEntryDialog.vEditString);
		SetChangeMade();
		UpdateDisplay();
		
		if ((vInspectionEditing->vInspectionType == cDripInspection) || (vInspectionEditing->vInspectionType == cDripBySpoutInspection))
		{
			//Set dialog box data titles and number value For MAXIMUM size
			INumericEntryDialog.vEditString = dtoa(vMaximumSize, 0);
			INumericEntryDialog.m_DialogTitleStaticText1 = "Maximum size of Contaminant or foreign matter";
			INumericEntryDialog.m_DialogTitleStaticText2 = "Above this size is considered normal product";
			INumericEntryDialog.vMaxValue = 65535;
			INumericEntryDialog.vMinValue = vSize;
			INumericEntryDialog.vAllowZero = true;
			INumericEntryDialog.vIntegerOnly = true;

			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vMaximumSize = _wtoi(INumericEntryDialog.vEditString);
				SetChangeMade();
				UpdateDisplay();
				
				INumericEntryDialog.vEditString = dtoa(vMinimumWidth, 0);
				INumericEntryDialog.m_DialogTitleStaticText1 = "Minimum Width of Contaminant or foreign matter";
				INumericEntryDialog.m_DialogTitleStaticText2 = "Above this width is considered defective";
				INumericEntryDialog.vMaxValue = 65535;
				INumericEntryDialog.vMinValue = 0;
				INumericEntryDialog.vAllowZero = true;
				INumericEntryDialog.vIntegerOnly = true;

				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vMinimumWidth = _wtoi(INumericEntryDialog.vEditString);
					SetChangeMade();
					UpdateDisplay();
				
					INumericEntryDialog.vEditString = dtoa(vMaximumHeightAboveBottom, 0);
					INumericEntryDialog.m_DialogTitleStaticText1 = "Maximum Contaminant Centroid Height Above Bottom of container";
					INumericEntryDialog.m_DialogTitleStaticText2 = "Above this height is considered OK";
					INumericEntryDialog.vMaxValue = 384;
					INumericEntryDialog.vMinValue = 0;
					INumericEntryDialog.vAllowZero = true;
					INumericEntryDialog.vIntegerOnly = true;

					//Pass control to dialog box and display
					int nResponse = INumericEntryDialog.DoModal();
					//dialog box is now closed, if user pressed select do this
					//if user pressed cancel, do nothing
					if (nResponse == IDOK)
					{
						vMaximumHeightAboveBottom = _wtoi(INumericEntryDialog.vEditString);
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
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					CDialog::EndDialog(10);
				}
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				CDialog::EndDialog(10);
			}
		}
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		CDialog::EndDialog(10);
	}
}

void CConfigureProductContaminantProcessesDialog::OnSubFunction8Button() 
{
	if ((vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection) || (vInspectionEditing->vAllowChangingAdvancedOptions))
	{
		if (vPerformGlassAlgorithm == 2)
			vPerformGlassAlgorithm = 0;
		else
		if (vPerformGlassAlgorithm == 1)
			vPerformGlassAlgorithm = 2;
		else
			vPerformGlassAlgorithm = 1;
		UpdateDisplay();
		SetChangeMade();
	}
	else
	if (vInspectionEditing->vInspectionType == cItemCountInspection)
	{
		//Maximum Acceptable Size button pressed
		CNumericEntryDialog INumericEntryDialog;  
	
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vMaximumSize, 0);
		INumericEntryDialog.m_DialogTitleStaticText1 = "Maximum Acceptable Item Size";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Containers without correct number of items are ejected";

		INumericEntryDialog.m_DialogTitleStaticText3 = "Product: " + *(vPointerToProductEditing->GetProductName());
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
		INumericEntryDialog.m_UnitsString = "Measered in number of pixels, 0 = Disabled";
		INumericEntryDialog.vMaxValue = 65535;
		INumericEntryDialog.vMinValue = vSize;
		INumericEntryDialog.vAllowZero = true;
		INumericEntryDialog.vIntegerOnly = true;

		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vMaximumSize = _wtoi(INumericEntryDialog.vEditString);
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
}

BOOL CConfigureProductContaminantProcessesDialog::PreTranslateMessage(MSG* pMsg) 
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
					cShowHelp, 39, 0);
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

HBRUSH CConfigureProductContaminantProcessesDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}
/*
  if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Display)
	if (!vInspectionEditing->vEnable)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vYellowBrush;
	}
*/

	if (pWnd->GetDlgCtrlID() == IDC_ProcessStatus6)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ProcessStatus6, 5);
	if (pWnd->GetDlgCtrlID() == IDC_ProcessStatus5)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ProcessStatus5, 5);
	if (pWnd->GetDlgCtrlID() == IDC_ProcessStatus4)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ProcessStatus4, 5);
	if (pWnd->GetDlgCtrlID() == IDC_ProcessStatus3)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ProcessStatus3, 5);
	if (pWnd->GetDlgCtrlID() == IDC_ProcessStatus2)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ProcessStatus2, 5);
	if (pWnd->GetDlgCtrlID() == IDC_ProcessStatus1)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ProcessStatus1, 5);
	if (pWnd->GetDlgCtrlID() == IDC_DilationThreshold)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5bDisplay, 5);
	if (pWnd->GetDlgCtrlID() == IDC_DilationCount)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5aDisplay, 5);
	if (pWnd->GetDlgCtrlID() == IDC_EnhancedImageFactor)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6bDisplay, 5);
	if (pWnd->GetDlgCtrlID() == IDC_OriginalImageFactor)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6aDisplay, 5);
	if (pWnd->GetDlgCtrlID() == IDC_StructureAdjustFactor)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Display, 5);
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
	if (pWnd->GetDlgCtrlID() == IDC_ProductNameTitle2)
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
	
	return hbr;
}

void CConfigureProductContaminantProcessesDialog::UpdateDisplay() 
{
	if (vInspectionEditing->vInspectionType == cItemCountInspection)
	{
		m_ProcessStatus1.EnableWindow(true);
		SetDlgItemText(IDC_ProcessStatus1, dtoa(vMinimumBrightnessAceptable, 0));
		SetDlgItemText(IDC_SubFunction1Button,  _T("Minimum Brightness Threshold"));
	}
	else
	if (vSubtractAverage)
	{
		m_ProcessStatus1.EnableWindow(true);
		SetDlgItemText(IDC_SubFunction1Display, vStep2 + " Enabled");
		SetDlgItemText(IDC_SubFunction1Button, "Disable " + vStep2);
	}
	else
	{
		m_ProcessStatus1.EnableWindow(false);
		SetDlgItemText(IDC_SubFunction1Display, vStep2 + " Disabled");
		SetDlgItemText(IDC_SubFunction1Button, "Enable " + vStep2);
	}

	if (vInspectionEditing->vInspectionType == cItemCountInspection)
	{
		m_ProcessStatus2.EnableWindow(true);
		SetDlgItemText(IDC_ProcessStatus2, dtoa(vThreshold, 0));
		SetDlgItemText(IDC_SubFunction2Button,  _T("Maximum Brightness Threshold"));
	}
	else
	if (vAddStructure)
	{
		m_ProcessStatus2.EnableWindow(true);
		SetDlgItemText(IDC_ProcessStatus2,vStep3 + " Enabled");
		SetDlgItemText(IDC_SubFunction2Button,"Disable \n" + vStep3);
	}
	else
	{
		m_ProcessStatus2.EnableWindow(false);
		SetDlgItemText(IDC_ProcessStatus2,vStep3 + " Disabled");
		SetDlgItemText(IDC_SubFunction2Button,"Enable " + vStep3);
	}

	if (vInspectionEditing->vInspectionType == cItemCountInspection)
	{
		m_SubFunction3Display.ShowWindow(SW_HIDE);
		m_ProcessStatus3.EnableWindow(true);
		SetDlgItemText(IDC_ProcessStatus3, dtoa(vMinimumNumberOfItemsRequired));
		SetDlgItemText(IDC_SubFunction3Button, _T("Minimum Number Of Items"));
	}
	else
	if (vInspectionEditing->vInspectionType != cJarBottomContaminantInspection)
	{
		SetDlgItemText(IDC_SubFunction3Display,dtoa(vPointerToProductEditing->vHistogramThreshold,0) + ", " +
			dtoa(vPointerToProductEditing->vHistogramLowerThreshold,0) + ", " + dtoa(vPointerToProductEditing->vHistogramDilateTimes,0));

		if (vAddHistogram)
		{
			m_ProcessStatus3.EnableWindow(true);
			SetDlgItemText(IDC_ProcessStatus3,vStep4 + " Enabled");
			SetDlgItemText(IDC_SubFunction3Button,"Disable \n" + vStep4);
		}
		else
		{
			m_ProcessStatus3.EnableWindow(false);
			SetDlgItemText(IDC_ProcessStatus3,vStep4 + " Disabled");
			SetDlgItemText(IDC_SubFunction3Button,"Enable \n" + vStep4);
		}
	}

	if (vInspectionEditing->vInspectionType == cItemCountInspection)
	{
		m_ProcessStatus4.EnableWindow(true);
		SetDlgItemText(IDC_ProcessStatus4, dtoa(vMaximumNumberOfItemsRequired));
		SetDlgItemText(IDC_SubFunction4Button, _T("Maximum Number Of Items"));
	}
	else
	if (vEnhanceMethod)
	{
		m_ProcessStatus4.EnableWindow(true);
		CString TempString2 = "";
		if (PasswordOK(cTemporaryInspxPassword,false))
		switch (vEnhanceMethod)
		{
			case 1: TempString2 = "SobelX";
			break;
			case 2: TempString2 = "SobelY";
			break;
			case 3: TempString2 = "SobelXY";
			break;
			case 4: TempString2 = "PrewittX";
			break;
			case 5: TempString2 = "PrewittY";
			break;
			case 6: TempString2 = "PrewittXY";
			break;
			case 7: TempString2 = "Erode";
			break;
			case 8: TempString2 = "Convolve 3 X 3";
			break;
			case 9: TempString2 = "Convolve 5 X 5";
			break;
			case 10: TempString2 = "Convolve 5 X 5 (2 times)";
			break;
			case 11: TempString2 = "Convolve 7 X 7";
			break;
			case 12: TempString2 = "Convolve 3 X 3 then SobelXY";
			break;
		}
		TempString2 = TempString2 + " " + vStep5;
		SetDlgItemText(IDC_ProcessStatus4,TempString2 + " Enabled");
		SetDlgItemText(IDC_SubFunction4Button,"Disable " + vStep5 + " Process");
	}
	else
	{
		m_ProcessStatus4.EnableWindow(false);
		SetDlgItemText(IDC_ProcessStatus4,vStep5 + " Disabled");
		SetDlgItemText(IDC_SubFunction4Button,"Enable " + vStep5 + " Process");
	}

	if (vInspectionEditing->vDoContaminant22Algorithm)
	{
		SetDlgItemText(IDC_SubFunction5Button, _T("Contam. 22 Thresholds"));
		m_ProcessStatus5.ShowWindow(SW_HIDE);
		m_SubFunction5aDisplay.ShowWindow(SW_HIDE);
		m_SubFunction5bDisplay.ShowWindow(SW_HIDE);
	}
	SetDlgItemText(IDC_SubFunction5Button, _T("Dilate and Erode\nAt Start"));
	SetDlgItemText(IDC_SubFunction6Button, _T("Dilate and Erode\nAt End"));

	if (vErodeCountAtStart)
		SetDlgItemText(IDC_DilationCount, dtoa(vErodeCount, 0) + ", " + dtoa(vErodeCountAtStart, 0));
	else
		SetDlgItemText(IDC_DilationCount, dtoa(vErodeCount, 0));

	SetDlgItemText(IDC_DilationThreshold, dtoa(vDilationCount, 0));
	SetDlgItemText(IDC_OriginalImageFactor,dtoa(vErodeCountAtEnd,2));
	SetDlgItemText(IDC_EnhancedImageFactor,dtoa(vDilationCountAtEnd,2));
	m_ProcessStatus5.ShowWindow(SW_HIDE);
	m_ProcessStatus6.ShowWindow(SW_HIDE);

	if (vInspectionEditing->vInspectionType == cItemCountInspection)
	{
		SetDlgItemText(IDC_SubFunction7Display, dtoa(vSize));
		SetDlgItemText(IDC_SubFunction7Button,  _T("Minimum Size Of Item"));
	}
	else
	{
		CString TempText = dtoa(vSize, 0);
		if ((vMaximumSize) && ((vInspectionEditing->vInspectionType == cDripInspection) || (vInspectionEditing->vInspectionType == cDripBySpoutInspection)))
			TempText = TempText + " - " + dtoa(vMaximumSize, 0);

		if ((vMinimumWidth) && (vInspectionEditing->vInspectionType == cDripInspection))
			TempText = TempText + ", Min Width: " + dtoa(vMinimumWidth,0);

		if ((vMaximumHeightAboveBottom) && (vInspectionEditing->vInspectionType == cDripInspection))
			TempText = TempText + ", Max Height Pos: " + dtoa(vMaximumHeightAboveBottom);

		SetDlgItemText(IDC_SubFunction7Display, TempText);
	}

	if (vInspectionEditing->vInspectionType == cItemCountInspection)
	{
		SetDlgItemText(IDC_SubFunction8Display, dtoa(vMaximumSize));
		SetDlgItemText(IDC_SubFunction8Button, _T("Maximum Size Of Item"));
	}
	else
	if (vPerformGlassAlgorithm == 0)
	{
		m_SubFunction8Display.EnableWindow(false);
		SetDlgItemText(IDC_SubFunction8Display, _T("Glass Algorithms Disabled"));
		SetDlgItemText(IDC_SubFunction8Button, _T("Enable Glass2C Algorithm"));
	}
	else
	if (vPerformGlassAlgorithm == 1)
	{
		m_SubFunction8Display.EnableWindow(true);
		SetDlgItemText(IDC_SubFunction8Display, _T("Glass2C Enabled"));
		SetDlgItemText(IDC_SubFunction8Button, _T("Enable Glass2D Algorithm"));
	}
	else
	{
		m_SubFunction8Display.EnableWindow(true);
		SetDlgItemText(IDC_SubFunction8Display, _T("Glass2D Enabled"));
		SetDlgItemText(IDC_SubFunction8Button, _T("Disable Glass Algorithms"));
	}
}

void CConfigureProductContaminantProcessesDialog::SetChangeMade() 
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

bool CConfigureProductContaminantProcessesDialog::CheckConfiguredOK()
{
	bool TempOK = true;
	return TempOK;
}



void CConfigureProductContaminantProcessesDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
