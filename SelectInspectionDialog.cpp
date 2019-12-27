//ScanTrac Side View Source File
// SelectInspectionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "SelectInspectionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern double vGlobalPercentToResizeY;
extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
/////////////////////////////////////////////////////////////////////////////
// CSelectInspectionDialog dialog


CSelectInspectionDialog::CSelectInspectionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectInspectionDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectInspectionDialog)
	//}}AFX_DATA_INIT
	//vNumberOfInspections = 0;
	vInspectionNumberSelected = 0;
	vCurrentFirstInspectionNumberShown = 1;
	SelectWindowForStyle = cSelectInspection;
	vLightGreenBrush = ::CreateSolidBrush(cLightGreen);
	vOneSecondTimerHandle = 55;
	vOldXRayOnNotice = false;
	vMouseDownRepeatTimerHandle = 3001;
	vMouseState = 0;		//1 is left mouse button clicked down, 2 is left mouse button transistioned up, 3 is double clicked left mouse button
	vRecievedButtonPress = false;
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_XRaysOn);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_ProductLabel);

	vLocalDisplayCollection.Add(&m_SubFunction8Button);
	vLocalDisplayCollection.Add(&m_SubFunction7Button);
	vLocalDisplayCollection.Add(&m_SubFunction6Button);
	vLocalDisplayCollection.Add(&m_SubFunction5Button);
	vLocalDisplayCollection.Add(&m_SubFunction4Button);
	vLocalDisplayCollection.Add(&m_SubFunction3Button);
	vLocalDisplayCollection.Add(&m_SubFunction2Button);
	vLocalDisplayCollection.Add(&m_SubFunction1Button);
}

CSelectInspectionDialog::~CSelectInspectionDialog()
{
	DeleteObject(vLightGreenBrush);
}


void CSelectInspectionDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	::SetupMenu(vLocalCWndCollection);
	
	if (vProductEditing->vNumberOfInspections == 1)
		SelectAndExit(1);
	
	CString TempText;
	CString TempText1;
	TempText.LoadString(IDS_XRAYON);
	SetDlgItemText(IDC_XRaysOn,TempText);

	UpdateInspectionDisplay();
	switch (SelectWindowForStyle)
	{												
		case cSelectInspectionForEdit:
			TempText.LoadString(IDS_SelectInspectionToEdit);
		break;
		case cSelectInspectionForCopy:
			TempText.LoadString(IDS_SelectInspectionToCopy);
		break;
		case cSelectInspectionForDelete:
			TempText.LoadString(IDS_SelectInspectionToDelete);
		break;
		case cSelectInspectionForDisplay:
			TempText.LoadString(IDS_SelectInspectionToDisplay);
		break;
		case cSelectInspectionForEvaluate:
			TempText.LoadString(IDS_SelectInspectionToEvaluate);
		break;
		case cSelectInspectionForLastPriority:
			TempText.LoadString(IDS_SelectInspectionToMakeLastPriority);
		break;
	}
	SetDlgItemText(IDC_DialogTitleStaticText1,TempText);

	TempText.LoadString(IDS_CurrentProduct);
	CString vTempString1 = TempText + ": ";
	CString vTempString3(*vProductEditing->GetProductName());
	if (vTempString3 == "")
		vTempString3 = vTempString1 + "Blank String 1";
	else
		vTempString3 = vTempString1 + vTempString3;
	//copy product name text into Label control on screen 
	SetDlgItemText(IDC_ProductLabel,vTempString3);

		TempText.LoadString(IDS_MainMenu);
	SetDlgItemText(IDC_Function1Button,TempText);
	TempText.LoadString(IDS_Cancel);
	SetDlgItemText(IDC_Function4Button,TempText);
	this->SetWindowText(_T("Select Inspection"));
	OneSecondUpdate();
	int TimerResult = SetTimer(vOneSecondTimerHandle,1000,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
}

void CSelectInspectionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectInspectionDialog)
	DDX_Control(pDX, IDC_XRaysOn, m_XRaysOn);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_ProductLabel, m_ProductLabel);
}


BEGIN_MESSAGE_MAP(CSelectInspectionDialog, CDialog)
	//{{AFX_MSG_MAP(CSelectInspectionDialog)
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
	ON_WM_DELETEITEM()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CSelectInspectionDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectInspectionDialog message handlers

void CSelectInspectionDialog::OnFunction1Button() 
{
	// User clicked Main Menu Button to exit without saving
	//CDialog::EndDialog(10);
}

void CSelectInspectionDialog::OnFunction2Button() 
{
}

void CSelectInspectionDialog::OnFunction3Button() 
{
	//Previous Group button was pressed
  if (vCurrentFirstInspectionNumberShown > 1)
  {
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down Select Inspection IN FUNCTION 3",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState == 3) || (vMouseState == 4)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 1;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		
		if (vCurrentFirstInspectionNumberShown > 8)
      vCurrentFirstInspectionNumberShown = vCurrentFirstInspectionNumberShown - 8;
		else
			vCurrentFirstInspectionNumberShown = 1;
		UpdateInspectionDisplay();
  }	
}

void CSelectInspectionDialog::OnFunction4Button() 
{
	//Cancel button pressed
	CDialog::OnCancel();
}

void CSelectInspectionDialog::OnFunction5Button() 
{
	//next button pressed to display next group of Inspections
	if (vProductEditing->vNumberOfInspections > 8)
	if (vCurrentFirstInspectionNumberShown + 7 < vProductEditing->vNumberOfInspections)
	{
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down SelectInspection IN FUNCTION 5",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState == 3) || (vMouseState == 4)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 2;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		//else
			vCurrentFirstInspectionNumberShown = vCurrentFirstInspectionNumberShown + 8;
		UpdateInspectionDisplay();
	}
}

void CSelectInspectionDialog::OnSubFunction1Button() 
{
	SelectAndExit(1);
}

void CSelectInspectionDialog::OnSubFunction2Button() 
{
	SelectAndExit(2);
}

void CSelectInspectionDialog::OnSubFunction3Button() 
{
	SelectAndExit(3);
}

void CSelectInspectionDialog::OnSubFunction4Button() 
{
	SelectAndExit(4);
}

void CSelectInspectionDialog::OnSubFunction5Button() 
{
	SelectAndExit(5);
}

void CSelectInspectionDialog::OnSubFunction6Button() 
{
	SelectAndExit(6);
}

void CSelectInspectionDialog::OnSubFunction7Button() 
{
	SelectAndExit(7);
}

void CSelectInspectionDialog::OnSubFunction8Button() 
{
	SelectAndExit(8);
}

BOOL CSelectInspectionDialog::PreTranslateMessage(MSG* pMsg) 
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
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (vMouseState > 0)
		{
			//ReportErrorMessage("Screen Clicked, repeat ended",cWriteToLog, 0);
			KillTimer(vMouseDownRepeatTimerHandle);
			vMouseState = 0;
		}
	}
	else
	if (pMsg->message == WM_LBUTTONUP)
	{
		//ReportErrorMessage("Left Button Up",cAction, 32000);
		if (vMouseState < 3)
		{
			if (vMouseState > 0)
			{
				KillTimer(vMouseDownRepeatTimerHandle);
				vMouseState = 0;
			}
		}
	}
	if (pMsg->message == WM_KEYDOWN)
	{
		int TempKey = pMsg->wParam;
		vRecievedButtonPress = true;
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
				PostThreadMessage(vGlobalCallingThreadID,
					cShowHelp, 59, 0);
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
		vRecievedButtonPress = false;
		return true;  //true indicates it has been handled, so do not process
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CSelectInspectionDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	
	// TODO: Change any attributes of the DC here
  if (pWnd->GetDlgCtrlID() == IDC_ProductLabel)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_ProductLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLightGreenBrush;
		//return vGreenBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_XRaysOn)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaysOn, 7);  //5 is medium large
		pDC->SetTextColor(cYellow);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkColor(cRed);
		return vLocalSystemData->vRedBrush;
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
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}
  if (pWnd->GetDlgCtrlID() == IDC_Background) 
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLightGreenBrush;
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CSelectInspectionDialog::SelectAndExit(BYTE TempInspectionNumber) 
{
	if ((TempInspectionNumber > 0) &&
		(vCurrentFirstInspectionNumberShown - 1 + TempInspectionNumber <= 
		vProductEditing->vNumberOfInspections))
	{
		//figure out what product number is selected by the button press
		vInspectionNumberSelected = vCurrentFirstInspectionNumberShown - 1 + TempInspectionNumber;

		CDialog::EndDialog(true);
	}
}

void CSelectInspectionDialog::UpdateInspectionDisplay()
{
	CWnd* pWnd;
	CDC* pDC;
	HWND TempDialogHwnd = this->m_hWnd;

	CString TempInspectionName;
	CString TempInspectionNameString("");
	CString TempBlankName(" ");
	CString TempText("");
	WORD TempLength = 0;
	for (BYTE TempLoop = 1; TempLoop < 9;  TempLoop++)
	{
		if (vCurrentFirstInspectionNumberShown + TempLoop - 2 < vProductEditing->vNumberOfInspections)
		{
			if ((vCurrentFirstInspectionNumberShown + TempLoop - 2) < 
				vProductEditing->vNumberOfInspections)
			{
				TempInspectionName = vProductEditing->vInspection
					[vCurrentFirstInspectionNumberShown + TempLoop - 2]->vName;
				if (!vProductEditing->vInspection
					[vCurrentFirstInspectionNumberShown + TempLoop - 2]->vEnable)
					TempInspectionName = TempInspectionName + "-Disabled";
			}

			if (TempInspectionName.IsEmpty())
				TempInspectionNameString = "Blank Name 2";
			else
			{
				if (TempLoop > 4)
					TempInspectionNameString = TempInspectionName + TempBlankName;
				else
					TempInspectionNameString =  TempBlankName + TempInspectionName;
			}
			TempLength = TempInspectionNameString.GetLength();
		}
		else
			TempLength = 0;

		switch (TempLoop)
		{
			case 1:
				if (!TempLength)
				{
					m_SubFunction1Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction1Button.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction1Button,TempInspectionNameString);

					pDC = m_SubFunction1Button.GetDC();
					pWnd = &m_SubFunction1Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5) + 30;  //5 is medium large
					m_SubFunction1Button.SetWindowPos(NULL, 11, 52, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
			case 2:
				if (!TempLength)
				{
					m_SubFunction2Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction2Button.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction2Button,TempInspectionNameString);

					pDC = m_SubFunction2Button.GetDC();
					pWnd = &m_SubFunction2Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5) + 30;  //5 is medium large
					m_SubFunction2Button.SetWindowPos(NULL, 11, 142, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
			case 3:
				if (!TempLength)
				{
					m_SubFunction3Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction3Button.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction3Button,TempInspectionNameString);

					pDC = m_SubFunction3Button.GetDC();
					pWnd = &m_SubFunction3Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5) + 30;  //5 is medium large
					m_SubFunction3Button.SetWindowPos(NULL, 11, 232, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
			case 4:
				if (!TempLength)
				{
					m_SubFunction4Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction4Button.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction4Button,TempInspectionNameString);

					pDC = m_SubFunction4Button.GetDC();
					pWnd = &m_SubFunction4Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5) + 30;  //5 is medium large
					m_SubFunction4Button.SetWindowPos(NULL, 11, 327, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
			case 5:
				if (!TempLength)
				{
					m_SubFunction5Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction5Button.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction5Button,TempInspectionNameString);

					pDC = m_SubFunction5Button.GetDC();
					pWnd = &m_SubFunction5Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5) + 30;  //5 is medium large
					m_SubFunction5Button.SetWindowPos(NULL, 623 - (int)(TempStringLength / vGlobalPercentToResizeY), 79, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
			case 6:
				if (!TempLength)
				{
					m_SubFunction6Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction6Button.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction6Button,TempInspectionNameString);

					pDC = m_SubFunction6Button.GetDC();
					pWnd = &m_SubFunction6Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5) + 30;  //5 is medium large
					m_SubFunction6Button.SetWindowPos(NULL, 623 - (int)(TempStringLength / vGlobalPercentToResizeY), 169, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
			case 7:
				if (!TempLength)
				{
					m_SubFunction7Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction7Button.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction7Button,TempInspectionNameString);

					pDC = m_SubFunction7Button.GetDC();
					pWnd = &m_SubFunction7Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5) + 30;  //5 is medium large
					m_SubFunction7Button.SetWindowPos(NULL, 623 - (int)(TempStringLength / vGlobalPercentToResizeY), 259, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
			case 8:
				if (!TempLength)
				{
					m_SubFunction8Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
				}
				else
				{
					m_SubFunction8Button.ShowWindow(SW_SHOW);
					SetDlgItemText(IDC_SubFunction8Button,TempInspectionNameString);

					pDC = m_SubFunction8Button.GetDC();
					pWnd = &m_SubFunction8Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5) + 30;  //5 is medium large
					m_SubFunction8Button.SetWindowPos(NULL, 623 - (int)(TempStringLength / vGlobalPercentToResizeY), 354, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
				}
			break;
		}
	}
	if (vProductEditing->vNumberOfInspections > vCurrentFirstInspectionNumberShown + 7)
	{
		m_Function5Button.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_Next);
		SetDlgItemText(IDC_Function5Button,TempText);
	}
	else
	{
		m_Function5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
		SetDlgItemText(IDC_Function5Button, _T(""));
	}

	if (vCurrentFirstInspectionNumberShown > 1)
	{
		m_Function3Button.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_Previous);
		SetDlgItemText(IDC_Function3Button,TempText);
	}
	else
	{
		m_Function3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
		SetDlgItemText(IDC_Function3Button, _T(""));
	}
	::SetupMenu(vLocalDisplayCollection);
}

void CSelectInspectionDialog::OneSecondUpdate() 
{
	if (vOldXRayOnNotice != vLocalSystemData->vXRayOnNotice)
	{
		vOldXRayOnNotice = vLocalSystemData->vXRayOnNotice;
		if (vOldXRayOnNotice)
			m_XRaysOn.ShowWindow(SW_SHOW);
		else
		{
			m_XRaysOn.ShowWindow(SW_HIDE);
			//vStartingUpSource = false;
		}
	}
}

void CSelectInspectionDialog::OnTimer(UINT nIDEvent) 
{
	 if (nIDEvent == vOneSecondTimerHandle)
  {
		OneSecondUpdate();
	}
	CDialog::OnTimer(nIDEvent);
}

void CSelectInspectionDialog::OnDialogtitlestatictext1()
{
	//bring up soft key menu for help, print screen, Image, Clear, and hidden shift key
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
