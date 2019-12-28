//ScanTrac Side View Source File
// SystemConfigurationData.cpp: implementation of the CSystemConfigurationData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SystemConfigurationData.h"
#include "ScanTrac.h"
#include "NoticeDialog.h"
#include "SelectMultipleItemDialog.h"
#include "YesNoDialog.h"
#include "SerialPortGuardMaster.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CSerialPortGuardMaster *vGlobalGuardMaster;
extern BYTE vGlobalNormalLearnDilateTimes;
extern BYTE vGlobalQuickLearnDilateTimes;
extern BYTE vGlobalScanTracType;
extern CString vGlobalCurrentDirectory;
extern int vGlobalArchiveVersionNumber;
extern BOOL vGlobalHaveDDrive;
extern WORD vGlobalDisplaySizeH;
extern WORD vGlobalDisplaySizeV;
extern double vGlobalPercentToResizeX;
extern double vGlobalPercentToResizeY;
extern BYTE vGlobalUseCommaForDecimalPoint;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CSystemConfigurationData,CObject,VERSIONABLE_SCHEMA|cCurrentArchiveVersionNumber)

CSystemConfigurationData::CSystemConfigurationData()
{
	for (BYTE TempLoop = 0; TempLoop < 3; TempLoop++)
	{
		vNumberOfHeadsToMonitor[TempLoop] = 0;
	}
	vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType] = 0;
	vShowSampleAllButton = 0;


	vAutoImproveEnabled = 0;
	vAutoImproveReduceThresholdFactor = 5;
	vAutoImproveNumberToAddToLearn = 4000;
	vNumberOfDaysToKeepLogs = 100;

	vUsePipeFlowInput = 0;
	vUseDownStreamSignalThatStopsConveyor = cDontUseDownStream;
	vResynchronizeEjectors = 0;
	vBeltPositionDistancetoEjectorResynchronizationSensor = 0;

	vEnableModBusTCPIPInterface = 0;
	vApplyRetriggerLockoutToResynchronizingSensors = 0;
	vNumberOfPixelAtBottomToIgnoreInCalibration = 10;
	vSupportContractYearExpires = 0;
	vSpareInt4 = 0;
	vSpareInt5 = 0;
	vTimeXRaysWereOnLast = 0;

	vUseXofYAlarmOutputForUnconfirmedEject = 0;  //bits 87654321, bit 3 is ejector 3, bit 2 is ejector 2, bit 1 is ejector 1.
	vSpareWasHeadOffsetFromIndexPulse[0] = 0;
	vSpareWasHeadOffsetFromIndexPulse[3] = 0;
	vSaveAllImagesToRejectsFolder = false;
	vScanTracProductionReportDirectory = cScanTracProductionReportDirectory;
	vScanTracEventLogDirectory = cScanTracEventLogDirectory;
	vScanTracImagesDirectory = cImageDirectory;
	vScanTracCalibrationImagesDirectory = cScanTracCalibrationImagesDirectory;
	vScanTracSerialLogsDirectory = cScanTracSerialLogsDirectory;

	vSupportContractType = 0;
		//enum {cSupportContractInvalid, cSupportContract8_5, cSupportContract16_5, cSupportContract24_7, cHideSupportContractDisplay, cSupportContractExpired};

	vNumberOfCheckWeighOffsetSamples = 0;
	vRFIDEASBadgeReaderCommPort = 0;
	vProductSlipPercentValue = 0;

	vPixelsPerUnitForPoint4mmMode = 0;

	vEnableOperationsGuardian = 0;
	vDetectedValidGuardMasterDevice = 0;

	vSpareDWORD4 = 0;
	vSpareDWORD5 = 0;
	vSpareDWORD6 = 0;

	vSamplePassword = "";
	vSpareString3 = "";
	vSpareStringSC4 = "";
	vOperationsGuardianServer = cOperationsGuardianDefaultServerName; 
	vSpareStringSC5 = "";
	vSupportContractDayExpires = 0;

	vScanTracCustomerName = "Preferred Customer";

	vScanTracScreenShotDirectory = cScanTracScreenShotDirectory;
	vDetectorLength = 2;
	SetPixelsPerDetector(64);
	vQuickLearnNumberOfStructureDeltas = 4;
	vNumberOfQuickLearnImagesToTake = 300;
	vAdjustAllAreaOfHistogram = false;
	vShapeHandlingProcessIfAtSides = false;
	vNumberOfDetectorsToCalibrate = 0;
	vStopRunningOnUnConfirmedEject = false;
		//cShapeHandlingProcessNormally,cShapeHandlingEject,cShapeHandlingAccept
	vUseDACAndADCinDensityCompensation = true;
	vSurfaceTemperatureMovedToDXM = false;
	vErrorMessageDisplayMask = 0xFFFF;
	vRadiationDoorInvertDoorOutput = 0;
	vRadiationShutterInSystem = 0;
	vRadiationDoorDistanceStayOpen = 10;
	vRadiationDoorInDoorDistance = 1;
	vReCalibrateAfterStopTimePeriod = 60;
	vAlarmEjectorReEnableX = 0;
	vAlarmEjectorReEnableY = 0;

	vShapeHandlingProcessIfWrongHeight = 0;
	vStopInspectionIfConveyorStops = false;

	vEjectContainersNotInspected = false;
	vDiverterEjector = 3;
	vDiverterMargin = 4;
	vRemoveBackgroundFromImage = true;
	vAllowClientConnections = false;
	vShapeHandlingProcessIfWrongWidth = 0;
	vShapeHandlingProcessIfAtSides = 2;
	vShapeHandlingProcessIfWrongHeight = 0;
	vUseXScanDetectors = 0;
	vShapeHandlingProcessIfConveyorStops = 2;
	vShapeHandlingHeightTolerance = .2;
	vShapeHandlingWidthTolerance = .2;
	vShapeHandlingHeightTolerancePixels = 6;
	vShapeHandlingWidthTolerancePixels = 6;
	vShapeHandlingEjectors = 1;

	vDriftCompensationThresholdReductionFactor = 10;
	vDriftCompensationThresholdReductionMaximum = 50;

	vAdjustImageIntensity = false;
	vMaxImageIntensityAdjust = 25;
	vMaxImageIntensityAdjustBeforeLocked = 60;
	vDriftCompensationEnabled = true;
	vDriftCompensationWaitTime = 5000;
	vNumberOfImagesForDriftDensity = 10;
	vDriftCompensationFastThreshold = 5;
	vDriftCompensationFastAdjust = .1;
	vDriftCompensationVeryFastThreshold = 40;
	vDriftCompensationVeryFastAdjust = .5;
	vDriftCompensationSlowThreshold = 1;
	vDriftCompensationSlowAdjust = .01;
	vDriftCompensationMaximumAdjust = 1;
	vTemperatureAirOutsideAdjust = 0;

	vHasOutsideAirTemp = 1;
	vNumberOfJitterPositions = 0;
	vNumberOfTopJitterPositions = 0;
	vDetectorsRunDuration = 0;
	vScanTracRunDuration = 0;
	vXRaySourceRunDuration = 0;
	vAverageDisableCount = 200;
	vHaveCheckedForMoreRAM = 0x04;
	vScanTracSetupBackupDirectory = cScanTracSetupBackupDirectory;
	vLastBackupDirectoryUsed = 0;
	vScanTracConfigurationFileType = 0;
	vStructureDisableCount = 0;
	vDensityDisableCount = 0;
	vCalibrateBodyTriggerReferenceDisableCount = 0;
	vSlippedIndicatorTime = 0;
	vDeltaTemperatureForFilterCloggedError = 15;
	vCommand1 = 31;
	vCommand2 = 32;
	vCommand3 = 33;
	vCommand4 = 34;
	vMaximumCurrent = 8.55;
	vMaximumVoltage = 70;
	vMaximumPower = 600;
	for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
	{
		vMeasuredDetectorGain[TempLoop] = 0;
		vMeasuredDetectorOffset[TempLoop] = 0;
	}
	vVoltageAdjustOffset = 0;
	vCurrentAdjustOffset = 0;
	vVoltageAdjustGain = 0;
	vCurrentAdjustGain = 0;
	vSystemBodyTriggerToXRayBeam = 0;
	vSystemEjectorPositionOffset = 0;
	vCalibratedPixelsPerUnit = 0;
	vScanTracID = "ScanTrac 01";
	for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
	{
		for (WORD TempLoop = 0; TempLoop < cMaximumEMailRecievers; TempLoop++)
		{
			vEMailUser[TempLoop * TempLoopW] = "";
			vEMailDomain[TempLoop * TempLoopW] = ".Com";
		}
		vEMailEnable[TempLoopW] = false;
		vNumberOfEMailRecievers[TempLoopW] = 0;
	}
	vEMailSubject[0] = "ScanTrac Plant Operations Message";
	vEMailSubject[1] = "ScanTrac Inspx Message";
	vEMailSubject[2] = "ScanTrac Plant Maintenance Message";
	vEMailSubject[3] = "ScanTrac Production Report";
/*
	cEMailPlantOperations = 0;
	cEMailInspx = 1;
	cEMailMaintenance = 2;
	cEMailReport = 3;
*/
	vPreAmplifierGain = 0;

	vSourceStrengthCount = 0;
	for (WORD TempLoopW = 0; TempLoopW < cMaximumSourceStrengthCount; TempLoopW++)
	{
		vSourceStrengthDate[TempLoopW] = 0;
		vSourceStrengthData[TempLoopW] = 0;
		vSourceStrengthComment[TempLoopW] = "";
	}
	vTemperatureSurfaceAdjust = 0;
	vTemperatureLiquidAdjust = 0;
	vTemperatureuCAdjust = 0;
	vTemperatureAirInsideAdjust = 0;
	vMaximumSurfaceTemperature = 50;
	vMaximumLiquidTemperature = 50;
	vMaximumuCTemperature = 60;
	vMaximumAirTemperature = 45;
	vMaximumTemperatureAirOutside = 40;
	//vMaximumCoolantPressure = 60;  
	//vMinimumCoolantPressure = 20;  
	vNumberOfLastRejectImages = 50;
	vXRaySourceFilter = true;
	vLastProduct = "No Product";
	vShowOnlyRealImages = true;
	vArchiveVersionNumber = 0;
	vIPDefaultDNSAddressString = "";
	vHVPSType = cXLFHVPS;
	vLogSerialData = false;
	vLogSyncData = true;
	//initialize Container trigger objects
	CString TempString;
	CString TempString1;
	for (BYTE TempByteLoop = 0; TempByteLoop < cNumberOfBodyTriggers; TempByteLoop++)
	{
		vBodyTrigger[TempByteLoop].vNumber = TempByteLoop;
		vBodyTrigger[TempByteLoop].vName = "Container Trigger " + dtoa(TempByteLoop + 1, 0);
		vBodyTrigger[TempByteLoop].vEnabled = false;
		vBodyTrigger[TempByteLoop].SetDigitalInputLine(TempByteLoop + 1);
		vBodyTrigger[TempByteLoop].vActiveHigh = 1;
	}
	//initialize Auxiliary detector objects
	for (BYTE TempByteLoop = 0; TempByteLoop < cNumberOfExternalDetectors; TempByteLoop++)
	{
		vExternalDetector[TempByteLoop].vNumber = TempByteLoop;
		vExternalDetector[TempByteLoop].vName = "Auxiliary Detector " + dtoa(TempByteLoop + 1, 0);
		vExternalDetector[TempByteLoop].vEnabled = false;
		vExternalDetector[TempByteLoop].SetDigitalInputLine(TempByteLoop + 1);
		vExternalDetector[TempByteLoop].vActiveHigh = 1;
	}
	//initialize ejector objects
	for (BYTE TempByteLoop = 0; TempByteLoop < cNumberOfEjectors; TempByteLoop++)
	{
		vEjector[TempByteLoop].vNumber = TempByteLoop;
		vEjector[TempByteLoop].vName = "Ejector " + dtoa(TempByteLoop + 1, 0);
		vEjector[TempByteLoop].vEnabled = false;
		vEjector[TempByteLoop].SetDigitalInputLine(TempByteLoop + 1);
		vEjector[TempByteLoop].vActiveHigh = false;
		vEjector[TempByteLoop].vConfirmEnabled = false;
		vEjector[TempByteLoop].vConfirmActiveHigh = 1;

		vSystemEjectorDistance[TempByteLoop] = 0;
	}
	vReferenceROIsToEdges = 0;
	vPipeLineFlowRate = 0;
	vDeltaTemperatureForCoolantProblem = 0;
	vLearnQualityPoorLimit = 45;
	vLearnQualityMarginalLimit = 35;
	vACPowerMonitorOffset = 0;
	vMaximumCPUTemperature = 62;
	vEncoderTolerance = 0;
	vXRayTubeHasCooled = true;
	vAutoXRaySourceOffEnabled = 1;
	vDemoMode = false;
	vEnableEjectors = 1;
	vRampXRayDuration = 20;
	vCorrectDetectorEdges = true;
	vSimulateLightControlBoard = false;

	vShowMouseCursor = false;
	vLanguage = cEnglish;

	vBulkProductMode = 0;
	vCPMTimePeriod = 30;
	
	vAutoProductionReportHour = 0;
	vImageShowScale = false;
	vNumberOfEncoderRateReversalsForError = false;

	vSuppressWarningMessages = 0;

	for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
	{
		vDetectorGain[TempLoop] = 10;
		vDACDetectorOffset[TempLoop] = 0;
		vADGainByte[TempLoop] = 120;
		vADGain[TempLoop] = 120;
		vADCDetectorOffset[TempLoop] = 0;
	}
	for (WORD TempDetectorTypeLoop = cPoint4mmDetectors; TempDetectorTypeLoop <= cPoint8mmDetectors; TempDetectorTypeLoop++)
	for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
	{
		vDetectorEdgeAdjustGain[TempLoop][TempDetectorTypeLoop] = 0;
		vDetectorEdgeAdjustOffset[TempLoop][TempDetectorTypeLoop] = 0;
		vOverWriteBadPixel[TempLoop][TempDetectorTypeLoop] = 0;
	}
	for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
		vDetectorEdgeAdjustOffsetInteger[TempLoop] = 0;
	
	for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfSourceStrengthValues21; TempLoop++)
		vSourceStrengthInitialValues[TempLoop] = 0;

	vSaveNextSourceStrengthValuesAsInitial = false;

	vEncoderDivider = 4;			//was 5 11/13/2012
	vNumberOfXRayDetectors = 6;
	vMaximumMonoBlockTemperature = 50;
	vNormalLearnDilateTimes = 0;
	vDetectorClockDivider = 20;
	vAskForProductionReportProductCode = 0;
	vBackupDetectionUsingBodyTriggerTimeOut = 0;
	vPixelsPerUnit = 32;
	vUnitsString.LoadString(IDS_Inches);

	vProductSimpleSetupPassword = "357";
	vProductSetupPassword = "1479";
	vQAPassword = "1479";
	vCertifiedSetupPassword = "147963";
	vInspxPassword = "304259";
	vSystemSetupResetPassword = "95038603";
	for (BYTE TempByteLoop = 0;TempByteLoop < cNumberOfDiagnosticLines; TempByteLoop++)
		vDiagnosticLineSelect[TempByteLoop] = TempByteLoop;
	vScanTracType = 0; //0 = solo, 1 = piccolo, 2 = legato 3 = 600 Watt Pipeline
	vUse101VACFor10SecondsShutDownLimit = 0;

	vRedIndicatorToShowOnMainMenu = cDefectInRedBox;
	vTimeLastMeasuredSourceStrength = 0;
	vUseExternalInterlock = 0;

	vSpareCTimeCD1 = 0;
	vSpareCTimeCD2 = 0;
	vSpareCTimeCD3 = 0;
	vSpareCTimeCD4 = 0;
	vSpareCTimeCD5 = 0;
	vAutoStop = 0;
	vIgnoreCheyneyInterlockStatus = 0;
	vReferenceContainersSideToSide = 0;
	vStopAfterAnInspectionRejects = 0;
	vSpareByteCD4 = 0;

	vSpareWordCD6 = 0;
	vSpareWordCD7 = 0;
	vSpareWordCD8 = 0;
	vSpareWordCD9 = 0;
	vSpareStringSC6 = "";

	vImageShowLines = true;
	vUseRealEncoder = 0;
	vImageShowFixedROILines = false;
	vNeedToCalibrateEncoder = 0;
	vWriteRejectsToFiles = 0;
	vSlipsEjectEmptyContainers = 0;
	vAdvancedSetupPassword = "147963";
	vIPAddressString = ""; 
	vIPDHCPEnabled = true;
	vIPSubNetMaskString = ""; 
	vIPDefaultGatewayString = ""; 
	vScanTracRejectedImagesDirectory = ""; 
	vOperatorPassword = ""; 
	vSupportContractMonthExpires = 0;
	vSpareDouble3 = 0;
	vSpareDoubleCD4 = 0;
	vSpareDoubleCD5 = 0;
	vSpareDoubleCD6 = 0;
	vEncoderRateTolerance = 0;
	vEncoderDividerForPoint4mmMode = 0;
	vSpareWord4 = 0;
	vSpareWord5 = 0;
	vAutoProductionReportHour2 = 0;
	vAutoProductionReportHour3 = 0;
	vShowSensitivityDisplays = cShowSensitivityDisplays;

	vShow24HourTime = 0;
	vAutoDeleteFileTypes = 0;
	vContinuousFeedEncoderRate = 0;
	vContinuousFeedContainerRate = 0;
	vResetCountsPassword = "";
	vSpareDWORD3 = 1000;
	vInspxMessageCount = 0;
	vAllowVoidInspection = 0;
	vTemperatureSurfaceAdjustint = 0;
	vTemperatureLiquidAdjustint = 0;
	vTemperatureuCAdjustint = 0;
	vTemperatureAirAdjustint = 0;
	vStaggerSampling = 0;
	vShowIndexErrors = 0;
	vDistanceFromSeamerObjectToScanTracTrigger = 0;
}

CSystemConfigurationData::~CSystemConfigurationData()
{
}

//IMPLEMENT_DYNCREATE(CSystemConfigurationData, CObject)
void CSystemConfigurationData::Serialize( CArchive& TempArchive )
{
  // call base class function first
  // base class is CObject in this case
  CObject::Serialize( TempArchive );

  // now do the stuff for our specific class
  if (TempArchive.IsStoring())
	{
		if ((vGlobalGuardMaster) && (vGlobalGuardMaster->vDetectedValidGuardMasterDevice)) //if detected the device for the first time, set that you have it.
			vDetectedValidGuardMasterDevice = true;

		vScanTracConfigurationFileType = 0;

		TempArchive << vEncoderDivider
			<< vNumberOfXRayDetectors
			<< vMaximumMonoBlockTemperature
			<< vNormalLearnDilateTimes
			<< vMaximumTemperatureAirOutside
			<< vDetectorClockDivider
			<< vNumberOfLastRejectImages
			<< vBackupDetectionUsingBodyTriggerTimeOut
			<< vNumberOfHeadsToMonitor[cFillerSamplingType]
			<< vImageShowLines
			<< vAutoImproveReduceThresholdFactor
			<< vNeedToCalibrateEncoder
			<< vWriteRejectsToFiles
			<< vSlipsEjectEmptyContainers
			<< vPixelsPerUnit
			<< vAutoImproveEnabled
			<< vScanTracType
			<< vCPMTimePeriod
			<< vEnableACMessages
			<< vBulkProductMode
			<< vRedIndicatorToShowOnMainMenu
			<< vImageShowFixedROILines
			<< vAutoImproveReduceThresholdFactor
			<< vSuppressWarningMessages
			<< vAutoProductionReportHour
			<< vImageShowScale
			<< vNumberOfEncoderRateReversalsForError
			<< vShowMouseCursor
			<< vLanguage
			<< vCorrectDetectorEdges
			<< vSimulateLightControlBoard
			<< vLearnQualityMarginalLimit
		  << vUseRealEncoder
			<< vRampXRayDuration
			<< vEnableEjectors
			<< vAutoDeleteFileTypes
			<< vAutoXRaySourceOffEnabled
			<< vInspxErrorMessageDisplayMask
			<< vXRayTubeHasCooled
			<< vAutoImproveEnabled
			<< vACPowerMonitorOffset
			<< vEncoderTolerance
			<< vQuickLearnDilateTimes
			<< vContinuousFeedEncoderRate
			<< vContinuousFeedContainerRate
			<< vInspxMessageCount
			<< vSpareDWORD3
			<< vAllowVoidInspection
			<< vDeltaTemperatureForCoolantProblem
			<< vLearnQualityPoorLimit
			<< vPipeLineFlowRate
			<< vReferenceROIsToEdges
			<< vLogSerialData
			<< vLogSyncData
			<< vHVPSType
			<< vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
			<< vDemoMode
			<< vIPDefaultDNSAddressString
			<< vShow24HourTime
			<< vShowOnlyRealImages
			<< vXRaySourceFilter
			<< vLastProduct
			<< vTemperatureSurfaceAdjust
			<< vTemperatureLiquidAdjust
			<< vTemperatureuCAdjust
			<< vTemperatureAirInsideAdjust
			<< vSourceStrengthCount
			<< vAskForProductionReportProductCode
			<< vPreAmplifierGain
			<< vRemoveBackgroundFromImage
			<< vScanTracID
			<< vVoltageAdjustOffset
			<< vCurrentAdjustOffset
			<< vVoltageAdjustGain
			<< vCurrentAdjustGain
			<< vSystemBodyTriggerToXRayBeam
			<< vSystemEjectorPositionOffset
			<< vCalibratedPixelsPerUnit
			<< vMaximumSurfaceTemperature
			<< vMaximumLiquidTemperature
			<< vMaximumuCTemperature
			<< vMaximumAirTemperature
			<< vMaximumVoltage
			<< vMaximumCurrent
			<< vMaximumPower
			<< vCommand1
			<< vCommand2
			<< vCommand3
			<< vCommand4
			<< vStructureDisableCount
			<< vDensityDisableCount
			<< vCalibrateBodyTriggerReferenceDisableCount
			<< vSlippedIndicatorTime
			<< vUseXScanDetectors
			<< vDeltaTemperatureForFilterCloggedError
			<< vShapeHandlingProcessIfConveyorStops
			<< vScanTracConfigurationFileType
			<< vLastBackupDirectoryUsed
			<< vScanTracSetupBackupDirectory
			<< vHaveCheckedForMoreRAM
			<< vDetectorLength
			<< vAverageDisableCount
			<< vXRaySourceRunDuration
			<< vNumberOfJitterPositions
			<< vHasOutsideAirTemp
			<< vDriftCompensationWaitTime
			<< vNumberOfImagesForDriftDensity
			<< vDriftCompensationFastThreshold
			<< vDriftCompensationFastAdjust
			<< vDriftCompensationSlowThreshold
			<< vDriftCompensationSlowAdjust
			<< vDriftCompensationEnabled
			<< vDriftCompensationThresholdReductionFactor
			<< vDriftCompensationThresholdReductionMaximum
			<< vDriftCompensationMaximumAdjust
			<< vTemperatureAirOutsideAdjust
			<< vAllowClientConnections
			<< vShapeHandlingProcessIfWrongWidth
			<< vShapeHandlingHeightTolerance
			<< vShapeHandlingWidthTolerance
			<< vShapeHandlingHeightTolerancePixels
			<< vShapeHandlingWidthTolerancePixels
			<< vShapeHandlingEjectors
			<< vMaxImageIntensityAdjustBeforeLocked
			<< vEjectContainersNotInspected
			<< vDiverterEjector
			<< vStopInspectionIfConveyorStops
			<< vShapeHandlingProcessIfWrongHeight
			<< vAlarmEjectorReEnableX
			<< vRadiationDoorDistanceStayOpen
			<< vRadiationDoorInDoorDistance
			<< vReCalibrateAfterStopTimePeriod
			<< vRadiationDoorInvertDoorOutput
			<< vRadiationShutterInSystem
			<< vErrorMessageDisplayMask
			<< vSurfaceTemperatureMovedToDXM
			<< vUseDACAndADCinDensityCompensation
			<< vAdjustImageIntensity
			<< vNumberOfDetectorsToCalibrate
			<< vAlarmEjectorReEnableY
			<< vNumberOfTopJitterPositions
			<< vDiverterMargin
			<< vStopRunningOnUnConfirmedEject
			<< vMaxImageIntensityAdjust
			<< vShapeHandlingProcessIfAtSides
			<< vDriftCompensationVeryFastThreshold
			<< vDriftCompensationVeryFastAdjust
			<< vAdjustAllAreaOfHistogram
			<< vNumberOfQuickLearnImagesToTake
			<< vQuickLearnNumberOfStructureDeltas
			<< vPixelsPerDetector
			<< vScanTracCustomerName

			<< vAutoImproveNumberToAddToLearn
			<< vNumberOfDaysToKeepLogs
			<< vAutoImproveNumberToAddToLearn
			<< vUsePipeFlowInput
			<< vUseDownStreamSignalThatStopsConveyor
			<< vLastSimulatedEncoderRateSet
			<< vNumberOfPixelAtBottomToIgnoreInCalibration

			<< vDetectorsRunDuration
			<< vScanTracRunDuration
			<< vSupportContractYearExpires
			<< vSpareInt4
			<< vSpareInt5
			<< vTimeXRaysWereOnLast

			<< vScanTracProductionReportDirectory
			<< vScanTracEventLogDirectory
			<< vScanTracImagesDirectory
			<< vScanTracCalibrationImagesDirectory
			<< vScanTracSerialLogsDirectory
			<< vScanTracScreenShotDirectory

			<< vPixelsPerUnitForPoint4mmMode
			<< vDistanceFromSeamerObjectToScanTracTrigger

			<< vSpareWasHeadOffsetFromIndexPulse[cSeamerSamplingType]
			<< vUseXofYAlarmOutputForUnconfirmedEject
			<< vStaggerSampling
			<< vShowIndexErrors
			<< vSpareWasHeadOffsetFromIndexPulse[0]
			<< vShowSampleAllButton
			<< vNumberOfHeadsToMonitor[cRandomSamplingType]
			<< vDetectorsAtBottomToIgnoreDuringCalibration
			<< vSpareDWORD4
			<< vSpareDWORD5
			<< vSpareDWORD6

			<< vCertifiedSetupPassword
			<< vAdvancedSetupPassword
			<< vIPAddressString
			<< vIPSubNetMaskString
			<< vIPDefaultGatewayString
			<< vScanTracRejectedImagesDirectory
			<< vOperatorPassword
			<< vMaximumCPUTemperature
			<< vSupportContractMonthExpires
			<< vSpareDouble3
			<< vAutoProductionReportHour2
			<< vAutoProductionReportHour3
			<< vIPDHCPEnabled
			<< vShowSensitivityDisplays
			<< vSaveNextSourceStrengthValuesAsInitial
			<< vSamplePassword
			<< vOperationsGuardianServer
			<< vSpareString3
			<< vQAPassword
			<< vProductSimpleSetupPassword
			<< vSupportContractDayExpires
			<< vSupportContractType
			<< vNumberOfCheckWeighOffsetSamples
			<< vNumberOfHeadsToMonitor[cSeamerSamplingType]
			<< vRFIDEASBadgeReaderCommPort
			<< vProductSlipPercentValue
			<< vProductSetupPassword
			<< vResetCountsPassword
			<< vSystemSetupResetPassword
			<< vInspxPassword
			<< vSpareWasHeadOffsetFromIndexPulse[3]
			<< vSaveAllImagesToRejectsFolder
			<< vGlobalUseCommaForDecimalPoint
			<< vSpareStringSC4
			<< vSpareStringSC5
			<< vSpareDoubleCD4
			<< vSpareDoubleCD5
			<< vEncoderRateTolerance
			<< vEncoderDividerForPoint4mmMode
			<< vBeltPositionDistancetoEjectorResynchronizationSensor
			<< vSpareWord4
			<< vSpareWord5
			<< vTimeLastMeasuredSourceStrength
			<< vSpareCTimeCD1
			<< vSpareCTimeCD2
			<< vSpareCTimeCD3
			<< vSpareCTimeCD4
			<< vUse101VACFor10SecondsShutDownLimit
			<< vUseExternalInterlock
			<< vEnableOperationsGuardian
			<< vAutoStop
			<< vDetectedValidGuardMasterDevice
			<< vEnableModBusTCPIPInterface
			<< vIgnoreCheyneyInterlockStatus
			<< vSpareWordCD6
			<< vSpareWordCD7
			<< vSpareWordCD8
			<< vSpareWordCD9
			<< vSpareStringSC6
			<< vReferenceContainersSideToSide
			<< vStopAfterAnInspectionRejects
			<< vResynchronizeEjectors
			<< vApplyRetriggerLockoutToResynchronizingSensors
			<< vSpareByteCD4
			<< vSpareDoubleCD6
			<< vSpareCTimeCD5;

		if (vScanTracConfigurationFileType == cSingleImageScanTrac)
		{
			for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
			{
				TempArchive << vEMailSubject[TempLoopW]
					<< vEMailEnable[TempLoopW]
					<< vNumberOfEMailRecievers[TempLoopW];
				for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
				{
					TempArchive << vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
						<< vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
				}
			}

			for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
			{
				TempArchive << vSourceStrengthDate[TempLoop]
					<< vSourceStrengthData[TempLoop] 
					<< vSourceStrengthComment[TempLoop];
			}

			for (WORD TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
				TempArchive	<< vSystemEjectorDistance[TempLoop];

			for (WORD TempLoop = 0; TempLoop < cNumberOfBodyTriggers; TempLoop++)
				vBodyTrigger[TempLoop].Serialize(TempArchive);
			
			for (WORD TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
				vExternalDetector[TempLoop].Serialize(TempArchive);
			
			for (WORD TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
				vEjector[TempLoop].Serialize(TempArchive);
			
			for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
				TempArchive << vDiagnosticLineSelect[TempLoop];
			
			for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
				TempArchive << vDetectorEdgeAdjustGain[TempLoop][0] << vDetectorEdgeAdjustGain[TempLoop][1];

			for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
				TempArchive << vOverWriteBadPixel[TempLoop][0] << vOverWriteBadPixel[TempLoop][1];

			for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
				TempArchive << vDetectorGain[TempLoop] << vADCDetectorOffset[TempLoop] << vDACDetectorOffset[TempLoop];

			for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
				TempArchive << vMeasuredDetectorGain[TempLoop] << vMeasuredDetectorOffset[TempLoop];

			for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
				TempArchive << vDetectorEdgeAdjustOffset[TempLoop][0] << vDetectorEdgeAdjustOffset[TempLoop][1];

			for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfSourceStrengthValues21; TempLoop++)
				TempArchive << vSourceStrengthInitialValues[TempLoop];
		}
	}
	else  //reading archive data
	{
		// may read different versions of object if have them
		// int vVersion = archive.GetObjectSchema();
    vArchiveVersionNumber = TempArchive.GetObjectSchema();
		vGlobalArchiveVersionNumber = vArchiveVersionNumber;

		if (vArchiveVersionNumber > 156) 
		{//version 157
			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectors
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions
				>> vDiverterMargin
				>> vStopRunningOnUnConfirmedEject
				>> vMaxImageIntensityAdjust
				>> vShapeHandlingProcessIfAtSides
				>> vDriftCompensationVeryFastThreshold
				>> vDriftCompensationVeryFastAdjust
				>> vAdjustAllAreaOfHistogram
				>> vNumberOfQuickLearnImagesToTake
				>> vQuickLearnNumberOfStructureDeltas
				>> vPixelsPerDetector
				>> vScanTracCustomerName

				>> vAutoImproveNumberToAddToLearn
				>> vNumberOfDaysToKeepLogs
				>> vAutoImproveNumberToAddToLearn
				>> vUsePipeFlowInput
				>> vUseDownStreamSignalThatStopsConveyor
				>> vLastSimulatedEncoderRateSet
				>> vNumberOfPixelAtBottomToIgnoreInCalibration

				>> vDetectorsRunDuration
				>> vScanTracRunDuration
				>> vSupportContractYearExpires
				>> vSpareInt4
				>> vSpareInt5
				>> vTimeXRaysWereOnLast

				>> vScanTracProductionReportDirectory
				>> vScanTracEventLogDirectory
				>> vScanTracImagesDirectory
				>> vScanTracCalibrationImagesDirectory
				>> vScanTracSerialLogsDirectory
				>> vScanTracScreenShotDirectory

				>> vPixelsPerUnitForPoint4mmMode
				>> vDistanceFromSeamerObjectToScanTracTrigger

				>> vSpareWasHeadOffsetFromIndexPulse[cSeamerSamplingType]
				>> vUseXofYAlarmOutputForUnconfirmedEject
				>> vStaggerSampling
				>> vShowIndexErrors
				>> vSpareWasHeadOffsetFromIndexPulse[0]
				>> vShowSampleAllButton
				>> vNumberOfHeadsToMonitor[cRandomSamplingType]
				>> vDetectorsAtBottomToIgnoreDuringCalibration
				>> vSpareDWORD4
				>> vSpareDWORD5
				>> vSpareDWORD6

				>> vCertifiedSetupPassword
				>> vAdvancedSetupPassword
				>> vIPAddressString
				>> vIPSubNetMaskString
				>> vIPDefaultGatewayString
				>> vScanTracRejectedImagesDirectory
				>> vOperatorPassword
				>> vMaximumCPUTemperature
				>> vSupportContractMonthExpires
				>> vSpareDouble3
				>> vAutoProductionReportHour2
				>> vAutoProductionReportHour3
				>> vIPDHCPEnabled
				>> vShowSensitivityDisplays
				>> vSaveNextSourceStrengthValuesAsInitial
				>> vSamplePassword
				>> vOperationsGuardianServer
				>> vSpareString3
				>> vQAPassword
				>> vProductSimpleSetupPassword
				>> vSupportContractDayExpires
				>> vSupportContractType
				>> vNumberOfCheckWeighOffsetSamples
				>> vNumberOfHeadsToMonitor[cSeamerSamplingType]
				>> vRFIDEASBadgeReaderCommPort
				>> vProductSlipPercentValue
				>> vProductSetupPassword
				>> vResetCountsPassword
				>> vSystemSetupResetPassword
				>> vInspxPassword
				>> vSpareWasHeadOffsetFromIndexPulse[3]
				>> vSaveAllImagesToRejectsFolder
				>> vGlobalUseCommaForDecimalPoint
				>> vSpareStringSC4
				>> vSpareStringSC5
				>> vSpareDoubleCD4
				>> vSpareDoubleCD5
				>> vEncoderRateTolerance
				>> vEncoderDividerForPoint4mmMode
				>> vBeltPositionDistancetoEjectorResynchronizationSensor
				>> vSpareWord4
				>> vSpareWord5
				>> vTimeLastMeasuredSourceStrength
				>> vSpareCTimeCD1
				>> vSpareCTimeCD2
				>> vSpareCTimeCD3
				>> vSpareCTimeCD4
				>> vUse101VACFor10SecondsShutDownLimit
				>> vUseExternalInterlock
				>> vEnableOperationsGuardian
				>> vAutoStop
				>> vDetectedValidGuardMasterDevice
				>> vEnableModBusTCPIPInterface
				>> vIgnoreCheyneyInterlockStatus
				>> vSpareWordCD6
				>> vSpareWordCD7
				>> vSpareWordCD8
				>> vSpareWordCD9
				>> vSpareStringSC6
				>> vReferenceContainersSideToSide
				>> vStopAfterAnInspectionRejects
				>> vResynchronizeEjectors
				>> vApplyRetriggerLockoutToResynchronizingSensors
				>> vSpareByteCD4
				>> vSpareDoubleCD6
				>> vSpareCTimeCD5;

			if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
					TempArchive	>> vSystemEjectorDistance[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0] >> vDetectorEdgeAdjustGain[TempLoop][1];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
					TempArchive >> vOverWriteBadPixel[TempLoop][0] >> vOverWriteBadPixel[TempLoop][1];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
					TempArchive >> vMeasuredDetectorGain[TempLoop] >> vMeasuredDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffset[TempLoop][0] >> vDetectorEdgeAdjustOffset[TempLoop][1];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfSourceStrengthValues21; TempLoop++)
					TempArchive >> vSourceStrengthInitialValues[TempLoop];
			}
		}  //end archive version 157
		else
		if (vArchiveVersionNumber > 155) 
		{//version 156
			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions
				>> vDiverterMargin
				>> vStopRunningOnUnConfirmedEject
				>> vMaxImageIntensityAdjust
				>> vShapeHandlingProcessIfAtSides
				>> vDriftCompensationVeryFastThreshold
				>> vDriftCompensationVeryFastAdjust
				>> vAdjustAllAreaOfHistogram
				>> vNumberOfQuickLearnImagesToTake
				>> vQuickLearnNumberOfStructureDeltas
				>> vPixelsPerDetector
				>> vScanTracCustomerName

				>> vAutoImproveNumberToAddToLearn
				>> vNumberOfDaysToKeepLogs
				>> vAutoImproveNumberToAddToLearn
				>> vUsePipeFlowInput
				>> vUseDownStreamSignalThatStopsConveyor
				>> vLastSimulatedEncoderRateSet
				>> vNumberOfPixelAtBottomToIgnoreInCalibration

				>> vDetectorsRunDuration
				>> vScanTracRunDuration
				>> vSupportContractYearExpires
				>> vSpareInt4
				>> vSpareInt5
				>> vTimeXRaysWereOnLast

				>> vScanTracProductionReportDirectory
				>> vScanTracEventLogDirectory
				>> vScanTracImagesDirectory
				>> vScanTracCalibrationImagesDirectory
				>> vScanTracSerialLogsDirectory
				>> vScanTracScreenShotDirectory

				>> vPixelsPerUnitForPoint4mmMode
				>> vDistanceFromSeamerObjectToScanTracTrigger

				>> vSpareWasHeadOffsetFromIndexPulse[cSeamerSamplingType]
				>> vUseXofYAlarmOutputForUnconfirmedEject
				>> vStaggerSampling
				>> vShowIndexErrors
				>> vSpareWasHeadOffsetFromIndexPulse[0]
				>> vShowSampleAllButton
				>> vNumberOfHeadsToMonitor[cRandomSamplingType]
				>> vDetectorsAtBottomToIgnoreDuringCalibration
				>> vSpareDWORD4
				>> vSpareDWORD5
				>> vSpareDWORD6

				>> vCertifiedSetupPassword
				>> vAdvancedSetupPassword
				>> vIPAddressString
				>> vIPSubNetMaskString
				>> vIPDefaultGatewayString
				>> vScanTracRejectedImagesDirectory
				>> vOperatorPassword
				>> vMaximumCPUTemperature
				>> vSupportContractMonthExpires
				>> vSpareDouble3
				>> vAutoProductionReportHour2
				>> vAutoProductionReportHour3
				>> vIPDHCPEnabled
				>> vShowSensitivityDisplays
				>> vSaveNextSourceStrengthValuesAsInitial
				>> vSamplePassword
				>> vOperationsGuardianServer
				>> vSpareString3
				>> vQAPassword
				>> vProductSimpleSetupPassword
				>> vSupportContractDayExpires
				>> vSupportContractType
				>> vNumberOfCheckWeighOffsetSamples
				>> vNumberOfHeadsToMonitor[cSeamerSamplingType]
				>> vRFIDEASBadgeReaderCommPort
				>> vProductSlipPercentValue
				>> vProductSetupPassword
				>> vResetCountsPassword
				>> vSystemSetupResetPassword
				>> vInspxPassword
				>> vSpareWasHeadOffsetFromIndexPulse[3]
				>> vSaveAllImagesToRejectsFolder
				>> vGlobalUseCommaForDecimalPoint
				>> vSpareStringSC4
				>> vSpareStringSC5
				>> vSpareDoubleCD4
				>> vSpareDoubleCD5
				>> vEncoderRateTolerance
				>> vEncoderDividerForPoint4mmMode
				>> vBeltPositionDistancetoEjectorResynchronizationSensor
				>> vSpareWord4
				>> vSpareWord5
				>> vTimeLastMeasuredSourceStrength
				>> vSpareCTimeCD1
				>> vSpareCTimeCD2
				>> vSpareCTimeCD3
				>> vSpareCTimeCD4
				>> vUse101VACFor10SecondsShutDownLimit
				>> vUseExternalInterlock
				>> vEnableOperationsGuardian
				>> vAutoStop
				>> vDetectedValidGuardMasterDevice
				>> vEnableModBusTCPIPInterface
				>> vIgnoreCheyneyInterlockStatus
				>> vSpareWordCD6
				>> vSpareWordCD7
				>> vSpareWordCD8
				>> vSpareWordCD9
				>> vSpareStringSC6
				>> vReferenceContainersSideToSide
				>> vStopAfterAnInspectionRejects
				>> vResynchronizeEjectors
				>> vApplyRetriggerLockoutToResynchronizingSensors
				>> vSpareByteCD4
				>> vSpareDoubleCD6
				>> vSpareCTimeCD5;

			if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0; TempLoop < cOldNumberOfEjectorsInStream6; TempLoop++)
					TempArchive	>> vSystemEjectorDistance[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cOldNumberOfEjectorsInStream6; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0] >> vDetectorEdgeAdjustGain[TempLoop][1];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
					TempArchive >> vOverWriteBadPixel[TempLoop][0] >> vOverWriteBadPixel[TempLoop][1];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
					TempArchive >> vMeasuredDetectorGain[TempLoop] >> vMeasuredDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffset[TempLoop][0] >> vDetectorEdgeAdjustOffset[TempLoop][1];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfSourceStrengthValues21; TempLoop++)
					TempArchive >> vSourceStrengthInitialValues[TempLoop];
			}
		}  //end archive version 156
		else
		if (vArchiveVersionNumber > 154) 
		{//version 155
			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions
				>> vDiverterMargin
				>> vStopRunningOnUnConfirmedEject
				>> vMaxImageIntensityAdjust
				>> vShapeHandlingProcessIfAtSides
				>> vDriftCompensationVeryFastThreshold
				>> vDriftCompensationVeryFastAdjust
				>> vAdjustAllAreaOfHistogram
				>> vNumberOfQuickLearnImagesToTake
				>> vQuickLearnNumberOfStructureDeltas
				>> vPixelsPerDetector
				>> vScanTracCustomerName

				>> vAutoImproveNumberToAddToLearn
				>> vNumberOfDaysToKeepLogs
				>> vAutoImproveNumberToAddToLearn
				>> vUsePipeFlowInput
				>> vUseDownStreamSignalThatStopsConveyor
				>> vLastSimulatedEncoderRateSet
				>> vNumberOfPixelAtBottomToIgnoreInCalibration

				>> vDetectorsRunDuration
				>> vScanTracRunDuration
				>> vSupportContractYearExpires
				>> vSpareInt4
				>> vSpareInt5
				>> vTimeXRaysWereOnLast

				>> vScanTracProductionReportDirectory
				>> vScanTracEventLogDirectory
				>> vScanTracImagesDirectory
				>> vScanTracCalibrationImagesDirectory
				>> vScanTracSerialLogsDirectory
				>> vScanTracScreenShotDirectory

				>> vPixelsPerUnitForPoint4mmMode
				>> vDistanceFromSeamerObjectToScanTracTrigger

				>> vSpareWasHeadOffsetFromIndexPulse[cSeamerSamplingType]
				>> vUseXofYAlarmOutputForUnconfirmedEject
				>> vStaggerSampling
				>> vShowIndexErrors
				>> vSpareWasHeadOffsetFromIndexPulse[0]
				>> vShowSampleAllButton
				>> vNumberOfHeadsToMonitor[cRandomSamplingType]
				>> vDetectorsAtBottomToIgnoreDuringCalibration
				>> vSpareDWORD4
				>> vSpareDWORD5
				>> vSpareDWORD6

				>> vCertifiedSetupPassword
				>> vAdvancedSetupPassword
				>> vIPAddressString
				>> vIPSubNetMaskString
				>> vIPDefaultGatewayString
				>> vScanTracRejectedImagesDirectory
				>> vOperatorPassword
				>> vMaximumCPUTemperature
				>> vSupportContractMonthExpires
				>> vSpareDouble3
				>> vAutoProductionReportHour2
				>> vAutoProductionReportHour3
				>> vIPDHCPEnabled
				>> vShowSensitivityDisplays
				>> vSaveNextSourceStrengthValuesAsInitial
				>> vSamplePassword
				>> vOperationsGuardianServer
				>> vSpareString3
				>> vQAPassword
				>> vProductSimpleSetupPassword
				>> vSupportContractDayExpires
				>> vSupportContractType
				>> vNumberOfCheckWeighOffsetSamples
				>> vNumberOfHeadsToMonitor[cSeamerSamplingType]
				>> vRFIDEASBadgeReaderCommPort
				>> vProductSlipPercentValue
				>> vProductSetupPassword
				>> vResetCountsPassword
				>> vSystemSetupResetPassword
				>> vInspxPassword
				>> vSpareWasHeadOffsetFromIndexPulse[3]
				>> vSaveAllImagesToRejectsFolder
				>> vGlobalUseCommaForDecimalPoint
				>> vSpareStringSC4
				>> vSpareStringSC5
				>> vSpareDoubleCD4
				>> vSpareDoubleCD5
				>> vEncoderRateTolerance
				>> vEncoderDividerForPoint4mmMode
				>> vBeltPositionDistancetoEjectorResynchronizationSensor
				>> vSpareWord4
				>> vSpareWord5
				>> vTimeLastMeasuredSourceStrength
				>> vSpareCTimeCD1
				>> vSpareCTimeCD2
				>> vSpareCTimeCD3
				>> vSpareCTimeCD4
				>> vUse101VACFor10SecondsShutDownLimit
				>> vUseExternalInterlock
				>> vEnableOperationsGuardian
				>> vAutoStop
				>> vDetectedValidGuardMasterDevice
				>> vEnableModBusTCPIPInterface
				>> vIgnoreCheyneyInterlockStatus
				>> vSpareWordCD6
				>> vSpareWordCD7
				>> vSpareWordCD8
				>> vSpareWordCD9
				>> vSpareStringSC6;

			if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0] >> vDetectorEdgeAdjustGain[TempLoop][1];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
					TempArchive >> vOverWriteBadPixel[TempLoop][0] >> vOverWriteBadPixel[TempLoop][1];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
					TempArchive >> vMeasuredDetectorGain[TempLoop] >> vMeasuredDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffset[TempLoop][0] >> vDetectorEdgeAdjustOffset[TempLoop][1];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfSourceStrengthValues21; TempLoop++)
					TempArchive >> vSourceStrengthInitialValues[TempLoop];
			}
		}  //end archive version 155
		else
		if (vArchiveVersionNumber > 153) 
		{//version 154
			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions
				>> vDiverterMargin
				>> vStopRunningOnUnConfirmedEject
				>> vMaxImageIntensityAdjust
				>> vShapeHandlingProcessIfAtSides
				>> vDriftCompensationVeryFastThreshold
				>> vDriftCompensationVeryFastAdjust
				>> vAdjustAllAreaOfHistogram
				>> vNumberOfQuickLearnImagesToTake
				>> vQuickLearnNumberOfStructureDeltas
				>> vPixelsPerDetector
				>> vScanTracCustomerName

				>> vAutoImproveNumberToAddToLearn
				>> vNumberOfDaysToKeepLogs
				>> vAutoImproveNumberToAddToLearn
				>> vUsePipeFlowInput
				>> vUseDownStreamSignalThatStopsConveyor
				>> vLastSimulatedEncoderRateSet
				>> vNumberOfPixelAtBottomToIgnoreInCalibration

				>> vDetectorsRunDuration
				>> vScanTracRunDuration
				>> vSupportContractYearExpires
				>> vSpareInt4
				>> vSpareInt5
				>> vTimeXRaysWereOnLast

				>> vScanTracProductionReportDirectory
				>> vScanTracEventLogDirectory
				>> vScanTracImagesDirectory
				>> vScanTracCalibrationImagesDirectory
				>> vScanTracSerialLogsDirectory
				>> vScanTracScreenShotDirectory

				>> vPixelsPerUnitForPoint4mmMode
				>> vDistanceFromSeamerObjectToScanTracTrigger

				>> vSpareWasHeadOffsetFromIndexPulse[cSeamerSamplingType]
				>> vUseXofYAlarmOutputForUnconfirmedEject
				>> vStaggerSampling
				>> vShowIndexErrors
				>> vSpareWasHeadOffsetFromIndexPulse[0]
				>> vShowSampleAllButton
				>> vNumberOfHeadsToMonitor[cRandomSamplingType]
				>> vDetectorsAtBottomToIgnoreDuringCalibration
				>> vSpareDWORD4
				>> vSpareDWORD5
				>> vSpareDWORD6

				>> vCertifiedSetupPassword
				>> vAdvancedSetupPassword
				>> vIPAddressString
				>> vIPSubNetMaskString
				>> vIPDefaultGatewayString
				>> vScanTracRejectedImagesDirectory
				>> vOperatorPassword
				>> vMaximumCPUTemperature
				>> vSupportContractMonthExpires
				>> vSpareDouble3
				>> vAutoProductionReportHour2
				>> vAutoProductionReportHour3
				>> vIPDHCPEnabled
				>> vShowSensitivityDisplays
				>> vSaveNextSourceStrengthValuesAsInitial
				>> vSamplePassword
				>> vOperationsGuardianServer
				>> vSpareString3
				>> vQAPassword
				>> vProductSimpleSetupPassword
				>> vSupportContractDayExpires
				>> vSupportContractType
				>> vNumberOfCheckWeighOffsetSamples
				>> vNumberOfHeadsToMonitor[cSeamerSamplingType]
				>> vRFIDEASBadgeReaderCommPort
				>> vProductSlipPercentValue
				>> vProductSetupPassword
				>> vResetCountsPassword
				>> vSystemSetupResetPassword
				>> vInspxPassword
				>> vSpareWasHeadOffsetFromIndexPulse[3]
				>> vSaveAllImagesToRejectsFolder
				>> vGlobalUseCommaForDecimalPoint
				>> vSpareStringSC4
				>> vSpareStringSC5
				>> vSpareDoubleCD4
				>> vSpareDoubleCD5
				>> vEncoderRateTolerance
				>> vEncoderDividerForPoint4mmMode
				>> vBeltPositionDistancetoEjectorResynchronizationSensor
				>> vSpareWord4
				>> vSpareWord5;

			if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0] >> vDetectorEdgeAdjustGain[TempLoop][1];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
					TempArchive >> vOverWriteBadPixel[TempLoop][0] >> vOverWriteBadPixel[TempLoop][1];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
					TempArchive >> vMeasuredDetectorGain[TempLoop] >> vMeasuredDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffset[TempLoop][0] >> vDetectorEdgeAdjustOffset[TempLoop][1];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfSourceStrengthValues21; TempLoop++)
					TempArchive >> vSourceStrengthInitialValues[TempLoop];
			}
		}  //end archive version 154
		else
		if (vArchiveVersionNumber > 152) 
		{//version 153
			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions
				>> vDiverterMargin
				>> vStopRunningOnUnConfirmedEject
				>> vMaxImageIntensityAdjust
				>> vShapeHandlingProcessIfAtSides
				>> vDriftCompensationVeryFastThreshold
				>> vDriftCompensationVeryFastAdjust
				>> vAdjustAllAreaOfHistogram
				>> vNumberOfQuickLearnImagesToTake
				>> vQuickLearnNumberOfStructureDeltas
				>> vPixelsPerDetector
				>> vScanTracCustomerName

				>> vAutoImproveNumberToAddToLearn
				>> vNumberOfDaysToKeepLogs
				>> vAutoImproveNumberToAddToLearn
				>> vUsePipeFlowInput
				>> vUseDownStreamSignalThatStopsConveyor
				>> vLastSimulatedEncoderRateSet
				>> vNumberOfPixelAtBottomToIgnoreInCalibration

				>> vDetectorsRunDuration
				>> vScanTracRunDuration
				>> vSupportContractYearExpires
				>> vSpareInt4
				>> vSpareInt5
				>> vTimeXRaysWereOnLast

				>> vScanTracProductionReportDirectory
				>> vScanTracEventLogDirectory
				>> vScanTracImagesDirectory
				>> vScanTracCalibrationImagesDirectory
				>> vScanTracSerialLogsDirectory
				>> vScanTracScreenShotDirectory

				>> vPixelsPerUnitForPoint4mmMode
				>> vDistanceFromSeamerObjectToScanTracTrigger

				>> vSpareWasHeadOffsetFromIndexPulse[cSeamerSamplingType]
				>> vUseXofYAlarmOutputForUnconfirmedEject
				>> vStaggerSampling
				>> vShowIndexErrors
				>> vSpareWasHeadOffsetFromIndexPulse[0]
				>> vShowSampleAllButton
				>> vNumberOfHeadsToMonitor[cRandomSamplingType]
				>> vDetectorsAtBottomToIgnoreDuringCalibration
				>> vSpareDWORD4
				>> vSpareDWORD5
				>> vSpareDWORD6

				>> vCertifiedSetupPassword
				>> vAdvancedSetupPassword
				>> vIPAddressString
				>> vIPSubNetMaskString
				>> vIPDefaultGatewayString
				>> vScanTracRejectedImagesDirectory
				>> vOperatorPassword
				>> vMaximumCPUTemperature
				>> vSupportContractMonthExpires
				>> vSpareDouble3
				>> vAutoProductionReportHour2
				>> vAutoProductionReportHour3
				>> vIPDHCPEnabled
				>> vShowSensitivityDisplays
				>> vSaveNextSourceStrengthValuesAsInitial
				>> vSamplePassword
				>> vOperationsGuardianServer
				>> vSpareString3
				>> vQAPassword
				>> vProductSimpleSetupPassword
				>> vSupportContractDayExpires
				>> vSupportContractType
				>> vNumberOfCheckWeighOffsetSamples
				>> vNumberOfHeadsToMonitor[cSeamerSamplingType]
				>> vRFIDEASBadgeReaderCommPort
				>> vProductSlipPercentValue
				>> vProductSetupPassword
				>> vResetCountsPassword
				>> vSystemSetupResetPassword
				>> vInspxPassword
				>> vSpareWasHeadOffsetFromIndexPulse[3]
				>> vSaveAllImagesToRejectsFolder
				>> vGlobalUseCommaForDecimalPoint
				>> vSpareStringSC4
				>> vSpareStringSC5
				>> vSpareDoubleCD4
				>> vSpareDoubleCD5
				>> vEncoderRateTolerance
				>> vEncoderDividerForPoint4mmMode
				>> vBeltPositionDistancetoEjectorResynchronizationSensor
				>> vSpareWord4
				>> vSpareWord5;

			if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0] >> vDetectorEdgeAdjustGain[TempLoop][1];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vOverWriteBadPixel[TempLoop][0] >> vOverWriteBadPixel[TempLoop][1];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
					TempArchive >> vMeasuredDetectorGain[TempLoop] >> vMeasuredDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffset[TempLoop][0] >> vDetectorEdgeAdjustOffset[TempLoop][1];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfSourceStrengthValues21; TempLoop++)
					TempArchive >> vSourceStrengthInitialValues[TempLoop];
			}
		}  //end archive version 153
		else
		if (vArchiveVersionNumber == 152) 
		{//version 152
			CString TempPasswordRead("");
			CString TempSystemPasswordRead("");

			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> TempPasswordRead
				>> TempSystemPasswordRead
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions
				>> vDiverterMargin
				>> vStopRunningOnUnConfirmedEject
				>> vMaxImageIntensityAdjust
				>> vShapeHandlingProcessIfAtSides
				>> vDriftCompensationVeryFastThreshold
				>> vDriftCompensationVeryFastAdjust
				>> vAdjustAllAreaOfHistogram
				>> vNumberOfQuickLearnImagesToTake
				>> vQuickLearnNumberOfStructureDeltas
				>> vPixelsPerDetector
				>> vScanTracCustomerName

				>> vAutoImproveNumberToAddToLearn
				>> vNumberOfDaysToKeepLogs
				>> vAutoImproveNumberToAddToLearn
				>> vUsePipeFlowInput
				>> vUseDownStreamSignalThatStopsConveyor
				>> vLastSimulatedEncoderRateSet
				>> vNumberOfPixelAtBottomToIgnoreInCalibration

				>> vDetectorsRunDuration
				>> vScanTracRunDuration
				>> vSupportContractYearExpires
				>> vSpareInt4
				>> vSpareInt5
				>> vTimeXRaysWereOnLast

				>> vScanTracProductionReportDirectory
				>> vScanTracEventLogDirectory
				>> vScanTracImagesDirectory
				>> vScanTracCalibrationImagesDirectory
				>> vScanTracSerialLogsDirectory
				>> vScanTracScreenShotDirectory

				>> vPixelsPerUnitForPoint4mmMode
				>> vDistanceFromSeamerObjectToScanTracTrigger

				>> vSpareWasHeadOffsetFromIndexPulse[cSeamerSamplingType]
				>> vUseXofYAlarmOutputForUnconfirmedEject
				>> vStaggerSampling
				>> vShowIndexErrors
				>> vSpareWasHeadOffsetFromIndexPulse[0]
				>> vShowSampleAllButton
				>> vNumberOfHeadsToMonitor[cRandomSamplingType]
				>> vDetectorsAtBottomToIgnoreDuringCalibration
				>> vSpareDWORD4
				>> vSpareDWORD5
				>> vSpareDWORD6

				>> vCertifiedSetupPassword
				>> vAdvancedSetupPassword
				>> vIPAddressString
				>> vIPSubNetMaskString
				>> vIPDefaultGatewayString
				>> vScanTracRejectedImagesDirectory
				>> vOperatorPassword
				>> vMaximumCPUTemperature
				>> vSupportContractMonthExpires
				>> vSpareDouble3
				>> vAutoProductionReportHour2
				>> vAutoProductionReportHour3
				>> vIPDHCPEnabled
				>> vShowSensitivityDisplays
				>> vSaveNextSourceStrengthValuesAsInitial
				>> vSamplePassword
				>> vOperationsGuardianServer
				>> vSpareString3
				>> vQAPassword
				>> vProductSimpleSetupPassword
				>> vSupportContractDayExpires
				>> vSupportContractType
				>> vNumberOfCheckWeighOffsetSamples
				>> vNumberOfHeadsToMonitor[cSeamerSamplingType]
				>> vRFIDEASBadgeReaderCommPort
				>> vProductSlipPercentValue;

			if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vOverWriteBadPixel[TempLoop][0];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffset[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfSourceStrengthValues21; TempLoop++)
					TempArchive >> vSourceStrengthInitialValues[TempLoop];

				WORD TempPosition = TempPasswordRead.Find(',',0);
				if (TempPosition < 1000)
				{
					if (TempPosition)
						vProductSetupPassword = TempPasswordRead.Left(TempPosition);
					else
						vProductSetupPassword = "";

					WORD TempLength = TempPasswordRead.GetLength();
					if (TempLength > TempPosition + 1)
						vResetCountsPassword = TempPasswordRead.Right(TempLength - TempPosition - 1);
					else
						vResetCountsPassword = "";
				}
				else
				if (TempPasswordRead.GetLength() > 2)
					vProductSetupPassword = TempPasswordRead;

				TempPosition = TempSystemPasswordRead.Find(',',0);
				if (TempPosition < 1000)
				{
					if (TempPosition)
						vInspxPassword = TempSystemPasswordRead.Left(TempPosition);
					else
						vInspxPassword = "";

					WORD TempLength = TempSystemPasswordRead.GetLength();
					if (TempLength > TempPosition + 1)
						vSystemSetupResetPassword = TempSystemPasswordRead.Right(TempLength - TempPosition - 1);
					else
						vSystemSetupResetPassword = "";
				}
				else
				if (TempSystemPasswordRead.GetLength() > 2)
					vInspxPassword = TempSystemPasswordRead;

				if (vSystemSetupResetPassword.GetLength() < 8)
					vSystemSetupResetPassword = "95038603";


				if (vArchiveVersionNumber == 145) 
				if ((!vDetectorsRunDuration) && (!vScanTracRunDuration) && (vXRaySourceRunDuration))
				{
					vDetectorsRunDuration = vXRaySourceRunDuration;
					vScanTracRunDuration = vXRaySourceRunDuration;
				}
			}
		}  //end archive version 152
		else
		if (vArchiveVersionNumber > 150) 
		{//version 151
			CString TempPasswordRead("");
			CString TempSystemPasswordRead("");

			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> TempPasswordRead
				>> TempSystemPasswordRead
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions
				>> vDiverterMargin
				>> vStopRunningOnUnConfirmedEject
				>> vMaxImageIntensityAdjust
				>> vShapeHandlingProcessIfAtSides
				>> vDriftCompensationVeryFastThreshold
				>> vDriftCompensationVeryFastAdjust
				>> vAdjustAllAreaOfHistogram
				>> vNumberOfQuickLearnImagesToTake
				>> vQuickLearnNumberOfStructureDeltas
				>> vPixelsPerDetector
				>> vScanTracCustomerName

				>> vAutoImproveNumberToAddToLearn
				>> vNumberOfDaysToKeepLogs
				>> vAutoImproveNumberToAddToLearn
				>> vUsePipeFlowInput
				>> vUseDownStreamSignalThatStopsConveyor
				>> vLastSimulatedEncoderRateSet
				>> vNumberOfPixelAtBottomToIgnoreInCalibration

				>> vDetectorsRunDuration
				>> vScanTracRunDuration
				>> vSupportContractYearExpires
				>> vSpareInt4
				>> vSpareInt5
				>> vTimeXRaysWereOnLast

				>> vScanTracProductionReportDirectory
				>> vScanTracEventLogDirectory
				>> vScanTracImagesDirectory
				>> vScanTracCalibrationImagesDirectory
				>> vScanTracSerialLogsDirectory
				>> vScanTracScreenShotDirectory

				>> vPixelsPerUnitForPoint4mmMode
				>> vDistanceFromSeamerObjectToScanTracTrigger

				>> vSpareWasHeadOffsetFromIndexPulse[cSeamerSamplingType]
				>> vUseXofYAlarmOutputForUnconfirmedEject
				>> vStaggerSampling
				>> vShowIndexErrors
				>> vSpareWasHeadOffsetFromIndexPulse[0]
				>> vShowSampleAllButton
				>> vNumberOfHeadsToMonitor[cRandomSamplingType]
				>> vDetectorsAtBottomToIgnoreDuringCalibration
				>> vSpareDWORD4
				>> vSpareDWORD5
				>> vSpareDWORD6

				>> vCertifiedSetupPassword
				>> vAdvancedSetupPassword
				>> vIPAddressString
				>> vIPSubNetMaskString
				>> vIPDefaultGatewayString
				>> vScanTracRejectedImagesDirectory
				>> vOperatorPassword
				>> vMaximumCPUTemperature
				>> vSupportContractMonthExpires
				>> vSpareDouble3
				>> vAutoProductionReportHour2
				>> vAutoProductionReportHour3
				>> vIPDHCPEnabled
				>> vShowSensitivityDisplays
				>> vSaveNextSourceStrengthValuesAsInitial
				>> vSamplePassword
				>> vOperationsGuardianServer
				>> vSpareString3
				>> vQAPassword
				>> vProductSimpleSetupPassword
				>> vSupportContractDayExpires
				>> vSupportContractType
				>> vNumberOfCheckWeighOffsetSamples
				>> vNumberOfHeadsToMonitor[cSeamerSamplingType]
				>> vRFIDEASBadgeReaderCommPort
				>> vProductSlipPercentValue;

			if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vOverWriteBadPixel[TempLoop][0];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffset[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfSourceStrengthValues21; TempLoop++)
					TempArchive >> vSourceStrengthInitialValues[TempLoop];

				WORD TempPosition = TempPasswordRead.Find(',',0);
				if (TempPosition < 1000)
				{
					if (TempPosition)
						vProductSetupPassword = TempPasswordRead.Left(TempPosition);
					else
						vProductSetupPassword = "";

					WORD TempLength = TempPasswordRead.GetLength();
					if (TempLength > TempPosition + 1)
						vResetCountsPassword = TempPasswordRead.Right(TempLength - TempPosition - 1);
					else
						vResetCountsPassword = "";
				}
				else
				if (TempPasswordRead.GetLength() > 2)
					vProductSetupPassword = TempPasswordRead;

				TempPosition = TempSystemPasswordRead.Find(',',0);
				if (TempPosition < 1000)
				{
					if (TempPosition)
						vInspxPassword = TempSystemPasswordRead.Left(TempPosition);
					else
						vInspxPassword = "";

					WORD TempLength = TempSystemPasswordRead.GetLength();
					if (TempLength > TempPosition + 1)
						vSystemSetupResetPassword = TempSystemPasswordRead.Right(TempLength - TempPosition - 1);
					else
						vSystemSetupResetPassword = "";
				}
				else
				if (TempSystemPasswordRead.GetLength() > 2)
					vInspxPassword = TempSystemPasswordRead;

				if (vSystemSetupResetPassword.GetLength() < 8)
					vSystemSetupResetPassword = "95038603";


				if (vArchiveVersionNumber == 145) 
				if ((!vDetectorsRunDuration) && (!vScanTracRunDuration) && (vXRaySourceRunDuration))
				{
					vDetectorsRunDuration = vXRaySourceRunDuration;
					vScanTracRunDuration = vXRaySourceRunDuration;
				}
			}
		}  //end archive version 151
		else
		if (vArchiveVersionNumber > 149) 
		{//version 150
			CString TempPasswordRead("");
			CString TempSystemPasswordRead("");

			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> TempPasswordRead
				>> TempSystemPasswordRead
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions
				>> vDiverterMargin
				>> vStopRunningOnUnConfirmedEject
				>> vMaxImageIntensityAdjust
				>> vShapeHandlingProcessIfAtSides
				>> vDriftCompensationVeryFastThreshold
				>> vDriftCompensationVeryFastAdjust
				>> vAdjustAllAreaOfHistogram
				>> vNumberOfQuickLearnImagesToTake
				>> vQuickLearnNumberOfStructureDeltas
				>> vPixelsPerDetector
				>> vScanTracCustomerName

				>> vAutoImproveNumberToAddToLearn
				>> vNumberOfDaysToKeepLogs
				>> vAutoImproveNumberToAddToLearn
				>> vUsePipeFlowInput
				>> vUseDownStreamSignalThatStopsConveyor
				>> vLastSimulatedEncoderRateSet
				>> vNumberOfPixelAtBottomToIgnoreInCalibration

				>> vDetectorsRunDuration
				>> vScanTracRunDuration
				>> vSupportContractYearExpires
				>> vSpareInt4
				>> vSpareInt5
				>> vTimeXRaysWereOnLast

				>> vScanTracProductionReportDirectory
				>> vScanTracEventLogDirectory
				>> vScanTracImagesDirectory
				>> vScanTracCalibrationImagesDirectory
				>> vScanTracSerialLogsDirectory
				>> vScanTracScreenShotDirectory

				>> vPixelsPerUnitForPoint4mmMode
				>> vDistanceFromSeamerObjectToScanTracTrigger

				>> vSpareWasHeadOffsetFromIndexPulse[cSeamerSamplingType]
				>> vUseXofYAlarmOutputForUnconfirmedEject
				>> vStaggerSampling
				>> vShowIndexErrors
				>> vSpareWasHeadOffsetFromIndexPulse[0]
				>> vShowSampleAllButton
				>> vNumberOfHeadsToMonitor[cRandomSamplingType]
				>> vDetectorsAtBottomToIgnoreDuringCalibration
				>> vSpareDWORD4
				>> vSpareDWORD5
				>> vSpareDWORD6

				>> vCertifiedSetupPassword
				>> vAdvancedSetupPassword
				>> vIPAddressString
				>> vIPSubNetMaskString
				>> vIPDefaultGatewayString
				>> vScanTracRejectedImagesDirectory
				>> vOperatorPassword
				>> vMaximumCPUTemperature
				>> vSupportContractMonthExpires
				>> vSpareDouble3
				>> vAutoProductionReportHour2
				>> vAutoProductionReportHour3
				>> vIPDHCPEnabled
				>> vShowSensitivityDisplays
				>> vSaveNextSourceStrengthValuesAsInitial
				>> vSamplePassword
				>> vOperationsGuardianServer
				>> vSpareString3
				>> vQAPassword
				>> vProductSimpleSetupPassword
				>> vSupportContractDayExpires
				>> vSupportContractType
				>> vNumberOfCheckWeighOffsetSamples
				>> vNumberOfHeadsToMonitor[cSeamerSamplingType]
				>> vRFIDEASBadgeReaderCommPort
				>> vProductSlipPercentValue;

			if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vOverWriteBadPixel[TempLoop][0];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffset[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfSourceStrengthValues21; TempLoop++)
					TempArchive >> vSourceStrengthInitialValues[TempLoop];

				WORD TempPosition = TempPasswordRead.Find(',',0);
				if (TempPosition < 1000)
				{
					if (TempPosition)
						vProductSetupPassword = TempPasswordRead.Left(TempPosition);
					else
						vProductSetupPassword = "";

					WORD TempLength = TempPasswordRead.GetLength();
					if (TempLength > TempPosition + 1)
						vResetCountsPassword = TempPasswordRead.Right(TempLength - TempPosition - 1);
					else
						vResetCountsPassword = "";
				}
				else
				if (TempPasswordRead.GetLength() > 2)
					vProductSetupPassword = TempPasswordRead;

				TempPosition = TempSystemPasswordRead.Find(',',0);
				if (TempPosition < 1000)
				{
					if (TempPosition)
						vInspxPassword = TempSystemPasswordRead.Left(TempPosition);
					else
						vInspxPassword = "";

					WORD TempLength = TempSystemPasswordRead.GetLength();
					if (TempLength > TempPosition + 1)
						vSystemSetupResetPassword = TempSystemPasswordRead.Right(TempLength - TempPosition - 1);
					else
						vSystemSetupResetPassword = "";
				}
				else
				if (TempSystemPasswordRead.GetLength() > 2)
					vInspxPassword = TempSystemPasswordRead;

				if (vSystemSetupResetPassword.GetLength() < 8)
					vSystemSetupResetPassword = "95038603";


				if (vArchiveVersionNumber == 145) 
				if ((!vDetectorsRunDuration) && (!vScanTracRunDuration) && (vXRaySourceRunDuration))
				{
					vDetectorsRunDuration = vXRaySourceRunDuration;
					vScanTracRunDuration = vXRaySourceRunDuration;
				}
			}
		}  //end archive version 150
		else
		if (vArchiveVersionNumber > 147) 
		{//version 148-149
			CString TempPasswordRead("");
			CString TempSystemPasswordRead("");

			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> TempPasswordRead
				>> TempSystemPasswordRead
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions
				>> vDiverterMargin
				>> vStopRunningOnUnConfirmedEject
				>> vMaxImageIntensityAdjust
				>> vShapeHandlingProcessIfAtSides
				>> vDriftCompensationVeryFastThreshold
				>> vDriftCompensationVeryFastAdjust
				>> vAdjustAllAreaOfHistogram
				>> vNumberOfQuickLearnImagesToTake
				>> vQuickLearnNumberOfStructureDeltas
				>> vPixelsPerDetector
				>> vScanTracCustomerName

				>> vAutoImproveNumberToAddToLearn
				>> vNumberOfDaysToKeepLogs
				>> vAutoImproveNumberToAddToLearn
				>> vUsePipeFlowInput
				>> vUseDownStreamSignalThatStopsConveyor
				>> vLastSimulatedEncoderRateSet
				>> vNumberOfPixelAtBottomToIgnoreInCalibration

				>> vDetectorsRunDuration
				>> vScanTracRunDuration
				>> vSupportContractYearExpires
				>> vSpareInt4
				>> vSpareInt5
				>> vTimeXRaysWereOnLast

				>> vScanTracProductionReportDirectory
				>> vScanTracEventLogDirectory
				>> vScanTracImagesDirectory
				>> vScanTracCalibrationImagesDirectory
				>> vScanTracSerialLogsDirectory
				>> vScanTracScreenShotDirectory

				>> vPixelsPerUnitForPoint4mmMode
				>> vDistanceFromSeamerObjectToScanTracTrigger

				>> vSpareWasHeadOffsetFromIndexPulse[cSeamerSamplingType]
				>> vUseXofYAlarmOutputForUnconfirmedEject
				>> vStaggerSampling
				>> vShowIndexErrors
				>> vSpareWasHeadOffsetFromIndexPulse[0]
				>> vShowSampleAllButton
				>> vNumberOfHeadsToMonitor[cRandomSamplingType]
				>> vDetectorsAtBottomToIgnoreDuringCalibration
				>> vSpareDWORD4
				>> vSpareDWORD5
				>> vSpareDWORD6

				>> vCertifiedSetupPassword
				>> vAdvancedSetupPassword
				>> vIPAddressString
				>> vIPSubNetMaskString
				>> vIPDefaultGatewayString
				>> vScanTracRejectedImagesDirectory
				>> vOperatorPassword
				>> vMaximumCPUTemperature
				>> vSupportContractMonthExpires
				>> vSpareDouble3
				>> vAutoProductionReportHour2
				>> vAutoProductionReportHour3
				>> vIPDHCPEnabled
				>> vShowSensitivityDisplays;

			if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vOverWriteBadPixel[TempLoop][0];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffset[TempLoop][0];

				WORD TempPosition = TempPasswordRead.Find(',',0);
				if (TempPosition < 1000)
				{
					if (TempPosition)
						vProductSetupPassword = TempPasswordRead.Left(TempPosition);
					else
						vProductSetupPassword = "";

					WORD TempLength = TempPasswordRead.GetLength();
					if (TempLength > TempPosition + 1)
						vResetCountsPassword = TempPasswordRead.Right(TempLength - TempPosition - 1);
					else
						vResetCountsPassword = "";
				}
				else
				if (TempPasswordRead.GetLength() > 2)
					vProductSetupPassword = TempPasswordRead;

				TempPosition = TempSystemPasswordRead.Find(',',0);
				if (TempPosition < 1000)
				{
					if (TempPosition)
						vInspxPassword = TempSystemPasswordRead.Left(TempPosition);
					else
						vInspxPassword = "";

					WORD TempLength = TempSystemPasswordRead.GetLength();
					if (TempLength > TempPosition + 1)
						vSystemSetupResetPassword = TempSystemPasswordRead.Right(TempLength - TempPosition - 1);
					else
						vSystemSetupResetPassword = "";
				}
				else
				if (TempSystemPasswordRead.GetLength() > 2)
					vInspxPassword = TempSystemPasswordRead;

				if (vSystemSetupResetPassword.GetLength() < 8)
					vSystemSetupResetPassword = "95038603";


				if (vArchiveVersionNumber == 145) 
				if ((!vDetectorsRunDuration) && (!vScanTracRunDuration) && (vXRaySourceRunDuration))
				{
					vDetectorsRunDuration = vXRaySourceRunDuration;
					vScanTracRunDuration = vXRaySourceRunDuration;
				}
			}
		}  //end archive version 148-149
		else
		if (vArchiveVersionNumber > 146) 
		{//version 147
			CString TempPasswordRead("");
			CString TempSystemPasswordRead("");

			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> TempPasswordRead
				>> TempSystemPasswordRead
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions
				>> vDiverterMargin
				>> vStopRunningOnUnConfirmedEject
				>> vMaxImageIntensityAdjust
				>> vShapeHandlingProcessIfAtSides
				>> vDriftCompensationVeryFastThreshold
				>> vDriftCompensationVeryFastAdjust
				>> vAdjustAllAreaOfHistogram
				>> vNumberOfQuickLearnImagesToTake
				>> vQuickLearnNumberOfStructureDeltas
				>> vPixelsPerDetector
				>> vScanTracCustomerName

				>> vAutoImproveNumberToAddToLearn
				>> vNumberOfDaysToKeepLogs
				>> vAutoImproveNumberToAddToLearn
				>> vUsePipeFlowInput
				>> vUseDownStreamSignalThatStopsConveyor
				>> vLastSimulatedEncoderRateSet
				>> vNumberOfPixelAtBottomToIgnoreInCalibration

				>> vDetectorsRunDuration
				>> vScanTracRunDuration
				>> vSupportContractYearExpires
				>> vSpareInt4
				>> vSpareInt5
				>> vTimeXRaysWereOnLast

				>> vScanTracProductionReportDirectory
				>> vScanTracEventLogDirectory
				>> vScanTracImagesDirectory
				>> vScanTracCalibrationImagesDirectory
				>> vScanTracSerialLogsDirectory
				>> vScanTracScreenShotDirectory

				>> vPixelsPerUnitForPoint4mmMode
				>> vDistanceFromSeamerObjectToScanTracTrigger

				>> vSpareWasHeadOffsetFromIndexPulse[cSeamerSamplingType]
				>> vUseXofYAlarmOutputForUnconfirmedEject
				>> vStaggerSampling
				>> vShowIndexErrors
				>> vSpareWasHeadOffsetFromIndexPulse[0]
				>> vShowSampleAllButton
				>> vNumberOfHeadsToMonitor[cRandomSamplingType]
				>> vDetectorsAtBottomToIgnoreDuringCalibration
				>> vSpareDWORD4
				>> vSpareDWORD5
				>> vSpareDWORD6

				>> vCertifiedSetupPassword
				>> vAdvancedSetupPassword
				>> vIPAddressString
				>> vIPSubNetMaskString
				>> vIPDefaultGatewayString
				>> vScanTracRejectedImagesDirectory
				>> vOperatorPassword
				>> vMaximumCPUTemperature
				>> vSupportContractMonthExpires
				>> vSpareDouble3
				>> vAutoProductionReportHour2
				>> vAutoProductionReportHour3
				>> vIPDHCPEnabled
				>> vShowSensitivityDisplays;

			if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffsetInteger[TempLoop];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffset[TempLoop][0];

				WORD TempPosition = TempPasswordRead.Find(',',0);
				if (TempPosition < 1000)
				{
					if (TempPosition)
						vProductSetupPassword = TempPasswordRead.Left(TempPosition);
					else
						vProductSetupPassword = "";

					WORD TempLength = TempPasswordRead.GetLength();
					if (TempLength > TempPosition + 1)
						vResetCountsPassword = TempPasswordRead.Right(TempLength - TempPosition - 1);
					else
						vResetCountsPassword = "";
				}
				else
				if (TempPasswordRead.GetLength() > 2)
					vProductSetupPassword = TempPasswordRead;

				TempPosition = TempSystemPasswordRead.Find(',',0);
				if (TempPosition < 1000)
				{
					if (TempPosition)
						vInspxPassword = TempSystemPasswordRead.Left(TempPosition);
					else
						vInspxPassword = "";

					WORD TempLength = TempSystemPasswordRead.GetLength();
					if (TempLength > TempPosition + 1)
						vSystemSetupResetPassword = TempSystemPasswordRead.Right(TempLength - TempPosition - 1);
					else
						vSystemSetupResetPassword = "";
				}
				else
				if (TempSystemPasswordRead.GetLength() > 2)
					vInspxPassword = TempSystemPasswordRead;

				if (vSystemSetupResetPassword.GetLength() < 8)
					vSystemSetupResetPassword = "95038603";


				if (vArchiveVersionNumber == 145) 
				if ((!vDetectorsRunDuration) && (!vScanTracRunDuration) && (vXRaySourceRunDuration))
				{
					vDetectorsRunDuration = vXRaySourceRunDuration;
					vScanTracRunDuration = vXRaySourceRunDuration;
				}
			}
		}  //end archive version 147
		else
		if (vArchiveVersionNumber > 144) 
		{//version 145, 146
			CString TempPasswordRead("");
			CString TempSystemPasswordRead("");

			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> TempPasswordRead
				>> TempSystemPasswordRead
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions
				>> vDiverterMargin
				>> vStopRunningOnUnConfirmedEject
				>> vMaxImageIntensityAdjust
				>> vShapeHandlingProcessIfAtSides
				>> vDriftCompensationVeryFastThreshold
				>> vDriftCompensationVeryFastAdjust
				>> vAdjustAllAreaOfHistogram
				>> vNumberOfQuickLearnImagesToTake
				>> vQuickLearnNumberOfStructureDeltas
				>> vPixelsPerDetector
				>> vScanTracCustomerName

				>> vAutoImproveNumberToAddToLearn
				>> vNumberOfDaysToKeepLogs
				>> vAutoImproveNumberToAddToLearn
				>> vUsePipeFlowInput
				>> vUseDownStreamSignalThatStopsConveyor
				>> vLastSimulatedEncoderRateSet
				>> vNumberOfPixelAtBottomToIgnoreInCalibration

				>> vDetectorsRunDuration
				>> vScanTracRunDuration
				>> vSupportContractYearExpires
				>> vSpareInt4
				>> vSpareInt5
				>> vTimeXRaysWereOnLast

				>> vScanTracProductionReportDirectory
				>> vScanTracEventLogDirectory
				>> vScanTracImagesDirectory
				>> vScanTracCalibrationImagesDirectory
				>> vScanTracSerialLogsDirectory
				>> vScanTracScreenShotDirectory

				>> vPixelsPerUnitForPoint4mmMode
				>> vDistanceFromSeamerObjectToScanTracTrigger

				>> vSpareWasHeadOffsetFromIndexPulse[cSeamerSamplingType]
				>> vUseXofYAlarmOutputForUnconfirmedEject
				>> vStaggerSampling
				>> vShowIndexErrors
				>> vSpareWasHeadOffsetFromIndexPulse[0]
				>> vShowSampleAllButton
				>> vNumberOfHeadsToMonitor[cRandomSamplingType]
				>> vDetectorsAtBottomToIgnoreDuringCalibration
				>> vSpareDWORD4
				>> vSpareDWORD5
				>> vSpareDWORD6;

			if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffsetInteger[TempLoop];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffset[TempLoop][0];

				WORD TempPosition = TempPasswordRead.Find(',',0);
				if (TempPosition < 1000)
				{
					if (TempPosition)
						vProductSetupPassword = TempPasswordRead.Left(TempPosition);
					else
						vProductSetupPassword = "";

					WORD TempLength = TempPasswordRead.GetLength();
					if (TempLength > TempPosition + 1)
						vResetCountsPassword = TempPasswordRead.Right(TempLength - TempPosition - 1);
					else
						vResetCountsPassword = "";
				}
				else
				if (TempPasswordRead.GetLength() > 2)
					vProductSetupPassword = TempPasswordRead;

				TempPosition = TempSystemPasswordRead.Find(',',0);
				if (TempPosition < 1000)
				{
					if (TempPosition)
						vInspxPassword = TempSystemPasswordRead.Left(TempPosition);
					else
						vInspxPassword = "";

					WORD TempLength = TempSystemPasswordRead.GetLength();
					if (TempLength > TempPosition + 1)
						vSystemSetupResetPassword = TempSystemPasswordRead.Right(TempLength - TempPosition - 1);
					else
						vSystemSetupResetPassword = "";
				}
				else
				if (TempSystemPasswordRead.GetLength() > 2)
					vInspxPassword = TempSystemPasswordRead;

				if (vSystemSetupResetPassword.GetLength() < 8)
					vSystemSetupResetPassword = "95038603";


				if (vArchiveVersionNumber == 145) 
				if ((!vDetectorsRunDuration) && (!vScanTracRunDuration) && (vXRaySourceRunDuration))
				{
					vDetectorsRunDuration = vXRaySourceRunDuration;
					vScanTracRunDuration = vXRaySourceRunDuration;
				}
			}
		}  //end archive version 145, 146
		else
		if (vArchiveVersionNumber == 144) 
		{//version 144
			CString TempPasswordRead("");

			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> TempPasswordRead
				>> vInspxPassword
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions
				>> vDiverterMargin
				>> vStopRunningOnUnConfirmedEject
				>> vMaxImageIntensityAdjust
				>> vShapeHandlingProcessIfAtSides
				>> vDriftCompensationVeryFastThreshold
				>> vDriftCompensationVeryFastAdjust
				>> vAdjustAllAreaOfHistogram
				>> vNumberOfQuickLearnImagesToTake
				>> vQuickLearnNumberOfStructureDeltas
				>> vPixelsPerDetector;

			//if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			vScanTracConfigurationFileType = cSingleImageScanTrac;
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffsetInteger[TempLoop];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffset[TempLoop][0];

				WORD TempPosition = TempPasswordRead.Find(',',0);
				if (TempPosition < 1000)
				{
					if (TempPosition)
						vProductSetupPassword = TempPasswordRead.Left(TempPosition);
					else
						vProductSetupPassword = "";

					WORD TempLength = TempPasswordRead.GetLength();
					if (TempLength > TempPosition + 1)
						vResetCountsPassword = TempPasswordRead.Right(TempLength - TempPosition - 1);
					else
						vResetCountsPassword = "";
				}
			}
		}  //end archive version 144
		else
		if (vArchiveVersionNumber == 143) 
		{//version 143
			CString TempPasswordRead("");

			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> TempPasswordRead
				>> vInspxPassword
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions
				>> vDiverterMargin
				>> vStopRunningOnUnConfirmedEject
				>> vMaxImageIntensityAdjust
				>> vShapeHandlingProcessIfAtSides
				>> vDriftCompensationVeryFastThreshold
				>> vDriftCompensationVeryFastAdjust
				>> vAdjustAllAreaOfHistogram
				>> vNumberOfQuickLearnImagesToTake
				>> vQuickLearnNumberOfStructureDeltas
				>> vPixelsPerDetector;

			//if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			vScanTracConfigurationFileType = cSingleImageScanTrac;
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels768; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffsetInteger[TempLoop];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}

				WORD TempPosition = TempPasswordRead.Find(',',0);
				if (TempPosition < 1000)
				{
					if (TempPosition)
						vProductSetupPassword = TempPasswordRead.Left(TempPosition);
					else
						vProductSetupPassword = "";

					WORD TempLength = TempPasswordRead.GetLength();
					if (TempLength > TempPosition + 1)
						vResetCountsPassword = TempPasswordRead.Right(TempLength - TempPosition - 1);
					else
						vResetCountsPassword = "";
				}
			}
		}  //end archive version 143
		else
		if (vArchiveVersionNumber > 140) 
		{//version 141 - 142
			CString TempPasswordRead("");

			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> TempPasswordRead
				>> vInspxPassword
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions
				>> vDiverterMargin
				>> vStopRunningOnUnConfirmedEject
				>> vMaxImageIntensityAdjust
				>> vShapeHandlingProcessIfAtSides
				>> vDriftCompensationVeryFastThreshold
				>> vDriftCompensationVeryFastAdjust
				>> vAdjustAllAreaOfHistogram
				>> vNumberOfQuickLearnImagesToTake
				>> vQuickLearnNumberOfStructureDeltas
				>> vPixelsPerDetector;

			//if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			vScanTracConfigurationFileType = cSingleImageScanTrac;
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffsetInteger[TempLoop];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}

				WORD TempPosition = TempPasswordRead.Find(',',0);
				if (TempPosition < 1000)
				{
					if (TempPosition)
						vProductSetupPassword = TempPasswordRead.Left(TempPosition);
					else
						vProductSetupPassword = "";

					WORD TempLength = TempPasswordRead.GetLength();
					if (TempLength > TempPosition + 1)
						vResetCountsPassword = TempPasswordRead.Right(TempLength - TempPosition - 1);
					else
						vResetCountsPassword = "";
				}
			}
		}  //end archive version 141 - 142
		else
		if (vArchiveVersionNumber == 140) 
		{//version 140

			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> vProductSetupPassword
				>> vInspxPassword
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions
				>> vDiverterMargin
				>> vStopRunningOnUnConfirmedEject
				>> vMaxImageIntensityAdjust
				>> vShapeHandlingProcessIfAtSides
				>> vDriftCompensationVeryFastThreshold
				>> vDriftCompensationVeryFastAdjust
				>> vAdjustAllAreaOfHistogram
				>> vNumberOfQuickLearnImagesToTake
				>> vQuickLearnNumberOfStructureDeltas;

			//if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			vScanTracConfigurationFileType = cSingleImageScanTrac;
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffsetInteger[TempLoop];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}
			}
		}  //end archive version 140
		else
		if (vArchiveVersionNumber == 139) 
		{//version 139
			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> vProductSetupPassword
				>> vInspxPassword
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions
				>> vDiverterMargin
				>> vStopRunningOnUnConfirmedEject
				>> vMaxImageIntensityAdjust
				>> vShapeHandlingProcessIfAtSides
				>> vDriftCompensationVeryFastThreshold
				>> vDriftCompensationVeryFastAdjust
				>> vAdjustAllAreaOfHistogram
				>> vNumberOfQuickLearnImagesToTake;


			//if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			vScanTracConfigurationFileType = cSingleImageScanTrac;
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffsetInteger[TempLoop];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}
			}
		}  //end archive version 139
		else
		if (vArchiveVersionNumber == 138) 
		{//version 138
			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> vProductSetupPassword
				>> vInspxPassword
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions
				>> vDiverterMargin
				>> vStopRunningOnUnConfirmedEject
				>> vMaxImageIntensityAdjust
				>> vShapeHandlingProcessIfAtSides
				>> vDriftCompensationVeryFastThreshold
				>> vDriftCompensationVeryFastAdjust
				>> vAdjustAllAreaOfHistogram;


			//if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			vScanTracConfigurationFileType = cSingleImageScanTrac;
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffsetInteger[TempLoop];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}
			}
		}  //end archive version 138
		else
		if (vArchiveVersionNumber == 137) 
		{//version 137
			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> vProductSetupPassword
				>> vInspxPassword
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions
				>> vDiverterMargin
				>> vStopRunningOnUnConfirmedEject
				>> vMaxImageIntensityAdjust
				>> vShapeHandlingProcessIfAtSides
				>> vDriftCompensationVeryFastThreshold
				>> vDriftCompensationVeryFastAdjust;


			//if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			vScanTracConfigurationFileType = cSingleImageScanTrac;
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffsetInteger[TempLoop];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}
			}
		}  //end archive version 137
		else
		if (vArchiveVersionNumber == 136) 
		{//version 136
			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> vProductSetupPassword
				>> vInspxPassword
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions
				>> vDiverterMargin
				>> vStopRunningOnUnConfirmedEject
				>> vMaxImageIntensityAdjust
				>> vShapeHandlingProcessIfAtSides;

			//if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			vScanTracConfigurationFileType = cSingleImageScanTrac;
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffsetInteger[TempLoop];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}
			}
		}  //end archive version 136
		else
		if (vArchiveVersionNumber == 135) 
		{//version 135
			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> vProductSetupPassword
				>> vInspxPassword
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions
				>> vDiverterMargin
				>> vStopRunningOnUnConfirmedEject
				>> vMaxImageIntensityAdjust;

			//if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			vScanTracConfigurationFileType = cSingleImageScanTrac;
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffsetInteger[TempLoop];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}
			}
		}  //end archive version 135
		else
		if (vArchiveVersionNumber == 134) 
		{//version 134
			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> vProductSetupPassword
				>> vInspxPassword
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions
				>> vDiverterMargin
				>> vStopRunningOnUnConfirmedEject;

			//if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			vScanTracConfigurationFileType = cSingleImageScanTrac;
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffsetInteger[TempLoop];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}
			}
		}  //end archive version 134
		else
		if (vArchiveVersionNumber == 133) 
		{//version 133
			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> vProductSetupPassword
				>> vInspxPassword
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions
				>> vDiverterMargin;

			//if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			vScanTracConfigurationFileType = cSingleImageScanTrac;
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffsetInteger[TempLoop];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}
			}
		}  //end archive version 133
		else
		if (vArchiveVersionNumber == 132) 
		{//version 132
			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> vProductSetupPassword
				>> vInspxPassword
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY
				>> vNumberOfTopJitterPositions;

			//if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			vScanTracConfigurationFileType = cSingleImageScanTrac;
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffsetInteger[TempLoop];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> 
					vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}
			}
		}  //end archive version 132
		else
		if ((vArchiveVersionNumber > 129) && (vArchiveVersionNumber < 132)) 
		{//version 130 - 131
			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> vProductSetupPassword
				>> vInspxPassword
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> vAlarmEjectorReEnableY;

			//if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			vScanTracConfigurationFileType = cSingleImageScanTrac;
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffsetInteger[TempLoop];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}
			}
		}  //end archive version 130 - 131
		else
		if (vArchiveVersionNumber == 129) 
		{//version 129
			BYTE TempByte = 0;
			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> vProductSetupPassword
				>> vInspxPassword
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate
				>> TempByte;

			//if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			vScanTracConfigurationFileType = cSingleImageScanTrac;
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffsetInteger[TempLoop];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}
			}
		}  //end archive version 129
		else
		if (vArchiveVersionNumber == 128) 
		{//version 128
			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> vProductSetupPassword
				>> vInspxPassword
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity
				>> vNumberOfDetectorsToCalibrate;

			//if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			vScanTracConfigurationFileType = cSingleImageScanTrac;
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffsetInteger[TempLoop];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}
			}
		}  //end archive version 128
		else
		if (vArchiveVersionNumber == 127) 
		{//version 127
			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> vProductSetupPassword
				>> vInspxPassword
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation
				>> vAdjustImageIntensity;

			//if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			vScanTracConfigurationFileType = cSingleImageScanTrac;
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffsetInteger[TempLoop];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}
			}
		}  //end archive version 127
		else
		if ((vArchiveVersionNumber > 124) &&  (vArchiveVersionNumber < 127))
		{//version 125 - 126
			TempArchive >> vEncoderDivider
				>> vNumberOfXRayDetectors
				>> vMaximumMonoBlockTemperature
				>> vNormalLearnDilateTimes
				>> vMaximumTemperatureAirOutside
				>> vDetectorClockDivider
				>> vNumberOfLastRejectImages
				>> vBackupDetectionUsingBodyTriggerTimeOut
				>> vNumberOfHeadsToMonitor[cFillerSamplingType]
				>> vProductSetupPassword
				>> vInspxPassword
				>> vImageShowLines
				>> vAutoImproveReduceThresholdFactor
				>> vNeedToCalibrateEncoder
				>> vWriteRejectsToFiles
				>> vSlipsEjectEmptyContainers
				>> vPixelsPerUnit
				>> vAutoImproveEnabled
				>> vScanTracType
				>> vCPMTimePeriod
				>> vEnableACMessages
				>> vBulkProductMode
				>> vRedIndicatorToShowOnMainMenu
				>> vImageShowFixedROILines
				>> vAutoImproveReduceThresholdFactor
				>> vSuppressWarningMessages
				>> vAutoProductionReportHour
				>> vImageShowScale
				>> vNumberOfEncoderRateReversalsForError
				>> vShowMouseCursor
				>> vLanguage
				>> vCorrectDetectorEdges
				>> vSimulateLightControlBoard
				>> vLearnQualityMarginalLimit
				>> vUseRealEncoder
				>> vRampXRayDuration
				>> vEnableEjectors
				>> vAutoDeleteFileTypes
				>> vAutoXRaySourceOffEnabled
				>> vInspxErrorMessageDisplayMask
				>> vXRayTubeHasCooled
				>> vAutoImproveEnabled
				>> vSystemEjectorDistance[2]
				>> vACPowerMonitorOffset
				>> vEncoderTolerance
				>> vQuickLearnDilateTimes
				>> vContinuousFeedEncoderRate
				>> vContinuousFeedContainerRate
				>> vInspxMessageCount
				>> vSpareDWORD3
				>> vAllowVoidInspection
				>> vDeltaTemperatureForCoolantProblem
				>> vLearnQualityPoorLimit
				>> vPipeLineFlowRate
				>> vSystemEjectorDistance[1]
				>> vReferenceROIsToEdges
				>> vLogSerialData
				>> vLogSyncData
				>> vHVPSType
				>> vSpareWasHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vDemoMode
				>> vIPDefaultDNSAddressString
				>> vShow24HourTime
				>> vShowOnlyRealImages
				>> vXRaySourceFilter
				>> vLastProduct
				>> vTemperatureSurfaceAdjust
				>> vTemperatureLiquidAdjust
				>> vTemperatureuCAdjust
				>> vTemperatureAirInsideAdjust
				>> vSourceStrengthCount
				>> vAskForProductionReportProductCode
				>> vPreAmplifierGain
				>> vRemoveBackgroundFromImage
				>> vScanTracID
				>> vVoltageAdjustOffset
				>> vCurrentAdjustOffset
				>> vVoltageAdjustGain
				>> vCurrentAdjustGain
				>> vSystemBodyTriggerToXRayBeam
				>> vSystemEjectorPositionOffset
				>> vCalibratedPixelsPerUnit
				>> vSystemEjectorDistance[0]
				>> vMaximumSurfaceTemperature
				>> vMaximumLiquidTemperature
				>> vMaximumuCTemperature
				>> vMaximumAirTemperature
				>> vMaximumVoltage
				>> vMaximumCurrent
				>> vMaximumPower
				>> vCommand1
				>> vCommand2
				>> vCommand3
				>> vCommand4
				>> vStructureDisableCount
				>> vDensityDisableCount
				>> vCalibrateBodyTriggerReferenceDisableCount
				>> vSlippedIndicatorTime
				>> vUseXScanDetectors
				>> vDeltaTemperatureForFilterCloggedError
				>> vShapeHandlingProcessIfConveyorStops
				>> vScanTracConfigurationFileType
				>> vLastBackupDirectoryUsed
				>> vScanTracSetupBackupDirectory
				>> vHaveCheckedForMoreRAM
				>> vDetectorLength
				>> vAverageDisableCount
				>> vXRaySourceRunDuration
				>> vNumberOfJitterPositions
				>> vHasOutsideAirTemp
				>> vDriftCompensationWaitTime
				>> vNumberOfImagesForDriftDensity
				>> vDriftCompensationFastThreshold
				>> vDriftCompensationFastAdjust
				>> vDriftCompensationSlowThreshold
				>> vDriftCompensationSlowAdjust
				>> vDriftCompensationEnabled
				>> vDriftCompensationThresholdReductionFactor
				>> vDriftCompensationThresholdReductionMaximum
				>> vDriftCompensationMaximumAdjust
				>> vTemperatureAirOutsideAdjust
				>> vAllowClientConnections
				>> vShapeHandlingProcessIfWrongWidth
				>> vShapeHandlingHeightTolerance
				>> vShapeHandlingWidthTolerance
				>> vShapeHandlingHeightTolerancePixels
				>> vShapeHandlingWidthTolerancePixels
				>> vShapeHandlingEjectorsVariableToReadInOldByte
				>> vMaxImageIntensityAdjustBeforeLocked
				>> vEjectContainersNotInspected
				>> vDiverterEjector
				>> vStopInspectionIfConveyorStops
				>> vShapeHandlingProcessIfWrongHeight
				>> vAlarmEjectorReEnableX
				>> vRadiationDoorDistanceStayOpen
				>> vRadiationDoorInDoorDistance
				>> vReCalibrateAfterStopTimePeriod
				>> vRadiationDoorInvertDoorOutput
				>> vRadiationShutterInSystem
				>> vErrorMessageDisplayMask
				>> vSurfaceTemperatureMovedToDXM
				>> vUseDACAndADCinDensityCompensation;

			//if (vScanTracConfigurationFileType == cSingleImageScanTrac)
			vScanTracConfigurationFileType = cSingleImageScanTrac;
			{
				for (WORD TempLoopW = 0; TempLoopW < cNumberOfEMailGroups; TempLoopW++)
				{
					TempArchive >> vEMailSubject[TempLoopW]
						>> vEMailEnable[TempLoopW]
						>> vNumberOfEMailRecievers[TempLoopW];
					for (BYTE TempLoop = 0; TempLoop < vNumberOfEMailRecievers[TempLoopW]; TempLoop++)
					{
						TempArchive >> vEMailUser[TempLoop + (TempLoopW * cMaximumEMailRecievers)]
							>> vEMailDomain[TempLoop + (TempLoopW * cMaximumEMailRecievers)];
					}
				}

				for (WORD TempLoop = 0; TempLoop < vSourceStrengthCount; TempLoop++)
				{
					TempArchive >> vSourceStrengthDate[TempLoop]
						>> vSourceStrengthData[TempLoop]
						>> vSourceStrengthComment[TempLoop];
				}

				for (WORD TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
					vBodyTrigger[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
					vExternalDetector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0;TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
					vEjector[TempLoop].Serialize(TempArchive);
				
				for (WORD TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
					TempArchive >> vDiagnosticLineSelect[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustGain[TempLoop][0];

				for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
					TempArchive >> vDetectorEdgeAdjustOffsetInteger[TempLoop];
			
				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
					TempArchive >> vDetectorGain[TempLoop] >> vADCDetectorOffset[TempLoop] >> vDACDetectorOffset[TempLoop];

				for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
				{
					TempArchive >> vMeasuredDetectorGain[TempLoop]
						>> vMeasuredDetectorOffset[TempLoop];
				}
			}
		}  //end archive version 125 - 126
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\nArchive file version error\n\nNo configuration data read";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		vGlobalScanTracType = vScanTracType;

		if (vGlobalArchiveVersionNumber < 157)
			vShapeHandlingEjectors = vShapeHandlingEjectorsVariableToReadInOldByte; //Changed to DWORD in version 157, so if reading 156 or lower, copy BYTE var to DWORD var

		if (vEncoderDividerForPoint4mmMode > 50)
			vEncoderDividerForPoint4mmMode = 0;  //puts it in Auto Mode

		if ((vPixelsPerUnitForPoint4mmMode < 0) || (vPixelsPerUnitForPoint4mmMode > 200))
			vPixelsPerUnitForPoint4mmMode = 0; //puts it in Auto Mode

		if (vArchiveVersionNumber < 152)
		for (BYTE TempLoop = 0; TempLoop < 3; TempLoop++)
			vNumberOfHeadsToMonitor[TempLoop] = 0;

		//vNumberOfXRayDetectorsToReadInImage = vNumberOfXRayDetectors;
		if (vScanTracType == cAllegroScanTrac)
		{
			//vNumberOfXRayDetectorsToReadInImage = 12;
			vNumberOfXRayDetectors = 9;
		}

		if (vCPMTimePeriod == 0)
			vCPMTimePeriod = 10;
		vCPMTimePeriod = 30;  //Can not be changed anymore, fixed at 30 CPM

		for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
		{
			if ((vDetectorGain[TempLoop] <= 1) || (vDetectorGain[TempLoop] > 10))
				vDetectorGain[TempLoop] = 10;
			if ((vDACDetectorOffset[TempLoop] < 0) || (vDACDetectorOffset[TempLoop] > 100))
				vDACDetectorOffset[TempLoop] = 0;
		}

		if ((vMaximumTemperatureAirOutside < 10) || (vMaximumTemperatureAirOutside > 70)) //set to 40~C if not in range
			vMaximumTemperatureAirOutside = 40;

		if (vUseDownStreamSignalThatStopsConveyor != cUseDownStream)
			vUseDownStreamSignalThatStopsConveyor = cDontUseDownStream;

		if ((vScanTracType != cAtempoScanTrac) && (vScanTracType != cForteScanTrac)) //Only TDs can use motor signal
		{
			vUseRealEncoder = true;
		}

		vSurfaceTemperatureMovedToDXM = false;

		if (vAutoXRaySourceOffEnabled == 0) //if it was disabled, set to enable.  New disabled value is 2
			vAutoXRaySourceOffEnabled = 1;
		if ((vAutoXRaySourceOffEnabled != 1) && (vAutoXRaySourceOffEnabled != 2))//if it was disabled, set to enable.  New disabled value is 2
			vAutoXRaySourceOffEnabled = 1;

		if (vNumberOfDaysToKeepLogs < 1)
		{
			vNumberOfDaysToKeepLogs = 60;
			vAutoDeleteFileTypes = 0x13;
		}

		if ((vMaximumMonoBlockTemperature < 40) || (vMaximumMonoBlockTemperature > 70))
			vMaximumMonoBlockTemperature = 50;

		vAutoImproveReduceThresholdFactor = 0; //alex does not want to use anymore so setting to zero
		if ((vQuickLearnDilateTimes == 0) || (vQuickLearnDilateTimes > 20)) //valid values 1 to 20, 20 is disabled so can set default to 5 when stream was zero, global var is 0 to 19 
			vQuickLearnDilateTimes = 5;
		if ((vNormalLearnDilateTimes == 0) || (vNormalLearnDilateTimes > 20)) //valid values 1 to 20, 20 is disabled
			vNormalLearnDilateTimes = 5;

		vGlobalNormalLearnDilateTimes = vNormalLearnDilateTimes;
		if (vGlobalNormalLearnDilateTimes > 19)
			vGlobalNormalLearnDilateTimes = 0;
		vGlobalQuickLearnDilateTimes = vQuickLearnDilateTimes;
		if (vGlobalQuickLearnDilateTimes > 19)
			vGlobalQuickLearnDilateTimes = 0;

		if (vPixelsPerDetector == 64)
		{
			vAdjustOffset[0] = 0; //1st Detector bottom
			vAdjustOffset[1] = 1; 
			vAdjustOffset[2] = 2; 
			vAdjustOffset[3] = 61; 
			vAdjustOffset[4] = 62; 
			vAdjustOffset[5] = 63;	//1st Detector top
			vAdjustOffset[6] = 64; //2nd Detector bottom
			vAdjustOffset[7] = 65; 
			vAdjustOffset[8] = 66; 
			vAdjustOffset[9] = 125; 
			vAdjustOffset[10] = 126; 
			vAdjustOffset[11] = 127;
			vAdjustOffset[12] = 128; //3rd Detector bottom
			vAdjustOffset[13] = 129; 
			vAdjustOffset[14] = 130; 
			vAdjustOffset[15] = 189; 
			vAdjustOffset[16] = 190; 
			vAdjustOffset[17] = 191;
			vAdjustOffset[18] = 192; //4th Detector bottom
			vAdjustOffset[19] = 193; 
			vAdjustOffset[20] = 194; 
			vAdjustOffset[21] = 253; 
			vAdjustOffset[22] = 254; 
			vAdjustOffset[23] = 255;
			vAdjustOffset[24] = 256; //5th Detector bottom
			vAdjustOffset[25] = 257; 
			vAdjustOffset[26] = 258; 
			vAdjustOffset[27] = 317; 
			vAdjustOffset[28] = 318; 
			vAdjustOffset[29] = 319;
			vAdjustOffset[30] = 320; //6th Detector bottom
			vAdjustOffset[31] = 321; 
			vAdjustOffset[32] = 322; 
			vAdjustOffset[33] = 381; 
			vAdjustOffset[34] = 382; 
			vAdjustOffset[35] = 383;
			vAdjustOffset[36] = 384; //7th Detector bottom
			vAdjustOffset[37] = 385; 
			vAdjustOffset[38] = 386; 
			vAdjustOffset[39] = 455; 
			vAdjustOffset[40] = 456; 
			vAdjustOffset[41] = 457;
			vAdjustOffset[42] = 448; //8th Detector bottom
			vAdjustOffset[43] = 449; 
			vAdjustOffset[44] = 450; 
			vAdjustOffset[45] = 509; //8th top
			vAdjustOffset[46] = 510; 
			vAdjustOffset[47] = 511;
			vAdjustOffset[48] = 512; //9th Detector bottom
			vAdjustOffset[49] = 513; 
			vAdjustOffset[50] = 514; 
			vAdjustOffset[51] = 573; //9th top
			vAdjustOffset[52] = 574; 
			vAdjustOffset[53] = 575;
			vAdjustOffset[54] = 576; //10th Detector bottom
			vAdjustOffset[55] = 577; 
			vAdjustOffset[56] = 578;
			vAdjustOffset[57] = 637; //10th top
			vAdjustOffset[58] = 638; 
			vAdjustOffset[59] = 639;
			vAdjustOffset[60] = 640; //11th Detector bottom
			vAdjustOffset[61] = 641; 
			vAdjustOffset[62] = 642;
			vAdjustOffset[63] = 701; //11th top
			vAdjustOffset[64] = 702; 
			vAdjustOffset[65] = 703;
			vAdjustOffset[66] = 704; //12th Detector bottom
			vAdjustOffset[67] = 705; 
			vAdjustOffset[68] = 706;
			vAdjustOffset[69] = 765; //12th top
			vAdjustOffset[70] = 766; 
			vAdjustOffset[71] = 767;
		}
		else
		if (vPixelsPerDetector == 128)
		{
			vAdjustOffset[0] = 0; //1st Detector bottom
			vAdjustOffset[1] = 1; 
			vAdjustOffset[2] = 2; 
			vAdjustOffset[3] = 126; 
			vAdjustOffset[4] = 127; 
			vAdjustOffset[5] = 128;	//1st Detector top
			vAdjustOffset[6] = 129; //2nd Detector bottom
			vAdjustOffset[7] = 130; 
			vAdjustOffset[8] = 131; 
			vAdjustOffset[9] = 254; 
			vAdjustOffset[10] = 255; 
			vAdjustOffset[11] = 256;
			vAdjustOffset[12] = 257; //3rd Detector bottom
			vAdjustOffset[13] = 258; 
			vAdjustOffset[14] = 259; 
			vAdjustOffset[15] = 382; 
			vAdjustOffset[16] = 383; 
			vAdjustOffset[17] = 384;
			vAdjustOffset[18] = 385; //4th Detector bottom
			vAdjustOffset[19] = 386; 
			vAdjustOffset[20] = 387; 
			vAdjustOffset[21] = 510; 
			vAdjustOffset[22] = 511; 
			vAdjustOffset[23] = 512;
			vAdjustOffset[24] = 513; //5th Detector bottom
			vAdjustOffset[25] = 514; 
			vAdjustOffset[26] = 515; 
			vAdjustOffset[27] = 638; 
			vAdjustOffset[28] = 639; 
			vAdjustOffset[29] = 640;
			vAdjustOffset[30] = 641; //6th Detector bottom
			vAdjustOffset[31] = 642; 
			vAdjustOffset[32] = 643; 
			vAdjustOffset[33] = 765; 
			vAdjustOffset[34] = 766; 
			vAdjustOffset[35] = 767;
			vAdjustOffset[36] = 768; //7th Detector bottom
			vAdjustOffset[37] = 769; 
			vAdjustOffset[38] = 770; 
			vAdjustOffset[39] = 894; 
			vAdjustOffset[40] = 895; 
			vAdjustOffset[41] = 896;
			vAdjustOffset[42] = 897; //8th Detector bottom
			vAdjustOffset[43] = 899; 
			vAdjustOffset[44] = 900; 
			vAdjustOffset[45] = 1021; //8th top
			vAdjustOffset[46] = 1022; 
			vAdjustOffset[47] = 1023;
			vAdjustOffset[48] = 1024; //9th Detector bottom
			vAdjustOffset[49] = 1025; 
			vAdjustOffset[50] = 1026; 
			vAdjustOffset[51] = 1149; //9th top
			vAdjustOffset[52] = 1150; 
			vAdjustOffset[53] = 1151;
			vAdjustOffset[54] = 1152; //10th Detector bottom
			vAdjustOffset[55] = 1153; 
			vAdjustOffset[56] = 1154;
			vAdjustOffset[57] = 1277; //10th top
			vAdjustOffset[58] = 1278; 
			vAdjustOffset[59] = 1279;
			vAdjustOffset[60] = 1280; //11th Detector bottom
			vAdjustOffset[61] = 1284; 
			vAdjustOffset[62] = 1282;
			vAdjustOffset[63] = 1405; //11th top
			vAdjustOffset[64] = 1406; 
			vAdjustOffset[65] = 1407;
			vAdjustOffset[66] = 1408; //12th Detector bottom
			vAdjustOffset[67] = 1409; 
			vAdjustOffset[68] = 1410;
			vAdjustOffset[69] = 1533; //12th top
			vAdjustOffset[70] = 1534; 
			vAdjustOffset[71] = 1535;
		}

		if (vArchiveVersionNumber < 147)
		{
			vSystemEjectorPositionOffset = 0;//zero as did not exist before this version
			vCalibratedPixelsPerUnit = 0;
			vSystemBodyTriggerToXRayBeam = 0;
			vNeedToCalibrateEncoder = 0;
		}
		if (vNeedToCalibrateEncoder > 2)
			vNeedToCalibrateEncoder = 0;
		if (vSystemEjectorPositionOffset > 1000)
			vSystemEjectorPositionOffset = 0;//zero as not valid
		if (vSystemEjectorPositionOffset < 2)
			vSystemEjectorPositionOffset = 0;//zero as not valid

		if (vSystemBodyTriggerToXRayBeam > 1000)
			vSystemBodyTriggerToXRayBeam = 0;//zero as not valid
		if (vSystemBodyTriggerToXRayBeam < 2)
			vSystemBodyTriggerToXRayBeam = 0;//zero as not valid

		if (vCalibratedPixelsPerUnit < 5)
			vCalibratedPixelsPerUnit = 0;
		if (vCalibratedPixelsPerUnit > 500)
			vCalibratedPixelsPerUnit = 0;

		if ((vBackupDetectionUsingBodyTriggerTimeOut & 0x7F) > 60) //over 60 not valid, so disable
			vBackupDetectionUsingBodyTriggerTimeOut = 0;

		if (vArchiveVersionNumber < 143)
		{
			//vCorrectDetectorEdges = 1;
			for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjust48; TempLoop++)
			{
				vDetectorEdgeAdjustOffset[vAdjustOffset[TempLoop]][0] = vDetectorEdgeAdjustOffsetInteger[TempLoop];

				if (TempLoop > 2)
				{
					vDetectorEdgeAdjustGain[vAdjustOffset[TempLoop]][0] = vDetectorEdgeAdjustGain[TempLoop][0];
					vDetectorEdgeAdjustGain[TempLoop][0] = 0;
				}
			}
		}
		if (vArchiveVersionNumber < 141)
		{
			vCalibrateBodyTriggerReferenceDisableCount = 20;
			vAverageDisableCount = 200;
			vStructureDisableCount = 2000;
			vDensityDisableCount = 2000;
			vShowOnlyRealImages = true;
			vNumberOfLastRejectImages = 50;
			vSuppressWarningMessages = 0;
		}
		if (vArchiveVersionNumber <= 145)
		if ((!vShapeHandlingProcessIfWrongWidth) && ((vShapeHandlingWidthTolerancePixels < 4) || (vShapeHandlingWidthTolerancePixels > 10)))
		{
			vShapeHandlingWidthTolerancePixels = 6;
			vShapeHandlingWidthTolerance = .2;
		}

		if (vArchiveVersionNumber == 141)
		{
			if (vNumberOfLastRejectImages < 2)
			{
				vNumberOfLastRejectImages = 50;
				vCalibrateBodyTriggerReferenceDisableCount = 20;
				vAverageDisableCount = 200;
				vStructureDisableCount = 2000;
				vDensityDisableCount = 2000;
				vShowOnlyRealImages = true;
				vSuppressWarningMessages = 0;
			}
			if (vSuppressWarningMessages == 2)
				vSuppressWarningMessages = 0;
			if (vShowOnlyRealImages > 1)
				vShowOnlyRealImages = 1;
		}

		if (vNumberOfDetectorsToCalibrate > vNumberOfXRayDetectors)
			vNumberOfDetectorsToCalibrate = 0;

		if (vDriftCompensationSlowAdjust < 1)  
		{	//if have old settings for ADC only, put in setting for ADC/DAC
			vDriftCompensationSlowAdjust = 1;
			vDriftCompensationSlowThreshold = 5;
			vDriftCompensationFastAdjust = 5;
			vDriftCompensationFastThreshold = 10;
			vDriftCompensationVeryFastThreshold = 15;
			vDriftCompensationVeryFastAdjust = 30;
			vDriftCompensationMaximumAdjust = 500;
			vDriftCompensationThresholdReductionMaximum = 20;
			vDriftCompensationThresholdReductionFactor = 1;
		}

		if (vCertifiedSetupPassword.GetLength() < 6)
			vCertifiedSetupPassword = "147963";
		if (vCertifiedSetupPassword == "468797326")
			vCertifiedSetupPassword = "147963";

		if (vScanTracScreenShotDirectory == "")
			vScanTracScreenShotDirectory = cScanTracScreenShotDirectory;
		if (vScanTracProductionReportDirectory == "")
			vScanTracProductionReportDirectory = cScanTracProductionReportDirectory;
		if (vScanTracEventLogDirectory == "")
			vScanTracEventLogDirectory = cScanTracEventLogDirectory;
		if (vScanTracImagesDirectory == "")
			vScanTracImagesDirectory = cImageDirectory;
		if (vScanTracCalibrationImagesDirectory == "")
			vScanTracCalibrationImagesDirectory = cScanTracCalibrationImagesDirectory;
		if (vScanTracSerialLogsDirectory == "")
			vScanTracSerialLogsDirectory = cScanTracSerialLogsDirectory;
		if (vScanTracRejectedImagesDirectory == "")
			vScanTracRejectedImagesDirectory = cRejectedImagesDirectoryDDrive;

		if (vRedIndicatorToShowOnMainMenu > cDefectsInRedBoxes)
			vRedIndicatorToShowOnMainMenu = cDefectInRedBox;
	
		if (vDetectorsAtBottomToIgnoreDuringCalibration > 12)
			vDetectorsAtBottomToIgnoreDuringCalibration = 0;

		vSpareDWORD3 = 0;
		vSpareDouble3 = 0;
//		vUseXofYAlarmOutputForUnconfirmedEject = 0;
//		vSpareWasHeadOffsetFromIndexPulse[0] = 0;

		if (vInspxPassword.GetLength() < 6)
			vInspxPassword = "304259";

		if (vNumberOfPixelAtBottomToIgnoreInCalibration > 255)
			vNumberOfPixelAtBottomToIgnoreInCalibration = 10;

		CString TempCurrentName = vScanTracSetupBackupDirectory;
		CString TempOriginalName = cOldScanTracSetupBackupDirectory;
		TempCurrentName.MakeUpper();
		TempOriginalName.MakeUpper();
		if (TempCurrentName == TempOriginalName) //"C:\\ScanTracSetupBackup\\"
		{
			vScanTracSetupBackupDirectory = cScanTracSetupBackupDirectory;
		}
		if (vAutoProductionReportHour > 24)
		{
			ReportErrorMessage("vAutoProductionReportHour1 parameter bad, set to disabled", cEMailInspx, 32000);
			vAutoProductionReportHour3 = 0;
		}

		if (vAutoProductionReportHour2 > 24)
		{
			ReportErrorMessage("vAutoProductionReportHour2 parameter bad, set to disabled", cEMailInspx, 32000);
			vAutoProductionReportHour3 = 0;
		}

		if (vAutoProductionReportHour3 > 24)
		{
			ReportErrorMessage("vAutoProductionReportHour3 parameter bad, set to disabled", cEMailInspx, 32000);
			vAutoProductionReportHour3 = 0;
		}

		if (vAskForProductionReportProductCode > 2)
		{
			ReportErrorMessage("vAskForProductionReportProductCode parameter bad, set to disabled", cEMailInspx, 32000);
			vAskForProductionReportProductCode = 0;
		}

//		vRFIDEASBadgeReaderCommPort = 0;
		vNumberOfJitterPositions = 0;
		vNumberOfTopJitterPositions = 0;

		int TempLength = vAdvancedSetupPassword.GetLength();
		if (TempLength < 6)
			vAdvancedSetupPassword = "147963";

		if (vInspxPassword == "302659")
				vInspxPassword = "304259";

		switch (vHVPSType)
		{
			case cDXMHVPS: 
				if (vMaximumPower > 600)
					vMaximumPower = 600;
				if (vMaximumVoltage > 70)
					vMaximumVoltage = 70;
				if (vMaximumCurrent > 8.55)
					vMaximumCurrent = 8.55;
			break;
			case cCheyneyMonoBlock: 
				if (vMaximumPower > 210)
					vMaximumPower = 210;
				if (vMaximumVoltage > 70)
					vMaximumVoltage = 70;
				if (vMaximumCurrent > 3)
					vMaximumCurrent = 3;
			break;
			case c100WMonoBlockHVPS: 
				if (vMaximumPower > 100)
					vMaximumPower = 100;
				if (vMaximumVoltage > 80)
					vMaximumVoltage = 80;
				if (vMaximumCurrent > 2)
					vMaximumCurrent = 2;
			break;
			case c210WMonoBlockHVPS: 
				if (vMaximumPower > 210)
					vMaximumPower = 210;
				if (vMaximumVoltage > 80)
					vMaximumVoltage = 80;
				if (vMaximumCurrent > 6)
					vMaximumCurrent = 6;
			break;
			case cXLFHVPS: 
				if (vMaximumPower > 480)
					vMaximumPower = 480;
				if (vMaximumVoltage > 60)
					vMaximumVoltage = 60;
				if (vMaximumCurrent > 8)
					vMaximumCurrent = 8;
			break;
			case cMNXHVPS: 
				if (vMaximumPower > 50)
					vMaximumPower = 50;
				if (vMaximumVoltage > 50)
					vMaximumVoltage = 50;
				if (vMaximumCurrent > 2)
					vMaximumCurrent = 2;
			break;
			case c100WVJTMonoBlockHVPS: 
				if (vMaximumPower > 100)
					vMaximumPower = 100;
				if (vMaximumVoltage > 80)
					vMaximumVoltage = 80;
				if (vMaximumCurrent > 2.5)
					vMaximumCurrent = 2.5;
			break;
		}
		if ((vOverWriteBadPixel[0][0] != 0xAAAA) && (vOverWriteBadPixel[0][0] != 0xAAAB))
		{
			for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
				vOverWriteBadPixel[TempLoop][0] = 0;

			vOverWriteBadPixel[0][0] = 0xAAAA;
		}
		if ((vACPowerMonitorOffset > 12) || (vACPowerMonitorOffset < -12))
			vACPowerMonitorOffset = 0;

		if ((vShowSensitivityDisplays != cShowSensitivityDisplays) && (vShowSensitivityDisplays != cHideSensitivityDisplays))
			vShowSensitivityDisplays = cShowSensitivityDisplays;

		if ((vShapeHandlingHeightTolerance > 255) || (vShapeHandlingHeightTolerance < 0))
		{
			vShapeHandlingHeightTolerance = 0.4;
			ReportErrorMessage("Bad float in stream corrected: vShapeHandlingHeightTolerance", cEMailInspx, 32000);
		}
		if ((vShapeHandlingWidthTolerance > 255) || (vShapeHandlingWidthTolerance < 0))
		{
			vShapeHandlingWidthTolerance = .4;
			ReportErrorMessage("Bad float in stream corrected: vShapeHandlingWidthTolerance", cEMailInspx, 32000);
		}
		if ((vDriftCompensationFastThreshold > 255) || (vDriftCompensationFastThreshold < 0))
		{
			vDriftCompensationFastThreshold = 10;
			ReportErrorMessage("Bad float in stream corrected: vDriftCompensationFastThreshold", cEMailInspx, 32000);
		}
		if ((vDriftCompensationVeryFastThreshold > 255) || (vDriftCompensationVeryFastThreshold < 0))
		{
			vDriftCompensationVeryFastThreshold = 15;
			ReportErrorMessage("Bad float in stream corrected: vDriftCompensationVeryFastThreshold", cEMailInspx, 32000);
		}
		if ((vDriftCompensationFastAdjust > 255) || (vDriftCompensationFastAdjust < 0))
		{
			vDriftCompensationFastAdjust = 20;
			ReportErrorMessage("Bad float in stream corrected: vDriftCompensationFastAdjust", cEMailInspx, 32000);
		}
		if ((vDriftCompensationVeryFastAdjust > 255) || (vDriftCompensationVeryFastAdjust < 0))
		{
			vDriftCompensationVeryFastAdjust = 40;
			ReportErrorMessage("Bad float in stream corrected: vDriftCompensationVeryFastAdjust", cEMailInspx, 32000);
		}
		if ((vDriftCompensationSlowThreshold > 255) || (vDriftCompensationSlowThreshold < 0))
		{
			vDriftCompensationSlowThreshold = 5;
			ReportErrorMessage("Bad float in stream corrected: vDriftCompensationSlowThreshold", cEMailInspx, 32000);
		}
		if ((vDriftCompensationSlowAdjust > 255) || (vDriftCompensationSlowAdjust < 0))
		{
			vDriftCompensationSlowAdjust = 1;
			ReportErrorMessage("Bad float in stream corrected: vDriftCompensationSlowAdjust", cEMailInspx, 32000);
		}
		if ((vDriftCompensationMaximumAdjust > 1000) || (vDriftCompensationMaximumAdjust < 0))
		{
			vDriftCompensationMaximumAdjust = 500;
			ReportErrorMessage("Bad float in stream corrected: vDriftCompensationMaximumAdjust", cEMailInspx, 32000);
		}
		if ((vDriftCompensationThresholdReductionFactor > 1000) || (vDriftCompensationThresholdReductionFactor < 0))
		{
			vDriftCompensationThresholdReductionFactor = 0;
			ReportErrorMessage("Bad float in stream corrected: vDriftCompensationThresholdReductionFactor", cEMailInspx, 32000);
		}
		
		if ((vQuickLearnNumberOfStructureDeltas > 255) || (vQuickLearnNumberOfStructureDeltas < 0))
		{
			vQuickLearnNumberOfStructureDeltas = 3.0;
			ReportErrorMessage("Bad float in stream corrected: vQuickLearnNumberOfStructureDeltas", cEMailInspx, 32000);
		}
		if ((vVoltageAdjustOffset > 255) || (vVoltageAdjustOffset < -255))
		{
			vVoltageAdjustOffset = 0;
			ReportErrorMessage("Bad float in stream corrected: vVoltageAdjustOffset", cEMailInspx, 32000);
		}
		if ((vCurrentAdjustOffset > 255) || (vCurrentAdjustOffset < -255))
		{
			vCurrentAdjustOffset = 0;
			ReportErrorMessage("Bad float in stream corrected: vCurrentAdjustOffset", cEMailInspx, 32000);
		}
		if ((vVoltageAdjustGain > 255) || (vVoltageAdjustGain < -255))
		{
			vVoltageAdjustGain = 0;
			ReportErrorMessage("Bad float in stream corrected: vVoltageAdjustGain", cEMailInspx, 32000);
		}
		if ((vCurrentAdjustGain > 255) || (vCurrentAdjustGain < -255))
		{
			vCurrentAdjustGain = 0;
			ReportErrorMessage("Bad float in stream corrected: vCurrentAdjustGain", cEMailInspx, 32000);
		}
		if ((vSystemBodyTriggerToXRayBeam > 255) || (vSystemBodyTriggerToXRayBeam < 0))
		{
			vSystemBodyTriggerToXRayBeam = 0;
			ReportErrorMessage("Bad float in stream corrected: vSystemBodyTriggerToXRayBeam", cEMailInspx, 32000);
		}
		if ((vSystemEjectorPositionOffset > 255) || (vSystemEjectorPositionOffset < 0))
		{
			vSystemEjectorPositionOffset = 0;
			ReportErrorMessage("Bad float in stream corrected: vSystemEjectorPositionOffset", cEMailInspx, 32000);
		}
		if ((vMaximumCurrent > 255) || (vMaximumCurrent < 0))
		{
			vMaximumCurrent = 10;
			ReportErrorMessage("Bad float in stream corrected: vMaximumCurrent", cEMailInspx, 32000);
		}
		if ((vMaximumVoltage > 255) || (vMaximumVoltage < 0))
		{
			vMaximumVoltage = 70;
			ReportErrorMessage("Bad float in stream corrected: vMaximumVoltage", cEMailInspx, 32000);
		}
		if ((vMaximumPower > 1000) || (vMaximumPower < 0))
		{
			vMaximumPower = 600;
			ReportErrorMessage("Bad float in stream corrected: vMaximumPower", cEMailInspx, 32000);
		}
		if ((vTemperatureSurfaceAdjust > 1000) || (vTemperatureSurfaceAdjust < -100))
		{
			vTemperatureSurfaceAdjust = 600;
			ReportErrorMessage("Bad float in stream corrected: vTemperatureSurfaceAdjust", cEMailInspx, 32000);
		}
		if ((vTemperatureLiquidAdjust > 1000) || (vTemperatureLiquidAdjust < -100))
		{
			vTemperatureLiquidAdjust = 600;
			ReportErrorMessage("Bad float in stream corrected: vTemperatureLiquidAdjust", cEMailInspx, 32000);
		}
		if ((vTemperatureuCAdjust > 1000) || (vTemperatureuCAdjust < -100))
		{
			vTemperatureuCAdjust = 600;
			ReportErrorMessage("Bad float in stream corrected: vTemperatureuCAdjust", cEMailInspx, 32000);
		}
		if ((vTemperatureAirInsideAdjust > 1000) || (vTemperatureAirInsideAdjust < -100))
		{
			vTemperatureAirInsideAdjust = 600;
			ReportErrorMessage("Bad float in stream corrected: vTemperatureAirInsideAdjust", cEMailInspx, 32000);
		}
		if ((vTemperatureAirOutsideAdjust > 1000) || (vTemperatureAirOutsideAdjust < -100))
		{
			vTemperatureAirOutsideAdjust = 600;
			ReportErrorMessage("Bad float in stream corrected: vTemperatureAirOutsideAdjust", cEMailInspx, 32000);
		}
		if ((vPixelsPerUnit > 1000) || (vPixelsPerUnit < 0))
		{
			vPixelsPerUnit = 32;
			ReportErrorMessage("Bad float in stream corrected: vPixelsPerUnit", cEMailInspx, 32000);
		}
		if ((vCalibratedPixelsPerUnit > 1000) || (vCalibratedPixelsPerUnit < 0))
		{
			vCalibratedPixelsPerUnit = 0;
			ReportErrorMessage("Bad float in stream corrected: vCalibratedPixelsPerUnit", cEMailInspx, 32000);
		}
		if ((vPipeLineFlowRate > 1000) || (vPipeLineFlowRate < 0))
		{
			vPipeLineFlowRate = 0;
			ReportErrorMessage("Bad float in stream corrected: vPipeLineFlowRate", cEMailInspx, 32000);
		}
		if (vAutoStop > 24)
			vAutoStop = 0;

		for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
		{
			if ((vDetectorGain[TempLoop] > 255) || (vDetectorGain[TempLoop] < -255))
			{
				vDetectorGain[TempLoop] = 10;
				ReportErrorMessage("Bad float in stream corrected: vDetectorGain[TempLoop]", cEMailInspx, 32000);
			}
			if ((vDACDetectorOffset[TempLoop] > 255) || (vDACDetectorOffset[TempLoop] < -255))
			{
				vDACDetectorOffset[TempLoop] = 0;
				ReportErrorMessage("Bad float in stream corrected: vDACDetectorOffset[TempLoop]", cEMailInspx, 32000);
			}
		}
		for (WORD TempDetectorTypeLoop = cPoint4mmDetectors; TempDetectorTypeLoop <= cPoint8mmDetectors; TempDetectorTypeLoop++)
		for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
		{
			if ((vDetectorEdgeAdjustGain[TempLoop][TempDetectorTypeLoop] > 255) || (vDetectorEdgeAdjustGain[TempLoop][TempDetectorTypeLoop] < -255))
			{
				vDetectorEdgeAdjustGain[TempLoop][TempDetectorTypeLoop] = 0;
				ReportErrorMessage("Bad float in stream corrected: vDetectorEdgeAdjustGain[TempLoop][0]", cEMailInspx, 32000);
			}
			if ((vDetectorEdgeAdjustOffset[TempLoop][TempDetectorTypeLoop] > 255) || (vDetectorEdgeAdjustOffset[TempLoop][TempDetectorTypeLoop] < -255))
			{
				vDetectorEdgeAdjustOffset[TempLoop][TempDetectorTypeLoop] = 0;
				ReportErrorMessage("Bad float in stream corrected: vDetectorEdgeAdjustOffset[TempLoop][0]", cEMailInspx, 32000);
			}
		}
		for (WORD TempLoop = 0; TempLoop < cMaximumSourceStrengthCount; TempLoop++)
		{
			if ((vSourceStrengthData[TempLoop] > 255) || (vSourceStrengthData[TempLoop] < 0))
			{
				vSourceStrengthData[TempLoop] = 0;
				ReportErrorMessage("Bad float in stream corrected: vSourceStrengthData[TempLoop]", cEMailInspx, 32000);
			}
		}
		for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
		{
			if ((vMeasuredDetectorGain[TempLoop] > 255) || (vMeasuredDetectorGain[TempLoop] < 0))
			{
				vMeasuredDetectorGain[TempLoop] = 0;
				ReportErrorMessage("Bad float in stream corrected: vMeasuredDetectorGain[TempLoop]", cEMailInspx, 32000);
			}
			if ((vMeasuredDetectorOffset[TempLoop] > 1000) || (vMeasuredDetectorOffset[TempLoop] < -1000))
			{
				vMeasuredDetectorOffset[TempLoop] = 0;
				ReportErrorMessage("Bad float in stream corrected: vMeasuredDetectorOffset[TempLoop]", cEMailInspx, 32000);
			}
		}

		if (vSlippedIndicatorTime > 10000)
			vSlippedIndicatorTime = 500;
		if ((vReCalibrateAfterStopTimePeriod < 10) ||  //if value is not valid, set it to default
			(vReCalibrateAfterStopTimePeriod > 1800))
		if (vReCalibrateAfterStopTimePeriod) // change only if not zero now as zero is disabled
			vReCalibrateAfterStopTimePeriod = 120;

		CString TempText;
		//if (vUseMetric)
		//	TempText.LoadString(IDS_Millimeters);
		//else
			TempText.LoadString(IDS_Inches);
		vUnitsString = TempText;

		vGlobalPixelsPerUnit = vPixelsPerUnit;

		if (vPixelsPerDetector == 128) //uses .4mm detectors which are 128 pixels in 2 inches
			vGlobalPixelsPerUnitInHeight = 64;
		else
		{
			SetPixelsPerDetector(64);
			if (vScanTracType == cCaseInspectorScanTrac) //uses 1.6mm detectors which are 64 pixels in 4 inches
			{
				vGlobalPixelsPerUnitInHeight = 16;
				vDetectorLength = 4;
			}
			else
				vGlobalPixelsPerUnitInHeight = 32; //uses .8mm detectors which are 64 pixels in 2 inches
		}

		if (vDetectorLength != 4)
			vDetectorLength = 2;

		if ((vTemperatureAirOutsideAdjust > 30) || (vTemperatureAirOutsideAdjust < -30))
			vTemperatureAirOutsideAdjust = 0;
	
		if ((vMaximumCPUTemperature < 40) || (vMaximumCPUTemperature > 70))
			vMaximumCPUTemperature = 62;

		if ((!vProductSimpleSetupPassword.GetLength()) && (vProductSetupPassword.GetLength())) //if have a setup password, but not a simple setup password, set the simple setup password
			vProductSimpleSetupPassword = "357";

		if (!vQAPassword.GetLength())
			vQAPassword = "6565";
		if (vQAPassword == vProductSetupPassword)
			vQAPassword = "6565";

		if (vSupportContractType == cSupportContractInvalid)
			vSupportContractType = cHideSupportContractDisplay;
	
		if (vSystemSetupResetPassword.GetLength() < 8)
			vSystemSetupResetPassword = "95038603";

		if ((vUseExternalInterlock) & (!((vScanTracType == cAllegroScanTrac) || (vScanTracType == cBriosoScanTrac) || (vScanTracType == cPiccoloScanTrac) || 
			(vScanTracType == cCaseInspectorScanTrac) || (vScanTracType == cForteScanTrac) || (vScanTracType == cAtempoScanTrac) || (vScanTracType == cLegatoScanTrac))))
			vUseExternalInterlock = 0;

		for (BYTE TempByteLoop = 0; TempByteLoop < cNumberOfEjectors; TempByteLoop++)
		{
			if (vEjector[TempByteLoop].vDigitalInputLine == 0)
			{
				vEjector[TempByteLoop].SetDigitalInputLine(TempByteLoop + 1);
				vEjector[TempByteLoop].vActiveHigh = false;
			}
		}

		if (vOperationsGuardianServer.GetLength() < 5)
			vOperationsGuardianServer = cOperationsGuardianDefaultServerName; 


	}//end of reading in setup data
	if ((vReferenceContainersSideToSide != cDontReferenceSideToSide) && (vReferenceContainersSideToSide != cReferenceSideToSide))
	{
		if (((vScanTracType == cSoloScanTrac) || (vScanTracType == cSoloPlusScanTrac) || (vScanTracType == cPiccoloScanTrac) || 
			(vScanTracType == cUpSideDownSoloScanTrac)) && (!vContinuousFeedContainerRate) && (!vBulkProductMode)) //Not Continuous Feed, or a Pipeline
			vReferenceContainersSideToSide = cReferenceSideToSide;
		else
			vReferenceContainersSideToSide = cDontReferenceSideToSide;
	}
	if (vAlarmEjectorReEnableY > cMaximumEjectDataSize)
		vAlarmEjectorReEnableY = cMaximumEjectDataSize;
		
	if (vAlarmEjectorReEnableX > cMaximumEjectDataSize)
		vAlarmEjectorReEnableX = cMaximumEjectDataSize;
}

int CSystemConfigurationData::SelectEjectors(CString TempEjectorType, 
	DWORD *TempEjectors, CSystemConfigurationData *TempConfigurationData, BYTE TempInAutoSetup)
{
	CSelectMultipleItemDialog ISelectItemDialog;  
	
	//Set dialog box data titles and number value
	ISelectItemDialog.m_DialogTitleStaticText = "Select " + TempEjectorType + " Ejectors";
	if (TempEjectorType == "Test Fire")
		ISelectItemDialog.m_DialogTitleStaticText = "Select Ejectors to " + TempEjectorType;
	ISelectItemDialog.vNumberOfItems = cNumberOfEjectors;
	ISelectItemDialog.vInAutoSetup = TempInAutoSetup;
	ISelectItemDialog.vUseSelectText = true;
	//ISelectItemDialog.vColor = cGreen;

	BYTE TempNumberOfEjectorsInSystem = 0;
	BYTE TempOnlyEjectorNumberConfigured = 0;

	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
	{
		if (TempConfigurationData->vEjector[TempLoop].vDigitalInputLine)
		if (TempConfigurationData->vEjector[TempLoop].vEnabled)
		{
			CString TempString(TempConfigurationData->vEjector[TempLoop].vName);
			if (!TempConfigurationData->vEjector[TempLoop].vDigitalInputLine)
			{
				TempString = TempString + "-Not Configured";
				ISelectItemDialog.vItemEnabled[TempLoop] = false;
			}
			else
			if (!TempConfigurationData->vEjector[TempLoop].vEnabled)
			{
				TempString = TempString + "-Disabled";
				ISelectItemDialog.vItemEnabled[TempLoop] = false;
			}

			ISelectItemDialog.vItems[TempLoop] = *TempEjectors & (1 << TempLoop);
			ISelectItemDialog.vItemsYes[TempLoop] = "Selected-" + TempString;
			ISelectItemDialog.vItemsNo[TempLoop] = "Not Selected-" + TempString;
			TempNumberOfEjectorsInSystem++;
			TempOnlyEjectorNumberConfigured = TempLoop;
		}
	}
	ISelectItemDialog.vNumberOfItems = TempNumberOfEjectorsInSystem;
	int nResponse = 0;

	if (TempNumberOfEjectorsInSystem == 1)
	{
		*TempEjectors = 1 << TempOnlyEjectorNumberConfigured;
		nResponse = IDOK;
	}
	else
	if (TempNumberOfEjectorsInSystem > 1)
	{
		bool TempNeedToSelect = true;
		while (TempNeedToSelect)
		{
			//Pass control to dialog box and display
			nResponse = ISelectItemDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				*TempEjectors = 0;
				BYTE TempNumberOfEjectorsSelected = 0;
				for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
				{
					if (ISelectItemDialog.vItems[TempLoop])
					{
						*TempEjectors = *TempEjectors | (1 << TempLoop);
						TempNumberOfEjectorsSelected++;
					}
				}
				if (TempNumberOfEjectorsSelected == 1)
					TempNeedToSelect = false;
				else
				if (TempNumberOfEjectorsSelected == 0)
				{
					CYesNoDialog TempYesNoDialog;
					TempYesNoDialog.vNoticeText = _T("\nNo Ejectors Selected\nfor this inspection\nAre you sure you want no ejector?");
					TempYesNoDialog.vYesButtonText = "No Ejector";
					TempYesNoDialog.vNoButtonText = "Select\nAgain";
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult == IDOK)
						TempNeedToSelect = false;
				}
				else
				{
					CYesNoDialog TempYesNoDialog;
					TempYesNoDialog.vNoticeText = _T("\nMore than 1 Ejectors Selected\nfor this inspection\nAre you sure you want more than 1 ejector?");
					TempYesNoDialog.vYesButtonText = "Save\nEjectors";
					TempYesNoDialog.vNoButtonText = "Select\nAgain";
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult == IDOK)
						TempNeedToSelect = false;
				}
			}
			else
			if ((nResponse == 10) || (nResponse == 2))
				TempNeedToSelect = false;
		}
	}
	return nResponse;
}

CString CSystemConfigurationData::GetEjectorNames(DWORD TempEjector)
{
	CString TempString = "";
	TempString = "";
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
	if (TempEjector & (1 << TempLoop))
	//if ((vEjector[TempLoop].vEnabled) &&
	//	(vEjector[TempLoop].vLineBitMask))
	{
		if (!(TempString == ""))
			TempString = TempString + ", ";
		TempString = TempString + 
			vEjector[TempLoop].vName;
	}
	if (TempString == "")
		TempString = "None";
	return TempString;
}

bool CSystemConfigurationData::LimitToMaximumVoltage(double &TempVoltage)
{
	if (TempVoltage > vMaximumVoltage / 6.0)
	{
		ReportErrorMessage("Notice: Voltage Limited from : " + dtoa(TempVoltage * 6.0, 1) + " to: " + dtoa(vMaximumVoltage, 1), cWriteToLog, 0);
		TempVoltage = vMaximumVoltage / 6.0;
		return true;
	}
	else
		return false;
}

bool CSystemConfigurationData::LimitToMaximumCurrent(double TempVoltage, double &TempCurrent)
{
		double TempMaximumCurrent = GetMaximumCurrentFromVoltage(TempVoltage);
		if (TempMaximumCurrent > vMaximumCurrent)
			TempMaximumCurrent = vMaximumCurrent;

		double TempMaximumCurrentP = GetMaximumCurrentFromPower(TempVoltage, vMaximumPower);

		if (TempMaximumCurrent > TempMaximumCurrentP)
			TempMaximumCurrent = TempMaximumCurrentP;

		if (TempCurrent > TempMaximumCurrent)
		{
			ReportErrorMessage("Notice: Current Limited from : " + dtoa(TempCurrent, 1) + " to: " + dtoa(TempMaximumCurrent, 1), cWriteToLog, 0);
			TempCurrent = TempMaximumCurrent;
			return true;
		}
		else
			return false;
}



void CSystemConfigurationData::SetPixelsPerDetector(BYTE TempPixelsPerDetector)
{
	vPixelsPerDetector = TempPixelsPerDetector;
	if (vDetectorLength)
		vGlobalPixelsPerUnitInHeight = vPixelsPerDetector / vDetectorLength;
}

void CSystemConfigurationData::SetDetectorLength(BYTE TempDetectorLength)
{
	vDetectorLength = TempDetectorLength;
	if (vDetectorLength)
		vGlobalPixelsPerUnitInHeight = vPixelsPerDetector / vDetectorLength;
}

CScanTracSystemRunningData::CScanTracSystemRunningData()
{
	vResynchronizeEjectorsCurrentEjectorSensorCount[0] = 0;
	vResynchronizeEjectorsCurrentEjectorSensorCount[1] = 0;
	vResynchronizeEjectorsCurrentTriggerCount = 0;
	vFirstContainerShouldNotTriggerSensorYet = false;
	vBeltPositionTunnelShouldBeClearTo = 0;
	vOperationsGuardianConnected = false;
	vOperationsGuardianError = 0;

	vMultiLaneInSetupLanePositionsMenu = false;
	vMultiLaneLastTriggerPosition = 0;
	vMultiLaneLastTriggerPositionForThisImage = 0;

	for (BYTE TempLaneLoop = 0; TempLaneLoop < cMaximumNumberForMultiLane; TempLaneLoop++)
	{
		vMultiLaneTotalCounts[TempLaneLoop] = 0;
		vMultiLaneTotalEjectsCounts[TempLaneLoop] = 0;
		vMultiLaneTotalDisabledEjectsCounts[TempLaneLoop] = 0;

		for (BYTE TempLoopInspection = 0; TempLoopInspection < cMaximumNumberOfInspections; TempLoopInspection++)
		{
			vMultiLaneInspectionCounts[TempLaneLoop][TempLoopInspection] = 0;
			vMultiLaneInspectionDisabledCounts[TempLaneLoop][TempLoopInspection] = 0;
		}
	}

	vModBusSocketOpen = false;
	vModBusClientConnected = false;
	vModBusClientConnectionCounts = 0;
	vModBusClientRequestCounts = 0;
	vModBusTCPIPErrorCounter = 0;

	vModBusTCPIPAddress = "Socket Not Open";
	vCalibrationADCOffsetMaximumBit = 0x400;  //11 bits to test  
	vSampleEjectNextContainerBitMap = 0;
	vCurrentEncoderDivider = 0;

	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors + 1; TempLoop++)
	{
		vDiverterActive[TempLoop] = 0;
		vDiverterDeactivatePosition[TempLoop] = 0;
	}

	vCalibratingXScanPoint4mmDetectors = 0;

	//Valve monitor data
	vValveMonitorInspection = 0xFF;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors + 1; TempLoop++)
		vRecievedFirstIndexPulse[TempLoop] = 0;
	vFillerIndexPulseBitMap = 0;
	vSeamerPocketSensorBitMap = 0;
	vSeamerObjectPulseBitMap = 0;
	vLastTimeObjectToPocketPulse = 0;
	vLastPocketTime = 0;
	vLastTimePocketToObjectPulse = 0;
	vLastObjectTime = 0;
	vMissedIndexCount = 0;
	vTimeToGetIndexPulse = true;
	vValveDataRowIndex = 0;
	vNumberValveRowDataHave = 0;
	vCurrentValveData = NULL; //is the size of the number of valves,  data is stored in package after sync pulse is first
	vHoldValveDataForNextIndexPulse = NULL;
	vValveData = NULL; //every n values is one set of cMaximumValveDataRows data from the valves, use vValveDataRowIndex * vNumberOfHeadsToMonitor to write next set of data in.  Data stored in Valve 1 is first
	vCurrentValveDataMaximum = NULL; //pointer must be less than this
	vValveDataMaximum = NULL; //pointer must be less than this
	vValveAverages = NULL;
	vValveAveragesMaximum = NULL;
	vValveStandardDeviations = NULL;
	vValveStandardDeviationsMaximum = NULL;
	vNumberOfValvesThatNeedAttention = 0;
	vFillerAverageWeight = 0;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
		vFindFillerOffset[TempLoop] = false;
	vFillerHighThreshold = 0;
	vFillerLowThreshold = 0;
	for (BYTE TempLoop = 0; TempLoop < 255; TempLoop++)
		vFillerLowFillHeadCounts[TempLoop] = 0;

	//Seamer Head Data
	vSeamerIndexPulseBitMap = 0;
	vSamplingSkipNextSample = 0;
	vTimeToGetSeamerIndexPulse = true;
	vMissedSeamerIndexCount = 0 ;
	vNeedToDisableEjectorsAfterSampleAll = false;
	vUseSamplingFIFO = false;

	for (BYTE TempLoop = 0; TempLoop < 3; TempLoop++)
	{
		vCurrentSampleDataCount[TempLoop] = 0;
		vSamplingFIFO[TempLoop] = NULL;
		vSamplingFIFOMaximumPointer[TempLoop] = NULL;
		vSamplingFIFOHead[TempLoop] = 0;
		vSamplingFIFOTail[TempLoop] = 0;
		vBeltPositionLastPutItemInSamplingFIFO = 0;
		vHaveFillerIndexError = 0;
		vPreviousFillerHeadPutInContainer = 0;

		vSampledOnLastRevolution[TempLoop] = false;
		vSamplesEjectedCount[TempLoop] = 0;

		vSamplingNowWhichHeadToSample[TempLoop] = 0;
		vSamplingNowNumberOfSamplesToTakeFromSeries[TempLoop] = 0;
		vSamplingNowNumberOfSamplesToTakeFromSameHead[TempLoop] = 0;

		vSamplingParameterSetStartHead[TempLoop] = 1;
		vSamplingParameterSetHeadQuantity[TempLoop] = 5;
		vSamplingParameterSingleHead[TempLoop] = 1;
		vSamplingParameterSingleHeadQuantity[TempLoop] = 5;

		vSamplingIndexErrorData[TempLoop] = NULL; //0=index pulse correct, Negative value = missed that many packages for that valve set, positive value = had that many extra packages for that valve set.  Is cMaximumValveDataRows in size
		vSamplingIndexErrorDataMaximum[TempLoop] = NULL; //Pointer must be less than this
		vSamplingIndexErrorDataIndex[TempLoop] = 0;
		vSamplingIndexErrorDataHave[TempLoop] = 0;
		vNextHeadToProcess[TempLoop] = 0;
		vNextHeadToPutInFIFO[TempLoop] = 0;
		vSamplingPulseLog[TempLoop] = "Pulse Log-While Logging Serial Data-Most Recent At Bottom\n";
		vSamplingPulseIndexCounter[TempLoop] = 0;
		vCountPutInFIFOThisRevolution[TempLoop] = 0;
		vPreviousHeadProcessed[TempLoop] = 0;
		vSamplingAuxiliaryDetector[TempLoop] = 0;
		vLastIndexPulseWasCorrect[TempLoop] = 0;
		vMostRecentTimeBeforeIndexPulse[TempLoop] = 0;
		vMostRecentTimeAfterIndexPulse[TempLoop] = 0;
		vSamplingLastProcessTime[TempLoop] = 0;
		vHeadCountAfterIndexPulse[TempLoop] = 0;

		vSampleIndexErrorsX[TempLoop] = 0;
		vSampleIndexErrorsY[TempLoop] = 0;
	}
	vSamplingSeamerObjectPulseCounter = 0;
	vSamplingSeamerPocketPulseCounter = 0;

	vSamplingName[0] = "Random";
	vSamplingName[1] = "Filler";
	vSamplingName[2] = "Seamer";

	vManuallyEditedDuringEvaluate = false;
	vSmallestDifferenceBetweenAverageAndMinimum = 255;
	vPipeBrightnessCount = 0;
	vCurrentPipeStatus = cNoPipeStatus;
	vAirBubblesInPipe = 0;
	vOldAirBubblesInPipe = 0;
	vPipeIsEmpty = 0;

	vNeedToVerifyDACValues = false;
	vVerifyDACErros = 0;
	vDACTestCount = 0;
	vADCTestCount = 0;
	vVerifyDACValues = 0;
	vGenerateDACVerifyReport = false;
	vVerifyADCValues = 0;
	vOldVerifyDACValues = 0;
	vOldVerifyADCValues = 0;
	for (BYTE TempLoopD = 0; TempLoopD < cMaximumNumberOfDetectors12; TempLoopD++)
	{
		vLatestDACValueRead[TempLoopD] = 0;
		vLastProgrammedDACDetectorOffset[TempLoopD] = 0;
		vLatestADCValueRead[TempLoopD] = 0;
		vLastProgrammedADCDetectorOffset[TempLoopD] = 0;
	}
	vSmallestDifferenceBetweenAverageAndMinimum = 255;
	vFPGADontCorrectForEncoderErrors = 0;
	vCheckWeighOffsetCalibrationNeeded = false;
	vStartRunningAfterLoadProduct = false;
	vDetectorCalibrationNeeded = false;
	vWaitingForDACValueReturn = false;
	vWaitingForADCValueReturn = false;
	vSupportContractTypeStrings[0] = "Support Contract Invalid";
	vSupportContractTypeStrings[1] = "Support Contract 8/5";
	vSupportContractTypeStrings[2] = "Support Contract 16/5";
	vSupportContractTypeStrings[3] = "Support Contract 24/7";
	vSupportContractTypeStrings[4] = "Hide Support Contract";
	vSupportContractTypeStrings[5] = "No Support Contract";
	vDACErrorCount = 0;
	vDACTestValue = 0x0ABC;
	vDACTestErrorCount = 0;
	vDACTestValueSent = 0x0ABC;
	vDACChannelWrote = 0;
	//vCollectingImagesToEvaluate = 0;
	vEvaluateAndOrQuickLearn = cQuickLearnOnly;
	vCompletedEvaluation = false;
	vNeedToAdjustEjector = 0;
	vSendDACValues = 0;
	vADCErrorCount = 0;
	vADCTestValue = 0x0ABC;
	vADCTestErrorCount = 0;
	vADCTestValueSent = 0x0ABC;
	vADCChannelWrote = 0;
	vSendADCValues = 0;
	vABPixelAtOtherSide = false;
	vContainerString = "Container";
	vCPUTemperaturesReadOK = false;		// read flag for CPU temperature read okay
	vCPUTemperature = 38;				// Cpu temperature
	vF_TemperatureType = true;			// Cpu temperature in celsius, true or false
	TempCPUIndex = 0;
	UseCPUAverage = false;
	TemperatureCPUAverage = 38;

	vGreenPen.CreatePen(PS_SOLID, 1, cGreen); 
	vMediumDarkGreenPen.CreatePen(PS_SOLID, 1, cMediumDarkGreen); 
	vYellowPen.CreatePen(PS_SOLID, 1, cYellow); 
	vRedPen.CreatePen(PS_SOLID, 1, cRed); 
	vBlackPen.CreatePen(PS_SOLID, 1, cBlack); 
	vLightGreyPen.CreatePen(PS_SOLID, 1, cLightGray); 
	vGreyPen.CreatePen(PS_SOLID, 1, cGray); 
	for (BYTE TempLoop = 0; TempLoop <= cMaximumNumberOfInspections; TempLoop++)
	{
		vSensitivities[TempLoop] = 0xFF;
		vThresholdSensitivities[TempLoop] = 0xFF;
		vLearnSensitivities[TempLoop] = 0xFF;
	}
	vNumberOfSensitivities = 0;
	
	vEjectTooLateMessageGivenThisRun = false;
	vOldSetupPasswordRequiredToRun = false;
	//vTestingOnlyStopImages = false;
	vNoImageCounter = 0;
	vRunningDiagnosticsMode = false;
	vSimulatingCamLinkImage = false;
	vWorkStopTimePeriod = 60;
	vPercentOfTimeForProcessing = 0;
	vPercentOfTimeBeforeEject = 0;
	vAllSystemSettingsAreStandard = true;
	vInspxMessagesToView = false;
	vOldAllSystemSettingsAreStandard = true;
	vOldInspxMessagesToView = false;
	vOldStylePowerMoniter = true;
	vNonStandardSetting = "";

	vBIOSVersion = "Not Read";
	vBIOSDate = "Not Read";
	vVideoBIOSDate = "Not Read";
	vVideoBIOSVersion = "Not Read";

	vDisplayingAnEncoderError = false;
	vEncoderFaultyCount = 0;
	vGaveEncoderErrorThisRun = false;
	vHadABrownOut = false;
	//vHadAnACPowerDropOut = false;
	vHadAPowerMoniterMessageError = 0;	//power moninter flags
		//4096 AC Voltage Sag <106
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
	vLastEjectTime = "";
	vRunStartTime = "--";
	vTimeStarted = 0;
	vOperatorPasswordOK = false;
	vConditionString = "Condition";
	vRegionalLocale = "United States";
	vImprovingLearn = false;
	vAdjustingBrightnessBeforeLearning = false;
	vCurrentBrightnessAdjustment = 0;
	vWrongWidthsDuringLearn = 0;
	vWrongBrightnessDuringLearn = 0;
	vRejectsDuringLearn = 0;
	vACPowerOn = true;
	vSimulatingFastestEncoder = false;
	vEncoderSampleIndex = 0;
	for (WORD TempLoop = 0; TempLoop < cNumberOfEncoderSamples; TempLoop++)
		vEncoderSample[TempLoop] = 0;

	vAirConditionerTemperatureSampleIndex = 0;
	vAirConditionerTemperatureSampleCount = 0;
	for (WORD TempLoop = 0; TempLoop < cNumberOfAirConditionerTemperatureSamples; TempLoop++)
		vAirConditionerTemperatureSampleData[TempLoop] = 0;

	vRedLampBrokenCounter = 0;
	vYellowLampBrokenCounter = 0;
	vBlueLampBrokenCounter = 0;
	vMessageToMainMenu = 0;
	vMessageToSubMenu = 0;
	vMessageToMainMenuParameter[0] = 0;
	vMessageToMainMenuParameter[1] = 0;

	vMissingRAM = false;
	vShutterFailedLastTest = false;
	vProductHasAPoorLearn = false;
	vAirConditionerFailed = false;
	vWeightTrendingAlarm = false;
	vProductHasAnOldInspection = false;
	vDXMFilamentLimit = 1;
	vuCBoardAnalogValueReadCount = 0;
	vNotCommissioned = true;
	vRemoveBackgroundInAutoSize = false;
	vConveyorEncoderRate = 0;
	vActualEncoderRate = 0;

	vOldActualEncoderRate = 0;
	vLastReportedEncoderRate = 0;
	vBeltRate = 0;
	vOldBeltRate = 0;
	vContainerRate = 0;
	vOldContainerRate = 0;
	vPercentBeltSpeed = 0;
	vGaveConveyorTooFastWarning = 0;
	vCheckConveyor = 0;

	vActualEncoderRateMaximum = 0;
	vBeltRateMaximum = 0;
	vContainerRateMaximum = 0;
	vPercentBeltSpeedMaximum = 0;

	vWriteCalibrationImages = false;
	vPixelValuesString = "";
	vPixelValuesCorrectedPixels = false;
	vShowingShuttingDownIndicator = false;
	vKeepDensityFeedbackLoopLocked = true;
	vFullDetectorCalibration = false;
	vGenerateTestError = 0;
	vCantInspectError = 0;
	vOKToCloseProgram = false;
	vXRayTubeConditionStepTime = 0;
	vXRayTubeConditionCurrentStep = 0;

	vLastCalibrationTime = 0;
	vLastTickleTime = 0;
	//vInLowExposureMode = false;
	vPreviousProduct = "";
	//vReadyForImage = false;
	vWaitingForImage = 0;
	vTimesRetried = 0;
	vGoodImagesIndex = 0;
	vHaveAutoDisabledEjectors = false;
	vReCalibrationTimerRunning = false;
	vConveyorIsOn = false;
	vInspectionNameChanged = false;
	vInspectionNameMoved = false;
	vAutoCalibrate = false;
	vAutoCalibrateWhileRunning = false;
	vCalibrateUsingDAC = false;
	vCalibrateADCAfterDAC = false;
	vNumberOfXRayDetectorsUsing = 6;
	vCalibrationImageNumber = 0;
	vCalibrateGainIterationNumber = 0;
	vFindingProductBrightness = 0;
	vCalibrationSimulationStarted = false;
	vCalibrateErrorCount = 0;
	vHadErrorTryRestart = 0;
	vCalibrationOffsetRange = 0;
	for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
		vCalibrationOffsetCurrentOffset[TempLoop] = 0;
	for (BYTE TempLoop = 0; TempLoop < cMaximumRejectReason; TempLoop++)
		vCountToUseForEjectReason[TempLoop] = 0;

	vTakingQuickLearnImages = false;
	vQuickLearnImageIndex = 0;
	for (WORD TempLoop = 0; TempLoop < cMaximumQuickLearnImages; TempLoop++)
	{
		vQuickLearnImages[TempLoop] = NULL;
		vQuickLearnWorkingImages[TempLoop] = NULL;
	}

	vMemoryOK = true;
	vLastTimeSinceMinimumGood = 0;
	vImageCounter = 0;
	vTriggerCounter = 0;
	vTriggerEndCounter = 0;
	vStartImageCounter = 0;
	vEndImageCounter = 0;
	vProcessImageCounter = 0;
	vTestingTrigger = false;
	vBodyTriggerWidthAverage = 0;
	vBodyTriggerWidthStandardDeviation = 0;
	vBodyTriggerWidthIndex = 0;
	vBodyTriggerWidthSamples = 0;

	//vGaveBodyTriggerWarning = 0;
	for (WORD TempLoop = 0; TempLoop < cMaximumBodyTriggerWidthData; TempLoop++)
		vBodyTriggerWidthData[TempLoop] = 0;
	for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfDemoImages; TempLoop++)
		vDemoImage[TempLoop] = NULL;
	vNumberOfDemoImages = 0;

	vIncludeRejectsInAverageAndStructure = true;
	//vLastTextFileDirectory = cLogDirectory;
	vNumberOfShapeInspections = 0;
	vTotalNumberOfInspections = 0;
	vRadiationDoorManualControlRegister = 0;
	vRadiationDoorManualControlCurrentStatus = 0xFF;
	vRadiationDoorInDoorDelaySteps = 0;
	vRadiationDoorOutDoorDelaySteps = 60;
	vRadiationDoorEncoderDividerFactor = 64;
	vRadiationDoorDwellDistance = 320;
	vDiverterOn = false;
	vLVDSErrorCount = 0;
	vMissedErrorMessages = false;
	/*
	vErrorMessageHeadIAT = 0;
	vErrorMessageTailIAT = 0;
	vErrorMessageHeadST = 0;
	vErrorMessageTailST = 0;
	//vErrorStringCollectionFlag = 0;

	for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfErrorMessages; TempLoop++)
	{
		vTheErrorMessageStringIAT[TempLoop] = 
			"01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";
		vTheErrorMessageTypeIAT[TempLoop] = 0;
		vTheErrorMessageStringST[TempLoop] =
			"01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";
		vTheErrorMessageTypeST[TempLoop] = 0;
	}
	*/

	vDriftCompensationThresholdReductionAmount = 0;
	vOldDriftCompensationThresholdReductionAmount = 0;
	vDriftCompensationThresholdIncreaseAmount = 0;
	vOldDriftCompensationThresholdIncreaseAmount = 0;
	vDriftCompensationImageBrightnessDelta = 0;
	vOldDriftCompensationImageBrightnessDelta = 0xFFFF;
	vLastXRayCurrentSetTime = 0;
	vDriftCompensationADCOffset = 0;
	vDriftCompensationDACOffset = 0;
	vOldDriftCompensationADCOffset = 999999;
	vOldDriftCompensationDACOffset = 999999;
	vNumberOfAuxiliaryDetectorsConfigured = 0;
	vMaximumExternalDetectorTime = 0;
	vLastExternalDetectorUsed = 0;
	vExtraWork = 0;
	vuCHealthFactor = 8;
	vOutstandingAuxiliaryDetectorEnables = 0;
	vAutoSetupAuxiliaryDetectorNumber = 0;
	vLastAutoSetupAuxiliaryDetectorActivePosition = 0;
	vLastAutoSetupAuxiliaryDetectorInActivePosition = 0;
	vOutstandingEjects = 0;
	vEjectorsToFireNow = 1;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
		vTestEjectDwellTime[TempLoop] = 300;
	vShowImages = true;
	vShuttingDownXRays = false;
	vAdjustingSourceSettings = false;
	vThisContainerShouldBeEjected = false;
	vOldThisContainerShouldBeEjected = 0;
	vTweakReferenceAdjust = 0;
	vTweakTopReferenceAdjust = 0;
	vLastEMailErrorMessageTime = 0;
	vLastErrorMessage = "None";
	vYellowMessageButtonYellow = false;
	vButton2ShowsError = false;
	vRampXRayWaitTime = 500;
	vOldOpenInterlock = "";
	vOpenInterlock = "";
	vTemporarilyChangeSource = false;
	vChangedSizeOfProduct = false;
	vSetupANewProduct = false;
	vChangedSizeOfProductCheckDensityROI = 0;
	vInAutoSetup = false;
	vInExpressSetup = false;
	vManualReLearnWhileInspecting = false;
	vLastReport = "None Generated";
	vReportingPeriodStartTime = 0;

	CTime TempTime;
	TempTime = CTime::GetCurrentTime();
	CString TempTimeString = TempTime.Format("%#I:%M:%S%p, %B %d, %Y");
	vErrorLogStarted = TempTimeString;
	vErrorLog.RemoveAll();
	vErrorCount = 0;
	vInspxLog.RemoveAll();
	vErrorCountForLogFile = 0;
	vImageTracerLineOn = false;
	vNumberOfVoltageSamples = 0;
	vNumberOfDataSamples = 0;
	vEject4InARowCount = 0;
	vTestEjectWaitCount = 0;
	vCurrentLampStatus = 7;
	vCurrentEjectorOnStatus = 0;
	vCurrentCanStopStatus = 0;
	vCurrentAlarmStatus = 0;
	vHaveABackup = 0;
	vBackupCanStop = 1;
	vDoingBackupDetection = false;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
	{
		vLastBackupDetectorChangePosition[TempLoop] = 0;
		vBackupDetectorOn[TempLoop] = 0;
		vAuxiliaryDetectorIndex[TempLoop] = 0;
	}

	vCurrentProductChanged = false;
	vGivenTemperatureWarning = false;
	vGivenFilterCloggedWarning = false;
	vGaveTooManyRejectsAlarm[0] = false;
	vTooManyRejectsIndex[0] = 0;
	vGaveTooManyRejectsAlarm[1] = false;
	vTooManyRejectsIndex[1] = 0;
	vGaveTooManyRejectsAlarm[2] = false;
	vTooManyRejectsIndex[2] = 0;
	for (WORD TempLoopW = 0; TempLoopW < cMaximumEjectDataSize; TempLoopW++)
	{
		vContainerEjectData[TempLoopW][0] = false;
		vContainerEjectData[TempLoopW][1] = false;
		vContainerEjectData[TempLoopW][2] = false;
	}

	vSendingGoodWithBad = false;
	vMaximumBadCount = 0;
	vMaximumBadData = NULL;
	vMinimumGoodData = NULL;
	vMaximumBadDataFloat = NULL;
	vMinimumGoodDataFloat = NULL;
	vMinimumGoodIndex = 0;
	vMinimumGoodStandardDeviation = 0;
	vMinimumGoodAverage = 0;
	vMaximumBadIndex = 0;
	vMaximumBadStandardDeviation = 0;
	vMaximumBadAverage = 0;
	vQualityFactor = 0;

	vMaximumBadIntensity = 0;
	vMinimumGoodIntensity = 256;

	vDensityLastValue = 0;
	//vDensityData = NULL;
	vDensityDataD = NULL;
	for (WORD TempLoopW = 0; TempLoopW < cMaximumNumberOfDetectors12; TempLoopW++)
		vDetectorDensityData[TempLoopW] = NULL;
	vDensityIndex = 0;

	vLoadedFileImage = NULL;
	vLoadedFileAvailable = false;
	vuCMainLoopCounter = 1;
	vPostedNewMainImageReadyToDisplayMessage = false;

	vPosteduCVoltageProblemMessage = 0;
	vLastEncoderCountToCalculateEncoderRate = 0;
	vuCPoweredUp = false;
	vRampXRayCurrentAmount = 0;
	vRampXRayVoltageAmount = 0;
	vSourceRampedUp = false;
	vSourceErrorReported = false;
	vUControllerReady = false;
	vWaitingForUControllerReady = false;
	vCompletelyInitialized = false;
	//vOlduCSerialPort = NULL;
	vTopReferenceSample = NULL;
	vRightReferenceSample = NULL;
	vLeftReferenceSample = NULL;
	vBottomReferenceSample = NULL;

	vCommunicationsTimeOutCounter = 254;
	vITIPCDig = NULL;
	vRampXRayStep = 0;
  vFrameCount = 0;
	vReceivedACalibrationImage = false;
	vSeqNum = 0;
	vDetectorModeString[0] = "Disabled";
	vDetectorModeString[1] = "1: Reject In Window";
	vDetectorModeString[2] = "2: Accept In Window";
	vDetectorModeString[3] = "3: Reject At Start";
	vDetectorModeString[4] = "4: Accept At Start";
	vDetectorModeString[5] = "5: Backup Detector";
	vDetectorModeString[6] = "6: Filler Index";
	vDetectorModeString[7] = "7: Seamer Index";
	vDetectorModeString[8] = "8: Seamer Object";
	vDetectorModeString[9] = "9: Seamer Pocket";
	vDetectorModeString[10] = "10: Sample Eject Next Container";

	vMaximumEncoderRate = 3333;
	vLastPattern = 0;
	//vSourceCurrentSetting = 0;
	vXRayOnNotice = false;
	vXRayState = 0;
	vImageNumberToDisplay = 0;
	vGaveNoXRayMessage = false;
	vLastBodyTriggerLength = 0;
	vLastBodyTriggerGapLength = 0;
	vKeepExternalDetectorsEnabled = false;
	vEjectConfirmerUsed = false;
	vuControllerSendExtraDiagnosticMessages = false;
	vCountTimeAsRunning = false;
	vSecondsSinceLastContainer = 0;

	for (BYTE TempLoop = 0; TempLoop < cMaxCPMTimePeriod; TempLoop++)
	{
		vCPMData[TempLoop] = 0;
		vCPMDataTime[TempLoop] = 0;
		vCPMDataNotRunning[TempLoop] = 0;
	}
	vCPMDataIndex = 0;
	vHaveAllCPMData = false;
	vCPMDataIndexNotRunning = 0;

	vTestEjectNextContainer = 0;

	//vContainerBeingImaged = NULL;
	vLastContainerImaged = NULL;
	vRawInterlockInput = 0;
	vRawExternalDetectorInput = 0;
	vRawBodyTriggerInput = 0;
	vADRange13 = false;
	vADInvert = false;
	vFPGASimulateEncoderRate = 0;
	vSimulateContainersRate = 0;
	vSecondsMeasuringRealConveyorEncoder = 0;
	vRealConveyorEncoderPulsesCount = 0;
	vSimulatedContainerTimerHandle = 101;
	vSimulatedContainerTimerDelay = 0;
	vCheckACBeforeThisYellowMessage = "";
	vuCSimulate16Hz = false;
	vuCOutputSerialDebugData = false;
	vEnableduC16HzInterrupt = true;
	vFPGAGeneratingSimulatedData = false;

	vNoCommunicationsWithUController = true;
	vOldNoCommunicationsWithUController = false;
	vCaptureDiagnosticData = false;
	vDiagnosticDataReadTimerHandle = 27;
	for (WORD TempLoopW = 0; TempLoopW < cDiagnosticLineArraySize*10; TempLoopW++)
		vDiagnosticData[TempLoopW] = 0;
	vNextDiagnosticArrayEntry = 0;
	vDiagnosticLineValue = 0;

	vCurrentBodyTriggerMask = 0;
	vOnlyOneBodyTriggerEnabled = 0;
	vDigitalInputLine3Mask = 7;
	vBackupBodyTriggerMask = 0;
	vCurrentExternalDetectorMask = 0;
	vCurrentExternalDetectorActiveLowMask = 0;
	vCurrentBodyTriggerActiveLowMask = 0;
	vBackupBodyTriggerLines = 0;
	vOldBackupBodyTriggerLines = 0;
	vBodyTriggerLines = 0;
	vOldBodyTriggerLines = 0;

	vBackupBodyTriggerVerifyFlag = 0;

	vShiftProductCode = " ";
	vPreviousShiftProductCode = "";
	vLastChatWindowText = "";
	vSimulating = false;
	vCurrentProductName = "No Current Product";
  vSystemRunMode = cStoppedSystemMode;
	vModeToRunAfterCalibrateOffset = cStoppedSystemMode;
	vMainDisplayMode = cMainDisplayBooting;
	SimEjectType = 0;  //is a zero based number
	vGoodCountTotal = 0;
	vGoodCountInBufferTotal = 0;
	vRealImageCountTotal = 0;
	vEjectCountTotal = 0;
	vRejectCountTotal = 0;
	vOldRejectCountTotal = 0;
	vProductionStopsCount = 0;
	vStopDuration = 0;
	vTimeSinceLastStop = 0;
	vRunDuration = 0;
	vCheckWeighRunDuration = 0;
	vTotalDuration = 0;
	vTotalContainers = 0;
	vImproveLearnTotalContainers = 0;
	vTotalConfirmed = 0;
	vTotalUnConfirmed = 0;
	vShiftRateCPM = 0;
	vCurrentConveyorOutput = 0;
	vOldShiftRateCPM = 0;

	vOldGoodCountTotal = 0;
	vOldEjectCountTotal = 0;
	vOldMissedEjectsBecauseDisabledCount = 0;
	for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfCountsDisplayed; TempLoop++)
	{
		vOldXRayCriteriaCount[TempLoop] = 0;
		vXRayCriteriaCount[TempLoop] = 0;
		vXRayCriteriaCountTotal[TempLoop] = 0;
		vOldXRayCriteriaCountButDisabled[TempLoop] = 0;
		vXRayCriteriaCountButDisabled[TempLoop] = 0;
	}
	vOldProductionStopsCount = 0;
	vOldStopDuration = 0;
	vOldTimeSinceLastStop = 0;
	vOldRunDuration = 0;
	vOldTotalContainers = 0;
	vOldTotalConfirmed = 0;
	vOldTotalUnConfirmed = 0;
	vProcessingErrorCount = 0;
	vTooLateToEjectErrorCount = 0;
	vMissedEjectsBecauseDisabledCount = 0;

	vEjectorCount[0] = 0;
	vEjectorCount[1] = 0;
	vEjectorCount[2] = 0;
	vEjectorConfirmCount[0] = 0;
	vEjectorConfirmCount[1] = 0;
	vEjectorConfirmCount[2] = 0;
	vBodyTriggersCount = 0;
	vBackupBodyTriggersCount = 0;
	vValidBodyTriggersCount = 0;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
		vExternalDetectorCount[TempLoop] = 0;

	vIncompleteDataCount = 0;
	vImageReferenceErrorCount = 0;

	vPercentRejected = 0;
	vOldPercentRejected = 0;
	vPercentEjected = 0;
	vOldPercentEjected = 0;
	vOldCurrentBeltPosition = 1;
	vCurrentBeltPosition = 1;
	vCurrentEstimatedBeltPosition = 0;

	vLastXRayCommandOn = false;
	vSourceVoltage = 0;
	vSourceCurrent = 0;

	vSourceVoltageMinRedLine = 0;
	vSourceVoltageMaxRedLine = 10;
	vSourceCurrentMinRedLine = 0;
	vSourceCurrentMaxRedLine = 10;

	vSourceVoltageMin = 254;
	vSourceCurrentMin = 254;
	vSourceVoltageMax = 0;
	vSourceCurrentMax = 0;

	vuCLEDGYR = 0;
	v5 = 0xFFFF;//180;
	v33 = 0xFFFF;//180;
	v25 = 0xFFFF;//180;
	v12 = 0xFFFF;//180;
	vMinus12 = 0xFFFF;//180;
	v15 = 0xFFFF;//180;

	vDataIndex = 0;
	vXRayStatusIndex = 0;
	for (WORD TempLoop = 0; TempLoop < cNumberOfVoltageSamples; TempLoop++)
	{
		vSourceVoltageReading[TempLoop] = 0;
		vSourceCurrentReading[TempLoop] = 0;
	}
	vTemperatureSurfaceMaximum = 0xFFFF;  //large value for low temperatures
	vTemperatureLiquidMaximum = 0xFFFF;
	vTemperatureuCMaximum = 0xFFFF;
	vTemperatureAirMaximum = 0xFFFF;
	vTemperatureAirOutsideMaximum = 0xFFFF; 
	vCPUTemperatureMaximum = 0;

	vTemperatureSurfaceMinimum = 0;
	vTemperatureLiquidMinimum = 0;
	vTemperatureuCMinimum = 0;
	vTemperatureAirMinimum = 0;
	vTemperatureAirOutsideMinimum = 0;  
	vCPUTemperatureMinimum = 0xFFFF;  

	vTemperatureSurface = 0xFFFF;
	vTemperatureLiquid = 0xFFFF;
	vTemperatureuC = 0xFFFF;
	vTemperatureAirInside = 0xFFFF;
	vTemperatureAirOutside = 0xFFFF; 
	vCPUTemperature = 0xFFFF; 

	vPSFault = 0;
	vSpareMonitor = 0;

	vMax5 = 0;
	vMax33 = 0;
	vMax25 = 0;
	vMax12 = 0;
	vMaxMinus12 = 0;
	vMax15 = 0;
	vMin5 = 0xFFFF;
	vMin33 = 0xFFFF;
	vMin25 = 0xFFFF;
	vMin12 = 0xFFFF;
	vMinMinus12 = 0xFFFF;
	vMin15 = 0xFFFF;

	vSourceInterLocks = 0;
	vPreviousInterlockValue = 0;
	vLastInterlockChangedTime = 0;
	vInterlock200mSecChangeCount = 0;

	//vHVPSInterlockDelay = 0;
	vOldSourceInterLocks = 0xFFFF;

	vuCWatchDogCount = 0;
	vuCResetCount = 0;
	vGaveQueueOverflowMessage = false;
	vuCMajorVersionNumber = 0;
	vuCMinorVersionNumber = 0;
	vFPGAMajorVersionNumber = 0;
	vFPGAMinorVersionNumber = 0;
	vFPGAVersion8Point7OrHigher = true;
	vFPGAVersion9Point0OrHigher = true;
	vFPGAVersion9Point1OrHigher = true;
	vFPGAVersion9Point2OrHigher = true;
	vFPGAVersion10Point7OrHigher = true;
	vFPGAVersion10Point86OrHigher = true;
	//vFPGAVersion10Point9OrHigher = true;
	vFPGASelfTest = 0;
	vCpuUsage = 0;

	vCurrentBeltPosition = cMaxBeltPositionOffset;
	vCurrentColor = cBlack;
  vGreenBrush = ::CreateSolidBrush(cGreen);
	vYellowBrush = ::CreateSolidBrush(cYellow);
	vWhiteBrush = ::CreateSolidBrush(cWhite);
	vGrayBrush = ::CreateSolidBrush(cGray);
	vBlueBrush = ::CreateSolidBrush(cBlue);
	vDarkBlueBrush = ::CreateSolidBrush(cDarkBlue);
 	vLightBlueBrush = ::CreateSolidBrush(cLightBlue);
	vLightGreenBrush = ::CreateSolidBrush(cLightGreen);
  vRedBrush = ::CreateSolidBrush(cRed);
  vBlackBrush = ::CreateSolidBrush(cBlack);
  vLightRedBrush = ::CreateSolidBrush(cLightRed);
  vLightBrownBrush = ::CreateSolidBrush(cLightBrown);
  vPlotBackGroundBrush = ::CreateSolidBrush(cVeryLightGray);
	
	vFontSelection = 0;

	vColorArray[0] = cLightRed;
	vColorArrayNames[0] = _T("cLightRed");
	vColorArrayBrush[0] = CreateSolidBrush(cLightRed);

	vColorArray[1] = cRed;
	vColorArrayNames[1] = _T("cRed");
	vColorArrayBrush[1] = CreateSolidBrush(cRed);

	vColorArray[2] = cDarkRed;
	vColorArrayNames[2] = _T("cDarkRed");
	vColorArrayBrush[2] = CreateSolidBrush(cDarkRed);

	vColorArray[3] = cLightPink;
	vColorArrayNames[3] = _T("cLightPink");
	vColorArrayBrush[3] = CreateSolidBrush(cLightPink);

	vColorArray[4] = cPink;
	vColorArrayNames[4] = _T("cPink");
	vColorArrayBrush[4] = CreateSolidBrush(cPink);

	vColorArray[5] = cDarkPink;
	vColorArrayNames[5] = _T("cDarkPink");
	vColorArrayBrush[5] = CreateSolidBrush(cDarkPink);

	vColorArray[6] = cLightYellow;
	vColorArrayNames[6] = _T("cLightYellow");
	vColorArrayBrush[6] = CreateSolidBrush(cLightYellow);

	vColorArray[7] = cYellow;
	vColorArrayNames[7] = _T("cYellow");
	vColorArrayBrush[7] = CreateSolidBrush(cYellow);

	vColorArray[8] = cDarkYellow;
	vColorArrayNames[8] = _T("cDarkYellow");
	vColorArrayBrush[8] = CreateSolidBrush(cDarkYellow);

	vColorArray[9] = cLightGreen;
	vColorArrayNames[9] = _T("cLightGreen");
	vColorArrayBrush[9] = CreateSolidBrush(cLightGreen);

	vColorArray[10] = cGreen;
	vColorArrayNames[10] = _T("cGreen");
	vColorArrayBrush[10] = CreateSolidBrush(cGreen);
	vColorArray[11] = cDarkGreen;
	vColorArrayNames[11] = _T("cDarkGreen");
	vColorArrayBrush[11] = CreateSolidBrush(cDarkGreen);
	vColorArray[12] = cLightBlue;
	vColorArrayNames[12] = _T("cLightBlue");
	vColorArrayBrush[12] = CreateSolidBrush(cLightBlue);
	vColorArray[13] = cBlue;
	vColorArrayNames[13] = _T("cBlue");
	vColorArrayBrush[13] = CreateSolidBrush(cBlue);
	vColorArray[14] = cDarkBlue;
	vColorArrayNames[14] = _T("cDarkBlue");
	vColorArrayBrush[14] = CreateSolidBrush(cDarkBlue);
	vColorArray[15] = cBlack;
	vColorArrayNames[15] = _T("cBlack");
	vColorArrayBrush[15] = CreateSolidBrush(cBlack);
	vColorArray[16] = cWhite;
	vColorArrayNames[16] = _T("cWhite");
	vColorArrayBrush[16] = CreateSolidBrush(cWhite);
	vColorArray[17] = cLightGray;
	vColorArrayNames[17] = _T("cLightGray");
	vColorArrayBrush[17] = CreateSolidBrush(cLightGray);
	vColorArray[18] = cGray;
	vColorArrayNames[18] = _T("cGray");
	vColorArrayBrush[18] = CreateSolidBrush(cGray);
	vColorArray[19] = cDarkGray;
	vColorArrayNames[19] = _T("cDarkGray");
	vColorArrayBrush[19] = CreateSolidBrush(cDarkGray);
	vColorArray[20] = cOlive;
	vColorArrayNames[20] = _T("cOlive");
	vColorArrayBrush[20] = CreateSolidBrush(cOlive);
	vColorArray[21] = cLightPurple;
	vColorArrayNames[21] = _T("cLightPurple");
	vColorArrayBrush[21] = CreateSolidBrush(cLightPurple);
	vColorArray[22] = cPurple;
	vColorArrayNames[22] = _T("cPurple");
	vColorArrayBrush[22] = CreateSolidBrush(cPurple);
	vColorArray[23] = cDarkPurple;
	vColorArrayNames[23] = _T("cDarkPurple");
	vColorArrayBrush[23] = CreateSolidBrush(cDarkPurple);
	vColorArray[24] = cBrown;
	vColorArrayNames[24] = _T("cBrown");
	vColorArrayBrush[24] = CreateSolidBrush(cBrown);
	vColorArray[25] = cOrange;
	vColorArrayNames[25] = _T("cOrange");
	vColorArrayBrush[25] = CreateSolidBrush(cOrange);

	//vCurrentPunyFont.DeleteObject();
	//vCurrentPunyFont.CreatePointFont(cPunyFont,cThickFont,NULL);
	//vCurrentTinyFont.DeleteObject();
	//vCurrentTinyFont.CreatePointFont(cTinyFont,cThickFont,NULL);
	//vCurrentSmallFont.DeleteObject();
	//vCurrentSmallFont.CreatePointFont(cSmallFont,cThickFont,NULL);
	//vCurrentMediumFont.DeleteObject();
	//vCurrentMediumFont.CreatePointFont(cMediumFont,cThickFont,NULL);

	//vUnderlinedFont.DeleteObject();
	//vUnderlinedFont.CreateFont(cMediumLargeFont, 0, 0, 0, FW_BOLD, 0, true, 0, DEFAULT_CHARSET, 0, 0, 0, 0, cThickFont); //was 40

	//vCurrentMediumLargeFont.DeleteObject();
	//vCurrentMediumLargeFont.CreatePointFont(cMediumLargeFont,cThickFont,NULL);
	//
	//vCurrentLargeFont.DeleteObject();
	//vCurrentLargeFont.CreatePointFont(cLargeFont,cThickFont,NULL);

	//vCurrentExtraLargeFont.DeleteObject();
	//vCurrentExtraLargeFont.CreatePointFont(cExtraLargeFont,cThickFont,NULL);

	//vCurrentHugeFont.DeleteObject();
	//vCurrentHugeFont.CreatePointFont(cHugeFont,cThickFont,NULL);

	//vCurrentExtraHugeFont.DeleteObject();
	//vCurrentExtraHugeFont.CreatePointFont(cExtraHugeFont,cThickFont,NULL);

	for (WORD TempLoop = 0;TempLoop < 3; TempLoop++)
		vLastCommTestBytes[TempLoop] = 0;

	vRampXRayCurrentIncrement = 0;
	vRampXRayVoltageIncrement = 0;
	vFrameCountString = "0";
	vContainerCollection = NULL;
	vApplyGainAndOffsetDuringCalibration = false;

	vTouchScreen = false;

	vDataIndex = 0;
	for (WORD TempLoop = 0; TempLoop < cNumberOfVoltageSamples; TempLoop++)
	{
		v5Data[TempLoop] = 0;
		v33Data[TempLoop] = 0;
		v25Data[TempLoop] = 0;
		v12Data[TempLoop] = 0;
		vMinus12Data[TempLoop] = 0;
		v15Data[TempLoop] = 0;
		vTemperatureSurfaceData[TempLoop] = 0;
		vTemperatureLiquidData[TempLoop] = 0;
		vTemperatureuCData[TempLoop] = 0;
		vTemperatureAirInsideData[TempLoop] = 0;
		vTemperatureAirOutsideData[TempLoop] = 0;
		vPSFaultData[TempLoop] = 0;
		vSpareMonitorData[TempLoop] = 0;
		vSourceVoltageReading[TempLoop] = 0;
		vSourceCurrentReading[TempLoop] = 0;
		vTemperatureCPUData[TempLoop] = 0;
	}
	vLastCommTestBytes[0] = 0;
	vLastCommTestBytes[1] = 0;
	vLastCommTestBytes[2] = 0;
	vEjectorUsed = 0;
	vExternalDetectorDecisionEnd = 0;
	vExternalDetectorDecisionStart = 0;
	vCurrentAlwaysOnExternalDetectorMask = 0;
	vXRayStatusIndex = 0;
	vOldLastDataSent = "None";
	vOldLastDataReceived = "None";
	vFontSelection = 0;
	vDiagnosticLineDataFile = NULL;

	vLastEjectedPosition = 0;
	vLastEjectedEjector = 0;
	vLastBodyTriggerPosition = 0;
	vLastBodyTriggerLineState = 0;
	vLastExternalDetectorPosition = 0;
	vLastExternalDetectorLineState = 0;
	vGivenTemperatureSensorWarning = false;
	vNumberOfUsers = 0;

	vGlobalMonitorSizeX = vGlobalDisplaySizeH;
	vGlobalMonitorSizeY = vGlobalDisplaySizeV;

}

void CScanTracSystemRunningData::CreateReferenceSamples(DWORD TempSize)
{
	FreeReferenceSamples();
	if (ThereIsEnoughMemory(TempSize * sizeof(double),"Top Reference Sample Array"))
		vTopReferenceSample = (double *)malloc(TempSize * sizeof(double));
	if (ThereIsEnoughMemory(TempSize * sizeof(double),"Right Reference Sample Array"))
		vRightReferenceSample = (double *)malloc(TempSize * sizeof(double));
	if (ThereIsEnoughMemory(TempSize * sizeof(double),"Left Reference Sample Array"))
		vLeftReferenceSample = (double *)malloc(TempSize * sizeof(double));
	if (ThereIsEnoughMemory(TempSize * sizeof(double),"Bottom Reference Sample Array"))
		vBottomReferenceSample = (double *)malloc(TempSize * sizeof(double));
}

void CScanTracSystemRunningData::FreeReferenceSamples()
{
	if (vTopReferenceSample != NULL)
	{
		free(vTopReferenceSample);
		vTopReferenceSample = NULL;
	}
	if (vRightReferenceSample != NULL)
	{
		free(vRightReferenceSample);
		vRightReferenceSample = NULL;
	}
	if (vLeftReferenceSample != NULL)
	{
		free(vLeftReferenceSample);
		vLeftReferenceSample = NULL;
	}
	if (vBottomReferenceSample != NULL)
	{
		free(vBottomReferenceSample);
		vBottomReferenceSample = NULL;
	}
}
/*
	vCurrentFont;
	vCurrentLargeFont;
	CLogFile vLogFile;
*/
IMPLEMENT_DYNCREATE(CScanTracSystemRunningData, CObject)

CScanTracSystemRunningData::~CScanTracSystemRunningData()
{
	DeleteObject(vRedBrush);
	DeleteObject(vGreenBrush);
	DeleteObject(vYellowBrush);
	DeleteObject(vWhiteBrush);
	DeleteObject(vGrayBrush);
	DeleteObject(vLightGreenBrush);
	DeleteObject(vBlueBrush);
	DeleteObject(vLightBlueBrush);
	DeleteObject(vDarkBlueBrush);
	DeleteObject(vLightRedBrush);
	DeleteObject(vLightBrownBrush);
}

void CScanTracSystemRunningData::CalculateQualityFactor()
{
	vQualityFactor = 0;
	if (vMaximumBadCount)
	if (vMinimumGoodAverage)
	//if (vMinimumGoodIndex > 2)
	//if (vMaximumBadIndex > 2)
	//if (vMaximumBadAverage)
	if (vMinimumGoodAverage > vMaximumBadAverage)
	{
		double TempBottom = vMinimumGoodStandardDeviation + vMaximumBadStandardDeviation;
		if (TempBottom)
			vQualityFactor = (vMinimumGoodAverage - vMaximumBadAverage) / TempBottom;
		else
			vQualityFactor = (vMinimumGoodAverage - vMaximumBadAverage);

		if (vQualityFactor < 0)
			vQualityFactor = -vQualityFactor;
	}
}

void CScanTracSystemRunningData::FreeQuickLearnImages()
{
	vTakingQuickLearnImages = false;
	vQuickLearnImageIndex = 0;
	for (WORD TempLoop = 0; TempLoop < cMaximumQuickLearnImages; TempLoop++)
	{
		if (vQuickLearnImages[TempLoop])
		{
			free(vQuickLearnImages[TempLoop]);
			vQuickLearnImages[TempLoop] = NULL;
		}
		if (vQuickLearnWorkingImages[TempLoop])
		{
			free(vQuickLearnWorkingImages[TempLoop]);
			vQuickLearnWorkingImages[TempLoop] = NULL;
			if (!(TempLoop % 1000))
			{
				Sleep(1);
			}
		}
	}
}

void CScanTracSystemRunningData::UpdateUserList(tUserCollection& TempNewUserCollection)
{
	//must go through the users, and if any were deleted while editing, you must delete the memory now.
	WORD TempOriginalNumberOfUsers = vUserCollection.GetCount();
	//for (POSITION TempPosition = vUserCollection.GetHeadPosition(); TempPosition; vUserCollection.GetNext(TempPosition))
	//if (TempPosition) 
	//{
	//	UserPasswords* TempUser = NULL;
	//	TempUser = vUserCollection.GetAt(TempPosition);
	//	if (TempUser)
	//	{
	//		POSITION TempFoundPosition = TempNewUserCollection.Find(TempUser);
	//		if (!TempFoundPosition)
	//			delete TempUser;
	//	}
	//}

	vUserCollection.RemoveAll();
	for (POSITION TempPosition = TempNewUserCollection.GetHeadPosition(); TempPosition; TempNewUserCollection.GetNext(TempPosition))
	if (TempPosition) 
	{
		vUserCollection.AddTail(TempNewUserCollection.GetAt(TempPosition));
	}
	vNumberOfUsers = vUserCollection.GetCount();
}

void CScanTracSystemRunningData::StoreUserList()
{
	vNumberOfUsers = vUserCollection.GetCount();
	CFile TempFile;
	CArchive TempArchive(&TempFile, CArchive::store);
	SerializeUserList(TempFile, TempArchive);
}

void CScanTracSystemRunningData::LoadUserList()
{
	CFileException *TempException = NULL;
	TRY
	{
		CFileStatus TempFileStatus;
		CString TempFileName = vGlobalCurrentDirectory + "Users.STR";
		if (CFile::GetStatus(TempFileName, TempFileStatus))
		{
			if (TempFileStatus.m_size > 10)
			{
				CFile TempFile;
				CArchive TempArchive(&TempFile, CArchive::load);
				SerializeUserList(TempFile, TempArchive);
			}
			else
				ReportErrorMessage("Zero length Users.STR File, no users or may be corrupt", cWriteToLog, 0);
		}
	}
	CATCH_ALL(TempException)
	{
	}
	END_CATCH_ALL
	vNumberOfUsers = vUserCollection.GetCount();
}

void CScanTracSystemRunningData::SerializeUserList(CFile &TempFile, CArchive &TempArchive)
{
	CString TempFileName = vGlobalCurrentDirectory + "Users.STR";
	LPTSTR TempUsersFileName = TempFileName.GetBuffer(TempFileName.GetLength());
	if (TempFile.Open(TempUsersFileName, CFile::modeReadWrite | CFile::shareDenyNone))
	{
		CObject::Serialize(TempArchive);
		if (TempArchive.IsStoring())
		{	
			vNumberOfUsers = vUserCollection.GetCount();

			TempArchive << vNumberOfUsers;
			for (POSITION TempPosition = vUserCollection.GetHeadPosition(); TempPosition ; vUserCollection.GetNext(TempPosition))
			if (TempPosition) 
			{
				UserPasswords* TempUser = vUserCollection.GetAt(TempPosition);
				TempArchive << TempUser->vUserName
							<< TempUser->vPassword
							<< TempUser->vID
							<< TempUser->vLevel
							<< TempUser->vBCID
							<< TempUser->vRFID;
			}
		}
		else
		{	
			TempArchive >> vNumberOfUsers;
			//this->ResizeUserList();
			for (int TempLoop = 0; TempLoop < vNumberOfUsers ; TempLoop++)
			{
				UserPasswords* TempUser = new UserPasswords; 
				TempArchive >> (TempUser)->vUserName
							>> (TempUser)->vPassword
							>> (TempUser)->vID
							>> (TempUser)->vLevel
							>> (TempUser)->vBCID
							>> (TempUser)->vRFID;
				InsertAlphabetically(this->vUserCollection, TempUser);
			}
		}
		TempArchive.Close();
		TempFile.Close();
	}
	else
	{
		CString TempFileName = vGlobalCurrentDirectory + "Users.STR";
		LPTSTR TempUsersFileName = TempFileName.GetBuffer(TempFileName.GetLength());

		TempFile.Open(TempUsersFileName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone);
		CArchive TempArchive(&TempFile, CArchive::store);
		CObject::Serialize(TempArchive);
		TempArchive << vNumberOfUsers;
		for (POSITION TempPosition = vUserCollection.GetHeadPosition(); TempPosition ; vUserCollection.GetNext(TempPosition))
		if (TempPosition) 
		{
			UserPasswords* TempUser = new UserPasswords;
			TempUser = vUserCollection.GetAt(TempPosition);
			TempArchive << (TempUser)->vUserName
						<< (TempUser)->vPassword
						<< (TempUser)->vID
						<< (TempUser)->vLevel
						<< (TempUser)->vBCID
						<< (TempUser)->vRFID;
		}
		TempArchive.Close();
		TempFile.Close();
		//ReportErrorMessage("Created new user stream file", cAction,0);
	}
	vNumberOfUsers = vUserCollection.GetCount();
}

IMPLEMENT_SERIAL( CLifeTimeCounters,CObject,VERSIONABLE_SCHEMA|cCurrentArchiveVersionNumber)

CLifeTimeCounters::CLifeTimeCounters()
{
	vArchiveVersionNumber = 0;
	vXRaySourceRunDuration = 0; //keeps track of how many seconds x-ray source
	vDetectorsRunDuration = 0; //keeps track of how many seconds Detectors
	vScanTracRunDuration = 0; //keeps track of how many seconds ScanTrac
	vSpareULongIntLifeCounters1 = 0; 
	vSpareULongIntLifeCounters2 = 0; 
	vSpareULongIntLifeCounters3 = 0; 
	vSpareULongIntLifeCounters4 = 0; 
	vSpareULongIntLifeCounters5 = 0; 
	vSpareULongIntLifeCounters6 = 0; 

	vSpareCStringLifeCounters1 = ""; 
	vSpareCStringLifeCounters2 = ""; 
	vSpareCStringLifeCounters3 = ""; 
	vSpareCStringLifeCounters4 = ""; 
	vSpareCStringLifeCounters5 = ""; 
	vSpareCStringLifeCounters7 = ""; 
	vSpareCStringLifeCounters8 = ""; 
	vSpareCStringLifeCounters9 = "";
	vXRayTubeSerialNumber = "";
	vUsedDisposablePasswords = "";
	vServiceRecordLog = "";
	vCommisionDate = 0;
	vXRaySourceWarrantyDuration = 0;
	vDetectorsWarrantyDuration = 0;
	vXRaySourceWarrantyDate = 0;
	vDetectorsWarrantyDate = 0;
}

CLifeTimeCounters::~CLifeTimeCounters()
{
}

void CLifeTimeCounters::Serialize( CArchive& TempArchive )
{
  // call base class function first
  // base class is CObject in this case
  CObject::Serialize( TempArchive );

  // now do the stuff for our specific class
  if( TempArchive.IsStoring() )
	{
		TempArchive << vXRaySourceRunDuration
			<< vDetectorsRunDuration
			<< vScanTracRunDuration
			<< vXRaySourceWarrantyDuration
			<< vDetectorsWarrantyDuration
			<< vXRaySourceWarrantyDate
			<< vDetectorsWarrantyDate
			<< vSpareULongIntLifeCounters1
			<< vSpareULongIntLifeCounters2
			<< vSpareULongIntLifeCounters3
			<< vSpareULongIntLifeCounters4
			<< vSpareULongIntLifeCounters5
			<< vSpareULongIntLifeCounters6

			<< vUsedDisposablePasswords
			<< vServiceRecordLog
			<< vCommisionDate
			<< vSpareCStringLifeCounters1
			<< vSpareCStringLifeCounters2
			<< vSpareCStringLifeCounters3
			<< vSpareCStringLifeCounters4
			<< vSpareCStringLifeCounters5
			<< vSpareCStringLifeCounters6
			<< vSpareCStringLifeCounters7
			<< vSpareCStringLifeCounters8
			<< vSpareCStringLifeCounters9
			<< vXRayTubeSerialNumber;
	}
  else  //reading archive data
	{
		// may read different versions of object if have them
		// int vVersion = archive.GetObjectSchema();
    vArchiveVersionNumber = TempArchive.GetObjectSchema();

		if (vArchiveVersionNumber > 1) 
		{//version 1
			TempArchive >> vXRaySourceRunDuration
				>> vDetectorsRunDuration
				>> vScanTracRunDuration
				>> vXRaySourceWarrantyDuration
				>> vDetectorsWarrantyDuration
				>> vXRaySourceWarrantyDate
				>> vDetectorsWarrantyDate
				>> vSpareULongIntLifeCounters1
				>> vSpareULongIntLifeCounters2
				>> vSpareULongIntLifeCounters3
				>> vSpareULongIntLifeCounters4
				>> vSpareULongIntLifeCounters5
				>> vSpareULongIntLifeCounters6

				>> vUsedDisposablePasswords
				>> vServiceRecordLog
				>> vCommisionDate
				>> vSpareCStringLifeCounters1
				>> vSpareCStringLifeCounters2
				>> vSpareCStringLifeCounters3
				>> vSpareCStringLifeCounters4
				>> vSpareCStringLifeCounters5
				>> vSpareCStringLifeCounters6
				>> vSpareCStringLifeCounters7
				>> vSpareCStringLifeCounters8
				>> vSpareCStringLifeCounters9
				>> vXRayTubeSerialNumber;
		}  //end archive version 1
	}
	if (vXRayTubeSerialNumber == "")
		vXRayTubeSerialNumber = "Not Entered";
}

//IMPLEMENT_DYNCREATE(CLifeTimeCounters, CObject)

IMPLEMENT_DYNAMIC( CFunctionButton, CStatic )

CFunctionButton::CFunctionButton()
: CStatic()
{
	vArrowOrientation = 0;
	vDoubleArrows = false;
	vNavigationArrow = 0;
}

int CFunctionButton::DestroyWindow()
{
	this->InvalidateRect(&vRectCoor,true);
	return true;
}

void CFunctionButton::SetArrows(BYTE TempArrowOrientation)
{
	vArrowOrientation = TempArrowOrientation;
	vDoubleArrows = false;
}

BYTE CFunctionButton::AreNavigationArrowsSet()
{
	return vNavigationArrow;
}

void CFunctionButton::SetArrows(BYTE TempArrowOrientation, bool TempDoubleArrows)
{
	vArrowOrientation = TempArrowOrientation;
	vDoubleArrows = TempDoubleArrows;
}

void CFunctionButton::ShowNavigationArrow(BYTE TempArrowOrientationAndSize)  //pass null to turn navigation arrow off
{
	if (TempArrowOrientationAndSize <= cDownArrowSmall)
		vNavigationArrow = TempArrowOrientationAndSize;
	else
		vNavigationArrow = NULL;
}

void CFunctionButton::DrawArrows(CPaintDC& TempCDC)
{
	//ASSERT(vArrowOrientation != 0 && vArrowOrientation < 4);
	if (!IsWindowVisible())
		return;
	
	this->GetWindowRect(&vRectCoor);
	CPen TempPen(PS_SOLID, 0,cBlack);
	CBrush TempBrush(cButtonColor);

	LONG TempNewLeftXCoord = vRectCoor.left - 15;
	LONG TempNewRightXCoord = vRectCoor.right + 15;
	LONG TempTopYCoord = vRectCoor.top - 15;
	LONG TempBottomYCoord = vRectCoor.bottom + 15;

/*	POINT Temp2POINTCollection[2];
	Temp2POINTCollection[0].x = vRectCoor.left;
	Temp2POINTCollection[0].y = vRectCoor.top;
	Temp2POINTCollection[1].y = vRectCoor.bottom;
	Temp2POINTCollection[1].x = vRectCoor.right;
	for (BYTE TempLoop = 0; TempLoop < 2; TempLoop++)
	{
		this->ScreenToClient(&Temp2POINTCollection[TempLoop]);
	}

	TempNewLeftXCoord = Temp2POINTCollection[0].x;
	TempNewRightXCoord = Temp2POINTCollection[1].x;
	TempTopYCoord = Temp2POINTCollection[0].y;
	TempBottomYCoord = Temp2POINTCollection[1].y;*/
	/*
	RECT TempRect = {1, vRectCoor.left+2,vRectCoor.top,vRectCoor.bottom};

	CWnd TempArrow;
	bool TempSuccess = Create("Arrow", WS_VISIBLE, vRectCoor, &TempArrow, 6553);
	//aggregate
	TempArrow.DestroyWindow();
	*/
	if (vArrowOrientation == cArrowLeft)
	{
		TempPen.DeleteObject();
		TempPen.CreatePen(PS_SOLID, 0,cButtonColor);
		TempCDC.SelectObject(TempPen);
		POINT Temp5POINTCollection[5];
		Temp5POINTCollection[0].x = vRectCoor.left + 2;
		Temp5POINTCollection[0].y = vRectCoor.top + 1;
		Temp5POINTCollection[1] = Temp5POINTCollection[0];
		Temp5POINTCollection[1].x -= ResizeXCoor(3);
		Temp5POINTCollection[2] = Temp5POINTCollection[1];
		Temp5POINTCollection[2].x -= ResizeXCoor(10);
		Temp5POINTCollection[2].y += ResizeYCoor(10);
		TempNewLeftXCoord = Temp5POINTCollection[2].x - 15;
		Temp5POINTCollection[3] = Temp5POINTCollection[2];
		Temp5POINTCollection[3].y += ResizeYCoor(13);
		Temp5POINTCollection[4].x = vRectCoor.left + 1;
		Temp5POINTCollection[4].y = vRectCoor.bottom - ResizeYCoor(2);

		for (BYTE TempLoop = 0; TempLoop < 5; TempLoop++)
		{
			this->ScreenToClient(&Temp5POINTCollection[TempLoop]);
		}

		
		TempCDC.MoveTo(Temp5POINTCollection[0]);
		CRgn TempRegion;
		TempRegion.CreatePolygonRgn(Temp5POINTCollection,5,WINDING);
		TempCDC.FillRgn(&TempRegion,&TempBrush);
		TempPen.DeleteObject();
		TempPen.CreatePen(PS_SOLID,1,cShadowGray);
		TempCDC.SelectObject(TempPen);
		TempCDC.PolylineTo(Temp5POINTCollection,5);
		TempBrush.DeleteObject();

		TempPen.DeleteObject();
		TempPen.CreatePen(PS_SOLID,1,cDarkGray);
		TempCDC.SelectObject(TempPen);
		Temp5POINTCollection[0].y -= 1;
		Temp5POINTCollection[1].y -= 1;
		Temp5POINTCollection[2].x -= 1;
		Temp5POINTCollection[3].x -= 1;
		Temp5POINTCollection[4].x -= 1;
		TempCDC.MoveTo(Temp5POINTCollection[0]);
		TempCDC.PolylineTo(Temp5POINTCollection,5);

		if (vDoubleArrows)
		{
			TempPen.DeleteObject();
			TempPen.CreatePen(PS_SOLID, 0,cButtonColor);
			TempCDC.SelectObject(TempPen);
			POINT TempDoublePOINTCollection[5];
			TempDoublePOINTCollection[0].x = vRectCoor.right - 2;
			TempDoublePOINTCollection[0].y = vRectCoor.top + 1;
			TempDoublePOINTCollection[1] = TempDoublePOINTCollection[0];
			TempDoublePOINTCollection[1].x += ResizeXCoor(3);
			TempDoublePOINTCollection[2] = TempDoublePOINTCollection[1];
			TempDoublePOINTCollection[2].x += ResizeXCoor(10);
			TempDoublePOINTCollection[2].y += ResizeYCoor(10);
			TempNewRightXCoord = TempDoublePOINTCollection[2].x + 15;
			TempDoublePOINTCollection[3] = TempDoublePOINTCollection[2];
			TempDoublePOINTCollection[3].y += ResizeYCoor(13);
			TempDoublePOINTCollection[4].x = vRectCoor.right - 2;
			TempDoublePOINTCollection[4].y = vRectCoor.bottom - ResizeYCoor(2);

			for (BYTE TempLoop = 0; TempLoop < 5; TempLoop++)
			{
				this->ScreenToClient(&TempDoublePOINTCollection[TempLoop]);
			}
			
			TempCDC.MoveTo(TempDoublePOINTCollection[0]);
			CRgn TempDoubleRegion;
			TempDoubleRegion.CreatePolygonRgn(TempDoublePOINTCollection,5,WINDING);
			TempBrush.DeleteObject();
			TempBrush.CreateSolidBrush(cButtonColor);
			TempCDC.FillRgn(&TempDoubleRegion,&TempBrush);
			TempPen.DeleteObject();
			TempPen.CreatePen(PS_SOLID,1,cBackgroundColor);
			TempCDC.SelectObject(TempPen);
			TempCDC.PolylineTo(TempDoublePOINTCollection,5);
			TempBrush.DeleteObject();

			TempPen.DeleteObject();
			TempPen.CreatePen(PS_SOLID,1,cShadowGray);
			TempCDC.SelectObject(TempPen);
			TempCDC.MoveTo(TempDoublePOINTCollection[0]);
			TempCDC.LineTo(TempDoublePOINTCollection[1]);

			TempPen.DeleteObject();
			TempPen.CreatePen(PS_SOLID,1,cVeryLightGray);
			TempCDC.SelectObject(TempPen);
			TempDoublePOINTCollection[0].y -= 1;
			TempDoublePOINTCollection[1].y -= 1;
			TempDoublePOINTCollection[2].x += 1;
			TempDoublePOINTCollection[3].x += 1;
			TempDoublePOINTCollection[3].y += 1;
			TempDoublePOINTCollection[4].x += 1;
			TempCDC.MoveTo(TempDoublePOINTCollection[0]);
			TempCDC.PolylineTo(TempDoublePOINTCollection,5);

			TempPen.DeleteObject();
			TempPen.CreatePen(PS_SOLID,1,cDarkGray);
			TempCDC.SelectObject(TempPen);
			TempCDC.MoveTo(TempDoublePOINTCollection[0]);
			TempCDC.LineTo(TempDoublePOINTCollection[1]);
		}
	}
	else if (vArrowOrientation == cArrowRight)
	{
		TempPen.DeleteObject();
		TempPen.CreatePen(PS_SOLID, 0,cBackgroundColor);
		TempCDC.SelectObject(TempPen);
		POINT Temp5POINTCollection[5];
		Temp5POINTCollection[0].x = vRectCoor.right - 2;
		Temp5POINTCollection[0].y = vRectCoor.top;
		Temp5POINTCollection[1] = Temp5POINTCollection[0];
		Temp5POINTCollection[1].x += ResizeXCoor(10);
		Temp5POINTCollection[1].y += ResizeYCoor(40);
		Temp5POINTCollection[2] = Temp5POINTCollection[1];
		Temp5POINTCollection[2].y += ResizeYCoor(10);
		TempNewRightXCoord = Temp5POINTCollection[2].x + 15;
		Temp5POINTCollection[3].x = Temp5POINTCollection[0].x + ResizeXCoor(4);
		Temp5POINTCollection[3].y = vRectCoor.bottom - 2;
		Temp5POINTCollection[4].x = Temp5POINTCollection[0].x;
		Temp5POINTCollection[4].y = vRectCoor.bottom - 2;

		for (BYTE TempLoop = 0; TempLoop < 5; TempLoop++)
		{
			this->ScreenToClient(&Temp5POINTCollection[TempLoop]);
		}

		
		TempCDC.MoveTo(Temp5POINTCollection[0]);
		CRgn TempRegion;
		TempRegion.CreatePolygonRgn(Temp5POINTCollection,5,WINDING);
		TempCDC.FillRgn(&TempRegion,&TempBrush);
		TempPen.DeleteObject();
		TempPen.CreatePen(PS_SOLID,1,cBackgroundColor);
		TempCDC.SelectObject(TempPen);
		TempCDC.MoveTo(Temp5POINTCollection[0]);
		TempCDC.PolylineTo(Temp5POINTCollection,5);
		TempBrush.DeleteObject();

		TempPen.DeleteObject();
		TempPen.CreatePen(PS_SOLID,1,cVeryLightGray);
		TempCDC.SelectObject(TempPen);
		Temp5POINTCollection[0].x += 1;
		Temp5POINTCollection[0].y += 3;
		Temp5POINTCollection[1].x += 1;
		Temp5POINTCollection[2].x += 1;
		Temp5POINTCollection[3].x += 1;
		Temp5POINTCollection[3].y += 1;
		Temp5POINTCollection[4].x += 1;
		Temp5POINTCollection[4].y += 1;
		TempCDC.MoveTo(Temp5POINTCollection[0]);
		TempCDC.PolylineTo(Temp5POINTCollection,5);

		if (vDoubleArrows)
		{
			TempPen.DeleteObject();
			TempPen.CreatePen(PS_SOLID, 0,cBackgroundColor);
			TempCDC.SelectObject(TempPen);
			POINT TempDoublePOINTCollection[5];
			TempDoublePOINTCollection[0].x = vRectCoor.left + 1;
			TempDoublePOINTCollection[0].y = vRectCoor.top;
			TempDoublePOINTCollection[1] = TempDoublePOINTCollection[0];
			TempDoublePOINTCollection[1].x -= ResizeXCoor(12);
			TempDoublePOINTCollection[1].y += ResizeYCoor(40);
			TempDoublePOINTCollection[2] = TempDoublePOINTCollection[1];
			TempDoublePOINTCollection[2].y += ResizeYCoor(10);
			TempNewLeftXCoord = TempDoublePOINTCollection[2].x - 15;
			TempDoublePOINTCollection[3].x = TempDoublePOINTCollection[0].x - ResizeXCoor(4);
			TempDoublePOINTCollection[3].y = vRectCoor.bottom - 2;
			TempDoublePOINTCollection[4].x = TempDoublePOINTCollection[0].x + 2;
			TempDoublePOINTCollection[4].y = TempDoublePOINTCollection[3].y;

			for (BYTE TempLoop = 0; TempLoop < 5; TempLoop++)
			{
				this->ScreenToClient(&TempDoublePOINTCollection[TempLoop]);
			}

			
			TempCDC.MoveTo(TempDoublePOINTCollection[0]);
			TempBrush.DeleteObject();
			TempBrush.CreateSolidBrush(cButtonColor);
			CRgn TempDoubleRegion;
			TempDoubleRegion.CreatePolygonRgn(TempDoublePOINTCollection,5,WINDING);
			TempCDC.FillRgn(&TempDoubleRegion,&TempBrush);
			TempPen.DeleteObject();
			TempPen.CreatePen(PS_SOLID,1,cShadowGray);
			TempCDC.SelectObject(TempPen);
			TempCDC.MoveTo(TempDoublePOINTCollection[0]);
			TempCDC.PolylineTo(TempDoublePOINTCollection,5);
			TempBrush.DeleteObject();

			TempPen.DeleteObject();
			TempPen.CreatePen(PS_SOLID,1,cBackgroundColor);
			TempCDC.SelectObject(TempPen);
			TempCDC.MoveTo(TempDoublePOINTCollection[3]);
			TempCDC.LineTo(TempDoublePOINTCollection[4]);

			TempPen.DeleteObject();
			TempPen.CreatePen(PS_SOLID,1,cDarkGray);
			TempCDC.SelectObject(TempPen);
			TempDoublePOINTCollection[0].x -= 1;
			TempDoublePOINTCollection[1].x -= 1;
			TempDoublePOINTCollection[2].x -= 1;
			TempDoublePOINTCollection[3].y += 1;
			TempDoublePOINTCollection[4].y += 1;
			TempCDC.MoveTo(TempDoublePOINTCollection[0]);
			TempCDC.PolylineTo(TempDoublePOINTCollection,5);

			TempPen.DeleteObject();
			TempPen.CreatePen(PS_SOLID,1,cVeryLightGray);
			TempCDC.SelectObject(TempPen);
			TempCDC.MoveTo(TempDoublePOINTCollection[3]);
			TempCDC.LineTo(TempDoublePOINTCollection[4]);
		}
	}

	else if (vArrowOrientation == cArrowDown)
	{
		TempPen.DeleteObject();
		TempPen.CreatePen(PS_SOLID, 0,cBackgroundColor);
		TempCDC.SelectObject(TempPen);
		POINT Temp4PointCollection[4];
		Temp4PointCollection[0].x = vRectCoor.left;
		Temp4PointCollection[0].y = vRectCoor.bottom - 2;
		Temp4PointCollection[1] = Temp4PointCollection[0];
		Temp4PointCollection[1].x += ResizeXCoor(31);
		Temp4PointCollection[1].y += ResizeYCoor(10);
		Temp4PointCollection[2] = Temp4PointCollection[1];
		Temp4PointCollection[2].x += ResizeXCoor(31);
		TempBottomYCoord = Temp4PointCollection[2].y + 15;
		Temp4PointCollection[3] = Temp4PointCollection[0];
		Temp4PointCollection[3].x = vRectCoor.right;

		for (BYTE TempLoop = 0; TempLoop < 4; TempLoop++)
		{
			this->ScreenToClient(&Temp4PointCollection[TempLoop]);
		}
		
		TempCDC.MoveTo(Temp4PointCollection[0]);
		CRgn TempRegion;
		TempRegion.CreatePolygonRgn(Temp4PointCollection,4,WINDING);
		TempCDC.FillRgn(&TempRegion,&TempBrush);
		TempPen.DeleteObject();
		TempPen.CreatePen(PS_SOLID,1,cBackgroundColor);
		TempCDC.SelectObject(TempPen);
		TempCDC.MoveTo(Temp4PointCollection[0]);
		TempCDC.PolylineTo(Temp4PointCollection,4);
		TempBrush.DeleteObject();

		TempPen.DeleteObject();
		TempPen.CreatePen(PS_SOLID,1,cVeryLightGray);
		TempCDC.SelectObject(TempPen);
		Temp4PointCollection[0].y += 1;
		Temp4PointCollection[1].y += 1;
		Temp4PointCollection[2].y += 1;
		Temp4PointCollection[3].y += 1;
		TempCDC.MoveTo(Temp4PointCollection[0]);
		TempCDC.PolylineTo(Temp4PointCollection,4);
	}
	else if (vArrowOrientation == cPointyArrowSelectItemRight)
	{
		TempPen.DeleteObject();
		TempPen.CreatePen(PS_SOLID, 0,cButtonColor);
		TempCDC.SelectObject(TempPen);
		POINT Temp3PointCollection[3];
		Temp3PointCollection[0].x = vRectCoor.right - 2;
		Temp3PointCollection[0].y = vRectCoor.top + 1;
		Temp3PointCollection[1] = Temp3PointCollection[0];
		Temp3PointCollection[1].x += (vRectCoor.bottom - vRectCoor.top)/2;
		Temp3PointCollection[1].y += (vRectCoor.bottom - vRectCoor.top)/2;
		Temp3PointCollection[2].x = vRectCoor.right - 2;
		Temp3PointCollection[2].y = vRectCoor.bottom - 1;
		TempNewRightXCoord = Temp3PointCollection[1].x + 15;

		for (BYTE TempLoop = 0; TempLoop < 3; TempLoop++)
		{
			this->ScreenToClient(&Temp3PointCollection[TempLoop]);
		}
		
		TempCDC.MoveTo(Temp3PointCollection[0]);
		CRgn TempRegion;
		TempRegion.CreatePolygonRgn(Temp3PointCollection,3,WINDING);
		TempCDC.FillRgn(&TempRegion,&TempBrush);
		TempPen.DeleteObject();
		TempPen.CreatePen(PS_SOLID,1,cShadowGray);
		TempCDC.SelectObject(TempPen);
		TempCDC.PolylineTo(Temp3PointCollection,3);
		TempBrush.DeleteObject();

		TempPen.DeleteObject();
		TempPen.CreatePen(PS_SOLID,1,cDarkGray);
		TempCDC.SelectObject(TempPen);
		Temp3PointCollection[0].y -= 1;
		Temp3PointCollection[1].y -= 1;
		Temp3PointCollection[2].x -= 1;
		TempCDC.MoveTo(Temp3PointCollection[0]);
		TempCDC.PolylineTo(Temp3PointCollection,3);
	}
	else if (vArrowOrientation == cPointyArrowSelectItemLeft)
	{
		TempPen.DeleteObject();
		TempPen.CreatePen(PS_SOLID, 0,cButtonColor);
		TempCDC.SelectObject(TempPen);
		POINT Temp3PointCollection[3];
		Temp3PointCollection[0].x = vRectCoor.left + 2;
		Temp3PointCollection[0].y = vRectCoor.top + 1;
		Temp3PointCollection[1] = Temp3PointCollection[0];
		Temp3PointCollection[1].x -= (vRectCoor.bottom - vRectCoor.top)/2;
		Temp3PointCollection[1].y += (vRectCoor.bottom - vRectCoor.top)/2;
		TempNewLeftXCoord = Temp3PointCollection[1].x - 15;
		Temp3PointCollection[2].x = vRectCoor.left + 2;
		Temp3PointCollection[2].y = vRectCoor.bottom - 1;

		for (BYTE TempLoop = 0; TempLoop < 3; TempLoop++)
		{
			this->ScreenToClient(&Temp3PointCollection[TempLoop]);
		}
		
		TempCDC.MoveTo(Temp3PointCollection[0]);
		CRgn TempRegion;
		TempRegion.CreatePolygonRgn(Temp3PointCollection,3,WINDING);
		TempCDC.FillRgn(&TempRegion,&TempBrush);
		TempPen.DeleteObject();
		TempPen.CreatePen(PS_SOLID,1,cShadowGray);
		TempCDC.SelectObject(TempPen);
		TempCDC.PolylineTo(Temp3PointCollection,3);
		TempBrush.DeleteObject();

		TempPen.DeleteObject();
		TempPen.CreatePen(PS_SOLID,1,cDarkGray);
		TempCDC.SelectObject(TempPen);
		Temp3PointCollection[0].y -= 1;
		Temp3PointCollection[1].y -= 1;
		Temp3PointCollection[2].x -= 1;
		TempCDC.MoveTo(Temp3PointCollection[0]);
		TempCDC.PolylineTo(Temp3PointCollection,3);
	}

	if (vNavigationArrow)
	{
		POINT Temp7PointCollection[7];
		CRgn TempRegion;
		this->GetWindowRect(&vRectCoor);
				
		switch (vNavigationArrow)
		{
			case NULL:
				break;

			case cLeftArrowBig:
				
				TempBrush.DeleteObject();
				TempPen.DeleteObject();
				TempPen.CreatePen(PS_SOLID, 0,cButtonTextColor);
				TempBrush.CreateSolidBrush(cButtonTextColor);
				TempCDC.SelectObject(TempPen);
				Temp7PointCollection[0].x = vRectCoor.left + ResizeXCoor(25);
				Temp7PointCollection[0].y = vRectCoor.top + ResizeYCoor(32);
				Temp7PointCollection[1] = Temp7PointCollection[0];
				Temp7PointCollection[1].x += ResizeXCoor(20);
				Temp7PointCollection[1].y -= ResizeYCoor(25) - 1;
				Temp7PointCollection[2] = Temp7PointCollection[1];
				Temp7PointCollection[2].y += ResizeYCoor(15);
				Temp7PointCollection[3] = Temp7PointCollection[2];
				Temp7PointCollection[3].x += ResizeXCoor(15);
				Temp7PointCollection[4] = Temp7PointCollection[3];
				Temp7PointCollection[4].y += ResizeYCoor(20) - 1;
				Temp7PointCollection[5] = Temp7PointCollection[4];
				Temp7PointCollection[5].x -= ResizeXCoor(15);
				Temp7PointCollection[6] = Temp7PointCollection[5];
				Temp7PointCollection[6].y += ResizeYCoor(15);

				for (BYTE TempLoop = 0; TempLoop < 7; TempLoop++)
				{
					this->ScreenToClient(&Temp7PointCollection[TempLoop]);
				}

				TempCDC.MoveTo(Temp7PointCollection[0]);
				TempRegion.CreatePolygonRgn(Temp7PointCollection,7,WINDING);
				TempCDC.FillRgn(&TempRegion,&TempBrush);
				TempPen.DeleteObject();
				TempBrush.DeleteObject();
				break;

			case cLeftArrowSmall:

				TempBrush.DeleteObject();
				TempPen.DeleteObject();
				TempPen.CreatePen(PS_SOLID, 0,cButtonTextColor);
				TempBrush.CreateSolidBrush(cButtonTextColor);
				TempCDC.SelectObject(TempPen);
				Temp7PointCollection[0].x = vRectCoor.left + ResizeXCoor(33);
				Temp7PointCollection[0].y = vRectCoor.top + ResizeYCoor(35);
				Temp7PointCollection[1] = Temp7PointCollection[0];
				Temp7PointCollection[1].x += ResizeXCoor(12);
				Temp7PointCollection[1].y -= ResizeYCoor(15);
				Temp7PointCollection[2] = Temp7PointCollection[1];
				Temp7PointCollection[2].y += ResizeYCoor(10);
				Temp7PointCollection[3] = Temp7PointCollection[2];
				Temp7PointCollection[3].x += ResizeXCoor(8);
				Temp7PointCollection[4] = Temp7PointCollection[3];
				Temp7PointCollection[4].y += ResizeYCoor(10);
				Temp7PointCollection[5] = Temp7PointCollection[4];
				Temp7PointCollection[5].x -= ResizeXCoor(8);
				Temp7PointCollection[6] = Temp7PointCollection[5];
				Temp7PointCollection[6].y += ResizeYCoor(10);

				for (BYTE TempLoop = 0; TempLoop < 7; TempLoop++)
				{
					this->ScreenToClient(&Temp7PointCollection[TempLoop]);
				}

				TempCDC.MoveTo(Temp7PointCollection[0]);
				TempRegion.CreatePolygonRgn(Temp7PointCollection,7,WINDING);
				TempCDC.FillRgn(&TempRegion,&TempBrush);
				TempPen.DeleteObject();
				TempBrush.DeleteObject();
				break;

			case cRightArrowBig:
				
				TempBrush.DeleteObject();
				TempPen.DeleteObject();
				TempPen.CreatePen(PS_SOLID, 0,cButtonTextColor);
				TempBrush.CreateSolidBrush(cButtonTextColor);
				TempCDC.SelectObject(TempPen);
				Temp7PointCollection[0].x = vRectCoor.left + ResizeXCoor(25);
				Temp7PointCollection[0].y = vRectCoor.top + ResizeYCoor(22);
				Temp7PointCollection[1] = Temp7PointCollection[0];
				Temp7PointCollection[1].y += ResizeYCoor(20);
				Temp7PointCollection[2] = Temp7PointCollection[1];
				Temp7PointCollection[2].x += ResizeXCoor(15);
				Temp7PointCollection[3] = Temp7PointCollection[2];
				Temp7PointCollection[3].y += ResizeYCoor(15);
				Temp7PointCollection[4] = Temp7PointCollection[3];
				Temp7PointCollection[4].y -= ResizeYCoor(25);
				Temp7PointCollection[4].x += ResizeXCoor(20) + 1;
				Temp7PointCollection[5] = Temp7PointCollection[4];
				Temp7PointCollection[5].y -= ResizeYCoor(25) - 1;
				Temp7PointCollection[5].x -= ResizeXCoor(20);
				Temp7PointCollection[6] = Temp7PointCollection[5];
				Temp7PointCollection[6].y += ResizeYCoor(15);

				for (BYTE TempLoop = 0; TempLoop < 7; TempLoop++)
				{
					this->ScreenToClient(&Temp7PointCollection[TempLoop]);
				}

				TempCDC.MoveTo(Temp7PointCollection[0]);
				TempRegion.CreatePolygonRgn(Temp7PointCollection,7,WINDING);
				TempCDC.FillRgn(&TempRegion,&TempBrush);
				TempPen.DeleteObject();
				TempBrush.DeleteObject();
				break;
			
			case cRightArrowSmall:
				
				TempBrush.DeleteObject();
				TempPen.DeleteObject();
				TempPen.CreatePen(PS_SOLID, 0,cButtonTextColor);
				TempBrush.CreateSolidBrush(cButtonTextColor);
				TempCDC.SelectObject(TempPen);
				Temp7PointCollection[0].x = vRectCoor.left + ResizeXCoor(33);
				Temp7PointCollection[0].y = vRectCoor.top + ResizeYCoor(34);
				Temp7PointCollection[1] = Temp7PointCollection[0];
				Temp7PointCollection[1].y += ResizeYCoor(10);
				Temp7PointCollection[2] = Temp7PointCollection[1];
				Temp7PointCollection[2].x += ResizeXCoor(8);
				Temp7PointCollection[3] = Temp7PointCollection[2];
				Temp7PointCollection[3].y += ResizeYCoor(10);
				Temp7PointCollection[4] = Temp7PointCollection[3];
				Temp7PointCollection[4].y -= ResizeYCoor(15);
				Temp7PointCollection[4].x += ResizeXCoor(12);
				Temp7PointCollection[5] = Temp7PointCollection[4];
				Temp7PointCollection[5].y -= ResizeYCoor(15);
				Temp7PointCollection[5].x -= ResizeXCoor(12);
				Temp7PointCollection[6] = Temp7PointCollection[5];
				Temp7PointCollection[6].y += ResizeYCoor(10);

				for (BYTE TempLoop = 0; TempLoop < 7; TempLoop++)
				{
					this->ScreenToClient(&Temp7PointCollection[TempLoop]);
				}

				TempCDC.MoveTo(Temp7PointCollection[0]);
				TempRegion.CreatePolygonRgn(Temp7PointCollection,7,WINDING);
				TempCDC.FillRgn(&TempRegion,&TempBrush);
				TempPen.DeleteObject();
				TempBrush.DeleteObject();
				break;

			case cUpArrowBig:
				
				TempBrush.DeleteObject();
				TempPen.DeleteObject();
				TempPen.CreatePen(PS_SOLID, 0,cButtonTextColor);
				TempBrush.CreateSolidBrush(cButtonTextColor);
				TempCDC.SelectObject(TempPen);
				Temp7PointCollection[0].x = vRectCoor.left + ResizeXCoor(15);
				Temp7PointCollection[0].y = vRectCoor.top + ResizeYCoor(33);
				Temp7PointCollection[1] = Temp7PointCollection[0];
				Temp7PointCollection[1].x += ResizeXCoor(15);
				Temp7PointCollection[2] = Temp7PointCollection[1];
				Temp7PointCollection[2].y += ResizeYCoor(15);
				Temp7PointCollection[3] = Temp7PointCollection[2];
				Temp7PointCollection[3].x += ResizeXCoor(20);
				Temp7PointCollection[4] = Temp7PointCollection[3];
				Temp7PointCollection[4].y -= ResizeYCoor(15);
				Temp7PointCollection[5] = Temp7PointCollection[4];
				Temp7PointCollection[5].x += ResizeXCoor(15);
				Temp7PointCollection[6] = Temp7PointCollection[5];
				Temp7PointCollection[6].x -= ResizeXCoor(25);
				Temp7PointCollection[6].y -= ResizeYCoor(20);

				for (BYTE TempLoop = 0; TempLoop < 7; TempLoop++)
				{
					this->ScreenToClient(&Temp7PointCollection[TempLoop]);
				}

				TempCDC.MoveTo(Temp7PointCollection[0]);
				TempRegion.CreatePolygonRgn(Temp7PointCollection,7,WINDING);
				TempCDC.FillRgn(&TempRegion,&TempBrush);
				TempPen.DeleteObject();
				TempBrush.DeleteObject();
				break;
			case cUpArrowSmall:
							TempBrush.DeleteObject();
				TempPen.DeleteObject();
				TempPen.CreatePen(PS_SOLID, 0,cButtonTextColor);
				TempBrush.CreateSolidBrush(cButtonTextColor);
				TempCDC.SelectObject(TempPen);
				Temp7PointCollection[0].x = vRectCoor.left + ResizeXCoor(27);
				Temp7PointCollection[0].y = vRectCoor.top + ResizeYCoor(33);
				Temp7PointCollection[1] = Temp7PointCollection[0];
				Temp7PointCollection[1].x += ResizeXCoor(10);
				Temp7PointCollection[2] = Temp7PointCollection[1];
				Temp7PointCollection[2].y += ResizeYCoor(8);
				Temp7PointCollection[3] = Temp7PointCollection[2];
				Temp7PointCollection[3].x += ResizeXCoor(10);
				Temp7PointCollection[4] = Temp7PointCollection[3];
				Temp7PointCollection[4].y -= ResizeYCoor(8);
				Temp7PointCollection[5] = Temp7PointCollection[4];
				Temp7PointCollection[5].x += ResizeXCoor(10);
				Temp7PointCollection[6] = Temp7PointCollection[5];
				Temp7PointCollection[6].x -= ResizeXCoor(15);
				Temp7PointCollection[6].y -= ResizeYCoor(12);

				for (BYTE TempLoop = 0; TempLoop < 7; TempLoop++)
				{
					this->ScreenToClient(&Temp7PointCollection[TempLoop]);
				}

				TempCDC.MoveTo(Temp7PointCollection[0]);
				TempRegion.CreatePolygonRgn(Temp7PointCollection,7,WINDING);
				TempCDC.FillRgn(&TempRegion,&TempBrush);
				TempPen.DeleteObject();
				TempBrush.DeleteObject();
				break;
			case cDownArrowBig:
				
				TempBrush.DeleteObject();
				TempPen.DeleteObject();
				TempPen.CreatePen(PS_SOLID, 0,cButtonTextColor);
				TempBrush.CreateSolidBrush(cButtonTextColor);
				TempCDC.SelectObject(TempPen);
				Temp7PointCollection[0].x = vRectCoor.left + ResizeXCoor(30);
				Temp7PointCollection[0].y = vRectCoor.top + ResizeYCoor(18);
				Temp7PointCollection[1] = Temp7PointCollection[0];
				Temp7PointCollection[1].y += ResizeYCoor(15) ;
				Temp7PointCollection[2] = Temp7PointCollection[1];
				Temp7PointCollection[2].x -= ResizeXCoor(15);
				Temp7PointCollection[3] = Temp7PointCollection[2];
				Temp7PointCollection[3].y += ResizeYCoor(20);
				Temp7PointCollection[3].x += ResizeXCoor(25);
				Temp7PointCollection[4] = Temp7PointCollection[3];
				Temp7PointCollection[4].y -= ResizeYCoor(20);
				Temp7PointCollection[4].x += ResizeXCoor(25);
				Temp7PointCollection[5] = Temp7PointCollection[4];
				Temp7PointCollection[5].x -= ResizeXCoor(15);
				Temp7PointCollection[6] = Temp7PointCollection[5];
				Temp7PointCollection[6].y -= ResizeYCoor(15);

				for (BYTE TempLoop = 0; TempLoop < 7; TempLoop++)
				{
					this->ScreenToClient(&Temp7PointCollection[TempLoop]);
				}

				TempCDC.MoveTo(Temp7PointCollection[0]);
				TempRegion.CreatePolygonRgn(Temp7PointCollection,7,WINDING);
				TempCDC.FillRgn(&TempRegion,&TempBrush);
				TempPen.DeleteObject();
				TempBrush.DeleteObject();
				break;

			case cDownArrowSmall:

				TempBrush.DeleteObject();
				TempPen.DeleteObject();
				TempPen.CreatePen(PS_SOLID, 0,cButtonTextColor);
				TempBrush.CreateSolidBrush(cButtonTextColor);
				TempCDC.SelectObject(TempPen);
				Temp7PointCollection[0].x = vRectCoor.left + ResizeXCoor(35);
				Temp7PointCollection[0].y = vRectCoor.top + ResizeYCoor(23);
				Temp7PointCollection[1] = Temp7PointCollection[0];
				Temp7PointCollection[1].y += ResizeYCoor(8);
				Temp7PointCollection[2] = Temp7PointCollection[1];
				Temp7PointCollection[2].x -= ResizeXCoor(10);
				Temp7PointCollection[3] = Temp7PointCollection[2];
				Temp7PointCollection[3].y += ResizeYCoor(12);
				Temp7PointCollection[3].x += ResizeXCoor(15);
				Temp7PointCollection[4] = Temp7PointCollection[3];
				Temp7PointCollection[4].y -= ResizeYCoor(12);
				Temp7PointCollection[4].x += ResizeXCoor(15);
				Temp7PointCollection[5] = Temp7PointCollection[4];
				Temp7PointCollection[5].x -= ResizeXCoor(10);
				Temp7PointCollection[6] = Temp7PointCollection[5];
				Temp7PointCollection[6].y -= ResizeYCoor(8);

				for (BYTE TempLoop = 0; TempLoop < 7; TempLoop++)
				{
					this->ScreenToClient(&Temp7PointCollection[TempLoop]);
				}

				TempCDC.MoveTo(Temp7PointCollection[0]);
				TempRegion.CreatePolygonRgn(Temp7PointCollection,7,WINDING);
				TempCDC.FillRgn(&TempRegion,&TempBrush);
				TempPen.DeleteObject();
				TempBrush.DeleteObject();
				break;
		}
	}
	vRectCoor.top = TempTopYCoord;
	vRectCoor.bottom = TempBottomYCoord;
	vRectCoor.right = TempNewRightXCoord;
	vRectCoor.left = TempNewLeftXCoord;

	//this->SetWindowRect(&vRectCoor);  SetBoundsRect
//	this->CalcWindowRect(&vRectCoor, adjustBorder);
//	this->AttachControlSite();
//	CHandleMap *pMap;
//	this->AttachControlSite(pMap);
}


RECT CFunctionButton::GetRectToInvalidateArrow()
{
	return vRectCoor;
}


void CFunctionButton::OnPaint()
{
	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages
	CStatic::OnPaint();
	Invalidate();
	CPaintDC TempDC(this);
	DrawArrows(TempDC);
}


BEGIN_MESSAGE_MAP(CFunctionButton, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

