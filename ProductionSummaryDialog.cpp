// ProductionSummaryDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "ProductionSummaryDialog.h"
#include "afxdialogex.h"
#include "ScanTracDlg.h"
#include "HelpDialog.h"
#include "DisplaySavedRejects.h"

// CProductionSummaryDialog dialog

IMPLEMENT_DYNAMIC(CProductionSummaryDialog, CDialogEx)

extern bool vGlobalEvenSecond;
extern BYTE vGlobalLearnState;
extern unsigned long vGlobalDialogTitleColor;
extern CProduct *vGlobalCurrentProduct;
extern BYTE vGlobalShiftKeyDown;
extern int vGlobalPasswordLevel;

CProductionSummaryDialog::CProductionSummaryDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProductionSummaryDialog::IDD, pParent)
{
	vOldTimeSinceLastStop = 0;
	vOldEjectCountTotal = 0;
	vOldTotalUnConfirmed = 0;
	vOldRejectCountTotal = 0;
	vOldContainerRate = 0;
	vOldBeltRate = 0;
	vOldPercentEjected = 0;
	vOldLastDateString = "-";
	vOldLastLearnState = 0;
	vOldYellowMessageButtonYellow = false;
	vOldPercentRejected = 0;
	vOldTotalContainers = 0;
	vOldXRayOnNotice = false;
	vMainWindowPointer = NULL;
	vLocalProductCollection = NULL;
	vLocalSystemData = NULL;
	vLocalConfigurationData = NULL;

	vUpdatePSDisplayTimerHandle = 37982;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_MainBackground);

	vLocalCWndCollection.Add(&m_BeltRateLabel);
	vLocalCWndCollection.Add(&m_BeltRate);
	vLocalCWndCollection.Add(&m_TimeSinceLastStopStaticText);
	vLocalCWndCollection.Add(&m_TimeSinceLastStopLabel);
	vLocalCWndCollection.Add(&m_ShiftRateCPMLabel);
	vLocalCWndCollection.Add(&m_ShiftRateCPM);
	vLocalCWndCollection.Add(&m_StopDurationLabel);
	vLocalCWndCollection.Add(&m_StopDurationStaticTextControl);//10
	vLocalCWndCollection.Add(&m_ProductionStopsLabel);
	vLocalCWndCollection.Add(&m_ProductionStopsCountStaticTextControl);
	vLocalCWndCollection.Add(&m_RunDurationLabel);
	vLocalCWndCollection.Add(&m_RunDurationStaticTextControl);
	vLocalCWndCollection.Add(&m_MissedColumnLabel);
	vLocalCWndCollection.Add(&m_EjectedColumnLabel);
	vLocalCWndCollection.Add(&m_Calibrating);
	vLocalCWndCollection.Add(&m_XRaysOn);
	vLocalCWndCollection.Add(&m_ContainersPerMinute);//20
	vLocalCWndCollection.Add(&m_ContainersPerMinuteLabel);
	vLocalCWndCollection.Add(&m_TotalContainersLabelControl);
	vLocalCWndCollection.Add(&m_TotalContainersControl);
	vLocalCWndCollection.Add(&m_PercentEjected);
	vLocalCWndCollection.Add(&m_PercentEjectedLabel);

	vLocalCWndCollection.Add(&m_TotalEjectsStaticTextLabel);
	vLocalCWndCollection.Add(&m_TotalEjectsStaticText);

	vLocalCWndCollection.Add(&m_TotalRejectsStaticText);
	vLocalCWndCollection.Add(&m_TotalRejectsLabelStaticText);

	vLocalCWndCollection.Add(&m_TotalRejectsPercent);
	vLocalCWndCollection.Add(&m_TotalRejectsPercentLabel);
	vLocalCWndCollection.Add(&m_TotalUnConfirmed);
	vLocalCWndCollection.Add(&m_TotalUnConfirmLabel);
	vLocalCWndCollection.Add(&m_TotalGoodStaticText);
	vLocalCWndCollection.Add(&m_TotalGoodLabelStaticText);

	vLocalCWndCollection.Add(&m_DialogTitleStaticText1);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountDisabled9);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountDisabled15);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountDisabled14);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountDisabled13);
	vLocalCWndCollection.Add(&m_EjectCriteriaCount15);
	vLocalCWndCollection.Add(&m_EjectCriteriaCount14);
	vLocalCWndCollection.Add(&m_EjectCriteriaCount13);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountLabel15);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountLabel14);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountLabel13);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountDisabled8);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountDisabled7);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountDisabled6);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountDisabled5);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountDisabled4);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountDisabled3);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountDisabled2);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountDisabled12);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountDisabled11);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountDisabled10);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountDisabled1);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountDisabled0);
	vLocalCWndCollection.Add(&m_EjectCriteriaCount12);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountLabel12);
	vLocalCWndCollection.Add(&m_EjectCriteriaCount11);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountLabel11);
	vLocalCWndCollection.Add(&m_EjectCriteriaCount10);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountLabel10);
	vLocalCWndCollection.Add(&m_EjectCriteriaCount0);
	vLocalCWndCollection.Add(&m_EjectCriteriaCount1);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountLabel8);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountLabel7);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountLabel6);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountLabel5);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountLabel4);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountLabel3);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountLabel2);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountLabel1);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountLabel0);
	vLocalCWndCollection.Add(&m_EjectCriteriaCount8);
	vLocalCWndCollection.Add(&m_EjectCriteriaCount7);
	vLocalCWndCollection.Add(&m_EjectCriteriaCount6);
	vLocalCWndCollection.Add(&m_EjectCriteriaCount5);
	vLocalCWndCollection.Add(&m_EjectCriteriaCount4);
	vLocalCWndCollection.Add(&m_EjectCriteriaCount3);
	vLocalCWndCollection.Add(&m_EjectCriteriaCount2);
	vLocalCWndCollection.Add(&m_EjectCriteriaCount9);
	vLocalCWndCollection.Add(&m_EjectCriteriaCountLabel9);

	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);

	vLocalCWndCollection.Add(&m_EjectorsDisabled);
	vLocalCWndCollection.Add(&m_CurrentProductLabel);
	vLocalCWndCollection.Add(&m_ProductLabel);
	vLocalCWndCollection.Add(&m_StatusLine);
	vLocalCWndCollection.Add(&m_Clock);
	vLocalCWndCollection.Add(&m_Date);
}

CProductionSummaryDialog::~CProductionSummaryDialog()
{
}

void CProductionSummaryDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_EjectorsDisabled, m_EjectorsDisabled);
	DDX_Control(pDX, IDC_TotalGoodStaticText, m_TotalGoodStaticText);
	DDX_Control(pDX, IDC_TotalGoodLabelStaticText, m_TotalGoodLabelStaticText);
	DDX_Control(pDX, IDC_TotalEjectsStaticText, m_TotalEjectsStaticText);
	DDX_Control(pDX, IDC_TotalEjectsLabelStaticText, m_TotalEjectsStaticTextLabel);
	DDX_Control(pDX, IDC_PercentEjected, m_PercentEjected);
	DDX_Control(pDX, IDC_PercentEjectedLabel, m_PercentEjectedLabel);
	
	DDX_Control(pDX, IDC_TotalContainers, m_TotalContainersControl);
	DDX_Control(pDX, IDC_TotalContainersLabel, m_TotalContainersLabelControl);
	DDX_Control(pDX, IDC_BeltRate, m_BeltRate);
	DDX_Control(pDX, IDC_BeltRateLabel, m_BeltRateLabel);
	DDX_Control(pDX, IDC_Calibrating, m_Calibrating);
	DDX_Control(pDX, IDC_Clock, m_Clock);
	DDX_Control(pDX, IDC_ContainersPerMinute, m_ContainersPerMinute);
	DDX_Control(pDX, IDC_ContainersPerMinuteLabel, m_ContainersPerMinuteLabel);
	DDX_Control(pDX, IDC_CurrentProductLabel, m_CurrentProductLabel);
	DDX_Control(pDX, IDC_ProductLabel, m_ProductLabel);
	DDX_Control(pDX, IDC_StatusLine, m_StatusLine);

	DDX_Control(pDX, IDC_Date, m_Date);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText1);
	DDX_Control(pDX, IDC_EjectCriteriaCountDisabled15, m_EjectCriteriaCountDisabled15);
	DDX_Control(pDX, IDC_EjectCriteriaCountDisabled14, m_EjectCriteriaCountDisabled14);
	DDX_Control(pDX, IDC_EjectCriteriaCountDisabled13, m_EjectCriteriaCountDisabled13);
	DDX_Control(pDX, IDC_EjectCriteriaCount15, m_EjectCriteriaCount15);
	DDX_Control(pDX, IDC_EjectCriteriaCount14, m_EjectCriteriaCount14);
	DDX_Control(pDX, IDC_EjectCriteriaCount13, m_EjectCriteriaCount13);
	DDX_Control(pDX, IDC_EjectCriteriaCountLabel15, m_EjectCriteriaCountLabel15);
	DDX_Control(pDX, IDC_EjectCriteriaCountLabel14, m_EjectCriteriaCountLabel14);
	DDX_Control(pDX, IDC_EjectCriteriaCountLabel13, m_EjectCriteriaCountLabel13);
	DDX_Control(pDX, IDC_EjectCriteriaCountDisabled9, m_EjectCriteriaCountDisabled9);
	DDX_Control(pDX, IDC_EjectCriteriaCountDisabled8, m_EjectCriteriaCountDisabled8);
	DDX_Control(pDX, IDC_EjectCriteriaCountDisabled7, m_EjectCriteriaCountDisabled7);
	DDX_Control(pDX, IDC_EjectCriteriaCountDisabled6, m_EjectCriteriaCountDisabled6);
	DDX_Control(pDX, IDC_EjectCriteriaCountDisabled5, m_EjectCriteriaCountDisabled5);
	DDX_Control(pDX, IDC_EjectCriteriaCountDisabled4, m_EjectCriteriaCountDisabled4);
	DDX_Control(pDX, IDC_EjectCriteriaCountDisabled3, m_EjectCriteriaCountDisabled3);
	DDX_Control(pDX, IDC_EjectCriteriaCountDisabled2, m_EjectCriteriaCountDisabled2);
	DDX_Control(pDX, IDC_EjectCriteriaCountDisabled12, m_EjectCriteriaCountDisabled12);
	DDX_Control(pDX, IDC_EjectCriteriaCountDisabled11, m_EjectCriteriaCountDisabled11);
	DDX_Control(pDX, IDC_EjectCriteriaCountDisabled10, m_EjectCriteriaCountDisabled10);
	DDX_Control(pDX, IDC_EjectCriteriaCountDisabled1, m_EjectCriteriaCountDisabled1);
	DDX_Control(pDX, IDC_EjectCriteriaCountDisabled0, m_EjectCriteriaCountDisabled0);
	DDX_Control(pDX, IDC_EjectCriteriaCount12, m_EjectCriteriaCount12);
	DDX_Control(pDX, IDC_EjectCriteriaCountLabel12, m_EjectCriteriaCountLabel12);
	DDX_Control(pDX, IDC_EjectCriteriaCount11, m_EjectCriteriaCount11);
	DDX_Control(pDX, IDC_EjectCriteriaCountLabel11, m_EjectCriteriaCountLabel11);
	DDX_Control(pDX, IDC_EjectCriteriaCount10, m_EjectCriteriaCount10);
	DDX_Control(pDX, IDC_EjectCriteriaCountLabel10, m_EjectCriteriaCountLabel10);
	DDX_Control(pDX, IDC_EjectCriteriaCount0, m_EjectCriteriaCount0);
	DDX_Control(pDX, IDC_EjectCriteriaCount1, m_EjectCriteriaCount1);
	DDX_Control(pDX, IDC_EjectCriteriaCountLabel8, m_EjectCriteriaCountLabel8);
	DDX_Control(pDX, IDC_EjectCriteriaCountLabel7, m_EjectCriteriaCountLabel7);
	DDX_Control(pDX, IDC_EjectCriteriaCountLabel6, m_EjectCriteriaCountLabel6);
	DDX_Control(pDX, IDC_EjectCriteriaCountLabel5, m_EjectCriteriaCountLabel5);
	DDX_Control(pDX, IDC_EjectCriteriaCountLabel4, m_EjectCriteriaCountLabel4);
	DDX_Control(pDX, IDC_EjectCriteriaCountLabel3, m_EjectCriteriaCountLabel3);
	DDX_Control(pDX, IDC_EjectCriteriaCountLabel2, m_EjectCriteriaCountLabel2);
	DDX_Control(pDX, IDC_EjectCriteriaCountLabel1, m_EjectCriteriaCountLabel1);
	DDX_Control(pDX, IDC_EjectCriteriaCountLabel0, m_EjectCriteriaCountLabel0);
	DDX_Control(pDX, IDC_EjectCriteriaCount8, m_EjectCriteriaCount8);
	DDX_Control(pDX, IDC_EjectCriteriaCount7, m_EjectCriteriaCount7);
	DDX_Control(pDX, IDC_EjectCriteriaCount6, m_EjectCriteriaCount6);
	DDX_Control(pDX, IDC_EjectCriteriaCount5, m_EjectCriteriaCount5);
	DDX_Control(pDX, IDC_EjectCriteriaCount4, m_EjectCriteriaCount4);
	DDX_Control(pDX, IDC_EjectCriteriaCount3, m_EjectCriteriaCount3);
	DDX_Control(pDX, IDC_EjectCriteriaCount2, m_EjectCriteriaCount2);
	DDX_Control(pDX, IDC_EjectCriteriaCount9, m_EjectCriteriaCount9);
	DDX_Control(pDX, IDC_EjectCriteriaCountLabel9, m_EjectCriteriaCountLabel9);
	DDX_Control(pDX, IDC_MissedColumnLabel, m_MissedColumnLabel);
	DDX_Control(pDX, IDC_EjectedColumnLabel, m_EjectedColumnLabel);

	DDX_Control(pDX, IDC_TimeSinceLastStopStaticText, m_TimeSinceLastStopStaticText);
	DDX_Control(pDX, IDC_TimeSinceLastStopLabel, m_TimeSinceLastStopLabel);
	DDX_Control(pDX, IDC_StopDurationLabel, m_StopDurationLabel);
	DDX_Control(pDX, IDC_StopDurationCountStaticText, m_StopDurationStaticTextControl);
	DDX_Control(pDX, IDC_ProductionStopsLabel, m_ProductionStopsLabel);
	DDX_Control(pDX, IDC_ProductionStopsCountStaticText, m_ProductionStopsCountStaticTextControl);
	DDX_Control(pDX, IDC_RunDurationLabel, m_RunDurationLabel);
	DDX_Control(pDX, IDC_TotalUnConfirmLabel, m_TotalUnConfirmLabel);
	DDX_Control(pDX, IDC_ShiftRateCPMLabel, m_ShiftRateCPMLabel);
	DDX_Control(pDX, IDC_ShiftRateCPM, m_ShiftRateCPM);
	DDX_Control(pDX, IDC_RunDurationStaticText, m_RunDurationStaticTextControl);
	DDX_Control(pDX, IDC_TotalUnConfirmed, m_TotalUnConfirmed);
	DDX_Control(pDX, IDC_TotalRejectsStaticText, m_TotalRejectsStaticText);
	DDX_Control(pDX, IDC_TotalRejectsLabelStaticText, m_TotalRejectsLabelStaticText);
	DDX_Control(pDX, IDC_TotalRejectsPercent, m_TotalRejectsPercent);
	DDX_Control(pDX, IDC_TotalRejectsPercentLabel, m_TotalRejectsPercentLabel);

	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_MainBackground, m_MainBackground);
	DDX_Control(pDX, IDC_Calibrating, m_Calibrating);
	DDX_Control(pDX, IDC_XRaysOn, m_XRaysOn);
}

BEGIN_MESSAGE_MAP(CProductionSummaryDialog, CDialogEx)
	ON_STN_CLICKED(IDC_Function1Button, &CProductionSummaryDialog::OnStnClickedFunction1button)
	ON_STN_CLICKED(IDC_Function2Button, &CProductionSummaryDialog::OnStnClickedFunction2button)
	ON_STN_CLICKED(IDC_Function3Button, &CProductionSummaryDialog::OnStnClickedFunction3button)
	ON_STN_CLICKED(IDC_Function4Button, &CProductionSummaryDialog::OnStnClickedFunction4button)
	ON_STN_CLICKED(IDC_Function5Button, &CProductionSummaryDialog::OnStnClickedFunction5button)
	ON_STN_CLICKED(IDC_SubFunction1Button, &CProductionSummaryDialog::OnStnClickedSubfunction1button)
	ON_STN_CLICKED(IDC_SubFunction2Button, &CProductionSummaryDialog::OnStnClickedSubfunction2button)
	ON_STN_CLICKED(IDC_SubFunction3Button, &CProductionSummaryDialog::OnStnClickedSubfunction3button)
	ON_STN_CLICKED(IDC_SubFunction4Button, &CProductionSummaryDialog::OnStnClickedSubfunction4button)
	ON_STN_CLICKED(IDC_SubFunction5Button, &CProductionSummaryDialog::OnStnClickedSubfunction5button)
	ON_STN_CLICKED(IDC_SubFunction6Button, &CProductionSummaryDialog::OnStnClickedSubfunction6button)
	ON_STN_CLICKED(IDC_SubFunction7Button, &CProductionSummaryDialog::OnStnClickedSubfunction7button)
	ON_STN_CLICKED(IDC_SubFunction8Button, &CProductionSummaryDialog::OnStnClickedSubfunction8button)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CProductionSummaryDialog::OnStnClickedDialogtitlestatictext1)
END_MESSAGE_MAP()

void CProductionSummaryDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	m_MainBackground.SetWindowPos(NULL,cMainBackgroundLeft, cMainBackgroundTop,cMainBackgroundLength, cMainBackgroundHeight, SWP_NOZORDER);

	SetDlgItemText(IDC_EjectCriteriaCountLabel0, _T(""));
	SetDlgItemText(IDC_EjectCriteriaCountLabel1, _T(""));
	SetDlgItemText(IDC_EjectCriteriaCountLabel2, _T(""));
	SetDlgItemText(IDC_EjectCriteriaCountLabel3, _T(""));
	SetDlgItemText(IDC_EjectCriteriaCountLabel4, _T(""));
	SetDlgItemText(IDC_EjectCriteriaCountLabel5, _T(""));
	SetDlgItemText(IDC_EjectCriteriaCountLabel6, _T(""));
	SetDlgItemText(IDC_EjectCriteriaCountLabel7, _T(""));
	SetDlgItemText(IDC_EjectCriteriaCountLabel8, _T(""));
	SetDlgItemText(IDC_EjectCriteriaCountLabel9, _T(""));
	SetDlgItemText(IDC_EjectCriteriaCountLabel10, _T(""));
	SetDlgItemText(IDC_EjectCriteriaCountLabel11, _T(""));
	SetDlgItemText(IDC_EjectCriteriaCountLabel12, _T(""));
	SetDlgItemText(IDC_EjectCriteriaCountLabel13, _T(""));
	SetDlgItemText(IDC_EjectCriteriaCountLabel14, _T(""));
	SetDlgItemText(IDC_EjectCriteriaCountLabel15, _T(""));

	vLocalSystemData->vOldShiftRateCPM = 0;
	vLocalSystemData->vOldRunDuration = 0;
	vLocalSystemData->vOldProductionStopsCount = 0;
	vLocalSystemData->vOldStopDuration = 0;
	vLocalSystemData->vOldTimeSinceLastStop = 0;
	vLocalSystemData->vOldGoodCountTotal = 0;
	for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfCountsDisplayed; TempLoop++)
	{
		vLocalSystemData->vOldXRayCriteriaCount[TempLoop] = 0;
		vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop] = 0;
	}

	DisplayInspectionNames();
	ShowOrHideCounts();

	CString TempText = " ";

	TempText.LoadString(IDS_CurrentProduct);
	SetDlgItemText(IDC_CurrentProductLabel,TempText + ":");
	if (vGlobalCurrentProduct == NULL)
	{
		TempText.LoadString(IDS_NoCurrentProduct);
		vLocalSystemData->vCurrentProductName = " " + TempText + " ";
		SetDlgItemText(IDC_ProductLabel,vLocalSystemData->vCurrentProductName);
	}
	else
	{
		CString TempString3 = " ";
		TempString3 = TempString3 + vLocalSystemData->vCurrentProductName + TempString3;
		if (vLocalConfigurationData->vAskForProductionReportProductCode)
			TempString3 = TempString3 + "(" + vLocalSystemData->vShiftProductCode + ") ";
		SetDlgItemText(IDC_ProductLabel,TempString3);
	}

	CString TempText2 = "-";
	TempText2.LoadString(IDS_LastStop);
	SetDlgItemText(IDC_TimeSinceLastStopLabel,TempText2 + ":");

	m_MissedColumnLabel.ShowWindow(SW_HIDE);
	m_EjectedColumnLabel.ShowWindow(SW_HIDE);

	TempText2.LoadString(IDS_ProductionSummary);
	SetDlgItemText(IDC_DialogTitleStaticText1, TempText2);

	TempText.LoadString(IDS_ContainersPerMin);
	TempText = TempText + ":";
	SetDlgItemText(IDC_ContainersPerMinuteLabel, TempText);

	TempText.LoadString(IDS_ConveyorRateFtMin);
	TempText = TempText + ":";

	TempText.LoadString(IDS_RunDuration);
	TempText = TempText + ":";
	SetDlgItemText(IDC_RunDurationLabel, TempText);

	TempText.LoadString(IDS_ProductionStops);
	TempText = TempText + ":";
	SetDlgItemText(IDC_ProductionStopsLabel, TempText);

	TempText.LoadString(IDS_StopDuration);
	TempText = TempText + ":";
	SetDlgItemText(IDC_StopDurationLabel, TempText);

	TempText.LoadString(IDS_TotalEjected);
	TempText = TempText + ":";
	SetDlgItemText(IDC_TotalEjectsLabelStaticText, TempText);
	TempText2.LoadString(IDS_Rejected);
	SetDlgItemText(IDC_TotalRejectsPercentLabel, "% " + TempText2 + ":");
	TempText2.LoadString(IDS_Unconfirmed);
	SetDlgItemText(IDC_TotalUnConfirmLabel,TempText2 + ":");
	TempText2.LoadString(IDS_TotalRejects);
	SetDlgItemText(IDC_TotalRejectsLabelStaticText,TempText2 + ":");
	
	TempText2.LoadString(IDS_Stop);
	SetDlgItemText(IDC_SubFunction1Button,TempText2);

	TempText2.LoadString(IDS_TotalGood);
	SetDlgItemText(IDC_TotalGoodLabelStaticText,TempText2 + ":");

	TempText.LoadString(IDS_ConveyorRateFtMin);
	SetDlgItemText(IDC_BeltRateLabel,TempText + ":");

	m_Function1Button.ShowWindow(SW_SHOW);
	TempText.LoadString(IDS_MainMenu);
	SetDlgItemText(IDC_Function1Button,TempText);

	TempText.LoadString(IDS_ShowProductionReport);
	SetDlgItemText(IDC_SubFunction3Button,TempText);

	TempText.LoadString(IDS_ClearYellowMessage);
	SetDlgItemText(IDC_SubFunction6Button,TempText);

	TempText.LoadString(IDS_ShowEventLog);
	SetDlgItemText(IDC_SubFunction8Button,TempText);

	TempText.LoadString(IDS_EjectorsDisabled);
	SetDlgItemText(IDC_EjectorsDisabled,TempText);

	TempText.LoadString(IDS_XRAYSON);
	SetDlgItemText(IDC_Calibrating,TempText);
	SetDlgItemText(IDC_XRaysOn,TempText);

	m_Function2Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());

	if (vMainWindowPointer->vNumberOfWeighInspections)
	{
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_Function2Button.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_CheckWeigh);
		SetDlgItemText(IDC_Function2Button, TempText);
	}
	else
	{
		m_Function2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());
	}

	if (vLocalConfigurationData->vWriteRejectsToFiles)
	{
		m_Function4Button.ShowWindow(SW_SHOW);
	}
	else
	{
		m_Function4Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());
	}
	m_Function4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);

	if (vLocalConfigurationData->vSaveAllImagesToRejectsFolder)
		TempText.LoadString(IDS_SavedImages);
	else
		TempText.LoadString(IDS_SavedRejects);

	SetDlgItemText(IDC_Function4Button, TempText);

	if (vLocalSystemData->vTotalContainers)
	{
		m_Function5Button.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_ResetCounters);
		SetDlgItemText(IDC_Function5Button,TempText);
	}
	else
	{
		m_Function5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
	}

	//subfunction1 and 2 do not change from the displays, so leave alone

	TempText.LoadString(IDS_ProductSetupSummary);
	SetDlgItemText(IDC_SubFunction5Button,TempText);

	m_SubFunction7Button.ShowWindow(SW_SHOW);
	TempText.LoadString(IDS_ShowRejectLog);
	SetDlgItemText(IDC_SubFunction7Button,TempText);

	//subfunction 8 button updated below by subroutine UpdateSubFunction8Button

	if ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && ((vGlobalCurrentProduct) && (!vGlobalCurrentProduct->vBulkProductMode))) //Not Continuous Feed, and Not a Pipeline
	{
		m_RunDurationStaticTextControl.ShowWindow(SW_SHOW);
		m_ProductionStopsLabel.ShowWindow(SW_SHOW);
		m_StopDurationLabel.ShowWindow(SW_SHOW);
		m_TimeSinceLastStopLabel.ShowWindow(SW_SHOW);
		m_RunDurationLabel.ShowWindow(SW_SHOW);
		m_ContainersPerMinute.ShowWindow(SW_SHOW);
		m_ContainersPerMinuteLabel.ShowWindow(SW_SHOW);
		m_ShiftRateCPM.ShowWindow(SW_SHOW);
		m_BeltRateLabel.ShowWindow(SW_SHOW);
		m_BeltRate.ShowWindow(SW_SHOW);
		m_ProductionStopsCountStaticTextControl.ShowWindow(SW_SHOW);
		m_StopDurationStaticTextControl.ShowWindow(SW_SHOW);
		m_TimeSinceLastStopStaticText.ShowWindow(SW_SHOW);

		TempText2.LoadString(IDS_ShiftRateCPM);
		SetDlgItemText(IDC_ShiftRateCPMLabel,TempText2 + ":");
		m_ShiftRateCPMLabel.ShowWindow(SW_SHOW);

		m_TotalRejectsPercent.ShowWindow(SW_SHOW);
		m_TotalRejectsPercentLabel.ShowWindow(SW_SHOW);

		m_TotalGoodStaticText.ShowWindow(SW_SHOW);
		m_TotalGoodLabelStaticText.ShowWindow(SW_SHOW);
	}
	else
	{
		m_ContainersPerMinute.ShowWindow(SW_HIDE); //hide on pipe lines if not burning in or conditioning a tube
		m_ContainersPerMinuteLabel.ShowWindow(SW_HIDE);
		m_RunDurationStaticTextControl.ShowWindow(SW_HIDE);
		m_ProductionStopsLabel.ShowWindow(SW_HIDE);
		m_StopDurationLabel.ShowWindow(SW_HIDE);
		m_TimeSinceLastStopLabel.ShowWindow(SW_HIDE);
		m_RunDurationLabel.ShowWindow(SW_HIDE);
		m_ShiftRateCPM.ShowWindow(SW_HIDE);
		m_BeltRateLabel.ShowWindow(SW_HIDE);
		m_BeltRate.ShowWindow(SW_HIDE);
		m_ProductionStopsCountStaticTextControl.ShowWindow(SW_HIDE);
		m_StopDurationStaticTextControl.ShowWindow(SW_HIDE);
		m_TimeSinceLastStopStaticText.ShowWindow(SW_HIDE);

		m_TotalRejectsPercent.ShowWindow(SW_HIDE);
		m_TotalRejectsPercentLabel.ShowWindow(SW_HIDE);

		m_TotalGoodStaticText.ShowWindow(SW_HIDE);
		m_TotalGoodLabelStaticText.ShowWindow(SW_HIDE);
	}
	m_TotalContainersControl.ShowWindow(SW_SHOW);
	m_TotalContainersLabelControl.ShowWindow(SW_SHOW);

	m_TotalEjectsStaticText.ShowWindow(SW_SHOW);
	m_TotalEjectsStaticTextLabel.ShowWindow(SW_SHOW);

	if (!vLocalSystemData->vTotalUnConfirmed)
	{
		m_TotalUnConfirmed.ShowWindow(SW_HIDE);
		m_TotalUnConfirmLabel.ShowWindow(SW_HIDE);
	}
	else
	{
		m_TotalUnConfirmed.ShowWindow(SW_SHOW);
		m_TotalUnConfirmLabel.ShowWindow(SW_SHOW);
	}


	m_TotalRejectsStaticText.ShowWindow(SW_SHOW);
	m_TotalRejectsLabelStaticText.ShowWindow(SW_SHOW);

	// TODO: Add your message handler code here
	UpdateButtons();
	this->SetWindowText(_T("Production Summary"));
	SetupBulkModeAndMainMenuScreen();
	UpdateDisplay();

	int TempTimerResult = SetTimer(vUpdatePSDisplayTimerHandle,1000,NULL);
	if (!TempTimerResult)
		ReportErrorMessage("Error-Display Timer Failed",cEMailInspx,32000);
}

// CProductionSummaryDialog message handlers


void CProductionSummaryDialog::OnStnClickedFunction1button()
{
	//Main Menu Button pressed
	ReportErrorMessage("Operator pressed Main Menu Button", cUserAction,0);
	CDialog::EndDialog(1);
}


void CProductionSummaryDialog::OnStnClickedFunction2button()
{
	vMainWindowPointer->StartCheckWeighingMenu(false);
}


void CProductionSummaryDialog::OnStnClickedFunction3button()
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	{ //show Inspx log button
		ReportErrorMessage("Operator pressed Inspx Log Button", cUserAction,0);
		CHelpDialog IHelpDialog;
		IHelpDialog.vHelpContext = 21;
		IHelpDialog.vHelpType = 0;
		IHelpDialog.vWindowType = cErrorLogWindow;
		IHelpDialog.vErrorMessageDisplayMask = cInspxLogMask; //show  the Inspx log
		IHelpDialog.vLocalProductCollection = vLocalProductCollection;
		IHelpDialog.vLocalSystemData = vLocalSystemData;
		IHelpDialog.vMainWindowPointer = vMainWindowPointer;
		IHelpDialog.vLocalConfigurationData = vLocalConfigurationData;
		IHelpDialog.vProductPointer = vGlobalCurrentProduct;
		int TempResult = IHelpDialog.DoModal();
		//9/30/2009
		vLocalSystemData->vInspxMessagesToView = false;
		CDialog::EndDialog(1);
	}
	else
		vMainWindowPointer->ShowThresholds();
}


void CProductionSummaryDialog::OnStnClickedFunction4button()
{
	if (vLocalConfigurationData->vWriteRejectsToFiles)
	{
		ReportErrorMessage("Operator pressed Saved Rejects Button", cUserAction,0);

		CDisplaySavedRejects IDisplaySavedRejects;  
		IDisplaySavedRejects.vLocalSystemData = vLocalSystemData;
		IDisplaySavedRejects.vLocalConfigurationData = vLocalConfigurationData;
		int nResponse = IDisplaySavedRejects.DoModal();
		//if (nResponse == 10) //only response possible
		{
			ReportErrorMessage("Operator Exited Saved Rejects Button", cWriteToLog,0);
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(1);
		}
	}
}

void CProductionSummaryDialog::OnStnClickedFunction5button()
{
	if ((vLocalSystemData->vTotalContainers) || (vLocalConfigurationData->vSimulateLightControlBoard))
	{
		if (PasswordOK(cResetPassword, true))
		{
			ReportErrorMessage("Operator pressed Reset Counts Button", cUserAction,0);
			vMainWindowPointer->AskIfShouldResetCounters(false, cResetCountersProductionReport);
			CDialog::EndDialog(1);
		}
	}
	else
	if (PasswordOK(cResetPassword, true))
	{
		ReportErrorMessage("Operator pressed Reset Counts Button-No Counts", cUserAction,0);
		vMainWindowPointer->ResetCounters();
	}
	if (!vLocalSystemData->vTotalContainers) //if was zero, now value, show reset counters
		m_Function5Button.ShowWindow(SW_HIDE);
}


void CProductionSummaryDialog::OnStnClickedSubfunction1button()
{
	vMainWindowPointer->OnSubFunction1Button();
}


void CProductionSummaryDialog::OnStnClickedSubfunction2button()
{
	vMainWindowPointer->OnSubFunction2Button();
	UpdateButtons();
}


void CProductionSummaryDialog::OnStnClickedSubfunction3button()
{
	ReportErrorMessage("Operator pressed Production Report Button", cUserChanges,0);
	CHelpDialog IHelpDialog;
	IHelpDialog.vHelpContext = 53;
	IHelpDialog.vHelpType = 0;
	IHelpDialog.vWindowType = cProductionReportWindow;
	IHelpDialog.vErrorMessageDisplayMask = vLocalConfigurationData->vErrorMessageDisplayMask;
	IHelpDialog.vLocalProductCollection = vLocalProductCollection;
	IHelpDialog.vLocalSystemData = vLocalSystemData;
	IHelpDialog.vMainWindowPointer = vMainWindowPointer;
	IHelpDialog.vLocalConfigurationData = vLocalConfigurationData;
	IHelpDialog.vProductPointer = vGlobalCurrentProduct;
	int TempResult = IHelpDialog.DoModal();
	CDialog::EndDialog(1);
}


void CProductionSummaryDialog::OnStnClickedSubfunction4button()
{
	// TODO: Add your control notification handler code here
}


void CProductionSummaryDialog::OnStnClickedSubfunction5button()
{
	ReportErrorMessage("Operator pressed Product Setup Summary Button", cUserAction,0);
	if (!vGlobalCurrentProduct)
		vMainWindowPointer->SelectCurrentProduct();
	if (vGlobalCurrentProduct)
	{
		CHelpDialog IHelpDialog;
		IHelpDialog.vHelpContext = 21;
		IHelpDialog.vHelpType = 0;
		IHelpDialog.vWindowType = cProductInformationWindow;
		IHelpDialog.vLocalProductCollection = vLocalProductCollection;
		IHelpDialog.vLocalSystemData = vLocalSystemData;
		IHelpDialog.vMainWindowPointer = vMainWindowPointer;
		IHelpDialog.vLocalConfigurationData = vLocalConfigurationData;
		IHelpDialog.vProductPointer = vGlobalCurrentProduct;
		int TempResult = IHelpDialog.DoModal();
	}
	CDialog::EndDialog(1);
}


void CProductionSummaryDialog::OnStnClickedSubfunction6button()
{
	if (vOldYellowMessageButtonYellow)
		vMainWindowPointer->OnSubFunction6Button();
	else
	if (PasswordOK(cTemporaryInspxPassword,false))
	{  //show system setup summary
		ReportErrorMessage("Operator pressed System Summary Button", cUserAction,0);
		CHelpDialog IHelpDialog;
		IHelpDialog.vHelpContext = 21;
		IHelpDialog.vHelpType = 0;
		IHelpDialog.vWindowType = cProductInformationWindow;
		IHelpDialog.vLocalProductCollection = vLocalProductCollection;
		IHelpDialog.vLocalSystemData = vLocalSystemData;
		IHelpDialog.vMainWindowPointer = vMainWindowPointer;
		IHelpDialog.vLocalConfigurationData = vLocalConfigurationData;
		IHelpDialog.vProductPointer = NULL;
		int TempResult = IHelpDialog.DoModal();
		CDialog::EndDialog(1);
	}
}


void CProductionSummaryDialog::OnStnClickedSubfunction7button()
{
	//show reject log button
	if (PasswordOK(cOperatorPassword,true))
	{
		//9/11/2009
		if (PasswordOK(cTemporaryInspxPassword,false))
		{//must clear inspx password if in or will show lots of log events
			PasswordOK(cNoPassword, false);
			vMainWindowPointer->vPasswordEntryString = "";
			PostThreadMessage(vGlobalCallingThreadID,cUpdateTitleBarMessage, 0, 0);
		}

		ReportErrorMessage("Operator pressed Reject Log Button", cUserAction,0);
		CHelpDialog IHelpDialog;
		IHelpDialog.vHelpContext = 21;
		IHelpDialog.vHelpType = 0;
		IHelpDialog.vWindowType = cErrorLogWindow;
		IHelpDialog.vErrorMessageDisplayMask = 0x80; //show only the rejects
		IHelpDialog.vLocalProductCollection = vLocalProductCollection;
		IHelpDialog.vLocalSystemData = vLocalSystemData;
		IHelpDialog.vMainWindowPointer = vMainWindowPointer;
		IHelpDialog.vLocalConfigurationData = vLocalConfigurationData;
		IHelpDialog.vProductPointer = vGlobalCurrentProduct;
		int TempResult = IHelpDialog.DoModal();
		if (IHelpDialog.vShowedInspxMessages)
			vLocalSystemData->vInspxMessagesToView = false;

		CDialog::EndDialog(1);
	}
}


void CProductionSummaryDialog::OnStnClickedSubfunction8button()
{
	//show event log button
	if (PasswordOK(cOperatorPassword,true))
	{
		ReportErrorMessage("Operator pressed Event Log Button", cUserAction,0);
		CHelpDialog IHelpDialog;
		IHelpDialog.vHelpContext = 21;
		IHelpDialog.vHelpType = 0;
		IHelpDialog.vWindowType = cErrorLogWindow;
		IHelpDialog.vErrorMessageDisplayMask = vLocalConfigurationData->vErrorMessageDisplayMask;
		IHelpDialog.vLocalProductCollection = vLocalProductCollection;
		IHelpDialog.vLocalSystemData = vLocalSystemData;
		IHelpDialog.vMainWindowPointer = vMainWindowPointer;
		IHelpDialog.vLocalConfigurationData = vLocalConfigurationData;
		IHelpDialog.vProductPointer = vGlobalCurrentProduct;
		int TempResult = IHelpDialog.DoModal();
		if (IHelpDialog.vShowedInspxMessages)
			//9/30/2009
			vLocalSystemData->vInspxMessagesToView = false;

		vLocalConfigurationData->vErrorMessageDisplayMask = IHelpDialog.vErrorMessageDisplayMask;
		CDialog::EndDialog(1);
	}
}


HBRUSH CProductionSummaryDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	int TempControlID = pWnd->GetDlgCtrlID();
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
 
	if (TempControlID == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText1, 8);  //5 is medium large

		pDC->SetBkMode(TRANSPARENT);
		if (vGlobalShiftKeyDown)
			pDC->SetTextColor(cBlack);
		else
			pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (TempControlID == IDC_MainBackground)
	{
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID ==IDC_CurrentProductLabel)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentProductLabel, 4);  //5 is medium large
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_ProductLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ProductLabel, 4);  //5 is medium large
		pDC->SetTextColor(cDarkBlue);
		if (!vGlobalCurrentProduct)
		{
			pDC->SetBkColor(cYellow);
			return vLocalSystemData->vLightBlueBrush;
		}
		else
			return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_StatusLine)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_StatusLine, 5);  //5 is medium large
		if (vLocalSystemData->vYellowMessageButtonYellow)
		{
			pDC->SetTextColor(cDarkBlue);
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vYellowBrush;
		}
		else
		if ((vGlobalLearnState) && (vGlobalLearnState != 255)) //colors for Improving Learn - Do not test contaminants indicator
		{
			pDC->SetTextColor(cWhite);
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vDarkBlueBrush;
		}
		else
		{
			pDC->SetTextColor(cBlack);
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vLightBlueBrush;
		}
			return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_EjectorsDisabled)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectorsDisabled, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		if (vGlobalEvenSecond)
			pDC->SetTextColor(cDarkBlue);
		else
			pDC->SetTextColor(cYellow);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vYellowBrush;
	}

	if (TempControlID == IDC_XRaysOn)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaysOn, 8);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetTextColor(cYellow);
		return vLocalSystemData->vRedBrush;
	}

	if (TempControlID == IDC_Calibrating)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Calibrating, 7);  //5 is medium large
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetTextColor(cYellow);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vRedBrush;
	}
	
	if (TempControlID == IDC_TotalContainersLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TotalContainersLabelControl, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_TotalContainers)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TotalContainersControl, 4);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_PercentEjected)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_PercentEjected, 4);  //5 is medium large
		
		pDC->SetTextColor(cDarkBlue);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_PercentEjectedLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_PercentEjectedLabel, 4);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_TotalEjectsLabelStaticText)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TotalEjectsStaticTextLabel, 4);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_TotalEjectsStaticText)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TotalEjectsStaticText, 4);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_TotalRejectsStaticText)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TotalRejectsStaticText, 4);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		if (vOldRejectCountTotal)
			pDC->SetTextColor(cRed);
		else
			pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_TotalRejectsLabelStaticText)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TotalRejectsLabelStaticText, 4);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_TotalRejectsPercent)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TotalRejectsPercent, 4);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_TotalRejectsPercentLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TotalRejectsPercentLabel, 4);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_TotalUnConfirmed)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TotalUnConfirmed, 4);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		if (vLocalSystemData->vOldTotalUnConfirmed)
			pDC->SetTextColor(cRed);
		else
			pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_TotalUnConfirmLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TotalUnConfirmLabel, 4);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_TotalGoodStaticText)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TotalGoodStaticText, 4);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_TotalGoodLabelStaticText)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TotalGoodLabelStaticText, 4);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_BeltRate)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_BeltRate, 4);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_BeltRateLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_BeltRateLabel, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_ContainersPerMinute)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ContainersPerMinute, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		if ((vLocalSystemData->vSystemRunMode == cConditionXRayTubeMode) && (vLocalSystemData->vConditionString == "Burn-in"))
		{
			if (vLocalSystemData->vXRayTubeConditionStepTimeLeft < 3600 * 48) //if less than 24 hours of burn in
				return vLocalSystemData->vYellowBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
		else
		if ((vMainWindowPointer->vCPMNotRunning) && (vLocalSystemData->vSystemRunMode != cAutoSetupRunningSystem) && (vLocalSystemData->vSystemRunMode != cRunningSystemMode))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cRed);
			return vLocalSystemData->vYellowBrush;
		}
		else
			return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_ContainersPerMinuteLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ContainersPerMinuteLabel, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_ShiftRateCPM)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ShiftRateCPM, 4);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_ShiftRateCPMLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ShiftRateCPMLabel, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID ==IDC_RunDurationLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_RunDurationLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID ==IDC_RunDurationStaticText) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_RunDurationStaticTextControl, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID ==IDC_ProductionStopsLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ProductionStopsLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		return vLocalSystemData->vLightBlueBrush;
	}
	if (TempControlID ==IDC_ProductionStopsCountStaticText)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ProductionStopsCountStaticTextControl, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBlueBrush;
	}
	if (TempControlID ==IDC_StopDurationLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_StopDurationLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		return vLocalSystemData->vLightBlueBrush;
	}
	if (TempControlID ==IDC_StopDurationCountStaticText)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_StopDurationStaticTextControl, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_StopDurationCountStaticText) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_StopDurationStaticTextControl, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_TimeSinceLastStopLabel)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TimeSinceLastStopLabel, 4);  //5 is medium large
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID ==IDC_TimeSinceLastStopStaticText) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TimeSinceLastStopStaticText, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBlueBrush;
	}
	if (TempControlID ==IDC_MissedColumnLabel)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MissedColumnLabel, 4);  //5 is medium large
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID ==IDC_EjectedColumnLabel)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectedColumnLabel, 4);  //5 is medium large
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_Clock) 
	{	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Clock, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBlueBrush;
	}
	if (TempControlID == IDC_Date)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Date, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBlueBrush;
	}

	if ((TempControlID ==IDC_EjectCriteriaCount0) ||
		(TempControlID ==IDC_EjectCriteriaCount1) ||
		(TempControlID ==IDC_EjectCriteriaCount2) ||
		(TempControlID ==IDC_EjectCriteriaCount3) ||
		(TempControlID ==IDC_EjectCriteriaCount4) ||
		(TempControlID ==IDC_EjectCriteriaCount5) ||
		(TempControlID ==IDC_EjectCriteriaCount6) ||
		(TempControlID ==IDC_EjectCriteriaCount7) ||
		(TempControlID ==IDC_EjectCriteriaCount8) ||
		(TempControlID ==IDC_EjectCriteriaCount9) ||
		(TempControlID ==IDC_EjectCriteriaCount10) ||
		(TempControlID ==IDC_EjectCriteriaCount11) ||
		(TempControlID ==IDC_EjectCriteriaCount12) ||
		(TempControlID ==IDC_EjectCriteriaCount13) ||
		(TempControlID ==IDC_EjectCriteriaCount14) ||
		(TempControlID ==IDC_EjectCriteriaCount15))
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectCriteriaCount0, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBlueBrush;
	}

	if ((TempControlID ==IDC_EjectCriteriaCountDisabled0) ||
		(TempControlID ==IDC_EjectCriteriaCountDisabled1) ||
		(TempControlID ==IDC_EjectCriteriaCountDisabled2) ||
		(TempControlID ==IDC_EjectCriteriaCountDisabled3) ||
		(TempControlID ==IDC_EjectCriteriaCountDisabled4) ||
		(TempControlID ==IDC_EjectCriteriaCountDisabled5) ||
		(TempControlID ==IDC_EjectCriteriaCountDisabled6) ||
		(TempControlID ==IDC_EjectCriteriaCountDisabled7) ||
		(TempControlID ==IDC_EjectCriteriaCountDisabled8) ||
		(TempControlID ==IDC_EjectCriteriaCountDisabled9) ||
		(TempControlID ==IDC_EjectCriteriaCountDisabled10) ||
		(TempControlID ==IDC_EjectCriteriaCountDisabled11) ||
		(TempControlID ==IDC_EjectCriteriaCountDisabled12) ||
		(TempControlID ==IDC_EjectCriteriaCountDisabled13) ||
		(TempControlID ==IDC_EjectCriteriaCountDisabled14) ||
		(TempControlID ==IDC_EjectCriteriaCountDisabled15)) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectCriteriaCountDisabled0, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cRed);
		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_EjectCriteriaCountLabel0)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectCriteriaCountLabel0, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if ((!vGlobalCurrentProduct) || (vGlobalCurrentProduct->vNumberOfInspections < 1) || (vLocalSystemData->vThresholdSensitivities[0] > 100))
			pDC->SetTextColor(cWhite);
		else
		if (vLocalSystemData->vThresholdSensitivities[0] >= 61)
			pDC->SetTextColor(cMediumDarkGreen);
		else
		if (vLocalSystemData->vThresholdSensitivities[0] >= 31)
			pDC->SetTextColor(cYellow);
		else
			pDC->SetTextColor(cRed);

		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_EjectCriteriaCountLabel1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectCriteriaCountLabel1, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if ((!vGlobalCurrentProduct) || (vGlobalCurrentProduct->vNumberOfInspections < 2) || (vLocalSystemData->vThresholdSensitivities[1] > 100))
			pDC->SetTextColor(cWhite);
		else
		if (vLocalSystemData->vThresholdSensitivities[1] >= 61)
			pDC->SetTextColor(cMediumDarkGreen);
		else
		if (vLocalSystemData->vThresholdSensitivities[1] >= 31)
			pDC->SetTextColor(cYellow);
		else
			pDC->SetTextColor(cRed);

		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_EjectCriteriaCountLabel2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectCriteriaCountLabel2, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if ((!vGlobalCurrentProduct) || (vGlobalCurrentProduct->vNumberOfInspections < 3) || (vLocalSystemData->vThresholdSensitivities[2] > 100))
			pDC->SetTextColor(cWhite);
		else
		if (vLocalSystemData->vThresholdSensitivities[2] >= 61)
			pDC->SetTextColor(cMediumDarkGreen);
		else
		if (vLocalSystemData->vThresholdSensitivities[2] >= 31)
			pDC->SetTextColor(cYellow);
		else
			pDC->SetTextColor(cRed);

		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_EjectCriteriaCountLabel3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectCriteriaCountLabel3, 5);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		if ((!vGlobalCurrentProduct) || (vGlobalCurrentProduct->vNumberOfInspections < 4) || (vLocalSystemData->vThresholdSensitivities[3] > 100))
			pDC->SetTextColor(cWhite);
		else
		if (vLocalSystemData->vThresholdSensitivities[3] >= 61)
			pDC->SetTextColor(cMediumDarkGreen);
		else
		if (vLocalSystemData->vThresholdSensitivities[3] >= 31)
			pDC->SetTextColor(cYellow);
		else
			pDC->SetTextColor(cRed);

		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_EjectCriteriaCountLabel4)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectCriteriaCountLabel4, 5);  //5 is medium large
				
		pDC->SetBkMode(TRANSPARENT);
		if ((!vGlobalCurrentProduct) || (vGlobalCurrentProduct->vNumberOfInspections < 5) || (vLocalSystemData->vThresholdSensitivities[4] > 100))
			pDC->SetTextColor(cWhite);
		else
		if (vLocalSystemData->vThresholdSensitivities[4] >= 61)
			pDC->SetTextColor(cMediumDarkGreen);
		else
		if (vLocalSystemData->vThresholdSensitivities[4] >= 31)
			pDC->SetTextColor(cYellow);
		else
			pDC->SetTextColor(cRed);

		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_EjectCriteriaCountLabel5)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectCriteriaCountLabel5, 5);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		if ((!vGlobalCurrentProduct) || (vGlobalCurrentProduct->vNumberOfInspections < 6) || (vLocalSystemData->vThresholdSensitivities[5] > 100))
			pDC->SetTextColor(cWhite);
		else
		if (vLocalSystemData->vThresholdSensitivities[5] >= 61)
			pDC->SetTextColor(cMediumDarkGreen);
		else
		if (vLocalSystemData->vThresholdSensitivities[5] >= 31)
			pDC->SetTextColor(cYellow);
		else
			pDC->SetTextColor(cRed);

		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_EjectCriteriaCountLabel6)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectCriteriaCountLabel6, 5);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		if ((!vGlobalCurrentProduct) || (vGlobalCurrentProduct->vNumberOfInspections < 7) || (vLocalSystemData->vThresholdSensitivities[6] > 100))
			pDC->SetTextColor(cWhite);
		else
		if (vLocalSystemData->vThresholdSensitivities[6] >= 61)
			pDC->SetTextColor(cMediumDarkGreen);
		else
		if (vLocalSystemData->vThresholdSensitivities[6] >= 31)
			pDC->SetTextColor(cYellow);
		else
			pDC->SetTextColor(cRed);

		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_EjectCriteriaCountLabel7)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectCriteriaCountLabel7, 5);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		if ((!vGlobalCurrentProduct) || (vGlobalCurrentProduct->vNumberOfInspections < 8) || (vLocalSystemData->vThresholdSensitivities[7] > 100))
			pDC->SetTextColor(cWhite);
		else
		if (vLocalSystemData->vThresholdSensitivities[7] >= 61)
			pDC->SetTextColor(cMediumDarkGreen);
		else
		if (vLocalSystemData->vThresholdSensitivities[7] >= 31)
			pDC->SetTextColor(cYellow);
		else
			pDC->SetTextColor(cRed);

		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_EjectCriteriaCountLabel8)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectCriteriaCountLabel8, 5);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		if ((!vGlobalCurrentProduct) || (vGlobalCurrentProduct->vNumberOfInspections < 9) || (vLocalSystemData->vThresholdSensitivities[8] > 100))
			pDC->SetTextColor(cWhite);
		else
		if (vLocalSystemData->vThresholdSensitivities[8] >= 61)
			pDC->SetTextColor(cMediumDarkGreen);
		else
		if (vLocalSystemData->vThresholdSensitivities[8] >= 31)
			pDC->SetTextColor(cYellow);
		else
			pDC->SetTextColor(cRed);

		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_EjectCriteriaCountLabel9)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectCriteriaCountLabel9, 5);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		if ((!vGlobalCurrentProduct) || (vGlobalCurrentProduct->vNumberOfInspections < 10) || (vLocalSystemData->vThresholdSensitivities[9] > 100))
			pDC->SetTextColor(cWhite);
		else
		if (vLocalSystemData->vThresholdSensitivities[9] >= 61)
			pDC->SetTextColor(cMediumDarkGreen);
		else
		if (vLocalSystemData->vThresholdSensitivities[9] >= 31)
			pDC->SetTextColor(cYellow);
		else
			pDC->SetTextColor(cRed);

		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_EjectCriteriaCountLabel10)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectCriteriaCountLabel10, 5);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		if ((!vGlobalCurrentProduct) || (vGlobalCurrentProduct->vNumberOfInspections < 11) || (vLocalSystemData->vThresholdSensitivities[10] > 100))
			pDC->SetTextColor(cWhite);
		else
		if (vLocalSystemData->vThresholdSensitivities[10] >= 61)
			pDC->SetTextColor(cMediumDarkGreen);
		else
		if (vLocalSystemData->vThresholdSensitivities[10] >= 31)
			pDC->SetTextColor(cYellow);
		else
			pDC->SetTextColor(cRed);

		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_EjectCriteriaCountLabel11)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectCriteriaCountLabel11, 5);  //5 is medium large

		pDC->SetBkMode(TRANSPARENT);
		if ((!vGlobalCurrentProduct) || (vGlobalCurrentProduct->vNumberOfInspections < 12) || (vLocalSystemData->vThresholdSensitivities[11] > 100))
			pDC->SetTextColor(cWhite);
		else
		if (vLocalSystemData->vThresholdSensitivities[11] >= 61)
			pDC->SetTextColor(cMediumDarkGreen);
		else
		if (vLocalSystemData->vThresholdSensitivities[11] >= 31)
			pDC->SetTextColor(cYellow);
		else
			pDC->SetTextColor(cRed);

		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_EjectCriteriaCountLabel12)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectCriteriaCountLabel12, 5);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		if ((!vGlobalCurrentProduct) || (vGlobalCurrentProduct->vNumberOfInspections < 13) || (vLocalSystemData->vThresholdSensitivities[12] > 100))
			pDC->SetTextColor(cWhite);
		else
		if (vLocalSystemData->vThresholdSensitivities[12] >= 61)
			pDC->SetTextColor(cMediumDarkGreen);
		else
		if (vLocalSystemData->vThresholdSensitivities[12] >= 31)
			pDC->SetTextColor(cYellow);
		else
			pDC->SetTextColor(cRed);

		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_EjectCriteriaCountLabel13)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectCriteriaCountLabel13, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if ((!vGlobalCurrentProduct) || (vGlobalCurrentProduct->vNumberOfInspections < 14) || (vLocalSystemData->vThresholdSensitivities[13] > 100))
			pDC->SetTextColor(cWhite);
		else
		if (vLocalSystemData->vThresholdSensitivities[13] >= 61)
			pDC->SetTextColor(cMediumDarkGreen);
		else
		if (vLocalSystemData->vThresholdSensitivities[13] >= 31)
			pDC->SetTextColor(cYellow);
		else
			pDC->SetTextColor(cRed);

		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_EjectCriteriaCountLabel14)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectCriteriaCountLabel14, 5);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		if ((!vGlobalCurrentProduct) || (vGlobalCurrentProduct->vNumberOfInspections < 15) || (vLocalSystemData->vThresholdSensitivities[14] > 100))
			pDC->SetTextColor(cWhite);
		else
		if (vLocalSystemData->vThresholdSensitivities[14] >= 61)
			pDC->SetTextColor(cMediumDarkGreen);
		else
		if (vLocalSystemData->vThresholdSensitivities[14] >= 31)
			pDC->SetTextColor(cYellow);
		else
			pDC->SetTextColor(cRed);

		return vLocalSystemData->vLightBlueBrush;
	}

	if (TempControlID == IDC_EjectCriteriaCountLabel15)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectCriteriaCountLabel15, 5);  //5 is medium large
		
		pDC->SetBkMode(TRANSPARENT);
		if ((!vGlobalCurrentProduct) || (vGlobalCurrentProduct->vNumberOfInspections < 16) || (vLocalSystemData->vThresholdSensitivities[15] > 100))
			pDC->SetTextColor(cWhite);
		else
		if (vLocalSystemData->vThresholdSensitivities[15] >= 61)
			pDC->SetTextColor(cMediumDarkGreen);
		else
		if (vLocalSystemData->vThresholdSensitivities[15] >= 31)
			pDC->SetTextColor(cYellow);
		else
			pDC->SetTextColor(cRed);

		return vLocalSystemData->vLightBlueBrush;
	}
	
	if (TempControlID == IDC_Function1Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5);
	
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_Function2Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_Function3Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Button, 5);
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_Function4Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function4Button, 5);
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_Function5Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_SubFunction1Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 6);
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vRedBrush;
	}
	if (TempControlID == IDC_SubFunction2Button) 
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);
		pDC->SetTextColor(cButtonTextColor);
		pDC->SetBkMode(TRANSPARENT);
		if (!vLocalConfigurationData->vEnableEjectors)
		{
			return vLocalSystemData->vGreenBrush;
		}
		else
		{
			return vLocalSystemData->vYellowBrush;
		}
		return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_SubFunction3Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);
		if (!vGlobalCurrentProduct)
		{
			pDC->SetTextColor(cButtonTextColor);
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vGreenBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_SubFunction5Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

	if (TempControlID == IDC_SubFunction6Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5);
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		if (vOldYellowMessageButtonYellow)
			return vLocalSystemData->vYellowBrush;
		else
			return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_SubFunction7Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

	if (TempControlID == IDC_SubFunction8Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CProductionSummaryDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
}

void CProductionSummaryDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
  if (nIDEvent == vUpdatePSDisplayTimerHandle)
	{
		KillTimer(vUpdatePSDisplayTimerHandle);
		UpdateDisplay();
		int TempTimerResult = SetTimer(vUpdatePSDisplayTimerHandle,1000,NULL);
		if (!TempTimerResult)
			ReportErrorMessage("Error-Display Timer Failed",cEMailInspx,32000);
	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CProductionSummaryDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if (pMsg->message == cUpdateTitleBarMessage)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
		UpdateButtons();
		return true;  //true indicates it has been handled, so do not process
	}
	else
	if (pMsg->message == WM_KEYDOWN)
	{
		{
			BYTE *TestCrashPointer = NULL;
			int TempKey = pMsg->wParam;
			int TempKey1 = pMsg->lParam;
			int TempError = 0;
			CContainer *TempContainer = NULL;
			//SetDlgItemText(IDC_TriggerIndicator,dtoa(TempKey));
			//if (vGlobalShiftKeyDown)
			//	PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, TempKey);
			// else
			switch (TempKey)
			{
				//case 35: // End Key, Left Right on duo but new Image Key on Universal Overlays, Image Button
				//	OnNewImageKey();
				//break;
				case 16: // Shift Key
				case 0x60:  //0 key on KeyPad
					PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
				break;
				case 89:  //letter Y to clear yellow message
						vMainWindowPointer->ClearYellowWarningButton();
				break;
				case 113: // F2:
					OnStnClickedSubfunction5button();
				break;
				case 114: // F3:
					OnStnClickedSubfunction7button();
				break;
				case 115: // F4:
					OnStnClickedSubfunction6button();
				break;
				case 116: // F5:
					OnStnClickedSubfunction1button();
				break;
				case 117: // F6 Key - Function 5:
					OnStnClickedSubfunction4button();
				break;
				case 118: // F7 Key - Sub Function 1:
					OnStnClickedFunction1button();
				break;
				case 119: // F8 Key - Sub Function 2:
					OnStnClickedSubfunction8button();
				break;
				case 120: // F9 Key - Sub Function 3:
					OnStnClickedSubfunction2button();
				break;
				case 122: // F11 Key - Sub Function 4:
					OnStnClickedSubfunction3button();
				break;
				case 111: // / Key - Sub Function 5:
					OnStnClickedFunction2button();
				break;
				case 106: // * Key - Sub Function 6:
					OnStnClickedFunction4button();
				break;
				case 109: // - Key - Sub Function 7  Decimal Point:
					OnStnClickedFunction3button();
				break;
				case 107: // + Key - Sub Function 8:
					OnStnClickedFunction5button();
				break;
				case 36:  //Home key, Screen Capture
					if (PasswordOK(cOperatorPassword,true))
					{
						vGlobalShiftKeyDown = true;
						PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
					}
				break;
				case cPageDown: // Help
					if (PasswordOK(cOperatorPassword,true))
					{
						PostThreadMessage(vGlobalCallingThreadID,cShowHelp, 21, 0);
					}
					{
						CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
						if (TempWindow)
							TempWindow->Invalidate(false);
					}
				break;
				case cPageUp: // Clear
				{
					if (vGlobalPasswordLevel)
					{
						PasswordOK(cNoPassword,false);
					}
					else
					{
						PasswordOK(cAnyPasswordToEnter, true);
					}
					UpdateButtons();
					CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
				break;
				case 27:// escape
					if (vLocalConfigurationData->vDemoMode)
						vMainWindowPointer->AskIfShouldExit();
					else //open a chat window
					//	if (Pass wor dOK())
					//		AskIfShouldExit();
					::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);

				break;
				//case 66: case 67: //pressed b or c on keyboard to clear brownout indicator
				//	if (PasswordOK(cSuperInspxPassword,false))
				//	{
				//		ClearRedStatusIndicator();
				//		UpdateClearYellowMessageButton();
				//	}
				//break;
			}
			return true;  //true indicates it has been handled, so do not process
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CProductionSummaryDialog::UpdateButtons() 
{
	UpdateClearYellowMessageButton();
	UpdateDisableEjectorsButton();

	CString TempText = "-";

	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		//show inspx log button
		m_Function3Button.ShowWindow(SW_SHOW);
		//m_Function3Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		SetDlgItemText(IDC_Function3Button, _T("Inspx Log"));
	}
	else
	{
		m_Function3Button.ShowWindow(SW_SHOW);
		//m_Function3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		CString TempString = " ";
		TempString.LoadString(IDS_ShowThresholds);
		SetDlgItemText(IDC_Function3Button,TempString);
	}

	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	{
		m_SubFunction1Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
	}
	else
		m_SubFunction1Button.ShowWindow(SW_SHOW);
}

void CProductionSummaryDialog::UpdateDisplay() 
{
	CTime TempTime = CTime::GetCurrentTime();
	CString TempTimeString = " ";
	if (vLocalConfigurationData->vShow24HourTime)
		TempTimeString = TempTime.Format("%H:%M:%S");//24 hour time format
	else
		TempTimeString = TempTime.Format("%#I:%M:%S%p");//12 hour time format with am pm

	SetDlgItemText(IDC_Clock,TempTimeString);

	if (vOldLastDateString != vMainWindowPointer->vLastDateString)
	{
		vOldLastDateString = vMainWindowPointer->vLastDateString;
		SetDlgItemText(IDC_Date, vOldLastDateString);
	}

	if (!vLocalConfigurationData->vEnableEjectors)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_EjectorsDisabled);
		if (TempWindow)
		if (TempWindow->IsWindowVisible())
			TempWindow->Invalidate(false);
	}

	CString TempText = "-";
	if (vOldLastLearnState != vMainWindowPointer->vLastLearnState)
	{
		vOldLastLearnState = vMainWindowPointer->vLastLearnState;
		if (!vOldYellowMessageButtonYellow)
		if ((vOldLastLearnState) && (vOldLastLearnState != 255)) //colors for Improving Learn - Do not test contaminants indicator
		{
			TempText.LoadString(IDS_ImprovingLearnDonottestsmallcontaminants);
			SetDlgItemText(IDC_StatusLine, TempText);
		}
		else
			SetDlgItemText(IDC_StatusLine, _T(""));
	}

	if (vLocalSystemData->vOldGoodCountTotal != vLocalSystemData->vGoodCountTotal)
	{
		vLocalSystemData->vOldGoodCountTotal = vLocalSystemData->vGoodCountTotal;

		SetDlgItemText(IDC_TotalGoodStaticText, IToAWithComma(vLocalSystemData->vOldGoodCountTotal));
	}


	if (vOldYellowMessageButtonYellow != vLocalSystemData->vYellowMessageButtonYellow)
		UpdateClearYellowMessageButton();

	if (!((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || ((vGlobalCurrentProduct) && (vGlobalCurrentProduct->vBulkProductMode)))) //Continuous Feed, or a Pipeline
	if (vOldPercentEjected != vLocalSystemData->vOldPercentEjected)
	{
		vOldPercentEjected = vLocalSystemData->vOldPercentEjected;
		SetDlgItemText(IDC_PercentEjected, dtoa(vOldPercentEjected, 3));
	}
		
	if (vOldContainerRate != vLocalSystemData->vContainerRate)
	{
		vOldContainerRate = vLocalSystemData->vContainerRate;

		SetDlgItemText(IDC_ContainersPerMinute, dtoa(vOldContainerRate, 0));
	}

	if (vOldBeltRate != vLocalSystemData->vOldBeltRate)
	{
		vOldBeltRate = vLocalSystemData->vOldBeltRate;
		SetDlgItemText(IDC_BeltRate,dtoa(vOldBeltRate / 12, 0));
	}

	if (vLocalSystemData->vOldShiftRateCPM != vLocalSystemData->vShiftRateCPM)
	{
		vLocalSystemData->vOldShiftRateCPM = vLocalSystemData->vShiftRateCPM;
		SetDlgItemText(IDC_ShiftRateCPM,IToAWithComma(vLocalSystemData->vOldShiftRateCPM));
	}

	if (vLocalSystemData->vOldRunDuration != vLocalSystemData->vRunDuration)
	{
		vLocalSystemData->vOldRunDuration = vLocalSystemData->vRunDuration;
		SetDlgItemText(IDC_RunDurationStaticText, FormatRunTimeCounterString(vLocalSystemData->vOldRunDuration));

		if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || ((vGlobalCurrentProduct) && (vGlobalCurrentProduct->vBulkProductMode))) //Continuous Feed, or a Pipeline
		if (vLocalSystemData->vOldRunDuration)
			SetDlgItemText(IDC_TotalContainers,FormatRunTimeCounterString(vLocalSystemData->vOldRunDuration));// if a pipe show run duration for total
		else
			SetDlgItemText(IDC_TotalContainers, _T(""));// if a pipe show blank if total zero
	}

	if (vLocalSystemData->vOldProductionStopsCount != vLocalSystemData->vProductionStopsCount)
	{
		vLocalSystemData->vOldProductionStopsCount = vLocalSystemData->vProductionStopsCount;
		SetDlgItemText(IDC_ProductionStopsCountStaticText, dtoa(vLocalSystemData->vOldProductionStopsCount, 0));
	}
	if (vLocalSystemData->vOldStopDuration != vLocalSystemData->vStopDuration)
	{
		vLocalSystemData->vOldStopDuration = vLocalSystemData->vStopDuration;
		SetDlgItemText(IDC_StopDurationCountStaticText, FormatRunTimeCounterString(vLocalSystemData->vOldStopDuration));
	}
	if (vLocalSystemData->vOldTimeSinceLastStop != vLocalSystemData->vTimeSinceLastStop)
	{
		vLocalSystemData->vOldTimeSinceLastStop = vLocalSystemData->vTimeSinceLastStop;
		SetDlgItemText(IDC_TimeSinceLastStopStaticText, FormatRunTimeCounterString(vLocalSystemData->vOldTimeSinceLastStop));
	}

	if (vOldXRayOnNotice != vLocalSystemData->vXRayOnNotice)
	{
		vOldXRayOnNotice = vLocalSystemData->vXRayOnNotice;
		if ((vOldXRayOnNotice) && (vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
		{
			m_XRaysOn.ShowWindow(SW_SHOW);
			m_Calibrating.ShowWindow(SW_SHOW);
		}
		else
		{
			m_XRaysOn.ShowWindow(SW_HIDE);
			m_Calibrating.ShowWindow(SW_HIDE);
		}
		UpdateButtons();
	}
		
	if (vLocalSystemData->vTotalContainers != vOldTotalContainers)
	{
		if (!vOldTotalContainers) //if was zero, now value, show reset counters
		{
			m_Function5Button.ShowWindow(SW_SHOW);
			CString TempText = "-";
			TempText.LoadString(IDS_ResetCounters);
			SetDlgItemText(IDC_Function5Button,TempText);
		}
		vOldTotalContainers = vLocalSystemData->vTotalContainers;
		if ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && ((vGlobalCurrentProduct) && (!vGlobalCurrentProduct->vBulkProductMode))) //Not Continuous Feed, and Not a Pipeline
			SetDlgItemText(IDC_TotalContainers,IToAWithComma(vOldTotalContainers));// if not a pipe show total

		if (vLocalSystemData->vTotalContainers > 0)
		{
			//socketio ejectcount
			vLocalSystemData->vPercentEjected = ((double)vLocalSystemData->vEjectCountTotal / (double)vLocalSystemData->vTotalContainers) * 100.0;
			if (vLocalSystemData->vPercentEjected > 100)
				vLocalSystemData->vPercentEjected = 100;
		}
		else
			vLocalSystemData->vPercentEjected = 0;

		if (vLocalSystemData->vPercentEjected != vLocalSystemData->vOldPercentEjected)
		{
			vLocalSystemData->vOldPercentEjected = vLocalSystemData->vPercentEjected;
			if (!((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || ((vGlobalCurrentProduct) && (vGlobalCurrentProduct->vBulkProductMode)))) //Continuous Feed, or a Pipeline
			{//Not Continuous Feed, and Not a Pipeline
				//socketio ejectrate
				SetDlgItemText(IDC_PercentRejected, dtoa(vLocalSystemData->vOldPercentEjected, 3));
			}
		}

		if (!((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || ((vGlobalCurrentProduct) && (vGlobalCurrentProduct->vBulkProductMode)))) //Not Continuous Feed, or a Pipeline
		if (vLocalSystemData->vPercentRejected != vOldPercentRejected)
		{
			vOldPercentRejected = vLocalSystemData->vPercentRejected;
			SetDlgItemText(IDC_TotalRejectsPercent, dtoa(vLocalSystemData->vOldPercentRejected, 3));
		}
	}

	if (vLocalSystemData->vMissedEjectsBecauseDisabledCount)
	if (vOldRejectCountTotal != vLocalSystemData->vOldRejectCountTotal)
	{
		if (!vOldRejectCountTotal)
		{
			m_TotalRejectsLabelStaticText.ShowWindow(SW_SHOW);
			m_TotalRejectsStaticText.ShowWindow(SW_SHOW);
			if (!vLocalSystemData->vYellowMessageButtonYellow)
			if ((vOldLastLearnState == 0) || (vOldLastLearnState == 255)) //Don't show labels if Improving Learn Message still up
			{
				m_MissedColumnLabel.ShowWindow(SW_SHOW);
				m_EjectedColumnLabel.ShowWindow(SW_SHOW);
			}
		}
		vOldRejectCountTotal = vLocalSystemData->vOldRejectCountTotal;
		SetDlgItemText(IDC_TotalRejectsStaticText,IToAWithComma(vOldRejectCountTotal));
		if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || ((vGlobalCurrentProduct) && (vGlobalCurrentProduct->vBulkProductMode))) //Continuous Feed, or a Pipeline
		{//if a pipeline put in when the last eject occured
			CString TempText = vLocalSystemData->vLastEjectTime.Left(8);
			SetDlgItemText(IDC_PercentRejected,	TempText);
		}
	}
	if (vOldEjectCountTotal != vLocalSystemData->vOldEjectCountTotal)
	{
		vOldEjectCountTotal = vLocalSystemData->vOldEjectCountTotal;
		SetDlgItemText(IDC_TotalEjectsStaticText,IToAWithComma(vOldEjectCountTotal));

		if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || ((vGlobalCurrentProduct) && (vGlobalCurrentProduct->vBulkProductMode))) //Continuous Feed, or a Pipeline
		{//if a pipeline put in when the last eject occured
			CString TempText = vLocalSystemData->vLastEjectTime.Left(8);
			SetDlgItemText(IDC_PercentRejected,	TempText);
		}
	}

	if (vOldTotalUnConfirmed != vLocalSystemData->vOldTotalUnConfirmed)
	{
		if (!vOldTotalUnConfirmed)
		{
			m_TotalUnConfirmLabel.ShowWindow(SW_SHOW);
			m_TotalUnConfirmed.ShowWindow(SW_SHOW);
		}
		vOldTotalUnConfirmed = vLocalSystemData->vOldTotalUnConfirmed;
		SetDlgItemText(IDC_TotalUnConfirmed,IToAWithComma(vOldTotalUnConfirmed));
	}

	//update inspection count displays on screen if need to
	BYTE TempEndLoopCount = vLocalSystemData->vTotalNumberOfInspections;
	if (TempEndLoopCount > cMaximumNumberOfCountsDisplayed)
		TempEndLoopCount = cMaximumNumberOfCountsDisplayed;
	for (BYTE TempLoop = 0; TempLoop < TempEndLoopCount; TempLoop++)
	{
		if (vLocalSystemData->vOldXRayCriteriaCount[TempLoop] != vLocalSystemData->vXRayCriteriaCount[TempLoop])
		{
			vLocalSystemData->vOldXRayCriteriaCount[TempLoop] = vLocalSystemData->vXRayCriteriaCount[TempLoop];

			CString vTempString = IToAWithComma(vLocalSystemData->vOldXRayCriteriaCount[TempLoop]);
			//std::string tempcstring(vTempString, vTempString.GetLength());
			switch (TempLoop)
			{
				case 0:
					SetDlgItemText(IDC_EjectCriteriaCount0, vTempString);
				break;
				case 1:
					SetDlgItemText(IDC_EjectCriteriaCount1, vTempString);
				break;
				case 2:
					SetDlgItemText(IDC_EjectCriteriaCount2, vTempString);
				break;
				case 3:
					SetDlgItemText(IDC_EjectCriteriaCount3, vTempString);
				break;
				case 4:
					SetDlgItemText(IDC_EjectCriteriaCount4, vTempString);
				break;
				case 5:
					SetDlgItemText(IDC_EjectCriteriaCount5, vTempString);
				break;
				case 6:
					SetDlgItemText(IDC_EjectCriteriaCount6, vTempString);
				break;
				case 7:
					SetDlgItemText(IDC_EjectCriteriaCount7, vTempString);
				break;
				case 8:
					SetDlgItemText(IDC_EjectCriteriaCount8, vTempString);
				break;
				case 9:
					SetDlgItemText(IDC_EjectCriteriaCount9, vTempString);
				break;
				case 10:
					SetDlgItemText(IDC_EjectCriteriaCount10, vTempString);
				break;
				case 11:
					SetDlgItemText(IDC_EjectCriteriaCount11, vTempString);
				break;
				case 12:
					SetDlgItemText(IDC_EjectCriteriaCount12, vTempString);
				break;
				case 13:
					SetDlgItemText(IDC_EjectCriteriaCount13, vTempString);
				break;
				case 14:
					SetDlgItemText(IDC_EjectCriteriaCount14, vTempString);
				break;
				case 15:
					SetDlgItemText(IDC_EjectCriteriaCount15, vTempString);
				break;
			}
		}
		//display inspection disabled counts
		bool TempTurnOnDisplay = false;
		bool TempHaveANewValueToDisplay = false;
		if (vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop] != vLocalSystemData->vXRayCriteriaCountButDisabled[TempLoop])
		{
			if (!vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop])
				TempTurnOnDisplay = true;

			vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop] = vLocalSystemData->vXRayCriteriaCountButDisabled[TempLoop];
			TempHaveANewValueToDisplay = true;
		}
		switch (TempLoop)
		{
			case 0:
				if (TempTurnOnDisplay)
					m_EjectCriteriaCountDisabled0.ShowWindow(SW_SHOW);
				if (TempHaveANewValueToDisplay) 
				{
					SetDlgItemText(IDC_EjectCriteriaCountDisabled0, IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
				}
			break;
			case 1:
				if (TempTurnOnDisplay)
					m_EjectCriteriaCountDisabled1.ShowWindow(SW_SHOW);
				if (TempHaveANewValueToDisplay) 
				{
					SetDlgItemText(IDC_EjectCriteriaCountDisabled1, IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
				}
			break;
			case 2:
				if (TempTurnOnDisplay)
					m_EjectCriteriaCountDisabled2.ShowWindow(SW_SHOW);
				if (TempHaveANewValueToDisplay) 
				{
					SetDlgItemText(IDC_EjectCriteriaCountDisabled2, IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
				}
			break;
			case 3:
				if (TempTurnOnDisplay)
					m_EjectCriteriaCountDisabled3.ShowWindow(SW_SHOW);
				if (TempHaveANewValueToDisplay) 
				{
					SetDlgItemText(IDC_EjectCriteriaCountDisabled3, IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
				}
			break;
			case 4:
				if (TempTurnOnDisplay)
					m_EjectCriteriaCountDisabled4.ShowWindow(SW_SHOW);
				if (TempHaveANewValueToDisplay) 
				{
					SetDlgItemText(IDC_EjectCriteriaCountDisabled4, IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
				}
			break;
			case 5:
				if (TempTurnOnDisplay)
					m_EjectCriteriaCountDisabled5.ShowWindow(SW_SHOW);
				if (TempHaveANewValueToDisplay) 
				{
					SetDlgItemText(IDC_EjectCriteriaCountDisabled5, IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
				}
			break;
			case 6:
				if (TempTurnOnDisplay)
					m_EjectCriteriaCountDisabled6.ShowWindow(SW_SHOW);
				if (TempHaveANewValueToDisplay) 
				{
					SetDlgItemText(IDC_EjectCriteriaCountDisabled6, IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
				}
			break;
			case 7:
				if (TempTurnOnDisplay)
					m_EjectCriteriaCountDisabled7.ShowWindow(SW_SHOW);
				if (TempHaveANewValueToDisplay) 
				{
					SetDlgItemText(IDC_EjectCriteriaCountDisabled7, IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
				}
			break;
			case 8:
				if (TempTurnOnDisplay)
					m_EjectCriteriaCountDisabled8.ShowWindow(SW_SHOW);
				if (TempHaveANewValueToDisplay) 
				{
					SetDlgItemText(IDC_EjectCriteriaCountDisabled8, IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
				}
			break;
			case 9:
				if (TempTurnOnDisplay)
					m_EjectCriteriaCountDisabled9.ShowWindow(SW_SHOW);
				if (TempHaveANewValueToDisplay)
				{
					SetDlgItemText(IDC_EjectCriteriaCountDisabled9, IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
				}
				break;
			case 10:
				if (TempTurnOnDisplay)
					m_EjectCriteriaCountDisabled10.ShowWindow(SW_SHOW);
				if (TempHaveANewValueToDisplay)
				{
					SetDlgItemText(IDC_EjectCriteriaCountDisabled10, IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
				}
				break;
			case 11:
				if (TempTurnOnDisplay)
					m_EjectCriteriaCountDisabled11.ShowWindow(SW_SHOW);
				if (TempHaveANewValueToDisplay)
				{
					SetDlgItemText(IDC_EjectCriteriaCountDisabled11, IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
				}
				break;
			case 12:
				if (TempTurnOnDisplay)
					m_EjectCriteriaCountDisabled12.ShowWindow(SW_SHOW);
				if (TempHaveANewValueToDisplay)
				{
					SetDlgItemText(IDC_EjectCriteriaCountDisabled12, IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
				}
				break;
			case 13:
				if (TempTurnOnDisplay)
					m_EjectCriteriaCountDisabled13.ShowWindow(SW_SHOW);
				if (TempHaveANewValueToDisplay)
				{
					SetDlgItemText(IDC_EjectCriteriaCountDisabled13, IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
				}
				break;
			case 14:
				if (TempTurnOnDisplay)
					m_EjectCriteriaCountDisabled14.ShowWindow(SW_SHOW);
				if (TempHaveANewValueToDisplay)
				{
					SetDlgItemText(IDC_EjectCriteriaCountDisabled14, IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
				}
				break;
			case 15:
				if (TempTurnOnDisplay)
					m_EjectCriteriaCountDisabled15.ShowWindow(SW_SHOW);
				if (TempHaveANewValueToDisplay)
				{
					SetDlgItemText(IDC_EjectCriteriaCountDisabled15, IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
				}
				break;
		}
	}
}

void CProductionSummaryDialog::OnStnClickedDialogtitlestatictext1()
{
	if (vGlobalShiftKeyDown)
		vMainWindowPointer->DisplayCSVFile();
	else
	{
		BYTE TempHiddenKeys = 8;
		if (!PasswordOK(cTemporaryInspxPassword,false))
			TempHiddenKeys = 10;
		PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
	}
}

void CProductionSummaryDialog::DisplayInspectionNames()
{
	//display the inspection labels on the screen
	CString TempString = " ";
	TempString = "";
	if (vGlobalCurrentProduct)
	{
		for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfCountsDisplayed; TempLoop++)  //there are 0 to 15 labels on the screen
		{
			if (TempLoop < vLocalSystemData->vTotalNumberOfInspections)
				TempString = vLocalSystemData->vCriteriaString[TempLoop] + ":";
			else
				TempString = "";

			switch (TempLoop)
			{
				case 0:
					SetDlgItemText(IDC_EjectCriteriaCountLabel0,TempString);
				break;
				case 1:
					SetDlgItemText(IDC_EjectCriteriaCountLabel1,TempString);
				break;
				case 2:
					SetDlgItemText(IDC_EjectCriteriaCountLabel2,TempString);
				break;
				case 3:
					SetDlgItemText(IDC_EjectCriteriaCountLabel3,TempString);
				break;
				case 4:
					SetDlgItemText(IDC_EjectCriteriaCountLabel4,TempString);
				break;
				case 5:
					SetDlgItemText(IDC_EjectCriteriaCountLabel5,TempString);
				break;
				case 6:
					SetDlgItemText(IDC_EjectCriteriaCountLabel6,TempString);
				break;
				case 7:
					SetDlgItemText(IDC_EjectCriteriaCountLabel7,TempString);
				break;
				case 8:
					SetDlgItemText(IDC_EjectCriteriaCountLabel8,TempString);
				break;
				case 9:
					SetDlgItemText(IDC_EjectCriteriaCountLabel9,TempString);
				break;
				case 10:
					SetDlgItemText(IDC_EjectCriteriaCountLabel10,TempString);
				break;
				case 11:
					SetDlgItemText(IDC_EjectCriteriaCountLabel11,TempString);
				break;
				case 12:
					SetDlgItemText(IDC_EjectCriteriaCountLabel12,TempString);
				break;
				case 13:
					SetDlgItemText(IDC_EjectCriteriaCountLabel13,TempString);
				break;
				case 14:
					SetDlgItemText(IDC_EjectCriteriaCountLabel14,TempString);
				break;
				case 15:
					SetDlgItemText(IDC_EjectCriteriaCountLabel15,TempString);
				break;
			}
		}
	}
}

void CProductionSummaryDialog::ShowOrHideCounts()
{
	BYTE TempConfirmerUsed = 0;
	if (vLocalSystemData->vEjectorUsed)
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
	if ((vLocalConfigurationData->vEjector[TempLoop].vConfirmEnabled) &&
		(vLocalConfigurationData->vEjector[TempLoop].vConfirmDigitalInputLine) &&
		(vLocalConfigurationData->vEjector[TempLoop].vEnabled))
		TempConfirmerUsed = TempConfirmerUsed | (1 << TempLoop);

	if (!vLocalSystemData->vTotalUnConfirmed)
	if (!TempConfirmerUsed)
	{
		m_TotalUnConfirmed.ShowWindow(SW_HIDE);
		m_TotalUnConfirmLabel.ShowWindow(SW_HIDE);
	}
	else
	{
		m_TotalUnConfirmed.ShowWindow(SW_SHOW);
		m_TotalUnConfirmLabel.ShowWindow(SW_SHOW);
	}

	if ((vLocalSystemData->vMissedEjectsBecauseDisabledCount != 0) && (!vLocalSystemData->vYellowMessageButtonYellow) && ((vOldLastLearnState == 0) || (vOldLastLearnState == 255)))
	{
		m_MissedColumnLabel.ShowWindow(SW_SHOW);
		m_EjectedColumnLabel.ShowWindow(SW_SHOW);
	}
	else
	{
		m_MissedColumnLabel.ShowWindow(SW_HIDE);
		m_EjectedColumnLabel.ShowWindow(SW_HIDE);
	}

	if (vLocalSystemData->vMissedEjectsBecauseDisabledCount == 0)
	{
		m_TotalRejectsStaticText.ShowWindow(SW_HIDE);
		m_TotalRejectsLabelStaticText.ShowWindow(SW_HIDE);
		m_TotalRejectsPercent.ShowWindow(SW_HIDE);
		m_TotalRejectsPercentLabel.ShowWindow(SW_HIDE);
	}
	else
	{
		m_TotalRejectsStaticText.ShowWindow(SW_SHOW);
		m_TotalRejectsLabelStaticText.ShowWindow(SW_SHOW);

		if ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && ((vGlobalCurrentProduct) && (!vGlobalCurrentProduct->vBulkProductMode))) //not Continuous Feed, or a Pipeline
		{
			m_TotalRejectsPercent.ShowWindow(SW_SHOW);
			m_TotalRejectsPercentLabel.ShowWindow(SW_SHOW);
		}
		else
		{
			m_TotalRejectsPercent.ShowWindow(SW_HIDE);
			m_TotalRejectsPercentLabel.ShowWindow(SW_HIDE);
		}
	}
	if (vGlobalCurrentProduct)
	{
		CString TempString("***");
		CStatic *TempLabel;
		CStatic *TempCounter;
		DWORD TempDisabled = 0;
		for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfCountsDisplayed; TempLoop++)  //there are 0 to 12 labels on the screen
		{
			if (TempLoop < vLocalSystemData->vTotalNumberOfInspections)
			{
				TempString = IToAWithComma(vLocalSystemData->vXRayCriteriaCountButDisabled[TempLoop]);
				vLocalSystemData->vOldXRayCriteriaCount[TempLoop] = vLocalSystemData->vXRayCriteriaCountButDisabled[TempLoop];
				TempDisabled = vLocalSystemData->vXRayCriteriaCountButDisabled[TempLoop];
				if (TempDisabled)
					vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop] = TempDisabled;
			}
			else
				TempDisabled = false;

			switch (TempLoop)
			{
				case 0:
					SetDlgItemText(IDC_EjectCriteriaCount0,TempString);
					TempLabel = &m_EjectCriteriaCountLabel0;
					TempCounter = &m_EjectCriteriaCount0;
					if (TempDisabled)
					{
						SetDlgItemText(IDC_EjectCriteriaCountDisabled0,IToAWithComma(
							vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
						m_EjectCriteriaCountDisabled0.ShowWindow(SW_SHOW);
					}
				break;
				case 1:
					SetDlgItemText(IDC_EjectCriteriaCount1,TempString);
					TempLabel = &m_EjectCriteriaCountLabel1;
					TempCounter = &m_EjectCriteriaCount1;
					if (TempDisabled)
					{
						SetDlgItemText(IDC_EjectCriteriaCountDisabled1,IToAWithComma(
							vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
						m_EjectCriteriaCountDisabled1.ShowWindow(SW_SHOW);
					}
				break;
				case 2:
					SetDlgItemText(IDC_EjectCriteriaCount2,TempString);
					TempLabel = &m_EjectCriteriaCountLabel2;
					TempCounter = &m_EjectCriteriaCount2;
					if (TempDisabled)
					{
						SetDlgItemText(IDC_EjectCriteriaCountDisabled2,IToAWithComma(
							vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
						m_EjectCriteriaCountDisabled2.ShowWindow(SW_SHOW);
					}
				break;
				case 3:
					SetDlgItemText(IDC_EjectCriteriaCount3,TempString);
					TempLabel = &m_EjectCriteriaCountLabel3;
					TempCounter = &m_EjectCriteriaCount3;
					if (TempDisabled)
					{
						SetDlgItemText(IDC_EjectCriteriaCountDisabled3,IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
						m_EjectCriteriaCountDisabled3.ShowWindow(SW_SHOW);
					}
				break;
				case 4:
					SetDlgItemText(IDC_EjectCriteriaCount4,TempString);
					TempLabel = &m_EjectCriteriaCountLabel4;
					TempCounter = &m_EjectCriteriaCount4;
					if (TempDisabled)
					{
						SetDlgItemText(IDC_EjectCriteriaCountDisabled4,IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
						m_EjectCriteriaCountDisabled4.ShowWindow(SW_SHOW);
					}
				break;
				case 5:
					SetDlgItemText(IDC_EjectCriteriaCount5,TempString);
					TempLabel = &m_EjectCriteriaCountLabel5;
					TempCounter = &m_EjectCriteriaCount5;
					if (TempDisabled)
					{
						SetDlgItemText(IDC_EjectCriteriaCountDisabled5,IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
						m_EjectCriteriaCountDisabled5.ShowWindow(SW_SHOW);
					}
				break;
				case 6:
					SetDlgItemText(IDC_EjectCriteriaCount6,TempString);
					TempLabel = &m_EjectCriteriaCountLabel6;
					TempCounter = &m_EjectCriteriaCount6;
					if (TempDisabled)
					{
						SetDlgItemText(IDC_EjectCriteriaCountDisabled6,IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
						m_EjectCriteriaCountDisabled6.ShowWindow(SW_SHOW);
					}
				break;
				case 7:
					SetDlgItemText(IDC_EjectCriteriaCount7,TempString);
					TempLabel = &m_EjectCriteriaCountLabel7;
					TempCounter = &m_EjectCriteriaCount7;
					if (TempDisabled)
					{
						SetDlgItemText(IDC_EjectCriteriaCountDisabled7,IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
						m_EjectCriteriaCountDisabled7.ShowWindow(SW_SHOW);
					}
				break;
				case 8:
					SetDlgItemText(IDC_EjectCriteriaCount8,TempString);
					TempLabel = &m_EjectCriteriaCountLabel8;
					TempCounter = &m_EjectCriteriaCount8;
					if (TempDisabled)
					{
						SetDlgItemText(IDC_EjectCriteriaCountDisabled8,IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
						m_EjectCriteriaCountDisabled8.ShowWindow(SW_SHOW);
					}
				break;
				case 9:
					SetDlgItemText(IDC_EjectCriteriaCount9,TempString);
					TempLabel = &m_EjectCriteriaCountLabel9;
					TempCounter = &m_EjectCriteriaCount9;
					if (TempDisabled)
					{
						SetDlgItemText(IDC_EjectCriteriaCountDisabled9,IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
						m_EjectCriteriaCountDisabled9.ShowWindow(SW_SHOW);
					}
				break;
				case 10:
					SetDlgItemText(IDC_EjectCriteriaCount10,TempString);
					TempLabel = &m_EjectCriteriaCountLabel10;
					TempCounter = &m_EjectCriteriaCount10;
					if (TempDisabled)
					{
						SetDlgItemText(IDC_EjectCriteriaCountDisabled10,IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
						m_EjectCriteriaCountDisabled10.ShowWindow(SW_SHOW);
					}
				break;
				case 11:
					SetDlgItemText(IDC_EjectCriteriaCount11,TempString);
					TempLabel = &m_EjectCriteriaCountLabel11;
					TempCounter = &m_EjectCriteriaCount11;
					if (TempDisabled)
					{
						SetDlgItemText(IDC_EjectCriteriaCountDisabled11,IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
						m_EjectCriteriaCountDisabled11.ShowWindow(SW_SHOW);
					}
				break;
				case 12:
					SetDlgItemText(IDC_EjectCriteriaCount12,TempString);
					TempLabel = &m_EjectCriteriaCountLabel12;
					TempCounter = &m_EjectCriteriaCount12;
					if (TempDisabled)
					{
						SetDlgItemText(IDC_EjectCriteriaCountDisabled12,IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
						m_EjectCriteriaCountDisabled12.ShowWindow(SW_SHOW);
					}
				break;
				case 13:
					SetDlgItemText(IDC_EjectCriteriaCount13,TempString);
					TempLabel = &m_EjectCriteriaCountLabel13;
					TempCounter = &m_EjectCriteriaCount13;
					if (TempDisabled)
					{
						SetDlgItemText(IDC_EjectCriteriaCountDisabled13,IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
						m_EjectCriteriaCountDisabled13.ShowWindow(SW_SHOW);
					}
				break;
				case 14:
					SetDlgItemText(IDC_EjectCriteriaCount14,TempString);
					TempLabel = &m_EjectCriteriaCountLabel14;
					TempCounter = &m_EjectCriteriaCount14;
					if (TempDisabled)
					{
						SetDlgItemText(IDC_EjectCriteriaCountDisabled14,IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
						m_EjectCriteriaCountDisabled14.ShowWindow(SW_SHOW);
					}
				break;
				case 15:
					SetDlgItemText(IDC_EjectCriteriaCount15,TempString);
					TempLabel = &m_EjectCriteriaCountLabel15;
					TempCounter = &m_EjectCriteriaCount15;
					if (TempDisabled)
					{
						SetDlgItemText(IDC_EjectCriteriaCountDisabled15,IToAWithComma(vLocalSystemData->vOldXRayCriteriaCountButDisabled[TempLoop]));
						m_EjectCriteriaCountDisabled15.ShowWindow(SW_SHOW);
					}
				break;
			}
			if (TempLoop < vLocalSystemData->vTotalNumberOfInspections)
			{
				TempLabel->ShowWindow(SW_SHOW);
				TempCounter->ShowWindow(SW_SHOW);
			}
			else
			{
				TempLabel->ShowWindow(SW_HIDE);
				TempCounter->ShowWindow(SW_HIDE);
			}
		}
		//hide the improve learn inspection counts in the production summary
		//1/9/2005 hide improve learn inspection counts
		/*
		BYTE TempImproveLearnInspectionNumber = GetImproveLearnInspectionNumber();
		WORD TempMask = 0xFFFF;
		if (TempImproveLearnInspectionNumber)
		{
			switch (TempImproveLearnInspectionNumber)
			{
				case 1:
						m_EjectCriteriaCountDisabled0.ShowWindow(SW_HIDE);
						m_EjectCriteriaCount0.ShowWindow(SW_HIDE);
						m_EjectCriteriaCountLabel0.ShowWindow(SW_HIDE);
				break;
				case 2:
						m_EjectCriteriaCountDisabled1.ShowWindow(SW_HIDE);
						m_EjectCriteriaCount1.ShowWindow(SW_HIDE);
						m_EjectCriteriaCountLabel1.ShowWindow(SW_HIDE);
				break;
				case 3:
						m_EjectCriteriaCountDisabled2.ShowWindow(SW_HIDE);
						m_EjectCriteriaCount2.ShowWindow(SW_HIDE);
						m_EjectCriteriaCountLabel2.ShowWindow(SW_HIDE);
				break;
				case 4:
						m_EjectCriteriaCountDisabled3.ShowWindow(SW_HIDE);
						m_EjectCriteriaCount3.ShowWindow(SW_HIDE);
						m_EjectCriteriaCountLabel3.ShowWindow(SW_HIDE);
				break;
				case 5:
						m_EjectCriteriaCountDisabled4.ShowWindow(SW_HIDE);
						m_EjectCriteriaCount4.ShowWindow(SW_HIDE);
						m_EjectCriteriaCountLabel4.ShowWindow(SW_HIDE);
				break;
				case 6:
						m_EjectCriteriaCountDisabled5.ShowWindow(SW_HIDE);
						m_EjectCriteriaCount5.ShowWindow(SW_HIDE);
						m_EjectCriteriaCountLabel5.ShowWindow(SW_HIDE);
				break;
				case 7:
						m_EjectCriteriaCountDisabled6.ShowWindow(SW_HIDE);
						m_EjectCriteriaCount6.ShowWindow(SW_HIDE);
						m_EjectCriteriaCountLabel6.ShowWindow(SW_HIDE);
				break;
				case 8:
						m_EjectCriteriaCountDisabled7.ShowWindow(SW_HIDE);
						m_EjectCriteriaCount7.ShowWindow(SW_HIDE);
						m_EjectCriteriaCountLabel7.ShowWindow(SW_HIDE);
				break;
				case 9:
						m_EjectCriteriaCountDisabled8.ShowWindow(SW_HIDE);
						m_EjectCriteriaCount8.ShowWindow(SW_HIDE);
						m_EjectCriteriaCountLabel8.ShowWindow(SW_HIDE);
				break;
				case 10:
						m_EjectCriteriaCountDisabled9.ShowWindow(SW_HIDE);
						m_EjectCriteriaCount9.ShowWindow(SW_HIDE);
						m_EjectCriteriaCountLabel9.ShowWindow(SW_HIDE);
				break;
				case 11:
						m_EjectCriteriaCountDisabled10.ShowWindow(SW_HIDE);
						m_EjectCriteriaCount10.ShowWindow(SW_HIDE);
						m_EjectCriteriaCountLabel10.ShowWindow(SW_HIDE);
				break;
				case 12:
						m_EjectCriteriaCountDisabled11.ShowWindow(SW_HIDE);
						m_EjectCriteriaCount11.ShowWindow(SW_HIDE);
						m_EjectCriteriaCountLabel11.ShowWindow(SW_HIDE);
				break;
				case 13:
						m_EjectCriteriaCountDisabled12.ShowWindow(SW_HIDE);
						m_EjectCriteriaCount12.ShowWindow(SW_HIDE);
						m_EjectCriteriaCountLabel12.ShowWindow(SW_HIDE);
				break;
				case 14:
						m_EjectCriteriaCountDisabled13.ShowWindow(SW_HIDE);
						m_EjectCriteriaCount13.ShowWindow(SW_HIDE);
						m_EjectCriteriaCountLabel13.ShowWindow(SW_HIDE);
				break;
				case 15:
						m_EjectCriteriaCountDisabled14.ShowWindow(SW_HIDE);
						m_EjectCriteriaCount14.ShowWindow(SW_HIDE);
						m_EjectCriteriaCountLabel14.ShowWindow(SW_HIDE);
				break;
				case 16:
						m_EjectCriteriaCountDisabled15.ShowWindow(SW_HIDE);
						m_EjectCriteriaCount15.ShowWindow(SW_HIDE);
						m_EjectCriteriaCountLabel15.ShowWindow(SW_HIDE);
				break;
			}
		}
*/
	}
	else
	{ //no product, so display nothing
		m_EjectCriteriaCountLabel15.ShowWindow(SW_HIDE);
		m_EjectCriteriaCount15.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountLabel14.ShowWindow(SW_HIDE);
		m_EjectCriteriaCount14.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountLabel13.ShowWindow(SW_HIDE);
		m_EjectCriteriaCount13.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountLabel12.ShowWindow(SW_HIDE);
		m_EjectCriteriaCount12.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountLabel11.ShowWindow(SW_HIDE);
		m_EjectCriteriaCount11.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountLabel10.ShowWindow(SW_HIDE);
		m_EjectCriteriaCount10.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountLabel9.ShowWindow(SW_HIDE);
		m_EjectCriteriaCount9.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountLabel8.ShowWindow(SW_HIDE);
		m_EjectCriteriaCount8.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountLabel7.ShowWindow(SW_HIDE);
		m_EjectCriteriaCount7.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountLabel6.ShowWindow(SW_HIDE);
		m_EjectCriteriaCount6.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountLabel5.ShowWindow(SW_HIDE);
		m_EjectCriteriaCount5.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountLabel4.ShowWindow(SW_HIDE);
		m_EjectCriteriaCount4.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountLabel3.ShowWindow(SW_HIDE);
		m_EjectCriteriaCount3.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountLabel2.ShowWindow(SW_HIDE);
		m_EjectCriteriaCount2.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountLabel1.ShowWindow(SW_HIDE);
		m_EjectCriteriaCount1.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountLabel0.ShowWindow(SW_HIDE);
		m_EjectCriteriaCount0.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountDisabled0.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountDisabled1.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountDisabled2.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountDisabled3.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountDisabled4.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountDisabled5.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountDisabled6.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountDisabled7.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountDisabled8.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountDisabled9.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountDisabled10.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountDisabled11.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountDisabled12.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountDisabled13.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountDisabled14.ShowWindow(SW_HIDE);
		m_EjectCriteriaCountDisabled15.ShowWindow(SW_HIDE);
	}
}

void CProductionSummaryDialog::UpdateClearYellowMessageButton() 
{
	if (vOldYellowMessageButtonYellow != vLocalSystemData->vYellowMessageButtonYellow)
	{
		vOldYellowMessageButtonYellow = vLocalSystemData->vYellowMessageButtonYellow;

		if (vOldYellowMessageButtonYellow)
		{
			m_SubFunction6Button.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_StatusLine, vLocalSystemData->vLastErrorMessage);
			CString TempText = "-";
			TempText.LoadString(IDS_ClearYellowMessage);
			SetDlgItemText(IDC_SubFunction6Button, TempText);
			m_MissedColumnLabel.ShowWindow(SW_HIDE);
			m_EjectedColumnLabel.ShowWindow(SW_HIDE);
		}
	}
	if (!vOldYellowMessageButtonYellow)
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			m_SubFunction6Button.ShowWindow(SW_SHOW);
			CString TempText = "-";
			TempText.LoadString(IDS_SystemSetupSummary);
			SetDlgItemText(IDC_SubFunction6Button,TempText);
		}
		else
		{
			m_SubFunction6Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
		}

		CString TempText = "-";
		vOldLastLearnState = vMainWindowPointer->vLastLearnState;
		if ((vOldLastLearnState) && (vOldLastLearnState != 255)) //colors for Improving Learn - Do not test contaminants indicator
		{
			TempText.LoadString(IDS_ImprovingLearnDonottestsmallcontaminants);
			SetDlgItemText(IDC_StatusLine, TempText);
			m_MissedColumnLabel.ShowWindow(SW_HIDE);
			m_EjectedColumnLabel.ShowWindow(SW_HIDE);
		}
		else
		{
			SetDlgItemText(IDC_StatusLine, _T(""));
			if ((vLocalSystemData->vMissedEjectsBecauseDisabledCount) && ((vOldLastLearnState == 0) || (vOldLastLearnState == 255)))
			{
				m_MissedColumnLabel.ShowWindow(SW_SHOW);
				m_EjectedColumnLabel.ShowWindow(SW_SHOW);
			}
			else
			{
				m_MissedColumnLabel.ShowWindow(SW_HIDE);
				m_EjectedColumnLabel.ShowWindow(SW_HIDE);
			}
		}
	}
}

void CProductionSummaryDialog::UpdateDisableEjectorsButton()
{
	CString TempText = "-";
	if (vLocalConfigurationData->vEnableEjectors)
	{
		if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || ((vGlobalCurrentProduct) && (vGlobalCurrentProduct->vBulkProductMode)))
			TempText.LoadString(IDS_DisableEjectors);
		else
			TempText.LoadString(IDS_DisableContainerEjectors);
		SetDlgItemText(IDC_SubFunction2Button,TempText);

		m_EjectorsDisabled.ShowWindow(SW_HIDE);
	}
	else
	{
		if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || ((vGlobalCurrentProduct) && (vGlobalCurrentProduct->vBulkProductMode)))
			TempText.LoadString(IDS_EnableEjectors);
		else
			TempText.LoadString(IDS_EnableContainerEjectors);
		SetDlgItemText(IDC_SubFunction2Button,TempText);
		m_EjectorsDisabled.ShowWindow(SW_SHOW);
	}
}

void CProductionSummaryDialog::SetupBulkModeAndMainMenuScreen()
{
	if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || ((vGlobalCurrentProduct) && (vGlobalCurrentProduct->vBulkProductMode))) //Continuous Feed, or a Pipeline
	//	m_PercentEjected.SetWindowPos(NULL,245,107,49,20,SWP_NOZORDER);
	//else
		m_PercentEjected.SetWindowPos(NULL,245,107,49,20,SWP_NOZORDER);

	CString TempText = " ";
	if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || ((vGlobalCurrentProduct) && (vGlobalCurrentProduct->vBulkProductMode))) //Continuous Feed, or a Pipeline
	{
		TempText.LoadString(IDS_RunTime);
		TempText = TempText + ":";
		SetDlgItemText(IDC_TotalContainersLabel,TempText);
		SetDlgItemText(IDC_TotalContainers, _T(""));
		CString TempText2 = " ";
		TempText2.LoadString(IDS_LastReject);
		TempText2 = TempText2 + ":";
		SetDlgItemText(IDC_PercentEjectedLabel,TempText2);
		SetDlgItemText(IDC_PercentRejected, _T(""));
	}
	else
	{
		TempText.LoadString(IDS_Total);
		SetDlgItemText(IDC_TotalContainersLabel,TempText + ":");
		SetDlgItemText(IDC_TotalContainers,IToAWithComma(vLocalSystemData->vOldTotalContainers));// if not a pipe show total

		CString TempText2 = " ";
		TempText2.LoadString(IDS_Ejected);
		SetDlgItemText(IDC_PercentEjectedLabel,"% " + TempText2 + ":");

		if (vLocalSystemData->vTotalContainers > 0)
		{
			vLocalSystemData->vPercentEjected = ((double)vLocalSystemData->vEjectCountTotal / (double)vLocalSystemData->vTotalContainers) * 100.0;
			if (vLocalSystemData->vPercentEjected > 100)
				vLocalSystemData->vPercentEjected = 100;
		}
		else
			vLocalSystemData->vPercentEjected = 0;
	}

	::SetupMenu(vLocalCWndCollection);
}

