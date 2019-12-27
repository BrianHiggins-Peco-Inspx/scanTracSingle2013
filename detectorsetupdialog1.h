//ScanTrac Side View Source File
#if !defined(AFX_DETECTORSETUPDIALOG_H__FFD92581_C706_11D4_AD08_00500466E305__INCLUDED_)
#define AFX_DETECTORSETUPDIALOG_H__FFD92581_C706_11D4_AD08_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetectorSetupDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetectorSetupDialog dialog

class CDetectorSetupDialog : public CDialog
{
public:
	//variables
	BYTE vUseXScanDetectors;
	BYTE vPixelsPerDetector;
	BYTE vDetectorLength;
	bool vChangedDetectorSettings;
	BYTE vPreAmplifierGain;
	unsigned long int vUpdateButtonTimerHandle;
	bool vOldCalibrating;
	//bool vOldMeasureSourceStrength;
	bool vChangeMade;
	bool vOldXRayOnNotice;
	unsigned long int vDisplayTimerHandle;
	double vDetectorGain[cMaximumNumberOfDetectors12];
	double vDACDetectorOffset[cMaximumNumberOfDetectors12];
	int vADCDetectorOffset[cMaximumNumberOfDetectors12];
	//bool vADRange13;
	//bool vADInvert;
	//CString vClockString;
	BYTE vNumberOfDetectors;

	CScanTracDlg *vMainWindowPointer;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;
	bool vSubFunction4ButtonEnable;

	// Construction
	CDetectorSetupDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void SetChangeMade();
	void UpdateDisplay();
	CString PadLeft(CString TempString,BYTE TempSpaces);
	void ShowSourceStrengthData();
	void ShowInspxFunctions();
	void PrepareToExit();


// Dialog Data
	//{{AFX_DATA(CDetectorSetupDialog)
	enum { IDD = IDD_DetectorSetupDialog };
	CStatic	m_DetectorMeasuredOffset12;
	CStatic	m_DetectorMeasuredOffset11;
	CStatic	m_DetectorMeasuredOffset10;
	CStatic	m_DetectorMeasuredOffset9;
	CStatic	m_DetectorMeasuredOffset8;
	CStatic	m_DetectorMeasuredOffset7;
	CStatic	m_DetectorOffset12;
	CStatic	m_DetectorOffset11;
	CStatic	m_DetectorOffset10;
	CStatic	m_DetectorOffset9;
	CStatic	m_DetectorOffset8;
	CStatic	m_DetectorOffset7;
	CStatic	m_DetectorMeasuredGain12;
	CStatic	m_DetectorMeasuredGain11;
	CStatic	m_DetectorMeasuredGain10;
	CStatic	m_DetectorMeasuredGain9;
	CStatic	m_DetectorMeasuredGain8;
	CStatic	m_DetectorMeasuredGain7;
	CStatic	m_DetectorGain12;
	CStatic	m_DetectorGain11;
	CStatic	m_DetectorGain10;
	CStatic	m_DetectorGain9;
	CStatic	m_DetectorGain8;
	CStatic	m_DetectorGain7;
	CStatic	m_Detector12;
	CStatic	m_Detector11;
	CStatic	m_Detector10;
	CStatic	m_Detector9;
	CStatic	m_Detector8;
	CStatic	m_Detector7;
	CLeftFunctionButton	m_SubFunction4Button;
	CStatic	m_ModeButton;
	CRightFunctionButton	m_SubFunction8Button;
	CStatic	m_SubFunction9Button;
	CRightFunctionButton	m_SubFunction7Button;
	CDownFunctionButton	m_Function3Button;
	CStatic	m_ArrowLeft2;
	CStatic	m_ADCRegister1Label;
	CStatic	m_ADCRegister1;
	CStatic	m_ADCRegister0Label;
	CStatic	m_ADCRegister0;
	CLeftFunctionButton	m_SubFunction2Button;
	CStatic	m_ArrowDown4;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_XRaysOn2;
	CStatic	m_Background;
	CStatic m_ADCOffsetButton;
	CStatic m_ClockDividerDisplay;
	CStatic m_DACOffsetsButton;
	CStatic m_Detector1;
	CStatic m_Detector2;
	CStatic m_Detector3;
	CStatic m_Detector4;
	CStatic m_Detector5;
	CStatic m_Detector6;
	CStatic m_DetectorGain1;
	CStatic m_DetectorGain2;
	CStatic m_DetectorGain3;
	CStatic m_DetectorGain4;
	CStatic m_DetectorGain5;
	CStatic m_DetectorGain6;
	CStatic m_DetectorMeasuredGain1;
	CStatic m_DetectorMeasuredGain2;
	CStatic m_DetectorMeasuredGain3;
	CStatic m_DetectorMeasuredGain4;
	CStatic m_DetectorMeasuredGain5;
	CStatic m_DetectorMeasuredGain6;
	CStatic m_DetectorMeasuredOffset1;
	CStatic m_DetectorMeasuredOffset2;
	CStatic m_DetectorMeasuredOffset3;
	CStatic m_DetectorMeasuredOffset4;
	CStatic m_DetectorMeasuredOffset5;
	CStatic m_DetectorMeasuredOffset6;
	CStatic m_DetectorOffset1;
	CStatic m_DetectorOffset2;
	CStatic m_DetectorOffset3;
	CStatic m_DetectorOffset4;
	CStatic m_DetectorOffset5;
	CStatic m_DetectorOffset6;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function5Button;
	CStatic m_Function5Display;
	CStatic m_NumberOfXRayDetectorsDisplay;
	CStatic m_PreAmplifierGain;
	CLeftFunctionButton m_SubFunction1Button;
	CStatic m_SubFunction2Display;
	CLeftFunctionButton m_SubFunction3Button;
	CRightFunctionButton m_SubFunction5Button;
	CRightFunctionButton m_SubFunction6Button;
	CStatic m_TableLabel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetectorSetupDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetectorSetupDialog)
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
	afx_msg void OnSubFunction9Button();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnModeButton();
	afx_msg void OnADCOffsetButton();
	afx_msg void OnDACOffsetsButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETECTORSETUPDIALOG_H__FFD92581_C706_11D4_AD08_00500466E305__INCLUDED_)
