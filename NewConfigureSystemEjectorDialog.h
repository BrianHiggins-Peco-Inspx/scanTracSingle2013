//ScanTrac Side View Source File
#if !defined(AFX_NEWCONFIGURESYSTEMEJECTORDIALOG_H__8B157162_F9D6_11D3_ABD6_00500466E305__INCLUDED_)
#define AFX_NEWCONFIGURESYSTEMEJECTORDIALOG_H__8B157162_F9D6_11D3_ABD6_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewConfigureSystemEjectorDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewConfigureSystemEjectorDialog dialog

class CNewConfigureSystemEjectorDialog : public CDialog
{
public:
	//variables
	bool vChangeMade;
	CString vName[cNumberOfEjectors];
	CString vDigitalInputLine[cNumberOfEjectors];
	CString vConfirmDigitalInputLine[cNumberOfEjectors];
	BYTE vEnabled[cNumberOfEjectors];
	BYTE vActiveHigh[cNumberOfEjectors];
	BYTE vConfirmActiveHigh[cNumberOfEjectors];
	BYTE vEjectorNumberEditing;
	BYTE vUseXofYAlarmOutputForUnconfirmedEject;
	BYTE vFirstEjectorShown;
	HBRUSH vLightGreenBrush;
	HBRUSH vGreenBrush;
	HBRUSH vYellowBrush;
	HBRUSH vWhiteBrush;
	HBRUSH vBlueBrush;
	CSystemConfigurationData *vLocalConfigurationData;
	CScanTracSystemRunningData *vLocalSystemData;
	tCWndCollection vLocalCWndCollection;
	CScanTracDlg *vMainWindowPointer;
	bool vFunction3ButtonEnable;
	bool vFunction5ButtonEnable;

	bool vSubFunction2ButtonEnable;
	bool vSubFunction3ButtonEnable;
	bool vSubFunction4ButtonEnable;
	bool vSubFunction5ButtonEnable;
	bool vSubFunction6ButtonEnable;
	bool vSubFunction7ButtonEnable;

	// Construction
	CNewConfigureSystemEjectorDialog(CWnd* pParent = NULL);   // standard constructor

	//destructor
	virtual ~CNewConfigureSystemEjectorDialog();

	//methods
	void DisplaySubFunction1To4();
	void UpdateDisplay();
	bool ConfigurationOK(BYTE TempDetector);
	void SetChangeMade();
	void ShowEjectorEditNotEnabledDialogMessage(void);
	void ShowEjectorAlreadySelectedDialogMessage(void); 
	void ShowFunctionNotValidDialogMessage(void);

// Dialog Data
	//{{AFX_DATA(CNewConfigureSystemEjectorDialog)
	enum { IDD = IDD_NewConfigureSystemEjectorDialog };
	CDownFunctionButton	m_Function5Button;
	CDownFunctionButton	m_Function3Button;
	CRightFunctionButton	m_SubFunction8Button;
	CRightFunctionButton	m_SubFunction7Button;
	CRightFunctionButton	m_SubFunction6Button;
	CRightFunctionButton	m_SubFunction5Button;
	CLeftFunctionButton	m_SubFunction4Button;
	CLeftFunctionButton	m_SubFunction1Button;
	CLeftFunctionButton	m_SubFunction3Button;
	CLeftFunctionButton	m_SubFunction2Button;
	CStatic	m_EjectorName3;
	CStatic	m_EjectorName2;
	CStatic	m_EjectorName1;
	CStatic	m_DigitalInputLine3;
	CStatic	m_DigitalInputLine2;
	CStatic	m_DigitalInputLine1;
	CStatic	m_ConfirmDigitalInputLine3;
	CStatic	m_ConfirmDigitalInputLine2;
	CStatic	m_ConfirmDigitalInputLine1;
	CStatic	m_ConfirmActiveHigh3;
	CStatic	m_ConfirmActiveHigh2;
	CStatic	m_ConfirmActiveHigh1;
	CStatic	m_ActiveHigh3;
	CStatic	m_ActiveHigh2;
	CStatic	m_ActiveHigh1;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_Background;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CStatic m_Note;
	CStatic m_Title1;
	CStatic m_Title10;
	CStatic m_Title6;
	CStatic m_Title7;
	CStatic m_Title8;
	CStatic m_Title9;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewConfigureSystemEjectorDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewConfigureSystemEjectorDialog)
	afx_msg void OnFunction1Button();
	afx_msg void OnFunction2Button();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
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

#endif // !defined(AFX_NEWCONFIGURESYSTEMEJECTORDIALOG_H__8B157162_F9D6_11D3_ABD6_00500466E305__INCLUDED_)
