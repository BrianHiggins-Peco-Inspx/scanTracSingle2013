#if !defined(AFX_CONFIGUREDIRECTORYNAMES_H__7FA4A5E1_61B8_11DA_B2DB_00500466E305__INCLUDED_)
#define AFX_CONFIGUREDIRECTORYNAMES_H__7FA4A5E1_61B8_11DA_B2DB_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigureDirectoryNames.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigureDirectoryNames dialog

class CConfigureDirectoryNames : public CDialog
{
public:
	//variables
	BYTE vSaveAllImagesToRejectsFolder;
	CString vScanTracSetupBackupDirectory;
	unsigned int vWaitToExitTimerHandle;
	bool vWaitingToExit;
	bool vChangeMade;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	CScanTracDlg *vMainWindowPointer;

	CString vScanTracProductionReportDirectory;
	CString vScanTracScreenShotDirectory;
	CString vScanTracEventLogDirectory;
	CString vScanTracImagesDirectory;
	CString vScanTracCalibrationImagesDirectory;
	CString vScanTracSerialLogsDirectory;
	CString vScanTracRejectedImagesDirectory;

	BYTE vWriteRejectsToFiles;

	tCWndCollection vLocalCWndCollection;
	// Construction
	CConfigureDirectoryNames(CWnd* pParent = NULL);   // standard constructor

	//methods
	void SetChangeMade();
	void UpdateDisplay();
	void UpdateInspxDisplay();

// Dialog Data
	//{{AFX_DATA(CConfigureDirectoryNames)
	enum { IDD = IDD_ConfigureDirectoryNames };
	CStatic	m_SubFunction4Display;
	CLeftFunctionButton	m_SubFunction4Button;
	CStatic	m_SubFunction3Display;
	CLeftFunctionButton	m_SubFunction3Button;
	CStatic	m_SubFunction2Display;
	CLeftFunctionButton	m_SubFunction2Button;
	CStatic	m_SubFunction1Display;
	CLeftFunctionButton	m_SubFunction1Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_Background;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function3Button;
	CDownFunctionButton m_Function5Button;
	CStatic m_Info2;
	CRightFunctionButton m_SubFunction5Button;
	CStatic m_SubFunction5Display;
	CRightFunctionButton m_SubFunction6Button;
	CStatic m_SubFunction6Display;
	CRightFunctionButton m_SubFunction7Button;
	CStatic m_SubFunction7Display;
	CRightFunctionButton m_SubFunction8Button;
	CStatic m_SubFunction8Display;
	CStatic m_SubFunction8Displayz;
	CStatic m_TextDisplay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigureDirectoryNames)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	bool IsDirectoryDefaultNameFor_C_D_Drive(CString variableDirectory,CString constDirectory);

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigureDirectoryNames)
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGUREDIRECTORYNAMES_H__7FA4A5E1_61B8_11DA_B2DB_00500466E305__INCLUDED_)
