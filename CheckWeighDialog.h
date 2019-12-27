#if !defined(AFX_CHECKWEIGHDIALOG_H__9A62CC6A_AC8B_478D_9335_457B913866A2__INCLUDED_)
#define AFX_CHECKWEIGHDIALOG_H__9A62CC6A_AC8B_478D_9335_457B913866A2__INCLUDED_

#include "InspectionCheckWeigh.h"
#include "afxwin.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckWeighDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCheckWeighDialog dialog
enum {cRecentDisplay, cTrendDisplay, cTrendMinuteDisplay};

class CCheckWeighDialog : public CDialog
{
public:
	//variables
	CTime vLastDisplayedTime;
	CTime vLastDisplayedTimeOld;

	//variables for data plot display
	bool vStartInCalibrateOffset;
	WORD vPlotMinuteOffsetWhereDataStarts;
	WORD vNumberOfPointsToPlot;
	WORD vOldNumberOfPointsToPlot;
	double *vDisplayDataPointer;
	int vDisplayIndex;
	double vDisplayThresholdLower;
	double vDisplayThresholdUpper;
	double *vPointerToAdjustedData;
	double vPlotMinimumY;
	double vPlotMaximumY;
	double vPlotMinimumX;
	double vPlotMaximumX;

	WORD vBlankLeft;
	WORD vBlankWidth;
	WORD vBlankTop;
	WORD vBlankHeight;
	WORD vGraphLeft;
	WORD vGraphWidth;
	WORD vGraphRight;
	WORD vGraphTop;
	WORD vGraphHeight;

	WORD vDisplayNumberOfPointsToPlot;
	bool vGrowingSoDontBlank;
	WORD vDivideFactor;
	WORD vOldDivideFactor;
	WORD vRangeYToShow;
	double vXScale;
	double vYScale;
	WORD vXInterval;
	WORD vYInterval;
	WORD vXNumberOfScaleLines;
	WORD vYNumberOfScaleLines;
	WORD vXScaleWord;

	int vCheckWeighDataIndexOld;
	int vCheckWeighTrendDataIndexOld;
	int vCheckWeighTrendDataIndexOnMinutesOld;
	double vLastWeightOld;
	DWORD vRecentUnderOld;
	DWORD vRecentOverOld;
	DWORD vTotalUnderOld;
	DWORD vTotalOverOld;
	WORD vCheckWeighDataSizeOld;
	CString vRunStartTimeOld;
	DWORD vNumberOfAlarmsOld;
	BYTE vPlotDisplay;
	BYTE vPlotDisplayOld;
	int vOldCheckWeighTrendDataIndex;
	int vOldCheckWeighDataIndex;
	BYTE vUpdateCountDown;
	unsigned long int vOldTotalContainers;
	CString vWeightAbreviation;
	unsigned long int vRunDurationOld;

	double vDisplayAverage;
	double vDisplayAverageOld;
	double vDisplayStandardDeviation;
	double vDisplayStandardDeviationOld;

	double vMinimumValue;
	double vMaximumValue;
	WORD vLastDisplayedIndex;
	int vOldPasswordLevel;
	bool vDrawHalfTicks;

	COLORREF vPlotBackGroundColor;
	CPen vRedPen;
	CPen vBluePen;
	CPen vDarkBluePen;
	CPen vPlotBackGroundPen;
	CPen vVeryLightBluePen;
  unsigned long int vOneSecondTimerHandle;
	int vOldDataIndex;
	BYTE vCheckWeighInspectionNumber;
	CInspectionCheckWeigh *vCheckWeighInspection;
	CScanTracDlg *vMainWindowPointer;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

	// Construction
	CCheckWeighDialog(CWnd* pParent = NULL);   // standard constructor
	//methods
	void OneSecondUpdate();
	void UpdatePlot();
	void UpdateScreen();
	void SetupDisplays();
	void PreparePlotData();
	BYTE FindInspectionLinkedToMe();
	void PrepareAndExit(BYTE TempCode);
	void DrawCompletelyNewPlot();

// Dialog Data
	//{{AFX_DATA(CCheckWeighDialog)
	enum { IDD = IDD_CheckWeighDialog };
	CRightFunctionButton	m_SubFunction7Button;
	CDownFunctionButton	m_Function3Button;
	CDownFunctionButton	m_Function5Button;
	CStatic	m_StandardDeviationLabel2;
	CStatic	m_AverageLabel2;
	CStatic	m_Background;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckWeighDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCheckWeighDialog)
	afx_msg void OnFunction1Button();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
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
	afx_msg void OnPaint();
	afx_msg void OnDialogtitlestatictext1();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_AlarmIndicator;
	CDownFunctionButton m_Function4Button;
	CStatic m_LastDisplayedLabel;
	CStatic m_LastDisplayed;
	CStatic m_TotalOver;
	CStatic m_TotalOverLabel;
	CStatic m_TotalUnder;
	CStatic m_TotalUnderLabel;
	CStatic m_Count;
	CStatic m_CountLabel;
	CDownFunctionButton m_Function2Button;
	CStatic m_LastLabel;
	CStatic m_MaximumTrendThreshold;
	CStatic m_MinimumTrendThreshold;
	CStatic m_ConversionFormula;
	CStatic m_XScale1;
	CStatic m_XScale10;
	CStatic m_XScale11;
	CStatic m_XScale12;
	CStatic m_XScale13;
	CStatic m_XScale14;
	CStatic m_XScale2;
	CStatic m_XScale3;
	CStatic m_XScale4;
	CStatic m_XScale5;
	CStatic m_XScale6;
	CStatic m_XScale7;
	CStatic m_XScale8;
	CStatic m_XScale9;
	CStatic m_YScale1;
	CStatic m_YScale10;
	CStatic m_YScale11;
	CStatic m_YScale12;
	CStatic m_YScale13;
//	CStatic m_YScale15;
	CStatic m_YScale14;
	CStatic m_YScale15;
	CStatic m_YScale2;
	CStatic m_YScale3;
	CStatic m_YScale4;
	CStatic m_YScale5;
	CStatic m_YScale6;
	CStatic m_YScale7;
	CStatic m_YScale8;
	CStatic m_YScale9;
	CStatic m_XLabel3;
	CStatic m_NumberOfAlarms;
	CStatic m_NumberOfAlarmsLabel;
	CStatic m_Average;
	CStatic m_AverageLabel;
	CDownFunctionButton m_Function1Button;
	CStatic m_Nominal;
	CStatic m_NominalLabel;
	CStatic m_RunTime;
	CStatic m_RunTimeLabel;
	CStatic m_StartTime;
	CStatic m_StartTimeLabel;
	CStatic m_Total;
	CStatic m_TotalLabel;
	CStatic m_DialogTitleStaticText;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKWEIGHDIALOG_H__9A62CC6A_AC8B_478D_9335_457B913866A2__INCLUDED_)
