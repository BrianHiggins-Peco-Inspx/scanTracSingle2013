//ScanTrac Side View Source File
// ConfigureProductBodyTriggerDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "ConfigureProductBodyTriggerDialog.h"
#include "SelectItemDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
IMPLEMENT_DYNCREATE(CConfigureProductBodyTriggerDialog, CDialog)

extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
/////////////////////////////////////////////////////////////////////////////
// CConfigureProductBodyTriggerDialog dialog


CConfigureProductBodyTriggerDialog::CConfigureProductBodyTriggerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigureProductBodyTriggerDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigureProductBodyTriggerDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	vChangeMade = false;
	vShowExit = 0;	
	vBodyTriggerToUse = 0;
	vBackupBodyTriggerToUse = 0;
	vInAutoSetup = false;
	vNumberOfBodyTriggersInSystem = 0;
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_BackupBodyTriggerToUse);
	vLocalCWndCollection.Add(&m_BackupBodyTriggerToUseLabel);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_BodyTriggerName);
	vLocalCWndCollection.Add(&m_BodyTriggerToUseLabel);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_ProductNameTitleStaticText);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
}

void CConfigureProductBodyTriggerDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);
	CString TempText("");

	for (BYTE TempLoop = 0; TempLoop < cNumberOfBodyTriggers; TempLoop++)
	{
		if ((vLocalConfigurationData->vBodyTrigger[TempLoop].vEnabled) &&
			(vLocalConfigurationData->vBodyTrigger[TempLoop].vDigitalInputLine))
			vNumberOfBodyTriggersInSystem++;
	}
	if (vNumberOfBodyTriggersInSystem > 1)
	{
		m_BackupBodyTriggerToUseLabel.ShowWindow(SW_SHOW);
		m_BackupBodyTriggerToUse.ShowWindow(SW_SHOW);
		m_SubFunction8Button.ShowWindow(SW_SHOW);
	}

	if (vInAutoSetup)
	{
		m_Function2Button.ShowWindow(SW_SHOW);
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		TempText.LoadString(IDS_FinishAutoSetup);
		SetDlgItemText(IDC_Function2Button,TempText);

		m_Function5Button.ShowWindow(SW_SHOW);
		m_Function5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		TempText.LoadString(IDS_NextAutoSetupStep);
		SetDlgItemText(IDC_Function5Button,TempText);

		if (vShowExit)
		{
			//m_Function4Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			//TempText.LoadString(IDS_Cancel);
		}
		else
		{
			m_Function4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			TempText.LoadString(IDS_PreviousAutoSetup);
			m_Function4Button.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_Function4Button,TempText);
		}
	}
	vBodyTriggerToUse = vCPBTLocalCurrentProductPointer->vBodyTrigger;
	vBackupBodyTriggerToUse = vCPBTLocalCurrentProductPointer->vBackupBodyTrigger;

	SetDlgItemText(IDC_ProductNameTitleStaticText,"Product: " + *(vCPBTLocalCurrentProductPointer->GetProductName()));

 	UpdateBodyTriggerDisplay();
	this->SetWindowText(_T("Body Triggers"));

	CDialog::OnShowWindow(bShow, nStatus);
	::SetupMenu(vLocalCWndCollection);
}

void CConfigureProductBodyTriggerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigureProductBodyTriggerDialog)
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_BackupBodyTriggerName, m_BackupBodyTriggerToUse);
	DDX_Control(pDX, IDC_BackupBodyTriggerToUseLabel, m_BackupBodyTriggerToUseLabel);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_BodyTriggerName, m_BodyTriggerName);
	DDX_Control(pDX, IDC_BodyTriggerToUseLabel, m_BodyTriggerToUseLabel);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_ProductNameTitleStaticText, m_ProductNameTitleStaticText);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigureProductBodyTriggerDialog, CDialog)
	//{{AFX_MSG_MAP(CConfigureProductBodyTriggerDialog)
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_Function5Button, OnFunction5Button)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_SubFunction5Button, OnSubFunction5Button)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SubFunction6Button, OnSubFunction6Button)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CConfigureProductBodyTriggerDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigureProductBodyTriggerDialog message handlers

void CConfigureProductBodyTriggerDialog::OnFunction1Button() 
{
	// User clicked Escape Button to exit without saving
	if (vInAutoSetup)
	{
		if (AbortSetupChanges())
			CDialog::EndDialog(10);
	}
	else
	{
		if ((!vChangeMade) || (AbortSetupChanges()))
			CDialog::EndDialog(10);
	}
}

void CConfigureProductBodyTriggerDialog::OnFunction2Button() 
{
	//Save and Exit button pressed
	if (!vInAutoSetup)
		SaveAndExit();
	else
	if (KeepSetupChanges())
	{
		vCPBTLocalCurrentProductPointer->vBodyTrigger = vBodyTriggerToUse;
		vCPBTLocalCurrentProductPointer->vBackupBodyTrigger = vBackupBodyTriggerToUse;
		CDialog::EndDialog(11);
	}
}

void CConfigureProductBodyTriggerDialog::SaveAndExit() 
{
	vCPBTLocalCurrentProductPointer->vBodyTrigger = vBodyTriggerToUse;
	vCPBTLocalCurrentProductPointer->vBackupBodyTrigger = vBackupBodyTriggerToUse;
	CDialog::EndDialog(true);
}


void CConfigureProductBodyTriggerDialog::OnFunction3Button() 
{
}

void CConfigureProductBodyTriggerDialog::OnFunction4Button() 
{
	// pressed cancel button
	if (vInAutoSetup)  //if in autosetup go to previous step
	if (!vShowExit)
	{
		vCPBTLocalCurrentProductPointer->vBodyTrigger = vBodyTriggerToUse;
		vCPBTLocalCurrentProductPointer->vBackupBodyTrigger = vBackupBodyTriggerToUse;
		CDialog::OnCancel();
	}
	/*
	else
	if (vChangeMade)
	if (AbortSetupChanges())
		CDialog::OnCancel();
	*/
}

void CConfigureProductBodyTriggerDialog::OnFunction5Button() 
{
	if (vInAutoSetup)
		SaveAndExit();
}

void CConfigureProductBodyTriggerDialog::OnSubFunction5Button() 
{
	// TODO: Add your control notification handler code here
}

void CConfigureProductBodyTriggerDialog::OnSubFunction6Button() 
{
	//pressed change container trigger button
	/*
	if (vBodyTriggerToUse < cNumberOfBodyTriggers)
		vBodyTriggerToUse++;
	else
		vBodyTriggerToUse = 1;
	SetChangeMade();
	UpdateBodyTriggerDisplay();
	*/
	CSelectItemDialog ISelectItemDialog;  
	
	//Set dialog box data titles and number value
	ISelectItemDialog.vTitleString = "Select Container Trigger";
	ISelectItemDialog.vNumberOfItems = cNumberOfBodyTriggers;
	ISelectItemDialog.vBackgroundColor = cGreen;

	for (BYTE TempLoop = 0; TempLoop < cNumberOfBodyTriggers; TempLoop++)
	{
		CString TempString = dtoa(TempLoop + 1, 0)  + ": " + vLocalConfigurationData->vBodyTrigger[TempLoop].vName;

		if (vBackupBodyTriggerToUse == TempLoop + 1)
		{
			TempString = "Backup Body Trigger-Used";
			ISelectItemDialog.vItemEnabled[TempLoop] = false;
		}

		if (!vLocalConfigurationData->vBodyTrigger[TempLoop].vEnabled)
		{
			TempString = TempString + "-Not Enabled";
			ISelectItemDialog.vItemEnabled[TempLoop] = false;
		}

		ISelectItemDialog.vItems[TempLoop] = TempString;
	}
	ISelectItemDialog.m_DialogTitleStaticText2 = "Product: " + 
		*(vCPBTLocalCurrentProductPointer->GetProductName());
	if (vBodyTriggerToUse)
		ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: " + 
			ISelectItemDialog.vItems[vBodyTriggerToUse - 1];
	else
		ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: Not Configured";
	//Pass control to dialog box and display
	int nResponse = ISelectItemDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		if (vBackupBodyTriggerToUse != ISelectItemDialog.vItemNumberSelected)
		{
			if (ISelectItemDialog.vItemNumberSelected <= cNumberOfBodyTriggers)
			{
				SetChangeMade();
				vBodyTriggerToUse = ISelectItemDialog.vItemNumberSelected;
				UpdateBodyTriggerDisplay();
			}
		}
		//if picked one, then exit dialog
		//if (vLocalConfigurationData->vBodyTrigger[vBodyTriggerToUse - 1].vEnabled)
		//	OnFunction2Button();
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		CDialog::EndDialog(10);
	}
}

void CConfigureProductBodyTriggerDialog::OnSubFunction8Button() 
{
	//pressed change backup container trigger button
	if (vNumberOfBodyTriggersInSystem > 1)
	{
		CSelectItemDialog ISelectItemDialog;  
	
		//Set dialog box data titles and number value
		ISelectItemDialog.vTitleString = "Select Backup Container Trigger";
		ISelectItemDialog.vNumberOfItems = cNumberOfBodyTriggers + 1;
		ISelectItemDialog.vBackgroundColor = cGreen;

		for (BYTE TempLoop = 0; TempLoop < cNumberOfBodyTriggers; TempLoop++)
		{
			CString TempString = dtoa(TempLoop + 1, 0)  + ": " + vLocalConfigurationData->vBodyTrigger[TempLoop].vName;

			if (vBodyTriggerToUse == TempLoop + 1)
			{
				TempString = "Primary Body Trigger-Used";
				ISelectItemDialog.vItemEnabled[TempLoop] = false;
			}

			if (!vLocalConfigurationData->vBodyTrigger[TempLoop].vEnabled)
			{
				TempString = TempString + "-Not Enabled";
				ISelectItemDialog.vItemEnabled[TempLoop] = false;
			}

			ISelectItemDialog.vItems[TempLoop] = TempString;
		}

		//ISelectItemDialog.vNumberOfItems = TempNumberOfChoices;
		ISelectItemDialog.vItems[cNumberOfBodyTriggers] = "None - Disable";

		ISelectItemDialog.m_DialogTitleStaticText2 = "Product: " + 
			*(vCPBTLocalCurrentProductPointer->GetProductName());
		if (vBackupBodyTriggerToUse)
			ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: " + 
				ISelectItemDialog.vItems[vBackupBodyTriggerToUse - 1];
		else
			ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: Not Configured";
		//Pass control to dialog box and display
		int nResponse = ISelectItemDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			if (vBodyTriggerToUse != ISelectItemDialog.vItemNumberSelected)
			{
				SetChangeMade();
				if (ISelectItemDialog.vItemNumberSelected <= cNumberOfBodyTriggers)
					vBackupBodyTriggerToUse = ISelectItemDialog.vItemNumberSelected;
				else
					vBackupBodyTriggerToUse = 0;
				UpdateBodyTriggerDisplay();
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

void CConfigureProductBodyTriggerDialog::UpdateBodyTriggerDisplay() 
{
	CString TempString = dtoa(vBodyTriggerToUse, 0)  + ": ";

	if (vBodyTriggerToUse == 0)
		TempString = TempString + "Not Configured";
	else
	if (vLocalConfigurationData->vBodyTrigger[vBodyTriggerToUse - 1].vEnabled == 1)
		TempString = TempString + vLocalConfigurationData->vBodyTrigger[vBodyTriggerToUse - 1].vName;
	else
		TempString = TempString + "Not Enabled";

	SetDlgItemText(IDC_BodyTriggerName,TempString);

	TempString = vBackupBodyTriggerToUse  + ": ";

	if (vBackupBodyTriggerToUse == 0)
		TempString = TempString + "Not Configured";
	else
	if (vLocalConfigurationData->vBodyTrigger[vBackupBodyTriggerToUse - 1].vEnabled == 1)
		TempString = TempString + vLocalConfigurationData->vBodyTrigger[vBackupBodyTriggerToUse - 1].vName;
	else
		TempString = TempString + "Not Enabled";

	SetDlgItemText(IDC_BackupBodyTriggerName,TempString);
}

BOOL CConfigureProductBodyTriggerDialog::PreTranslateMessage(MSG* pMsg) 
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
				//OnSubFunction7Button();
			break;
			case 115: // F4:
				OnSubFunction6Button();
			break;
			case 116: // F5:
				//OnSubFunction1Button();
			break;
			case 117: // F6 Key - Function 5:
				//OnSubFunction4Button();
			break;
			case 118: // F7 Key - Sub Function 1:
				OnFunction1Button();
			break;
			case 119: // F8 Key - Sub Function 2:
				OnSubFunction8Button();
			break;
			case 120: // F9 Key - Sub Function 3:
				//OnSubFunction2Button();
			break;
			case 122: // F11 Key - Sub Function 4:
				//OnSubFunction3Button();
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
				PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 5, 0);
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

HBRUSH CConfigureProductBodyTriggerDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	
	// TODO: Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function5Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);
		if (vInAutoSetup)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_BackupBodyTriggerToUseLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_BackupBodyTriggerToUseLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_BodyTriggerToUseLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_BodyTriggerToUseLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_BodyTriggerName)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_BodyTriggerName, 5);
		pDC->SetBkMode(TRANSPARENT);
	}
	if (pWnd->GetDlgCtrlID() == IDC_BackupBodyTriggerName)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_BackupBodyTriggerToUse, 5);
		pDC->SetBkMode(TRANSPARENT);
	}
	if (pWnd->GetDlgCtrlID() == IDC_ProductNameTitleStaticText)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ProductNameTitleStaticText, 5);
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
	/*if (pWnd->GetDlgCtrlID() == IDC_BodyTriggerName)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vWhiteBrush;
  }
	*/
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CConfigureProductBodyTriggerDialog::SetChangeMade() 
{
	if (!vChangeMade)
	{
		vChangeMade = true;
		if (!vInAutoSetup)
		{
			m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_Function2Button, _T("Save\nand Exit"));
			//m_Function4Button.ShowWindow(SW_SHOW);
			//m_ArrowDown4.ShowWindow(SW_SHOW);
			//SetDlgItemText(IDC_Function4Button,"Cancel");
		}
	}
}



void CConfigureProductBodyTriggerDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
