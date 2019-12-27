//ScanTrac Side View Source File
#if !defined(AFX_SYSTEMCOUNTERSDIALOG_H__AAA59FD1_AA10_11DA_B315_00500466E305__INCLUDED_)
#define AFX_SYSTEMCOUNTERSDIALOG_H__AAA59FD1_AA10_11DA_B315_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemCountersDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSystemCountersDialog dialog

class CSystemCountersDialog : public CDialog
{
public:
	//variables
	BYTE vSupportContractStatus;
	bool vMakeExitButtonGreen;
	bool vOldXRayOnNotice;
	bool vChangeMade;
	unsigned long int vUpdateDisplayTimerHandle;
	CScanTracDlg *vMainWindowPointer;
	tProductCollection *vLocalProductCollection;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

	unsigned long int vOldXRaySourceRunDuration;
	unsigned long int vOldDetectorsRunDuration; //keeps track of how many seconds Detectors
	unsigned long int vOldScanTracRunDuration; //keeps track of how many seconds ScanTrac
	unsigned long int vOldXRaySourceWarrantyDuration;
	unsigned long int vOldDetectorsWarrantyDuration;

						//has been on for its whole life

	// Construction
	CSystemCountersDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void SetChangeMade();
	void UpdateDisplay();
	void UpdateButtons();
	bool ResetPasswordOK();
	void ShowDisplay();
	void ResetTubeWarranty(CTime TempDate, CString TempReason, CString TempPerson);
	void ResetTubeRunTimeCounter(CString TempReason, CString TempPerson);
	void ResetDetectorWarranty(CTime TempDate, CString TempReason, CString TempPerson);
	void ResetDetectorRunTimeCounter(CString TempReason, CString TempPerson);
	bool GetName(CString *TempName);
	bool GetReason(CString *TempReason);

// Dialog Data
	//{{AFX_DATA(CSystemCountersDialog)
	enum { IDD = IDD_SystemCountersDialog };
	CStatic	m_SubFunction1eButton2;
	CStatic	m_SubFunction1eButton;
	CStatic	m_SubFunction8eButton;
	CStatic	m_SubFunction6eButton;
	CStatic	m_SubFunction7eButton;
	CStatic	m_SubFunction5eButton;
	CDownFunctionButton	m_Function5Button;
	CDownFunctionButton	m_Function2Button;
	CDownFunctionButton	m_Function1Button;
	CStatic	m_Background;
	CStatic	m_XRaysOn;
	CRightFunctionButton	m_SubFunction8Button;
	CRightFunctionButton	m_SubFunction7Button;
	CRightFunctionButton	m_SubFunction6Button;
	CRightFunctionButton	m_SubFunction5Button;
	CLeftFunctionButton	m_SubFunction4Button;
	CLeftFunctionButton	m_SubFunction3Button;
	CLeftFunctionButton	m_SubFunction2Button;
	CLeftFunctionButton	m_SubFunction1Button;
	CDownFunctionButton m_Function3Button;
	CStatic m_DetectorRunDuration;
	CStatic m_DetectorRunDuration2;
	CStatic m_DetectorRunDuration3;
	CStatic m_DetectorRunDurationLabel;
	CStatic m_DetectorRunDurationLabel2;
	CStatic m_DetectorRunDurationLabel3;
	CStatic m_DetectorWarrantyDate;
	CStatic m_DetectorWarrantyDateLabel;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function4Button;
	CStatic m_MonthsSinceCommissioned;
	CStatic m_MonthsSinceCommissionedLabel;
	CStatic m_ScanTracCommissionedDate;
	CStatic m_ScanTracCommissionedDateLabel;
	CStatic m_ScanTracID;
	CStatic m_ScanTracRunDuration;
	CStatic m_ScanTracRunDurationLabel;
	CStatic m_SupportContract;
	CStatic m_SupportContract2;
	CStatic m_TodaysDate;
	CStatic m_TubeWarrantyDate;
	CStatic m_TubeWarrantyDateLabel;
	CStatic m_XRaySourceDuration;
	CStatic m_XRaySourceDuration2;
	CStatic m_XRaySourceDuration3;
	CStatic m_XRaySourceDurationLabel;
	CStatic m_XRaySourceDurationLabel2;
	CStatic m_XRaySourceDurationLabel3;
	CStatic m_XRayTubeSerialNumber;
	CStatic m_XRayTubeSerialNumberLabel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemCountersDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSystemCountersDialog)
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
	afx_msg void OnSubFunction5eButton();
	afx_msg void OnSubFunction6eButton();
	afx_msg void OnSubFunction7eButton();
	afx_msg void OnSubFunction8eButton();
	afx_msg void OnSubFunction1eButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMCOUNTERSDIALOG_H__AAA59FD1_AA10_11DA_B315_00500466E305__INCLUDED_)
