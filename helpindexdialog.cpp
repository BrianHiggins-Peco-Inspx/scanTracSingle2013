//ScanTrac Side View Source File
// HelpIndexDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "HelpIndexDialog.h"
#include "HelpDialog.h"
#include "NoticeDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BYTE vGlobalLanguage;
extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
extern CScanTracSystemRunningData *vGlobalRunningData;
/////////////////////////////////////////////////////////////////////////////
// CHelpIndexDialog dialog


CHelpIndexDialog::CHelpIndexDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CHelpIndexDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHelpIndexDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	vWhiteBrush = ::CreateSolidBrush(cWhite);
	vBlueBrush = ::CreateSolidBrush(cBlue);
	vLightGrayBrush = ::CreateSolidBrush(cLightGray);
	vCurrentFirstNumberShown = 1;
	vMouseState = 0;		//1 is left mouse button clicked down, 2 is left mouse button transistioned up, 3 is double clicked left mouse button
	vRecievedButtonPress = false;

	//the value of each item is the index of the following array
	//sort order         //HelpTitlexx number
	vHelpOrderIndex [0] = 0;		//About ScanTrac
	vHelpOrderIndex [1] = 1;		//Main Menu
	vHelpOrderIndex [2] = 2;		//Select Current Product
	vHelpOrderIndex [3] = 3;		//Setup
	vHelpOrderIndex [4] = 49;		//Setup Product Summary
	vHelpOrderIndex [5] = 4;		//Setup Product
	vHelpOrderIndex [6] = 5;		//Setup Product - Container Trigger
	vHelpOrderIndex [7] = 33;		//Setup Product - Auto Setup Size
	vHelpOrderIndex [8] = 54;	  //Setup Product - Auto Setup Size - Image Settings
	vHelpOrderIndex [9] = 34;		//Setup Product - Auto Setup Source
	vHelpOrderIndex [10] = 35;		//Setup Product - Auto Setup Image Learn
	vHelpOrderIndex [11] = 37;	//Setup Product - Auto Setup ROI
	vHelpOrderIndex [12] = 36;	//Setup Product - Auto Setup Contaminant Threshold
	vHelpOrderIndex [13] = 47;	//Setup Product - Auto Setup Void Threshold
	vHelpOrderIndex [14] = 55;	//Setup Product - Auto Setup ContaminantThreshold - Inspection Settings
	vHelpOrderIndex [15] = 7;		//Setup Product - Ejector
	vHelpOrderIndex [16] = 8;		//Setup Product - Auxiliary Detector
	vHelpOrderIndex [17] = 11;	//Setup Product - X-Ray Image Other Reject Criteria
	vHelpOrderIndex [18] = 6;		//Setup Product - Edit Product
	vHelpOrderIndex [19] = 30;	//Setup Product - Contaminant Bounds
	vHelpOrderIndex [20] = 39;	//Setup Product - Contaminant Image Learn
	vHelpOrderIndex [21] = 40;	//Setup Product - Image Learning Reference
	vHelpOrderIndex [22] = 9;		//Setup Product - Region of Interest
	vHelpOrderIndex [23] = 10;	//Setup Product - X-Ray Detector
	vHelpOrderIndex [24] = 48;	//Setup System Summary
	vHelpOrderIndex [25] = 12;	//Setup System
	vHelpOrderIndex [26] = 45;	//Setup System - Body Trigger
	vHelpOrderIndex [27] = 13;	//Setup System - Ejector
	vHelpOrderIndex [28] = 14;	//Setup System - Auxiliary Detector
	vHelpOrderIndex [29] = 41;	//Setup System - X-Ray Detector
	vHelpOrderIndex [30] = 38;	//Setup System - X-Ray Detector Edge Pixels
	vHelpOrderIndex [31] = 52;	//Setup System - E-mail Addresses
	vHelpOrderIndex [32] = 15;	//Diagnostics - Summary
	vHelpOrderIndex [33] = 19;	//Diagnostics - Voltages Status
	vHelpOrderIndex [34] = 43;	//Diagnostics - Micro Controller Communication
	vHelpOrderIndex [35] = 60;	//Diagnostics - AC Power Monitor Voltage Plot
	vHelpOrderIndex [36] = 61;	//Diagnostics - AC Power Monitor error log
	vHelpOrderIndex [37] = 22;	//Diagnostics - X-Ray
	vHelpOrderIndex [38] = 42;	//Diagnostics - Counters
	vHelpOrderIndex [39] = 20;	//Diagnostics - Test Modes
	vHelpOrderIndex [40] = 17;	//Diagnostics - Keypad Test
	vHelpOrderIndex [41] = 18;	//Diagnostics - Line Display
	vHelpOrderIndex [42] = 16;	//Image Display
	vHelpOrderIndex [43] = 50;	//X-Ray Source Strength Data
	vHelpOrderIndex [44] = 53;	//Production Report
	vHelpOrderIndex [45] = 51;	//Event Log Window
	vHelpOrderIndex [46] = 23;	//Enter Name
	vHelpOrderIndex [47] = 24;	//Enter Number
	vHelpOrderIndex [48] = 29;	//Select an Item
	vHelpOrderIndex [49] = 46;	//Select Multiple Items
	vHelpOrderIndex [50] = 25;	//Enter Password
	vHelpOrderIndex [51] = 26;	//Enter Yes or No
	vHelpOrderIndex [52] = 44;	//Histogram Display
	vHelpOrderIndex [53] = 27;	//Help
	vHelpOrderIndex [54] = 28;	//Help Index

	//not shown in index
	vHelpOrderIndex [55] = 32;	//Special Setup - Image Menu
	vHelpOrderIndex [56] = 31;	//Special Setup - Setup Menu

	vNumberOfHelpContextsToShow = cNumberOfHelpContexts - 3;
/*
	//HelpTitlexx number  //String Table Number  //does not appear to be used any where
	vHelpTitleIndex [0] = 260;  //About ScanTrac
	vHelpTitleIndex [1] = 1;		//Main Menu
	vHelpTitleIndex [2] = 2;		//Select Current Product
	vHelpTitleIndex [3] = 3;		//Setup
	vHelpTitleIndex [4] = 4;		//Setup Product
	vHelpTitleIndex [5] = 5;		//Setup Product - Container Trigger
	vHelpTitleIndex [6] = 6;		//Setup Product - Edit Product
	vHelpTitleIndex [7] = 7;		//Setup Product - Ejector
	vHelpTitleIndex [8] = 8;		//Setup Product - Auxiliary Detector
	vHelpTitleIndex [9] = 9;		//Setup Product - Region of Interest
	vHelpTitleIndex [10] = 10;	//Setup Product - X-Ray Detector
	vHelpTitleIndex [11] = 11;	//Setup Product - X-Ray Image Other Reject Criteria
	vHelpTitleIndex [12] = 12;	//Setup System
	vHelpTitleIndex [13] = 13;	//Setup System - Ejector
	vHelpTitleIndex [14] = 14;	//Setup System - Auxiliary Detector
	vHelpTitleIndex [15] = 15;	//Diagnostics - Summary
	vHelpTitleIndex [16] = 16;	//Image Display
	vHelpTitleIndex [17] = 17;	//Diagnostics - Keypad Test
	vHelpTitleIndex [18] = 18;	//Diagnostics - Line Display
	vHelpTitleIndex [19] = 19;	//Diagnostics - Voltages Status
	vHelpTitleIndex [20] = 20;	//Diagnostics - Test Modes
	vHelpTitleIndex [21] = 21;	//not used ****** was simulated encoder rate
	vHelpTitleIndex [22] = 22;	//Diagnostics - X-Ray
	vHelpTitleIndex [23] = 23;	//Enter Name
	vHelpTitleIndex [24] = 24;	//Enter Number
	vHelpTitleIndex [25] = 25;	//Enter Password
	vHelpTitleIndex [26] = 26;	//Enter Yes or No
	vHelpTitleIndex [27] = 27;	//Help
	vHelpTitleIndex [28] = 28;	//Help Index
	vHelpTitleIndex [29] = 29;	//Select an Item
	vHelpTitleIndex [30] = 30;	//Setup Product - Contaminant Bounds
	vHelpTitleIndex [31] = 31;	//Special Setup - Setup Menu
	vHelpTitleIndex [32] = 32;	//Special Setup - Image Menu
	vHelpTitleIndex [33] = 93;	//Setup Product - Auto Setup Size
	vHelpTitleIndex [34] = 94;	//Setup Product - Auto Setup Source
	vHelpTitleIndex [35] = 96;	//Setup Product - Auto Setup Image Learn
	vHelpTitleIndex [36] = 124;	//Setup Product - Auto Setup ContaminantThreshold
	vHelpTitleIndex [37] = 126; //Setup Product - Auto Setup ROI
	vHelpTitleIndex [38] = 128;	//Setup System - X-Ray Detector Edge Pixels
	vHelpTitleIndex [39] = 130;	//Setup Product - Contaminant Image Learn
	vHelpTitleIndex [40] = 132;	//Setup Product - Image Learning Reference
	vHelpTitleIndex [41] = 134;	//Setup System - X-Ray Detector
	vHelpTitleIndex [42] = 136;	//Diagnostics - Counters
	vHelpTitleIndex [43] = 138;	//Diagnostics - Micro Controller Communication
	vHelpTitleIndex [44] = 140;	//Histogram Display
	vHelpTitleIndex [45] = 142;	//Setup System - Body Trigger
	vHelpTitleIndex [46] = 143;	//Select Multiple Items
	vHelpTitleIndex [47] = 146;	//Setup Product - Auto Setup Void Threshold
	vHelpTitleIndex [48] = 148; //Setup System Summary
	vHelpTitleIndex [49] = 150; //Setup Product Summary
	vHelpTitleIndex [50] = 151; //Source Strength data window
	vHelpTitleIndex [51] = 165; //Event Log Window
	vHelpTitleIndex [52] = 172; //Setup System - E-Mail Addresses
	vHelpTitleIndex [53] = 173; //Production Report
	vHelpTitleIndex [54] = 168; //Setup Product - Setup Size - Image Settings
	vHelpTitleIndex [55] = 170; //Setup Product - Setup Threshold - Inspection Settings
	vHelpTitleIndex [56] = 160; //Setup Product - Setup Size - Image Settings
	vHelpTitleIndex [57] = 161; //Setup Product - Setup Threshold - Inspection Settings
	*/
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function4Button);

	vLocalDisplayCollection.Add(&m_SubFunction8Button);
	vLocalDisplayCollection.Add(&m_SubFunction7Button);
	vLocalDisplayCollection.Add(&m_SubFunction6Button);
	vLocalDisplayCollection.Add(&m_SubFunction5Button);
	vLocalDisplayCollection.Add(&m_SubFunction4Button);
	vLocalDisplayCollection.Add(&m_SubFunction3Button);
	vLocalDisplayCollection.Add(&m_SubFunction2Button);
	vLocalDisplayCollection.Add(&m_SubFunction1Button);
}

CHelpIndexDialog::~CHelpIndexDialog()
{
	DeleteObject(vWhiteBrush);
	DeleteObject(vBlueBrush);
	DeleteObject(vLightGrayBrush);
}

void CHelpIndexDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	this->SetWindowText(_T("Help Index"));
	::SetupMenu(vLocalCWndCollection);
	UpdateDisplay();
}

void CHelpIndexDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHelpIndexDialog)
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHelpIndexDialog, CDialog)
	//{{AFX_MSG_MAP(CHelpIndexDialog)
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
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
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CHelpIndexDialog::OnDialogtitlestatictext1)
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelpIndexDialog message handlers
void CHelpIndexDialog::OnFunction1Button() 
{
	// TODO: Add your control notification handler code here
}

void CHelpIndexDialog::OnFunction2Button() 
{
	// TODO: Add your control notification handler code here
}

void CHelpIndexDialog::OnFunction3Button() 
{
	//Previous Group button was pressed
  if (vCurrentFirstNumberShown > 1)
  {
		if (vCurrentFirstNumberShown > 8)
      vCurrentFirstNumberShown = vCurrentFirstNumberShown - 8;
		else
			vCurrentFirstNumberShown = 1;
		UpdateDisplay();
  }	
}

void CHelpIndexDialog::OnFunction4Button() 
{
	//Cancel button pressed
	CDialog::OnCancel();
}

void CHelpIndexDialog::OnFunction5Button() 
{
	if (vNumberOfHelpContextsToShow > vCurrentFirstNumberShown + 7)
	{
		//next button pressed to display next group of products
		if (vCurrentFirstNumberShown + 15 > vNumberOfHelpContextsToShow)
			vCurrentFirstNumberShown = vNumberOfHelpContextsToShow - 7;
		else
			vCurrentFirstNumberShown = vCurrentFirstNumberShown + 8;
		UpdateDisplay();
	}
}

void CHelpIndexDialog::OnSubFunction1Button() 
{
	SelectAndExit(1);
}

void CHelpIndexDialog::OnSubFunction2Button() 
{
	SelectAndExit(2);
}

void CHelpIndexDialog::OnSubFunction3Button() 
{
	SelectAndExit(3);
}

void CHelpIndexDialog::OnSubFunction4Button() 
{
	SelectAndExit(4);
}

void CHelpIndexDialog::OnSubFunction5Button() 
{
	SelectAndExit(5);
}

void CHelpIndexDialog::OnSubFunction6Button() 
{
	SelectAndExit(6);
}

void CHelpIndexDialog::OnSubFunction7Button() 
{
	SelectAndExit(7);
}

void CHelpIndexDialog::OnSubFunction8Button() 
{
	SelectAndExit(8);
}

void CHelpIndexDialog::SelectAndExit(BYTE TempNumber) 
{
	if ((TempNumber > 0) &&
		(vCurrentFirstNumberShown - 1 + TempNumber <= vNumberOfHelpContextsToShow))
	{
		//figure out what product number is selected by the button press
		vNumberSelected = vHelpOrderIndex[vCurrentFirstNumberShown - 1 + TempNumber];
		CDialog::EndDialog(true);
	}
	/*
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nNo item selected";
		TempNoticeDialog.vType = cWarningMessage;
		TempNoticeDialog.DoModal();
	}
	*/
}

void CHelpIndexDialog::UpdateDisplay()
{
	CString TempName("");
	CString TempProductNameString("");
	CString TempBlankName(" ");
	WORD TempLength = 0;
	for (WORD TempLoop = 1; TempLoop < 9;  TempLoop++)
	{
		if (vCurrentFirstNumberShown + TempLoop - 2 < vNumberOfHelpContextsToShow)
		{
			TempName.LoadString(100 +vHelpOrderIndex[vCurrentFirstNumberShown + TempLoop - 1]);
			if (TempLoop > 4)
				TempProductNameString = TempName + TempBlankName;
			else
				TempProductNameString =  TempBlankName + TempName;
			TempLength = TempProductNameString.GetLength();
		}
		else
			TempLength = 0;
		CWnd* pWnd;
		CDC* pDC;
		HWND TempDialogHwnd = this->m_hWnd;

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
					this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
					SetDlgItemText(IDC_SubFunction1Button,TempProductNameString);
					pDC = m_SubFunction1Button.GetDC();
					pWnd = &m_SubFunction1Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5) + 30;  //5 is medium large
					m_SubFunction1Button.SetWindowPos(NULL, ResizeXCoor(11), ResizeYCoor(52), TempStringLength, ResizeYCoor(28), SWP_NOZORDER);
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
					this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
					SetDlgItemText(IDC_SubFunction2Button,TempProductNameString);
					pDC = m_SubFunction2Button.GetDC();
					pWnd = &m_SubFunction2Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5) + 30;  //5 is medium large
					m_SubFunction2Button.SetWindowPos(NULL, ResizeXCoor(11), ResizeYCoor(142), TempStringLength, ResizeYCoor(28), SWP_NOZORDER);
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
					this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
					SetDlgItemText(IDC_SubFunction3Button,TempProductNameString);
					pDC = m_SubFunction3Button.GetDC();
					pWnd = &m_SubFunction3Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5) + 30;  //5 is medium large
					m_SubFunction3Button.SetWindowPos(NULL, ResizeXCoor(11), ResizeYCoor(232), TempStringLength, ResizeYCoor(28), SWP_NOZORDER);
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
					this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
					SetDlgItemText(IDC_SubFunction4Button,TempProductNameString);
					pDC = m_SubFunction4Button.GetDC();
					pWnd = &m_SubFunction4Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5) + 30;  //5 is medium large
					m_SubFunction4Button.SetWindowPos(NULL, ResizeXCoor(11), ResizeYCoor(327), TempStringLength, ResizeYCoor(28), SWP_NOZORDER);
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
					this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
					SetDlgItemText(IDC_SubFunction5Button,TempProductNameString);
					pDC = m_SubFunction5Button.GetDC();
					pWnd = &m_SubFunction5Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5) + 30;  //5 is medium large
					m_SubFunction5Button.SetWindowPos(NULL, ResizeXCoor(623) - TempStringLength, ResizeYCoor(79), TempStringLength, ResizeYCoor(28), SWP_NOZORDER);
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
					this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
					SetDlgItemText(IDC_SubFunction6Button,TempProductNameString);
					pDC = m_SubFunction6Button.GetDC();
					pWnd = &m_SubFunction6Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5) + 30;  //5 is medium large
					m_SubFunction6Button.SetWindowPos(NULL, ResizeXCoor(623) - TempStringLength, ResizeYCoor(169), TempStringLength, ResizeYCoor(28), SWP_NOZORDER);
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
					this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
					SetDlgItemText(IDC_SubFunction7Button,TempProductNameString);
					pDC = m_SubFunction7Button.GetDC();
					pWnd = &m_SubFunction7Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5) + 30;  //5 is medium large
					m_SubFunction7Button.SetWindowPos(NULL, ResizeXCoor(623) - TempStringLength, ResizeYCoor(259), TempStringLength, ResizeYCoor(28), SWP_NOZORDER);
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
					this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
					SetDlgItemText(IDC_SubFunction8Button,TempProductNameString);
					pDC = m_SubFunction8Button.GetDC();
					pWnd = &m_SubFunction8Button;
 					int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5) + 30;  //5 is medium large
					m_SubFunction8Button.SetWindowPos(NULL, ResizeXCoor(623) - TempStringLength, ResizeYCoor(354), TempStringLength, ResizeYCoor(28), SWP_NOZORDER);
				}
			break;
		}
	}
	CString TempText = "";
	if (vNumberOfHelpContextsToShow > vCurrentFirstNumberShown + 7)
	{
		m_Function5Button.ShowWindow(SW_SHOW);
		TempText.LoadStringW(IDS_Next);
		SetDlgItemText(IDC_Function5Button, TempText);
	}
	else
	{
		m_Function5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
		SetDlgItemText(IDC_Function5Button, _T(""));
	}

	if (vCurrentFirstNumberShown > 1)
	{
		m_Function3Button.ShowWindow(SW_SHOW);
		TempText.LoadStringW(IDS_Previous);
		SetDlgItemText(IDC_Function3Button, TempText);
	}
	else
	{
		m_Function3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
		SetDlgItemText(IDC_Function3Button, _T(""));
	}
}


BOOL CHelpIndexDialog::PreTranslateMessage(MSG* pMsg) 
{
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
				EndDialog(2);
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


HBRUSH CHelpIndexDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
		pDC->SetBkMode(TRANSPARENT);
		return vWhiteBrush;
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
	return hbr;
}

void CHelpIndexDialog::OnDialogtitlestatictext1()
{
	//bring up soft key menu for help, print screen, Image, Clear, and hidden shift key
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}

void CHelpIndexDialog::OnTimer(UINT_PTR nIDEvent)
{
	CDialog::OnTimer(nIDEvent);
}
