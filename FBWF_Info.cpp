//ScanTrac Side View Source File
// FBWF_Info.cpp : implementation file
//

#include "stdafx.h"
#include "string.h"
#include "stdio.h"
#include "afx.h"
#include "afxwin.h"
#include "process.h"
#include "scantrac.h"
#include "scantracdlg.h"
#include "selectitemdialog.h"
#include "FBWF_Info.h"
#include "HelpDialog.h"
#include "ConfigureDirectoryNames.h"
#include "AlphaEntryDialog.h"
#include "YesNoDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int vGlobalPasswordLevel;
extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
extern CProduct *vGlobalCurrentProduct;
extern CString vGlobalCurrentDirectory;
//extern bool vGlobalInWindowsDialog;
extern CScanTracDlg *vGlobalMainWindowPointer;
extern HMODULE vGlobalhLibFBWFNetwork;
extern double vGlobalHardDriveTotalBytes;
extern double DetermineHardDiskSize(CString TempCheckDirectory);	// 
extern DWORD vGlobalFBWFEnable;
/////////////////////////////////////////////////////////////////////////////
// FBWF_Info dialog



FBWF_Info::FBWF_Info(CWnd* pParent /*=NULL*/)
	: CDialog(FBWF_Info::IDD, pParent)
{
	//{{AFX_DATA_INIT(FBWF_Info)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	vLocalProductCollection = NULL;
	vLocalSystemData = NULL;
	vMainWindowPointer = NULL;
	vLocalConfigurationData = NULL;
	if (vGlobalhLibFBWFNetwork == NULL)
	{
		vGlobalhLibFBWFNetwork = LoadLibrary(_T("fbwflib.dll")); // file name of module
	}
	vFBWFTimerHandle = 47;
	vUsedSpace = 1;
	vTotalSpace = 65536L;
	vTotalNextSpace = 65536L;
	vFBWFCurrentEnabled = true;
	vFBWFNextEnabled = true;
	vFbwfEnabledStatusReadok = true;
	vFbwfStorageStatusReadok = true;
	vFBWFDirectoryToCommit = " c:\\ScanTracLogs" ;
	vFBWFFileToCommit = "\\ScanTracLogs\\FBWFLogFile.txt" ;
	vFBWFAvailable = false ;
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_CommitFileLabel);
	vLocalCWndCollection.Add(&m_AvailSpaceLabel);
	vLocalCWndCollection.Add(&m_UsedSpaceLabel);
	vLocalCWndCollection.Add(&m_TotalSpaceLabel);
	vLocalCWndCollection.Add(&m_AvailSpace);
	vLocalCWndCollection.Add(&m_UsedSpace);
	vLocalCWndCollection.Add(&m_TotalSpace);
	vLocalCWndCollection.Add(&m_WaitMessageLabel);
	vLocalCWndCollection.Add(&m_ChangeOnRebootNotifyLabel);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_AvaiableHardDriveNotifyLabel);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_EnableDisableStateLabel);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_MoreDiskDriveInfo);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
}


void FBWF_Info::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FBWF_Info)
	DDX_Control(pDX, IDC_CommitFileLabel, m_CommitFileLabel);
	DDX_Control(pDX, IDC_AvailSpaceLabel, m_AvailSpaceLabel);
	DDX_Control(pDX, IDC_UsedSpaceLabel, m_UsedSpaceLabel);
	DDX_Control(pDX, IDC_TotalSpaceLabel, m_TotalSpaceLabel);
	DDX_Control(pDX, IDC_AvailSpace, m_AvailSpace);
	DDX_Control(pDX, IDC_UsedSpace, m_UsedSpace);
	DDX_Control(pDX, IDC_TotalSpace, m_TotalSpace);
	DDX_Control(pDX, IDC_WaitMessageLabel, m_WaitMessageLabel);
	DDX_Control(pDX, IDC_ChangeOnRebootNotifyLabel, m_ChangeOnRebootNotifyLabel);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_AvailableHardDriveNotifyLabel, m_AvaiableHardDriveNotifyLabel);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_EnableDisableStateLabel, m_EnableDisableStateLabel);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_MoreDiskDriveInfo, m_MoreDiskDriveInfo);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
}


BEGIN_MESSAGE_MAP(FBWF_Info, CDialog)
	//{{AFX_MSG_MAP(FBWF_Info)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_BN_CLICKED(IDC_SubFunction4Button, OnSubFunction4Button)
	ON_BN_CLICKED(IDC_SubFunction1Button, OnSubFunction1Button)
	ON_BN_CLICKED(IDC_SubFunction3Button, OnSubFunction3Button)
	ON_BN_CLICKED(IDC_SubFunction5Button, OnSubFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction6Button, OnSubFunction6Button)
	ON_BN_CLICKED(IDC_SubFunction7Button, OnSubFunction7Button)
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_BN_CLICKED(IDC_SubFunction2Button, OnSubFunction2Button)
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
	ON_BN_CLICKED(IDC_Function5Button, OnFunction5Button)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &FBWF_Info::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FBWF_Info message handlers

BOOL FBWF_Info::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL FBWF_Info::PreTranslateMessage(MSG* pMsg) 
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
		if (vGlobalShiftKeyDown)
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
		else
		switch (pMsg->wParam)
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
			case cPageUp: // Clear, but enter System setup password
			{	
				if (vGlobalPasswordLevel)
				{
					PasswordOK(cNoPassword,false);
				}
				else
				{
					PasswordOK(cAnyPasswordToEnter, true);

					//if (PasswordOK(cTemporaryInspxPassword, true))
					//	ShowInspxButtons();
				}
				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			}
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
					cShowHelp, 9, 0);
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

void FBWF_Info::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	unsigned long TempDirBytes;
	unsigned long TempFileBytes;
	unsigned long TempCacheBytes;
	unsigned long TempNextCacheBytes;
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

	vSubFunction1ButtonEnable = true;
	vSubFunction2ButtonEnable = true;
	vSubFunction3ButtonEnable = true;
	vSubFunction4ButtonEnable = true;
	vSubFunction5ButtonEnable = true;
	vSubFunction6ButtonEnable = true;

	m_CommitFileLabel.ShowWindow(SW_HIDE);
	m_WaitMessageLabel.ShowWindow(SW_HIDE);

	//does FBWF manager exist on this system
	CString TempFileName = "\\WINDOWS\\system32\\fbwfmgr.exe";

	CFileStatus TempFileStatus;

	CString cTextString ;
	double TempFBWFHardDriveSpaceFree = CheckDiskSpaceFree("C:\\"); //	double vGlobalHardDriveTotalBytes set in function call
	//(vGlobalHardDriveTotalBytes)

	if (TempFBWFHardDriveSpaceFree >= 0x40000000)
			cTextString = "Drive C:\\ " + (dtoaWithCommas(TempFBWFHardDriveSpaceFree / 0x40000000, 0) + "GB Available, ");
		else
		if (TempFBWFHardDriveSpaceFree >= 0x100000)
			cTextString = "Drive C:\\ " + (dtoaWithCommas(TempFBWFHardDriveSpaceFree / 0x100000, 0) + "MB Available, ");
		else
			cTextString = "Drive C:\\ " + (dtoaWithCommas(TempFBWFHardDriveSpaceFree, 0) + " Bytes Available, ");

	if (vGlobalHardDriveTotalBytes >= 0x40000000)
			cTextString += (dtoaWithCommas(vGlobalHardDriveTotalBytes / 0x40000000, 0) + "GB Total");
		else
		if (vGlobalHardDriveTotalBytes >= 0x100000)
			cTextString += (dtoaWithCommas(vGlobalHardDriveTotalBytes / 0x100000, 0) + "MB Total");
		else
			cTextString += (dtoaWithCommas(vGlobalHardDriveTotalBytes, 0) + " Bytes Total");

	SetDlgItemText(IDC_AvailableHardDriveNotifyLabel, cTextString);

	
// FBWF get actual size of C:
/*	CString cNumbersString;
	double ValueDiskSize = DetermineHardDiskSize("C:\\");
	if (ValueDiskSize == 0)
	{
		cTextString = " Fail ";
	}
	else
	{
		cTextString = " ";
	}

	if (ValueDiskSize >= 0x40000000)
		cNumbersString = "Drive C:\\ " + (dtoaWithCommas(ValueDiskSize / 0x40000000, 0) + "GB Total");
	else
	if (TempFBWFHardDriveSpaceFree >= 0x100000)
		cNumbersString = "Drive C:\\ " + (dtoaWithCommas(ValueDiskSize / 0x100000, 0) + "MB Total");
	else
		cNumbersString = "Drive C:\\ " + (dtoaWithCommas(ValueDiskSize, 0) + " Bytes Total");

	cTextString += cNumbersString;

// FBWF get actual size of D:
	ValueDiskSize = DetermineHardDiskSize("D:\\");

	if (ValueDiskSize == 0)
	{
		cTextString += " Fail ";
	}
	else
	{
		cTextString += " ";
	}

	if (ValueDiskSize >= 0x40000000)
		cNumbersString = "Drive D:\\ " + (dtoaWithCommas(ValueDiskSize / 0x40000000, 0) + "GB Total");
	else
	if (TempFBWFHardDriveSpaceFree >= 0x100000)
		cNumbersString = "Drive D:\\ " + (dtoaWithCommas(ValueDiskSize / 0x100000, 0) + "MB Total");
	else
		cNumbersString = "Drive D:\\ " + (dtoaWithCommas(ValueDiskSize, 0) + " Bytes Total");


	cTextString += cNumbersString;
*/
	cTextString = " ";
	// get hard drive space on D: if D: exists
	TempFBWFHardDriveSpaceFree = CheckDiskSpaceFree("D:\\"); //	double vGlobalHardDriveTotalBytes set in function call
	//(vGlobalHardDriveTotalBytes)

	if (TempFBWFHardDriveSpaceFree >= 0x40000000)
			cTextString = "Drive D:\\ " + (dtoaWithCommas(TempFBWFHardDriveSpaceFree / 0x40000000, 0) + "GB Available, ");
		else
		if (TempFBWFHardDriveSpaceFree >= 0x100000)
			cTextString = "Drive D:\\ " + (dtoaWithCommas(TempFBWFHardDriveSpaceFree / 0x100000, 0) + "MB Available, ");
		else
			cTextString = "Drive D:\\ " + (dtoaWithCommas(TempFBWFHardDriveSpaceFree, 0) + " Bytes Available, ");

	if (vGlobalHardDriveTotalBytes >= 0x40000000)
			cTextString += (dtoaWithCommas(vGlobalHardDriveTotalBytes / 0x40000000, 0) + "GB Total");
		else
		if (vGlobalHardDriveTotalBytes >= 0x100000)
			cTextString += (dtoaWithCommas(vGlobalHardDriveTotalBytes / 0x100000, 0) + "MB Total");
		else
			cTextString += (dtoaWithCommas(vGlobalHardDriveTotalBytes, 0) + " Bytes Total");



	SetDlgItemText(IDC_MoreDiskDriveInfo, cTextString);
// end get disk partition information

// NO_ERROR
	if (CFile::GetStatus(TempFileName, TempFileStatus))
	{
		vFBWFAvailable = true;
		if (MemoryUsageFBWF(&TempDirBytes,&TempFileBytes,&TempCacheBytes,&TempNextCacheBytes) == NO_ERROR)
		{
			vTotalSpace = TempCacheBytes/1024 ;
			vTotalNextSpace = TempNextCacheBytes/1024 ;
		}

		UpdateDisplay();

	// value for vFBWFNextEnabled updated in UpdateDisplay
		if (vFBWFNextEnabled == false)
		{
			SetDlgItemText(IDC_SubFunction5Button,  _T("Enable Protection On Reboot"));
		}

		int TempTimerResult = SetTimer(vFBWFTimerHandle,1000,NULL);
		if (!TempTimerResult)
			ReportErrorMessage("Error-FBWF Timer Failed",cEMailInspx,32000);
	}
	else
	{
		DWORD vErrorValue ;
		vErrorValue = GetLastError();
		cTextString.Format(_T("Protection is not available on this system. Error = %d."),vErrorValue);

//		cTextString = FBWF is not available on this system."\n";
//		LPVOID ErrorMsgBuf;
//		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,NULL, vErrorValue , 
//									MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &ErrorMsgBuf,0,NULL);
//		cTextString = cTextString + (LPTSTR) ErrorMsgBuf;
//		LocalFree(ErrorMsgBuf);

		vFBWFAvailable = false;
		// grey all selections except main menu and exit, post notice FBWF not available
		SetDlgItemText(IDC_EnableDisableStateLabel,cTextString);
		m_AvailSpaceLabel.ShowWindow(SW_HIDE);
		m_UsedSpaceLabel.ShowWindow(SW_HIDE); 
		m_TotalSpaceLabel.ShowWindow(SW_HIDE);
		m_AvailSpace.ShowWindow(SW_HIDE);
		m_UsedSpace.ShowWindow(SW_HIDE); 
		m_TotalSpace.ShowWindow(SW_HIDE);
		vSubFunction1ButtonEnable = false;	//m_SubFunction1Button.EnableWindow(false);
		vSubFunction2ButtonEnable = false;	//m_SubFunction2Button.EnableWindow(false);
		vSubFunction3ButtonEnable = false;	//m_SubFunction3Button.EnableWindow(false);
		vSubFunction4ButtonEnable = false;	//m_SubFunction4Button.EnableWindow(false);
		vSubFunction5ButtonEnable = false;	//m_SubFunction5Button.EnableWindow(false);
		vSubFunction6ButtonEnable = false;	//m_SubFunction6Button.EnableWindow(false);
	}


	this->SetWindowText(_T("Inspx-Protection"));
	::SetupMenu(vLocalCWndCollection);
}

void FBWF_Info::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == vFBWFTimerHandle)
  {
		UpdateDisplay();
	}
	
	CDialog::OnTimer(nIDEvent);
}

HBRUSH FBWF_Info::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
		pDC->SetAttribDC(pDC->m_hDC);
	// TODO: Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_AvailSpaceLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_AvailSpaceLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_UsedSpaceLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_UsedSpaceLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_TotalSpaceLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TotalSpaceLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_AvailableHardDriveNotifyLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_AvaiableHardDriveNotifyLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MoreDiskDriveInfo)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MoreDiskDriveInfo, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EnableDisableStateLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EnableDisableStateLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ChangeOnRebootNotifyLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_AvailSpaceLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_CommitFileLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CommitFileLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_WaitMessageLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_WaitMessageLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_AvailSpace)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_AvailSpace, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_UsedSpace)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_UsedSpace, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_TotalSpace)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TotalSpace, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
  }

// make blue like the rest 
//	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
//	{
//		pDC->SetBkMode(TRANSPARENT);
//		return vLocalSystemData->vGreenBrush;
//  }

	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if ((pWnd->GetDlgCtrlID() == IDC_Background) ||
		(pWnd->GetDlgCtrlID() == IDC_CPProductLabel))
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

int FBWF_Info::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DoModal();
}

void FBWF_Info::OnFunction1Button() 
{
	// TODO: Add your control notification handler code here
	// User clicked Main Menu Button to exit 
	CDialog::EndDialog(10);	
}

void FBWF_Info::OnFunction2Button() 
{
	// TODO: Add your control notification handler code here
		CDialog::EndDialog(true);	
}

void FBWF_Info::OnFunction3Button() 
{
	// TODO: Add your control notification handler code here
	
}

void FBWF_Info::OnFunction4Button() 
{
	// TODO: Add your control notification handler code here
	
}

void FBWF_Info::OnFunction5Button() 
{
	// TODO: Add your control notification handler code here
	
}


// List files in Overlay
void FBWF_Info::OnSubFunction1Button() 
{
	CString TempCString;

	if (vFBWFAvailable)
	{
		m_WaitMessageLabel.ShowWindow(SW_SHOW);
		//send to cmd.exe the cmd: fbwfmgr /overlaydetail >FBWFLogFile.txt in the batch file

		//check if batch file exists, if not write it
		//If CreateFile() successfully creates the file, a valid HANDLE to that Win32 file object is returned; otherwise, INVALID_HANDLE_VALUE is returned
		HANDLE hBatchFileConfigInfo = CreateFile(_T("\\ScanTracFBWFBatchFiles\\OverlayDetail.bat"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ |FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hBatchFileConfigInfo != INVALID_HANDLE_VALUE) // returns invalid handle if file exists, don't need ((ErrorMsg = GetLastError()) != ERROR_FILE_EXISTS))
		{	// if file does not exist, write it
			CString BufferToWrite = "fbwfmgr /overlaydetail >C:\\ScanTracLogs\\FBWFOverlayDetail.txt";
			DWORD BytesWritten = 0; 
			
			CW2A TempStringToWrite(BufferToWrite);
			WriteFile(hBatchFileConfigInfo, TempStringToWrite, BufferToWrite.GetLength(), &BytesWritten, NULL);
		}
		CloseHandle(hBatchFileConfigInfo);

		//NOTE FILE NAME IN OVERLAYDETAIL.BAT MUST MATCH file name in CString FileNameToDisplay = "FBWFOverlayDetail.txt"
		//NOTE DIRECTORY NAME IN OVERLAYDETAIL.BAT MUST MATCH directory name in cLogDirectory c:\ScanTracLogs
		// OverlayDetail batch file is -- fbwfmgr /overlaydetail >C:\ScanTracLogs\FBWFOverlayDetail.txt
		int TempError = _spawnl(_P_NOWAIT, "C:\\ScanTracFBWFBatchFiles\\OverlayDetail.bat", "C:\\ScanTracFBWFBatchFiles\\OverlayDetail.bat", NULL);

		if (errno)
		{
				LPVOID ErrorMsgBuf;
				vOutputTextString.Format(_T("Could Not Run Overlay Detail File: Error %d.  \n"),errno);

				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,NULL, GetLastError(), 
											MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &ErrorMsgBuf,0,NULL);

				vOutputTextString = vOutputTextString + (LPTSTR) ErrorMsgBuf;

				LocalFree(ErrorMsgBuf);
		}
		else
		{
				Sleep(500);
				CString FileNameToDisplay = "FBWFOverlayDetail.txt"; 
				GetFBWFLogData(FileNameToDisplay);
		}

		// read file in and display to text screen
		CHelpDialog * IHelpDialog;
		IHelpDialog = new(CHelpDialog);
		IHelpDialog->vHelpContext = 16;
		IHelpDialog->vHelpType = 0;
		IHelpDialog->vWindowType = cTextWindow;
		IHelpDialog->vLocalSystemData = vLocalSystemData;
		IHelpDialog->vMainWindowPointer = vMainWindowPointer;
		IHelpDialog->vLocalConfigurationData = vLocalConfigurationData;
		IHelpDialog->vProductPointer = vGlobalCurrentProduct;

		IHelpDialog->vTitleString = "FBWF Files In Overlay";
		IHelpDialog->vTextString = vOutputTextString ;
		int TempResult = IHelpDialog->DoModal();
		delete IHelpDialog;
		if (TempResult == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(TempResult);
		}
		m_WaitMessageLabel.ShowWindow(SW_HIDE);
	}
	else
	if (vSubFunction1ButtonEnable == false)
		ShowFunctionNotValidDialogMessage();
}

//Files in Overlay Information
//fbwfmgr /displayconfig >c:\ScanTracLogs\FBWFconfig.txt	
// c:\ScanTracFBWFBatchFiles\DisplayConfig.bat
void FBWF_Info::OnSubFunction2Button() 
{
	CString TempCString;

	if (vFBWFAvailable)
	{
		m_WaitMessageLabel.ShowWindow(SW_SHOW);
		
	//was, this requires a batch file on the hard disk with the correct file name for the batch file and the result file 
	// better to have file names here

	//	_fflushall();
	// it seems like shouldn't need a batch file . . . did not get it to work
	//  int TempError = _spawnl(_P_NOWAIT, "C:\\WINDOWS\\system32\\fbwfmgr /displayconfig >c:\\scantrac\\FBWFconfig.txt", "C:\\WINDOWS\\system32\\fbwfmgr /displayconfig >c:\\scantrac\\FBWFconfig.txt", "\e /displayconfig >c:\\scantrac\\FBWFconfig.txt ", NULL);
	//  int TempError = _spawnl(_P_NOWAIT, "C:\\WINDOWS\\system32\\fbwfmgr.exe", "C:\\WINDOWS\\system32\\fbwfmgr.exe","/displayconfig >c:\\scantrac\\FBWFconfig.txt", NULL);
	//  int TempError = _spawnl(_P_NOWAIT, "C:\\WINDOWS\\system32\\fbwfmgr.exe", "C:\\WINDOWS\\system32\\fbwfmgr.exe","/displayconfig", NULL);
	//  int TempError = _spawnl(_P_NOWAIT, "C:\\WINDOWS\\system32\\fbwfmgr.exe", "C:\\WINDOWS\\system32\\fbwfmgr.exe","/displayconfig",">c:\\ScanTracLogs\\FBWFconfig.txt", NULL);
	//  int TempError = _spawnl(_P_NOWAIT, "C:\\WINDOWS\\system32\\fbwfmgr.exe", "C:\\WINDOWS\\system32\\fbwfmgr.exe","/displayconfig >c:\\ScanTracLogs\\FBWFconfig.txt", NULL);
	//  int TempError = _spawnl(_P_NOWAIT, "C:\\WINDOWS\\system32\\cmd.exe", "C:\\WINDOWS\\system32\\cmd.exe","fbwfmgr /displayconfig >c:\\ScanTracLogs\\FBWFconfig.txt","exit", NULL);
	//  int TempError = _spawnl(_P_NOWAIT, "C:\\WINDOWS\\system32\\cmd.exe", "C:\\WINDOWS\\system32\\cmd.exe","fbwfmgr /displayconfig >c:\\ScanTracLogs\\FBWFconfig.txt","exit", NULL);
	//  int TempError = _spawnl(_P_NOWAIT, "C:\\WINDOWS\\system32\\cmd.exe", "C:\\WINDOWS\\system32\\cmd.exe","edit c:\\ScanTracLogs\\FBWFconfig.txt", NULL);
	//  int TempError = _spawnl(_P_NOWAIT, "C:\\WINDOWS\\system32\\cmd.exe", "dir","exit", NULL);
	//  int TempError = _spawnl(_P_NOWAIT, "C:\\WINDOWS\\system32\\cmd.exe", "/e,dir","/e,exit", NULL);
	//  int TempError = _spawnl(_P_NOWAIT, "C:\\WINDOWS\\system32\\cmd.exe","C:\\WINDOWS\\system32\\cmd.exe","/e,dir", NULL);
	//  int TempError = _spawnl(_P_NOWAIT, "C:\\WINDOWS\\system32\\cmd.exe","C:\\WINDOWS\\system32\\cmd.exe","dir", NULL);
	//  int TempError = _spawnl(_P_NOWAIT, "C:\\WINDOWS\\system32\\fbwfmgr.exe", "C:\\WINDOWS\\system32\\fbwfmgr.exe","/displayconfig", NULL); //works does not save to file
	//  int TempError = _spawnl(_P_NOWAIT, "C:\\WINDOWS\\system32\\fbwfmgr.exe", "C:\\WINDOWS\\system32\\fbwfmgr.exe","/displayconfig",">c:\\ScanTracLogs\\FBWFconfig.txt", NULL); //no work
	//  int TempError = _spawnl(_P_NOWAIT, "C:\\WINDOWS\\system32\\fbwfmgr.exe", "C:\\WINDOWS\\system32\\fbwfmgr.exe","/displayconfig >c:\\ScanTracLogs\\FBWFconfig.txt", NULL); 
	//	int TempError = _spawnl(_P_NOWAIT, "C:\\WINDOWS\\system32\\fbwfmgr.exe", "C:\\WINDOWS\\system32\\fbwfmgr.exe","/displayconfig",">c:\\ScanTracLogs\\FBWFconfig.txt", NULL); //no work
	//  int TempError = _spawnl(_P_NOWAIT, "C:\\WINDOWS\\system32\\fbwfmgr.exe","C:\\WINDOWS\\system32\\fbwfmgr.exe","/displayconfig"," >c:\\ScanTracLogs\\FBWFconfig.txt", NULL);
	//  int TempError = _spawnl(_P_NOWAIT, "C:\\WINDOWS\\system32\\cmd.exe","C:\\WINDOWS\\system32\\cmd.exe","C:\\WINDOWS\\system32\\fbwfmgr.exe","/displayconfig"," >c:\\ScanTracLogs\\FBWFconfig.txt", NULL);

		//NOTE FILE NAME IN DISPLAYCONFIG.BAT MUST MATCH file name in CString FileNameToDisplay = "FBWFConfig.txt"
		//NOTE DIRECTORY NAME IN DISPLAYCONFIG.BAT MUST MATCH directory name in cLogDirectory c:\ScanTracLogs
		//batch file for DisplayConfig.bat is fbwfmgr /displayconfig >c:\ScanTracLogs\FBWFConfig.txt

		//check if batch file exists, if not write it
		//If CreateFile() successfully creates the file, a valid HANDLE to that Win32 file object is returned; otherwise, INVALID_HANDLE_VALUE is returned
		HANDLE hBatchFileConfigInfo = CreateFile(_T("\\ScanTracFBWFBatchFiles\\DisplayConfig.bat"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ |FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hBatchFileConfigInfo != INVALID_HANDLE_VALUE) // returns invalid handle if file exists, don't need ((ErrorMsg = GetLastError()) != ERROR_FILE_EXISTS))
		{	// if file does not exist, write it
			CString BufferToWrite = "fbwfmgr /displayconfig >c:\\ScanTracLogs\\FBWFConfig.txt";
			DWORD BytesWritten = 0; 
			
			CW2A TempStringToWrite(BufferToWrite);
			WriteFile(hBatchFileConfigInfo, TempStringToWrite, BufferToWrite.GetLength(), &BytesWritten, NULL);
		}
		CloseHandle(hBatchFileConfigInfo);

		int TempError = _spawnl(_P_NOWAIT, "C:\\ScanTracFBWFBatchFiles\\DisplayConfig.bat", "C:\\ScanTracFBWFBatchFiles\\DisplayConfig.bat", NULL);
		if (errno)
		{
				LPVOID ErrorMsgBuf;
				vOutputTextString.Format(_T("Could Not Display Excluded Files and Folders: Error %d.  \n"),errno);

				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,NULL, GetLastError(), 
											MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &ErrorMsgBuf,0,NULL);

				vOutputTextString = vOutputTextString + (LPTSTR) ErrorMsgBuf;

				LocalFree(ErrorMsgBuf);
		}
		else
		{
				Sleep(500);
				CString FileNameToDisplay = "FBWFConfig.txt";
				GetFBWFLogData(FileNameToDisplay);
		}

		
	/*	int TempError = system("fbwfmgr /displayconfig >c:\\ScanTracLogs\\FBWFconfig.txt");
		if (TempError)
		{
				vOutputTextString.Format(_T("Could Not Run FBWF Configuration Display: Error %d"),TempError);
		}
		else
		{
				Sleep(10);
				CString FileNameToDisplay = "FBWFconfig.txt";
				GetFBWFLogData(FileNameToDisplay);
		}
	*/
		// read file in and display to text screen
		CHelpDialog * IHelpDialog;
		IHelpDialog = new(CHelpDialog);
		IHelpDialog->vHelpContext = 16;
		IHelpDialog->vHelpType = 0;
		IHelpDialog->vWindowType = cTextWindow;
		IHelpDialog->vLocalSystemData = vLocalSystemData;
		IHelpDialog->vMainWindowPointer = vMainWindowPointer;
		IHelpDialog->vLocalConfigurationData = vLocalConfigurationData;
		IHelpDialog->vProductPointer = vGlobalCurrentProduct;

		IHelpDialog->vTitleString = "FBWF Excluded Folders";
		IHelpDialog->vTextString = vOutputTextString ;
		int TempResult = IHelpDialog->DoModal();
		delete IHelpDialog;
		if (TempResult == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(TempResult);
		}
		m_WaitMessageLabel.ShowWindow(SW_HIDE);
	}
	else
	if (vSubFunction2ButtonEnable == false)
		ShowFunctionNotValidDialogMessage();
}
	


#define constFileNameLength 250
// commit folder to FBWF
void FBWF_Info::OnSubFunction3Button() 
{
	if (vFBWFAvailable)
	{
		int DirPathCount;
		CString TempFileType = "*.*";
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		CString cSaveDirectory;
		CString TempFileString;
		CString TempCString;
		CString TempCString2;
		CString TempCString3;
		CString TempDisplayResultString;
		WCHAR FileName[constFileNameLength] =L" ";//= L"\\ScanTracLogs\\FBWFLogFile.txt";  
		PWCHAR pFileName = &FileName[0] ;
		BOOL FlagFileNameOk;	
		int TempReturnVal;

		CFileDialog *TempFileDialog = new CFileDialog(true,NULL,
			TempFileString, //NULL,
			OFN_HIDEREADONLY|OFN_ENABLESIZING|OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ,
			NULL, NULL);//vMainWindowPointer);

	//	vGlobalInWindowsDialog = true;
		if (TempFileDialog)
		{
			int nResponse1 = TempFileDialog->DoModal();

		//	vGlobalInWindowsDialog = false;

			if (nResponse1 == IDOK)
			{
				TempCString2 = TempFileDialog->GetFileName();
				TempCString3 = TempFileDialog->GetPathName();
		//use directory from TempCString to save files in directory	
				DirPathCount = TempCString3.GetLength() - TempCString2.GetLength();
			
		//	CString TempString = cACPowerMoniterLogDirectory + TempFileType;
		//	LPTSTR TempDirectoryName = TempString.GetBuffer(TempString.GetLength());

				cSaveDirectory = TempCString3.GetBufferSetLength(DirPathCount);

				vOutputTextString = "Commit Directory - " + cSaveDirectory + "\n" ;

				CString TempString = cSaveDirectory + TempFileType;
				LPTSTR TempDirectoryName = TempString.GetBuffer(TempString.GetLength());

				hFind = FindFirstFile(TempDirectoryName, &FindFileData);

				if (hFind == INVALID_HANDLE_VALUE) // No files in directory
				{
					TempDisplayResultString = "Commit Failed, No Files in Directory";
					vOutputTextString += TempDisplayResultString ;
				}
				else 
				{
					int LoopCount;
					do
					{
						TempCString2 = FindFileData.cFileName;
						vOutputTextString += "\n";
						vOutputTextString += cSaveDirectory;
						vOutputTextString += TempCString2;
						// if file exists, it is not a directory and the first char is not '.' then commit file
						if ((!TempCString2.IsEmpty()) && (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
									&& (TempCString2.GetAt(0)!='.') && (TempCString2.GetAt(0)!='~'))
						{
							TempCString3 = cSaveDirectory + TempCString2;										
							// add filename and directory
							FlagFileNameOk = true;
							for (LoopCount=0; LoopCount < ((TempCString3.GetLength())-2); LoopCount++)
							{
								FileName[LoopCount] = TempCString3.GetAt(LoopCount+2);	//skip c:
								if (LoopCount>(constFileNameLength-3))
								{	//error condition
									ReportErrorMessage("FBWF-Dir path + Filename too long",cEMailInspx,32000);
									vOutputTextString += "FBWF-Dir path + Filename too long";
									FlagFileNameOk = false;
									break;
								}
							}
							FileName[LoopCount] = 0;
							FileName[LoopCount+1] = 0;
							if ((FlagFileNameOk) && ((TempReturnVal = CommitFBWF(pFileName))== NO_ERROR))
							{
								TempCString = " commit pass";
								TempDisplayResultString = TempCString3 + TempCString;

								vOutputTextString += TempCString ;
							}
							else
							{
								TempCString.Format(_T(" commit failed, Error=%ld"),TempReturnVal);
								TempDisplayResultString = TempCString3 + TempCString;

								vOutputTextString += TempCString ;
							}
						}
						else
						{
							vOutputTextString += "Not a File, not committed";
						}
					}
					while (FindNextFile(hFind,&FindFileData));
				}
		//			output results to a help screen
				CHelpDialog * IHelpDialog;
				IHelpDialog = new(CHelpDialog);
				IHelpDialog->vHelpContext = 16;
				IHelpDialog->vHelpType = 0;
				IHelpDialog->vWindowType = cTextWindow;
				IHelpDialog->vLocalSystemData = vLocalSystemData;
				IHelpDialog->vMainWindowPointer = vMainWindowPointer;
				IHelpDialog->vLocalConfigurationData = vLocalConfigurationData;
				IHelpDialog->vProductPointer = vGlobalCurrentProduct;

				IHelpDialog->vTitleString = "FBWF Commit Information";
				IHelpDialog->vTextString = vOutputTextString;
				int TempResult = IHelpDialog->DoModal();
				delete IHelpDialog;
				if (TempResult == 10)
				{
					//Main Menu button pressed
					Sleep(1); //is the equivelent of a yeild statement;
					CDialog::EndDialog(TempResult);
				}
				SetDlgItemText(IDC_CommitFileLabel,TempDisplayResultString);
				m_CommitFileLabel.ShowWindow(SW_SHOW);

				FindClose(hFind);
			}	
		}
	}
	else
	if (vSubFunction3ButtonEnable == false)
		ShowFunctionNotValidDialogMessage();
}

// commit file to FBWF
void FBWF_Info::OnSubFunction4Button() 
{
	if (vFBWFAvailable)
	{
		ULONG TempReturnVal;
		WCHAR FileName[constFileNameLength] = L" "; //= L"\\ScanTracLogs\\FBWFLogFile.txt";  
		PWCHAR pFileName = &FileName[0] ;		
		CString TempCString;
		CString TempFileString;
		CString TempCString2;
		CString TempCString3;
		BOOL FlagFileNameOk;

		CFileDialog *TempFileDialog = new CFileDialog(true,NULL,
			TempFileString, //NULL,
			OFN_HIDEREADONLY|OFN_ENABLESIZING|OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ,
			NULL,NULL);//vMainWindowPointer);

	//	vGlobalInWindowsDialog = true;
		int nResponse1 = TempFileDialog->DoModal();

	//	vGlobalInWindowsDialog = false;

		if (nResponse1 == IDOK)
		{
			TempCString2 = TempFileDialog->GetFileName();
			TempCString3 = TempFileDialog->GetPathName();
		
	// below how to enter the filename with alpha entry dialog
	//	//Set dialog box data titles and number value
	//	CAlphaEntryDialog IAlphaEntryDialog;  
	//	IAlphaEntryDialog.vEditString = vFBWFFileToCommit;
	//
	//	IAlphaEntryDialog.m_DialogTitleStaticText = "FBWF Directory Name to Commit";
	//
	//	IAlphaEntryDialog.m_DialogTitleStaticText2 = "Default: \\ScanTracLogs\\FBWFLogFile.txt";
	//	IAlphaEntryDialog.vAllowSlashes = true;
	//	IAlphaEntryDialog.vAllowMoreThan32Characters = true;
	//	IAlphaEntryDialog.vBlankNameOK = true;
	//	//Pass control to dialog box and display
	//	int nResponse = IAlphaEntryDialog.DoModal();
	//	//dialog box is now closed, if user pressed select do this
	//	//if user pressed cancel, do nothing
	//	if (nResponse == IDOK)
	//	{
	//		int LoopCount;
	//		vFBWFFileToCommit = IAlphaEntryDialog.vEditString;
	//		if (!vFBWFFileToCommit.IsEmpty())
	//		{
	//			for (LoopCount=0; i < vFBWFFileToCommit.GetLength(); LoopCount++)
	//			{
	//				FileName[LoopCount] = vFBWFFileToCommit.GetAt(LoopCount);
	//			}
	//			FileName[LoopCount] = 0;
	//			FileName[LoopCount+1] = 0;
	//		}
	//	}

			if (!TempCString3.IsEmpty() && (TempCString2.GetAt(0)!='.') && (TempCString2.GetAt(0)!='~'))
			{
				int LoopCount;
				FlagFileNameOk = true;
				for (LoopCount=0; LoopCount < ((TempCString3.GetLength())-2); LoopCount++)
				{
					FileName[LoopCount] = TempCString3.GetAt(LoopCount+2);	//skip c:
					if (LoopCount>(constFileNameLength-3))
					{
						ReportErrorMessage("FBWF-Dir path + Filename too long",cEMailInspx,32000);
						FlagFileNameOk = false;
						break;
					}
				}
				FileName[LoopCount] = 0;
				FileName[LoopCount+1] = 0;
			}
			if (FlagFileNameOk) 
			{
				if ((TempReturnVal = CommitFBWF(pFileName))== NO_ERROR)
				{
					TempCString = " Commit pass";
					TempCString2 = TempCString3 + TempCString;
				}
				else
				{
					TempCString.Format(_T(" Commit failed, Error=%ld"),TempReturnVal);
					TempCString2 = TempCString3 + TempCString;			
//				ERROR_FILE_NOT_FOUND(2), ERROR_INVALID_DRIVE(15),ERROR_INVALID_FUNCTION(1),ERROR_INVALID_PARAMETER(87),ERROR_WRITE_PROTECT(19)
//					LPVOID ErrorMsgBuf;
//					FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,NULL, GetLastError(), 
//												MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &ErrorMsgBuf,0,NULL);
//					TempCString2 = TempCString2 + (LPTSTR) ErrorMsgBuf;
//					LocalFree(ErrorMsgBuf);
				}
			}
			else
			{
				TempCString2 = TempCString3 + " Filename too long, Commit fail.";
			}

			SetDlgItemText(IDC_CommitFileLabel,TempCString2);
			m_CommitFileLabel.ShowWindow(SW_SHOW);
		}
		else 
		if (nResponse1 == 10)
		{
			delete TempFileDialog;
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yield statement;
			CDialog::EndDialog(10);
		}	
		delete TempFileDialog;
	}
	else
	if (vSubFunction4ButtonEnable == false)
		ShowFunctionNotValidDialogMessage();
}

void FBWF_Info::OnSubFunction5Button() 
{
	if (vFBWFAvailable)
	{
		CString TempCString;
		ULONG TempReturnVal;

		//called on UpdateDisplay ULONG FBWF_Info::IsFBWFEnabled(unsigned long * currentSession,unsigned long * nextSession)
		if (vFBWFNextEnabled)
		{	// currently next reboot enabled, switch to next reboot disabled
			if ((TempReturnVal = DisableFBWF()) == NO_ERROR)
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nHard Drive Protection Will Be Disabled After Reboot";
				TempNoticeDialog.vType = cNoticeMessage;
				TempNoticeDialog.vAutoClose = 3000;
				TempNoticeDialog.DoModal();
				// set in UpdateDisplay() when FBWF okay		vFBWFNextEnabled = false;
				SetDlgItemText(IDC_SubFunction5Button, _T("Enable Protection On Reboot"));
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nError-Disable Hard Drive Protection Did Not Work\nProtection not Disabled, err = %ld";
				TempNoticeDialog.vType = cNoticeMessage;
				TempNoticeDialog.DoModal();

				vFBWFNextEnabled = false; // set here when FBWF not okay
				//TempCString.Format(_T("Protection not Disabled, err = %ld"),TempReturnVal);
				//SetDlgItemText(IDC_SubFunction5Button,TempCString);
			}
		}
		else
		{ // currently next reboot disabled, switch to next reboot enabled
			if ((TempReturnVal = EnableFBWF()) == NO_ERROR)
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nHard Drive Protection Will Be Enabled After Reboot";
				TempNoticeDialog.vType = cNoticeMessage;
				TempNoticeDialog.vAutoClose = 3000;
				TempNoticeDialog.DoModal();
				// set in UpdateDisplay()	when FBWF okay	vFBWFNextEnabled = true;
				SetDlgItemText(IDC_SubFunction5Button,  _T("Disable Protection On Reboot"));
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nError-Disable Hard Drive Protection Did Not Work\nProtection not Enabled, rtn = %ld";
				TempNoticeDialog.vType = cNoticeMessage;
				TempNoticeDialog.DoModal();

				vFBWFNextEnabled = true; // set here when FBWF not okay
				//TempCString.Format(_T("Protection not Enabled, rtn = %ld"),TempReturnVal);
				//SetDlgItemText(IDC_SubFunction5Button,TempCString);
			}
		}
		// want to update display here
		KillTimer(vFBWFTimerHandle);		// -- ok to add then restart timer ??
		UpdateDisplay();
		int TempTimerResult = SetTimer(vFBWFTimerHandle,1000,NULL);
		if (!TempTimerResult)
			ReportErrorMessage("Error-FBWF Timer Failed",cEMailInspx,32000);
	}
	else
	if (vSubFunction5ButtonEnable == false)
		ShowFunctionNotValidDialogMessage();
}

//Change ram overlay size here
void FBWF_Info::OnSubFunction6Button() 
{
	if (vFBWFAvailable)
	{
		int TempSelRamSize;
		CString CStringTemp;
		CString CStringTemp2;
		CString TempCString;
		ULONG vTotalNextSpaceTemp;
		DWORD vMemoryChoices;
		DWORD vAmountOfMemory;

		MEMORYSTATUS memstat;
		GlobalMemoryStatus(&memstat);
		if (memstat.dwTotalPhys > memstat.dwTotalPageFile)
			vAmountOfMemory = memstat.dwTotalPhys;
		else
			vAmountOfMemory = memstat.dwTotalPageFile;

		vMemoryChoices = vAmountOfMemory/(1024 * 4);		// # bytes divided by 4 and divided by 1024 into KB, 
																											// same as vTotalNextSpace(KB) and vTotalSpace(KB) of FBWF sizes
		CStringTemp.Format(_T("%ldMB"),vTotalNextSpace/1024);
		CStringTemp2 = "Next Reboot Protection = " + CStringTemp; 
	//for debug	CStringTemp.Format(_T("%ldMB"),vMemoryChoices/1024);
	//for debug	CStringTemp2 = CStringTemp2 + ", 1/4 Total Memory = " + CStringTemp ;
		SetDlgItemText(IDC_ChangeOnRebootNotifyLabel,CStringTemp2);
		m_ChangeOnRebootNotifyLabel.ShowWindow(SW_SHOW);

	//131072 = 128MB, sometimes memory reads low make compare to value 10% less
		if (vMemoryChoices >= 117760L)		// 1/4 total memory is great than or equal to 128MB
		{																	// start selection menu
			CSelectItemDialog ISelectItemDialog;  
			ISelectItemDialog.vNumberOfItems = 2;
			ISelectItemDialog.vItems[0] = "64MB";
			ISelectItemDialog.vItems[1] = "128MB";

			if (vMemoryChoices >= 235520L)		// 1/4 total memory is great than or equal to 256MB
			{
				ISelectItemDialog.vItems[2] = "256MB";
				ISelectItemDialog.vNumberOfItems++;
			}
			if (vMemoryChoices >= 471040L)		// 1/4 total memory is great than or equal to 512MB
			{
				ISelectItemDialog.vItems[3] = "512MB";
				ISelectItemDialog.vNumberOfItems++;
			}

			//Set dialog box data titles and number value
			ISelectItemDialog.vTitleString = "Select FBWF RAM Size";
			ISelectItemDialog.vBackgroundColor = cGreen;
			ISelectItemDialog.vShowExit = true;
			ISelectItemDialog.vInAutoSetup = false;
			ISelectItemDialog.vStayTillDecissionSetup = false;
			ISelectItemDialog.vStringItemSelecting = "After Reboot FBWF RAM: ";

			TempCString.Format(_T("After Reboot FBWF RAM: %ldMB"),vTotalNextSpace/1024);
			ISelectItemDialog.m_DialogTitleStaticText2 = TempCString ;
			TempCString.Format(_T("Current FBWF RAM: %ldMB"),vTotalSpace/1024);
			ISelectItemDialog.m_DialogTitleStaticText3 = TempCString ;
			//Pass control to dialog box and display
			int nResponse = ISelectItemDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				TempSelRamSize = ISelectItemDialog.vItemNumberSelected; // <<<--- answer select back here
				// set FBWF new size, and make reboot change message visible,1 = 64M, 2 = 128M, 3= 256M, 5 = 512M
				//vTotalSpace = current, vTotalNextSpace = next reboot from below function call done on update display
					switch (TempSelRamSize)
					{
					case 2:
							vTotalNextSpaceTemp = 131072L ;		
							CStringTemp = "128MB";
						break;
					case 3:
							vTotalNextSpaceTemp = 262144L;
							CStringTemp = "256MB";
						break;
					case 4:
							vTotalNextSpaceTemp = 524288L;
							CStringTemp = "512MB";
						break;
					case 1:
					default:
							CStringTemp = "64MB";
							vTotalNextSpaceTemp = 65536L;
						break;
					}
					if (vTotalNextSpace != vTotalNextSpaceTemp)
					{
						KillTimer(vFBWFTimerHandle);		// -- ok to add then restart timer ??
						if (SetNextCacheFBWF(vTotalNextSpaceTemp) == NO_ERROR)
						{
							vTotalNextSpace = vTotalNextSpaceTemp;
							CStringTemp2 = "Next Reboot Protection " + CStringTemp; 
							SetDlgItemText(IDC_ChangeOnRebootNotifyLabel,CStringTemp2);
						}
						else
						{
							CStringTemp2 = "Overlay Change to " + CStringTemp + " Fail" ; 
							SetDlgItemText(IDC_ChangeOnRebootNotifyLabel,CStringTemp2);
						}

						//Reset Timer
						int TempTimerResult = SetTimer(vFBWFTimerHandle,1000,NULL);
						if (!TempTimerResult)
							ReportErrorMessage("Error-FBWF Timer Failed",cEMailInspx,32000);
					}
					else
					{
							CStringTemp2 = "Next Reboot Overlay " + CStringTemp; 
							SetDlgItemText(IDC_ChangeOnRebootNotifyLabel,CStringTemp2);
					}
			}
			else
			if (nResponse != 11)
			{
				int TempOK = false;
				if (nResponse == 10)
				{
					Sleep(1); //is the equivelent of a yeild statement;
					CDialog::EndDialog(10);
				}
			}
		}	
		else
		{
			CStringTemp.Format(_T("%ldMB"),vTotalNextSpace/1024);
			CStringTemp2 = "Max Next Reboot Overlay " + CStringTemp; 
			CStringTemp.Format(_T("%ldMB"),vMemoryChoices/1024);
			CStringTemp2 = CStringTemp2 + ". MaxMem " + CStringTemp ;
			SetDlgItemText(IDC_ChangeOnRebootNotifyLabel,CStringTemp2);
			m_ChangeOnRebootNotifyLabel.ShowWindow(SW_SHOW);
		}
	}
	else
	if (vSubFunction6ButtonEnable == false)
		ShowFunctionNotValidDialogMessage();
}

void FBWF_Info::OnSubFunction7Button() 
{
	// TODO: Add your control notification handler code here
	
}

void FBWF_Info::OnSubFunction8Button() 
{
	// TODO: Add your control notification handler code here
	{
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText = _T("\n\n\nReboot Computer Now?");
		TempYesNoDialog.vYesButtonText = "Reboot Now!";
		TempYesNoDialog.vNoButtonText = "Reboot Later";
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
		{
			if (vMainWindowPointer)
				vMainWindowPointer->WriteConfigurationDataToFile(false);
			SystemShutDown(cReboot);
		}
	}
}



//kjh add below for FBWF testing
// return call succeed or falled, return in calling variables current session 1= enable FBWF, 0 = disabled FBWF
ULONG FBWF_Info::IsFBWFEnabled(unsigned long * currentSession,unsigned long * nextSession)
{
	DWORD dwRetVal = 0;
//	DWORD vTempStore = 0;
	*currentSession = 0;
	*nextSession = 0; 

	typedef ULONG (CALLBACK* mytry) (unsigned long * ,unsigned long * );
	mytry NewFbwfIsFilterEnabled;    // Function pointer

// free Library is causing an MFC exception error.  Load Library once when start, make a global
//	HMODULE hLibNetwork = LoadLibrary( ("fbwflib.dll")); // file name of module
//	HINSTANCE hLibNetwork = LoadLibrary( ("fbwflib.dll")); // file name of module

	if (vGlobalhLibFBWFNetwork != NULL)
	{
		LPCSTR lpProcName = "FbwfIsFilterEnabled";		// handle to DLL module // function name		 
		NewFbwfIsFilterEnabled = (mytry)GetProcAddress(vGlobalhLibFBWFNetwork,lpProcName);   

		if (NewFbwfIsFilterEnabled != NULL)
		{
			dwRetVal = NewFbwfIsFilterEnabled(currentSession, nextSession);
		}
	}		
	return(dwRetVal);
}

// return call succeed or falled
ULONG FBWF_Info::EnableFBWF(void)
{
	DWORD dwRetVal = 0;

	typedef ULONG (CALLBACK* mytry) ();
	mytry NewFbwfEnableFilter;    // Function pointer

// free Library is causing an MFC exception error.  Load Library once when start, make a global
//	HMODULE hLibNetwork = LoadLibrary( ("fbwflib.dll")); // file name of module
//	HINSTANCE hLibNetwork = LoadLibrary( ("fbwflib.dll")); // file name of module

	if (vGlobalhLibFBWFNetwork != NULL)
	{

		LPCSTR lpProcName = "FbwfEnableFilter";		// handle to DLL module // function name		 
		NewFbwfEnableFilter = (mytry)GetProcAddress(vGlobalhLibFBWFNetwork,lpProcName);   

		ReportErrorMessage("FBWF-Try to Enable Filter",cWriteToLog,0);

		if (NewFbwfEnableFilter != NULL)
		{
			dwRetVal = NewFbwfEnableFilter();
			if (dwRetVal == NO_ERROR)			//reboot only changes status of FBWF enable
				ReportErrorMessage("FBWF-Enable FBWFilter Worked",cWriteToLog,0);
			else
				ReportErrorMessage("FBWF-Enable FBWFilter DID NOT WORK",cWriteToLog,0);
//			if (dwRetVal == NO_ERROR)			reboot only changes status of FBWF enable
//				vGlobalFBWFEnable = 1;			this is 1 only after a reboot
		}
		else
			ReportErrorMessage("FBWF-NewFbwfEnableFilter is NULL",cEMailInspx,32000);
	}		
	else
		ReportErrorMessage("FBWF-vGlobalhLibFBWFNetwork is NULL",cEMailInspx,32000);

	return(dwRetVal);
}

// return call succeed or falled
ULONG FBWF_Info::DisableFBWF(void)
{
	DWORD dwRetVal = 0;
	CString TempCString = "";

	typedef ULONG (CALLBACK* mytry) ( );
	mytry NewFbwfDisableFilter;    // Function pointer

// free Library is causing an MFC exception error.  Load Library once when start, make a global
//	HMODULE hLibNetwork = LoadLibrary( ("fbwflib.dll")); // file name of module
//	HINSTANCE hLibNetwork = LoadLibrary( ("fbwflib.dll")); // file name of module

	if (vGlobalhLibFBWFNetwork != NULL)
	{
		LPCSTR lpProcName = "FbwfDisableFilter";		// handle to DLL module // function name		 
		NewFbwfDisableFilter = (mytry)GetProcAddress(vGlobalhLibFBWFNetwork,lpProcName);   

		if (NewFbwfDisableFilter != NULL)
		{
			dwRetVal = NewFbwfDisableFilter();
//			if (dwRetVal == NO_ERROR)		reboot only changes status of FBWF disable
//				vGlobalFBWFEnable = 0;		this is 0 only after a reboot
		}
	}		

	return(dwRetVal);
}

// return call succeed or falled
ULONG FBWF_Info::MemoryUsageFBWF(unsigned long * DirStructureBytes,unsigned long * FileDataBytes,unsigned long * CurrentCacheBytes,unsigned long * NextCacheBytes)
{
	DWORD dwRetVal = 0;
	CString TempCString = "";
	*DirStructureBytes = 0;
	*FileDataBytes = 0;
	*CurrentCacheBytes = 0;
	*NextCacheBytes=0;
//	BOOLEAN bLoadedLibraryHere = false;

	struct FbwfMemoryUsage
	{
		 ULONG currentCacheThreshold; // Cache threshold in bytes for the current session. 
		 ULONG nextCacheThreshold;		// Cache threshold in bytes for the next session
		 ULONG dirStructure;					// Memory in bytes used to store directory structure
		 ULONG fileData;							// Memory in bytes used to cache file data
	};

	FbwfMemoryUsage MemoryUsageStructure;
	FbwfMemoryUsage * ptrMemoryUsageStructure = &MemoryUsageStructure ;

		typedef ULONG (CALLBACK* mytry) (FbwfMemoryUsage * );
		mytry NewFbwfGetMemoryUsage;    // Function pointer

	// free Library is causing an MFC exception error.  Load Library once when start, make a global
	//	HMODULE hLibNetwork = LoadLibrary( ("fbwflib.dll")); // file name of module
	//	HINSTANCE hLibNetwork = LoadLibrary( ("fbwflib.dll")); // file name of module

//		if (vGlobalhLibFBWFNetwork == NULL)
//		{
//			vGlobalhLibFBWFNetwork = LoadLibrary("fbwflib.dll"); // file name of module
//			if (vGlobalhLibFBWFNetwork != NULL) // tried to load library, is successful then need to free library after function call
//			{
//				bLoadedLibraryHere = true;
//			}
//		}

		if (vGlobalhLibFBWFNetwork != NULL)
		{
			LPCSTR lpProcName = "FbwfGetMemoryUsage";		// handle to DLL module // function name		 
			NewFbwfGetMemoryUsage = (mytry)GetProcAddress(vGlobalhLibFBWFNetwork,lpProcName);   

			if (NewFbwfGetMemoryUsage != NULL)
			{
				dwRetVal = NewFbwfGetMemoryUsage(ptrMemoryUsageStructure);

				if (dwRetVal == NO_ERROR)
				{
					*CurrentCacheBytes = MemoryUsageStructure.currentCacheThreshold;
					*NextCacheBytes = MemoryUsageStructure.nextCacheThreshold;
					*DirStructureBytes = MemoryUsageStructure.dirStructure;
					*FileDataBytes = MemoryUsageStructure.fileData;
				}
			}
//			if (bLoadedLibraryHere)
//			{
//				FreeLibrary(vGlobalhLibFBWFNetwork);
//				vGlobalhLibFBWFNetwork = NULL ;
//			}

		}		

	return(dwRetVal);
}

// return call succeed or falled
// fbwf library must be opened before using this function.
// the libray function is a global vGlobalhLibFBWFNetwork, initialized to null
// file name pointer passed must be to a wide char string.
ULONG FBWF_Info::CommitFBWF(PWCHAR ptrFileName)
{
	ULONG dwRetVal = 0;

	WCHAR VolumeName[10] = L"c:";
	PWCHAR pVolumeName = &VolumeName[0];  // could use LPWSTR ?

	//ULONG FbwfCommitFile(PWCHAR volume,PWCHAR path );
		typedef ULONG (CALLBACK* mytry) (PWCHAR, PWCHAR );
		mytry NewFbwfCommitFile;    // Function pointer

	// free Library is causing an MFC exception error.  Load Library once when start, make a global
	//	HMODULE hLibNetwork = LoadLibrary( ("fbwflib.dll")); // file name of module
	//	HINSTANCE hLibNetwork = LoadLibrary( ("fbwflib.dll")); // file name of module

		if (vGlobalhLibFBWFNetwork != NULL)
		{
			LPCSTR lpProcName = "FbwfCommitFile";		// handle to DLL module // function name		 
			NewFbwfCommitFile = (mytry)GetProcAddress(vGlobalhLibFBWFNetwork,lpProcName);   

			if (NewFbwfCommitFile != NULL)
			{
				dwRetVal = NewFbwfCommitFile(pVolumeName, ptrFileName);

			}
		}		

	return(dwRetVal);
}

// return call succeed or falled
// function to set the cache size for the next FBWF after reboot
ULONG FBWF_Info::SetNextCacheFBWF(ULONG NextCacheInKBytes)
{
	ULONG dwRetVal = 0;
	ULONG NextCacheInMBytes;

	NextCacheInMBytes = NextCacheInKBytes / 1024L ;

	typedef ULONG (CALLBACK* mytry) (ULONG );
	mytry NewFbwfSetCacheThreshold;    // Function pointer

// free Library is causing an MFC exception error.  Load Library once when start, make a global
//	HMODULE hLibNetwork = LoadLibrary( ("fbwflib.dll")); // file name of module
//	HINSTANCE hLibNetwork = LoadLibrary( ("fbwflib.dll")); // file name of module

	if (vGlobalhLibFBWFNetwork != NULL)
	{
		LPCSTR lpProcName = "FbwfSetCacheThreshold";		// handle to DLL module // function name		 
		NewFbwfSetCacheThreshold = (mytry)GetProcAddress(vGlobalhLibFBWFNetwork,lpProcName);   

		if (NewFbwfSetCacheThreshold != NULL)
		{
			dwRetVal = NewFbwfSetCacheThreshold(NextCacheInMBytes);
		}
	}
				
	return(dwRetVal);
}


void FBWF_Info::UpdateDisplay()
{
	CString TempCString;
	CString TempCString2;
	CString TempCString3;
	CString TempCString4;
	unsigned long TempCurrentSession;
	unsigned long TempNextSession;

	unsigned long TempDirBytes;
	unsigned long TempFileBytes;
	unsigned long TempCacheBytes;
	unsigned long TempNextCacheBytes;

	ULONG TempReturnVal;

	// only update display if there is a change

	//check if enable state change in FBWF  //
	TempReturnVal = IsFBWFEnabled(&TempCurrentSession,&TempNextSession);

	// FBWF  Enabled, Next session Enabled
	if (TempReturnVal != NO_ERROR)		//read of enable/disable failed
	{
		if (vFbwfEnabledStatusReadok)
		{
			TempCString = "Overlay Status Read Error";
			SetDlgItemText(IDC_EnableDisableStateLabel,TempCString);
			vFbwfEnabledStatusReadok = false;
		}
	}
	else if (((TempCurrentSession != vFBWFCurrentEnabled) || (TempNextSession != vFBWFNextEnabled)))
	{
		TempCString2 = (TempCurrentSession ? "Enabled" : "Disabled")  ;
		TempCString3 = (TempNextSession ? "Enabled" : "Disabled")  ;

		vFBWFCurrentEnabled = TempCurrentSession;
		vFBWFNextEnabled = TempNextSession;

 		TempCString = "Protection " + TempCString2 + ", Next session " + TempCString3 ;
		SetDlgItemText(IDC_EnableDisableStateLabel,TempCString);
	}

//
	TempReturnVal = MemoryUsageFBWF(&TempDirBytes,&TempFileBytes,&TempCacheBytes,&TempNextCacheBytes);

	if (TempReturnVal != NO_ERROR)		//read of enable/disable failed
	{
		if (vFbwfStorageStatusReadok)
		{
			TempCString ="Failed Read";
			TempCString2 ="Failed Read";
			TempCString3 ="Failed Read";

			SetDlgItemText(IDC_AvailSpace,TempCString);
			SetDlgItemText(IDC_UsedSpace,TempCString2);
			SetDlgItemText(IDC_TotalSpace,TempCString3);
			vFbwfStorageStatusReadok = false;
		}
	}
	else
	{		
		if ((vTotalSpace != (TempCacheBytes/1024)) ||
		    (vUsedSpace  != ((TempDirBytes + TempFileBytes)/1024)))
		{
			//total ram space
			vTotalSpace = TempCacheBytes/1024;
			TempCString3.Format(_T("%ldMB"),(vTotalSpace + 512)/1024); // display in MB

			//used ram space
			vUsedSpace = (TempDirBytes + TempFileBytes)/1024; 
			TempCString2.Format(_T("%ldMB"),(vUsedSpace + 512)/1024);   // display in MB

			//available ram space
			TempCString.Format(_T("%ldMB"),((vTotalSpace - vUsedSpace)+ 512)/1024); // display in MB		

			SetDlgItemText(IDC_AvailSpace,TempCString);
			SetDlgItemText(IDC_UsedSpace,TempCString2);
			SetDlgItemText(IDC_TotalSpace,TempCString3);
		}
	}
}


void FBWF_Info::GetFBWFLogData(CString TempFileType)			//gets all files for today and compiles the strings from all files
{
	CString TempString1;
	CString	TempFileName;
	int TempLength;
	char TempChars[101];
	int LoopCount;

//	LPCTSTR cLogDirectory log file directory;
	
	TempString1 = cLogDirectory + TempFileType;
	TempFileName = TempString1.GetBuffer(TempString1.GetLength());

	CFileStatus TempFileStatus;

// NO_ERROR
	if (!(CFile::GetStatus(TempFileName, TempFileStatus)))
	{
		DWORD TempErrorNumber = GetLastError();
		vOutputTextString.Format(_T("Error reading FBWF Log Directory: %ld"),TempErrorNumber);
	}
	else
	{
		vOutputTextString ="FBWF files: \n";
		if (ThereIsEnoughMemory(sizeof(CFile), "FBWF Data Log File"))
		{
			CFile TempFBWFReportFile;
			CFileException TempException;

			if (TempFBWFReportFile)
			if (TempFBWFReportFile.Open( TempFileName, CFile::modeRead, &TempException))
			{
				do
				{
					TempLength = TempFBWFReportFile.Read(TempChars,100);
					//want only valid chars
					for(LoopCount=0; LoopCount<TempLength; LoopCount++)
						vOutputTextString += TempChars[LoopCount] ;
				}
				while (TempLength > 99);
			}
		}
		else
			vOutputTextString = "Memory too low for reading FBWF Log Directory";

//		CATCH(TempFileException)
//		{
//			ReportErrorMessage("FBWF Monitor Log output error", cEMailInspx, 32000);
//		}
	}

}

// call FBWF after checking if library is openned and FBWF is enabled
// return = 0 => no_error; return !=0 => then error message sent
ULONG CommitNowFBWF(CString cFilenamePlusDirectory, CString cFileName)
{
	WCHAR widecharFileName[constFileNameLength];
	PWCHAR ptrwFileName = &widecharFileName[0] ;

	WCHAR VolumeName[10] = L"c:";
	PWCHAR pVolumeName = &VolumeName[0];  // could use LPWSTR ?

	BOOL bFlagFileNameOk;
	int LoopCount;
	BOOL bReleaseLibrary = false;
	CString TempCString = "File not committed" ;
	DWORD dwRetVal = 1;		// intialize return to file not found
//	BOOL bLoadedLibraryHere = false;

	// if file exists, it is not a directory and the first char is not '.' then commit file
	if ((!cFileName.IsEmpty()) && (cFileName.GetAt(0)!='.') && (cFileName.GetAt(0)!='~'))
	{
		//ULONG FbwfCommitFile(PWCHAR volume,PWCHAR path );
		// initialize for function call
		typedef ULONG (CALLBACK* mytry) (PWCHAR, PWCHAR );
		mytry NewFbwfCommitFile;    // Function pointer

		// add filename and directory
		bFlagFileNameOk = true;
		for (LoopCount=0; LoopCount < ((cFilenamePlusDirectory.GetLength())-2); LoopCount++)
		{
			widecharFileName[LoopCount] = cFilenamePlusDirectory.GetAt(LoopCount+2);	//skip c:
			if (LoopCount>(constFileNameLength-3))
			{	//error condition
				ReportErrorMessage("FBWF-Dir path + Filename too long",cEMailInspx,32000);
				TempCString = "FBWF-Dir path + Filename too long";
				bFlagFileNameOk = false;
				break;
			}
		}
		widecharFileName[LoopCount] = 0;
		widecharFileName[LoopCount+1] = 0;
		if (bFlagFileNameOk) 
		{	// valid file name, check Library active not active 
			
//			if (vGlobalhLibFBWFNetwork == NULL)
//			{
//				vGlobalhLibFBWFNetwork = LoadLibrary("fbwflib.dll"); // file name of module
//				if (vGlobalhLibFBWFNetwork != NULL) // tried to load library, is successful then need to free library after function call
//				{
//					bLoadedLibraryHere = true;
//				}
//			}
			// if library is loaded either above or elsewhere
			if (vGlobalhLibFBWFNetwork != NULL)
			{
				LPCSTR lpProcName = "FbwfCommitFile";		// handle to DLL module // function name		 
				NewFbwfCommitFile = (mytry)GetProcAddress(vGlobalhLibFBWFNetwork,lpProcName);   

				if (NewFbwfCommitFile != NULL)
				{
					dwRetVal = NewFbwfCommitFile(pVolumeName, ptrwFileName);
				}
			}
//			if (bLoadedLibraryHere)
//			{
//				FreeLibrary(vGlobalhLibFBWFNetwork);
//				vGlobalhLibFBWFNetwork = NULL ;
//			}
		}
	}
	return(dwRetVal);
}
void FBWF_Info::ShowFunctionNotValidDialogMessage() 
{
	CNoticeDialog TempNoticeDialog;
	CString TempText("");
	TempText = ("Currently Function Not Valid");
	TempNoticeDialog.vNoticeText = "\n\n\nCurrently Function Not Valid";
	TempNoticeDialog.vType = cInformationMessage;
	TempNoticeDialog.DoModal();
}


// FBWF getactualsize start, function does not seem to be in library	
//	typedef struct _FbwfFSInfo
//	{
//	      LARGE_INTEGER TotalAllocationUnits;
//	      LARGE_INTEGER AvailableAllocationUnits;
//				ULONG SectorsPerAllocationUnit;
//				ULONG BytesPerSector;
//	} FbwfFSInfo, *PFbwfFSInfo;

//	TotalAllocationUnits 
//	Total number of allocation units on the volume that are available to the user associated with the calling thread.
//
//	If per-user quotas are in use, this value may be less than the total number of allocation units on the disk.
//
//	AvailableAllocationUnits 
//	Total number of free allocation units on the volume that are available to the user associated with the calling thread.
//
//	If per-user quotas are in use, this value may be less than the total number of free allocation units on the disk.
//
//	SectorsPerAllocationUnit 
//	Number of sectors in each allocation unit.
//
//	BytesPerSector 
//	Number of bytes in each sector.
//
//	Requirements
//	--------------------------------------------------------------------------------
//	OS Versions: Windows XP Embedded.
//	Header: fbwfapi.h.
//	Link Library: fbwflib.lib.
	
		
//	FbwfFSInfo FSInfoSave;
//	FbwfFSInfo * ptrFSInfoSave = &FSInfoSave ;

//	typedef ULONG (CALLBACK* mytry) ( PWSTR, FbwfFSInfo * );
//	mytry NewFbwfGetActualSize;    // Function pointer

//	if (vGlobalhLibFBWFNetwork == NULL)
//	{
//		vGlobalhLibFBWFNetwork = LoadLibrary("fbwflib.dll"); // file name of module
//		if (vGlobalhLibFBWFNetwork != NULL) // tried to load library, is successful then need to free library after function call
//		{
////			bLoadedLibraryHere = true;
//		}
//	}

//	cTextString = "Fail ";

//	FSInfoSave.TotalAllocationUnits.LowPart = 5;
//	FSInfoSave.TotalAllocationUnits.HighPart = 0;
//	FSInfoSave.AvailableAllocationUnits.LowPart = 6;
//	FSInfoSave.AvailableAllocationUnits.HighPart = 0;
//	FSInfoSave.SectorsPerAllocationUnit = 7;
//	FSInfoSave.BytesPerSector = 8;
//
//	if (vGlobalhLibFBWFNetwork != NULL)
//	{
//
//		LPCSTR lpProcName = "FbwfGetActualSize";		// handle to DLL module // function name		 
//		NewFbwfGetActualSize = (mytry)GetProcAddress(vGlobalhLibFBWFNetwork,lpProcName);   
//
//		ULONG ulRetVal = 5;
//
//		WCHAR VolumeName[10] = L"c:";
//		PWSTR pVolumeName = &VolumeName[0];  // could use LPWSTR ?
//
//		if (NewFbwfGetActualSize != NULL)
//		{
//			ulRetVal = NewFbwfGetActualSize(VolumeName ,ptrFSInfoSave);
//			if (ulRetVal == NO_ERROR)
//				cTextString = "Pass ";
//		}
//
//	}	
//
//	long ApartI = FSInfoSave.TotalAllocationUnits.LowPart ;
//	long ApartII = (long)FSInfoSave.TotalAllocationUnits.HighPart;
//	
//	long BpartI = FSInfoSave.AvailableAllocationUnits.LowPart ;
//	long BpartII = (long)FSInfoSave.AvailableAllocationUnits.HighPart;
//
//	//	  FSInfoSave.TotalAllocationUnits + FSInfoSave.AvailableAllocationUnits + FSInfoSave.SectorsPerAllocationUnit + FSInfoSave.BytesPerSector);
//	CString cNumbersString;
//	cNumbersString.Format(" 0x%lx%lx  0x%lx%lx  0x%lx  0x%lx",ApartII,ApartI,BpartII,BpartI,FSInfoSave.SectorsPerAllocationUnit,FSInfoSave.BytesPerSector ); 
//
//	cTextString += cNumbersString;
//	SetDlgItemText(IDC_MoreDiskDriveInfo, cTextString);
//	FBWF getactualsize end 
//
// **********alternate directory size method************
//DRIVE_LAYOUT_INFORMATION
/*
BOOL DeviceIoControl(
  (HANDLE) hDevice,            // handle to device
  IOCTL_DISK_GET_DRIVE_LAYOUT, // dwIoControlCode operation
  NULL,                        // lpInBuffer; must be NULL
  0,                           // nInBufferSize; must be zero
  (LPVOID) lpOutBuffer,        // output buffer
  (DWORD) nOutBufferSize,      // size of output buffer
  (LPDWORD) lpBytesReturned,   // number of bytes returned
  (LPOVERLAPPED) lpOverlapped  // OVERLAPPED structure
);
*/
/* reference
http://us.generation-nt.com/answer/fail-get-drive-information-deviceiocontrol-help-24864692.html
*/	
/*
//below works good 
	HANDLE hdevice;


	hdevice=CreateFile("\\\\.\\C:",												// handle to c:
			GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ |
			FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	DWORD buffersize = 128;
	DWORD bytesreturned;

//	DRIVE_LAYOUT_INFORMATION	InfoLayout = malloc( buffersize );
	DRIVE_LAYOUT_INFORMATION *drivelayout = (DRIVE_LAYOUT_INFORMATION*) malloc( buffersize );

	BOOL LoopOK = true;
	DWORD size;

	if ((hdevice != 0) && (hdevice != INVALID_HANDLE_VALUE)) // ( && (hdevice != 0))
	{
//		BOOL bresult=DeviceIoControl(hdevice,IOCTL_DISK_GET_DRIVE_LAYOUT, NULL, 0, drivelayout, sizeof(drivelayout), &size, NULL);

		DWORD bytesreturned = 0;

		// loop til we get the buffer size right 
		while( (LoopOK == true) && (!DeviceIoControl( hdevice, IOCTL_DISK_GET_DRIVE_LAYOUT,NULL, 0, drivelayout, buffersize,	&bytesreturned, 0 )) )
		{
			if ( GetLastError() != ERROR_INSUFFICIENT_BUFFER )
			{
			// uh-oh ... some other error 
				LoopOK = false;
				free( drivelayout );
			}
			else
			{
				buffersize += 4*sizeof( PARTITION_INFORMATION );
				free( drivelayout );
				drivelayout = (DRIVE_LAYOUT_INFORMATION*) malloc( buffersize );
			}
		}
	}

	CString cNumbersString;
	if ((LoopOK == true) && (hdevice != INVALID_HANDLE_VALUE) && (hdevice != NULL))
	{
		cTextString = "Pass ";
		LARGE_INTEGER PartLength;
// initial output

		PartLength = drivelayout->PartitionEntry[0].PartitionLength;
		long ApartI = PartLength.LowPart ;
		long ApartII = (long)PartLength.HighPart;
		PartLength = drivelayout->PartitionEntry[1].PartitionLength;
		long BpartI = PartLength.LowPart ;
		long BpartII = (long)PartLength.HighPart;
		PartLength = drivelayout->PartitionEntry[2].PartitionLength;
		long CpartI = PartLength.LowPart ;
		long CpartII = (long)PartLength.HighPart;
		PartLength = drivelayout->PartitionEntry[3].PartitionLength;
		long DpartI = PartLength.LowPart ;
		long DpartII = (long)PartLength.HighPart;
		cNumbersString.Format(" 0x%lx  0x%lx 0x%lx%lx; 0x%lx 0x%lx%lx",
																																		drivelayout->PartitionCount,
																																		drivelayout->PartitionEntry[0].PartitionNumber,ApartII,ApartI,
																																		drivelayout->PartitionEntry[1].PartitionNumber,BpartII,BpartI ); 
// // end initial output

		PartLength = drivelayout->PartitionEntry[0].PartitionLength;
		LONGLONG ValueDiskSize64 = 0;
		ValueDiskSize64 = (long)PartLength.HighPart;
		ValueDiskSize64 <<= 32;		//shift ms part over
		ValueDiskSize64 += PartLength.LowPart ;

		double ValueDiskSize = (double)ValueDiskSize64;

		if (ValueDiskSize >= 0x40000000)
			cNumbersString = "Drive C:\\ " + (dtoaWithCommas(ValueDiskSize / 0x40000000, 0) + "GB Total");
		else
		if (TempFBWFHardDriveSpaceFree >= 0x100000)
			cNumbersString = "Drive C:\\ " + (dtoaWithCommas(ValueDiskSize / 0x100000, 0) + "MB Total");
		else
			cNumbersString = "Drive C:\\ " + (dtoaWithCommas(ValueDiskSize, 0) + " Bytes Total");

		cTextString += cNumbersString;
	}
		// close c:
	free(drivelayout);
	CloseHandle(hdevice);
*/
/* notes for MemoryUsageFBWF
currentCacheThreshold
Cache threshold in bytes for the current session.
nextCacheThreshold
Cache threshold in bytes for the next session.
dirStructure
Memory in bytes used to store directory structure.
fileData
Memory in bytes used to cache file data..


	unsigned long TempDirBytes;
	unsigned long TempFileBytes;
	unsigned long TempCacheBytes;
	unsigned long TempNextCacheBytes;

	TempReturnVal = MemoryUsageFBWF(&TempDirBytes,&TempFileBytes,&TempCacheBytes,&TempNextCacheBytes);
			vTotalSpace = ((TempCacheBytes/1024)+512)/1024;		// in MB
			vUsedSpace = (((TempDirBytes + TempFileBytes)/1024)+512)/1024; // in MB 
			vAvailableSpace = vTotalSpace - vUsedSpace ;// in MB

			*/

void FBWF_Info::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
