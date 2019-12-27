#if !defined(AFX_SETUPUSERSDIALOG_H__8463B021_4DF6_11D5_AE03_00500466E305__INCLUDED_)
#define AFX_SETUPUSERSDIALOG_H__8463B021_4DF6_11D5_AE03_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//typedef  CTypedPtrList<CObList, UserPasswords**> tUserPointerCollection;

const BYTE cMaxNumberOfItemsPerPage = 8;

// CSetupUserDialog dialog

class CSetupUserDialog : public CDialog
{
	DECLARE_DYNAMIC(CSetupUserDialog)
	bool vChangeMade;
	BYTE vCurrentFilteredLevel;
	BYTE vCurrentFirstItem;
	BYTE vCurrentNumberOfFilteredUsers;
	tUserCollection vLocalUserCollection;
	tUserCollection vFilteredUserCollection;

	CScanTracSystemRunningData *vLocalSystemData;
	tCWndCollection vLocalCWndCollection;
	tCWndCollection vLocalDisplayCollection;
	CScanTracDlg *vMainWindowPointer;

	public:
	CSetupUserDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetupUserDialog();
	void UpdateDisplay();
	void MakeFilteredList(BYTE UserLevel);
	void LoadData (tUserCollection &UserCollection, BYTE FilterLevel);

	// Dialog Data
	//{{AFX_DATA(CSetupUsers)
	enum { IDD = IDD_SetupUsers };
	CStatic	m_Background;
//	CStatic m_ArrowDown1;
//	CStatic m_ArrowDown2;
//	CStatic m_ArrowDown3;
//	CStatic m_ArrowDown4;
//	CStatic m_ArrowDown5;
//	CStatic m_ArrowLeft1;
//	CStatic m_ArrowLeft2;
//	CStatic m_ArrowLeft3;
//	CStatic m_ArrowLeft4;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton	m_Function2Button;
	CDownFunctionButton m_Function3Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton m_Function5Button;
	CLeftDoubleFunctionButton m_SubFunction1Button;
	CStatic m_SubFunction1Display;
	CLeftDoubleFunctionButton m_SubFunction2Button;
	CStatic m_SubFunction2Display;
	CLeftDoubleFunctionButton m_SubFunction3Button;
	CStatic m_SubFunction3Display;
	CLeftDoubleFunctionButton m_SubFunction4Button;
	CStatic m_SubFunction4Display;
	CRightDoubleFunctionButton m_SubFunction5Button;
	CStatic m_SubFunction5Display;
	CRightDoubleFunctionButton m_SubFunction6Button;
	CStatic m_SubFunction6Display;
	CRightDoubleFunctionButton m_SubFunction7Button;
	CStatic m_SubFunction7Display;
	CRightDoubleFunctionButton m_SubFunction8Button;
	CStatic m_SubFunction8Display;
//	CStatic m_ArrowRight1;
//	CStatic m_ArrowRight2;
//	CStatic m_ArrowRight3;
//	CStatic m_ArrowRight4;
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
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
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
	afx_msg void OnDialogtitlestatictext1();
	DECLARE_MESSAGE_MAP()

	private:
	void DeleteUser(UserPasswords* TempUser);
	void SetupMenu();
};

#endif