//ScanTrac Side View Source File
#if !defined(AFX_FBWF_INFO_H__BF9D794D_BB26_42B8_9FB2_61D4B072010D__INCLUDED_)
#define AFX_FBWF_INFO_H__BF9D794D_BB26_42B8_9FB2_61D4B072010D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FBWF_Info.h : header file
//
	ULONG CommitNowFBWF(CString cSaveDirectory, CString cFileName);

/////////////////////////////////////////////////////////////////////////////
// FBWF_Info dialog

class FBWF_Info : public CDialog
{
	bool vSubFunction1ButtonEnable;
	bool vSubFunction2ButtonEnable;
	bool vSubFunction3ButtonEnable;
	bool vSubFunction4ButtonEnable;
	bool vSubFunction5ButtonEnable;
	bool vSubFunction6ButtonEnable;

// Construction
public:
	FBWF_Info(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	CScanTracDlg *vMainWindowPointer;
	CScanTracSystemRunningData *vLocalSystemData;
	tProductCollection *vLocalProductCollection;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

//functions with library dlls for FBWF info
	ULONG IsFBWFEnabled(unsigned long * currentSession,unsigned long * nextSession);
	ULONG MemoryUsageFBWF(unsigned long * DirStructureBytes,unsigned long * FileDataBytes,unsigned long * CurrentCacheBytes,unsigned long * NextCacheBytes);
	ULONG EnableFBWF(void);
	ULONG DisableFBWF(void);
	ULONG SetNextCacheFBWF(ULONG NextCacheInKBytes);
	void UpdateDisplay(void);
	void GetFBWFLogData(CString TempFileType);			//gets all files for today and compiles the strings from all files
	void ShowFunctionNotValidDialogMessage(void);

	unsigned int vFBWFTimerHandle;
	ULONG vUsedSpace;
	ULONG vTotalSpace;
	ULONG vTotalNextSpace;
	ULONG	vFBWFCurrentEnabled ;
	ULONG vFBWFNextEnabled ;
	int vFbwfEnabledStatusReadok;
	int vFbwfStorageStatusReadok;
//	LPTSTR vOutputTextString;
	CString vOutputTextString;
	CString vFBWFDirectoryToCommit;
	CString vFBWFFileToCommit;
	double TempFBWFHardDriveSpaceFree;

	//{{AFX_DATA(FBWF_Info)
	enum { IDD = IDD_FBWF_InfoDialog };
	CStatic	m_CommitFileLabel;
	CStatic	m_AvailSpaceLabel;
	CStatic	m_UsedSpaceLabel;
	CStatic	m_TotalSpaceLabel;
	CStatic	m_AvailSpace;
	CStatic	m_UsedSpace;
	CStatic	m_TotalSpace;

	CStatic	m_WaitMessageLabel;
	CStatic	m_ChangeOnRebootNotifyLabel;
	CDownFunctionButton	m_Function2Button;
	CDownFunctionButton	m_Function1Button;
	CRightFunctionButton	m_SubFunction6Button;;
	CRightFunctionButton	m_SubFunction5Button;;
	CLeftFunctionButton	m_SubFunction4Button;;
	CLeftFunctionButton	m_SubFunction3Button;;
	CLeftFunctionButton	m_SubFunction2Button;;
	CLeftFunctionButton	m_SubFunction1Button;;
	CStatic	m_Background;
	CStatic m_AvaiableHardDriveNotifyLabel;
	CStatic m_DialogTitleStaticText;
	CStatic m_EnableDisableStateLabel;
	CDownFunctionButton m_Function3Button;
	CDownFunctionButton m_Function4Button;
	CDownFunctionButton m_Function5Button;
	CStatic m_MoreDiskDriveInfo;
	CRightFunctionButton m_SubFunction7Button;
	CRightFunctionButton m_SubFunction8Button;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FBWF_Info)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int DoModal();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	private:
	ULONG CommitFBWF(PWCHAR ptrFileName);
	BOOL vFBWFAvailable;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(FBWF_Info)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnFunction1Button();
	afx_msg void OnFunction2Button();
	afx_msg void OnSubFunction4Button();
	afx_msg void OnSubFunction1Button();
	afx_msg void OnSubFunction3Button();
	afx_msg void OnSubFunction5Button();
	afx_msg void OnSubFunction6Button();
	afx_msg void OnSubFunction7Button();
	afx_msg void OnSubFunction8Button();
	afx_msg void OnSubFunction2Button();
	afx_msg void OnFunction3Button();
	afx_msg void OnFunction4Button();
	afx_msg void OnFunction5Button();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FBWF_INFO_H__BF9D794D_BB26_42B8_9FB2_61D4B072010D__INCLUDED_)
