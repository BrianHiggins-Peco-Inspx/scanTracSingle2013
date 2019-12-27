#if !defined(AFX_DISPLAYSAVEDREJECTS_H__1CBCB990_3B56_4113_BA94_395DF153A4EB__INCLUDED_)
#define AFX_DISPLAYSAVEDREJECTS_H__1CBCB990_3B56_4113_BA94_395DF153A4EB__INCLUDED_
#define InScanTrac

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DisplaySavedRejects.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDisplaySavedRejects dialog

class CDisplaySavedRejects : public CDialog
{
// Construction
public:

#ifdef InScanTrac
	unsigned long int vDisplayTimerHandle;
	unsigned long int vGrabFocusTimerHandle;
	unsigned long int vGrabFocusAgainTimerHandle;
	bool vGreyPreviousImageButton;
	bool vGreyNextImageButton;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
#else
	HBRUSH vButtonColorBrush;
#endif
	
	bool vShowBox;
	bool vImageHasBox;
	bool vFoundDateFolders;
	int vDateOfImage;
	int vTimeOfImage;
	int vRejectImageX;
	int vRejectImageY;
	CString vProductName;
	CString vImageTimeString;
	CString vImageDateString;
	CString vCurrentImageFileName;
	CString vScanTracRejectedImagesDirectory;
	BYTE *vRejectImage;
	tCWndCollection vLocalCWndCollection;
	bool vFunction3ButtonEnable;
	bool vFunction4ButtonEnable;
	bool vFunction5ButtonEnable;
	bool vSubFunction5ButtonEnable;
	bool vSubFunction6ButtonEnable;
	bool vSubFunction8ButtonEnable;
	
	
	CDisplaySavedRejects(CWnd* pParent = NULL);   // standard constructor
	void FindPreviousImage();
	void FindNextImage();
	int PreviousImageExists();
	int NextImageExists();
	void DisplayImage();
	void ReDrawScreen();

#ifdef InScanTrac
	void UpdateDisplay();
#else
	void ChatWindow();

	CString GetProductName(CString TempFileName);
	bool WasRejectDisabled(CString TempFileName);
	CString GetInspectionName(CString TempFileName);
	CString GetDateFromFileName(CString TempFileName);
	CString GetTimeFromFileName(CString TempFileName);
	CString GetDashNumberFromFileName(CString TempFileName);
	CString FindPreviousFolder(); //finds folder closest to current, if no previous finds closest next
	CString FindNextFolder(); //finds folder closest to current, if no previous finds closest next
#endif
	void FunctionNotAvailable(void);
	// Dialog Data
	//{{AFX_DATA(CDisplaySavedRejects)
	enum { IDD = IDD_DisplaySavedRejects };
	CDownFunctionButton	m_Function2Button;
	CRightFunctionButton	m_SubFunction6Button;
	CRightFunctionButton	m_SubFunction5Button;
	CDownFunctionButton	m_Function5Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function3Button;
	CStatic	m_EjectorDisabledDisplay;
	CRightFunctionButton	m_SubFunction8Button;
	CStatic m_Background;
	CStatic m_Clock;
	CStatic m_DateDisplay;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CStatic m_FileNameLine;
	CStatic m_InspectionNameDisplay;
	CStatic m_ProductNameDisplay;
	CFunctionButton m_SubFunction7Button;
	CStatic m_TimeDisplay;
	//}}AFX_DATA

		//destructor
	virtual ~CDisplaySavedRejects();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDisplaySavedRejects)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDisplaySavedRejects)
	afx_msg void OnFunction1Button();
	afx_msg void OnFunction2Button();
	afx_msg void OnFunction3Button();
	afx_msg void OnFunction4Button();
	afx_msg void OnFunction5Button();
	afx_msg void OnSubFunction5Button();
	afx_msg void OnSubFunction6Button();
	afx_msg void OnSubFunction7Button();
	afx_msg void OnSubFunction8Button();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
#ifdef InScanTrac
	afx_msg void OnTimer(UINT nIDEvent);
#endif
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISPLAYSAVEDREJECTS_H__1CBCB990_3B56_4113_BA94_395DF153A4EB__INCLUDED_)
