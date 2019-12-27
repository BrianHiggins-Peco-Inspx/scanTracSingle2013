//ScanTrac Side View Source File
#if !defined(AFX_DIAGNOSTICCOUNTERSDIALOG_H__42073132_FB7B_11D4_AD4C_00500466E305__INCLUDED_)
#define AFX_DIAGNOSTICCOUNTERSDIALOG_H__42073132_FB7B_11D4_AD4C_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiagnosticCountersDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiagnosticCountersDialog dialog

class CDiagnosticCountersDialog : public CDialog
{
public:
	//variables
	bool vMakeExitButtonGreen;
	bool vOldXRayOnNotice;
	bool vChangeMade;
	unsigned long int vUpdateDisplayTimerHandle;
	CScanTracDlg *vMainWindowPointer;
	tProductCollection *vLocalProductCollection;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

	double vOldMaximumExternalDetectorTime;
	unsigned long int vOldEjector[cNumberOfEjectors];
	unsigned long int vOldEjectorConfirm[cNumberOfEjectors];
	unsigned long int vOldBodyTriggersCount;
	unsigned long int vOldBackupBodyTriggersCount;
	unsigned long int vOldValidBodyTriggersCount;
	unsigned long int vOldExternalDetectorCount[cNumberOfExternalDetectors];
	//unsigned long int vOldExternalDetectorEjectsCount[cNumberOfExternalDetectors];
	unsigned long int vOldXRayCriteriaCount[cMaximumNumberOfCountsDisplayed];
	
	unsigned long int vOldIncompleteDataCount;
	unsigned long int vOldImageReferenceErrorCount;
	unsigned long int vOldProcessingErrorCount;
	unsigned long int vOldTooLateToEjectErrorCount;
	unsigned long int vOldMissedEjectsBecauseDisabledCount;
	unsigned long int vOldUnConfirmedCount;
	unsigned long int vOldRealImageCountTotal;
	unsigned long int vOldGoodCountTotal;
	unsigned long int vOldEjectCountTotal;

	// Construction
	CDiagnosticCountersDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void SetChangeMade();
	void UpdateDisplay();
	void UpdateButtons();


// Dialog Data
	//{{AFX_DATA(CDiagnosticCountersDialog)
	enum { IDD = IDD_DiagnosticCountersDialog };
	CRightFunctionButton	m_SubFunction8Button;
	CRightFunctionButton	m_SubFunction7Button;
	CDownFunctionButton	m_Function1Button;
	CStatic	m_UnConfirmed;
	CStatic	m_UnConfirmedLabel;
	CStatic	m_BackupBodyTriggersLabel;
	CStatic	m_BackupBodyTriggers;
	CStatic	m_EjectedLabel;
	CStatic	m_ConfirmLabel;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_XRaysOn;
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
	CStatic m_BodyTriggers;
	CStatic m_BodyTriggersLabel;
	CStatic m_DetectorRunDuration;
	CStatic m_DetectorRunDurationLabel;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function3Button;
	CDownFunctionButton m_Function4Button;
	CDownFunctionButton m_Function5Button;
	CStatic m_ImageReferenceError;
	CStatic m_ImageReferenceErrorLabel;
	CStatic m_IncompleteData;
	CStatic m_IncompleteDataLabel;
	CStatic m_MaxTime;
	CStatic m_MaxTimeLabel;
	CStatic m_MissedEjectsBecauseDisabled;
	CStatic m_MissedEjectsBecauseDisabledLabel;
	CStatic m_ProcessingError;
	CStatic m_ProcessingErrorLabel;
	CStatic m_RealImages;
	CStatic m_RealImagesLabel;
	CStatic m_ScanTracRunDuration;
	CStatic m_ScanTracRunDurationLabel;
	CStatic m_SubFunction1Button;
	CRightFunctionButton m_SubFunction2Button;
	CRightFunctionButton m_SubFunction3Button;
	CRightFunctionButton m_SubFunction4Button;
	CLeftFunctionButton m_SubFunction5Button;
	CLeftFunctionButton m_SubFunction6Button;
	CStatic m_TooLateToEjectError;
	CStatic m_TooLateToEjectErrorLabel;
	CStatic m_ValidBodyTriggers;
	CStatic m_ValidBodyTriggersLabel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiagnosticCountersDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiagnosticCountersDialog)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAGNOSTICCOUNTERSDIALOG_H__42073132_FB7B_11D4_AD4C_00500466E305__INCLUDED_)
