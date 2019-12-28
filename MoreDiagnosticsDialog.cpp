//ScanTrac Side View Source File
// MoreDiagnosticsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "MoreDiagnosticsDialog.h"
#include "NumericEntryDialog.h"
#include "SelectItemDialog.h"
#include "NoticeDialog.h"
#include "SerialPort.h"
#include "YesNoDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMoreDiagnosticsDialog dialog
extern bool vGlobalFPGAVersion11Point0OrHigher;
extern BYTE vGlobalFrameGrabberType;
extern bool vGlobalFPGAVersion10Point87OrHigher;
extern BYTE vGlobalShiftKeyDown;
extern 	CSerialPort *vGlobaluCSerialPort;
extern CProduct *vGlobalCurrentProduct;
extern bool vGlobalFPGAVersion10Point0OrHigher;
extern unsigned long vGlobalDialogTitleColor;
extern int vGlobalPasswordLevel;
extern unsigned long vGlobalDialogTitleColor;

CMoreDiagnosticsDialog::CMoreDiagnosticsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMoreDiagnosticsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMoreDiagnosticsDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	vMakeExitButtonGreen = false;
	vDisableEjectorsWhenExit = false;
	vMainWindowPointer = NULL;
	vOldFPGAMajorVersionNumber = 0;
	vOldFPGAMinorVersionNumber = 0;
	vDisplayTimerHandle = 77;
	vLastRawInterlockInput = 0;
	vOldActualEncoderRate = 0;
	vLastRawExternalDetectorInput = 0;
	vLastRawBodyTriggerInput = 0;
	vOldCurrentBeltPosition = 0;
	vWaitingForTestEjectNextContainer = false;
	vOldXRayOnNotice = false;
	vChangeMade = false;
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Function3Display);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_XRaysOn2);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_ActualEncoderRate);
	vLocalCWndCollection.Add(&m_ActualEncoderRateLabel);
	vLocalCWndCollection.Add(&m_BeltPositionLabelStaticText);
	vLocalCWndCollection.Add(&m_ContainerSimulation);
	vLocalCWndCollection.Add(&m_ContainersInQueue);
	vLocalCWndCollection.Add(&m_ContainersInQueueLabel);
	vLocalCWndCollection.Add(&m_CurrentBeltPositionStaticText);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_EncoderSimulation);
	vLocalCWndCollection.Add(&m_EncoderSimulationLabel);
	vLocalCWndCollection.Add(&m_ExternalDetectorsForcedEnabled);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_ImageDataType);
	vLocalCWndCollection.Add(&m_ImageShowTestPatterns);
	vLocalCWndCollection.Add(&m_RawBodyTriggerInput);
	vLocalCWndCollection.Add(&m_RawBodyTriggerInputLabel);
	vLocalCWndCollection.Add(&m_RawExternalDetectorInput);
	vLocalCWndCollection.Add(&m_RawExternalDetectorInputLabel);
	vLocalCWndCollection.Add(&m_RawInterlockInput);
	vLocalCWndCollection.Add(&m_RawInterlockInputLabel);
	vLocalCWndCollection.Add(&m_ShowContainerTriggerLength);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_UnitsLabel);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
}

void CMoreDiagnosticsDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);
	vSubFunction1ButtonEnable = true;
	SetDlgItemText(IDC_UnitsLabel, _T(""));
	SetDlgItemText(IDC_RawExternalDetectorInput, _T(""));
	SetDlgItemText(IDC_RawExternalDetectorInputLabel, _T(""));
	SetDlgItemText(IDC_RawBodyTriggerInputLabel, _T(""));
	SetDlgItemText(IDC_RawBodyTriggerInput, _T(""));
	SetDlgItemText(IDC_RawInterlockInputLabel, _T(""));
	SetDlgItemText(IDC_RawInterlockInput, _T(""));
	SetDlgItemText(IDC_ContainersInQueueLabel, _T(""));
	SetDlgItemText(IDC_ContainersInQueue, _T(""));
	SetDlgItemText(IDC_BeltPositionLabelStaticText, _T(""));
	SetDlgItemText(IDC_CurrentBeltPositionStaticText, _T(""));
	vNumberOfEjectorsConfigured = 0;
	vOnlyEjectorNumber = 0;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
	{
		if ((vLocalConfigurationData->vEjector[TempLoop].vEnabled) &&
			(vLocalConfigurationData->vEjector[TempLoop].vLineBitMask))
		{
			vNumberOfEjectorsConfigured++;
			vOnlyEjectorNumber = TempLoop;
		}
	}

	if ((!vNumberOfEjectorsConfigured) || 
		(vLocalSystemData->vSystemRunMode != cRunningSystemMode))
	{
		vSubFunction1ButtonEnable = false;	//m_SubFunction1Button.EnableWindow(false);
	}

#ifdef UseTestModeCode
	SetDlgItemText(IDC_ContainersInQueueLabel,"Cont. In Queue:");
	SetDlgItemText(IDC_ContainersInQueue,"0");
	SetDlgItemText(IDC_BeltPositionLabelStaticText,"Current Conveyor Position:");
	SetDlgItemText(IDC_CurrentBeltPositionStaticText,"0");
	SetDlgItemText(IDC_RawExternalDetectorInput,"0");
	SetDlgItemText(IDC_RawExternalDetectorInputLabel,"Raw Auxiliary Detector Input:");
	SetDlgItemText(IDC_RawBodyTriggerInputLabel,"Raw Container Trigger Input:");
	SetDlgItemText(IDC_RawBodyTriggerInput,"0");
	SetDlgItemText(IDC_RawInterlockInputLabel,"Raw Interlock Input:");
	SetDlgItemText(IDC_RawInterlockInput,"0");
#endif

	SetDlgItemText(IDC_EncoderSimulation,dtoa(vLocalSystemData->vFPGASimulateEncoderRate,0));
	vShowOnlyRealImages = vLocalSystemData->vITIPCDig->vShowOnlyRealImages;

	if (vLocalSystemData->vSystemRunMode == cMeasureSourceStrengthMode)
	{
		SetDlgItemText(IDC_Function3Button, _T("Abort Measure!"));
		vOldMeasureSourceStrength = true;
	}
	else
	{
		vOldMeasureSourceStrength = false;
		SetDlgItemText(IDC_Function3Button, _T("Measure Source!"));
	}

	if (vLocalSystemData->vInAutoSetup)
	{
		vMakeExitButtonGreen = true;
		m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_Function1Button, _T("Return to Auto Setup"));
	}
	else
	{
		CWnd *TempWindow = NULL;
		TempWindow = FindWindow(NULL, _T("Help"));
		if (TempWindow) 
		{
			vMakeExitButtonGreen = true;
			m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_Function1Button, _T("Exit Diagnostics"));
		}
	}
/*
	if (vGlobalFrameGrabberType == cPC2CamLinkFrameGrabber)
	if (vLocalSystemData->vITIPCDig->vCaptureModule)
	{
		m_Function3Button.ShowWindow(SW_SHOW);
		m_Function3Display.ShowWindow(SW_SHOW);
		m_ArrowDown3.ShowWindow(SW_SHOW);
	}
	*/

	DisplayContainerRate();
	UpdateButtons();
	UpdateDisplay();

	int TempTimerResult = SetTimer(vDisplayTimerHandle,500,NULL);
	if (!TempTimerResult)
		ReportErrorMessage("Error-Display Timer Failed",cEMailInspx,32000);
	this->SetWindowText(_T("Tests"));
	::SetupMenu(vLocalCWndCollection);
}

void CMoreDiagnosticsDialog::DisplayContainerRate()
{
	SetDlgItemText(IDC_ContainerSimulation,dtoa(vLocalSystemData->vSimulateContainersRate,0) +
		" CPM");
}

void CMoreDiagnosticsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMoreDiagnosticsDialog)
	DDX_Control(pDX, IDC_Function3Display, m_Function3Display);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_XRaysOn2, m_XRaysOn2);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ActualEncoderRate, m_ActualEncoderRate);
	DDX_Control(pDX, IDC_ActualEncoderRateLabel, m_ActualEncoderRateLabel);
	DDX_Control(pDX, IDC_BeltPositionLabelStaticText, m_BeltPositionLabelStaticText);
	DDX_Control(pDX, IDC_ContainerSimulation, m_ContainerSimulation);
	DDX_Control(pDX, IDC_ContainersInQueue, m_ContainersInQueue);
	DDX_Control(pDX, IDC_ContainersInQueueLabel, m_ContainersInQueueLabel);
	DDX_Control(pDX, IDC_CurrentBeltPositionStaticText, m_CurrentBeltPositionStaticText);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_EncoderSimulation, m_EncoderSimulation);
	DDX_Control(pDX, IDC_EncoderSimulationLabel, m_EncoderSimulationLabel);
	DDX_Control(pDX, IDC_ExternalDetectorsForcedEnabled, m_ExternalDetectorsForcedEnabled);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_ImageDataType, m_ImageDataType);
	DDX_Control(pDX, IDC_ImageShowTestPatterns, m_ImageShowTestPatterns);
	DDX_Control(pDX, IDC_RawBodyTriggerInput, m_RawBodyTriggerInput);
	DDX_Control(pDX, IDC_RawBodyTriggerInputLabel, m_RawBodyTriggerInputLabel);
	DDX_Control(pDX, IDC_RawExternalDetectorInput, m_RawExternalDetectorInput);
	DDX_Control(pDX, IDC_RawExternalDetectorInputLabel, m_RawExternalDetectorInputLabel);
	DDX_Control(pDX, IDC_RawInterlockInput, m_RawInterlockInput);
	DDX_Control(pDX, IDC_RawInterlockInputLabel, m_RawInterlockInputLabel);
	DDX_Control(pDX, IDC_ShowContainerTriggerLength, m_ShowContainerTriggerLength);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_UnitsLabel, m_UnitsLabel);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
}


BEGIN_MESSAGE_MAP(CMoreDiagnosticsDialog, CDialog)
	//{{AFX_MSG_MAP(CMoreDiagnosticsDialog)
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
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CMoreDiagnosticsDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMoreDiagnosticsDialog message handlers

void CMoreDiagnosticsDialog::OnFunction1Button() 
{
	// Main Menu Button was pressed
	PrepareAndExit(10);
}

void CMoreDiagnosticsDialog::OnFunction2Button() 
{
	// Save and Exit Button was pressed
	if ((!vChangeMade) || (KeepSetupChanges()))
	{
		vLocalSystemData->vITIPCDig->vShowOnlyRealImages = vShowOnlyRealImages;
		vLocalConfigurationData->vShowOnlyRealImages = vShowOnlyRealImages;
		PrepareAndExit(true);
	}
}

void CMoreDiagnosticsDialog::OnFunction3Button() 
{
}

void CMoreDiagnosticsDialog::OnFunction4Button() 
{
	//Cancel was pressed
	if (vChangeMade)
	{
		vLocalSystemData->vTestEjectNextContainer = 0;
		vLocalSystemData->vEject4InARowCount = 0;
		CDialog::OnCancel();
	}
}

void CMoreDiagnosticsDialog::OnFunction5Button() 
{
	CSelectItemDialog ISelectItemDialog;  
	
	//Set dialog box data titles and number value
	ISelectItemDialog.vTitleString = "Select Type of Test Error Message";
	ISelectItemDialog.vNumberOfItems = 8;
	ISelectItemDialog.vBackgroundColor = cBlue;

	ISelectItemDialog.vItems[0] = "Test Error Message";
	ISelectItemDialog.vItems[1] = "Test Wrong Size Message";
	ISelectItemDialog.vItems[2] = "Test Error Message to Plant Operations";
	ISelectItemDialog.vItems[3] = "Test Error Message to Maintenance";
	ISelectItemDialog.vItems[4] = "Test Error Message to Inspx";
	ISelectItemDialog.vItems[5] = "Test Reject Message";
	ISelectItemDialog.vItems[6] = "Test Action Message";
	ISelectItemDialog.vItems[7] = "Test Error Message to Critical";
	ISelectItemDialog.m_DialogTitleStaticText2 = "Generates a test error message to check e-mail";
	ISelectItemDialog.vGreenItem = 1;
	ISelectItemDialog.m_DialogTitleStaticText3 = "";
	//Pass control to dialog box and display
	int nResponse = ISelectItemDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		switch (ISelectItemDialog.vItemNumberSelected)
		{  
			case 1:
					ReportErrorMessage("Test Error Message Error",cError,22);
			break;
			case 2:
					ReportErrorMessage("Test Error Message Wrong Size Notice",cWrongSizeNotice,22);
			break;
			case 3:
					ReportErrorMessage("Test Error Message to Plant Operations",cEMailPlantOperations,22);
			break;
			case 4:
					ReportErrorMessage("Test Error Message to Maintenance",cEMailMaintenance,22);
			break;
			case 5:
					ReportErrorMessage("Test Error Message to Inspx",cEMailInspx,22);
			break;
			case 6:
					ReportErrorMessage("Test Eject Message",cReject,22);
			break;
			case 7:
					ReportErrorMessage("Test Action Message",cAction,22);
			break;
			case 8:
					ReportErrorMessage("Test Error Message to Critical",cCritical,22);
			break;
		}
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		Sleep(1); //is the equivelent of a yeild statement;
		PrepareAndExit(10);
	}
}

void CMoreDiagnosticsDialog::OnSubFunction1Button() 
{
}

void CMoreDiagnosticsDialog::OnSubFunction2Button() 
{
	//Test Fire Ejector Setup
	if (vNumberOfEjectorsConfigured == 0)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nNo Ejectors configured in system.";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	else
	{
		int TempResult = 0;
		if (vNumberOfEjectorsConfigured > 1)
			TempResult = vLocalConfigurationData->SelectEjectors("Test Fire", 
				&vLocalSystemData->vEjectorsToFireNow, vLocalConfigurationData, vLocalSystemData->vInAutoSetup);
		else
		{
			vLocalSystemData->vEjectorsToFireNow = 1 << (vOnlyEjectorNumber);
			TempResult = IDOK;
		}

		if (TempResult == IDOK)
		if (vLocalSystemData->vEjectorsToFireNow)
		{
			for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
			if (TempResult == IDOK)
			if (vLocalSystemData->vEjectorsToFireNow & (1 << (TempLoop)))
			{
				CNumericEntryDialog INumericEntryDialog;  
				INumericEntryDialog.vShowNextOnButton2 = true;
				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Ejector Dwell Time";
				INumericEntryDialog.m_DialogTitleStaticText2 = "for test fire of: " + vLocalConfigurationData->vEjector[TempLoop].vName;
				INumericEntryDialog.m_UnitsString = "Measured in Milli Seconds";
				INumericEntryDialog.vIntegerOnly = true;
				INumericEntryDialog.vAllowNegative = false;
				INumericEntryDialog.vEditString = dtoa(vLocalSystemData->vTestEjectDwellTime[TempLoop], 0);
				INumericEntryDialog.m_DialogTitleStaticText3 = "";
				INumericEntryDialog.m_DialogTitleStaticText4 = "Default Value: 300 MS";
				INumericEntryDialog.vMaxValue = 2000;
				//Pass control to dialog box and display
				TempResult = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (TempResult == IDOK)
				{
					vLocalSystemData->vTestEjectDwellTime[TempLoop] = _wtoi(INumericEntryDialog.vEditString);
				}
			}
		}
	}
}

void CMoreDiagnosticsDialog::OnSubFunction3Button() 
{
	//Test Fire Ejector Now
	if (vNumberOfEjectorsConfigured == 0)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nNo Ejectors configured in system.";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	else
	{
		int TempResult = 0;
		if (vLocalSystemData->vEjectorsToFireNow)
		{
			if (!vLocalConfigurationData->vEnableEjectors)
			{
				vDisableEjectorsWhenExit = true;
				//vGlobaluCSerialPort->EnableDisableEjectors(true);
				//vMainWindowPointer->UpdateDisableEjectorsButton();
				vMainWindowPointer->EnableDisableEjectors(true);
			}
			tSerialCommand TempCommand;
			for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
			if (vLocalSystemData->vEjectorsToFireNow & (1 << (TempLoop)))
			{
				TempCommand[0] = 0x23;

				if (TempLoop > 2)
					TempCommand[1] = 0x91 + ((TempLoop - 3) * 2);  //0x91 gets translated into FPGA address 9800 by micro controller.  0x93 >> 9802, 0x95 >> 0x9804;  Bit zero gets moved to bit 11 by micro controller
				else //ejector 4 = 0x91 (0x9800), Ejector 5 = 0x93 (0x9802), Ejector 6 = 0x95 (0x9804)  format = Address to send Micro Controller (FPGAs Address)
					TempCommand[1] = 0x9E + ((TempLoop) * 2);  //Ejector 1 = 0x9E (0x900E), Ejector 2 = 0xA0 (0xA000), Ejector 3 = 0xA2 (0xA002)

				//ejector dwell for each of 3 ejectors will cause ejectors to fire
				WORD TempWord = (WORD)(vLocalSystemData->vTestEjectDwellTime[TempLoop] * 31.25);
				if (vGlobalFPGAVersion10Point87OrHigher)
					TempWord = (WORD)(vLocalSystemData->vTestEjectDwellTime[TempLoop] * 3.975); // clock 8 times slower

				TempCommand[2] = TempWord >> 8; //300 milliseconds
				TempCommand[3] = (BYTE)TempWord;

				if (vGlobaluCSerialPort)
					vGlobaluCSerialPort->SendSerialCommand(TempCommand);

				CNoticeDialog TempNoticeDialog;
				
				TempNoticeDialog.vNoticeText = "\n\n\nFired Ejector " + dtoa(TempLoop + 1,0) +
					"  for " + dtoa(vLocalSystemData->vTestEjectDwellTime[TempLoop],0) + "mSec.";
				TempNoticeDialog.vType = cNoticeMessage;
				TempNoticeDialog.vAutoClose = vLocalSystemData->vTestEjectDwellTime[TempLoop];
				TempNoticeDialog.DoModal();

				Sleep(cSendCommandSleepTime);
			}

			if (vLocalConfigurationData->vResynchronizeEjectors)
				vMainWindowPointer->SendEjectorDwellTimesToFPGA();
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nNo Ejectors configured to Test Fire.";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}

	/*
	//Display how long body trigger is
	vLocalSystemData->vShowContainerTriggerLength = 
		!vLocalSystemData->vShowContainerTriggerLength;

	tSerialCommand TempCommand;
	TempCommand[0] = 0x33;
	TempCommand[1] = 0x00;
	TempCommand[2] = 0x00;
	if (vLocalSystemData->vShowContainerTriggerLength)
		TempCommand[3] = 3;
	else
		TempCommand[3] = 4;

	UpdateButtons();
	if (vGlobaluCSerialPort)
		vGlobaluCSerialPort->SendSerialCommand(TempCommand);
	*/
}

void CMoreDiagnosticsDialog::OnSubFunction4Button() 
{
	//Keep Auxiliary Detectors enabled
	if ((vLocalConfigurationData->vResynchronizeEjectors) && (vLocalSystemData->vKeepExternalDetectorsEnabled))
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nResynchronizing Ejector Mode requires Auxiliry Detectors\nto always be enabled.";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
	else
	{
		vLocalSystemData->vKeepExternalDetectorsEnabled = !vLocalSystemData->vKeepExternalDetectorsEnabled;
		tSerialCommand TempCommand;
		TempCommand[0] = 0x1C;
		TempCommand[1] = 0x02;  //group 2 is Auxiliary Detectors
		TempCommand[2] = 0x00;
		if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
		{
			if (vLocalSystemData->vKeepExternalDetectorsEnabled)
				TempCommand[3] = vLocalSystemData->vCurrentExternalDetectorMask;
			else
				TempCommand[3] = vLocalSystemData->vCurrentAlwaysOnExternalDetectorMask;
		}
		else
			TempCommand[3] = 0;
		if (vGlobaluCSerialPort)
			vGlobaluCSerialPort->SendSerialCommand(TempCommand);
		TempCommand[0] = 0x33;
		TempCommand[1] = 0x00;
		TempCommand[2] = 0x00;
		if (vLocalSystemData->vKeepExternalDetectorsEnabled)
			TempCommand[3] = 3;
		else
			TempCommand[3] = 4;
		if (vGlobaluCSerialPort)
			vGlobaluCSerialPort->SendSerialCommand(TempCommand);

		UpdateButtons();
	}
}

void CMoreDiagnosticsDialog::OnSubFunction5Button() 
{
	//Simulated Encoder Rate
	//ask user for rate
	// Continuous Feed Encoder Rate
	if (vLocalSystemData->vFPGAVersion9Point0OrHigher)
	{  //uses micro-seconds for simulated encoder
		CNumericEntryDialog INumericEntryDialog;  
		
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = 
			dtoa(vLocalSystemData->vFPGASimulateEncoderRate,0);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Simulated Encoder Rate for testing";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Test Mode cleared if Current Product is selected";
		INumericEntryDialog.m_DialogTitleStaticText3 = "For testing without real conveyor encoder";

		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
			INumericEntryDialog.vEditString + ", Product Maximum: " + dtoa(vLocalSystemData->vMaximumEncoderRate,0);

		INumericEntryDialog.m_UnitsString = "0=Disabled, 2307 Maximum, Units in Hertz";
		INumericEntryDialog.vMaxValue = 2307;
		if (vGlobalFPGAVersion10Point0OrHigher)
			INumericEntryDialog.vMaxValue = 50000;
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vLocalSystemData->vFPGASimulateEncoderRate = _wtoi(INumericEntryDialog.vEditString);
			SetDlgItemText(IDC_EncoderSimulation,dtoa(vLocalSystemData->vFPGASimulateEncoderRate,0));

			if ((vLocalSystemData->vFPGASimulateEncoderRate) ||
				(vLocalConfigurationData->vContinuousFeedEncoderRate))
			{
				if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) ||
					(vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))
					vMainWindowPointer->SendConveyorOnOffToUController(0);
				vMainWindowPointer->TurnSimulatedEncoderOn();
			}
			else
			{
				vMainWindowPointer->TurnSimulatedEncoderOff();
				if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) ||
					(vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))
					vMainWindowPointer->SendConveyorOnOffToUController(1);
			}

			UpdateButtons();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			PrepareAndExit(10);
		}
	}
	else
	{
		CSelectItemDialog ISelectItemDialog;  
		
		//Set dialog box data titles and number value
		ISelectItemDialog.vTitleString = "Simulated Encoder Rate";
		ISelectItemDialog.vNumberOfItems = 8;
		ISelectItemDialog.vBackgroundColor = cGreen;

		ISelectItemDialog.vItems[0] = "Off";
		ISelectItemDialog.vItems[1] = "2307Hz";
		ISelectItemDialog.vItems[2] = "1000Hz";
		ISelectItemDialog.vItems[3] = "500Hz";
		ISelectItemDialog.vItems[4] = "200Hz";
		ISelectItemDialog.vItems[5] = "100Hz";
		ISelectItemDialog.vItems[6] = "50Hz";
		ISelectItemDialog.vItems[7] = "12Hz";
		ISelectItemDialog.m_DialogTitleStaticText2 = "For testing without real conveyor encoder";
		BYTE TempIndex = 7;

		if (vLocalSystemData->vFPGASimulateEncoderRate)
			ISelectItemDialog.vGreenItem = 1;
		ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: " + 
			dtoa(vLocalSystemData->vFPGASimulateEncoderRate,0);
		//Pass control to dialog box and display
		int nResponse = ISelectItemDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			WORD TempSimulatedEncoderRate = 0;  //off; was code 255
			switch (ISelectItemDialog.vItemNumberSelected)
			{  
				case 2:
					TempSimulatedEncoderRate = 2307; //2307 hz; was code 1
				break;
				case 3:
					TempSimulatedEncoderRate = 1000;  //1000 hz; was code 3
				break;
				case 4:
					TempSimulatedEncoderRate = 500; //500 hz; was code 6
				break;
				case 5:
					TempSimulatedEncoderRate = 200; //200 hz; was code 15
				break;
				case 6:
					TempSimulatedEncoderRate = 100; //100 hz; was code 30
				break;
				case 7:
					TempSimulatedEncoderRate = 50; //50 hz; was code 60
				break;
				case 8:
					TempSimulatedEncoderRate = 12; //12 hz; was code 240
				break;
			}
			vLocalSystemData->vFPGASimulateEncoderRate = TempSimulatedEncoderRate;
			SetDlgItemText(IDC_EncoderSimulation,dtoa(vLocalSystemData->vFPGASimulateEncoderRate,0));

			if ((vLocalSystemData->vFPGASimulateEncoderRate) ||
				(vLocalConfigurationData->vContinuousFeedEncoderRate))
			{
				if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) ||
					(vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))
					vMainWindowPointer->SendConveyorOnOffToUController(0);
				vMainWindowPointer->TurnSimulatedEncoderOn();
			}
			else
			{
				vMainWindowPointer->TurnSimulatedEncoderOff();
				if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))
					vMainWindowPointer->SendConveyorOnOffToUController(1);
			}

			UpdateButtons();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			PrepareAndExit(10);
		}
	}
}

void CMoreDiagnosticsDialog::OnSubFunction6Button() 
{
	//Simulate containers button pressed
	if (vGlobalCurrentProduct)
	{
		CNumericEntryDialog INumericEntryDialog;  
		
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vLocalSystemData->vSimulateContainersRate,0);

		WORD TempCurrentEncoderRate = vLocalSystemData->vActualEncoderRate;
		if (!TempCurrentEncoderRate)
			TempCurrentEncoderRate = vLocalSystemData->vFPGASimulateEncoderRate;
		if (!TempCurrentEncoderRate)
			TempCurrentEncoderRate = vLocalConfigurationData->vContinuousFeedContainerRate;

		WORD TempContinuousFeedContainerRate = (WORD)(1.1 * (60.05 * TempCurrentEncoderRate / 
				(vGlobalCurrentProduct->vProductImageWidth * vGlobalPixelsPerUnit)));

		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the Simulated Container Rate to use";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Use MAXIMUM rate so images will overlap so inspect all";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Setup System, Original Value: " + INumericEntryDialog.vEditString;
		INumericEntryDialog.m_DialogTitleStaticText4 = "WARNING: " + 
			dtoa(TempContinuousFeedContainerRate,0) + " is MAXIMUM value for this product setup";

		INumericEntryDialog.m_UnitsString = "0=Disabled, Units in Containers Per Minute";
		INumericEntryDialog.vMaxValue = 5000;
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vLocalSystemData->vSimulateContainersRate = _wtoi(INumericEntryDialog.vEditString);
			if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))
				vMainWindowPointer->TurnSimulatedContainersOn();
			UpdateButtons();
			SetDlgItemText(IDC_ContainerSimulation, dtoa(vLocalSystemData->vSimulateContainersRate,0));
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			PrepareAndExit(10);
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nYou must Select A Product for this function.";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
/*
	//Simulate containers button pressed
	//ask user for rate
	CSelectItemDialog ISelectItemDialog;  
	
	//Set dialog box data titles and number value
	ISelectItemDialog.vTitleString = "Simulated Containers Rate";
	ISelectItemDialog.vNumberOfItems = 8;
	ISelectItemDialog.vBackgroundColor = cGreen;
	if (vLocalSystemData->vSimulateContainersRate)
		ISelectItemDialog.vGreenItem = 1;

	ISelectItemDialog.vItems[0] = "Off";
	ISelectItemDialog.vItems[1] = "60 CPM";
	ISelectItemDialog.vItems[2] = "30 CPM";
	ISelectItemDialog.vItems[3] = "12 CPM";
	ISelectItemDialog.vItems[4] = "120 CPM";
	ISelectItemDialog.vItems[5] = "480 CPM";
	ISelectItemDialog.vItems[6] = "960 CPM";
	ISelectItemDialog.vItems[7] = "1920 CPM";
	ISelectItemDialog.m_DialogTitleStaticText2 = "For testing without real containers";
	BYTE TempIndex = 7;
	if (vLocalSystemData->vSimulateContainersRate > 7)
		vLocalSystemData->vSimulateContainersRate = 0;
	ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: " + 
		ISelectItemDialog.vItems[vLocalSystemData->vSimulateContainersRate];
	//Pass control to dialog box and display
	int nResponse = ISelectItemDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		SetDlgItemText(IDC_ContainerSimulation,
			ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1]);
		if (vLocalSystemData->vSimulateContainersRate != (ISelectItemDialog.vItemNumberSelected - 1))
		{
			
			switch (ISelectItemDialog.vItemNumberSelected)
			{
				case 1: vLocalSystemData->vSimulateContainersRate = 0; break;
				case 2: vLocalSystemData->vSimulateContainersRate = 60; break;
				case 3: vLocalSystemData->vSimulateContainersRate = 30; break;
				case 4: vLocalSystemData->vSimulateContainersRate = 12; break;
				case 5: vLocalSystemData->vSimulateContainersRate = 120; break;
				case 6: vLocalSystemData->vSimulateContainersRate = 480; break;
				case 7: vLocalSystemData->vSimulateContainersRate = 960; break;
				case 8: vLocalSystemData->vSimulateContainersRate = 1920; break;
				default: vLocalSystemData->vSimulateContainersRate = 0; break;
			}
		}
		vMainWindowPointer->TurnSimulatedContainersOn();
		UpdateButtons();
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		Sleep(1); //is the equivelent of a yeild statement;
		PrepareAndExit(10);
	}
	*/
}

//FPGA Simulate Data, then Simulate Software Images, then CamLink
void CMoreDiagnosticsDialog::OnSubFunction7Button() 
{
	if (vLocalSystemData->vFPGAGeneratingSimulatedData)
	{
		//Use real image data
		vMainWindowPointer->TurnOnOffFPGASimulatedData(false);  //disable uC FPGA simulated 8 grey scale images
		ReportErrorMessage("Inspx Disabled uC FPGA Simulating Grey Images", cWriteToLog, 32000);
		vShowOnlyRealImages = false;  //enable software simulated images
		ReportErrorMessage("Inspx Enabled Simulating Software Images", cWriteToLog, 32000);
		SetChangeMade();
	}
	else
	if (!vShowOnlyRealImages)
	{
		vShowOnlyRealImages = true;  //disable software simulated images
		ReportErrorMessage("Inspx Disabled Simulating Software Images", cWriteToLog, 32000);
		SetChangeMade();
		if ((vGlobalFrameGrabberType == cPC2CamLinkFrameGrabber) && (vLocalSystemData->vITIPCDig->vCaptureModule))
			StartOrStopCamLinkSimulation();
	}
	else
	if ((vGlobalFrameGrabberType == cPC2CamLinkFrameGrabber) && (vLocalSystemData->vITIPCDig->vCaptureModule) && (vLocalSystemData->vSimulatingCamLinkImage))
		StartOrStopCamLinkSimulation();
	else
	{
		vMainWindowPointer->TurnOnOffFPGASimulatedData(true);
		ReportErrorMessage("Inspx Enabled uC FPGA Simulating Grey Images", cWriteToLog, 32000);
	}
	UpdateButtons();
	UpdateDisplay();
}

void CMoreDiagnosticsDialog::OnSubFunction8Button() 
{
	/*
	SetChangeMade();
	vShowOnlyRealImages = !vShowOnlyRealImages;
	UpdateButtons();
	*/
}

BOOL CMoreDiagnosticsDialog::PreTranslateMessage(MSG* pMsg) 
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
	if (pMsg->message == cNewImageReadyToDisplay)
	{
		return true;
	}
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
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
			break;
			case 36:  //Home key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
			case cPageDown: // Help
				PostThreadMessage(vGlobalCallingThreadID,
					cShowHelp, 20, 0);
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
			break;
			//9/30/2009
			case cPageUp: // Clear
			{
				if (vGlobalPasswordLevel)
					PasswordOK(cNoPassword, false);
				else
					PasswordOK(cAnyPasswordToEnter, true);

				CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
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

HBRUSH CMoreDiagnosticsDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

	if (pWnd->GetDlgCtrlID() == IDC_Function1Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vMakeExitButtonGreen)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ActualEncoderRateLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ActualEncoderRateLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EncoderSimulationLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EncoderSimulationLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ContainerSimulation)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ContainerSimulation, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
	}
	if (pWnd->GetDlgCtrlID() == IDC_ExternalDetectorsForcedEnabled)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ExternalDetectorsForcedEnabled, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
	}
	if (pWnd->GetDlgCtrlID() == IDC_ImageShowTestPatterns)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ImageShowTestPatterns, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
	}
	if (pWnd->GetDlgCtrlID() == IDC_ImageDataType)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_ImageDataType, 5);	//using function button, not a button but is two lines
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
	}
	if (pWnd->GetDlgCtrlID() == IDC_EncoderSimulation)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EncoderSimulation, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
	}
	if (pWnd->GetDlgCtrlID() == IDC_ActualEncoderRate)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ActualEncoderRate, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		if (vWaitingForTestEjectNextContainer)
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction1ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vLocalSystemData->vRedBrush;
		}
		else
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_XRaysOn2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaysOn2, 7);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cYellow);
		pDC->SetBkColor(cRed);
		return vLocalSystemData->vRedBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)  
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		if ((vLocalSystemData->vKeepExternalDetectorsEnabled) && (!vLocalConfigurationData->vResynchronizeEjectors))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button)  
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		if (vLocalSystemData->vFPGASimulateEncoderRate)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Button)  
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5);  //5 is medium large
		if (vLocalSystemData->vSimulateContainersRate)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button)  
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
		if ((vLocalSystemData->vFPGAGeneratingSimulatedData)	|| (!vShowOnlyRealImages) || (vLocalSystemData->vSimulatingCamLinkImage))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}

	}
	if (pWnd->GetDlgCtrlID() == IDC_ImageDataType)  
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ImageDataType, 5);
		if ((vLocalSystemData->vFPGAGeneratingSimulatedData) || (!vShowOnlyRealImages) || (vLocalSystemData->vSimulatingCamLinkImage))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}
/*
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)  
	if (!vShowOnlyRealImages)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vGreenBrush;
	}
	*/
	/*
	if ()
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
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button) 
	{   
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction1ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}

	return hbr;
}


void CMoreDiagnosticsDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == vDisplayTimerHandle)
  {
		UpdateDisplay();
	}
	CDialog::OnTimer(nIDEvent);
}


void CMoreDiagnosticsDialog::UpdateDisplay()
{
	if (vOldXRayOnNotice != vLocalSystemData->vXRayOnNotice)
	{
		vOldXRayOnNotice = vLocalSystemData->vXRayOnNotice;
		if (vOldXRayOnNotice)
			m_XRaysOn2.ShowWindow(SW_SHOW);
		else
			m_XRaysOn2.ShowWindow(SW_HIDE);
	}
	if (vOldActualEncoderRate != vLocalSystemData->vActualEncoderRate)
	{
		vOldActualEncoderRate = vLocalSystemData->vActualEncoderRate;
		SetDlgItemText(IDC_ActualEncoderRate, dtoa(vOldActualEncoderRate, 0) + "Hz");
	}

	if (vOldMeasureSourceStrength)
	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	{
		SetDlgItemText(IDC_Function3Button, _T("Measure Source!"));
		vOldMeasureSourceStrength = false;
	}

	if (vWaitingForTestEjectNextContainer)
	if (!vLocalSystemData->vTestEjectNextContainer)
	{
		vWaitingForTestEjectNextContainer = false;
		SetDlgItemText(IDC_SubFunction1Button, _T("Test Eject Next Container!"));
	}
}

void CMoreDiagnosticsDialog::SetChangeMade() 
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

void CMoreDiagnosticsDialog::UpdateButtons() 
{
	/*
	if (vLocalSystemData->vShowContainerTriggerLength)
	{
		SetDlgItemText(IDC_SubFunction3Button,"Hide Container Trigger!");
		SetDlgItemText(IDC_ShowContainerTriggerLength,"Showing Container Trigger Length");
	}
	else
	{
		SetDlgItemText(IDC_SubFunction3Button,"Show Container Trigger!");
		SetDlgItemText(IDC_ShowContainerTriggerLength,"Not Showing Container Trigger Length");
	}
	if (vGlobalFrameGrabberType == cPC2CamLinkFrameGrabber)
	if (vLocalSystemData->vITIPCDig->vCaptureModule)
	{
		if (vLocalSystemData->vSimulatingCamLinkImage)
		{
			SetDlgItemText(IDC_Function3Button,"Turn Off Sim CamLink!");
			SetDlgItemText(IDC_Function3Display,"CamLink Board is Simulating Images");
		}
		else
		{
			SetDlgItemText(IDC_Function3Button,"Turn On Sim CamLink!");
			SetDlgItemText(IDC_Function3Display,"CamLink Board is Not Simulating Images");
		}
	}
	*/

	if (vLocalSystemData->vKeepExternalDetectorsEnabled)
	{
		SetDlgItemText(IDC_SubFunction4Button, _T("Auxiliary Detectors Normal!"));
		if (vLocalConfigurationData->vResynchronizeEjectors)
			SetDlgItemText(IDC_ExternalDetectorsForcedEnabled, _T("Auxiliary Detectors are always Enabled"));
		else
			SetDlgItemText(IDC_ExternalDetectorsForcedEnabled, _T("Auxiliary Detectors are Forced Enabled"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction4Button, _T("Force Aux Detectors Enable!"));
		SetDlgItemText(IDC_ExternalDetectorsForcedEnabled, _T("Auxiliary Detectors Operating Normally"));
	}
	/*
		if (vLocalSystemData->vFPGAGeneratingSimulatedData)
	{
		//Use real image data
		vMainWindowPointer->TurnOnOffFPGASimulatedData(false);  //disable uC FPGA simulated 8 grey scale images
		vShowOnlyRealImages = false;  //enable software simulated images
	}
	else
	if (!vShowOnlyRealImages)
	{
		vShowOnlyRealImages = true;  //disable software simulated images
		if ((vGlobalFrameGrabberType == cPC2CamLinkFrameGrabber) && (vLocalSystemData->vITIPCDig->vCaptureModule))
			StartOrStopCamLinkSimulation(true);
	}
	else
	if ((vGlobalFrameGrabberType == cPC2CamLinkFrameGrabber) && (vLocalSystemData->vITIPCDig->vCaptureModule) && (vLocalSystemData->vSimulatingCamLinkImage))
		StartOrStopCamLinkSimulation(false);
	else
		vMainWindowPointer->TurnOnOffFPGASimulatedData(true);
	UpdateButtons();
	if (vLocalSystemData->vFPGAGeneratingSimulatedData)
	{
		SetDlgItemText(IDC_SubFunction7Button,"Disable H/W Test Images!");
		SetDlgItemText(IDC_ImageDataType,"H/W Test Images Enabled");
	}
	else
	{
		SetDlgItemText(IDC_SubFunction7Button,"Enable H/W Test Images!");
		SetDlgItemText(IDC_ImageDataType,"H/W Test Images Disabled");
	}
	if (!vShowOnlyRealImages)
	{
		SetDlgItemText(IDC_SubFunction8Button,"Disable S/W Test Images");
		SetDlgItemText(IDC_ImageShowTestPatterns,"S/W Test Images Enabled");
	}
	else
	{
		SetDlgItemText(IDC_SubFunction8Button,"Enable S/W Test Images");
		SetDlgItemText(IDC_ImageShowTestPatterns,"S/W Test Images Disabled");
	}
*/
	if (vLocalSystemData->vFPGAGeneratingSimulatedData)
	{
		SetDlgItemText(IDC_SubFunction7Button, _T("S/W Test Images"));
		SetDlgItemText(IDC_ImageDataType, _T("uC FPGA Test Images Enabled"));
	}
	else
	if (!vShowOnlyRealImages)
	{
		if ((vGlobalFrameGrabberType == cPC2CamLinkFrameGrabber) && (vLocalSystemData->vITIPCDig->vCaptureModule))
			SetDlgItemText(IDC_SubFunction7Button, _T("\nSimulate CamLink"));
		else
			SetDlgItemText(IDC_SubFunction7Button, _T("Turn Off\nS/W Test\nImages"));
		SetDlgItemText(IDC_ImageDataType, _T("S/W Test Images Enabled"));
	}
	else
	if ((vGlobalFrameGrabberType == cPC2CamLinkFrameGrabber) && (vLocalSystemData->vITIPCDig->vCaptureModule) && (vLocalSystemData->vSimulatingCamLinkImage))
	{
		SetDlgItemText(IDC_SubFunction7Button, _T("Turn Off Sim CamLink!"));
		SetDlgItemText(IDC_ImageDataType, _T("CamLink Board is Simulating Images"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction7Button, _T("uC FPGA\nTest\nImages!"));
		SetDlgItemText(IDC_ImageDataType, _T("Real Images"));
	}

	if (vLocalSystemData->vTestEjectNextContainer)
		SetDlgItemText(IDC_SubFunction1Button, _T("Cancel Test Eject Container!"));
	else
		SetDlgItemText(IDC_SubFunction1Button, _T("Test Eject Next Container!"));

		SetDlgItemText(IDC_SubFunction5Button, _T("Simulated Encoder Rate!"));
		SetDlgItemText(IDC_SubFunction6Button, _T("Simulated Container Rate!"));
}

void CMoreDiagnosticsDialog::PrepareAndExit(int TempExitCode) 
{
	vLocalSystemData->vTestEjectNextContainer = 0;
	vLocalSystemData->vEject4InARowCount = 0;

	if (vDisableEjectorsWhenExit)
	{
		vMainWindowPointer->EnableDisableEjectors(false);
	}
	CDialog::EndDialog(TempExitCode);
}

void CMoreDiagnosticsDialog::LoadTestCamLinkProduct()
{
	vMainWindowPointer->MakeProductionReportIfNeeded();
	vMainWindowPointer->ResetCounters();
	vLocalSystemData->vDensityIndex = 0;
	//look for product "~CamLink Test 384 x 256"
	CProduct *TempProduct = vMainWindowPointer->GetProductByName("~CamLink Test 384 x 256");
	//if don't find a calibrate product, create and save it, then use it
	if (!TempProduct)
	if (ThereIsEnoughMemory(sizeof(CProduct), "New CamLink Product "))
	{
		TempProduct = new CProduct;
		if (TempProduct)
		{
			TempProduct->SetProductName("~CamLink Test 384 x 256");

			TempProduct->vProductImageHeightTop = (float)(vLocalConfigurationData->vDetectorLength * vLocalConfigurationData->vNumberOfXRayDetectors);
			//if (vLocalConfigurationData->vUseMetric)
			//	TempProduct->vProductImageHeightTop = (WORD)(TempProduct->vProductImageHeightTop * 25.4);

			double TempDelay = 2;
			//if (vLocalConfigurationData->vUseMetric)
			//	TempDelay = TempDelay * 25.4;
			TempProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)TempDelay);
			TempProduct->SetProductImageWidth((float)(256.0 / vGlobalPixelsPerUnit));
			TempProduct->SetProductLockOutWidth((float)TempDelay);


			TempProduct->vEndOfLineTimeOut = (float)(16);
			//if (vLocalConfigurationData->vUseMetric)
			//	TempProduct->vEndOfLineTimeOut = (float)(16 * 25.4);
			TempProduct->SetEndOfLineTimeOut(TempProduct->vEndOfLineTimeOut);

			TempProduct->vEdgeLocationBottom = (float)(.5);
			TempProduct->SetEdgeLocationBottom(TempProduct->vEdgeLocationBottom);
			TempProduct->vEdgeLocationHeight = (float)(1);
			TempProduct->SetEdgeLocationHeight(TempProduct->vEdgeLocationHeight);

			if ((vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac)) //50 watt ScanTrac with MNX
			{
				TempProduct->vXRaySourceVoltage = (float)(20. / 6.0);  //20 kv
				TempProduct->vXRaySourceCurrent = (float)1;
			}
			else //600 watt ScanTrac
			{
				TempProduct->vXRaySourceVoltage = (float)40 / 6;
				TempProduct->vXRaySourceCurrent = (float)1.1;
			}

			if (vLocalConfigurationData->vMaximumCurrent < TempProduct->vXRaySourceCurrent)
				TempProduct->vXRaySourceCurrent = (float)vLocalConfigurationData->vMaximumCurrent;

			if (vLocalConfigurationData->vMaximumVoltage < TempProduct->vXRaySourceVoltage)
				TempProduct->vXRaySourceVoltage = (float)vLocalConfigurationData->vMaximumVoltage;

			TempProduct->vXRayIntegrationTime = 750; //.3 ms
		//	TempProduct->vXRayIntegrationTimeAtEdge = TempProduct->vXRayIntegrationTime;
			TempProduct->vBodyTrigger = 1;
			TempProduct->SetEjectorBeltPositionOffset(0,(float)40);
			//if (vLocalConfigurationData->vUseMetric)
			//	TempProduct->SetEjectorBeltPositionOffset(0,(float)(40 * 25.4));
			TempProduct->vNotes = "Auto Product for Testing CamLink";
			TempProduct->CalculateEndOfLineTimeOut();
			vMainWindowPointer->AddProduct(((CProduct *)TempProduct));
		}
	}
	CString TempProductName = *TempProduct->GetProductName();
	vMainWindowPointer->SetupProduct(TempProduct, false);
}


void CMoreDiagnosticsDialog::StartRunningCamLinkProduct()
{
	if (vMainWindowPointer->PrepareToRun())
	{
		if (vLocalConfigurationData->vEnableEjectors)
		{
			vMainWindowPointer->OnSubFunction2Button();

			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\nEjectors have been Disabled\nfor Run Diagnostic Mode.";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}

		vMainWindowPointer->StartRunning(true, true);
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		CString TempText("\n\n\nX-Rays not on, Check Key Switch, E-Stop, Interlocks");
		if (PasswordOK(cSuperInspxPassword,false))
			TempText = TempText + "\nInterlocks State: " + WordToHex(vLocalSystemData->vSourceInterLocks);
		TempNoticeDialog.vNoticeText = TempText;
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
}

void CMoreDiagnosticsDialog::StartOrStopCamLinkSimulation()
{	//send pattern to camlink board to turn on or off simulated images from LVDS to CAMLink board
	if ((vGlobalFrameGrabberType == cPC2CamLinkFrameGrabber))
	if (vLocalSystemData->vITIPCDig->vCaptureModule)
	{
		if (!vLocalSystemData->vSimulatingCamLinkImage)
		{  
			CYesNoDialog TempYesNoDialog;
			TempYesNoDialog.vNoticeText =  _T("\n\n\nDo you want to Simulate CamLink Images?");
			TempYesNoDialog.vYesButtonText = "Simulate CamLink";
			TempYesNoDialog.vNoButtonText = "Don't Simulate";
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
			{
				/*
				bool TempOKToLoadProduct = false;
				if ((vLocalSystemData->vSystemRunMode == cStoppedSystemMode) || ((vGlobalCurrentProduct) && (*vGlobalCurrentProduct->GetProductName() == "~CamLink Test 384 x 256")))
					TempOKToLoadProduct = true;
				else
				{
					CYesNoDialog TempYesNoDialog;
					TempYesNoDialog.vNoticeText = "\n\nSystem is running.\nStop running and Load CamLink Test Image Product?";
					TempYesNoDialog.vYesButtonText = "Load CamLink";
					TempYesNoDialog.vNoButtonText = "Cancel";
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult == IDOK)
						TempOKToLoadProduct = true;
				}

				if (TempOKToLoadProduct)
				*/
				{
					if ((!vGlobalCurrentProduct) || (*vGlobalCurrentProduct->GetProductName() != "~CamLink Test 384 x 256"))
					{
						if (vGlobalCurrentProduct)
							vLocalSystemData->vPreviousProduct = *vGlobalCurrentProduct->GetProductName();

						if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
							vMainWindowPointer->StopRunning(true);

						LoadTestCamLinkProduct();
					}

					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\nScanTrac will return to the Main Menu.\nCamLink Test Images should display 3 test patterns:\n1. Black Diamond. 2. White Diamond. 3. Grey Scale.\n Simulating containers at 30 CPM\nSimulating encoder at 300 Hz.";
					TempNoticeDialog.vType = cNoticeMessage;
					TempNoticeDialog.DoModal();

					StartRunningCamLinkProduct();

					//turn cam link simulation images ON
					vMainWindowPointer->TurnOnOffCamLinkTestImages(true);

					//if ((vLocalSystemData->vFPGASimulateEncoderRate != 300) || (vLocalSystemData->vSimulateContainersRate != 30))
					{
						//setup simulated encoder and turn on
						vLocalSystemData->vFPGASimulateEncoderRate = 300;

						if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) ||
							(vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))
							vMainWindowPointer->SendConveyorOnOffToUController(0);
						vMainWindowPointer->TurnSimulatedEncoderOn();
						if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) ||
							(vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))
							vMainWindowPointer->SendConveyorOnOffToUController(1);

						//setup simulated containers and turn on
						vLocalSystemData->vSimulateContainersRate = 30;
						vMainWindowPointer->TurnSimulatedContainersOn();
					}
					vLocalSystemData->vITIPCDig->vShowOnlyRealImages = vShowOnlyRealImages;
					vLocalConfigurationData->vShowOnlyRealImages = vShowOnlyRealImages;
					OnFunction1Button();
				}
			}
		}
		else
		{  //turn cam link simulation images OFF
			vMainWindowPointer->StopRunning(true);
		}
		UpdateButtons();
	}
}

void CMoreDiagnosticsDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}