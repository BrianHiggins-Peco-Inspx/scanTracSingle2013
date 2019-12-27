//ScanTrac Side View Source File
// ProductDensityCompensationDialog.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "ProductDensityCompensationDialog.h"
#include "NumericEntryDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
/////////////////////////////////////////////////////////////////////////////
// CProductDensityCompensationDialog dialog


CProductDensityCompensationDialog::CProductDensityCompensationDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CProductDensityCompensationDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProductDensityCompensationDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	vDriftCompensationEnabled = true;
	vDriftCompensationWaitTime = 500;
	vNumberOfImagesForDriftDensity = 10;
	vDriftCompensationFastThreshold = 5;
	vDriftCompensationFastAdjust = .1;
	vDriftCompensationVeryFastThreshold = 40;
	vDriftCompensationVeryFastAdjust = .5;
	vDriftCompensationSlowThreshold = 5;
	vDriftCompensationSlowAdjust = 1;
	vDriftCompensationMaximumAdjust = 500;
	//vDriftCompensationThresholdReductionMaximum = 0;
	//vDriftCompensationThresholdReductionFactor = 10;
	vUseDACAndADCinDensityCompensation = true;

	//vMainWindowPointer = NULL;
	vChangeMade = false;

	vLocalSystemData = NULL;
	vLocalConfigurationData = NULL;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_DialogStaticText1);
	vLocalCWndCollection.Add(&m_Function3Display);
	vLocalCWndCollection.Add(&m_Function5Display);
	vLocalCWndCollection.Add(&m_SubFunction0Button);
	vLocalCWndCollection.Add(&m_SubFunction0Display);
	vLocalCWndCollection.Add(&m_Info);
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
	vLocalCWndCollection.Add(&m_SubFunction3Display);
	vLocalCWndCollection.Add(&m_SubFunction4Display);
	vLocalCWndCollection.Add(&m_SubFunction5Display);
	vLocalCWndCollection.Add(&m_SubFunction6Display);
	vLocalCWndCollection.Add(&m_SubFunction7Display);
	vLocalCWndCollection.Add(&m_SubFunction8Display);
	vLocalCWndCollection.Add(&m_Background);
	
	
}

void CProductDensityCompensationDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

	vUseDACAndADCinDensityCompensation = 
		vLocalConfigurationData->vUseDACAndADCinDensityCompensation;
	vDriftCompensationEnabled = vLocalConfigurationData->vDriftCompensationEnabled;
	vDriftCompensationWaitTime = vLocalConfigurationData->vDriftCompensationWaitTime;
	vNumberOfImagesForDriftDensity = vLocalConfigurationData->vNumberOfImagesForDriftDensity;
	vDriftCompensationFastThreshold = vLocalConfigurationData->vDriftCompensationFastThreshold;
	vDriftCompensationFastAdjust = vLocalConfigurationData->vDriftCompensationFastAdjust;
	vDriftCompensationVeryFastThreshold = vLocalConfigurationData->vDriftCompensationVeryFastThreshold;
	vDriftCompensationVeryFastAdjust = vLocalConfigurationData->vDriftCompensationVeryFastAdjust;
	vDriftCompensationSlowThreshold = vLocalConfigurationData->vDriftCompensationSlowThreshold;
	vDriftCompensationSlowAdjust = vLocalConfigurationData->vDriftCompensationSlowAdjust;
	vDriftCompensationMaximumAdjust = vLocalConfigurationData->vDriftCompensationMaximumAdjust;
	//vDriftCompensationThresholdReductionMaximum = vLocalConfigurationData->vDriftCompensationThresholdReductionMaximum;
	//vDriftCompensationThresholdReductionFactor = vLocalConfigurationData->vDriftCompensationThresholdReductionFactor;
	UpdateDisplay();
	this->SetWindowText(_T("Density Comp"));
	::SetupMenu(vLocalCWndCollection);
}

void CProductDensityCompensationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProductDensityCompensationDialog)
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
	DDX_Control(pDX, IDC_SubFunction1Display, m_SubFunction1Display);
	DDX_Control(pDX, IDC_SubFunction2Display, m_SubFunction2Display);
	DDX_Control(pDX, IDC_SubFunction3Display, m_SubFunction3Display);
	DDX_Control(pDX, IDC_SubFunction4Display, m_SubFunction4Display);
	DDX_Control(pDX, IDC_SubFunction5Display, m_SubFunction5Display);
	DDX_Control(pDX, IDC_SubFunction6Display, m_SubFunction6Display);
	DDX_Control(pDX, IDC_SubFunction7Display, m_SubFunction7Display);
	DDX_Control(pDX, IDC_SubFunction8Display, m_SubFunction8Display);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogStaticText1);
	DDX_Control(pDX, IDC_Function3Display, m_Function3Display);
	DDX_Control(pDX, IDC_Function5Display, m_Function5Display);
	DDX_Control(pDX, IDC_SubFunction0Button, m_SubFunction0Button);
	DDX_Control(pDX, IDC_SubFunction0Display, m_SubFunction0Display);
	DDX_Control(pDX, IDC_Info, m_Info);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProductDensityCompensationDialog, CDialog)
	//{{AFX_MSG_MAP(CProductDensityCompensationDialog)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
	ON_BN_CLICKED(IDC_Function5Button, OnFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction0Button, OnSubFunction0Button)
	ON_BN_CLICKED(IDC_SubFunction1Button, OnSubFunction1Button)
	ON_BN_CLICKED(IDC_SubFunction2Button, OnSubFunction2Button)
	ON_BN_CLICKED(IDC_SubFunction3Button, OnSubFunction3Button)
	ON_BN_CLICKED(IDC_SubFunction4Button, OnSubFunction4Button)
	ON_BN_CLICKED(IDC_SubFunction5Button, OnSubFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction6Button, OnSubFunction6Button)
	ON_BN_CLICKED(IDC_SubFunction7Button, OnSubFunction7Button)
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CProductDensityCompensationDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProductDensityCompensationDialog message handlers

void CProductDensityCompensationDialog::OnFunction1Button() 
{
	//Main Menu was pressed
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::EndDialog(10);
}

void CProductDensityCompensationDialog::OnFunction2Button() 
{
	//Save and exit was pressed
	if ((!vChangeMade) || (KeepSetupChanges()))
	{
		if (vChangeMade)
		{
			vLocalConfigurationData->vUseDACAndADCinDensityCompensation = 
				vUseDACAndADCinDensityCompensation;
			vLocalConfigurationData->vDriftCompensationEnabled = vDriftCompensationEnabled;
			vLocalConfigurationData->vDriftCompensationWaitTime = vDriftCompensationWaitTime;
			vLocalConfigurationData->vNumberOfImagesForDriftDensity = vNumberOfImagesForDriftDensity;
			vLocalConfigurationData->vDriftCompensationFastThreshold = vDriftCompensationFastThreshold;
			vLocalConfigurationData->vDriftCompensationFastAdjust = vDriftCompensationFastAdjust;
			vLocalConfigurationData->vDriftCompensationVeryFastThreshold = vDriftCompensationVeryFastThreshold;
			vLocalConfigurationData->vDriftCompensationVeryFastAdjust = vDriftCompensationVeryFastAdjust;
			vLocalConfigurationData->vDriftCompensationSlowThreshold = vDriftCompensationSlowThreshold;
			vLocalConfigurationData->vDriftCompensationSlowAdjust = vDriftCompensationSlowAdjust;
			vLocalConfigurationData->vDriftCompensationMaximumAdjust = vDriftCompensationMaximumAdjust;
			//vLocalConfigurationData->vDriftCompensationThresholdReductionMaximum = 
			//	vDriftCompensationThresholdReductionMaximum;
			//vLocalConfigurationData->vDriftCompensationThresholdReductionFactor = 
			//	vDriftCompensationThresholdReductionFactor;
		}
		CDialog::EndDialog(true);
	}
}

void CProductDensityCompensationDialog::OnFunction3Button() 
{
	vDriftCompensationEnabled = !vDriftCompensationEnabled;
  UpdateDisplay();	
	SetChangeMade();
}

void CProductDensityCompensationDialog::OnFunction4Button() 
{
	//Cancel was pressed
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::OnCancel();
}

void CProductDensityCompensationDialog::OnFunction5Button() 
{
	CNumericEntryDialog INumericEntryDialog;
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vDriftCompensationMaximumAdjust,2);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Maximum amount offset adjust allowed";
	INumericEntryDialog.m_DialogTitleStaticText2 = "";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Default: 500";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
		INumericEntryDialog.vEditString;

	INumericEntryDialog.m_UnitsString = "Units in milli-volts";
	INumericEntryDialog.vMaxValue = 500;
	INumericEntryDialog.vMinValue = 0;
	INumericEntryDialog.vIntegerOnly = true;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vDriftCompensationMaximumAdjust = ATOF(INumericEntryDialog.vEditString);
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

void CProductDensityCompensationDialog::OnSubFunction0Button() 
{
	vUseDACAndADCinDensityCompensation = !vUseDACAndADCinDensityCompensation;
	UpdateDisplay();
	SetChangeMade();
}

void CProductDensityCompensationDialog::OnSubFunction1Button() 
{
	CNumericEntryDialog INumericEntryDialog;
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vDriftCompensationWaitTime, 0);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Time to wait after adjust offset before ";
	INumericEntryDialog.m_DialogTitleStaticText2 = "analyze images for next adjustment";
	INumericEntryDialog.m_DialogTitleStaticText3 = "System Setting for all products";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString + ", Default: 100";

	INumericEntryDialog.m_UnitsString = "Unit in Milli-Seconds";
	INumericEntryDialog.vMaxValue = 65000;
	INumericEntryDialog.vMinValue = 100;
	INumericEntryDialog.vIntegerOnly = true;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vDriftCompensationWaitTime = _wtoi(INumericEntryDialog.vEditString);
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

void CProductDensityCompensationDialog::OnSubFunction2Button() 
{
	CNumericEntryDialog INumericEntryDialog;
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vDriftCompensationSlowThreshold,2);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Adjust offset if images are brighter or darker than this";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Below this Hysteresis no adjustment is made";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Fast speed takes over when greater than Fast Threshold";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
		INumericEntryDialog.vEditString + ", Default: 5";

	INumericEntryDialog.m_UnitsString = "Unit in pixel intensity";
	INumericEntryDialog.vMaxValue = vDriftCompensationFastThreshold;
	INumericEntryDialog.vMinValue = .1;
	INumericEntryDialog.vIntegerOnly = false;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vDriftCompensationSlowThreshold = ATOF(INumericEntryDialog.vEditString);
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

void CProductDensityCompensationDialog::OnSubFunction3Button() 
{
	CNumericEntryDialog INumericEntryDialog;
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vDriftCompensationFastThreshold,2);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Adjust offset faster if images brighter or darker than this";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Below this Hysteresis slow or no adjustment is made";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Fast offset adjustment is made when over this Hysteresis";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
		INumericEntryDialog.vEditString + ", Default: 10";

	INumericEntryDialog.m_UnitsString = "Unit in pixel intensity";
	INumericEntryDialog.vMaxValue = 100;
	INumericEntryDialog.vMinValue = vDriftCompensationSlowThreshold;
	INumericEntryDialog.vIntegerOnly = false;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vDriftCompensationFastThreshold = ATOF(INumericEntryDialog.vEditString);
		UpdateDisplay();
		SetChangeMade();
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vDriftCompensationVeryFastThreshold,2);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Adjust offset quickly if images are brighter or darker than this";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Below this Hysteresis fast, slow or no adjustment is made";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Very Fast offset adjustment is made when over this Hysteresis";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
			INumericEntryDialog.vEditString + ", Default: 20";

		INumericEntryDialog.m_UnitsString = "Unit in pixel intensity";
		INumericEntryDialog.vMaxValue = 100;
		INumericEntryDialog.vMinValue = vDriftCompensationFastThreshold;
		INumericEntryDialog.vIntegerOnly = false;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vDriftCompensationVeryFastThreshold = ATOF(INumericEntryDialog.vEditString);
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
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CProductDensityCompensationDialog::OnSubFunction4Button() 
{
	//CNumericEntryDialog INumericEntryDialog;
	//
	////Set dialog box data titles and number value
	//INumericEntryDialog.vEditString = dtoa(vDriftCompensationThresholdReductionFactor,2);

	//INumericEntryDialog.m_DialogTitleStaticText1 = "Factor to multiply the density delta to reduce Thresholds";
	//INumericEntryDialog.m_DialogTitleStaticText2 = "When the density is off by more than the slow Hysteresis";
	//INumericEntryDialog.m_DialogTitleStaticText3 = "the thresholds are reduced by the difference times this factor.";
	//INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
	//	INumericEntryDialog.vEditString + ", Default: 1";

	//INumericEntryDialog.m_UnitsString = "Unit in pixel intensity";
	//INumericEntryDialog.vMaxValue = 100;
	//INumericEntryDialog.vMinValue = .01;
	//INumericEntryDialog.vIntegerOnly = false;
	////Pass control to dialog box and display
	//int nResponse = INumericEntryDialog.DoModal();
	////dialog box is now closed, if user pressed select do this
	////if user pressed cancel, do nothing
	//if (nResponse == IDOK)
	//{
	//	vDriftCompensationThresholdReductionFactor = ATOF(INumericEntryDialog.vEditString);
	//	UpdateDisplay();
	//	SetChangeMade();
	//}
	//else 
	//if (nResponse == 10)
	//{
	//	//Main Menu button pressed
	//	Sleep(1); //is the equivelent of a yeild statement;
	//	CDialog::EndDialog(10);
	//}
}

void CProductDensityCompensationDialog::OnSubFunction5Button() 
{
	CNumericEntryDialog INumericEntryDialog;
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vNumberOfImagesForDriftDensity, 0);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Number of images to analyze to determine";
	INumericEntryDialog.m_DialogTitleStaticText2 = "amount of next offset adjustment";
	INumericEntryDialog.m_DialogTitleStaticText3 = "System Setting for all products";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString + ", Default: 10";

	INumericEntryDialog.m_UnitsString = "Unit in number of images";
	INumericEntryDialog.vMaxValue = 250;
	INumericEntryDialog.vMinValue = 1;
	INumericEntryDialog.vIntegerOnly = true;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vNumberOfImagesForDriftDensity = _wtoi(INumericEntryDialog.vEditString);
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

void CProductDensityCompensationDialog::OnSubFunction6Button() 
{
	CNumericEntryDialog INumericEntryDialog;
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vDriftCompensationSlowAdjust,4);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Amount to adjust offset if intensity > slow threshold";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Below this threshold no adjustment is made";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Fast speed takes over when greater than Fast Threshold";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
		INumericEntryDialog.vEditString + ", Default: 1";

	INumericEntryDialog.m_UnitsString = "Units in mill-volts";
	INumericEntryDialog.vMaxValue = 50;
	INumericEntryDialog.vMinValue = 1;
	INumericEntryDialog.vIntegerOnly = true;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vDriftCompensationSlowAdjust = ATOF(INumericEntryDialog.vEditString);
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

void CProductDensityCompensationDialog::OnSubFunction7Button() 
{
	CNumericEntryDialog INumericEntryDialog;
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vDriftCompensationFastAdjust,3);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Amount to adjust offset if intensity > fast threshold";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Below the threshold slow or no adjustment is made";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Fast takes over when greater than Fast Threshold";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
		INumericEntryDialog.vEditString + ", Default: 15";

	INumericEntryDialog.m_UnitsString = "Units in mill-volts";
	INumericEntryDialog.vMaxValue = 50;
	INumericEntryDialog.vMinValue = 1;
	INumericEntryDialog.vIntegerOnly = true;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vDriftCompensationFastAdjust = ATOF(INumericEntryDialog.vEditString);
		UpdateDisplay();
		SetChangeMade();
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vDriftCompensationVeryFastAdjust,2);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Amount to adjust offset if intensity > very fast threshold";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Below the threshold fast, slow or no adjustment is made";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Very Fast is used when greater than Very Fast Threshold";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
			INumericEntryDialog.vEditString + ", Default: 30";

		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vDriftCompensationVeryFastAdjust = ATOF(INumericEntryDialog.vEditString);
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
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CProductDensityCompensationDialog::OnSubFunction8Button() 
{
	//CNumericEntryDialog INumericEntryDialog;
	//
	////Set dialog box data titles and number value
	//INumericEntryDialog.vEditString = dtoa(vDriftCompensationThresholdReductionMaximum,2);

	//INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the maximum threshold reduction";
	//INumericEntryDialog.m_DialogTitleStaticText2 = "";
	//INumericEntryDialog.m_DialogTitleStaticText3 = "";
	//INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
	//	INumericEntryDialog.vEditString + ", Default: 0 = Disabled";

	//INumericEntryDialog.m_UnitsString = "Unit in pixel intensity";
	//INumericEntryDialog.vMaxValue = 100;
	//INumericEntryDialog.vMinValue = 0;
	//INumericEntryDialog.vIntegerOnly = false;
	////Pass control to dialog box and display
	//int nResponse = INumericEntryDialog.DoModal();
	////dialog box is now closed, if user pressed select do this
	////if user pressed cancel, do nothing
	//if (nResponse == IDOK)
	//{
	//	vDriftCompensationThresholdReductionMaximum = (BYTE)ATOF(INumericEntryDialog.vEditString);
	//	UpdateDisplay();
	//	SetChangeMade();
	//}
	//else 
	//if (nResponse == 10)
	//{
	//	//Main Menu button pressed
	//	Sleep(1); //is the equivelent of a yeild statement;
	//	CDialog::EndDialog(10);
	//}
}

BOOL CProductDensityCompensationDialog::PreTranslateMessage(MSG* pMsg) 
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
					cShowHelp, 57, 0);
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

HBRUSH CProductDensityCompensationDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize = 8;
	
	pDC->SetAttribDC(pDC->m_hDC);
	pDC->SetBkMode(TRANSPARENT);
	// TODO: Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogStaticText1, 8);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction0Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction0Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction0Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction0Display, 5);  //5 is medium large
		pDC->SetTextColor(cBlack);
		if (!vUseDACAndADCinDensityCompensation)
			return vLocalSystemData->vYellowBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_Function3Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Function3Display, 5);  //5 is medium large
		if (vDriftCompensationEnabled)
			return vLocalSystemData->vGreenBrush;
		else
			return vLocalSystemData->vRedBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_Function5Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Function5Display, 5);  //5 is medium large
		pDC->SetTextColor(cBlack);
		if (vDriftCompensationMaximumAdjust != 500)
			return vLocalSystemData->vYellowBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5);  //5 is medium large
		pDC->SetTextColor(cBlack);
		if (vDriftCompensationWaitTime != 100)
			return vLocalSystemData->vYellowBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Display, 5);  //5 is medium large
		pDC->SetTextColor(cBlack);
		if ((vDriftCompensationSlowThreshold < 3) || (vDriftCompensationSlowThreshold > 6))
			return vLocalSystemData->vYellowBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Display, 5);  //5 is medium large
		pDC->SetTextColor(cBlack);
		if ((vDriftCompensationFastThreshold < 5) || (vDriftCompensationFastThreshold > 25) ||
			(vDriftCompensationVeryFastThreshold < 10) || (vDriftCompensationVeryFastThreshold > 50))
			return vLocalSystemData->vYellowBrush;
	}

	//if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Display)
	//if ((vDriftCompensationThresholdReductionFactor < 1) || (vDriftCompensationFastThreshold > 10))
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	pDC->SetTextColor(cButtonTextColor);
	//	return vLocalSystemData->vYellowBrush;
	//}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Display, 5);  //5 is medium large
		pDC->SetTextColor(cBlack);
		if (vNumberOfImagesForDriftDensity != 10)
			return vLocalSystemData->vYellowBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Display, 8);  //5 is medium large
		pDC->SetTextColor(cBlack);
		if (vDriftCompensationSlowAdjust != 1)
			return vLocalSystemData->vYellowBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Display, 5);  //5 is medium large
		pDC->SetTextColor(cBlack);
		if ((vDriftCompensationFastAdjust < 10) || (vDriftCompensationFastAdjust > 40) ||
			(vDriftCompensationVeryFastAdjust < 20) || (vDriftCompensationVeryFastAdjust > 70))
			return vLocalSystemData->vYellowBrush;
	}

	//if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Display)
	//if (vDriftCompensationThresholdReductionMaximum != 0)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	pDC->SetTextColor(cButtonTextColor);
	//	return vLocalSystemData->vYellowBrush;
	//}

	if ((pWnd->GetDlgCtrlID() == IDC_Background) ||
		(pWnd->GetDlgCtrlID() == IDC_Info))
	{
		return vLocalSystemData->vLightGreenBrush;
  }

  if (pWnd->GetDlgCtrlID() == IDC_Function1Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

  if (pWnd->GetDlgCtrlID() == IDC_Function2Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

  if (pWnd->GetDlgCtrlID() == IDC_Function3Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

  if (pWnd->GetDlgCtrlID() == IDC_Function4Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function4Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

  if (pWnd->GetDlgCtrlID() == IDC_Function5Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

  if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

  if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

  if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

  if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

  if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

  if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

  if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

  if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CProductDensityCompensationDialog::SetChangeMade() 
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

void CProductDensityCompensationDialog::UpdateDisplay()
{
	if (vUseDACAndADCinDensityCompensation)
	{
		SetDlgItemText(IDC_SubFunction0Button, _T("Use Only ADC"));
		SetDlgItemText(IDC_SubFunction0Display, _T("Using DAC and ADC in Density Compensation"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction0Button, _T("Use DAC/ADC"));
		SetDlgItemText(IDC_SubFunction0Display, _T("Using Only ADC in Density Compensation"));
	}
	if (vDriftCompensationEnabled)
	{
		SetDlgItemText(IDC_Function3Button, _T("Disable Correction"));
		SetDlgItemText(IDC_Function3Display, _T("Product Density Correction Enabled"));
	}
	else
	{
		SetDlgItemText(IDC_Function3Button, _T("Enable Correction"));
		SetDlgItemText(IDC_Function3Display, _T("Product Density Correction Disabled"));
	}
	SetDlgItemText(IDC_SubFunction1Display,dtoa(vDriftCompensationWaitTime, 0) + "ms");
	SetDlgItemText(IDC_SubFunction5Display,dtoa(vNumberOfImagesForDriftDensity, 0));
	SetDlgItemText(IDC_SubFunction2Display,dtoa(vDriftCompensationSlowThreshold,2));
	SetDlgItemText(IDC_SubFunction3Display,dtoa(vDriftCompensationFastThreshold,2) + ", " + dtoa(vDriftCompensationVeryFastThreshold,2));
	//SetDlgItemText(IDC_SubFunction4Display,dtoa(vDriftCompensationThresholdReductionFactor,2));
	SetDlgItemText(IDC_SubFunction6Display,dtoa(vDriftCompensationSlowAdjust,4));
	SetDlgItemText(IDC_Function5Display,dtoa(vDriftCompensationMaximumAdjust,2));
	SetDlgItemText(IDC_SubFunction7Display,dtoa(vDriftCompensationFastAdjust,3) + ", " + dtoa(vDriftCompensationVeryFastAdjust,2));
	//SetDlgItemText(IDC_SubFunction8Display,dtoa(vDriftCompensationThresholdReductionMaximum,2));
	
}


void CProductDensityCompensationDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
