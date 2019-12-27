//ScanTrac Side View Source File
#if !defined(AFX_SELECTMULTIPLEITEMDIALOG_H__8463B021_4DF6_11D5_AE03_00500466E305__INCLUDED_)
#define AFX_SELECTMULTIPLEITEMDIALOG_H__8463B021_4DF6_11D5_AE03_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectMultipleItemDialog.h : header file
//

const BYTE cMaxMultipleSelectDialogItems = 36;

/////////////////////////////////////////////////////////////////////////////
// CSelectMultipleItemDialog dialog

class CSelectMultipleItemDialog : public CDialog
{
public:
	CBitmap vGraphicBitmap;
	RECT TempRect;
	bool vOutboardItemMustBeDisabled;
  int vCurrentFirstItemNumberShown;
	bool vUseSelectText;
	bool vItemEnabled[cMaxMultipleSelectDialogItems];
	bool vChangeMade;
	BYTE vInAutoSetup;
	BYTE vNumberOfItems;
	HBRUSH vLightGreenBrush;
	HBRUSH vGreenBrush;
	HBRUSH vYellowBrush;
	CString vItemsYes[cMaxMultipleSelectDialogItems + 1];
	CString vItemsNo[cMaxMultipleSelectDialogItems + 1];
	DWORD vItems[cMaxMultipleSelectDialogItems + 1];
	CString vTitleString;
	tCWndCollection vLocalCWndCollection;
	tCWndCollection vLocalDisplayCollection;

	// Construction
	CSelectMultipleItemDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void SelectAndExit(BYTE TempProductNumber);
	void SetChangeMade();
	void UpdateItemDisplay();
	bool IsThereAnInwardItemDisabled(BYTE TempCurrentItem);
	bool IsThereAnOutwardItemEnabled(BYTE TempCurrentItem);

	// Dialog Data
	//{{AFX_DATA(CSelectMultipleItemDialog)
	enum { IDD = IDD_SelectMultipleItemDialog };
	CDownFunctionButton	m_Function3Button;
	CLeftDoubleFunctionButton	m_SubFunction2Button;
	CLeftDoubleFunctionButton	m_SubFunction1Button;
	CDownFunctionButton	m_Function5Button;
	CRightDoubleFunctionButton	m_SubFunction8Button;
	CRightDoubleFunctionButton	m_SubFunction7Button;
	CRightDoubleFunctionButton	m_SubFunction6Button;
	CRightDoubleFunctionButton	m_SubFunction5Button;
	CLeftDoubleFunctionButton	m_SubFunction4Button;
	CLeftDoubleFunctionButton	m_SubFunction3Button;
	CStatic	m_CheckMarkGraphic8;
	CStatic	m_CheckMarkGraphic7;
	CStatic	m_CheckMarkGraphic6;
	CStatic	m_CheckMarkGraphic5;
	CStatic	m_CheckMarkGraphic4;
	CStatic	m_CheckMarkGraphic3;
	CStatic	m_CheckMarkGraphic2;
	CStatic	m_CheckMarkGraphic1;
	CStatic	m_Item8;
	CStatic	m_Item7;
	CStatic	m_Item6;
	CStatic	m_Item5;
	CStatic	m_Item4;
	CStatic	m_Item3;
	CStatic	m_Item2;
	CStatic	m_Item1;
	CDownFunctionButton	m_Function2Button;
	CDownFunctionButton	m_Function4Button;
	CStatic	m_Background;
	CString	m_DialogTitleStaticText;
	CStatic m_DialogTitleStaticTextBox;
	CDownFunctionButton m_Function1Button;
	//}}AFX_DATA

	//destructor
	virtual ~CSelectMultipleItemDialog();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectMultipleItemDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectMultipleItemDialog)
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
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTMULTIPLEITEMDIALOG_H__8463B021_4DF6_11D5_AE03_00500466E305__INCLUDED_)
