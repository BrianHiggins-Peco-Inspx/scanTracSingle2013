//ScanTrac Side View Source File
// InspxHiddenSystemDialog.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "InspxHiddenSystemDialog.h"
#include "NumericEntryDialog.h"
#include "SelectItemDialog.h"
#include "NoticeDialog.h"
#include "AlphaEntryDialog.h"
#include "ConfigureEMailAddresses.h"
#include "InspxHiddenDialog.h"
#include "SelectMultipleItemDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
/////////////////////////////////////////////////////////////////////////////
// CInspxHiddenSystemDialog dialog


CInspxHiddenSystemDialog::CInspxHiddenSystemDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CInspxHiddenSystemDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInspxHiddenSystemDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	vLearnQualityPoorLimit = 45;
	vLearnQualityMarginalLimit = 35;
	vAlarmEjectorReEnableX = 0;
	vAlarmEjectorReEnableY = 0;
	vScanTracID = "ScanTrac 01";
	vChangeMade = false;
	vEjectContainersNotInspected = false;
	vDiverterEjector = 0;
	vDiverterMargin = 0;
	vStopInspectionIfConveyorStops = false;
	//vEjectorAirPressureSensorConnected = false;
	vRedIndicatorToShowOnMainMenu = cDefectInRedBox;
	vAutoDeleteFileTypes = 0x13;
	vNumberOfDaysToKeepLogs = 0;
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
	vLocalCWndCollection.Add(&m_Function6Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction1Display);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction2Display);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction3Display);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction4Display);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction5Display);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction6Display);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction7Display);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction8Display);
}

void CInspxHiddenSystemDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);
			//_ivTempChars4,10);
	vLearnQualityPoorLimit = vLocalConfigurationData->vLearnQualityPoorLimit;
	vLearnQualityMarginalLimit = vLocalConfigurationData->vLearnQualityMarginalLimit;
	vAlarmEjectorReEnableX = vLocalConfigurationData->vAlarmEjectorReEnableX;
	vAlarmEjectorReEnableY = vLocalConfigurationData->vAlarmEjectorReEnableY;
	vScanTracID = vLocalConfigurationData->vScanTracID;
	vEjectContainersNotInspected = vLocalConfigurationData->vEjectContainersNotInspected;
	vDiverterEjector = vLocalConfigurationData->vDiverterEjector;
	vStopInspectionIfConveyorStops = vLocalConfigurationData->vStopInspectionIfConveyorStops;
	//vEjectorAirPressureSensorConnected = vLocalConfigurationData->vEjectorAirPressureSensorConnected;
	vRedIndicatorToShowOnMainMenu = vLocalConfigurationData->vRedIndicatorToShowOnMainMenu;
	vDiverterMargin = vLocalConfigurationData->vDiverterMargin;
	vStopRunningOnUnConfirmedEject = vLocalConfigurationData->vStopRunningOnUnConfirmedEject;
	vAutoDeleteFileTypes = vLocalConfigurationData->vAutoDeleteFileTypes;
	vNumberOfDaysToKeepLogs = vLocalConfigurationData->vNumberOfDaysToKeepLogs;
	UpdateDisplay();
			
	this->SetWindowText(_T("System Features Setup"));
	
	::SetupMenu(vLocalCWndCollection);
}

void CInspxHiddenSystemDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInspxHiddenSystemDialog)
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function3Display, m_Function3Display);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function5Display, m_Function5Display);
	DDX_Control(pDX, IDC_Function6Button, m_Function6Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction1Display, m_SubFunction1Display);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction2Display, m_SubFunction2Display);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction3Display, m_SubFunction3Display);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction4Display, m_SubFunction4Display);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction5Display, m_SubFunction5Display);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction6Display, m_SubFunction6Display);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction7Display, m_SubFunction7Display);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction8Display, m_SubFunction8Display);
}


BEGIN_MESSAGE_MAP(CInspxHiddenSystemDialog, CDialog)
	//{{AFX_MSG_MAP(CInspxHiddenSystemDialog)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
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
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_Function6Button, &CInspxHiddenSystemDialog::OnFunction6button)
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CInspxHiddenSystemDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInspxHiddenSystemDialog message handlers

void CInspxHiddenSystemDialog::OnFunction1Button() 
{
	// Main Menu Button was pressed
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::EndDialog(10);
}

void CInspxHiddenSystemDialog::OnFunction2Button() 
{
	// Save and Exit Button was pressed
	if (ConfigurationOK())
	if ((!vChangeMade) || (KeepSetupChanges()))
	{
		vLocalConfigurationData->vLearnQualityPoorLimit = vLearnQualityPoorLimit;
		vLocalConfigurationData->vLearnQualityMarginalLimit = vLearnQualityMarginalLimit;
		vLocalConfigurationData->vAlarmEjectorReEnableX = vAlarmEjectorReEnableX;
		vLocalConfigurationData->vAlarmEjectorReEnableY = vAlarmEjectorReEnableY;
		vLocalConfigurationData->vScanTracID = vScanTracID;
		vLocalConfigurationData->vEjectContainersNotInspected = vEjectContainersNotInspected;
		vLocalConfigurationData->vDiverterEjector = vDiverterEjector;
		vLocalConfigurationData->vStopInspectionIfConveyorStops = vStopInspectionIfConveyorStops;
		//vLocalConfigurationData->vEjectorAirPressureSensorConnected = vEjectorAirPressureSensorConnected;

		//enum {cDefectsInNoColor, cDefectsInPseudoColor, cDefectsInRed, cDefectInRedBox, cDefectsInRedBoxes};
		vLocalConfigurationData->vRedIndicatorToShowOnMainMenu = vRedIndicatorToShowOnMainMenu;

		//if (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInPseudoColor)
		//	vLocalConfigurationData->vDefectDisplayMode = cDefectsInPseudoColor;  //increase rejects menu to same as Main Menu display

		//if (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInRed)
		//if (vLocalConfigurationData->vDefectDisplayMode != cDefectsInPseudoColor)  
		//	vLocalConfigurationData->vDefectDisplayMode = cDefectsInRed;  //increase rejects menu to same as Main Menu display if not higher

		//if (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInRedBoxes)
		//if (vLocalConfigurationData->vDefectDisplayMode != cDefectsInPseudoColor)  
		//if (vLocalConfigurationData->vDefectDisplayMode != cDefectsInRed)  
		//	vLocalConfigurationData->vDefectDisplayMode = cDefectsInRedBoxes;  //increase rejects menu to same as Main Menu display if not higher

		vLocalConfigurationData->vDiverterMargin = vDiverterMargin;
		vLocalConfigurationData->vStopRunningOnUnConfirmedEject = vStopRunningOnUnConfirmedEject;
		vLocalConfigurationData->vAutoDeleteFileTypes = vAutoDeleteFileTypes;
		vLocalConfigurationData->vNumberOfDaysToKeepLogs = vNumberOfDaysToKeepLogs;

		CDialog::EndDialog(true);
	}
}

void CInspxHiddenSystemDialog::OnFunction3Button() 
{
	//alarm re-enable x of y
	CNumericEntryDialog INumericEntryDialog;  

	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vAlarmEjectorReEnableX,0);
	INumericEntryDialog.m_DialogTitleStaticText1 = "Number of good containers (x) out of (y) samples";
	INumericEntryDialog.m_DialogTitleStaticText2 = "must get to re-enable ejectors";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Enter X = Number of good in samples to enable ejectors";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
		INumericEntryDialog.vEditString;

	INumericEntryDialog.m_UnitsString = "0 is disabled, Default: 100";
	INumericEntryDialog.vMaxValue = cMaximumEjectDataSize;
	INumericEntryDialog.vMinValue = 0;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vAlarmEjectorReEnableX = _wtoi(INumericEntryDialog.vEditString);
		UpdateDisplay();


		WORD TempMin = vAlarmEjectorReEnableX;
		if (TempMin)
		{
			INumericEntryDialog.vEditString = dtoa(vAlarmEjectorReEnableY);
			INumericEntryDialog.m_DialogTitleStaticText3 = "Enter Y = Number of samples to enable ejectors";
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
				INumericEntryDialog.vEditString;

			INumericEntryDialog.m_UnitsString = "Zero is disabled";
			INumericEntryDialog.vMaxValue = cMaximumEjectDataSize;
			INumericEntryDialog.vMinValue = TempMin;
			INumericEntryDialog.vIntegerOnly = true;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vAlarmEjectorReEnableY = _wtoi(INumericEntryDialog.vEditString);
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
			vAlarmEjectorReEnableY = 0;
		}
		SetChangeMade();
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		CDialog::EndDialog(10);
	}
}

void CInspxHiddenSystemDialog::OnFunction4Button() 
{
	//Cancel was pressed
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::OnCancel();
}

void CInspxHiddenSystemDialog::OnFunction5Button() 
{
	CNumericEntryDialog INumericEntryDialog;  
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vLearnQualityPoorLimit,0);
	INumericEntryDialog.m_DialogTitleStaticText1 = "Learn Quality Poor Limit for a Yellow Message";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Default value is  45";
	INumericEntryDialog.m_DialogTitleStaticText3 = "";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

	INumericEntryDialog.m_UnitsString = "0 is disabled";
	INumericEntryDialog.vMaxValue = 128;
	INumericEntryDialog.vMinValue = 0;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vLearnQualityPoorLimit = _wtoi(INumericEntryDialog.vEditString);
		UpdateDisplay();

		if (vLearnQualityPoorLimit)
		{
			INumericEntryDialog.vEditString = dtoa(vLearnQualityMarginalLimit);
			INumericEntryDialog.m_DialogTitleStaticText1 = "Learn Quality Marginal Limit for a Yellow Message";
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
			INumericEntryDialog.m_DialogTitleStaticText2 = "Default value is  35";

			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vLearnQualityMarginalLimit = _wtoi(INumericEntryDialog.vEditString);
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
			vLearnQualityMarginalLimit = 0;
		}
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

void CInspxHiddenSystemDialog::OnSubFunction1Button() 
{
	// specify the diverter position	
	CNumericEntryDialog INumericEntryDialog;  
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vDiverterEjector,0);

	INumericEntryDialog.m_DialogTitleStaticText1 = "";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Ejector output diverter is wired to";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Default: Ejector 3";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
		INumericEntryDialog.vEditString;

	INumericEntryDialog.m_UnitsString = "Can be ejector 1, 2, or 3";
	INumericEntryDialog.vMaxValue = cNumberOfEjectors;
	INumericEntryDialog.vIntegerOnly = true;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vDiverterEjector = _wtoi(INumericEntryDialog.vEditString);

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

void CInspxHiddenSystemDialog::OnSubFunction2Button() 
{
	// specify the diverter position	
	CNumericEntryDialog INumericEntryDialog;  
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vDiverterMargin,0);

	INumericEntryDialog.m_DialogTitleStaticText1 = "";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Margin to allow before opening or closing diverter";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Default: 4";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
		INumericEntryDialog.vEditString;

	INumericEntryDialog.m_UnitsString = "Units in " + vLocalConfigurationData->vUnitsString;
	INumericEntryDialog.vMaxValue = 255;
	INumericEntryDialog.vIntegerOnly = true;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vDiverterMargin = _wtoi(INumericEntryDialog.vEditString);

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

void CInspxHiddenSystemDialog::OnSubFunction3Button() 
{
	// toggle un inspected containers eject mode
	vEjectContainersNotInspected = !vEjectContainersNotInspected;
	UpdateDisplay();
	SetChangeMade();	
}

void CInspxHiddenSystemDialog::OnSubFunction4Button() 
{
	//enum {cDefectsInNoColor, cDefectsInPseudoColor, cDefectsInRed, cDefectInRedBox, cDefectsInRedBoxes};
	if (vRedIndicatorToShowOnMainMenu < cDefectsInRedBoxes)
		vRedIndicatorToShowOnMainMenu++;
	else
		vRedIndicatorToShowOnMainMenu = cDefectsInNoColor;

	// Toggle Ejector air pressure sensor connected
	//vEjectorAirPressureSensorConnected = !vEjectorAirPressureSensorConnected;
	UpdateDisplay();
	SetChangeMade();	
}

void CInspxHiddenSystemDialog::OnSubFunction5Button() 
{
	vStopInspectionIfConveyorStops = !vStopInspectionIfConveyorStops;
	UpdateDisplay();
	SetChangeMade();	
}

void CInspxHiddenSystemDialog::OnSubFunction6Button() 
{
	vStopRunningOnUnConfirmedEject = ! vStopRunningOnUnConfirmedEject;
	UpdateDisplay();
	SetChangeMade();	
}

void CInspxHiddenSystemDialog::OnSubFunction7Button() 
{
	//Auto File Delete
	CNumericEntryDialog INumericEntryDialog;  
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vNumberOfDaysToKeepLogs,0);

	INumericEntryDialog.m_DialogTitleStaticText1 = "";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Auto Delete Logs Older Than Number of Days";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Recomended Time: 100 Days";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString + " Days";

	INumericEntryDialog.m_UnitsString = "Enter 1 - 60,000 days, or 0 to Disable Auto Delete";
	INumericEntryDialog.vMaxValue = 60000;//size of word
	INumericEntryDialog.vIntegerOnly = true;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vNumberOfDaysToKeepLogs = _wtoi(INumericEntryDialog.vEditString);

		if (vNumberOfDaysToKeepLogs != 0)
		{
			CSelectMultipleItemDialog ISelectItemDialog;  
			
			//Set dialog box data titles and number value
			ISelectItemDialog.m_DialogTitleStaticText = "Select File Types To Auto Delete";
			ISelectItemDialog.vNumberOfItems = 5;
			ISelectItemDialog.vInAutoSetup = false;
			ISelectItemDialog.vUseSelectText = false;
			//ISelectItemDialog.vColor = cGreen;

			ISelectItemDialog.vItems[0] = vAutoDeleteFileTypes & 0x01; //reject
			ISelectItemDialog.vItemsYes[0] = "Selected-AC Power Monitor Log Auto Delete";
			ISelectItemDialog.vItemsNo[0] = "Not Selected-AC Power Monitor Log Auto Delete";

			ISelectItemDialog.vItems[1] = vAutoDeleteFileTypes & 0x02;
			ISelectItemDialog.vItemsYes[1] = "Selected-Event Log Auto Delete";
			ISelectItemDialog.vItemsNo[1] = "Not Selected-Event Log Auto Delete";

			ISelectItemDialog.vItems[2] = vAutoDeleteFileTypes & 0x04;
			ISelectItemDialog.vItemsYes[2] = "Selected-Image Files Auto Delete";
			ISelectItemDialog.vItemsNo[2] = "Not Selected-Image Files Auto Delete";

			ISelectItemDialog.vItems[3] = vAutoDeleteFileTypes & 0x08;
			ISelectItemDialog.vItemsYes[3] = "Selected-Screen Shots Auto Delete";
			ISelectItemDialog.vItemsNo[3] = "Not Selected-Screen Shots Auto Delete";

			ISelectItemDialog.vItems[4] = vAutoDeleteFileTypes & 0x0010;
			ISelectItemDialog.vItemsYes[4] = "Selected-Serial Data Log Auto Delete";
			ISelectItemDialog.vItemsNo[4] = "Not Selected-Serial Data Log Auto Delete";

			//if (PasswordOK(cTemporaryInspxPassword,false))
			//	ISelectItemDialog.vNumberOfItems = 4;

			//Pass control to dialog box and display
			int nResponse = ISelectItemDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vAutoDeleteFileTypes = 0;
				if (ISelectItemDialog.vItems[0])
					vAutoDeleteFileTypes = vAutoDeleteFileTypes | 0x01; //Delete AC Power Monitor Logs
				if (ISelectItemDialog.vItems[1])
					vAutoDeleteFileTypes = vAutoDeleteFileTypes | 0x02; //Delete Event Logs
				if (ISelectItemDialog.vItems[2])
					vAutoDeleteFileTypes = vAutoDeleteFileTypes | 0x04; //Delete Images
				if (ISelectItemDialog.vItems[3])
					vAutoDeleteFileTypes = vAutoDeleteFileTypes | 0x08; //Delete Screen Shots
				if (ISelectItemDialog.vItems[4])
					vAutoDeleteFileTypes = vAutoDeleteFileTypes | 0x10; //Delete Serial Logs
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
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

	SetChangeMade();
	UpdateDisplay();
}

void CInspxHiddenSystemDialog::OnSubFunction8Button() 
{
	//ScanTrac id pressed
	CAlphaEntryDialog IAlphaEntryDialog;  
	IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
	IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
	//Set dialog box data titles and number value
	IAlphaEntryDialog.vEditString = vScanTracID;
	IAlphaEntryDialog.m_DialogTitleStaticText = "Edit ScanTrac Machine Name";

	IAlphaEntryDialog.m_DialogTitleStaticText2 = "Original Value: " + vScanTracID;
	//Pass control to dialog box and display
	int nResponse = IAlphaEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vScanTracID = IAlphaEntryDialog.vEditString;
		UpdateDisplay();
		SetChangeMade();
		
	}
	else
	if (nResponse == 10)  //main menu pressed
		CDialog::EndDialog(10);
}

HBRUSH CInspxHiddenSystemDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_Function3Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Function3Display, 5);  //5 is medium large
		if ((vAlarmEjectorReEnableX < 98) || (vAlarmEjectorReEnableX > 100) || (vAlarmEjectorReEnableY != 100))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Display, 5);  //5 is medium large
		if ( (vNumberOfDaysToKeepLogs < 30) || ( (vAutoDeleteFileTypes % 0x20) == 0 ) )
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function5Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Function5Display, 5);  //5 is medium large
	if ((vLocalConfigurationData->vScanTracType != cAtempoScanTrac) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cAllegroScanTrac) && 
		(vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac)) //Not TDs
	{
		if ((vLearnQualityPoorLimit != 45) || (vLearnQualityMarginalLimit != 35))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}
	else
	{
		if ((vLearnQualityPoorLimit) || (vLearnQualityMarginalLimit))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5);
	if (vDiverterEjector != 3)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
	}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Display, 5);
	if (vDiverterMargin != 4)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
	}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Display, 5);
		if (vEjectContainersNotInspected)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}

	//enum {cDefectsInNoColor, cDefectInRedBox, cDefectsInPseudoColor, cDefectsInRed, cDefectsInRedBoxes};
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Display, 5);
		if ((vRedIndicatorToShowOnMainMenu != cDefectInRedBox) && (vRedIndicatorToShowOnMainMenu != cDefectsInRedBoxes))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}
	//if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Display)
	//if (vEjectorAirPressureSensorConnected)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	pDC->SetTextColor(cButtonTextColor);
	//	return vLocalSystemData->vYellowBrush;
	//}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Display, 5);
		if (vStopInspectionIfConveyorStops)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Display, 5);
	if (vStopRunningOnUnConfirmedEject)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
	}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Display, 5);
	if (vScanTracID.GetLength() < 5)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
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
	if (pWnd->GetDlgCtrlID() == IDC_Function6Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function6Button, 5);  //5 is medium large
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

BOOL CInspxHiddenSystemDialog::PreTranslateMessage(MSG* pMsg) 
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
				return true;
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
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
			break;
			case 36:  //Home key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
			case cPageDown: // Help
				PostThreadMessage(vGlobalCallingThreadID,
					cShowHelp, 10, 0);
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

void CInspxHiddenSystemDialog::SetChangeMade() 
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

void CInspxHiddenSystemDialog::UpdateDisplay() 
{
	 
	SetDlgItemText(IDC_Function5Display,"Poor LQ: " + dtoa(vLearnQualityPoorLimit,0) + ", M: " + dtoa(vLearnQualityMarginalLimit,0));

	CString TempFileTypeString = "Auto Delete Files ";
	if ( (vNumberOfDaysToKeepLogs == 0) || ( (vAutoDeleteFileTypes % 0x20) == 0 ) )
		TempFileTypeString = TempFileTypeString + "Disabled";
	else
	{
		TempFileTypeString = TempFileTypeString + "Older Than " + dtoa(vNumberOfDaysToKeepLogs) + " Days, ";

		if (vAutoDeleteFileTypes & 0x001)
			TempFileTypeString = TempFileTypeString + "A"; //AC Power Monitor Logs
		if (vAutoDeleteFileTypes & 0x002)
			TempFileTypeString = TempFileTypeString + "E";	//Event Logs
		if (vAutoDeleteFileTypes & 0x004)
			TempFileTypeString = TempFileTypeString + "I";	//Images
		if (vAutoDeleteFileTypes & 0x008)
			TempFileTypeString = TempFileTypeString + "C";	//Screen Shots - C for Capture
		if (vAutoDeleteFileTypes & 0x010)
			TempFileTypeString = TempFileTypeString + "S";	//Serial Logs
		if (TempFileTypeString == "")
			TempFileTypeString = "Disabled";
	}
	SetDlgItemText(IDC_SubFunction7Display,TempFileTypeString);

	SetDlgItemText(IDC_Function3Display,dtoa(vAlarmEjectorReEnableX,0) + " of " +
		dtoa(vAlarmEjectorReEnableY,0));
	SetDlgItemText(IDC_SubFunction6Display,vScanTracID);
	if (vStopRunningOnUnConfirmedEject)
	{
		SetDlgItemText(IDC_SubFunction6Button, _T("Don't Stop On Unconfirm"));
		SetDlgItemText(IDC_SubFunction6Display, _T("Stop Inspecting On Unconfirmed Eject"));
	}
	else	
	{
		SetDlgItemText(IDC_SubFunction6Button, _T("Stop On Unconfirm Eject"));
		SetDlgItemText(IDC_SubFunction6Display, _T("Don't Stop Inspecting On Unconfirmed Eject"));
	}

	if (vStopInspectionIfConveyorStops)
	{
		SetDlgItemText(IDC_SubFunction5Button, _T("Disable Conveyor Stop"));
		SetDlgItemText(IDC_SubFunction5Display, _T("Stop Inspecting When Conveyor Stops"));
	}
	else	
	{
		SetDlgItemText(IDC_SubFunction5Button, _T("Enable Conveyor Stop"));
		SetDlgItemText(IDC_SubFunction5Display, _T("Don't Stop Inspecting When Conveyor Stops"));
	}

	if (vEjectContainersNotInspected)
	{
		SetDlgItemText(IDC_SubFunction3Button, _T("Pass Un- Inspected Containers"));
		SetDlgItemText(IDC_SubFunction3Display, _T("Divert Containers Not Inspected"));
	}
	else	
	{
		SetDlgItemText(IDC_SubFunction3Button, _T("Divert Un- Inspected Containers"));
		SetDlgItemText(IDC_SubFunction3Display, _T("Pass Containers Not Inspected"));
	}
	
	//enum {cDefectsInNoColor, cDefectsInPseudoColor, cDefectsInRed, cDefectInRedBox, cDefectsInRedBoxes};
	if (vRedIndicatorToShowOnMainMenu == cDefectsInNoColor)
	{
		SetDlgItemText(IDC_SubFunction4Button, _T("Defects in Pseudo Color"));
		SetDlgItemText(IDC_SubFunction4Display, _T("No Defect Indicator on Display"));
	}
	else	
	if (vRedIndicatorToShowOnMainMenu == cDefectsInPseudoColor)
	{
		SetDlgItemText(IDC_SubFunction4Button, _T("Defects in Red on Display"));
		SetDlgItemText(IDC_SubFunction4Display, _T("Defects in Pseudo Color on Display"));
	}
	else	
	if (vRedIndicatorToShowOnMainMenu == cDefectsInRed)
	{
		SetDlgItemText(IDC_SubFunction4Button, _T("Defects in Red Box on Display"));
		SetDlgItemText(IDC_SubFunction4Display, _T("Defects in Red on Display"));
	}
	else	
	if (vRedIndicatorToShowOnMainMenu == cDefectInRedBox)
	{
		SetDlgItemText(IDC_SubFunction4Button, _T("Defects in Up to 10 Red Boxes"));
		SetDlgItemText(IDC_SubFunction4Display, _T("Defects in Red Box on Display"));
	}
	else	
	{
		SetDlgItemText(IDC_SubFunction4Button, _T("No Defect Indicator Display"));
		if (vRedIndicatorToShowOnMainMenu == cDefectsInRedBoxes)
			SetDlgItemText(IDC_SubFunction4Display, _T("Defects in Up to 10 Red Boxes on Display"));
		else
			SetDlgItemText(IDC_SubFunction4Display, _T("Unknown"));
	}
	/*
	if (vEjectorAirPressureSensorConnected)
	{
		SetDlgItemText(IDC_SubFunction4Button,"No Ejector Sensor Connected");
		SetDlgItemText(IDC_SubFunction4Display,"Ejector Air Pressure Sensor Connected");
	}
	else	
	{
		SetDlgItemText(IDC_SubFunction4Button,"Ejector Sensor Connected");
		SetDlgItemText(IDC_SubFunction4Display,"Ejector Air Pressure Sensor Not Connected");
	}
	*/
	SetDlgItemText(IDC_SubFunction1Display,dtoa(vDiverterEjector,0));
	SetDlgItemText(IDC_SubFunction2Display,dtoa(vDiverterMargin,0));
	SetDlgItemText(IDC_SubFunction8Display,vScanTracID);
}

bool CInspxHiddenSystemDialog::ConfigurationOK()
{
	bool TempConfigurationOK = true;
	if (vEjectContainersNotInspected)
	if (!vDiverterEjector)
	{
		TempConfigurationOK = false;
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nDiverting containers not inspected requires\nan ejector output for the diverter";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	return TempConfigurationOK;
}



void CInspxHiddenSystemDialog::OnFunction6button()
{
	////e-mail
	////setup E-Mail
	////Create dialog box
	//CConfigureEMailAddresses IConfigureEMailAddressesDialog;  
	//IConfigureEMailAddressesDialog.vMainWindowPointer = vMainWindowPointer;
	////Set dialog box data like how many products there are
	//IConfigureEMailAddressesDialog.vEMailCategory = cEMailPlantOperations;
	////cEMailInspx)
	//IConfigureEMailAddressesDialog.vLocalConfigurationData = vLocalConfigurationData;
	//IConfigureEMailAddressesDialog.vLocalSystemData = vLocalSystemData;

	////Pass control to dialog box and display
	//int nResponse = IConfigureEMailAddressesDialog.DoModal();
	////dialog box is now closed, if user pressed select do this
	////if user pressed cancel, do nothing
	//if (nResponse == IDOK)
	//{
	//	// TODO: Place code here to handle when the dialog is accepted
	//	IConfigureEMailAddressesDialog.vEMailCategory = cEMailMaintenance;
	//	//Pass control to dialog box and display
	//	int nResponse = IConfigureEMailAddressesDialog.DoModal();
	//	//dialog box is now closed, if user pressed select do this
	//	//if user pressed cancel, do nothing
	//	if (nResponse == IDOK)
	//	{
	//		// TODO: Place code here to handle when the dialog is accepted
	//		IConfigureEMailAddressesDialog.vEMailCategory = cEMailReport;
	//		//Pass control to dialog box and display
	//		int nResponse = IConfigureEMailAddressesDialog.DoModal();
	//		//dialog box is now closed, if user pressed select do this
	//		//if user pressed cancel, do nothing
	//		if (nResponse == IDOK)
	//		{
	//			// TODO: Place code here to handle when the dialog is accepted
	//			if (PasswordOK(cTemporaryInspxPassword,false))
	//			{
	//				IConfigureEMailAddressesDialog.vEMailCategory = cEMailInspx;
	//				//Pass control to dialog box and display
	//				int nResponse = IConfigureEMailAddressesDialog.DoModal();
	//				//dialog box is now closed, if user pressed select do this
	//				//if user pressed cancel, do nothing
	//				if (nResponse == IDOK)
	//				{
	//					// TODO: Place code here to handle when the dialog is accepted
	//				}
	//				else 
	//				if (nResponse == 10)
	//				{
	//					// TODO: Place code here to handle when the dialog is
	//					//  dismissed with Main Menu
	//					Sleep(1); //is the equivelent of a yeild statement;
	//					CDialog::EndDialog(10);
	//				}
	//			}
	//		}
	//		else 
	//		if (nResponse == 10)
	//		{
	//			// TODO: Place code here to handle when the dialog is
	//			//  dismissed with Main Menu
	//			Sleep(1); //is the equivelent of a yeild statement;
	//			CDialog::EndDialog(10);
	//		}
	//	}
	//	else 
	//	if (nResponse == 10)
	//	{
	//		// TODO: Place code here to handle when the dialog is
	//		//  dismissed with Main Menu
	//		Sleep(1); //is the equivelent of a yeild statement;
	//		CDialog::EndDialog(10);
	//	}
	//}
	//else 
	//if (nResponse == 10)
	//{
	//	// TODO: Place code here to handle when the dialog is
	//	//  dismissed with Main Menu
	//	Sleep(1); //is the equivelent of a yeild statement;
	//	CDialog::EndDialog(10);
	//}
}


void CInspxHiddenSystemDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
