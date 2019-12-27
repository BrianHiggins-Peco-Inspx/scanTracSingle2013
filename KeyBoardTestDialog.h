//ScanTrac Side View Source File
#if !defined(AFX_KEYBOARDTESTDIALOG_H__AA677CB2_0B0F_11D4_ABEA_00500466E305__INCLUDED_)
#define AFX_KEYBOARDTESTDIALOG_H__AA677CB2_0B0F_11D4_ABEA_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyBoardTestDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKeyBoardTestDialog dialog

const WORD cGreenTimeDelay = 1000;
class CKeyBoardTestDialog : public CDialog
{
public:
	//variables
	bool vShowRed;
	bool vShowMultiple;
	unsigned long int vGreenLightTimerHandle;
	int vCurrentKeyIDD;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

	// Construction
	CKeyBoardTestDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void TurnKeyGreen(int TempKeyIDC);

// Dialog Data
	//{{AFX_DATA(CKeyBoardTestDialog)
	enum { IDD = IDD_KeyBoardTestDialog };
	CStatic	m_LeftRight;
	CStatic	m_ScreenCapture;
	CStatic	m_HiddenKey;
	CStatic	m_UnusedKey;
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
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function2Button;
	CDownFunctionButton m_Function3Button;
	CDownFunctionButton m_Function4Button;
	CDownFunctionButton m_Function5Button;
	CStatic m_NoWindow;
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
	//{{AFX_VIRTUAL(CKeyBoardTestDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKeyBoardTestDialog)
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
	afx_msg void OnKey1();
	afx_msg void OnKey2();
	afx_msg void OnKey3();
	afx_msg void OnKey4();
	afx_msg void OnKey5();
	afx_msg void OnKey6();
	afx_msg void OnKey7();
	afx_msg void OnKey8();
	afx_msg void OnKey9();
	afx_msg void OnKey0();
	afx_msg void OnKeyClear();
	afx_msg void OnKeyHelp();
	afx_msg void OnScreenCapture();
	afx_msg void OnLeftRight();
	afx_msg void OnHiddenKey();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUnused();
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYBOARDTESTDIALOG_H__AA677CB2_0B0F_11D4_ABEA_00500466E305__INCLUDED_)
