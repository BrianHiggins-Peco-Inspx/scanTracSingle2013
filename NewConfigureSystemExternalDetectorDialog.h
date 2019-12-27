//ScanTrac Side View Source File
#if !defined(AFX_NEWCONFIGURESYSTEMEXTERNALDETECTORDIALOG_H__8B157161_F9D6_11D3_ABD6_00500466E305__INCLUDED_)
#define AFX_NEWCONFIGURESYSTEMEXTERNALDETECTORDIALOG_H__8B157161_F9D6_11D3_ABD6_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewConfigureSystemExternalDetectorDialog.h : header file
//

enum {cExternalDetectorWindow, cBodyTriggerWindow};
/////////////////////////////////////////////////////////////////////////////
// CNewConfigureSystemExternalDetectorDialog dialog

class CNewConfigureSystemExternalDetectorDialog : public CDialog
{
public:
	//variables
	CScanTracDlg *vMainWindowPointer;
	bool vChangeMade;
	BYTE vFirstShown;
	BYTE vWindowType;
	CString vName[cNumberOfExternalDetectors];
	CString vDigitalInputLine[cNumberOfExternalDetectors];
	BYTE vEnabled[cNumberOfExternalDetectors];
	BYTE vActiveHigh[cNumberOfExternalDetectors];
	BYTE vDetectorNumberEditing;
	CSystemConfigurationData *vLocalConfigurationData;
	CScanTracSystemRunningData *vLocalSystemData;
	tCWndCollection vLocalCWndCollection;
	bool vSubFunction1ButtonEnable;
	bool vSubFunction2ButtonEnable;
	bool vSubFunction3ButtonEnable;
	bool vSubFunction4ButtonEnable;
	bool vSubFunction5ButtonEnable;
	bool vSubFunction6ButtonEnable;
	bool vSubFunction7ButtonEnable;

	// Construction
	CNewConfigureSystemExternalDetectorDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void DisplaySubFunction1To4();
	bool ConfigurationOK(BYTE TempDetector);
	void SetChangeMade();
	void UpdateDisplay();
	void ShowDetectorEditNotEnabledDialog(void);
	void ShowDetectorAlreadySelectedDialogMessage(void);

// Dialog Data
	//{{AFX_DATA(CNewConfigureSystemExternalDetectorDialog)
	enum { IDD = IDD_NewConfigureSystemExternalDetectorDialog };
	CRightFunctionButton	m_SubFunction8Button;
	CRightFunctionButton	m_SubFunction7Button;
	CRightFunctionButton	m_SubFunction6Button;
	CRightFunctionButton	m_SubFunction5Button;
	CLeftFunctionButton	m_SubFunction4Button;
	CLeftFunctionButton	m_SubFunction3Button;
	CLeftFunctionButton	m_SubFunction2Button;
	CLeftFunctionButton	m_SubFunction1Button;
	CStatic	m_DigitalInputLine4;
	CStatic	m_DigitalInputLine3;
	CStatic	m_DigitalInputLine2;
	CStatic	m_DigitalInputLine1;
	CStatic	m_DetectorName4;
	CStatic	m_DetectorName3;
	CStatic	m_DetectorName2;
	CStatic	m_DetectorName1;
	CStatic	m_ActiveHigh4;
	CStatic	m_ActiveHigh3;
	CStatic	m_ActiveHigh2;
	CStatic	m_ActiveHigh1;
	CDownFunctionButton	m_Function5Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_Background;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function3Button;
	CStatic m_Title1;
	CStatic m_Title6;
	CStatic m_Title8;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewConfigureSystemExternalDetectorDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewConfigureSystemExternalDetectorDialog)
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
	afx_msg void OnDialogtitlestatictext7();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWCONFIGURESYSTEMEXTERNALDETECTORDIALOG_H__8B157161_F9D6_11D3_ABD6_00500466E305__INCLUDED_)
