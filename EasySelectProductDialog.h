//ScanTrac Side View Source File
#if !defined(AFX_EASYSELECTPRODUCTDIALOG_H__5F229EC4_F3B3_11D3_ABD6_00500466E305__INCLUDED_)
#define AFX_EASYSELECTPRODUCTDIALOG_H__5F229EC4_F3B3_11D3_ABD6_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EasySelectProductDialog.h : header file
//
enum {SelectCurrentProduct,SelectProductForEdit,
	SelectProductForCopy, SelectProductForDelete, SelectProductForNew, SelectProductForDisplay,
	SelectProductForEvaluate};

/////////////////////////////////////////////////////////////////////////////
// CEasySelectProductDialog dialog

class CEasySelectProductDialog : public CDialog
{
public:

	//variables
	bool vRecievedButtonPress;	//do not repeat click and wait for mouse button up if you used keyboard down 
	char vQuickFindCharacter;
	BYTE vMouseState;		//1 - 8 is left mouse button clicked down, 9 - 16 is left mouse button double click, 0 is idle or all functions completed
	unsigned long int vMouseDownRepeatTimerHandle;
	WORD vNumberOfProducts;
	WORD vExtraForDefault;
	CScanTracSystemRunningData *vLocalSystemData;
	HBRUSH vLightGreenBrush;
	HBRUSH vGreenBrush;
	HBRUSH vBlueBrush;
	int SelectWindowForStyle;
	tProductCollection *vLocalProductCollection;
	//int vSPLocalCurrentProductNumber;
	//CProduct *VSPLocalCurrentProduct;
  int vCurrentFirstProductNumberShown;
	WORD vProductNumberSelected;
  //int vOldCurrentFirstProductNumberShown;
	tCWndCollection vLocalCWndCollection;
	tCWndCollection vLocalDisplayCollection;

	// Construction
	CEasySelectProductDialog(CWnd* pParent = NULL);   // standard constructor

	//destructor
	virtual ~CEasySelectProductDialog();

	//methods
	void SelectAndExit(WORD TempProductNumber);
	void UpdateProductDisplay();
	WORD CalculateNumberOfHiddenProducts();
	//void AddProduct(CProduct *TempProduct);

// Dialog Data
	//{{AFX_DATA(CEasySelectProductDialog)
	enum { IDD = IDD_EasySelectProductDialog };
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
	CStatic m_Background;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function4Button;
	CStatic m_ProductLabel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEasySelectProductDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEasySelectProductDialog)
	afx_msg void OnFunction1Button();
	afx_msg void OnFunction2Button();
	afx_msg void OnFunction3Button();
	afx_msg void OnFunction4Button();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSubFunction8Button();
	afx_msg void OnSubFunction1Button();
	afx_msg void OnSubFunction2Button();
	afx_msg void OnSubFunction3Button();
	afx_msg void OnSubFunction4Button();
	afx_msg void OnSubFunction5Button();
	afx_msg void OnSubFunction6Button();
	afx_msg void OnSubFunction7Button();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnFunction5Button();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EASYSELECTPRODUCTDIALOG_H__5F229EC4_F3B3_11D3_ABD6_00500466E305__INCLUDED_)
