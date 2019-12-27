//ScanTrac Side View Source File
#if !defined(AFX_CONFIGUREEDGEPIXELADJUSTDIALOG_H__976CAF41_A52F_11D4_ACDC_00500466E305__INCLUDED_)
#define AFX_CONFIGUREEDGEPIXELADJUSTDIALOG_H__976CAF41_A52F_11D4_ACDC_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigureEdgePixelAdjustDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigureEdgePixelAdjustDialog dialog

class CConfigureEdgePixelAdjustDialog : public CDialog
{
public:
	//variables
	BYTE vUsePoint4mmXScanDetectors;
	BYTE vUsePoint4mmXScanDetectorsMultiplier;
	WORD vNumberOfPixelAtBottomToIgnoreInCalibration;
	BYTE vNumberOfDetectorsToCalibrate;
	BYTE vCorrectDetectorEdges;
	WORD vFirst;
	bool vChangeMade;
	WORD vNumberOfPixelsToAdjust;
	double vPixelM[cNumberOfPixelsToAdjustAllPixels1152];
	double vPixelB[cNumberOfPixelsToAdjustAllPixels1152];
	int vOverWriteBadPixel[cNumberOfPixelsToAdjustAllPixels1152];
	CString vPixelNumberString[cNumberOfPixelsToAdjustAllPixels1152];

	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

	// Construction
	CConfigureEdgePixelAdjustDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void SetChangeMade();
	void AdjustPixel(WORD TempPixelNumber);
	void ShowAdjustPixelValues();
	void DisplayButtonLabels();

// Dialog Data
	//{{AFX_DATA(CConfigureEdgePixelAdjustDialog)
	enum { IDD = IDD_NewConfigureEdgePixelAdjustDialog };
	CStatic	m_Function3Display;
	CDownFunctionButton	m_Function5Button;
	CDownFunctionButton	m_Function3Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_Background;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CStatic m_Pixel1;
	CStatic m_Pixel2;
	CStatic m_Pixel3;
	CStatic m_Pixel4;
	CStatic m_Pixel5;
	CStatic m_Pixel6;
	CStatic m_Pixel7;
	CStatic m_Pixel8;
	CLeftFunctionButton m_SubFunction1Button;
	CLeftFunctionButton m_SubFunction2Button;
	CLeftFunctionButton m_SubFunction3Button;
	CLeftFunctionButton m_SubFunction4Button;
	CRightFunctionButton m_SubFunction5Button;
	CRightFunctionButton m_SubFunction6Button;
	CRightFunctionButton m_SubFunction7Button;
	CRightFunctionButton m_SubFunction8Button;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigureEdgePixelAdjustDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigureEdgePixelAdjustDialog)
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
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGUREEDGEPIXELADJUSTDIALOG_H__976CAF41_A52F_11D4_ACDC_00500466E305__INCLUDED_)
