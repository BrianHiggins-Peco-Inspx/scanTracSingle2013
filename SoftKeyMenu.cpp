// SoftKeyMenu.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "SoftKeyMenu.h"
#include "afxdialogex.h"

// CSoftKeyMenu dialog

IMPLEMENT_DYNAMIC(CSoftKeyMenu, CDialogEx)

extern int vGlobalPasswordLevel;

CSoftKeyMenu::CSoftKeyMenu(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSoftKeyMenu::IDD, pParent)
{
	vResultKey = 0;
	vOnMainMenu = false;
	vDisableKeys = 0;
	vMenuType = 0;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_SoftKey1);
	vLocalCWndCollection.Add(&m_SoftKey2);
	vLocalCWndCollection.Add(&m_SoftKey3);
	vLocalCWndCollection.Add(&m_SoftKey4);
	vLocalCWndCollection.Add(&m_SoftKey5);
	vLocalCWndCollection.Add(&m_Cancel);
}

void CSoftKeyMenu::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (vGlobalPasswordLevel)
		SetDlgItemText(IDC_SoftKey1, _T("Clear Password"));

	if (vDisableKeys & 1)
		m_SoftKey1.ShowWindow(SW_HIDE);

	if (vDisableKeys & 2)
		m_SoftKey2.ShowWindow(SW_HIDE);

	if (vDisableKeys & 4)
		m_SoftKey3.ShowWindow(SW_HIDE);

	if (vOnMainMenu)
		SetDlgItemText(IDC_SoftKey4, vLanguageString);
	else
		SetDlgItemText(IDC_SoftKey4, _T("Keyboard Show/Hide"));
	if (vDisableKeys & 8)
		m_SoftKey4.ShowWindow(SW_HIDE);

	if (vDisableKeys & 16)
		m_SoftKey5.ShowWindow(SW_HIDE);

	this->SetWindowPos(NULL,0,0,640,120,SWP_NOZORDER);

	this->SetWindowText(_T("SoftKey"));

	SetupMenu(vLocalCWndCollection);

	// TODO: Add your message handler code here
}

CSoftKeyMenu::~CSoftKeyMenu()
{
}

void CSoftKeyMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SoftKey1, m_SoftKey1);
	DDX_Control(pDX, IDC_SoftKey2, m_SoftKey2);
	DDX_Control(pDX, IDC_SoftKey3, m_SoftKey3);
	DDX_Control(pDX, IDC_SoftKey4, m_SoftKey4);
	DDX_Control(pDX, IDC_SoftKey5, m_SoftKey5);
	DDX_Control(pDX, IDC_Cancel, m_Cancel);
}


BEGIN_MESSAGE_MAP(CSoftKeyMenu, CDialogEx)
	ON_STN_CLICKED(IDC_Cancel, &CSoftKeyMenu::OnCancel)
	ON_STN_CLICKED(IDC_SoftKey1, &CSoftKeyMenu::OnSoftkey1)
	ON_STN_CLICKED(IDC_SoftKey2, &CSoftKeyMenu::OnSoftkey2)
	ON_STN_CLICKED(IDC_SoftKey3, &CSoftKeyMenu::OnSoftkey3)
	ON_STN_CLICKED(IDC_SoftKey4, &CSoftKeyMenu::OnSoftkey4)
	ON_STN_CLICKED(IDC_SoftKey5, &CSoftKeyMenu::OnSoftkey5)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CSoftKeyMenu message handlers


void CSoftKeyMenu::OnCancel()
{
	// TODO: Add your control notification handler code here
	vResultKey = 0;
	CDialog::EndDialog(0);
}


void CSoftKeyMenu::OnSoftkey1()
{
	// Enter or Clear Password
	if ((vDisableKeys & 1) == 0)
		SendSoftKey((vMenuType * 5) + 1);
}


void CSoftKeyMenu::OnSoftkey2()
{
	// Shift Key - Shown as Alternate Functions
	if ((vDisableKeys & 2) == 0)
		SendSoftKey((vMenuType * 5) + 2);
}


void CSoftKeyMenu::OnSoftkey3()
{
	// Help
	if ((vDisableKeys & 4) == 0)
		SendSoftKey((vMenuType * 5) + 3);
}


void CSoftKeyMenu::OnSoftkey4()
{
	//subfunction5button + turns vTouchScreen=true
	if ((vDisableKeys & 8) == 0)
		SendSoftKey((vMenuType * 5) + 4);
}


void CSoftKeyMenu::OnSoftkey5()
{
	// Print Screen
	if ((vDisableKeys & 16) == 0)
		SendSoftKey((vMenuType * 5) + 5);
}

void CSoftKeyMenu::SendSoftKey(BYTE TempKey)
{
	vResultKey = TempKey;
	CDialog::EndDialog(1);
}


HBRUSH CSoftKeyMenu::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
		
	if (pWnd->GetDlgCtrlID() == IDC_SoftKey1)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SoftKey1, 5);  //5 is medium large
	if (vDisableKeys & 1)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SoftKey2)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SoftKey2, 5);
	if (vDisableKeys & 2)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SoftKey3)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SoftKey3, 5);
	if (vDisableKeys & 4)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SoftKey4)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SoftKey4, 5);
	if (vDisableKeys & 8)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SoftKey5)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SoftKey5, 5);
	if (vDisableKeys & 16)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	}

	if ((pWnd->GetDlgCtrlID() == IDC_SoftKey1) ||
		(pWnd->GetDlgCtrlID() == IDC_SoftKey2) ||
		(pWnd->GetDlgCtrlID() == IDC_SoftKey3) ||
		(pWnd->GetDlgCtrlID() == IDC_SoftKey4) ||
		(pWnd->GetDlgCtrlID() == IDC_SoftKey5))
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Cancel)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Cancel, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

	return hbr;
}

