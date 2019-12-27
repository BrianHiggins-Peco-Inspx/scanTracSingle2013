// UsersDialog.cpp : Implementation of CUsersDialog

#include "stdafx.h"
#include "ScanTrac.h"
#include "SetupUserDialog.h"
#include "afxdialogex.h"
#include "UsersDialog.h"

extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
extern int vGlobalPasswordLevel;
const extern WORD cOldResolutionX;
const extern WORD cOldResolutionY;
extern CScanTracDlg* vGlobalMainWindowPointer;
extern CString cLevelName[];

// CUsersDialog
BEGIN_MESSAGE_MAP(CUsersDialog, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_Function1Button, &CUsersDialog::OnFunction1Button)
	ON_STN_CLICKED(IDC_Function2Button, &CUsersDialog::OnFunction2Button)
	ON_STN_CLICKED(IDC_Function3Button, &CUsersDialog::OnFunction3Button)
	ON_STN_CLICKED(IDC_Function4Button, &CUsersDialog::OnFunction4Button)
	ON_STN_CLICKED(IDC_Function5Button, &CUsersDialog::OnFunction5Button)
	ON_STN_CLICKED(IDC_SubFunction1Button, &CUsersDialog::OnSubFunction1Button)
	ON_STN_CLICKED(IDC_SubFunction2Button, &CUsersDialog::OnSubFunction2Button)
	ON_STN_CLICKED(IDC_SubFunction3Button, &CUsersDialog::OnSubFunction3Button)
	ON_STN_CLICKED(IDC_SubFunction4Button, &CUsersDialog::OnSubFunction4Button)
	ON_STN_CLICKED(IDC_SubFunction5Button, &CUsersDialog::OnSubFunction5Button)
	ON_STN_CLICKED(IDC_SubFunction6Button, &CUsersDialog::OnSubFunction6Button)
	ON_STN_CLICKED(IDC_SubFunction7Button, &CUsersDialog::OnSubFunction7Button)
	ON_STN_CLICKED(IDC_SubFunction8Button, &CUsersDialog::OnSubFunction8Button)
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CUsersDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

CUsersDialog::CUsersDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CUsersDialog::IDD, pParent)
{
	vChangeMade = false;
	vCurrentNumberOfUsers = 0;
	//vCurrentMaxNumberOfUsers = 1;
	for (BYTE TempLoop = 0; TempLoop < cSuperInspxPassword; TempLoop++)
		vNumberOfLevelList[TempLoop] = 0;
	//vUserList = new UserPasswords [vCurrentMaxNumberOfUsers];
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Background			 );
	vLocalCWndCollection.Add(&m_SubFunction1Button	 );
	vLocalCWndCollection.Add(&m_SubFunction2Button	 );
	vLocalCWndCollection.Add(&m_SubFunction3Button	 );
	vLocalCWndCollection.Add(&m_SubFunction4Button	 );
	vLocalCWndCollection.Add(&m_SubFunction5Button	 );
	vLocalCWndCollection.Add(&m_SubFunction6Button	 );
	vLocalCWndCollection.Add(&m_SubFunction7Button	 );
	vLocalCWndCollection.Add(&m_SubFunction8Button	 );
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button		 );
	vLocalCWndCollection.Add(&m_Function2Button		 );
	vLocalCWndCollection.Add(&m_Function3Button		 );
	vLocalCWndCollection.Add(&m_Function4Button		 );
	vLocalCWndCollection.Add(&m_Function5Button		 );
	//vLocalCWndCollection.Add(&m_SubFunction1Display	 );
	//vLocalCWndCollection.Add(&m_SubFunction2Display	 );
	//vLocalCWndCollection.Add(&m_SubFunction3Display	 );
	//vLocalCWndCollection.Add(&m_SubFunction4Display	 );
	//vLocalCWndCollection.Add(&m_SubFunction5Display	 );
	//vLocalCWndCollection.Add(&m_SubFunction6Display	 );
	//vLocalCWndCollection.Add(&m_SubFunction7Display	 );
	//vLocalCWndCollection.Add(&m_SubFunction8Display	 );
}

CUsersDialog::~CUsersDialog()
{
}

void CUsersDialog::UpdateUsersData(CSetupUserDialog &TempSetupUserDialog)
{
	if (TempSetupUserDialog.vChangeMade)
	{
		vChangeMade = true;
		//m_Function4Button.ShowWindow(SW_SHOW);
		//m_ArrowDown4.ShowWindow(SW_SHOW);
		//CString TempText = "-";
		//TempText.LoadString(IDS_Cancel);
		//SetDlgItemText(IDC_Function2Button,TempText);
		//m_Function2Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		//TempText.LoadString(IDS_SaveAndExit);
		//SetDlgItemText(IDC_Function2Button,TempText);
	}

	vLocalUserCollection.RemoveAll();
	vCurrentNumberOfUsers = TempSetupUserDialog.vLocalUserCollection.GetCount();
	SetDlgItemText(IDC_Background, dtoa(vCurrentNumberOfUsers, 0) + " Users");

	for (WORD TempLoop = 0; TempLoop < cSuperInspxPassword; TempLoop++)
		vNumberOfLevelList[TempLoop] = 0;
	for (POSITION TempPosition = TempSetupUserDialog.vLocalUserCollection.GetHeadPosition(); TempPosition; TempSetupUserDialog.vLocalUserCollection.GetNext(TempPosition))
	if (TempPosition) 
	{
		vLocalUserCollection.AddTail(TempSetupUserDialog.vLocalUserCollection.GetAt(TempPosition));
		switch ((vLocalUserCollection.GetTail())->vLevel)
		{
			case cOperatorPassword:
				vNumberOfLevelList[cOperatorPassword]++;
			break;
			case cResetPassword:
				vNumberOfLevelList[cResetPassword]++;
			break;
			case cSamplePassword:
				vNumberOfLevelList[cSamplePassword]++;
			break;
			case cSimpleSetupPassword:
				vNumberOfLevelList[cSimpleSetupPassword]++;
			break;
			case cSetupPassword:
				vNumberOfLevelList[cSetupPassword]++;
			break;
			case cQAPassword:
				vNumberOfLevelList[cQAPassword]++;
			break;
			//No Case 5 for Temporary Advanced Password Users
			case cAdvancedSetupPassword:
				vNumberOfLevelList[cAdvancedSetupPassword]++;
			break;
			case cCertifiedPassword:
				vNumberOfLevelList[cCertifiedPassword]++;
			break;
			default:
				;//do nothing
			break;
		}
	}
	vCurrentNumberOfUsers = vLocalUserCollection.GetCount();
	SetDlgItemText(IDC_Background, dtoa(vCurrentNumberOfUsers, 0) + " Users");
}

BYTE CUsersDialog::FindLevelFromPassword(CString Password)
{
	//for (BYTE TempLoop = 0; TempLoop < vCurrentNumberOfUsers; TempLoop++)
	//{
	//	if (vUserList[TempLoop].vPassword == Password)
	//		return vUserList[TempLoop].vLevel; 
	//}
	return cNoPassword;
}

CString CUsersDialog::FindUserFromPassword(CString Password)
{
	//for (BYTE TempLoop = 0; TempLoop < vCurrentNumberOfUsers; TempLoop++)
	//{
	//	if (vUserList[TempLoop].vPassword == Password)
	//		return vUserList[TempLoop].vUserName; 
	//}
	return "Error";
}

void CUsersDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	LoadUserData();
	this->SetWindowText(_T("Users"));
	UpdateDisplay();
	::SetupMenu(vLocalCWndCollection);
}

HBRUSH CUsersDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Background, 5);  //5 is medium large
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
	if (pWnd->GetDlgCtrlID() == IDC_Item1)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Item2)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Item3)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Item4)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Item5)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Item6)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Item7)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Item8)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Display, 5);

	//if (pWnd->GetDlgCtrlID() == IDC_Item1)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	if (vGlobalPasswordLevel >= 1)
	//		return vLocalSystemData->vGreenBrush;
	//}
	//if (pWnd->GetDlgCtrlID() == IDC_Item2)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	if (vGlobalPasswordLevel >= 2)
	//		return vLocalSystemData->vGreenBrush;
	//}
	//if (pWnd->GetDlgCtrlID() == IDC_Item3)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	if (vGlobalPasswordLevel >= 3)
	//		return vLocalSystemData->vGreenBrush;
	//}
	//if (pWnd->GetDlgCtrlID() == IDC_Item4)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	if (vGlobalPasswordLevel >= 4)
	//		return vLocalSystemData->vGreenBrush;
	//}
	//if (pWnd->GetDlgCtrlID() == IDC_Item5)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	if (vGlobalPasswordLevel >= 5)
	//		return vLocalSystemData->vGreenBrush;
	//}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}


void CUsersDialog::DoDataExchange(CDataExchange* pDX)
{
	// TODO: Add your specialized code here and/or call the base class
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Item1, m_SubFunction1Display);
	DDX_Control(pDX, IDC_Item2, m_SubFunction2Display);
	DDX_Control(pDX, IDC_Item3, m_SubFunction3Display);
	DDX_Control(pDX, IDC_Item4, m_SubFunction4Display);
	DDX_Control(pDX, IDC_Item5, m_SubFunction5Display);
	DDX_Control(pDX, IDC_Item6, m_SubFunction6Display);
	DDX_Control(pDX, IDC_Item7, m_SubFunction7Display);
	DDX_Control(pDX, IDC_Item8, m_SubFunction8Display);
	DDX_Control(pDX, IDC_CheckMarkGraphic1, m_CheckMarkGraphic1);
	DDX_Control(pDX, IDC_CheckMarkGraphic2, m_CheckMarkGraphic2);
	DDX_Control(pDX, IDC_CheckMarkGraphic3, m_CheckMarkGraphic3);
	DDX_Control(pDX, IDC_CheckMarkGraphic4, m_CheckMarkGraphic4);
	DDX_Control(pDX, IDC_CheckMarkGraphic5, m_CheckMarkGraphic5);
	DDX_Control(pDX, IDC_CheckMarkGraphic6, m_CheckMarkGraphic6);
	DDX_Control(pDX, IDC_CheckMarkGraphic7, m_CheckMarkGraphic7);
	DDX_Control(pDX, IDC_CheckMarkGraphic8, m_CheckMarkGraphic8);
}


void CUsersDialog::OnFunction1Button()
{
	//Main Menu was pressed
	if (vChangeMade)
		ReportErrorMessage("User saved the updated the user list going to Main Menu", cUserChanges, 0);
	vLocalSystemData->UpdateUserList(vLocalUserCollection);
	vLocalSystemData->StoreUserList();
	CDialog::EndDialog(10);
}


void CUsersDialog::OnFunction2Button()
{
	//Save and exit was pressed
	if (vChangeMade)
		ReportErrorMessage("User saved the updated the user list", cUserChanges, 0);
	vLocalSystemData->UpdateUserList(vLocalUserCollection);
	vLocalSystemData->StoreUserList();
	CDialog::EndDialog(true);
}


void CUsersDialog::OnFunction3Button()
{
	//Import was pressed
	this->LoadUserFromCSVFile();
	this->UpdateDisplay();
}


void CUsersDialog::OnFunction4Button()
{
	//Cancel was pressed
	//CDialog::EndDialog(0);
}


void CUsersDialog::OnFunction5Button()
{
	//Export was pressed
	this->WriteUsersToCSVFile();
}


void CUsersDialog::OnSubFunction1Button()
{
	CSetupUserDialog ISetupUserDialog;
	ISetupUserDialog.vMainWindowPointer = vMainWindowPointer;
	ISetupUserDialog.vLocalSystemData = this->vLocalSystemData;
	ISetupUserDialog.LoadData(vLocalUserCollection, cOperatorPassword);
	int nResponse = ISetupUserDialog.DoModal();
	if (nResponse == IDOK)
	{
		UpdateUsersData(ISetupUserDialog);
	}
	else 
	if (nResponse == 10)
	{
		CDialog::EndDialog(10);
	}
	UpdateDisplay();
}


void CUsersDialog::OnSubFunction2Button()
{
	CSetupUserDialog ISetupUserDialog;
	ISetupUserDialog.vMainWindowPointer = vMainWindowPointer;
	ISetupUserDialog.LoadData(vLocalUserCollection, cResetPassword);
	ISetupUserDialog.vLocalSystemData = this->vLocalSystemData;
	int nResponse = ISetupUserDialog.DoModal();
	if (nResponse == IDOK)
	{
		UpdateUsersData(ISetupUserDialog);
	}
	else 
	if (nResponse == 10)
	{
		CDialog::EndDialog(10);
	}
	UpdateDisplay();
}


void CUsersDialog::OnSubFunction3Button()
{
	CSetupUserDialog ISetupUserDialog;
	ISetupUserDialog.vMainWindowPointer = vMainWindowPointer;
	ISetupUserDialog.vLocalSystemData = this->vLocalSystemData;
	ISetupUserDialog.LoadData(vLocalUserCollection, cSamplePassword);
	int nResponse = ISetupUserDialog.DoModal();
	if (nResponse == IDOK)
	{
		UpdateUsersData(ISetupUserDialog);
	}
	else 
	if (nResponse == 10)
	{
		CDialog::EndDialog(10);
	}
	UpdateDisplay();
}


void CUsersDialog::OnSubFunction4Button()
{
	CSetupUserDialog ISetupUserDialog;
	ISetupUserDialog.vMainWindowPointer = vMainWindowPointer;
	ISetupUserDialog.vLocalSystemData = this->vLocalSystemData;
	ISetupUserDialog.LoadData(vLocalUserCollection, cSimpleSetupPassword);
	int nResponse = ISetupUserDialog.DoModal();
	if (nResponse == IDOK)
	{
		UpdateUsersData(ISetupUserDialog);
	}
	else 
	if (nResponse == 10)
	{
		CDialog::EndDialog(10);
	}
	UpdateDisplay();
}


void CUsersDialog::OnSubFunction5Button()
{
	CSetupUserDialog ISetupUserDialog;
	ISetupUserDialog.vMainWindowPointer = vMainWindowPointer;
	ISetupUserDialog.vLocalSystemData = this->vLocalSystemData;
	ISetupUserDialog.LoadData(vLocalUserCollection, cSetupPassword);
	int nResponse = ISetupUserDialog.DoModal();
	if (nResponse == IDOK)
	{
		UpdateUsersData(ISetupUserDialog);
	}
	else 
	if (nResponse == 10)
	{
		CDialog::EndDialog(10);
	}
	UpdateDisplay();
}


void CUsersDialog::OnSubFunction6Button()
{
	CSetupUserDialog ISetupUserDialog;
	ISetupUserDialog.vMainWindowPointer = vMainWindowPointer;
	ISetupUserDialog.vLocalSystemData = this->vLocalSystemData;
	ISetupUserDialog.LoadData(vLocalUserCollection, cQAPassword);
	int nResponse = ISetupUserDialog.DoModal();
	if (nResponse == IDOK)
	{
		UpdateUsersData(ISetupUserDialog);
	}
	else 
	if (nResponse == 10)
	{
		CDialog::EndDialog(10);
	}
	UpdateDisplay();
}


void CUsersDialog::OnSubFunction7Button()
{
	CSetupUserDialog ISetupUserDialog;
	ISetupUserDialog.vMainWindowPointer = vMainWindowPointer;
	ISetupUserDialog.vLocalSystemData = this->vLocalSystemData;
	ISetupUserDialog.LoadData(vLocalUserCollection, cAdvancedSetupPassword);
	int nResponse = ISetupUserDialog.DoModal();
	if (nResponse == IDOK)
	{
		UpdateUsersData(ISetupUserDialog);
	}
	else 
	if (nResponse == 10)
	{
		CDialog::EndDialog(10);
	}
	UpdateDisplay();
}



void CUsersDialog::OnSubFunction8Button()
{
	CSetupUserDialog ISetupUserDialog;
	ISetupUserDialog.vMainWindowPointer = vMainWindowPointer;
	ISetupUserDialog.vLocalSystemData = this->vLocalSystemData;
	ISetupUserDialog.LoadData(vLocalUserCollection, cCertifiedPassword);
	int nResponse = ISetupUserDialog.DoModal();
	if (nResponse == IDOK)
	{
		UpdateUsersData(ISetupUserDialog);
	}
	else 
	if (nResponse == 10)
	{
		CDialog::EndDialog(10);
	}
	UpdateDisplay();
	
}

void CUsersDialog::UpdateDisplay()
{
	m_Background.ShowWindow(SW_SHOW);
	CWnd* pWnd;
	CDC* pDC;
	HWND TempDialogHwnd = this->m_hWnd;

	for (BYTE TempLoop = 0 ; TempLoop < 8 ; TempLoop++)
	{
		CString TempButtonString("");
		CString TempBoxString("");
		switch (TempLoop)
		{
			case 0:
			{
				TempButtonString = "\nOperator";
				char TempBuffer [16];
				_itoa_s(vNumberOfLevelList[cOperatorPassword],TempBuffer,16,10);
				TempBoxString = TempBuffer;
			}
			break;
			case 1:
			{
				TempButtonString = "\nCounters Ejectors";
				char TempBuffer [16];
				_itoa_s(vNumberOfLevelList[cResetPassword],TempBuffer,16,10);
				TempBoxString = TempBuffer;
			}
			break;
			case 2:
			{
				TempButtonString = "\nSample Level";
				char TempBuffer [16];
				_itoa_s(vNumberOfLevelList[cSamplePassword],TempBuffer,16,10);
				TempBoxString = TempBuffer;
			}
			break;
			case 3:
			{
				TempButtonString = "\nSimple Setup";
				char TempBuffer [16];
				_itoa_s(vNumberOfLevelList[cSimpleSetupPassword],TempBuffer,16,10);
				TempBoxString = TempBuffer;
			}
			break;
			case 4:
			{
				TempButtonString = "\nSetup";
				char TempBuffer [16];
				_itoa_s(vNumberOfLevelList[cSetupPassword],TempBuffer,16,10);
				TempBoxString = TempBuffer;
			}
			break;
			case 5:
			{
				TempButtonString = "\nQA";
				char TempBuffer [16];
				_itoa_s(vNumberOfLevelList[cQAPassword],TempBuffer,16,10);
				TempBoxString = TempBuffer;
			}
			break;
			case 6:
			{
				TempButtonString = "\nAdvanced";
				char TempBuffer [16];
				_itoa_s(vNumberOfLevelList[cAdvancedSetupPassword],TempBuffer,16,10);
				TempBoxString = TempBuffer;
			}
			break;
			case 7:
			{
				TempButtonString = "\nCertfied";
				char TempBuffer [16];
				_itoa_s(vNumberOfLevelList[cCertifiedPassword],TempBuffer,16,10);
				TempBoxString = TempBuffer;
			}
			break;
		}
		switch (TempLoop)
		{
			case 0:
			{
				m_SubFunction1Button.ShowWindow(SW_SHOW);

				m_SubFunction1Display.ShowWindow(SW_SHOW);
				m_SubFunction1Display.EnableWindow(true);
				m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				/*m_CheckMarkGraphic1.SetWindowPos(NULL,150,48,39,37,SWP_NOZORDER);
				if (vGlobalPasswordLevel >= 1)
					m_CheckMarkGraphic1.ShowWindow(SW_SHOW);
				else*/
					m_CheckMarkGraphic1.ShowWindow(SW_HIDE);
				SetDlgItemText(IDC_SubFunction1Button,TempButtonString);
				SetDlgItemText(IDC_Item1,TempBoxString);

				pDC = m_SubFunction1Display.GetDC();
				pWnd = &m_SubFunction1Display;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5) + 30;  //5 is medium large
				m_SubFunction1Display.SetWindowPos(NULL, ResizeXCoor(110), ResizeYCoor(67), TempStringLength, ResizeYCoor(28), SWP_NOZORDER);
			}
			break;
			case 1:
			{
				m_SubFunction2Button.ShowWindow(SW_SHOW);
				m_SubFunction2Display.ShowWindow(SW_SHOW);
				m_SubFunction2Display.EnableWindow(true);
				m_SubFunction2Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				/*m_CheckMarkGraphic2.SetWindowPos(NULL,150,138,39,37,SWP_NOZORDER);
				if (vGlobalPasswordLevel >= 2)
					m_CheckMarkGraphic2.ShowWindow(SW_SHOW);
				else*/
					m_CheckMarkGraphic2.ShowWindow(SW_HIDE);
				SetDlgItemText(IDC_SubFunction2Button,TempButtonString);
				SetDlgItemText(IDC_Item2,TempBoxString);

				pDC = m_SubFunction2Display.GetDC();
				pWnd = &m_SubFunction2Display;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Display, 5) + 30;  //5 is medium large
				m_SubFunction2Display.SetWindowPos(NULL, ResizeXCoor(110), ResizeYCoor(159), TempStringLength, ResizeYCoor(28), SWP_NOZORDER);
			}
			break;
			case 2:
			{

				m_SubFunction3Button.ShowWindow(SW_SHOW);
				m_SubFunction3Display.ShowWindow(SW_SHOW);
				m_SubFunction3Display.EnableWindow(true);
				m_SubFunction3Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				/*m_CheckMarkGraphic3.SetWindowPos(NULL,150,229,39,37,SWP_NOZORDER);
				if (vGlobalPasswordLevel >= 3)
					m_CheckMarkGraphic3.ShowWindow(SW_SHOW);
				else*/
					m_CheckMarkGraphic3.ShowWindow(SW_HIDE);
				SetDlgItemText(IDC_SubFunction3Button,TempButtonString);
				SetDlgItemText(IDC_Item3,TempBoxString);

				pDC = m_SubFunction3Display.GetDC();
				pWnd = &m_SubFunction3Display;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Display, 5) + 30;  //5 is medium large
				m_SubFunction3Display.SetWindowPos(NULL, ResizeXCoor(110), ResizeYCoor(250), TempStringLength, ResizeYCoor(28), SWP_NOZORDER);
			}
			break;
			case 3:
			{
				m_SubFunction4Button.ShowWindow(SW_SHOW);
				m_SubFunction4Display.ShowWindow(SW_SHOW);
				m_SubFunction4Display.EnableWindow(true);
				m_SubFunction4Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				/*m_CheckMarkGraphic4.SetWindowPos(NULL,150,319,39,37,SWP_NOZORDER);
				if (vGlobalPasswordLevel >= 4)
					m_CheckMarkGraphic4.ShowWindow(SW_SHOW);
				else*/
					m_CheckMarkGraphic4.ShowWindow(SW_HIDE);
				SetDlgItemText(IDC_SubFunction4Button,TempButtonString);
				SetDlgItemText(IDC_Item4,TempBoxString);

				pDC = m_SubFunction4Display.GetDC();
				pWnd = &m_SubFunction4Display;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Display, 5) + 30;  //5 is medium large
				m_SubFunction4Display.SetWindowPos(NULL, ResizeXCoor(110), ResizeYCoor(340), TempStringLength, ResizeYCoor(28), SWP_NOZORDER);
			}
			break;
			case 4:
			{
				m_SubFunction5Button.ShowWindow(SW_SHOW);
				m_SubFunction5Display.ShowWindow(SW_SHOW);
				m_SubFunction5Display.EnableWindow(true);
				m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				/*m_CheckMarkGraphic5.SetWindowPos(NULL,435,67,39,37,SWP_NOZORDER);
				if (vGlobalPasswordLevel >= 5)
					m_CheckMarkGraphic5.ShowWindow(SW_SHOW);
				else*/
					m_CheckMarkGraphic5.ShowWindow(SW_HIDE);
				SetDlgItemText(IDC_SubFunction5Button,TempButtonString);
				SetDlgItemText(IDC_Item5,TempBoxString);

				pDC = m_SubFunction5Display.GetDC();
				pWnd = &m_SubFunction5Display;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Display, 5) + 30;  //5 is medium large
				m_SubFunction5Display.SetWindowPos(NULL, ResizeXCoor(525) - TempStringLength, ResizeYCoor(67), TempStringLength, ResizeYCoor(28), SWP_NOZORDER);
			}
			break;
			case 5:
			{
				m_SubFunction6Button.ShowWindow(SW_SHOW);
				m_SubFunction6Display.ShowWindow(SW_SHOW);
				m_SubFunction6Display.EnableWindow(true);
				m_SubFunction6Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				/*if (this->vLocalSystemData->vOperatorPasswordOK)
					m_CheckMarkGraphic6.ShowWindow(SW_SHOW);
				else*/
					m_CheckMarkGraphic6.ShowWindow(SW_HIDE);
				SetDlgItemText(IDC_SubFunction6Button,TempButtonString);
				SetDlgItemText(IDC_Item6,TempBoxString);

				pDC = m_SubFunction6Display.GetDC();
				pWnd = &m_SubFunction6Display;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Display, 5) + 30;  //5 is medium large
				m_SubFunction6Display.SetWindowPos(NULL, ResizeXCoor(525) - TempStringLength, ResizeYCoor(159), TempStringLength, ResizeYCoor(28), SWP_NOZORDER);
			}
			break;
			case 6:
			{
				m_SubFunction7Button.ShowWindow(SW_SHOW);
				m_SubFunction7Display.ShowWindow(SW_SHOW);
				m_SubFunction7Display.EnableWindow(true);
				m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				/*if (this->vLocalSystemData->vOperatorPasswordOK)
					m_CheckMarkGraphic7.ShowWindow(SW_SHOW);
				else*/
					m_CheckMarkGraphic7.ShowWindow(SW_HIDE);
				SetDlgItemText(IDC_SubFunction7Button,TempButtonString);
				SetDlgItemText(IDC_Item7,TempBoxString);

				pDC = m_SubFunction7Display.GetDC();
				pWnd = &m_SubFunction7Display;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Display, 5) + 30;  //5 is medium large
				m_SubFunction7Display.SetWindowPos(NULL, ResizeXCoor(525) - TempStringLength, ResizeYCoor(250), TempStringLength, ResizeYCoor(28), SWP_NOZORDER);
			}
			break;
			case 7:
			{
				m_SubFunction8Button.ShowWindow(SW_SHOW);
				m_SubFunction8Display.ShowWindow(SW_SHOW);
				m_SubFunction8Display.EnableWindow(true);
				m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
				/*if (this->vLocalSystemData->vOperatorPasswordOK)
					m_CheckMarkGraphic2.ShowWindow(SW_SHOW);
				else*/
					m_CheckMarkGraphic8.ShowWindow(SW_HIDE);
				SetDlgItemText(IDC_SubFunction8Button,TempButtonString);
				SetDlgItemText(IDC_Item8,TempBoxString);

				pDC = m_SubFunction8Display.GetDC();
				pWnd = &m_SubFunction8Display;
 				int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Display, 5) + 30;  //5 is medium large
				m_SubFunction8Display.SetWindowPos(NULL, ResizeXCoor(525) - TempStringLength, ResizeYCoor(340), TempStringLength, ResizeYCoor(28), SWP_NOZORDER);
			}
			break;
		}
	}

/* do not hide password level options from user setup menu, even if passwords are not yet set
	if (vGlobalMainWindowPointer->vConfigurationData->vProductSetupPassword.GetLength() < 1)
	{
		m_SubFunction4Button.ShowWindow(SW_HIDE);
		m_ArrowLeft4.ShowWindow(SW_HIDE);
		m_SubFunction4Display.ShowWindow(SW_HIDE);
	}
	if (vGlobalMainWindowPointer->vConfigurationData->vProductSimpleSetupPassword.GetLength() < 1)
	{
		m_SubFunction4Button.ShowWindow(SW_HIDE);
		m_ArrowLeft4.ShowWindow(SW_HIDE);
		m_SubFunction4Display.ShowWindow(SW_HIDE);
	}
	if (vGlobalMainWindowPointer->vConfigurationData->vSamplePassword.GetLength() < 1)
	{
		m_SubFunction3Button.ShowWindow(SW_HIDE);
		m_ArrowLeft3.ShowWindow(SW_HIDE);
		m_SubFunction3Display.ShowWindow(SW_HIDE);
	}	
	if (vGlobalMainWindowPointer->vConfigurationData->vResetCountsPassword.GetLength() < 1)
	{
		m_SubFunction2Button.ShowWindow(SW_HIDE);
		m_ArrowLeft2.ShowWindow(SW_HIDE);
		m_SubFunction2Display.ShowWindow(SW_HIDE);
	}
	if (vGlobalMainWindowPointer->vConfigurationData->vOperatorPassword.GetLength() < 1)
	{
		m_SubFunction1Button.ShowWindow(SW_HIDE);
		m_ArrowLeft1.ShowWindow(SW_HIDE);
		m_SubFunction1Display.ShowWindow(SW_HIDE);
	}
*/
	CString TempText("");

	TempText.LoadString(IDS_MainMenu);
	SetDlgItemText(IDC_Function1Button,TempText);

	if (vChangeMade)
	{
		TempText.LoadString(IDS_SaveAndExit);
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
	}
	else
		TempText.LoadString(IDS_Exit);
	
	SetDlgItemText(IDC_Function2Button,TempText);

	//if (vChangeMade)
	//{
	//	TempText.LoadString(IDS_Cancel);
	//	SetDlgItemText(IDC_Function4Button,TempText);
	//	m_Function4Button.ShowWindow(SW_SHOW);
	//	m_ArrowDown4.ShowWindow(SW_SHOW);
	//}
	//else
	//{
	//	m_Function4Button.ShowWindow(SW_HIDE);
	//	m_ArrowDown4.ShowWindow(SW_HIDE);
	//}

	//SHOW BUTTON THREE & FIVE
	TempText = "Import from CSV file";
	SetDlgItemText(IDC_Function3Button,TempText);
	m_Function3Button.ShowWindow(SW_SHOW);
	TempText = "Export to CSV file";
	SetDlgItemText(IDC_Function5Button,TempText);
	m_Function5Button.ShowWindow(SW_SHOW);

	TempText = "Users";
	SetDlgItemText(IDC_DialogTitleStaticText1, TempText);
	Invalidate(true); 
}

void CUsersDialog::Serialize(CArchive& TempArchive)
{
	return; // Obsolete function
	//CObject::Serialize(TempArchive);

	//if (TempArchive.IsStoring())
	//{	
	//	TempArchive << vCurrentNumberOfUsers;
	//	for (int TempLoop = 0; TempLoop < vCurrentNumberOfUsers ; TempLoop++)
	//	{
	//		TempArchive << (vUserList[TempLoop]).vUserName
	//					<< (vUserList[TempLoop]).vPassword
	//					<< (vUserList[TempLoop]).vID
	//					<< (vUserList[TempLoop]).vLevel
	//					<< (vUserList[TempLoop]).vBCID
	//					<< (vUserList[TempLoop]).vRFID;
	//	}
	//}
	//else
	//{	
	//	TempArchive >> vCurrentNumberOfUsers;
	//	this->ResizeUserArray();
	//	for (int TempLoop = 0; TempLoop < vCurrentNumberOfUsers ; TempLoop++)
	//	{
	//		TempArchive >> (vUserList[TempLoop]).vUserName
	//					>> (vUserList[TempLoop]).vPassword
	//					>> (vUserList[TempLoop]).vID
	//					>> (vUserList[TempLoop]).vLevel
	//					>> (vUserList[TempLoop]).vBCID
	//					>> (vUserList[TempLoop]).vRFID;
	//		switch (vUserList[TempLoop].vLevel)
	//		{
	//			case cResetPassword:
	//				vNumberOfLevelList[cResetPassword]++;
	//			break;
	//			case cOperatorPassword:
	//				vNumberOfLevelList[cOperatorPassword]++;
	//			break;
	//			case cSimpleSetupPassword:
	//				vNumberOfLevelList[cSimpleSetupPassword]++;
	//			break;
	//			case cSetupPassword:
	//				vNumberOfLevelList[cSetupPassword]++;
	//			break;
	//			//No Case 5 for Temporary Advanced Password Users
	//			case cAdvancedSetupPassword:
	//				vNumberOfLevelList[cAdvancedSetupPassword]++;
	//			break;
	//			case cCertifiedPassword:
	//				vNumberOfLevelList[cCertifiedPassword]++;
	//			break;
	//			default:
	//				;//do nothing
	//			break;
	//		}
	//	}
	//}
	//TempArchive.Close();
}

void CUsersDialog::ResizeUserArray()
{
	//BYTE TempArraySize = vCurrentMaxNumberOfUsers;
	//while (TempArraySize <= vCurrentNumberOfUsers)
	//	TempArraySize = 2 * TempArraySize;
	//UserPasswords *TempArray = new UserPasswords [TempArraySize];
	//for (BYTE TempLoop = 0; TempLoop < vCurrentMaxNumberOfUsers; TempLoop++)
	//{
	//	TempArray[TempLoop] = vUserList[TempLoop];
	//}
	//delete [] vUserList;
	//vUserList = TempArray;
	//vCurrentMaxNumberOfUsers = TempArraySize;
}

void CUsersDialog::LoadUserData()
{
	if (!vLocalSystemData)
		return;
	vLocalUserCollection.RemoveAll();
	vCurrentNumberOfUsers = vLocalSystemData->vUserCollection.GetCount();
	SetDlgItemText(IDC_Background, dtoa(vCurrentNumberOfUsers, 0) + " Users");
	for (POSITION TempPosition = vLocalSystemData->vUserCollection.GetHeadPosition() ; TempPosition ; vLocalSystemData->vUserCollection.GetNext(TempPosition))
	if (TempPosition) 
	{
		vLocalUserCollection.AddTail(vLocalSystemData->vUserCollection.GetAt(TempPosition));
		switch ((vLocalUserCollection.GetTail())->vLevel)
		{
			case cOperatorPassword:
				vNumberOfLevelList[cOperatorPassword]++;
				/*
				if (vLocalConfigurationData->vOperatorPassword.GetLength() == 0)
				{
					CNoticeDialog TempNoticeDialog;
					CString TempText("\n\n\nUser " + vLocalUserCollection.GetTail()->vUserName + " is Operator Level,\nbut is hidden because Universal Operator Password is blank.");
					TempNoticeDialog.vNoticeText = TempText;
					TempNoticeDialog.vType = cNoticeMessage;
					TempNoticeDialog.DoModal();
				}*/
			break;
			case cResetPassword:
				vNumberOfLevelList[cResetPassword]++;
				/*
				if (vLocalConfigurationData->vResetCountsPassword.GetLength() == 0)
				{
					CNoticeDialog TempNoticeDialog;
					CString TempText("\n\n\nUser " + vLocalUserCollection.GetTail()->vUserName + " is Counters-Ejectors Level,\nbut is hidden because Universal Counters-Ejectors Password is blank.");
					TempNoticeDialog.vNoticeText = TempText;
					TempNoticeDialog.vType = cNoticeMessage;
					TempNoticeDialog.DoModal();
				}*/
			case cSamplePassword:
				vNumberOfLevelList[cSamplePassword]++;
				/*
				if (vLocalConfigurationData->vResetCountsPassword.GetLength() == 0)
				{
					CNoticeDialog TempNoticeDialog;
					CString TempText("\n\n\nUser " + vLocalUserCollection.GetTail()->vUserName + " is Counters-Ejectors Level,\nbut is hidden because Universal Counters-Ejectors Password is blank.");
					TempNoticeDialog.vNoticeText = TempText;
					TempNoticeDialog.vType = cNoticeMessage;
					TempNoticeDialog.DoModal();
				}*/
			break;
			case cSimpleSetupPassword:
				vNumberOfLevelList[cSimpleSetupPassword]++;
				/*
				if (vLocalConfigurationData->vProductSimpleSetupPassword.GetLength() == 0)
				{
					CNoticeDialog TempNoticeDialog;
					CString TempText("\n\n\nUser " + vLocalUserCollection.GetTail()->vUserName + " is Simple Setup Level,\nbut is hidden because Universal Simple Setup Password is blank.");
					TempNoticeDialog.vNoticeText = TempText;
					TempNoticeDialog.vType = cNoticeMessage;
					TempNoticeDialog.DoModal();
				}*/
			break;
			case cSetupPassword:
				vNumberOfLevelList[cSetupPassword]++;
				
				/*if (vLocalConfigurationData->vSetupPassword.GetLength() == 0)
				{
					CNoticeDialog TempNoticeDialog;
					CString TempText("\n\n\nUser " + vLocalUserCollection.GetTail()->vUserName + " is Setup Level,\nbut is hidden because Universal Setup Password is blank.");
					TempNoticeDialog.vNoticeText = TempText;
					TempNoticeDialog.vType = cNoticeMessage;
					TempNoticeDialog.DoModal();
				}*/
			break;
			case cQAPassword:
				vNumberOfLevelList[cQAPassword]++;
				
/*				if (vLocalConfigurationData->vQAPassword.GetLength() == 0)
				{
					CNoticeDialog TempNoticeDialog;
					CString TempText("\n\n\nUser " + vLocalUserCollection.GetTail()->vUserName + " is QA Level,\nbut is hidden because Universal QA Password is blank.");
					TempNoticeDialog.vNoticeText = TempText;
					TempNoticeDialog.vType = cNoticeMessage;
					TempNoticeDialog.DoModal();
				}*/
			break;
			//No Case 5 for Temporary Advanced Password Users
			case cAdvancedSetupPassword:
				vNumberOfLevelList[cAdvancedSetupPassword]++;
			break;
			case cCertifiedPassword:
				vNumberOfLevelList[cCertifiedPassword]++;
			break;
			default:
				;//do nothing
			break;
		}
	}
	vCurrentNumberOfUsers = vLocalUserCollection.GetCount();
}

void CUsersDialog::WriteUsersToCSVFile()
{
	CString TempUserFileName = "ScanTracUsers.CSV";
	TempUserFileName = cLogDirectory + TempUserFileName;
	LPCTSTR TempFileName = TempUserFileName;
	CFileException TempFileException;
	if (ThereIsEnoughMemory(sizeof(CFile), "User Data File"))
	{
		CFile *vDataFile;
		vDataFile = new CFile;
		if (vDataFile)
		{
			int TempFileOpened = vDataFile->Open(TempFileName, (CFile::modeCreate | CFile::modeReadWrite), &TempFileException);
			if (TempFileOpened == 0)
			{
				//could not open serial log file
				ReportErrorMessage("Error-Open User File Failed: ", cEMailInspx,32000);// + TempFileException.m_cause); 
				#ifdef _DEBUG
					afxDump << "Could Not Open User Data File"  << "\n";
				#endif
			}
			else
			{
				//write the user data to the file
				TRY
				{
					CString TempString = " ";
					WORD TempUserCount = vLocalUserCollection.GetCount();
					WORD TempCount = 0;
					TempString = "Username,Level,Password,ID,BCID,RFID\n";
					for (POSITION TempPosition = vLocalUserCollection.GetHeadPosition(); TempPosition;)
					if (TempPosition) 
					{
						UserPasswords* TempUser = vLocalUserCollection.GetNext(TempPosition);
						TempString = TempString + TempUser->vUserName + "," + cLevelName[TempUser->vLevel] + "," + TempUser->vPassword + ","
									 + TempUser->vID + "," + dtoa(TempUser->vBCID) + "," + dtoa(TempUser->vRFID) + "," + "\n";

						TempCount++;
					}
					CW2A TempStringToWrite(TempString);
					vDataFile->Write(TempStringToWrite, TempString.GetLength());

					CNoticeDialog TempNoticeDialog;
					CString TempText("\n\n\n" + dtoa(TempCount, 0) + " Users successfully exported to: " + TempUserFileName);
					TempNoticeDialog.vNoticeText = TempText;
					TempNoticeDialog.vType = cNoticeMessage;
					TempNoticeDialog.DoModal();
				}
				CATCH_ALL(TempFileException)
				{
				 //#ifdef _DEBUG
				//		afxDump << "Could Not Write Calibration File"  << "\n";
				 //#endif
				}
				END_CATCH_ALL
				TRY
				{
					vDataFile->Close();
				}
				CATCH_ALL(TempFileException)
				{
				 #ifdef _DEBUG
						afxDump << "Could Not Close User File"  << "\n";
				 #endif
				}
				END_CATCH_ALL
			}
			delete vDataFile;
			vDataFile = NULL;
		}
	}
}

void CUsersDialog::LoadUserFromCSVFile()
{
	CString TempUserFileInput = "";
	CString TempSeperationCharacters = ",\n";
	CFileException TempFileException;
	CString TempFilePath = "ScanTracUsers.CSV";
	TempFilePath = cLogDirectory + TempFilePath;
	LPCTSTR TempFileName = TempFilePath;
	if (ThereIsEnoughMemory(sizeof(CFile), "Read in ScanTracUsers.CSV"))
	{
		vCurrentNumberOfUsers = vLocalUserCollection.GetCount();
		int TempOrignalNumberOfUsers = vCurrentNumberOfUsers;
		WORD TempDuplicateUsers = 0;
		CFile TempUserFile;
		if ((TempUserFile) && (TempUserFile.Open(TempFileName, CFile::modeRead | CFile::shareDenyNone, &TempFileException)))
		{
			char* TempBuffer = new char[(UINT)TempUserFile.GetLength()];
			if (TempUserFile.GetLength() == TempUserFile.Read(TempBuffer,(UINT)TempUserFile.GetLength()))
			{
				TempUserFileInput = TempBuffer;
				delete[] TempBuffer;
				for (BYTE TempLoop = 0; TempLoop < 6; TempLoop++)
				{
					int TempDeletionLocation = TempUserFileInput.FindOneOf(TempSeperationCharacters);//remove the column names
					TempUserFileInput.Delete(0,TempDeletionLocation+1);
				}
				while (TempUserFileInput.GetLength() > 0 && TempUserFileInput.Find(',',0) != -1)
				{
					DWORD TempDeletionLocation = TempUserFileInput.Find(',',0);
					CString TempUserString = TempUserFileInput.Mid(0, TempDeletionLocation);
					UserPasswords* TempUser = new UserPasswords;
					TempUser->vUserName = TempUserString;
					TempUserFileInput.Delete(0,TempDeletionLocation+1);

					TempDeletionLocation = TempUserFileInput.Find(',',0);
					TempUserString = TempUserFileInput.Mid(0, TempDeletionLocation);
					for (BYTE TempLoop = 0; TempLoop < cSuperInspxPassword; TempLoop++)
						if (cLevelName[TempLoop] == TempUserString)
							TempUser->vLevel = TempLoop;
					//if ((TempUser->vLevel == cNoPassword) || (TempUser->vLevel == cOperatorPassword)) //for duo
					if (TempUser->vLevel == cNoPassword) //for solo
					{
						TempUser->vLevel = cResetPassword;
						ReportErrorMessage("Import User: " + TempUser->vUserName + ", Level changed to: " + cLevelName[TempUser->vLevel],cEMailMaintenance,0);
					}
					TempUserFileInput.Delete(0,TempDeletionLocation+1);
					
					TempDeletionLocation = TempUserFileInput.Find(',',0);
					TempUserString = TempUserFileInput.Mid(0, TempDeletionLocation);
					TempUser->vPassword = TempUserString;
					TempUserFileInput.Delete(0,TempDeletionLocation+1);

					TempDeletionLocation = TempUserFileInput.Find(',',0);
					TempUserString = TempUserFileInput.Mid(0, TempDeletionLocation);
					TempUser->vID = TempUserString;
					TempUserFileInput.Delete(0,TempDeletionLocation+1);

					TempDeletionLocation = TempUserFileInput.Find(',',0);
					TempUserString = TempUserFileInput.Mid(0, TempDeletionLocation);
					TempUser->vBCID = _wtoi(TempUserString);
					TempUserFileInput.Delete(0,TempDeletionLocation+1);

					TempDeletionLocation = TempUserFileInput.Find('\n',0);
					TempUserString = TempUserFileInput.Mid(0, TempDeletionLocation);
					TempUser->vRFID = _wtoi(TempUserString);
					TempUserFileInput.Delete(0,TempDeletionLocation+1);

					if (!FindUsername(TempUser->vUserName) && !PasswordExist(this->vLocalUserCollection,TempUser->vPassword))
					{
						InsertAlphabetically(this->vLocalUserCollection,TempUser);
						switch (TempUser->vLevel)
						{
							case cOperatorPassword:
								vNumberOfLevelList[cOperatorPassword]++;
							break;
							case cResetPassword:
								vNumberOfLevelList[cResetPassword]++;
							break;
							case cSimpleSetupPassword:
								vNumberOfLevelList[cSimpleSetupPassword]++;
							break;
							case cSetupPassword:
								vNumberOfLevelList[cSetupPassword]++;
							break;
							//No Case 5 for Temporary Advanced Password Users
							case cAdvancedSetupPassword:
								vNumberOfLevelList[cAdvancedSetupPassword]++;
							break;
							case cCertifiedPassword:
								vNumberOfLevelList[cCertifiedPassword]++;
							break;
							default:
								;//do nothing
							break;
						}
					}
					else
						TempDuplicateUsers++;
				}
			}
			CNoticeDialog TempNoticeDialog;

			vCurrentNumberOfUsers = vLocalUserCollection.GetCount();
			SetDlgItemText(IDC_Background, dtoa(vCurrentNumberOfUsers, 0) + " Users");
			int TempNumberOfUsersImported = vCurrentNumberOfUsers - TempOrignalNumberOfUsers;

			CString TempText("\n\n\n" + dtoa(TempNumberOfUsersImported, 0) + " Users imported successfully.");
			CString TempDuplicateUsersString = "";
			if (TempDuplicateUsers)
			{
				TempDuplicateUsersString = "\n" + dtoa(TempDuplicateUsers, 0) + " Duplicate Users were ignored.";
				TempText = TempText + TempDuplicateUsersString;
			}
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
			TempUserFile.Close();
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText("\n\n\nCould not open file: " +  TempFilePath);
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		CString TempText("\n\n\nNot enough memory to open ScanTracUsers.CSV file");
		TempNoticeDialog.vNoticeText = TempText;
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
}

bool CUsersDialog::FindUsername(CString TempUsername)
{
	for (POSITION TempPosition = vLocalUserCollection.GetHeadPosition(); TempPosition;)
	if (vLocalUserCollection.GetNext(TempPosition)) 
	if (TempPosition) 
	if ((vLocalUserCollection.GetAt(TempPosition))->vUserName.CompareNoCase(TempUsername) == 0) 
		return true;
	return false;
}


BOOL CUsersDialog::PreTranslateMessage(MSG* pMsg)
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
			case 36:  //Home key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
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


void CUsersDialog::OnDialogtitlestatictext1()
{
	//bring up soft key menu for help, print screen, Image, Clear, and hidden shift key
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
