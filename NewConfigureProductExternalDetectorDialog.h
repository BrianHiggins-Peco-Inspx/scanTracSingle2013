//ScanTrac Side View Source File
#if !defined(AFX_NEWCONFIGUREPRODUCTEXTERNALDETECTORDIALOG_H__7E9F9722_F607_11D3_ABD6_00500466E305__INCLUDED_)
#define AFX_NEWCONFIGUREPRODUCTEXTERNALDETECTORDIALOG_H__7E9F9722_F607_11D3_ABD6_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewConfigureProductExternalDetectorDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewConfigureProductExternalDetectorDialog dialog

class CNewConfigureProductExternalDetectorDialog : public CDialog
{
public:
	//variables
	bool vEnableMode1;
	bool vEnableMode2;
	bool vFunction5ButtonEnable;
	bool vInSideTrip;
	BYTE vNumberEjectorsConfigured;
	BYTE vOnlyEjectorNumberConfigured;
	bool vInAutoSetup;
	bool vChangeMade;
	BYTE vFirstShown;
	double vWindowStart[cNumberOfExternalDetectors];
	double vWindowEnd[cNumberOfExternalDetectors];
	BYTE vEnabled[cNumberOfExternalDetectors];
	BYTE vDetectorMode[cNumberOfExternalDetectors];
	BYTE vOriginalDetectorMode[cNumberOfExternalDetectors];
	DWORD vDetectorEjector[cNumberOfExternalDetectors];
	CString *vProductName;
	CString vCurrentDetectorName;
	BYTE vDetectorNumberEditing;
	CProduct *vLocalCurrentProductPointer;
	CSystemConfigurationData *vLocalConfigurationData;
	CScanTracSystemRunningData *vLocalSystemData;
	CScanTracDlg *vMainWindowPointer;
	unsigned long int vStartSetupTimerHandle;
	unsigned long int vStartTimerHandle;
	unsigned long int vUpdateDisplayTimerHandle;
	bool vOldXRayOnNotice;
	bool vStopSystemOnExit;
	double vOldWindowStartDetector1;
	double vOldWindowStartDetector2;
	double vOldWindowEndDetector1;
	double vOldWindowEndDetector2;
	CString vDiscoverModeTextBoxString;
	int vDiscoverModeTextPosition;
	BYTE vOldDetectorMode1;
	BYTE vOldDetectorMode2;
	DWORD vOldDetectorEjector1;
	DWORD vOldDetectorEjector2;
	tCWndCollection vLocalCWndCollection;
	bool vSubFunction1ButtonEnable;
	bool vSubFunction2ButtonEnable;
	bool vSubFunction5ButtonEnable;
	bool vSubFunction6ButtonEnable;
	bool vSubFunction7ButtonEnable;

	// Construction
	CNewConfigureProductExternalDetectorDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void DisplayEjectorNames();
	void DisplayDetectorNames();
	bool ConfigurationOK(BYTE TempDetector);
	void SetChangeMade();
	void UpdateDisplay();
	void SaveAndExit(int TempReturn);
	void OneSecondUpdate();
	void SelectAuxiliaryDetectorEjectors();
	void UpdateSetupWithNewChanges();
	void MustSelectActiveModeDialog();

// Dialog Data
	//{{AFX_DATA(CNewConfigureProductExternalDetectorDialog)
	enum { IDD = IDD_NewConfigProductExternalDetectorDialog };
	CStatic	m_DiscoverModeTextBox;
	CDownFunctionButton	m_Function3Button;
	CStatic	m_Calibrating;
	CStatic	m_XRaysOn;
	CDownFunctionButton	m_Function5Button;
	CStatic	m_WindowStart2;
	CStatic	m_WindowStart1;
	CStatic	m_WindowEnd2;
	CStatic	m_WindowEnd1;
	CRightFunctionButton	m_SubFunction8Button;
	CRightFunctionButton	m_SubFunction7Button;
	CRightFunctionButton	m_SubFunction6Button;
	CRightFunctionButton	m_SubFunction5Button;
	CLeftFunctionButton	m_SubFunction4Button;
	CLeftFunctionButton	m_SubFunction3Button;
	CLeftFunctionButton	m_SubFunction2Button;
	CLeftFunctionButton	m_SubFunction1Button;
	CStatic	m_DetectorName2;
	CStatic	m_DetectorName1;
	CStatic	m_DetectorMode2;
	CStatic	m_DetectorMode1;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_Background;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CStatic m_Title1;
	CStatic m_Title2;
	CStatic m_Title3;
	CStatic m_Title4;
	CStatic m_Title5;
	CStatic m_Title6;
	CStatic m_Title7;
	CStatic m_Title8;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewConfigureProductExternalDetectorDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewConfigureProductExternalDetectorDialog)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
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
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWCONFIGUREPRODUCTEXTERNALDETECTORDIALOG_H__7E9F9722_F607_11D3_ABD6_00500466E305__INCLUDED_)
