//ScanTrac Side View Source File
// uControllerStatusDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "uControllerStatusDialog.h"
#include "YesNoDialog.h"
#include "NoticeDialog.h"
#include "PasswordEntryDialog.h"
#include "SerialPort.h"
#include "NumericEntryDialog.h"
#include "HelpDialog.h"
#include "ACPowerMoniter.h"
#include "HistogramDisplayDialog.h"
#include "Process.h" //for _execl or _spawn
#include "ScanTracDlg.h"
#include "SystemConfigurationData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CuControllerStatusDialog dialog
extern bool vGlobalFPGAVersion10Point0OrHigher;
extern BYTE vGlobalShiftKeyDown;
extern CACPowerMoniter *vGlobalACPowerMoniter;
extern 	CSerialPort *vGlobaluCSerialPort;
extern unsigned long vGlobalDialogTitleColor;
extern int vGlobalPasswordLevel;


CuControllerStatusDialog::CuControllerStatusDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CuControllerStatusDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CuControllerStatusDialog)
	//}}AFX_DATA_INIT
	vMakeExitButtonGreen = false;
	vDisplayTimerHandle = 77;
	vOldCurrentACInputVoltage = 0;
	vOldCurrentACInputVoltageMinimum = 500;
	vOldCurrentACInputVoltageMaximum = 0;
	vOld5 = 0xFFFF;//0;
	vOld33 = 0xFFFF;//0;
	vOld25 = 0xFFFF;//0;
	vOld12 = 0xFFFF;//0;
	vOldMinus12 = 0xFFFF;//0;
	vOld15 = 0xFFFF;//0;

	vOldMax5 = 0;
	vOldMax33 = 0;
	vOldMax25 = 0;
	vOldMax12 = 0;
	vOldMaxMinus12 = 0;
	vOldMax15 = 0;

	vOldMin5 = 0;
	vOldMin33 = 0;
	vOldMin25 = 0;
	vOldMin12 = 0;
	vOldMinMinus12 = 0;
	vOldMin15 = 0;

	vOlduCWatchDogCount = 0;
	vOlduCResetCount = 0;
	vOlduCMajorVersionNumber = 0;
	vOlduCMinorVersionNumber = 0;
	vOldNoCommunicationsWithUController = true;
	vRunningDisplayedInVolts = true;
	vOldXRayOnNotice = false;
	vChangeMade = false;
	vMinus12DisplayValue = 15;
	vPlus15DisplayValue = 15;
	vOriginalACPowerMonitorOffset = 0;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function5Display);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_VoltageError13);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_VoltageError9);
	vLocalCWndCollection.Add(&m_VoltageError8);
	vLocalCWndCollection.Add(&m_VoltageError7);
	vLocalCWndCollection.Add(&m_VoltageError6);
	vLocalCWndCollection.Add(&m_VoltageError5);
	vLocalCWndCollection.Add(&m_VoltageError3);
	vLocalCWndCollection.Add(&m_VoltageError4);
	vLocalCWndCollection.Add(&m_VoltageError2);
	vLocalCWndCollection.Add(&m_VoltageError12);
	vLocalCWndCollection.Add(&m_VoltageError11);
	vLocalCWndCollection.Add(&m_VoltageError10);
	vLocalCWndCollection.Add(&m_VoltageError1);
	vLocalCWndCollection.Add(&m_Max120);
	vLocalCWndCollection.Add(&m_Min120VAC);
	vLocalCWndCollection.Add(&m_Max120VAC);
	vLocalCWndCollection.Add(&m_Min120);
	vLocalCWndCollection.Add(&m_120VACLabel);
	vLocalCWndCollection.Add(&m_120VAC);
	vLocalCWndCollection.Add(&m_120);
	vLocalCWndCollection.Add(&m_MinV33);
	vLocalCWndCollection.Add(&m_MinV25);
	vLocalCWndCollection.Add(&m_Min33);
	vLocalCWndCollection.Add(&m_Min25);
	vLocalCWndCollection.Add(&m_MaxV33);
	vLocalCWndCollection.Add(&m_MaxV25);
	vLocalCWndCollection.Add(&m_Max33);
	vLocalCWndCollection.Add(&m_Max25);
	vLocalCWndCollection.Add(&m_33VLabel);
	vLocalCWndCollection.Add(&m_33V);
	vLocalCWndCollection.Add(&m_25VLabel);
	vLocalCWndCollection.Add(&m_25V);
	vLocalCWndCollection.Add(&m_25);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Minus12);
	vLocalCWndCollection.Add(&m_ReadingLabel);
	vLocalCWndCollection.Add(&m_5);
	vLocalCWndCollection.Add(&m_33);
	vLocalCWndCollection.Add(&m_15);
	vLocalCWndCollection.Add(&m_12);
	vLocalCWndCollection.Add(&m_XRaysOn);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_VoltageError14);
	vLocalCWndCollection.Add(&m_12V);
	vLocalCWndCollection.Add(&m_12VLabel);
	vLocalCWndCollection.Add(&m_15V);
	vLocalCWndCollection.Add(&m_15VLabel);
	vLocalCWndCollection.Add(&m_5V);
	vLocalCWndCollection.Add(&m_5VLabel);
	vLocalCWndCollection.Add(&m_CurrentLabel);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_LimitsLabel);
	vLocalCWndCollection.Add(&m_Max12);
	vLocalCWndCollection.Add(&m_Max15);
	vLocalCWndCollection.Add(&m_Max5);
	vLocalCWndCollection.Add(&m_MaxLabel);
	vLocalCWndCollection.Add(&m_MaxMinus12);
	vLocalCWndCollection.Add(&m_MaxV12);
	vLocalCWndCollection.Add(&m_MaxV15);
	vLocalCWndCollection.Add(&m_MaxV5);
	vLocalCWndCollection.Add(&m_MaxVLabel);
	vLocalCWndCollection.Add(&m_MaxVMinus12);
	vLocalCWndCollection.Add(&m_Min12);
	vLocalCWndCollection.Add(&m_Min15);
	vLocalCWndCollection.Add(&m_Min5);
	vLocalCWndCollection.Add(&m_MinLabel);
	vLocalCWndCollection.Add(&m_MinMinus12);
	vLocalCWndCollection.Add(&m_Minus12V);
	vLocalCWndCollection.Add(&m_Minus12VLabel);
	vLocalCWndCollection.Add(&m_MinV12);
	vLocalCWndCollection.Add(&m_MinV15);
	vLocalCWndCollection.Add(&m_MinV5);
	vLocalCWndCollection.Add(&m_MinVLabel);
	vLocalCWndCollection.Add(&m_MinVMinus12);
	vLocalCWndCollection.Add(&m_PowerErrorTableTitle);
	vLocalCWndCollection.Add(&m_RunningLabel);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_VoltageLabel);
}

void CuControllerStatusDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);
	double TempD = 0;
	CString TempString("");
	TempString = "";

	vSubFunction7ButtonEnable = true;
	vSubFunction8ButtonEnable = true;

	vOriginalACPowerMonitorOffset = vLocalConfigurationData->vACPowerMonitorOffset;

	vNormalValue = 193;
	vHighValue = 215;
	vLowValue = 170;
	if (vGlobalFPGAVersion10Point0OrHigher)
	{
		vPlus15DisplayValue = 24;
		SetDlgItemText(IDC_15VLabel, _T("24V IO"));

		vMinus12DisplayValue = 12;
		SetDlgItemText(IDC_Minus12VLabel, _T("-12 V"));

		vNormalValue = cuCNormalVoltageValue;
		vHighValue = cuCHighVoltageValue;
		vLowValue = cuCLowVoltageValue;
	}
	else
	{
		m_33VLabel.ShowWindow(SW_SHOW);
		m_33V.ShowWindow(SW_SHOW);
		m_MinV33.ShowWindow(SW_SHOW);
		m_Min33.ShowWindow(SW_SHOW);
		m_Max33.ShowWindow(SW_SHOW);
		m_MaxV33.ShowWindow(SW_SHOW);
		//m_33.ShowWindow(SW_SHOW);

		m_25VLabel.ShowWindow(SW_SHOW);
		m_25V.ShowWindow(SW_SHOW);
		m_MinV25.ShowWindow(SW_SHOW);
		m_Min25.ShowWindow(SW_SHOW);
		m_Max25.ShowWindow(SW_SHOW);
		m_MaxV25.ShowWindow(SW_SHOW);
		//m_25.ShowWindow(SW_SHOW);
	}

	TempD = vLowValue;
	TempD = TempD * 5 / vNormalValue;
	TempString = dtoa(TempD,2);
	SetDlgItemText(IDC_MinV5,TempString);

	TempD = vLowValue;
	TempD = TempD * 3.3 / vNormalValue;
	TempString = dtoa(TempD,2);
	SetDlgItemText(IDC_MinV33,TempString);

	TempD = vLowValue;
	TempD = TempD * 2.5 / vNormalValue;
	TempString = dtoa(TempD,2);
	SetDlgItemText(IDC_MinV25,TempString);

	TempD = vLowValue;
	TempD = TempD * 12 / vNormalValue;
	TempString = dtoa(TempD,1);
	SetDlgItemText(IDC_MinV12,TempString);

	TempD = vLowValue;
	TempD = TempD * vMinus12DisplayValue / vNormalValue;
	TempString = "-" + dtoa(TempD,1);
	SetDlgItemText(IDC_MinVMinus12,TempString);

	TempD = vLowValue;
	TempD = TempD * vPlus15DisplayValue / vNormalValue;
	TempString = dtoa(TempD,1);
	SetDlgItemText(IDC_MinV15,TempString);

	TempD = vHighValue;
	TempD = TempD * 5 / vNormalValue;
	TempString = dtoa(TempD,2);
	SetDlgItemText(IDC_MaxV5,TempString);

	TempD = vHighValue;
	TempD = TempD * 3.3 / vNormalValue;
	TempString = dtoa(TempD,2);
	SetDlgItemText(IDC_MaxV33,TempString);

	TempD = vHighValue;
	TempD = TempD * 2.5 / vNormalValue;
	TempString = dtoa(TempD,2);
	SetDlgItemText(IDC_MaxV25,TempString);

	TempD = vHighValue;
	TempD = TempD * 12 / vNormalValue;
	TempString = dtoa(TempD,1);
	SetDlgItemText(IDC_MaxV12,TempString);

	TempD = vHighValue;
	TempD = TempD * vMinus12DisplayValue / vNormalValue;
	TempString = "-" + dtoa(TempD,1);
	SetDlgItemText(IDC_MaxVMinus12,TempString);

	TempD = vHighValue;
	TempD = TempD * vPlus15DisplayValue / vNormalValue;
	TempString = dtoa(TempD,1);
	SetDlgItemText(IDC_MaxV15,TempString);

	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		ShowInspxButtons();
	}
	UpdateDisplay();
	UpdateButtons();

	if (vLocalSystemData->vInAutoSetup)
	{
		vMakeExitButtonGreen = true;
		m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_Function1Button, _T("Return to Auto Setup"));
	}
	else
	{
		CWnd *TempWindow = NULL;
		TempWindow = FindWindow(NULL, _T("Help"));
		if (TempWindow) 
		{
			vMakeExitButtonGreen = true;
			m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_Function1Button, _T("Exit Diagnostics"));
		}
	}
	//if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	//	vSubFunction1ButtonEnable = true;	//m_SubFunction1Button.EnableWindow(true);
	//else
	//	vSubFunction1ButtonEnable = false;	//m_SubFunction1Button.EnableWindow(false);

	if ((vGlobalACPowerMoniter) && (vGlobalACPowerMoniter->vCurrentACInputVoltage))
	{
		m_120VACLabel.ShowWindow(SW_SHOW);
		m_120VAC.ShowWindow(SW_SHOW);
		m_Min120VAC.ShowWindow(SW_SHOW);
		m_Max120VAC.ShowWindow(SW_SHOW);
		m_Min120.ShowWindow(SW_SHOW);
		m_Max120.ShowWindow(SW_SHOW);
		m_VoltageError1.ShowWindow(SW_SHOW);
		m_VoltageError2.ShowWindow(SW_SHOW);
		m_VoltageError3.ShowWindow(SW_SHOW);
		m_VoltageError4.ShowWindow(SW_SHOW);
		m_VoltageError5.ShowWindow(SW_SHOW);
		m_VoltageError6.ShowWindow(SW_SHOW);
		m_VoltageError7.ShowWindow(SW_SHOW);
		m_VoltageError8.ShowWindow(SW_SHOW);
		m_VoltageError9.ShowWindow(SW_SHOW);
		m_VoltageError10.ShowWindow(SW_SHOW);
		m_VoltageError11.ShowWindow(SW_SHOW);
		m_VoltageError12.ShowWindow(SW_SHOW);
		m_VoltageError13.ShowWindow(SW_SHOW);
		m_VoltageError14.ShowWindow(SW_SHOW);

		if (!vLocalSystemData->vOldStylePowerMoniter)
		{
			SetDlgItemText(IDC_VoltageError5, _T("20V on Hot"));
			SetDlgItemText(IDC_VoltageError6, _T("50V on Hot"));
			SetDlgItemText(IDC_VoltageError7, _T("100V on Hot"));
			SetDlgItemText(IDC_VoltageError8, _T("500V on Hot"));
			SetDlgItemText(IDC_VoltageError9, _T("1V on Neutral"));
			SetDlgItemText(IDC_VoltageError10, _T("5V on Neutral"));
			SetDlgItemText(IDC_VoltageError11, _T("10V on Neutral"));
			SetDlgItemText(IDC_VoltageError12, _T("50V on Neutral"));
		}
	}
	else
	{
		SetDlgItemText(IDC_PowerErrorTableTitle, _T("A/C Power Monitor Not Installed"));		
		vSubFunction7ButtonEnable = false;	//m_SubFunction7Button.EnableWindow(false);		
		vSubFunction8ButtonEnable = false;	//m_SubFunction8Button.EnableWindow(false);
	}
	if (vLocalConfigurationData->vUse101VACFor10SecondsShutDownLimit)
		m_VoltageError2.SetWindowText(_T("AC Power Sag <101V"));
	else
		m_VoltageError2.SetWindowText(_T("AC Power Sag <106V"));

	int TempTimerResult = SetTimer(vDisplayTimerHandle,1000,NULL);
	if (!TempTimerResult)
		ReportErrorMessage(_T("Error-Display Timer Failed"),cEMailInspx, 32000);
	this->SetWindowText(_T("Voltages"));
	::SetupMenu(vLocalCWndCollection);
}

void CuControllerStatusDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CuControllerStatusDialog)
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function5Display, m_Function5Display);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_VoltageError13, m_VoltageError13);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_VoltageError9, m_VoltageError9);
	DDX_Control(pDX, IDC_VoltageError8, m_VoltageError8);
	DDX_Control(pDX, IDC_VoltageError7, m_VoltageError7);
	DDX_Control(pDX, IDC_VoltageError6, m_VoltageError6);
	DDX_Control(pDX, IDC_VoltageError5, m_VoltageError5);
	DDX_Control(pDX, IDC_VoltageError3, m_VoltageError3);
	DDX_Control(pDX, IDC_VoltageError4, m_VoltageError4);
	DDX_Control(pDX, IDC_VoltageError2, m_VoltageError2);
	DDX_Control(pDX, IDC_VoltageError12, m_VoltageError12);
	DDX_Control(pDX, IDC_VoltageError11, m_VoltageError11);
	DDX_Control(pDX, IDC_VoltageError10, m_VoltageError10);
	DDX_Control(pDX, IDC_VoltageError1, m_VoltageError1);
	DDX_Control(pDX, IDC_Max120, m_Max120);
	DDX_Control(pDX, IDC_Min120VAC, m_Min120VAC);
	DDX_Control(pDX, IDC_Max120VAC, m_Max120VAC);
	DDX_Control(pDX, IDC_Min120, m_Min120);
	DDX_Control(pDX, IDC_120VACLabel, m_120VACLabel);
	DDX_Control(pDX, IDC_120VAC, m_120VAC);
	DDX_Control(pDX, IDC_120, m_120);
	DDX_Control(pDX, IDC_MinV33, m_MinV33);
	DDX_Control(pDX, IDC_MinV25, m_MinV25);
	DDX_Control(pDX, IDC_Min33, m_Min33);
	DDX_Control(pDX, IDC_Min25, m_Min25);
	DDX_Control(pDX, IDC_MaxV33, m_MaxV33);
	DDX_Control(pDX, IDC_MaxV25, m_MaxV25);
	DDX_Control(pDX, IDC_Max33, m_Max33);
	DDX_Control(pDX, IDC_Max25, m_Max25);
	DDX_Control(pDX, IDC_33VLabel, m_33VLabel);
	DDX_Control(pDX, IDC_33V, m_33V);
	DDX_Control(pDX, IDC_25VLabel, m_25VLabel);
	DDX_Control(pDX, IDC_25V, m_25V);
	DDX_Control(pDX, IDC_25, m_25);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Minus12, m_Minus12);
	DDX_Control(pDX, IDC_ReadingLabel, m_ReadingLabel);
	DDX_Control(pDX, IDC_5, m_5);
	DDX_Control(pDX, IDC_33, m_33);
	DDX_Control(pDX, IDC_15, m_15);
	DDX_Control(pDX, IDC_12, m_12);
	DDX_Control(pDX, IDC_XRaysOn, m_XRaysOn);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_VoltageError14, m_VoltageError14);
	DDX_Control(pDX, IDC_12V, m_12V);
	DDX_Control(pDX, IDC_12VLabel, m_12VLabel);
	DDX_Control(pDX, IDC_15V, m_15V);
	DDX_Control(pDX, IDC_15VLabel, m_15VLabel);
	DDX_Control(pDX, IDC_5V, m_5V);
	DDX_Control(pDX, IDC_5VLabel, m_5VLabel);
	DDX_Control(pDX, IDC_CurrentLabel, m_CurrentLabel);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_LimitsLabel, m_LimitsLabel);
	DDX_Control(pDX, IDC_Max12, m_Max12);
	DDX_Control(pDX, IDC_Max15, m_Max15);
	DDX_Control(pDX, IDC_Max5, m_Max5);
	DDX_Control(pDX, IDC_MaxLabel, m_MaxLabel);
	DDX_Control(pDX, IDC_MaxMinus12, m_MaxMinus12);
	DDX_Control(pDX, IDC_MaxV12, m_MaxV12);
	DDX_Control(pDX, IDC_MaxV15, m_MaxV15);
	DDX_Control(pDX, IDC_MaxV5, m_MaxV5);
	DDX_Control(pDX, IDC_MaxVLabel, m_MaxVLabel);
	DDX_Control(pDX, IDC_MaxVMinus12, m_MaxVMinus12);
	DDX_Control(pDX, IDC_Min12, m_Min12);
	DDX_Control(pDX, IDC_Min15, m_Min15);
	DDX_Control(pDX, IDC_Min5, m_Min5);
	DDX_Control(pDX, IDC_MinLabel, m_MinLabel);
	DDX_Control(pDX, IDC_MinMinus12, m_MinMinus12);
	DDX_Control(pDX, IDC_Minus12V, m_Minus12V);
	DDX_Control(pDX, IDC_Minus12VLabel, m_Minus12VLabel);
	DDX_Control(pDX, IDC_MinV12, m_MinV12);
	DDX_Control(pDX, IDC_MinV15, m_MinV15);
	DDX_Control(pDX, IDC_MinV5, m_MinV5);
	DDX_Control(pDX, IDC_MinVLabel, m_MinVLabel);
	DDX_Control(pDX, IDC_MinVMinus12, m_MinVMinus12);
	DDX_Control(pDX, IDC_PowerErrorTableTitle, m_PowerErrorTableTitle);
	DDX_Control(pDX, IDC_RunningLabel, m_RunningLabel);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_VoltageLabel, m_VoltageLabel);
}


BEGIN_MESSAGE_MAP(CuControllerStatusDialog, CDialog)
	//{{AFX_MSG_MAP(CuControllerStatusDialog)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
	ON_BN_CLICKED(IDC_Function5Button, OnFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction1Button, OnSubFunction1Button)
	ON_BN_CLICKED(IDC_SubFunction2Button, OnSubFunction2Button)
	ON_BN_CLICKED(IDC_SubFunction3Button, OnSubFunction3Button)
	ON_BN_CLICKED(IDC_SubFunction4Button, OnSubFunction4Button)
	ON_BN_CLICKED(IDC_SubFunction5Button, OnSubFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction6Button, OnSubFunction6Button)
	ON_BN_CLICKED(IDC_SubFunction7Button, OnSubFunction7Button)
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_TCARD()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CuControllerStatusDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CuControllerStatusDialog message handlers

void CuControllerStatusDialog::OnFunction1Button() 
{
	// Main Menu Button was pressed
	Sleep(1); //is the equivelent of a yeild statement;
	vLocalConfigurationData->vACPowerMonitorOffset = vOriginalACPowerMonitorOffset;
	CDialog::EndDialog(10);
}

void CuControllerStatusDialog::OnFunction2Button() 
{
	// Save and Exit Button was pressed
	if ((!vChangeMade) || (KeepSetupChanges()))
	{
		CDialog::EndDialog(true);
	}
}

void CuControllerStatusDialog::OnFunction3Button() 
{
}

void CuControllerStatusDialog::OnFunction4Button() 
{
	// Cancel
	if (vChangeMade)
	if (AbortSetupChanges())
	{
		vLocalConfigurationData->vACPowerMonitorOffset = vOriginalACPowerMonitorOffset;
		CDialog::OnCancel();
	}
}

void CuControllerStatusDialog::OnFunction5Button() 
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		if (PasswordOK(cSuperInspxPassword,false))
		{
			CNumericEntryDialog INumericEntryDialog;  
	
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(vLocalConfigurationData->vACPowerMonitorOffset,0);

			INumericEntryDialog.m_DialogTitleStaticText1 = "";
			INumericEntryDialog.m_DialogTitleStaticText2 = "A/C Power Monitor Voltage Offset";
			INumericEntryDialog.m_DialogTitleStaticText3 = "Setup System";
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

			INumericEntryDialog.m_UnitsString = "Default: 0, 0 = Disabled, Value in AC Volts";
			INumericEntryDialog.vMaxValue = 10;
			INumericEntryDialog.vMinValue = -10;
			INumericEntryDialog.vAllowNegative = true;
			INumericEntryDialog.vIntegerOnly = true;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vLocalConfigurationData->vACPowerMonitorOffset = _wtoi(INumericEntryDialog.vEditString);
				UpdateDisplay();
				UpdateButtons();
				SetChangeMade();
				//Set dialog box data titles and number value
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				vLocalConfigurationData->vACPowerMonitorOffset = vOriginalACPowerMonitorOffset;
				CDialog::EndDialog(10);
			}
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\nSuper Inspx Password now required\nto change AC Power Monitor Offset\nClear password, enter super, then re-do";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
		}
	}
}

void CuControllerStatusDialog::OnSubFunction1Button() 
{
}

void CuControllerStatusDialog::OnSubFunction2Button() 
{  
}

void CuControllerStatusDialog::OnSubFunction3Button() 
{

}

void CuControllerStatusDialog::OnSubFunction4Button() 
{
	/*
	CNoticeDialog TempNoticeDialog;
	
	if(vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	{
		int TempTimerResult = SetTimer(vExitTimerHandle,500,NULL);
		if (!TempTimerResult)
			ReportErrorMessage("Error-Exit Timer Failed",cEMailInspx,32000);

		int TempError = _spawnl(_P_NOWAIT, "C:\\Ifc59\\bin\\IDX21.exe", "C:\\Ifc59\\bin\\IDX21.exe", "-a");

		if (errno)
			ReportErrorMessage("could not launch explorer: " + dtoa(errno,0),cEMailInspx,22);
	}
	else
	{
		TempNoticeDialog.vNoticeText = "\n\n\nCannot run Frame Grabber Test while ScanTrac is running.";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}
	*/
}

void CuControllerStatusDialog::OnSubFunction5Button() 
{  
  ClearMinMaxVoltages();
}

void CuControllerStatusDialog::OnSubFunction6Button() 
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	{ //running displayed in K volts or reading button pressed
		vRunningDisplayedInVolts = !vRunningDisplayedInVolts;
		if (vRunningDisplayedInVolts)
			SetDlgItemText(IDC_VoltageLabel,_T("Voltage"));
		else
			SetDlgItemText(IDC_VoltageLabel,_T("Value"));
		UpdateButtons();
		vOld5 = 0xFFFF;
		vOld33 = 0xFFFF;
		vOld25 = 0xFFFF;
		vOld12 = 0xFFFF;
		vOldMinus12 = 0xFFFF;
		vOld15 = 0xFFFF;

		vOldMax5 = 0;
		vOldMax33 = 0;
		vOldMax25 = 0;
		vOldMax12 = 0;
		vOldMaxMinus12 = 0;
		vOldMax15 = 0;

		vOldMin5 = 0;
		vOldMin33 = 0;
		vOldMin25 = 0;
		vOldMin12 = 0;
		vOldMinMinus12 = 0;
		vOldMin15 = 0;
	}
}

void CuControllerStatusDialog::OnSubFunction7Button() 
{	
	if (((vGlobalACPowerMoniter) && (vGlobalACPowerMoniter->vCurrentACInputVoltage) && (vSubFunction7ButtonEnable == true)) ||
		(vLocalConfigurationData->vSimulateLightControlBoard))
	{
		//read recent file
		//plot data
		int TempDate = cToday;
		int TempChangeType = cHideZeros;
		bool TempShowZeros = false;
		float * TempPtr = NULL;
		CNoticeDialog TempNoticeDialog;
		bool TempChangeDate = true;
		bool TempFoundFiles = false;

		vGlobalACPowerMoniter->IACPowerMoniterLogFile.vShowZeros = false;
		vGlobalACPowerMoniter->IACPowerMoniterLogFile.vDateIndex = 0;
		vGlobalACPowerMoniter->IACPowerMoniterLogFile.vNumberOfValues = 0;

		while (true)
		{
			CHistogramDisplayDialog IHistogramDisplayDialog;
			IHistogramDisplayDialog.vValuesAreFloats = true;
			IHistogramDisplayDialog.vLocalSystemData = vLocalSystemData;
			IHistogramDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
			IHistogramDisplayDialog.vMainWindowPointer = vMainWindowPointer;
			IHistogramDisplayDialog.vPlotRawData = true;
			IHistogramDisplayDialog.vShowCurveFitDataLine = false;
			IHistogramDisplayDialog.vACPowerMoniterFormat = true;

			if (TempChangeDate == true)
			{
				//note:only use getACPowerMoniterLogData(cToday) for initialization, otherwise use cPreviousDay and cNextDay
				vGlobalACPowerMoniter->IACPowerMoniterLogFile.GetACPowerMoniterLogData(TempDate);
				
				if ((!vGlobalACPowerMoniter->IACPowerMoniterLogFile.vFoundFiles) && (TempDate == cToday)) //did not find today, so search for previous
				{
					TempDate = cPreviousDay;
					vGlobalACPowerMoniter->IACPowerMoniterLogFile.GetACPowerMoniterLogData(TempDate);
					if (!vGlobalACPowerMoniter->IACPowerMoniterLogFile.vFoundFiles)
						TempDate = cToday; //did not find any previous files set back to today so will give message Power Monitor Not Installed
				}

				TempChangeDate = false;
				TempFoundFiles = vGlobalACPowerMoniter->IACPowerMoniterLogFile.vFoundFiles;
				if (TempShowZeros == true)
					vGlobalACPowerMoniter->IACPowerMoniterLogFile.DataLogChangeZoomOrZeros(cShowZeros);
				else
					vGlobalACPowerMoniter->IACPowerMoniterLogFile.DataLogChangeZoomOrZeros(cHideZeros);
			}
			else
			{
				if (!vGlobalACPowerMoniter->IACPowerMoniterLogFile.DataLogChangeZoomOrZeros(TempChangeType))
				{
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.DoModal();
				}
			}
			if (!TempFoundFiles)
			{
				TempFoundFiles = true;
				if (TempDate == cPreviousDay)
				{
					TempNoticeDialog.vNoticeText = "\n\n\nNo Previous Log";
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.DoModal();
				}
				else
				if (TempDate == cNextDay)
				{
					TempNoticeDialog.vNoticeText = "\n\n\nNo Next Log";
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.DoModal();
				}
				else
				{
					TempNoticeDialog.vNoticeText = "\n\n\nAC Power Monitor Not Installed";
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.DoModal();
					break;
				}
			}
			
			IHistogramDisplayDialog.vPointerToTimes = vGlobalACPowerMoniter->IACPowerMoniterLogFile.vCurrentGraphTimes;
			IHistogramDisplayDialog.vPointerToFloats = vGlobalACPowerMoniter->IACPowerMoniterLogFile.vDataFromLogPointer;
			IHistogramDisplayDialog.vNumberOfNumbers = vGlobalACPowerMoniter->IACPowerMoniterLogFile.vNumberOfValues;

			IHistogramDisplayDialog.vTopDataLine = "AC Power Monitor Data";
			if (IHistogramDisplayDialog.vNumberOfNumbers == 0)
			{  //did not find any numbers
				vGlobalACPowerMoniter->IACPowerMoniterLogFile.GetACPowerMoniterLogData(TempDate);
				if (!vGlobalACPowerMoniter->IACPowerMoniterLogFile.vFoundFiles)
				{
					//TempNoticeDialog.vNoticeText = "\n\n\nNo AC Power Monitor Data This Day";
					//TempNoticeDialog.vType = cInformationMessage;
					//TempNoticeDialog.DoModal();
				}
				else
				{ //you found some files for this day
					vGlobalACPowerMoniter->IACPowerMoniterLogFile.vNumberOfValues = 0;
					vGlobalACPowerMoniter->IACPowerMoniterLogFile.vDateIndex = 0;
					vGlobalACPowerMoniter->IACPowerMoniterLogFile.GetACPowerMoniterLogData(TempDate);
				}
				IHistogramDisplayDialog.vDataTitle = "No Data found for: " 
					+ vGlobalACPowerMoniter->IACPowerMoniterLogFile.vCurrentFileDateString;
			}
			else
				IHistogramDisplayDialog.vDataTitle = vGlobalACPowerMoniter->IACPowerMoniterLogFile.vCurrentFileDateString + vGlobalACPowerMoniter->IACPowerMoniterLogFile.vCurrentDayFileTimeRange;

			int nResponse = IHistogramDisplayDialog.DoModal();
			
			if ((nResponse == 37) || (nResponse == 100)) //left arrows or number pad 4
			{
				if (vGlobalACPowerMoniter->IACPowerMoniterLogFile.vMaxZoomedOut)
				{
					TempDate = cPreviousDay;
					TempChangeDate = true;
				}
				else
				{
					TempChangeType = cPanLeft;
					TempNoticeDialog.vNoticeText = "\n\n\nPanned to Farthest Left";
				}
			} 
			else 
			if ((nResponse == 39) || (nResponse == 102)) //right arrow or number pad 6
			{
				if (vGlobalACPowerMoniter->IACPowerMoniterLogFile.vMaxZoomedOut)
				{
					TempDate = cNextDay;
					TempChangeDate = true;
				}
				else
				{
					TempChangeType = cPanRight;
					TempNoticeDialog.vNoticeText = "\n\n\nPanned to Farthest Right";
				}
			} 
			else 
			if ((nResponse == 98) || (nResponse == 40)) //down arrow or key pad 8
			{
				TempChangeType = cZoomIn;
				TempNoticeDialog.vNoticeText = "\n\n\nFully Zoomed In";
			}
			else
			if ((nResponse == 104) || (nResponse == 38)) //up arrow or key pad 2
			{
				TempChangeType = cZoomOut;
				TempNoticeDialog.vNoticeText = "\n\n\nFully Zoomed Out";
			}
			else
			if (nResponse == 99) //9 on number pad
			{
				TempChangeType = cFullZoomIn;
				TempNoticeDialog.vNoticeText = "\n\n\nFully Zoomed In";
			}
			else
			if (nResponse == 105) //3 on number pad
			{
				TempChangeType = cFullZoomOut;
				TempNoticeDialog.vNoticeText = "\n\n\nFully Zoomed Out";
			}
			else
			if (nResponse == 96) //zero on number pad
			{
				TempShowZeros = !TempShowZeros;
				if (TempShowZeros == true)
					TempChangeType = cShowZeros;
				else
			//	if(TempShowZeros == false)		//for clarity
					TempChangeType = cHideZeros;

			}
			else
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				vLocalConfigurationData->vACPowerMonitorOffset = vOriginalACPowerMonitorOffset;
				CDialog::EndDialog(10);
				break;
			}
			else	//down = 40.  up = 38.
				break;
		}
		vGlobalACPowerMoniter->IACPowerMoniterLogFile.FreeGlobalDataPointer();
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nAC Power Monitor Not Installed";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}
}

void CuControllerStatusDialog::OnSubFunction8Button() 
{
	if ((((vGlobalACPowerMoniter) && (vGlobalACPowerMoniter->vCurrentACInputVoltage)) ||
		(vLocalConfigurationData->vSimulateLightControlBoard)) && (vSubFunction8ButtonEnable == true))
	{
		CHelpDialog IHelpDialog;
		int TempResult = 0;
		int TempDate = cToday;
		CString TempString = "";
		TempString = "";
		
		vGlobalACPowerMoniter->IACPowerMoniterLogFile.vDateIndex = 0;
		
		IHelpDialog.vHelpContext = 16;
		IHelpDialog.vACPowerMoniterFormat = true;
		IHelpDialog.vHelpType = 0;
		IHelpDialog.vWindowType = cTextWindow;
		//IHelpDialog.vLocalProductCollection = vLocalProductCollection;
		IHelpDialog.vLocalSystemData = vLocalSystemData;
		IHelpDialog.vMainWindowPointer = vMainWindowPointer;
		IHelpDialog.vLocalConfigurationData = vLocalConfigurationData;
		
	//	IHelpDialog.vProductPointer = vGlobalCurrentProduct;

		while (true)
		{
			TempString = vGlobalACPowerMoniter->IACPowerMoniterLogFile.GetACPowerMoniterErrorLogData(TempDate);
				
			if ((!vGlobalACPowerMoniter->IACPowerMoniterLogFile.vFoundFiles) && (TempDate == cToday)) //did not find today, so search for previous
			{
				TempDate = cPreviousDay;
				TempString = vGlobalACPowerMoniter->IACPowerMoniterLogFile.GetACPowerMoniterErrorLogData(TempDate);
				if (!vGlobalACPowerMoniter->IACPowerMoniterLogFile.vFoundFiles)
					TempDate = cToday; //did not find any previous files set back to today so will give message Power Monitor Not Installed
			}

			IHelpDialog.vACPowerMoniterErrorLogFile = vGlobalACPowerMoniter->IErrorACPowerMoniterLogFile.vLogFileName;
			
			CFileStatus TempStatus;
			if (CFile::GetStatus( IHelpDialog.vACPowerMoniterErrorLogFile, TempStatus))
			IHelpDialog.vOldACLogFileSize = (long)TempStatus.m_size;

			if (!vGlobalACPowerMoniter->IACPowerMoniterLogFile.vFoundFiles)
			{
				CNoticeDialog TempNoticeDialog;
				if (TempDate == cPreviousDay)
					TempNoticeDialog.vNoticeText = "\n\n\nNo Previous Log";
				else
				if (TempDate == cNextDay)
					TempNoticeDialog.vNoticeText = "\n\n\nNo Next Log";
				else
				{
					TempNoticeDialog.vNoticeText = "\n\n\nAC Power Monitor Not Installed";
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.DoModal();
					break;
				}
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.DoModal();
			}
			else
			{
				IHelpDialog.vTextString = TempString;
				IHelpDialog.vTitleString = "AC Power Monitor Error Log " + vGlobalACPowerMoniter->IACPowerMoniterLogFile.vCurrentFileDateString;
			}
			TempResult = IHelpDialog.DoModal();
			if(TempResult == 96){ //zero on number pad
				if(PasswordOK(cTemporaryInspxPassword,false))
				{
					vGlobalACPowerMoniter->IACPowerMoniterLogFile.vShowAllErrorMessages = 
						!vGlobalACPowerMoniter->IACPowerMoniterLogFile.vShowAllErrorMessages;
				}
				TempDate = cToday;
			}
			else 
			if (TempResult == 1000)
				TempDate = cToday;
			else 
			if( (TempResult == 37) || (TempResult == 100) ) //right arrow
				TempDate = cPreviousDay;
			else
			if( (TempResult == 39) || (TempResult == 102) )//left arrow
				TempDate = cNextDay;
			else	//down = 40.  up = 38.
				break;
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nAC Power Monitor Not Installed";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}
}

int CuControllerStatusDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL CuControllerStatusDialog::PreTranslateMessage(MSG* pMsg) 
{
	CWnd * TempWindow = NULL;
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == cUpdateTitleBarMessage)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
		return true;  //true indicates it has been handled, so do not process
	}
	else
	if (pMsg->message == WM_KEYDOWN)
	{
		if (vGlobalShiftKeyDown)
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
		else
		switch (pMsg->wParam)
		{
			case 67:  //c
				if (PasswordOK(cSuperInspxPassword,false))
				{
					vLocalSystemData->vHadAPowerMoniterMessageError = 0;//clear all voltage errors displayed on table
					vLocalSystemData->vHadABrownOut = 0;
					if (vGlobalACPowerMoniter)
						vGlobalACPowerMoniter->vGaveBrownOutError = 0;
					//vLocalSystemData->vHadAnACPowerDropOut = 0;
					vMainWindowPointer->SetInterlockStatus();
					vMainWindowPointer->UpdateClearYellowMessageButton();

					if (vLocalConfigurationData->vUse101VACFor10SecondsShutDownLimit)
						m_VoltageError2.SetWindowText(_T("AC Power Sag <101V"));
					else
						m_VoltageError2.SetWindowText(_T("AC Power Sag <106V"));
					TempWindow = CWnd::GetDlgItem(IDC_VoltageError1);
					if (TempWindow)
						TempWindow->Invalidate(false);
					TempWindow = CWnd::GetDlgItem(IDC_VoltageError2);
					if (TempWindow)
						TempWindow->Invalidate(false);
					TempWindow = CWnd::GetDlgItem(IDC_VoltageError3);
					if (TempWindow)
						TempWindow->Invalidate(false);
					TempWindow = CWnd::GetDlgItem(IDC_VoltageError4);
					if (TempWindow)
						TempWindow->Invalidate(false);
					TempWindow = CWnd::GetDlgItem(IDC_VoltageError5);
					if (TempWindow)
						TempWindow->Invalidate(false);
					TempWindow = CWnd::GetDlgItem(IDC_VoltageError6);
					if (TempWindow)
						TempWindow->Invalidate(false);
					TempWindow = CWnd::GetDlgItem(IDC_VoltageError7);
					if (TempWindow)
						TempWindow->Invalidate(false);
					TempWindow = CWnd::GetDlgItem(IDC_VoltageError8);
					if (TempWindow)
						TempWindow->Invalidate(false);
					TempWindow = CWnd::GetDlgItem(IDC_VoltageError9);
					if (TempWindow)
						TempWindow->Invalidate(false);
					TempWindow = CWnd::GetDlgItem(IDC_VoltageError10);
					if (TempWindow)
						TempWindow->Invalidate(false);
					TempWindow = CWnd::GetDlgItem(IDC_VoltageError11);
					if (TempWindow)
						TempWindow->Invalidate(false);
					TempWindow = CWnd::GetDlgItem(IDC_VoltageError12);
					if (TempWindow)
						TempWindow->Invalidate(false);
					TempWindow = CWnd::GetDlgItem(IDC_VoltageError13);
					if (TempWindow)
						TempWindow->Invalidate(false);
					TempWindow = CWnd::GetDlgItem(IDC_VoltageError14);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
			break;
			case 113: // F2:
				OnSubFunction5Button();
			break;
			case 114: // F3:
				OnSubFunction7Button();
			break;
			case 115: // F4:
				OnSubFunction6Button();
			break;
			case 116: // F5:
				OnSubFunction1Button();
			break;
			case 117: // F6 Key - Function 5:
				OnSubFunction4Button();
			break;
			case 118: // F7 Key - Sub Function 1:
				OnFunction1Button();
			break;
			case 119: // F8 Key - Sub Function 2:
				OnSubFunction8Button();
			break;
			case 120: // F9 Key - Sub Function 3:
				OnSubFunction2Button();
			break;
			case 122: // F11 Key - Sub Function 4:
				OnSubFunction3Button();
			break;
			case 111: // / Key - Sub Function 5:
				OnFunction2Button();
			break;
			case 106: // * Key - Sub Function 6:
				OnFunction4Button();
			break;
			case 109: // - Key - Sub Function 7  Decimal Point:
				OnFunction3Button();
			break;
			case 107: // + Key - Sub Function 8:
				OnFunction5Button();
			break;
			case 16: // Shift Key
			case 0x60:  //0 key on KeyPad
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
			break;
			case 36:  //Home key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
			case cPageDown: // Help
				PostThreadMessage(vGlobalCallingThreadID,
					cShowHelp, 19, 0);
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
			break;
			case cPageUp: // Clear, but do inspx hidden dialog
				if (vGlobalPasswordLevel)
				{
					PasswordOK(cNoPassword, false);
					m_SubFunction2Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
					m_SubFunction6Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
					m_Function5Display.ShowWindow(SW_HIDE);
					m_Function5Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
				}
				else
				{
					PasswordOK(cAnyPasswordToEnter, true);

					if (PasswordOK(cTemporaryInspxPassword, false))
					{
						ShowInspxButtons();
						UpdateButtons();
					}
				}
				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			break;	
			/*
			case c1: // test
				vLocalSystemData->vTemperatureuC =
					vLocalSystemData->vTemperatureuC + 0x0F;
			break;
			*/
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
		}
		return true;  //true indicates it has been handled, so do not process
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CuControllerStatusDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);  //5 is medium large
		if (vGlobalShiftKeyDown)
			pDC->SetTextColor(cBlack);
		else
			pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function1Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5);  //5 is medium large
		if (vMakeExitButtonGreen)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_120VAC)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_120VAC, 5);
		if ((vOldCurrentACInputVoltage < 114) || (vOldCurrentACInputVoltage > 126))//make red if low or high voltage
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Min120)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Min120, 5);
		if (vOldCurrentACInputVoltageMinimum < 114)//make red if low voltage
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_Max120)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Max120, 5);
		if (vOldCurrentACInputVoltageMaximum > 126)//make red if high voltage
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_5)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_5, 5);
		if ((vOld5 < vLowValue) || (vOld5 > vHighValue))
		if (vOld5 < 0xFFFF)//if got a reading, was initalized to 0xFFFF
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_5V)
	{	
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_5V, 5);
		if ((vOld5 < vLowValue) || (vOld5 > vHighValue))
		if (vOld5 < 0xFFFF)//if got a reading, was initalized to 0xFFFF
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_33)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_33, 5);
		if ((vOld33 < vLowValue) || (vOld33 > vHighValue))
		if (vOld33 < 0xFFFF)//if got a reading, was initalized to 0xFFFF
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_33V)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_33V, 5);
		if ((vOld33 < vLowValue) || (vOld33 > vHighValue))
	if (vOld33 < 0xFFFF)//if got a reading, was initalized to 0xFFFF
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		return vLocalSystemData->vRedBrush;
	}
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_25)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_25, 5);
		if ((vOld25 < vLowValue) || (vOld25 > vHighValue))
		if (vOld25 < 0xFFFF)//if got a reading, was initalized to 0xFFFF
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_25V)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_25V, 5);
		if ((vOld25 < vLowValue) || (vOld25 > vHighValue))
		if (vOld25 < 0xFFFF)//if got a reading, was initalized to 0xFFFF
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_15)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_15, 5);
		if ((vOld15 < vLowValue) || (vOld15 > vHighValue))
		if (vOld15 < 0xFFFF)//if got a reading, was initalized to 0xFFFF
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_15V)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_15V, 5);
		if ((vOld15 < vLowValue) || (vOld15 > vHighValue))
	if (vOld15 < 0xFFFF)//if got a reading, was initalized to 0xFFFF
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cWhite);
		return vLocalSystemData->vRedBrush;
	}
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_12)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_12, 5);
		if ((vOld12 < vLowValue) || (vOld12 > vHighValue))
		if (vOld12 < 0xFFFF)//if got a reading, was initalized to 0xFFFF
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_12V)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_12V, 5);
		if ((vOld12 < vLowValue) || (vOld12 > vHighValue))
		if (vOld12 < 0xFFFF)//if got a reading, was initalized to 0xFFFF
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Minus12) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Minus12, 5);
		if ((vOldMinus12 < vLowValue) || (vOldMinus12 > vHighValue))
		if (vOldMinus12 < 0xFFFF)//if got a reading, was initalized to 0xFFFF
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Minus12V)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Minus12V, 5);
		if ((vOldMinus12 < vLowValue) || (vOldMinus12 > vHighValue))
		if (vOldMinus12 < 0xFFFF)//if got a reading, was initalized to 0xFFFF
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Min5)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Min5, 5);
		if (vOldMin5 < vLowValue)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Max5)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Max5, 5);
		if (vOldMax5 >vHighValue)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Min33)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Min33, 5);
		if (vOldMin33 < vLowValue)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Max33)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Max33, 5);
		if (vOldMax33 >vHighValue)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Min25)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Min25, 5);
		if (vOldMin25 < vLowValue)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Max25)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Max25, 5);
		if (vOldMax25 >vHighValue)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Min15)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Min15, 5);
		if (vOldMin15 < vLowValue)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Max15)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Max15, 5);
		if (vOldMax15 >vHighValue)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Min12) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Min12, 5);
		if (vOldMin12 < vLowValue)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Max12) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Max12, 5);
		if (vOldMax12 > vHighValue)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MinMinus12)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinMinus12, 5);
		if (vOldMinMinus12 < vLowValue)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaxMinus12)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaxMinus12, 5);
		if (vOldMaxMinus12 >vHighValue)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	//if (pWnd->GetDlgCtrlID() == IDC_uCResetCount)
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_uCResetCount, 5);
	//	if (vOlduCResetCount > 1)
	//	{
	//		pDC->SetBkMode(TRANSPARENT);
	//		return vLocalSystemData->vRedBrush;
	//	}
	//}
	//if (pWnd->GetDlgCtrlID() == IDC_uCWatchDogCount)
	//{
	//	if (vOlduCWatchDogCount > 1)
	//	{
	//		pDC->SetBkMode(TRANSPARENT);
	//		return vLocalSystemData->vRedBrush;
	//	}
	//}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_VoltageLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_VoltageLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ReadingLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ReadingLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_5) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_5, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_33) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_33, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_25) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_25, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_12) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_12, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Minus12) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Minus12, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_15) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_15, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	
	if (pWnd->GetDlgCtrlID() == IDC_Min5) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Min5, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Min33) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Min33, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Min25) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Min25, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Min12) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Min12, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MinMinus12) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinMinus12, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Min15) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Min15, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	
	if (pWnd->GetDlgCtrlID() == IDC_MaxV5)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaxV5, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaxV33) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaxV33, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaxV25) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaxV25, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaxV12) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaxV12, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaxVMinus12) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaxVMinus12, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaxV15) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaxV15, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Max5)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Max5, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Max33) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Max33, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Max25) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Max25, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Max12) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Max12, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaxMinus12) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaxMinus12, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Max15) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Max15, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	
	if (pWnd->GetDlgCtrlID() == IDC_MinV5) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinV5, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MinV33) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinV33, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MinV25) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinV25, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MinV12) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinV12, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MinVMinus12) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinVMinus12, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MinV15) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinV15, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_5VLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_5VLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_33VLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_33VLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_25VLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_25VLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_12VLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_12VLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Minus12VLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Minus12VLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_15VLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_15VLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_CurrentLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_LimitsLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LimitsLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_RunningLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_RunningLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MinVLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinVLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaxVLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaxVLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MinLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaxLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaxLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}

		//(pWnd->GetDlgCtrlID() == IDC_uCResetCountLabel) ||
		//(pWnd->GetDlgCtrlID() == IDC_uCWatchDogCountLabel) ||

	if (pWnd->GetDlgCtrlID() == IDC_120VACLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_120VACLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Min120VAC) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Min120VAC, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Max120VAC) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Max120VAC, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Min120) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Min120, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Max120) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Max120, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	
	if (pWnd->GetDlgCtrlID() == IDC_PowerErrorTableTitle)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_PowerErrorTableTitle, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	//if ((pWnd->GetDlgCtrlID() == IDC_uCResetCount) ||
	if (pWnd->GetDlgCtrlID() == IDC_5V) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_5V, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_33V) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_33V, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_25V) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_25V, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_12V) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_12V, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Minus12V) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Minus12V, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_15V) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_15V, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_120VAC) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_120VAC, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	//	(pWnd->GetDlgCtrlID() == IDC_uCWatchDogCount))
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	pDC->SetTextColor(cDarkBlue);
	//	return vLocalSystemData->vLightBrownBrush;
	//}
	if (pWnd->GetDlgCtrlID() == IDC_XRaysOn)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaysOn, 7);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cYellow);
		pDC->SetBkColor(cRed);
		return vLocalSystemData->vRedBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function1Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5);  //5 is medium large
		//if (vLEDTestStep >= 0)
		//{
		//	pDC->SetTextColor(cGray);
		//	return vGlobalButtonColorBrush;
		//}
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
  if (pWnd->GetDlgCtrlID() == IDC_Function2Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);  //5 is medium large
		//if (vLEDTestStep >= 0)
		//{
		//	pDC->SetBkMode(TRANSPARENT);
		//	pDC->SetTextColor(cGray);
		//	return vGlobalButtonColorBrush;
		//}
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function3Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function4Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function4Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function5Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		//if (vLEDTestStep >= 0)
		//{
		//	pDC->SetBkMode(TRANSPARENT);
		//	pDC->SetTextColor(cButtonTextColor);
		//	return vLocalSystemData->vRedBrush;
		//}
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Button) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction7ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction8ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	
	if (pWnd->GetDlgCtrlID() == IDC_VoltageError1) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_VoltageError1, 5);
		if (vLocalSystemData)
		{
			if (vLocalSystemData->vHadAPowerMoniterMessageError & 1)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
			else
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cLightGray);
				return vLocalSystemData->vGrayBrush;
			}
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_VoltageError2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_VoltageError2, 5);
		if (vLocalSystemData)
		{
			if (vLocalSystemData->vHadAPowerMoniterMessageError & 2)
			{
				m_VoltageError2.SetWindowText(_T("AC Power Sag <95V"));

				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
			else if (vLocalSystemData->vHadAPowerMoniterMessageError & 4096)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
			else
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cLightGray);
				return vLocalSystemData->vGrayBrush;
			}
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_VoltageError3) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_VoltageError3, 5);
		if (vLocalSystemData)
		{
			if (vLocalSystemData->vHadAPowerMoniterMessageError & 4)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
			else
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cLightGray);
				return vLocalSystemData->vGrayBrush;
			}
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_VoltageError4)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_VoltageError4, 5);
		if (vLocalSystemData)
		{
			if (vLocalSystemData->vHadAPowerMoniterMessageError & 8)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
			else
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cLightGray);
				return vLocalSystemData->vGrayBrush;
			}
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_VoltageError5)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_VoltageError5, 5);
		if (vLocalSystemData)
		{
			if (vLocalSystemData->vHadAPowerMoniterMessageError & 0x10)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cBlue);
				return vLocalSystemData->vYellowBrush;;
			}
			else
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cLightGray);
				return vLocalSystemData->vGrayBrush;
			}
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_VoltageError6)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_VoltageError6, 5);
		if (vLocalSystemData)
		{
			if (vLocalSystemData->vHadAPowerMoniterMessageError & 0x20)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cBlue);
				return vLocalSystemData->vYellowBrush;;
			}
			else
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cLightGray);
				return vLocalSystemData->vGrayBrush;
			}
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_VoltageError7)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_VoltageError7, 5);
		if (vLocalSystemData)
		{
			if (vLocalSystemData->vHadAPowerMoniterMessageError & 0x40)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cBlue);
				return vLocalSystemData->vYellowBrush;;
			}
			else
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cLightGray);
				return vLocalSystemData->vGrayBrush;
			}
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_VoltageError8)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_VoltageError8, 5);
		if (vLocalSystemData)
		{
			if (vLocalSystemData->vHadAPowerMoniterMessageError & 0x80)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cBlue);
				return vLocalSystemData->vYellowBrush;;
			}
			else
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cLightGray);
				return vLocalSystemData->vGrayBrush;
			}
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_VoltageError9)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_VoltageError9, 5);
		if (vLocalSystemData)
		{
			if (vLocalSystemData->vHadAPowerMoniterMessageError & 0x100)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cBlue);
				return vLocalSystemData->vYellowBrush;;
			}
			else
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cLightGray);
				return vLocalSystemData->vGrayBrush;
			}
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_VoltageError10)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_VoltageError10, 5);
		if (vLocalSystemData)
		{
			if (vLocalSystemData->vHadAPowerMoniterMessageError & 0x200)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cBlue);
				return vLocalSystemData->vYellowBrush;;
			}
			else
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cLightGray);
				return vLocalSystemData->vGrayBrush;
			}
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_VoltageError11)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_VoltageError11, 5);
		if (vLocalSystemData)
		{
			if (vLocalSystemData->vHadAPowerMoniterMessageError & 0x400)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cBlue);
				return vLocalSystemData->vYellowBrush;;
			}
			else
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cLightGray);
				return vLocalSystemData->vGrayBrush;
			}
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_VoltageError12)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_VoltageError12, 5);
		if (vLocalSystemData)
		{
			if (vLocalSystemData->vHadAPowerMoniterMessageError & 0x800)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cBlue);
				return vLocalSystemData->vYellowBrush;;
			}
			else
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cLightGray);
				return vLocalSystemData->vGrayBrush;

			//	vGrayBrush;cLightGray,cDarkGray,cGray
				//grey
			}
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_VoltageError13)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_VoltageError13, 5);
		if (vLocalSystemData)
		{
			if (vLocalSystemData->vHadAPowerMoniterMessageError & 0x2000)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
			else
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cLightGray);
				return vLocalSystemData->vGrayBrush;
			}
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function5Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Function5Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_VoltageError14)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_VoltageError14, 5);
		if (vLocalSystemData)
		{
			if (vLocalSystemData->vHadAPowerMoniterMessageError & 0x4000)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cWhite);
				return vLocalSystemData->vRedBrush;
			}
			else
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cLightGray);
				return vLocalSystemData->vGrayBrush;
			}
		}
	}
	return hbr;
}

void CuControllerStatusDialog::OnTCard(UINT idAction, DWORD dwActionData) 
{
	// TODO: Add your message handler code here
	
}

void CuControllerStatusDialog::UpdateDisplay()
{
	double TempD = 0;
	if (vOldXRayOnNotice != vLocalSystemData->vXRayOnNotice)
	{
		vOldXRayOnNotice = vLocalSystemData->vXRayOnNotice;
		if (vOldXRayOnNotice)
			m_XRaysOn.ShowWindow(SW_SHOW);
		else
			m_XRaysOn.ShowWindow(SW_HIDE);
	}

	if (vGlobalACPowerMoniter)
	if (vOldCurrentACInputVoltage != vGlobalACPowerMoniter->vCurrentACInputVoltage)
	{
		vOldCurrentACInputVoltage = vGlobalACPowerMoniter->vCurrentACInputVoltage;
		SetDlgItemText(IDC_120VAC,dtoa(vOldCurrentACInputVoltage,0));
	}

	if (vGlobalACPowerMoniter)
	if (vOldCurrentACInputVoltageMinimum != vGlobalACPowerMoniter->vCurrentACInputVoltageMinimum)
	{
		vOldCurrentACInputVoltageMinimum = vGlobalACPowerMoniter->vCurrentACInputVoltageMinimum;
		SetDlgItemText(IDC_Min120,dtoa(vOldCurrentACInputVoltageMinimum,0));
	}

	if (vGlobalACPowerMoniter)
	if (vOldCurrentACInputVoltageMaximum != vGlobalACPowerMoniter->vCurrentACInputVoltageMaximum)
	{
		vOldCurrentACInputVoltageMaximum = vGlobalACPowerMoniter->vCurrentACInputVoltageMaximum;
		SetDlgItemText(IDC_Max120,dtoa(vOldCurrentACInputVoltageMaximum,0));
	}

	if (vOld5 != vLocalSystemData->v5)
	{
		vOld5 = vLocalSystemData->v5;
		SetDlgItemText(IDC_5, dtoa(vOld5, 0));
		TempD = vOld5;
		if (vRunningDisplayedInVolts)
			TempD = TempD * 5 / vNormalValue;
		SetDlgItemText(IDC_5V,dtoa(TempD,2));
	}
	
	if (vOld33 != vLocalSystemData->v33)
	{
		vOld33 = vLocalSystemData->v33;
		SetDlgItemText(IDC_33, dtoa(vOld33, 0));
		TempD = vOld33;
		if (vRunningDisplayedInVolts)
			TempD = TempD * 3.3 / vNormalValue;
		SetDlgItemText(IDC_33V,dtoa(TempD,2));
	}
	if (vOld25 != vLocalSystemData->v25)
	{
		vOld25 = vLocalSystemData->v25;
		SetDlgItemText(IDC_25,dtoa(vOld25, 0));
		TempD = vOld25;
		if (vRunningDisplayedInVolts)
			TempD = TempD * 2.5 / vNormalValue;
		SetDlgItemText(IDC_25V,dtoa(TempD,2));
	}
	
	if (vOld12 != vLocalSystemData->v12)
	{
		vOld12 = vLocalSystemData->v12;
		SetDlgItemText(IDC_12,dtoa(vOld12, 0));
		TempD = vOld12;
		if (vRunningDisplayedInVolts)
			TempD = TempD * 12 / vNormalValue;
		SetDlgItemText(IDC_12V,dtoa(TempD,1));
	}
	if (vOldMinus12 != vLocalSystemData->vMinus12)
	{
		vOldMinus12 = vLocalSystemData->vMinus12;
		SetDlgItemText(IDC_Minus12,dtoa(vOldMinus12, 0));
		TempD = vOldMinus12;
		if (vRunningDisplayedInVolts)
		{
			TempD = TempD * vMinus12DisplayValue / vNormalValue;
			if (vOldMinus12 > 3750)
				TempD = 0;
		}
		SetDlgItemText(IDC_Minus12V,"-" + dtoa(TempD,1));
	}
	if (vOld15 != vLocalSystemData->v15)
	{
		vOld15 = vLocalSystemData->v15;
		SetDlgItemText(IDC_15,dtoa(vOld15, 0));
		TempD = vOld15;
		if (vRunningDisplayedInVolts)
			TempD = TempD * vPlus15DisplayValue / vNormalValue;
		SetDlgItemText(IDC_15V,dtoa(TempD,1));
	}

	if (vOldMax5 != vLocalSystemData->vMax5)
	{
		vOldMax5 = vLocalSystemData->vMax5;
		if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
		{
			if (vRunningDisplayedInVolts)
			{
				TempD = vOldMax5;
			if (vRunningDisplayedInVolts)
					TempD = TempD * 5 / vNormalValue;
				SetDlgItemText(IDC_Max5,dtoa(TempD,2));
			}
			else
			{
				SetDlgItemText(IDC_Max5, dtoa(vOldMax5, 0));
			}
		}
		else
			SetDlgItemText(IDC_Max5, _T("--"));
	}
	if (vOldMax33 != vLocalSystemData->vMax33)
	{
		vOldMax33 = vLocalSystemData->vMax33;
		if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
		{
			TempD = vOldMax33;
			if (vRunningDisplayedInVolts)
				TempD = TempD * 3.3 / vNormalValue;
			SetDlgItemText(IDC_Max33,dtoa(TempD,2));
		}
		else
			SetDlgItemText(IDC_Max33, _T("--"));
	}
	if (vOldMax25 != vLocalSystemData->vMax25)
	{
		vOldMax25 = vLocalSystemData->vMax25;
		if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
		{
			TempD = vOldMax25;
			if (vRunningDisplayedInVolts)
				TempD = TempD * 2.5 / vNormalValue;
			SetDlgItemText(IDC_Max25,dtoa(TempD,2));
		}
		else
			SetDlgItemText(IDC_Max25, _T("--"));
	}
	if (vOldMax12 != vLocalSystemData->vMax12)
	{
		vOldMax12 = vLocalSystemData->vMax12;
		if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
		{
			TempD = vOldMax12;
			if (vRunningDisplayedInVolts)
				TempD = TempD * 12 / vNormalValue;
			SetDlgItemText(IDC_Max12,dtoa(TempD,1));
		}
		else
			SetDlgItemText(IDC_Max12, _T("--"));
	}
	if (vOldMaxMinus12 != vLocalSystemData->vMaxMinus12)
	{
		vOldMaxMinus12 = vLocalSystemData->vMaxMinus12;
		if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
		{
			TempD = vOldMaxMinus12;
			if (vRunningDisplayedInVolts)
			{
				TempD = TempD * vMinus12DisplayValue / vNormalValue;
				if (vOldMinus12 > 3750)
					TempD = 0;
			}
			SetDlgItemText(IDC_MaxMinus12,"-" + dtoa(TempD,1));
		}
		else
			SetDlgItemText(IDC_MaxMinus12, _T("--"));
	}
	if (vOldMax15 != vLocalSystemData->vMax15)
	{
		vOldMax15 = vLocalSystemData->vMax15;
		if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
		{
			TempD = vOldMax15;
			if (vRunningDisplayedInVolts)
				TempD = TempD * vPlus15DisplayValue / vNormalValue;
			SetDlgItemText(IDC_Max15,dtoa(TempD,1));
		}
		else
			SetDlgItemText(IDC_Max15, _T("--"));
	}

	if (vOldMin5 != vLocalSystemData->vMin5)
	{
		vOldMin5 = vLocalSystemData->vMin5;
		if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
		{
			TempD = vOldMin5;
			if (vRunningDisplayedInVolts)
				TempD = TempD * 5 / vNormalValue;
			SetDlgItemText(IDC_Min5,dtoa(TempD,2));
		}
		else
			SetDlgItemText(IDC_Min5, _T("--"));
	}
	if (vOldMin33 != vLocalSystemData->vMin33)
	{
		vOldMin33 = vLocalSystemData->vMin33;
		if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
		{
			TempD = vOldMin33;
			if (vRunningDisplayedInVolts)
				TempD = TempD * 3.3 / vNormalValue;
			SetDlgItemText(IDC_Min33,dtoa(TempD,2));
		}
		else
			SetDlgItemText(IDC_Min33, _T("--"));
	}
	if (vOldMin25 != vLocalSystemData->vMin25)
	{
		vOldMin25 = vLocalSystemData->vMin25;
		if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
		{
			TempD = vOldMin25;
			if (vRunningDisplayedInVolts)
				TempD = TempD * 2.5 / vNormalValue;
			SetDlgItemText(IDC_Min25,dtoa(TempD,2));
		}
		else
			SetDlgItemText(IDC_Min25, _T("--"));
	}
	if (vOldMin12 != vLocalSystemData->vMin12)
	{
		vOldMin12 = vLocalSystemData->vMin12;
		if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
		{
			TempD = vOldMin12;
			if (vRunningDisplayedInVolts)
				TempD = TempD * 12 / vNormalValue;
			SetDlgItemText(IDC_Min12,dtoa(TempD,1));
		}
		else
			SetDlgItemText(IDC_Min12, _T("--"));
	}
	if (vOldMinMinus12 != vLocalSystemData->vMinMinus12)
	{
		vOldMinMinus12 = vLocalSystemData->vMinMinus12;
		if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
		{
			TempD = vOldMinMinus12;
			if (vRunningDisplayedInVolts)
			{
				TempD = TempD * vMinus12DisplayValue / vNormalValue;
				if (vOldMinus12 > 3750)
					TempD = 0;
			}
			SetDlgItemText(IDC_MinMinus12,"-" + dtoa(TempD,1));
		}
		else
			SetDlgItemText(IDC_MinMinus12, _T("--"));
	}
	if (vOldMin15 != vLocalSystemData->vMin15)
	{
		vOldMin15 = vLocalSystemData->vMin15;
		if (vLocalSystemData->vNumberOfDataSamples >= cNumberOfVoltageSamples)
		{
			TempD = vOldMin15;
			if (vRunningDisplayedInVolts)
				TempD = TempD * vPlus15DisplayValue / vNormalValue;
			SetDlgItemText(IDC_Min15,dtoa(TempD,1));
		}
		else
			SetDlgItemText(IDC_Min15, _T("--"));
	}

	if (vLocalSystemData->vHadAPowerMoniterMessageError & 1)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_VoltageError1);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}
	if ((vLocalSystemData->vHadAPowerMoniterMessageError & 2) 
		|| (vLocalSystemData->vHadAPowerMoniterMessageError & 4096))
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_VoltageError2);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}
	if (vLocalSystemData->vHadAPowerMoniterMessageError & 4)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_VoltageError3);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}
	if (vLocalSystemData->vHadAPowerMoniterMessageError & 8)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_VoltageError4);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}
	if (vLocalSystemData->vHadAPowerMoniterMessageError & 16)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_VoltageError5);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}
	if (vLocalSystemData->vHadAPowerMoniterMessageError & 32)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_VoltageError6);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}
	if (vLocalSystemData->vHadAPowerMoniterMessageError & 64)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_VoltageError7);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}
	if (vLocalSystemData->vHadAPowerMoniterMessageError & 128)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_VoltageError8);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}
	if (vLocalSystemData->vHadAPowerMoniterMessageError & 256)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_VoltageError9);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}
	if (vLocalSystemData->vHadAPowerMoniterMessageError & 512)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_VoltageError10);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}
	if (vLocalSystemData->vHadAPowerMoniterMessageError & 1024)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_VoltageError11);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}
	if (vLocalSystemData->vHadAPowerMoniterMessageError & 2048)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_VoltageError12);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}
	if (vLocalSystemData->vHadAPowerMoniterMessageError & 0x2000)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_VoltageError13);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}
	if (vLocalSystemData->vHadAPowerMoniterMessageError & 0x4000)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_VoltageError14);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}
}

void CuControllerStatusDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == vDisplayTimerHandle)
	{
		UpdateDisplay();
	}
	CDialog::OnTimer(nIDEvent);
}

void CuControllerStatusDialog::ClearMinMaxVoltages()
{
	if (vGlobalACPowerMoniter)
	{
		vGlobalACPowerMoniter->vCurrentACInputVoltageMaximum = 0;
		vGlobalACPowerMoniter->vCurrentACInputVoltageMinimum = 500;
	}
	//9/4/2009
	//clear all Impulse voltage errors displayed on table
	vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError & 0xF00F;

	vLocalSystemData->vMax5 = 0;
	vLocalSystemData->vMax33 = 0;
	vLocalSystemData->vMax25 = 0;
	vLocalSystemData->vMax12 = 0;
	vLocalSystemData->vMaxMinus12 = 0;
	vLocalSystemData->vMax15 = 0;
	if (vGlobalFPGAVersion10Point0OrHigher)
	{
		vLocalSystemData->vMin5 = 0xFFFF;
		vLocalSystemData->vMin33 = 0xFFFF;
		vLocalSystemData->vMin25 = 0xFFFF;
		vLocalSystemData->vMin12 = 0xFFFF;
		vLocalSystemData->vMinMinus12 = 0xFFFF;
		vLocalSystemData->vMin15 = 0xFFFF;
//		vLocalSystemData->vTemperatureuCMaximum = 0xFFFF;
	}
	else
	{
		vLocalSystemData->vMin5 = 0xFF;
		vLocalSystemData->vMin33 = 0xFF;
		vLocalSystemData->vMin25 = 0xFF;
		vLocalSystemData->vMin12 = 0xFF;
		vLocalSystemData->vMinMinus12 = 0xFF;
		vLocalSystemData->vMin15 = 0xFF;
//		vLocalSystemData->vTemperatureuCMaximum = 0xFF;
	}

	vOldMax5 = vLocalSystemData->vMax5;
	vOldMax33 = vLocalSystemData->vMax33;
	vOldMax25 = vLocalSystemData->vMax25;
	vOldMax12 = vLocalSystemData->vMax12;
	vOldMaxMinus12 = vLocalSystemData->vMaxMinus12;
	vOldMax15 = vLocalSystemData->vMax15;
	vOldMin5 = vLocalSystemData->vMin5;
	vOldMin33 = vLocalSystemData->vMin33;
	vOldMin25 = vLocalSystemData->vMin25;
	vOldMin12 = vLocalSystemData->vMin12;
	vOldMinMinus12 = vLocalSystemData->vMinMinus12;
	vOldMin15 = vLocalSystemData->vMin15;

	SetDlgItemText(IDC_Min15, _T("-"));
	SetDlgItemText(IDC_MinMinus12, _T("-"));
	SetDlgItemText(IDC_Min12, _T("-"));
	SetDlgItemText(IDC_Min25, _T("-"));
	SetDlgItemText(IDC_Min33, _T("-"));
	SetDlgItemText(IDC_Min5, _T("-"));
	SetDlgItemText(IDC_Max15, _T("-"));
	SetDlgItemText(IDC_MaxMinus12, _T("-"));
	SetDlgItemText(IDC_Max12, _T("-"));
	SetDlgItemText(IDC_Max25, _T("-"));
	SetDlgItemText(IDC_Max33, _T("-"));
	SetDlgItemText(IDC_Max5, _T("-"));

	if (vLocalSystemData->vHadABrownOut)
	{
		vLocalSystemData->vHadABrownOut = false;
		vLocalSystemData->vHadAPowerMoniterMessageError = 0;	//all flags cleared
		if (vGlobalACPowerMoniter)
		{
			vGlobalACPowerMoniter->vGaveBrownOutError = 0;
			for (int TempIndex = 0; TempIndex < cNumberOfACVoltageSamples; TempIndex++)
				vGlobalACPowerMoniter->vACInputVoltageData[TempIndex] = 0;
		}
		vMainWindowPointer->SetInterlockStatus();
	}
	//if (vLocalSystemData->vHadAnACPowerDropOut)
	//{
	//	vLocalSystemData->vHadAnACPowerDropOut = false;
	//	vMainWindowPointer->SetInterlockStatus();
	//	vMainWindowPointer->UpdateClearYellowMessageButton();
	//}

//vLocalSystemData->vHadAPowerMoniterMessageError = vLocalSystemData->vHadAPowerMoniterMessageError & 4111;//clear all non-critical voltage errors displayed on table (clears bits 1,2,3,4, 13)
	vLocalSystemData->vHadAPowerMoniterMessageError = 0;//clear all errors displayed on table

	CWnd *TempWindow = this;
	if (TempWindow)
		TempWindow->Invalidate(false);
}

void CuControllerStatusDialog::SetChangeMade() 
{
	if (!vChangeMade)
	{
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_Function2Button, _T("Save\nand Exit"));
		m_Function4Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_Function4Button, _T("Cancel"));
		vChangeMade = true;
	}
}

void CuControllerStatusDialog::ShowInspxButtons()
{
	m_Function5Display.ShowWindow(SW_SHOW);
	m_Function5Button.ShowWindow(SW_SHOW);
	//m_SubFunction1Button.ShowWindow(SW_SHOW);
	//m_ArrowLeft5.ShowWindow(SW_SHOW);
//	m_SubFunction2Button.ShowWindow(SW_SHOW);
//	m_ArrowLeft2.ShowWindow(SW_SHOW);
	m_SubFunction6Button.ShowWindow(SW_SHOW);
//	SetDlgItemText(IDC_SubFunction2Button,"Set Temp. Adjust");

	//if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	//{
	//	m_SubFunction1Button.ShowWindow(SW_SHOW);
	//	m_ArrowLeft5.ShowWindow(SW_SHOW);
	//}
	UpdateButtons();
}

void CuControllerStatusDialog::UpdateButtons()
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
//		SetDlgItemText(IDC_SubFunction2Button,"Set Temperature Adjust");
		if (vRunningDisplayedInVolts)
			SetDlgItemText(IDC_SubFunction6Button, _T("Display Running in Values"));
		else
			SetDlgItemText(IDC_SubFunction6Button, _T("Display Running In Volts"));

		SetDlgItemText(IDC_Function5Display,"Offset: " + dtoa(vLocalConfigurationData->vACPowerMonitorOffset));
	}
}


void CuControllerStatusDialog::OnPaint() 
{

	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}


void CuControllerStatusDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
