// Contaminant22EditThresholdsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "Contaminant22EditThresholdsDialog.h"
#include "NumericEntryDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern unsigned long vGlobalDialogTitleColor;
/////////////////////////////////////////////////////////////////////////////
// CContaminant22EditThresholdsDialog dialog


CContaminant22EditThresholdsDialog::CContaminant22EditThresholdsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CContaminant22EditThresholdsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CContaminant22EditThresholdsDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_ArrowDown4);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_ArrowDown1);
	vLocalCWndCollection.Add(&m_ArrowDown2);
	vLocalCWndCollection.Add(&m_ArrowDown3);
	vLocalCWndCollection.Add(&m_ArrowDown5);
	vLocalCWndCollection.Add(&m_ArrowLeft1);
	vLocalCWndCollection.Add(&m_ArrowLeft2);
	vLocalCWndCollection.Add(&m_ArrowLeft3);
	vLocalCWndCollection.Add(&m_ArrowLeft4);
	vLocalCWndCollection.Add(&m_ArrowRight1);
	vLocalCWndCollection.Add(&m_ArrowRight2);
	vLocalCWndCollection.Add(&m_ArrowRight3);
	vLocalCWndCollection.Add(&m_ArrowRight4);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function3Display);
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


void CContaminant22EditThresholdsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContaminant22EditThresholdsDialog)
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_ArrowDown4, m_ArrowDown4);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ArrowDown1, m_ArrowDown1);
	DDX_Control(pDX, IDC_ArrowDown2, m_ArrowDown2);
	DDX_Control(pDX, IDC_ArrowDown3, m_ArrowDown3);
	DDX_Control(pDX, IDC_ArrowDown5, m_ArrowDown5);
	DDX_Control(pDX, IDC_ArrowLeft1, m_ArrowLeft1);
	DDX_Control(pDX, IDC_ArrowLeft2, m_ArrowLeft2);
	DDX_Control(pDX, IDC_ArrowLeft3, m_ArrowLeft3);
	DDX_Control(pDX, IDC_ArrowLeft4, m_ArrowLeft4);
	DDX_Control(pDX, IDC_ArrowRight1, m_ArrowRight1);
	DDX_Control(pDX, IDC_ArrowRight2, m_ArrowRight2);
	DDX_Control(pDX, IDC_ArrowRight3, m_ArrowRight3);
	DDX_Control(pDX, IDC_ArrowRight4, m_ArrowRight4);
	DDX_Control(pDX, IDC_DialogTitleStaticText, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function3Display, m_Function3Display);
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


BEGIN_MESSAGE_MAP(CContaminant22EditThresholdsDialog, CDialog)
	//{{AFX_MSG_MAP(CContaminant22EditThresholdsDialog)
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
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContaminant22EditThresholdsDialog message handlers

void CContaminant22EditThresholdsDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);
	vHorizontal = true;
	vChangeMade = false;

	if (vInspectionEditing)
	for (BYTE TempLoop = 0; TempLoop < 7; TempLoop++)
	{
		_ThresholdsH[TempLoop] = vInspectionEditing->_ThresholdsH[TempLoop];
		_ThresholdsV[TempLoop] = vInspectionEditing->_ThresholdsV[TempLoop];
	}

	DisplayButtonLabels();
	ShowThresholdValues();
	this->SetWindowText( _T("Cont22 Thres"));
	
	::SetupMenu(vLocalCWndCollection);
}

void CContaminant22EditThresholdsDialog::OnFunction1Button() 
{
	//Main Menu was pressed
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::EndDialog(10);
}

void CContaminant22EditThresholdsDialog::OnFunction2Button() 
{
	//Save and exit was pressed
	if ((!vChangeMade) || (KeepSetupChanges()))
	{
		if (vInspectionEditing)
		for (BYTE TempLoop = 0; TempLoop < 7; TempLoop++)
		{
			vInspectionEditing->_ThresholdsH[TempLoop] = _ThresholdsH[TempLoop];
			vInspectionEditing->_ThresholdsV[TempLoop] = _ThresholdsV[TempLoop];
		}

		CDialog::EndDialog(true);
	}
}

void CContaminant22EditThresholdsDialog::OnFunction3Button() 
{
	//set multipliers to default or 1
	if (vInspectionEditing->_MultipliersH[0] == 0.75)
	{
		for (BYTE TempLoop = 0; TempLoop < 7; TempLoop++)
		{
			vInspectionEditing->_MultipliersH[TempLoop] = 1;
			vInspectionEditing->_MultipliersV[TempLoop] = 1; 	
		}
	}
	else
	{
		vInspectionEditing->_MultipliersH[0] = 0.75;
		vInspectionEditing->_MultipliersH[1] = 0.9;
		vInspectionEditing->_MultipliersH[2] = 0.85;
		vInspectionEditing->_MultipliersH[3] = 1.15;
		vInspectionEditing->_MultipliersH[4] = 1.0;
		vInspectionEditing->_MultipliersH[5] = 1.1;
		vInspectionEditing->_MultipliersH[6] = 1.1;

		vInspectionEditing->_MultipliersV[0] = 0.85; 	
		vInspectionEditing->_MultipliersV[1] = 0.8; 	
		vInspectionEditing->_MultipliersV[2] = 0.85; 	
		vInspectionEditing->_MultipliersV[3] = 1.15; 	
		vInspectionEditing->_MultipliersV[4] = 1.0; 	
		vInspectionEditing->_MultipliersV[5] = 1.05; 	
		vInspectionEditing->_MultipliersV[6] = 1.05; 	
	}
	DisplayButtonLabels();
}

void CContaminant22EditThresholdsDialog::OnFunction4Button() 
{
	//Cancel was pressed
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::OnCancel();
}

void CContaminant22EditThresholdsDialog::OnFunction5Button() 
{
	vHorizontal = !vHorizontal;
	DisplayButtonLabels();
	ShowThresholdValues();
}
void CContaminant22EditThresholdsDialog::OnSubFunction1Button() 
{
	EditThresholdValue(1);
}

void CContaminant22EditThresholdsDialog::OnSubFunction2Button() 
{
	EditThresholdValue(2);
}

void CContaminant22EditThresholdsDialog::OnSubFunction3Button() 
{
	EditThresholdValue(3);
}

void CContaminant22EditThresholdsDialog::OnSubFunction4Button() 
{
	EditThresholdValue(4);
}

void CContaminant22EditThresholdsDialog::OnSubFunction5Button() 
{
	EditThresholdValue(5);
}

void CContaminant22EditThresholdsDialog::OnSubFunction6Button() 
{
	EditThresholdValue(6);
}

void CContaminant22EditThresholdsDialog::OnSubFunction7Button() 
{
	EditThresholdValue(7);
}

void CContaminant22EditThresholdsDialog::OnSubFunction8Button() 
{
	// Set Default Thresholds

/*
	// with Latvians
							//Ours	//Latv
	_ThresholdsH[0] = -499;	//-521;	//-499;
	_ThresholdsH[1] = -622;	//-921;	//-622;
	_ThresholdsH[2] = -688;	//-870;	//-688;
	_ThresholdsH[3] = -675;	//-618;	//-675;
	_ThresholdsH[4] = 0;
	_ThresholdsH[5] = -732;	//-764;	//-732;
	_ThresholdsH[6] = -789;	//-940;	//-789;

	_ThresholdsV[0] = -553;	//-568;	//-553;
	_ThresholdsV[1] = -691;	//-647;	//-691;
	_ThresholdsV[2] = -793;	//-870;	//-793;
	_ThresholdsV[3] = -627;	//-683;	//-627;
	_ThresholdsV[4] = 0;
	_ThresholdsV[5] = -684;	//-702;	//-684;
	_ThresholdsV[6] = -683;	//-737;	//-683;

*/



	// with ours

	_ThresholdsH[0] = -699;	//-521;	//-499;	
	_ThresholdsH[1] = -858;	//-921;	//-622;
	_ThresholdsH[2] = -828;	//-870;	//-688;
	_ThresholdsH[3] = -883;	//-618;	//-675;
	_ThresholdsH[4] = 0;
	_ThresholdsH[5] = -1025;	//-764;	//-732;
	_ThresholdsH[6] = -1012;	//-940;	//-789;

  	

	_ThresholdsV[0] = -1154;	//-568;	//-553;	
	_ThresholdsV[1] = -1125;	//-647;	//-691;
	_ThresholdsV[2] = -1195;	//-870;	//-793;
	_ThresholdsV[3] = -1330;	//-683;	//-627;
	_ThresholdsV[4] = 0;
	_ThresholdsV[5] = -1425;	//-702;	//-684;
	_ThresholdsV[6] = -1436;	//-737;	//-683;

	SetChangeMade();

	ShowThresholdValues();
}

HBRUSH CContaminant22EditThresholdsDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
  }
  if ((pWnd->GetDlgCtrlID() == IDC_Function1Button) ||
		(pWnd->GetDlgCtrlID() == IDC_Function2Button) ||
		(pWnd->GetDlgCtrlID() == IDC_Function3Button) ||
		(pWnd->GetDlgCtrlID() == IDC_Function4Button) ||
		(pWnd->GetDlgCtrlID() == IDC_Function5Button) ||
		(pWnd->GetDlgCtrlID() == IDC_SubFunction1Button) ||
		(pWnd->GetDlgCtrlID() == IDC_SubFunction2Button) ||
		(pWnd->GetDlgCtrlID() == IDC_SubFunction3Button) ||
		(pWnd->GetDlgCtrlID() == IDC_SubFunction4Button) ||
		(pWnd->GetDlgCtrlID() == IDC_SubFunction5Button) ||
		(pWnd->GetDlgCtrlID() == IDC_SubFunction6Button) ||
		(pWnd->GetDlgCtrlID() == IDC_SubFunction7Button) ||
		(pWnd->GetDlgCtrlID() == IDC_SubFunction8Button))
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CContaminant22EditThresholdsDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
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
				PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 38, 0);

				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText);
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

void CContaminant22EditThresholdsDialog::ShowThresholdValues()
{
//short _ThresholdsH[7];
//short _ThresholdsV[7];

	if (vHorizontal)
	{
		SetDlgItemText(IDC_SubFunction1Display,dtoa(_ThresholdsH[0],0));
		SetDlgItemText(IDC_SubFunction2Display,dtoa(_ThresholdsH[1],0));
		SetDlgItemText(IDC_SubFunction3Display,dtoa(_ThresholdsH[2],0));
		SetDlgItemText(IDC_SubFunction4Display,dtoa(_ThresholdsH[3],0));
		SetDlgItemText(IDC_SubFunction5Display,dtoa(_ThresholdsH[4],0));
		SetDlgItemText(IDC_SubFunction6Display,dtoa(_ThresholdsH[5],0));
		SetDlgItemText(IDC_SubFunction7Display,dtoa(_ThresholdsH[6],0));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction1Display,dtoa(_ThresholdsV[0],0));
		SetDlgItemText(IDC_SubFunction2Display,dtoa(_ThresholdsV[1],0));
		SetDlgItemText(IDC_SubFunction3Display,dtoa(_ThresholdsV[2],0));
		SetDlgItemText(IDC_SubFunction4Display,dtoa(_ThresholdsV[3],0));
		SetDlgItemText(IDC_SubFunction5Display,dtoa(_ThresholdsV[4],0));
		SetDlgItemText(IDC_SubFunction6Display,dtoa(_ThresholdsV[5],0));
		SetDlgItemText(IDC_SubFunction7Display,dtoa(_ThresholdsV[6],0));
	}
}

void CContaminant22EditThresholdsDialog::DisplayButtonLabels()
{
	if (vHorizontal)
	{
		SetDlgItemText(IDC_Function5Button, _T("Change To Vertical"));
		SetDlgItemText(IDC_Function3Display, _T("Horizontal Filters"));

		SetDlgItemText(IDC_SubFunction1Button, _T("Threshold\n1\nFilter 5"));
		SetDlgItemText(IDC_SubFunction2Button, _T("Threshold\n2\nFilter 7"));
		SetDlgItemText(IDC_SubFunction3Button, _T("Threshold\n3\nFilter 9"));
		SetDlgItemText(IDC_SubFunction4Button, _T("Threshold\n4\nFilter 3"));
		SetDlgItemText(IDC_SubFunction5Button, _T("Threshold\n5\nFilter -"));
		SetDlgItemText(IDC_SubFunction6Button, _T("Threshold\n6\nFilter 5"));
		SetDlgItemText(IDC_SubFunction7Button, _T("Threshold\n7\nFilter 5a"));
	}
	else
	{
		SetDlgItemText(IDC_Function5Button, _T("Change To Horizontal"));
		SetDlgItemText(IDC_Function3Display, _T("Vertical Filters"));

		SetDlgItemText(IDC_SubFunction1Button, _T("Threshold\n1\nFilter 5"));
		SetDlgItemText(IDC_SubFunction2Button, _T("Threshold\n2\nFilter 9"));
		SetDlgItemText(IDC_SubFunction3Button, _T("Threshold\n3\nFilter 13"));
		SetDlgItemText(IDC_SubFunction4Button, _T("Threshold\n4\nFilter 3"));
		SetDlgItemText(IDC_SubFunction5Button, _T("Threshold\n5\nFilter -"));
		SetDlgItemText(IDC_SubFunction6Button, _T("Threshold\n6\nFilter 5"));
		SetDlgItemText(IDC_SubFunction7Button, _T("Threshold\n7\nFilter 5a"));
	}

	if (vInspectionEditing->_MultipliersH[0] == 0.75)
		SetDlgItemText(IDC_Function3Button, _T("Multipliers Set to 1!"));
	else
		SetDlgItemText(IDC_Function3Button, _T("Set Default Multipliers!"));

}

void CContaminant22EditThresholdsDialog::SetChangeMade() 
{
	if (!vChangeMade)
	{
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_Function4Button.ShowWindow(SW_SHOW);
		m_ArrowDown4.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_Function2Button, _T("Save\nand Exit"));
		SetDlgItemText(IDC_Function4Button, _T("Cancel"));
		vChangeMade = true;
	}
}

CString CContaminant22EditThresholdsDialog::GetName(BYTE TempNumber)
{
	if (vHorizontal)
	{
		switch (TempNumber)
		{
			case 1: return "Horizontal Threshold 1: Filter 5"; break;
			case 2: return "Horizontal Threshold 2: Filter 7"; break;
			case 3: return "Horizontal Threshold 3: Filter 9"; break;
			case 4: return "Horizontal Threshold 4: Filter 3"; break;
			case 5: return "Horizontal Threshold 5: nFilter -"; break;
			case 6: return "Horizontal Threshold 6: Filter 5"; break;
			case 7: return "Horizontal Threshold 7: Filter 5a"; break;
			default: return "Undefined";
		}
	}
	else
	{
		switch (TempNumber)
		{
			case 1: return "Vertical Threshold 1: Filter 5"; break;
			case 2: return "Vertical Threshold 2: Filter 9"; break;
			case 3: return "Vertical Threshold 3: Filter 13"; break;
			case 4: return "Vertical Threshold 4: Filter 3"; break;
			case 5: return "Vertical Threshold 5: nFilter -"; break;
			case 6: return "Vertical Threshold 6: Filter 5"; break;
			case 7: return "Vertical Threshold 7: Filter 5a"; break;
			default: return "Undefined";
		}
	}
}

short CContaminant22EditThresholdsDialog::GetDefault(BYTE TempNumber)
{

	//short _ThresholdsH[7] = {-499,-622,-688,-675,0,-732,-789};
	//short _ThresholdsV[7] = {-553,-691,-793,-627,0,-684,-683};
/*
	if (vHorizontal)
	{
		switch (TempNumber)
		{
			case 1: return -499; break;
			case 2: return -622; break;
			case 3: return -688; break;
			case 4: return -675; break;
			case 5: return 0; break;
			case 6: return -732; break;
			case 7: return -789; break;
			default: return 0;
		}
	}
	else
	{
		switch (TempNumber)
		{
			case 1: return -553; break;
			case 2: return -691; break;
			case 3: return -793; break;
			case 4: return -627; break;
			case 5: return 0; break;
			case 6: return -684; break;
			case 7: return -683; break;
			default: return 0;
		}
	}
*/
  

	if (vHorizontal)
	{
		switch (TempNumber)
		{
			case 1: return -699 ; break;
			case 2: return -858 ;  break;
			case 3: return -828 ;  break;
			case 4: return -883 ;  break;
			case 5: return 0; break;
			case 6: return -1025 ;  break;
			case 7: return -1012 ;  break;
			default: return 0;
		}
	}
	else
	{
		switch (TempNumber)
		{
			case 1: return -1154 ;  break;
			case 2: return -1125 ;  break;
			case 3: return -1195 ;  break;
			case 4: return -1330 ;  break;
			case 5: return 0; break;
			case 6: return -1425 ;  break;
			case 7: return -1436 ;  break;
			default: return 0;
		}
	}
}


void CContaminant22EditThresholdsDialog::EditThresholdValue(BYTE TempNumber)
{
	CNumericEntryDialog INumericEntryDialog;  
	
	//Set dialog box data titles and number value 1st
	if (vHorizontal)
		INumericEntryDialog.vEditString = dtoa(_ThresholdsH[TempNumber - 1],0);
	else
		INumericEntryDialog.vEditString = dtoa(_ThresholdsV[TempNumber - 1],0);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Threshold for " + GetName(TempNumber);
	INumericEntryDialog.m_DialogTitleStaticText2 = "Inspection: " + vInspectionEditing->vName;
	INumericEntryDialog.m_DialogTitleStaticText3 = "Default: " + dtoa(GetDefault(TempNumber),0);
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

	INumericEntryDialog.m_UnitsString = "0 is Disabled";
	INumericEntryDialog.vMaxValue = 65536;
	INumericEntryDialog.vAllowNegative = true;
	INumericEntryDialog.vIntegerOnly = true;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		if (vHorizontal)
			_ThresholdsH[TempNumber - 1] = _wtoi(INumericEntryDialog.vEditString);
		else
			_ThresholdsV[TempNumber - 1] = _wtoi(INumericEntryDialog.vEditString);
		SetChangeMade();
		ShowThresholdValues();
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}


