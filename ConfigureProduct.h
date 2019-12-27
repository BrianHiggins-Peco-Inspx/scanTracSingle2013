//ScanTrac Side View Source File
#if !defined(AFX_CONFIGUREPRODUCT_H__54309CC3_D2A5_11D3_ABCE_00500466E305__INCLUDED_)
#define AFX_CONFIGUREPRODUCT_H__54309CC3_D2A5_11D3_ABCE_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigureProduct.h : header file
//
#include "ScanTracDlg.h"
#include "SystemConfigurationData.h"

/////////////////////////////////////////////////////////////////////////////
// CConfigureProduct dialog

class CConfigureProduct : public CDialog
{
public:
	//variables
	CString vOldCurrentUser;
	bool vChangeMade;
	CScanTracDlg *vMainWindowPointer;
	CScanTracSystemRunningData *vLocalSystemData;
	tProductCollection *vLocalProductCollection;
	CSystemConfigurationData *vLocalConfigurationData;
  unsigned int vOneSecondTimerHandle;
	BYTE vOldSystemRunMode;
	tCWndCollection vLocalCWndCollection;
	bool vFunction4ButtonEnable;
	bool vSubFunction6ButtonEnable;
	bool vSubFunction7ButtonEnable;
	bool vSubFunction8ButtonEnable;
	
// Construction
	CConfigureProduct(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CConfigureProduct)

	//methods
	CProduct *GetProductByNumber(int TempProductNumber);
	void AddProduct(CProduct *TempProduct);
	void SelectAndEdit(int TempWindowTypeMode);
	//void UpdateNumberOfProductsDisplay();
	//void UpdateCurrentProductOnDisplay(); 
	void EditTheProduct(CProduct *TempProductPointer, int TempWindowTypeMode);
	void UpdateButtons();
	bool HardDriveProtectionCanBeEnabled();
	ULONG EnableFBWF(void);
	ULONG IsFBWFEnabled(unsigned long * currentSession,unsigned long * nextSession);

// Dialog Data
	//{{AFX_DATA(CConfigureProduct)
	enum { IDD = IDD_ConfigureProductDialog };
	CLeftFunctionButton	m_SubFunction2Button;
//	CStatic	m_ArrowLeft2;
	CRightFunctionButton	m_SubFunction5Button;
	CRightFunctionButton	m_SubFunction8Button;
	CRightFunctionButton	m_SubFunction7Button;
	CRightFunctionButton	m_SubFunction6Button;
	CDownFunctionButton	m_Function3Button;
//	CStatic	m_ArrowDown3;
	CLeftFunctionButton	m_SubFunction3Button;
//	CStatic	m_ArrowLeft3;
	CLeftFunctionButton	m_SubFunction4Button;
//	CStatic	m_ArrowLeft4;
//	CStatic	m_ArrowDown4;
	CDownFunctionButton	m_Function4Button;
	CStatic	m_Background;
	CStatic m_UserLabel;
	CStatic m_CPProductLabel;
	CDownFunctionButton m_Function1Button;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function2Button;
	CDownFunctionButton m_Function5Button;
	CLeftFunctionButton m_SubFunction1Button;
	CStatic m_CurrentUserLabel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigureProduct)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigureProduct)
	afx_msg void OnFunction1Button();
	afx_msg void OnFunction2Button();
	afx_msg void OnFunction4Button();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnFunction3Button();
	afx_msg void OnFunction5Button();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSubFunction1Button();
	afx_msg void OnSubFunction5Button();
	afx_msg void OnSubFunction2Button();
	afx_msg void OnSubFunction7Button();
	afx_msg void OnSubFunction8Button();
	afx_msg void OnSubFunction6Button();
	afx_msg void OnSubFunction4Button();
	afx_msg void OnSubFunction3Button();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void OneSecondUpdate();
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGUREPRODUCT_H__54309CC3_D2A5_11D3_ABCE_00500466E305__INCLUDED_)
