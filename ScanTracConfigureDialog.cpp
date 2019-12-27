//ScanTrac Side View Source File
// ScanTracConfigureDialog.cpp : implementation file
//
#include <stdio.h>

#include "stdafx.h"
#include "ScanTrac.h"
#include "ScanTracConfigureDialog.h"
#include "NewConfigureSystemDialog.h"
#include "YesNoDialog.h"
#include "PasswordEntryDialog.h"
#include "NoticeDialog.h"
#include "NumericEntryDialog.h"
#include "InspxHiddenDialog.h"
#include "SelectItemDialog.h"
#include "AlphaEntryDialog.h"
#include "ConfigureDirectoryNames.h"
#include <atlbase.h> //for registry
#include "ConfigurePasswordsDialog.h"
#include "Process.h" //for _execl or _spawn
#include "Ping.h" //
#include "PingTestThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
IMPLEMENT_DYNCREATE(CScanTracConfigureDialog, CDialog)

extern BYTE vGlobalLanguage;
extern HMODULE vGlobalhLibFBWFNetwork; 
extern DWORD vGlobalFBWFEnable;
extern BYTE vGlobalUseCommaForDecimalPoint;
extern CString vGlobalNetworkBoardCode;
extern CString vGlobalRegistryKeyPath;
extern CString vGlobalIPAddressString;
extern CString vGlobalIPSubNetMaskString;
extern CString vGlobalIPDefaultGatewayString;
extern CString vGlobalIPDNSAddressString1;
extern CString vGlobalIPDHCPServer;
extern BYTE vGlobalIPDHCPEnabled;
extern BYTE vGlobalShiftKeyDown;
extern int vGlobalPasswordLevel;
extern unsigned long vGlobalDialogTitleColor;
extern CPingTestThread *vGlobalPingTestThread;
extern BYTE vGlobalPingStatus;
extern BYTE vGlobalPingReply;
extern BYTE vGlobalPCRunningWindowsXP;
/////////////////////////////////////////////////////////////////////////////
// CScanTracConfigureDialog dialog


CScanTracConfigureDialog::CScanTracConfigureDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CScanTracConfigureDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScanTracConfigureDialog)
	//}}AFX_DATA_INIT
	vAutoStop = 0;
	vChangedLanguage = false;
	vLanguage = 0;
	vReadCount = 0;
	PingTimeOut = 0;
	vPingDNS = false;
	vMainWindowPointer = NULL;
	//vCPMTimePeriodString = "10";
	vOneSecondTimerHandle = 31368;
	vPingTimerHandle = 31369;
	vUpdateIPAddressTimerHandle = 7912;
	vLastDateString = "1";
	vAskForProductionReportProductCode = false;
	vAutoProductionReportHour = 0;
	vAutoProductionReportHour2 = 0;
	vAutoProductionReportHour3 = 0;
	vScanTracCustomerName = "";
	vIPAddressString = "0.0.0.0";
	vIPSubNetMaskString = "0.0.0.0";
	vIPDefaultGatewayString = "0.0.0.0";
	vIPDefaultDNSAddressString = "0.0.0.0";
	//vNetworkBoardCode = "";
	//vRegistryKeyPath = "";
	vIPDHCPEnabled = true;
	vProductPasswordOK = false;
	vGlobalPingReply = cPingResultNotSent;
	vTeamViewerInstalled = false ;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Network2RedFlash);
	vLocalCWndCollection.Add(&m_Network2GreenFlash);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_Network1RedFlash);
	vLocalCWndCollection.Add(&m_Network1GreenFlash);
	vLocalCWndCollection.Add(&m_SubFunction3Display);
	vLocalCWndCollection.Add(&m_SubFunction2Display);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_SubFunction1Display);
	vLocalCWndCollection.Add(&m_SubFunction4Display);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction5Display);
	vLocalCWndCollection.Add(&m_SubFunction7Display);
	vLocalCWndCollection.Add(&m_Clock);
//	vLocalCWndCollection.Add(&m_CPMTimePeriodDisplay);
	vLocalCWndCollection.Add(&m_Date);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_DNSLabel);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function5Display);
	vLocalCWndCollection.Add(&m_GatewayLabel);
	vLocalCWndCollection.Add(&m_TeamViewLabel);
	vLocalCWndCollection.Add(&m_WorkStopTimePeriodDisplay);
	vLocalCWndCollection.Add(&m_BlackBox1);
	vLocalCWndCollection.Add(&m_BlackBox2);
}

void CScanTracConfigureDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);
	vChangeMade = false;
	vFunction3ButtonEnable = true;
	vSubFunction6ButtonEnable = true;
	vOldShiftKeyDown = vGlobalShiftKeyDown;
	CString vTempString1 = "Number Of Products: ";
	CString vTempString2(vTempString1 + dtoa(vLocalProductCollection->GetUpperBound() + 1, 0));
	vScanTracCustomerName = vLocalConfigurationData->vScanTracCustomerName;
	//vCPMTimePeriodString = dtoa(vLocalConfigurationData->vCPMTimePeriod, 0);
	CDialog::OnShowWindow(bShow, nStatus);
	vIPDHCPEnabled = vLocalConfigurationData->vIPDHCPEnabled;
	vIPAddressString = vLocalConfigurationData->vIPAddressString;
	vIPSubNetMaskString = vLocalConfigurationData->vIPSubNetMaskString;
	vIPDefaultGatewayString = vLocalConfigurationData->vIPDefaultGatewayString;
	vIPDefaultDNSAddressString = vLocalConfigurationData->vIPDefaultDNSAddressString;
	vLanguage = vLocalConfigurationData->vLanguage;
	vAutoStop = vLocalConfigurationData->vAutoStop;

	CString TempText = "-";
	TempText.LoadString(IDS_AutoStop);
	SetDlgItemText(IDC_SubFunction7Button, TempText);

	//10/17/2009 read registry values for TeamView Client ID
	//string TempSecurityPasswordAESString set to display results 
	//vTeamViewerInstalled variable set for Teamviewer installed true or false
	CRegKey TempRegistryKey;
	DWORD dwType = REG_MULTI_SZ;
	CString TempSecurityPasswordAESString = "TV not installed"; 

	long TempErrorCode = TempRegistryKey.Open(HKEY_LOCAL_MACHINE, _T("Software\\TeamViewer\\Version8")); 
	if (TempErrorCode != ERROR_SUCCESS)
		TempErrorCode = TempRegistryKey.Open(HKEY_LOCAL_MACHINE, _T("Software\\TeamViewer\\Version7")); 
	if (TempErrorCode != ERROR_SUCCESS)
		TempErrorCode = TempRegistryKey.Open(HKEY_LOCAL_MACHINE, _T("Software\\TeamViewer\\Version6")); 
	if (TempErrorCode != ERROR_SUCCESS)
		TempErrorCode = TempRegistryKey.Open(HKEY_LOCAL_MACHINE, _T("Software\\TeamViewer\\Version4")); 
	if (TempErrorCode == ERROR_SUCCESS)
	{
			DWORD TempValueField;
			CString KeyName = "ClientID";
			TempErrorCode = TempRegistryKey.QueryDWORDValue(KeyName, TempValueField);
			if (TempErrorCode == ERROR_SUCCESS)
			{
				TempSecurityPasswordAESString = "TV ID: " + dtoa(TempValueField);
				vTeamViewerInstalled = true ;
			}
			else
			{
				TempSecurityPasswordAESString = "TV ID not installed";
			}
	}
	TempRegistryKey.Close();
	// end registry check for password for Teamviewer

	//	vTeamViewerInstalled = true ;  // uncomment for debugging

	if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
	{
		vSubFunction6ButtonEnable = false;	//m_SubFunction6Button.EnableWindow(false);		
		m_SubFunction6Button.Invalidate(false);
	}

	//make so does not highlight first button
	CWnd *TempDialogPointer = GetDlgItem(IDC_Background);

  GotoDlgCtrl(TempDialogPointer);
	// Toggle Display only Rejects

	//SetDlgItemText(IDC_CPMTimePeriodDisplay,vCPMTimePeriodString + " ");
	vOldSystemRunMode = vLocalSystemData->vSystemRunMode;
	vAskForProductionReportProductCode = vLocalConfigurationData->vAskForProductionReportProductCode;
	vAutoProductionReportHour = vLocalConfigurationData->vAutoProductionReportHour;
	vAutoProductionReportHour2 = vLocalConfigurationData->vAutoProductionReportHour2;
	vAutoProductionReportHour3 = vLocalConfigurationData->vAutoProductionReportHour3;

	// display results of registry check for password for Teamviewer 10/23/09
	//11/5/2009
	CString TempTeamViewerID = TempSecurityPasswordAESString;
	int TempPosition = TempTeamViewerID.Find(_T("not installed"), 0);

	if (TempPosition == -1) //did not find "not installed" so must have an ID
	if (TempTeamViewerID.GetLength() > 14)
	{
		TempTeamViewerID.Insert(10, _T(" "));
		TempTeamViewerID.Insert(14, _T(" "));
	}
	SetDlgItemText(IDC_TeamViewLabel,TempTeamViewerID);

	int TimerResult = SetTimer(vOneSecondTimerHandle,1000,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-One Second Timer Failed",cEMailInspx,32000);
	TimerResult = SetTimer(vPingTimerHandle,1000,NULL);
	if (!TimerResult)
			ReportErrorMessage("Error-Starting Ping Timer Failed",cEMailInspx,32000);
	
	this->SetWindowText(_T("Options"));

	
	m_Network1GreenFlash.ShowWindow(SW_HIDE);	// turn green light off
	m_Network1RedFlash.ShowWindow(SW_HIDE);		// turn red light off

	if (vMainWindowPointer)
		vMainWindowPointer->CheckIPSettingsAndDisplayInHelpDialog(false);

	::SetupMenu(vLocalCWndCollection);
	UpdateDisplay();
	UpdateTimeDisplay();
	UpdateIPAddressDisplay();
}

void CScanTracConfigureDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScanTracConfigureDialog)
	DDX_Control(pDX, IDC_Network2RedFlash, m_Network2RedFlash);
	DDX_Control(pDX, IDC_Network2GreenFlash, m_Network2GreenFlash);
	DDX_Control(pDX, IDC_Network1RedFlash, m_Network1RedFlash);
	DDX_Control(pDX, IDC_Network1GreenFlash, m_Network1GreenFlash);
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
	DDX_Control(pDX, IDC_Function5Display, m_Function5Display);
	DDX_Control(pDX, IDC_SubFunction1Display, m_SubFunction1Display);
	DDX_Control(pDX, IDC_SubFunction2Display, m_SubFunction2Display);
	DDX_Control(pDX, IDC_SubFunction3Display, m_SubFunction3Display);
	DDX_Control(pDX, IDC_SubFunction4Display, m_SubFunction4Display);
	DDX_Control(pDX, IDC_SubFunction5Display, m_SubFunction5Display);
	DDX_Control(pDX, IDC_Clock, m_Clock);
	//	DDX_Control(pDX, IDC_CPMTimePeriodDisplay, m_CPMTimePeriodDisplay);
	DDX_Control(pDX, IDC_Date, m_Date);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_DNSLabel, m_DNSLabel);
	DDX_Control(pDX, IDC_GatewayLabel, m_GatewayLabel);
	DDX_Control(pDX, IDC_TeamViewLabel, m_TeamViewLabel);
	DDX_Control(pDX, IDC_WorkStopTimePeriodDisplay, m_WorkStopTimePeriodDisplay);
	DDX_Control(pDX, IDC_BlackBox1, m_BlackBox1);
	DDX_Control(pDX, IDC_BlackBox2, m_BlackBox2);
	DDX_Control(pDX, IDC_SubFunction7Display, m_SubFunction7Display);
}


BEGIN_MESSAGE_MAP(CScanTracConfigureDialog, CDialog)
	//{{AFX_MSG_MAP(CScanTracConfigureDialog)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SubFunction1Button, OnSubFunction1Button)
	ON_BN_CLICKED(IDC_Function5Button, OnFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction2Button, OnSubFunction2Button)
	ON_BN_CLICKED(IDC_SubFunction3Button, OnSubFunction3Button)
	ON_BN_CLICKED(IDC_SubFunction4Button, OnSubFunction4Button)
	ON_BN_CLICKED(IDC_SubFunction5Button, OnSubFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction6Button, OnSubFunction6Button)
	ON_BN_CLICKED(IDC_SubFunction7Button, OnSubFunction7Button)
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_WM_TIMER()
	//ON_BN_CLICKED(IDC_RestorePrimaryHardDriveButton, OnRestorePrimaryHardDriveButton)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CScanTracConfigureDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScanTracConfigureDialog message handlers

void CScanTracConfigureDialog::OnFunction1Button() 
{
	// Main Menu
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::EndDialog(10);
}

void CScanTracConfigureDialog::OnFunction2Button() 
{
	//Exit button pressed
	//CheckSystemFullyConfigured(vLocalConfigurationData);
	if ((!vChangeMade) || (KeepSetupChanges()))
	{
		vLocalConfigurationData->vScanTracCustomerName = vScanTracCustomerName;
		//vLocalConfigurationData->vCPMTimePeriod = _wtoi(vCPMTimePeriodString);
		vLocalConfigurationData->vAutoProductionReportHour = vAutoProductionReportHour;
		vLocalConfigurationData->vAutoProductionReportHour2 = vAutoProductionReportHour2;
		vLocalConfigurationData->vAutoProductionReportHour3 = vAutoProductionReportHour3;
		vLocalConfigurationData->vAskForProductionReportProductCode = vAskForProductionReportProductCode;
		vLocalConfigurationData->vIPDHCPEnabled = vIPDHCPEnabled;
		vLocalConfigurationData->vIPAddressString = vIPAddressString;
		vLocalConfigurationData->vIPSubNetMaskString = vIPSubNetMaskString;
		vLocalConfigurationData->vIPDefaultGatewayString = vIPDefaultGatewayString;
		vLocalConfigurationData->vIPDefaultDNSAddressString = vIPDefaultDNSAddressString;
		vLocalConfigurationData->vLanguage = vLanguage;
		vGlobalLanguage = vLanguage;
		vLocalConfigurationData->vAutoStop = vAutoStop;
		if (vChangedLanguage)
			CDialog::EndDialog(10);
		else
			CDialog::EndDialog(true);
	}
}

void CScanTracConfigureDialog::OnFunction3Button() 
{
	//Create dialog box
	CConfigurePasswordsDialog IConfigurePasswordsDialog;  
	IConfigurePasswordsDialog.vMainWindowPointer = vMainWindowPointer;
	//Set dialog box data like how many products there are
	IConfigurePasswordsDialog.vLocalConfigurationData = vLocalConfigurationData;
	IConfigurePasswordsDialog.vLocalSystemData = vLocalSystemData;

	//Pass control to dialog box and display
	int nResponse = IConfigurePasswordsDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is accepted
	}
	else 
	if (nResponse == 10)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Main Menu
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CScanTracConfigureDialog::OnFunction4Button() 
{
	// Cancel
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::OnCancel();
}

void CScanTracConfigureDialog::OnFunction5Button() 
{
	//Create dialog box
	CConfigureDirectoryNames IConfigureDirectoryNames;  
	IConfigureDirectoryNames.vMainWindowPointer = vMainWindowPointer;
	//Set dialog box data like how many products there are
	IConfigureDirectoryNames.vLocalConfigurationData = vLocalConfigurationData;
	IConfigureDirectoryNames.vLocalSystemData = vLocalSystemData;

	//Pass control to dialog box and display
	int nResponse = IConfigureDirectoryNames.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is accepted
	}
	else 
	if (nResponse == 10)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Main Menu
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CScanTracConfigureDialog::OnSubFunction1Button() 
{
	CSelectItemDialog ISelectItemDialog;  
	ISelectItemDialog.vNumberOfItems = 3;
	ISelectItemDialog.vItems[0] = "Don't use Product Codes in Production Reports";
	ISelectItemDialog.vItems[1] = "Use Product Codes, select from a list";
	ISelectItemDialog.vItems[2] = "Use Product Codes, enter each time";

	//Set dialog box data titles and number value
	ISelectItemDialog.vTitleString = "Select Product Code for Product Report Configuration";
	ISelectItemDialog.vBackgroundColor = cGreen;
	ISelectItemDialog.vShowExit = true;
	ISelectItemDialog.vGreenItem = vAskForProductionReportProductCode + 1;

	ISelectItemDialog.m_DialogTitleStaticText2 = "Currently: " + ISelectItemDialog.vItems[vAskForProductionReportProductCode];
	ISelectItemDialog.m_DialogTitleStaticText3 = "";
	//Pass control to dialog box and display
	int nResponse = ISelectItemDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vAskForProductionReportProductCode = ISelectItemDialog.vItemNumberSelected - 1;
		SetChangeMade();
		UpdateDisplay();
	}
	else
	if (nResponse != 11)
	{
		if (nResponse == 10)
		{
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
}

void CScanTracConfigureDialog::OnSubFunction2Button() 
{
	CAlphaEntryDialog IAlphaEntryDialog;  
	IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
	//Set dialog box data titles and number value
	IAlphaEntryDialog.vEditString = vScanTracCustomerName;

	IAlphaEntryDialog.m_DialogTitleStaticText = "Customer Name";

	IAlphaEntryDialog.m_DialogTitleStaticText2 = "Default Format: Customer Name (City, State) Line #";
	IAlphaEntryDialog.vAllowMoreThan32Characters = true;
	IAlphaEntryDialog.vBlankNameOK = true;
	IAlphaEntryDialog.vAllowApproximate = true;
	IAlphaEntryDialog.vAllowDashes = true;
	IAlphaEntryDialog.vAllowComma = true;
	IAlphaEntryDialog.vAllowPeriod = true;
	IAlphaEntryDialog.vAcceptAllKeys = true;
	IAlphaEntryDialog.vAllowParenthesis = true;
	IAlphaEntryDialog.vLocalSystemData = this->vLocalSystemData;

	//Pass control to dialog box and display
	int nResponse = IAlphaEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vScanTracCustomerName = IAlphaEntryDialog.vEditString;
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

//9/30/2009
void CScanTracConfigureDialog::OnSubFunction3Button() 
{
	if (vGlobalShiftKeyDown)
	{
		vGlobalShiftKeyDown = false;
		if (vMainWindowPointer)
		{
			vMainWindowPointer->CheckIPSettingsAndDisplayInHelpDialog(true);
		}
	}
	else
	if ((vGlobalNetworkBoardCode == "Unknown") && (vGlobalPCRunningWindowsXP == cWindowsNT))
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\nCan not read IP Address in Registry,\nSo Can Not Set IP Address in Registry";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
		return;
	}
	else
	if ((vGlobalRegistryKeyPath == "Unknown") && (vGlobalPCRunningWindowsXP == cWindowsNT))
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\nCan not read Registry,\nSo Can Not Set IP Address in Registry";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	else
	{
		KillTimer(vPingTimerHandle);
		vGlobalPingReply = cPingResultNotSent;
	//	m_Network1GreenFlash.ShowWindow(SW_HIDE);	// turn green light off
		//m_Network1RedFlash.ShowWindow(SW_HIDE);		// turn red light off

		//m_Network2GreenFlash.ShowWindow(SW_HIDE);	// turn green light off
		//m_Network2RedFlash.ShowWindow(SW_HIDE);		// turn red light off
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText = _T("\nUse Static IP Address?\nWarning Dynamic IP Address from DHCP Server\nNormally does not allow remote network access");
		TempYesNoDialog.vYesButtonText = "Use Static Address";
		TempYesNoDialog.vNoButtonText = "Use Dynamic";
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		bool TempIPDHCPEnabled = true;
		if (TempResult == IDOK)
			TempIPDHCPEnabled = false;

		if (!TempIPDHCPEnabled)   //static IP address
		{
			//edit IP Address
			CAlphaEntryDialog IAlphaEntryDialog;  
			IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
			//Set dialog box data titles and number value
			vIPAddressString = vGlobalIPAddressString;
			IAlphaEntryDialog.vEditString = vIPAddressString;
			IAlphaEntryDialog.m_DialogTitleStaticText = "Change IP Address for this ScanTrac";

			IAlphaEntryDialog.m_DialogTitleStaticText2 = "Must be format 000.000.000.000";
			IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
			IAlphaEntryDialog.vAllowPeriod = true;
			IAlphaEntryDialog.vNumbersOnly = true;
			IAlphaEntryDialog.vAcceptAllKeys = false;

			//Pass control to dialog box and display
			int nResponse = IAlphaEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				CString TempIPAddressString = IAlphaEntryDialog.vEditString;

				WORD TempPosition = vIPSubNetMaskString.Find(_T("255.255"),0);
				if (TempPosition > 20)//if don't have 255.255 in it, set the mask to the default
					vIPSubNetMaskString = "255.255.255.0";

				IAlphaEntryDialog.vEditString = vIPSubNetMaskString;
				IAlphaEntryDialog.m_DialogTitleStaticText = "Change network Subnet Mask for this ScanTrac";
				//Pass control to dialog box and display
				int nResponse = IAlphaEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					CString TempIPSubNetMaskString = IAlphaEntryDialog.vEditString;

					WORD TempLength = vIPDefaultGatewayString.GetLength();
					if (TempLength < 9)
					{
						vIPDefaultGatewayString = vIPAddressString;
						while ((vIPDefaultGatewayString.GetLength() > 4) && 
							(vIPDefaultGatewayString.GetAt(vIPDefaultGatewayString.GetLength() - 1) != '.'))
						{
							vIPDefaultGatewayString.Delete(vIPDefaultGatewayString.GetLength() - 1,1);
						}
						vIPDefaultGatewayString = vIPDefaultGatewayString + "1";
					}

					IAlphaEntryDialog.vEditString = vIPDefaultGatewayString;
					IAlphaEntryDialog.m_DialogTitleStaticText = "Change network Default Gateway for this ScanTrac";
					//Pass control to dialog box and display
					int nResponse = IAlphaEntryDialog.DoModal();
					//dialog box is now closed, if user pressed select do this
					//if user pressed cancel, do nothing
					if (nResponse == IDOK)
					{
						CString TempIPDefaultGatewayString = IAlphaEntryDialog.vEditString;


						if (vGlobalPCRunningWindowsXP >= cWindowsXP)		//XP requires when changing/setting network values 5/6/10 kjh
						{
							WORD TempLength = vGlobalIPDNSAddressString1.GetLength();
							if (TempLength < 9)
							{
								vIPDefaultDNSAddressString = vIPAddressString;	// set the string to a reasonable value is string is blank
							}
							else
							{
								vIPDefaultDNSAddressString = vGlobalIPDNSAddressString1;
							}

//							IAlphaEntryDialog.vEditString = vIPGlobalDNSAddressString;
							IAlphaEntryDialog.vEditString = vIPDefaultDNSAddressString;
							IAlphaEntryDialog.m_DialogTitleStaticText = "Change network DNS for this ScanTrac";
							IAlphaEntryDialog.m_DialogTitleStaticText3 = "If 2 DNS Servers, separate with a comma";
							IAlphaEntryDialog.vAllowMoreThan32Characters = true;
							IAlphaEntryDialog.vAllowIPComma = true;
							IAlphaEntryDialog.vNumbersOnly = true;
							//Pass control to dialog box and display
							nResponse = IAlphaEntryDialog.DoModal();
							//dialog box is now closed, if user pressed select do this
							//if user pressed cancel, do nothing
							if (nResponse == IDOK)
							{
								vIPDefaultDNSAddressString = IAlphaEntryDialog.vEditString; 
								vGlobalIPDNSAddressString1 = vIPDefaultDNSAddressString;
							}
						}


						if (nResponse == IDOK)
						{
							//9/4/2009
							vIPAddressString = TempIPAddressString;
							vIPSubNetMaskString = TempIPSubNetMaskString;
							vIPDefaultGatewayString = TempIPDefaultGatewayString;
							vLocalConfigurationData->vIPAddressString = TempIPAddressString;
							vLocalConfigurationData->vIPSubNetMaskString = TempIPSubNetMaskString;
							vLocalConfigurationData->vIPDefaultGatewayString = TempIPDefaultGatewayString;
							vLocalConfigurationData->vIPDHCPEnabled = false;	//static/Fixed IP Addressing

							//9/30/2009
							vGlobalIPAddressString = TempIPAddressString;
							vGlobalIPSubNetMaskString = TempIPSubNetMaskString;
							vGlobalIPDefaultGatewayString = TempIPDefaultGatewayString;
							vGlobalIPDHCPEnabled = TempIPDHCPEnabled;
							vIPDHCPEnabled = TempIPDHCPEnabled;
							vLocalConfigurationData->vIPDHCPEnabled = TempIPDHCPEnabled;

							if (vGlobalPCRunningWindowsXP == cWindowsNT)
								UpdateDisplay();
							vMainWindowPointer->SetIPSettingsInRegistryAndReboot();
							
							ReportErrorMessage("Operator Changed IP Address to Static Addressing: " + TempIPAddressString, cUserChanges, 0);

							vReadCount = 0;
							int TimerResult = SetTimer(vUpdateIPAddressTimerHandle,1000,NULL);
							if (!TimerResult)
								ReportErrorMessage("Error-Update IP Display Timer Failed",cEMailInspx,32000);
						}
					}
				}
			}
		}
		else
		{//enable DHCP, Dynamic Addressing
			//9/30/2009
			vGlobalIPDHCPEnabled = true;
			vIPDHCPEnabled = true;
			vLocalConfigurationData->vIPDHCPEnabled = true;

			vLocalConfigurationData->vIPAddressString = "0.0.0.0";
			vLocalConfigurationData->vIPSubNetMaskString = "0.0.0.0";
			vLocalConfigurationData->vIPDefaultGatewayString = "0.0.0.0";
			vGlobalIPAddressString = "0.0.0.0";
			vGlobalIPSubNetMaskString = "0.0.0.0";
			vGlobalIPDefaultGatewayString = "0.0.0.0";

			vMainWindowPointer->SetIPSettingsInRegistryAndReboot();
			if (vGlobalPCRunningWindowsXP == cWindowsNT)
				UpdateDisplay();

			vReadCount = 0;
			int TimerResult = SetTimer(vUpdateIPAddressTimerHandle,1000,NULL);
			if (!TimerResult)
				ReportErrorMessage("Error-Update IP Display Timer Failed",cEMailInspx,32000);

			ReportErrorMessage("Operator Changed IP Address to Dynamic Addressing (DHCP) ", cUserChanges, 0);
		}
		vGlobalPingReply = cPingResultNotSent;
		UpdateDisplay();
	}
	UpdateIPAddressDisplay();
	int TimerResult = SetTimer(vPingTimerHandle,1000,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-Restarting Ping Timer Failed",cEMailInspx,32000);
}

void CScanTracConfigureDialog::OnSubFunction4Button() 
{
	CNumericEntryDialog INumericEntryDialog;  
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vAutoProductionReportHour);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the Hour of the Day to make an ";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Automatic Production Report";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Note: This will also clear the counters";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

	INumericEntryDialog.m_UnitsString = "0 = Disabled, 6 = 6 AM, 18 = 6 PM, 24 = Midnight";
	INumericEntryDialog.vMaxValue = 24;
	INumericEntryDialog.vMinValue = 0;
	INumericEntryDialog.vIntegerOnly = true;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		SetChangeMade();
		vAutoProductionReportHour = _wtoi(INumericEntryDialog.vEditString);
		UpdateDisplay();

		if (vAutoProductionReportHour)
		{
			INumericEntryDialog.vEditString = dtoa(vAutoProductionReportHour2);
			INumericEntryDialog.m_DialogTitleStaticText2 = "Automatic Production Report 2";
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				SetChangeMade();
				vAutoProductionReportHour2 = _wtoi(INumericEntryDialog.vEditString);
				UpdateDisplay();

				if (vAutoProductionReportHour2)
				{
					INumericEntryDialog.vEditString = dtoa(vAutoProductionReportHour3);
					INumericEntryDialog.m_DialogTitleStaticText2 = "Automatic Production Report 3";
					INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

					//Pass control to dialog box and display
					int nResponse = INumericEntryDialog.DoModal();
					//dialog box is now closed, if user pressed select do this
					//if user pressed cancel, do nothing
					if (nResponse == IDOK)
					{
						SetChangeMade();
						vAutoProductionReportHour3 = _wtoi(INumericEntryDialog.vEditString);
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
					vAutoProductionReportHour3 = 0;
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
		{
			vAutoProductionReportHour2 = 0;
			vAutoProductionReportHour3 = 0;
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

void CScanTracConfigureDialog::OnSubFunction5Button() 
{
	//choose which Language
	CSelectItemDialog ISelectItemDialog;
	ISelectItemDialog.vTitleString = "Select Language for User Menus";
	ISelectItemDialog.vNumberOfItems = 5;
	ISelectItemDialog.vBackgroundColor = cGreen;

	ISelectItemDialog.vItems[0] = "English";
	ISelectItemDialog.vItems[1] = "French";
	ISelectItemDialog.vItems[2] = "Spanish";
	ISelectItemDialog.vItems[3] = "Polish";
	ISelectItemDialog.vItems[4] = "German";
	ISelectItemDialog.m_DialogTitleStaticText2 = "";
	ISelectItemDialog.vGreenItem = vLanguage;
	ISelectItemDialog.m_DialogTitleStaticText3 = "";
	//Pass control to dialog box and display
	int nResponse = ISelectItemDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		if (ISelectItemDialog.vItemNumberSelected != vLanguage)
		{
			CYesNoDialog TempYesNoDialog;
			TempYesNoDialog.vNoticeText = _T("\nAre you sure you want to change the display language?\nConfirming will take you to the Main Menu and\ndisplay it with the new language.");
			TempYesNoDialog.vYesButtonText = "Change Language";
			TempYesNoDialog.vNoButtonText = "Cancel";
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
			{
				vLanguage = ISelectItemDialog.vItemNumberSelected;
				vLocalConfigurationData->vLanguage = vLanguage;
				vChangedLanguage = true;
				if (vLocalConfigurationData->vLanguage == cSpanish)
				{
					CYesNoDialog TempYesNoDialog;
					TempYesNoDialog.vNoticeText = _T("\n\n\nUse . or , in numbers?");
					TempYesNoDialog.vQuestionType = cQuestion;
					TempYesNoDialog.vYesButtonText = ".";
					TempYesNoDialog.vNoButtonText = ",";
					int TempResult = TempYesNoDialog.DoModal();
					//if user said OK
					if (TempResult != IDOK)
					{
						vGlobalUseCommaForDecimalPoint = true;
					}
				}
				OnFunction2Button(); //press save and exit
			}
		}
		SetChangeMade();
		UpdateDisplay();
	}
	//if (PasswordOK(cSetupPassword, true))
	//{
	//	//Containers Per Minute Time Period Button Pressed
	//	CNumericEntryDialog INumericEntryDialog;  
	//	
	//	//Set dialog box data titles and number value
	//	INumericEntryDialog.vEditString = vCPMTimePeriodString;

	//	INumericEntryDialog.m_DialogTitleStaticText1 = "Containers Per Minute Time Period";
	//	INumericEntryDialog.m_DialogTitleStaticText2 = "Display Setting";
	//	INumericEntryDialog.m_DialogTitleStaticText3 = "Time Interval CPM is calculated over";
	//	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + vCPMTimePeriodString;

	//	INumericEntryDialog.m_UnitsString = "Time in Seconds";
	//	INumericEntryDialog.vMaxValue = cMaxCPMTimePeriod;
	//	INumericEntryDialog.vMinValue = 2;
	//	INumericEntryDialog.vIntegerOnly = true;
	//	//Pass control to dialog box and display
	//	int nResponse = INumericEntryDialog.DoModal();
	//	//dialog box is now closed, if user pressed select do this
	//	//if user pressed cancel, do nothing
	//	if (nResponse == IDOK)
	//	{
	//		SetChangeMade();
	//		vCPMTimePeriodString = INumericEntryDialog.vEditString;
	//		vLocalSystemData->vCPMDataIndex = 0;
	//		vLocalSystemData->vHaveAllCPMData = false;
	//		for (WORD TempLoop	= 0; TempLoop < cMaxCPMTimePeriod; TempLoop++)
	//			vLocalSystemData->vCPMData[TempLoop] = 0;

	//		SetDlgItemText(IDC_CPMTimePeriodDisplay,vCPMTimePeriodString + " ");
	//	}
	//	else 
	//	if (nResponse == 10)
	//	{
	//		//Main Menu button pressed
	//		Sleep(1); //is the equivelent of a yeild statement;
	//		CDialog::EndDialog(10);
	//	}
	//}
}

void CScanTracConfigureDialog::OnSubFunction6Button() 
{
	// check variable to see if TeamViewer is installed
	if (vTeamViewerInstalled)
	{
		// only change password if system is in stopped mode
		if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		{
			//confirm y/n dialog then set TV password 
			CYesNoDialog TempYesNoDialog;
			CString TempString = "\nChange Team Viewer Password to an Emergency Value?";
			TempString = TempString + "\n\nOnly do this if instructed to by an Inspx Service person.";
			TempYesNoDialog.vNoticeText = TempString;
			TempYesNoDialog.vYesButtonText = "Change Password";
			TempYesNoDialog.vNoButtonText = "Cancel";
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
			if (HardDriveProtectionIsOff())
			{
				// reconfirm again
				CYesNoDialog Temp2YesNoDialog;
				Temp2YesNoDialog.vNoticeText = _T("\nScanTrac will reboot after change.\nDo not change password unless directed to by Inspx.\nAre you sure you want to change the password?");
				Temp2YesNoDialog.vYesButtonText = "Reboot Now";
				Temp2YesNoDialog.vNoButtonText = "Cancel";
				Temp2YesNoDialog.vQuestionType = cConfirmQuestion;
				int Temp2Result = Temp2YesNoDialog.DoModal();
				if (Temp2Result == IDOK)
				{
					//change password to default 3059peco
					CRegKey TempRegistryKey;
					DWORD dwType = REG_MULTI_SZ;

					long TempErrorCode = TempRegistryKey.Open(HKEY_LOCAL_MACHINE, _T("Software\\TeamViewer\\Version8")); 
					if (TempErrorCode != ERROR_SUCCESS)
						TempErrorCode = TempRegistryKey.Open(HKEY_LOCAL_MACHINE, _T("Software\\TeamViewer\\Version7")); 
					if (TempErrorCode != ERROR_SUCCESS)
						TempErrorCode = TempRegistryKey.Open(HKEY_CURRENT_USER, _T("Software\\TeamViewer\\Version6")); 
					if (TempErrorCode != ERROR_SUCCESS)
						TempErrorCode = TempRegistryKey.Open(HKEY_CURRENT_USER, _T("Software\\TeamViewer\\Version4")); 

					if (TempErrorCode == ERROR_SUCCESS)
					{
						LPTSTR TempKeyString = _T("SecurityPasswordAES");
	// read the key
	//				CString TempSecurityPasswordAESString = "TV not installed"; 
	//				DWORD TempCount = 20;
	//				unsigned char TempAString[100];
	//				LONG lRes = RegQueryValueEx(TempRegistryKey.m_hKey, TempKeyString, NULL, &dwType, (LPBYTE)TempAString, &TempCount);
	//
	//				if ((TempCount >= 20) && (TempCount < 100))
	//				{
	//					lRes = RegQueryValueEx(TempRegistryKey.m_hKey, TempKeyString, NULL, &dwType, (LPBYTE)TempAString, &TempCount);
	//				}
	//
	//				if (lRes == 0)
	//				{
	//					TempSecurityPasswordAESString = TempAString;
	//					TempSecurityPasswordAESString = TempSecurityPasswordAESString.GetBufferSetLength(TempCount);
	//				}
	//				else
	//				{
	//					TempSecurityPasswordAESString = "ID not installed";
	//				}
	// write the key
						unsigned char TempBString[100] = {	0x89, 0x9B, 0x4A, 0x74, 0x73, 0x53, 0xF2, 0x74, 
																								0x11, 0xA5, 0x4A, 0x40, 0x19, 0xA1, 0x70, 0xF4,	
																								0xFF, 0x68, 0xEA, 0x0F, 0x87, 0x4C, 0x1C, 0x46,	
																								0x7C, 0x4A, 0x96, 0xA4, 0xEC, 0x14, 0xB2, 0x54	};
						DWORD TempCount = 0x20 ;

		//type is REG_BINARY, REG_DWORD, or REG_MULTI_SZ, or REG_SZ
						TempErrorCode = RegSetValueEx(TempRegistryKey.m_hKey, TempKeyString, NULL, REG_BINARY, (BYTE * const)TempBString, TempCount);
						if (TempErrorCode == ERROR_SUCCESS)
						{
							vMainWindowPointer->WriteConfigurationDataToFile(false);
							SystemShutDown(cReboot);
						}

						TempRegistryKey.Close();
					}// end registry read ok

					//check if registry write was okay, if not okay then display message
					if (TempErrorCode != ERROR_SUCCESS)
					{
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\nSystem Error -- Contact Inspx";
						TempNoticeDialog.vType = cInformationMessage;
						TempNoticeDialog.DoModal();
					}
				} //end second check for password change
			}	//end first check for password change
		}	// end if system is in stop
		else	// if system is in stop display need to be in stop mode to change password
		{
			//	Dont change password, inform user in the wrong mode
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\nPassword change requires a reboot.\nYou cannot do this function while the system is running.";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
		}
	}		// end if TeamView is installed
	else
	{		// message for Team Viewer not installed
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\nTV is not installed.";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
	}
}

void CScanTracConfigureDialog::OnSubFunction7Button() 
{
	CNumericEntryDialog INumericEntryDialog;

	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vAutoStop);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the Hour of no production after ";
	INumericEntryDialog.m_DialogTitleStaticText2 = "which inspection will be stopped";
	INumericEntryDialog.m_DialogTitleStaticText3 = "To turn off X-Rays if operator forgets";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

	INumericEntryDialog.m_UnitsString = "0 = Disabled, Units in Hours";
	INumericEntryDialog.vMaxValue = 24;
	INumericEntryDialog.vMinValue = 0;
	INumericEntryDialog.vIntegerOnly = true;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		SetChangeMade();
		vAutoStop = _wtoi(INumericEntryDialog.vEditString);
		UpdateDisplay();
		if (vAutoStop)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\nIt is STRONGLY recommended to Interlock the conveyor with the ScanTrac System Inspecting output when the Auto Stop feature is Enabled";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
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

void CScanTracConfigureDialog::OnSubFunction8Button() 
{
	//set date
	bool TempOK = true;
	WORD TempMonth = 0;
	WORD TempDay = 0;
	WORD TempYear = 0;
	WORD TempHour = 0;
	WORD TempMinute = 0;
	WORD TempSecond = 0;

	CTime TempTime;
	TempTime = CTime::GetCurrentTime();
	CString TempTimeString = TempTime.Format("%#I:%M:%S%p");
	
	CString TempDateString = TempTime.Format("%B %#d, %Y");

	CString TempTimeString1 = TempTime.Format("%d, %H:%M:%S");

	SYSTEMTIME TempSystemTime1;
	GetLocalTime(&TempSystemTime1);
	TempMonth = TempSystemTime1.wMonth;
	TempDay = TempSystemTime1.wDay;
	TempYear = TempSystemTime1.wYear;
	TempHour = TempSystemTime1.wHour;
	TempMinute = TempSystemTime1.wMinute;
	TempSecond = TempSystemTime1.wSecond;

	CSelectItemDialog ISelectItemDialog;  
	ISelectItemDialog.vNumberOfItems = 12;
	ISelectItemDialog.vItems[0] = "January";
	ISelectItemDialog.vItems[1] = "February";
	ISelectItemDialog.vItems[2] = "March";
	ISelectItemDialog.vItems[3] = "April";
	ISelectItemDialog.vItems[4] = "May";
	ISelectItemDialog.vItems[5] = "June";
	ISelectItemDialog.vItems[6] = "July";
	ISelectItemDialog.vItems[7] = "August";
	ISelectItemDialog.vItems[8] = "September";
	ISelectItemDialog.vItems[9] = "October";
	ISelectItemDialog.vItems[10] = "November";
	ISelectItemDialog.vItems[11] = "December";

	//Set dialog box data titles and number value
	ISelectItemDialog.vTitleString = "Select the Current Month";
	ISelectItemDialog.vBackgroundColor = cGreen;
	ISelectItemDialog.vShowExit = true;

	ISelectItemDialog.m_DialogTitleStaticText2 = "For System Date";
	ISelectItemDialog.m_DialogTitleStaticText3 = "Original Date: " + TempDateString;
	//Pass control to dialog box and display
	int nResponse = ISelectItemDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
		TempMonth = ISelectItemDialog.vItemNumberSelected;
	else
	if (nResponse != 11)
	{
		TempOK = false;
		if (nResponse == 10)
		{
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}

	CNumericEntryDialog INumericEntryDialog;  
	if (TempOK)
	{
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(TempDay, 0); 
		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the Day of the Month";
		INumericEntryDialog.m_DialogTitleStaticText2 = "";
		INumericEntryDialog.m_UnitsString = "";

		INumericEntryDialog.m_DialogTitleStaticText3 = "Setup System Date";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Date: " + TempDateString;

		INumericEntryDialog.vMaxValue = 31;
		INumericEntryDialog.vMinValue = 1;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
			TempDay = _wtoi(INumericEntryDialog.vEditString);
		else
		{
			TempOK = false;
			if (nResponse == 10)
			{
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
	}
		
	if (TempOK)
	{
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(TempYear, 0); 
		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the Current Year";
		INumericEntryDialog.m_DialogTitleStaticText2 = "";
		INumericEntryDialog.m_UnitsString = "";

		INumericEntryDialog.m_DialogTitleStaticText3 = "Setup System Date";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Date: " + TempDateString;

		INumericEntryDialog.vMaxValue = 2037;
		INumericEntryDialog.vMinValue = 2000;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			TempYear = _wtoi(INumericEntryDialog.vEditString);
			if (TempYear < 2001)
				TempYear = 2001;
		}
		else
		{
			TempOK = false;
			if (nResponse == 10)
			{
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
	}
		
	if (TempOK)
	{
		SYSTEMTIME TempSystemTime;
		TempSystemTime.wDayOfWeek = 1;
		TempSystemTime.wMilliseconds = 1;
		//if (TempHour < 4)
		//	TempSystemTime.wHour = 24 + TempHour - 4;  //must correct for time zone
		//else
			TempSystemTime.wHour = TempHour;// - 4;  //must correct for time zone
		TempSystemTime.wMinute = TempMinute;
		TempSystemTime.wSecond = TempSecond;
		TempSystemTime.wMonth = TempMonth;
		TempSystemTime.wDay = TempDay;
		TempSystemTime.wYear = TempYear;

		if ((TempYear) && (TempDay) && (TempMonth))
		{
			TempTime = CTime::GetCurrentTime();
			CString TempOriginalDate = TempTime.Format("%B %#d, %Y");
			if (SetLocalTime(&TempSystemTime))
			{
				//new Date has been set
				TempTime = CTime::GetCurrentTime();
				CString TempCurrentDate = TempTime.Format("%B %#d, %Y");
				ReportErrorMessage("Notice Date changed from: " + TempOriginalDate + " to: " + TempCurrentDate, cEMailInspx, 32002); //32002 makes green in log
				ReportErrorMessage("Notice Date changed from: " + TempOriginalDate + " to: " + TempCurrentDate, cUserChanges, 0);

				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nThe Date has been set";
				//TempNoticeDialog.vAutoClose = 15000;
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.DoModal();
			}
			else
			{
				//error, time not set
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nError - The Time has NOT been set";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
		}
	}
	//set time
	TempTime = CTime::GetCurrentTime();
	TempTimeString = TempTime.Format("%#I:%M:%S%p");

	TempDateString = TempTime.Format("%B %#d, %Y");

	TempTimeString1 = TempTime.Format("%d, %H:%M:%S");

	GetLocalTime(&TempSystemTime1);
	TempMonth = TempSystemTime1.wMonth;
	TempDay = TempSystemTime1.wDay;
	TempYear = TempSystemTime1.wYear;
	TempHour = TempSystemTime1.wHour;
	TempMinute = TempSystemTime1.wMinute;
	TempSecond = TempSystemTime1.wSecond;

	if (TempOK)
	{
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(TempHour, 0);
		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the Current Hour";
		INumericEntryDialog.m_DialogTitleStaticText2 = "24 Hours per Day";
		INumericEntryDialog.m_UnitsString = "";

		INumericEntryDialog.m_DialogTitleStaticText3 = "Setup System Time";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Time: " + TempTimeString;

		INumericEntryDialog.vMaxValue = 23;
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
			TempHour = _wtoi(INumericEntryDialog.vEditString);
		else
		{
			TempOK = false;
			if (nResponse == 10)
			{
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
	}
	if (TempOK)
	{
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(TempMinute, 0);
		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the Current Minute";
		INumericEntryDialog.m_DialogTitleStaticText2 = "";
		INumericEntryDialog.m_UnitsString = "";

		INumericEntryDialog.m_DialogTitleStaticText3 = "Setup System Time";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Time: " + TempTimeString;

		INumericEntryDialog.vMaxValue = 59;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
			TempMinute = _wtoi(INumericEntryDialog.vEditString);
		else
		{
			TempOK = false;
			if (nResponse == 10)
			{
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
	}
	if (TempOK)
	{
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(TempSecond, 0);
		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the Current Second";
		INumericEntryDialog.m_DialogTitleStaticText2 = "";
		INumericEntryDialog.m_UnitsString = "";

		INumericEntryDialog.m_DialogTitleStaticText3 = "Setup System Time";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Time: " + TempTimeString;

		INumericEntryDialog.vMaxValue = 59;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
			TempSecond = _wtoi(INumericEntryDialog.vEditString);
		else
		{
			TempOK = false;
			if (nResponse == 10)
			{
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
	}
	if (TempOK)
	{
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText = _T("\n\n\nShow Time on Main Menu in 24 hour format?");
		TempYesNoDialog.vYesButtonText = "24 Hour Display";
		TempYesNoDialog.vNoButtonText = "AM/PM Display";
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
			vLocalConfigurationData->vShow24HourTime = 1;
		else
			vLocalConfigurationData->vShow24HourTime = 0;

		SYSTEMTIME TempSystemTime;
		TempSystemTime.wDayOfWeek = 1;
		TempSystemTime.wMilliseconds = 1;
		//if (TempHour < 4)
		//	TempSystemTime.wHour = 24 + TempHour - 4;  //must correct for time zone
		//else
		TempSystemTime.wHour = TempHour;// - 4;  //must correct for time zone
		TempSystemTime.wMinute = TempMinute;
		TempSystemTime.wSecond = TempSecond;
		TempSystemTime.wMonth = TempMonth;
		TempSystemTime.wDay = TempDay;
		TempSystemTime.wYear = TempYear;

		if ((TempYear) && (TempDay) && (TempMonth))
		{
			TempTime = CTime::GetCurrentTime();
			CString TempOriginalTime = TempTime.Format("%#I:%M:%S%p");
			if (SetLocalTime(&TempSystemTime))
			{
				//new time has been set
				TempTime = CTime::GetCurrentTime();
				CString TempCurrentTime = TempTime.Format("%#I:%M:%S%p");
				ReportErrorMessage("Notice Time changed from: " + TempOriginalTime + " to: " + TempCurrentTime, cEMailInspx, 32002); //32002 makes green in log
				ReportErrorMessage("Notice Time changed from: " + TempOriginalTime + " to: " + TempCurrentTime, cUserChanges, 0);

				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nThe Time has been set";
				TempNoticeDialog.vType = cInformationMessage;
				//TempNoticeDialog.vAutoClose = 15000;
				TempNoticeDialog.DoModal();
			}
			else
			{
				//error, time not set
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nError - The Time has NOT been set";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
		}
	}
}

BOOL CScanTracConfigureDialog::PreTranslateMessage(MSG* pMsg) 
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
	if (pMsg->message == WM_KEYDOWN)
	{
		int TempKey = pMsg->wParam;
		CWnd *TempWindow;
		if (vGlobalShiftKeyDown)
		{
			if (TempKey == 122)
				OnSubFunction3Button();
			else
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
		}
		else
		switch (TempKey)
		{
			case 113: // F2:
				OnSubFunction5Button();
			break;
			case 114: // F3:
				OnSubFunction7Button();
				return true;
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
			case 36: // Home Key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
			case cPageDown: // Help
				PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 3, 0);

				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			break;
			case cPageUp: // Clear, but do inspx hidden dialog
				if (vGlobalPasswordLevel)
				{
					PasswordOK(cNoPassword,false);
				}
				else
				{
					PasswordOK(cAnyPasswordToEnter, true);
				}
				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			break;
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
		}
		return true;  //true indicates it has been handled, so do not process
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CScanTracConfigureDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
		pDC->SetAttribDC(pDC->m_hDC);
	// TODO: Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);  
		
		if (vGlobalShiftKeyDown)
			pDC->SetTextColor(cBlack);
		else
			pDC->SetTextColor(vGlobalDialogTitleColor);
	}
	if (pWnd->GetDlgCtrlID() == IDC_Background) 
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_GatewayLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_GatewayLabel, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DNSLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DNSLabel, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_TeamViewLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TeamViewLabel, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
/*
	if (pWnd->GetDlgCtrlID() == IDC_RestorePrimaryHardDriveButton)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
  }
*/
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
	if (pWnd->GetDlgCtrlID() == IDC_Function3Button) 
	{   
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vFunction3ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_WorkStopTimePeriodDisplay)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_WorkStopTimePeriodDisplay, 5);  //5 is medium large
	if (pWnd->GetDlgCtrlID() == IDC_Clock)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Clock, 5);  //5 is medium large
	if (pWnd->GetDlgCtrlID() == IDC_Date)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Date, 5);  //5 is medium large
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5);  //5 is medium large
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Display, 5);  //5 is medium large
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Display)
		TextSize = SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Display, 5);  //5 is medium large
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Display, 5);  //5 is medium large
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Display, 5);  //5 is medium large
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Display, 5);  //5 is medium large
// previously the code did not get to below, delete if not needed
//  kjh team viewer password set, need to add to change button to grey or yellow
//	else 	if(pWnd->GetDlgCtrlID() == IDC_SubFunction6Button)
//	{
//		if ((vLocalSystemData->vSystemRunMode == cStoppedSystemMode) && (vTeamViewerInstalled))
//		{
//
//			pDC->SetTextColor(cButtonTextColor);
//			pDC->SetBkMode(TRANSPARENT);
//			return vLocalSystemData->vYellowBrush;
//		}
//		else
//		{
//			pDC->SetTextColor(cButtonTextColor);
//			pDC->SetBkMode(TRANSPARENT);
//			return vLocalSystemData->vGrayBrush;
//		}
//	}
	if(pWnd->GetDlgCtrlID() == IDC_SubFunction6Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction6ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}

	return hbr;
}

void CScanTracConfigureDialog::SetChangeMade() 
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

void CScanTracConfigureDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
  if (nIDEvent == vOneSecondTimerHandle)
  {
		if (vOldGlobalPasswordOK != PasswordOK(cTemporaryInspxPassword,false))
		{
			vOldGlobalPasswordOK = PasswordOK(cTemporaryInspxPassword,false);
			CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
			if (TempWindow)
				TempWindow->Invalidate(false);
		}
		if (vOldSystemRunMode != vLocalSystemData->vSystemRunMode)
		{
			if (vOldSystemRunMode != cStoppedSystemMode)
			if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
			{
				vFunction3ButtonEnable = true;	//m_Function3Button.EnableWindow(true);				
				m_Function3Button.Invalidate(false);
				//m_Function5Button.EnableWindow(true);
			}
			vOldSystemRunMode = vLocalSystemData->vSystemRunMode;
		}

		if (vOldShiftKeyDown != vGlobalShiftKeyDown)
		{
			vOldShiftKeyDown = vGlobalShiftKeyDown;
			CWnd *TempWindow;
			TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
			if (TempWindow)
				TempWindow->Invalidate(false);

			UpdateDisplay();
		}

		UpdateTimeDisplay();
  }
	
	if (nIDEvent == vUpdateIPAddressTimerHandle)	
  {
		vReadCount++;
		if (vReadCount >= 60)
			KillTimer(vUpdateIPAddressTimerHandle);
		ReportErrorMessage("Read IP Address after operator changed", cAction, 0);
		if (vMainWindowPointer)
			vMainWindowPointer->CheckIPSettingsAndDisplayInHelpDialog(false);
		UpdateIPAddressDisplay();
	}
	if (nIDEvent == vPingTimerHandle)	
  {
		// if there is an answer from the thread, update results and run the thread again
		if (vGlobalPingStatus == cPingThreadDone)
		{
			PingTimeOut = 0;
			UpdatePingDisplay();
			vGlobalPingTestThread = NULL;
			vGlobalPingStatus = cPingThreadNull;
		}
		else
		if (!vGlobalPingTestThread)
		{
			// start Thread
			PingTimeOut = 0;
			CreatePingTestThread();
		}
		else
		{
			PingTimeOut++;
			if (PingTimeOut > 9)
			{
				vGlobalPingStatus = cPingThreadDone;
				ReportErrorMessage("Error-Ping Time Out after 10 seconds 2", cEMailInspx,32000); 
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CScanTracConfigureDialog::UpdateTimeDisplay()
{
	CTime TempTime;
	TempTime = CTime::GetCurrentTime();
	CString TempTimeString;
	if (vLocalConfigurationData->vShow24HourTime)
		TempTimeString = TempTime.Format("%H:%M:%S");
	else
		TempTimeString = TempTime.Format("%#I:%M:%S%p");
	SetDlgItemText(IDC_Clock,TempTimeString);
	
	CString TempDateString = TempTime.Format("%B %#d, %Y");
	if (vLastDateString != TempDateString)
	{
		SetDlgItemText(IDC_Date,TempDateString);
		vLastDateString = TempDateString;
	}
}

void CScanTracConfigureDialog::UpdateDisplay()
{
	if (vLanguage == cFrench)
	{
		SetDlgItemText(IDC_SubFunction5Display, _T("French"));
	}
	else
	if (vLanguage == cPolish)
	{
		SetDlgItemText(IDC_SubFunction5Display, _T("Polish"));
	}
	else
	if (vLanguage == cSpanish)
	{
		SetDlgItemText(IDC_SubFunction5Display, _T("Spanish"));
	}
	else
	if (vLanguage == cGerman)
	{
		SetDlgItemText(IDC_SubFunction5Display, _T("German"));
	}
	else
		SetDlgItemText(IDC_SubFunction5Display, _T("English"));

	CString TempString = ""; 
	TempString = "";
	//9/30/2009
	//UpdateIPAddressDisplay();

	if (vGlobalShiftKeyDown)
		SetDlgItemText(IDC_SubFunction3Button, _T("Display Network Settings"));
	else
		SetDlgItemText(IDC_SubFunction3Button, _T("Change Network Settings"));

	SetDlgItemText(IDC_SubFunction2Display,vScanTracCustomerName);

	SetDlgItemText(IDC_SubFunction1Button, _T("Product\nCode For\nReport"));
	if (vAskForProductionReportProductCode == 1)
		SetDlgItemText(IDC_SubFunction1Display, _T("Using Product Codes, select from a list"));
	else
	if (vAskForProductionReportProductCode == 2)
		SetDlgItemText(IDC_SubFunction1Display, _T("Using Product Codes, enter each time"));
	else
		SetDlgItemText(IDC_SubFunction1Display, _T("Not Using Production Report Product Code"));

	if (!vAutoProductionReportHour)
		TempString = "Disabled";
	else
	{
		TempString = dtoa(vAutoProductionReportHour);
		if (vAutoProductionReportHour2)
		{
			TempString = TempString + ", " + dtoa(vAutoProductionReportHour2);
			if (vAutoProductionReportHour3)
			{
				TempString = TempString + ", " + dtoa(vAutoProductionReportHour3);
			}
		}
	}

	SetDlgItemText(IDC_SubFunction4Display,"Hour of day: " + TempString);

	if (vAutoStop == 1)
		SetDlgItemText(IDC_SubFunction7Display, dtoa(vAutoStop, 0) + " hour");
	else
		if (vAutoStop)
			SetDlgItemText(IDC_SubFunction7Display, dtoa(vAutoStop, 0) + " hours");
		else
			SetDlgItemText(IDC_SubFunction7Display,  _T("Disabled"));
}

void CScanTracConfigureDialog::UpdateIPAddressDisplay()
{
	CString TempString = ""; 
	TempString = "";
	if (vGlobalIPDHCPEnabled)
	{
		//TempString = "TCPIP Address: DHCP Server Enabled\nDynamic-Not Suitable for most\nRemote Control over the Internet\nBoard Type: " + 
		//	vNetworkBoardCode + "\nDHCP IP Address: " + vDHCPIPAddressString + "\nDHCP Subnet Mask: " + vDHCPIPSubNetMaskString +
		//	"\nDHCP Default Gateway: " + vDHCPDefaultGatewayString + "\nDHCP Server: " + vDHCPIPServerString;
		if (vGlobalPCRunningWindowsXP == cWindowsNT)	// netsh sets regedit for xp
		{
			TempString = "TCPIP Address: DHCP Server Enabled\nDynamic-Not Suitable for most\nRemote Control over the Internet\nBoard Type: " + 
				vGlobalNetworkBoardCode + "\nDHCP IP Address: " + vGlobalIPAddressString + "\nDHCP Subnet Mask: " + vGlobalIPSubNetMaskString +
				"\nDHCP Gateway: " + vGlobalIPDefaultGatewayString + "\nDHCP Server: " + vGlobalIPDHCPServer;
		}
		else
		{
			TempString = "TCPIP Address: DHCP Server Enabled\nDynamic-Not Suitable for most\nRemote Control over the Internet\nDHCP IP Address: " + vGlobalIPAddressString + "\nDHCP Subnet Mask: " + vGlobalIPSubNetMaskString +
				"\nDHCP Gateway: " + vGlobalIPDefaultGatewayString + "\nDNS Server: " + vGlobalIPDNSAddressString1;
		}

		//m_SubFunction3Display.SetWindowPos(NULL,119,186,280,153,SWP_NOZORDER);
	}
	else
	{
		if (vGlobalPCRunningWindowsXP == cWindowsNT)	// netsh sets regedit for xp
		{
			TempString = "Static IP Address: " + vGlobalIPAddressString + "\nSubnet Mask: " + vGlobalIPSubNetMaskString +  
				"\nDefault Gateway: " + vGlobalIPDefaultGatewayString + "\nBoard Type: " + vGlobalNetworkBoardCode ;

			//m_SubFunction3Display.SetWindowPos(NULL,119,228,280,80,SWP_NOZORDER);
		}
		else
		{
			TempString = "Static IP Address: " + vGlobalIPAddressString + "\nSubnet Mask: " + vGlobalIPSubNetMaskString +  
				"\nDefault Gateway: " + vGlobalIPDefaultGatewayString + "\nDNS Server: " + vGlobalIPDNSAddressString1;

			//m_SubFunction3Display.SetWindowPos(NULL,119,228,280,97,SWP_NOZORDER);
		}
	}
	if ((vGlobalIPDHCPEnabled != vIPDHCPEnabled) || ((vIPDHCPEnabled == false) && ((vGlobalIPAddressString != vIPAddressString) || 
		(vGlobalIPSubNetMaskString != vIPSubNetMaskString) || (vGlobalIPDefaultGatewayString != vIPDefaultGatewayString))))
	{
		TempString = TempString + "\nScanTrac and Windows are Different";
		//m_SubFunction3Display.SetWindowPos(NULL,119,186,280,153,SWP_NOZORDER);
		if (vIPDHCPEnabled)
			TempString = TempString + "\nScanTrac Dynamic Addressing";  
		else
			TempString = TempString + "\nScanTrac Static IP: " + vIPAddressString + "\nSubnet Mask: " + vIPSubNetMaskString +  
				"\nDefault Gateway: " + vIPDefaultGatewayString;
	}

	SetDlgItemText(IDC_SubFunction3Display,TempString);
}

//	CString EchoDefaultGatewayString = "224.0.0.1" ; address current not pinged,was Network2RedFlash and Network2GreenFlash
void CScanTracConfigureDialog::UpdatePingDisplay() 
{
	// visual of Ping results
	if (vGlobalPingReply == cPingResultReply)
	{
		if (vPingDNS)
		{
			m_Network2GreenFlash.ShowWindow(SW_SHOW);	// turn green light on
			m_Network2RedFlash.ShowWindow(SW_HIDE);		// turn red light off
		}
		else
		{
			m_Network1GreenFlash.ShowWindow(SW_SHOW);	// turn green light on
			m_Network1RedFlash.ShowWindow(SW_HIDE);		// turn red light off
		}
	}
	else if (vGlobalPingReply == cPingResultNoReply) // network seen, no reply on ping 
	{
		if (vPingDNS)
		{
			m_Network2RedFlash.ShowWindow(SW_SHOW);	// turn red light on
			m_Network2GreenFlash.ShowWindow(SW_HIDE);	// turn green light off
		}
		else
		{
			m_Network1RedFlash.ShowWindow(SW_SHOW);	// turn red light on
			m_Network1GreenFlash.ShowWindow(SW_HIDE);	// turn green light off
		}
	}
	/*
	else	// (vIPPingGatewayReply == cPingResultNotSent)  network contection fail
	{
		m_Network1GreenFlash.ShowWindow(SW_HIDE);	// turn green light off
		m_Network1RedFlash.ShowWindow(SW_HIDE);		// turn red light off
	}
	*/
}

void CScanTracConfigureDialog::CreatePingTestThread()
{
	//m_Network1GreenFlash.ShowWindow(SW_HIDE);	// turn green light off
	//m_Network1RedFlash.ShowWindow(SW_HIDE);		// turn red light off

	//m_Network2GreenFlash.ShowWindow(SW_HIDE);	// turn green light off
	//m_Network2RedFlash.ShowWindow(SW_HIDE);		// turn red light off

	vGlobalPingTestThread = (CPingTestThread *)AfxBeginThread(
		RUNTIME_CLASS(CPingTestThread),
		//THREAD_PRIORITY_LOWEST  // dont use lowest, makes it too slow
		THREAD_PRIORITY_BELOW_NORMAL
		//THREAD_PRIORITY_NORMAL
		,0,CREATE_SUSPENDED);

	if (!vGlobalPingTestThread)
	{
		ReportErrorMessage("Error-Could Not Create Process Ping Test Thread 2", cEMailInspx,32000); 
		vGlobalPingStatus = cPingThreadNull;
	}
	else
	{
		vGlobalPingStatus = cPingThreadSending;
		vPingDNS = !vPingDNS;
		vGlobalPingTestThread->vPingDNS = vPingDNS;
		vGlobalPingTestThread->ResumeThread();
	}
}

/*
void CScanTracConfigureDialog::OnRestorePrimaryHardDriveButton() 
{
	if (vNeedToRestorePrimaryHardDrive)
	{
		if (!vLocalSystemData->vPrimaryDrive.GetLength())
			vLocalSystemData->vPrimaryDrive = "D";

		CYesNoDialog TempYesNoDialog;
		CString TempText = "Restore Primary Hard Drive From Backup?\nWarning: This will erase everything on the Primary Drive.\n";
		TempText = TempText +	"This copies the Backup Drive: ";
		TempText = TempText + vLocalSystemData->vBackupDrive + 
			" to Primary Drive: " + vLocalSystemData->vPrimaryDrive + 
			".\nYou can not inspect product during restore (20 minutes).\nYou must reboot computer after restore.";

		TempYesNoDialog.vNoticeText = TempText;
		TempYesNoDialog.vYesButtonText = "Restore Hard Drive";
		TempYesNoDialog.vNoButtonText = "Abort";
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
		{
			vLocalConfigurationData->vJustRestoredPrimaryHardDrive = true;

			if (vMainWindowPointer)
			{
				vMainWindowPointer->WriteConfigurationDataToFile(false); //write to main config file
			}

			//Sleep(100);
			// to do launch ghost

			int TempError = _execl("C:\\Windows\\explorer.exe", "C:\\Windows\\explorer.exe", "/e,C:\\ScanTrac", NULL);
			//int TempError = _execl("C:\\Program Files\\Ghost\\Ghost.exe", "C:\\Program Files\\Ghost\\Ghost.exe", "-Quiet", "-CLONE", "-FRO", 
			//"-move=COPY", "src=" + vLocalSystemData->vBackupDrive + ":\\", "dst=vLocalSystemData->vPrimaryDrive + ":\\", NULL);

			if (errno)
				ReportErrorMessage("Could not start Ghost To Restore Primary Hard Drive: " + dtoa(errno,0),cEMailInspx,22);
		}
	}
}
*/


void CScanTracConfigureDialog::OnDialogtitlestatictext1()
{
	//bring up soft key menu for help, print screen, Image, Clear, and hidden shift key
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}

bool CScanTracConfigureDialog::HardDriveProtectionIsOff()
{
	bool TempHardDriveProtectionIsOff = false;
	CFileStatus TempFileStatus;
	CString TempString1;
	CString TempFileName = "\\WINDOWS\\system32\\fbwfmgr.exe";
	bool TempFBWFAvailable = false;
	if (CFile::GetStatus(TempFileName, TempFileStatus))
		TempFBWFAvailable = true;
	else
		TempHardDriveProtectionIsOff = true;

	if (TempFBWFAvailable)
	{
		CString TempCString;
		ULONG TempReturnVal;

		//called on UpdateDisplay ULONG FBWF_Info::IsFBWFEnabled(unsigned long * currentSession,unsigned long * nextSession)
		if (vGlobalFBWFEnable)
		{	// currently next reboot enabled, switch to next reboot disabled
			CYesNoDialog TempYesNoDialog;
			TempYesNoDialog.vNoticeText =  _T("\nHard Drive Protection is Active.\nIt must be disabled before\nEmergency TV Password can be set.\nDo you want to Disable Hard Drive Protection\nand Reboot?");

			TempYesNoDialog.vYesButtonText = "Disable Protect";
			TempYesNoDialog.vNoButtonText = "Cancel";
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
			if ((TempReturnVal = DisableFBWF()) == NO_ERROR)
			{
				SystemShutDown(cReboot);
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nDisable Hard Drive Protection Failed, Call Inspx.";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
		}
		else
			TempHardDriveProtectionIsOff = true;
	}
	return(TempHardDriveProtectionIsOff);
}

// return call succeed or falled
ULONG CScanTracConfigureDialog::DisableFBWF(void)
{
	DWORD dwRetVal = 0;
	CString TempCString = "";

	typedef ULONG(CALLBACK* mytry) ();
	mytry NewFbwfDisableFilter;    // Function pointer

	// free Library is causing an MFC exception error.  Load Library once when start, make a global
	//	HMODULE hLibNetwork = LoadLibrary( ("fbwflib.dll")); // file name of module
	//	HINSTANCE hLibNetwork = LoadLibrary( ("fbwflib.dll")); // file name of module

	if (vGlobalhLibFBWFNetwork != NULL)
	{
		LPCSTR lpProcName = "FbwfDisableFilter";		// handle to DLL module // function name		 
		NewFbwfDisableFilter = (mytry)GetProcAddress(vGlobalhLibFBWFNetwork, lpProcName);

		if (NewFbwfDisableFilter != NULL)
		{
			dwRetVal = NewFbwfDisableFilter();
			//			if (dwRetVal == NO_ERROR)		reboot only changes status of FBWF disable
			//				vGlobalFBWFEnable = 0;		this is 0 only after a reboot
		}
	}

	return(dwRetVal);
}

