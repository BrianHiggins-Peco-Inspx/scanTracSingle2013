//ScanTrac Side View Source File
#if !defined(AFX_UCONTROLLERSTATUSDIALOG_H__7E627181_3BF3_11D4_AC71_00500466E305__INCLUDED_)
#define AFX_UCONTROLLERSTATUSDIALOG_H__7E627181_3BF3_11D4_AC71_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// uControllerStatusDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CuControllerStatusDialog dialog

class CuControllerStatusDialog : public CDialog
{
public:
	//variables
	int vOriginalACPowerMonitorOffset;
	WORD vPlus15DisplayValue;
	WORD vMinus12DisplayValue;
	WORD vNormalValue;
	WORD vHighValue;
	WORD vLowValue;
	bool vMakeExitButtonGreen;
	bool vChangeMade;
	bool vOldXRayOnNotice;
	BYTE vRunningDisplayedInVolts;
	bool vOldNoCommunicationsWithUController;
	CScanTracDlg *vMainWindowPointer;
	unsigned long int vDisplayTimerHandle;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

	int vOldCurrentACInputVoltage;
	int vOldCurrentACInputVoltageMinimum;
	int vOldCurrentACInputVoltageMaximum;
	WORD vOld5;
	WORD vOld33;
	WORD vOld25;
	WORD vOld12;
	WORD vOldMinus12;
	WORD vOld15;
	WORD vOldMax5;
	WORD vOldMax33;
	WORD vOldMax25;
	WORD vOldMax12;
	WORD vOldMaxMinus12;
	WORD vOldMax15;
	WORD vOldMin5;
	WORD vOldMin33;
	WORD vOldMin25;
	WORD vOldMin12;
	WORD vOldMinMinus12;
	WORD vOldMin15;
	WORD vOlduCWatchDogCount;
	WORD vOlduCResetCount;
	BYTE vOlduCMajorVersionNumber;
	BYTE vOlduCMinorVersionNumber;
	bool vSubFunction7ButtonEnable;
	bool vSubFunction8ButtonEnable;

// Construction
	CuControllerStatusDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void UpdateDisplay();
	void ClearMinMaxVoltages();
	void SetChangeMade();
	void UpdateButtons();
	void ShowInspxButtons();

// Dialog Data
	//{{AFX_DATA(CuControllerStatusDialog)
	enum { IDD = IDD_uControllerStatusDialog };
	CDownFunctionButton	m_Function4Button;
	CStatic	m_Function5Display;
	CDownFunctionButton	m_Function5Button;
	CStatic	m_VoltageError13;
	CRightFunctionButton	m_SubFunction8Button;
	CRightFunctionButton	m_SubFunction7Button;
	CStatic	m_VoltageError9;
	CStatic	m_VoltageError8;
	CStatic	m_VoltageError7;
	CStatic	m_VoltageError6;
	CStatic	m_VoltageError5;
	CStatic	m_VoltageError3;
	CStatic	m_VoltageError4;
	CStatic	m_VoltageError2;
	CStatic	m_VoltageError12;
	CStatic	m_VoltageError11;
	CStatic	m_VoltageError10;
	CStatic	m_VoltageError1;
	CStatic	m_Max120;
	CStatic	m_Min120VAC;
	CStatic	m_Max120VAC;
	CStatic	m_Min120;
	CStatic	m_120VACLabel;
	CStatic	m_120VAC;
	CStatic	m_120;
	CStatic	m_MinV33;
	CStatic	m_MinV25;
	CStatic	m_Min33;
	CStatic	m_Min25;
	CStatic	m_MaxV33;
	CStatic	m_MaxV25;
	CStatic	m_Max33;
	CStatic	m_Max25;
	CStatic	m_33VLabel;
	CStatic	m_33V;
	CStatic	m_25VLabel;
	CStatic	m_25V;
	CStatic	m_25;
	CRightFunctionButton	m_SubFunction6Button;
	CLeftFunctionButton	m_SubFunction2Button;
	CLeftFunctionButton	m_SubFunction1Button;
	CDownFunctionButton	m_Function1Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_Minus12;
	CStatic	m_ReadingLabel;
	CStatic	m_5;
	CStatic	m_33;
	CStatic	m_15;
	CStatic	m_12;
	CStatic	m_XRaysOn;
	CStatic	m_Background;
	CStatic m_VoltageError14;
	CStatic m_12V;
	CStatic m_12VLabel;
	CStatic m_15V;
	CStatic m_15VLabel;
	CStatic m_5V;
	CStatic m_5VLabel;
	CStatic m_CurrentLabel;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function3Button;
	CStatic m_LimitsLabel;
	CStatic m_Max12;
	CStatic m_Max15;
	CStatic m_Max5;
	CStatic m_MaxLabel;
	CStatic m_MaxMinus12;
	CStatic m_MaxV12;
	CStatic m_MaxV15;
	CStatic m_MaxV5;
	CStatic m_MaxVLabel;
	CStatic m_MaxVMinus12;
	CStatic m_Min12;
	CStatic m_Min15;
	CStatic m_Min5;
	CStatic m_MinLabel;
	CStatic m_MinMinus12;
	CStatic m_Minus12V;
	CStatic m_Minus12VLabel;
	CStatic m_MinV12;
	CStatic m_MinV15;
	CStatic m_MinV5;
	CStatic m_MinVLabel;
	CStatic m_MinVMinus12;
	CStatic m_PowerErrorTableTitle;
	CStatic m_RunningLabel;
	CLeftFunctionButton m_SubFunction3Button;
	CLeftFunctionButton m_SubFunction4Button;
	CRightFunctionButton m_SubFunction5Button;
	CStatic m_VoltageLabel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CuControllerStatusDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CuControllerStatusDialog)
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
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTCard(UINT idAction, DWORD dwActionData);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UCONTROLLERSTATUSDIALOG_H__7E627181_3BF3_11D4_AC71_00500466E305__INCLUDED_)
