//ScanTrac Side View Source File
#if !defined(AFX_PRODUCTSHAPEHANDLINGDIALOG_H__F7B541F1_E1E0_11D6_B030_00500466E305__INCLUDED_)
#define AFX_PRODUCTSHAPEHANDLINGDIALOG_H__F7B541F1_E1E0_11D6_B030_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProductShapeHandlingDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProductShapeHandlingDialog dialog

class CProductShapeHandlingDialog : public CDialog
{
public:
	//variables
	BYTE vSlipsEjectEmptyContainers;  //0 = eject slips that are empty containers, 1 = pass slips
	BYTE vShapeHandlingProcessIfAtSides; // 0 don't, 1 reject, 2 accept
	//BYTE vShapeHandlingProcessIfAtTop; // 0 don't, 1 reject, 2 accept 
	BYTE vShapeHandlingProcessIfWrongWidth;// 0 don't, 1 reject, 2 accept 
	BYTE vShapeHandlingProcessIfWrongHeight;// 0 don't, 1 reject, 2 accept 
	BYTE vShapeHandlingProcessIfConveyorStops;// 0 don't, 1 reject, 2 accept 
	DWORD vSlippedIndicatorTime;
	double vShapeHandlingHeightTolerance;
	double vShapeHandlingWidthTolerance;
	DWORD vShapeHandlingEjectors;
	BYTE vProductSlipPercentValue;

	//CScanTracDlg *vMainWindowPointer;
	bool vChangeMade;

	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

	// Construction
	CProductShapeHandlingDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void SetChangeMade();
	void UpdateDisplay();

// Dialog Data
	//{{AFX_DATA(CProductShapeHandlingDialog)
	enum { IDD = IDD_ProductShapeHandlingDialog };
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
	CStatic m_Function3Display;
	CStatic m_SubFunction1Display;
	CStatic m_SubFunction2Display;
	CStatic m_SubFunction3Display;
	CStatic m_SubFunction4Display;
	CStatic m_SubFunction5Display;
	CStatic	m_SubFunction6Display;
	CStatic m_SubFunction7Display;
	CStatic m_SubFunction8Display;
	CStatic m_DialogTitleStaticText1;
	CStatic m_Function5ContainerWarning;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProductShapeHandlingDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProductShapeHandlingDialog)
	afx_msg void OnFunction1Button();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
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
	afx_msg void OnDialogtitlestatictext1();
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRODUCTSHAPEHANDLINGDIALOG_H__F7B541F1_E1E0_11D6_B030_00500466E305__INCLUDED_)
