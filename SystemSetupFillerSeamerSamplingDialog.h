#pragma once


// CSystemSetupFillerSeamerSamplingDialog dialog

class CSystemSetupFillerSeamerSamplingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSystemSetupFillerSeamerSamplingDialog)

public:
	CScanTracDlg *vMainWindowPointer;
	//tProductCollection *vLocalProductCollection;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;

	unsigned int vUpdateDisplayTimerHandle;
	bool vChangeMade;
	BYTE vNumberOfHeadsToMonitor[3];
	//BYTE vHeadOffsetFromIndexPulse[8];
	BYTE vShowIndexErrors;
	BYTE vShowSampleAllButton;
	bool vOldFindFillerOffsetFullSpeed;
	bool vOldFindFillerOffsetHalfSpeed;
	int vEncoderRateTolerance;
	WORD vOldEncoderRateTolerance;
	tCWndCollection vLocalCWndCollection;

	BYTE vOldIndexPulses[8];

	//constructor
	CSystemSetupFillerSeamerSamplingDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSystemSetupFillerSeamerSamplingDialog();

// Dialog Data
	enum { IDD = IDD_SystemSetupFillerSeamerSamplingDialog1 };

protected:
	CStatic m_Background;
	CStatic m_DialogTitleStaticText1;
	CStatic m_Display1;
	CStatic m_Display2;
	CStatic m_Display3;
	CStatic m_Display4;
	CStatic m_Display5;
	CStatic m_Display6;
	CStatic m_Display7;
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
	CStatic m_Function3Display;
	CStatic m_SubFunction1Display;
	CStatic m_SubFunction1DisplayAbove;
	CStatic m_SubFunction2Display;
	CStatic m_SubFunction2Display2;
	CStatic m_SubFunction2Display3;
	CStatic m_SubFunction4Display;
	CStatic m_SubFunction5Display;
	CStatic m_SubFunction5DisplayAbove;
	CStatic m_SubFunction6Display;
	CStatic m_SubFunction6Display4;
	CStatic m_SubFunction7Display;
	CStatic m_SubFunction8Display;
	
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
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDialogtitlestatictext1();
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetChangeMade() ;
	void UpdateDisplay();
	void OneSecondDisplayUpdate();
	CStatic m_Function5Display1;
};
