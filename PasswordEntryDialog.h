//ScanTrac Side View Source File
#if !defined(AFX_PASSWORDENTRYDIALOG_H__57EFD4C2_FA14_11D3_ABD6_00500466E305__INCLUDED_)
#define AFX_PASSWORDENTRYDIALOG_H__57EFD4C2_FA14_11D3_ABD6_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PasswordEntryDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPasswordEntryDialog dialog
extern BYTE vGlobalShowMouseCursor;

class CPasswordEntryDialog : public CDialog
{
public:
	//variables
	unsigned int vCheckIfShouldCloseTimerHandle;
	BYTE vMinimumPasswordLength;
	bool vLowerPasswordRequiresThisOne;
	HBRUSH vGreenBrush;
	HBRUSH vWhiteBrush;
	HBRUSH vLightBlueBrush;
	HBRUSH vBlueBrush;
	//bool vDeleteAll;
	BYTE vCursorPosition;
	CString vEditString;
	CString vDisplayString;
	tCWndCollection vLocalCWndCollection;

	// Construction
	CPasswordEntryDialog(CWnd* pParent = NULL);   // standard constructor

	//destructor
	virtual ~CPasswordEntryDialog();

	//Methods
	void SetCursor();
	void ProcessKey(BYTE TempKey);

	// Dialog Data
	//{{AFX_DATA(CPasswordEntryDialog)
	enum { IDD = IDD_PasswordEntryDialog };
	CStatic	m_KeyHelp;
	CStatic	m_KeyClear;
	CStatic	m_Key9;
	CStatic	m_Key8;
	CStatic	m_Key7;
	CStatic	m_Key6;
	CStatic	m_Key5;
	CStatic	m_Key4;
	CStatic	m_Key3;
	CStatic	m_Key2;
	CStatic	m_Key1;
	CStatic	m_Key0;
	CStatic	m_Background;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function2Button;
	CDownFunctionButton m_Function3Button;
	CDownFunctionButton m_Function4Button;
	CDownFunctionButton m_Function5Button;
	CLeftFunctionButton m_SubFunction1Button;
	CLeftFunctionButton m_SubFunction2Button;
	CLeftFunctionButton m_SubFunction3Button;
	CLeftFunctionButton m_SubFunction4Button;
	CRightFunctionButton m_SubFunction5Button;
	CRightFunctionButton m_SubFunction6Button;
	CRightFunctionButton m_SubFunction7Button;
	CRightFunctionButton m_SubFunction8Button;
	CStatic m_DialogTitleStaticTextBox1;
	CStatic m_DialogTitleStaticTextBox2;
	CString	m_DialogTitleStaticText1;
	CString	m_DialogTitleStaticText2;
	CStatic m_DialogTitleStaticText7;
	CStatic m_EditStaticText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPasswordEntryDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPasswordEntryDialog)
	afx_msg void OnFunction1Button();
	afx_msg void OnFunction2Button();
	afx_msg void OnFunction4Button();
	afx_msg void OnFunction5Button();
	afx_msg void OnSubFunction3Button();
	afx_msg void OnSubFunction4Button();
	afx_msg void OnSubFunction8Button();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnKey0();
	afx_msg void OnKey1();
	afx_msg void OnKey2();
	afx_msg void OnKey3();
	afx_msg void OnKey4();
	afx_msg void OnKey5();
	afx_msg void OnKey6();
	afx_msg void OnKey7();
	afx_msg void OnKey8();
	afx_msg void OnKey9();
	afx_msg void OnKeyClear();
	afx_msg void OnKeyHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext7();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSWORDENTRYDIALOG_H__57EFD4C2_FA14_11D3_ABD6_00500466E305__INCLUDED_)
