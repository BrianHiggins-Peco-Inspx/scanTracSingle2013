#pragma once


// CCheckWeighOffsetDialog dialog

class CCheckWeighOffsetDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckWeighOffsetDialog)

public:
	unsigned long int vWeightsTakenTimerHandle;
	double vAverageMeasuredWeightOfContainer;
	double vContainerWeights[50];
	BYTE vContainersSentSoFar;
	bool vEnteringWeight;
	CString vEditString;
	BYTE vCursorPosition;
	bool vDeleteAll;
	BYTE vCheckWeighInspectionNumber;
	CInspectionCheckWeigh *vCheckWeighInspection;
	CScanTracDlg *vMainWindowPointer;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

	CCheckWeighOffsetDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckWeighOffsetDialog();
		
	//Methods
	void SetCursor();
	void ProcessKey(BYTE TempKey);
	void StartEnteringWeight();
	void UpdateDisplay();
	void OnKeyHelp();
	void UpdateFullTextDisplay();

// Dialog Data
	enum { IDD = IDD_CheckWeighOffsetDialog };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFunction2button();
	afx_msg void OnFunction1button();
	afx_msg void OnFunction3button();
	afx_msg void OnFunction4button();
	afx_msg void OnFunction5button();
	afx_msg void OnSubfunction4button();
	afx_msg void OnSubfunction8button();
	afx_msg void OnSubfunction1button();
	afx_msg void OnSubfunction2button();
	afx_msg void OnSubfunction3button();
	afx_msg void OnSubfunction5button();
	afx_msg void OnSubfunction6button();
	afx_msg void OnSubfunction7button();
	afx_msg void OnKey1();
	afx_msg void OnKey2();
	afx_msg void OnKey3();
	afx_msg void OnKey4();
	afx_msg void OnKey5();
	afx_msg void OnKey6();
	afx_msg void OnKey7();
	afx_msg void OnKey8();
	afx_msg void OnKey9();
	afx_msg void OnKey0();
	afx_msg void OnKeyclear();
	afx_msg void OnKeyhelp();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	CStatic m_Background;
	CStatic m_DialogTitleStaticText1;
	CStatic m_DialogTitleStaticText2;
	CStatic m_DialogTitleStaticText3;
	CStatic m_DialogTitleStaticText4;
	CStatic m_DialogTitleStaticText;
	CStatic m_EditStaticText;
	CStatic m_Key0;
	CStatic m_Key1;
	CStatic m_Key2;
	CStatic m_Key3;
	CStatic m_Key4;
	CStatic m_Key5;
	CStatic m_Key6;
	CStatic m_Key7;
	CStatic m_Key8;
	CStatic m_Key9;
	CStatic m_KeyClear;
	CStatic m_KeyHelp;
	CStatic m_UnitsString;
	CStatic m_FullTextDisplay;
	CStatic m_FullTextDisplay3;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton	m_Function2Button;
	CDownFunctionButton	m_Function3Button;
	CDownFunctionButton m_Function4Button;
	CDownFunctionButton m_Function5Button;
	CLeftFunctionButton	m_SubFunction1Button;
	CLeftFunctionButton m_SubFunction2Button;
	CLeftFunctionButton m_SubFunction3Button;
	CLeftFunctionButton m_SubFunction4Button;
	CRightFunctionButton m_SubFunction5Button;
	CRightFunctionButton m_SubFunction6Button;
	CRightFunctionButton m_SubFunction7Button;
	CRightFunctionButton m_SubFunction8Button;
	CStatic m_DialogTitleStaticText8;
	CStatic m_UnitsStringBox;

	afx_msg void OnDialogtitlestatictext8();
};
