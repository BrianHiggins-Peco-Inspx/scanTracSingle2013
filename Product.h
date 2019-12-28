//ScanTrac Side View Source File
// Product.h: interface for the CProduct class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_PRODUCT_H__54309CC5_D2A5_11D3_ABCE_00500466E305__INCLUDED_)
#define AFX_PRODUCT_H__54309CC5_D2A5_11D3_ABCE_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//not sure if need next two lines to export extraction operator
#undef AFX_API
#define AFX_API AFX_EXT_CLASS

#include "Inspection.h"
#include "InspectionContaminant.h"
#include "InspectionVoid.h"
#include "InspectionDensity.h"
#include "InspectionStandardDeviation.h"

const BYTE cMaximumNumberOfInspections = 14;
const int cMaximumNumberOfSelectProductCodes = 1000;
const BYTE cDisplayProductCodeCount = 33;
//IMPORTANT NOTE: YOU CANNOT CHANGE THE CONSTANTS BELOW WITHOUT CHANGING
//THE VERSION NUMBER OF THE ARCHIVE FILE, AND HARD CODING THESE VALUES 
//IN THE OLD VERSION READ CODE
const BYTE cMaximumNumberOfCountsDisplayed = 36;
const BYTE cNumberOfExternalDetectors = 5;
const BYTE cOldNumberOfEjectorsInStream3 = 3;
const BYTE cOldNumberOfEjectorsInStream6 = 6;
const BYTE cNumberOfEjectors = 32; //only 30 supported in Hardware, but may add 2 more, if so, change maximum in SystemEjectorConfiguration
const BYTE cNumberOfEjectorsForUser = 30; //only 30 supported in Hardware, but may add 2 more, if so, change maximum in SystemEjectorConfiguration
const BYTE cNumberOfBodyTriggers = 5;
const BYTE cNumberOfCanStops = 2;
const BYTE cNumberOfAlarms = 2;
const BYTE cNumberOfXRayImageCriteriaInArchive = 16;
const BYTE cNumberOfEnhanceMethods = 8;
const BYTE cMaximumNumberOfDetectors8 = 8;  //in config file, don't change
const BYTE cMaximumNumberOfDetectors12 = 12;  //in config file, don't change
const BYTE cMaximumNumberOfSourceStrengthValues21 = 21;  //18 detectors plus 3-12 inch image sets, in config file, don't change
const BYTE cNumberOfFillerOffsetSpeeds = 4;
const BYTE cSamplingFIFOLength = 255;
const BYTE cMaximumNumberForMultiLane = 32;  //only 30 ejectors possible, so only 30 lanes

extern double vGlobalPixelsPerUnit;
extern double vGlobalPixelsPerUnitInHeight;

#include "ImageData.h"

//<your class declarations here>

class CProduct : public CObject
{
private:
	//For serializing, writing to disk product info
DECLARE_SERIAL(CProduct)
//DECLARE_DYNCREATE(CProduct)
public:
	// static member is initalized at top of CScanTracDlg
	//    static int vNumberOfProducts;
	CString vProductName;
	BYTE vHistogramThreshold;
	BYTE vHistogramLowerThreshold;
	BYTE vHistogramDilateTimes;
	WORD vTypesOfRejectsToView;
	double vMaxConveyorSpeedAsEntered;
	BYTE vBulkProductMode;

	double vSetupHistoryAverageDensity;
	BYTE vNumberOfInspections;
	BYTE vNumberOfAuxiliaryDetectors;
	CInspection *vInspection[cMaximumNumberOfInspections];
	CInspection *vDriftDensityInspection;
	
	//Filler Monitoring Sampling
	CInspection *vFillerMonitorInspection;
	double vFillerTolerance;
	BYTE vHeadOffsetFromIndexPulse[8];


	BYTE vNumberOfInspectionsRequiredToEject;
	CString vNotes;
	BYTE vOverScanFactor;
	BYTE vOverScanMultiplier;
	WORD vTooManyRejectsAlarmX[2];
	WORD vTooManyRejectsAlarmY[2];
	BYTE vTooManyRejectsAlarmDisableEjectors[2];
	DWORD vInspectionAlarmSettings[2]; 
	double vMaximumImageOverlap;
	WORD vReferenceTop;
	WORD vReferenceBottom;
	WORD vReferenceLeft;
	WORD vReferenceRight;
	WORD vLowerRimHeightInPixels;
	WORD vXRayIntegrationTime; //new style code where 750 = .3 mSec.  Each 1 in code = 4uSec or .0004 mSec
	BYTE vXRayIntegrationTimeByte;  //old style, 0 = .3, 1 = .4 but must keep so can convert to new style, can not re-use

	WORD vReferenceWidth;
	BYTE vReferenceROIsToEdges;  //using One in ConfigurationData now as Alex wants to

	DWORD vSetupHistoryAverageCount;
	DWORD vSetupHistoryStructureCount;
	CString vHistoryTookAverageDate;
	BYTE vSetupHistoryHistogramThreshold;
	double vSetupHistoryContainerTriggerStandardDeviation;
	WORD vAddToLearnCount;
	BYTE vHaveManuallySetRetriggerLockout;

	BYTE vNeedToRelearn;
	double vStructureROIAverage;
	double vStructureROIStandardDievation;

	double vDriftROITop;//Used for ROI of Drift Compensation
	double vDriftROIBottom;//Used for ROI of Drift Compensation
	double vDriftROILeft;//Used for ROI of Drift Compensation
	double vDriftROIRight;//Used for ROI of Drift Compensation

	CString vProductCodeSelectionCString;

	BYTE vThresholdForContainerBounds;
	float vProductImageHeightTop;
	float vProductImageHeightBottom;
	//Note, all Conveyor positions have two values, one in inches,
	//and one in Conveyor counts
	//only the one in inches is saved to the config file
	float vProductImageWidth;
	WORD vImageWidthPosition;

	float vProductLockOutWidth;
	WORD vLockoutPosition;

	float vProductBodyTriggerToImageDistanceInInches;
	WORD vBTToImagePosition;

	float vEndOfLineTimeOut;
	WORD vEndOfLineBeltPosition;

	float vEdgeLocationBottom;
	WORD vEdgeLocationBottomPixel;
	float vEdgeLocationHeight;
	WORD vEdgeLocationHeightPixel;

	float vBottomLocationLeft;
	WORD vBottomLocationLeftPixel;
	float vBottomLocationLength;
	WORD vBottomLocationLengthPixel;

	// main x ray detector configuration
	float vXRaySourceVoltage;
	float vXRaySourceCurrent;
	//Container trigger data
	BYTE vBodyTrigger;
	BYTE vBackupBodyTrigger;
	BYTE vSamplingEjectorVariableToReadInOldByte[3];
	DWORD vSamplingEjector[3];
	BYTE vUsingXScanPoint4mmDetectors;

	//Auxiliary detector data
	BYTE vExternalDetectorEnable[cNumberOfExternalDetectors];
	float vExternalDetectorWindowStart[cNumberOfExternalDetectors];
	WORD vExternalDetectorWindowStartPosition[cNumberOfExternalDetectors];
	float vExternalDetectorWindowEnd[cNumberOfExternalDetectors];
	WORD vExternalDetectorWindowEndPosition[cNumberOfExternalDetectors];
	BYTE vExternalDetectorMode[cNumberOfExternalDetectors];
	BYTE vExternalDetectorEjectorVariableToReadInOldByte[cNumberOfExternalDetectors];
	DWORD vExternalDetectorEjector[cNumberOfExternalDetectors];
	//ejector data
	//do not use Dwell position
	float vEjectorDwellPosition[cNumberOfEjectors]; // not used, removed from stream, but keep variable to read in old streams
	float vEjectorDwellTime[cNumberOfEjectors];
	float vEjectorResponseTime[cNumberOfEjectors];
	float vResyncTriggerToEjectTime[cNumberOfEjectors];

	float vEjectorDistanceFromTriggerInInches[cNumberOfEjectors];  //in inches or mm
	WORD vEjectorDelayBeltPosition[cNumberOfEjectors];

	//Multi Lane Feature Variables
	BYTE vMultiLaneNumberOfLanes;
	WORD vMultiLaneStartPixel[cMaximumNumberForMultiLane];
	WORD vMultiLaneWidthInPixels[cMaximumNumberForMultiLane];
	BYTE vMultiLaneNumberOfLinesBeforeContainer;
	BYTE vMultiLaneEjectAdjacentLanes;

	//spares
	double vSpareElectronicOffset; //obsolete
	double vSpareDACDetectorOffset[cMaximumNumberOfDetectors8];//obsolete Not in stream anymore, but needed to read in old streams

	double vSpareDoubleWasStructureAdjustFactor;
	double vSpareDoubleWasStructureAdjustSubtractiveFactor;

	double vSpareDoubleCB;//not used anymore
	double vSpareDoubleCL;//not used anymore
	double vSpareDoubleCR;//not used anymore

	int vSpareIntP1;
	int vSpareIntP2;
	int vSpareIntP3;
	int vSpareIntP4;
	int vSpareIntP5;

	BYTE vSpareByteDT;//not used anymore

	BYTE vSpareByteSHMG;//not used anymore
	BYTE vSpareByteSHMB;//not used anymore
	//float vSpareFloatSHMBA;//not used anymore
	float vSpareFloatSHMBSD;//not used anymore

	BYTE vSpareByte;//not used anymore
	float vSpareFloat1;//not used anymore
	float vSpareFloat2;//not used anymore
	BYTE vSpareByteVSHMG;//not used anymore
	float vSpareFloatVSHMGA;//not used anymore
	float vSpareFloatVSHMGSD;//not used anymore
	BYTE vSpareByteVSHMB;//not used anymore
	float vSpareFloatVSHMBA;//not used anymore
	float vSpareFloatVSHMBSD;//not used anymore
	BYTE vSpareByteVSHHT;//not used anymore

	CString vTempScreenShotDirectoryName;//Spare, not used anymore
	CString vSpareCString1;//Spare
	CString vSpareCString2;//Spare
	CString vSpareCString3;//Spare
	CString vSpareCString4;//Spare
	CString vSpareCString5;//Spare

	WORD vEncoderRateIndexesForFillerSeamerSampling[cNumberOfFillerOffsetSpeeds]; //vFoundFullSpeedEncoderRate; 
	WORD vSpareWordP1; //Spare
	WORD vSpareWordP2; //Spare
	WORD vSpareWordP3; //Spare
	WORD vSpareWordP4; //Spare
	WORD vSpareWordP5; //Spare
	DWORD vSpareDWordVSHAC;//not used anymore
	DWORD vSpareDWordVSHSC;//not used anymore
	DWORD vSpareDWordVSMGC;//not used anymore
	DWORD vSpareDWordVSMBC;//not used anymore

	//density variables
	CString vSpareStringHDN;//not used anymore
	CString vSpareStringLDN;//not used anymore
	float vSpareFloatDTU;//not used anymore
	float vSpareFloatDTL;//not used anymore
	DWORD vSpareDWordDAC;//not used anymore
	BYTE vSpareByteDE1;//not used anymore
	float vSpareFloatDMV;//not used anymore
	float vSpareFloatDMiV;//not used anymore
	float vSpareFloatDAV;//not used anymore
	float vSpareFloatDSD;//not used anymore

	float vSpareFloatSHDATU;//not used anymore
	float vSpareFloatSHDATL;//not used anymore

	//void variables
	CString vSpareStringVN;//not used anymore
	double vSpareDoubleVT1;//not used anymore
	double vSpareDoubleVL;//not used anymore
	double vSpareDoubleVR;//not used anymore
	WORD vSpareWordVS;//not used anymore
	BYTE vSpareByteVSHAT1;//not used anymore
	BYTE vSpareByteVAS;//not used anymore
	BYTE vSpareByteVDIF;//not used anymore
	BYTE vSpareByteVDD;//not used anymore
	double vSpareDoubleVOIF;//not used anymore
	double vSpareDoubleVEIF;//not used anymore
	BYTE vSpareByteVDC;//not used anymore
	BYTE vSpareByteVDT;//not used anymore
	BYTE vSpareByteCanImageBottomOfContainer; //not used anymore
	BYTE vSpareByteXRSE;//obsolete

	BYTE vSpareByteXRCE[cNumberOfXRayImageCriteriaInArchive]; //obsolete Not in stream anymore, but needed to read in old streams
	BYTE vSpareByteXRCE1[cNumberOfXRayImageCriteriaInArchive]; //obsolete Not in stream anymore, but needed to read in old streams
	float vSpareFloatXRCP[cNumberOfXRayImageCriteriaInArchive]; //obsolete Not in stream anymore, but needed to read in old streams
	float vSpareFloatXRCP2[cNumberOfXRayImageCriteriaInArchive]; //obsolete Not in stream anymore, but needed to read in old streams

	//methods
	CProduct();
	//CProduct (CProduct &TempProduct);
	virtual ~CProduct();
	virtual void Serialize(CArchive& TempArchive);
	CString *GetProductName();
	void SetProductName(CString TempProductName);
	void SetEndOfLineTimeOut(float TempValue);
	void SetProductImageWidth(float TempValue);
	void SetProductLockOutWidth(float TempValue);
	void SetProductBodyTriggerToImageBeltPositionOffset(float TempValue);
	void SetEjectorBeltPositionOffset(BYTE TempEjectorNumber, float TempValue);
	void SetExternalDetectorWindowStart(BYTE TempDetectorNumber, float TempValue);
	void SetExternalDetectorWindowEnd(BYTE TempDetectorNumber, float TempValue);
	void CopyProduct(CProduct *TempProduct);

	void SetEdgeLocationBottom(float TempValue);
	void SetEdgeLocationHeight(float TempValue);
	void SetBottomLocationLeft(float TempValue);
	void SetBottomLocationLength(float TempValue);

	void CalculateEndOfLineTimeOut();
	void ClearInspectionHistory();
	void SetOverScanFactor(BYTE TempOverScanFactor);
	bool ProductHasImageFiles(CString TempDirectory);
	bool ProductHasBackgroundFile(CString TempDirectory);
	BYTE InspectionNumber(CInspection *TempInspection);
	bool HaveInspection(CInspection *TempInspection);
	void SetupAllInspectionROIsFromReference(CImageData *TempContainer, BYTE TempSetProductDensityROI, bool TempMakeROIFullImage, CImageData *TempAverageImage);
	void CheckAllROIs(CImageData *TempAverageImage);
	void MatchInspectionsForWeightTrending();
	BYTE AreThereAnyOtherWeightTrendingInspections(BYTE TempThisInspection);
	BYTE AreThereAnyWeightTrendingInspections();
	bool CalculateLowerRimHeight(BYTE *TempAverageImage, BYTE *TempCurrentImage);
	bool CalculateLowerRimHeight(CImageData *TempAverageImage, CImageData *TempCurrentImage);
	void CheckInspectionsAreValid();
	void SetAllDistances();
	void ChangeInspectionNames();

	//int WriteConfigurationDataToINIFile(WORD TempNumber);
	//int ReadConfigurationDataFromINIFile();
};

//not sure if need next two lines to export extraction operator
#undef AFX_API
#define AFX_API

#endif // !defined(AFX_PRODUCT_H__54309CC5_D2A5_11D3_ABCE_00500466E305__INCLUDED_)
