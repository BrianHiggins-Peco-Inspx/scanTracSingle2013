//ScanTrac Side View Source File
#if !defined(AFX_HELPDIALOG_H__9BC66D53_3E5C_11D4_AC7B_00500466E305__INCLUDED_)
#define AFX_HELPDIALOG_H__9BC66D53_3E5C_11D4_AC7B_00500466E305__INCLUDED_

#include "Inspection.h"
#include "InspectionContaminant.h"
#include "InspectionVoid.h"
#include "InspectionDensity.h"
#include "InspectionStandardDeviation.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HelpDialog.h : header file
//

#define cInspxLogMask 0x8000

/////////////////////////////////////////////////////////////////////////////
// CHelpDialog dialog

const BYTE cFunctions = 3;
const BYTE cHiddenFunctions = 2;
const BYTE cHints = 1;
const BYTE cOverview = 0;
const int cSizeOfVColor = 300000;

enum {cHelpWindow,cProductInformationWindow,cTextWindow,cErrorLogWindow,
	cSourceStrengthWindow, cProductionReportWindow, cRemoteKeyLabelWindow,cMainHelpWindow};

enum {cFindTextNoHighlight,cFindTextHighlight};

class CHelpDialog : public CDialog
{
public:

	//variables
	CFont vFontToUse;
	bool vShowTable;
	bool vMakeRedTextBoldAndUnderLined;
	BYTE vSourceStrengthDisplayMode;
	CString vDialogTitleString;
	CString vACPowerMoniterErrorLogFile;
	//WORD vOldUnderRemoteControl;
	CHARFORMAT vRedCharacterFormat;
	CHARFORMAT2 vBlackCharacterFormat;
	CHARFORMAT vBlueCharacterFormat;
	CHARFORMAT vGreenCharacterFormat;
	CHARFORMAT vYellowCharacterFormat;
	CHARFORMAT vPinkCharacterFormat;
	int *vColor;
	ULONGLONG vProductionReportStartIndex;
	ULONGLONG vProductionReportEndIndex;
	int vProductDataIndex;
	int vHeaderCount;
	int vEndTimeIndex;
	ULONGLONG vLastFileIndex;
	ULONGLONG vLinesInCurrentDayEventLog;
	ULONGLONG vFirstLineNumberOnEventLogScreen;
	ULONGLONG *vLineIndexInFile;
	bool vShowedInspxMessages;
	bool vMakeExitButtonGreen;
	bool vACPowerMoniterFormat;
	bool vMaxEventLogScrollDown;
	bool vMaxEventLogScrollUp;
	bool vShowUpdateButton;
	bool vRecievedButtonPress;	//do not repeat click and wait for mouse button up if you used keyboard down 
	BYTE vMouseState;		//1 - 8 is left mouse button clicked down, 9 - 16 is left mouse button double click, 0 is idle or all functions completed
	unsigned long int vMouseDownRepeatTimerHandle;
	WORD vErrorMessageDisplayMask;
	CString vTitleString;
	CString vEventLogString;
	CString * vEventLogCurrentFiles;
	CString vProductionReportHeaderString;
	DWORD vOldErrorLogCount;
	ULONGLONG vOldACLogFileSize;
	unsigned long int vHideCaretTimerHandle;
	unsigned long int vCheckErrorMessagesTimerHandle;
	unsigned long int vOneSecondTimerHandle;
	unsigned long int vProcessingMessageTimerHandle;
	unsigned long int vUpdateInspxLogDisplay;
	int vLinesOnScreen;
	tProductCollection *vLocalProductCollection;
	CScanTracDlg *vMainWindowPointer;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

	CString vTextString;
	CProduct *vProductPointer;
	BYTE vWindowType;
	HBRUSH vWhiteBrush;
	HBRUSH vBlueBrush;
	HBRUSH vYellowBrush;
	long vHelpContext;
	long vHelpType;
	DWORD *vHistogramData;
	CString vLastFindString;
	LONG vStartFindPosition;
	LONG vLastFindPosition;
	LONG vLastEventLogFindPosition;
	int vFindTextShow;
	bool vStartFindSearch;
	bool vSameLogFile;

	DWORD vDoNotClickUpperYHelp;
	DWORD vDoNotClickLowerYHelp;
	DWORD vDoNotClickLeftXHelp;
	DWORD vDoNotClickRightXHelp;

	// Construction
	CHelpDialog(CWnd* pParent = NULL);   // standard constructor

	//destructor
	virtual ~CHelpDialog();

	//methods
	void OneSecondUpdate();
	void ShowHelp();
	void ShowHelpTitle();
	void ShowHelpText();
	void ShowHelpTextHints();
	void ShowHelpTextOverview();
	void ShowHelpTextHiddenFunctions();
	void ShowHelpTextFunctions();
	void DisplayProductInformation();
	void NewPage();
	void ShowErrorLog(bool TempDoRefresh);
	void ShowEndOfEventLog();
	void ShowPreviousErrorLog();
	void ShowNextErrorLog();
	void UpdateButtons();
	void ShowSourceStrengthData();
	void ShowProductionReport();
	void DisplayColor(CString *TempErrorString);
	void ShiftEventLogUp ( int TempUpDown );
	void ShiftEventLogDown ( int TempDown );
	void ShiftEventLogFindDown ( int TempDown );
	int NumberOfLinesInAllFilesOfDay();
	BOOL FindHelpText(void);
	BOOL FindHelpEventLogText(void);
	BOOL HighlightFindHelpText(CString ChangeTextColor, bool AddHighlight);
//	int GetLines(void);
	void CreateColorDataStructure();
	void ScrollOneLineUp();
	void ScrollOneLineDown();
	void ScrollOnePageUp();
	void ScrollOnePageDown();
	void SetupFont();

// Dialog Data
	//{{AFX_DATA(CHelpDialog)
	enum { IDD = IDD_HelpDialog };
	CRichEditCtrl	m_HelpTextControl;
	CStatic	m_HelpTitle;
	CDownFunctionButton	m_Function2Button;
	CDownFunctionButton	m_Function1Button;
	CDownFunctionButton	m_Function4Button;
	CLeftFunctionButton	m_SubFunction4Button;
	CLeftFunctionButton	m_SubFunction3Button;
	CRightFunctionButton	m_SubFunction8Button;
	CRightFunctionButton	m_SubFunction7Button;
	CRightFunctionButton	m_SubFunction6Button;
	CRightFunctionButton	m_SubFunction5Button;
	CLeftFunctionButton	m_SubFunction1Button;
	CLeftFunctionButton	m_SubFunction2Button;
	CDownFunctionButton	m_Function3Button;
	CDownFunctionButton	m_Function5Button;
	CStatic	m_Background;
	CStatic m_DialogTitleStaticText;
	CStatic m_RemoteKeyLabelsButton;
	CString	m_HelpText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelpDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
		CString TempHoldNextOrPreviousLogString;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHelpDialog)
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
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
	afx_msg void OnDoubleClickSubFunction5Button();
	afx_msg void OnDoubleClickSubFunction6Button();
	afx_msg void OnDoubleClickSubFunction7Button();
	afx_msg void OnDoubleClickSubFunction8Button();
	virtual BOOL DestroyWindow();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELPDIALOG_H__9BC66D53_3E5C_11D4_AC7B_00500466E305__INCLUDED_)
