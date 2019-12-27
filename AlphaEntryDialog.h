//ScanTrac Side View Source File
#if !defined(AFX_ALPHAENTRYDIALOG_H__D6B43361_F136_11D3_ABD6_00500466E305__INCLUDED_)
#define AFX_ALPHAENTRYDIALOG_H__D6B43361_F136_11D3_ABD6_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlphaEntryDialog.h : header file
//
const WORD cNumberOfKeys = cMaxKeys * 4;
extern BYTE vGlobalShowMouseCursor;

typedef  BYTE tLocalKeyMap[cNumberOfKeys+100];

const tLocalKeyMap cLocalAlphaKey = {
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

const tLocalKeyMap cLocalLowerAlphaKey = {
	cF1,cF2,46,cF4,cF5,
	58,92,95,cSF4,126,
	58,45,cSF8,121,cSpace,97,100,
	103,106,109,112,115,118,
	//group 2
	cF1,cF2,46,cF4,cF5,
	58,92,95,cSF4,126,
	39,93,cSF8,122,cChar1,98,101,
	104,107,110,113,116,119,
	//group 3
	cF1,cF2,46,cF4,cF5,
	58,92,95,cSF4,126,
	34,44,cSF8,cPeriod,cSpace,99,102,
	105,108,111,114,117,120,
	//group 4
	cF1,cF2,46,cF4,cF5,
	58,92,95,cSF4,126,
	63,cPeriod,cSF8,cChar0,cChar1,cChar2,cChar3,
	cChar4,cChar5,cChar6,cChar7,cChar8,cChar9};


/////////////////////////////////////////////////////////////////////////////
// CAlphaEntryDialog dialog

class CAlphaEntryDialog : public CDialog
{
public:

	//variables
	BYTE vFlashAllLampStatus;
	CFont vCurrentSmallFont;
	int *vColor;
	CHARRANGE vFormatCharacterRange;
	CHARFORMAT vCharacterFormat;
	CHARFORMAT vRedCharacterFormat;
	CHARFORMAT vSelectCharacterFormat;
	CScanTracSystemRunningData *vLocalSystemData;
	CScanTracDlg *vMainWindowPointer;
	unsigned int vFlashLightsTimerHandle;
	BYTE vOriginalLampStatus;
	bool vFlashingLights;
	bool vNumbersOnly;
	bool vNoMainMenu;
	bool vAcceptAllKeys;
	bool vChatMode;
	bool vFindTextMode;
	bool vShowingTouchKeyboard;
	bool vAllowDashes;
	bool vAllowPeriod;
	bool vAllowApproximate;
	bool vAllowParenthesis;
	bool vAllowComma;
	bool vAllowIPComma;
	bool vAllowSlashes;
	bool vBlankNameOK;
	bool vAllowMoreThan32Characters;
	BYTE vMaximumNumberOfCharacters;
	bool vInAutoSetup;
	BYTE vMinimumNumberOfCharacters;
	HBRUSH vWhiteBrush;
	HBRUSH vGreenBrush;
	HBRUSH vLightBlueBrush;
	HBRUSH vBlueBrush;
	HBRUSH vRedBrush;
	bool vUpperCase;
	bool vCapLock;
	bool vDeleteAll;
	bool vOnScreenKeyBoardPress;
	WORD vCursorPosition;
 	BYTE vLastKey;
	WORD vLastKeyTimes;
	tLocalKeyMap vLocalKey;
	CString vEditString;
	tCWndCollection vLocalCWndCollection;
	WORD vDoNotClickUpperY;
	WORD vDoNotClickLowerY;
	WORD vDoNotClickLeftX;
	WORD vDoNotClickRightX;

	void ShowTouchScreenKeyboard(bool vShowKeyboard);
	void ProcessAlphaNumericKey(char TempKey);
	
	// Construction
	CAlphaEntryDialog(CWnd* pParent = NULL);   // standard constructor

	//destructor
	virtual ~CAlphaEntryDialog();

	//Methods
	void SetCursor();
	bool ProcessKey(BYTE TempKey);
	void SaveAndExit();
	void PrepareToExit();
	void DisplayColor();
	void SetKeyBoardToLowerCase();
	void SetKeyBoardToUpperCase();

// Dialog Data
	//{{AFX_DATA(CAlphaEntryDialog)
	enum { IDD = IDD_AlphaEntryDialog };
	CStatic	m_DialogTitleStaticText3Control;
	CRichEditCtrl	m_TextEditing;
	CDownFunctionButton	m_Function1Button;
	CRightFunctionButton	m_SubFunction5Button;
	CRightFunctionButton	m_SubFunction6Button;
	CStatic	m_DialogTitleStaticText2Control;
	CStatic	m_DialogTitleStaticTextControl;
	CStatic	m_DialogTitleStaticText1Control;
	CLeftFunctionButton	m_SubFunction1Button;
	CLeftFunctionButton	m_SubFunction2Button;
	CRightFunctionButton	m_SubFunction7Button;
	CLeftFunctionButton	m_SubFunction3Button;
	CDownFunctionButton	m_Function5Button;
	CDownFunctionButton	m_Function3Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_KeyHelp;
	CStatic	m_KeyClear;
	CStatic	m_Key9;
	CStatic	m_Key8;
	CStatic	m_Key7;
	CStatic	m_Key6;
	CStatic	m_Key5;
	CStatic	m_Key4;
	CStatic	m_Key3;
	CStatic	m_Key2;
	CStatic	m_Key1;
	CStatic	m_Key0;
	CStatic	m_Background;
	CEdit	m_EditBoxControl;
	CString	m_DialogTitleStaticText2;
	CString	m_EditBoxValue;
	CString	m_DialogTitleStaticText;
	CString	m_TextEditingString;
	CString	m_DialogTitleStaticText3;
	CDownFunctionButton m_Function4Button;
	CLeftFunctionButton m_SubFunction4Button;
	CRightFunctionButton m_SubFunction8Button;
//	CStatic m_Title;
	CStatic m_DialogTitleStaticTextBox1;
	CStatic m_DialogTitleStaticTextBox2;
	CStatic m_DialogTitleStaticTextBox3;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlphaEntryDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlphaEntryDialog)
	afx_msg void OnSubFunction8Button();
	afx_msg void OnSubFunction7Button();
	afx_msg void OnSubFunction6Button();
	afx_msg void OnFunction3Button();
	afx_msg void OnFunction2Button();
	afx_msg void OnFunction1Button();
	afx_msg void OnFunction4Button();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSubFunction3Button();
	afx_msg void OnSubFunction4Button();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSubFunction1Button();
	afx_msg void OnSubFunction2Button();
	afx_msg void OnSubFunction5Button();
	afx_msg void OnKey0();
	afx_msg void OnKey1();
	afx_msg void OnKey2();
	afx_msg void OnKey3();
	afx_msg void OnKey4();
	afx_msg void OnKey5();
	afx_msg void OnKey6();
	afx_msg void OnKey7();
	afx_msg void OnKey8();
	afx_msg void OnKey9();
	afx_msg void OnKeyClear();
	afx_msg void OnKeyHelp();
	afx_msg void OnFunction5Button();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
	CStatic m_KeyBoard1;
	CStatic m_KeyBoard10;
	CStatic m_KeyBoard11;
	CStatic m_KeyBoard12;
	CStatic m_KeyBoard13;
	CStatic m_KeyBoard14;
	CStatic m_KeyBoard15;
	CStatic m_KeyBoard16;
	CStatic m_KeyBoard17;
	CStatic m_KeyBoard18;
	CStatic m_KeyBoard19;
	CStatic m_KeyBoard2;
	CStatic m_KeyBoard20;
	CStatic m_KeyBoard21;
	CStatic m_KeyBoard22;
	CStatic m_KeyBoard23;
	CStatic m_KeyBoard24;
	CStatic m_KeyBoard25;
	CStatic m_KeyBoard26;
	CStatic m_KeyBoard27;
	CStatic m_KeyBoard3;
	CStatic m_KeyBoard4;
	CStatic m_KeyBoard5;
	CStatic m_KeyBoard6;
	CStatic m_KeyBoard7;
	CStatic m_KeyBoard8;
	CStatic m_KeyBoard9;
	CStatic m_KeyBoard28;
	CStatic m_KeyBoard29;
	CStatic m_KeyBoard30;
	CStatic m_KeyBoard31;
	CStatic m_KeyBoard32;
	CStatic m_KeyBoard33;
	CStatic m_KeyBoard34;
	CStatic m_KeyBoard35;
	CStatic m_KeyBoard36;
	CStatic m_KeyBoard37;

	CStatic m_KeyBoard38;
	CStatic m_KeyBoard39;
	CStatic m_KeyBoard40;
//	CStatic m_KeyBoard47;
//	CStatic m_KeyBoard48;
//	CStatic m_KeyBoard49;
	CStatic m_KeyBoard41;
	CStatic m_KeyBoard42;
	CStatic m_KeyBoard43;
	CStatic m_KeyBoard44;
	CStatic m_KeyBoard45;
	CStatic m_KeyBoard46;
	CStatic m_KeyBoard47;

	afx_msg void OnKeyboard1();
	afx_msg void OnKeyboard2();
	afx_msg void OnKeyboard3();
	afx_msg void OnKeyboard4();
	afx_msg void OnKeyboard5();
	afx_msg void OnKeyboard6();
	afx_msg void OnKeyboard7();
	afx_msg void OnKeyboard8();
	afx_msg void OnKeyboard9();
	afx_msg void OnKeyboard10();
	afx_msg void OnKeyboard11();
	afx_msg void OnKeyboard13();
	afx_msg void OnKeyboard12();
	afx_msg void OnKeyboard14();
	afx_msg void OnKeyboard15();
	afx_msg void OnKeyboard16();
	afx_msg void OnKeyboard17();
	afx_msg void OnKeyboard18();
	afx_msg void OnKeyboard19();
	afx_msg void OnKeyboard20();
	afx_msg void OnKeyboard21();
	afx_msg void OnKeyboard22();
	afx_msg void OnKeyboard23();
	afx_msg void OnKeyboard24();
	afx_msg void OnKeyboard25();
	afx_msg void OnKeyboard26();
	afx_msg void OnKeyboard27();
	afx_msg void OnKeyboard28();
	afx_msg void OnKeyboard29();
	afx_msg void OnKeyboard30();
	afx_msg void OnKeyboard31();
	afx_msg void OnKeyboard32();
	afx_msg void OnKeyboard33();
	afx_msg void OnKeyboard34();
	afx_msg void OnKeyboard35();
	afx_msg void OnKeyboard36();
	afx_msg void OnKeyboard37();
	afx_msg void OnKeyboard38();
	afx_msg void OnKeyboard39();
	afx_msg void OnKeyboard40();
	afx_msg void OnKeyboard41();
	afx_msg void OnKeyboard42();
	afx_msg void OnKeyboard43();
	afx_msg void OnKeyboard44();
	afx_msg void OnKeyboard45();
	afx_msg void OnKeyboard46();
	afx_msg void OnKeyboard47();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALPHAENTRYDIALOG_H__D6B43361_F136_11D3_ABD6_00500466E305__INCLUDED_)
