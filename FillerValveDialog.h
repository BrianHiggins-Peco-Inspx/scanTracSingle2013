#pragma once
#include "afxwin.h"
#include "ChartCtrl\ChartCtrl.h"


// CFillerValveDialog dialog

class CFillerValveDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CFillerValveDialog)

public:
	CFillerValveDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFillerValveDialog();
	CScanTracDlg *vMainWindowPointer;
	//tProductCollection *vLocalProductCollection;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	CTime vTime;
	int vDateToday;
	int vDisplayMode;  //1 All Valve Plot, 2 All Valve Standard Deviation Plot, 3 One Valve Plot, 4 All Valve Histogram,
	unsigned int vUpdateDisplayTimerHandle;
	unsigned int vRefreshDisplayTimerHandle;
	COLORREF vBackgroundColor;
	HBRUSH vAquaBrush;
	HBRUSH vWhiteBrush;
	int vDisplayValveNumber;
	int vOldNumberOfValvesThatNeedAttentionMonitor;
	double vOldFillerAverage;
	int vOldNumberofDataRows;
	int vLocalNumberOfHeads;
	CChartCtrl m_ChartCtrl;
	DWORD *tempBinInfo; //bins to drop things into
	tCWndCollection vLocalCWndCollection;
	int * vValveLabels;
	BYTE vValveSortOrder;
	BYTE vRefreshSoon;
	CBitmap vGraphicBitmap;
	CBitmap vGraphicBitmap1;
	CBitmap vGraphicBitmap2;
	CBitmap vGraphicBitmap3;
	CBitmap vGraphicBitmap4;
	CBitmap vGraphicBitmap5;
	CBitmap vGraphicBitmap6;
	CBitmap vGraphicBitmap7;
	CBitmap vGraphicBitmap8;
	CBitmap vGraphicBitmap9;
	CBitmap vGraphicBitmap10;
	CBitmap vGraphicBitmap11;
	CBitmap vGraphicBitmap12;
	CBitmap vGraphicBitmap13;
	CBitmap vGraphicBitmap14;
	CBitmap vGraphicBitmap15;
	CBitmap vGraphicBitmap16;
	CBitmap vGraphicBitmap17;
	CBitmap vGraphicBitmap18;
	CBitmap vGraphicBitmap19;
	CBitmap vGraphicBitmap20;
	CBitmap vGraphicBitmap21;
	CBitmap vGraphicBitmap22;
	CBitmap vGraphicBitmap23;
	CBitmap vGraphicBitmap24;
	CBitmap vGraphicBitmap25;
	CBitmap vGraphicBitmap26;
	CBitmap vGraphicBitmap27;
	CBitmap vGraphicBitmap28;
	CBitmap vGraphicBitmap29;
	CBitmap vGraphicBitmap30;
	CBitmap vGraphicBitmap31;
	CBitmap vGraphicBitmap32;
	CBitmap vGraphicBitmap33;
	CBitmap vGraphicBitmap34;
	CBitmap vGraphicBitmap35;
	CBitmap vGraphicBitmap36;
	CBitmap vGraphicBitmap37;
	CBitmap vGraphicBitmap38;
	CBitmap vGraphicBitmap39;
	CBitmap vGraphicBitmap40;
	CBitmap vGraphicBitmap41;
	CBitmap vGraphicBitmap42;
	CBitmap vGraphicBitmap43;
	CBitmap vGraphicBitmap44;
	CBitmap vGraphicBitmap45;
	CBitmap vGraphicBitmap46;
	CBitmap vGraphicBitmap47;
	CBitmap vGraphicBitmap48;
	CBitmap vGraphicBitmap49;
	CBitmap vGraphicBitmap50;
	int vUnderfillCount;
	int vOverfillCount;
	int vForeignObjectCount;

	int vOldUnderfillCount;
	int vOldOverfillCount;
	int vOldForeignObjectCount;

// Dialog Data
	enum { IDD = IDD_FillerValveDialog };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShadedbutton1();
	afx_msg void OnShadedbutton2();
	afx_msg void OnShadedbutton3();
	afx_msg void OnShadedbutton4();

	void OneSecondDisplayUpdate();
	afx_msg void OnMainmenu();
	CStatic m_ShadedButton4;
	CStatic m_ShadedButton3;
	CStatic m_ShadedButton2;
	CStatic m_ShadedButton1;
	CStatic m_ShadedButton1Selected;
	CStatic m_ShadedButton2Selected;
	CStatic m_ShadedButton3Selected;
	CStatic m_ShadedButton4Selected;
	CStatic m_MainBackground2;
	CStatic m_Border22;
	CStatic m_Border23;
	CStatic m_Border19;
	CStatic m_Border20;
	CStatic m_Border21;
	CStatic m_Border25;
	CStatic m_Border30;
	CStatic m_UnderfillCount;
	CStatic m_OverfillCount;
	CStatic m_ForiegnObjectCount;
	afx_msg void OnShadedbutton5();
	CStatic m_ShadedButton5;
	CStatic m_ShadedButton6;
	CStatic m_Border45;
	CStatic m_Border46;
	CStatic m_Border52;
	CStatic m_Border54;
	CStatic m_Border64;
	CStatic m_Border63;
	afx_msg void OnBackbutton();
	CStatic m_AverageFill;
	CStatic m_AverageFillLabel;
	CStatic m_BackButton;
//	CStatic m_Border;
//	CStatic m_Border10;
//	CStatic m_Border11;
//	CStatic m_Border12;
//	CStatic m_Border13;
//	CStatic m_Border14;
//	CStatic m_Border15;
//	CStatic m_Border16;
	CStatic m_Border;
	CStatic m_Border10;
	CStatic m_Border11;
	CStatic m_Border12;
	CStatic m_Border13;
	CStatic m_Border14;
	CStatic m_Border15;
	CStatic m_Border16;
	CStatic m_Border17;
	CStatic m_Border18;
	CStatic m_Border2;
	CStatic m_Border24;
	CStatic m_Border26;
	CStatic m_Border27;
	CStatic m_Border28;
	CStatic m_Border29;
	CStatic m_Border3;
	CStatic m_Border31;
	CStatic m_Border32;
	CStatic m_Border33;
	CStatic m_Border34;
	CStatic m_Border35;
	CStatic m_Border36;
	CStatic m_Border38;
	CStatic m_Border37;
	CStatic m_Border39;
	CStatic m_Border4;
	CStatic m_Border40;
	CStatic m_Border41;
	CStatic m_Border42;
	CStatic m_Border43;
	CStatic m_Border44;
	CStatic m_Border47;
	CStatic m_Border48;
	CStatic m_Border49;
	CStatic m_Border5;
	CStatic m_Border50;
	CStatic m_Border51;
	CStatic m_Border53;
	CStatic m_Border55;
	CStatic m_Border56;
	CStatic m_Border57;
	CStatic m_Border58;
	CStatic m_Border59;
	CStatic m_Border6;
	CStatic m_Border60;
	CStatic m_Border61;
	CStatic m_Border62;
	CStatic m_Border65;
	CStatic m_Border66;
	CStatic m_Border67;
	CStatic m_Border7;
	CStatic m_Border9;
	CStatic m_Border8;
	CStatic m_Date;
	CStatic m_FillerValveNumber;
	CStatic m_FillerValveNumberLabel;
	CStatic m_ForiegnObjectLabel;
	CStatic m_Header;
	CStatic m_Header2;
	CStatic m_Header4;
	CStatic m_Header5;
	CStatic m_Header6;
	CStatic m_Header7;
	CStatic m_HeaderStatus;
	CStatic m_InspectedCount;
	CStatic m_InspectedCountLabel;
	CStatic m_MainMenu;
	CStatic m_NumberofSamplesLabel;
	CStatic m_NumberofSamples;
	CStatic m_OverfillCount1;
	CStatic m_OverfillLabel;
	CStatic m_PercentRejected;
	CStatic m_PercentRejectedLabel;
	CStatic m_StandardDeviation;
	CStatic m_StandardDeviationLabel;
	CStatic m_StatusLabel;
	CStatic m_StatusLine1;
	CStatic m_Time;
	CStatic m_UnderfillCount1;
	CStatic m_UnderfillLabel;
	CStatic m_Border22a;
	afx_msg void OnStnClickedShadedbutton6();
	afx_msg void OnStnClickedShadedbutton7();
	CStatic m_ShadedButton7;
	CStatic m_ShadedButton7Selected;
	afx_msg void OnStnClickedShadedbutton7selected();
	afx_msg void OnStnClickedShadedbutton4selected();
	afx_msg void OnStnClickedShadedbutton3selected();
	afx_msg void OnStnClickedShadedbutton2selected();
	afx_msg void OnStnClickedShadedbutton1selected();
};
