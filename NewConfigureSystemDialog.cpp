//ScanTrac Side View Source File
// NewConfigureSystemDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "NewConfigureSystemDialog.h"
#include "NoticeDialog.h"
#include "NumericEntryDialog.h"
#include "SelectItemDialog.h"
#include "DetectorSetupDialog1.h"
#include "ConfigureEMailAddresses.h"
#include "AlphaEntryDialog.h"
#include "PasswordEntryDialog.h"
#include "InspxHiddenSystemDialog.h"
#include "ConfigureRadiationDoorDialog.h"
#include "SystemProductSettingsDialog.h"
#include "InspxHiddenDialog.h"
#include "ProductShapeHandlingDialog.h"
#include "SpellmanDXMXRaySourcePort.h"
#include "SetupSystemExternalHardware.h"
#include "SystemTemperaturesDialog.h"
#include "XRaySource.h"
#include "YesNoDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern bool vGlobalFPGAVersion10Point0OrHigher;
extern CXRaySource *vGlobalMonoBlockXRayPort;
extern CSpellmanDXMXRaySourcePort *vGlobalDXMXRayPort;
extern BYTE vGlobalShiftKeyDown;
extern BYTE vGlobalDemoMode;
extern unsigned long vGlobalDialogTitleColor;
//extern WORD vGlobalUnderRemoteControl;
/////////////////////////////////////////////////////////////////////////////
// CNewConfigureSystemDialog dialog


CNewConfigureSystemDialog::CNewConfigureSystemDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNewConfigureSystemDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewConfigureSystemDialog)
	//}}AFX_DATA_INIT
	vMainWindowPointer = NULL;
	vOneSecondTimerHandle = 122;
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
}

void CNewConfigureSystemDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);


	//make so does not highlight first button
	CWnd *TempDialogPointer = GetDlgItem(IDC_Background);
  GotoDlgCtrl(TempDialogPointer);

	int TimerResult = SetTimer(vOneSecondTimerHandle,1000,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-One Second Timer Failed",cEMailInspx,32000);
	this->SetWindowText( _T("System"));
	::SetupMenu(vLocalCWndCollection);
}

void CNewConfigureSystemDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewConfigureSystemDialog)
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
}


BEGIN_MESSAGE_MAP(CNewConfigureSystemDialog, CDialog)
	//{{AFX_MSG_MAP(CNewConfigureSystemDialog)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_Function5Button, OnFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction1Button, OnSubFunction1Button)
	ON_BN_CLICKED(IDC_SubFunction2Button, OnSubFunction2Button)
	ON_BN_CLICKED(IDC_SubFunction3Button, OnSubFunction3Button)
	ON_BN_CLICKED(IDC_SubFunction5Button, OnSubFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction6Button, OnSubFunction6Button)
	ON_BN_CLICKED(IDC_SubFunction7Button, OnSubFunction7Button)
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_SubFunction4Button, OnSubFunction4Button)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CNewConfigureSystemDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewConfigureSystemDialog message handlers

void CNewConfigureSystemDialog::OnFunction1Button() 
{
	//Main Menu was pressed
		CDialog::EndDialog(10);
}

void CNewConfigureSystemDialog::OnFunction2Button() 
{
	//Save and exit was pressed
	CDialog::EndDialog(true);
}

void CNewConfigureSystemDialog::OnFunction3Button() 
{
	if (vGlobalShiftKeyDown)
	{
		vGlobalShiftKeyDown = false;
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText =  _T("\n\n\nShut Down Computer??");
		TempYesNoDialog.vYesButtonText = "Shut Down Computer!";
		TempYesNoDialog.vNoButtonText = "Cancel";
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
			SystemShutDown(cShutDown);
		UpdateButtons();
	}
	else
	{
		vMainWindowPointer->AskIfShouldExit();
		CDialog::EndDialog(10);
	}
}

void CNewConfigureSystemDialog::OnFunction4Button() 
{
}

void CNewConfigureSystemDialog::OnFunction5Button() 
{
	CSystemTemperaturesDialog ISystemTemperaturesDialog;
	ISystemTemperaturesDialog.vLocalSystemData = vLocalSystemData;
	ISystemTemperaturesDialog.vLocalConfigurationData = vLocalConfigurationData;
	int nResponse = ISystemTemperaturesDialog.DoModal();

	if (nResponse == 10)
	{
		//Main Menu button pressed
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
/*	//setup Auxiliary Detectors Button Pressed
  //Create dialog box
  CNewConfigureSystemExternalDetectorDialog IConfigureSystemBodyTriggersDialog;  
	//Set dialog box data like how many products there are
	IConfigureSystemBodyTriggersDialog.vWindowType = cExternalDetectorWindow;
	IConfigureSystemBodyTriggersDialog.vLocalConfigurationData = vLocalConfigurationData;
	IConfigureSystemBodyTriggersDialog.vLocalSystemData = vLocalSystemData;
	IConfigureSystemBodyTriggersDialog.vMainWindowPointer = vMainWindowPointer;

  //Pass control to dialog box and display
  int nResponse = IConfigureSystemBodyTriggersDialog.DoModal();
  //dialog box is now closed, if user pressed select do this
  //if user pressed cancel, do nothing
  if (nResponse == IDOK)
  {
    // TODO: Place code here to handle when the dialog is accepted
  }
	else 
	if (nResponse == 10)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Main Menu
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
	*/
}

void CNewConfigureSystemDialog::OnSubFunction1Button() 
{

	CSetupSystemExternalHardware ISetupSystemExternalHardware;  

		//Set dialog box data like how many products there are
	ISetupSystemExternalHardware.vMainWindowPointer = vMainWindowPointer;
	ISetupSystemExternalHardware.vLocalConfigurationData = vLocalConfigurationData;
	ISetupSystemExternalHardware.vLocalSystemData = vLocalSystemData;
	ISetupSystemExternalHardware.vLocalProductCollection = vLocalProductCollection;			

	//Pass control to dialog box and display
	int nResponse = ISetupSystemExternalHardware.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == 10)
	{
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}

/*	//setup Container Triggers Button moved to setup system external hardware dialog
  //Create dialog box
  CNewConfigureSystemExternalDetectorDialog IConfigureSystemBodyTriggersDialog;  
	//Set dialog box data like how many products there are
	IConfigureSystemBodyTriggersDialog.vWindowType = cBodyTriggerWindow;
	IConfigureSystemBodyTriggersDialog.vLocalConfigurationData = vLocalConfigurationData;
	IConfigureSystemBodyTriggersDialog.vLocalSystemData = vLocalSystemData;
	IConfigureSystemBodyTriggersDialog.vMainWindowPointer = vMainWindowPointer;

  //Pass control to dialog box and display
  int nResponse = IConfigureSystemBodyTriggersDialog.DoModal();
  //dialog box is now closed, if user pressed select do this
  //if user pressed cancel, do nothing
  if (nResponse == IDOK)
  {
    // TODO: Place code here to handle when the dialog is accepted
  }
	else 
	if (nResponse == 10)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Main Menu
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
	*/
}

void CNewConfigureSystemDialog::OnSubFunction2Button() 
{
/*
	//setup system ejectors
	//Create dialog box
  CNewConfigureSystemEjectorDialog INewConfigureEjectorsDialog;  

	//Set dialog box data like how many products there are
	INewConfigureEjectorsDialog.vLocalConfigurationData = vLocalConfigurationData;
	INewConfigureEjectorsDialog.vLocalSystemData = vLocalSystemData;
		//Pass control to dialog box and display
  int nResponse = INewConfigureEjectorsDialog.DoModal();
  //dialog box is now closed, if user pressed select do this
  //if user pressed cancel, do nothing
  if (nResponse == IDOK)
  {
    // TODO: Place code here to handle when the dialog is accepted
  }
	else 
	if (nResponse == 10)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Main Menu
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
	*/
}

void CNewConfigureSystemDialog::OnSubFunction3Button() 
{
	//radiation Shutter
	//Create dialog box
	CConfigureRadiationDoorDialog IConfigureRadiationDoorsDialog;  
	//Set dialog box data like how many products there are
	IConfigureRadiationDoorsDialog.vLocalConfigurationData = vLocalConfigurationData;
	IConfigureRadiationDoorsDialog.vLocalSystemData = vLocalSystemData;
	IConfigureRadiationDoorsDialog.vMainWindowPointer = vMainWindowPointer;

	//Pass control to dialog box and display
	int nResponse = IConfigureRadiationDoorsDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is accepted
	}
	else 
	if (nResponse == 10)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Main Menu
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
	CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction3Button);
	if (TempWindow)
		TempWindow->Invalidate(false);
}

void CNewConfigureSystemDialog::OnSubFunction4Button() 
{
	//configure edge pixel compensation button pressed
  //Create dialog box
  CDetectorSetupDialog IDetectorSetupDialog;  

  //Set dialog box data like how many products there are
	IDetectorSetupDialog.vLocalSystemData = vLocalSystemData;
	IDetectorSetupDialog.vLocalConfigurationData = vLocalConfigurationData;
	IDetectorSetupDialog.vMainWindowPointer = vMainWindowPointer;
	//Pass control to dialog box and display
  int nResponse = IDetectorSetupDialog.DoModal();
  //dialog box is now closed, if user pressed select do this
  //if user pressed cancel, do nothing
  if (nResponse == IDOK)
  {
	  // TODO: Place code here to handle when the dialog is closed
  }
	else if (nResponse == 10)
	{
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CNewConfigureSystemDialog::OnSubFunction5Button() 
{ 
		//Create diagnostic dialog box
		CProductShapeHandlingDialog IProductShapeHandlingDialog;  
		//Set dialog box data like how many products there are
		IProductShapeHandlingDialog.vLocalSystemData = vLocalSystemData;
		IProductShapeHandlingDialog.vLocalConfigurationData = vLocalConfigurationData;
		//IProductShapeHandlingDialog.vMainWindowPointer = vMainWindowPointer;

		//Pass control to dialog box and display
		int nResponse = IProductShapeHandlingDialog.DoModal();
		//if changes were made, save data to file
		//if (nResponse == IDOK)
		//{
		//}
		if (nResponse == 10)
		{
			//Main Menu Button Pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
}

void CNewConfigureSystemDialog::OnSubFunction6Button() 
{ 
	//System Product Settings
	//Create dialog box
	CSystemProductSettingsDialog ISystemProductSettingsDialog;  
	//Set dialog box data like how many products there are
	ISystemProductSettingsDialog.vLocalConfigurationData = vLocalConfigurationData;
	ISystemProductSettingsDialog.vLocalSystemData = vLocalSystemData;
	ISystemProductSettingsDialog.vMainWindowPointer = vMainWindowPointer;

	//Pass control to dialog box and display
	int nResponse = ISystemProductSettingsDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is accepted
	}
	else 
	if (nResponse == 10)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Main Menu
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CNewConfigureSystemDialog::OnSubFunction7Button() 
{ 
	DoInspxHiddenSystemSetup();
}

void CNewConfigureSystemDialog::OnSubFunction8Button() 
{
	// Hardware options setup
	DoInspxHiddenSetup();
}

BOOL CNewConfigureSystemDialog::PreTranslateMessage(MSG* pMsg) 
{
	CWnd * TempWindow = NULL;
	if (pMsg->message == WM_KEYDOWN)
	{
		int TempKey = pMsg->wParam;
		if (vGlobalShiftKeyDown)
		{
			if (pMsg->wParam == 109) // - Key - Sub Function 7  Decimal Point:
				OnFunction3Button();
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
			UpdateButtons();
		}
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
				return true;
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
				UpdateButtons();
			break;
			case cPageDown: // Help
				PostThreadMessage(vGlobalCallingThreadID,
					cShowHelp, 12, 0);
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
	else
	if (pMsg->message == cUpdateTitleBarMessage)  //the hidden key was pressed or cleared so update buttons and title bar
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
		UpdateButtons();
		return true;  //true indicates it has been handled, so do not process
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CNewConfigureSystemDialog::UpdateButtons()
{
	CWnd * TempWindow = NULL;
	TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
	if (TempWindow)
		TempWindow->Invalidate(false);
			//if ((vGlobalShiftKeyDown) && (PasswordOK(cTemporaryInspxPassword,false)) && (vLocalSystemData->vITIPCDig->HaveUndoAddToLearnImage())) //undo last add to learn if shift key down
	if (vGlobalShiftKeyDown)
		SetDlgItemText(IDC_Function3Button,  _T("Shut Down Computer"));
	else
		SetDlgItemText(IDC_Function3Button,  _T("Exit \nScanTrac"));
}

HBRUSH CNewConfigureSystemDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		pDC->SetTextColor(vGlobalDialogTitleColor);
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);
		if (vGlobalShiftKeyDown)
			pDC->SetTextColor(cBlack);
		else
			pDC->SetTextColor(vGlobalDialogTitleColor);
	}

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

		if (vLocalSystemData->vRadiationDoorManualControlRegister)
		{
				pDC->SetBkMode(TRANSPARENT);
				return vLocalSystemData->vGreenBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
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
	return hbr;
}

void CNewConfigureSystemDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnTimer(nIDEvent);
}

void CNewConfigureSystemDialog::DoInspxHiddenSystemSetup()
{
	//Create Additional System Setup dialog box
	CInspxHiddenSystemDialog IAdditionalSystemSetupDialog;  
	IAdditionalSystemSetupDialog.vMainWindowPointer = vMainWindowPointer;
	//Set dialog box data like how many products there are
	IAdditionalSystemSetupDialog.vLocalSystemData = vLocalSystemData;
	IAdditionalSystemSetupDialog.vLocalConfigurationData = vLocalConfigurationData;
	//IAdditionalSystemSetupDialog.vMainWindowPointer = this;

	//Pass control to dialog box and display
	int nResponse = IAdditionalSystemSetupDialog.DoModal();

	//if changes were made, save data to file
	if (nResponse == IDOK)
	{
	}
	if (nResponse == 10)
	{
		//Main Menu button pressed
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CNewConfigureSystemDialog::DoInspxHiddenSetup()
{
	if (PasswordOK(cTemporaryInspxPassword, true))
	{
		//Create Special Setup dialog box
		CInspxHiddenDialog IInspxHiddenDialog;  
		//Set dialog box data like how many products there are
		IInspxHiddenDialog.vLocalSystemData = vLocalSystemData;
		IInspxHiddenDialog.vLocalConfigurationData = vLocalConfigurationData;
		//IDiagnosticDialog.vMainWindowPointer = this;

		//Pass control to dialog box and display
		int nResponse = IInspxHiddenDialog.DoModal();

		//if changes were made, save data to file
		if (nResponse == IDOK)
		{
		}
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
		if (vLocalConfigurationData->vSimulateLightControlBoard)
		{
			if (vGlobalFPGAVersion10Point0OrHigher)
			{
				vLocalSystemData->vTemperatureSurface = cuCNormalVoltageValue;
				vLocalSystemData->vTemperatureLiquid = cuCNormalVoltageValue;
				vLocalSystemData->vTemperatureAirInside = cuCNormalVoltageValue;
				vLocalSystemData->vTemperatureuC = cuCNormalVoltageValue;
				vLocalSystemData->vTemperatureAirOutside = cuCNormalVoltageValue;  
				//vLocalSystemData->vCoolantPressure = cuCNormalVoltageValue;  
			}
			else
			{
				vLocalSystemData->vTemperatureSurface = 193;
				vLocalSystemData->vTemperatureLiquid = 193;
				vLocalSystemData->vTemperatureAirInside = 193;
				vLocalSystemData->vTemperatureuC = 193;
				vLocalSystemData->vTemperatureAirOutside = 193;  
				//vLocalSystemData->vCoolantPressure = 193; 
			}
			
			//vLocalSystemData->vPSFault = 254;
			if (vLocalSystemData->vXRayOnNotice)
				vLocalSystemData->vRawInterlockInput = 0xFF7F;
			else
				vLocalSystemData->vRawInterlockInput = 0xFF77;
			vLocalSystemData->vOpenInterlock = "";

			vLocalSystemData->vSourceInterLocks = vLocalSystemData->vRawInterlockInput;
		}

		vGlobalDemoMode = vLocalConfigurationData->vDemoMode;
	}
}



void CNewConfigureSystemDialog::OnDialogtitlestatictext1()
{
	//bring up soft key menu for help, print screen, Image, Clear, and hidden shift key
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
