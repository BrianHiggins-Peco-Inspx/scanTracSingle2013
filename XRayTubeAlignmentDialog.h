//ScanTrac Side View Source File
#if !defined(AFX_XRAYTUBEALIGNMENTDIALOG_H__9194B301_BA85_11DA_B322_00500466E305__INCLUDED_)
#define AFX_XRAYTUBEALIGNMENTDIALOG_H__9194B301_BA85_11DA_B322_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XRayTubeAlignmentDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXRayTubeAlignmentDialog dialog

class CXRayTubeAlignmentDialog : public CDialog
{
public:
	BYTE vOldSystemRunMode;
	double vOldRampXRayVoltageAmount;
	double vOldRampXRayCurrentAmount;
	bool vHaveAnImage;
	unsigned long int vUpdateButtonTimerHandle;
	unsigned long int vGrabFocusTimerHandle;
	bool vSavedData;
	CPen vDarkBluePen;
	CPen vBluePen;
	int vLeftSide;
	int vRightSide;
	int vTop; 
	int vBottom;
	int vHeight;
	int vWidth;
	int vTraceHeight;
	bool vOldXRayOnNotice;
	BYTE vOriginalPreAmplifierGain;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	CScanTracDlg *vMainWindowPointer;
	CProduct *vOriginalCurrentProduct;
	tCWndCollection vLocalCWndCollection;

// Construction
	CXRayTubeAlignmentDialog(CWnd* pParent = NULL);   // standard constructor

	//methods or functions
	void PrepareToExit();
	void UpdateButtons();
	void UpdateDisplay();
	bool GetName(CString *TempName);
	bool GetReason(CString *TempReason);
	void ShowInspxItems();

// Dialog Data
	//{{AFX_DATA(CXRayTubeAlignmentDialog)
	enum { IDD = IDD_XRayTubeAlignmentDialog };
	CStatic	m_BackLabel;
	CStatic	m_MaximumDetectorAverage9;
	CStatic	m_MaximumDetectorAverage8;
	CStatic	m_MaximumDetectorAverage7;
	CStatic	m_MaximumDetectorAverage12;
	CStatic	m_MaximumDetectorAverage11;
	CStatic	m_MaximumDetectorAverage10;
	CStatic	m_CurrentDetectorAverage9;
	CStatic	m_CurrentDetectorAverage8;
	CStatic	m_CurrentDetectorAverage7;
	CStatic	m_CurrentDetectorAverage12;
	CStatic	m_CurrentDetectorAverage11;
	CStatic	m_CurrentDetectorAverage10;
	CStatic	m_Detector9;
	CStatic	m_Detector8;
	CStatic	m_Detector7;
	CStatic	m_Detector12;
	CStatic	m_Detector11;
	CStatic	m_Detector10;
	CStatic	m_MaximumDetectorAverage6;
	CStatic	m_MaximumDetectorAverage5;
	CStatic	m_Detector6;
	CStatic	m_Detector5;
	CStatic	m_CurrentDetectorAverage5;
	CStatic	m_CurrentDetectorAverage6;
	CStatic	m_VoltageButton;
	CStatic	m_CurrentButton;
	CRightFunctionButton	m_SubFunction8Button;
	CStatic	m_SubFunction4Display;
	CDownFunctionButton	m_Function5Button;
	CDownFunctionButton	m_Function4Button;
	CLeftFunctionButton	m_SubFunction4Button;
	CStatic	m_Calibrating;
	CStatic	m_XRaysOn;
	CStatic m_FrontLabel;
	CStatic m_Maximum;
	CStatic m_Current;
	CStatic m_Black;
	CStatic m_CurrentDetectorAverage1;
	CStatic m_CurrentDetectorAverage2;
	CStatic m_CurrentDetectorAverage3;
	CStatic m_CurrentDetectorAverage4;
	CStatic m_Detector1;
	CStatic m_Detector2;
	CStatic m_Detector3;
	CStatic m_Detector4;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function2Button;
	CDownFunctionButton m_Function3Button;
	CStatic m_Function6Button;
	CStatic m_Function6Display;
	CStatic m_MaximumDetectorAverage1;
	CStatic m_MaximumDetectorAverage2;
	CStatic m_MaximumDetectorAverage3;
	CStatic m_MaximumDetectorAverage4;
	CStatic m_SubFunction8DisplayA;
	CStatic m_White;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXRayTubeAlignmentDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXRayTubeAlignmentDialog)
	afx_msg void OnFunction1Button();
	afx_msg void OnFunction2Button();
	afx_msg void OnFunction3Button();
	afx_msg void OnFunction4Button();
	afx_msg void OnFunction5Button();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSubFunction4Button();
	afx_msg void OnPaint();
	afx_msg void OnSubFunction8Button();
	afx_msg void OnFunction6Button();
	afx_msg void OnCurrentButton();
	afx_msg void OnVoltageButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XRAYTUBEALIGNMENTDIALOG_H__9194B301_BA85_11DA_B322_00500466E305__INCLUDED_)
