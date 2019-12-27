#if !defined(AFX_EDITUSERSDIALOG_H__8463B021_4DF6_11D5_AE03_00500466E305__INCLUDED_)
#define AFX_EDITUSERSDIALOG_H__8463B021_4DF6_11D5_AE03_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEditUser : public CDialog
{
	DECLARE_DYNAMIC(CEditUser)

	bool vChangeMade;
	bool vNewUser;
	bool vUserClickedPassword;
	bool vShowMainMenuButton;
	UserPasswords vCurrentUser;
	tUserCollection* vLocalUserCollection;
	CScanTracSystemRunningData *vLocalSystemData;
	CScanTracDlg *vMainWindowPointer;
	tCWndCollection vLocalCWndCollection;

	public:
	CEditUser(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditUser();
	void UpdateDisplay();
	INT_PTR DoModal();

	// Dialog Data
	//{{AFX_DATA(CSetupUsers)
	enum { IDD = IDD_EditUserDialog };
	CStatic	m_Background;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton	m_Function2Button;
	CDownFunctionButton m_Function3Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton m_Function5Button;
	CLeftFunctionButton m_SubFunction1Button;
	CLeftFunctionButton m_SubFunction2Button;
	CLeftFunctionButton m_SubFunction3Button;
	CLeftFunctionButton m_SubFunction4Button;
	CRightFunctionButton m_SubFunction5Button;
	CRightFunctionButton m_SubFunction6Button;
	CRightFunctionButton m_SubFunction7Button;
	CRightFunctionButton m_SubFunction8Button;
	CStatic m_SubFunction1Display;
	CStatic m_SubFunction2Display;
	CStatic m_SubFunction3Display;
	CStatic m_SubFunction4Display;
	CStatic m_SubFunction5Display;
	CStatic m_SubFunction6Display;
	CStatic m_SubFunction7Display;
	CStatic m_SubFunction8Display;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetupUsers)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	protected:

	// Generated message map functions
	//{{AFX_MSG(CSetupUsers)
//	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	
	afx_msg void OnSubfunction3display();

	private:
	bool FindUserName(CString TempUserName);
public:
	afx_msg void OnDialogtitlestatictext1();
};

#endif