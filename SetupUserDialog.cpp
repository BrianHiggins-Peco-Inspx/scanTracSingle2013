// CSetupUserDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "SetupUserDialog.h"
#include "afxdialogex.h"
#include "EditUser.h"
#ifndef DELETE_USER
#define DELETE_USER 2
#endif

extern double vGlobalPercentToResizeY;
extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
extern CString cLevelName[];
// CSetupUserDialog dialog

IMPLEMENT_DYNAMIC(CSetupUserDialog, CDialog)

CSetupUserDialog::CSetupUserDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupUserDialog::IDD, pParent)
{
	vCurrentFilteredLevel = 0;
	vCurrentFirstItem = 0;
	vCurrentNumberOfFilteredUsers = 0;
	vChangeMade = false;
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

	vLocalDisplayCollection.Add(&m_SubFunction1Display);
	vLocalDisplayCollection.Add(&m_SubFunction2Display);
	vLocalDisplayCollection.Add(&m_SubFunction3Display);
	vLocalDisplayCollection.Add(&m_SubFunction4Display);
	vLocalDisplayCollection.Add(&m_SubFunction5Display);
	vLocalDisplayCollection.Add(&m_SubFunction6Display);
	vLocalDisplayCollection.Add(&m_SubFunction7Display);
	vLocalDisplayCollection.Add(&m_SubFunction8Display);
}

CSetupUserDialog::~CSetupUserDialog()
{
	//POSITION TempPosition = vLocalUserCollection.GetHeadPosition();
	//while (TempPosition)
	//	delete vLocalUserCollection.GetNext(TempPosition);
	//vLocalUserCollection.RemoveAll();
}

void CSetupUserDialog::DeleteUser(UserPasswords* TempUser)
{
	vLocalUserCollection.RemoveAt(vLocalUserCollection.Find(TempUser));
	delete TempUser;
	MakeFilteredList(vCurrentFilteredLevel);
}

void CSetupUserDialog::MakeFilteredList(BYTE UserLevel)
{
	vFilteredUserCollection.RemoveAll();
	for (POSITION TempPosition = vLocalUserCollection.GetHeadPosition(); TempPosition; vLocalUserCollection.GetNext(TempPosition))
	if (TempPosition) 
	{
		if ((vLocalUserCollection.GetAt(TempPosition))->vLevel == UserLevel)
			vFilteredUserCollection.AddTail(vLocalUserCollection.GetAt(TempPosition));
	}
	vCurrentNumberOfFilteredUsers = vFilteredUserCollection.GetCount();
	if (vCurrentNumberOfFilteredUsers == 1)
		SetDlgItemText(IDC_Background, dtoa(vCurrentNumberOfFilteredUsers, 0) + " " + cLevelName[vCurrentFilteredLevel] + " User");
	else
		SetDlgItemText(IDC_Background, dtoa(vCurrentNumberOfFilteredUsers, 0) + " " + cLevelName[vCurrentFilteredLevel] + " Users");
}

void CSetupUserDialog::LoadData(tUserCollection &TempUserCollection, BYTE FilterLevel)
{
	WORD TempCount = TempUserCollection.GetCount();
	for (POSITION TempPosition = TempUserCollection.GetHeadPosition(); TempPosition; TempUserCollection.GetNext(TempPosition))
	if (TempPosition) 
	{
		vLocalUserCollection.AddTail(TempUserCollection.GetAt(TempPosition));
	}
	vCurrentFilteredLevel = FilterLevel;
}

BEGIN_MESSAGE_MAP(CSetupUserDialog, CDialog)
	ON_WM_SHOWWINDOW()
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
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CSetupUserDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()


// CSetupUserDialog message handlers


/////////////////////////////////////////////////////////////////////////////
// CContaminant22EditThresholdsDialog dialog


void CSetupUserDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContaminant22EditThresholdsDialog)
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	//  DDX_Control(pDX, IDC_ArrowDown4, m_ArrowDown4);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
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
}



/////////////////////////////////////////////////////////////////////////////
// CContaminant22EditThresholdsDialog message handlers

void CSetupUserDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	//MakeFilteredList(vCurrentFilteredLevel);
	// TODO: Add your message handler code here
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft, cMainBackgroundTop,cMainBackgroundLength, cMainBackgroundHeight,SWP_NOZORDER);

	MakeFilteredList(vCurrentFilteredLevel);
	this->SetWindowText(cLevelName[vCurrentFilteredLevel] + " Users");
	::SetupMenu(vLocalCWndCollection);
	UpdateDisplay();
}

void CSetupUserDialog::OnFunction1Button() 
{
	//New was pressed
	CEditUser IEditUser;
	IEditUser.vCurrentUser.vLevel = vCurrentFilteredLevel;
	IEditUser.vLocalSystemData = this->vLocalSystemData;
	IEditUser.vLocalUserCollection = &(this->vLocalUserCollection);
	int nResponse = IEditUser.DoModal();
	if (nResponse == IDOK)
	{
		UserPasswords* TempUser = new UserPasswords; 
		*TempUser = IEditUser.vCurrentUser;
		InsertAlphabetically(vLocalUserCollection, TempUser);
		ReportErrorMessage("Operator added new user: " + TempUser->vUserName, cUserChanges, 0);
		MakeFilteredList(vCurrentFilteredLevel);
		vChangeMade = true;
	}
	else 
	if (nResponse == 10)
	{
		CDialog::EndDialog(10);
	}
	this->Invalidate();
	UpdateDisplay();
}

void CSetupUserDialog::OnFunction2Button() 
{
	//Exit was pressed
	CDialog::EndDialog(true);
}

void CSetupUserDialog::OnFunction3Button() 
{
	//previous page of users button pressed
	if (vCurrentFirstItem > 8)
		vCurrentFirstItem = vCurrentFirstItem - 8;
	else
		vCurrentFirstItem = 0;

	this->Invalidate();
	UpdateDisplay();
}

void CSetupUserDialog::OnFunction4Button() 
{
	//Cancel was pressed
	//if (vChangeMade)
	//if ((!vChangeMade) || (AbortSetupChanges()))
	//	CDialog::OnCancel();
}

void CSetupUserDialog::OnFunction5Button() 
{
	//next page of users button pressed
	if (vCurrentNumberOfFilteredUsers > vCurrentFirstItem + 8) //if there is a next page of users
		vCurrentFirstItem = vCurrentFirstItem + 8;

	this->Invalidate();
	UpdateDisplay();
}

void CSetupUserDialog::OnSubFunction1Button() 
{
	if (vCurrentFirstItem + 0 < vCurrentNumberOfFilteredUsers)
	{
		CEditUser IEditUser;
		IEditUser.vMainWindowPointer = vMainWindowPointer;
		IEditUser.vNewUser = false;
		IEditUser.vCurrentUser = *(vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem)));
		IEditUser.vLocalSystemData = this->vLocalSystemData;
		IEditUser.vLocalUserCollection = &(this->vLocalUserCollection);
		int nResponse = IEditUser.DoModal();
		if (nResponse == IDOK)
		{
			*(vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem))) = IEditUser.vCurrentUser;
			MakeFilteredList(vCurrentFilteredLevel); 
			if (IEditUser.vChangeMade)
				vChangeMade = true;
		}
		else 
		if (nResponse == DELETE_USER)
		{
			UserPasswords* TempUser = vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem));
			ReportErrorMessage("Operator deleted user: " + TempUser->vUserName, cUserChanges, 0);
			DeleteUser(TempUser);
			vChangeMade = true;
		}
		else 
		if (nResponse == 10)
		{
			CDialog::EndDialog(10);
		}
		this->Invalidate();
		UpdateDisplay();
	}
}

void CSetupUserDialog::OnSubFunction2Button() 
{
	if (vCurrentFirstItem + 1 < vCurrentNumberOfFilteredUsers)
	{
		CEditUser IEditUser;
		IEditUser.vMainWindowPointer = vMainWindowPointer;
		IEditUser.vNewUser = false;
		IEditUser.vCurrentUser = *(vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 1)));
		IEditUser.vLocalSystemData = this->vLocalSystemData;
		IEditUser.vLocalUserCollection = &(this->vLocalUserCollection);
		int nResponse = IEditUser.DoModal();
		if (nResponse == IDOK)
		{
			*(vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 1))) = IEditUser.vCurrentUser;
			MakeFilteredList(vCurrentFilteredLevel); 
			if (IEditUser.vChangeMade)
				vChangeMade = true;
		}
		else 
		if (nResponse == DELETE_USER)
		{
			UserPasswords* TempUser = (vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 1)));
			ReportErrorMessage("Operator deleted user: " + TempUser->vUserName, cUserChanges, 0);
			DeleteUser(TempUser);
			vChangeMade = true;
		}
		else 
		if (nResponse == 10)
		{
			CDialog::EndDialog(10);
		}
		this->Invalidate();
		UpdateDisplay();
	}
}

void CSetupUserDialog::OnSubFunction3Button() 
{
	if (vCurrentFirstItem + 2 < vCurrentNumberOfFilteredUsers)
	{
		CEditUser IEditUser;
		IEditUser.vMainWindowPointer = vMainWindowPointer;
		IEditUser.vNewUser = false;
		IEditUser.vCurrentUser = *(vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 2)));
		IEditUser.vLocalSystemData = this->vLocalSystemData;
		IEditUser.vLocalUserCollection = &(this->vLocalUserCollection);
		int nResponse = IEditUser.DoModal();
		if (nResponse == IDOK)
		{
			*(vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 2))) = IEditUser.vCurrentUser;
			MakeFilteredList(vCurrentFilteredLevel); 
			if (IEditUser.vChangeMade)
				vChangeMade = true;
		}
		else 
		if (nResponse == DELETE_USER)
		{
			UserPasswords* TempUser = (vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 2)));
			ReportErrorMessage("Operator deleted user: " + TempUser->vUserName, cUserChanges, 0);
			DeleteUser(TempUser);
			vChangeMade = true;
		}
		else 
		if (nResponse == 10)
		{
			CDialog::EndDialog(10);
		}
		this->Invalidate();
		UpdateDisplay();
	}
}

void CSetupUserDialog::OnSubFunction4Button() 
{
	if (vCurrentFirstItem + 3 < vCurrentNumberOfFilteredUsers)
	{
		CEditUser IEditUser;
		IEditUser.vMainWindowPointer = vMainWindowPointer;
		IEditUser.vNewUser = false;
		IEditUser.vCurrentUser = *(vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 3)));
		IEditUser.vLocalSystemData = this->vLocalSystemData;
		IEditUser.vLocalUserCollection = &(this->vLocalUserCollection);
		int nResponse = IEditUser.DoModal();
		if (nResponse == IDOK)
		{
			*(vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 3))) = IEditUser.vCurrentUser;
			MakeFilteredList(vCurrentFilteredLevel); 
			if (IEditUser.vChangeMade)
				vChangeMade = true;
		}
		else 
		if (nResponse == DELETE_USER)
		{
			UserPasswords* TempUser = (vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 3)));
			ReportErrorMessage("Operator deleted user: " + TempUser->vUserName, cUserChanges, 0);
			DeleteUser(TempUser);
			vChangeMade = true;
		}
		else 
		if (nResponse == 10)
		{
			CDialog::EndDialog(10);
		}
		this->Invalidate();
		UpdateDisplay();
	}
}

void CSetupUserDialog::OnSubFunction5Button() 
{
	if (vCurrentFirstItem + 4 < vCurrentNumberOfFilteredUsers)
	{
		CEditUser IEditUser;
		IEditUser.vMainWindowPointer = vMainWindowPointer;
		IEditUser.vNewUser = false;
		IEditUser.vCurrentUser = *(vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 4)));
		IEditUser.vLocalSystemData = this->vLocalSystemData;
		IEditUser.vLocalUserCollection = &(this->vLocalUserCollection);
		int nResponse = IEditUser.DoModal();
		if (nResponse == IDOK)
		{
			*(vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 4))) = IEditUser.vCurrentUser;
			MakeFilteredList(vCurrentFilteredLevel); 
			if (IEditUser.vChangeMade)
				vChangeMade = true;
		}
		else 
		if (nResponse == DELETE_USER)
		{
			UserPasswords* TempUser = (vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 4)));
			ReportErrorMessage("Operator deleted user: " + TempUser->vUserName, cUserChanges, 0);
			DeleteUser(TempUser);
			vChangeMade = true;
		}
		else 
		if (nResponse == 10)
		{
			CDialog::EndDialog(10);
		}
		this->Invalidate();
		UpdateDisplay();
	}
}

void CSetupUserDialog::OnSubFunction6Button() 
{
	if (vCurrentFirstItem + 5 < vCurrentNumberOfFilteredUsers)
	{
		CEditUser IEditUser;
		IEditUser.vMainWindowPointer = vMainWindowPointer;
		IEditUser.vNewUser = false;
		IEditUser.vCurrentUser = *(vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 5)));
		IEditUser.vLocalSystemData = this->vLocalSystemData;
		IEditUser.vLocalUserCollection = &(this->vLocalUserCollection);
		int nResponse = IEditUser.DoModal();
		if (nResponse == IDOK)
		{
			*(vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 5))) = IEditUser.vCurrentUser;
			MakeFilteredList(vCurrentFilteredLevel); 
			if (IEditUser.vChangeMade)
				vChangeMade = true;
		}
		else 
		if (nResponse == DELETE_USER)
		{
			UserPasswords* TempUser = (vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 5)));
			ReportErrorMessage("Operator deleted user: " + TempUser->vUserName, cUserChanges, 0);
			DeleteUser(TempUser);
			vChangeMade = true;
		}
		else 
		if (nResponse == 10)
		{
			CDialog::EndDialog(10);
		}
		this->Invalidate();
		UpdateDisplay();
	}
}

void CSetupUserDialog::OnSubFunction7Button() 
{
	if (vCurrentFirstItem + 6 < vCurrentNumberOfFilteredUsers)
	{
		CEditUser IEditUser;
		IEditUser.vMainWindowPointer = vMainWindowPointer;
		IEditUser.vNewUser = false;
		IEditUser.vCurrentUser = *(vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 6)));
		IEditUser.vLocalSystemData = this->vLocalSystemData;
		IEditUser.vLocalUserCollection = &(this->vLocalUserCollection);
		int nResponse = IEditUser.DoModal();
		if (nResponse == IDOK)
		{
			*(vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 6))) = IEditUser.vCurrentUser;
			MakeFilteredList(vCurrentFilteredLevel); 
			if (IEditUser.vChangeMade)
				vChangeMade = true;
		}
		else 
		if (nResponse == DELETE_USER)
		{
			UserPasswords* TempUser = (vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 6)));
			ReportErrorMessage("Operator deleted user: " + TempUser->vUserName, cUserChanges, 0);
			DeleteUser(TempUser);
			vChangeMade = true;
		}
		else 
		if (nResponse == 10)
		{
			CDialog::EndDialog(10);
		}
		this->Invalidate();
		UpdateDisplay();
	}
}

void CSetupUserDialog::OnSubFunction8Button() 
{
	if (vCurrentFirstItem + 7 < vCurrentNumberOfFilteredUsers)
	{
		CEditUser IEditUser;
		IEditUser.vMainWindowPointer = vMainWindowPointer;
		IEditUser.vNewUser = false;
		IEditUser.vCurrentUser = *(vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 7)));
		IEditUser.vLocalSystemData = this->vLocalSystemData;
		IEditUser.vLocalUserCollection = &(this->vLocalUserCollection);
		int nResponse = IEditUser.DoModal();
		if (nResponse == IDOK)
		{
			*(vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 7))) = IEditUser.vCurrentUser;
			MakeFilteredList(vCurrentFilteredLevel); 
			if (IEditUser.vChangeMade)
				vChangeMade = true;
		}
		else 
		if (nResponse == DELETE_USER)
		{
			UserPasswords* TempUser = (vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 7)));
			ReportErrorMessage("Operator deleted user: " + TempUser->vUserName, cUserChanges, 0);
			DeleteUser(TempUser);
			vChangeMade = true;
		}
		else 
		if (nResponse == 10)
		{
			CDialog::EndDialog(10);
		}
		this->Invalidate();
		UpdateDisplay();
	}
}

HBRUSH CSetupUserDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Background, 5);  
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

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Display, 5);
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

BOOL CSetupUserDialog::PreTranslateMessage(MSG* pMsg) 
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

void CSetupUserDialog::UpdateDisplay()
{
	CWnd* pWnd;
	CDC* pDC;
	HWND TempDialogHwnd = this->m_hWnd;

	if ((vCurrentFirstItem >= vCurrentNumberOfFilteredUsers) && (vCurrentFirstItem > 0))
	if (vCurrentFirstItem > 8)
		vCurrentFirstItem = vCurrentFirstItem - 8; //Accounts for deleting all users off a page
	else
		vCurrentFirstItem = 0;

	for (BYTE TempLoop = 0; TempLoop  < 8; TempLoop++)
	{ 
		CString TempFunctionString = "Edit User"; 
		switch (TempLoop)
		{
			case 0:
			{
				if (vCurrentFirstItem < vCurrentNumberOfFilteredUsers)
				{
					CString TempBoxString = (*vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem))).vUserName;
					m_SubFunction1Button.ShowWindow(SW_SHOW);
					m_SubFunction1Display.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction1Display,TempBoxString);
					SetDlgItemText(IDC_SubFunction1Button,TempFunctionString);

					pDC = m_SubFunction1Display.GetDC();
					pWnd = &m_SubFunction1Display;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5) + 30;  //5 is medium large
					int TempDisplayBoxLength = (int)(TempStringLength / vGlobalPercentToResizeY);
					if (TempDisplayBoxLength > 400)
						TempDisplayBoxLength = 400;

					m_SubFunction1Display.SetWindowPos(NULL, 110, 52, TempDisplayBoxLength, 28, SWP_NOZORDER);
				}
				else
				{
					m_SubFunction1Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
					m_SubFunction1Display.ShowWindow(SW_HIDE);
				}
			}
			break;
			case 1:
			{
				if (vCurrentFirstItem + 1 < vCurrentNumberOfFilteredUsers)
				{
					CString TempBoxString = (*vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 1))).vUserName;
					m_SubFunction2Button.ShowWindow(SW_SHOW);
					m_SubFunction2Display.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction2Display,TempBoxString);
					SetDlgItemText(IDC_SubFunction2Button,TempFunctionString);

					pDC = m_SubFunction2Display.GetDC();
					pWnd = &m_SubFunction2Display;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Display, 5) + 30;  //5 is medium large
					int TempDisplayBoxLength = (int)(TempStringLength / vGlobalPercentToResizeY);
					if (TempDisplayBoxLength > 400)
						TempDisplayBoxLength = 400;

					m_SubFunction2Display.SetWindowPos(NULL, 110, 142, TempDisplayBoxLength, 28, SWP_NOZORDER);
				}
				else
				{
					m_SubFunction2Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
					m_SubFunction2Display.ShowWindow(SW_HIDE);
				}
			}
			break;
			case 2:
			{
				if (vCurrentFirstItem + 2 < vCurrentNumberOfFilteredUsers)
				{
					CString TempBoxString = (*vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 2))).vUserName;
					m_SubFunction3Button.ShowWindow(SW_SHOW);
					m_SubFunction3Display.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction3Display,TempBoxString);
					SetDlgItemText(IDC_SubFunction3Button,TempFunctionString);

					pDC = m_SubFunction3Display.GetDC();
					pWnd = &m_SubFunction3Display;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Display, 5) + 30;  //5 is medium large
					int TempDisplayBoxLength = (int)(TempStringLength / vGlobalPercentToResizeY);
					if (TempDisplayBoxLength > 400)
						TempDisplayBoxLength = 400;

					m_SubFunction3Display.SetWindowPos(NULL, 110, 233, TempDisplayBoxLength, 28, SWP_NOZORDER);
				}
				else
				{
					m_SubFunction3Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
					m_SubFunction3Display.ShowWindow(SW_HIDE);
				}
			}
			break;
			case 3:
			{
				if (vCurrentFirstItem + 3 < vCurrentNumberOfFilteredUsers)
				{
					CString TempBoxString = (*vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 3))).vUserName;
					m_SubFunction4Button.ShowWindow(SW_SHOW);
					m_SubFunction4Display.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction4Display,TempBoxString);
					SetDlgItemText(IDC_SubFunction4Button,TempFunctionString);

					pDC = m_SubFunction4Display.GetDC();
					pWnd = &m_SubFunction4Display;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Display, 5) + 30;  //5 is medium large
					int TempDisplayBoxLength = (int)(TempStringLength / vGlobalPercentToResizeY);
					if (TempDisplayBoxLength > 400)
						TempDisplayBoxLength = 400;

					m_SubFunction4Display.SetWindowPos(NULL, 110, 323, TempDisplayBoxLength, 28, SWP_NOZORDER);
				}
				else
				{
					m_SubFunction4Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
					m_SubFunction4Display.ShowWindow(SW_HIDE);
				}
			}
			break;
			case 4:
			{
				if (vCurrentFirstItem + 4 < vCurrentNumberOfFilteredUsers)
				{
					CString TempBoxString = (*vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 4))).vUserName;
					m_SubFunction5Button.ShowWindow(SW_SHOW);
					m_SubFunction5Display.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction5Display,TempBoxString);
					SetDlgItemText(IDC_SubFunction5Button,TempFunctionString);

					pDC = m_SubFunction5Display.GetDC();
					pWnd = &m_SubFunction5Display;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Display, 5) + 30;  //5 is medium large
					int TempDisplayBoxLength = (int)(TempStringLength / vGlobalPercentToResizeY);
					if (TempDisplayBoxLength > 400)
						TempDisplayBoxLength = 400;

					m_SubFunction5Display.SetWindowPos(NULL, 525 - TempStringLength, 86, TempDisplayBoxLength, 28, SWP_NOZORDER);
				}
				else
				{
					m_SubFunction5Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
					m_SubFunction5Display.ShowWindow(SW_HIDE);
				}
			}
			break;
			case 5:
			{
				if (vCurrentFirstItem + 5 < vCurrentNumberOfFilteredUsers)
				{
					CString TempBoxString = (*vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 5))).vUserName;
					m_SubFunction6Button.ShowWindow(SW_SHOW);
					m_SubFunction6Display.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction6Display,TempBoxString);
					SetDlgItemText(IDC_SubFunction6Button,TempFunctionString);

					pDC = m_SubFunction6Display.GetDC();
					pWnd = &m_SubFunction6Display;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Display, 5) + 30;  //5 is medium large
					int TempDisplayBoxLength = (int)(TempStringLength / vGlobalPercentToResizeY);
					if (TempDisplayBoxLength > 400)
						TempDisplayBoxLength = 400;

					m_SubFunction6Display.SetWindowPos(NULL, 525 - TempStringLength, 176, TempDisplayBoxLength, 28, SWP_NOZORDER);
				}
				else
				{
					m_SubFunction6Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
					m_SubFunction6Display.ShowWindow(SW_HIDE);
				}
			}
			break;
			case 6:
			{
				if (vCurrentFirstItem + 6 < vCurrentNumberOfFilteredUsers)
				{
					CString TempBoxString = (*vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 6))).vUserName;
					m_SubFunction7Button.ShowWindow(SW_SHOW);
					m_SubFunction7Display.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction7Display,TempBoxString);
					SetDlgItemText(IDC_SubFunction7Button,TempFunctionString);

					pDC = m_SubFunction7Display.GetDC();
					pWnd = &m_SubFunction7Display;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Display, 5) + 30;  //5 is medium large
					int TempDisplayBoxLength = (int)(TempStringLength / vGlobalPercentToResizeY);
					if (TempDisplayBoxLength > 400)
						TempDisplayBoxLength = 400;

					m_SubFunction7Display.SetWindowPos(NULL, 525 - TempStringLength, 267, TempDisplayBoxLength, 28, SWP_NOZORDER);
				}
				else
				{
					m_SubFunction7Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
					m_SubFunction7Display.ShowWindow(SW_HIDE);
				}
			}
			break;
			case 7:
			{
				if (vCurrentFirstItem + 7 < vCurrentNumberOfFilteredUsers)
				{
					CString TempBoxString = (*vFilteredUserCollection.GetAt(vFilteredUserCollection.FindIndex(vCurrentFirstItem + 7))).vUserName;
					m_SubFunction8Button.ShowWindow(SW_SHOW);
					m_SubFunction8Display.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction8Display,TempBoxString);
					SetDlgItemText(IDC_SubFunction8Button,TempFunctionString);

					pDC = m_SubFunction8Display.GetDC();
					pWnd = &m_SubFunction8Display;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Display, 5) + 30;  //5 is medium large
					int TempDisplayBoxLength = (int)(TempStringLength / vGlobalPercentToResizeY);
					if (TempDisplayBoxLength > 400)
						TempDisplayBoxLength = 400;

					m_SubFunction8Display.SetWindowPos(NULL, 525 - TempStringLength, 357, TempDisplayBoxLength, 28, SWP_NOZORDER);
				}
				else
				{
					m_SubFunction8Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
					m_SubFunction8Display.ShowWindow(SW_HIDE);
				}
			}
			break;
		}
	}

	CString TempText;

	if (vCurrentFirstItem + cMaxNumberOfItemsPerPage < vCurrentNumberOfFilteredUsers)
	{
		m_Function5Button.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_Next);
		SetDlgItemText(IDC_Function5Button,TempText);
	}
	else
	{
		m_Function5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
	}

	if (vCurrentFirstItem > 1)
	{
		m_Function3Button.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_Previous);
		SetDlgItemText(IDC_Function3Button,TempText);
	}
	else
	{
		m_Function3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
	}

	if (vChangeMade == true)
	{
		//m_Function4Button.ShowWindow(SW_SHOW);
		//TempText.LoadString(IDS_Cancel);
		SetDlgItemText(IDC_Function2Button,TempText);
		m_Function2Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		TempText.LoadString(IDS_Exit);
		SetDlgItemText(IDC_Function2Button,TempText);
	}
	else
	{
		m_Function4Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());
		TempText.LoadString(IDS_Exit);
		SetDlgItemText(IDC_Function2Button,TempText);
	}
	
	TempText.LoadString(IDS_New);
	SetDlgItemText(IDC_Function1Button,TempText);
	
	TempText.LoadString(IDS_Cancel);
	SetDlgItemText(IDC_Function4Button,TempText);

	TempText = cLevelName[vCurrentFilteredLevel] + " Users";
	SetDlgItemText(IDC_DialogTitleStaticText1, TempText);
	::SetupMenu(vLocalDisplayCollection);
}

void CSetupUserDialog::SetupMenu()
{
	;
}

void CSetupUserDialog::OnDialogtitlestatictext1()
{
	//bring up soft key menu for help, print screen, Image, Clear, and hidden shift key
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
