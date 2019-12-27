//ScanTrac Side View Source File
// ScanTracDlg.h : header file
//


#include "Product.h"
#include "Container.h"
#include "BodyTrigger.h"
#include "ExternalDetector.h"
#include "Ejector.h"
#include "Win32SerialPort.h"
#include "SystemConfigurationData.h"
#include "NoticeDialog.h"
#include "TCPIPServerSocket.h"
#include "ListeningSocket.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <string>

#ifdef CompileInHTTPClientOperationsGuardian
#include "SendNextOGMessageThread.h"
#endif
#include "ModBusTCPIPInterface.h"
#include "afxwin.h"

#if !defined(AFX_SCANTRACDLG_H__1C80CF1B_CEB9_11D3_ABCC_00500466E305__INCLUDED_)
#define AFX_SCANTRACDLG_H__1C80CF1B_CEB9_11D3_ABCC_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef CTypedPtrArray<CObArray, CProduct*> tProductCollection;
typedef CTypedPtrList<CObList, CContainer*> tContainerCollection;
typedef CTypedPtrArray<CObArray, CWnd*> tCWndCollection;

const BYTE cSendCommandSleepTime = 1; //was 1
const BYTE cSendCommandSleepTimeSetup = 5; // was 5

enum {cNoPipeStatus,cPipeEmpty,cPipePumping,cPipePumpStopped};

//cCritical errors cause a red dialog to pop up and show on the screen until any key closes
//cError causes the Critical Alarm Opto output to go active, it goes inactive when yellow message is cleared
//certain codes to not get written to the log file (used for hard drive full type messages)

enum {cCritical,cError,cWrongSizeNotice,cEMailPlantOperations,cEMailInspx, cEMailMaintenance,cEMailReport, cReject, cAction, cWriteToLog, 
	cUserAction, cUserChanges};

enum {cNoPassword, cSparePassword1, cSparePassword2, cSparePassword3, cSparePassword4, cSparePassword5, cAnyPasswordToEnter, cSparePassword6, cSparePassword7, cSparePassword8,
	cSparePassword9, cSparePassword10, cOperatorPassword, cSparePassword11, cSparePassword12, cSparePassword13, cSparePassword14, cSparePassword15, cResetPassword,
	cSparePassword16, cSparePassword17, cSparePassword18, cSparePassword19, cSparePassword20, cSamplePassword, cSparePassword21, cSparePassword22, cSparePassword23, 
	cSparePassword24, cSparePassword25, cSimpleSetupPassword, cSparePassword26, cSparePassword27, cSparePassword28, cSparePassword29, cSparePassword30, cSetupPassword, 
	cSparePassword31, cSparePassword32, cSparePassword33, cSparePassword34, cSparePassword35, cQAPassword, cSparePassword36, cSparePassword37, cSparePassword38, cSparePassword39,
	cSparePassword40, cTemporaryAdvancedPassword, cSparePassword41, cSparePassword42, cSparePassword43, cSparePassword44, cSparePassword45, cAdvancedSetupPassword, 
	cSparePassword46, cSparePassword47, cSparePassword48, cSparePassword49, cSparePassword50, cCertifiedPassword, cSparePassword51, cSparePassword52, cSparePassword53,
	cSparePassword54, cSparePassword55, cTemporaryInspxPassword, cSparePassword56, cSparePassword57,cSparePassword58,cSparePassword59,cSparePassword60, cAllansPassword,
	cSparePassword61, cSparePassword62, cSparePassword63, cSparePassword64, cSparePassword65, cInspxPassword, cSparePassword66, cSparePassword67, cSparePassword68, cSparePassword69, cSparePassword70,
	cSuperInspxPassword};

enum SystemShutDownMode {cShutDown = 1, cReboot, cPowerOff, cLogOff};

enum {FileNameNotFound, FileNameFoundOnce, FileNameFoundTwice};

enum {cNoCommSeenEver, cYesCommSeen, cNoCommPortFound };
//	cUseDefaultSettings = Done search, port not found, look for default becoming active

 /* kjh for screen capture Error constants */ 
 enum {       
 ERR_MIN = 0,                     // All error #s >= this value       
 ERR_NOT_DIB = 0,                 // Tried to load a file, NOT a DIB!       
 ERR_MEMORY,                      // Not enough memory!       
 ERR_READ,                        // Error reading file!       
 ERR_LOCK,                        // Error on a GlobalLock()!       
 ERR_OPEN,                        // Error opening a file!       
 ERR_CREATEPAL,                   // Error creating palette.       
 ERR_GETDC,                       // Couldn't get a DC.       
 ERR_CREATEDDB,                   // Error create a DDB.       
 ERR_STRETCHBLT,                  // StretchBlt() returned failure.       
 ERR_STRETCHDIBITS,               // StretchDIBits() returned failure.       
 ERR_SETDIBITSTODEVICE,           // SetDIBitsToDevice() failed.       
 ERR_STARTDOC,                    // Error calling StartDoc().       
 ERR_NOGDIMODULE,                 // Couldn't find GDI module in memory.       
 ERR_SETABORTPROC,                // Error calling SetAbortProc().       
 ERR_STARTPAGE,                   // Error calling StartPage().       
 ERR_NEWFRAME,                    // Error calling NEWFRAME escape.       
 ERR_ENDPAGE,                     // Error calling EndPage().       
 ERR_ENDDOC,                      // Error calling EndDoc().       
 ERR_SETDIBITS,                   // Error calling SetDIBits().       
 ERR_FILENOTFOUND,                // Error opening file in GetDib()       
 ERR_INVALIDHANDLE,               // Invalid Handle       
 ERR_DIBFUNCTION,                 // Error on call to DIB function       
 ERR_MAX                          // All error #s < this value      
 
 };   

// #defines for screen capture
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B') 

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) 
#define cProductSlippedHere 1
#define cProductDidNotSlipHere 0
#define cTimesToTryPorts 10

extern HBRUSH vGlobalButtonColorBrush;
//extern BYTE vGlobalShowMouseCursor;
//extern HCURSOR vGlobalCursorSave;
//extern DWORD vGlobalAmountOfFreeMemory;

/////////////////////////////////////////////////////////////////////////////
// CScanTracDlg dialog

class CScanTracDlg : public CDialog
{
public:
	//main data variables
	BYTE vEjectorNumberToTurnOff;
	BYTE vSupportContractStatus;
	DWORD vOldLearnCount;
	tProductCollection vProductCollection;
	tContainerCollection vContainerCollection;
	CScanTracSystemRunningData vSystemData;
	CSystemConfigurationData *vConfigurationData;
#ifdef CompileInHTTPClientOperationsGuardian
	int vOldLastOGServerWriteStatusCode;
	CString vLastOGSensitivity;
	UINT vLastOGBeltRate;
	UINT vLastOGContainerRate;
	WORD vLastOGShiftRateCPM;
	unsigned long int vLastOGRunDuration;
	unsigned long int vLastOGEjectCountTotal;
	unsigned long int vLastOGTotalContainers;
	unsigned long int vLastOGTimeSinceLastStop;
	unsigned long int vLastOGXRaySourceRunDuration;
	unsigned long int vLastOGRejectCountTotal;
	unsigned long int vLastOGStopDuration;
	double vLastOGPercentEjected;
	double vLastOGPercentRejected;
	bool vGaveOGOverFlowError;
#endif


	//minor variables
	int vHoldDriftCompensationADCOffset;
	int vHoldDriftCompensationDACOffset;
	BYTE vOldPipeStatus;
	CString vProductionReportString;
	long vLastProductionReportCount;
	BYTE vLastProductionReportType;
	//CWnd *vMostRecentActiveWindow;
	//CString vMostRecentActiveWindowName;
	//CString vPreviousActiveWindowName;
	//WORD vScanTracIsNotTheActiveWindow;
	bool vEveryFourthSecond;

	bool vAbortCalibration;
	bool vOldProcessingQuickLearn;
	bool vGaveHardDriveFailedMessageToday;
	bool vConditioningBeforeRun;
	bool vRunButtonPressedLastNotStop;
	BYTE vBackgroundCount;
	WORD vNumberOfBadPixels;
	CString vCalibratePixelResultsDisplayString;
	bool vShowingPipeEmpty;
	bool vShowingAirInPipe;
	CString vLanAdaptorIDString;
	bool vSelectProductAgain;
	bool vOldDownStreamSignalSaysConveyorOn;
	BYTE *TempReadBuffer;
	CString WriteARejectToTheHardDriveLastTimeString;
	WORD WriteARejectToTheHardDriveLastMultipleSecondFileCount;

	BYTE vLastLearnState;
	BYTE vBackupHardDisk;
	bool vOldAdjustingBrightnessBeforeLearning;
	CString vOldCPMString;
	//BYTE vHaveCheckWeighInspection;
	BYTE vNumberOfWeighInspections;
	bool vShowuControllerStatusInRed;
	unsigned long int vOldCPMTotalContainers;
	bool vFirstHalfSecond;
	CString vDialogTitleString;
	//WORD vOldUnderRemoteControl;

	WORD vBodyTriggerActiveTimer;
	bool vGaveBodyTriggerJammedMessage;
	BYTE vShutterFailedLastTest;
	BYTE vProcessCount;
	double vBackupDetectorGain[cMaximumNumberOfDetectors12];
	double vBackupDetectorEdgeAdjustGain[cNumberOfPixelsToAdjustAllPixels1152];
	double vBackupDetectorEdgeAdjustOffset[cNumberOfPixelsToAdjustAllPixels1152];
	bool vOldNoCommunicationsWithDXM;
	CString vPasswordEntryString;
	bool vEnteringPecoPasswordMode;
	CStringList vSocketMessageHistoryList; //this is all the messages saved to send to new clients

	CListeningSocket* vListeningSocket;
	CPtrList vClientConnectionList;
	//CTCPIPServerSocket vTCPIPServerSocket;

	int vLastHour;
	int vLastMinute;
	bool vHaveRejects;
	DWORD vTriggerIndicatorCount;
	DWORD vOldTriggerIndicatorCount;
	WORD vCPMNotRunning;
	WORD vOldCPMNotRunning;
	//bool vNewDisplay;
	//bool vHaveNotBackedUpYet;
	BYTE vHoldSuppressWarningMessages;
	BYTE vOldTriggerIndicatorValue;
	BYTE vTriggerIndicatorValue;
	BYTE vOldShuttingDownIndicatorValue;
	BYTE vShuttingDownIndicatorValue;
	int vOldGlobalPasswordOK;
	bool vOldAverageEnabled;
	bool vOldStructureEnabled;
	bool vOldCalibrateBodyTriggerReferenceEnabled;

	CNoticeDialog *vBackupNoticeDialog;
	bool vTurnOffSourceWhenStopRunning;

	CString vTheErrorMessageStringUIT[cMaximumNumberOfErrorMessages];
	BYTE vTheErrorMessageTypeUIT[cMaximumNumberOfErrorMessages];
	WORD vTheErrorMessageCodeUIT[cMaximumNumberOfErrorMessages];
	WORD vErrorMessageHeadUIT;
	WORD vErrorMessageTailUIT;

	CString vEMailMessage;
	BYTE vEMailType;
	int vMainImageRejectReason;
	CString vMainImageWeight;
	BYTE vMainImageRejectDisable;
	BYTE vMainImageLane;
	//BYTE vMainImageImproveLearn;
	bool vMainImageRejected;
	WORD vTest;
	BYTE vOldTestModesActive;
	WORD vOldSamplesToGo;
	bool vAutoProductLoad;
	CNoticeDialog *vWaitToLoadProductNoticeDialog;
	bool vReportedBeltSpeedTooFast;
	bool vUpdateGainAndOffsetConfiguration;
	WORD vCalibrateClipboardNumberOfLines;
	CProduct *vCalibrateHoldCurrentProduct;
	CFile *vCalibrationDataFile;

	double *vCalibrationAverageImage;
	double *vCalibrationAverageImageMaximum;
	double vCalibrateCurrent;
	double vCalibrateCurrentStep;
	//float vCalibrateProductCurrent;
	bool vOldXRayOnNotice;
	CString vLastDateString;
	double vLastTimeShowedBeltRate;
	DWORD vOldAmountOfFreeMemory;
	double vLastTimeReadEncoderRate;
	double vThisTimeReadEncoderRate;
	BYTE *TestBC;
	CString TempText;
	bool vDisplayingVersion;

	//LPTSTR vCopyDirectoryFromLPTSTR;
	CString vCopyDirectoryFromString;
	CString vCopyDirectoryToString;
	WORD vCopyDirectoryNextFileIndex;
	WIN32_FIND_DATA vCopyDirectoryFindFileData;
	HANDLE vCopyDirectoryFindHandle;
	unsigned int vXRayTubeHasCooledTimerHandle;
	unsigned int v50HertzTimerHandle;
	unsigned int vOneHourUpdatesTimerHandle;
	unsigned int vBackupToBackupHardDriveTimer;
	unsigned int vSerialPortTimerHandle;
	unsigned int vCopyNextFilesTimerHandle;
	unsigned int vTurnDiverterOnTimerHandle;
	unsigned int vTurnDiverterOffTimerHandle;
	unsigned int vTriggerIndicatorTimerHandle;

	unsigned int vRampXRayTimerHandle;
	unsigned int vSplashScreenTimerHandle;
	unsigned int vSetWindowActiveTimerHandle;
	unsigned int vGoodCountTimerHandle;
	unsigned int vBeltPositionTimerHandle;
	unsigned int vUpdateDisplayTimerHandle;
	unsigned int vCheckForErrorMessagesTimerHandle;
	unsigned int vEjectCountTimerHandle;
	unsigned int vSendSyncCommandTimerHandle;
	unsigned int vSimulateOneContainerTimer;
	unsigned int vShowSafetyViolationMessageTimer;
	unsigned int vSourceStabilizeTimerHandle;
	unsigned int vCheckHVPSVoltageSoonTimerHandle;
	unsigned int vWaitForScreenUpdateTimerHandle;
	unsigned int vShowSourceStrengthTimerHandle;
	unsigned int vSetupProductTimerHandle;
	unsigned int vCheckLampsTimerHandle;
	unsigned int vStopRunningTimerHandle;
	unsigned int vCheckHardDrivesTimerHandle;
	unsigned int vExitTimerHandle;
	unsigned int vConveyorStartTimerHandle;
	unsigned int vEndCalibrateDetectorsTimerHandle;
	unsigned int vEndMeasureSourceStrengthTimerHandle;
	unsigned int vBackupTimerHandle;
	unsigned int vEnableErrorMessagesTimerHandle;
	unsigned int vReCalibrateOffsetTimerHandle;
	unsigned int vClearSlippedTimerHandle;
	unsigned int vConveyorStartStopSignalOffTimerHandle;
	unsigned int vClearRejectHiddenTimerHandle;
//	unsigned int vScreenShot1TimerHandle;
//	unsigned int vScreenShot2TimerHandle;
	unsigned int vCheckACBeforeThisYellowMessageTimerHandle;
	unsigned int vDisableEjectorsTimerHandle;
	unsigned int vOperatorPasswordTimerHandle;
	unsigned int vCheckInterlocksIn200MSTimerHandle;
	unsigned int vSetupDataBackupTimerHandle;
	unsigned int vFlashYellowLightTimerHandle;
	unsigned int vClearScreenCaptureNoticeTimerHandle;
	unsigned int vCheckForNewImageToDisplayTimerHandle;
	unsigned int vStartRunningTimerHandle;
	unsigned int vWaitToCalibrateForXRaysToSettleTimer;
	unsigned int vTestDACsTimerHandle;
	unsigned int vTestADCsTimerHandle;
	unsigned int vNeedToTestDACsTimerHandle;
	unsigned int vSendPrintScreenMessageSoon;
	unsigned int vTurnOffEjectorIn100mSecTimerHandle;
	bool vTrackingCheckWeighMinuteData;

	double vLast10MilliSecondTimerTime;
	double vLast10MilliSecondSpan;
	double vMaximum10MilliSecondSpan;
	double vOldMaximum10MilliSecondSpan;
	double vOldLast10MilliSecondSpan;
	BYTE vAirConditioningLogSecond;

	BYTE vStopRunningTimerActive;
	bool vWaitForScreenUpdate;
	CObject *vReadInConfigurationDataPointer;
	CObject *vReadInLifeTimeCounterDataPointer;

	RECT ItemRECTArray[200];
	tCWndCollection vLocalCWndCollection;

	CBitmap vInspxGraphicRedBigger, vInspxGraphicBlueBigger, vInspxGraphicYellowBigger;
	bool vSetGraphicRed1, vSetGraphicRed2, vSetGraphicYellow1, vSetGraphicYellow2, vSetGraphicBlue;

	//added to access affinity
	unsigned int vAfinityImage;
	DWORD TempAfinity;
	int vReplyOK;

	// kjh added for cpu usage of Acquision Thread report once if over 90%
	bool vGlobalFlagSendOnceThreadError;
	HMODULE hCT; //pointer to DLL to get temperature


	// kjh added for screen capture
	BOOL gbIsCaptured;
	BOOL gbDrawImage;
	HBITMAP     ghBitmap;

	//kjh added for product code restore if cancel load
	CString vSaveProductCodeTemp;

	// track sensors
	int vTemperatureSecondTime;
	
	// kjh added for track ACPower Monitor status
	int vACPowerMonitorTroubleCount;
	int vACPowerMonitorDoesNotExistsCount;
	bool vSubFunction4ButtonEnable;

	// kjh 8/28/2014 track Xray port initialization
	int vXRayPortTry;
	int vDXMPortInitState;
	int vMonoPortInitState;

	// port communications for AC monitor new serial port if needed
	int vACPowerMoniterPortCommCheckState;
	int vCheckPortsCount;
	int vDisplayPortVerifyMessage;

	// check if rejectcounts have changed before sending using this
	CModBusTCPIPInterface *vModBusTCPIPInterfaceThread;
	std::string oldcriteriaJSON;

	// Construction
	CScanTracDlg(CWnd* pParent = NULL);	// standard constructor
	DECLARE_DYNCREATE(CScanTracDlg)

	//Methods
	void AddProduct(CProduct *TempProduct);
	CProduct *GetProductByNumber(int TempProductNumber);
	CProduct *GetProductByName(CString TempProductName);
	bool ReadInConfigurationDataFromFile(bool TempReadBackup);
	//int ReadConfigurationDataFromINIFile();
	void WriteConfigurationDataToFile(bool TempWriteToBackup); 
	//int WriteConfigurationDataToINIFile();

	void SendProductSetupToUController(bool TempLeaveSourceOn);
	void SendInitializationToUController(bool TempLeaveSourceOn);
	void SendSynchronizeCommand();
	void SendConveyorOnOffToUController(BYTE TempOn);
	void SendImageToImageDelay(WORD TempDelay); //number of encoder pulses between images
	void SendManualConveyorOnOffToUController(BYTE TempOn);
	void SendDiagnosticLineSelect();
	void ReportErrorMessage(CString TempErrorMessage, BYTE TempLevel, WORD TempErrorCode);
	void HandleErrorMessage(CString TempErrorMessage, BYTE TempLevel, WORD TempErrorCode);
	void SetupFrameGrabber();
	void SetupProduct(CProduct *TempProduct, bool TempLeaveSourceOn);
	void SetExternalDetectorDecisionOffset();
	void ShowHelp(long TempContext, long TempType);
	void ShowAuxiliaryDetectorEject(long TempContext, long TempType);
	void ClearVersionDisplay();
	void AskIfShouldExit();
	void StartImageAcquisition();
	void StopImageAcquisition();
	void TurnOnOffFPGASimulatedData(bool TempOn);
	void AskIfShouldResetCounters(bool TempChangedProduct, BYTE TempFrom);
	void ResetCounters();
	void StartRunning(bool TempTurnOnSource, bool TempNormalRunning);
	void StopRunning(bool TempTurnOffSource);
	void FinishStopRunning(bool TempTurnOffSource);
	void CalibrateDetectorEdges(bool TempSaveEdgeCalibrateValues);
	void CalibrateDACDetectorOffset();
	void CalibrateADCDetectorOffset(bool TempAutoCalibrate, bool TempCalibrateWhileRunning);
	//void SetCalibrateStep();
	void MeasureSourceStrength();
	void LoadMeasureSourceStrengthProduct();
	void ProcessCalibrateDetectorGain();
	void ProcessCalibrateDetectorEdgeFrame();
	void ProcessMeasureSourceStrengthFrame();
	void OutputImageToFile();
	void EndCalibrateDetectorGain(bool TempAbort);
	void CalibrateDetectorGain();
	void EndCalibrateDetectorEdges(bool TempAbort);
	void EndCalibrateDACDetectorOffset(bool TempAbort);
	void EndCalibrateADCDetectorOffset(bool TempAbort);
	void EndMeasureSourceStrength(bool TempAbort);
	void ShowSourceStrengthData();
	void SimulateOneContainer();
	//9/11/2009
	void VerifyScanTracIPAddress();
	//9/11/2009
//	void ParseSettingFromFileString(CString TempStringToParse);
	void UseIPHelperForIPInfo();
	void GetCurrentMachineIPAddressValuesFromRegistry();
	//9/11/2009
	void SetIPSettingsInRegistryAndReboot();
	void CheckIPSettingsAndDisplayInHelpDialog(bool TempDisplayInHelpDialog);	// for tcpip problems debug in XP
	void SimulateOneContainerIn10MS(DWORD TempMilliSeconds);
	void FillInAverageCalibrationImageLine();
	void CalibrationModeDoneCalculateResults();
	void ScaleImageToMainDisplayBuffer();
	void DisplayWaitToSetupProduct();
	void CloseWaitToSetupProduct();
	void UpdateSubFunction8Button();
	void UpdateClearYellowMessageButton();
	void ClearYellowWarningButton();
	void HandleuControllerReset(WORD TempWatchDog);
	void RampingSource();
	void NextSourceRampUpStep();
	void SendADCGainsAndOffsets(bool TempSetAllGainsToTen);
	void CheckForNewImageToDisplay();
	void UpdateDisplay();
	void TerminateImageAquisitionThread();
	void RemoveAllContainers();
	bool InterlocksOK();
	void SetupExternalDetectors();
	void OneSecondDisplayUpdate();
	void SendEMail(CString TempString, BYTE TempLevel);
	void GenerateProductionReport(BYTE TempReason);
	void SendProductionReport(BYTE TempReason);
	void SaveReportStartTime();
	void ChangeDisplays();
	void CheckLamps();
	void DoDiagnostics();
	void ExitProgram();
	void FinishCalibrateDetectorEdges(bool TempAbort);
	void FinishCalibrateDACDetectorOffset(bool TempAbort);
	void FinishCalibrateADCDetectorOffset(bool TempAbort);
	void FinishMeasureSourceStrength(bool TempAbort);
	void EnableDisableEjectors(bool TempEnable);
	void DisableEjectorsInXSeconds(WORD TempMilliSeconds);
	void BackupSetupFiles(bool BackupHardDiskAlso);
	void MakeSetupBackup();
	void CopyFilesInDirectoryToDirectory(
			CString TempFromDirectory, CString TempToDirectory);
	void DeleteFilesInDirectory(CString TempDirectoryString);
	void EditShift(bool bNewProductSelected);
	void ChatWindow();
	void CopyNextFilesInDirectoryToDirectory();
	void StartPecoPasswordTimeout();
	void CheckForErrorMessages();
	void TurnDiverterOn();
	void SetTurnDiverterOnTimer();
	void TurnDiverterOff();
	void SetTurnDiverterOffTimer();
	void ShowCPM();
	void SetProductDensityDisplay();
	void SendRadiationDoorParameters();
	void UpdateDisableEjectorsButton();
	void CheckLearningStatus();
	void AutoDeleteLogFiles();
	//void DeleteOldLogFiles(BYTE TempMoreThanXDaysOld, CString TempDirectory);
	void SetupRejectReasonNames();
	void SendLinesPerFrame();
	void CreateImageAquisionThread();
	bool HaveRejects(bool TempIncludeGoodImages);//bool TempIncludeImproveLearn);
	void UpdateRunButton();
	BYTE FindLongestWord(CString TempString);
	void SelectCurrentProduct();
	void UpdateRejectReasonOnScreen();
	void ReCalibrateWhileRunning();
	void KillReCalibrationTimer();
	void ShowSlippedIndicator();
	void OpenModBusTCPIPInterface();
	void CloseModBusTCPIPInterface();
	void InitializeTCPIPServerSocket();
	void CloseAllSockets();
	void CloseOneClientSocket(CTCPIPServerSocket* TempClientSocket);
	void ProcessMessageTCPIPServerSocket(CTCPIPServerSocket* TempSocketConnection,
		CSocketMessage *TempMessage);
	void ProcessPendingRead(CTCPIPServerSocket* TempClientSocketConnection);
	void ProcessPendingAccept();
	void OnUpdateConnections();
	void SendSocketMessageToAClient(CTCPIPServerSocket* TempClientSocketConnection, 
																		 CSocketMessage* TempSocketMessage);
	CSocketMessage* ReadMessageFromClientSocket(CTCPIPServerSocket* TempSocketConnection);
	void SendSocketAMessageFromServerToClients(CSocketMessage* TempSocketMessage);
	bool HandlePecoPasswordKeyPress(int TempKey);
	void HandleHiddenKeyPress(int TempKeyDown, int TempKey);
	void NextTubeConditionStep();
	void StartXRayTubeCondition(BYTE TempDoBurnIn, BYTE TempProcedure);
//	void PrintScreen();
//	void PrintScreen1();
//	void PrintScreen2();
	void MarkAllContainersAsConveyorStop();
	void ResetFPGAABFrameCounters();
	void DisplayRejectHiddenMessage(int TempRejectReason);
	void ShowIndexPulseError(int TempSeamer);
	void SetFastestSimulatedEncoder(double TempFactor);
	void TurnSimulatedEncoderOn();
	void TurnSimulatedEncoderOff();
	void TurnSimulatedContainersOff(); 
	void TurnSimulatedContainersOn();
	void LoadCalibrateOffsetProduct();
	void LoadCalibrateEdgesProduct();
	void CheckHVPSVoltageSoon();
	void RestoreOriginalProduct();
	void UpdateTitleBar();
	bool CurrentProductIsInspxProduct();
	void SendXRayPowerOnOffToUController(BYTE TempOn);
	void SendSourceCurrentToUController(double TempCurrent);
	void SendSourceSettingsToUController(double TempVoltage, double TempCurrent);
	void InitializeDXM();
	void EmergencyShutXRaysDown(long TempSafetyViolation);
	void DownStreamSignalChanged(long TempDownStreamOn);
	void TurnOffEjectorIn100mSec(long TempEjectorNumber);
	void CheckACIn3SecondsBeforeYellowMessage(long TempMessageCode);
	void KillCheckACBeforeYellowMessage(long TempMessageCode);
	void ShowSafetyViolationMessage();
	void CreateBackupGainAndPixelAdjustments();
	void RestoreBackupGainAndPixelAdjustments();
	BYTE GetImproveLearnInspectionNumber();
	void EndTestShutter(bool TempAbort);
	void ProcessTestShutter();
	void TestShutter();
	bool PrepareToRun();
	//void MakeGlassImageProcessingImages();
	void ProcessGlassAlgorithm(BYTE *TempImageIn);
	void WriteLifeTimeCounter();
	void ReadLifeTimeCounter();
	void UpdateCustomerNameLine();
	void InitializeADCs();
	void SendProductIntegrationTime(WORD TempIntegrationTime);//, WORD TempIntegrationTimeAtEdge);
	void StartAlignTubeMode();
	//void CheckSynchronizeCommand();
	void CreateProcessQuickLearnImagesThread();
	void CheckForIFC59();
	void UpdateCurrentProductNameDisplay();
	void ClearDesignatedFrameGrabberMemorySettings(bool TempAskIfShouldRebootNow);
	void WriteImagesToFiles(CImageData *TempImage,CString TempNameString);
	void SetInterlockStatus();
	void QuickLearnFinished();
	bool ChangeFilter();
	void SendBodyTriggerInverterControl();
	void StartLearning();
	void QuickSetupStartAutoReLearnIfNeeded(int TempLogStatistics);
	void StartAutoReLearn();
	void SetLearnState();
	void CheckForCorrectMemorySize();
	void CheckLearnQuality();
	void CheckHardDrives();
	BOOL IfSameScanTracDirectories(CString cDirectory1Check,CString cDirectory2Check);
	void StartCopyAllScanTracFoldersToBackupHardDrive();
	void EndCopyAllScanTracFoldersToBackupHardDrive();
	void MoveFoldersToNewFormatIfNeeded();
	//void BackupRegistryToBackupHardDrive();
	void CheckInterlocksIn200MS();
	bool CheckProductFullyConfigured(CProduct *TempProduct);
	bool CheckEjectorsConfigured(CProduct *TempProduct, DWORD TempEjectorUsed);
	void ConvertImagesTo16Bit();
	void ConvertImageFrom8to16bit(CString TempFileNameString);
	void CalculatePipelineEncoderAndTrigger();
	void CheckInspectionNamesForOldInspections(bool TempGiveWarning);
	void WriteARejectToTheHardDrive();
	void SetUsePipeFlowInput(BYTE TempValue);
	void ReadBIOSVersionFromRegistry();
	void SetFirstTraceOfOscilloscopeToContainerTrigger();
	//9/30/2009
	void CheckSystemSettingsAreStandard();
	BYTE TestModesAreActive();
	UINT FindCPUUsage(void);
	//11/5/2009
	void CalculateIfRunningOutOfTime();
	void Send0x9ACommand();
	void ShowOrHideProductDensityCompensation();
	void UpdateProductDensityCompensation();
	bool PowerWiringOK();
	void OnNewImageKey();
	//void ShowPipeEmptyIndicator();
	//void ShowAirInPipeIndicator();
	//void CheckForHalconLibrary();
	void SaveIPSettingsInConfigurationData();
	void CalculateSensitivity();
	void DoBackGroundWork();
	void SetABPixelToOtherEnd(bool TempSetABPixelToOtherEnd);
	void SendAllDACDetectorOffsets();
	void SendAllADCDetectorOffsetsToZero();
	void SendAllADCOffsets();
	bool ConditionTubeIfNeeded();
	void SaveCheckWeighTrendPoint();
	void InitializeDACs();
	void CreateProcessEvaluateImagesThread();
	BYTE WhatIsSupportContractStatus();
	void DisplaySupportContractStatus();
	CString SupportContracExpirationString();
	void ShowSupportPhoneNumber();
	void ShowThresholds();
	void ShowInspxButtons();
	//void CheckIfDetectorCalibrationNeeded();
	void ReadInProductionReport(bool TempFromServer);
	void WriteOutProductionReport(bool TempFromServer);
	void FinalizeProductionReport(bool TempOnFileServer);
	void MakeProductionReportIfNeeded();
	void RenameProductionReportIfNeeded(bool TempOnFileServer);
	bool IfTubeHasBeenOffLongEnough();
	void StartDACandADCVerify();
	void ClearRedStatusIndicator();

	void AllocateValveMonitoringData();
	void ResetValveMonitoringData();
	void ClearValveMonitoringFIFO();
	void ShowValveMonitorData(WORD TempNumberOfDataPoints);
	void SelectValveMonitoringInspection();
	void ShowSamplingIndexErrors(BYTE TempShowFiller);
	void CalculateValveStatus();

	void StartCalibrateDetectorEdgesForXScanPoint4Mode();
	void SendResolutionSettingsTouCHardware();
	void DisplaySoftKeyMenu(BYTE TempKeyDisables);
	void SendSoftKey(BYTE TempKey);
	void HandleClearButtonPress();
	void HandleSoftKeyPress(int TempKey1, int TempKey2);
	void CheckIfShouldMeasureSourceStrength();
	void HandleRFIDEASBadgeRead();

#ifdef CompileInHTTPClientOperationsGuardian
	void SendOGInfo(std::wstring TempKeyWord, CString TempMessage, bool TempMultiItemFormat);
	void SendOGInfoWithoutCurrent(std::wstring TempKeyWord, CString TempMessage, bool TempMultiItemFormat);
	void DisplayOGStatus();
	void CreateOGServerSendMessageThread();
	void CloseOGServerSendMessageThread();
	void SendOGOneMinueData(bool TempSendAll);
	void SendOGTimeZoneData();
	void SendOGSevereEvent(BYTE TempEventType);
#endif
	void DisplayCSVFile();
	void UpdateAllInspectionNames();
	void TickleWatchDogs();
	void SelectLanguage();
	void ShowMultiLaneCounts();
	void UpdateButtons();
	void TestDTOA();
	void SendRetriggerLockout(WORD TempLockout);

	// kjh next 11 functions for screen capture, 11/29/10 some functions combined for clarity
	WORD DibNumColors (VOID FAR * pv);
	WORD PaletteSize (VOID FAR * pv);
//	HANDLE AllocRoomForDIB(BITMAPINFOHEADER bi, HBITMAP hBitmap , HWND hwnd) ;
//	HANDLE ChangeBitmapFormat(HBITMAP hBitmap,WORD wBitCount,DWORD dwCompression,HPALETTE hPal,HWND hwnd);
	WORD SaveDIB(HANDLE hDib, LPWSTR lpFileName);
	HPALETTE GetSystemPalette(HWND hwnd);
//	void WindowCapture(HWND wnd);
//	void UpdateCaptureStatus(BOOL bCaptured);
//	HBITMAP CopyWindowToBitmap(CWnd* wnd , HWND hWnd, WORD fPrintArea);
//	HBITMAP CopyScreenToBitmap(LPRECT lpRect);
//	void DoSize(CWnd* wnd , HBITMAP ghBitmap);
	void TurnOnOffCamLinkTestImages(bool TempTurnOn);
	void SetSimulatedEncoderTo16TimesConveyor();
	double GetDefaultMeasureSourceStrengthVoltage();
	double GetDefaultMeasureSourceStrengthCurrent(BYTE TempHasFilter);
	double GetDefaultAlignTubeVoltage();
	double GetDefaultAlignTubeCurrent(BYTE TempHasFilter);
	void SetMotorSignalSimulatedEncoderRate(WORD TempRate);
	WORD CalculateMaximumEncoderRate(WORD TempProductIntegrationTime);
	void SendPreAmpGain(BYTE TempPreAmplifierGain);
	//void TestingOnlySendSourceCurrentToUController();
	//void TestingOnlySendSourceVoltageToUController();
	bool GetCoreTempInfoAlternative(void);
	void StartDACTest();
	void StopDACTest();
	void StartADCTest();
	void StopADCTest();

	//Filler Monitor an Sampling
	int GetCurrentFillerIndexOffset();
	int GetCurrentSeamerIndexOffset();
	BYTE CalculateHeadNumber(int TempNextHeadToProcess, BYTE TempWhichDevice);

	void SetupBulkModeAndMainMenuScreen();
	void SetupLanguage();
	void DisableCamLinkImages();
	void EnableCamLinkImages();
	void StartCheckWeighingMenu(bool TempGoToOffsetCalibration);
	void ClearXScanProducts();

	// Dialog Data
	//{{AFX_DATA(CScanTracDlg)
	enum { IDD = IDD_ScanTracDlg };
	CStatic	m_NonStandardSystemSetting;
	CStatic	m_DoNotShutOffPower;
	CStatic	m_TimeSinceLastStopStaticText;
	CStatic	m_TimeSinceLastStopLabel;
	CStatic	m_AutoImproving;
	CStatic	m_LearnState;
	CStatic	m_ChatMessage;
	CStatic	m_DensityLockTooFarOff;
	CStatic	m_TotalRejectsPercentLabel;
	CStatic	m_TotalRejectsPercent;
	CStatic	m_DensityNotLockedIndicator;
	CStatic	m_InspxMessage;
	CStatic	m_TriggerIndicator;
	CStatic	m_TotalRejectsStaticText;
	CStatic	m_TotalRejectsLabelStaticText;
	CStatic	m_ThresholdReduction;
	CStatic	m_ThresholdIncrease;
	CStatic	m_DensityCompensation;
	CStatic	m_RejectReason;
	CStatic	m_StatusLine;
	CStatic	m_TestModesActive;
	CStatic	m_EjectorsDisabled;
	CStatic	m_ShowingRejects;
	CStatic	m_Calibrating;
	CStatic	m_XRaysOn;
	CStatic	m_Date;
	CStatic	m_LastAction;
	CDownFunctionButton	m_Function1Button;
	CDownFunctionButton	m_Function2Button;
	CDownFunctionButton	m_Function3Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function5Button;

	CLeftFunctionButton	m_SubFunction1Button;
	CLeftFunctionButton	m_SubFunction2Button;
	CLeftFunctionButton	m_SubFunction3Button;
	CLeftFunctionButton	m_SubFunction4Button;
	CRightFunctionButton	m_SubFunction5Button;
	CRightFunctionButton	m_SubFunction6Button;
	CRightFunctionButton	m_SubFunction7Button;
	CRightFunctionButton	m_SubFunction8Button;

	CRightFunctionButton	m_SubFunction6ButtonT2;
	CRightFunctionButton	m_SubFunction6ButtonT1;

	CDownFunctionButton	m_DemoHelpButton;
	CStatic	m_TotalEjectsLabelStaticTextControl;
	CStatic	m_StopDurationLabel;
	CStatic	m_RunDurationLabel;
	CStatic	m_ProductionStopsLabel;
	CStatic	m_TotalUnConfirmLabel;
	CStatic	m_TotalUnConfirmed;
	CStatic	m_MainBackground2;
	CStatic	m_MainBackground;
	CStatic	m_CurrentProductLabel;
	CStatic	m_uControllerStatus;
	CStatic	m_InspxLogo;
	CStatic	m_Clock;
	CButton	m_SimpleDividingLine;
	CStatic	m_RunDurationStaticTextControl;
	CStatic	m_BeltRateLabel;
	CStatic	m_BeltRate;
	CStatic	m_TotalContainersLabelControl;
	CStatic	m_TotalContainersControl;
	CStatic	m_TotalConfirmedLabelControl;
	CStatic	m_TotalConfirmedControl;
	CStatic	m_ShiftLabelControl;
	CStatic	m_ShiftControl;
	CStatic	m_PercentRejectedLabelControl;
	CStatic	m_PercentRejectedControl;
	CStatic	m_ContainersPerMinuteControl;
	CStatic	m_DamagedCountStaticTextControl;
	CStatic	m_DownCountStaticTextControl;
	CStatic	m_HighCapCountStaticTextControl;
	CStatic	m_MissingCapCountStaticTextControl;
	CStatic	m_MissingLabelCountStaticTextControl;
	CStatic	m_MissingSealCountStaticTextControl;
	CStatic	m_UnderFillCountStaticTextControl;
	CStatic	m_StopDurationStaticTextControl;
	CStatic	m_ProductionStopsCountStaticTextControl;
	CStatic	m_OverFillCountStaticTextControl;
	CStatic	m_ReceivedSerialDataStaticText;
	CStatic	m_DataReceivedLabelStaticTextControl;
	CStatic	m_ProductLabelControl;
	CStatic	m_TotalGoodStaticText;
	CStatic	m_TotalEjectsStaticText;
	CStatic	m_BackGroundStaticText;
	CStatic	m_DialogTitleStaticText;
	CStatic	m_TotalGoodLabelStaticTextControl;
	CString m_ProductLabelNameValue;
	CString	m_ClockText;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScanTracDlg)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CScanTracDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnFunction2Button();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnFunction3Button();
	afx_msg void OnFunction1Button();
	afx_msg void OnFunction4Button();
	afx_msg void OnFunction5Button();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSubFunction8Button();
	afx_msg void OnSubFunction2Button();
	afx_msg void OnSubFunction7Button();
	afx_msg void OnSubFunction6Button();
	afx_msg void OnSubFunction5Button();
	afx_msg void OnSubFunction1Button();
	afx_msg void OnSubFunction4Button();
	afx_msg void OnSubFunction3Button();
	afx_msg void OnDialogTitleStaticText();
	afx_msg void OnDemoHelpButton();
	afx_msg void OnClose();
	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CStatic m_SupportContract;
	CStatic m_SupportContract2;
	afx_msg void OnSupportcontract2();
	afx_msg void OnSupportcontract();
	//afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
	afx_msg void OnDialogtitlestatictext();
	afx_msg void OnDialogtitlestatictext1();
	CStatic m_DialogTitleStaticText1;
	private:
	void SetupMenu();
public:
	afx_msg void OnStnClickedTotalcontainerslabel();
};

//Global functions
BOOL CALLBACK UpdateAllOpenMenuTitleBarColors(HWND hwnd, LPARAM lParam);	//http://www.cplusplus.com/forum/windows/25280/ enumerate open windows
BOOL CALLBACK LogoutToMainMenuAfterBadgeOut(HWND hwnd, LPARAM lParam);
CString ConvertToDegreesCString(WORD TempReading, double TempAdjust);
CString uCConvertToDegreesCString(WORD TempReading, double TempAdjust);
int ConvertToDegreesC(WORD TempReading, double TempAdjust);
int uCConvertToDegreesC(WORD TempReading, double TempAdjust);
//CString ConvertToPressurePSIString(WORD TempReading, double TempAdjust);
//int ConvertToPressurePSI(WORD TempReading, double TempAdjust);
void Wait100MicroSecondsForCommandSend();
CString ByteToHex(BYTE TempData);
bool GetNetworkCardName(void);
CString WordToHex(WORD TempData);
WORD HexToWord(CString TempString);
BYTE HexToByte(CString TempString);
CString Pad(CString TempString, BYTE TempLengthLeft, BYTE TempLengthRight);
CString IToAWithComma(DWORD TempData);
CString dtoa(double TempData);
BYTE FindMedianValue(BYTE *TempLineData, WORD TempHeight);
void QuickSort(BYTE *arr, WORD low, WORD high);
double ATOF(CString TempString);
CString dtoaWithCommas(double TempData, BYTE TempClipLength);
CString GetCoreTemperatureString(WORD TemperatureIn);
CString dtoa(double TempData, BYTE TempClipLength);
CString dtoa(double TempData, BYTE TempClipLength, BYTE TempNumberOfSignificantDigits);
DWORD CalculateDirectorySize(CString TempString);
//#ifdef UseHalconLibraries
//HByteImage MakeHalconImage(Image *TempInputImage);
//void CopyFromHalconImage(HByteImage TempHalconImage, Image *TempOutputImage);
//void CopyFromHalconRegion(HRegion TempHalconRegion, Image *TempOutputImage);
//void ConvertHalconRegionToImage();
//void CopyFromHalconTuple(HTuple TempHalconTuple, Image *TempOutputImage);
//void ScanTracHalconExceptionHandler(const Halcon::HException& except);
//#endif
void RotateImage90Degrees(Image *TempInputImage, int TempSteps);
void DoMedianSmooth(Image *TempInputImage, BYTE TempSmoothAmountX, BYTE TempSmoothAmountY);
void SubtractImage(Image *TempImageIn, Image *TempImageOut);
//void DilateImage(Image *TempInputImage);
//void DilateImage(Image *TempInputImage, Image *TempScratchImage);
//void ErodeImage(Image *TempInputImage);
//void ErodeImage(Image *TempInputImage, Image *TempScratchImage);
void ThresholdImage(Image *TempImageIn, BYTE TempThreshold);
void ThresholdImageDown(Image *TempImageIn, BYTE TempThreshold);
BYTE MaximumValueInImage(Image *TempImageIn);
BYTE MinimumValueInImage(Image *TempImageIn);

void SystemShutDown(SystemShutDownMode mode);
WORD BPAdd(WORD TempWord1, WORD TempWord2);
WORD BPSubtract(WORD TempWord1, WORD TempWord2);
void ReportErrorMessage(CString TempErrorMessage, BYTE TempLevel, WORD TempCode);
bool CheckSystemFullyConfigured(CSystemConfigurationData *TempConfigurationData);
bool KeepSetupChanges();
bool AbortSetupChanges();
bool ThereIsEnoughMemory(DWORD TempMemorySizeNeeded, CString TempReasonNeeded);
bool PasswordOK(int TempPasswordToTest, bool TempAskForPasswordIfNoPassword);
bool DisposablePasswordOK(CString TempPasswordString, BYTE TempPasswordType);
CString GetPasswordCodeFromDate(CTime TempDate, BYTE TempPasswordType);
void SetNumLock(BOOL bState);
double GetMaximumCurrentFromVoltage(double TempVoltage); 
double GetMaximumCurrentFromPower(double TempVoltage, double TempMaximumPower); 
double CheckDiskSpaceFree(CString TempDirectory);
CString FormatRunTimeCounterString(unsigned long int TempCounter);
void WriteStringToFile(CString TempFileName, CString TempStringToWrite); 
double CalculateStandardDeviation(double *TempDataArray, DWORD TempNumberOfElements, double *TempAverage);
double CalculateStandardDeviation(BYTE *TempDataArray, DWORD TempNumberOfElements, double *TempAverage);
void SetupMenu(tCWndCollection&);
void SetupItem(CWnd* TempCWnd);
DWORD ResizeXCoor(int TempIntToResize);
DWORD ResizeYCoor(int TempIntToResize);
int GetLongestWordFromString(CString TempInString, CString *TempOutString, int TempLength);
int GetNumberOfWordsInString(CString TempInString, int TempLength);
void FillToTab(CString *TempString, WORD TempTabPlace);
int GetTextSizeForASingleLineDisplay(HWND TempDialogHwnd, CWnd* pWnd, CDC* pDC, CWnd* m_TempControlVariable, int TempMaxTextSize);
int SetTextSizeSingleLineDisplay(HWND TempDialogHwnd, CWnd* pWnd, CDC* pDC, CWnd* m_TempControlVariable, int TempMaxTextSize);	//one line
int SetTextSizeSubFunctionButton(HWND TempDialogHwnd, CWnd* pWnd, CDC* pDC, CWnd* m_TempControlVariable, int TempMaxTextSize, BYTE TempArrowsAreSet);	//three lines
int SetTextSizeMultilineDisplay(HWND TempDialogHwnd, CWnd* pWnd, CDC* pDC,  CWnd* m_TempControlVariable, int TempMaxTextSize);
int SetTextSizeSubFunctionButton(HWND TempDialogHwnd, CWnd* pWnd, CDC* pDC, CWnd* m_TempControlVariable, int TempMaxTextSize);
int SetTextSizeFunctionButton(HWND TempDialogHwnd, CWnd* pWnd, CDC* pDC, CWnd* m_TempControlVariable, int TempMaxTextSize);	//two lines
int* NumberOfWords(CString TempString, int TempLength);
BOOL MByteToUnicode(LPCSTR multiByteStr, LPWSTR unicodeStr, DWORD size);

UserPasswords* PasswordExist(tUserCollection &TempUserCollection, CString PasswordToFind);
POSITION InsertAlphabetically(tUserCollection &TempUserCollection, UserPasswords* UserToInsert);
/*
void CALLBACK EXPORT SerialPortTimerHandler(
	 HWND hWnd,      // handle of CWnd that called SetTimer
	 UINT nMsg,      // WM_TIMER
	 UINT nIDEvent,   // timer identification
	 DWORD dwTime    // system time
);
*/

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCANTRACDLG_H__1C80CF1B_CEB9_11D3_ABCC_00500466E305__INCLUDED_)
