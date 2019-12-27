#pragma once


// CProductFillerSeamerSampling dialog

class CProductFillerSeamerSampling : public CDialogEx
{
	DECLARE_DYNAMIC(CProductFillerSeamerSampling)

	CScanTracDlg *vMainWindowPointer;
	CScanTracSystemRunningData *vLocalSystemData;
	tProductCollection *vLocalProductCollection;
	CSystemConfigurationData *vLocalConfigurationData;
	CProduct *vProductEditing;
	tCWndCollection vLocalCWndCollection;

	CTime vTime;
	int vDateToday;

	WORD vEncoderRateTolerance;
	WORD vOldEncoderRateTolerance;

	unsigned int vUpdateDisplayTimerHandle;
	bool vChangeMade;
	
	double vDistanceFromSeamerObjectToScanTracTrigger;
	BYTE vOldFindFillerOffset[4];
	WORD vOldEncoderRate[4];
	WORD vOldCurrentEncoderRate;
	
public:
	CProductFillerSeamerSampling(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProductFillerSeamerSampling();

// Dialog Data
	enum { IDD = IDD_ProductSetupFillerSeamerSamplingDialog };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void UpdateDisplay();
	void OneSecondDisplayUpdate();
	void SetChangeMade();
	void ShowHideInspxButtons(bool vShowInspxButtons);
	BYTE AskIfContainerIsHighLowOrVariable();
	void SetFillerOffsets();
	void SetSeamerOffsets();

	afx_msg void OnDialogTitleStaticText1();
	
	CStatic m_Background;
	CStatic m_DialogTitleStaticText;
	CStatic m_Display1;
	CStatic m_Display2;
	CStatic m_Display3;
	CStatic m_Display4;
	CStatic m_Display5;
	CStatic m_Display6;
	CStatic m_Display7;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton	m_Function2Button;
	CDownFunctionButton m_Function3Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton m_Function5Button;
	CLeftFunctionButton m_SubFunction1Button;
	CLeftFunctionButton m_SubFunction2Button;
	CLeftFunctionButton m_SubFunction3Button;
	CLeftFunctionButton m_SubFunction4Button;
	CRightFunctionButton m_SubFunction5Button;
	CRightFunctionButton m_SubFunction6Button;
	CRightFunctionButton m_SubFunction7Button;
	CRightFunctionButton m_SubFunction8Button;
	CStatic m_SubFunction1DisplayAbove;
	CStatic m_SubFunction5DisplayAbove;
	CStatic m_SubFunction1Display;
	CStatic m_SubFunction2Display;
	CStatic m_SubFunction3Display;
	CStatic m_SubFunction4Display;
	CStatic m_SubFunction5Display;
	CStatic m_SubFunction6Display;
	CStatic m_SubFunction7Display;
	CStatic m_SubFunction8Display;
	CStatic m_Function3Display1;
	CStatic m_FrequencyRange1Display;
	CStatic m_FrequencyRange2Display;
	CStatic m_FrequencyRange3Display;
	CStatic m_FrequencyRange4Display;
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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStatic m_Time2;
	CStatic m_Date;
	CStatic m_EncoderRate;
	CStatic m_EncoderRateLabel;
};
