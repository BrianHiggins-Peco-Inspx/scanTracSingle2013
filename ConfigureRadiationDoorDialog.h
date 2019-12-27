//ScanTrac Side View Source File
#if !defined(AFX_CONFIGURERADIATIONDOORDIALOG_H__C20D2601_4433_11D7_B079_00500466E305__INCLUDED_)
#define AFX_CONFIGURERADIATIONDOORDIALOG_H__C20D2601_4433_11D7_B079_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigureRadiationDoorDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigureRadiationDoorDialog dialog

class CConfigureRadiationDoorDialog : public CDialog
{
public:
	//variables
	WORD vReCalibrateAfterStopTimePeriod;
	BYTE vRadiationShutterInSystem;
	bool vChangeMade;
	BYTE vRadiationDoorInvertDoorOutput;
	//WORD vRadiationDoorMaximumDistance;
	WORD vRadiationDoorDistanceStayOpen;
	WORD vRadiationDoorInDoorDistance;

	CScanTracDlg *vMainWindowPointer;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

	// Construction
	CConfigureRadiationDoorDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void SetChangeMade();
	void UpdateDisplay();
	void UpdateButtons();
	void PrepareToExit();

// Dialog Data
	//{{AFX_DATA(CConfigureRadiationDoorDialog)
	enum { IDD = IDD_ConfigureRadiationDoorsDialog };
	CStatic	m_SubFunction1Display;
	CLeftFunctionButton	m_SubFunction1Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_Background;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function3Button;
	CStatic m_Function3Display;
	CDownFunctionButton m_Function5Button;
	CStatic m_Function5Display;
	CLeftFunctionButton m_SubFunction2Button;
	CStatic m_SubFunction2Display;
	CLeftFunctionButton m_SubFunction3Button;
	CStatic m_SubFunction3Display;
	CLeftFunctionButton m_SubFunction4Button;
	CStatic m_SubFunction4Display;
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
	//{{AFX_VIRTUAL(CConfigureRadiationDoorDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigureRadiationDoorDialog)
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
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGURERADIATIONDOORDIALOG_H__C20D2601_4433_11D7_B079_00500466E305__INCLUDED_)
