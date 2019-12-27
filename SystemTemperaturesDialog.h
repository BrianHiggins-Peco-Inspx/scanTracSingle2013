#if !defined(AFX_SYSTEMTEMPERATURESDIALOG_H__3633ECF4_1A3E_497B_8BE6_067D6FDFA9DC__INCLUDED_)
#define AFX_SYSTEMTEMPERATURESDIALOG_H__3633ECF4_1A3E_497B_8BE6_067D6FDFA9DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemTemperaturesDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSystemTemperaturesDialog dialog

class CSystemTemperaturesDialog : public CDialog
{
// Construction
public:
	// variables
	BYTE vMaximumMonoBlockTemperature;
	BYTE vSurfaceTemperatureMovedToDXM;
	bool vChangeMade;
	CSystemTemperaturesDialog(CWnd* pParent = NULL);   // standard constructor
	double vTemperatureSurfaceAdjust;
	double vTemperatureAirInsideAdjust;
	double vTemperatureLiquidAdjust;
	double vTemperatureAirOutsideAdjust;
	double vTemperatureuCAdjust;
	int vMaximumAirTemperature;
	int vMaximumLiquidTemperature;
	int vMaximumSurfaceTemperature;
	int vMaximumTemperatureAirOutside;
	int vMaximumuCTemperature;
	int vMaximumCPUTemperature;
	BYTE vDeltaTemperatureForFilterCloggedError;
	BYTE vDeltaTemperatureForCoolantProblem;
	BYTE vHasOutsideAirTemp;
	bool vSubFunction1ButtonEnable;
	bool vSubFunction3ButtonEnable;
	bool vSubFunction5ButtonEnable;
	bool vSubFunction7ButtonEnable;

	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

	//methods
	void SetChangeMade();
	void UpdateDisplay();
// Dialog Data
	//{{AFX_DATA(CSystemTemperaturesDialog)
	enum { IDD = IDD_SystemTemperaturesDialog };
	CLeftFunctionButton	m_SubFunction3Button;
	CFunctionButton	m_SubFunction3Button3;
	CRightFunctionButton	m_SubFunction5Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_Background;
	CRightFunctionButton	m_SubFunction7Button;
	CLeftFunctionButton	m_SubFunction1Button;
	CStatic m_SubFunction11Button;
	CStatic m_MonoBlockTitle;
	CStatic m_MonoBlockMaximumDisplay;
	CStatic m_MaximumTitle;
	CLeftFunctionButton m_SubFunction4Button;
	CRightFunctionButton m_SubFunction8Button;
	CDownFunctionButton m_Function5Button;
	CStatic m_Function5Display;
	CStatic m_SubFunction4Display;
	CStatic m_SubFunction8Display;
	CStatic m_Adjust;
	CStatic m_CPUMaximum;
	CStatic m_CPUTemps;
	CStatic m_CPUTempMaximum;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function3Button;
	CStatic m_Function3Display;
	CStatic m_Maximum;
	CStatic m_SubFunction10Button;
	CStatic m_SubFunction1Display;
	CLeftFunctionButton m_SubFunction2Button;
	CStatic m_SubFunction2Display;
	CStatic m_SubFunction3Display;
	CStatic m_SubFunction5Display;
	CRightFunctionButton m_SubFunction6Button;
	CStatic m_SubFunction6Display;
	CStatic m_SubFunction7Display;
	CStatic m_SubFunction9Button;
	CStatic m_uCAdjust;
	CStatic m_uCMaximum;
	CStatic m_ucTemps;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemTemperaturesDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSystemTemperaturesDialog)
	afx_msg void OnFunction1Button();
	afx_msg void OnFunction2Button();
	afx_msg void OnFunction3Button();
	afx_msg void OnFunction4Button();
	afx_msg void OnFunction5Button();
	afx_msg void OnSubFunction1Button();
	afx_msg void OnSubFunction2Button();
	afx_msg void OnSubFunction3Button();
	afx_msg void OnSubFunction3Button3();
	afx_msg void OnSubFunction4Button();
	afx_msg void OnSubFunction5Button();
	afx_msg void OnSubFunction6Button();
	afx_msg void OnSubFunction7Button();
	afx_msg void OnSubFunction8Button();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSubFunction9Button();
	afx_msg void OnSubFunction10Button();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSubfunction11button();
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMTEMPERATURESDIALOG_H__3633ECF4_1A3E_497B_8BE6_067D6FDFA9DC__INCLUDED_)
