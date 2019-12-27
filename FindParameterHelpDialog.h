//ScanTrac Side View Source File
#if !defined(AFX_FINDPARAMETERHELPDIALOG_H__76546441_88BA_11D8_B17E_00500466E305__INCLUDED_)
#define AFX_FINDPARAMETERHELPDIALOG_H__76546441_88BA_11D8_B17E_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindParameterHelpDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFindParameterHelpDialog dialog
const WORD cMaximumNumberOfItems = 1000;
const WORD cNumberOfKeys1 = cMaxKeys * 4;
extern BYTE vGlobalShowMouseCursor;

typedef  BYTE tLocalKeyMap1[cNumberOfKeys1];

const tLocalKeyMap1 cLocalAlphaKey1 = {
	cF1,cF2,46,cF4,cF5,
	58,92,95,cSF4,126,
	58,45,cSF8,89,cSpace,65,68,
	71,74,77,80,83,86,
	//group 2
	cF1,cF2,46,cF4,cF5,
	58,92,95,cSF4,126,
	39,45,cSF8,90,cChar1,66,69,
	72,75,78,81,84,87,
	//group 3
	cF1,cF2,46,cF4,cF5,
	58,92,95,cSF4,126,
	34,45,cSF8,cPeriod,cSpace,67,70,
	73,76,79,82,85,88,
	//group 4
	cF1,cF2,46,cF4,cF5,
	58,92,95,cSF4,126,
	63,45,cSF8,cChar0,cChar1,cChar2,cChar3,
	cChar4,cChar5,cChar6,cChar7,cChar8,cChar9};

class CFindParameterHelpDialog : public CDialog
{
public:
	//variables
	WORD vCursorPosition;
 	BYTE vLastKey;
	bool vRecievedButtonPress;	//do not repeat click and wait for mouse button up if you used keyboard down 
	BYTE vMouseState;			//1 - 8 is left mouse button clicked down, 9 - 16 is left mouse button double click, 0 is idle or all functions completed
	unsigned long int vMouseDownRepeatTimerHandle;
	bool vFirstOpenedFindParameterHelp;
	WORD vLastKeyTimes;
	WORD vFirstItemShown;
	WORD vNumberOfItems;
	CString vSearchPattern;
	CString vItem[cMaximumNumberOfItems];
	CString vText[cMaximumNumberOfItems];
	bool vHidden[cMaximumNumberOfItems];
	WORD vItemShowingOnButton[9];
	HBRUSH vWhiteBrush;
	HBRUSH vBlueBrush;
	HBRUSH vYellowBrush;
	tLocalKeyMap1 vLocalKey;

	CSystemConfigurationData *vLocalConfigurationData;
	CScanTracSystemRunningData *vLocalSystemData;
	tCWndCollection vLocalCWndCollection;
	tCWndCollection vLocalDisplayCollection;
	
	// Construction
	CFindParameterHelpDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void ShowItem(WORD TempItemNumber);
	void UpdateButtons();
	void SetCursor();
	void OnKeyClear();
	bool ProcessKey(BYTE TempKey);
	void LoadSearchStrings();

// Dialog Data
	//{{AFX_DATA(CFindParameterHelpDialog)
	enum { IDD = IDD_FindParameterHelpDialog };
	CDownFunctionButton	m_Function5Button;
	CDownFunctionButton	m_Function4Button;
	CStatic	m_Background;
	CRightSelectItemFunctionButton	m_SubFunction8Button;
	CRightSelectItemFunctionButton	m_SubFunction7Button;
	CRightSelectItemFunctionButton	m_SubFunction6Button;
	CRightSelectItemFunctionButton	m_SubFunction5Button;
	CLeftSelectItemFunctionButton	m_SubFunction4Button;
	CLeftSelectItemFunctionButton	m_SubFunction3Button;
	CLeftSelectItemFunctionButton	m_SubFunction2Button;
	CLeftSelectItemFunctionButton	m_SubFunction1Button;
	CStatic m_DialogTitleStaticText;
	CStatic m_DialogTitleStaticText2;
	CStatic m_DialogTitleStaticText3;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function2Button;
	CDownFunctionButton m_Function3Button;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindParameterHelpDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFindParameterHelpDialog)
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
	afx_msg void OnSubFunction7Button();
	afx_msg void OnSubFunction8Button();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSubFunction6Button();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDPARAMETERHELPDIALOG_H__76546441_88BA_11D8_B17E_00500466E305__INCLUDED_)
