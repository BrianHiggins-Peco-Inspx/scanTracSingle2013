#if !defined(AFX_CONFIGUREPASSWORDSDIALOG_H__E4BDD1BC_CB3B_4383_AA53_B7669B72A2D0__INCLUDED_)
#define AFX_CONFIGUREPASSWORDSDIALOG_H__E4BDD1BC_CB3B_4383_AA53_B7669B72A2D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigurePasswordsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigurePasswordsDialog dialog

class CConfigurePasswordsDialog : public CDialog
{
public:
	bool vChangeMade;
	CString vProductSetupPassword;
	CString vProductSimpleSetupPassword;
	CString vAdvancedSetupPassword;
	CString vCertifiedSetupPassword;
	CString vInspxPassword;
	CString vResetCountsPassword;
	CString vSystemSetupResetPassword;
	CString vOperatorPassword;
	CString vQAPassword;
	CString vSamplePassword;

	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;
	CScanTracDlg *vMainWindowPointer;
  unsigned long int vOneSecondTimerHandle;
	int vOldPasswordLevel;

	// Construction
	CConfigurePasswordsDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void SetChangeMade();
	void ShowInspxItems();
	void HideInspxItems();
	void UpdateDisplay();

// Dialog Data
	//{{AFX_DATA(CConfigurePasswordsDialog)
	enum { IDD = IDD_ConfigurePasswords };
	CLeftFunctionButton	m_SubFunction3Button;
	CRightFunctionButton	m_SubFunction6Button;
	CLeftFunctionButton	m_SubFunction1Button;
	CLeftFunctionButton	m_SubFunction2Button;
	CStatic	m_Background;
	CLeftFunctionButton	m_SubFunction4Button;
	CRightFunctionButton	m_SubFunction8Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CDownFunctionButton m_Function3Button;
	CDownFunctionButton m_Function5Button;
	CRightFunctionButton m_SubFunction7Button;
	CStatic m_Clock;
	CStatic m_CPMTimePeriodDisplay;
	CStatic m_Date;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CStatic m_Function5Display;
	CStatic m_SubFunction1Display;
	CStatic m_SubFunction2Display;
	CStatic m_SubFunction3Display;
	CStatic m_SubFunction4Display;
	CRightFunctionButton m_SubFunction5Button;
	CStatic m_WorkStopTimePeriodDisplay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigurePasswordsDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigurePasswordsDialog)
	afx_msg void OnFunction1Button();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnFunction2Button();
	afx_msg void OnFunction3Button();
	afx_msg void OnFunction4Button();
	afx_msg void OnSubFunction1Button();
	afx_msg void OnFunction5Button();
	afx_msg void OnSubFunction2Button();
	afx_msg void OnSubFunction3Button();
	afx_msg void OnSubFunction5Button();
	afx_msg void OnSubFunction4Button();
	afx_msg void OnSubFunction6Button();
	afx_msg void OnSubFunction7Button();
	afx_msg void OnSubFunction8Button();
	afx_msg void OnDialogTitleStaticText();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGUREPASSWORDSDIALOG_H__E4BDD1BC_CB3B_4383_AA53_B7669B72A2D0__INCLUDED_)
