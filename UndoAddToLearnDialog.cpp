//ScanTrac Side View Source File
// UndoAddToLearnDialog.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "UndoAddToLearnDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BYTE vGlobalShiftKeyDown;
extern CProduct *vGlobalCurrentProduct;
extern unsigned long vGlobalDialogTitleColor;
extern int vGlobalPasswordLevel;
/////////////////////////////////////////////////////////////////////////////
// CUndoAddToLearnDialog dialog


CUndoAddToLearnDialog::CUndoAddToLearnDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CUndoAddToLearnDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUndoAddToLearnDialog)
	//}}AFX_DATA_INIT
	vLocalConfigurationData = NULL;
	vLocalSystemData = NULL;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_StatusDisplay);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
}


void CUndoAddToLearnDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUndoAddToLearnDialog)
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_StatusDisplay, m_StatusDisplay);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
}


BEGIN_MESSAGE_MAP(CUndoAddToLearnDialog, CDialog)
	//{{AFX_MSG_MAP(CUndoAddToLearnDialog)
	ON_WM_SHOWWINDOW()
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
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CUndoAddToLearnDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUndoAddToLearnDialog message handlers

void CUndoAddToLearnDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

	UpdateButtons();

	this->SetWindowText(_T("Un-Do Learn"));
	::SetupMenu(vLocalCWndCollection);
}

void CUndoAddToLearnDialog::OnFunction1Button() 
{
	// User clicked Main Menu Button to exit 
	CDialog::EndDialog(10);
}

void CUndoAddToLearnDialog::OnFunction2Button() 
{
	// User clicked exit 
	CDialog::EndDialog(1);
}

void CUndoAddToLearnDialog::OnFunction3Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CUndoAddToLearnDialog::OnFunction4Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CUndoAddToLearnDialog::OnFunction5Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CUndoAddToLearnDialog::OnSubFunction1Button() 
{
	//revert to original Learn Permanently
	if ((vLocalSystemData->vITIPCDig->HaveLearnImageFile("=Process 2o")) &&
		(vGlobalCurrentProduct) && (vGlobalCurrentProduct->vAddToLearnCount))
	{
		vLocalSystemData->vITIPCDig->RestoreOriginalStructurePermanently();
		vLocalSystemData->vITIPCDig->vUsingOriginalLearnTemporarily = false;
		if (vLocalConfigurationData)
		if (vLocalConfigurationData->vAutoImproveEnabled)
		{
			vLocalSystemData->vITIPCDig->ClearImproveLearn(false);//clear AutoImprove Learn so does not over write
		}

		UpdateButtons();

		CNoticeDialog TempNoticeDialog;
		CString TempString = "\n\n\nReverteded Original Learn Permanently.";
		//TempString.LoadString(IDS_RejectAddedToLearn);//"\n\n\nReject Added To Learn."
		TempNoticeDialog.vNoticeText = TempString;
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.vAutoClose = 3000;
		TempNoticeDialog.DoModal();
	}
}

void CUndoAddToLearnDialog::OnSubFunction2Button() 
{
	//revert to original Learn Temporarily
	if ((vLocalSystemData->vITIPCDig->HaveLearnImageFile("=Process 2o")) &&
		(vGlobalCurrentProduct) && (vGlobalCurrentProduct->vAddToLearnCount))
	if (!vLocalSystemData->vITIPCDig->vUsingOriginalLearnTemporarily)
	{
		vLocalSystemData->vITIPCDig->RestoreOriginalStructureTemporarily();
		UpdateButtons();

		CNoticeDialog TempNoticeDialog;
		CString TempString = "\n\n\nReverteded Original Learn Temporarily.";
		//TempString.LoadString(IDS_RejectAddedToLearn);//"\n\n\nReject Added To Learn."
		TempNoticeDialog.vNoticeText = TempString;
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.vAutoClose = 3000;
		TempNoticeDialog.DoModal();
	}
}

void CUndoAddToLearnDialog::OnSubFunction3Button() 
{
	
	//revert to original Learn Temporarily
	if (vLocalSystemData->vITIPCDig->HaveLearnImageFile("=Process 2o"))
	if (vLocalSystemData->vITIPCDig->vUsingOriginalLearnTemporarily)
	{
		vLocalSystemData->vITIPCDig->RestoreToCurrentStructure();
		UpdateButtons();

		CNoticeDialog TempNoticeDialog;
		CString TempString = "\n\n\nRestored Current Learn.";
		//TempString.LoadString(IDS_RejectAddedToLearn);//"\n\n\nReject Added To Learn."
		TempNoticeDialog.vNoticeText = TempString;
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.vAutoClose = 3000;
		TempNoticeDialog.DoModal();
	}
}

void CUndoAddToLearnDialog::OnSubFunction4Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CUndoAddToLearnDialog::OnSubFunction5Button() 
{
	//undo last add to learn
	if (vLocalSystemData->vITIPCDig->HaveUndoAddToLearnImage())
	{
		vLocalSystemData->vITIPCDig->UndoLastAddToLearn();
		UpdateButtons();

		CNoticeDialog TempNoticeDialog;
		CString TempString = "\n\n\nLast Add To Learn Removed.";
		//TempString.LoadString(IDS_RejectAddedToLearn);//"\n\n\nReject Added To Learn."
		TempNoticeDialog.vNoticeText = TempString;
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.vAutoClose = 3000;
		TempNoticeDialog.DoModal();
	}
}

void CUndoAddToLearnDialog::OnSubFunction6Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CUndoAddToLearnDialog::OnSubFunction7Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CUndoAddToLearnDialog::OnSubFunction8Button() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CUndoAddToLearnDialog::PreTranslateMessage(MSG* pMsg) 
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
		CWnd *TempWindow;
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
			case 111: //Sub Function 5:
				OnFunction2Button();
			break;
			case 106: //Sub Function 6:
				OnFunction4Button();
			break;
			case 109: //Sub Function 7
				OnFunction3Button();
			break;
			case 107: //Sub Function 8:
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
					cShowHelp, 0, 0);
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
			break;
			case cPageUp: // Clear, but do inspx hidden dialog
				if (vGlobalPasswordLevel)
				{
					PasswordOK(cNoPassword, false);
				}
				else
				{
					PasswordOK(cAnyPasswordToEnter, true);
				}
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

void CUndoAddToLearnDialog::UpdateButtons()
{
	CString TempText = "Product: " + *vGlobalCurrentProduct->GetProductName();

	if (vLocalSystemData->vITIPCDig->vUsingOriginalLearnTemporarily)
		TempText = TempText + "\nUsing Original Learn Temporarily.";
	if (vGlobalCurrentProduct)
		TempText = TempText + "\n" + dtoa(vGlobalCurrentProduct->vAddToLearnCount,0) + 
		" Rejects Added To Learn.";
	if (!vLocalSystemData->vITIPCDig->HaveLearnImageFile("=Process 2o"))
		TempText = TempText + "\nNo Original Learn to Restore to for this product.";

	SetDlgItemText(IDC_StatusDisplay,TempText);

	if ((vLocalSystemData->vITIPCDig->HaveLearnImageFile("=Process 2o")) &&
		(vGlobalCurrentProduct) && (vGlobalCurrentProduct->vAddToLearnCount))
	{
		m_SubFunction1Button.ShowWindow(SW_SHOW);
	}
	else
	{
		m_SubFunction1Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
	}
	if ((vLocalSystemData->vITIPCDig->HaveLearnImageFile("=Process 2o")) &&
		(vGlobalCurrentProduct) && (vGlobalCurrentProduct->vAddToLearnCount))
	{
		if (vLocalSystemData->vITIPCDig->vUsingOriginalLearnTemporarily)
		{
			m_SubFunction2Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
			m_SubFunction3Button.ShowWindow(SW_SHOW);
			this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
		}
		else
		{
			m_SubFunction2Button.ShowWindow(SW_SHOW);
			m_SubFunction3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
		}
	}
	else
	{
		m_SubFunction2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
		m_SubFunction3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
	}
		
	if (vLocalSystemData->vITIPCDig->HaveUndoAddToLearnImage())
	{
		m_SubFunction5Button.ShowWindow(SW_SHOW);
	}
	else
	{
		m_SubFunction5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
	}

}


HBRUSH CUndoAddToLearnDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
  if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
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
	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_StatusDisplay)
	{
		TextSize = SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_StatusDisplay, 5);	//todo: multiline
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
  }
	
	return hbr;
}


void CUndoAddToLearnDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
