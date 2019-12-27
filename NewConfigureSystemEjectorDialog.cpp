//ScanTrac Side View Source File
// NewConfigureSystemEjectorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "NewConfigureSystemEjectorDialog.h"
#include "NumericEntryDialog.h"
#include "AlphaEntryDialog.h"
#include "NoticeDialog.h"
#include "YesNoDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern bool vGlobalFPGAVersion15Point34OrHigher;
extern bool vGlobalFPGAVersion16point0orAbove;
extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
/////////////////////////////////////////////////////////////////////////////
// CNewConfigureSystemEjectorDialog dialog


CNewConfigureSystemEjectorDialog::CNewConfigureSystemEjectorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNewConfigureSystemEjectorDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewConfigureSystemEjectorDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	vChangeMade = false;
	vEjectorNumberEditing = 0;
	vFirstEjectorShown = 0;

	vLightGreenBrush = ::CreateSolidBrush(cLightGreen);
	vGreenBrush = ::CreateSolidBrush(cGreen);
	vYellowBrush = ::CreateSolidBrush(cYellow);
	vWhiteBrush = ::CreateSolidBrush(cWhite);
	vBlueBrush = ::CreateSolidBrush(cBlue);
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_EjectorName3);
	vLocalCWndCollection.Add(&m_EjectorName2);
	vLocalCWndCollection.Add(&m_EjectorName1);
	vLocalCWndCollection.Add(&m_DigitalInputLine3);
	vLocalCWndCollection.Add(&m_DigitalInputLine2);
	vLocalCWndCollection.Add(&m_DigitalInputLine1);
	vLocalCWndCollection.Add(&m_ConfirmDigitalInputLine3);
	vLocalCWndCollection.Add(&m_ConfirmDigitalInputLine2);
	vLocalCWndCollection.Add(&m_ConfirmDigitalInputLine1);
	vLocalCWndCollection.Add(&m_ConfirmActiveHigh3);
	vLocalCWndCollection.Add(&m_ConfirmActiveHigh2);
	vLocalCWndCollection.Add(&m_ConfirmActiveHigh1);
	vLocalCWndCollection.Add(&m_ActiveHigh3);
	vLocalCWndCollection.Add(&m_ActiveHigh2);
	vLocalCWndCollection.Add(&m_ActiveHigh1);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Note);
	vLocalCWndCollection.Add(&m_Title1);
	vLocalCWndCollection.Add(&m_Title10);
	vLocalCWndCollection.Add(&m_Title6);
	vLocalCWndCollection.Add(&m_Title7);
	vLocalCWndCollection.Add(&m_Title8);
	vLocalCWndCollection.Add(&m_Title9);
}

CNewConfigureSystemEjectorDialog::~CNewConfigureSystemEjectorDialog()
{
	DeleteObject(vLightGreenBrush);
	DeleteObject(vGreenBrush);
	DeleteObject(vYellowBrush);
	DeleteObject(vWhiteBrush);
	DeleteObject(vBlueBrush);
}

void CNewConfigureSystemEjectorDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);
	vFunction3ButtonEnable = true;
	vFunction5ButtonEnable = true;
	vSubFunction2ButtonEnable = true;
	vSubFunction3ButtonEnable = true;
	vSubFunction4ButtonEnable = true;
	vSubFunction5ButtonEnable = true;
	vSubFunction6ButtonEnable = true;
	vSubFunction7ButtonEnable = true;

	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
	{
		vName[TempLoop] = vLocalConfigurationData->vEjector[TempLoop].vName;
		vDigitalInputLine[TempLoop] = dtoa(vLocalConfigurationData->vEjector[TempLoop].vDigitalInputLine, 0);
		vEnabled[TempLoop] = vLocalConfigurationData->vEjector[TempLoop].vEnabled;
		vActiveHigh[TempLoop] = vLocalConfigurationData->vEjector[TempLoop].vActiveHigh;

		vConfirmDigitalInputLine[TempLoop] = dtoa(vLocalConfigurationData->vEjector[TempLoop].vConfirmDigitalInputLine, 0);
		vConfirmActiveHigh[TempLoop] = vLocalConfigurationData->vEjector[TempLoop].vConfirmActiveHigh;
	}
	vUseXofYAlarmOutputForUnconfirmedEject = vLocalConfigurationData->vUseXofYAlarmOutputForUnconfirmedEject;
	UpdateDisplay();
	this->SetWindowText(_T("Ejectors"));
	::SetupMenu(vLocalCWndCollection);
}

void CNewConfigureSystemEjectorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewConfigureSystemEjectorDialog)
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_EjectorName3, m_EjectorName3);
	DDX_Control(pDX, IDC_EjectorName2, m_EjectorName2);
	DDX_Control(pDX, IDC_EjectorName1, m_EjectorName1);
	DDX_Control(pDX, IDC_DigitalInputLine3, m_DigitalInputLine3);
	DDX_Control(pDX, IDC_DigitalInputLine2, m_DigitalInputLine2);
	DDX_Control(pDX, IDC_DigitalInputLine1, m_DigitalInputLine1);
	DDX_Control(pDX, IDC_ConfirmDigitalInputLine3, m_ConfirmDigitalInputLine3);
	DDX_Control(pDX, IDC_ConfirmDigitalInputLine2, m_ConfirmDigitalInputLine2);
	DDX_Control(pDX, IDC_ConfirmDigitalInputLine1, m_ConfirmDigitalInputLine1);
	DDX_Control(pDX, IDC_ConfirmActiveHigh3, m_ConfirmActiveHigh3);
	DDX_Control(pDX, IDC_ConfirmActiveHigh2, m_ConfirmActiveHigh2);
	DDX_Control(pDX, IDC_ConfirmActiveHigh1, m_ConfirmActiveHigh1);
	DDX_Control(pDX, IDC_ActiveHigh3, m_ActiveHigh3);
	DDX_Control(pDX, IDC_ActiveHigh2, m_ActiveHigh2);
	DDX_Control(pDX, IDC_ActiveHigh1, m_ActiveHigh1);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Note, m_Note);
	DDX_Control(pDX, IDC_Title1, m_Title1);
	DDX_Control(pDX, IDC_Title10, m_Title10);
	DDX_Control(pDX, IDC_Title6, m_Title6);
	DDX_Control(pDX, IDC_Title7, m_Title7);
	DDX_Control(pDX, IDC_Title8, m_Title8);
	DDX_Control(pDX, IDC_Title9, m_Title9);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewConfigureSystemEjectorDialog, CDialog)
	//{{AFX_MSG_MAP(CNewConfigureSystemEjectorDialog)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_WM_SHOWWINDOW()
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
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CNewConfigureSystemEjectorDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewConfigureSystemEjectorDialog message handlers

void CNewConfigureSystemEjectorDialog::OnFunction1Button() 
{
	//Main Menu was pressed
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::EndDialog(10);
}

void CNewConfigureSystemEjectorDialog::OnFunction2Button() 
{
	//Save and exit was pressed
	if (ConfigurationOK(vEjectorNumberEditing + vFirstEjectorShown))
	{
		if ((!vChangeMade) || (KeepSetupChanges()))
		{
			if (vChangeMade)
				vLocalSystemData->vCurrentProductChanged = true;

			for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
			{
				vLocalConfigurationData->vEjector[TempLoop].vName = vName[TempLoop];
				vLocalConfigurationData->vEjector[TempLoop].SetDigitalInputLine(_wtoi(vDigitalInputLine[TempLoop]));
				vLocalConfigurationData->vEjector[TempLoop].vEnabled = vEnabled[TempLoop];
				vLocalConfigurationData->vEjector[TempLoop].vActiveHigh = vActiveHigh[TempLoop];
				vLocalConfigurationData->vEjector[TempLoop].SetConfirmDigitalInputLine(_wtoi(vConfirmDigitalInputLine[TempLoop]));
				vLocalConfigurationData->vEjector[TempLoop].vConfirmActiveHigh = vConfirmActiveHigh[TempLoop];
				if (vLocalConfigurationData->vEjector[TempLoop].vConfirmDigitalInputLine == 0)
					vLocalConfigurationData->vEjector[TempLoop].vConfirmEnabled = false;
				else
					vLocalConfigurationData->vEjector[TempLoop].vConfirmEnabled = true;
			}
			vLocalConfigurationData->vUseXofYAlarmOutputForUnconfirmedEject	= vUseXofYAlarmOutputForUnconfirmedEject;
			CDialog::EndDialog(true);
		}
	}
}


void CNewConfigureSystemEjectorDialog::OnFunction3Button() 
{
	if (vFunction3ButtonEnable == true)
	{
	//Toggle Confirm Active High button Was pressed
	if (vEnabled[vEjectorNumberEditing + vFirstEjectorShown])
	if (vConfirmDigitalInputLine[vEjectorNumberEditing + vFirstEjectorShown] != "0")
	{
		if (vConfirmActiveHigh[vEjectorNumberEditing + vFirstEjectorShown] == 0)
			vConfirmActiveHigh[vEjectorNumberEditing + vFirstEjectorShown] = 1;
		else
			vConfirmActiveHigh[vEjectorNumberEditing + vFirstEjectorShown] = 0;
		UpdateDisplay();
		SetChangeMade();
	}
	}
	else
	{
		ShowFunctionNotValidDialogMessage();
	}
}

void CNewConfigureSystemEjectorDialog::OnFunction4Button() 
{
	//Cancel was pressed
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::OnCancel();
}

void CNewConfigureSystemEjectorDialog::OnFunction5Button() 
{
	if (vFunction5ButtonEnable == true)
	{
	//Edit Confirm Digital Input Line Button Was Pressed
	if (vEnabled[vEjectorNumberEditing + vFirstEjectorShown])
	{
		CNumericEntryDialog INumericEntryDialog;  
		
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = vConfirmDigitalInputLine[vEjectorNumberEditing + vFirstEjectorShown];
		INumericEntryDialog.m_DialogTitleStaticText1 = "Confirm Digital Input Line For";
		INumericEntryDialog.m_DialogTitleStaticText2 = "System Ejector " + dtoa(vEjectorNumberEditing + vFirstEjectorShown + 1, 0);
		INumericEntryDialog.m_DialogTitleStaticText3 = vName[vEjectorNumberEditing + vFirstEjectorShown];
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + vConfirmDigitalInputLine[vEjectorNumberEditing + vFirstEjectorShown];

		INumericEntryDialog.m_UnitsString = "Zero Indicates No Detector Connected";
		INumericEntryDialog.vMaxValue = 8;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vConfirmDigitalInputLine[vEjectorNumberEditing + vFirstEjectorShown] = INumericEntryDialog.vEditString;
			if (_wtoi(vConfirmDigitalInputLine[vEjectorNumberEditing + vFirstEjectorShown]) != 0)
			{
				CYesNoDialog TempYesNoDialog;
				TempYesNoDialog.vNoticeText = _T("\nDo you want to generate a\nsignal on X of Y Alarm 2 output\nwhen an unconfirmed eject error occurs?\n");
				TempYesNoDialog.vYesButtonText = "Ouput on Alarm 2";
				TempYesNoDialog.vNoButtonText = "No Output";
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult == IDOK)
				{
					if (vEjectorNumberEditing + vFirstEjectorShown == 0)
						vUseXofYAlarmOutputForUnconfirmedEject = vUseXofYAlarmOutputForUnconfirmedEject | 1;
					else 
						if (vEjectorNumberEditing + vFirstEjectorShown == 1)
						vUseXofYAlarmOutputForUnconfirmedEject = vUseXofYAlarmOutputForUnconfirmedEject | 2;
					else 
						if (vEjectorNumberEditing + vFirstEjectorShown == 2)
						vUseXofYAlarmOutputForUnconfirmedEject = vUseXofYAlarmOutputForUnconfirmedEject | 4;
				}
				else
				{
					if ((vEjectorNumberEditing + vFirstEjectorShown == 0) && (vUseXofYAlarmOutputForUnconfirmedEject & 1))
						vUseXofYAlarmOutputForUnconfirmedEject = vUseXofYAlarmOutputForUnconfirmedEject & 6;
					else if ((vEjectorNumberEditing == 1) && (vUseXofYAlarmOutputForUnconfirmedEject & 2))
						vUseXofYAlarmOutputForUnconfirmedEject = vUseXofYAlarmOutputForUnconfirmedEject & 5;
					else if ((vEjectorNumberEditing == 2) && (vUseXofYAlarmOutputForUnconfirmedEject & 4))
						vUseXofYAlarmOutputForUnconfirmedEject = vUseXofYAlarmOutputForUnconfirmedEject & 3;
				}
			}
				
			SetChangeMade();
			UpdateDisplay();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
	}
	else
	{
		ShowEjectorEditNotEnabledDialogMessage();
	}
}

void CNewConfigureSystemEjectorDialog::OnSubFunction1Button() 
{
	//Next or Previous Page
	if (vGlobalFPGAVersion15Point34OrHigher)
	{
		vFirstEjectorShown = vFirstEjectorShown + 3;
		if (vFirstEjectorShown >= cNumberOfEjectorsForUser)  //should be cNumberOfEjectors  //only allow 30 ejectors as that is all hardware supports, but software supports 32 if remove this
			vFirstEjectorShown = 0;

		vEjectorNumberEditing = 0; //start at first ejector in this group

		UpdateDisplay();
	}
}

void CNewConfigureSystemEjectorDialog::OnSubFunction2Button() 
{
	if (vSubFunction2ButtonEnable == true)
	{
	//Select Ejector 1 button pressed
		if (ConfigurationOK(vEjectorNumberEditing + vFirstEjectorShown))
		{
			vEjectorNumberEditing = 0;
			UpdateDisplay();
		}
	}
	else
	{
		ShowEjectorAlreadySelectedDialogMessage();
	}
}

void CNewConfigureSystemEjectorDialog::OnSubFunction3Button() 
{
	if (vSubFunction3ButtonEnable == true)
	{
	//Select Ejector 2 button pressed
		if (ConfigurationOK(vEjectorNumberEditing + vFirstEjectorShown))
		{
			vEjectorNumberEditing = 1;
			UpdateDisplay();
		}
	}
	else
	{
		ShowEjectorAlreadySelectedDialogMessage();
	}
}

void CNewConfigureSystemEjectorDialog::OnSubFunction4Button() 
{
	if (vSubFunction4ButtonEnable == true)
	{
	//Select Ejector 3 button pressed
		if (ConfigurationOK(vEjectorNumberEditing + vFirstEjectorShown))
		{
			vEjectorNumberEditing = 2;
			UpdateDisplay();
		}
	}
	else
	{
		ShowEjectorAlreadySelectedDialogMessage();
	}
}

void CNewConfigureSystemEjectorDialog::OnSubFunction5Button() 
{
	if (vSubFunction5ButtonEnable == true)
	{
	//Edit Digital Output Line Button Was Pressed
	if (vEnabled[vEjectorNumberEditing + vFirstEjectorShown])
	{
		CNumericEntryDialog INumericEntryDialog;  
		
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = vDigitalInputLine[vEjectorNumberEditing + vFirstEjectorShown];
		INumericEntryDialog.m_DialogTitleStaticText1 = "Digital Output Line For";
		INumericEntryDialog.m_DialogTitleStaticText2 = "System Ejector " + dtoa(vEjectorNumberEditing + vFirstEjectorShown + 1, 0);
		INumericEntryDialog.m_DialogTitleStaticText3 = vName[vEjectorNumberEditing + vFirstEjectorShown];
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + vDigitalInputLine[vEjectorNumberEditing + vFirstEjectorShown];

		INumericEntryDialog.m_UnitsString = "Zero Indicates None";
		INumericEntryDialog.vMaxValue = cNumberOfEjectors;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vDigitalInputLine[vEjectorNumberEditing + vFirstEjectorShown] = INumericEntryDialog.vEditString;

			SetChangeMade();
			UpdateDisplay();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
	}
	else
	{
		ShowEjectorEditNotEnabledDialogMessage();
	}
}

void CNewConfigureSystemEjectorDialog::OnSubFunction6Button() 
{
	if (vSubFunction6ButtonEnable == true)
	{
  //Edit Ejector Name button was pressed
	if (vEnabled[vEjectorNumberEditing + vFirstEjectorShown])
	{
		CAlphaEntryDialog IAlphaEntryDialog;  
		
		//Set dialog box data titles and number value
		IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
		IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
		
		IAlphaEntryDialog.vEditString = vName[vEjectorNumberEditing + vFirstEjectorShown];
		IAlphaEntryDialog.m_DialogTitleStaticText = "System Ejector " + dtoa(vEjectorNumberEditing + vFirstEjectorShown + 1, 0);
		IAlphaEntryDialog.m_DialogTitleStaticText2 = "Original Value: " + vName[vEjectorNumberEditing + vFirstEjectorShown];
		//Pass control to dialog box and display
		int nResponse = IAlphaEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vName[vEjectorNumberEditing + vFirstEjectorShown] = IAlphaEntryDialog.vEditString;
			UpdateDisplay();
			SetChangeMade();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
	}
	else
	{
		ShowEjectorEditNotEnabledDialogMessage();
	}
}

void CNewConfigureSystemEjectorDialog::OnSubFunction7Button() 
{
	if (vSubFunction7ButtonEnable == true)
	{
	//Toggle Active High button Was pressed
	if (vEnabled[vEjectorNumberEditing + vFirstEjectorShown])
	{
		vActiveHigh[vEjectorNumberEditing + vFirstEjectorShown] = !vActiveHigh[vEjectorNumberEditing + vFirstEjectorShown];
		SetChangeMade();
		UpdateDisplay();
	}
	}
	else
	{
		ShowEjectorEditNotEnabledDialogMessage();
	}
}

void CNewConfigureSystemEjectorDialog::OnSubFunction8Button() 
{
	//Toggle Enable button Was pressed
	bool TempAllEjectorsBeforeOK = true;
	if (vEnabled[vEjectorNumberEditing + vFirstEjectorShown]) //if this one is enabled, then will disable.  Only allowed to disable if all ones after are disabled
	{
		for (BYTE TempLoop = vEjectorNumberEditing + vFirstEjectorShown + 1; TempLoop < cNumberOfEjectors; TempLoop++)
		{
			if (vEnabled[TempLoop])
				TempAllEjectorsBeforeOK = false;
		}
		if (!TempAllEjectorsBeforeOK)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nTo Disable this ejector\nall ejectors after it must be Disabled";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	{ //if this one disabled, will enable.  Only allowed to enable if all ones before it are enabled
		for (BYTE TempLoop = 0; TempLoop < vEjectorNumberEditing + vFirstEjectorShown; TempLoop++)
		{
			if (!vEnabled[TempLoop])
				TempAllEjectorsBeforeOK = false;
		}
		if (!TempAllEjectorsBeforeOK)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nTo Enable this ejector\nall ejectors before it must be Enabled";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}

	if (TempAllEjectorsBeforeOK)
	{
		vEnabled[vEjectorNumberEditing + vFirstEjectorShown] = !vEnabled[vEjectorNumberEditing + vFirstEjectorShown];
		SetChangeMade();
		UpdateDisplay();
	}
}

BOOL CNewConfigureSystemEjectorDialog::PreTranslateMessage(MSG* pMsg) 
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
		int TempKey = pMsg->wParam;
		if (vGlobalShiftKeyDown)
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
		else
		switch (TempKey)
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
				PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 13, 0);
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

HBRUSH CNewConfigureSystemEjectorDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	
	// TODO: Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Note)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Note, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Title1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title1, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLightGreenBrush;
	}
	//if (pWnd->GetDlgCtrlID() == IDC_Title2)
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title2, 5);
	//	pDC->SetBkMode(TRANSPARENT);
	//	return vLightGreenBrush;
	//}
	//if (pWnd->GetDlgCtrlID() == IDC_Title4)
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title4, 5);
	//	pDC->SetBkMode(TRANSPARENT);
	//	return vLightGreenBrush;
	//}
	//if (pWnd->GetDlgCtrlID() == IDC_Title5)
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title5, 5);
	//	pDC->SetBkMode(TRANSPARENT);
	//	return vLightGreenBrush;
	//}
	if (pWnd->GetDlgCtrlID() == IDC_Title6)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title6, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Title7)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title7, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Title8)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title8, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Title9)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title9, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Title10)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title10, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLightGreenBrush;
	}
	//if (pWnd->GetDlgCtrlID() == IDC_Title11)
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title11, 5);
	//	pDC->SetBkMode(TRANSPARENT);
	//	return vLightGreenBrush;
	//}

	if (pWnd->GetDlgCtrlID() == IDC_ConfirmDigitalInputLine1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ConfirmDigitalInputLine1, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vEjectorNumberEditing == 0)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ActiveHigh1)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_ActiveHigh1, 5);	//using functionbutton for display as is two lines
		pDC->SetBkMode(TRANSPARENT);
		if (vEjectorNumberEditing == 0)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EjectorName1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectorName1, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vEjectorNumberEditing == 0)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ConfirmActiveHigh1)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_ConfirmActiveHigh1, 5);	//using functionbutton for display as is two lines
		pDC->SetBkMode(TRANSPARENT);
		if (vEjectorNumberEditing == 0)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DigitalInputLine1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DigitalInputLine1, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vEjectorNumberEditing == 0)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ConfirmDigitalInputLine2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ConfirmDigitalInputLine2, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vEjectorNumberEditing == 1)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ActiveHigh2)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_ActiveHigh2, 5); //using functionbutton for display as is two lines
		pDC->SetBkMode(TRANSPARENT);
		if (vEjectorNumberEditing == 1)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EjectorName2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectorName2, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vEjectorNumberEditing == 1)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ConfirmActiveHigh2)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_ConfirmActiveHigh2, 5);	//using functionbutton for display as is two lines
		pDC->SetBkMode(TRANSPARENT);
		if (vEjectorNumberEditing == 1)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DigitalInputLine2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DigitalInputLine2, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vEjectorNumberEditing == 1)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ConfirmDigitalInputLine3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ConfirmDigitalInputLine3, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vEjectorNumberEditing == 2)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ActiveHigh3)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_ActiveHigh3, 5);	//using functionbutton for display as is two lines
		pDC->SetBkMode(TRANSPARENT);
		if (vEjectorNumberEditing == 2)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EjectorName3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectorName3, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vEjectorNumberEditing == 2)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ConfirmActiveHigh3)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_ConfirmActiveHigh3, 5);		//using functionbutton for display as is two lines
		pDC->SetBkMode(TRANSPARENT);
		if (vEjectorNumberEditing == 2)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DigitalInputLine3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DigitalInputLine3, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vEjectorNumberEditing == 2)
			return vLocalSystemData->vGreenBrush;
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
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vFunction3ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
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
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vFunction5ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
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
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction2ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
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
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction4ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
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
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction6ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
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
	return hbr;
}

void CNewConfigureSystemEjectorDialog::UpdateDisplay()
{
	if (vGlobalFPGAVersion15Point34OrHigher)
		SetDlgItemText(IDC_SubFunction1Button, _T("Next"));

	SetDlgItemText(IDC_SubFunction2Button, "Edit " + dtoa(vFirstEjectorShown + 1, 0));
	SetDlgItemText(IDC_SubFunction3Button, "Edit " + dtoa(vFirstEjectorShown + 2, 0));
	SetDlgItemText(IDC_SubFunction4Button, "Edit " + dtoa(vFirstEjectorShown + 3, 0));

	if (vDigitalInputLine[0 + vFirstEjectorShown])
		SetDlgItemText(IDC_DigitalInputLine1,vDigitalInputLine[0 + vFirstEjectorShown]);
	else
		SetDlgItemText(IDC_DigitalInputLine1, _T(""));

	if (vDigitalInputLine[1 + vFirstEjectorShown])
		SetDlgItemText(IDC_DigitalInputLine2,vDigitalInputLine[1 + vFirstEjectorShown]);
	else
		SetDlgItemText(IDC_DigitalInputLine2, _T(""));

	if (vDigitalInputLine[2 + vFirstEjectorShown])
		SetDlgItemText(IDC_DigitalInputLine3,vDigitalInputLine[2 + vFirstEjectorShown]);
	else
		SetDlgItemText(IDC_DigitalInputLine3, _T(""));

	if (vName[0 + vFirstEjectorShown].GetLength() > 18)
		m_EjectorName1.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	else
		m_EjectorName1.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);

	if (vName[1 + vFirstEjectorShown].GetLength() > 18)
		m_EjectorName2.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	else
		m_EjectorName2.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);

	if (vName[2 + vFirstEjectorShown].GetLength() > 18)
		m_EjectorName3.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	else
		m_EjectorName3.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);

	SetDlgItemText(IDC_EjectorName1,vName[0 + vFirstEjectorShown]);
	SetDlgItemText(IDC_EjectorName2,vName[1 + vFirstEjectorShown]);
	SetDlgItemText(IDC_EjectorName3,vName[2 + vFirstEjectorShown]);

	if (vConfirmDigitalInputLine[0 + vFirstEjectorShown])
		SetDlgItemText(IDC_ConfirmDigitalInputLine1,vConfirmDigitalInputLine[0 + vFirstEjectorShown]);
	else
		SetDlgItemText(IDC_ConfirmDigitalInputLine1, _T(""));

	if (vConfirmDigitalInputLine[1 + vFirstEjectorShown])
		SetDlgItemText(IDC_ConfirmDigitalInputLine2,vConfirmDigitalInputLine[1 + vFirstEjectorShown]);
	else
		SetDlgItemText(IDC_ConfirmDigitalInputLine2, _T(""));

	if (vConfirmDigitalInputLine[2 + vFirstEjectorShown])
		SetDlgItemText(IDC_ConfirmDigitalInputLine3,vConfirmDigitalInputLine[2 + vFirstEjectorShown]);
	else
		SetDlgItemText(IDC_ConfirmDigitalInputLine3, _T(""));

	if ((vEnabled[0 + vFirstEjectorShown]) && (vDigitalInputLine[0 + vFirstEjectorShown] != "0"))
	{
		m_ActiveHigh1.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_ConfirmActiveHigh1.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		if (vActiveHigh[0 + vFirstEjectorShown])
			SetDlgItemText(IDC_ActiveHigh1, _T("Active High"));
		else
			SetDlgItemText(IDC_ActiveHigh1, _T("Active Low"));
		if (vConfirmDigitalInputLine[0 + vFirstEjectorShown] != "0")
		{
			if (vConfirmActiveHigh[0 + vFirstEjectorShown])
				SetDlgItemText(IDC_ConfirmActiveHigh1, _T("Active High"));
			else
				SetDlgItemText(IDC_ConfirmActiveHigh1, _T("Active Low"));
			m_ConfirmActiveHigh1.EnableWindow(true);
			m_ConfirmDigitalInputLine1.EnableWindow(true);
		}
		else
		{
			m_ConfirmActiveHigh1.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			SetDlgItemText(IDC_ConfirmActiveHigh1, _T("Disabled"));
			m_ConfirmActiveHigh1.EnableWindow(false);
			m_ConfirmDigitalInputLine1.EnableWindow(false);
		}
		m_ActiveHigh1.EnableWindow(true);
		m_DigitalInputLine1.EnableWindow(true);
		m_EjectorName1.EnableWindow(true);
	}
	else
	{
		if (!vEnabled[0 + vFirstEjectorShown])
		{
			m_ActiveHigh1.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			SetDlgItemText(IDC_ActiveHigh1, _T("Disabled"));
		}
		else
		{
			m_ActiveHigh1.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_ActiveHigh1, _T("No Output"));
		}
		m_ConfirmActiveHigh1.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		SetDlgItemText(IDC_ConfirmActiveHigh1, _T("Disabled"));
		m_ActiveHigh1.EnableWindow(false);
		m_DigitalInputLine1.EnableWindow(false);
		m_EjectorName1.EnableWindow(false);
		m_ConfirmActiveHigh1.EnableWindow(false);
		m_ConfirmDigitalInputLine1.EnableWindow(false);
	}

	if ((vEnabled[1 + vFirstEjectorShown]) && (vDigitalInputLine[1 + vFirstEjectorShown] != "0"))
	{
		m_ActiveHigh2.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_ConfirmActiveHigh2.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		if (vActiveHigh[1 + vFirstEjectorShown])
			SetDlgItemText(IDC_ActiveHigh2, _T("Active High"));
		else
			SetDlgItemText(IDC_ActiveHigh2, _T("Active Low"));
		if (vConfirmDigitalInputLine[1 + vFirstEjectorShown] != "0")
		{
			if (vConfirmActiveHigh[1 + vFirstEjectorShown])
				SetDlgItemText(IDC_ConfirmActiveHigh2, _T("Active High"));
			else
				SetDlgItemText(IDC_ConfirmActiveHigh2, _T("Active Low"));
			m_ConfirmActiveHigh2.EnableWindow(true);
			m_ConfirmDigitalInputLine2.EnableWindow(true);
		}
		else
		{
			m_ConfirmActiveHigh2.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			SetDlgItemText(IDC_ConfirmActiveHigh2, _T("Disabled"));
			m_ConfirmActiveHigh2.EnableWindow(false);
			m_ConfirmDigitalInputLine2.EnableWindow(false);
		}
		m_ActiveHigh2.EnableWindow(true);
		m_DigitalInputLine2.EnableWindow(true);
		m_EjectorName2.EnableWindow(true);
	}
	else
	{
		if (!vEnabled[1 + vFirstEjectorShown])
		{
			m_ActiveHigh2.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			SetDlgItemText(IDC_ActiveHigh2, _T("Disabled"));
		}
		else
		{
			m_ActiveHigh2.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_ActiveHigh2, _T("No Output"));
		}
		m_ConfirmActiveHigh2.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		SetDlgItemText(IDC_ConfirmActiveHigh2, _T("Disabled"));
		m_ActiveHigh2.EnableWindow(false);
		m_DigitalInputLine2.EnableWindow(false);
		m_EjectorName2.EnableWindow(false);
		m_ConfirmActiveHigh2.EnableWindow(false);
		m_ConfirmDigitalInputLine2.EnableWindow(false);
	}

	if ((vEnabled[2 + vFirstEjectorShown]) && (vDigitalInputLine[2 + vFirstEjectorShown] != "0"))
	{
		m_ActiveHigh3.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_ConfirmActiveHigh3.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		if (vActiveHigh[2 + vFirstEjectorShown])
			SetDlgItemText(IDC_ActiveHigh3, _T("Active High"));
		else
			SetDlgItemText(IDC_ActiveHigh3, _T("Active Low"));
		if (vConfirmDigitalInputLine[2 + vFirstEjectorShown] != "0")
		{
			if (vConfirmActiveHigh[2 + vFirstEjectorShown])
				SetDlgItemText(IDC_ConfirmActiveHigh3, _T("Active High"));
			else
				SetDlgItemText(IDC_ConfirmActiveHigh3, _T("Active Low"));
			m_ConfirmActiveHigh3.EnableWindow(true);
			m_ConfirmDigitalInputLine3.EnableWindow(true);
		}
		else
		{
			m_ConfirmActiveHigh3.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			SetDlgItemText(IDC_ConfirmActiveHigh3, _T("Disabled"));
			m_ConfirmActiveHigh3.EnableWindow(false);
			m_ConfirmDigitalInputLine3.EnableWindow(false);
		}
		m_ActiveHigh3.EnableWindow(true);
		m_DigitalInputLine3.EnableWindow(true);
		m_EjectorName3.EnableWindow(true);
	}
	else
	{
		if (!vEnabled[2 + vFirstEjectorShown])
		{
			m_ActiveHigh3.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			SetDlgItemText(IDC_ActiveHigh3, _T("Disabled"));
		}
		else
		{
			m_ActiveHigh3.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_ActiveHigh3, _T("No Output"));
		}
		m_ConfirmActiveHigh3.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		SetDlgItemText(IDC_ConfirmActiveHigh3, _T("Disabled"));
		m_DigitalInputLine3.EnableWindow(false);
		m_EjectorName3.EnableWindow(false);
		m_ActiveHigh3.EnableWindow(false);
		m_ConfirmActiveHigh3.EnableWindow(false);
		m_ConfirmDigitalInputLine3.EnableWindow(false);
	}

	if (vEjectorNumberEditing == 0)
		vSubFunction2ButtonEnable = false;	//m_SubFunction2Button.EnableWindow(false);
	else
		vSubFunction2ButtonEnable = true;	//m_SubFunction2Button.EnableWindow(true);

	if (vEjectorNumberEditing == 1)
		vSubFunction3ButtonEnable = false;	//m_SubFunction3Button.EnableWindow(false);
	else
		vSubFunction3ButtonEnable = true;	//m_SubFunction3Button.EnableWindow(true);

	if (vEjectorNumberEditing == 2)
		vSubFunction4ButtonEnable = false;	//m_SubFunction4Button.EnableWindow(false);
	else
		vSubFunction4ButtonEnable = true;	//m_SubFunction4Button.EnableWindow(true);

	if (vEnabled[vEjectorNumberEditing + vFirstEjectorShown])
	{
		SetDlgItemText(IDC_SubFunction8Button, _T("Disable"));
		if (vConfirmDigitalInputLine[vEjectorNumberEditing + vFirstEjectorShown] != "0")
			vFunction3ButtonEnable = true;	//m_Function3Button.EnableWindow(true);
		else
			vFunction3ButtonEnable = false;	//m_Function3Button.EnableWindow(false);
		vFunction5ButtonEnable = true;	//m_Function5Button.EnableWindow(true);
		vSubFunction5ButtonEnable = true;	//m_SubFunction5Button.EnableWindow(true);
		vSubFunction6ButtonEnable = true;	//m_SubFunction6Button.EnableWindow(true);
		vSubFunction7ButtonEnable = true;	//m_SubFunction7Button.EnableWindow(true);
	}
	else
	{
		SetDlgItemText(IDC_SubFunction8Button, _T("Enable"));
		vFunction3ButtonEnable = false;	//m_Function3Button.EnableWindow(false);
		vFunction5ButtonEnable = false;	//m_Function5Button.EnableWindow(false);
		vSubFunction5ButtonEnable = false;	//m_SubFunction5Button.EnableWindow(false);
		vSubFunction6ButtonEnable = false;	//m_SubFunction6Button.EnableWindow(false);
		vSubFunction7ButtonEnable = false;	//m_SubFunction7Button.EnableWindow(false);
	}

	m_Function3Button.Invalidate(false); 
	m_Function5Button.Invalidate(false);
	m_SubFunction2Button.Invalidate(false);
	m_SubFunction3Button.Invalidate(false);
	m_SubFunction4Button.Invalidate(false);
	m_SubFunction5Button.Invalidate(false);
	m_SubFunction6Button.Invalidate(false);
	m_SubFunction7Button.Invalidate(false); 

	if (vActiveHigh[vEjectorNumberEditing + vFirstEjectorShown])
		SetDlgItemText(IDC_SubFunction7Button, _T("Make Active Low"));
	else
		SetDlgItemText(IDC_SubFunction7Button, _T("Make Active High"));

	if (vConfirmActiveHigh[vEjectorNumberEditing + vFirstEjectorShown])
		SetDlgItemText(IDC_Function3Button, _T("Confirm Active Low"));
	else
		SetDlgItemText(IDC_Function3Button, _T("Confirm Active High"));

	if (vUseXofYAlarmOutputForUnconfirmedEject & 1)
	{
		m_ConfirmDigitalInputLine1.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_ConfirmDigitalInputLine1, vConfirmDigitalInputLine[0] + "\nAL2");
	}
	else
	{
		m_ConfirmDigitalInputLine1.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		SetDlgItemText(IDC_ConfirmDigitalInputLine1, vConfirmDigitalInputLine[0]);
	}
	if (vUseXofYAlarmOutputForUnconfirmedEject & 2)
	{
		m_ConfirmDigitalInputLine2.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_ConfirmDigitalInputLine2, vConfirmDigitalInputLine[1] + "\nAL2");
	}
	else
	{
		m_ConfirmDigitalInputLine2.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		SetDlgItemText(IDC_ConfirmDigitalInputLine2, vConfirmDigitalInputLine[1]);
	}
	if (vUseXofYAlarmOutputForUnconfirmedEject & 4)
	{
		m_ConfirmDigitalInputLine3.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_ConfirmDigitalInputLine3, vConfirmDigitalInputLine[2] + "\nAL2");
	}
	else
	{
		m_ConfirmDigitalInputLine3.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		SetDlgItemText(IDC_ConfirmDigitalInputLine3, vConfirmDigitalInputLine[2]);
	}
}

bool CNewConfigureSystemEjectorDialog::ConfigurationOK(BYTE TempDetector)
{
	bool TempOK = true;

	int TempDigitalInputLine = _wtoi(vDigitalInputLine[TempDetector]);

	if ((vEnabled[TempDetector])  && (TempDigitalInputLine == 0))
	{
		TempOK = false;
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nMust enter Digital Input Line when enabled";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	if (TempOK)
	if ((vEnabled[TempDetector])  && (vName[TempDetector] == ""))
	{
		TempOK = false;
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nMust enter Name when enabled";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}

	return TempOK;
}

void CNewConfigureSystemEjectorDialog::SetChangeMade() 
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

void CNewConfigureSystemEjectorDialog::ShowEjectorEditNotEnabledDialogMessage() 
{
	CNoticeDialog TempNoticeDialog;
	CString TempText("");
	TempText = ("Ejector Edit Not Enabled");
	TempNoticeDialog.vNoticeText = "\n\n\nEjector edit Not Enabled";
	TempNoticeDialog.vType = cInformationMessage;
	TempNoticeDialog.DoModal();
}


void CNewConfigureSystemEjectorDialog::ShowEjectorAlreadySelectedDialogMessage() 
{
	CNoticeDialog TempNoticeDialog;
	TempNoticeDialog.vNoticeText = "\n\n\nThis ejector already selected to edit.";
	TempNoticeDialog.vType = cInformationMessage;
	TempNoticeDialog.DoModal();
}

void CNewConfigureSystemEjectorDialog::ShowFunctionNotValidDialogMessage() 
{
	CNoticeDialog TempNoticeDialog;
	CString TempText("");
	TempText = ("Currently Function Not Valid");
	TempNoticeDialog.vNoticeText = "\n\n\nCurrently function not valid";
	TempNoticeDialog.vType = cInformationMessage;
	TempNoticeDialog.DoModal();
}
			
void CNewConfigureSystemEjectorDialog::OnDialogtitlestatictext1()
{
	//bring up soft key menu for help, print screen, Image, Clear, and hidden shift key
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
