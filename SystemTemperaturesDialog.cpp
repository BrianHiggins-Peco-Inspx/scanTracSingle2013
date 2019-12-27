// SystemTemperaturesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "SystemTemperaturesDialog.h"
#include "NumericEntryDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSystemTemperaturesDialog dialog


extern bool vGlobalFPGAVersion10Point0OrHigher;
extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;

CSystemTemperaturesDialog::CSystemTemperaturesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemTemperaturesDialog::IDD, pParent)
{

	vChangeMade = false;

	//{{AFX_DATA_INIT(CSystemTemperaturesDialog)
	//}}AFX_DATA_INIT

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button3);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction11Button);
	vLocalCWndCollection.Add(&m_MonoBlockTitle);
	vLocalCWndCollection.Add(&m_MonoBlockMaximumDisplay);
	vLocalCWndCollection.Add(&m_MaximumTitle);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Function5Display);
	vLocalCWndCollection.Add(&m_SubFunction4Display);
	vLocalCWndCollection.Add(&m_SubFunction8Display);
	vLocalCWndCollection.Add(&m_Adjust);
	vLocalCWndCollection.Add(&m_CPUMaximum);
	vLocalCWndCollection.Add(&m_CPUTemps);
	vLocalCWndCollection.Add(&m_CPUTempMaximum);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function3Display);
	vLocalCWndCollection.Add(&m_Maximum);
	vLocalCWndCollection.Add(&m_SubFunction10Button);
	vLocalCWndCollection.Add(&m_SubFunction1Display);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction2Display);
	vLocalCWndCollection.Add(&m_SubFunction3Display);
	vLocalCWndCollection.Add(&m_SubFunction5Display);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction6Display);
	vLocalCWndCollection.Add(&m_SubFunction7Display);
	vLocalCWndCollection.Add(&m_SubFunction9Button);
	vLocalCWndCollection.Add(&m_uCAdjust);
	vLocalCWndCollection.Add(&m_uCMaximum);
	vLocalCWndCollection.Add(&m_ucTemps);
}

void CSystemTemperaturesDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

	vSubFunction1ButtonEnable = true;
	vSubFunction3ButtonEnable = true;
	vSubFunction5ButtonEnable = true;
	vSubFunction7ButtonEnable = true;

	vTemperatureSurfaceAdjust = vLocalConfigurationData->vTemperatureSurfaceAdjust;
	vTemperatureAirInsideAdjust = vLocalConfigurationData->vTemperatureAirInsideAdjust;
	vTemperatureAirOutsideAdjust = vLocalConfigurationData->vTemperatureAirOutsideAdjust;
	vTemperatureLiquidAdjust = vLocalConfigurationData->vTemperatureLiquidAdjust;
	vTemperatureuCAdjust = vLocalConfigurationData->vTemperatureuCAdjust;
	
	vMaximumMonoBlockTemperature = vLocalConfigurationData->vMaximumMonoBlockTemperature;
	vMaximumAirTemperature = vLocalConfigurationData->vMaximumAirTemperature;
	vMaximumLiquidTemperature = vLocalConfigurationData->vMaximumLiquidTemperature;
	vMaximumSurfaceTemperature = vLocalConfigurationData->vMaximumSurfaceTemperature;
	vMaximumTemperatureAirOutside = vLocalConfigurationData->vMaximumTemperatureAirOutside;
	vMaximumuCTemperature = vLocalConfigurationData->vMaximumuCTemperature;
	vMaximumCPUTemperature = vLocalConfigurationData->vMaximumCPUTemperature;
	vSurfaceTemperatureMovedToDXM = vLocalConfigurationData->vSurfaceTemperatureMovedToDXM;

	if (!vGlobalFPGAVersion10Point0OrHigher)
	{
		vSurfaceTemperatureMovedToDXM = false;
		m_SubFunction3Button3.ShowWindow(SW_HIDE);
	}

	vHasOutsideAirTemp = vLocalConfigurationData->vHasOutsideAirTemp;
	vDeltaTemperatureForFilterCloggedError = 
		vLocalConfigurationData->vDeltaTemperatureForFilterCloggedError;
	vDeltaTemperatureForCoolantProblem =
		vLocalConfigurationData->vDeltaTemperatureForCoolantProblem;


//	if (vGlobalFPGAVersion10Point0OrHigher)
//	{
//;//		SetDlgItemText(IDC_SubFunction8Button,"Max. Sur. && Outside Air Temp.");
//	}
//	else
//	if (vLocalConfigurationData->vHasOutsideAirTemp)
//	{
//;//		SetDlgItemText(IDC_SubFunction8Button,"Max Outside Air Temp.");
//	}
	if ((vLocalConfigurationData->vHVPSType == c100WMonoBlockHVPS) || (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock))
	{
		m_MonoBlockMaximumDisplay.ShowWindow(SW_SHOW);
		m_SubFunction11Button.ShowWindow(SW_SHOW);
		m_MaximumTitle.ShowWindow(SW_SHOW);
		m_MonoBlockTitle.ShowWindow(SW_SHOW);
	}
	
	if ((vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock) || (vLocalConfigurationData->vHVPSType == c210WMonoBlockHVPS) || 
		(vLocalConfigurationData->vHVPSType == c100WMonoBlockHVPS) || (vLocalConfigurationData->vHVPSType == cMNXHVPS)) //MNX and 210W MonoBlock dont have coolant or MonoBlock Temp
	{
		m_Function5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
		m_Function5Display.ShowWindow(SW_HIDE);
		m_SubFunction4Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
		m_SubFunction4Display.ShowWindow(SW_HIDE);
		m_SubFunction8Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
		m_SubFunction8Display.ShowWindow(SW_HIDE);
	}

	UpdateDisplay();
	this->SetWindowText(_T("Temperatures"));

	m_Function4Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());

	::SetupMenu(vLocalCWndCollection);
}

void CSystemTemperaturesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemTemperaturesDialog)
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction3Button3, m_SubFunction3Button3);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SubFunction11Button, m_SubFunction11Button);
	DDX_Control(pDX, IDC_MonoBlockTitle, m_MonoBlockTitle);
	DDX_Control(pDX, IDC_MonoBlockMaximumDisplay, m_MonoBlockMaximumDisplay);
	DDX_Control(pDX, IDC_MaximumTitle, m_MaximumTitle);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function5Display, m_Function5Display);
	DDX_Control(pDX, IDC_SubFunction4Display, m_SubFunction4Display);
	DDX_Control(pDX, IDC_SubFunction8Display, m_SubFunction8Display);
	DDX_Control(pDX, IDC_Adjust, m_Adjust);
	DDX_Control(pDX, IDC_CPUMaximum, m_CPUMaximum);
	DDX_Control(pDX, IDC_CPUTemps, m_CPUTemps);
	DDX_Control(pDX, IDC_CPUTempMaximum, m_CPUTempMaximum);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function3Display, m_Function3Display);
	DDX_Control(pDX, IDC_Maximum, m_Maximum);
	DDX_Control(pDX, IDC_SubFunction10Button, m_SubFunction10Button);
	DDX_Control(pDX, IDC_SubFunction1Display, m_SubFunction1Display);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction2Display, m_SubFunction2Display);
	DDX_Control(pDX, IDC_SubFunction3Display, m_SubFunction3Display);
	DDX_Control(pDX, IDC_SubFunction5Display, m_SubFunction5Display);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction6Display, m_SubFunction6Display);
	DDX_Control(pDX, IDC_SubFunction7Display, m_SubFunction7Display);
	DDX_Control(pDX, IDC_SubFunction9Button, m_SubFunction9Button);
	DDX_Control(pDX, IDC_uCAdjust, m_uCAdjust);
	DDX_Control(pDX, IDC_uCMaximum, m_uCMaximum);
	DDX_Control(pDX, IDC_uCTemps, m_ucTemps);
}


BEGIN_MESSAGE_MAP(CSystemTemperaturesDialog, CDialog)
	//{{AFX_MSG_MAP(CSystemTemperaturesDialog)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
	ON_BN_CLICKED(IDC_Function5Button, OnFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction1Button, OnSubFunction1Button)
	ON_BN_CLICKED(IDC_SubFunction2Button, OnSubFunction2Button)
	ON_BN_CLICKED(IDC_SubFunction3Button, OnSubFunction3Button)
	ON_BN_CLICKED(IDC_SubFunction3Button3, OnSubFunction3Button3)
	ON_BN_CLICKED(IDC_SubFunction4Button, OnSubFunction4Button)
	ON_BN_CLICKED(IDC_SubFunction5Button, OnSubFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction6Button, OnSubFunction6Button)
	ON_BN_CLICKED(IDC_SubFunction7Button, OnSubFunction7Button)
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SubFunction9Button, OnSubFunction9Button)
	ON_BN_CLICKED(IDC_SubFunction10Button, OnSubFunction10Button)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_SubFunction11Button, &CSystemTemperaturesDialog::OnSubfunction11button)
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CSystemTemperaturesDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemTemperaturesDialog message handlers

void CSystemTemperaturesDialog::OnFunction1Button() 
{
	// Main Menu Button was pressed
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::EndDialog(10);
	
}

void CSystemTemperaturesDialog::OnFunction2Button() 
{
	// Save and Exit Button was pressed
	if ((!vChangeMade) || (KeepSetupChanges()))
	{
		vLocalConfigurationData->vTemperatureSurfaceAdjust = vTemperatureSurfaceAdjust;
		vLocalConfigurationData->vTemperatureAirOutsideAdjust = vTemperatureAirOutsideAdjust;
		vLocalConfigurationData->vTemperatureAirInsideAdjust = vTemperatureAirInsideAdjust;
		vLocalConfigurationData->vTemperatureLiquidAdjust = vTemperatureLiquidAdjust;
		vLocalConfigurationData->vTemperatureuCAdjust = vTemperatureuCAdjust;
		vLocalConfigurationData->vMaximumMonoBlockTemperature = vMaximumMonoBlockTemperature;
		vLocalConfigurationData->vMaximumAirTemperature = vMaximumAirTemperature;
		vLocalConfigurationData->vMaximumLiquidTemperature = vMaximumLiquidTemperature;
		vLocalConfigurationData->vMaximumSurfaceTemperature = vMaximumSurfaceTemperature;
		vLocalConfigurationData->vMaximumTemperatureAirOutside = vMaximumTemperatureAirOutside;
		vLocalConfigurationData->vMaximumuCTemperature = vMaximumuCTemperature;
		vLocalConfigurationData->vMaximumCPUTemperature = vMaximumCPUTemperature;
		vLocalConfigurationData->vHasOutsideAirTemp = vHasOutsideAirTemp;
		vLocalConfigurationData->vDeltaTemperatureForFilterCloggedError = vDeltaTemperatureForFilterCloggedError;
		vLocalConfigurationData->vDeltaTemperatureForCoolantProblem = vDeltaTemperatureForCoolantProblem;
		vLocalConfigurationData->vSurfaceTemperatureMovedToDXM = vSurfaceTemperatureMovedToDXM;
		CDialog::EndDialog(true);
	}
	
}

void CSystemTemperaturesDialog::OnFunction3Button() 
{
	if ( !vHasOutsideAirTemp || vGlobalFPGAVersion10Point0OrHigher)
	{
		CNumericEntryDialog INumericEntryDialog;  
		
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vDeltaTemperatureForFilterCloggedError,0);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Outside Air Temp to Inside Temp delta";
		INumericEntryDialog.m_DialogTitleStaticText2 = "To Give a Filter Clogged Error Message";
		INumericEntryDialog.m_DialogTitleStaticText3 = "If Inside Air is hotter than Outside by more than:";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Default: 5, Original Value: " + INumericEntryDialog.vEditString;

		INumericEntryDialog.m_UnitsString = "Measured in Degrees C, 0 = Disabled (AC Units)";
		INumericEntryDialog.vMaxValue = 60;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vHasOutsideAirTemp = !vHasOutsideAirTemp;
			vDeltaTemperatureForFilterCloggedError = _wtoi(INumericEntryDialog.vEditString);
			SetChangeMade();
			UpdateDisplay();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			CDialog::EndDialog(10);
		}	
	}
	else
	{
		vHasOutsideAirTemp = !vHasOutsideAirTemp;
		SetChangeMade();
		UpdateDisplay();
	}
}

void CSystemTemperaturesDialog::OnFunction4Button() 
{
	//Cancel was pressed
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::OnCancel();
	
}

void CSystemTemperaturesDialog::OnFunction5Button() 
{
	if ((vLocalConfigurationData->vHVPSType != cCheyneyMonoBlock) && (vLocalConfigurationData->vHVPSType != c210WMonoBlockHVPS) && 
	(vLocalConfigurationData->vHVPSType != c100WMonoBlockHVPS) && (vLocalConfigurationData->vHVPSType != cMNXHVPS)) //MNX and 210W MonoBlock dont have coolant or MonoBlock Temp
	{
		CNumericEntryDialog INumericEntryDialog;  
		INumericEntryDialog.vEditString = dtoa(vDeltaTemperatureForCoolantProblem,0);
		INumericEntryDialog.m_DialogTitleStaticText1 = "Inside Air Temp to Coolant Temp delta";
		INumericEntryDialog.m_DialogTitleStaticText2 = "To Give a Coolant Problem Error Message";
		INumericEntryDialog.m_DialogTitleStaticText3 = "If Coolant is hotter than Inside by more than:";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Default: 12, 19 with AC, Original Value: " + INumericEntryDialog.vEditString;

		INumericEntryDialog.m_UnitsString = "Measured in Degrees C, 0 = Disabled";
		INumericEntryDialog.vMaxValue = 60;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vDeltaTemperatureForCoolantProblem = _wtoi(INumericEntryDialog.vEditString);
			SetChangeMade();
			UpdateDisplay();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			CDialog::EndDialog(10);
		}
	}
}

void CSystemTemperaturesDialog::OnSubFunction1Button() 
{
	if (vSubFunction1ButtonEnable == true)
	{
	if (vGlobalFPGAVersion10Point0OrHigher)
	{
		// Maximum Outside Air Temperature
		CNumericEntryDialog INumericEntryDialog;  
		INumericEntryDialog.m_DialogTitleStaticText1 = "";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 40`C";
		INumericEntryDialog.m_UnitsString = "Measured in degrees C";
		INumericEntryDialog.vIntegerOnly = true;
		INumericEntryDialog.vAllowNegative = false;
		
		INumericEntryDialog.vEditString = dtoa(vMaximumTemperatureAirOutside, 0);
		INumericEntryDialog.m_DialogTitleStaticText2 = "Maximum Outside Air Temperature before shut down";
		if (vHasOutsideAirTemp)
			INumericEntryDialog.m_DialogTitleStaticText2 = "Maximum Outside Air Temperature before shut down";

		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
		INumericEntryDialog.vMaxValue = 50;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vMaximumTemperatureAirOutside = _wtoi(INumericEntryDialog.vEditString);
			UpdateDisplay();
			SetChangeMade();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			CDialog::EndDialog(10);
		}
	}
	else 
	if (vHasOutsideAirTemp)
	{
		// Maximum Outside Air Temperature
		CNumericEntryDialog INumericEntryDialog;  
		INumericEntryDialog.m_DialogTitleStaticText2 = "";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 40`C";
		INumericEntryDialog.m_UnitsString = "Measured in degrees C";
		INumericEntryDialog.vIntegerOnly = true;
		INumericEntryDialog.vAllowNegative = false;
		
		INumericEntryDialog.vEditString = dtoa(vMaximumSurfaceTemperature,0);
		INumericEntryDialog.m_DialogTitleStaticText1 = "Maximum Outside Air Temperature before shut down";

		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
		INumericEntryDialog.vMaxValue = 50;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vMaximumSurfaceTemperature = _wtoi(INumericEntryDialog.vEditString);
			UpdateDisplay();
			SetChangeMade();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			CDialog::EndDialog(10);
		}
	}
	}
	else		//vHasOutsideAirTemp == false
	{
		CNoticeDialog TempNoticeDialog;
		CString TempText("");
		TempText = ("Outside Air Temperature Not Available");
		TempNoticeDialog.vNoticeText = "\n\n\nOutside Air Temperature Not Available";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}
}

void CSystemTemperaturesDialog::OnSubFunction2Button() 
{
	// Maximum (inside) Air Temperature
	CNumericEntryDialog INumericEntryDialog;  
	INumericEntryDialog.m_DialogTitleStaticText2 = "";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 45`C";
	INumericEntryDialog.m_UnitsString = "Measured in degrees C";
	INumericEntryDialog.vIntegerOnly = true;
	INumericEntryDialog.vAllowNegative = false;

	INumericEntryDialog.vEditString = dtoa(vMaximumAirTemperature, 0);
	INumericEntryDialog.m_DialogTitleStaticText1 = "Maximum Inside Air Temperature before shut down";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
	INumericEntryDialog.vMaxValue = 50;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vMaximumAirTemperature = _wtoi(INumericEntryDialog.vEditString);
		UpdateDisplay();
		SetChangeMade();
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		CDialog::EndDialog(10);
	}
}

void CSystemTemperaturesDialog::OnSubFunction3Button() 
{ 
	if (vSubFunction3ButtonEnable == true)
	{
	if (vGlobalFPGAVersion10Point0OrHigher || (!vHasOutsideAirTemp))
	{
		// Maximum Surface Temperature
		CNumericEntryDialog INumericEntryDialog;  
		INumericEntryDialog.m_DialogTitleStaticText2 = "";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 50`C";
		INumericEntryDialog.m_UnitsString = "Measured in degrees C";
		INumericEntryDialog.vIntegerOnly = true;
		INumericEntryDialog.vAllowNegative = false;
		
		INumericEntryDialog.vEditString = dtoa(vMaximumSurfaceTemperature, 0);
		INumericEntryDialog.m_DialogTitleStaticText1 = "Maximum Surface temperature before shut down";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
		INumericEntryDialog.vMaxValue = 70;
		if (vGlobalFPGAVersion10Point0OrHigher)
		if (vSurfaceTemperatureMovedToDXM)
		{
			INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 45`C";
			INumericEntryDialog.m_DialogTitleStaticText1 = "Maximum DXM temperature before shut down";
		}
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vMaximumSurfaceTemperature = _wtoi(INumericEntryDialog.vEditString);
			UpdateDisplay();
			SetChangeMade();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			CDialog::EndDialog(10);
		}
	}
	}
	else		//vHasOutsideAirTemp == false
	{
		CNoticeDialog TempNoticeDialog;
		CString TempText("");
		TempText = ("Surface Temperature Not Available");
		TempNoticeDialog.vNoticeText = "\n\n\nSurface Temperature Not Available";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}
}

void CSystemTemperaturesDialog::OnSubFunction3Button3() 
{ 
	if (vGlobalFPGAVersion10Point0OrHigher)
	{
		vSurfaceTemperatureMovedToDXM = !vSurfaceTemperatureMovedToDXM;

		if (vSurfaceTemperatureMovedToDXM)
			vMaximumSurfaceTemperature = 45;
		else
			vMaximumSurfaceTemperature = 50;

		UpdateDisplay();
		SetChangeMade();
	}
}

void CSystemTemperaturesDialog::OnSubFunction4Button() 
{
	// Maximum Coolant Temperature
	if ((vLocalConfigurationData->vHVPSType != cCheyneyMonoBlock) && (vLocalConfigurationData->vHVPSType != c210WMonoBlockHVPS) && 
		(vLocalConfigurationData->vHVPSType != c100WMonoBlockHVPS) && (vLocalConfigurationData->vHVPSType != cMNXHVPS)) //MNX and 210W MonoBlock dont have coolant or MonoBlock Temp
	{
		CNumericEntryDialog INumericEntryDialog;  
		INumericEntryDialog.m_DialogTitleStaticText2 = "";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 50`C";
		INumericEntryDialog.m_UnitsString = "Measured in degrees C";
		INumericEntryDialog.vIntegerOnly = true;
		INumericEntryDialog.vAllowNegative = false;
	
		INumericEntryDialog.vEditString = dtoa(vMaximumLiquidTemperature, 0);
		INumericEntryDialog.m_DialogTitleStaticText1 = "Maximum Coolant temperature before shut down";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
		INumericEntryDialog.m_UnitsString = "In Degrees Celsius";
		INumericEntryDialog.vIntegerOnly = true;
		INumericEntryDialog.vMaxValue = 55;
		INumericEntryDialog.vAllowNegative = false;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vMaximumLiquidTemperature = _wtoi(INumericEntryDialog.vEditString);
			UpdateDisplay();
			SetChangeMade();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			CDialog::EndDialog(10);
		}
	}
}

void CSystemTemperaturesDialog::OnSubFunction5Button() 
{
	if (vSubFunction5ButtonEnable == true)
	{
	if (vGlobalFPGAVersion10Point0OrHigher)
	{
			// Temperature Monitor Adjust
		CNumericEntryDialog INumericEntryDialog;  
		INumericEntryDialog.m_DialogTitleStaticText1 = "";
		INumericEntryDialog.m_UnitsString = "Measured in degrees C";
		INumericEntryDialog.vMaxValue = 50;
		INumericEntryDialog.vIntegerOnly = false;
		INumericEntryDialog.vAllowNegative = true;

		INumericEntryDialog.vEditString = dtoa(vTemperatureAirOutsideAdjust,2);
		INumericEntryDialog.m_DialogTitleStaticText2 = "Outside Air Temperature Thermistor Output Offset Adjust";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 0";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vTemperatureAirOutsideAdjust = ATOF(INumericEntryDialog.vEditString);
			UpdateDisplay();
			SetChangeMade();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			CDialog::EndDialog(10);
		}
	}
	else
	{
		if (vHasOutsideAirTemp)
		{
					// Temperature Monitor Adjust
			CNumericEntryDialog INumericEntryDialog;  
			INumericEntryDialog.m_DialogTitleStaticText1 = "";
			INumericEntryDialog.m_UnitsString = "Measured in degrees C";
			INumericEntryDialog.vMaxValue = 50;
			INumericEntryDialog.vIntegerOnly = false;
			INumericEntryDialog.vAllowNegative = true;

			INumericEntryDialog.vEditString = dtoa(vTemperatureAirOutsideAdjust,2);
			INumericEntryDialog.m_DialogTitleStaticText2 = "Outside Air Temperature Thermistor Output Offset Adjust";
			INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 0";
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vTemperatureSurfaceAdjust = ATOF(INumericEntryDialog.vEditString);
				UpdateDisplay();
				SetChangeMade();
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				CDialog::EndDialog(10);
			}
		}
	}
	}
	else		//vHasOutsideAirTemp == false
	{
		CNoticeDialog TempNoticeDialog;
		CString TempText("");
		TempText = ("Outside Air Temperature Not Available");
		TempNoticeDialog.vNoticeText = "\n\n\nOutside Air Temperature Not Available";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}
}

void CSystemTemperaturesDialog::OnSubFunction6Button() 
{
		// Temperature Monitor Adjust
	CNumericEntryDialog INumericEntryDialog;  
	INumericEntryDialog.m_DialogTitleStaticText1 = "";
	INumericEntryDialog.m_UnitsString = "Measured in degrees C";
	INumericEntryDialog.vMaxValue = 50;
	INumericEntryDialog.vIntegerOnly = false;
	INumericEntryDialog.vAllowNegative = true;
	
	INumericEntryDialog.vEditString = dtoa(vTemperatureAirInsideAdjust,2);
	INumericEntryDialog.m_DialogTitleStaticText2 = "Inside Air Thermistor Output Offset Adjust";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 0";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
		INumericEntryDialog.vEditString;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vTemperatureAirInsideAdjust = ATOF(INumericEntryDialog.vEditString);
		UpdateDisplay();
		SetChangeMade();
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		CDialog::EndDialog(10);
	}
}

void CSystemTemperaturesDialog::OnSubFunction7Button() 
{
	if (vSubFunction7ButtonEnable == true)
	{
	if (vGlobalFPGAVersion10Point0OrHigher || !vHasOutsideAirTemp)
	{
		// Surface temp Temperature Monitor Adjust
		CNumericEntryDialog INumericEntryDialog;  
		INumericEntryDialog.m_DialogTitleStaticText1 = "";
		INumericEntryDialog.m_UnitsString = "Measured in degrees C";
		INumericEntryDialog.vMaxValue = 50;
		INumericEntryDialog.vIntegerOnly = false;
		INumericEntryDialog.vAllowNegative = true;
		
		INumericEntryDialog.vEditString = dtoa(vTemperatureSurfaceAdjust,2);
		INumericEntryDialog.m_DialogTitleStaticText2 = "X-Ray Source Surface Thermistor Output Offset Adjust";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 0";
		if (vLocalConfigurationData->vHasOutsideAirTemp)
			INumericEntryDialog.m_DialogTitleStaticText3 = "Surface Thermistor Output Offset Adjust";

		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
			INumericEntryDialog.vEditString;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vTemperatureSurfaceAdjust = ATOF(INumericEntryDialog.vEditString);
			UpdateDisplay();
			SetChangeMade();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			CDialog::EndDialog(10);
		}	
	}
	}
	else		//vHasOutsideAirTemp == false
	{
		CNoticeDialog TempNoticeDialog;
		CString TempText("");
		TempText = ("Surface Temperature Not Available");
		TempNoticeDialog.vNoticeText = "\n\n\nSurface Temperature Not Available";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}

}

void CSystemTemperaturesDialog::OnSubFunction8Button() 
{
	// Temperature Monitor Adjust
	if ((vLocalConfigurationData->vHVPSType != cCheyneyMonoBlock) && (vLocalConfigurationData->vHVPSType != c210WMonoBlockHVPS) && 
		(vLocalConfigurationData->vHVPSType != c100WMonoBlockHVPS) && (vLocalConfigurationData->vHVPSType != cMNXHVPS)) //MNX and 210W MonoBlock dont have coolant or MonoBlock Temp
	{
		CNumericEntryDialog INumericEntryDialog;  
		INumericEntryDialog.m_DialogTitleStaticText1 = "";
		INumericEntryDialog.m_UnitsString = "Measured in degrees C";
		INumericEntryDialog.vIntegerOnly = false;
		INumericEntryDialog.vAllowNegative = true;
	
	
		INumericEntryDialog.vEditString = dtoa(vTemperatureLiquidAdjust,2);
		INumericEntryDialog.m_DialogTitleStaticText2 = "X-Ray Source Coolant Thermistor Output Offset Adjust";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 0";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
			INumericEntryDialog.vEditString;
		INumericEntryDialog.m_UnitsString = "Measured in degrees C";
		INumericEntryDialog.vMaxValue = 50;
		INumericEntryDialog.vIntegerOnly = false;
		INumericEntryDialog.vAllowNegative = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vTemperatureLiquidAdjust = 
				ATOF(INumericEntryDialog.vEditString);
			UpdateDisplay();
			SetChangeMade();

	/*		INumericEntryDialog.vEditString = dtoa(vTemperatureSurfaceAdjust,2);
			INumericEntryDialog.m_DialogTitleStaticText3 = "X-Ray Source Surface Thermistor Output Offset Adjust";
			if (vLocalConfigurationData->vHasOutsideAirTemp)
				INumericEntryDialog.m_DialogTitleStaticText3 = "Outside Air Thermistor Output Offset Adjust";

			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
				INumericEntryDialog.vEditString;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vTemperatureSurfaceAdjust = 
					ATOF(INumericEntryDialog.vEditString);
				UpdateDisplay();
				SetChangeMade();
			
				INumericEntryDialog.vEditString = dtoa(vTemperatureAirInsideAdjust,2);
				INumericEntryDialog.m_DialogTitleStaticText3 = "Inside Air Thermistor Output Offset Adjust";
				INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
					INumericEntryDialog.vEditString;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vTemperatureAirInsideAdjust = 
						ATOF(INumericEntryDialog.vEditString);
					UpdateDisplay();
					SetChangeMade();

					if (vGlobalFPGAVersion10Point0OrHigher)
					{
						INumericEntryDialog.vEditString = dtoa(vTemperatureAirOutsideAdjust,2);
						INumericEntryDialog.m_DialogTitleStaticText3 = "Outside Air Temperature Thermistor Output Offset Adjust";
						INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
						//Pass control to dialog box and display
						int nResponse = INumericEntryDialog.DoModal();
						//dialog box is now closed, if user pressed select do this
						//if user pressed cancel, do nothing
						if (nResponse == IDOK)
						{
							vTemperatureAirOutsideAdjust = ATOF(INumericEntryDialog.vEditString);
							UpdateDisplay();
							SetChangeMade();
						}
						else 
						if (nResponse == 10)
						{
							//Main Menu button pressed
							CDialog::EndDialog(10);
						}
					}
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					CDialog::EndDialog(10);
				}
		
			if (nResponse == 10)
			{
				//Main Menu button pressed
				CDialog::EndDialog(10);
			}
			*/
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			CDialog::EndDialog(10);
		}
	}
}

BOOL CSystemTemperaturesDialog::PreTranslateMessage(MSG* pMsg) 
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
			case 35: //left/right right/left left right for dual beam End Button cEnd button  Image button
				OnSubFunction9Button();
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
				return true;
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
					cShowHelp, 32, 0);
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
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
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CSystemTemperaturesDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
		pDC->SetAttribDC(pDC->m_hDC);
	
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function3Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Function3Display, 5);
	if ((!vHasOutsideAirTemp && !vGlobalFPGAVersion10Point0OrHigher) 
		|| (vDeltaTemperatureForFilterCloggedError < 3) || (vDeltaTemperatureForFilterCloggedError > 20))
	if (vDeltaTemperatureForFilterCloggedError) //disabled for AC is OK
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
	}
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function5Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Function5Display, 5);
	if ((vDeltaTemperatureForCoolantProblem < 3) || (vDeltaTemperatureForCoolantProblem > 25))
	if (vDeltaTemperatureForCoolantProblem)//disabled for AC or TD OK
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
	}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5);
		if ((vGlobalFPGAVersion10Point0OrHigher && (vMaximumTemperatureAirOutside != 40)) || 
			(!vGlobalFPGAVersion10Point0OrHigher && (!vHasOutsideAirTemp) && (vMaximumSurfaceTemperature != 40)))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Display, 5);
	if (vMaximumAirTemperature != 45)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
	}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Display, 5);
		bool TempNotDefault = false;

		if (vSurfaceTemperatureMovedToDXM)
		{
			if (vMaximumSurfaceTemperature != 45)
				TempNotDefault = true;
		}
		else
		if (vGlobalFPGAVersion10Point0OrHigher)
		{
			if (vMaximumSurfaceTemperature != 50)
				TempNotDefault = true;
		}
		else
		{
			if (!vHasOutsideAirTemp)
			{
			if (vMaximumSurfaceTemperature != 50)
				TempNotDefault = true;
			}
		}
		if (TempNotDefault)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Display, 5);
		if (vMaximumLiquidTemperature != 50)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Display, 5);
	if ( ( vGlobalFPGAVersion10Point0OrHigher && (vTemperatureAirOutsideAdjust != 0) )
		|| ( !vGlobalFPGAVersion10Point0OrHigher && (( !vHasOutsideAirTemp ) || (vTemperatureSurfaceAdjust != 0)) ) )
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
	}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Display, 5);
	if (vTemperatureAirInsideAdjust != 0)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
	}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Display, 5);
	if ((vTemperatureSurfaceAdjust != 0) || ( !vGlobalFPGAVersion10Point0OrHigher && vHasOutsideAirTemp ))
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
	}
	}

	if (pWnd->GetDlgCtrlID() == IDC_MonoBlockMaximumDisplay)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MonoBlockMaximumDisplay, 5);
	if (vMaximumMonoBlockTemperature != 60)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
	}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Display, 5);
	if (vTemperatureLiquidAdjust != 0)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
	}
	}
	if (pWnd->GetDlgCtrlID() == IDC_uCAdjust) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_uCAdjust, 5);
	if (vTemperatureuCAdjust)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
	}
	}
	if (pWnd->GetDlgCtrlID() == IDC_uCMaximum)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_uCMaximum, 5);
	if ( vMaximumuCTemperature != 60)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
	}
	}
	if (pWnd->GetDlgCtrlID() == IDC_CPUMaximum)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CPUMaximum, 5);
		if ((vMaximumCPUTemperature > 66) || (vMaximumCPUTemperature < 62))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background) 
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	//if (pWnd->GetDlgCtrlID() == IDC_VoltageLabel)
	//{
	//	TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_VoltageLabel, 5);
	//	pDC->SetBkMode(TRANSPARENT);
	//	return vLocalSystemData->vLightGreenBrush;
	//}
	//if (pWnd->GetDlgCtrlID() == IDC_CurrentLabel) 
	//{
	//	TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_CurrentLabel, 5);
	//	pDC->SetBkMode(TRANSPARENT);
	//	return vLocalSystemData->vLightGreenBrush;
	//}
	if (pWnd->GetDlgCtrlID() == IDC_MaximumTitle) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaximumTitle, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MonoBlockTitle) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MonoBlockTitle, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Maximum) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Maximum, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Adjust)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Adjust, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_uCTemps)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ucTemps, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_CPUTemps) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CPUTemps, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_CPUTempMaximum) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CPUTempMaximum, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_Function1Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function2Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);  //5 is medium large
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
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction1ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
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
		if (vSubFunction3ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button3)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button3, 5);  //5 is medium large
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
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction5ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
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
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction9Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction9Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction10Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction10Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction11Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction11Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	
	return hbr;
}

void CSystemTemperaturesDialog::UpdateDisplay() 
{
	if ((vLocalConfigurationData->vHVPSType == c100WMonoBlockHVPS) || (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock))
		SetDlgItemText(IDC_MonoBlockMaximumDisplay, dtoa(vMaximumMonoBlockTemperature, 0) + "`C");

		if (vGlobalFPGAVersion10Point0OrHigher)
	{
		if (vDeltaTemperatureForFilterCloggedError)
			SetDlgItemText(IDC_Function3Display,"Inside / Outside Air Maximum Delta: " +
				dtoa(vDeltaTemperatureForFilterCloggedError,0) + "`C");
		else
			SetDlgItemText(IDC_Function3Display, _T("In/Outside Air Temp Delta is Disabled - AC Unit"));

		if (vSurfaceTemperatureMovedToDXM)
		{
			SetDlgItemText(IDC_SubFunction3Button, _T("Maximum DXM\nTemp"));
			SetDlgItemText(IDC_SubFunction3Button3, _T("Convert DXM to Surface"));
		}
		else
		{
			SetDlgItemText(IDC_SubFunction3Button, _T("Maximum Surface Temp"));
			SetDlgItemText(IDC_SubFunction3Button3, _T("Convert Surface to DXM"));
		}
	}
	else
	{
		if (vHasOutsideAirTemp)
		{
			SetDlgItemText(IDC_Function3Display,"Inside / Outside Air Maximum Delta: " +
				dtoa(vDeltaTemperatureForFilterCloggedError,0) + "`C");
			SetDlgItemText(IDC_Function3Button, _T("Has Surface Temp"));
		}
		else
		{
			SetDlgItemText(IDC_Function3Display, _T("Has Surface Temp"));
			SetDlgItemText(IDC_Function3Button, _T("Has Outside Air Temp"));
		}
	}
	if (vDeltaTemperatureForCoolantProblem)
		SetDlgItemText(IDC_Function5Display,"Inside Air / Coolant Maximum Delta: " +
			dtoa(vDeltaTemperatureForCoolantProblem,0) + "`C");
	else
		SetDlgItemText(IDC_Function5Display, _T("Inside Air / Coolant Maximum Delta is Disabled"));

	SetDlgItemText(IDC_uCMaximum, dtoa(vMaximumuCTemperature));
	SetDlgItemText(IDC_uCAdjust, dtoa(vTemperatureuCAdjust));
	SetDlgItemText(IDC_CPUMaximum, dtoa(vMaximumCPUTemperature));

	if (vGlobalFPGAVersion10Point0OrHigher)
	{
		SetDlgItemText(IDC_SubFunction1Display,dtoa(vMaximumTemperatureAirOutside,0) + "`C");
		SetDlgItemText(IDC_SubFunction3Display,dtoa(vMaximumSurfaceTemperature,0) + "`C");
	}
	else
	{
		if (vHasOutsideAirTemp)
		{
			SetDlgItemText(IDC_SubFunction1Display, dtoa(vMaximumSurfaceTemperature,0) + "`C");
			vSubFunction1ButtonEnable = true;	//m_SubFunction1Button.EnableWindow(true);
			SetDlgItemText(IDC_SubFunction3Display, _T("Not Installed"));
			vSubFunction3ButtonEnable = false;	//m_SubFunction3Button.EnableWindow(false);
		}
		else
		{
			SetDlgItemText(IDC_SubFunction1Display, _T("Not Installed"));
			vSubFunction1ButtonEnable = false;	//m_SubFunction1Button.EnableWindow(false);
			SetDlgItemText(IDC_SubFunction3Display, dtoa(vMaximumSurfaceTemperature,0) + "`C");
			vSubFunction3ButtonEnable = true;	//m_SubFunction3Button.EnableWindow(true);
		}
	}
	SetDlgItemText(IDC_SubFunction2Display,dtoa(vMaximumAirTemperature,0) + "`C");
	SetDlgItemText(IDC_SubFunction4Display,dtoa(vMaximumLiquidTemperature,0) + "`C");

	SetDlgItemText(IDC_SubFunction8Display,  dtoa(vTemperatureLiquidAdjust,2) + "`C");
	SetDlgItemText(IDC_SubFunction6Display,  dtoa(vTemperatureAirInsideAdjust,2) + "`C" );
	if (vGlobalFPGAVersion10Point0OrHigher)
	{
		SetDlgItemText(IDC_SubFunction5Display, dtoa(vTemperatureAirOutsideAdjust,2) + "`C");
		SetDlgItemText(IDC_SubFunction7Display, dtoa(vTemperatureSurfaceAdjust,2) + "`C");
	}
	else
	{
		if (vHasOutsideAirTemp)
		{
			SetDlgItemText(IDC_SubFunction5Display, dtoa(vTemperatureSurfaceAdjust,2)  + "`C");
			vSubFunction5ButtonEnable = true;	//m_SubFunction5Button.EnableWindow(true);
			SetDlgItemText(IDC_SubFunction7Display, _T("Not Installed"));
			vSubFunction7ButtonEnable = false;	//m_SubFunction7Button.EnableWindow(false);
		}
		else
		{
			SetDlgItemText(IDC_SubFunction5Display, _T("Not Installed"));
			vSubFunction5ButtonEnable = false;	//m_SubFunction5Button.EnableWindow(false);
			SetDlgItemText(IDC_SubFunction7Display, dtoa(vTemperatureSurfaceAdjust,2) + "`C");
			vSubFunction7ButtonEnable = true;	//m_SubFunction7Button.EnableWindow(true);
		}
	}

	//CString TempString = dtoa(vVoltageControlAdjustOffset,2);
	//CString TempString1 = dtoa(vVoltageControlAdjustGain,4);
	//TempString = TempString1 + "X + " + TempString;
	//SetDlgItemText(IDC_SubFunction1Display,TempString);

	//TempString = dtoa(vCurrentControlAdjustOffset,2);
	//TempString1 = dtoa(vCurrentControlAdjustGain,4);
	//TempString = TempString1 + "X + " + TempString;
	//SetDlgItemText(IDC_SubFunction2Display,TempString);
}

void CSystemTemperaturesDialog::SetChangeMade() 
{
	if (!vChangeMade)
	{
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_Function4Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_Function2Button, _T("Save\nand Exit"));
		SetDlgItemText(IDC_Function4Button, _T("Cancel"));
		vChangeMade = true;
	}
}

void CSystemTemperaturesDialog::OnSubFunction9Button() 
{
	CNumericEntryDialog INumericEntryDialog;  
		
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vMaximumuCTemperature,2);
	INumericEntryDialog.m_DialogTitleStaticText1 = "Maximum uC temperature before shut down";
	INumericEntryDialog.m_DialogTitleStaticText2 = "";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 60`C";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
		INumericEntryDialog.vEditString;
	INumericEntryDialog.m_UnitsString = "Measured in degrees C";
	INumericEntryDialog.vMaxValue = 70;
	INumericEntryDialog.vIntegerOnly = true;
	INumericEntryDialog.vAllowNegative = false;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vMaximumuCTemperature = _wtoi(INumericEntryDialog.vEditString);
		INumericEntryDialog.vEditString = dtoa(vTemperatureuCAdjust,2);
		INumericEntryDialog.m_DialogTitleStaticText1 = "uController Thermistor Output Adjust";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 0`C";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
		INumericEntryDialog.vMaxValue = 50;
		INumericEntryDialog.vAllowNegative = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vTemperatureuCAdjust = _wtoi(INumericEntryDialog.vEditString);
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		CDialog::EndDialog(10);
	}
	
	SetChangeMade();
	UpdateDisplay();

	CWnd * TempWindow = NULL;
	TempWindow = CWnd::GetDlgItem(IDC_uCAdjust);
	if (TempWindow)
		TempWindow->Invalidate(false);
	TempWindow = CWnd::GetDlgItem(IDC_uCMaximum);
	if (TempWindow)
		TempWindow->Invalidate(false);
}

// data sheet specifing maximum temperature for Intel Core i7-2600K 3.40GHz 8MB Cache, LGA 1155, 95W
//  http://ark.intel.com/products/52214/Intel-Core-i7-2600K-Processor-(8M-Cache-up-to-3_80-GHz)
// max temperature 72.6C
void CSystemTemperaturesDialog::OnSubFunction10Button() 
{
	CNumericEntryDialog INumericEntryDialog;  
		
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vMaximumCPUTemperature,2);
	INumericEntryDialog.m_DialogTitleStaticText1 = "Maximum CPU temperature before Shut Down X-Rays";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Intel i7 Max CPU temperature 72.6`C, ASUS CPU Slows at 66";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Giga CPU Slows at 50, Default Value: Giga 62`C ASUS 66`C";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
	INumericEntryDialog.m_UnitsString = "Measured in degrees C";
	INumericEntryDialog.vMaxValue = 80;
	INumericEntryDialog.vMinValue = 40;		
	INumericEntryDialog.vIntegerOnly = true;
	INumericEntryDialog.vAllowNegative = false;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vMaximumCPUTemperature = _wtoi(INumericEntryDialog.vEditString);
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		CDialog::EndDialog(10);
	}
	
	SetChangeMade();
	UpdateDisplay();

	CWnd * TempWindow = NULL;
	TempWindow = CWnd::GetDlgItem(IDC_CPUMaximum);
	if (TempWindow)
		TempWindow->Invalidate(false);
}





void CSystemTemperaturesDialog::OnSubfunction11button()
{
	if ((vLocalConfigurationData->vHVPSType == c100WMonoBlockHVPS) || (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock))
	{
		CNumericEntryDialog INumericEntryDialog;  
		
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vMaximumMonoBlockTemperature,2);
		INumericEntryDialog.m_DialogTitleStaticText1 = "Maximum MonoBlock temperature before Shut Down X-Rays";
		INumericEntryDialog.m_DialogTitleStaticText2 = "MonoBlock will shut down itself at 65`C";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 60`C";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
		INumericEntryDialog.m_UnitsString = "Measured in degrees C";
		INumericEntryDialog.vMaxValue = 80;
		INumericEntryDialog.vMinValue = 40;		
		INumericEntryDialog.vIntegerOnly = true;
		INumericEntryDialog.vAllowNegative = false;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vMaximumMonoBlockTemperature = _wtoi(INumericEntryDialog.vEditString);
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			CDialog::EndDialog(10);
		}
	
		SetChangeMade();
		UpdateDisplay();

		CWnd * TempWindow = NULL;
		TempWindow = CWnd::GetDlgItem(IDC_MonoBlockMaximumDisplay);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}
}


void CSystemTemperaturesDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
