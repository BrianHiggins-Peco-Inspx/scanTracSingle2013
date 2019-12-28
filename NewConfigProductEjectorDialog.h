//ScanTrac Side View Source File
#if !defined(AFX_NEWCONFIGPRODUCTEJECTORDIALOG_H__7E9F9720_F607_11D3_ABD6_00500466E305__INCLUDED_)
#define AFX_NEWCONFIGPRODUCTEJECTORDIALOG_H__7E9F9720_F607_11D3_ABD6_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewConfigProductEjectorDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewConfigProductEjectorDialog dialog

class CNewConfigProductEjectorDialog : public CDialog
{
public:
	//variables
	BYTE vNumberOfEjectorsInSystem;
	BYTE vFirstEjectorShown;
	bool vEjectorUsed[cNumberOfEjectors];
	bool vShowNextButton;
	bool vShowPreviousAndNextButtons;
	double vMinimumEjectorDistance;
	bool vStopRunningWhenExit;
	bool vDisableEjectorsWhenExit;
	bool vInSideTrip;
	CString vOldErrorMessage;
	bool vOldButton3Yellow;
	tProductCollection *vLocalProductCollection;
	bool vOldSourceRampedUp;
	CScanTracDlg *vMainWindowPointer;
	unsigned long int vDisplayTimerHandle;
	unsigned long int vStartSetupTimerHandle;
	unsigned long int vStart16XEncoderTimerHandle;
	bool vWaitingForTestEjectNextContainer;
	bool vTestingEject2ndContainer;
	bool vChangeMade;
	double vEjectorDistanceFromTriggerInInches[cNumberOfEjectors];
	double vResyncTriggerToEjectTime[cNumberOfEjectors];
	float vOriginalResyncTriggerToEjectTime[cNumberOfEjectors];
	float vEjectorResponseTime[cNumberOfEjectors];
	CString vDwellTime[cNumberOfEjectors];
	CString vOriginalDelayPosition[cNumberOfEjectors];
	float vOriginalEjectorResponseTime[cNumberOfEjectors];
	CString vOriginalDwellTime[cNumberOfEjectors];
	CString *vProductName;
	CString vCurrentEjectorName;
	BYTE vEjectorNumberEditing;
	CSystemConfigurationData *vLocalConfigurationData;
	CScanTracSystemRunningData *vLocalSystemData;
	bool vOldXRayOnNotice;
	tCWndCollection vLocalCWndCollection;
	bool vFunction2ButtonEnable;
	bool vFunction5ButtonEnable;
	bool vSubFunction1ButtonEnable;
	bool vSubFunction2ButtonEnable;
	bool vSubFunction3ButtonEnable;
	bool vSubFunction4ButtonEnable;
	bool vSubFunction5ButtonEnable;

	// Construction
	CNewConfigProductEjectorDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void UpdateDisplay();
	void DisplayEjectorNames();
	void SetChangeMade();
	void UpdateButtons();
	void UpdateDisplayFromTimer();
	void PrepareToExitDialog();
	void SaveAndExit(int TempReturnCode);
	void HandleErrorButton();
	void ClearYellowWarningButton();
	void UpdateFunction3Button();
	void TestEject4InARow();
	void TestEjectNextContainer(BYTE TempWaitCount);
	void GiveEjectorNotUsedMessage();
	void ShowNoMouseClickDialogMessage();

// Dialog Data
	//{{AFX_DATA(CNewConfigProductEjectorDialog)
	enum { IDD = IDD_NewConfigProductEjectorDialog };
	CRightFunctionButton	m_SubFunction8Button;
	CStatic	m_SubFunction8Display;
	CStatic	m_DwellTime2;
	CDownFunctionButton	m_Function5Button;
	CDownFunctionButton	m_Function3Button;
	CStatic	m_XRaysOn;
	CStatic	m_Calibrating;
	CRightFunctionButton	m_SubFunction5Button;
	CLeftFunctionButton	m_SubFunction4Button;
	CLeftFunctionButton	m_SubFunction3Button;
	CLeftFunctionButton	m_SubFunction2Button;
	CLeftFunctionButton	m_SubFunction1Button;
	CStatic	m_EjectorName3;
	CStatic	m_EjectorName2;
	CStatic	m_EjectorName1;
	CStatic	m_DwellTime3;
	CStatic	m_DwellTime1;
	CStatic	m_DelayPosition3;
	CStatic	m_DelayPosition2;
	CStatic	m_DelayPosition1;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_Background;
	CStatic m_DialogTitleStaticText;
	CStatic m_DistanceUnitsLabel;
	CStatic m_DistanceUnitsLabel2;
	CStatic m_ErrorLine;
	CDownFunctionButton m_Function1Button;
	CRightFunctionButton m_SubFunction6Button;
	CRightFunctionButton m_SubFunction7Button;
	CStatic m_Title1;
	CStatic m_Title2;
	CStatic m_Title4;
	CStatic m_Title5;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewConfigProductEjectorDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewConfigProductEjectorDialog)
	afx_msg void OnFunction1Button();
	afx_msg void OnFunction2Button();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnFunction3Button();
	afx_msg void OnFunction4Button();
	afx_msg void OnFunction5Button();
	afx_msg void OnSubFunction2Button();
	afx_msg void OnSubFunction3Button();
	afx_msg void OnSubFunction4Button();
	afx_msg void OnSubFunction6Button();
	afx_msg void OnSubFunction7Button();
	afx_msg void OnSubFunction8Button();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSubFunction1Button();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSubFunction5Button();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWCONFIGPRODUCTEJECTORDIALOG_H__7E9F9720_F607_11D3_ABD6_00500466E305__INCLUDED_)
