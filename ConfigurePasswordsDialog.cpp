// ConfigurePasswordsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "ConfigurePasswordsDialog.h"
#include "PasswordEntryDialog.h"
#include "SetupUserDialog.h"
#include "UsersDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
extern int vGlobalPasswordLevel;
/////////////////////////////////////////////////////////////////////////////
// CConfigurePasswordsDialog dialog


CConfigurePasswordsDialog::CConfigurePasswordsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigurePasswordsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigurePasswordsDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	vChangeMade = false;
	vOneSecondTimerHandle = 29997;
	vOldPasswordLevel = 0;
	vLocalCWndCollection.Add(this						 );
	vLocalCWndCollection.Add(&m_SubFunction3Button		 );
	vLocalCWndCollection.Add(&m_SubFunction6Button		 );
	vLocalCWndCollection.Add(&m_SubFunction1Button		 );
	vLocalCWndCollection.Add(&m_SubFunction2Button		 );
	vLocalCWndCollection.Add(&m_Background				 );
	vLocalCWndCollection.Add(&m_SubFunction4Button		 );
	vLocalCWndCollection.Add(&m_SubFunction8Button		 );
	vLocalCWndCollection.Add(&m_Function4Button			 );
	vLocalCWndCollection.Add(&m_Function2Button			 );
	vLocalCWndCollection.Add(&m_Function3Button			 );
	vLocalCWndCollection.Add(&m_Function5Button			 );
	vLocalCWndCollection.Add(&m_SubFunction7Button		 );
	vLocalCWndCollection.Add(&m_Clock					 );
	vLocalCWndCollection.Add(&m_CPMTimePeriodDisplay	 );
	vLocalCWndCollection.Add(&m_Date					 );
	vLocalCWndCollection.Add(&m_DialogTitleStaticText	 );
	vLocalCWndCollection.Add(&m_Function1Button			 );
	vLocalCWndCollection.Add(&m_Function5Display		 );
	vLocalCWndCollection.Add(&m_SubFunction1Display		 );
	vLocalCWndCollection.Add(&m_SubFunction2Display		 );
	vLocalCWndCollection.Add(&m_SubFunction3Display		 );
	vLocalCWndCollection.Add(&m_SubFunction4Display		 );
	vLocalCWndCollection.Add(&m_SubFunction5Button		 );
	vLocalCWndCollection.Add(&m_WorkStopTimePeriodDisplay);
}


void CConfigurePasswordsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigurePasswordsDialog)
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Clock, m_Clock);
	DDX_Control(pDX, IDC_CPMTimePeriodDisplay, m_CPMTimePeriodDisplay);
	DDX_Control(pDX, IDC_Date, m_Date);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function5Display, m_Function5Display);
	DDX_Control(pDX, IDC_SubFunction1Display, m_SubFunction1Display);
	DDX_Control(pDX, IDC_SubFunction2Display, m_SubFunction2Display);
	DDX_Control(pDX, IDC_SubFunction3Display, m_SubFunction3Display);
	DDX_Control(pDX, IDC_SubFunction4Display, m_SubFunction4Display);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_WorkStopTimePeriodDisplay, m_WorkStopTimePeriodDisplay);
}


BEGIN_MESSAGE_MAP(CConfigurePasswordsDialog, CDialog)
	//{{AFX_MSG_MAP(CConfigurePasswordsDialog)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
	ON_BN_CLICKED(IDC_SubFunction1Button, OnSubFunction1Button)
	ON_BN_CLICKED(IDC_Function5Button, OnFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction2Button, OnSubFunction2Button)
	ON_BN_CLICKED(IDC_SubFunction3Button, OnSubFunction3Button)
	ON_BN_CLICKED(IDC_SubFunction5Button, OnSubFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction4Button, OnSubFunction4Button)
	ON_BN_CLICKED(IDC_SubFunction6Button, OnSubFunction6Button)
	ON_BN_CLICKED(IDC_SubFunction7Button, OnSubFunction7Button)
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_BN_CLICKED(IDC_DialogTitleStaticText, OnDialogTitleStaticText)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CConfigurePasswordsDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigurePasswordsDialog message handlers

void CConfigurePasswordsDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);
	vChangeMade = false;
	vAdvancedSetupPassword = vLocalConfigurationData->vAdvancedSetupPassword;
	vProductSetupPassword = vLocalConfigurationData->vProductSetupPassword;
	vProductSimpleSetupPassword = vLocalConfigurationData->vProductSimpleSetupPassword;
	vCertifiedSetupPassword = vLocalConfigurationData->vCertifiedSetupPassword;
	vInspxPassword = vLocalConfigurationData->vInspxPassword;
	vResetCountsPassword = vLocalConfigurationData->vResetCountsPassword;
	vSystemSetupResetPassword = vLocalConfigurationData->vSystemSetupResetPassword;
	vOperatorPassword = vLocalConfigurationData->vOperatorPassword;
	vQAPassword = vLocalConfigurationData->vQAPassword;
	vSamplePassword = vLocalConfigurationData->vSamplePassword;
	
	if (PasswordOK(cTemporaryInspxPassword,false))
		ShowInspxItems();
	else
		HideInspxItems();

	this->SetWindowText(_T("Passwords"));

	m_Function3Button.ShowWindow(SW_SHOW);

	SetupMenu(vLocalCWndCollection);

	UpdateDisplay();

	int TimerResult = SetTimer(vOneSecondTimerHandle,1000,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-One Second Timer Failed",cEMailInspx,32000);
}

void CConfigurePasswordsDialog::OnFunction1Button() 
{
	// Main Menu
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::EndDialog(10);
}

void CConfigurePasswordsDialog::OnFunction2Button() 
{
	//Exit button pressed
	if ((!vChangeMade) || (KeepSetupChanges()))
	{
		vLocalConfigurationData->vResetCountsPassword = vResetCountsPassword;
		vLocalConfigurationData->vSystemSetupResetPassword = vSystemSetupResetPassword;
		vLocalConfigurationData->vProductSetupPassword = vProductSetupPassword;
		vLocalConfigurationData->vProductSimpleSetupPassword = vProductSimpleSetupPassword;
		vLocalConfigurationData->vAdvancedSetupPassword = vAdvancedSetupPassword;
		vLocalConfigurationData->vCertifiedSetupPassword = vCertifiedSetupPassword;
		vLocalConfigurationData->vInspxPassword = vInspxPassword;
		vLocalConfigurationData->vOperatorPassword = vOperatorPassword;
		vLocalConfigurationData->vQAPassword = vQAPassword;
		vLocalConfigurationData->vSamplePassword = vSamplePassword;
		CDialog::EndDialog(true);
	}
}

void CConfigurePasswordsDialog::OnFunction3Button() 
{
	// TODO: Add your control notification handler code here
	CUsersDialog IUserDialog;
	IUserDialog.vMainWindowPointer = vMainWindowPointer;
	IUserDialog.vLocalSystemData = vLocalSystemData;
	IUserDialog.vLocalConfigurationData = vLocalConfigurationData;
	if (PasswordOK(cCertifiedPassword,false))
	{
		ReportErrorMessage("User entered Setup Users Menu", cUserAction, 0);
		int nResponse = IUserDialog.DoModal();
		if (nResponse == 10)
			CDialog::EndDialog(10);
		//else 
		//if (nResponse == IDOK)
		//	vLocalSystemData->UpdateUserList(IUserDialog.vLocalUserCollection);
	}
}

void CConfigurePasswordsDialog::OnFunction4Button() 
{
	// Cancel
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::OnCancel();
}

void CConfigurePasswordsDialog::OnFunction5Button() 
{
	// TODO: Add your control notification handler code here
	// Set Inspx Password was pressed
	if (PasswordOK(cInspxPassword,false))
	{
		CPasswordEntryDialog IPasswordEntryDialog;  
		
		//Set dialog box data titles and number value
		IPasswordEntryDialog.vEditString = "";
		IPasswordEntryDialog.m_DialogTitleStaticText1 = "Set the Inspx Setup Password";
		IPasswordEntryDialog.m_DialogTitleStaticText2 = "Password must be at least 6 characters long";
		IPasswordEntryDialog.vMinimumPasswordLength = 6;
		//Pass control to dialog box and display
		int nResponse = IPasswordEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			CString TempPassword = IPasswordEntryDialog.vEditString;
			if (TempPassword.GetLength() < 6)
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\nPassword must be at least 6 characters long\n\nPassword not changed";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
			else
			{
				IPasswordEntryDialog.vEditString = "";
				IPasswordEntryDialog.vDisplayString = "";
				IPasswordEntryDialog.m_DialogTitleStaticText1 = "Confirm the new Inspx Setup Password";
				IPasswordEntryDialog.m_DialogTitleStaticText2 = "";
				//Pass control to dialog box and display
				int nResponse = IPasswordEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					if (TempPassword == IPasswordEntryDialog.vEditString)
					{
						SetChangeMade();
						vInspxPassword = IPasswordEntryDialog.vEditString;
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\n\nPassword successfully changed";
						TempNoticeDialog.vType = cNoticeMessage;
						TempNoticeDialog.DoModal();
					}
					else
					{
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\nConfirm Password does not match\n\nPassword not changed";
						TempNoticeDialog.vType = cErrorMessage;
						TempNoticeDialog.DoModal();
					}
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
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
	
}

void CConfigurePasswordsDialog::OnSubFunction1Button() 
{
		if (PasswordOK(cTemporaryInspxPassword,false))
	{
		// Set Operator Password was pressed
		CPasswordEntryDialog IPasswordEntryDialog;  
		
		//Set dialog box data titles and number value
		IPasswordEntryDialog.vEditString = "";
		IPasswordEntryDialog.m_DialogTitleStaticText1 = "Set the Operator Password";
		IPasswordEntryDialog.m_DialogTitleStaticText2 = "";
		//Pass control to dialog box and display
		int nResponse = IPasswordEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			CString TempPassword = IPasswordEntryDialog.vEditString;
			IPasswordEntryDialog.vEditString = "";
			IPasswordEntryDialog.vDisplayString = "";
			IPasswordEntryDialog.m_DialogTitleStaticText1 = "Confirm the new Operator Password";
			IPasswordEntryDialog.m_DialogTitleStaticText2 = "";
			//Pass control to dialog box and display
			int nResponse = IPasswordEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				if (TempPassword == IPasswordEntryDialog.vEditString)
				{
					SetChangeMade();
					vOperatorPassword = IPasswordEntryDialog.vEditString;
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nPassword successfully changed";
					TempNoticeDialog.vType = cNoticeMessage;
					TempNoticeDialog.DoModal();
					vLocalSystemData->vOperatorPasswordOK = false;

					CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction5Button);
					TempWindow->Invalidate(false);
					if (vOperatorPassword.GetLength())
					{
						if (!vResetCountsPassword.GetLength())
						{
							vResetCountsPassword = "22";
							CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction6Button);
							if (TempWindow)
								TempWindow->Invalidate(false);
							TempNoticeDialog.vNoticeText = "\n\nReset Counters/Disable Ejectors Password was blank\nSet to default";
							TempNoticeDialog.vType = cNoticeMessage;
							TempNoticeDialog.DoModal();
						}
						if (!vProductSimpleSetupPassword.GetLength())
						{
							vProductSimpleSetupPassword = "357";
							CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction7Button);
							if (TempWindow)
								TempWindow->Invalidate(false);
							TempNoticeDialog.vNoticeText = "\n\nSimple Setup Password was blank\nSet to default";
							TempNoticeDialog.vType = cNoticeMessage;
							TempNoticeDialog.DoModal();
						}
						if (!vProductSetupPassword.GetLength())
						{
							vProductSetupPassword = "1479";
							CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction1Button);
							if (TempWindow)
								TempWindow->Invalidate(false);
							TempNoticeDialog.vNoticeText = "\n\nSetup Password was blank\nSet to default";
							TempNoticeDialog.vType = cNoticeMessage;
							TempNoticeDialog.DoModal();
						}
						if (!vCertifiedSetupPassword.GetLength())
						{
							vCertifiedSetupPassword = "147963";
							CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction2Button);
							if (TempWindow)
								TempWindow->Invalidate(false);
							TempNoticeDialog.vNoticeText = "\n\nCertified Setup Password was blank\nSet to default";
							TempNoticeDialog.vType = cNoticeMessage;
							TempNoticeDialog.DoModal();
						}
					}
				}
				else
				{
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\nConfirm Password does not match\n\nPassword not changed";
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}
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
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}	
	}
}

void CConfigurePasswordsDialog::OnSubFunction2Button() 
{
	// Set Reset Counters Password was pressed
	CPasswordEntryDialog IPasswordEntryDialog;  
	
	//Set dialog box data titles and number value
	IPasswordEntryDialog.vEditString = "";
	IPasswordEntryDialog.m_DialogTitleStaticText1 = "Set the Reset Counters/Disable Ejectors Password";
	IPasswordEntryDialog.m_DialogTitleStaticText2 = "";
	if (vOperatorPassword.GetLength())
		IPasswordEntryDialog.vLowerPasswordRequiresThisOne = true;
	//Pass control to dialog box and display
	int nResponse = IPasswordEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		CString TempPassword = IPasswordEntryDialog.vEditString;
		IPasswordEntryDialog.vEditString = "";
		IPasswordEntryDialog.vDisplayString = "";
		IPasswordEntryDialog.m_DialogTitleStaticText1 = "Enter again to Confirm the new";
		IPasswordEntryDialog.m_DialogTitleStaticText2 = "Reset Counters/Disable Ejectors Password";
		//Pass control to dialog box and display
		int nResponse = IPasswordEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			if (TempPassword == IPasswordEntryDialog.vEditString)
			{
				SetChangeMade();
				vResetCountsPassword = IPasswordEntryDialog.vEditString;

				int TempSize = 0;
				if (!vResetCountsPassword.GetLength())
				{
					TempSize = vOperatorPassword.GetLength();
				}
				if (TempSize)
				{
					vResetCountsPassword = "22";
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\nBecause a lower level password is set,\nthe Reset password can not be blank.\nThe Reset Password has been set to the default.";
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}
				else
				{
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nPassword successfully changed";
					TempNoticeDialog.vType = cNoticeMessage;
					TempNoticeDialog.DoModal();
				}

				CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction6Button);
				TempWindow->Invalidate(false);
				if (vResetCountsPassword.GetLength())
				{
					
					if (!vProductSimpleSetupPassword.GetLength())
					{
						vProductSimpleSetupPassword = "357";
						CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction7Button);
						if (TempWindow)
							TempWindow->Invalidate(false);

						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\nSimple Setup Password was blank\nSet to default";
						TempNoticeDialog.vType = cNoticeMessage;
						TempNoticeDialog.DoModal();
					}
					if (!vProductSetupPassword.GetLength())
					{
						vProductSetupPassword = "1479";
						CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction1Button);
						if (TempWindow)
							TempWindow->Invalidate(false);

						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\nSetup Password was blank\nSet to default";
						TempNoticeDialog.vType = cNoticeMessage;
						TempNoticeDialog.DoModal();
					}
					if (!vCertifiedSetupPassword.GetLength())
					{
						vCertifiedSetupPassword = "147963";
						CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction2Button);
						if (TempWindow)
							TempWindow->Invalidate(false);

						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\nCertified Setup Password was blank\nSet to default";
						TempNoticeDialog.vType = cNoticeMessage;
						TempNoticeDialog.DoModal();
					}
				}
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\nConfirm Password does not match\n\nPassword not changed";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
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
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CConfigurePasswordsDialog::OnSubFunction3Button() 
{
	// Set Sample Password was pressed
	if (PasswordOK(cInspxPassword,false))
	{
		CPasswordEntryDialog IPasswordEntryDialog;  
		
		//Set dialog box data titles and number value
		IPasswordEntryDialog.vEditString = "";
		IPasswordEntryDialog.m_DialogTitleStaticText1 = "Set the Sample Password";
		IPasswordEntryDialog.m_DialogTitleStaticText2 = "";
		//Pass control to dialog box and display
		int nResponse = IPasswordEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			CString TempPassword = IPasswordEntryDialog.vEditString;
			if (TempPassword.GetLength() < 6)
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\nPassword must be at least 6 characters long\n\nPassword not changed";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
			else
			{
				CString TempPassword = IPasswordEntryDialog.vEditString;
				IPasswordEntryDialog.vEditString = "";
				IPasswordEntryDialog.vDisplayString = "";
				IPasswordEntryDialog.m_DialogTitleStaticText1 = "Confirm the new Certified Setup Password";
				IPasswordEntryDialog.m_DialogTitleStaticText2 = "";
				//Pass control to dialog box and display
				int nResponse = IPasswordEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					if (TempPassword == IPasswordEntryDialog.vEditString)
					{
						SetChangeMade();
						vSamplePassword = IPasswordEntryDialog.vEditString;
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\n\nPassword successfully changed";
						TempNoticeDialog.vType = cNoticeMessage;
						TempNoticeDialog.DoModal();

						CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction3Button);
						if (TempWindow)
							TempWindow->Invalidate(false);
					}
					else
					{
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\nConfirm Password does not match\n\nPassword not changed";
						TempNoticeDialog.vType = cErrorMessage;
						TempNoticeDialog.DoModal();
					}
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
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
}

void CConfigurePasswordsDialog::OnSubFunction4Button() 
{
	// TODO: Add your control notification handler code here
	if (PasswordOK(cCertifiedPassword,false))
	{
		// Set setup Password was pressed
		CPasswordEntryDialog IPasswordEntryDialog;  
		
		//Set dialog box data titles and number value
		IPasswordEntryDialog.vEditString = "";
		IPasswordEntryDialog.m_DialogTitleStaticText1 = "Set the Sample Password";
		IPasswordEntryDialog.m_DialogTitleStaticText2 = "";
		IPasswordEntryDialog.vLowerPasswordRequiresThisOne = (bool)((vResetCountsPassword.GetLength()) || (vOperatorPassword.GetLength()));
		//Pass control to dialog box and display
		int nResponse = IPasswordEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			CString TempPassword = IPasswordEntryDialog.vEditString;
			IPasswordEntryDialog.vEditString = "";
			IPasswordEntryDialog.vDisplayString = "";
			IPasswordEntryDialog.m_DialogTitleStaticText1 = "Confirm the new Sample Password";
			IPasswordEntryDialog.m_DialogTitleStaticText2 = "";
			//Pass control to dialog box and display
			int nResponse = IPasswordEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				if (TempPassword == IPasswordEntryDialog.vEditString)
				{
					SetChangeMade();
					vProductSimpleSetupPassword = IPasswordEntryDialog.vEditString;

					int TempSize = 0;
					if (!vProductSimpleSetupPassword.GetLength())
					{
						TempSize = vResetCountsPassword.GetLength();
						if (!TempSize)
							TempSize = vOperatorPassword.GetLength();
					}
					if (TempSize)
					{
						vProductSimpleSetupPassword = "468";
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\nBecause a lower level password is set,\nthe Sample password can not be blank.\nThe Simple Setup Password has been set to the default.";
						TempNoticeDialog.vType = cErrorMessage;
						TempNoticeDialog.DoModal();
					}
					else
					{
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\n\nPassword successfully changed";
						TempNoticeDialog.vType = cNoticeMessage;
						TempNoticeDialog.DoModal();
					}

					if (!vProductSetupPassword.GetLength())
					{
						vProductSetupPassword = "1479";
						CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction1Button);
						if (TempWindow)
							TempWindow->Invalidate(false);

						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\nSetup Password was blank\nSet to default";
						TempNoticeDialog.vType = cNoticeMessage;
						TempNoticeDialog.DoModal();
					}
					if (!vCertifiedSetupPassword.GetLength())
					{
						vCertifiedSetupPassword = "147963";
						CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction3Button);
						if (TempWindow)
							TempWindow->Invalidate(false);

						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\nCertified Setup Password was blank\nSet to default";
						TempNoticeDialog.vType = cNoticeMessage;
						TempNoticeDialog.DoModal();
					}

					CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction7Button);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
				else
				{
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\nConfirm Password does not match\n\nPassword not changed";
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}
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
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
	/*
	if (PasswordOK(cSuperInspxPassword,false))
	{
		CPasswordEntryDialog IPasswordEntryDialog;  
		//Set dialog box data titles and number value
		IPasswordEntryDialog.vEditString = "";
		IPasswordEntryDialog.vDisplayString = "";
		IPasswordEntryDialog.m_DialogTitleStaticText1 = "Set the Inspx Reset Password";
		IPasswordEntryDialog.m_DialogTitleStaticText2 = "";
		IPasswordEntryDialog.vMinimumPasswordLength = 6;
		//Pass control to dialog box and display
		int nResponse = IPasswordEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			CString TempPassword = IPasswordEntryDialog.vEditString;

			IPasswordEntryDialog.vEditString = "";
			IPasswordEntryDialog.vDisplayString = "";
			IPasswordEntryDialog.m_DialogTitleStaticText1 = "Confirm the new Inspx Reset Password";
			IPasswordEntryDialog.m_DialogTitleStaticText2 = "";
			//Pass control to dialog box and display
			int nResponse = IPasswordEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				if (TempPassword == IPasswordEntryDialog.vEditString)
				{
					SetChangeMade();
					vSystemSetupResetPassword = IPasswordEntryDialog.vEditString;
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nPassword successfully changed";
					TempNoticeDialog.vType = cNoticeMessage;
					TempNoticeDialog.DoModal();
						CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction4Button);
						if (TempWindow)
							TempWindow->Invalidate(false);
				}
				else
				{
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\nConfirm Password does not match\n\nPassword not changed";
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}
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
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
	*/
}

void CConfigurePasswordsDialog::OnSubFunction5Button() 
{
		if (PasswordOK(cCertifiedPassword,false))
	{
		// Set setup Password was pressed
		CPasswordEntryDialog IPasswordEntryDialog;  
		
		//Set dialog box data titles and number value
		IPasswordEntryDialog.vEditString = "";
		IPasswordEntryDialog.m_DialogTitleStaticText1 = "Set the Setup Password";
		IPasswordEntryDialog.m_DialogTitleStaticText2 = "";
		IPasswordEntryDialog.vLowerPasswordRequiresThisOne = (bool)((vResetCountsPassword.GetLength()) || (vOperatorPassword.GetLength()));
		//Pass control to dialog box and display
		int nResponse = IPasswordEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			CString TempPassword = IPasswordEntryDialog.vEditString;
			IPasswordEntryDialog.vEditString = "";
			IPasswordEntryDialog.vDisplayString = "";
			IPasswordEntryDialog.m_DialogTitleStaticText1 = "Confirm the new Setup Password";
			IPasswordEntryDialog.m_DialogTitleStaticText2 = "";
			//Pass control to dialog box and display
			int nResponse = IPasswordEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				if (TempPassword == IPasswordEntryDialog.vEditString)
				{
					SetChangeMade();
					vProductSetupPassword = IPasswordEntryDialog.vEditString;

					CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction1Button);
					if (TempWindow)
						TempWindow->Invalidate(false);

					int TempSize = 0;
					if (!vProductSetupPassword.GetLength())
					{
						TempSize = vProductSimpleSetupPassword.GetLength();
						if (!TempSize)
							TempSize = vResetCountsPassword.GetLength();
						if (!TempSize)
							TempSize = vOperatorPassword.GetLength();
					}
					if (TempSize)
					{
						vProductSetupPassword = "1479";
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\nBecause a lower level password is set,\nthe Setup password can not be blank.\nThe Setup Password has been set to the default.";
						TempNoticeDialog.vType = cErrorMessage;
						TempNoticeDialog.DoModal();
					}
					else
					{
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\n\nPassword successfully changed";
						TempNoticeDialog.vType = cNoticeMessage;
						TempNoticeDialog.DoModal();
					}
					if (vProductSetupPassword.GetLength())
					{
						if (!vCertifiedSetupPassword.GetLength())
						{
							vCertifiedSetupPassword = vProductSetupPassword;
							CNoticeDialog TempNoticeDialog;
							TempNoticeDialog.vNoticeText = "\n\nCertified Setup Password was blank\nSet to password just entered";
							TempNoticeDialog.vType = cNoticeMessage;
							TempNoticeDialog.DoModal();
							CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction2Button);
							if (TempWindow)
								TempWindow->Invalidate(false);
						}
					}
				}
				else
				{
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\nConfirm Password does not match\n\nPassword not changed";
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}
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
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
}

void CConfigurePasswordsDialog::OnSubFunction6Button() 
{
	// Set QA Password was pressed
	if (PasswordOK(cInspxPassword,false))
	{
		CPasswordEntryDialog IPasswordEntryDialog;  
		
		//Set dialog box data titles and number value
		IPasswordEntryDialog.vEditString = "";
		IPasswordEntryDialog.m_DialogTitleStaticText1 = "Set the QA Password";
		IPasswordEntryDialog.m_DialogTitleStaticText2 = "0 = Auto Clearn Trend Alarm Red Status Indicator";
		//Pass control to dialog box and display
		int nResponse = IPasswordEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			CString TempPassword = IPasswordEntryDialog.vEditString;
			IPasswordEntryDialog.vEditString = "";
			IPasswordEntryDialog.vDisplayString = "";
			IPasswordEntryDialog.m_DialogTitleStaticText1 = "Confirm the new QA Password";
			IPasswordEntryDialog.m_DialogTitleStaticText2 = "";
			//Pass control to dialog box and display
			int nResponse = IPasswordEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				if (TempPassword == IPasswordEntryDialog.vEditString)
				{
					SetChangeMade();
					vQAPassword = IPasswordEntryDialog.vEditString;
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nPassword successfully changed";
					TempNoticeDialog.vType = cNoticeMessage;
					TempNoticeDialog.DoModal();

					CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction3Button);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
				else
				{
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\nConfirm Password does not match\n\nPassword not changed";
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}
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
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
	
}

void CConfigurePasswordsDialog::OnSubFunction7Button() 
{
	//advanced setup password entry
	if (PasswordOK(cInspxPassword,false))
	{
		// Set setup Password was pressed
		CPasswordEntryDialog IPasswordEntryDialog;  
		
		//Set dialog box data titles and number value
		IPasswordEntryDialog.vEditString = "";
		IPasswordEntryDialog.m_DialogTitleStaticText1 = "Set the Advanced Setup Password";
		IPasswordEntryDialog.m_DialogTitleStaticText2 = "";
		IPasswordEntryDialog.vLowerPasswordRequiresThisOne = false;//(bool)((vProductSetupPassword.GetLength()) || (vResetCountsPassword.GetLength()) || (vOperatorPassword.GetLength()));
		//Pass control to dialog box and display
		int nResponse = IPasswordEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			CString TempPassword = IPasswordEntryDialog.vEditString;
			
			if ((TempPassword.GetLength() > 0) && (TempPassword.GetLength() < 6))
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\nPassword must be at least 6 characters long\n\nPassword not changed";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
			else
			{
				IPasswordEntryDialog.vEditString = "";
				IPasswordEntryDialog.vDisplayString = "";
				IPasswordEntryDialog.m_DialogTitleStaticText1 = "Confirm the new Advanced Setup Password";
				IPasswordEntryDialog.m_DialogTitleStaticText2 = "";
				//Pass control to dialog box and display
				int nResponse = IPasswordEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					if (TempPassword == IPasswordEntryDialog.vEditString)
					{
						SetChangeMade();
						vAdvancedSetupPassword = IPasswordEntryDialog.vEditString;
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\n\nPassword successfully changed";
						TempNoticeDialog.vType = cNoticeMessage;
						TempNoticeDialog.DoModal();

						CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction2Button);
						if (TempWindow)
							TempWindow->Invalidate(false);
					}
					else
					{
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\nConfirm Password does not match\n\nPassword not changed";
						TempNoticeDialog.vType = cErrorMessage;
						TempNoticeDialog.DoModal();
					}
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
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
	
}

void CConfigurePasswordsDialog::OnSubFunction8Button() 
{
	
// Set Certified Setup Password was pressed
	if (PasswordOK(cInspxPassword,false))
	{
		CPasswordEntryDialog IPasswordEntryDialog;  
		
		//Set dialog box data titles and number value
		IPasswordEntryDialog.vEditString = "";
		IPasswordEntryDialog.m_DialogTitleStaticText1 = "Set the Certified Setup Password";
		IPasswordEntryDialog.m_DialogTitleStaticText2 = "";
		//Pass control to dialog box and display
		int nResponse = IPasswordEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			CString TempPassword = IPasswordEntryDialog.vEditString;
			if (TempPassword.GetLength() < 6)
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\nPassword must be at least 6 characters long\n\nPassword not changed";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
			else
			{
				CString TempPassword = IPasswordEntryDialog.vEditString;
				IPasswordEntryDialog.vEditString = "";
				IPasswordEntryDialog.vDisplayString = "";
				IPasswordEntryDialog.m_DialogTitleStaticText1 = "Confirm the new Certified Setup Password";
				IPasswordEntryDialog.m_DialogTitleStaticText2 = "";
				//Pass control to dialog box and display
				int nResponse = IPasswordEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					if (TempPassword == IPasswordEntryDialog.vEditString)
					{
						SetChangeMade();
						vCertifiedSetupPassword = IPasswordEntryDialog.vEditString;
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\n\nPassword successfully changed";
						TempNoticeDialog.vType = cNoticeMessage;
						TempNoticeDialog.DoModal();

						CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction3Button);
						if (TempWindow)
							TempWindow->Invalidate(false);
					}
					else
					{
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\nConfirm Password does not match\n\nPassword not changed";
						TempNoticeDialog.vType = cErrorMessage;
						TempNoticeDialog.DoModal();
					}
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
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
}

BOOL CConfigurePasswordsDialog::PreTranslateMessage(MSG* pMsg) 
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
				return true;
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
			case 36: // Home Key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
			case cPageDown: // Help
				PostThreadMessage(vGlobalCallingThreadID,
					cShowHelp, 3, 0);
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
			break;
			case cPageUp: // Clear, but do inspx hidden dialog
			{
				if (vGlobalPasswordLevel)
				{
					PasswordOK (cNoPassword, false);	//todo, search on PasswordOK (cNoPassword)
					HideInspxItems();
				}
				else
				{
					PasswordOK(cAnyPasswordToEnter, true);
					if (PasswordOK(cTemporaryInspxPassword, false))
						ShowInspxItems();
				}
				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			}
			break;
			//case 84: // T
			//	if (PasswordOK(cInspxPassword,false))
			//	vLocalSystemData->vImageTracerLineOn = 
			//		!vLocalSystemData->vImageTracerLineOn;
			//break;
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
		}
		return true;  //true indicates it has been handled, so do not process
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CConfigurePasswordsDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		if (!vOperatorPassword.GetLength())
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
	  }
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		if (!vProductSimpleSetupPassword.GetLength())
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
	  }
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}


	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);
		if (!vAdvancedSetupPassword.GetLength())
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;

	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		if (!vCertifiedSetupPassword.GetLength())
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		if (!vResetCountsPassword.GetLength())
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
	  }
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		if (!vProductSetupPassword.GetLength())
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
	  }
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
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
	if(pWnd->GetDlgCtrlID() == IDC_SubFunction4Button) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
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

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Display, 5);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function5Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Function5Display, 5);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}
	if (pWnd->GetDlgCtrlID() == IDC_CPMTimePeriodDisplay)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CPMTimePeriodDisplay, 5);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}
	if (pWnd->GetDlgCtrlID() == IDC_WorkStopTimePeriodDisplay)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_WorkStopTimePeriodDisplay, 5);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}
	if (pWnd->GetDlgCtrlID() == IDC_Clock)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Clock, 5);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}
	if (pWnd->GetDlgCtrlID() == IDC_Date)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Date, 5);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}
	return hbr;
}

void CConfigurePasswordsDialog::SetChangeMade() 
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

void CConfigurePasswordsDialog::ShowInspxItems()
{
	m_Function3Button.ShowWindow(SW_SHOW);

	if (PasswordOK(cInspxPassword,false))
	{
		m_SubFunction6Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction6Button, _T("Set\nQA Password"));

		m_SubFunction7Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction7Button, _T("Set Advanced Password"));

		m_SubFunction8Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction8Button, _T("Set Certified Password"));

		m_Function5Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_Function5Button, _T("Set Inspx Password"));
	}
	else
	{
		m_SubFunction6Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());

		m_SubFunction7Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());

		m_SubFunction8Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());

		m_Function5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
	}

	m_SubFunction1Button.ShowWindow(SW_SHOW);
	SetDlgItemText(IDC_SubFunction1Button, _T("Set Operator Password"));

	if (PasswordOK(cCertifiedPassword,false))
	{
		m_SubFunction5Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction5Button, _T("Set\nSetup Password"));

		m_SubFunction4Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction4Button, _T("Set Simple Setup Password"));
	}
	else
	{
		m_SubFunction5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());

		m_SubFunction4Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
	}
/*
	if (PasswordOK(cSuperInspxPassword,false))
	{
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		m_ArrowLeft4.ShowWindow(SW_SHOW);
	}
	else
	{
		m_SubFunction4Button.ShowWindow(SW_HIDE);
		m_ArrowLeft4.ShowWindow(SW_HIDE);
	}
	*/
}

void CConfigurePasswordsDialog::HideInspxItems()
{
	if (PasswordOK(cCertifiedPassword,false))
		m_Function3Button.ShowWindow(SW_SHOW);
	else
	{
		m_Function3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
	}

	m_SubFunction6Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());

	m_SubFunction7Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());

	m_SubFunction8Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());

//	m_SubFunction4Button.ShowWindow(SW_HIDE);
//	m_ArrowLeft4.ShowWindow(SW_HIDE);

	m_SubFunction1Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());

	if (PasswordOK(cCertifiedPassword,false))
	{
		m_SubFunction5Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction5Button, _T("Set\nSetup Password"));

		m_SubFunction4Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction4Button, _T("Set Simple Setup Password"));
	}
	else
	{
		m_SubFunction5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());

		m_SubFunction4Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
	}

	m_SubFunction8Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
}

void CConfigurePasswordsDialog::UpdateDisplay()
{
}

void CConfigurePasswordsDialog::OnDialogTitleStaticText() 
{
	if (PasswordOK(cSuperInspxPassword,false))
	{
		CString TempText = "Operator: " + vOperatorPassword;
		TempText = TempText + "\nReset Counter/Disable Ejectors: " + vResetCountsPassword;
		TempText = TempText + "\nSimple Setup: " + vProductSimpleSetupPassword;
		TempText = TempText + "\nSetup: " + vProductSetupPassword;
		TempText = TempText + "\nAdvanced Setup: " + vAdvancedSetupPassword;
		TempText = TempText + "\nCertified: " + vCertifiedSetupPassword;
		TempText = TempText + ",     QA: " + vQAPassword;
		//TempText = TempText + "\nInspx: " + vInspxPassword;
		//TempText = TempText + "\nInspx Reset: " + vSystemSetupResetPassword;

		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = TempText;
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}
}

void CConfigurePasswordsDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
  if (nIDEvent == vOneSecondTimerHandle)
  {
		if (vGlobalPasswordLevel != vOldPasswordLevel)
		{
			vOldPasswordLevel = vGlobalPasswordLevel;
			if (PasswordOK(cTemporaryInspxPassword,false))
				ShowInspxItems();
			else
				HideInspxItems();

			CWnd * TempWindow = NULL;
			TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
			if (TempWindow)
				TempWindow->Invalidate(false);
		}
	}
	CDialog::OnTimer(nIDEvent);
}


void CConfigurePasswordsDialog::OnDialogtitlestatictext1()
{
	if (vGlobalShiftKeyDown)
		OnDialogTitleStaticText();
	else
	{
		BYTE TempHiddenKeys = 8;
		if (!PasswordOK(cTemporaryInspxPassword,false))
			TempHiddenKeys = 10;
		PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
	}
}
