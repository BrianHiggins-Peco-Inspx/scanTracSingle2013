//ScanTrac Side View Source File
#include "afxwin.h"
#if !defined(AFX_NEWEDITPRODUCTDIALOG_H__0F46AAC4_F51C_11D3_ABD6_00500466E305__INCLUDED_)
#define AFX_NEWEDITPRODUCTDIALOG_H__0F46AAC4_F51C_11D3_ABD6_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewEditProductDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewEditProductDialog dialog

class CNewEditProductDialog : public CDialog
{

public:
	//variables
	CString vOriginalName;
	int vNumberOfBodyTriggersInSystem;
	unsigned long int vOpenNewWindowTimerHandle;
	tProductCollection *vLocalProductCollection;
	BYTE vEditExistingProduct;
	bool vChangeMade;
	CString vProductNameString;
	CScanTracDlg *vMainWindowPointer;

	CSystemConfigurationData *vLocalConfigurationData;
	CScanTracSystemRunningData *vLocalSystemData;
	tCWndCollection vLocalCWndCollection;

	// Construction
	CNewEditProductDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void SetChangeMade();
	BYTE SetupBodyTrigger(bool TempInAutoSetup, BYTE TempShowExit);
	BYTE SetupExternalDetectors(bool TempInAutoSetup);
	BYTE SetupProductEjectors(bool TempInAutoSetup, bool TempStopRunningWhenExit);
	void AbortDialog(WORD TempExitCode);
	void EvaluateProduct();
	void DoProductAutoSetup(BYTE TempStartStep, BYTE TempStartWindow);
	void SaveAndExit();
	void AddProduct(CProduct *TempProduct);

// Dialog Data
	//{{AFX_DATA(CNewEditProductDialog)
	enum { IDD = IDD_NewEditProductDialog };
	CDownFunctionButton	m_Function5Button;
	CRightFunctionButton	m_SubFunction8Button;
	CLeftFunctionButton	m_SubFunction3Button;
	CLeftFunctionButton	m_SubFunction1Button;
	CLeftFunctionButton	m_SubFunction2Button;
	CRightFunctionButton	m_SubFunction5Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_Background;
	CRightFunctionButton m_SubFunction7Button;
	CStatic m_BodyTriggerToImager;
	CStatic m_DialogTitleStaticText;
	CStatic m_EndOfLine;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function3Button;
	CStatic m_ProductName;
	CLeftFunctionButton m_SubFunction4Button;
	CRightFunctionButton m_SubFunction6Button;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewEditProductDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewEditProductDialog)
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
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSubFunction6Button();
	afx_msg void OnSubFunction7Button();
	afx_msg void OnSubFunction8Button();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWEDITPRODUCTDIALOG_H__0F46AAC4_F51C_11D3_ABD6_00500466E305__INCLUDED_)
