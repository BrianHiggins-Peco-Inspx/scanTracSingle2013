// SetupSystemExternalHardware.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "SetupSystemExternalHardware.h"
#include "NewConfigureSystemExternalDetectorDialog.h"
#include "NewConfigureSystemEjectorDialog.h"
#include "SystemValuesDialog.h"
#include "NumericEntryDialog.h"
#include "YesNoDialog.h"
#include "NoticeDialog.h"
#include "SystemSetupFillerSeamerSamplingDialog.h"
#include "AlphaEntryDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString vGlobalOperationsGuardianServer;
extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
/////////////////////////////////////////////////////////////////////////////
// CSetupSystemExternalHardware dialog


CSetupSystemExternalHardware::CSetupSystemExternalHardware(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupSystemExternalHardware::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetupSystemExternalHardware)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	vUseExternalInterlock = 0;
	vUseDownStreamSignalThatStopsConveyor = cDontUseDownStream;
	//vHideNeedDetectorCal = false;
	vEnableOperationsGuardian = false;
	vOperationsGuardianServer = cOperationsGuardianDefaultServerName;
	vChangeMade = false;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_SubFunction3Display);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Function5Display);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction4Display);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction7Display);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction8Display);
}


void CSetupSystemExternalHardware::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupSystemExternalHardware)
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_SubFunction3Display, m_SubFunction3Display);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction7Display, m_SubFunction7Display);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction8Display, m_SubFunction8Display);
	DDX_Control(pDX, IDC_SubFunction4Display, m_SubFunction4Display);
	DDX_Control(pDX, IDC_Function5Display, m_Function5Display);
}

void CSetupSystemExternalHardware::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);	
	
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

	vBackupDetectionUsingBodyTriggerTimeOut = vLocalConfigurationData->vBackupDetectionUsingBodyTriggerTimeOut;
	vUseDownStreamSignalThatStopsConveyor = vLocalConfigurationData->vUseDownStreamSignalThatStopsConveyor;
	//vHideNeedDetectorCal = vLocalConfigurationData->vHideNeedDetectorCal;
	vEnableOperationsGuardian = vLocalConfigurationData->vEnableOperationsGuardian;
	vOperationsGuardianServer = vLocalConfigurationData->vOperationsGuardianServer;
	vRFIDEASBadgeReaderCommPort = vLocalConfigurationData->vRFIDEASBadgeReaderCommPort;

	UpdateButtons();
	//make so does not highlight first button
	CWnd *TempDialogPointer = GetDlgItem(IDC_Background);
	GotoDlgCtrl(TempDialogPointer);

//	int TimerResult = SetTimer(vOneSecondTimerHandle,1000,NULL);
//	if (!TimerResult)
//		ReportErrorMessage("Error-One Second Timer Failed",cEMailInspx,32000);
	if ((vLocalConfigurationData->vScanTracType == cAllegroScanTrac) || (vLocalConfigurationData->vScanTracType == cBriosoScanTrac) || (vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || 
		(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || 
		(vLocalConfigurationData->vScanTracType == cLegatoScanTrac))
	{
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		m_SubFunction4Display.ShowWindow(SW_SHOW);
	}
	else
	{
		m_SubFunction4Button.ShowWindow(SW_HIDE);
		m_SubFunction4Display.ShowWindow(SW_HIDE);
	}

	this->SetWindowText(_T("Ext. Hardware"));
	::SetupMenu(vLocalCWndCollection);
}

BEGIN_MESSAGE_MAP(CSetupSystemExternalHardware, CDialog)
	//{{AFX_MSG_MAP(CSetupSystemExternalHardware)
	ON_BN_CLICKED(IDC_SubFunction1Button, OnSubFunction1Button)
	ON_BN_CLICKED(IDC_SubFunction2Button, OnSubFunction2Button)
	ON_BN_CLICKED(IDC_SubFunction3Button, OnSubFunction3Button)
	ON_BN_CLICKED(IDC_SubFunction4Button, OnSubFunction4Button)
	ON_BN_CLICKED(IDC_SubFunction5Button, OnSubFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction6Button, OnSubFunction6Button)
	ON_BN_CLICKED(IDC_SubFunction7Button, OnSubFunction7Button)
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
	ON_BN_CLICKED(IDC_Function5Button, OnFunction5Button)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CSetupSystemExternalHardware::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupSystemExternalHardware message handlers

void CSetupSystemExternalHardware::OnFunction1Button() 
{
	//Main Menu was pressed
	if ((!vChangeMade) || (AbortSetupChanges()))
	{
		CDialog::EndDialog(10);
	}
}

void CSetupSystemExternalHardware::OnFunction2Button() 
{
	//Exit button pressed
	if ((!vChangeMade) || (KeepSetupChanges()))
	{
		if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode)) //Continuous Feed, or a Pipeline
		{
			vBackupDetectionUsingBodyTriggerTimeOut = 0;
		}

		vLocalConfigurationData->vBackupDetectionUsingBodyTriggerTimeOut = vBackupDetectionUsingBodyTriggerTimeOut;
		vLocalConfigurationData->vUseDownStreamSignalThatStopsConveyor = vUseDownStreamSignalThatStopsConveyor;
		//vLocalConfigurationData->vHideNeedDetectorCal = vHideNeedDetectorCal;
		vLocalConfigurationData->vEnableOperationsGuardian = vEnableOperationsGuardian;
		vLocalConfigurationData->vOperationsGuardianServer = vOperationsGuardianServer;
		vGlobalOperationsGuardianServer = vOperationsGuardianServer;
		vLocalConfigurationData->vUseExternalInterlock = vUseExternalInterlock;
		vLocalConfigurationData->vRFIDEASBadgeReaderCommPort = vRFIDEASBadgeReaderCommPort;
	}
	CDialog::EndDialog(true);
}

void CSetupSystemExternalHardware::OnFunction3Button() 
{
	// TODO: Add your control notification handler code here
	CSystemSetupFillerSeamerSamplingDialog ISystemSetupFillerSeamerSamplingDialog;  
		
	ISystemSetupFillerSeamerSamplingDialog.vLocalConfigurationData = vLocalConfigurationData;
	ISystemSetupFillerSeamerSamplingDialog.vLocalSystemData = vLocalSystemData;
	//ISystemSetupFillerSeamerSamplingDialog.vLocalProductCollection = vLocalProductCollection;
	ISystemSetupFillerSeamerSamplingDialog.vMainWindowPointer = vMainWindowPointer;
	//Set dialog box data titles and number value
	int nResponse = ISystemSetupFillerSeamerSamplingDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == 10)
	{
		//Main Menu button pressed
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CSetupSystemExternalHardware::OnFunction4Button() 
{
	// TODO: Add your control notification handler code here
	if (vChangeMade)
	{
		CDialog::OnCancel();
	}
}

void CSetupSystemExternalHardware::OnFunction5Button() 
{
	// TODO: Add your control notification handler code here
#ifdef CompileInHTTPClientOperationsGuardian
	if (vEnableOperationsGuardian)
	{
		vEnableOperationsGuardian = false; 
		vMainWindowPointer->CloseOGServerSendMessageThread();
	}
	else
	{
		if (vOperationsGuardianServer.GetLength() < 10)
			vOperationsGuardianServer = cOperationsGuardianDefaultServerName;

		CAlphaEntryDialog IAlphaEntryDialog;  
		
		//Set dialog box data titles and number value
		IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
		IAlphaEntryDialog.vEditString = vOperationsGuardianServer;

		IAlphaEntryDialog.m_DialogTitleStaticText = "Enter Server Address for Operations Guardian";

		CString TempServerName = cOperationsGuardianDefaultServerName;
		CString TempString = "Default: " + TempServerName;
		IAlphaEntryDialog.m_DialogTitleStaticText2 = TempString;
		IAlphaEntryDialog.vAllowSlashes = true;
		IAlphaEntryDialog.vAllowMoreThan32Characters = true;
		IAlphaEntryDialog.vBlankNameOK = true;
		//Pass control to dialog box and display
		int nResponse = IAlphaEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vOperationsGuardianServer = IAlphaEntryDialog.vEditString;
			if (!vOperationsGuardianServer.IsEmpty())
			{
				vEnableOperationsGuardian = true;
				vMainWindowPointer->CreateOGServerSendMessageThread();
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
#else
	CNoticeDialog TempNoticeDialog;
	TempNoticeDialog.vNoticeText = "\n\nOperations Guardian is not available in this release of software\nYou need a release with Operations Guardian included.";
	TempNoticeDialog.vType = cNoticeMessage;
	TempNoticeDialog.DoModal();
	vEnableOperationsGuardian = false; 
#endif
	UpdateButtons();
	SetChangeMade();
}

void CSetupSystemExternalHardware::OnSubFunction1Button() 
{
	//setup Container Triggers Button
  //Create dialog box
	CNewConfigureSystemExternalDetectorDialog IConfigureSystemBodyTriggersDialog;  
	//Set dialog box data like how many products there are
	IConfigureSystemBodyTriggersDialog.vWindowType = cBodyTriggerWindow;
	IConfigureSystemBodyTriggersDialog.vLocalConfigurationData = vLocalConfigurationData;
	IConfigureSystemBodyTriggersDialog.vLocalSystemData = vLocalSystemData;
	IConfigureSystemBodyTriggersDialog.vMainWindowPointer = vMainWindowPointer;

  //Pass control to dialog box and display
  int nResponse = IConfigureSystemBodyTriggersDialog.DoModal();
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

void CSetupSystemExternalHardware::OnSubFunction2Button() 
{
		//setup system ejectors
	//Create dialog box
  CNewConfigureSystemEjectorDialog INewConfigureEjectorsDialog;  
  INewConfigureEjectorsDialog.vMainWindowPointer = vMainWindowPointer;

	//Set dialog box data like how many products there are
	INewConfigureEjectorsDialog.vLocalConfigurationData = vLocalConfigurationData;
	INewConfigureEjectorsDialog.vLocalSystemData = vLocalSystemData;
		//Pass control to dialog box and display
  int nResponse = INewConfigureEjectorsDialog.DoModal();
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

void CSetupSystemExternalHardware::OnSubFunction3Button() 
{
	// TODO: Add your control notification handler code here
	if ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode)) //Not Continuous Feed, and Not a Pipeline
	{
		//vBackupDetectionUsingBodyTriggerTimeOut
		CNumericEntryDialog INumericEntryDialog;  
		
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vBackupDetectionUsingBodyTriggerTimeOut & 0x7F,0);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Timeout to detect backups using Container Trigger";
		INumericEntryDialog.m_DialogTitleStaticText2 = "If Trigger block for this number of seconds, Inspection Stops";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Setup System";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

		INumericEntryDialog.m_UnitsString = "Default: 3, 0=Disabled, Units in Seconds";
		INumericEntryDialog.vMaxValue = 60;
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vBackupDetectionUsingBodyTriggerTimeOut = _wtoi(INumericEntryDialog.vEditString);

			CYesNoDialog TempYesNoDialog;
			TempYesNoDialog.vNoticeText = _T("\nStop Inspecting (and Stop Conveyor)\nwhen a backup is detected!");
			TempYesNoDialog.vYesButtonText = "Stop Inspecting";
			TempYesNoDialog.vNoButtonText = "Keep Inspecting";
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
			{
				vBackupDetectionUsingBodyTriggerTimeOut = vBackupDetectionUsingBodyTriggerTimeOut | 0x80;  //set msbit to indicate stop inspecting and conveyor on backup
			}
			UpdateButtons();
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
}

void CSetupSystemExternalHardware::OnSubFunction4Button() 
{
	// TODO: Add your control notification handler code here
	if ((vLocalConfigurationData->vScanTracType == cAllegroScanTrac) || (vLocalConfigurationData->vScanTracType == cBriosoScanTrac) || (vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || 
		(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || 
		(vLocalConfigurationData->vScanTracType == cLegatoScanTrac))
	{
		if (vUseExternalInterlock)
			vUseExternalInterlock = 0;
		else
			vUseExternalInterlock = 1;

		if (vUseExternalInterlock)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\nWire the External Interlock through a relay for protection\nand into J6 on the uC Board (Coolant Flow Input).";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}

		UpdateButtons();
		SetChangeMade();
	}
}

void CSetupSystemExternalHardware::OnSubFunction5Button() 
{
	//SETUP CONVEYOR
	 //System Product Settings
	//Create dialog box
	CSystemValuesDialog ISystemValuesDialog;  
	//Set dialog box data like how many products there are
	ISystemValuesDialog.vLocalConfigurationData = vLocalConfigurationData;
	ISystemValuesDialog.vLocalSystemData = vLocalSystemData;
	ISystemValuesDialog.vLocalProductCollection = vLocalProductCollection;
	ISystemValuesDialog.vMainWindowPointer = vMainWindowPointer;

	//Pass control to dialog box and display
	int nResponse = ISystemValuesDialog.DoModal();
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

void CSetupSystemExternalHardware::OnSubFunction6Button() 
{
	//setup Auxiliary Detectors Button Pressed
  //Create dialog box
  CNewConfigureSystemExternalDetectorDialog IConfigureSystemBodyTriggersDialog;  
	//Set dialog box data like how many products there are
	IConfigureSystemBodyTriggersDialog.vWindowType = cExternalDetectorWindow;
	IConfigureSystemBodyTriggersDialog.vLocalConfigurationData = vLocalConfigurationData;
	IConfigureSystemBodyTriggersDialog.vLocalSystemData = vLocalSystemData;
	IConfigureSystemBodyTriggersDialog.vMainWindowPointer = vMainWindowPointer;

  //Pass control to dialog box and display
  int nResponse = IConfigureSystemBodyTriggersDialog.DoModal();
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

void CSetupSystemExternalHardware::OnSubFunction7Button() 
{
		CNumericEntryDialog INumericEntryDialog;  
	//	if (vRFIDEASBadgeReaderCommPort < 10)
			INumericEntryDialog.vEditString = dtoa(vRFIDEASBadgeReaderCommPort);
	//	else
	//		INumericEntryDialog.vEditString = "0";
		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the Comm Port number";
		INumericEntryDialog.m_DialogTitleStaticText2 = "The Badge Reader is plugged into.";

		INumericEntryDialog.m_DialogTitleStaticText3 = "0 = Disabled";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Default: Com5 if no elevator, Com7 if have elevator, ";
		INumericEntryDialog.m_UnitsString = "unless Guard Master in system, then add one.";

		INumericEntryDialog.vMaxValue = 9; //(DWORD MAX)
		INumericEntryDialog.vMinValue = 0;
		int nResponse = INumericEntryDialog.DoModal();
		if (nResponse == IDOK)
		{
			vRFIDEASBadgeReaderCommPort = _wtoi(INumericEntryDialog.vEditString);
			UpdateButtons();
			SetChangeMade();
		}
		else if (nResponse == 10)
			CDialog::EndDialog(10);
}

void CSetupSystemExternalHardware::OnSubFunction8Button() 
{
	// TODO: Add your control notification handler code here
	if (vUseDownStreamSignalThatStopsConveyor == cDontUseDownStream)
		vUseDownStreamSignalThatStopsConveyor = cUseDownStream;
	else
		vUseDownStreamSignalThatStopsConveyor = cDontUseDownStream;
	
	UpdateButtons();
	SetChangeMade();
}

HBRUSH CSetupSystemExternalHardware::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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


	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5); 
		if (vLocalSystemData->vRadiationDoorManualControlRegister)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vGreenBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Display, 5);
		if ((vBackupDetectionUsingBodyTriggerTimeOut & 0x7F) != 3)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Display, 5);
	//if (vHideNeedDetectorCal)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	return vLocalSystemData->vYellowBrush;
 // }
	

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Display, 5);
		if (vUseExternalInterlock)
		{
			pDC->SetAttribDC(pDC->m_hDC);
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vYellowBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Display, 5);
		if (vUseDownStreamSignalThatStopsConveyor == cUseDownStream)
		{
			pDC->SetAttribDC(pDC->m_hDC);
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vYellowBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
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
	if (pWnd->GetDlgCtrlID() == IDC_Function5Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Function5Display, 5);
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

BOOL CSetupSystemExternalHardware::PreTranslateMessage(MSG* pMsg) 
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
				return true;
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
					cShowHelp, 12, 0);
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

void CSetupSystemExternalHardware::UpdateButtons()
{
	{
		SetDlgItemTextW(IDC_SubFunction7Button, _T("Set Badge Reader Comm Port"));
		if (vRFIDEASBadgeReaderCommPort == 0)
				SetDlgItemTextW(IDC_SubFunction7Display, _T("Badge Reader Disabled")); 
		else
		{
				CString TempString = "Badge Reader Comm Port Number: " + dtoa(vRFIDEASBadgeReaderCommPort);
				SetDlgItemTextW(IDC_SubFunction7Display, TempString);
		}
	}	
	if ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode)) //Not Continuous Feed, and Not a Pipeline
	{
		m_SubFunction3Display.ShowWindow(SW_SHOW);
		m_SubFunction3Button.ShowWindow(SW_SHOW);
	}
	else
	{
		m_SubFunction3Display.ShowWindow(SW_HIDE);
		m_SubFunction3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
	}
	
	SetDlgItemText(IDC_SubFunction3Button, _T("Backup Detection Timeout"));
	if (vBackupDetectionUsingBodyTriggerTimeOut & 0x7F)
	{
		if (vBackupDetectionUsingBodyTriggerTimeOut & 0x80)
			SetDlgItemText(IDC_SubFunction3Display, "Backup Timeout: " + dtoa(vBackupDetectionUsingBodyTriggerTimeOut & 0x7F, 0) + " seconds then Stop Inspecting");
		else
			SetDlgItemText(IDC_SubFunction3Display, "Backup Timeout: " + dtoa(vBackupDetectionUsingBodyTriggerTimeOut & 0x7F, 0) + " seconds and Keep Inspecting");
	}
	else
	{
		SetDlgItemText(IDC_SubFunction3Display, _T("Backup Detection Disabled"));
	}
	if (vEnableOperationsGuardian)
	{
		SetDlgItemText(IDC_Function5Button, _T("Disable Operations Guardian"));
		SetDlgItemText(IDC_Function5Display, "Operations Guardian Enabled: " + vOperationsGuardianServer);
	}
	else
	{
		SetDlgItemText(IDC_Function5Button, _T("Enable Operations Guardian"));
		SetDlgItemText(IDC_Function5Display, _T("Operations Guardian Disabled"));
	}
	//if (vHideNeedDetectorCal)
	//{
	//	SetDlgItemText(IDC_SubFunction7Display, "Hiding \"Need Detector Cal\" Red Status Indicator");
	//	SetDlgItemText(IDC_SubFunction7Button, "Show Need Cal Detector");
	//}
	//else
	//{
	//	SetDlgItemText(IDC_SubFunction7Display, "Showing \"Need Detector Cal\" Red Status Indicator");
	//	SetDlgItemText(IDC_SubFunction7Button, "Hide Need Detector\nCal");
	//}

	if (vUseExternalInterlock)
	{
		SetDlgItemText(IDC_SubFunction4Display, _T("Using External Interlock"));
		SetDlgItemText(IDC_SubFunction4Button, _T("Don't Use External Interlock"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction4Display, _T("Not Using External Interlock"));
		SetDlgItemText(IDC_SubFunction4Button, _T("Use External Interlock"));
	}
	
	if (vUseDownStreamSignalThatStopsConveyor == cUseDownStream)
	{
		SetDlgItemText(IDC_SubFunction8Display, _T("Using Down Stream Signal that stops conveyor"));
		SetDlgItemText(IDC_SubFunction8Button, _T("Don't Use Down Stream"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction8Display, _T("Not Using Down Stream Signal that stops conveyor"));
		SetDlgItemText(IDC_SubFunction8Button, _T("Use Down Stream\nSignal"));
	}
}

void CSetupSystemExternalHardware::SetChangeMade() 
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



void CSetupSystemExternalHardware::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
