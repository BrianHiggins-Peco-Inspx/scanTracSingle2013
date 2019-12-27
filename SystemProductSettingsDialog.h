//ScanTrac Side View Source File
#include "afxwin.h"
#if !defined(AFX_SYSTEMPRODUCTSETTINGSDIALOG_H__AD725691_AD79_11D7_B0DE_00500466E305__INCLUDED_)
#define AFX_SYSTEMPRODUCTSETTINGSDIALOG_H__AD725691_AD79_11D7_B0DE_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemProductSettingsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSystemProductSettingsDialog dialog

class CSystemProductSettingsDialog : public CDialog
{
public:
	//variables
	BYTE vReferenceContainersSideToSide;
	BYTE vStopAfterAnInspectionRejects;
	bool vProductDensityCompensationLoopParametersAreStandard;
	CScanTracDlg *vMainWindowPointer;
	BYTE vAutoImproveEnabled;
	BYTE vAutoImproveReduceThresholdFactor;
	WORD vAutoImproveNumberToAddToLearn;
	BYTE vQuickLearnDilateTimes;
	BYTE vNormalLearnDilateTimes;
	BYTE vShowSensitivityDisplays;
	bool vOldCurrentProductChanged;
	BYTE vAllowVoidInspection;
	BYTE vRemoveBackgroundFromImage;
	BYTE vReferenceROIsToEdges;
	BYTE vNumberOfLastRejectImages;
	bool vChangeMade;
	BYTE vAdjustAllAreaOfHistogram;
	BYTE vAdjustImageIntensity;
	BYTE vMaxImageIntensityAdjust;
	BYTE vMaxImageIntensityAdjustBeforeLocked;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

// Construction
	CSystemProductSettingsDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void SetChangeMade();
	void UpdateDisplay();
	//9/30/2009
	void CheckProductDensityCompensationLoopParameters();


// Dialog Data
	//{{AFX_DATA(CSystemProductSettingsDialog)
	enum { IDD = IDD_SystemProductSettingsDialog };
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_Background;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function3Button;
	CStatic m_Function3Display;
	CDownFunctionButton m_Function5Button;
	CStatic m_Function5Display;
	CLeftFunctionButton m_SubFunction1Button;
	CStatic m_SubFunction1Display;
	CLeftFunctionButton m_SubFunction2Button;
	CStatic m_SubFunction2Display;
	CLeftFunctionButton m_SubFunction3Button;
	CStatic m_SubFunction3Display;
	CStatic m_SubFunction4Display;
	CLeftFunctionButton m_SubFunction4Button;
	CRightFunctionButton m_SubFunction5Button;
	CStatic m_SubFunction5Display;
	CRightFunctionButton m_SubFunction6Button;
	CStatic m_SubFunction6Display;
	CRightFunctionButton m_SubFunction7Button;
	CStatic m_SubFunction7Display;
	CRightFunctionButton m_SubFunction8Button;
	CStatic m_SubFunction8Display;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemProductSettingsDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSystemProductSettingsDialog)
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
	afx_msg void OnStnClickedSubfunction9button();
	CStatic m_SubFunction9Button;
	CStatic m_SubFunction9Display;
	afx_msg void OnStnClickedSubfunction0button();
	CStatic m_SubFunction0Button;
	CStatic m_SubFunction0Display;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMPRODUCTSETTINGSDIALOG_H__AD725691_AD79_11D7_B0DE_00500466E305__INCLUDED_)
