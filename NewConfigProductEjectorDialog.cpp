//ScanTrac Side View Source File
// NewConfigProductEjectorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "NewConfigProductEjectorDialog.h"
#include "NumericEntryDialog.h"
#include "SerialPort.h"
#include "NewDiagnosticDialog.h"
#include "HelpDialog.h"
#include "PasswordEntryDialog.h"
#include "YesNoDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CProduct *vGlobalCurrentProduct;
extern bool vGlobalFPGAVersion16point7orAbove;
extern BYTE vGlobalEndOfLineTimeOutMargin;
extern bool vGlobalFPGAVersion10Point87OrHigher;
extern BYTE vGlobalShiftKeyDown;
extern CSerialPort *vGlobaluCSerialPort;
extern CProduct *vGlobalCurrentProduct;
extern int vGlobalPasswordLevel;
extern unsigned long vGlobalDialogTitleColor;

/////////////////////////////////////////////////////////////////////////////
// CNewConfigProductEjectorDialog dialog


CNewConfigProductEjectorDialog::CNewConfigProductEjectorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNewConfigProductEjectorDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewConfigProductEjectorDialog)
	//}}AFX_DATA_INIT
	vNumberOfEjectorsInSystem = 0;
	vFirstEjectorShown = 0;
	vShowPreviousAndNextButtons = false;
	vStopRunningWhenExit = false;
	vDisableEjectorsWhenExit = false;
	vOldErrorMessage = "None";
	vOldButton3Yellow = false;
	vMainWindowPointer = NULL;
	vChangeMade = false;
	vEjectorNumberEditing = 0;
	vDisplayTimerHandle = 56019;
	vStartSetupTimerHandle = 56020;
	vStart16XEncoderTimerHandle = 56021;
	vWaitingForTestEjectNextContainer = false;
	vTestingEject2ndContainer = false;
	vOldXRayOnNotice = false;
	vOldSourceRampedUp = false;
	vLocalProductCollection = NULL;
	vMinimumEjectorDistance = 20;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
		vEjectorUsed[TempLoop] = false;
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction8Display);
	vLocalCWndCollection.Add(&m_DwellTime2);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_XRaysOn);
	vLocalCWndCollection.Add(&m_Calibrating);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_EjectorName3);
	vLocalCWndCollection.Add(&m_EjectorName2);
	vLocalCWndCollection.Add(&m_EjectorName1);
	vLocalCWndCollection.Add(&m_DwellTime3);
	vLocalCWndCollection.Add(&m_DwellTime1);
	vLocalCWndCollection.Add(&m_DelayPosition3);
	vLocalCWndCollection.Add(&m_DelayPosition2);
	vLocalCWndCollection.Add(&m_DelayPosition1);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_DistanceUnitsLabel);
	vLocalCWndCollection.Add(&m_DistanceUnitsLabel2);
	vLocalCWndCollection.Add(&m_ErrorLine);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_Title1);
	vLocalCWndCollection.Add(&m_Title2);
	vLocalCWndCollection.Add(&m_Title4);
	vLocalCWndCollection.Add(&m_Title5);
}

void CNewConfigProductEjectorDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	if ((!vGlobalCurrentProduct) || (!vLocalConfigurationData) || (!vLocalSystemData) || (!vMainWindowPointer) || (!vLocalProductCollection))
	{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "Pointer Error, Can not open Ejector Timing Menu";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();

			CDialog::EndDialog(10);
	}
		
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);
	vCurrentEjectorName = vLocalConfigurationData->vEjector[0].vName;

	vFunction2ButtonEnable = true;
	vFunction5ButtonEnable = true;

	vSubFunction1ButtonEnable = true;
	vSubFunction2ButtonEnable = true;
	vSubFunction3ButtonEnable = true;
	vSubFunction4ButtonEnable = true;
	vSubFunction5ButtonEnable = true;

	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
	if (vLocalConfigurationData->vEjector[TempLoop].vEnabled)
		vNumberOfEjectorsInSystem++;

	if (vNumberOfEjectorsInSystem > 3)
	{
		vShowNextButton = true;
		m_Function1Button.ShowWindow(SW_SHOW);
	}

	if (vLocalSystemData->vXRayOnNotice)
	{
		vOldXRayOnNotice = vLocalSystemData->vXRayOnNotice;
		vOldSourceRampedUp = vLocalSystemData->vSourceRampedUp;
		m_XRaysOn.ShowWindow(SW_SHOW);
	}

	m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	m_SubFunction5Button.ShowWindow(SW_SHOW);

	m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	m_SubFunction1Button.ShowWindow(SW_SHOW);

	if (vLocalSystemData->vInAutoSetup)
	{
		SetDlgItemText(IDC_DialogTitleStaticText1, _T("Auto Setup Product Ejectors"));

		if (!vLocalConfigurationData->vEnableEjectors)
		{
			vDisableEjectorsWhenExit = true;
			//vLocalConfigurationData->vEnableEjectors = true;
			//vGlobaluCSerialPort->EnableDisableEjectors(vLocalConfigurationData->vEnableEjectors);
			//vMainWindowPointer->UpdateDisableEjectorsButton();
			vMainWindowPointer->EnableDisableEjectors(true);
		}

		ReportErrorMessage("Auto Setup Ejectors", cWriteToLog,0);
	}
	else
	{
		SetDlgItemText(IDC_DialogTitleStaticText1, _T("Setup Product Ejectors"));

	
		ReportErrorMessage("Setup Ejectors", cWriteToLog,0);
	}
	UpdateButtons();
	UpdateFunction3Button();
	if (vShowPreviousAndNextButtons)
	{
		m_Function4Button.ShowWindow(SW_SHOW);
		m_Function4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_Function4Button, _T("Previous Auto Setup"));
	
		m_Function5Button.ShowWindow(SW_SHOW);
		m_Function5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_Function5Button, _T("Next Auto Setup"));
	}
	else
	{
		m_Function2Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_Function2Button, _T("Exit"));
	}

  SetDlgItemText(IDC_ErrorLine, _T(""));
	CString TempText("");
	TempText.LoadString(IDS_XRAYON);
	SetDlgItemText(IDC_XRaysOn,TempText);
	if (vGlobalCurrentProduct)
	{
		double TempMargin = 0.0;
		//if (vLocalConfigurationData->vUseMetric)
		//{
		//	TempMargin = TempMargin * 25.4;
		//	if (vGlobalCurrentProduct->vProductImageWidth + 4 * 25.4 > TempMargin)
		//		TempMargin = vGlobalCurrentProduct->vProductImageWidth + 4 * 25.4;
		//}
		//else
		if (vGlobalCurrentProduct->vProductImageWidth + 1 > TempMargin)
			TempMargin = vGlobalCurrentProduct->vProductImageWidth + 1;

		if ((vGlobalCurrentProduct->vBulkProductMode) || (vGlobalCurrentProduct->vMultiLaneNumberOfLanes) ||
			(vLocalSystemData->vSimulateContainersRate) || (vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode))
			vMinimumEjectorDistance = 1 + TempMargin;
		else
			vMinimumEjectorDistance = vGlobalCurrentProduct->vProductBodyTriggerToImageBeltPositionOffset + TempMargin;
	}
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
	{
		vEjectorDelayPosition[TempLoop] = vGlobalCurrentProduct->vEjectorDelayPosition[TempLoop];
		vEjectorResponseTime[TempLoop] = vGlobalCurrentProduct->vEjectorResponseTime[TempLoop];
		vDwellTime[TempLoop] = dtoa(vGlobalCurrentProduct->vEjectorDwellTime[TempLoop],2);
		vOriginalDelayPosition[TempLoop] = dtoa(vGlobalCurrentProduct->vEjectorDelayPosition[TempLoop],2);
		vOriginalEjectorResponseTime[TempLoop] = vGlobalCurrentProduct->vEjectorResponseTime[TempLoop];
		vOriginalDwellTime[TempLoop] = dtoa(vGlobalCurrentProduct->vEjectorDwellTime[TempLoop],2);

		if (vGlobalCurrentProduct->vEjectorDelayPosition[TempLoop] + .02 < vMinimumEjectorDistance)
		{
			vEjectorDelayPosition[TempLoop] = vMinimumEjectorDistance;
			vGlobalCurrentProduct->SetEjectorBeltPositionOffset(TempLoop, (float)vEjectorDelayPosition[vEjectorNumberEditing]);
			vGlobalCurrentProduct->CalculateEndOfLineTimeOut();

			if (vEjectorDelayPosition[TempLoop] + vGlobalEndOfLineTimeOutMargin > vGlobalCurrentProduct->vEndOfLineTimeOut)
				vGlobalCurrentProduct->SetEndOfLineTimeOut((float)vEjectorDelayPosition[TempLoop] + vGlobalEndOfLineTimeOutMargin);

			SetChangeMade();
			if (vLocalConfigurationData->vEjector[TempLoop].vEnabled)
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nEjector: " + dtoa(TempLoop + 1,0) + " Position is shorter than minimum.\nSetting to minimum value of: " + dtoa(vMinimumEjectorDistance,2);
				TempNoticeDialog.vType = cNoticeMessage;
				TempNoticeDialog.DoModal();
			}
		}
	}
	vProductName = vGlobalCurrentProduct->GetProductName();
	SetDlgItemText(IDC_Title5,"Product: " + *vProductName);

	if (!vLocalConfigurationData->vEjector[0].vEnabled)
	{
		m_DwellTime1.ShowWindow(SW_HIDE);
		m_DelayPosition1.ShowWindow(SW_HIDE);
		m_EjectorName1.ShowWindow(SW_HIDE);
		m_SubFunction2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
	}
	if (!vLocalConfigurationData->vEjector[1].vEnabled)
	{
		m_DwellTime2.ShowWindow(SW_HIDE);
		m_DelayPosition2.ShowWindow(SW_HIDE);
		m_EjectorName2.ShowWindow(SW_HIDE);
		m_SubFunction3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
	}
	if (!vLocalConfigurationData->vEjector[2].vEnabled)
	{
		m_DwellTime3.ShowWindow(SW_HIDE);
		m_DelayPosition3.ShowWindow(SW_HIDE);
		m_EjectorName3.ShowWindow(SW_HIDE);
		m_SubFunction4Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
	}

	for (BYTE TempLoop = 0; TempLoop < vGlobalCurrentProduct->vNumberOfInspections; TempLoop++)
	{
		if (vGlobalCurrentProduct->vInspection[TempLoop])
		if (vGlobalCurrentProduct->vInspection[TempLoop]->vEjector)
		{
			for (BYTE TempEjectorLoop = 0; TempEjectorLoop < cNumberOfEjectors; TempEjectorLoop++)
			if (vGlobalCurrentProduct->vInspection[TempLoop]->vEjector & (1 << TempEjectorLoop))
			{
				vEjectorUsed[TempEjectorLoop] = true;
				if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
				for (BYTE TempLaneLoop = 1; TempLaneLoop < vGlobalCurrentProduct->vMultiLaneNumberOfLanes; TempLaneLoop++)
				if (TempEjectorLoop + TempLaneLoop < cNumberOfEjectors)
					vEjectorUsed[TempEjectorLoop + TempLaneLoop] = true;
			}
		}
	}

	//look for shuttleworth second ejector for pulse to extend  and mark used
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
	{
		if (vDwellTime[TempLoop] == "0")
		if (TempLoop < 2)
		{
			vEjectorUsed[TempLoop + 1] = true;
		}
	}

	for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
	{
		if (vGlobalCurrentProduct->vExternalDetectorEnable[TempLoop])
		if (vGlobalCurrentProduct->vExternalDetectorMode[TempLoop])
		if (vGlobalCurrentProduct->vExternalDetectorEjector[TempLoop])
		{
			for (BYTE TempEjectorLoop = 0; TempEjectorLoop < cNumberOfEjectors; TempEjectorLoop++)
			if (vGlobalCurrentProduct->vExternalDetectorEjector[TempLoop] & (1 << TempEjectorLoop))
			{
				vEjectorUsed[TempEjectorLoop] = true;
			}
		}
	}

	for (BYTE TempLoop = cFillerSamplingType; TempLoop <= cSeamerSamplingType; TempLoop++)
	if (vLocalConfigurationData->vNumberOfHeadsToMonitor[TempLoop])
	if (vGlobalCurrentProduct->vSamplingEjector[TempLoop])
	{
		for (BYTE TempEjectorLoop = 0; TempEjectorLoop < cNumberOfEjectors; TempEjectorLoop++)
		if (vGlobalCurrentProduct->vSamplingEjector[TempLoop] & (1 << TempEjectorLoop))
			vEjectorUsed[TempEjectorLoop] = true;
	}

	int TempTimerResult = SetTimer(vDisplayTimerHandle,500,NULL);
	if (!TempTimerResult)
		ReportErrorMessage("Error-Display Timer Failed",cEMailInspx, 32000);
	this->SetWindowText(_T("Ejectors"));
	if (!vLocalSystemData->vXRayOnNotice)
	{
		int TimerResult = SetTimer(vStartSetupTimerHandle,20,NULL);
		if (!TimerResult)
			ReportErrorMessage("Error-Start Timer Failed",cEMailInspx, 32000);
	}

	UpdateDisplay();

	::SetupMenu(vLocalCWndCollection);

	//if (((vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]) || (vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType]) || 
	//	(vLocalConfigurationData->vNumberOfHeadsToMonitor[cRandomSamplingType])) && (vGlobalFPGAVersion16point7orAbove))
	{
		SetDlgItemText(IDC_SubFunction7Button, _T("Dwell Time or Diverter"));
	}
}

void CNewConfigProductEjectorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewConfigProductEjectorDialog)
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction8Display, m_SubFunction8Display);
	DDX_Control(pDX, IDC_DwellTime2, m_DwellTime2);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_XRaysOn, m_XRaysOn);
	DDX_Control(pDX, IDC_Calibrating, m_Calibrating);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_EjectorName3, m_EjectorName3);
	DDX_Control(pDX, IDC_EjectorName2, m_EjectorName2);
	DDX_Control(pDX, IDC_EjectorName1, m_EjectorName1);
	DDX_Control(pDX, IDC_DwellTime3, m_DwellTime3);
	DDX_Control(pDX, IDC_DwellTime1, m_DwellTime1);
	DDX_Control(pDX, IDC_DelayPosition3, m_DelayPosition3);
	DDX_Control(pDX, IDC_DelayPosition2, m_DelayPosition2);
	DDX_Control(pDX, IDC_DelayPosition1, m_DelayPosition1);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_DistanceUnitsLabel, m_DistanceUnitsLabel);
	DDX_Control(pDX, IDC_DistanceUnitsLabel2, m_DistanceUnitsLabel2);
	DDX_Control(pDX, IDC_ErrorLine, m_ErrorLine);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_Title1, m_Title1);
	DDX_Control(pDX, IDC_Title2, m_Title2);
	DDX_Control(pDX, IDC_Title4, m_Title4);
	DDX_Control(pDX, IDC_Title5, m_Title5);
}


BEGIN_MESSAGE_MAP(CNewConfigProductEjectorDialog, CDialog)
	//{{AFX_MSG_MAP(CNewConfigProductEjectorDialog)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
	ON_BN_CLICKED(IDC_Function5Button, OnFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction2Button, OnSubFunction2Button)
	ON_BN_CLICKED(IDC_SubFunction3Button, OnSubFunction3Button)
	ON_BN_CLICKED(IDC_SubFunction4Button, OnSubFunction4Button)
	ON_BN_CLICKED(IDC_SubFunction6Button, OnSubFunction6Button)
	ON_BN_CLICKED(IDC_SubFunction7Button, OnSubFunction7Button)
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SubFunction1Button, OnSubFunction1Button)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SubFunction5Button, OnSubFunction5Button)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CNewConfigProductEjectorDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewConfigProductEjectorDialog message handlers

void CNewConfigProductEjectorDialog::OnFunction1Button() 
{
	/*
	//Main Menu was pressed
	if(AbortSetupChanges())	
	{
		PrepareToExitDialog();
		CDialog::EndDialog(10);
	}
	*/
	if (vNumberOfEjectorsInSystem > 3)
	{
		vFirstEjectorShown = vFirstEjectorShown + 3;
		if (vFirstEjectorShown >= vNumberOfEjectorsInSystem)
			vFirstEjectorShown = 0;

		vEjectorNumberEditing = 0;

		UpdateDisplay();
		UpdateButtons();
	}
}

void CNewConfigProductEjectorDialog::OnFunction2Button() 
{
	if ((!vShowPreviousAndNextButtons) && (vFunction2ButtonEnable == true))
		SaveAndExit(true);
	else
	{
		if (vFunction2ButtonEnable == true)
			TestEject4InARow();
		else
			ShowNoMouseClickDialogMessage();
	}
}

void CNewConfigProductEjectorDialog::SaveAndExit(int TempReturnCode) 
{
	if (TempReturnCode == 1)
	{
		ReportErrorMessage("Save and Exit from Edit Ejector Timing Menu", cUserAction,0);
		for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
		{
			vGlobalCurrentProduct->SetEjectorBeltPositionOffset(TempLoop, (float)vEjectorDelayPosition[TempLoop]);
			vGlobalCurrentProduct->vEjectorResponseTime[TempLoop] = vEjectorResponseTime[TempLoop];
			vGlobalCurrentProduct->vEjectorDwellTime[TempLoop] = ((float)ATOF(vDwellTime[TempLoop]));
		}
	}
	else
	{
		ReportErrorMessage("Abort from Edit Ejector Timing Menu", cUserAction,0);
		for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
		{
			vGlobalCurrentProduct->SetEjectorBeltPositionOffset(TempLoop, (float)ATOF(vOriginalDelayPosition[TempLoop]));
			vGlobalCurrentProduct->vEjectorResponseTime[TempLoop] = vOriginalEjectorResponseTime[TempLoop];
			vGlobalCurrentProduct->vEjectorDwellTime[TempLoop] = ((float)ATOF(vOriginalDwellTime[TempLoop]));
		}
	}
	vGlobalCurrentProduct->CalculateEndOfLineTimeOut();
	PrepareToExitDialog();
	CDialog::EndDialog(TempReturnCode);
}

void CNewConfigProductEjectorDialog::OnFunction3Button() 
{
	// TODO: Add your control notification handler code here
	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	{
		if (vMainWindowPointer->PrepareToRun())
		{
			if ((vGlobalCurrentProduct != vGlobalCurrentProduct) || (!vGlobalCurrentProduct))
			{
				vMainWindowPointer->SetupProduct(vGlobalCurrentProduct, false);//turn source off
				Sleep(1000);
			}
			//dont turn source on
			vMainWindowPointer->StartRunning(false, false);
			vLocalSystemData->vSystemRunMode = cAutoSetupRunningSystem;

			if ((vLocalSystemData->vFPGASimulateEncoderRate) || (vLocalConfigurationData->vContinuousFeedEncoderRate))
				vMainWindowPointer->TurnSimulatedEncoderOn();
			else
			if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac))
			if (!vLocalConfigurationData->vUseRealEncoder) //only use if using the motor signal
			{
				vLocalSystemData->vRealConveyorEncoderPulsesCount = vLocalSystemData->vConveyorEncoderRate;
				vLocalSystemData->vSecondsMeasuringRealConveyorEncoder = 1;
				vMainWindowPointer->SetSimulatedEncoderTo16TimesConveyor();
			}

			UpdateButtons();
		}
	}
	else
	if (vLocalSystemData->vYellowMessageButtonYellow)
		HandleErrorButton();
}

void CNewConfigProductEjectorDialog::OnFunction4Button() 
{
	if (vShowPreviousAndNextButtons)
	//Save and exit was pressed
		SaveAndExit(true);
	else
		//cancel was pressed
	if (vChangeMade)
		SaveAndExit(false);
}

void CNewConfigProductEjectorDialog::OnFunction5Button() 
{
	//Save and exit was pressed
	if ((vShowPreviousAndNextButtons) && (vFunction5ButtonEnable == true))
		SaveAndExit(true);
	else
	{
		if (vFunction5ButtonEnable == true)
			TestEject4InARow();
		else
			ShowNoMouseClickDialogMessage();
	}
}

void CNewConfigProductEjectorDialog::OnSubFunction1Button() 
{
	if ((vLocalSystemData->vTestEjectNextContainer) && (!vLocalSystemData->vEject4InARowCount) && (!vTestingEject2ndContainer))
	{
		vWaitingForTestEjectNextContainer = false;
		vLocalSystemData->vTestEjectNextContainer = 0;
		UpdateDisplay();
		UpdateButtons();
	}
	else
		TestEjectNextContainer(0);
}

void CNewConfigProductEjectorDialog::OnSubFunction2Button() 
{
	//Select Ejector 1 button pressed
	if (vLocalConfigurationData->vEjector[0	+ vFirstEjectorShown].vEnabled)
	{
		if (vEjectorNumberEditing == 0)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nThis ejector already selected to edit.";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			vEjectorNumberEditing = 0;

			if (!vEjectorUsed[vEjectorNumberEditing	+ vFirstEjectorShown])
				GiveEjectorNotUsedMessage();
			//else
			//if (!vLocalSystemData->vTestEjectNextContainer)
			//	vLocalSystemData->vTestEjectNextContainer = 1 << vEjectorNumberEditing;

			UpdateDisplay();
			UpdateButtons();
		}
	}
}

void CNewConfigProductEjectorDialog::OnSubFunction3Button() 
{
	//Select Ejector 2 button pressed
	if (1	+ vFirstEjectorShown < vNumberOfEjectorsInSystem)
	if (vLocalConfigurationData->vEjector[1].vEnabled)
	{
		if (vEjectorNumberEditing == 1)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nThis ejector already selected to edit.";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			vEjectorNumberEditing = 1;

			if (!vEjectorUsed[vEjectorNumberEditing	+ vFirstEjectorShown])
				GiveEjectorNotUsedMessage();
			//else
			//if (!vLocalSystemData->vTestEjectNextContainer)
			//	vLocalSystemData->vTestEjectNextContainer = 1 << vEjectorNumberEditing;

			UpdateDisplay();
			UpdateButtons();
		}
	}
}

void CNewConfigProductEjectorDialog::OnSubFunction4Button() 
{
	//Select Ejector 3 button pressed
	if (2	+ vFirstEjectorShown < vNumberOfEjectorsInSystem)
	if (vLocalConfigurationData->vEjector[2].vEnabled)
	{
		if (vEjectorNumberEditing == 2)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nThis ejector already selected to edit.";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			vEjectorNumberEditing = 2;

			if (!vEjectorUsed[vEjectorNumberEditing	+ vFirstEjectorShown])
				GiveEjectorNotUsedMessage();
			//else
			//if (!vLocalSystemData->vTestEjectNextContainer)
			//	vLocalSystemData->vTestEjectNextContainer = 1 << vEjectorNumberEditing;

			UpdateDisplay();
			UpdateButtons();
		}
	}
}

void CNewConfigProductEjectorDialog::OnSubFunction5Button() 
{
	if (vTestingEject2ndContainer)
	{
		vWaitingForTestEjectNextContainer = false;
		vTestingEject2ndContainer = false;
		vLocalSystemData->vTestEjectNextContainer = 0;
		vLocalSystemData->vTestEjectWaitCount = 0;
		UpdateButtons();
	}
	else
	{
		TestEjectNextContainer(1);
		vTestingEject2ndContainer = true;
		UpdateButtons();
	}
}

void CNewConfigProductEjectorDialog::OnSubFunction6Button() 
{
	//Edit Distance From Container Trigger Button Was Pressed
	CNumericEntryDialog INumericEntryDialog;  
	
	//Set dialog box data titles and number value

	if (vLocalConfigurationData->vSystemEjectorDistance[vEjectorNumberEditing	+ vFirstEjectorShown])
		INumericEntryDialog.vEditString = dtoa(vEjectorDelayPosition[vEjectorNumberEditing	+ vFirstEjectorShown] - 
		vLocalConfigurationData->vSystemEjectorDistance[vEjectorNumberEditing	+ vFirstEjectorShown] - vLocalConfigurationData->vSystemEjectorPositionOffset, 2);
	else
	if (vLocalConfigurationData->vSystemEjectorPositionOffset)
		INumericEntryDialog.vEditString = dtoa(vEjectorDelayPosition[vEjectorNumberEditing	+ vFirstEjectorShown] - vLocalConfigurationData->vSystemEjectorPositionOffset, 2);
	else
		INumericEntryDialog.vEditString = dtoa(vEjectorDelayPosition[vEjectorNumberEditing	+ vFirstEjectorShown], 2);

	if (vLocalConfigurationData->vSystemEjectorDistance[vEjectorNumberEditing	+ vFirstEjectorShown])
		INumericEntryDialog.m_DialogTitleStaticText1 = "Ejector position OFFSET (Normally Zero)";
	else
	if (vLocalConfigurationData->vSystemEjectorPositionOffset)
		INumericEntryDialog.m_DialogTitleStaticText1 = "Distance from ScanTrac Output Side Wall to Ejector";
	else
		INumericEntryDialog.m_DialogTitleStaticText1 = "Distance from Optical Trigger to Ejector";

	INumericEntryDialog.m_DialogTitleStaticText2 = "Ejector: " + vCurrentEjectorName;
	INumericEntryDialog.m_DialogTitleStaticText3 = "Product: " + *vProductName;
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
	INumericEntryDialog.m_UnitsString = "Measured in " + vLocalConfigurationData->vUnitsString;
	INumericEntryDialog.vMaxValue = 500; //cMaxBeltPositionOffset / vGlobalPixelsPerUnit;

	if (vLocalConfigurationData->vSystemEjectorDistance[vEjectorNumberEditing	+ vFirstEjectorShown])
	{
		if (vMinimumEjectorDistance < vLocalConfigurationData->vSystemEjectorDistance[vEjectorNumberEditing	+ vFirstEjectorShown] + vLocalConfigurationData->vSystemEjectorPositionOffset)
			INumericEntryDialog.vMinValue = vMinimumEjectorDistance - vLocalConfigurationData->vSystemEjectorDistance[vEjectorNumberEditing	+ vFirstEjectorShown] - vLocalConfigurationData->vSystemEjectorPositionOffset;
		else
			INumericEntryDialog.vMinValue = 0;
	}
	else
	if (vLocalConfigurationData->vSystemEjectorPositionOffset)
	{
		if (vMinimumEjectorDistance < vLocalConfigurationData->vSystemEjectorPositionOffset)
			INumericEntryDialog.vMinValue = vMinimumEjectorDistance - vLocalConfigurationData->vSystemEjectorPositionOffset;
		else
			INumericEntryDialog.vMinValue = 0;
	}
	else
		INumericEntryDialog.vMinValue = vMinimumEjectorDistance;

		// + vGlobalCurrentProduct->vProductImageWidth
	INumericEntryDialog.vIntegerOnly = false;
	if (vLocalConfigurationData->vSystemEjectorDistance[vEjectorNumberEditing	+ vFirstEjectorShown])
	{
		INumericEntryDialog.vAllowNegative = true;
		INumericEntryDialog.vWarnIfLargerThan4 = true;
	}

	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		if (vLocalConfigurationData->vSystemEjectorDistance[vEjectorNumberEditing	+ vFirstEjectorShown])
			vEjectorDelayPosition[vEjectorNumberEditing	+ vFirstEjectorShown] = (float)(ATOF(INumericEntryDialog.vEditString) + 
			vLocalConfigurationData->vSystemEjectorDistance[vEjectorNumberEditing	+ vFirstEjectorShown] + vLocalConfigurationData->vSystemEjectorPositionOffset);
		else
		if (vLocalConfigurationData->vSystemEjectorPositionOffset)
			vEjectorDelayPosition[vEjectorNumberEditing	+ vFirstEjectorShown] = (float)(ATOF(INumericEntryDialog.vEditString) + vLocalConfigurationData->vSystemEjectorPositionOffset);
		else
			vEjectorDelayPosition[vEjectorNumberEditing	+ vFirstEjectorShown] = (float)(ATOF(INumericEntryDialog.vEditString));

		if (vEjectorResponseTime[vEjectorNumberEditing	+ vFirstEjectorShown] > .09)
			vEjectorResponseTime[vEjectorNumberEditing	+ vFirstEjectorShown] = (float).004;

		vGlobalCurrentProduct->SetEjectorBeltPositionOffset(vEjectorNumberEditing	+ vFirstEjectorShown, (float)vEjectorDelayPosition[vEjectorNumberEditing	+ vFirstEjectorShown]);

		SetChangeMade();
		UpdateDisplay();

		if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
		{
			CYesNoDialog TempYesNoDialog;
			TempYesNoDialog.vNoticeText = _T("\n\nMulti Lane Product\n\nWould you like this eject position for all ejectors?");
			TempYesNoDialog.vYesButtonText = "Set In All Ejectors";
			TempYesNoDialog.vNoButtonText = "Just this ejector";
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
			{
				for (BYTE TempLoop = 1; TempLoop < vGlobalCurrentProduct->vMultiLaneNumberOfLanes; TempLoop++)
				{
					vEjectorDelayPosition[vEjectorNumberEditing	+ vFirstEjectorShown + TempLoop] = (float)vEjectorDelayPosition[vEjectorNumberEditing	+ vFirstEjectorShown];
					vGlobalCurrentProduct->SetEjectorBeltPositionOffset(vEjectorNumberEditing	+ vFirstEjectorShown + TempLoop, (float)vEjectorDelayPosition[vEjectorNumberEditing	+ vFirstEjectorShown]);
				}

				UpdateDisplay();
			}
		}

		vGlobalCurrentProduct->CalculateEndOfLineTimeOut();

		if (vEjectorDelayPosition[vEjectorNumberEditing	+ vFirstEjectorShown] + vGlobalEndOfLineTimeOutMargin > vGlobalCurrentProduct->vEndOfLineTimeOut)
			vGlobalCurrentProduct->SetEndOfLineTimeOut((float)(vEjectorDelayPosition[vEjectorNumberEditing	+ vFirstEjectorShown] + vGlobalEndOfLineTimeOutMargin));

		if (!vLocalSystemData->vEject4InARowCount)
		//if (vLocalConfigurationData->vEnableEjectors)
		if (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem)
		if (!vLocalSystemData->vTestEjectNextContainer)
		{
			vWaitingForTestEjectNextContainer = true;
			vLocalSystemData->vTestEjectNextContainer = 1 << (vEjectorNumberEditing	+ vFirstEjectorShown);
			UpdateButtons();
		}
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		PrepareToExitDialog();
		CDialog::EndDialog(10);
	}
}

void CNewConfigProductEjectorDialog::OnSubFunction7Button() 
{
	//Edit Dwell Time Button Was Pressed
	CNumericEntryDialog INumericEntryDialog;  
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = vDwellTime[vEjectorNumberEditing	+ vFirstEjectorShown];

	INumericEntryDialog.m_DialogTitleStaticText1 = "Ejector Dwell Time";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Ejector: " + vCurrentEjectorName;
	INumericEntryDialog.m_DialogTitleStaticText3 = "Product: " + *vProductName;

	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + vDwellTime[vEjectorNumberEditing	+ vFirstEjectorShown] + " Default: 35";

	//if (((vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]) || (vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType]) || 
	//	(vLocalConfigurationData->vNumberOfHeadsToMonitor[cRandomSamplingType])) && (vGlobalFPGAVersion16point7orAbove))
	//{
		INumericEntryDialog.m_UnitsString = "Time in ms, 0 = Diverter";
		INumericEntryDialog.vAllowZero = true;
	//}
	//else
	//{
	//	INumericEntryDialog.m_UnitsString = "Time in ms";
	//	INumericEntryDialog.vAllowZero = false;
	//}

	INumericEntryDialog.vMaxValue = 2000;

	if (vGlobalFPGAVersion10Point87OrHigher)
		INumericEntryDialog.vMaxValue = 16000;

	INumericEntryDialog.vIntegerOnly = false;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vDwellTime[vEjectorNumberEditing	+ vFirstEjectorShown] = INumericEntryDialog.vEditString;

		SetChangeMade();
		UpdateDisplay();
		//Send Ejector Dwell Conveyor Postion
		tSerialCommand TempCommand;
		TempCommand[0] = 0x4E;
		TempCommand[1] = vEjectorNumberEditing	+ vFirstEjectorShown + 1;
		//must multiply user entry of milliseconds by 31.25 as need to count 31.25Kilohertz
		//clock signals.
		WORD TempWord = (WORD)(ATOF(vDwellTime[vEjectorNumberEditing	+ vFirstEjectorShown]) * 31.25);
		if (vGlobalFPGAVersion10Point87OrHigher)
			TempWord = (WORD)(ATOF(vDwellTime[vEjectorNumberEditing	+ vFirstEjectorShown]) * 3.975); // clock 8 times slower

		TempCommand[2] = (BYTE)(TempWord >> 8);
		TempCommand[3] = (BYTE)TempWord;
		if (vGlobaluCSerialPort)
			vGlobaluCSerialPort->SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTime);

		if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
		{
			CYesNoDialog TempYesNoDialog;
			TempYesNoDialog.vNoticeText = _T("\n\nMulti Lane Product\n\nWould you like this eject dwell for all ejectors?");
			TempYesNoDialog.vYesButtonText = "Set In All Ejectors";
			TempYesNoDialog.vNoButtonText = "Just this ejector";
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
			{
				for (BYTE TempLoop = 1; TempLoop < vGlobalCurrentProduct->vMultiLaneNumberOfLanes; TempLoop++)
				{
					vDwellTime[vEjectorNumberEditing	+ vFirstEjectorShown + TempLoop] = vDwellTime[vEjectorNumberEditing	+ vFirstEjectorShown];

					//Send Ejector Dwell Conveyor Postion
					tSerialCommand TempCommand;
					TempCommand[0] = 0x4E;
					TempCommand[1] = vEjectorNumberEditing	+ vFirstEjectorShown + 1 + TempLoop;
					//must multiply user entry of milliseconds by 31.25 as need to count 31.25Kilohertz
					//clock signals.
					WORD TempWord = (WORD)(ATOF(vDwellTime[vEjectorNumberEditing	+ vFirstEjectorShown + TempLoop]) * 31.25);
					if (vGlobalFPGAVersion10Point87OrHigher)
						TempWord = (WORD)(ATOF(vDwellTime[vEjectorNumberEditing	+ vFirstEjectorShown + TempLoop]) * 3.975); // clock 8 times slower

					TempCommand[2] = (BYTE)(TempWord >> 8);
					TempCommand[3] = (BYTE)TempWord;
					if (vGlobaluCSerialPort)
						vGlobaluCSerialPort->SendSerialCommand(TempCommand);
					Sleep(cSendCommandSleepTime);
				}
				UpdateDisplay();
			}
		}

		if (!vLocalSystemData->vEject4InARowCount)
		//if (vLocalConfigurationData->vEnableEjectors)
		if (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem)
		if (!vLocalSystemData->vTestEjectNextContainer)
		{
			vWaitingForTestEjectNextContainer = true;
			vLocalSystemData->vTestEjectNextContainer = 1 << (vEjectorNumberEditing	+ vFirstEjectorShown);
			UpdateButtons();
		}
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		PrepareToExitDialog();
		CDialog::EndDialog(10);
	}
}

void CNewConfigProductEjectorDialog::OnSubFunction8Button() 
{
	
	//Edit Response Factor Button Was Pressed
	CNumericEntryDialog INumericEntryDialog;  
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vEjectorResponseTime[vEjectorNumberEditing	+ vFirstEjectorShown],4);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Conveyor Speed Correction";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Amount to trigger ejector: " + vCurrentEjectorName + " early";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Product: " + *vProductName;
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
		INumericEntryDialog.vEditString + " Default: .004";

	INumericEntryDialog.m_UnitsString = "Value is multiplied by encoder pulses per second";
	INumericEntryDialog.vMaxValue = .1;
	INumericEntryDialog.vIntegerOnly = false;
	INumericEntryDialog.vAllowNegative = true;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vEjectorResponseTime[vEjectorNumberEditing	+ vFirstEjectorShown] = (float)ATOF(INumericEntryDialog.vEditString);

		SetChangeMade();
		UpdateDisplay();
		vGlobalCurrentProduct->vEjectorResponseTime[vEjectorNumberEditing	+ vFirstEjectorShown] = vEjectorResponseTime[vEjectorNumberEditing	+ vFirstEjectorShown];

		if (!vLocalSystemData->vEject4InARowCount)
		//if (vLocalConfigurationData->vEnableEjectors)
		if (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem)
		if (!vLocalSystemData->vTestEjectNextContainer)
		{
			vWaitingForTestEjectNextContainer = true;
			vLocalSystemData->vTestEjectNextContainer = 1 << (vEjectorNumberEditing	+ vFirstEjectorShown);
			UpdateButtons();
		}
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		PrepareToExitDialog();
		CDialog::EndDialog(10);
	}
	
}

BOOL CNewConfigProductEjectorDialog::PreTranslateMessage(MSG* pMsg) 
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
	if (pMsg->message == cXRaysOnMessage)
	{
		if (pMsg->lParam)
		{
			CString TempText("");
			TempText.LoadString(IDS_XRAY);
			SetDlgItemText(IDC_Calibrating,TempText + " " + dtoa(pMsg->lParam, 0));
			m_Calibrating.ShowWindow(SW_SHOW);
		}
		else
		{
			switch (pMsg->wParam)
			{
				case 0:
				{
					m_Calibrating.ShowWindow(SW_HIDE);
				}
				break;
				case 24:
				{
					CString TempText("");
					if (vLocalSystemData->vXRayOnNotice)
					{
						TempText.LoadString(IDS_XRAYON);
					}
					else
					{
						TempText.LoadString(IDS_XRAY);
						TempText = TempText + " **";
					}
					SetDlgItemText(IDC_Calibrating,TempText);
					m_Calibrating.ShowWindow(SW_SHOW);
				}
				break;
				case 25:
				{
					CString TempText("");
					if (vLocalSystemData->vXRayOnNotice)
					{
						TempText.LoadString(IDS_XRAYON);
					}
					else
					{
						TempText.LoadString(IDS_XRAY);
						TempText = TempText + " **";
					}
					SetDlgItemText(IDC_Calibrating,TempText);
					m_Calibrating.ShowWindow(SW_SHOW);
				}
				break;
			}
		}
		UpdateFunction3Button();
		return true;
	}
	if (pMsg->message == WM_KEYDOWN)
	{
		CWnd * TempWindow = NULL;
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
					cShowHelp, 7, 0);
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
			break;
			//case c1: 
			//	vMainWindowPointer->StopRunning(true);
			//break;
			case cPageUp: // Clear, but enter System setup password
			{
				if (vGlobalPasswordLevel)
				{
					PasswordOK(cNoPassword,false);
					UpdateButtons();
				}
				else
				{
					PasswordOK(cAnyPasswordToEnter, true); //allow any password to be entered
					if (PasswordOK(cTemporaryInspxPassword, false))
					{
						UpdateButtons();
					}
				}
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

HBRUSH CNewConfigProductEjectorDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	pDC->SetTextColor(cBlack);
	
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function2Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vFunction2ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vLocalSystemData->vGreenBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function3Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Button, 5);  //5 is medium large
		if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		{
			pDC->SelectObject(vLocalSystemData->vCurrentLargeFont);
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
		else
		if (vLocalSystemData->vYellowMessageButtonYellow)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
		else
		{
			pDC->SetBkMode(TRANSPARENT);
			TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Button, 5);  //5 is medium large
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_XRaysOn)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaysOn, 7);
		pDC->SetTextColor(cYellow);
		pDC->SetBkColor(cRed);
		return vLocalSystemData->vRedBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Calibrating)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Calibrating, 7);
		pDC->SetTextColor(cYellow);
		pDC->SetBkColor(cRed);
		return vLocalSystemData->vRedBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function5Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		if ((vLocalSystemData->vEject4InARowCount) && (!vShowPreviousAndNextButtons))
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vFunction5ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vLocalSystemData->vRedBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		if ((vWaitingForTestEjectNextContainer) && (!vLocalSystemData->vEject4InARowCount) && (!vLocalSystemData->vTestEjectWaitCount))
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction1ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vLocalSystemData->vRedBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button) 
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		if(vTestingEject2ndContainer)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vRedBrush;
		}
	}
	
	if (pWnd->GetDlgCtrlID() == IDC_ErrorLine)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ErrorLine, 5);  //5 is medium large
		if (vLocalSystemData->vYellowMessageButtonYellow)
		{
			pDC->SetTextColor(cDarkBlue);
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vYellowBrush;
		}
		else
			return vLocalSystemData->vLightGreenBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Title1) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title1, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Title2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title1, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	//if (pWnd->GetDlgCtrlID() == IDC_Title3)
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title3, 5);
	//	pDC->SetBkMode(TRANSPARENT);
	//	return vLocalSystemData->vLightGreenBrush;
	//}
	if (pWnd->GetDlgCtrlID() == IDC_Title4)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title4, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Title5)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title5, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DistanceUnitsLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DistanceUnitsLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DistanceUnitsLabel2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DistanceUnitsLabel2, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DelayPosition1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DelayPosition1, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vEjectorNumberEditing == 0)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EjectorName1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectorName1, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vEjectorNumberEditing == 0)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DwellTime1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DwellTime1, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vEjectorNumberEditing == 0)
			return vLocalSystemData->vGreenBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_DelayPosition2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DelayPosition2, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vEjectorNumberEditing == 1)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EjectorName2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectorName2, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vEjectorNumberEditing == 1)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DwellTime2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DwellTime2, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vEjectorNumberEditing == 1)
			return vLocalSystemData->vGreenBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_DelayPosition3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DelayPosition3, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		if (vEjectorNumberEditing == 2)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EjectorName3) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectorName3, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		if (vEjectorNumberEditing == 2)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DwellTime3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DwellTime3, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		if (vEjectorNumberEditing == 2)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Display, 5);

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
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vFunction5ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
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
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button) 
	{   
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction2ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button) 
	{   
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction3ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button) 
	{   
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction4ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button) 
	{   
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction5ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
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

void CNewConfigProductEjectorDialog::UpdateDisplay()
{
	vCurrentEjectorName = vLocalConfigurationData->vEjector[vEjectorNumberEditing	+ vFirstEjectorShown].vName;

	DisplayEjectorNames();

	if (vLocalConfigurationData->vSystemEjectorDistance[0	+ vFirstEjectorShown])
		SetDlgItemText(IDC_DelayPosition1, dtoa(vEjectorDelayPosition[0	+ vFirstEjectorShown] - 
			vLocalConfigurationData->vSystemEjectorPositionOffset - vLocalConfigurationData->vSystemEjectorDistance[0	+ vFirstEjectorShown], 2));
	else
	if (vLocalConfigurationData->vSystemEjectorPositionOffset)
		SetDlgItemText(IDC_DelayPosition1, dtoa(vEjectorDelayPosition[0	+ vFirstEjectorShown] - vLocalConfigurationData->vSystemEjectorPositionOffset, 2));
	else
		SetDlgItemText(IDC_DelayPosition1, dtoa(vEjectorDelayPosition[0	+ vFirstEjectorShown], 2));

	if (vLocalConfigurationData->vSystemEjectorDistance[1	+ vFirstEjectorShown])
		SetDlgItemText(IDC_DelayPosition2, dtoa(vEjectorDelayPosition[1	+ vFirstEjectorShown] - 
			vLocalConfigurationData->vSystemEjectorPositionOffset - vLocalConfigurationData->vSystemEjectorDistance[1	+ vFirstEjectorShown], 2));
	else
	if (vLocalConfigurationData->vSystemEjectorPositionOffset)
		SetDlgItemText(IDC_DelayPosition2, dtoa(vEjectorDelayPosition[1	+ vFirstEjectorShown] - vLocalConfigurationData->vSystemEjectorPositionOffset, 2));
	else
		SetDlgItemText(IDC_DelayPosition2, dtoa(vEjectorDelayPosition[1	+ vFirstEjectorShown], 2));

	if (vLocalConfigurationData->vSystemEjectorDistance[2	+ vFirstEjectorShown])
		SetDlgItemText(IDC_DelayPosition3, dtoa(vEjectorDelayPosition[2	+ vFirstEjectorShown] - 
			vLocalConfigurationData->vSystemEjectorPositionOffset - vLocalConfigurationData->vSystemEjectorDistance[2	+ vFirstEjectorShown], 2));
	else
	if (vLocalConfigurationData->vSystemEjectorPositionOffset)
		SetDlgItemText(IDC_DelayPosition3, dtoa(vEjectorDelayPosition[2	+ vFirstEjectorShown] - vLocalConfigurationData->vSystemEjectorPositionOffset, 2));
	else
		SetDlgItemText(IDC_DelayPosition3, dtoa(vEjectorDelayPosition[2	+ vFirstEjectorShown], 2));

	if (_wtoi(vDwellTime[0	+ vFirstEjectorShown]) == 0)// && ((vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]) || (vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType]) || 
		//(vLocalConfigurationData->vNumberOfHeadsToMonitor[cRandomSamplingType])))
		SetDlgItemText(IDC_DwellTime1,_T("Diverter"));
	else
		SetDlgItemText(IDC_DwellTime1,vDwellTime[0	+ vFirstEjectorShown]);

	if (_wtoi(vDwellTime[1	+ vFirstEjectorShown]) == 0)
		SetDlgItemText(IDC_DwellTime2,_T("Diverter"));
	else
		SetDlgItemText(IDC_DwellTime2,vDwellTime[1	+ vFirstEjectorShown]);

	if (_wtoi(vDwellTime[2	+ vFirstEjectorShown]) == 0)
		SetDlgItemText(IDC_DwellTime3,_T("Diverter"));
	else
		SetDlgItemText(IDC_DwellTime3,vDwellTime[2	+ vFirstEjectorShown]);

	if (vEjectorNumberEditing == 0)
		vSubFunction2ButtonEnable = false;	//m_SubFunction2Button.EnableWindow(false);
	else
		vSubFunction2ButtonEnable = true;	//m_SubFunction2Button.EnableWindow(true);

	if (vEjectorNumberEditing == 1)
		vSubFunction3ButtonEnable = false;	//m_SubFunction3Button.EnableWindow(false);
	else
		vSubFunction3ButtonEnable = true;	//m_SubFunction3Button.EnableWindow(true);

	if (vEjectorNumberEditing == 2)
		vSubFunction4ButtonEnable = false;	//m_SubFunction4Button.EnableWindow(false);
	else
		vSubFunction4ButtonEnable = true;	//m_SubFunction4Button.EnableWindow(true);

	SetDlgItemText(IDC_SubFunction8Display, dtoa(vEjectorResponseTime[vEjectorNumberEditing	+ vFirstEjectorShown],4));
	m_SubFunction2Button.Invalidate(false);
	m_SubFunction3Button.Invalidate(false);
	m_SubFunction4Button.Invalidate(false);
}

void CNewConfigProductEjectorDialog::DisplayEjectorNames()
{
	CString TempString("");
	TempString = "";

	if (vLocalConfigurationData->vEjector[0	+ vFirstEjectorShown].vEnabled)
	{
		TempString = vLocalConfigurationData->vEjector[0	+ vFirstEjectorShown].vName;
		m_EjectorName1.EnableWindow(true);
		m_DelayPosition1.EnableWindow(true);
		m_DwellTime1.EnableWindow(true);
	}
	else
	{
		TempString = vLocalConfigurationData->vEjector[0	+ vFirstEjectorShown].vName + " - Disabled";
		m_EjectorName1.EnableWindow(false);
		m_DelayPosition1.EnableWindow(false);
		m_DwellTime1.EnableWindow(false);
	}
	if (TempString.GetLength() > 18)
		m_EjectorName1.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	else
		m_EjectorName1.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
	SetDlgItemText(IDC_EjectorName1,TempString);

	if (vLocalConfigurationData->vEjector[1].vEnabled)
	{
		TempString = vLocalConfigurationData->vEjector[1	+ vFirstEjectorShown].vName;
		m_EjectorName2.EnableWindow(true);
		m_DelayPosition2.EnableWindow(true);
		m_DwellTime2.EnableWindow(true);
	}
	else
	{
		TempString = vLocalConfigurationData->vEjector[1	+ vFirstEjectorShown].vName + " - Disabled";
		m_EjectorName2.EnableWindow(false);
		m_DelayPosition2.EnableWindow(false);
		m_DwellTime2.EnableWindow(false);
	}
	if (TempString.GetLength() > 18)
		m_EjectorName2.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	else
		m_EjectorName2.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
	SetDlgItemText(IDC_EjectorName2,TempString);

	if (vLocalConfigurationData->vEjector[2	+ vFirstEjectorShown].vEnabled)
	{
		TempString = vLocalConfigurationData->vEjector[2	+ vFirstEjectorShown].vName;
		m_EjectorName3.EnableWindow(true);
		m_DelayPosition3.EnableWindow(true);
		m_DwellTime3.EnableWindow(true);
	}
	else
	{
		TempString = vLocalConfigurationData->vEjector[2	+ vFirstEjectorShown].vName + " - Disabled";
		m_EjectorName3.EnableWindow(false);
		m_DelayPosition3.EnableWindow(false);
		m_DwellTime3.EnableWindow(false);
	}
	if (TempString.GetLength() > 18)
		m_EjectorName3.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	else
		m_EjectorName3.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
	SetDlgItemText(IDC_EjectorName3,TempString);
}

void CNewConfigProductEjectorDialog::SetChangeMade() 
{
	if (!vChangeMade)
	{
		vChangeMade = true;
		if (!vShowPreviousAndNextButtons)
		{
			m_Function4Button.ShowWindow(SW_SHOW);
			m_Function4Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			SetDlgItemText(IDC_Function4Button, _T("Cancel"));
			m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_Function2Button, _T("Save and Exit"));
		}
	}
}

void CNewConfigProductEjectorDialog::UpdateButtons() 
{
	if (vNumberOfEjectorsInSystem > 3)
	{
		SetDlgItemText(IDC_Function1Button, _T("Next"));
		SetDlgItemText(IDC_SubFunction2Button, "Select Ejector " + dtoa(vFirstEjectorShown + 1, 0));
		SetDlgItemText(IDC_SubFunction3Button, "Select Ejector " + dtoa(vFirstEjectorShown + 2, 0));
		SetDlgItemText(IDC_SubFunction4Button, "Select Ejector " + dtoa(vFirstEjectorShown + 3, 0));

		if (vLocalConfigurationData->vEjector[0	+ vFirstEjectorShown].vEnabled)
		{
			m_SubFunction2Button.ShowWindow(SW_SHOW);
			m_EjectorName1.ShowWindow(SW_SHOW);
			m_DelayPosition1.ShowWindow(SW_SHOW);
			m_DwellTime1.ShowWindow(SW_SHOW);
		}
		else
		{
			m_SubFunction2Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
			m_EjectorName1.ShowWindow(SW_HIDE);
			m_DelayPosition1.ShowWindow(SW_HIDE);
			m_DwellTime1.ShowWindow(SW_HIDE);
		}

		if (vLocalConfigurationData->vEjector[1	+ vFirstEjectorShown].vEnabled)
		{
			m_SubFunction3Button.ShowWindow(SW_SHOW);
			m_EjectorName2.ShowWindow(SW_SHOW);
			m_DelayPosition2.ShowWindow(SW_SHOW);
			m_DwellTime2.ShowWindow(SW_SHOW);
		}
		else
		{
			m_SubFunction3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
			m_EjectorName2.ShowWindow(SW_HIDE);
			m_DelayPosition2.ShowWindow(SW_HIDE);
			m_DwellTime2.ShowWindow(SW_HIDE);
		}

		if (vLocalConfigurationData->vEjector[2	+ vFirstEjectorShown].vEnabled)
		{
			m_SubFunction4Button.ShowWindow(SW_SHOW);
			m_EjectorName3.ShowWindow(SW_SHOW);
			m_DelayPosition3.ShowWindow(SW_SHOW);
			m_DwellTime3.ShowWindow(SW_SHOW);
		}
		else
		{
			m_SubFunction4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
			m_EjectorName3.ShowWindow(SW_HIDE);
			m_DelayPosition3.ShowWindow(SW_HIDE);
			m_DwellTime3.ShowWindow(SW_HIDE);
		}
	}
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		m_SubFunction8Button.ShowWindow(SW_SHOW);
		m_SubFunction8Display.ShowWindow(SW_SHOW);
		//SetDlgItemText(IDC_SubFunction8Display, dtoa(vEjectorResponseTime[vEjectorNumberEditing	+ vFirstEjectorShown], 4));
	}
	else
	{
		m_SubFunction8Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
		m_SubFunction8Display.ShowWindow(SW_HIDE);
	}

	if ((vOldSourceRampedUp) || (vLocalSystemData->vLastXRayCommandOn) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))
	{
		vSubFunction1ButtonEnable = true;	//m_SubFunction1Button.EnableWindow(true);
		vSubFunction5ButtonEnable = true;	//m_SubFunction5Button.EnableWindow(true);
		if (vShowPreviousAndNextButtons)
			vFunction2ButtonEnable = true;		//m_Function2Button.EnableWindow(true);
		else
			vFunction5ButtonEnable = true;		//m_Function5Button.EnableWindow(true);
	}
	else
	{
		vSubFunction1ButtonEnable = false;	//m_SubFunction1Button.EnableWindow(false);
		vSubFunction5ButtonEnable = false;	//m_SubFunction5Button.EnableWindow(false);
		if (vShowPreviousAndNextButtons)
			vFunction2ButtonEnable = false;		//m_Function2Button.EnableWindow(false);
		else
			vFunction5ButtonEnable = false;		//m_Function5Button.EnableWindow(false);
	}
	m_Function2Button.Invalidate(false);
	m_Function5Button.Invalidate(false);
	m_SubFunction1Button.Invalidate(false);
	m_SubFunction5Button.Invalidate(false);

	if ((vLocalSystemData->vTestEjectNextContainer) && (!vLocalSystemData->vEject4InARowCount) && (!vLocalSystemData->vTestEjectWaitCount))
	{
		if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
			SetDlgItemText(IDC_SubFunction1Button, _T("Cancel Test Eject Next!"));
		else
			SetDlgItemText(IDC_SubFunction1Button, _T("Cancel Test Eject Container!"));
	}
	else
	{
		if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
			SetDlgItemText(IDC_SubFunction1Button, _T("Test Eject Next Image!"));
		else
			SetDlgItemText(IDC_SubFunction1Button, _T("Test Eject Next Container!"));
	}


	if (vShowPreviousAndNextButtons)
	{
		m_Function2Button.ShowWindow(SW_SHOW);
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		if (vLocalSystemData->vEject4InARowCount)
			SetDlgItemText(IDC_Function2Button, _T("Cancel Eject 4 In a Row"));
		else
			SetDlgItemText(IDC_Function2Button, _T("Test Eject 4 In a Row"));
	}
	else
	{
		m_Function5Button.ShowWindow(SW_SHOW);
		m_Function5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		if (vLocalSystemData->vEject4InARowCount)
			SetDlgItemText(IDC_Function5Button, _T("Cancel Eject 4 In a Row"));
		else
			SetDlgItemText(IDC_Function5Button, _T("Test Eject 4 In a Row"));
	}

	if (vLocalSystemData->vTestEjectWaitCount)
	{
		if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
			SetDlgItemText(IDC_SubFunction5Button, _T("Cancel Eject 2nd Image"));
		else
			SetDlgItemText(IDC_SubFunction5Button, _T("Cancel Eject 2nd Container"));
	}
	else
	{
		if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
			SetDlgItemText(IDC_SubFunction5Button, _T("Test Eject 2nd\nImage"));
		else
			SetDlgItemText(IDC_SubFunction5Button, _T("Test Eject 2nd Container"));
	}
}

void CNewConfigProductEjectorDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == vDisplayTimerHandle)
  {
		UpdateDisplayFromTimer();
	}
	else
	if (nIDEvent == vStartSetupTimerHandle)  //done only when enter the window
	{
		KillTimer(vStartSetupTimerHandle);
		if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		{
			if (vMainWindowPointer)
			{
				if ((vGlobalCurrentProduct != vGlobalCurrentProduct) || (!vGlobalCurrentProduct))
				{
					vMainWindowPointer->SetupProduct(vGlobalCurrentProduct, false);//turn source off
					Sleep(1000);
				}
				//dont turn source on
				vMainWindowPointer->StartRunning(false, false);
				vLocalSystemData->vSystemRunMode = cAutoSetupRunningSystem;
				ReportErrorMessage("Start Edit Ejector Timing Menu", cUserAction,0);

				if ((vLocalSystemData->vFPGASimulateEncoderRate) || (vLocalConfigurationData->vContinuousFeedEncoderRate))
					vMainWindowPointer->TurnSimulatedEncoderOn();
				else
				if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac))
				if (!vLocalConfigurationData->vUseRealEncoder) //only use if using the motor signal
				{
					vLocalSystemData->vRealConveyorEncoderPulsesCount = vLocalSystemData->vConveyorEncoderRate;
					vLocalSystemData->vSecondsMeasuringRealConveyorEncoder = 1;
					ReportErrorMessage("Edit Ejector Timing Menu Encoder Rate: " + dtoa(vLocalSystemData->vConveyorEncoderRate), cAction,0);

					int TimerResult = SetTimer(vStart16XEncoderTimerHandle,4000,NULL);
					if (!TimerResult)
						ReportErrorMessage("Error-Start 16x Encoder Timer Failed",cEMailInspx, 32000);

					//ReportErrorMessage("Edit Ejector Timing Menu Final Encoder Rate: " + dtoa(vLocalSystemData->vConveyorEncoderRate), cAction,0);
					//vMainWindowPointer->SetSimulatedEncoderTo16TimesConveyor();
				}

				UpdateButtons();
			}
		}
		else
		{
			//Turn off x-rays so can test ejector with x-rays
			vGlobaluCSerialPort->SendXRayPowerOnOffToUController(0);
			vLocalSystemData->vShuttingDownXRays = false;

		}
		UpdateFunction3Button();
	}
	else
	if (nIDEvent == vStart16XEncoderTimerHandle)  //done only when enter the window
	{
		KillTimer(vStart16XEncoderTimerHandle);
		ReportErrorMessage("Edit Ejector Timing Menu Final Encoder Rate: " + dtoa(vLocalSystemData->vConveyorEncoderRate), cUserChanges,0);
		vMainWindowPointer->SetSimulatedEncoderTo16TimesConveyor();
		UpdateButtons();
		UpdateFunction3Button();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CNewConfigProductEjectorDialog::UpdateDisplayFromTimer()
{

	if (vWaitingForTestEjectNextContainer)
	if (!vLocalSystemData->vTestEjectNextContainer)
	{
		vWaitingForTestEjectNextContainer = false;
		vTestingEject2ndContainer = false;
		UpdateButtons();
	}
	if (vOldXRayOnNotice != vLocalSystemData->vXRayOnNotice)
	{
		vLocalSystemData->vEject4InARowCount = 0;
		vWaitingForTestEjectNextContainer = false;
		vTestingEject2ndContainer = false;
		vLocalSystemData->vTestEjectNextContainer = 0;

		vOldXRayOnNotice = vLocalSystemData->vXRayOnNotice;
		if (vOldXRayOnNotice)
			m_XRaysOn.ShowWindow(SW_SHOW);
		else
			m_XRaysOn.ShowWindow(SW_HIDE);
		UpdateButtons();
	}
	if (vLocalSystemData->vSourceRampedUp != vOldSourceRampedUp)
	{
		vOldSourceRampedUp = vLocalSystemData->vSourceRampedUp;
		UpdateButtons();
	}

		if (vLocalSystemData->vYellowMessageButtonYellow != vOldButton3Yellow)
	{
		if (vLocalSystemData->vYellowMessageButtonYellow)
		{
			SetDlgItemText(IDC_ErrorLine,vLocalSystemData->vLastErrorMessage);
			vOldErrorMessage = vLocalSystemData->vLastErrorMessage;
		}
		UpdateFunction3Button();
		vOldButton3Yellow = vLocalSystemData->vYellowMessageButtonYellow;
	}
	else
	if (vOldButton3Yellow)
	{
		if (vOldErrorMessage != vLocalSystemData->vLastErrorMessage)
		{
			SetDlgItemText(IDC_ErrorLine,vLocalSystemData->vLastErrorMessage);
			vOldErrorMessage = vLocalSystemData->vLastErrorMessage;
		}
	}
}


void CNewConfigProductEjectorDialog::PrepareToExitDialog()
{
	if (vLocalSystemData->vInAutoSetup)  //if in auto setup, load product to clear images
		vLocalSystemData->vCurrentProductChanged = true;//force system to reload product settings

	vGlobalCurrentProduct->CalculateEndOfLineTimeOut();
	vLocalSystemData->vEject4InARowCount = 0;
	vLocalSystemData->vTestEjectNextContainer = 0;
	vLocalSystemData->vTestEjectWaitCount = 0;
	if (vLocalSystemData->vInAutoSetup)
	{
		if (vDisableEjectorsWhenExit)
		{
			vMainWindowPointer->EnableDisableEjectors(false);
		}
		if (vStopRunningWhenExit)
		{
			if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
				vMainWindowPointer->StopRunning(true);
		}
		else
		if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
		{
			vGlobaluCSerialPort->SendXRayPowerOnOffToUController(1);
		}

		ReportErrorMessage("Exit Auto Setup Ejectors", cWriteToLog,0);
	}
	else
		ReportErrorMessage("Exit Setup Ejectors", cWriteToLog,0);
}

void CNewConfigProductEjectorDialog::HandleErrorButton()
{
	if (vLocalSystemData->vYellowMessageButtonYellow)
	{
		ClearYellowWarningButton();
		for (BYTE TempLoop = 0; TempLoop < 2; TempLoop++)
		if (vLocalSystemData->vGaveTooManyRejectsAlarm[TempLoop])
		{//turn alarm output bit off
			vLocalSystemData->vGaveTooManyRejectsAlarm[TempLoop] = false;
		}
		if (vLocalSystemData->vCurrentAlarmStatus)
			vGlobaluCSerialPort->TurnAlarmOnOff(1,false);//clear alarm 1, 2, 3 bits
	}
	UpdateFunction3Button();
}

void CNewConfigProductEjectorDialog::UpdateFunction3Button() 
{
	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	{
		SetDlgItemText(IDC_Function3Button, _T("Run"));
		m_Function3Button.ShowWindow(SW_SHOW);
	}
	else
	if (vLocalSystemData->vYellowMessageButtonYellow)
	{
		CString TempText("");
		TempText.LoadString(IDS_ClearWarning);
		m_Function3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_Function3Button,TempText);
		m_Function3Button.ShowWindow(SW_SHOW);
	}
	else
	{
		m_Function3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
	}
}

void CNewConfigProductEjectorDialog::ClearYellowWarningButton() 
{
	vMainWindowPointer->ClearYellowWarningButton();
	SetDlgItemText(IDC_ErrorLine, _T(""));
	UpdateFunction3Button();
}

void CNewConfigProductEjectorDialog::TestEject4InARow()
{
	// Test Eject 4 in a Row Containers button pressed
	if (vLocalSystemData->vEject4InARowCount)
	{
		vLocalSystemData->vTestEjectNextContainer =  0;
		vWaitingForTestEjectNextContainer = false;
		vTestingEject2ndContainer = false;
		vLocalSystemData->vTestEjectWaitCount = 0;
		vLocalSystemData->vEject4InARowCount = 0;
		UpdateButtons();
	}
	else
	//if (vLocalConfigurationData->vEnableEjectors)
	{
		if ((vOldSourceRampedUp) || (vLocalSystemData->vLastXRayCommandOn) ||
			(vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))
		{
			vTestingEject2ndContainer = false;
			vLocalSystemData->vTestEjectWaitCount = 0;
			vLocalSystemData->vEject4InARowCount = 4;
			vLocalSystemData->vTestEjectNextContainer =  1 << (vEjectorNumberEditing	+ vFirstEjectorShown);
			vWaitingForTestEjectNextContainer = true;
			UpdateButtons();
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nSystem must be running to test ejectors.";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	/*
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nEjector are disabled on the Main Menu.";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	*/
}

void CNewConfigProductEjectorDialog::TestEjectNextContainer(BYTE TempWaitCount)
{
	//eject next containter button pressed
	//if (vLocalConfigurationData->vEnableEjectors)
	{
		vTestingEject2ndContainer = false;
		vLocalSystemData->vEject4InARowCount = 0;
		if ((vOldSourceRampedUp) || (vLocalSystemData->vLastXRayCommandOn) ||
			(vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))
		{
			vLocalSystemData->vTestEjectWaitCount = TempWaitCount;
			vWaitingForTestEjectNextContainer = true;
			vLocalSystemData->vTestEjectNextContainer = 1 << (vEjectorNumberEditing	+ vFirstEjectorShown);
			UpdateButtons();
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nSystem must be running to test ejectors.";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
}

void CNewConfigProductEjectorDialog::GiveEjectorNotUsedMessage()
{
	if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes == 0)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\nThis ejector is not used by any inspections in this product.\nIt does not need to be configured\nand may not be programmed.\nSelect to use in an inspection before timing.";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
}

void CNewConfigProductEjectorDialog::ShowNoMouseClickDialogMessage() 
{
	CNoticeDialog TempNoticeDialog;
	CString TempText("");
	TempText = ("Mouse Click Not Available");
	TempNoticeDialog.vNoticeText = "\n\n\nMouse Click Not Available";
	TempNoticeDialog.vType = cInformationMessage;
	TempNoticeDialog.DoModal();
}

void CNewConfigProductEjectorDialog::OnDialogtitlestatictext1()
{
	//bring up soft key menu for help, print screen, Image, Clear, and hidden shift key
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
