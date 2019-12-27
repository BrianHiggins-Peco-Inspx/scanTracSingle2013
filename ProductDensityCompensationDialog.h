//ScanTrac Side View Source File
#if !defined(AFX_PRODUCTDENSITYCOMPENSATIONDIALOG_H__0062D9E1_D7AA_11D6_B025_00500466E305__INCLUDED_)
#define AFX_PRODUCTDENSITYCOMPENSATIONDIALOG_H__0062D9E1_D7AA_11D6_B025_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProductDensityCompensationDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProductDensityCompensationDialog dialog

class CProductDensityCompensationDialog : public CDialog
{
public:
	//variables
	BYTE vUseDACAndADCinDensityCompensation;
	//BYTE vDriftCompensationThresholdReductionMaximum;
	//double vDriftCompensationThresholdReductionFactor;
	WORD vDriftCompensationWaitTime;
	WORD vNumberOfImagesForDriftDensity;
	double vDriftCompensationFastThreshold;
	double vDriftCompensationFastAdjust;
	double vDriftCompensationVeryFastThreshold;
	double vDriftCompensationVeryFastAdjust;
	double vDriftCompensationSlowThreshold;
	double vDriftCompensationSlowAdjust;
	double vDriftCompensationMaximumAdjust;
	BYTE vDriftCompensationEnabled;

	//CScanTracDlg *vMainWindowPointer;
	bool vChangeMade;

	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

	// Construction
	CProductDensityCompensationDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void SetChangeMade();
	void UpdateDisplay();

// Dialog Data
	//{{AFX_DATA(CProductDensityCompensationDialog)
	enum { IDD = IDD_ProductDensityCompensationDialog };

	CStatic m_DialogStaticText1;
	CStatic m_Function3Display;
	CStatic m_Function5Display;
	CStatic m_SubFunction0Button;
	CStatic m_SubFunction0Display;
	CStatic m_Info;
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
	CStatic m_SubFunction1Display;
	CStatic m_SubFunction2Display;
	CStatic m_SubFunction3Display;
	CStatic m_SubFunction4Display;
	CStatic m_SubFunction5Display;
	CStatic m_SubFunction6Display;
	CStatic m_SubFunction7Display;
	CStatic m_SubFunction8Display;
	CStatic	m_Background;
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProductDensityCompensationDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProductDensityCompensationDialog)
	afx_msg void OnFunction1Button();
	afx_msg void OnFunction2Button();
	afx_msg void OnFunction3Button();
	afx_msg void OnFunction4Button();
	afx_msg void OnFunction5Button();
	afx_msg void OnSubFunction0Button();
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
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRODUCTDENSITYCOMPENSATIONDIALOG_H__0062D9E1_D7AA_11D6_B025_00500466E305__INCLUDED_)
