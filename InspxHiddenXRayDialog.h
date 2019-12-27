//ScanTrac Side View Source File
#if !defined(AFX_INSPXHIDDENXRAYDIALOG_H__46F9D211_F32C_11D5_AF22_00500466E305__INCLUDED_)
#define AFX_INSPXHIDDENXRAYDIALOG_H__46F9D211_F32C_11D5_AF22_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InspxHiddenXRayDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInspxHiddenXRayDialog dialog

class CInspxHiddenXRayDialog : public CDialog
{
public:
	// variables
	BYTE vUse101VACFor10SecondsShutDownLimit;
	bool vChangeMade;
//	double vTemperatureSurfaceAdjust;
//	double vTemperatureAirInsideAdjust;
//	double vTemperatureLiquidAdjust;
//	double vTemperatureAirOutsideAdjust;
//	int vMaximumAirTemperature;
//	int vMaximumLiquidTemperature;
//	int vMaximumSurfaceTemperature;
//	int vMaximumTemperatureAirOutside;
	//double vVoltageControlAdjustOffset;
	//double vCurrentControlAdjustOffset;
	//double vVoltageControlAdjustGain;
	//double vCurrentControlAdjustGain;
	double vMaximumVoltage;
	double vMaximumCurrent;
	double vMaximumPower;
	double vVoltageAdjustOffset;
	double vCurrentAdjustOffset;
	double vVoltageAdjustGain;
	double vCurrentAdjustGain;
	BYTE vIgnoreCheyneyInterlockStatus;


	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

	// Construction
	CInspxHiddenXRayDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void SetChangeMade();
	void UpdateDisplay();

// Dialog Data
	//{{AFX_DATA(CInspxHiddenXRayDialog)
	enum { IDD = IDD_InspxHiddenXRayDialog };
	CStatic	m_SubFunction6Display;
	CRightFunctionButton	m_SubFunction6Button;
	CRightFunctionButton	m_SubFunction5Button;
	CStatic	m_SubFunction5Display;
	CStatic	m_SubFunction3Display;
	CStatic	m_SubFunction2Display;
	CStatic	m_SubFunction1Display;
	CStatic	m_Background;
	CDownFunctionButton	m_Function2Button;
	CDownFunctionButton	m_Function4Button;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function3Button;
	CStatic m_Function3Display;
	CDownFunctionButton m_Function5Button;
	CStatic m_Functio5Display;
	CLeftFunctionButton m_SubFunction1Button;
	CLeftFunctionButton m_SubFunction2Button;
	CLeftFunctionButton m_SubFunction3Button;
	CLeftFunctionButton m_SubFunction4Button;
	CStatic m_SubFunction4Display;
	CRightFunctionButton m_SubFunction7Button;
	CStatic m_SubFunction7Display;
	CRightFunctionButton m_SubFunction8Button;
	CStatic m_SubFunction8Display;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInspxHiddenXRayDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInspxHiddenXRayDialog)
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
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSPXHIDDENXRAYDIALOG_H__46F9D211_F32C_11D5_AF22_00500466E305__INCLUDED_)
