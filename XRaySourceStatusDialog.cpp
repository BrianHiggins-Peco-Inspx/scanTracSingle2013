//ScanTrac Side View Source File
// XRaySourceStatusDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "XRaySourceStatusDialog.h"
#include "SerialPort.h"
#include "PasswordEntryDialog.h"
#include "NumericEntryDialog.h"
#include "InspxHiddenXRayDialog.h"
#include "YesNoDialog.h"
#include "HistogramDisplayDialog.h"
#include "SpellmanDXMXRaySourcePort.h"
#include "AlphaEntryDialog.h"
#include "HelpDialog.h"
#include "SelectItemDialog.h"
#include "XRayTubeAlignmentDialog.h"
#include "XRaySource.h"
#include "Spellman100WattMonoBlock.H"
#include "SerialPortGuardMaster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXRaySourceStatusDialog dialog

extern CSerialPortGuardMaster *vGlobalGuardMaster;
extern BYTE vGlobalScanTracType;
extern bool vGlobalFPGAVersion10Point0OrHigher;
extern CXRaySource *vGlobalMonoBlockXRayPort;
extern CSpellmanDXMXRaySourcePort *vGlobalDXMXRayPort;
extern BYTE vGlobalShiftKeyDown;
extern CSerialPort *vGlobaluCSerialPort;
extern CProduct *vGlobalCurrentProduct;
extern CLifeTimeCounters *vGlobalLifeTimeCounters;
extern int vGlobalPasswordLevel;
extern unsigned long vGlobalDialogTitleColor;

CXRaySourceStatusDialog::CXRaySourceStatusDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CXRaySourceStatusDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXRaySourceStatusDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	vOldSourceFault = "-";
	vOldlGuardMasterNoCommunications = false;
	vOldGuardMasterFaultLEDData = 0;
	vOldDetectedValidGuardMasterDevice = 0;
	vShowingYellowMessage = false;
	vCoolantTemperatureMaximum = "0";
	vOldXRayNoCommuncations = false;
	vOldYellowMessageButtonYellow = false;
	vDarkCurrentCurrentLevel = 0;
	vDarkCurrent = 0;
	vTakingDarkCurrentMeasurment = false;
	vDarkCurrentThreshold = 1.5;
	vOldGlobalShiftKeyDown = 0;
	vOldErrorMessage = "None";

	vCyclingShutter = false;
	vOldTemperatureSurface = 0xFFFE;
	vOldTemperatureLiquid = 0xFFFE;
	vOldTemperatureAir = 0xFFFE;
	vOldTemperatureAir = 0xFFFE;
	vOldTemperatureuC = 0xFFFE;

	//vOldCoolantPressure = 1;

	vOldTemperatureSurfaceMaximum = 1;
	vOldTemperatureLiquidMaximum = 1;
	vOldTemperatureAirMaximum = 1;
	vOldTemperatureAirOutsideMaximum = 1;
	vOldTemperatureuCMaximum = 1;
	//vOldCoolantPressureMaximum = 1;

	vOldTemperatureSurfaceMinimum = 0xFFFE;
	vOldTemperatureLiquidMinimum = 0xFFFE;
	vOldTemperatureAirMinimum = 0xFFFE;
	vOldTemperatureAirOutsideMinimum = 0xFFFE;
	vOldTemperatureuCMinimum = 0xFFFE;
	vOldCPUTemperature = 0xFFFE;
	vOldCPUTemperatureMinimum = 0xFFFE;
	vOldCPUTemperatureMaximum = 0xFFFE;
	//vOldCoolantPressureMinimum = 0xFFFE;

	vOldDXMStatus = "";
	vOldDXM15VStatus = 0;
	vOldFilamentPreHeatMonitor = 0;
	vTestLampsOn = 0;
	vOldRadiationDoorManualControlRegister = cRadiationShutterAutoControl;
	vMakeExitButtonGreen = false;
	vMainWindowPointer = NULL;
	vTestingLamp = 0;
	vProductPasswordOK = false;
	vDisplayTimerHandle = 77;
	vResetMinMaxAfterStablizeTimerHandle = 178;
	vDarkCurrentTimerHandle = 179;
	vCycleShutterTimerHandle = 507;
	vOldXRayOnNotice = false;
	vOldNoCommunicationsWithUController = true;
	vOldLastXRayCommandOn = 0xAA;
	vOldRampXRayVoltageAmount = 0;
	vOldRampXRayCurrentAmount = 0;

	vOldSourceVoltageMin = 254.2; //for it to update the screen
	vOldSourceVoltageMax = -1;
	vOldSourceCurrentMin = 254.2;
	vOldSourceCurrentMax = -1;
			//9/11/2009
	//vOriginalEjectorsEnabled = true;
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_ShutterStatusLabel);
	vLocalCWndCollection.Add(&m_ShutterStatus);
	vLocalCWndCollection.Add(&m_MinRunningFilamentCurrent);
	vLocalCWndCollection.Add(&m_MaxRunningFilamentCurrent);
	vLocalCWndCollection.Add(&m_TemperatureLiquidMinimum);
	vLocalCWndCollection.Add(&m_TemperatureLiquidMaximum);
	vLocalCWndCollection.Add(&m_ShutterTestButton);
	vLocalCWndCollection.Add(&m_ChangedFilterButton);
	vLocalCWndCollection.Add(&m_StatusLine);
	vLocalCWndCollection.Add(&m_TemperatureAirOutsideMinimum);
	vLocalCWndCollection.Add(&m_TemperatureAirOutsideMaximum);
	vLocalCWndCollection.Add(&m_TemperatureAirOutsideLabel);
	vLocalCWndCollection.Add(&m_TemperatureAirOutside);
	vLocalCWndCollection.Add(&m_HVPSPreHeatLabel);
	vLocalCWndCollection.Add(&m_HVPSPreHeat);
	vLocalCWndCollection.Add(&m_HVPS15VLabel);
	vLocalCWndCollection.Add(&m_HVPS15V);
	vLocalCWndCollection.Add(&m_HVPSLabel);
	vLocalCWndCollection.Add(&m_HVPS);
	vLocalCWndCollection.Add(&m_VoltageSetting2);
	vLocalCWndCollection.Add(&m_CurrentSetting2);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_OnOffLabel);
	vLocalCWndCollection.Add(&m_OnOff);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_TemperatureLiquid);
	vLocalCWndCollection.Add(&m_TemperatureLiquidLabel);
	vLocalCWndCollection.Add(&m_CoolantFlow);
	vLocalCWndCollection.Add(&m_CoolantFlowLabel);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_SourceInterlocksLabel);
	vLocalCWndCollection.Add(&m_SourceInterlocks);
	vLocalCWndCollection.Add(&m_SourceFaultLabel);
	vLocalCWndCollection.Add(&m_SourceFault);
	vLocalCWndCollection.Add(&m_XRaysOn);
	vLocalCWndCollection.Add(&m_Background1);
	vLocalCWndCollection.Add(&m_TemperatureCPU);
	vLocalCWndCollection.Add(&m_TemperatureCPULabel);
	vLocalCWndCollection.Add(&m_TemperatureCPUMaximum);
	vLocalCWndCollection.Add(&m_TemperatureCPUMinimum);
	vLocalCWndCollection.Add(&m_TemperatureuC);
	vLocalCWndCollection.Add(&m_TemperatureuCLabel);
	vLocalCWndCollection.Add(&m_TemperatureuCMaximum);
	vLocalCWndCollection.Add(&m_TemperatureuCMinimum);
	vLocalCWndCollection.Add(&m_Current);
	vLocalCWndCollection.Add(&m_CurrentLabel);
	vLocalCWndCollection.Add(&m_CurrentSetting);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_EStop);
	vLocalCWndCollection.Add(&m_EStopLabel);
	vLocalCWndCollection.Add(&m_KeySwitch);
	vLocalCWndCollection.Add(&m_KeySwitchLabel);
	vLocalCWndCollection.Add(&m_LimitsLabel);
	vLocalCWndCollection.Add(&m_MaxCurrent);
	vLocalCWndCollection.Add(&m_MaximumLabel);
	vLocalCWndCollection.Add(&m_MaxLabel);
	vLocalCWndCollection.Add(&m_MaxRunningCurrent);
	vLocalCWndCollection.Add(&m_MaxRunningVoltage);
	vLocalCWndCollection.Add(&m_MaxVLabel);
	vLocalCWndCollection.Add(&m_MaxVoltage);
	vLocalCWndCollection.Add(&m_MinCurrent);
	vLocalCWndCollection.Add(&m_MinimumLabel);
	vLocalCWndCollection.Add(&m_MinLabel);
	vLocalCWndCollection.Add(&m_MinRunningCurrent);
	vLocalCWndCollection.Add(&m_MinRunningVoltage);
	vLocalCWndCollection.Add(&m_MinVLabel);
	vLocalCWndCollection.Add(&m_MinVoltage);
	vLocalCWndCollection.Add(&m_ReadingLabel);
	vLocalCWndCollection.Add(&m_RunningLabel);
	vLocalCWndCollection.Add(&m_SettingLabel);
	vLocalCWndCollection.Add(&m_SourceCurrentLabel);
	vLocalCWndCollection.Add(&m_SourceInterlocks2);
	vLocalCWndCollection.Add(&m_SourceInterlocksLabel2);
	vLocalCWndCollection.Add(&m_SourceVoltageLabel);
	vLocalCWndCollection.Add(&m_SVoltageReading);
	vLocalCWndCollection.Add(&m_TemperatureAir);
	vLocalCWndCollection.Add(&m_TemperatureAirLabel);
	vLocalCWndCollection.Add(&m_TemperatureAirMaximum);
	vLocalCWndCollection.Add(&m_TemperatureAirMinimum);
	vLocalCWndCollection.Add(&m_TemperatureSurface);
	vLocalCWndCollection.Add(&m_TemperatureSurfaceLabel);
	vLocalCWndCollection.Add(&m_TemperatureSurfaceMaximum);
	vLocalCWndCollection.Add(&m_TemperatureSurfaceMinimum);
	vLocalCWndCollection.Add(&m_UControllerCommunications);
	vLocalCWndCollection.Add(&m_UControllerCommunicationsLabel);
	vLocalCWndCollection.Add(&m_VoltageSetting);
}

void CXRaySourceStatusDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background1.SetWindowPos(NULL,cMainBackgroundLeft, cMainBackgroundTop,cMainBackgroundLength, cMainBackgroundHeight,SWP_NOZORDER);

			//9/11/2009
	//vOriginalEjectorsEnabled = vLocalConfigurationData->vEnableEjectors;

	vChangedLamps = false;
	vOriginalLampStatus = vLocalSystemData->vCurrentLampStatus;
	vLocalSystemData->vGivenTemperatureWarning = false;
	vOldSourceInterLocks = !vLocalSystemData->vSourceInterLocks;
	vOldSourceVoltage = 0;
	vOldSourceCurrent = 0;
	vFunction4ButtonEnable = true;
	vFunction5ButtonEnable = true;
	vSubFunction2ButtonEnable = true;
	vSubFunction6ButtonEnable = true;
	vSubFunction7ButtonEnable = true;

	if ((vLocalConfigurationData->vHVPSType == c100WMonoBlockHVPS) || (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock))
	if (vGlobalDXMXRayPort)
	{
		vGlobalDXMXRayPort->vOldTemperature = -2;
		vGlobalDXMXRayPort->vOldTemperatureMaximum = -1;
		vGlobalDXMXRayPort->vOldTemperatureMinimum = 0xFFFF;
	}
	
	if (vLocalConfigurationData->vHVPSType == c100WVJTMonoBlockHVPS)
	{
		m_HVPS15V.ShowWindow(SW_HIDE);
		m_HVPSLabel.ShowWindow(SW_HIDE);
	}

	if (vLocalSystemData->vSystemRunMode == cMeasureSourceStrengthMode)
	{
		SetDlgItemText(IDC_SubFunction2Button, _T("Abort Measure Source!"));
		vOldMeasureSourceStrength = true;
	}
	else
	{
		vOldMeasureSourceStrength = false;
		SetDlgItemText(IDC_SubFunction2Button, _T("Measure Source Strength!"));
	}

	//SetDlgItemText(IDC_SubFunction1Button,"");
	//SetDlgItemText(IDC_SubFunction4Button,"");
	//SetDlgItemText(IDC_SubFunction8Button,"");
	m_SubFunction1Button.ShowWindow(SW_HIDE);
	m_SubFunction4Button.ShowWindow(SW_HIDE);
	m_SubFunction8Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
	this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
	this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());

	m_SourceFault.ShowWindow(SW_SHOW);
	m_SourceFaultLabel.ShowWindow(SW_SHOW);

	SetMinimumAndMaximum();
	UpdateButtons();
	UpdateDisplay();

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
	if (vLocalConfigurationData->vDeltaTemperatureForFilterCloggedError == 0) //have have AirConditioning
		SetDlgItemText(IDC_TemperatureAirOutsideLabel, _T("A/C Out:"));

	if (!vGlobalFPGAVersion10Point0OrHigher)
	if (vLocalConfigurationData->vHasOutsideAirTemp)
		SetDlgItemText(IDC_TemperatureSurfaceLabel, _T("Outside Air:"));

	SetDlgItemText(IDC_Function5Button, _T("Align\nX-Ray Tube"));
	m_Function5Button.ShowWindow(SW_SHOW);

	if (vGlobalFPGAVersion10Point0OrHigher)
	{
		vOldTemperatureSurface = 0xFFFE;
		vOldTemperatureLiquid = 0xFFFE;
		vOldTemperatureAir = 0xFFFE;
		vOldTemperatureAirOutside = 0xFFFE;
		vOldTemperatureuC = 0xFFFE;
		//vOldCoolantPressure = 0xFFFE;

		vOldTemperatureSurfaceMaximum = 0xFFFE;
		vOldTemperatureLiquidMaximum = 0xFFFE;
		vOldTemperatureAirMaximum = 0xFFFE;
		vOldTemperatureAirOutsideMaximum = 0xFFFE;
		vOldTemperatureuCMaximum = 0xFFFE;
		//vOldCoolantPressureMaximum = 0xFFFE;

		vOldTemperatureSurfaceMinimum = 0xFFFE;
		vOldTemperatureLiquidMinimum = 0xFFFE;
		vOldTemperatureAirMinimum = 0xFFFE;
		vOldTemperatureAirOutsideMinimum = 0xFFFE;
		vOldTemperatureuCMinimum = 0xFFFE;
		//vOldCoolantPressureMinimum = 0xFFFE;

		//m_CoolantPressure.ShowWindow(SW_SHOW);
		//m_CoolantPressureLabel.ShowWindow(SW_SHOW);
		//m_CoolantPressureMaximum.ShowWindow(SW_SHOW);
		//m_CoolantPressureMinimum.ShowWindow(SW_SHOW);
		m_TemperatureAirOutside.ShowWindow(SW_SHOW);
		m_TemperatureAirOutsideLabel.ShowWindow(SW_SHOW);
		m_TemperatureAirOutsideMaximum.ShowWindow(SW_SHOW);
		m_TemperatureAirOutsideMinimum.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_TemperatureSurfaceLabel, _T("Surface:"));
	}
	
//	if ((vLocalSystemData->vCPUTemperaturesReadOK = GetCoreTempInfoAlternative()) == true)		//4/24/2012 kjh
	if (vLocalSystemData->vCPUTemperaturesReadOK == true)		//4/24/2012 kjh
	{
		DisplayCPUTemperatures();
		SetDlgItemText(IDC_TemperatureCPULabel, _T("CPU:"));
		m_TemperatureCPULabel.ShowWindow(SW_SHOW);
		m_TemperatureCPU.ShowWindow(SW_SHOW);
		m_TemperatureCPUMinimum.ShowWindow(SW_SHOW);
		m_TemperatureCPUMaximum.ShowWindow(SW_SHOW);
	}
	else
	{
		m_TemperatureCPULabel.ShowWindow(SW_HIDE);
		m_TemperatureCPU.ShowWindow(SW_HIDE);
		m_TemperatureCPUMinimum.ShowWindow(SW_HIDE);
		m_TemperatureCPUMaximum.ShowWindow(SW_HIDE);
	}

	if (vLocalSystemData->vFPGAVersion8Point7OrHigher)
	{
		m_OnOff.ShowWindow(SW_SHOW);
		m_OnOffLabel.ShowWindow(SW_SHOW);
	}

	if (vGlobalDXMXRayPort)
	{
		vGlobalDXMXRayPort->vOldFilamentCurrentMin = -1; //force display to update
		vGlobalDXMXRayPort->vOldFilamentCurrentMax = -1;
	}

	if ((vLocalConfigurationData->vHVPSType == c210WMonoBlockHVPS) || (vLocalConfigurationData->vHVPSType == cMNXHVPS)) //MNX and 210W MonoBlock dont have coolant or MonoBlock Temp
	{ //no cooling with 50 watt tube
		SetDlgItemText(IDC_HVPS15VLabel, _T("HVPS +24V:"));
		if (vLocalConfigurationData->vUseExternalInterlock)
		{
			CString TempText = "";
			TempText.LoadString(IDS_ExternalInterlock);
			SetDlgItemText(IDC_CoolantFlowLabel, TempText + ":");
		}
		else
		{
			m_CoolantFlowLabel.ShowWindow(SW_HIDE);
			m_CoolantFlow.ShowWindow(SW_HIDE);
		}
		m_TemperatureLiquidLabel.ShowWindow(SW_HIDE);
		m_TemperatureLiquid.ShowWindow(SW_HIDE);
		m_TemperatureLiquidMinimum.ShowWindow(SW_HIDE);
		m_TemperatureLiquidMaximum.ShowWindow(SW_HIDE);

		m_TemperatureuCLabel.SetWindowPos(NULL,110,348,155,20,SWP_NOZORDER); //move it up one line so don't have a blank line in middle of temperatures.
		m_TemperatureuC.SetWindowPos(NULL,269,348,85,20,SWP_NOZORDER); //move it up one line so don't have a blank line in middle of temperatures.
		m_TemperatureuCMinimum.SetWindowPos(NULL,358,348,85,20,SWP_NOZORDER); //move it up one line so don't have a blank line in middle of temperatures.
		m_TemperatureuCMaximum.SetWindowPos(NULL,444,348,80,20,SWP_NOZORDER); //move it up one line so don't have a blank line in middle of temperatures.

		m_TemperatureCPULabel.SetWindowPos(NULL,110,366,155,20,SWP_NOZORDER); //move it up one line so don't have a blank line in middle of temperatures.
		m_TemperatureCPU.SetWindowPos(NULL,269,366,85,20,SWP_NOZORDER); //move it up one line so don't have a blank line in middle of temperatures.
		m_TemperatureCPUMinimum.SetWindowPos(NULL,358,366,85,20,SWP_NOZORDER); //move it up one line so don't have a blank line in middle of temperatures.
		m_TemperatureCPUMaximum.SetWindowPos(NULL,444,366,80,20,SWP_NOZORDER); //move it up one line so don't have a blank line in middle of temperatures.
	}
	if ((vLocalConfigurationData->vHVPSType == c100WMonoBlockHVPS) || (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock))
	{
		if (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock)
			SetDlgItemText(IDC_HVPS15VLabel, _T("HVPS +24V:"));

		SetDlgItemText(IDC_TemperatureLiquidLabel, _T("MonoBlock:"));
		if (vLocalConfigurationData->vUseExternalInterlock)
		{
			CString TempText = "";
			TempText.LoadString(IDS_ExternalInterlock);
			SetDlgItemText(IDC_CoolantFlowLabel, TempText + ":");
		}
		else
		{
			m_CoolantFlowLabel.ShowWindow(SW_HIDE);
			m_CoolantFlow.ShowWindow(SW_HIDE);
		}
		SetDlgItemText(IDC_OnOffLabel, _T("MonoBlock Interlock:"));
		SetDlgItemText(IDC_SourceFaultLabel, _T("MonoBlock:"));
	}

	if (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)
	{
		SetDlgItemText(IDC_SourceInterlocksLabel2, _T("Front/Side Door Interlock:"));
		SetDlgItemText(IDC_SourceInterlocksLabel, _T("Tunnel Interlock:"));
	}

	if (vGlobalMonoBlockXRayPort)  //no cooling with monoblock
	{
		SetDlgItemText(IDC_HVPS15VLabel, _T("HVPS +24V:"));
		if (vLocalConfigurationData->vUseExternalInterlock)
		{
			CString TempText = "";
			TempText.LoadString(IDS_ExternalInterlock);
			SetDlgItemText(IDC_CoolantFlowLabel, TempText + ":");
		}
		else
		{
			m_CoolantFlowLabel.ShowWindow(SW_HIDE);
			m_CoolantFlow.ShowWindow(SW_HIDE);
		}
		m_TemperatureLiquidLabel.ShowWindow(SW_HIDE);
		m_TemperatureLiquid.ShowWindow(SW_HIDE);
		m_TemperatureLiquidMinimum.ShowWindow(SW_HIDE);
		m_TemperatureLiquidMaximum.ShowWindow(SW_HIDE);
	}

	if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
	{
		if ((vGlobalGuardMaster) && (vGlobalGuardMaster->vDetectedValidGuardMasterDevice))
		{
			SetDlgItemText(IDC_SourceInterlocksLabel, _T("Conveyor Interlock:"));
			SetDlgItemText(IDC_SourceInterlocksLabel2, _T("Cabinet Interlock:"));
		}
		else
		{
			SetDlgItemText(IDC_SourceInterlocksLabel, _T("Lower Door Interlock:"));
			SetDlgItemText(IDC_SourceInterlocksLabel2, _T("Upper Door Interlock:"));
		}
		SetDlgItemText(IDC_OnOffLabel, _T("MonoBlock Interlock:"));
		SetDlgItemText(IDC_SourceFaultLabel, _T("MonoBlock Comm:"));
	}

	if (!vLocalConfigurationData->vRadiationShutterInSystem)
	{
		m_ShutterStatus.ShowWindow(SW_HIDE);
		m_ShutterStatusLabel.ShowWindow(SW_HIDE);
	}

	int TempTimerResult = SetTimer(vDisplayTimerHandle,500,NULL);
	if (!TempTimerResult)
		ReportErrorMessage("Error-Display Timer Failed",cEMailInspx,32000);
	this->SetWindowText( _T("X-Ray"));

	::SetupMenu(vLocalCWndCollection);
}

void CXRaySourceStatusDialog::ChangedMinimumAndMaximum()
{
	//vOldSourceVoltageMin = 10;
	//vOldSourceVoltageMax = 0;
	//vOldSourceCurrentMin = 10;
	//vOldSourceCurrentMax = 0;
	SetDlgItemText(IDC_MinRunningVoltage, _T("--"));
	SetDlgItemText(IDC_MaxRunningVoltage, _T("--"));
	SetDlgItemText(IDC_MinRunningCurrent, _T("--"));
	SetDlgItemText(IDC_MaxRunningCurrent, _T("--"));
	SetDlgItemText(IDC_MinRunningFilamentCurrent, _T("--"));
	SetDlgItemText(IDC_MaxRunningFilamentCurrent, _T("--"));
}

void CXRaySourceStatusDialog::SetMinimumAndMaximum()
{
	//vOldSourceVoltageMin = 10;
	//vOldSourceCurrentMin = 10;
	//vOldSourceVoltageMax = 0;
	//vOldSourceCurrentMax = 0;

	vOldRampXRayVoltageAmount = vLocalSystemData->vRampXRayVoltageAmount;
	vOldRampXRayCurrentAmount = vLocalSystemData->vRampXRayCurrentAmount;

	CString TempString("");
	TempString = "";
	TempString = dtoa(vLocalSystemData->vSourceCurrentMinRedLine,1);
	SetDlgItemText(IDC_MinCurrent,TempString);
	TempString = dtoa(vLocalSystemData->vSourceCurrentMaxRedLine,1);
	SetDlgItemText(IDC_MaxCurrent,TempString);

	TempString = dtoa(vLocalSystemData->vSourceVoltageMinRedLine * 6,1);
	SetDlgItemText(IDC_MinVoltage,TempString);
	TempString = dtoa(vLocalSystemData->vSourceVoltageMaxRedLine * 6,1);
	SetDlgItemText(IDC_MaxVoltage,TempString);

	TempString = dtoa(vOldRampXRayVoltageAmount * 6,1);
	SetDlgItemText(IDC_VoltageSetting,TempString);
	SetDlgItemText(IDC_VoltageSetting2,TempString);

	if (PasswordOK(cTemporaryInspxPassword,false))
		TempString = dtoa(vOldRampXRayCurrentAmount,4);
	else
		TempString = dtoa(vOldRampXRayCurrentAmount,1);

	SetDlgItemText(IDC_CurrentSetting,TempString);
	SetDlgItemText(IDC_CurrentSetting2,TempString);

	CWnd *TempWindow = CWnd::GetDlgItem(IDC_SVoltageReading);
	if (TempWindow)
		TempWindow->Invalidate(false);
	TempWindow = CWnd::GetDlgItem(IDC_Current);
	if (TempWindow)
		TempWindow->Invalidate(false);
	UpdateButtons();
}

void CXRaySourceStatusDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXRaySourceStatusDialog)
	DDX_Control(pDX, IDC_ShutterStatusLabel, m_ShutterStatusLabel);
	DDX_Control(pDX, IDC_ShutterStatus, m_ShutterStatus);
	DDX_Control(pDX, IDC_MinRunningFilamentCurrent, m_MinRunningFilamentCurrent);
	DDX_Control(pDX, IDC_MaxRunningFilamentCurrent, m_MaxRunningFilamentCurrent);
	DDX_Control(pDX, IDC_TemperatureLiquidMinimum, m_TemperatureLiquidMinimum);
	DDX_Control(pDX, IDC_TemperatureLiquidMaximum, m_TemperatureLiquidMaximum);
	DDX_Control(pDX, IDC_ShutterTestButton, m_ShutterTestButton);
	DDX_Control(pDX, IDC_ChangedFilterButton, m_ChangedFilterButton);
	DDX_Control(pDX, IDC_StatusLine, m_StatusLine);
	DDX_Control(pDX, IDC_TemperatureAirOutsideMinimum, m_TemperatureAirOutsideMinimum);
	DDX_Control(pDX, IDC_TemperatureAirOutsideMaximum, m_TemperatureAirOutsideMaximum);
	DDX_Control(pDX, IDC_TemperatureAirOutsideLabel, m_TemperatureAirOutsideLabel);
	DDX_Control(pDX, IDC_TemperatureAirOutside, m_TemperatureAirOutside);
	DDX_Control(pDX, IDC_HVPSPreHeatLabel, m_HVPSPreHeatLabel);
	DDX_Control(pDX, IDC_HVPSPreHeat, m_HVPSPreHeat);
	DDX_Control(pDX, IDC_HVPS15VLabel, m_HVPS15VLabel);
	DDX_Control(pDX, IDC_HVPS15V, m_HVPS15V);
	DDX_Control(pDX, IDC_HVPSLabel, m_HVPSLabel);
	DDX_Control(pDX, IDC_HVPS, m_HVPS);
	DDX_Control(pDX, IDC_VoltageSetting2, m_VoltageSetting2);
	DDX_Control(pDX, IDC_CurrentSetting2, m_CurrentSetting2);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_OnOffLabel, m_OnOffLabel);
	DDX_Control(pDX, IDC_OnOff, m_OnOff);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_TemperatureLiquid, m_TemperatureLiquid);
	DDX_Control(pDX, IDC_TemperatureLiquidLabel, m_TemperatureLiquidLabel);
	DDX_Control(pDX, IDC_CoolantFlow, m_CoolantFlow);
	DDX_Control(pDX, IDC_CoolantFlowLabel, m_CoolantFlowLabel);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_SourceInterlocksLabel, m_SourceInterlocksLabel);
	DDX_Control(pDX, IDC_SourceInterlocks, m_SourceInterlocks);
	DDX_Control(pDX, IDC_SourceFaultLabel, m_SourceFaultLabel);
	DDX_Control(pDX, IDC_SourceFault, m_SourceFault);
	DDX_Control(pDX, IDC_XRaysOn, m_XRaysOn);
	DDX_Control(pDX, IDC_Background, m_Background1);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TemperatureCPU, m_TemperatureCPU);
	DDX_Control(pDX, IDC_TemperatureCPULabel, m_TemperatureCPULabel);
	DDX_Control(pDX, IDC_TemperatureCPUMaximum, m_TemperatureCPUMaximum);
	DDX_Control(pDX, IDC_TemperatureCPUMinimum, m_TemperatureCPUMinimum);
	DDX_Control(pDX, IDC_TemperatureuC, m_TemperatureuC);
	DDX_Control(pDX, IDC_TemperatureuCLabel, m_TemperatureuCLabel);
	DDX_Control(pDX, IDC_TemperatureuCMaximum, m_TemperatureuCMaximum);
	DDX_Control(pDX, IDC_TemperatureuCMinimum, m_TemperatureuCMinimum);
	DDX_Control(pDX, IDC_Current, m_Current);
	DDX_Control(pDX, IDC_CurrentLabel, m_CurrentLabel);
	DDX_Control(pDX, IDC_CurrentSetting, m_CurrentSetting);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_EStop, m_EStop);
	DDX_Control(pDX, IDC_EStopLabel, m_EStopLabel);
	DDX_Control(pDX, IDC_KeySwitch, m_KeySwitch);
	DDX_Control(pDX, IDC_KeySwitchLabel, m_KeySwitchLabel);
	DDX_Control(pDX, IDC_LimitsLabel, m_LimitsLabel);
	DDX_Control(pDX, IDC_MaxCurrent, m_MaxCurrent);
	DDX_Control(pDX, IDC_MaximumLabel, m_MaximumLabel);
	DDX_Control(pDX, IDC_MaxLabel, m_MaxLabel);
	DDX_Control(pDX, IDC_MaxRunningCurrent, m_MaxRunningCurrent);
	DDX_Control(pDX, IDC_MaxRunningVoltage, m_MaxRunningVoltage);
	DDX_Control(pDX, IDC_MaxVLabel, m_MaxVLabel);
	DDX_Control(pDX, IDC_MaxVoltage, m_MaxVoltage);
	DDX_Control(pDX, IDC_MinCurrent, m_MinCurrent);
	DDX_Control(pDX, IDC_MinimumLabel, m_MinimumLabel);
	DDX_Control(pDX, IDC_MinLabel, m_MinLabel);
	DDX_Control(pDX, IDC_MinRunningCurrent, m_MinRunningCurrent);
	DDX_Control(pDX, IDC_MinRunningVoltage, m_MinRunningVoltage);
	DDX_Control(pDX, IDC_MinVLabel, m_MinVLabel);
	DDX_Control(pDX, IDC_MinVoltage, m_MinVoltage);
	DDX_Control(pDX, IDC_ReadingLabel, m_ReadingLabel);
	DDX_Control(pDX, IDC_RunningLabel, m_RunningLabel);
	DDX_Control(pDX, IDC_SettingLabel, m_SettingLabel);
	DDX_Control(pDX, IDC_SourceCurrentLabel, m_SourceCurrentLabel);
	DDX_Control(pDX, IDC_SourceInterlocks2, m_SourceInterlocks2);
	DDX_Control(pDX, IDC_SourceInterlocksLabel2, m_SourceInterlocksLabel2);
	DDX_Control(pDX, IDC_SourceVoltageLabel, m_SourceVoltageLabel);
	DDX_Control(pDX, IDC_SVoltageReading, m_SVoltageReading);
	DDX_Control(pDX, IDC_TemperatureAir, m_TemperatureAir);
	DDX_Control(pDX, IDC_TemperatureAirLabel, m_TemperatureAirLabel);
	DDX_Control(pDX, IDC_TemperatureAirMaximum, m_TemperatureAirMaximum);
	DDX_Control(pDX, IDC_TemperatureAirMinimum, m_TemperatureAirMinimum);
	DDX_Control(pDX, IDC_TemperatureSurface, m_TemperatureSurface);
	DDX_Control(pDX, IDC_TemperatureSurfaceLabel, m_TemperatureSurfaceLabel);
	DDX_Control(pDX, IDC_TemperatureSurfaceMaximum, m_TemperatureSurfaceMaximum);
	DDX_Control(pDX, IDC_TemperatureSurfaceMinimum, m_TemperatureSurfaceMinimum);
	DDX_Control(pDX, IDC_UControllerCommunications, m_UControllerCommunications);
	DDX_Control(pDX, IDC_UControllerCommunicationsLabel, m_UControllerCommunicationsLabel);
	DDX_Control(pDX, IDC_VoltageSetting, m_VoltageSetting);
}

BEGIN_MESSAGE_MAP(CXRaySourceStatusDialog, CDialog)
	//{{AFX_MSG_MAP(CXRaySourceStatusDialog)
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
	ON_BN_CLICKED(IDC_StatusLine, OnStatusLine)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_ChangedFilterButton, OnChangedFilterButton)
	ON_BN_CLICKED(IDC_ShutterTestButton, OnShutterTestButton)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_TemperatureAirOutsideLabel, &CXRaySourceStatusDialog::OnTemperatureairoutsidelabel)
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CXRaySourceStatusDialog::OnDialogtitlestatictext1)
	ON_STN_CLICKED(IDC_OnOff, &CXRaySourceStatusDialog::OnStnClickedOnoff)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXRaySourceStatusDialog message handlers

void CXRaySourceStatusDialog::OnFunction1Button() 
{
	// Main Menu Button was pressed
	PrepareToExit();
	CDialog::EndDialog(10);
}

void CXRaySourceStatusDialog::OnFunction2Button() 
{
	// Save and Exit Button was pressed
	PrepareToExit();
	CDialog::EndDialog(true);
}

void CXRaySourceStatusDialog::PrepareToExit()
{
	if (vTakingDarkCurrentMeasurment)
		StopDarkCurrentMeasurement(true);

	if (vCyclingShutter)
		OnShutterTestButton();

	if (vChangedLamps)
	{
		if ((vOriginalLampStatus & 1) != (vLocalSystemData->vCurrentLampStatus & 1))
			vGlobaluCSerialPort->TurnOnOffRedLamp(vOriginalLampStatus & 1);
		if ((vOriginalLampStatus & 2) != (vLocalSystemData->vCurrentLampStatus & 2))
			vGlobaluCSerialPort->TurnOnOffYellowLamp(vOriginalLampStatus & 2);
		if ((vOriginalLampStatus & 4) != (vLocalSystemData->vCurrentLampStatus & 4))
			vGlobaluCSerialPort->TurnOnOffBlueLamp(vOriginalLampStatus & 4);
	}
	/*
			//9/11/2009
	if (vLocalConfigurationData->vEnableEjectors != vOriginalEjectorsEnabled)
	{
		bool TempEnable = false;
		if (vOriginalEjectorsEnabled)
			TempEnable = true;
		//vLocalConfigurationData->vEnableEjectors = vOriginalEjectorsEnabled;
		//vGlobaluCSerialPort->EnableDisableEjectors(vLocalConfigurationData->vEnableEjectors);
		//vMainWindowPointer->UpdateDisableEjectorsButton();
		vMainWindowPointer->EnableDisableEjectors(TempEnable);
	}
	*/
}

void CXRaySourceStatusDialog::OnFunction3Button() 
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		if (vCyclingShutter)
			OnShutterTestButton();

		//Create diagnostic dialog box
		CInspxHiddenXRayDialog IInspxHiddenXRayDialog;  
		//Set dialog box data like how many products there are
		//IMoreDiagnosticsDialog.vLocalProductCollection = &vProductCollection;
		IInspxHiddenXRayDialog.vLocalSystemData = vLocalSystemData;
		IInspxHiddenXRayDialog.vLocalConfigurationData = vLocalConfigurationData;
		//IInspxHiddenXRayDialog.vMainWindowPointer = vMainWindowPointer;

		//Pass control to dialog box and display
		int nResponse = IInspxHiddenXRayDialog.DoModal();

		//if changes were made, save data to file
		vOldTemperatureAir = 0;
		vOldTemperatureLiquid = 0;
		vOldTemperatureSurface = 0;

		if (nResponse == 10)
		{
			//Main Menu button pressed
			PrepareToExit();
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
	/*
	if (PasswordOK(cTemporaryInspxPassword,false))
	//if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	{
		bool TempOKToChange = true;
		if (!vChangedLamps)
		{
			vOriginalLampStatus = vLocalSystemData->vCurrentLampStatus;
			vChangedLamps = true;
		}
		vTestingLamp = vTestingLamp ^ 1;
		if (!(vTestingLamp & 1))
		{
			if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
				vGlobaluCSerialPort->TurnOnOffRedLamp(0);
			else
				vGlobaluCSerialPort->TurnOnOffRedLamp(0);
		}
		//vGlobaluCSerialPort->TurnOnOffRedLamp(!(vLocalSystemData->vCurrentLampStatus & 1));
		UpdateButtons();
	}
	*/
}

void CXRaySourceStatusDialog::OnFunction4Button()
{
	if (vCyclingShutter)
		OnShutterTestButton();

	if (PasswordOK(cTemporaryInspxPassword,false)) //measure Dark Current
	if (vGlobalDXMXRayPort)
	if (vTakingDarkCurrentMeasurment)
		StopDarkCurrentMeasurement(true);
	else
	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	{
		if (!vMainWindowPointer->InterlocksOK())
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nCheck Interlocks.";
			if (PasswordOK(cSuperInspxPassword,false))
				TempNoticeDialog.vNoticeText = TempNoticeDialog.vNoticeText + "\nInterlocks State: " + WordToHex(vLocalSystemData->vSourceInterLocks);

			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			vDarkCurrent = vGlobalDXMXRayPort->vFilamentCurrentFeedbackMonitor;
			vDarkCurrentThreshold = 1.5 * vDarkCurrent;
			vTakingDarkCurrentMeasurment = true;
			vLocalSystemData->vSystemRunMode = cDarkCurrentMode;
			SetDlgItemText(IDC_DialogTitleStaticText1, _T("Measuring Dark Current"));
			SetDlgItemText(IDC_Function4Button, _T("Abort Measuring!"));
			if ((vGlobalDXMXRayPort) && 
				((vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac)))
				vLocalSystemData->vRampXRayVoltageAmount = 50. / 6.;
			else
				vLocalSystemData->vRampXRayVoltageAmount = 70. / 6.;
			vLocalSystemData->vRampXRayCurrentAmount = .000;
			vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount,vLocalSystemData->vRampXRayCurrentAmount);
			//vGlobaluCSerialPort->TurnOnOffRedLamp(true);//done automatically
			vMainWindowPointer->SendXRayPowerOnOffToUController(1);
			vLocalSystemData->vXRayState = 25;
			::PostThreadMessage(vGlobalCallingThreadID,cXRaysOnMessage,25,0);
			vDarkCurrentCurrentLevel = 0;
			vDarkCurrent = 0;
			UpdateDisplay();

			int TempTimerResult = SetTimer(vDarkCurrentTimerHandle,15000,NULL);
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nYou cannot do this function while the system is running";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	/*
	//Cancel was pressed
	//CDialog::OnCancel();
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		//if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		{
			if (!vChangedLamps)
			{
				vOriginalLampStatus = vLocalSystemData->vCurrentLampStatus;
				vChangedLamps = true;
			}
			vTestingLamp = vTestingLamp ^ 2;
			if (!(vTestingLamp & 2))
				vGlobaluCSerialPort->TurnOnOffYellowLamp(0);
			//vGlobaluCSerialPort->TurnOnOffYellowLamp(!(vLocalSystemData->vCurrentLampStatus & 2));
			UpdateButtons();
		}
		//else
		//	ShowVoltageHistogram();
	}
	*/
}

void CXRaySourceStatusDialog::OnFunction5Button() 
{
	/*
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		//if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		{
			if (!vChangedLamps)
			{
				vOriginalLampStatus = vLocalSystemData->vCurrentLampStatus;
				vChangedLamps = true;
			}
			if (!vLocalConfigurationData->vEnableEjectors)
			{
				vLocalConfigurationData->vEnableEjectors = true;
				vGlobaluCSerialPort->EnableDisableEjectors(vLocalConfigurationData->vEnableEjectors);
				vMainWindowPointer->UpdateDisableEjectorsButton();
			}
			vTestingLamp = vTestingLamp ^ 4;
			if (!(vTestingLamp & 4))
				vGlobaluCSerialPort->TurnOnOffBlueLamp(0);
			//vGlobaluCSerialPort->TurnOnOffBlueLamp(!(vLocalSystemData->vCurrentLampStatus & 4));
			UpdateButtons();
		}
		//else
		//	ShowCurrentHistogram();
	}
	*/
	bool TempPasswordOK = PasswordOK(cTemporaryInspxPassword,false);

/*	if (!TempPasswordOK)
	{
		//first check password
		CPasswordEntryDialog IPasswordEntryDialog;  

		//Set dialog box data titles and number value
		IPasswordEntryDialog.vEditString = "";
		CString TempText("");
		TempText.LoadString(IDS_EnterCertifiedSetupPassword);
		IPasswordEntryDialog.m_DialogTitleStaticText1 = TempText;
		IPasswordEntryDialog.m_DialogTitleStaticText2 = "Only given to customers with Advanced Training";
		//Pass control to dialog box and display
		int nResponse = IPasswordEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			PasswordOK(cSuperInspxPassword,true)
			if ((vLocalConfigurationData->vCertifiedSetupPassword == IPasswordEntryDialog.vEditString) ||
				("72054328" == IPasswordEntryDialog.vEditString) ||
				("7205" == IPasswordEntryDialog.vEditString) ||
				("31415926" == IPasswordEntryDialog.vEditString) ||
				("5104682199" == IPasswordEntryDialog.vEditString) ||
				(vLocalConfigurationData->vInspxPassword == IPasswordEntryDialog.vEditString))
			{
				TempPasswordOK = true;
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				CString TempText;
				TempText.LoadString(IDS_PasswordNotCorrect);
				TempNoticeDialog.vNoticeText = TempText;
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
		}
		
	}*/
	if (PasswordOK(cCertifiedPassword, true))
	{
		if (vCyclingShutter)
			OnShutterTestButton();

		if (!vMainWindowPointer->InterlocksOK())
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nCheck Interlocks.";
			if (PasswordOK(cSuperInspxPassword,false))
				TempNoticeDialog.vNoticeText = TempNoticeDialog.vNoticeText + "\nInterlocks State: " + WordToHex(vLocalSystemData->vSourceInterLocks);
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
		else
		if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nYou Can Not Align Tube When ScanTrac Is Running.";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
		else
		if (vLocalSystemData->vXRayOnNotice)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nYou Can Not Align Tube When ScanTrac Is Running.";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			CXRayTubeAlignmentDialog IXRayTubeAlignmentDialog;  

			//Set dialog box data like how many products there are
			IXRayTubeAlignmentDialog.vLocalConfigurationData = vLocalConfigurationData;
			IXRayTubeAlignmentDialog.vLocalSystemData = vLocalSystemData;
			IXRayTubeAlignmentDialog.vMainWindowPointer = vMainWindowPointer;;

			//Pass control to dialog box and display
			int nResponse = IXRayTubeAlignmentDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			//if (nResponse == IDOK)
			//{
				// TODO: Place code here to handle when the dialog is closed
			//}
			//else 
			if (nResponse == 10)
			{
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
	}
	CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
	if (TempWindow)
		TempWindow->Invalidate(false);
}

void CXRaySourceStatusDialog::OnSubFunction1Button() 
{
	//Turn on X-Rays button was pressed
	//if (!(vTestingLamp))
	if (vCyclingShutter)
		OnShutterTestButton();

	if (PasswordOK(cSetupPassword,true))
	if (vGlobalCurrentProduct)
	{
		if (vMainWindowPointer->InterlocksOK())
		{
			if (vMainWindowPointer->IfTubeHasBeenOffLongEnough())
			{
				//9/11/2009
				/*
				//vGlobaluCSerialPort->SendXRayPowerOnOffToUController(1);
				if (vLocalConfigurationData->vEnableEjectors != vOriginalEjectorsEnabled)
				{
					//vLocalConfigurationData->vEnableEjectors = vOriginalEjectorsEnabled;
					//vGlobaluCSerialPort->EnableDisableEjectors(vLocalConfigurationData->vEnableEjectors);
					//vMainWindowPointer->UpdateDisableEjectorsButton();
					bool TempEnable = false;
					if (vOriginalEjectorsEnabled)
						TempEnable = true;
					vMainWindowPointer->EnableDisableEjectors(TempEnable);
				}
				*/

				//9/11/2009
				if (vLocalConfigurationData->vEnableEjectors)
				{
					vMainWindowPointer->OnSubFunction2Button();

					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\nEjectors have been Disabled\nfor Run Diagnostic Mode.";
					TempNoticeDialog.vType = cNoticeMessage;
					TempNoticeDialog.DoModal();
				}

				if (vMainWindowPointer->PrepareToRun())
				{
					SetMinimumAndMaximum();
					ReportErrorMessage("Operator pressed Run in Diagnostics Mode", cUserAction,0);
					vLocalSystemData->vRunningDiagnosticsMode = true;
					vMainWindowPointer->StartRunning(true, true);

					int TempTimerResult = SetTimer(vResetMinMaxAfterStablizeTimerHandle,10000,NULL);
					if (!TempTimerResult)
						ReportErrorMessage("Error-Reset Min Max Timer Failed",cEMailInspx,32000);
				}

				if (vChangedLamps)
				{
					vOriginalLampStatus = vOriginalLampStatus | 1;
				}
			}
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nCheck Interlocks";
			if (PasswordOK(cSuperInspxPassword,false))
				TempNoticeDialog.vNoticeText = TempNoticeDialog.vNoticeText + "\nInterlocks State: " + WordToHex(vLocalSystemData->vSourceInterLocks);
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	UpdateButtons();
}

void CXRaySourceStatusDialog::OnSubFunction2Button() 
{
	BYTE TempAlternateFunction = vGlobalShiftKeyDown;

	if (vCyclingShutter)
		OnShutterTestButton();

	//measure source Strength button pressed
	//if (!(vTestingLamp))
	if (PasswordOK(cSetupPassword,true))
	{
		if (vMainWindowPointer->InterlocksOK())
		{
			if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
			{
				bool TempOKToRun = true;
				if (vLocalSystemData->vActualEncoderRate > 0)
				{
					CYesNoDialog TempYesNoDialog;
					if ((vLocalConfigurationData->vScanTracType == cFermataScanTrac) || (vLocalConfigurationData->vScanTracType == cFermataScanTrac))
						TempYesNoDialog.vNoticeText =  _T("\nThe Pipe must be removed from the X-Ray beam,\nor the measurement will be wrong.\nStart measuring?");
					else
						TempYesNoDialog.vNoticeText =  _T("\nAssure no containers go through while measuring,\nor the measurement will be wrong.\nStart measuring?");
					TempYesNoDialog.vYesButtonText = "Start Measuring";
					TempYesNoDialog.vNoButtonText = "";
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult != IDOK)
						TempOKToRun = false;
				}

				if (TempOKToRun)
				{
					ReportErrorMessage("Save Original Product so can restore after calibration complete", cWriteToLog, 0);
					vMainWindowPointer->vCalibrateHoldCurrentProduct = vGlobalCurrentProduct;
								
					vMainWindowPointer->LoadMeasureSourceStrengthProduct();

					vLocalSystemData->vCalibratingXScanPoint4mmDetectors = 0;
					if ((vLocalConfigurationData->vUseXScanDetectors) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))
						vLocalSystemData->vCalibratingXScanPoint4mmDetectors = vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors;

					if ((vLocalConfigurationData->vScanTracType != cSoloScanTrac) && (vLocalConfigurationData->vScanTracType != cSoloPlusScanTrac) && (vLocalConfigurationData->vScanTracType != cUpSideDownSoloScanTrac) && 
						(vLocalConfigurationData->vScanTracType != cFermataScanTrac)) //on solos set the voltage and current to standard values each time. //new source strength code
					if ((dtoa(vGlobalCurrentProduct->vXRaySourceVoltage * 6.0, 1) != dtoa(vMainWindowPointer->GetDefaultMeasureSourceStrengthVoltage(), 1)) ||  
						(dtoa(vGlobalCurrentProduct->vXRaySourceCurrent, 1) != dtoa(vMainWindowPointer->GetDefaultMeasureSourceStrengthCurrent(vLocalConfigurationData->vXRaySourceFilter), 1)))
					{
						TempOKToRun = AskIfFilterChanged();
						if (TempOKToRun)
						{
							vMainWindowPointer->LoadMeasureSourceStrengthProduct();
						}
					}
				}
				if (TempOKToRun)
				{
					//set gains to 10
					//if ((dtoa(vGlobalCurrentProduct->vXRaySourceVoltage * 6.0, 1) != dtoa(vMainWindowPointer->GetDefaultMeasureSourceStrengthVoltage(), 1)) ||
					//	(vGlobalCurrentProduct->vXRayIntegrationTime != 750) ||
					//	(dtoa(vGlobalCurrentProduct->vXRaySourceCurrent, 1) != dtoa(vMainWindowPointer->GetDefaultMeasureSourceStrengthCurrent(vLocalConfigurationData->vXRaySourceFilter), 1)))
					{
						vMainWindowPointer->SendADCGainsAndOffsets(true);

						if (vGlobalFPGAVersion10Point0OrHigher)
							vMainWindowPointer->SendPreAmpGain(1);//preamp gain of 2 
						else
							vMainWindowPointer->SendPreAmpGain(2);//preamp gain of 4 
					}

					vLocalSystemData->vApplyGainAndOffsetDuringCalibration = false;
					vLocalSystemData->vModeToRunAfterCalibrateOffset = cMeasureSourceStrengthMode;

					/*
					if (PasswordOK(cTemporaryInspxPassword,false))
					{
						CYesNoDialog TempYesNoDialog;
						TempYesNoDialog.vNoticeText = "\n\n\nApply Pixel Corrections Durring Source Strength?";
						TempYesNoDialog.vYesButtonText = "Don't Apply Corrections";
						TempYesNoDialog.vNoButtonText = "Apply Corrections";
						TempYesNoDialog.vQuestionType = cConfirmQuestion;
						int TempResult = TempYesNoDialog.DoModal();
						if (TempResult != IDOK)
							vLocalSystemData->vApplyGainAndOffsetDuringCalibration = true;
					}
					*/
					if (TempAlternateFunction)  //do measure source strength without calibrating the black level
					{
						if (vMainWindowPointer)
						{
							vMainWindowPointer->MeasureSourceStrength();
							PrepareToExit();
							CDialog::EndDialog(10);
						}
					}
					else
					{
						//this will calibrate detectors, then do measure source strength
						vLocalSystemData->vCalibrateErrorCount = 0;
						//vMainWindowPointer->CalibrateADCDetectorOffset(true,false);
						vLocalSystemData->vCalibrateADCAfterDAC = true;
						vLocalSystemData->vAutoCalibrate = true;
						vLocalSystemData->vAutoCalibrateWhileRunning = false;
						vLocalSystemData->vFullDetectorCalibration = false;
						if (vLocalConfigurationData->vUseDACAndADCinDensityCompensation)
							vMainWindowPointer->CalibrateDACDetectorOffset();
						else
							vMainWindowPointer->CalibrateADCDetectorOffset(vLocalSystemData->vAutoCalibrate, false);
						PrepareToExit();
						CDialog::EndDialog(10);
					}
				}
			}
			else
			if (vLocalSystemData->vSystemRunMode == cMeasureSourceStrengthMode)
			{
				vMainWindowPointer->EndMeasureSourceStrength(true);
				SetDlgItemText(IDC_SubFunction2Button, _T("Measure Source Strength!"));
				vLocalSystemData->vInAutoSetup = false;
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nYou cannot do this function while the system is running";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
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
}

void CXRaySourceStatusDialog::OnSubFunction3Button() 
{
	if (vCyclingShutter)
		OnShutterTestButton();

	if ((vLocalSystemData->vTemporarilyChangeSource) && (PasswordOK(cTemporaryInspxPassword,false)))
	{
		vLocalSystemData->vRampXRayVoltageAmount = vGlobalCurrentProduct->vXRaySourceVoltage;
		vLocalSystemData->vRampXRayCurrentAmount = vGlobalCurrentProduct->vXRaySourceCurrent;
		if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
		{
			vLocalSystemData->vSourceRampedUp = false;
			vMainWindowPointer->SendSourceSettingsToUController(
				vLocalSystemData->vRampXRayVoltageAmount,vLocalSystemData->vRampXRayCurrentAmount);
			//::PostThreadMessage(vGlobalCallingThreadID,cRampingSourceMessage,0,0);

			int TempTimerResult = SetTimer(vResetMinMaxAfterStablizeTimerHandle,7000,NULL);
			if (!TempTimerResult)
				ReportErrorMessage("Error-Reset Min Max Timer Failed",cEMailInspx,32000);
		}
		vLocalSystemData->vTemporarilyChangeSource = false;
		SetMinimumAndMaximum();
		ChangedMinimumAndMaximum();
	}
	else
	{
		CHelpDialog * IHelpDialog;
		IHelpDialog = new(CHelpDialog);
		//IHelpDialog->vHelpContext = 0;
		IHelpDialog->vMainWindowPointer = vMainWindowPointer;
		IHelpDialog->vLocalSystemData = vLocalSystemData;
		//IHelpDialog->vHelpType = 0;
		IHelpDialog->vWindowType = cSourceStrengthWindow;
		IHelpDialog->vLocalProductCollection = vLocalProductCollection;
		IHelpDialog->vLocalConfigurationData = vLocalConfigurationData;
		IHelpDialog->vProductPointer = vGlobalCurrentProduct;
		int TempResult = IHelpDialog->DoModal();
		delete IHelpDialog;
	}
}

void CXRaySourceStatusDialog::OnSubFunction4Button() 
{
	if (vCyclingShutter)
		OnShutterTestButton();

	//Turn off X-Rays button was pressed
	if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) && (vLocalSystemData->vSystemRunMode != cDarkCurrentMode))
	{
		vMainWindowPointer->StopRunning(true);
		//9/11/2009
		/*
		if (PasswordOK(cTemporaryInspxPassword,false))
		if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		if (!vLocalConfigurationData->vEnableEjectors)
		{
			//vLocalConfigurationData->vEnableEjectors = true;
			//vGlobaluCSerialPort->EnableDisableEjectors(vLocalConfigurationData->vEnableEjectors);
			//vMainWindowPointer->UpdateDisableEjectorsButton();
			vMainWindowPointer->EnableDisableEjectors(true);
		}
		*/
		if (vChangedLamps)
		{
			vOriginalLampStatus = vOriginalLampStatus & 0xFE;
		}
		UpdateButtons();
	}
	else
	if (vGlobaluCSerialPort)
	if (vLocalConfigurationData->vRadiationShutterInSystem)
	{
		//pulse shutter for 1 second
		SetDlgItemText(IDC_SubFunction4Button, _T("Shutter Was Pulsed"));
		WORD TempOriginalStatus = vLocalSystemData->vRadiationDoorManualControlRegister;

		if (vLocalSystemData->vRadiationDoorManualControlRegister == cRadiationShutterClose) 
		{
			vGlobaluCSerialPort->SetRadiationDoorManualControl(cRadiationShutterOpen);
			vLocalSystemData->vRadiationDoorManualControlRegister = cRadiationShutterOpen;
		}
		else
		{
			vGlobaluCSerialPort->SetRadiationDoorManualControl(cRadiationShutterClose);
			vLocalSystemData->vRadiationDoorManualControlRegister = cRadiationShutterClose;
		}
		UpdateDisplay();
		Sleep(1000);

		vLocalSystemData->vRadiationDoorManualControlRegister = TempOriginalStatus;
		if (TempOriginalStatus == cRadiationShutterClose)
			vGlobaluCSerialPort->SetRadiationDoorManualControl(cRadiationShutterClose);
		else
			vGlobaluCSerialPort->SetRadiationDoorManualControl(cRadiationShutterOpen);

		SetDlgItemText(IDC_SubFunction4Button, _T("Exercise Shutter 1 Second"));
		UpdateDisplay();
	}
}

void CXRaySourceStatusDialog::OnSubFunction5Button() 
{
	// Clear Min and Max Running
	//if (!(vTestingLamp))
	{
		vLocalSystemData->vSourceVoltageMin = 254;
		vLocalSystemData->vSourceVoltageMax = 0;
		vLocalSystemData->vSourceCurrentMin = 254;
		vLocalSystemData->vSourceCurrentMax = 0;

		vLocalSystemData->vTemperatureAirMaximum = 0xFFFF;
		vLocalSystemData->vTemperatureLiquidMaximum = 0xFFFF;
		vLocalSystemData->vTemperatureSurfaceMaximum = 0xFFFF;
		vLocalSystemData->vTemperatureAirOutsideMaximum = 0xFFFF;  
		vLocalSystemData->vTemperatureuCMaximum = 0xFFFF;
		vLocalSystemData->vCPUTemperatureMaximum = 0;

		vLocalSystemData->vTemperatureAirMinimum = 0;
		vLocalSystemData->vTemperatureLiquidMinimum = 0;
		vLocalSystemData->vTemperatureSurfaceMinimum = 0;
		vLocalSystemData->vTemperatureAirOutsideMinimum = 0;  
		vLocalSystemData->vTemperatureuCMinimum = 0;
		vLocalSystemData->vCPUTemperatureMinimum = 0xFFFF;

		if (vGlobalDXMXRayPort)
		{
			vGlobalDXMXRayPort->vFilamentCurrentMin = 9999999;
			vGlobalDXMXRayPort->vFilamentCurrentMax = 0;
			vGlobalDXMXRayPort->vTemperatureMaximum = 0;
			vGlobalDXMXRayPort->vTemperatureMinimum = 0xFFFE;
		}

		vLocalSystemData->vSourceErrorReported = false;
		UpdateDisplay();
	}
}

void CXRaySourceStatusDialog::OnSubFunction6Button() 
{
	if (!(vTestingLamp))
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		//if (vLocalSystemData->vSystemRunMode != cAlignTubeMode)
		if (vGlobalCurrentProduct)
		{
			double TempOriginalVoltage = vLocalSystemData->vRampXRayVoltageAmount;
			CString TempVoltageString = dtoa(vLocalSystemData->vRampXRayVoltageAmount * 6,1); //convert from 0 to 10 volt to 0 to 60 KV
			CNumericEntryDialog INumericEntryDialog;  
			
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = TempVoltageString;

			CString TempText("");
			TempText.LoadString(IDS_XRaySourceVoltage);

			TempText.LoadString(IDS_Product);
			//INumericEntryDialog.m_DialogTitleStaticText1 = "X-Ray Source Voltage for " + TempText + ": " + vProductEditingName;
			INumericEntryDialog.m_DialogTitleStaticText1 = "Temporary X-Ray Source Voltage";

			INumericEntryDialog.m_DialogTitleStaticText4 = "Cleared when load a product, Product Setting: " + 
				dtoa(vGlobalCurrentProduct->vXRaySourceVoltage * 6,2);

			TempText.LoadString(IDS_OriginalValue);
			//INumericEntryDialog.m_DialogTitleStaticText4 = TempText + ": " + TempVoltageString;
			CString TempText1("");
			TempText1.LoadString(IDS_MeasuredInVolts);
			INumericEntryDialog.m_UnitsString = TempText + ": " + TempVoltageString + " " + TempText1;
			INumericEntryDialog.vMinValue = cMinimumXRayVoltage;  
			INumericEntryDialog.vAllowZero = true;
			INumericEntryDialog.vMaxValue = vLocalConfigurationData->vMaximumVoltage;


			INumericEntryDialog.m_DialogTitleStaticText3 = "";
			INumericEntryDialog.m_DialogTitleStaticText2 = "Maximum Voltage: " + dtoa(vLocalConfigurationData->vMaximumVoltage,2);

			if (vLocalSystemData->vSystemRunMode == cAlignTubeMode)
			if (INumericEntryDialog.vMaxValue > 35)
			{
				INumericEntryDialog.vMaxValue = 35;
				INumericEntryDialog.m_DialogTitleStaticText2 = "Maximum Voltage in Align Tube: 35";
			}
			INumericEntryDialog.vIntegerOnly = false;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				TempVoltageString = INumericEntryDialog.vEditString;

				double TempXRayVoltageAmount = ATOF(TempVoltageString);
				double TempMaximumCurrentV = GetMaximumCurrentFromVoltage(TempXRayVoltageAmount);
				double TempMaximumCurrentP = GetMaximumCurrentFromPower(TempXRayVoltageAmount, vLocalConfigurationData->vMaximumPower);

				double TempMaximumCurrent = TempMaximumCurrentV;
				if (TempMaximumCurrentP < TempMaximumCurrentV)
					TempMaximumCurrent = TempMaximumCurrentP;

				if (TempMaximumCurrent > vLocalConfigurationData->vMaximumCurrent)
					TempMaximumCurrent = vLocalConfigurationData->vMaximumCurrent;
				if (TempXRayVoltageAmount == 0)
				if (vLocalSystemData->vRampXRayCurrentAmount)
				{ // if set to zero voltage, zero the current, lower current first with old voltage
					vLocalSystemData->vRampXRayCurrentAmount = 0;
					vMainWindowPointer->SendSourceCurrentToUController(vLocalSystemData->vRampXRayCurrentAmount);

					if (PasswordOK(cTemporaryInspxPassword,false))
						SetDlgItemText(IDC_Current,dtoa(vLocalSystemData->vRampXRayCurrentAmount,4));
					else
						SetDlgItemText(IDC_Current,dtoa(vLocalSystemData->vRampXRayCurrentAmount,1));
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nReduced current to 0 for Voltage setting of 0";
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.DoModal();
					Sleep(1);//was 100 2/9/2005
				}
				if (vLocalSystemData->vRampXRayCurrentAmount > TempMaximumCurrent)
				{// lower current first with old voltage
					vLocalSystemData->vRampXRayCurrentAmount = TempMaximumCurrent;
					vMainWindowPointer->SendSourceCurrentToUController(TempMaximumCurrent);

					//SetDlgItemText(IDC_Current,dtoa(vLocalSystemData->vRampXRayVoltageAmount,2));
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nReduced current to: " + dtoa(vLocalSystemData->vRampXRayCurrentAmount,2) +
						"\nDue to X-Ray Power Limit at new Voltage";
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.DoModal();
					Sleep(1);//was 100 2/9/2005
				}
				vLocalSystemData->vRampXRayVoltageAmount = ATOF(TempVoltageString);
				vLocalSystemData->vRampXRayVoltageAmount = vLocalSystemData->vRampXRayVoltageAmount / 6;

				vLocalSystemData->vTemporarilyChangeSource = true;
				if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
				{
					vMainWindowPointer->SendSourceSettingsToUController(
						vLocalSystemData->vRampXRayVoltageAmount,vLocalSystemData->vRampXRayCurrentAmount);
					//::PostThreadMessage(vGlobalCallingThreadID,cRampingSourceMessage,0,0);
					int TempTimerResult = SetTimer(vResetMinMaxAfterStablizeTimerHandle,7000,NULL);
					if (!TempTimerResult)
						ReportErrorMessage("Error-Reset Min Max Timer Failed",cEMailInspx,32000);
					ReportErrorMessage("Inspx Change Voltage Temporarily", cAction,0);
				}
				SetMinimumAndMaximum();
				ChangedMinimumAndMaximum();
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				PrepareToExit();
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nYou must have a current product Selected";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else if (vSubFunction6ButtonEnable == false)
	{
		ShowNoMouseClickDialogMessage();
	}
}

void CXRaySourceStatusDialog::OnSubFunction7Button() 
{
	if (!(vTestingLamp))
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		if (vGlobalCurrentProduct)
		{
			CString TempCurrentString = dtoa(vLocalSystemData->vRampXRayCurrentAmount,2);
			CNumericEntryDialog INumericEntryDialog;  
			
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = TempCurrentString;

			CString TempText;
			CString TempText1("");
			CString TempText2("");
			TempText.LoadString(IDS_Product);
			INumericEntryDialog.m_DialogTitleStaticText1 = "Temporary X-Ray Source Current";
			TempText.LoadString(IDS_OriginalValue);
			INumericEntryDialog.m_DialogTitleStaticText4 = TempText + ": " + INumericEntryDialog.vEditString;

			TempText.LoadString(IDS_MeasuredIn);
			TempText1.LoadString(IDS_MilliAmps);
			INumericEntryDialog.m_UnitsString = TempText + " " + TempText1;

			INumericEntryDialog.m_DialogTitleStaticText4 = "Cleared when load a product, Product Setting: " + dtoa(vGlobalCurrentProduct->vXRaySourceCurrent,2);

			TempText.LoadString(IDS_OriginalValue);
			TempText2.LoadString(IDS_MeasuredIn);
			TempText1.LoadString(IDS_MilliAmps);
			INumericEntryDialog.m_UnitsString = TempText + ": " + INumericEntryDialog.vEditString + " " + TempText2 + " " + TempText1;
			if ((vLocalConfigurationData->vHVPSType == c100WMonoBlockHVPS) || (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock))
			{
				INumericEntryDialog.m_DialogTitleStaticText2 = "Minimum Current for Mono Block is: " + dtoa(vGlobalDXMXRayPort->vMinimumCurrent, 2);
				INumericEntryDialog.vMinValue = vGlobalDXMXRayPort->vMinimumCurrent;
			}
			else
				INumericEntryDialog.m_DialogTitleStaticText2 = "";

			INumericEntryDialog.vMaxValue = vLocalConfigurationData->vMaximumCurrent;
			INumericEntryDialog.m_DialogTitleStaticText3 = "Maximum Current: " + dtoa(INumericEntryDialog.vMaxValue,2);

			if (vGlobalCurrentProduct->vXRaySourceVoltage == 0)
			{
				INumericEntryDialog.vMaxValue = 0;
				INumericEntryDialog.m_DialogTitleStaticText3 = "Maximum Current: 0 at 0 Voltage";
			}

			double TempMaximumCurrent = GetMaximumCurrentFromVoltage(vLocalSystemData->vRampXRayVoltageAmount * 6);
			if (INumericEntryDialog.vMaxValue > TempMaximumCurrent)
			{
				INumericEntryDialog.vMaxValue = TempMaximumCurrent;
				INumericEntryDialog.m_DialogTitleStaticText3 = "Maximum Current: " + dtoa(TempMaximumCurrent,2) + " from Voltage Limit";
			}

			TempMaximumCurrent = GetMaximumCurrentFromPower(vLocalSystemData->vRampXRayVoltageAmount * 6,vLocalConfigurationData->vMaximumPower);
			if (INumericEntryDialog.vMaxValue > TempMaximumCurrent)
			{
				INumericEntryDialog.vMaxValue = TempMaximumCurrent;
				INumericEntryDialog.m_DialogTitleStaticText3 = "Maximum Current: " + dtoa(TempMaximumCurrent,2) + " from Power Limit";
			}

			if (vLocalSystemData->vSystemRunMode == cAlignTubeMode)
			if (INumericEntryDialog.vMaxValue > 2)
			{
				INumericEntryDialog.vMaxValue = 2;
				INumericEntryDialog.m_DialogTitleStaticText3 = "Maximum Current: 2 while Aligning Tube";
			}
			if (vLocalConfigurationData->vHVPSType == cDXMHVPS)
				INumericEntryDialog.vAllowZero = true;

			INumericEntryDialog.vIntegerOnly = false;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				TempCurrentString = INumericEntryDialog.vEditString;
				vLocalSystemData->vRampXRayCurrentAmount = ATOF(INumericEntryDialog.vEditString);

				vLocalSystemData->vTemporarilyChangeSource = true;
				if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
				{
					vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount,vLocalSystemData->vRampXRayCurrentAmount);
					int TempTimerResult = SetTimer(vResetMinMaxAfterStablizeTimerHandle,7000,NULL);
					if (!TempTimerResult)
						ReportErrorMessage("Error-Reset Min Max Timer Failed",cEMailInspx,32000);
					ReportErrorMessage("Inspx Change Current Temporarily", cAction,0);
				}
				SetMinimumAndMaximum();
				ChangedMinimumAndMaximum();
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				PrepareToExit();
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nYou must have a current product Selected";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else if (vSubFunction7ButtonEnable == false)
	{
		ShowNoMouseClickDialogMessage();
	}
}

void CXRaySourceStatusDialog::OnSubFunction8Button() 
{
	if (vCyclingShutter)
		OnShutterTestButton();

	//Turn off X-Rays button was pressed
	if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) && (vLocalSystemData->vSystemRunMode != cDarkCurrentMode))
	{
		vMainWindowPointer->StopRunning(true);
		//9/11/2009
		/*
		if (PasswordOK(cTemporaryInspxPassword,false))
		if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		if (!vLocalConfigurationData->vEnableEjectors)
		{
			//vLocalConfigurationData->vEnableEjectors = true;
			//vGlobaluCSerialPort->EnableDisableEjectors(vLocalConfigurationData->vEnableEjectors);
			//vMainWindowPointer->UpdateDisableEjectorsButton();
			vMainWindowPointer->EnableDisableEjectors(true);
		}
		*/
		if (vChangedLamps)
		{
			vOriginalLampStatus = vOriginalLampStatus & 0xFE;
		}
	}
	else
	{ // Condition tube
		if (vMainWindowPointer->InterlocksOK())
		{
			vLocalSystemData->vModeToRunAfterCalibrateOffset = 0;
			BYTE TempGlobalShiftKeyDown = vGlobalShiftKeyDown;
			vGlobalShiftKeyDown = false;
			vMainWindowPointer->StartXRayTubeCondition(TempGlobalShiftKeyDown, 0);
			//OnFunction1Button();
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
	UpdateButtons();
}

void CXRaySourceStatusDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == vDisplayTimerHandle)
  {
		UpdateDisplay();
	}
	if (nIDEvent == vResetMinMaxAfterStablizeTimerHandle)
  {
		KillTimer(vResetMinMaxAfterStablizeTimerHandle);
		vLocalSystemData->vSourceVoltageMin = 254;
		vLocalSystemData->vSourceVoltageMax = 0;
		vLocalSystemData->vSourceCurrentMin = 254;
		vLocalSystemData->vSourceCurrentMax = 0;
		if (vGlobalDXMXRayPort)
		{
			vGlobalDXMXRayPort->vFilamentCurrentMin = 9999999;
			vGlobalDXMXRayPort->vFilamentCurrentMax = 0;
		}
	}
	if (nIDEvent == vCycleShutterTimerHandle)
  {
		//toggle shutter every 100 msec
		if (vLocalSystemData->vRadiationDoorManualControlRegister == cRadiationShutterClose) 
		{
			vGlobaluCSerialPort->SetRadiationDoorManualControl(cRadiationShutterOpen);
			vLocalSystemData->vRadiationDoorManualControlRegister = cRadiationShutterOpen;
		}
		else
		{
			vGlobaluCSerialPort->SetRadiationDoorManualControl(cRadiationShutterClose);
			vLocalSystemData->vRadiationDoorManualControlRegister = cRadiationShutterClose;
		}
		if (vLocalSystemData->vRadiationDoorManualControlRegister != 
			vOldRadiationDoorManualControlRegister)
		{
			vOldRadiationDoorManualControlRegister = 
				vLocalSystemData->vRadiationDoorManualControlRegister;
			if (vOldRadiationDoorManualControlRegister == cRadiationShutterOpen)
				SetDlgItemText(IDC_ShutterStatus, _T("Open"));
			else
			if (vOldRadiationDoorManualControlRegister == cRadiationShutterClose)
				SetDlgItemText(IDC_ShutterStatus, _T("Closed"));
			else
				SetDlgItemText(IDC_ShutterStatus, _T("Auto"));
		}
	}

	
	if (nIDEvent == vDarkCurrentTimerHandle)
  {
		if (vOldSourceVoltage * 6 > 68)
		if (vGlobalDXMXRayPort->vHaveNewFilamentFeedbackMonitorReading > 4) //if you got a new reading check to see if at threshold of 2
		{
			vDarkCurrent = vGlobalDXMXRayPort->vFilamentCurrentFeedbackMonitor;
			UpdateDisplay();

			if (vDarkCurrent > vDarkCurrentThreshold)  //if at threshold of 1.5 * preamp current or higher, save current value and save
			{
				vDarkCurrentCurrentLevel = vLocalSystemData->vRampXRayCurrentAmount * 1000.0;
				StopDarkCurrentMeasurement(false);
			}
			else
			{ //if not at threshold, go to next step
				vLocalSystemData->vRampXRayCurrentAmount = vLocalSystemData->vRampXRayCurrentAmount + .0005;
				vMainWindowPointer->SendSourceSettingsToUController(
					vLocalSystemData->vRampXRayVoltageAmount,vLocalSystemData->vRampXRayCurrentAmount);
				vGlobalDXMXRayPort->vHaveNewFilamentFeedbackMonitorReading = 0;
			}
		}
		if (vTakingDarkCurrentMeasurment)
		{
			KillTimer(vDarkCurrentTimerHandle);
			int TempTimerResult = SetTimer(vDarkCurrentTimerHandle,8500,NULL);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CXRaySourceStatusDialog::PreTranslateMessage(MSG* pMsg) 
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
		CWnd *TempWindow;
		if (vGlobalShiftKeyDown)
		{
			if (pMsg->wParam == 119) // F8 Key - Sub Function 8:
				OnSubFunction8Button();
			else
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
		}
		else
		switch (pMsg->wParam)
		{
			case 35: // End Key, Left Right on duo but new Image Key on Universal Overlays, Image Button
				OnTemperatureairoutsidelabel();
			break;
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
			case 119: // F8 Key - Sub Function 8:
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
			case 89:  //letter Y to clear yellow message
				vMainWindowPointer->ClearYellowWarningButton();
			break;
			case cPageDown: // Help
				PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 0, 0);
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
			break;
			case cPageUp: // Clear, but do inspx hidden dialog
			{
				if (vGlobalPasswordLevel)
				{
					PasswordOK(cNoPassword, false);
				}
				else
				{
					PasswordOK(cAnyPasswordToEnter, true);
					//9/11/2009
					/*
					if (PasswordOK(cTemporaryInspxPassword,false))
					if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
					if (!vLocalConfigurationData->vEnableEjectors)
					{
						//vLocalConfigurationData->vEnableEjectors = true;
						//vGlobaluCSerialPort->EnableDisableEjectors(vLocalConfigurationData->vEnableEjectors);
						//vMainWindowPointer->UpdateDisableEjectorsButton();
						vMainWindowPointer->EnableDisableEjectors(true);
					}
					*/
				}
				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
				UpdateButtons();
			}
			break;
//			case c5:
//				if (PasswordOK(cSuperInspxPassword,false))
//					vGlobalGuardMaster->SendResetToGuardMaster();
//			break;
			/*
			case c5:
				if (PasswordOK(cSuperInspxPassword,false))
					vMainWindowPointer->TestingOnlySendSourceCurrentToUController();
			break;
			case c6:
				if (PasswordOK(cSuperInspxPassword,false))
					vMainWindowPointer->TestingOnlySendSourceVoltageToUController();
			break;
			case c1:
				if (vGlobalDXMXRayPort)
					vGlobalDXMXRayPort->TurnOnOffXRays(true);
			break;
			case c2:
				if (vGlobalDXMXRayPort)
					vGlobalDXMXRayPort->TurnOnOffXRays(false);
			break;
			case c1: // br htest
			break;
			*/
			//case c9: // test
				//br htest
				//UserSetFilamentLimit();
			//break;
			/*
			case c1: // test
				if (vGlobalDXMXRayPort)
					vGlobalDXMXRayPort->SetInRemoteMode(false);
			break;
			case c2: // test
				if (vGlobalDXMXRayPort)
					vGlobalDXMXRayPort->SetInRemoteMode(true);
			break;
			*/
			//case c9: // test
			//	if (PasswordOK(cTemporaryInspxPassword,false))
			//	if (vGlobalDXMXRayPort)
			//	{
			//		CYesNoDialog TempYesNoDialog;
			//		TempYesNoDialog.vNoticeText = "\n\nInitialize DXM HVPS?";
			//		TempYesNoDialog.vYesButtonText = "Initialize DXM HVPS";
			//		TempYesNoDialog.vNoButtonText = "";
			//		TempYesNoDialog.vQuestionType = cConfirmQuestion;
			//		int TempResult = TempYesNoDialog.DoModal();
			//		if (TempResult == IDOK)
			//		{
			//			vGlobalDXMXRayPort->vWaitingForReply = 0;
			//			vGlobalDXMXRayPort->ClearCommandQueue();
			//			vGlobalDXMXRayPort->vRemoteMode = false;
			//			vMainWindowPointer->InitializeDXM();
			//			ReportErrorMessage("Error-Manual Reset DXM HVPS", cEMailInspx,32000);
			//		}
			//	}
			//break;
			/*
			case c4: // test
				//br htest
				if (vGlobalDXMXRayPort)
					ShowHVPSStatus();
			break;
			case c5: // test
				//br htest
				if (vGlobalDXMXRayPort)
					vGlobalDXMXRayPort->SetXRayFilamentLimit(.1);// 1 = full scale, or 4.6Amp limit
			break;
			case c6: // test
				//br htest
				if (vGlobalDXMXRayPort)
					vGlobalDXMXRayPort->SetXRayFilamentPreHeat(.33);//.2 = .5 Amp Preheat
			break;
			case c7: // test
				//br htest
				if (vGlobalDXMXRayPort)
					vGlobalDXMXRayPort->SetXRayFilamentPreHeat(.1);//.2 = .5 Amp Preheat
			break;
			case c8: // test
				//br htest
				if (vGlobalDXMXRayPort)
					vGlobalDXMXRayPort->SetXRayFilamentLimit(.5);// 1 = full scale, or 4.6Amp limit
			break;
			case c9: // test
				//br htest
				if (vGlobalDXMXRayPort)
					vGlobalDXMXRayPort->SetXRayFilamentLimit(.75);// 1 = full scale, or 4.6Amp limit
			break;
*/
			/*
			case c1: // test
				if (vLocalSystemData->vSourceInterLocks == 0)
					vLocalSystemData->vSourceInterLocks = 1;
				else
				if (vLocalSystemData->vSourceInterLocks == 0x8000)
					vLocalSystemData->vSourceInterLocks = 0;
				else
					vLocalSystemData->vSourceInterLocks =
						(vLocalSystemData->vSourceInterLocks << 1);
				vLocalSystemData->vSourceInterLocks = 
					vLocalSystemData->vSourceInterLocks & 0x3F7F;
			break;
			case c2: // test
				vLocalSystemData->vTemperatureSurface =
					vLocalSystemData->vTemperatureSurface + 0x0F;
			break;
			case c3: // test
				vLocalSystemData->vTemperatureAirInside =
					vLocalSystemData->vTemperatureAirInside + 0x0F;
			break;
			case c4: // test
				vLocalSystemData->vTemperatureLiquid =
					vLocalSystemData->vTemperatureLiquid + 0x0F;
			break;
			case c5: // test
				vLocalSystemData->vPSFault =
					vLocalSystemData->vPSFault + 0x0F;
			break;
			*/
			//br ht
			/*
			case c4:
				vLocalSystemData->vGenerateTestError = 1;
			break;
			case c5:
				vLocalSystemData->vGenerateTestError = 2;
			break;
			case c6:
				vLocalSystemData->vGenerateTestError = 3;
			break;
			case c7:
				vLocalSystemData->vGenerateTestError = 4;
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

HBRUSH CXRaySourceStatusDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	int TempControlID = pWnd->GetDlgCtrlID();
	if (TempControlID == IDC_SubFunction2Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		if (vLocalSystemData->vSystemRunMode == cMeasureSourceStrengthMode)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vRedBrush;
		}
	}

	if (TempControlID == IDC_Function1Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5);  //5 is medium large
		if (vMakeExitButtonGreen)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
	}

	if (TempControlID == IDC_Function4Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function4Button, 5);  //5 is medium large
		if (vTakingDarkCurrentMeasurment)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vRedBrush;
		}
	}
	if (TempControlID == IDC_Function5Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBrown);
		return vGlobalButtonColorBrush;
	}

	if (TempControlID == IDC_SubFunction2Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		if (vSubFunction2ButtonEnable == true)
			pDC->SetTextColor(cSupervisorTextColor);
		else
			pDC->SetTextColor(cGray);
		pDC->SetBkMode(TRANSPARENT);
		return vGlobalButtonColorBrush;
	}

	if (TempControlID == IDC_StatusLine)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_StatusLine, 4);  //5 is medium large
		if (vLocalSystemData->vYellowMessageButtonYellow)
		{
			pDC->SetTextColor(cDarkBlue);
			pDC->SetBkMode(TRANSPARENT);

			return vLocalSystemData->vYellowBrush;
		}
	}

	if (TempControlID == IDC_TemperatureAirOutsideLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureAirOutsideLabel, 4);
		if (vLocalConfigurationData->vDeltaTemperatureForFilterCloggedError == 0) //have AirConditioning
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SelectObject(vLocalSystemData->vUnderlinedFont);
			pDC->SetTextColor(cBlue);
			return vLocalSystemData->vLightBrownBrush;
		}
	}

	if (TempControlID == IDC_HVPS)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_HVPS, 4);
		if (vGlobalDXMXRayPort)
		{
			if ((vGlobalDXMXRayPort->vNoCommunications) || (!vGlobalDXMXRayPort->vRemoteMode))
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
		}
		else
		if (vGlobalMonoBlockXRayPort)
		{
			if (vGlobalDXMXRayPort->vNoCommunications)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
		}
	}

	if (TempControlID == IDC_HVPS15V)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_HVPS15V, 4);
		if (vGlobalDXMXRayPort)
		{
			if ((vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac))
			{
				if ((vGlobalDXMXRayPort->v15VoltMonitor < 20) ||  //24 volt HVPS supply below 20 volts
					(vGlobalDXMXRayPort->v15VoltMonitor > 28)) //24 volt supply above 28 volts
				{
					pDC->SetBkMode(TRANSPARENT);
					pDC->SetTextColor(cWhite);
					return vLocalSystemData->vRedBrush;
				}
			}
			else
			if (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock)
			{
				if ((vGlobalDXMXRayPort->v15VoltMonitor < 20) ||  //24 volt HVPS supply below 11 volts
					(vGlobalDXMXRayPort->v15VoltMonitor > 28))
				{
					pDC->SetBkMode(TRANSPARENT);
					pDC->SetTextColor(cWhite);
					return vLocalSystemData->vRedBrush;
				}
			}
			else
			{
				if ((vGlobalDXMXRayPort->v15VoltMonitor < 11) ||  //-15 volt HVPS supply below 11 volts
					(vGlobalDXMXRayPort->v15VoltMonitor > 18))
				{
					pDC->SetBkMode(TRANSPARENT);
					pDC->SetTextColor(cWhite);
					return vLocalSystemData->vRedBrush;
				}
			}
		}
	}

	if (TempControlID == IDC_MinRunningVoltage) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinRunningVoltage, 4);
		pDC->SetBkMode(TRANSPARENT);
		if (vOldSourceVoltageMin < vLocalSystemData->vSourceVoltageMinRedLine)
		{
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		return vLocalSystemData->vLightBrownBrush;
	}
	if ((TempControlID == IDC_MaxRunningVoltage) &&
		(vOldSourceVoltageMax > vLocalSystemData->vSourceVoltageMaxRedLine))
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		return vLocalSystemData->vRedBrush;
	}

	if (TempControlID == IDC_ShutterTestButton)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_ShutterTestButton, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vCyclingShutter)
			return vLocalSystemData->vRedBrush;
		else
			return vGlobalButtonColorBrush;
	}
	/*
  if ((TempControlID == IDC_Function3Button) &&
		(vTestingLamp & 1))
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vGreenBrush;
	}
	if ((TempControlID == IDC_Function4Button) &&
	{
		(vTestingLamp & 2))
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vGreenBrush;
	}
	if ((TempControlID == IDC_Function5Button) &&
		(vTestingLamp & 4))
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vGreenBrush;
	}
	*/
	if (TempControlID == IDC_SubFunction6Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5); 
		if (vLocalSystemData->vTemporarilyChangeSource)
		if (vLocalSystemData->vRampXRayVoltageAmount != vGlobalCurrentProduct->vXRaySourceVoltage)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
	}
	if (TempControlID == IDC_SubFunction7Button) 
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
		if ((vLocalSystemData->vTemporarilyChangeSource) &&
		(vLocalSystemData->vRampXRayCurrentAmount != vGlobalCurrentProduct->vXRaySourceCurrent))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
	}
	if (TempControlID == IDC_SubFunction3Button) 
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
	
		if ((vLocalSystemData->vTemporarilyChangeSource) && (PasswordOK(cTemporaryInspxPassword,false)))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
	}
	if (TempControlID == IDC_CurrentSetting2) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentSetting2, 4);
		if ((vLocalSystemData->vTemporarilyChangeSource) && (vGlobalCurrentProduct))
		if (vLocalSystemData->vRampXRayCurrentAmount != vGlobalCurrentProduct->vXRaySourceCurrent)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (TempControlID == IDC_VoltageSetting2) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_VoltageSetting2, 4);
		if (vLocalSystemData->vTemporarilyChangeSource)
		if (vLocalSystemData->vRampXRayVoltageAmount != vGlobalCurrentProduct->vXRaySourceVoltage)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}
	if (TempControlID == IDC_MinRunningCurrent) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinRunningCurrent, 4);
		if (vOldSourceCurrentMin < vLocalSystemData->vSourceCurrentMinRedLine)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
	}
	if (TempControlID == IDC_MaxRunningCurrent)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaxRunningCurrent, 4);
		if (vOldSourceCurrentMax > vLocalSystemData->vSourceCurrentMaxRedLine)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
	}

	if (TempControlID == IDC_SVoltageReading)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SVoltageReading, 4);	
		if (vLocalSystemData->vSourceRampedUp)
		if ((vOldSourceVoltage < vLocalSystemData->vSourceVoltageMinRedLine) ||
			(vOldSourceVoltage > vLocalSystemData->vSourceVoltageMaxRedLine))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
	}
	
	if (TempControlID == IDC_Current)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Current, 4);	
		if (vLocalSystemData->vSourceRampedUp)
		if ((vOldSourceCurrent < vLocalSystemData->vSourceCurrentMinRedLine) ||
			(vOldSourceCurrent > vLocalSystemData->vSourceCurrentMaxRedLine))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
	}

	if (TempControlID == IDC_TemperatureAirMaximum)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureAirMaximum, 4);
		if ((vOldTemperatureAirMaximum < 0xFFFE) && (vOldTemperatureAirMaximum > 1))//if got a reading, was initalized to 0xFFFF
		{
			int TempAir = ConvertToDegreesC(vOldTemperatureAirMaximum,
				vLocalConfigurationData->vTemperatureAirInsideAdjust);

			if (TempAir > vLocalConfigurationData->vMaximumAirTemperature)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
		}
	}
	
	if (TempControlID == IDC_TemperatureSurfaceMaximum)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureSurfaceMaximum, 4);
		if ((vOldTemperatureSurfaceMaximum < 0xFFFE) && (vOldTemperatureSurfaceMaximum > 1))//if got a reading, was initalized to 0xFFFF
		{
			int TempSurface = ConvertToDegreesC(vOldTemperatureSurfaceMaximum,
				vLocalConfigurationData->vTemperatureSurfaceAdjust);

			if (TempSurface > vLocalConfigurationData->vMaximumSurfaceTemperature)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
		}
	}
	if (TempControlID == IDC_TemperatureLiquidMaximum)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureLiquidMaximum, 4);
		if ((vLocalConfigurationData->vHVPSType == c100WMonoBlockHVPS) || (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock))
		{
			if (vGlobalDXMXRayPort)
			if (vLocalConfigurationData->vMaximumMonoBlockTemperature < (BYTE)vGlobalDXMXRayPort->vTemperatureMaximum)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
		}
		else
		if ((vOldTemperatureLiquidMaximum < 0xFFFE) && (vOldTemperatureLiquidMaximum > 1)) //if got a reading, was initalized to 0xFFFF
		{
			int TempLiquid = ConvertToDegreesC(vOldTemperatureLiquidMaximum, vLocalConfigurationData->vTemperatureLiquidAdjust);

			if (TempLiquid > vLocalConfigurationData->vMaximumLiquidTemperature)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
		}
	}
	if (TempControlID == IDC_TemperatureAirOutsideMaximum)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureAirOutsideMaximum, 4);
		if ((vOldTemperatureAirOutsideMaximum < 0xFFFE) && (vOldTemperatureAirOutsideMaximum > 1))//if got a reading, was initalized to 0xFFFF
		{
			int TempLiquid = ConvertToDegreesC(vOldTemperatureAirOutsideMaximum,
				vLocalConfigurationData->vTemperatureAirOutsideAdjust);

			if (TempLiquid > vLocalConfigurationData->vMaximumTemperatureAirOutside)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
		}
	}

	if (TempControlID == IDC_TemperatureuCMaximum)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureuCMaximum, 4);
		if ((vOldTemperatureuCMaximum < 0xFFFE) && (vOldTemperatureuCMaximum > 1))  //if got a reading, was initalized to 0xFFFF
		{
			int TempLiquid = ConvertToDegreesC(vOldTemperatureuCMaximum,
				vLocalConfigurationData->vTemperatureuCAdjust);

			if (TempLiquid > vLocalConfigurationData->vMaximumuCTemperature)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
		}
	}
	
/*
	if (TempControlID == IDC_CoolantPressureMaximum)
	{
		int TempPressure = ConvertToPressurePSI(vOldCoolantPressureMaximum,0);
			//vLocalConfigurationData->vCoolantPressureAdjust);

		if (TempPressure > vLocalConfigurationData->vMaximumCoolantPressure)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
	}

	if (TempControlID == IDC_CoolantPressureMinimum)
	{
		int TempPressure = ConvertToPressurePSI(vOldCoolantPressureMinimum,0);
			//vLocalConfigurationData->vCoolantPressureAdjust);

		if (TempPressure < vLocalConfigurationData->vMinimumCoolantPressure)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
	}
*/
	if (TempControlID == IDC_TemperatureAir)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureAir, 4);
		if (vOldTemperatureAir < 0xFFFE)  //if got a reading, was initalized to 0xFFFF
		{
			int TempAir = ConvertToDegreesC(vOldTemperatureAir, vLocalConfigurationData->vTemperatureAirInsideAdjust);

			if ((TempAir > vLocalConfigurationData->vMaximumAirTemperature) || (TempAir <= 0))
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
		}
	}
	
	if (TempControlID == IDC_TemperatureSurface)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureSurface, 4);
		if (vOldTemperatureSurface < 0xFFFE)  //if got a reading, was initalized to 0xFFFF
		{
			int TempSurface = ConvertToDegreesC(vOldTemperatureSurface, vLocalConfigurationData->vTemperatureSurfaceAdjust);

			if ((TempSurface > vLocalConfigurationData->vMaximumSurfaceTemperature) || (TempSurface <= 0))
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
		}
	}
	
	if (TempControlID == IDC_TemperatureAirOutside)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureAirOutside, 4);
		if (vOldTemperatureAirOutside < 0xFFFE)  //if got a reading, was initalized to 0xFFFF
		{
			int TempAirOutside = ConvertToDegreesC(vOldTemperatureAirOutside, vLocalConfigurationData->vTemperatureAirOutsideAdjust);

			if ((TempAirOutside > vLocalConfigurationData->vMaximumTemperatureAirOutside) || (TempAirOutside <= 0))
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
		}
	}

	if (TempControlID == IDC_TemperatureLiquid)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureLiquid, 4);
		if ((vLocalConfigurationData->vHVPSType == c100WMonoBlockHVPS) || (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock))
		{
			if (vGlobalDXMXRayPort)
			if (vLocalConfigurationData->vMaximumMonoBlockTemperature < (BYTE)vGlobalDXMXRayPort->vTemperature)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
		}
		else
		if (vOldTemperatureLiquid < 0xFFFE)  //if got a reading, was initalized to 0xFFFF
		{
			int TempLiquid = ConvertToDegreesC(vOldTemperatureLiquid, vLocalConfigurationData->vTemperatureLiquidAdjust);

			if ((TempLiquid > vLocalConfigurationData->vMaximumLiquidTemperature) || (TempLiquid <= 0))
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
		}
	}

	if (TempControlID == IDC_TemperatureuC)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureuC, 4);
		if (vOldTemperatureuC < 0xFFFE)  //if got a reading, was initalized to 0xFFFF
		{
			int TempTemp = ConvertToDegreesC(vOldTemperatureuC, vLocalConfigurationData->vTemperatureuCAdjust);

			if ((TempTemp > vLocalConfigurationData->vMaximumuCTemperature) || (TempTemp <= 0))
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
		}
	}

	if (TempControlID == IDC_OnOffLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_OnOffLabel, 4);
		//pDC->SelectObject(vLocalSystemData->vCurrentLargeFont);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}

	if (TempControlID == IDC_OnOff)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_OnOff, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlue);
		pDC->SetBkMode(TRANSPARENT);
		if (((vGlobalGuardMaster) && (vGlobalGuardMaster->vDetectedValidGuardMasterDevice)) && 
			(((vGlobalGuardMaster->vOldGuardMasterRawInterlockData & cInterlockOutputBits) != cInterlockOutputBits) || (vGlobalGuardMaster->vGuardMasterFaultLEDData & 3) || (vGlobalGuardMaster->vNoCommunications)))
			pDC->SelectObject(vLocalSystemData->vUnderlinedFont);

		if (((vOldSourceInterLocks & 0x0020) != 0x0020) || (((vGlobalGuardMaster) && (vGlobalGuardMaster->vDetectedValidGuardMasterDevice)) && 
			(((vGlobalGuardMaster->vOldGuardMasterRawInterlockData & cInterlockOutputBits) != cInterlockOutputBits) || (vGlobalGuardMaster->vGuardMasterFaultLEDData & 3) || (vGlobalGuardMaster->vNoCommunications))))
			return vLocalSystemData->vRedBrush;
		else
			return vLocalSystemData->vLightBrownBrush;
	}

	if (TempControlID == IDC_SubFunction4Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 7); 
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) && (vLocalSystemData->vSystemRunMode != cDarkCurrentMode))
			if (vLocalSystemData->vLastXRayCommandOn)
			{
				return vLocalSystemData->vRedBrush;
			}
		return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_SubFunction8Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 7);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
				
		if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) && (vLocalSystemData->vSystemRunMode != cDarkCurrentMode))
			if (vLocalSystemData->vLastXRayCommandOn)
			{
				pDC->SetTextColor(cButtonTextColor);
				return vLocalSystemData->vRedBrush;
			}
		if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cRed);
			return vGlobalButtonColorBrush;
		}
	}
  if (TempControlID == IDC_SubFunction1Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		if ((!vOldLastXRayCommandOn) && (vGlobalCurrentProduct))
		{
			pDC->SetAttribDC(pDC->m_hDC);
			//pDC->SelectObject(vLocalSystemData->vCurrentLargeFont);
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
				return vLocalSystemData->vYellowBrush;
		}
	}

	if (TempControlID == IDC_SourceFault)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SourceFault, 4);
		pDC->SetBkMode(TRANSPARENT);
		if (((vOldSourceInterLocks & 0x0010) != 0x0010) || ((vGlobalDXMXRayPort) && (vGlobalDXMXRayPort->vNoCommunications)) || ((vGlobalMonoBlockXRayPort) && (vGlobalMonoBlockXRayPort->vNoCommunications)))
		{
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		else
		{
			pDC->SetTextColor(cDarkBlue);
			return vLocalSystemData->vLightBrownBrush;
		}
	}

	if (TempControlID == IDC_SourceInterlocks)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SourceInterlocks, 4);
		if ((vOldSourceInterLocks & 0x0800) != 0x0800)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
	}

	if (TempControlID == IDC_SourceInterlocks2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SourceInterlocks2, 4);
		if ((vOldSourceInterLocks & 0x1000) != 0x1000)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
	}

	if (TempControlID == IDC_CoolantFlow)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CoolantFlow, 4);
		if ((vOldSourceInterLocks & 0x0400) != 0x0400)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
	}

	if (TempControlID == IDC_EStop)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EStop, 4);
		pDC->SetBkMode(TRANSPARENT);
		if ((vOldSourceInterLocks & 0x0200) != 0x0200)
		{
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_KeySwitch)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_KeySwitch, 4);
		pDC->SetBkMode(TRANSPARENT);
		if ((vOldSourceInterLocks & 0x0100) != 0x0100)
		{
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_KeySwitchLabel)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_KeySwitchLabel, 4);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_EStopLabel)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EStopLabel, 4);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_UControllerCommunications)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_UControllerCommunications, 4);
		if (vOldNoCommunicationsWithUController)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
	}

	//if piccolo then if max more than one more than min make them red, if DXM if more than 1000 difference make them red
	if (TempControlID == IDC_MinRunningFilamentCurrent) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinRunningFilamentCurrent, 4);
		if ((((vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac)) &&
			(vGlobalDXMXRayPort->vOldFilamentCurrentMin + 1 < vGlobalDXMXRayPort->vOldFilamentCurrentMax)) ||
			(vGlobalDXMXRayPort->vOldFilamentCurrentMin + 125 < vGlobalDXMXRayPort->vOldFilamentCurrentMax))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
	}
	if (TempControlID == IDC_MaxRunningFilamentCurrent)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaxRunningFilamentCurrent, 4);
		if ((((vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac)) &&
			(vGlobalDXMXRayPort->vOldFilamentCurrentMin + 1 < vGlobalDXMXRayPort->vOldFilamentCurrentMax)) ||
			(vGlobalDXMXRayPort->vOldFilamentCurrentMin + 125 < vGlobalDXMXRayPort->vOldFilamentCurrentMax))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
	}
	if (TempControlID == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}

	if (TempControlID == IDC_HVPSLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_HVPSLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_HVPSPreHeatLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_HVPSPreHeatLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_HVPS15VLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_HVPS15VLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_CurrentSetting) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentSetting, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_VoltageSetting)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_VoltageSetting, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	
	if (TempControlID == IDC_MinCurrent) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinCurrent, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_MinVoltage) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinVoltage, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_MaxCurrent) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaxCurrent, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_MaxVoltage) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaxVoltage, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_MinRunningCurrent) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinRunningCurrent, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_MaxRunningCurrent) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinRunningCurrent, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_MaxRunningVoltage) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaxRunningVoltage, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_CurrentLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_ReadingLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ReadingLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_CoolantFlowLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CoolantFlowLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_SettingLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SettingLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_LimitsLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LimitsLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_RunningLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_RunningLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_MinVLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinVLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_MaxVLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaxVLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_MinLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_MaxLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaxLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}

	if (TempControlID == IDC_TemperatureSurfaceLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureSurfaceLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureLiquidLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureLiquidLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureAirLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureAirLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureSurfaceLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureSurfaceLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureuCLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureuCLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureCPULabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureCPULabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}

	if (TempControlID == IDC_SourceVoltageLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SourceVoltageLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_SourceCurrentLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SourceCurrentLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_OnOffLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_OnOffLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_ShutterStatusLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ShutterStatusLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureAirOutsideLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureAirOutsideLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
		//(TempControlID == IDC_CoolantPressureLabel) ||
		
	if (TempControlID == IDC_MinimumLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinimumLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_MaximumLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaximumLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_SourceInterlocksLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SourceInterlocksLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_SourceFaultLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SourceFaultLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_SourceInterlocksLabel2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SourceInterlocksLabel2, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_UControllerCommunicationsLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_UControllerCommunicationsLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	//if (TempControlID == IDC_YellowLampLabel)
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinimumLabel, 4);
	//	pDC->SetBkMode(TRANSPARENT);
	//	return vLocalSystemData->vLightBrownBrush;
	//}
	//if (TempControlID == IDC_RedLampLabel)
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_RedLampLabel, 4);
	//	pDC->SetBkMode(TRANSPARENT);
	//	return vLocalSystemData->vLightBrownBrush;
	//}
	if (TempControlID == IDC_MinRunningFilamentCurrent) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinRunningFilamentCurrent, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_MaxRunningFilamentCurrent)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaxRunningFilamentCurrent, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_CoolantFlow) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CoolantFlow, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_HVPS) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_HVPS, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_HVPSPreHeat) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_HVPSPreHeat, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_HVPS15V) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_HVPS15V, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	//if (TempControlID == IDC_RedLamp) 
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_RedLamp, 4);
	//	pDC->SetBkMode(TRANSPARENT);
	//	pDC->SetTextColor(cDarkBlue);
	//	return vLocalSystemData->vLightBrownBrush;
	//}
	//if (TempControlID == IDC_YellowLamp) 
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YellowLamp, 4);
	//	pDC->SetBkMode(TRANSPARENT);
	//	pDC->SetTextColor(cDarkBlue);
	//	return vLocalSystemData->vLightBrownBrush;
	//}
	if (TempControlID == IDC_Current) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Current, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_SVoltageReading) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SVoltageReading, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_OnOff) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_OnOff, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_ShutterStatus) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ShutterStatus, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_SourceInterlocks2) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SourceInterlocks2, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_UControllerCommunications) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_UControllerCommunications, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_SourceInterlocks) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SourceInterlocks, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureSurfaceMinimum) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureSurfaceMinimum, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureAirMinimum) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureAirMinimum, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureLiquidMinimum)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureLiquidMinimum, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureuCMinimum) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureuCMinimum, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureSurfaceMaximum) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureSurfaceMaximum, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureAirMaximum) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureAirMaximum, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureLiquidMaximum)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureLiquidMaximum, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureuCMaximum)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureuCMaximum, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureSurface) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureSurface, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureAir) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureAir, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureAirOutside) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureAirOutside, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureuC) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureuC, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureAirOutsideMaximum) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureAirOutsideMaximum, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureAirOutsideMinimum) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureAirOutsideMinimum, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureCPU) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureCPU, 4);
		pDC->SetBkMode(TRANSPARENT);
		if (vLocalSystemData->vCPUTemperaturesReadOK)
		if (vOldCPUTemperature < 0xFFE)  //if got a reading, was initalized to 0xFFF
		if (!vShowingYellowMessage)
		{
			if (vOldCPUTemperature > vLocalConfigurationData->vMaximumCPUTemperature)
			{
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
		}
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureCPUMinimum) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureCPUMinimum, 4);
		pDC->SetBkMode(TRANSPARENT);
		if (vLocalSystemData->vCPUTemperaturesReadOK)
		if (vOldCPUTemperatureMinimum < 0xFFE)  //if got a reading, was initalized to 0xFFF
		if (!vShowingYellowMessage)
		{
			if (vOldCPUTemperatureMinimum > vLocalConfigurationData->vMaximumCPUTemperature)
			{
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
		}
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_TemperatureCPUMaximum)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureCPUMaximum, 4);
		pDC->SetBkMode(TRANSPARENT);
		if (vLocalSystemData->vCPUTemperaturesReadOK)
		if (vOldCPUTemperatureMaximum < 0xFFE)  //if got a reading, was initalized to 0xFFF
		if (!vShowingYellowMessage)
		{
			if (vOldCPUTemperatureMaximum > vLocalConfigurationData->vMaximumCPUTemperature)
			{
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
		}
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}

		//(TempControlID == IDC_CoolantPressure) ||
		//(TempControlID == IDC_CoolantPressureMaximum) ||
		//(TempControlID == IDC_CoolantPressureMinimum) ||
	if (TempControlID == IDC_TemperatureLiquid)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TemperatureLiquid, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}

	if (TempControlID == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);
		if (vGlobalShiftKeyDown)
			pDC->SetTextColor(cBlack);
		else
			pDC->SetTextColor(vGlobalDialogTitleColor);
	}
	if (TempControlID == IDC_XRaysOn)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaysOn, 7);
		pDC->SetTextColor(cYellow);
		//pDC->SetBkColor(cRed);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vRedBrush;
	}
	if (TempControlID == IDC_Function1Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_Function2Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_Function3Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_ChangedFilterButton)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_ChangedFilterButton, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_Function4Button) 
	{   
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function4Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vFunction4ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_Function5Button) 
	{   
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vFunction5ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_SubFunction1Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_SubFunction2Button) 
	{   
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction2ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_SubFunction3Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_SubFunction4Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_SubFunction5Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_SubFunction6Button) 
	{   
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction6ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_SubFunction7Button) 
	{   
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction7ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_SubFunction8Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	
	return hbr;
}

void CXRaySourceStatusDialog::UpdateDisplay()
{

	if (vOldRampXRayVoltageAmount != vLocalSystemData->vRampXRayVoltageAmount)
	{
		vOldRampXRayVoltageAmount = vLocalSystemData->vRampXRayVoltageAmount;
		CString TempString = dtoa(vOldRampXRayVoltageAmount * 6,1);
		SetDlgItemText(IDC_VoltageSetting,TempString);
		//SetDlgItemText(IDC_VoltageSetting2,TempString);
	}
	if (vOldRampXRayCurrentAmount != vLocalSystemData->vRampXRayCurrentAmount)
	{
		vOldRampXRayCurrentAmount = vLocalSystemData->vRampXRayCurrentAmount;
		CString TempString = dtoa(vOldRampXRayCurrentAmount,1);
		if (PasswordOK(cTemporaryInspxPassword,false))
			TempString = dtoa(vOldRampXRayCurrentAmount,4);
		SetDlgItemText(IDC_CurrentSetting,TempString);
		//SetDlgItemText(IDC_CurrentSetting2,TempString);
	}

	if (vOldGlobalPasswordOK != vGlobalPasswordLevel)
	{
		vOldGlobalPasswordOK = vGlobalPasswordLevel;

		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
		UpdateButtons();
	}

	if (vGlobalShiftKeyDown != vOldGlobalShiftKeyDown)
	{
		vOldGlobalShiftKeyDown = vGlobalShiftKeyDown;
		UpdateButtons();
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}

	if ((vOldYellowMessageButtonYellow != vLocalSystemData->vYellowMessageButtonYellow) ||
		(vOldErrorMessage != vLocalSystemData->vLastErrorMessage))
	{
		if (vLocalSystemData->vYellowMessageButtonYellow)
		{
			m_StatusLine.ShowWindow(SW_SHOW);
			vShowingYellowMessage = true;
			SetDlgItemText(IDC_StatusLine,vLocalSystemData->vLastErrorMessage);
		}
		else
		{
			m_StatusLine.ShowWindow(SW_HIDE);
			SetDlgItemText(IDC_StatusLine, _T(""));
			vShowingYellowMessage = false;
		}
		vOldYellowMessageButtonYellow = vLocalSystemData->vYellowMessageButtonYellow;
		vOldErrorMessage = vLocalSystemData->vLastErrorMessage;
	}

	if (vGlobalDXMXRayPort)
	{
		CString TempStatus = "";
		if (!vGlobalDXMXRayPort->vRemoteMode)
			TempStatus = " LOCAL";

		//if (!vGlobalDXMXRayPort->vInterLockStatus)
		//	TempStatus = TempStatus + " Open InterLock";
		//else
		//	TempStatus = TempStatus + " IL Closed";

		//hvps -15 volt monitor is 0 to 4095 represening 0 to -37.469 volts
		if (vOldDXM15VStatus != vGlobalDXMXRayPort->v15VoltMonitor)
		{
			if (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock)
			{
				vOldDXM15VStatus = vGlobalDXMXRayPort->v15VoltMonitor;
				SetDlgItemText(IDC_HVPS15V, dtoa(vOldDXM15VStatus, 1));
			}
			else
			{
				vOldDXM15VStatus = vGlobalDXMXRayPort->v15VoltMonitor;
				SetDlgItemText(IDC_HVPS15V, "-" + dtoa(vOldDXM15VStatus, 1));
			}
		}

		if (vOldFilamentPreHeatMonitor != vGlobalDXMXRayPort->vFilamentPreHeatMonitor)
		{
			vOldFilamentPreHeatMonitor = vGlobalDXMXRayPort->vFilamentPreHeatMonitor;
			SetDlgItemText(IDC_HVPSPreHeat,dtoa(vOldFilamentPreHeatMonitor / 4095.0 * 2.5,2));
		}

		TempStatus = dtoa(vGlobalDXMXRayPort->vFilamentCurrentFeedbackMonitor, 2) + TempStatus;

		if (vGlobalDXMXRayPort->vNoCommunications)
			TempStatus = "No X-Ray HVPS Comm.";
		if (TempStatus != vOldDXMStatus)
		{
			vOldDXMStatus = TempStatus;
			SetDlgItemText(IDC_HVPS,vOldDXMStatus);
		}
		
		if (vGlobalDXMXRayPort->vFilamentCurrentMin != vGlobalDXMXRayPort->vOldFilamentCurrentMin)
		{
			vGlobalDXMXRayPort->vOldFilamentCurrentMin = vGlobalDXMXRayPort->vFilamentCurrentMin;
			if (vGlobalDXMXRayPort->vOldFilamentCurrentMin > 9999998)
				SetDlgItemText(IDC_MinRunningFilamentCurrent, _T("--"));
			else
			if ((vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac))
				SetDlgItemText(IDC_MinRunningFilamentCurrent,dtoa(vGlobalDXMXRayPort->vOldFilamentCurrentMin,2));
			else
				SetDlgItemText(IDC_MinRunningFilamentCurrent,dtoa(vGlobalDXMXRayPort->vOldFilamentCurrentMin,2));
		}

		if (vGlobalDXMXRayPort->vFilamentCurrentMax != vGlobalDXMXRayPort->vOldFilamentCurrentMax)
		{
			vGlobalDXMXRayPort->vOldFilamentCurrentMax = vGlobalDXMXRayPort->vFilamentCurrentMax;

			if (vGlobalDXMXRayPort->vOldFilamentCurrentMax == 0)
				SetDlgItemText(IDC_MaxRunningFilamentCurrent, _T("--"));
			else
			if ((vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac))
				SetDlgItemText(IDC_MaxRunningFilamentCurrent,dtoa(vGlobalDXMXRayPort->vOldFilamentCurrentMax,2));
			else
				SetDlgItemText(IDC_MaxRunningFilamentCurrent,dtoa(vGlobalDXMXRayPort->vOldFilamentCurrentMax,2));
		}
	}
	else
	if (vGlobalMonoBlockXRayPort)
	{
		CString TempStatus = "";
		if (vGlobalMonoBlockXRayPort->vNoCommunications)
			TempStatus = "No X-Ray HVPS Comm.";
		if (TempStatus != vOldDXMStatus)
		{
			vOldDXMStatus = TempStatus;
			SetDlgItemText(IDC_HVPS,vOldDXMStatus);
		}
		
	}

	if (vTestingLamp)
	{
		vTestLampsOn = !vTestLampsOn;

		if (vTestingLamp & 4)
			vGlobaluCSerialPort->TurnOnOffBlueLamp(vTestLampsOn);
		if (vTestingLamp & 2)
			vGlobaluCSerialPort->TurnOnOffYellowLamp(vTestLampsOn);
		if (vTestingLamp & 1)
			vGlobaluCSerialPort->TurnOnOffRedLamp(vTestLampsOn);
		/*
		if (vTestingLamp & 4)
			vGlobaluCSerialPort->TurnOnOffBlueLamp(!(vLocalSystemData->vCurrentLampStatus & 4));
		if (vTestingLamp & 2)
			vGlobaluCSerialPort->TurnOnOffYellowLamp(!(vLocalSystemData->vCurrentLampStatus & 2));
		if (vTestingLamp & 1)
			vGlobaluCSerialPort->TurnOnOffRedLamp(!(vLocalSystemData->vCurrentLampStatus & 1));
		*/
	}

	if (vLocalSystemData->vRadiationDoorManualControlRegister != vOldRadiationDoorManualControlRegister)
	{
		vOldRadiationDoorManualControlRegister = 
			vLocalSystemData->vRadiationDoorManualControlRegister;
		if (vOldRadiationDoorManualControlRegister == cRadiationShutterOpen)
			SetDlgItemText(IDC_ShutterStatus, _T("Open"));
		else
		if (vOldRadiationDoorManualControlRegister == cRadiationShutterClose)
			SetDlgItemText(IDC_ShutterStatus, _T("Closed"));
		else
			SetDlgItemText(IDC_ShutterStatus, _T("Auto"));
	}

	if (vOldLastXRayCommandOn != vLocalSystemData->vLastXRayCommandOn)
	{
		vOldLastXRayCommandOn = vLocalSystemData->vLastXRayCommandOn;
		//CWnd *TempWindow = CWnd::GetDlgItem(IDC_MinRunningVoltage);
		//TempWindow->Invalidate(false);
		//TempWindow = CWnd::GetDlgItem(IDC_MaxRunningVoltage);
		//TempWindow->Invalidate(false);
		//TempWindow = CWnd::GetDlgItem(IDC_MinRunningCurrent);
		//TempWindow->Invalidate(false);
		//TempWindow = CWnd::GetDlgItem(IDC_MaxRunningCurrent);
		//TempWindow->Invalidate(false);
		//TempWindow = CWnd::GetDlgItem(IDC_SVoltageReading);
		//TempWindow->Invalidate(false);
		//TempWindow = CWnd::GetDlgItem(IDC_Current);
		//TempWindow->Invalidate(false);
		/*
		if (vGlobalCurrentProduct)
		if (vOldLastXRayCommandOn)
		{
			m_SubFunction1Button.ShowWindow(SW_HIDE);
			m_SubFunction4Button.ShowWindow(SW_SHOW);
			m_SubFunction8Button.ShowWindow(SW_SHOW);
			m_ArrowLeft1.ShowWindow(SW_HIDE);
			m_ArrowLeft4.ShowWindow(SW_SHOW);
			m_ArrowRight4.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_SubFunction1Button,"");
			m_SubFunction4Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			SetDlgItemText(IDC_SubFunction4Button,"Stop");
			SetDlgItemText(IDC_SubFunction8Button,"Stop");
		}
		else
		{
			m_SubFunction1Button.ShowWindow(SW_SHOW);
			if (PasswordOK(cTemporaryInspxPassword,false))
				SetDlgItemText(IDC_SubFunction4Button,"Reset Source Duration!");
			else
			{
				m_SubFunction4Button.ShowWindow(SW_HIDE);
				m_ArrowLeft4.ShowWindow(SW_HIDE);
				m_SubFunction8Button.ShowWindow(SW_HIDE);
			}
			m_ArrowLeft1.ShowWindow(SW_SHOW);
			m_ArrowRight4.ShowWindow(SW_HIDE);
			SetDlgItemText(IDC_SubFunction1Button,"Run");
			//SetDlgItemText(IDC_SubFunction4Button,"");
			//SetDlgItemText(IDC_SubFunction8Button,"");
		}
		*/
		UpdateButtons();
	}
	if (vOldXRayOnNotice != vLocalSystemData->vXRayOnNotice)
	{
		vOldXRayOnNotice = vLocalSystemData->vXRayOnNotice;
		if (vOldXRayOnNotice)
			m_XRaysOn.ShowWindow(SW_SHOW);
		else
			m_XRaysOn.ShowWindow(SW_HIDE);
	}
	if ((vOldRampXRayVoltageAmount != vLocalSystemData->vRampXRayVoltageAmount) || (vOldRampXRayCurrentAmount != vLocalSystemData->vRampXRayCurrentAmount))
		SetMinimumAndMaximum();

	if (vOldMeasureSourceStrength)
	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	{
		SetDlgItemText(IDC_SubFunction2Button, _T("Measure Source Strength!"));
		vOldMeasureSourceStrength = false;
	}
		
	if ((vLocalConfigurationData->vHVPSType == c100WMonoBlockHVPS) || (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock))
	{
		if (vGlobalDXMXRayPort)
		if (vGlobalDXMXRayPort->vOldTemperature != vGlobalDXMXRayPort->vTemperature)
		{
			vGlobalDXMXRayPort->vOldTemperature = vGlobalDXMXRayPort->vTemperature;
			double TempDegreesF = (vGlobalDXMXRayPort->vOldTemperature * 9 / 5) + 32;
			if (vGlobalDXMXRayPort->vOldTemperature == 0)
				SetDlgItemText(IDC_TemperatureLiquid, _T("--"));
			else
				SetDlgItemText(IDC_TemperatureLiquid,	dtoa((BYTE)vGlobalDXMXRayPort->vOldTemperature, 0) + "`C, " + dtoa(TempDegreesF, 0) + "`F");
		}
	}
	else
	if (vOldTemperatureLiquid != vLocalSystemData->vTemperatureLiquid)
	{
		vOldTemperatureLiquid = vLocalSystemData->vTemperatureLiquid;
		SetDlgItemText(IDC_TemperatureLiquid,	ConvertToDegreesCString(vOldTemperatureLiquid, vLocalConfigurationData->vTemperatureLiquidAdjust));
	}

	if (vOldTemperatureSurface != vLocalSystemData->vTemperatureSurface)
	{
		vOldTemperatureSurface = vLocalSystemData->vTemperatureSurface;
		SetDlgItemText(IDC_TemperatureSurface,ConvertToDegreesCString(vOldTemperatureSurface, vLocalConfigurationData->vTemperatureSurfaceAdjust));
	}

	if (vOldTemperatureAir != vLocalSystemData->vTemperatureAirInside)
	{
		vOldTemperatureAir = vLocalSystemData->vTemperatureAirInside;
		SetDlgItemText(IDC_TemperatureAir,ConvertToDegreesCString(vOldTemperatureAir, vLocalConfigurationData->vTemperatureAirInsideAdjust));
	}

	if (vOldTemperatureAirOutside != vLocalSystemData->vTemperatureAirOutside)
	{
		vOldTemperatureAirOutside = vLocalSystemData->vTemperatureAirOutside;
		SetDlgItemText(IDC_TemperatureAirOutside,ConvertToDegreesCString(vOldTemperatureAirOutside, vLocalConfigurationData->vTemperatureAirOutsideAdjust));
	}
/*
	if (vOldCoolantPressure != vLocalSystemData->vCoolantPressure)
	{
		vOldCoolantPressure = vLocalSystemData->vCoolantPressure;
		SetDlgItemText(IDC_CoolantPressure,ConvertToPressurePSIString(vOldCoolantPressure,
			0)); //vLocalConfigurationData->vCoolantPressureAdjust));
	}
*/
	if (vOldTemperatureSurfaceMaximum != vLocalSystemData->vTemperatureSurfaceMaximum)
	{
		vOldTemperatureSurfaceMaximum = vLocalSystemData->vTemperatureSurfaceMaximum;
		SetDlgItemText(IDC_TemperatureSurfaceMaximum,ConvertToDegreesCString(
			vOldTemperatureSurfaceMaximum, vLocalConfigurationData->vTemperatureSurfaceAdjust));
	}

	if ((vLocalConfigurationData->vHVPSType == c100WMonoBlockHVPS) || (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock))
	{
		if (vGlobalDXMXRayPort)
		if (vGlobalDXMXRayPort->vOldTemperatureMaximum != vGlobalDXMXRayPort->vTemperatureMaximum)
		{
			vGlobalDXMXRayPort->vOldTemperatureMaximum = vGlobalDXMXRayPort->vTemperatureMaximum;
			if (vGlobalDXMXRayPort->vTemperatureMaximum < 1)
				SetDlgItemText(IDC_TemperatureLiquidMaximum, _T("--"));
			else
			{
				double TempDegreesF = (vGlobalDXMXRayPort->vOldTemperatureMaximum * 9 / 5) + 32;
				SetDlgItemText(IDC_TemperatureLiquidMaximum,	dtoa((BYTE)vGlobalDXMXRayPort->vOldTemperatureMaximum, 0) + "`C, " + dtoa(TempDegreesF, 0) + "`F");
			}
		}
	}
	else
	if (vOldTemperatureLiquidMaximum != vLocalSystemData->vTemperatureLiquidMaximum)
	{
		vOldTemperatureLiquidMaximum = vLocalSystemData->vTemperatureLiquidMaximum;
		vCoolantTemperatureMaximum = ConvertToDegreesCString(vOldTemperatureLiquidMaximum, vLocalConfigurationData->vTemperatureLiquidAdjust);

		SetDlgItemText(IDC_TemperatureLiquidMaximum,vCoolantTemperatureMaximum);
	}

	if (vOldTemperatureAirMaximum != vLocalSystemData->vTemperatureAirMaximum)
	{
		vOldTemperatureAirMaximum = vLocalSystemData->vTemperatureAirMaximum;
		SetDlgItemText(IDC_TemperatureAirMaximum,ConvertToDegreesCString(vOldTemperatureAirMaximum, vLocalConfigurationData->vTemperatureAirInsideAdjust));
	}

	if (vOldTemperatureAirOutsideMaximum != vLocalSystemData->vTemperatureAirOutsideMaximum)
	{
		vOldTemperatureAirOutsideMaximum = vLocalSystemData->vTemperatureAirOutsideMaximum;
		SetDlgItemText(IDC_TemperatureAirOutsideMaximum,ConvertToDegreesCString(vOldTemperatureAirOutsideMaximum, vLocalConfigurationData->vTemperatureAirOutsideAdjust));
	}
/*
	if (vOldCoolantPressureMaximum != vLocalSystemData->vCoolantPressureMaximum)
	{
		vOldCoolantPressureMaximum = vLocalSystemData->vCoolantPressureMaximum;
		SetDlgItemText(IDC_CoolantPressureMaximum,ConvertToPressurePSIString(
			vOldCoolantPressureMaximum, 0)); //vLocalConfigurationData->vCoolantPressureAdjust));
	}
*/
	if (vOldTemperatureSurfaceMinimum != vLocalSystemData->vTemperatureSurfaceMinimum)
	{
		vOldTemperatureSurfaceMinimum = vLocalSystemData->vTemperatureSurfaceMinimum;
		SetDlgItemText(IDC_TemperatureSurfaceMinimum,ConvertToDegreesCString(
			vOldTemperatureSurfaceMinimum, vLocalConfigurationData->vTemperatureSurfaceAdjust));
	}

	if ((vLocalConfigurationData->vHVPSType == c100WMonoBlockHVPS) || (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock))
	{
		if (vGlobalDXMXRayPort)
		if (vGlobalDXMXRayPort->vOldTemperatureMinimum != vGlobalDXMXRayPort->vTemperatureMinimum)
		{
			vGlobalDXMXRayPort->vOldTemperatureMinimum = vGlobalDXMXRayPort->vTemperatureMinimum;
			if (vGlobalDXMXRayPort->vTemperatureMinimum > 100)
				SetDlgItemText(IDC_TemperatureLiquidMinimum, _T("-- "));
			else
			{
				double TempDegreesF = (vGlobalDXMXRayPort->vOldTemperatureMinimum * 9 / 5) + 32;
				SetDlgItemText(IDC_TemperatureLiquidMinimum,	dtoa(vGlobalDXMXRayPort->vOldTemperatureMinimum, 0) + "`C, " + dtoa(TempDegreesF, 0) + "`F");
			}
		}
	}
	else
	if (vOldTemperatureLiquidMinimum != vLocalSystemData->vTemperatureLiquidMinimum)
	{
		vOldTemperatureLiquidMinimum = vLocalSystemData->vTemperatureLiquidMinimum;
		SetDlgItemText(IDC_TemperatureLiquidMinimum,ConvertToDegreesCString(vOldTemperatureLiquidMinimum, vLocalConfigurationData->vTemperatureLiquidAdjust));
	}

	if (vOldTemperatureAirMinimum != vLocalSystemData->vTemperatureAirMinimum)
	{
		vOldTemperatureAirMinimum = vLocalSystemData->vTemperatureAirMinimum;
		SetDlgItemText(IDC_TemperatureAirMinimum,ConvertToDegreesCString(vOldTemperatureAirMinimum, vLocalConfigurationData->vTemperatureAirInsideAdjust));
	}

	if (vOldTemperatureAirOutsideMinimum != vLocalSystemData->vTemperatureAirOutsideMinimum)
	{
		vOldTemperatureAirOutsideMinimum = vLocalSystemData->vTemperatureAirOutsideMinimum;
		SetDlgItemText(IDC_TemperatureAirOutsideMinimum,ConvertToDegreesCString(vOldTemperatureAirOutsideMinimum, vLocalConfigurationData->vTemperatureAirOutsideAdjust));
	}

	if (vOldTemperatureuC != vLocalSystemData->vTemperatureuC)
	{
		vOldTemperatureuC = vLocalSystemData->vTemperatureuC;
		SetDlgItemText(IDC_TemperatureuC,ConvertToDegreesCString(vOldTemperatureuC, vLocalConfigurationData->vTemperatureuCAdjust));
	}

	if (vOldTemperatureuCMaximum != vLocalSystemData->vTemperatureuCMaximum)
	{
		vOldTemperatureuCMaximum = vLocalSystemData->vTemperatureuCMaximum;
		SetDlgItemText(IDC_TemperatureuCMaximum,ConvertToDegreesCString(vOldTemperatureuCMaximum, vLocalConfigurationData->vTemperatureuCAdjust));
	}

	if (vOldTemperatureuCMinimum != vLocalSystemData->vTemperatureuCMinimum)
	{
		vOldTemperatureuCMinimum = vLocalSystemData->vTemperatureuCMinimum;
		SetDlgItemText(IDC_TemperatureuCMinimum,ConvertToDegreesCString(vOldTemperatureuCMinimum, vLocalConfigurationData->vTemperatureuCAdjust));
	}
/*
	if (vOldCoolantPressureMinimum != vLocalSystemData->vCoolantPressureMinimum)
	{
		vOldCoolantPressureMinimum = vLocalSystemData->vCoolantPressureMinimum;
		SetDlgItemText(IDC_CoolantPressureMinimum,ConvertToPressurePSIString(
			vOldCoolantPressureMinimum, 0)); //vLocalConfigurationData->vCoolantPressureAdjust));
	}
*/
	if (vOldSourceVoltage != vLocalSystemData->vSourceVoltage)
	{
		vOldSourceVoltage = vLocalSystemData->vSourceVoltage;
		CString TempString("");
		TempString = "";
		TempString = dtoa(vOldSourceVoltage * 6,1);
		SetDlgItemText(IDC_SVoltageReading,TempString);
	}
	if (vOldSourceCurrent != vLocalSystemData->vSourceCurrent)
	{
		vOldSourceCurrent = vLocalSystemData->vSourceCurrent;
		if (PasswordOK(cTemporaryInspxPassword,false))
			SetDlgItemText(IDC_Current,dtoa(vOldSourceCurrent, 4));
		else
			SetDlgItemText(IDC_Current,dtoa(vOldSourceCurrent, 1));
	}
	if (vOldSourceVoltageMin != vLocalSystemData->vSourceVoltageMin)
	{
		vOldSourceVoltageMin = vLocalSystemData->vSourceVoltageMin;
		if (vOldSourceVoltageMin > 200)
			SetDlgItemText(IDC_MinRunningVoltage, _T("--"));
		else
			SetDlgItemText(IDC_MinRunningVoltage,dtoa(vOldSourceVoltageMin * 6,1));
		//if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) || 
		//	(vOldSourceVoltageMin != 100))
		//	SetDlgItemText(IDC_MinRunningVoltage,TempString);
		//else
		//	SetDlgItemText(IDC_MinRunningVoltage,"--");
	}
	if (vOldSourceCurrentMin != vLocalSystemData->vSourceCurrentMin)
	{
		vOldSourceCurrentMin = vLocalSystemData->vSourceCurrentMin;
		if (vOldSourceCurrentMin > 200)
			SetDlgItemText(IDC_MinRunningCurrent, _T("--"));
		else
			SetDlgItemText(IDC_MinRunningCurrent,dtoa(vOldSourceCurrentMin, 1));
		//if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) || 
		//	(vOldSourceCurrentMin != 10))
		//	SetDlgItemText(IDC_MinRunningCurrent,TempString);
		//else
		//	SetDlgItemText(IDC_MinRunningCurrent,"--");
	}
	if (vOldSourceVoltageMax != vLocalSystemData->vSourceVoltageMax)
	{
		vOldSourceVoltageMax = vLocalSystemData->vSourceVoltageMax;
		if (vOldSourceVoltageMax == 0)
			SetDlgItemText(IDC_MaxRunningVoltage, _T("--"));
		else
			SetDlgItemText(IDC_MaxRunningVoltage,dtoa(vOldSourceVoltageMax * 6,1));
	}
	if (vOldSourceCurrentMax != vLocalSystemData->vSourceCurrentMax)
	{
		vOldSourceCurrentMax = vLocalSystemData->vSourceCurrentMax;
		if (vOldSourceCurrentMax == 0)
			SetDlgItemText(IDC_MaxRunningCurrent, _T("--"));
		else
			SetDlgItemText(IDC_MaxRunningCurrent,dtoa(vOldSourceCurrentMax, 1));
	}

	if ((vOldSourceInterLocks != vLocalSystemData->vSourceInterLocks) ||
		((vGlobalGuardMaster) && (vGlobalGuardMaster->vDetectedValidGuardMasterDevice) && 
		((vGlobalGuardMaster->vGuardMasterRawInterlockData != vOldDetectedValidGuardMasterDevice) || (vGlobalGuardMaster->vGuardMasterFaultLEDData != vOldGuardMasterFaultLEDData) ||
		(vOldlGuardMasterNoCommunications != vGlobalGuardMaster->vNoCommunications))))
	{
		if (vGlobalGuardMaster)
		{
			vOldlGuardMasterNoCommunications = vGlobalGuardMaster->vNoCommunications;
			vOldDetectedValidGuardMasterDevice = vGlobalGuardMaster->vGuardMasterRawInterlockData;
			vOldGuardMasterFaultLEDData = vGlobalGuardMaster->vGuardMasterFaultLEDData;
		}
		vOldSourceInterLocks = vLocalSystemData->vSourceInterLocks;

		if ((vOldSourceInterLocks & 0x0800) == 0x0800)
			SetDlgItemText(IDC_SourceInterlocks, _T("OK"));  //interlock Side Shield
		else
		{
			if ((vGlobalGuardMaster) && (vGlobalGuardMaster->vDetectedValidGuardMasterDevice))
			{
				if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) || 
					(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
				{
					CString TempText = "";
					if ((vGlobalGuardMaster->vGuardMasterRawInterlockData & cBackDoorMaskBits) != cBackDoorMaskBits)
						TempText = "Center";
					if ((vGlobalGuardMaster->vGuardMasterRawInterlockData & cLeftConveyorCoversMaskBits) != cLeftConveyorCoversMaskBits)
					{
						if (TempText.GetLength() > 2)
							TempText = TempText + ", ";
						TempText = TempText + "Left";
					}
					if ((vGlobalGuardMaster->vGuardMasterRawInterlockData & cRightConveyorCoversMaskBits) != cRightConveyorCoversMaskBits)
					{
						if (TempText.GetLength() > 2)
							TempText = TempText + ", ";
						TempText = TempText + "Right";
					}
					if (TempText.GetLength() > 2)
						TempText = TempText + " Conveyor Door Open";
					SetDlgItemText(IDC_SourceInterlocks, TempText);
				}
				else
				{
					CString TempText = "";
					if ((vGlobalGuardMaster->vGuardMasterRawInterlockData & cTubeCoverMaskBits) != cTubeCoverMaskBits)
						TempText = TempText + "X-Ray Tube Cover";

					if (((vGlobalGuardMaster->vGuardMasterRawInterlockData & cLeftConveyorCoversMaskBits) != cLeftConveyorCoversMaskBits) &&
						((vGlobalGuardMaster->vGuardMasterRawInterlockData & cRightConveyorCoversMaskBits) != cRightConveyorCoversMaskBits))
					{
						if (TempText.GetLength() > 2)
							TempText = TempText + ", ";
						TempText = TempText + "Both Conveyor Door";
					}
					else
					if ((vGlobalGuardMaster->vGuardMasterRawInterlockData & cLeftConveyorCoversMaskBits) != cLeftConveyorCoversMaskBits)
					{
						if (TempText.GetLength() > 2)
							TempText = TempText + ", ";
						TempText = TempText + "Left Conveyor Door";
					}
					else
					{
						if (TempText.GetLength() > 2)
							TempText = TempText + ", ";
						TempText = TempText + "Right Conveyor Door";
					}
					SetDlgItemText(IDC_SourceInterlocks, TempText);
				}
			}
			else
				SetDlgItemText(IDC_SourceInterlocks, _T("Open"));
		}

		if ((vOldSourceInterLocks & 0x1000) == 0x1000)
			SetDlgItemText(IDC_SourceInterlocks2, _T("OK"));  //interlock Front or Rear Door
		else
		{
			if ((vGlobalGuardMaster) && (vGlobalGuardMaster->vDetectedValidGuardMasterDevice))
			{
				if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) || (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
				{
					if (((vGlobalGuardMaster->vGuardMasterRawInterlockData & cFrontDoorMaskBits) != cFrontDoorMaskBits) && ((vGlobalGuardMaster->vGuardMasterRawInterlockData & cTubeCoverMaskBits) != cTubeCoverMaskBits))
						SetDlgItemText(IDC_SourceInterlocks2, _T("Cabinet and Detector Doors Open"));
					else
					if ((vGlobalGuardMaster->vGuardMasterRawInterlockData & cFrontDoorMaskBits) != cFrontDoorMaskBits)
						SetDlgItemText(IDC_SourceInterlocks2, _T("Cabinet Door Open"));
					else
					if ((vGlobalGuardMaster->vGuardMasterRawInterlockData & cTubeCoverMaskBits) != cTubeCoverMaskBits)
							SetDlgItemText(IDC_SourceInterlocks2, _T("Detector Door Open"));
					else
						SetDlgItemText(IDC_SourceInterlocks2, _T("???"));
				}
				else //not a top down ScanTrac
				{
					if (((vGlobalGuardMaster->vGuardMasterRawInterlockData & cFrontDoorMaskBits) != cFrontDoorMaskBits) && ((vGlobalGuardMaster->vOldGuardMasterRawInterlockData & cBackDoorMaskBitB) != cBackDoorMaskBitB))
						SetDlgItemText(IDC_SourceInterlocks2, _T("Both Doors Open"));
					else
					if ((vGlobalGuardMaster->vGuardMasterRawInterlockData & cFrontDoorMaskBits) != cFrontDoorMaskBits)
						SetDlgItemText(IDC_SourceInterlocks2, _T("Front Door Open"));
					else
					if ((vGlobalGuardMaster->vGuardMasterRawInterlockData & cBackDoorMaskBitB) != cBackDoorMaskBitB)
					{
						if (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)
							SetDlgItemText(IDC_SourceInterlocks2, _T("Side Door Open"));
						else
							SetDlgItemText(IDC_SourceInterlocks2, _T("Rear Door Open"));
					}
					else
						SetDlgItemText(IDC_SourceInterlocks2, _T("???"));
				}
			}
			else
				SetDlgItemText(IDC_SourceInterlocks2, _T("Open"));
		}

		if ((vOldSourceInterLocks & 0x0100) == 0x0100)
			SetDlgItemText(IDC_KeySwitch, _T("OK"));
		else
			SetDlgItemText(IDC_KeySwitch, _T("Off"));

		if ((vOldSourceInterLocks & 0x0200) == 0x0200)
			SetDlgItemText(IDC_EStop, _T("OK"));
		else
			SetDlgItemText(IDC_EStop, _T("Stopped"));
		
		if ((vOldSourceInterLocks & 0x0400) == 0x0400)
			SetDlgItemText(IDC_CoolantFlow, _T("OK"));
		else
		{
			if (vLocalConfigurationData->vUseExternalInterlock)
				SetDlgItemText(IDC_CoolantFlow, _T("Open"));
			else
				SetDlgItemText(IDC_CoolantFlow, _T("Fault"));
		}

		CString TempText = "";
		if ((vGlobalGuardMaster) && (vGlobalGuardMaster->vDetectedValidGuardMasterDevice))
		{
			if (vGlobalGuardMaster->vNoCommunications)
				TempText = TempText + ", GM = No Comm";
			else
			if ((vGlobalGuardMaster->vOldGuardMasterRawInterlockData & cInterlockOutputBits) == cInterlockOutputBits)
				TempText = TempText + ", GM = OK";
			else
				TempText = TempText + ", GM = Open";

			if (vGlobalGuardMaster->vGuardMasterFaultLEDData & 1)
				TempText = TempText + ", Major Fault";
			if (vGlobalGuardMaster->vGuardMasterFaultLEDData & 2)
				TempText = TempText + ", Minor Fault";
		}

		if ((vOldSourceInterLocks & 0x0020) == 0x0020)  //HV Power Supply Interlock
			SetDlgItemText(IDC_OnOff, "OK" + TempText);// + WordToHex(vLocalSystemData->vSourceInterLocks));
		else
			SetDlgItemText(IDC_OnOff, "Open" + TempText);// + WordToHex(vLocalSystemData->vSourceInterLocks));
	}
	CString TempText = "";
	if (((vGlobalDXMXRayPort) && (vGlobalDXMXRayPort->vNoCommunications)) || ((vGlobalMonoBlockXRayPort) && (vGlobalMonoBlockXRayPort->vNoCommunications))) //spellman HVPS source fault indicator
		TempText = "No Comm";
	else
	if ((vOldSourceInterLocks & 0x0010) == 0x0010) 
		TempText = "OK";
	else
		TempText = "Fault";

	if (TempText != vOldSourceFault)
	{
		vOldSourceFault = TempText;
		SetDlgItemText(IDC_SourceFault, vOldSourceFault);
	}

	if (vOldNoCommunicationsWithUController != vLocalSystemData->vNoCommunicationsWithUController)
	{
		vOldNoCommunicationsWithUController = vLocalSystemData->vNoCommunicationsWithUController;
		if ((vLocalSystemData->vSourceInterLocks & 0x0040) != 0x0040)
		{
			if ((!vLocalSystemData->vFPGAVersion9Point1OrHigher) && (vLocalSystemData->vFPGAMajorVersionNumber))
				SetDlgItemText(IDC_UControllerCommunications, _T("Shutting Down"));
			else
				SetDlgItemText(IDC_UControllerCommunications, _T("Fault"));
		}
		else
		if (!vOldNoCommunicationsWithUController)
			SetDlgItemText(IDC_UControllerCommunications, _T("OK"));
		else
			SetDlgItemText(IDC_UControllerCommunications, _T("Fault"));
	}
	if (vLocalSystemData->vCPUTemperaturesReadOK == true)
	{
		DisplayCPUTemperatures();
	}
}

void CXRaySourceStatusDialog::UpdateButtons()
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac) || (vLocalConfigurationData->vScanTracType == cUpSideDownSoloScanTrac) || 
			(vLocalConfigurationData->vScanTracType == cFermataScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac))
			m_ChangedFilterButton.ShowWindow(SW_SHOW);
		else
			m_ChangedFilterButton.ShowWindow(SW_HIDE);
	}

	if ((PasswordOK(cTemporaryInspxPassword,false)) && (vLocalSystemData->vSystemRunMode == cStoppedSystemMode) && (vGlobaluCSerialPort) && 
		(vLocalConfigurationData->vRadiationShutterInSystem))
	{
		m_ShutterTestButton.ShowWindow(SW_SHOW);
		if (vCyclingShutter)
			SetDlgItemText(IDC_ShutterTestButton,  _T("Stop Test"));
		else
			SetDlgItemText(IDC_ShutterTestButton,  _T("Test Shutter"));
	}
	else
		m_ShutterTestButton.ShowWindow(SW_HIDE);

	if ((vGlobalDXMXRayPort) && (PasswordOK(cTemporaryInspxPassword,false)))
	{
		m_HVPS.ShowWindow(SW_SHOW);
		m_HVPSLabel.ShowWindow(SW_SHOW);
		//m_HVPSPreHeat.ShowWindow(SW_SHOW);
		//m_HVPSPreHeatLabel.ShowWindow(SW_SHOW);
		m_HVPS15V.ShowWindow(SW_SHOW);
		m_HVPS15VLabel.ShowWindow(SW_SHOW);
		m_MaxRunningFilamentCurrent.ShowWindow(SW_SHOW);
		m_MinRunningFilamentCurrent.ShowWindow(SW_SHOW);

		m_Function4Button.ShowWindow(SW_SHOW);
		if ((vLocalSystemData->vSystemRunMode == cStoppedSystemMode) || (vLocalSystemData->vSystemRunMode == cDarkCurrentMode))
			vFunction4ButtonEnable = true;	//m_Function4Button.EnableWindow(true);
		else
			vFunction4ButtonEnable = false;	//m_Function4Button.EnableWindow(false);
	}
	else
	{
		m_MaxRunningFilamentCurrent.ShowWindow(SW_HIDE);
		m_MinRunningFilamentCurrent.ShowWindow(SW_HIDE);
		m_HVPS.ShowWindow(SW_HIDE);
		m_HVPSLabel.ShowWindow(SW_HIDE);
		//m_HVPSPreHeat.ShowWindow(SW_HIDE);
		//m_HVPSPreHeatLabel.ShowWindow(SW_HIDE);
		m_HVPS15V.ShowWindow(SW_HIDE);
		m_HVPS15VLabel.ShowWindow(SW_HIDE);
		m_Function4Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());
	}

	if ((vLocalSystemData->vSystemRunMode == cStoppedSystemMode) && (vGlobalCurrentProduct))
	{
		m_SubFunction1Button.ShowWindow(SW_SHOW);
		m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_SubFunction1Button, _T("Run Diagnostic Mode"));
		m_SubFunction8Button.ShowWindow(SW_SHOW);
		m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		if (vGlobalShiftKeyDown)
			SetDlgItemText(IDC_SubFunction8Button,  _T("Burn-in\nX-Ray\nTube!"));
		else
			SetDlgItemText(IDC_SubFunction8Button,  _T("Condition X-Ray\nTube!"));

	}
	else
	if (vLocalSystemData->vSystemRunMode == cDarkCurrentMode)
	{
		m_SubFunction8Button.ShowWindow(SW_HIDE);
		m_SubFunction1Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
		this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
	}
	else
	if (vGlobalCurrentProduct)
	{
		m_SubFunction1Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
		m_SubFunction8Button.ShowWindow(SW_SHOW);
		m_SubFunction8Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		SetDlgItemText(IDC_SubFunction8Button, _T("Stop"));
	}
	else
	{
		m_SubFunction1Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());

		m_SubFunction8Button.ShowWindow(SW_SHOW);
		m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		if (vGlobalShiftKeyDown)
			SetDlgItemText(IDC_SubFunction8Button, _T("Burn-in\nX-Ray\nTube!"));
		else
			SetDlgItemText(IDC_SubFunction8Button, _T("Condition X-Ray\nTube!"));
	}

	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	{
		if ((vGlobaluCSerialPort) && (vLocalConfigurationData->vRadiationShutterInSystem))
		{
			m_SubFunction4Button.ShowWindow(SW_SHOW);
			m_SubFunction4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction4Button, _T("Exercise Shutter 1 Second"));
		}
		else
		{
			m_SubFunction4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
		}
	}
	else
	if (vLocalSystemData->vSystemRunMode == cDarkCurrentMode)
	{
		m_SubFunction4Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
	}
	else
	{
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		m_SubFunction4Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		SetDlgItemText(IDC_SubFunction4Button, _T("Stop"));
	}

	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		vFunction5ButtonEnable = true;	//m_Function5Button.EnableWindow(true);
	else
		vFunction5ButtonEnable = false;	//m_Function5Button.EnableWindow(false);

	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		//m_SubFunction3Button.ShowWindow(SW_SHOW);
		//m_ArrowLeft3.ShowWindow(SW_SHOW);
		CString TempText("");
		if ((vLocalSystemData->vTemporarilyChangeSource) && (PasswordOK(cTemporaryInspxPassword,false)))
			TempText = "Return to Product Settings";
		else
			TempText = "Show\nSource\nLog";
		SetDlgItemText(IDC_SubFunction3Button,TempText);

		m_SubFunction6Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction6Button, _T("Change Temporary Voltage!"));

		m_SubFunction7Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction7Button, _T("Change Temporary Current!"));
		m_VoltageSetting2.ShowWindow(SW_SHOW);
		m_CurrentSetting2.ShowWindow(SW_SHOW);
		if (vGlobalCurrentProduct)
			vSubFunction7ButtonEnable = true;	//m_SubFunction7Button.EnableWindow(true);
		else
			vSubFunction7ButtonEnable = false;	//m_SubFunction7Button.EnableWindow(false);

		m_Function3Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_Function3Button, _T("Source Settings"));

		/*
		if (vTestingLamp & 1)
			SetDlgItemText(IDC_Function3Button,"Stop Red Light Test!");
		else
			SetDlgItemText(IDC_Function3Button,"Test Red Light!");

		m_Function4Button.ShowWindow(SW_SHOW);
		m_ArrowDown4.ShowWindow(SW_SHOW);
		if (vTestingLamp & 2)
			SetDlgItemText(IDC_Function4Button,"Stop Yellow Light Test!");
		else
			SetDlgItemText(IDC_Function4Button,"Test Yellow Light!");

		m_Function5Button.ShowWindow(SW_SHOW);
		m_ArrowDown5.ShowWindow(SW_SHOW);
		if (vTestingLamp)
			SetDlgItemText(IDC_Function5Button,"Stop Blue Light Test!");
		else
			SetDlgItemText(IDC_Function5Button,"Test Blue Light!");
			*/

	}
	else
	{
		m_VoltageSetting2.ShowWindow(SW_HIDE);
		m_CurrentSetting2.ShowWindow(SW_HIDE);
		m_SubFunction6Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
		m_SubFunction7Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
		//m_SubFunction3Button.ShowWindow(SW_HIDE);
		//m_ArrowLeft3.ShowWindow(SW_HIDE);
		m_Function3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
		//m_Function5Button.ShowWindow(SW_HIDE);
		//m_ArrowDown5.ShowWindow(SW_HIDE);
		m_Function4Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());
	}
	if (((vLocalSystemData->vSystemRunMode == cStoppedSystemMode) ||
		(vLocalSystemData->vSystemRunMode == cMeasureSourceStrengthMode)) &&
		(!(vTestingLamp)))
		vSubFunction2ButtonEnable = true;	//m_SubFunction2Button.EnableWindow(true);
	else
		vSubFunction2ButtonEnable = false;	//m_SubFunction2Button.EnableWindow(false);

	if (!(vTestingLamp))
	{
		if (vGlobalCurrentProduct)
			vSubFunction6ButtonEnable = true;	//m_SubFunction6Button.EnableWindow(true);
		else
			vSubFunction6ButtonEnable = false;	//m_SubFunction6Button.EnableWindow(false);
		if (vGlobalCurrentProduct)
			vSubFunction7ButtonEnable = true;	//m_SubFunction7Button.EnableWindow(true);
		else
			vSubFunction7ButtonEnable = false;	//m_SubFunction7Button.EnableWindow(false);
	}
	else
	{
		vSubFunction6ButtonEnable = false;	//m_SubFunction6Button.EnableWindow(false);
		vSubFunction7ButtonEnable = false;	//m_SubFunction7Button.EnableWindow(false);
	}
}
/*
void CXRaySourceStatusDialog::ShowCurrentHistogram()
{
	CHistogramDisplayDialog IHistogramDisplayDialog;
	IHistogramDisplayDialog.vNumberOfNumbers = cNumberOfVoltageSamples;
	IHistogramDisplayDialog.vDataTitle = "X-Ray Current Readings";
	IHistogramDisplayDialog.vValuesAreBytes = true;
	IHistogramDisplayDialog.vPointerToBytes = vLocalSystemData->vSourceCurrentReading;
	IHistogramDisplayDialog.vLocalSystemData = vLocalSystemData;
	IHistogramDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
	IHistogramDisplayDialog.vMainWindowPointer = vMainWindowPointer;
	int nResponse = IHistogramDisplayDialog.DoModal();
}

void CXRaySourceStatusDialog::ShowVoltageHistogram()
{
	CHistogramDisplayDialog IHistogramDisplayDialog;
	IHistogramDisplayDialog.vNumberOfNumbers = cNumberOfVoltageSamples;
	IHistogramDisplayDialog.vDataTitle = "X-Ray Voltage Readings";
	IHistogramDisplayDialog.vValuesAreBytes = true;
	IHistogramDisplayDialog.vPointerToBytes = vLocalSystemData->vSourceVoltageReading;
	IHistogramDisplayDialog.vLocalSystemData = vLocalSystemData;
	IHistogramDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
	IHistogramDisplayDialog.vMainWindowPointer = vMainWindowPointer;
	int nResponse = IHistogramDisplayDialog.DoModal();
}
*/
void CXRaySourceStatusDialog::ShowHVPSStatus()
{
	CString TempText = "";
	if (vGlobalDXMXRayPort)
	{
		TempText = "Model Number: " + vGlobalDXMXRayPort->vModelNumber;
		TempText = TempText + "\nInterlocks: " + WordToHex(vLocalSystemData->vSourceInterLocks);

		TempText = TempText + "  HVPS Interlock: ";
		if (vGlobalDXMXRayPort->vInterLockStatus)
			TempText = TempText + "Closed";
		else
			TempText = TempText + "Open";

		TempText = TempText + "\nSoftware Version: " + vGlobalDXMXRayPort->vSoftwareVersion;
		TempText = TempText + "\nHardware Version: " + vGlobalDXMXRayPort->vHardwareVersion;
		TempText = TempText + "\nWebserver Version: " + vGlobalDXMXRayPort->vWebserverVersion;
		if ((vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac))
			TempText = TempText + "\nTemperature `C: " + dtoa(vGlobalDXMXRayPort->vHVPSTemperatureMonitor * .0732, 1);
			// + "   Multiplier: " + dtoa(vGlobalDXMXRayPort->vMultplierTemperatureMonitor * .0732, 1);

		if ((vLocalConfigurationData->vHVPSType == c100WMonoBlockHVPS) || (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock))
			TempText = TempText + "\nVoltage Scale: " + dtoa(vGlobalDXMXRayPort->vVoltageMaximumScale, 2) + ",     Current Scale: " + dtoa(vGlobalDXMXRayPort->vCurrentMaximumScale, 4);
	}
	else
		TempText = TempText + "\nNo DXM Port Initialized";

	CNoticeDialog TempNoticeDialog;
	TempNoticeDialog.vNoticeText = TempText;
	TempNoticeDialog.vType = cInformationMessage;
	TempNoticeDialog.DoModal();

}

void CXRaySourceStatusDialog::OnDialogTitleStaticText() 
{
	// Display DXM information
	ShowHVPSStatus();
}
/*
void CXRaySourceStatusDialog::UserSetFilamentLimit()
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	if (vGlobalCurrentProduct)
	{
		CNumericEntryDialog INumericEntryDialog;  
		
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vLocalSystemData->vDXMFilamentLimit * 4.6,2);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Temporary Filament Limit";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Limit Set to 4.6 when start ScanTrac";
		INumericEntryDialog.m_UnitsString = "Measured in Amps";
		INumericEntryDialog.m_DialogTitleStaticText4 = "";
		INumericEntryDialog.m_DialogTitleStaticText3 = "4.6 Amps Maximum";

		INumericEntryDialog.vMaxValue = 4.6;
		INumericEntryDialog.vIntegerOnly = false;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vLocalSystemData->vDXMFilamentLimit = ATOF(INumericEntryDialog.vEditString) / 4.6;
			if (vGlobalDXMXRayPort)
				vGlobalDXMXRayPort->SetXRayFilamentLimit(vLocalSystemData->vDXMFilamentLimit);// 1 = full scale, or 4.6Amp limit
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nSent DXM Filament Limit: " +
				dtoa(vLocalSystemData->vDXMFilamentLimit * 4.6, 2);
			ReportErrorMessage("Notice-Set Filament Limit: " +
				dtoa(vLocalSystemData->vDXMFilamentLimit * 4.6, 2),cEMailInspx,32000);
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.vAutoClose = 2000;
			TempNoticeDialog.DoModal();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			PrepareToExit();
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nYou must have a current product Selected";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
}
*/
void CXRaySourceStatusDialog::OnChangedFilterButton()
{
	AskIfFilterChanged();
}

bool CXRaySourceStatusDialog::AskIfFilterChanged() 
{
	CSelectItemDialog ISelectItemDialog;  
	
	CProduct *TempProduct = vMainWindowPointer->GetProductByName("~SourceStrength");
	bool TempSourceStrengthIsCurrentProduct = false;
	if (TempProduct == vGlobalCurrentProduct)
		TempSourceStrengthIsCurrentProduct = true;

	if (TempProduct)
	{
		bool TempSourceStrengthIsStandard = true;

		if (dtoa(TempProduct->vXRaySourceVoltage * 6.0, 1) != dtoa(vMainWindowPointer->GetDefaultMeasureSourceStrengthVoltage(), 1))
			TempSourceStrengthIsStandard = false;

		if (dtoa(TempProduct->vXRaySourceCurrent, 1) > dtoa(vMainWindowPointer->GetDefaultMeasureSourceStrengthCurrent(vLocalConfigurationData->vXRaySourceFilter), 1))
			TempSourceStrengthIsStandard = false;

		if (!((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac) || (vLocalConfigurationData->vScanTracType == cUpSideDownSoloScanTrac) || 
			(vLocalConfigurationData->vScanTracType == cFermataScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac)))
		{
			CYesNoDialog TempYesNoDialog;
			TempYesNoDialog.vNoticeText = _T("\nCurrent and/or Voltage are non standard\nsettings for Source Strength Measurements.\nUse Default Source Strength?");
			TempYesNoDialog.vYesButtonText = "Use Default";
			TempYesNoDialog.vNoButtonText = "Keep as is";
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
			{
				TempProduct->vXRaySourceVoltage = (float)(vMainWindowPointer->GetDefaultMeasureSourceStrengthVoltage() / 6.0);
				TempProduct->vXRaySourceCurrent = (float)vMainWindowPointer->GetDefaultMeasureSourceStrengthCurrent(vLocalConfigurationData->vXRaySourceFilter);

				if (vLocalConfigurationData->vMaximumCurrent < TempProduct->vXRaySourceCurrent)
					TempProduct->vXRaySourceCurrent = (float)vLocalConfigurationData->vMaximumCurrent;

				if (vLocalConfigurationData->vMaximumVoltage < TempProduct->vXRaySourceVoltage)
					TempProduct->vXRaySourceVoltage = (float)vLocalConfigurationData->vMaximumVoltage;

				TempProduct->vXRayIntegrationTime = 750; //.3 mSec
				if (TempSourceStrengthIsCurrentProduct)
				{
					vLocalSystemData->vRampXRayCurrentAmount = vGlobalCurrentProduct->vXRaySourceCurrent;
					vLocalSystemData->vRampXRayVoltageAmount = vGlobalCurrentProduct->vXRaySourceVoltage;
					vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount,vLocalSystemData->vRampXRayCurrentAmount);
					vMainWindowPointer->SendProductIntegrationTime(vGlobalCurrentProduct->vXRayIntegrationTime);
				}
				UpdateButtons();
			}
			return true;
		}
		else
		if ((TempSourceStrengthIsStandard) && (vLocalConfigurationData->vXRaySourceFilter))
		{
			CYesNoDialog TempYesNoDialog;
			TempYesNoDialog.vNoticeText = _T("\nAluminum X-Ray Filter was installed,\nhas it been removed?");
			TempYesNoDialog.vYesButtonText = "Filter Was Removed";
			TempYesNoDialog.vNoButtonText = "Filter Still Installed";
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
			{
				vLocalConfigurationData->vXRaySourceFilter = false;
				TempProduct->vXRaySourceVoltage = (float)(vMainWindowPointer->GetDefaultMeasureSourceStrengthVoltage() / 6.0);
				TempProduct->vXRaySourceCurrent = (float)vMainWindowPointer->GetDefaultMeasureSourceStrengthCurrent(vLocalConfigurationData->vXRaySourceFilter);

				if (vLocalConfigurationData->vMaximumCurrent < TempProduct->vXRaySourceCurrent)
					TempProduct->vXRaySourceCurrent = (float)vLocalConfigurationData->vMaximumCurrent;

				if (vLocalConfigurationData->vMaximumVoltage < TempProduct->vXRaySourceVoltage)
					TempProduct->vXRaySourceVoltage = (float)vLocalConfigurationData->vMaximumVoltage;

				TempProduct->vXRayIntegrationTime = 750;  //.3 mSec
				if (TempSourceStrengthIsCurrentProduct)
				{
					vLocalSystemData->vRampXRayCurrentAmount = vGlobalCurrentProduct->vXRaySourceCurrent;
					vLocalSystemData->vRampXRayVoltageAmount = vGlobalCurrentProduct->vXRaySourceVoltage;
					vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount,vLocalSystemData->vRampXRayCurrentAmount);
					vMainWindowPointer->SendProductIntegrationTime(vGlobalCurrentProduct->vXRayIntegrationTime);
				}
			}
			return true;
		}
		else
		if ((TempSourceStrengthIsStandard) && (!vLocalConfigurationData->vXRaySourceFilter))
		{
			CYesNoDialog TempYesNoDialog;
			TempYesNoDialog.vNoticeText = _T("\nAluminum X-Ray Filter was not installed,\nhas it been installed?");
			TempYesNoDialog.vYesButtonText = "Filter Was Installed";
			TempYesNoDialog.vNoButtonText = "Filter not Installed";
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
			{
				vLocalConfigurationData->vXRaySourceFilter = true;
				TempProduct->vXRaySourceVoltage = (float)(vMainWindowPointer->GetDefaultMeasureSourceStrengthVoltage() / 6.0);

				if (vLocalConfigurationData->vMaximumCurrent < TempProduct->vXRaySourceCurrent)
					TempProduct->vXRaySourceCurrent = (float)vLocalConfigurationData->vMaximumCurrent;

				if (vLocalConfigurationData->vMaximumVoltage < TempProduct->vXRaySourceVoltage)
					TempProduct->vXRaySourceVoltage = (float)vLocalConfigurationData->vMaximumVoltage;

				TempProduct->vXRaySourceCurrent = (float)vMainWindowPointer->GetDefaultMeasureSourceStrengthCurrent(vLocalConfigurationData->vXRaySourceFilter);
				TempProduct->vXRayIntegrationTime = 750;  //.3 mSec
				if (TempSourceStrengthIsCurrentProduct)
				{
					vLocalSystemData->vRampXRayCurrentAmount = vGlobalCurrentProduct->vXRaySourceCurrent;
					vLocalSystemData->vRampXRayVoltageAmount = vGlobalCurrentProduct->vXRaySourceVoltage;
					vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount,vLocalSystemData->vRampXRayCurrentAmount);
					vMainWindowPointer->SendProductIntegrationTime(vGlobalCurrentProduct->vXRayIntegrationTime);
				}
			}
			return true;
		}
		else
		{
			//Set dialog box data titles and number value
			ISelectItemDialog.vTitleString = "Source Strength Settings Non-Standard";
			//ISelectItemDialog.vBackgroundColor = cGreen;

			ISelectItemDialog.vItems[0] = "Keep Existing Settings";
			if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac) || (vLocalConfigurationData->vScanTracType == cUpSideDownSoloScanTrac))
			{
				ISelectItemDialog.vNumberOfItems = 3;
				ISelectItemDialog.vItems[1] = "Use Standard Settings for Aluminum Filter";
				ISelectItemDialog.vItems[2] = "Use Standard Settings for No Aluminum Filter";
			}
			else
			{
				ISelectItemDialog.vNumberOfItems = 2;
				ISelectItemDialog.vItems[1] = "Use Standard Settings";
			}
			//ISelectItemDialog.m_DialogTitleStaticText2 = "";
			//ISelectItemDialog.m_DialogTitleStaticText3 = "";
			if ((dtoa(TempProduct->vXRaySourceVoltage * 6, 1) == dtoa(vMainWindowPointer->GetDefaultMeasureSourceStrengthVoltage(), 1)) && 
				(dtoa(TempProduct->vXRaySourceCurrent, 1) == dtoa(vMainWindowPointer->GetDefaultMeasureSourceStrengthCurrent(true), 1)))
			{
				ISelectItemDialog.vItemEnabled[1] = false;
				ISelectItemDialog.m_DialogTitleStaticText2 = "Disabled Entry is current Setting";
			}
			if ((dtoa(TempProduct->vXRaySourceVoltage * 6, 1) == dtoa(vMainWindowPointer->GetDefaultMeasureSourceStrengthVoltage(), 1)) && 
				(dtoa(TempProduct->vXRaySourceCurrent, 1) == dtoa(vMainWindowPointer->GetDefaultMeasureSourceStrengthCurrent(false), 1)))
			{
				ISelectItemDialog.vItemEnabled[2] = false;
				ISelectItemDialog.m_DialogTitleStaticText2 = "Disabled Entry is current Setting";
			}
			ISelectItemDialog.m_DialogTitleStaticText3 = "Current Values: Voltage: " + dtoa(TempProduct->vXRaySourceVoltage * 6,1) + " Current: " + dtoa(TempProduct->vXRaySourceCurrent,2);

			//Pass control to dialog box and display
			int nResponse = ISelectItemDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				if ((ISelectItemDialog.vItemNumberSelected == 2) || (ISelectItemDialog.vItemNumberSelected == 3))
				{
					if (ISelectItemDialog.vItemNumberSelected == 2)
						vLocalConfigurationData->vXRaySourceFilter = true;
					else
						vLocalConfigurationData->vXRaySourceFilter = false;

					TempProduct->vXRaySourceVoltage = (float)(vMainWindowPointer->GetDefaultMeasureSourceStrengthVoltage() / 6.0);
					TempProduct->vXRaySourceCurrent = (float)vMainWindowPointer->GetDefaultMeasureSourceStrengthCurrent(vLocalConfigurationData->vXRaySourceFilter);

					if (vLocalConfigurationData->vMaximumCurrent < TempProduct->vXRaySourceCurrent)
						TempProduct->vXRaySourceCurrent = (float)vLocalConfigurationData->vMaximumCurrent;

					if (vLocalConfigurationData->vMaximumVoltage < TempProduct->vXRaySourceVoltage)
						TempProduct->vXRaySourceVoltage = (float)vLocalConfigurationData->vMaximumVoltage;

					TempProduct->vXRayIntegrationTime = 750; //.3 mSec
					if (TempSourceStrengthIsCurrentProduct)
					{
						vLocalSystemData->vRampXRayCurrentAmount = vGlobalCurrentProduct->vXRaySourceCurrent;
						vLocalSystemData->vRampXRayVoltageAmount = vGlobalCurrentProduct->vXRaySourceVoltage;
						vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount,vLocalSystemData->vRampXRayCurrentAmount);
						vMainWindowPointer->SendProductIntegrationTime(vGlobalCurrentProduct->vXRayIntegrationTime);
					}
				}
				return true;
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
				return false;
			}
			else
				return false;
		}
	}
	else
		return false;
}

void CXRaySourceStatusDialog::StopDarkCurrentMeasurement(bool TempAbort)
{
	KillTimer(vDarkCurrentTimerHandle);
	vTakingDarkCurrentMeasurment = false;
	//vLocalSystemData->vSystemRunMode = cStoppedSystemMode;
	vMainWindowPointer->StopRunning(true);

	//vMainWindowPointer->SendXRayPowerOnOffToUController(0);
	//::PostThreadMessage(vGlobalCallingThreadID,cXRaysOnMessage,0,0);
	//vGlobaluCSerialPort->TurnOnOffRedLamp(false);
	if (vGlobalCurrentProduct)
	{
		vLocalSystemData->vRampXRayVoltageAmount = vGlobalCurrentProduct->vXRaySourceVoltage;
		vLocalSystemData->vRampXRayCurrentAmount = vGlobalCurrentProduct->vXRaySourceCurrent;
		if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
		{
			vLocalSystemData->vSourceRampedUp = false;
			vMainWindowPointer->SendSourceSettingsToUController(
				vLocalSystemData->vRampXRayVoltageAmount,vLocalSystemData->vRampXRayCurrentAmount);
			int TempTimerResult = SetTimer(vResetMinMaxAfterStablizeTimerHandle,7000,NULL);
			if (!TempTimerResult)
				ReportErrorMessage("Error-Reset Min Max Timer Failed",cEMailInspx,32000);
		}
	}
	vLocalSystemData->vTemporarilyChangeSource = false;
	SetMinimumAndMaximum();
	ChangedMinimumAndMaximum();

	CNoticeDialog TempNoticeDialog;
	TempNoticeDialog.vType = cNoticeMessage;
	CString TempString = "\n\n\nDark Current Measurement Failed or Aborted";
	if (!TempAbort)
	{
		TempString = "\n\nDark Current = " + dtoa(vDarkCurrentCurrentLevel,1) + " uA\nTube SN: " + 
			vGlobalLifeTimeCounters->vXRayTubeSerialNumber;

		if (vDarkCurrentCurrentLevel > 15)
		{
			TempString = TempString + "\n Dark Current > 15 uA, Notify Alex!";
			TempNoticeDialog.vType = cWarningMessage;
		}

		ReportErrorMessage("Dark Current = " + dtoa(vDarkCurrentCurrentLevel,1) + " uA, Tube SN: " + 
			vGlobalLifeTimeCounters->vXRayTubeSerialNumber, cEMailInspx, 32003);
	}

	TempNoticeDialog.vNoticeText = TempString;
	TempNoticeDialog.DoModal();

	SetDlgItemText(IDC_DialogTitleStaticText1, _T("X-Ray Diagnostics"));
	SetDlgItemText(IDC_Function4Button, _T("Measure DarkCurrent"));
}

void CXRaySourceStatusDialog::OnShutterTestButton() 
{
	if (vCyclingShutter)
	{
		KillTimer(vCycleShutterTimerHandle);
		if (vGlobaluCSerialPort)
		{
			vGlobaluCSerialPort->SetRadiationDoorManualControl(cRadiationShutterAutoControl);
			vLocalSystemData->vRadiationDoorManualControlRegister = cRadiationShutterAutoControl;
		}
		vCyclingShutter = false;
		UpdateButtons();
	}
	else
	if (PasswordOK(cTemporaryInspxPassword,false))
	if (vGlobaluCSerialPort)
	if (vLocalConfigurationData->vRadiationShutterInSystem)
	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	{
		vCyclingShutter = true;
		int TempTimerResult = SetTimer(vCycleShutterTimerHandle,250,NULL);
		if (!TempTimerResult)
			ReportErrorMessage("Error-Cycle Shutter Timer Failed",cEMailInspx,32000);
		UpdateButtons();
	}
}

void CXRaySourceStatusDialog::OnStatusLine()
{
	vMainWindowPointer->ClearYellowWarningButton();
}

void CXRaySourceStatusDialog::DisplayCPUTemperatures(void)
{

	if (vLocalSystemData->vCPUTemperaturesReadOK == true)		// called in ScanTrac Dialog, 1 second loop, GetCoreTempInfoAlternative();
	{
		if (vOldCPUTemperature != vLocalSystemData->vCPUTemperature)
		if ((!vShowingYellowMessage)	|| ((vLocalConfigurationData->vHVPSType == c210WMonoBlockHVPS) || (vLocalConfigurationData->vHVPSType == cMNXHVPS))) //MNX and 210W MonoBlock dont have coolant or MonoBlock Temp
		{
			vOldCPUTemperature = vLocalSystemData->vCPUTemperature;
			if (vOldCPUTemperature < 0xFFF)
			{
				SetDlgItemText(IDC_TemperatureCPU,GetCoreTemperatureString(vOldCPUTemperature));
			}
			else	// still averaging  temp
			{
				SetDlgItemText(IDC_TemperatureCPU, _T("--"));
			}
		}

		if (vOldCPUTemperatureMaximum != vLocalSystemData->vCPUTemperatureMaximum)
		if ((!vShowingYellowMessage)	|| ((vLocalConfigurationData->vHVPSType == c210WMonoBlockHVPS) || (vLocalConfigurationData->vHVPSType == cMNXHVPS))) //MNX and 210W MonoBlock dont have coolant or MonoBlock Temp
		{
			vOldCPUTemperatureMaximum = vLocalSystemData->vCPUTemperatureMaximum;
			if ((vOldCPUTemperatureMaximum < 0xFFE) && (vOldCPUTemperatureMaximum > 1))
			{
				SetDlgItemText(IDC_TemperatureCPUMaximum,GetCoreTemperatureString(vOldCPUTemperatureMaximum));
			}
			else	// still averaging Max temp
			{
				SetDlgItemText(IDC_TemperatureCPUMaximum, _T("--"));
			}
		}

		if (vOldCPUTemperatureMinimum != vLocalSystemData->vCPUTemperatureMinimum)
		if ((!vShowingYellowMessage)	|| ((vLocalConfigurationData->vHVPSType == c210WMonoBlockHVPS) || (vLocalConfigurationData->vHVPSType == cMNXHVPS))) //MNX and 210W MonoBlock dont have coolant or MonoBlock Temp
		{
			vOldCPUTemperatureMinimum = vLocalSystemData->vCPUTemperatureMinimum;
			if (vOldCPUTemperatureMinimum < 0xFFF) 
			{
				SetDlgItemText(IDC_TemperatureCPUMinimum,GetCoreTemperatureString(vOldCPUTemperatureMinimum));
			}
			else	// still averaging Min temp
			{
				SetDlgItemText(IDC_TemperatureCPUMinimum, _T("--"));
			}
		}
	}
}



void CXRaySourceStatusDialog::OnTemperatureairoutsidelabel()
{
	// Plot Air Conditioner Temperature Data
	if (vLocalConfigurationData->vDeltaTemperatureForFilterCloggedError == 0) //have have AirConditioning
	{
		double TempData[cNumberOfAirConditionerTemperatureSamples];
		for (WORD TempLoop = 0; TempLoop < cNumberOfAirConditionerTemperatureSamples; TempLoop++)
			TempData[TempLoop] = 0;

		WORD TempSourceIndex = vLocalSystemData->vAirConditionerTemperatureSampleIndex - 1;
		if (vLocalSystemData->vAirConditionerTemperatureSampleIndex == 0)
			TempSourceIndex = cNumberOfAirConditionerTemperatureSamples - 1;
		for (WORD TempLoop = 0; TempLoop < vLocalSystemData->vAirConditionerTemperatureSampleCount; TempLoop++)
		{
			//flip data so most current is on right, oldest on left
			TempData[vLocalSystemData->vAirConditionerTemperatureSampleCount - TempLoop - 1] = vLocalSystemData->vAirConditionerTemperatureSampleData[TempSourceIndex];
			if (TempSourceIndex > 0)
				TempSourceIndex--;
			else
				TempSourceIndex = cNumberOfAirConditionerTemperatureSamples - 1;
		}

		CHistogramDisplayDialog IHistogramDisplayDialog;
		IHistogramDisplayDialog.vNumberOfNumbers = vLocalSystemData->vAirConditionerTemperatureSampleCount;
		IHistogramDisplayDialog.vTopDataLine = "Air Conditioner Temp";
		IHistogramDisplayDialog.vDataTitle = "Temperature at output for last " + dtoa(vLocalSystemData->vAirConditionerTemperatureSampleCount / 6.0, 1) + " minutes (every 10 seconds)";
		IHistogramDisplayDialog.vValuesAreFloats = true;
		IHistogramDisplayDialog.vXScaleIsTime = true;
		IHistogramDisplayDialog.vPointerToFloats = &TempData[0];
		IHistogramDisplayDialog.vLocalSystemData = vLocalSystemData;
		IHistogramDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
		IHistogramDisplayDialog.vMainWindowPointer = vMainWindowPointer;
		IHistogramDisplayDialog.vPlotRawData = true;

		int nResponse = IHistogramDisplayDialog.DoModal();
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
}

void CXRaySourceStatusDialog::ShowNoMouseClickDialogMessage() 
{
	CNoticeDialog TempNoticeDialog;
	CString TempText("");
	TempText = ("Mouse Click Not Available");
	TempNoticeDialog.vNoticeText = "\n\n\nMouse Click Not Available";
	TempNoticeDialog.vType = cInformationMessage;
	TempNoticeDialog.DoModal();
}

void CXRaySourceStatusDialog::OnDialogtitlestatictext1()
{
	if (vGlobalShiftKeyDown)
		ShowHVPSStatus();
	else
	{
		BYTE TempHiddenKeys = 8;
		if (!PasswordOK(cTemporaryInspxPassword,false))
			TempHiddenKeys = 10;
		PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
	}
}

void CXRaySourceStatusDialog::OnStnClickedOnoff()
{
	//show guard Master status
	if ((vGlobalGuardMaster) && (vGlobalGuardMaster->vDetectedValidGuardMasterDevice))
	{
		CString TempText = "";
		
		if ((vGlobalGuardMaster->vOldGuardMasterRawInterlockData & cInterlockOutputBits) != cInterlockOutputBits)
			TempText = TempText + "\n*Output Status: Open, can not generate X-Rays";
		else
			TempText = TempText + "\nOutput Status: Closed, can generate X-Rays";

		TempText = TempText + "\n\n\nStatus of Interlock Inputs: ";
		CString TempOpenInterlockText = "";
		if ((vGlobalGuardMaster->vOldGuardMasterRawInterlockData & cFrontDoorMaskBits) != cFrontDoorMaskBits)
		{
			if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
				(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
				TempOpenInterlockText = TempOpenInterlockText + "\n     Cabinet Door Open";
			else
				TempOpenInterlockText = TempOpenInterlockText + "\n     Front Door Open";
		}

		if ((vGlobalGuardMaster->vOldGuardMasterRawInterlockData & cBackDoorMaskBits) != cBackDoorMaskBits)
		{
			if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
				(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
				TempOpenInterlockText = TempOpenInterlockText + "\n     Center Conveyor Door Open";
			else
				TempOpenInterlockText = TempOpenInterlockText + "\n     Back Door Open";
		}

		if ((vGlobalGuardMaster->vOldGuardMasterRawInterlockData & cEStopMaskBits) != cEStopMaskBits)
			TempOpenInterlockText = TempOpenInterlockText + "\n     E-Stop In (X-Rays Disabled)";

		if ((vGlobalGuardMaster->vOldGuardMasterRawInterlockData & cKeySwitchMaskBits) != cKeySwitchMaskBits)
			TempOpenInterlockText = TempOpenInterlockText + "\n     Key Switch Off (X-Rays Disabled)";

		//if ((vGlobalGuardMaster->vOldGuardMasterRawInterlockData & cDetectorXRayShieldsMaskBits) != cDetectorXRayShieldsMaskBits)
		//	TempOpenInterlockText = TempOpenInterlockText + "\n     Detector X-Ray Shield Removed";

		if ((vGlobalGuardMaster->vOldGuardMasterRawInterlockData & cTubeCoverMaskBits) != cTubeCoverMaskBits)
		{
			if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
				(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
				TempOpenInterlockText = TempOpenInterlockText + "\n     Detector Door Open";
			else
				TempOpenInterlockText = TempOpenInterlockText + "\n     Tube Cover Removed";
		}

		if ((vGlobalGuardMaster->vOldGuardMasterRawInterlockData & cLeftConveyorCoversMaskBits) != cLeftConveyorCoversMaskBits)
			TempOpenInterlockText = TempOpenInterlockText + "\n     Left Conveyor Door Open";

		if ((vGlobalGuardMaster->vOldGuardMasterRawInterlockData & cRightConveyorCoversMaskBits) != cRightConveyorCoversMaskBits)
			TempOpenInterlockText = TempOpenInterlockText + "\n     Right Conveyor Door Open";

		if (TempOpenInterlockText.GetLength() < 5)
			TempOpenInterlockText = TempOpenInterlockText + "All enabled (can run X-Rays)";

		TempText = TempText + TempOpenInterlockText;

		if (vGlobalGuardMaster->vGuardMasterFaultLEDData & 3)
		{
			if (vGlobalGuardMaster->vGuardMasterFaultLEDData & 21)
				TempText = TempText + "\n\n\n*Fault LED on Guard Master is ON, Major Fault";
			if (vGlobalGuardMaster->vGuardMasterFaultLEDData & 12)
				TempText = TempText + "\n\n\n*Fault LED on Guard Master is Blinking, Minor Fault";
		}
		else
			TempText = TempText + "\n\n\nFault LED on Guard Master is OFF";

		if (vGlobalGuardMaster->vGuardMasterHardwareFaultsData == 0)
			TempText = TempText + "\n\n\nNo Hardware Faults";
		else
		{
			TempText = TempText + "\n\n\n*Hardware Faults:\n";

			if (vGlobalGuardMaster->vGuardMasterHardwareFaultsData & 1)
				TempText = TempText +  "\n     Processor HW fault";
					
			if (vGlobalGuardMaster->vGuardMasterHardwareFaultsData & 2)
				TempText = TempText +  "\n     Safety Input HW fault";
					
			if (vGlobalGuardMaster->vGuardMasterHardwareFaultsData & 4)
				TempText = TempText +  "\n     Safety Output HW fault";
					
			if (vGlobalGuardMaster->vGuardMasterHardwareFaultsData & 8)
				TempText = TempText +  "\n     Power supply fault / Main transistor fault";
					
			if (vGlobalGuardMaster->vGuardMasterHardwareFaultsData & 0x10)
				TempText = TempText +  "\n     Communication fault";
					
			if (vGlobalGuardMaster->vGuardMasterHardwareFaultsData & 0x20)
				TempText = TempText +  "\n     Configuration fault (wrong revision, invalid configuration";
					
			if (vGlobalGuardMaster->vGuardMasterHardwareFaultsData & 0x40)
				TempText = TempText +  "\n     Time out (Clock monitoring)";
					
			if (vGlobalGuardMaster->vGuardMasterHardwareFaultsData & 0x80)
				TempText = TempText +  "\n     Plug-in fault";
		}

		if (PasswordOK(cSuperInspxPassword,false))
		if ((vGlobalGuardMaster->vGuardMasterFaultsData[0]) || (vGlobalGuardMaster->vGuardMasterFaultsData[1]) || (vGlobalGuardMaster->vGuardMasterFaultsData[2]) || 
			(vGlobalGuardMaster->vGuardMasterFaultsData[3]) || (vGlobalGuardMaster->vGuardMasterFaultsData[4]))
		{

			TempText = TempText + "\n\n\nGuard Master Interlock System has the following faults: \n";

			CString TempFaultsText = "";
			if (vGlobalGuardMaster->vGuardMasterFaultsData[0])
			{
				TempFaultsText = TempFaultsText + "\n    *Guard Master SMF0 Fault Code: " + WordToHex((WORD)(vGlobalGuardMaster->vGuardMasterFaultsData[0] >> 16)) + " "  + WordToHex((WORD)vGlobalGuardMaster->vGuardMasterFaultsData[0]);

				if (vGlobalGuardMaster->vGuardMasterFaultsData[0] & cFrontDoorFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in Front Door Interlock";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[0] & cBackDoorFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in Back Door Interlock";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[0] & cEStopFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in E-Stop";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[0] & cKeySwitchFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in Key Switch";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[0] & cDetectorXRayShieldsFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in X-Ray Detector Shield Interlock";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[0] & cTubeCoverFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in X-Ray Tube Cover Interlock";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[0] & cLeftConveyorCoversFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in Left Conveyor Cover Interlock";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[0] & cRightConveyorCoversFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in Right Conveyor Cover Interlock";
			}

			if (vGlobalGuardMaster->vGuardMasterFaultsData[1])
			{
				TempFaultsText = TempFaultsText + "\n    *Guard Master SMF1 Fault Code: " + WordToHex((WORD)(vGlobalGuardMaster->vGuardMasterFaultsData[1] >> 16)) + " "  + WordToHex((WORD)vGlobalGuardMaster->vGuardMasterFaultsData[1]);

				if (vGlobalGuardMaster->vGuardMasterFaultsData[1] & cFrontDoorFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in Front Door Interlock";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[1] & cBackDoorFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in Back Door Interlock";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[1] & cEStopFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in E-Stop";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[1] & cKeySwitchFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in Key Switch";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[1] & cDetectorXRayShieldsFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in X-Ray Detector Shield Interlock";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[1] & cTubeCoverFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in X-Ray Tube Cover Interlock";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[1] & cLeftConveyorCoversFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in Left Conveyor Cover Interlock";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[1] & cRightConveyorCoversFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in Right Conveyor Cover Interlock";
			}

			if (vGlobalGuardMaster->vGuardMasterFaultsData[2])
			{
				TempFaultsText = TempFaultsText + "\n    *Guard Master SMF2 Fault Code: " + WordToHex((WORD)(vGlobalGuardMaster->vGuardMasterFaultsData[2] >> 16)) + " "  + WordToHex((WORD)vGlobalGuardMaster->vGuardMasterFaultsData[2]);

				if (vGlobalGuardMaster->vGuardMasterFaultsData[2] & cFrontDoorFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in Front Door Interlock";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[2] & cBackDoorFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in Back Door Interlock";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[2] & cEStopFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in E-Stop";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[2] & cKeySwitchFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in Key Switch";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[2] & cDetectorXRayShieldsFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in X-Ray Detector Shield Interlock";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[2] & cTubeCoverFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in X-Ray Tube Cover Interlock";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[2] & cLeftConveyorCoversFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in Left Conveyor Cover Interlock";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[2] & cRightConveyorCoversFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in Right Conveyor Cover Interlock";
			}

			if (vGlobalGuardMaster->vGuardMasterFaultsData[3])
			{
				TempFaultsText = TempFaultsText + "\n    *Guard Master SMF3 Fault Code: " + WordToHex((WORD)(vGlobalGuardMaster->vGuardMasterFaultsData[3] >> 16)) + " "  + WordToHex((WORD)vGlobalGuardMaster->vGuardMasterFaultsData[3]);

				if (vGlobalGuardMaster->vGuardMasterFaultsData[3] & cFrontDoorFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in Front Door Interlock";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[3] & cBackDoorFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in Back Door Interlock";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[3] & cEStopFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in E-Stop";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[3] & cKeySwitchFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in Key Switch";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[3] & cDetectorXRayShieldsFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in X-Ray Detector Shield Interlock";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[3] & cTubeCoverFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in X-Ray Tube Cover Interlock";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[3] & cLeftConveyorCoversFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in Left Conveyor Cover Interlock";
				if (vGlobalGuardMaster->vGuardMasterFaultsData[3] & cRightConveyorCoversFaultMaskBit)
					TempFaultsText = TempFaultsText + "\n        Fault in Right Conveyor Cover Interlock";
			}
			if (TempFaultsText.GetLength() > 5)
				TempFaultsText = TempFaultsText + "\n\n            SMF0 only = Crossloop Error\n            SMF1 only = Simultaneity fault\n            SMF0 and SMF1 = One channel open after reset";

			if (vGlobalGuardMaster->vGuardMasterFaultsData[4] & 0x00FF)
				TempFaultsText = TempFaultsText + "\n    *Guard Master Retrigger Fault SOF Code: " + WordToHex((WORD)vGlobalGuardMaster->vGuardMasterFaultsData[4]);

			if (vGlobalGuardMaster->vGuardMasterFaultsData[4] & 0xFF00)
				TempFaultsText = TempFaultsText + "\n    *Guard Master Cross Fault of Terminals 12…17 Code: " + WordToHex((WORD)(vGlobalGuardMaster->vGuardMasterFaultsData[4] >> 16));

			TempText = TempText + TempFaultsText;
		}
		else
			TempText = TempText + "\n\n\nGuard Master Interlock System has No faults.";


		TempText = TempText + "\n\n\nGuard Master Firmware Version: " + dtoa(vGlobalGuardMaster->vGuardMasterFirmWareVersion >> 8, 0) + "." + dtoa((BYTE)vGlobalGuardMaster->vGuardMasterFirmWareVersion, 0);
			

		CHelpDialog * IHelpDialog;
		IHelpDialog = new(CHelpDialog);
		//IHelpDialog->vHelpContext = 0;
		IHelpDialog->vMainWindowPointer = vMainWindowPointer;
		IHelpDialog->vLocalSystemData = vLocalSystemData;
		IHelpDialog->vWindowType = cTextWindow;
		IHelpDialog->vLocalProductCollection = vLocalProductCollection;
		IHelpDialog->vLocalConfigurationData = vLocalConfigurationData;
		IHelpDialog->vProductPointer = vGlobalCurrentProduct;
		IHelpDialog->vTitleString = "Guard Master Interlock System Status";
		IHelpDialog->vTextString = TempText;
		int TempResult = IHelpDialog->DoModal();
		delete IHelpDialog;

	}
}
