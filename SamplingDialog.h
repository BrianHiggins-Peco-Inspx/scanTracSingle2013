#pragma once


// CSamplingDialog dialog

class CSamplingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSamplingDialog)

public:
	BYTE vSamplingType;
	BYTE vNumberOfEjectors;
	CString vValve_HeadTextString;

	CScanTracDlg *vMainWindowPointer;
	//tProductCollection *vLocalProductCollection;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

	WORD vOldSamplingIndexErrorDataHave[3];
	bool vOldLastIndexPulseWasCorrect[3];
	WORD vOldNextHeadToProcess[3];
	DWORD vOldSamplesEjectedCount[3];
	bool vOldShowAbortSamplingOnButtons;
	bool vShowAbortSamplingOnButtons;
	WORD 	vSamplesToTake;
	WORD vOldSamplesToTake;

	bool vChangeMade;
	unsigned int vUpdateDisplayTimerHandle;

	BYTE vOldValveSamplesEjectedCount;
	BYTE vOldHeadSamplesEjectedCount;

	CSamplingDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSamplingDialog();

// Dialog Data
	enum { IDD = IDD_SamplingDialog };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void UpdateDisplay();
	void OneSecondDisplayUpdate();
	void SetChangeMade();
	
	CStatic m_Background;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function2Button;
	CDownFunctionButton m_Function3Button;
	CDownFunctionButton m_Function4Button;
	CDownFunctionButton m_Function5Button;
	CLeftFunctionButton m_SubFunction1Button;
	CLeftFunctionButton m_SubFunction2Button;
	CLeftFunctionButton m_SubFunction3Button;
	CLeftFunctionButton m_SubFunction4Button;
	CRightFunctionButton m_SubFunction5Button;
	CRightFunctionButton m_SubFunction6Button;
	CRightFunctionButton m_SubFunction7Button;
	CRightFunctionButton m_SubFunction8Button;
	CStatic m_SubFunction1Display;
	CStatic m_SubFunction3Display;
	CStatic m_SubFunction5Display;
	CStatic m_SubFunction6Display;
	
//	CStatic m_LastHeadprocessedDisplay;
	CStatic m_NumberOfRotationsDisplay;
	CStatic m_LastIndexPulseDisplay;
	CStatic m_SeamerStatusDisplay;
	CStatic m_LastHeadProcessedDisplay;
	CStatic m_FillerStatusDisplay2;
	afx_msg void OnDialogtitlestatictext1();
	CStatic m_ClockDisplay2;
	CStatic m_DialogStaticText1;
	CStatic m_FillerStatusDisplay;
	CStatic m_SamplesToTakeDisplay;
};
