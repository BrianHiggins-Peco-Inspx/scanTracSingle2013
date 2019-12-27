//ScanTrac Side View Source File
// ConfigureEMailAddresses.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "ConfigureEMailAddresses.h"
#include "AlphaEntryDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
/////////////////////////////////////////////////////////////////////////////
// CConfigureEMailAddresses dialog


CConfigureEMailAddresses::CConfigureEMailAddresses(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigureEMailAddresses::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigureEMailAddresses)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	vEMailCategory = cEMailPlantOperations;
	vChangeMade = false;
	vLocalCWndCollection.Add(this					 );
	vLocalCWndCollection.Add(&m_Function5Button		 );
	vLocalCWndCollection.Add(&m_Background			 );
	vLocalCWndCollection.Add(&m_SubFunction8Button	 );
	vLocalCWndCollection.Add(&m_SubFunction7Button	 );
	vLocalCWndCollection.Add(&m_SubFunction6Button	 );
	vLocalCWndCollection.Add(&m_SubFunction5Button	 );
	vLocalCWndCollection.Add(&m_SubFunction4Button	 );
	vLocalCWndCollection.Add(&m_SubFunction3Button	 );
	vLocalCWndCollection.Add(&m_SubFunction2Button	 );
	vLocalCWndCollection.Add(&m_SubFunction1Button	 );
	vLocalCWndCollection.Add(&m_Function3Button		 );
	vLocalCWndCollection.Add(&m_Enable				 );
	vLocalCWndCollection.Add(&m_Subject				 );
	vLocalCWndCollection.Add(&m_User4				 );
	vLocalCWndCollection.Add(&m_User3				 );
	vLocalCWndCollection.Add(&m_User2				 );
	vLocalCWndCollection.Add(&m_User1				 );
	vLocalCWndCollection.Add(&m_Domain4				 );
	vLocalCWndCollection.Add(&m_Domain3				 );
	vLocalCWndCollection.Add(&m_Domain2				 );
	vLocalCWndCollection.Add(&m_Domain1				 );
	vLocalCWndCollection.Add(&m_Function4Button		 );
	vLocalCWndCollection.Add(&m_Function2Button		 );
	vLocalCWndCollection.Add(&m_AddressType			 );
	vLocalCWndCollection.Add(&m_At1					 );
	vLocalCWndCollection.Add(&m_At2					 );
	vLocalCWndCollection.Add(&m_At3					 );
	vLocalCWndCollection.Add(&m_At4					 );
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button		 );
}


void CConfigureEMailAddresses::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	vFirstUser = 0;
	vChangeMade = false;
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);
	
	vFunction3ButtonEnable = true;
	vFunction5ButtonEnable = false;
	vSubFunction1ButtonEnable = true;
	vSubFunction2ButtonEnable = true;
	vSubFunction3ButtonEnable = true;
	vSubFunction4ButtonEnable = true;
	vSubFunction5ButtonEnable = true;
	vSubFunction6ButtonEnable = true;
	vSubFunction7ButtonEnable = true;

	BYTE TempSubjectIndex = 0;
	if (vEMailCategory == cEMailPlantOperations)
	{
		TempSubjectIndex = 0;
		SetDlgItemText(IDC_AddressType, _T("Addresses for: Plant Operations"));
	}
	else
	if (vEMailCategory == cEMailInspx)
	{
		TempSubjectIndex = 1;
		SetDlgItemText(IDC_AddressType, _T("Addresses for: Inspx"));
	}
	else
	if (vEMailCategory == cEMailMaintenance)
	{
		SetDlgItemText(IDC_AddressType, _T("Addresses for: Plant Maintenance"));
		TempSubjectIndex = 2;
	}
	else
	if (vEMailCategory == cEMailReport)
	{
		SetDlgItemText(IDC_AddressType, _T("Addresses for: End of Shift Report"));
		TempSubjectIndex = 3;
	}

	BYTE TempUserIndex = TempSubjectIndex * cMaximumEMailRecievers;

	for (BYTE TempLoop = 0;  TempLoop < cMaximumEMailRecievers; TempLoop++)
	{
		vEMailUser[TempLoop] = 
			vLocalConfigurationData->vEMailUser[TempUserIndex + TempLoop];
		vEMailDomain[TempLoop] = 
			vLocalConfigurationData->vEMailDomain[TempUserIndex + TempLoop];
	}
	vEMailSubject = 
			vLocalConfigurationData->vEMailSubject[TempSubjectIndex];
	vNumberOfEMailRecievers = 
			vLocalConfigurationData->vNumberOfEMailRecievers[TempSubjectIndex];
	vEMailEnable = 
			vLocalConfigurationData->vEMailEnable[TempSubjectIndex];
	UpdateDisplay();
	UpdateButtons();
	UpdateDisplayEnable();
	this->SetWindowText(_T("E-Mail"));
	SetupMenu(vLocalCWndCollection);
}

void CConfigureEMailAddresses::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigureEMailAddresses)
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Enable, m_Enable);
	DDX_Control(pDX, IDC_Subject, m_Subject);
	DDX_Control(pDX, IDC_User4, m_User4);
	DDX_Control(pDX, IDC_User3, m_User3);
	DDX_Control(pDX, IDC_User2, m_User2);
	DDX_Control(pDX, IDC_User1, m_User1);
	DDX_Control(pDX, IDC_Domain4, m_Domain4);
	DDX_Control(pDX, IDC_Domain3, m_Domain3);
	DDX_Control(pDX, IDC_Domain2, m_Domain2);
	DDX_Control(pDX, IDC_Domain1, m_Domain1);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_AddressType, m_AddressType);
	DDX_Control(pDX, IDC_At1, m_At1);
	DDX_Control(pDX, IDC_At2, m_At2);
	DDX_Control(pDX, IDC_At3, m_At3);
	DDX_Control(pDX, IDC_At4, m_At4);
	DDX_Control(pDX, IDC_DialogTitleStaticText, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
}


BEGIN_MESSAGE_MAP(CConfigureEMailAddresses, CDialog)
	//{{AFX_MSG_MAP(CConfigureEMailAddresses)
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
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigureEMailAddresses message handlers

void CConfigureEMailAddresses::OnFunction1Button() 
{
	//Main Menu was pressed
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::EndDialog(10);
}

void CConfigureEMailAddresses::OnFunction2Button() 
{
	//Save and exit was pressed
	if ((!vChangeMade) || (KeepSetupChanges()))
	{
		BYTE TempSubjectIndex = 0;
		if (vEMailCategory == cEMailPlantOperations)
			TempSubjectIndex = 0;
		else
		if (vEMailCategory == cEMailInspx)
			TempSubjectIndex = 1;
		else
		if (vEMailCategory == cEMailMaintenance)
			TempSubjectIndex = 2;
		else
		if (vEMailCategory == cEMailReport)
			TempSubjectIndex = 3;

		BYTE TempUserIndex = TempSubjectIndex * cMaximumEMailRecievers;
		BYTE TempDestinationCount = 0;

		for (BYTE TempLoop = 0;  TempLoop < cMaximumEMailRecievers; TempLoop++)
		{
			if ((vEMailUser[TempLoop] != "") &&
				(vEMailDomain[TempLoop] != ""))
			{
				vLocalConfigurationData->vEMailUser[TempUserIndex + TempDestinationCount] = 
					vEMailUser[TempLoop];
				vLocalConfigurationData->vEMailDomain[TempUserIndex + TempDestinationCount] = 
					vEMailDomain[TempLoop];
				TempDestinationCount++;
			}
		}
		for (BYTE TempLoop = TempDestinationCount;  TempLoop < cMaximumEMailRecievers; TempLoop++)
		{
			vLocalConfigurationData->vEMailUser[TempUserIndex + TempLoop] = "";
			vLocalConfigurationData->vEMailDomain[TempUserIndex + TempLoop] = ".Com";
		}
		vLocalConfigurationData->vEMailSubject[TempSubjectIndex] = 
				vEMailSubject;
		vLocalConfigurationData->vEMailEnable[TempSubjectIndex] = 
				vEMailEnable;
		vLocalConfigurationData->vNumberOfEMailRecievers[TempSubjectIndex] = TempDestinationCount;
	
		SortAddresses(TempUserIndex, TempSubjectIndex);
		CDialog::EndDialog(true);
	}
}

void CConfigureEMailAddresses::SortAddresses(WORD TempUserIndex, WORD TempSubjectIndex) 
{
	bool TempDone = false;
	while (!TempDone)
	{
		bool TempMadeChange = false;
		for (BYTE TempLoop = 0;  TempLoop < 
			vLocalConfigurationData->vNumberOfEMailRecievers[TempSubjectIndex] - 1; TempLoop++)
		{
			if (vLocalConfigurationData->vEMailUser[TempUserIndex + TempLoop] > 
				vLocalConfigurationData->vEMailUser[TempUserIndex + TempLoop + 1])
			{
				CString TempUserString = vLocalConfigurationData->vEMailUser[TempUserIndex + TempLoop];
				vLocalConfigurationData->vEMailUser[TempUserIndex + TempLoop] = 
					vLocalConfigurationData->vEMailUser[TempUserIndex + TempLoop + 1];
				vLocalConfigurationData->vEMailUser[TempUserIndex + TempLoop + 1] = TempUserString;
				TempUserString = vLocalConfigurationData->vEMailDomain[TempUserIndex + TempLoop];
				vLocalConfigurationData->vEMailDomain[TempUserIndex + TempLoop] = 
					vLocalConfigurationData->vEMailDomain[TempUserIndex + TempLoop + 1];
				vLocalConfigurationData->vEMailDomain[TempUserIndex + TempLoop + 1] = TempUserString;
				TempMadeChange = true;
			}
		}
		if (!TempMadeChange)
			TempDone = true;
	}
}

void CConfigureEMailAddresses::OnFunction3Button() 
{
	if ((vEMailEnable) && (vFunction3ButtonEnable == true))
	{
		if (vFirstUser > 2)
			vFirstUser = vFirstUser - 3;
		else
			vFirstUser = 0;
		UpdateButtons();
		UpdateDisplay();
	}
	else if (vFunction3ButtonEnable == false)
		ClickEnableBeforeChangeAvailableDialogMessage();

}

void CConfigureEMailAddresses::OnFunction4Button() 
{
	//Cancel was pressed
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::OnCancel();
}

void CConfigureEMailAddresses::OnFunction5Button() 
{
	if ((vEMailEnable) && (vFunction5ButtonEnable == true))
	{
		if (vFirstUser < cMaximumEMailRecievers - 6)
			vFirstUser = vFirstUser + 3;
		else
			vFirstUser = cMaximumEMailRecievers - 3;
		UpdateButtons();
		UpdateDisplay();
	}
	else if (vFunction5ButtonEnable == false)
		ClickEnableBeforeChangeAvailableDialogMessage();
}

void CConfigureEMailAddresses::OnSubFunction1Button() 
{
	if (vSubFunction1ButtonEnable == true) 
		EditEMailUser(0);
	else
		ClickEnableBeforeChangeAvailableDialogMessage();
}

void CConfigureEMailAddresses::EditEMailUser(BYTE TempUserNumber) 
{
	if (vEMailEnable)
	{
		CAlphaEntryDialog IAlphaEntryDialog;  
		//Set dialog box data titles and number value
		IAlphaEntryDialog.vEditString = vEMailUser[TempUserNumber + vFirstUser];
		IAlphaEntryDialog.m_DialogTitleStaticText = "Edit Name For E-Mail User";
		IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
		IAlphaEntryDialog.m_DialogTitleStaticText2 = "Original Value: " + 
			vEMailUser[TempUserNumber + vFirstUser];
		IAlphaEntryDialog.vBlankNameOK = true;
		IAlphaEntryDialog.vAllowPeriod = true;
		IAlphaEntryDialog.vAllowDashes = true;
		//Pass control to dialog box and display
		int nResponse = IAlphaEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vEMailUser[TempUserNumber + vFirstUser] = IAlphaEntryDialog.vEditString;
			SetChangeMade();
			UpdateDisplay();
		}
		else
		if (nResponse == 10)  //main menu pressed
			CDialog::EndDialog(10);
	}
}

void CConfigureEMailAddresses::OnSubFunction2Button() 
{
	if (vSubFunction2ButtonEnable == true)
		EditEMailUser(1);
	else
		ClickEnableBeforeChangeAvailableDialogMessage();
}

void CConfigureEMailAddresses::OnSubFunction3Button() 
{
	if (vSubFunction3ButtonEnable == true)
		EditEMailUser(2);
	else
		ClickEnableBeforeChangeAvailableDialogMessage();
}

void CConfigureEMailAddresses::OnSubFunction4Button() 
{
	if ((vEMailEnable) && (vSubFunction4ButtonEnable == true))
	{
		CAlphaEntryDialog IAlphaEntryDialog;  
		//Set dialog box data titles and number value
		IAlphaEntryDialog.vEditString = vEMailSubject;
		IAlphaEntryDialog.vAllowDashes = true;
		IAlphaEntryDialog.vAllowPeriod = true;
		IAlphaEntryDialog.m_DialogTitleStaticText = "Edit E-Mail Subject Line";
		IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
		IAlphaEntryDialog.m_DialogTitleStaticText2 = "Original Value: " + 
			vEMailSubject;
		//Pass control to dialog box and display
		int nResponse = IAlphaEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vEMailSubject = IAlphaEntryDialog.vEditString;
			SetChangeMade();
			UpdateDisplay();
		}
		else
		if (nResponse == 10)  //main menu pressed
			CDialog::EndDialog(10);
	}
	else
	if (vSubFunction4ButtonEnable == false)
			ClickEnableBeforeChangeAvailableDialogMessage();
}

void CConfigureEMailAddresses::OnSubFunction5Button() 
{
	if (vSubFunction5ButtonEnable == true)
		EditEMailDomain(0);
	else
		ClickEnableBeforeChangeAvailableDialogMessage();

}

void CConfigureEMailAddresses::EditEMailDomain(BYTE TempUserNumber) 
{
	if (vEMailEnable)
	{
		CAlphaEntryDialog IAlphaEntryDialog;  
		//Set dialog box data titles and number value
		IAlphaEntryDialog.vEditString = vEMailDomain[TempUserNumber + vFirstUser];
		IAlphaEntryDialog.m_DialogTitleStaticText = "Edit Name For E-Mail Domain";
		IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
		IAlphaEntryDialog.m_DialogTitleStaticText2 = "Original Value: " + 
			vEMailDomain[TempUserNumber + vFirstUser];
		IAlphaEntryDialog.vAllowPeriod = true;
		IAlphaEntryDialog.vBlankNameOK = true;
		IAlphaEntryDialog.vAllowDashes = true;
		//Pass control to dialog box and display
		int nResponse = IAlphaEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vEMailDomain[TempUserNumber + vFirstUser] = IAlphaEntryDialog.vEditString;
			SetChangeMade();
			UpdateDisplay();
		}
		else
		if (nResponse == 10)  //main menu pressed
			CDialog::EndDialog(10);
	}
}

void CConfigureEMailAddresses::OnSubFunction6Button() 
{
	if (vSubFunction6ButtonEnable == true)
		EditEMailDomain(1);
	else
		ClickEnableBeforeChangeAvailableDialogMessage();
}

void CConfigureEMailAddresses::OnSubFunction7Button() 
{
	if (vSubFunction7ButtonEnable == true)
		EditEMailDomain(2);
	else
		ClickEnableBeforeChangeAvailableDialogMessage();
}

void CConfigureEMailAddresses::OnSubFunction8Button() 
{
	vEMailEnable = !vEMailEnable;
	SetChangeMade();
	UpdateDisplayEnable();
}

void CConfigureEMailAddresses::UpdateButtons()
{
	if (vFirstUser < cMaximumEMailRecievers - 3)
	{
		m_Function5Button.ShowWindow(SW_SHOW);
	}
	else
	{
		m_Function5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
	}
	if (vFirstUser)
	{
		m_Function3Button.ShowWindow(SW_SHOW);
	}
	else
	{
		m_Function3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
	}
	CString TempString = dtoa(vFirstUser + 1, 0);
	SetDlgItemText(IDC_SubFunction1Button,"Change User Name " + TempString);
	SetDlgItemText(IDC_SubFunction5Button,"Change Domain Name " + TempString);

	TempString = dtoa(vFirstUser + 2, 0);
	SetDlgItemText(IDC_SubFunction2Button,"Change User Name " + TempString);
	SetDlgItemText(IDC_SubFunction6Button,"Change Domain Name " + TempString);

	TempString = dtoa(vFirstUser + 3, 0);
	SetDlgItemText(IDC_SubFunction3Button,"Change User Name " + TempString);
	SetDlgItemText(IDC_SubFunction7Button,"Change Domain Name " + TempString);
}

BOOL CConfigureEMailAddresses::PreTranslateMessage(MSG* pMsg) 
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
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, TempKey);
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
			break;
			case cPageDown: // Help
				PostThreadMessage(vGlobalCallingThreadID,
					cShowHelp, 52, 0);
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
	
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CConfigureEMailAddresses::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (vLocalSystemData->vGlobalMonitorSizeX >= 1024 && vLocalSystemData->vGlobalMonitorSizeY >= 768)
	{
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SelectObject(vLocalSystemData->vCurrentMediumLargeFont);
	}
	// TODO: Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText)
	{
		pDC->SetAttribDC(pDC->m_hDC);
		if (vLocalSystemData->vGlobalMonitorSizeX >= 1024 && vLocalSystemData->vGlobalMonitorSizeY >= 768)
			pDC->SelectObject(vLocalSystemData->vCurrentHugeFont);
		else
			pDC->SelectObject(vLocalSystemData->vCurrentExtraLargeFont);
		pDC->SetTextColor(vGlobalDialogTitleColor);
//		if (PasswordOK(cTemporaryInspxPassword,false))
//			pDC->SetTextColor(cBrilliantBlue);
//		else
//			pDC->SetTextColor(cDarkBlue);
	}

	if ((pWnd->GetDlgCtrlID() == IDC_Background) ||
		(pWnd->GetDlgCtrlID() == IDC_At1) ||
		(pWnd->GetDlgCtrlID() == IDC_At2) ||
		(pWnd->GetDlgCtrlID() == IDC_At3) ||
		(pWnd->GetDlgCtrlID() == IDC_At4) ||
		(pWnd->GetDlgCtrlID() == IDC_AddressType))
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
  }

  if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		if (vEMailEnable)
			return vLocalSystemData->vRedBrush;
		else
			return vLocalSystemData->vGreenBrush;
	}
	
  if ((pWnd->GetDlgCtrlID() == IDC_Function1Button) ||
		(pWnd->GetDlgCtrlID() == IDC_Function2Button) ||
		//(pWnd->GetDlgCtrlID() == IDC_Function3Button) ||
		(pWnd->GetDlgCtrlID() == IDC_Function4Button) ||
		//(pWnd->GetDlgCtrlID() == IDC_Function5Button) ||
		//(pWnd->GetDlgCtrlID() == IDC_SubFunction1Button) ||
		//(pWnd->GetDlgCtrlID() == IDC_SubFunction2Button) ||
		//(pWnd->GetDlgCtrlID() == IDC_SubFunction3Button) ||
		//(pWnd->GetDlgCtrlID() == IDC_SubFunction4Button) ||
		//(pWnd->GetDlgCtrlID() == IDC_SubFunction5Button) ||
		//(pWnd->GetDlgCtrlID() == IDC_SubFunction6Button) ||
		//(pWnd->GetDlgCtrlID() == IDC_SubFunction7Button) ||
		(pWnd->GetDlgCtrlID() == IDC_SubFunction8Button))
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function3Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		if (vFunction3ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}	
	if (pWnd->GetDlgCtrlID() == IDC_Function5Button)
	{
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
		if (vSubFunction1ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}	
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		if(vSubFunction2ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}	
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		if(vSubFunction3ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}	
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		if(vSubFunction4ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}	
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		if(vSubFunction5ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}	
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		if(vSubFunction6ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}	
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		if(vSubFunction7ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CConfigureEMailAddresses::SetChangeMade() 
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

void CConfigureEMailAddresses::UpdateDisplay() 
{
	SetDlgItemText(IDC_User1,vEMailUser[vFirstUser + 0]);
	SetDlgItemText(IDC_User2,vEMailUser[vFirstUser + 1]);
	SetDlgItemText(IDC_User3,vEMailUser[vFirstUser + 2]);
	SetDlgItemText(IDC_User4,vEMailUser[vFirstUser + 3]);
	SetDlgItemText(IDC_Domain1,vEMailDomain[vFirstUser + 0]);
	SetDlgItemText(IDC_Domain2,vEMailDomain[vFirstUser + 1]);
	SetDlgItemText(IDC_Domain3,vEMailDomain[vFirstUser + 2]);
	SetDlgItemText(IDC_Domain4,vEMailDomain[vFirstUser + 3]);
	SetDlgItemText(IDC_Subject,vEMailSubject);
}

void CConfigureEMailAddresses::UpdateDisplayEnable() 
{
	if (vEMailEnable)
	{
		SetDlgItemText(IDC_SubFunction8Button, _T("Disable"));
		SetDlgItemText(IDC_Enable, _T("Enabled"));
		m_User1.EnableWindow(true);
		m_User2.EnableWindow(true);
		m_User3.EnableWindow(true);
		m_User4.EnableWindow(true);
		m_Domain1.EnableWindow(true);
		m_Domain2.EnableWindow(true);
		m_Domain3.EnableWindow(true);
		m_Domain4.EnableWindow(true);
		m_Subject.EnableWindow(true);
		m_Enable.EnableWindow(true);
		vFunction3ButtonEnable = true;	//m_Function3Button.EnableWindow(true);
		vFunction5ButtonEnable = true;	//m_Function5Button.EnableWindow(true);
		vSubFunction1ButtonEnable = true;	//m_SubFunction1Button.EnableWindow(true);
		vSubFunction2ButtonEnable = true;	//m_SubFunction2Button.EnableWindow(true);
		vSubFunction3ButtonEnable = true;	//m_SubFunction3Button.EnableWindow(true);
		vSubFunction4ButtonEnable = true;	//m_SubFunction4Button.EnableWindow(true);
		vSubFunction5ButtonEnable = true;	//m_SubFunction5Button.EnableWindow(true);
		vSubFunction6ButtonEnable = true;	//m_SubFunction6Button.EnableWindow(true);
		vSubFunction7ButtonEnable = true;	//m_SubFunction7Button.EnableWindow(true);
		//m_SubFunction8Button.EnableWindow(true);
	}
	else
	{
		SetDlgItemText(IDC_SubFunction8Button, _T("Enable"));
		SetDlgItemText(IDC_Enable, _T("Disabled"));
		m_User1.EnableWindow(false);
		m_User2.EnableWindow(false);
		m_User3.EnableWindow(false);
		m_User4.EnableWindow(false);
		m_Domain1.EnableWindow(false);
		m_Domain2.EnableWindow(false);
		m_Domain3.EnableWindow(false);
		m_Domain4.EnableWindow(false);
		m_Subject.EnableWindow(false);
		m_Enable.EnableWindow(false);
		vFunction3ButtonEnable = false;		//m_Function3Button.EnableWindow(false);
		vFunction5ButtonEnable = false;		//m_Function5Button.EnableWindow(false);
		vSubFunction1ButtonEnable = false;	//m_SubFunction1Button.EnableWindow(false);
		vSubFunction2ButtonEnable = false;	//m_SubFunction2Button.EnableWindow(false);
		vSubFunction3ButtonEnable = false;	//m_SubFunction3Button.EnableWindow(false);
		vSubFunction4ButtonEnable = false;	//m_SubFunction4Button.EnableWindow(false);
		vSubFunction5ButtonEnable = false;	//m_SubFunction5Button.EnableWindow(false);
		vSubFunction6ButtonEnable = false;	//m_SubFunction6Button.EnableWindow(false);
		vSubFunction7ButtonEnable = false;	//m_SubFunction7Button.EnableWindow(false);
		//m_SubFunction8Button.EnableWindow(false);
	}
	m_Function3Button.Invalidate(false);
	m_Function5Button.Invalidate(false);
	m_SubFunction1Button.Invalidate(false);
	m_SubFunction2Button.Invalidate(false);
	m_SubFunction3Button.Invalidate(false);
	m_SubFunction4Button.Invalidate(false);
	m_SubFunction5Button.Invalidate(false);
	m_SubFunction6Button.Invalidate(false);
	m_SubFunction7Button.Invalidate(false);
}

void CConfigureEMailAddresses::ShowNoMouseClickDialogMessage() 
{
	CNoticeDialog TempNoticeDialog;
	CString TempText("");
	TempText = ("Mouse Click Not Available");
	TempNoticeDialog.vNoticeText = "\n\n\nMouse Click Not Available";
	TempNoticeDialog.vType = cInformationMessage;
	TempNoticeDialog.DoModal();
}

void CConfigureEMailAddresses::ClickEnableBeforeChangeAvailableDialogMessage() 
{
	CNoticeDialog TempNoticeDialog;
	CString TempText("");
	TempText = ("Enable Required First");
	TempNoticeDialog.vNoticeText = "\n\n\nEnable Required First";
	TempNoticeDialog.vType = cInformationMessage;
	TempNoticeDialog.DoModal();
}



