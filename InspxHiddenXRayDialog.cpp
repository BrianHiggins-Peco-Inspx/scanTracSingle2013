//ScanTrac Side View Source File
// InspxHiddenXRayDialog.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "InspxHiddenXRayDialog.h"
#include "NumericEntryDialog.h"
#include "SpellmanDXMXRaySourcePort.h"
#include "XRaySource.h"
#include "SpellmanDXMXRaySourcePort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInspxHiddenXRayDialog dialog


extern CSpellmanDXMXRaySourcePort *vGlobalDXMXRayPort;
extern bool vGlobalFPGAVersion10Point0OrHigher;
extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;

CInspxHiddenXRayDialog::CInspxHiddenXRayDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CInspxHiddenXRayDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInspxHiddenXRayDialog)
	//}}AFX_DATA_INIT
	vChangeMade = false;
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_SubFunction6Display);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction5Display);
	vLocalCWndCollection.Add(&m_SubFunction3Display);
	vLocalCWndCollection.Add(&m_SubFunction2Display);
	vLocalCWndCollection.Add(&m_SubFunction1Display);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function3Display);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Functio5Display);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction4Display);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction7Display);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction8Display);
}

void CInspxHiddenXRayDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);
	
	// TODO: Add your message handler code here
//	vTemperatureSurfaceAdjust = vLocalConfigurationData->vTemperatureSurfaceAdjust;
//	vTemperatureAirInsideAdjust = vLocalConfigurationData->vTemperatureAirInsideAdjust;
//	vTemperatureAirOutsideAdjust = vLocalConfigurationData->vTemperatureAirOutsideAdjust;
//	vTemperatureLiquidAdjust = vLocalConfigurationData->vTemperatureLiquidAdjust;
//	vMaximumAirTemperature = vLocalConfigurationData->vMaximumAirTemperature;
	//vVoltageControlAdjustOffset = vLocalConfigurationData->vVoltageControlAdjustOffset;
	//vCurrentControlAdjustOffset = vLocalConfigurationData->vCurrentControlAdjustOffset;
	//vVoltageControlAdjustGain = vLocalConfigurationData->vVoltageControlAdjustGain;
	//vCurrentControlAdjustGain = vLocalConfigurationData->vCurrentControlAdjustGain;
	vMaximumVoltage = vLocalConfigurationData->vMaximumVoltage;
	vMaximumCurrent = vLocalConfigurationData->vMaximumCurrent;
	vMaximumPower = vLocalConfigurationData->vMaximumPower;
	vVoltageAdjustOffset = vLocalConfigurationData->vVoltageAdjustOffset;
	vCurrentAdjustOffset = vLocalConfigurationData->vCurrentAdjustOffset;
	vVoltageAdjustGain = vLocalConfigurationData->vVoltageAdjustGain;
	vCurrentAdjustGain = vLocalConfigurationData->vCurrentAdjustGain;
	vUse101VACFor10SecondsShutDownLimit = vLocalConfigurationData->vUse101VACFor10SecondsShutDownLimit;
	vIgnoreCheyneyInterlockStatus = vLocalConfigurationData->vIgnoreCheyneyInterlockStatus;
//	vMaximumLiquidTemperature = vLocalConfigurationData->vMaximumLiquidTemperature;
//	vMaximumSurfaceTemperature = vLocalConfigurationData->vMaximumSurfaceTemperature;
//	vMaximumTemperatureAirOutside = vLocalConfigurationData->vMaximumTemperatureAirOutside;
//	if (vGlobalFPGAVersion10Point0OrHigher)
//		SetDlgItemText(IDC_SubFunction8Button,"Max. Sur. && Outside Air Temp.");
//	else
//	if (vLocalConfigurationData->vHasOutsideAirTemp)
//		SetDlgItemText(IDC_SubFunction8Button,"Max Outside Air Temp.");
	m_SubFunction8Display.ShowWindow(SW_SHOW);
	m_SubFunction8Button.ShowWindow(SW_SHOW);

	if (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock)
	{
		m_SubFunction4Display.ShowWindow(SW_SHOW);
		m_SubFunction4Button.ShowWindow(SW_SHOW);
	}
	else
	{
		m_SubFunction4Display.ShowWindow(SW_HIDE);
		m_SubFunction4Button.ShowWindow(SW_HIDE);
	}
	UpdateDisplay();
	this->SetWindowText(_T("X-Ray System Setup"));
	::SetupMenu(vLocalCWndCollection);
}

void CInspxHiddenXRayDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInspxHiddenXRayDialog)
	DDX_Control(pDX, IDC_SubFunction6Display, m_SubFunction6Display);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction5Display, m_SubFunction5Display);
	DDX_Control(pDX, IDC_SubFunction3Display, m_SubFunction3Display);
	DDX_Control(pDX, IDC_SubFunction2Display, m_SubFunction2Display);
	DDX_Control(pDX, IDC_SubFunction1Display, m_SubFunction1Display);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function3Display, m_Function3Display);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function5Display, m_Functio5Display);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction4Display, m_SubFunction4Display);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction7Display, m_SubFunction7Display);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction8Display, m_SubFunction8Display);
}


BEGIN_MESSAGE_MAP(CInspxHiddenXRayDialog, CDialog)
	//{{AFX_MSG_MAP(CInspxHiddenXRayDialog)
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
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CInspxHiddenXRayDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInspxHiddenXRayDialog message handlers

void CInspxHiddenXRayDialog::OnFunction1Button() 
{
	// Main Menu Button was pressed
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::EndDialog(10);
}

void CInspxHiddenXRayDialog::OnFunction2Button() 
{
	// Save and Exit Button was pressed
	if ((!vChangeMade) || (KeepSetupChanges()))
	{
//		vLocalConfigurationData->vTemperatureSurfaceAdjust = vTemperatureSurfaceAdjust;
//		vLocalConfigurationData->vTemperatureAirOutsideAdjust = vTemperatureAirOutsideAdjust;
//		vLocalConfigurationData->vTemperatureAirInsideAdjust = vTemperatureAirInsideAdjust;
//		vLocalConfigurationData->vTemperatureLiquidAdjust = vTemperatureLiquidAdjust;
//		vLocalConfigurationData->vMaximumAirTemperature = vMaximumAirTemperature;
		//vLocalConfigurationData->vVoltageControlAdjustOffset = vVoltageControlAdjustOffset;
		//vLocalConfigurationData->vCurrentControlAdjustOffset = vCurrentControlAdjustOffset;
		//vLocalConfigurationData->vVoltageControlAdjustGain = vVoltageControlAdjustGain;
		//vLocalConfigurationData->vCurrentControlAdjustGain = vCurrentControlAdjustGain;
		vLocalConfigurationData->vMaximumVoltage = vMaximumVoltage;
		vLocalConfigurationData->vMaximumCurrent = vMaximumCurrent;
		vLocalConfigurationData->vMaximumPower = vMaximumPower;
		vLocalConfigurationData->vVoltageAdjustOffset = vVoltageAdjustOffset;
		vLocalConfigurationData->vCurrentAdjustOffset = vCurrentAdjustOffset;
		vLocalConfigurationData->vVoltageAdjustGain = vVoltageAdjustGain;
		vLocalConfigurationData->vCurrentAdjustGain = vCurrentAdjustGain;
		vLocalConfigurationData->vUse101VACFor10SecondsShutDownLimit = vUse101VACFor10SecondsShutDownLimit;
		vLocalConfigurationData->vIgnoreCheyneyInterlockStatus = vIgnoreCheyneyInterlockStatus;
//		vLocalConfigurationData->vMaximumLiquidTemperature = vMaximumLiquidTemperature;
//		vLocalConfigurationData->vMaximumSurfaceTemperature = vMaximumSurfaceTemperature;
//		vLocalConfigurationData->vMaximumTemperatureAirOutside = vMaximumTemperatureAirOutside;
		CDialog::EndDialog(true);
	}
}

void CInspxHiddenXRayDialog::OnFunction3Button() 
{
}

void CInspxHiddenXRayDialog::OnFunction4Button() 
{
	//Cancel was pressed
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::OnCancel();
}

void CInspxHiddenXRayDialog::OnFunction5Button() 
{
}

void CInspxHiddenXRayDialog::OnSubFunction1Button() 
{

	// Maximum Voltage Allowed
	CNumericEntryDialog INumericEntryDialog;  
	INumericEntryDialog.vEditString = dtoa(vMaximumVoltage,1);
	INumericEntryDialog.m_DialogTitleStaticText1 = "Maximum Voltage Allowed for This ScanTrac";
	INumericEntryDialog.m_UnitsString = "Measured in kV";
	INumericEntryDialog.vIntegerOnly = false;
	INumericEntryDialog.vAllowNegative = false;
	
	INumericEntryDialog.m_DialogTitleStaticText2 = "X-Ray Source Maximum Voltage";


	double TempMaxVoltage = 50;
	switch (vLocalConfigurationData->vHVPSType)
	{
		case cCheyneyMonoBlock:
			TempMaxVoltage = 70;
			if (vGlobalDXMXRayPort)
			if (vGlobalDXMXRayPort->vVoltageMaximumScale)
				TempMaxVoltage = vGlobalDXMXRayPort->vVoltageMaximumScale;
		break;
		case c100WMonoBlockHVPS:
			TempMaxVoltage = 80;
			if (vGlobalDXMXRayPort)
				if (vGlobalDXMXRayPort->vVoltageMaximumScale)
				if (vGlobalDXMXRayPort->vVoltageMaximumScale > 71)
					TempMaxVoltage = vGlobalDXMXRayPort->vVoltageMaximumScale * 0.9;
			break;
		case c210WMonoBlockHVPS: 
		case c100WVJTMonoBlockHVPS:
			TempMaxVoltage = 80;
		break;
		case cDXMHVPS: 
			TempMaxVoltage = 70;
		break;
		case cXLFHVPS: 
			TempMaxVoltage = 60;
		break;
		case cMNXHVPS: 
			TempMaxVoltage = 50;
		break;
	}

	INumericEntryDialog.m_DialogTitleStaticText3 = "Default Voltage = Maximum Voltage = " + dtoa(TempMaxVoltage);
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
	INumericEntryDialog.vMinValue = cMinimumXRayVoltage;
	INumericEntryDialog.vMaxValue = TempMaxVoltage;
				
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vMaximumVoltage = ATOF(INumericEntryDialog.vEditString);
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

void CInspxHiddenXRayDialog::OnSubFunction2Button() 
{
	// Maximum Current Allowed
	CNumericEntryDialog INumericEntryDialog;  

	double TempMaxCurrent = 2;
	switch (vLocalConfigurationData->vHVPSType)
	{
		case cCheyneyMonoBlock:
			TempMaxCurrent = 3;
			if (vGlobalDXMXRayPort->vCurrentMaximumScale)
				TempMaxCurrent = vGlobalDXMXRayPort->vCurrentMaximumScale;
		break;
		case c100WMonoBlockHVPS: 
			TempMaxCurrent = 1.25;
			if (vGlobalDXMXRayPort->vCurrentMaximumScale)
			if (vGlobalDXMXRayPort->vCurrentMaximumScale > 2.05)
				TempMaxCurrent = vGlobalDXMXRayPort->vCurrentMaximumScale * 0.9;
			break;
		case c210WMonoBlockHVPS: 
			TempMaxCurrent = 6;
		break;
		case cDXMHVPS: 
			TempMaxCurrent = 8.55;
		break;
		case cXLFHVPS: 
			TempMaxCurrent = 8;
		break;
		case cMNXHVPS: 
			TempMaxCurrent = 2;
		break;
		case c100WVJTMonoBlockHVPS:
			TempMaxCurrent = 2.5;
		break;
	}

	INumericEntryDialog.vEditString = dtoa(vMaximumCurrent,2);
	INumericEntryDialog.m_DialogTitleStaticText1 = "Maximum Current Allowed for This ScanTrac";
	INumericEntryDialog.m_DialogTitleStaticText2 = "X-Ray Source Maximum Current";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Default Current = Maximum Current = " + dtoa(TempMaxCurrent);
	INumericEntryDialog.m_UnitsString = "Measured in mA";
	INumericEntryDialog.vIntegerOnly = false;
	INumericEntryDialog.vAllowNegative = false;
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
	INumericEntryDialog.vMaxValue = TempMaxCurrent;

	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vMaximumCurrent = ATOF(INumericEntryDialog.vEditString);
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

void CInspxHiddenXRayDialog::OnSubFunction3Button() 
{
	// Maximum Power Allowed
	CNumericEntryDialog INumericEntryDialog;  
	INumericEntryDialog.m_DialogTitleStaticText1 = "";
	INumericEntryDialog.m_UnitsString = "Measured in Watts";
	INumericEntryDialog.vIntegerOnly = false;
	INumericEntryDialog.vAllowNegative = false;

	int TempMaxPower = 50;
	switch (vLocalConfigurationData->vHVPSType)
	{
		case cDXMHVPS: 
			TempMaxPower = 600;
		break;
		case c210WMonoBlockHVPS: 
			TempMaxPower = 210;
		break;
		case cCheyneyMonoBlock:
			TempMaxPower = 210;
			break;
		case c100WMonoBlockHVPS:
		case c100WVJTMonoBlockHVPS:
			TempMaxPower = 100;
		break;
		case cXLFHVPS: 
			TempMaxPower = 480;
		break;
		case cMNXHVPS: 
			TempMaxPower = 50;
		break;
	}
	
	INumericEntryDialog.vEditString = dtoa(vMaximumPower,1);
	INumericEntryDialog.m_DialogTitleStaticText2 = "X-Ray Source Maximum Power";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Default Power = Maximum Power = " + dtoa(TempMaxPower);
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
	INumericEntryDialog.vMaxValue = TempMaxPower;
	INumericEntryDialog.vMinValue = 0;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vMaximumPower = ATOF(INumericEntryDialog.vEditString);
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

void CInspxHiddenXRayDialog::OnSubFunction4Button() 
{
	if (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock)
	{
		if (vIgnoreCheyneyInterlockStatus)
			vIgnoreCheyneyInterlockStatus = 0;
		else
			vIgnoreCheyneyInterlockStatus = 1;

		if (vIgnoreCheyneyInterlockStatus)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\nYou must Save to Main Menu, then exit the ScanTrac program and restart program for the Cheyney Interlock status to be ignored";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
		}

		UpdateDisplay();
		SetChangeMade();
	}
}

void CInspxHiddenXRayDialog::OnSubFunction5Button() 
{
	if (vLocalConfigurationData->vHVPSType == cXLFHVPS)
	{
		// Voltage Monitor Adjust
		CNumericEntryDialog INumericEntryDialog;  
		INumericEntryDialog.m_DialogTitleStaticText1 = "";
		INumericEntryDialog.m_DialogTitleStaticText2 = "";
		INumericEntryDialog.m_UnitsString = "Measured in kV";
		INumericEntryDialog.vIntegerOnly = false;
		INumericEntryDialog.vAllowNegative = false;
		
		INumericEntryDialog.vEditString = dtoa(vVoltageAdjustGain,4);
		INumericEntryDialog.m_DialogTitleStaticText3 = 
			"X-Ray Voltage Monitor Adjust Gain Amount";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
			INumericEntryDialog.vEditString;
		//INumericEntryDialog.m_UnitsString = "Amount to multiply original value by";
		INumericEntryDialog.vMaxValue = 2;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vVoltageAdjustGain = 
				ATOF(INumericEntryDialog.vEditString);
			UpdateDisplay();
			SetChangeMade();
			// Voltage Monitor Adjust
			CNumericEntryDialog INumericEntryDialog;  
			INumericEntryDialog.m_DialogTitleStaticText1 = "";
			INumericEntryDialog.m_DialogTitleStaticText2 = "";
			INumericEntryDialog.m_UnitsString = "Measured in kV";
			INumericEntryDialog.vIntegerOnly = false;
			INumericEntryDialog.vAllowNegative = true;
			
			INumericEntryDialog.vEditString = dtoa(vVoltageAdjustOffset,2);
			INumericEntryDialog.m_DialogTitleStaticText3 = "X-Ray Voltage Monitor Adjust Offset Amount";
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
			//INumericEntryDialog.m_UnitsString = "Amount to add to original value";
			INumericEntryDialog.vMaxValue = 60;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vVoltageAdjustOffset = ATOF(INumericEntryDialog.vEditString);
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
		if (nResponse == 10)
		{
			//Main Menu button pressed
			CDialog::EndDialog(10);
		}
	}
}

void CInspxHiddenXRayDialog::OnSubFunction6Button() 
{
	if (vLocalConfigurationData->vHVPSType == cXLFHVPS)
	{
		// Current Monitor Adjust
		CNumericEntryDialog INumericEntryDialog;  
		INumericEntryDialog.m_DialogTitleStaticText1 = "";
		INumericEntryDialog.m_DialogTitleStaticText2 = "";
		INumericEntryDialog.m_UnitsString = "Measured in mA";
		INumericEntryDialog.vIntegerOnly = false;
		INumericEntryDialog.vAllowNegative = false;

		INumericEntryDialog.vEditString = dtoa(vCurrentAdjustGain,4);
		INumericEntryDialog.m_DialogTitleStaticText3 = "X-Ray Current Monitor Gain Amount";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
		INumericEntryDialog.vMaxValue = 2;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vCurrentAdjustGain = ATOF(INumericEntryDialog.vEditString);
			UpdateDisplay();
			SetChangeMade();
			// Current Monitor Adjust
			CNumericEntryDialog INumericEntryDialog;  
			INumericEntryDialog.m_DialogTitleStaticText1 = "";
			INumericEntryDialog.m_DialogTitleStaticText2 = "";
			INumericEntryDialog.m_UnitsString = "Measured in mA";
			INumericEntryDialog.vIntegerOnly = false;
			INumericEntryDialog.vAllowNegative = true;

			INumericEntryDialog.vEditString = dtoa(vCurrentAdjustOffset,2);
			INumericEntryDialog.m_DialogTitleStaticText3 = "X-Ray Current Monitor Adjust Amount";
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
			INumericEntryDialog.vMaxValue = 60;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vCurrentAdjustOffset = ATOF(INumericEntryDialog.vEditString);
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
		if (nResponse == 10)
		{
			//Main Menu button pressed
			CDialog::EndDialog(10);
		}
	}
}

void CInspxHiddenXRayDialog::OnSubFunction7Button() 
{
}

void CInspxHiddenXRayDialog::OnSubFunction8Button() 
{
	if (vUse101VACFor10SecondsShutDownLimit)
		vUse101VACFor10SecondsShutDownLimit = 0;
	else
		vUse101VACFor10SecondsShutDownLimit = 1;
	UpdateDisplay();
	SetChangeMade();
}

BOOL CInspxHiddenXRayDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd * TempWindow = NULL;
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

HBRUSH CInspxHiddenXRayDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

	/*
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)  
	if (vSimulateLightControlBoard)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vGreenBrush;
	}
	*/

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	//if (pWnd->GetDlgCtrlID() == IDC_VoltageLabel)
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_VoltageLabel, 5); 
	//	pDC->SetBkMode(TRANSPARENT);
	//	return vLocalSystemData->vLightBrownBrush;
	//}
	//if (pWnd->GetDlgCtrlID() == IDC_CurrentLabel)
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentLabel, 5); 
	//	pDC->SetBkMode(TRANSPARENT);
	//	return vLocalSystemData->vLightBrownBrush;
	//}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Display, 5); 
		if (vUse101VACFor10SecondsShutDownLimit)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vYellowBrush;
		}
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
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
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
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
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
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Display, 5);
		if (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock)
		if (vIgnoreCheyneyInterlockStatus)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vYellowBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Display, 5);
		if (vUse101VACFor10SecondsShutDownLimit)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vYellowBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function3Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Function5Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Display, 5);
	
	return hbr;
}

void CInspxHiddenXRayDialog::UpdateDisplay() 
{
	if (vIgnoreCheyneyInterlockStatus)
	{
		SetDlgItemText(IDC_SubFunction4Display, _T("Ignoring Cheyney Interlock Status"));
		SetDlgItemText(IDC_SubFunction4Button, _T("Use Cheyney Interlock Status"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction4Display, _T("Using Cheyney Interlock Status"));
		SetDlgItemText(IDC_SubFunction4Button, _T("Ignore Cheyney Interlock Status"));
	}

	if (vLocalConfigurationData->vHVPSType != cXLFHVPS)
	{
		m_SubFunction5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
		m_SubFunction5Display.ShowWindow(SW_HIDE);
		m_SubFunction6Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
		m_SubFunction6Display.ShowWindow(SW_HIDE);
	}

	SetDlgItemText(IDC_SubFunction1Display,dtoa(vMaximumVoltage,1));
	SetDlgItemText(IDC_SubFunction2Display,dtoa(vMaximumCurrent,2));
	SetDlgItemText(IDC_SubFunction3Display,dtoa(vMaximumPower,1));

	CString TempString = dtoa(vVoltageAdjustOffset,2);
	CString TempString1 = dtoa(vVoltageAdjustGain,4);
	TempString = TempString1 + "X + " + TempString;
	SetDlgItemText(IDC_SubFunction5Display,TempString);

	TempString = dtoa(vCurrentAdjustOffset,2);
	TempString1 = dtoa(vCurrentAdjustGain,4);
	TempString = TempString1 + "X + " + TempString;
	SetDlgItemText(IDC_SubFunction6Display,TempString);

	if (vUse101VACFor10SecondsShutDownLimit)
	{
		SetDlgItemText(IDC_SubFunction8Display, _T("Shut Down X-Rays at 101 VAC for 10 Seconds"));
		SetDlgItemText(IDC_SubFunction8Button, _T("Shut Down at 106 VAC for 10 Seconds"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction8Display, _T("Shut Down X-Rays at 106 VAC for 10 Seconds (Default)"));
		SetDlgItemText(IDC_SubFunction8Button, _T("Shut Down at 101 VAC for 10 Seconds"));
	}
}

void CInspxHiddenXRayDialog::SetChangeMade() 
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


void CInspxHiddenXRayDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
