// CEditUser.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "afxdialogex.h"
#include "SystemConfigurationData.h"
#include "NumericEntryDialog.h"
#include "PasswordEntryDialog.h"
#include "AlphaEntryDialog.h"
#include "EditUser.h"
#include "selectitemdialog.h"
#ifndef DELETE_USER
#define DELETE_USER 2
#endif

extern unsigned long vGlobalDialogTitleColor;
extern int vGlobalPasswordLevel;
extern CScanTracDlg* vGlobalMainWindowPointer;
BYTE cStandardHidePasswordLength = 8;
CString cLevelName[] = {"None", "Spare Password 1", "Spare Password 2", "Spare Password 3", "Spare Password 4", "Spare Password 5", "Ask For Password", "Spare Password 6", "Spare Password 7", 
"Spare Password 8", "Spare Password 9", "Spare Password 10", "Operator", "Spare Password 11", "Spare Password 12", "Spare Password 13", "Spare Password 14", "Spare Password 15", "Counters-Ejectors ",
"Spare Password 16", "Spare Password 17", "Spare Password 18", "Spare Password 19", "Spare Password 20",  "Sample", "Spare Password 21", "Spare Password 22", "Spare Password 23", "Spare Password 24", 
"Spare Password 25", "Simple Setup", "Spare Password 26", "Spare Password 27", "Spare Password 28", "Spare Password 29", "Spare Password 30", "Setup", "Spare Password 31", "Spare Password 32", "Spare Password 33",
"Spare Password 34", "Spare Password 35", "QA", "Spare Password 36", "Spare Password 37", "Spare Password 38", "Spare Password 39", "Spare Password 40", "Temporary Advanced ", "Spare Password 41", 
"Spare Password 42", "Spare Password 43", "Spare Password 44", "Spare Password 45", "Advanced Setup", "Spare Password 46", "Spare Password 47", "Spare Password 48", "Spare Password 49", "Spare Password 50", 
"Certified", "Spare Password 51", "Spare Password 52", "Spare Password 53", "Spare Password 54", "Spare Password 55", "Temporary Inspx", "Spare Password 56", "Spare Password 57", "Spare Password 58", "Spare Password 59",
"Spare Password 60", "Allan", "Spare Password 61", "Spare Password 62", "Spare Password 63", "Spare Password 64", "Spare Password 65", "Inspx", "Spare Password 66", "Spare Password 67", "Spare Password 68", 
"Spare Password 69", "Spare Password 70", "Super Inspx"};
// CSetupUserDialog dialog
/*cNoPassword, cSparePassword1, cSparePassword2, cSparePassword3, cSparePassword4, cSparePassword5, cAnyPasswordToEnter, cSparePassword6, cSparePassword7, cSparePassword8,
	cSparePassword9, cSparePassword10, cOperatorPassword, cSparePassword11, cSparePassword12, cSparePassword13, cSparePassword14, cSparePassword15, cResetPassword,
	cSparePassword16, cSparePassword17, cSparePassword18, cSparePassword19, cSparePassword20, cSamplePassword, cSparePassword21, cSparePassword22, cSparePassword23, 
	cSparePassword24, cSparePassword25, cSimpleSetupPassword, cSparePassword26, cSparePassword27, cSparePassword28, cSparePassword29, cSparePassword30, cSetupPassword, 
	cSparePassword31, cSparePassword32, cSparePassword33, cSparePassword34, cSparePassword35, cQAPassword, cSparePassword36, cSparePassword37, cSparePassword38, cSparePassword39,
	cSparePassword40, cTemporaryAdvancedPassword, cSparePassword41, cSparePassword42, cSparePassword43, cSparePassword44, cSparePassword45, cAdvancedSetupPassword, 
	cCertifiedPassword, cSparePassword47, cSparePassword48, cSparePassword49, cSparePassword50,	cTemporaryInspxPassword, cSparePassword51, cSparePassword52, cSparePassword53,
	cSparePassword54, cSparePassword55, cAllansPassword, cSparePassword56, cSparePassword57,cSparePassword58,cSparePassword59,cSparePassword60,cInspxPassword, 
	cSparePassword61, cSparePassword62, cSparePassword63, cSparePassword64, cSparePassword65, cSuperInspxPassword
*/

IMPLEMENT_DYNAMIC(CEditUser, CDialog)

CEditUser::CEditUser(CWnd* pParent /*=NULL*/)
	: CDialog(CEditUser::IDD, pParent)
{
	vNewUser = true;
	vChangeMade = false;
	vUserClickedPassword = false;
	vCurrentUser.vUserName = "";
	vCurrentUser.vPassword = "";
	vCurrentUser.vID = "";
	vCurrentUser.vLevel = 1;
	vCurrentUser.vBCID = 0;
	vCurrentUser.vRFID = 0;
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction1Display);
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

CEditUser::~CEditUser()
{
}

BEGIN_MESSAGE_MAP(CEditUser, CDialog)
//	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
	ON_BN_CLICKED(IDC_SubFunction1Button, OnSubFunction1Button)
	ON_BN_CLICKED(IDC_Function5Button, OnFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction2Button, OnSubFunction2Button)
	ON_BN_CLICKED(IDC_SubFunction3Button, OnSubFunction3Button)
	ON_BN_CLICKED(IDC_SubFunction4Button, OnSubFunction4Button)
	ON_BN_CLICKED(IDC_SubFunction5Button, OnSubFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction6Button, OnSubFunction6Button)
	ON_BN_CLICKED(IDC_SubFunction7Button, OnSubFunction7Button)
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_WM_SHOWWINDOW()
	ON_STN_CLICKED(IDC_SubFunction3Display, &CEditUser::OnSubfunction3display)
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CEditUser::OnDialogtitlestatictext1)
END_MESSAGE_MAP()


// CSetupUserDialog message handlers


/////////////////////////////////////////////////////////////////////////////
// CContaminant22EditThresholdsDialog dialog


void CEditUser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContaminant22EditThresholdsDialog)
	//  DDX_Control(pDX, IDC_ArrowDown4, m_ArrowDown4);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction1Display, m_SubFunction1Display);
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
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
}

void CEditUser::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

	this->SetWindowText(_T("Edit User"));

	CString TempButtonString;
	
	TempButtonString.LoadString(IDS_Exit);
	SetDlgItemText(IDC_Function2Button,TempButtonString);

	if (vShowMainMenuButton)
	{
		TempButtonString.LoadString(IDS_MainMenu);
		SetDlgItemText(IDC_Function3Button,TempButtonString);
		m_Function3Button.ShowWindow(SW_SHOW);
	}
	else
	{
		m_Function3Button.EnableWindow(false);
		m_Function3Button.ShowWindow(false);
	}

	TempButtonString.LoadString(IDS_Cancel);
	SetDlgItemText(IDC_Function4Button,TempButtonString);

	TempButtonString.LoadString(IDS_Delete);
	SetDlgItemText(IDC_Function5Button,TempButtonString);

	m_Function2Button.ShowWindow(SW_SHOW);
	
	if (!vNewUser)
	{
		m_Function5Button.ShowWindow(SW_SHOW);
	}
	else
	{
		m_Function5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
	}

	m_Function1Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_Function1Button.GetRectToInvalidateArrow());
	m_Function4Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());

	m_SubFunction7Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
	m_SubFunction7Display.ShowWindow(SW_HIDE);
	m_SubFunction8Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
	m_SubFunction8Display.ShowWindow(SW_HIDE);

	SetupMenu(vLocalCWndCollection);
	UpdateDisplay();
}

void CEditUser::OnFunction1Button() 
{
	//No button
}

void CEditUser::OnFunction2Button() 
{
	//(Save and) Exit
	if (vChangeMade == true)
	{
		bool TempPasswordOkay = false;
		if (vCurrentUser.vLevel <= cSimpleSetupPassword)
			TempPasswordOkay = true;
		else if (vCurrentUser.vLevel >= cSimpleSetupPassword)
			TempPasswordOkay = true;
		if (vCurrentUser.vUserName != "" && TempPasswordOkay)
		{	
			CDialog::EndDialog(IDOK);
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText("");
			if (vCurrentUser.vUserName == "" && vCurrentUser.vPassword == "")
			{
				TempText = "\n\n\nYou must enter an username and a password!";
			}
			else 
			if (vCurrentUser.vUserName == "")
			{
				TempText = "\n\n\nYou must enter an username!";
			}
			else 
			if (vCurrentUser.vPassword.GetLength() > 1)
			{
				TempText = "\n\n\nYou must enter a password!";
			}
			else 
			if (vCurrentUser.vLevel <= cSimpleSetupPassword && !TempPasswordOkay)
			{
				TempText = "\n\n\nYou must enter a password with three or more dights!";
			}
			else 
			if (vCurrentUser.vLevel >= cSimpleSetupPassword && !TempPasswordOkay)
			{
				TempText = "\n\n\nYou must enter a password with four or more dights!";
			}
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
		CDialog::EndDialog(false);
	UpdateDisplay();
}

void CEditUser::OnFunction3Button() 
{
	//Main Menu
	if (vShowMainMenuButton)
		CDialog::EndDialog(10);
}

void CEditUser::OnFunction4Button() 
{
	//Cancel
	CDialog::EndDialog(false);
}

void CEditUser::OnFunction5Button() 
{
	//Delete
	//Prompt confirmation box

	//End Dialog
	CDialog::EndDialog(DELETE_USER);
}

void CEditUser::OnSubFunction1Button() 
{
	CAlphaEntryDialog IAlphaEntryDialog;
	if (vCurrentUser.vUserName == "")
		vCurrentUser.vUserName = "Enter New User Name";
	IAlphaEntryDialog.vEditString = vCurrentUser.vUserName;

	IAlphaEntryDialog.m_DialogTitleStaticText = "Username";
	IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;

	IAlphaEntryDialog.m_DialogTitleStaticText2 = vCurrentUser.vUserName;
	IAlphaEntryDialog.vAllowSlashes = true;
	IAlphaEntryDialog.vAllowMoreThan32Characters = true;
	IAlphaEntryDialog.vBlankNameOK = false;

	IAlphaEntryDialog.vLocalSystemData = this->vLocalSystemData;
	int nResponse = IAlphaEntryDialog.DoModal();

	if (nResponse == IDOK)
	{
		if (FindUserName(IAlphaEntryDialog.vEditString) && IAlphaEntryDialog.vEditString != vCurrentUser.vUserName)
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText("\n\n\nInvalid user name! Please enter another.");
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
			OnSubFunction1Button();
			return;
		}
		else
		{
		vCurrentUser.vUserName = IAlphaEntryDialog.vEditString;
		vChangeMade = true;
		}
	}
	else if (nResponse == 10)
		CDialog::EndDialog(10);
	UpdateDisplay();
}

void CEditUser::OnSubFunction2Button() 
{
	//Open up new dialog that allows user to select level
	/*vCurrentUser.vLevel++;
	if (vCurrentUser.vLevel > vGlobalPasswordLevel)
		vCurrentUser.vLevel = 1;
	vChangeMade = true;
	UpdateDisplay();*/
	CSelectItemDialog ISelectItemDialog;  
	//Set dialog box data titles and number value
	if (vCurrentUser.vUserName != "")
	{
		ISelectItemDialog.vTitleString = "Choose level to set current user to";
		if (vCurrentUser.vLevel == 0)
			ISelectItemDialog.m_DialogTitleStaticText2 = vCurrentUser.vUserName + " has no level.";
		else
			ISelectItemDialog.m_DialogTitleStaticText2 = vCurrentUser.vUserName + "'s level is " + cLevelName[vCurrentUser.vLevel];
	}
	else
	{
		ISelectItemDialog.vTitleString = "Choose level to set current user to";
		if (vCurrentUser.vLevel == 0)
			ISelectItemDialog.m_DialogTitleStaticText2 = "Current user has no level.";
		else
			ISelectItemDialog.m_DialogTitleStaticText2 = "Current user level is " + cLevelName[vCurrentUser.vLevel];
	}
	
	ISelectItemDialog.m_DialogTitleStaticText3 = "";
	ISelectItemDialog.vBackgroundColor = cGreen;
	BYTE TempGreenLevel = vCurrentUser.vLevel;
	switch (TempGreenLevel)
	{
		case 6:
			TempGreenLevel = TempGreenLevel - 1;
		break;
		case 7:
			TempGreenLevel = TempGreenLevel - 1;
		break;
		default:
			TempGreenLevel = TempGreenLevel;
		break;
	}
	ISelectItemDialog.vGreenItem = TempGreenLevel;
	ISelectItemDialog.vShowExit = 0;
	ISelectItemDialog.vInAutoSetup = vLocalSystemData->vInAutoSetup;

	ISelectItemDialog.vNumberOfItems = 0;
	ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = cLevelName[cOperatorPassword];
	ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = cLevelName[cResetPassword];
	ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = cLevelName[cSimpleSetupPassword];
	ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = cLevelName[cSetupPassword];
	ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = cLevelName[cQAPassword];
	ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = cLevelName[cAdvancedSetupPassword];
	ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = cLevelName[cCertifiedPassword];

	bool TempBool = false;
	if (vGlobalMainWindowPointer->vConfigurationData->vProductSetupPassword.GetLength() < 1)
	{
		ISelectItemDialog.vItemEnabled[3] = false;
		TempBool = true;
	}
	if (vGlobalMainWindowPointer->vConfigurationData->vProductSimpleSetupPassword.GetLength() < 1 || TempBool)
	{
		ISelectItemDialog.vItemEnabled[2] = false;
		TempBool = true;
	}
	if (vGlobalMainWindowPointer->vConfigurationData->vResetCountsPassword.GetLength() < 1 || TempBool)
	{
		ISelectItemDialog.vItemEnabled[1] = false;
	}
	if (vGlobalMainWindowPointer->vConfigurationData->vOperatorPassword.GetLength() < 1 || TempBool)
	{
		ISelectItemDialog.vItemEnabled[0] = false;
		TempBool = true;
	}

	ISelectItemDialog.vMakeTitle3Big = false;
	int nResponse = ISelectItemDialog.DoModal();
	if (nResponse == IDOK)
	{
		 switch (ISelectItemDialog.vItemNumberSelected - 1)
		 {
			case 0:
				vCurrentUser.vLevel = cOperatorPassword;
			break;
			case 1:
				vCurrentUser.vLevel = cResetPassword;
			break;
			case 2:
				vCurrentUser.vLevel = cSimpleSetupPassword;
			break;
			case 3:
				vCurrentUser.vLevel = cSetupPassword;
			break;
			case 4:
				vCurrentUser.vLevel = cQAPassword;
			break;
			case 5:
				vCurrentUser.vLevel = cAdvancedSetupPassword;
			break;
			case 6:
				vCurrentUser.vLevel = cCertifiedPassword;
			break;
		}
		vChangeMade = true;
	}
	if (nResponse == 10)
		CDialog::EndDialog(10);

	UpdateDisplay();
}

void CEditUser::OnSubFunction3Button() 
{
	CPasswordEntryDialog IPasswordEntryDialog;  
	IPasswordEntryDialog.m_DialogTitleStaticText1 = "User: " + vCurrentUser.vUserName;
	IPasswordEntryDialog.m_DialogTitleStaticText2 = "Level: " + cLevelName[vCurrentUser.vLevel];
	int nResponse = IPasswordEntryDialog.DoModal();
	if (nResponse == IDOK)
	{
		if (PasswordExist(*vLocalUserCollection,IPasswordEntryDialog.vEditString) || 
			IPasswordEntryDialog.vEditString == "7205" || IPasswordEntryDialog.vEditString == "72054328" ||
			IPasswordEntryDialog.vEditString == "31415926" || IPasswordEntryDialog.vEditString == "5104682199" ||
			IPasswordEntryDialog.vEditString == "304259" || IPasswordEntryDialog.vEditString == "10221022" ||
			IPasswordEntryDialog.vEditString == "147963" || IPasswordEntryDialog.vEditString == "357" ||
			IPasswordEntryDialog.vEditString == "1479" ||
			DisposablePasswordOK(IPasswordEntryDialog.vEditString, cTemporaryInspxPassword))
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText("\n\n\nPassword already used! Please enter another.");
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
			OnSubFunction3Button();
			return;
		}
		else if ((IPasswordEntryDialog.vEditString.GetLength() < 3 && vCurrentUser.vLevel <= cSimpleSetupPassword) ||
				 (IPasswordEntryDialog.vEditString.GetLength() < 4 && vCurrentUser.vLevel > cSimpleSetupPassword))
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText("\n\n\nPassword is too short! Please enter another.");
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
			OnSubFunction3Button();
			return;
		}
		else
		{
		vCurrentUser.vPassword = IPasswordEntryDialog.vEditString; 
		vChangeMade = true;
		}
	}
	else if (nResponse == 10)
		CDialog::EndDialog(10);
	UpdateDisplay();
}

void CEditUser::OnSubFunction4Button() 
{
	CAlphaEntryDialog IAlphaEntryDialog;
	IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
	if (vCurrentUser.vID == "")
		vCurrentUser.vID = "NICK";
	else
		IAlphaEntryDialog.m_DialogTitleStaticText2 = vCurrentUser.vID;
	IAlphaEntryDialog.vEditString = vCurrentUser.vID;

	IAlphaEntryDialog.m_DialogTitleStaticText = "ID";
	IAlphaEntryDialog.vAllowSlashes = true;
	IAlphaEntryDialog.vAllowMoreThan32Characters = true;
	IAlphaEntryDialog.vBlankNameOK = false;

	IAlphaEntryDialog.vLocalSystemData = this->vLocalSystemData;
	int nResponse = IAlphaEntryDialog.DoModal();

	if (nResponse == IDOK)
	{
		vCurrentUser.vID = IAlphaEntryDialog.vEditString;
		vChangeMade = true;
	}
	else if (nResponse == 10)
		CDialog::EndDialog(10);
	UpdateDisplay();
}

void CEditUser::OnSubFunction5Button() 
{
	CNumericEntryDialog INumericEntryDialog;  
	INumericEntryDialog.vMaxValue = 4294967295; //(DWORD MAX)
	INumericEntryDialog.vMinValue = 0;
	int nResponse = INumericEntryDialog.DoModal();
	if (nResponse == IDOK)
	{
		vCurrentUser.vBCID = _wtoi(INumericEntryDialog.vEditString); 
		vChangeMade = true;
	}
	else if (nResponse == 10)
		CDialog::EndDialog(10);
	UpdateDisplay();
}

void CEditUser::OnSubFunction6Button() 
{
	CNumericEntryDialog INumericEntryDialog;  
	INumericEntryDialog.vMaxValue = 4294967295; //(DWORD MAX)
	INumericEntryDialog.vMinValue = 0;
	int nResponse = INumericEntryDialog.DoModal();
	if (nResponse == IDOK)
	{
		vCurrentUser.vRFID = _wtoi(INumericEntryDialog.vEditString); 
		vChangeMade = true;
	}
	else if (nResponse == 10)
		CDialog::EndDialog(10);
	UpdateDisplay();
}

void CEditUser::OnSubFunction7Button() 
{
	//No button
}

void CEditUser::OnSubFunction8Button() 
{
	//No button
}

HBRUSH CEditUser::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
		if (vCurrentUser.vUserName.GetLength() == 0)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5);  //5 is medium large
		if (vCurrentUser.vUserName.GetLength() == 0)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
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
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
		if (vCurrentUser.vPassword.GetLength() == 0)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Display, 5);
		if (vCurrentUser.vPassword.GetLength() == 0)
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
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Display, 5);
	return hbr;
}

BOOL CEditUser::PreTranslateMessage(MSG* pMsg) 
{
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
		CWnd *TempWindow = NULL;
		int TempKey = pMsg->wParam;
		//if (vGlobalShiftKeyDown)
		//	PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
		//else
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
			/*
			case 36:  //Home key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
			*/
			case cPageDown: // Help
				PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 0, 0);

				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			break;
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
		}
		return true;  //true indicates it has been handled, so do not process
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CEditUser::UpdateDisplay()
{
	Invalidate(1);
	for (BYTE TempLoop = 0; TempLoop  < 6; TempLoop++)
	{
		switch (TempLoop)
		{
			case 0:
			{
				CString TempFunctionString = "Username"; 
				CString TempBoxString(vCurrentUser.vUserName);
				m_SubFunction1Button.ShowWindow(SW_SHOW);
				m_SubFunction1Display.ShowWindow(SW_SHOW);
				SetDlgItemText(IDC_SubFunction1Display,TempBoxString);
				SetDlgItemText(IDC_SubFunction1Button,TempFunctionString);
			}
			break;
			case 1:
			{
				CString TempFunctionString = "Level";
				CString TempBoxString = cLevelName[vCurrentUser.vLevel];
				m_SubFunction2Button.ShowWindow(SW_SHOW);
				m_SubFunction2Display.ShowWindow(SW_SHOW);
				SetDlgItemText(IDC_SubFunction2Display,TempBoxString);
				SetDlgItemText(IDC_SubFunction2Button,TempFunctionString);
			}
			break;
			case 2:
			{
				CString TempFunctionString = "Password";
				CString TempBoxString("");
				if (vUserClickedPassword)
				{
					CString TempText(vCurrentUser.vPassword);
					TempBoxString = TempText;
				}
				else
				{
					CString TempText('*', (vCurrentUser.vPassword.GetLength() == 0 ? 0 : cStandardHidePasswordLength));
					TempBoxString = TempText;
				}
				m_SubFunction3Button.ShowWindow(SW_SHOW);
				m_SubFunction3Display.ShowWindow(SW_SHOW);
				SetDlgItemText(IDC_SubFunction3Display,TempBoxString);
				SetDlgItemText(IDC_SubFunction3Button,TempFunctionString);
			}
			break;
			case 3:
			{
				CString TempFunctionString = "ID";
				CString TempBoxString = vCurrentUser.vID;
				m_SubFunction4Button.ShowWindow(SW_SHOW);
				m_SubFunction4Display.ShowWindow(SW_SHOW);
				SetDlgItemText(IDC_SubFunction4Display,TempBoxString);
				SetDlgItemText(IDC_SubFunction4Button,TempFunctionString);
			}
			break;
			case 4:
			{
				CString TempFunctionString = "BCID";
				char TempBuffer[16];
				_itoa_s(vCurrentUser.vBCID, TempBuffer, 16, 10);
				CString TempBoxString = TempBuffer;
				m_SubFunction5Button.ShowWindow(SW_SHOW);
				m_SubFunction5Display.ShowWindow(SW_SHOW);
				SetDlgItemText(IDC_SubFunction5Display,TempBoxString);
				SetDlgItemText(IDC_SubFunction5Button,TempFunctionString);
			}
			break;
			case 5:
			{
				CString TempFunctionString = "RFID";
				char TempBuffer[16];
				_itoa_s(vCurrentUser.vRFID, TempBuffer, 16, 10);
				CString TempBoxString = TempBuffer;
				m_SubFunction6Button.ShowWindow(SW_SHOW);
				m_SubFunction6Display.ShowWindow(SW_SHOW);
				SetDlgItemText(IDC_SubFunction6Display,TempBoxString);
				SetDlgItemText(IDC_SubFunction6Button,TempFunctionString);
			}
			break;
			//case 6:
			//{
			//	
			//}
			//break;
			//case 7:
			//{
			//	
			//}
			//break;
		}
	}
	if (vChangeMade == true)
	{
		CString TempButtonString;
		TempButtonString.LoadString(IDS_SaveAndExit);
		SetDlgItemText(IDC_Function2Button,TempButtonString);
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_Function4Button.ShowWindow(SW_SHOW);
	}

	CString TempText;
	if (vCurrentUser.vUserName == "")
		TempText = "Edit User";
	else
		TempText = "Edit " + vCurrentUser.vUserName;
	SetDlgItemText(IDC_DialogTitleStaticText1, TempText);
}



void CEditUser::OnSubfunction3display()
{
	if (vGlobalPasswordLevel >= cTemporaryInspxPassword)
	{
		vUserClickedPassword = !vUserClickedPassword;
		UpdateDisplay();
	}
}

INT_PTR CEditUser::DoModal()
{
	if (!vLocalSystemData || !vLocalUserCollection)
		OnCancel();
	return CDialog::DoModal();
}

bool CEditUser::FindUserName(CString TempUserName)
{
	for (POSITION TempPosition = vLocalUserCollection->GetHeadPosition(); TempPosition;)
		if ((vLocalUserCollection->GetNext(TempPosition))) 
		if (TempPosition) 
		if ((vLocalUserCollection->GetAt(TempPosition))->vUserName.CompareNoCase(TempUserName) == 0) 
			return true;
	return false;
}

void CEditUser::OnDialogtitlestatictext1()
{
	//bring up soft key menu for help, print screen, Image, Clear, and hidden shift key
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
