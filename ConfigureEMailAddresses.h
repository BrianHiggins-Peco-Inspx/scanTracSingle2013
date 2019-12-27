//ScanTrac Side View Source File
#if !defined(AFX_CONFIGUREEMAILADDRESSES_H__2D4640A1_B425_11D5_AEAE_00500466E305__INCLUDED_)
#define AFX_CONFIGUREEMAILADDRESSES_H__2D4640A1_B425_11D5_AEAE_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigureEMailAddresses.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigureEMailAddresses dialog

class CConfigureEMailAddresses : public CDialog
{
public:
	BYTE vFirstUser;
	bool vChangeMade;
	BYTE vEMailCategory;
	CString vEMailUser[cMaximumEMailRecievers];
	CString vEMailDomain[cMaximumEMailRecievers];
	CString vEMailSubject;
	BYTE vEMailEnable;
	BYTE vNumberOfEMailRecievers;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;
	CScanTracDlg *vMainWindowPointer;
	bool vFunction3ButtonEnable;
	bool vFunction5ButtonEnable;
	bool vSubFunction1ButtonEnable;
	bool vSubFunction2ButtonEnable;
	bool vSubFunction3ButtonEnable;
	bool vSubFunction4ButtonEnable;
	bool vSubFunction5ButtonEnable;
	bool vSubFunction6ButtonEnable;
	bool vSubFunction7ButtonEnable;

// Construction
	CConfigureEMailAddresses(CWnd* pParent = NULL);   // standard constructor

	//methods
	void SetChangeMade();
	void UpdateDisplay();
	void UpdateDisplayEnable();
	void EditEMailUser(BYTE TempUser);
	void EditEMailDomain(BYTE TempUser);
	void UpdateButtons();
	void SortAddresses(WORD TempUserIndex, WORD TempSubjectIndex);

	// Dialog Data
	//{{AFX_DATA(CConfigureEMailAddresses)
	enum { IDD = IDD_ConfigureEMailAddresses };
	CDownFunctionButton	m_Function5Button;
	CStatic	m_Background;
	CRightFunctionButton	m_SubFunction8Button;
	CRightFunctionButton	m_SubFunction7Button;
	CRightFunctionButton	m_SubFunction6Button;
	CRightFunctionButton	m_SubFunction5Button;
	CLeftFunctionButton	m_SubFunction4Button;
	CLeftFunctionButton	m_SubFunction3Button;
	CLeftFunctionButton	m_SubFunction2Button;
	CLeftFunctionButton	m_SubFunction1Button;
	CDownFunctionButton	m_Function3Button;
	CStatic	m_Enable;
	CStatic	m_Subject;
	CStatic	m_User4;
	CStatic	m_User3;
	CStatic	m_User2;
	CStatic	m_User1;
	CStatic	m_Domain4;
	CStatic	m_Domain3;
	CStatic	m_Domain2;
	CStatic	m_Domain1;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic m_AddressType;
	CStatic m_At1;
	CStatic m_At2;
	CStatic m_At3;
	CStatic m_At4;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigureEMailAddresses)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ClickEnableBeforeChangeAvailableDialogMessage(void);
	void ShowNoMouseClickDialogMessage(void);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigureEMailAddresses)
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGUREEMAILADDRESSES_H__2D4640A1_B425_11D5_AEAE_00500466E305__INCLUDED_)
