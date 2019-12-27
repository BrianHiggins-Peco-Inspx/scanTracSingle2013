//ScanTrac Side View Source File
// HistogramDisplayDialog.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "HistogramDisplayDialog.h"
#include "NewImageDisplayDialog.h"
#include "NumericEntryDialog.h"
#include "HelpDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BYTE vGlobalCurveFitEquationOrder;
extern WORD vGlobalCurveFitMaxCall; 
extern double vGlobalCurveFitEpsilon;
extern WORD vGlobalCurveFitStepBound;
extern double vGlobalCurveFitFTolerance;
extern double vGlobalCurveFitXTolerance;
extern double vGlobalCurveFitGTolerance;

extern BYTE vGlobalShiftKeyDown;
extern CProduct *vGlobalCurrentProduct;
extern int vGlobalPasswordLevel;
extern unsigned long vGlobalDialogTitleColor;
/////////////////////////////////////////////////////////////////////////////
// CHistogramDisplayDialog dialog


CHistogramDisplayDialog::CHistogramDisplayDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CHistogramDisplayDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHistogramDisplayDialog)
	//}}AFX_DATA_INIT
	vTopDataLine = " ";
	vXScaleCompression = 1;
	vXText = "";
	vYText = "";
	vShowFittedCurve = true;
	vCurveFitInspection = NULL;
	vCreatedDataArray = false;
	vCreatedIntegerArray = false;
	vPlotRawData = false;
	vXNumberOfScaleLines = 1;
	vYNumberOfScaleLines = 1;
	vDataTitle = "";
	vDataArray = NULL;
	vCurveFitDataArray = NULL;
	vNumberOfNumbers = 0;
	vValuesAreIntegers = false;
	vValuesAreBytes = false;
	vValuesAreFloats = false;
	vPointerToBytes = NULL;
	vPointerToIntegers = NULL;
	vHistogramData = NULL;
	vPointerToFloats = NULL;
	vPointerToTimes = NULL;
	vLocalSystemData = NULL;
	vMinimumX = 0;
	vMaximumX = 0;
	vNumberOfPointsToPlot = 0;
	vMinimumY = 0;
	vMaximumY = 0;
	vMaximumYToShow = 0;
	vMinimumYToShow = 0;
	vXScale = 0;
	vYScale = 0;
	vXScaleIsTime = false;
	vXScaleWord = 0;
	vClipCounts = 0;
	vBluePen.CreatePen(PS_SOLID, 1, cBlue);
	vRedPen.CreatePen(PS_SOLID, 1, cRed);
	vGreyPen.CreatePen(PS_SOLID, 1, cDarkGray);
	vShowCurveFitDataLine = true;
	vACPowerMoniterFormat = false;
	
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_XScale14);
	vLocalCWndCollection.Add(&m_XScale13);
	vLocalCWndCollection.Add(&m_XScale12);
	vLocalCWndCollection.Add(&m_XScale11);
	vLocalCWndCollection.Add(&m_YScale15);
	vLocalCWndCollection.Add(&m_YScale14);
	vLocalCWndCollection.Add(&m_YScale13);
	vLocalCWndCollection.Add(&m_YScale12);
	vLocalCWndCollection.Add(&m_YScale11);
	vLocalCWndCollection.Add(&m_XScale9);
	vLocalCWndCollection.Add(&m_XScale8);
	vLocalCWndCollection.Add(&m_XScale6);
	vLocalCWndCollection.Add(&m_XScale7);
	vLocalCWndCollection.Add(&m_XScale5);
	vLocalCWndCollection.Add(&m_XScale4);
	vLocalCWndCollection.Add(&m_XScale3);
	vLocalCWndCollection.Add(&m_XScale2);
	vLocalCWndCollection.Add(&m_XScale10);
	vLocalCWndCollection.Add(&m_XScale1);
	vLocalCWndCollection.Add(&m_YScale9);
	vLocalCWndCollection.Add(&m_YScale8);
	vLocalCWndCollection.Add(&m_YScale7);
	vLocalCWndCollection.Add(&m_YScale6);
	vLocalCWndCollection.Add(&m_YScale5);
	vLocalCWndCollection.Add(&m_YScale4);
	vLocalCWndCollection.Add(&m_YScale3);
	vLocalCWndCollection.Add(&m_YScale2);
	vLocalCWndCollection.Add(&m_YScale10);
	vLocalCWndCollection.Add(&m_YScale1);
	vLocalCWndCollection.Add(&m_TotalSamples);
	vLocalCWndCollection.Add(&m_LineLabel1);
	vLocalCWndCollection.Add(&m_LineLabel2);
	vLocalCWndCollection.Add(&m_LineLabel3);
	vLocalCWndCollection.Add(&m_LineLabel4);
	vLocalCWndCollection.Add(&m_LineLabel6);
	vLocalCWndCollection.Add(&m_XLabel2);
	vLocalCWndCollection.Add(&m_XLabel3);
	vLocalCWndCollection.Add(&m_XLabel1);
	vLocalCWndCollection.Add(&m_DataTitle);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_NextButton);
	vLocalCWndCollection.Add(&m_PreviousButton);
	vLocalCWndCollection.Add(&m_XRaysOn);
}

void CHistogramDisplayDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	// TODO: Add your message handler code here
	::SetupMenu(vLocalCWndCollection);
	SetDlgItemText(IDC_DataTitle,vDataTitle);
	if (vNumberOfNumbers)
	{
		CString TempString = dtoa(vNumberOfNumbers, 0);
		if( (!vACPowerMoniterFormat)  || PasswordOK(cTemporaryInspxPassword,false) )
			TempString = "Total Samples: " + TempString;
		else
			TempString = "";
		SetDlgItemText(IDC_TotalSamples,TempString);

		if (vTopDataLine.GetLength() > 1)
			SetDlgItemText(IDC_DialogTitleStaticText1,vTopDataLine);

		if ((vPlotRawData) && (vPointerToFloats))
			DisplayRawDataPlot();
		else
		if ((vValuesAreIntegers) && (vHistogramData))
			DisplayIntegerHistogram();
		else
		if ((vValuesAreIntegers) && (vPointerToIntegers))
			DisplayIntegerHistogram();
		else
		if ((vValuesAreFloats) && (vPointerToFloats))
			DisplayFloatHistogram();
		else
		if ((vValuesAreBytes) && (vPointerToBytes))
			DisplayByteHistogram();
		else
		{
			SetDlgItemText(IDC_LineLabel3, _T("Histogram Pointer Error"));
			ClearWindowText();
		}
		ShowInspxButtons();
	}
	else
	{
		ClearWindowText();
		SetDlgItemText(IDC_LineLabel3, _T("No Data to plot"));
	}

	if (vPlotRawData)
	{
		if(vACPowerMoniterFormat)
		{
			SetDlgItemText(IDC_LineLabel4, _T("X is the time of the sample"));
			SetDlgItemText(IDC_LineLabel2, _T("Y is the Value of the Input AC Voltage"));
		}
		else
		{
			if (vXText.GetLength() > 0)
				SetDlgItemText(IDC_LineLabel4,vXText);
			else
			{
				if (vXScaleIsTime)
					SetDlgItemText(IDC_LineLabel4, _T("X is the Sample Time"));
				else
					SetDlgItemText(IDC_LineLabel4, _T("X is the Sample Number"));
			}

			if (vYText.GetLength() > 0)
				SetDlgItemText(IDC_LineLabel2,vYText);
			else
				SetDlgItemText(IDC_LineLabel2, _T("Y is the value of the Sample"));
		}
		SetDlgItemText(IDC_Function2Button, _T("Clip Data"));
	}
	if (vACPowerMoniterFormat)
	{
		m_NextButton.ShowWindow(SW_SHOW);
		m_PreviousButton.ShowWindow(SW_SHOW);
	}

	this->SetWindowText(_T("Histogram"));
}

BOOL CHistogramDisplayDialog::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (vDataArray)
	if (vCreatedDataArray)
		free(vDataArray);

	if (vCurveFitDataArray)
		free(vCurveFitDataArray);

	if (vPointerToIntegers)
	if (vCreatedIntegerArray)
	{
		free(vPointerToIntegers);
		vPointerToIntegers = NULL;
	}

	if (!vValuesAreIntegers)
	if ((vPointerToFloats) || (vPointerToBytes))
	if (vPointerToIntegers)
		free(vPointerToIntegers);

	vBluePen.DeleteObject( );
	vRedPen.DeleteObject( );
	vGreyPen.DeleteObject( );

	return CDialog::DestroyWindow();
}

void CHistogramDisplayDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHistogramDisplayDialog)
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_XScale14, m_XScale14);
	DDX_Control(pDX, IDC_XScale13, m_XScale13);
	DDX_Control(pDX, IDC_XScale12, m_XScale12);
	DDX_Control(pDX, IDC_XScale11, m_XScale11);
	DDX_Control(pDX, IDC_YScale15, m_YScale15);
	DDX_Control(pDX, IDC_YScale14, m_YScale14);
	DDX_Control(pDX, IDC_YScale13, m_YScale13);
	DDX_Control(pDX, IDC_YScale12, m_YScale12);
	DDX_Control(pDX, IDC_YScale11, m_YScale11);
	DDX_Control(pDX, IDC_XScale9, m_XScale9);
	DDX_Control(pDX, IDC_XScale8, m_XScale8);
	DDX_Control(pDX, IDC_XScale6, m_XScale6);
	DDX_Control(pDX, IDC_XScale7, m_XScale7);
	DDX_Control(pDX, IDC_XScale5, m_XScale5);
	DDX_Control(pDX, IDC_XScale4, m_XScale4);
	DDX_Control(pDX, IDC_XScale3, m_XScale3);
	DDX_Control(pDX, IDC_XScale2, m_XScale2);
	DDX_Control(pDX, IDC_XScale10, m_XScale10);
	DDX_Control(pDX, IDC_XScale1, m_XScale1);
	DDX_Control(pDX, IDC_YScale9, m_YScale9);
	DDX_Control(pDX, IDC_YScale8, m_YScale8);
	DDX_Control(pDX, IDC_YScale7, m_YScale7);
	DDX_Control(pDX, IDC_YScale6, m_YScale6);
	DDX_Control(pDX, IDC_YScale5, m_YScale5);
	DDX_Control(pDX, IDC_YScale4, m_YScale4);
	DDX_Control(pDX, IDC_YScale3, m_YScale3);
	DDX_Control(pDX, IDC_YScale2, m_YScale2);
	DDX_Control(pDX, IDC_YScale10, m_YScale10);
	DDX_Control(pDX, IDC_YScale1, m_YScale1);
	DDX_Control(pDX, IDC_TotalSamples, m_TotalSamples);
	DDX_Control(pDX, IDC_LineLabel4, m_LineLabel4);
	DDX_Control(pDX, IDC_LineLabel2, m_LineLabel2);
	DDX_Control(pDX, IDC_XLabel2, m_XLabel2);
	DDX_Control(pDX, IDC_XLabel3, m_XLabel3);
	DDX_Control(pDX, IDC_XLabel1, m_XLabel1);
	DDX_Control(pDX, IDC_LineLabel6, m_LineLabel6);
	DDX_Control(pDX, IDC_LineLabel3, m_LineLabel3);
	DDX_Control(pDX, IDC_LineLabel1, m_LineLabel1);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DataTitle, m_DataTitle);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_XRaysOn, m_XRaysOn);
	DDX_Control(pDX, IDC_NextButton, m_NextButton);
	DDX_Control(pDX, IDC_PreviousButton, m_PreviousButton);
}


BEGIN_MESSAGE_MAP(CHistogramDisplayDialog, CDialog)
	//{{AFX_MSG_MAP(CHistogramDisplayDialog)
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
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CHistogramDisplayDialog::OnDialogtitlestatictext1)
	ON_STN_CLICKED(IDC_NextButton, &CHistogramDisplayDialog::OnStnClickedNextbutton)
	ON_STN_CLICKED(IDC_PreviousButton, &CHistogramDisplayDialog::OnStnClickedPreviousbutton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistogramDisplayDialog message handlers

void CHistogramDisplayDialog::OnFunction1Button() 
{
	// Save and Exit Button was pressed
	CDialog::EndDialog(true);
}

void CHistogramDisplayDialog::OnFunction2Button() 
{
	if (m_Function2Button.IsWindowVisible())
	{
		CNumericEntryDialog INumericEntryDialog;  
		
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vClipCounts,0);

		INumericEntryDialog.m_DialogTitleStaticText1 = "";
			INumericEntryDialog.m_DialogTitleStaticText2 = "Clip Display Top or Bottom to";
		INumericEntryDialog.m_DialogTitleStaticText3 = "";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
			INumericEntryDialog.vEditString;

		INumericEntryDialog.m_UnitsString = "0 = Disabled";
		if (vPlotRawData)
			INumericEntryDialog.vMaxValue = (WORD)(vMaximumYDouble - 1);
		else
			INumericEntryDialog.vMaxValue = vMaximumY;
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vClipCounts = ResizeYCoor(_wtoi(INumericEntryDialog.vEditString));
			if ((vPlotRawData) && (vPointerToFloats))
				DisplayRawDataPlot();
			else
			if ((vValuesAreIntegers) && (vHistogramData))
				DisplayIntegerHistogram();
			else
			if ((vValuesAreIntegers) && (vPointerToIntegers))
				DisplayIntegerHistogram();
			else
			if ((vValuesAreFloats) && (vPointerToFloats))
				DisplayFloatHistogram();
			else
			if ((vValuesAreBytes) && (vPointerToBytes))
				DisplayByteHistogram();
			else
			{
				SetDlgItemText(IDC_LineLabel3, _T("Histogram Pointer Error"));
				ClearWindowText();
			}
			this->Invalidate(true);
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

void CHistogramDisplayDialog::OnFunction3Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CHistogramDisplayDialog::OnFunction4Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CHistogramDisplayDialog::OnFunction5Button() 
{
	if (vLocalSystemData->vInAutoSetup)
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		//Images Button Pressed
		if ((vLocalSystemData->vITIPCDig->vAquisitionBuffer == NULL) ||
			(vGlobalCurrentProduct == NULL))
		{
			CString TempText;
			TempText.LoadString(IDS_Youmustselectaproducttoseeimages);  //need 3 blank lines first

			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			//Create diagnostic dialog box
			CNewImageDisplayDialog IImageDisplayDialog;  
			//Set dialog box data like how many products there are
			IImageDisplayDialog.vLocalSystemData = vLocalSystemData;
			IImageDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
			IImageDisplayDialog.vMainWindowPointer = vMainWindowPointer;

			//Pass control to dialog box and display
			int nResponse = IImageDisplayDialog.DoModal();
			CDialog::EndDialog(12);
		}
	}
}

void CHistogramDisplayDialog::OnSubFunction1Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CHistogramDisplayDialog::OnSubFunction2Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CHistogramDisplayDialog::OnSubFunction3Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CHistogramDisplayDialog::OnSubFunction4Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CHistogramDisplayDialog::OnSubFunction5Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CHistogramDisplayDialog::OnSubFunction6Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CHistogramDisplayDialog::OnSubFunction7Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CHistogramDisplayDialog::OnSubFunction8Button() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CHistogramDisplayDialog::PreTranslateMessage(MSG* pMsg) 
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
			case 37: //arrow Left
			case 39: //arrow Right
			case 38: //arrow up
			case 40: //arrow down
			//case c3: //used below to exit
			case 105:
			case c2:
			case c8:
			case c4:
			case c6:
				CDialog::EndDialog(pMsg->wParam);
			break;
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
				return true;
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
				//if( vACPowerMoniterFormat )
				//	CDialog::EndDialog(pMsg->wParam);
				//else
					PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
			break;
			case 36:  //Home key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
			case cPageDown: // Help
				if (vACPowerMoniterFormat)
					PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 60, 0);
				else
					PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 44, 0);
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
					PasswordOK(cAnyPasswordToEnter, true); //allow any password to be entered
				}
				ShowInspxButtons();
				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			break;	
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
			case c3:
				if (vACPowerMoniterFormat)
					CDialog::EndDialog(pMsg->wParam);
				else
				if (PasswordOK(cSuperInspxPassword,false))
				if (vShowFittedCurve)
				if (vCurveFitDataArray)
					ShowCurveFitAnalysisData();
			break;
			case c5:
				if (PasswordOK(cSuperInspxPassword,false))
					ShowData();
			break;
			case c1:
				if (PasswordOK(cSuperInspxPassword,false))
					vGlobalCurveFitEquationOrder--;
			break;
			case c7:
				if (PasswordOK(cSuperInspxPassword,false))
					vGlobalCurveFitEquationOrder++;
			break;
		}
		return true;  //true indicates it has been handled, so do not process
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CHistogramDisplayDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

	if (pWnd->GetDlgCtrlID() == IDC_PreviousButton)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_PreviousButton, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_NextButton)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_NextButton, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
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

	if (pWnd->GetDlgCtrlID() == IDC_XScale14)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale14, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_XScale13)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale13, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_XScale12)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale12, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_XScale11)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale11, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_XScale9)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale9, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_XScale8)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale8, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_XScale6)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale6, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_XScale7)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale7, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_XScale5)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale5, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_XScale4)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale4, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_XScale3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale3, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_XScale2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale2, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_XScale10)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale10, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_XScale1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale1, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() == IDC_YScale15)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale15, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_YScale14)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale14, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_YScale13)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale13, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_YScale12)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale12, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_YScale11)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale11, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_YScale9)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale9, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_YScale8)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale8, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_YScale7)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale7, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_YScale6)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale6, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_YScale5)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale5, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_YScale4)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale4, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_YScale3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale3, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_YScale2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale2, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_YScale10)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale10, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() ==	IDC_YScale1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale1, 5);  //5 is medium large
	}
	if (pWnd->GetDlgCtrlID() == IDC_TotalSamples)
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_TotalSamples, 5);  //not a subfunctionbutton but has 3 lines
	if (pWnd->GetDlgCtrlID() == IDC_LineLabel4)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LineLabel4, 5);
	if (pWnd->GetDlgCtrlID() == IDC_LineLabel2)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LineLabel2, 5);
	if (pWnd->GetDlgCtrlID() == IDC_XLabel2)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XLabel2, 5);
	if (pWnd->GetDlgCtrlID() == IDC_XLabel3)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XLabel3, 5);
	if (pWnd->GetDlgCtrlID() == IDC_XLabel1)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XLabel1, 5);
	if (pWnd->GetDlgCtrlID() == IDC_LineLabel6)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LineLabel6, 5);
	if (pWnd->GetDlgCtrlID() == IDC_LineLabel3)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LineLabel3, 5);
	if (pWnd->GetDlgCtrlID() == IDC_LineLabel1)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LineLabel1, 5);
	if (pWnd->GetDlgCtrlID() == IDC_DataTitle)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DataTitle, 5);
	return hbr;
}

void CHistogramDisplayDialog::DisplayFloatHistogram()
{
	if (ThereIsEnoughMemory(vNumberOfNumbers * 8,"Histogram Integer Buffer"))
	{  //create a temporary array to contain the number of occurrences of each possible value
		vPointerToIntegers = (DWORD *)malloc(vNumberOfNumbers * 8);
		if (vPointerToIntegers)
		{
			vCreatedIntegerArray = true;

			DWORD *TempDestinationPointer = vPointerToIntegers;  
			double *TempSourcePointer = vPointerToFloats; 
			for (DWORD TempLoop = 0; TempLoop < vNumberOfNumbers; TempLoop++)
			{ //sum up how many of each value occured
				*TempDestinationPointer = (int)(*TempSourcePointer + .5);
				TempDestinationPointer++;
				TempSourcePointer++;
			}
			DisplayIntegerHistogram();
		}
	}
}

void CHistogramDisplayDialog::DisplayByteHistogram()
{
	if (vPointerToBytes)
	if (ThereIsEnoughMemory(vNumberOfNumbers * sizeof(DWORD),"Histogram Integer Buffer"))
	{  //create a temporary array to contain the number of occurrences of each possible value
		vPointerToIntegers = (DWORD*)malloc(vNumberOfNumbers * sizeof(DWORD));
		if (vPointerToIntegers)
		{
			vCreatedIntegerArray = true;
			DWORD *TempDestinationPointer = vPointerToIntegers;  
			BYTE *TempSourcePointer = vPointerToBytes; 
			for (WORD TempLoop = 0; TempLoop < vNumberOfNumbers; TempLoop++)
			{ //sum up how many of each value occured
				*TempDestinationPointer++ = (int)*TempSourcePointer++;
			}
			DisplayIntegerHistogram();
		}
	}
}

void CHistogramDisplayDialog::DisplayIntegerHistogram()
{
	vDataArray = NULL;
	vMinimumX = (DWORD)0xFFFFFFFFFFFFFFFF;
	vMaximumX = 0;
	if (vHistogramData)
	{
		vNumberOfNumbers = 0;
		bool TempFoundFirst = false;
		DWORD *TempPointer = (DWORD *)vHistogramData;
		for (int TempLoop = 0; TempLoop < 256; TempLoop++)
		{  //find the minimum and maximum values of the data
			if (*TempPointer)
			{
				if (!TempFoundFirst)
				{
					vMinimumX = (WORD)TempLoop;
					TempFoundFirst = true;
				}
				vNumberOfNumbers = vNumberOfNumbers + *TempPointer;
				vMaximumX = (WORD)TempLoop;
			}
			TempPointer++;
		}

		SetDlgItemText(IDC_TotalSamples,"Total Samples: " + dtoa(vNumberOfNumbers,0));

		if (vMaximumX >= vMinimumX)
		{
			vNumberOfPointsToPlot = vMaximumX - vMinimumX + 1;
			vDataArray = (DWORD *)vHistogramData;
		}
	}
	else
	{
		DWORD *TempPointer = vPointerToIntegers;
		for (DWORD TempLoop = 0; TempLoop < vNumberOfNumbers; TempLoop++)
		{  //find the minimum and maximum values of the data
			if (*TempPointer < vMinimumX)
				vMinimumX = *TempPointer;
			if (*TempPointer > vMaximumX)
				vMaximumX = *TempPointer;
			TempPointer++;
		}

		if ((vMaximumX) && (vMaximumX >= vMinimumX))
		{
			vNumberOfPointsToPlot = vMaximumX - vMinimumX + 1;
			DWORD TempDataPointsPerPlotPoint = vNumberOfPointsToPlot / 570;
			if (TempDataPointsPerPlotPoint < 1)
				TempDataPointsPerPlotPoint = 1;

			vXScaleCompression = vNumberOfPointsToPlot / 570.0;

			vNumberOfPointsToPlot = vNumberOfPointsToPlot / TempDataPointsPerPlotPoint;
			if (TempDataPointsPerPlotPoint > 1)
				TempDataPointsPerPlotPoint++;


			if (vNumberOfPointsToPlot)
			if (ThereIsEnoughMemory((vNumberOfPointsToPlot + 1) * sizeof(DWORD),"Histogram Data Buffer"))
			{  //create a temporary array to contain the number of occurrences of each possible value
				vDataArray = (DWORD*)malloc((vNumberOfPointsToPlot + 1) * sizeof(DWORD));
				if (vDataArray)
				{
					vCreatedDataArray = true;
					//fill buffer with zeros
					DWORD *TempDataArrayPointer = vDataArray;
					for (DWORD TempLoop = 0; TempLoop <= vNumberOfPointsToPlot; TempLoop++)
						*TempDataArrayPointer++ = 0;

					TempPointer = vPointerToIntegers;  //points to the item adding to the histogram
					DWORD TempMaximumPlotPoint = vMaximumX / TempDataPointsPerPlotPoint;
					DWORD TempMinimumPlotPoint = vMinimumX / TempDataPointsPerPlotPoint;

					for (DWORD TempLoop = 0; TempLoop < vNumberOfNumbers; TempLoop++)
					{ //sum up how many of each value occured
						DWORD TempWord = (DWORD)(*TempPointer / TempDataPointsPerPlotPoint);
						if (TempWord > TempMaximumPlotPoint)
						{
							ReportErrorMessage("Histogram Error 1",cEMailInspx,32000);
							TempWord = TempMaximumPlotPoint;
						}
						if (TempWord < TempMinimumPlotPoint)
						{
							ReportErrorMessage("Histogram Error 2",cEMailInspx,32000);
							TempWord = TempMinimumPlotPoint;
						}
						if ((TempWord >= TempMinimumPlotPoint) && ((TempWord - TempMinimumPlotPoint) <= vNumberOfPointsToPlot))
						{
							TempDataArrayPointer = vDataArray + TempWord - TempMinimumPlotPoint;
							*TempDataArrayPointer = *TempDataArrayPointer + 1;
							TempPointer++;
						}
						else
							ReportErrorMessage("Histogram Error 21",cEMailInspx,32000);
					}
				}
			}
		}
	}

	if ((vDataArray) && (vNumberOfPointsToPlot))
	{
		//find the minimum and maximum Y values
		vMinimumY = 65000;
		vMaximumY = 0;
		DWORD *TempPointer = vDataArray;
		for (DWORD TempLoop = 0; TempLoop < vNumberOfPointsToPlot; TempLoop++)
		{  //find the minimum and maximum values of the data
			if (*TempPointer < vMinimumY)
				vMinimumY = *TempPointer;
			if (*TempPointer > vMaximumY)
				vMaximumY = *TempPointer;
			TempPointer++;
		}
		//vMinimumY = 0;

		if ((vMaximumY) && (vMaximumY >= vMinimumY))
		{
			vMaximumYToShow = vMaximumY;
			vMinimumYToShow = vMinimumY;

			if (vClipCounts)
			{
				double TempCountsToMaximum = vMaximumYToShow - vClipCounts;
				double TempCountsToMinimum = vClipCounts - vMinimumYToShow;

				if ((TempCountsToMaximum > 0) && (TempCountsToMaximum < TempCountsToMinimum))
					vMinimumYToShow = (WORD)vClipCounts;
				else
					vMaximumYToShow = (WORD)vClipCounts;

				if (vMaximumYToShow < vMinimumYToShow)
					vMaximumYToShow = vMinimumYToShow + 2;
			}


			//plot the data on the screen
			//x dimension is  top=41 bottom = 408 height = 367; vMinimumX to vMaximumX
			//y dimension is  left = 106 right=529 width= 423; vMinimumYToShow to vMaximumY
			//vXScale = 423.0 / vNumberOfPointsToPlot;
			vXScale = (double) ResizeXCoor( (int)(570.0 / vNumberOfPointsToPlot));  
			if (vMaximumYToShow > vMinimumYToShow)
				vYScale = (double) ResizeYCoor( (int)(30000.0 / (vMaximumYToShow - vMinimumYToShow) ) ) / 100;
			else
				vYScale = ResizeYCoor(50);
			if (vXScale > ResizeXCoor(50))
				vXScale = ResizeXCoor(50);
			if (vYScale > ResizeYCoor(50))
				vYScale = ResizeYCoor(50);
			vXScaleWord = (WORD)(vXScale);  //truncate
			vXInterval = (WORD)((vNumberOfPointsToPlot / 10.0) + .5);
			vYInterval = (WORD)(((vMaximumYToShow - vMinimumYToShow) / 10.0) + .5);
			if (!vXInterval)
				vXInterval = 1;
			if (!vYInterval)
				vYInterval = 1;
			vXNumberOfScaleLines = (int)((vNumberOfPointsToPlot) / vXInterval);
			vYNumberOfScaleLines = (vMaximumYToShow - vMinimumYToShow) / vYInterval + 1;
			if ((vXNumberOfScaleLines * vXInterval) + vMinimumX <= vMaximumX)
				vXNumberOfScaleLines++;

			if ((vMaximumYToShow > vMinimumYToShow) && 
				(vMaximumYToShow > vMinimumYToShow + ((vYNumberOfScaleLines - 1) * vYInterval)))
			{
				m_LineLabel1.ShowWindow(SW_SHOW);
				SetDlgItemText(IDC_LineLabel1, dtoa(vMaximumYToShow, 0));
			}
			else
			{
				m_LineLabel1.ShowWindow(SW_HIDE);
				SetDlgItemText(IDC_LineLabel1, _T(""));
			}
			SetDlgItemText(IDC_LineLabel6, _T(""));
			
			CString TempString = dtoa(vYInterval, 0);
			if (vACPowerMoniterFormat)
			{
				if (vYInterval == 1)
					TempString = TempString + " Volt";
				else
					TempString = TempString + " Volts";
			}
							
			if (vXScaleIsTime)
				SetDlgItemText(IDC_LineLabel3,"Y Scale: " + TempString + "`C");
			else
				SetDlgItemText(IDC_LineLabel3,"Y Scale: " + TempString);

			SetDlgItemText(IDC_XLabel1, _T(""));
			if ((vMaximumX > vMinimumX) && (vMaximumX > vMinimumX + ((vXNumberOfScaleLines - 1) * vXInterval)))
			{
				if(vACPowerMoniterFormat)
				{
					if (vMaximumX == 1)
						TempString=TempString+ " Volt";
					else
						TempString=TempString+ " Volts";
				}
				SetDlgItemText(IDC_XLabel3, dtoa(vMaximumX, 0));
			}
			else
				SetDlgItemText(IDC_XLabel3, _T(""));
			
			if (!vACPowerMoniterFormat)
			{
				if (vXScaleIsTime)
					SetDlgItemText(IDC_XLabel2, _T("Hrs:Mins"));
				else
					SetDlgItemText(IDC_XLabel2, "X Scale: " + dtoa(vXInterval, 0));
			}
			else
				SetDlgItemText(IDC_XLabel2, _T("Hrs:Mins"));
		}
		else
		{
			SetDlgItemText(IDC_DataTitle, _T("Y Data Problem or too few points to plot"));
			m_LineLabel3.ShowWindow(SW_HIDE);
			ClearWindowText();
		}

		
		
		WORD TempXMiddle = (WORD) ResizeXCoor(340);  //320
		WORD TempYMiddle = (WORD) ResizeYCoor(245); //240
		WORD TempWidth = (WORD)(vNumberOfPointsToPlot * vXScale / 2.0);
		if (TempWidth > ResizeXCoor(298))
		{
			TempWidth = (WORD) ResizeXCoor(298);
			ReportErrorMessage("Histogram Error 5",cEMailInspx,32000);
		}
		WORD TempHeight = (WORD)((vMaximumYToShow - vMinimumYToShow) * vYScale / 2.0);
		if (TempHeight > (WORD) ResizeYCoor(200))
		{
			TempHeight = (WORD) ResizeYCoor(200);
			ReportErrorMessage("Histogram Error 6",cEMailInspx,32000);
		}
		WORD TempLeftSide = TempXMiddle - TempWidth;//200; //106;
		if (TempLeftSide < (WORD) ResizeXCoor(8))
		{
			ReportErrorMessage("Histogram Error 7",cEMailInspx,32000);
			TempLeftSide = (WORD) ResizeXCoor(8);
		}
		WORD TempRightSide = TempXMiddle + TempWidth;//TempLeftSide + vNumberOfPointsToPlot;//529;
		WORD TempBottom = TempYMiddle + TempHeight; //170; //408;
		WORD TempTop = TempYMiddle - TempHeight;//TempBottom - (vYScale); //41;
		if (TempTop < vYScale + 2)
		{
			ReportErrorMessage("Histogram Error 8",cEMailInspx,32000);
			TempTop = 80;
		}
		//	WORD TempY = (WORD)(TempBottom  - (vYScale / 2) - 
		//		(((TempLoop) *  vYInterval) * vYScale));

		int TempResizedY16 = ResizeYCoor(16);
		int TempResizedY6 = ResizeYCoor(6);
		int TempResizedX12 = ResizeXCoor(12);
		int TempResizedX50 = ResizeXCoor(50);
		int TempResizedY8 = ResizeYCoor(8);
		int TempResizedY20 = ResizeYCoor(20);
		int TempResizedY17 = ResizeYCoor(17);
					

		m_LineLabel6.SetWindowPos(NULL,TempLeftSide - ResizeXCoor(60),(int)(TempBottom - (vYScale / 2)),TempResizedX50,TempResizedY20,SWP_NOZORDER);
		m_LineLabel3.SetWindowPos(NULL,ResizeXCoor(10),ResizeYCoor(10),ResizeXCoor(110),ResizeYCoor(20),SWP_NOZORDER);
		//m_LineLabel1 is maximum Y
		m_LineLabel1.SetWindowPos(NULL,TempLeftSide - TempResizedX12 - TempResizedX50,
			(WORD)(TempBottom  - (((vYNumberOfScaleLines - 1) *  vYInterval) * vYScale)
			- (vYScale / 2) - ResizeYCoor(25))
			,TempResizedX50,TempResizedY20,SWP_NOZORDER);
			//(int)(TempTop - vYScale - 10),50,20,SWP_NOZORDER);

		WORD TempLengthOfNumbers = (WORD) ResizeXCoor(30);
		WORD TempStaggerNumberAmount = (WORD) ResizeYCoor(6);
		if (vMaximumX > 999)
		{
			TempLengthOfNumbers = (WORD) ResizeXCoor(70);
			TempStaggerNumberAmount = (WORD) ResizeYCoor(22);
		}
		m_XLabel1.SetWindowPos(NULL,TempLeftSide,TempBottom + TempResizedY17,TempResizedX50,TempResizedY20,SWP_NOZORDER);
		//if (vMaximumX - vMinimumX < 3)
			m_XLabel2.SetWindowPos(NULL,TempXMiddle - ResizeXCoor(37),TempBottom + ResizeYCoor(11) + TempStaggerNumberAmount,TempResizedX50*3,TempResizedY20,SWP_NOZORDER);
		//else
		//	m_XLabel2.SetWindowPos(NULL,TempXMiddle - 37,TempBottom + 5,150,20,SWP_NOZORDER);
		m_XLabel3.SetWindowPos(NULL,TempRightSide - TempResizedX50,TempBottom + TempResizedY17,TempResizedX50,TempResizedY20,SWP_NOZORDER);


		//fill in scale values
		for (DWORD TempLoop = 0; TempLoop < vXNumberOfScaleLines; TempLoop++)
		{
			WORD TempXPosition = (WORD)(TempLeftSide + (TempLoop * vXInterval * vXScale)
				- 15 + (vXScale / 2));
			//	- (vXScale / 2));
			
			if (TempXPosition < ResizeXCoor(638))
			{
				//dc.MoveTo(TempXPosition,TempBottom - 2);
				//dc.LineTo(TempXPosition,TempBottom + 6); //lines along bottom
				switch (TempLoop)
				{
					case 0:
						m_XScale1.ShowWindow(SW_SHOW);
						m_XScale1.SetWindowPos(NULL,TempXPosition,TempBottom + TempResizedY6,TempLengthOfNumbers,TempResizedY16,SWP_NOZORDER);
						SetDlgItemText(IDC_XScale1,dtoa((TempLoop * vXInterval * vXScaleCompression) + vMinimumX,1));
					break;
					case 1:
						m_XScale2.ShowWindow(SW_SHOW);
						m_XScale2.SetWindowPos(NULL,TempXPosition,TempBottom + TempStaggerNumberAmount,TempLengthOfNumbers,TempResizedY16,SWP_NOZORDER);
						SetDlgItemText(IDC_XScale2,dtoa((TempLoop * vXInterval * vXScaleCompression) + vMinimumX,1));
					break;
					case 2:
						m_XScale3.ShowWindow(SW_SHOW);
						m_XScale3.SetWindowPos(NULL,TempXPosition,TempBottom + TempResizedY6,TempLengthOfNumbers,TempResizedY16,SWP_NOZORDER);
						SetDlgItemText(IDC_XScale3,dtoa((TempLoop * vXInterval * vXScaleCompression) + vMinimumX,1));
					break;
					case 3:
						m_XScale4.ShowWindow(SW_SHOW);
						m_XScale4.SetWindowPos(NULL,TempXPosition,TempBottom + TempStaggerNumberAmount,TempLengthOfNumbers,TempResizedY16,SWP_NOZORDER);
						SetDlgItemText(IDC_XScale4,dtoa((TempLoop * vXInterval * vXScaleCompression) + vMinimumX,1));
					break;
					case 4:
						m_XScale5.ShowWindow(SW_SHOW);
						m_XScale5.SetWindowPos(NULL,TempXPosition,TempBottom + TempResizedY6,TempLengthOfNumbers,TempResizedY16,SWP_NOZORDER);
						SetDlgItemText(IDC_XScale5,dtoa((TempLoop * vXInterval * vXScaleCompression) + vMinimumX,1));
					break;
					case 5:
						m_XScale6.ShowWindow(SW_SHOW);
						m_XScale6.SetWindowPos(NULL,TempXPosition,TempBottom + TempStaggerNumberAmount,TempLengthOfNumbers,TempResizedY16,SWP_NOZORDER);
						SetDlgItemText(IDC_XScale6,dtoa((TempLoop * vXInterval * vXScaleCompression) + vMinimumX,1));
					break;
					case 6:
						m_XScale7.ShowWindow(SW_SHOW);
						m_XScale7.SetWindowPos(NULL,TempXPosition,TempBottom + TempResizedY6,TempLengthOfNumbers,16,SWP_NOZORDER);
						SetDlgItemText(IDC_XScale7,dtoa((TempLoop * vXInterval * vXScaleCompression) + vMinimumX,1));
					break;
					case 7:
						m_XScale8.ShowWindow(SW_SHOW);
						m_XScale8.SetWindowPos(NULL,TempXPosition,TempBottom + TempStaggerNumberAmount,TempLengthOfNumbers,TempResizedY16,SWP_NOZORDER);
						SetDlgItemText(IDC_XScale8,dtoa((TempLoop * vXInterval * vXScaleCompression) + vMinimumX,1));
					break;
					case 8:
						m_XScale9.ShowWindow(SW_SHOW);
						m_XScale9.SetWindowPos(NULL,TempXPosition,TempBottom + TempResizedY6,TempLengthOfNumbers,TempResizedY16,SWP_NOZORDER);
						SetDlgItemText(IDC_XScale9,dtoa((TempLoop * vXInterval * vXScaleCompression) + vMinimumX,1));
					break;
					case 9:
						m_XScale10.ShowWindow(SW_SHOW);
						m_XScale10.SetWindowPos(NULL,TempXPosition,TempBottom + TempStaggerNumberAmount,TempLengthOfNumbers,TempResizedY16,SWP_NOZORDER);
						SetDlgItemText(IDC_XScale10,dtoa((TempLoop * vXInterval * vXScaleCompression) + vMinimumX,1));
					break;
					case 10:
						m_XScale11.ShowWindow(SW_SHOW);
						m_XScale11.SetWindowPos(NULL,TempXPosition,TempBottom + TempResizedY6,TempLengthOfNumbers,TempResizedY16,SWP_NOZORDER);
						SetDlgItemText(IDC_XScale11,dtoa((TempLoop * vXInterval * vXScaleCompression) + vMinimumX,1));
					break;
					case 11:
						m_XScale12.ShowWindow(SW_SHOW);
						m_XScale12.SetWindowPos(NULL,TempXPosition,TempBottom + TempStaggerNumberAmount,TempLengthOfNumbers,TempResizedY16,SWP_NOZORDER);
						SetDlgItemText(IDC_XScale12,dtoa((TempLoop * vXInterval * vXScaleCompression) + vMinimumX,1));
					break;
					case 12:
						m_XScale13.ShowWindow(SW_SHOW);
						m_XScale13.SetWindowPos(NULL,TempXPosition,TempBottom + TempResizedY6,TempLengthOfNumbers,TempResizedY16,SWP_NOZORDER);
						SetDlgItemText(IDC_XScale13,dtoa((TempLoop * vXInterval * vXScaleCompression) + vMinimumX,1));
					break;
					case 13:
						m_XScale14.ShowWindow(SW_SHOW);
						m_XScale14.SetWindowPos(NULL,TempXPosition,TempBottom + TempStaggerNumberAmount,TempLengthOfNumbers,TempResizedY16,SWP_NOZORDER);
						SetDlgItemText(IDC_XScale14,dtoa((TempLoop * vXInterval * vXScaleCompression) + vMinimumX,1));
					break;
				}
			}
		//	else
			//	ReportErrorMessage("Histogram Error 9",cEMailInspx,32000);
		}
		m_YScale1.ShowWindow(SW_HIDE);
		m_YScale2.ShowWindow(SW_HIDE);
		m_YScale3.ShowWindow(SW_HIDE);
		m_YScale4.ShowWindow(SW_HIDE);
		m_YScale5.ShowWindow(SW_HIDE);
		m_YScale6.ShowWindow(SW_HIDE);
		m_YScale7.ShowWindow(SW_HIDE);
		m_YScale8.ShowWindow(SW_HIDE);
		m_YScale9.ShowWindow(SW_HIDE);
		m_YScale10.ShowWindow(SW_HIDE);
		m_YScale11.ShowWindow(SW_HIDE);
		m_YScale12.ShowWindow(SW_HIDE);
		m_YScale13.ShowWindow(SW_HIDE);
		m_YScale14.ShowWindow(SW_HIDE);
		m_YScale15.ShowWindow(SW_HIDE);

		for (WORD TempLoop = 0; TempLoop < vYNumberOfScaleLines; TempLoop++)
		{
			WORD TempY = (WORD)(TempBottom  - (vYScale / 2) - 
				(((TempLoop) *  vYInterval) * vYScale));
			switch (TempLoop)
			{
				case 0:
					m_YScale1.ShowWindow(SW_SHOW);
					m_YScale1.SetWindowPos(NULL,TempLeftSide - TempResizedX12 - TempResizedX50,TempY - TempResizedY8,TempResizedX50,TempResizedY16,SWP_NOZORDER);
					SetDlgItemText(IDC_YScale1,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
				break;
				case 1:
					m_YScale2.ShowWindow(SW_SHOW);
					m_YScale2.SetWindowPos(NULL,TempLeftSide - TempResizedX12 - TempResizedX50,TempY - TempResizedY8,TempResizedX50,TempResizedY16,SWP_NOZORDER);
					SetDlgItemText(IDC_YScale2,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
				break;
				case 2:
					m_YScale3.ShowWindow(SW_SHOW);
					m_YScale3.SetWindowPos(NULL,TempLeftSide - TempResizedX12 - TempResizedX50,TempY - TempResizedY8,TempResizedX50,TempResizedY16,SWP_NOZORDER);
					SetDlgItemText(IDC_YScale3,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
				break;
				case 3:
					m_YScale4.ShowWindow(SW_SHOW);
					m_YScale4.SetWindowPos(NULL,TempLeftSide - TempResizedX12 - TempResizedX50,TempY - TempResizedY8,TempResizedX50,TempResizedY16,SWP_NOZORDER);
					SetDlgItemText(IDC_YScale4,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
				break;
				case 4:
					m_YScale5.ShowWindow(SW_SHOW);
					m_YScale5.SetWindowPos(NULL,TempLeftSide - TempResizedX12 - TempResizedX50,TempY - TempResizedY8,TempResizedX50,TempResizedY16,SWP_NOZORDER);
					SetDlgItemText(IDC_YScale5,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
				break;
				case 5:
					m_YScale6.ShowWindow(SW_SHOW);
					m_YScale6.SetWindowPos(NULL,TempLeftSide - TempResizedX12 - TempResizedX50,TempY - TempResizedY8,TempResizedX50,TempResizedY16,SWP_NOZORDER);
					SetDlgItemText(IDC_YScale6,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
				break;
				case 6:
					m_YScale7.ShowWindow(SW_SHOW);
					m_YScale7.SetWindowPos(NULL,TempLeftSide - TempResizedX12 - TempResizedX50,TempY - TempResizedY8,TempResizedX50,TempResizedY16,SWP_NOZORDER);
					SetDlgItemText(IDC_YScale7,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
				break;
				case 7:
					m_YScale8.ShowWindow(SW_SHOW);
					m_YScale8.SetWindowPos(NULL,TempLeftSide - TempResizedX12 - TempResizedX50,TempY - TempResizedY8,TempResizedX50,TempResizedY16,SWP_NOZORDER);
					SetDlgItemText(IDC_YScale8,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
				break;
				case 8:
					m_YScale9.ShowWindow(SW_SHOW);
					m_YScale9.SetWindowPos(NULL,TempLeftSide - TempResizedX12 - TempResizedX50,TempY - TempResizedY8,TempResizedX50,TempResizedY16,SWP_NOZORDER);
					SetDlgItemText(IDC_YScale9,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
				break;
				case 9:
					m_YScale10.ShowWindow(SW_SHOW);
					m_YScale10.SetWindowPos(NULL,TempLeftSide - TempResizedX12 - TempResizedX50,TempY - TempResizedY8,TempResizedX50,TempResizedY16,SWP_NOZORDER);
					SetDlgItemText(IDC_YScale10,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
				break;
				case 10:
					m_YScale11.ShowWindow(SW_SHOW);
					m_YScale11.SetWindowPos(NULL,TempLeftSide - TempResizedX12 - TempResizedX50,TempY - TempResizedY8,TempResizedX50,TempResizedY16,SWP_NOZORDER);
					SetDlgItemText(IDC_YScale11,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
				break;
				case 11:
					m_YScale12.ShowWindow(SW_SHOW);
					m_YScale12.SetWindowPos(NULL,TempLeftSide - TempResizedX12 - TempResizedX50,TempY - TempResizedY8,TempResizedX50,TempResizedY16,SWP_NOZORDER);
					SetDlgItemText(IDC_YScale12,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
				break;
				case 12:
					m_YScale13.ShowWindow(SW_SHOW);
					m_YScale13.SetWindowPos(NULL,TempLeftSide - TempResizedX12 - TempResizedX50,TempY - TempResizedY8,TempResizedX50,TempResizedY16,SWP_NOZORDER);
					SetDlgItemText(IDC_YScale13,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
				break;
				case 13:
					m_YScale14.ShowWindow(SW_SHOW);
					m_YScale14.SetWindowPos(NULL,TempLeftSide - TempResizedX12 - TempResizedX50,TempY - TempResizedY8,TempResizedX50,TempResizedY16,SWP_NOZORDER);
					SetDlgItemText(IDC_YScale14,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
				break;
				case 14:
					m_YScale15.ShowWindow(SW_SHOW);
					m_YScale15.SetWindowPos(NULL,TempLeftSide - TempResizedX12 - TempResizedX50,TempY - TempResizedY8,TempResizedX50,TempResizedY16,SWP_NOZORDER);
					SetDlgItemText(IDC_YScale15,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
				break;
			}
		}
	}
	else
	{
		SetDlgItemText(IDC_DataTitle, _T("X Data Problem or too few points to plot"));
		m_LineLabel3.ShowWindow(SW_HIDE);
		ClearWindowText();
	}
}

void CHistogramDisplayDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if (vNumberOfPointsToPlot)
	if (vMaximumX)
	if (vMaximumX >= vMinimumX)
	if (vDataArray)
	{
		WORD TempXMiddle = (WORD) ResizeXCoor(340);  //320
		WORD TempYMiddle = (WORD) ResizeYCoor(245); //240
		WORD TempWidth = (WORD)(vNumberOfPointsToPlot * vXScale/*Word*/ / 2.0);
		if (TempWidth > (WORD) ResizeXCoor(298))
		{
			TempWidth = (WORD) ResizeXCoor(298);
			ReportErrorMessage("Histogram Error 5",cEMailInspx,32000);
		}
		WORD TempHeight = (WORD)((vMaximumYToShow - vMinimumYToShow) * vYScale/*Word*/ / 2.0);
		if (TempHeight >(WORD)  ResizeYCoor(200))
		{
			TempHeight = (WORD) ResizeYCoor(200);
			ReportErrorMessage("Histogram Error 6",cEMailInspx,32000);
		}
		WORD TempLeftSide = TempXMiddle - TempWidth;//200; //106;
		if (TempLeftSide < (WORD) ResizeXCoor(8))
		{
			ReportErrorMessage("Histogram Error 7",cEMailInspx,32000);
			TempLeftSide = (WORD) ResizeXCoor(8);
		}
		WORD TempRightSide = TempXMiddle + TempWidth;//TempLeftSide + vNumberOfPointsToPlot;//529;
		WORD TempBottom = TempYMiddle + TempHeight; //170; //408;
		WORD TempTop = TempYMiddle - TempHeight;//TempBottom - (vYScale); //41;
		if (TempTop < vYScale + 2)
		{
			ReportErrorMessage("Histogram Error 8",cEMailInspx,32000);
			TempTop = 80;
		}

		int Temp1XResized = ResizeXCoor(1);
		int Temp2XResized = ResizeXCoor(2);
		int Temp2YResized = ResizeYCoor(2);
		int Temp638XResized = ResizeXCoor(638);
		//draw box around
		dc.SelectObject(vBluePen);
		dc.MoveTo(TempLeftSide - Temp2XResized,(int)(TempTop - Temp2YResized - vYScale/*Word*/));
		dc.LineTo(TempRightSide + Temp1XResized,(int)(TempTop - Temp2YResized - vYScale/*Word*/)); //top line
		dc.LineTo(TempRightSide + Temp1XResized,TempBottom + Temp2YResized); //right line
		dc.LineTo(TempLeftSide - Temp2XResized,TempBottom + Temp2YResized); //bottom line
		dc.LineTo(TempLeftSide - Temp2XResized,(int)(TempTop - Temp2YResized - vYScale/*Word*/)); //Left line

		//draw scale lines
		dc.SelectObject(vRedPen);
		for (DWORD TempLoop = 0; TempLoop < vXNumberOfScaleLines; TempLoop++)
		{
			WORD TempXPosition = (WORD)(TempLeftSide + (TempLoop * vXInterval * vXScale)
				+ (vXScale / 2));
			if (TempXPosition < Temp638XResized)
			{
				dc.MoveTo(TempXPosition,TempBottom - Temp2YResized);
				dc.LineTo(TempXPosition,TempBottom + Temp2YResized*3); //lines along bottom
			}
		}
		for (DWORD TempLoop = 0; TempLoop < vYNumberOfScaleLines; TempLoop++)
		{
			dc.SelectObject(vRedPen);
			WORD TempY = (WORD)(TempBottom  - (vYScale/*Word*/ / 2) - 
				(((TempLoop) *  vYInterval) * vYScale/*Word*/));
			dc.MoveTo(TempLeftSide - Temp2XResized*3,TempY);
			dc.LineTo(TempLeftSide + Temp2XResized,TempY); //lines along Left side

		//draw horizontal lines across the screen
			dc.SelectObject(vGreyPen);
			dc.LineTo(TempRightSide,TempY); //lines along Left side
		}

		//plot RAW data
		dc.SelectObject(vRedPen);
		if ((vPlotRawData) && (vPointerToFloats))
		{
			double *TempPointer = vPointerToFloats;
			double TempDataPoint = 0;
			for (DWORD TempLoop = 0; TempLoop < vNumberOfPointsToPlot; TempLoop++)
			{
				for (DWORD TempLoopX = 0; TempLoopX < vXScale/*Word*/; TempLoopX++)
				for (DWORD TempLoopY = 0; TempLoopY < vYScale/*Word*/; TempLoopY++)
				{ 
					TempDataPoint = *TempPointer;

					//if (vClipCounts)
				//	if (TempDataPoint > vClipCounts)
					//	TempDataPoint = (double)vClipCounts;

					if (TempDataPoint > vMaximumYToShow)
						TempDataPoint = (double)vMaximumYToShow;

					if (TempDataPoint < vMinimumYToShow)
						TempDataPoint = (double)vMinimumYToShow;

					WORD TempX = (WORD)(TempLeftSide + (TempLoop * vXScale/*Word*/) + TempLoopX);
					WORD TempY = (WORD)(TempBottom - ((TempDataPoint - vMinimumYToShow) * vYScale/*Word*/) - //was vMinimumYDouble
						TempLoopY);
					if (TempX > TempRightSide + 1)
					{
						ReportErrorMessage("Histogram Error 3",cEMailInspx,32000);
						TempX = TempRightSide;
					}
					if (TempY > TempBottom + 1)
					if (!vPlotRawData)
					{
						ReportErrorMessage("Histogram Error 4",cEMailInspx,32000);
						TempY = TempBottom;
					}
					dc.SetPixelV(TempX,TempY,cBlack);
				}
				TempPointer++;
			}
			TempPointer = vPointerToFloats;
			for (DWORD TempLoop = 0; TempLoop < vNumberOfPointsToPlot; TempLoop++)
			{//draw the lines through the data points
				TempDataPoint = *TempPointer;

				//if (vClipCounts)
				//if (TempDataPoint < vClipCounts)
				//	TempDataPoint = (double)vClipCounts;

				if (TempDataPoint > vMaximumYToShow)
					TempDataPoint = (double)vMaximumYToShow;

				if (TempDataPoint < vMinimumYToShow)
					TempDataPoint = (double)vMinimumYToShow;

				WORD TempX = (WORD)(TempLeftSide + ((TempLoop * vXScale/*Word*/) + (vXScale / 2)));
				WORD TempY = (WORD)(TempBottom - ((TempDataPoint - vMinimumYToShow) * vYScale/*Word*/ + (vYScale / 2)));//was vMinimumYDouble
				if (TempX > TempRightSide + 1)
				{
					ReportErrorMessage("Histogram Error 11a",cEMailInspx,32000);
					TempX = TempRightSide;
				}
				if (TempY > TempBottom + 1)
				if (!vPlotRawData)
				{
					ReportErrorMessage("Histogram Error 10a",cEMailInspx,32000);
					TempY = TempBottom;
				}
				if (TempLoop == 0)
					dc.MoveTo(TempX,TempY);
				else
					dc.LineTo(TempX,TempY); //lines through data
				TempPointer++;
			}
			if (vCurveFitDataArray && vShowCurveFitDataLine) //draw the fitted curve on the screen in blue
			{	
				dc.SelectObject(vBluePen);
				BYTE *TempCurvePointer = vCurveFitDataArray + vMinimumX;
				bool TempFirtsOne = true;
				for (DWORD TempLoop = 0; TempLoop < vNumberOfPointsToPlot - 1; TempLoop++)
				{//draw the lines through the data points

					if ((*TempCurvePointer) && (*TempCurvePointer < 255))
					{
						TempDataPoint = *TempCurvePointer;
						if (TempDataPoint > vMaximumYToShow)
							TempDataPoint = (double)vMaximumYToShow;

						if (TempDataPoint < vMinimumYToShow)
							TempDataPoint = (double)vMinimumYToShow;

						WORD TempX = (WORD)(TempLeftSide + (((TempLoop + 1) * vXScale/*Word*/) + (vXScale / 2))); // ??? + 1
						WORD TempY = (WORD)(TempBottom - ((TempDataPoint - vMinimumYToShow) * vYScale/*Word*/ + (vYScale / 2)));//was vMinimumYDouble
						if (TempX >= TempRightSide)
						{
							ReportErrorMessage("Histogram Error 11b",cEMailInspx,32000);
							TempX = TempRightSide;
						}
						if (TempY > TempBottom)
						if (!vPlotRawData)
						{
							ReportErrorMessage("Histogram Error 10b",cEMailInspx,32000);
							TempY = TempBottom;
						}
						if (TempFirtsOne)
						{
							dc.MoveTo(TempX,TempY);
							TempFirtsOne = false;
						}
						else
							dc.LineTo(TempX,TempY); //lines through data
					}
					TempCurvePointer++;
				}
			}
		}
		else
		{		//plot HISTOGRAM data
			int Temp6XResized = ResizeXCoor(6);
			int Temp6YResized = ResizeYCoor(6);
			DWORD *TempPointer = vDataArray;
			DWORD TempDataPoint = 0;
			double TempDotSizeX = vXScale;
			double TempDotSizeY = vYScale;
			if (TempDotSizeX > Temp6XResized)
				TempDotSizeX = Temp6XResized;
			if (TempDotSizeY > Temp6YResized)
				TempDotSizeY = Temp6YResized;
			int TempDotOffsetX = (int)(vXScale - TempDotSizeX) / 2;
			int TempDotOffsetY = (int)(vYScale - TempDotSizeX) / 2;

			for (DWORD TempLoop = 0; TempLoop < vNumberOfPointsToPlot; TempLoop++)
			{
				for (DWORD TempLoopX = 0; TempLoopX < TempDotSizeX/*Word*/; TempLoopX++)
				for (DWORD TempLoopY = 0; TempLoopY < TempDotSizeY/*Word*/; TempLoopY++)
				{
					TempDataPoint = *TempPointer;
					if (TempDataPoint > vMaximumYToShow)
						TempDataPoint = vMaximumYToShow;
					WORD TempX = (WORD)(TempLeftSide + (TempLoop * vXScale/*Word*/) + TempLoopX);
					WORD TempY = (WORD)(TempBottom - ((TempDataPoint - vMinimumYToShow) * vYScale/*Word*/) - TempLoopY);
					if (TempX >= TempRightSide)
					{
						ReportErrorMessage("Histogram Error 3a",cEMailInspx,32000);
						TempX = TempRightSide;
					}
					if (TempY > TempBottom)
					if (!vPlotRawData)
					{
						ReportErrorMessage("Histogram Error 4",cEMailInspx,32000);
						TempY = TempBottom;
					}
					dc.SetPixelV(TempX + TempDotOffsetX,TempY - TempDotOffsetY,cBlack);
				}
				TempPointer++;
			}
			TempPointer = vDataArray;
			for (DWORD TempLoop = 0; TempLoop < vNumberOfPointsToPlot; TempLoop++)
			{//draw the lines through the data points
				TempDataPoint = *TempPointer;

				if (TempDataPoint > vMaximumYToShow)
					TempDataPoint = vMaximumYToShow;

				if (TempDataPoint < vMinimumYToShow)
					TempDataPoint = vMinimumYToShow;

				WORD TempX = (WORD)(TempLeftSide + ((TempLoop * vXScale/*Word*/) + (vXScale / 2)));
				WORD TempY = (WORD)(TempBottom - ((TempDataPoint - vMinimumYToShow) * vYScale/*Word*/ + (vYScale / 2)));
				if (TempX > TempRightSide)
				{
					ReportErrorMessage("Histogram Error 11c",cEMailInspx,32000);
					TempX = TempRightSide;
				}
				if (TempY > TempBottom)
				if (!vPlotRawData)
				{
					ReportErrorMessage("Histogram Error 10c",cEMailInspx,32000);
					TempY = TempBottom;
				}
				if ((TempLoop == 0) && (vNumberOfPointsToPlot != 1))
					dc.MoveTo(TempX,TempY);
				else
					dc.LineTo(TempX,TempY); //lines through data
				TempPointer++;
			}
		}
	}
	// Do not call CDialog::OnPaint() for painting messages
}

void CHistogramDisplayDialog::ClearWindowText()
{
	m_LineLabel1.ShowWindow(false);
	m_LineLabel6.ShowWindow(false);
	m_LineLabel2.ShowWindow(false);
	m_LineLabel4.ShowWindow(false);
	m_XLabel1.ShowWindow(false);
	m_XLabel3.ShowWindow(false);
	m_XLabel2.ShowWindow(false);
	m_TotalSamples.ShowWindow(false);
}

void CHistogramDisplayDialog::ShowInspxButtons()
{
	if ((vLocalSystemData->vInAutoSetup) &&
		(PasswordOK(cTemporaryInspxPassword,false)))
	{
		SetDlgItemText(IDC_Function5Button, _T("Rejects"));
		m_Function5Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_LineLabel2, _T("Y is Number of value"));
	}
	else
	{
		m_Function5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
		SetDlgItemText(IDC_LineLabel2, _T("Y is number of occurrences of value"));
	}
}

void CHistogramDisplayDialog::DisplayRawDataPlot()
{
	vMinimumYDouble = (DWORD)0xFFFFFFFFFFFFFFFF;
	vMaximumYDouble = 0;
	vMaximumX = (WORD)vNumberOfPointsToPlot;
	vMinimumX = 1;
	double *TempPointer = vPointerToFloats;
	DWORD *TempTimes = vPointerToTimes;
	for (DWORD TempLoop = 0; TempLoop < vNumberOfNumbers; TempLoop++)
	{  //find the minimum and maximum values of the data
		if (*TempPointer < vMinimumYDouble)
			vMinimumYDouble = *TempPointer;
		if (*TempPointer > vMaximumYDouble)
			vMaximumYDouble = *TempPointer;
		TempPointer++;
	}
	//vMaximumYDouble++;
	vMaximumYToShow = int(vMaximumYDouble);
	vMinimumYToShow = int(vMinimumYDouble);

	if (vMinimumYToShow + 15 > vMaximumYToShow)
	{
		m_Function2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());
	}

	if (vClipCounts)
	{
		double TempCountsToMaximum = vMaximumYToShow - vClipCounts;
		double TempCountsToMinimum = vClipCounts - vMinimumYToShow;

		if ((TempCountsToMaximum > 0) && (TempCountsToMaximum < TempCountsToMinimum))
			vMinimumYToShow = (WORD)vClipCounts;
		else
			vMaximumYToShow = (WORD)vClipCounts;

		if (vMaximumYToShow < vMinimumYToShow)
			vMaximumYToShow = vMinimumYToShow + 2;
	}

	if (vMaximumYToShow >= vMinimumYToShow)
	{
		vNumberOfPointsToPlot = vNumberOfNumbers;
		if (vNumberOfPointsToPlot)
		{  //create a temporary array to contain the number of occurrences of each possible value
			//vXScale =  ResizeXCoor((int) (570.0 / (double) vNumberOfPointsToPlot));
			vXScale = ((double)ResizeXCoor(570)) / ((double)vNumberOfPointsToPlot);
			int TempResizedX50 =  ResizeXCoor(50);
			int TempResizedY50 =  ResizeYCoor(50);
			if ((vMaximumYToShow - vMinimumYToShow) > 6)
				vYScale = (double) ResizeYCoor((int) (30000.0 / (double)(vMaximumYToShow - vMinimumYToShow))) / 100;
			else
				vYScale =TempResizedY50;
			if (vXScale > TempResizedX50)
				vXScale = TempResizedX50;
			if (vYScale > TempResizedY50)
				vYScale = TempResizedY50;

			vXScaleWord = (WORD)(vXScale);  //truncate

			vXInterval = (WORD)(ResizeXCoor((int)((((double) vNumberOfPointsToPlot) / 10.0) + .5)));
			vYInterval = (WORD)(ResizeYCoor((int)((((double) (vMaximumYToShow - vMinimumYToShow)) / 10.0) + .5)));

			if (!vXInterval)
				vXInterval = 1;

			if (!vYInterval)
				vYInterval = 1;

			vXNumberOfScaleLines = (int)((vNumberOfNumbers) / vXInterval);
			vYNumberOfScaleLines = (WORD)((vMaximumYToShow - vMinimumYToShow) / vYInterval + 1);

			if ((vXNumberOfScaleLines) * vXInterval + vMinimumX <= vMaximumX)
				vXNumberOfScaleLines++;

			if ((vMaximumYToShow > vMinimumYToShow) && (vMaximumYToShow > vMinimumYToShow + ((vYNumberOfScaleLines - 1) * vYInterval)))
			{
				m_LineLabel1.ShowWindow(SW_SHOW);
				SetDlgItemText(IDC_LineLabel1, dtoa(vMaximumYToShow, 0));
			}
			else
			{
				m_LineLabel1.ShowWindow(SW_HIDE);
				SetDlgItemText(IDC_LineLabel1, _T(""));
			}
			SetDlgItemText(IDC_LineLabel6, _T(""));
			
			CString TempString = dtoa(vYInterval, 0);
			if (vACPowerMoniterFormat)
			{
				if (vYInterval == 1)
					TempString = TempString + " Volt";
				else
					TempString = TempString + " Volts";
			}

			if (vXScaleIsTime)
				SetDlgItemText(IDC_LineLabel3,"Y Scale: " + TempString + "`C");
			else
				SetDlgItemText(IDC_LineLabel3,"Y Scale: " + TempString);

			SetDlgItemText(IDC_XLabel1, _T(""));
			if ((vMaximumX > vMinimumX) && (vMaximumX > vMinimumX + ((vXNumberOfScaleLines - 1) * vXInterval)))
			{
				TempString = dtoa(vMaximumX, 0);
				if (vACPowerMoniterFormat)
				{
					if(TempString == "1")
						TempString = TempString + " Volt";
					else
						TempString = TempString + " Volts";
				}
				SetDlgItemText(IDC_XLabel3,TempString);
			}
			else
				SetDlgItemText(IDC_XLabel3, _T(""));
			
			if (vACPowerMoniterFormat == true)
				SetDlgItemText(IDC_XLabel2, _T("X Scale: Hrs:Mins"));
			else
				SetDlgItemText(IDC_XLabel2, "X Scale: " + dtoa(vXInterval, 0));
			
			WORD TempXMiddle = (WORD) ResizeXCoor(340);  //320
			WORD TempYMiddle = (WORD) ResizeYCoor(245); //240
			WORD TempWidth = (WORD)(vNumberOfNumbers * vXScale / 2.0);
			int TempResizedX298 = ResizeXCoor(298);
			if (TempWidth > TempResizedX298)
			{
				TempWidth = TempResizedX298;
				ReportErrorMessage("Histogram Error 5",cEMailInspx,32000);
			}
			WORD TempHeight = (WORD)((vMaximumYToShow - vMinimumYToShow) * vYScale / 2.0);
			int TempResizedY200 = ResizeYCoor(200);
			if (TempHeight > TempResizedY200)
			{
				TempHeight = TempResizedY200;
				ReportErrorMessage("Histogram Error 6",cEMailInspx,32000);
			}
			WORD TempLeftSide = TempXMiddle - TempWidth;//200; //106;
			int TempResizedX8 = ResizeXCoor(8);
			if (TempLeftSide < TempResizedX8)
			{
				ReportErrorMessage("Histogram Error 7",cEMailInspx,32000);
				TempLeftSide = TempResizedX8;
			}
			WORD TempRightSide = TempXMiddle + TempWidth;//TempLeftSide + vNumberOfNumbers;//529;
			WORD TempBottom = TempYMiddle + TempHeight; //170; //408;
			WORD TempTop = TempYMiddle - TempHeight;//TempBottom - (vYScale); //41;
			if (TempTop < vYScale + ResizeYCoor(2))
			{
				ReportErrorMessage("Histogram Error 8",cEMailInspx,32000);
				TempTop = (WORD) ResizeYCoor(80);
			}
			//	WORD TempY = (WORD)(TempBottom  - (vYScale / 2) - 
			//		(((TempLoop) *  vYInterval) * vYScale));
			int TempResizedY20 = ResizeYCoor(20);
			int TempResizedX60 = ResizeXCoor(60);
			int TempResizedX17 = ResizeXCoor(17);
			m_LineLabel6.SetWindowPos(NULL,TempLeftSide - TempResizedX60,(int)(TempBottom - (vYScale / 2)),TempResizedX50,TempResizedY20,SWP_NOZORDER);
			m_LineLabel3.SetWindowPos(NULL,ResizeXCoor(10),ResizeYCoor(10),ResizeXCoor(110),TempResizedY20,SWP_NOZORDER);
			//m_LineLabel1 is maximum Y
			m_LineLabel1.SetWindowPos(NULL,TempLeftSide - 12 - TempResizedX50,
				(WORD)(TempBottom  - (((vYNumberOfScaleLines - 1) *  vYInterval) * vYScale)
				- (vYScale / 2) - ResizeYCoor(25))
				,50,20,SWP_NOZORDER);
				//(int)(TempTop - vYScale - 10),50,20,SWP_NOZORDER);

			m_XLabel1.SetWindowPos(NULL,TempLeftSide,TempBottom + TempResizedX17,TempResizedX50,TempResizedY20,SWP_NOZORDER);
			//if (vMaximumX - vMinimumX < 3)
			m_XLabel2.SetWindowPos(NULL,TempXMiddle - ResizeXCoor(37),TempBottom + TempResizedX17,TempResizedX50*3,TempResizedY20,SWP_NOZORDER);
			//else
			//	m_XLabel2.SetWindowPos(NULL,TempXMiddle - 37,TempBottom + 5,150,20,SWP_NOZORDER);
			m_XLabel3.SetWindowPos(NULL,TempRightSide - TempResizedX50,TempBottom + TempResizedX17,TempResizedX50,TempResizedY20,SWP_NOZORDER);


			//fill in scale values
			if (!vMinimumX)
				vMinimumX = 1;
			for (DWORD TempLoop = 0; TempLoop < vXNumberOfScaleLines; TempLoop++)
			{
				WORD TempXPosition = (WORD)(TempLeftSide + (TempLoop * vXInterval * vXScale) - 15 + (vXScale / 2));
				//	- (vXScale / 2));
				if (TempXPosition < ResizeXCoor(638))
				{
					int TempSizeOfNumberBox = 30;
					if(TempLoop * vXInterval > 999)
					{
						TempSizeOfNumberBox = 40;
						if(TempLoop * vXInterval > 9999)
							TempSizeOfNumberBox = 50;
					}

					CString TempXScaleString = dtoa((TempLoop * vXInterval) + vMinimumX,1);

					if (vXScaleIsTime)
					{
						int TempSecondsAhead = ((vXNumberOfScaleLines - TempLoop) * vXInterval) * 10; //each temperature point is 10 seconds apart
						if (TempSecondsAhead < 0)
							TempSecondsAhead = 0;

						CTime TempTime = CTime::GetCurrentTime();
						CString TempSecondsString = TempTime.Format("%S");
						int TempSeconds = _wtoi(TempSecondsString);
						TempSeconds = TempSeconds % 10; //take mod 10 of seconds so can remove odd seconds from display

						CTimeSpan TempSecondsAheadTime = TempSecondsAhead + TempSeconds;

						TempTime = TempTime - TempSecondsAheadTime; //each interval is 6 seconds, convert from seconds to milliseconds

						if (vXInterval > 2)
						{
							TempXScaleString = TempTime.Format("%H:%M");
							SetDlgItemText(IDC_XLabel2, _T("X Scale: Hrs:Mins"));
						}
						else
						{
							TempXScaleString = TempTime.Format("%M:%S");
							SetDlgItemText(IDC_XLabel2, _T("X Scale: Mins:Secs"));
						}

						TempSizeOfNumberBox = 40;
					}
					else
					if (vACPowerMoniterFormat && vPointerToTimes)
					{
						int TempPointerOffset = (TempLoop * vXInterval);
						DWORD TempTime = *(TempPointerOffset+vPointerToTimes);
						TempXScaleString = dtoa(TempTime/3600) + ":";
						if(TempXScaleString.GetLength() == 2)
							TempXScaleString = "0"+ TempXScaleString;
						TempXScaleString = TempXScaleString + dtoa((TempTime%3600)/60);
						TempSizeOfNumberBox = 50;
						if(TempXScaleString.GetLength() == 4)
							TempXScaleString.Insert(3, _T("0"));
					}

					int TempResizedY6 = ResizeYCoor(6);
					//dc.MoveTo(TempXPosition,TempBottom - 2);
					//dc.LineTo(TempXPosition,TempBottom + 6); //lines along bottom
					switch (TempLoop)
					{
						case 0:
							m_XScale1.ShowWindow(SW_SHOW);
							m_XScale1.SetWindowPos(NULL,TempXPosition,TempBottom + TempResizedY6, ResizeXCoor(TempSizeOfNumberBox), ResizeYCoor(16), SWP_NOZORDER);
							SetDlgItemText(IDC_XScale1,TempXScaleString);
						break;
						case 1:
							m_XScale2.ShowWindow(SW_SHOW);
							m_XScale2.SetWindowPos(NULL,TempXPosition,TempBottom + TempResizedY6, ResizeXCoor(TempSizeOfNumberBox), ResizeYCoor(16), SWP_NOZORDER);
							SetDlgItemText(IDC_XScale2,TempXScaleString);
						break;
						case 2:
							m_XScale3.ShowWindow(SW_SHOW);
							m_XScale3.SetWindowPos(NULL,TempXPosition,TempBottom + TempResizedY6, ResizeXCoor(TempSizeOfNumberBox), ResizeYCoor(16), SWP_NOZORDER);
							SetDlgItemText(IDC_XScale3,TempXScaleString);
						break;
						case 3:
							m_XScale4.ShowWindow(SW_SHOW);
							m_XScale4.SetWindowPos(NULL,TempXPosition,TempBottom + TempResizedY6, ResizeXCoor(TempSizeOfNumberBox), ResizeYCoor(16), SWP_NOZORDER);
							SetDlgItemText(IDC_XScale4,TempXScaleString);
						break;
						case 4:
							m_XScale5.ShowWindow(SW_SHOW);
							m_XScale5.SetWindowPos(NULL,TempXPosition,TempBottom + TempResizedY6, ResizeXCoor(TempSizeOfNumberBox), ResizeYCoor(16), SWP_NOZORDER);
							SetDlgItemText(IDC_XScale5,TempXScaleString);
						break;
						case 5:
							m_XScale6.ShowWindow(SW_SHOW);
							m_XScale6.SetWindowPos(NULL,TempXPosition,TempBottom + TempResizedY6, ResizeXCoor(TempSizeOfNumberBox), ResizeYCoor(16), SWP_NOZORDER);
							SetDlgItemText(IDC_XScale6,TempXScaleString);
						break;
						case 6:
							m_XScale7.ShowWindow(SW_SHOW);
							m_XScale7.SetWindowPos(NULL,TempXPosition,TempBottom + TempResizedY6, ResizeXCoor(TempSizeOfNumberBox), ResizeYCoor(16), SWP_NOZORDER);
							SetDlgItemText(IDC_XScale7,TempXScaleString);
						break;
						case 7:
							m_XScale8.ShowWindow(SW_SHOW);
							m_XScale8.SetWindowPos(NULL,TempXPosition,TempBottom + TempResizedY6, ResizeXCoor(TempSizeOfNumberBox), ResizeYCoor(16), SWP_NOZORDER);
							SetDlgItemText(IDC_XScale8,TempXScaleString);
						break;
						case 8:
							m_XScale9.ShowWindow(SW_SHOW);
							m_XScale9.SetWindowPos(NULL,TempXPosition,TempBottom + TempResizedY6, ResizeXCoor(TempSizeOfNumberBox), ResizeYCoor(16), SWP_NOZORDER);
							SetDlgItemText(IDC_XScale9,TempXScaleString);
						break;
						case 9:
							m_XScale10.ShowWindow(SW_SHOW);
							m_XScale10.SetWindowPos(NULL,TempXPosition,TempBottom + TempResizedY6, ResizeXCoor(TempSizeOfNumberBox), ResizeYCoor(16), SWP_NOZORDER);
							SetDlgItemText(IDC_XScale10,TempXScaleString);
						break;
						case 10:
							m_XScale11.ShowWindow(SW_SHOW);
							m_XScale11.SetWindowPos(NULL,TempXPosition,TempBottom + TempResizedY6, ResizeXCoor(TempSizeOfNumberBox), ResizeYCoor(16), SWP_NOZORDER);
							SetDlgItemText(IDC_XScale11,TempXScaleString);
						break;
						case 11:
							m_XScale12.ShowWindow(SW_SHOW);
							m_XScale12.SetWindowPos(NULL,TempXPosition,TempBottom + TempResizedY6, ResizeXCoor(TempSizeOfNumberBox), ResizeYCoor(16), SWP_NOZORDER);
							SetDlgItemText(IDC_XScale12,TempXScaleString);
						break;
						case 12:
							m_XScale13.ShowWindow(SW_SHOW);
							m_XScale13.SetWindowPos(NULL,TempXPosition,TempBottom + TempResizedY6, ResizeXCoor(TempSizeOfNumberBox), ResizeYCoor(16), SWP_NOZORDER);
							SetDlgItemText(IDC_XScale13,TempXScaleString);
						break;
						case 13:
							m_XScale14.ShowWindow(SW_SHOW);
							m_XScale14.SetWindowPos(NULL,TempXPosition,TempBottom + TempResizedY6, ResizeXCoor(TempSizeOfNumberBox), ResizeYCoor(16), SWP_NOZORDER);
							SetDlgItemText(IDC_XScale14,TempXScaleString);
						break;
					}
				}
			//	else
				//	ReportErrorMessage("Histogram Error 9",cEMailInspx,32000);
			}
			m_YScale1.ShowWindow(SW_HIDE);
			m_YScale2.ShowWindow(SW_HIDE);
			m_YScale3.ShowWindow(SW_HIDE);
			m_YScale4.ShowWindow(SW_HIDE);
			m_YScale5.ShowWindow(SW_HIDE);
			m_YScale6.ShowWindow(SW_HIDE);
			m_YScale7.ShowWindow(SW_HIDE);
			m_YScale8.ShowWindow(SW_HIDE);
			m_YScale9.ShowWindow(SW_HIDE);
			m_YScale10.ShowWindow(SW_HIDE);
			m_YScale11.ShowWindow(SW_HIDE);
			m_YScale12.ShowWindow(SW_HIDE);
			m_YScale13.ShowWindow(SW_HIDE);
			m_YScale14.ShowWindow(SW_HIDE);
			m_YScale15.ShowWindow(SW_HIDE);

			for (DWORD TempLoop = 0; TempLoop < vYNumberOfScaleLines; TempLoop++)
			{
				WORD TempY = (WORD)(TempBottom  - (vYScale / 2) - 
					(((TempLoop) *  vYInterval) * vYScale));
				switch (TempLoop)
				{
					case 0:
						m_YScale1.ShowWindow(SW_SHOW);
						m_YScale1.SetWindowPos(NULL, TempLeftSide - ResizeXCoor(62), TempY - ResizeYCoor(8), ResizeXCoor(50), ResizeYCoor(16), SWP_NOZORDER);
						SetDlgItemText(IDC_YScale1,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
					break;
					case 1:
						m_YScale2.ShowWindow(SW_SHOW);
						m_YScale2.SetWindowPos(NULL, TempLeftSide - ResizeXCoor(62), TempY - ResizeYCoor(8), ResizeXCoor(50), ResizeYCoor(16), SWP_NOZORDER);
						SetDlgItemText(IDC_YScale2,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
					break;
					case 2:
						m_YScale3.ShowWindow(SW_SHOW);
						m_YScale3.SetWindowPos(NULL, TempLeftSide - ResizeXCoor(62), TempY - ResizeYCoor(8), ResizeXCoor(50), ResizeYCoor(16), SWP_NOZORDER);
						SetDlgItemText(IDC_YScale3,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
					break;
					case 3:
						m_YScale4.ShowWindow(SW_SHOW);
						m_YScale4.SetWindowPos(NULL, TempLeftSide - ResizeXCoor(62), TempY - ResizeYCoor(8), ResizeXCoor(50), ResizeYCoor(16), SWP_NOZORDER);
						SetDlgItemText(IDC_YScale4,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
					break;
					case 4:
						m_YScale5.ShowWindow(SW_SHOW);
						m_YScale5.SetWindowPos(NULL, TempLeftSide - ResizeXCoor(62), TempY - ResizeYCoor(8), ResizeXCoor(50), ResizeYCoor(16), SWP_NOZORDER);
						SetDlgItemText(IDC_YScale5,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
					break;
					case 5:
						m_YScale6.ShowWindow(SW_SHOW);
						m_YScale6.SetWindowPos(NULL, TempLeftSide - ResizeXCoor(62), TempY - ResizeYCoor(8), ResizeXCoor(50), ResizeYCoor(16), SWP_NOZORDER);
						SetDlgItemText(IDC_YScale6,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
					break;
					case 6:
						m_YScale7.ShowWindow(SW_SHOW);
						m_YScale7.SetWindowPos(NULL, TempLeftSide - ResizeXCoor(62), TempY - ResizeYCoor(8), ResizeXCoor(50), ResizeYCoor(16), SWP_NOZORDER);
						SetDlgItemText(IDC_YScale7,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
					break;
					case 7:
						m_YScale8.ShowWindow(SW_SHOW);
						m_YScale8.SetWindowPos(NULL, TempLeftSide - ResizeXCoor(62), TempY - ResizeYCoor(8), ResizeXCoor(50), ResizeYCoor(16), SWP_NOZORDER);
						SetDlgItemText(IDC_YScale8,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
					break;
					case 8:
						m_YScale9.ShowWindow(SW_SHOW);
						m_YScale9.SetWindowPos(NULL, TempLeftSide - ResizeXCoor(62), TempY - ResizeYCoor(8), ResizeXCoor(50), ResizeYCoor(16), SWP_NOZORDER);
						SetDlgItemText(IDC_YScale9,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
					break;
					case 9:
						m_YScale10.ShowWindow(SW_SHOW);
						m_YScale10.SetWindowPos(NULL, TempLeftSide - ResizeXCoor(62), TempY - ResizeYCoor(8), ResizeXCoor(50), ResizeYCoor(16), SWP_NOZORDER);
						SetDlgItemText(IDC_YScale10,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
					break;
					case 10:
						m_YScale11.ShowWindow(SW_SHOW);
						m_YScale11.SetWindowPos(NULL, TempLeftSide - ResizeXCoor(62), TempY - ResizeYCoor(8), ResizeXCoor(50), ResizeYCoor(16), SWP_NOZORDER);
						SetDlgItemText(IDC_YScale11,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
					break;
					case 11:
						m_YScale12.ShowWindow(SW_SHOW);
						m_YScale12.SetWindowPos(NULL, TempLeftSide - ResizeXCoor(62), TempY - ResizeYCoor(8), ResizeXCoor(50), ResizeYCoor(16), SWP_NOZORDER);
						SetDlgItemText(IDC_YScale12,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
					break;
					case 12:
						m_YScale13.ShowWindow(SW_SHOW);
						m_YScale13.SetWindowPos(NULL, TempLeftSide - ResizeXCoor(62), TempY - ResizeYCoor(8), ResizeXCoor(50), ResizeYCoor(16), SWP_NOZORDER);
						SetDlgItemText(IDC_YScale13,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
					break;
					case 13:
						m_YScale14.ShowWindow(SW_SHOW);
						m_YScale14.SetWindowPos(NULL, TempLeftSide - ResizeXCoor(62), TempY - ResizeYCoor(8), ResizeXCoor(50), ResizeYCoor(16), SWP_NOZORDER);
						SetDlgItemText(IDC_YScale14,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
					break;
					case 14:
						m_YScale15.ShowWindow(SW_SHOW);
						m_YScale15.SetWindowPos(NULL, TempLeftSide - ResizeXCoor(62), TempY - ResizeYCoor(8), ResizeXCoor(50), ResizeYCoor(16), SWP_NOZORDER);
						SetDlgItemText(IDC_YScale15,dtoa((TempLoop * vYInterval) + vMinimumYToShow,1));
					break;
				}
			}

			//plot data
			vMaximumX = (DWORD)vNumberOfPointsToPlot;

			vDataArray = (DWORD *)malloc(vNumberOfPointsToPlot * sizeof(DWORD));
			if (vDataArray)
			{
				vCreatedDataArray = true;
				DWORD *TempDestination = vDataArray;
				double *TempSource = vPointerToFloats;
				for (WORD TempLoop = 0; TempLoop < vNumberOfPointsToPlot; TempLoop++)
					*TempDestination++ = (int)*TempSource++;

				if (vCurveFitInspection)
				{
					if (vCurveFitDataArray)
						free(vCurveFitDataArray);

					vCurveFitDataArray = (BYTE *)malloc(vNumberOfPointsToPlot);
					if (vCurveFitDataArray)
					{
						BYTE *TempDestination = vCurveFitDataArray;
						double *TempSource = vPointerToFloats;
						for (WORD TempLoop = 0; TempLoop < vNumberOfPointsToPlot; TempLoop++)
						{
							*TempDestination = (BYTE)*TempSource;
							TempDestination++;
							TempSource++;
						}

						double vStartTime = CIFCOS::GetSystimeMicrosecs();
						vCurveFitAnalysisData.vTime = 0;
						vCurveFitAnalysisData.vCallsToSquare = 0;
						vCurveFitAnalysisData.vMainCalls = 0;
						vCurveFitAnalysisData.vNumberOfPointsToPlot = 0;
						for (DWORD TempLoop = 0; TempLoop < 10; TempLoop++)
							vCurveFitAnalysisData.vTerms[TempLoop] = 0;
						vCurveFitAnalysisData.vTerminationReason = -1;
						vCurveFitAnalysisData.vFNorm = 0;

						vCurveFitInspection->ReturnCurveFitLine(vCurveFitDataArray, (WORD)vNumberOfPointsToPlot, &vCurveFitAnalysisData, 
							vGlobalCurveFitEquationOrder);
						double vEndTime = CIFCOS::GetSystimeMicrosecs();
						vCurveFitAnalysisData.vTime = vEndTime - vStartTime;
					}
				}
			}
		}
	}
	else
	{
		SetDlgItemText(IDC_DataTitle, _T("X Data Problem or too few points to plot"));
		m_LineLabel3.ShowWindow(SW_HIDE);
		ClearWindowText();
	}
}

void CHistogramDisplayDialog::ShowCurveFitAnalysisData()
{
	if (vCurveFitDataArray)
	{

		CString TempText = "";

		TempText = TempText + "\nNumber Of Points Fit: " + dtoa(vCurveFitAnalysisData.vNumberOfPointsToPlot,0);
		TempText = TempText + "\nTime To Fit: " + dtoa(vCurveFitAnalysisData.vTime,6) + " microSeconds";

		TempText = TempText + "\n\nNumber Of Calls To Square: " + dtoa(vCurveFitAnalysisData.vCallsToSquare,0);
		TempText = TempText + "\nNumber Of Main Loops: " + dtoa(vCurveFitAnalysisData.vMainCalls,0);

		//square
		//TempText = TempText + "\n\nEquation: " + dtoa(vCurveFitAnalysisData.vTerms[2],4) + "*X^2 + " + 
		//	dtoa(vCurveFitAnalysisData.vTerms[1],4) + "*X + " + dtoa(vCurveFitAnalysisData.vTerms[0],4);

		//cubic
		TempText = TempText + "\n\nEquation: ";
		if (vGlobalCurveFitEquationOrder > 9)
			TempText = TempText + dtoa(vCurveFitAnalysisData.vTerms[9],9) + "*X^9 + ";
		if (vGlobalCurveFitEquationOrder > 8)
			TempText = TempText + dtoa(vCurveFitAnalysisData.vTerms[8],9) + "*X^8 + ";
		if (vGlobalCurveFitEquationOrder > 7)
			TempText = TempText + dtoa(vCurveFitAnalysisData.vTerms[7],9) + "*X^7 + ";
		if (vGlobalCurveFitEquationOrder > 6)
			TempText = TempText + dtoa(vCurveFitAnalysisData.vTerms[6],9) + "*X^6 + ";
		if (vGlobalCurveFitEquationOrder > 5)
			TempText = TempText + dtoa(vCurveFitAnalysisData.vTerms[5],9) + "*X^5 + ";
		if (vGlobalCurveFitEquationOrder > 4)
			TempText = TempText + dtoa(vCurveFitAnalysisData.vTerms[4],9) + "*X^4 + ";
		if (vGlobalCurveFitEquationOrder > 3)
			TempText = TempText + dtoa(vCurveFitAnalysisData.vTerms[3],9) + "*X^3 + ";

		TempText = TempText + dtoa(vCurveFitAnalysisData.vTerms[2],4) + "*X^2 + " + 
			dtoa(vCurveFitAnalysisData.vTerms[1],4) + "*X + " + dtoa(vCurveFitAnalysisData.vTerms[0],4);

		TempText = TempText + "\nFNorm: " + dtoa(vCurveFitAnalysisData.vFNorm,4);

		TempText = TempText + "\n\nTermination Reason: ";

		switch (vCurveFitAnalysisData.vTerminationReason)
		{
			case 0:
				TempText = TempText + "Failed, Bad Input Parameters";
			break;
			case 1:
				TempText = TempText + "both actual and predicted relative reductions in the sum of squares are at most ftol";
			break;
			case 2:
				TempText = TempText + "relative error between two consecutive iterates is at most xtol";
			break;
			case 3:
				TempText = TempText + "both actual and predicted relative reductions in the sum of squares are at most ftol, and relative error between two consecutive iterates is at most xtol";
			break;
			case 4:
				TempText = TempText + "the cosine of the angle between fvec and any column of the jacobian is at most gtol in absolute value";
			break;
			case 5:
				TempText = TempText + "number of calls to lm_fcn has reached or exceeded maxfev";
			break;
			case 6:
				TempText = TempText + "ftol is too small. no further reduction in the sum of squares is possible";
			break;
			case 7:
				TempText = TempText + "xtol is too small. no further improvement in the approximate solution x is possible";
			break;
			case 8:
				TempText = TempText + "gtol is too small. fvec is orthogonal to the columns of the jacobian to machine precision";
			break;
			case 9:
				TempText = TempText + "Failed, parameter error";
			break;
			case 10:
				TempText = TempText + "Failed";
			break;
		}

		TempText = TempText + "\n\n\n\n2. Number of Terms: " + dtoa(vGlobalCurveFitEquationOrder,4);
		TempText = TempText + "\n3. Max Calls: " + dtoa(vGlobalCurveFitMaxCall,4);
		TempText = TempText + "\n4. Epsilon: " + dtoa(vGlobalCurveFitEpsilon,9);
		TempText = TempText + "\n5. Step Bound: " + dtoa(vGlobalCurveFitStepBound,4);
		TempText = TempText + "\n6. F Tolerance: " + dtoa(vGlobalCurveFitFTolerance,4);
		TempText = TempText + "\n7. X Tolerance: " + dtoa(vGlobalCurveFitXTolerance,4);
		TempText = TempText + "\n8. G Tolerance angle: " + dtoa(vGlobalCurveFitGTolerance,4);

		CHelpDialog *IHelpDialog;
		IHelpDialog = new(CHelpDialog);
		IHelpDialog->vHelpContext = 9999;
		IHelpDialog->vHelpType = 0;
		IHelpDialog->vWindowType = cTextWindow;
		IHelpDialog->vLocalSystemData = vLocalSystemData;
		IHelpDialog->vMainWindowPointer = vMainWindowPointer;
		IHelpDialog->vLocalConfigurationData = vLocalConfigurationData;
		IHelpDialog->vProductPointer = vGlobalCurrentProduct;
		IHelpDialog->vTextString = TempText;
		IHelpDialog->vTitleString = "Image Data";
		int TempResult = IHelpDialog->DoModal();
		delete IHelpDialog;
	}
}
void CHistogramDisplayDialog::ShowData()
{
	if (vDataArray)
	{
		CString TempText = vDataTitle;

		TempText = TempText + "\nNumber Of Points Fit: " + dtoa(vNumberOfPointsToPlot,0) + "\n";

		DWORD *TempPointer = vDataArray;
		for (WORD TempLoop = 0; TempLoop < vNumberOfPointsToPlot; TempLoop++)
		{
			TempText = TempText + "\n" + dtoa(TempLoop + 1,0) + ". " + dtoa(*TempPointer);
			TempPointer++;
		}


		CHelpDialog *IHelpDialog;
		IHelpDialog = new(CHelpDialog);
		IHelpDialog->vHelpContext = 9999;
		IHelpDialog->vHelpType = 0;
		IHelpDialog->vWindowType = cTextWindow;
		IHelpDialog->vLocalSystemData = vLocalSystemData;
		IHelpDialog->vMainWindowPointer = vMainWindowPointer;
		IHelpDialog->vLocalConfigurationData = vLocalConfigurationData;
		IHelpDialog->vProductPointer = vGlobalCurrentProduct;
		IHelpDialog->vTextString = TempText;
		IHelpDialog->vTitleString = "Plot Data";
		int TempResult = IHelpDialog->DoModal();
		delete IHelpDialog;
	}
}


void CHistogramDisplayDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}


void CHistogramDisplayDialog::OnStnClickedNextbutton()
{
	if (vACPowerMoniterFormat)
		CDialog::EndDialog(39);
}


void CHistogramDisplayDialog::OnStnClickedPreviousbutton()
{
	if (vACPowerMoniterFormat)
		CDialog::EndDialog(37);
}
