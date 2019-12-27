//ScanTrac Side View Source File
// ConfigureRadiationDoorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "ConfigureRadiationDoorDialog.h"
#include "NumericEntryDialog.h"
#include "YesNoDialog.h"
#include "SerialPort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BYTE vGlobalShiftKeyDown;
extern CSerialPort *vGlobaluCSerialPort;
extern unsigned long vGlobalDialogTitleColor;
/////////////////////////////////////////////////////////////////////////////
// CConfigureRadiationDoorDialog dialog


CConfigureRadiationDoorDialog::CConfigureRadiationDoorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigureRadiationDoorDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigureRadiationDoorDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	vReCalibrateAfterStopTimePeriod = 60;
	vRadiationShutterInSystem = false;
	vMainWindowPointer = NULL;
	vChangeMade = false;
	vRadiationDoorInvertDoorOutput = 0;
	//vRadiationDoorMaximumDistance = 120;
	vRadiationDoorDistanceStayOpen = 10;
	vRadiationDoorInDoorDistance = 1;
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_SubFunction1Display);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function3Display);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Function5Display);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction2Display);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction3Display);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction4Display);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction5Display);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction6Display);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction7Display);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction8Display);

}

void CConfigureRadiationDoorDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

	vReCalibrateAfterStopTimePeriod = vLocalConfigurationData->vReCalibrateAfterStopTimePeriod;
	vRadiationShutterInSystem = vLocalConfigurationData->vRadiationShutterInSystem;
	vRadiationDoorInvertDoorOutput = vLocalConfigurationData->vRadiationDoorInvertDoorOutput;
	//vRadiationDoorMaximumDistance = 
	//	vLocalConfigurationData->vRadiationDoorMaximumDistance;
	vRadiationDoorDistanceStayOpen = vLocalConfigurationData->vRadiationDoorDistanceStayOpen;
	vRadiationDoorInDoorDistance = vLocalConfigurationData->vRadiationDoorInDoorDistance;

	UpdateDisplay();
	UpdateButtons();

	this->SetWindowText( _T("Shutter"));
	::SetupMenu(vLocalCWndCollection);
}

void CConfigureRadiationDoorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigureRadiationDoorDialog)
	DDX_Control(pDX, IDC_SubFunction1Display, m_SubFunction1Display);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function3Display, m_Function3Display);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function5Display, m_Function5Display);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction2Display, m_SubFunction2Display);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction3Display, m_SubFunction3Display);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction4Display, m_SubFunction4Display);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction5Display, m_SubFunction5Display);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction6Display, m_SubFunction6Display);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction7Display, m_SubFunction7Display);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction8Display, m_SubFunction8Display);
}


BEGIN_MESSAGE_MAP(CConfigureRadiationDoorDialog, CDialog)
	//{{AFX_MSG_MAP(CConfigureRadiationDoorDialog)
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
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CConfigureRadiationDoorDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigureRadiationDoorDialog message handlers

void CConfigureRadiationDoorDialog::OnFunction1Button() 
{
	//Main Menu Button Pressed
	//PrepareToExit();
	CDialog::EndDialog(10);
}

void CConfigureRadiationDoorDialog::OnFunction2Button() 
{
	//Save and exit was pressed
	if ((!vChangeMade) || (KeepSetupChanges()))
	{
		vLocalConfigurationData->vReCalibrateAfterStopTimePeriod = vReCalibrateAfterStopTimePeriod;
		vLocalConfigurationData->vRadiationShutterInSystem = vRadiationShutterInSystem;
		vLocalConfigurationData->vRadiationDoorInvertDoorOutput = vRadiationDoorInvertDoorOutput;
		//vLocalConfigurationData->vRadiationDoorMaximumDistance = 
		//	vRadiationDoorMaximumDistance;
		vLocalConfigurationData->vRadiationDoorDistanceStayOpen = vRadiationDoorDistanceStayOpen;
		vLocalConfigurationData->vRadiationDoorInDoorDistance = vRadiationDoorInDoorDistance;

		vMainWindowPointer->SendRadiationDoorParameters();

		//PrepareToExit();
		CDialog::EndDialog(true);
	}
}

void CConfigureRadiationDoorDialog::OnFunction3Button() 
{
}

void CConfigureRadiationDoorDialog::OnFunction4Button() 
{
	//Cancel was pressed
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
	{
		//PrepareToExit();
		CDialog::OnCancel();
	}
}

void CConfigureRadiationDoorDialog::OnFunction5Button() 
{
}

void CConfigureRadiationDoorDialog::OnSubFunction1Button() 
{  //if in manual mode, put shutter in Auto mode 
	if (vLocalSystemData->vRadiationDoorManualControlRegister & 3) 
	{
		vLocalSystemData->vRadiationDoorManualControlRegister = cRadiationShutterAutoControl;
		if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
			vGlobaluCSerialPort->SetRadiationDoorManualControl(cRadiationShutterAutoControl);
		else
			vGlobaluCSerialPort->SetRadiationDoorManualControl(cRadiationShutterOpen);
		UpdateButtons();
	}

}

void CConfigureRadiationDoorDialog::OnSubFunction2Button() 
{
	// Open or Close Shutter manually
	if (vLocalSystemData->vRadiationDoorManualControlRegister & 2) //if open, then close it
	{
		vGlobaluCSerialPort->SetRadiationDoorManualControl(cRadiationShutterClose);
		vLocalSystemData->vRadiationDoorManualControlRegister = cRadiationShutterClose;
	}
	else
	{
		vGlobaluCSerialPort->SetRadiationDoorManualControl(cRadiationShutterOpen);
		vLocalSystemData->vRadiationDoorManualControlRegister = cRadiationShutterOpen;
	}

	UpdateButtons();
}

void CConfigureRadiationDoorDialog::OnSubFunction3Button() 
{
	vRadiationShutterInSystem = !vRadiationShutterInSystem;
	SetChangeMade();
	UpdateDisplay();
}

void CConfigureRadiationDoorDialog::OnSubFunction4Button() 
{
	// Invert Shutter Output
	vRadiationDoorInvertDoorOutput = !vRadiationDoorInvertDoorOutput;
	UpdateButtons();
	SetChangeMade();
}

void CConfigureRadiationDoorDialog::OnSubFunction5Button() 
{
	/*
  //Encoder Divider button pressed
	CNumericEntryDialog INumericEntryDialog;  
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vRadiationDoorMaximumDistance,0);

	INumericEntryDialog.m_DialogTitleStaticText1 = "";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Radiation Shutter Maximum Distance";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Setup System";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
		INumericEntryDialog.vEditString;

	INumericEntryDialog.m_UnitsString = "Default: 120";
	INumericEntryDialog.vMaxValue = 500;
	INumericEntryDialog.vMinValue = 44;
	INumericEntryDialog.vIntegerOnly = true;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vRadiationDoorMaximumDistance = _wtoi(INumericEntryDialog.vEditString);
		UpdateDisplay();
		SetChangeMade();
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		PrepareToExit();
		CDialog::EndDialog(10);
	}
	*/
}

void CConfigureRadiationDoorDialog::OnSubFunction6Button() 
{
  //Distance Shutter Stays Open
	CNumericEntryDialog INumericEntryDialog;  
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vRadiationDoorDistanceStayOpen,0);

	INumericEntryDialog.m_DialogTitleStaticText1 = "";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Radiation Shutter Distance To Stay Open";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Setup System";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
		INumericEntryDialog.vEditString;

	INumericEntryDialog.m_UnitsString = "Default: 100, Minimum: 1 " + 
		vLocalConfigurationData->vUnitsString;
	INumericEntryDialog.vMaxValue = 2000;
	INumericEntryDialog.vMinValue = 1;
	INumericEntryDialog.vIntegerOnly = true;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vRadiationDoorDistanceStayOpen = _wtoi(INumericEntryDialog.vEditString);
		UpdateDisplay();
		SetChangeMade();
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		//PrepareToExit();
		CDialog::EndDialog(10);
	}
}

void CConfigureRadiationDoorDialog::OnSubFunction7Button() 
{
	CNumericEntryDialog INumericEntryDialog;  
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vRadiationDoorInDoorDistance,0);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Radiation Shutter Distance";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Setup System";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Default: 0 = Open Immediately";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
		INumericEntryDialog.vEditString;

	INumericEntryDialog.m_UnitsString = "Units in " + 
		vLocalConfigurationData->vUnitsString;

	INumericEntryDialog.vMaxValue = (double)512.0  / (double)vGlobalPixelsPerUnit;
	INumericEntryDialog.vMinValue = 0;
	INumericEntryDialog.vIntegerOnly = true;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vRadiationDoorInDoorDistance = _wtoi(INumericEntryDialog.vEditString);
		UpdateDisplay();
		SetChangeMade();
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		//PrepareToExit();
		CDialog::EndDialog(10);
	}
}

void CConfigureRadiationDoorDialog::OnSubFunction8Button() 
{
	CNumericEntryDialog INumericEntryDialog;  
	
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vReCalibrateAfterStopTimePeriod,0);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Time after shutter door closes before";
	INumericEntryDialog.m_DialogTitleStaticText2 = "perform Re-calibrate Detector Offsets";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Setup System";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
		INumericEntryDialog.vEditString;

	INumericEntryDialog.m_UnitsString = "Default: 60, Minimum: 10, 0=Disabled";

	INumericEntryDialog.vMaxValue = 1800;
	INumericEntryDialog.vMinValue = 10;
	INumericEntryDialog.vAllowZero = true;
	INumericEntryDialog.vIntegerOnly = true;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vReCalibrateAfterStopTimePeriod = _wtoi(INumericEntryDialog.vEditString);
		UpdateDisplay();
		SetChangeMade();
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		//PrepareToExit();
		CDialog::EndDialog(10);
	}
}

BOOL CConfigureRadiationDoorDialog::PreTranslateMessage(MSG* pMsg) 
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
					cShowHelp, 58, 0);
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
		return true;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CConfigureRadiationDoorDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
		pDC->SetAttribDC(pDC->m_hDC);

	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Display, 5);
		if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cUpSideDownSoloScanTrac) || 
		(vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)) //Sideview ScanTracs should have a shutter
		{
			if (!vRadiationShutterInSystem)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cButtonTextColor);
				return vLocalSystemData->vYellowBrush;
			}
		}
		else
		{
			if (vRadiationShutterInSystem)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cButtonTextColor);
				return vLocalSystemData->vYellowBrush;
			}
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Display, 5);
	if (!vRadiationDoorInvertDoorOutput)  //if door open or closed
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vYellowBrush;
  }
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5);
		if (vLocalSystemData->vRadiationDoorManualControlRegister & 3)  //if door open or closed
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vYellowBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Display, 5);
	if (vLocalSystemData->vRadiationDoorManualControlRegister & 3)  //if door open or closed
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vYellowBrush;
  }
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Display, 5);
	if ((vRadiationDoorDistanceStayOpen < 24) || (vRadiationDoorDistanceStayOpen > 200))
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
	}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Display, 5);
	if (vRadiationDoorInDoorDistance)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
	}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Function3Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Function3Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Function5Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Function5Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
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
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  
		if (vLocalSystemData->vRadiationDoorManualControlRegister & 3)  //if door open or closed
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vGreenBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  
	if (!vRadiationDoorInvertDoorOutput)  //if door open or closed
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vGreenBrush;
  }
		pDC->SetBkMode(TRANSPARENT);
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
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CConfigureRadiationDoorDialog::SetChangeMade() 
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

void CConfigureRadiationDoorDialog::UpdateDisplay() 
{
	SetDlgItemText(IDC_SubFunction8Display,dtoa(vReCalibrateAfterStopTimePeriod,0) + " Seconds");

	if (vRadiationShutterInSystem)
	{
		SetDlgItemText(IDC_SubFunction3Button, _T("No Radiation Shutter"));
		SetDlgItemText(IDC_SubFunction3Display, _T("System has Radiation Shutter"));
	}
	else	
	{
		SetDlgItemText(IDC_SubFunction3Button, _T("Radiation Shutter in System"));
		SetDlgItemText(IDC_SubFunction3Display, _T("No Radiation Shutter in System"));
	}

	//SetDlgItemText(IDC_SubFunction5Display,dtoa(vRadiationDoorMaximumDistance,0) + " " +
	//	vLocalConfigurationData->vUnitsString);
	SetDlgItemText(IDC_SubFunction6Display,dtoa(vRadiationDoorDistanceStayOpen,0) + " " +
		vLocalConfigurationData->vUnitsString);
	SetDlgItemText(IDC_SubFunction7Display,dtoa(vRadiationDoorInDoorDistance,0) + " " +
		vLocalConfigurationData->vUnitsString);
}

void CConfigureRadiationDoorDialog::UpdateButtons()
{
	if (vLocalSystemData->vRadiationDoorManualControlRegister & 1)
	{
		if (vLocalSystemData->vRadiationDoorManualControlRegister & 2)
			SetDlgItemText(IDC_SubFunction2Display, _T("Shutter Open"));
		else
			SetDlgItemText(IDC_SubFunction2Display, _T("Shutter Closed"));
	}
	else
		SetDlgItemText(IDC_SubFunction2Display, _T("Auto Shutter Control"));

	if (vLocalSystemData->vRadiationDoorManualControlRegister & 3)  //if door open or closed
	{
		//SetDlgItemText(IDC_SubFunction1Button,"Return To Auto Mode");
		//SetDlgItemText(IDC_SubFunction1Display,"Shutter In Manual Mode");
		m_SubFunction1Button.ShowWindow(SW_SHOW);
		m_SubFunction1Display.ShowWindow(SW_SHOW);
	}
	else
	{
		m_SubFunction1Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
		m_SubFunction1Display.ShowWindow(SW_HIDE);
	}

	/*
	if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
	{
		if (vLocalSystemData->vRadiationDoorManualControlRegister & 1)
			SetDlgItemText(IDC_SubFunction1Button,"Auto Control of Shutter");
		else
		if (vLocalSystemData->vRadiationDoorManualControlRegister & 2)
			SetDlgItemText(IDC_SubFunction1Button,"Close\nShutter");
		else
			SetDlgItemText(IDC_SubFunction1Button,"Open\nShutter");
	}
	else
	*/
	//{
		if (vLocalSystemData->vRadiationDoorManualControlRegister & 2)
			SetDlgItemText(IDC_SubFunction2Button, _T("Close\nShutter\nNow!"));
		else
			SetDlgItemText(IDC_SubFunction2Button, _T("Open\nShutter\nNow!"));
	//}
	if (vRadiationDoorInvertDoorOutput)
	{
		SetDlgItemText(IDC_SubFunction4Button, _T("Normal Shutter Output"));
		SetDlgItemText(IDC_SubFunction4Display, _T("Shutter Output Inverted"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction4Button, _T("Invert Shutter Output"));
		SetDlgItemText(IDC_SubFunction4Display, _T("Shutter Output Normal"));
	}
}

void CConfigureRadiationDoorDialog::PrepareToExit()
{
	if (vLocalSystemData->vRadiationDoorManualControlRegister)
	{
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText =  _T("\nManual Control of Radiation Shutter Still Active.\nReturn to Automatic Control of Radiation Shutter?");
		TempYesNoDialog.vYesButtonText = "Return to Auto";
		TempYesNoDialog.vNoButtonText = "Stay Manual";
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
		{
			vLocalSystemData->vRadiationDoorManualControlRegister = cRadiationShutterAutoControl;
			if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
				vGlobaluCSerialPort->SetRadiationDoorManualControl(cRadiationShutterAutoControl);
			else
				vGlobaluCSerialPort->SetRadiationDoorManualControl(cRadiationShutterOpen);
		}
	}
	Sleep(1); //is the equivelent of a yeild statement;
}




void CConfigureRadiationDoorDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
