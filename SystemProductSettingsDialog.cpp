//ScanTrac Side View Source File
// SystemProductSettingsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "SystemProductSettingsDialog.h"
#include "ProductDensityCompensationDialog.h"
#include "NumericEntryDialog.h"
#include "AlphaEntryDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BYTE vGlobalNormalLearnDilateTimes;
extern BYTE vGlobalQuickLearnDilateTimes;
extern BYTE vGlobalScanTracType;
extern BYTE vGlobalAllowVoidInspection;
extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
/////////////////////////////////////////////////////////////////////////////
// CSystemProductSettingsDialog dialog


CSystemProductSettingsDialog::CSystemProductSettingsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemProductSettingsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemProductSettingsDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	vReferenceContainersSideToSide = 0;
	vOldCurrentProductChanged = false;
	vChangeMade = false;
	vNumberOfLastRejectImages = 50;
	vAllowVoidInspection = 0;
	vStopAfterAnInspectionRejects = 0;
	vMainWindowPointer = NULL;
	vProductDensityCompensationLoopParametersAreStandard = true;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function3Display);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Function5Display);
	vLocalCWndCollection.Add(&m_SubFunction0Button);
	vLocalCWndCollection.Add(&m_SubFunction0Display);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction1Display);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction2Display);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction3Display);
	vLocalCWndCollection.Add(&m_SubFunction4Display);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction5Display);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction6Display);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction7Display);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction8Display);
	vLocalCWndCollection.Add(&m_SubFunction9Button);
	vLocalCWndCollection.Add(&m_SubFunction9Display);
}

void CSystemProductSettingsDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);
		
	vReferenceContainersSideToSide = vLocalConfigurationData->vReferenceContainersSideToSide;
	vAllowVoidInspection = vLocalConfigurationData->vAllowVoidInspection;
	vStopAfterAnInspectionRejects = vLocalConfigurationData->vStopAfterAnInspectionRejects;
	vRemoveBackgroundFromImage = vLocalConfigurationData->vRemoveBackgroundFromImage;
	vNumberOfLastRejectImages = vLocalSystemData->vITIPCDig->vNumberOfLastRejectImages;
	vAdjustAllAreaOfHistogram = vLocalConfigurationData->vAdjustAllAreaOfHistogram;
	vAdjustImageIntensity = vLocalConfigurationData->vAdjustImageIntensity;
	vMaxImageIntensityAdjust = vLocalConfigurationData->vMaxImageIntensityAdjust;
	vMaxImageIntensityAdjustBeforeLocked = vLocalConfigurationData->vMaxImageIntensityAdjustBeforeLocked;
	vReferenceROIsToEdges = vLocalConfigurationData->vReferenceROIsToEdges;
	vOldCurrentProductChanged = vLocalSystemData->vCurrentProductChanged;
	vAutoImproveEnabled = vLocalConfigurationData->vAutoImproveEnabled;
	//vAutoImproveReduceThresholdFactor = vLocalConfigurationData->vAutoImproveReduceThresholdFactor;
	vAutoImproveNumberToAddToLearn = vLocalConfigurationData->vAutoImproveNumberToAddToLearn;
	vShowSensitivityDisplays = vLocalConfigurationData->vShowSensitivityDisplays;
	vQuickLearnDilateTimes = vLocalConfigurationData->vQuickLearnDilateTimes;
	vNormalLearnDilateTimes = vLocalConfigurationData->vNormalLearnDilateTimes;
	if (vQuickLearnDilateTimes == 20)
		vQuickLearnDilateTimes = 0;
	if (vNormalLearnDilateTimes == 20)
		vNormalLearnDilateTimes = 0;
	if (vQuickLearnDilateTimes > 20)
		vQuickLearnDilateTimes = 5;
	if (vNormalLearnDilateTimes > 20)
		vNormalLearnDilateTimes = 3;

	CheckProductDensityCompensationLoopParameters();

	UpdateDisplay();

	this->SetWindowText(_T("System/Product"));
	::SetupMenu(vLocalCWndCollection);
}

void CSystemProductSettingsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemProductSettingsDialog)
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function3Display, m_Function3Display);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function5Display, m_Function5Display);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction1Display, m_SubFunction1Display);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction2Display, m_SubFunction2Display);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction3Display, m_SubFunction3Display);
	DDX_Control(pDX, IDC_SubFunction4Display, m_SubFunction4Display);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction5Display, m_SubFunction5Display);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction6Display, m_SubFunction6Display);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction7Display, m_SubFunction7Display);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction8Display, m_SubFunction8Display);
	DDX_Control(pDX, IDC_SubFunction0Display, m_SubFunction0Display);
	DDX_Control(pDX, IDC_SubFunction9Button, m_SubFunction9Button);
	DDX_Control(pDX, IDC_SubFunction9Display, m_SubFunction9Display);
	DDX_Control(pDX, IDC_SubFunction0Button, m_SubFunction0Button);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSystemProductSettingsDialog, CDialog)
	//{{AFX_MSG_MAP(CSystemProductSettingsDialog)
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
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CSystemProductSettingsDialog::OnDialogtitlestatictext1)
	ON_STN_CLICKED(IDC_SubFunction9Button, &CSystemProductSettingsDialog::OnStnClickedSubfunction9button)
	ON_STN_CLICKED(IDC_SubFunction0Button, &CSystemProductSettingsDialog::OnStnClickedSubfunction0button)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemProductSettingsDialog message handlers

void CSystemProductSettingsDialog::OnFunction1Button() 
{
	// Exit to Main Menu
	if ((!vChangeMade) || (AbortSetupChanges()))
	{
		vLocalSystemData->vCurrentProductChanged = vOldCurrentProductChanged;
		CDialog::EndDialog(10);
	}
}

void CSystemProductSettingsDialog::OnFunction2Button() 
{
	//Exit button pressed
	if ((!vChangeMade) || (KeepSetupChanges()))
	{
		if (vAutoImproveEnabled)
		{
			//if (!vAutoImproveReduceThresholdFactor)
			//	vAutoImproveReduceThresholdFactor = 5;
			if (vAutoImproveNumberToAddToLearn < 500)
				vAutoImproveNumberToAddToLearn = 4000;
		}

		//vLocalConfigurationData->vAutoImproveReduceThresholdFactor = vAutoImproveReduceThresholdFactor;
		vLocalConfigurationData->vAutoImproveNumberToAddToLearn = vAutoImproveNumberToAddToLearn;

		if (vQuickLearnDilateTimes == 0)
			vQuickLearnDilateTimes = 20;

		if (vNormalLearnDilateTimes == 0)
			vNormalLearnDilateTimes = 20;

		if ((vLocalConfigurationData->vQuickLearnDilateTimes != vQuickLearnDilateTimes) || (vLocalConfigurationData->vNormalLearnDilateTimes != vNormalLearnDilateTimes))
			vLocalSystemData->vCurrentProductChanged = true;

		vLocalConfigurationData->vQuickLearnDilateTimes = vQuickLearnDilateTimes;
		vLocalConfigurationData->vNormalLearnDilateTimes = vNormalLearnDilateTimes;
		vGlobalNormalLearnDilateTimes = vNormalLearnDilateTimes;
		if (vGlobalNormalLearnDilateTimes > 19)
			vGlobalNormalLearnDilateTimes = 0;
		vGlobalQuickLearnDilateTimes = vQuickLearnDilateTimes;
		if (vGlobalQuickLearnDilateTimes > 19)
			vGlobalQuickLearnDilateTimes = 0;

		if (vLocalConfigurationData->vAutoImproveEnabled != vAutoImproveEnabled) 
		{
			vLocalConfigurationData->vAutoImproveEnabled = vAutoImproveEnabled;
			if (vMainWindowPointer)  
				vMainWindowPointer->SetLearnState();
		}
		vLocalConfigurationData->vReferenceContainersSideToSide = vReferenceContainersSideToSide;
		vLocalConfigurationData->vRemoveBackgroundFromImage = vRemoveBackgroundFromImage;
		vLocalSystemData->vITIPCDig->vNumberOfLastRejectImages = vNumberOfLastRejectImages;
		vLocalConfigurationData->vNumberOfLastRejectImages = vNumberOfLastRejectImages;
		vLocalConfigurationData->vAdjustAllAreaOfHistogram = vAdjustAllAreaOfHistogram;
		vLocalConfigurationData->vAdjustImageIntensity = vAdjustImageIntensity;
		vLocalConfigurationData->vMaxImageIntensityAdjust = vMaxImageIntensityAdjust;
		vLocalConfigurationData->vMaxImageIntensityAdjustBeforeLocked = vMaxImageIntensityAdjustBeforeLocked;
		vLocalConfigurationData->vReferenceROIsToEdges = vReferenceROIsToEdges;
		vLocalConfigurationData->vAllowVoidInspection = vAllowVoidInspection;
		vLocalConfigurationData->vStopAfterAnInspectionRejects = vStopAfterAnInspectionRejects;
		vGlobalAllowVoidInspection = vLocalConfigurationData->vAllowVoidInspection;
		vLocalConfigurationData->vShowSensitivityDisplays = vShowSensitivityDisplays;

		CDialog::EndDialog(true);
	}
}

void CSystemProductSettingsDialog::OnFunction3Button() 
{
	CNumericEntryDialog INumericEntryDialog;  
	//Set dialog box data titles and number value

	INumericEntryDialog.vEditString = dtoa(vQuickLearnDilateTimes);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Quick Learn Dilate Times";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Number of pixels it will Dilate a Quick Learn";
	INumericEntryDialog.m_DialogTitleStaticText3 = "";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

	INumericEntryDialog.m_UnitsString = "Default Value: 5, 0 = Disabled";
	INumericEntryDialog.vMaxValue = 19;
	INumericEntryDialog.vIntegerOnly = true;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vQuickLearnDilateTimes = _wtoi(INumericEntryDialog.vEditString);
		SetChangeMade();
		UpdateDisplay();

		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vNormalLearnDilateTimes);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Normal Learn Dilate Times";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Number of pixels it will Dilate a Normal Learn";
		INumericEntryDialog.m_DialogTitleStaticText3 = "";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

		INumericEntryDialog.m_UnitsString = "Default Value: 3, 0 = Disabled";
		INumericEntryDialog.vMaxValue = 19;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vNormalLearnDilateTimes = _wtoi(INumericEntryDialog.vEditString);

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
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CSystemProductSettingsDialog::OnFunction4Button() 
{
	// Cancel
	if (vChangeMade)
	if (AbortSetupChanges())
	{
		vLocalSystemData->vCurrentProductChanged = vOldCurrentProductChanged;
		CDialog::OnCancel();
	}
}

void CSystemProductSettingsDialog::OnFunction5Button() 
{
	// Set Number of Last Eject Buffers
	CNumericEntryDialog INumericEntryDialog;  
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vNumberOfLastRejectImages,0);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Number of Last Ejected Buffers";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Make sure there is enough memory for";
	INumericEntryDialog.m_DialogTitleStaticText3 = "the number you set";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

	INumericEntryDialog.m_UnitsString = "Default Value: 50";
	INumericEntryDialog.vMaxValue = cMaxNumberOfLastRejectImages;
	INumericEntryDialog.vIntegerOnly = true;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vNumberOfLastRejectImages = _wtoi(INumericEntryDialog.vEditString);
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

void CSystemProductSettingsDialog::OnSubFunction1Button() 
{
	// Edit Product Density Compensation values
	//Create diagnostic dialog box
	CProductDensityCompensationDialog IProductDensityCompensationDialog;  
	//Set dialog box data like how many products there are
	IProductDensityCompensationDialog.vLocalSystemData = vLocalSystemData;
	IProductDensityCompensationDialog.vLocalConfigurationData = vLocalConfigurationData;
	//IProductDensityCompensationDialog.vMainWindowPointer = vMainWindowPointer;

	//Pass control to dialog box and display
	int nResponse = IProductDensityCompensationDialog.DoModal();
	if (nResponse == IDOK)
	{
		CheckProductDensityCompensationLoopParameters();
		UpdateDisplay();
	}
	if (nResponse == 10)
	{
		//Main Menu Button Pressed
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CSystemProductSettingsDialog::OnSubFunction2Button() 
{
	// toggle Adjust Frame Brightness
	vAdjustImageIntensity = !vAdjustImageIntensity;
	if (vAdjustImageIntensity)
	{
		CNumericEntryDialog INumericEntryDialog;  
		
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vMaxImageIntensityAdjustBeforeLocked,0);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Before Density Loop is locked";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Maximum amount that can adjust Frame Brightness.";
		INumericEntryDialog.m_DialogTitleStaticText3 = "(System Setting, applies to all products)";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
			INumericEntryDialog.vEditString;

		INumericEntryDialog.m_UnitsString = "Measured in pixel value (0-255), Default: 50";
		INumericEntryDialog.vMaxValue = 255;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vMaxImageIntensityAdjustBeforeLocked = _wtoi(INumericEntryDialog.vEditString);
			SetChangeMade();
			
			INumericEntryDialog.vEditString = dtoa(vMaxImageIntensityAdjust,0);

			INumericEntryDialog.m_DialogTitleStaticText1 = "After Density Loop is locked";
			INumericEntryDialog.m_DialogTitleStaticText2 = "Maximum amount that can adjust Frame Brightness by";
			INumericEntryDialog.m_DialogTitleStaticText3 = "(System Setting, applies to all products)";
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
				INumericEntryDialog.vEditString;

			INumericEntryDialog.m_UnitsString = "Measured in pixel value (0-255), Default: 20";
			INumericEntryDialog.vMaxValue = 255;
			INumericEntryDialog.vIntegerOnly = true;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vMaxImageIntensityAdjust = _wtoi(INumericEntryDialog.vEditString);
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
	SetChangeMade();	
	UpdateDisplay();
}

void CSystemProductSettingsDialog::OnSubFunction3Button() 
{
	// toggle Adjust Frame Brightness of whole area of container
	vAdjustAllAreaOfHistogram = !vAdjustAllAreaOfHistogram;
	SetChangeMade();	
	UpdateDisplay();
}

void CSystemProductSettingsDialog::OnSubFunction4Button() 
{
	vAllowVoidInspection = !vAllowVoidInspection;
	SetChangeMade();
	UpdateDisplay();
}

void CSystemProductSettingsDialog::OnSubFunction5Button() 
{
	if (vShowSensitivityDisplays == cShowSensitivityDisplays)
	{
		vShowSensitivityDisplays = cHideSensitivityDisplays;
	}
	else
	{
		vShowSensitivityDisplays = cShowSensitivityDisplays;
	}

	SetChangeMade();
	UpdateDisplay();
}

void CSystemProductSettingsDialog::OnSubFunction6Button() 
{
	// Remove Background parameter
	vRemoveBackgroundFromImage = !vRemoveBackgroundFromImage;
	UpdateDisplay();
	SetChangeMade();
}

void CSystemProductSettingsDialog::OnSubFunction7Button() 
{
	vReferenceROIsToEdges = !vReferenceROIsToEdges;
	UpdateDisplay();
	SetChangeMade();
}

void CSystemProductSettingsDialog::OnSubFunction8Button() 
{
	vAutoImproveEnabled = !vAutoImproveEnabled;
	if (vAutoImproveEnabled)
	{
		CNumericEntryDialog INumericEntryDialog;  
		
		////Set dialog box data titles and number value
		//INumericEntryDialog.vEditString = dtoa(vAutoImproveReduceThresholdFactor,0);

		//INumericEntryDialog.m_DialogTitleStaticText1 = "Auto Improve Threshold Reduction Factor";
		//INumericEntryDialog.m_DialogTitleStaticText2 = "Amount to Multiply by State Number and Reduce Threshold";
		//INumericEntryDialog.m_DialogTitleStaticText3 = "Only Inspections using Structure have Threshold reduced";
		//INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
		//	INumericEntryDialog.vEditString;

		//INumericEntryDialog.m_UnitsString = "Default Value: 5, 0 = Disabled";
		//INumericEntryDialog.vMaxValue = 20;
		//INumericEntryDialog.vIntegerOnly = true;
		////Pass control to dialog box and display
		//int nResponse = INumericEntryDialog.DoModal();
		////dialog box is now closed, if user pressed select do this
		////if user pressed cancel, do nothing
		//if (nResponse == IDOK)
		//{
		//	vAutoImproveReduceThresholdFactor = _wtoi(INumericEntryDialog.vEditString);
		//	SetChangeMade();

			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(vAutoImproveNumberToAddToLearn);

			INumericEntryDialog.m_DialogTitleStaticText1 = "Auto Improve Number of Add To Learns";
			INumericEntryDialog.m_DialogTitleStaticText2 = "Number of Improve Learn Images that will be Added To Learn";
			INumericEntryDialog.m_DialogTitleStaticText3 = "";
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
			INumericEntryDialog.m_UnitsString = "Default Value: 4000, 0 = Disabled";
			INumericEntryDialog.vMaxValue = 64000;
			INumericEntryDialog.vIntegerOnly = true;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vAutoImproveNumberToAddToLearn = _wtoi(INumericEntryDialog.vEditString);
				SetChangeMade();
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		//}
		//else 
		//if (nResponse == 10)
		//{
		//	//Main Menu button pressed
		//	Sleep(1); //is the equivelent of a yeild statement;
		//	CDialog::EndDialog(10);
		//}
	}
	SetChangeMade();
	UpdateDisplay();
}

void CSystemProductSettingsDialog::OnStnClickedSubfunction9button()
{
	if (vStopAfterAnInspectionRejects)
		vStopAfterAnInspectionRejects = 0;
	else
		vStopAfterAnInspectionRejects = 1;

	SetChangeMade();
	UpdateDisplay();
}

BOOL CSystemProductSettingsDialog::PreTranslateMessage(MSG* pMsg) 
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
		if (vGlobalShiftKeyDown)
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
		else
		switch (pMsg->wParam)
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
					cShowHelp, 37, 0);
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

HBRUSH CSystemProductSettingsDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

	if (pWnd->GetDlgCtrlID() == IDC_Function5Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Function5Display, 5);
	if ((vNumberOfLastRejectImages != 50) && (vNumberOfLastRejectImages != 200))
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vYellowBrush;
	}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Display, 5);
		if (vShowSensitivityDisplays == cHideSensitivityDisplays)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vYellowBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5);
		pDC->SetBkMode(TRANSPARENT);
		if ((vGlobalScanTracType != cAtempoScanTrac) && (vGlobalScanTracType != cForteScanTrac) && (vGlobalScanTracType != cAllegroScanTrac) &&
			(vGlobalScanTracType != cCaseInspectorScanTrac)) //Not TDs
		{
			if (!vProductDensityCompensationLoopParametersAreStandard)
				return vLocalSystemData->vYellowBrush;
			else
			if (vLocalConfigurationData->vDriftCompensationEnabled)
				return vLocalSystemData->vGreenBrush;
			else
				return vLocalSystemData->vRedBrush;
		}
		else
		{
			if (vLocalConfigurationData->vDriftCompensationEnabled)
				return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction0Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction0Display, 5);
		pDC->SetBkMode(TRANSPARENT);
		if ((vGlobalScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloPlusScanTrac) || (vGlobalScanTracType == cPiccoloScanTrac) || 
			(vGlobalScanTracType == cUpSideDownSoloScanTrac))
		{
			if (vReferenceContainersSideToSide != cReferenceSideToSide)
				return vLocalSystemData->vYellowBrush;
		}
		else
		{
			if (vReferenceContainersSideToSide != cDontReferenceSideToSide)
				return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Display, 5);
		if ((vGlobalScanTracType != cAtempoScanTrac) && (vGlobalScanTracType != cForteScanTrac) && (vGlobalScanTracType != cAllegroScanTrac) &&
			(vGlobalScanTracType != cCaseInspectorScanTrac)) //Not TDs
		{
			if ((vMaxImageIntensityAdjust > 50) || (vMaxImageIntensityAdjust < 10) ||
				(vMaxImageIntensityAdjustBeforeLocked > 100) || (vMaxImageIntensityAdjustBeforeLocked < 25) || (!vAdjustImageIntensity))
			{
				pDC->SetBkMode(TRANSPARENT);
				return vLocalSystemData->vYellowBrush;
			}
		}
		else
		{
			if (vAdjustImageIntensity)
			{
				pDC->SetBkMode(TRANSPARENT);
				return vLocalSystemData->vYellowBrush;
			}
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Display, 5);
		if (vAdjustAllAreaOfHistogram)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Display, 5);
		if (vAllowVoidInspection)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Display, 5);
		if (vRemoveBackgroundFromImage)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Display, 5);
		if (vReferenceROIsToEdges)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Display, 5);
		if ((vGlobalScanTracType != cAtempoScanTrac) && (vGlobalScanTracType != cForteScanTrac) && (vGlobalScanTracType != cAllegroScanTrac) &&
			(vGlobalScanTracType != cCaseInspectorScanTrac)) //Not TDs
		{
			if (//(vAutoImproveReduceThresholdFactor > 10) || (vAutoImproveReduceThresholdFactor < 5) ||
				(vAutoImproveNumberToAddToLearn > 10000) || (vAutoImproveNumberToAddToLearn < 2000))
			{
				pDC->SetBkMode(TRANSPARENT);
				return vLocalSystemData->vYellowBrush;
			}
		}
		else
		{
			if (vAutoImproveEnabled)
			{
				pDC->SetBkMode(TRANSPARENT);
				return vLocalSystemData->vYellowBrush;
			}
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function3Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Function3Display, 5);
		if ((vGlobalScanTracType != cAtempoScanTrac) && (vGlobalScanTracType != cForteScanTrac) && (vGlobalScanTracType != cAllegroScanTrac) &&
			(vGlobalScanTracType != cCaseInspectorScanTrac)) //Not TDs
		{
			if ((vQuickLearnDilateTimes < 3) || (vQuickLearnDilateTimes > 7) || (vNormalLearnDilateTimes < 3) || (vNormalLearnDilateTimes > 7))
			{
				pDC->SetBkMode(TRANSPARENT);
				return vLocalSystemData->vYellowBrush;
			}
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
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction0Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction0Button, 5);  //5 is medium large
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
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction9Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction9Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction9Display)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction9Display, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() == IDC_Background) //||
		//(pWnd->GetDlgCtrlID() == IDC_Info) ||
		//(pWnd->GetDlgCtrlID() == IDC_Info2))
	{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Info, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	
	return hbr;
}

void CSystemProductSettingsDialog::UpdateDisplay()
{
	if (vReferenceContainersSideToSide == cDontReferenceSideToSide)
	{
		SetDlgItemText(IDC_SubFunction0Display, _T("Not Referencing"));
		SetDlgItemText(IDC_SubFunction0Button, _T("Reference to Sides"));
	}
	else
	if (vReferenceContainersSideToSide == cReferenceSideToSide)
	{
		SetDlgItemText(IDC_SubFunction0Display, _T("Referencing to Sides"));
		SetDlgItemText(IDC_SubFunction0Button, _T("Don't Reference to Sides"));
	}
	else
	if (vReferenceContainersSideToSide == cReferenceSideToSide)
	{
		SetDlgItemText(IDC_SubFunction0Display, _T("Not Defined"));
		SetDlgItemText(IDC_SubFunction0Button, _T("Don't Reference to Sides"));
	}

	if (vShowSensitivityDisplays == cShowSensitivityDisplays)
	{
		SetDlgItemText(IDC_SubFunction5Display, _T("Showing Sensitivity Displays"));
		SetDlgItemText(IDC_SubFunction5Button, _T("Hide Sensitivity Displays"));
	}
	else
	if (vShowSensitivityDisplays == cHideSensitivityDisplays)
	{
		SetDlgItemText(IDC_SubFunction5Display, _T("Hiding Sensitivity Displays"));
		SetDlgItemText(IDC_SubFunction5Button, _T("Show Sensitivity Displays"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction5Display, _T("Undefined Value"));
		SetDlgItemText(IDC_SubFunction5Button, _T("Show Sensitivity Displays"));
	}
	
	if (vAutoImproveEnabled)
	{
		SetDlgItemText(IDC_SubFunction8Button, _T("Disable\nAuto\nImprove"));
		//SetDlgItemText(IDC_SubFunction8Display,"Auto Improve Enabled " + //TRF: " + dtoa(vAutoImproveReduceThresholdFactor,0) + 
		SetDlgItemText(IDC_SubFunction8Display,"Auto Improve Enabled ATL#: " + dtoa(vAutoImproveNumberToAddToLearn, 0));
	}
	else	
	{
		SetDlgItemText(IDC_SubFunction8Button, _T("Enable\nAuto\nImprove"));
		SetDlgItemText(IDC_SubFunction8Display, _T("Auto Improve Disabled"));
	}
		
	SetDlgItemText(IDC_Function3Display, "QL Dilate: " + dtoa(vQuickLearnDilateTimes, 0) + ", NL Dilate: " + dtoa(vNormalLearnDilateTimes, 0));

	if (vAllowVoidInspection)
	{
		SetDlgItemText(IDC_SubFunction4Button, _T("Disable\nVoid\nInspection"));
		SetDlgItemText(IDC_SubFunction4Display, _T("Void Inspection Enabled"));
	}
	else	
	{
		SetDlgItemText(IDC_SubFunction4Button, _T("Enable\nVoid\nInspection"));
		SetDlgItemText(IDC_SubFunction4Display, _T("Void Inspection Disabled"));
	}

	if (vRemoveBackgroundFromImage)
	{
		SetDlgItemText(IDC_SubFunction6Button, _T("Don't\nRemove\nBackgrnd"));
		SetDlgItemText(IDC_SubFunction6Display, _T("Removing Background From Image"));
	}
	else	
	{
		SetDlgItemText(IDC_SubFunction6Button, _T("Remove\nBack-\nground"));
		SetDlgItemText(IDC_SubFunction6Display, _T("Not Removing Backgrnd From Image"));
	}

	if (vReferenceROIsToEdges)
	{
		SetDlgItemText(IDC_SubFunction7Button, _T("Fixed Inspection\nROI Size"));
		SetDlgItemText(IDC_SubFunction7Display, _T("Adjusting Inspection ROI Size to edges"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction7Button, _T("Adjust\nROI Size\nto Edges"));
		SetDlgItemText(IDC_SubFunction7Display, _T("Keeping Inspection ROI Size fixed"));
	}

	if (vLocalConfigurationData->vDriftCompensationEnabled)
	{
		SetDlgItemText(IDC_SubFunction1Display, _T("Enabled"));
	}
	else	
	{
		SetDlgItemText(IDC_SubFunction1Display, _T("Disabled"));
	}

	SetDlgItemText(IDC_Function5Display,dtoa(vNumberOfLastRejectImages,2));
	if (vAdjustAllAreaOfHistogram)
	{
		SetDlgItemText(IDC_SubFunction3Button, _T("Adjust\nOnly\nGrey Area"));
		SetDlgItemText(IDC_SubFunction3Display, _T("Adjusting Entire Area of Container"));
	}
	else	
	{
		SetDlgItemText(IDC_SubFunction3Button, _T("Adjust All\nArea Of\nContainer"));
		SetDlgItemText(IDC_SubFunction3Display, _T("Adjusting Only Grey Area of Container (Default)"));
	}

	if (vAdjustImageIntensity)
	{
		SetDlgItemText(IDC_SubFunction2Button, _T("Don't\nAdjust\nBrightness"));
		SetDlgItemText(IDC_SubFunction2Display,"Adjusting Frame Brightness, Max: " + dtoa(vMaxImageIntensityAdjustBeforeLocked,0) + ", " + dtoa(vMaxImageIntensityAdjust,0));
	}
	else	
	{
		SetDlgItemText(IDC_SubFunction2Button, _T("Adjust\nFrame\nBrightness"));
		SetDlgItemText(IDC_SubFunction2Display, _T("Not Adjusting Frame Brightness"));
	}

	if (vStopAfterAnInspectionRejects)
	{
		SetDlgItemText(IDC_SubFunction9Button, _T("Show All Reject Inspections"));
		SetDlgItemText(IDC_SubFunction9Display, _T("Showing just 1 reject inspection (original way)"));
	}
	else	
	{
		SetDlgItemText(IDC_SubFunction9Button, _T("Show 1 Reject Inspection"));
		SetDlgItemText(IDC_SubFunction9Display, _T("Showing all reject inspections (new way)"));
	}
}

void CSystemProductSettingsDialog::SetChangeMade() 
{
	if (!vChangeMade)
	{
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_Function2Button, _T("Save\nand Exit"));
		SetDlgItemText(IDC_Function4Button, _T("Cancel"));
		m_Function4Button.ShowWindow(SW_SHOW);
		vChangeMade = true;
	}
}

void CSystemProductSettingsDialog::CheckProductDensityCompensationLoopParameters()
{
	vProductDensityCompensationLoopParametersAreStandard = true;

	if (vLocalConfigurationData->vDriftCompensationEnabled)
	{
		if (!vLocalConfigurationData->vUseDACAndADCinDensityCompensation)
			vProductDensityCompensationLoopParametersAreStandard = false;

		if (vLocalConfigurationData->vDriftCompensationWaitTime != 100)
			vProductDensityCompensationLoopParametersAreStandard = false;

		if (vLocalConfigurationData->vNumberOfImagesForDriftDensity != 10)
			vProductDensityCompensationLoopParametersAreStandard = false;

		if ((vLocalConfigurationData->vDriftCompensationFastThreshold < 5) || (vLocalConfigurationData->vDriftCompensationFastThreshold > 25))
			vProductDensityCompensationLoopParametersAreStandard = false;

		if ((vLocalConfigurationData->vDriftCompensationVeryFastThreshold < 10) || 
			(vLocalConfigurationData->vDriftCompensationVeryFastThreshold > 50))
			vProductDensityCompensationLoopParametersAreStandard = false;

		if (vLocalConfigurationData->vNumberOfImagesForDriftDensity != 10)
			vProductDensityCompensationLoopParametersAreStandard = false;

		if ((vLocalConfigurationData->vDriftCompensationFastAdjust < 10) || (vLocalConfigurationData->vDriftCompensationFastAdjust > 40))
			vProductDensityCompensationLoopParametersAreStandard = false;

		if ((vLocalConfigurationData->vDriftCompensationVeryFastAdjust < 20) || 
			(vLocalConfigurationData->vDriftCompensationVeryFastAdjust > 70))
			vProductDensityCompensationLoopParametersAreStandard = false;

		if ((vLocalConfigurationData->vDriftCompensationSlowThreshold < 3) || (vLocalConfigurationData->vDriftCompensationSlowThreshold > 6))
			vProductDensityCompensationLoopParametersAreStandard = false;

		if (vLocalConfigurationData->vDriftCompensationSlowAdjust != 1)
			vProductDensityCompensationLoopParametersAreStandard = false;

		if (vLocalConfigurationData->vDriftCompensationMaximumAdjust != 500)
			vProductDensityCompensationLoopParametersAreStandard = false;

		if ((vLocalConfigurationData->vDriftCompensationThresholdReductionFactor < 1) || 
			(vLocalConfigurationData->vDriftCompensationFastThreshold > 10))
			vProductDensityCompensationLoopParametersAreStandard = false;

		if (vLocalConfigurationData->vDriftCompensationThresholdReductionMaximum != 0)
			vProductDensityCompensationLoopParametersAreStandard = false;
	}
}

void CSystemProductSettingsDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}



	void CSystemProductSettingsDialog::OnStnClickedSubfunction0button()
	{
		if (vReferenceContainersSideToSide == cDontReferenceSideToSide)
			vReferenceContainersSideToSide = cReferenceSideToSide;
		else
			vReferenceContainersSideToSide = cDontReferenceSideToSide;

		SetChangeMade();
		UpdateDisplay();

	}
