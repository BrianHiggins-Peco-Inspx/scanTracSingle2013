//ScanTrac Side View Source File
#include "afxwin.h"
#if !defined(AFX_SELECTITEMDIALOG_H__1202A541_42E0_11D4_AC80_00500466E305__INCLUDED_)
#define AFX_SELECTITEMDIALOG_H__1202A541_42E0_11D4_AC80_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectItemDialog.h : header file
//
const BYTE cMaxSelectDialogItems = 200;
enum {cItemSelectDialog,cProductSelectDialog,
	cHelpIndexSelectDialog};

/////////////////////////////////////////////////////////////////////////////
// CSelectItemDialog dialog

class CSelectItemDialog : public CDialog
{
public:

	//variables
	unsigned int vSetWindowActiveTimerHandle;
	WORD vGreenItem;
	WORD vYellowItem;
	bool vItemEnabled[cMaxSelectDialogItems];
	bool vShowExit;
	bool vInAutoSetup;
	bool vStayTillDecissionSetup;
	CString vStringItemSelecting;
	BYTE vSelectDialogType;
	COLORREF vBackgroundColor;
	BYTE vNumberOfItems;
	HBRUSH vLightBackgroundBrush;
	HBRUSH vGreenBrush;
	HBRUSH vBlueBrush;
	HBRUSH vYellowBrush;
	HBRUSH vBackgroundBrush;
	CString vItems[cMaxSelectDialogItems + 1];
	CString vTitleString;
	CString vOkayMessage;
	int vCurrentFirstItemNumberShown;
	BYTE vItemNumberSelected;
	bool vMakeTitle3Big;
	bool vMakeTitle3Highlighted;
	char vQuickFindCharacter;			//8/8/2012
	tCWndCollection vLocalCWndCollection;
	tCWndCollection vLocalDisplayCollection;
	bool vSubFunction1ButtonEnable;
	bool vSubFunction2ButtonEnable;
	bool vSubFunction3ButtonEnable;
	bool vSubFunction4ButtonEnable;
	bool vSubFunction5ButtonEnable;
	bool vSubFunction6ButtonEnable;
	bool vSubFunction7ButtonEnable;
	bool vSubFunction8ButtonEnable;

	// Construction
	CSelectItemDialog(CWnd* pParent = NULL);   // standard constructor

	//destructor
	virtual ~CSelectItemDialog();

	//methods
	void SelectAndExit(BYTE TempProductNumber);
	void SelectAndContinue(BYTE TempItemNumber);
	void UpdateItemDisplay();
	void ShowCurrentSelectionNotAvailableDialog(void);

// Dialog Data
	//{{AFX_DATA(CSelectItemDialog)
	enum { IDD = IDD_SelectItemDialog };
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CDownFunctionButton	m_Function5Button;
	CDownFunctionButton	m_Function3Button;
	CRightSelectItemFunctionButton	m_SubFunction8Button;
	CRightSelectItemFunctionButton	m_SubFunction7Button;
	CRightSelectItemFunctionButton	m_SubFunction6Button;
	CRightSelectItemFunctionButton	m_SubFunction5Button;
	CLeftSelectItemFunctionButton	m_SubFunction4Button;
	CLeftSelectItemFunctionButton	m_SubFunction3Button;
	CLeftSelectItemFunctionButton	m_SubFunction2Button;
	CLeftSelectItemFunctionButton	m_SubFunction1Button;
	CString	m_DialogTitleStaticText2;
	CString	m_DialogTitleStaticText3;
	CString m_DialogTitleStaticText4;
	CString m_DialogTitleStaticText5;
	CStatic m_DialogTitleControlStaticText5;
	CStatic m_Background;
	CStatic m_DialogTitleStaticText1;
	CDownFunctionButton m_Function1Button;
	CStatic m_DialogTitleStaticTextBox2;
	CStatic m_DialogTitleStaticTextBox3;
	CStatic m_DialogTitleStaticTextBox4;
	CStatic m_DialogTitleStaticTextBox5;
	CStatic m_DialogTitleControlStaticTextBox5;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectItemDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectItemDialog)
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTITEMDIALOG_H__1202A541_42E0_11D4_AC80_00500466E305__INCLUDED_)
