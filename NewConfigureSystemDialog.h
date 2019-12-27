//ScanTrac Side View Source File
#if !defined(AFX_NEWCONFIGURESYSTEMDIALOG_H__0F46AAC3_F51C_11D3_ABD6_00500466E305__INCLUDED_)
#define AFX_NEWCONFIGURESYSTEMDIALOG_H__0F46AAC3_F51C_11D3_ABD6_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewConfigureSystemDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewConfigureSystemDialog dialog

class CNewConfigureSystemDialog : public CDialog
{
public:
	//variables
	unsigned long int vOneSecondTimerHandle;
	CScanTracDlg *vMainWindowPointer;
	//bool vChangeMade;

	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tProductCollection *vLocalProductCollection;
	tCWndCollection vLocalCWndCollection;

	// Construction
	CNewConfigureSystemDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	//void SetChangeMade();
	//void UpdateTimeDisplay();
	//void EnableEMail();
	void DoInspxHiddenSetup();
	void DoInspxHiddenSystemSetup();
	void UpdateButtons();
// Dialog Data
	//{{AFX_DATA(CNewConfigureSystemDialog)
	enum { IDD = IDD_NewConfigureSystemDialog };
	CDownFunctionButton	m_Function5Button;
	CDownFunctionButton	m_Function3Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_Background;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CLeftFunctionButton m_SubFunction1Button;
	CLeftFunctionButton m_SubFunction2Button;
	CLeftFunctionButton m_SubFunction3Button;
	CLeftFunctionButton m_SubFunction4Button;
	CRightFunctionButton m_SubFunction5Button;
	CRightFunctionButton m_SubFunction6Button;
	CRightFunctionButton m_SubFunction7Button;
	CRightFunctionButton m_SubFunction8Button;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewConfigureSystemDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewConfigureSystemDialog)
	afx_msg void OnFunction1Button();
	afx_msg void OnFunction2Button();
	afx_msg void OnFunction4Button();
	afx_msg void OnFunction3Button();
	afx_msg void OnFunction5Button();
	afx_msg void OnSubFunction1Button();
	afx_msg void OnSubFunction2Button();
	afx_msg void OnSubFunction3Button();
	afx_msg void OnSubFunction5Button();
	afx_msg void OnSubFunction6Button();
	afx_msg void OnSubFunction7Button();
	afx_msg void OnSubFunction8Button();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSubFunction4Button();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWCONFIGURESYSTEMDIALOG_H__0F46AAC3_F51C_11D3_ABD6_00500466E305__INCLUDED_)
