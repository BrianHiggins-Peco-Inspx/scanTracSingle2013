//ScanTrac Side View Source File
#if !defined(AFX_HELPINDEXDIALOG_H__B1BD5700_420E_11D4_AC7D_00500466E305__INCLUDED_)
#define AFX_HELPINDEXDIALOG_H__B1BD5700_420E_11D4_AC7D_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HelpIndexDialog.h : header file
//
/*
class CHelpTopic : public CObject
{
public:
	//variables
	CString TopicName;
	long TopicNumber;
};

typedef CTypedPtrArray<CObArray, CHelpTopic*> tHelpTopicCollection;
*/
/////////////////////////////////////////////////////////////////////////////
// CHelpIndexDialog dialog

const BYTE cNumberOfHelpContexts = 57;

class CHelpIndexDialog : public CDialog
{
public:
	//variables
	WORD vHelpOrderIndex[cNumberOfHelpContexts];
	WORD vNumberOfHelpContextsToShow;
	HBRUSH vLightGrayBrush;
	HBRUSH vWhiteBrush;
	HBRUSH vBlueBrush;
  int vCurrentFirstNumberShown;
	WORD vNumberSelected;
	bool vRecievedButtonPress;	//do not repeat click and wait for mouse button up if you used keyboard down 
	BYTE vMouseState;		//1 - 8 is left mouse button clicked down, 9 - 16 is left mouse button double click, 0 is idle or all functions completed
	tCWndCollection vLocalCWndCollection;
	tCWndCollection vLocalDisplayCollection;

	// Construction
	CHelpIndexDialog(CWnd* pParent = NULL);   // standard constructor

	//destructor
	virtual ~CHelpIndexDialog();

	//methods
	void SelectAndExit(BYTE TempProductNumber);
	void UpdateDisplay();

// Dialog Data
	//{{AFX_DATA(CHelpIndexDialog)
	enum { IDD = IDD_HelpIndexDialog };
	CDownFunctionButton	m_Function5Button;
	CDownFunctionButton	m_Function3Button;
	CRightSelectItemFunctionButton	m_SubFunction8Button;
	CRightSelectItemFunctionButton	m_SubFunction7Button;
	CRightSelectItemFunctionButton	m_SubFunction6Button;
	CRightSelectItemFunctionButton	m_SubFunction5Button;
	CLeftSelectItemFunctionButton m_SubFunction4Button;
	CLeftSelectItemFunctionButton m_SubFunction3Button;
	CLeftSelectItemFunctionButton m_SubFunction2Button;
	CLeftSelectItemFunctionButton m_SubFunction1Button;
	CStatic m_DialogTitleStaticText;
	CStatic m_Background;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function2Button;
	CDownFunctionButton m_Function4Button;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelpIndexDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHelpIndexDialog)
	afx_msg void OnFunction3Button();
	afx_msg void OnFunction1Button();
	afx_msg void OnFunction2Button();
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
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELPINDEXDIALOG_H__B1BD5700_420E_11D4_AC7D_00500466E305__INCLUDED_)
