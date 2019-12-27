//ScanTrac Side View Source File
// NewDiagnosticDialog.cpp : implementation file
//

#include "stdafx.h"
#include "windows.h"  //for MEMORYSTATUSEX
#include "ScanTrac.h"
#include "NewDiagnosticDialog.h"
#include "SystemConfigurationData.h"
#include "AlphaEntryDialog.h"
#include "PasswordEntryDialog.h"
#include "NoticeDialog.h"
#include "NumericEntryDialog.h"
#include "NewImageDisplayDialog.h"
#include "MoreDiagnosticsDialog.h"
#include "XRaySourceStatusDialog.h"
#include "uControllerStatusDialog.h"
#include "DiagnosticLineDialog.h"
#include "KeyBoardTestDialog.h"
#include "SelectItemDialog.h"
#include "SystemCountersDialog.h"
#include "FrontEndDiagnosticDialog.h"
#include "NewImageDisplayDialog.h"
#include "SerialPort.h"
#include "HelpDialog.h"
#include "YesNoDialog.h"
#include "SpellmanDXMXRaySourcePort.h"
#include "HistogramDisplayDialog.h"
#include "XRaySource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern DWORD vGlobalFBWFEnable;
extern bool vGlobalFPGAVersion10Point87OrHigher;
extern BYTE vGlobalPCRunningWindowsXP;
extern bool vGlobalFPGAVersion10Point0OrHigher;
extern BYTE vGlobalShiftKeyDown;
extern DWORD vGlobalAmountOfFreeMemory;
extern CProduct *vGlobalCurrentProduct;
extern CString vGlobalCurrentDirectory;
extern CSerialPort *vGlobaluCSerialPort;
extern HCURSOR vGlobalCursorSave;
extern CXRaySource *vGlobalMonoBlockXRayPort;
extern CSpellmanDXMXRaySourcePort *vGlobalDXMXRayPort;
extern int vGlobalPasswordLevel;
extern unsigned long vGlobalDialogTitleColor;
extern double vGlobalHardDriveTotalBytes;
extern UINT vGlobalProcessTimePercent;
extern UINT vGlobalThreadTimePercent; 

/////////////////////////////////////////////////////////////////////////////
// CNewDiagnosticDialog dialog

BYTE *TempReadBuffer;

CNewDiagnosticDialog::CNewDiagnosticDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNewDiagnosticDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewDiagnosticDialog)
	//}}AFX_DATA_INIT
	vOldPosteduCVoltageProblemMessage = 0;
	vOldConveyorEncoderRate = 0;
	vOldShiftKeyDown = 0;
	vOldLastBodyTriggerLength = 0;
	vOldBodyTriggerWidthStandardDeviation = 0;
	vOldBodyTriggerWidthAverage = 0;
	vHadSourceEvent = false;
	vOldGlobalPasswordOK = false;
	vAllowExtraFunctions = false;
	vOldFreeTimeMinimum = 0;
	vOldFreeTimeAverage = 0;
	vOldFreeTimeStandardDeviation = 0;
	//vOldDriftCompensationThresholdReductionAmount = 0;
	//vOldDriftCompensationThresholdIncreaseAmount = 0;
	vOldDriftCompensationADCOffset = 999999;
	vOldDriftCompensationDACOffset = 999999;

	vMainWindowPointer = NULL;

	vOldContainerRate = 0;
	vOldBeltRate = 0;
	vOldActualEncoderRate = 0;
	vOldPercentBeltSpeed = 0;

	vOldContainerRateMaximum = 0;
	vOldBeltRateMaximum = 0;
	vOldActualEncoderRateMaximum = 0;
	vOldPercentBeltSpeedMaximum = 0;

 	vOlduCTempOK = true;
	vuCTempOK = true;
	vOldSourceTooHot = false;
	vSourceTooHot = false;
	vUpdateDisplayTimerHandle = 56;
	//vBackupTimerHandle = 57;
	//vTestTimerHandle = 58;
	vChangeMade = false;
	vOldXRayOnNotice = false;
	vProductPasswordOK = false;
	vOldNoCommunicationsWithUController = false;
	vOldSourceInterLocks = 0xAA;
	vOlduCLEDGYR = 0xAA;
// currently not monitored 11/3/09 kjh	vOldFPGASelfTest = 0;
	vOldAmountOfFreeMemory = 0;
	vOlduCMainLoopCounter = 0;
	vOneExtraWork = 0;
	vOldRedLampBrokenCounter = 255;
	vOldYellowLampBrokenCounter = 255;
	vOldBlueLampBrokenCounter = 255;
	vHardDriveSpaceFree = 0;
	vOldCpuUsage = 101;
	vOld_ProcessTimePercent = 101;
	vOld_ThreadTimePercent = 101;
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_InspxHDUnProtected);
	vLocalCWndCollection.Add(&m_FBWF_ProtectedLabel);
	vLocalCWndCollection.Add(&m_CpuLoadImageThreadLabel);
	vLocalCWndCollection.Add(&m_CpuLoadImageThread);
	vLocalCWndCollection.Add(&m_ProcessUsageT);
	vLocalCWndCollection.Add(&m_ActualCpuLoadLabel);
	vLocalCWndCollection.Add(&m_ActualCpuLoad);
	vLocalCWndCollection.Add(&m_FreeDiskSpacePercentageLabel);
	vLocalCWndCollection.Add(&m_FreeDiskSpacePercentage);
	vLocalCWndCollection.Add(&m_FreeMemoryPercentageLabel);
	vLocalCWndCollection.Add(&m_FreeMemoryPercentage);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_EncoderCountsOnLastTriggerLabel3);
	vLocalCWndCollection.Add(&m_EncoderCountsOnLastTrigger3);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_SourceCoolantFlow);
	vLocalCWndCollection.Add(&m_SourceCoolantFlowLabel);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_XRaysOn);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_ActualEncoderRate);
	vLocalCWndCollection.Add(&m_ActualEncoderRatelabel);
	vLocalCWndCollection.Add(&m_ActualEncoderRateMaximum);
	vLocalCWndCollection.Add(&m_BeltSpeed);
	vLocalCWndCollection.Add(&m_BeltSpeedLabel);
	vLocalCWndCollection.Add(&m_BeltSpeedMaximum);
	vLocalCWndCollection.Add(&m_BlueLamp);
	vLocalCWndCollection.Add(&m_BlueLampLabel);
	vLocalCWndCollection.Add(&m_ContainersPerMinute);
	vLocalCWndCollection.Add(&m_ContainersPerMinuteLabel);
	vLocalCWndCollection.Add(&m_ContainersPerMinuteMaximum);
	vLocalCWndCollection.Add(&m_CustomerName);
	vLocalCWndCollection.Add(&m_DensityCompensation);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_EncoderPercentOfMaximum);
	vLocalCWndCollection.Add(&m_EncoderPercentOfMaximumLabel);
	vLocalCWndCollection.Add(&m_EncoderPercentOfMaximumMaximum);
	vLocalCWndCollection.Add(&m_Interlocks);
	vLocalCWndCollection.Add(&m_InterlocksLabel);
	vLocalCWndCollection.Add(&m_MaximumLabel);
	vLocalCWndCollection.Add(&m_OnOff);
	vLocalCWndCollection.Add(&m_OnOffLabel);
	vLocalCWndCollection.Add(&m_RedLamp);
	vLocalCWndCollection.Add(&m_RedLampLabel);
	vLocalCWndCollection.Add(&m_SourceTemperature);
	vLocalCWndCollection.Add(&m_SourceTemperatureLabel);
	vLocalCWndCollection.Add(&m_SpeedFactor);
	vLocalCWndCollection.Add(&m_SpeedFactor1);
	vLocalCWndCollection.Add(&m_SpeedFactorLabel);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_ThresholdIncrease);
	vLocalCWndCollection.Add(&m_ThresholdReduction);
	vLocalCWndCollection.Add(&m_uCStatus);
	vLocalCWndCollection.Add(&m_uCStatusLabel);
	vLocalCWndCollection.Add(&m_XRayStatus);
	vLocalCWndCollection.Add(&m_XRayStatusLabel);
	vLocalCWndCollection.Add(&m_YellowLamp);
	vLocalCWndCollection.Add(&m_YellowLampLabel);
	vLocalCWndCollection.Add(&m_FrontEndStatus);
	vLocalCWndCollection.Add(&m_FrontEndStatusLabel);
}

void CNewDiagnosticDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

	vOldCPMNotRunning = 0xFFFF; //force window to update CPM display

	vFunction5ButtonEnable = true;
	vSubFunction8ButtonEnable = true;
	MEMORYSTATUS memstat;
	GlobalMemoryStatus(&memstat);
	vGlobalAmountOfFreeMemory = memstat.dwAvailPageFile;
	FLOAT vTempAvailMemory = (FLOAT) (memstat.dwAvailPageFile);
	FLOAT vTempTotalMemory = (FLOAT)(memstat.dwTotalPageFile);
	FLOAT vFLOATPercentageOfFreeMemory = (((vTempAvailMemory * 1000L)/vTempTotalMemory) + 5L)/10L;
	vPercentageOfFreeMemory = (UINT)(vFLOATPercentageOfFreeMemory);

	m_Function4Button.ShowWindow(SW_SHOW);

	SetDlgItemText(IDC_DensityCompensation, _T(""));
	//SetDlgItemText(IDC_ThresholdReduction,"");
	//SetDlgItemText(IDC_ThresholdIncrease,"");

	CString TempText("");
	CString TempText1("");
	TempText.LoadString(IDS_EncoderCountsOnLastTrigger);
	SetDlgItemText(IDC_EncoderCountsOnLastTriggerLabel3,TempText);
	TempText.LoadString(IDS_DiagnosticSummaryMenu);
	SetDlgItemText(IDC_DialogTitleStaticText1,TempText);
	TempText.LoadString(IDS_TriggerData);
	SetDlgItemText(IDC_Function2Button,TempText);
	TempText.LoadString(IDS_EncoderData);
	SetDlgItemText(IDC_Function3Button,TempText);
	TempText.LoadString(IDS_TestFireEjectorNow);
	SetDlgItemText(IDC_Function4Button,TempText);

	TempText.LoadString(IDS_Interlocks);
	SetDlgItemText(IDC_InterlocksLabel,TempText + ":");

	TempText.LoadString(IDS_OK);
	SetDlgItemText(IDC_uCStatus,TempText);
	SetDlgItemText(IDC_XRayStatus,TempText);
	SetDlgItemText(IDC_Interlocks,TempText);
	SetDlgItemText(IDC_SourceTemperature,TempText);
	SetDlgItemText(IDC_FrontEndStatus,TempText);
	
	if (vLocalConfigurationData->vUseExternalInterlock)
		SetDlgItemText(IDC_CoolantFlow, _T("Open"));
	else
		TempText.LoadString(IDS_Fault);
	SetDlgItemText(IDC_SourceCoolantFlow,TempText);
	SetDlgItemText(IDC_YellowLamp,TempText);
	SetDlgItemText(IDC_RedLamp,TempText);
	SetDlgItemText(IDC_BlueLamp,TempText);

	TempText.LoadString(IDS_MicroControllerComm);
	SetDlgItemText(IDC_FrontEndStatusLabel,TempText + ":");
	TempText.LoadString(IDS_MicroControllerPower);
	SetDlgItemText(IDC_uCStatusLabel,TempText + ":");
//	SetDlgItemText(IDC_uCResetsLabel,"uController Resets:");
//	SetDlgItemText(IDC_uCMainLoopCounterLabel,"uController Health:");
	TempText.LoadString(IDS_CPULoad);
	SetDlgItemText(IDC_CpuLoadLabel,TempText + ":");
	SetDlgItemText(IDC_CpuLoad, _T("  1%"));
	TempText.LoadString(IDS_FreeMemory);
	SetDlgItemText(IDC_FreeMemoryPercentageLabel,TempText + ":");
	TempText.LoadString(IDS_FreeDiskSpace);
	SetDlgItemText(IDC_FreeDiskSpacePercentageLabel,TempText + ":");
	
	TempText.LoadString(IDS_XRAYON);
	SetDlgItemText(IDC_XRaysOn,TempText);

	TempText.LoadString(IDS_XRaySourceStatus);
	SetDlgItemText(IDC_XRayStatusLabel,TempText + ":");
	TempText.LoadString(IDS_XRaySourceTemperature);
	SetDlgItemText(IDC_SourceTemperatureLabel,TempText + ":");

	if (vLocalConfigurationData->vUseExternalInterlock)
		TempText.LoadString(IDS_ExternalInterlock);
	else
		TempText.LoadString(IDS_XRaySourceCoolantFlow);

	SetDlgItemText(IDC_SourceCoolantFlowLabel,TempText + ":");
	TempText.LoadString(IDS_YellowLight);
	SetDlgItemText(IDC_YellowLampLabel,TempText + ":");
	TempText.LoadString(IDS_BlueLight);
	SetDlgItemText(IDC_BlueLampLabel,TempText + ":");
	TempText.LoadString(IDS_RedLight);
	SetDlgItemText(IDC_RedLampLabel,TempText + ":");
	TempText.LoadString(IDS_MeasuredEncoderRate);
	SetDlgItemText(IDC_ActualEncoderRateLabel,TempText + ":");
	TempText.LoadString(IDS_EncoderPercentOfMaximum);
	SetDlgItemText(IDC_EncoderPercentOfMaximumLabel,TempText + ":");
	//if (vLocalConfigurationData->vUseMetric)
	//	TempText.LoadString(IDS_ConveyorRateMMin);
	//else
		TempText.LoadString(IDS_ConveyorRateFtMin);
	SetDlgItemText(IDC_BeltSpeedLabel,TempText + ":");

	TempText.LoadString(IDS_MainMenu);
	SetDlgItemText(IDC_Function1Button,TempText);

	TempText.LoadString(IDS_MicroControllerPower);
	SetDlgItemText(IDC_SubFunction2Button,TempText);
	TempText.LoadString(IDS_MicroControllerComm);
	SetDlgItemText(IDC_SubFunction3Button,TempText);
	TempText.LoadString(IDS_XRaySourceStatus);
	SetDlgItemText(IDC_SubFunction1Button,TempText);
	TempText.LoadString(IDS_SystemCountersMenu);
	SetDlgItemText(IDC_SubFunction4Button,TempText);

	TempText.LoadString(IDS_ShowSoftwareVersion);
	SetDlgItemText(IDC_SubFunction5Button,TempText);
	TempText.LoadString(IDS_DiagnosticOscilloscope);
	SetDlgItemText(IDC_SubFunction6Button,TempText);
	TempText.LoadString(IDS_KeypadTestMenu);
	SetDlgItemText(IDC_SubFunction7Button,TempText);

	SetDlgItemText(IDC_CustomerName,vLocalConfigurationData->vScanTracCustomerName);

	TempText.LoadString(IDS_ContainersPerMin);
	SetDlgItemText(IDC_ContainersPerMinuteLabel,TempText + ":");

	ShowInspxItems();

	if (vLocalSystemData->vInAutoSetup)
	{
		vAllowExtraFunctions = true;
		m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_Function1Button, _T("Return to Auto Setup"));
	}
	else
	{
		CWnd *TempWindow = NULL;
		TempWindow = FindWindow(NULL, _T("Help"));
		if (TempWindow) 
		{
			vAllowExtraFunctions = true;
			m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_Function1Button, _T("Exit Diagnostics"));
		}
	}
	if (vAllowExtraFunctions)
	{
		CWnd *TempWindow = NULL;
		TempWindow = FindWindow(NULL, _T("Image Display"));
		if (TempWindow) 
		{
			m_Function5Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
		}
		else
		{
			m_Function5Button.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_Function5Button, _T("Rejects Display"));
		}

		if (!vGlobalCurrentProduct)
		{
			vFunction5ButtonEnable = false;	//m_Function5Button.EnableWindow(false);
			m_Function5Button.Invalidate(false);
			SetDlgItemText(IDC_Function3Button, _T("System Summary"));
		}
		else
			SetDlgItemText(IDC_Function3Button, _T("Product Summary"));
		//m_Function3Button.ShowWindow(SW_SHOW);
		//m_ArrowDown3.ShowWindow(SW_SHOW);
	}
	else
	{
		TempText.LoadString(IDS_EncoderData);
		SetDlgItemText(IDC_Function3Button,TempText);
		//m_Function3Button.ShowWindow(SW_SHOW);
		//m_ArrowDown3.ShowWindow(SW_SHOW);

		TempText.LoadString(IDS_ClearMaximums);
		SetDlgItemText(IDC_Function5Button,TempText);
		m_Function5Button.ShowWindow(SW_SHOW);
	}

	//if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	if ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && ((vGlobalCurrentProduct) && (!vGlobalCurrentProduct->vBulkProductMode))) //Not Continuous Feed, Not a Pipeline
	{
		m_SubFunction8Button.ShowWindow(SW_SHOW);
		m_EncoderCountsOnLastTriggerLabel3.ShowWindow(SW_SHOW);
		m_EncoderCountsOnLastTrigger3.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction8Button, _T("Start Test Trigger"));
	}

	if (((vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac)) && (!vLocalConfigurationData->vUseExternalInterlock))
		//hide coolant flow since 50 watt has no coolant 
	{
		m_SourceCoolantFlowLabel.ShowWindow(SW_HIDE);
		m_SourceCoolantFlow.ShowWindow(SW_HIDE);
	}

	if ((vLocalConfigurationData->vScanTracType == cLegatoScanTrac) || (vLocalConfigurationData->vScanTracType == cFermataScanTrac))// a pipeline
	{
		m_EncoderCountsOnLastTriggerLabel3.ShowWindow(SW_HIDE);
		m_EncoderCountsOnLastTrigger3.ShowWindow(SW_HIDE);

		SetDlgItemText(IDC_ContainersPerMinuteLabel, _T("Frames Per Minute:"));
		SetDlgItemText(IDC_BeltSpeedLabel, _T("Product Flow Rate:"));
		SetDlgItemText(IDC_EncoderPercentOfMaximumLabel, _T("Product Flow Percent of Max:"));
	}
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		m_InspxHDUnProtected.ShowWindow(SW_HIDE);
		m_FBWF_ProtectedLabel.ShowWindow(SW_SHOW);
		if (vGlobalFBWFEnable == 1)
			SetDlgItemText(IDC_FBWF_ProtectedLabel, _T("Protected"));
		else
		{
			CFileStatus TempFileStatus;
			CString TempFileName = "\\WINDOWS\\system32\\fbwfmgr.exe";
			if (CFile::GetStatus(TempFileName, TempFileStatus))
				SetDlgItemText(IDC_FBWF_ProtectedLabel, _T("UnProtected"));
			else
				SetDlgItemText(IDC_FBWF_ProtectedLabel, _T("Protect Not Installed"));
		}
	}
	else
	{
		m_FBWF_ProtectedLabel.ShowWindow(SW_HIDE);
		if (vGlobalFBWFEnable != 1)
		{
			m_InspxHDUnProtected.ShowWindow(SW_SHOW);
			if ((vLocalSystemData->vGlobalMonitorSizeX >= 1024) && (vLocalSystemData->vGlobalMonitorSizeY >= 768) )
			{
				m_InspxHDUnProtected.SetBitmap(vMainWindowPointer->vInspxGraphicYellowBigger);
			}
		}
		else
			m_InspxHDUnProtected.ShowWindow(SW_HIDE);
	}
		
	//if (vGlobalFBWFEnable == 1)
	//	SetDlgItemText(IDC_FBWF_ProtectedLabel, _T("Protected"));
	//else
	//{
	//	CFileStatus TempFileStatus;
	//	CString TempFileName = "\\WINDOWS\\system32\\fbwfmgr.exe";
	//	if (CFile::GetStatus(TempFileName, TempFileStatus))
	//		SetDlgItemText(IDC_FBWF_ProtectedLabel, _T("UnProtected"));
	//	else
	//		SetDlgItemText(IDC_FBWF_ProtectedLabel, _T("Protect Not Installed"));
	//}
	//	m_FBWF_ProtectedLabel.ShowWindow(SW_SHOW);
	//else
	//	m_FBWF_ProtectedLabel.ShowWindow(SW_HIDE);

	//SetDlgItemText(IDC_FreeTime,"");
	UpdateDisplay();
	UpdateButtons();

	if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || ((vGlobalCurrentProduct) && (vGlobalCurrentProduct->vBulkProductMode))) //Continuous Feed, or a Pipeline
	{
		m_Function2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());
	}

	this->SetWindowText(_T("Diagnostics"));

	int TempTimerResult = SetTimer(vUpdateDisplayTimerHandle,500,NULL);
	if (!TempTimerResult)
		ReportErrorMessage("Error-Display Timer Failed",cEMailInspx,32000);
	if (vLocalSystemData->vGlobalMonitorSizeX >= 1024 && vLocalSystemData->vGlobalMonitorSizeY >= 768)
	{
		m_Function3Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
	}
	::SetupMenu(vLocalCWndCollection);
}

void CNewDiagnosticDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewDiagnosticDialog)
	DDX_Control(pDX, IDC_InspxHDUnProtected, m_InspxHDUnProtected);
	DDX_Control(pDX, IDC_FBWF_ProtectedLabel, m_FBWF_ProtectedLabel);
	DDX_Control(pDX, IDC_CpuLoadImageThreadLabel, m_CpuLoadImageThreadLabel);
	DDX_Control(pDX, IDC_CpuLoadImageThread, m_CpuLoadImageThread);
	DDX_Control(pDX, IDC_ProcessUsageT, m_ProcessUsageT);
	DDX_Control(pDX, IDC_ActualCpuLoadLabel, m_ActualCpuLoadLabel);
	DDX_Control(pDX, IDC_ActualCpuLoad, m_ActualCpuLoad);
	DDX_Control(pDX, IDC_FreeDiskSpacePercentageLabel, m_FreeDiskSpacePercentageLabel);
	DDX_Control(pDX, IDC_FreeDiskSpacePercentage, m_FreeDiskSpacePercentage);
	DDX_Control(pDX, IDC_FreeMemoryPercentageLabel, m_FreeMemoryPercentageLabel);
	DDX_Control(pDX, IDC_FreeMemoryPercentage, m_FreeMemoryPercentage);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_EncoderCountsOnLastTriggerLabel3, m_EncoderCountsOnLastTriggerLabel3);
	DDX_Control(pDX, IDC_EncoderCountsOnLastTrigger3, m_EncoderCountsOnLastTrigger3);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_SourceCoolantFlow, m_SourceCoolantFlow);
	DDX_Control(pDX, IDC_SourceCoolantFlowLabel, m_SourceCoolantFlowLabel);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_XRaysOn, m_XRaysOn);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_ActualEncoderRate, m_ActualEncoderRate);
	DDX_Control(pDX, IDC_ActualEncoderRateLabel, m_ActualEncoderRatelabel);
	DDX_Control(pDX, IDC_ActualEncoderRateMaximum, m_ActualEncoderRateMaximum);
	DDX_Control(pDX, IDC_BeltSpeed, m_BeltSpeed);
	DDX_Control(pDX, IDC_BeltSpeedLabel, m_BeltSpeedLabel);
	DDX_Control(pDX, IDC_BeltSpeedMaximum, m_BeltSpeedMaximum);
	DDX_Control(pDX, IDC_BlueLamp, m_BlueLamp);
	DDX_Control(pDX, IDC_BlueLampLabel, m_BlueLampLabel);
	DDX_Control(pDX, IDC_ContainersPerMinute, m_ContainersPerMinute);
	DDX_Control(pDX, IDC_ContainersPerMinuteLabel, m_ContainersPerMinuteLabel);
	DDX_Control(pDX, IDC_ContainersPerMinuteMaximum, m_ContainersPerMinuteMaximum);
	DDX_Control(pDX, IDC_CustomerName, m_CustomerName);
	DDX_Control(pDX, IDC_DensityCompensation, m_DensityCompensation);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_EncoderPercentOfMaximum, m_EncoderPercentOfMaximum);
	DDX_Control(pDX, IDC_EncoderPercentOfMaximumLabel, m_EncoderPercentOfMaximumLabel);
	DDX_Control(pDX, IDC_EncoderPercentOfMaximumMaximum, m_EncoderPercentOfMaximumMaximum);
	DDX_Control(pDX, IDC_Interlocks, m_Interlocks);
	DDX_Control(pDX, IDC_InterlocksLabel, m_InterlocksLabel);
	DDX_Control(pDX, IDC_MaximumLabel, m_MaximumLabel);
	DDX_Control(pDX, IDC_OnOff, m_OnOff);
	DDX_Control(pDX, IDC_OnOffLabel, m_OnOffLabel);
	DDX_Control(pDX, IDC_RedLamp, m_RedLamp);
	DDX_Control(pDX, IDC_RedLampLabel, m_RedLampLabel);
	DDX_Control(pDX, IDC_SourceTemperature, m_SourceTemperature);
	DDX_Control(pDX, IDC_SourceTemperatureLabel, m_SourceTemperatureLabel);
	DDX_Control(pDX, IDC_SpeedFactor, m_SpeedFactor);
	DDX_Control(pDX, IDC_SpeedFactor1, m_SpeedFactor1);
	DDX_Control(pDX, IDC_SpeedFactorLabel, m_SpeedFactorLabel);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_ThresholdIncrease, m_ThresholdIncrease);
	DDX_Control(pDX, IDC_ThresholdReduction, m_ThresholdReduction);
	DDX_Control(pDX, IDC_uCStatus, m_uCStatus);
	DDX_Control(pDX, IDC_uCStatusLabel, m_uCStatusLabel);
	DDX_Control(pDX, IDC_XRayStatus, m_XRayStatus);
	DDX_Control(pDX, IDC_XRayStatusLabel, m_XRayStatusLabel);
	DDX_Control(pDX, IDC_YellowLamp, m_YellowLamp);
	DDX_Control(pDX, IDC_YellowLampLabel, m_YellowLampLabel);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_FrontEndStatus, m_FrontEndStatus);
	DDX_Control(pDX, IDC_FrontEndStatusLabel, m_FrontEndStatusLabel);
}


BEGIN_MESSAGE_MAP(CNewDiagnosticDialog, CDialog)
	//{{AFX_MSG_MAP(CNewDiagnosticDialog)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_SubFunction1Button, OnSubFunction1Button)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SubFunction2Button, OnSubFunction2Button)
	ON_BN_CLICKED(IDC_SubFunction3Button, OnSubFunction3Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
	ON_BN_CLICKED(IDC_Function5Button, OnFunction5Button)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SubFunction4Button, OnSubFunction4Button)
	ON_BN_CLICKED(IDC_SubFunction5Button, OnSubFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_BN_CLICKED(IDC_SubFunction7Button, OnSubFunction7Button)
	ON_BN_CLICKED(IDC_SubFunction6Button, OnSubFunction6Button)
//	ON_BN_CLICKED(IDC_DialogTitleStaticText, OnDialogTitleStaticText)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CNewDiagnosticDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewDiagnosticDialog message handlers

void CNewDiagnosticDialog::OnFunction1Button() 
{
	// User clicked Main Menu Button to exit 
	PrepareToExit();
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::EndDialog(10);
}

void CNewDiagnosticDialog::OnFunction2Button() 
{
	if ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && ((vGlobalCurrentProduct) && (!vGlobalCurrentProduct->vBulkProductMode))) //Not Continuous Feed, Not a Pipeline
	if (vLocalSystemData->vBodyTriggerWidthSamples > 0)
	{
		double TempData[cMaximumBodyTriggerWidthData];
		WORD TempSourceIndex = vLocalSystemData->vBodyTriggerWidthIndex;
		if (TempSourceIndex)
			TempSourceIndex--;
		for (WORD TempLoop = 0; TempLoop < vLocalSystemData->vBodyTriggerWidthSamples; TempLoop++)
		{
			TempData[TempLoop] = vLocalSystemData->vBodyTriggerWidthData[TempSourceIndex];
			if (TempSourceIndex)
				TempSourceIndex--;
			else
				TempSourceIndex = vLocalSystemData->vBodyTriggerWidthSamples - 1;
		}

		//histogram of body trigger width data
		CHistogramDisplayDialog IHistogramDisplayDialog;
		IHistogramDisplayDialog.vTopDataLine = "Trigger Data Plot";
		IHistogramDisplayDialog.vNumberOfNumbers = vLocalSystemData->vBodyTriggerWidthSamples;
		IHistogramDisplayDialog.vDataTitle = "Most Recent Trigger Samples";
		IHistogramDisplayDialog.vValuesAreFloats = true;
		IHistogramDisplayDialog.vPointerToFloats = &TempData[0];
		IHistogramDisplayDialog.vLocalSystemData = vLocalSystemData;
		IHistogramDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
		IHistogramDisplayDialog.vMainWindowPointer = vMainWindowPointer;
		IHistogramDisplayDialog.vPlotRawData = true;
		IHistogramDisplayDialog.vXText = "X = Trigger Count";
		IHistogramDisplayDialog.vYText = "Y = Encoder Pulses During Trigger";
		IHistogramDisplayDialog.vShowFittedCurve = false;
		int nResponse = IHistogramDisplayDialog.DoModal();
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		CString TempText;
		TempText.LoadString(IDS_NoTriggerDataToPlot);
		TempNoticeDialog.vNoticeText = TempText;
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}
}

void CNewDiagnosticDialog::OnFunction3Button() 
{
	//display current product information
	//if (PasswordOK(cTemporaryInspxPassword,false))
	if (vAllowExtraFunctions)
	{
		CHelpDialog * IHelpDialog;
		IHelpDialog = new(CHelpDialog);
		IHelpDialog->vHelpContext = 15;
		IHelpDialog->vHelpType = 0;
		IHelpDialog->vWindowType = cProductInformationWindow;
		IHelpDialog->vLocalProductCollection = vLocalProductCollection;
		IHelpDialog->vLocalSystemData = vLocalSystemData;
		IHelpDialog->vMainWindowPointer = vMainWindowPointer;
		IHelpDialog->vLocalConfigurationData = vLocalConfigurationData;
		IHelpDialog->vProductPointer = vGlobalCurrentProduct;
		int TempResult = IHelpDialog->DoModal();
		delete IHelpDialog;
	}
	else
	{
		double TempData[cNumberOfEncoderSamples];
		WORD TempSourceIndex = vLocalSystemData->vEncoderSampleIndex;
		for (WORD TempLoop = 0; TempLoop < cNumberOfEncoderSamples; TempLoop++)
		{
			TempData[TempLoop] = vLocalSystemData->vEncoderSample[TempSourceIndex];
			if (TempSourceIndex < cNumberOfEncoderSamples - 1)
				TempSourceIndex++;
			else
				TempSourceIndex =  0;
		}

		CHistogramDisplayDialog IHistogramDisplayDialog;
		IHistogramDisplayDialog.vTopDataLine = "Encoder Frequency Plot";
		IHistogramDisplayDialog.vNumberOfNumbers = cNumberOfEncoderSamples;
		IHistogramDisplayDialog.vDataTitle = "Encoder Samples For Last 120 Seconds";
		IHistogramDisplayDialog.vValuesAreFloats = true;
		IHistogramDisplayDialog.vPointerToFloats = &TempData[0];
		IHistogramDisplayDialog.vLocalSystemData = vLocalSystemData;
		IHistogramDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
		IHistogramDisplayDialog.vMainWindowPointer = vMainWindowPointer;
		IHistogramDisplayDialog.vPlotRawData = true;
		IHistogramDisplayDialog.vXText = "X is the Second";
		IHistogramDisplayDialog.vYText = "Y is the Encoder Frequency";
		IHistogramDisplayDialog.vShowFittedCurve = false;
		int nResponse = IHistogramDisplayDialog.DoModal();
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
}

void CNewDiagnosticDialog::OnFunction4Button() 
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		//More Diagnostics or Test Menu button pressed
		//Create diagnostic dialog box
		CMoreDiagnosticsDialog IMoreDiagnosticsDialog;  
		//Set dialog box data like how many products there are
		//IMoreDiagnosticsDialog.vLocalProductCollection = &vProductCollection;
		IMoreDiagnosticsDialog.vLocalSystemData = vLocalSystemData;
		IMoreDiagnosticsDialog.vLocalConfigurationData = vLocalConfigurationData;
		IMoreDiagnosticsDialog.vMainWindowPointer = vMainWindowPointer;
		//IMoreDiagnosticsDialog.vRunWithoutXRays = vLocalSystemData->vRunWithoutXRays;

		//Pass control to dialog box and display
		int nResponse = IMoreDiagnosticsDialog.DoModal();

		//if changes were made, save data to file

		if (nResponse == IDOK)
		{
			//Save and exit button pressed
		}
		if (nResponse == 10)
		{
			//Main Menu button pressed
			PrepareToExit();
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_Function4Button);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}
	else
	{
		//Test Fire Ejector Now
		BYTE TempNumberOfEjectorsConfigured = 0;
		for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
		{
			if ((vLocalConfigurationData->vEjector[TempLoop].vEnabled) && (vLocalConfigurationData->vEjector[TempLoop].vLineBitMask))
			{
				TempNumberOfEjectorsConfigured++;
			}
		}
		if (!TempNumberOfEjectorsConfigured)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nNo Ejectors configured in system.";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			bool TempOKToFire = true;

			if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) && (vLocalSystemData->vContainerRate))
				TempOKToFire = false;
			if (!TempOKToFire)
			{
				CYesNoDialog TempYesNoDialog;
				TempYesNoDialog.vNoticeText = _T("\n\nInspecting Containers,\nAre you sure you want to fire ejectors?");
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				TempYesNoDialog.vYesButtonText = "Fire Ejectors";
				TempYesNoDialog.vNoButtonText = "Cancel";
				int TempResult = TempYesNoDialog.DoModal();
				//if user said OK, 
				if (TempResult == IDOK)
					TempOKToFire = true;
			}

			if (TempOKToFire)
			{
				bool TempDisableEjectorsWhenExit = false;
				int TempResult = 0;
				if (!vLocalConfigurationData->vEnableEjectors)
				{
					TempDisableEjectorsWhenExit = true;
					//vGlobaluCSerialPort->EnableDisableEjectors(true);
					//vMainWindowPointer->UpdateDisableEjectorsButton();
					vMainWindowPointer->EnableDisableEjectors(true);
				}
				tSerialCommand TempCommand;
				for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
				if (vLocalConfigurationData->vEjector[TempLoop].vEnabled)
				{
					TempCommand[0] = 0x23;

					if (TempLoop > 2)
						TempCommand[1] = 0x91 + ((TempLoop - 3) * 2);  //0x91 gets translated into FPGA address 9800 by micro controller.  0x93 >> 9802, 0x95 >> 0x9804;  Bit zero gets moved to bit 11 by micro controller
					else //ejector 4 = 0x91 (0x9800), Ejector 5 = 0x93 (0x9802), Ejector 6 = 0x95 (0x9804)  format = Address to send Micro Controller (FPGAs Address)
						TempCommand[1] = 0x9E + ((TempLoop) * 2);  //Ejector 1 = 0x9E (0x900E), Ejector 2 = 0xA0 (0xA000), Ejector 3 = 0xA2 (0xA002)

					//ejector dwell for each of 3 ejectors will cause ejectors to fire
					WORD TempWord = (WORD)(1000 * 31.25);
					if (vGlobalFPGAVersion10Point87OrHigher)
						TempWord = (WORD)(1000 * 3.975); // clock 8 times slower

					TempCommand[2] = TempWord >> 8;
					TempCommand[3] = (BYTE)TempWord;

					if (vGlobaluCSerialPort)
						vGlobaluCSerialPort->SendSerialCommand(TempCommand);

					CNoticeDialog TempNoticeDialog;
					
					TempNoticeDialog.vNoticeText = "\n\n\nFired Ejector " + dtoa(TempLoop + 1,0) + "  for 1 Second";
					TempNoticeDialog.vType = cNoticeMessage;
					TempNoticeDialog.vAutoClose = 1100;
					TempNoticeDialog.DoModal();

					Sleep(cSendCommandSleepTime);
				}
				if (TempDisableEjectorsWhenExit)
					vMainWindowPointer->EnableDisableEjectors(false);
			}
		}
	}
}

void CNewDiagnosticDialog::OnFunction5Button() 
{  
	//Images Button Pressed
	if (vAllowExtraFunctions)
	//if (PasswordOK(cTemporaryInspxPassword,false))
	{
		CWnd *TempWindow = NULL;
		TempWindow = FindWindow(NULL, _T("Image Display"));
		if (TempWindow) 
		{
			//CNoticeDialog TempNoticeDialog;
			//TempNoticeDialog.vNoticeText = "\n\nA Rejects Display is already open.\nClose windows until you return to it.\nClosing Diagnostics and Help Windows now.";
			//TempNoticeDialog.vType = cErrorMessage;
			//TempNoticeDialog.DoModal();

			//OnFunction1Button();
		}
		else
		{
			if ((vLocalSystemData->vITIPCDig->vAquisitionBuffer == NULL) ||
				(vGlobalCurrentProduct == NULL))
			{
				CString TempText;
				TempText.LoadString(IDS_Youmustselectaproducttoseeimages);  //need 3 blank lines first

				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = TempText;
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
			else
			{
				//Create diagnostic dialog box
				CNewImageDisplayDialog IImageDisplayDialog;  
				//Set dialog box data like how many products there are
				IImageDisplayDialog.vLocalSystemData = vLocalSystemData;
				IImageDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
				IImageDisplayDialog.vMainWindowPointer = vMainWindowPointer;

				//Pass control to dialog box and display
				int nResponse = IImageDisplayDialog.DoModal();

				PrepareToExit();
				CDialog::EndDialog(1);
			}
		}
	}
	else
	{
		//Reset running maximums
		vLocalSystemData->vPercentBeltSpeedMaximum = 0;
		vLocalSystemData->vContainerRateMaximum = 0;
		vLocalSystemData->vBeltRateMaximum = 0;
		vLocalSystemData->vActualEncoderRateMaximum = 0;
	}
}


void CNewDiagnosticDialog::OnSubFunction1Button() 
{
	//X Ray Source Status button pressed
	//Create diagnostic dialog box
	CXRaySourceStatusDialog IXRaySourceStatusDialog;  
	//Set dialog box data like how many products there are
	IXRaySourceStatusDialog.vLocalProductCollection = vLocalProductCollection;
	IXRaySourceStatusDialog.vLocalSystemData = vLocalSystemData;
	IXRaySourceStatusDialog.vLocalConfigurationData = vLocalConfigurationData;
	IXRaySourceStatusDialog.vMainWindowPointer = vMainWindowPointer;

	//Pass control to dialog box and display
	int nResponse = IXRaySourceStatusDialog.DoModal();

	//if changes were made, save data to file
	if (nResponse == 10)
	{
		//Main Menu button pressed
		PrepareToExit();
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
	UpdateButtons();
	CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction1Button);
	if (TempWindow)
		TempWindow->Invalidate(false);
}

void CNewDiagnosticDialog::OnSubFunction2Button() 
{
	//uController Power Status Menu button pressed
	//Create diagnostic dialog box
	CuControllerStatusDialog IuControllerStatusDialog;  
	//Set dialog box data like how many products there are
	//IMoreDiagnosticsDialog.vLocalProductCollection = &vProductCollection;
	IuControllerStatusDialog.vLocalSystemData = vLocalSystemData;
	IuControllerStatusDialog.vLocalConfigurationData = vLocalConfigurationData;
	IuControllerStatusDialog.vMainWindowPointer = vMainWindowPointer;

	//Pass control to dialog box and display
	int nResponse = IuControllerStatusDialog.DoModal();

	//if changes were made, save data to file
	if (nResponse == 10)
	{
		//Main Menu button pressed
		PrepareToExit();
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CNewDiagnosticDialog::OnSubFunction3Button() 
{
	//9/11/2009
	//if (PasswordOK(cSetupPassword,true))
	{
		//uController Communications Menu button pressed
		//Create diagnostic dialog box
		CFrontEndDiagnosticDialog IFrontEndDiagnosticDialog;  
		//Set dialog box data like how many products there are
		//IMoreDiagnosticsDialog.vLocalProductCollection = &vProductCollection;
		IFrontEndDiagnosticDialog.vLocalSystemData = vLocalSystemData;
		IFrontEndDiagnosticDialog.vLocalConfigurationData = vLocalConfigurationData;
		IFrontEndDiagnosticDialog.vMainWindowPointer = vMainWindowPointer;

		//Pass control to dialog box and display
		int nResponse = IFrontEndDiagnosticDialog.DoModal();

		//if changes were made, save data to file
		if (nResponse == 10)
		{
			//Main Menu button pressed
			PrepareToExit();
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction3Button);
		if (TempWindow)
			TempWindow->Invalidate(false);
		//9/4/2009
		TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}
}

void CNewDiagnosticDialog::OnSubFunction4Button() 
{
	if (PasswordOK(cSetupPassword,true))
	{
		//Create diagnostic dialog box
		CSystemCountersDialog ISystemCountersDialog;  

		//Set dialog box data like how many products there are
		ISystemCountersDialog.vLocalSystemData = vLocalSystemData;
		ISystemCountersDialog.vLocalConfigurationData = vLocalConfigurationData;
		ISystemCountersDialog.vMainWindowPointer = vMainWindowPointer;

		//Pass control to dialog box and display
		int nResponse = ISystemCountersDialog.DoModal();

		//if changes were made, save data to file
		if (nResponse == 10)
		{
			//Main Menu button pressed
			PrepareToExit();
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
		UpdateButtons();
		//9/4/2009
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}
}

void CNewDiagnosticDialog::OnSubFunction5Button() 
{
	//display software version button
		CString TempText("");
		CString TempScanTracTypeString("");
		
		if (vLocalConfigurationData->vScanTracType == cUpSideDownSoloScanTrac)
			TempScanTracTypeString = "UpSideDown Solo";
		else
		if (vLocalConfigurationData->vScanTracType == cSoloScanTrac)
			TempScanTracTypeString = "Solo";
		else
		if (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)
			TempScanTracTypeString = "Solo Plus";
		else
		if (vLocalConfigurationData->vScanTracType == cLegatoScanTrac)
			TempScanTracTypeString = "Legato";
		else
		if (vLocalConfigurationData->vScanTracType == cFermataScanTrac)
			TempScanTracTypeString = "Fermata";
		else
		if (vLocalConfigurationData->vScanTracType == cAtempoScanTrac)
			TempScanTracTypeString = "Atempo";
		else
		if (vLocalConfigurationData->vScanTracType == cAllegroScanTrac)
			TempScanTracTypeString = "Allegro";
		else
		if (vLocalConfigurationData->vScanTracType == cForteScanTrac)
			TempScanTracTypeString = "Forte";
		else
		if (vLocalConfigurationData->vScanTracType == cPiccoloScanTrac)
			TempScanTracTypeString = "Piccolo";
		else
		if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
			TempScanTracTypeString = "Case Inspector";
		else
		if (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)
			TempScanTracTypeString = "Brioso";
		else
			TempScanTracTypeString.LoadString(IDS_Unknown);

		TempText.LoadString(IDS_ClearSoftwareVersion);
		m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_SubFunction5Button,TempText);
		TempText.LoadString(IDS_VersionDisplayString);

		TempText = TempScanTracTypeString + " " + TempText;

		CString TempString = dtoa(vLocalSystemData->vuCMajorVersionNumber, 0) + "." + dtoa(vLocalSystemData->vuCMinorVersionNumber, 0);
		CString TempText1("");
		CString TempText2("");

		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			TempText1.LoadString(IDS_MicroControllerSWVersion);
			TempText = TempText + "\n" + TempText1 + " " + " " +TempString;

			TempString = dtoa(vLocalSystemData->vFPGAMajorVersionNumber, 0) + "." + dtoa(vLocalSystemData->vFPGAMinorVersionNumber, 0);
			TempText1.LoadString(IDS_FPGASWVersion);
			TempText = TempText + "\n" + TempText1 + " " + " " +TempString;
		}
		else
		{
			TempString = dtoa(vLocalSystemData->vFPGAMajorVersionNumber, 0) + "." + dtoa(vLocalSystemData->vFPGAMinorVersionNumber, 0);
			TempText1.LoadString(IDS_FPGASWVersion);
			TempText = TempText + "\n" + TempText1 + " " + " " +TempString;
		}

		vVersionNoticeDialog.vNoticeText = TempText;
		vVersionNoticeDialog.vAutoClose = 10000;
		vVersionNoticeDialog.vType = cVersionMessage;
		vVersionNoticeDialog.DoModal();
		TempText.LoadString(IDS_ShowSoftwareVersion);
		m_SubFunction5Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		SetDlgItemText(IDC_SubFunction5Button,TempText);
}


void CNewDiagnosticDialog::OnSubFunction6Button() 
{
	// Show Diagnostic Lines
	CDiagnosticLineDialog IDiagnosticLineDialog;  
	//Set dialog box data like how many products there are
	IDiagnosticLineDialog.vLocalSystemData = vLocalSystemData;
	IDiagnosticLineDialog.vLocalConfigurationData = vLocalConfigurationData;
	IDiagnosticLineDialog.vMainWindowPointer = vMainWindowPointer;

	//Pass control to dialog box and display
	int nResponse = IDiagnosticLineDialog.DoModal();
	if (nResponse == 10)  //main menu pressed, so close this dialog
	{
		PrepareToExit();
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CNewDiagnosticDialog::OnSubFunction7Button() 
{
	//Keyboard Test
	//Create keyboard test dialog box
	CKeyBoardTestDialog IKeyBoardTestDialog;  
	//Set dialog box data like how many products there are
	IKeyBoardTestDialog.vLocalSystemData = vLocalSystemData;
	IKeyBoardTestDialog.vLocalConfigurationData = vLocalConfigurationData;
	//Pass control to dialog box and display
	int nResponse = IKeyBoardTestDialog.DoModal();
}

void CNewDiagnosticDialog::OnSubFunction8Button() 
{
	if (vSubFunction8ButtonEnable == true)
	{
		//test body trigger width
		if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		{
			if (vGlobalCurrentProduct)
			{
				if (!vLocalSystemData->vTestingTrigger)
				{
					if ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && ((vGlobalCurrentProduct) && (!vGlobalCurrentProduct->vBulkProductMode))) //Not Continuous Feed, Not a Pipeline
					{
						vLocalSystemData->vBodyTriggerWidthSamples = 0;
						vLocalSystemData->vTestingTrigger = true;

						if ((vLocalSystemData->vFPGASimulateEncoderRate) || (vLocalConfigurationData->vContinuousFeedEncoderRate))
							vMainWindowPointer->TurnSimulatedEncoderOn();
						else
							vMainWindowPointer->SendConveyorOnOffToUController(true);

						vLocalSystemData->vCurrentBodyTriggerMask = vLocalConfigurationData->
							vBodyTrigger[vGlobalCurrentProduct->vBodyTrigger - 1].vLineBitMask;
						if (!vLocalConfigurationData->vBodyTrigger[vGlobalCurrentProduct->vBodyTrigger - 1].vActiveHigh)
							vLocalSystemData->vCurrentBodyTriggerActiveLowMask = 
								vLocalSystemData->vCurrentBodyTriggerActiveLowMask | vLocalConfigurationData->
							vBodyTrigger[vGlobalCurrentProduct->vBodyTrigger - 1].vLineBitMask;

						vLocalSystemData->vBackupBodyTriggerMask = 0;
						if (vGlobalCurrentProduct->vBackupBodyTrigger)
						{
							vLocalSystemData->vBackupBodyTriggerMask = vLocalConfigurationData->
								vBodyTrigger[vGlobalCurrentProduct->vBackupBodyTrigger - 1].vLineBitMask;
							if (!vLocalConfigurationData->vBodyTrigger[vGlobalCurrentProduct->vBackupBodyTrigger - 1].vActiveHigh)
								vLocalSystemData->vCurrentBodyTriggerActiveLowMask = 
									vLocalSystemData->vCurrentBodyTriggerActiveLowMask | vLocalConfigurationData->
								vBodyTrigger[vGlobalCurrentProduct->vBackupBodyTrigger - 1].vLineBitMask;
						}
						vLocalSystemData->vDigitalInputLine3Mask = vLocalSystemData->vBackupBodyTriggerMask | 
							vLocalSystemData->vCurrentBodyTriggerMask;
						if (vLocalSystemData->vBackupBodyTriggerMask &&
							vLocalSystemData->vCurrentBodyTriggerMask)
							vLocalSystemData->vOnlyOneBodyTriggerEnabled = false;
						else
							vLocalSystemData->vOnlyOneBodyTriggerEnabled = true;

						vMainWindowPointer->SendRetriggerLockout(3); //turn off retrigger lockout in case pulses are close together

						//Send Interrupt Mask for body trigger
						tSerialCommand TempCommand;
						TempCommand[0] = 0x1C;
						TempCommand[1] = 0x03;  //send mask for 16 digital lines in #3 , body triggers
						TempCommand[2] = 0x00;
						TempCommand[3] = vLocalSystemData->vDigitalInputLine3Mask;
						if (vGlobaluCSerialPort)
							vGlobaluCSerialPort->SendSerialCommand(TempCommand);
						Sleep(cSendCommandSleepTime);

						TempCommand[1] = 0x04;  //send mask so uC know which bit is body trigger
						TempCommand[2] = 0x00;
						TempCommand[3] = vLocalSystemData->vCurrentBodyTriggerMask;
						if (vGlobaluCSerialPort)
							vGlobaluCSerialPort->SendSerialCommand(TempCommand);
						Sleep(cSendCommandSleepTime);

						if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac))
						if (!vLocalConfigurationData->vUseRealEncoder) //only use if using the motor signal
						{
							WORD TempRate = vLocalConfigurationData->vLastSimulatedEncoderRateSet * 16;
							if (TempRate > vLocalSystemData->vBeltRateMaximum)
								TempRate = (WORD)(vLocalSystemData->vBeltRateMaximum * .95);

							if (vMainWindowPointer)
								vMainWindowPointer->SetMotorSignalSimulatedEncoderRate((WORD)TempRate);
						}
					}
				}
				else
				{ //stop trigger test
					tSerialCommand TempCommand;
					vLocalSystemData->vTestingTrigger = false;
					vLocalSystemData->vDigitalInputLine3Mask = 0x00;
					vLocalSystemData->vCurrentBodyTriggerMask = 0;
					TempCommand[0] = 0x1C;
					TempCommand[1] = 0x03;  //send mask for 16 digital lines in #3
					TempCommand[2] = 0x00;
					TempCommand[3] = 0x00;
					if (vGlobaluCSerialPort)
						vGlobaluCSerialPort->SendSerialCommand(TempCommand);
					Sleep(cSendCommandSleepTime);

					TempCommand[1] = 0x04;  //send mask so uC know which bit is body trigger
					TempCommand[2] = 0x00;
					TempCommand[3] = 0x00;
					if (vGlobaluCSerialPort)
						vGlobaluCSerialPort->SendSerialCommand(TempCommand);
					Sleep(cSendCommandSleepTime);
					vMainWindowPointer->SendConveyorOnOffToUController(false);
					vMainWindowPointer->TurnSimulatedEncoderOff();

					vMainWindowPointer->SendRetriggerLockout(vGlobalCurrentProduct->vLockoutPosition);
				}
				UpdateButtons();
			}
			else
			{
				CString TempText;
				TempText.LoadString(IDS_Youmustselectaproducttotesttrigger);  

				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = TempText;
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
		}
	}
	else
	{	// not a function here because only used once
		CNoticeDialog TempNoticeDialog;
		CString TempText("");
		TempText = ("Trigger Test Not Available while Inspecting Product");
		TempNoticeDialog.vNoticeText = "\n\n\nTrigger Test Not Available while Inspecting Product";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}
}

BOOL CNewDiagnosticDialog::PreTranslateMessage(MSG* pMsg) 
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
		CWnd *TempWindow;
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
				PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 15, 0);

				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			break;
			case cPageUp: // Clear
				if (vGlobalPasswordLevel)
				{
					PasswordOK(cNoPassword,false);
				}
				else
				{
					PasswordOK(cAnyPasswordToEnter, true);
				}
				ShowInspxItems();
				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			break;
			/* br ht
			case c2: // 1 button
				vGlobalCursorSave = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
			break; //this
			case c1: // 1 button
				vGlobalCursorSave = ::SetCursor(::LoadCursor(NULL, IDC_CROSS));
			break;
			case c0: // 1 button
				vGlobalCursorSave = ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
			break;
			*/
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
		}
		return true;  //true indicates it has been handled, so do not process
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CNewDiagnosticDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	// TODO: Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);  //5 is medium large
		
		if (vGlobalShiftKeyDown)
			pDC->SetTextColor(cBlack);
		else
			pDC->SetTextColor(vGlobalDialogTitleColor);
	}


	if (pWnd->GetDlgCtrlID() == IDC_ContainersPerMinute)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ContainersPerMinute, 5);
		if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		if (vOldCPMNotRunning)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cBlack);
			return vLocalSystemData->vYellowBrush;
		}
	}

	//11/5/2009
	if (pWnd->GetDlgCtrlID() == IDC_SpeedFactor)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SpeedFactor, 5);
		if (vOldPercentOfTimeForProcessing > 100)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cBlack);
			return vLocalSystemData->vRedBrush;
	  }
		else
		if (vOldPercentOfTimeForProcessing > 90)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cBlack);
			return vLocalSystemData->vYellowBrush;
	  }
		else
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cBlack);
			return vLocalSystemData->vLightBrownBrush;
	  }
	}

	if (pWnd->GetDlgCtrlID() == IDC_SpeedFactor1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SpeedFactor1, 5);
		if (vOldPercentOfTimeBeforeEject > 100)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cBlack);
			return vLocalSystemData->vRedBrush;
	  }
		else
		if (vOldPercentOfTimeBeforeEject > 90)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cBlack);
			return vLocalSystemData->vYellowBrush;
	  }
		else
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cBlack);
			return vLocalSystemData->vLightBrownBrush;
	  }
	}

	//9/4/2009
	if (pWnd->GetDlgCtrlID() == IDC_FreeDiskSpacePercentage)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FreeDiskSpacePercentage, 5);
		if (vPercentageOfDiskSpaceFree < 5)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cBlack);
			return vLocalSystemData->vRedBrush;
		}
		else
		if (vPercentageOfDiskSpaceFree < 10)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cBlack);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_FreeMemoryPercentage)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FreeMemoryPercentage, 5);
		if (vOldPercentageOfFreeMemory < 5)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cBlack);
			return vLocalSystemData->vRedBrush;
		}
		else
		if (vOldPercentageOfFreeMemory < 10)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cBlack);
			return vLocalSystemData->vYellowBrush;
		}
	}
	
	//make return to Image Display green
	

  if (pWnd->GetDlgCtrlID() == IDC_CustomerName) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CustomerName, 5);
		pDC->SetTextColor(cButtonTextColor);
		pDC->SetBkMode(TRANSPARENT);
	}

	if (pWnd->GetDlgCtrlID() == IDC_RedLamp)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_RedLamp, 5);
		if (vOldRedLampBrokenCounter >= 10)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_YellowLamp)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YellowLamp, 5);
		if (vOldYellowLampBrokenCounter >= 10)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_BlueLamp)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_BlueLamp, 5);
		if (vOldBlueLampBrokenCounter >= 10)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
	}
	

	if (pWnd->GetDlgCtrlID() == IDC_SourceTemperature)
	if (vOldSourceTooHot)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		return vLocalSystemData->vRedBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SourceCoolantFlow)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SourceCoolantFlow, 5);
	if ((vOldSourceInterLocks & 0x0400) != 0x0400)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		return vLocalSystemData->vRedBrush;
	}
	}

	if (pWnd->GetDlgCtrlID() == IDC_FrontEndStatus)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FrontEndStatus, 5);
		if (vOldNoCommunicationsWithUController)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_XRayStatus)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRayStatus, 5);
		if (((vOldSourceInterLocks & 0x0010) != 0x0010) || (vHadSourceEvent))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_Interlocks)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Interlocks, 5);
	if (((vOldSourceInterLocks & 0x1B20) != 0x1B20))
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		return vLocalSystemData->vRedBrush;
	}
	}

	if (pWnd->GetDlgCtrlID() == IDC_uCStatus)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_uCStatus, 5);
		BYTE TempMask = 7;
		if (vGlobalFPGAVersion10Point0OrHigher)
			TempMask = 6; //ignore Red LED on 10.0 and higher
		if (((vOlduCLEDGYR & TempMask) != TempMask) || (!vOlduCTempOK) || (vOldPosteduCVoltageProblemMessage))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
	}

//	if (pWnd->GetDlgCtrlID() == IDC_uCResets)
//	if (vOlduCWatchDogCount > 1)
//	{
//		pDC->SetBkMode(TRANSPARENT);
//		pDC->SetTextColor(cWhite);
//		return vLocalSystemData->vRedBrush;
//	}

// 11/3/09 currently FPGA self test not monitored
//	if (pWnd->GetDlgCtrlID() == IDC_FPGASelfTest)
//	if (!vOldFPGASelfTest)
//	{
//		pDC->SetBkMode(TRANSPARENT);
//		pDC->SetTextColor(cWhite);
//		return vLocalSystemData->vRedBrush;
//	}


	if (pWnd->GetDlgCtrlID() == IDC_EncoderPercentOfMaximum)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EncoderPercentOfMaximum, 5);
		if (((vOldPercentBeltSpeed > 99) && 
			((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && ((vGlobalCurrentProduct) && (!vGlobalCurrentProduct->vBulkProductMode)) && (!vLocalSystemData->vFPGASimulateEncoderRate))) || //Not Continuous Feed, Not a Pipeline
			(vOldPercentBeltSpeed > 105)) //if over 105% and pipeline
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_EncoderPercentOfMaximumMaximum)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EncoderPercentOfMaximumMaximum, 5);
		if (((vOldPercentBeltSpeedMaximum > 99) && 
			((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && ((vGlobalCurrentProduct) && (!vGlobalCurrentProduct->vBulkProductMode)) && (!vLocalSystemData->vFPGASimulateEncoderRate))) || //Not Continuous Feed, Not a Pipeline
			(vOldPercentBeltSpeedMaximum > 105)) //if over 105% and pipeline
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
	}
	
	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ActualEncoderRateLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ActualEncoderRatelabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_uCStatusLabel)
	
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_uCStatusLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_FrontEndStatusLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FrontEndStatusLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XRayStatusLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRayStatusLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EncoderPercentOfMaximumLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EncoderPercentOfMaximumLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_BeltSpeedLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_BeltSpeedLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ContainersPerMinuteLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ContainersPerMinuteLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaximumLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaximumLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EncoderCountsOnLastTriggerLabel3) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EncoderCountsOnLastTriggerLabel3, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SpeedFactorLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SpeedFactorLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_CpuLoadImageThreadLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CpuLoadImageThreadLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	//if (pWnd->GetDlgCtrlID() == IDC_UnitsLabel) 
	//{
	//	TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_UnitsLabel, 5);
	//	pDC->SetBkMode(TRANSPARENT);
	//	return vLocalSystemData->vLightBrownBrush;
	//}
	if (pWnd->GetDlgCtrlID() == IDC_ActualCpuLoadLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ActualCpuLoadLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_FreeMemoryPercentageLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FreeMemoryPercentageLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_BlueLampLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_BlueLampLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_InterlocksLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_InterlocksLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_YellowLampLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YellowLampLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_RedLampLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_RedLampLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SourceCoolantFlowLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SourceCoolantFlowLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SourceTemperatureLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SourceTemperatureLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_FreeDiskSpacePercentageLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FreeDiskSpacePercentageLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_FreeDiskSpacePercentage) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FreeDiskSpacePercentage, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ContainersPerMinute)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ContainersPerMinute, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ContainersPerMinuteMaximum)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ContainersPerMinuteMaximum, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_uCStatus) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_uCStatus, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ActualEncoderRate) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ActualEncoderRate, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ActualEncoderRateMaximum) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ActualEncoderRateMaximum, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_FrontEndStatus) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FrontEndStatus, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XRayStatus) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRayStatus, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EncoderPercentOfMaximum) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EncoderPercentOfMaximum, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EncoderPercentOfMaximumMaximum) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EncoderPercentOfMaximumMaximum, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ProcessUsageT) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ProcessUsageT, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_BeltSpeed) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_BeltSpeed, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_BeltSpeedMaximum) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_BeltSpeedMaximum, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
		//11/5/2009
	if (pWnd->GetDlgCtrlID() == IDC_SpeedFactor) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SpeedFactor, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SpeedFactor1) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SpeedFactor1, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_CpuLoadImageThread) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CpuLoadImageThread, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ActualCpuLoad) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ActualCpuLoad, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_FreeMemoryPercentage)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FreeMemoryPercentage, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_BlueLamp)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_BlueLamp, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_YellowLamp)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YellowLamp, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_RedLamp) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_RedLamp, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Interlocks)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Interlocks, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_FBWF_ProtectedLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FBWF_ProtectedLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (!vGlobalFBWFEnable)
			return vLocalSystemData->vYellowBrush;

		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SourceCoolantFlow) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SourceCoolantFlow, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EncoderCountsOnLastTrigger3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EncoderCountsOnLastTrigger3, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SourceTemperature)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SourceTemperature, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
  }
	if (pWnd->GetDlgCtrlID() == IDC_XRaysOn)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaysOn, 7);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cYellow);
		pDC->SetBkColor(cRed);
		return vLocalSystemData->vRedBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DensityCompensation)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DensityCompensation, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
	}
	if (pWnd->GetDlgCtrlID() == IDC_ThresholdReduction)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ThresholdReduction, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
	}
	if (pWnd->GetDlgCtrlID() == IDC_ThresholdIncrease)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ThresholdIncrease, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function1Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5);  //5 is medium large
		if (vAllowExtraFunctions)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}

			pDC->SetBkMode(TRANSPARENT);
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
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function4Button, 5);  //5 is medium large
		
		if ((vLocalSystemData->vKeepExternalDetectorsEnabled) ||
			(!vLocalSystemData->vITIPCDig->vShowOnlyRealImages) || 
			(vLocalSystemData->vFPGAGeneratingSimulatedData) ||
			(vLocalSystemData->vFPGASimulateEncoderRate) ||
			(vLocalSystemData->vSimulateContainersRate))
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
			pDC->SetBkMode(TRANSPARENT);
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

		if (vLocalSystemData->vTemporarilyChangeSource)
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
		if ((vLocalSystemData->vuCSimulate16Hz) ||
			(vLocalSystemData->vuControllerSendExtraDiagnosticMessages) || 
			(vLocalSystemData->vuCOutputSerialDebugData) ||
			(vLocalSystemData->vLogFile.vLogSerialData) ||
			(!vLocalSystemData->vLogFile.vLogSyncData))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cSupervisorTextColor);
			return vLocalSystemData->vGreenBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		pDC->SetTextColor(cSupervisorTextColor);
		pDC->SetBkMode(TRANSPARENT);
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
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		if (vLocalSystemData->vTestingTrigger)
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction8ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vLocalSystemData->vRedBrush;
		}

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction8ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CNewDiagnosticDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
		
  if (nIDEvent == vUpdateDisplayTimerHandle)
		UpdateDisplay();
	/*
  if (nIDEvent == vTestTimerHandle)
	{
		KillTimer(vTestTimerHandle);
		::PostThreadMessage(vGlobalCallingThreadID,cTestMessage,0,0);
	}
	*/
	CDialog::OnTimer(nIDEvent);
}

void CNewDiagnosticDialog::UpdateDisplay()
{
	//br ht for office testing
	/*
	if (vLocalSystemData->vBodyTriggerWidthSamples < cMaximumBodyTriggerWidthData)
		vLocalSystemData->vBodyTriggerWidthSamples++;
	vLocalSystemData->vBodyTriggerWidthData[vLocalSystemData->vBodyTriggerWidthIndex] = 
		vLocalSystemData->vBodyTriggerWidthIndex;
	if (vLocalSystemData->vBodyTriggerWidthIndex + 1 >= cMaximumBodyTriggerWidthData)
		vLocalSystemData->vBodyTriggerWidthIndex = 0;
	else
		vLocalSystemData->vBodyTriggerWidthIndex++;

	if (vGlobalShiftKeyDown != vOldShiftKeyDown)
	{
		UpdateButtons();
		vOldShiftKeyDown = vGlobalShiftKeyDown;
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		TempWindow->Invalidate(false);
	}
*/
	if (vOldPercentOfTimeForProcessing != vLocalSystemData->vPercentOfTimeForProcessing)
	{
		vOldPercentOfTimeForProcessing = (WORD)vLocalSystemData->vPercentOfTimeForProcessing;
		SetDlgItemText(IDC_SpeedFactor, dtoa(vOldPercentOfTimeForProcessing, 0) + "%");
	}

	if (vOldPercentOfTimeBeforeEject != vLocalSystemData->vPercentOfTimeBeforeEject)
	{
		vOldPercentOfTimeBeforeEject = vLocalSystemData->vPercentOfTimeBeforeEject;
		SetDlgItemText(IDC_SpeedFactor1, dtoa(vOldPercentOfTimeBeforeEject, 0) + "%");
	}

	bool TempHadSourceEvent = 
		((vLocalSystemData->vSourceCurrentMin < vLocalSystemData->vSourceCurrentMinRedLine) ||
		(vLocalSystemData->vSourceVoltageMin < vLocalSystemData->vSourceVoltageMinRedLine) ||
		(vLocalSystemData->vSourceCurrentMax > vLocalSystemData->vSourceCurrentMaxRedLine) ||
		(vLocalSystemData->vSourceVoltageMax > vLocalSystemData->vSourceVoltageMaxRedLine));

	if ((vOldLastBodyTriggerLength != vLocalSystemData->vLastBodyTriggerLength) ||
		(vOldBodyTriggerWidthStandardDeviation != vLocalSystemData->vBodyTriggerWidthStandardDeviation) ||
		(vOldBodyTriggerWidthAverage != vLocalSystemData->vBodyTriggerWidthAverage))
	{
		vOldLastBodyTriggerLength = vLocalSystemData->vLastBodyTriggerLength;
		vOldBodyTriggerWidthAverage = vLocalSystemData->vBodyTriggerWidthAverage;
		vOldBodyTriggerWidthStandardDeviation = vLocalSystemData->vBodyTriggerWidthStandardDeviation;
		CString TempString = dtoa(vOldLastBodyTriggerLength,0) + ", A: " + dtoa(vLocalSystemData->vBodyTriggerWidthAverage,1) + ", S: " + dtoa(vLocalSystemData->vBodyTriggerWidthStandardDeviation,1);
		SetDlgItemText(IDC_EncoderCountsOnLastTrigger3,TempString);
	}

	if (vGlobalDXMXRayPort)
	{
		//ignore the 24 volt monitor (stored in 15v monitor variable) on Cheyney monoblock because of a firmware error in the Cheyney Monoblock
		if ((vGlobalDXMXRayPort->vNoCommunications) || (!vGlobalDXMXRayPort->vRemoteMode) || ((vGlobalDXMXRayPort->v15VoltMonitor < 11) && (vLocalConfigurationData->vHVPSType != cCheyneyMonoBlock))) //-15 volt HVPS supply below 11 volts
		if (!vLocalConfigurationData->vSimulateLightControlBoard)
			TempHadSourceEvent = true;
	}
	else
	if (vGlobalMonoBlockXRayPort)
	{
		if (vGlobalMonoBlockXRayPort->vNoCommunications)
			TempHadSourceEvent = true;
	}

	if (vHadSourceEvent != TempHadSourceEvent)
	{
		CString TempString;
		vHadSourceEvent = TempHadSourceEvent;
		if (((vOldSourceInterLocks & 0x0010) != 0x0010) || (vHadSourceEvent))
			TempString.LoadString(IDS_Fault);
		else
			TempString.LoadString(IDS_OK);
		SetDlgItemText(IDC_XRayStatus,TempString);
	}

	if (vOldGlobalPasswordOK != PasswordOK(cTemporaryInspxPassword,false))
	{
		vOldGlobalPasswordOK = PasswordOK(cTemporaryInspxPassword,false);
		ShowInspxItems();
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
		UpdateDisplay();
	}

	MEMORYSTATUS memstat;
	GlobalMemoryStatus(&memstat);
	vGlobalAmountOfFreeMemory = memstat.dwAvailPageFile;
	FLOAT vTempAvailMemory = (FLOAT)(memstat.dwAvailPageFile);
	FLOAT vTempTotalMemory = (FLOAT)(memstat.dwTotalPageFile);
	FLOAT vFLOATPercentageOfFreeMemory = (((vTempAvailMemory * 1000L)/vTempTotalMemory) + 5L)/10L ;
	vPercentageOfFreeMemory = (UINT)(vFLOATPercentageOfFreeMemory);

	CString TempString("");
	TempString = "";
	CString TempString1("");
	TempString1 = "";

	if (vOldPercentBeltSpeed != vLocalSystemData->vPercentBeltSpeed)
	{
		vOldPercentBeltSpeed = vLocalSystemData->vPercentBeltSpeed;
		TempString = dtoa(vOldPercentBeltSpeed,1);
		SetDlgItemText(IDC_EncoderPercentOfMaximum,TempString);
	}

	if (vOldPercentBeltSpeedMaximum != vLocalSystemData->vPercentBeltSpeedMaximum)
	{
		vOldPercentBeltSpeedMaximum = vLocalSystemData->vPercentBeltSpeedMaximum;
		TempString = dtoa(vOldPercentBeltSpeedMaximum,1);
		SetDlgItemText(IDC_EncoderPercentOfMaximumMaximum,TempString);
	}

	if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
	{
		if (vLocalSystemData->vContainerRate != vOldContainerRate)
		{
			vOldContainerRate = vLocalSystemData->vContainerRate;
			SetDlgItemText(IDC_ContainersPerMinute,IToAWithComma(vOldContainerRate));
		}
	}
	else
	{
		if ((vMainWindowPointer->vOldTriggerIndicatorCount != vMainWindowPointer->vTriggerIndicatorCount) ||
			(vOldCPMNotRunning != vMainWindowPointer->vCPMNotRunning))
		{
			vMainWindowPointer->vOldTriggerIndicatorCount = vMainWindowPointer->vTriggerIndicatorCount;
			vOldCPMNotRunning = vMainWindowPointer->vCPMNotRunning;
			if (vOldCPMNotRunning)
				SetDlgItemText(IDC_ContainersPerMinute,dtoa(vOldCPMNotRunning,0));
			else
				SetDlgItemText(IDC_ContainersPerMinute, _T("0"));
		}
	}

	if (vLocalSystemData->vContainerRateMaximum != vOldContainerRateMaximum)
	{
		vOldContainerRateMaximum = vLocalSystemData->vContainerRateMaximum;
		SetDlgItemText(IDC_ContainersPerMinuteMaximum,IToAWithComma(vOldContainerRateMaximum));
	}

	if (vOldBeltRate != vLocalSystemData->vBeltRate)
	{
		vOldBeltRate = vLocalSystemData->vBeltRate;
		if (((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac)) &&
			(!vLocalConfigurationData->vUseRealEncoder) && //using the motor signal so must increase 16X
			((vLocalSystemData->vSystemRunMode == cStoppedSystemMode) || (vLocalSystemData->vSystemRunMode == cConditionXRayTubeMode)) &&
			(!vLocalSystemData->vFPGASimulateEncoderRate) && (!vLocalConfigurationData->vContinuousFeedEncoderRate))
		{
			if (vLocalConfigurationData->vPixelsPerDetector == 128)
				SetDlgItemText(IDC_BeltSpeed, dtoa(vOldBeltRate / 12 * 32,0));
			else
				SetDlgItemText(IDC_BeltSpeed, dtoa(vOldBeltRate / 12 * 16,0));
		}
		//else
		//if (vLocalConfigurationData->vUseMetric)
		//	SetDlgItemText(IDC_BeltSpeed, dtoa(vOldBeltRate / 1000, 0));
		else
			SetDlgItemText(IDC_BeltSpeed, dtoa(vOldBeltRate / 12,0));
	}

	if (vOldBeltRateMaximum != vLocalSystemData->vBeltRateMaximum)
	{
		vOldBeltRateMaximum = vLocalSystemData->vBeltRateMaximum;
			SetDlgItemText(IDC_BeltSpeedMaximum,dtoa(vOldBeltRateMaximum / 12, 0));
	}
	if (vOldXRayOnNotice != vLocalSystemData->vXRayOnNotice)
	{
		vOldXRayOnNotice = vLocalSystemData->vXRayOnNotice;
		if (vOldXRayOnNotice)
			m_XRaysOn.ShowWindow(SW_SHOW);
		else
			m_XRaysOn.ShowWindow(SW_HIDE);
		UpdateButtons();
	}

	int TempTemp = ConvertToDegreesC(vLocalSystemData->vTemperatureuC,
		vLocalConfigurationData->vTemperatureuCAdjust);

	vuCTempOK = (TempTemp <= vLocalConfigurationData->vMaximumuCTemperature);

	if ((vOlduCLEDGYR != vLocalSystemData->vuCLEDGYR) || (vOlduCTempOK != vuCTempOK) || (vLocalSystemData->vPosteduCVoltageProblemMessage != vOldPosteduCVoltageProblemMessage))
	{
		vOldPosteduCVoltageProblemMessage = vLocalSystemData->vPosteduCVoltageProblemMessage;
		vOlduCTempOK = vuCTempOK;
		vOlduCLEDGYR = vLocalSystemData->vuCLEDGYR;
		BYTE TempMask = 7;
		if (vGlobalFPGAVersion10Point0OrHigher)
			TempMask = 6; //ignore Red LED on 10.0 and higher

		if (((vOlduCLEDGYR & TempMask) != TempMask) || (!vOlduCTempOK) || (vLocalSystemData->vPosteduCVoltageProblemMessage))
			TempString.LoadString(IDS_Fault);
		else
			TempString.LoadString(IDS_OK);
		SetDlgItemText(IDC_uCStatus, TempString);
	}

	if (vOldNoCommunicationsWithUController != vLocalSystemData->vNoCommunicationsWithUController)
	{
		vOldNoCommunicationsWithUController = vLocalSystemData->vNoCommunicationsWithUController;
		if (!vLocalSystemData->vACPowerOn)
			TempString.LoadString(IDS_ShuttingDown);
		else
		if (vOldNoCommunicationsWithUController)
			TempString.LoadString(IDS_Fault);
		else
			TempString.LoadString(IDS_OK);
		SetDlgItemText(IDC_FrontEndStatus,TempString);
	}
	if ((PasswordOK(cTemporaryInspxPassword,false)) && (!vLocalConfigurationData->vUseRealEncoder))
	{
		if ((vOldActualEncoderRate != vLocalSystemData->vActualEncoderRate) || (vOldConveyorEncoderRate != vLocalSystemData->vConveyorEncoderRate))
		{
			vOldActualEncoderRate = vLocalSystemData->vActualEncoderRate;
			vOldConveyorEncoderRate = vLocalSystemData->vConveyorEncoderRate;
			SetDlgItemText(IDC_ActualEncoderRate,IToAWithComma(vOldActualEncoderRate) + ", " + IToAWithComma(vOldConveyorEncoderRate) + ", " + IToAWithComma(vOldConveyorEncoderRate * 16));
		}
	}
	else
	if (vOldActualEncoderRate != vLocalSystemData->vActualEncoderRate)
	{
		vOldActualEncoderRate = vLocalSystemData->vActualEncoderRate;
		TempString1.LoadString(IDS_Hz);
		SetDlgItemText(IDC_ActualEncoderRate,IToAWithComma(vOldActualEncoderRate) + TempString1);
	}
	if (vOldActualEncoderRateMaximum != vLocalSystemData->vActualEncoderRateMaximum)
	{
		vOldActualEncoderRateMaximum = vLocalSystemData->vActualEncoderRateMaximum;
		TempString1.LoadString(IDS_Hz);
		SetDlgItemText(IDC_ActualEncoderRateMaximum,
			IToAWithComma(vOldActualEncoderRateMaximum) + TempString1);
	}

	if (vOldSourceInterLocks != vLocalSystemData->vSourceInterLocks)
	{
		vOldSourceInterLocks = vLocalSystemData->vSourceInterLocks;
		{
			if (!vLocalSystemData->vACPowerOn)
				TempString.LoadString(IDS_ShuttingDown);
			else
			if (vOldNoCommunicationsWithUController)
				TempString.LoadString(IDS_Fault);
			else
				TempString.LoadString(IDS_OK);
			SetDlgItemText(IDC_FrontEndStatus,TempString);

			if (((vOldSourceInterLocks & 0x0010) != 0x0010) || (vHadSourceEvent))
				TempString.LoadString(IDS_Fault);
			else
				TempString.LoadString(IDS_OK);
			SetDlgItemText(IDC_XRayStatus,TempString);

			if (((vOldSourceInterLocks & 0x1B20) != 0x1B20))
				TempString.LoadString(IDS_Fault);
			else
				TempString.LoadString(IDS_OK);
			SetDlgItemText(IDC_Interlocks,TempString);
		}

		if ((vOldSourceInterLocks & 0x0400) == 0x0400)
			TempString.LoadString(IDS_OK);
		else
		{
			if (vLocalConfigurationData->vUseExternalInterlock)
			TempString.LoadString(IDS_Open);
			else
			TempString.LoadString(IDS_Fault);
		}
		SetDlgItemText(IDC_SourceCoolantFlow,TempString);
	}

	if (vOldRedLampBrokenCounter != vLocalSystemData->vRedLampBrokenCounter)
	{
		vOldRedLampBrokenCounter = vLocalSystemData->vRedLampBrokenCounter;
		if (vOldRedLampBrokenCounter >= 10)
			TempString.LoadString(IDS_Fault);
		else
			TempString.LoadString(IDS_OK);
		SetDlgItemText(IDC_RedLamp,TempString);
	}

	if (vOldYellowLampBrokenCounter != vLocalSystemData->vYellowLampBrokenCounter)
	{
		vOldYellowLampBrokenCounter = vLocalSystemData->vYellowLampBrokenCounter;
		if (vOldYellowLampBrokenCounter >= 10)
			TempString.LoadString(IDS_Fault);
		else
			TempString.LoadString(IDS_OK);
		SetDlgItemText(IDC_YellowLamp,TempString);
	}

	if (vOldBlueLampBrokenCounter != vLocalSystemData->vBlueLampBrokenCounter)
	{
		vOldBlueLampBrokenCounter = vLocalSystemData->vBlueLampBrokenCounter;
		if (vOldBlueLampBrokenCounter >= 10)
			TempString.LoadString(IDS_Fault);
		else
			TempString.LoadString(IDS_OK);
		SetDlgItemText(IDC_BlueLamp,TempString);
	}

	int TempSurface = ConvertToDegreesC(vLocalSystemData->vTemperatureSurfaceMaximum,
		vLocalConfigurationData->vTemperatureSurfaceAdjust);
	int TempAir = ConvertToDegreesC(vLocalSystemData->vTemperatureAirMaximum,
		vLocalConfigurationData->vTemperatureAirInsideAdjust);
	int TempLiquid = ConvertToDegreesC(vLocalSystemData->vTemperatureLiquidMaximum,
		vLocalConfigurationData->vTemperatureLiquidAdjust);

	vSourceTooHot = (
		(TempLiquid > vLocalConfigurationData->vMaximumLiquidTemperature) ||
		(TempAir > vLocalConfigurationData->vMaximumAirTemperature) || 
		(TempSurface > vLocalConfigurationData->vMaximumSurfaceTemperature));

	if (vGlobalFPGAVersion10Point0OrHigher)
	{
		if (!vSourceTooHot)
		{
			int TempLiquid = ConvertToDegreesC(vLocalSystemData->vTemperatureAirOutside,
				vLocalConfigurationData->vTemperatureAirOutsideAdjust);
			//int TempPressure = ConvertToPressurePSI(vLocalSystemData->vCoolantPressure,0);
				//vLocalConfigurationData->vCoolantPressureAdjust);

			vSourceTooHot = //((TempPressure > vLocalConfigurationData->vMaximumCoolantPressure) ||
				(TempLiquid > vLocalConfigurationData->vMaximumTemperatureAirOutside);
		}
	}

	if (vOldSourceTooHot != vSourceTooHot)
	{
		vOldSourceTooHot = vSourceTooHot;
		if (vOldSourceTooHot)
			TempString.LoadString(IDS_Fault);
		else
			TempString.LoadString(IDS_OK);
		SetDlgItemText(IDC_SourceTemperature,TempString);
	}
// kjh 11/03/2009, replace FPGA Self Test notification with CPU usage display
//	vLocalSystemData->vCpuUsage = FindCPUUsage(); done in OneSecondDisplayUpdate()
	if (vOldCpuUsage != vLocalSystemData->vCpuUsage)
	{
		SetDlgItemText(IDC_ActualCpuLoad, dtoa(vLocalSystemData->vCpuUsage) + "%");
		vOldCpuUsage = vLocalSystemData->vCpuUsage;
	}
	if (vOld_ThreadTimePercent != vGlobalThreadTimePercent)
	{
		vOld_ThreadTimePercent = vGlobalThreadTimePercent;
		SetDlgItemText(IDC_CpuLoadImageThread, dtoa(vGlobalThreadTimePercent) + "%");
	}
	if (vOld_ProcessTimePercent != vGlobalProcessTimePercent)
	{
		vOld_ProcessTimePercent = vGlobalProcessTimePercent;
		SetDlgItemText(IDC_ProcessUsageT, dtoa(vGlobalProcessTimePercent) + "%");
	}

//	if (vOldFPGASelfTest != vLocalSystemData->vFPGASelfTest)
//	{
//		vOldFPGASelfTest = vLocalSystemData->vFPGASelfTest;
//		if (vOldFPGASelfTest)
//			TempString.LoadString(IDS_Passed);
//		else
//			TempString.LoadString(IDS_Failed);
//		SetDlgItemText(IDC_FPGASelfTest,TempString);
//	}
	if ((vOldPercentageOfFreeMemory != vPercentageOfFreeMemory) || ((abs((long)(vGlobalAmountOfFreeMemory - vOldAmountOfFreeMemory))) > 1000000))
	{
		vOldPercentageOfFreeMemory = vPercentageOfFreeMemory;
		vOldAmountOfFreeMemory = vGlobalAmountOfFreeMemory;
		CString MemoryTempCString;
		MemoryTempCString.Format(_T("%u%%  "),vPercentageOfFreeMemory);

		//2009/11/12
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			if (vGlobalAmountOfFreeMemory >= 0x40000000)
				MemoryTempCString += (dtoaWithCommas(vGlobalAmountOfFreeMemory / 0x40000000, 0) + " GB");
			else
			if (vGlobalAmountOfFreeMemory >= 0x100000)
				MemoryTempCString += (dtoaWithCommas(vGlobalAmountOfFreeMemory / 0x100000, 0) + " MB");
			else
				MemoryTempCString += (dtoaWithCommas(vGlobalAmountOfFreeMemory, 0) + " Bytes");
		}
		SetDlgItemText(IDC_FreeMemoryPercentage,MemoryTempCString);
	}

	if ((PasswordOK(cTemporaryInspxPassword,false)) && (vLocalConfigurationData->vDriftCompensationEnabled))
	{
		if ((vOldDriftCompensationADCOffset != vLocalSystemData->vDriftCompensationADCOffset) ||
			(vOldDriftCompensationDACOffset != vLocalSystemData->vDriftCompensationDACOffset))
		{
			vOldDriftCompensationADCOffset = vLocalSystemData->vDriftCompensationADCOffset;
			vOldDriftCompensationDACOffset = vLocalSystemData->vDriftCompensationDACOffset;
			CString TempSign = "";
			if (vLocalSystemData->vCurrentBrightnessAdjustment > 0)
				TempSign = "+";
			SetDlgItemText(IDC_DensityCompensation,"Corr: " + TempSign + dtoa(vLocalSystemData->vCurrentBrightnessAdjustment,0));
/*			if (vLocalConfigurationData->vUseDACAndADCinDensityCompensation)
			{
				SetDlgItemText(IDC_DensityCompensation,"DO: " + 
					dtoa(vOldDriftCompensationADCOffset,0) + "mV " +
					dtoa(vOldDriftCompensationDACOffset,0) + "DAC");  //show in pixel intensity
			}
			else
				SetDlgItemText(IDC_DensityCompensation,"DO: " + 
					dtoa(vOldDriftCompensationADCOffset,0));  //show in pixel intensity
					*/
		}
		/*
		if (vOldDriftCompensationThresholdReductionAmount != 
			vLocalSystemData->vDriftCompensationThresholdReductionAmount)
		{
			vOldDriftCompensationThresholdReductionAmount = 
				vLocalSystemData->vDriftCompensationThresholdReductionAmount;
			CString TempString = dtoa(vOldDriftCompensationThresholdReductionAmount,2);
			if (vOldDriftCompensationThresholdReductionAmount)
				TempString = "TC: -" + TempString;
			else
				TempString = "TC: " + TempString;
			SetDlgItemText(IDC_ThresholdReduction,TempString);
		}
		if (vOldDriftCompensationThresholdIncreaseAmount != 
			vLocalSystemData->vDriftCompensationThresholdIncreaseAmount)
		{
			vOldDriftCompensationThresholdIncreaseAmount = 
				vLocalSystemData->vDriftCompensationThresholdIncreaseAmount;
			CString TempString = dtoa(vOldDriftCompensationThresholdIncreaseAmount,2);
			TempString = "TC: " + TempString;
			SetDlgItemText(IDC_ThresholdIncrease,TempString);
		}
		*/
	}

}

void CNewDiagnosticDialog::UpdateButtons()
{
	CString TempText;
	if (vLocalSystemData->vTestingTrigger)
	{
		vSubFunction8ButtonEnable = true;	//m_SubFunction8Button.EnableWindow(true);
		TempText.LoadString(IDS_StopTriggerTest);
		SetDlgItemText(IDC_SubFunction8Button,TempText);
	}
	else
	{
		if ((vLocalSystemData->vSystemRunMode == cStoppedSystemMode) && (vGlobalCurrentProduct))
			vSubFunction8ButtonEnable = true;	//m_SubFunction8Button.EnableWindow(true);
		else
			vSubFunction8ButtonEnable = false;	//m_SubFunction8Button.EnableWindow(false);
		TempText.LoadString(IDS_StartTriggerTest);
		SetDlgItemText(IDC_SubFunction8Button,TempText);
	}
}

void CNewDiagnosticDialog::ShowInspxItems()
{
	//2009/11/12
	vOldPercentageOfFreeMemory = 666; //force update of free memory display

	//2009/11/12
// Hard Drive space check
	vHardDriveSpaceFree = CheckDiskSpaceFree("C:\\"); //	double vGlobalHardDriveTotalBytes set in function call

	if (vGlobalHardDriveTotalBytes != 0)
		vPercentageOfDiskSpaceFree = (UINT)((((vHardDriveSpaceFree * 1000L) / vGlobalHardDriveTotalBytes) + 5L) / 10L);
	else
		vPercentageOfDiskSpaceFree = -1;
	CString TempHardDrivePercentageString;
	TempHardDrivePercentageString.Format (_T("%u%%  "),vPercentageOfDiskSpaceFree);

	//2009/11/12
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		if (vHardDriveSpaceFree >= 0x40000000)
			TempHardDrivePercentageString += (dtoaWithCommas(vHardDriveSpaceFree / 0x40000000, 0) + " GB");
		else
		if (vHardDriveSpaceFree >= 0x100000)
			TempHardDrivePercentageString += (dtoaWithCommas(vHardDriveSpaceFree / 0x100000, 0) + " MB");
		else
			TempHardDrivePercentageString += (dtoaWithCommas(vHardDriveSpaceFree, 0) + " Bytes");
	}
	SetDlgItemText(IDC_FreeDiskSpacePercentage, TempHardDrivePercentageString);

	/*
	m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	SetDlgItemText(IDC_Function2Button,"uC One Extra Work!");
	m_Function2Button.ShowWindow(SW_SHOW);
	m_ArrowDown2.ShowWindow(SW_SHOW);

	SetDlgItemText(IDC_Function3Button,"uC Health Factor!");
	m_Function3Button.ShowWindow(SW_SHOW);
	m_ArrowDown3.ShowWindow(SW_SHOW);

	m_Function4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	SetDlgItemText(IDC_Function4Button,"uC Extra Work!");
	m_Function4Button.ShowWindow(SW_SHOW);
	m_ArrowDown4.ShowWindow(SW_SHOW);
	*/
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		SetDlgItemText(IDC_Function4Button, _T("Test Modes\nMenu"));
		m_Function4Button.ShowWindow(SW_SHOW);

		SetDlgItemText(IDC_CpuLoadImageThreadLabel, _T("Image Processing CPU Load:"));		
		if (vGlobalPCRunningWindowsXP >= cWindowsXP)
		{
			m_ProcessUsageT.ShowWindow(SW_SHOW);
			m_ActualCpuLoadLabel.ShowWindow(SW_SHOW);
			m_ActualCpuLoad.ShowWindow(SW_SHOW);
			m_CpuLoadImageThreadLabel.ShowWindow(SW_SHOW);
			m_CpuLoadImageThread.ShowWindow(SW_SHOW);
		}
		//2009/11/12
		//m_FreeMemoryPercentage.ShowWindow(SW_SHOW);
		//m_FreeMemoryPercentageLabel.ShowWindow(SW_SHOW);
		//m_FreeDiskSpacePercentage.ShowWindow(SW_SHOW);
		//m_FreeDiskSpacePercentageLabel.ShowWindow(SW_SHOW);
		//m_FreeTime.ShowWindow(SW_SHOW);
		vOldDriftCompensationADCOffset = 999999;
		vOldDriftCompensationDACOffset = 999999;
		//vOldDriftCompensationThresholdReductionAmount = 999999;
		//vOldDriftCompensationThresholdIncreaseAmount = 999999;
	}
	else
	{
		m_Function4Button.ShowWindow(SW_SHOW);
		CString TempText;
		TempText.LoadString(IDS_TestFireEjectorNow);
		SetDlgItemText(IDC_Function4Button,TempText);

		SetDlgItemText(IDC_DensityCompensation, _T(""));
		//SetDlgItemText(IDC_ThresholdReduction,"");
		//SetDlgItemText(IDC_ThresholdIncrease,"");

		TempText.LoadString(IDS_CPULoad);
		SetDlgItemText(IDC_CpuLoadImageThreadLabel, TempText + ":");		

		TempText.LoadString(IDS_SpeedFactor);
		SetDlgItemText(IDC_SpeedFactorLabel, TempText + ":");		

		m_ProcessUsageT.ShowWindow(SW_HIDE);
		m_ActualCpuLoadLabel.ShowWindow(SW_HIDE);
		m_ActualCpuLoad.ShowWindow(SW_HIDE);
		if (vGlobalPCRunningWindowsXP >= cWindowsXP)
		{
			m_CpuLoadImageThreadLabel.ShowWindow(SW_SHOW);
			m_CpuLoadImageThread.ShowWindow(SW_SHOW);
		}
		//2009/11/12
		//m_FreeMemoryPercentage.ShowWindow(SW_HIDE);
		//m_FreeMemoryPercentageLabel.ShowWindow(SW_HIDE);
		//m_FreeDiskSpacePercentage.ShowWindow(SW_HIDE);
		//m_FreeDiskSpacePercentageLabel.ShowWindow(SW_HIDE);
		//m_FreeTime.ShowWindow(SW_HIDE);
	}
}

void CNewDiagnosticDialog::PrepareToExit()
{
	if (vLocalSystemData->vTestingTrigger)
	{
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText = _T("\n\nTesting Triggers, Exit Testing Triggers?");
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		TempYesNoDialog.vYesButtonText = "Exit Test";
		TempYesNoDialog.vNoButtonText = "Continue Test";
		int TempResult = TempYesNoDialog.DoModal();
		//if user said OK, 
		if (TempResult == IDOK)
			OnSubFunction8Button();
	}
}

void CNewDiagnosticDialog::OnDialogTitleStaticText() 
{
	// TODO: Add your control notification handler code here
	
	MEMORYSTATUS memstat;
	GlobalMemoryStatus(&memstat);

	CNoticeDialog TempNoticeDialog;
	CString TempText = "TotalPhys: " + dtoaWithCommas(memstat.dwTotalPhys,2);
	TempText = TempText + "\ndwAvailPhys: " + dtoaWithCommas(memstat.dwAvailPhys,2);
	TempText = TempText + "\ndwTotalPageFile: " + dtoaWithCommas(memstat.dwTotalPageFile,2);
	TempText = TempText + "\ndwAvailPageFile: " + dtoaWithCommas(memstat.dwAvailPageFile,2);
	TempText = TempText + "\ndwTotalVirtual: " + dtoaWithCommas(memstat.dwTotalVirtual,2);
	TempText = TempText + "\ndwAvailVirtual: " + dtoaWithCommas(memstat.dwAvailVirtual,2);
	//TempText = TempText + ",  dwMemoryLoad: " + dtoaWithCommas(memstat.dwMemoryLoad,2);

	TempNoticeDialog.vNoticeText = TempText;
	TempNoticeDialog.vType = cInformationMessage;
	TempNoticeDialog.DoModal();

	//dwLength Size, in bytes, of the MEMORYSTATUS data structure. You do not need to set this member before calling the GlobalMemoryStatus function; the function sets it. 
	//dwMemoryLoad Windows NT 3.1 – NT 4.0: The percentage of approximately the last 1000 pages of physical memory that is in use. 

	//dwTotalPhys:Total size, in bytes, of physical memory. 
	//dwAvailPhys:Size, in bytes, of physical memory available. 
	//dwTotalPageFile:Total possible size, in bytes, of the paging file. Note that this number does not represent the actual physical size of the paging file on disk. 
	//dwAvailPageFile:Size, in bytes, of space available in the paging file. The operating system can enlarge the paging file from time to time. The dwAvailPageFile member shows the difference between the size of current committed memory and the current size of the paging file — it does not show the largest possible size of the paging file. 
	//dwTotalVirtual:Total size, in bytes, of the user mode portion of the virtual address space of the calling process. 
	//dwAvailVirtual:Size, in bytes, of unreserved and uncommitted memory in the user mode portion of the virtual address space of the calling process. 
}


void CNewDiagnosticDialog::OnDialogtitlestatictext1()
{
	if (vGlobalShiftKeyDown)
		OnDialogTitleStaticText();
	else
	{
		BYTE TempHiddenKeys = 8;
		if (!PasswordOK(cTemporaryInspxPassword,false))
			TempHiddenKeys = 10;
		PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
	}
}
