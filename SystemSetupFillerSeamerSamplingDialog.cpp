// SystemSetupFillerSeamerSamplingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "SystemSetupFillerSeamerSamplingDialog.h"
#include "afxdialogex.h"
#include "NumericEntryDialog.h"
#include "YesNoDialog.h"

// CSystemSetupFillerSeamerSamplingDialog dialog

IMPLEMENT_DYNAMIC(CSystemSetupFillerSeamerSamplingDialog, CDialogEx)

extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;

CSystemSetupFillerSeamerSamplingDialog::CSystemSetupFillerSeamerSamplingDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSystemSetupFillerSeamerSamplingDialog::IDD, pParent)
{
	vMainWindowPointer = NULL;
	vLocalSystemData = NULL;
	vLocalConfigurationData = NULL;
	vChangeMade = false;
	vUpdateDisplayTimerHandle = 31982;
	BYTE TempLoop;
	for (TempLoop = 0; TempLoop < 3; TempLoop++)
	{
		vNumberOfHeadsToMonitor[TempLoop] = 0;
	}
	for (TempLoop = 0; TempLoop < 4; TempLoop++)
	{
		vOldIndexPulses[TempLoop] = 0;
	}

	//vHeadOffsetFromIndexPulse[cFillerSamplingType] = 0;
	//vHeadOffsetFromIndexPulse[cSeamerSamplingType] = 0;
	vShowIndexErrors = 0;
	vShowSampleAllButton = 0;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText1);
	vLocalCWndCollection.Add(&m_Display1);
	vLocalCWndCollection.Add(&m_Display2);
	vLocalCWndCollection.Add(&m_Display3);
	vLocalCWndCollection.Add(&m_Display4);
	vLocalCWndCollection.Add(&m_Display5);
	vLocalCWndCollection.Add(&m_Display6);
	vLocalCWndCollection.Add(&m_Display7);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_Function3Display);
	vLocalCWndCollection.Add(&m_SubFunction1Display);
	vLocalCWndCollection.Add(&m_SubFunction1DisplayAbove);
	vLocalCWndCollection.Add(&m_SubFunction2Display);
	vLocalCWndCollection.Add(&m_SubFunction2Display2);
	vLocalCWndCollection.Add(&m_SubFunction2Display3);
	vLocalCWndCollection.Add(&m_SubFunction4Display);
	vLocalCWndCollection.Add(&m_SubFunction5Display);
	vLocalCWndCollection.Add(&m_SubFunction5DisplayAbove);
	vLocalCWndCollection.Add(&m_SubFunction6Display);
	vLocalCWndCollection.Add(&m_SubFunction6Display4);
	vLocalCWndCollection.Add(&m_SubFunction7Display);
	vLocalCWndCollection.Add(&m_SubFunction8Display);
	vLocalCWndCollection.Add(&m_Function5Display1);
}

void CSystemSetupFillerSeamerSamplingDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft, cMainBackgroundTop, cMainBackgroundLength, cMainBackgroundHeight, SWP_NOZORDER);

	BYTE TempLoop;
	for (TempLoop = 0; TempLoop < 3; TempLoop++)
	{
		vNumberOfHeadsToMonitor[TempLoop] = vLocalConfigurationData->vNumberOfHeadsToMonitor[TempLoop];
	}
	//for (TempLoop = 0; TempLoop < 4; TempLoop++)
	//{
	//	vOldIndexPulses[TempLoop] = vLocalConfigurationData->vHeadOffsetFromIndexPulse[TempLoop];
	//}

	//vHeadOffsetFromIndexPulse[0] = vLocalConfigurationData->vHeadOffsetFromIndexPulse[0];//this is the 1/2 speed filler sampling index
	//vHeadOffsetFromIndexPulse[cFillerSamplingType] = vLocalConfigurationData->vHeadOffsetFromIndexPulse[cFillerSamplingType];
	//vHeadOffsetFromIndexPulse[cSeamerSamplingType] = vLocalConfigurationData->vHeadOffsetFromIndexPulse[cSeamerSamplingType];
	//vHeadOffsetFromIndexPulse[3] = vLocalConfigurationData->vHeadOffsetFromIndexPulse[3];
	vShowIndexErrors = vLocalConfigurationData->vShowIndexErrors;
	vShowSampleAllButton = vLocalConfigurationData->vShowSampleAllButton;
	vEncoderRateTolerance = vLocalConfigurationData->vEncoderRateTolerance;


	UpdateDisplay();
	this->SetWindowText(_T("SetupFillSeamSample"));

	int TempTimerResult = SetTimer(vUpdateDisplayTimerHandle,1000,NULL);
	if (!TempTimerResult)
		ReportErrorMessage("Error-Display Timer SD Failed",cEMailInspx,32000);

	SetupMenu(vLocalCWndCollection);
}


CSystemSetupFillerSeamerSamplingDialog::~CSystemSetupFillerSeamerSamplingDialog()
{
}

void CSystemSetupFillerSeamerSamplingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction4Display, m_SubFunction4Display);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText1);
	DDX_Control(pDX, IDC_Display1, m_Display1);
	DDX_Control(pDX, IDC_Display2, m_Display2);
	DDX_Control(pDX, IDC_Display3, m_Display3);
	DDX_Control(pDX, IDC_Display4, m_Display4);
	DDX_Control(pDX, IDC_Display5, m_Display5);
	DDX_Control(pDX, IDC_Display6, m_Display6);
	DDX_Control(pDX, IDC_Display7, m_Display7);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function3Display, m_Function3Display);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction1Display, m_SubFunction1Display);
	DDX_Control(pDX, IDC_SubFunction1DisplayAbove, m_SubFunction1DisplayAbove);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction2Display, m_SubFunction2Display);
	DDX_Control(pDX, IDC_SubFunction2Display2, m_SubFunction2Display2);
	DDX_Control(pDX, IDC_SubFunction2Display3, m_SubFunction2Display3);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction5Display, m_SubFunction5Display);
	DDX_Control(pDX, IDC_SubFunction5DisplayAbove, m_SubFunction5DisplayAbove);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction6Display, m_SubFunction6Display);
	DDX_Control(pDX, IDC_SubFunction6Display4, m_SubFunction6Display4);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction7Display, m_SubFunction7Display);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction8Display, m_SubFunction8Display);
	DDX_Control(pDX, IDC_Function5Display1, m_Function5Display1);
}


BEGIN_MESSAGE_MAP(CSystemSetupFillerSeamerSamplingDialog, CDialogEx)
	ON_STN_CLICKED(IDC_Function1Button, &CSystemSetupFillerSeamerSamplingDialog::OnFunction1Button)
	ON_STN_CLICKED(IDC_Function2Button, &CSystemSetupFillerSeamerSamplingDialog::OnFunction2Button)
	ON_STN_CLICKED(IDC_Function3Button, &CSystemSetupFillerSeamerSamplingDialog::OnFunction3Button)
	ON_STN_CLICKED(IDC_Function4Button, &CSystemSetupFillerSeamerSamplingDialog::OnFunction4Button)
	ON_STN_CLICKED(IDC_Function5Button, &CSystemSetupFillerSeamerSamplingDialog::OnFunction5Button)
	ON_STN_CLICKED(IDC_SubFunction1Button, &CSystemSetupFillerSeamerSamplingDialog::OnSubFunction1Button)
	ON_STN_CLICKED(IDC_SubFunction2Button, &CSystemSetupFillerSeamerSamplingDialog::OnSubFunction2Button)
	ON_STN_CLICKED(IDC_SubFunction3Button, &CSystemSetupFillerSeamerSamplingDialog::OnSubFunction3Button)
	ON_STN_CLICKED(IDC_SubFunction4Button, &CSystemSetupFillerSeamerSamplingDialog::OnSubFunction4Button)
	ON_STN_CLICKED(IDC_SubFunction5Button, &CSystemSetupFillerSeamerSamplingDialog::OnSubFunction5Button)
	ON_STN_CLICKED(IDC_SubFunction6Button, &CSystemSetupFillerSeamerSamplingDialog::OnSubFunction6Button)
	ON_STN_CLICKED(IDC_SubFunction7Button, &CSystemSetupFillerSeamerSamplingDialog::OnSubFunction7Button)
	ON_STN_CLICKED(IDC_SubFunction8Button, &CSystemSetupFillerSeamerSamplingDialog::OnSubFunction8Button)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CSystemSetupFillerSeamerSamplingDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()


// CSystemSetupFillerSeamerSamplingDialog message handlers


void CSystemSetupFillerSeamerSamplingDialog::OnFunction1Button()
{
	//Main Menu was pressed
	if ((!vChangeMade) || (AbortSetupChanges()))
	{
		KillTimer(vUpdateDisplayTimerHandle);
		if (vChangeMade)	//if there were changes, undo changes to global variables
		{	
			//for (BYTE TempLoop = 0; TempLoop < 4; TempLoop++)
			//{
			//	vLocalConfigurationData->vHeadOffsetFromIndexPulse[TempLoop] = vOldIndexPulses[TempLoop];
			//}
			vLocalConfigurationData->vEncoderRateTolerance = vOldEncoderRateTolerance;
		}
		CDialog::EndDialog(10);
	}
}


void CSystemSetupFillerSeamerSamplingDialog::OnFunction2Button()
{
	//Save and exit was pressed
	if (!vChangeMade) 
	{
		KillTimer(vUpdateDisplayTimerHandle);
		CDialog::EndDialog(1);
	}
	else
	if (KeepSetupChanges())
	{
		if ((vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] != vNumberOfHeadsToMonitor[cFillerSamplingType]) ||
			(vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType] != vNumberOfHeadsToMonitor[cSeamerSamplingType]))
		{
			vLocalSystemData->vCurrentProductChanged = true;
			vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] = vNumberOfHeadsToMonitor[cFillerSamplingType];
			vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType] = vNumberOfHeadsToMonitor[cSeamerSamplingType];
			vMainWindowPointer->AllocateValveMonitoringData();
		}
		vLocalConfigurationData->vEncoderRateTolerance = vEncoderRateTolerance;
		vLocalConfigurationData->vNumberOfHeadsToMonitor[0] = vNumberOfHeadsToMonitor[0];
		//vLocalConfigurationData->vHeadOffsetFromIndexPulse[3] = vHeadOffsetFromIndexPulse[3];
		//vLocalConfigurationData->vHeadOffsetFromIndexPulse[0] = vHeadOffsetFromIndexPulse[0];	//half speed filler sampling index pulse
		//vLocalConfigurationData->vHeadOffsetFromIndexPulse[cFillerSamplingType] = vHeadOffsetFromIndexPulse[cFillerSamplingType];
		//vLocalConfigurationData->vHeadOffsetFromIndexPulse[cSeamerSamplingType] = vHeadOffsetFromIndexPulse[cSeamerSamplingType];
		vLocalConfigurationData->vShowIndexErrors = vShowIndexErrors;

		vLocalConfigurationData->vShowSampleAllButton = vShowSampleAllButton;

		KillTimer(vUpdateDisplayTimerHandle);
		CDialog::EndDialog(1);
	}
}


void CSystemSetupFillerSeamerSamplingDialog::OnFunction3Button()
{
	vShowIndexErrors = !vShowIndexErrors;
	SetChangeMade();
	UpdateDisplay();
}


void CSystemSetupFillerSeamerSamplingDialog::OnFunction4Button()
{
	//Cancel was pressed
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
	{
		if (vChangeMade)	//if a change was made, undo changes to global variables
		{	
			vLocalConfigurationData->vEncoderRateTolerance = vOldEncoderRateTolerance;
		}

		CDialog::OnCancel();
	}
}


void CSystemSetupFillerSeamerSamplingDialog::OnFunction5Button()
{
	CNumericEntryDialog INumericEntryDialog;  
	
	char TempStringBuffer[11];
	int TempErr = _itoa_s(vLocalConfigurationData->vEncoderRateTolerance,TempStringBuffer,11,10);
	INumericEntryDialog.vEditString = TempStringBuffer;

	INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Encoder Rate Tolerance";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Global Setting; effects all products";
	INumericEntryDialog.m_DialogTitleStaticText3 = "Default: 5%";
	INumericEntryDialog.m_DialogTitleStaticText4 = "";

	INumericEntryDialog.m_UnitsString = "Hertz (Hz)";
	INumericEntryDialog.vIntegerOnly = true;
	INumericEntryDialog.vAllowNegative = false;
	INumericEntryDialog.vMaxValue = vLocalSystemData->vMaximumEncoderRate;
	INumericEntryDialog.vMinValue = 0;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		int TempInt = _wtoi(INumericEntryDialog.vEditString);
		vEncoderRateTolerance = TempInt;

		UpdateDisplay();
		SetChangeMade();

	}
	else 
	if (nResponse == 10)
	{
		//Main Menu Button pressed
		KillTimer(vUpdateDisplayTimerHandle);
		CDialog::EndDialog(10);
	}
}

void CSystemSetupFillerSeamerSamplingDialog::OnSubFunction1Button()
{
	if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nYou must stop inspecting to make changes\nto the Number of Valves";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	else
	{
		CNumericEntryDialog INumericEntryDialog;  
				
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vNumberOfHeadsToMonitor[cFillerSamplingType], 0);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Number Of " + vLocalSystemData->vSamplingName[cFillerSamplingType] + " valves";
		INumericEntryDialog.m_DialogTitleStaticText2 = "System Settting; effects all products";
		INumericEntryDialog.m_DialogTitleStaticText3 = "";
		INumericEntryDialog.m_DialogTitleStaticText4 = "";

		INumericEntryDialog.m_UnitsString = "0 = Disable Filler Monitor and Sampling";
		INumericEntryDialog.vIntegerOnly = true;
		INumericEntryDialog.vMaxValue = 254;
		INumericEntryDialog.vMinValue = 0;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vNumberOfHeadsToMonitor[cFillerSamplingType] = _wtoi(INumericEntryDialog.vEditString);
			SetChangeMade();
			UpdateDisplay();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu Button pressed
			KillTimer(vUpdateDisplayTimerHandle);
			CDialog::EndDialog(10);
		}
	}
}

void CSystemSetupFillerSeamerSamplingDialog::OnSubFunction2Button()
{
	
}


void CSystemSetupFillerSeamerSamplingDialog::OnSubFunction3Button()
{
/*	if (vLocalSystemData->vFindFillerOffsetHalfSpeed == true)  //if we were already trying to find half speed offset
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "You can only find one offset at a time, please wait until the Half Speed Filler Sampler Offset is found.";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
	else
	if (vLocalSystemData->vFindFillerOffsetFullSpeed)	//if already calculating full speed offset, stop
	{
		vLocalSystemData->vFindFillerOffsetFullSpeed = false;
		UpdateDisplay();
	}
	else
	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "You must start inspecting to find Index Offset.\nSet Filler Head #1 to highest amount or\nput dense material in or around continers for Head #1\nThen press this button.\nThe software finds the heaviest container and\nadjusts the offset so it is Head #1.\nRequires at least 3 revolutions (2 to sync index pulse)";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\nSet Filler Head #1 to highest amount or\nput dense material in or around continers for Head #1\nThe software finds the heaviest container and\nadjusts the offset so it is Head #1.\nRequires at least 3 revolutions (2 to sync index pulse)";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();

		vLocalSystemData->vFindFillerOffsetFullSpeed = true;
		SetChangeMade();
		UpdateDisplay();
	}
	*/
}


void CSystemSetupFillerSeamerSamplingDialog::OnSubFunction4Button()
{
	if ((vNumberOfHeadsToMonitor[cFillerSamplingType]) || (vNumberOfHeadsToMonitor[cSeamerSamplingType]))
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nRandom Sampling is not supported when Filler or Seamer Sampling is enabled.";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	else
	{
		if (vNumberOfHeadsToMonitor[cRandomSamplingType] == 0)
			vNumberOfHeadsToMonitor[cRandomSamplingType] = 1;
		else
			vNumberOfHeadsToMonitor[cRandomSamplingType] = 0;
		SetChangeMade();
		UpdateDisplay();
		vLocalSystemData->vCurrentProductChanged = true;
	}
}


void CSystemSetupFillerSeamerSamplingDialog::OnSubFunction5Button()
{
	CNumericEntryDialog INumericEntryDialog;  
				
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vNumberOfHeadsToMonitor[cSeamerSamplingType], 0);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Number Of Seamer Heads";
	INumericEntryDialog.m_DialogTitleStaticText2 = "System Settting; effects all products";
	INumericEntryDialog.m_DialogTitleStaticText3 = "";
	INumericEntryDialog.m_DialogTitleStaticText4 = "";

	INumericEntryDialog.m_UnitsString = "0 = Disable Filler Monitor and Sampling";
	INumericEntryDialog.vIntegerOnly = true;
	INumericEntryDialog.vMaxValue = 254;
	INumericEntryDialog.vMinValue = 0;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vNumberOfHeadsToMonitor[cSeamerSamplingType] = _wtoi(INumericEntryDialog.vEditString);
		SetChangeMade();
		UpdateDisplay();
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu Button pressed
		KillTimer(vUpdateDisplayTimerHandle);
		CDialog::EndDialog(10);
	}
}


void CSystemSetupFillerSeamerSamplingDialog::OnSubFunction6Button()
{
	
}


void CSystemSetupFillerSeamerSamplingDialog::OnSubFunction7Button()
{
	CYesNoDialog TempYesNoDialog;
	CString TempText2 = "Hiding";
	if (vShowSampleAllButton & 1)
		TempText2 = "Showing";

	CString TempText = "\nFiller\nShow or Hide the Sample All valves button?\nCurrently " + TempText2 + " button for Filler.";
	TempYesNoDialog.vNoticeText = TempText;
	TempYesNoDialog.vYesButtonText = "Show Button";
	TempYesNoDialog.vNoButtonText = "Hide Button";
	TempYesNoDialog.vQuestionType = cQuestion;
	int TempResult = TempYesNoDialog.DoModal();
	if (TempResult == IDOK)
		vShowSampleAllButton = vShowSampleAllButton | 1; //set bit to show button
	else
		vShowSampleAllButton = vShowSampleAllButton & 0xFE; //clear bit to hide button

	TempText2 = "Hiding";
	if (vShowSampleAllButton & 2)
		TempText2 = "Showing";

	TempText = "\nSeamer\nShow or Hide the Sample All Heads button?\nCurrently " + TempText2 + " button for Seamer.";
	TempYesNoDialog.vNoticeText = TempText;
	TempYesNoDialog.vYesButtonText = "Show Button";
	TempYesNoDialog.vNoButtonText = "Hide Button";
	TempYesNoDialog.vQuestionType = cQuestion;
	TempResult = TempYesNoDialog.DoModal();
	if (TempResult == IDOK)
		vShowSampleAllButton = vShowSampleAllButton | 2; //set bit to show button
	else
		vShowSampleAllButton = vShowSampleAllButton & 0xFD; //clear bit to hide button

	SetChangeMade();
	UpdateDisplay();
}


void CSystemSetupFillerSeamerSamplingDialog::OnSubFunction8Button()
{
}


BOOL CSystemSetupFillerSeamerSamplingDialog::PreTranslateMessage(MSG* pMsg)
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
					cShowHelp, 6, 0);
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

	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CSystemSetupFillerSeamerSamplingDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);

	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText1, 8);
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1DisplayAbove) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1DisplayAbove, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5DisplayAbove)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5DisplayAbove, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Function3Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Function3Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Function5Display1)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Function5Display1, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Display1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Display1, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Display2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Display2, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Display3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Display3, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Display4)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Display4, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Display5)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Display5, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Display6)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Display6, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Display7)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Display7, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	{

	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	{

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

void CSystemSetupFillerSeamerSamplingDialog::SetChangeMade() 
{
	if (!vChangeMade)
	{
		m_Function2Button.ShowWindow(SW_SHOW);
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_Function4Button.ShowWindow(SW_SHOW);
		CString TempText;
		TempText.LoadString(IDS_SaveAndExit);
		SetDlgItemText(IDC_Function2Button,TempText);
		TempText.LoadString(IDS_Cancel);
		SetDlgItemText(IDC_Function4Button,TempText);
		vChangeMade = true;
	}
}

void CSystemSetupFillerSeamerSamplingDialog::UpdateDisplay()
{
	CString TempText = "Hiding";
	if (vShowSampleAllButton & 1)
		TempText = "Showing";
	CString TempText1 = "Hiding";
	if (vShowSampleAllButton & 2)
		TempText1 = "Showing";
	SetDlgItemText(IDC_SubFunction7Display, "Filler: " + TempText + "  Seamer: " + TempText1);

	if (vShowIndexErrors)
	{
		SetDlgItemText(IDC_Function3Display, _T("Showing Index Pulse Errors"));
		SetDlgItemText(IDC_Function3Button, _T("Hide Index Errors"));
	}
	else
	{
		SetDlgItemText(IDC_Function3Display, _T("Hiding Index Pulse Errors"));
		SetDlgItemText(IDC_Function3Button, _T("Show Index Errors"));
	}

	SetDlgItemText(IDC_SubFunction1Display, dtoa(vNumberOfHeadsToMonitor[cFillerSamplingType], 0));
	//SetDlgItemText(IDC_SubFunction2Display, dtoa(vHeadOffsetFromIndexPulse[cFillerSamplingType], 0));
	//SetDlgItemText(IDC_SubFunction2Display2, dtoa(vHeadOffsetFromIndexPulse[0], 0));	//half speed offset
	SetDlgItemText(IDC_SubFunction5Display, dtoa(vNumberOfHeadsToMonitor[cSeamerSamplingType], 0));
	//SetDlgItemText(IDC_SubFunction6Display, dtoa(vHeadOffsetFromIndexPulse[cSeamerSamplingType], 0));
	//SetDlgItemText(IDC_SubFunction6Display4, dtoa(vHeadOffsetFromIndexPulse[3], 0));

	
//	SetDlgItemText(IDC_Display6, dtoa(vGlobalCurrentProduct->vFoundFullSpeedEncoderRate)+"Hz");
//	SetDlgItemText(IDC_Display7, dtoa(vGlobalCurrentProduct->vFoundHalfSpeedEncoderRate)+"Hz");
//	SetDlgItemText(IDC_SubFunction2Display3, dtoa(vLocalConfigurationData->vEncoderRateBoundry,0) + "Hz");

	//if (vLocalSystemData->vFindFillerOffsetFullSpeed)
	//	SetDlgItemText(IDC_SubFunction3Button, "Stop Find Full Speed F.I. Offset");
	//else
	//	SetDlgItemText(IDC_SubFunction3Button, "Find Full Speed F.I. Offset");
	
	m_SubFunction4Display.ShowWindow(SW_SHOW);
	if (vNumberOfHeadsToMonitor[cRandomSamplingType])
	{
		SetDlgItemText(IDC_SubFunction4Button, _T("Disable Random Sampling"));
		SetDlgItemText(IDC_SubFunction4Display, _T("Random Sampling is Enabled"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction4Button, _T("Enable Random Sampling"));
		SetDlgItemText(IDC_SubFunction4Display, _T("Random Sampling is Disabled"));
	}
		
	if ((vNumberOfHeadsToMonitor[cFillerSamplingType]) || (vNumberOfHeadsToMonitor[cSeamerSamplingType]))
	{
		m_SubFunction4Button.ShowWindow(SW_HIDE);
//		m_SubFunction4Display.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
//		SetDlgItemText(IDC_SubFunction4Button, "Find Half Speed F.I. Offset");
	}
	else
	{
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		//m_ArrowLeft4.ShowWindow(SW_SHOW);

		if (vNumberOfHeadsToMonitor[cRandomSamplingType])
		{
			SetDlgItemText(IDC_SubFunction4Button, _T("Disable Random Sampling"));
			SetDlgItemText(IDC_SubFunction4Display, _T("Random Sampling is Enabled"));
		}
		else
		{
			SetDlgItemText(IDC_SubFunction4Button, _T("Enable Random Sampling"));
			SetDlgItemText(IDC_SubFunction4Display, _T("Random Sampling is Disabled"));
		}
	}
	SetDlgItemText(IDC_Function5Display1, dtoa(vEncoderRateTolerance,0) + "%");
}




//void CSystemSetupFillerSeamerSamplingDialog::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO: Add your message handler code here and/or call default
//	if (nIDEvent == vUpdateDisplayTimerHandle)
//	{
//		KillTimer(vUpdateDisplayTimerHandle);
//		OneSecondDisplayUpdate();
//		int TempTimerResult = SetTimer(vUpdateDisplayTimerHandle,1000,NULL);
//		if (!TempTimerResult)
//			ReportErrorMessage("Error-Display Timer Failed",cEMailInspx,32000);
//	}
//
//	CDialogEx::OnTimer(nIDEvent);
//}

//void CSystemSetupFillerSeamerSamplingDialog::OneSecondDisplayUpdate()
//{
//	if (vOldFindFillerOffsetFullSpeed != vLocalSystemData->vFindFillerOffsetFullSpeed)
//	{
//		vOldFindFillerOffsetFullSpeed = vLocalSystemData->vFindFillerOffsetFullSpeed;
//		if (!vOldFindFillerOffsetFullSpeed)
//			vHeadOffsetFromIndexPulse[cFillerSamplingType] = vLocalConfigurationData->vHeadOffsetFromIndexPulse[cFillerSamplingType];
//		UpdateDisplay();
//	}
//	if (vOldFindFillerOffsetHalfSpeed != vLocalSystemData->vFindFillerOffsetHalfSpeed)
//	{
//		vOldFindFillerOffsetHalfSpeed = vLocalSystemData->vFindFillerOffsetHalfSpeed;
//		if (!vOldFindFillerOffsetHalfSpeed)
//			vHeadOffsetFromIndexPulse[0] = vLocalConfigurationData->vHeadOffsetFromIndexPulse[0];
//		UpdateDisplay();
//	}
//}

void CSystemSetupFillerSeamerSamplingDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
