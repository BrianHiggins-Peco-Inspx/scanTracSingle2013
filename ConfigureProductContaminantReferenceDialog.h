//ScanTrac Side View Source File
#if !defined(AFX_CONFIGUREPRODUCTCONTAMINANTREFERENCEDIALOG_H__CC402F91_2533_11D5_ADB2_00500466E305__INCLUDED_)
#define AFX_CONFIGUREPRODUCTCONTAMINANTREFERENCEDIALOG_H__CC402F91_2533_11D5_ADB2_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigureProductContaminantReferenceDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigureProductContaminantReferenceDialog dialog

class CConfigureProductContaminantReferenceDialog : public CDialog
{
public:
	//variables
	BYTE vReferenceROIsToEdges;
	BYTE vReferenceToTop;
	WORD vReferenceToBottom;
	CInspection *vInspectionEditing;
	CString vReferenceLeft;
	CString vReferenceRight;
	CString vReferenceTop;
	CString vReferenceBottom;
	//CString vReferenceTolerance;
	bool vChangeMade;
	CString *vProductName;
	CProduct *vPointerToProductEditing;
	CSystemConfigurationData *vLocalConfigurationData;
	CScanTracSystemRunningData *vLocalSystemData;
	tCWndCollection vLocalCWndCollection;

	// Construction
	CConfigureProductContaminantReferenceDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void SetChangeMade();
	void UpdateDisplay();
// Dialog Data
	//{{AFX_DATA(CConfigureProductContaminantReferenceDialog)
	enum { IDD = IDD_ConfigureProductContaminantReferenceDialog };
	
	CStatic	m_Background;
	CStatic m_ContaminantEjector;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton	m_Function2Button;
	CDownFunctionButton m_Function3Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton m_Function5Button;
	CStatic m_Left;
	CStatic m_LeftReference;
	CStatic m_ProductNameTitle;
	CStatic m_Right;
	CStatic m_RightReference;
	CLeftFunctionButton m_SubFunction1Button;
	CLeftFunctionButton m_SubFunction2Button;
	CLeftFunctionButton m_SubFunction3Button;
	CLeftFunctionButton m_SubFunction4Button;
	CRightFunctionButton	m_SubFunction5Button;
	CRightFunctionButton m_SubFunction6Button;
	CRightFunctionButton m_SubFunction7Button;
	CRightFunctionButton m_SubFunction8Button;
	CStatic m_SubFunction2Display;
	CStatic	m_SubFunction5Display;
	CStatic m_SubFunction6Display;
	CStatic	m_SubFunction7Display;
	CStatic m_ToleranceReference;
	CStatic m_TopReference;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigureProductContaminantReferenceDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigureProductContaminantReferenceDialog)
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

#endif // !defined(AFX_CONFIGUREPRODUCTCONTAMINANTREFERENCEDIALOG_H__CC402F91_2533_11D5_ADB2_00500466E305__INCLUDED_)
