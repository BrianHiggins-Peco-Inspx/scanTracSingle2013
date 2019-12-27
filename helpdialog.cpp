//ScanTrac Side View Source File
// HelpDialog.cpp : implementation file
//

//for red text flag line with a *
//for green text flag line with a + or -

#include "stdafx.h"
#include "afxdlgs.h"
#include "windows.h"
#include "mmsystem.h"
#include "ScanTrac.h"
#include "ScanTracDlg.h"
#include "HelpDialog.h"
#include "HelpIndexDialog.h"
#include "EasySelectProductDialog.h"
#include "SerialPort.h"
#include "NoticeDialog.h"
#include "SelectMultipleItemDialog.h"
#include "FindParameterHelpDialog.h"
#include "AlphaEntryDialog.h"
#include "HistogramDisplayDialog.h"
#include "BackupHardDriveThread.h"
#include "SpellmanDXMXRaySourcePort.h"
#include "ProcessQuickLearnImagesThread.h"
#include "XRaySource.h"
#include "DisplayDialogThread.h"

#define cErrorLogBufferSize 100
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const BYTE cMaximumEventLogFiles = 5;

enum {cDisplayPercentOfInitial, cDisplayPercentOfStandard, cDisplayRawValues}; // for vSourceStrengthDisplayMode

/////////////////////////////////////////////////////////////////////////////
// CHelpDialog dialog

extern BYTE vGlobalEjectorConfirmMask[cNumberOfEjectors];
#ifdef CompileInHTTPClientOperationsGuardian
	extern DWORD vGlobalOGSendMessageexceptionCount;
	extern DWORD vGlobalOGSendMessageErrorCount;
	extern DWORD vGlobalOGSendMessageGoodCount;
	extern int vGlobalLastOGServerWriteStatusCode;
	extern std::wstring vGlobalOGLastErrorKeyword;
	extern std::wstring vGlobalOGLastErrorMessage;
	extern int vGlobalLastOGServerErrorCode;
	extern DWORD vGlobalOGMessageSendCounter;
#endif
extern BYTE vGlobalUseCommaForDecimalPoint;
extern double vGlobalPercentToResizeY;
extern double vGlobalPercentToResizeX;
extern CFont vGlobalFixedFont;
extern CFont vGlobalTextFont;
extern BYTE vGlobalLanguage;
extern DWORD vGlobalFBWFEnable;
extern bool vGlobalFPGAVersion10Point0OrHigher;
extern bool vGlobalFPGAVersion16point0orAbove;
extern bool vGlobalFPGAVersion18point0orAbove;
extern CProduct *vGlobalCurrentProduct;
extern BYTE vGlobalInAutoSize;
extern CDisplayDialogThread *vGlobalDisplayDialogThread;
extern bool vGlobalFPGAVersion11Point0OrHigher;
//extern bool vGlobalUseHalconLibrary;
extern double vGlobalHardDriveTotalBytes;
extern CString vGlobalTopString;
extern CString vGlobalBottomString;
extern CProcessQuickLearnImagesThread *vGlobalProcessQuickLearnImagesThread;
extern BYTE vGlobalPCRunningWindowsXP;
extern WORD vGlobalPasswordTimeOut;
extern DWORD vGlobalSystemAffinityMask;
extern CXRaySource *vGlobalMonoBlockXRayPort;
extern CSpellmanDXMXRaySourcePort *vGlobalDXMXRayPort;
extern BYTE vGlobalLearnState;
extern CLifeTimeCounters *vGlobalLifeTimeCounters;
extern bool vGlobalEvenSecond;
//extern BYTE vGlobalMouseMoveCounter;
//extern WORD vGlobalUnderRemoteControl;
extern CString vGlobalCurrentDirectory;
extern BYTE vGlobalShiftKeyDown;
extern 	CSerialPort *vGlobaluCSerialPort;
extern int vGlobalArchiveVersionNumber;
//extern bool vGlobalInWindowsDialog;
extern CBackupHardDriveThread *vGlobalBackupHardDriveThread;
extern int vGlobalPasswordLevel;
extern unsigned long vGlobalDialogTitleColor;
extern CString vGlobalDHCPString[2];
extern CString vGlobalIPAddressString;
extern CString vGlobalIPSubNetMaskString;
extern CString vGlobalIPDefaultGatewayString;
extern CString vGlobalIPDNSAddressString1;
extern BYTE vGlobalIPDHCPEnabled;
extern CString vGlobalNetworkBoardCode;
extern CString vGlobalRegistryKeyPath;
extern BYTE vGlobalFrameGrabberType;
extern CString vFindString;
extern WORD vGlobalDisplaySizeH;
extern WORD vGlobalDisplaySizeV;

CHelpDialog::CHelpDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CHelpDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHelpDialog)
	m_HelpText = _T("");
	//}}AFX_DATA_INIT
	//vOldUnderRemoteControl = vGlobalUnderRemoteControl;
	vShowTable = false;
	vHistogramData = NULL;
	vSourceStrengthDisplayMode = cDisplayPercentOfInitial;
	vMakeRedTextBoldAndUnderLined = false;
	vShowedInspxMessages = false;
	vMakeExitButtonGreen = false;
	vACPowerMoniterFormat = false;
	vOldErrorLogCount = 0;
	vWhiteBrush = ::CreateSolidBrush(cWhite);
	vBlueBrush = ::CreateSolidBrush(cLightBlue);
	vYellowBrush = ::CreateSolidBrush(cYellow);
	AfxInitRichEdit( );

	vWindowType = cMainHelpWindow;
	vProductPointer = NULL;
	vTitleString = "Information Window";
	vTextString = "No Information Available";
	vACPowerMoniterErrorLogFile = "";
	vOldACLogFileSize = 0;
	vLocalProductCollection = NULL;
	vLocalSystemData = NULL;
	vMainWindowPointer = NULL;
	vLocalConfigurationData = NULL;
//	vHideCaretTimerHandle = 31348;
	vCheckErrorMessagesTimerHandle = 31349;
	vOneSecondTimerHandle = 31350;
	vProcessingMessageTimerHandle = 31351;
	vUpdateInspxLogDisplay = 31352;

	vProductionReportStartIndex=0;
	vProductionReportEndIndex=0;
	vProductionReportHeaderString = "";
	vProductDataIndex = 0;
	vHeaderCount = 0;
	vEndTimeIndex = 0;
	vMaxEventLogScrollDown = false;
	vMaxEventLogScrollUp = false;
	vLineIndexInFile = NULL;
	vEventLogCurrentFiles = NULL;
	vLastFileIndex = 0;
	vLinesInCurrentDayEventLog = -1;
	vFirstLineNumberOnEventLogScreen = -1;
	vShowUpdateButton = true;
	vColor = NULL;
	vLinesOnScreen = 21;
	vStartFindSearch = false;
	vSameLogFile = false;
	TempHoldNextOrPreviousLogString = "Loading Log File";

	vMouseDownRepeatTimerHandle = 3005;
	vMouseState = 0;		//1 is left mouse button clicked down, 2 is left mouse button transistioned up, 3 is double clicked left mouse button
	vRecievedButtonPress = false;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_HelpTextControl);
	vLocalCWndCollection.Add(&m_HelpTitle);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_RemoteKeyLabelsButton);
}

CHelpDialog::~CHelpDialog()
{
	DeleteObject(vWhiteBrush);
	DeleteObject(vBlueBrush);
	DeleteObject(vYellowBrush);
	CFont *TempPointer = CFont::FromHandle(vFontToUse);
	if (TempPointer)
		vFontToUse.DeleteObject();
}

void CHelpDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

	vBlueCharacterFormat.dwMask = CFM_COLOR | CFM_BOLD | CFM_UNDERLINE;
	vBlueCharacterFormat.crTextColor = cDarkBlue;
	if (vMakeRedTextBoldAndUnderLined)
		vBlueCharacterFormat.dwEffects = CFE_BOLD | CFE_UNDERLINE;
	else
		vBlueCharacterFormat.dwEffects = 0;

	vGreenCharacterFormat.dwMask = CFM_COLOR | CFM_BOLD | CFM_UNDERLINE;
	vGreenCharacterFormat.crTextColor = cDarkGreen;
	if (vMakeRedTextBoldAndUnderLined)
		vGreenCharacterFormat.dwEffects = CFE_BOLD | CFE_UNDERLINE;
	else
		vGreenCharacterFormat.dwEffects = 0;

	vYellowCharacterFormat.dwMask = CFM_COLOR | CFM_BOLD | CFM_UNDERLINE;
	vYellowCharacterFormat.crTextColor = cPurple;
	vYellowCharacterFormat.dwEffects = 0;
	vBlackCharacterFormat.dwMask = CFM_COLOR | CFM_BOLD | CFM_UNDERLINE;
	vBlackCharacterFormat.crTextColor = cBlack;
	vBlackCharacterFormat.dwEffects = 0;

	vRedCharacterFormat.dwMask = CFM_COLOR | CFM_BOLD | CFM_UNDERLINE;
	vRedCharacterFormat.crTextColor = cRed;
	if (vMakeRedTextBoldAndUnderLined)
		vRedCharacterFormat.dwEffects = CFE_BOLD | CFE_UNDERLINE;
	else
		vRedCharacterFormat.dwEffects = 0;

	vPinkCharacterFormat.dwMask = CFM_COLOR | CFM_BOLD | CFM_UNDERLINE;
	vPinkCharacterFormat.crTextColor = cPink;
	vPinkCharacterFormat.dwEffects = CFE_BOLD | CFE_UNDERLINE;

	SetupFont();

	m_HelpTextControl.SetDefaultCharFormat(vBlackCharacterFormat);
	// Done with the font. Delete the font object.
	//font.DeleteObject();
// made as a global var	vFindString = "";
	vLastFindString = "";
	vStartFindPosition = 0;
	vLastFindPosition = 0;		// if significant change to display window want to restart find location
	vFindTextShow = cFindTextNoHighlight;

	if (vWindowType == cErrorLogWindow)
	{
		vEventLogCurrentFiles = new CString[cMaximumEventLogFiles];
		for (int i = 0; i < cMaximumEventLogFiles; i++)
			vEventLogCurrentFiles[i] = " ";
	
		if ((vErrorMessageDisplayMask & 0x184) == 0x080)
			vDialogTitleString = "Reject Log";
		else
			vDialogTitleString = "Event Log";
		SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);

		SetDlgItemText(IDC_HelpTitle,"Log Started: " + vLocalSystemData->vErrorLogStarted);
		SetDlgItemText(IDC_Function3Button, _T(""));
		SetDlgItemText(IDC_Function5Button, _T(""));
		if (vErrorMessageDisplayMask == cInspxLogMask)  //inspx log
		{
			m_SubFunction3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
		}
		else //error log
		{
			if (vLineIndexInFile)
				free(vLineIndexInFile);
			if (ThereIsEnoughMemory(sizeof(ULONGLONG) * 1001, "Event Log Dialog File to string index"))
			{
				vLineIndexInFile = (ULONGLONG *)(malloc(sizeof(ULONGLONG) * 1001));
				//vLineIndexInFile[0] = 1;
				int i;
				for (i = 0; i < 1000; i++)
					vLineIndexInFile[i] = 0;
			}
			else
			{
				ReportErrorMessage("Not Enough Memory for EventLog Alloc",cEMailInspx, 32000);
				#ifdef _DEBUG
				afxDump << "Not Enough Memory for EventLog Alloc";
				#endif
			}
			vLastFileIndex = 0;
			m_SubFunction1Button.ShowWindow(SW_SHOW);
			m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction1Button, _T("Clear Reject- Event Log"));

			SetDlgItemText(IDC_SubFunction3Button, _T("Event Types To Display"));
		}
		//kjh 7/21 added next 3 lines
		SetDlgItemText(IDC_Function2Button, _T("Find Text"));
		m_Function2Button.ShowWindow(SW_SHOW);

		SetDlgItemText(IDC_SubFunction2Button, _T(""));
		SetDlgItemText(IDC_SubFunction4Button, _T(""));
		m_Function5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
		m_Function3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
		m_SubFunction2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		ShowErrorLog(true); //do refresh
		int vTimerResult = SetTimer(vCheckErrorMessagesTimerHandle,1000,NULL);
	}
	else
	if ((vWindowType == cHelpWindow) || (vWindowType == cMainHelpWindow))
	{
		this->SetWindowText(_T("Help"));
		SetDlgItemText(IDC_Function1Button, _T("Exit Help"));
		if (vWindowType == cHelpWindow)
		ShowHelp();
	}
	else	//product or system configuration window or show source strength, or 
				//Production Report window
	{//text window
		if (vLocalConfigurationData)
		if (!vLocalConfigurationData->vSourceStrengthInitialValues[0])
			vSourceStrengthDisplayMode = cDisplayPercentOfStandard;

		SetDlgItemText(IDC_Function3Button, _T(""));
		CString TempText = _T("-");
		TempText.LoadString(IDS_Exit);
		SetDlgItemText(IDC_Function2Button, TempText);
		m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_Function5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		//SetDlgItemText(IDC_SubFunction1Button,"Show Production Report");
		m_SubFunction1Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());

		//SetDlgItemText(IDC_SubFunction2Button,"Show Source Log");
		m_SubFunction2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());

		if (vWindowType == cProductInformationWindow)
		{
			SetDlgItemText(IDC_SubFunction3Button, _T("Show\nSystem\nSetup"));
			SetDlgItemText(IDC_SubFunction4Button, _T("Show a Different Product"));
			m_SubFunction3Button.ShowWindow(SW_SHOW);
			m_SubFunction4Button.ShowWindow(SW_SHOW);
		}
		else
		{
			m_SubFunction3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
			m_SubFunction4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
		}
		m_Function3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
		m_SubFunction1Button.ShowWindow(SW_SHOW);

		if (vWindowType == cProductInformationWindow)
		{
			this->SetWindowText(_T("Product Summary"));
			SetDlgItemText(IDC_DialogTitleStaticText1, _T("Product Setup Summary"));
			SetDlgItemText(IDC_HelpTitle, _T("No Product Selected"));
			SetDlgItemText(IDC_HelpText, _T(""));
			DisplayProductInformation();
		}
		else
		if (vWindowType == cSourceStrengthWindow)
		{
			this->SetWindowText(_T("Source Strength"));
			ShowSourceStrengthData();
		}
		else
		if (vWindowType == cProductionReportWindow)
		{
			this->SetWindowText(_T("Production Report"));
			ShowProductionReport();
		}
		else
		{
			this->SetWindowText(_T("Information"));

			CString TempTitleString;
			TempTitleString.LoadString(IDS_ImageInformation);
			if (vTitleString == TempTitleString)
				vDialogTitleString.LoadString(IDS_RejectInformation);
			else
				vDialogTitleString = "ScanTrac Information";
			SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);

			SetDlgItemText(IDC_HelpTitle,vTitleString);
			vTextString = vTextString + "\n\n";
			SetDlgItemText(IDC_HelpText,vTextString);

			m_SubFunction1Button.ShowWindow(SW_HIDE);
			m_SubFunction2Button.ShowWindow(SW_HIDE);
			m_SubFunction3Button.ShowWindow(SW_HIDE);
			m_SubFunction4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
			this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
			this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
			this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());

			if (vColor) //color info was passed by the calling function that made the string, so show it
			{
				DisplayColor(&vTextString);
			}
			else
			{ //no color info from calling function, so make color info from * and + symbols
				CreateColorDataStructure();

				int TempEndCount = vTextString.GetLength();
				if (TempEndCount >= cSizeOfVColor - 1)
					TempEndCount = cSizeOfVColor - 2;
				if (TempEndCount > 5)
				for (int TempPosition = 0; TempPosition < TempEndCount - 5; TempPosition++)
				{
					wchar_t TempChar = vTextString.GetAt(TempPosition);
					wchar_t TempNextChar = vTextString.GetAt(TempPosition + 1);
					if ((TempChar == '+') || ((TempChar == '-') && (TempNextChar == '-')))
					{
						int TempStop = vTextString.Find((char)0x0A,TempPosition);
						if (TempStop > TempEndCount)
							TempStop = TempEndCount;
						for (int TempOffset = TempPosition; TempOffset <= TempStop; TempOffset++)
						if (TempOffset < cSizeOfVColor)
							vColor[TempOffset] = 3; //green
							//vColor[TempOffset] = 2; //blue
							//vColor[TempOffset] = 1; //red
						TempPosition = TempStop;
					}
					else
					if (TempChar == '*')
					{
						int TempStop = vTextString.Find((char)0x0A,TempPosition);
						if (TempStop > TempEndCount)
							TempStop = TempEndCount;
						for (int TempOffset = TempPosition; TempOffset <= TempStop; TempOffset++)
						if (TempOffset < cSizeOfVColor)
							vColor[TempOffset] = 1; //red
						//vColor[TempOffset] = 3; //green
						//vColor[TempOffset] = 2; //blue
						TempPosition = TempStop;
					}
				}
				DisplayColor(&vTextString);
			}
		}
	}
	if (vWindowType == cMainHelpWindow)
	{
		m_HelpTitle.ShowWindow(SW_HIDE);
		m_HelpTextControl.ShowWindow(SW_HIDE);
	}

	if ((vLocalSystemData) && (vLocalSystemData->vInAutoSetup))
	if (vWindowType != cTextWindow)
	{
		vMakeExitButtonGreen = true;
		m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_Function1Button, _T("Return to Auto Setup"));
	}
	if (vLocalSystemData)
		vOldErrorLogCount = vLocalSystemData->vErrorCount;

	UpdateButtons();
	//vGlobalMouseMoveCounter = 0;

	if (vACPowerMoniterFormat)
	{
		m_Function3Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_Function3Button, _T("Previous Day's Log"));
		m_Function5Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_Function5Button, _T("Next\nDay's Log"));
	}

	int TimerResult = SetTimer(vOneSecondTimerHandle,1000,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-One Second Timer Failed",cEMailInspx,32000);

	vDoNotClickUpperYHelp = (WORD) ResizeYCoor(42);
	vDoNotClickLowerYHelp = (WORD) ResizeYCoor(412);
	vDoNotClickLeftXHelp = (WORD) ResizeXCoor(105);
	vDoNotClickRightXHelp = (WORD) ResizeXCoor(533);

	::SetupMenu(vLocalCWndCollection);
}

void CHelpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHelpDialog)
	DDX_Control(pDX, IDC_HelpText, m_HelpTextControl);
	DDX_Control(pDX, IDC_HelpTitle, m_HelpTitle);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	//DDX_Text(pDX, IDC_HelpText, m_HelpText);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_RemoteKeyLabelsButton, m_RemoteKeyLabelsButton);
}


BEGIN_MESSAGE_MAP(CHelpDialog, CDialog)
	//{{AFX_MSG_MAP(CHelpDialog)
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
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CHelpDialog::OnDialogtitlestatictext1)
	ON_STN_DBLCLK(IDC_SubFunction5Button, &CHelpDialog::OnDoubleClickSubFunction5Button)
	ON_STN_DBLCLK(IDC_SubFunction6Button, &CHelpDialog::OnDoubleClickSubFunction6Button)
	ON_STN_DBLCLK(IDC_SubFunction7Button, &CHelpDialog::OnDoubleClickSubFunction7Button)
	ON_STN_DBLCLK(IDC_SubFunction8Button, &CHelpDialog::OnDoubleClickSubFunction8Button)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelpDialog message handlers

void CHelpDialog::OnFunction1Button() 
{
	vFindTextShow = cFindTextNoHighlight;
	
	// User clicked exit Button
	//vGlobalMouseMoveCounter = 0;
	if (vWindowType == cRemoteKeyLabelWindow)
	{
		vWindowType = cMainHelpWindow;
		m_HelpTextControl.ShowWindow(SW_HIDE);
		ShowHelp();
		UpdateButtons();
	}
	else
		CDialog::EndDialog(true);
}

void CHelpDialog::OnFunction2Button() 
{
	//vGlobalMouseMoveCounter = 0;
	
	if (vWindowType == cTextWindow)
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
			WriteStringToFile(vTitleString, vTextString);
	}
	else
	if (vWindowType == cHelpWindow)
	{
		//show using ScanTrac Text
		//m_HelpTextControl.LineScroll(-1000);
		//vHelpContext = 0;
		//vHelpType = 0;
		//ShowHelp();
		vWindowType = cMainHelpWindow;
		m_HelpTitle.ShowWindow(SW_HIDE);
		m_HelpTextControl.ShowWindow(SW_HIDE);
		vHelpType = cOverview;
		UpdateButtons();
	}
	else
	if (vWindowType == cErrorLogWindow)
	//if (PasswordOK(cTemporaryInspxPassword,false))
	{
		//vLocalConfigurationData->vWriteErrorLogOnDisk = ! vLocalConfigurationData->vWriteErrorLogOnDisk;
		//UpdateButtons();
		//kjh 7/21/10 for now setup for find windows here
		// want to call Alpha Entry and get an entry
		CAlphaEntryDialog IAlphaEntryDialog;  
		IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
		IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
		//Set dialog box data titles and number value
		IAlphaEntryDialog.vEditString = vFindString;
		IAlphaEntryDialog.m_DialogTitleStaticText = "";
		IAlphaEntryDialog.vFindTextMode = true;
		IAlphaEntryDialog.m_DialogTitleStaticText2 = " ";
		IAlphaEntryDialog.vAllowDashes = true;
		IAlphaEntryDialog.vAllowPeriod = true;
		IAlphaEntryDialog.vBlankNameOK = true;
		IAlphaEntryDialog.vNoMainMenu = true;
		//Pass control to dialog box and display
		int nResponse = IAlphaEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vFindString = IAlphaEntryDialog.vEditString;
			vStartFindSearch = true;
			if (vErrorMessageDisplayMask == cInspxLogMask)
				FindHelpText();		//m_HelpTextControl
			else
				FindHelpEventLogText();
			vStartFindSearch = false;
		}
	}
}

void CHelpDialog::OnFunction3Button() 
{
	// Using ScanTrac Help
	vFindTextShow = cFindTextNoHighlight;
	//vGlobalMouseMoveCounter = 0;
	if (vWindowType == cSourceStrengthWindow)
	{
		if (PasswordOK(cTemporaryInspxPassword, false))
		{
			vSourceStrengthDisplayMode++;
			if (vSourceStrengthDisplayMode > cDisplayRawValues)
				vSourceStrengthDisplayMode = cDisplayPercentOfInitial;

			if ((vSourceStrengthDisplayMode == cDisplayPercentOfInitial) && (vLocalConfigurationData) && (!vLocalConfigurationData->vSourceStrengthInitialValues[0]))
			{
				vSourceStrengthDisplayMode = cDisplayPercentOfStandard;
				if (PasswordOK(cTemporaryInspxPassword,false))
				{
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nInitial values have not been saved,\nshowing % of Standard Values.";
					TempNoticeDialog.vType = cNoticeMessage;
					TempNoticeDialog.DoModal();
				}
			}
			this->SetWindowText(_T("Source Strength"));
			ShowSourceStrengthData();
		}
	}
	else
	if (vACPowerMoniterFormat)
	{
		CDialog::EndDialog(37);
	}
	else
	if ((vWindowType == cErrorLogWindow) && (vErrorMessageDisplayMask != cInspxLogMask))
	{
		ShowPreviousErrorLog();
		UpdateButtons();
	}
	else
	if (vWindowType == cMainHelpWindow)
	//if (PasswordOK(cTemporaryInspxPassword,false))
	{
		CWnd *TempWindow = NULL;
		TempWindow = FindWindow(NULL, _T("Diagnostics"));
		if (TempWindow) 
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nA Diagnostics window is currently open.\nClosing help to return to it.";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
			CDialog::EndDialog(true);
		}
		else
		if (vMainWindowPointer)
		{
			vMainWindowPointer->DoDiagnostics();
			CDialog::EndDialog(true);
		}
	}
	else
	if (vWindowType == cProductionReportWindow)		//kjh use default directory for display report.
	{
		char TempFieldSeperator = ',';
		if (vGlobalUseCommaForDecimalPoint)
			TempFieldSeperator = ';';

		bool TempAtFirstProductionReport = false;
		CString TempLastReportString = "";
		CString TempEndTimeString = "";
		CFileException TempFileException;
		CString TempFilePath = cScanTracProductionReportDirectory;
		TempFilePath += "ScanTracProductionReportSummary.CSV";
		LPCTSTR TempFileName = TempFilePath;
		CFileStatus TempFileStatus;
		ULONGLONG TempFileSize = 0;
		if (CFile::GetStatus(TempFileName, TempFileStatus))
			TempFileSize = TempFileStatus.m_size;

		if (ThereIsEnoughMemory(sizeof(CFile), "Serial Data Log File2"))
		{
			CFile TempProductionReportFile;
			if (TempProductionReportFile)
			if (TempProductionReportFile.Open(TempFileName, (CFile::modeRead | CFile::shareDenyNone), &TempFileException))
			{
				ULONGLONG TempReadCount = 0;
				ULONGLONG TempLength;
//				ULONGLONG TempMaxFileLength;
//				TempMaxFileLength = TempProductionReportFile.GetLength();
				int i;
				char TempChars[20];
				CString TempProductionReportDataString = "";
				int TempDataCount = 0;
				bool TempThisIsTheFirstLoop = true;
				bool TempNewHeaderIsAvailable = false;
				CString TempProductDataString = "";

				for (i = 0; i < 20; i++)
					TempChars[i] = 0;

				i = -1;
				do
				{
					if (i != -1)
					{
						TempNewHeaderIsAvailable = true;
						vProductionReportEndIndex = vProductionReportStartIndex;
					}

					TempProductionReportDataString = " ";
					TempDataCount = 0;

					for (i++ ; i < 100; i++) //condition is random, the code will break out of the for loop below
					{
						if ((i%10) == 0)
						{
							if (TempThisIsTheFirstLoop == true)
							{
								TempThisIsTheFirstLoop = false;

								TempProductionReportFile.Seek(vProductionReportStartIndex, CFile::begin);
								if (vProductionReportStartIndex < (vProductionReportHeaderString.GetLength() + 10))
								{
									TempAtFirstProductionReport = true;
	//								CNoticeDialog TempNoticeDialog;
	//								TempNoticeDialog.vNoticeText = "\n\n\nOldest Production Report.";
	//								TempNoticeDialog.vType = cInformationMessage;
	//								TempNoticeDialog.DoModal();
									break;
								}
								vProductionReportEndIndex = vProductionReportStartIndex;
							}
							else
							if (vProductionReportStartIndex >= 20)
								vProductionReportStartIndex = TempProductionReportFile.Seek(-20, CFile::current);
							else
								vProductionReportStartIndex = TempProductionReportFile.Seek((-1*vProductionReportStartIndex), CFile::current);

							TempLength = TempProductionReportFile.Read(TempChars,10);
							TempReadCount = TempReadCount + TempLength;
							if (TempLength < 10) //something not right, didn't get back expected amount of chars from read
								break;

							i = 0;
						}

						if (TempChars[9 - i] == TempFieldSeperator)
						{
							TempDataCount++;
						}

						if (TempChars[9 - i] == (char)13)
						{
							if (TempDataCount < 5)
							{
								TempDataCount = 0;
								TempProductionReportDataString = " ";
								continue;
							}
							else
								break;
						}

						if (TempChars[9 - i] == (char)10)
							continue;

						TempProductionReportDataString.Insert(0,TempChars[9-i]);
					}
				}
				while ((TempProductionReportDataString.Find(_T("Product Code")) != -1) && (TempReadCount < TempFileSize));

				if (TempNewHeaderIsAvailable)
				{
					vProductionReportHeaderString = " ";
					int TempDataCount = 0;

					vProductDataIndex = 0;
					vHeaderCount = 1;
					vEndTimeIndex = 0;

					ULONGLONG TempFileIndex = 0;
					bool TempThisIsTheFirstLoop = true;
					ULONGLONG ByteCountTotal = 0;
			

					while ((vProductionReportHeaderString.Find(_T("Product Code")) == -1) && (ByteCountTotal < TempFileStatus.m_size))
					{
						vProductionReportHeaderString = " ";

						for (i++; i < 100; i++) //condition is random, the code will break out of the for loop below
						{
							if ((i%10) == 0)
							{
								i = 0;
								if (TempThisIsTheFirstLoop == true)
								{
									TempFileIndex = TempProductionReportFile.Seek(-10, CFile::current);
									TempThisIsTheFirstLoop = false;

									TempLength = TempProductionReportFile.Read(TempChars,10);
									ByteCountTotal = ByteCountTotal + TempLength;
									if (TempLength < 10) //something not right, didn't get back expected amount of chars from read
										break;
								}
								else
								{
									TempFileIndex = TempProductionReportFile.Seek(0, CFile::current);

									if (TempFileIndex >= 20)
									{
										TempFileIndex = TempProductionReportFile.Seek(-20, CFile::current);
										TempLength = TempProductionReportFile.Read(TempChars,10);
										ByteCountTotal = ByteCountTotal + TempLength;
										if (TempLength < 10) //something not right, didn't get back expected amount of chars from read
											break;
									}
									else 
									if (TempFileIndex > 10)
									{
										TempProductionReportFile.SeekToBegin();
									
										TempLength = TempProductionReportFile.Read(TempChars, UINT(TempFileIndex-10));
										ByteCountTotal = TempFileStatus.m_size;

										i = (int)(20 - TempFileIndex);
										if ((int) TempLength < (TempFileIndex-10)) //something not right, didn't get back expected amount of chars from read
											break;

										TempFileIndex = 0;
									}
									else
									{
										ByteCountTotal = TempFileStatus.m_size;
										break;
									}
								}
							}

							if (TempChars[9 - i] == (char)13)
									break;

							if (TempChars[9 - i] == (char)10)
								continue;

							vProductionReportHeaderString.Insert(0,TempChars[9-i]);
						}
					}

					for (i = 0; i < vProductionReportHeaderString.GetLength(); i++)
					{
						if (vProductionReportHeaderString.GetAt(i) == TempFieldSeperator)
						{
							if (vEndTimeIndex == 0)
							if (vProductionReportHeaderString.Find( _T("End Time"), i) == -1) //if you just passed end time
								vEndTimeIndex = vHeaderCount;

							if (vProductDataIndex == 0) 
							{
								CString TempProductWithSeparators = _T(",Product,");
								if (vGlobalUseCommaForDecimalPoint)
									TempProductWithSeparators = _T(";Product;");

								if (vProductionReportHeaderString.Find(TempProductWithSeparators, i+1) == -1) //if you just passed product, -1 is returned when it cannot find the string anymore
								{ 
									if (vProductionReportHeaderString.GetLength() > i + 8)
										vProductionReportHeaderString.Delete(i , 8); //delete word product and the comma
									vProductDataIndex = vHeaderCount;
								}
								else
									vHeaderCount++;
							}
							else
								vHeaderCount++;
						}
					}
					vHeaderCount++;

					//vProductionReportHeaderString = "Product" + TempFieldSeperator + vProductionReportHeaderString;
					vProductionReportHeaderString = TempFieldSeperator + vProductionReportHeaderString; //brh try this
					vProductionReportHeaderString = "Product" + vProductionReportHeaderString; //brh try this
				}

				if (vProductDataIndex < TempDataCount)
				{
					int TempIndexOfFirstCharacterOfProductData = 0;
					//put header into own string
					for (i = 0; i < vProductDataIndex; i++)
							TempIndexOfFirstCharacterOfProductData = TempProductionReportDataString.Find(TempFieldSeperator, TempIndexOfFirstCharacterOfProductData) + 1;
					
					int TempIndexOfLastCharacterOfProductData = TempProductionReportDataString.Find(TempFieldSeperator,TempIndexOfFirstCharacterOfProductData);
					int TempLengthOfProductDataString = TempIndexOfLastCharacterOfProductData - TempIndexOfFirstCharacterOfProductData;

					TempProductDataString = TempProductionReportDataString.Mid(TempIndexOfFirstCharacterOfProductData, TempLengthOfProductDataString);
					TempProductionReportDataString.Delete(TempIndexOfFirstCharacterOfProductData, TempLengthOfProductDataString + 1); //plus one for the extra comma
				}
				if (vEndTimeIndex < TempDataCount)
				{
					int TempIndexOfFirstCharacterOfEndTime = 0;
					//put header into own string
					for(i = 0; i < vEndTimeIndex - 1; i++)
							TempIndexOfFirstCharacterOfEndTime = TempProductionReportDataString.Find(TempFieldSeperator, TempIndexOfFirstCharacterOfEndTime) + 1;
					
					int TempIndexOfLastCharacterOfEndTime = TempProductionReportDataString.Find(TempFieldSeperator,TempIndexOfFirstCharacterOfEndTime);
					int TempLengthOfEndTimeString = TempIndexOfLastCharacterOfEndTime - TempIndexOfFirstCharacterOfEndTime;

					TempEndTimeString = " " + TempProductionReportDataString.Mid(TempIndexOfFirstCharacterOfEndTime, TempLengthOfEndTimeString);
				}

				TempProductionReportDataString = TempProductDataString+ "," + TempProductionReportDataString;
				
	//			if(TempDataCount != vHeaderCount)
	//			{
	//				CNoticeDialog TempNoticeDialog;
	//				TempNoticeDialog.vNoticeText = "\n\n\nHeader Values and Data Values do not match.";
	//				TempNoticeDialog.vType = cInformationMessage;
	//				TempNoticeDialog.DoModal();
	//			}

				int TempHeaderNextCommaIndex = 0;
				int TempDataNextCommaIndex = 0;
				CString TempHeaderCellString = "";
				CString TempDataCellString = "";
				CString TempString = vProductionReportHeaderString;
				bool TempInsertNewLineNext = false;

				for (i = 0; (i <= vHeaderCount) || (i <= TempDataCount); i++)
				{
					if (i < vHeaderCount)
					{
						TempHeaderNextCommaIndex = TempString.Find(TempFieldSeperator);
						if (TempHeaderNextCommaIndex > 0)
						{
							TempHeaderCellString = TempString.Left(TempHeaderNextCommaIndex);
							TempString.Delete(0,TempHeaderNextCommaIndex+1);
						}

						if (TempHeaderCellString == "Total Containers")
							TempHeaderCellString = "\nTotal Containers";
						if (TempHeaderCellString == "Total")
							TempHeaderCellString = "\nTotal";
						if (TempHeaderCellString.Find( _T("CPM")) > 0)
							TempInsertNewLineNext = true;

					}
					else 
					if (i == vHeaderCount)
						TempHeaderCellString = TempString;
					else
						TempHeaderCellString = "Unnamed Inspection";

					if (i < TempDataCount)
					{
						TempDataNextCommaIndex = TempProductionReportDataString.Find(TempFieldSeperator);
						if (TempDataNextCommaIndex > 0)
						{
							TempDataCellString = TempProductionReportDataString.Left(TempDataNextCommaIndex);
							TempProductionReportDataString.Delete(0, TempDataNextCommaIndex+1);
						}
					}
					else 
					if (i == TempDataCount)
						TempDataCellString = TempProductionReportDataString;
					else
						TempDataCellString = "Not Available";

					TempLastReportString = TempLastReportString + TempHeaderCellString + " : " + TempDataCellString + "\n";

					if (TempInsertNewLineNext == true)
					{
						TempLastReportString = TempLastReportString +"\n";
						TempInsertNewLineNext = false;
					}
				}
				if (vProductionReportStartIndex < (vProductionReportHeaderString.GetLength() + 10))
				{
					TempAtFirstProductionReport = true;
				}
				TempProductionReportFile.Close();
			}
			else
			{
				ReportErrorMessage("Could Not Read Production Report 1",cEMailInspx, 32000);
				TempLastReportString = "No Previous Production Report";
			}
		}
		if (TempAtFirstProductionReport == true)
		{
			if(!m_Function3Button.ShowWindow(SW_HIDE) ) //show window hide returns 0 if it was already invisible
			{
				this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
				return;
			}
		}
		
		SetDlgItemText(IDC_HelpTitle, "Production Report" + TempEndTimeString);
		m_HelpTextControl.LineScroll(-9999);
		SetDlgItemText(IDC_HelpText, TempLastReportString);			
		m_Function5Button.ShowWindow(SW_SHOW);
			
		
//		int vTimerResult = SetTimer(vHideCaretTimerHandle,10,NULL);
		UpdateButtons();

		CHARFORMAT TempCharacterFormat;
		TempCharacterFormat.dwMask = CFM_COLOR ;
		TempCharacterFormat.dwEffects = NULL;
		TempCharacterFormat.crTextColor = cBlack;
		m_HelpTextControl.SetDefaultCharFormat(TempCharacterFormat);

	}
	else
	if (vWindowType == cHelpWindow)
	{
		//show using ScanTrac Text
		m_HelpTextControl.LineScroll(-1000);
		vHelpContext = 0;
		vHelpType = 0;
		ShowHelp();
	}
	else
	if (vWindowType == cTextWindow)
	{
		if (vHistogramData)
		{
			CHistogramDisplayDialog IHistogramDisplayDialog;
			IHistogramDisplayDialog.vNumberOfNumbers = 256;
			IHistogramDisplayDialog.vValuesAreIntegers = true;
			IHistogramDisplayDialog.vDataTitle = "Pixel Values in ROI";
			IHistogramDisplayDialog.vHistogramData = vHistogramData;
			IHistogramDisplayDialog.vLocalSystemData = vLocalSystemData;
			IHistogramDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
			IHistogramDisplayDialog.vMainWindowPointer = vMainWindowPointer;
			int nResponse = IHistogramDisplayDialog.DoModal();
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
	}
}

void CHelpDialog::OnFunction4Button() 
{
	//m_HelpTextControl.HideCaret();
	//Top or Bottom Button
	vLastFindPosition = 0;		// if significant change to display window want to restart find location
	//vGlobalMouseMoveCounter = 0;
	CString TempButtonCString;

	GetDlgItemTextW(IDC_Function4Button, TempButtonCString);
//	if ((vWindowType == cErrorLogEventLogWindow) && (TempButtonCString.CompareNoCase("Goto Top") == 0))
	if ((vWindowType == cErrorLogWindow) && (vErrorMessageDisplayMask != cInspxLogMask) && (TempButtonCString.CompareNoCase(_T("Goto Top")) == 0))
	{
		CClientDC dc(this);
		m_HelpTextControl.SetFont(&vFontToUse, true);

		CHARFORMAT TempCharacterFormat;
		TempCharacterFormat.dwMask = CFM_COLOR ;
		TempCharacterFormat.dwEffects = NULL;
		TempCharacterFormat.crTextColor = cBlack;
		m_HelpTextControl.SetDefaultCharFormat(TempCharacterFormat);

		vStartFindPosition = 0;
		vLastFindPosition = 0;		// if significant change to display window want to restart find location
		vLinesInCurrentDayEventLog = -1;


		if ((vErrorMessageDisplayMask & 0x184) == 0x080)
			vDialogTitleString = "Reject Log";
		else
		{
			vDialogTitleString = "Event Log";
		}
		SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);

		if (vLineIndexInFile)
			free(vLineIndexInFile);
		if (ThereIsEnoughMemory(sizeof(ULONGLONG) * 1001, "Event Log Dialog File to string index"))
		{
			vLineIndexInFile = (ULONGLONG *)(malloc(sizeof(ULONGLONG) * 1001));
			int i;
			for (i = 0; i < 1000; i++)
				vLineIndexInFile[i] = 0;
		}
		else
		{
			ReportErrorMessage("Not Enough Memory for EventLog Alloc",cEMailInspx, 32000);
			#ifdef _DEBUG
			afxDump << "Not Enough Memory for EventLog Alloc";
			#endif
		}
		vLastFileIndex = 0;
		m_SubFunction1Button.ShowWindow(SW_SHOW);
		m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_SubFunction1Button, _T("Clear Reject- Event Log"));

		SetDlgItemText(IDC_SubFunction3Button, _T("Event Types To Display"));

		SetDlgItemText(IDC_Function2Button, _T("Find Text"));
		m_Function2Button.ShowWindow(SW_SHOW);

		ShowErrorLog(false); //do refresh  <<<<<<<<<<<<<-------a true resets file directory queue

		int vTimerResult = SetTimer(vCheckErrorMessagesTimerHandle,1000,NULL);


		if (vLocalSystemData)
			vOldErrorLogCount = vLocalSystemData->vErrorCount;

		UpdateButtons();
		//vGlobalMouseMoveCounter = 0;
	}
	else 
	{
	if (vWindowType != cRemoteKeyLabelWindow)
	{
		int TempTotalLines = m_HelpTextControl.GetLineCount();
		int TempCurrentLine = m_HelpTextControl.GetFirstVisibleLine();
		if ((vWindowType == cErrorLogWindow) && (vErrorMessageDisplayMask != cInspxLogMask))
		{
			//if (TempCurrentLine == 0)
			{//go to bottom
				ShowEndOfEventLog();
				TempCurrentLine = m_HelpTextControl.GetFirstVisibleLine();
				TempTotalLines = m_HelpTextControl.GetLineCount();
				// want to display from line number TempTotalLines - vLinesOnScreen
				// at line number TempCurrentLine
				if (TempTotalLines > vLinesOnScreen)
				{
					m_HelpTextControl.LineScroll(-TempCurrentLine);
					m_HelpTextControl.LineScroll(TempTotalLines - (/*TempCurrentLine +*/ 1 + vLinesOnScreen));
				}
				
				UpdateButtons();

//				// if find active display of end carat moved, reset kjh
//				TempCurrentLine = m_HelpTextControl.GetFirstVisibleLine();
//				TempTotalLines = m_HelpTextControl.GetLineCount();
//				m_HelpTextControl.LineScroll(TempTotalLines - (1 + vLinesOnScreen));
				
				m_SubFunction7Button.ShowNavigationArrow(NULL);
				m_SubFunction8Button.ShowNavigationArrow(NULL);
				m_SubFunction7Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
				m_SubFunction8Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());

//				m_HelpTextControl.LineScroll(-1);
//				m_HelpTextControl.HideSelection(false, false);

//				m_HelpTextControl.HideCaret();
//				this->Invalidate(false);

			}
			//else
			//{//goto top
			//	ShowErrorLog(false); //do not do refresh
			//	m_HelpTextControl.LineScroll(-TempCurrentLine);
			//	UpdateButtons();
			//}
			//	m_HelpTextControl.LineScroll(-1);
			//m_HelpTextControl.HideSelection(false, false);  <<< first false, causes find selection to be highlight, cursor active
		}
		else
		{
			if (TempTotalLines > vLinesOnScreen)
			{
				if (TempCurrentLine == 0) 
				{//go to bottom
					m_HelpTextControl.LineScroll(TempTotalLines - (TempCurrentLine + 1 + vLinesOnScreen));
				}
				else
				{//goto top
					m_HelpTextControl.LineScroll(-TempCurrentLine);
				}
//				m_HelpTextControl.HideCaret();
				UpdateButtons();
			}
		}
	}
	}

}

void CHelpDialog::OnFunction5Button() 
{
	vFindTextShow = cFindTextNoHighlight;
 	//vGlobalMouseMoveCounter = 0;
	if (vACPowerMoniterFormat)
	{
		CDialog::EndDialog(39);  //arrow Right
	}
	if (vWindowType == cSourceStrengthWindow)
	{
		if (vLocalSystemData->vPixelValuesString != "")
		{
			CHelpDialog IHelpDialog;
			IHelpDialog.vHelpContext = vHelpContext;
			IHelpDialog.vHelpType = 0;
			IHelpDialog.vWindowType = cTextWindow;
			IHelpDialog.vLocalSystemData = vLocalSystemData;
			IHelpDialog.vMainWindowPointer = vMainWindowPointer;
			IHelpDialog.vLocalConfigurationData = vLocalConfigurationData;
			IHelpDialog.vProductPointer = vProductPointer;
			IHelpDialog.vTextString = vLocalSystemData->vPixelValuesString + "\n\n";
			if (vLocalSystemData->vPixelValuesCorrectedPixels)
				IHelpDialog.vTitleString = "Corrected Pixel Values";
			else
				IHelpDialog.vTitleString = "Un-Corrected Pixel Values";
			int TempResult = IHelpDialog.DoModal();
		}
	}
	else
	if (vWindowType == cProductionReportWindow)
	{
		char TempFieldSeperator = ',';
		if (vGlobalUseCommaForDecimalPoint)
			TempFieldSeperator = ';';

		bool TempAtLastProductionReport = false;
		CString TempLastReportString = "";
		CString TempEndTimeString = "";
		CFileException TempFileException;
		CString TempFilePath = cScanTracProductionReportDirectory;
		TempFilePath += "ScanTracProductionReportSummary.CSV";
		LPCTSTR TempFileName = TempFilePath;
		CFileStatus TempFileStatus;
		ULONGLONG TempFileSize = 0;
		if (CFile::GetStatus(TempFileName, TempFileStatus))
			TempFileSize = TempFileStatus.m_size;


		if (ThereIsEnoughMemory(sizeof(CFile), "Serial Data Log File2"))
		{
			CFile TempProductionReportFile;
			if (TempProductionReportFile)
			if (TempProductionReportFile.Open(TempFileName, (CFile::modeRead | CFile::shareDenyNone), &TempFileException))
			{
				UINT TempLength;
				int i;
				char TempChars[20];
				CString TempProductionReportDataString = " ";
				int TempDataCount = 0;

				bool TempThisIsTheFirstLoop = true;
				CString TempProductDataString = "";
				for (i = 0; i < 20; i++)
					TempChars[i] = 0;

				bool TempNewHeaderIsAvailable = false;
				bool TempReachedEndOfFile = false;

				for (i = 0; i < 20; i++)
					TempChars[i] = 0;

				ULONGLONG TempByteCount = 0;
				i = -1;
				do
				{
					if (i != -1)
					{
						vProductionReportHeaderString = TempProductionReportDataString;
						TempNewHeaderIsAvailable = true;

						i = -1;
						TempThisIsTheFirstLoop = true;
					}

					TempProductionReportDataString = " ";
					TempDataCount = 0;

					for (i++ ; i < 100; i++) //condition is random, the code will break out of the for loop below
					{
						if ((i%10) == 0)
						{
							if (TempThisIsTheFirstLoop == true)
							{
								TempThisIsTheFirstLoop = false;

								TempProductionReportFile.Seek(vProductionReportEndIndex, CFile::begin);

								for (int j = 0; !TempReachedEndOfFile ;j++)
								{
									if (1 != TempProductionReportFile.Read(TempChars,1))
									{
										TempByteCount = TempFileSize;
										TempReachedEndOfFile = true;
										vProductionReportStartIndex = vProductionReportEndIndex = TempProductionReportFile.Seek(-10, CFile::current);
									}
									else
										//TempByteCount = TempFileStatus.m_size; 10/10/2016
										TempByteCount++;

									if ((TempChars[0] == 13) || (TempChars[0] == 10))
									if (j>10)
									{
										vProductionReportStartIndex = vProductionReportEndIndex = TempProductionReportFile.Seek(-10, CFile::current);
										break;
									}
								}
							}
							else
								if (vProductionReportStartIndex >= 20)
									vProductionReportStartIndex = TempProductionReportFile.Seek(-20, CFile::current);
								else
									vProductionReportStartIndex = TempProductionReportFile.Seek((-1*vProductionReportStartIndex), CFile::current);

							TempLength = TempProductionReportFile.Read(TempChars,10);
							TempByteCount = TempByteCount + TempLength;
							if (TempLength < 10) //something not right, didn't get back expected amount of chars from read
								break;

							i = 0;
						}

						if (TempChars[9 - i] == TempFieldSeperator)
						{
							TempDataCount++;
						}

						if (TempChars[9 - i] == (char)13)
						{
							if (TempDataCount < 5)
							{
								TempDataCount = 0;
								TempProductionReportDataString = " ";
								continue;
							}
							else
								break;
						}

						if (TempChars[9 - i] == (char)10)
							continue;

						TempProductionReportDataString.Insert(0,TempChars[9-i]);
					}
				}
				while ((TempProductionReportDataString.Find(_T("Product Code")) != -1) && (TempByteCount < TempFileStatus.m_size));

				if (TempNewHeaderIsAvailable)
				{
					vProductDataIndex = 0;
					vHeaderCount = 1;
					vEndTimeIndex = 0;

					int TempFileIndex = 0;
					for (i = 0; i < vProductionReportHeaderString.GetLength() ; i++)
					{
						if (vProductionReportHeaderString.GetAt(i) == TempFieldSeperator)
						{
							if (vEndTimeIndex == 0)
							if (vProductionReportHeaderString.Find(_T("End Time"), i) == -1) //if you just passed end time
								vEndTimeIndex = vHeaderCount;

							if (vProductDataIndex == 0) 
							{
								CString TempProductWithSeparators = _T(",Product,");
								if (vGlobalUseCommaForDecimalPoint)
									TempProductWithSeparators = _T(";Product;");

								if (vProductionReportHeaderString.Find(TempProductWithSeparators, i+1) == -1) //if you just passed product, -1 is returned when it cannot find the string anymore
								{ 
									if (vProductionReportHeaderString.GetLength() > i + 8)
										vProductionReportHeaderString.Delete(i , 8); //delete word product and the comma
									vProductDataIndex = vHeaderCount;
								}
								else
									vHeaderCount++;
							}
							else
								vHeaderCount++;
						}
					}
					vHeaderCount++;

					//vProductionReportHeaderString = "Product" + TempFieldSeperator + vProductionReportHeaderString;
					vProductionReportHeaderString = TempFieldSeperator + vProductionReportHeaderString; //brh try this
					vProductionReportHeaderString = "Product" + vProductionReportHeaderString; //brh try this
				}

				if (vProductDataIndex < TempDataCount)
				{
					int TempIndexOfFirstCharacterOfProductData = 0;
					//put header into own string
					for (i = 0; i < vProductDataIndex; i++)
							TempIndexOfFirstCharacterOfProductData = TempProductionReportDataString.Find(TempFieldSeperator, TempIndexOfFirstCharacterOfProductData) + 1;
					
					int TempIndexOfLastCharacterOfProductData = TempProductionReportDataString.Find(TempFieldSeperator,TempIndexOfFirstCharacterOfProductData);
					int TempLengthOfProductDataString = TempIndexOfLastCharacterOfProductData - TempIndexOfFirstCharacterOfProductData;

					TempProductDataString = TempProductionReportDataString.Mid(TempIndexOfFirstCharacterOfProductData, TempLengthOfProductDataString);
					TempProductionReportDataString.Delete( TempIndexOfFirstCharacterOfProductData, TempLengthOfProductDataString + 1); //plus one for the extra comma
				}
				if (vEndTimeIndex < TempDataCount)
				{
					int TempIndexOfFirstCharacterOfEndTime = 0;
					//put header into own string
					for (i = 0; i < vEndTimeIndex - 1; i++)
							TempIndexOfFirstCharacterOfEndTime = TempProductionReportDataString.Find(TempFieldSeperator, TempIndexOfFirstCharacterOfEndTime) + 1;
					
					int TempIndexOfLastCharacterOfEndTime = TempProductionReportDataString.Find(TempFieldSeperator,TempIndexOfFirstCharacterOfEndTime);
					int TempLengthOfEndTimeString = TempIndexOfLastCharacterOfEndTime - TempIndexOfFirstCharacterOfEndTime;

					TempEndTimeString = " " + TempProductionReportDataString.Mid(TempIndexOfFirstCharacterOfEndTime, TempLengthOfEndTimeString);
				}

				TempProductionReportDataString = TempProductDataString+ "," + TempProductionReportDataString;
				
	//			if(TempDataCount != vHeaderCount)
	//			{
	//				CNoticeDialog TempNoticeDialog;
	//				TempNoticeDialog.vNoticeText = "\n\n\nHeader Values and Data Values do not match.";
	//				TempNoticeDialog.vType = cInformationMessage;
	//				TempNoticeDialog.DoModal();
	//			}

				int TempHeaderNextCommaIndex = 0;
				int TempDataNextCommaIndex = 0;
				CString TempHeaderCellString = "";
				CString TempDataCellString = "";
				CString TempString = vProductionReportHeaderString;
				bool TempInsertNewLineNext = false;

				for (i = 0; (i <= vHeaderCount) || (i <= TempDataCount); i++)
				{
					if (i < vHeaderCount)
					{
						TempHeaderNextCommaIndex = TempString.Find(TempFieldSeperator);
						if (TempHeaderNextCommaIndex > 0)
						{
							TempHeaderCellString = TempString.Left(TempHeaderNextCommaIndex);
							TempString.Delete(0,TempHeaderNextCommaIndex+1);
						}

						if (TempHeaderCellString == "Total Containers")
							TempHeaderCellString = "\nTotal Containers";
						if (TempHeaderCellString == "Total")
							TempHeaderCellString = "\nTotal";
						if (TempHeaderCellString.Find(_T("CPM")) != -1)
							TempInsertNewLineNext = true;

					}
					else 
					if (i == vHeaderCount)
						TempHeaderCellString = TempString;
					else
						TempHeaderCellString = "Unnamed Inspection";

					if (i < TempDataCount)
					{
						TempDataNextCommaIndex = TempProductionReportDataString.Find(TempFieldSeperator);
						if (TempDataNextCommaIndex > 0)
						{
							TempDataCellString = TempProductionReportDataString.Left(TempDataNextCommaIndex);
							TempProductionReportDataString.Delete(0, TempDataNextCommaIndex+1);
						}
					}
					else 
					if (i == TempDataCount )
						TempDataCellString = TempProductionReportDataString;
					else
						TempDataCellString = "Not Available";

					TempLastReportString = TempLastReportString + TempHeaderCellString + " : " + TempDataCellString + "\n";

					if (TempInsertNewLineNext == true)
					{
						TempLastReportString = TempLastReportString +"\n";
						TempInsertNewLineNext = false;
					}
				}
				if (vProductionReportEndIndex >= (TempProductionReportFile.Seek(0, CFile::end) - 10))
				{
					TempAtLastProductionReport = true;
				}		
				TempProductionReportFile.Close();
			}
			else
			{
				ReportErrorMessage("Could Not Read Production Report 2",cEMailInspx, 32000);
				TempLastReportString = "No Next Production Report";
			}
		}
		if (TempAtLastProductionReport == true)
		{
			if( !m_Function5Button.ShowWindow(SW_HIDE) )
			{
				this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
				return;
			}
		}

		SetDlgItemText(IDC_HelpTitle, "Production Report" + TempEndTimeString);
		m_Function3Button.ShowWindow(SW_SHOW);

		m_HelpTextControl.LineScroll(-9999);
		SetDlgItemText(IDC_HelpText, TempLastReportString);
				
//		int vTimerResult = SetTimer(vHideCaretTimerHandle,10,NULL);
		UpdateButtons();

		CHARFORMAT TempCharacterFormat;
		TempCharacterFormat.dwMask = CFM_COLOR ;
		TempCharacterFormat.dwEffects = NULL;
		TempCharacterFormat.crTextColor = cBlack;
		m_HelpTextControl.SetDefaultCharFormat(TempCharacterFormat);
	}
	else
	if (vWindowType == cErrorLogWindow)
	{
		if (vErrorMessageDisplayMask != cInspxLogMask)
		{
			vSameLogFile = false;		// OnFunction5Button
			if (vLineIndexInFile)
				free(vLineIndexInFile);
			if (ThereIsEnoughMemory(sizeof(ULONGLONG) * 1001, "Event Log Dialog File to string index"))
			{
				vLineIndexInFile = (ULONGLONG *)(malloc(sizeof(ULONGLONG) * 1001));
				//vLineIndexInFile[0] = 1;
				int i;
				for (i = 0; i < 1000; i++)
					vLineIndexInFile[i] = 0;
			}
			else
			{
				ReportErrorMessage("Not Enough Memory for EventLog Alloc",cEMailInspx, 32000);
				#ifdef _DEBUG
				afxDump << "Not Enough Memory for EventLog Alloc";
				#endif
			}
			vLastFileIndex = 0;
			ShowNextErrorLog();
			UpdateButtons();
		}
		/*
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			char TempBuffer[255] = "";
			char *TempOriginalCurrentDirectory = TempBuffer;
			int TempErrorCode = GetCurrentDirectory(255,TempOriginalCurrentDirectory);
			CString TempFileString = vLocalSystemData->vLastTextFileDirectory;
			char TempLastChar = TempFileString.GetAt(TempFileString.GetLength() - 1);
			if (TempLastChar == '\\')
				TempFileString = TempFileString + "*.txt";
			else
			{
				BYTE TempPlace = TempFileString.Find(".txt",0);
				if (TempPlace == 255)
					TempFileString = TempFileString + "\\*.txt";
			}
			CFileDialog *TempFileDialog = new CFileDialog(true,".txt",
				TempFileString, //NULL,
				OFN_HIDEREADONLY,
				NULL,vMainWindowPointer);
			vGlobalInWindowsDialog = true;
			int nResponse = TempFileDialog->DoModal();
			vGlobalInWindowsDialog = false;
			vLocalSystemData->vPostedNewMainImageReadyToDisplayMessage = false;
			if (nResponse == IDOK)
			{
				CString TempString = TempFileDialog->GetFileName();
				LPCTSTR TempFileName = TempString;
				CFileException TempFileException;
				if (ThereIsEnoughMemory(sizeof(CFile), "Text File"))
				{
					CFile TempFile;
					if (TempFile)
					{
						int TempFileOpened = TempFile.Open( TempFileName, (CFile::modeRead | CFile::shareDenyNone), &TempFileException);
						if (TempFileOpened == 0)
						{
							//could not open text file
							ReportErrorMessage("Error-Open File Failed: " + TempString, cWriteToLog,32000);// + TempFileException.m_cause); 
							#ifdef _DEBUG
								afxDump << "Could Not Calibration Data File"  << "\n";
							#endif
						}
						else
						{
							//read data from the file
							TRY
							{
								ULONGLONG TempFileLength = TempFile.GetLength();
								if (ThereIsEnoughMemory(TempFileLength, "Text File Buffer"))
								{
									BYTE *TempReadBuffer = (BYTE *)malloc(TempFileLength);
									if (TempReadBuffer)
									{
										WORD TempLength = TempFile.Read(TempReadBuffer,TempFileLength);

										//display text
										CHelpDialog IHelpDialog;
										IHelpDialog.vWindowType = cTextWindow;
										IHelpDialog.vLocalConfigurationData = vLocalConfigurationData;
										IHelpDialog.vLocalSystemData = vLocalSystemData;
										IHelpDialog.vLocalProductCollection = vLocalProductCollection;
										IHelpDialog.vMainWindowPointer = vMainWindowPointer;

										IHelpDialog.vTitleString= "File: " + TempString;
										if (TempLength)
										{
											IHelpDialog.vTextString = TempReadBuffer;
											IHelpDialog.vTextString = IHelpDialog.vTextString  + "\n\n Length: " +
												dtoa(TempLength,0);
										}
										else 
											IHelpDialog.vTextString = "No Text In File";
										TRY
										{
											TempFile.Close();
										}
										CATCH_ALL(TempFileException)
										{
										 #ifdef _DEBUG
												afxDump << "Could Not Close text File"  << "\n";
										 #endif
										}
										END_CATCH_ALL
										int TempResult = IHelpDialog.DoModal();
										free(TempReadBuffer);
									}
								}
							}
							CATCH_ALL(TempFileException)
							{
							 #ifdef _DEBUG
									afxDump << "Could Not read text File"  << "\n";
							 #endif
							}
							END_CATCH_ALL
						}
					}
				}
			}
			delete TempFileDialog;
			char TempBuffer1[255] = "";
			char *TempOriginalCurrentDirectory1 = TempBuffer;
			TempErrorCode = GetCurrentDirectory(255,TempBuffer1);
			vLocalSystemData->vLastTextFileDirectory = TempBuffer1;
			TempErrorCode = SetCurrentDirectory(TempOriginalCurrentDirectory);
		}
		*/
	}
	else
	if (vWindowType == cHelpWindow)
	{
		//Help Index button
		CHelpIndexDialog IHelpIndexDialog;  
		//Set dialog box data titles and number value
		int nResponse = IHelpIndexDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			m_HelpTextControl.LineScroll(-1000);
			vHelpContext = IHelpIndexDialog.vNumberSelected;
			vHelpType = cOverview;
			ShowHelp();
		}
		else
		if (nResponse == 11)
		{
			m_HelpTextControl.LineScroll(-1000);
			vHelpContext = 28; //help on help index
			vHelpType = cOverview;
			ShowHelp();
		}
	}
}

void CHelpDialog::OnSubFunction1Button() 
{
	vFindTextShow = cFindTextNoHighlight;
	//vGlobalMouseMoveCounter = 0;
	if (vWindowType == cMainHelpWindow)
	{
		::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
		CDialog::EndDialog(true);
	}
	else
	if (vWindowType == cHelpWindow)
	{
		// functions button pressed
		m_HelpTextControl.LineScroll(-1000);
		vHelpType = cFunctions;
		ShowHelp();
	}
	if (vWindowType == cProductionReportWindow)
	{
		if (vLocalSystemData->vTotalContainers > 0)
		{
			vMainWindowPointer->GenerateProductionReport(cManualProductionReport);
			vMainWindowPointer->SendProductionReport(cManualProductionReport);
			//vMainWindowPointer->SaveReportStartTime();
			ShowProductionReport();
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nNo production complete to report";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
	}
	if (vWindowType == cErrorLogWindow)
	if (vErrorMessageDisplayMask == cInspxLogMask)
	{ //inspx log
		//add comment to inspx log
		CAlphaEntryDialog IAlphaEntryDialog;  
		IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
		//Set dialog box data titles and number value
		IAlphaEntryDialog.vEditString = "";
		IAlphaEntryDialog.m_DialogTitleStaticText = "Enter Comment to put in Inspx Log.";

		IAlphaEntryDialog.m_DialogTitleStaticText2 = "Comment will be time stamped";
		IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
		IAlphaEntryDialog.vAcceptAllKeys = true;
		IAlphaEntryDialog.vAllowPeriod = true;
		IAlphaEntryDialog.vAllowMoreThan32Characters = true;
		IAlphaEntryDialog.vAllowDashes = true;
		IAlphaEntryDialog.vAllowSlashes = true;
		//Pass control to dialog box and display
		int nResponse = IAlphaEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			ReportErrorMessage("Inspx Comment: " + IAlphaEntryDialog.vEditString, cEMailInspx,32002); //cWriteToLog);
			//m_SubFunction2Button.ShowWindow(SW_SHOW);
			//m_ArrowLeft2.ShowWindow(SW_SHOW);
			//SetDlgItemText(IDC_SubFunction2Button,"Update Event Display");
			int vTimerResult = SetTimer(vUpdateInspxLogDisplay,1000,NULL);
		}
		else
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
	else
	{  //not an inspx log
/*		CTime TempTime;
		TempTime = CTime::GetCurrentTime();
		CString TempTimeString = TempTime.Format("%#I:%M:%S%p, %B %d, %Y");
		vLocalSystemData->vErrorLogStarted = TempTimeString;
		SetDlgItemText(IDC_HelpTitle,"Log Started: " + vLocalSystemData->vErrorLogStarted);
*/
		vLocalSystemData->vErrorLog.RemoveAll();
		vLocalSystemData->vErrorCount = 0;
		ShowErrorLog(true); //Do Refresh
	}
	//if ((vWindowType == cProductInformationWindow) ||
	//	(vWindowType == cSourceStrengthWindow))
	//{
	//	vWindowType = cProductionReportWindow;
	//	ShowProductionReport();
	//}
}

void CHelpDialog::OnSubFunction2Button() 
{
	vFindTextShow = cFindTextNoHighlight;
	//vGlobalMouseMoveCounter = 0;
	if(vACPowerMoniterFormat == true)
	{
		CFileStatus TempStatus;
		if(CFile::GetStatus( vACPowerMoniterErrorLogFile, TempStatus ))
		if(TempStatus.m_size != vOldACLogFileSize)
		{
			vOldACLogFileSize = TempStatus.m_size;
			CDialog::EndDialog(1000);
		}
	}

	if (vWindowType == cMainHelpWindow)
	{
		CAlphaEntryDialog IAlphaEntryDialog;  
		IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
		//Set dialog box data titles and number value
		IAlphaEntryDialog.vEditString = "";
		IAlphaEntryDialog.m_DialogTitleStaticText = "Enter Comment to put in Event Log.";

		IAlphaEntryDialog.m_DialogTitleStaticText2 = "Comment will be time stamped";
		IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
		IAlphaEntryDialog.vAcceptAllKeys = true;
		IAlphaEntryDialog.vAllowPeriod = true;
		IAlphaEntryDialog.vAllowMoreThan32Characters = true;
		IAlphaEntryDialog.vAllowDashes = true;
		IAlphaEntryDialog.vAllowSlashes = true;
		//Pass control to dialog box and display
		int nResponse = IAlphaEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			ReportErrorMessage("Operator Comment: " + IAlphaEntryDialog.vEditString, cUserChanges,0); //cWriteToLog);
		}
		else
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
	else
	if (vWindowType == cHelpWindow)
	{
		// Hidden functions button pressed
		m_HelpTextControl.LineScroll(-1000);
		vHelpType = cHiddenFunctions;
		ShowHelp();
	}
	if (vWindowType == cErrorLogWindow)
	{
		ShowErrorLog(true); //do refresh
	}
}

void CHelpDialog::OnSubFunction3Button() 
{
	vFindTextShow = cFindTextNoHighlight;
	//vGlobalMouseMoveCounter = 0;
	if (vWindowType == cMainHelpWindow)
	{
	}
	else
	if (vWindowType == cErrorLogWindow)
	{
		if (vErrorMessageDisplayMask != cInspxLogMask)
		{
			CSelectMultipleItemDialog ISelectItemDialog;  
			
			//Set dialog box data titles and number value
			ISelectItemDialog.m_DialogTitleStaticText = "Select Event Types To Display";
			ISelectItemDialog.vNumberOfItems = 5;
			ISelectItemDialog.vInAutoSetup = false;
			ISelectItemDialog.vUseSelectText = false;

			ISelectItemDialog.vItems[0] = vErrorMessageDisplayMask & 0x0080; //reject
			ISelectItemDialog.vItemsYes[0] = "Selected-Show Rejects";
			ISelectItemDialog.vItemsNo[0] = "Not Selected-Show Rejects";
			ISelectItemDialog.vItems[1] = BYTE ((vErrorMessageDisplayMask & 0x0100) >> 8);
			ISelectItemDialog.vItemsYes[1] = "Selected-Show Actions";
			ISelectItemDialog.vItemsNo[1] = "Not Selected-Show Actions";
			ISelectItemDialog.vItems[2] = vErrorMessageDisplayMask & 0x0004;
			ISelectItemDialog.vItemsYes[2] = "Selected-Show Wrong Size";
			ISelectItemDialog.vItemsNo[2] = "Not Selected-Show Wrong Size";
			ISelectItemDialog.vItems[3] = vErrorMessageDisplayMask & 0x0400;
			ISelectItemDialog.vItemsYes[3] = "Selected-Show User Actions";
			ISelectItemDialog.vItemsNo[3] = "Not Selected-Show User Actions";
			ISelectItemDialog.vItems[4] = vErrorMessageDisplayMask & 0x0800;
			ISelectItemDialog.vItemsYes[4] = "Selected-Show User Changes";
			ISelectItemDialog.vItemsNo[4] = "Not Selected-Show User Changes";

			if (PasswordOK(cTemporaryInspxPassword,false))
			{
				ISelectItemDialog.vItems[5] = vErrorMessageDisplayMask & 0x0200;
				ISelectItemDialog.vItemsYes[5] = "Selected-Show Detailed Actions";
				ISelectItemDialog.vItemsNo[5] = "Not Selected-Show Detailed Actions";
				ISelectItemDialog.vNumberOfItems++;
			}

			//Pass control to dialog box and display
			int nResponse = ISelectItemDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vErrorMessageDisplayMask = 0x6B;
				if (ISelectItemDialog.vItems[0])
					vErrorMessageDisplayMask = vErrorMessageDisplayMask | 0x0080; //show rejects
				if (ISelectItemDialog.vItems[1])
					vErrorMessageDisplayMask = vErrorMessageDisplayMask | 0x0100; //show Actions
				if (ISelectItemDialog.vItems[2])
					vErrorMessageDisplayMask = vErrorMessageDisplayMask | 0x0004; //show Wrong Size
				if (ISelectItemDialog.vItems[3])
					vErrorMessageDisplayMask = vErrorMessageDisplayMask | 0x0400; //show User Action
				if (ISelectItemDialog.vItems[4])
					vErrorMessageDisplayMask = vErrorMessageDisplayMask | 0x0800; //show User Changes
				if (ISelectItemDialog.vItems[5])
					vErrorMessageDisplayMask = vErrorMessageDisplayMask | 0x0200; //show Detailed Actions
				ShowErrorLog(false); //do not refresh
				m_SubFunction3Button.Invalidate(false);
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
	if (vWindowType == cHelpWindow)
	{
		// Hints button pressed
		m_HelpTextControl.LineScroll(-1000);
		vHelpType = cHints;
		ShowHelp();
	}
	else
	if (vWindowType == cProductInformationWindow)
	{
		m_Function3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());

		SetDlgItemText(IDC_SubFunction1Button, _T("Show Production Report"));
		vWindowType = cProductInformationWindow;
		vProductPointer = NULL;
		m_HelpTextControl.LineScroll(-1000);
		DisplayProductInformation();
		UpdateButtons();
	}
}

void CHelpDialog::OnSubFunction4Button() 
{
	vFindTextShow = cFindTextNoHighlight;
	//vGlobalMouseMoveCounter = 0;
	if (vWindowType == cMainHelpWindow)
	{
		vWindowType = cRemoteKeyLabelWindow;
		m_HelpTextControl.ShowWindow(SW_SHOW);
		UpdateButtons();
	}
	else
	if (vWindowType == cErrorLogWindow)
	{
		// Overview button pressed
		//vLocalConfigurationData->vErrorLogEjects = !vLocalConfigurationData->vErrorLogEjects;
		//UpdateButtons();
	}
	else
	if (vWindowType == cHelpWindow)
	{
		// Overview button pressed
		vHelpType = cOverview;
		m_HelpTextControl.LineScroll(-1000);
		ShowHelp();
	}
	else
	if (vWindowType == cProductInformationWindow)
	{
		SetDlgItemText(IDC_SubFunction1Button, _T("Show Production Report"));
		vWindowType = cProductInformationWindow;
   //Create dialog box
    CEasySelectProductDialog ISelectProductDialog;  
		//Set dialog box data like product collection pointer
		ISelectProductDialog.vLocalProductCollection = vLocalProductCollection;
		ISelectProductDialog.vLocalSystemData = vLocalSystemData;
		ISelectProductDialog.SelectWindowForStyle = SelectProductForDisplay;

		//Pass control to dialog box and display
    int nResponse = ISelectProductDialog.DoModal();

		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
    if (nResponse == IDOK)
		{
		  //  closed with OK so update current product and display
			//Set current product to user's selection
			m_HelpTextControl.LineScroll(-1000);
			vProductPointer = vMainWindowPointer->GetProductByNumber(ISelectProductDialog.vProductNumberSelected);
			DisplayProductInformation();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
		UpdateButtons();
	}
}

void CHelpDialog::OnSubFunction5Button() 
{
	vLastFindPosition = 0;		// if significant change to display window want to restart find location

	//Page up button
	//vGlobalMouseMoveCounter = 0;
	if (vWindowType == cMainHelpWindow)
	{
		//find setting location
		CFindParameterHelpDialog IFindParameterHelpDialog;
		IFindParameterHelpDialog.vLocalConfigurationData = vLocalConfigurationData;
		IFindParameterHelpDialog.vLocalSystemData = vLocalSystemData;
		int nResponse = IFindParameterHelpDialog.DoModal();
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
	else
	if (vWindowType != cRemoteKeyLabelWindow)
	{
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 5",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState > 4) && (vMouseState < 9)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 5;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		ScrollOnePageUp();
	}
}

void CHelpDialog::OnSubFunction6Button() 
{
	vLastFindPosition = 0;		// if significant change to display window want to restart find location

	//vGlobalMouseMoveCounter = 0;
	if (vWindowType == cMainHelpWindow)
	{
	}
	else
	if (vWindowType != cRemoteKeyLabelWindow)
	{
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 6",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState > 4) && (vMouseState < 9)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 6;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		ScrollOneLineUp();
	}
}

void CHelpDialog::OnSubFunction7Button() 
{
	vLastFindPosition = 0;		// if significant change to display window want to restart find location

	//vGlobalMouseMoveCounter = 0;
	if (vWindowType == cMainHelpWindow)
	{
		;
	}
	else
	if (vWindowType != cRemoteKeyLabelWindow)
	{
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 7",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState > 4) && (vMouseState < 9)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 7;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		ScrollOneLineDown();
	}
}

void CHelpDialog::OnSubFunction8Button() 
{
	vLastFindPosition = 0;		// if significant change to display window want to restart find location

	//Page down button
	//vGlobalMouseMoveCounter = 0;
	if (vWindowType == cMainHelpWindow)
	{
		vWindowType = cHelpWindow;
		m_HelpTitle.ShowWindow(SW_SHOW);
		m_HelpTextControl.ShowWindow(SW_SHOW);
		ShowHelp();
		UpdateButtons();
	}
	else
	if (vWindowType != cRemoteKeyLabelWindow)
	{
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 8",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState > 4) && (vMouseState < 9)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 8;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		ScrollOnePageDown();
	}
}

BOOL CHelpDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	//if ((pMsg->message == 15) || (pMsg->message == 275) || (pMsg->message == 280)
	//	 || (pMsg->message == 160) || (pMsg->message == 512))// on paint
	//{
	//}
	//else
	if (pMsg->message == cUpdateTitleBarMessage)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
		return true;  //true indicates it has been handled, so do not process
	}
	else
	if ((pMsg->message == 513) || (pMsg->message == 514) || (pMsg->message == 515)) //mouse click
	{
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
			//ReportErrorMessage("Help Dialog: Left ButtonUp",cAction, 32000);
			if (vMouseState < 9)
			{
				if (vMouseState > 0)
				{
					KillTimer(vMouseDownRepeatTimerHandle);
					vMouseState = 0;
				}
			}
		}

		//system locks up if click mouse in edit box, so don't let system
		//process mouse clicks in the edit box
		/*
		if ((pMsg->pt.x > 105) && (pMsg->pt.x < 533) && 
			(pMsg->pt.y > 42) && (pMsg->pt.y < 412))
			return true;
			*/
		if (((DWORD)pMsg->pt.x > vDoNotClickLeftXHelp) && ((DWORD)pMsg->pt.x < vDoNotClickRightXHelp) && 
			((DWORD)pMsg->pt.y > vDoNotClickUpperYHelp) && ((DWORD)pMsg->pt.y < vDoNotClickLowerYHelp))
			return true;
	}
	else
	if (pMsg->message == WM_KEYDOWN)
	{
		if (vGlobalPasswordTimeOut)
			vGlobalPasswordTimeOut = 600;
		int TempKey = pMsg->wParam;
		CWnd *TempWindow;
		vRecievedButtonPress = true;
		if (vGlobalShiftKeyDown)
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
		else
		switch (TempKey)
		{
			case 37: //arrow Left
			case 39: //arrow Right
			case 38: //arrow up
			case 40: //arrow down
			case 98:
			case 100:
			case 102:
			case 104:
				if(vWindowType == cTextWindow)
					CDialog::EndDialog(TempKey);
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
				//if (vACPowerMoniterFormat)
				//	CDialog::EndDialog(TempKey);
				//else
					PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
			break;
			case 36:  //Home key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
			case cPageDown: // Help
				//vGlobalMouseMoveCounter = 0;
				if (vACPowerMoniterFormat)
					PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 61, 0);
				else
				if (vWindowType == cTextWindow)
				{
					PostThreadMessage(vGlobalCallingThreadID, cShowHelp, vHelpContext, 0);
				}
				else
				if ((vWindowType == cMainHelpWindow) || (vWindowType == cHelpWindow))
				{  //help for help
					vHelpContext = 27;
					vHelpType = 0;
					vWindowType = cHelpWindow;
					m_HelpTitle.ShowWindow(SW_SHOW);
					m_HelpTextControl.ShowWindow(SW_SHOW);
					ShowHelp();
					UpdateButtons();
				}
				else
				if (vWindowType == cSourceStrengthWindow)
					PostThreadMessage(vGlobalCallingThreadID,	cShowHelp, 50, 0);
				else
				if (vWindowType == cProductionReportWindow)
					PostThreadMessage(vGlobalCallingThreadID,	cShowHelp, 53, 0);
				else
				if (vWindowType == cErrorLogWindow)
					PostThreadMessage(vGlobalCallingThreadID,	cShowHelp, 51, 0);
				else
				{
					if (vProductPointer)  //if pointing to a product, help for product summary
						PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 49, 0);
					else  //help for system summary
						PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 48, 0);
				}
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
			break;
			case cPageUp: // Clear, but do inspx hidden dialog
			{
				if (vGlobalPasswordLevel)
				{
					PasswordOK(cNoPassword, false);
					UpdateButtons();
					if (vWindowType == cErrorLogWindow)
						ShowErrorLog(false); //do refresh

					if (vWindowType == cSourceStrengthWindow)
							ShowSourceStrengthData();
				}
				else
				{
					PasswordOK(cAnyPasswordToEnter, true); //allow any password to be entered
					if (PasswordOK(cTemporaryInspxPassword, false))
					{
						UpdateButtons();
						if (vWindowType == cErrorLogWindow)
							ShowErrorLog(false); //do refresh

						if (vWindowType == cSourceStrengthWindow)
								ShowSourceStrengthData();
					}
				}
				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			}	
			break;	
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;
			/*
			case c2:

				if( vACPowerMoniterFormat && (vWindowType == cTextWindow) )
					CDialog::EndDialog(TempKey);

			if (vHelpContext == 9999)
			if (vWindowType == cTextWindow)
			if (PasswordOK(cTemporaryInspxPassword,false))
			{
				//Set dialog box data titles and number value
				INumericEntryDialog.vEditString = dtoa(vGlobalCurveFitEquationOrder,0);
				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Number of Terms in Equation";
				INumericEntryDialog.m_DialogTitleStaticText2 = "Square has 3, Cubic has 4";
				INumericEntryDialog.m_UnitsString = "Original Value: " + INumericEntryDialog.vEditString;
				INumericEntryDialog.vMaxValue = 10;
				INumericEntryDialog.vMinValue = 3;
				INumericEntryDialog.vIntegerOnly = true;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vGlobalCurveFitEquationOrder = (BYTE)ATOF(INumericEntryDialog.vEditString);
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					Sleep(1); //is the equivelent of a yeild statement;
					CDialog::EndDialog(true);
				}
			}
			break;
			case c3:
			if (vHelpContext == 9999)
			if (vWindowType == cTextWindow)
			if (PasswordOK(cTemporaryInspxPassword,false))
			{
				//Set dialog box data titles and number value
				INumericEntryDialog.vEditString = dtoa(vGlobalCurveFitMaxCall,0);
				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Maximum Number of Times through Main Loop";
				INumericEntryDialog.m_DialogTitleStaticText2 = "Default: 100";
				INumericEntryDialog.m_UnitsString = "Original Value: " + INumericEntryDialog.vEditString;
				INumericEntryDialog.vMaxValue = 32000;
				INumericEntryDialog.vMinValue = 1;
				INumericEntryDialog.vIntegerOnly = true;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vGlobalCurveFitMaxCall = (WORD)ATOF(INumericEntryDialog.vEditString);
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					Sleep(1); //is the equivelent of a yeild statement;
					CDialog::EndDialog(true);
				}
			}
			break;
			case c4:

				if( vACPowerMoniterFormat && (vWindowType == cTextWindow) )
					CDialog::EndDialog(TempKey);

			if (vHelpContext == 9999)
			if (vWindowType == cTextWindow)
			if (PasswordOK(cTemporaryInspxPassword,false))
			{
				//Set dialog box data titles and number value
				INumericEntryDialog.vEditString = dtoa(vGlobalCurveFitEpsilon,9);
				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Epsilon";
				INumericEntryDialog.m_DialogTitleStaticText2 = "Default: .1";
				INumericEntryDialog.m_UnitsString = "Original Value: " + INumericEntryDialog.vEditString;
				INumericEntryDialog.vMaxValue = 32000;
				INumericEntryDialog.vMinValue = 0;
				INumericEntryDialog.vIntegerOnly = false;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vGlobalCurveFitEpsilon = ATOF(INumericEntryDialog.vEditString);
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					Sleep(1); //is the equivelent of a yeild statement;
					CDialog::EndDialog(true);
				}
			}
			break;
			case c5:
			if (vHelpContext == 9999)
			if (vWindowType == cTextWindow)
			if (PasswordOK(cTemporaryInspxPassword,false))
			{
				//Set dialog box data titles and number value
				INumericEntryDialog.vEditString = dtoa(vGlobalCurveFitStepBound,9);
				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Step Bound";
				INumericEntryDialog.m_DialogTitleStaticText2 = "Default: 100";
				INumericEntryDialog.m_UnitsString = "Original Value: " + INumericEntryDialog.vEditString;
				INumericEntryDialog.vMaxValue = 32000;
				INumericEntryDialog.vMinValue = 1;
				INumericEntryDialog.vIntegerOnly = true;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vGlobalCurveFitStepBound = (WORD)ATOF(INumericEntryDialog.vEditString);
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					Sleep(1); //is the equivelent of a yeild statement;
					CDialog::EndDialog(true);
				}
			}
			break;
			case c6:

				if( vACPowerMoniterFormat && (vWindowType == cTextWindow) )
					CDialog::EndDialog(TempKey);

			if (vHelpContext == 9999)
			if (vWindowType == cTextWindow)
			if (PasswordOK(cTemporaryInspxPassword,false))
			{
				//Set dialog box data titles and number value
				INumericEntryDialog.vEditString = dtoa(vGlobalCurveFitFTolerance,9);
				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter F Tolerance";
				INumericEntryDialog.m_DialogTitleStaticText2 = "Default: 2";
				INumericEntryDialog.m_UnitsString = "Original Value: " + INumericEntryDialog.vEditString;
				INumericEntryDialog.vMaxValue = 32000;
				INumericEntryDialog.vMinValue = 0;
				INumericEntryDialog.vIntegerOnly = false;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vGlobalCurveFitFTolerance = ATOF(INumericEntryDialog.vEditString);
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					Sleep(1); //is the equivelent of a yeild statement;
					CDialog::EndDialog(true);
				}
			}
			break;
			case c7:
			if (vHelpContext == 9999)
			if (vWindowType == cTextWindow)
			if (PasswordOK(cTemporaryInspxPassword,false))
			{
				//Set dialog box data titles and number value
				INumericEntryDialog.vEditString = dtoa(vGlobalCurveFitXTolerance,9);
				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter X Tolerance";
				INumericEntryDialog.m_DialogTitleStaticText2 = "Default: 2";
				INumericEntryDialog.m_UnitsString = "Original Value: " + INumericEntryDialog.vEditString;
				INumericEntryDialog.vMaxValue = 32000;
				INumericEntryDialog.vMinValue = 0;
				INumericEntryDialog.vIntegerOnly = false;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vGlobalCurveFitXTolerance = ATOF(INumericEntryDialog.vEditString);
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					Sleep(1); //is the equivelent of a yeild statement;
					CDialog::EndDialog(true);
				}
			}
			break;
			case c8:
				
				if( vACPowerMoniterFormat && (vWindowType == cTextWindow) )
					CDialog::EndDialog(TempKey);

			if (vHelpContext == 9999)
			if (vWindowType == cTextWindow)
			if (PasswordOK(cTemporaryInspxPassword,false))
			{
				//Set dialog box data titles and number value
				INumericEntryDialog.vEditString = dtoa(vGlobalCurveFitGTolerance,9);
				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter G Tolerance";
				INumericEntryDialog.m_DialogTitleStaticText2 = "Default: 2";
				INumericEntryDialog.m_UnitsString = "Original Value: " + INumericEntryDialog.vEditString;
				INumericEntryDialog.vMaxValue = 32000;
				INumericEntryDialog.vMinValue = 0;
				INumericEntryDialog.vIntegerOnly = false;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vGlobalCurveFitGTolerance = ATOF(INumericEntryDialog.vEditString);
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					Sleep(1); //is the equivelent of a yeild statement;
					CDialog::EndDialog(true);
				}
			}
			break;
			*/
		}
		vRecievedButtonPress = false;
		return true;  //true indicates it has been handled, so do not process
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CHelpDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);

	// TODO: Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);  //5 is medium large
		//if ((vGlobalUnderRemoteControl) && (vGlobalEvenSecond))
		//	pDC->SetTextColor(cRed);
		//else
		if (vGlobalShiftKeyDown)
			pDC->SetTextColor(cBlack);
		else
			pDC->SetTextColor(vGlobalDialogTitleColor);
	}

/*
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)
	if (vWindowType == cMainHelpWindow)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vGreenBrush;
  }
*/


	if ((pWnd->GetDlgCtrlID() == IDC_Background) ||
		(pWnd->GetDlgCtrlID() == IDC_HelpText))
	{
		pDC->SetTextColor(cButtonTextColor);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(vGlobalFixedFont);
		return vWhiteBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_HelpTitle)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_HelpTitle, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
			return vBlueBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function1Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vMakeExitButtonGreen)
		{
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function2Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
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
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vWindowType == cErrorLogWindow)
		if ((vErrorMessageDisplayMask & 0x1EF) != 0x1EF)
		{
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
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
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CHelpDialog::ShowHelp() 
{
	m_HelpTextControl.SetSelectionCharFormat(vBlackCharacterFormat);
	if (vWindowType == cHelpWindow)
	{
		CString TempString("Using ScanTrac");
		TempString.LoadString(IDS_HelpTitle0);
		SetDlgItemText(IDC_Function3Button, TempString);

		ShowHelpTitle();
		ShowHelpText();
		UpdateButtons();
//		int vTimerResult = SetTimer(vHideCaretTimerHandle,10,NULL);
	}
}

void CHelpDialog::ShowHelpTitle() 
{
	CString TempStringTitle("Welcome to ScanTrac");
	if ((vHelpContext >= 0) && (vHelpContext <= 99))
			TempStringTitle.LoadString(100 + vHelpContext);
	else
		TempStringTitle.LoadString(IDS_HelpTitle0);

	SetDlgItemText(IDC_HelpTitle,TempStringTitle);
}

void CHelpDialog::ShowHelpText() 
{
	m_HelpTextControl.SetSelectionCharFormat(vBlackCharacterFormat);
	switch (vHelpType)
	{
		case cOverview:
			ShowHelpTextOverview();
		break;
		case cHints:
			ShowHelpTextHints();
		break;
		case cHiddenFunctions:
			ShowHelpTextHiddenFunctions();
		break;
		case cFunctions:
			ShowHelpTextFunctions();
		break;
	}
	NewPage();
}

void CHelpDialog::ShowHelpTextOverview() 
{
	//m_HelpTextControl.SetSelectionCharFormat(vBlackCharacterFormat);
	CString TempStringText("");
	TempStringText.LoadString(IDS_HelpNoOverview);
  //cNumberOfHelpContexts
	if ((vHelpContext >= 0) && (vHelpContext <= 99))
			TempStringText.LoadString(vHelpContext + 200);
	else
		TempStringText.LoadString(IDS_HelpText0);
		/*
	switch (vHelpContext)
	{
		case 33:
			TempStringText.LoadString(IDS_HelpText33);
		break;
		case 34:
			TempStringText.LoadString(IDS_HelpText34);
		break;
		case 35:
			TempStringText.LoadString(IDS_HelpText35);
		break;
		case 36:
			TempStringText.LoadString(IDS_HelpText36);
		break;
		case 37:
			TempStringText.LoadString(IDS_HelpText37);
		break;
		case 38:
			TempStringText.LoadString(IDS_HelpText38);
		break;
		case 39:
			TempStringText.LoadString(IDS_HelpText39);
		break;
		case 40:
			TempStringText.LoadString(IDS_HelpText40);
		break;
		case 41:
			TempStringText.LoadString(IDS_HelpText41);
		break;
		case 42:
			TempStringText.LoadString(IDS_HelpText42);
		break;
		case 43:
			TempStringText.LoadString(IDS_HelpText43);
		break;
		case 44:
			TempStringText.LoadString(IDS_HelpText44);
		break;
		case 45:
			TempStringText.LoadString(IDS_HelpText45);
		break;
		case 46:
			TempStringText.LoadString(IDS_HelpText46);
		break;
		case 47:
			TempStringText.LoadString(IDS_HelpText47);
		break;
		case 48:
			TempStringText.LoadString(IDS_HelpText48);
		break;
		case 49:
			TempStringText.LoadString(IDS_HelpText49);
		break;
		case 50:
			TempStringText.LoadString(IDS_HelpText50);
		break;
		case 51:
			TempStringText.LoadString(IDS_HelpText51);
		break;
		case 52:
			TempStringText.LoadString(IDS_HelpText52);
		break;
		case 53:
			TempStringText.LoadString(IDS_HelpText53);
		break;
		case 54:
			TempStringText.LoadString(IDS_HelpText54);
		break;
		case 55:
			TempStringText.LoadString(IDS_HelpText55);
		break;
		case 56:
			TempStringText.LoadString(IDS_HelpText56);
		break;
		case 57:
			TempStringText.LoadString(IDS_HelpText57);
		break;
		case 58:
			TempStringText.LoadString(IDS_HelpText58);
		break;
		case 59:
			TempStringText.LoadString(IDS_HelpText59);
		break;
		default:
			TempStringText.LoadString(IDS_HelpText0);
	}
	*/
	if (vLocalConfigurationData->vLanguage != cSimplifiedChinese)	
		TempStringText = TempStringText + "\n\n";
	else
	{
		//GB2312-CHARSET
		//SetDefaultCharFormat
		//vBlackCharacterFormat.dwMask = CFM_COLOR | CFM_BOLD | CFM_UNDERLINE;
		//vBlackCharacterFormat.crTextColor = cRed;
		//vBlackCharacterFormat.dwEffects = 0;

		CHARFORMAT2W TestCharacterFormat;
		m_HelpTextControl.GetDefaultCharFormat(TestCharacterFormat);
		TestCharacterFormat.crTextColor = cRed;
		TestCharacterFormat.bCharSet = GB2312_CHARSET;

		//CHARFORMAT cf;
		//COLORREF crNewColor = (COLORREF)0x00FF0000;
		//cf.cbSize = sizeof(CHARFORMAT);
		//cf.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD | CFM_SIZE | CFM_FACE | CFM_CHARSET;
		//cf.dwEffects =(unsigned long) ~( CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
		//cf.crTextColor = crNewColor;
		////wcscpy_s(cf.szFaceName, _T("Fixedsys"));
		//cf.yHeight = 250;		
		//cf.bCharSet = GB2312_CHARSET;
		//if (m_HelpTextControl.SetDefaultCharFormat(cf))

		if (m_HelpTextControl.SetDefaultCharFormat(TestCharacterFormat))
		 ReportErrorMessage("codepage change is successful", cWriteToLog, 0);
	 else
		 ReportErrorMessage("codepage change failed", cWriteToLog, 0);
	}
	//SetDlgItemText(IDC_HelpText,TempStringText);
	//m_HelpTextControl.StreamIn(SF_TEXT, TempStringText); 
	SetDlgItemText(IDC_HelpText,TempStringText);
}

void CHelpDialog::ShowHelpTextHints() 
{
	//m_HelpTextControl.SetSelectionCharFormat(vBlackCharacterFormat);
	CString TempStringText("");
	TempStringText.LoadString(IDS_HelpNoHints);
	switch (vHelpContext)
	{
		case 51:
			TempStringText.LoadString(IDS_HelpTextHints51);
		break;
		case 1:
			TempStringText.LoadString(IDS_HelpTextHints1);
		break;
		case 2:
			//TempStringText.LoadString(IDS_HelpTextHints2);
		break;
		case 3:
			//TempStringText.LoadString(IDS_HelpTextHints3);
		break;
		case 4:
			//TempStringText.LoadString(IDS_HelpTextHints4);
		break;
		case 5:
			//TempStringText.LoadString(IDS_HelpTextHints5);
		break;
		case 6:
			//TempStringText.LoadString(IDS_HelpTextHints6);
		break;
		case 7:
			//TempStringText.LoadString(IDS_HelpTextHints7);
		break;
		case 8:
			TempStringText.LoadString(IDS_HelpTextHints8);
		break;
		case 9:
			//TempStringText.LoadString(IDS_HelpTextHints9);
		break;
		case 10:
			//TempStringText.LoadString(IDS_HelpTextHints10);
		break;
		case 11:
			//TempStringText.LoadString(IDS_HelpTextHints11);
		break;
		case 12:
			//TempStringText.LoadString(IDS_HelpTextHints12);
		break;
		case 13:
			//TempStringText.LoadString(IDS_HelpTextHints13);
		break;
		case 14:
			TempStringText.LoadString(IDS_HelpTextHints14);
		break;
		case 15:
			TempStringText.LoadString(IDS_HelpTextHints15);
		break;
		case 16:
			TempStringText.LoadString(IDS_HelpTextHints16);
		break;
		case 17:
			//TempStringText.LoadString(IDS_HelpTextHints17);
		break;
		case 18:
			//TempStringText.LoadString(IDS_HelpTextHints18);
		break;
		case 19:
			//TempStringText.LoadString(IDS_HelpTextHints19);
		break;
		case 20:
			//TempStringText.LoadString(IDS_HelpTextHints20);
		break;
		case 21:
			//TempStringText.LoadString(IDS_HelpTextHints21);
		break;
		case 22:
			//TempStringText.LoadString(IDS_HelpTextHints22);
		break;
		case 23:
			TempStringText.LoadString(IDS_HelpTextHints23);
		break;
		case 24:
			//TempStringText.LoadString(IDS_HelpTextHints24);
		break;
		case 25:
			TempStringText.LoadString(IDS_HelpTextHints25);
		break;
		case 26:
			//TempStringText.LoadString(IDS_HelpTextHints26);
		break;
		case 27:
			//TempStringText.LoadString(IDS_HelpTextHints27);
		break;
		case 28:
			TempStringText.LoadString(IDS_HelpTextHints28);
		break;
		case 33:
			TempStringText.LoadString(IDS_HelpTextHints33);
		break;
		case 34:
			TempStringText.LoadString(IDS_HelpTextHints34);
		break;
		//default:
			//do nothing as strings are initialized
	}
	if (vLocalConfigurationData->vLanguage != cSimplifiedChinese)	
		TempStringText = TempStringText + "\n\n";
	SetDlgItemText(IDC_HelpText,TempStringText);
	//m_HelpTextControl.SetWindowText(TempStringText);
}

void CHelpDialog::ShowHelpTextHiddenFunctions() 
{
	//m_HelpTextControl.SetSelectionCharFormat(vBlackCharacterFormat);
	CString TempStringText("");
	TempStringText.LoadString(IDS_HelpNoHiddenFunctions);
	switch (vHelpContext)
	{
		case 1:
			TempStringText.LoadString(IDS_HelpTextHiddenFunctions1);
		break;
		case 2:
			//TempStringText.LoadString(IDS_HelpTextHiddenFunctions2);
		break;
		case 3:
			//TempStringText.LoadString(IDS_HelpTextHiddenFunctions3);
		break;
		case 4:
			TempStringText.LoadString(IDS_HelpTextHiddenFunctions4);
		break;
		case 5:
			//TempStringText.LoadString(IDS_HelpTextHiddenFunctions5);
		break;
		case 6:
			//TempStringText.LoadString(IDS_HelpTextHiddenFunctions6);
		break;
		case 7:
			//TempStringText.LoadString(IDS_HelpTextHiddenFunctions7);
		break;
		case 8:
			//TempStringText.LoadString(IDS_HelpTextHiddenFunctions8);
		break;
		case 9:
			//TempStringText.LoadString(IDS_HelpTextHiddenFunctions9);
		break;
		case 10:
			//TempStringText.LoadString(IDS_HelpTextHiddenFunctions10);
		break;
		case 11:
			//TempStringText.LoadString(IDS_HelpTextHiddenFunctions11);
		break;
		case 12:
			TempStringText.LoadString(IDS_HelpTextHiddenFunctions12);
		break;
		case 13:
			//TempStringText.LoadString(IDS_HelpTextHiddenFunctions13);
		break;
		case 14:
			//TempStringText.LoadString(IDS_HelpTextHiddenFunctions14);
		break;
		case 15:
			//TempStringText.LoadString(IDS_HelpTextHiddenFunctions15);
		break;
		case 16:
			TempStringText.LoadString(IDS_HelpTextHiddenFunctions16);
		break;
		case 17:
			//TempStringText.LoadString(IDS_HelpTextHiddenFunctions17);
		break;
		case 18:
			//TempStringText.LoadString(IDS_HelpTextHiddenFunctions18);
		break;
		case 19:
			//TempStringText.LoadString(IDS_HelpTextHiddenFunctions19);
		break;
		case 20:
			//TempStringText.LoadString(IDS_HelpTextHiddenFunctions20);
		break;
		case 21:
			//TempStringText.LoadString(IDS_HelpTextHiddenFunctions21);
		break;
		case 22:
			//TempStringText.LoadString(IDS_HelpTextHiddenFunctions22);
		break;
		case 23:
			//TempStringText.LoadString(IDS_HelpTextHiddenFunctions23);
		break;
		case 24:
			//TempStringText.LoadString(IDS_HelpTextHiddenFunctions24);
		break;
		case 25:
			///TempStringText.LoadString(IDS_HelpTextHiddenFunctions25);
		break;
		case 26:
			//TempStringText.LoadString(IDS_HelpTextHiddenFunctions26);
		break;
		case 27:
			//TempStringText.LoadString(IDS_HelpTextHiddenFunctions27);
		break;
		case 33:
			TempStringText.LoadString(IDS_HelpTextHiddenFunctions33);
		break;
		case 36:
			TempStringText.LoadString(IDS_HelpTextHiddenFunctions36);
		break;
		case 54:
			TempStringText.LoadString(IDS_HelpTextHiddenFunctions54);
		break;
		case 61:
			if (PasswordOK(cTemporaryInspxPassword,false))
				TempStringText.LoadString(IDS_HelpTextHiddenFunctions61);
		break;
		//default:
			//do nothing as strings are initialized
	}
	if (vLocalConfigurationData->vLanguage != cSimplifiedChinese)	
		TempStringText = TempStringText + "\n\n";
	SetDlgItemText(IDC_HelpText,TempStringText);
	//m_HelpTextControl.SetWindowText(TempStringText);
}

void CHelpDialog::ShowHelpTextFunctions() 
{
	//m_HelpTextControl.SetSelectionCharFormat(vBlackCharacterFormat);
	CString TempStringText("");
	//TempStringText.LoadString(IDS_HelpNoFunctions);
	TempStringText.LoadString(IDS_HelpNoOverview);
	switch (vHelpContext)
	{
		case 1:
			TempStringText.LoadString(IDS_HelpTextFunctions1);
		break;
		case 2:
			//TempStringText.LoadString(IDS_HelpTextFunctions2);
		break;
		case 3:
			//TempStringText.LoadString(IDS_HelpTextFunctions3);
		break;
		case 4:
			///TempStringText.LoadString(IDS_HelpTextFunctions4);
		break;
		case 5:
			//TempStringText.LoadString(IDS_HelpTextFunctions5);
		break;
		case 6:
			//TempStringText.LoadString(IDS_HelpTextFunctions6);
		break;
		case 7:
			//TempStringText.LoadString(IDS_HelpTextFunctions7);
		break;
		case 8:
			//TempStringText.LoadString(IDS_HelpTextFunctions8);
		break;
		case 9:
			//TempStringText.LoadString(IDS_HelpTextFunctions9);
		break;
		case 10:
			//TempStringText.LoadString(IDS_HelpTextFunctions10);
		break;
		case 11:
			//TempStringText.LoadString(IDS_HelpTextFunctions11);
		break;
		case 12:
			TempStringText.LoadString(IDS_HelpTextFunctions12);
		break;
		case 13:
			//TempStringText.LoadString(IDS_HelpTextFunctions13);
		break;
		case 14:
			//TempStringText.LoadString(IDS_HelpTextFunctions14);
		break;
		case 15:
			///TempStringText.LoadString(IDS_HelpTextFunctions15);
		break;
		case 16:
			TempStringText.LoadString(IDS_HelpTextFunctions16);
		break;
		case 17:
			TempStringText.LoadString(IDS_HelpTextFunctions17);
		break;
		case 18:
			TempStringText.LoadString(IDS_HelpTextFunctions18);
		break;
		case 19:
			//TempStringText.LoadString(IDS_HelpTextFunctions19);
		break;
		case 20:
			//TempStringText.LoadString(IDS_HelpTextFunctions20);
		break;
		case 21:
			//TempStringText.LoadString(IDS_HelpTextFunctions21);
		break;
		case 22:
			//TempStringText.LoadString(IDS_HelpTextFunctions22);
		break;
		case 23:
			//TempStringText.LoadString(IDS_HelpTextFunctions23);
		break;
		case 24:
			//TempStringText.LoadString(IDS_HelpTextFunctions24);
		break;
		case 25:
			//TempStringText.LoadString(IDS_HelpTextFunctions25);
		break;
		case 26:
			//TempStringText.LoadString(IDS_HelpTextFunctions26);
		break;
		case 27:
			//TempStringText.LoadString(IDS_HelpTextFunctions27);
		break;
		case 51:
			TempStringText.LoadString(IDS_HelpTextFunctions51);
		break;
		//default:
			//do nothing as strings are initialized
	}
	if (vLocalConfigurationData->vLanguage != cSimplifiedChinese)	
		TempStringText = TempStringText + "\n\n";
	SetDlgItemText(IDC_HelpText, TempStringText);
	//m_HelpTextControl.SetWindowText(TempStringText);
}

void CHelpDialog::NewPage()
{
	int TempTotalLines = m_HelpTextControl.GetLineCount();
	int TempCurrentLine = m_HelpTextControl.GetFirstVisibleLine();
	if (TempCurrentLine == 0)
	{  //show up arrows if first time moved down
		m_SubFunction5Button.ShowNavigationArrow(NULL);
		m_SubFunction6Button.ShowNavigationArrow(NULL);
		m_SubFunction5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
		m_SubFunction6Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
	}
	if (TempCurrentLine + 1 + vLinesOnScreen >= TempTotalLines)
	{  
		m_SubFunction7Button.ShowNavigationArrow(NULL);
		m_SubFunction8Button.ShowNavigationArrow(NULL);
		m_SubFunction7Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
		m_SubFunction8Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
	}
	else
	{  
		m_SubFunction7Button.ShowNavigationArrow(cDownArrowSmall);
		m_SubFunction8Button.ShowNavigationArrow(cDownArrowBig);
		m_SubFunction7Button.ShowWindow(SW_SHOW);
		m_SubFunction8Button.ShowWindow(SW_SHOW);
	}
}

void CHelpDialog::DisplayProductInformation() 
{
	m_SubFunction1Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());

	m_HelpTextControl.SetSelectionCharFormat(vBlackCharacterFormat);
	CString TempDividerString("\n========================================================\n\n");

	CString TempStep0 = "Step 0";
	CString TempStep1 = "Step 1";
	CString TempStep2 = "Step 2";
	CString TempStep3 = "Step 3";
	CString TempStep3a = "Step 3a";
	CString TempStep3b = "Step 3b";
	CString TempStep4 = "Step 4";
	CString TempStep5 = "Step 5";
	CString TempStep6 = "Step 6";
	CString TempStep7 = "Step 7";
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		TempStep0 = "Background";
		TempStep1 = "Reference";
		TempStep2 = "Average";
		TempStep3 = "Structure";
		TempStep3a = "Contaminant Structure";
		TempStep3b = "Void Structure";
		TempStep4 = "Histogram";
		TempStep5 = "Enhance";
		TempStep6 = "Dilation";
		TempStep7 = "Image Factors";
	}

	CreateColorDataStructure();

	m_HelpTextControl.LineScroll(-9999);
	LONG TempTestModesLength = 0;
	if (vLocalConfigurationData)
	if (vProductPointer)
	{
		this->SetWindowText(_T("Product Summary"));
		vDialogTitleString = "Product Setup Summary";
		SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
		CString *TempProductName = vProductPointer->GetProductName();
		vTextString = "Product: " + *TempProductName;
		SetDlgItemText(IDC_HelpTitle,vTextString);
		
		char TempChars[20] = "";
		CString TempString("");
		TempString = "";

		vTextString = "Date product setup: " + vProductPointer->vHistoryTookAverageDate;

		if (vProductPointer->vNeedToRelearn)
			vTextString = vTextString + "\n\n++This product needs to be Re-Learned";

		if (vLocalConfigurationData->vUseXScanDetectors)
		if (vProductPointer->vUsingXScanPoint4mmDetectors)
			vTextString = vTextString + "\n++Product in X-Scan 0.4mm Detector Mode";
		else
			vTextString = vTextString + "\nProduct in X-Scan 0.8mm Detector Mode";

		//vTextString = vTextString + "\nNotes: " + vProductPointer->vNotes;

		// dead code int TempBeginOfRedPosition = vTextString.GetLength();
		// dead code int TempEndOfRedPosition = 0;

		if (vProductPointer->vMultiLaneNumberOfLanes)
		{
			vTextString = vTextString + "\n--------------------------------------------------\nProduct in Multi Lane Mode:  " + dtoa(vProductPointer->vMultiLaneNumberOfLanes, 0) + " Lanes";
			vTextString = vTextString + "\n    Lines of white space before container:  " + dtoa(vProductPointer->vMultiLaneNumberOfLinesBeforeContainer, 0);
			if (vProductPointer->vMultiLaneEjectAdjacentLanes)
				vTextString = vTextString + "\n    Eject lane contaminant found in, plus lanes next to it";
			else
				vTextString = vTextString + "\n    Eject only lane contaminant found in";
			
			if (vProductPointer == vGlobalCurrentProduct)
			{
				if (vLocalSystemData->vITIPCDig)
					vTextString = vTextString + "\n    Lane Width (Image Height): " + dtoa(vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, 0) + " pixels";

				vTextString = vTextString + "\n\tLane    Start Pixel    \tLane Width in Pixels     \tEnd Pixel";
				WORD TempEndPosition = 0;
				for (BYTE TempLaneLoop = 0; TempLaneLoop < vProductPointer->vMultiLaneNumberOfLanes; TempLaneLoop++)
				{
					CString TempGapFlag = "";

					if (TempEndPosition)
					if (TempEndPosition < vProductPointer->vMultiLaneStartPixel[TempLaneLoop])
						TempGapFlag = "*";

					vTextString = vTextString + "\n\t" + TempGapFlag + Pad(dtoa(TempLaneLoop + 1, 0), 5, 0) + "\t\t\t" + Pad(dtoa(vProductPointer->vMultiLaneStartPixel[TempLaneLoop], 0), 5, 0) + "\t\t\t\t" + 
						Pad(dtoa(vProductPointer->vMultiLaneWidthInPixels[TempLaneLoop], 0), 5, 0) + "\t\t\t\t" + 
						Pad(dtoa((vProductPointer->vMultiLaneStartPixel[TempLaneLoop] + vProductPointer->vMultiLaneWidthInPixels[TempLaneLoop]), 0), 5, 0);

					TempEndPosition = vProductPointer->vMultiLaneStartPixel[TempLaneLoop] + vProductPointer->vMultiLaneWidthInPixels[TempLaneLoop];
				}

				//int TempUnusedDetectorPixels = vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors - 
				//	(vProductPointer->vMultiLaneStartPixel[vProductPointer->vMultiLaneNumberOfLanes - 1] + vProductPointer->vMultiLaneWidthInPixels[vProductPointer->vMultiLaneNumberOfLanes - 1]);

				//if (TempUnusedDetectorPixels > 0)
				//	vTextString = vTextString + "\n*\tDetector Pixels Not Used: " + dtoa(TempUnusedDetectorPixels, 0);
				//if (TempUnusedDetectorPixels < 0)
				//	vTextString = vTextString + "\n*\tLanes wider than detector pixels by: " + dtoa(TempUnusedDetectorPixels, 0) + " pixels";
			}
			vTextString = vTextString + "\n--------------------------------------------------";
		}

		double TempWhiteSpaceOnOneSideOfImage = (vProductPointer->vProductImageWidth / 1.6) * 0.3;
		if (TempWhiteSpaceOnOneSideOfImage > 4)
			TempWhiteSpaceOnOneSideOfImage = 4;
		
		double TempTriggerToBeamOffset = 0;

		if (vLocalConfigurationData)
		if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam > 1)
			TempTriggerToBeamOffset = -(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - vProductPointer->vProductBodyTriggerToImageBeltPositionOffset - TempWhiteSpaceOnOneSideOfImage);

		if (vLocalConfigurationData)
		if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam)
		if ((TempTriggerToBeamOffset < -1) || (TempTriggerToBeamOffset > 1))
		if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac) || (vLocalConfigurationData->vScanTracType == cPiccoloScanTrac)) //not a pipeline
		{
			vTextString = vTextString + "\n\n*Trigger to X-Ray Beam Offset unusually large: " + dtoa(TempTriggerToBeamOffset,2);
			// dead code TempEndOfRedPosition = vTextString.GetLength();
		}

		TempString = dtoa(vProductPointer->vXRaySourceVoltage * 6,2);
		vTextString = vTextString + "\n\nX-Ray Source Voltage: " + TempString + " kV";

		TempString = dtoa(vProductPointer->vXRaySourceCurrent,2);
		vTextString = vTextString + "\nX-Ray Source Current: " + TempString + " mA";

		//convert X-Ray Integration Time code from old style to new style if needed.
		if (vProductPointer->vXRayIntegrationTimeByte != 0xFF) //0xFF indicates it is not used now, use the word value
		{//convert product from old byte data type, to new word data type
			if (vProductPointer->vXRayIntegrationTimeByte < 98)
				vProductPointer->vXRayIntegrationTime = ((WORD)vProductPointer->vXRayIntegrationTimeByte + 3) * 250;  //convert old style code to new style code
			else
				vProductPointer->vXRayIntegrationTime = 750; //is .3mSec

			//vProductPointer->vXRayIntegrationTimeAtEdge = vProductPointer->vXRayIntegrationTime;
			vProductPointer->vXRayIntegrationTimeByte = 0xFF;  //don't use byte value anymore
		}
		vTextString = vTextString + "\nX-Ray Exposure Time: " + dtoa((vProductPointer->vXRayIntegrationTime * .0004),3) + " mSec";

		WORD TempMaximumEncoderRate = vMainWindowPointer->CalculateMaximumEncoderRate(vProductPointer->vXRayIntegrationTime);
		vTextString = vTextString + "\nMaximum Encoder Rate: " + dtoa(TempMaximumEncoderRate,0) + " Hertz";

		if (vProductPointer->vOverScanMultiplier > 1)
			TempMaximumEncoderRate = TempMaximumEncoderRate / vProductPointer->vOverScanMultiplier;

		double TempMaximumConveyorRate = 0;
		if (vLocalConfigurationData)
			TempMaximumConveyorRate = (TempMaximumEncoderRate / vGlobalPixelsPerUnit) * 60;  //get units per minute

		CString TempText("");
		TempText.LoadString(IDS_MaximumConveyorSpeed);
		CString TempText1("");
		//if (vLocalConfigurationData->vUseMetric)
		//	TempText1.LoadString(IDS_MetersPerMinute);
		//else
			TempText1.LoadString(IDS_FeetPerMinute);
		TempString = "\n" + TempText + ": ";
		//if (vLocalConfigurationData->vUseMetric)
		//	TempMaximumConveyorRate = TempMaximumConveyorRate / 1000;  //convert mm to meters
		//else
			TempMaximumConveyorRate = TempMaximumConveyorRate / 12;  //convert inches to feet

		TempString = TempString + dtoa(TempMaximumConveyorRate,1) + " " + TempText1;
		vTextString = vTextString + TempString;
	
		if (vProductPointer->vBulkProductMode)
			vTextString = vTextString + "\n\nTHIS PRODUCT IS CONFIGURED IN BULK MODE";

		TempString = " ";
		if (vProductPointer->vBodyTrigger == 0)
			TempString = TempString + "Not Configured";
		else
		if ((vLocalConfigurationData) && (vLocalConfigurationData->vBodyTrigger[vProductPointer->vBodyTrigger - 1].vEnabled == 1))
			TempString = TempString + vLocalConfigurationData->vBodyTrigger[vProductPointer->vBodyTrigger - 1].vName;
		else
			TempString = TempString + "Not Enabled";
		vTextString = vTextString + "\n\nContainer Trigger: " + TempString;

		TempString = dtoa(vProductPointer->vProductBodyTriggerToImageBeltPositionOffset,2);
		if (vLocalConfigurationData)
			vTextString = vTextString + "\nContainer Trigger to Detector Distance: " + TempString + " " + vLocalConfigurationData->vUnitsString;

		if (vLocalConfigurationData)
		if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam)
			vTextString = vTextString + "\nTrigger to X-Ray Beam offset: " + dtoa(TempTriggerToBeamOffset,2);

		TempString = dtoa(vProductPointer->vBackupBodyTrigger, 0)  + ": ";

		if (vProductPointer->vBackupBodyTrigger == 0)
			TempString = TempString + "Not Configured";
		else
		if ((vLocalConfigurationData) && (vLocalConfigurationData->vBodyTrigger[vProductPointer->vBackupBodyTrigger - 1].vEnabled == 1))
		{
			TempString = TempString + vLocalConfigurationData->vBodyTrigger[vProductPointer->vBackupBodyTrigger - 1].vName;
			vTextString = vTextString + "\nBackup Container Trigger: " + TempString;
		}
		else
			TempString = TempString + "Not Enabled";

		double TempProductPixelsPerUnitInHeight = 64 / vLocalConfigurationData->vDetectorLength;
		if (vProductPointer->vUsingXScanPoint4mmDetectors)
			TempProductPixelsPerUnitInHeight = 128 / vLocalConfigurationData->vDetectorLength;

		if (vLocalConfigurationData)
		if (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)
			vTextString = vTextString + "\n\nRight: " + dtoa(vProductPointer->vProductImageHeightTop,2) + " " + vLocalConfigurationData->vUnitsString + ", pixels " + dtoa(vProductPointer->vProductImageHeightTop * TempProductPixelsPerUnitInHeight, 0);
		else
			vTextString = vTextString + "\n\nTop: " + dtoa(vProductPointer->vProductImageHeightTop,2) + " " + vLocalConfigurationData->vUnitsString + ", pixels " + dtoa(vProductPointer->vProductImageHeightTop * TempProductPixelsPerUnitInHeight, 0);

		if (vLocalConfigurationData)
		if (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)
			vTextString = vTextString + "\nLeft: " + dtoa(vProductPointer->vProductImageHeightBottom,2) + " " + vLocalConfigurationData->vUnitsString +	", pixels " + 
				dtoa((WORD)(vProductPointer->vProductImageHeightBottom * TempProductPixelsPerUnitInHeight) ,0);
		else
			vTextString = vTextString + "\nBottom: " + dtoa(vProductPointer->vProductImageHeightBottom,2) + " " + vLocalConfigurationData->vUnitsString +	", pixels " + 
				dtoa((WORD)(vProductPointer->vProductImageHeightBottom * TempProductPixelsPerUnitInHeight), 0);

		if (vLocalConfigurationData)
		if (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)
			vTextString = vTextString + "\nHeight: " + dtoa(vProductPointer->vProductImageWidth,2) + " Inches, " + dtoa(vProductPointer->vImageWidthPosition, 0) + " lines";
		else
			vTextString = vTextString + "\nWidth: " + dtoa(vProductPointer->vProductImageWidth,2) + " Inches, " + dtoa(vProductPointer->vImageWidthPosition, 0) + " lines";

		vTextString = vTextString + "\nContainer Trigger Lockout: " + dtoa(vProductPointer->vProductLockOutWidth,2) + " Inches, pixels " + dtoa(vProductPointer->vLockoutPosition, 0);

		if (vProductPointer->vHaveManuallySetRetriggerLockout)
			vTextString = vTextString + " Manually Entered";

		if (vLocalConfigurationData)
		if (((vLocalConfigurationData->vScanTracType != cBriosoScanTrac) && (vLocalConfigurationData->vScanTracType != cFermataScanTrac) &&
			(vLocalConfigurationData->vScanTracType != cAtempoScanTrac) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cAllegroScanTrac) &&
			(vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac)) || (vProductPointer->vMultiLaneNumberOfLanes))
		{
			TempString = dtoa(vProductPointer->vEdgeLocationBottom,2);
			vTextString = vTextString + "\nBottom of area to look for edges: " + TempString + " " + vLocalConfigurationData->vUnitsString;

			TempString = dtoa(vProductPointer->vEdgeLocationHeight,2);
			vTextString = vTextString + "\nHeight of area to look for edges: " + TempString + " " + vLocalConfigurationData->vUnitsString;
		}

		TempString = dtoa(vProductPointer->vSetupHistoryAverageDensity,1);
		vTextString = vTextString + "\n\n" + " Average Density: " + TempString;

		if (!vProductPointer->vSetupHistoryStructureCount)
			vTextString = vTextString + "\n\nLearn not done";
		else
		if ((!vProductPointer->vSetupHistoryAverageCount) || (vProductPointer->vSetupHistoryAverageCount == vProductPointer->vSetupHistoryStructureCount))
			vTextString = vTextString + "\n\nSetup with Quick Learn";
		else
			vTextString = vTextString + "\n\nSetup with Standard Learn";

		if (vLocalConfigurationData)
		if ((vLocalConfigurationData->vRemoveBackgroundFromImage) && (vProductPointer->ProductHasBackgroundFile(vGlobalCurrentDirectory)))
			vTextString = vTextString + "\n" + TempStep0;
		else
			vTextString = vTextString + "\n" + TempStep0 + " disabled";

		vTextString = vTextString + "\n" + TempStep2 + " Count: " + dtoa(vProductPointer->vSetupHistoryAverageCount, 0);
		vTextString = vTextString + "\n" + TempStep3 + " Count: " + dtoa(vProductPointer->vSetupHistoryStructureCount, 0);

		if (PasswordOK(cTemporaryInspxPassword,false))
		{		
			vTextString = vTextString + "\n        Structure ROI Average: " + dtoa(vProductPointer->vStructureROIAverage,2) + ", SD:" + dtoa(vProductPointer->vStructureROIStandardDievation,2);
		}

		vTextString = vTextString + "\n        " + dtoa(vProductPointer->vAddToLearnCount,0) + " Rejects have been Added To Learn"; 

		if (vLocalConfigurationData)
		if (PasswordOK(cTemporaryInspxPassword,false))
		if (vLocalConfigurationData->vAutoImproveEnabled)
			vTextString = vTextString + "\n        Auto Improve State: " + dtoa(vGlobalLearnState,0);
		else
			vTextString = vTextString + "\n        Auto Improve Disabled";

		if (vLocalConfigurationData)
		if (vProductPointer->vSetupHistoryStructureCount > 1)
		{
			double TempLearnQuality = vLocalSystemData->vITIPCDig->CalculateStructureDensityInDriftDensityROI(vProductPointer);
			if ((vLocalConfigurationData->vLearnQualityPoorLimit) && (TempLearnQuality > vLocalConfigurationData->vLearnQualityPoorLimit))
				vTextString = vTextString + "\n*";
			else
			if ((vLocalConfigurationData->vLearnQualityMarginalLimit) && (TempLearnQuality > vLocalConfigurationData->vLearnQualityMarginalLimit))
				vTextString = vTextString + "\n+";
			else
				vTextString = vTextString + "\n";
				vTextString = vTextString + "        Learn Quality Middle of " + vLocalSystemData->vContainerString + ": " + dtoa(TempLearnQuality,1); 

			TempLearnQuality = vLocalSystemData->vITIPCDig->CalculateStructureDensityInProduct(vProductPointer);
			if ((vLocalConfigurationData->vLearnQualityPoorLimit) && (TempLearnQuality > vLocalConfigurationData->vLearnQualityPoorLimit + 20))
				vTextString = vTextString + "\n*";
			else
			if ((vLocalConfigurationData->vLearnQualityMarginalLimit) && (TempLearnQuality > vLocalConfigurationData->vLearnQualityMarginalLimit + 20))
				vTextString = vTextString + "\n+";
			else
				vTextString = vTextString + "\n";

			vTextString = vTextString + "        Learn Quality Whole Container: " + dtoa(TempLearnQuality,1); 

			vTextString = vTextString + "\n" + TempStep4 + " Range: " + dtoa(vProductPointer->vHistogramLowerThreshold,0) + " - " + dtoa(vProductPointer->vHistogramThreshold,0) + " D: " + dtoa(vProductPointer->vHistogramDilateTimes,0);

			vTextString = vTextString + "\nContainer Trigger Width Standard Deviation: " + dtoa(vProductPointer->vSetupHistoryContainerTriggerStandardDeviation,2);

			vTextString = vTextString + "\n\n\tLearn Quality 0-15 very good for middle";
			vTextString = vTextString + "\n\tLearn Quality 15-25 good";
			vTextString = vTextString + "\n\tLearn Quality 25-35 OK";
			vTextString = vTextString + "\n\tLearn Quality 35-45 Marginal";
			vTextString = vTextString + "\n\tLearn Quality 45-up bad, re-learn, check referencing";
			vTextString = vTextString + "\n\tWhole Learn Quality 10 higher than middle on cans OK";
			vTextString = vTextString + "\n\tWhole Learn Quality 20 higher than middle on Jars OK";
			vTextString = vTextString + "\n        " + dtoa(vProductPointer->vAddToLearnCount,0) + " Rejects have been Added To Learn"; 
		}

		if (vLocalConfigurationData)
		if ((vLocalConfigurationData->vScanTracType != cBriosoScanTrac) && (vLocalConfigurationData->vScanTracType != cFermataScanTrac) &&
			(vLocalConfigurationData->vScanTracType != cAtempoScanTrac) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cAllegroScanTrac) &&
			(vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))
		{
			vTextString = vTextString + "\n\nReference Width: " + dtoa((vProductPointer->vReferenceWidth) / vGlobalPixelsPerUnit,2) + " Inches, " + dtoa(vProductPointer->vReferenceRight - vProductPointer->vReferenceLeft, 0) + " Pixels";

			vTextString = vTextString + "\nLeft Reference: " + dtoa(vProductPointer->vReferenceLeft / vGlobalPixelsPerUnit,2) + " " + vLocalConfigurationData->vUnitsString + ", " + dtoa(vProductPointer->vReferenceLeft, 0) + " Pixels";

			vTextString = vTextString + "\nRight Reference: " + dtoa(vProductPointer->vReferenceRight / vGlobalPixelsPerUnit,2) + ", " + dtoa(vProductPointer->vReferenceRight, 0) + " Pixels";

			vTextString = vTextString + "\nTop Reference: " + dtoa(vProductPointer->vReferenceTop / vGlobalPixelsPerUnitInHeight,2) +
				" " + vLocalConfigurationData->vUnitsString + ", " + dtoa(vProductPointer->vReferenceTop, 0) + " Pixels";

			vTextString = vTextString + "\nBottom Reference: " + dtoa(vProductPointer->vReferenceBottom / vGlobalPixelsPerUnitInHeight,2) +
				" " + vLocalConfigurationData->vUnitsString + ", " + dtoa(vProductPointer->vReferenceBottom, 0) + " Pixels";

			if (vLocalConfigurationData->vReferenceROIsToEdges)
				vTextString = vTextString + "\nReference ROIs to edges of containers";
			else
				vTextString = vTextString + "\nReference ROIs to image";
		}

		if (vProductPointer->vDriftDensityInspection)
		{
			vTextString = vTextString + "\n\nProduct Density Correction Setup";

			vTextString = vTextString + "\n    ROI " + vGlobalTopString + ": " + dtoa(vProductPointer->vDriftDensityInspection->vROITop,2) + " " + 
				vLocalConfigurationData->vUnitsString + ", " + dtoa(vProductPointer->vDriftDensityInspection->vROIPixelTop,0) + " Pixels";

			vTextString = vTextString + "\n    ROI " + vGlobalBottomString + ": " + dtoa(vProductPointer->vDriftDensityInspection->vROIBottom,2) + " " + 
				vLocalConfigurationData->vUnitsString + ", " + dtoa(vProductPointer->vDriftDensityInspection->vROIPixelBottom,0) + " Pixels";

			vTextString = vTextString + "\n    ROI Left: " + dtoa(vProductPointer->vDriftDensityInspection->vROILeft,2) + " " + 
				vLocalConfigurationData->vUnitsString + ", " + dtoa(vProductPointer->vDriftDensityInspection->vROIPixelLeft,0) + " Pixels";

			vTextString = vTextString + "\n    ROI Right: " + dtoa(vProductPointer->vDriftDensityInspection->vROIRight,2) + " " + 
				vLocalConfigurationData->vUnitsString + ", " + dtoa(vProductPointer->vDriftDensityInspection->vROIPixelRight,0) + " Pixels";
		}
		else
			vTextString = vTextString + "\n\nProduct Density Correction Not Setup";

		if (vProductPointer->vTooManyRejectsAlarmX[0])
		{
			vTextString = vTextString + "\n\n" + dtoa(vProductPointer->vTooManyRejectsAlarmX[0], 0) + " Rejects of " + dtoa(vProductPointer->vTooManyRejectsAlarmY[0], 0) + " Containers Alarm 1 Enabled";
			if (vProductPointer->vTooManyRejectsAlarmDisableEjectors[0])
			{
				if (vLocalConfigurationData->vEjectContainersNotInspected)
					vTextString = vTextString + "\n    Stop System on alarm";
				else
					vTextString = vTextString + "\n    Disable Ejectors on Alarm";
			}
			int TempBitVector = 1;
			BYTE TempNumberOfInspectionsSelected = 0;
			for (BYTE TempLoop = 0; TempLoop < vProductPointer->vNumberOfInspections; TempLoop++)
			{
				if (TempBitVector & vProductPointer->vInspectionAlarmSettings[0])
					TempNumberOfInspectionsSelected++;
				TempBitVector = TempBitVector * 2;
			}
			char TempBuffer [16];
			_itoa_s(TempNumberOfInspectionsSelected,TempBuffer,16,10);
			vTextString = vTextString + " (" + TempBuffer + " Inspections)" ;
		}
		else
			vTextString = vTextString + "\n\nX of Y Alarm 1 Disabled";

		if (vProductPointer->vTooManyRejectsAlarmX[1])
		{
			vTextString = vTextString + "\n" + dtoa(vProductPointer->vTooManyRejectsAlarmX[1], 0) + " Rejects of " + dtoa(vProductPointer->vTooManyRejectsAlarmY[1], 0) + " Containers Alarm 2 Enabled";

			if (vProductPointer->vTooManyRejectsAlarmDisableEjectors[1])
			{
				if (vLocalConfigurationData->vEjectContainersNotInspected)
					vTextString = vTextString + "\n    Stop System on alarm";
				else
					vTextString = vTextString + "\n    Disable Ejectors on Alarm";
			}
			int TempBitVector = 1;
			BYTE TempNumberOfInspectionsSelected = 0;
			for (BYTE TempLoop = 0; TempLoop < vProductPointer->vNumberOfInspections; TempLoop++)
			{
				if (TempBitVector & vProductPointer->vInspectionAlarmSettings[1])
					TempNumberOfInspectionsSelected++;
				TempBitVector = TempBitVector * 2;
			}
			char TempBuffer [16];
			_itoa_s(TempNumberOfInspectionsSelected,TempBuffer,16,10);
			vTextString = vTextString + " (" + TempBuffer + " Inspections)" ;
		}
		else
			vTextString = vTextString + "\nX of Y Alarm 2 Disabled";

		vTextString = vTextString + "\n\nThreshold for Container Boundary: " + dtoa(vProductPointer->vThresholdForContainerBounds, 0);

		vTextString = vTextString + "\nMaximum Image Overlap: " + dtoa(vProductPointer->vMaximumImageOverlap,2) + " " + vLocalConfigurationData->vUnitsString;

		TempString = "\nEncoder Overscan Factor: ";

		if (vProductPointer->vOverScanFactor == 2)
			TempString = TempString + "4 X";
		else
		if (vProductPointer->vOverScanFactor == 1)
			TempString = TempString + "2 X";
		else
			TempString = TempString + "1 X (Disabled)";

		vTextString = vTextString + TempString;

		vTextString = vTextString + "\n" + TempDividerString;

		if (vProductPointer->vNumberOfInspections)
		{
			for (BYTE TempLoop = 0; TempLoop < vProductPointer->vNumberOfInspections; TempLoop++)
			{
				vTextString = vTextString + "Inspection Setup: " + dtoa(TempLoop + 1, 0);
				bool TempReferenceEnabled = ((vProductPointer->vReferenceRight) || (vProductPointer->vReferenceTop));

				TempString = vProductPointer->vInspection[TempLoop]->MakeSetupDisplayString(TempReferenceEnabled,vProductPointer->vOverScanMultiplier,
					vLocalConfigurationData->GetEjectorNames(vProductPointer->vInspection[TempLoop]->vEjector),vLocalConfigurationData->vUnitsString);

				vTextString = vTextString + TempString;

				vTextString = vTextString + "\n" + TempDividerString;
			}
		}
		else
			vTextString = vTextString + "\n\nNo Inspections Setup";

		for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
		{
			vTextString = vTextString + "\nAuxiliary Detector " + dtoa(TempLoop + 1, 0) + ": " + vLocalConfigurationData->vExternalDetector[TempLoop].vName;

			if (vProductPointer->vExternalDetectorEnable[TempLoop])
			{
				vTextString = vTextString + "\n    Enabled";
				if (!vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled)
				vTextString = vTextString + ", but detector disabled";

				if (vProductPointer->vExternalDetectorMode[TempLoop] < cNumberOfExternalDetectorModes)
				vTextString = vTextString + "\n    Detector Mode: " + vLocalSystemData->vDetectorModeString[vProductPointer->vExternalDetectorMode[TempLoop]];

				if (vProductPointer->vExternalDetectorMode[TempLoop] < cBackupDetectorMode)
				{
					vTextString = vTextString + "\n    Ejector: " + vLocalConfigurationData->GetEjectorNames(vProductPointer->vExternalDetectorEjector[TempLoop]);
					
					vTextString = vTextString + "\n    Window Start: " + dtoa(vProductPointer->vExternalDetectorWindowStart[TempLoop],2);

					vTextString = vTextString + "\n    Window End: " + dtoa(vProductPointer->vExternalDetectorWindowEnd[TempLoop],2);
				}
				else
				if (vProductPointer->vExternalDetectorMode[TempLoop] == cBackupDetectorMode)
					vTextString = vTextString + "\n    Can Stop: " + dtoa(vProductPointer->vExternalDetectorEjector[TempLoop] + 1, 0);

				vTextString = vTextString + "\n";
			}
			else
				vTextString = vTextString + "\n    Disabled"; 
		}

		vTextString = vTextString + "\n";
		vTextString = vTextString + TempDividerString;

		for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectorsForUser; TempLoop++)
		{
			if (!vLocalConfigurationData->vEjector[TempLoop].vEnabled)
			{
				if (TempLoop < 6)
				{
					vTextString = vTextString + "\nEjector " + dtoa(TempLoop + 1, 0) + ": " + vLocalConfigurationData->vEjector[TempLoop].vName;
					vTextString = vTextString + "\n    Disabled";
				}
			}
			else
			{
				vTextString = vTextString + "\nEjector " + dtoa(TempLoop + 1, 0) + ": " + vLocalConfigurationData->vEjector[TempLoop].vName;

				vTextString = vTextString + "\n    Ejector Position: " + dtoa(vProductPointer->vEjectorDelayPosition[TempLoop], 2);

				if (vLocalConfigurationData->vSystemEjectorDistance[TempLoop])
				vTextString = vTextString + "\n    Ejector Offset: " + dtoa(vProductPointer->vEjectorDelayPosition[TempLoop] - 
					vLocalConfigurationData->vSystemEjectorPositionOffset - vLocalConfigurationData->vSystemEjectorDistance[TempLoop],2) + " inches from System Distance of: "  + 
					dtoa(vLocalConfigurationData->vSystemEjectorPositionOffset + vLocalConfigurationData->vSystemEjectorDistance[TempLoop],2);

				//CString TempEjectorText = "Trigger to Ejector Distance: ";
				//if (vLocalConfigurationData->vSystemEjectorPositionOffset > 1)
				//	TempEjectorText = "ScanTrac Output Wall to Ejector Distance: ";

				//if (vProductPointer->vEjectorDelayPosition[TempLoop] > vLocalConfigurationData->vSystemEjectorPositionOffset)
				//	vTextString = vTextString + "\n    " + TempEjectorText + dtoa(vProductPointer->vEjectorDelayPosition[TempLoop] - vLocalConfigurationData->vSystemEjectorPositionOffset,2);
				//else
				//	vTextString = vTextString + "\n*    Distance: Too Short";

				//if (PasswordOK(cTemporaryInspxPassword,false))
				//	vTextString = vTextString + " + ScanTrac Width of " + dtoa(vLocalConfigurationData->vSystemEjectorPositionOffset,2) +
				//	" = " + dtoa(vProductPointer->vEjectorDelayPosition[TempLoop],2) + " Trigger To Ejector";

				vTextString = vTextString + "\n    Response Factor: " + dtoa(vProductPointer->vEjectorResponseTime[TempLoop],4);

				if (vProductPointer->vEjectorDwellTime[TempLoop] == 0)
					vTextString = vTextString + "\n    *Dwell Time: 0 = Diverter";
				else
					vTextString = vTextString + "\n    Dwell Time: " + dtoa(vProductPointer->vEjectorDwellTime[TempLoop],2);
			}
		}

		TempString = dtoa(vProductPointer->vEndOfLineTimeOut,2);
		vTextString = vTextString + "\n\nDistance to Last Ejector: " + TempString + " " + vLocalConfigurationData->vUnitsString;
		vTextString = vTextString + "\n\n";

		SetDlgItemText(IDC_HelpText,vTextString);
		//if (PasswordOK(cTemporaryInspxPassword,false))
			WriteStringToFile("ProductSummary " + *TempProductName, vTextString);

		CHARFORMAT TempCharacterFormat;
		TempCharacterFormat.dwMask = CFM_COLOR ;
		TempCharacterFormat.dwEffects = NULL;
		TempCharacterFormat.crTextColor = cBlack;
		m_HelpTextControl.SetDefaultCharFormat(TempCharacterFormat);

		//show lines in color
		int TempEndCount = vTextString.GetLength();
		if (TempEndCount > cSizeOfVColor)
			TempEndCount = cSizeOfVColor;
		for (int TempPosition = 0; TempPosition < TempEndCount - 5; TempPosition++)
		{
			wchar_t TempChar = vTextString.GetAt(TempPosition);
			wchar_t TempNextChar = vTextString.GetAt(TempPosition + 1);
			if ((TempChar == '+') || ((TempChar == '-') && (TempNextChar == '-')))
			{
				int TempStop = vTextString.Find((char)0x0A,TempPosition);
				if (TempStop > TempEndCount)
					TempStop = TempEndCount;
				for (int TempOffset = TempPosition; TempOffset <= TempStop; TempOffset++)
				if (TempOffset < cSizeOfVColor)
					vColor[TempOffset] = 3; //green
					//vColor[TempOffset] = 2; //blue
					//vColor[TempOffset] = 1; //red
				TempPosition = TempStop;
			}
			else
			if (TempChar == '*')
			{
				int TempStop = vTextString.Find((char)0x0A,TempPosition);
				if (TempStop > TempEndCount)
					TempStop = TempEndCount;
				for (int TempOffset = TempPosition; TempOffset <= TempStop; TempOffset++)
				if (TempOffset < cSizeOfVColor)
					vColor[TempOffset] = 1; //red
				//vColor[TempOffset] = 3; //green
				//vColor[TempOffset] = 2; //blue
				TempPosition = TempStop;
			}
		}
		vShowTable = false;
		SetupFont();

		SetDlgItemText(IDC_HelpText,vTextString);
		DisplayColor(&vTextString);
//		vTimerResult = SetTimer(vHideCaretTimerHandle,10,NULL);

	}
	else
	{  //display system setup summary
		this->SetWindowText(_T("System Summary"));
		vDialogTitleString = "System Setup Summary";
		SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
		SetDlgItemText(IDC_HelpTitle, _T("The same for all products"));
		vShowTable = true;
		SetupFont();
		
		char TempChars[20] = "";
		CString TempString("");
		TempString = "";
		CString TempString2("");
		TempString2 = "";
		vTextString = "";
		
		vTextString = "Active Test Modes:";
		
		if (vLocalSystemData->vNumberOfDemoImages)
			vTextString = vTextString + "\n    " + dtoa(vLocalSystemData->vNumberOfDemoImages,0) +
			" Demo Images";
		if (vLocalSystemData->vTemporarilyChangeSource)
			vTextString = vTextString + "\n    Temporarily Changed X-Ray Source Voltage/Current";
		if (vLocalSystemData->vKeepExternalDetectorsEnabled)
			vTextString = vTextString + "\n    Auxiliary Detectors Forced Enabled";
		//if (vLocalSystemData->vShowContainerTriggerLength)
		//	vTextString = vTextString + "\n    Showing Container Trigger Length";
		if (!vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
			vTextString = vTextString + "\n    Software Simulating Images";
		if (vLocalSystemData->vSimulatingCamLinkImage)
			vTextString = vTextString + "\n    CamLink Board Simulating Images";

		if (vLocalConfigurationData->vSimulateLightControlBoard)
			vTextString = vTextString + "\n    Simulating Interlocks and X-Rays";
		if (vLocalSystemData->vSimulating)
			vTextString = vTextString + "\n    Software Simulating Containers";
		if (vLocalSystemData->vFPGAGeneratingSimulatedData)
			vTextString = vTextString + "\n    uC FPGA Hardware Simulating Images";
		if (vLocalSystemData->vSimulatingCamLinkImage)
			vTextString = vTextString + "\n    CamLink Board Simulating Images";
		//if (vLocalSystemData->vRunWithoutXRays)
		//	vTextString = vTextString + "\n    X-Rays Disabled";
		if (vLocalSystemData->vFPGASimulateEncoderRate)
			vTextString = vTextString + "\n    Simulating Encoder";
		if (vLocalSystemData->vSimulateContainersRate)
			vTextString = vTextString + "\n    Simulating Containers";
		if (vLocalSystemData->vLogFile.vLogSerialData)
			vTextString = vTextString + "\n    Logging Serial Data to/from Micro Controller";
		if (!vLocalSystemData->vLogFile.vLogSyncData)
			vTextString = vTextString + "\n    Not Sending Sync Serial Data to Micro Controller";
		if (vLocalSystemData->vuControllerSendExtraDiagnosticMessages)
			vTextString = vTextString + "\n    Micro Controller Sending Extra Diagnostics";
		if (vLocalSystemData->vuCOutputSerialDebugData)
			vTextString = vTextString + "\n    Micro Controller Outputting Serial Debug Data";
		if (vLocalSystemData->vTestingTrigger)
			vTextString = vTextString + "\n    Testing Triggers";
		if (vLocalSystemData->vuCSimulate16Hz)
			vTextString = vTextString + "\n    Micro Controller Generating 16 Hertz Interrupt";
		if (!vLocalSystemData->vEnableduC16HzInterrupt)
			vTextString = vTextString + "\n    Micro Controller 16 Hertz Interrupt Disabled";
		if (vLocalSystemData->vFPGADontCorrectForEncoderErrors)
			vTextString = vTextString + "\n    Not Correcting Images for Early Encoder Pulses";

		if (vLocalSystemData->vRadiationDoorManualControlRegister)
		{
			if (vLocalSystemData->vRadiationDoorManualControlRegister & 2)
				vTextString = vTextString + "\n    Manual Control of Radiation Shutter, Open";
			else
				vTextString = vTextString + "\n    Manual Control of Radiation Shutter, Closed";
		}
		if (vLocalConfigurationData->vDemoMode)
			vTextString = vTextString + "\n    Demo Mode - Simulate Counts when running";

		if (!vLocalSystemData->vAllSystemSettingsAreStandard)
			vTextString = vTextString + "\n\nSome System Settings are Non-Standard" + vLocalSystemData->vNonStandardSetting;

		if (vMainWindowPointer)
		if (!vMainWindowPointer->TestModesAreActive())
			vTextString = vTextString + "\n    None";

		vTextString = vTextString + "\n\nSpecial Modes:";
		if (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock)
		if (vLocalConfigurationData->vIgnoreCheyneyInterlockStatus)
			vTextString = vTextString + "\n    Ignoring Cheyney Monoblock Interlock Status";


		if (vLocalConfigurationData->vEnableOperationsGuardian)
		{
			vTextString = vTextString + "\n    Operations Guardian Enabled";
			if (vLocalSystemData->vOperationsGuardianConnected)
			{
				vTextString = vTextString + "\n        Operations Guardian Is Sending Message (status at bottom)";
				if (vLocalSystemData->vOperationsGuardianError)
					vTextString = vTextString + ". But has " + dtoa(vLocalSystemData->vOperationsGuardianError, 0) + " transmission errors.";
			}
			else
				vTextString = vTextString + "\n        Operations Guardian Is NOT Connected";
#ifdef _DEBUG
				vTextString = vTextString + "\n    Compiled for Debug Mode. Can not send message to OG Server. You need a release version of ScanTrac";
#endif
		}
		if (vGlobalFBWFEnable == 0)
				vTextString = vTextString + "\n    Hard Drive Protection Disabled";

		if (vLocalConfigurationData->vContinuousFeedEncoderRate)
				vTextString = vTextString + "\n    Continuous Inspection Mode: " + 
				dtoa(vLocalConfigurationData->vContinuousFeedEncoderRate,0) + " Hertz Encoder";
		if (vLocalConfigurationData->vContinuousFeedContainerRate)
				vTextString = vTextString + "\n    Continuous Inspection Mode: " + 
				dtoa(vLocalConfigurationData->vContinuousFeedContainerRate,0) + " CPM";

		if (vGlobalCurrentProduct)
		for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectorsForUser; TempLoop++)
		if (vGlobalCurrentProduct->vEjectorDwellTime[TempLoop] == 0)
			vTextString = vTextString + "\n    Ejector " + dtoa(TempLoop + 1, 0) + " is being controlled as a Diverter";

		if (vLocalConfigurationData->vWriteRejectsToFiles)
		{
			if (vLocalConfigurationData->vSaveAllImagesToRejectsFolder)
				vTextString = vTextString + "\n    Writing All Images to Files";
			else
				vTextString = vTextString + "\n    Writing Rejects to Files";
		}

		if (vLocalConfigurationData->vPixelsPerDetector == 128)
			vTextString = vTextString + "\n    .4 mm Detector Mode";

		if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
		{
			if (vLocalConfigurationData->vDetectorLength != 4)
				vTextString = vTextString + "\nDetector Length Non Standard: " + dtoa(vLocalConfigurationData->vDetectorLength, 0) + ", Normal is 4\" 1.6mm Detectors";
		}
		else
		{
			if (vLocalConfigurationData->vDetectorLength != 2)
				vTextString = vTextString + "\nDetector Length Non Standard: " + dtoa(vLocalConfigurationData->vDetectorLength, 0) + ", Normal is 2\" .8mm Detectors";
		}

		if ((vGlobalCurrentProduct) && (vGlobalCurrentProduct->vNumberOfInspectionsRequiredToEject > 1))
				vTextString = vTextString + "\n     " + dtoa(vGlobalCurrentProduct->vNumberOfInspectionsRequiredToEject, 0) + " Inspections must find a contaminant to Eject container";

		if (vLocalSystemData->vInspxMessagesToView)
			vTextString = vTextString + "\n\nInspx messages available to view";


		vTextString = vTextString + "\n";
		BYTE TempWhichModesToShow = 0;
		if ((vLocalConfigurationData->vUseXScanDetectors) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))
			TempWhichModesToShow = 1;

		for (WORD TempDetectorTypeLoop = 0; TempDetectorTypeLoop <= TempWhichModesToShow; TempDetectorTypeLoop++)
		{
			BYTE TempDisplayPixelsPerDetector = vLocalConfigurationData->vPixelsPerDetector;
			CString vDetectorTypeTextString = "";
			if (vLocalConfigurationData->vUseXScanDetectors)
			if (TempDetectorTypeLoop == 0)
			{
				vDetectorTypeTextString = ".8mm Pixel mode ";
				TempDisplayPixelsPerDetector = 64;
			}
			else
			{
				vDetectorTypeTextString = ".4mm Pixel mode ";
				TempDisplayPixelsPerDetector = 128;
			}

			WORD TempNumberOfPixelsToAdjust = vLocalConfigurationData->vNumberOfXRayDetectors * TempDisplayPixelsPerDetector;
			if (TempDetectorTypeLoop == 1)
				TempNumberOfPixelsToAdjust = vLocalConfigurationData->vNumberOfXRayDetectors * 128; //128 is .4mm Detector Mode

			for (WORD TempLoop = 1; TempLoop < TempNumberOfPixelsToAdjust - 1; TempLoop++)
			{
				if (vLocalConfigurationData->vDetectorEdgeAdjustGain[TempLoop][TempDetectorTypeLoop] > .1)
				if ((vLocalConfigurationData->vDetectorEdgeAdjustGain[TempLoop][TempDetectorTypeLoop] < .52) || (vLocalConfigurationData->vDetectorEdgeAdjustGain[TempLoop][TempDetectorTypeLoop] > 2))
				{
					vTextString = vTextString + "\n" + vDetectorTypeTextString + "Detector Pixel " + Pad(dtoa(TempLoop), 3,3) + "     \tGain: " + dtoa(vLocalConfigurationData->vDetectorEdgeAdjustGain[TempLoop][TempDetectorTypeLoop],2) + " At Limit";
				}
				if (vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempLoop][TempDetectorTypeLoop])
				if ((vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempLoop][TempDetectorTypeLoop] <= -50) || (vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempLoop][TempDetectorTypeLoop] >= 50))
				{
					vTextString = vTextString + "\n" + vDetectorTypeTextString + "Detector Pixel " + Pad(dtoa(TempLoop), 3,3) + "     \tOffset: " + dtoa(vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempLoop][TempDetectorTypeLoop],2) + " At Limit";
				}
			}
		}
		vTextString = vTextString + "\n";

		TempTestModesLength = vTextString.GetLength();

		switch (vLocalSystemData->vSystemRunMode)
		{
			case cStoppedSystemMode: vTextString = vTextString + "\nInspection is Stopped"; break;
			case cRunningSystemMode: vTextString = vTextString + "\nSystem is Inspecting"; break;
			case cCalibratingDetectorEdgesMode: vTextString = vTextString + "\nSystem is Calibrating Detectors"; break;
			case cCalibratingDetectorOffsetMode: vTextString = vTextString + "\nSystem is Calibrating Black Level"; break;
			case cAutoSetupRunningSystem: vTextString = vTextString + "\nSystem is in Setup Mode"; break;
			case cMeasureSourceStrengthMode: vTextString = vTextString + "\nSystem is Measuring Source Strength"; break;
			case cConditionXRayTubeMode: vTextString = vTextString + "\nSystem is Conditioning Tube"; break;
			case cCalibratingDetectorGainMode: vTextString = vTextString + "\nSystem is Calibrating Detector Gain"; break;
			case cTestShutterMode: vTextString = vTextString + "\nSystem is Testing Shutter"; break;
			case cAlignTubeMode: vTextString = vTextString + "\nSystem is in Tube Align Mode"; break;
			case cDarkCurrentMode: vTextString = vTextString + "\nSystem is Measuring Dark Current"; break;
			case cCalibrateEncoderMode: vTextString = vTextString + "\nSystem is Calibrating Encoder"; break;
		}

		if (vLocalConfigurationData->vBulkProductMode)
			vTextString = vTextString + "\nUsing Bulk Mode";
		//else
		//	vTextString = vTextString + "\nUsing Real Trigger";

		if (!vLocalConfigurationData->vEnableEjectors)
		{
			if (vLocalSystemData->vInAutoSetup)
				vTextString = vTextString + "\nEjectors Disabled - In Auto Setup";
			else
				vTextString = vTextString + "\nEjectors Disabled - System Disabled";
		}
		else
		if (vLocalSystemData->vInAutoSetup)
			vTextString = vTextString + "\n++In Auto Setup";


		vTextString = vTextString + "\n\nScanTrac Machine Name: " + vLocalConfigurationData->vScanTracID;
		vTextString = vTextString + "\nX-Ray Tube Serial Number: " + vGlobalLifeTimeCounters->vXRayTubeSerialNumber;
		vTextString = vTextString + "\n";

		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		CString TempFileNameC = vLocalConfigurationData->vScanTracCalibrationImagesDirectory + "CalibratePixelsData*.csv";
		CString TempFileNameStringF = "Test";
		LPTSTR TempFileFindNameString = TempFileNameC.GetBuffer(TempFileNameC.GetLength());
		CFileStatus TempFileStatus;

		hFind = FindFirstFile(TempFileFindNameString, &FindFileData);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			CString TempFileNameStringA = FindFileData.cFileName;
			if (TempFileNameStringA.GetLength() > 20)
			{
				CTime TempMostRecentDate = FindFileData.ftLastWriteTime;
				TempFileNameStringF = vLocalConfigurationData->vScanTracCalibrationImagesDirectory + FindFileData.cFileName;
				while (FindNextFile(hFind,&FindFileData))
				{
					if (TempMostRecentDate < FindFileData.ftLastWriteTime)
					{
						TempMostRecentDate = FindFileData.ftLastWriteTime;
						TempFileNameStringF = vLocalConfigurationData->vScanTracCalibrationImagesDirectory + FindFileData.cFileName;
					}
				}

				LPTSTR TempFileNameString = TempFileNameStringF.GetBuffer(TempFileNameStringF.GetLength());
				CFileStatus TempStatus;
				if (CFile::GetStatus(TempFileNameString, TempStatus))
				{
					vTextString = vTextString + "\nDetectors last calibrated on: " + TempMostRecentDate.Format("%B %d %Y %H:%M:%S");//24 hour time format
					//vTextString = vTextString + "\nDetectors calibrated file: " + TempStatus.m_szFullName;
				}
				if (vLocalConfigurationData->vTimeLastMeasuredSourceStrength > 100)
					vTextString = vTextString + "\nMeasure Source Strength done on: " + vLocalConfigurationData->vTimeLastMeasuredSourceStrength.Format("%B %d %Y %H:%M:%S");//24 hour time format
				else
					vTextString = vTextString + "\nMeasure Source Strength done on: NOT RECORDED";

				if (vLocalConfigurationData->vTimeXRaysWereOnLast > 100)
					vTextString = vTextString + "\nX-Rays last run on: " + vLocalConfigurationData->vTimeXRaysWereOnLast.Format("%B %d %Y %H:%M:%S");//24 hour time format
				else
					vTextString = vTextString + "\nX-Rays last run on: NOT RECORDED";
			}
		}
		vTextString = vTextString + "\n";

		if ((vLocalConfigurationData->vUseXScanDetectors) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))
		{
			if (vLocalConfigurationData->vEncoderDividerForPoint4mmMode == 0)
				vTextString = vTextString + "\nConfigured Encoder Divider: .8mm Mode: " + dtoa(vLocalConfigurationData->vEncoderDivider, 0) + ", .4mm Mode: Auto = 1/2 of .8mm";
			else
				vTextString = vTextString + "\nConfigured Encoder Divider: .8mm Mode: " + dtoa(vLocalConfigurationData->vEncoderDivider, 0) + ", .4mm Mode: " + dtoa(vLocalConfigurationData->vEncoderDividerForPoint4mmMode, 0);
			
			vTextString = vTextString + "\nCurrent Encoder Divider: " + dtoa(vLocalSystemData->vCurrentEncoderDivider, 0);
		}
		else
			vTextString = vTextString + "\nEncoder Divider: " + dtoa(vLocalConfigurationData->vEncoderDivider, 0);

		if ((vLocalConfigurationData->vUseXScanDetectors) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))
		{
			if (vLocalConfigurationData->vPixelsPerUnitForPoint4mmMode == 0)
				vTextString = vTextString + "\nConfigured Pulses Per Inch: .8mm Mode: " + dtoa(vLocalConfigurationData->vPixelsPerUnit, 2) + ", .4mm Mode: Auto=double .8mm";
			else
				vTextString = vTextString + "\nConfigured Encoder Pulses Per Inch: .8mm Mode: " + dtoa(vLocalConfigurationData->vPixelsPerUnit, 2) + ", .4mm Mode: " + dtoa(vLocalConfigurationData->vPixelsPerUnitForPoint4mmMode, 2);
			
			vTextString = vTextString + "\nCurrent Encoder Pulses Per Inch: " + dtoa(vGlobalPixelsPerUnit, 2);
		}
		else
			vTextString = vTextString + "\nEncoder Pulses Per Inch: " + dtoa(vLocalConfigurationData->vPixelsPerUnit, 0);

		vTextString = vTextString + "\nDetector Clock Divider: " + dtoa(vLocalConfigurationData->vDetectorClockDivider, 0);

		if (vLocalConfigurationData->vCalibratedPixelsPerUnit)
		{
			if ((int)vLocalConfigurationData->vCalibratedPixelsPerUnit != (int)vGlobalPixelsPerUnit)
				vTextString = vTextString + "\n+    Auto Encoder Calibration Resulted In: " + 
				dtoa(vLocalConfigurationData->vCalibratedPixelsPerUnit,2) + " PPI. Current Pulses Per Inch: " + dtoa(vGlobalPixelsPerUnit,2);
		}
		vTextString = vTextString + "\nPixels Per Inch In Height: " + dtoa(vGlobalPixelsPerUnitInHeight,2);

		if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam == cSoloLeftToRightDistance)
			vTextString = vTextString + "\nScanTrac is a Left to Right Solo";
		else
		if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam == cSoloRightToLeftDistance)
			vTextString = vTextString + "\nScanTrac is a Right to Left Solo";
		else
		if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam == cPiccoloDistance)
			vTextString = vTextString + "\nScanTrac is a Piccolo";
		else
		if ((vLocalConfigurationData->vSystemBodyTriggerToXRayBeam == cForteTriggerToBeamOffset) && 
			(vLocalConfigurationData->vSystemEjectorPositionOffset == cForteLeftToRightEjectorOffset))
			vTextString = vTextString + "\nScanTrac is a 600 Watt Pipeline Left To Right";
		else
		if ((vLocalConfigurationData->vSystemBodyTriggerToXRayBeam == cForteTriggerToBeamOffset) && 
			(vLocalConfigurationData->vSystemEjectorPositionOffset == cForteRightToLeftEjectorOffset))
			vTextString = vTextString + "\nScanTrac is a 600 Watt Pipeline Right To Left";

		TempString = "";
		if ((vLocalConfigurationData->vSystemBodyTriggerToXRayBeam != cPiccoloDistance) &&
			(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam != cSoloRightToLeftDistance) &&
			(!((vLocalConfigurationData->vSystemBodyTriggerToXRayBeam == cForteTriggerToBeamOffset) && 
			(vLocalConfigurationData->vSystemEjectorPositionOffset == cForteLeftToRightEjectorOffset))) &&
			(!((vLocalConfigurationData->vSystemBodyTriggerToXRayBeam == cForteTriggerToBeamOffset) && 
			(vLocalConfigurationData->vSystemEjectorPositionOffset == cForteRightToLeftEjectorOffset))) &&
			(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam != cSoloLeftToRightDistance))
			TempString = "*";

		vTextString = vTextString + "\n" + TempString + "Distance From Container Trigger to X-Ray Beam: " + 
			dtoa(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam,2);

		TempString = "";
		if ((vLocalConfigurationData->vSystemEjectorPositionOffset != cSoloEjectorOffset) &&
			(vLocalConfigurationData->vSystemEjectorPositionOffset != cPiccoloEjectorOffset))
			TempString = "*";
		vTextString = vTextString + "\n" + TempString + "Ejector Offset (Trigger to output wall): " + dtoa(vLocalConfigurationData->vSystemEjectorPositionOffset,2);

		if (vLocalConfigurationData->vSystemEjectorPositionOffset)
		{
			for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectorsForUser; TempLoop++)
			if (vLocalConfigurationData->vEjector[TempLoop].vEnabled)
			if (vLocalConfigurationData->vSystemEjectorDistance[TempLoop])
			{
				vTextString = vTextString + "\n    ";

				vTextString = vTextString + "Ejector " + dtoa(TempLoop + 1, 0) + ": ScanTrac output wall to ejector center: " + 
					dtoa(vLocalConfigurationData->vSystemEjectorDistance[TempLoop], 2) + 
					"         T to E: " + dtoa(vLocalConfigurationData->vSystemEjectorDistance[TempLoop] + vLocalConfigurationData->vSystemEjectorPositionOffset, 2);
			}
		}
		vTextString = vTextString + "\n";

		vTextString = vTextString + "\n";
		vTextString = vTextString + TempDividerString;

		if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
		{
			vTextString = vTextString + "Filler Monitoring/Sampling Enabled for: " + dtoa(vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType], 0) + " valves";

			if (vLocalSystemData->vUseSamplingFIFO)
				vTextString = vTextString + "\nUsing FIFO to track containers from seamer/filler";
			else
				vTextString = vTextString + "\nNot Using FIFO to track containers from seamer/filler";

			vTextString = vTextString + "\nHead 1 Offset from Index Pulse: " + dtoa(vMainWindowPointer->GetCurrentFillerIndexOffset(), 0);
			vTextString = vTextString + "\nIndex Pulse Bit Map: " + dtoa(vLocalSystemData->vFillerIndexPulseBitMap, 0);
			vTextString = vTextString + "\nSeamer Object Pulse Bit Map: " + dtoa(vLocalSystemData->vSeamerObjectPulseBitMap, 0);
			vTextString = vTextString + "\nSeamer Pocket Sensor Pulse Bit Map: " + dtoa(vLocalSystemData->vSeamerPocketSensorBitMap, 0);
			
			vTextString = vTextString + "\nTracking FIFO Timeout Distance: " + dtoa(vLocalConfigurationData->vDistanceFromSeamerObjectToScanTracTrigger, 0) + " inches";

			if (vLocalSystemData->vSampledOnLastRevolution[cFillerSamplingType])
				vTextString = vTextString + "\nFiller Sample on Last Revolution";
			else
				vTextString = vTextString + "\nDid not Filler Sample on Last Revolution";

			if (vLocalSystemData->vSampledOnLastRevolution[cSeamerSamplingType])
				vTextString = vTextString + "\nSeamer Sample on Last Revolution";
			else
				vTextString = vTextString + "\nDid not Seamer Sample on Last Revolution";

			if (vLocalSystemData->vSamplingFIFOHead[cFillerSamplingType] == vLocalSystemData->vSamplingFIFOTail[cFillerSamplingType])
				vTextString = vTextString + "\nFiller FIFO Empty";
			else
			{
				int TempFIFOItems = vLocalSystemData->vSamplingFIFOHead[cFillerSamplingType] - vLocalSystemData->vSamplingFIFOTail[cFillerSamplingType];
				if (vLocalSystemData->vSamplingFIFOHead[cFillerSamplingType] < vLocalSystemData->vSamplingFIFOTail[cFillerSamplingType])
					TempFIFOItems = TempFIFOItems + 256;
				vTextString = vTextString + "\nFiller FIFO has " + dtoa(TempFIFOItems, 0);
			}

			if (vLocalSystemData->vSamplingFIFOHead[cSeamerSamplingType] == vLocalSystemData->vSamplingFIFOTail[cSeamerSamplingType])
				vTextString = vTextString + "\nSeamer FIFO Empty";
			else
			{
				int TempFIFOItems = vLocalSystemData->vSamplingFIFOHead[cSeamerSamplingType] - vLocalSystemData->vSamplingFIFOTail[cSeamerSamplingType];
				if (vLocalSystemData->vSamplingFIFOHead[cSeamerSamplingType] < vLocalSystemData->vSamplingFIFOTail[cSeamerSamplingType])
					TempFIFOItems = TempFIFOItems + 256;
				vTextString = vTextString + "\nSeamer FIFO has " + dtoa(TempFIFOItems, 0);
			}

			
		//			CInspection *TempInspection = NULL;
		//if (vLocalSystemData->vValveMonitorInspection <= vGlobalCurrentProduct->vNumberOfInspections)
		//	TempInspection = vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection];
		//if (!TempInspection)
		//	TempInspection = vGlobalCurrentProduct->vFillerMonitorInspection;

			if (vGlobalCurrentProduct)
			if (vLocalSystemData->vValveMonitorInspection <= vGlobalCurrentProduct->vNumberOfInspections)
			{
				vTextString = vTextString + "\nInspection number: " + dtoa(vLocalSystemData->vValveMonitorInspection + 1, 0);
				if ((vLocalSystemData->vValveMonitorInspection < vGlobalCurrentProduct->vNumberOfInspections) && (vGlobalCurrentProduct) && (vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection]))
					vTextString = vTextString + "\nInspection Name: " + vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection]->vName;
			}
			if (vGlobalCurrentProduct->vFillerMonitorInspection)
			{
				vTextString = vTextString + "\nFiller Monitor Inspection Upper Threshold (Underfill): " + dtoa(vGlobalCurrentProduct->vFillerMonitorInspection->vDensityThresholdUpper, 0);
				vTextString = vTextString + "\nFiller Monitor Inspection Lower Threshold (Overfill): " + dtoa(vGlobalCurrentProduct->vFillerMonitorInspection->vDensityThresholdLower, 0);
			}
			
			if (PasswordOK(cSuperInspxPassword,false))
			if (vLocalSystemData->vRecievedFirstIndexPulse[cFillerSamplingType])
			{
				vTextString = vTextString + "\nFirst Index Pulse has occured";
		
				if (vLocalSystemData->vUseSamplingFIFO)
				{
					vTextString = vTextString + "\n    vNextHeadToPutInFIFO[cFillerSamplingType]: " + dtoa(vLocalSystemData->vNextHeadToPutInFIFO[cFillerSamplingType] + 1, 0);
					vTextString = vTextString + "\n    vCountPutInFIFOThisRevolution[cFillerSamplingType]: " + dtoa(vLocalSystemData->vCountPutInFIFOThisRevolution[cSeamerSamplingType] + 1, 0);
				}

				vTextString = vTextString + "\n    vNextHeadToProcess[cFillerSamplingType]: " + dtoa(vLocalSystemData->vNextHeadToProcess[cFillerSamplingType] + 1, 0);
				vTextString = vTextString + "\n    vLast filler valve processed: " + dtoa(vMainWindowPointer->CalculateHeadNumber(vLocalSystemData->vNextHeadToProcess[cFillerSamplingType], cFillerSamplingType), 0);
				vTextString = vTextString + "\n    vValveDataRowIndex: " + dtoa(vLocalSystemData->vValveDataRowIndex, 0);
				vTextString = vTextString + "\n    vNumberValveRowDataHave: " + dtoa(vLocalSystemData->vNumberValveRowDataHave, 0);
				vTextString = vTextString + "\n    vSamplingIndexErrorDataIndex[cFillerSamplingType]: " + dtoa(vLocalSystemData->vSamplingIndexErrorDataIndex[cFillerSamplingType], 0);
				vTextString = vTextString + "\n    vSamplingIndexErrorDataHave[cFillerSamplingType]: " + dtoa(vLocalSystemData->vSamplingIndexErrorDataHave[cFillerSamplingType], 0);
			}
			else
				vTextString = vTextString + "\nFirst Index Pulse has not occured";

			vTextString = vTextString + "\n";
			vTextString = vTextString + TempDividerString;
		}
		else
			vTextString = vTextString + "\nFiller Monitoring/Sampling Disabled\n";

		if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType])
		{
			vTextString = vTextString + "Seamer Sampling Enabled for: " + dtoa(vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType], 0) + " heads";
			vTextString = vTextString + "\nHead 1 Offset from Index Pulse: " + dtoa(vMainWindowPointer->GetCurrentSeamerIndexOffset(), 0);
			vTextString = vTextString + "\nIndex Pulse Bit Map: " + dtoa(vLocalSystemData->vSeamerIndexPulseBitMap, 0);
			
			if (PasswordOK(cSuperInspxPassword,false))
			if (vLocalSystemData->vRecievedFirstIndexPulse[cSeamerSamplingType])
			{
				vTextString = vTextString + "\nFirst Index Pulse has occured";
		
				if (vLocalSystemData->vUseSamplingFIFO)
				{
					vTextString = vTextString + "\n    vNextHeadToPutInFIFO[cSeamerSamplingType]: " + dtoa(vLocalSystemData->vNextHeadToPutInFIFO[cSeamerSamplingType] + 1, 0);
					vTextString = vTextString + "\n    vCountPutInFIFOThisRevolution[cSeamerSamplingType]: " + dtoa(vLocalSystemData->vCountPutInFIFOThisRevolution[cSeamerSamplingType] + 1, 0);
				}

				vTextString = vTextString + "\n    vNextHeadToProcess[cSeamerSamplingType]: " + dtoa(vLocalSystemData->vNextHeadToProcess[cSeamerSamplingType], 0);
				vTextString = vTextString + "\n    vLast Seamer Head Processed: " + dtoa(vMainWindowPointer->CalculateHeadNumber(vLocalSystemData->vNextHeadToProcess[cSeamerSamplingType], cSeamerSamplingType), 0);
				vTextString = vTextString + "\n    vSamplingIndexErrorDataIndex[cSeamerSamplingType]: " + dtoa(vLocalSystemData->vSamplingIndexErrorDataIndex[cSeamerSamplingType], 0);
				vTextString = vTextString + "\n    vSamplingIndexErrorDataHave[cSeamerSamplingType]: " + dtoa(vLocalSystemData->vSamplingIndexErrorDataHave[cSeamerSamplingType], 0);
			}
			else
				vTextString = vTextString + "\nFirst Index Pulse has not occured";

			vTextString = vTextString + "\n";
			vTextString = vTextString + TempDividerString;
		}
		else
			vTextString = vTextString + "\nSeamer Sampling Disabled\n";

		vTextString = vTextString + "\nNumber of X-Ray Detectors: " + dtoa(vLocalConfigurationData->vNumberOfXRayDetectors, 0);
		vTextString = vTextString + "\nNumber of X-Ray Detectors Using: " + dtoa(vLocalSystemData->vNumberOfXRayDetectorsUsing, 0);


		if (vLocalConfigurationData->vStopAfterAnInspectionRejects)
			vTextString = vTextString + "\nJust showing first inspection that rejects a container";
		else
			vTextString = vTextString + "\nShowing all inspections that reject a container";

		vTextString = vTextString + "\nTime Period for CPM: " + dtoa(vLocalConfigurationData->vCPMTimePeriod, 0) + " seconds";

		vTextString = vTextString + "\nTime Period for Work Stop: " + dtoa(vLocalSystemData->vWorkStopTimePeriod, 0) + " seconds";

		if (vLocalConfigurationData->vAutoProductionReportHour)
		{
			vTextString = vTextString + "\nAuto Production Report Hour: " + dtoa(vLocalConfigurationData->vAutoProductionReportHour, 0);

			if (vLocalConfigurationData->vAutoProductionReportHour2)
			{
				vTextString = vTextString + "\n    Auto Production Report 2 Hour: " + dtoa(vLocalConfigurationData->vAutoProductionReportHour2, 0);

				if (vLocalConfigurationData->vAutoProductionReportHour3)
				{
					vTextString = vTextString + "\n    Auto Production Report 3 Hour: " + dtoa(vLocalConfigurationData->vAutoProductionReportHour3, 0);
				}
			}
		}
		else
			vTextString = vTextString + "\nAuto Production Report Disabled";

		vTextString = vTextString + "\nAuto Stop: ";
		if (vLocalConfigurationData->vAutoStop == 1)
			vTextString = vTextString + dtoa(vLocalConfigurationData->vAutoStop, 0) + " hour";
		else
			if (vLocalConfigurationData->vAutoStop)
				vTextString = vTextString + dtoa(vLocalConfigurationData->vAutoStop, 0) + " hours";
		else
			vTextString = vTextString + "Disabled";

		vTextString = vTextString + "\nNumber of X-Ray Detectors: " + dtoa(vLocalConfigurationData->vNumberOfXRayDetectors, 0);

		if (vGlobalFPGAVersion16point0orAbove)
		{
			if ((vLocalConfigurationData->vUseXScanDetectors) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))
			{
				if (vLocalConfigurationData->vPixelsPerDetector == 128)
					vTextString = vTextString + "\nX-Scan .4mm X-Ray Detector Pixel Size, 128 Pixels per Detector";
				else
				if (vLocalConfigurationData->vPixelsPerDetector == 64)
					vTextString = vTextString + "\nX-Scan .8mm X-Ray Detector Pixel Size, 64 Pixels per Detector";
				else
					vTextString = vTextString + "\nError X-Ray Detector Pixel Size";
			}
			else
			{
				if (vLocalConfigurationData->vPixelsPerDetector == 128)
					vTextString = vTextString + "\nDT .4mm X-Ray Detector Pixel Size, 128 Pixels per Detector";
				else
				if (vLocalConfigurationData->vPixelsPerDetector == 64)
					vTextString = vTextString + "\nDT .8mm X-Ray Detector Pixel Size, 64 Pixels per Detector";
				else
					vTextString = vTextString + "\nError X-Ray Detector Pixel Size";
			}
		}
		else
		{
			if (vLocalConfigurationData->vPixelsPerDetector == 128)
				vTextString = vTextString + "\n.4mm X-Ray Detector Pixel Size, 128 Pixels per Detector";
			else
			if (vLocalConfigurationData->vPixelsPerDetector == 64)
				vTextString = vTextString + "\n.8mm X-Ray Detector Pixel Size, 64 Pixels per Detector";
			else
				vTextString = vTextString + "\nError X-Ray Detector Pixel Size";
		}

		TempString = "Undefined";
		switch(vLocalConfigurationData->vPreAmplifierGain)
		{
			case 0:	TempString = "1";
			break;
			case 1:	TempString = "2";
			break;
			case 2:	TempString = "4";
			break;
			case 3:	TempString = "8";
			break;
		}
		vTextString = vTextString + "\nPreamplifier Gain for All Detectors: " + TempString;

		vTextString = vTextString + "\n\nEncoder Tolerance for warning in Hz: " + dtoa(vLocalConfigurationData->vEncoderTolerance,0);
		vTextString = vTextString + "\nEncoder Number Of Rate Reversals in 2 minutes for warning: " + dtoa(vLocalConfigurationData->vNumberOfEncoderRateReversalsForError,0);

		if (vLocalConfigurationData->vBackupDetectionUsingBodyTriggerTimeOut & 0x7F)
		{
			if (vLocalConfigurationData->vBackupDetectionUsingBodyTriggerTimeOut & 0x80)
				vTextString = vTextString + "\nBackup Detection Timeout: " + dtoa(vLocalConfigurationData->vBackupDetectionUsingBodyTriggerTimeOut & 0x7F,0) + " and Stop Inspecting";
			else
				vTextString = vTextString + "\nBackup Detection Timeout: " + dtoa(vLocalConfigurationData->vBackupDetectionUsingBodyTriggerTimeOut & 0x7F,0) + " and Keep Inspecting";
		}
		else
			vTextString = vTextString + "\nNOT Using Body Trigger for Container Backup Detection in Tunnel";

		vTextString = vTextString + "\nDelta Temp for Filter Clogged Warning: " + dtoa(vLocalConfigurationData->vDeltaTemperatureForFilterCloggedError,0);

		vTextString = vTextString + "\nDelta Temp for Coolant Problem Warning: " + dtoa(vLocalConfigurationData->vDeltaTemperatureForCoolantProblem,0);

		vTextString = vTextString + "\n";
		for (WORD TempLoop = 0; TempLoop < cNumberOfBodyTriggers; TempLoop++)
		{
			if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode))
				vTextString = vTextString + "\nTrigger " + dtoa(TempLoop + 1, 0) + ": " + vLocalConfigurationData->vBodyTrigger[TempLoop].vName;
			else
				vTextString = vTextString + "\nContainer Trigger " + dtoa(TempLoop + 1, 0) + ": " + vLocalConfigurationData->vBodyTrigger[TempLoop].vName;

			if (vLocalConfigurationData->vBodyTrigger[TempLoop].vEnabled)
			{
				vTextString = vTextString + "\n    Enabled";
				
				vTextString = vTextString + "\n    Digital Line Input: " + dtoa(vLocalConfigurationData->vBodyTrigger[TempLoop].vDigitalInputLine, 0);

				if (vLocalConfigurationData->vBodyTrigger[TempLoop].vActiveHigh)
					TempString = "Active High";
				else
					TempString = "Active Low";
				vTextString = vTextString + " " + TempString;
			}
			else
				vTextString = vTextString + "\n    Disabled";
		}

		vTextString = vTextString + "\n";
		vTextString = vTextString + TempDividerString;

		for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
		{
			if (TempLoop)
				vTextString = vTextString + "\n"; 

			vTextString = vTextString + "Auxiliary Detector " + dtoa(TempLoop + 1, 0) + ": " + vLocalConfigurationData->vExternalDetector[TempLoop].vName;

			if (vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled)
			{
				vTextString = vTextString + "\n    Enabled";
				
				vTextString = vTextString + "\n    Digital Line Input: " + dtoa(vLocalConfigurationData->vExternalDetector[TempLoop].vDigitalInputLine, 0);

				if (vLocalConfigurationData->vExternalDetector[TempLoop].vActiveHigh)
					TempString = "Active High";
				else
					TempString = "Active Low";
				vTextString = vTextString + " " + TempString;
			}
			else
				vTextString = vTextString + "\n    Disabled";
		}

		vTextString = vTextString + "\n";
		vTextString = vTextString + TempDividerString;

		for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectorsForUser; TempLoop++)
		{
			if (TempLoop)
				vTextString = vTextString + "\n"; 

			vTextString = vTextString + "Ejector " + dtoa(TempLoop + 1, 0) + ": " + vLocalConfigurationData->vEjector[TempLoop].vName;

			if (vLocalConfigurationData->vEjector[TempLoop].vEnabled)
			{
				vTextString = vTextString + "\n    Enabled";
				
				vTextString = vTextString + "\n    Digital Line Input: " + dtoa(vLocalConfigurationData->vEjector[TempLoop].vDigitalInputLine, 0);

				if (vLocalConfigurationData->vEjector[TempLoop].vActiveHigh)
					TempString = "Active High";
				else
					TempString = "Active Low";
				vTextString = vTextString + " " + TempString;

				if (vLocalConfigurationData->vEjector[TempLoop].vConfirmEnabled)
				{
					vTextString = vTextString + "\n    Confirm Enabled";

					vTextString = vTextString + "\n    Confirm Digital Line Input: " + dtoa(vLocalConfigurationData->vEjector[TempLoop].vConfirmDigitalInputLine, 0);

					if (vLocalConfigurationData->vEjector[TempLoop].vConfirmActiveHigh)
						TempString = "Active High";
					else
						TempString = "Active Low";
					vTextString = vTextString + " " + TempString;
				}
				else
					vTextString = vTextString + "\n    Confirm Disabled";
			}
			else
				vTextString = vTextString + "\n    Disabled";
		}
		if (vLocalConfigurationData->vAlarmEjectorReEnableX)
		{
			vTextString = vTextString + "\n\nRe-enable ejectors after alarm when: " +
				dtoa(vLocalConfigurationData->vAlarmEjectorReEnableX,0) + " of " +
				dtoa(vLocalConfigurationData->vAlarmEjectorReEnableY,0) + " good";
		}
		else
			vTextString = vTextString + "\n\nRe-enable ejectors when running good after alarm Disabled";

		vTextString = vTextString + "\n";
		vTextString = vTextString + TempDividerString;


		if (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)
			vTextString = vTextString + "\n\nFront Detector\n";
		else
			vTextString = vTextString + "\n\n" + vGlobalTopString + " Detector";

		for (int TempLoopI = vLocalConfigurationData->vNumberOfXRayDetectors - 1; TempLoopI >= 0; TempLoopI--)
		{
			//if (TempLoopI != vLocalConfigurationData->vNumberOfXRayDetectors - 1)
			//	vTextString = vTextString + "\n"; 

			//TempString = dtoa(vLocalConfigurationData->vDetectorGain[TempLoopI],2);
			//vTextString = vTextString + "Detector " + dtoa(TempLoopI + 1, 0) + " Gain: " + TempString;
		
			//vTextString = vTextString + " Offset: " + 
			//	dtoa(vLocalSystemData->vDACDetectorOffset[TempLoopI], 2) + "%, " +
			//	dtoa(vLocalSystemData->vADCDetectorOffset[TempLoopI], 0) + "mV  Setup Offset: " +
			//	dtoa(vLocalConfigurationData->vDACDetectorOffset[TempLoopI], 2)+ "%, " + 
			//	dtoa(vLocalConfigurationData->vADCDetectorOffset[TempLoopI], 0)+ "mv";

			CString TempLineString = dtoa(TempLoopI + 1, 0);

			FillToTab(&TempLineString, 3);
			TempLineString = TempLineString + "Gain: " + Pad(dtoa(vLocalConfigurationData->vDetectorGain[TempLoopI],2), 2, 2);
		
			FillToTab(&TempLineString, 15);
			TempLineString = TempLineString + "Offset: " + Pad(dtoa(vLocalSystemData->vDACDetectorOffset[TempLoopI], 2), 2, 2) + "%, ";
				  
			FillToTab(&TempLineString, 30);
			TempString = Pad(dtoa(vLocalSystemData->vADCDetectorOffset[TempLoopI], 0), 4 ,0);
			TempLineString = TempLineString + TempString + "mV";

			FillToTab(&TempLineString, 40);
			TempLineString = TempLineString +"Setup Offset: " + Pad(dtoa(vLocalConfigurationData->vDACDetectorOffset[TempLoopI], 2), 2, 2) + "%, ";

			FillToTab(&TempLineString, 60);
			TempLineString = TempLineString + Pad(dtoa(vLocalConfigurationData->vADCDetectorOffset[TempLoopI], 0), 4 , 0)+ "mv";

			vTextString = vTextString + "\n" + TempLineString; 
		}
		if (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)
			vTextString = vTextString + "\n\nBack Detector\n";
		else
			vTextString = vTextString + "\n" + vGlobalBottomString + " Detector";

		vTextString = vTextString + "\n";

		if (vLocalConfigurationData->vCorrectDetectorEdges == 1)
		{
			vTextString = vTextString + "\nCorrecting Only Edge Pixels";
		}
		else
		if (vLocalConfigurationData->vCorrectDetectorEdges == 2)
		{
			CString TempString5 = " In All Detectors";
			if (vLocalConfigurationData->vNumberOfDetectorsToCalibrate)
				TempString5 = " In Lower " + dtoa(vLocalConfigurationData->vNumberOfDetectorsToCalibrate,0) + " Detectors";
			vTextString = vTextString + "\nCorrecting All Pixels";
		}
		else
		{
			vTextString = vTextString + "\nNot Correcting Any Pixels";
		}
		vTextString = vTextString + "\nConveyor blocks bottom " + dtoa(vLocalConfigurationData->vNumberOfPixelAtBottomToIgnoreInCalibration,0) + " pixels";

		BYTE *TempPixelValuePointer = NULL;
		if (PasswordOK(cTemporaryInspxPassword,false))
		if (vLocalSystemData->vITIPCDig->vImageToDisplayOnMainMenu)
		if (vLocalSystemData->vITIPCDig->vImageToDisplayOnMainMenu->vHeight) //have a valid image
				TempPixelValuePointer = vLocalSystemData->vITIPCDig->vImageToDisplayOnMainMenu->vGreyImage + 1;
		
		BYTE TempModesToSHow = 0;
		if ((vLocalConfigurationData->vUseXScanDetectors) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))
			TempModesToSHow = 1;
		for (WORD TempDetectorTypeLoop = 0; TempDetectorTypeLoop <= TempModesToSHow; TempDetectorTypeLoop++)
		{
			BYTE TempDisplayPixelsPerDetector = vLocalConfigurationData->vPixelsPerDetector;

			if (vLocalConfigurationData->vUseXScanDetectors)
			if (TempDetectorTypeLoop == 0)
			{
				vTextString = vTextString + "\n\nPixel Corrections when Detectors are in .8mm Pixel mode";
				TempDisplayPixelsPerDetector = 64;
			}
			else
			{
				vTextString = vTextString + "\n\nPixel Corrections when Detectors are in .4mm Pixel mode";
				TempDisplayPixelsPerDetector = 128;
			}

			vTextString = vTextString + "\n\n" + vGlobalBottomString + " Pixel, Detector 1";

			WORD TempNumberOfPixelsToAdjust = vLocalConfigurationData->vNumberOfXRayDetectors * TempDisplayPixelsPerDetector;
			if (TempDetectorTypeLoop == 1)
				TempNumberOfPixelsToAdjust = vLocalConfigurationData->vNumberOfXRayDetectors * 128; //128 is .4mm Detector Mode

			for (WORD TempLoop = 1; TempLoop < TempNumberOfPixelsToAdjust; TempLoop++)
			{
				CString TempBeginningText = "";
				CString TempEndingText = "";
				if ((vLocalConfigurationData->vDetectorEdgeAdjustGain[TempLoop][TempDetectorTypeLoop] < .52) || (vLocalConfigurationData->vDetectorEdgeAdjustGain[TempLoop][TempDetectorTypeLoop] > 2))
				{
					TempBeginningText = "**";
					if (vLocalConfigurationData->vDetectorEdgeAdjustGain[TempLoop][TempDetectorTypeLoop] < .1)
						TempEndingText = "-Gain Not Set";
					else
						TempEndingText = "-Gain At Limit";
				}
				if ((vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempLoop][TempDetectorTypeLoop] <= -50) || (vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempLoop][TempDetectorTypeLoop] >= 50))
				{
					TempBeginningText = "**";
					if (TempEndingText == "")
						TempEndingText = "-Offset At Limit";
					else
						TempEndingText = "-Gain and Offset At Limit";
				}

				//vTextString = vTextString + "\n" + TempBeginningText + "Detector Pixel " + dtoa(TempLoop) + ": ";

				//TempString = dtoa(vLocalConfigurationData->vDetectorEdgeAdjustGain[TempLoop][TempDetectorTypeLoop],2);
				//vTextString = vTextString + "     \tGain: " + TempString;
				//TempString = dtoa(vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempLoop][TempDetectorTypeLoop],2);
				//vTextString = vTextString + "      \tOffset: " + TempString + TempEndingText;

				//if (PasswordOK(cSuperInspxPassword,false))
				//if (TempPixelValuePointer)
				//	vTextString = vTextString + "    \tCur: " + dtoa(*TempPixelValuePointer++, 0);

				CString TempLineString = "Pixel " + Pad(dtoa(TempLoop, 0), 3, 0);

				FillToTab(&TempLineString, 12);
				TempLineString = TempLineString + "Gain: " + Pad(dtoa(vLocalConfigurationData->vDetectorEdgeAdjustGain[TempLoop][TempDetectorTypeLoop],2), 3, 2);

				FillToTab(&TempLineString, 27);
				TempLineString = TempLineString + "Offset: " + Pad(dtoa(vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempLoop][TempDetectorTypeLoop], 2), 4, 2) + TempEndingText;


				if (PasswordOK(cSuperInspxPassword,false))
				if (TempPixelValuePointer)
				{
					FillToTab(&TempLineString, 49);
					TempLineString = TempLineString + "Cur: " + Pad(dtoa(*TempPixelValuePointer++, 0), 3, 0);
				}
				vTextString = vTextString + "\n" + TempBeginningText + TempLineString;

				if (vLocalConfigurationData->vOverWriteBadPixel[TempLoop][TempDetectorTypeLoop])
				if (vLocalConfigurationData->vOverWriteBadPixel[0][TempDetectorTypeLoop] == 0xAAAB)
				if (PasswordOK(cTemporaryInspxPassword,false))
				{
					vTextString = vTextString + "\n*+++++++++COPY PIXEL: " + dtoa(vLocalConfigurationData->vOverWriteBadPixel[TempLoop][TempDetectorTypeLoop], 0) + " ON TOP OF PIXEL: " + dtoa(TempLoop, 0);
				}

				if ((double)(TempLoop) / (double)TempDisplayPixelsPerDetector == (int)((TempLoop) / TempDisplayPixelsPerDetector))
					vTextString = vTextString + "\nDetector " + dtoa((TempLoop / TempDisplayPixelsPerDetector),0) + " Edge -----------";
				if ((double)(TempLoop) / (double)TempDisplayPixelsPerDetector == (int)((TempLoop) / TempDisplayPixelsPerDetector))
					vTextString = vTextString + "\nDetector " + dtoa(1 + (TempLoop / TempDisplayPixelsPerDetector),0) + " Edge -----------";
			}
		}
		vTextString = vTextString + "\n" + vGlobalTopString + " Pixel";

		vTextString = vTextString + "\n";

		if (vLocalConfigurationData->vUseXScanDetectors)
			vTextString = vTextString + "\nX-Scan Detectors";
		else
			vTextString = vTextString + "\nDT Detectors";

		if (vLocalConfigurationData->vPixelsPerDetector == 128)
			vTextString = vTextString + "\n.4mm Detectors, 128 pixels in system";
		else
			vTextString = vTextString + "\n.8mm Detectors, 64 pixels in system";

		if (vLocalSystemData->vCalibratingXScanPoint4mmDetectors == 0)
			vTextString = vTextString + "\nCalibrating default values for .8mm Detectors";
		else
			vTextString = vTextString + "\nCalibrating second set values for .4mm Detectors";

		vTextString = vTextString + "\nPixels Per Inch in Height: " + dtoa(vGlobalPixelsPerUnitInHeight, 0);

		if (vGlobalCurrentProduct)
		if (vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors == 0)
			vTextString = vTextString + "\nProduct set for .8mm Detectors";
		else
			vTextString = vTextString + "\nProduct set for .4mm Detectors";

		vTextString = vTextString + TempDividerString;

		if (vLocalConfigurationData->vDriftCompensationEnabled)
		{
			vTextString = vTextString + "Product Density Compensation Enabled";
			vTextString = vTextString + "\n    Current Density Correction: " + dtoa(vLocalSystemData->vDriftCompensationADCOffset,2) + " mV, " + dtoa(vLocalSystemData->vDriftCompensationDACOffset,2) + "DAC";

			if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
				vTextString = vTextString + "\n    Locked On Product Density";
			else
				vTextString = vTextString + "\n    Not Locked On Product Density";

			if (!vLocalSystemData->vKeepDensityFeedbackLoopLocked)
				vTextString = vTextString + "\n    Temporarily Suspended Product Density Lock";


			//TempString = dtoa(vLocalSystemData->vDriftCompensationThresholdReductionAmount,2);
			//CString TempString1 = "";
			//if (vLocalSystemData->vDriftCompensationThresholdReductionAmount)
			//	TempString1 = "-";
			//vTextString = vTextString + "\n    Current Threshold Correction: " + TempString1 + 
			//	TempString;
			//TempString = dtoa(vLocalSystemData->vDriftCompensationThresholdIncreaseAmount,2);
			//if (vLocalSystemData->vDriftCompensationThresholdIncreaseAmount)
			//	vTextString = vTextString + "\n    Current Threshold Correction: " + TempString;

			TempString = dtoa(vLocalConfigurationData->vDriftCompensationWaitTime,2);
			vTextString = vTextString + "\n    Correction Wait Time: " + TempString + " ms";
			TempString = dtoa(vLocalConfigurationData->vNumberOfImagesForDriftDensity,2);
			vTextString = vTextString + "\n    Correction process Number of Images: " + TempString;
			TempString = dtoa(vLocalConfigurationData->vDriftCompensationSlowThreshold,2);
			vTextString = vTextString + "\n    Correction Slow Threshold: " + TempString;
			TempString = dtoa(vLocalConfigurationData->vDriftCompensationSlowAdjust,2);
			vTextString = vTextString + "\n    Correction Slow Adjust amount: " + TempString;
			TempString = dtoa(vLocalConfigurationData->vDriftCompensationFastThreshold,2);
			vTextString = vTextString + "\n    Correction Fast Threshold: " + TempString;
			TempString = dtoa(vLocalConfigurationData->vDriftCompensationFastAdjust,2);
			vTextString = vTextString + "\n    Correction Fast Adjust amount: " + TempString;
			TempString = dtoa(vLocalConfigurationData->vDriftCompensationVeryFastThreshold,2);
			vTextString = vTextString + "\n    Correction Very Fast Threshold: " + TempString;
			TempString = dtoa(vLocalConfigurationData->vDriftCompensationVeryFastAdjust,2);
			vTextString = vTextString + "\n    Correction Very Fast Adjust amount: " + TempString;
			TempString = dtoa(vLocalConfigurationData->vDriftCompensationMaximumAdjust,2);
			vTextString = vTextString + "\n    Correction Maximum Adjust amount: " + TempString;

			//TempString = dtoa(vLocalConfigurationData->vDriftCompensationThresholdReductionFactor,2);
			//vTextString = vTextString + "\n    Threshold Correction Factor: " + TempString;
			//TempString = dtoa(vLocalConfigurationData->vDriftCompensationThresholdReductionMaximum,2);
			//vTextString = vTextString + "\n    Threshold Correction Maximum: " + TempString;
		}
		else
		{
			vTextString = vTextString + "\nProduct Density Compensation Disabled";
		}

		if (vLocalConfigurationData->vMaxImageIntensityAdjustBeforeLocked)
			vTextString = vTextString + "\nAdjusting Image Intensity before density locked, Maximum Amount: " +
			dtoa(vLocalConfigurationData->vMaxImageIntensityAdjustBeforeLocked,0);
		else
			vTextString = vTextString + "\nNot Adjusting Image Intensity before density locked";

		if (vLocalConfigurationData->vAdjustImageIntensity)
			vTextString = vTextString + "\nAdjusting Image Intensity after density locked, Maximum Amount: " +
			dtoa(vLocalConfigurationData->vMaxImageIntensityAdjust,0);
		else
			vTextString = vTextString + "\nNot Adjusting Image Intensity after density locked";

		if (vLocalConfigurationData->vAdjustAllAreaOfHistogram)
			vTextString = vTextString + "\n    Adjusting All Container Area";
		else
			vTextString = vTextString + "\n    Not Adjusting All Container Area";
		
		if (vLocalConfigurationData->vAutoXRaySourceOffEnabled == 1)
			vTextString = vTextString + "\n    Shut Down DXM on BrownOut (10 seconds)";
		else
		if (vLocalConfigurationData->vAutoXRaySourceOffEnabled == 2)
			vTextString = vTextString + "\n    Don't Shut Down DXM on BrownOut (10 seconds)";
		else
			vTextString = vTextString + "\n    Shut Down DXM on BrownOut (10 seconds) is an Invalid Value";

		if (!((vLocalConfigurationData->vShapeHandlingProcessIfAtSides) || 
			//(vLocalConfigurationData->vShapeHandlingProcessIfAtTop) || 
			(vLocalConfigurationData->vShapeHandlingProcessIfWrongWidth) || 
			(vLocalConfigurationData->vShapeHandlingProcessIfWrongHeight)))
		{
			vTextString = vTextString + "\n\nProduct Shape Handling Disabled";
		}
		else
		{
			vTextString = vTextString + "\n\nProduct Shape Handling Setup";

			if (vLocalConfigurationData->vShapeHandlingProcessIfAtSides == 1)
				vTextString = vTextString + "\n    Eject Slipped Containers";
			else 
			if (vLocalConfigurationData->vShapeHandlingProcessIfAtSides == 2)
			{
				vTextString = vTextString + "\n    Pass Slipped Containers";

				if (vLocalConfigurationData->vSlipsEjectEmptyContainers)  //control inverted from normal logic
					vTextString = vTextString + "\n        Pass Empty Containers";
				else 
					vTextString = vTextString + "\n        Eject Empty Containers";
			}
			else
			{
				vTextString = vTextString + "\n    Slipped Container Checking Disabled";
			}
			if ((vLocalConfigurationData->vProductSlipPercentValue > 0) && (vLocalConfigurationData->vShapeHandlingProcessIfAtSides != 2))
			{
				vTextString = vTextString + "\nSlipped Containers critical warning sent when\n    ";
				vTextString = vTextString + dtoa(vLocalConfigurationData->vProductSlipPercentValue) +  "% slipped containers exceeded.";
			}
			else
			{
				vTextString = vTextString + "\n Slipped Containers Warning Message DISABLED.";
			}
			if (vLocalConfigurationData->vShapeHandlingProcessIfAtSides)
				vTextString = vTextString + "\n    Slipped Container Indicator Time: "  +
				dtoa(vLocalConfigurationData->vSlippedIndicatorTime,0) + " mS";
			/*
			if (vLocalConfigurationData->vShapeHandlingProcessIfAtTop == 1)
				vTextString = vTextString + "\n    Eject if Container Touching Top";
			else 
			if (vLocalConfigurationData->vShapeHandlingProcessIfAtTop == 2)
				vTextString = vTextString + "\n    Pass if Container Touching Top";
			else
			{
				vTextString = vTextString + "\n    Container Touching Top Check Disabled";
			}
			*/
			if (vLocalConfigurationData->vShapeHandlingProcessIfWrongWidth == 1)
				vTextString = vTextString + "\n    Eject if Container Wrong Width";
			else 
			if (vLocalConfigurationData->vShapeHandlingProcessIfWrongWidth == 2)
				vTextString = vTextString + "\n    Pass if Container Wrong Width";
			else
			{
				vTextString = vTextString + "\n    Container Width Checking Disabled";
			}

			if (vLocalConfigurationData->vShapeHandlingWidthTolerancePixels)
				vTextString = vTextString + "\n        Width Tolerance: " +
					dtoa(vLocalConfigurationData->vShapeHandlingWidthTolerance,2) +  " " +
					vLocalConfigurationData->vUnitsString +
					" (" + dtoa(vLocalConfigurationData->vShapeHandlingWidthTolerancePixels) + " pixels)";

			if (vLocalConfigurationData->vShapeHandlingProcessIfWrongHeight == 1)
				vTextString = vTextString + "\n    Eject if Container Wrong Height";
			else 
			if (vLocalConfigurationData->vShapeHandlingProcessIfWrongHeight == 2)
				vTextString = vTextString + "\n    Pass if Container Wrong Height";
			else
			{
				vTextString = vTextString + "\n    Container Height Checking Disabled";
			}

			if (vLocalConfigurationData->vShapeHandlingHeightTolerancePixels)
				vTextString = vTextString + "\n        Height Tolerance: " +
					dtoa(vLocalConfigurationData->vShapeHandlingHeightTolerance,2) + " " +
					vLocalConfigurationData->vUnitsString +
					" (" + dtoa(vLocalConfigurationData->vShapeHandlingHeightTolerancePixels) + " pixels)";

			if (vLocalConfigurationData->vShapeHandlingProcessIfConveyorStops == 1)
				vTextString = vTextString + "\n    Eject if Conveyor Stops";
			else 
			if (vLocalConfigurationData->vShapeHandlingProcessIfConveyorStops == 2)
				vTextString = vTextString + "\n    Pass if Conveyor Stops";
			else
			{
				vTextString = vTextString + "\n    Conveyor Stop Check Disabled";
			}

		}

		vTextString = vTextString + "\n\nSave the images of the last " + 
			dtoa(vLocalConfigurationData->vNumberOfLastRejectImages,0) + " containers rejected for each inspection";
		//9/30/2009
		/*
		vTextString = vTextString + "\nSide Reference Tests: +-" + 
			dtoa(vLocalConfigurationData->vNumberOfJitterPositions,0) + " locations";
		vTextString = vTextString + "\nTop Reference Tests: +-" + 
			dtoa(vLocalConfigurationData->vNumberOfTopJitterPositions,0) + " locations";
			*/

		if (vLocalConfigurationData->vEjectContainersNotInspected)
		{
			vTextString = vTextString + "\n\nEject containers not inspected";
			vTextString = vTextString + "\n    Diverter Ejector:" + 
				dtoa(vLocalConfigurationData->vDiverterEjector,2);
			vTextString = vTextString + "\n    Diverter Margin:" + 
				dtoa(vLocalConfigurationData->vDiverterMargin,2);
		}
		else
			vTextString = vTextString + "\n\nIgnore containers not inspected";

		if (vLocalConfigurationData->vStopInspectionIfConveyorStops)
			vTextString = vTextString + "\nStop Inspection if Conveyor Stops";
		else
			vTextString = vTextString + "\nDon't Stop Inspection if Conveyor Stops";

		if (vLocalConfigurationData->vStopRunningOnUnConfirmedEject)
			vTextString = vTextString + "\nStop Inspection on Unconfirmed Eject";
		else
			vTextString = vTextString + "\nDon't Stop Inspection on Unconfirmed Eject";
		
		/*
		vTextString = vTextString + "\n\nRadiation Shutter Maximum Distance: " + 
			dtoa(vLocalConfigurationData->vRadiationDoorMaximumDistance,0) + " " + 
			vLocalConfigurationData->vUnitsString;
		vTextString = vTextString + "\n     Radiation Shutter Encoder Divider: " + 
			dtoa(vLocalSystemData->vRadiationDoorEncoderDividerFactor,0);
*/
		if (vLocalConfigurationData->vXRaySourceFilter)
			vTextString = vTextString + "\n\n2 mm Aluminum X-Ray Source Filter in ScanTrac";
		else
			vTextString = vTextString + "\n\nNo Aluminum X-Ray Source Filter in ScanTrac";

		if (vLocalConfigurationData->vRadiationShutterInSystem)
		{
			if (vLocalSystemData->vRadiationDoorManualControlRegister)
			{
				if (vLocalSystemData->vRadiationDoorManualControlRegister & 2)
					vTextString = vTextString + "\n\nManual Control of Radiation Shutter, Open";
				else
					vTextString = vTextString + "\n\nManual Control of Radiation Shutter, Closed";
			}
			else
				vTextString = vTextString + "\n\nRadiation Shutter in Auto Mode";

			vTextString = vTextString + "\n    Radiation Shutter Dwell Distance: " + 
				dtoa(vLocalConfigurationData->vRadiationDoorDistanceStayOpen,0) + " " + 
				vLocalConfigurationData->vUnitsString + " (" + 
				dtoa(vLocalSystemData->vRadiationDoorDwellDistance,0) + " Lines)";

			vTextString = vTextString + "\n    Radiation Shutter Delay Distance: " + 
				dtoa(vLocalConfigurationData->vRadiationDoorInDoorDistance,0) + " " + 
				vLocalConfigurationData->vUnitsString + " (" + 
				dtoa(vLocalSystemData->vRadiationDoorInDoorDelaySteps,0) + " Lines)";

			if (vLocalConfigurationData->vRadiationDoorInvertDoorOutput)
				vTextString = vTextString + "\n    Radiation Shutter Output Inverted"; 
			else
				vTextString = vTextString + "\n    Radiation Shutter Output Normal"; 
		}
		else
			vTextString = vTextString + "\n\n" + "No Radiation Shutter In System";
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			if (vLocalConfigurationData->vReCalibrateAfterStopTimePeriod)
			{
				if (vLocalSystemData->vReCalibrationTimerRunning)
					vTextString = vTextString + "\n    Recalibration Timer Running";
				else
					vTextString = vTextString + "\n    Recalibration Timer Not Running";
				vTextString = vTextString + "\n    After gap in production, perform recalibrate in: " +
					dtoa(vLocalConfigurationData->vReCalibrateAfterStopTimePeriod,0) + " Seconds";

				vTextString = vTextString + "\n    " + 
					dtoa(vLocalSystemData->vSecondsSinceLastContainer,0) +
					" Seconds since the last container";
				vTextString = vTextString + "\n    " + 
					dtoa(vLocalSystemData->vContainerCollection->GetCount(),0) +
					" Containers in tunnel";
				vTextString = vTextString + "\n    Time last calibrated:" + 
					vLocalSystemData->vLastCalibrationTime.Format("%H:%M:%S");
				
			}
			else
				vTextString = vTextString + "\n    Recalibrate during production gaps disabled";

		}

		vTextString = vTextString + "\n";
		vTextString = vTextString + TempDividerString;

																											//cNumberOfEMailGroups
		for (BYTE TempLoopSubject = 0;  TempLoopSubject < 4; TempLoopSubject++)
		{
			int TempSubjectIndex = 0;
			switch (TempLoopSubject)
			{
				case 0:
				{
					TempSubjectIndex = 0;
					TempString = "E-Mail for: Plant Operations";
				}
				break;
				case 1:
				{
					if (PasswordOK(cTemporaryInspxPassword,false))
					{
						TempSubjectIndex = 1;
						TempString = "E-Mail for: Inspx";
					}
					else
					{
						TempSubjectIndex = -1;
					}
				}
				break;
				case 2:
				{
					TempSubjectIndex = 2;
					TempString = "E-Mail for: Plant Maintenance";
				}
				break;
				case 3:
				{
					TempSubjectIndex = 3;
					TempString = "E-Mail for: End of Shift Report";
				}
				break;
			}
			if (TempSubjectIndex >= 0)
			{
				if (TempLoopSubject)
					vTextString = vTextString + "\n"; 
				if (vLocalConfigurationData->vEMailEnable[TempLoopSubject])
				{
					vTextString = vTextString + TempString + ":";
					BYTE TempUserIndex = TempSubjectIndex * cMaximumEMailRecievers;

					vTextString = vTextString + "    Subject: " + vLocalConfigurationData->vEMailSubject[TempSubjectIndex];
					for (BYTE TempLoop = 0;  TempLoop < vLocalConfigurationData->vNumberOfEMailRecievers[TempSubjectIndex]; 
						TempLoop++)
					{
						vTextString = vTextString + "\n    Address " + dtoa(TempLoop, 0) + ": " + vLocalConfigurationData->vEMailUser[TempUserIndex + TempLoop] + "@" + vLocalConfigurationData->vEMailDomain[TempUserIndex + TempLoop];
					}
				}
				else
					vTextString = vTextString + TempString + ": Disabled";
			}
		}

		vTextString = vTextString + "\n";
		vTextString = vTextString + TempDividerString;

		vTextString = vTextString + "\n" + dtoa(vLocalSystemData->vContainerCollection->GetCount(),0) + " Containers in tunnel";

		if (vLocalConfigurationData->vCertifiedSetupPassword.GetLength())
			vTextString = vTextString + "\nCertified Setup Password Enabled";
		else
			vTextString = vTextString + "\nCertified Setup Password Disabled";

		if (vLocalConfigurationData->vProductSimpleSetupPassword.GetLength())
			vTextString = vTextString + "\nSimple Setup Password Enabled";
		else
			vTextString = vTextString + "\nSimple Setup Password Disabled";

		if (vLocalConfigurationData->vProductSetupPassword.GetLength())
			vTextString = vTextString + "\nSetup Password Enabled";
		else
			vTextString = vTextString + "\nSetup Password Disabled";

		if (vLocalConfigurationData->vOperatorPassword.GetLength())
			vTextString = vTextString + "\nOperator Password Enabled";
		else
			vTextString = vTextString + "\nOperator Password Disabled";

		if (vLocalConfigurationData->vResetCountsPassword.GetLength())
			vTextString = vTextString + "\nReset Counters/Disable Ejectors Password Enabled";
		else
			vTextString = vTextString + "\nReset Counters/Disable Ejectors Password Disabled";

		if (vLocalSystemData->vContainerCollection->GetCount())
			vTextString = vTextString + "\n" + dtoa(vLocalSystemData->vContainerCollection->GetCount(),0) + " Containers in tunnel";

		if (vLocalSystemData->vRemoveBackgroundInAutoSize)
			vTextString = vTextString + "\nRemoving Background In Autosize in AutoSetup";

		if (vLocalSystemData->vITIPCDig->vUsingLocalSetupDataFile)
			vTextString = vTextString + "\nUsing Local Setup Directory";
		else
			vTextString = vTextString + "\nUsing Default Setup Directory";

		vTextString = vTextString + "\nScanTrac Setup Backup Directory: " + vLocalConfigurationData->vScanTracSetupBackupDirectory;
		CString TempBackupDirectoryString1("C:\\ScanTracSetupBackup");
		TempBackupDirectoryString1 = TempBackupDirectoryString1 + dtoa(vLocalConfigurationData->vLastBackupDirectoryUsed, 0) + "\\";
		vTextString = vTextString + "\n" + "Last Backup Directory Used: " + TempBackupDirectoryString1;

		if (vGlobalLanguage == cFrench)
			vTextString = vTextString + "\n" + "French Language: " + vLocalSystemData->vRegionalLocale;
		else
		if (vGlobalLanguage == cGerman)
			vTextString = vTextString + "\n" + "German Language: " + vLocalSystemData->vRegionalLocale;
		else
		if (vGlobalLanguage == cSpanish)
			vTextString = vTextString + "\n" + "Spanish Language: " + vLocalSystemData->vRegionalLocale;
		else
		if (vGlobalLanguage == cPolish)
			vTextString = vTextString + "\n" + "Polish Language: " + vLocalSystemData->vRegionalLocale;
		else
		if (vGlobalLanguage == cSimplifiedChinese)
			vTextString = vTextString + "\n" + "Simplified Chinese Language: " + vLocalSystemData->vRegionalLocale;
		else
			vTextString = vTextString + "\n" + "English Language: " + vLocalSystemData->vRegionalLocale;

		vTextString = vTextString + "\n";

		double TempDriveSpaceFree = CheckDiskSpaceFree("C:\\");
		vTextString = vTextString + "\nHard disk space available (bytes):";
		if (vGlobalHardDriveTotalBytes)
			vTextString = vTextString + "\nC: " + dtoaWithCommas(TempDriveSpaceFree,0) + " Drive Size: " + dtoaWithCommas(vGlobalHardDriveTotalBytes,0);
		else
			vTextString = vTextString + "\nC drive not installed";

		TempDriveSpaceFree = CheckDiskSpaceFree("D:\\");
		if (vGlobalHardDriveTotalBytes)
			vTextString = vTextString + "\nD: " + dtoaWithCommas(TempDriveSpaceFree,0) + " Drive Size: " + dtoaWithCommas(vGlobalHardDriveTotalBytes,0);
		else
			vTextString = vTextString + "\nD drive not installed";

		TempDriveSpaceFree = CheckDiskSpaceFree("E:\\");
		if (vGlobalHardDriveTotalBytes)
			vTextString = vTextString + "\nE: " + dtoaWithCommas(TempDriveSpaceFree,0) + " Drive Size: " + dtoaWithCommas(vGlobalHardDriveTotalBytes,0);

		TempDriveSpaceFree = CheckDiskSpaceFree("F:\\");
		if (vGlobalHardDriveTotalBytes)
			vTextString = vTextString + "\nF: " + dtoaWithCommas(TempDriveSpaceFree,0) + " Drive Size: " + dtoaWithCommas(vGlobalHardDriveTotalBytes,0);

		TempDriveSpaceFree = CheckDiskSpaceFree("G:\\");
		if (vGlobalHardDriveTotalBytes)
			vTextString = vTextString + "\nG: " + dtoaWithCommas(TempDriveSpaceFree,0) + " Drive Size: " + dtoaWithCommas(vGlobalHardDriveTotalBytes,0);

		TempDriveSpaceFree = CheckDiskSpaceFree("H:\\");
		if (vGlobalHardDriveTotalBytes)
			vTextString = vTextString + "\nH: " + dtoaWithCommas(TempDriveSpaceFree,0) + " Drive Size: " + dtoaWithCommas(vGlobalHardDriveTotalBytes,0);

		TempDriveSpaceFree = CheckDiskSpaceFree("I:\\");
		if (vGlobalHardDriveTotalBytes)
			vTextString = vTextString + "\nI: " + dtoaWithCommas(TempDriveSpaceFree,0) + " Drive Size: " + dtoaWithCommas(vGlobalHardDriveTotalBytes,0);

		TempDriveSpaceFree = CheckDiskSpaceFree("M:\\");
		if (vGlobalHardDriveTotalBytes)
			vTextString = vTextString + "\nM: " + dtoaWithCommas(TempDriveSpaceFree,0) + " Drive Size: " + dtoaWithCommas(vGlobalHardDriveTotalBytes,0);

		TempDriveSpaceFree = CheckDiskSpaceFree("N:\\");
		if (vGlobalHardDriveTotalBytes)
			vTextString = vTextString + "\nN: " + dtoaWithCommas(TempDriveSpaceFree,0) + " Drive Size: " + dtoaWithCommas(vGlobalHardDriveTotalBytes,0);

		TempDriveSpaceFree = CheckDiskSpaceFree("O:\\");
		if (vGlobalHardDriveTotalBytes)
			vTextString = vTextString + "\nO: " + dtoaWithCommas(TempDriveSpaceFree,0) + " Drive Size: " + dtoaWithCommas(vGlobalHardDriveTotalBytes,0);

		TempDriveSpaceFree = CheckDiskSpaceFree("P:\\");
		if (vGlobalHardDriveTotalBytes)
			vTextString = vTextString + "\nP: " + dtoaWithCommas(TempDriveSpaceFree,0) + " Drive Size: " + dtoaWithCommas(vGlobalHardDriveTotalBytes,0);

		TempDriveSpaceFree = CheckDiskSpaceFree("Q:\\");
		if (vGlobalHardDriveTotalBytes)
			vTextString = vTextString + "\nQ: " + dtoaWithCommas(TempDriveSpaceFree,0) + " Drive Size: " + dtoaWithCommas(vGlobalHardDriveTotalBytes,0);

		TempDriveSpaceFree = CheckDiskSpaceFree("R:\\");
		if (vGlobalHardDriveTotalBytes)
			vTextString = vTextString + "\nR: " + dtoaWithCommas(TempDriveSpaceFree,0) + " Drive Size: " + dtoaWithCommas(vGlobalHardDriveTotalBytes,0);

		TempDriveSpaceFree = CheckDiskSpaceFree("S:\\");
		if (vGlobalHardDriveTotalBytes)
			vTextString = vTextString + "\nS: " + dtoaWithCommas(TempDriveSpaceFree,0) + " Drive Size: " + dtoaWithCommas(vGlobalHardDriveTotalBytes,0);

		TempDriveSpaceFree = CheckDiskSpaceFree("C:\\");//leave global value on C drive

		if (vGlobalBackupHardDriveThread)
			vTextString = vTextString + "\nCurrently Backing Up to Backup Hard Disk";

		vTextString = vTextString + "\nMaximum filename and path length:" + dtoa(MAX_PATH, 0);
		
		if (vGlobalProcessQuickLearnImagesThread)
			vTextString = vTextString + "\nCurrently Processing Quick Learn";
		vTextString = vTextString + "\n";

		if (vLocalSystemData->vPrimaryDrive.GetLength())
			vTextString = vTextString + "\nPrimary Hard Drive: " + vLocalSystemData->vPrimaryDrive;
		else
			vTextString = vTextString + "\nPrimary Hard Drive: Not Found";
		if (vLocalSystemData->vBackupDrive.GetLength())
			vTextString = vTextString + "\nBackup Hard Drive: " + vLocalSystemData->vBackupDrive;
		else
			vTextString = vTextString + "\nBackup Hard Drive: Not Found";

		vTextString = vTextString + "\n";
		if (vLocalSystemData->vACPowerOn)
			vTextString = vTextString + "\nA/C Power On";
		else
			vTextString = vTextString + "\nA/C Power Off";
		if (vLocalSystemData->vShuttingDownXRays)
			vTextString = vTextString + "\nShut Down X-Rays";
		else
			vTextString = vTextString + "\nNot Shut Down X-Rays";

#ifdef IFC59Driver
		char TempStringVersion[100];
		IfxGetVersion(TempStringVersion);
			vTextString = vTextString + "\nIFC Version: " + TempStringVersion;
#endif
			/*

	typedef struct _OSVERSIONINFOEX {
			DWORD dwOSVersionInfoSize;
			DWORD dwMajorVersion;
			DWORD dwMinorVersion;
			DWORD dwBuildNumber;
			DWORD dwPlatformId;
			TCHAR szCSDVersion[128];
			WORD  wServicePackMajor;
			WORD  wServicePackMinor;
			WORD  wSuiteMask;
			BYTE  wProductType;
			BYTE  wReserved;
		} OSVERSIONINFOEX, *POSVERSIONINFOEX, *LPOSVERSIONINFOEX;

		*/
		

		OSVERSIONINFO TempOSInfo;
		TempOSInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (GetVersionEx(&TempOSInfo))
		{
			vTextString = vTextString + "\nOperating System Major Version: " + dtoa(TempOSInfo.dwMajorVersion,0);
			vTextString = vTextString + "\nOperating System Minor Version: " + dtoa(TempOSInfo.dwMinorVersion,0);
			vTextString = vTextString + "\nOperating System Build Number: " + dtoa(TempOSInfo.dwBuildNumber,0);
		}

		vTextString = vTextString + "\nMotherboard BIOS Version: " + vLocalSystemData->vBIOSVersion;
		vTextString = vTextString + "\nMotherboard BIOS Date: " + vLocalSystemData->vBIOSDate;
		vTextString = vTextString + "\nVideo BIOS Version: " + vLocalSystemData->vVideoBIOSVersion;
		vTextString = vTextString + "\nVideo BIOS Date: " + vLocalSystemData->vVideoBIOSDate;

		int TempPosition2 = vLocalSystemData->vBIOSVersion.Find(_T("P4-865G"));
		if (TempPosition2 >= 0)
		{
			vTextString = vTextString + "\nDetected Seavo Motherboard\n";
		}
		TempPosition2 = vLocalSystemData->vBIOSVersion.Find(_T("GBT"));
		if (TempPosition2 >= 0)
		{
			vTextString = vTextString + "\nDetected GigaByte Motherboard\n";
		}
		TempPosition2 = vLocalSystemData->vBIOSVersion.Find(_T("Supermicro"));
		if (TempPosition2 >= 0)
		{
			vTextString = vTextString + "\nDetected SuperMicro Motherboard\n";
		}

		vTextString = vTextString + "\n_MSC_VER: " + dtoa(_MSC_VER,0);
		
		if (vGlobalPCRunningWindowsXP == cWindowsNT)
			vTextString = vTextString + "\nDetected Windows NT Embedded";
		else
		if (vGlobalPCRunningWindowsXP == cWindowsXP)
			vTextString = vTextString + "\nDetected Windows XP Embedded";
		else
		if (vGlobalPCRunningWindowsXP == cWindows7)
			vTextString = vTextString + "\nDetected Windows 7 Embedded";
		else
			vTextString = vTextString + "\nUnknown Operating System";

/*		HMONITOR hMonitor;
		POINT pt;
		DWORD dwFlags;
		MONITORINFO MonitorInfo;
		LPMONITORINFO lpmi;
		MonitorInfo.cbSize = sizeof(MONITORINFO);
		lpmi = &MonitorInfo;
		pt.x = 125l;
		pt.y = 125l;
		dwFlags = MONITOR_DEFAULTTOPRIMARY;
		hMonitor = MonitorFromPoint(pt,dwFlags);
		if (GetMonitorInfo(hMonitor,lpmi))
		{
			TempString = "Monitor Size, Horizontal: " + dtoa(lpmi->rcMonitor.right) + " Vertical: "+ dtoa(lpmi->rcMonitor.bottom) ;
			vTextString = vTextString + "\n" + TempString;
		}
*/
		TempString.Format( _T("Monitor Size, Horizontal: %d Vertical: %d"),vGlobalDisplaySizeH,vGlobalDisplaySizeV) ;
		vTextString = vTextString + "\n" + TempString;

		WORD TempCPUCount = 0;
		for (BYTE TempLoop = 0; TempLoop < 32; TempLoop++)
		{
			DWORD TempTryMask = 1;
			if (vGlobalSystemAffinityMask & (TempTryMask << TempLoop))
				TempCPUCount++;
		}
		vTextString = vTextString + "\nNumber of CPU Cores: " + dtoa(TempCPUCount,0);
		vTextString = vTextString + "\nCPU Processor Bit Map: " + ByteToHex((BYTE)(vGlobalSystemAffinityMask));

		MEMORYSTATUS memstat;
		GlobalMemoryStatus(&memstat);
		double TempAmountOfFreeMemory = memstat.dwAvailPageFile;
		double TempTotalRAMInPC = memstat.dwTotalPageFile;
		vTextString = vTextString + "\n\nAmount of RAM in PC: " + dtoaWithCommas(TempTotalRAMInPC, 0);
		vTextString = vTextString + "\nAmount of Free RAM: " + dtoaWithCommas(TempAmountOfFreeMemory,0);
		vTextString = vTextString + "\n\t\tRAM Length: " + dtoaWithCommas(memstat.dwLength, 0);
		vTextString = vTextString + "\n\t\tRAM %Used: " + dtoaWithCommas(memstat.dwMemoryLoad, 0);
		vTextString = vTextString + "\n\t\tRAM Page File Available: " + dtoaWithCommas(memstat.dwAvailPageFile, 0);
		vTextString = vTextString + "\n\t\tRAM Physical Available: " + dtoaWithCommas(memstat.dwAvailPhys, 0);
		vTextString = vTextString + "\n\t\tRAM Virtual Available: " + dtoaWithCommas(memstat.dwAvailVirtual, 0);
		vTextString = vTextString + "\n\t\tRAM Page File Total: " + dtoaWithCommas(memstat.dwTotalPageFile, 0);
		vTextString = vTextString + "\n\t\tRAM Physical Total: " + dtoaWithCommas(memstat.dwTotalPhys, 0);
		vTextString = vTextString + "\n\t\tRAM Virtual Total: " + dtoaWithCommas(memstat.dwTotalVirtual, 0);

		vTextString = vTextString + "\n";

		if (vLocalConfigurationData->vDetectedValidGuardMasterDevice)
			vTextString = vTextString + "\nUsing Guard Master Interlock Device";
		else
			vTextString = vTextString + "\nUsing uController Board for Interlock Monitoring";

		if (!vGlobalCurrentProduct)
			vTextString = vTextString + "\nNO CURRENT PRODUCT";
		else
		if (!vLocalSystemData->vITIPCDig->vCaptureModule)
		{
			ReportErrorMessage("NO FRAME GRABBER DETECTED (PCDig or PC2-CamLink", cEMailInspx,32000);
			vTextString = vTextString + "\nNO FRAME GRABBER DETECTED (PCDig or PC2-CamLink)";
		}
		else
		{
			/*
			CAM_ATTR TempCameraAttributes;
			vLocalSystemData->vITIPCDig->vCamera->GetAttr(&TempCameraAttributes, false);
			CString TempCameraName = TempCameraAttributes.camName;
			int TempPosition = TempCameraName.Find("CL2");
			if (TempPosition < 0)
				TempPosition = TempCameraName.Find("CamLink");
			if (TempPosition >= 0) //have a PC2-CamLink Frame Grabber
			*/
			if (vGlobalFrameGrabberType == cPC2CamLinkFrameGrabber)
				vTextString = vTextString + "\nPC2-CamLink Frame Grabber detected";
			else
			{
				//TempPosition = TempCameraName.Find("PCDig");
				//if (TempPosition >= 0) //have a PC2-CamLink Frame Grabber
				if (vGlobalFrameGrabberType == cPCDigFrameGrabber)
					vTextString = vTextString + "\nPC-Dig Frame Grabber detected";
				else
				if (vGlobalFrameGrabberType == cNoFrameGrabber)
					vTextString = vTextString + "\nNO Frame Grabber detected";
				else
					vTextString = vTextString + "\nUnknown Frame Grabber detected";
			}
		}

		if (vLocalConfigurationData->vEnableModBusTCPIPInterface)
			vTextString = vTextString + "\nModBus Interface to Kepware Server Enabled";
		else
			vTextString = vTextString + "\nModBus Interface to Kepware Server Disabled";

		//if (vGlobalUseHalconLibrary)
		//{
		//		vTextString = vTextString + "\nUsing Halcon Library";
		//		//tuple TempHalconCheckInfo;
		//		//get_check ( : : : TempHalconCheckInfo );
		//}
		if (vGlobalFPGAVersion10Point0OrHigher)
				vTextString = vTextString + "\nFPGA Version >= 10";

		if (vGlobalFPGAVersion16point0orAbove)
				vTextString = vTextString + "\nFPGA Version >= 16";

		if (vGlobalFPGAVersion18point0orAbove)
				vTextString = vTextString + "\nFPGA Version >= 18";


		//if (vLocalConfigurationData->vWriteErrorLogOnDisk)
		//	vTextString = vTextString + "\n" + "Write events to disk";
		//else
		//	vTextString = vTextString + "\n" + "Don't Write events to disk";

		vTextString = vTextString + "\n" + "Current data storage format: " + dtoa(cCurrentArchiveVersionNumber, 0);
		vTextString = vTextString + "\n" + "Read data storage format: " + dtoa(vGlobalArchiveVersionNumber,0);
	
		vTextString = vTextString + "\n" + dtoa(vLocalProductCollection->GetSize(),0) + " Products configured";

		if (vLocalSystemData->vSourceRampedUp)
			vTextString = vTextString + "\n" + "Source Ramped Up";
		else
			vTextString = vTextString + "\n" + "Source Not Ramped Up";

		if (vLocalSystemData->vInAutoSetup)
			vTextString = vTextString + "\nIn Auto Setup";

		if (vLocalSystemData->vOutstandingEjects)
		{
			vTextString = vTextString + "\n" + dtoa(vLocalSystemData->vOutstandingEjects, 0) + " outstanding ejects";
		}
		if (vLocalSystemData->vOutstandingAuxiliaryDetectorEnables)
		{
			vTextString = vTextString + "\n" + dtoa(vLocalSystemData->vOutstandingAuxiliaryDetectorEnables, 0) + " outstanding Auxiliary Detector enables";
		}

		if (vLocalSystemData->vACPowerOn)
			vTextString = vTextString + "\nA/C Power On";
		else
			vTextString = vTextString + "\nA/C Power Off";
		if (vLocalSystemData->vShuttingDownXRays)
			vTextString = vTextString + "\nShut Down X-Rays";
		else
			vTextString = vTextString + "\nNot Shut Down X-Rays";
		//if (vLocalSystemData->vInterlockOpened)
		//	vTextString = vTextString + "\n" + "Interlock Opened";
		//else
		//	vTextString = vTextString + "\n" + "Interlock Closed";

		if (vLocalSystemData->vAdjustingSourceSettings)
			vTextString = vTextString + "\n" + "Adjusting Source Settings";
		else
			vTextString = vTextString + "\n" + "Source Settings Stable";

		if (vLocalSystemData->vLoadedFileAvailable)
			vTextString = vTextString + "\n" + "Loaded File Image to Show";

		if (vLocalSystemData->vInAutoSetup)
			vTextString = vTextString + "\nIn Auto Setup";

		if (vGlobalInAutoSize)
			vTextString = vTextString + "\nIn Auto Size Menu";
		
		//if (vGlobalInAutoExposure)
		//	vTextString = vTextString + "\nIn Auto Exposure Menu";

		if (vLocalSystemData->vOutstandingEjects)
			vTextString = vTextString + "\n" + dtoa(vLocalSystemData->vOutstandingEjects, 0) + " outstanding ejects";

		if (vLocalSystemData->vOutstandingAuxiliaryDetectorEnables)
			vTextString = vTextString + "\n" + dtoa(vLocalSystemData->vOutstandingAuxiliaryDetectorEnables, 0) + " outstanding Auxiliary Detector enables";

		vTextString = vTextString + "\nInterlaced: " + dtoa(vLocalSystemData->vITIPCDig->vInterlaced, 0);
		vTextString = vTextString + "\nBytes per Pixel: " + dtoa(vLocalSystemData->vITIPCDig->vBytesPerPixel, 0);
		vTextString = vTextString + "\nO Size X: " + dtoa(vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, 0);
		vTextString = vTextString + "\nA Size X: " + dtoa(vLocalSystemData->vITIPCDig->vAquisitionSizeX, 0);
		vTextString = vTextString + "\nA Size X 12 D: " + dtoa(vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors, 0);
		
		vTextString = vTextString + "\nO Size Y: " + dtoa(vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 0);
		vTextString = vTextString + "\nA Size Y: " + dtoa(vLocalSystemData->vITIPCDig->vAquisitionSizeY, 0);

		vTextString = vTextString + "\n# of Detectors: " + dtoa(vLocalConfigurationData->vNumberOfXRayDetectors, 0);
		vTextString = vTextString + "\n# of Detectors Using: " + dtoa(vLocalSystemData->vNumberOfXRayDetectorsUsing, 0);

		if (vLocalSystemData->vABPixelAtOtherSide)
			vTextString = vTextString + "\nA/B Pixel at Other Side of image";
		else
			vTextString = vTextString + "\nA/B Pixel at Normal end of image";

		if (vLocalConfigurationData->vScanTracType == cUpSideDownSoloScanTrac)
			vTextString = vTextString + "\nScanTrac is an Inverted Solo (600 Watt SideView)";
		else
		if (vLocalConfigurationData->vScanTracType == cSoloScanTrac)
			vTextString = vTextString + "\nScanTrac is a Solo (600 Watt SideView)";
		else
		if (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)
			vTextString = vTextString + "\nScanTrac is an 18 inch tall Solo (600 Watt SideView)";
		else
		if (vLocalConfigurationData->vScanTracType == cFermataScanTrac)
			vTextString = vTextString + "\nScanTrac is a 600 Watt Pipeline";
		else
		if (vLocalConfigurationData->vScanTracType == cLegatoScanTrac)
			vTextString = vTextString + "\nScanTrac is a Legato (50 Watt Pipeline)";
		else
		if (vLocalConfigurationData->vScanTracType == cPiccoloScanTrac)
			vTextString = vTextString + "\nScanTrac is a Piccolo (50 Watt SideView)";
		else
		if (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)
			vTextString = vTextString + "\nScanTrac is a Brioso (100 Watt Web)";
		else
		if (vLocalConfigurationData->vScanTracType == cAtempoScanTrac)
			vTextString = vTextString + "\nScanTrac is an Atempo (210 Watt 12\" TD)";
		else
		if (vLocalConfigurationData->vScanTracType == cForteScanTrac)
			vTextString = vTextString + "\nScanTrac is an Forte (210 Watt 24\" TD)";
		else
		if (vLocalConfigurationData->vScanTracType == cAllegroScanTrac)
			vTextString = vTextString + "\nScanTrac is an Allegro (100 Watt 18\" TD)";
		else
		if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
			vTextString = vTextString + "\nScanTrac is a Largo (210 Watt 36\" TD)";
		else
			vTextString = vTextString + "\n*ScanTrac Type INVALID";

		if (vLocalConfigurationData->vHVPSType == cDXMHVPS)
				vTextString = vTextString + "\nX-Ray Source is an H3 Tube with a Spellman DXM HVPS";
		else
		if (vLocalConfigurationData->vHVPSType == cMNXHVPS)
				vTextString = vTextString + "\nX-Ray Source is an Varian Tube with a Spellman MNX HVPS";
		else
		
		if (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock)
			vTextString = vTextString + "\nX-Ray Source is a Cheyney MonoBlock";
		else
		if (vLocalConfigurationData->vHVPSType == c210WMonoBlockHVPS)
			vTextString = vTextString + "\nX-Ray Source is a Spellman XRB 210 Watt MonoBlock";
		else
		if (vLocalConfigurationData->vHVPSType == c100WMonoBlockHVPS)
				vTextString = vTextString + "\nX-Ray Source is a Spellman XRB 100 Watt MonoBlock";
		else
		if (vLocalConfigurationData->vHVPSType == c100WVJTMonoBlockHVPS)
				vTextString = vTextString + "\nX-Ray Source is a VJT 100 Watt MonoBlock";
		else
				vTextString = vTextString + "\n*X-Ray Source type is INVALID";
		
		if (((vLocalConfigurationData->vScanTracType == cFermataScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac)) || (vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode))
		{
			vTextString = vTextString + "\n";
			switch (vLocalSystemData->vCurrentPipeStatus)
			{
				case cNoPipeStatus: vTextString = vTextString + "\nPipeline Status - Not Inspecting"; break;
				case cPipeEmpty: vTextString = vTextString + "\nPipeline Status - Empty"; break;
				case cPipePumping: vTextString = vTextString + "\nPipeline Status - Pumping"; break;
				case cPipePumpStopped: vTextString = vTextString + "\nPipeline Status - Stopped"; break;
			}
			if (vLocalSystemData->vAirBubblesInPipe)
				vTextString = vTextString + "\nAir In Pipe: " + dtoa(vLocalSystemData->vAirBubblesInPipe, 0);
			else
				vTextString = vTextString + "\nNo Air In Pipe";

			CString TempString = " In Progress";
			if (vLocalSystemData->vPipeBrightnessCount >= 200)
				TempString = " Final";
			vTextString = vTextString + "\nDensity Correction: " + dtoa(vLocalSystemData->vSmallestDifferenceBetweenAverageAndMinimum, 1) + TempString;
		}

#ifdef ScanTracDemo
		vTextString = vTextString + "\n" + "Compiled for Demo Mode Only";
#endif
			//show IP Address Settings values

		//IP
		//9/11/2009
		//if (vMainWindowPointer)
			//vMainWindowPointer->GetCurrentMachineIPAddressValuesFromRegistry();

		vTextString = vTextString + "\n\nMother Board Network Type: " + vGlobalNetworkBoardCode
			+ "\nRegistry Path: " + vGlobalRegistryKeyPath;

		vTextString = vTextString + "\n\nWindows IP Settings"
			+ "\n    " + vGlobalDHCPString[vGlobalIPDHCPEnabled] 
			+ "\n    DHCP IP Address: " + vGlobalIPAddressString
			+ "\n    DHCP SubNet Mask: " + vGlobalIPSubNetMaskString
			+ "\n    DHCP Default Gateway: " + vGlobalIPDefaultGatewayString
			+ "\n    DNS Servers: " + vGlobalIPDNSAddressString1;

		if ((vGlobalIPDHCPEnabled == vLocalConfigurationData->vIPDHCPEnabled) && (vGlobalIPAddressString == vLocalConfigurationData->vIPAddressString) && 
			(vGlobalIPSubNetMaskString == vLocalConfigurationData->vIPSubNetMaskString) && (vGlobalIPDefaultGatewayString == vLocalConfigurationData->vIPDefaultGatewayString))
			vTextString = vTextString + "\n\n    ScanTrac IP Settings are the same as Windows";
		else
			vTextString = vTextString + "\n\nScanTrac IP Settings:\n    " + vGlobalDHCPString[vLocalConfigurationData->vIPDHCPEnabled] + "\n    IP Address: " + vLocalConfigurationData->vIPAddressString
				+ "\n    SubNet Mask: " + vLocalConfigurationData->vIPSubNetMaskString + "\n    Default Gateway: " + vLocalConfigurationData->vIPDefaultGatewayString +
				"\n    DNS Servers: " + vLocalConfigurationData->vIPDefaultDNSAddressString;

		vTextString = vTextString + "\n\n";
			
//end showing ip settings

		if (vLocalSystemData->vPipeIsEmpty)
			vTextString = vTextString + "\n\nPipe Is Empty: " + dtoa(vLocalSystemData->vPipeIsEmpty,0);

#ifdef CompileInHTTPClientOperationsGuardian
		if (vLocalConfigurationData->vEnableOperationsGuardian)
		{
				vTextString = vTextString + "\n\nOperations Guardian: ";
#ifdef _DEBUG
				vTextString = vTextString + "\nCompiled for Debug Mode. Can not send message to OG Server. You need a release version of ScanTrac";
#endif
				vTextString = vTextString + "\n     Messages Sent: " + dtoa(vGlobalOGMessageSendCounter,0);
				vTextString = vTextString + "\n     Message Errors: " + dtoa(vGlobalOGSendMessageErrorCount,0);
				vTextString = vTextString + "\n     Message Exceptions: " + dtoa(vGlobalOGSendMessageexceptionCount,0);
				vTextString = vTextString + "\n     Message Good Replies: " + dtoa(vGlobalOGSendMessageGoodCount,0);
				vTextString = vTextString + "\n     Last Message Code (200 and 201 = OK): " + dtoa(vGlobalLastOGServerWriteStatusCode);
				if (vGlobalLastOGServerErrorCode >= 0)
				{
					CString TempURL(vGlobalOGLastErrorKeyword.c_str());
					CString TempMessage(vGlobalOGLastErrorMessage.c_str());

					vTextString = vTextString + "\n          Last Error Code: " + dtoa(vGlobalLastOGServerErrorCode);
					if (vGlobalLastOGServerErrorCode == 404)
						vTextString = vTextString + "-Maybe ScanTrac Serial Number not in OG Database";
					if (vGlobalLastOGServerErrorCode == 1)
						vTextString = vTextString + "-Data Invalid";

					vTextString = vTextString + "\n          Last URL that got error: " + TempURL;
					vTextString = vTextString + "\n          Last Message that got error: " + TempMessage;

					#ifdef _DEBUG
					if (vGlobalLastOGServerErrorCode == 0)
						vTextString = vTextString + "\n          Operations Guardian Transmission Diabled; Compiled for Debug";
					#endif
				}
		}
#endif

		if (vLocalSystemData->vEjectConfirmerUsed)
			vTextString = vTextString + "\nEjector confirmation Used";
		for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
		if (vGlobalEjectorConfirmMask[TempLoop])
			vTextString = vTextString + "\nEjector " + dtoa(TempLoop + 1, 0) + " Confirmer mask: " + dtoa(vGlobalEjectorConfirmMask[TempLoop], 0);

		SetDlgItemText(IDC_HelpText,vTextString);
		if (PasswordOK(cTemporaryInspxPassword,false))
			WriteStringToFile("SystemSummary", vTextString);

		CHARFORMAT TempCharacterFormat;
		TempCharacterFormat.dwMask = CFM_COLOR ;
		TempCharacterFormat.dwEffects = NULL;
		TempCharacterFormat.crTextColor = cBlack;
		m_HelpTextControl.SetDefaultCharFormat(TempCharacterFormat);


		//m_HelpTextControl.SetBackgroundColor(false,cVeryLightBlue);

		//m_HelpTextControl.SetBackgroundColor(false,cYellow);
		m_HelpTextControl.HideSelection(true,false);


		CHARRANGE TempFormatCharacterRange;
		TempFormatCharacterRange.cpMin = 0;
		TempFormatCharacterRange.cpMax = TempTestModesLength;
		m_HelpTextControl.SetSel(TempFormatCharacterRange);

		//CHARFORMAT TempCharacterFormat;
		TempCharacterFormat.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD;
		TempCharacterFormat.dwEffects = CFE_UNDERLINE;
		TempCharacterFormat.crTextColor = cRed;
		m_HelpTextControl.SetSelectionCharFormat(TempCharacterFormat);
		//m_HelpTextControl.SetSel(TempFormatCharacterRange);

		//show lines in color
		int TempEndCount = vTextString.GetLength();
		if (TempEndCount > cSizeOfVColor)
			TempEndCount = cSizeOfVColor;
		for (int TempPosition = 0; TempPosition < TempEndCount - 5; TempPosition++)
		{
			wchar_t TempChar = vTextString.GetAt(TempPosition);
			wchar_t TempNextChar = vTextString.GetAt(TempPosition + 1);
			if ((TempChar == '*') || (TempPosition < TempTestModesLength))
			{
				int TempStop = vTextString.Find((char)0x0A,TempPosition);
				if (TempStop > TempEndCount)
					TempStop = TempEndCount;
				for (int TempOffset = TempPosition; TempOffset <= TempStop; TempOffset++)
				if (TempOffset < cSizeOfVColor)
					vColor[TempOffset] = 1; //red
				//vColor[TempOffset] = 3; //green
				//vColor[TempOffset] = 2; //blue
				TempPosition = TempStop;
			}
			//else
			//if ((TempChar == '+') || ((TempChar == '-') && (TempNextChar == '-')))
			//{
			//	int TempStop = vTextString.Find((char)0x0A,TempPosition);
			//	if (TempStop > TempEndCount)
			//		TempStop = TempEndCount;
			//	for (int TempOffset = TempPosition; TempOffset <= TempStop; TempOffset++)
			//	if (TempOffset < cSizeOfVColor)
			//		vColor[TempOffset] = 3; //green
			//		//vColor[TempOffset] = 2; //blue
			//		//vColor[TempOffset] = 1; //red
			//	TempPosition = TempStop;
			//}
		}

		SetDlgItemText(IDC_HelpText,vTextString);
		DisplayColor(&vTextString);

/*
		TempFormatCharacterRange.cpMin = TempTestModesLength + 20;
		TempFormatCharacterRange.cpMax = TempTestModesLength + 100;
		m_HelpTextControl.SetSel(TempFormatCharacterRange);

		//CHARFORMAT TempCharacterFormat;
		TempCharacterFormat.dwMask = CFM_COLOR | CFM_UNDERLINE | CFM_BOLD;
		TempCharacterFormat.dwEffects = CFE_UNDERLINE;
		TempCharacterFormat.crTextColor = cBlue;
		m_HelpTextControl.SetSelectionCharFormat(TempCharacterFormat);
		m_HelpTextControl.SetSel(TempFormatCharacterRange);
*/
	}//end of system setup summary
	NewPage();
}

void CHelpDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	// Do not call CDialog::OnPaint() for painting messages
}

void CHelpDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(vACPowerMoniterFormat && (m_SubFunction2Button.IsWindowVisible() == false))
	{
		CFileStatus TempStatus;
		if(CFile::GetStatus( vACPowerMoniterErrorLogFile, TempStatus ))
		if(TempStatus.m_size != vOldACLogFileSize)
		{
			m_SubFunction2Button.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_SubFunction2Button, _T("Update Event Display"));
		}
	}
	else
	if (nIDEvent == vProcessingMessageTimerHandle)
	{
		vLinesInCurrentDayEventLog = NumberOfLinesInAllFilesOfDay();
		ShowEndOfEventLog();

		int TempTotalLines = m_HelpTextControl.GetLineCount();
		int TempCurrentLine = m_HelpTextControl.GetFirstVisibleLine();

		m_HelpTextControl.LineScroll(TempTotalLines - (TempCurrentLine + 1 + vLinesOnScreen));
				
		UpdateButtons();
		
		m_SubFunction7Button.ShowNavigationArrow(NULL);
		m_SubFunction8Button.ShowNavigationArrow(NULL);
		m_SubFunction7Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
		m_SubFunction8Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());

		KillTimer(vProcessingMessageTimerHandle);
	}
	else
	if (nIDEvent == vOneSecondTimerHandle)
	{
		OneSecondUpdate();
	}
//	else	
//	if (nIDEvent == vHideCaretTimerHandle)
//  {
//		m_HelpTextControl.HideCaret();
//		KillTimer(vHideCaretTimerHandle);
//	}
	else
	if (nIDEvent == vUpdateInspxLogDisplay)
  {
		KillTimer(vUpdateInspxLogDisplay);
		ShowErrorLog(true); //do refresh Inspx Log Display
	}
	else
	if (nIDEvent == vCheckErrorMessagesTimerHandle)
	{
		if (vOldErrorLogCount != vLocalSystemData->vErrorCount)
		if (vErrorMessageDisplayMask != cInspxLogMask)
		{
			vOldErrorLogCount = vLocalSystemData->vErrorCount;
			if (vShowUpdateButton)
			{
				m_SubFunction2Button.ShowWindow(SW_SHOW);
				SetDlgItemText(IDC_SubFunction2Button, _T("Update Event Display"));
			}
		}
	}
	else
	if (nIDEvent == vMouseDownRepeatTimerHandle)
	{
		
		switch (vMouseState)
		{
			case 5:
				ScrollOnePageUp();
			break;
			case 6:
				ScrollOneLineUp();
			break;
			case 7:
				ScrollOneLineDown();
			break;
			case 8:
				ScrollOnePageDown();
			break;
			default:
			break;
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CHelpDialog::OneSecondUpdate()
{
	//if ((vOldUnderRemoteControl) && (!vGlobalUnderRemoteControl))
	//{
	//	SetDlgItemText(IDC_DialogTitleStaticText,vDialogTitleString);
	//}
	//else
	//if (vGlobalUnderRemoteControl)
	//{
	//	if (vGlobalEvenSecond)
	//	{
	//		CString TempText;
	//		TempText.LoadString(IDS_UnderRemoteControl);
	//		SetDlgItemText(IDC_DialogTitleStaticText,TempText);
	//	}
	//	else
	//		SetDlgItemText(IDC_DialogTitleStaticText,vDialogTitleString);
	//}
	//vOldUnderRemoteControl = vGlobalUnderRemoteControl;
}

void CHelpDialog::ShiftEventLogDown(int TempDown) //temp down is how many lines to scroll down
{
	int TempNumberOfLinesShown = 0;
	bool TempDoneSearchingFiles = false;
	bool TempUseNextFile = false;
	int TempNumberOfCharsDeletedFromTop = 0;
	int TempNumberOfNewChars = 0;

	//todo scroll down
	if ((vMaxEventLogScrollDown) || (TempDown < 0))
		return;

	int TempNumberOfCharactersShown = vEventLogString.GetLength();

 	int * TempColors = NULL;
	int TempSizeOfTempColors = 124 * TempDown;
	if (ThereIsEnoughMemory(sizeof(int) * TempSizeOfTempColors, "temp Color array in help dialog"))
		TempColors = (int *)malloc(sizeof(int) * TempSizeOfTempColors);
	else
	{
		ReportErrorMessage("Not Enough Memory for temp color Array Alloc in Help Dialog shift down",cEMailInspx, 32000);
		#ifdef _DEBUG
		afxDump << "Not Enough Memory for temp color array in Help Dialog shift down Alloc";
		#endif
	}
	if (TempColors)
		ZeroMemory(TempColors, sizeof(int) * TempSizeOfTempColors);

	ULONGLONG TempStartIndex;
	ULONGLONG TempEventLogCurrentFileIndex;

	if (vLineIndexInFile != 0)	// kjh bounds checking
	{
		int IndexLocation1 = ((vLineIndexInFile[0] + cErrorLogBufferSize-2) % cErrorLogBufferSize) + 1;	// added for exception error crash added to see more, exception didnt happen
		TempStartIndex = vLineIndexInFile[IndexLocation1] % 10000000; //get line file index for last line on screen
		int IndexLocation2 = ((vLineIndexInFile[0] + cErrorLogBufferSize-2) % cErrorLogBufferSize) + 1; // added for exception error crash added to see more, exception didnt happen
		TempEventLogCurrentFileIndex = vLineIndexInFile[IndexLocation2] / 10000000;
	}

	if (vEventLogString != "")		// kjh bounds checking
	{
		while (vEventLogString.GetAt(vEventLogString.GetLength() - 1 ) == '\n')
			vEventLogString.Delete((vEventLogString.GetLength() - 1) , 1);
	}

	if (TempColors)
	while ((TempNumberOfLinesShown < TempDown) && (TempDoneSearchingFiles == false))
	{
		CString TempErrorString = "";
		ULONGLONG TempEndIndex = 0;
		ULONGLONG TempSizeOfErrorLog = 0;
		char TempChars[20];

		if (TempEventLogCurrentFileIndex == -1)
		{
			vMaxEventLogScrollDown = true;
			break;
		}

		LPTSTR TempFileNameString;
		if (TempEventLogCurrentFileIndex < cMaximumEventLogFiles)
		{
			TempFileNameString = vEventLogCurrentFiles[TempEventLogCurrentFileIndex].GetBuffer(vEventLogCurrentFiles[TempEventLogCurrentFileIndex].GetLength());

			if (TempFileNameString != _T(""))
			{
				TRY
				{
					CFileException TempFileException;
					if (ThereIsEnoughMemory(sizeof(CFile), "Event Log File"))
					{
						CFile *TempErrorLogFile = new CFile;
						if (TempErrorLogFile->Open(TempFileNameString, (CFile::modeRead | CFile::shareDenyNone), &TempFileException))
						{
							if (TempEventLogCurrentFileIndex == vLastFileIndex / 10000000)
								TempSizeOfErrorLog = vLastFileIndex % 10000000;
							else
								TempSizeOfErrorLog = TempErrorLogFile->SeekToEnd();

							ULONGLONG TempAmountOfDataToProcess = TempSizeOfErrorLog;
							int TempLineType = 9;
							bool TempSkipNextDash = false;
							CString TempLine = "";
							int i = 0;

							if (TempUseNextFile == true)
							{
								TempUseNextFile = false;
								TempStartIndex = TempSizeOfErrorLog;
								i++;
							}
							else
								TempErrorLogFile->Seek(TempStartIndex,CFile::begin);
					
							if (TempErrorLogFile->Read(TempChars,1) == 1) //if can read a character from file
							while ((TempNumberOfLinesShown < TempDown) && (TempUseNextFile == false))
							{
								TempEndIndex = TempStartIndex;
								while (TempChars[0] != '\n') //get next line
								{
									if (TempStartIndex == 0)
									{
										TempUseNextFile = true;
										break;					//  hit begining of file
									}
									TempStartIndex = TempErrorLogFile->Seek(-2, CFile::current);
									if(TempErrorLogFile->Read(TempChars,1) != 1)
										break;  //unexpected amount of characters
									if(TempChars[0] == (char)10)
										continue;
									TempLine = TempChars[0]+TempLine;		
								}

								if (TempLine.GetLength() > 1) //if it got a whole line
								{
									if (i != 0)
									{
										TempLineType = _wtoi((CString)TempLine.GetAt(0));
										if (TempLine.GetLength() > 3 && (TempLine.GetAt(1) >= '0' && TempLine.GetAt(1) <= '9') && TempLine.GetAt(2) == '>')
											TempLineType = 10 * TempLineType + _wtoi( (CString)TempLine.GetAt(1));
										char TempColorCharacter = 0;
										if ((TempLine.GetAt(1) == '>') || (TempLine.GetAt(2) == '>')) //If new log format
										{
											if (TempLineType == cUserChanges) 
											{
												TempColorCharacter = '^'; //user changes are blue
											}
											else 
											if ((TempLineType == cCritical) || (TempLineType == cError) || (TempLineType == cEMailPlantOperations) || (TempLineType == cEMailMaintenance))//(1 << TempLineType & 0x2B) //0x2B is bitmask for just error mesgs
											{
												TempColorCharacter = '*'; //errors are red
											}
											else 
											if (TempLineType == cEMailInspx)
											{
												TempColorCharacter = '+';	//inspex messages are green
											}
										}
										TempLine.Delete(0,2);
										if ( TempLine.GetAt(0) == '>' )
										{
											TempLine.Delete(0,1);
										}
										
										if (1 << TempLineType & vErrorMessageDisplayMask)
										{
											TempNumberOfLinesShown++;

											for (int j = 0; j < 7; j++)
											{
												if (TempLine.GetLength() > 0)
												{
													wchar_t TempChar = TempLine.GetAt(0);
													if ((TempChar >= '0') && (TempChar <= '9'))
														TempLine.Delete(0,1);
													else
														break;
												}
												else
													break;
											}
											vFirstLineNumberOnEventLogScreen++;
											TempLine = dtoa((double)(vFirstLineNumberOnEventLogScreen + cErrorLogBufferSize)) + TempLine;

											int TempLineLength = TempLine.GetLength() + 1;	//plus one for the new line character
											vEventLogString = vEventLogString + '\n' + TempLine;	//add new line to end of log file

											if (TempColorCharacter)
											{
												int TempEndCount = TempNumberOfNewChars + TempLineLength;
												for (; TempNumberOfNewChars < TempEndCount; TempNumberOfNewChars++)
												{
													if (TempColorCharacter == '+')
													{
														if (TempNumberOfNewChars < TempSizeOfTempColors)
															TempColors[TempNumberOfNewChars] = 3; //green
													}
													else if (TempColorCharacter == '^')
													{
														if (TempNumberOfNewChars < TempSizeOfTempColors)
															TempColors[TempNumberOfNewChars] = 2; // blue
													}
													else if (TempColorCharacter == '*')
													{
														if (TempNumberOfNewChars < TempSizeOfTempColors)
															TempColors[TempNumberOfNewChars] = 1; //red
													}
												}
											}
											else
											{
												TempNumberOfNewChars = TempNumberOfNewChars + TempLineLength;
											}

											int TempSizeOfOldLine = vEventLogString.Find('\n') + 1;
											TempNumberOfCharsDeletedFromTop = TempNumberOfCharsDeletedFromTop + TempSizeOfOldLine;
											vEventLogString.Delete(0, TempSizeOfOldLine); //delete old line from begining of log file

											if (vLineIndexInFile[0] > 1000)
											{
												ReportErrorMessage("vLineIndexInFile OverFlow",cEMailInspx, 32000);
												#ifdef _DEBUG
												afxDump << "vLineIndexInFile OverFlow";
												#endif

											}
											else
											{
												vLineIndexInFile[vLineIndexInFile[0]] = TempEndIndex //line in file index
													+ TempEventLogCurrentFileIndex * 10000000; //file index
												vLineIndexInFile[0] = (vLineIndexInFile[0] % cErrorLogBufferSize) + 1; 
												vMaxEventLogScrollUp = false;
											}
										}
									}
									i++;
								}

								TempLine = "";
								TempChars[0] = ' ';
							}
						
							TempErrorLogFile->Close();
							Sleep(10); 
						}
						else
							ReportErrorMessage("Failed to read Event Log File when shifting down 1",cEMailInspx, 32000); //6/28/2011

						delete TempErrorLogFile;
					}
					else
						ReportErrorMessage("Failed to read Event Log File when shifting down 2",cEMailInspx, 32000); //6/28/2011
				}
				CATCH_ALL(TempFileException)
				{
					ReportErrorMessage("Failed to read Event Log File when shifting down 3",cEMailInspx,32000); //6/28/2011

					throw;
				}
				END_CATCH_ALL
			}
			else
				ReportErrorMessage("Failed to read Event Log File when shifting down 4",cEMailInspx, 32000); //6/28/2011
		}
		else
			ReportErrorMessage("Failed to read Event Log File when shifting down 4-Too Many Files",cEMailInspx, 32000); //6/28/2011

		if ( TempNumberOfLinesShown < TempDown )
		{
			TempEventLogCurrentFileIndex--;
		}
	}

	vEventLogString = vEventLogString + "\n\n";

	SetDlgItemText(IDC_HelpText,vEventLogString);

	//update vColor array
	int TempLoopD = 0;
	for (; TempLoopD < TempNumberOfCharactersShown - TempNumberOfCharsDeletedFromTop; TempLoopD++)
	{
		if (TempNumberOfCharsDeletedFromTop + TempLoopD < cSizeOfVColor)
			vColor[TempLoopD] = vColor[TempNumberOfCharsDeletedFromTop + TempLoopD];
	}

	for (int j = 0; TempLoopD < TempNumberOfCharactersShown - TempNumberOfCharsDeletedFromTop + TempNumberOfNewChars; TempLoopD++)
	{
		if ((TempLoopD >= cSizeOfVColor) || (j >= 124 * TempDown))
			j = TempNumberOfCharactersShown - TempNumberOfCharsDeletedFromTop + TempNumberOfNewChars;	//Exit loop if over color array size
		else
		if (TempLoopD < cSizeOfVColor)
		if (j < TempSizeOfTempColors)
			vColor[TempLoopD] = TempColors[j];
		j++;
	}
	for (;TempLoopD < TempNumberOfCharactersShown; TempLoopD++)
	{
		if (TempLoopD < cSizeOfVColor)
			vColor[TempLoopD] = 0;
	}
	DisplayColor(&vEventLogString);
	
	free(TempColors);
	TempColors = NULL;
// temp dont hide carat kjh	int 	int vTimerResult = SetTimer(vHideCaretTimerHandle,10,NULL);
//	m_SubFunction2Button.ShowWindow(SW_HIDE);
//	m_ArrowLeft2.ShowWindow(SW_HIDE);
	UpdateButtons();
	Sleep(100);
}

void CHelpDialog::ShiftEventLogFindDown(int TempDown) //temp down is how many lines to scroll down
{
	int TempNumberOfLinesShown = 0;
	bool TempDoneSearchingFiles = false;
	bool TempUseNextFile = false;
	int TempNumberOfCharsDeletedFromTop = 0;
	int TempNumberOfNewChars = 0;

	//todo scroll down
	if (( vMaxEventLogScrollDown ) || (TempDown < 0))
		return;

	int TempNumberOfCharactersShown = vEventLogString.GetLength();

	int * TempColors = NULL;
	int TempSizeOfTempColors = 124 * TempDown;
	if (ThereIsEnoughMemory(sizeof(int) * TempSizeOfTempColors, "temp Color array in help dialog"))
		TempColors = (int *)malloc(sizeof(int) * TempSizeOfTempColors);
	else
	{
		ReportErrorMessage("Not Enough Memory for temp color Array Alloc in Help Dialog shift down",cEMailInspx, 32000);
		#ifdef _DEBUG
		afxDump << "Not Enough Memory for temp color array in Help Dialog shift down Alloc";
		#endif
	}
	if (TempColors)
		ZeroMemory(TempColors, sizeof(int) * TempSizeOfTempColors);

	ULONGLONG TempStartIndex;
	ULONGLONG TempEventLogCurrentFileIndex;

	if (vLineIndexInFile != 0)	// kjh bounds checking
	{
		int IndexLocation1 = ((vLineIndexInFile[0] + cErrorLogBufferSize-2) % cErrorLogBufferSize) + 1;	// added for exception error crash added to see more, exception didnt happen
		TempStartIndex = vLineIndexInFile[IndexLocation1] % 10000000; //get line file index for last line on screen
		int IndexLocation2 = ((vLineIndexInFile[0] + cErrorLogBufferSize-2) % cErrorLogBufferSize) + 1; // added for exception error crash added to see more, exception didnt happen
		TempEventLogCurrentFileIndex = vLineIndexInFile[IndexLocation2] / 10000000;
	}

	if (vEventLogString != "")		// kjh bounds checking
	{
		while (vEventLogString.GetAt(vEventLogString.GetLength() - 1 ) == '\n')
			vEventLogString.Delete((vEventLogString.GetLength() - 1) , 1);
	}

	if (TempColors)
	while ((TempNumberOfLinesShown < TempDown) && (TempDoneSearchingFiles == false))
	{
		CString TempErrorString = "";
		ULONGLONG TempEndIndex = 0;
		ULONGLONG TempSizeOfErrorLog = 0;
		char TempChars[20];

		if (TempEventLogCurrentFileIndex == -1)
		{
			vMaxEventLogScrollDown = true;
			break;
		}
		LPTSTR TempFileNameString;
		if (TempEventLogCurrentFileIndex < cMaximumEventLogFiles)
		{
			TempFileNameString = vEventLogCurrentFiles[TempEventLogCurrentFileIndex].GetBuffer(vEventLogCurrentFiles[TempEventLogCurrentFileIndex].GetLength());
		
			if (TempFileNameString != _T(""))
			{
				TRY
				{
					CFileException TempFileException;
					if (ThereIsEnoughMemory(sizeof(CFile), "Event Log File"))
					{
						CFile *TempErrorLogFile = new CFile;
						if (TempErrorLogFile->Open(TempFileNameString, (CFile::modeRead | CFile::shareDenyNone), &TempFileException))
						{
							if (TempEventLogCurrentFileIndex == vLastFileIndex / 10000000)
								TempSizeOfErrorLog = vLastFileIndex % 10000000;
							else
								TempSizeOfErrorLog = TempErrorLogFile->SeekToEnd();

							ULONGLONG TempAmountOfDataToProcess = TempSizeOfErrorLog;
							int TempLineType = 9;
							bool TempSkipNextDash = false;
							CString TempLine = "";
							int i = 0;

							if (TempUseNextFile == true)
							{
								TempUseNextFile = false;
								TempStartIndex = TempSizeOfErrorLog;
								i++;
							}
							else
								TempErrorLogFile->Seek(TempStartIndex,CFile::begin);
					
							if (TempErrorLogFile->Read(TempChars,1) == 1) //if can read a character from file
							while ((TempNumberOfLinesShown < TempDown) && (TempUseNextFile == false))
							{
								TempEndIndex = TempStartIndex;
								while (TempChars[0] != '\n') //get next line
								{
									if (TempStartIndex == 0)
									{
										TempUseNextFile = true;
										break;					//  hit begining of file
									}
									TempStartIndex = TempErrorLogFile->Seek(-2, CFile::current);
									if(TempErrorLogFile->Read(TempChars,1) != 1)
										break;  //unexpected amount of characters
									if(TempChars[0] == (char)10)
										continue;
									TempLine = TempChars[0]+TempLine;		
								}

								if (TempLine.GetLength() > 1) //if it got a whole line
								{
									if ( i != 0 )
									{
										TempLineType = _wtoi((CString)TempLine.GetAt(0));
										if (TempLine.GetLength() > 2 && (TempLine.GetAt(1) >= '0' && TempLine.GetAt(1) <= '9'))
											TempLineType = 10 * TempLineType + _wtoi( (CString)TempLine.GetAt(1));
										char TempColorCharacter = 0;
										if ((TempLine.GetAt(1) == '>') || (TempLine.GetAt(2) == '>')) //If new log format
										{
											if (TempLineType == cUserChanges) 
											{
												TempColorCharacter = '^'; //user changes are blue
											}
											else 
											if ((TempLineType == cCritical) || (TempLineType == cError) || (TempLineType == cEMailPlantOperations) || (TempLineType == cEMailMaintenance))//(1 << TempLineType & 0x2B) //0x2B is bitmask for just error mesgs
											{
												TempColorCharacter = '*'; //errors are red
											}
											else 
											if (TempLineType == cEMailInspx)
											{
												TempColorCharacter = '+';	//inspex messages are green
											}
										}
										TempLine.Delete(0,2);
										if ( TempLine.GetAt(0) == '>' )
										{
											TempLine.Delete(0,1);
										}
										
										if (1 << TempLineType & vErrorMessageDisplayMask)
										{
											TempNumberOfLinesShown++;

											for (int j = 0; j < 7; j++)
											{
												if (TempLine.GetLength() > 0)
												{
													wchar_t TempChar = TempLine.GetAt(0);
													if ((TempChar >= '0') && (TempChar <= '9'))
														TempLine.Delete(0,1);
													else
														break;
												}
												else
													break;
											}
											vFirstLineNumberOnEventLogScreen++;
											TempLine = dtoa((double)(vFirstLineNumberOnEventLogScreen + cErrorLogBufferSize)) + TempLine;

											int TempLineLength = TempLine.GetLength() + 1;	//plus one for the new line character
											vEventLogString = vEventLogString + '\n' + TempLine;	//add new line to end of log file

											if (TempColorCharacter)
											{
												int TempEndCount = TempNumberOfNewChars + TempLineLength;
												for (; TempNumberOfNewChars < TempEndCount; TempNumberOfNewChars++)
												{
													if (TempColorCharacter == '+')
													{
														if (TempNumberOfNewChars < TempSizeOfTempColors)
															TempColors[TempNumberOfNewChars] = 3; //green
													}
													else if (TempColorCharacter == '^')
													{
														if (TempNumberOfNewChars < TempSizeOfTempColors)
															TempColors[TempNumberOfNewChars] = 2; // blue
													}
													else if (TempColorCharacter == '*')
													{
														if (TempNumberOfNewChars < TempSizeOfTempColors)
															TempColors[TempNumberOfNewChars] = 1; //red
													}
												}
											}
											else
											{
												TempNumberOfNewChars = TempNumberOfNewChars + TempLineLength;
											}

											int TempSizeOfOldLine = vEventLogString.Find('\n') + 1;
											TempNumberOfCharsDeletedFromTop = TempNumberOfCharsDeletedFromTop + TempSizeOfOldLine;
											vEventLogString.Delete(0, TempSizeOfOldLine); //delete old line from begining of log file

											vLineIndexInFile[vLineIndexInFile[0]] = TempEndIndex //line in file index
												+ TempEventLogCurrentFileIndex * 10000000; //file index
											vLineIndexInFile[0] = (vLineIndexInFile[0] % cErrorLogBufferSize) + 1; 
											vMaxEventLogScrollUp = false;
										}
									}
									i++;
								}

								TempLine = "";
								TempChars[0] = ' ';
							}
						
							TempErrorLogFile->Close();
							Sleep(10); 
						}
						else
							ReportErrorMessage("Failed to read Event Log File when shifting down 1",cEMailInspx, 32000); //6/28/2011

						delete TempErrorLogFile;
					}
					else
						ReportErrorMessage("Failed to read Event Log File when shifting down 2",cEMailInspx, 32000); //6/28/2011
				}
				CATCH_ALL(TempFileException)
				{
					ReportErrorMessage("Failed to read Event Log File when shifting down 3",cEMailInspx,32000); //6/28/2011

					throw;
				}
				END_CATCH_ALL
			}
			else
				ReportErrorMessage("Failed to read Event Log File when shifting down 4",cEMailInspx, 32000); //6/28/2011
		}
		else
			ReportErrorMessage("Failed to read Event Log File when shifting down 4-Too Many Files",cEMailInspx, 32000); //6/28/2011

		if ( TempNumberOfLinesShown < TempDown )
		{
			TempEventLogCurrentFileIndex--;
		}
	}

	vEventLogString = vEventLogString + "\n\n";

	SetDlgItemText(IDC_HelpText,vEventLogString);

	//update vColor array
	int TempLoopD = 0;
	for (; TempLoopD < TempNumberOfCharactersShown - TempNumberOfCharsDeletedFromTop; TempLoopD++)
	{
		if (TempNumberOfCharsDeletedFromTop + TempLoopD < cSizeOfVColor)
			vColor[TempLoopD] = vColor[TempNumberOfCharsDeletedFromTop + TempLoopD];
	}

	for (int j = 0; TempLoopD < TempNumberOfCharactersShown - TempNumberOfCharsDeletedFromTop + TempNumberOfNewChars; TempLoopD++)
	{
		if ((TempLoopD >= cSizeOfVColor) || (j >= 124 * (int)TempDown))
			j = TempNumberOfCharactersShown - TempNumberOfCharsDeletedFromTop + TempNumberOfNewChars;	//Exit loop if over array
		else
		if (TempLoopD < cSizeOfVColor)
		if (j < TempSizeOfTempColors)
			vColor[TempLoopD] = TempColors[j];
		j++;
	}

	for (;TempLoopD < TempNumberOfCharactersShown; TempLoopD++)
	{
		if (TempLoopD < cSizeOfVColor)
			vColor[TempLoopD] = 0;
	}
	if (vEventLogString.Find(vFindString) != -1)
	{
		DisplayColor(&vEventLogString);
		UpdateButtons();
	}
	
	free(TempColors);
	TempColors = NULL;
// temp dont hide carat kjh	int 	int vTimerResult = SetTimer(vHideCaretTimerHandle,10,NULL);
//	m_SubFunction2Button.ShowWindow(SW_HIDE);
//	m_ArrowLeft2.ShowWindow(SW_HIDE);
	Sleep(1);
}

void CHelpDialog::ShiftEventLogUp(int TempUp)
{
	int i = 0;
	ULONGLONG TempStartIndex = 0;
	int TempNumberOfLinesShown = 0;
	ULONGLONG TempEventLogCurrentFileIndex = 0;
	CString TempErrorString = "";
	bool TempDoneSearchingFiles = false;

	int TempMaxFileIndex = -1;
	
	for (i = cMaximumEventLogFiles - 1; i > -1; i--)
		if (vEventLogCurrentFiles[i] == "")
			continue;
		else
		{
			TempMaxFileIndex = i;
			break;
		}

	if (vMaxEventLogScrollUp)
		return;

	int TempNumberOfCharsDeletedFromBottom = 0;
	int TempNumberOfNewChars = 0;

	int TempNumberOfCharactersShown = vEventLogString.GetLength();

	TempStartIndex = vLineIndexInFile[ vLineIndexInFile[0] ] % 10000000; //get line file index for first line on screen
	TempEventLogCurrentFileIndex = vLineIndexInFile[ vLineIndexInFile[0] ] / 10000000;

	if (TempEventLogCurrentFileIndex > TempMaxFileIndex)
	{
		vMaxEventLogScrollUp = true;
		return;
	}


	int * TempColors = NULL;
	int TempSizeOfTempColors = 124 * TempUp;
	if (ThereIsEnoughMemory(sizeof(int) * TempSizeOfTempColors, "temp Color array in help dialog shift up"))
		TempColors = (int *)malloc(sizeof(int) * TempSizeOfTempColors);
	else
	{
		ReportErrorMessage("Not Enough Memory for temp color Array Alloc in Help Dialog shift up",cEMailInspx, 32000);
		#ifdef _DEBUG
		afxDump << "Not Enough Memory for temp color array in Help Dialog shift up Alloc";
		#endif
	}
	if (TempColors)
		ZeroMemory(TempColors, sizeof(int) * TempSizeOfTempColors);

	if (TempColors)
	while ((TempNumberOfLinesShown < TempUp) && (TempDoneSearchingFiles == false))
	{
		if (TempEventLogCurrentFileIndex > TempMaxFileIndex)
		{
			vMaxEventLogScrollUp = true;
			break;
		}

		LPTSTR TempFileNameString;
		if (TempEventLogCurrentFileIndex < cMaximumEventLogFiles)
		{
			TempFileNameString = vEventLogCurrentFiles[TempEventLogCurrentFileIndex].GetBuffer(vEventLogCurrentFiles[TempEventLogCurrentFileIndex].GetLength());

			if (TempFileNameString != _T(""))
			{
				TRY
				{
					CFileException TempFileException;
					if (ThereIsEnoughMemory(sizeof(CFile), "Event Log File"))
					{
						CFile *TempErrorLogFile = new CFile;
						if (TempErrorLogFile->Open(TempFileNameString, (CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyNone), &TempFileException))
						{
							char TempChars[20];
							bool TempDone = false;
							int TempLineType = 9; //initial line type that is not writen to log - emailinspx
							CString TempLine = "";
							bool TempSkipNextDash = false;
							ULONGLONG TempEndOfFileIndex = 0;
							bool TempReadingGoodLine = false;
							i = 0;

							if( TempEventLogCurrentFileIndex == vLastFileIndex /10000000)
								TempEndOfFileIndex = vLastFileIndex % 10000000;
							else
								TempEndOfFileIndex = TempErrorLogFile->SeekToEnd();

							for (i = 0; i< 20; i++)
								TempChars[i] = 0;

							ULONGLONG ErrorSeek = TempErrorLogFile->Seek(TempStartIndex,CFile::begin);


							while ( (TempNumberOfLinesShown < TempUp) && (TempDone == false) )
							{
								while (TempChars[0] != '\n') //get next line
								{
									if(TempStartIndex >= TempEndOfFileIndex )
									{
										TempDone = true;
										break;					//  hit begining of file
									}
									TempStartIndex++;
									if(TempErrorLogFile->Read(TempChars,1) != 1)
									{
										break;  //unexpected amount of characters
										TempDone = true;
									}
									if(TempChars[0] == (char)10)
										continue;
									TempLine = TempLine + TempChars[0];		
								}

								if ( i != 0 )
								if ( TempLine.GetLength() > 1 ) //if it got a whole line
								{
									TempLineType = _wtoi( (CString)TempLine.GetAt(0));
									if (TempLine.GetLength() > 2 && (TempLine.GetAt(1) >= '0' && TempLine.GetAt(1) <= '9') && TempLine.GetAt(2) == '>')
										 TempLineType = 10 * TempLineType + _wtoi( (CString)TempLine.GetAt(1));
									if ( (TempLineType == 0) && (TempLine.GetAt(0) != '0') )
									{
										TempLine.Delete(0,1);
										TempLineType = _wtoi( (CString)TempLine.GetAt(0));
										if ( (TempLineType == 0) && (TempLine.GetAt(0) != '0') )
											TempLineType = -1;
									}

									char TempColorCharacter = 0;
									if ( (TempLine.GetAt(1) == '>') || (TempLine.GetAt(2) == '>') ) //If new log format
									{
										if (TempLineType == cUserChanges) 
										{
											TempColorCharacter = '^'; //user changes are blue
										}
										else 
										if ((TempLineType == cCritical) || (TempLineType == cError) || (TempLineType == cEMailPlantOperations) || (TempLineType == cEMailMaintenance))//(1 << TempLineType & 0x2B) //0x2B is bitmask for just error mesgs
										{
											TempColorCharacter = '*'; //errors are red
										}
										else 
										if (TempLineType == cEMailInspx)
										{
											TempColorCharacter = '+';	//inspx messages are green
										}
									}

									TempLine.Delete(0,2);
									if ( TempLine.GetAt(0) == '>' )
									{
										TempLine.Delete(0,1);
									}
									
									if (1 << TempLineType & vErrorMessageDisplayMask)
									{
										int j = 0;
										TempNumberOfLinesShown++;
										for ( j = 0; j < 7; j++)
										{
											if( TempLine.GetLength() > 0 )
											{
												wchar_t TempChar = TempLine.GetAt(0);
												if( (TempChar >= '0') && (TempChar <= '9') )
													TempLine.Delete(0,1);
												else
													break;
											}
											else
												break;
										}

										TempLine = dtoa((double)vFirstLineNumberOnEventLogScreen) + TempLine;
										vFirstLineNumberOnEventLogScreen--;

										int TempCharsInNewLine = TempLine.GetLength() + 1; 
										if (TempColorCharacter)
										{
											int TempEndCount = TempNumberOfNewChars + TempCharsInNewLine;
										
											for (; TempNumberOfNewChars < TempEndCount; TempNumberOfNewChars++)
											{
												if (TempColorCharacter == '+')
												{
													if (TempNumberOfNewChars < TempSizeOfTempColors)
														TempColors[TempNumberOfNewChars] = 3; //green
												}
												else if (TempColorCharacter == '^')
												{
													if (TempNumberOfNewChars < TempSizeOfTempColors)
														TempColors[TempNumberOfNewChars] = 2; // blue
												}
												else if (TempColorCharacter == '*')
												{
													if (TempNumberOfNewChars < TempSizeOfTempColors)
														TempColors[TempNumberOfNewChars] = 1; //red
												}
											}
											TempColorCharacter = 0;
										}
										else
										{
											TempNumberOfNewChars = TempNumberOfNewChars + TempCharsInNewLine;
										}

										TempErrorString = TempLine + '\n' + TempErrorString;
										vLineIndexInFile[0] = ((vLineIndexInFile[0] + cErrorLogBufferSize-2) % cErrorLogBufferSize) + 1;
										vLineIndexInFile[vLineIndexInFile[0]] = TempStartIndex//store line in file index
											+ TempEventLogCurrentFileIndex * 10000000;					//also store file index
										if (TempStartIndex == TempEndOfFileIndex)
											vMaxEventLogScrollUp = true;
										//vMaxEventLogScrollDown = false; only after delete line

										//delete chars from the back of the string, and keep track of how many
										int TempStringLength = vEventLogString.GetLength();
										int StartBackwardSearch = TempStringLength - 1;		//last char in string
										int EndDeleteOfLine = StartBackwardSearch;
										while ((vEventLogString.GetAt(StartBackwardSearch) == '\n'))
										{
											StartBackwardSearch--;	// delete extra carriage returns at end of string
										}

										// look for end of first string
										while (vEventLogString.GetAt(StartBackwardSearch) != '\n')
										{
											StartBackwardSearch--;
										}

										int TempDeletedStringLength = EndDeleteOfLine - StartBackwardSearch /* + 1*/; //delete last string, leave double <cr> at end of file
										TempNumberOfCharsDeletedFromBottom = TempNumberOfCharsDeletedFromBottom + TempDeletedStringLength;
										vEventLogString.Delete(StartBackwardSearch, TempDeletedStringLength);
										vMaxEventLogScrollDown = false; // only after delete line
									}
								}
								TempLine = "";
								TempChars[0] = ' ';
								i++;
							}

							TempErrorLogFile->Close();
							Sleep(10); 
						}
						else
							ReportErrorMessage("Failed to Open Event Log File to Read 1 - Did not open",cEMailInspx, 32000); //6/28/2011

						delete TempErrorLogFile;
					}
					else
						ReportErrorMessage("Failed to Open Event Log File to Read 1 - Out of memory",cEMailInspx, 32000); //6/28/2011
				}
				CATCH_ALL(TempFileException)
				{
					ReportErrorMessage("Failed to Open Event Log File to Read 1 - Exception",cEMailInspx,32000); //6/28/2011

					throw;
				}
				END_CATCH_ALL
			}
			else
				ReportErrorMessage("Failed to Open Event Log File to Read 1 - Name blank",cEMailInspx, 32000); //6/28/2011
		}
		else
			ReportErrorMessage("Failed to Open Event Log File to Read 1 - Too Many Files",cEMailInspx, 32000); //6/28/2011
		
		if (TempNumberOfLinesShown < TempUp)
		{
			TempStartIndex = 0;
			TempEventLogCurrentFileIndex++;

			if (vGlobalDisplayDialogThread)
				vGlobalDisplayDialogThread->NextStep();
		}
	}

	int TempStringLength = vEventLogString.GetLength();
	int TempOnePageStartIndex = TempStringLength;
	int TempDeletedNewLines = -1;

	while ((vEventLogString.GetLength()) && (vEventLogString.GetAt(vEventLogString.GetLength() - 1 ) == '\n'))
	{
		vEventLogString.Delete((vEventLogString.GetLength() - 1) , 1);
		TempDeletedNewLines++;
	}
	//if (TempDeletedNewLines != 0)
	//	ReportErrorMessage("Log may be corrupted as deleted characters from end of log", cEMailInspx, 32000);

	vEventLogString = vEventLogString + "\n\n";

	vEventLogString = TempErrorString + vEventLogString;

	SetDlgItemText(IDC_HelpText,vEventLogString);

	//update vColor array
	int TempNumberShift = (int)TempNumberOfNewChars - (int)TempNumberOfCharsDeletedFromBottom;
	int TempNewIndex = vEventLogString.GetLength()-1;//TempNumberOfCharactersShown + TempNumberShift; -1 to start at 0
	int TempOldIndex = 0;

	TempOldIndex = TempNumberOfCharactersShown - TempNumberOfCharsDeletedFromBottom;

	if (TempOldIndex < (int)TempNewIndex)
	{
		for (; TempOldIndex > 0 ; TempOldIndex--) //shift block of old numbers to their shifted postions
		{
			if ((TempNewIndex >= cSizeOfVColor) || (TempOldIndex >= cSizeOfVColor) || (TempNewIndex < 0) || (TempOldIndex < 0))
				TempOldIndex = 0;	//exit loop if anything out of bounds
			else
			{
				if (TempNewIndex < cSizeOfVColor)
				if (TempOldIndex < cSizeOfVColor)
					vColor[TempNewIndex] = vColor[TempOldIndex];
				TempNewIndex--;
			}
		}
		
		if (TempNewIndex < cSizeOfVColor)
			vColor[TempNewIndex] = vColor[0];
		TempNewIndex--;

		int TempLoopC = TempNumberOfNewChars;
		for (; TempLoopC > 0; TempLoopC--)
		if (TempNumberOfNewChars - TempLoopC < cSizeOfVColor)
		if (TempLoopC < TempSizeOfTempColors)
		{
			vColor[TempNumberOfNewChars - TempLoopC] = TempColors[TempLoopC-1];
		}

		if (TempLoopC < cSizeOfVColor)
		if (TempLoopC < TempSizeOfTempColors)
			vColor[0] = TempColors[TempLoopC];
	}

	DisplayColor(&vEventLogString);

	if (TempColors)
		free(TempColors);

	UpdateButtons();
}

int CHelpDialog::NumberOfLinesInAllFilesOfDay()
{
	int TempEndIndex = 0;
	int TempStartIndex = 0;
	int TempMaxFileIndex = 0;
	int TempNumberOfLinesShown = 0;
	bool TempDoneSearchingFiles = false;
	int TempEventLogCurrentFileIndex = 0;
	int i = 0;
	char TempChars[1000];
	//debug 
//	CString TempDebugString = "";

	TempStartIndex = 0;

	for (i = cMaximumEventLogFiles - 1; i > -1; i--)
		if (vEventLogCurrentFiles[i] == "")
			continue;
		else
		{
			TempMaxFileIndex = i;
			break;
		}
	
//	if( TempMaxFileIndex > 0)	//if there is more than one file, warn that it will take a while
		

	while (TempDoneSearchingFiles == false) 
	{
		if (TempEventLogCurrentFileIndex > TempMaxFileIndex)
			break;

		CString TempFileString = vEventLogCurrentFiles[TempEventLogCurrentFileIndex];
		LPTSTR TempFileNameString = TempFileString.GetBuffer(TempFileString.GetLength());

		if( TempFileNameString != _T(""))
		{
			TRY
			{
				CFileException TempFileException;
				if (ThereIsEnoughMemory(sizeof(CFile), "Event Log File"))
				{
					CFile *TempErrorLogFile = new CFile;
					if (TempErrorLogFile->Open(TempFileNameString, (CFile::modeRead | CFile::shareDenyNone), &TempFileException))
					{
						bool TempDone = false;
						int TempLineType = 9; //initial line type that is not writen to log - emailinspx
						CString TempLine = "";
						bool TempSkipNextDash = false;
						ULONGLONG TempEndOfFileIndex = 0;
						bool TempReadingGoodLine = false;
						
						if( TempEventLogCurrentFileIndex == vLastFileIndex /10000000)
							TempEndOfFileIndex = vLastFileIndex % 10000000;
						else
						{
							if (TempFileNameString != _T(""))
								TempEndOfFileIndex = TempErrorLogFile->SeekToEnd();
						}

						TempEndIndex = 0;
						TempStartIndex = 0;
						TempErrorLogFile->SeekToBegin();
						
						
						for (i = 0; i< 1000; i++)
							TempChars[i] = 0;
						
						i = 0;
						while (TempDone == false)
						{
							int TempNumberOfCharsReadIn = 1000;
							while (TempChars[i] != '\n') //get next line
							{
								i++;
								if( (i % TempNumberOfCharsReadIn) == 0)
								{
									if(TempStartIndex >= TempEndOfFileIndex )
									{
										TempDone = true;
										break;					//  hit begining of file
									}

									if (TempNumberOfCharsReadIn < 1000)
									{
										TempDone = true;
										break;
									}
									
									int TempValue = TempErrorLogFile->Read(TempChars,TempNumberOfCharsReadIn);
									if( TempValue < TempNumberOfCharsReadIn) 
									{
										if( TempValue < 1)
										{
											TempDone = true;
											break;  //unexpected amount of characters
										}
										else
											TempNumberOfCharsReadIn = TempValue;
									}
									TempStartIndex = TempStartIndex + TempNumberOfCharsReadIn;
									i = 0;
								}
								if( (TempChars[i] != (char)13) && (TempChars[i] != (char)10) && (TempChars[i] != '*') )
								{
									TempLine = TempLine + (char)TempChars[i];
								}
								
							}

							if(TempDone == true)
								break;

							if ( TempLine.GetLength() > 1 ) //if it got a whole line
							{
								TempLineType = _wtoi( (CString)TempLine.GetAt(0));
								if ( (TempLineType == 0) && (TempLine.GetAt(0) != '0') )
								{
									TempLine.Delete(0);
									TempLineType = _wtoi( (CString)TempLine.GetAt(0));
									if ( (TempLineType == 0) && (TempLine.GetAt(0) != '0') )
										TempLineType = 9;
								}

								if (1 << TempLineType & vErrorMessageDisplayMask)
								{
				//					TempLine.Delete(0,2);
				//					for ( int j = 0; j < 7; j++)
				//					{
				//						if (TempLine.GetLength() > 0)
				//						{
				//							char TempChar = TempLine.GetAt(0);
				//							if( (TempChar >= '0') && (TempChar <= '9') )
				///								TempLine.Delete(0,1);
				//							else
				///								break;
				//						}
				//						else
				//							break;
				//					}
									TempNumberOfLinesShown++;
									//if ((TempNumberOfLinesShown % 100) == 0)
									//	vMainWindowPointer->DoBackGroundWork();
									//	Sleep(1); //yeald thread
	//								TempLine = dtoa(TempNumberOfLinesShown) + TempLine;
									
	//								TempDebugString = TempDebugString + '\n' + TempLine;
									
				//					if(TempNumberOfLinesShown == 10286)
				//						int x = 0;
				//					if(TempNumberOfLinesShown == 47)
				//						int x = 0;
								}
								TempLine = "";
							}
							TempChars[i] = '*'; //remove the '\n' from this space so it will get a new line.
						}

						TempErrorLogFile->Close();
						Sleep(10); 
					}
					else
						ReportErrorMessage("Failed to Open Event Log File to Read 2 - Did not open",cEMailInspx, 32000); //6/28/2011

					delete TempErrorLogFile;
				}
				else
					ReportErrorMessage("Failed to Open Event Log File to Read 2 - Out of memory",cEMailInspx, 32000); //6/28/2011
			}
			CATCH_ALL(TempFileException)
			{
				ReportErrorMessage("Failed to Open Event Log File to Read 2 - Exception",cEMailInspx,32000); //6/28/2011

				throw;
			}
			END_CATCH_ALL
		}
		else
			ReportErrorMessage("Failed to Open Event Log File to Read 2 - Name blank",cEMailInspx, 32000); //6/28/2011
		
		TempEventLogCurrentFileIndex++;
		TempNumberOfLinesShown++;
		if (vGlobalDisplayDialogThread)
			vGlobalDisplayDialogThread->NextStep();
	}
	if (TempNumberOfLinesShown > 0)
		TempNumberOfLinesShown--;	// when done, dont increment.
/*
	//debugging code to print out everything found
	CString TempFileString = "C:\\NumberOfLines.txt";
	LPTSTR TempFileNameString = TempFileString.GetBuffer(TempFileString.GetLength());
//LPTSTR
		if( TempFileNameString != "" )
		{
			TRY
			{
				CFileException TempFileException;
				if (ThereIsEnoughMemory(sizeof(CFile), "Event Log File"))
				{
					CFile *TempErrorLogFile = new CFile;
					if (TempErrorLogFile->Open(TempFileNameString, 
					(CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone), &TempFileException))
					{
						TempErrorLogFile->Write(TempDebugString, TempDebugString.GetLength());
						TempErrorLogFile->Close();
					}
					else
						ReportErrorMessage("Failed to Write Event Log File to Disk",cEMailInspx,0);

					delete TempErrorLogFile;
				}
				else
					ReportErrorMessage("Failed to Write Event Log File to Disk",cEMailInspx,0);
			}
			CATCH_ALL(TempFileException)
			{
				ReportErrorMessage("System could not write to Event Log",cEMailInspx,32000);

				throw;
			}
			END_CATCH_ALL
		}
		else
			ReportErrorMessage("Failed to Write Event Log File to Disk",cEMailInspx,0);
		//end debug section
*/
	return TempNumberOfLinesShown;
}


void CHelpDialog::ShowEndOfEventLog()
{
	if (vMaxEventLogScrollDown == true)
	if (vMaxEventLogScrollUp == true)
		return;

	int TempEndIndex = 0;
	int TempStartIndex = 0;
	int TempMaxFileIndex = 0;
	int TempNumberOfLinesShown = 0;
	int TempNumberOfCharactersShown = 0;
	bool TempDoneSearchingFiles = false;
	int TempEventLogCurrentFileIndex = 0;
	int i = 0;
	if (vLinesInCurrentDayEventLog == -1)
	{
		//SetDlgItemText(IDC_HelpText, _T("Reading In Data, Please Wait..."));
		//int vTimerResult = SetTimer(vProcessingMessageTimerHandle,10,NULL);
		//return;
		if (!vGlobalDisplayDialogThread)
		{
			vGlobalDisplayDialogThread = (CDisplayDialogThread*)AfxBeginThread(RUNTIME_CLASS(CDisplayDialogThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
			if (!vGlobalDisplayDialogThread)
				ReportErrorMessage("Error-Could Not Create Display Dialog Thread", cEMailInspx,32000); 
			else
			{
				vGlobalDisplayDialogThread->vMessageToDisplay = "\n\n\nReading Event Log Data . . .";
				vGlobalDisplayDialogThread->ResumeThread();
			}
		}
		else
				vGlobalDisplayDialogThread->vMessageToDisplay = "\n\n\nReading Event Log Data . . ." + dtoa(TempEventLogCurrentFileIndex, 0);
	}
	
	if (vLinesInCurrentDayEventLog < cErrorLogBufferSize)
		return;

	vFirstLineNumberOnEventLogScreen = vLinesInCurrentDayEventLog - cErrorLogBufferSize;
	ULONGLONG TempCurrentLineNumber = vLinesInCurrentDayEventLog;

	CreateColorDataStructure();
	
	TempStartIndex = 0;
	vEventLogString = "";

	vLineIndexInFile[0] = 1;
	for (i = 0; i < 1000; i++)
		vLineIndexInFile[i] = 0;

	for (i = cMaximumEventLogFiles - 1; i > -1; i--)
	if (vEventLogCurrentFiles[i] == "")
		continue;
	else
	{
		TempMaxFileIndex = i;
		break;
	}
	
	while ((TempNumberOfLinesShown < cErrorLogBufferSize) && (TempDoneSearchingFiles == false))
	{
		if (TempEventLogCurrentFileIndex > TempMaxFileIndex)
			break;

		CString TempFileString = vEventLogCurrentFiles[TempEventLogCurrentFileIndex];
		LPTSTR TempFileNameString = TempFileString.GetBuffer(TempFileString.GetLength());

		if (TempFileNameString != _T(""))
		{
			TRY
			{
				CFileException TempFileException;
				if (ThereIsEnoughMemory(sizeof(CFile), "Event Log File"))
				{
					CFile *TempErrorLogFile = new CFile;
					if (TempErrorLogFile->Open(TempFileNameString, (CFile::modeRead | CFile::shareDenyNone), &TempFileException))
					{
						char TempChars[20];
						bool TempDone = false;
						int TempLineType = -1; //initial line type that is not writen to log - emailinspx
						CString TempLine = "";
						bool TempSkipNextDash = false;
						ULONGLONG TempEndOfFileIndex = 0;
						bool TempReadingGoodLine = false;
						
						if (TempEventLogCurrentFileIndex == vLastFileIndex /10000000)
							TempEndOfFileIndex = vLastFileIndex % 10000000;
						else
							TempEndOfFileIndex = TempErrorLogFile->SeekToEnd();

						TempEndIndex = 0;
						TempStartIndex = 0;
						TempErrorLogFile->SeekToBegin();
						
						for (i = 0; i< 20; i++)
							TempChars[i] = 0;

						while ((TempNumberOfLinesShown < cErrorLogBufferSize) && (TempDone == false))
						{
							while (TempChars[0] != '\n') //get next line
							{
								if (TempStartIndex >= TempEndOfFileIndex)
								{
									TempDone = true;
									vMaxEventLogScrollUp = true;
									break;					//  hit begining of file
								}
								TempStartIndex++;
								if (TempErrorLogFile->Read(TempChars,1) != 1)
								{
									TempDone = true;
									break;  //unexpected amount of characters
								}
								if (TempChars[0] == (char)10)
									continue;
								TempLine = TempLine + TempChars[0];		
							}

							if (TempLine.GetLength() > 1) //if it got a whole line
							{
								TempLineType = _wtoi( (CString)TempLine.GetAt(0));
								if (TempLine.GetLength() > 2 && (TempLine.GetAt(1) >= '0' && TempLine.GetAt(1) <= '9'))
										TempLineType = 10 * TempLineType + _wtoi( (CString)TempLine.GetAt(1));
								if ( (TempLineType == 0) && (TempLine.GetAt(0) != '0') )
								{
									TempLine.Delete(0,1);
									TempLineType = _wtoi( (CString)TempLine.GetAt(0));
									if ( (TempLineType == 0) && (TempLine.GetAt(0) != '0') )
										TempLineType = 9;
								}

								char TempColorCharacter = 0;
								if ( (TempLine.GetAt(1) == '>') || (TempLine.GetAt(2) == '>') ) //If new log format
								{
									if (TempLineType == cUserChanges) 
									{
										TempColorCharacter = '^'; //user changes are blue
									}
									else
									if ((TempLineType == cCritical) || (TempLineType == cError) || (TempLineType == cEMailPlantOperations) || (TempLineType == cEMailMaintenance))//(1 << TempLineType & 0x2B) //0x2B is bitmask for just error mesgs
									{
										TempColorCharacter = '*'; //errors are red
									}
									else 
									if (TempLineType == cEMailInspx)
									{
										TempColorCharacter = '+';	//inspx messages are green
									}
								}

								TempLine.Delete(0,2);
								if (TempLine.GetAt(0) == '>')
								{
									TempLine.Delete(0,1);
								}
									
								if (1 << TempLineType & vErrorMessageDisplayMask)
								{
									for ( int j = 0; j < 7; j++)
									{
										if (TempLine.GetLength() > 0)
										{
											wchar_t TempChar = TempLine.GetAt(0);
											if ((TempChar >= '0') && (TempChar <= '9'))
												TempLine.Delete(0,1);
											else
												break;
										}
										else
											break;
									}
									TempLine = dtoa((double)(vLinesInCurrentDayEventLog - TempNumberOfLinesShown)) + TempLine;
									TempNumberOfLinesShown++;
									if (TempColorCharacter)
									{
										int TempEndCount = TempNumberOfCharactersShown + TempLine.GetLength()+1;
										for (; TempNumberOfCharactersShown < TempEndCount; TempNumberOfCharactersShown++)
										{
											if (TempColorCharacter == '+')
											{
												if (cSizeOfVColor - TempNumberOfCharactersShown < cSizeOfVColor)
													vColor[cSizeOfVColor - TempNumberOfCharactersShown] = 3; //green
											}
											else if (TempColorCharacter == '^')
											{
												if (cSizeOfVColor - TempNumberOfCharactersShown < cSizeOfVColor)
													vColor[cSizeOfVColor - TempNumberOfCharactersShown] = 2; // blue
											}
											else if (TempColorCharacter == '*')
											{
												if (cSizeOfVColor - TempNumberOfCharactersShown < cSizeOfVColor)
													vColor[cSizeOfVColor - TempNumberOfCharactersShown] = 1; //red
											}
										}
									}
									else
									{
										TempNumberOfCharactersShown = TempNumberOfCharactersShown + TempLine.GetLength() + 1;
									}

									vEventLogString = TempLine + '\n' + vEventLogString;
									vLineIndexInFile[0] = ( (vLineIndexInFile[0] + cErrorLogBufferSize-2) % cErrorLogBufferSize) + 1;
									vLineIndexInFile[ vLineIndexInFile[0] ] = TempStartIndex
										+ TempEventLogCurrentFileIndex * 10000000;
									if ( TempStartIndex == TempEndOfFileIndex)
										vMaxEventLogScrollUp = true;
									vMaxEventLogScrollDown = false;
								}
							}
							TempLine = "";
							TempChars[0] = ' ';
							i++;
							Sleep(1);
						}
						TempErrorLogFile->Close();
						Sleep(10); 
					}
					else
						ReportErrorMessage("Failed to Open Event Log File to Read 3 - File not opened",cEMailInspx, 32000); //6/28/2011

					delete TempErrorLogFile;
				}
				else
					ReportErrorMessage("Failed to Open Event Log File to Read 3 - Out of Memory",cEMailInspx, 32000); //6/28/2011
			}
			CATCH_ALL(TempFileException)
			{
				ReportErrorMessage("Failed to Open Event Log File to Read 3 - Exception",cEMailInspx,32000); //6/28/2011

				throw;
			}
			END_CATCH_ALL
		}
		else
			ReportErrorMessage("Failed to Open Event Log File to Read 3 - Name blank",cEMailInspx, 32000); //6/28/2011
		if (TempNumberOfLinesShown < cErrorLogBufferSize)
			TempEventLogCurrentFileIndex++;
	}

	vEventLogString = vEventLogString + "\n\n"; //extra line at end for looks
	
	vMaxEventLogScrollDown = true;
	vMaxEventLogScrollUp = false;

	SetDlgItemText(IDC_HelpText,vEventLogString);

	//update colors
	int TempEmptySpace = cSizeOfVColor - TempNumberOfCharactersShown;
	for (i = 0; (int)i < TempNumberOfCharactersShown; i++)
	{
		if (i + TempEmptySpace >= cSizeOfVColor)
			i = TempNumberOfCharactersShown;	//Exit loop if beyond color array
		else
		if (i + TempEmptySpace < cSizeOfVColor)
			vColor[i] = vColor[i + TempEmptySpace];
	}
	DisplayColor(&vEventLogString);

//	int vTimerResult = SetTimer(vHideCaretTimerHandle,10,NULL);
//	m_SubFunction2Button.ShowWindow(SW_HIDE);
//	m_ArrowLeft2.ShowWindow(SW_HIDE);
	UpdateButtons();

	if (vGlobalDisplayDialogThread)
	{
		vGlobalDisplayDialogThread->ExitDisplayThread();
		vGlobalDisplayDialogThread = NULL;
	}
}

void CHelpDialog::ShowPreviousErrorLog()
{			//find files to read from

	TempHoldNextOrPreviousLogString = "Loading Previous Day's Log File"; //or "Loading Next Day's Log File"
	m_HelpTextControl.SetWindowText(_T("Loading Previous Day's Log . . ."));
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	LPTSTR TempFileNameString;
	CFileStatus TempFileStatus;
	CString TempDateString = vEventLogCurrentFiles[0].Mid(37, 10);		//day of file(s) last processed.
	CString TempFileDirectoryString = cScanTracEventLogDirectory;
	CString TempTitleDateString = "";
	int TempNextFileIndex = 0;
	int TempDateIndex = 0; 
	bool TempFoundPreviousFile = false;
	bool TempPreviousFilesExist = false;
	
	vSameLogFile = false; // new log file loaded
	if (vLineIndexInFile)
		free(vLineIndexInFile);
	if (ThereIsEnoughMemory(sizeof(ULONGLONG) * 1001, "Event Log Dialog File to string index"))
	{
		vLineIndexInFile = (ULONGLONG *)(malloc(sizeof(ULONGLONG) * 1001));
		//vLineIndexInFile[0] = 1;
		int i;
		for (i = 0; i < 1000; i++)
			vLineIndexInFile[i] = 0;
	}
	else
	{
		ReportErrorMessage("Not Enough Memory for EventLog Alloc",cEMailInspx, 32000);
		#ifdef _DEBUG
		afxDump << "Not Enough Memory for EventLog Alloc";
		#endif
	}
	vLastFileIndex = 0;
	if ( TempDateString.GetAt(2) == '-' ) // file is month-day-year old format instead of year-month-day new format
	{
		TempDateIndex = _wtoi( TempDateString.Mid(0,2) ) * 100// Months
			+ _wtoi(TempDateString.Mid(3,2)) //days
			+ _wtoi(TempDateString.Mid(6,4)) * 10000; //years
	}
	else
	{
		TempDateIndex = _wtoi( TempDateString.Mid(0,4) )* 10000//years
			+ _wtoi(TempDateString.Mid(5,2))*100 //months
			+ _wtoi(TempDateString.Mid(8,2)); //days
	}

	vLinesInCurrentDayEventLog = -1;
	vLastFileIndex = 0;

//	for (int i = 0; i < 50; i++)
//		vEventLogCurrentFiles[i] = "";

	TempFileDirectoryString.Delete(TempFileDirectoryString.GetLength() -1, 1);
	TempFileNameString = TempFileDirectoryString.GetBuffer(TempFileDirectoryString.GetLength()); //+ "\\ScanTracEventLog2008-07-14_12-34-08.txt";
	if (CFile::GetStatus(TempFileNameString, TempFileStatus))
	{
		TempFileDirectoryString = TempFileDirectoryString + "\\";
		CString TempFilePathString = TempFileDirectoryString + "ScanTracEventLog" + "*" + ".txt";

		LPTSTR TempDirectoryFiles = TempFilePathString.GetBuffer(TempFilePathString.GetLength());
		
		hFind = FindFirstFile(TempDirectoryFiles, &FindFileData);
		
		TempFilePathString = FindFileData.cFileName; 
		TempFilePathString = TempFileDirectoryString + TempFilePathString;
		TempFileNameString = TempFilePathString.GetBuffer(TempFilePathString.GetLength());
		
		if (CFile::GetStatus(TempFileNameString, TempFileStatus))
		{
			ULONGLONG TempFileSize = TempFileStatus.m_size;

			//look for old event log under 2Mb
			CString TempFilePathString = TempFileDirectoryString + "ScanTracEventLog" + "*" + ".txt";

			LPTSTR TempDirectoryFiles = TempFilePathString.GetBuffer(TempFilePathString.GetLength());
			
			hFind = FindFirstFile(TempDirectoryFiles, &FindFileData);
			
			if (hFind == INVALID_HANDLE_VALUE) // no files in directory
			{			//if no old files found, 
				;//TempFoundPreviousFile = false;
			} 
			else	//else scan through files to see one less than 2MB
			{
				for (int i = 0; i < cMaximumEventLogFiles;)
				{
					TempFilePathString = FindFileData.cFileName; 
					TempFilePathString = TempFileDirectoryString + TempFilePathString;
						
					LPTSTR TempFileNameString = TempFilePathString.GetBuffer(TempFilePathString.GetLength());
					CFile::GetStatus(TempFileNameString, TempFileStatus);
					ULONGLONG TempFileSize = TempFileStatus.m_size;
					if ( TempFileSize > 0)
					{
						CString TempFileDateString = TempFilePathString.Mid(37,10); //CString Mid( int nFirst, int nCount ) const; throw( CMemoryException );
						int TempCurrentFileDateIndex = 0;
						if (TempFileDateString.GetAt(2) == '-') // file is month-day-year old format instead of year-month-day new format
						{
							TempCurrentFileDateIndex = _wtoi( TempFileDateString.Mid(0,2) ) * 100// Months
								+ _wtoi(TempFileDateString.Mid(3,2)) //days
								+ _wtoi(TempFileDateString.Mid(6,4)) * 10000; //years
						}
						else
						{
							TempCurrentFileDateIndex = _wtoi( TempFileDateString.Mid(0,4) ) * 10000// years
								+ _wtoi(TempFileDateString.Mid(5,2))*100 //months
								+ _wtoi(TempFileDateString.Mid(8,2)); //days
						}

						if (TempCurrentFileDateIndex < TempDateIndex)
						{
							if ((TempNextFileIndex == 0) || (TempNextFileIndex < TempCurrentFileDateIndex))
							{
								if (TempFoundPreviousFile)
									TempPreviousFilesExist = true;
								TempFoundPreviousFile = true;

								TempTitleDateString = TempFilePathString;
								if (TempTitleDateString.GetLength() > 36)
									TempTitleDateString.Delete(0,37);
								int TempFileTypeIndex = TempTitleDateString.ReverseFind('.');

								if (TempFileTypeIndex < TempTitleDateString.GetLength())
									TempTitleDateString.Delete(TempFileTypeIndex,TempTitleDateString.GetLength() - TempFileTypeIndex);
								
								TempNextFileIndex = TempCurrentFileDateIndex;
								for (i = 0; i < cMaximumEventLogFiles; i++)
									vEventLogCurrentFiles[i] = "";
								i = 0;
								vEventLogCurrentFiles[i] = TempFilePathString;
								i++;
							}
							else
							if (TempNextFileIndex == TempCurrentFileDateIndex)
							{
								if (i >= cMaximumEventLogFiles)
									ReportErrorMessage("More than " + dtoa(cMaximumEventLogFiles, 0) + " Event Logs today " + TempDateString, cEMailInspx, 32000);
								else
									vEventLogCurrentFiles[i] = TempFilePathString;
								i++;
							}
							else
							if (TempFoundPreviousFile) //(TempNextFileIndex < TempCurrentFileDateIndex)
							{
								TempPreviousFilesExist = true;
							}
						}
					}
					if (!FindNextFile(hFind,&FindFileData))
					{
						break;
					}
				}		
			}
			FindClose(hFind);
		}
	}
	if (TempFoundPreviousFile)
	{
		ShowErrorLog(0);
		SetDlgItemText(IDC_HelpTitle, "Log Started: " + TempTitleDateString);
		vShowUpdateButton = false;
		m_Function5Button.ShowWindow(SW_SHOW);
		m_SubFunction2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nNo Previous Log.\nThis is the Oldest Log.";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}

	if (TempPreviousFilesExist == false)
	{
		m_Function3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
	}
}

void CHelpDialog::ShowNextErrorLog()
{			//find files to read from in a more current day
	TempHoldNextOrPreviousLogString = "Loading Next Day's Log File"; //or "Loading Previous Day's Log File"; 
	m_HelpTextControl.SetWindowText(_T("Loading Next Day's Log . . ."));
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	LPTSTR TempFileNameString;
	CString TempCurrentFileDayString = vEventLogCurrentFiles[0].Mid(37, 10);
	CString TempFileDirectoryString = cScanTracEventLogDirectory;
	CString TempTitleDateString = "";
	CFileStatus TempFileStatus;
	bool TempFoundNextFile = false;
	bool TempNextFilesExist = false;

	vLinesInCurrentDayEventLog = -1;
	vLastFileIndex = 0;
	vShowUpdateButton = false;

	TempFileDirectoryString.Delete(TempFileDirectoryString.GetLength() -1, 1);
	TempFileNameString = TempFileDirectoryString.GetBuffer(TempFileDirectoryString.GetLength()); //+ "\\ScanTracEventLog2008-07-14_12-34-08.txt";
	if (CFile::GetStatus(TempFileNameString, TempFileStatus))
	{
		TempFileDirectoryString = TempFileDirectoryString + "\\";
		CString TempFilePathString = TempFileDirectoryString + "ScanTracEventLog" + "*" + ".txt";

		LPTSTR TempDirectoryFiles = TempFilePathString.GetBuffer(TempFilePathString.GetLength());
		
		hFind = FindFirstFile(TempDirectoryFiles, &FindFileData);
		
		if (hFind == INVALID_HANDLE_VALUE) // no files in directory
		{
			//if no old files found, 
			; //TempFoundNextFile = false;
		} 
		else	//else scan through files to see one less than 2MB
		{
			int TempOldFileDateIndex = 0;

			if ( TempCurrentFileDayString.GetAt(2) == '-' ) // file is month-day-year old format instead of year-month-day new format
			{
				TempOldFileDateIndex = _wtoi( TempCurrentFileDayString.Mid(0,2) ) * 100// months
					+ _wtoi(TempCurrentFileDayString.Mid(3,2)) //days
					+ _wtoi(TempCurrentFileDayString.Mid(6,4)) * 10000; //years
			}
			else
			{
				TempOldFileDateIndex = _wtoi(TempCurrentFileDayString.Mid(0,4)) * 10000// years
					+ _wtoi(TempCurrentFileDayString.Mid(5,2))*100 //months
					+ _wtoi(TempCurrentFileDayString.Mid(8,2)); //days
			}

			int TempNewFileDateIndex = 0;

			int TempLoopP = 0;
			for (int TempLoopP = 0; TempLoopP < cMaximumEventLogFiles;)
			{
				TempFilePathString = FindFileData.cFileName; 
				TempFilePathString = TempFileDirectoryString + TempFilePathString;
					
				LPTSTR TempFileNameString = TempFilePathString.GetBuffer(TempFilePathString.GetLength());
				CFile::GetStatus(TempFileNameString, TempFileStatus);
				ULONGLONG TempFileSize = TempFileStatus.m_size;
				if (TempFileSize > 0)
				{
					CString TempFileDateString = TempFilePathString.Mid(37,10); //CString Mid( int nFirst, int nCount ) const; throw( CMemoryException );
					int TempCurrentFileDateIndex = 0;

					if (TempFileDateString.GetAt(2) == '-') // file is month-day-year old format instead of year-month-day new format
					{
						TempCurrentFileDateIndex = _wtoi( TempFileDateString.Mid(0,2) ) * 100// months
							+ _wtoi(TempFileDateString.Mid(3,2)) //days
							+ _wtoi(TempFileDateString.Mid(6,4)) * 10000; //years
					}
					else
					{
						TempCurrentFileDateIndex = _wtoi(TempFileDateString.Mid(0,4)) * 10000// years
							+ _wtoi(TempFileDateString.Mid(5,2)) * 100 //months
							+ _wtoi(TempFileDateString.Mid(8,2)); //days
					}

					if (TempCurrentFileDateIndex > TempOldFileDateIndex)
					{
						if ((TempNewFileDateIndex == 0) || (TempNewFileDateIndex > TempCurrentFileDateIndex))
						{
							if (TempFoundNextFile)
									TempNextFilesExist = true;
							TempFoundNextFile = true;

							TempTitleDateString = TempFilePathString;
							if (TempTitleDateString.GetLength() > 36)
								TempTitleDateString.Delete(0,37);
							int TempFileTypeIndex = TempTitleDateString.ReverseFind('.');

							if (TempFileTypeIndex < TempTitleDateString.GetLength())
								TempTitleDateString.Delete(TempFileTypeIndex,TempTitleDateString.GetLength() - TempFileTypeIndex);

							TempNewFileDateIndex = TempCurrentFileDateIndex;
							for (TempLoopP = 0; TempLoopP < cMaximumEventLogFiles; TempLoopP++)
								vEventLogCurrentFiles[TempLoopP] = "";
							TempLoopP = 0;
							vEventLogCurrentFiles[TempLoopP] = TempFilePathString;
							TempLoopP++;
						}
						else
						if (TempNewFileDateIndex == TempCurrentFileDateIndex)
						{
							if (TempLoopP >= cMaximumEventLogFiles)
							{
								ReportErrorMessage("Too Many Log Files.  There are > " + dtoa(cMaximumEventLogFiles, 0) + ".  Delete some.",cEMailInspx, 32000); 
								continue;
							}
							else
							{
							vEventLogCurrentFiles[TempLoopP] = TempFilePathString;
								TempLoopP++;
							}
						}
						else
						if (TempFoundNextFile == true)
						{
							TempNextFilesExist = true;
						}
					}	
				}

				if (!FindNextFile(hFind,&FindFileData))
					break;
			} 
//			TempMaxFileIndex = TempEventLogCurrentFileIndex = TempLoopP;
			if (TempLoopP == cMaximumEventLogFiles)
				ReportErrorMessage("More than " + dtoa(cMaximumEventLogFiles, 0) + " Event Logs on " + TempCurrentFileDayString, cEMailInspx, 32000);
			
//				if(TempFileNameString == "")
//				{
//					TempFilePathString = TempFileDirectoryString + "ScanTracEventLog" + TempDateString 
//					+ " " + TempTimeString + ".txt";
//					TempFileNameString = TempFilePathString.GetBuffer(TempFilePathString.GetLength());
//				}

		}
		FindClose(hFind);
	}

	if (TempFoundNextFile)
	{
		ShowErrorLog(0);
		SetDlgItemText(IDC_HelpTitle, "Log Started: " + TempTitleDateString);
		m_Function3Button.ShowWindow(SW_SHOW);
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nNo Next Log.\nThis is Today's Log.";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}

	if (!TempNextFilesExist)
	{
		m_Function5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
		vShowUpdateButton = true;
	}
}

void CHelpDialog::ShowErrorLog(bool TempDoRefresh)
{
	m_HelpTextControl.SetSelectionCharFormat(vBlackCharacterFormat);
	this->SetWindowText(_T("Event Log"));

	if ((vWindowType == cErrorLogWindow) && (vErrorMessageDisplayMask == cInspxLogMask))
	{  //inspx log
		vShowedInspxMessages = true;
		vDialogTitleString = "Inspx Log";
		SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
		m_HelpTextControl.LineScroll(-9999);
		CString TempErrorString = "";
		long TempCount = vLocalSystemData->vInspxLog.GetCount();
		if (TempCount)
		{
			bool TempHaveErrorsToShow = false;
			CStringBufferObject *TempStringObject = NULL;
			POSITION TempPosition = vLocalSystemData->vInspxLog.GetHeadPosition();
			for (long TempLoop = 0; TempLoop < TempCount; TempLoop++)
			{
				if (TempPosition)
					TempStringObject = vLocalSystemData->vInspxLog.GetNext(TempPosition);
				if (TempStringObject)
				{
					if (TempStringObject->vTheString)
					{
						if (TempLoop)  //add line feed except first line
							TempErrorString = TempErrorString + "\n" + TempStringObject->vTheString;
						else  //first line gets no line feed
							TempErrorString = TempErrorString + TempStringObject->vTheString;
						TempHaveErrorsToShow = true;
					}
				}
			}
			if (!TempHaveErrorsToShow)
				TempErrorString = "No Inspx Events to display.";
		}
		else
			TempErrorString = "No Inspx Events to display.";

		CreateColorDataStructure();

		int TempEndCount = TempErrorString.GetLength();
		for (int TempPosition = 0; TempPosition < TempEndCount - 5; TempPosition++)
		{
			wchar_t TempChar = TempErrorString.GetAt(TempPosition);
			wchar_t TempNextChar = TempErrorString.GetAt(TempPosition + 1);
			if ((TempChar == '+') || ((TempChar == '-') && (TempNextChar == '-')))
			{
				int TempStop = TempErrorString.Find((char)0x0A,TempPosition);
				if ((TempStop > TempEndCount) || (TempStop < 0) || (TempStop >= cSizeOfVColor))
					TempStop = TempEndCount;
				else
				{
					for (int TempOffset = TempPosition; TempOffset <= TempStop; TempOffset++)
					if (TempOffset < cSizeOfVColor)
						vColor[TempOffset] = 3; //green
				}
				TempPosition = TempStop;
			}
			else 
			if (TempChar == '^')
			{
				int TempStop = TempErrorString.Find((char)0x0A,TempPosition);
				if ((TempStop > TempEndCount) || (TempStop < 0) || (TempStop >= cSizeOfVColor))
					TempStop = TempEndCount;
				else
				{
					for (int TempOffset = TempPosition; TempOffset <= TempStop; TempOffset++)
					if (TempOffset < cSizeOfVColor)
						vColor[TempOffset] = 2; //blue
				}
				TempPosition = TempStop;
			}
			else 
			if (TempChar == '*')
			{
				int TempStop = TempErrorString.Find((char)0x0A,TempPosition);
				if ((TempStop > TempEndCount) || (TempStop < 0) || (TempStop >= cSizeOfVColor))
					TempStop = TempEndCount;
				else
				{
					for (int TempOffset = TempPosition; TempOffset <= TempStop; TempOffset++)
					if (TempOffset < cSizeOfVColor)
						vColor[TempOffset] = 1; //red
				}
				TempPosition = TempStop;
			}
		}
		TempErrorString = TempErrorString + "\n\n";
		SetDlgItemText(IDC_HelpText,TempErrorString);
		DisplayColor(&TempErrorString);
//		long vTimerResult = SetTimer(vHideCaretTimerHandle,10,NULL);
		m_SubFunction2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
		UpdateButtons();
	}
	else  //normal event log
	{
		CString TempFileDirectoryString = cScanTracEventLogDirectory;
		CString TempDateString = "", TempFilePathString = ""; 
		long TempEventLogCurrentFileIndex = -1;
		long TempMaxFileIndex = -1;
		long i = 0;
		bool TempPreviousFilesExist = false;

		vEventLogString = "";
		vFirstLineNumberOnEventLogScreen = 0;

		if (PasswordOK(cTemporaryInspxPassword,false))
			vShowedInspxMessages = true;
		if ((vErrorMessageDisplayMask & 0x184) == 0x080)
			vDialogTitleString = "Reject Log";
		else
		{
			vDialogTitleString = "Event Log";
		}
		SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);

		if (PasswordOK(cTemporaryInspxPassword,false))
			vErrorMessageDisplayMask = vErrorMessageDisplayMask | 0x0010;
		else
			vErrorMessageDisplayMask = vErrorMessageDisplayMask & 0xFDEF;

		m_HelpTextControl.LineScroll(-9999);
		char TempChars[20];
		for( i; i < 20; i++)
			TempChars[i] = 0;

		vLineIndexInFile[0] = 1;
		for (i = 0; i < 1000; i++)
			vLineIndexInFile[i] = 0;
		
		if (TempDoRefresh)
		{	//find files to read from
			vLinesInCurrentDayEventLog = -1;
			vLastFileIndex = 0;
			vShowUpdateButton = true;
			m_SubFunction2Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());

			for (i = 0; i < cMaximumEventLogFiles; i++)
				vEventLogCurrentFiles[i] = "";

			WIN32_FIND_DATA FindFileData;
			HANDLE hFind;

			TempFileDirectoryString.Delete(TempFileDirectoryString.GetLength() -1, 1);
			LPTSTR TempFileNameString = TempFileDirectoryString.GetBuffer(TempFileDirectoryString.GetLength()); //+ "\\ScanTracEventLog2008-07-14_12-34-08.txt";
			CFileStatus TempFileStatus;
			CTime TempTime = CTime::GetCurrentTime();
			if (CFile::GetStatus(TempFileNameString, TempFileStatus))
			{
				ULONGLONG TempFileSize = TempFileStatus.m_size;

				TempDateString = TempTime.Format("%Y-%m-%d");

				//look for old event log under 2Mb
				TempFileDirectoryString = TempFileDirectoryString + "\\";
				CString TempFilePathString = TempFileDirectoryString + "ScanTracEventLog" + TempDateString 
					+ "*" + ".txt";

				LPTSTR TempDirectoryFiles = TempFilePathString.GetBuffer(TempFilePathString.GetLength());
				
				hFind = FindFirstFile(TempDirectoryFiles, &FindFileData);
				
				if (hFind == INVALID_HANDLE_VALUE) // no files in directory
				{

					//if no old files found, 
					TempEventLogCurrentFileIndex = -1;
					

				} 
				else	//else scan through files to see one less than 2MB
				{
					for (i = 0; i < cMaximumEventLogFiles; i++)
					{
						TempFilePathString = FindFileData.cFileName;
						if (i == 0)
						{
							CString TempString = TempFilePathString;
							if (TempString.GetLength() > 15)
								TempString.Delete(0,16);
							if (TempString.GetLength() > 22)
								TempString.Delete(19,4);
							SetDlgItemText(IDC_HelpTitle,"Log Started: " + TempString);
						}
						TempFilePathString = TempFileDirectoryString + TempFilePathString;
							
						LPTSTR TempFileNameString = TempFilePathString.GetBuffer(TempFilePathString.GetLength());
						CFile::GetStatus(TempFileNameString, TempFileStatus);
						ULONGLONG TempFileSize = TempFileStatus.m_size;
						if (TempFileSize > 0)
						{
							vEventLogCurrentFiles[i] = TempFilePathString;
						}

						if (!FindNextFile(hFind,&FindFileData))
							break;
					} 
					TempMaxFileIndex = TempEventLogCurrentFileIndex = i;
					if (i == cMaximumEventLogFiles)
						ReportErrorMessage("More than " + dtoa(cMaximumEventLogFiles, 0) + " Event Logs today " + TempDateString, cEMailInspx, 32000);
				}

				TempFilePathString = TempFileDirectoryString + "ScanTracEventLog" + "*" + ".txt";

				TempDirectoryFiles = TempFilePathString.GetBuffer(TempFilePathString.GetLength());
				
				hFind = FindFirstFile(TempDirectoryFiles, &FindFileData);
				
				if (i <= cMaximumEventLogFiles)
					i = cMaximumEventLogFiles - 1;
				if (hFind != INVALID_HANDLE_VALUE) // no files in directory
					if (FindFileData.cFileName != vEventLogCurrentFiles[i])
						TempPreviousFilesExist = true;
			}
			FindClose(hFind);

			if (TempPreviousFilesExist)
			{
				m_Function3Button.ShowWindow(SW_SHOW);
			}
			m_Function5Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
			SetDlgItemText(IDC_Function3Button, _T("Previous Day's Log"));
			m_Function5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_Function5Button, _T("Next\nDay's Log"));
		}
		else
		{
			long TempLoopS = cMaximumEventLogFiles - 1;
			for (; TempLoopS > -1; TempLoopS--)
			{
				if (vEventLogCurrentFiles[TempLoopS] == "")
					continue;
				else
					break;
			}
			TempMaxFileIndex = TempEventLogCurrentFileIndex = TempLoopS;
		}

		bool TempDoneShowingDataInAllFiles = false;
		long TempNumberOfLinesShown = 0;
		long TempNumberOfCharactersShown = 0;

		//sets all colors to black
		CreateColorDataStructure();

		while (!TempDoneShowingDataInAllFiles && (TempNumberOfLinesShown < cErrorLogBufferSize))
		{
			ULONGLONG TempStartIndex = 0;
			ULONGLONG TempEndIndex = 0;
			ULONGLONG TempSizeOfErrorLog = 0;
		// where is this array allocated kjh 8/21/13 onShow()
			vLineIndexInFile[0] = 1;

			if (TempEventLogCurrentFileIndex >= cMaximumEventLogFiles)
			{
				CNoticeDialog TempNoticeDialog;
				CString TempAString = "\n\n\nToo many log files for today.\nWill only show " + dtoa(cMaximumEventLogFiles, 0);
				TempNoticeDialog.vNoticeText = TempAString;
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.DoModal();

				TempEventLogCurrentFileIndex = cMaximumEventLogFiles - 1;
			}

			if (TempEventLogCurrentFileIndex != -1)
			{
				TempFilePathString = vEventLogCurrentFiles[TempEventLogCurrentFileIndex];
				//CString TempHoldNextOrPreviousLogString = "Loading Previous Day's Log File" or "Loading Next Day's Log File"
				CString TempAString;
				TempAString.Format(_T(" %d of %d. . ."),((TempMaxFileIndex - TempEventLogCurrentFileIndex) + 1),(TempMaxFileIndex + 1));
				TempAString = TempHoldNextOrPreviousLogString + TempAString;
				m_HelpTextControl.SetWindowText(TempAString);
				if (!vGlobalDisplayDialogThread)
				{
					vGlobalDisplayDialogThread = (CDisplayDialogThread*)AfxBeginThread(RUNTIME_CLASS(CDisplayDialogThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
					if (!vGlobalDisplayDialogThread)
						ReportErrorMessage("Error-Could Not Create Display Dialog Thread", cEMailInspx,32000); 
					else
					{
						vGlobalDisplayDialogThread->vMessageToDisplay = "\n\n\nLoading Event Log. . .";
						vGlobalDisplayDialogThread->ResumeThread();
					}
				}
				else
				if (vGlobalDisplayDialogThread)
					vGlobalDisplayDialogThread->NextStep();
			}
			else
				break;

		//		TempFilePathString = TempFileDirectoryString + "ScanTracEventLog2008-08-07 11-48-42.txt"; //for debug only
				
			LPTSTR TempFileNameString = TempFilePathString.GetBuffer(TempFilePathString.GetLength());
			if (TempFileNameString != 0)
			{
				TRY
				{
					CFileException TempFileException;
					if (ThereIsEnoughMemory(sizeof(CFile), "Event Log File"))
					{
						CFile *TempErrorLogFile = new CFile;
						if (TempErrorLogFile->Open(TempFileNameString, (CFile::modeRead | CFile::shareDenyNone), &TempFileException))
						{
							if (TempEventLogCurrentFileIndex == TempMaxFileIndex) 
							{
								if ( TempDoRefresh || (vLinesInCurrentDayEventLog == -1)) //also vLastFileIndex should be -1 when refreshing
								{
									TempErrorLogFile->SeekToBegin();
									TempSizeOfErrorLog = TempErrorLogFile->SeekToEnd();
									vLastFileIndex = TempSizeOfErrorLog + TempEventLogCurrentFileIndex * 10000000;
								//	m_SubFunction2Button.ShowWindow(SW_HIDE);
								//	m_ArrowLeft2.ShowWindow(SW_HIDE);
								}
								else
									TempSizeOfErrorLog = vLastFileIndex % 10000000;
							}
							else
								TempSizeOfErrorLog = TempErrorLogFile->SeekToEnd();
							
							ULONGLONG TempAmountOfDataToProcess = TempSizeOfErrorLog;
							bool TempDone = false;
							long TempNumberOfNewLines = 0;
							
							bool TempSkipNextDash = false;
							
							CString TempErrorString = "";
							TempEndIndex = TempSizeOfErrorLog;
							TempStartIndex = TempEndIndex;
							ULONGLONG TempIndexs[100];
							ULONGLONG i = 0;

							for (i=0; i < 100; i++)
								TempIndexs[i] = 0;

							//2009/11/12
							DWORD TempLoopCounter = 0;
								
							while ((TempDone == false) && ( TempNumberOfLinesShown < cErrorLogBufferSize))
							{
								if (TempStartIndex < 583000)
									int x = 0;

								if (TempStartIndex > 1000)
								{
									TempEndIndex = TempStartIndex;									
									TempAmountOfDataToProcess = 1000;
									TempStartIndex = TempErrorLogFile->Seek(TempEndIndex - 1000,CFile::begin);
									i = 0;
									TempChars[0] = 0;
									//Jessica got stuck in an infinite loop so I added second condition
									while ((TempChars[0] != '\n') && (TempAmountOfDataToProcess > 0))//move file pointer to the start of a line
									{
										i++;
										TempErrorLogFile->Read(TempChars,1);
										TempAmountOfDataToProcess--;
									}
									
									TempStartIndex = TempErrorLogFile->Seek(0, CFile::current);
									vMaxEventLogScrollUp = true;
									vMaxEventLogScrollDown = false;

									//jessica, I added this as it got stuck in an infinte loop
									if (TempStartIndex == TempEndIndex)
										TempDone = true;
								}
								else
								{
									TempAmountOfDataToProcess = TempStartIndex;
									vMaxEventLogScrollDown = true;
									vMaxEventLogScrollUp = true;
									TempEndIndex = TempSizeOfErrorLog;
									TempErrorLogFile->SeekToBegin();
									TempStartIndex = 0;
									TempDone = true;
								}

								long TempLineType = -1;
								CString TempLine = "";
								CString TempLineToIgnore = "";
								long TempSplitNumber = 0;
								bool TempCopyThisLine = false;
								bool TempIgnoreThisLine = false;

								for (i = 0; i < TempAmountOfDataToProcess; i++)  //copy appropriate data into lines to display
								{
									if (i % 20 == 0)  //need to read in more characters from file
									{
										if ((TempAmountOfDataToProcess - i) >= 20)
										{
											if (TempErrorLogFile->Read(TempChars,20) != 20)
												break;  //unexpected amount of characters
										}
										else
										{	//kjh???
											if ((TempErrorLogFile->Read(TempChars, (UINT)(TempAmountOfDataToProcess - i))) != (TempAmountOfDataToProcess - i))
											{
												TempDone = true;
												break; //unexpected amount of characters
											}
										}
									}
									if ((TempIgnoreThisLine) || (TempCopyThisLine))
									{
										if ((TempChars[i%20] == 13 ) || (TempChars[i%20] == 10)) //finished this line
										{
											if ((TempCopyThisLine) && (TempLine != "")) //finished line and it was not blank and to be shown
											{
												if (TempLineType == cUserChanges) 
												{
													TempLine = '^' + TempLine; //user changes are blue
												}
												else 
												if ((TempLineType == cCritical) || (TempLineType == cError) || (TempLineType == cEMailPlantOperations) || (TempLineType == cEMailMaintenance))//(1 << TempLineType & 0x2B) //0x2B is bitmask for just error mesgs
												{
													TempLine = '*' + TempLine; //errors are red
												}
												else 
												if (TempLineType == cEMailInspx)
												{
													TempLine = '+' + TempLine;	//inspx messages are green
												}
											
												TempErrorString = TempLine + '\n' + TempErrorString; //add this line to text to display
										
												ULONGLONG TempIndex = TempStartIndex + i + TempEventLogCurrentFileIndex * 10000000;
											
												TempIndexs[TempNumberOfNewLines - 1] = TempIndex;
												TempLine = "";
											}
											else
												TempLineToIgnore = "";  //just to see what it is for debugging purposes

											TempLineType = -1; //clear what kind of line so start looking for next line
											TempSplitNumber = 0;
											TempCopyThisLine = false;
											TempIgnoreThisLine = false;
										}
										else
										if (TempCopyThisLine)
										{
											TempLine = TempLine + TempChars[i%20];
										}
										else
											TempLineToIgnore = TempLineToIgnore + TempChars[i%20];
									}
									else
									if (TempLineType == -1) //line type not yet found
									{
										if (TempSplitNumber != 0)
										{
											if (TempChars[i%20] == '>') //had number split from before >
											{
												TempLineType = TempSplitNumber; 
												TempSplitNumber = 0;
											}
											else
											if ((TempChars[i%20] >= '0') && (TempChars[i%20] <= '9')) //another digit to add to the split number
											{
												TempSplitNumber = (TempSplitNumber * 10) + _wtoi((CString)TempChars[i%20]); 
											}
											else
											{
												TempSplitNumber = 0;
												//ReportErrorMessage("Did not find Event Log Line Type from split type number", cEMailInspx, 32000);
											}
										}//end of finding 2nd half of split number
										else //not a split number already
										{
											if ((TempChars[i%20] >= '0') && (TempChars[i%20] <= '9'))
											{
												TempSplitNumber = _wtoi((CString)TempChars[i%20]);  //got a type value, add to splitNumber so can check the next character
											}
											//else
											//if (TempChars[i%20] != 10)
											//{
											//	ReportErrorMessage("Unknown Event Log Character: " + (CString)TempChars[i%20], cEMailInspx, 32000);
											//}
										}

										if (TempLineType != -1) //found line type for this line
										{
											if ((1 << TempLineType) & (vErrorMessageDisplayMask))
											{
												TempNumberOfNewLines++;
												TempCopyThisLine = true;
											}
											else
												TempIgnoreThisLine = true;
										} //end of found line type
									} //end of looking for line type
								} //for data to process
								for (i = 0; i < TempNumberOfNewLines; i++)
								{
									long TempLineStartIndex = 0;
									
									TempLineStartIndex = TempErrorString.Find('\n', 0) + 1;

									CString TempOneLineString = TempErrorString.Left(TempLineStartIndex);
									TempErrorString.Delete(0,TempLineStartIndex);
									if (TempOneLineString.GetLength() > 0)
									{
										char TempColorCharacter = 0;
										TempNumberOfLinesShown++;

										for (long j = 0; j < 7; j++)
										{
											if (TempOneLineString.GetLength() > 0)
											{
												wchar_t TempChar = TempOneLineString.GetAt(0);
												if (TempChar == '^')
												{
													TempColorCharacter = '^';
													TempOneLineString.Delete(0,1);
												}
												else 
												if (TempChar == '+')
												{
													TempColorCharacter = '+';
													TempOneLineString.Delete(0,1);
												}
												else 
												if (TempChar == '*')
												{
													TempColorCharacter = '*';
													TempOneLineString.Delete(0,1);
												}
												else 
												if ((TempChar >= '0') && (TempChar <= '9'))
													TempOneLineString.Delete(0,1);
												else
													break;
											}
											else
												break;
										}
										TempOneLineString = dtoa(TempNumberOfLinesShown) + TempOneLineString;
										if (TempColorCharacter)
										{
											long TempEndCount = TempNumberOfCharactersShown + TempOneLineString.GetLength();
											if (TempEndCount >= cSizeOfVColor)
											{
												//ReportErrorMessage("Log greater than cSizeOfVColor characters, so ignore color beyond cSizeOfVColor characters", cEMailInspx, 32000);
											}
											else
											for (; TempNumberOfCharactersShown < TempEndCount; TempNumberOfCharactersShown++)
											{
												if (TempColorCharacter == '+')
												{
													if (TempNumberOfCharactersShown < cSizeOfVColor)
														vColor[TempNumberOfCharactersShown] = 3; //green
												}
												else 
												if (TempColorCharacter == '*')
												{
													if (TempNumberOfCharactersShown < cSizeOfVColor)
														vColor[TempNumberOfCharactersShown] = 1; //red
												}
												else 
												if (TempColorCharacter == '^')
												{
													if (TempNumberOfCharactersShown < cSizeOfVColor)
														vColor[TempNumberOfCharactersShown] = 2; //blue
												}
											}
										}
										else
										{
											TempNumberOfCharactersShown = TempNumberOfCharactersShown + TempOneLineString.GetLength();
										}

										vEventLogString = vEventLogString + TempOneLineString;

										vLineIndexInFile[vLineIndexInFile[0]] = TempIndexs[(TempNumberOfNewLines - i) - 1];
										vLineIndexInFile[0] = (vLineIndexInFile[0] % cErrorLogBufferSize) + 1;
										TempIndexs[(TempNumberOfNewLines - i) - 1] = 0;

										if (TempNumberOfLinesShown == cErrorLogBufferSize)
											break;
									}
								}
								TempNumberOfNewLines = 0;
								TempErrorString = "";

								TempLoopCounter++;
								if ((TempLoopCounter % 1000) == 0)
									Sleep(1); //yeild thread
							}
							TempErrorLogFile->Close();
							Sleep(10); 
						}
						else
							ReportErrorMessage("Failed to Open Event Log File to Read 4 - File not opened",cEMailInspx, 32000);

						delete TempErrorLogFile;
					}
					else
						ReportErrorMessage("Failed to Open Event Log File to Read 4 - Out of memory",cEMailInspx, 32000);
				}
				CATCH_ALL(TempFileException)
				{
					ReportErrorMessage("Failed to Open Event Log File to Read 4 - Exception",cEMailInspx, 32000);

					throw;
				}
				END_CATCH_ALL
			}
			else
				ReportErrorMessage("Failed to Open Event Log File to Read 4 - Name blank",cEMailInspx, 32000);

			TempEventLogCurrentFileIndex--;
		}

		vEventLogString = vEventLogString + "\n\n";

		int TempStringLength = vEventLogString.GetLength();
		if ( vEventLogString == "")
			vEventLogString = "No Events Today with Current Filter Settings.";
		
		vEventLogString = vEventLogString + "\n\n";
		SetDlgItemText(IDC_HelpText,vEventLogString);
		
		DisplayColor(&vEventLogString);
		UpdateButtons();
	}
	if (vGlobalDisplayDialogThread)
	{
		vGlobalDisplayDialogThread->ExitDisplayThread();
		vGlobalDisplayDialogThread = NULL;
	}
}

void CHelpDialog::UpdateButtons()
{
	int TempCurrentLine = m_HelpTextControl.GetFirstVisibleLine();
	int TempTotalLines = m_HelpTextControl.GetLineCount();

	if (vWindowType == cMainHelpWindow)
	{
		m_Function1Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		m_Function1Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_Function1Button, _T("Exit Help"));

		m_Function2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			m_Function3Button.ShowWindow(SW_SHOW);
		}
		else
		{
			m_Function3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
		}

		m_Function4Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());
		m_Function5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());

		m_SubFunction1Button.ShowWindow(SW_SHOW);
		m_SubFunction1Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		SetDlgItemText(IDC_SubFunction1Button, _T("Chat"));

		m_SubFunction2Button.ShowWindow(SW_SHOW);
		m_SubFunction2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_SubFunction2Button, _T("Comment For Event Log"));

		m_SubFunction3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());

		m_SubFunction4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction4Button, _T("Keyboard Function Keys"));

		m_SubFunction5Button.ShowWindow(SW_SHOW);
		m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_SubFunction5Button, _T("Command Finder"));
		m_SubFunction6Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
		m_SubFunction7Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
		m_SubFunction8Button.ShowWindow(SW_SHOW);
		m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_SubFunction8Button, _T("Help on Current Menu"));

		//if (PasswordOK(cTemporaryInspxPassword,false))
		//{
			m_Function3Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			m_Function3Button.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_Function3Button, _T("Diagnostics"));
		//}
		//else
		//{
		//	m_Function3Button.ShowWindow(SW_HIDE);
		//	m_ArrowDown3.ShowWindow(SW_HIDE);
		//}
		m_SubFunction5Button.ShowNavigationArrow(NULL);
		m_SubFunction6Button.ShowNavigationArrow(NULL);
		m_SubFunction7Button.ShowNavigationArrow(NULL);
		m_SubFunction8Button.ShowNavigationArrow(NULL);
	}
	else
	if (vWindowType != cRemoteKeyLabelWindow)
	{
		if (TempCurrentLine > 0)
		{  //show up arrows if first time moved down
			m_SubFunction5Button.ShowNavigationArrow(cUpArrowBig);
			m_SubFunction6Button.ShowNavigationArrow(cUpArrowSmall);
			m_SubFunction5Button.ShowWindow(SW_SHOW);
			m_SubFunction6Button.ShowWindow(SW_SHOW);
			m_Function4Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			SetDlgItemText(IDC_Function4Button, _T("Goto Top"));
		}
		else
		{  //show up arrows if first time moved down
			m_SubFunction5Button.ShowNavigationArrow(NULL);
			m_SubFunction6Button.ShowNavigationArrow(NULL);
			m_SubFunction5Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
			m_SubFunction6Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
			m_Function4Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);

			CString TempText = _T("-");
			TempText.LoadString(IDS_GotoBottom);
			SetDlgItemText(IDC_Function4Button, TempText);
		}
		
		if (TempTotalLines > (vLinesOnScreen+1))
		{
			m_Function4Button.ShowWindow(SW_SHOW);
		}
		else
		{
			m_Function4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());
			m_SubFunction7Button.ShowNavigationArrow(NULL);
			m_SubFunction8Button.ShowNavigationArrow(NULL);
			m_SubFunction7Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
			m_SubFunction8Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
		}
		if (TempCurrentLine + 1 + vLinesOnScreen >= TempTotalLines)
		{  //show up arrows if first time moved down
			m_SubFunction7Button.ShowNavigationArrow(NULL);
			m_SubFunction8Button.ShowNavigationArrow(NULL);
			m_SubFunction7Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
			m_SubFunction8Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
		}
		else
		{
			m_SubFunction7Button.ShowWindow(SW_SHOW);
			m_SubFunction8Button.ShowWindow(SW_SHOW);
			m_SubFunction7Button.ShowNavigationArrow(cDownArrowSmall);
			m_SubFunction8Button.ShowNavigationArrow(cDownArrowBig);
		}
	}
	if (vWindowType == cErrorLogWindow)
	{
// kjh 7/21/10		m_Function2Button.ShowWindow(SW_HIDE);
//  kjh 7/21/10		m_ArrowDown2.ShowWindow(SW_HIDE);
		m_SubFunction4Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());

		if (vErrorMessageDisplayMask == cInspxLogMask)
		{
			m_SubFunction1Button.ShowWindow(SW_SHOW);
			m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction1Button, _T("Comment For Inspx Log"));
		}
		else
		{
			m_SubFunction1Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
		}
	}
	else
	if (vWindowType == cProductionReportWindow)
	{
		SetDlgItemText(IDC_SubFunction1Button, _T("Generate New Report"));
	//	m_Function5Button.ShowWindow(SW_HIDE);
	//	m_ArrowDown5.ShowWindow(SW_HIDE);
	}
	else
	if (vWindowType == cHelpWindow)
	{
		CString TempString("");
		CString TempString1("");
		m_Function1Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		m_Function1Button.ShowWindow(SW_SHOW);
		TempString.LoadString(IDS_ExitHelp);
		SetDlgItemText(IDC_Function1Button, TempString);

		m_Function2Button.ShowWindow(SW_SHOW);
		m_Function2Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		TempString.LoadString(IDS_Exit);
		SetDlgItemText(IDC_Function2Button, TempString);

		m_Function3Button.ShowWindow(SW_SHOW);
		m_Function3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		TempString.LoadString(IDS_HelpTitle0);
		//SetDlgItemText(IDC_Function3Button, _T("Using ScanTrac"));
		SetDlgItemText(IDC_Function3Button, TempString);

		m_Function5Button.ShowWindow(SW_SHOW);
		m_Function5Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		TempString.LoadString(IDS_HelpIndex);
		SetDlgItemText(IDC_Function5Button, TempString);

		m_SubFunction1Button.ShowWindow(SW_SHOW);
		TempString.LoadString(IDS_Functions);
		SetDlgItemText(IDC_SubFunction1Button, TempString);

		m_SubFunction2Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		m_SubFunction2Button.ShowWindow(SW_SHOW);
		TempString.LoadString(IDS_More);
		SetDlgItemText(IDC_SubFunction2Button, TempString);

		m_SubFunction3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_SubFunction3Button.ShowWindow(SW_SHOW);
		TempString.LoadString(IDS_HintsOnUsingScanTrac);
		SetDlgItemText(IDC_SubFunction3Button, TempString);

		m_SubFunction4Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		TempString.LoadString(IDS_Overview);
		SetDlgItemText(IDC_SubFunction4Button, TempString);

		SetDlgItemText(IDC_SubFunction5Button, _T(""));
		SetDlgItemText(IDC_SubFunction6Button, _T(""));
		SetDlgItemText(IDC_SubFunction7Button, _T(""));
		SetDlgItemText(IDC_SubFunction8Button, _T(""));
	}
	else
	if (vWindowType == cSourceStrengthWindow)
	{
		if ((vLocalConfigurationData) && ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cUpSideDownSoloScanTrac) || 
			(vLocalConfigurationData->vScanTracType == cFermataScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)) && (PasswordOK(cTemporaryInspxPassword,false)))
		{
			m_Function3Button.ShowWindow(SW_SHOW);

			if (vSourceStrengthDisplayMode == cDisplayRawValues)
				SetDlgItemText(IDC_Function3Button, _T("Show % of Initial"));
			else
			if (vSourceStrengthDisplayMode == cDisplayPercentOfInitial)
				SetDlgItemText(IDC_Function3Button, _T("Show % of Standard"));
			else
				SetDlgItemText(IDC_Function3Button, _T("Show Raw Values"));
		}
		else
		{
			m_Function3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
		}

		if (vLocalSystemData->vPixelValuesString == "")
		{
			m_Function5Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
		}
		else
		{
			m_SubFunction4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_Function5Button, _T("Show Pixel Values"));
			m_Function5Button.ShowWindow(SW_SHOW);
		}
	}
	else
	if (vWindowType == cTextWindow)
	{
		if (vHistogramData)
		{
			SetDlgItemText(IDC_Function3Button, _T("Histogram\nImage ROI"));
			m_Function3Button.ShowWindow(SW_SHOW);
		}
		m_Function5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			SetDlgItemText(IDC_Function2Button, _T("Write To File"));
			m_Function2Button.ShowWindow(SW_SHOW);
		}
		else
		{
			m_Function2Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());
		}
	}
	else
	{
		SetDlgItemText(IDC_Function5Button, _T(""));
		m_Function5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
	}

	if (vWindowType == cRemoteKeyLabelWindow)
	{
		m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_Function1Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_Function1Button, _T("F7\nExit"));

		m_Function2Button.ShowWindow(SW_SHOW);
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_Function2Button, _T("/\n(Key Pad)"));

		m_Function3Button.ShowWindow(SW_SHOW);
		m_Function3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_Function3Button, _T("-\n(Key Pad)"));

		m_Function4Button.ShowWindow(SW_SHOW);
		m_Function4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_Function4Button, _T("*\n(Key Pad)"));

		m_Function5Button.ShowWindow(SW_SHOW);
		m_Function5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_Function5Button, _T(" +\n(Key Pad)"));

		m_SubFunction1Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction1Button, _T("F5"));

		m_SubFunction2Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		m_SubFunction2Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction2Button, _T("F9"));

		m_SubFunction3Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		m_SubFunction3Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction3Button, _T("F11"));

		m_SubFunction4Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction4Button, _T("F6"));

		m_SubFunction5Button.ShowWindow(SW_SHOW);
		m_SubFunction5Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		SetDlgItemText(IDC_SubFunction5Button, _T("F2"));

		m_SubFunction6Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction6Button, _T("F4"));

		m_SubFunction7Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction7Button, _T("F3"));

		m_SubFunction8Button.ShowWindow(SW_SHOW);
		m_SubFunction8Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		SetDlgItemText(IDC_SubFunction8Button, _T("F8"));

		m_HelpTitle.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_HelpTitle, _T("Remote Control Keyboard Keys"));

		SetDlgItemText(IDC_HelpText, _T("Computer Keypad Key >> ScanTrac Input\n\n                Page Down >> Help (all menus)\n                     Page Up >> Clear (data entry only)\n                         Home >> Screen Capture\n                           End >> Image (Left/Right)\n\n                                 1 >> Space,1 (alphanumeric entry only)\n                                 2 >> A,B,C,2\n                                 3 >> D,E,F,3\n                                 4 >> G,H,I,4\n                                 5 >> J,K,L,5\n                                 6 >> M,N,O,6\n                                 7 >> P,Q,R,7\n                                 8 >> S,T,U,8\n                                 9 >> V,W,X,9\n                                 0 >> Y,Z,0\n\n\nNote: Must have NumLock on for keypad number to function."));
	}
	this->Invalidate(false);
}

void CHelpDialog::ShowProductionReport()
{
	//note: the new lines in the production report are put in before 'total containers' 
	//	and again after any header with the letters cpm in them.
	//	product will always be the first entry.

	char TempFieldSeperator = ',';
	if (vGlobalUseCommaForDecimalPoint)
		TempFieldSeperator = ';';

	m_HelpTextControl.SetSelectionCharFormat(vBlackCharacterFormat);
	this->SetWindowText(_T("Production Report"));
	//m_HelpTextControl.SetDefaultCharFormat(vOriginalCharacterFormat);

	vDialogTitleString = "Production Report";

	m_Function5Button.ShowWindow(SW_SHOW);
	SetDlgItemText(IDC_SubFunction6Button, _T("F4"));
	m_Function3Button.ShowWindow(SW_SHOW);
	SetDlgItemText(IDC_Function3Button, _T("Previous Report"));
	SetDlgItemText(IDC_Function5Button, _T("Next\nReport"));
	
	
	//replace this
	SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
	//with the string from the file

	CString TempLastReportString = "";
	CString TempEndTimeString = "";
	CFileException TempFileException;
	CString TempFilePath = cScanTracProductionReportDirectory;
	TempFilePath += "ScanTracProductionReportSummary.CSV";
	LPCTSTR TempFileName = TempFilePath;

	if (ThereIsEnoughMemory(sizeof(CFile), "Serial Data Log File2"))
	{
		CFile TempProductionReportFile;
		if (TempProductionReportFile)
		if (TempProductionReportFile.Open(TempFileName, (CFile::modeRead | CFile::shareDenyNone), &TempFileException))
		{
			UINT TempLength;
			int i = 0;
			int TempDataCount = 0;
			ULONGLONG TempFileIndex = 0;
			char TempChars[20];
			CString TempProductionReportDataString = "";
			vProductionReportHeaderString = " ";

			vProductDataIndex = 0;
			vHeaderCount = 1;
			vEndTimeIndex = 0;

			bool TempThisIsTheFirstLoop = true;
			TempFileIndex = TempProductionReportFile.SeekToEnd();
			bool TempReadEndOfFile = false;
			ULONGLONG TempFileSize = 0;
			ULONGLONG TempReadCount = 0;
			CFileStatus TempStatus;
			if (CFile::GetStatus(TempFileName, TempStatus))
			 TempFileSize = TempStatus.m_size;

			TempLength = TempProductionReportFile.Read(TempChars,10);

			TempReadCount = TempReadCount + TempLength;

			while ((vProductionReportHeaderString.Find(_T("Product Code")) == -1) && (!TempReadEndOfFile) && (TempReadCount < TempFileSize))
			{
				int TempIndexDebug = vProductionReportHeaderString.Find(_T("Product Code"));
				vProductionReportHeaderString = " ";
				if (TempFileIndex == 0)
					TempReadEndOfFile = true;

				for (i++; i < 100; i++) //condition is random, the code will break out of the for loop below
				{
					if ((i%10) == 0)
					{
						i = 0;
						if (TempThisIsTheFirstLoop == true)
						{
							TempFileIndex = TempProductionReportFile.Seek(0, CFile::current);
							TempThisIsTheFirstLoop = false;

							TempLength = TempProductionReportFile.Read(TempChars,10);
							TempReadCount = TempReadCount + TempLength;
							if (TempLength < 10) //something not right, didn't get back expected amount of chars from read
								break;
						}
						else
						{
							TempFileIndex = TempProductionReportFile.Seek(0, CFile::current);

							if (TempFileIndex>=20)
							{
								TempFileIndex = TempProductionReportFile.Seek(-20, CFile::current); //must seek back 20 as read forward 10 at last step
								TempLength = TempProductionReportFile.Read(TempChars,10);
								TempReadCount = TempReadCount + TempLength;
								if (TempLength < 10) //something not right, didn't get back expected amount of chars from read
									break;
							}
							else 
							if (TempFileIndex > 10)
							{
								TempProductionReportFile.SeekToBegin();
							
								TempLength = TempProductionReportFile.Read(TempChars, (UINT)(TempFileIndex-10));
								TempReadCount = TempFileSize;

								i = (UINT)(20 - TempFileIndex);
								if ( (int) TempLength < (TempFileIndex-10)) //something not right, didn't get back expected amount of chars from read
									break;

								TempFileIndex = 0;
							}
							else
							{
								TempReadCount = TempFileSize;
								break;
							}
						}
					}
					if (TempChars[9 - i] == (char)13)
							break;

					if (TempChars[9 - i] == (char)10)
						continue;

					vProductionReportHeaderString.Insert(0,TempChars[9-i]);
				}
			}

			if (TempReadEndOfFile)
			{
				TempLastReportString = "File Corrupt, can not read properly.  File:\n";
				vMainWindowPointer->ReadInProductionReport(false);
				TempLastReportString = TempLastReportString + vMainWindowPointer->vProductionReportString;
			}
			else
			{
				for (i = 0; i < vProductionReportHeaderString.GetLength() ; i++)
				{
					if (vProductionReportHeaderString.GetAt(i) == TempFieldSeperator)
					{
						if (vEndTimeIndex == 0)
						if (vProductionReportHeaderString.Find(_T("End Time"), i) == -1) //if you just passed end time
							vEndTimeIndex = vHeaderCount;

						if (vProductDataIndex == 0) 
						{
							CString TempProductWithSeparators = _T(",Product,");
							if (vGlobalUseCommaForDecimalPoint)
								TempProductWithSeparators = _T(";Product;");

							if (vProductionReportHeaderString.Find(TempProductWithSeparators, i+1) == -1) //if you just passed product, -1 is returned when it cannot find the string anymore
							{ 
								if (vProductionReportHeaderString.GetLength() > i + 8)
									vProductionReportHeaderString.Delete(i , 8); //delete word product and the comma
								vProductDataIndex = vHeaderCount;
							}
							else
								vHeaderCount++;
						}
						else
							vHeaderCount++;
					}
				}

				vHeaderCount++;

				CString TempText45 = _T("Product");
				vProductionReportHeaderString = TempText45 + TempFieldSeperator + vProductionReportHeaderString;

				TempProductionReportDataString = " ";
				TempProductionReportFile.SeekToEnd();
				CString TempProductDataString = "";
				for (i = 0; i < 20; i++)
					TempChars[i] = 0;
				for (i = 0; i < 100; i++) //condition is random, the code will break out of the for loop below
				{
					if ((i%10) == 0)
					{
						if (TempDataCount==0)
						{
							vProductionReportEndIndex = vProductionReportStartIndex = TempProductionReportFile.Seek(-10, CFile::current);
						}
						else
							vProductionReportStartIndex = TempProductionReportFile.Seek(-20, CFile::current);

						TempLength = TempProductionReportFile.Read(TempChars,10);
						i = 0;
					}
				
					if (TempLength < 10) //something not right, didn't get back expected amount of chars from read
						break;

					if (TempChars[9 - i] == TempFieldSeperator)
					{
						TempDataCount++;
					}

					if (TempChars[9 - i] == (char)13)
					{
						if (TempDataCount > 3)
							break;
						else
						{
							TempDataCount = 0;
							TempProductionReportDataString = " ";
							continue;
						}
					}

					if (TempChars[9 - i] == (char)10)
						continue;

					TempProductionReportDataString.Insert(0,TempChars[9-i]);
				}
				if (vProductDataIndex < TempDataCount)
				{
					int TempIndexOfFirstCharacterOfProductData = 0;
					//put header into own string
					for(i = 0; i < vProductDataIndex; i++)
						TempIndexOfFirstCharacterOfProductData = TempProductionReportDataString.Find(TempFieldSeperator, TempIndexOfFirstCharacterOfProductData) + 1;
				
					int TempIndexOfLastCharacterOfProductData = TempProductionReportDataString.Find(TempFieldSeperator,TempIndexOfFirstCharacterOfProductData);
					int TempLengthOfProductDataString = TempIndexOfLastCharacterOfProductData - TempIndexOfFirstCharacterOfProductData;

					TempProductDataString = TempProductionReportDataString.Mid(TempIndexOfFirstCharacterOfProductData, TempLengthOfProductDataString);
					TempProductionReportDataString.Delete(TempIndexOfFirstCharacterOfProductData, TempLengthOfProductDataString + 1); //plus one for the extra comma

					//if no product code used on this product
//brh fix if product code					
					CString TempDash = "-";
					TempProductionReportDataString = TempFieldSeperator + TempDash + TempProductionReportDataString;
				}
				if (vEndTimeIndex < TempDataCount)
				{
					int TempIndexOfFirstCharacterOfEndTime = 0;
					//put header into own string
					for (i = 0; i < vEndTimeIndex - 1; i++)
							TempIndexOfFirstCharacterOfEndTime = TempProductionReportDataString.Find(TempFieldSeperator, TempIndexOfFirstCharacterOfEndTime) + 1;
				
					int TempIndexOfLastCharacterOfEndTime = TempProductionReportDataString.Find(TempFieldSeperator,TempIndexOfFirstCharacterOfEndTime);
					int TempLengthOfEndTimeString = TempIndexOfLastCharacterOfEndTime - TempIndexOfFirstCharacterOfEndTime;

					TempEndTimeString = " " + TempProductionReportDataString.Mid(TempIndexOfFirstCharacterOfEndTime, TempLengthOfEndTimeString);
				}

				TempProductionReportDataString = TempProductDataString + TempProductionReportDataString;
			
				int TempHeaderNextCommaIndex = 0;
				int TempDataNextCommaIndex = 0;
				CString TempHeaderCellString = "";
				CString TempDataCellString = "";
				CString TempString = vProductionReportHeaderString;
				bool TempInsertNewLineNext = false;

				for (i = 0; (i <= vHeaderCount) || (i <= TempDataCount); i++)
				{
					if (i < vHeaderCount)
					{
						TempHeaderNextCommaIndex = TempString.Find(TempFieldSeperator);
						if (TempHeaderNextCommaIndex > 0)
						{
							TempHeaderCellString = TempString.Left(TempHeaderNextCommaIndex);
							TempString.Delete(0,TempHeaderNextCommaIndex + 1);
						}

						if (TempHeaderCellString == "Total Containers")
							TempHeaderCellString = "\nTotal Containers";
						if(TempHeaderCellString.Find(_T("CPM")) != -1)
							TempInsertNewLineNext = true;

					}
					else 
					if (i == vHeaderCount)
						TempHeaderCellString = TempString;
					else
						TempHeaderCellString = "Unnamed Inspection";

					if (i < TempDataCount)
					{
						TempDataNextCommaIndex = TempProductionReportDataString.Find(TempFieldSeperator);
						if (TempDataNextCommaIndex > 0)
						{
							TempDataCellString = TempProductionReportDataString.Left(TempDataNextCommaIndex);
							TempProductionReportDataString.Delete(0, TempDataNextCommaIndex+1);
						}
					}
					else if ( i == TempDataCount )
						TempDataCellString = TempProductionReportDataString;
					else
						TempDataCellString = "Not Available";

					TempLastReportString = TempLastReportString + TempHeaderCellString
							+ " : " + TempDataCellString + "\n";

					if( TempInsertNewLineNext == true )
					{
						TempLastReportString = TempLastReportString +"\n";
						TempInsertNewLineNext = false;
					}
				}

				if (vProductionReportStartIndex < (vProductionReportHeaderString.GetLength() + 10))
				{
					m_Function3Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
				}
				if (vProductionReportEndIndex >= (TempProductionReportFile.Seek(0, CFile::end) - 10))
				{
					m_Function5Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
				}	
			}
			TempProductionReportFile.Close();
		}
		else
		{
			ReportErrorMessage("Could Not Read Production Report 3",cEMailInspx, 32000);
			TempLastReportString = "No Production Reports on This ScanTrac";
		}
	}
	TempLastReportString = TempLastReportString + "\n\n";

	SetDlgItemText(IDC_HelpTitle, "Production Report" + TempEndTimeString);
	m_HelpTextControl.LineScroll(-9999);
	SetDlgItemText(IDC_HelpText, TempLastReportString);//vLocalSystemData->vLastReport);
//	int vTimerResult = SetTimer(vHideCaretTimerHandle,10,NULL);
	UpdateButtons();

	CHARFORMAT TempCharacterFormat;
	TempCharacterFormat.dwMask = CFM_COLOR ;
	TempCharacterFormat.dwEffects = NULL;
	TempCharacterFormat.crTextColor = cBlack;
	m_HelpTextControl.SetDefaultCharFormat(TempCharacterFormat);
}

void CHelpDialog::ShowSourceStrengthData()
{
	m_HelpTextControl.SetSelectionCharFormat(vBlackCharacterFormat);
	this->SetWindowText(_T("Source Strength"));
	//m_HelpTextControl.SetDefaultCharFormat(vOriginalCharacterFormat);

	m_SubFunction1Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());

	BYTE TempNumberOf12InchSections = 1;
	if (vLocalConfigurationData)
	if (vLocalConfigurationData->vNumberOfXRayDetectors > 6)
		TempNumberOf12InchSections = 2; //case inspector and Forte

	vDialogTitleString = "X-Ray Source Strength Data";
	SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);

	vDialogTitleString = "Strength Date Time   Detector Strengths                               ";

	CString TempUnits = "";
	if (vLocalConfigurationData)
	if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cUpSideDownSoloScanTrac) || 
	(vLocalConfigurationData->vScanTracType == cFermataScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac))
	{
		if (!PasswordOK(cTemporaryInspxPassword,false))
		{
			m_HelpTitle.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			vSourceStrengthDisplayMode = cDisplayPercentOfInitial; //show percent of Max if not Inspx
			TempUnits = "%";
		}
		if (vSourceStrengthDisplayMode == cDisplayRawValues)
			vDialogTitleString = "Display in Raw Values (Replace detectors if less than 20)\n" + vDialogTitleString;
		else
		if ((vSourceStrengthDisplayMode == cDisplayPercentOfInitial) && (vLocalConfigurationData->vSourceStrengthInitialValues[0]))
		{
			vDialogTitleString = "Display in % of Initial (Replace detectors if less than 15%)\n" + vDialogTitleString;
			TempUnits = "%";
		}
		else
		if ((vSourceStrengthDisplayMode == cDisplayPercentOfStandard) || (vSourceStrengthDisplayMode == cDisplayPercentOfInitial))
		{
			vDialogTitleString = "Display in % of Standard (Replace detectors if less than 15%)\n" + vDialogTitleString;
			TempUnits = "%";
		}
		else
			vDialogTitleString = "Display Mode Invalid\n" + vDialogTitleString;

		m_HelpTitle.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	}
	else
		vSourceStrengthDisplayMode = cDisplayRawValues; //show raw values if not Solo or Fermata

	SetDlgItemText(IDC_HelpTitle,vDialogTitleString);

	CString TempString("");
	TempString = "";

	if (vSourceStrengthDisplayMode == cDisplayRawValues)
	{
		if (vLocalConfigurationData)
		for (WORD TempLoop = vLocalConfigurationData->vSourceStrengthCount; TempLoop > 0; TempLoop--)
		{
			if (TempLoop < vLocalConfigurationData->vSourceStrengthCount)
				TempString = TempString + "\n";

			TempString = TempString + Pad(dtoa(vLocalConfigurationData->vSourceStrengthData[TempLoop - 1],0),3,5) + "\t";
			TempString = TempString + vLocalConfigurationData->vSourceStrengthDate[TempLoop - 1].Format("%m/%d/%y_%H:%M");
			TempString = TempString + "    ";

			BYTE TempPosition = 0;
			if (vLocalConfigurationData->vSourceStrengthComment[TempLoop - 1].GetLength() > 0)
			for (WORD TempDetectorLoop = 0; TempDetectorLoop < vLocalConfigurationData->vNumberOfXRayDetectors; TempDetectorLoop++)
			{
				//TempPosition = 3 + (7 * (TempDetectorLoop));
				while (((vLocalConfigurationData->vSourceStrengthComment[TempLoop - 1].Mid(TempPosition, 1) < "0") || (vLocalConfigurationData->vSourceStrengthComment[TempLoop - 1].Mid(TempPosition, 1) > "9")) && 
					(vLocalConfigurationData->vSourceStrengthComment[TempLoop - 1].GetLength() > TempPosition + 2))
					TempPosition++;

				CString TempNumberString = vLocalConfigurationData->vSourceStrengthComment[TempLoop - 1].Mid(TempPosition, 4);
				double TempValue = _wtoi(TempNumberString);
				TempNumberString = Pad(dtoa(TempValue,0),3,5);
				TempString = TempString + TempNumberString;
				TempPosition = TempPosition + 3;

				if ((TempDetectorLoop == 5) && (vLocalConfigurationData->vNumberOfXRayDetectors == 12))
				TempString = TempString + "\n\t\t\t";
			}
			if (vLocalConfigurationData->vSourceStrengthComment[TempLoop - 1].GetLength() > 50)
			{
				TempPosition = vLocalConfigurationData->vSourceStrengthComment[TempLoop - 1].Find(_T("Temp"), TempPosition);
				TempPosition = vLocalConfigurationData->vSourceStrengthComment[TempLoop - 1].GetLength() - TempPosition;
				CString TempCommentString =	vLocalConfigurationData->vSourceStrengthComment[TempLoop - 1].Right(TempPosition);
				if (TempCommentString.GetLength() > 5)
				{
					if (vLocalConfigurationData->vNumberOfXRayDetectors == 9)
						TempString = TempString + "\n\t";

					TempString = TempString + TempCommentString;
				}
			}
		}
	}
	else
	if ((vLocalConfigurationData) && (vLocalConfigurationData->vSourceStrengthCount))
	{ //not raw values, so show percent of standard or initial
		//These are the absolute percent values
		BYTE TempDetectorFullReadingNoFilter[cMaximumNumberOfSourceStrengthValues21] = {190,186,155,131,98,72, 137,190,186,155,131,98,72, 137,190,186,155,131,98,72, 137};//last value is average
		BYTE TempDetectorFullReadingWithFilter[cMaximumNumberOfSourceStrengthValues21] = {193,191,167,145,112,86, 147,193,191,167,145,112,86, 147,193,191,167,145,112,86, 147};

		double TempDetectorFullReading[cMaximumNumberOfSourceStrengthValues21];
		for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfSourceStrengthValues21; TempLoop++)
			TempDetectorFullReading[TempLoop] = 0;

		if ((vSourceStrengthDisplayMode == cDisplayPercentOfInitial) && (vLocalConfigurationData->vSourceStrengthInitialValues[0]))
		{ //use initial values if mode and have them
			for (BYTE TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfXRayDetectors + TempNumberOf12InchSections; TempLoop++)
			{
				TempDetectorFullReading[TempLoop] = vLocalConfigurationData->vSourceStrengthInitialValues[TempLoop];
			}
		}
		else
		{
			for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfSourceStrengthValues21; TempLoop++)
			{
				if (vLocalConfigurationData->vXRaySourceFilter)
					TempDetectorFullReading[TempLoop] = TempDetectorFullReadingWithFilter[TempLoop];
				else
					TempDetectorFullReading[TempLoop] = TempDetectorFullReadingNoFilter[TempLoop];
			}
		}

		//display the values as a percent of the maximum value
		for (WORD TempLoop = vLocalConfigurationData->vSourceStrengthCount; TempLoop > 0; TempLoop--)
		{
			if (TempLoop < vLocalConfigurationData->vSourceStrengthCount)
				TempString = TempString + "\n";

			double TempPercent = 0;
			if (TempDetectorFullReading[6])
				TempPercent = vLocalConfigurationData->vSourceStrengthData[TempLoop - 1] / TempDetectorFullReading[vLocalConfigurationData->vNumberOfXRayDetectors] * 100.0;

			TempString = TempString + Pad(dtoa(TempPercent,0) + TempUnits,3,5);
			TempString = TempString + "\t" + vLocalConfigurationData->vSourceStrengthDate[TempLoop - 1].Format("%m/%d/%y_%H:%M");
			TempString = TempString + "    ";
			BYTE TempPosition = 0;

			if (vLocalConfigurationData->vSourceStrengthComment[TempLoop - 1].GetLength() > 0)
			for (WORD TempDetectorLoop = 0; TempDetectorLoop < vLocalConfigurationData->vNumberOfXRayDetectors; TempDetectorLoop++)
			{
				//TempPosition = 3 + (7 * (TempDetectorLoop));
				while ((vLocalConfigurationData->vSourceStrengthComment[TempLoop - 1].Mid(TempPosition, 1) == " ") && (vLocalConfigurationData->vSourceStrengthComment[TempLoop - 1].GetLength() > TempPosition + 2))
					TempPosition++;

				if (vLocalConfigurationData->vSourceStrengthComment[TempLoop - 1].GetLength() > TempPosition + 5)
				{
					CString TempNumberString = vLocalConfigurationData->vSourceStrengthComment[TempLoop - 1].Mid(TempPosition, 4);
					double TempValue = _wtoi(TempNumberString);
					double TempPercent = 0;
					if (TempDetectorFullReading[TempDetectorLoop])
						TempPercent = TempValue / TempDetectorFullReading[TempDetectorLoop] * 100.0;
					TempNumberString = Pad(dtoa(TempPercent,0) + TempUnits,3,5);
					TempString = TempString + TempNumberString;
					TempPosition = TempPosition + 4;
				}
			}
			if (vLocalConfigurationData->vSourceStrengthComment[TempLoop - 1].GetLength() > 50)
			{
				TempPosition = vLocalConfigurationData->vSourceStrengthComment[TempLoop - 1].Find(_T("Temp"), TempPosition);
				TempPosition = vLocalConfigurationData->vSourceStrengthComment[TempLoop - 1].GetLength() - TempPosition;
				CString TempCommentString =	vLocalConfigurationData->vSourceStrengthComment[TempLoop - 1].Right(TempPosition);
				if (TempCommentString.GetLength() > 5)
					TempString = TempString + TempCommentString;
			}
		}
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			if (vSourceStrengthDisplayMode == cDisplayPercentOfInitial)
				TempString = TempString + "\nInitial Values for % Calculation:\n";
			else
				TempString = TempString + "\nStandard Values for % Calculation:\n";

			TempString = TempString + Pad(dtoa(TempDetectorFullReading[vLocalConfigurationData->vNumberOfXRayDetectors],0),3,5);
			TempString = TempString + "\t" + "                    \t";

			for (WORD TempDetectorLoop = 0; TempDetectorLoop < vLocalConfigurationData->vNumberOfXRayDetectors; TempDetectorLoop++)
			{
				TempString = TempString + Pad(dtoa(TempDetectorFullReading[TempDetectorLoop],0),3,5);
			}
		}
	}

	if (PasswordOK(cTemporaryInspxPassword,false))
	if (vLocalConfigurationData)
	if (vLocalConfigurationData->vSourceStrengthCount)
		WriteStringToFile("Source Strength " + vLocalConfigurationData->vScanTracID + " Tube SN " + vGlobalLifeTimeCounters->vXRayTubeSerialNumber, TempString);

	CreateColorDataStructure();

	int TempEndCount = TempString.GetLength();
	if (TempEndCount >= cSizeOfVColor - 1)
		TempEndCount = cSizeOfVColor - 2;
	int TempPreviousLine = 2;
	int TempThisLine = 2;
	if (TempEndCount)
	for (int TempPosition = 0; TempPosition < TempEndCount - 5; TempPosition++)
	{
		wchar_t TempChar = TempString.GetAt(TempPosition);
		wchar_t TempNextChar = TempString.GetAt(TempPosition + 1);
		if (TempChar == (char)0x0A)
		{
			TempPreviousLine = TempThisLine;
			TempThisLine = TempPosition + 2;
		}
		if ((TempChar == '+') || ((TempChar == '-') && (TempNextChar == '-')))
		{
			int TempStop = TempString.Find((char)0x0A,TempPosition);
			if (TempStop > TempEndCount)
				TempStop = TempEndCount;
			for (int TempOffset = TempPreviousLine; TempOffset <= TempStop; TempOffset++)
			if (TempOffset < cSizeOfVColor)
				vColor[TempOffset] = 3; //green
				//vColor[TempOffset] = 2; //blue
				//vColor[TempOffset] = 1; //red
			TempPosition = TempStop;
		}
		else
		if (TempChar == '*')
		{
			int TempStop = TempString.Find((char)0x0A,TempPosition);
			if (TempStop > TempEndCount)
				TempStop = TempEndCount;
			for (int TempOffset = TempPreviousLine; TempOffset <= TempStop; TempOffset++)
			if (TempOffset < cSizeOfVColor)
				vColor[TempOffset] = 1; //red
			//vColor[TempOffset] = 3; //green
			//vColor[TempOffset] = 2; //blue
			TempPosition = TempStop;
		}
	}
	TempString = TempString + "\n\n";

	m_HelpTextControl.LineScroll(-9999);
	SetDlgItemText(IDC_HelpText,TempString);
	DisplayColor(&TempString);
//	int vTimerResult = SetTimer(vHideCaretTimerHandle,10,NULL);
	UpdateButtons();

	CHARFORMAT TempCharacterFormat;
	TempCharacterFormat.dwMask = CFM_COLOR ;
	TempCharacterFormat.dwEffects = NULL;
	TempCharacterFormat.crTextColor = cBlack;
	m_HelpTextControl.SetDefaultCharFormat(TempCharacterFormat);
}


void CHelpDialog::DisplayColor(CString *TempErrorString)
{
	if (vColor)
	{
		int TempLength = TempErrorString->GetLength();
		if (TempLength > cSizeOfVColor)
			TempLength = cSizeOfVColor;

		int TempLast = 1;
		int TempLoop = 0;
		for (TempLoop = 2; TempLoop <= TempLength; TempLoop++)
		if (TempLoop < cSizeOfVColor)
		if (vColor[TempLoop] != vColor[TempLoop - 1])
		{
			m_HelpTextControl.SetSel(TempLast - 1,(TempLast - 1) + (TempLoop - 1));
			if (vColor[TempLoop - 1] == 1) //red
				m_HelpTextControl.SetSelectionCharFormat(vRedCharacterFormat);
			else
			if (vColor[TempLoop - 1] == 2) //blue
				m_HelpTextControl.SetSelectionCharFormat(vBlueCharacterFormat);
			else
			if (vColor[TempLoop - 1] == 3) //green
				m_HelpTextControl.SetSelectionCharFormat(vGreenCharacterFormat);
			else
			if (vColor[TempLoop - 1] == 4) //yellow
				m_HelpTextControl.SetSelectionCharFormat(vYellowCharacterFormat);
			else
			//if (vColor[TempLoop - 1] == 2) //black
				m_HelpTextControl.SetSelectionCharFormat(vBlackCharacterFormat);
			TempLast = TempLoop;
		}
		m_HelpTextControl.SetSel(TempLast - 1,(TempLast - 1) + TempLength);
		if (TempLoop < cSizeOfVColor)
		if (vColor[TempLoop - 1] == 1) //red
			m_HelpTextControl.SetSelectionCharFormat(vRedCharacterFormat);
		else
		/*
		if (vColor[TempLoop - 1] == 2) //blue
			m_HelpTextControl.SetSelectionCharFormat(vBlueCharacterFormat);
		else
		*/
		if (vColor[TempLoop - 1] == 3) //green
			m_HelpTextControl.SetSelectionCharFormat(vGreenCharacterFormat);
		else
			/*
		if (vColor[TempLoop - 1] == 4) //yellow
			m_HelpTextControl.SetSelectionCharFormat(vYellowCharacterFormat);
		else
		*/
		//if (vColor[TempLoop - 1] == 2) //black
		m_HelpTextControl.SetSelectionCharFormat(vBlackCharacterFormat);
		m_HelpTextControl.SetSel(TempLength + 1,TempLength + 1);
	}
	else
	{ //no color data, so set all text black
		m_HelpTextControl.SetSelectionCharFormat(vBlackCharacterFormat);
		m_HelpTextControl.SetSel(0,TempErrorString->GetLength() + 1);
	}
	if ((cErrorLogWindow) && (vFindTextShow == cFindTextHighlight))
	{
		HighlightFindHelpText(vFindString, true);
	}
}

//void CHelpDialog::OnMouseMove(UINT nFlags, CPoint point) 
//{
//	// TODO: Add your message handler code here and/or call default
//	if (vGlobalMouseMoveCounter < 255)
//		vGlobalMouseMoveCounter++;
//	if (vGlobalMouseMoveCounter > 5)
//	{
//		vGlobalMouseMoveCounter = 0;
//		if (vGlobalPasswordTimeOut)
//			vGlobalPasswordTimeOut = 600;
//		if (vGlobalUnderRemoteControl)
//			vGlobalUnderRemoteControl = 600;
//	}
//	CDialog::OnMouseMove(nFlags, point);
//}


BOOL CHelpDialog::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if (zDelta > 0)
	{
		ScrollOneLineUp();  //scroll 1 line up
	}
	else
	if (zDelta < 0)
	{
		ScrollOneLineDown();  //scroll 1 line down
	}
	
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL CHelpDialog::FindHelpText(void)
{
	//	vFindString = IAlphaEntryDialog.vEditString;
	//	int CountLines = 0;					// Current Line Position, start at 0
	//	int TotalCountLines = 0;		// increment to 0 on first pass through while loop
//	int TempTotalLines;

// if same find as last time move the find point thru the file, else clear possible other searches
	if (vFindString == vLastFindString)
	{
		if (vFindTextShow == cFindTextHighlight)	// last search was in this window
		{
			vStartFindPosition = vLastFindPosition; // set for continue search in string
		}
		else	// last search probably not in this window, highlight find text
		{
		// if in this window set value for vFindTextShow to cFindTextHighlight;
			vFindTextShow = cFindTextHighlight;
//			HighlightFindHelpText(vFindString, true);
		}
	}
	else
	{
		// if in this window set value for vFindTextShow to cFindTextHighlight;
		vFindTextShow = cFindTextHighlight;

		HighlightFindHelpText(vLastFindString, false);

		vLastFindString = vFindString;		// set for next search
		vStartFindPosition = 0;						// restart at beginning of text selection
		
		//goto top  //kjh top for both types??
		ShowErrorLog(false); //do not do refresh
		m_HelpTextControl.LineScroll(-9999);
		UpdateButtons();//			ShowErrorLog(false); //do not do refresh

//		HighlightFindHelpText(vFindString, true);
	}

	FINDTEXTEX	TextToLocate;
//	CHARRANGE SearchRange;

	TextToLocate.chrg.cpMin = vStartFindPosition;
	TextToLocate.chrg.cpMax = -1;		// search to the end
	LPWSTR FindToCharString = _T("");

// TextToLocate.chrg.cpMin	// start of text to search
// TextToLocate.chrg.cpMax	// end   of text to search
// TextToLocate.chrgText.cpMin	// start of text found
// TextToLocate.chrgText.cpMax	// end   of text found

	lstrcpy(FindToCharString, vFindString);
	TextToLocate.lpstrText = FindToCharString ;
 
	long IndexToFindText = m_HelpTextControl.FindText(FR_DOWN, &TextToLocate);		//options eliminated FR_WHOLEWORD and FR_MATCHCASE

// This is character sensitive, make not char sensitive, use above	int IndexToFindText = vEventLogString.Find((_T(vFindString)),StartFindPosition);

	if (IndexToFindText == -1)	//string not found, display not found info or if error log read in more file and check for string
	{

// if the file type is 0x8000 = inspx log, otherwise file type is 0x8000 != error log 	
// inpsx log is all read in at once, error log might have more to read in
		if ((vWindowType == cErrorLogWindow) && (vErrorMessageDisplayMask != cInspxLogMask))
		{
			if (!vGlobalDisplayDialogThread)
			{
				vGlobalDisplayDialogThread = (CDisplayDialogThread*)AfxBeginThread(RUNTIME_CLASS(CDisplayDialogThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
				if (!vGlobalDisplayDialogThread)
					ReportErrorMessage("Error-Could Not Create Display Dialog Thread", cEMailInspx,32000); 
				else
				{
					vGlobalDisplayDialogThread->vMessageToDisplay = "\n\n\nSearching for text: \"" + vFindString + "\"";
					vGlobalDisplayDialogThread->ResumeThread();
				}
			}
			// scroll through rest of file looking for text
			while ((IndexToFindText == -1) && (vMaxEventLogScrollDown == false))
			{
				// if not at the bottom of the file effectively do a page down
	//			ShiftEventLogDown((m_HelpTextControl.GetLineCount()-10)); //shows more current data
				// search again
				ShiftEventLogDown((vLinesOnScreen-1));
	//			TextToLocate.chrg.cpMin = vLastFindPosition - vLinesOnScreen;		// start at last find of text
				TextToLocate.chrg.cpMin = 0;		// start at new beginning of text
				TextToLocate.chrg.cpMax = -1;		// search to the end

				IndexToFindText = m_HelpTextControl.FindText(FR_DOWN, &TextToLocate);		//options eliminated FR_WHOLEWORD and FR_MATCHCASE	
			}
			if (vGlobalDisplayDialogThread)
			{
				vGlobalDisplayDialogThread->ExitDisplayThread();
				vGlobalDisplayDialogThread = NULL;
			}
		}	// end if error log

		//find done through file and ready to display "not found" message.
		if (((IndexToFindText == -1) && (vWindowType == cErrorLogWindow) && (vErrorMessageDisplayMask != cInspxLogMask) && (vMaxEventLogScrollDown == true)) ||
			 (IndexToFindText == -1) && (vWindowType == cErrorLogWindow) && (vErrorMessageDisplayMask == cInspxLogMask))	 // if at the bottom and did not find -- done
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nText \"" + vFindString + "\" Not Found";
			if (vStartFindPosition != 0)
			{
				vLastFindPosition = 0;
				TempNoticeDialog.vNoticeText = "\n\nEnd of File\nText \"" + vFindString + "\" Not Found";
			}
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();

			// sequence of commands (3) that send the text to the top
			ShowErrorLog(false); //do not do refresh
			m_HelpTextControl.LineScroll(-9999);
			UpdateButtons();

			return(true);
		}
	}

// if here have text to show
	vLastFindPosition = IndexToFindText + 1;
	long dFindStringLinesFromTop = m_HelpTextControl.LineFromChar(IndexToFindText);			// first line is 0
	int FindString_LinesFromTop = (int)dFindStringLinesFromTop;
	int TempTotalLines = m_HelpTextControl.GetLineCount();
	int TempCurrentLine = m_HelpTextControl.GetFirstVisibleLine();
	// is there more than one screen/window of data
	if (TempTotalLines > vLinesOnScreen)
	{
		// move edit window down in file more for next find
		// scroll down if not at bottom in error log
		if ((FindString_LinesFromTop > vLinesOnScreen) && (vMaxEventLogScrollDown == false) && ((vErrorMessageDisplayMask != cInspxLogMask) && (vWindowType == cErrorLogWindow)))	
		{
			ShiftEventLogDown((vLinesOnScreen-1));
			vStartFindPosition = 0;
			vLastFindPosition = 0;

			// find current find
			TextToLocate.chrg.cpMin = vStartFindPosition;
			TextToLocate.chrg.cpMax = -1;		// search to the end
			IndexToFindText = m_HelpTextControl.FindText(FR_DOWN, &TextToLocate);		//options eliminated FR_WHOLEWORD and FR_MATCHCASE

			vLastFindPosition  = IndexToFindText + 1; 

			dFindStringLinesFromTop = m_HelpTextControl.LineFromChar(IndexToFindText);			// first line is 0
			FindString_LinesFromTop = (int)dFindStringLinesFromTop;
			TempTotalLines = m_HelpTextControl.GetLineCount();
			TempCurrentLine = m_HelpTextControl.GetFirstVisibleLine();	// where is the text box now, go to to
		}

		m_HelpTextControl.LineScroll(-TempCurrentLine);						// scroll to top
		if ((TempTotalLines - (vLinesOnScreen-1)) > FindString_LinesFromTop)
		{
			m_HelpTextControl.LineScroll(FindString_LinesFromTop);		// scroll to  location
		}
		else
		{
			m_HelpTextControl.LineScroll((TempTotalLines - (vLinesOnScreen-1)));		// scroll to  top of bottom
		}
	}
		
//	m_HelpTextControl.HideCaret();
//	this->Invalidate(false);

	UpdateButtons();

	if ((TempTotalLines > vLinesOnScreen) || !vMaxEventLogScrollUp)
	{
		m_SubFunction5Button.ShowNavigationArrow(cUpArrowBig);
		m_SubFunction6Button.ShowNavigationArrow(cUpArrowSmall);
		m_SubFunction5Button.ShowWindow(SW_SHOW);
		m_SubFunction6Button.ShowWindow(SW_SHOW);
	}
	if (((TempCurrentLine + 1 + vLinesOnScreen) >= TempTotalLines) && vMaxEventLogScrollDown)
	{  
		m_SubFunction7Button.ShowNavigationArrow(NULL);
		m_SubFunction8Button.ShowNavigationArrow(NULL);
		m_SubFunction7Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
		m_SubFunction8Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
	}

	return(true);
}

BOOL CHelpDialog::FindHelpEventLogText(void)
{
	//	vFindString = IAlphaEntryDialog.vEditString;
	//	int CountLines = 0;					// Current Line Position, start at 0
	//	int TotalCountLines = 0;		// increment to 0 on first pass through while loop
//	int TempTotalLines;

// if same find as last time move the find point thru the file, else clear possible other searches
	if ((vSameLogFile) && (vFindString == vLastFindString))
	{
		if (vFindTextShow == cFindTextHighlight)	// last search was in this window
		{
			vStartFindPosition = vLastFindPosition; // set for continue search in string
		}
		else	// last search probably not in this window, highlight find text
		{
		// if in this window set value for vFindTextShow to cFindTextHighlight;
			vFindTextShow = cFindTextHighlight;
//			HighlightFindHelpText(vFindString, true);
		}
	}
	else
	{
		// if in this window set value for vFindTextShow to cFindTextHighlight;
		vFindTextShow = cFindTextHighlight;

		HighlightFindHelpText(vLastFindString, false);

		vLastFindString = vFindString;		// set for next search
		vStartFindPosition = 0;				// restart at beginning of text selection << ?? restart at beginning of file ??
		
		//goto top  //kjh top for both types??
		ShowErrorLog(false); //do not do refresh
		m_HelpTextControl.LineScroll(-9999);
		UpdateButtons();//			ShowErrorLog(false); //do not do refresh

//		HighlightFindHelpText(vFindString, true);
	}
	if (vSameLogFile == false)
		vSameLogFile = true;	//<< next time it is the same log file unless a new log file is loaded

	FINDTEXTEX	TextToLocate;
//	CHARRANGE SearchRange;

	TextToLocate.chrg.cpMin = vStartFindPosition;
	TextToLocate.chrg.cpMax = -1;		// search to the end
	TextToLocate.chrgText.cpMin = 0;
	TextToLocate.chrgText.cpMax = 0;
	LPWSTR FindToCharString = _T("");

// TextToLocate.chrg.cpMin	// start of text to search
// TextToLocate.chrg.cpMax	// end   of text to search
// TextToLocate.chrgText.cpMin	// start of text found
// TextToLocate.chrgText.cpMax	// end   of text found

	lstrcpy(FindToCharString, vFindString);
	TextToLocate.lpstrText = FindToCharString ;
 
	long IndexToFindText = m_HelpTextControl.FindText(FR_DOWN, &TextToLocate);		//options eliminated FR_WHOLEWORD and FR_MATCHCASE

	long TempSizeMin = TextToLocate.chrgText.cpMin;

	long TempSizeMax = TextToLocate.chrgText.cpMax;

	//	long FindText(   DWORD dwFlags,   FINDTEXTEX* pFindText ) const;
	//	typedef struct _findtextex {    CHARRANGE chrg;    LPCTSTR lpstrText;    CHARRANGE chrgText;} FINDTEXTEX;

// This is character sensitive, make not char sensitive, use above	int IndexToFindText = vEventLogString.Find((_T(vFindString)),StartFindPosition);

	if (IndexToFindText == -1)	//string not found, display not found info or if error log read in more file and check for string
	{

// if the file type is 0x8000 = inspx log, otherwise file type is 0x8000 != error log 	
// inpsx log is all read in at once, error log might have more to read in
		if ((vWindowType == cErrorLogWindow) && (vErrorMessageDisplayMask != cInspxLogMask))
		{
			// scroll through rest of file looking for text
			while ((IndexToFindText == -1) && (vMaxEventLogScrollDown == false))
			{
				// if not at the bottom of the file effectively do a page down
				
				if (m_HelpTextControl.GetLineCount() > 10)
					ShiftEventLogFindDown((m_HelpTextControl.GetLineCount()-10)); //shows more current data
				else
					ShiftEventLogFindDown((-(m_HelpTextControl.GetLineCount())+2)); //shows more current data

				// search again
				//ShiftEventLogDown((vLinesOnScreen-1)); // 7/23/13 kjh try just shift down, ShiftEventLogFindDown((vLinesOnScreen-1));  //ShiftEventLogFindDown
			//ShiftEventLogFindDown((vLinesOnScreen-1));
//			TextToLocate.chrg.cpMin = vLastFindPosition - vLinesOnScreen;		// start at last find of text
				TextToLocate.chrg.cpMin = 0;		// start at new beginning of text
				TextToLocate.chrg.cpMax = -1;		// search to the end

				IndexToFindText = m_HelpTextControl.FindText(FR_DOWN, &TextToLocate);		//options eliminated FR_WHOLEWORD and FR_MATCHCASE	
			}
		}	// end if error log

		//find done through file and ready to display "not found" message.
		if (((IndexToFindText == -1) && (vWindowType == cErrorLogWindow) && (vErrorMessageDisplayMask != cInspxLogMask) && (vMaxEventLogScrollDown == true)) ||
			 (IndexToFindText == -1) && (vWindowType == cErrorLogWindow) && (vErrorMessageDisplayMask == cInspxLogMask) )	 // if at the bottom and did not find -- done
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nText \"" + vFindString + "\" Not Found";
			if (vStartFindPosition != 0)
			{
				vLastFindPosition = 0;
				TempNoticeDialog.vNoticeText = "\n\nEnd of File\nText \"" + vFindString + "\" Not Found";
			}
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();

			// sequence of commands (3) that send the text to the top
			ShowErrorLog(false); //do not do refresh
			m_HelpTextControl.LineScroll(-9999);
			UpdateButtons();

			return(true);
		}
	}

	// if here have text to show
	
	vLastFindPosition = IndexToFindText + 1;
	long dFindStringLinesFromTop = m_HelpTextControl.LineFromChar(IndexToFindText);			// first line is 0
	int FindString_LinesFromTop = (int)dFindStringLinesFromTop;
	int TempTotalLines = m_HelpTextControl.GetLineCount();
	int TempCurrentLine = m_HelpTextControl.GetFirstVisibleLine();

	// is there more than one screen/window of data
	if (TempTotalLines > vLinesOnScreen)
	{
		// move edit window down in file more for next find
		// scroll down if not at bottom in error log
		if ((FindString_LinesFromTop > vLinesOnScreen) && (vMaxEventLogScrollDown == false) && (vErrorMessageDisplayMask != cInspxLogMask) && (vWindowType == cErrorLogWindow))	
		{
//			ShiftEventLogDown((vLinesOnScreen-1));
			while ((FindString_LinesFromTop > (vLinesOnScreen - 1)) && (vMaxEventLogScrollDown == false))
			{
				ShiftEventLogDown((vLinesOnScreen-1));
				FindString_LinesFromTop = FindString_LinesFromTop - (vLinesOnScreen-1);
			}
			vStartFindPosition = 0;
			vLastFindPosition = 0;

			// find current find
			TextToLocate.chrg.cpMin = vStartFindPosition;
			TextToLocate.chrg.cpMax = -1;		// search to the end
			IndexToFindText = m_HelpTextControl.FindText(FR_DOWN, &TextToLocate);		//options eliminated FR_WHOLEWORD and FR_MATCHCASE

			vLastFindPosition  = IndexToFindText + 1; 

			dFindStringLinesFromTop = m_HelpTextControl.LineFromChar(IndexToFindText);			// first line is 0
			FindString_LinesFromTop = (int)dFindStringLinesFromTop;
			TempTotalLines = m_HelpTextControl.GetLineCount();
			TempCurrentLine = m_HelpTextControl.GetFirstVisibleLine();	// where is the text box now, go to to
		}
		else
		{
			ShiftEventLogDown(0);
		}


		if (TempCurrentLine != 0)
		{
			m_HelpTextControl.LineScroll(-TempCurrentLine);						// scroll to top
		}
		if ((TempTotalLines - (vLinesOnScreen-1)) > FindString_LinesFromTop)
		{
			m_HelpTextControl.LineScroll(FindString_LinesFromTop);		// scroll to  location
		}
		else
		{
			m_HelpTextControl.LineScroll((TempTotalLines - (vLinesOnScreen-1)));		// scroll to  top of bottom
		}
	}
		
// temp dont hide carat kjh	int 	m_HelpTextControl.HideCaret();
// temp dont hide carat kjh	int		this->Invalidate(false);

	UpdateButtons();

	if ((TempTotalLines > vLinesOnScreen) || !vMaxEventLogScrollUp )
	{
		m_SubFunction5Button.ShowNavigationArrow(cUpArrowBig);
		m_SubFunction6Button.ShowNavigationArrow(cUpArrowSmall);
		m_SubFunction5Button.ShowWindow(SW_SHOW);
		m_SubFunction6Button.ShowWindow(SW_SHOW);
	}
	if (((TempCurrentLine + 1 + vLinesOnScreen) >= TempTotalLines) && vMaxEventLogScrollDown)
	{  
		m_SubFunction7Button.ShowNavigationArrow(NULL);
		m_SubFunction8Button.ShowNavigationArrow(NULL);
		m_SubFunction7Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
		m_SubFunction8Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
	}

	HighlightFindHelpText(vFindString, true);
	return(true);
}

BOOL CHelpDialog::HighlightFindHelpText(CString ChangeTextColor, bool AddHighlight )
{
	if (ChangeTextColor != "")
	{
		FINDTEXTEX	TextToLocate;
		LPWSTR FindToCharString = _T("");
		long IndexToFindText;

		TextToLocate.chrg.cpMin =  0;		//start search location
		TextToLocate.chrg.cpMax = -1;		// end search location (-1 is search to end)
	// TextToLocate.chrg.cpMin			// start of text to search
	// TextToLocate.chrg.cpMax			// end   of text to search
	// TextToLocate.chrgText.cpMin	// start of text found
	// TextToLocate.chrgText.cpMax	// end   of text found

		lstrcpy(FindToCharString, ChangeTextColor);
		TextToLocate.lpstrText = FindToCharString ;
	//	TextToLocate.lpstrText = (LPCTSTR)vFindString;
 
		IndexToFindText = m_HelpTextControl.FindText(FR_DOWN, &TextToLocate);		//options eliminated FR_WHOLEWORD and FR_MATCHCASE

		CHARFORMAT cf;

		if (AddHighlight == true)
		{
			while (IndexToFindText != -1)
			{
				//m_HelpTextControl.HideCaret();
				//this->Invalidate(false);
/*
				if (TextToLocate.chrgText.cpMin >= 2)
				{
					m_HelpTextControl.SetSel(TextToLocate.chrgText.cpMin - 2,TextToLocate.chrgText.cpMin - 1);		//just before char, what is the color?
					m_HelpTextControl.GetSelectionCharFormat(cf);		// get format of char before or after selection
				}
				else
				{
					m_HelpTextControl.SetSel(TextToLocate.chrgText.cpMax + 1,TextToLocate.chrgText.cpMax + 2);		//just after char, what is the color?
					m_HelpTextControl.GetSelectionCharFormat(cf);		// get format of char before or after selection
				}
				cf.dwMask = CFM_COLOR | CFM_BOLD | CFM_UNDERLINE;
				cf.dwEffects = 0L ;		// turn off all effects
				cf.dwMask = CFM_COLOR | CFM_BOLD | CFM_UNDERLINE;
				cf.dwEffects =  CFE_BOLD ; // turn on bold effect  CFE_BOLD | CFE_UNDERLINE;
				cf.crTextColor = cPink;
*/
				m_HelpTextControl.SetSel(TextToLocate.chrgText);		//begining char of text, to end char of text, highlight
				m_HelpTextControl.SetSelectionCharFormat(vPinkCharacterFormat);

/*				TextToLocate.chrg.cpMin = TextToLocate.chrgText.cpMax;
				FINDTEXTEX TextToClear;
				TextToClear.chrg.cpMin =  TextToLocate.chrgText.cpMin;
				TextToClear.chrg.cpMax =  TextToLocate.chrgText.cpMin + 1;

				IndexToFindText = m_HelpTextControl.FindText(FR_DOWN, &TextToLocate);
				TextToClear.lpstrText = " " ;
				int IndexToClearSelectHighlight = m_HelpTextControl.FindText(FR_DOWN,&TextToClear);
*/
				TextToLocate.chrg.cpMin = TextToLocate.chrgText.cpMax;
				IndexToFindText = m_HelpTextControl.FindText(FR_DOWN, &TextToLocate);
			}
		}
		else
		{
			while (IndexToFindText != -1)
			{
				if (TextToLocate.chrgText.cpMin >= 2)
				{
					m_HelpTextControl.SetSel(TextToLocate.chrgText.cpMin - 2,TextToLocate.chrgText.cpMin - 1);		//just before char, what is the color?
					m_HelpTextControl.GetSelectionCharFormat(cf);		// get format of char before or after selection
				}
				else
				{
					m_HelpTextControl.SetSel(TextToLocate.chrgText.cpMax + 1,TextToLocate.chrgText.cpMax + 2);		//just after char, what is the color?
					m_HelpTextControl.GetSelectionCharFormat(cf);		// get format of char before or after selection
				}
				m_HelpTextControl.SetSel(TextToLocate.chrgText);	// 
				m_HelpTextControl.SetSelectionCharFormat(cf);		// make all the same color as before or after
				//m_HelpTextControl.SetSelectionCharFormat(vBlackCharacterFormat);
				TextToLocate.chrg.cpMin = TextToLocate.chrgText.cpMax;
				IndexToFindText = m_HelpTextControl.FindText(FR_DOWN, &TextToLocate);
			}
		}
	//	m_HelpTextControl.LineScroll(-9999);		//scroll to top

//		m_HelpTextControl.HideCaret();
//		this->Invalidate(false);
	}

	return (true);

}

/*
int CHelpDialog::GetLines(void)
{
	int CarriageReturnsCounted = 0;
	char FindToCharString[5];

	FindToCharString[0] = (char)(0x0a);	// carriage return
	FindToCharString[1] = (char)(0x00);	// null

	FINDTEXTEX	TextToLocate;

	TextToLocate.chrg.cpMin = vStartFindPosition;
	TextToLocate.chrg.cpMax = -1;		// search to the end
	TextToLocate.lpstrText = FindToCharString ;
	//TextToLocate.chrgText.cpMin	// start of found text
	//TextToLocate.chrgText.cpMax	// end of found text

	while (m_HelpTextControl.FindText(FR_DOWN, &TextToLocate) != -1)		//options eliminated FR_WHOLEWORD and FR_MATCHCASE
	{
		CarriageReturnsCounted++;
		TextToLocate.chrg.cpMin = TextToLocate.chrgText.cpMax;
	}

	return (CarriageReturnsCounted);
}
*/

void CHelpDialog::OnDoubleClickSubFunction5Button()
{
	//ReportErrorMessage("Help Dialog: Left Button Double Click on SubFunction 5",cAction, 32000);
		
	//if (vMouseState > 4)	//if ((vMouseState >= 9) && (vMouseState <= 16))
	//{
	//	KillTimer(vMouseDownRepeatTimerHandle);
	//	//stop scrolling
	//	vMouseState = 0;
	//}
	//else	//if (vMouseState == 0 idle)
	//{
	//	vMouseState = 5;	//toggle move ROI lines
	//	int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
	//	if (!TimerResult)
	//		ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
	//}
}


void CHelpDialog::OnDoubleClickSubFunction6Button()
{
	//ReportErrorMessage("Help Dialog: Left Button Double Click on SubFunction 6",cAction, 32000);
	//	
	//if (vMouseState > 4)	//if ((vMouseState >= 9) && (vMouseState <= 16))
	//{
	//	KillTimer(vMouseDownRepeatTimerHandle);
	//	//stop scrolling
	//	vMouseState = 0;
	//}
	//else	//if (vMouseState == 0 idle)
	//{
	//	vMouseState = 6;	//toggle move ROI lines
	//	int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
	//	if (!TimerResult)
	//		ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
	//}
}


void CHelpDialog::OnDoubleClickSubFunction7Button()
{
	//ReportErrorMessage("Help Dialog: Left Button Double Click on SubFunction 7",cAction, 32000);
		
	//if (vMouseState > 4)	//if ((vMouseState >= 9) && (vMouseState <= 16))
	//{
	//	KillTimer(vMouseDownRepeatTimerHandle);
	//	//stop scrolling
	//	vMouseState = 0;
	//}
	//else	//if (vMouseState == 0 idle)
	//{
	//	vMouseState = 7;	//toggle move ROI lines
	//	int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
	//	if (!TimerResult)
	//		ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
	//}
}


void CHelpDialog::OnDoubleClickSubFunction8Button()
{
	//ReportErrorMessage("Help Dialog: Left Button Double Click on SubFunction 8",cAction, 32000);
		
	//if (vMouseState > 4)	//if ((vMouseState >= 9) && (vMouseState <= 16))
	//{
	//	KillTimer(vMouseDownRepeatTimerHandle);
	//	//stop scrolling
	//	vMouseState = 0;
	//}
	//else	//if (vMouseState == 0 idle)
	//{
	//	vMouseState = 8;	//toggle move ROI lines
	//	int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
	//	if (!TimerResult)
	//		ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
	//}
}

void CHelpDialog::CreateColorDataStructure()
{
	if (!vColor)
	{
		if (ThereIsEnoughMemory(sizeof(int) * cSizeOfVColor, "Color array in help dialog"))
			vColor = (int *)malloc(sizeof(int) * cSizeOfVColor);
		else
		{
			ReportErrorMessage("Not Enough Memory for Color Array Alloc in Help Dialog",cEMailInspx, 32000);
			#ifdef _DEBUG
			afxDump << "Not Enough Memory for Help Dialog Alloc";
			#endif
		}
	}
	ZeroMemory(vColor, cSizeOfVColor * sizeof(int));
}

void CHelpDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}


BOOL CHelpDialog::DestroyWindow()
{
	if (vLineIndexInFile)
		free(vLineIndexInFile);

	if (vColor)
	{
		free(vColor);
		vColor = NULL;
	}

	if (vEventLogCurrentFiles)
		delete [] vEventLogCurrentFiles;

	return CDialog::DestroyWindow();
}

void CHelpDialog::ScrollOnePageUp()
{
	if ((vWindowType != cRemoteKeyLabelWindow) && (vWindowType != cMainHelpWindow))
	{
		// here for Event Log
		int TempTotalLines = m_HelpTextControl.GetLineCount();
		int TempCurrentLine = m_HelpTextControl.GetFirstVisibleLine();
		if ((vWindowType == cErrorLogWindow) && (vErrorMessageDisplayMask != cInspxLogMask))//regualar log, not inspx
		{
			if (!vMaxEventLogScrollUp) 
			{
				// if inside richeditcontrol, and can scroll up, just scroll up 
				if (TempCurrentLine > vLinesOnScreen)
					m_HelpTextControl.LineScroll(-vLinesOnScreen);
				else
				{
					ShiftEventLogUp (vLinesOnScreen); //shows more current data
				}
			}
			else
			{
				if ( TempCurrentLine > vLinesOnScreen )
					m_HelpTextControl.LineScroll(-vLinesOnScreen);
				else
					m_HelpTextControl.LineScroll(-TempCurrentLine);

			}
			TempCurrentLine = m_HelpTextControl.GetFirstVisibleLine();

//			m_HelpTextControl.HideCaret();
//			this->Invalidate(false);

			UpdateButtons();

			if ((TempCurrentLine == 0) && vMaxEventLogScrollUp)
			{
				m_SubFunction5Button.ShowNavigationArrow(cUpArrowBig);
				m_SubFunction6Button.ShowNavigationArrow(cUpArrowSmall);
				m_SubFunction5Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
				m_SubFunction6Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
			}
			else
			{
				m_SubFunction5Button.ShowNavigationArrow(cUpArrowBig);
				m_SubFunction6Button.ShowNavigationArrow(cUpArrowSmall);
				m_SubFunction5Button.ShowWindow(SW_SHOW);
				m_SubFunction6Button.ShowWindow(SW_SHOW);
			}

			if ((TempCurrentLine + 1 + vLinesOnScreen < TempTotalLines) || !vMaxEventLogScrollDown)
			{  
				//m_SmallDownArrow.ShowWindow(SW_SHOW);
				//m_DownArrow.ShowWindow(SW_SHOW);
				m_SubFunction7Button.ShowNavigationArrow(cDownArrowSmall);
				m_SubFunction8Button.ShowNavigationArrow(cDownArrowBig);
				m_SubFunction7Button.ShowWindow(SW_SHOW);
				m_SubFunction8Button.ShowWindow(SW_SHOW);
			}
	
		}
		else
		if (TempCurrentLine > 0)
		{
			m_HelpTextControl.LineScroll(-vLinesOnScreen);
//			m_HelpTextControl.HideCaret();
//			this->Invalidate(false);

			UpdateButtons();
		}
	}
}

void CHelpDialog::ScrollOneLineUp()
{
	if ((vWindowType != cRemoteKeyLabelWindow) && (vWindowType != cMainHelpWindow))
	{
		//line up
		int TempTotalLines = m_HelpTextControl.GetLineCount();
		int TempCurrentLine = m_HelpTextControl.GetFirstVisibleLine();
		if ((vWindowType == cErrorLogWindow) && (vErrorMessageDisplayMask != cInspxLogMask)) //regualar log, not inspx
		{
			bool TempDoScroll = false;

			if (!vMaxEventLogScrollUp) 
			if (!((vMaxEventLogScrollDown) && (TempCurrentLine < TempTotalLines - (vLinesOnScreen * 3))))
				//there are 1000 lines on the screen, don't get more data unless it is more than three pages from the end
			{
				ShiftEventLogUp (1); //shows more current data
				int TempNewCurrentLine = m_HelpTextControl.GetFirstVisibleLine();
				if (TempCurrentLine != TempNewCurrentLine)
					m_HelpTextControl.LineScroll(TempCurrentLine - TempNewCurrentLine);
			}
			else
				TempDoScroll = true;
			else
			{
				if (TempCurrentLine != 0)
					TempDoScroll = true;
			}

			if (TempDoScroll)
			{
				m_HelpTextControl.LineScroll(-1);
				TempCurrentLine = m_HelpTextControl.GetFirstVisibleLine();
			}

//			m_HelpTextControl.HideCaret();
//			this->Invalidate(false);

			UpdateButtons();

			if( (TempCurrentLine == 0) && vMaxEventLogScrollUp )
			{
				m_SubFunction5Button.ShowNavigationArrow(NULL);
				m_SubFunction6Button.ShowNavigationArrow(NULL);
				m_SubFunction5Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
				m_SubFunction6Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
			}
			else
			{
				m_SubFunction5Button.ShowNavigationArrow(cUpArrowBig);
				m_SubFunction6Button.ShowNavigationArrow(cUpArrowSmall);
				m_SubFunction5Button.ShowWindow(SW_SHOW);
				m_SubFunction6Button.ShowWindow(SW_SHOW);
			}

			if ( ( (TempCurrentLine + 1 + vLinesOnScreen) < TempTotalLines) || !vMaxEventLogScrollDown )
			{  
				m_SubFunction7Button.ShowNavigationArrow(cDownArrowSmall);
				m_SubFunction8Button.ShowNavigationArrow(cDownArrowBig);
				m_SubFunction7Button.ShowWindow(SW_SHOW);
				m_SubFunction8Button.ShowWindow(SW_SHOW);
			}
		}
		else
		if (TempCurrentLine > 0)
		{
			m_HelpTextControl.LineScroll(-1);
//			m_HelpTextControl.HideCaret();
//			this->Invalidate(false);

			UpdateButtons();
		}
	}
}

void CHelpDialog::ScrollOneLineDown()
{
	if ((vWindowType != cRemoteKeyLabelWindow) && (vWindowType != cMainHelpWindow))
	{
		//line down
		int TempTotalLines = m_HelpTextControl.GetLineCount();
		int TempCurrentLine = m_HelpTextControl.GetFirstVisibleLine();
		bool TempDoScroll = false;

		if ((vWindowType == cErrorLogWindow) && (vErrorMessageDisplayMask != cInspxLogMask)) //regualar log, not inspx
		{
			if (!vMaxEventLogScrollDown)
			{
				if ((TempTotalLines - TempCurrentLine) < (vLinesOnScreen * 2 + 2))
				{
					ShiftEventLogDown (1); //shows more current data
					TempTotalLines = m_HelpTextControl.GetLineCount();	
				}
				else
					TempDoScroll = true;
			}
			else //if ( vMaxEventLogScrollDown )
			{
				TempDoScroll = true;
				if (vMouseState > 0)
				{
					//ReportErrorMessage("Screen Clicked, repeat ended",cWriteToLog, 0);
					KillTimer(vMouseDownRepeatTimerHandle);
					vMouseState = 0;
				}
			}
			
			int TempNewCurrentLine = m_HelpTextControl.GetFirstVisibleLine();
			if( TempNewCurrentLine != TempCurrentLine)
				m_HelpTextControl.LineScroll( TempCurrentLine - TempNewCurrentLine );

			if ( TempDoScroll )
			{
				if (TempTotalLines - TempCurrentLine > vLinesOnScreen )
					m_HelpTextControl.LineScroll(1);
			}

//			m_HelpTextControl.HideCaret();
//			this->Invalidate(false);

			UpdateButtons();

			if ( (TempTotalLines > vLinesOnScreen) || !vMaxEventLogScrollUp )
			{
				m_SubFunction5Button.ShowNavigationArrow(cUpArrowBig);
				m_SubFunction6Button.ShowNavigationArrow(cUpArrowSmall);
				m_SubFunction5Button.ShowWindow(SW_SHOW);
				m_SubFunction6Button.ShowWindow(SW_SHOW);
			}
			if ( (TempCurrentLine + 1 + vLinesOnScreen >= TempTotalLines) && vMaxEventLogScrollDown)
			{  
		//		m_SubFunction7Button.ShowNavigationArrow(cDownArrowSmall);
		//		m_SubFunction8Button.ShowNavigationArrow(cDownArrowBig);
				m_SubFunction7Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
				m_SubFunction8Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
			}
		}
		else
		{
			if (TempCurrentLine == 0)
			if (TempTotalLines > vLinesOnScreen)
			{  //show up arrows if first time moved down
				m_SubFunction5Button.ShowNavigationArrow(cUpArrowBig);
				m_SubFunction6Button.ShowNavigationArrow(cUpArrowSmall);
				m_SubFunction5Button.ShowWindow(SW_SHOW);
				m_SubFunction6Button.ShowWindow(SW_SHOW);
			}
			if (TempCurrentLine + vLinesOnScreen + 1 < TempTotalLines)
			{
				m_HelpTextControl.LineScroll(1);
//				m_HelpTextControl.HideCaret();
			}
			if (TempCurrentLine == 0)
				UpdateButtons();
			
			int TempNewCurrentLine = m_HelpTextControl.GetFirstVisibleLine();
			if ((TempNewCurrentLine + 2 + vLinesOnScreen >= TempTotalLines) || (TempNewCurrentLine == TempCurrentLine))
			{  //show up arrows if first time moved down
				m_SubFunction7Button.ShowNavigationArrow(cDownArrowSmall);
				m_SubFunction8Button.ShowNavigationArrow(cDownArrowBig);
				m_SubFunction7Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
				m_SubFunction8Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
				if (vMouseState > 0)
				{
					//ReportErrorMessage("Screen Clicked, repeat ended",cWriteToLog, 0);
					KillTimer(vMouseDownRepeatTimerHandle);
					vMouseState = 0;
				}
			}
			this->Invalidate(false);
		}
	}
}

void CHelpDialog::ScrollOnePageDown()
{
	if ((vWindowType != cRemoteKeyLabelWindow) && (vWindowType != cMainHelpWindow))
	{
		int TempTotalLines = m_HelpTextControl.GetLineCount();
		int TempCurrentLine = m_HelpTextControl.GetFirstVisibleLine();
		if ((vWindowType == cErrorLogWindow) && (vErrorMessageDisplayMask != cInspxLogMask))//regualar log, not inspx
		{
			bool TempDoScroll = false;

			if (!vMaxEventLogScrollDown)
			{
				if ((TempTotalLines - TempCurrentLine) < (vLinesOnScreen * 2 + 2))	// there is not a scroll down page available
					ShiftEventLogDown(vLinesOnScreen); //shows more current data
				else
					TempDoScroll = true;
			}

			if (vMaxEventLogScrollDown)
				TempDoScroll = true;


			if ( TempDoScroll )
			{
				int TempNumberOfLinesOnCurrentPage = TempTotalLines - TempCurrentLine;
				if( TempNumberOfLinesOnCurrentPage < vLinesOnScreen * 2)
					m_HelpTextControl.LineScroll(TempTotalLines - TempCurrentLine);
				else
					m_HelpTextControl.LineScroll(vLinesOnScreen);
			}

//			m_HelpTextControl.HideCaret();
//			this->Invalidate(false);

			UpdateButtons();

			if ( (TempTotalLines > vLinesOnScreen) || !vMaxEventLogScrollUp )
			{
				m_SubFunction5Button.ShowNavigationArrow(cUpArrowBig);
				m_SubFunction6Button.ShowNavigationArrow(cUpArrowSmall);
				m_SubFunction5Button.ShowWindow(SW_SHOW);
				m_SubFunction6Button.ShowWindow(SW_SHOW);
			}
			if ( (TempCurrentLine + 1 + vLinesOnScreen >= TempTotalLines) && vMaxEventLogScrollDown)
			{  
				if (vMouseState > 0)
				{
					//ReportErrorMessage("Screen Clicked, repeat ended",cWriteToLog, 0);
					KillTimer(vMouseDownRepeatTimerHandle);
					vMouseState = 0;
				}
				m_SubFunction7Button.ShowNavigationArrow(NULL);
				m_SubFunction8Button.ShowNavigationArrow(NULL);
				m_SubFunction7Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
				m_SubFunction8Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
			}
		}
		else
		{
			if (TempCurrentLine == 0)
			if (TempTotalLines > vLinesOnScreen)
			{  //show up arrows if first time moved down
				m_SubFunction5Button.ShowNavigationArrow(cUpArrowBig);
				m_SubFunction6Button.ShowNavigationArrow(cUpArrowSmall);
				m_SubFunction5Button.ShowWindow(SW_SHOW);
				m_SubFunction6Button.ShowWindow(SW_SHOW);
			}
			//if (TempCurrentLine + ((vLinesOnScreen + 1) * 2) < TempTotalLines)
			//{
				m_HelpTextControl.LineScroll(vLinesOnScreen);
//				m_HelpTextControl.HideCaret();
			//}
			//else
			//if (TempCurrentLine + vLinesOnScreen + 1 < TempTotalLines)
			//{
			//	m_HelpTextControl.LineScroll(TempTotalLines - (TempCurrentLine + 1 + vLinesOnScreen));
			//	m_HelpTextControl.HideCaret();
			//}
			if (TempCurrentLine == 0)
				UpdateButtons();
			
			//hide down arrows if at end of file
			int TempNewCurrentLine = m_HelpTextControl.GetFirstVisibleLine();
			if ((TempNewCurrentLine + 2 + vLinesOnScreen >= TempTotalLines) || (TempNewCurrentLine == TempCurrentLine))
			{  //show up arrows if first time moved down
				if (vMouseState > 0)
				{
					//ReportErrorMessage("Screen Clicked, repeat ended",cWriteToLog, 0);
					KillTimer(vMouseDownRepeatTimerHandle);
					vMouseState = 0;
				}
				m_SubFunction7Button.ShowNavigationArrow(NULL);
				m_SubFunction8Button.ShowNavigationArrow(NULL);
				m_SubFunction7Button.ShowWindow(SW_HIDE);
				m_SubFunction8Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
				this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
			}
			this->Invalidate(false);
		}
	}
}

void CHelpDialog::SetupFont()
{
	CClientDC dc(this);
	vLinesOnScreen = 22; //640 x 480
	if (vShowTable)
	{ //use fixed spacing font so tables line up

		m_HelpTextControl.SetFont(&vGlobalFixedFont, true);
			
		if ((vLocalSystemData->vGlobalMonitorSizeX == 1024) && (vLocalSystemData->vGlobalMonitorSizeY == 768))
			vLinesOnScreen = 31; //1024x768
		else
		if ((vLocalSystemData->vGlobalMonitorSizeX == 640) && (vLocalSystemData->vGlobalMonitorSizeY == 480))
			vLinesOnScreen = 39; //640x480
		else
		if ((vLocalSystemData->vGlobalMonitorSizeX == 1280) && (vLocalSystemData->vGlobalMonitorSizeY == 1024))
			vLinesOnScreen = 37; //1280x1024
		else
		if ((vLocalSystemData->vGlobalMonitorSizeX == 800) && (vLocalSystemData->vGlobalMonitorSizeY == 600))
			vLinesOnScreen = 33; //800x600
	}
	else
	{
		m_HelpTextControl.SetFont(&vGlobalTextFont, true);

		if ((vLocalSystemData->vGlobalMonitorSizeX == 1024) && (vLocalSystemData->vGlobalMonitorSizeY == 768))
			vLinesOnScreen = 24; //1024x768
		else 
		if ((vLocalSystemData->vGlobalMonitorSizeX == 1280) && (vLocalSystemData->vGlobalMonitorSizeY == 1024))
			vLinesOnScreen = 26; //1280x1024
		else
		if ((vLocalSystemData->vGlobalMonitorSizeX == 800) && (vLocalSystemData->vGlobalMonitorSizeY == 600))
			vLinesOnScreen = 24; //800x600
	}

	CHARFORMAT TempCharacterFormat;
	TempCharacterFormat.dwMask = CFM_COLOR ;
	TempCharacterFormat.dwEffects = NULL;
	TempCharacterFormat.crTextColor = cBlack;
	m_HelpTextControl.SetDefaultCharFormat(TempCharacterFormat);
}