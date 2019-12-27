#pragma once


// CFillerSeamerSamplingDialog dialog

class CFillerSeamerSamplingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CFillerSeamerSamplingDialog)

public:
	CScanTracDlg *vMainWindowPointer;
	//tProductCollection *vLocalProductCollection;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;
	
	WORD vOldSampleIndexErrorsX[3];
	WORD vOldSampleIndexErrorsY[3];
	WORD vOldNextHeadToProcess[3];
	double vOldMostRecentTimeBeforeIndexPulse[3];
	double vOldMostRecentTimeAfterIndexPulse[3];
	WORD vOldValveDataRowIndex;
	WORD vOldNumberValveRowDataHave;

	unsigned int vUpdateDisplayTimerHandle;
	bool vChangeMade;
	double vOriginalFillerTolerance;
	BYTE vOldNumberOfValvesThatNeedAttention;
	DWORD vFillerIndexErrorsX;
	DWORD vOldFillerIndexErrorsX;
	DWORD vFillerIndexErrorsY;
	DWORD vOldFillerIndexErrorsY;
	DWORD vSeamerIndexErrorsX;
	DWORD vOldSeamerIndexErrorsX;
	DWORD vSeamerIndexErrorsY;
	DWORD vOldSeamerIndexErrorsY;
	double vOldFillerAverageWeight;

	CFillerSeamerSamplingDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFillerSeamerSamplingDialog();

// Dialog Data
	enum { IDD = IDD_FillerSeamerSamplingDialog };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void SetChangeMade() ;
	void OneSecondDisplayUpdate();
	void UpdateDisplay();

	CDownFunctionButton m_Function2Button;
	CDownFunctionButton m_Function3Button;
	CDownFunctionButton m_Function4Button;
	CDownFunctionButton m_Function5Button;
	CRightFunctionButton m_SubFunction8Button;
	CStatic m_Background;
	CStatic m_Function3Display;
//	CStatic m_FillAfterIndexDisplay;
	CStatic m_FillerBeforeIndexDisplay;
	CStatic m_SeamerAfterIndexDisplay;
	CStatic m_SeamerBeforeIndexDisplay;
	CStatic m_FillerAfterIndexDisplay;
	CDownFunctionButton m_Function1Button;
	CLeftFunctionButton m_SubFunction1Button;
	CLeftFunctionButton m_SubFunction2Button;
	CStatic m_SubFunction1Display;
	CStatic m_SubFunction2Display;
	CLeftFunctionButton m_SubFunction3Button;
	CStatic m_SubFunction3Display;
	CLeftFunctionButton m_SubFunction4Button;
	CStatic m_SubFunction4Display;
	CRightFunctionButton m_SubFunction5Button;
	CRightFunctionButton m_SubFunction6Button;
	CRightFunctionButton m_SubFunction7Button;
	CStatic m_ClockDisplay;
	CStatic m_DialogTitleStaticText1;

	public:
	afx_msg void OnDialogtitlestatictext1();

	CStatic m_ConveyorSpeedError;
};
