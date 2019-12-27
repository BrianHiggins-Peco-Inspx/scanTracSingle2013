// FillerValveDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "FillerValveDialog.h"
#include "afxdialogex.h"
#include "ChartCtrl/ChartCtrl.h"
#include "ChartCtrl/ChartLineSerie.h"
#include "ChartCtrl/ChartBarSerie.h"
#include "ChartCtrl/ChartAxis.h"
#include "ChartCtrl/ChartAxisLabel.h"
#include "ChartCtrl/ChartLegend.h"
#include <vector>
#include "NumericEntryDialog.h"
#include <iostream>
#include <math.h>
#include "SelectItemDialog.h"


// CFillerValveDialog dialog

IMPLEMENT_DYNAMIC(CFillerValveDialog, CDialogEx)

extern unsigned long vGlobalDialogTitleColor;
extern CProduct *vGlobalCurrentProduct;
extern BYTE vGlobalShiftKeyDown;

CFillerValveDialog::CFillerValveDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFillerValveDialog::IDD, pParent)
{
	vDisplayValveNumber = 1;
	vMainWindowPointer = NULL;
	vLocalSystemData = NULL;
	vLocalConfigurationData = NULL;
	vTime = CTime::GetCurrentTime();
	vDateToday = vTime.GetDay();
	vUpdateDisplayTimerHandle = 31982;
	vRefreshDisplayTimerHandle = 31983;
	const COLORREF cAqua = 0x00D7BF17;
	vAquaBrush = ::CreateSolidBrush(cAqua);
	vOldNumberOfValvesThatNeedAttentionMonitor=0;
	vOldFillerAverage = 0;
	vOldNumberofDataRows = 0;
	vValveSortOrder = 1; //default 1 sort by valve #, 2 sort by most full, 3 sort by least full
	vRefreshSoon = 0;
	vDisplayMode = 1;  //1 All Valve Plot, 2 All Valve Standard Deviation Plot, 3 One Valve Plot, 4 All Valve Histogram,

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_ShadedButton6);
	vLocalCWndCollection.Add(&m_ShadedButton5);
	vLocalCWndCollection.Add(&m_ShadedButton4);
	vLocalCWndCollection.Add(&m_ShadedButton3);
	vLocalCWndCollection.Add(&m_ShadedButton2);
	vLocalCWndCollection.Add(&m_ShadedButton1);
	vLocalCWndCollection.Add(&m_ShadedButton7);
	vLocalCWndCollection.Add(&m_ShadedButton7Selected);
	vLocalCWndCollection.Add(&m_ShadedButton1Selected);
	vLocalCWndCollection.Add(&m_ShadedButton2Selected);
	vLocalCWndCollection.Add(&m_ShadedButton3Selected);
	vLocalCWndCollection.Add(&m_ShadedButton4Selected);
	vLocalCWndCollection.Add(&m_MainBackground2);
	vLocalCWndCollection.Add(&m_Border23);
	vLocalCWndCollection.Add(&m_Border19);
	vLocalCWndCollection.Add(&m_Border20);
	vLocalCWndCollection.Add(&m_Border21);
	vLocalCWndCollection.Add(&m_Border25);
	vLocalCWndCollection.Add(&m_Border30);
	vLocalCWndCollection.Add(&m_ForiegnObjectCount);
	vLocalCWndCollection.Add(&m_Border45);
	vLocalCWndCollection.Add(&m_Border46);
	vLocalCWndCollection.Add(&m_Border52);
	vLocalCWndCollection.Add(&m_Border54);
	vLocalCWndCollection.Add(&m_Border64);
	vLocalCWndCollection.Add(&m_Border63);
	vLocalCWndCollection.Add(&m_AverageFill);
	vLocalCWndCollection.Add(&m_AverageFillLabel);
	vLocalCWndCollection.Add(&m_BackButton);
	vLocalCWndCollection.Add(&m_Border);
	vLocalCWndCollection.Add(&m_Border10);
	vLocalCWndCollection.Add(&m_Border11);
	vLocalCWndCollection.Add(&m_Border12);
	vLocalCWndCollection.Add(&m_Border13);
	vLocalCWndCollection.Add(&m_Border14);
	vLocalCWndCollection.Add(&m_Border15);
	vLocalCWndCollection.Add(&m_Border16);
	vLocalCWndCollection.Add(&m_Border17);
	vLocalCWndCollection.Add(&m_Border18);
	vLocalCWndCollection.Add(&m_Border2);
	vLocalCWndCollection.Add(&m_Border24);
	vLocalCWndCollection.Add(&m_Border26);
	vLocalCWndCollection.Add(&m_Border27);
	vLocalCWndCollection.Add(&m_Border28);
	vLocalCWndCollection.Add(&m_Border29);
	vLocalCWndCollection.Add(&m_Border3);
	vLocalCWndCollection.Add(&m_Border31);
	vLocalCWndCollection.Add(&m_Border32);
	vLocalCWndCollection.Add(&m_Border33);
	vLocalCWndCollection.Add(&m_Border34);
	vLocalCWndCollection.Add(&m_Border35);
	vLocalCWndCollection.Add(&m_Border36);
	vLocalCWndCollection.Add(&m_Border38);
	vLocalCWndCollection.Add(&m_Border37);
	vLocalCWndCollection.Add(&m_Border39);
	vLocalCWndCollection.Add(&m_Border4);
	vLocalCWndCollection.Add(&m_Border40);
	vLocalCWndCollection.Add(&m_Border41);
	vLocalCWndCollection.Add(&m_Border42);
	vLocalCWndCollection.Add(&m_Border43);
	vLocalCWndCollection.Add(&m_Border44);
	vLocalCWndCollection.Add(&m_Border47);
	vLocalCWndCollection.Add(&m_Border48);
	vLocalCWndCollection.Add(&m_Border49);
	vLocalCWndCollection.Add(&m_Border5);
	vLocalCWndCollection.Add(&m_Border50);
	vLocalCWndCollection.Add(&m_Border51);
	vLocalCWndCollection.Add(&m_Border53);
	vLocalCWndCollection.Add(&m_Border55);
	vLocalCWndCollection.Add(&m_Border56);
	vLocalCWndCollection.Add(&m_Border57);
	vLocalCWndCollection.Add(&m_Border58);
	vLocalCWndCollection.Add(&m_Border59);
	vLocalCWndCollection.Add(&m_Border6);
	vLocalCWndCollection.Add(&m_Border60);
	vLocalCWndCollection.Add(&m_Border61);
	vLocalCWndCollection.Add(&m_Border62);
	vLocalCWndCollection.Add(&m_Border65);
	vLocalCWndCollection.Add(&m_Border66);
	vLocalCWndCollection.Add(&m_Border67);
	vLocalCWndCollection.Add(&m_Border7);
	vLocalCWndCollection.Add(&m_Border9);
	vLocalCWndCollection.Add(&m_Border8);
	vLocalCWndCollection.Add(&m_Date);
	vLocalCWndCollection.Add(&m_FillerValveNumber);
	vLocalCWndCollection.Add(&m_FillerValveNumberLabel);
	vLocalCWndCollection.Add(&m_ForiegnObjectLabel);
	vLocalCWndCollection.Add(&m_Header);
	vLocalCWndCollection.Add(&m_Header2);
	vLocalCWndCollection.Add(&m_Header4);
	vLocalCWndCollection.Add(&m_Header5);
	vLocalCWndCollection.Add(&m_Header6);
	vLocalCWndCollection.Add(&m_Header7);
	vLocalCWndCollection.Add(&m_HeaderStatus);
	vLocalCWndCollection.Add(&m_InspectedCount);
	vLocalCWndCollection.Add(&m_InspectedCountLabel);
	vLocalCWndCollection.Add(&m_MainMenu);
	vLocalCWndCollection.Add(&m_NumberofSamplesLabel);
	vLocalCWndCollection.Add(&m_NumberofSamples);
	vLocalCWndCollection.Add(&m_OverfillCount1);
	vLocalCWndCollection.Add(&m_OverfillLabel);
	vLocalCWndCollection.Add(&m_PercentRejected);
	vLocalCWndCollection.Add(&m_PercentRejectedLabel);
	vLocalCWndCollection.Add(&m_StandardDeviation);
	vLocalCWndCollection.Add(&m_StandardDeviationLabel);
	vLocalCWndCollection.Add(&m_StatusLabel);
	vLocalCWndCollection.Add(&m_StatusLine1);
	vLocalCWndCollection.Add(&m_Time);
	vLocalCWndCollection.Add(&m_UnderfillCount1);
	vLocalCWndCollection.Add(&m_UnderfillLabel);
	vLocalCWndCollection.Add(&m_Border22a);
	vLocalCWndCollection.Add(&m_ChartCtrl);
}

CFillerValveDialog::~CFillerValveDialog()
{
	DeleteObject(vAquaBrush);
}

void CFillerValveDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ShadedButton4, m_ShadedButton4);
	DDX_Control(pDX, IDC_ShadedButton3, m_ShadedButton3);
	DDX_Control(pDX, IDC_ShadedButton2, m_ShadedButton2);
	DDX_Control(pDX, IDC_ShadedButton1, m_ShadedButton1);
	DDX_Control(pDX, IDC_ShadedButton1Selected, m_ShadedButton1Selected);
	DDX_Control(pDX, IDC_ShadedButton2Selected, m_ShadedButton2Selected);
	DDX_Control(pDX, IDC_ShadedButton3Selected, m_ShadedButton3Selected);
	DDX_Control(pDX, IDC_ShadedButton4Selected, m_ShadedButton4Selected);
	DDX_Control(pDX, IDC_MainBackground2, m_MainBackground2);
	DDX_Control(pDX, IDC_Border23, m_Border23);
	DDX_Control(pDX, IDC_Border19, m_Border19);
	DDX_Control(pDX, IDC_Border20, m_Border20);
	DDX_Control(pDX, IDC_Border21, m_Border21);
	DDX_Control(pDX, IDC_Border25, m_Border25);
	DDX_Control(pDX, IDC_Border30, m_Border30);
	DDX_Control(pDX, IDC_ShadedButton5, m_ShadedButton5);
	DDX_Control(pDX, IDC_ShadedButton6, m_ShadedButton6);
	DDX_Control(pDX, IDC_CHARTCTRL, m_ChartCtrl);
	DDX_Control(pDX, IDC_Border45, m_Border45);
	DDX_Control(pDX, IDC_Border46, m_Border46);
	DDX_Control(pDX, IDC_Border52, m_Border52);
	DDX_Control(pDX, IDC_Border54, m_Border54);
	DDX_Control(pDX, IDC_Border64, m_Border64);
	DDX_Control(pDX, IDC_Border63, m_Border63);
	DDX_Control(pDX, IDC_AverageFill, m_AverageFill);
	DDX_Control(pDX, IDC_AverageFillLabel, m_AverageFillLabel);
	DDX_Control(pDX, IDC_BackButton, m_BackButton);
	DDX_Control(pDX, IDC_Border, m_Border);
	DDX_Control(pDX, IDC_Border, m_Border);
	DDX_Control(pDX, IDC_Border10, m_Border10);
	DDX_Control(pDX, IDC_Border11, m_Border11);
	DDX_Control(pDX, IDC_Border12, m_Border12);
	DDX_Control(pDX, IDC_Border13, m_Border13);
	DDX_Control(pDX, IDC_Border14, m_Border14);
	DDX_Control(pDX, IDC_Border15, m_Border15);
	DDX_Control(pDX, IDC_Border16, m_Border16);
	DDX_Control(pDX, IDC_Border17, m_Border17);
	DDX_Control(pDX, IDC_Border18, m_Border18);
	DDX_Control(pDX, IDC_Border2, m_Border2);
	DDX_Control(pDX, IDC_Border24, m_Border24);
	DDX_Control(pDX, IDC_Border26, m_Border26);
	DDX_Control(pDX, IDC_Border27, m_Border27);
	DDX_Control(pDX, IDC_Border28, m_Border28);
	DDX_Control(pDX, IDC_Border29, m_Border29);
	DDX_Control(pDX, IDC_Border3, m_Border3);
	DDX_Control(pDX, IDC_Border31, m_Border31);
	DDX_Control(pDX, IDC_Border32, m_Border32);
	DDX_Control(pDX, IDC_Border33, m_Border33);
	DDX_Control(pDX, IDC_Border34, m_Border34);
	DDX_Control(pDX, IDC_Border35, m_Border35);
	DDX_Control(pDX, IDC_Border36, m_Border36);
	DDX_Control(pDX, IDC_Border38, m_Border38);
	DDX_Control(pDX, IDC_Border37, m_Border37);
	DDX_Control(pDX, IDC_Border39, m_Border39);
	DDX_Control(pDX, IDC_Border4, m_Border4);
	DDX_Control(pDX, IDC_Border40, m_Border40);
	DDX_Control(pDX, IDC_Border41, m_Border41);
	DDX_Control(pDX, IDC_Border42, m_Border42);
	DDX_Control(pDX, IDC_Border43, m_Border43);
	DDX_Control(pDX, IDC_Border44, m_Border44);
	DDX_Control(pDX, IDC_Border47, m_Border47);
	DDX_Control(pDX, IDC_Border48, m_Border48);
	DDX_Control(pDX, IDC_Border49, m_Border49);
	DDX_Control(pDX, IDC_Border5, m_Border5);
	DDX_Control(pDX, IDC_Border50, m_Border50);
	DDX_Control(pDX, IDC_Border51, m_Border51);
	DDX_Control(pDX, IDC_Border53, m_Border53);
	DDX_Control(pDX, IDC_Border55, m_Border55);
	DDX_Control(pDX, IDC_Border56, m_Border56);
	DDX_Control(pDX, IDC_Border57, m_Border57);
	DDX_Control(pDX, IDC_Border58, m_Border58);
	DDX_Control(pDX, IDC_Border59, m_Border59);
	DDX_Control(pDX, IDC_Border6, m_Border6);
	DDX_Control(pDX, IDC_Border60, m_Border60);
	DDX_Control(pDX, IDC_Border61, m_Border61);
	DDX_Control(pDX, IDC_Border62, m_Border62);
	DDX_Control(pDX, IDC_Border65, m_Border65);
	DDX_Control(pDX, IDC_Border66, m_Border66);
	DDX_Control(pDX, IDC_Border67, m_Border67);
	DDX_Control(pDX, IDC_Border7, m_Border7);
	DDX_Control(pDX, IDC_Border9, m_Border9);
	DDX_Control(pDX, IDC_Border8, m_Border8);
	DDX_Control(pDX, IDC_Date, m_Date);
	DDX_Control(pDX, IDC_FillerValveNumber, m_FillerValveNumber);
	DDX_Control(pDX, IDC_FillerValveNumberLabel, m_FillerValveNumberLabel);
	DDX_Control(pDX, IDC_ForiegnObjectCount, m_ForiegnObjectCount);
	DDX_Control(pDX, IDC_ForiegnObjectLabel, m_ForiegnObjectLabel);
	DDX_Control(pDX, IDC_Header, m_Header);
	DDX_Control(pDX, IDC_Header2, m_Header2);
	DDX_Control(pDX, IDC_Header4, m_Header4);
	DDX_Control(pDX, IDC_Header5, m_Header5);
	DDX_Control(pDX, IDC_Header6, m_Header6);
	DDX_Control(pDX, IDC_Header7, m_Header7);
	DDX_Control(pDX, IDC_HeaderStatus, m_HeaderStatus);
	DDX_Control(pDX, IDC_InspectedCount, m_InspectedCount);
	DDX_Control(pDX, IDC_InspectedCountLabel, m_InspectedCountLabel);
	DDX_Control(pDX, IDC_MainMenu, m_MainMenu);
	DDX_Control(pDX, IDC_NumberofSamplesLabel, m_NumberofSamplesLabel);
	DDX_Control(pDX, IDC_NumberOfSamples, m_NumberofSamples);
	DDX_Control(pDX, IDC_OverfillCount, m_OverfillCount1);
	DDX_Control(pDX, IDC_OverfillLabel, m_OverfillLabel);
	DDX_Control(pDX, IDC_PercentRejected, m_PercentRejected);
	DDX_Control(pDX, IDC_PercentRejectLabel, m_PercentRejectedLabel);
	DDX_Control(pDX, IDC_StandardDeviation, m_StandardDeviation);
	DDX_Control(pDX, IDC_StandardDeviationLabel, m_StandardDeviationLabel);
	DDX_Control(pDX, IDC_StatusLabel, m_StatusLabel);
	DDX_Control(pDX, IDC_StatusLine1, m_StatusLine1);
	DDX_Control(pDX, IDC_Time, m_Time);
	DDX_Control(pDX, IDC_UnderfillCount, m_UnderfillCount1);
	DDX_Control(pDX, IDC_UnderfillLabel, m_UnderfillLabel);
	DDX_Control(pDX, IDC_Border22, m_Border22a);
	DDX_Control(pDX, IDC_ShadedButton7, m_ShadedButton7);
	DDX_Control(pDX, IDC_ShadedButton7Selected, m_ShadedButton7Selected);
}


BEGIN_MESSAGE_MAP(CFillerValveDialog, CDialogEx)\
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_ShadedButton1, &CFillerValveDialog::OnShadedbutton1)
	ON_STN_CLICKED(IDC_ShadedButton2, &CFillerValveDialog::OnShadedbutton2)
	ON_STN_CLICKED(IDC_ShadedButton3, &CFillerValveDialog::OnShadedbutton3)
	ON_STN_CLICKED(IDC_ShadedButton4, &CFillerValveDialog::OnShadedbutton4)
	ON_STN_CLICKED(IDC_MainMenu, &CFillerValveDialog::OnMainmenu)
	ON_STN_CLICKED(IDC_ShadedButton5, &CFillerValveDialog::OnShadedbutton5)
	ON_STN_CLICKED(IDC_BackButton, &CFillerValveDialog::OnBackbutton)
	ON_STN_CLICKED(IDC_ShadedButton6, &CFillerValveDialog::OnStnClickedShadedbutton6)
	ON_STN_CLICKED(IDC_ShadedButton7, &CFillerValveDialog::OnStnClickedShadedbutton7)
	ON_STN_CLICKED(IDC_ShadedButton7Selected, &CFillerValveDialog::OnStnClickedShadedbutton7selected)
	ON_STN_CLICKED(IDC_ShadedButton4Selected, &CFillerValveDialog::OnStnClickedShadedbutton4selected)
	ON_STN_CLICKED(IDC_ShadedButton3Selected, &CFillerValveDialog::OnStnClickedShadedbutton3selected)
	ON_STN_CLICKED(IDC_ShadedButton2Selected, &CFillerValveDialog::OnStnClickedShadedbutton2selected)
	ON_STN_CLICKED(IDC_ShadedButton1Selected, &CFillerValveDialog::OnStnClickedShadedbutton1selected)
END_MESSAGE_MAP()


// CFillerValveDialog message handlers




HBRUSH CFillerValveDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	 // Call the base class implementation first! Otherwise, it may
  // undo what we're trying to accomplish here.
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	int TempControlID = pWnd->GetDlgCtrlID();
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);

	//if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText)
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText1, 8);
	//	if (vGlobalShiftKeyDown)
	//		pDC->SetTextColor(cBlack);
	//	else
	//	pDC->SetTextColor(vGlobalDialogTitleColor);
	//}
	//
	if (TempControlID == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
//		return vLightBackgroundBrush;
	}
	//if (TempControlID == IDC_CurrentProductLabel)
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentProductLabel, 4);  //5 is medium large
	//	pDC->SetBkMode(TRANSPARENT);
	//	pDC->SetTextColor(cBlack);
	//	return vLocalSystemData->vWhiteBrush;
	//}
	if (TempControlID == IDC_AverageFillLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_AverageFillLabel, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	if (TempControlID == IDC_AverageFill) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_AverageFill, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	if (TempControlID == IDC_StandardDeviationLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_StandardDeviationLabel, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	if (TempControlID == IDC_StandardDeviation) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_StandardDeviation, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	if (TempControlID == IDC_NumberofSamplesLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_NumberofSamplesLabel, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	if (TempControlID == IDC_NumberOfSamples) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_NumberofSamples, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	//if (TempControlID == IDC_ProductNameLabel)
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ProductNameLabel, 4);  //5 is medium large
	//	pDC->SetBkMode(TRANSPARENT);
	//	pDC->SetTextColor(cBlack);
	//	return vLocalSystemData->vWhiteBrush;
	//}
	//if (TempControlID == IDC_ProductName) 
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ProductName, 4);  //5 is medium large
	//	pDC->SetBkMode(TRANSPARENT);
	//	pDC->SetTextColor(cBlack);
	//	return vLocalSystemData->vWhiteBrush;
	//}
	if (TempControlID == IDC_InspectedCountLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_InspectedCountLabel, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	if (TempControlID == IDC_InspectedCount) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_InspectedCount, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	if (TempControlID == IDC_PercentRejectLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_PercentRejectedLabel, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	if (TempControlID == IDC_PercentRejected) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_PercentRejected, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	if (TempControlID == IDC_Date) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Date, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	if (TempControlID == IDC_Time) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Time, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	if (TempControlID == IDC_MainBackground2) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MainBackground2, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	if (TempControlID == IDC_OverfillCount) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_OverfillCount1, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	if (TempControlID == IDC_OverfillLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_OverfillLabel, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	if (TempControlID == IDC_UnderfillCount) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_UnderfillCount1, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	if (TempControlID == IDC_UnderfillLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_UnderfillLabel, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	if (TempControlID == IDC_ForiegnObjectCount) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ForiegnObjectCount, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	if (TempControlID == IDC_ForiegnObjectLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ForiegnObjectLabel, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	if (TempControlID == IDC_FillerValveNumber) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FillerValveNumber, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	if (TempControlID == IDC_FillerValveNumberLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FillerValveNumberLabel, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	if (TempControlID == IDC_StatusLine1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_StatusLine1, 4);  //5 is medium large
		if (vOldNumberOfValvesThatNeedAttentionMonitor)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cBlack);
			return vLocalSystemData->vYellowBrush;
		}
		else 
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vLightGreenBrush;
		}
	}
	if (TempControlID == IDC_StatusLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_StatusLabel, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	
	if (TempControlID == IDC_StatusLine1) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_StatusLine1, 5);  //5 is medium large
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}
	//if (TempControlID == IDC_StatusLine2)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_StatusLine2, 4);  //5 is medium large
	//	pDC->SetTextColor(cBlack);
	//	return vLocalSystemData->vWhiteBrush;
	//}
	if (TempControlID == IDC_HeaderStatus)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_HeaderStatus, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vWhiteBrush;
	}

	return hbr;
}


void CFillerValveDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages

	double * TempValveAverages = 0;
	m_ChartCtrl.EnableRefresh(false);

	//vDisplayMode, 1 All Valve Plot, 2 All Valve Standard Deviation Plot, 3 One Valve Plot, 4 All Valve Histogram,
	if (vDisplayMode == 1)
	{
		vMainWindowPointer->CalculateValveStatus(); //pointer to scantrac dlg
		m_ChartCtrl.RemoveAllSeries();//First empty

		CChartStandardAxis* pBottomAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
		//pBottomAxis->SetAutomatic(true);
		//pBottomAxis->SetAutomaticMode(CChartAxis::FullAutomatic);
		pBottomAxis->SetMinMax(0, vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] + 1);

		pBottomAxis->EnableScrollBar(true);
		pBottomAxis->SetAutoHideScrollBar(true);

		CChartStandardAxis* pLeftAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);

		//create labels
		CString TempUnits = "(gm)";
		if (vGlobalCurrentProduct->vFillerMonitorInspection)
			TempUnits = " ";
		else
		if (vLocalSystemData->vValveMonitorInspection <= vGlobalCurrentProduct->vNumberOfInspections)
		if (vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection])
			TempUnits = "(" + vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection]->vCheckWeighUnits + ")";

		std::wstring TempText = _T("Avg. Fill Level ") + TempUnits;
		pLeftAxis->GetLabel()->SetText(TempText);
		pBottomAxis->GetLabel()->SetText(_T("Valve Number"));
		//draw threshold lines
		m_ChartCtrl.SetBackColor(RGB(255,255,255));
		m_ChartCtrl.SetBorderColor(RGB(255,255,255));

		CChartLineSerie *pLineSerie1 = NULL;
		if (vLocalSystemData->vFillerHighThreshold > 0)
		{
			pLineSerie1 = m_ChartCtrl.CreateLineSerie();
			pLineSerie1->SetColor(RGB(255,0,0));
		}

		CChartLineSerie *pLineSerie2 = NULL;
		if (vLocalSystemData->vFillerLowThreshold > 0)
		{
			pLineSerie2 = m_ChartCtrl.CreateLineSerie();
			pLineSerie2->SetColor(RGB(255,0,0));
		}
		CChartBarSerie* pBarSeries1 = m_ChartCtrl.CreateBarSerie();
		//pBarSeries1->SetColor(RGB(0,255,0));

		RECT TempRectCoor;
		m_ChartCtrl.GetWindowRect(&TempRectCoor);
		double TempWidthOfChart =TempRectCoor.right-TempRectCoor.left;
		WORD TempBarWidth = (WORD)(TempWidthOfChart / vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]);
		if (TempBarWidth > 1)
			TempBarWidth = TempBarWidth - 1;
		if (TempBarWidth > 1)
			TempBarWidth = TempBarWidth - 1;
		if (TempBarWidth < 1)
			TempBarWidth = 1;
		if (TempBarWidth > 20)
			TempBarWidth = 20;

		if (vValveSortOrder == 1)	//(vDisplayMode == 1) - fill monitor / valve order 1 - default by valve number
			pBarSeries1->SetBarWidth(TempBarWidth);
		else
			pBarSeries1->SetBarWidth(20);
		
		//initialize the minimum and maximum value variables
		double tempmin = 9999999;
		double tempmax = 0;
		
		if (vLocalSystemData->vValveAverages < 0)
			vLocalSystemData->vValveAverages = 0;

		if (vLocalSystemData->vValveAverages)
			//create array with size number of valves
		if (vValveSortOrder == 1)	//(vDisplayMode == 1) - fill monitor / valve order 1 - default by valve number
		{
			if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] < 40)
				pBottomAxis->SetTickIncrement(false, 1.0);
			else
				pBottomAxis->SetTickIncrement(false, 10);

			for (int i=0; i<vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; i++)
			{
				double xval;
				xval = i+1;
				pBarSeries1->AddPoint(xval, vLocalSystemData->vValveAverages[ i ]);

				if (vLocalSystemData->vFillerHighThreshold > 0)
					pLineSerie1->AddPoint(xval, vLocalSystemData->vFillerHighThreshold);
				if (vLocalSystemData->vFillerLowThreshold > 0)
					pLineSerie2->AddPoint(xval, vLocalSystemData->vFillerLowThreshold);

				//discover the maximum and minimum values
				if (vLocalSystemData->vValveAverages[ i ] < tempmin)
					tempmin = vLocalSystemData->vValveAverages[ i ];
				if (vLocalSystemData->vValveAverages[ i ] >= tempmax)
					tempmax = vLocalSystemData->vValveAverages[ i ];
			}
			if (tempmin < 0)
				tempmin = 0;
				
			if (tempmax <= tempmin)
				tempmax = tempmin + 1;
			//scale the axis to be 5% lower than the minimum value and 5% higher than the maximum
			pLeftAxis->SetMinMax(tempmin * 0.95, tempmax * 1.05);
		}
		else 
		if (vValveSortOrder == 2)		//(vDisplayMode == 1) - fill monitor / valve order 2 - descending fill level
		{
			pBottomAxis->SetTickIncrement(false, 1.0);
			if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] > 16)
				pBottomAxis->SetMinMax(0, 16);

			int TempSortedArrayIndex = 0;
			int TempPlaceHolderValveNumber = 0;
			int TempPlaceHolderAverageFillLevel = 0;
			int TempFilledToIndex = 0;
			
			//create temporary arrays
			if (ThereIsEnoughMemory(sizeof(double) * vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType], "temp Color array in help dialog"))
				TempValveAverages = (double *)malloc(sizeof(double) * vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]);
			else
			{
				ReportErrorMessage("Not Enough Memory for temp vNumberOfHeadsSort in Help Dialog shift down",cEMailInspx, 32000);
				#ifdef _DEBUG
				afxDump << "Not Enough Memory for temp color array in Help Dialog shift down Alloc";
				#endif
			}
			

			//bubblesort
			for (int i = 0; i < vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; i++)
			{
				
				if (i == 0)		//put first data point into sorted array
				{
					TempValveAverages[TempSortedArrayIndex] = vLocalSystemData->vValveAverages[i];
					vValveLabels[TempSortedArrayIndex] = 1;
					TempFilledToIndex++;
				}
				else
				{
					for (TempSortedArrayIndex = 0; TempSortedArrayIndex < TempFilledToIndex; TempSortedArrayIndex++)	//find where to put next data point in the sorted array
					{
						if (TempValveAverages[TempSortedArrayIndex] < vLocalSystemData->vValveAverages[ i ])	//if the new value to insert is larger than this point in the sorted array
						{
							for (int j = TempFilledToIndex; j > TempSortedArrayIndex; j--)	//for loop pushes all data one down the array
							{
								TempValveAverages[j] = TempValveAverages[j - 1];
								vValveLabels[j] = vValveLabels[j - 1];
							}
							TempValveAverages[TempSortedArrayIndex] = vLocalSystemData->vValveAverages[i];	//put new entry in array
							vValveLabels[TempSortedArrayIndex] = i + 1;

							TempFilledToIndex++;
							break;
						}
						else if (TempSortedArrayIndex == TempFilledToIndex - 1)		//smallest average fill, put in last slot.
						{
							TempValveAverages[TempFilledToIndex] = vLocalSystemData->vValveAverages[ i ];	//put new entry in array
							vValveLabels[TempFilledToIndex] = i + 1;
							
							TempFilledToIndex++;
							break;
						}
					}
				}
			}

			pBottomAxis->vVolumeSort = 1;
			pBottomAxis->vValveLabels = vValveLabels;
			pBottomAxis->vNumberOfValves = vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType];
		
			for (int i = 0; i < vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; i++)
			{
				double xval;
				xval = i+1;
				pBarSeries1->AddPoint(xval, TempValveAverages[i]);
				if (vLocalSystemData->vFillerHighThreshold > 0)
					pLineSerie1->AddPoint(xval, vLocalSystemData->vFillerHighThreshold);
				if (vLocalSystemData->vFillerLowThreshold > 0)
					pLineSerie2->AddPoint(xval, vLocalSystemData->vFillerLowThreshold);
				//discover the maximum and minimum values
				if (vLocalSystemData->vValveAverages[i] < tempmin) { tempmin = vLocalSystemData->vValveAverages[i];}
				if (vLocalSystemData->vValveAverages[i] >= tempmax) { tempmax = vLocalSystemData->vValveAverages[i];}
			}
			//scale the axis to be 5% lower than the minimum value and 5% higher than the maximum
			pLeftAxis->SetMinMax(tempmin * 0.95, tempmax * 1.05);
		}
		else 
		if (vValveSortOrder == 3)	//(vDisplayMode == 1) - fill monitor / valve order 3 - ascending fill level
		{
			pBottomAxis->SetTickIncrement(false, 1.0);
			if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] > 16)
				pBottomAxis->SetMinMax(0, 16);

			int TempSortedArrayIndex = 0;
			int TempPlaceHolderValveNumber = 0;
			int TempPlaceHolderAverageFillLevel = 0;
			int TempFilledToIndex = 0;
			
			//create temporary arrays
			if (ThereIsEnoughMemory(sizeof(double) * vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType], "temp Color array in help dialog"))
				TempValveAverages = (double *)malloc(sizeof(double) * vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]);
			else
			{
				ReportErrorMessage("Not Enough Memory for temp vNumberOfHeadsSort in Help Dialog shift down",cEMailInspx, 32000);
				#ifdef _DEBUG
				afxDump << "Not Enough Memory for temp color array in Help Dialog shift down Alloc";
				#endif
			}

			//bubblesort
			for (int i = 0; i < vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; i++)
			{
				
				if (i == 0)		//put first data point into sorted array
				{
					TempValveAverages[TempSortedArrayIndex] = vLocalSystemData->vValveAverages[ i ];
					vValveLabels[TempSortedArrayIndex] = 1;
					TempFilledToIndex++;
				}
				else
				{
					for (TempSortedArrayIndex = 0; TempSortedArrayIndex < TempFilledToIndex; TempSortedArrayIndex++)	//find where to put next data point in the sorted array
					{
						if (vLocalSystemData->vValveAverages[i] < TempValveAverages[TempSortedArrayIndex])	//if the new value to insert is less than this point in the sorted array, then insert now
						{
							for (int j = TempFilledToIndex; j > TempSortedArrayIndex; j--)	//for loop pushes all data one down the array
							{
								TempValveAverages[j] = TempValveAverages[j - 1];
								vValveLabels[j] = vValveLabels[j - 1];
							}
							TempValveAverages[TempSortedArrayIndex] = vLocalSystemData->vValveAverages[i];	//put new entry in array
							vValveLabels[TempSortedArrayIndex] = i + 1;

							TempFilledToIndex++;
							break;
						}
						else if (TempSortedArrayIndex == TempFilledToIndex - 1)		//biggest average fill, put in last slot.
						{
							TempValveAverages[TempFilledToIndex] = vLocalSystemData->vValveAverages[ i ];	//put new entry in array
							vValveLabels[TempFilledToIndex] = i + 1;
							
							TempFilledToIndex++;
							break;
						}
					}
				}
			}

			pBottomAxis->vVolumeSort = 2; //brh was 1;
			pBottomAxis->vValveLabels = vValveLabels;
			pBottomAxis->vNumberOfValves = vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType];
		
			for (int i = 0; i < vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; i++)
			{
				double xval;
				xval = i+1;
				pBarSeries1->AddPoint(xval, TempValveAverages[i]);
				if (vLocalSystemData->vFillerHighThreshold > 0)
					pLineSerie1->AddPoint(xval, vLocalSystemData->vFillerHighThreshold);
				if (vLocalSystemData->vFillerLowThreshold > 0)
					pLineSerie2->AddPoint(xval, vLocalSystemData->vFillerLowThreshold);
				//discover the maximum and minimum values
				if (vLocalSystemData->vValveAverages[ i ] < tempmin) { tempmin = vLocalSystemData->vValveAverages[ i ];}
				if (vLocalSystemData->vValveAverages[ i ] >= tempmax) { tempmax = vLocalSystemData->vValveAverages[ i ];}
			}
			//scale the axis to be 5% lower than the minimum value and 5% higher than the maximum
			pLeftAxis->SetMinMax(tempmin * 0.95, tempmax * 1.05);
		}
		if (vLocalSystemData->vFillerHighThreshold > 0)
		{
			pLineSerie1->AddPoint(0, vLocalSystemData->vFillerHighThreshold);
			pLineSerie1->AddPoint(vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]+1, vLocalSystemData->vFillerHighThreshold);
			pLineSerie1->SetName(_T("High Threshold"));
			pLineSerie1->SetWidth(2);
		}
		if (vLocalSystemData->vFillerLowThreshold > 0)
		{
			pLineSerie2->AddPoint(0, vLocalSystemData->vFillerLowThreshold);
			pLineSerie2->AddPoint(vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]+1, vLocalSystemData->vFillerLowThreshold);
			pLineSerie2->SetName(_T("Low Threshold"));
			pLineSerie2->SetWidth(2);
		}
		pBarSeries1->SetColor(RGB(0,255,0));
	}
	else 
	if (vDisplayMode == 2)		//(vDisplayMode == 2) - standard deviation 
	{
		m_ChartCtrl.RemoveAllSeries();

		CChartStandardAxis* pBottomAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
		pBottomAxis->SetMinMax(0, vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] + 1);

		CChartStandardAxis* pLeftAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
		pLeftAxis->SetAutomaticMode(CChartAxis::FullAutomatic);

		m_ChartCtrl.SetBackColor(RGB(255,255,255));
		m_ChartCtrl.SetBorderColor(RGB(255,255,255));

				//create labels
		pLeftAxis->GetLabel()->SetText(_T("Standard Deviation"));
		pBottomAxis->GetLabel()->SetText(_T("Valve Number"));
		pBottomAxis->EnableScrollBar(true);
		pBottomAxis->SetAutoHideScrollBar(true);

		CChartBarSerie* pBarSeries1 = m_ChartCtrl.CreateBarSerie();
		//pBarSeries1->SetColor(RGB(0,0,200));

		RECT TempRectCoor;
		m_ChartCtrl.GetWindowRect(&TempRectCoor);
		double TempWidthOfChart =TempRectCoor.right-TempRectCoor.left;
		WORD TempBarWidth = (WORD)(TempWidthOfChart / vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]);
		if (TempBarWidth > 1)
			TempBarWidth = TempBarWidth - 1;
		if (TempBarWidth > 1)
			TempBarWidth = TempBarWidth - 1;
		if (TempBarWidth < 1)
			TempBarWidth = 1;
		if (TempBarWidth > 20)
			TempBarWidth = 20;

		if (vValveSortOrder == 1)	//(vDisplayMode == 1) - fill monitor / valve order 1 - default by valve number
			pBarSeries1->SetBarWidth(TempBarWidth);
		else
			pBarSeries1->SetBarWidth(20);
		
		//graph points if data present
		if(vLocalSystemData->vNumberValveRowDataHave > 2) 
		if (vValveSortOrder == 1)			//(vDisplayMode == 2) - standard deviation / valve order 1 - default by valve number
		{
			if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] < 40)
				pBottomAxis->SetTickIncrement(false, 1.0);
			else
				pBottomAxis->SetTickIncrement(false, 10);


			for (int i=0; i<vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; i++)
			{
				double xval;
				xval = i+1;
				pBarSeries1->AddPoint(xval , vLocalSystemData->vValveStandardDeviations[ i ]);
			}
		}
		else 
		if (vValveSortOrder == 2)		//(vDisplayMode == 2) - standard deviation / valve order 2 - descending standard deviation
		{
			pBottomAxis->SetTickIncrement(false, 1.0);
			if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] > 16)
				pBottomAxis->SetMinMax(0, 16);

			int TempSortedArrayIndex = 0;
			int TempPlaceHolderValveNumber = 0;
			int TempPlaceHolderAverageFillLevel = 0;
			int TempFilledToIndex = 0;

			//create temporary arrays
			if (ThereIsEnoughMemory(sizeof(double) * vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType], "temp Color array in help dialog"))
				TempValveAverages = (double *)malloc(sizeof(double) * vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]);
			else
			{
				ReportErrorMessage("Not Enough Memory for temp vNumberOfHeadsSort in Help Dialog shift down",cEMailInspx, 32000);
				#ifdef _DEBUG
				afxDump << "Not Enough Memory for temp color array in Help Dialog shift down Alloc";
				#endif
			}
			if (ThereIsEnoughMemory(sizeof(int) * vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType], "temp Color array in help dialog"))
				vValveLabels = (int *)malloc(sizeof(int) * vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]);
			else
			{
				ReportErrorMessage("Not Enough Memory for temp vNumberOfHeadsSort in Help Dialog shift down",cEMailInspx, 32000);
				#ifdef _DEBUG
				afxDump << "Not Enough Memory for temp color array in Help Dialog shift down Alloc";
				#endif
			}

			//bubblesort
			for (int i = 0; i < vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; i++)
			{
				
				if (i == 0)		//put first data point into sorted array
				{
					TempValveAverages[TempSortedArrayIndex] = vLocalSystemData->vValveStandardDeviations[i];
					vValveLabels[TempSortedArrayIndex] = 1;
					TempFilledToIndex++;
				}
				else
				{
					for (TempSortedArrayIndex = 0; TempSortedArrayIndex < TempFilledToIndex; TempSortedArrayIndex++)	//find where to put next data point in the sorted array
					{
						if (TempValveAverages[TempSortedArrayIndex] < vLocalSystemData->vValveStandardDeviations[i])	//if the new value to insert is larger than this point in the sorted array
						{
							for (int j = TempFilledToIndex; j > TempSortedArrayIndex; j--)	//for loop pushes all data one down the array
							{
								TempValveAverages[j] = TempValveAverages[j - 1];
								vValveLabels[j] = vValveLabels[j - 1];
							}
							TempValveAverages[TempSortedArrayIndex] = vLocalSystemData->vValveStandardDeviations[i];	//put new entry in array
							vValveLabels[TempSortedArrayIndex] = i + 1;

							TempFilledToIndex++;
							break;
						}
						else if (TempSortedArrayIndex == TempFilledToIndex - 1)		//smallest average fill, put in last slot.
						{
							TempValveAverages[TempFilledToIndex] = vLocalSystemData->vValveStandardDeviations[i];	//put new entry in array
							vValveLabels[TempFilledToIndex] = i + 1;
							
							TempFilledToIndex++;
							break;
						}
					}
				}
			}

			pBottomAxis->vVolumeSort = 1;
			pBottomAxis->vValveLabels = vValveLabels;
			pBottomAxis->vNumberOfValves = vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType];

			for (int i=0; i<vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; i++)
			{
				double xval;
				xval = i+1;
				pBarSeries1->AddPoint(xval , TempValveAverages[i]);
			}
		}
		else 
		if (vValveSortOrder == 3)		//(vDisplayMode == 2) - standard deviation / valve order 3 - ascending standard deviation
		{
			pBottomAxis->SetTickIncrement(false, 1.0);
			if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] > 16)
				pBottomAxis->SetMinMax(0, 16);

			int TempSortedArrayIndex = 0;
			int TempPlaceHolderValveNumber = 0;
			int TempPlaceHolderAverageFillLevel = 0;
			int TempFilledToIndex = 0;
			
			//create temporary arrays
			if (ThereIsEnoughMemory(sizeof(double) * vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType], "temp Color array in help dialog"))
				TempValveAverages = (double *)malloc(sizeof(double) * vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]);
			else
			{
				ReportErrorMessage("Not Enough Memory for temp vNumberOfHeadsSort in Help Dialog shift down",cEMailInspx, 32000);
				#ifdef _DEBUG
				afxDump << "Not Enough Memory for temp color array in Help Dialog shift down Alloc";
				#endif
			}

			//bubblesort
			for (int i = 0; i < vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; i++)
			{
				
				if (i == 0)		//put first data point into sorted array
				{
					TempValveAverages[TempSortedArrayIndex] = vLocalSystemData->vValveStandardDeviations[i];
					vValveLabels[TempSortedArrayIndex] = 1;
					TempFilledToIndex++;
				}
				else
				{
					for (TempSortedArrayIndex = 0; TempSortedArrayIndex < TempFilledToIndex; TempSortedArrayIndex++)	//find where to put next data point in the sorted array
					{
						if (vLocalSystemData->vValveStandardDeviations[i] < TempValveAverages[TempSortedArrayIndex])	//if the new value to insert is less than this point in the sorted array, then insert now
						{
							for (int j = TempFilledToIndex; j > TempSortedArrayIndex; j--)	//for loop pushes all data one down the array
							{
								TempValveAverages[j] = TempValveAverages[j - 1];
								vValveLabels[j] = vValveLabels[j - 1];
							}
							TempValveAverages[TempSortedArrayIndex] = vLocalSystemData->vValveStandardDeviations[i];	//put new entry in array
							vValveLabels[TempSortedArrayIndex] = i + 1;

							TempFilledToIndex++;
							break;
						}
						else if (TempSortedArrayIndex == TempFilledToIndex - 1)		//biggest average fill, put in last slot.
						{
							TempValveAverages[TempFilledToIndex] = vLocalSystemData->vValveStandardDeviations[i];	//put new entry in array
							vValveLabels[TempFilledToIndex] = i + 1;
							
							TempFilledToIndex++;
							break;
						}
					}
				}
			}

			pBottomAxis->vVolumeSort = 2;
			pBottomAxis->vValveLabels = vValveLabels;
			pBottomAxis->vNumberOfValves = vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType];

			for (int i=0; i<vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; i++)
			{
				double xval;
				xval = i+1;
				pBarSeries1->AddPoint(xval , TempValveAverages[i]);
			}
		}
		pBarSeries1->SetColor(RGB(0,0,200));
	}
	else 
	if (vDisplayMode == 3)  //display the fill value of one valve for all revoltions in memory
	{
		//m_ChartCtrl.EnableRefresh(false);
		m_ChartCtrl.RemoveAllSeries();
		CChartStandardAxis* pBottomAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
		pBottomAxis->SetAutomaticMode(CChartAxis::FullAutomatic);
		CChartStandardAxis* pLeftAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);

		//create labels
		CString TempUnits = "(gm)";
		if (vGlobalCurrentProduct->vFillerMonitorInspection)
			TempUnits = " ";
		else
		if (vLocalSystemData->vValveMonitorInspection <= vGlobalCurrentProduct->vNumberOfInspections)
		if (vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection])
			TempUnits = "(" + vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection]->vCheckWeighUnits + ")";

		std::wstring TempText = "Fill Level " + TempUnits + " for valve " + dtoa(vDisplayValveNumber, 0);
		pLeftAxis->GetLabel()->SetText(TempText);
		pBottomAxis->GetLabel()->SetText(_T("Time (filler rotations)"));
		m_ChartCtrl.SetBackColor(RGB(255,255,255));
		m_ChartCtrl.SetBorderColor(RGB(255,255,255));

		CChartLineSerie *pLineSerie1 = NULL;
		pLineSerie1 = m_ChartCtrl.CreateLineSerie();

		CChartLineSerie *pLineSerie2 = NULL;
		if (vLocalSystemData->vFillerHighThreshold > 0)
			pLineSerie2 = m_ChartCtrl.CreateLineSerie();

		CChartLineSerie *pLineSerie3 = NULL;
		if (vLocalSystemData->vFillerLowThreshold > 0)
			pLineSerie3 = m_ChartCtrl.CreateLineSerie();

		int ValveofInterest;
		ValveofInterest = 1;
		if (vDisplayValveNumber < vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
			ValveofInterest = vDisplayValveNumber;

		double tempMax = 0;
		double tempMin = 99999999;

		int tempIndexToUse = 0;//the index of the data point we are looking at
		if (vLocalSystemData->vNumberValveRowDataHave == cMaximumValveDataRows)
			tempIndexToUse = vLocalSystemData->vValveDataRowIndex;

		WORD TempGoodDataCount = 0;

		tempIndexToUse = tempIndexToUse  + (ValveofInterest - 1);
		for (int i=0; i < vLocalSystemData->vNumberValveRowDataHave; i++)
		{
			if (vLocalSystemData->vValveData[tempIndexToUse] > 0)
			{
				TempGoodDataCount++;
				if (vLocalSystemData->vValveData[tempIndexToUse] > tempMax)
					tempMax = vLocalSystemData->vValveData[tempIndexToUse];
				if (vLocalSystemData->vValveData[tempIndexToUse] < tempMin)
					tempMin = vLocalSystemData->vValveData[tempIndexToUse];
			
				pLineSerie1->AddPoint(i, vLocalSystemData->vValveData[tempIndexToUse]);

				if (vLocalSystemData->vFillerHighThreshold > 0)
					pLineSerie2->AddPoint(i, vLocalSystemData->vFillerHighThreshold);

				if (vLocalSystemData->vFillerLowThreshold > 0)
					pLineSerie3->AddPoint(i, vLocalSystemData->vFillerLowThreshold);
			}

			//increment to next data point
			tempIndexToUse = tempIndexToUse + vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType];
			//if we reach the end of data array, go back to beginning
			if (tempIndexToUse > (cMaximumValveDataRows * vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]))
				tempIndexToUse = ValveofInterest - 1;
		}

		if (tempMin < 0)
			tempMin = 0;

		if (tempMax <= tempMin)
			tempMax = tempMin + 1;

		pLeftAxis->SetMinMax(tempMin*0.95, tempMax*1.05);

		pLineSerie1->SetColor(RGB(0,255,0));

		if (vLocalSystemData->vFillerHighThreshold > 0)
		{
			pLineSerie2->SetColor(RGB(255,0,0));
			pLineSerie2->SetWidth(2);
			pLineSerie2->SetName(_T("High Threshold"));
		}
		if (vLocalSystemData->vFillerLowThreshold > 0)
		{
			pLineSerie3->SetColor(RGB(255,0,0));
			pLineSerie3->SetWidth(2);
			pLineSerie3->SetName(_T("Low Threshold"));
		}
		m_ChartCtrl.EnableRefresh(true);
	}
	else 
	if (vDisplayMode == 4) //show a histogram of a single valves data
	{
		//running data
		if (vLocalSystemData->vNumberValveRowDataHave) 
		{
			//m_ChartCtrl.EnableRefresh(false);
			m_ChartCtrl.RemoveAllSeries();
			CChartStandardAxis* pBottomAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);

			pBottomAxis->SetAutomatic(true);
			pBottomAxis->SetAutomaticMode(CChartAxis::FullAutomatic);
			CChartStandardAxis* pLeftAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
			pLeftAxis->SetAutomaticMode(CChartAxis::FullAutomatic);

					//create labels
			pLeftAxis->GetLabel()->SetText(_T("Count Numbers"));
			 std::wstring TempText = "Fill level values for valve " + dtoa(vDisplayValveNumber, 0);
			pBottomAxis->GetLabel()->SetText(TempText);
			m_ChartCtrl.SetBackColor(RGB(255,255,255));
			m_ChartCtrl.SetBorderColor(RGB(255,255,255));

			//graph points
			CChartBarSerie* pBarSeries1 = m_ChartCtrl.CreateBarSerie();

			//create array with size number of valves

			//create array with size number of valves
			//initialize histogram counts
			WORD tempNumberofBins = 1;
			double tempBinSize, tempMinValue, tempMaxValue, higherthanthresh, quartilefour, quartilethree, quartiletwo, quartileone, lowerthanthresh;
			double histinterval;
			histinterval = (vLocalSystemData->vFillerHighThreshold - vLocalSystemData->vFillerLowThreshold)/4;
			higherthanthresh = 0 ;
			quartilefour = 0; 
			quartilethree = 0;
			quartiletwo = 0; 
			quartileone = 0;
			lowerthanthresh = 0;
			tempMinValue = 99999999; //arbitrarily high number
			tempMaxValue = 0;
			tempBinSize = 0;
			int ValveofInterest = 1;
			if (vDisplayValveNumber < vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]) 
				ValveofInterest = vDisplayValveNumber;

			//1000 is cMaximumValveDataRows
			//find minimum and maximum...
			for (int i=1; i<vLocalSystemData->vNumberValveRowDataHave; i++)
			{ 
				double x; 
				int lookup;
				lookup = (ValveofInterest-1) + vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] * i;
				x = vLocalSystemData->vValveData[lookup];
				if (x > 0)
				{
					if (x < tempMinValue) 
						tempMinValue = x;
					if (x > tempMaxValue)
						tempMaxValue = x;
				}
			}
			//cast int to double
			//tempNumberofBins = floor(sqrt((double) vLocalSystemData->vNumberValveRowDataHave)); //number of bins

			double TempRange = tempMaxValue - tempMinValue;
			if (TempRange < 1)
				tempNumberofBins = 4;
			else
			{
				tempNumberofBins = (WORD)(TempRange + .99); //round up to get number of bins

				//while (tempNumberofBins < 10)
				//	tempNumberofBins = tempNumberofBins * 2;
			}


			tempBinSize = (tempMaxValue - tempMinValue) / tempNumberofBins;
			//if (tempBinSize < 1)
			//	tempBinSize = 1;
			tempNumberofBins = tempNumberofBins + 2;
			//double TempTickIncrement = ((tempMaxValue - tempBinSize) - (tempMinValue + tempBinSize)) / 10;
			//pBottomAxis->SetTickIncrement(false, tempBinSize * 10);
			//pBottomAxis->SetMinMax(tempMinValue - tempBinSize, tempMaxValue + (2* tempBinSize));
			pBottomAxis->EnableScrollBar(true);
			pBottomAxis->SetAutoHideScrollBar(true);
			//put stuff into bins

			//allocate memory to tempbininfo
			if (ThereIsEnoughMemory(tempNumberofBins * sizeof(DWORD), "tempBinInfo"))
				tempBinInfo = (DWORD *)malloc(tempNumberofBins * sizeof(DWORD));
		
			if (tempBinInfo)
			{
				//reset counts
				for (DWORD j=0; j < tempNumberofBins; j++)
					tempBinInfo[j] = 0;			

				for (int i=1; i<vLocalSystemData->vNumberValveRowDataHave; i++)
				{ 
					double x; 
					int lookup;
					lookup = (ValveofInterest - 1) + (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] * i);
					x = vLocalSystemData->vValveData[lookup];
					//initialize values of bins to 0

					if (x > 0)
					{
						WORD TempBinIndex = (WORD)((x - tempMinValue) / tempBinSize) + 1;
						tempBinInfo[TempBinIndex]++;
					}
				}
				for (WORD j=0; j < tempNumberofBins; j++)
				{
					//plot points
					double yval = tempBinInfo[j];
					pBarSeries1->AddPoint(tempMinValue + (j * tempBinSize) - tempBinSize, yval);	//the first number is the weight that bin represent, the second number is the number of containers in that weight	
				}
				pBarSeries1->SetColor(RGB(0,0,200));
				//free memory
				if (tempBinInfo) 
				{
					free(tempBinInfo); 
					tempBinInfo = NULL;
				}
			}
		}
		else 
		{
			//m_ChartCtrl.EnableRefresh(false);
			m_ChartCtrl.RemoveAllSeries();
			CChartStandardAxis* pBottomAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
			
			pBottomAxis->SetAutomaticMode(CChartAxis::FullAutomatic);
			CChartStandardAxis* pLeftAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
			pLeftAxis->SetAutomaticMode(CChartAxis::FullAutomatic);
			//create labels
			pLeftAxis->GetLabel()->SetText(_T("Counts"));
			pBottomAxis->GetLabel()->SetText(_T("Bin Values"));
			m_ChartCtrl.SetBackColor(RGB(255,255,255));
			m_ChartCtrl.SetBorderColor(RGB(255,255,255));

			m_ChartCtrl.EnableRefresh(true);
		}
	}
	else 
	if (vDisplayMode == 5)		//(vDisplayMode == 5) - eject counts
	{
		//m_ChartCtrl.EnableRefresh(false);
		m_ChartCtrl.RemoveAllSeries();

		CChartStandardAxis* pBottomAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
		CChartStandardAxis* pLeftAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
		pLeftAxis->SetAutomaticMode(CChartAxis::FullAutomatic);

		DWORD TempMaximumCount = 0;
		for (int i=0; i<vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; i++)
		{
			if (TempMaximumCount < vLocalSystemData->vFillerLowFillHeadCounts[ i ])
				TempMaximumCount = vLocalSystemData->vFillerLowFillHeadCounts[ i ];
		}
		DWORD TempDivision = TempMaximumCount / 10;

		if (TempMaximumCount > 5000)
			pLeftAxis->SetTickIncrement(false, 1000);
		else
		if (TempMaximumCount > 2500)
			pLeftAxis->SetTickIncrement(false, 500);
		else
		if (TempMaximumCount > 500)
			pLeftAxis->SetTickIncrement(false, 100);
		else
		if (TempMaximumCount > 250)
			pLeftAxis->SetTickIncrement(false, 50);
		else
		if (TempMaximumCount > 50)
			pLeftAxis->SetTickIncrement(false, 10);
		else
		if (TempMaximumCount > 25)
			pLeftAxis->SetTickIncrement(false, 5);
		else
			pLeftAxis->SetTickIncrement(false, 1);

		pBottomAxis->SetMinMax(0, vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] + 1);

		m_ChartCtrl.SetBackColor(RGB(255,255,255));
		m_ChartCtrl.SetBorderColor(RGB(255,255,255));

				//create labels
		pLeftAxis->GetLabel()->SetText(_T("Valve Under Fill Counts"));
		pBottomAxis->GetLabel()->SetText(_T("Valve Number"));
		pBottomAxis->EnableScrollBar(true);
		pBottomAxis->SetAutoHideScrollBar(true);

		CChartBarSerie* pBarSeries1 = m_ChartCtrl.CreateBarSerie();
		//pBarSeries1->SetColor(RGB(200,0,0));

		RECT TempRectCoor;
		m_ChartCtrl.GetWindowRect(&TempRectCoor);
		double TempWidthOfChart =TempRectCoor.right-TempRectCoor.left;
		WORD TempBarWidth = (WORD)(TempWidthOfChart / vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]);
		if (TempBarWidth > 1)
			TempBarWidth = TempBarWidth - 1;
		if (TempBarWidth > 1)
			TempBarWidth = TempBarWidth - 1;
		if (TempBarWidth < 1)
			TempBarWidth = 1;
		if (TempBarWidth > 20)
			TempBarWidth = 20;

		if (vValveSortOrder == 1)	//(vDisplayMode == 1) - fill monitor / valve order 1 - default by valve number
			pBarSeries1->SetBarWidth(TempBarWidth);
		else
			pBarSeries1->SetBarWidth(20);
		

		//graph points if data present
		if (vLocalSystemData->vNumberValveRowDataHave > 2) 
		if (vValveSortOrder == 1)			//(vDisplayMode == 5) - eject counts / valve order 1 - default by valve number
		{
			if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] < 40)
				pBottomAxis->SetTickIncrement(false, 1.0);
			else
				pBottomAxis->SetTickIncrement(false, 10);


			for (int i=0; i<vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; i++)
			{
				double xval;
				xval = i+1;
				if (xval > 0)
					pBarSeries1->AddPoint(xval , vLocalSystemData->vFillerLowFillHeadCounts[ i ]);
			}
		}
		else 
		if (vValveSortOrder == 2)		//(vDisplayMode == 5) - eject counts / valve order 2 - descending standard deviation
		{
			pBottomAxis->SetTickIncrement(false, 1.0);
			if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] > 16)
				pBottomAxis->SetMinMax(0, 16);

			int TempSortedArrayIndex = 0;
			int TempPlaceHolderValveNumber = 0;
			int TempPlaceHolderAverageFillLevel = 0;
			int TempFilledToIndex = 0;

			//create temporary arrays
			if (ThereIsEnoughMemory(sizeof(double) * vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType], "temp Color array in help dialog"))
			{
				TempValveAverages = (double *)malloc(sizeof(double) * vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]);
			}
			else
			{
				ReportErrorMessage("Not Enough Memory for temp vNumberOfHeadsSort in Help Dialog shift down",cEMailInspx, 32000);
				#ifdef _DEBUG
				afxDump << "Not Enough Memory for temp color array in Help Dialog shift down Alloc";
				#endif
			}
			if (ThereIsEnoughMemory(sizeof(int) * vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType], "temp Color array in help dialog"))
				vValveLabels = (int *)malloc(sizeof(int) * vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]);
			else
			{
				ReportErrorMessage("Not Enough Memory for temp vNumberOfHeadsSort in Help Dialog shift down",cEMailInspx, 32000);
				#ifdef _DEBUG
				afxDump << "Not Enough Memory for temp color array in Help Dialog shift down Alloc";
				#endif
			}

			//bubblesort
			if (TempValveAverages)
			for (int i = 0; i < vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; i++)
			{
				if (i == 0)		//put first data point into sorted array
				{
					TempValveAverages[TempSortedArrayIndex] =  vLocalSystemData->vFillerLowFillHeadCounts[i];
					vValveLabels[TempSortedArrayIndex] = 1;
					TempFilledToIndex++;
				}
				else
				{
					for (TempSortedArrayIndex = 0; TempSortedArrayIndex < TempFilledToIndex; TempSortedArrayIndex++)	//find where to put next data point in the sorted array
					{
						if (TempValveAverages[TempSortedArrayIndex] < vLocalSystemData->vFillerLowFillHeadCounts[i])	//if the new value to insert is larger than this point in the sorted array
						{
							for (int j = TempFilledToIndex; j > TempSortedArrayIndex; j--)	//for loop pushes all data one down the array
							{
								TempValveAverages[j] = TempValveAverages[j - 1];
								vValveLabels[j] = vValveLabels[j - 1];
							}
							TempValveAverages[TempSortedArrayIndex] = vLocalSystemData->vFillerLowFillHeadCounts[i];	//put new entry in array
							vValveLabels[TempSortedArrayIndex] = i + 1;

							TempFilledToIndex++;
							break;
						}
						else if (TempSortedArrayIndex == TempFilledToIndex - 1)		//smallest average fill, put in last slot.
						{
							TempValveAverages[TempFilledToIndex] = vLocalSystemData->vFillerLowFillHeadCounts[i];	//put new entry in array
							vValveLabels[TempFilledToIndex] = i + 1;
							
							TempFilledToIndex++;
							break;
						}
					}
				}
			}

			pBottomAxis->vVolumeSort = 1;
			pBottomAxis->vValveLabels = vValveLabels;
			pBottomAxis->vNumberOfValves = vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType];


			for (int i=0; i<vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; i++)
			{
				double xval;
				xval = i+1;
				if (xval > 0)
					pBarSeries1->AddPoint(xval , TempValveAverages[i]);
			}
		}
		else if (vValveSortOrder == 3)		//(vDisplayMode == 2) - standard deviation / valve order 3 - ascending standard deviation
		{
			pBottomAxis->SetTickIncrement(false, 1.0);
			if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] > 16)
				pBottomAxis->SetMinMax(0, 16);

			int TempSortedArrayIndex = 0;
			int TempPlaceHolderValveNumber = 0;
			int TempPlaceHolderAverageFillLevel = 0;
			int TempFilledToIndex = 0;
			
			//create temporary arrays
			if (ThereIsEnoughMemory(sizeof(double) * vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType], "temp Color array in help dialog"))
				TempValveAverages = (double *)malloc(sizeof(double) * vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]);
			else
			{
				ReportErrorMessage("Not Enough Memory for temp vNumberOfHeadsSort in Help Dialog shift down",cEMailInspx, 32000);
				#ifdef _DEBUG
				afxDump << "Not Enough Memory for temp color array in Help Dialog shift down Alloc";
				#endif
			}

			//bubblesort
			for (int i = 0; i < vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; i++)
			{
				
				if (i == 0)		//put first data point into sorted array
				{
					TempValveAverages[TempSortedArrayIndex] = vLocalSystemData->vFillerLowFillHeadCounts[i];
					vValveLabels[TempSortedArrayIndex] = 1;
					TempFilledToIndex++;
				}
				else
				{
					for (TempSortedArrayIndex = 0; TempSortedArrayIndex < TempFilledToIndex; TempSortedArrayIndex++)	//find where to put next data point in the sorted array
					{
						if (vLocalSystemData->vFillerLowFillHeadCounts[i] < TempValveAverages[TempSortedArrayIndex])	//if the new value to insert is less than this point in the sorted array, then insert now
						{
							for (int j = TempFilledToIndex; j > TempSortedArrayIndex; j--)	//for loop pushes all data one down the array
							{
								TempValveAverages[j] = TempValveAverages[j - 1];
								vValveLabels[j] = vValveLabels[j - 1];
							}
							TempValveAverages[TempSortedArrayIndex] = vLocalSystemData->vFillerLowFillHeadCounts[i];	//put new entry in array
							vValveLabels[TempSortedArrayIndex] = i + 1;

							TempFilledToIndex++;
							break;
						}
						else if (TempSortedArrayIndex == TempFilledToIndex - 1)		//biggest average fill, put in last slot.
						{
							TempValveAverages[TempFilledToIndex] = vLocalSystemData->vFillerLowFillHeadCounts[i];	//put new entry in array
							vValveLabels[TempFilledToIndex] = i + 1;
							
							TempFilledToIndex++;
							break;
						}
					}
				}
			}

			pBottomAxis->vVolumeSort = 2;
			pBottomAxis->vValveLabels = vValveLabels;
			pBottomAxis->vNumberOfValves = vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType];


			for (int i=0; i<vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; i++)
			{
				double xval;
				xval = i+1;
				if (xval > 0)
					pBarSeries1->AddPoint(xval , TempValveAverages[i]);
			}
		}
		pBarSeries1->SetColor(RGB(200,0,0));
	}
	m_ChartCtrl.EnableRefresh(true);
	//m_ChartCtrl.Invalidate();
	//vRefreshSoon = true;
	int TempTimerResult = SetTimer(vRefreshDisplayTimerHandle,50,NULL);
	if (!TempTimerResult)
		ReportErrorMessage("Error-Update Display Timer Failed in FillerValveDialog",cEMailInspx,32000);

	if (TempValveAverages)
		free(TempValveAverages);
}


void CFillerValveDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	
	vLocalNumberOfHeads = vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType];

//set time and date
	vTime =CTime::GetCurrentTime();
	CString TempTimeString = " ";
	if (vLocalConfigurationData->vShow24HourTime)
		TempTimeString = vTime.Format("%H:%M:%S");//24 hour time format
	else
		TempTimeString = vTime.Format("%#I:%M:%S%p");//12 hour time format with am pm

	SetDlgItemText(IDC_Time,TempTimeString);

	CString TempDateString = vTime.Format("%B %#d, %Y");
	CString TempChar = TempDateString.GetAt(0);
	TempChar.MakeUpper();
	TempDateString.SetAt(0,TempChar.GetAt(0));
		
	SetDlgItemText(IDC_Date,TempDateString);

	vDateToday= vTime.GetDay();

//set time and date timer
	int TempTimerResult = SetTimer(vUpdateDisplayTimerHandle,1000,NULL);
	if (!TempTimerResult)
		ReportErrorMessage("Error-Update Display Timer Failed in FillerValveDialog",cEMailInspx,32000);

//set labels on screen
	SetDlgItemText(IDC_ProductName,vLocalSystemData->vCurrentProductName);
	SetDlgItemText(IDC_FillerValveNumber, dtoa(vLocalNumberOfHeads,2));
	SetDlgItemText(IDC_InspectedCount, dtoa(vLocalSystemData->vTotalContainers, 0));
	SetDlgItemText(IDC_PercentRejected, dtoa(vLocalSystemData->vPercentRejected, 4));
	SetDlgItemText(IDC_OverfillCount, dtoa(vLocalSystemData->vTotalContainers, 0));

	if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
	if (vOldFillerAverage != vLocalSystemData->vFillerAverageWeight)
	{
		vOldFillerAverage = vLocalSystemData->vFillerAverageWeight;
		if (vLocalSystemData->vNumberValveRowDataHave <= 4)
			SetDlgItemText(IDC_AverageFill, _T("--"));
		else
			SetDlgItemText(IDC_AverageFill, dtoa(vOldFillerAverage, 2));
	}
	
	double tempAverageStdDev;
	tempAverageStdDev = 0;
	if (vLocalSystemData->vValveStandardDeviations)
	if (vLocalSystemData->vNumberValveRowDataHave)
	{
		for (int i=0; i<vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; i++)
			tempAverageStdDev += vLocalSystemData->vValveStandardDeviations[ i ];

		tempAverageStdDev = tempAverageStdDev / vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType];
		if (tempAverageStdDev < 0)
			tempAverageStdDev = 0;
		SetDlgItemText(IDC_StandardDeviation, dtoa(tempAverageStdDev, 2));
	}
	
	vOldNumberofDataRows = vLocalSystemData->vNumberValveRowDataHave;
	int TempNumOfSamples = vOldNumberofDataRows;
	TempNumOfSamples = TempNumOfSamples * vLocalNumberOfHeads;
	SetDlgItemText(IDC_NumberOfSamples,dtoa(TempNumOfSamples));

	//remove border around graph/plot/chart
	m_ChartCtrl.SetEdgeType(NULL);


	vUnderfillCount = 0;
	vOverfillCount = 0;
	vForeignObjectCount = 0;
	bool TempHaveOverFill = false;
	//set system scanning statistics
	for (int TempInspectionNumber = 0; TempInspectionNumber < vGlobalCurrentProduct->vNumberOfInspections; TempInspectionNumber++)
	{
		//if (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cDensityInspection);
		//else 
		//if	(vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cCheckWeighInspection);
		//else 
		//if (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cMissingLidInspection)
		//else 
		if ((vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cContaminantInspection)
				|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cIPContaminantInspection)
				|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cKernelInspection)
				|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cJarBottomContaminantInspection)
				|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cIPContaminantInspection)
				|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cThresholdContaminantInspection)
				|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cTopContaminantInspection)
				|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cBottomContaminantInspection)
				|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cContaminant2Inspection)
				|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cAdvancedContaminant2Inspection)
				|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cGlass13mmContaminantInspection)
				|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cSpotInROIContaminantInspection)
				|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cContaminantBonesInspection)
				|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cContaminant22Inspection)
				|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cWireContaminantInspection))
			vForeignObjectCount = vForeignObjectCount + vLocalSystemData->vXRayCriteriaCount[TempInspectionNumber] + vLocalSystemData->vXRayCriteriaCountButDisabled[TempInspectionNumber];
		else 
		if	((vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cUnderfillByWeightInspection)
				|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cUnderfillInspection))
			vUnderfillCount = vUnderfillCount + vLocalSystemData->vXRayCriteriaCount[TempInspectionNumber] + vLocalSystemData->vXRayCriteriaCountButDisabled[TempInspectionNumber];
		else 
		if ((vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cOverfillByWeightInspection)
				|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cOverfillInspection))
		{
			vOverfillCount = vOverfillCount + vLocalSystemData->vXRayCriteriaCount[TempInspectionNumber] + vLocalSystemData->vXRayCriteriaCountButDisabled[TempInspectionNumber];
			TempHaveOverFill = true;
		}
	}
	if (TempHaveOverFill)
		SetDlgItemText(IDC_OverfillCount, dtoa(vOverfillCount));
	else
	{
		m_OverfillCount1.ShowWindow(SW_HIDE);
		m_OverfillLabel.ShowWindow(SW_HIDE);
	}
	SetDlgItemText(IDC_UnderfillCount, dtoa(vUnderfillCount));
	SetDlgItemText(IDC_ForiegnObjectCount, dtoa(vForeignObjectCount));
	vOldUnderfillCount = vUnderfillCount;
	vOldOverfillCount = vOverfillCount;
	vOldForeignObjectCount = vForeignObjectCount;

//select Fill Monitor to show all valves to start
	m_ShadedButton1.ShowWindow(SW_HIDE);
	m_ShadedButton1Selected.ShowWindow(SW_SHOW);
	m_ShadedButton2Selected.ShowWindow(SW_HIDE);
	m_ShadedButton3Selected.ShowWindow(SW_HIDE);
	m_ShadedButton4Selected.ShowWindow(SW_HIDE);
	m_ShadedButton7Selected.ShowWindow(SW_HIDE);

//set white background and border position
	m_MainBackground2.SetWindowPos(NULL,16,
	90,608,380,SWP_NOZORDER);
//	m_Border19.SetWindowPos(NULL,612,100,12,60,SWP_NOZORDER);
//	m_Border20.SetWindowPos(NULL,612,160,12,60,SWP_NOZORDER);
//	m_Border21.SetWindowPos(NULL,612,220,12,60,SWP_NOZORDER);
//	m_Border22.SetWindowPos(NULL,612,270,12,60,SWP_NOZORDER);
//	m_Border23.SetWindowPos(NULL,612,345,12,60,SWP_NOZORDER);
	//m_Border25.SetWindowPos(NULL,611,405,13,60,SWP_NOZORDER);
	m_Border25.SetWindowPos(NULL,611,100,13,362,SWP_NOZORDER);
//	m_Border30.SetWindowPos(NULL,15,475,595,13,SWP_NOZORDER);
	m_Border30.SetWindowPos(NULL,26,460,588,13,SWP_NOZORDER);
	
	m_Border46.SetWindowPos(NULL,597,170,13,120,SWP_NOZORDER);
	m_Border52.SetWindowPos(NULL,425,287,171,13,SWP_NOZORDER);

	m_Border45.SetWindowPos(NULL,597,335,13,121,SWP_NOZORDER);
	m_Border54.SetWindowPos(NULL,425,453,171,13,SWP_NOZORDER);

	m_Border64.SetWindowPos(NULL,388,400,13,65,SWP_NOZORDER);
	m_Border63.SetWindowPos(NULL,174,453,217,13,SWP_NOZORDER);

	if ((vLocalSystemData->vGlobalMonitorSizeX == 1024) && (vLocalSystemData->vGlobalMonitorSizeY == 768))
	{
		m_Border30.SetWindowPos(NULL,26,461,588,13,SWP_NOZORDER);
	//	m_Border25.SetWindowPos(NULL,611,99,13,366,SWP_NOZORDER);	//outer border vertical

		m_Border63.SetWindowPos(NULL,173,454,218,13,SWP_NOZORDER);	//status box horizontal
		m_Border64.SetWindowPos(NULL,388,400,13,65,SWP_NOZORDER);	//status box vertical

		m_Border52.SetWindowPos(NULL,425,287,172,13,SWP_NOZORDER);	//filler statistics box vertical
		m_Border54.SetWindowPos(NULL,425,454,172,13,SWP_NOZORDER);
	}
	else if ((vLocalSystemData->vGlobalMonitorSizeX > 1024) && (vLocalSystemData->vGlobalMonitorSizeY > 768))
	{
		m_Border25.SetWindowPos(NULL,611,99,13,366,SWP_NOZORDER);	//outer border vertical
		
		m_Border63.SetWindowPos(NULL,173,456,218,13,SWP_NOZORDER);	//status box horizontal
		m_Border64.SetWindowPos(NULL,386,400,13,65,SWP_NOZORDER);	//status box vertical
	//	m_Border46.SetWindowPos(NULL,596,170,13,121,SWP_NOZORDER);
		m_Border52.SetWindowPos(NULL,425,287,172,13,SWP_NOZORDER);	//filler statistics box vertical
		
	//	m_Border45.SetWindowPos(NULL,596,335,13,121,SWP_NOZORDER);
		m_Border54.SetWindowPos(NULL,425,456,172,13,SWP_NOZORDER);
	}
		/*
		m_MainBackground2.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

		cMainBackgroundLeft = 106;
		const BYTE cMainBackgroundTop = 41;
		const WORD cMainBackgroundLength = 423;
		const WORD cMainBackgroundHeight = 367;
		*/

	SetupMenu(vLocalCWndCollection);

	if (ResizeXCoor(1) >= 2)
	{
		//start replace buttons
		vGraphicBitmap.DeleteObject();	//main menu
		vGraphicBitmap.LoadBitmap(IDB_CorneredButtonBlue4xBiggerMainMenu1);
		m_MainMenu.SetBitmap(vGraphicBitmap);

		vGraphicBitmap1.DeleteObject();
		vGraphicBitmap1.LoadBitmap(IDB_CorneredButtonBlue4xBiggerBack);	//back
		m_BackButton.SetBitmap(vGraphicBitmap1);

		RECT TempMainMenuRect, TempBackRect;
		m_MainMenu.GetWindowRect(&TempMainMenuRect);
		m_BackButton.GetWindowRect(&TempBackRect);

		m_MainMenu.SetWindowPos(NULL,TempMainMenuRect.left - 30, TempMainMenuRect.top, TempMainMenuRect.right - TempMainMenuRect.left,
								TempMainMenuRect.bottom - TempMainMenuRect.top, SWP_NOZORDER);
		m_BackButton.SetWindowPos(NULL,TempBackRect.left - 30, TempBackRect.top + 4, TempBackRect.right - TempBackRect.left,
								TempBackRect.bottom - TempBackRect.top, SWP_NOZORDER);

		vGraphicBitmap2.DeleteObject();
		vGraphicBitmap2.LoadBitmap(IDB_CorneredButtonGrey4xBiggerFunction1Select);	//fill monitor button
		m_ShadedButton1Selected.SetBitmap(vGraphicBitmap2);

		vGraphicBitmap3.DeleteObject();
		vGraphicBitmap3.LoadBitmap(IDB_CorneredButtonGrey4xBiggerFunction2Select);	//standard deviation button
		m_ShadedButton2Selected.SetBitmap(vGraphicBitmap3);

		vGraphicBitmap4.DeleteObject();
		vGraphicBitmap4.LoadBitmap(IDB_CorneredButtonGrey4xBiggerFunction3Select);	//valve history button
		m_ShadedButton3Selected.SetBitmap(vGraphicBitmap4);

		vGraphicBitmap5.DeleteObject();
		vGraphicBitmap5.LoadBitmap(IDB_CorneredButtonGrey4xBiggerFunction4Select);	//valve distribution button
		m_ShadedButton4Selected.SetBitmap(vGraphicBitmap5);

		vGraphicBitmap6.DeleteObject();
		vGraphicBitmap6.LoadBitmap(IDB_CorneredButtonGrey4xBiggerFunction1);
		m_ShadedButton1.SetBitmap(vGraphicBitmap6);

		vGraphicBitmap7.DeleteObject();
		vGraphicBitmap7.LoadBitmap(IDB_CorneredButtonGrey4xBiggerFunction2);
		m_ShadedButton2.SetBitmap(vGraphicBitmap7);
		
		vGraphicBitmap8.DeleteObject();
		vGraphicBitmap8.LoadBitmap(IDB_CorneredButtonGrey4xBiggerFunction3);
		m_ShadedButton3.SetBitmap(vGraphicBitmap8);
		
		vGraphicBitmap9.DeleteObject();
		vGraphicBitmap9.LoadBitmap(IDB_CorneredButtonGrey4xBiggerFunction4);
		m_ShadedButton4.SetBitmap(vGraphicBitmap9);

		vGraphicBitmap10.DeleteObject();
		vGraphicBitmap10.LoadBitmap(IDB_CorneredButtonGrey4xBiggerFunction5);	//select filler valve button
		m_ShadedButton5.SetBitmap(vGraphicBitmap10);

		vGraphicBitmap48.DeleteObject();
		vGraphicBitmap48.LoadBitmap(IDB_CorneredButtonGrey4xBiggerFunction6);	//select filler valve button
		m_ShadedButton6.SetBitmap(vGraphicBitmap48);
		m_ShadedButton6.ShowWindow(SW_SHOW);

		RECT TempRectRight;
		m_ShadedButton6.GetWindowRect(&TempRectRight);
		m_ShadedButton6.SetWindowPos(NULL, TempRectRight.left - 50, 
								TempRectRight.top - 3,
								TempRectRight.right - TempRectRight.left,
								TempRectRight.bottom - TempRectRight.top, SWP_NOZORDER);


		m_ShadedButton6.ShowWindow(SW_SHOW);
		//end replace buttons

		//start outer border
		RECT TempRectLowerLeft, TempRectUpperLeft, TempRectLowerRight, TempRectUpperRight, 
				TempRectBottom, TempRectLeft, TempRectTop;

		vGraphicBitmap11.DeleteObject();
		vGraphicBitmap11.LoadBitmap(IDB_BorderCornerOuterLowerLeft4xBigger);
		m_Border11.SetBitmap(vGraphicBitmap11);

		vGraphicBitmap12.DeleteObject();
		vGraphicBitmap12.LoadBitmap(IDB_BorderCornerOuterLowerRight4xBigger);
		m_Border10.SetBitmap(vGraphicBitmap12);

		vGraphicBitmap13.DeleteObject();
		vGraphicBitmap13.LoadBitmap(IDB_BorderCornerOuterUpperLeft4xBigger);
		m_Border2.SetBitmap(vGraphicBitmap13);

		vGraphicBitmap14.DeleteObject();
		vGraphicBitmap14.LoadBitmap(IDB_BorderCornerOuterUpperRight4xBigger);
		m_Border8.SetBitmap(vGraphicBitmap14);

		vGraphicBitmap36.DeleteObject();		//bottom
		vGraphicBitmap36.LoadBitmap(IDB_BorderBottom4xBigger);
		m_Border30.SetBitmap(vGraphicBitmap36);

		vGraphicBitmap37.DeleteObject();	//left
		vGraphicBitmap37.LoadBitmap(IDB_BorderLeft4xBigger);
		m_Border17.SetBitmap(vGraphicBitmap37);

		vGraphicBitmap38.DeleteObject();		//right
		vGraphicBitmap38.LoadBitmap(IDB_BorderRight4xBigger);
		m_Border25.SetBitmap(vGraphicBitmap38);

		vGraphicBitmap39.DeleteObject();	//top
		vGraphicBitmap39.LoadBitmap(IDB_BorderTop4xBigger);
		m_Border6.SetBitmap(vGraphicBitmap39);

		m_Border11.GetWindowRect(&TempRectLowerLeft);
		m_Border2.GetWindowRect(&TempRectUpperLeft);
		m_Border10.GetWindowRect(&TempRectLowerRight);
		m_Border8.GetWindowRect(&TempRectUpperRight);

	//	m_Border35.ShowWindow(SW_HIDE);
		m_Border.ShowWindow(SW_HIDE);
		m_Border3.ShowWindow(SW_HIDE);
		m_Border4.ShowWindow(SW_HIDE);
		m_Border5.ShowWindow(SW_HIDE);
		m_Border7.ShowWindow(SW_HIDE);
		m_Border9.ShowWindow(SW_HIDE);
		m_Border16.ShowWindow(SW_HIDE);
		m_Border14.ShowWindow(SW_HIDE);
		m_Border15.ShowWindow(SW_HIDE);
		m_Border13.ShowWindow(SW_HIDE);
		m_Border12.ShowWindow(SW_HIDE);
		m_Border18.ShowWindow(SW_HIDE);
		m_Border20.ShowWindow(SW_HIDE);
		m_Border21.ShowWindow(SW_HIDE);
		m_Border22a.ShowWindow(SW_HIDE);
		m_Border23.ShowWindow(SW_HIDE);
		m_Border19.ShowWindow(SW_HIDE);
		m_Border31.ShowWindow(SW_HIDE);
		m_Border29.ShowWindow(SW_HIDE);
		m_Border32.ShowWindow(SW_HIDE);
		m_Border26.ShowWindow(SW_HIDE);
		m_Border28.ShowWindow(SW_HIDE);
		m_Border27.ShowWindow(SW_HIDE);

			//lower left corner
		m_Border11.SetWindowPos(NULL, TempRectLowerLeft.left - 30, TempRectLowerLeft.top - 15,
								TempRectLowerLeft.right - TempRectLowerLeft.left,
								TempRectLowerLeft.bottom - TempRectLowerLeft.top, SWP_NOZORDER);
			//upper left corner
		m_Border2.SetWindowPos(NULL, TempRectUpperLeft.left - 30, TempRectUpperLeft.top - 15,
								TempRectUpperLeft.right - TempRectUpperLeft.left,
								TempRectUpperLeft.bottom - TempRectUpperLeft.top, SWP_NOZORDER);
			//lower right corner
		m_Border10.SetWindowPos(NULL, TempRectLowerRight.left - 5, TempRectLowerLeft.top - 15,
								TempRectLowerRight.right - TempRectLowerRight.left,
								TempRectLowerRight.bottom - TempRectLowerRight.top, SWP_NOZORDER);
			//upper right corner
		m_Border8.SetWindowPos(NULL, TempRectUpperRight.left - 5, TempRectUpperRight.top - 15,
								TempRectUpperRight.right - TempRectUpperRight.left,
								TempRectUpperRight.bottom - TempRectUpperRight.top, SWP_NOZORDER);
		
		m_Border11.GetWindowRect(&TempRectLowerLeft);
		m_Border2.GetWindowRect(&TempRectUpperLeft);
		m_Border10.GetWindowRect(&TempRectLowerRight);
		m_Border8.GetWindowRect(&TempRectUpperRight);

		m_Border30.GetWindowRect(&TempRectBottom);
		m_Border25.GetWindowRect(&TempRectRight);
		m_Border17.GetWindowRect(&TempRectLeft);
		m_Border6.GetWindowRect(&TempRectTop);

					//lower left corner
		m_Border11.SetWindowPos(NULL, TempRectLowerLeft.left - 4, TempRectLowerLeft.top - 8,
								TempRectLowerLeft.right - TempRectLowerLeft.left,
								TempRectLowerLeft.bottom - TempRectLowerLeft.top, SWP_NOZORDER);
			//upper left corner
		m_Border2.SetWindowPos(NULL, TempRectUpperLeft.left - 4, TempRectUpperLeft.top,
								TempRectUpperLeft.right - TempRectUpperLeft.left,
								TempRectUpperLeft.bottom - TempRectUpperLeft.top, SWP_NOZORDER);
			//lower right corner
		m_Border10.SetWindowPos(NULL, TempRectLowerRight.left - 5, TempRectLowerLeft.top - 8,
								TempRectLowerRight.right - TempRectLowerRight.left,
								TempRectLowerRight.bottom - TempRectLowerRight.top, SWP_NOZORDER);
			//upper right corner
		m_Border8.SetWindowPos(NULL, TempRectUpperRight.left - 5, TempRectUpperRight.top,
								TempRectUpperRight.right - TempRectUpperRight.left,
								TempRectUpperRight.bottom - TempRectUpperRight.top, SWP_NOZORDER);

		//bottom
		m_Border30.SetWindowPos(NULL, TempRectLowerLeft.right - 4,
								TempRectLowerLeft.bottom - 8 - (TempRectBottom.bottom - TempRectBottom.top),
								TempRectLowerRight.left - TempRectLowerLeft.right,
								TempRectBottom.bottom - TempRectBottom.top, SWP_NOZORDER);

		//left
		m_Border17.SetWindowPos(NULL, TempRectUpperLeft.left - 4, 
								TempRectUpperLeft.bottom,
								TempRectLeft.right - TempRectLeft.left,
								TempRectLowerLeft.top - TempRectUpperLeft.bottom, SWP_NOZORDER);

		//right
		m_Border25.SetWindowPos(NULL, TempRectUpperRight.right - (TempRectRight.right - TempRectRight.left) - 5, 
								TempRectUpperRight.bottom,
								TempRectRight.right - TempRectRight.left,
								TempRectLowerLeft.top - TempRectUpperLeft.bottom, SWP_NOZORDER);

		//top
		m_Border6.SetWindowPos(NULL, TempRectUpperLeft.right - 4, 
								TempRectUpperLeft.top,
								TempRectLowerRight.left - TempRectLowerLeft.right,
								TempRectBottom.bottom - TempRectBottom.top, SWP_NOZORDER);
		//end outer border

		//set background to meet outer border
		m_MainBackground2.SetWindowPos(NULL,TempRectUpperLeft.left,TempRectUpperLeft.top,
											TempRectUpperRight.right - TempRectUpperLeft.left - 9,
											TempRectLowerLeft.bottom - TempRectUpperLeft.top - 8,
											SWP_NOZORDER);

		//start border status box
		vGraphicBitmap15.DeleteObject();
		vGraphicBitmap15.LoadBitmap(IDB_BorderCornerInnerLowerLeft4xBigger);
		m_Border62.SetBitmap(vGraphicBitmap15);

		vGraphicBitmap16.DeleteObject();
		vGraphicBitmap16.LoadBitmap(IDB_BorderCornerInnerLowerRight4xBigger);
		m_Border65.SetBitmap(vGraphicBitmap16);

		vGraphicBitmap17.DeleteObject();
		vGraphicBitmap17.LoadBitmap(IDB_BorderCornerInnerUpperLeft4xBigger);
		m_Border56.SetBitmap(vGraphicBitmap17);

		vGraphicBitmap18.DeleteObject();
		vGraphicBitmap18.LoadBitmap(IDB_BorderCornerInnerUpperRight4xBigger);
		m_Border59.SetBitmap(vGraphicBitmap18);

		vGraphicBitmap44.DeleteObject();		//bottom
		vGraphicBitmap44.LoadBitmap(IDB_BorderBottom4xBigger);
		m_Border63.SetBitmap(vGraphicBitmap44);

		vGraphicBitmap45.DeleteObject();	//left
		vGraphicBitmap45.LoadBitmap(IDB_BorderLeft4xBigger);
		m_Border61.SetBitmap(vGraphicBitmap45);

		vGraphicBitmap46.DeleteObject();		//right
		vGraphicBitmap46.LoadBitmap(IDB_BorderRight4xBigger);
		m_Border64.SetBitmap(vGraphicBitmap46);

		vGraphicBitmap47.DeleteObject();	//top
		vGraphicBitmap47.LoadBitmap(IDB_BorderTop4xBigger);
		m_Border57.SetBitmap(vGraphicBitmap47);

		
		m_Border62.GetWindowRect(&TempRectLowerLeft);
		m_Border56.GetWindowRect(&TempRectUpperLeft);
		m_Border65.GetWindowRect(&TempRectLowerRight);
		m_Border59.GetWindowRect(&TempRectUpperRight);
		m_Border63.GetWindowRect(&TempRectBottom);
		m_Border61.GetWindowRect(&TempRectLeft);
		m_Border64.GetWindowRect(&TempRectRight);
		m_Border57.GetWindowRect(&TempRectTop);

		m_Border58.ShowWindow(SW_HIDE);
		m_Border67.ShowWindow(SW_HIDE);
		m_Border60.ShowWindow(SW_HIDE);


			//lower left corner
		m_Border62.SetWindowPos(NULL, TempRectLowerLeft.left - 30, TempRectLowerLeft.top - 20,
								TempRectLowerLeft.right - TempRectLowerLeft.left,
								TempRectLowerLeft.bottom - TempRectLowerLeft.top, SWP_NOZORDER);
			//upper left corner
		m_Border56.SetWindowPos(NULL, TempRectUpperLeft.left - 30, TempRectUpperLeft.top,
								TempRectUpperLeft.right - TempRectUpperLeft.left,
								TempRectUpperLeft.bottom - TempRectUpperLeft.top, SWP_NOZORDER);
			//lower right corner
		m_Border65.SetWindowPos(NULL, TempRectLowerRight.left - 15, TempRectLowerLeft.top - 20,
								TempRectLowerRight.right - TempRectLowerRight.left,
								TempRectLowerRight.bottom - TempRectLowerRight.top, SWP_NOZORDER);
			//upper right corner
		m_Border59.SetWindowPos(NULL, TempRectUpperRight.left - 15, TempRectUpperRight.top,
								TempRectUpperRight.right - TempRectUpperRight.left,
								TempRectUpperRight.bottom - TempRectUpperRight.top, SWP_NOZORDER);

		//bottom
		m_Border63.SetWindowPos(NULL, TempRectLowerLeft.right - 30,
								(TempRectLowerLeft.bottom - 20) - (TempRectBottom.bottom - TempRectBottom.top),
								TempRectLowerRight.left - (TempRectLowerLeft.right - 30),
								TempRectBottom.bottom - TempRectBottom.top, SWP_NOZORDER);

		//left
		m_Border61.SetWindowPos(NULL, TempRectUpperLeft.left - 30, 
								TempRectUpperLeft.bottom,
								TempRectLeft.right - TempRectLeft.left,
								(TempRectLowerLeft.top - 20) - TempRectUpperLeft.bottom, SWP_NOZORDER);

		//right
		m_Border64.SetWindowPos(NULL, TempRectUpperRight.right - 15 - (TempRectRight.right - TempRectRight.left), 
								TempRectUpperRight.bottom,
								TempRectLeft.right - TempRectLeft.left,
								(TempRectLowerLeft.top - 20) - TempRectUpperLeft.bottom, SWP_NOZORDER);

		//top
		m_Border57.SetWindowPos(NULL, TempRectUpperLeft.right - 30, 
								TempRectUpperLeft.top,
								TempRectLowerRight.left - (TempRectLowerLeft.right - 30),
								TempRectBottom.bottom - TempRectBottom.top, SWP_NOZORDER);

		RECT TempStatusRect;
		m_StatusLine1.GetWindowRect(&TempStatusRect);
		m_StatusLine1.SetWindowPos(NULL, TempRectUpperLeft.left - 30 + (TempRectLeft.right - TempRectLeft.left), TempStatusRect.top, 
				(TempRectUpperRight.right - 15 - (TempRectRight.right - TempRectRight.left)) - (TempRectUpperLeft.left - 30 + (TempRectLeft.right - TempRectLeft.left)),
				TempStatusRect.bottom - TempStatusRect.top, SWP_NOZORDER);
										
		//end status box

		//start inspection statistics box
		vGraphicBitmap19.DeleteObject();
		vGraphicBitmap19.LoadBitmap(IDB_BorderCornerInnerLowerLeft4xBigger);
		m_Border41.SetBitmap(vGraphicBitmap19);

		vGraphicBitmap20.DeleteObject();
		vGraphicBitmap20.LoadBitmap(IDB_BorderCornerInnerLowerRight4xBigger);
		m_Border39.SetBitmap(vGraphicBitmap20);

		vGraphicBitmap21.DeleteObject();
		vGraphicBitmap21.LoadBitmap(IDB_BorderCornerInnerUpperLeft4xBigger);
		m_Border42.SetBitmap(vGraphicBitmap21);

		vGraphicBitmap22.DeleteObject();
		vGraphicBitmap22.LoadBitmap(IDB_BorderCornerInnerUpperRight4xBigger);
		m_Border43.SetBitmap(vGraphicBitmap22);

		vGraphicBitmap40.DeleteObject();		//bottom
		vGraphicBitmap40.LoadBitmap(IDB_BorderBottom4xBigger);
		m_Border54.SetBitmap(vGraphicBitmap40);

		vGraphicBitmap41.DeleteObject();	//left
		vGraphicBitmap41.LoadBitmap(IDB_BorderLeft4xBigger);
		m_Border50.SetBitmap(vGraphicBitmap41);

		vGraphicBitmap42.DeleteObject();		//right
		vGraphicBitmap42.LoadBitmap(IDB_BorderRight4xBigger);
		m_Border45.SetBitmap(vGraphicBitmap42);

		vGraphicBitmap43.DeleteObject();	//top
		vGraphicBitmap43.LoadBitmap(IDB_BorderTop4xBigger);
		m_Border48.SetBitmap(vGraphicBitmap43);

		
		m_Border41.GetWindowRect(&TempRectLowerLeft);
		m_Border42.GetWindowRect(&TempRectUpperLeft);
		m_Border39.GetWindowRect(&TempRectLowerRight);
		m_Border43.GetWindowRect(&TempRectUpperRight);
		m_Border54.GetWindowRect(&TempRectBottom);
		m_Border50.GetWindowRect(&TempRectLeft);
		m_Border45.GetWindowRect(&TempRectRight);
		m_Border48.GetWindowRect(&TempRectTop);

		m_Border47.ShowWindow(SW_HIDE);
		m_Border49.ShowWindow(SW_HIDE);
		m_Border44.ShowWindow(SW_HIDE);
		m_Border55.ShowWindow(SW_HIDE);


			//lower left corner
		m_Border41.SetWindowPos(NULL, TempRectLowerLeft.left, TempRectLowerLeft.top - 20,
								TempRectLowerLeft.right - TempRectLowerLeft.left,
								TempRectLowerLeft.bottom - TempRectLowerLeft.top, SWP_NOZORDER);
			//upper left corner
		m_Border42.SetWindowPos(NULL, TempRectUpperLeft.left, TempRectUpperLeft.top,
								TempRectUpperLeft.right - TempRectUpperLeft.left,
								TempRectUpperLeft.bottom - TempRectUpperLeft.top, SWP_NOZORDER);
			//lower right corner
		m_Border39.SetWindowPos(NULL, TempRectLowerRight.left, TempRectLowerLeft.top - 20,
								TempRectLowerRight.right - TempRectLowerRight.left,
								TempRectLowerRight.bottom - TempRectLowerRight.top, SWP_NOZORDER);
			//upper right corner
		m_Border43.SetWindowPos(NULL, TempRectUpperRight.left, TempRectUpperRight.top,
								TempRectUpperRight.right - TempRectUpperRight.left,
								TempRectUpperRight.bottom - TempRectUpperRight.top, SWP_NOZORDER);

		//bottom
		m_Border54.SetWindowPos(NULL, TempRectLowerLeft.right,
								(TempRectLowerLeft.bottom - 20) - (TempRectBottom.bottom - TempRectBottom.top),
								TempRectLowerRight.left - TempRectLowerLeft.right,
								TempRectBottom.bottom - TempRectBottom.top, SWP_NOZORDER);

		//left
		m_Border50.SetWindowPos(NULL, TempRectUpperLeft.left, 
								TempRectUpperLeft.bottom,
								TempRectLeft.right - TempRectLeft.left,
								(TempRectLowerLeft.top - 20) - TempRectUpperLeft.bottom, SWP_NOZORDER);

		//right
		m_Border45.SetWindowPos(NULL, TempRectUpperRight.right - (TempRectRight.right - TempRectRight.left), 
								TempRectUpperRight.bottom,
								TempRectLeft.right - TempRectLeft.left,
								(TempRectLowerLeft.top - 20) - TempRectUpperLeft.bottom, SWP_NOZORDER);

		//top
		m_Border48.SetWindowPos(NULL, TempRectUpperLeft.right, 
								TempRectUpperLeft.top,
								TempRectLowerRight.left - TempRectLowerLeft.right,
								TempRectBottom.bottom - TempRectBottom.top, SWP_NOZORDER);
		//end inspection statistics box
		
		//start filler statistics box
		vGraphicBitmap23.DeleteObject();
		vGraphicBitmap23.LoadBitmap(IDB_BorderCornerInnerLowerLeft4xBigger);
		m_Border40.SetBitmap(vGraphicBitmap23);

		vGraphicBitmap24.DeleteObject();
		vGraphicBitmap24.LoadBitmap(IDB_BorderCornerInnerLowerRight4xBigger);
		m_Border38.SetBitmap(vGraphicBitmap24);

		vGraphicBitmap25.DeleteObject();
		vGraphicBitmap25.LoadBitmap(IDB_BorderCornerInnerUpperLeft4xBigger);
		m_Border33.SetBitmap(vGraphicBitmap25);

		vGraphicBitmap26.DeleteObject();
		vGraphicBitmap26.LoadBitmap(IDB_BorderCornerInnerUpperRight4xBigger);
		m_Border24.SetBitmap(vGraphicBitmap26);

		vGraphicBitmap32.DeleteObject();		//bottom
		vGraphicBitmap32.LoadBitmap(IDB_BorderBottom4xBigger);
		m_Border52.SetBitmap(vGraphicBitmap32);

		vGraphicBitmap33.DeleteObject();	//left
		vGraphicBitmap33.LoadBitmap(IDB_BorderLeft4xBigger);
		m_Border51.SetBitmap(vGraphicBitmap33);

		vGraphicBitmap34.DeleteObject();		//right
		vGraphicBitmap34.LoadBitmap(IDB_BorderRight4xBigger);
		m_Border46.SetBitmap(vGraphicBitmap34);

		vGraphicBitmap35.DeleteObject();	//top
		vGraphicBitmap35.LoadBitmap(IDB_BorderTop4xBigger);
		m_Border34.SetBitmap(vGraphicBitmap35);

		
		m_Border40.GetWindowRect(&TempRectLowerLeft);
		m_Border33.GetWindowRect(&TempRectUpperLeft);
		m_Border38.GetWindowRect(&TempRectLowerRight);
		m_Border24.GetWindowRect(&TempRectUpperRight);
		m_Border52.GetWindowRect(&TempRectBottom);
		m_Border51.GetWindowRect(&TempRectLeft);
		m_Border46.GetWindowRect(&TempRectRight);
		m_Border34.GetWindowRect(&TempRectTop);

		m_Border35.ShowWindow(SW_HIDE);
		m_Border36.ShowWindow(SW_HIDE);


			//lower left corner
		m_Border40.SetWindowPos(NULL, TempRectLowerLeft.left, TempRectLowerLeft.top,
								TempRectLowerLeft.right - TempRectLowerLeft.left,
								TempRectLowerLeft.bottom - TempRectLowerLeft.top, SWP_NOZORDER);
			//upper left corner
		m_Border33.SetWindowPos(NULL, TempRectUpperLeft.left, TempRectUpperLeft.top,
								TempRectUpperLeft.right - TempRectUpperLeft.left,
								TempRectUpperLeft.bottom - TempRectUpperLeft.top, SWP_NOZORDER);
			//lower right corner
		m_Border38.SetWindowPos(NULL, TempRectLowerRight.left, TempRectLowerLeft.top,
								TempRectLowerRight.right - TempRectLowerRight.left,
								TempRectLowerRight.bottom - TempRectLowerRight.top, SWP_NOZORDER);
			//upper right corner
		m_Border24.SetWindowPos(NULL, TempRectUpperRight.left, TempRectUpperRight.top,
								TempRectUpperRight.right - TempRectUpperRight.left,
								TempRectUpperRight.bottom - TempRectUpperRight.top, SWP_NOZORDER);

		//bottom
		m_Border52.SetWindowPos(NULL, TempRectLowerLeft.right,
								TempRectLowerLeft.bottom - (TempRectBottom.bottom - TempRectBottom.top),
								TempRectLowerRight.left - TempRectLowerLeft.right,
								TempRectBottom.bottom - TempRectBottom.top, SWP_NOZORDER);

		//left
		m_Border51.SetWindowPos(NULL, TempRectUpperLeft.left, 
								TempRectUpperLeft.bottom,
								TempRectLeft.right - TempRectLeft.left,
								TempRectLowerLeft.top - TempRectUpperLeft.bottom, SWP_NOZORDER);

		//right
		m_Border46.SetWindowPos(NULL, TempRectUpperRight.right - (TempRectRight.right - TempRectRight.left), 
								TempRectUpperRight.bottom,
								TempRectLeft.right - TempRectLeft.left,
								TempRectLowerLeft.top - TempRectUpperLeft.bottom, SWP_NOZORDER);

		//top
		m_Border34.SetWindowPos(NULL, TempRectUpperLeft.right, 
								TempRectUpperLeft.top,
								TempRectLowerRight.left - TempRectLowerLeft.right,
								TempRectBottom.bottom - TempRectBottom.top, SWP_NOZORDER);
		//end border filler statistics box



		//start header
		vGraphicBitmap27.DeleteObject();
		vGraphicBitmap27.LoadBitmap(IDB_CorneredButtonBlue4xBiggerHeaderInspx);
		m_Header.SetBitmap(vGraphicBitmap27);

		vGraphicBitmap28.DeleteObject();
		vGraphicBitmap28.LoadBitmap(IDB_CorneredButtonBlue4xBiggerHeaderStraight);
		m_Header2.SetBitmap(vGraphicBitmap28);
		//m_Header3.SetBitmap(vGraphicBitmap28);
		m_Header4.SetBitmap(vGraphicBitmap28);

		vGraphicBitmap29.DeleteObject();
		vGraphicBitmap29.LoadBitmap(IDB_CorneredButtonBlue4xBiggerHeaderTitle1);
		m_Header5.SetBitmap(vGraphicBitmap29);

		vGraphicBitmap30.DeleteObject();
		vGraphicBitmap30.LoadBitmap(IDB_CorneredButtonBlue4xBiggerHeaderTitle2);
		m_Header6.SetBitmap(vGraphicBitmap30);

		vGraphicBitmap31.DeleteObject();
		vGraphicBitmap31.LoadBitmap(IDB_CorneredButtonBlue4xBiggerHeaderTitle3);
		m_Header7.SetBitmap(vGraphicBitmap31);

		RECT TempRectEnd, TempRectStart;
		int TempHeaderTileLength;
		m_Header7.GetWindowRect(&TempRectEnd);
		m_Header.GetWindowRect(&TempRectStart);
		TempHeaderTileLength = (TempRectEnd.right - TempRectEnd.left);
		
		m_Header7.SetWindowPos(NULL,TempRectEnd.left, TempRectEnd.top, TempHeaderTileLength,
									TempRectStart.bottom - TempRectStart.top,SWP_NOZORDER);
		m_Header6.SetWindowPos(NULL,TempRectEnd.left - TempHeaderTileLength, TempRectEnd.top, 
									TempHeaderTileLength,
									TempRectStart.bottom - TempRectStart.top,SWP_NOZORDER);
		m_Header5.SetWindowPos(NULL,TempRectEnd.left - TempHeaderTileLength*2, TempRectEnd.top, 
									TempHeaderTileLength,
									TempRectStart.bottom - TempRectStart.top,SWP_NOZORDER);
		m_Header2.SetWindowPos(NULL, TempRectStart.right - 15, TempRectStart.top, 
									TempRectEnd.left - (TempRectStart.right - 15),
									TempRectStart.bottom - TempRectStart.top, SWP_NOZORDER);
		m_Header4.ShowWindow(SW_HIDE);
		m_Header.SetWindowPos(NULL, TempRectStart.left - 15, TempRectStart.top, 
									TempHeaderTileLength,
									TempRectStart.bottom - TempRectStart.top, SWP_NOZORDER);


		//end header
	}
	else if (ResizeXCoor(10) >= 15)
	{
		//start header
		vGraphicBitmap27.DeleteObject();
		vGraphicBitmap27.LoadBitmap(IDB_CorneredButtonBlue2xBiggerHeaderInspx);
		m_Header.SetBitmap(vGraphicBitmap27);

		vGraphicBitmap28.DeleteObject();
		vGraphicBitmap28.LoadBitmap(IDB_CorneredButtonBlue2xBiggerHeaderStraight);
		m_Header2.SetBitmap(vGraphicBitmap28);
		//m_Header3.SetBitmap(vGraphicBitmap28);
		m_Header4.SetBitmap(vGraphicBitmap28);

		vGraphicBitmap29.DeleteObject();
		vGraphicBitmap29.LoadBitmap(IDB_CorneredButtonBlue2xBiggerHeaderTitle1);
		m_Header5.SetBitmap(vGraphicBitmap29);

		vGraphicBitmap30.DeleteObject();
		vGraphicBitmap30.LoadBitmap(IDB_CorneredButtonBlue2xBiggerHeaderTitle2);
		m_Header6.SetBitmap(vGraphicBitmap30);

		vGraphicBitmap31.DeleteObject();
		vGraphicBitmap31.LoadBitmap(IDB_CorneredButtonBlue2xBiggerHeaderTitle3);
		m_Header7.SetBitmap(vGraphicBitmap31);

		RECT TempRectEnd, TempRectStart;
		int TempHeaderTileLength;
		m_Header7.GetWindowRect(&TempRectEnd);
		m_Header.GetWindowRect(&TempRectStart);
		TempHeaderTileLength = (TempRectEnd.right - TempRectEnd.left);
		
		m_Header.SetWindowPos(NULL, TempRectStart.left - 16, TempRectStart.top, TempHeaderTileLength,
									TempRectStart.bottom - TempRectStart.top, SWP_NOZORDER);
		m_Header2.SetWindowPos(NULL, TempRectStart.right - 16, TempRectStart.top, 
									(TempRectEnd.left + 88 - TempHeaderTileLength*2) - (TempRectStart.right - 16),
									TempRectStart.bottom - TempRectStart.top, SWP_NOZORDER);
		m_Header7.SetWindowPos(NULL,TempRectEnd.left - 55, TempRectStart.top, TempHeaderTileLength,
									TempRectStart.bottom - TempRectStart.top,SWP_NOZORDER);
		m_Header6.SetWindowPos(NULL,TempRectEnd.left - TempHeaderTileLength + 88, TempRectEnd.top, 
									TempHeaderTileLength,
									TempRectStart.bottom - TempRectStart.top,SWP_NOZORDER);
		m_Header5.SetWindowPos(NULL,TempRectEnd.left - TempHeaderTileLength*2 + 88, TempRectEnd.top, 
									TempHeaderTileLength,
									TempRectStart.bottom - TempRectStart.top,SWP_NOZORDER);
		
		m_Header4.ShowWindow(SW_HIDE);
		


		//end header

		//start replace buttons
		vGraphicBitmap.DeleteObject();	//main menu
		vGraphicBitmap.LoadBitmap(IDB_CorneredButtonBlue2xBiggerMainMenu1);
		m_MainMenu.SetBitmap(vGraphicBitmap);

		vGraphicBitmap1.DeleteObject();
		vGraphicBitmap1.LoadBitmap(IDB_CorneredButtonBlue2xBiggerBack);	//back
		m_BackButton.SetBitmap(vGraphicBitmap1);

		vGraphicBitmap2.DeleteObject();
		vGraphicBitmap2.LoadBitmap(IDB_CorneredButtonGrey2xBiggerFunction1Select);	//fill monitor button
		m_ShadedButton1Selected.SetBitmap(vGraphicBitmap2);

		vGraphicBitmap3.DeleteObject();
		vGraphicBitmap3.LoadBitmap(IDB_CorneredButtonGrey2xBiggerFunction2Select);	//standard deviation button
		m_ShadedButton2Selected.SetBitmap(vGraphicBitmap3);

		vGraphicBitmap4.DeleteObject();
		vGraphicBitmap4.LoadBitmap(IDB_CorneredButtonGrey2xBiggerFunction3Select);	//valve history button
		m_ShadedButton3Selected.SetBitmap(vGraphicBitmap4);

		vGraphicBitmap5.DeleteObject();
		vGraphicBitmap5.LoadBitmap(IDB_CorneredButtonGrey2xBiggerFunction4Select);	//valve distribution button
		m_ShadedButton4Selected.SetBitmap(vGraphicBitmap5);

		vGraphicBitmap6.DeleteObject();
		vGraphicBitmap6.LoadBitmap(IDB_CorneredButtonGrey2xBiggerFunction1);
		m_ShadedButton1.SetBitmap(vGraphicBitmap6);

		vGraphicBitmap7.DeleteObject();
		vGraphicBitmap7.LoadBitmap(IDB_CorneredButtonGrey2xBiggerFunction2);
		m_ShadedButton2.SetBitmap(vGraphicBitmap7);
		
		vGraphicBitmap8.DeleteObject();
		vGraphicBitmap8.LoadBitmap(IDB_CorneredButtonGrey2xBiggerFunction3);
		m_ShadedButton3.SetBitmap(vGraphicBitmap8);
		
		vGraphicBitmap9.DeleteObject();
		vGraphicBitmap9.LoadBitmap(IDB_CorneredButtonGrey2xBiggerFunction4);
		m_ShadedButton4.SetBitmap(vGraphicBitmap9);

		vGraphicBitmap10.DeleteObject();
		vGraphicBitmap10.LoadBitmap(IDB_CorneredButtonGrey2xBiggerFunction5);	//select filler valve button
		m_ShadedButton5.SetBitmap(vGraphicBitmap10);

		vGraphicBitmap48.DeleteObject();
		vGraphicBitmap48.LoadBitmap(IDB_CorneredButtonGrey2xBiggerFunction6);	//select filler valve button
		m_ShadedButton6.SetBitmap(vGraphicBitmap48);

		RECT TempRectRight;
		m_ShadedButton6.GetWindowRect(&TempRectRight);
		m_ShadedButton6.SetWindowPos(NULL, TempRectRight.left - 50, 
								TempRectRight.top - 3,
								TempRectRight.right - TempRectRight.left,
								TempRectRight.bottom - TempRectRight.top, SWP_NOZORDER);


		m_ShadedButton6.ShowWindow(SW_SHOW);
		//end replace buttons

		//replace border 64 right side status box
		vGraphicBitmap11.DeleteObject();
		vGraphicBitmap11.LoadBitmap(IDB_BorderRight2xBigger);	//select filler valve button
		m_Border64.GetWindowRect(&TempRectRight);
		m_Border64.SetBitmap(vGraphicBitmap11);
		m_Border64.SetWindowPos(NULL, TempRectRight.left - 1, 
								TempRectRight.top,
								TempRectRight.right - TempRectRight.left - 1,
								TempRectRight.bottom - TempRectRight.top - 2, SWP_NOZORDER);
		
	}
	else if (ResizeXCoor(10) >= 1)	//800x600
	{
		//start replace buttons
		vGraphicBitmap.DeleteObject();	//main menu
		vGraphicBitmap.LoadBitmap(IDB_CornerMainMenuShort);
		m_MainMenu.SetBitmap(vGraphicBitmap);

		vGraphicBitmap1.DeleteObject();
		vGraphicBitmap1.LoadBitmap(IDB_CornerBackShort);	//back
		m_BackButton.SetBitmap(vGraphicBitmap1);

		RECT TempMainMenuRect, TempBackRect;
		m_MainMenu.GetWindowRect(&TempMainMenuRect);
		m_BackButton.GetWindowRect(&TempBackRect);

		//m_MainMenu.SetWindowPos(NULL,TempMainMenuRect.left - 30, TempMainMenuRect.top, TempMainMenuRect.right - TempMainMenuRect.left,
		//						TempMainMenuRect.bottom - TempMainMenuRect.top, SWP_NOZORDER);
		//m_BackButton.SetWindowPos(NULL,TempBackRect.left - 30, TempBackRect.top + 4, TempBackRect.right - TempBackRect.left,
		//						TempBackRect.bottom - TempBackRect.top, SWP_NOZORDER);

		vGraphicBitmap2.DeleteObject();
		vGraphicBitmap2.LoadBitmap(IDB_CorneredButtonShort1Select);	//fill monitor button
		m_ShadedButton1Selected.SetBitmap(vGraphicBitmap2);

		vGraphicBitmap3.DeleteObject();
		vGraphicBitmap3.LoadBitmap(IDB_CorneredButtonShort2Select);	//standard deviation button
		m_ShadedButton2Selected.SetBitmap(vGraphicBitmap3);

		vGraphicBitmap4.DeleteObject();
		vGraphicBitmap4.LoadBitmap(IDB_CorneredButtonShort3Select);	//valve history button
		m_ShadedButton3Selected.SetBitmap(vGraphicBitmap4);

		vGraphicBitmap5.DeleteObject();
		vGraphicBitmap5.LoadBitmap(IDB_CorneredButtonShort4Select);	//valve distribution button
		m_ShadedButton4Selected.SetBitmap(vGraphicBitmap5);

		vGraphicBitmap6.DeleteObject();
		vGraphicBitmap6.LoadBitmap(IDB_CorneredButtonShort1);
		m_ShadedButton1.SetBitmap(vGraphicBitmap6);

		vGraphicBitmap7.DeleteObject();
		vGraphicBitmap7.LoadBitmap(IDB_CorneredButtonShort2);
		m_ShadedButton2.SetBitmap(vGraphicBitmap7);
		
		vGraphicBitmap8.DeleteObject();
		vGraphicBitmap8.LoadBitmap(IDB_CorneredButtonShort3);
		m_ShadedButton3.SetBitmap(vGraphicBitmap8);
		
		vGraphicBitmap9.DeleteObject();
		vGraphicBitmap9.LoadBitmap(IDB_CorneredButtonShort4);
		m_ShadedButton4.SetBitmap(vGraphicBitmap9);

		vGraphicBitmap10.DeleteObject();
		vGraphicBitmap10.LoadBitmap(IDB_CorneredButtonShort5);	//select filler valve button
		m_ShadedButton5.SetBitmap(vGraphicBitmap10);

		vGraphicBitmap48.DeleteObject();
		vGraphicBitmap48.LoadBitmap(IDB_CorneredButtonShort6);	//select filler valve button
		m_ShadedButton6.SetBitmap(vGraphicBitmap48);
		m_ShadedButton6.ShowWindow(SW_SHOW);
		//end replace buttons
	}
		

	if (ThereIsEnoughMemory(sizeof(int) * vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType], "v valve label array for sort in filler valve dialog"))
		vValveLabels = (int *)malloc(sizeof(int) * vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]);
	else
	{
		ReportErrorMessage("Not Enough Memory for temp vNumberOfHeadsSort in Help Dialog shift down",cEMailInspx, 32000);
		#ifdef _DEBUG
		afxDump << "Not Enough Memory for temp color array in Help Dialog shift down Alloc";
		#endif
	}
	this->SetWindowText(_T("Filler Display"));

}


void CFillerValveDialog::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == vUpdateDisplayTimerHandle)
	{
		KillTimer(vUpdateDisplayTimerHandle);
		OneSecondDisplayUpdate();
		//if (vRefreshSoon)
		//{
		//	m_ChartCtrl.Invalidate();
		//	//vRefreshSoon++;
		//	//if (vRefreshSoon = 3)
		//		vRefreshSoon = 0;
		//}
		int TempTimerResult = SetTimer(vUpdateDisplayTimerHandle,1000,NULL);
		if (!TempTimerResult)
			ReportErrorMessage("Error-Display Timer Failed",cEMailInspx,32000);
	}
	else
	
	if (nIDEvent == vRefreshDisplayTimerHandle)
	{
		KillTimer(vRefreshDisplayTimerHandle);
		m_ChartCtrl.Invalidate();
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CFillerValveDialog::OneSecondDisplayUpdate()
{

	//update time
	vTime =CTime::GetCurrentTime();
	int TempDateToday = vTime.GetDay();
	if (vDateToday != TempDateToday)
	{
		CString TempDateString = vTime.Format("%B %#d, %Y");
		CString TempChar = TempDateString.GetAt(0);
		TempChar.MakeUpper();
		TempDateString.SetAt(0,TempChar.GetAt(0));
		
		SetDlgItemText(IDC_Date,TempDateString);

		vDateToday=TempDateToday;
	}

	CString TempTimeString = " ";
	if (vLocalConfigurationData->vShow24HourTime)
		TempTimeString = vTime.Format("%H:%M:%S");//24 hour time format
	else
		TempTimeString = vTime.Format("%#I:%M:%S%p");//12 hour time format with am pm

	SetDlgItemText(IDC_Time,TempTimeString);

	//set AverageFill and Standard Deviation
	//if (vDisplayMode < 3)	//for all valves
	//{
	//	if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
	//	if (vOldFillerAverage != vLocalSystemData->vFillerAverageWeight)
	//	{
	//		vOldFillerAverage = vLocalSystemData->vFillerAverageWeight;
	//		if (vLocalSystemData->vNumberValveRowDataHave <= 4)
	//			SetDlgItemText(IDC_AverageFill, _T("--"));
	//		else
	//			SetDlgItemText(IDC_AverageFill, dtoa(vOldFillerAverage, 2));
	//	}
	//	double tempAverageStdDev;
	//	tempAverageStdDev = 0;
	//	
	//	if (vLocalSystemData->vValveStandardDeviations)
	//	if (vLocalSystemData->vNumberValveRowDataHave)
	//	{
	//		for (int i=0; i<vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; i++)
	//		{
	//			tempAverageStdDev += vLocalSystemData->vValveStandardDeviations[ i ];
	//		}
	//		tempAverageStdDev = tempAverageStdDev / vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType];
	//		if (tempAverageStdDev < 0)
	//			tempAverageStdDev = 0;
	//		SetDlgItemText(IDC_StandardDeviation, dtoa(tempAverageStdDev, 2));
	//	}
	//}
	//else		//for one valve
	//{
	//	//calculate sd for one valve
	//	//valve requested
	//	if (vLocalSystemData->vValveAverages)
	//		SetDlgItemText(IDC_AverageFill, dtoa(vLocalSystemData->vValveAverages[ vDisplayValveNumber - 1 ], 2));
	//	if (vLocalSystemData->vValveStandardDeviations)
	//		SetDlgItemText(IDC_StandardDeviation, dtoa(vLocalSystemData->vValveStandardDeviations[ vDisplayValveNumber - 1 ], 2));

	//}

	//Update StatusLine1
	if ((vLocalSystemData->vNumberOfValvesThatNeedAttention != vOldNumberOfValvesThatNeedAttentionMonitor) ||
			((vLocalSystemData->vNumberValveRowDataHave > 4) && (vOldNumberofDataRows <= 4)) ||
			((vLocalSystemData->vNumberValveRowDataHave < 4) && (vOldNumberofDataRows > 4)))
	{
		vOldNumberofDataRows = vLocalSystemData->vNumberValveRowDataHave;
		vOldNumberOfValvesThatNeedAttentionMonitor = vLocalSystemData->vNumberOfValvesThatNeedAttention;
		if (vLocalSystemData->vNumberValveRowDataHave <= 4)
			SetDlgItemText(IDC_StatusLine1, _T(" Filler status not acquired"));
		else
		if (vOldNumberOfValvesThatNeedAttentionMonitor == 1)
			SetDlgItemText(IDC_StatusLine1, _T(" 1 valve needs attention"));
		else
		if (vOldNumberOfValvesThatNeedAttentionMonitor)
		{
			CString TempText = " ";
			TempText = dtoa(vOldNumberOfValvesThatNeedAttentionMonitor, 0);
			TempText = " " + TempText  + " valves need attention";
			SetDlgItemText(IDC_StatusLine1, TempText);
		}
		else
			SetDlgItemText(IDC_StatusLine1, _T(" All valves Okay"));

		this->Invalidate();
	}

	//number of samples -update
	//if((vOldNumberofDataRows < 1000) && (vOldNumberofDataRows != vLocalSystemData->vNumberValveRowDataHave))
	//{	
	//	vOldNumberofDataRows = vLocalSystemData->vNumberValveRowDataHave;
	//	int TempNumOfSamples = vOldNumberofDataRows;
	//	if (vDisplayMode < 3)
	//		TempNumOfSamples = TempNumOfSamples * vLocalNumberOfHeads;
	//	SetDlgItemText(IDC_NumberOfSamples,dtoa(TempNumOfSamples));
	//}

	//update inspected count
	//SetDlgItemText(IDC_InspectedCount, dtoa(vLocalSystemData->vTotalContainers, 0));

	//update percent rejected
	//CString TempString = dtoa(vLocalSystemData->vPercentRejected,3) + "%"; 
	//SetDlgItemText(IDC_PercentRejected, TempString);

	//update overfill, underfill and foreign object count
	//vOverfillCount = 0;
	//vUnderfillCount = 0;
	//vForeignObjectCount = 0;
	//for (int TempInspectionNumber = 0; TempInspectionNumber < vGlobalCurrentProduct->vNumberOfInspections; TempInspectionNumber++)
	//{
	//	//if (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cDensityInspection);
	//	//else 
	//	//if	(vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cCheckWeighInspection);
	//	//else 
	//	//if (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cMissingLidInspection)
	//	//else 
	//	if ((vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cContaminantInspection)
	//			|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cIPContaminantInspection)
	//			|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cKernelInspection)
	//			|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cJarBottomContaminantInspection)
	//			|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cIPContaminantInspection)
	//			|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cThresholdContaminantInspection)
	//			|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cTopContaminantInspection)
	//			|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cBottomContaminantInspection)
	//			|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cContaminant2Inspection)
	//			|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cAdvancedContaminant2Inspection)
	//			|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cGlass13mmContaminantInspection)
	//			|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cSpotInROIContaminantInspection)
	//			|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cContaminantBonesInspection)
	//			|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cContaminant22Inspection)
	//			|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cWireContaminantInspection))
	//		vForeignObjectCount = vForeignObjectCount + vLocalSystemData->vXRayCriteriaCount[TempInspectionNumber] + vLocalSystemData->vXRayCriteriaCountButDisabled[TempInspectionNumber];
	//	else 
	//	if	((vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cUnderfillByWeightInspection)
	//			|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cUnderfillInspection))
	//		vUnderfillCount = vUnderfillCount + vLocalSystemData->vXRayCriteriaCount[TempInspectionNumber] + vLocalSystemData->vXRayCriteriaCountButDisabled[TempInspectionNumber];
	//	else 
	//	if ((vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cOverfillByWeightInspection)
	//			|| (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInspectionType == cOverfillInspection))
	//		vOverfillCount = vOverfillCount + vLocalSystemData->vXRayCriteriaCount[TempInspectionNumber] + vLocalSystemData->vXRayCriteriaCountButDisabled[TempInspectionNumber];
	//}

	//if (vOldOverfillCount != vOverfillCount)
	//{
	//	vOldOverfillCount = vOverfillCount;
	//	SetDlgItemText(IDC_OverfillCount, dtoa(vOldOverfillCount));
	//}
	//if (vOldUnderfillCount != vUnderfillCount)
	//{
	//	vOldUnderfillCount = vUnderfillCount;
	//	SetDlgItemText(IDC_UnderfillCount, dtoa(vOldUnderfillCount));
	//}
	//if (vOldForeignObjectCount != vForeignObjectCount)
	//{
	//	vOldForeignObjectCount = vForeignObjectCount;
	//	SetDlgItemText(IDC_ForiegnObjectCount, dtoa(vOldForeignObjectCount));
	//}
}



BOOL CFillerValveDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd * TempWindow = NULL;
	if (pMsg->message == cUpdateTitleBarMessage)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText);
		if (TempWindow)
			TempWindow->Invalidate(false);
		//UpdateDisplay();
		return true;  //true indicates it has been handled, so do not process
	}
	else
	if (pMsg->message == WM_KEYDOWN)
	{
		if (vGlobalShiftKeyDown)
		{
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
	//		UpdateDisplay();
		}
		else
		switch (pMsg->wParam)
		{
			case 113: // F2:
//				OnSubFunction5Button();
				OnShadedbutton1();
			break;
			case 114: // F3:
	//			OnSubFunction7Button();
				OnShadedbutton3();
			break;
			case 115: // F4:
				OnShadedbutton2();//			OnSubFunction6Button();
			break;
			case 116: // F5:
				OnShadedbutton1();//OnSubFunction1Button();
			break;
			case 117: // F6 Key - Function 5:
				OnShadedbutton4();//	OnSubFunction4Button();
			break;
			case 118: // F7 Key - Sub Function 1:
				OnMainmenu(); //OnFunction1Button();
			break;
			case 119: // F8 Key - Sub Function 2:
				OnShadedbutton4();//		OnSubFunction8Button();
			break;
			case 120: // F9 Key - Sub Function 3:
				OnShadedbutton2();//			OnSubFunction2Button();
			break;
			case 122: // F11 Key - Sub Function 4:
				OnShadedbutton3();//		OnSubFunction3Button();
			break;
			case 111: // / Key - Sub Function 5:
				OnBackbutton();	//OnFunction2Button();
			break;
			case 106: // * Key - Sub Function 6:
				OnShadedbutton5();//OnFunction5Button();
				//OnFunction4Button();
			break;
			case 109: // - Key - Sub Function 7  Decimal Point:
				;//OnFunction3Button();
			break;
			case 107: // + Key - Sub Function 8:
				OnStnClickedShadedbutton7();
			break;
			case 16: // Shift Key
			case 0x60:  //0 key on KeyPad
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
			break;
			case 36:  //Home key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			//	UpdateDisplay();
			break;
			case cPageDown: // Help
				PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 0, 0);
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
			break;
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
		}
		return true;  //true indicates it has been handled, so do not process
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CFillerValveDialog::OnShadedbutton1()
{
	if (vDisplayMode == 5)
	{
		m_ShadedButton7.ShowWindow(SW_SHOW);
		m_ShadedButton7Selected.ShowWindow(SW_HIDE);
	}
	else 
	if (vDisplayMode == 2)
	{
		m_ShadedButton2.ShowWindow(SW_SHOW);
		m_ShadedButton2Selected.ShowWindow(SW_HIDE);
	}
	else 
	if (vDisplayMode == 3)
	{
		m_ShadedButton3.ShowWindow(SW_SHOW);
		m_ShadedButton3Selected.ShowWindow(SW_HIDE);
		m_ShadedButton5.ShowWindow(SW_HIDE); //hide choose valve
		m_ShadedButton6.ShowWindow(SW_SHOW); //show sort
		SetDlgItemText(IDC_FillerValveNumberLabel, _T("# Of Valves:"));
		SetDlgItemText(IDC_FillerValveNumber, dtoa(vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType], 0));
	
		
		SetDlgItemText(IDC_NumberOfSamples, dtoa(vOldNumberofDataRows * vLocalNumberOfHeads));
	}
	else 
	if (vDisplayMode == 4)
	{
		m_ShadedButton4.ShowWindow(SW_SHOW);
		m_ShadedButton4Selected.ShowWindow(SW_HIDE);
		m_ShadedButton5.ShowWindow(SW_HIDE); //hide choose valve
		m_ShadedButton6.ShowWindow(SW_SHOW); //show sort
		SetDlgItemText(IDC_FillerValveNumberLabel, _T("# Of Valves:"));
		SetDlgItemText(IDC_FillerValveNumber, dtoa(vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType], 0));
		SetDlgItemText(IDC_NumberOfSamples, dtoa(vOldNumberofDataRows * vLocalNumberOfHeads));
	}
	vDisplayMode = 1;		//1 "Fill Monitor" Plot all Valves
	
	m_ShadedButton1.ShowWindow(SW_HIDE);
	m_ShadedButton1Selected.ShowWindow(SW_SHOW);

	this->Invalidate();
}


void CFillerValveDialog::OnShadedbutton2()
{
	if (vDisplayMode == 5)
	{
		m_ShadedButton7.ShowWindow(SW_SHOW);
		m_ShadedButton7Selected.ShowWindow(SW_HIDE);
	}
	else 
	if (vDisplayMode == 1)
	{
		m_ShadedButton1.ShowWindow(SW_SHOW);
		m_ShadedButton1Selected.ShowWindow(SW_HIDE);
	}
	else 
	if (vDisplayMode == 3)
	{
		m_ShadedButton3.ShowWindow(SW_SHOW);
		m_ShadedButton3Selected.ShowWindow(SW_HIDE);
		m_ShadedButton5.ShowWindow(SW_HIDE); //hide choose valve
		m_ShadedButton6.ShowWindow(SW_SHOW); //show sort
		SetDlgItemText(IDC_FillerValveNumberLabel, _T("# Of Valves:"));
		SetDlgItemText(IDC_FillerValveNumber, dtoa(vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType], 0));
		SetDlgItemText(IDC_NumberOfSamples, dtoa(vOldNumberofDataRows * vLocalNumberOfHeads));
	}
	else 
	if (vDisplayMode == 4)
	{
		m_ShadedButton4.ShowWindow(SW_SHOW);
		m_ShadedButton4Selected.ShowWindow(SW_HIDE);
		m_ShadedButton5.ShowWindow(SW_HIDE); //hide choose valve
		m_ShadedButton6.ShowWindow(SW_SHOW); //show sort
		SetDlgItemText(IDC_FillerValveNumberLabel, _T("# Of Valves:"));
		SetDlgItemText(IDC_FillerValveNumber, dtoa(vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType], 0));
		SetDlgItemText(IDC_NumberOfSamples, dtoa(vOldNumberofDataRows * vLocalNumberOfHeads));
	}
	vDisplayMode =2;		// 2 "Fill Standard Deviation" plot all standard deviations
	
	m_ShadedButton2.ShowWindow(SW_HIDE);
	m_ShadedButton2Selected.ShowWindow(SW_SHOW);

	this->Invalidate();
}

void CFillerValveDialog::OnShadedbutton3()
{
//ask which valve to look at
	CNumericEntryDialog INumericEntryDialog;  
	char TempStringBuff[5] = " ";
	int TempErr = _itoa_s(vDisplayValveNumber,TempStringBuff,5,10);
	INumericEntryDialog.vEditString = TempStringBuff;

	INumericEntryDialog.m_DialogTitleStaticText1 = "Filler Valve Monitoring System";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Plot Fill Level Over Time"/*numberofheads*/;
	CString TempText = " ";
	TempText.LoadString(IDS_Product);
	INumericEntryDialog.m_DialogTitleStaticText3 = TempText + ": " + *vGlobalCurrentProduct->GetProductName();
	INumericEntryDialog.m_DialogTitleStaticText4 = "Choose Filler Valve Number:";
		
	TempErr = _itoa_s(vLocalNumberOfHeads,TempStringBuff,5, 10);
	INumericEntryDialog.m_UnitsString = "1 - " + (CString)TempStringBuff;

	INumericEntryDialog.vAllowNegative = false;
	INumericEntryDialog.vIntegerOnly = true;
	INumericEntryDialog.vAllowZero = false;
	INumericEntryDialog.vMaxValue = vLocalNumberOfHeads;
	INumericEntryDialog.vMinValue = 1;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vDisplayValveNumber = _wtoi(INumericEntryDialog.vEditString);
		SetDlgItemText(IDC_FillerValveNumber,INumericEntryDialog.vEditString);
		this->Invalidate();

	//show button 3 as selected
		if (vDisplayMode == 5)
		{
			m_ShadedButton7.ShowWindow(SW_SHOW);
			m_ShadedButton7Selected.ShowWindow(SW_HIDE);
			m_ShadedButton5.ShowWindow(SW_SHOW); //show choose valve
			m_ShadedButton6.ShowWindow(SW_HIDE); //hide sort
		}
		else 
		if (vDisplayMode == 1)
		{
			m_ShadedButton1.ShowWindow(SW_SHOW);
			m_ShadedButton1Selected.ShowWindow(SW_HIDE);
			m_ShadedButton5.ShowWindow(SW_SHOW); //show choose valve
			m_ShadedButton6.ShowWindow(SW_HIDE); //hide sort
			SetDlgItemText(IDC_FillerValveNumberLabel, _T("Filler Valve #:"));
			SetDlgItemText(IDC_NumberOfSamples, dtoa(vOldNumberofDataRows));
		}
		else 
		if (vDisplayMode == 2)
		{
			m_ShadedButton2.ShowWindow(SW_SHOW);
			m_ShadedButton2Selected.ShowWindow(SW_HIDE);
			m_ShadedButton5.ShowWindow(SW_SHOW); //show choose valve
			m_ShadedButton6.ShowWindow(SW_HIDE); //hide sort
			SetDlgItemText(IDC_FillerValveNumberLabel, _T("Filler Valve #:"));
			SetDlgItemText(IDC_NumberOfSamples, dtoa(vOldNumberofDataRows));
		}
		else 
		if (vDisplayMode == 4)
		{
			m_ShadedButton4.ShowWindow(SW_SHOW);
			m_ShadedButton4Selected.ShowWindow(SW_HIDE);
		}
		vDisplayMode = 3;		// 3 "Valve History" plot one valve -keypad ask which valve

		m_ShadedButton3.ShowWindow(SW_HIDE);
		m_ShadedButton3Selected.ShowWindow(SW_SHOW);

		this->Invalidate();		//refresh screen
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		KillTimer(vUpdateDisplayTimerHandle);
		CDialog::EndDialog(10);
	}
}

void CFillerValveDialog::OnShadedbutton4()
{ //show histogram of single valve data
	//ask which valve to look at
	CNumericEntryDialog INumericEntryDialog;  
	char TempStringBuff[5] = " ";
	int TempErr = _itoa_s(vDisplayValveNumber,TempStringBuff,5,10);
	INumericEntryDialog.vEditString = TempStringBuff;

	INumericEntryDialog.m_DialogTitleStaticText1 = "Filler Valve Monitoring System";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Plot Histogram"/*numberofheads*/;
	CString TempText = " ";
	TempText.LoadString(IDS_Product);
	INumericEntryDialog.m_DialogTitleStaticText3 = TempText + ": " + *vGlobalCurrentProduct->GetProductName();
	INumericEntryDialog.m_DialogTitleStaticText4 = "Choose Filler Valve Number:";
		
	TempErr = _itoa_s(vLocalNumberOfHeads,TempStringBuff,5, 10);
	INumericEntryDialog.m_UnitsString = "1 - " + (CString)TempStringBuff;

	INumericEntryDialog.vAllowNegative = false;
	INumericEntryDialog.vIntegerOnly = true;
	INumericEntryDialog.vAllowZero = false;
	INumericEntryDialog.vMaxValue = vLocalNumberOfHeads;
	INumericEntryDialog.vMinValue = 1;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vDisplayValveNumber = _wtoi(INumericEntryDialog.vEditString);
		SetDlgItemText(IDC_FillerValveNumber,INumericEntryDialog.vEditString);
		this->Invalidate();

			//show button 4 selected
		if (vDisplayMode == 5)
		{
			m_ShadedButton7.ShowWindow(SW_SHOW);
			m_ShadedButton7Selected.ShowWindow(SW_HIDE);
			m_ShadedButton5.ShowWindow(SW_SHOW); //show choose valve
			m_ShadedButton6.ShowWindow(SW_HIDE); //hide sort
		}
		else 
		if (vDisplayMode == 1)
		{
			m_ShadedButton1.ShowWindow(SW_SHOW);
			m_ShadedButton1Selected.ShowWindow(SW_HIDE);
			m_ShadedButton5.ShowWindow(SW_SHOW); //show choose valve
			m_ShadedButton6.ShowWindow(SW_HIDE); //hide sort
			SetDlgItemText(IDC_FillerValveNumberLabel, _T("Filler Valve #:"));
			SetDlgItemText(IDC_NumberOfSamples, dtoa(vOldNumberofDataRows));
		}
		else 
		if (vDisplayMode == 2)
		{
			m_ShadedButton2.ShowWindow(SW_SHOW);
			m_ShadedButton2Selected.ShowWindow(SW_HIDE);
			m_ShadedButton5.ShowWindow(SW_SHOW); //show choose valve
			m_ShadedButton6.ShowWindow(SW_HIDE); //hide sort
			SetDlgItemText(IDC_FillerValveNumberLabel, _T("Filler Valve #:"));
			SetDlgItemText(IDC_NumberOfSamples, dtoa(vOldNumberofDataRows));
		}
		else 
		if (vDisplayMode == 3)
		{
			m_ShadedButton3.ShowWindow(SW_SHOW);
			m_ShadedButton3Selected.ShowWindow(SW_HIDE);
		}
	
		vDisplayMode = 4;		// 4 "Valve Distribution" Histogram

		m_ShadedButton4.ShowWindow(SW_HIDE);
		m_ShadedButton4Selected.ShowWindow(SW_SHOW);

		this->Invalidate();		//refresh screen
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		KillTimer(vUpdateDisplayTimerHandle);
		CDialog::EndDialog(10);
	}
}

void CFillerValveDialog::OnMainmenu()
{
	KillTimer(vUpdateDisplayTimerHandle);
	free(vValveLabels);
	CDialog::EndDialog(10);
}

void CFillerValveDialog::OnShadedbutton5()
{
	// Select Filler Valve
	CNumericEntryDialog INumericEntryDialog;  
	char TempStringBuff[5] = " ";
	int TempErr;
	CString TempText = " ";

	if (vDisplayMode == 3){
		
		TempErr = _itoa_s(vDisplayValveNumber,TempStringBuff,5,10);
		INumericEntryDialog.vEditString = TempStringBuff;

		INumericEntryDialog.m_DialogTitleStaticText1 = "Filler Valve Monitoring System";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Plot Fill Level Over Time"/*numberofheads*/;
		
		TempText.LoadString(IDS_Product);
		INumericEntryDialog.m_DialogTitleStaticText3 = TempText + ": " + *vGlobalCurrentProduct->GetProductName();
		INumericEntryDialog.m_DialogTitleStaticText4 = "Choose Filler Valve Number:";
		
		TempErr = _itoa_s(vLocalNumberOfHeads,TempStringBuff,5, 10);
		INumericEntryDialog.m_UnitsString = "1 - " + (CString)TempStringBuff;

		INumericEntryDialog.vIntegerOnly = true;
		INumericEntryDialog.vAllowZero = false;
		INumericEntryDialog.vAllowNegative = false;
		INumericEntryDialog.vMaxValue = vLocalNumberOfHeads;
		INumericEntryDialog.vMinValue = 1;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vDisplayValveNumber = _wtoi(INumericEntryDialog.vEditString);
			SetDlgItemText(IDC_FillerValveNumber,INumericEntryDialog.vEditString);
			this->Invalidate();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			KillTimer(vUpdateDisplayTimerHandle);
			CDialog::EndDialog(10);
		}
	}
	else if (vDisplayMode = 4){
		//ask which valve to look at
		TempErr = _itoa_s(vDisplayValveNumber,TempStringBuff,5,10);
		INumericEntryDialog.vEditString = TempStringBuff;

		INumericEntryDialog.m_DialogTitleStaticText1 = "Filler Valve Monitoring System";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Plot Histogram"/*numberofheads*/;
		TempText.LoadString(IDS_Product);
		INumericEntryDialog.m_DialogTitleStaticText3 = TempText + ": " + *vGlobalCurrentProduct->GetProductName();
		INumericEntryDialog.m_DialogTitleStaticText4 = "Choose Filler Valve Number:";
		
		TempErr = _itoa_s(vLocalNumberOfHeads,TempStringBuff,5, 10);
		INumericEntryDialog.m_UnitsString = "1 - " + (CString)TempStringBuff;

		INumericEntryDialog.vIntegerOnly = true;
		INumericEntryDialog.vMaxValue = vLocalNumberOfHeads;
		INumericEntryDialog.vMinValue = 1;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vDisplayValveNumber = _wtoi(INumericEntryDialog.vEditString);
			SetDlgItemText(IDC_FillerValveNumber,INumericEntryDialog.vEditString);
			this->Invalidate();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			KillTimer(vUpdateDisplayTimerHandle);
			free(vValveLabels);
			CDialog::EndDialog(10);
		}
	}
}

void CFillerValveDialog::OnBackbutton()
{
	if (vValveLabels)
		free(vValveLabels);
	KillTimer(vUpdateDisplayTimerHandle);
	CDialog::EndDialog(true);
}


void CFillerValveDialog::OnStnClickedShadedbutton6()
{
	CSelectItemDialog ISelectItemDialog;  
		
	//Set dialog box data titles and number value
	CString TempText;
	ISelectItemDialog.vTitleString = "Select Graph Sorting Method";
	ISelectItemDialog.vBackgroundColor = cGreen;
	ISelectItemDialog.vNumberOfItems = 3;
	CString TempString = "";

	TempString = "Sort by Valve # (default)";
	if (vValveSortOrder == 1)
		TempString = TempString + " - Current Selection";
	ISelectItemDialog.vItems[0] = TempString;

	if (vDisplayMode == 1)
		TempString = "Sort by Highest Fill Level";
	else 
	if (vDisplayMode == 5)
		TempString = "Most Low Fill Counts";
	else //if (vDisplayMode == 2)
		TempString = "Sort by Highest Standard Deviation";

	if (vValveSortOrder == 2)
		TempString = TempString + " - Current Selection";
	ISelectItemDialog.vItems[1] = TempString;

	if (vDisplayMode == 1)
		TempString = "Sort by Lowest Fill Level";
	else 
	if (vDisplayMode == 5)
		TempString = "Fewest Low Fill Counts";
	else //if (vDisplayMode == 2)
		TempString = "Sort by Lowest Standard Deviation";
	if (vValveSortOrder == 3)
		TempString = TempString + " - Current Selection";
	ISelectItemDialog.vItems[2] = TempString;
	
//	ISelectItemDialog.m_DialogTitleStaticText2 = ;
//	ISelectItemDialog.m_DialogTitleStaticText3 = "";

	//ISelectItemDialog.vInAutoSetup = false;
		//Pass control to dialog box and display
	int nResponse = ISelectItemDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vValveSortOrder = ISelectItemDialog.vItemNumberSelected;
		this->Invalidate();
		//vRefreshSoon = 1;
	}
}

void CFillerValveDialog::OnStnClickedShadedbutton7()
{  //display the number of rejects per valve plot
	if (vDisplayMode == 2)
	{
		m_ShadedButton2.ShowWindow(SW_SHOW);
		m_ShadedButton2Selected.ShowWindow(SW_HIDE);
	}
	else 
	if (vDisplayMode == 1)
	{
		m_ShadedButton1.ShowWindow(SW_SHOW);
		m_ShadedButton1Selected.ShowWindow(SW_HIDE);
	}
	else 
	if (vDisplayMode == 3)
	{
		m_ShadedButton3.ShowWindow(SW_SHOW);
		m_ShadedButton3Selected.ShowWindow(SW_HIDE);
		m_ShadedButton5.ShowWindow(SW_HIDE); //hide choose valve
		m_ShadedButton6.ShowWindow(SW_SHOW); //show sort
		SetDlgItemText(IDC_FillerValveNumberLabel, _T("# Of Valves:"));
		SetDlgItemText(IDC_FillerValveNumber, dtoa(vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType], 0));
	
		SetDlgItemText(IDC_NumberOfSamples, dtoa(vOldNumberofDataRows * vLocalNumberOfHeads));
	}
	else 
	if (vDisplayMode == 4)
	{
		m_ShadedButton4.ShowWindow(SW_SHOW);
		m_ShadedButton4Selected.ShowWindow(SW_HIDE);
		m_ShadedButton5.ShowWindow(SW_HIDE); //hide choose valve
		m_ShadedButton6.ShowWindow(SW_SHOW); //show sort
		SetDlgItemText(IDC_FillerValveNumberLabel, _T("# Of Valves:"));
		SetDlgItemText(IDC_FillerValveNumber, dtoa(vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType], 0));
		SetDlgItemText(IDC_NumberOfSamples, dtoa(vOldNumberofDataRows * vLocalNumberOfHeads));
	}

	vDisplayMode = 5;		//5 Eject Counts, Plot all Valves
	
	m_ShadedButton7.ShowWindow(SW_HIDE);
	m_ShadedButton7Selected.ShowWindow(SW_SHOW);

	this->Invalidate();
	//vRefreshSoon = 1;
}


void CFillerValveDialog::OnStnClickedShadedbutton7selected()
{
	OnStnClickedShadedbutton7();
}


void CFillerValveDialog::OnStnClickedShadedbutton4selected()
{
	OnShadedbutton4();
}


void CFillerValveDialog::OnStnClickedShadedbutton3selected()
{
	OnShadedbutton3();
}


void CFillerValveDialog::OnStnClickedShadedbutton2selected()
{
	OnShadedbutton2();
}


void CFillerValveDialog::OnStnClickedShadedbutton1selected()
{
	OnShadedbutton1();
}
