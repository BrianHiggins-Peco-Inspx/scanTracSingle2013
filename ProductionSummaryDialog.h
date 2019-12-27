#pragma once
#include "afxwin.h"


// CProductionSummaryDialog dialog

class CProductionSummaryDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CProductionSummaryDialog)

public:
	unsigned long int vOldTimeSinceLastStop;
	unsigned long int vOldRejectCountTotal;
	unsigned long int vOldEjectCountTotal;
	unsigned long int vOldTotalUnConfirmed;
	UINT vOldContainerRate;
	UINT vOldBeltRate;
	double vOldPercentEjected;
	CString vOldLastDateString;
	BYTE vOldLastLearnState;
	double vOldPercentRejected;
	unsigned long int vOldTotalContainers;
	bool vOldXRayOnNotice;
	bool vOldYellowMessageButtonYellow;

	unsigned int vUpdatePSDisplayTimerHandle;
	CScanTracDlg *vMainWindowPointer;
	tProductCollection *vLocalProductCollection;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

	CProductionSummaryDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProductionSummaryDialog();

// Dialog Data
	enum { IDD = IDD_ProductionSummaryDialog };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	void UpdateDisplay();
	void DisplayInspectionNames();
	void ShowOrHideCounts();
	void UpdateButtons() ;
	void UpdateClearYellowMessageButton() ;
	void UpdateDisableEjectorsButton();
	void SetupBulkModeAndMainMenuScreen();



	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedFunction1button();
	afx_msg void OnStnClickedFunction2button();
	afx_msg void OnStnClickedFunction3button();
	afx_msg void OnStnClickedFunction4button();
	afx_msg void OnStnClickedSubfunction1button();
	afx_msg void OnStnClickedSubfunction2button();
	afx_msg void OnStnClickedSubfunction3button();
	afx_msg void OnStnClickedSubfunction4button();
	afx_msg void OnStnClickedSubfunction5button();
	afx_msg void OnStnClickedSubfunction6button();
	afx_msg void OnStnClickedSubfunction7button();
	afx_msg void OnStnClickedSubfunction8button();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnStnClickedFunction5button();
	afx_msg void OnStnClickedDialogtitlestatictext1();

	CStatic m_Calibrating;
	CStatic	m_XRaysOn;
	CStatic m_ProductLabel;
	CStatic m_CurrentProductLabel;
	CStatic m_Date;
	CStatic m_Clock;
	CStatic m_DialogTitleStaticText1;
	CStatic	m_StatusLine;
	CStatic	m_EjectorsDisabled;

	CStatic m_BeltRate;
	CStatic m_BeltRateLabel;
	CStatic m_ContainersPerMinute;
	CStatic m_ContainersPerMinuteLabel;
	CStatic	m_TimeSinceLastStopStaticText;
	CStatic	m_TimeSinceLastStopLabel;
	CStatic	m_TotalRejectsPercent;
	CStatic	m_TotalRejectsStaticText;
	CStatic	m_TotalRejectsLabelStaticText;
	CStatic	m_TotalRejectsPercentLabel;
	CStatic	m_ShiftRateCPMLabel;
	CStatic	m_ShiftRateCPM;
	CStatic	m_TotalEjectsLabelStaticTextControl;
	CStatic	m_StopDurationLabel;
	CStatic	m_RunDurationLabel;
	CStatic	m_RunDurationStaticTextControl;
	CStatic	m_ProductionStopsLabel;
	CStatic	m_TotalUnConfirmed;
	CStatic	m_TotalContainersLabelControl;
	CStatic	m_TotalContainersControl;
	CStatic	m_TotalConfirmedLabelControl;
	CStatic	m_TotalConfirmedControl;
	CStatic	m_ShiftLabelControl;
	CStatic	m_ShiftControl;
	CStatic	m_PercentEjected;
	CStatic	m_PercentEjectedLabel;
	CStatic	m_StopDurationStaticTextControl;
	CStatic	m_ProductionStopsCountStaticTextControl;
	CStatic	m_TotalGoodStaticText;
	CStatic	m_TotalGoodLabelStaticText;
	CStatic	m_TotalEjectsStaticText;
	CStatic	m_TotalEjectsStaticTextLabel;

	CStatic	m_MissedColumnLabel;
	CStatic	m_EjectedColumnLabel;
	CStatic	m_EjectCriteriaCountDisabled15;
	CStatic	m_EjectCriteriaCountDisabled14;
	CStatic	m_EjectCriteriaCountDisabled13;
	CStatic	m_EjectCriteriaCountDisabled9;
	CStatic	m_EjectCriteriaCountDisabled8;
	CStatic	m_EjectCriteriaCountDisabled7;
	CStatic	m_EjectCriteriaCountDisabled6;
	CStatic	m_EjectCriteriaCountDisabled5;
	CStatic	m_EjectCriteriaCountDisabled4;
	CStatic	m_EjectCriteriaCountDisabled3;
	CStatic	m_EjectCriteriaCountDisabled2;
	CStatic	m_EjectCriteriaCountDisabled12;
	CStatic	m_EjectCriteriaCountDisabled11;
	CStatic	m_EjectCriteriaCountDisabled10;
	CStatic	m_EjectCriteriaCountDisabled1;
	CStatic	m_EjectCriteriaCountDisabled0;
	CStatic	m_EjectCriteriaCount1;
	CStatic	m_EjectCriteriaCount15;
	CStatic	m_EjectCriteriaCount14;
	CStatic	m_EjectCriteriaCount13;
	CStatic	m_EjectCriteriaCount12;
	CStatic	m_EjectCriteriaCount11;
	CStatic	m_EjectCriteriaCount10;
	CStatic	m_EjectCriteriaCount9;
	CStatic	m_EjectCriteriaCount8;
	CStatic	m_EjectCriteriaCount7;
	CStatic	m_EjectCriteriaCount6;
	CStatic	m_EjectCriteriaCount5;
	CStatic	m_EjectCriteriaCount4;
	CStatic	m_EjectCriteriaCount3;
	CStatic	m_EjectCriteriaCount2;
	CStatic	m_TotalUnConfirmLabel;
	CStatic	m_EjectCriteriaCount0;
	CStatic	m_EjectCriteriaCountLabel15;
	CStatic	m_EjectCriteriaCountLabel12;
	CStatic	m_EjectCriteriaCountLabel14;
	CStatic	m_EjectCriteriaCountLabel13;
	CStatic	m_EjectCriteriaCountLabel11;
	CStatic	m_EjectCriteriaCountLabel10;
	CStatic	m_EjectCriteriaCountLabel9;
	CStatic	m_EjectCriteriaCountLabel8;
	CStatic	m_EjectCriteriaCountLabel7;
	CStatic	m_EjectCriteriaCountLabel6;
	CStatic	m_EjectCriteriaCountLabel5;
	CStatic	m_EjectCriteriaCountLabel4;
	CStatic	m_EjectCriteriaCountLabel3;
	CStatic	m_EjectCriteriaCountLabel2;
	CStatic	m_EjectCriteriaCountLabel1;
	CStatic	m_EjectCriteriaCountLabel0;

	CDownFunctionButton	m_Function1Button;
	CDownFunctionButton	m_Function2Button;
	CDownFunctionButton	m_Function3Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function5Button;

	CLeftFunctionButton	m_SubFunction1Button;
	CLeftFunctionButton	m_SubFunction2Button;
	CLeftFunctionButton	m_SubFunction3Button;
	CLeftFunctionButton	m_SubFunction4Button;
	CRightFunctionButton	m_SubFunction5Button;
	CRightFunctionButton	m_SubFunction6Button;
	CRightFunctionButton	m_SubFunction7Button;
	CRightFunctionButton	m_SubFunction8Button;
	CStatic m_MainBackground;
};
