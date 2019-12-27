//ScanTrac Side View Source File
#if !defined(AFX_HISTOGRAMDISPLAYDIALOG_H__3FEC5391_725B_11D6_AFDE_00500466E305__INCLUDED_)
#define AFX_HISTOGRAMDISPLAYDIALOG_H__3FEC5391_725B_11D6_AFDE_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistogramDisplayDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHistogramDisplayDialog dialog

class CHistogramDisplayDialog : public CDialog
{
public:
	//variables
	CString vTopDataLine;
	double vXScaleCompression;
	tCurveFitAnalysisData vCurveFitAnalysisData;
	CInspection *vCurveFitInspection;
	CString vXText;
	CString vYText;
	bool vShowFittedCurve;
	bool vCreatedDataArray;
	bool vCreatedIntegerArray;
	bool vPlotRawData;
	bool vShowCurveFitDataLine;
	bool vACPowerMoniterFormat;
	bool vXScaleIsTime;
	DWORD vMaximumYToShow;
	DWORD vMinimumYToShow;
	double vClipCounts;
	CString vDataTitle;
	DWORD vMinimumX;
	DWORD vMaximumX;
	DWORD vNumberOfPointsToPlot;
	DWORD vMinimumY;
	DWORD vMaximumY;
	double vMinimumYDouble;
	double vMaximumYDouble;
	double vXScale;
	double vYScale;
	DWORD vXInterval;
	DWORD vYInterval;
	DWORD vXNumberOfScaleLines;
	DWORD vYNumberOfScaleLines;
	DWORD vXScaleWord;
	DWORD *vDataArray;
	BYTE *vCurveFitDataArray;
	
	CPen vBluePen;
	CPen vRedPen;
	CPen vGreyPen;

	DWORD vNumberOfNumbers;
	bool vValuesAreIntegers;
	bool vValuesAreBytes;
	bool vValuesAreFloats;
	DWORD *vPointerToIntegers;
	DWORD *vHistogramData;
	BYTE *vPointerToBytes;
	double *vPointerToFloats;
	DWORD *vPointerToTimes;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	CScanTracDlg *vMainWindowPointer;
	tCWndCollection vLocalCWndCollection;

	// Construction
	CHistogramDisplayDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void DisplayRawDataPlot();
	void DisplayIntegerHistogram();
	void DisplayFloatHistogram();
	void DisplayByteHistogram();
	void ClearWindowText();
	void ShowInspxButtons();
	void ShowCurveFitAnalysisData();
	void ShowData();

// Dialog Data
	//{{AFX_DATA(CHistogramDisplayDialog)
	enum { IDD = IDD_HistogramDisplayDialog };
	CDownFunctionButton	m_Function2Button;
	CDownFunctionButton	m_Function5Button;
	CStatic	m_XScale14;
	CStatic	m_XScale13;
	CStatic	m_XScale12;
	CStatic	m_XScale11;
	CStatic	m_YScale15;
	CStatic	m_YScale14;
	CStatic	m_YScale13;
	CStatic	m_YScale12;
	CStatic	m_YScale11;
	CStatic	m_XScale9;
	CStatic	m_XScale8;
	CStatic	m_XScale6;
	CStatic	m_XScale7;
	CStatic	m_XScale5;
	CStatic	m_XScale4;
	CStatic	m_XScale3;
	CStatic	m_XScale2;
	CStatic	m_XScale10;
	CStatic	m_XScale1;
	CStatic	m_YScale9;
	CStatic	m_YScale8;
	CStatic	m_YScale7;
	CStatic	m_YScale6;
	CStatic	m_YScale5;
	CStatic	m_YScale4;
	CStatic	m_YScale3;
	CStatic	m_YScale2;
	CStatic	m_YScale10;
	CStatic	m_YScale1;
	CStatic	m_TotalSamples;
	CStatic	m_LineLabel4;
	CStatic	m_LineLabel2;
	CStatic	m_XLabel2;
	CStatic	m_XLabel3;
	CStatic	m_XLabel1;
	CStatic	m_LineLabel6;
	CStatic	m_LineLabel3;
	CStatic	m_LineLabel1;
	CStatic m_DataTitle;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function3Button;
	CDownFunctionButton m_Function4Button;
	CFunctionButton m_SubFunction1Button;
	CFunctionButton m_SubFunction2Button;
	CFunctionButton m_SubFunction3Button;
	CFunctionButton m_SubFunction4Button;
	CFunctionButton m_SubFunction5Button;
	CFunctionButton m_SubFunction6Button;
	CFunctionButton m_SubFunction7Button;
	CFunctionButton m_SubFunction8Button;
	CStatic m_XRaysOn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistogramDisplayDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHistogramDisplayDialog)
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
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
	afx_msg void OnStnClickedNextbutton();
	afx_msg void OnStnClickedPreviousbutton();
	CStatic m_NextButton;
	CStatic m_PreviousButton;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTOGRAMDISPLAYDIALOG_H__3FEC5391_725B_11D6_AFDE_00500466E305__INCLUDED_)
