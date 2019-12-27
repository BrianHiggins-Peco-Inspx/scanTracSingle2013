#pragma once
#include "afxwin.h"


// CConfigureProductInspectionOptions dialog

class CConfigureProductInspectionOptions : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigureProductInspectionOptions)

public:
//variables
	bool vChangeMade;
	bool vHaveExternalDetectorsConfigured;
	BYTE vNumberOfInspectionsRequiredToEject;
	BYTE vMultiLaneNumberOfLanes;
	BYTE vMultiLaneNumberOfLinesBeforeContainer;
	BYTE vMultiLaneEjectAdjacentLanes;

	CScanTracDlg *vMainWindowPointer;
	CScanTracSystemRunningData *vLocalSystemData;
	tProductCollection *vLocalProductCollection;
	CSystemConfigurationData *vLocalConfigurationData;
	CProduct *vProductEditing;
	tCWndCollection vLocalCWndCollection;

	//Constructor
	CConfigureProductInspectionOptions(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigureProductInspectionOptions();

	//methods
	void SetChangeMade();
	void UpdateDisplay();

// Dialog Data
	enum { IDD = IDD_ConfigureProductInspectionOptions };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support



	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedFunction1button();
	afx_msg void OnStnClickedFunction2button();
	afx_msg void OnStnClickedFunction3button();
	afx_msg void OnStnClickedFunction4button();
	afx_msg void OnStnClickedFunction5button();
	afx_msg void OnStnClickedSubfunction1button();
	afx_msg void OnStnClickedSubfunction2button();
	afx_msg void OnStnClickedSubfunction3button();
	afx_msg void OnStnClickedSubfunction4button();
	afx_msg void OnStnClickedSubfunction5button();
	afx_msg void OnStnClickedSubfunction6button();
	afx_msg void OnStnClickedSubfunction7button();
	afx_msg void OnStnClickedSubfunction8button();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStatic m_Background;
	CStatic m_DialogTitleStaticText;
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
	CStatic m_SubFunction2Display;

	BYTE SetupExternalDetectors(bool TempInAutoSetup);
	CStatic m_SubFunction4Display;
};
