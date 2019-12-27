//ScanTrac Side View Source File
// NewConfigureSystemExternalDetectorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "NewConfigureSystemExternalDetectorDialog.h"
#include "NumericEntryDialog.h"
#include "AlphaEntryDialog.h"
#include "NoticeDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
/////////////////////////////////////////////////////////////////////////////
// CNewConfigureSystemExternalDetectorDialog dialog


CNewConfigureSystemExternalDetectorDialog::CNewConfigureSystemExternalDetectorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNewConfigureSystemExternalDetectorDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewConfigureSystemExternalDetectorDialog)
	//}}AFX_DATA_INIT
	vMainWindowPointer = NULL;
	vChangeMade = false;
	vDetectorNumberEditing = 0;
	vFirstShown = 0;
	vWindowType = cExternalDetectorWindow;
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_DigitalInputLine4);
	vLocalCWndCollection.Add(&m_DigitalInputLine3);
	vLocalCWndCollection.Add(&m_DigitalInputLine2);
	vLocalCWndCollection.Add(&m_DigitalInputLine1);
	vLocalCWndCollection.Add(&m_DetectorName4);
	vLocalCWndCollection.Add(&m_DetectorName3);
	vLocalCWndCollection.Add(&m_DetectorName2);
	vLocalCWndCollection.Add(&m_DetectorName1);
	vLocalCWndCollection.Add(&m_ActiveHigh4);
	vLocalCWndCollection.Add(&m_ActiveHigh3);
	vLocalCWndCollection.Add(&m_ActiveHigh2);
	vLocalCWndCollection.Add(&m_ActiveHigh1);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Title1);
	vLocalCWndCollection.Add(&m_Title6);
	vLocalCWndCollection.Add(&m_Title8);
}

void CNewConfigureSystemExternalDetectorDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);
	vSubFunction1ButtonEnable = true;
	vSubFunction2ButtonEnable = true;
	vSubFunction3ButtonEnable = true;
	vSubFunction4ButtonEnable = true;
	vSubFunction5ButtonEnable = true;
	vSubFunction6ButtonEnable = true;
	vSubFunction7ButtonEnable = true;
	if (vWindowType == cBodyTriggerWindow)
	{
		for (BYTE TempLoop = 0; TempLoop < cNumberOfBodyTriggers; TempLoop++)
		{
			vName[TempLoop] = vLocalConfigurationData->vBodyTrigger[TempLoop].vName;
			vDigitalInputLine[TempLoop] = dtoa(vLocalConfigurationData->vBodyTrigger[TempLoop].vDigitalInputLine, 0);
			vEnabled[TempLoop] = vLocalConfigurationData->vBodyTrigger[TempLoop].vEnabled;
			vActiveHigh[TempLoop] = vLocalConfigurationData->vBodyTrigger[TempLoop].vActiveHigh;
		}
		SetDlgItemText(IDC_DialogTitleStaticText7, _T("Setup System Container Triggers"));
	}
	else
	{  //window is to edit system Auxiliary detectors parameters
		for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
		{
			vName[TempLoop] = vLocalConfigurationData->vExternalDetector[TempLoop].vName;
			vDigitalInputLine[TempLoop] = dtoa(vLocalConfigurationData->vExternalDetector[TempLoop].vDigitalInputLine, 0);

			vEnabled[TempLoop] = vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled;
			vActiveHigh[TempLoop] = vLocalConfigurationData->vExternalDetector[TempLoop].vActiveHigh;
		}
		SetDlgItemText(IDC_DialogTitleStaticText7, _T("Setup System Auxiliary Detectors"));
	}
	UpdateDisplay();
	this->SetWindowText(_T("Auxiliary Detectors"));
	::SetupMenu(vLocalCWndCollection);
}

void CNewConfigureSystemExternalDetectorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewConfigureSystemExternalDetectorDialog)
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_ActiveHigh1, m_ActiveHigh1);
	DDX_Control(pDX, IDC_DetectorName1, m_DetectorName1);
	DDX_Control(pDX, IDC_DigitalInputLine1, m_DigitalInputLine1);
	DDX_Control(pDX, IDC_ActiveHigh2, m_ActiveHigh2);
	DDX_Control(pDX, IDC_DetectorName2, m_DetectorName2);
	DDX_Control(pDX, IDC_DigitalInputLine2, m_DigitalInputLine2);
	DDX_Control(pDX, IDC_ActiveHigh3, m_ActiveHigh3);
	DDX_Control(pDX, IDC_DetectorName3, m_DetectorName3);
	DDX_Control(pDX, IDC_DigitalInputLine3, m_DigitalInputLine3);
	DDX_Control(pDX, IDC_ActiveHigh4, m_ActiveHigh4);
	DDX_Control(pDX, IDC_DetectorName4, m_DetectorName4);
	DDX_Control(pDX, IDC_DigitalInputLine4, m_DigitalInputLine4);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DialogTitleStaticText7, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Title1, m_Title1);
	DDX_Control(pDX, IDC_Title6, m_Title6);
	DDX_Control(pDX, IDC_Title8, m_Title8);
}


BEGIN_MESSAGE_MAP(CNewConfigureSystemExternalDetectorDialog, CDialog)
	//{{AFX_MSG_MAP(CNewConfigureSystemExternalDetectorDialog)
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
	ON_STN_CLICKED(IDC_DialogTitleStaticText7, OnDialogtitlestatictext7)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewConfigureSystemExternalDetectorDialog message handlers

void CNewConfigureSystemExternalDetectorDialog::OnFunction1Button() 
{
	//Main Menu was pressed
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::EndDialog(10);
}

void CNewConfigureSystemExternalDetectorDialog::OnFunction2Button() 
{
	//Save and exit was pressed
	if (ConfigurationOK(vDetectorNumberEditing + vFirstShown))
	{
		if ((!vChangeMade) || (KeepSetupChanges()))
		{
			if (vChangeMade)
				vLocalSystemData->vCurrentProductChanged = true;
			if (vWindowType == cBodyTriggerWindow)
			{
				BYTE TempLoop = 0;
				for (; TempLoop < cNumberOfBodyTriggers; TempLoop++)
				{
					vLocalConfigurationData->vBodyTrigger[TempLoop].vName = vName[TempLoop];
					vLocalConfigurationData->vBodyTrigger[TempLoop].SetDigitalInputLine(_wtoi(vDigitalInputLine[TempLoop]));
					vLocalConfigurationData->vBodyTrigger[TempLoop].vEnabled = vEnabled[TempLoop];
					vLocalConfigurationData->vBodyTrigger[TempLoop].vActiveHigh = vActiveHigh[TempLoop];
				}
				vMainWindowPointer->SendBodyTriggerInverterControl();
			}
			else
			{  //window is to edit system Auxiliary detectors parameters
				BYTE TempLoop = 0;
				for (; TempLoop < cNumberOfExternalDetectors; TempLoop++)
				{
					vLocalConfigurationData->vExternalDetector[TempLoop].vName = vName[TempLoop];
					vLocalConfigurationData->vExternalDetector[TempLoop].SetDigitalInputLine(_wtoi(vDigitalInputLine[TempLoop]));
					vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled = vEnabled[TempLoop];
					vLocalConfigurationData->vExternalDetector[TempLoop].vActiveHigh = vActiveHigh[TempLoop];
				}
			}
			CDialog::EndDialog(true);
		}
	}
}


void CNewConfigureSystemExternalDetectorDialog::OnFunction3Button() 
{
	//Next / Previous button was pressed
	if (ConfigurationOK(vDetectorNumberEditing + vFirstShown))
	{
		if (vFirstShown == 0)
			vFirstShown = 1;
		else
			vFirstShown = 0;

		if (vFirstShown == 0)
			SetDlgItemText(IDC_Function3Button, _T("Next"));
		else
			SetDlgItemText(IDC_Function3Button, _T("Previous"));

		UpdateDisplay();
	}
}

void CNewConfigureSystemExternalDetectorDialog::OnFunction4Button() 
{
	//Cancel was pressed
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::OnCancel();
}

void CNewConfigureSystemExternalDetectorDialog::OnFunction5Button() 
{
	// Help Button Was Pressed
	CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText7);
	if (TempWindow)
		TempWindow->Invalidate(false);
}

void CNewConfigureSystemExternalDetectorDialog::OnSubFunction1Button() 
{
	if (vSubFunction1ButtonEnable == true)
	{
	//Select ExternalDetector 1 button pressed
	if (ConfigurationOK(vDetectorNumberEditing + vFirstShown))
	{
		vDetectorNumberEditing = 0;
		UpdateDisplay();
	}
	}
	else
	{
		ShowDetectorAlreadySelectedDialogMessage();
	}
}

void CNewConfigureSystemExternalDetectorDialog::OnSubFunction2Button() 
{
	if (vSubFunction2ButtonEnable == true)
	{
	//Select ExternalDetector 2 button pressed
	if (ConfigurationOK(vDetectorNumberEditing + vFirstShown))
	{
		vDetectorNumberEditing = 1;
		UpdateDisplay();
	}
	}
	else
	{
		ShowDetectorAlreadySelectedDialogMessage();
	}
}

void CNewConfigureSystemExternalDetectorDialog::OnSubFunction3Button() 
{
	if (vSubFunction3ButtonEnable == true)
	{
	//Select ExternalDetector 3 button pressed
	if (ConfigurationOK(vDetectorNumberEditing + vFirstShown))
	{
		vDetectorNumberEditing = 2;
		UpdateDisplay();
	}
	}
	else
	{
		ShowDetectorAlreadySelectedDialogMessage();
	}
}

void CNewConfigureSystemExternalDetectorDialog::OnSubFunction4Button() 
{
	//Select ExternalDetector 4 button pressed
	if (vSubFunction4ButtonEnable == true)
	{
	if (ConfigurationOK(vDetectorNumberEditing + vFirstShown))
	{
		vDetectorNumberEditing = 3;
		UpdateDisplay();
	}
	}
	else
	{
		ShowDetectorEditNotEnabledDialog();
	}
}

void CNewConfigureSystemExternalDetectorDialog::OnSubFunction5Button() 
{
	if (vSubFunction5ButtonEnable == true)
	{
	//Edit Digital Input Line Button Was Pressed
	if (vEnabled[vDetectorNumberEditing + vFirstShown])
	{
		CNumericEntryDialog INumericEntryDialog;
		
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = vDigitalInputLine[vDetectorNumberEditing + vFirstShown];
		INumericEntryDialog.m_DialogTitleStaticText1 = "Digital Input Line For";
		if (vWindowType == cBodyTriggerWindow)
			INumericEntryDialog.m_DialogTitleStaticText2 = "System Container Trigger " + dtoa(vDetectorNumberEditing + vFirstShown + 1, 0);
		else
			INumericEntryDialog.m_DialogTitleStaticText2 = "System Auxiliary Detector " + dtoa(vDetectorNumberEditing + vFirstShown + 1, 0);
		INumericEntryDialog.m_DialogTitleStaticText3 = vName[vDetectorNumberEditing + vFirstShown];
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + vDigitalInputLine[vDetectorNumberEditing + vFirstShown];

		INumericEntryDialog.m_UnitsString = "Zero Indicates No Detector Connected";
		INumericEntryDialog.vMaxValue = 8;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vDigitalInputLine[vDetectorNumberEditing + vFirstShown] = INumericEntryDialog.vEditString;

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
		ShowDetectorEditNotEnabledDialog();
	}
}

void CNewConfigureSystemExternalDetectorDialog::OnSubFunction6Button() 
{
	if (vSubFunction6ButtonEnable == true)
	{
  //Edit Detector Name button was pressed
	if (vEnabled[vDetectorNumberEditing + vFirstShown])
	{
		CAlphaEntryDialog IAlphaEntryDialog;  
		IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
		IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
		//Set dialog box data titles and number value
		IAlphaEntryDialog.vEditString = vName[vDetectorNumberEditing + vFirstShown];

		if (vWindowType == cBodyTriggerWindow)
			IAlphaEntryDialog.m_DialogTitleStaticText = "System Container Trigger " + dtoa(vDetectorNumberEditing + vFirstShown + 1, 0);
		else
			IAlphaEntryDialog.m_DialogTitleStaticText = "System Auxiliary Detector " + dtoa(vDetectorNumberEditing + vFirstShown + 1, 0);

		IAlphaEntryDialog.m_DialogTitleStaticText2 = "Original Value: " + vName[vDetectorNumberEditing + vFirstShown];
		//Pass control to dialog box and display
		int nResponse = IAlphaEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vName[vDetectorNumberEditing + vFirstShown] = IAlphaEntryDialog.vEditString;
			SetChangeMade();
			SetDlgItemText(IDC_DetectorName1,vName[0 + vFirstShown]);
			SetDlgItemText(IDC_DetectorName2,vName[1 + vFirstShown]);
			SetDlgItemText(IDC_DetectorName3,vName[2 + vFirstShown]);
			SetDlgItemText(IDC_DetectorName4,vName[3 + vFirstShown]);
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
		ShowDetectorEditNotEnabledDialog();
	}
}

void CNewConfigureSystemExternalDetectorDialog::OnSubFunction7Button() 
{
	if (vSubFunction7ButtonEnable == true)
	{
	//Toggle Active High button Was pressed
	if (vEnabled[vDetectorNumberEditing + vFirstShown])
	{
		vActiveHigh[vDetectorNumberEditing + vFirstShown] =
			!vActiveHigh[vDetectorNumberEditing + vFirstShown];
		UpdateDisplay();
		SetChangeMade();
	}
	}
	else
	{
		ShowDetectorAlreadySelectedDialogMessage();
	}
}

void CNewConfigureSystemExternalDetectorDialog::OnSubFunction8Button() 
{
	//Toggle Enable button Was pressed
	vEnabled[vDetectorNumberEditing + vFirstShown] = !vEnabled[vDetectorNumberEditing + vFirstShown];
	SetChangeMade();
	UpdateDisplay();
}

void CNewConfigureSystemExternalDetectorDialog::DisplaySubFunction1To4()
{
	CString TempTypeString("Edit ");
	SetDlgItemText(IDC_SubFunction1Button,TempTypeString + dtoa(vFirstShown + 1, 0));
	SetDlgItemText(IDC_SubFunction2Button,TempTypeString + dtoa(vFirstShown + 2, 0));
	SetDlgItemText(IDC_SubFunction3Button,TempTypeString + dtoa(vFirstShown + 3, 0));
	SetDlgItemText(IDC_SubFunction4Button,TempTypeString + dtoa(vFirstShown + 4, 0));
}

BOOL CNewConfigureSystemExternalDetectorDialog::PreTranslateMessage(MSG* pMsg) 
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
				if (vWindowType == cExternalDetectorWindow)
					PostThreadMessage(vGlobalCallingThreadID,
						cShowHelp, 14, 0);
				else
					PostThreadMessage(vGlobalCallingThreadID,
						cShowHelp, 45, 0);
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText7);
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

HBRUSH CNewConfigureSystemExternalDetectorDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText7)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_DigitalInputLine1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DigitalInputLine1, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vDetectorNumberEditing == 0)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ActiveHigh1) 
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_ActiveHigh1, 5);	//using functionbutton for display as is two lines
		pDC->SetBkMode(TRANSPARENT);
		if (vDetectorNumberEditing == 0)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorName1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorName1, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vDetectorNumberEditing == 0)
			return vLocalSystemData->vGreenBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_DigitalInputLine2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DigitalInputLine2, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vDetectorNumberEditing == 1)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ActiveHigh2)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_ActiveHigh2, 5);	//using functionbutton for display as is two lines
		pDC->SetBkMode(TRANSPARENT);
		if (vDetectorNumberEditing == 1)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorName2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorName2, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vDetectorNumberEditing == 1)
			return vLocalSystemData->vGreenBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_DigitalInputLine3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DigitalInputLine3, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vDetectorNumberEditing == 2)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ActiveHigh3)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_ActiveHigh3, 5);	//using functionbutton for display as is two lines
		pDC->SetBkMode(TRANSPARENT);
		if (vDetectorNumberEditing == 2)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorName3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorName3, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vDetectorNumberEditing == 2)
			return vLocalSystemData->vGreenBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_DigitalInputLine4)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DigitalInputLine4, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vDetectorNumberEditing == 3)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ActiveHigh4)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_ActiveHigh4, 5);		//using functionbutton for display as is two lines
		pDC->SetBkMode(TRANSPARENT);
		if (vDetectorNumberEditing == 3)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorName4)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorName4, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vDetectorNumberEditing == 3)
			return vLocalSystemData->vGreenBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Title1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title1, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	//if (pWnd->GetDlgCtrlID() == IDC_Title2)
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title2, 5);
	//	pDC->SetBkMode(TRANSPARENT);
	//	return vLocalSystemData->vLightGreenBrush;
	//}
	//if (pWnd->GetDlgCtrlID() == IDC_Title4)
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title4, 5);
	//	pDC->SetBkMode(TRANSPARENT);
	//	return vLocalSystemData->vLightGreenBrush;
	//}
	if (pWnd->GetDlgCtrlID() == IDC_Title6)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title6, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Title8)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title8, 5);
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
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
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
	
	return hbr;
}

void CNewConfigureSystemExternalDetectorDialog::UpdateDisplay()
{
	DisplaySubFunction1To4();

	if (vDigitalInputLine[0 + vFirstShown])
		SetDlgItemText(IDC_DigitalInputLine1,vDigitalInputLine[0 + vFirstShown]);
	else
		SetDlgItemText(IDC_DigitalInputLine1, _T(""));

	if (vDigitalInputLine[0 + vFirstShown])
		SetDlgItemText(IDC_DigitalInputLine2,vDigitalInputLine[1 + vFirstShown]);
	else
		SetDlgItemText(IDC_DigitalInputLine2, _T(""));

	if (vDigitalInputLine[0 + vFirstShown])
		SetDlgItemText(IDC_DigitalInputLine3,vDigitalInputLine[2 + vFirstShown]);
	else
		SetDlgItemText(IDC_DigitalInputLine3, _T(""));

	if (vDigitalInputLine[0 + vFirstShown])
		SetDlgItemText(IDC_DigitalInputLine4,vDigitalInputLine[3 + vFirstShown]);
	else
		SetDlgItemText(IDC_DigitalInputLine4, _T(""));

	SetDlgItemText(IDC_DetectorName1,vName[0 + vFirstShown]);
	SetDlgItemText(IDC_DetectorName2,vName[1 + vFirstShown]);
	SetDlgItemText(IDC_DetectorName3,vName[2 + vFirstShown]);
	SetDlgItemText(IDC_DetectorName4,vName[3 + vFirstShown]);

	if ((vEnabled[0 + vFirstShown]) && (vDigitalInputLine[0 + vFirstShown] != "0"))
	{
		m_ActiveHigh1.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		if (vActiveHigh[0 + vFirstShown])
			SetDlgItemText(IDC_ActiveHigh1, _T("Active High"));
		else
			SetDlgItemText(IDC_ActiveHigh1, _T("Active Low"));
		m_ActiveHigh1.EnableWindow(true);
		m_DigitalInputLine1.EnableWindow(true);
		m_DetectorName1.EnableWindow(true);
	}
	else
	{
		m_ActiveHigh1.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		if (!vEnabled[0 + vFirstShown])
			SetDlgItemText(IDC_ActiveHigh1, _T("Disabled"));
		else
			SetDlgItemText(IDC_ActiveHigh1, _T("No Input"));
		m_ActiveHigh1.EnableWindow(false);
		m_DigitalInputLine1.EnableWindow(false);
		m_DetectorName1.EnableWindow(false);
	}

	if ((vEnabled[1 + vFirstShown]) && (vDigitalInputLine[1 + vFirstShown] != "0"))
	{
		m_ActiveHigh2.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		if (vActiveHigh[1 + vFirstShown])
			SetDlgItemText(IDC_ActiveHigh2, _T("Active High"));
		else
			SetDlgItemText(IDC_ActiveHigh2, _T("Active Low"));
		m_ActiveHigh2.EnableWindow(true);
		m_DigitalInputLine2.EnableWindow(true);
		m_DetectorName2.EnableWindow(true);
	}
	else
	{
		m_ActiveHigh2.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		if (!vEnabled[1 + vFirstShown])
			SetDlgItemText(IDC_ActiveHigh2, _T("Disabled"));
		else
			SetDlgItemText(IDC_ActiveHigh2, _T("No Input"));
		m_ActiveHigh2.EnableWindow(false);
		m_DigitalInputLine2.EnableWindow(false);
		m_DetectorName2.EnableWindow(false);
	}

	if ((vEnabled[2 + vFirstShown]) && (vDigitalInputLine[2 + vFirstShown] != "0"))
	{
		m_ActiveHigh3.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		if (vActiveHigh[2 + vFirstShown])
			SetDlgItemText(IDC_ActiveHigh3, _T("Active High"));
		else
			SetDlgItemText(IDC_ActiveHigh3, _T("Active Low"));
		m_ActiveHigh3.EnableWindow(true);
		m_DigitalInputLine3.EnableWindow(true);
		m_DetectorName3.EnableWindow(true);
	}
	else
	{
		m_ActiveHigh3.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		if (!vEnabled[2 + vFirstShown])
			SetDlgItemText(IDC_ActiveHigh3, _T("Disabled"));
		else
			SetDlgItemText(IDC_ActiveHigh3, _T("No Input"));
		m_ActiveHigh3.EnableWindow(false);
		m_DigitalInputLine3.EnableWindow(false);
		m_DetectorName3.EnableWindow(false);
	}

	if ((vEnabled[3 + vFirstShown]) && (vDigitalInputLine[3 + vFirstShown] != "0"))
	{
		m_ActiveHigh4.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		if (vActiveHigh[3 + vFirstShown])
			SetDlgItemText(IDC_ActiveHigh4, _T("Active High"));
		else
			SetDlgItemText(IDC_ActiveHigh4, _T("Active Low"));
		m_ActiveHigh4.EnableWindow(true);
		m_DigitalInputLine4.EnableWindow(true);
		m_DetectorName4.EnableWindow(true);
	}
	else
	{
		m_ActiveHigh4.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		if (!vEnabled[3 + vFirstShown])
			SetDlgItemText(IDC_ActiveHigh4, _T("Disabled"));
		else
			SetDlgItemText(IDC_ActiveHigh4, _T("No Input"));
		m_ActiveHigh4.EnableWindow(false);
		m_DigitalInputLine4.EnableWindow(false);
		m_DetectorName4.EnableWindow(false);
	}

	if (vDetectorNumberEditing == 0)
		vSubFunction1ButtonEnable = false;	//m_SubFunction1Button.EnableWindow(false);
	else
		vSubFunction1ButtonEnable = true;	//m_SubFunction1Button.EnableWindow(true);

	if (vDetectorNumberEditing == 1)
		vSubFunction2ButtonEnable = false;	//m_SubFunction2Button.EnableWindow(false);
	else
		vSubFunction2ButtonEnable = true;	//m_SubFunction2Button.EnableWindow(true);

	if (vDetectorNumberEditing == 2)
		vSubFunction3ButtonEnable = false;	//m_SubFunction3Button.EnableWindow(false);
	else
		vSubFunction3ButtonEnable = true;	//m_SubFunction3Button.EnableWindow(true);

	if (vDetectorNumberEditing == 3)
		vSubFunction4ButtonEnable = false;	//m_SubFunction4Button.EnableWindow(false);
	else
		vSubFunction4ButtonEnable = true;	//m_SubFunction4Button.EnableWindow(true);

	if (vEnabled[vDetectorNumberEditing + vFirstShown])
	{
		SetDlgItemText(IDC_SubFunction8Button, _T("Disable"));
		vSubFunction5ButtonEnable = true;	//m_SubFunction5Button.EnableWindow(true);
		vSubFunction6ButtonEnable = true;	//m_SubFunction6Button.EnableWindow(true);
		vSubFunction7ButtonEnable = true;	//m_SubFunction7Button.EnableWindow(true);
	}
	else
	{
		SetDlgItemText(IDC_SubFunction8Button, _T("Enable"));
		vSubFunction5ButtonEnable = false;	//m_SubFunction5Button.EnableWindow(false);
		vSubFunction6ButtonEnable = false;	//m_SubFunction6Button.EnableWindow(false);
		vSubFunction7ButtonEnable = false;	//m_SubFunction7Button.EnableWindow(false);
	}
	m_SubFunction1Button.Invalidate(false);
	m_SubFunction2Button.Invalidate(false);
	m_SubFunction3Button.Invalidate(false);
	m_SubFunction4Button.Invalidate(false);
	m_SubFunction5Button.Invalidate(false);
	m_SubFunction6Button.Invalidate(false);
	m_SubFunction7Button.Invalidate(false);

	if (vActiveHigh[vDetectorNumberEditing + vFirstShown])
		SetDlgItemText(IDC_SubFunction7Button, _T("Make Active Low"));
	else
		SetDlgItemText(IDC_SubFunction7Button, _T("Make Active High"));
}

bool CNewConfigureSystemExternalDetectorDialog::ConfigurationOK(BYTE TempDetector)
{
	bool TempOK = true;

	int TempDigitalInputLine = _wtoi(vDigitalInputLine[TempDetector]);

	if (vEnabled[TempDetector])  
	{
		if (TempDigitalInputLine == 0)
		{
			TempOK = false;
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nMust enter Digital Input Line when enabled";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		if (TempOK)
		if (vName[TempDetector] == "")
		{
			TempOK = false;
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nMust enter Name when enabled";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}

	return TempOK;
}

void CNewConfigureSystemExternalDetectorDialog::SetChangeMade() 
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

void CNewConfigureSystemExternalDetectorDialog::ShowDetectorEditNotEnabledDialog()
{
	CNoticeDialog TempNoticeDialog;
	TempNoticeDialog.vNoticeText = "\n\n\nDetector edit not Enabled";
	TempNoticeDialog.vType = cErrorMessage;
	TempNoticeDialog.DoModal();
}

void CNewConfigureSystemExternalDetectorDialog::ShowDetectorAlreadySelectedDialogMessage() 
{
	CNoticeDialog TempNoticeDialog;
	TempNoticeDialog.vNoticeText = "\n\n\nThis Detector already selected to edit.";
	TempNoticeDialog.vType = cErrorMessage;
	TempNoticeDialog.DoModal();
}

void CNewConfigureSystemExternalDetectorDialog::OnDialogtitlestatictext7()
{
	//bring up soft key menu for help, print screen, Image, Clear, and hidden shift key
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
