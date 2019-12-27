//ScanTrac Side View Source File
#if !defined(AFX_XRAYSOURCESTATUSDIALOG_H__182D00D1_10B0_11D4_AC27_00500466E305__INCLUDED_)
#define AFX_XRAYSOURCESTATUSDIALOG_H__182D00D1_10B0_11D4_AC27_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XRaySourceStatusDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXRaySourceStatusDialog dialog

class CXRaySourceStatusDialog : public CDialog
{
public:
	//variables
	CString vOldSourceFault;
	bool vOldlGuardMasterNoCommunications;
	DWORD vOldDetectedValidGuardMasterDevice;
	bool vShowingYellowMessage;
	int vOldGlobalPasswordOK;
	CString vCoolantTemperatureMaximum;
	bool vOldXRayNoCommuncations;
	bool vOldYellowMessageButtonYellow;
	double vDarkCurrentCurrentLevel;
	double vDarkCurrent;
	double vDarkCurrentThreshold;
	unsigned long int vDarkCurrentTimerHandle;
	bool vTakingDarkCurrentMeasurment;

	BYTE vOldGlobalShiftKeyDown;
	CString vOldErrorMessage;
	bool vCyclingShutter;
	WORD vOldTemperatureSurfaceMaximum;
	WORD vOldTemperatureLiquidMaximum;
	WORD vOldTemperatureAirMaximum;
	WORD vOldTemperatureSurfaceMinimum;
	WORD vOldTemperatureLiquidMinimum;
	WORD vOldTemperatureAirMinimum;
	WORD vOldTemperatureAirOutsideMaximum;
	WORD vOldTemperatureAirOutsideMinimum;
	WORD vOldTemperatureuC;
	WORD vOldTemperatureuCMaximum;
	WORD vOldTemperatureuCMinimum;
	WORD vOldCPUTemperature;
	WORD vOldCPUTemperatureMinimum;
	WORD vOldCPUTemperatureMaximum;
	//WORD vOldCoolantPressureMaximum;
	//WORD vOldCoolantPressureMinimum;

	CString vOldDXMStatus;
	double vOldDXM15VStatus;
	double vOldFilamentPreHeatMonitor;
	bool vTestLampsOn;
	WORD vOldRadiationDoorManualControlRegister;
	bool vMakeExitButtonGreen;
	double vOldRampXRayVoltageAmount;
	double vOldRampXRayCurrentAmount;
	BYTE vTestingLamp;
	bool vOldMeasureSourceStrength;
	bool vChangedLamps;
	BYTE vOriginalLampStatus;
			//9/11/2009
	//BYTE vOriginalEjectorsEnabled;
	BYTE vOldLastXRayCommandOn;
	bool vOldXRayOnNotice;
	CScanTracDlg *vMainWindowPointer;
	unsigned long int vDisplayTimerHandle;
	unsigned long int vCycleShutterTimerHandle;
	unsigned long int vResetMinMaxAfterStablizeTimerHandle;
	tProductCollection *vLocalProductCollection;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

	WORD vOldSourceInterLocks;
	WORD vOldTemperatureAirOutside;
	WORD vOldTemperatureSurface;
	WORD vOldTemperatureLiquid;
	WORD vOldTemperatureAir;
	BYTE vOldGuardMasterFaultLEDData;
	//WORD vOldCoolantPressure;
	

	double vOldSourceVoltage;
	double vOldSourceCurrent;
	double vOldSourceVoltageMin;
	double vOldSourceCurrentMin;
	double vOldSourceVoltageMax;
	double vOldSourceCurrentMax;
	bool vOldNoCommunicationsWithUController;
	bool vProductPasswordOK;

	bool vFunction4ButtonEnable;
	bool vFunction5ButtonEnable;
	bool vSubFunction2ButtonEnable;
	bool vSubFunction6ButtonEnable;
	bool vSubFunction7ButtonEnable;

	// Construction
	CXRaySourceStatusDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void UpdateDisplay();
	void UpdateButtons();
	void SetMinimumAndMaximum();
	void ChangedMinimumAndMaximum();
	void PrepareToExit();
	void ShowHVPSStatus();
	//void UserSetFilamentLimit();
	void StopDarkCurrentMeasurement(bool TempAbort);
	void DisplayCPUTemperatures(void);
	void ShowNoMouseClickDialogMessage(void);


	// Dialog Data
	//{{AFX_DATA(CXRaySourceStatusDialog)
	enum { IDD = IDD_XRaySourceStatusDialog };
	CStatic	m_ShutterStatusLabel;
	CStatic	m_ShutterStatus;
	CStatic	m_MinRunningFilamentCurrent;
	CStatic	m_MaxRunningFilamentCurrent;
	CStatic	m_TemperatureLiquidMinimum;
	CStatic	m_TemperatureLiquidMaximum;
	CStatic	m_ShutterTestButton;
	CStatic	m_ChangedFilterButton;
	CStatic	m_StatusLine;
	CStatic	m_TemperatureAirOutsideMinimum;
	CStatic	m_TemperatureAirOutsideMaximum;
	CStatic	m_TemperatureAirOutsideLabel;
	CStatic	m_TemperatureAirOutside;
	CStatic	m_HVPSPreHeatLabel;
	CStatic	m_HVPSPreHeat;
	CStatic	m_HVPS15VLabel;
	CStatic	m_HVPS15V;
	CStatic	m_HVPSLabel;
	CStatic	m_HVPS;
	CStatic	m_VoltageSetting2;
	CStatic	m_CurrentSetting2;
	CDownFunctionButton	m_Function2Button;
	CDownFunctionButton	m_Function1Button;
	CStatic	m_OnOffLabel;
	CStatic	m_OnOff;
	CDownFunctionButton	m_Function5Button;
	CDownFunctionButton	m_Function3Button;
	CStatic	m_RawInterlockLabel;
	CStatic	m_RawInterlock;
	CLeftFunctionButton	m_SubFunction3Button;;
	CRightFunctionButton	m_SubFunction7Button;
	CRightFunctionButton	m_SubFunction6Button;
	CRightFunctionButton	m_SubFunction5Button;
	CLeftFunctionButton	m_SubFunction2Button;
	CStatic	m_TemperatureLiquid;
	CStatic	m_TemperatureLiquidLabel;
	CStatic	m_CoolantFlow;
	CStatic	m_CoolantFlowLabel;
	CRightFunctionButton	m_SubFunction8Button;
	CLeftFunctionButton	m_SubFunction4Button;
	CLeftFunctionButton	m_SubFunction1Button;
	CDownFunctionButton	m_Function4Button;
	CStatic	m_SourceInterlocksLabel;
	CStatic	m_SourceInterlocks;
	CStatic	m_SourceFaultLabel;
	CStatic	m_SourceFault;
	CStatic	m_XRaysOn;
	CStatic	m_Background1;
	CStatic	m_Background;
	CStatic m_TemperatureCPU;
	CStatic m_TemperatureCPULabel;
	CStatic m_TemperatureCPUMaximum;
	CStatic m_TemperatureCPUMinimum;
	CStatic m_TemperatureuC;
	CStatic m_TemperatureuCLabel;
	CStatic m_TemperatureuCMaximum;
	CStatic m_TemperatureuCMinimum;
	CStatic m_Current;
	CStatic m_CurrentLabel;
	CStatic m_CurrentSetting;
	CStatic m_DialogTitleStaticText;
	CStatic m_EStop;
	CStatic m_EStopLabel;
	CStatic m_KeySwitch;
	CStatic m_KeySwitchLabel;
	CStatic m_LimitsLabel;
	CStatic m_MaxCurrent;
	CStatic m_MaximumLabel;
	CStatic m_MaxLabel;
	CStatic m_MaxRunningCurrent;
	CStatic m_MaxRunningVoltage;
	CStatic m_MaxVLabel;
	CStatic m_MaxVoltage;
	CStatic m_MinCurrent;
	CStatic m_MinimumLabel;
	CStatic m_MinLabel;
	CStatic m_MinRunningCurrent;
	CStatic m_MinRunningVoltage;
	CStatic m_MinVLabel;
	CStatic m_MinVoltage;
	CStatic m_ReadingLabel;
	CStatic m_RunningLabel;
	CStatic m_SettingLabel;
	CStatic m_SourceCurrentLabel;
	CStatic m_SourceInterlocks2;
	CStatic m_SourceInterlocksLabel2;
	CStatic m_SourceVoltageLabel;
	CStatic m_SVoltageReading;
	CStatic m_TemperatureAir;
	CStatic m_TemperatureAirLabel;
	CStatic m_TemperatureAirMaximum;
	CStatic m_TemperatureAirMinimum;
	CStatic m_TemperatureSurface;
	CStatic m_TemperatureSurfaceLabel;
	CStatic m_TemperatureSurfaceMaximum;
	CStatic m_TemperatureSurfaceMinimum;
	CStatic m_UControllerCommunications;
	CStatic m_UControllerCommunicationsLabel;
	CStatic m_VoltageSetting;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXRaySourceStatusDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	bool AskIfFilterChanged();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXRaySourceStatusDialog)
	afx_msg void OnFunction1Button();
	afx_msg void OnFunction2Button();
	afx_msg void OnFunction3Button();
	afx_msg void OnFunction4Button();
	afx_msg void OnFunction5Button();
	afx_msg void OnSubFunction1Button();
	afx_msg void OnSubFunction2Button();
	afx_msg void OnSubFunction3Button();
	afx_msg void OnSubFunction4Button();
	afx_msg void OnSubFunction5Button();
	afx_msg void OnSubFunction6Button();
	afx_msg void OnSubFunction7Button();
	afx_msg void OnSubFunction8Button();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDialogTitleStaticText();
	afx_msg void OnChangedFilterButton();
	afx_msg void OnShutterTestButton();
	afx_msg void OnStatusLine();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTemperatureairoutsidelabel();
	afx_msg void OnDialogtitlestatictext1();
	afx_msg void OnStnClickedOnoff();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XRAYSOURCESTATUSDIALOG_H__182D00D1_10B0_11D4_AC27_00500466E305__INCLUDED_)
