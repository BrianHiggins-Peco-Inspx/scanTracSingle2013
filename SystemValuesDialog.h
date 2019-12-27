//ScanTrac Side View Source File
#if !defined(AFX_SYSTEMVALUESDIALOG_H__2003B461_AD99_11D7_B0DE_00500466E305__INCLUDED_)
#define AFX_SYSTEMVALUESDIALOG_H__2003B461_AD99_11D7_B0DE_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemValuesDialog.h : header file
//
enum {cNotCalibrating, cCalXrayOnNoProduction, cCalXrayOffYesProduction};
/////////////////////////////////////////////////////////////////////////////
// CSystemValuesDialog dialog

class CSystemValuesDialog : public CDialog
{
public:
	//variables
	WORD vOldImageNumberToDisplay;
	WORD vOldXRayState;
	BYTE vMaximumSuggestedPPI;
	BYTE vMinimumSuggestedPPI;
	BYTE vUseRealEncoder;
	BYTE vBulkProductMode; 
	byte vNeedToCalibrateEncoder;
	double vCalibrateEncoderContainerWidth;
	unsigned long int vUpdateDisplayTimerHandle;
	unsigned long int vUpdateEncoderRateTimerHandle;
	WORD vOldLastBodyTriggerLength;
	double vOldBodyTriggerWidthStandardDeviation;
	double vOldBodyTriggerWidthAverage;
	bool vOldXRayOnNotice;
	bool vWaitingForContainer;
	unsigned long int vOldContainerCount;
	BYTE vOldSystemRunMode;
	CProduct *vOriginalProduct;
	double vSystemBodyTriggerToXRayBeam;
	double vSystemEjectorPositionOffset;
	double vSystemEjectorDistance[cNumberOfEjectors];
	bool vCalibratingWithXRays;

	BYTE vUsePipeFlowInput;
	double vPipeLineFlowRate;
	BYTE vEncoderTolerance;
	BYTE vNumberOfEncoderRateReversalsForError;
	CScanTracDlg *vMainWindowPointer;
	WORD vContinuousFeedEncoderRate;
	WORD vContinuousFeedContainerRate;
	WORD vOriginalEncoderDivider;
	WORD vOriginalEncoderDividerForPoint4mmMode;
	bool vChangeMade;
	CString vUnitsString;
	double vPixelsPerUnit;
	double vPixelsPerUnitForPoint4mmMode;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tProductCollection *vLocalProductCollection;
	WORD vCalibrateEncoderMethod;
	BOOL vEncoderDividerChanged;
	double vEnteredContainerWidth;
	BOOL vEncoderProblemTryAgain;
	BOOL vLockOutFunction3Button;
	tCWndCollection vLocalCWndCollection;
	bool vSubFunction5ButtonEnable;
	bool vSubFunction6ButtonEnable;

// Construction
	CSystemValuesDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void SetChangeMade();
	void UpdateDisplay();
	//void ConvertToInches();
	//void ConvertToMetric();
	void ChangeScale();
	void UpdateButtons();
	void RestoreValues();
	void PrepareToExit();
	void CalculateCalibration();
	void CalibrateEncoderWithXRaysNoProductionSelection();		// was Function 3 button
	void EndCalibrateEncoderWithoutXRaysYesProductionSelection();
	void CalibrateEncoderWithoutXRaysYesProductionSelection();		// corner button
	void SelectTypeOfEncoder();
	//void SendEncoderDivider();


// Dialog Data
	//{{AFX_DATA(CSystemValuesDialog)
	enum { IDD = IDD_SystemValuesDialog };
	CStatic	m_SubFunction9Display;
	CStatic	m_SubFunction9Button;
	CStatic	m_SubFunction2Display;
	CLeftFunctionButton	m_SubFunction2Button;
	CStatic	m_SubFunction1Display;
	CLeftFunctionButton	m_SubFunction1Button;
	CLeftFunctionButton	m_SubFunction3Button;
	CStatic	m_SubFunction3Display;
	CDownFunctionButton	m_Function5Button;
	CRightFunctionButton	m_SubFunction7Button;
	CStatic	m_SubFunction6Display;
	CRightFunctionButton	m_SubFunction6Button;
	CStatic	m_SubFunction5Display;
	CRightFunctionButton	m_SubFunction5Button;
	CStatic	m_Calibrating;
//	CStatic	m_CalibrateEncoderWithoutXRays;
	CStatic	m_XRaysOn;
	CDownFunctionButton	m_Function3Button;
	CStatic	m_Background;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic m_SubFunction7Display;
	CStatic m_DialogTitleStaticText;
	CStatic m_EncoderCountsOnLastTrigger;
	CDownFunctionButton m_Function1Button;
	CStatic m_EncoderCountsOnLastTriggerLabel3;
	CStatic m_Function3Display;
	CLeftFunctionButton m_SubFunction4Button;
	CStatic m_SubFunction4Display;
	CRightFunctionButton m_SubFunction8Button;
	CStatic m_SubFunction8Display;
	CStatic m_YellowNoticeLabel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemValuesDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSystemValuesDialog)
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
	afx_msg void OnTimer(UINT nIDEvent);
//	afx_msg void OnCalibrateEncoderWithoutXRays();
	afx_msg void OnSubFunction9Button();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMVALUESDIALOG_H__2003B461_AD99_11D7_B0DE_00500466E305__INCLUDED_)
