//ScanTrac Side View Source File
// ProductShapeHandlingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "ProductShapeHandlingDialog.h"
#include "NumericEntryDialog.h"
#include "YesNoDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BYTE vGlobalShiftKeyDown;
extern CProduct *vGlobalCurrentProduct;
extern unsigned long vGlobalDialogTitleColor;
/////////////////////////////////////////////////////////////////////////////
// CProductShapeHandlingDialog dialog


CProductShapeHandlingDialog::CProductShapeHandlingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CProductShapeHandlingDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProductShapeHandlingDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//vMainWindowPointer = NULL;
	vChangeMade = false;
	vShapeHandlingProcessIfAtSides = 2; // 0 don't, 1 reject, 2 accept
	//vShapeHandlingProcessIfAtTop = 0; // 0 don't, 1 reject, 2 accept 
	vShapeHandlingProcessIfWrongWidth = 2;// 0 don't, 1 reject, 2 accept 
	vShapeHandlingProcessIfWrongHeight = 0;// 0 don't, 1 reject, 2 accept 
	vShapeHandlingProcessIfConveyorStops = 2;// 0 don't, 1 reject, 2 accept 
	vSlipsEjectEmptyContainers = 0;

	vShapeHandlingHeightTolerance = .2;
	vShapeHandlingWidthTolerance = .2;
	vShapeHandlingEjectors = 1;

	vLocalSystemData = NULL;
	vLocalConfigurationData = NULL;
	vSlippedIndicatorTime = 0;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_SubFunction6Display);
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
	vLocalCWndCollection.Add(&m_DialogTitleStaticText1);
	vLocalCWndCollection.Add(&m_Function5ContainerWarning);
	vLocalCWndCollection.Add(&m_SubFunction1Display);
	vLocalCWndCollection.Add(&m_Function3Display);
	vLocalCWndCollection.Add(&m_SubFunction2Display);
	vLocalCWndCollection.Add(&m_SubFunction3Display);
	vLocalCWndCollection.Add(&m_SubFunction4Display);
	vLocalCWndCollection.Add(&m_SubFunction5Display);
	vLocalCWndCollection.Add(&m_SubFunction7Display);
	vLocalCWndCollection.Add(&m_SubFunction8Display);
}

void CProductShapeHandlingDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

	vSlippedIndicatorTime = vLocalConfigurationData->vSlippedIndicatorTime;
	vShapeHandlingProcessIfWrongWidth = vLocalConfigurationData->vShapeHandlingProcessIfWrongWidth;
	vShapeHandlingProcessIfWrongHeight = vLocalConfigurationData->vShapeHandlingProcessIfWrongHeight;
	//vShapeHandlingProcessIfAtTop = vLocalConfigurationData->vShapeHandlingProcessIfAtTop;
	vShapeHandlingProcessIfAtSides = vLocalConfigurationData->vShapeHandlingProcessIfAtSides;
	vSlipsEjectEmptyContainers = vLocalConfigurationData->vSlipsEjectEmptyContainers;
	vShapeHandlingProcessIfConveyorStops = vLocalConfigurationData->vShapeHandlingProcessIfConveyorStops;

	vShapeHandlingHeightTolerance = vLocalConfigurationData->vShapeHandlingHeightTolerance;
	vShapeHandlingWidthTolerance = vLocalConfigurationData->vShapeHandlingWidthTolerance;
	vShapeHandlingEjectors = vLocalConfigurationData->vShapeHandlingEjectors;
	vProductSlipPercentValue = vLocalConfigurationData->vProductSlipPercentValue;
	UpdateDisplay();
	this->SetWindowText(_T("Product Size"));
	::SetupMenu(vLocalCWndCollection);
}

void CProductShapeHandlingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProductShapeHandlingDialog)
	DDX_Control(pDX, IDC_SubFunction6Display, m_SubFunction6Display);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText1);
	DDX_Control(pDX, IDC_Function5ContainerWarningPercentageDisplay, m_Function5ContainerWarning);
	DDX_Control(pDX, IDC_Function3Display, m_Function3Display);
	DDX_Control(pDX, IDC_SubFunction1Display, m_SubFunction1Display);
	DDX_Control(pDX, IDC_SubFunction2Display, m_SubFunction2Display);
	DDX_Control(pDX, IDC_SubFunction3Display, m_SubFunction3Display);
	DDX_Control(pDX, IDC_SubFunction4Display, m_SubFunction4Display);
	DDX_Control(pDX, IDC_SubFunction5Display, m_SubFunction5Display);
	DDX_Control(pDX, IDC_SubFunction7Display, m_SubFunction7Display);
	DDX_Control(pDX, IDC_SubFunction8Display, m_SubFunction8Display);
}


BEGIN_MESSAGE_MAP(CProductShapeHandlingDialog, CDialog)
	//{{AFX_MSG_MAP(CProductShapeHandlingDialog)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_WM_SHOWWINDOW()
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
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CProductShapeHandlingDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProductShapeHandlingDialog message handlers

void CProductShapeHandlingDialog::OnFunction1Button() 
{
	//Main Menu was pressed
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::EndDialog(10);
}


void CProductShapeHandlingDialog::OnFunction2Button() 
{
	//Save and exit was pressed
	if ((!vChangeMade) || (KeepSetupChanges()))
	{
		if (vChangeMade)
		{
			vLocalConfigurationData->vSlippedIndicatorTime = vSlippedIndicatorTime;
			vLocalConfigurationData->vShapeHandlingProcessIfWrongWidth = vShapeHandlingProcessIfWrongWidth;
			vLocalConfigurationData->vShapeHandlingProcessIfWrongHeight = vShapeHandlingProcessIfWrongHeight;
			//vLocalConfigurationData->vShapeHandlingProcessIfAtTop = vShapeHandlingProcessIfAtTop;
			vLocalConfigurationData->vShapeHandlingProcessIfAtSides = vShapeHandlingProcessIfAtSides;
			vLocalConfigurationData->vSlipsEjectEmptyContainers = vSlipsEjectEmptyContainers;
			vLocalConfigurationData->vShapeHandlingProcessIfConveyorStops = vShapeHandlingProcessIfConveyorStops;

			vLocalConfigurationData->vShapeHandlingHeightTolerance = vShapeHandlingHeightTolerance;
			vLocalConfigurationData->vShapeHandlingWidthTolerance = vShapeHandlingWidthTolerance;
			vLocalConfigurationData->vShapeHandlingEjectors = vShapeHandlingEjectors;
			vLocalConfigurationData->vProductSlipPercentValue = vProductSlipPercentValue;

			//if (vLocalConfigurationData->vShapeHandlingProcessIfWrongWidth)
			{
				if (vGlobalCurrentProduct)
					vLocalConfigurationData->vShapeHandlingWidthTolerancePixels = 
						(WORD)((vLocalConfigurationData->vShapeHandlingWidthTolerance *
						vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier) + .5);
				else
					vLocalConfigurationData->vShapeHandlingWidthTolerancePixels = 
						(WORD)((vLocalConfigurationData->vShapeHandlingWidthTolerance *
						vGlobalPixelsPerUnit) + .5);
			}
			//else
			//	vLocalConfigurationData->vShapeHandlingWidthTolerancePixels = 0;

			//if (vLocalConfigurationData->vShapeHandlingProcessIfWrongHeight)
			{
				vLocalConfigurationData->vShapeHandlingHeightTolerancePixels = 
					(WORD)((vLocalConfigurationData->vShapeHandlingHeightTolerance * 
					vGlobalPixelsPerUnitInHeight) + .5);
			}
			//else
			//	vLocalConfigurationData->vShapeHandlingHeightTolerancePixels = 0;

		}
		CDialog::EndDialog(true);
	}
}

void CProductShapeHandlingDialog::OnFunction3Button() 
{
	int TempResult = vLocalConfigurationData->SelectEjectors("Wrong Size Container", &vShapeHandlingEjectors, vLocalConfigurationData, vLocalSystemData->vInAutoSetup);
	if (TempResult == IDOK)
	{
		UpdateDisplay();
		SetChangeMade();
	}
	else
	if (TempResult == 10)
	{
		//Main Menu button pressed
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CProductShapeHandlingDialog::OnFunction4Button() 
{
	//Cancel was pressed
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::OnCancel();
}

void CProductShapeHandlingDialog::OnFunction5Button() 
{
	// TODO: Add your control notification handler code here
	//If here want to pick a percentage of cans slipped for failure message.
	// vSystemBodyTriggerToXRayBeam = ATOF(INumericEntryDialog.vEditString);
	//if (vSystemBodyTriggerToXRayBeam > 15)
	//	TempProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)vSystemBodyTriggerToXRayBeam - 12);
	//else
	//	TempProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)2);
/*
						//enter distance from X-Ray Beam to body trigger
						CNumericEntryDialog INumericEntryDialog;  
						
						INumericEntryDialog.vEditString = dtoa(vSystemBodyTriggerToXRayBeam, 2);

						//Set dialog box data titles and number value
						INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the distance from the ";
						INumericEntryDialog.m_DialogTitleStaticText2 = "Container Trigger to the X-Ray Beam";
						INumericEntryDialog.m_UnitsString = "Units in Inches. Original Value: " + INumericEntryDialog.vEditString;

						INumericEntryDialog.m_DialogTitleStaticText3 = "This is fixed for this ScanTrac";
						INumericEntryDialog.m_DialogTitleStaticText4 = "";

						INumericEntryDialog.vMaxValue = 100;
						INumericEntryDialog.vMinValue = 0;
						INumericEntryDialog.vIntegerOnly = false;
						//Pass control to dialog box and display
						nResponse = INumericEntryDialog.DoModal();
						//dialog box is now closed, if user pressed select do this
						//if user pressed cancel, do nothing
						if (nResponse == IDOK)
						{

*/
	//enter the next percent of Cans Slipped before critical message is displayed
	CNumericEntryDialog INumericEntryDialog;
	INumericEntryDialog.vEditString = dtoa(vProductSlipPercentValue, 2);

	//Set dialog box data titles and number value
	INumericEntryDialog.m_DialogTitleStaticText1 = "Slipped Containers Warning Message";
	INumericEntryDialog.m_DialogTitleStaticText2 = "is generated when the percentage is exceeded.";
	INumericEntryDialog.m_UnitsString = "Value in Percentage. Current Value: " + INumericEntryDialog.vEditString;

	INumericEntryDialog.m_DialogTitleStaticText3 = "Enter Percentage for Slipped Containers.";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Value of 0, disables error message.  3% = Default.";

	INumericEntryDialog.vMaxValue = 100;
	INumericEntryDialog.vMinValue = 0;
	//INumericEntryDialog.vIntegerOnly = false;
	INumericEntryDialog.vIntegerOnly = true;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		SetDlgItemText(IDC_Function5ContainerWarningPercentageDisplay,("Warn Slip \n" + INumericEntryDialog.vEditString + "%"));
		vProductSlipPercentValue = _wtoi(INumericEntryDialog.vEditString);
		SetChangeMade();
		UpdateDisplay();
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CProductShapeHandlingDialog::OnSubFunction1Button() 
{
	CNumericEntryDialog INumericEntryDialog;
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vSlippedIndicatorTime,2);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Duration to display 'Slipped' indicator on Main Menu";
	INumericEntryDialog.m_DialogTitleStaticText2 = "System Setting";
	INumericEntryDialog.m_DialogTitleStaticText3 = "";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
		INumericEntryDialog.vEditString + ", Default Value: 1000";

	INumericEntryDialog.m_UnitsString = "Units in milli-seconds, zero is disabled";
	INumericEntryDialog.vMaxValue = 10000;
	INumericEntryDialog.vMinValue = 0;
	INumericEntryDialog.vIntegerOnly = false;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vSlippedIndicatorTime = (DWORD)ATOF(INumericEntryDialog.vEditString);
		SetChangeMade();
		UpdateDisplay();
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CProductShapeHandlingDialog::OnSubFunction2Button() 
{
	// Toggle how to handle containers on conveyor stops
	if (vShapeHandlingProcessIfConveyorStops < 2)
		vShapeHandlingProcessIfConveyorStops++;
	else
		vShapeHandlingProcessIfConveyorStops = 0;
	UpdateDisplay();
	SetChangeMade();
	vLocalSystemData->vInspectionNameChanged = true;
}

void CProductShapeHandlingDialog::OnSubFunction3Button() 
{
	//Set width tolerance
	CNumericEntryDialog INumericEntryDialog;
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vShapeHandlingWidthTolerance,2);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Tolerance of Container Width";
	INumericEntryDialog.m_DialogTitleStaticText2 = "If container is beyond tolerance from";
	INumericEntryDialog.m_DialogTitleStaticText3 = "average it will be considered the wrong size";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
		INumericEntryDialog.vEditString + ", Default Value: 0.2"; 
	//if (vLocalConfigurationData->vUseMetric)
	//	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
	//		INumericEntryDialog.vEditString + ", Default Value: 5.1"; 

	INumericEntryDialog.m_UnitsString = "Unit in " + vLocalConfigurationData->vUnitsString + ", zero is disabled";
	INumericEntryDialog.vMaxValue = 5;
	INumericEntryDialog.vMinValue = 0;
	INumericEntryDialog.vIntegerOnly = false;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vShapeHandlingWidthTolerance = ATOF(INumericEntryDialog.vEditString);
		WORD TempPixels = (WORD)((vShapeHandlingWidthTolerance * vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier) + .5);
		vShapeHandlingWidthTolerance = (double)TempPixels / (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier);
		SetChangeMade();
		UpdateDisplay();
		vLocalSystemData->vInspectionNameChanged = true;
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CProductShapeHandlingDialog::OnSubFunction4Button() 
{
	//Set height tolerance
	CNumericEntryDialog INumericEntryDialog;
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vShapeHandlingHeightTolerance,2);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Tolerance of Container Height";
	INumericEntryDialog.m_DialogTitleStaticText2 = "If container is beyond tolerance from";
	INumericEntryDialog.m_DialogTitleStaticText3 = "average it will be considered the wrong size";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
		INumericEntryDialog.vEditString + ", Default Value: 0.2"; 
	//if (vLocalConfigurationData->vUseMetric)
	//	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
	//		INumericEntryDialog.vEditString + ", Default Value: 5.1"; 

	INumericEntryDialog.m_UnitsString = "Unit in " + vLocalConfigurationData->vUnitsString +
		", zero is disabled";
	INumericEntryDialog.vMaxValue = 5;
	INumericEntryDialog.vMinValue = 0;
	INumericEntryDialog.vIntegerOnly = false;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vShapeHandlingHeightTolerance = ATOF(INumericEntryDialog.vEditString);
		WORD TempPixels = (WORD)((vShapeHandlingHeightTolerance * 
			vGlobalPixelsPerUnitInHeight) + .5);
		vShapeHandlingHeightTolerance = (double)TempPixels / vGlobalPixelsPerUnitInHeight;
		SetChangeMade();
		UpdateDisplay();
		vLocalSystemData->vInspectionNameChanged = true;
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CProductShapeHandlingDialog::OnSubFunction5Button() 
{
	// Toggle treat containers on edges as wrong size
	if (vShapeHandlingProcessIfAtSides < 2)  //2 = Pass Slips  1 = Eject slips
		vShapeHandlingProcessIfAtSides++;
	else
		vShapeHandlingProcessIfAtSides = 0;
	UpdateDisplay();
	SetChangeMade();
	vLocalSystemData->vInspectionNameChanged = true;
}

void CProductShapeHandlingDialog::OnSubFunction6Button() 
{
	if (vShapeHandlingProcessIfAtSides == 2)  //2 = Pass Slips  1 = Eject slips
	{
		vSlipsEjectEmptyContainers = !vSlipsEjectEmptyContainers;
		UpdateDisplay();
		SetChangeMade();
		vLocalSystemData->vInspectionNameChanged = true;
	}
	/*
	// Toggle treat containers on top as wrong size
	if (vShapeHandlingProcessIfAtTop < 2)
		vShapeHandlingProcessIfAtTop++;
	else
		vShapeHandlingProcessIfAtTop = 0;
	UpdateDisplay();
	SetChangeMade();
	vLocalSystemData->vInspectionNameChanged = true;
	*/
}

void CProductShapeHandlingDialog::OnSubFunction7Button() 
{
	if (vShapeHandlingProcessIfWrongWidth < 2)
		vShapeHandlingProcessIfWrongWidth++;
	else
		vShapeHandlingProcessIfWrongWidth = 0;
	UpdateDisplay();
	SetChangeMade();
	vLocalSystemData->vInspectionNameChanged = true;
}

void CProductShapeHandlingDialog::OnSubFunction8Button() 
{
	if (vShapeHandlingProcessIfWrongHeight < 2)
		vShapeHandlingProcessIfWrongHeight++;
	else
		vShapeHandlingProcessIfWrongHeight = 0;
	UpdateDisplay();
	SetChangeMade();
	vLocalSystemData->vInspectionNameChanged = true;
}

HBRUSH CProductShapeHandlingDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
		pDC->SetAttribDC(pDC->m_hDC);
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText1, 8);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Display)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5);
		if ((vSlippedIndicatorTime < 500) || (vSlippedIndicatorTime > 2000))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function3Display)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Display, 5);
		if (vShapeHandlingEjectors != 1)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Display)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Display, 5);
		if (vShapeHandlingProcessIfConveyorStops != 0)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Display)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Display, 5);
		if ((vShapeHandlingWidthTolerance < .07) || (vShapeHandlingWidthTolerance > .4))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Display) 
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Display, 5);
		if ((vShapeHandlingHeightTolerance < .07) || (vShapeHandlingHeightTolerance > .4))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Display)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Display, 5);
		if ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode) && //Not Continuous Feed, Not a Pipeline
			(vLocalConfigurationData->vScanTracType != cAtempoScanTrac) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cAllegroScanTrac) &&
			(vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac)) //Not TDs
		{
			if (vShapeHandlingProcessIfAtSides != 2)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cButtonTextColor);
				return vLocalSystemData->vYellowBrush;
			}
		}
		else
		{
			if (vShapeHandlingProcessIfAtSides != 0)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cButtonTextColor);
				return vLocalSystemData->vYellowBrush;
			}
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Display)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Display, 5);
		if (vSlipsEjectEmptyContainers)  //sense is invert so 0=Eject Empties will be default from
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}

	
	/*
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Display)
	if (vShapeHandlingProcessIfAtTop != 0)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
	}
	*/
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Display)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Display, 5);
		if (vShapeHandlingProcessIfWrongWidth != 0)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Display)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Display, 5);
		if (vShapeHandlingProcessIfWrongHeight != 0)
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
	if (pWnd->GetDlgCtrlID() == IDC_Function5ContainerWarningPercentageDisplay)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5ContainerWarning, 5);
		if (vProductSlipPercentValue != 3)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vYellowBrush;
		}
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

BOOL CProductShapeHandlingDialog::PreTranslateMessage(MSG* pMsg) 
{
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
					cShowHelp, 55, 0);
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

void CProductShapeHandlingDialog::SetChangeMade() 
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

void CProductShapeHandlingDialog::UpdateDisplay()
{
	SetDlgItemText(IDC_SubFunction1Display,dtoa(vSlippedIndicatorTime,0) + " mS");
	SetDlgItemText(IDC_Function5ContainerWarningPercentageDisplay,"Warn Slip " + dtoa(vProductSlipPercentValue) + "%");

	if (vShapeHandlingProcessIfAtSides == 0)
	{
		SetDlgItemText(IDC_SubFunction5Button, _T("Eject\nSlipped Containers"));
		SetDlgItemText(IDC_SubFunction5Display, _T("Inspect Slipped Containers"));
		this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
		m_SubFunction6Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
		m_SubFunction6Display.ShowWindow(SW_HIDE);
	}
	else
	if (vShapeHandlingProcessIfAtSides == 1)
	{
		SetDlgItemText(IDC_SubFunction5Button, _T("Pass\nSlipped Containers"));
		SetDlgItemText(IDC_SubFunction5Display, _T("Eject Slipped Containers"));
		this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
		m_SubFunction6Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
		m_SubFunction6Display.ShowWindow(SW_HIDE);
	}
	else
	{
		SetDlgItemText(IDC_SubFunction5Button, _T("Inspect Slipped Containers"));
		SetDlgItemText(IDC_SubFunction5Display, _T("Pass Slipped Containers"));
		m_SubFunction6Button.ShowWindow(SW_SHOW);
		m_SubFunction6Display.ShowWindow(SW_SHOW);

		if (vSlipsEjectEmptyContainers)
		{
			SetDlgItemText(IDC_SubFunction6Button, _T("Eject\nEmpty Containers"));
			SetDlgItemText(IDC_SubFunction6Display, _T("Pass Slipped Containers that appear empty"));
		}
		else
		{
			SetDlgItemText(IDC_SubFunction6Button, _T("Pass\nEmpty Containers"));
			SetDlgItemText(IDC_SubFunction6Display, _T("Eject Slipped Containers that appear empty"));
		}
	}

	if (vShapeHandlingProcessIfConveyorStops == 0)
	{
		SetDlgItemText(IDC_SubFunction2Button, _T("Eject On Conveyor Stops"));
		SetDlgItemText(IDC_SubFunction2Display, _T("Inspect Containers On Conveyor Stop"));
	}
	else
	if (vShapeHandlingProcessIfConveyorStops == 1)
	{
		SetDlgItemText(IDC_SubFunction2Button, _T("Pass On Conveyor Stops"));
		SetDlgItemText(IDC_SubFunction2Display, _T("Eject Containers On Conveyor Stop"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction2Button, _T("Inspct On Conveyor Stops"));
		SetDlgItemText(IDC_SubFunction2Display, _T("Pass Containers On Conveyor Stop"));
	}
	/*
	if (vShapeHandlingProcessIfAtTop == 0)
	{
		SetDlgItemText(IDC_SubFunction6Button,"Eject Containers At Top");
		SetDlgItemText(IDC_SubFunction6Display,"Inspect Containers Touching Top");
	}
	else
	if (vShapeHandlingProcessIfAtTop == 1)
	{
		SetDlgItemText(IDC_SubFunction6Button,"Pass Containers At Top");
		SetDlgItemText(IDC_SubFunction6Display,"Eject Containers Touching Top");
	}
	else
	{
		SetDlgItemText(IDC_SubFunction6Button,"Inspect Containers At Top");
		SetDlgItemText(IDC_SubFunction6Display,"Pass Containers Touching Top");
	}
	*/

	if (vShapeHandlingProcessIfWrongWidth == 0)
	{
		SetDlgItemText(IDC_SubFunction7Button, _T("Eject\nWrong\nWidth"));
		SetDlgItemText(IDC_SubFunction7Display, _T("Inspect Wrong Width Containers"));
	}
	else
	if (vShapeHandlingProcessIfWrongWidth == 1)
	{
		SetDlgItemText(IDC_SubFunction7Button, _T("Pass\nWrong\nWidth"));
		SetDlgItemText(IDC_SubFunction7Display, _T("Eject Wrong Width Containers"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction7Button, _T("Inspect\nWrong\nWidth"));
		SetDlgItemText(IDC_SubFunction7Display, _T("Pass Wrong Width Containers"));
	}

	if (vShapeHandlingProcessIfWrongHeight == 0)
	{
		SetDlgItemText(IDC_SubFunction8Button, _T("Eject\nWrong\nHeight"));
		SetDlgItemText(IDC_SubFunction8Display, _T("Inspect Wrong Height Containers"));
	}
	else
	if (vShapeHandlingProcessIfWrongHeight == 1)
	{
		SetDlgItemText(IDC_SubFunction8Button, _T("Pass\nWrong\nHeight"));
		SetDlgItemText(IDC_SubFunction8Display, _T("Eject Wrong Height Containers"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction8Button, _T("Inspect\nWrong\nHeight"));
		SetDlgItemText(IDC_SubFunction8Display, _T("Pass Wrong Height Containers"));
	}

	SetDlgItemText(IDC_SubFunction4Display,dtoa(vShapeHandlingHeightTolerance,2) + " (" +
		dtoa(vShapeHandlingHeightTolerance * vGlobalPixelsPerUnitInHeight,0) + " pixels)");

	if (vGlobalCurrentProduct)
		SetDlgItemText(IDC_SubFunction3Display,dtoa(vShapeHandlingWidthTolerance,2) + " (" + 
			dtoa(vShapeHandlingWidthTolerance * vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier,0) + " pixels)");
	else
		SetDlgItemText(IDC_SubFunction3Display,dtoa(vShapeHandlingWidthTolerance,2) + " (" + dtoa(vShapeHandlingWidthTolerance * vGlobalPixelsPerUnit,0) + " pixels)");

	CString TempString = vLocalConfigurationData->GetEjectorNames(vShapeHandlingEjectors);
	SetDlgItemText(IDC_Function3Display,TempString);
}



void CProductShapeHandlingDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
