//ScanTrac Side View Source File
// SystemConfigurationData.h: interface for the CSystemConfigurationData class.
//
//////////////////////////////////////////////////////////////////////
//#define ScanTracDemo

#ifndef ScanTracDemo
	#define UseIFCDLLs
	#define UsePCDigBoard
	// new IFC driver 5.9
	#define IFC59Driver
	#define UseSerialPort
#endif

//#define _WIN32_WINNT 0x0501 // NT is 500
//#define _WIN32_WINNT NTDDI_WINXPSP2; // 0x0500
//#define _WIN32_WINNT 0x0501		//declared in NewDiagnosticDialog

//#define UseTestModeCode
#define UseNewTestModeCode


// monitor temperature sensors
#define cTemperatureSensorWarningsNoneSent 0
#define cTemperatureSensorWarningRedBoxSent 1
#define cTemperatureSensorWarningCriticalSent 2

//#include "mvtools.h"
#include "ITIPCDig.h"
#include "BodyTrigger.h"
#include "Product.h"
#include "ExternalDetector.h"
#include "Ejector.h"
#include "Container.h"
#include "Win32SerialPort.h"
#include "LogFile.h"
#include <afxmt.h>  //for syncronization objects like critical sections

#if !defined(AFX_SYSTEMCONFIGURATIONDATA_H__61E500A4_EEC9_11D3_ABD6_00500466E305__INCLUDED_)
#define AFX_SYSTEMCONFIGURATIONDATA_H__61E500A4_EEC9_11D3_ABD6_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class UserPasswords;
typedef CTypedPtrList<CObList, CContainer*> tContainerCollection;
typedef CTypedPtrList<CObList, UserPasswords*> tUserCollection;

enum {cDefectsInNoColor, cDefectsInPseudoColor, cDefectsInRed, cDefectInRedBox, cDefectsInRedBoxes};

enum {cStoppedSystemMode, cDarkCurrentMode, cConditionXRayTubeMode, cCalibratingDetectorOffsetMode, cRunningSystemMode, cCalibratingDetectorEdgesMode, cAutoSetupRunningSystem, cMeasureSourceStrengthMode,
	cCalibratingDetectorGainMode, cTestShutterMode, cAlignTubeMode, cCalibrateEncoderMode};

enum {cMainDisplayBooting, cMainDisplayImage};

enum {cContaminantInspection, cVoidInspection, cDensityInspection, 
	cKernelInspection, cSizeInspection, cStandardDeviationInspection, 
	cJarBottomContaminantInspection,cIPContaminantInspection,cMissingLidInspection,
	cUnderfillInspection,cThresholdContaminantInspection,cTopContaminantInspection,
	cOverfillInspection,cBottomContaminantInspection,cContaminant2Inspection,
	cAdvancedContaminant2Inspection,cCheckWeighInspection, cGlass13mmContaminantInspection,
	cSpotInROIContaminantInspection, cContaminantBonesInspection, cContaminant22Inspection, cUnderfillByWeightInspection, cOverfillByWeightInspection, cMissingLidInspectionOnSize, 
	cWireContaminantInspection, cDripInspection, cFloodedInspection, cDripBySpoutInspection, cBadHeartInspection, cItemCountInspection};

enum {cButton2Hidden,cButton2Events,cButton2Rejects,cButton2Yellow};

enum {cMyNewImageMessage,cMyEmergencyShutDownMessage,cMyStartChatWindowMessage,
	cMyStartHelpWindowMessage,cMyHiddenKeyPressedMessage,cMyStartPasswordTimerMessage,
	cMySimulateOneContainerIn10MSMessage,cMyKillCalibrationTimerMessage,cMyShowSlippedMessage,
	cMyTurnDiverterOffMessage,cMyDisplayRejectHiddenMessage,cMyCheckLearningMessage,
	cMyOffsetCalibrationNextStepMessage,cMyOffsetCalibrationDoneMessage,
	cMyNewImageReadyToDisplay,cMyCheckHVPSVoltageSoonMessage,cMyInitializeDXMMessage,
	cMySerialReceivedDataMessage};

enum {cMyDataReceivedMessage,cMyDataSentMessage,cMyXRaysOnMessage,cMyUpdateTitleBarMessage,
	cMyEmergencyShutXRaysDownMessage,cMyNewImageReadyToDisplayOnSubWindow,
	cMySentCommTestMessage};

enum {cEnglish, cEnglishUSA, cFrench, cSpanish, cPolish, cGerman, cSimplifiedChinese};

enum {cXLFHVPS, cDXMHVPS, cMNXHVPS, c210WMonoBlockHVPS, c100WMonoBlockHVPS, cCheyneyMonoBlock, c100WVJTMonoBlockHVPS};

//     0=Sideview,   1=50 watt sideview, 2=50 watt pipeline, 3=600 watt pipeline, 4=12 inch TD,    24 inch TD,      36 inch TD           ,  web or band  ,up side down solo for hospira, 18" TD 100 W
enum {cSoloScanTrac, cPiccoloScanTrac,   cLegatoScanTrac,    cFermataScanTrac,    cAtempoScanTrac, cForteScanTrac, cCaseInspectorScanTrac, cBriosoScanTrac, cUpSideDownSoloScanTrac, cAllegroScanTrac, cSoloPlusScanTrac};


enum {cNoFrameGrabber, cPCDigFrameGrabber, cPC2CamLinkFrameGrabber};

enum {cQuickLearnOnly, cEvaluateAndQuickLearn, cEvaluateOnly};

enum {cExpired, cContractLastMonth, cNone, cContractGood};
enum {cSupportContractInvalid, cSupportContract8_5, cSupportContract16_5, cSupportContract24_7, cHideSupportContractDisplay, cSupportContractExpired};
enum {cManualProductionReport, cSelectProductProductionReport, cShiftChangeProductionReport, cResetCountersProductionReport, cTimeSetupProductionReport, cAutoSetupProductionReport,
	cStopInspectingProductionReport, cTimeUpdateProductoinReport, cChangedInspectionsWhileRunning};

enum {cRandomSamplingType, cFillerSamplingType, cSeamerSamplingType};
enum {cArrowLeft = 1, cArrowRight, cArrowDown, cPointyArrowSelectItemRight, cPointyArrowSelectItemLeft};
enum {cLeftArrowBig = 1, cLeftArrowSmall, cRightArrowBig, cRightArrowSmall, cUpArrowBig, cUpArrowSmall, cDownArrowBig, cDownArrowSmall};

enum {cExternalDetectorDisabledMode, cRejectInWindowMode, cAcceptInWindowMode, cRejectAtStartMode, cAcceptAtStartMode, cBackupDetectorMode,
	cValveMonitorIndexPulseInput, cSeamerIndexPulseInput, cSeamerObjectPulseInput, cSeamerPocketPulseInput, cSampleEjectNextContainerMode};

const BYTE cNumberOfExternalDetectorModes = 11;

const BYTE cCurrentArchiveVersionNumber = 158;

const BYTE cShowSensitivityDisplays = 1;
const BYTE cHideSensitivityDisplays = 2;

const BYTE cSingleImageScanTrac = 0;
const BYTE cMultiImageScanTrac = 0xAA; //not used in single image software
const BYTE cCrescendoScanTrac = 0xFF; //not used in single image sofware
const BYTE cTopDownScanTrac = 0x55; //for Top Downs with IO Board, no longer made since 2008

const BYTE cImageBrightnessThreshold = 250;

const double cSoloRightToLeftDistance = 16.5; //was 12.25;
const double cSoloLeftToRightDistance = 23;
const double cPiccoloDistance = 13;
const double cSoloEjectorOffset = 37.5;
const double cPiccoloEjectorOffset = 25;
const double cForteRightToLeftEjectorOffset = 24;  //was 21.5 beam to wall, plus 4 for body trigger
const double cForteLeftToRightEjectorOffset = 24;  //was 14.5 beam to wall, plus 4 for body trigger
const double cForteTriggerToBeamOffset = 12;  
const double cAllegroTriggerToBeamOffset = 24;  
const double cAllegroEjectorOffset = 36;  //beam to wall = 13, plus 23 for body trigger

const BYTE cNumberOfTimesToCalibrateGain = 5;
const BYTE cMaximumNumberOfErrorMessages = 100;

const BYTE cNumberOfEncoderSamples = 120;

const BYTE cNumberOfImagesToEvaluate = 10;
//vEjectReason values
//1 to 15 eject for inspection
//16  to 31 following constant values
//32 to 34 Auxiliary Detector ejects
//35 test eject

const BYTE cEjectSlippedContainer = 16;
const BYTE cPassSlippedContainer = 17;
const BYTE cEjectContainerWrongWidth = 18;
const BYTE cPassContainerWrongWidth = 19;
const BYTE cEjectContainerOnTop = 20;
const BYTE cPassContainerOnTop = 21;
const BYTE cEjectContainerWrongHeight = 22;
const BYTE cPassContainerWrongHeight = 23;
const BYTE cEjectContainerOnConveyorStop = 24;
const BYTE cPassContainerOnConveyorStop = 25;
const BYTE cEjectEmptySlippedContainer = 26;
const BYTE cEjectAuxilaryDetectorOffset = 32;
const BYTE cTestEject = 35;
const BYTE cEjectInspectionOffset = 1;
const BYTE cMaximumRejectReason = 36;

const BYTE cAlarmEjectorReEnableIndex = 2;

const BYTE cMaximumIntegrationTime = 10; //milliseconds
const BYTE cMinimumXRayVoltage = 10; //10KV Minimum 4/26/2006

const BYTE cMaximumOutstandingEjects = 80;//48;
const BYTE cMaximumOutstandingAuxiliaryDetectorEnables = 80;//48;
const BYTE cDefaultThreshold = 128;
const LPCTSTR cImageDirectory = _T("C:\\ScanTracImages\\");
const LPCTSTR cSetupDataDirectory = _T("C:\\ScanTracSetupData\\");
const LPCTSTR cScanTracDirectory = _T("C:\\ScanTrac\\");
const LPCTSTR cOldScanTracSetupBackupDirectory = _T("C:\\ScanTracSetupBackup\\");
const LPCTSTR cScanTracSetupBackupDirectory = _T("C:\\ScanTrac Automatic Backups\\ScanTracSetupBackup\\");
const LPCTSTR cScanTracScreenShotDirectory = _T("C:\\ScanTracScreenShots\\");
const LPCTSTR cACPowerMoniterLogDirectory = _T("C:\\ScanTracACPowerMoniter\\");
const LPCTSTR cRejectedImagesDirectoryDDrive = _T("D:\\ScanTracRejectedImages\\"); // "D:\\ScanTracRejectedImages\\";
const LPCTSTR cRejectedImagesDirectoryCDrive = _T("C:\\ScanTracRejectedImages\\"); // "C:\\ScanTracRejectedImages\\";
const LPCTSTR cScanTracEventLogDirectory = _T("c:\\ScanTracEventLogs\\");
const LPCTSTR cScanTracSerialLogsDirectory = _T("c:\\ScanTracSerialLogs\\");
const LPCTSTR cScanTracProductionReportDirectory = _T("c:\\ScanTracProductionReports\\");
const LPCTSTR cScanTracCalibrationImagesDirectory = _T("c:\\ScanTracCalibrationImages\\");
//const LPCTSTR cOperationsGuardianDefaultServerName = _T("https://og-demo.pecoinspx-internal.com");
const LPCTSTR cOperationsGuardianDefaultServerName = _T("https://og-qa.pecoinspx-internal.com");

const unsigned int cShadowRejectedImageBit = 0x0001;
const unsigned int cShadowScreenShotsBit  = 0x0002;
const unsigned int cShadowEventLogBit = 0x0004;
const unsigned int cShadowProductionReportBit = 0x0008;
const BYTE cMaximumNumberOfDemoImages = 200;
const WORD cMaximumQuickLearnImages = 100;

const BYTE cMaximumNumberOfImagesForDriftDensity = 100;
const WORD cCalibrateEdgesTimeInterval = 3000;  //time between detector edge calibration images
const BYTE cNumberOfOffsetBits = 9; //was 11, now 9.  11 is 50% DAC range, 10 is 25% DAC Range, 9 is 12.5% DAC Range.
const BYTE cCalibrateOffsetSleepAfterCommandTime = 10; //worked good with value of 10
const BYTE cCalibrateOffsetTimeInterval = 10;
const WORD cStandardDeviationSampleSize = 15000; 
const BYTE cNumberOfCalibrationImages = 20;
const BYTE cNumberOfCalibrationOffsetImages = 100;

const BYTE cBackupDetectorStopFactor = 2;
const BYTE cBackupDetectorStartFactor = 3;
const BYTE cRadiationShutterAutoControl = 0;
const BYTE cRadiationShutterOpen = 3;
const BYTE cRadiationShutterClose = 1;
const WORD cImageProcessingTimeMaximum = 1000;
const double cMaximumConveyorSpeedDeratingWithSimulatedEncoder = 97.0;
const double cMaximumConveyorSpeedDerating = 111.0;

const WORD cMaximumValveDataRows = 1000;


//constants for messages
const WORD cShowHelp = 42122;
const WORD cNewMainImageReadyToDisplay = 42125;
const WORD cXRaysOnMessage = 42126;
const WORD cDisableEjectorsMessage = 42127;
const WORD cStartPasswordTimerMessage = 42128;
const WORD cTurnDiverterOnMessage = 42129;
const WORD cCheckLearningMessage = 42130;
const WORD cStartChatWindowMessage = 42131;
const WORD cTurnDiverterOffMessage = 42132;
const WORD cOffsetCalibrationDoneMessage = 42133;
const WORD cEnableEjectorsMessage = 42134;
const WORD cShowSlippedMessage = 42135;
const WORD cOffsetCalibrationNextStep = 42136;
const WORD cHiddenKeyPressed = 42137;
const WORD cUpdateTitleBarMessage = 42139;
const WORD cDisplayRejectHiddenMessage = 42140;
const WORD cQuickLearnProcessingFinishedMessage = 42141;
const WORD cCheckACBeforeYellowMessage = 42142;
const WORD cKillCheckACBeforeYellowMessage = 42143;
const WORD cDensityLoopLockedMessage = 42144;
const WORD cDensityLoopLockedClearMessage = 42145;

const WORD cEvaluateImagesProcessingFinishedMessage = 42146;
const WORD cNewImageReadyToDisplay = 42147;
const WORD cIndexPulseErrorMessage = 42148;
const WORD cSoftKeyPressed = 42149;
const WORD cDisplaySoftKeyMenu = 42150;

const WORD cNumberOfEdgePixelsToAdjust = 3;  //in config file, don't change
const WORD cNumberOfPixelsToAdjust48 = 2 * cMaximumNumberOfDetectors8 * cNumberOfEdgePixelsToAdjust; //in config file, don't change
const WORD cNumberOfPixelsToAdjust72 = 2 * cMaximumNumberOfDetectors12 * cNumberOfEdgePixelsToAdjust; //in config file, don't change
const WORD cNumberOfPixelsToAdjustAllPixels768 = 768;//in config file, don't change
const WORD cNumberOfPixelsToAdjustAllPixels1152 = 1152;//in config file, don't change //for Allegro in .4 mm mode need 128 pixels per detector times 9 detectors = 1152 pixels
const BYTE cMaximumImageWidth = 48; //inches
const BYTE cCalibrationImageWidth = cMaximumImageWidth;

//defines locations of color background on screen displays
const BYTE cMainBackgroundLeft = 106;
const BYTE cMainBackgroundTop = 41;
const WORD cMainBackgroundLength = 423;
const WORD cMainBackgroundHeight = 367;

const BYTE cMaxCPMTimePeriod = 120;

const WORD cMaxBeltPositionOffset = 65000;

const BYTE cInterlockInterruptEnable = 0x7F;  //was 0x48

const WORD cSplashScreenDelay = 4000;
const BYTE cGoodCountDelay = 20;
const BYTE cBeltPositionDelay = 1;
const WORD cEjectCountDelay = 2000;
const BYTE cNumberOfDiagnosticLines = 6;
const WORD cUseDownStream = 234;
const BYTE cDontUseDownStream = 235;

const WORD cMaximumEjectDataSize = 200;

const WORD cuCNormalVoltageValue = 3276;
const WORD cuCLowVoltageValue = 2730;
const WORD cuCHighVoltageValue = 3700; //was 3452;

const WORD cNumberOfAirConditionerTemperatureSamples = 720; //take a temp sample every 10 seconds for 2 hours gives 720 samples

const BYTE cPunyFont = 65;
const BYTE cTinyFont = 75;
const BYTE cSmallFont = 85;
const BYTE cMediumFont = 100;
const BYTE cNormalFont = 115;
const BYTE cMediumLargeFont = 145;
const BYTE cLargeFont = 165;
const BYTE cExtraLargeFont = 200;
const BYTE cFixedFontSize = 60;


const int cHugeFont = 250;
const int cExtraHugeFont = 300;
const LPCTSTR cThinFont = _T("Arial");
const LPCTSTR cBoldFont = _T("Impact");
const LPCTSTR cThickFont = _T("Arial Black");
const LPCTSTR cFixedFont = _T("Courier New"); //Lucida Console,Consolas
const LPCTSTR cFixedFontXP = _T("Consolas"); //Lucida Console,Consolas

const COLORREF cButtonTextColor = 0x00A00000; //cDarkBlue
const COLORREF cButtonColor = 0x00FFFF00; //cBrilliantBlue

const COLORREF cBackgroundColor = 0x00C6C3C6;
const COLORREF cLightRed = 0x00B3B3FF;
const COLORREF cRed = 0x000000FF;
const COLORREF cDarkRed = 0x000000C0;
const COLORREF cLightPink = 0x00FCB0FC;
const COLORREF cPink = 0x00D070FF;
const COLORREF cDarkPink = 0x00C000E0;
const COLORREF cLightYellow = 0x00C4FDFA;
const COLORREF cYellow = 0x0000FFFF;
const COLORREF cDarkYellow = 0x0000C0C0;
const COLORREF cLightGreen = 0x00C0FFC0;
const COLORREF cGreen = 0x0000F000;
const COLORREF cMediumDarkGreen = 0x00009800;
const COLORREF cDarkGreen = 0x00008000;

const COLORREF cVeryLightBlue = 0x00FFE0E0;
const COLORREF cLightBrilliantBlue = 0x00FFFFD0;
const COLORREF cBrilliantBlue = 0x00FFFF00;

const COLORREF cLightBlue = 0x00EFA3A3;
const COLORREF cBlue = 0x00FF0000;
const COLORREF cDarkBlue = 0x00A00000;

const COLORREF cBlack = 0x00000000;
const COLORREF cWhite = 0x00FFFFFF;

const COLORREF cVeryLightGray = 0x00E0E0E0;
const COLORREF cLightGray = 0x00C0C0C0;
const COLORREF cGray = 0x00A0A0A0;
const COLORREF cShadowGray = 0x00404040;
const COLORREF cDarkGray = 0x00808080;

const COLORREF cOlive = 0x00008080;
const COLORREF cLightPurple = 0x00FEC2E2;
const COLORREF cPurple = 0x00803090;
const COLORREF cDarkPurple = 0x00A01080;
const COLORREF cBrown = 0x00604080;
const COLORREF cLightBrown = 0x00C4FDFA;
const COLORREF cOrange = 0x000080FF;
const COLORREF cNumberOfADCChannels = 4;  //in config file, don't change
const COLORREF cSupervisorTextColor = cBlack;

const WORD cDiagnosticLineArraySize = 400;
const BYTE cNumberOfVoltageSamples = 16;
const WORD cMaximumSourceStrengthCount = 1000;

const BYTE cMaximumEMailRecievers = 25;
const BYTE cNumberOfEMailGroups = 10;
const BYTE cNumberOfRadiationDoorFlipFlops = 120;
const BYTE cNumberOfRadiationInDoorFlipFlops = 40;
	//write rejects to Hard Disk
const BYTE cNumberOfRejectImageBuffers = 200;

const WORD cOldResolutionX = 640;
const WORD cOldResolutionY = 480;
const BYTE cPoint8mmDetectors = 0;
const BYTE cPoint4mmDetectors = 1;
const BYTE cDontReferenceSideToSide = 1; 
const BYTE cReferenceSideToSide = 2;


//master extern place for entire program
extern double vGlobalPixelsPerUnit;
extern double vGlobalPixelsPerUnitInHeight;

#ifndef UseIFCDLLs
/*
	class Image : public CObject  
	{
		//variables
		BYTE vHolder;

		//Contructor Destructor
		vImage();
		virtual ~Image();
		
		//methods
		void DoWork();
		//	DECLARE_DYNCREATE(Image)
	};
	*/
#endif

	//write rejects to Hard Disk
class CRejectImageBuffer : public CObject
{
public:
	bool vLogThisReject;
	bool vEjectorsDisabled;
	int vContainerEjectInspection;
	CTime vTime;
	BYTE *vGreyImage;
		
	WORD vRedBoxCornerX;
	WORD vRedBoxCornerY;
	WORD vRedBoxWidth;
	WORD vRedBoxHeight;
};

class CLifeTimeCounters : public CObject  
{

	DECLARE_SERIAL( CLifeTimeCounters )

public:
	int vArchiveVersionNumber;
	unsigned long int vXRaySourceRunDuration; //keeps track of how many seconds x-ray source
	unsigned long int vDetectorsRunDuration; //keeps track of how many seconds Detectors
	unsigned long int vScanTracRunDuration; //keeps track of how many seconds ScanTrac
						//has been on for its whole life
	unsigned long int vSpareULongIntLifeCounters1; 
	unsigned long int vSpareULongIntLifeCounters2; 
	unsigned long int vSpareULongIntLifeCounters3; 
	unsigned long int vSpareULongIntLifeCounters4; 
	unsigned long int vSpareULongIntLifeCounters5; 
	unsigned long int vSpareULongIntLifeCounters6; 

	unsigned long int vXRaySourceWarrantyDuration; //keeps track of how many seconds x-ray source
	unsigned long int vDetectorsWarrantyDuration; //keeps track of how many seconds Detectors
	CTime vXRaySourceWarrantyDate;
	CTime vDetectorsWarrantyDate;
	CTime vCommisionDate;
	CString vUsedDisposablePasswords; 
	CString vServiceRecordLog; 
	CString vSpareCStringLifeCounters1; 
	CString vSpareCStringLifeCounters2; 
	CString vSpareCStringLifeCounters3; 
	CString vSpareCStringLifeCounters4; 
	CString vSpareCStringLifeCounters5; 
	CString vSpareCStringLifeCounters6; 
	CString vSpareCStringLifeCounters7; 
	CString vSpareCStringLifeCounters8; 
	CString vSpareCStringLifeCounters9; 
	CString vXRayTubeSerialNumber; 

	//Contructor
	CLifeTimeCounters();
	virtual ~CLifeTimeCounters();
	
	//methods
	virtual void Serialize(CArchive& TempArchive);

	//DECLARE_DYNCREATE(CLifeTimeCounters)
};


class CSystemConfigurationData : public CObject  
{

	DECLARE_SERIAL( CSystemConfigurationData )

public:
	CTime vTimeXRaysWereOnLast;
	WORD vAdjustOffset[cNumberOfPixelsToAdjust72];

	CString vScanTracProductionReportDirectory;
	CString vScanTracScreenShotDirectory;
	CString vScanTracImagesDirectory;
	CString vScanTracEventLogDirectory;
	CString vScanTracCalibrationImagesDirectory;
	CString vScanTracSerialLogsDirectory;
	CString vScanTracRejectedImagesDirectory;

	double vQuickLearnNumberOfStructureDeltas;
	WORD vNumberOfQuickLearnImagesToTake;
	BYTE vAdjustAllAreaOfHistogram;

	WORD vReCalibrateAfterStopTimePeriod;
	BYTE vRadiationShutterInSystem;

	BYTE vStopRunningOnUnConfirmedEject;
	BYTE vSurfaceTemperatureMovedToDXM;
	WORD vErrorMessageDisplayMask;
	WORD vInspxErrorMessageDisplayMask;

	BYTE vRadiationDoorInvertDoorOutput;
	WORD vRadiationDoorInDoorDistance;
	WORD vRadiationDoorDistanceStayOpen;
	WORD vAlarmEjectorReEnableX;
	WORD vAlarmEjectorReEnableY;

	BYTE vStopInspectionIfConveyorStops;
	BYTE vEjectContainersNotInspected;
	BYTE vDiverterEjector;
	BYTE vDiverterMargin;

	//shape handling variables
	BYTE vShapeHandlingProcessIfAtSides; // 0 don't, 1 reject, 2 accept
	BYTE vSlipsEjectEmptyContainers;
	//BYTE vShapeHandlingProcessIfAtTop; // 0 don't, 1 reject, 2 accept  not used anymore
	BYTE vUseXScanDetectors;
	BYTE vShapeHandlingProcessIfWrongWidth;// 0 don't, 1 reject, 2 accept 
	BYTE vShapeHandlingProcessIfWrongHeight;// 0 don't, 1 reject, 2 accept 
	BYTE vShapeHandlingProcessIfConveyorStops;// 0 don't, 1 reject, 2 accept 

	BYTE vAllowClientConnections; 
	BYTE vEnableModBusTCPIPInterface;
	double vShapeHandlingHeightTolerance;
	double vShapeHandlingWidthTolerance;
	WORD vShapeHandlingHeightTolerancePixels;
	WORD vShapeHandlingWidthTolerancePixels;
	DWORD vShapeHandlingEjectors;
	BYTE vShapeHandlingEjectorsVariableToReadInOldByte;

	//drift compensation constants
	WORD vDriftCompensationWaitTime;
	WORD vNumberOfImagesForDriftDensity;
	double vDriftCompensationFastThreshold;
	double vDriftCompensationVeryFastThreshold;
	double vDriftCompensationFastAdjust;
	double vDriftCompensationVeryFastAdjust;
	double vDriftCompensationSlowThreshold;
	double vDriftCompensationSlowAdjust;
	double vDriftCompensationMaximumAdjust;
	BYTE vDriftCompensationEnabled;
	BYTE vAdjustImageIntensity;
	BYTE vMaxImageIntensityAdjust;
	BYTE vMaxImageIntensityAdjustBeforeLocked;  
	double vDriftCompensationThresholdReductionFactor;
	BYTE vDriftCompensationThresholdReductionMaximum;

	//variables
	unsigned long int vXRaySourceRunDuration; //keeps track of how many seconds x-ray source
	unsigned long int vDetectorsRunDuration; //keeps track of how many seconds Detectors
	unsigned long int vScanTracRunDuration; //keeps track of how many seconds ScanTrac
						//has been on for its whole life

	BYTE vHasOutsideAirTemp;  //obsolete
	//to configure ITIPCDig object
	DWORD vAverageDisableCount;
	DWORD vStructureDisableCount;
	DWORD vDensityDisableCount;
	DWORD vCalibrateBodyTriggerReferenceDisableCount;

	BYTE vRemoveBackgroundFromImage;
	DWORD vSlippedIndicatorTime;
	BYTE vDeltaTemperatureForFilterCloggedError;
	BYTE vDeltaTemperatureForCoolantProblem;
	BYTE vScanTracConfigurationFileType;//0 = Side View, 0x55 = Top  Down, 0xAA = Dual Beam
	//end of ITIPCDig configuration items

	CString vScanTracSetupBackupDirectory;
	BYTE vLastBackupDirectoryUsed;
	BYTE vCommand1;//inspx diagnostics only, remembers last command so do not have to re-type in
	BYTE vCommand2;
	BYTE vCommand3;
	BYTE vCommand4;
	double vMeasuredDetectorGain[cMaximumNumberOfDetectors12];
	double vMeasuredDetectorOffset[cMaximumNumberOfDetectors12];
	double vVoltageAdjustOffset;
	double vCurrentAdjustOffset;
	double vVoltageAdjustGain;
	double vCurrentAdjustGain;
	double vSystemBodyTriggerToXRayBeam;
	double vSystemEjectorPositionOffset;
	double vSystemEjectorDistance[cNumberOfEjectors];
	double vMaximumCurrent;
	double vMaximumVoltage;
	double vMaximumPower;
	CString vScanTracID;
	CString vEMailUser[cMaximumEMailRecievers * cNumberOfEMailGroups];
	CString vEMailDomain[cMaximumEMailRecievers * cNumberOfEMailGroups];
	CString vEMailSubject[cNumberOfEMailGroups];
	BYTE vEMailEnable[cNumberOfEMailGroups];
/*
	cEMailPlantOperations = 0;
	cEMailInspx = 1;
	cEMailMaintenance = 2;
	cEMailReport = 3;
*/
	BYTE vNumberOfEMailRecievers[cNumberOfEMailGroups];

	BYTE vPreAmplifierGain;
	WORD vSourceStrengthCount;
	CTime vSourceStrengthDate[cMaximumSourceStrengthCount];
	double vSourceStrengthData[cMaximumSourceStrengthCount];
	CString vSourceStrengthComment[cMaximumSourceStrengthCount];
	double vTemperatureSurfaceAdjust;
	double vTemperatureLiquidAdjust;
	double vTemperatureuCAdjust;
	double vTemperatureAirInsideAdjust;
	double vTemperatureAirOutsideAdjust;  
	//double vCoolantPressureAdjust;  

	int vTemperatureSurfaceAdjustint;
	int vTemperatureLiquidAdjustint;
	int vTemperatureuCAdjustint;
	int vTemperatureAirAdjustint;
	int vMaximumSurfaceTemperature;
	int vMaximumLiquidTemperature;
	int vMaximumuCTemperature;
	int vMaximumAirTemperature;
	BYTE vMaximumTemperatureAirOutside;  
	//int vMaximumCoolantPressure;  
	//int vMinimumCoolantPressure;  
	
	BYTE vEnableOperationsGuardian;
	BYTE vDemoMode;
	BYTE vXRaySourceFilter;  
	CString vLastProduct;
	BYTE vLogSyncData;
	BYTE vLogSerialData;
	//BYTE vDefectDisplayMode; //not used anymore
	//BYTE vShowDefectsInRed;
	BYTE vNumberOfLastRejectImages;
	BYTE vShowOnlyRealImages;
	BYTE vShow24HourTime;
	WORD vContinuousFeedEncoderRate;
	WORD vContinuousFeedContainerRate;
	CString vResetCountsPassword;
	BYTE vReferenceROIsToEdges;
	BYTE vReferenceContainersSideToSide;
	DWORD vInspxMessageCount;

	BYTE vAskForProductionReportProductCode;
	BYTE vAutoXRaySourceOffEnabled;  //1=Enabled, 2=Disabled, all other values invalid
	BYTE vEnableEjectors;
	BYTE vRampXRayDuration;//obsolete but kept code in case need someday
	BYTE vCorrectDetectorEdges;
	BYTE vSimulateLightControlBoard;
	BYTE vShowMouseCursor;

	//const BYTE cPoint8mmDetectors = 0;
	//const BYTE cPoint8mmDetectors = 1;

	double vDetectorEdgeAdjustGain[cNumberOfPixelsToAdjustAllPixels1152][2]; //0 = .8 mm X-Scan OR either DT Detectors, 1 = .4 mm X-Scan only.
	double vDetectorEdgeAdjustOffset[cNumberOfPixelsToAdjustAllPixels1152][2];
	int vOverWriteBadPixel[cNumberOfPixelsToAdjustAllPixels1152][2];
	int vDetectorEdgeAdjustOffsetInteger[cNumberOfPixelsToAdjustAllPixels768]; //not used or in stream anymore, but still needed to read in old sream formats
	BYTE vSourceStrengthInitialValues[cMaximumNumberOfSourceStrengthValues21];
	BYTE vSaveNextSourceStrengthValuesAsInitial;

	WORD vEnableACMessages;
	int vArchiveVersionNumber;

	double vDetectorGain[cMaximumNumberOfDetectors12];
	double vDACDetectorOffset[cMaximumNumberOfDetectors12];
	WORD vADGain[cMaximumNumberOfDetectors12];
	int vADCDetectorOffset[cMaximumNumberOfDetectors12]; //not used anymore 3/26/03 left in case need test
	BYTE vADGainByte[cMaximumNumberOfDetectors12];//not used anymore 3/26/03
  WORD vEncoderDivider;  //really encoder divider
	WORD vEncoderDividerForPoint4mmMode;
	BYTE vPixelsPerDetector; //128 for .4mm detector 2" tall, 64 for .8 mm detectors 2" tall, 64 for 1.6mm detectors 4 " tall
	BYTE vDetectorLength;
	BYTE vNumberOfXRayDetectors;
	//BYTE vNumberOfXRayDetectorsToReadInImage;
	BYTE vDetectorClockDivider;
	double vPixelsPerUnit;
	double vPixelsPerUnitForPoint4mmMode;
	double vCalibratedPixelsPerUnit;
	CString vUnitsString;
	CString vProductSetupPassword;
	CString vQAPassword;
	CString vProductSimpleSetupPassword;
	CString vCertifiedSetupPassword;
	CString vInspxPassword;
	CString vSystemSetupResetPassword;
	CBodyTrigger vBodyTrigger[cNumberOfBodyTriggers];
	CExternalDetector vExternalDetector[cNumberOfExternalDetectors];
	CEjector vEjector[cNumberOfEjectors];
	BYTE vBackupDetectionUsingBodyTriggerTimeOut;  //msbit indicates if it should stop inspecting on a backup
	BYTE vDiagnosticLineSelect[cNumberOfDiagnosticLines];
	BYTE vImageShowLines;
	BYTE vImageShowFixedROILines;
	BYTE vImageShowScale;
	BYTE vSuppressWarningMessages;
	CString vScanTracCustomerName;
	BYTE vNumberOfDetectorsToCalibrate;
	BYTE vUseDACAndADCinDensityCompensation;
	BYTE vHVPSType;
	WORD vNumberOfPixelAtBottomToIgnoreInCalibration;
	BYTE vXRayTubeHasCooled;

	BYTE vLearnQualityPoorLimit;
	BYTE vAllowVoidInspection;
	BYTE vEncoderTolerance;
	BYTE vNumberOfEncoderRateReversalsForError;

	BYTE vAutoImproveEnabled;
	BYTE vAutoImproveReduceThresholdFactor; //alex does not want to use, so setting to zero and removing UI, but could restore if needed
	WORD vAutoImproveNumberToAddToLearn;
	BYTE vAutoDeleteFileTypes; 
	BYTE vBulkProductMode;  //in pipeline uses real encoder and FPGA simulates frame after correct # of encoder pulses

	BYTE vLearnQualityMarginalLimit;
	BYTE vAutoProductionReportHour;
	BYTE vAutoProductionReportHour2;
	BYTE vAutoProductionReportHour3;

	BYTE vRedIndicatorToShowOnMainMenu;
	//     0=Sideview,   1=50 watt sideview, 2=50 watt pipeline, 3=600 watt pipeline, 4=12 inch TD,    24 inch TD, 
	BYTE vScanTracType; 
	BYTE vUseRealEncoder;

	double vPipeLineFlowRate;
	BYTE vUsePipeFlowInput;

	BYTE vNeedToCalibrateEncoder;
	BYTE vWriteRejectsToFiles;
	BYTE vLanguage; //enum {cEnglish, cEnglishUSA, cFrench, cSpanish, cPolish, cGerman};

	BYTE vHaveCheckedForMoreRAM; 
	BYTE vMaximumMonoBlockTemperature;
	WORD vLastSimulatedEncoderRateSet;
	WORD vNumberOfDaysToKeepLogs;
	BYTE vUseDownStreamSignalThatStopsConveyor;

	BYTE vShowSensitivityDisplays;

	int vACPowerMonitorOffset;
	int vMaximumCPUTemperature;
	CString vAdvancedSetupPassword; 
	CString vIPAddressString; 
	CString vIPSubNetMaskString; 
	CString vIPDefaultGatewayString; 
	CString vOperatorPassword; 
	CString vIPDefaultDNSAddressString;
	BYTE vIPDHCPEnabled;

	BYTE vSupportContractType;
	BYTE vNumberOfCheckWeighOffsetSamples;
	BYTE vQuickLearnDilateTimes; //valid values 1 to 20, 20 is disabled
	BYTE vNormalLearnDilateTimes;
	BYTE vProductSlipPercentValue;
	BYTE vRFIDEASBadgeReaderCommPort; 

	//Valve Monitor Data
	BYTE vNumberOfHeadsToMonitor[3];
	BYTE vSpareWasHeadOffsetFromIndexPulse[4];//not used, use one in product  //0 -1/2 speed filler index or Random(unused) 1-full speed filler index offset 2 full speed seamer index offset 3-half speed seamer index
	BYTE vStaggerSampling;
	BYTE vShowIndexErrors;
	BYTE vShowSampleAllButton;
	double vDistanceFromSeamerObjectToScanTracTrigger;

	BYTE vDetectorsAtBottomToIgnoreDuringCalibration;
	BYTE vUseXofYAlarmOutputForUnconfirmedEject;
	BYTE vSaveAllImagesToRejectsFolder;
	CTime vTimeLastMeasuredSourceStrength;
	CString vSamplePassword;
	BYTE vUse101VACFor10SecondsShutDownLimit;
	BYTE vUseExternalInterlock;
	BYTE vAutoStop;
	BYTE vDetectedValidGuardMasterDevice;
	CString vOperationsGuardianServer;
	BYTE vResynchronizeEjectors;
	BYTE vApplyRetriggerLockoutToResynchronizingSensors;
	WORD vBeltPositionDistancetoEjectorResynchronizationSensor;

	//SPARE STREAM Variables
	WORD vCPMTimePeriod; //Can not be changed anymore, fixed at 30 CPM  //spare if need the stream

	int vNumberOfJitterPositions; //spare - obsolete
	int vNumberOfTopJitterPositions; //spare - obsolete
	int vSupportContractDayExpires;
	int vSupportContractMonthExpires;
	int vSupportContractYearExpires;
	int vSpareInt4;
	int vSpareInt5;

	CTime vSpareCTimeCD1;
	CTime vSpareCTimeCD2;
	CTime vSpareCTimeCD3;
	CTime vSpareCTimeCD4;
	CTime vSpareCTimeCD5;

	double vSpareDouble3;
	double vSpareDoubleCD4;
	double vSpareDoubleCD5;
	double vSpareDoubleCD6;

	BYTE vIgnoreCheyneyInterlockStatus;
	BYTE vStopAfterAnInspectionRejects;
	BYTE vSpareByteCD4;

	WORD vEncoderRateTolerance;
	WORD vSpareWord4;
	WORD vSpareWord5;
	WORD vSpareWordCD6;
	WORD vSpareWordCD7;
	WORD vSpareWordCD8;
	WORD vSpareWordCD9;

	DWORD vSpareDWORD2;
	DWORD vSpareDWORD3;
	DWORD vSpareDWORD4;
	DWORD vSpareDWORD5;
	DWORD vSpareDWORD6;

	CString vSpareString3;
	CString vSpareStringSC4;
	CString vSpareStringSC5;
	CString vSpareStringSC6;

	//Contructor
	CSystemConfigurationData();
	virtual ~CSystemConfigurationData();
	
	//methods
	virtual void Serialize( CArchive& TempArchive );
	int SelectEjectors(CString TempEjectorType, DWORD *TempEjectors, CSystemConfigurationData *TempConfigurationData, BYTE TempInAutoSetup);
	CString GetEjectorNames(DWORD TempEjector);
	bool LimitToMaximumVoltage(double &TempVoltage);
	bool LimitToMaximumCurrent(double TempVoltage, double &TempCurrent);
	void SetPixelsPerDetector(BYTE TempPixelsPerDetector);
	void SetDetectorLength(BYTE TempDetectorLength);

	//int WriteConfigurationDataToINIFile();
	//int ReadConfigurationDataFromINIFile();

	//	DECLARE_DYNCREATE(CSystemConfigurationData)
};


class CScanTracSystemRunningData : public CObject
{
public:
	bool vMultiLaneInSetupLanePositionsMenu;
	WORD vMultiLaneLastTriggerPosition;
	WORD vMultiLaneLastTriggerPositionForThisImage;
	DWORD vMultiLaneInspectionCounts[cMaximumNumberForMultiLane][cMaximumNumberOfInspections];
	DWORD vMultiLaneInspectionDisabledCounts[cMaximumNumberForMultiLane][cMaximumNumberOfInspections];
	DWORD vMultiLaneTotalCounts[cMaximumNumberForMultiLane];
	DWORD vMultiLaneTotalEjectsCounts[cMaximumNumberForMultiLane];
	DWORD vMultiLaneTotalDisabledEjectsCounts[cMaximumNumberForMultiLane];
	BYTE vResynchronizeEjectorsCurrentEjectorSensorCount[2];
	BYTE vResynchronizeEjectorsCurrentTriggerCount;
	bool vEjectorsNotSynchronized;
	bool vFirstContainerShouldNotTriggerSensorYet;
	WORD vBeltPositionTunnelShouldBeClearTo;
	bool vModBusSocketOpen;
	bool vModBusClientConnected;
	DWORD vModBusClientConnectionCounts;
	DWORD vModBusClientRequestCounts;
	CString vModBusTCPIPAddress;
	DWORD vModBusTCPIPErrorCounter;

	//OG connection status variable 11/2/2015
	bool vOperationsGuardianConnected;
	DWORD vOperationsGuardianError;
	BYTE vSampleEjectNextContainerBitMap;
	WORD vCurrentEncoderDivider;
	WORD vDiverterDeactivatePosition[cNumberOfEjectors + 1];
	bool vDiverterActive[cNumberOfEjectors + 1];
	BYTE vCalibratingXScanPoint4mmDetectors;

	//valve Monitoring Data
	BYTE vValveMonitorInspection;
	bool vRecievedFirstIndexPulse[3];
	BYTE vFillerIndexPulseBitMap;
	BYTE vSeamerPocketSensorBitMap;
	double vLastTimeObjectToPocketPulse;
	double vLastPocketTime;
	double vLastTimePocketToObjectPulse;
	double vLastObjectTime;
	BYTE vSeamerObjectPulseBitMap;
	BYTE vNextHeadToProcess[3];
	BYTE vNextHeadToPutInFIFO[3];
	BYTE vCountPutInFIFOThisRevolution[3];
	BYTE vPreviousHeadProcessed[3];
	BYTE vMissedIndexCount;
	bool vTimeToGetIndexPulse;
	BYTE vCurrentSampleDataCount[3];
	WORD vValveDataRowIndex;
	WORD vNumberValveRowDataHave;
	double *vCurrentValveData; //is the size of the number of valves,  data is stored in package after sync pulse is first
	double *vHoldValveDataForNextIndexPulse; //is the size of the number of valves,  when get a complete set of values, hold until next index pulse to be sure the index pulse was aligned properly
	double *vHoldValveDataForNextIndexPulseMaximum; 
	double *vValveData; //every n values is one set of cMaximumValveDataRows data from the valves, use vValveDataRowIndex * vNumberOfHeadsToMonitor to write next set of data in.  Data stored in Valve 1 is first
	double *vCurrentValveDataMaximum; //pointer must be less than this
	double *vValveDataMaximum; //pointer must be less than this
	double vFillerHighThreshold;
	double vFillerLowThreshold;
	double *vValveAverages;
	double *vValveAveragesMaximum;
	double *vValveStandardDeviations;
	double *vValveStandardDeviationsMaximum;
	double *vValveDataToAverage;
	int *vSamplingIndexErrorData[3]; //0=index pulse correct, Negative value = missed that many packages for that valve set, positive value = had that many extra packages for that valve set.  Is cMaximumValveDataRows in size
	int *vSamplingIndexErrorDataMaximum[3]; //Pointer must be less than this
	WORD vSamplingIndexErrorDataIndex[3];
	WORD vSamplingIndexErrorDataHave[3];
	BYTE vSamplingAuxiliaryDetector[3];
	double vSamplingLastProcessTime[3];
	double vMostRecentTimeBeforeIndexPulse[3];
	double vHeadCountAfterIndexPulse[3];
	double vMostRecentTimeAfterIndexPulse[3];
	bool vLastIndexPulseWasCorrect[3];
	CString vSamplingName[3];
	BYTE vNumberOfValvesThatNeedAttention;
	double vFillerAverageWeight;
	BYTE vFindFillerOffset[cNumberOfFillerOffsetSpeeds]; // 0 = Not Finding, 1 = Finding Fullest, 2 = Finding Lowest Fill, 3 = Finding most variable fill
	BYTE *vSamplingFIFO[3];
	BYTE *vSamplingFIFOMaximumPointer[3];
	BYTE vSamplingFIFOHead[3]; //points at the memory location to write the next value into
	BYTE vSamplingFIFOTail[3]; //points at the memory location to read the next value out of.  If Head and Tail are the same then the FIFO is empty
	CString vSamplingPulseLog[3];
	DWORD vSamplingPulseIndexCounter[3];
	bool vUseSamplingFIFO;
	WORD vBeltPositionLastPutItemInSamplingFIFO;
	int vHaveFillerIndexError;
	BYTE vPreviousFillerHeadPutInContainer;
	DWORD vFillerLowFillHeadCounts[255];
	DWORD vSamplingSeamerObjectPulseCounter;
	DWORD vSamplingSeamerPocketPulseCounter;

	//vSamplingIndexErrorDataMaximum[cSeamerSamplingType]
	//enum {cNoSamplingType, cFillerSamplingType, cSeamerSamplingType};

	//Seamer Head Sampling
	BYTE vSeamerIndexPulseBitMap;
	bool vTimeToGetSeamerIndexPulse;
	BYTE vMissedSeamerIndexCount;
	bool vSampledOnLastRevolution[3];

	BYTE vSamplingSkipNextSample;

	BYTE vSamplingNowWhichHeadToSample[3];
	BYTE vSamplingNowNumberOfSamplesToTakeFromSeries[3];
	BYTE vSamplingNowNumberOfSamplesToTakeFromSameHead[3];

	BYTE vSamplingParameterSetStartHead[3];  //this number is zero based, so add 1 to display to user, subtrac one if user enters which head
	BYTE vSamplingParameterSetHeadQuantity[3];
	BYTE vSamplingParameterSingleHead[3];	//this number is zero based, so add 1 to display to user, subtrac one if user enters which head
	BYTE vSamplingParameterSingleHeadQuantity[3];

	bool vNeedToDisableEjectorsAfterSampleAll;

	DWORD vSamplesEjectedCount[3];

	WORD vSampleIndexErrorsX[3];
	WORD vSampleIndexErrorsY[3];

	bool vManuallyEditedDuringEvaluate;
	BYTE vFPGADontCorrectForEncoderErrors;
	bool vCheckWeighOffsetCalibrationNeeded;
	bool vStartRunningAfterLoadProduct;
	bool vDetectorCalibrationNeeded;
	bool vWaitingForDACValueReturn;
	bool vWaitingForADCValueReturn;
	CString vSupportContractTypeStrings[6];
	WORD vDACTestErrorCount;
	WORD vDACTestValue;
	WORD vDACTestValueSent;
	BYTE vDACChannelWrote;
	WORD vDACErrorCount;
	bool vCompletedEvaluation;
	WORD vADCTestErrorCount;
	WORD vADCTestValue;
	WORD vADCTestValueSent;
	BYTE vADCChannelWrote;
	WORD vADCErrorCount;
	//BYTE vCollectingImagesToEvaluate;  //set to 1 to start, ends when reaches 11
		//vEvaluateDataThread
	BYTE vEvaluateAndOrQuickLearn;  //cQuickLearnOnly, cEvaluateAndQuickLearn, cEvaluateOnly

	BYTE vNeedToAdjustEjector;
	BYTE vSendDACValues;
	BYTE vSendADCValues;
	bool vABPixelAtOtherSide;
	CString vContainerString;
	CPen vBlackPen; 
	CPen vRedPen; 
	CPen vGreenPen; 
	CPen vMediumDarkGreenPen;
	CPen vYellowPen; 
	CPen vLightGreyPen; 
	CPen vGreyPen; 
	double vSensitivities[cMaximumNumberOfInspections];
	double vLearnSensitivities[cMaximumNumberOfInspections];
	double vThresholdSensitivities[cMaximumNumberOfInspections];
	BYTE vNumberOfSensitivities;
	
	//bool vTestingOnlyStopImages;
	BYTE vNoImageCounter;
	bool vRunningDiagnosticsMode;

	BYTE vPipeIsEmpty;
	BYTE vAirBubblesInPipe;
	BYTE vCurrentPipeStatus;
	BYTE vOldAirBubblesInPipe;
	double vSmallestDifferenceBetweenAverageAndMinimum;
	BYTE vPipeBrightnessCount;

	bool vDownStreamSignalSaysConveyorOn;
	WORD vOldConveyorEncoderRate;
	BOOL vSimulatingCamLinkImage;
	WORD vWorkStopTimePeriod;
	bool vAllSystemSettingsAreStandard;
	bool vInspxMessagesToView;
	bool vOldAllSystemSettingsAreStandard;
	bool vOldInspxMessagesToView;
	bool vOldStylePowerMoniter;
	CString vNonStandardSetting;

	CString vBIOSVersion;
	CString vBIOSDate;
	CString vVideoBIOSDate;
	CString vVideoBIOSVersion;

	//write rejects to Hard Disk
	CRejectImageBuffer vRejectImageBuffer[cNumberOfRejectImageBuffers];
	BYTE vNextRejectToWrite;
	BYTE vNextRejectToSave;

	bool vOperatorPasswordOK;
	CString vPrimaryDrive;
	CString vBackupDrive;
	CString vConditionString;
	CString vRegionalLocale;
	bool vImprovingLearn;
	bool vAdjustingBrightnessBeforeLearning;
	CString vCheckACBeforeThisYellowMessage;
	BYTE vCheckACBeforeThisYellowMessageTempLevel;
	WORD vCheckACBeforeThisYellowMessageTempErrorCode;
	double vCurrentBrightnessAdjustment;
	WORD vWrongWidthsDuringLearn;
	WORD vWrongBrightnessDuringLearn;
	WORD vRejectsDuringLearn;
	bool vACPowerOn;
	BYTE vSimulatingFastestEncoder;
	WORD vEncoderSampleIndex;
	int vEncoderSample[cNumberOfEncoderSamples];

	WORD vAirConditionerTemperatureSampleIndex;
	WORD vAirConditionerTemperatureSampleCount;
	BYTE vAirConditionerTemperatureSampleData[cNumberOfAirConditionerTemperatureSamples];
	
	BYTE vRedLampBrokenCounter;
	BYTE vYellowLampBrokenCounter;
	BYTE vBlueLampBrokenCounter;
	DWORD vMessageToMainMenu;
	DWORD vMessageToSubMenu;
	BYTE vMessageToMainMenuParameter[2];

	bool vMissingRAM;
	bool vShutterFailedLastTest;
	bool vProductHasAPoorLearn;
	bool vAirConditionerFailed;
	bool vWeightTrendingAlarm;
	bool vHadABrownOut;
	//bool vHadAnACPowerDropOut;
	int vHadAPowerMoniterMessageError;	
	bool vGaveEncoderErrorThisRun;
	bool vDisplayingAnEncoderError;
	BYTE vEncoderFaultyCount;
		//4096 Sag <106 for 10 seconds in the last 30, data from ac power monitor data log
		//2048 Spike on Nuetral 500
		//1024 Spike on Nuetral 100
		//512  Spike on Nuetral 50
		//256  Spike on Nuetral 5
		//128  Spike on Hot 500
		//64   Spike on Hot 100
		//32   Spike on Hot 50
		//16   Spike on Hot 5
		//8    Ground Nuetral Reversed
		//4    AC Voltage Surge > 135V
		//2    AC Voltage Sag < 95V
		//1    AC PowerDropout
	bool vProductHasAnOldInspection;
	double vDXMFilamentLimit;

	BYTE vCurrentDetectorAverage[cMaximumNumberOfDetectors12];
	BYTE vMaximumDetectorAverage[cMaximumNumberOfDetectors12];
	bool vHaveAlignmentDataToSave;
	BYTE vuCBoardAnalogValueReadCount;
	bool vNotCommissioned;
	bool vRemoveBackgroundInAutoSize;
	bool vWriteCalibrationImages;
	BYTE vPixelValuesCorrectedPixels;
	CString vPixelValuesString;
	BYTE vFullDetectorCalibration;
	bool vKeepDensityFeedbackLoopLocked;
	bool vShowingShuttingDownIndicator;
	BYTE vGenerateTestError;
	BYTE vCantInspectError;
	bool vOKToCloseProgram;
	DWORD vXRayTubeConditionStepTime;
	DWORD vXRayTubeConditionStepTimeLeft;
	BYTE vXRayTubeConditionGlitchCount;
	int vXRayTubeConditionCurrentStep;
	double vXRayTubeConditionVoltageTable[18];
	double vXRayTubeConditionCurrentTable[18];

	CTime vLastCalibrationTime;
	double vLastTickleTime;
	//bool vInLowExposureMode;
	CString vPreviousProduct;
	//bool vReadyForImage;
	BYTE vWaitingForImage;
	BYTE vTimesRetried;
	BYTE vGoodImagesIndex;
	bool vHaveAutoDisabledEjectors;
	bool vReCalibrationTimerRunning;
	BYTE vConveyorIsOn;
	bool vInspectionNameChanged;
	bool vInspectionNameMoved;
	bool vButtonNameLong[4 + //number of shape checks possible
		cMaximumNumberOfInspections + cNumberOfExternalDetectors + 1];  //one for good containers
	bool vButtonNameMedium[4 + 
		cMaximumNumberOfInspections + cNumberOfExternalDetectors + 1];
	BYTE vCountToUseForEjectReason[cMaximumRejectReason];
	bool vAutoCalibrate;
	bool vAutoCalibrateWhileRunning;
	bool vCalibrateUsingDAC;
	bool vCalibrateADCAfterDAC;
	BYTE vCalibrateErrorCount;
	BYTE vHadErrorTryRestart;
	BYTE vNumberOfXRayDetectorsUsing;
	//double vOriginalDetectorOffset[cMaximumNumberOfDetectors12];
	//double vNewDetectorOffset[cMaximumNumberOfDetectors12];
	bool vCalibrationSimulationStarted;

	BYTE vCalibrationImageNumber;
	BYTE vCalibrateGainIterationNumber;
	BYTE vFindingProductBrightness;
	BYTE vCalibrateDACPreFrames;

	int vCalibrationOffsetRange;
	int vCalibrationADCOffsetMaximumBit;
	int vCalibrationOffsetCurrentOffset[cMaximumNumberOfDetectors12];

	double vCalibrationGainRange;
	double vTargetDetectorGainValue;
	double vCalibrationOffsetCurrentGain[cMaximumNumberOfDetectors12];

	int vADCDetectorOffset[cMaximumNumberOfDetectors12]; 
	double vDACDetectorOffset[cMaximumNumberOfDetectors12];
	WORD vLastProgrammedDACDetectorOffset[cMaximumNumberOfDetectors12];
	WORD vLastProgrammedADCDetectorOffset[cMaximumNumberOfDetectors12];

	BYTE *vQuickLearnImages[cMaximumQuickLearnImages];
	BYTE *vQuickLearnWorkingImages[cMaximumQuickLearnImages];
	bool vTakingQuickLearnImages;
	WORD vQuickLearnImageIndex;
	bool vMemoryOK;
	Image *vDemoImage[cMaximumNumberOfDemoImages];
	WORD vNumberOfDemoImages;

	double vLastTimeSinceMinimumGood;
	double vImageProcessingTimeMaximum;
	double vImageProcessingTimeMinimum;
	double vImageProcessingTimeAverage;
	double vImageProcessingTimeStandardDeviation;
	WORD vImageProcessingTimeIndex;
	WORD vImageProcessingTimeSamples;
	double vImageProcessingTimeData[cImageProcessingTimeMaximum];

	DWORD vImageCounter;
	DWORD vTriggerCounter;
	DWORD vTriggerEndCounter;
	DWORD vStartImageCounter;
	DWORD vEndImageCounter;
	DWORD vProcessImageCounter;

	bool vTestingTrigger;
	float vFreeTimeMaximum;
	float vFreeTimeMinimum;
	float vFreeTimeAverage;
	float vFreeTimeStandardDeviation;
	WORD vFreeTimeIndex;
	WORD vFreeTimeSamples;
	float vFreeTimeData[cMaximumBodyTriggerWidthData];

	double vBodyTriggerWidthAverage;
	double vBodyTriggerWidthStandardDeviation;
	WORD vBodyTriggerWidthIndex;
	int vBodyTriggerWidthSamples;
	double vBodyTriggerWidthData[cMaximumBodyTriggerWidthData];
	//BYTE vGaveBodyTriggerWarning;
	bool vIncludeRejectsInAverageAndStructure;
	//CString vLastTextFileDirectory;
	BYTE vNumberOfShapeInspections;
	BYTE vTotalNumberOfInspections;
	BYTE vAuxiliaryDetectorIndex[cNumberOfExternalDetectors];
	WORD vRadiationDoorManualControlCurrentStatus;
	WORD vRadiationDoorManualControlRegister;
	WORD vRadiationDoorInDoorDelaySteps;
	WORD vRadiationDoorOutDoorDelaySteps;
	WORD vRadiationDoorEncoderDividerFactor;
	WORD vRadiationDoorDwellDistance;
	bool vDiverterOn;
	WORD vLVDSErrorCount;
	bool vMissedErrorMessages;
	CCriticalSection vErrorStringCollectionLock;
	/*
	CString vTheErrorMessageStringIAT[cMaximumNumberOfErrorMessages];
	BYTE vTheErrorMessageTypeIAT[cMaximumNumberOfErrorMessages];
	WORD vErrorMessageHeadIAT;
	WORD vErrorMessageTailIAT;
	CString vTheErrorMessageStringST[cMaximumNumberOfErrorMessages];
	BYTE vTheErrorMessageTypeST[cMaximumNumberOfErrorMessages];
	WORD vErrorMessageHeadST;
	WORD vErrorMessageTailST;
	*/
	WORD vCurrentConveyorOutput;

	WORD vShiftRateCPM;
	WORD vOldShiftRateCPM;
	double DensityOfLastNImages[cMaximumNumberOfImagesForDriftDensity];
	WORD vIndexForDriftDensityOfLastNImages;
	double vDriftCompensationThresholdReductionAmount;
	double vOldDriftCompensationThresholdReductionAmount;
	double vDriftCompensationThresholdIncreaseAmount;
	double vOldDriftCompensationThresholdIncreaseAmount;
	double vDriftCompensationImageBrightnessDelta;
	double vOldDriftCompensationImageBrightnessDelta;
	double vLastXRayCurrentSetTime;
	int vDriftCompensationADCOffset;
	int vDriftCompensationDACOffset;
	int vOldDriftCompensationADCOffset;
	int vOldDriftCompensationDACOffset;
	BYTE vNumberOfContainerTriggersConfigured;
	BYTE vNumberOfAuxiliaryDetectorsConfigured;
	double vMaximumExternalDetectorTime;
	BYTE vLastExternalDetectorUsed;
	BYTE vExtraWork;
	BYTE vuCHealthFactor;
	WORD vOutstandingAuxiliaryDetectorEnables;
	BYTE vAutoSetupAuxiliaryDetectorNumber;
	DWORD vLastAutoSetupAuxiliaryDetectorActivePosition;
	DWORD vLastAutoSetupAuxiliaryDetectorInActivePosition;
	WORD vOutstandingEjects;
	WORD vTestEjectDwellTime[cNumberOfEjectors];
	DWORD vEjectorsToFireNow;
	bool vShowImages;
	bool vShuttingDownXRays;
	bool vAdjustingSourceSettings;
	bool vThisContainerShouldBeEjected;
	BYTE vOldThisContainerShouldBeEjected;
	int vTweakReferenceAdjust;
	int vTweakTopReferenceAdjust;
	CString vLastErrorMessage;
	CTime vLastEMailErrorMessageTime;
	bool vYellowMessageButtonYellow;
	BYTE vButton2ShowsError;
	CString vOldOpenInterlock;
	CString vOpenInterlock;
	bool vTemporarilyChangeSource;
	bool vChangedSizeOfProduct;
	bool vSetupANewProduct;
	BYTE vChangedSizeOfProductCheckDensityROI;
	bool vInAutoSetup;
	bool vInExpressSetup;
	bool vManualReLearnWhileInspecting;
	CString vLastReport;
	CTime vReportingPeriodStartTime;
	CString vErrorLogStarted;
	tOutputStringCollection vErrorLog;
	tOutputStringCollection vInspxLog;
	DWORD vErrorCount;
	DWORD vErrorCountForLogFile;
	bool vImageTracerLineOn;
	BYTE vNumberOfVoltageSamples;
	BYTE vNumberOfDataSamples;
	BYTE vEject4InARowCount;
	BYTE vTestEjectWaitCount;
	BYTE vCurrentLampStatus;
	DWORD vCurrentEjectorOnStatus;
	DWORD vCurrentCanStopStatus;
	BYTE vCurrentAlarmStatus;
	BYTE vHaveABackup;  //which Auxiliary detector number caused the backup 1 to cNumberOfExternalDetectors
	DWORD vBackupCanStop;
	bool vDoingBackupDetection;
	WORD vLastBackupDetectorChangePosition[cNumberOfExternalDetectors];
	BYTE vBackupDetectorOn[cNumberOfExternalDetectors];

	bool vGivenTemperatureWarning;
	bool vGivenFilterCloggedWarning;
	bool vGaveTooManyRejectsAlarm[3];
	WORD vTooManyRejectsIndex[3];
	bool vContainerEjectData[cMaximumEjectDataSize][3];

	bool vSendingGoodWithBad;

	DWORD vMaximumBadCount;
	WORD vMinimumGoodIndex;
	BYTE *vMinimumGoodData;
	double *vMinimumGoodDataFloat;
	double vMinimumGoodStandardDeviation;
	double vMinimumGoodAverage;
	WORD vMaximumBadIndex;
	BYTE *vMaximumBadData;
	double *vMaximumBadDataFloat;
	double vMaximumBadStandardDeviation;
	double vMaximumBadAverage;
	double vQualityFactor;

	double vDensityLastValue;
	//float *vDensityData;
	double *vDensityDataD;
	double *vDetectorDensityData[cMaximumNumberOfDetectors12];
	WORD vLatestDACValueRead[cMaximumNumberOfDetectors12];
	WORD vLatestADCValueRead[cMaximumNumberOfDetectors12];
	BYTE vVerifyDACValues;
	BYTE vVerifyADCValues;
	BYTE vOldVerifyDACValues;
	BYTE vOldVerifyADCValues;
	WORD vDACTestCount;
	WORD vADCTestCount;
	bool vNeedToVerifyDACValues;
	bool vGenerateDACVerifyReport;
	BYTE vVerifyDACErros;
	WORD vDensityIndex;

	double vMaximumBadIntensity;
	double vMinimumGoodIntensity;
	//float vMaximumBadIntensityFloat;
	//float vMinimumGoodIntensityFloat;

	Image *vLoadedFileImage;
	bool vLoadedFileAvailable;
	BYTE vuCMainLoopCounter;
	bool vPostedNewMainImageReadyToDisplayMessage;
	CLogFile vLogFile;
	BYTE vPosteduCVoltageProblemMessage;
	WORD vLastEncoderCountToCalculateEncoderRate;
	bool vuCPoweredUp;
	BYTE vSourceErrorReported;
	bool vSourceRampedUp;
	double vRampXRayCurrentIncrement;
	double vRampXRayCurrentAmount;
	double vRampXRayVoltageIncrement;
	double vRampXRayVoltageAmount;
	DWORD vRampXRayWaitTime;

	bool vUControllerReady;
	bool vWaitingForUControllerReady;
	bool vCompletelyInitialized;
	CString vFrameCountString;
	double *vTopReferenceSample;
	double *vBottomReferenceSample;
	double *vRightReferenceSample;
	double *vLeftReferenceSample;
	WORD vRampXRayStep;
	tContainerCollection *vContainerCollection;
	bool vApplyGainAndOffsetDuringCalibration;
	bool vTouchScreen;
  DWORD vFrameCount;
	bool vReceivedACalibrationImage;
	int vSeqNum;

	CString vDetectorModeString[cNumberOfExternalDetectorModes];  // five modes, and not set
	double vMaximumEncoderRate;
	//double vSourceCurrentSetting;
	bool vXRayOnNotice;
	WORD vXRayState;
	WORD vImageNumberToDisplay;
	bool vGaveNoXRayMessage;
	unsigned long int vSimulatedContainerTimerHandle;
	unsigned long int vSimulatedContainerTimerDelay;
	bool vKeepExternalDetectorsEnabled;
	bool vEjectConfirmerUsed;
	BYTE vEjectConfirmerActive[cNumberOfEjectors]; //if eject confirmer active more than 5 seconds that eject tray must be full as sensor is blocked
	bool vuControllerSendExtraDiagnosticMessages;
	bool vCountTimeAsRunning;
	WORD vSecondsSinceLastContainer;

	WORD vCPMData[cMaxCPMTimePeriod];
	double vCPMDataTime[cMaxCPMTimePeriod];
	WORD vCPMDataIndex;
	bool vHaveAllCPMData;

	WORD vCPMDataNotRunning[cMaxCPMTimePeriod];
	WORD vCPMDataIndexNotRunning;

	BYTE vTestEjectNextContainer;
	BYTE vLastPattern;
	CString vCriteriaString[cMaximumRejectReason];
		//4 + cMaximumNumberOfInspections + cNumberOfExternalDetectors];  
	WORD vRawInterlockInput;
	BYTE vRawExternalDetectorInput;
	BYTE vRawBodyTriggerInput;
	int vFPGASimulateEncoderRate;
	WORD vSimulateContainersRate;
	WORD vSecondsMeasuringRealConveyorEncoder;
	DWORD vRealConveyorEncoderPulsesCount;

	bool vADRange13;
	bool vADInvert;
	bool vuCSimulate16Hz;
	bool vuCOutputSerialDebugData;
	bool vEnableduC16HzInterrupt;
	bool vFPGAGeneratingSimulatedData;
	WORD vuCResetCount;
	bool vGaveQueueOverflowMessage;
	BYTE vuCMajorVersionNumber;
	BYTE vuCMinorVersionNumber;
	int vuCWatchDogCount;
	BYTE vFPGAMajorVersionNumber;
	BYTE vFPGAMinorVersionNumber;
	BYTE vFPGASelfTest;
	UINT vCpuUsage;
	bool vFPGAVersion8Point7OrHigher;
	bool vFPGAVersion9Point0OrHigher;
	bool vFPGAVersion9Point1OrHigher;
	bool vFPGAVersion9Point2OrHigher;
	bool vFPGAVersion10Point7OrHigher;
	bool vFPGAVersion10Point86OrHigher;
	//bool vFPGAVersion10Point9OrHigher;

	WORD v5;  //from 12 bit a/d so never get 0xFFFF
	WORD v33;
	WORD v25;
	WORD v12;
	WORD vMinus12;
	WORD v15;  //in version 10.0 fpga and higher use for 12VIO

	WORD vTemperatureSurface; //is now used for outside air UP UNTIL VER 10.0 fpga
	WORD vTemperatureLiquid;
	WORD vTemperatureuC;
	WORD vTemperatureAirInside;  //is used for inside air
	WORD vTemperatureAirOutside;  

	WORD vTemperatureSurfaceMaximum;
	WORD vTemperatureLiquidMaximum;
	WORD vTemperatureuCMaximum;
	WORD vTemperatureAirMaximum;
	WORD vTemperatureAirOutsideMaximum;  
	WORD vCPUTemperatureMaximum;	// Cpu temperature max

	WORD vTemperatureSurfaceMinimum;
	WORD vTemperatureLiquidMinimum;
	WORD vTemperatureuCMinimum;
	WORD vTemperatureAirMinimum;
	WORD vTemperatureAirOutsideMinimum;  
	WORD vCPUTemperatureMinimum;	// Cpu temperature min

	BYTE vPSFault;
	BYTE vSpareMonitor;

	BYTE vDataIndex;
	BYTE v5Data[cNumberOfVoltageSamples];
	BYTE v33Data[cNumberOfVoltageSamples];
	BYTE v25Data[cNumberOfVoltageSamples];
	BYTE v12Data[cNumberOfVoltageSamples];
	BYTE vMinus12Data[cNumberOfVoltageSamples];
	BYTE v15Data[cNumberOfVoltageSamples];
	WORD vTemperatureSurfaceData[cNumberOfVoltageSamples];
	WORD vTemperatureLiquidData[cNumberOfVoltageSamples];
	WORD vTemperatureuCData[cNumberOfVoltageSamples];
	WORD vTemperatureCPUData[cNumberOfVoltageSamples];
	WORD vTemperatureAirInsideData[cNumberOfVoltageSamples];
	WORD vTemperatureAirOutsideData[cNumberOfVoltageSamples];
	BYTE vPSFaultData[cNumberOfVoltageSamples];
	BYTE vSpareMonitorData[cNumberOfVoltageSamples];

	bool vCPUTemperaturesReadOK;	// flag for CPU temperature read okay
	WORD vCPUTemperature;		// Cpu temperature
	bool vF_TemperatureType;	// Cpu temperature in Farienheit, true or false
	int TempCPUIndex;
	bool UseCPUAverage;
	WORD TemperatureCPUAverage;

	WORD vMax5;
	WORD vMax33;
	WORD vMax25;
	WORD vMax12;
	WORD vMaxMinus12;
	WORD vMax15;
	WORD vMin5;
	WORD vMin33;
	WORD vMin25;
	WORD vMin12;
	WORD vMinMinus12;
	WORD vMin15;
	BYTE vuCLEDGYR;
	BYTE vLastCommTestBytes[3];
	unsigned long int vDiagnosticDataReadTimerHandle;
	BYTE vDiagnosticData[cDiagnosticLineArraySize * 10];
	WORD vNextDiagnosticArrayEntry;
	BYTE vDiagnosticLineValue;
	bool vCaptureDiagnosticData;
	DWORD vEjectorUsed;
	WORD vExternalDetectorDecisionEnd;
	WORD vExternalDetectorDecisionStart;
	BYTE vCurrentBodyTriggerMask;
	BYTE vOnlyOneBodyTriggerEnabled;
	BYTE vBackupBodyTriggerMask;
	BYTE vDigitalInputLine3Mask;
	BYTE vCurrentExternalDetectorMask;
	BYTE vCurrentAlwaysOnExternalDetectorMask;
	BYTE vCurrentExternalDetectorActiveLowMask;
	BYTE vCurrentBodyTriggerActiveLowMask;
	BYTE vBackupBodyTriggerLines;
	BYTE vOldBackupBodyTriggerLines;
	BYTE vBodyTriggerLines;
	BYTE vOldBodyTriggerLines;

	int vBackupBodyTriggerVerifyFlag;

	bool vCurrentProductChanged;
	bool vNoCommunicationsWithUController;
	bool vOldNoCommunicationsWithUController;
	BYTE vCommunicationsTimeOutCounter;

	CITIPCDig *vITIPCDig;
	BYTE vLastXRayCommandOn;
	BYTE vXRayStatusIndex;
	BYTE vSourceVoltageReading[cNumberOfVoltageSamples];
	BYTE vSourceCurrentReading[cNumberOfVoltageSamples];
	double vSourceVoltage;
	double vSourceCurrent;
	double vSourceVoltageMin;
	double vSourceCurrentMin;
	double vSourceVoltageMax;
	double vSourceCurrentMax;

	double vSourceVoltageMinRedLine;
	double vSourceCurrentMinRedLine;
	double vSourceVoltageMaxRedLine;
	double vSourceCurrentMaxRedLine;

	WORD vSourceInterLocks;
	WORD vPreviousInterlockValue;
	double vLastInterlockChangedTime;
	DWORD vInterlock200mSecChangeCount;

	//BYTE vHVPSInterlockDelay;
	WORD vOldSourceInterLocks;
	CString vShiftProductCode;
	CString vPreviousShiftProductCode;
	CString vLastChatWindowText;
	bool vSimulating;
	CString vCurrentProductName;
	CString vOldLastDataSent;
	CString vOldLastDataReceived;
	COLORREF vCurrentColor;
	COLORREF vColorArray[32];
	LPCTSTR vColorArrayNames[32];
	HBRUSH vColorArrayBrush[32];
	HBRUSH vRedBrush;
	HBRUSH vBlackBrush;
	HBRUSH vLightGreenBrush;
	HBRUSH vLightRedBrush;
	HBRUSH vLightBrownBrush;
	HBRUSH vGreenBrush;
	HBRUSH vYellowBrush;
	HBRUSH vWhiteBrush;
	HBRUSH vGrayBrush;
	HBRUSH vLightBlueBrush;
	HBRUSH vBlueBrush;
	HBRUSH vDarkBlueBrush;
	HBRUSH vPlotBackGroundBrush;

	CFont vCurrentPunyFont;
	CFont vCurrentTinyFont;
	CFont vCurrentSmallFont;
	CFont vCurrentFont;
	CFont vCurrentMediumFont;
	CFont vCurrentMediumLargeFont;
	CFont vCurrentLargeFont;
	CFont vCurrentExtraHugeFont;
	CFont vUnderlinedFont;
	CFont vCurrentHugeFont;
	CFont vCurrentExtraLargeFont;

	BYTE vFontSelection;
	BYTE vSystemRunMode;
	BYTE vModeToRunAfterCalibrateOffset;
	BYTE vMainDisplayMode;
	BYTE SimEjectType;  //is a zero based number
	WORD vCurrentBeltPosition;
	WORD vCurrentEstimatedBeltPosition;
	WORD vOldCurrentBeltPosition;
	bool vEjectTooLateMessageGivenThisRun;

	WORD vConveyorEncoderRate;
	WORD vActualEncoderRate;
	UINT vBeltRate;
	UINT vContainerRate;
	double vPercentBeltSpeed;
	BYTE vGaveConveyorTooFastWarning;

	WORD vPercentOfTimeForProcessing;
	WORD vPercentOfTimeBeforeEject;

	WORD vActualEncoderRateMaximum;
	UINT vBeltRateMaximum;
	UINT vContainerRateMaximum;
	double vPercentBeltSpeedMaximum;

	BYTE vCheckConveyor;

	WORD vOldActualEncoderRate;
	WORD vLastReportedEncoderRate;
	UINT vOldBeltRate;
	UINT vOldContainerRate;

	double vPercentRejected;
	double vOldPercentRejected;
	double vPercentEjected;
	double vOldPercentEjected;
	unsigned long int vGoodCountInBufferTotal;
	unsigned long int vGoodCountTotal;
	unsigned long int vRealImageCountTotal;
	unsigned long int vEjectCountTotal;
	unsigned long int vRejectCountTotal;
	unsigned long int vOldRejectCountTotal;
	unsigned long int vProductionStopsCount;
	unsigned long int vStopDuration;
	unsigned long int vTimeSinceLastStop;
	unsigned long int vRunDuration;
	unsigned long int vCheckWeighRunDuration;
	unsigned long int vTotalDuration;
	unsigned long int vTotalContainers;
	unsigned long int vImproveLearnTotalContainers;
	unsigned long int vTotalConfirmed;
	unsigned long int vTotalUnConfirmed;

	unsigned long int vOldGoodCountTotal;
	unsigned long int vOldEjectCountTotal;
	unsigned long int vOldMissedEjectsBecauseDisabledCount;

	unsigned long int vOldXRayCriteriaCount[cMaximumNumberOfCountsDisplayed];
	unsigned long int vOldXRayCriteriaCountButDisabled[cMaximumNumberOfCountsDisplayed];
	unsigned long int vXRayCriteriaCount[cMaximumNumberOfCountsDisplayed];//ejected
	unsigned long int vXRayCriteriaCountButDisabled[cMaximumNumberOfCountsDisplayed];//disabled
	unsigned long int vXRayCriteriaCountTotal[cMaximumNumberOfCountsDisplayed];//rejected
	unsigned long int vOldProductionStopsCount;
	unsigned long int vOldStopDuration;
	unsigned long int vOldTimeSinceLastStop;
	unsigned long int vOldRunDuration;
	unsigned long int vOldTotalContainers;
	unsigned long int vOldTotalConfirmed;
	unsigned long int vOldTotalUnConfirmed;
	CString vRunStartTime;
	CTime vTimeStarted;

	unsigned long int vEjectorCount[cNumberOfEjectors];
	unsigned long int vEjectorConfirmCount[cNumberOfEjectors];
	unsigned long int vBodyTriggersCount;
	unsigned long int vBackupBodyTriggersCount;
	unsigned long int vValidBodyTriggersCount;
	unsigned long int vExternalDetectorCount[cNumberOfExternalDetectors];
	unsigned long int vIncompleteDataCount;
	unsigned long int vProcessingErrorCount;
	unsigned long int vTooLateToEjectErrorCount;
	bool vOldSetupPasswordRequiredToRun;
	unsigned long int vMissedEjectsBecauseDisabledCount;
	unsigned long int vImageReferenceErrorCount;

	CFile *vDiagnosticLineDataFile;

	WORD vLastEjectedPosition;
	BYTE vLastEjectedEjector;
	WORD vLastBodyTriggerPosition;
	WORD vLastBodyTriggerLength;
	WORD vLastBodyTriggerGapLength;
	BYTE vLastBodyTriggerLineState;
	WORD vLastExternalDetectorPosition;
	BYTE vLastExternalDetectorLineState;
	CString vLastEjectTime;
	int vGivenTemperatureSensorWarning;

	WORD vGlobalMonitorSizeX;
	WORD vGlobalMonitorSizeY;

	//CContainer *vContainerBeingImaged;
	CContainer *vLastContainerImaged;
	//CCriticalSection vLastContainerImagedLock;

	//WORD vUserListSize;
	WORD vNumberOfUsers;
	//UserPasswords* vUserList;
	tUserCollection vUserCollection;
	WORD vUserArchiveVersion;

	//Contructor
	CScanTracSystemRunningData();
	virtual ~CScanTracSystemRunningData();

	//methods
	//void SetGoodCountTotal(int NewGoodCount);
	//void SetEjectCountTotal(int NewEjectCount);
	DECLARE_DYNCREATE(CScanTracSystemRunningData)
	void CreateReferenceSamples(DWORD TempSize);
	void FreeReferenceSamples();
	void CalculateQualityFactor();
	void FreeQuickLearnImages();
	void UpdateUserList(tUserCollection& NewUserCollection);
	void SerializeUserList(CFile &TempFile, CArchive &TempArchive);
	void StoreUserList();
	void LoadUserList();

};

class UserPasswords : public CObject
{
	public: 
	CString vUserName;
	CString vPassword;
	CString vID;
	BYTE vLevel;
	DWORD vBCID;
	DWORD vRFID;
	bool operator==(UserPasswords &rhs)
	{
		if (vUserName == rhs.vUserName &&
			vPassword == rhs.vPassword &&
			vID == rhs.vID &&
			vLevel == rhs.vLevel &&
			vBCID == rhs.vBCID &&
			vRFID == rhs.vRFID)
			return true;
		return false; 
	}
	UserPasswords& operator =(const UserPasswords &rhs)
	{
		vUserName = rhs.vUserName;
		vPassword = rhs.vPassword;
		vID = rhs.vID;
		vLevel = rhs.vLevel;
		vBCID = rhs.vBCID;
		vRFID = rhs.vRFID;
		return *this;
	}
};

class CFunctionButton : public CStatic
{
	private:
		BYTE vArrowOrientation;
		bool vDoubleArrows;
		RECT vRectCoor;
		BYTE vNavigationArrow;
	public:
		CFunctionButton();
		void ShowNavigationArrow(BYTE TempArrowOrentationAndSize);
		int DestroyWindow();
		void SetArrows(BYTE TempArrowOrientation);
		BYTE AreNavigationArrowsSet();
		void SetArrows(BYTE TempArrowOrientation, bool TempDoubleArrows);
		RECT GetRectToInvalidateArrow();
		virtual void DrawArrows(CPaintDC& TempCDC);
	DECLARE_DYNAMIC( CFunctionButton )
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg void OnNcPaint();
};

class CLeftFunctionButton : public CFunctionButton
{
	public:
		CLeftFunctionButton() : CFunctionButton() {SetArrows(cArrowLeft);}
};

class CRightFunctionButton : public CFunctionButton
{
	public:
		CRightFunctionButton() : CFunctionButton() {SetArrows(cArrowRight);}
};

class CDownFunctionButton : public CFunctionButton
{
	public:
		CDownFunctionButton() : CFunctionButton() {SetArrows(cArrowDown);}
};

class CLeftDoubleFunctionButton : public CFunctionButton
{
	public:
		CLeftDoubleFunctionButton() : CFunctionButton() {SetArrows(cArrowLeft, true);}
};

class CRightDoubleFunctionButton : public CFunctionButton
{
	public:
		CRightDoubleFunctionButton() : CFunctionButton() {SetArrows(cArrowRight, true);}
};

class CRightSelectItemFunctionButton : public CFunctionButton
{
	public:
		CRightSelectItemFunctionButton() : CFunctionButton() {SetArrows(cPointyArrowSelectItemRight, true);}
};

class CLeftSelectItemFunctionButton : public CFunctionButton
{
	public:
		CLeftSelectItemFunctionButton() : CFunctionButton() {SetArrows(cPointyArrowSelectItemLeft, true);}
};
#endif // !defined(AFX_SYSTEMCONFIGURATIONDATA_H__61E500A4_EEC9_11D3_ABD6_00500466E305__INCLUDED_)
