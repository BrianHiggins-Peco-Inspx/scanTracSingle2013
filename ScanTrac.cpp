//ScanTrac Side View Source File
// ScanTrac.cpp : Defines the class behaviors for the application.
//
#define NO_WARN_MBCS_MFC_DEPRECATION

#include "stdafx.h"
#include "Windows.h"
#include <initguid.h>
#include "ScanTrac_i.c"
#include "ScanTrac.h"
#include "ScanTracDlg.h"
#include "SerialPort.h"
#include "ImageAquisitionThread.h"
#include "ProcessQuickLearnImagesThread.h"
#include "BackupHardDriveThread.h"
#include "DisplayDialogThread.h"
#include "SpellmanDXMXRaySourcePort.h"
#include "ACPowerMoniter.h"
#include "SerialPortGuardMaster.h"
#include "PingTestThread.h"
#include "XRaySource.h"
#include "FBWF_Info.h"
#include "SerialPortRFIdeasBadgeReader.h"
#include "ProcessEvaluateImagesThread.h"
#ifdef CompileInHTTPClientOperationsGuardian
#include "SendNextOGMessageThread.h"
#endif
#include "SystemConfigurationData.h"
#include "SerialPortPowerFlex40.h"

#ifdef _DEBUG
#define new DEBUG_NEW

#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef CompileInHTTPClientOperationsGuardian
	CSendNextOGMessageThread *vGlobalSendNextOGMessageThread;
	int vGlobalLastOGServerWriteStatusCode;
	DWORD vGlobalOGSendMessageexceptionCount;
	DWORD vGlobalOGSendMessageErrorCount;
	DWORD vGlobalOGSendMessageGoodCount;
	std::wstring vGlobalOGLastErrorKeyword;
	std::wstring vGlobalOGLastErrorMessage;

	int vGlobalLastOGServerErrorCode;
	DWORD vGlobalOGMessageSendCounter;
	WORD vGlobalOGQueueHead;
	WORD vGlobalOGQueueTail;
	std::wstring vGlobalOGQueueMessage[cOGQueueLength];
	std::wstring vGlobalOGQueueKeyWord[cOGQueueLength];
	CString vGlobalOGLocationString;
	CString vGlobalOGLimitString;
	WORD vGlobalOGErrorCode;
	CString vGlobalNewOperationsGuardianErrorMessage;
#endif

	bool vGlobalClosePasswordDialog;
	CString vGlobalPasswordToEnter;
	CString vGlobalOperationsGuardianServer;
	DWORD vGlobalCopyFileCount;
	BYTE vGlobalCopyDone;
	BYTE vGlobalEndOfLineTimeOutMargin;
	BYTE vGlobalLanguage;
	bool vGlobalUse1024By768Display;
	WORD vGlobalDisplaySizeH;
	WORD vGlobalDisplaySizeV;
	CSystemConfigurationData *vGlobalConfigurationData;
	CScanTracSystemRunningData *vGlobalRunningData;
	CITIPCDig *vGlobalITIPCDig;
	BYTE vGlobalEjectorConfirmMask[cNumberOfEjectors];
	BYTE vGlobalFrameGrabberType;
	BYTE vGlobalAllowVoidInspection;
	//bool vGlobalUseHalconLibrary;
	BYTE vGlobalErrorCount;
	CString vGlobalRegistryTCPIPKeyPath;
	BYTE vGlobalScanTracType;
	CString vGlobalTopString;
	CString vGlobalBottomString;
	BYTE vGlobalCurveFitEquationOrder;
  WORD vGlobalCurveFitMaxCall; 
  double vGlobalCurveFitEpsilon;
  WORD vGlobalCurveFitStepBound;
  double vGlobalCurveFitFTolerance;
  double vGlobalCurveFitXTolerance;
  double vGlobalCurveFitGTolerance;

	BYTE vGlobalPCRunningWindowsXP;
	BYTE vGlobalLearnState;
	BYTE vGlobalNormalLearnDilateTimes;
	BYTE vGlobalQuickLearnDilateTimes;
	bool vGlobalEvenSecond;
	//BYTE vGlobalMouseMoveCounter;
	//WORD vGlobalUnderRemoteControl;
	WORD vGlobalPasswordTimeOut;
	BYTE vGlobalInAutoSize;
	bool vGlobalInAutoExposure;
	bool vGlobalFPGAVersion10Point0OrHigher;
	bool vGlobalFPGAVersion10Point87OrHigher;
	bool vGlobalFPGAVersion11Point0OrHigher;
	bool vGlobalFPGAVersion15Point2OrHigher; //needed for Trio
	bool vGlobalFPGAVersion15Point31OrHigher;
	bool vGlobalFPGAVersion15Point34OrHigher;
	bool vGlobalFPGAVersion15point42orAbove;
	bool vGlobalFPGAVersion16point0orAbove; //needed for X-Scan Detectors
	bool vGlobalFPGAVersion16point7orAbove; //needed for Diverter in Sampling
	bool vGlobalFPGAVersion16Point20OrHigher;
	bool vGlobalFPGAVersion17point0orAbove; //need for HDRX but not used in Solo
	bool vGlobalFPGAVersion18point0orAbove; //need for new DACs on Version 18 uC board

	unsigned long vGlobalDialogTitleColor;
	CString vGlobalIPAddressString;
	CString vGlobalIPSubNetMaskString;
	CString vGlobalIPDefaultGatewayString;
	CString vGlobalIPDNSAddressString1;
	CString vGlobalIPDNSAddressString2;
	BYTE vGlobalIPDHCPEnabled;
	CString vGlobalIPDHCPServer;
	CString vGlobalDHCPString[2];
	BYTE vGlobalPingStatus;
	BYTE vGlobalPingReply;
	CString vGlobalNetworkBoardCode;
	CString vGlobalRegistryKeyPath;
	int vGlobalPasswordLevel;
	CLifeTimeCounters *vGlobalLifeTimeCounters;
	//BYTE vGlobalUseMetric;
	bool vGlobalShowDebugWindows;
	HCURSOR vGlobalCursorSave;
	CSerialPort *vGlobaluCSerialPort;
	CSpellmanDXMXRaySourcePort *vGlobalDXMXRayPort;
	CXRaySource *vGlobalMonoBlockXRayPort;
	CACPowerMoniter *vGlobalACPowerMoniter;

	CSerialPortGuardMaster *vGlobalGuardMaster;
	CSerialPortPowerFlex40 *vGlobalPowerFlex40;
	CSerialPortRFIDEASBadgeReader *vGlobalRFIDEASBadgeReaderSerialPort;
	CScanTracDlg *vGlobalMainWindowPointer;
	BYTE vGlobalDemoMode;
	bool vGlobalRunningCheckDisk;
	BYTE vGlobalShowMouseCursor;
	HBRUSH vGlobalButtonColorBrush;
	//CCriticalSection vGlobalSerialPortSendLock;
	CCriticalSection vGlobalWriteToLogLock;

	//BYTE *vGlobalReceiveDataBufferPointer;
	//WORD *vGlobalReceiveTailPointer;
	//WORD *vGlobalReceiveHeadPointer;
	//BYTE vGlobalReadDataBuffer[cReceiveBufferSize];
	DWORD vGlobalCallingThreadID;
	double vGlobalPixelsPerUnit;
	double vGlobalPixelsPerUnitInHeight;
	bool *vGlobalReadError;
	bool vGlobalInWindowsDialog;
	CImageAquisitionThread *vGlobalImageAquisitionThread;
	CProcessQuickLearnImagesThread *vGlobalProcessQuickLearnImagesThread;
	CProcessEvaluateImagesThread *vGlobalProcessEvaluateImagesThread;
	CBackupHardDriveThread *vGlobalBackupHardDriveThread;
	CDisplayDialogThread *vGlobalDisplayDialogThread;
	CPingTestThread *vGlobalPingTestThread;
	CString vGlobalCurrentDirectory;

	DWORD vGlobalAmountOfFreeMemory;
	BYTE vGlobalGaveMemoryError;
	int vGlobalArchiveVersionNumber;
	BYTE vGlobalUseCommaForDecimalPoint;

	CProduct *vGlobalCurrentProduct;
	HICON vGlobalIcon;
	HICON vGlobalIconSmall;
	BYTE vGlobalShiftKeyDown;
	BYTE vReplyOK;
	DWORD vGlobalProcessAffinityMask ;
	DWORD vGlobalSystemAffinityMask ;
	double vGlobalHardDriveTotalBytes;
	bool vGlobalGaveHardDriveLowOnMemoryWarning;
	DWORD vGlobalFBWFEnable;
	UINT vGlobalProcessTimePercent;
	UINT vGlobalThreadTimePercent;
	HANDLE vGlobalMyProcessHandle;
	HMODULE vGlobalhLibFBWFNetwork;
	CString vFindString;
	unsigned int vGlobalShadowImages;
	// track sensors with no sensor outputs
	unsigned int vGlobalTrackSensors;
	unsigned int vOldGlobalTrackSensors;
	//kjh added for reject directory not available
	int vGlobalCountErrorMsgPossibleDirectoryNotAvailableSent;
	BOOL vGlobalHaveDDrive;

	// kjh added for product slip
	unsigned char vGlobalProductSlipArray[cSlipArraySize];
	int vGlobalProductSlipErrorSent;
	int vGlobalProductSlipCount;
	int vGlobalProductSlipArrayLocation;

	CString vGlobalCurrentUser = "No User";
	CString vGlobalBadgeReaderRFIDString;
	double vGlobalPercentToResizeX;
	double vGlobalPercentToResizeY;
	CFont * vGlobalCurrentFonts;
	CFont vGlobalFixedFont;
	CFont vGlobalTextFont;

/////////////////////////////////////////////////////////////////////////////
// CScanTracApp

BEGIN_MESSAGE_MAP(CScanTracApp, CWinApp)
	//{{AFX_MSG_MAP(CScanTracApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	//ON_COMMAND(ID_HELP, CWinApp::OnHelp)  

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScanTracApp construction

CScanTracApp::CScanTracApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	vGlobalCopyFileCount = 0;
	vGlobalCopyDone = 0;
#ifdef CompileInHTTPClientOperationsGuardian
	vGlobalSendNextOGMessageThread = NULL;
	vGlobalLastOGServerWriteStatusCode;
	vGlobalOGSendMessageErrorCount = 0;
	vGlobalOGSendMessageGoodCount = 0;
	vGlobalOGSendMessageexceptionCount = 0;
	vGlobalOGLastErrorKeyword = L"-";
	vGlobalOGLastErrorMessage = L"-";

	vGlobalLastOGServerErrorCode = -4;
	vGlobalOGMessageSendCounter = 0;
	vGlobalOGQueueHead = 0;
	vGlobalOGQueueTail = 0;
	for (WORD TempLoop = 0;TempLoop < cOGQueueLength; TempLoop++)
	{
		vGlobalOGQueueMessage[TempLoop] = L"-";
		vGlobalOGQueueKeyWord[TempLoop] = L"-";
	}
	vGlobalOGLocationString = "";
	vGlobalOGLimitString = "";
	vGlobalOGErrorCode = 0;
	vGlobalNewOperationsGuardianErrorMessage = "";
#endif

	vGlobalOperationsGuardianServer = cOperationsGuardianDefaultServerName;
	vGlobalClosePasswordDialog = false;
	vGlobalPasswordToEnter = "-";
	vGlobalLanguage = 0;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
		vGlobalEjectorConfirmMask[TempLoop] = 0;

	vGlobalEndOfLineTimeOutMargin = 4;

	HMONITOR hMonitor;
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

	vGlobalDisplaySizeH = 640;
	vGlobalDisplaySizeV = 480;

	if (GetMonitorInfo(hMonitor,lpmi))
	{
		if (lpmi->rcMonitor.right >= 1024)
			vGlobalUse1024By768Display = true;
		vGlobalDisplaySizeH = (WORD)lpmi->rcMonitor.right;
		vGlobalDisplaySizeV = (WORD)lpmi->rcMonitor.bottom;
	}

	//screen resolution for testing in office only next 2 lines
#ifdef _DEBUG
	//vGlobalDisplaySizeH = 1280;
	//vGlobalDisplaySizeV = 1024;
	vGlobalDisplaySizeH = 1024;
	vGlobalDisplaySizeV = 768;
	//vGlobalDisplaySizeH = 640;
	//vGlobalDisplaySizeV = 480;
	//vGlobalDisplaySizeH = 800;  //horizontal resolution
	//vGlobalDisplaySizeV = 600;  //Vertical resolution
#endif
	//for testing only
	//vGlobalDisplaySizeH = 1280;
	//vGlobalDisplaySizeV = 1024;
	vGlobalGaveHardDriveLowOnMemoryWarning = false;

	vGlobalPercentToResizeX = ((double)(vGlobalDisplaySizeH))/((double)(cOldResolutionX));
	vGlobalPercentToResizeY = ((double)(vGlobalDisplaySizeV))/((double)(cOldResolutionY));

	vGlobalConfigurationData = NULL;
	vGlobalRunningData = NULL;
	vGlobalITIPCDig = NULL;
	vGlobalFrameGrabberType = cNoFrameGrabber;
	vGlobalAllowVoidInspection = 0;
	vGlobalIPDHCPEnabled = true;
	vGlobalDHCPString[0] = "Static";
	vGlobalDHCPString[1] = "Dynamic";

	vGlobalPingTestThread = NULL;
	//vGlobalUseHalconLibrary = false;
	vGlobalErrorCount = 0;
	vGlobalRegistryTCPIPKeyPath = '0';
	vGlobalScanTracType = cSoloScanTrac;
	vGlobalTopString = "Top";
	vGlobalBottomString = "Bottom";

	vGlobalCurveFitEquationOrder = 4; //cubic 
  vGlobalCurveFitMaxCall = 100; //100; 
  vGlobalCurveFitEpsilon = .1; //1.e-14;
  vGlobalCurveFitStepBound = 100; // 100
  vGlobalCurveFitFTolerance = 2;//1.e-14; //termination Tolerance
  vGlobalCurveFitXTolerance = 2;//1.e-14;
  vGlobalCurveFitGTolerance = .01;//1.e-14;

	vGlobalPingStatus = cPingThreadNull;

	vGlobalPCRunningWindowsXP = cWindowsUnknown;

	OSVERSIONINFO TempOSInfo;
	TempOSInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (GetVersionEx(&TempOSInfo))
	{
		if (TempOSInfo.dwBuildNumber == 1381)
			vGlobalPCRunningWindowsXP = cWindowsNT;
		else
		if (TempOSInfo.dwBuildNumber == 2600)
			vGlobalPCRunningWindowsXP = cWindowsXP;
		else
		if (TempOSInfo.dwBuildNumber == 7601)
			vGlobalPCRunningWindowsXP = cWindows7;
	}
	vGlobalInAutoSize = false;
	vGlobalInAutoExposure = false;
	vGlobalLearnState = 0;
	vGlobalNormalLearnDilateTimes = 0;
	vGlobalQuickLearnDilateTimes = 0;
	//vGlobalMouseMoveCounter = 0;
	//vGlobalUnderRemoteControl = 0;
	vGlobalPasswordTimeOut = 0;
	vGlobalShowDebugWindows = false;
	vGlobalCursorSave = ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_WAIT)));
	vGlobalButtonColorBrush = ::CreateSolidBrush(cButtonColor);
	vGlobalInWindowsDialog = false;
	vGlobalShiftKeyDown = false;
	vGlobalFPGAVersion10Point0OrHigher = true;
	vGlobalFPGAVersion10Point87OrHigher = true;
	vGlobalFPGAVersion11Point0OrHigher = true;
	vGlobalFPGAVersion15Point2OrHigher = true;
	vGlobalFPGAVersion15Point31OrHigher = true;
	vGlobalFPGAVersion15Point34OrHigher = true;
	vGlobalFPGAVersion15point42orAbove = true;
	vGlobalFPGAVersion16point0orAbove = true;
	vGlobalFPGAVersion16point7orAbove = true;
	vGlobalFPGAVersion17point0orAbove = true;
	vGlobalFPGAVersion16Point20OrHigher = true;
	vGlobalFPGAVersion18point0orAbove = true;

	HANDLE TempMyProcessHandle = GetCurrentProcess();
	//DWORD TempApplicationPriority = GetPriorityClass(TempMyProcessHandle);
	//make ScanTrac highest priority possible
	SetPriorityClass(TempMyProcessHandle,REALTIME_PRIORITY_CLASS);
	//SetPriorityClass(TempMyProcessHandle,HIGH_PRIORITY_CLASS);
	
	//DWORD TempApplicationPriority1 = GetPriorityClass(TempMyProcessHandle);

	//vGlobalUseMetric = false;
	vGlobaluCSerialPort = NULL;
	vGlobalDXMXRayPort = NULL;
	vGlobalACPowerMoniter = NULL;
	vGlobalMonoBlockXRayPort = NULL;
	vGlobalGuardMaster = NULL;
	vGlobalPowerFlex40 = NULL;
	vGlobalRFIDEASBadgeReaderSerialPort = NULL;

	vGlobalMainWindowPointer = NULL;
	vGlobalDemoMode = 0;
	vGlobalRunningCheckDisk = false;
	vGlobalShowMouseCursor = 0;
	//vGlobalReceiveDataBufferPointer = NULL;
	//vGlobalReceiveTailPointer = NULL;
	//vGlobalReceiveHeadPointer = NULL;
	//for (WORD TempLoop = 0; TempLoop < cReceiveBufferSize; TempLoop++)
	//	vGlobalReadDataBuffer[TempLoop] = 0;
	//vGlobalCallingThreadID = 0;
	vGlobalPixelsPerUnit = 32;
	vGlobalPixelsPerUnitInHeight = 32;
	vGlobalReadError = NULL;
	vGlobalImageAquisitionThread = NULL;
	vGlobalProcessQuickLearnImagesThread = NULL;
	vGlobalProcessEvaluateImagesThread = NULL;
	vGlobalDisplayDialogThread = NULL;
	vGlobalCurrentDirectory = "C:\\ScanTrac\\";
	vGlobalAmountOfFreeMemory = 0;
	vGlobalGaveMemoryError = 0;
	vGlobalCurrentProduct = NULL;
	vGlobalArchiveVersionNumber = 0;
	vGlobalUseCommaForDecimalPoint = false;
	vGlobalBackupHardDriveThread = NULL;
	vGlobalPasswordLevel = 0;
	vGlobalDialogTitleColor = cDarkBlue;
	vFindString = "";
	vGlobalhLibFBWFNetwork = NULL;
	vGlobalShadowImages = 0;		//clear all bits
	vGlobalCountErrorMsgPossibleDirectoryNotAvailableSent = 0;

	for (int i=0; i < cSlipArraySize; i++)
	{
		vGlobalProductSlipArray[i] = 0;
	}
	vGlobalProductSlipCount = 0;
	vGlobalProductSlipArrayLocation = 0;
	vGlobalProductSlipErrorSent = 0;
	vGlobalTrackSensors = 0;
	vOldGlobalTrackSensors = 0;

	vGlobalCurrentUser = "No User";
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CScanTracApp object

CScanTracApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CScanTracApp initialization

BOOL CScanTracApp::InitInstance()
{
	// CG: This line was added by the ActiveX Control Containment component
	//AfxGetApp()->DoWaitCursor(
	AfxEnableControlContainer();

	if (!InitATL())
		return false;

	AfxEnableControlContainer();

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		return true;
	}



	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	/*
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	*/

	BOOL vProgramRunning = true;
	int i = 0;
	vGlobalCurrentFonts = (CFont *)malloc(sizeof(CFont) * 9);
	for (i = 0; i < 9; i++)
	{
		vGlobalCurrentFonts[i].DeleteObject();
		switch (i)
		{
		case 0:
			vGlobalCurrentFonts[i].CreatePointFont(int(vGlobalPercentToResizeY * cPunyFont),cThinFont,NULL);
			break;
		case 1:
			vGlobalCurrentFonts[i].CreatePointFont(int(vGlobalPercentToResizeY * cPunyFont),cThinFont,NULL);
			break;
		case 2:
			vGlobalCurrentFonts[i].CreatePointFont(int(vGlobalPercentToResizeY * cTinyFont),cThinFont,NULL);
			break;
		case 3:
			vGlobalCurrentFonts[i].CreatePointFont(int(vGlobalPercentToResizeY * cSmallFont),cThinFont,NULL);
			break;
		case 4:
			vGlobalCurrentFonts[i].CreatePointFont(int(vGlobalPercentToResizeY * cMediumFont),cThinFont,NULL);
			break;
		case 5:
			vGlobalCurrentFonts[i].CreatePointFont(int(vGlobalPercentToResizeY * cNormalFont),cThinFont,NULL);
			break;
		case 6:
			vGlobalCurrentFonts[i].CreatePointFont(int(vGlobalPercentToResizeY * cMediumLargeFont),cThickFont,NULL);
			break;
		case 7:
			vGlobalCurrentFonts[i].CreatePointFont(int(vGlobalPercentToResizeY * cLargeFont),cThickFont,NULL);
			break;
		case 8:
			vGlobalCurrentFonts[i].CreatePointFont(int(vGlobalPercentToResizeY * cExtraLargeFont),cThickFont,NULL);
			break;
		}
		
		if (i != 0)
		{
			LOGFONT TempLogFont;
			memset(&TempLogFont, 0, sizeof(LOGFONT));
			vGlobalCurrentFonts[i].GetLogFont(&TempLogFont);
			TempLogFont.lfWeight = 700;
			vGlobalCurrentFonts[i].DeleteObject();
			vGlobalCurrentFonts[i].CreateFontIndirect(&TempLogFont);
		}
	}

	HWND      TempWindowHandle;
	TempWindowHandle = FindWindow(NULL, _T("Inspx-ScanTrac"));
	if (!TempWindowHandle) 
		TempWindowHandle = FindWindow(NULL, _T("ScanTrac Main"));
	if (!TempWindowHandle) 
		TempWindowHandle = FindWindow(NULL, _T("Version"));
	if (!TempWindowHandle) 
		TempWindowHandle = FindWindow(NULL, _T("Notice"));
	if (TempWindowHandle) 
	{
		LPCTSTR TempTextMessage = _T("ScanTrac is already running\nThis copy is aborting now.");
		LPCTSTR TempTitleMessage =_T( "ScanTrac Notice");
		MessageBox(NULL,TempTextMessage, TempTitleMessage	,0);

		// We found another copy of ourself. Lets bring it up, and exit this copy:
		if (IsIconic(TempWindowHandle)) 
		{
			ShowWindow(TempWindowHandle, SW_RESTORE);
		}
		SetForegroundWindow(TempWindowHandle);
	}
	else
	{
		CTime TempTime;
		TempTime = CTime::GetCurrentTime();
		//check hard drive space free on c drive.  need atleast 25 mega bytes
		LPCTSTR TempDirectory = _T("C:\\");
		DWORD TempSectorsPerCluster = 0;
		DWORD TempBytesPerSector = 0;
		DWORD TempNumberOfFreeClusters = 0;
		DWORD TempTotalNumberOfClusters = 0;

		int TempResults = GetDiskFreeSpace(TempDirectory,&TempSectorsPerCluster, &TempBytesPerSector,&TempNumberOfFreeClusters,&TempTotalNumberOfClusters);

		double TempTotalBytesAvailable = (double)TempNumberOfFreeClusters * (double)TempSectorsPerCluster * (double)TempBytesPerSector;
		DWORD vFBWFEnable ;
		DWORD vFBWFEnableNext;
		DWORD vDirStructureBytes;
		DWORD vFileDataBytes;
		DWORD vCurrentCacheBytes;
		DWORD vNextCacheBytes;

		FBWF_Info * IFBWF_Info;
		IFBWF_Info = new(FBWF_Info);

		if (IFBWF_Info->IsFBWFEnabled(&vFBWFEnable,&vFBWFEnableNext) == NO_ERROR) //if valid response, vFBWFEnable is set
		{
			if (vFBWFEnable != 0)		// if FBWF enabled
			{	// get memory usage, if valid use it
				if (IFBWF_Info->MemoryUsageFBWF(&vDirStructureBytes,&vFileDataBytes,&vCurrentCacheBytes,&vNextCacheBytes) == NO_ERROR)
				{
					vGlobalHardDriveTotalBytes = vCurrentCacheBytes;	// if FBWF is on set vGlobalHardDriveTotalBytes to FBWF maximum 
				}
			}
		}
		else
		{
			vFBWFEnable = 0;	// if the call fails, set FBWF enabled to false
		}

		vGlobalFBWFEnable = vFBWFEnable;		//global FBWF flag set

		delete IFBWF_Info;

		vGlobalhLibFBWFNetwork = NULL;

		if (TempTotalBytesAvailable < 25000000)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\nHard Disk Full.\nDelete some Files on Hard Disk.\nCall Service to configure Auto File Delete.\nThen run ScanTrac.";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		//else //more than 25 megabytes free

		//if the year is larger than 2038, then checking file status will give an exception
		//and crash the application.  Need to change way check file status, or fix Visual Studio code.
		if (TempTime <= 0) // if the year is 2038, temp time is negative because variable is too small to hold year
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\nDate in PC Corrupted, Date must be Reset.\nIf this happens again, replace Coin Battery on Motherboard.\n On Start Menu, right click on time to set date correctly.\nThen run ScanTrac.";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		else //if (TempTime > 0)
		{
			vGlobalIconSmall = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
			vGlobalIcon = AfxGetApp()->LoadStandardIcon(IDI_APPLICATION);

			CString TempText("");
			TempText.LoadString(IDS_VersionDisplayString);
			if ((vGlobalDisplaySizeH != 1024) && (vGlobalDisplaySizeV != 768))
				TempText = " \n " + TempText;

			CString TempText1("");
			TempText1.LoadString(IDS_StartingSystemPleaseWait);
			TempText1	= TempText + "\n" + TempText1;

			CNoticeDialog vVersionNoticeDialog;
			vVersionNoticeDialog.vNoticeText = TempText1;
			vVersionNoticeDialog.vType = cVersionMessage;
			vVersionNoticeDialog.Create(IDD_NoticeDialog,NULL);
			vVersionNoticeDialog.vAutoClose = 10000;
			vVersionNoticeDialog.ShowWindow(SW_SHOW);

			CNoticeDialog vVersionNoticeDialog1;
			vVersionNoticeDialog1.vNoticeText = TempText;
			vVersionNoticeDialog1.vType = cVersionMessage;
			vVersionNoticeDialog1.vAutoClose = 1000;
			vVersionNoticeDialog1.DoModal();


			dlg = new CScanTracDlg;
			int nResponse = dlg->DoModal();
			if (nResponse == IDOK)
			{
				// TODO: Place code here to handle when the dialog is
				//  dismissed with OK
			}
			else if (nResponse == IDCANCEL)
			{
				// TODO: Place code here to handle when the dialog is
				//  dismissed with Cancel
			}
		}
//		else
//		{
//			LPCTSTR TempTextMessage = "Invalid Date.  Set Date, then run ScanTrac";
//			LPCTSTR TempTitleMessage = "ScanTrac Notice";
//			MessageBox(NULL,TempTextMessage, TempTitleMessage	,0);
//		}
		//free(dlg);
		// Since the dialog has been closed, return false so that we exit the
		//  application, rather than start the application's message pump.
	}
	return false;
}

void CScanTracApp::AddToRecentFileList(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CWinApp::AddToRecentFileList(lpszPathName);
}

BOOL CScanTracApp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
#ifdef CompileInHTTPClientOperationsGuardian
	if (pMsg->message == cOGSevereMessage)
	{
		dlg->SendOGSevereEvent(pMsg->wParam);
		return true;  //true indicates it has been handled, so do not process
	}
	else
#endif
	if (pMsg->message == cRFIDEASBadgeReadMessage)
	{
		dlg->HandleRFIDEASBadgeRead(); 
		return true;  //true indicates it has been handled, so do not process
	}
	if (pMsg->message == WM_KEYDOWN)
	{
		if (vGlobalInWindowsDialog)
		{
			if (pMsg->wParam == 0x21)
				pMsg->wParam = 0x1b;
		}
	}
	else
	if (pMsg->message == cSimulateOneContainerIn10MSMessage)
	{
		dlg->SimulateOneContainerIn10MS(pMsg->wParam);
		return true;  //true indicates it has been handled, so do not process
	}
	else
	if (pMsg->message == cKillCalibrationTimerMessage)
	{
		dlg->KillReCalibrationTimer();
		return true;  //true indicates it has been handled, so do not process
	}
	else
	if (pMsg->message == cStartChatWindowMessage)
	{
		dlg->ChatWindow();
		return true;
	}
	else
	if (pMsg->message == cDownStreamSignalChangedMessage)
	{
		dlg->DownStreamSignalChanged(pMsg->lParam);
		return true;
	}
	else
	if (pMsg->message == cTurnOffEjectorMessage)
	{
		dlg->TurnOffEjectorIn100mSec(pMsg->lParam);
		return true;
	}
	else
	if (pMsg->message == cEmergencyShutXRaysDownMessage)
	{
		dlg->EmergencyShutXRaysDown(pMsg->lParam);
		return true;
	}
	else
	if (pMsg->message == cCheckACBeforeYellowMessage)
	{
		dlg->CheckACIn3SecondsBeforeYellowMessage(pMsg->lParam);
		return true;
	}
	else
	if (pMsg->message == cKillCheckACBeforeYellowMessage)
	{
		dlg->KillCheckACBeforeYellowMessage(pMsg->lParam);
		return true;
	}
	else
	if (pMsg->message == cInitializeDXMMessage)
	{
		//newimage newframe to display on screen
		dlg->InitializeDXM();
		return true;
	}
	else
	if (pMsg->message == cTurnDiverterOnMessage)
	{
		dlg->SetTurnDiverterOnTimer();
		return true;
	}
	else
	if (pMsg->message == cCheckLearnQualityMessage)
	{
		dlg->CheckLearnQuality();
		return true;  //true indicates it has been handled, so do not process
	}
	else
	if (pMsg->message == cCheckHVPSVoltageSoonMessage)
	{
		dlg->CheckHVPSVoltageSoon();
		return true;  //true indicates it has been handled, so do not process
	}
	else
	if (pMsg->message == cUpdateTitleBarMessage)
	{
		dlg->UpdateTitleBar();
		//don't say handled so open window can get message too
		//return true;  //true indicates it has been handled, so do not process
	}
	else
	if (pMsg->message == cOffsetCalibrationDoneMessage)
	{
		if (pMsg->wParam == 2)
			dlg->EndCalibrateDACDetectorOffset(false);
		else
		if (pMsg->wParam == 1)
			dlg->EndCalibrateADCDetectorOffset(false);
		return true;
	}
	else
	if (pMsg->message == cTurnDiverterOffMessage)
	{
		dlg->SetTurnDiverterOffTimer();
		return true;
	}
	else
	if (pMsg->message == cDensityLoopLockedMessage)
	{
		dlg->QuickSetupStartAutoReLearnIfNeeded(pMsg->wParam);
		return true;
	}
	else
	if (pMsg->message == cDisplayRejectHiddenMessage)
	{
		dlg->DisplayRejectHiddenMessage(pMsg->wParam);
		return true;
	}
	else
	if (pMsg->message == cIndexPulseErrorMessage)
	{
		dlg->ShowIndexPulseError(pMsg->wParam);
		return true;
	}
	else
	if (pMsg->message == cShowSlippedMessage)
	{
		dlg->ShowSlippedIndicator();
		return true;
	}
	else	
	if (pMsg->message == cEnableEjectorsMessage)
	{
		dlg->EnableDisableEjectors(true);
		return true;
	}
	else	
	if (pMsg->message == cDisableEjectorsMessage)
	{
		if (pMsg->wParam)
			dlg->DisableEjectorsInXSeconds(pMsg->wParam);
		else
			dlg->EnableDisableEjectors(false);
		return true;
	}
	else	
	if (pMsg->message == cStartPasswordTimerMessage)
	{
		dlg->StartPecoPasswordTimeout();
		return true;
	}
	else	
	if (pMsg->message == cCheckInterlocksIn200MSMessage)
	{
		dlg->CheckInterlocksIn200MS();
		return true;
	}
	else
	if (pMsg->message == cuControllerReadyToRunMessage)
	{
		dlg->StartRunning(true, true);
		//clear yellow status line message
		dlg->ClearYellowWarningButton();
		return true;
	}
	else
	if (pMsg->message == cCheckLampsMessage)
	{
		dlg->CheckLamps();
		//clear yellow status line message
		return true;
	}
	else
	if (pMsg->message == cNextSourceRampStepMessage)
	{
		dlg->NextSourceRampUpStep();
		return true;
	}
	else
	if (pMsg->message == cuControllerStopRunningMessage)
	{
		//dlg->DisplayWaitToSetupProduct();
		dlg->StopRunning(true);
		//dlg->CloseWaitToSetupProduct();
		return true;
	}
	else
	if (pMsg->message == cuControllerResetMessage)
	{
		dlg->HandleuControllerReset(pMsg->wParam);
		return true;
	}
	else
	if (pMsg->message == cRampingSourceMessage)
	{
		dlg->RampingSource();
		return true;
	}
	else
	if (pMsg->message == cQuickLearnProcessingFinishedMessage)
	{
		dlg->QuickLearnFinished();
		return true;
	}
	else
	if (pMsg->message == cHiddenKeyPressed)
	{
		dlg->HandleHiddenKeyPress(pMsg->wParam, pMsg->lParam);
		return true;
	}
	else
	if (pMsg->message == cDisplaySoftKeyMenu)
	{
		dlg->DisplaySoftKeyMenu(pMsg->wParam);
		return true;
	}
	else
		
	if (pMsg->message == cSoftKeyPressed)
	{
		dlg->HandleSoftKeyPress(pMsg->wParam, pMsg->lParam);
		return true;
	}
	
	else
	if (pMsg->message == cShowHelp)
	{
		dlg->ShowHelp(pMsg->wParam, pMsg->lParam);
		return true;
	}
	else
	if (pMsg->message == cAuxiliaryDetectorEjectMessage)
	{
		dlg->ShowAuxiliaryDetectorEject(pMsg->wParam,pMsg->lParam);
		return true;
	}
	else
	if (pMsg->message == cCheckLearningMessage)
	{
		dlg->CheckLearningStatus();
		return true;
	}
	return CWinApp::PreTranslateMessage(pMsg);
}

CScanTracModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

LONG CScanTracModule::Unlock()
{
	AfxOleUnlockApp();
	return 0;
}

LONG CScanTracModule::Lock()
{
	AfxOleLockApp();
	return 1;
}
LPCTSTR CScanTracModule::FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
	while (*p1 != NULL)
	{
		LPCTSTR p = p2;
		while (*p != NULL)
		{
			if (*p1 == *p)
				return CharNext(p1);
			p = CharNext(p);
		}
		p1++;
	}
	return NULL;
}


int CScanTracApp::ExitInstance()
{
	if (m_bATLInited)
	{
		_Module.RevokeClassObjects();
		_Module.Term();
		CoUninitialize();
	}

	return CWinApp::ExitInstance();

}

BOOL CScanTracApp::InitATL()
{
	m_bATLInited = true;

#if _WIN32_WINNT >= 0x0400
	HRESULT hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
	HRESULT hRes = CoInitialize(NULL);
#endif

	if (FAILED(hRes))
	{
		m_bATLInited = false;
		return false;
	}

	_Module.Init(ObjectMap, AfxGetInstanceHandle());
	_Module.dwThreadID = GetCurrentThreadId();

	LPTSTR lpCmdLine = GetCommandLine(); //this line necessary for _ATL_MIN_CRT
	TCHAR szTokens[] = _T("-/");

	BOOL bRun = true;
	LPCTSTR lpszToken = _Module.FindOneOf(lpCmdLine, szTokens);
	while (lpszToken != NULL)
	{
		if (lstrcmpi(lpszToken, _T("UnregServer"))==0)
		{
			_Module.UpdateRegistryFromResource(IDR_SCANTRAC, false);
			_Module.UnregisterServer(true); //true means typelib is unreg'd
			bRun = false;
			break;
		}
		if (lstrcmpi(lpszToken, _T("RegServer"))==0)
		{
			_Module.UpdateRegistryFromResource(IDR_SCANTRAC, true);
			_Module.RegisterServer(true);
			bRun = false;
			break;
		}
		lpszToken = _Module.FindOneOf(lpszToken, szTokens);
	}

	if (!bRun)
	{
		m_bATLInited = false;
		_Module.Term();
		CoUninitialize();
		return false;
	}

	hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER, 
		REGCLS_MULTIPLEUSE);
	if (FAILED(hRes))
	{
		m_bATLInited = false;
		CoUninitialize();
		return false;
	}	

	vGlobalProcessAffinityMask = 0x01;
	vGlobalSystemAffinityMask = 0x01;

	vGlobalMyProcessHandle = GetCurrentProcess();
	if (!(::GetProcessAffinityMask(vGlobalMyProcessHandle ,&vGlobalProcessAffinityMask , &vGlobalSystemAffinityMask)))
	{
		vGlobalProcessAffinityMask = 0x01 ;
		vGlobalSystemAffinityMask = 0x01 ;
	}

	HANDLE TempHandle = GetCurrentThread();
	DWORD TempWord = 0x02;	//AFFINITYMASK_MAIN_WINDOW 2
	TempWord &= vGlobalSystemAffinityMask;
	/*
	if ((TempWord != 0) && (vGlobalSystemAffinityMask >= 0x0F))
	{
		if (vGlobalSystemAffinityMask == 0xFF)
			DWORD TempOriginalAffinityMask = SetThreadAffinityMask(TempHandle, 0x80);
		else
		if (SetThreadAffinityMask(TempHandle,TempWord))
		{
			vReplyOK |= 0x08;
		}
	}
	*/

	return true;

}

