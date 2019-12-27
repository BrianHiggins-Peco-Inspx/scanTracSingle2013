//Scan Trac Solo Source File ScanTrac
// ConfigureProductAlarmsDialog.h : header file
//
#if !defined(AFX_SELECTMULTIPLEITEMDIALOG_H__8463B021_4DF6_11D5_AE03_00500466E305__INCLUDED_)
#define AFX_SELECTMULTIPLEITEMDIALOG_H__8463B021_4DF6_11D5_AE03_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Product.h"

const BYTE cMaxItemsPerPage = 8;
const DWORD cDWORDLastSignificantDigitValue = 2147483648; // 2^31 or bit 31 in [31:0]

/////////////////////////////////////////////////////////////////////////////
// CConfigureProductAlarmsDialog dialog

class CConfigureProductAlarmsDialog : public CDialog
{
	DECLARE_DYNAMIC(CConfigureProductAlarmsDialog)

public:
	RECT vCheckMarkGraphicSizeRect;
	int vCurrentFirstItemNumberShown;
	WORD vXValue;
	WORD vYValue;
	bool vUseSelectText;
	bool vItemEnabled[cMaximumNumberOfInspections];
	bool vChangeMade;
	bool vItems[cMaximumNumberOfInspections];
	BYTE vEjectorsEnabled;
	bool vAllSelected;
	BYTE vInAutoSetup;
	BYTE vNumberOfItems;
	BYTE vNumberOfItemsSelected;
	BYTE vCurrentPage;
	BYTE vAlarmNumber;
	HBRUSH vLightGreenBrush;
	HBRUSH vGreenBrush;
	HBRUSH vYellowBrush;
	HBRUSH vWhiteBrush;
	DWORD vConfigurationSettings;
	CString vInspectionName[cMaximumNumberOfInspections];
	CBitmap vGraphicBitmap;
	//CString vInspectionName[cMaximumNumberOfInspections + 1];
	tCWndCollection vLocalCWndCollection;
	tCWndCollection vLocalDisplayCollection;

	// Construction
	CConfigureProductAlarmsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigureProductAlarmsDialog();

	//methods
	void UpdateItemDisplay();
	void ToggleInspection(BYTE TempInspectionNumber);
	bool CheckAllSelected();
	void UpdateSettings(BYTE TempBitNum);
	void LoadSettings(DWORD TempSettings);
	void ToggleAllSelected();
	void SetupMenu();

	// Dialog Data
	enum { IDD = IDD_ConfigureProductAlarmsDialog };
	CRightDoubleFunctionButton	m_SubFunction8Button;
	CRightDoubleFunctionButton	m_SubFunction7Button;
	CRightDoubleFunctionButton	m_SubFunction6Button;
	CRightDoubleFunctionButton	m_SubFunction5Button;
	CLeftDoubleFunctionButton	m_SubFunction4Button;
	CLeftDoubleFunctionButton	m_SubFunction3Button;
	CLeftDoubleFunctionButton	m_SubFunction2Button;
	CLeftDoubleFunctionButton	m_SubFunction1Button;
	CStatic	m_CheckMarkGraphic8;
	CStatic	m_CheckMarkGraphic7;
	CStatic	m_CheckMarkGraphic6;
	CStatic	m_CheckMarkGraphic5;
	CStatic	m_CheckMarkGraphic4;
	CStatic	m_CheckMarkGraphic3;
	CStatic	m_CheckMarkGraphic2;
	CStatic	m_CheckMarkGraphic1;
	CStatic	m_Item8;
	CStatic	m_Item7;
	CStatic	m_Item6;
	CStatic	m_Item5;
	CStatic	m_Item4;
	CStatic	m_Item3;
	CStatic	m_Item2;
	CStatic	m_Item1;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton	m_Function2Button;
	CDownFunctionButton	m_Function3Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function5Button;
	CStatic	m_Background;
	CStatic	m_DialogTitleStaticText;
	CStatic m_XOfYTextBox;
	CStatic m_TotalTextbox;
	CStatic m_RejectsTextbox;
	CStatic m_ofTotalTextbox;

//Overrides
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:

	// Generated message map functions
	afx_msg void OnFunction1button();
	afx_msg void OnFunction2button();
	afx_msg void OnFunction3button();
	afx_msg void OnFunction4button();
	afx_msg void OnFunction5button();
	afx_msg void OnSubfunction1button();
	afx_msg void OnSubfunction2button();
	afx_msg void OnSubfunction3button();
	afx_msg void OnSubfunction4button();
	afx_msg void OnSubfunction5button();
	afx_msg void OnSubfunction6button();
	afx_msg void OnSubfunction7button();
	afx_msg void OnSubfunction8button();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDialogtitlestatictext1();
};


#endif