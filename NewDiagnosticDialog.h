//ScanTrac Side View Source File
#if !defined(AFX_NEWDIAGNOSTICDIALOG_H__57EFD4C1_FA14_11D3_ABD6_00500466E305__INCLUDED_)
#define AFX_NEWDIAGNOSTICDIALOG_H__57EFD4C1_FA14_11D3_ABD6_00500466E305__INCLUDED_

#include "NoticeDialog.h"
#include "afxwin.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewDiagnosticDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewDiagnosticDialog dialog

class CNewDiagnosticDialog : public CDialog
{
public:
	//variables
	UINT vPercentageOfDiskSpaceFree;
	WORD vOldConveyorEncoderRate;
	WORD vOldPercentOfTimeForProcessing;
	WORD vOldPercentOfTimeBeforeEject;
	double vHardDriveSpaceFree;
	WORD vOldCPMNotRunning;
	BYTE vOldShiftKeyDown;
	WORD vOldLastBodyTriggerLength;
	double vOldBodyTriggerWidthStandardDeviation;
	double vOldBodyTriggerWidthAverage;
	bool vHadSourceEvent;
	bool vOldGlobalPasswordOK;
	bool vMakeExitButtonGreen;
	bool vAllowExtraFunctions;
	double vOldFreeTimeMinimum;
	double vOldFreeTimeAverage;
	double vOldFreeTimeStandardDeviation;
	//double vOldDriftCompensationThresholdReductionAmount;
	//double vOldDriftCompensationThresholdIncreaseAmount;
	double vOldDriftCompensationADCOffset;
	double vOldDriftCompensationDACOffset;

	BYTE vOneExtraWork;
	//unsigned long int vBackupTimerHandle;
	//CNoticeDialog *vBackupNoticeDialog;

	UINT vOldContainerRate;
	UINT vOldBeltRate;
	double vOldPercentBeltSpeed;
	WORD vOldActualEncoderRate;
	UINT vOldContainerRateMaximum;
	UINT vOldBeltRateMaximum;
	WORD vOldActualEncoderRateMaximum;
	double vOldPercentBeltSpeedMaximum;
	BYTE vOldRedLampBrokenCounter;
	BYTE vOldYellowLampBrokenCounter;
	BYTE vOldBlueLampBrokenCounter;

	bool vOlduCTempOK;
	bool vuCTempOK;
	bool vOldSourceTooHot;
	bool vSourceTooHot;
	BYTE vOlduCMainLoopCounter;
	DWORD vOldAmountOfFreeMemory;

// kjh 11/3/09 currently FPGA self test not monitored	BYTE vOldFPGASelfTest;
	UINT vOldCpuUsage;
	UINT vOld_ProcessTimePercent;
	UINT vOld_ThreadTimePercent;
	WORD vOldSourceInterLocks;
	bool vOldNoCommunicationsWithUController;
	BYTE vOlduCLEDGYR;
	BYTE vOldPosteduCVoltageProblemMessage;
	bool vProductPasswordOK;
	CNoticeDialog vVersionNoticeDialog;
	bool vOldXRayOnNotice;
	bool vChangeMade;
	UINT vPercentageOfFreeMemory;
	UINT vOldPercentageOfFreeMemory;
	unsigned long int vUpdateDisplayTimerHandle;
	//unsigned long int vTestTimerHandle;
	CScanTracDlg *vMainWindowPointer;
	tProductCollection *vLocalProductCollection;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;
	bool vFunction5ButtonEnable;
	bool vSubFunction8ButtonEnable;

// Construction
	CNewDiagnosticDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void SetChangeMade();
	void UpdateDisplay();
	void UpdateButtons();
	void ShowInspxItems();
	void PrepareToExit();

// Dialog Data
	//{{AFX_DATA(CNewDiagnosticDialog)
	enum { IDD = IDD_NewDiagnosticDialog };
	CStatic	m_InspxHDUnProtected;
	CStatic	m_FBWF_ProtectedLabel;
	CStatic	m_CpuLoadImageThreadLabel;
	CStatic	m_CpuLoadImageThread;
	CStatic	m_ProcessUsageT;
	CStatic	m_ActualCpuLoadLabel;
	CStatic	m_ActualCpuLoad;
	CStatic	m_FreeDiskSpacePercentageLabel;
	CStatic	m_FreeDiskSpacePercentage;
	CStatic	m_FreeMemoryPercentageLabel;
	CStatic	m_FreeMemoryPercentage;
	CRightFunctionButton	m_SubFunction5Button;
	CStatic	m_EncoderCountsOnLastTriggerLabel3;
	CStatic	m_EncoderCountsOnLastTrigger3;
	CRightFunctionButton	m_SubFunction8Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function3Button;
	CDownFunctionButton	m_Function1Button;
	CDownFunctionButton	m_Function5Button;
	CStatic	m_SourceCoolantFlow;
	CStatic	m_SourceCoolantFlowLabel;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_XRaysOn;
	CStatic	m_ConfirmLabel;
	CStatic	m_ExternalDetectorLabel5;
	CStatic	m_ExternalDetectorLabel4;
	CStatic	m_ExternalDetectorLabel3;
	CStatic	m_ExternalDetectorLabel2;
	CStatic	m_ExternalDetectorLabel1;
	CStatic	m_ExternalDetector5;
	CStatic	m_ExternalDetector4;
	CStatic	m_ExternalDetector3;
	CStatic	m_ExternalDetector2;
	CStatic	m_ExternalDetector1;
	CStatic	m_EjectorLabel3;
	CStatic	m_EjectorLabel2;
	CStatic	m_EjectorLabel1;
	CStatic	m_EjectorConfirm3;
	CStatic	m_EjectorConfirm2;
	CStatic	m_EjectorConfirm1;
	CStatic	m_Ejector3;
	CStatic	m_Ejector2;
	CStatic	m_Ejector1;
	CStatic	m_Background;
	CStatic	m_FBWFProtectedLabelControl;
	CString	m_FBWFProtectedLabel;
	CLeftFunctionButton m_SubFunction2Button;
	CStatic m_ActualEncoderRate;
	CStatic m_ActualEncoderRatelabel;
	CStatic m_ActualEncoderRateMaximum;
	CStatic m_BeltSpeed;
	CStatic m_BeltSpeedLabel;
	CStatic m_BeltSpeedMaximum;
	CStatic m_BlueLamp;
	CStatic m_BlueLampLabel;
	CStatic m_ContainersPerMinute;
	CStatic m_ContainersPerMinuteLabel;
	CStatic m_ContainersPerMinuteMaximum;
	CStatic m_CustomerName;
	CStatic m_DensityCompensation;
	CStatic m_DialogTitleStaticText;
	CStatic m_EncoderPercentOfMaximum;
	CStatic m_EncoderPercentOfMaximumLabel;
	CStatic m_EncoderPercentOfMaximumMaximum;
	CStatic m_Interlocks;
	CStatic m_InterlocksLabel;
	CStatic m_MaximumLabel;
	CStatic m_OnOff;
	CStatic m_OnOffLabel;
	CStatic m_RedLamp;
	CStatic m_RedLampLabel;
	CStatic m_SourceTemperature;
	CStatic m_SourceTemperatureLabel;
	CStatic m_SpeedFactor;
	CStatic m_SpeedFactor1;
	CStatic m_SpeedFactorLabel;
	CLeftFunctionButton m_SubFunction1Button;
	CLeftFunctionButton m_SubFunction3Button;
	CLeftFunctionButton m_SubFunction4Button;
	CRightFunctionButton m_SubFunction6Button;
	CRightFunctionButton m_SubFunction7Button;
	CStatic m_ThresholdIncrease;
	CStatic m_ThresholdReduction;
	CStatic m_uCStatus;
	CStatic m_uCStatusLabel;
	CStatic m_XRayStatus;
	CStatic m_XRayStatusLabel;
	CStatic m_YellowLamp;
	CStatic m_YellowLampLabel;
	CStatic m_FrontEndStatus;
	CStatic m_FrontEndStatusLabel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewDiagnosticDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewDiagnosticDialog)
	afx_msg void OnFunction1Button();
	afx_msg void OnFunction3Button();
	afx_msg void OnSubFunction1Button();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSubFunction2Button();
	afx_msg void OnSubFunction3Button();
	afx_msg void OnFunction2Button();
	afx_msg void OnFunction4Button();
	afx_msg void OnFunction5Button();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSubFunction4Button();
	afx_msg void OnSubFunction5Button();
	afx_msg void OnSubFunction8Button();
	afx_msg void OnSubFunction7Button();
	afx_msg void OnSubFunction6Button();
	afx_msg void OnDialogTitleStaticText();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWDIAGNOSTICDIALOG_H__57EFD4C1_FA14_11D3_ABD6_00500466E305__INCLUDED_)
