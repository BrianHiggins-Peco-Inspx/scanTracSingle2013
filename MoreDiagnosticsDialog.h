//ScanTrac Side View Source File
#if !defined(AFX_MOREDIAGNOSTICSDIALOG_H__AA677CB1_0B0F_11D4_ABEA_00500466E305__INCLUDED_)
#define AFX_MOREDIAGNOSTICSDIALOG_H__AA677CB1_0B0F_11D4_ABEA_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MoreDiagnosticsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMoreDiagnosticsDialog dialog

class CMoreDiagnosticsDialog : public CDialog
{
public:
	bool vMakeExitButtonGreen;
	bool vDisableEjectorsWhenExit;
	BYTE vNumberOfEjectorsConfigured;
	BYTE vOnlyEjectorNumber;
	bool vOldMeasureSourceStrength;
	BYTE vShowOnlyRealImages;
	bool vChangeMade;
	bool vOldXRayOnNotice;
	bool vWaitingForTestEjectNextContainer;
	WORD vOldActualEncoderRate;
	BYTE vLastRawInterlockInput;
	BYTE vLastRawExternalDetectorInput;
	BYTE vLastRawBodyTriggerInput;
	WORD vOldCurrentBeltPosition;
	//bool vRunWithoutXRays;
	BYTE vOldFPGAMajorVersionNumber;
	BYTE vOldFPGAMinorVersionNumber;
	unsigned long int vDisplayTimerHandle;

	CScanTracDlg *vMainWindowPointer;
	//unsigned long int vGreenLightTimerHandle;
	//tProductCollection *vLocalProductCollection;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;
	bool vSubFunction1ButtonEnable;

	// Construction
	CMoreDiagnosticsDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void UpdateDisplay();
	void DisplayContainerRate();
	void SetChangeMade();
	void UpdateButtons();
	void PrepareAndExit(int TempExitCode);
	void LoadTestCamLinkProduct();
	void StartRunningCamLinkProduct();
	void StartOrStopCamLinkSimulation();
	//void TestSendEMail(CString TempMessage, BYTE TempType);

// Dialog Data
	//{{AFX_DATA(CMoreDiagnosticsDialog)
	enum { IDD = IDD_MoreDiagnosticsDialog };
	CStatic	m_Function3Display;
	CDownFunctionButton	m_Function3Button;
	CLeftFunctionButton	m_SubFunction3Button;
	CLeftFunctionButton	m_SubFunction2Button;
	CLeftFunctionButton	m_SubFunction1Button;
	CDownFunctionButton	m_Function1Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_XRaysOn2;
	CStatic	m_MemoryDisplay;
	CStatic	m_Background;
	CStatic m_ActualEncoderRate;
	CStatic m_ActualEncoderRateLabel;
	CStatic m_BeltPositionLabelStaticText;
	CStatic m_ContainerSimulation;
	CStatic m_ContainersInQueue;
	CStatic m_ContainersInQueueLabel;
	CStatic m_CurrentBeltPositionStaticText;
	CStatic m_DialogTitleStaticText;
	CStatic m_EncoderSimulation;
	CStatic m_EncoderSimulationLabel;
	CStatic m_ExternalDetectorsForcedEnabled;
	CDownFunctionButton m_Function5Button;
	CStatic m_ImageDataType;
	CStatic m_ImageShowTestPatterns;
	CStatic m_RawBodyTriggerInput;
	CStatic m_RawBodyTriggerInputLabel;
	CStatic m_RawExternalDetectorInput;
	CStatic m_RawExternalDetectorInputLabel;
	CStatic m_RawInterlockInput;
	CStatic m_RawInterlockInputLabel;
	CStatic m_ShowContainerTriggerLength;
	CLeftFunctionButton m_SubFunction4Button;
	CRightFunctionButton m_SubFunction5Button;
	CRightFunctionButton m_SubFunction6Button;
	CRightFunctionButton m_SubFunction7Button;
	CStatic m_UnitsLabel;
	CRightFunctionButton m_SubFunction8Button;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMoreDiagnosticsDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMoreDiagnosticsDialog)
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
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOREDIAGNOSTICSDIALOG_H__AA677CB1_0B0F_11D4_ABEA_00500466E305__INCLUDED_)
