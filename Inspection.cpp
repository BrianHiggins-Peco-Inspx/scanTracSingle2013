//ScanTrac Side View Source File
// Inspection.cpp: implementation of the CInspection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "math.h"  //for sqrt
#include "scantrac.h"
#include "Inspection.h"
#include "lutLog2_1024_16bit.h"
#include "ITIPCDig.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//IMPLEMENT_DYNCREATE(CInspection, CObject)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CSystemConfigurationData *vGlobalConfigurationData;
extern CScanTracSystemRunningData *vGlobalRunningData;
extern BYTE vGlobalScanTracType;
extern BYTE vGlobalLearnState;
extern CITIPCDig *vGlobalITIPCDig;
extern CProduct *vGlobalCurrentProduct;
//extern bool vGlobalUseHalconLibrary;
//extern BYTE vGlobalUseMetric;
extern BYTE vGlobalCurveFitEquationOrder;
extern WORD vGlobalCurveFitMaxCall; 
extern double vGlobalCurveFitEpsilon;
extern WORD vGlobalCurveFitStepBound;
extern double vGlobalCurveFitFTolerance;
extern double vGlobalCurveFitXTolerance;
extern double vGlobalCurveFitGTolerance;

DWORD vCallsToSquareFunction;

static double h0, h1, h2, h3, h5, h6;
static double v0, v1, v2, v3, v5, v6;

static double vh0, vh1, vh2, vh3, vh5, vh6;
static double vv0, vv1, vv2, vv3, vv5, vv6;
const BYTE cBackgroundThreshold = 240; //was 240

CInspection::CInspection()
{
	vFillBackground = false;
	vRecentPuntRemovalBottomHeight = 0;
	vRecentPuntRemovalNumberOfPixelsCopied = 0;
	for (WORD TempLoop = 0; TempLoop < cMaximumProfileSize; TempLoop++)
		vRecentPuntRemovalPuntHeightProfile[TempLoop] = 0;

	for (BYTE TempLoop = 0; TempLoop < 14; TempLoop++)
	{
		vSpareDWordA[TempLoop] = 0;
		vSpareDoubleA[TempLoop] = 0;
		vSpareIntA[TempLoop] = 0;
	}
	for (BYTE TempLoop = 0; TempLoop < 13; TempLoop++)
		vSpareByteA[TempLoop] = 0;

	vPixelLookAhead = 0;
	vLookForDarkSpots = 0;
	vOverlap = 0;
	vErodeCountAtStart = 0;
	vCheckWeighTrendAlarmMaximum = 0;
	vCheckWeighTrendAlarmMinimum = 0;

	vSpareCStringI1 = "";
	vSpareCStringI2 = "";
	vSpareCStringI3 = "";
	vSpareCStringI4 = "";
	vSpareCStringI5 = "";

	vCheckWeighOffset = 0;
	vCheckWeighMFactor = 1;  //for y=mx+b calibration of density to weight
	vCheckWeighBFactor = 0;
	vCheckWeighUnits = "Grams";
	vCheckWeighMaximumGoodWeight = 0;
	vCheckWeighMinimumGoodWeight = 0;
	vInspectionNameForWeightTrendingName = "";
	vInspectionNameForWeightTrendingPointer = NULL;

	vHasDensityLowerThreshold = false;
	vHasDensityUpperThreshold = false;
	vAllowChangingAdvancedOptions = false;
	vHasAMinimumSize = true;

	vReferenceToTop = 0;
	vInspectionType = 0;
	vInvertEjectLogic = false;
	vEnable = true;
	vName = "Generic Inspection";
	SetROITop(0);
	SetROIBottom(0);
	SetROILeft(0,1);
	SetROIRight(0,1);
	vThreshold = 64;
	vSize = 1;
	vSizeToReadInOldStream = 0;
	vMaximumSize = 0;
	vMinimumWidth = 0;
	vMinimumNumberOfItemsRequired = 0;
	vMaximumNumberOfItemsRequired = 0;
	vMinimumBrightnessAceptable = 0;
	vWhiteOutAnythingTouchingTheTop = 0;
	vIPContaminant = 0;

	vSpareWordI3 = 0;

	vSpareDoubleI2 = 0;
	vSpareDoubleI3 = 0;
	vSpareDoubleI4 = 0;
	vSpareDoubleI5 = 0;

	vSpareByteI3 = 0;
	vSpareByteI4 = 0;
	vSpareByteI5 = 0;
	vSpareByteI6 = 0;
	vSpareByteI7 = 0;
	vSpareByteI8 = 0;
	vSpareByteI9 = 0;
	vSpareByteI10 = 0;

	vEjector = 0;
	vEnhanceMethod = 0;
	vPerformGlassAlgorithm = 0;
	vSubtractAverage = 0;
	vTakeDifferenceFromMedianFilter = 0;

	vDoContaminant22Algorithm = 0;
	vAddHistogram = 0;
	vAddStructure = 0;
	vFillInLightSpots = 0;
	vMaximumDrop = 4;
	vFollowBottomUpAndDown = false;
	vTryThresholdsDownToThreshold = 0;
	vShowRejects = true;
	vSaveRejects = true;
	vSwungBaffleTolerance = 0;
	vTrimEdgesOfJar = 0;
	vTakeDerivative = 0;
	vErodeCountAtEnd = 0;
	vDilationCountAtEnd = 0;
	vOriginalImageFactor = 0;
	vEnhancedImageFactor = 0;
	vDilationCount = 0;
	vErodeCount = 0;
	vTakeDifferenceFromMedian = 0;

	vSetupHistoryMinimumGood = 255;
	vSetupHistoryMinimumGoodCount = 0;
	vSetupHistoryMinimumGoodAverage = 0;
	vSetupHistoryMinimumGoodStandardDeviation = 0;

	vPreSmoothImage = 0;
	vBonesInspection = 0;
	vSetupHistoryMaximumBad = 0;
	vSetupHistoryMaximumBadCount = 0;
	vSetupHistoryMaximumBadAverage = 0;
	vSetupHistoryMaximumBadStandardDeviation = 0;
	vSetupHistoryQualityFactor = 0;

	vSetupHistoryAutoThreshold = 0;
	vCollectingDensityAverage = false;
	vFindBadIntensity = false;
	vFindGoodIntensity = false;
	//density
	vDensityThresholdUpper = 255;
	vSetupHistoryDensityAutoThresholdUpper = 0;
	SetDensityLowerThreshold(0);
	vSetupHistoryDensityAutoThresholdLower = 0;
	vSetupHistoryMinimumValue = 0;
	vSetupHistoryMaximumValue = 0;
	vSetupHistoryBadMinimumValue = 0;
	vSetupHistoryBadMaximumValue = 0;

	vFindDarkSpotOnLine = 0;
	vPixelsStepToNextROI = 0;
	vRemoveJarPunt = 0;
	vFindSpotInSubROI = 0;
	vDesensitizeEdgePixels = 0;
	vDesensitizeIrregularArea = 0;

	vIgnoreEdgePixels = 0;
	//vConnect4 = 0;
	vMinimumLength = 0;
	vMaximumHeightAboveBottom = 0;
	vFillBackground = 0;

	vReferenceToBottom = 0;
	vSpareWordI4 = 0;
	vSpareWordI5 = 0;
	vSpareWordI6 = 0;

	vSpareDWordI2 = 0;
	vSpareDWordI3 = 0;
	vSpareDWordI4 = 0;
	vSpareDWordI5 = 0;
	vSpareDWordI6 = 0;

	vSpareIntI1 = 0;
	vSpareIntI2 = 0;
	vSpareIntI3 = 0;
	vSpareIntI4 = 0;
	vSpareIntI5 = 0;

	vCheckWeighNominalWeight = 0;

	//initialize variables for Latvian Algorithm
	/*
// thresholds calculated from sample 'Healthy Choice Good' images
	//short _MinValuesGoodH[7] = {-665, -691, -809, -587, 0, -665, -718}; 		//  filter lengths 5,7,9,3,-,5,5a
	// with Latvians
	_MinValuesGoodH[0] = -665;	//-695;	//-665;//  filter lengths 5,7,9,3,-,5,5a
	_MinValuesGoodH[1] = -691;	//-1023;	//-691;
	_MinValuesGoodH[2] = -809;	//-1024;	//-809;
	_MinValuesGoodH[3] = -587;	//-537;	//-587;
	_MinValuesGoodH[4] = -0;
	_MinValuesGoodH[5] = -665;	//-695;	//-665;
	_MinValuesGoodH[6] = -718;	//-855;	//-718;

	//double _MultipliersH[7] = {0.75, 0.9, 0.85, 1.15, 1.0, 1.1, 1.1};
	_MultipliersH[0] = 0.75;
	_MultipliersH[1] = 0.9;
	_MultipliersH[2] = 0.85;
	_MultipliersH[3] = 1.15;
	_MultipliersH[4] = 1.0;
	_MultipliersH[5] = 1.1;
	_MultipliersH[6] = 1.1;

	//short _ThresholdsH[7] = {-499,-622,-688,-675,0,-732,-789};
	
	_ThresholdsH[0] = -499;	//-521;	//-499;	
	_ThresholdsH[1] = -622;	//-921;	//-622;
	_ThresholdsH[2] = -688;	//-870;	//-688;
	_ThresholdsH[3] = -675;	//-618;	//-675;
	_ThresholdsH[4] = 0;
	_ThresholdsH[5] = -732;	//-764;	//-732;
	_ThresholdsH[6] = -789;	//-940;	//-789;
												 
	//_MinValuesGoodV[7] = {-651, -864, -933, -545, 0, -651, -650};			//  filter lengths 5,9,13,3,-,5,5a

	
	_MinValuesGoodV[0] = -651;	//-668;	//-651;
	_MinValuesGoodV[1] = -864;	//-809;	//-864;
	_MinValuesGoodV[2] = -933;	//-1024;	//-933;
	_MinValuesGoodV[3] = -545;	//-594;	//-545;
	_MinValuesGoodV[4] = 0;
	_MinValuesGoodV[5] = -651;	//-669;	//-651;
	_MinValuesGoodV[6] = -650;	//-702;	//-650;
	

	//_MultipliersV[7] = {0.85, 0.8, 0.85, 1.15, 1.0, 1.05, 1.05}; 	
	_MultipliersV[0] = 0.85;
	_MultipliersV[1] = 0.8; 	
	_MultipliersV[2] = 0.85; 	
	_MultipliersV[3] = 1.15; 	
	_MultipliersV[4] = 1.0; 	
	_MultipliersV[5] = 1.05; 	
	_MultipliersV[6] = 1.05; 	

	//_ThresholdsV[7] = {-553,-691,-793,-627,0,-684,-683};
	
	_ThresholdsV[0] = -553;	//-568;	//-553;	
	_ThresholdsV[1] = -691;	//-647;	//-691;
	_ThresholdsV[2] = -793;	//-870;	//-793;
	_ThresholdsV[3] = -627;	//-683;	//-627;
	_ThresholdsV[4] = 0;
	_ThresholdsV[5] = -684;	//-702;	//-684;
	_ThresholdsV[6] = -683;	//-737;	//-683;

*/
	

  //with ours

	_MinValuesGoodH[0] = -932;	//-695;	//-665;//  filter lengths 5,7,9,3,-,5,5a
	_MinValuesGoodH[1] = -953;	//-1023;	//-691;
	_MinValuesGoodH[2] = -974;	//-1024;	//-809;
	_MinValuesGoodH[3] = -768;	//-537;	//-587;
	_MinValuesGoodH[4] = -0;
	_MinValuesGoodH[5] = -932;	//-695;	//-665;
	_MinValuesGoodH[6] = -920;	//-855;	//-718;

	//double _MultipliersH[7] = {0.75, 0.9, 0.85, 1.15, 1.0, 1.1, 1.1};
	_MultipliersH[0] = 0.75;
	_MultipliersH[1] = 0.9;
	_MultipliersH[2] = 0.85;
	_MultipliersH[3] = 1.15;
	_MultipliersH[4] = 1.0;
	_MultipliersH[5] = 1.1;
	_MultipliersH[6] = 1.1;

	//short _ThresholdsH[7] = {-499,-622,-688,-675,0,-732,-789};
	
	_ThresholdsH[0] = -699;	//-521;	//-499;	
	_ThresholdsH[1] = -858;	//-921;	//-622;
	_ThresholdsH[2] = -828;	//-870;	//-688;
	_ThresholdsH[3] = -883;	//-618;	//-675;
	_ThresholdsH[4] = 0;
	_ThresholdsH[5] = -1025;	//-764;	//-732;
	_ThresholdsH[6] = -1012;	//-940;	//-789;
												 
	//_MinValuesGoodV[7] = {-651, -864, -933, -545, 0, -651, -650};			//  filter lengths 5,9,13,3,-,5,5a

	
	_MinValuesGoodV[0] = -1358;	//-668;	//-651;
	_MinValuesGoodV[1] = -1406;	//-809;	//-864;
	_MinValuesGoodV[2] = -1406;	//-1024;	//-933;
	_MinValuesGoodV[3] = -1557;	//-594;	//-545;
	_MinValuesGoodV[4] = 0;
	_MinValuesGoodV[5] = -1357;	//-669;	//-651;
	_MinValuesGoodV[6] = -1368;	//-702;	//-650;
	

	//_MultipliersV[7] = {0.85, 0.8, 0.85, 1.15, 1.0, 1.05, 1.05}; 	
	_MultipliersV[0] = 0.85;
	_MultipliersV[1] = 0.8; 	
	_MultipliersV[2] = 0.85; 	
	_MultipliersV[3] = 1.15; 	
	_MultipliersV[4] = 1.0; 	
	_MultipliersV[5] = 1.05; 	
	_MultipliersV[6] = 1.05; 	

	//_ThresholdsV[7] = {-553,-691,-793,-627,0,-684,-683};
	
	_ThresholdsV[0] = -1154;	//-568;	//-553;	
	_ThresholdsV[1] = -1125;	//-647;	//-691;
	_ThresholdsV[2] = -1195;	//-870;	//-793;
	_ThresholdsV[3] = -1330;	//-683;	//-627;
	_ThresholdsV[4] = 0;
	_ThresholdsV[5] = -1425;	//-702;	//-684;
	_ThresholdsV[6] = -1436;	//-737;	//-683;

/// Maximum image size on one axis
	_maxImageSize = 384;	

/// Maximum number of pixels in image
	_maxPixels = 384*384;

/// Threshold used to separate package from empty background (for the WORD range)
	_backgroundThreshold = 45000;	//50000;	//45000;	//ams changed May 7, 2010

/// Minimum X size of the package image after trimming white margins 
	_minSizeX = 230;

/// Minimum Y size of the package image after trimming white margins 
	_minSizeY = 200;	//200;//160; //200; //was 280; //ams changed to 160 on May5, 2010, was 200

/// Number of side rows in trimmed image where the package edges are searched for 
	_rowsEdge = 50;

/// Flag indicating whether package edges should be found and masked before dealing with package contents
	_processEdges = true;
// ----------------------------

// local buffers
	for (int i = 0; i<2;i++)
	{
		ImageTmpL1[i] = NULL;
		ImageTmpL2[i] = NULL;
		_MinimaColumnsDown[i] = NULL;
		_MinimaColumnsUp[i] = NULL;
		_MinimaColumns[i] = NULL;
		_MinimaRows[i] = NULL;
		_MinIndexesColumnsDown[i] = NULL;
		_MinIndexesColumnsUp[i] = NULL;
		_MinIndexesColumns[i] = NULL;
		_MinIndexesRows[i] = NULL;
		_ImageAd2[i] = NULL;
		_ImageAd1[i] = NULL;
		_ImageAd[i] = NULL;
		_ImageAbv5[i] = NULL;
		_ImageAbh5[i] = NULL;
		_ImageAb[i] = NULL;
		_ImageAlog0[i] = NULL;
		_ImageAlogT[i] = NULL;
		_ImageAlog[i] = NULL;
		_ImageCroppedT[i] = NULL;
		_ImageTmp2[i] = NULL;
		_ImageTmp1[i] = NULL;
	}

	inspxErrorTexts[0] = "OK";
	inspxErrorTexts[1] = "Memory not allocated";  				// -1
	inspxErrorTexts[2] = "Illegal argument value";	   			// -2
	inspxErrorTexts[3] = "In-place operation not supported";		// -3
	inspxErrorTexts[4] = "Package not found in image";			// -4
	inspxErrorTexts[5] = "Illegal image size";					// -5
								   
	// 	returned when error code is beyond limits						   
	inspxErrorTexts[6] = "Illegal error code";
}

CInspection::~CInspection()
{
	InspxContaminant22FreeBuffers();
}

IMPLEMENT_SERIAL(CInspection, CObject, VERSIONABLE_SCHEMA | cCurrentArchiveVersionNumber)

void CInspection::Serialize(CArchive& TempArchive)
{
  // call base class function first
  // base class is CObject in this case
  CObject::Serialize( TempArchive );

  // now do the stuff for our specific class
  if (TempArchive.IsStoring())
	{
    TempArchive << vEnable
			<< vName
			<< vInspectionType
			<< vInvertEjectLogic
			<< vROITop
			<< vROIBottom
			<< vROILeft
			<< vROIRight
			<< vThreshold
			<< vSizeToReadInOldStream
			<< vEjector
			<< vEnhanceMethod
			<< vSubtractAverage

			<< vAddHistogram
			<< vAddStructure
			<< vFillInLightSpots
			<< vMaximumDrop
			<< vFollowBottomUpAndDown
			<< vTryThresholdsDownToThreshold
			<< vHasDensityLowerThreshold
			<< vHasDensityUpperThreshold
			<< vAllowChangingAdvancedOptions
			<< vHasAMinimumSize

			<< vShowRejects
			<< vSaveRejects
			<< vFindDarkSpotOnLine
			<< vPixelsStepToNextROI
			<< vPerformGlassAlgorithm
			<< vSwungBaffleTolerance
			<< vTrimEdgesOfJar
			<< vTakeDerivative
			<< vErodeCountAtEnd
			<< vDilationCountAtEnd
			<< vOriginalImageFactor
			<< vEnhancedImageFactor
			<< vDilationCount
			<< vErodeCount

			<< vPreSmoothImage
			<< vSetupHistoryMinimumGoodCount
			<< vSetupHistoryMinimumGoodAverage
			<< vSetupHistoryMinimumGoodStandardDeviation

			<< vBonesInspection
			<< vSetupHistoryMaximumBadCount
			<< vSetupHistoryMaximumBadAverage
			<< vSetupHistoryMaximumBadStandardDeviation

			<< vSetupHistoryAutoThreshold
			<< vSetupHistoryQualityFactor
			<< vSetupHistoryMinimumGood
			<< vSetupHistoryMaximumBad
			<< vReferenceToTop

			<< vDesensitizeEdgePixels
			<< vRemoveJarPunt
			<< vFindSpotInSubROI

			<< vMaximumSize
			<< vReferenceToBottom
			<< vMaximumHeightAboveBottom

			<< vSize
			<< vSpareDWordI2
			<< vSpareDWordI3
			<< vSpareDWordI4
			<< vSpareDWordI5
			<< vMinimumWidth
			<< vMinimumNumberOfItemsRequired
			<< vMaximumNumberOfItemsRequired
			<< vSpareWordI3
			<< vSpareWordI4
			<< vSpareWordI5

			<< vSpareIntI1
			<< vSpareIntI2
			<< vSpareIntI3
			<< vSpareIntI4
			<< vSpareIntI5

			<< vCheckWeighNominalWeight
			<< vCheckWeighMFactor
			<< vCheckWeighBFactor
			<< vCheckWeighMaximumGoodWeight
			<< vCheckWeighMinimumGoodWeight

			<< vCheckWeighUnits
			<< vSpareCStringI1
			<< vSpareCStringI2
			<< vSpareCStringI3
			<< vSpareCStringI4
			<< vSpareCStringI5
			<< vDesensitizeIrregularArea
			<< vFillBackground
			<< vDoContaminant22Algorithm
			<< vTakeDifferenceFromMedian
			<< vTakeDifferenceFromMedianFilter
			<< vPixelLookAhead
			<< vLookForDarkSpots
			<< vMinimumLength
			<< vOverlap
			<< vErodeCountAtStart
			<< vCheckWeighTrendAlarmMaximum
			<< vCheckWeighTrendAlarmMinimum
			<< vSetupHistoryMinimumValue
			<< vSetupHistoryMaximumValue
			<< vSetupHistoryBadMinimumValue
			<< vSetupHistoryBadMaximumValue
			<< vInspectionNameForWeightTrendingName
			<< vCheckWeighOffset
			<< vIgnoreEdgePixels
			<< vMinimumBrightnessAceptable
			<< vSpareDoubleI2
			<< vSpareDoubleI3
			<< vSpareDoubleI4
			<< vSpareDoubleI5
			<< vWhiteOutAnythingTouchingTheTop
			<< vIPContaminant
			<< vSpareByteI3
			<< vSpareByteI4
			<< vSpareByteI5
			<< vSpareByteI6
			<< vSpareByteI7
			<< vSpareByteI8
			<< vSpareByteI9
			<< vSpareByteI10
			<< vSpareWordI6
			<< vSpareDWordI6;
	}
  else  //reading archive data
	{
		// may read different versions of object if have them
		// int vVersion = archive.GetObjectSchema();
    int TempVersion = TempArchive.GetObjectSchema();

		if (TempVersion > 156)
		{  // version 157 and up
			TempArchive >> vEnable
				>> vName
				>> vInspectionType
				>> vInvertEjectLogic
				>> vROITop
				>> vROIBottom
				>> vROILeft
				>> vROIRight
				>> vThreshold
				>> vSizeToReadInOldStream
				>> vEjector
				>> vEnhanceMethod
				>> vSubtractAverage

				>> vAddHistogram
				>> vAddStructure
				>> vFillInLightSpots
				>> vMaximumDrop
				>> vFollowBottomUpAndDown
				>> vTryThresholdsDownToThreshold
				>> vHasDensityLowerThreshold
				>> vHasDensityUpperThreshold
				>> vAllowChangingAdvancedOptions
				>> vHasAMinimumSize

				>> vShowRejects
				>> vSaveRejects
				>> vFindDarkSpotOnLine
				>> vPixelsStepToNextROI
				>> vPerformGlassAlgorithm
				>> vSwungBaffleTolerance
				>> vTrimEdgesOfJar
				>> vTakeDerivative
				>> vErodeCountAtEnd
				>> vDilationCountAtEnd
				>> vOriginalImageFactor
				>> vEnhancedImageFactor
				>> vDilationCount
				>> vErodeCount

				>> vPreSmoothImage
				>> vSetupHistoryMinimumGoodCount
				>> vSetupHistoryMinimumGoodAverage
				>> vSetupHistoryMinimumGoodStandardDeviation

				>> vBonesInspection
				>> vSetupHistoryMaximumBadCount
				>> vSetupHistoryMaximumBadAverage
				>> vSetupHistoryMaximumBadStandardDeviation

				>> vSetupHistoryAutoThreshold
				>> vSetupHistoryQualityFactor
				>> vSetupHistoryMinimumGood
				>> vSetupHistoryMaximumBad
				>> vReferenceToTop

				>> vDesensitizeEdgePixels
				>> vRemoveJarPunt
				>> vFindSpotInSubROI

				>> vMaximumSize
				>> vReferenceToBottom
				>> vMaximumHeightAboveBottom

				>> vSize
				>> vSpareDWordI2
				>> vSpareDWordI3
				>> vSpareDWordI4
				>> vSpareDWordI5
				>> vMinimumWidth
				>> vMinimumNumberOfItemsRequired
				>> vMaximumNumberOfItemsRequired
				>> vSpareWordI3
				>> vSpareWordI4
				>> vSpareWordI5

				>> vSpareIntI1
				>> vSpareIntI2
				>> vSpareIntI3
				>> vSpareIntI4
				>> vSpareIntI5

				>> vCheckWeighNominalWeight
				>> vCheckWeighMFactor
				>> vCheckWeighBFactor
				>> vCheckWeighMaximumGoodWeight
				>> vCheckWeighMinimumGoodWeight

				>> vCheckWeighUnits
				>> vSpareCStringI1
				>> vSpareCStringI2
				>> vSpareCStringI3
				>> vSpareCStringI4
				>> vSpareCStringI5
				>> vDesensitizeIrregularArea
				>> vFillBackground
				>> vDoContaminant22Algorithm
				>> vTakeDifferenceFromMedian
				>> vTakeDifferenceFromMedianFilter
				>> vPixelLookAhead
				>> vLookForDarkSpots
				>> vMinimumLength
				>> vOverlap
				>> vErodeCountAtStart
				>> vCheckWeighTrendAlarmMaximum
				>> vCheckWeighTrendAlarmMinimum
				>> vSetupHistoryMinimumValue
				>> vSetupHistoryMaximumValue
				>> vSetupHistoryBadMinimumValue
				>> vSetupHistoryBadMaximumValue
				>> vInspectionNameForWeightTrendingName
				>> vCheckWeighOffset
				>> vIgnoreEdgePixels
				>> vMinimumBrightnessAceptable
				>> vSpareDoubleI2
				>> vSpareDoubleI3
				>> vSpareDoubleI4
				>> vSpareDoubleI5
				>> vWhiteOutAnythingTouchingTheTop
				>> vIPContaminant
				>> vSpareByteI3
				>> vSpareByteI4
				>> vSpareByteI5
				>> vSpareByteI6
				>> vSpareByteI7
				>> vSpareByteI8
				>> vSpareByteI9
				>> vSpareByteI10
				>> vSpareWordI6
				>> vSpareDWordI6;
		} //end version 157 and up
		else
		if (TempVersion > 154)
		{  // version 155 and 156
			TempArchive >> vEnable
				>> vName
				>> vInspectionType
				>> vInvertEjectLogic
				>> vROITop
				>> vROIBottom
				>> vROILeft
				>> vROIRight
				>> vThreshold
				>> vSizeToReadInOldStream
				>> vEjectorVariableToReadInOldByte
				>> vEnhanceMethod
				>> vSubtractAverage

				>> vAddHistogram
				>> vAddStructure
				>> vFillInLightSpots
				>> vMaximumDrop
				>> vFollowBottomUpAndDown
				>> vTryThresholdsDownToThreshold
				>> vHasDensityLowerThreshold
				>> vHasDensityUpperThreshold
				>> vAllowChangingAdvancedOptions
				>> vHasAMinimumSize

				>> vShowRejects
				>> vSaveRejects
				>> vFindDarkSpotOnLine
				>> vPixelsStepToNextROI
				>> vPerformGlassAlgorithm
				>> vSwungBaffleTolerance
				>> vTrimEdgesOfJar
				>> vTakeDerivative
				>> vErodeCountAtEnd
				>> vDilationCountAtEnd
				>> vOriginalImageFactor
				>> vEnhancedImageFactor
				>> vDilationCount
				>> vErodeCount

				>> vPreSmoothImage
				>> vSetupHistoryMinimumGoodCount
				>> vSetupHistoryMinimumGoodAverage
				>> vSetupHistoryMinimumGoodStandardDeviation

				>> vBonesInspection
				>> vSetupHistoryMaximumBadCount
				>> vSetupHistoryMaximumBadAverage
				>> vSetupHistoryMaximumBadStandardDeviation

				>> vSetupHistoryAutoThreshold
				>> vSetupHistoryQualityFactor
				>> vSetupHistoryMinimumGood
				>> vSetupHistoryMaximumBad
				>> vReferenceToTop

				>> vDesensitizeEdgePixels
				>> vRemoveJarPunt
				>> vFindSpotInSubROI

				>> vMaximumSize
				>> vReferenceToBottom
				>> vMaximumHeightAboveBottom

				>> vSize
				>> vSpareDWordI2
				>> vSpareDWordI3
				>> vSpareDWordI4
				>> vSpareDWordI5
				>> vMinimumWidth
				>> vMinimumNumberOfItemsRequired
				>> vMaximumNumberOfItemsRequired
				>> vSpareWordI3
				>> vSpareWordI4
				>> vSpareWordI5

				>> vSpareIntI1
				>> vSpareIntI2
				>> vSpareIntI3
				>> vSpareIntI4
				>> vSpareIntI5

				>> vCheckWeighNominalWeight
				>> vCheckWeighMFactor
				>> vCheckWeighBFactor
				>> vCheckWeighMaximumGoodWeight
				>> vCheckWeighMinimumGoodWeight

				>> vCheckWeighUnits
				>> vSpareCStringI1
				>> vSpareCStringI2
				>> vSpareCStringI3
				>> vSpareCStringI4
				>> vSpareCStringI5
				>> vDesensitizeIrregularArea
				>> vFillBackground
				>> vDoContaminant22Algorithm
				>> vTakeDifferenceFromMedian
				>> vTakeDifferenceFromMedianFilter
				>> vPixelLookAhead
				>> vLookForDarkSpots
				>> vMinimumLength
				>> vOverlap
				>> vErodeCountAtStart
				>> vCheckWeighTrendAlarmMaximum
				>> vCheckWeighTrendAlarmMinimum
				>> vSetupHistoryMinimumValue
				>> vSetupHistoryMaximumValue
				>> vSetupHistoryBadMinimumValue
				>> vSetupHistoryBadMaximumValue
				>> vInspectionNameForWeightTrendingName
				>> vCheckWeighOffset
				>> vIgnoreEdgePixels
				>> vMinimumBrightnessAceptable
				>> vSpareDoubleI2
				>> vSpareDoubleI3
				>> vSpareDoubleI4
				>> vSpareDoubleI5
				>> vWhiteOutAnythingTouchingTheTop
				>> vIPContaminant
				>> vSpareByteI3
				>> vSpareByteI4
				>> vSpareByteI5
				>> vSpareByteI6
				>> vSpareByteI7
				>> vSpareByteI8
				>> vSpareByteI9
				>> vSpareByteI10
				>> vSpareWordI6
				>> vSpareDWordI6;
		} //end version 155 and 156
		else
		if (TempVersion > 152)
		{  // version 153 and 154
			TempArchive >> vEnable
				>> vName
				>> vInspectionType
				>> vInvertEjectLogic
				>> vROITop
				>> vROIBottom
				>> vROILeft
				>> vROIRight
				>> vThreshold
				>> vSizeToReadInOldStream
				>> vEjectorVariableToReadInOldByte
				>> vEnhanceMethod
				>> vSubtractAverage

				>> vAddHistogram
				>> vAddStructure
				>> vFillInLightSpots
				>> vMaximumDrop
				>> vFollowBottomUpAndDown
				>> vTryThresholdsDownToThreshold
				>> vHasDensityLowerThreshold
				>> vHasDensityUpperThreshold
				>> vAllowChangingAdvancedOptions
				>> vHasAMinimumSize

				>> vShowRejects
				>> vSaveRejects
				>> vFindDarkSpotOnLine
				>> vPixelsStepToNextROI
				>> vPerformGlassAlgorithm
				>> vSwungBaffleTolerance
				>> vTrimEdgesOfJar
				>> vTakeDerivative
				>> vErodeCountAtEnd
				>> vDilationCountAtEnd
				>> vOriginalImageFactor
				>> vEnhancedImageFactor
				>> vDilationCount
				>> vErodeCount

				>> vPreSmoothImage
				>> vSetupHistoryMinimumGoodCount
				>> vSetupHistoryMinimumGoodAverage
				>> vSetupHistoryMinimumGoodStandardDeviation

				>> vBonesInspection
				>> vSetupHistoryMaximumBadCount
				>> vSetupHistoryMaximumBadAverage
				>> vSetupHistoryMaximumBadStandardDeviation

				>> vSetupHistoryAutoThreshold
				>> vSetupHistoryQualityFactor
				>> vSetupHistoryMinimumGood
				>> vSetupHistoryMaximumBad
				>> vReferenceToTop

				>> vDesensitizeEdgePixels
				>> vRemoveJarPunt
				>> vFindSpotInSubROI

				>> vMaximumSize
				>> vReferenceToBottom
				>> vMaximumHeightAboveBottom

				>> vSize
				>> vSpareDWordI2
				>> vSpareDWordI3
				>> vSpareDWordI4
				>> vSpareDWordI5
				>> vMinimumWidth
				>> vMinimumNumberOfItemsRequired
				>> vMaximumNumberOfItemsRequired
				>> vSpareWordI3
				>> vSpareWordI4
				>> vSpareWordI5

				>> vSpareIntI1
				>> vSpareIntI2
				>> vSpareIntI3
				>> vSpareIntI4
				>> vSpareIntI5

				>> vCheckWeighNominalWeight
				>> vCheckWeighMFactor
				>> vCheckWeighBFactor
				>> vCheckWeighMaximumGoodWeight
				>> vCheckWeighMinimumGoodWeight

				>> vCheckWeighUnits
				>> vSpareCStringI1
				>> vSpareCStringI2
				>> vSpareCStringI3
				>> vSpareCStringI4
				>> vSpareCStringI5
				>> vDesensitizeIrregularArea
				>> vFillBackground
				>> vDoContaminant22Algorithm
				>> vTakeDifferenceFromMedian
				>> vTakeDifferenceFromMedianFilter
				>> vPixelLookAhead
				>> vLookForDarkSpots
				>> vMinimumLength
				>> vOverlap
				>> vErodeCountAtStart
				>> vCheckWeighTrendAlarmMaximum
				>> vCheckWeighTrendAlarmMinimum
				>> vSetupHistoryMinimumValue
				>> vSetupHistoryMaximumValue
				>> vSetupHistoryBadMinimumValue
				>> vSetupHistoryBadMaximumValue
				>> vInspectionNameForWeightTrendingName
				>> vCheckWeighOffset
				>> vIgnoreEdgePixels
				>> vMinimumBrightnessAceptable
				>> vSpareDoubleI2
				>> vSpareDoubleI3
				>> vSpareDoubleI4
				>> vSpareDoubleI5
				>> vWhiteOutAnythingTouchingTheTop
				>> vIPContaminant
				>> vSpareByteI3
				>> vSpareByteI4
				>> vSpareByteI5
				>> vSpareByteI6
				>> vSpareByteI7
				>> vSpareByteI8
				>> vSpareByteI9
				>> vSpareByteI10;
		} //end version 153 and 154
		else
		if (TempVersion > 149)
		{  // version 150 to 152
			TempArchive >> vEnable
				>> vName
				>> vInspectionType
				>> vInvertEjectLogic
				>> vROITop
				>> vROIBottom
				>> vROILeft
				>> vROIRight
				>> vThreshold
				>> vSizeToReadInOldStream
				>> vEjectorVariableToReadInOldByte
				>> vEnhanceMethod
				>> vSubtractAverage

				>> vAddHistogram
				>> vAddStructure
				>> vFillInLightSpots
				>> vMaximumDrop
				>> vFollowBottomUpAndDown
				>> vTryThresholdsDownToThreshold
				>> vHasDensityLowerThreshold
				>> vHasDensityUpperThreshold
				>> vAllowChangingAdvancedOptions
				>> vHasAMinimumSize

				>> vShowRejects
				>> vSaveRejects
				>> vFindDarkSpotOnLine
				>> vPixelsStepToNextROI
				>> vPerformGlassAlgorithm
				>> vSwungBaffleTolerance
				>> vTrimEdgesOfJar
				>> vTakeDerivative
				>> vErodeCountAtEnd
				>> vDilationCountAtEnd
				>> vOriginalImageFactor
				>> vEnhancedImageFactor
				>> vDilationCount
				>> vErodeCount

				>> vPreSmoothImage
				>> vSetupHistoryMinimumGoodCount
				>> vSetupHistoryMinimumGoodAverage
				>> vSetupHistoryMinimumGoodStandardDeviation

				>> vBonesInspection
				>> vSetupHistoryMaximumBadCount
				>> vSetupHistoryMaximumBadAverage
				>> vSetupHistoryMaximumBadStandardDeviation

				>> vSetupHistoryAutoThreshold
				>> vSetupHistoryQualityFactor
				>> vSetupHistoryMinimumGood
				>> vSetupHistoryMaximumBad
				>> vReferenceToTop

				>> vDesensitizeEdgePixels
				>> vRemoveJarPunt
				>> vFindSpotInSubROI

				>> vMaximumSize
				>> vReferenceToBottom
				>> vMaximumHeightAboveBottom
				>> vSpareWordI4
				>> vSpareWordI5

				>> vSize
				>> vSpareDWordI2
				>> vSpareDWordI3
				>> vSpareDWordI4
				>> vMinimumWidth
				>> vMinimumNumberOfItemsRequired

				>> vSpareIntI1
				>> vSpareIntI2
				>> vSpareIntI3
				>> vSpareIntI4
				>> vSpareIntI5

				>> vCheckWeighNominalWeight
				>> vCheckWeighMFactor
				>> vCheckWeighBFactor
				>> vCheckWeighMaximumGoodWeight
				>> vCheckWeighMinimumGoodWeight

				>> vCheckWeighUnits
				>> vSpareCStringI2
				>> vSpareCStringI3
				>> vSpareCStringI4
				>> vSpareCStringI5
				>> vDesensitizeIrregularArea
				>> vFillBackground
				>> vDoContaminant22Algorithm
				>> vTakeDifferenceFromMedian
				>> vTakeDifferenceFromMedianFilter
				>> vPixelLookAhead
				>> vLookForDarkSpots
				>> vMinimumLength
				>> vOverlap
				>> vErodeCountAtStart
				>> vCheckWeighTrendAlarmMaximum
				>> vCheckWeighTrendAlarmMinimum
				>> vSetupHistoryMinimumValue
				>> vSetupHistoryMaximumValue
				>> vSetupHistoryBadMinimumValue
				>> vSetupHistoryBadMaximumValue
				>> vInspectionNameForWeightTrendingName
				>> vSpareCStringI2
				>> vSpareCStringI3
				>> vSpareCStringI4
				>> vSpareCStringI5;

				for (BYTE TempLoop = 0; TempLoop < 14; TempLoop++)
					TempArchive >> vSpareDWordA[TempLoop];

				for (BYTE TempLoop = 0; TempLoop < 14; TempLoop++)
					TempArchive >> vSpareDoubleA[TempLoop];
			
				vCheckWeighOffset = vSpareDoubleA[0];

				for (BYTE TempLoop = 0; TempLoop < 14; TempLoop++)
					TempArchive >> vSpareIntA[TempLoop];

				for (BYTE TempLoop = 0; TempLoop < 13; TempLoop++)
					TempArchive >> vSpareByteA[TempLoop];

				TempArchive >> vIgnoreEdgePixels;
		} //end version 150-152
		else
		if (TempVersion > 147)
		{  // version 148 and 149
			TempArchive >> vEnable
				>> vName
				>> vInspectionType
				>> vInvertEjectLogic
				>> vROITop
				>> vROIBottom
				>> vROILeft
				>> vROIRight
				>> vThreshold
				>> vSizeToReadInOldStream
				>> vEjectorVariableToReadInOldByte
				>> vEnhanceMethod
				>> vSubtractAverage

				>> vAddHistogram
				>> vAddStructure
				>> vFillInLightSpots
				>> vMaximumDrop
				>> vFollowBottomUpAndDown
				>> vTryThresholdsDownToThreshold
				>> vHasDensityLowerThreshold
				>> vHasDensityUpperThreshold
				>> vAllowChangingAdvancedOptions
				>> vHasAMinimumSize

				>> vShowRejects
				>> vSaveRejects
				>> vFindDarkSpotOnLine
				>> vPixelsStepToNextROI
				>> vPerformGlassAlgorithm
				>> vSwungBaffleTolerance
				>> vTrimEdgesOfJar
				>> vTakeDerivative
				>> vErodeCountAtEnd
				>> vDilationCountAtEnd
				>> vOriginalImageFactor
				>> vEnhancedImageFactor
				>> vDilationCount
				>> vErodeCount

				>> vPreSmoothImage
				>> vSetupHistoryMinimumGoodCount
				>> vSetupHistoryMinimumGoodAverage
				>> vSetupHistoryMinimumGoodStandardDeviation

				>> vBonesInspection
				>> vSetupHistoryMaximumBadCount
				>> vSetupHistoryMaximumBadAverage
				>> vSetupHistoryMaximumBadStandardDeviation

				>> vSetupHistoryAutoThreshold
				>> vSetupHistoryQualityFactor
				>> vSetupHistoryMinimumGood
				>> vSetupHistoryMaximumBad
				>> vReferenceToTop

				>> vDesensitizeEdgePixels
				>> vRemoveJarPunt
				>> vFindSpotInSubROI

				>> vMaximumSize
				>> vReferenceToBottom
				>> vMaximumHeightAboveBottom
				>> vSpareWordI4
				>> vSpareWordI5

				>> vSize
				>> vSpareDWordI2
				>> vSpareDWordI3
				>> vSpareDWordI4
				>> vMinimumWidth
				>> vMinimumNumberOfItemsRequired

				>> vSpareIntI1
				>> vSpareIntI2
				>> vSpareIntI3
				>> vSpareIntI4
				>> vSpareIntI5

				>> vCheckWeighNominalWeight
				>> vCheckWeighMFactor
				>> vCheckWeighBFactor
				>> vCheckWeighMaximumGoodWeight
				>> vCheckWeighMinimumGoodWeight

				>> vCheckWeighUnits
				>> vSpareCStringI2
				>> vSpareCStringI3
				>> vSpareCStringI4
				>> vSpareCStringI5
				>> vDesensitizeIrregularArea
				>> vFillBackground
				>> vDoContaminant22Algorithm
				>> vTakeDifferenceFromMedian;

				for (BYTE TempLoop = 0; TempLoop < 14; TempLoop++)
					TempArchive >> vSpareDWordA[TempLoop];
				for (BYTE TempLoop = 0; TempLoop < 14; TempLoop++)
					TempArchive >> vSpareDoubleA[TempLoop];
				for (BYTE TempLoop = 0; TempLoop < 14; TempLoop++)
					TempArchive >> vSpareIntA[TempLoop];
				for (BYTE TempLoop = 0; TempLoop < 13; TempLoop++)
					TempArchive >> vSpareByteA[TempLoop];

				TempArchive >> vIgnoreEdgePixels;
		} //end version 148, 149
		else
		if ((TempVersion > 145)) // && (TempVersion < 31))
		{  // version 146-147
			TempArchive >> vEnable
				>> vName
				>> vInspectionType
				>> vInvertEjectLogic
				>> vROITop
				>> vROIBottom
				>> vROILeft
				>> vROIRight
				>> vThreshold
				>> vSizeToReadInOldStream
				>> vEjectorVariableToReadInOldByte
				>> vEnhanceMethod
				>> vSubtractAverage

				>> vAddHistogram
				>> vAddStructure
				>> vFillInLightSpots
				>> vMaximumDrop
				>> vFollowBottomUpAndDown
				>> vTryThresholdsDownToThreshold
				>> vHasDensityLowerThreshold
				>> vHasDensityUpperThreshold
				>> vAllowChangingAdvancedOptions
				>> vHasAMinimumSize

				>> vShowRejects
				>> vSaveRejects
				>> vFindDarkSpotOnLine
				>> vPixelsStepToNextROI
				>> vPerformGlassAlgorithm
				>> vSwungBaffleTolerance
				>> vTrimEdgesOfJar
				>> vTakeDerivative
				>> vErodeCountAtEnd
				>> vDilationCountAtEnd
				>> vOriginalImageFactor
				>> vEnhancedImageFactor
				>> vDilationCount
				>> vErodeCount

				>> vPreSmoothImage
				>> vSetupHistoryMinimumGoodCount
				>> vSetupHistoryMinimumGoodAverage
				>> vSetupHistoryMinimumGoodStandardDeviation

				>> vBonesInspection
				>> vSetupHistoryMaximumBadCount
				>> vSetupHistoryMaximumBadAverage
				>> vSetupHistoryMaximumBadStandardDeviation

				>> vSetupHistoryAutoThreshold
				>> vSetupHistoryQualityFactor
				>> vSetupHistoryMinimumGood
				>> vSetupHistoryMaximumBad
				>> vReferenceToTop

				>> vDesensitizeEdgePixels
				>> vRemoveJarPunt
				>> vFindSpotInSubROI

				>> vMaximumSize
				>> vReferenceToBottom
				>> vMaximumHeightAboveBottom
				>> vSpareWordI4
				>> vSpareWordI5

				>> vSize
				>> vSpareDWordI2
				>> vSpareDWordI3
				>> vSpareDWordI4
				>> vMinimumWidth
				>> vMinimumNumberOfItemsRequired

				>> vSpareIntI1
				>> vSpareIntI2
				>> vSpareIntI3
				>> vSpareIntI4
				>> vSpareIntI5

				>> vCheckWeighNominalWeight
				>> vCheckWeighMFactor
				>> vCheckWeighBFactor
				>> vCheckWeighMaximumGoodWeight
				>> vCheckWeighMinimumGoodWeight

				>> vCheckWeighUnits
				>> vSpareCStringI2
				>> vSpareCStringI3
				>> vSpareCStringI4
				>> vSpareCStringI5;

		} //end version 146-147
		else
		if ((TempVersion > 125) && (TempVersion < 146))
		{  // version 126 - 145
			TempArchive >> vEnable
				>> vName
				>> vInspectionType
				>> vInvertEjectLogic
				>> vROITop
				>> vROIBottom
				>> vROILeft
				>> vROIRight
				>> vThreshold
				>> vSizeToReadInOldStream
				>> vEjectorVariableToReadInOldByte
				>> vEnhanceMethod
				>> vSubtractAverage

				>> vAddHistogram
				>> vAddStructure
				>> vFillInLightSpots
				>> vMaximumDrop
				>> vFollowBottomUpAndDown
				>> vTryThresholdsDownToThreshold
				>> vHasDensityLowerThreshold
				>> vHasDensityUpperThreshold
				>> vAllowChangingAdvancedOptions
				>> vHasAMinimumSize

				>> vShowRejects
				>> vSaveRejects
				>> vFindDarkSpotOnLine
				>> vPixelsStepToNextROI
				>> vPerformGlassAlgorithm
				>> vSwungBaffleTolerance
				>> vTrimEdgesOfJar
				>> vTakeDerivative
				>> vErodeCountAtEnd
				>> vDilationCountAtEnd
				>> vOriginalImageFactor
				>> vEnhancedImageFactor
				>> vDilationCount
				>> vErodeCount

				>> vPreSmoothImage
				>> vSetupHistoryMinimumGoodCount
				>> vSetupHistoryMinimumGoodAverage
				>> vSetupHistoryMinimumGoodStandardDeviation

				>> vBonesInspection
				>> vSetupHistoryMaximumBadCount
				>> vSetupHistoryMaximumBadAverage
				>> vSetupHistoryMaximumBadStandardDeviation

				>> vSetupHistoryAutoThreshold
				>> vSetupHistoryQualityFactor
				>> vSetupHistoryMinimumGood
				>> vSetupHistoryMaximumBad
				>> vReferenceToTop;
		} //end version 126

		if (TempVersion < 157)
			vEjector = vEjectorVariableToReadInOldByte; //before version 157 was a byte.  Now it is a DWORD

		if (vPerformGlassAlgorithm)  //1 is curve fittin, 2 = line derivative which is Glass On Bottom
		if ((vPixelLookAhead < 1) || (vPixelLookAhead > 12))
			vPixelLookAhead = 3;

		if (TempVersion < 112)
		{
			vSetupHistoryMinimumGood = vPreSmoothImage;
		} //end version 112

		if (vSize == 0)
			vSize = vSizeToReadInOldStream;

		if ((vInspectionType != cCheckWeighInspection) && (vInspectionType != cUnderfillByWeightInspection) && (vInspectionType != cOverfillByWeightInspection))
		{
			vCheckWeighMFactor = 1;
			vCheckWeighBFactor = 0;
			vCheckWeighOffset = 0;
		}

		if ((vInspectionType == cContaminantInspection) ||
			(vInspectionType == cIPContaminantInspection) ||
			(vInspectionType == cWireContaminantInspection) ||
			(vInspectionType == cVoidInspection) ||
			(vInspectionType == cKernelInspection) ||
			(vInspectionType == cIPContaminantInspection) ||
			(vInspectionType == cJarBottomContaminantInspection))
		{
			vAllowChangingAdvancedOptions = true;
		}
		if ((vInspectionType == cContaminantInspection) ||
			(vInspectionType == cIPContaminantInspection) ||
			(vInspectionType == cWireContaminantInspection) ||
			(vInspectionType == cVoidInspection) ||
			(vInspectionType == cItemCountInspection) ||
			(vInspectionType == cSizeInspection) ||
			(vInspectionType == cMissingLidInspectionOnSize) ||
			(vInspectionType == cThresholdContaminantInspection) ||
			(vInspectionType == cBadHeartInspection) ||
			(vInspectionType == cDripBySpoutInspection) ||
			(vInspectionType == cDripInspection) ||
			(vInspectionType == cFloodedInspection) ||
			(vInspectionType == cGlass13mmContaminantInspection) ||
			(vInspectionType == cSpotInROIContaminantInspection) ||
			(vInspectionType == cAdvancedContaminant2Inspection) ||
			(vInspectionType == cContaminant2Inspection) ||
			(vInspectionType == cTopContaminantInspection) ||
			(vInspectionType == cBottomContaminantInspection) ||
			(vInspectionType == cContaminantBonesInspection) ||
			(vInspectionType == cContaminant22Inspection) ||
			(vInspectionType == cKernelInspection) ||
			(vInspectionType == cJarBottomContaminantInspection))
		{
			vHasAMinimumSize = true;
		}

		if (TempVersion < 148)
		{
			vFillBackground = 0;
			vDesensitizeIrregularArea = 0;
			vDesensitizeEdgePixels = 0;
		}

		if ((!vAllowChangingAdvancedOptions) && (vInspectionType == cContaminantInspection))
			vPerformGlassAlgorithm = 0;
		else
		if (vInspectionType != cGlass13mmContaminantInspection)
			vPerformGlassAlgorithm = 0;

		if ((vInspectionType == cItemCountInspection) || (vInspectionType == cSizeInspection) || (vInspectionType == cMissingLidInspectionOnSize))
		{
			vHasAMinimumSize = true;
		}

		if (TempVersion <= 145)
		{
			vShowRejects = true;
			vSaveRejects = true;
			//if (vInspectionType == cCheckWeighInspection)
			//{
			//	vShowRejects = false;
			//	vSaveRejects = false;
			//}
		}

		if (vInspectionType == cDensityInspection)
		{
			vHasDensityLowerThreshold = true;
			vHasDensityUpperThreshold = true;
		}
		if ((TempVersion < 148)) // versions 146 and less are upgrades
		if (vInspectionType != cContaminantBonesInspection)
		{
			vIgnoreEdgePixels = 0;
			vFillBackground = 0;
			//vMinimumAspectRatio = 0;  
			//vConnect4 = false;
		}
		if ((vSetupHistoryQualityFactor > 999999) || (vSetupHistoryQualityFactor < 0))
		{
			vSetupHistoryQualityFactor = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryQualityFactor", cEMailInspx, 32000);
		}
		if (vSetupHistoryMinimumGood < 0)
		{
			vSetupHistoryMinimumGood = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryMinimumGood", cEMailInspx, 32000);
		}
		if (vSetupHistoryMaximumBad < 0)
		{
			vSetupHistoryMaximumBad = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryMaximumBad", cEMailInspx, 32000);
		}
		if (vSetupHistoryMinimumGoodAverage < 0)
		{
			vSetupHistoryMinimumGoodAverage = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryMinimumGoodAverage", cEMailInspx, 32000);
		}
		if (vSetupHistoryMinimumGoodStandardDeviation < 0)
		{
			vSetupHistoryMinimumGoodStandardDeviation = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryMinimumGoodStandardDeviation", cEMailInspx, 32000);
		}
		if (vSetupHistoryMinimumValue < 0)
		{
			vSetupHistoryMinimumValue = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryMinimumValue", cEMailInspx, 32000);
		}
		if (vSetupHistoryMaximumValue < 0)
		{
			vSetupHistoryMaximumValue = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryMaximumValue", cEMailInspx, 32000);
		}
		if (vSetupHistoryBadMinimumValue < 0)
		{
			vSetupHistoryBadMinimumValue = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryBadMinimumValue", cEMailInspx, 32000);
		}
		if (vSetupHistoryBadMaximumValue < 0)
		{
			vSetupHistoryBadMaximumValue = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryBadMaximumValue", cEMailInspx, 32000);
		}
		if (vSetupHistoryMaximumBadStandardDeviation < 0)
		{
			vSetupHistoryMaximumBadStandardDeviation = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryMaximumBadStandardDeviation", cEMailInspx, 32000);
		}
		//if ((vOriginalImageFactor > 1) || (vOriginalImageFactor < 0))
		//{
		//	vOriginalImageFactor = 0;
		//	ReportErrorMessage("Bad float in stream corrected: vOriginalImageFactor", cEMailInspx, 32000);
		//}
		//if ((vEnhancedImageFactor > 1) || (vEnhancedImageFactor < 0))
		//{
		//	vEnhancedImageFactor = 0;
		//	ReportErrorMessage("Bad float in stream corrected: vEnhancedImageFactor", cEMailInspx, 32000);
		//}

		vOriginalImageFactor = 0;
		vEnhancedImageFactor = 0;

		if ((vROITop > 36) || (vROITop <= 0))
		{
		ReportErrorMessage("Bad float in stream " + vName + " corrected: vROITop was: " + dtoa(vROITop, 2), cEMailInspx, 32000);
		SetROITop(1);
		}
		if ((vROIBottom > vROITop) || (vROIBottom < 0))
		{
			ReportErrorMessage("Bad float in stream " + vName + " corrected: vROIBottom was: " + dtoa(vROIBottom, 2) + " vROITop was: " + dtoa(vROITop, 2), cEMailInspx, 32000);
			SetROIBottom(0);
		}
		
		if ((vROILeft > cMaximumImageWidth) || (vROILeft < 0))
		{
			ReportErrorMessage("Bad float in stream " + vName + " corrected: vROILeft was: " + dtoa(vROILeft, 2), cEMailInspx, 32000);
			SetROILeft(0, 1);
		}
		if ((vROIRight > cMaximumImageWidth) || (vROIRight < 0))
		{
			ReportErrorMessage("Bad float in stream " + vName + " corrected: vROIRight was:" + dtoa(vROIRight, 2), cEMailInspx, 32000);
			SetROIRight(vROILeft + 1, 1);
		}
		
		if (vSetupHistoryMinimumGoodCount > cStandardDeviationSampleSize)
		{
			vSetupHistoryMinimumGoodCount = 0;
			ReportErrorMessage("Bad value in stream corrected: vSetupHistoryMinimumGoodCount", cEMailInspx, 32000);
		}
		if (vSetupHistoryMaximumBadCount > cStandardDeviationSampleSize)
		{
			vSetupHistoryMaximumBadCount = 0;
			ReportErrorMessage("Bad value in stream corrected: vSetupHistoryMaximumBadCount", cEMailInspx, 32000);
		}
		if ((vCheckWeighMFactor > 1000000) || (vCheckWeighMFactor < -1000000))
		{
			vCheckWeighMFactor = 1;
			vCheckWeighOffset = 0;
			ReportErrorMessage("Bad float in stream corrected: vCheckWeighMFactor", cEMailInspx, 32000);
		}
		if ((vCheckWeighBFactor > 1000000) || (vCheckWeighBFactor < -1000000))
		{
			vCheckWeighBFactor = 0;
			vCheckWeighOffset = 0;
			ReportErrorMessage("Bad float in stream corrected: vCheckWeighBFactor", cEMailInspx, 32000);
		}
		if ((vCheckWeighOffset > 1000) || (vCheckWeighBFactor < -1000))
		{
			vCheckWeighOffset = 0;
			ReportErrorMessage("Bad float in stream corrected: vCheckWeighOffset", cEMailInspx, 32000);
		}
		if ((vCheckWeighMaximumGoodWeight > 1000000) || (vCheckWeighMaximumGoodWeight < -1000000))
		{
			vCheckWeighMaximumGoodWeight = 0;
			ReportErrorMessage("Bad float in stream corrected: vCheckWeighMaximumGoodWeight", cEMailInspx, 32000);
		}
		if ((vCheckWeighMinimumGoodWeight > 1000000) || (vCheckWeighMinimumGoodWeight < -1000000))
		{
			vCheckWeighMinimumGoodWeight = 0;
			ReportErrorMessage("Bad float in stream corrected: vCheckWeighMinimumGoodWeight", cEMailInspx, 32000);
		}
	} //end reading in archive
}

void CInspection::SetupInspection(double TempTop, double TempBottom, double TempLeft, double TempRight,BYTE TempOverScanMultiplier) //parameters passed in inches, not pixels
{
	SetROITop(TempTop);
	SetROIBottom(TempBottom);

	SetROIRight(TempRight, TempOverScanMultiplier);
	SetROILeft(TempLeft, TempOverScanMultiplier);
}

void CInspection::SetupInspectionInPixels(WORD TempTop, WORD TempBottom, WORD TempLeft, WORD TempRight,BYTE TempOverScanMultiplier) //parameters passed in pixels
{
	SetROITop(TempTop / vGlobalPixelsPerUnitInHeight);
	SetROIBottom(TempBottom / vGlobalPixelsPerUnitInHeight);

	SetROIRight(TempRight / (vGlobalPixelsPerUnit * TempOverScanMultiplier), TempOverScanMultiplier);
	SetROILeft(TempLeft / (vGlobalPixelsPerUnit * TempOverScanMultiplier), TempOverScanMultiplier);
}

void CInspection::SetupInspectionROIFromReference(WORD TempTop, WORD TempBottom, WORD TempLeft, WORD TempRight, WORD TempLowerRimHeight, BYTE TempOverScanMultiplier)  //in pixels
{
	if (TempBottom >= TempTop)
		TempBottom = 0;

	if (TempLeft >= TempRight)
		TempLeft = 0;

	if ((TempRight) && (TempTop))
	{
		WORD TempQuarterWidth = (WORD)((TempRight - TempLeft) / 4);
		WORD TempMiddle = TempLeft + (TempQuarterWidth * 2);
		switch (vInspectionType)
		{
			case cMissingLidInspection: 
			{  //missing lid density inspection
				if (TempLowerRimHeight > 30)
				{
					WORD TempCenter = TempLeft + ((TempRight - TempLeft) / 2);
					SetupInspectionInPixels(TempLowerRimHeight + 10, TempLowerRimHeight - 10, TempCenter - (TempQuarterWidth / 2), TempCenter + (TempQuarterWidth / 2), TempOverScanMultiplier);
				}
				else
					SetupInspectionInPixels((WORD)((double)TempTop *.95), (WORD)((double)TempTop *.7), TempLeft + TempQuarterWidth, TempRight - TempQuarterWidth, TempOverScanMultiplier);
			}
			break;
			case cUnderfillInspection: 
			{  //underfill density inspection
				if (TempLowerRimHeight > 30)
					SetupInspectionInPixels(TempLowerRimHeight - 10, TempLowerRimHeight - 20, TempLeft + TempQuarterWidth, TempRight - TempQuarterWidth, TempOverScanMultiplier);
				else
					SetupInspectionInPixels((WORD)((double)TempTop *.70), (WORD)((double)TempTop *.65), TempLeft + TempQuarterWidth, TempRight - TempQuarterWidth, TempOverScanMultiplier);
			}
			break;
			case cUnderfillByWeightInspection:
			case cOverfillByWeightInspection:
			case cCheckWeighInspection:
			{  //underfill weight inspection
				SetupInspectionInPixels(TempTop, TempTop / 2, TempLeft, TempRight, TempOverScanMultiplier);
			}
			break;
			case cContaminant2Inspection:
			case cBottomContaminantInspection://Learned contaminant inspection
			case cAdvancedContaminant2Inspection:
			case cSpotInROIContaminantInspection://Spot in ROI inspection
			case cIPContaminantInspection:
			case cContaminantInspection://advanced contaminant inspection
			case cWireContaminantInspection:
			case cVoidInspection:  //void inspection
			case cStandardDeviationInspection:  //texture inspection
			case cItemCountInspection:  //size inspection
			case cSizeInspection:  //size inspection
			case cKernelInspection: //kernel contaminant inspection
			case cContaminant22Inspection:
			case cContaminantBonesInspection:
			{  
				if (TempLowerRimHeight > 30)
					SetupInspectionInPixels(TempLowerRimHeight - 10, 10, TempLeft + 1, TempRight - 1, TempOverScanMultiplier);
				else
					SetupInspectionInPixels((WORD)((double)TempTop * 0.73), 10, TempLeft + 1, TempRight - 1, TempOverScanMultiplier);
			}
			break;
			case cMissingLidInspectionOnSize:
			{  
				WORD TempCenter = TempLeft + ((TempRight - TempLeft) / 2);

				//SetupInspectionInPixels((WORD)((double)TempTop * 0.9), (WORD)((double)TempTop * 0.50), TempCenter - (TempQuarterWidth / 2), TempCenter + (TempQuarterWidth / 2), TempOverScanMultiplier);

				if (TempLowerRimHeight > 30)
					SetupInspectionInPixels(TempLowerRimHeight + 20, TempLowerRimHeight - 20, TempCenter - (TempQuarterWidth / 2), TempCenter + (TempQuarterWidth / 2), TempOverScanMultiplier);
				else
					SetupInspectionInPixels((WORD)((double)TempTop *.95), (WORD)((double)TempTop *.5), TempCenter - (TempQuarterWidth / 2), TempCenter + (TempQuarterWidth / 2), TempOverScanMultiplier);
			}
			break;
			case cTopContaminantInspection:
			{  //Top Learned contaminant inspection
				if (TempLowerRimHeight > 30)
					SetupInspectionInPixels(TempTop, TempLowerRimHeight - 20, TempLeft + 1, TempRight - 1, TempOverScanMultiplier);
				else
					SetupInspectionInPixels(TempTop, (WORD)((double)TempTop * 0.7), TempLeft, TempRight, TempOverScanMultiplier);
			}
			break;
			case cGlass13mmContaminantInspection://Glass contaminant inspection
			{  
				if (vPerformGlassAlgorithm == 2)
				{
					if (vTrimEdgesOfJar)
						SetupInspectionInPixels(30, 6, TempLeft, TempRight, TempOverScanMultiplier);
					else
					{
						WORD TempWidth = 10;
						if (TempRight > TempLeft)
						{
							TempWidth = TempRight - TempLeft;
							TempWidth = (WORD)(TempWidth * .15);
						}

						SetupInspectionInPixels(30, 6, TempLeft + TempWidth, TempRight - TempWidth, TempOverScanMultiplier);
					}
				}
				else
					SetupInspectionInPixels((WORD)((double)TempTop * 0.7), 1, TempLeft, TempRight, TempOverScanMultiplier);
			}
			break;
			case cJarBottomContaminantInspection://jar bottom inspection
			{ 
				SetupInspectionInPixels(55, 3, TempLeft + (TempQuarterWidth / 2), TempRight - (TempQuarterWidth / 2), TempOverScanMultiplier);
			}
			break;
			case cOverfillInspection: //Overfill density inspection
			{  
				if (TempLowerRimHeight > 30)
					SetupInspectionInPixels(TempTop, TempLowerRimHeight + 10, TempLeft + TempQuarterWidth, TempRight - TempQuarterWidth, TempOverScanMultiplier);
				else
					SetupInspectionInPixels((WORD)((double)TempTop *.95), (WORD)((double)TempTop *.90), TempLeft + TempQuarterWidth, TempRight - TempQuarterWidth, TempOverScanMultiplier);
			}
			break;
			case cThresholdContaminantInspection://simple contaminant, straight threshold
			case cDensityInspection://advanced density inspection
				if (TempLowerRimHeight > 30)
					SetupInspectionInPixels(TempLowerRimHeight - 10, 10, TempLeft + 1, TempRight - 1, TempOverScanMultiplier);
				else
					SetupInspectionInPixels((WORD)((double)TempTop *.66), 10, TempLeft + 1, TempRight - 1, TempOverScanMultiplier);
			break;
			case cDripInspection:
				SetupInspectionInPixels(TempTop, 0, 0, TempRight, TempOverScanMultiplier);
			break;
			case cDripBySpoutInspection:
				SetupInspectionInPixels(TempBottom + 6, TempBottom, TempMiddle - TempQuarterWidth, TempMiddle + TempQuarterWidth, TempOverScanMultiplier);
			break;
			case cFloodedInspection:
				SetupInspectionInPixels(TempTop, 0, TempLeft + 10, TempRight - 10, TempOverScanMultiplier);
			break;
			case cBadHeartInspection:
				SetupInspectionInPixels(TempTop, 0, 0, TempRight, TempOverScanMultiplier);
			break;
		}
	}
}

void CInspection::CopyInspection(CInspection *TempInspectionToCopy, BYTE TempOverScanMultiplier)
{
	vEnable = TempInspectionToCopy->vEnable;

	vSaveRejects = TempInspectionToCopy->vSaveRejects;
	vShowRejects = TempInspectionToCopy->vShowRejects;

	vName = TempInspectionToCopy->vName;
	vInvertEjectLogic = TempInspectionToCopy->vInvertEjectLogic;
	vInspectionType = TempInspectionToCopy->vInspectionType;
	SetROITop(TempInspectionToCopy->vROITop);
	SetROIBottom(TempInspectionToCopy->vROIBottom);
	SetROILeft(TempInspectionToCopy->vROILeft, TempOverScanMultiplier);
	SetROIRight(TempInspectionToCopy->vROIRight, TempOverScanMultiplier);
	vThreshold = TempInspectionToCopy->vThreshold;
	vSize = TempInspectionToCopy->vSize;
	vMaximumSize = TempInspectionToCopy->vMaximumSize;
	vMaximumHeightAboveBottom = TempInspectionToCopy->vMaximumHeightAboveBottom;
	vMinimumWidth = TempInspectionToCopy->vMinimumWidth;
	vMinimumNumberOfItemsRequired = TempInspectionToCopy->vMinimumNumberOfItemsRequired;
	vMaximumNumberOfItemsRequired = TempInspectionToCopy->vMaximumNumberOfItemsRequired;

	vPreSmoothImage = TempInspectionToCopy->vPreSmoothImage;
	vMaximumDrop = TempInspectionToCopy->vMaximumDrop;

	vEjector = TempInspectionToCopy->vEjector;
	vEnhanceMethod = TempInspectionToCopy->vEnhanceMethod;
	vSubtractAverage = TempInspectionToCopy->vSubtractAverage;
	vHasDensityUpperThreshold = TempInspectionToCopy->vHasDensityUpperThreshold;
	vHasDensityLowerThreshold = TempInspectionToCopy->vHasDensityLowerThreshold;
	SetDensityLowerThreshold(TempInspectionToCopy->vDensityThresholdLower);
	vDensityThresholdUpper = TempInspectionToCopy->vDensityThresholdUpper;
	vMinimumBrightnessAceptable = TempInspectionToCopy->vMinimumBrightnessAceptable;

	vPerformGlassAlgorithm = TempInspectionToCopy->vPerformGlassAlgorithm;
	vDesensitizeEdgePixels = TempInspectionToCopy->vDesensitizeEdgePixels;
	vDesensitizeIrregularArea = TempInspectionToCopy->vDesensitizeIrregularArea;
	vWhiteOutAnythingTouchingTheTop = TempInspectionToCopy->vWhiteOutAnythingTouchingTheTop;
	vIPContaminant = TempInspectionToCopy->vIPContaminant;

	vTakeDifferenceFromMedianFilter = TempInspectionToCopy->vTakeDifferenceFromMedianFilter;
	vBonesInspection = TempInspectionToCopy->vBonesInspection;
	vFindSpotInSubROI = TempInspectionToCopy->vFindSpotInSubROI;
	vPixelLookAhead = TempInspectionToCopy->vPixelLookAhead;
	vLookForDarkSpots = TempInspectionToCopy->vLookForDarkSpots;
	vOverlap = TempInspectionToCopy->vOverlap;
	vPixelsStepToNextROI = TempInspectionToCopy->vPixelsStepToNextROI;
	vSwungBaffleTolerance = TempInspectionToCopy->vSwungBaffleTolerance;
	vTrimEdgesOfJar = TempInspectionToCopy->vTrimEdgesOfJar;
	vTakeDerivative = TempInspectionToCopy->vTakeDerivative;
	vFillInLightSpots = TempInspectionToCopy->vFillInLightSpots;
	vFollowBottomUpAndDown = TempInspectionToCopy->vFollowBottomUpAndDown;
	vTryThresholdsDownToThreshold = TempInspectionToCopy->vTryThresholdsDownToThreshold;
	vTakeDifferenceFromMedian = TempInspectionToCopy->vTakeDifferenceFromMedian;

	vCheckWeighTrendAlarmMinimum = TempInspectionToCopy->vCheckWeighTrendAlarmMinimum;
	vCheckWeighTrendAlarmMaximum = TempInspectionToCopy->vCheckWeighTrendAlarmMaximum;
	vCheckWeighNominalWeight = TempInspectionToCopy->vCheckWeighNominalWeight;
	vCheckWeighMFactor = TempInspectionToCopy->vCheckWeighMFactor;
	vCheckWeighOffset = TempInspectionToCopy->vCheckWeighOffset;
	vCheckWeighBFactor = TempInspectionToCopy->vCheckWeighBFactor;
	vCheckWeighUnits = TempInspectionToCopy->vCheckWeighUnits;
	vCheckWeighMaximumGoodWeight = TempInspectionToCopy->vCheckWeighMaximumGoodWeight;
	vCheckWeighMinimumGoodWeight = TempInspectionToCopy->vCheckWeighMinimumGoodWeight;
	vInspectionNameForWeightTrendingName = TempInspectionToCopy->vInspectionNameForWeightTrendingName;
	vInspectionNameForWeightTrendingPointer = TempInspectionToCopy->vInspectionNameForWeightTrendingPointer;

	vAllowChangingAdvancedOptions = TempInspectionToCopy->vAllowChangingAdvancedOptions;

	vHasAMinimumSize = TempInspectionToCopy->vHasAMinimumSize;
	vFindDarkSpotOnLine = TempInspectionToCopy->vFindDarkSpotOnLine;

	vAddHistogram = TempInspectionToCopy->vAddHistogram;
	vAddStructure = TempInspectionToCopy->vAddStructure;
	vErodeCountAtEnd = TempInspectionToCopy->vErodeCountAtEnd;
	vDilationCountAtEnd = TempInspectionToCopy->vDilationCountAtEnd;
	//vOriginalImageFactor = TempInspectionToCopy->vOriginalImageFactor;
	//vEnhancedImageFactor = TempInspectionToCopy->vEnhancedImageFactor;
	vDilationCount = TempInspectionToCopy->vDilationCount;
	vErodeCount = TempInspectionToCopy->vErodeCount;
	vDoContaminant22Algorithm = TempInspectionToCopy->vDoContaminant22Algorithm;

	vSetupHistoryMinimumGood = TempInspectionToCopy->vSetupHistoryMinimumGood;
	vSetupHistoryMinimumGoodCount = TempInspectionToCopy->vSetupHistoryMinimumGoodCount;
	vSetupHistoryMinimumGoodAverage = TempInspectionToCopy->vSetupHistoryMinimumGoodAverage;
	vSetupHistoryMinimumGoodStandardDeviation = TempInspectionToCopy->vSetupHistoryMinimumGoodStandardDeviation;

	vSetupHistoryMaximumBad = TempInspectionToCopy->vSetupHistoryMaximumBad;
	vSetupHistoryMaximumBadCount = TempInspectionToCopy->vSetupHistoryMaximumBadCount;
	vSetupHistoryMaximumBadAverage = TempInspectionToCopy->vSetupHistoryMaximumBadAverage;
	vSetupHistoryMaximumBadStandardDeviation = TempInspectionToCopy->vSetupHistoryMaximumBadStandardDeviation;
	vSetupHistoryQualityFactor = TempInspectionToCopy->vSetupHistoryQualityFactor;

	vSetupHistoryAutoThreshold = TempInspectionToCopy->vSetupHistoryAutoThreshold;
	vReferenceToTop = TempInspectionToCopy->vReferenceToTop;
	vReferenceToBottom = TempInspectionToCopy->vReferenceToBottom;
	vRemoveJarPunt = TempInspectionToCopy->vRemoveJarPunt;

	vIgnoreEdgePixels = TempInspectionToCopy->vIgnoreEdgePixels;
	vMinimumLength = TempInspectionToCopy->vMinimumLength;  
	vFillBackground = TempInspectionToCopy->vFillBackground;
	//vMinimumAspectRatio = TempInspectionToCopy->vMinimumAspectRatio;  
	//vConnect4 = TempInspectionToCopy->vConnect4;

	vSetupHistoryDensityAutoThresholdUpper = TempInspectionToCopy->vSetupHistoryDensityAutoThresholdUpper;
	vSetupHistoryDensityAutoThresholdLower = TempInspectionToCopy->vSetupHistoryDensityAutoThresholdLower;
	vSetupHistoryMinimumValue = TempInspectionToCopy->vSetupHistoryMinimumValue;
	vSetupHistoryMaximumValue = TempInspectionToCopy->vSetupHistoryMaximumValue;
	vSetupHistoryBadMinimumValue = TempInspectionToCopy->vSetupHistoryBadMinimumValue;
	vSetupHistoryBadMaximumValue = TempInspectionToCopy->vSetupHistoryBadMaximumValue;
}

CString CInspection::MakeSetupDisplayString(bool TempReferenceEnabled, 
	BYTE TempOverScanFactor, CString TempEjectorNameString, CString TempUnitsString)
{
	CString TempStep1 = "Step 1";
	CString TempStep2 = "Step 2";
	CString TempStep3 = "Step 3";
	CString TempStep3a = "Step 3a";
	CString TempStep3b = "Step 3b";
	CString TempStep4 = "Step 4";
	CString TempStep5 = "Step 5";
	CString TempStep6 = "Step 6";
	CString TempStep7 = "Step 7";
	CString TempStep8 = "Step 8";
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		TempStep1 = "Reference";
		TempStep2 = "Average";
		TempStep3 = "Structure";
		TempStep3a = "Contaminant Structure";
		TempStep3b = "Void Structure";
		TempStep4 = "Histogram";
		TempStep5 = "Enhance";
		TempStep6 = "Dilation";
		TempStep7 = "Image Factors";
		TempStep8 = "Glass";
	}
	CString vTextString = "";
	vTextString = vTextString + "\n    Name: " + vName;
	if (!vEnable)
		vTextString = vTextString + "\n    Inspection Disabled";

	vTextString = vTextString + "\n    Inspection Type: " + GetInspectionTypeString();

	if (vInvertEjectLogic)
		vTextString = vTextString + "\n**++    Inverting Eject Logic";

	vTextString = vTextString + "\n\n    Threshold: " + dtoa(vThreshold, 0);
	if ((vInspectionType == cItemCountInspection) || (vInspectionType == cMissingLidInspectionOnSize) || (vInspectionType == cSizeInspection))
	{
		vTextString = vTextString + "\n    Minimum Acceptable Size: " + dtoa(vSize, 0);
		vTextString = vTextString + "\n    Maximum Acceptable Size: " + dtoa(vMaximumSize, 0);
	}
	if (vInspectionType == cItemCountInspection)
	{
		vTextString = vTextString + "\n    Minimum Number Of Items: " + dtoa(vMinimumNumberOfItemsRequired, 0);
		vTextString = vTextString + "\n    Maximum Number Of Items: " + dtoa(vMaximumNumberOfItemsRequired, 0);
	}

	if (PasswordOK(cTemporaryInspxPassword,false))
	if (vDoContaminant22Algorithm)
	{
		CString TempString = "    Contaminant 22 H Thres:\t";
		for (BYTE TempLoop = 0; TempLoop < 7; TempLoop++)
		{
			TempString = TempString + dtoa(_ThresholdsH[TempLoop],0);
			if (TempLoop < 6)
				TempString = TempString + ", ";
		}
		TempString = TempString + "\n    Default H: \t\t-499, -622, -688, -675, 0, -732, -789";
		TempString = TempString + "\n    Contaminant 22 V Thres:\t";
		for (BYTE TempLoop = 0; TempLoop < 7; TempLoop++)
		{
			TempString = TempString + dtoa(_ThresholdsV[TempLoop],0);
			if (TempLoop < 6)
				TempString = TempString + ", ";
		}
		TempString = TempString + "\n    Default V: \t\t-553, -691, -793, -627, 0, -684, -683";
		vTextString = vTextString + "\n" + TempString;
	}

	CString TempNumberString = " ";
	if (vSetupHistoryAutoThreshold)
		TempNumberString = dtoa(vSetupHistoryAutoThreshold,0);
	else
	if (vSetupHistoryMinimumGoodCount)
		TempNumberString = "Evaluate Not successful";
	else
		TempNumberString = "Evaluate Not Done";

	vTextString = vTextString + "\n    Evaluate Threshold: " + TempNumberString;
	//if ((vInspectionType == cItemCountInspection) || (vInspectionType == cMissingLidInspectionOnSize) || (vInspectionType == cSizeInspection))
	//	vTextString = vTextString + "\n    Evaluate Size: " + dtoa(vSetupHistoryDensityAutoThresholdUpper, 0);

	if (vInspectionType == cJarBottomContaminantInspection)
	{
		vTextString = vTextString + "\n\n    Maximum Bump Up Allowed: " + dtoa(vSize,0);
		vTextString = vTextString + "\n    Minimum Drop Allowed: " + dtoa(vMaximumDrop,0);
		if (vTryThresholdsDownToThreshold)
			vTextString = vTextString + "\n    Try Thresholds Down To: " + dtoa(vTryThresholdsDownToThreshold,0);
		else
			vTextString = vTextString + "\n    Try Only Set Threshold";
		if (vFollowBottomUpAndDown)
			vTextString = vTextString + "\n    Follow Bottom Up and Down";
		else
			vTextString = vTextString + "\n    Follow Bottom Down Only";
	}
	else
	if (vInspectionType == cKernelInspection)
		vTextString = vTextString + "\n\n    Kernel Size: " + dtoa(vSize,0);
	else
	if (vInspectionType == cDripInspection)
	{
		vTextString = vTextString + "\n\n    Minimum Defect Size: " + dtoa(vSize,0);
		if (vMaximumSize)
			vTextString = vTextString + "\n    Maximum Defect Size: " + dtoa(vMaximumSize,0);
		if (vMinimumWidth)
			vTextString = vTextString + "\n    Minimum Defect Length: " + dtoa(vMinimumWidth,0);
		if (vMaximumHeightAboveBottom)
			vTextString = vTextString + "\n    Maximum Defect Centroid above container bottom: " + dtoa(vMaximumHeightAboveBottom,0);
	}
	else
		vTextString = vTextString + "\n\n    Minimum Defect Size: " + dtoa(vSize,0);


	if ((vInspectionNameForWeightTrendingPointer) && (vInspectionNameForWeightTrendingName.GetLength()))
	{
		vTextString = vTextString + "\n\n    Using Inspection: " + vInspectionNameForWeightTrendingName + " Weight Calibration";
	}
	else
	{
		if (vSetupHistoryMinimumGoodCount)
		{
			if (vInspectionType != cJarBottomContaminantInspection)
			{
				vTextString = vTextString + "\n\n    Evaluate Good Average Intensity: " + dtoa(vSetupHistoryMinimumGoodAverage,2);
				vTextString = vTextString + "\n    Evaluate Good Standard Deviation: " + dtoa(vSetupHistoryMinimumGoodStandardDeviation,2);
			}
			vTextString = vTextString + "\n    Evaluate Good Minimum Intensity: " + dtoa(vSetupHistoryMinimumGood,0);
			vTextString = vTextString + "\n    Evaluate Good Count: " + dtoa(vSetupHistoryMinimumGoodCount, 0);
		}
		else
		{
			vTextString = vTextString + "\n\n    Evaluate Good Not Done";
		}

		if (vInspectionType != cJarBottomContaminantInspection)
		if (vSetupHistoryMaximumBadCount)
		{
			TempNumberString = dtoa(vSetupHistoryMaximumBadAverage,2);
			vTextString = vTextString + "\n\n    Evaluate Bad Average Intensity: " + TempNumberString;
			TempNumberString = dtoa(vSetupHistoryMaximumBadStandardDeviation,2);
			vTextString = vTextString + "\n    Evaluate Bad Standard Deviation: " + TempNumberString;
			vTextString = vTextString + "\n    Evaluate Bad, Maximum Intensity: " + dtoa(vSetupHistoryMaximumBad,0);
			vTextString = vTextString + "\n    Evaluate Bad Count: " + dtoa(vSetupHistoryMaximumBadCount, 0);
			vTextString = vTextString + "\n    Evaluate Quality Factor (>1 good): " + dtoa(vSetupHistoryQualityFactor,2);
		}
		else
		{
			vTextString = vTextString + "\n\n    Evaluate Bad Not Done";
		}
	}

	if (vReferenceToTop)
		vTextString = vTextString + "\n\n++    Reference To Top";
	else
	if (vInspectionType != cJarBottomContaminantInspection)
	if (vAllowChangingAdvancedOptions)
		vTextString = vTextString + "\n\n    Reference To Bottom";

	if ((vInspectionNameForWeightTrendingPointer) && (vInspectionNameForWeightTrendingName.GetLength()))
	{
		vTextString = vTextString + "\n\n    Using Inspection: " + vInspectionNameForWeightTrendingName + " for ROI";
	}
	else
	{
		vTextString = vTextString + "\n\n    Inspection ROI Top: " + dtoa(vROITop,2) + " " + TempUnitsString + ", " + dtoa(vROIPixelTop,0) + " Pixels";
		vTextString = vTextString + "\n    Inspection ROI Bottom: " + dtoa(vROIBottom,2) + " " + TempUnitsString + ", " + dtoa(vROIPixelBottom,0) + " Pixels";
		vTextString = vTextString + "\n    Inspection ROI Left: " + dtoa(vROILeft,2) + " " + TempUnitsString + ", " + dtoa(vROIPixelLeft,0) + " Pixels";
		vTextString = vTextString + "\n    Inspection ROI Right: " + dtoa(vROIRight,2) + " " + TempUnitsString + ", " + dtoa(vROIPixelRight,0) + " Pixels";
	}

	if (vInspectionType != cJarBottomContaminantInspection)
	if ((vAllowChangingAdvancedOptions) || (PasswordOK(cTemporaryInspxPassword,false)))
	{
		vTextString = vTextString + "\n";
		
		if (vAllowChangingAdvancedOptions)
			vTextString = vTextString + "\n    Advanced Inspection";

		if (vErodeCount)
		{
			vTextString = vTextString + "\n    " + TempStep6 + " Parameter 1: " + dtoa(vErodeCount, 0);
		}
		else
			vTextString = vTextString + "\n    " + TempStep6 + " Parameter 1: Disabled";

		if (vDilationCount)
		{
			vTextString = vTextString + "\n    " + TempStep6 + " Parameter 2: " + dtoa(vDilationCount, 0);
		}
		else
			vTextString = vTextString + "\n    " + TempStep6 + " Parameter 2: Disabled";

		if (vPreSmoothImage)
		{
			if (PasswordOK(cTemporaryInspxPassword,false))
				vTextString = vTextString + "\n    Pre-Smooth Image";
			else
				vTextString = vTextString + "\n    Step P";
		}
		
		if (vTakeDifferenceFromMedianFilter) //wire
		{
			CString TempText = "";
			if (PasswordOK(cTemporaryInspxPassword,false))
				vTextString = vTextString + "\n    Difference From Median Filter: " + dtoa(vTakeDifferenceFromMedianFilter,0);
			else
				vTextString = vTextString + "\n    Small Bones: " + dtoa(vTakeDifferenceFromMedianFilter,0);
		}
		else
		{
			vTextString = vTextString + "\n    Small bones Disabled";
		}

		if (vBonesInspection) //large bones
		{
			vTextString = vTextString + "\n    Large Bones: " + dtoa(vBonesInspection,0);
		}

		if (vTakeDifferenceFromMedian) //bones
		{
			CString TempText = "";
			if (PasswordOK(cTemporaryInspxPassword,false))
			{
				/*
				if (vTakeDifferenceFromMedian & 0x0F)
					TempText = dtoa(vTakeDifferenceFromMedian & 0x0F, 0) + "H";
				if (vTakeDifferenceFromMedian & 0xF0)
				{
					if (vTakeDifferenceFromMedian & 0x0F)
						TempText = TempText + ", ";
					TempText = TempText + dtoa((vTakeDifferenceFromMedian & 0xF0) >> 4, 0) + "V";
				}
				*/
					TempText = TempText + dtoa(vTakeDifferenceFromMedian, 0);

				if (vMinimumLength)
					TempText = TempText + "\n    Minimum Length: " + dtoa(vMinimumLength, 0);

				if (vFillBackground)
					TempText = TempText + "\n    Fill Background";

				//if (vMinimumAspectRatio)
				//	TempText = TempText + "\n    Minimum Aspect Ratio: " + dtoa(vMinimumAspectRatio, 2);

				//if (vConnect4)
				//	TempText = TempText + "\n    Connect4";
				
				if (vIgnoreEdgePixels)
					TempText = TempText + "\n    Ignore Edge Pixels: " + dtoa(vIgnoreEdgePixels, 0);
			}

			vTextString = vTextString + "\n    Find Bones: " + TempText;
		}
		else
		{
			vTextString = vTextString + "\n    Find Bones Disabled";
		}

		if (vFindSpotInSubROI)
		{
			if (vFindSpotInSubROI < 13)
				vTextString = vTextString + "\n    Small Spot Algorithm: " + dtoa(vFindSpotInSubROI,0) + ", " + dtoa(vPixelsStepToNextROI,0);
			else
				vTextString = vTextString + "\n    Medium Spot Algorithm: " + dtoa(vFindSpotInSubROI,0) + ", " + dtoa(vPixelsStepToNextROI,0);
		}

		if (vFindDarkSpotOnLine)
				vTextString = vTextString + "\n    Spot Segment Algorithm: " + dtoa(vFindDarkSpotOnLine,0) + ", " + dtoa(vPixelsStepToNextROI,0);

		if (vTakeDerivative)
		{
			if (PasswordOK(cTemporaryInspxPassword,false))
				vTextString = vTextString + "\n    Take Derivative";
			else
				vTextString = vTextString + "\n    Step D";
		}
		else
			if (PasswordOK(cTemporaryInspxPassword,false))
				vTextString = vTextString + "\n    Derivative Disabled";
			else
				vTextString = vTextString + "\n    Step D Disabled";

		if (vTakeDifferenceFromMedian)
		{
			CString TempText = "";
			if (vTakeDifferenceFromMedian & 0x0F)
				TempText = dtoa(vTakeDifferenceFromMedian & 0x0F, 0) + "H";
			if (vTakeDifferenceFromMedian & 0xF0)
			{
				if (vTakeDifferenceFromMedian & 0x0F)
					TempText = TempText + ", ";
				TempText = TempText + dtoa((vTakeDifferenceFromMedian & 0xF0) >> 4, 0) + "V";
			}
			/*
			if (vIgnoreQualifier)
				TempText = TempText + ", 2 Matrixes";
			else
				TempText = TempText + ", 1 Matrix";
				*/

			vTextString = vTextString + "\n    Find Bones: " + TempText;
		}
		else
		{
			vTextString = vTextString + "\n    Find Bones Disabled";
		}

		if (vPerformGlassAlgorithm)
		{
			if (PasswordOK(cTemporaryInspxPassword,false))
			{
				if (vPerformGlassAlgorithm == 1)
					vTextString = vTextString + "\n    Glass Curve";
				else
					vTextString = vTextString + "\n    Glass Derivative";
				if (vRemoveJarPunt)
					vTextString = vTextString + "\n    Remove Punt";
			}
			else
			{
				vTextString = vTextString + "\n    Step G";
				if (vRemoveJarPunt)
					vTextString = vTextString + "P";
			}
			if (vSwungBaffleTolerance)
				vTextString = vTextString + "\n    Swung Baffle: " + dtoa(vSwungBaffleTolerance,0);
		}

		if (vLookForDarkSpots)
			vTextString = vTextString + "\n    Wire: " + dtoa(vPixelLookAhead, 0) + ", " + dtoa(vOverlap, 0);

		if (vDoContaminant22Algorithm)
		{
			vTextString = vTextString + "\n    Contaminant22";
		}

		if (vDoContaminant22Algorithm)
		{
			vTextString = vTextString + "\n    Contaminant22";
		}

		if (vSubtractAverage)
		{
			vTextString = vTextString + "\n    " + TempStep2 + " Enabled";
		}
		else
			vTextString = vTextString + "\n    " + TempStep2 + " Disabled";

		if (vAddStructure)
		{
			vTextString = vTextString + "\n    " + TempStep3 + " Enabled";
		}
		else
			vTextString = vTextString + "\n    " + TempStep3 + " Disabled";
		
		if (vAddHistogram)
		{
			vTextString = vTextString + "\n    " + TempStep4 + " Enabled";
		}
		else
			vTextString = vTextString + "\n    " + TempStep4 + " Disabled";

		if (vEnhanceMethod)
		{
			CString TempString2 = "";
			if (PasswordOK(cTemporaryInspxPassword,false))
			switch (vEnhanceMethod)
			{
				case 1: TempString2 = "SobelX";
				break;
				case 2: TempString2 = "SobelY";
				break;
				case 3: TempString2 = "SobelXY";
				break;
				case 4: TempString2 = "PrewittX";
				break;
				case 5: TempString2 = "PrewittY";
				break;
				case 6: TempString2 = "PrewittXY";
				break;
				case 7: TempString2 = "Erode";
				break;
				case 8: TempString2 = "Convolve 3 X 3";
				break;
				case 9: TempString2 = "Convolve 5 X 5";
				break;
				case 10: TempString2 = "Convolve 5 X 5 (2 times)";
				break;
				case 11: TempString2 = "Convolve 7 X 7";
				break;
				case 12: TempString2 = "Convolve 3 X 3 then SobelXY";
				break;
			}
			vTextString = vTextString + "\n    " + TempString2 + " " + TempStep5 + " Enabled";
		}
		else
			vTextString = vTextString + "\n    " + TempStep5 + " Disabled";

		if (vErodeCountAtEnd)
		{
			vTextString = vTextString + "\n    " + TempStep6 + " Parameter 1: " + dtoa(vErodeCountAtEnd, 0);
		}
		else
			vTextString = vTextString + "\n    " + TempStep6 + " Parameter 1: Disabled";

		if (vDilationCountAtEnd)
		{
			vTextString = vTextString + "\n    " + TempStep6 + " Parameter 2: " + dtoa(vDilationCountAtEnd, 0);
		}
		else
			vTextString = vTextString + "\n    " + TempStep6 + " Parameter 2: Disabled";

		//if (vDoImageFactors)
		//{
		//	vTextString = vTextString + "\n    " + TempStep7 + " Enabled";
		//	vTextString = vTextString + "\n        " + TempStep7 + " Parameter 1: " + dtoa(vOriginalImageFactor,2);
		//	vTextString = vTextString + "\n        " + TempStep7 + " Parameter 2: " + dtoa(vEnhancedImageFactor,2);
		//}
		//else
		//	vTextString = vTextString + "\n    " + TempStep7 + " Disabled";
	}
	vTextString = vTextString + "\n\n    Ejector: " + TempEjectorNameString;

	return vTextString;
}

void CInspection::SetDefaultROI(WORD TempTop, WORD TempBottom, WORD TempLeft, WORD TempRight, BYTE TempOverScanMultiplier) //in pixels
{
	//9/9/2009
	SetROITop(TempTop / vGlobalPixelsPerUnitInHeight);
	SetROIBottom(TempBottom / vGlobalPixelsPerUnitInHeight);
	SetROILeft(TempLeft / vGlobalPixelsPerUnit / TempOverScanMultiplier, TempOverScanMultiplier);
	SetROIRight(TempRight / vGlobalPixelsPerUnit / TempOverScanMultiplier, TempOverScanMultiplier);
}

void CInspection::SetROITop(double TempTop)
{
	vROITop = TempTop;
	if (vROITop > 36)
		vROITop = 36;
	vROIPixelTop = (WORD)(TempTop * vGlobalPixelsPerUnitInHeight + .49);
}

void CInspection::SetROIBottom(double TempBottom)
{
	vROIBottom = TempBottom;
	vROIPixelBottom = (WORD)(TempBottom * vGlobalPixelsPerUnitInHeight + .49);
}

void CInspection::SetROILeft(double TempLeft,BYTE vOverScanMultiplier)
{
	vROILeft = TempLeft;
	vROIPixelLeft = (WORD)(TempLeft * (vGlobalPixelsPerUnit * vOverScanMultiplier) + .49);
}

void CInspection::SetROIRight(double TempRight,BYTE vOverScanMultiplier)
{
	vROIRight = TempRight;
	vROIPixelRight = (WORD)(TempRight * (vGlobalPixelsPerUnit * vOverScanMultiplier) + .49);
}

void CInspection::ClearInspectionHistory()
{
	vSetupHistoryMinimumGood = 255;
	vSetupHistoryMinimumGoodCount = 0;
	vSetupHistoryMinimumGoodAverage = 0;
	vSetupHistoryMinimumGoodStandardDeviation = 0;

	vSetupHistoryMaximumBad = 0;
	vSetupHistoryMaximumBadCount = 0;
	vSetupHistoryMaximumBadAverage = 0;
	vSetupHistoryMaximumBadStandardDeviation = 0;
	vSetupHistoryQualityFactor = 0;

	vSetupHistoryMinimumValue = 0;
	vSetupHistoryMaximumValue = 0;
	vSetupHistoryBadMinimumValue = 0;
	vSetupHistoryBadMaximumValue = 0;
	vSetupHistoryAutoThreshold = 0;
	vSetupHistoryDensityAutoThresholdLower = 0;
	vSetupHistoryDensityAutoThresholdUpper = 0;
}

void CInspection::SetDensityLowerThreshold(double TempThreshold)
{
	if (vHasDensityLowerThreshold)
	{
		vDensityThresholdLower = (float)TempThreshold;
		vThreshold = (BYTE)TempThreshold;
	}
	else
	if (TempThreshold == 0)
		vDensityThresholdLower = 0;
}

CString CInspection::GetInspectionTypeString()
{
	CString TempString("Undefined");
	switch (vInspectionType)
	{
		case cWireContaminantInspection:
			TempString = "Wire";
		break;
		case cContaminantInspection:
			TempString.LoadString(IDS_ContaminantAdvanced);
		break;
		case cSpotInROIContaminantInspection:
			TempString.LoadString(IDS_SpotInROI);
		break;
		case cVoidInspection:
			TempString.LoadString(IDS_Void);
		break;
		case cDensityInspection:
			TempString.LoadString(IDS_Density);
		break;
		case cCheckWeighInspection:
			TempString.LoadString(IDS_CheckWeigh);
		break;
		case cKernelInspection:
			TempString.LoadString(IDS_ContaminantKernel);
		break;
		case cItemCountInspection:
			TempString = "Item Count";
		break;
		case cSizeInspection:
			//TempString.LoadString(IDS_Size);
			TempString = "Insert";
		break;
		case cMissingLidInspectionOnSize:
			TempString.LoadString(IDS_MissingLid);
			TempString = TempString + " on Size";
		break;
		case cStandardDeviationInspection:
			TempString.LoadString(IDS_Texture);
		break;
		case cJarBottomContaminantInspection:
			TempString.LoadString(IDS_JarBottom);
		break;
		case cIPContaminantInspection:
			TempString.LoadString(IDS_IPContaminant);
		break;
		case cMissingLidInspection:
			TempString.LoadString(IDS_MissingLid);
		break;
		case cUnderfillInspection:
			TempString.LoadString(IDS_Underfill);
		break;
		case cUnderfillByWeightInspection:
			TempString.LoadString(IDS_UnderfillByWeight);
		break;
		case cOverfillByWeightInspection:
			TempString.LoadString(IDS_OverfillByWeight);
		break;
		case cThresholdContaminantInspection:
			TempString.LoadString(IDS_ContaminantThreshold);
		break;
		case cBadHeartInspection:
			TempString = "Bad Heart";
		break;
		case cDripBySpoutInspection:
			TempString = "Drip By Spout Inspection";
		break;
		case cDripInspection:
			TempString = "Drip Inspection";
		break;
		case cFloodedInspection:
			TempString = "Flooded IV Bag Inspection";
		break;
		case cTopContaminantInspection:
			TempString.LoadString(IDS_ContaminantTop);
		break;
		case cOverfillInspection:
			TempString.LoadString(IDS_Overfill);
		break;
		case cBottomContaminantInspection:
			TempString.LoadString(IDS_ForeignMaterial);
		break;
		case cContaminant2Inspection:
			TempString.LoadString(IDS_Contaminant2);
		break;
		case cAdvancedContaminant2Inspection:
			TempString.LoadString(IDS_AdvancedContaminant2);
		break;
		case cGlass13mmContaminantInspection:
			if (vPerformGlassAlgorithm == 2)
				TempString = "GlassD";
			else
			if (vPerformGlassAlgorithm == 1)
				TempString = "GlassC";
			else
				TempString = "GlassU";
		break;
		case cContaminantBonesInspection:
			TempString = "Bones";
		break;
		case cContaminant22Inspection:
			TempString = "Contaminant 22";
		break;
	}
	return TempString;
}

//void CInspection::FillInLightSpots(BYTE *TempImage, WORD TempSizeX, WORD TempSizeY, BYTE TempLightSpotsType)
//{
//	if (vEnable)
//	{
//		WORD TempBottom = vROIPixelBottom;
//		if (TempBottom >= TempSizeX)
//			TempBottom = 0;
//		WORD TempLeft = vROIPixelLeft;
//		WORD TempRight = vROIPixelRight;
//		//if (TempLeft < 5)
//		//	TempLeft = 5;
//		//if (TempRight + 5 > TempSizeY)
//		//	TempRight = TempSizeY - 5;
//
//		WORD TempHeight = vROIPixelTop - TempBottom;
//		if (TempHeight + TempBottom > TempSizeX)
//			TempHeight = TempSizeX - TempBottom;
//
//		WORD TempWidth = TempRight - vROIPixelLeft;
//		if (TempWidth + TempLeft > TempSizeY)
//			TempWidth = TempSizeY - TempLeft;
//
//		//if (TempWidth + 10 > TempSizeY)
//		//	TempWidth = TempSizeY - 10;
//
//		WORD TempHalfWidth = TempWidth / 2;
//		WORD TempCenter = TempLeft + TempHalfWidth;
//
//		if (TempImage)
//		{
//			BYTE *TempCurrentPixel = NULL;
//			BYTE *TempMaximumPointer = TempImage + (TempSizeY * TempSizeX);
//			//scanning from left to right, eliminate white spots on each line
//			for (WORD TempLineLoop = 0; TempLineLoop <= TempWidth; TempLineLoop++)
//			{
//				//start at the bottom ROI line and study every line up to the top ROI line
//				TempCurrentPixel = TempImage + TempBottom + ((TempLineLoop + TempLeft) * TempSizeX);
//				BYTE TempMedianValueOfLine = FindMedianValue(TempCurrentPixel, TempHeight);
//				TempCurrentPixel = TempImage + TempBottom + ((TempLineLoop + TempLeft) * TempSizeX);
//
//				for (WORD TempPixelLoop = 0; TempPixelLoop <= TempHeight; TempPixelLoop++)
//				{
//					if (TempCurrentPixel < TempMaximumPointer)
//					{
//						if (*TempCurrentPixel > TempMedianValueOfLine)
//							*TempCurrentPixel = TempMedianValueOfLine;
//
//						TempCurrentPixel++;
//					}
//					else
//						ReportErrorMessage("Bad Pointer 1w", cEMailInspx,32000);
//				}
//			}
//		}
//	}
//	//if (vEnable)
//	//{
//	//	WORD TempBottom = vROIPixelBottom;
//	//	if (TempBottom >= TempSizeX)
//	//		TempBottom = 0;
//	//	WORD TempLeft = vROIPixelLeft;
//	//	WORD TempRight = vROIPixelRight;
//	//	if (TempLeft < 5)
//	//		TempLeft = 5;
//	//	if (TempRight + 5 > TempSizeY)
//	//		TempRight = TempSizeY - 5;
//
//	//	WORD TempHeight = vROIPixelTop - TempBottom;
//	//	if (TempHeight + TempBottom > TempSizeX)
//	//		TempHeight = TempSizeX - TempBottom;
//
//	//	WORD TempWidth = TempRight - vROIPixelLeft;
//	//	if (TempWidth + TempLeft > TempSizeY)
//	//		TempWidth = TempSizeY - TempLeft;
//
//	//	if (TempWidth + 10 > TempSizeY)
//	//		TempWidth = TempSizeY - 10;
//
//	//	WORD TempHalfWidth = TempWidth / 2;
//	//	WORD TempCenter = TempLeft + TempHalfWidth;
//	//	WORD TempSideOverlap = TempOverlap;
//	//	if (TempImage)
//	//	{
//	//		BYTE *TempCurrentPixel = NULL;
//	//		BYTE *TempNextPixel = NULL;
//	//		BYTE *TempMaximumPointer = TempImage + (TempSizeY * TempSizeX);
//	//		//start at the bottom ROI line and study every line up to the top ROI line
//	//		for (WORD TempLineLoop = 0; TempLineLoop <= TempHeight; TempLineLoop++)
//	//		{
//	//			//scanning from center to left, eliminate white spots
//	//			BYTE TempNumberOfPixelsToLookAheadForWhiteSpots = TempLightSpotWidth;
//	//			for (WORD TempPixelLoop = 0; TempPixelLoop < TempHalfWidth + TempSideOverlap; TempPixelLoop++)
//	//			{
//	//				TempCurrentPixel = TempImage + TempLineLoop + TempBottom + ((TempCenter + TempSideOverlap - TempPixelLoop) * TempSizeX);
//	//				if ((TempCurrentPixel < TempMaximumPointer) && (TempCurrentPixel >= TempImage))
//	//				{
//	//					BYTE TempEndCount = 0;
//	//					bool TempFoundAPeak = false;
//	//					TempNextPixel = TempCurrentPixel - TempSizeX;
//	//					//check next 10 pixels for a white spot
//
//	//					for (BYTE TempCheckLoop = 0; TempCheckLoop < TempNumberOfPixelsToLookAheadForWhiteSpots; TempCheckLoop++)  
//	//					if (TempCheckLoop + TempPixelLoop < TempHalfWidth + TempSideOverlap)
//	//					if ((TempNextPixel < TempMaximumPointer) && (TempNextPixel >= TempImage))
//	//					{
//	//						if ((TempFoundAPeak) && (*TempNextPixel < *TempCurrentPixel + 4)) //must be within 5 to find end of white spot
//	//						{
//	//							BYTE *TempPixelAfterNext = TempNextPixel - TempSizeX;
//	//							//if the next two pixels are not significantly darker like a real contaminant
//	//							if ((*TempNextPixel + 10 > *TempCurrentPixel) && (*TempPixelAfterNext + 10 > *TempCurrentPixel))
//	//								TempEndCount = TempCheckLoop;
//	//							TempCheckLoop = TempNumberOfPixelsToLookAheadForWhiteSpots;
//	//						}
//	//						else
//	//						if (*TempNextPixel > *TempCurrentPixel + 4) //must be 10 higher to find a peak
//	//							TempFoundAPeak = true;
//	//						else
//	//						if (*TempNextPixel <= *TempCurrentPixel) //next pixel smaller, so abort checking at this pixel
//	//							TempCheckLoop = TempNumberOfPixelsToLookAheadForWhiteSpots;
//
//	//						TempNextPixel = TempNextPixel - TempSizeX;
//	//					}
//	//					else
//	//						ReportErrorMessage("Bad Pointer 2", cEMailInspx,32000);
//	//					if ((TempFoundAPeak) && (TempEndCount))  //if you found a white spot, clip it to the start pixel value
//	//					{
//	//						TempNextPixel = TempCurrentPixel - TempSizeX;
//	//						for (DWORD TempCheckLoop = 0; TempCheckLoop < TempEndCount; TempCheckLoop++)
//	//						{
//	//							*TempNextPixel = *TempCurrentPixel;
//	//							TempNextPixel = TempNextPixel - TempSizeX;
//	//						}
//	//					}
//	//				}
//	//				else
//	//					ReportErrorMessage("Bad Pointer 1", cEMailInspx,32000);
//	//			}
//
//	//			//scanning from center to right, eliminate white spots
//	//			for (WORD TempPixelLoop = 0; TempPixelLoop < TempHalfWidth + TempSideOverlap; TempPixelLoop++)
//	//			{
//	//				TempCurrentPixel = TempImage + TempLineLoop + TempBottom + ((TempCenter - TempSideOverlap + TempPixelLoop) * TempSizeX);
//	//				if ((TempCurrentPixel < TempMaximumPointer) && (TempCurrentPixel >= TempImage))
//	//				{
//	//					BYTE TempEndCount = 0;
//	//					bool TempFoundAPeak = false;
//	//					TempNextPixel = TempCurrentPixel + TempSizeX;
//	//					//check next 10 pixels for a white spot
//	//					for (BYTE TempCheckLoop = 0; TempCheckLoop < TempNumberOfPixelsToLookAheadForWhiteSpots; TempCheckLoop++)  
//	//					if (TempCheckLoop + TempPixelLoop < TempHalfWidth + TempSideOverlap)
//	//					if ((TempNextPixel < TempMaximumPointer) && (TempNextPixel >= TempImage))
//	//					{
//	//						if ((TempFoundAPeak) && (*TempNextPixel < *TempCurrentPixel + 4)) //must be within 5 to find end of white spot
//	//						{
//	//							BYTE *TempPixelAfterNext = TempNextPixel + TempSizeX;
//	//							//if the next two pixels are not significantly darker like a real contaminant
//	//							if ((*TempNextPixel + 10 > *TempCurrentPixel) && (*TempPixelAfterNext + 10 > *TempCurrentPixel))
//	//								TempEndCount = TempCheckLoop;
//	//							TempCheckLoop = TempNumberOfPixelsToLookAheadForWhiteSpots;
//	//						}
//	//						else
//	//						if (*TempNextPixel > *TempCurrentPixel + 4) //must be 10 higher to find a peak was 4
//	//							TempFoundAPeak = true;
//	//						else
//	//						if (*TempNextPixel <= *TempCurrentPixel) //next pixel smaller, so abort checking at this pixel
//	//							TempCheckLoop = TempNumberOfPixelsToLookAheadForWhiteSpots;
//
//	//						TempNextPixel = TempNextPixel + TempSizeX;
//	//					}
//	//					else
//	//						ReportErrorMessage("Bad Pointer 4", cEMailInspx,32000);
//	//					if ((TempFoundAPeak) && (TempEndCount))  //if you found a white spot, clip it to the start pixel value
//	//					{
//	//						TempNextPixel = TempCurrentPixel + TempSizeX;
//	//						for (BYTE TempCheckLoop = 0; TempCheckLoop < TempEndCount; TempCheckLoop++)
//	//						{
//	//							*TempNextPixel = *TempCurrentPixel;
//	//							TempNextPixel = TempNextPixel + TempSizeX;
//	//						}
//	//					}
//	//				}
//	//				else
//	//					ReportErrorMessage("Bad Pointer 3", cEMailInspx,32000);
//	//			}
//	//		}
//	//	}
//	//}
//}

void CInspection::CalculateLineDerivative(BYTE *TempImage, BYTE *TempScratchImage, WORD TempSizeX, WORD TempSizeY, double TempImageBrightness)
{
	BYTE TempRequire2InARow = true;
	if (vEnable)
	{
		DWORD TempSize = TempSizeX * TempSizeY;

		WORD TempTop = vROIPixelTop;
		WORD TempBottom = vROIPixelBottom;
		if (TempTop >= TempSizeX)
			TempTop = TempSizeX - 1;
		if (TempBottom >= TempTop)
			TempBottom = 2;
		WORD TempLeft = vROIPixelLeft;
		WORD TempRight = vROIPixelRight;
		if (TempRight + 5 > TempSizeY)
			TempRight = TempSizeY - 5;
		if (TempLeft >= TempRight)
			TempLeft = 0;
		if (TempLeft < 5)
			TempLeft = 5;

		WORD TempHeight = vROIPixelTop - TempBottom;
		if (TempHeight + TempBottom > TempSizeX)
			TempHeight = TempSizeX - TempBottom;

		WORD TempWidth = TempRight - TempLeft;
		if (TempWidth + TempLeft + 5 > TempSizeY)
			TempWidth = TempSizeY - TempLeft - 5;

		WORD TempHalfWidth = TempWidth / 2;
		WORD TempCenter = TempLeft + TempHalfWidth;
		WORD TempSideOverlap = 0;

		//ReportErrorMessage("Glass On Bottom Left: " + dtoa(TempLeft,0) + " Right: " + dtoa(TempRight,0) + " Bottom: " +
		//	dtoa(TempBottom,0) + " Top: " + dtoa(TempTop,0) + " Width: " + dtoa(TempWidth,0), cWriteToLog,0);

		if (TempImage)
		if (TempScratchImage)
		if (TempWidth < 20)
		{
			ReportErrorMessage("Notice: Glass On Bottom too few pixels, Ignore Inspection ", cWriteToLog,0);
		}
		else
		{
			//copy original image to scratch image
			CopyMemory(TempScratchImage, TempImage, TempSize);
			/*BYTE *TempSourcePointer = TempImage;
			BYTE *TempDestinationPointer = TempScratchImage;
			for (DWORD TempLoop = 0; TempLoop < TempSize; TempLoop++)
				*TempDestinationPointer++ = *TempSourcePointer++;
				*/

			//clear original image to white to use as the destination image for the results
			FillMemory(TempImage, TempSize, 255);
			/*
			TempDestinationPointer = TempImage;
			for (TempLoop = 0; TempLoop < TempSize; TempLoop++)
				*TempDestinationPointer++ = 255;
				*/

			BYTE *TempPreviousPixel = NULL;
			BYTE *TempCurrentPixel = NULL;
			BYTE *TempNextPixel = NULL;
			BYTE *TempNextPixelAfterThat = NULL;
			BYTE *TempResultPixel = NULL;
			BYTE *TempNextResultPixel = NULL;
			BYTE *TempMaximumScratchPointer = TempScratchImage + (TempSizeY * TempSizeX);
			BYTE *TempMaximumResultPointer = TempImage + (TempSizeY * TempSizeX);
			bool TempFoundDarkeningArea = false;

			//find punt height above bottom ROI line
			BYTE TempThreshold = (BYTE)(TempImageBrightness / 2);
			BYTE TempPuntHeight = 15;
			if (TempBottom < 14)
			for (WORD TempLineLoop = 0; TempLineLoop < TempHeight; TempLineLoop++)
			{
				TempCurrentPixel = TempScratchImage + TempLineLoop + TempBottom + (TempCenter * TempSizeX);
				TempNextPixel = TempCurrentPixel + 1;
				TempNextPixelAfterThat = TempNextPixel + 1;
				if ((*TempCurrentPixel > TempThreshold) && (*TempNextPixel > TempThreshold) && (*TempNextPixelAfterThat > TempThreshold))
				{
					TempPuntHeight = TempLineLoop + 2;
					//TempLineLoop = TempHeight;
				}
			}

			//ReportErrorMessage("Glass On Bottom Punt Height: " + dtoa(TempPuntHeight,0), cWriteToLog,0);
			if (TempPuntHeight + TempBottom > TempSizeX)
			{
				ReportErrorMessage("Error-Punt Height Extreme, set default", cEMailMaintenance,30);
				TempPuntHeight = TempSizeX - 5 - TempBottom;
			}

			//start at the bottom ROI line and study every line up to the top ROI line
			for (WORD TempLineLoop = 0; TempLineLoop < TempHeight; TempLineLoop++)
			{
				//scanning from center to left, indicate where image gets darker than is

				//if the line you are checking is above the punt, then start much further to the side from the center to overlap the center
				if (TempLineLoop > TempPuntHeight)
				{
					TempSideOverlap = TempWidth / 3;
				}
				if ((TempSideOverlap + TempCenter) + 10 > TempRight)
					TempSideOverlap = TempRight - 10 - TempCenter;

				if ((TempCenter - TempSideOverlap - 10) < TempLeft)
					TempSideOverlap = TempCenter - TempLeft - 10;

				TempCurrentPixel = TempScratchImage + TempLineLoop + TempBottom + ((TempCenter + TempSideOverlap) * TempSizeX);
				TempPreviousPixel = TempCurrentPixel + TempSizeX;
				TempNextPixel = TempCurrentPixel - TempSizeX;
				TempNextPixelAfterThat = TempNextPixel - TempSizeX;
				TempResultPixel = TempImage + TempLineLoop + TempBottom + ((TempCenter + TempSideOverlap) * TempSizeX);

				//scanning from center to Left ROI Line, indicate where image gets darker than is, output image is how much next pixel got darker
				for (WORD TempPixelLoop = 0; TempPixelLoop < TempHalfWidth + TempSideOverlap; TempPixelLoop++)
				if (*TempCurrentPixel < 255)
				if ((TempNextPixelAfterThat < TempMaximumScratchPointer) && (TempNextPixelAfterThat >= TempScratchImage) &&
					(TempResultPixel < TempMaximumResultPointer) && (TempResultPixel >= TempImage))
				{
					if ((*TempNextPixel < *TempCurrentPixel) && ((!TempRequire2InARow) || ((*TempNextPixelAfterThat < *TempCurrentPixel) &&
						(*TempNextPixel < *TempPreviousPixel) && (*TempNextPixelAfterThat < *TempPreviousPixel))))
					{
						if (*TempResultPixel == 255)  //if no previous value, set it
							*TempResultPixel = 255 - (*TempCurrentPixel - *TempNextPixel);
						else
						{  //if previous value, and this one smaller, set it, else keep previous value
							BYTE TempByte = 255 - (*TempCurrentPixel - *TempNextPixel);
							if (TempByte < *TempResultPixel)
							*TempResultPixel = TempByte;
						}

						//save greatest difference
						if (TempRequire2InARow)
						{
							TempNextResultPixel = TempResultPixel - TempSizeX;
							if (*TempCurrentPixel > *TempNextPixelAfterThat)
								*TempNextResultPixel = 255 - (*TempCurrentPixel - *TempNextPixelAfterThat);
							else
								*TempNextResultPixel = 255 - (*TempNextPixel - *TempNextPixelAfterThat);
						}

					}
					//else  //dont need to do as already set to 255
					//	*TempResultPixel = 255;

					TempPreviousPixel = TempCurrentPixel;
					TempCurrentPixel = TempNextPixel;
					TempNextPixel = TempNextPixelAfterThat;
					TempNextPixelAfterThat = TempNextPixelAfterThat - TempSizeX;
					TempResultPixel = TempResultPixel - TempSizeX;
				}
				//scanning from center to right ROI Line, indicate where image gets darker than is, output image is how much next pixel got darker
				TempCurrentPixel = TempScratchImage + TempLineLoop + TempBottom + ((TempCenter - TempSideOverlap) * TempSizeX);
				TempPreviousPixel = TempNextPixel - TempSizeX;
				TempNextPixel = TempCurrentPixel + TempSizeX;
				TempNextPixelAfterThat = TempNextPixel + TempSizeX;
				TempResultPixel = TempImage + TempLineLoop + TempBottom + ((TempCenter - TempSideOverlap) * TempSizeX);

				for (WORD TempPixelLoop = 0; TempPixelLoop < TempHalfWidth + TempSideOverlap; TempPixelLoop++)
				if (*TempCurrentPixel < 255)
				if ((TempNextPixelAfterThat < TempMaximumScratchPointer) && (TempNextPixelAfterThat >= TempScratchImage) &&
					(TempResultPixel < TempMaximumResultPointer) && (TempResultPixel >= TempImage))
				{
					//if (*TempResultPixel == 255)
					if ((*TempNextPixel < *TempCurrentPixel) && ((!TempRequire2InARow) || ((*TempNextPixelAfterThat < *TempCurrentPixel) &&
						(*TempNextPixel < *TempPreviousPixel) && (*TempNextPixelAfterThat < *TempPreviousPixel))))
					{
						if (*TempResultPixel == 255)  //if no previous value, set it
							*TempResultPixel = 255 - (*TempCurrentPixel - *TempNextPixel);
						else
						{  //if previous value, and this one smaller, set it, else keep previous value
							BYTE TempByte = 255 - (*TempCurrentPixel - *TempNextPixel);
							if (TempByte < *TempResultPixel)
							*TempResultPixel = TempByte;
						}

						//save greatest difference
						if (TempRequire2InARow)
						{
							TempNextResultPixel = TempResultPixel + TempSizeX;
							if (*TempCurrentPixel > *TempNextPixelAfterThat)
								*TempNextResultPixel = 255 - (*TempCurrentPixel - *TempNextPixelAfterThat);
							else
								*TempNextResultPixel = 255 - (*TempNextPixel - *TempNextPixelAfterThat);
						}
					}
					//else  //dont need to do as already set to 255
					//	*TempResultPixel = 255;

					TempPreviousPixel = TempCurrentPixel;
					TempCurrentPixel = TempNextPixel;
					TempNextPixel = TempNextPixelAfterThat;
					TempNextPixelAfterThat = TempNextPixelAfterThat + TempSizeX;
					TempResultPixel = TempResultPixel + TempSizeX;
				}
			}
		}
	}
}

 void CInspection::GenerateLinearBestFit(BYTE *TempData, WORD TempDataCount)
    {
				BYTE TempMinimumY = 255;
        double TempAverageOfX = TempDataCount / 2.0;
        double TempAverageOfY = 0;
				BYTE *TempDataPointer = TempData;
				for (WORD TempLoop = 0; TempLoop < TempDataCount; TempLoop++)
				{
					if (TempMinimumY > *TempDataPointer)
						TempMinimumY = *TempDataPointer;

					TempAverageOfY = TempAverageOfY + *TempDataPointer++;
				}
				 
        TempAverageOfY = TempAverageOfY / TempDataCount;

        double TempSumOfXSquared = 0;//TempPoints.Sum(point => point.X * point.X);
 				for (WORD TempLoop = 1; TempLoop <= TempDataCount; TempLoop++)
					TempSumOfXSquared = TempSumOfXSquared + (TempLoop * TempLoop);
 
       double TempSumOfXY = 0;//TempPoints.Sum(point => point.X * point.Y);
 				TempDataPointer = TempData;
				for (WORD TempLoop = 1; TempLoop <= TempDataCount; TempLoop++)
					TempSumOfXY = TempSumOfXY + (TempLoop * *TempDataPointer++);

        double m = ((TempSumOfXY / TempDataCount) - (TempAverageOfX * TempAverageOfY)) / ((TempSumOfXSquared / TempDataCount) - (TempAverageOfX * TempAverageOfX));
        double b = -((m * TempAverageOfX) - TempAverageOfY);

				b = b - 4; //give margin of 4 because noisy image

 				TempDataPointer = TempData;
				for (WORD TempLoop = 1; TempLoop <= TempDataCount; TempLoop++)
				{
					double TempValue = (m * TempLoop) + b;
					if (TempValue > 254.5)
						*TempDataPointer = 255;
					else
					if (TempValue < 0)
						*TempDataPointer = 0;
					else
						*TempDataPointer = (BYTE)(TempValue + .5);

					TempDataPointer++;
				}
    }

		//public static List<XYPoint> GenerateLinearBestFit(List<XYPoint> points, out double a, out double b)
  //  {
  //      int numPoints = points.Count;
  //      double meanX = points.Average(point => point.X);
  //      double meanY = points.Average(point => point.Y);

  //      double sumXSquared = points.Sum(point => point.X * point.X);
  //      double sumXY = points.Sum(point => point.X * point.Y);

  //      a = (sumXY / numPoints - meanX * meanY) / (sumXSquared / numPoints - meanX * meanX);
  //      b = (a * meanX - meanY);

  //      double a1 = a;
  //      double b1 = b;

  //      return points.Select(point => new XYPoint() { X = point.X, Y = a1 * point.X - b1 }).ToList();
  //  }


void CInspection::FillInLightSpots(BYTE *TempImage, WORD TempSizeX, WORD TempSizeY, BYTE TempLightSpotsType)
{
	if (vEnable)
	{
		WORD TempBottom = vROIPixelBottom;
		if (TempBottom >= TempSizeX)
			TempBottom = 0;
		WORD TempLeft = vROIPixelLeft;
		WORD TempRight = vROIPixelRight;

		WORD TempHeight = vROIPixelTop - TempBottom;
		if (TempHeight + TempBottom > TempSizeX)
			TempHeight = TempSizeX - TempBottom;

		WORD TempWidth = TempRight - vROIPixelLeft;
		if (TempWidth + TempLeft > TempSizeY)
			TempWidth = TempSizeY - TempLeft;

		WORD TempHalfWidth = TempWidth / 2;
		WORD TempCenter = TempLeft + TempHalfWidth;

		if (TempImage)
		{
			BYTE *TempCurrentPixel = NULL;
			BYTE *TempMaximumPointer = TempImage + (TempSizeY * TempSizeX);
			BYTE *TempCurveFitPoints = NULL;
			if (TempLightSpotsType == 2)
				TempCurveFitPoints = (BYTE *)malloc(TempHeight);

			//scanning from left to right, eliminate white spots on each line

								//next block for testing only

			//double *TempOriginalDoubleData = NULL;
			//double *TempCurveFitDoubleData = NULL;
			//if (PasswordOK(cSuperInspxPassword,false))
			//if (TempLineLoop == 50)
			//{
			//	TempOriginalDoubleData = (double*)malloc(TempHeight * sizeof(double));
			//	TempCurveFitDoubleData = (double*)malloc(TempHeight * sizeof(double));
			//}

			for (WORD TempLineLoop = 0; TempLineLoop <= TempWidth; TempLineLoop++)
			{
				//look to see if can fit a line.  If can, use it.  If can't then use Median Value
				if (TempLightSpotsType == 2)
				{
					TempCurrentPixel = TempImage + TempBottom + ((TempLineLoop + TempLeft) * TempSizeX);
					CopyMemory(TempCurveFitPoints, TempCurrentPixel, TempHeight);

					//next block for testing only
					//if (PasswordOK(cSuperInspxPassword,false))
					//if (TempLineLoop == 50)
					//{
					//	BYTE *TempSourcePointer = TempCurrentPixel;
					//	double *TempOriginalPointer = TempOriginalDoubleData;
					//	for (WORD TempPixelLoop = 0; TempPixelLoop < TempHeight; TempPixelLoop++)
					//	{
					//		*TempOriginalPointer = *TempSourcePointer;
					//		TempOriginalPointer++;
					//		TempSourcePointer++;
					//	}
					//}

					TempCurrentPixel = TempImage + TempBottom + ((TempLineLoop + TempLeft) * TempSizeX);

					BYTE *TempDataPointer = TempCurveFitPoints;

					//ReturnCurveFitLine(TempDataPointer, TempHeight, NULL, 3);
					GenerateLinearBestFit(TempDataPointer, TempHeight);

					//if (PasswordOK(cSuperInspxPassword,false))
					//if (TempLineLoop == 50)
					//{
					//	BYTE *TempSourcePointerC = TempCurveFitPoints;
					//	double *TempCurveFitDoublePointer = TempCurveFitDoubleData;
					//	for (WORD TempPixelLoop = 0; TempPixelLoop < TempHeight; TempPixelLoop++)
					//	{
					//		*TempCurveFitDoublePointer = *TempSourcePointerC;
					//		TempCurveFitDoublePointer++;
					//		TempSourcePointerC++;
					//	}
					//	CHistogramDisplayDialog IHistogramDisplayDialog;
					//	IHistogramDisplayDialog.vNumberOfNumbers = TempHeight;
					//	IHistogramDisplayDialog.vDataTitle = "Original Data";
					//	IHistogramDisplayDialog.vPlotRawData = true;
					//	IHistogramDisplayDialog.vValuesAreFloats = true;
					//	double *TempOriginalPointer = TempOriginalDoubleData;
					//	IHistogramDisplayDialog.vPointerToFloats = TempOriginalPointer;
					//	IHistogramDisplayDialog.vLocalSystemData = NULL;
					//	IHistogramDisplayDialog.vLocalConfigurationData = NULL;
					//	IHistogramDisplayDialog.vMainWindowPointer = NULL;
					//	if (PasswordOK(cSuperInspxPassword,false))
					//		int nResponse = IHistogramDisplayDialog.DoModal();

					//	IHistogramDisplayDialog.vDataTitle = "Curve Fit Data";
					//	IHistogramDisplayDialog.vPointerToFloats = TempCurveFitDoubleData;
					//	if (PasswordOK(cSuperInspxPassword,false))
					//		int nResponse = IHistogramDisplayDialog.DoModal();
					//}

					//replace the line data with the fitted curve to the line data and ends set to white

					BYTE *PointerToCurveFitLine = TempCurveFitPoints;

					for (WORD TempPixelLoop = 0; TempPixelLoop < TempHeight; TempPixelLoop++)
					{
						if (TempCurrentPixel < TempMaximumPointer)
						{
							if (*TempCurrentPixel > *PointerToCurveFitLine)
								*TempCurrentPixel = *PointerToCurveFitLine;

							TempCurrentPixel++;
							PointerToCurveFitLine++;
						}
						else
							ReportErrorMessage("Bad Pointer 1w", cEMailInspx,32000);
					}
				} //done advanced remove white spots
				else
				{
					//start at the bottom ROI line and study every line up to the top ROI line
					TempCurrentPixel = TempImage + TempBottom + ((TempLineLoop + TempLeft) * TempSizeX);
					BYTE TempMedianValueOfLine = FindMedianValue(TempCurrentPixel, TempHeight);
					TempCurrentPixel = TempImage + TempBottom + ((TempLineLoop + TempLeft) * TempSizeX);

					for (WORD TempPixelLoop = 0; TempPixelLoop < TempHeight; TempPixelLoop++)
					{
						if (TempCurrentPixel < TempMaximumPointer)
						{
							if (*TempCurrentPixel > TempMedianValueOfLine)
								*TempCurrentPixel = TempMedianValueOfLine;

							TempCurrentPixel++;
						}
						else
							ReportErrorMessage("Bad Pointer 1w", cEMailInspx,32000);
					} //done loop for each pixel in height
				} //end of standard remove white spots
			} //for each line in the image
			//if (PasswordOK(cSuperInspxPassword,false))
			//if (TempLineLoop == 50)
			//{
			//	if (TempOriginalDoubleData)
			//	 free(TempOriginalDoubleData);
			//	if (TempCurveFitDoubleData)
			//	 free(TempCurveFitDoubleData);
			//}
			if (TempCurveFitPoints)
			 free(TempCurveFitPoints);
		}
	}
/*
	if (vEnable)
	{
		WORD TempBottom = vROIPixelBottom;
		if (TempBottom >= TempSizeX)
			TempBottom = 0;
		WORD TempLeft = vROIPixelLeft;
		WORD TempRight = vROIPixelRight;
		if (TempLeft < 5)
			TempLeft = 5;
		if (TempRight + 5 > TempSizeY)
			TempRight = TempSizeY - 5;

		WORD TempHeight = vROIPixelTop - TempBottom;
		if (TempHeight + TempBottom > TempSizeX)
			TempHeight = TempSizeX - TempBottom;

		WORD TempWidth = TempRight - vROIPixelLeft;
		if (TempWidth + TempLeft > TempSizeY)
			TempWidth = TempSizeY - TempLeft;

		if (TempWidth + 10 > TempSizeY)
			TempWidth = TempSizeY - 10;

		WORD TempHalfWidth = TempWidth / 2;
		WORD TempCenter = TempLeft + TempHalfWidth;
		WORD TempSideOverlap = TempOverlap;

		if (TempImage)
		{
			BYTE *TempCurrentPixel = NULL;
			BYTE *TempNextPixel = NULL;
			BYTE *TempMaximumPointer = TempImage + (TempSizeY * TempSizeX);
			//start at the bottom ROI line and study every line up to the top ROI line
			for (WORD TempLineLoop = 0; TempLineLoop <= TempHeight; TempLineLoop++)
			{
				//scanning from center to left, eliminate white spots
				BYTE TempNumberOfPixelsToLookAheadForWhiteSpots = TempLightSpotWidth;
				for (WORD TempPixelLoop = 0; TempPixelLoop < TempHalfWidth + TempSideOverlap; TempPixelLoop++)
				{
					TempCurrentPixel = TempImage + TempLineLoop + TempBottom + ((TempCenter + TempSideOverlap - TempPixelLoop) * TempSizeX);
					if ((TempCurrentPixel < TempMaximumPointer) && (TempCurrentPixel >= TempImage))
					{
						BYTE TempEndCount = 0;
						bool TempFoundAPeak = false;
						TempNextPixel = TempCurrentPixel - TempSizeX;
						//check next 10 pixels for a white spot

						for (BYTE TempCheckLoop = 0; TempCheckLoop < TempNumberOfPixelsToLookAheadForWhiteSpots; TempCheckLoop++)  
						if (TempCheckLoop + TempPixelLoop < TempHalfWidth + TempSideOverlap)
						if ((TempNextPixel < TempMaximumPointer) && (TempNextPixel >= TempImage))
						{
							if ((TempFoundAPeak) && (*TempNextPixel < *TempCurrentPixel + 4)) //must be within 5 to find end of white spot
							{
								BYTE *TempPixelAfterNext = TempNextPixel - TempSizeX;
								//if the next two pixels are not significantly darker like a real contaminant
								if ((*TempNextPixel + 10 > *TempCurrentPixel) && (*TempPixelAfterNext + 10 > *TempCurrentPixel))
									TempEndCount = TempCheckLoop;
								TempCheckLoop = TempNumberOfPixelsToLookAheadForWhiteSpots;
							}
							else
							if (*TempNextPixel > *TempCurrentPixel + 4) //must be 10 higher to find a peak
								TempFoundAPeak = true;
							else
							if (*TempNextPixel <= *TempCurrentPixel) //next pixel smaller, so abort checking at this pixel
								TempCheckLoop = TempNumberOfPixelsToLookAheadForWhiteSpots;

							TempNextPixel = TempNextPixel - TempSizeX;
						}
						else
							ReportErrorMessage("Bad Pointer 2", cEMailInspx,32000);
						if ((TempFoundAPeak) && (TempEndCount))  //if you found a white spot, clip it to the start pixel value
						{
							TempNextPixel = TempCurrentPixel - TempSizeX;
							for (TempCheckLoop = 0; TempCheckLoop < TempEndCount; TempCheckLoop++)
							{
								*TempNextPixel = *TempCurrentPixel;
								TempNextPixel = TempNextPixel - TempSizeX;
							}
						}
					}
					else
						ReportErrorMessage("Bad Pointer 1", cEMailInspx,32000);
				}

				//scanning from center to right, eliminate white spots
				for (TempPixelLoop = 0; TempPixelLoop < TempHalfWidth + TempSideOverlap; TempPixelLoop++)
				{
					TempCurrentPixel = TempImage + TempLineLoop + TempBottom + ((TempCenter - TempSideOverlap + TempPixelLoop) * TempSizeX);
					if ((TempCurrentPixel < TempMaximumPointer) && (TempCurrentPixel >= TempImage))
					{
						BYTE TempEndCount = 0;
						bool TempFoundAPeak = false;
						TempNextPixel = TempCurrentPixel + TempSizeX;
						//check next 10 pixels for a white spot
						for (BYTE TempCheckLoop = 0; TempCheckLoop < TempNumberOfPixelsToLookAheadForWhiteSpots; TempCheckLoop++)  
						if (TempCheckLoop + TempPixelLoop < TempHalfWidth + TempSideOverlap)
						if ((TempNextPixel < TempMaximumPointer) && (TempNextPixel >= TempImage))
						{
							if ((TempFoundAPeak) && (*TempNextPixel < *TempCurrentPixel + 4)) //must be within 5 to find end of white spot
							{
								BYTE *TempPixelAfterNext = TempNextPixel + TempSizeX;
								//if the next two pixels are not significantly darker like a real contaminant
								if ((*TempNextPixel + 10 > *TempCurrentPixel) && (*TempPixelAfterNext + 10 > *TempCurrentPixel))
									TempEndCount = TempCheckLoop;
								TempCheckLoop = TempNumberOfPixelsToLookAheadForWhiteSpots;
							}
							else
							if (*TempNextPixel > *TempCurrentPixel + 4) //must be 10 higher to find a peak was 4
								TempFoundAPeak = true;
							else
							if (*TempNextPixel <= *TempCurrentPixel) //next pixel smaller, so abort checking at this pixel
								TempCheckLoop = TempNumberOfPixelsToLookAheadForWhiteSpots;

							TempNextPixel = TempNextPixel + TempSizeX;
						}
						else
							ReportErrorMessage("Bad Pointer 4", cEMailInspx,32000);
						if ((TempFoundAPeak) && (TempEndCount))  //if you found a white spot, clip it to the start pixel value
						{
							TempNextPixel = TempCurrentPixel + TempSizeX;
							for (TempCheckLoop = 0; TempCheckLoop < TempEndCount; TempCheckLoop++)
							{
								*TempNextPixel = *TempCurrentPixel;
								TempNextPixel = TempNextPixel + TempSizeX;
							}
						}
					}
					else
						ReportErrorMessage("Bad Pointer 3", cEMailInspx,32000);
				}
			}
		}
	}
*/
}
// the following values seem good for an x86:
#define LM_MACHEP .555e-16 /* resolution of arithmetic */
#define LM_DWARF  9.9e-324 /* smallest nonzero number */
// the follwoing values should work on any machine:
// #define LM_MACHEP 1.2e-16
// #define LM_DWARF 1.0e-38

// the squares of the following constants shall not under/overflow:
// these values seem good for an x86:
#define LM_SQRT_DWARF 1.e-160
#define LM_SQRT_GIANT 1.e150
// the following values should work on any machine:
// #define LM_SQRT_DWARF 3.834e-20
// #define LM_SQRT_GIANT 1.304e19
#define LMIN(a,b) (((a)<=(b)) ? (a) : (b))
#define LMAX(a,b) (((a)>=(b)) ? (a) : (b))
#define SQR(x)   (x)*(x) 


void CInspection::LeastSquaresCurveFit( int m, int n, double* x, double* fvec, double ftol, double xtol,
               double gtol, int maxfev, double epsfcn, double* diag, int mode,
               double factor, int *info, int *nfev, 
               double* fjac, int* ipvt, double* qtf,
               double* wa1, double* wa2, double* wa3, double* wa4,
               //lm_evaluate_ftype *evaluate, 
							 //lm_print_ftype *printout,
               void *data )
{
	double TempStartTime = CIFCOS::GetSystimeMicrosecs();
/*
 *   the purpose of lmdif is to minimize the sum of the squares of
 *   m nonlinear functions in n variables by a modification of
 *   the levenberg-marquardt algorithm. the user must provide a
 *   subroutine evaluate which calculates the functions. the jacobian
 *   is then calculated by a forward-difference approximation.
 *
 *   the multi-parameter interface lm_lmdif is for users who want
 *   full control and flexibility. most users will be better off using
 *   the simpler interface lmfit provided above.
 *
 *   the parameters are the same as in the legacy FORTRAN implementation,
 *   with the following exceptions:
 *      the old parameter ldfjac which gave leading dimension of fjac has
 *        been deleted because this C translation makes no use of two-
 *        dimensional arrays;
 *      the old parameter nprint has been deleted; printout is now controlled
 *        by the user-supplied routine *printout;
 *      the parameter field *data and the function parameters *evaluate and
 *        *printout have been added; they help avoiding global variables.
 *
 *   parameters:
 *
 *	m is a positive integer input variable set to the number
 *	  of functions.  Number of points in the array
 *
 *	n is a positive integer input variable set to the number
 *	  of variables. n must not exceed m.  Number of terms in the equation
 *
 *	x is an array of length n. on input x must contain
 *	  an initial estimate of the solution vector. on output x
 *	  contains the final estimate of the solution vector.
 *
 *	fvec is an output array of length m which contains
 *	  the functions evaluated at the output x.
 *
 *	ftol is a nonnegative input variable. termination
 *	  occurs when both the actual and predicted relative
 *	  reductions in the sum of squares are at most ftol.
 *	  therefore, ftol measures the relative error desired
 *	  in the sum of squares.
 *
 *	xtol is a nonnegative input variable. termination
 *	  occurs when the relative error between two consecutive
 *	  iterates is at most xtol. therefore, xtol measures the
 *	  relative error desired in the approximate solution.
 *
 *	gtol is a nonnegative input variable. termination
 *	  occurs when the cosine of the angle between fvec and
 *	  any column of the jacobian is at most gtol in absolute
 *	  value. therefore, gtol measures the orthogonality
 *	  desired between the function vector and the columns
 *	  of the jacobian.
 *
 *	maxfev is a positive integer input variable. termination
 *	  occurs when the number of calls to lm_fcn is at least
 *	  maxfev by the end of an iteration.
 *
 *	epsfcn is an input variable used in determining a suitable
 *	  step length for the forward-difference approximation. this
 *	  approximation assumes that the relative errors in the
 *	  functions are of the order of epsfcn. if epsfcn is less
 *	  than the machine precision, it is assumed that the relative
 *	  errors in the functions are of the order of the machine
 *	  precision.
 *
 *	diag is an array of length n. if mode = 1 (see below), diag is
 *        internally set. if mode = 2, diag must contain positive entries
 *        that serve as multiplicative scale factors for the variables.
 *
 *	mode is an integer input variable. if mode = 1, the
 *	  variables will be scaled internally. if mode = 2,
 *	  the scaling is specified by the input diag. other
 *	  values of mode are equivalent to mode = 1.
 *
 *	factor is a positive input variable used in determining the
 *	  initial step bound. this bound is set to the product of
 *	  factor and the euclidean norm of diag*x if nonzero, or else
 *	  to factor itself. in most cases factor should lie in the
 *	  interval (.1,100.). 100. is a generally recommended value.
 *
 *	info is an integer output variable that indicates the termination
 *        status of lm_lmdif as follows:
 *
 *        info < 0  termination requested by user-supplied routine *evaluate;
 *
 *	  info = 0  improper input parameters;
 *
 *	  info = 1  both actual and predicted relative reductions
 *		    in the sum of squares are at most ftol;
 *
 *	  info = 2  relative error between two consecutive iterates
 *		    is at most xtol;
 *
 *	  info = 3  conditions for info = 1 and info = 2 both hold;
 *
 *	  info = 4  the cosine of the angle between fvec and any
 *		    column of the jacobian is at most gtol in
 *		    absolute value;
 *
 *	  info = 5  number of calls to lm_fcn has reached or
 *		    exceeded maxfev;
 *
 *	  info = 6  ftol is too small. no further reduction in
 *		    the sum of squares is possible;
 *
 *	  info = 7  xtol is too small. no further improvement in
 *		    the approximate solution x is possible;
 *
 *	  info = 8  gtol is too small. fvec is orthogonal to the
 *		    columns of the jacobian to machine precision;
 *
 *	nfev is an output variable set to the number of calls to the
 *        user-supplied routine *evaluate.
 *
 *	fjac is an output m by n array. the upper n by n submatrix
 *	  of fjac contains an upper triangular matrix r with
 *	  diagonal elements of nonincreasing magnitude such that
 *
 *		 t     t	   t
 *		p *(jac *jac)*p = r *r,
 *
 *	  where p is a permutation matrix and jac is the final
 *	  calculated jacobian. column j of p is column ipvt(j)
 *	  (see below) of the identity matrix. the lower trapezoidal
 *	  part of fjac contains information generated during
 *	  the computation of r.
 *
 *	ipvt is an integer output array of length n. ipvt
 *	  defines a permutation matrix p such that jac*p = q*r,
 *	  where jac is the final calculated jacobian, q is
 *	  orthogonal (not stored), and r is upper triangular
 *	  with diagonal elements of nonincreasing magnitude.
 *	  column j of p is column ipvt(j) of the identity matrix.
 *
 *	qtf is an output array of length n which contains
 *	  the first n elements of the vector (q transpose)*fvec.
 *
 *	wa1, wa2, and wa3 are work arrays of length n.
 *
 *	wa4 is a work array of length m.
 *
 *   the following parameters are newly introduced in this C translation:
 *
 *      evaluate is the name of the subroutine which calculates the functions.
 *        a default implementation lm_evaluate_default is provided in lm_eval.c;
 *        alternatively, evaluate can be provided by a user calling program.
 *        it should be written as follows:
 *
 *        void evaluate ( double* par, int m_dat, double* fvec, 
 *                       void *data, int *info )
 *        {
 *           // for ( i=0; i<m_dat; ++i )
 *           //     calculate fvec[i] for given parameters par;
 *           // to stop the minimization, 
 *           //     set *info to a negative integer.
 *        }
 *
 *      printout is the name of the subroutine which nforms about fit progress.
 *        a default implementation lm_print_default is provided in lm_eval.c;
 *        alternatively, printout can be provided by a user calling program.
 *        it should be written as follows:
 *
 *        void printout ( int n_par, double* par, int m_dat, double* fvec, 
 *                       void *data, int iflag, int iter, int nfev )
 *        {
 *           // iflag : 0 (init) 1 (outer loop) 2(inner loop) -1(terminated)
 *           // iter  : outer loop counter
 *           // nfev  : number of calls to *evaluate
 *        }
 *
 *      data is an input pointer to an arbitrary structure that is passed to
 *        evaluate. typically, it contains experimental data to be fitted.
 *
 */
  int i, iter, j;
  double actred, delta, dirder, eps, fnorm, fnorm1, gnorm, par, pnorm,
      prered, ratio, step, sum, temp, temp1, temp2, temp3, xnorm;
  static double p1 = 0.1;
  static double p5 = 0.5;
  static double p25 = 0.25;
  static double p75 = 0.75;
  static double p0001 = 1.0e-4;

  *nfev = 0; // function evaluation counter
  iter = 1;  // outer loop counter
  par = 0;   // levenberg-marquardt parameter 
  delta = 0; // just to prevent a warning (initialization within if-clause)
  xnorm = 0; // dito

  temp = LMAX(epsfcn,LM_MACHEP);
  eps = sqrt(temp); // used in calculating the Jacobian by forward differences

	//check the input parameters for errors.
  if ( (n <= 0) || (m < n) || (ftol < 0.) || (xtol < 0.) || (gtol < 0.) || (maxfev <= 0) || (factor <= 0.) )
  {
    *info = 0; // invalid parameter
		double TempEndTime = CIFCOS::GetSystimeMicrosecs();
		double TempTime = ((TempEndTime - TempStartTime) / 1000.0);
		ReportErrorMessage("Glass2C Invalid Parameter", cEMailInspx,32000);
    return;
  }
  if ( mode == 2 )  /* scaling by diag[] */
  {
		for ( j=0; j<n; j++ )  /* check for nonpositive elements */
    {
      if ( diag[j] <= 0.0 )
      {
         *info = 0; // invalid parameter
				double TempEndTime = CIFCOS::GetSystimeMicrosecs();
				double TempTime = ((TempEndTime - TempStartTime) / 1000.0);
				ReportErrorMessage("Glass2C Invalid Parameter", cEMailInspx,32000);
        return;
      }
    }	
  }

	//evaluate the function at the starting point and calculate its norm.
  *info = 0;
  EvaluateLeastSquaresFitFunction( x, m, fvec, data, info );
	*nfev = *nfev + 1;
  if ( *info < 0 ) 
	{
		/*
		double TempEndTime = CIFCOS::GetSystimeMicrosecs();
		double TempTime = ((TempEndTime - TempStartTime) / 1000.0);
		ReportErrorMessage("Glass2C Aborted Time:" + dtoa(TempTime,3) + "mSec, EndC:" + dtoa(*info,3) + " Calls:" + dtoa(*nfev,0), cAction,0);
		*/
    return;
	}
  fnorm = lm_enorm(m,fvec);

  do //the outer loop.
	{ 
		//calculate the jacobian matrix.
		for ( j=0; j<n; j++ )
		{
      temp = x[j];
      step = eps * fabs(temp);
      if (step == 0.) step = eps;
      x[j] = temp + step;
      *info = 0;
			//go through each point in array and put difference from what want in wa4
      EvaluateLeastSquaresFitFunction( x, m, wa4, data, info ); 
			*nfev = *nfev + 1;
      //(*printout)( n, x, m, wa4, data, 1, iter, ++(*nfev) );
      if ( *info < 0 ) 
			// user requested break
			{
				/*
				double TempEndTime = CIFCOS::GetSystimeMicrosecs();
				double TempTime = ((TempEndTime - TempStartTime) / 1000.0);
				ReportErrorMessage("Glass2C User Break:" + dtoa(TempTime,3) + "mSec, EndC:" + dtoa(*info,3) + " Calls:" + dtoa(*nfev,0), cAction,0);
				*/
        return;
			}
      x[j] = temp;
      for ( i=0; i<m; i++ )
			if (step)
        fjac[j*m+i] = (wa4[i] - fvec[i]) / step;
    }

		//compute the qr factorization of the jacobian.
		lm_qrfac( m, n, fjac, 1, ipvt, wa1, wa2, wa3);

		//on the first iteration ... 
    if (iter == 1)
    {
      if (mode != 2)
			//scale according to the norms of the columns of the initial jacobian.
      {
        for ( j=0; j<n; j++ )
        {
          diag[j] = wa2[j];
          if ( wa2[j] == 0. )
            diag[j] = 1.;
        }
      }

			//calculate the norm of the scaled x and 
			//initialize the step bound delta.
      for ( j=0; j<n; j++ )
        wa3[j] = diag[j] * x[j];

      xnorm = lm_enorm( n, wa3 );
      delta = factor*xnorm;
      if (delta == 0.)
        delta = factor;
    }

		//form (q transpose)*fvec and store the first n components in qtf.
    for ( i=0; i<m; i++ )
      wa4[i] = fvec[i];

    for ( j=0; j<n; j++ )
    {
      temp3 = fjac[j*m+j];
      if (temp3 != 0.)
      {
        sum = 0;
        for ( i=j; i<m; i++ )
          sum += fjac[j*m+i] * wa4[i];
				if (temp3)
        temp = -sum / temp3;
        for ( i=j; i<m; i++ )
          wa4[i] += fjac[j*m+i] * temp;
      }
      fjac[j*m+j] = wa1[j];
      qtf[j] = wa4[j];
    }

		//compute the norm of the scaled gradient and test for convergence.
    gnorm = 0;
    if ( fnorm != 0 )
    {
      for ( j=0; j<n; j++ )
      {
        if ( wa2[ ipvt[j] ] == 0 ) continue;
        
        sum = 0.;
        for ( i=0; i<=j; i++ )
					if (fnorm)
          sum += fjac[j*m+i] * qtf[i] / fnorm;
        gnorm = LMAX( gnorm, fabs(sum/wa2[ ipvt[j] ]) );
      }
    }

    if ( gnorm <= gtol )
    {
      *info = 4;
			{
				/*
				double TempEndTime = CIFCOS::GetSystimeMicrosecs();
				double TempTime = ((TempEndTime - TempStartTime) / 1000.0);
				ReportErrorMessage("Glass2C Time:" + dtoa(TempTime,3) + "mSec, EndC:" + dtoa(*info,3) + " Calls:" + dtoa(*nfev,0), cAction,0);
				*/
        return;
			}
    }

		//rescale if necessary.
    if ( mode != 2 )
    {
      for ( j=0; j<n; j++ )
        diag[j] = LMAX(diag[j],wa2[j]);
    }

    do //the inner loop.
		{
			//determine the levenberg-marquardt parameter.
      lm_lmpar( n,fjac,m,ipvt,diag,qtf,delta,&par,wa1,wa2,wa3,wa4 );

			//store the direction p and x + p. calculate the norm of p.
      for ( j=0; j<n; j++ )
      {
          wa1[j] = -wa1[j];
          wa2[j] = x[j] + wa1[j];
          wa3[j] = diag[j]*wa1[j];
      }
      pnorm = lm_enorm(n,wa3);

			//on the first iteration, adjust the initial step bound.
      if ( *nfev<= 1+n ) // bug corrected by J. Wuttke in 2004
        delta = LMIN(delta,pnorm);

			//evaluate the function at x + p and calculate its norm.
      *info = 0;
      EvaluateLeastSquaresFitFunction( wa2, m, wa4, data, info );
			*nfev = *nfev + 1;
      //(*printout)( n, x, m, wa4, data, 2, iter, ++(*nfev) );
      if ( *info < 0 ) 
			{
				/*
				double TempEndTime = CIFCOS::GetSystimeMicrosecs();
				double TempTime = ((TempEndTime - TempStartTime) / 1000.0);
				ReportErrorMessage("Glass2C Time:" + dtoa(TempTime,3) + "mSec, EndC:" + dtoa(*info,3) + " Calls:" + dtoa(*nfev,0), cAction,0);
				*/
        return;
			}

      fnorm1 = lm_enorm(m,wa4);

			// OI* compute the scaled actual reduction.
      if ( p1*fnorm1 < fnorm )
          actred = 1 - SQR( fnorm1/fnorm );
      else
          actred = -1;

			//compute the scaled predicted reduction andthe scaled directional derivative.
      for ( j=0; j<n; j++ )
      {
        wa3[j] = 0;
        for ( i=0; i<=j; i++ )
          wa3[i] += fjac[j*m+i]*wa1[ ipvt[j] ];
      }
			if (fnorm)
			{
				temp1 = lm_enorm(n,wa3) / fnorm;
				temp2 = sqrt(par) * pnorm / fnorm;
			}
      prered = SQR(temp1) + 2 * SQR(temp2);
      dirder = - ( SQR(temp1) + SQR(temp2) );

			//compute the ratio of the actual to the predicted reduction.
      ratio = prered!=0 ? actred/prered : 0;

			//update the step bound.
      if (ratio <= p25)
      {
        if (actred >= 0.)
            temp = p5;
        else
            temp = p5*dirder/(dirder + p5*actred);
        if ( p1*fnorm1 >= fnorm || temp < p1 )
            temp = p1;
        delta = temp * LMIN(delta,pnorm/p1);
				if (temp)
	        par /= temp;
      }
      else if ( par == 0. || ratio >= p75 )
      {
        delta = pnorm/p5;
        par *= p5;
      }

			//test for successful iteration...
      if (ratio >= p0001)
      {
				//successful iteration. update x, fvec, and their norms.
        for ( j=0; j<n; j++ )
        {
            x[j] = wa2[j];
            wa2[j] = diag[j]*x[j];
        }
        for ( i=0; i<m; i++ )
            fvec[i] = wa4[i];
        xnorm = lm_enorm(n,wa2);
        fnorm = fnorm1;
        iter++;
      }

			//tests for convergence ( otherwise *info = 1, 2, or 3 )
      *info = 0; // do not terminate (unless overwritten by nonzero value)
      if ( fabs(actred) <= ftol && prered <= ftol && p5*ratio <= 1 )
          *info = 1;
      if (delta <= xtol*xnorm)
          *info += 2;
      if ( *info != 0)
			{
				//*info = 1 or 3;
				/*
				double TempEndTime = CIFCOS::GetSystimeMicrosecs();
				double TempTime = ((TempEndTime - TempStartTime) / 1000.0);
				ReportErrorMessage("Glass2C Time:" + dtoa(TempTime,3) + "mSec, EndC:" + dtoa(*info,0) + " Calls:" + dtoa(*nfev,0), cAction,0);
				*/
        return;
			}

			// OI* tests for termination and stringent tolerances.
      if ( *nfev >= maxfev)
          *info = 5;
      if ( fabs(actred) <= LM_MACHEP &&
           prered <= LM_MACHEP && p5*ratio <= 1 )
          *info = 6;
      if (delta <= LM_MACHEP*xnorm)
          *info = 7;
      if (gnorm <= LM_MACHEP)
          *info = 8;
      if ( *info != 0)
			{
				//*info = 5, 6, 7, or 8 //5 = went through loop enough times
				/*
				double TempEndTime = CIFCOS::GetSystimeMicrosecs();
				double TempTime = ((TempEndTime - TempStartTime) / 1000.0);
				ReportErrorMessage("Glass2C Time:" + dtoa(TempTime,3) + "mSec, EndC:" + dtoa(*info,3) + " Calls:" + dtoa(*nfev,0), cAction,0);
				*/
        return;
			}
    } while (ratio < p0001);//end of the inner loop. repeat if iteration unsuccessful.
  } while (1);//end of the outer loop.
}

void CInspection::EvaluateLeastSquaresFitFunction( double* par, int m_dat, double* fvec, void *data, int *info )
{
  int i;
  lm_data_type *mydata;
  mydata = (lm_data_type*)data;

  for (i=0; i<m_dat; i++)
    fvec[i] = mydata->user_y[i] - mydata->user_func( mydata->user_t[i], par);

  *info = *info; /* to prevent a 'unused variable' warning */
    /* if <parameters drifted away> { *info = -1; } */
/* 
 *	par is an input array. At the end of the minimization, it contains
 *        the approximate solution vector.  Par is the equation terms
 *
 *	m_dat is a positive integer input variable set to the number
 *	  of functions.  Is the number of data points in array to fit
 *
 *	fvec is an output array of length m_dat which contains the function
 *        values the square sum of which ought to be minimized.
 *
 *	data is a read-only pointer to lm_data_type, as specified by lmuse.h.
 *
 *      info is an integer output variable. If set to a negative value, the
 *        minimization procedure will stop.
 */
}

double CInspection::lm_enorm( int n, double *x )
{
/*     given an n-vector x, this function calculates the
 *     euclidean norm of x.
 *
 *     the euclidean norm is computed by accumulating the sum of
 *     squares in three different sums. the sums of squares for the
 *     small and large components are scaled so that no overflows
 *     occur. non-destructive underflows are permitted. underflows
 *     and overflows do not occur in the computation of the unscaled
 *     sum of squares for the intermediate components.
 *     the definitions of small, intermediate and large components
 *     depend on two constants, LM_SQRT_DWARF and LM_SQRT_GIANT. the main
 *     restrictions on these constants are that LM_SQRT_DWARF**2 not
 *     underflow and LM_SQRT_GIANT**2 not overflow.
 *
 *     parameters
 *
 *	n is a positive integer input variable.
 *
 *	x is an input array of length n.
 */
  int i;
  double agiant, s1, s2, s3, xabs, x1max, x3max, temp;

  s1 = 0;
  s2 = 0;
  s3 = 0;
  x1max = 0;
  x3max = 0;
  agiant = LM_SQRT_GIANT/( (double) n);

  for ( i=0; i<n; i++ )
  {
    xabs = fabs(x[i]);
    if ((xabs > LM_SQRT_DWARF) && (xabs < agiant))
		{
			//sum for intermediate components.
      s2 += xabs*xabs;
      continue;
		}

		if ( xabs >  LM_SQRT_DWARF )
		{
			//sum for large components.
			if (xabs > x1max)
			{
				temp = x1max/xabs;
				s1 = 1 + s1*SQR(temp);
				x1max = xabs;
			}
			else
			{
				temp = xabs/x1max;
				s1 += SQR(temp);
			}
			continue;
		}
		//sum for small components.
		if (xabs > x3max)
		{
      temp = x3max/xabs;
      s3 = 1 + s3*SQR(temp);
      x3max = xabs;
		}
		else	
		{
      if (xabs != 0.)
      {
				temp = xabs/x3max;
				s3 += SQR(temp);
			}
		}
	}

	//calculation of norm.
	if (s1 != 0)
		return x1max*sqrt(s1 + (s2/x1max)/x1max);
	if (s2 != 0)
	{
		if (s2 >= x3max)
			return sqrt( s2*(1+(x3max/s2)*(x3max*s3)) );
		else
			return sqrt( x3max*((s2/x3max)+(x3max*s3)) );
	}
	return x3max*sqrt(s3);
}

void CInspection::lm_qrfac(int m, int n, double* a, int pivot, int* ipvt,
           double* rdiag, double* acnorm, double* wa)
{
/*
 *     this subroutine uses householder transformations with column
 *     pivoting (optional) to compute a qr factorization of the
 *     m by n matrix a. that is, qrfac determines an orthogonal
 *     matrix q, a permutation matrix p, and an upper trapezoidal
 *     matrix r with diagonal elements of nonincreasing magnitude,
 *     such that a*p = q*r. the householder transformation for
 *     column k, k = 1,2,...,min(m,n), is of the form
 *
 *			    t
 *	    i - (1/u(k))*u*u
 *
 *     where u has 0.s in the first k-1 positions. the form of
 *     this transformation and the method of pivoting first
 *     appeared in the corresponding linpack subroutine.
 *
 *     parameters:
 *
 *	m is a positive integer input variable set to the number
 *	  of rows of a.
 *
 *	n is a positive integer input variable set to the number
 *	  of columns of a.
 *
 *	a is an m by n array. on input a contains the matrix for
 *	  which the qr factorization is to be computed. on output
 *	  the strict upper trapezoidal part of a contains the strict
 *	  upper trapezoidal part of r, and the lower trapezoidal
 *	  part of a contains a factored form of q (the non-trivial
 *	  elements of the u vectors described above).
 *
 *	pivot is a logical input variable. if pivot is set true,
 *	  then column pivoting is enforced. if pivot is set false,
 *	  then no column pivoting is done.
 *
 *	ipvt is an integer output array of length lipvt. ipvt
 *	  defines the permutation matrix p such that a*p = q*r.
 *	  column j of p is column ipvt(j) of the identity matrix.
 *	  if pivot is false, ipvt is not referenced.
 *
 *	rdiag is an output array of length n which contains the
 *	  diagonal elements of r.
 *
 *	acnorm is an output array of length n which contains the
 *	  norms of the corresponding columns of the input matrix a.
 *	  if this information is not needed, then acnorm can coincide
 *	  with rdiag.
 *
 *	wa is a work array of length n. if pivot is false, then wa
 *	  can coincide with rdiag.
 *
 */
  int i, j, k, kmax, minmn;
  double ajnorm, sum, temp;
  static double p05 = 0.05;

	//compute the initial column norms and initialize several arrays.
  for (j=0; j<n; j++)
  {
		acnorm[j] = lm_enorm(m, &a[j*m]);
		rdiag[j] = acnorm[j];
		wa[j] = rdiag[j];
		if (pivot)
			ipvt[j] = j;
  }

	//reduce a to r with householder transformations.
  minmn = LMIN(m,n);
  for (j=0; j<minmn; j++)
  {
    if (!pivot) goto pivot_ok;

		//bring the column of largest norm into the pivot position.
    kmax = j;
    for (k=j+1; k<n; k++)
        if (rdiag[k] > rdiag[kmax])
		kmax = k;
    if (kmax == j) goto pivot_ok; // bug fixed in rel 2.1

		for (i=0; i<m; i++)
		{
      temp = a[j*m+i];
      a[j*m+i] = a[kmax*m+i];
      a[kmax*m+i] = temp;
		}
    rdiag[kmax] = rdiag[j];
    wa[kmax] = wa[j];
    k = ipvt[j];
    ipvt[j] = ipvt[kmax];
    ipvt[kmax] = k;

    pivot_ok:

		//compute the Householder transformation to reduce the j-th column of a to a multiple of the j-th unit vector.
    ajnorm = lm_enorm(m-j, &a[j*m+j]);
    if (ajnorm == 0.)
    {
      rdiag[j] = 0;
      continue;
    }

    if (a[j*m+j] < 0.)
      ajnorm = -ajnorm;
    for (i=j; i<m; i++)
 		if (ajnorm)
	    a[j*m+i] /= ajnorm;
    a[j*m+j] += 1;

		//apply the transformation to the remaining columns and update the norms.
    for (k=j+1; k<n; k++)
    {
      sum = 0;
      for ( i=j; i<m; i++ )
				sum += a[j*m+i]*a[k*m+i];

      temp = sum/a[j+m*j];

      for (i=j; i<m; i++)
				a[k*m+i] -= temp * a[j*m+i];

      if (pivot && rdiag[k] != 0.)
      {
        temp = a[m*k+j]/rdiag[k];
        temp = LMAX(0., 1-temp*temp);
        rdiag[k] *= sqrt(temp);
        temp = rdiag[k]/wa[k];
        if (p05*SQR(temp) <= LM_MACHEP)
        {
          rdiag[k] = lm_enorm(m-j-1, &a[m*k+j+1]);
          wa[k] = rdiag[k];
        }
      }
    }
		rdiag[j] = -ajnorm;
  }
}

void CInspection::lm_lmpar(int n, double* r, int ldr, int* ipvt, double* diag, double* qtb,
              double delta, double* par, double* x, double* sdiag,
              double* wa1, double* wa2)
{
/*     given an m by n matrix a, an n by n nonsingular diagonal
 *     matrix d, an m-vector b, and a positive number delta,
 *     the problem is to determine a value for the parameter
 *     par such that if x solves the system
 *
 *	    a*x = b ,	  sqrt(par)*d*x = 0 ,
 *
 *     in the least squares sense, and dxnorm is the euclidean
 *     norm of d*x, then either par is 0. and
 *
 *	    (dxnorm-delta) .le. 0.1*delta ,
 *
 *     or par is positive and
 *
 *	    abs(dxnorm-delta) .le. 0.1*delta .
 *
 *     this subroutine completes the solution of the problem
 *     if it is provided with the necessary information from the
 *     qr factorization, with column pivoting, of a. that is, if
 *     a*p = q*r, where p is a permutation matrix, q has orthogonal
 *     columns, and r is an upper triangular matrix with diagonal
 *     elements of nonincreasing magnitude, then lmpar expects
 *     the full upper triangle of r, the permutation matrix p,
 *     and the first n components of (q transpose)*b. on output
 *     lmpar also provides an upper triangular matrix s such that
 *
 *	     t	 t		     t
 *	    p *(a *a + par*d*d)*p = s *s .
 *
 *     s is employed within lmpar and may be of separate interest.
 *
 *     only a few iterations are generally needed for convergence
 *     of the algorithm. if, however, the limit of 10 iterations
 *     is reached, then the output par will contain the best
 *     value obtained so far.
 *
 *     parameters:
 *
 *	n is a positive integer input variable set to the order of r.
 *
 *	r is an n by n array. on input the full upper triangle
 *	  must contain the full upper triangle of the matrix r.
 *	  on output the full upper triangle is unaltered, and the
 *	  strict lower triangle contains the strict upper triangle
 *	  (transposed) of the upper triangular matrix s.
 *
 *	ldr is a positive integer input variable not less than n
 *	  which specifies the leading dimension of the array r.
 *
 *	ipvt is an integer input array of length n which defines the
 *	  permutation matrix p such that a*p = q*r. column j of p
 *	  is column ipvt(j) of the identity matrix.
 *
 *	diag is an input array of length n which must contain the
 *	  diagonal elements of the matrix d.
 *
 *	qtb is an input array of length n which must contain the first
 *	  n elements of the vector (q transpose)*b.
 *
 *	delta is a positive input variable which specifies an upper
 *	  bound on the euclidean norm of d*x.
 *
 *	par is a nonnegative variable. on input par contains an
 *	  initial estimate of the levenberg-marquardt parameter.
 *	  on output par contains the final estimate.
 *
 *	x is an output array of length n which contains the least
 *	  squares solution of the system a*x = b, sqrt(par)*d*x = 0,
 *	  for the output par.
 *
 *	sdiag is an output array of length n which contains the
 *	  diagonal elements of the upper triangular matrix s.
 *
 *	wa1 and wa2 are work arrays of length n.
 *
 */
  int i, iter, j, nsing;
  double dxnorm, fp, fp_old, gnorm, parc, parl, paru;
  double sum, temp;
  static double p1 = 0.1;
  static double p001 = 0.001;

	//compute and store in x the gauss-newton direction. if the jacobian is rank-deficient, obtain a least squares solution.
  nsing = n;
  for (j=0; j<n; j++)
  {
		wa1[j] = qtb[j];
		if (r[j*ldr+j] == 0 && nsing == n)
			nsing = j;
		if (nsing < n)
			wa1[j] = 0;
  }
  for (j=nsing-1; j>=0; j--)
  {
      wa1[j] = wa1[j]/r[j+ldr*j];
      temp = wa1[j];
      for (i=0; i<j; i++)
          wa1[i] -= r[j*ldr+i]*temp;
  }

  for (j=0; j<n; j++)
		x[ipvt[j]] = wa1[j];

	//initialize the iteration counter. evaluate the function at the origin, and test for acceptance of the gauss-newton direction.
  iter = 0;
  for (j=0; j<n; j++)
		wa2[j] = diag[j]*x[j];
  dxnorm = lm_enorm(n,wa2);
  fp = dxnorm - delta;
  if (fp <= p1*delta)
  {
    *par = 0;
    return;
  }

	//if the jacobian is not rank deficient, the newton step provides a lower bound, parl, for the 0. of the function. 
	//otherwise set this bound to 0..

  parl = 0;
  if (nsing >= n)
  {
		for (j=0; j<n; j++)
		if (dxnorm)
			wa1[j] = diag[ipvt[j]] * wa2[ipvt[j]] / dxnorm;

		for (j=0; j<n; j++)
    {
      sum = 0.;
      for (i=0; i<j; i++)
          sum += r[j*ldr+i]*wa1[i];
      wa1[j] = (wa1[j] - sum)/r[j+ldr*j];
    }
		temp = lm_enorm(n,wa1);
		parl = fp/delta/temp/temp;
  }

	//calculate an upper bound, paru, for the 0. of the function.
  for (j=0; j<n; j++)
  {
		sum = 0;
		for (i=0; i<=j; i++)
      sum += r[j*ldr+i]*qtb[i];
		wa1[j] = sum/diag[ipvt[j]];
  }
  gnorm = lm_enorm(n,wa1);
  paru = gnorm/delta;
  if (paru == 0.)
		paru = LM_DWARF/LMIN(delta,p1);

	//if the input par lies outside of the interval (parl,paru), set par to the closer endpoint.
  *par = LMAX(*par,parl);
  *par = LMIN(*par,paru);
  if (*par == 0.)
	*par = gnorm/dxnorm;
	//iterate.
  for ( ; ; iter++ )
	{
		//evaluate the function at the current value of par.
		if (*par == 0.)
				*par = LMAX(LM_DWARF,p001*paru);
		temp = sqrt( *par );
		for (j=0; j<n; j++)
				wa1[j] = temp*diag[j];
		lm_qrsolv(n, r, ldr, ipvt, wa1, qtb, x, sdiag, wa2);
		for (j=0; j<n; j++)
				wa2[j] = diag[j]*x[j];
		dxnorm = lm_enorm(n,wa2);
		fp_old = fp;
		fp = dxnorm - delta;

		//if the function is small enough, accept the current value of par. also test for the exceptional cases where parl
		//is 0. or the number of iterations has reached 10.
    if ((fabs(fp) <= p1*delta) || ((parl == 0.) && (fp <= fp_old) && (fp_old < 0.)) || (iter == 10))
			break; // the only exit from this loop

		//compute the Newton correction.
    for (j=0; j<n; j++)
		if (dxnorm)
			wa1[j] = diag[ipvt[j]] * wa2[ipvt[j]] / dxnorm;

    for (j=0; j<n; j++)
    {
      wa1[j] = wa1[j]/sdiag[j];
      for (i=j+1; i<n; i++)
        wa1[i] -= r[j*ldr+i]*wa1[j];
    }
    temp = lm_enorm(n, wa1);
    parc = fp/delta/temp/temp;

		//depending on the sign of the function, update parl or paru.
    if (fp > 0)
        parl = LMAX(parl, *par);
    else if (fp < 0)
        paru = LMIN(paru, *par);
    // the case fp==0 is precluded by the break condition 

		//compute an improved estimate for par.
    *par = LMAX(parl, *par + parc);
	}
}


void CInspection::lm_qrsolv(int n, double* r, int ldr, int* ipvt, double* diag,
              double* qtb, double* x, double* sdiag, double* wa)
{
/*
 *     given an m by n matrix a, an n by n diagonal matrix d,
 *     and an m-vector b, the problem is to determine an x which
 *     solves the system
 *
 *	    a*x = b ,	  d*x = 0 ,
 *
 *     in the least squares sense.
 *
 *     this subroutine completes the solution of the problem
 *     if it is provided with the necessary information from the
 *     qr factorization, with column pivoting, of a. that is, if
 *     a*p = q*r, where p is a permutation matrix, q has orthogonal
 *     columns, and r is an upper triangular matrix with diagonal
 *     elements of nonincreasing magnitude, then qrsolv expects
 *     the full upper triangle of r, the permutation matrix p,
 *     and the first n components of (q transpose)*b. the system
 *     a*x = b, d*x = 0, is then equivalent to
 *
 *		   t	   t
 *	    r*z = q *b ,  p *d*p*z = 0 ,
 *
 *     where x = p*z. if this system does not have full rank,
 *     then a least squares solution is obtained. on output qrsolv
 *     also provides an upper triangular matrix s such that
 *
 *	     t	 t		 t
 *	    p *(a *a + d*d)*p = s *s .
 *
 *     s is computed within qrsolv and may be of separate interest.
 *
 *     parameters
 *
 *	n is a positive integer input variable set to the order of r.
 *
 *	r is an n by n array. on input the full upper triangle
 *	  must contain the full upper triangle of the matrix r.
 *	  on output the full upper triangle is unaltered, and the
 *	  strict lower triangle contains the strict upper triangle
 *	  (transposed) of the upper triangular matrix s.
 *
 *	ldr is a positive integer input variable not less than n
 *	  which specifies the leading dimension of the array r.
 *
 *	ipvt is an integer input array of length n which defines the
 *	  permutation matrix p such that a*p = q*r. column j of p
 *	  is column ipvt(j) of the identity matrix.
 *
 *	diag is an input array of length n which must contain the
 *	  diagonal elements of the matrix d.
 *
 *	qtb is an input array of length n which must contain the first
 *	  n elements of the vector (q transpose)*b.
 *
 *	x is an output array of length n which contains the least
 *	  squares solution of the system a*x = b, d*x = 0.
 *
 *	sdiag is an output array of length n which contains the
 *	  diagonal elements of the upper triangular matrix s.
 *
 *	wa is a work array of length n.
 *
 */
  int i, kk, j, k, nsing;
  double qtbpj, sum, temp;
  double sin, cos, tan, cotan; // these are local variables, not functions
  static double p25 = 0.25;
  static double p5 = 0.5;

	//copy r and (q transpose)*b to preserve input and initialize s. in particular, save the diagonal elements of r in x.
  for (j=0; j<n; j++)
  {
		for (i=j; i<n; i++)
			r[j*ldr+i] = r[i*ldr+j];
		x[j] = r[j*ldr+j];
		wa[j] = qtb[j];
	}

	//eliminate the diagonal matrix d using a givens rotation.
  for (j=0; j<n; j++)
  {
		//prepare the row of d to be eliminated, locating the diagonal element using p from the qr factorization.
    if (diag[ipvt[j]] == 0.)
			goto L90;
    for (k=j; k<n; k++)
			sdiag[k] = 0.;
    sdiag[j] = diag[ipvt[j]];

		//the transformations to eliminate the row of d modify only a single 
		//element of (q transpose)*b beyond the first n, which is initially 0..
    qtbpj = 0.;
    for (k=j; k<n; k++)
		{
			//determine a givens rotation which eliminates the appropriate element in the current row of d.
      if (sdiag[k] == 0.)
				continue;
      kk = k + ldr * k; // <! keep this shorthand !>
      if (fabs(r[kk]) < fabs(sdiag[k]))
      {
				cotan = r[kk]/sdiag[k];
				sin = p5/sqrt(p25+p25*SQR(cotan));
				cos = sin*cotan;
      }
      else
      {
				tan = sdiag[k]/r[kk];
				cos = p5/sqrt(p25+p25*SQR(tan));
				sin = cos*tan;
      }

			//compute the modified diagonal element of r and the modified element of ((q transpose)*b,0).
      r[kk] = cos*r[kk] + sin*sdiag[k];
      temp = cos*wa[k] + sin*qtbpj;
      qtbpj = -sin*wa[k] + cos*qtbpj;
      wa[k] = temp;

			//accumulate the tranformation in the row of s.
      for (i=k+1; i<n; i++)
      {
        temp = cos*r[k*ldr+i] + sin*sdiag[i]; 
        sdiag[i] = -sin*r[k*ldr+i] + cos*sdiag[i];
        r[k*ldr+i] = temp;
      }
		}

    L90:
		//store the diagonal element of s and restore the corresponding diagonal element of r.
		sdiag[j] = r[j*ldr+j];
		r[j*ldr+j] = x[j];
  }

	//solve the triangular system for z. if the system is singular, then obtain a least squares solution.
  nsing = n;
  for (j=0; j<n; j++)
  {
		if (sdiag[j] == 0. && nsing == n)
      nsing = j;
		if (nsing < n)
      wa[j] = 0;
  }

  for (j=nsing-1; j>=0; j--)
  {
		sum = 0;
    for (i=j+1; i<nsing; i++)
        sum += r[j*ldr+i]*wa[i];
		wa[j] = (wa[j] - sum)/sdiag[j];
  }

	//permute the components of z back to components of x.
	for (j=0; j<n; j++)
	x[ipvt[j] ] = wa[j];
}

void CInspection::lm_initialize_control(lm_control_type *control)
{
  control->maxcall = vGlobalCurveFitMaxCall; //100; 
  control->epsilon = vGlobalCurveFitEpsilon; //1.e-14;
  control->stepbound = vGlobalCurveFitStepBound; // 100
  control->ftol = vGlobalCurveFitFTolerance;//1.e-14; //termination Tolerance
  control->xtol = vGlobalCurveFitXTolerance;//1.e-14;
  control->gtol = vGlobalCurveFitGTolerance;//1.e-14;
}

void CInspection::lm_minimize(int m_dat, int n_par, double* par, void *data, lm_control_type *control)
{
	//allocate work space.
  double *fvec, *diag, *fjac, *qtf, *wa1, *wa2, *wa3, *wa4;
  int *ipvt;
  int n = n_par;
  int m = m_dat;

  if (!(fvec = (double*) malloc(  m*sizeof(double))) ||
    !(diag = (double*) malloc(n*  sizeof(double))) ||
    !(qtf =  (double*) malloc(n*  sizeof(double))) ||
    !(fjac = (double*) malloc(n*m*sizeof(double))) ||
    !(wa1 =  (double*) malloc(n*  sizeof(double))) ||
    !(wa2 =  (double*) malloc(n*  sizeof(double))) ||
    !(wa3 =  (double*) malloc(n*  sizeof(double))) ||
    !(wa4 =  (double*) malloc(  m*sizeof(double))) ||
    !(ipvt = (int*)    malloc(n*  sizeof(int)))) 
	{
    control->info = 9;
    return;
  }

	//perform fit.
  control->info = 0;
  control->nfev = 0;

  // this goes through the modified legacy interface:
	//lm_lmdif was original name
  LeastSquaresCurveFit(m, n, par, fvec, control->ftol, control->xtol, control->gtol,
    control->maxcall*(n+1), control->epsilon, diag, 1, control->stepbound, &(control->info),
    &(control->nfev), fjac, ipvt, qtf, wa1, wa2, wa3, wa4, data);

  control->fnorm = lm_enorm(m, fvec);
  if (control->info < 0)
		control->info = 10;

	//Free allocated Memory.
  free(fvec);
  free(diag);
  free(qtf); 
  free(fjac);
  free(wa1); 
  free(wa2); 
  free(wa3 );
  free(wa4); 
  free(ipvt);
}

double My_Fit_Function9(double t, double* p)
{
	vCallsToSquareFunction = vCallsToSquareFunction + 1;
	return ((p[9]*t*t*t*t*t*t*t*t*t) + (p[8]*t*t*t*t*t*t*t*t) + (p[7]*t*t*t*t*t*t*t) + (p[6]*t*t*t*t*t*t) + (p[5]*t*t*t*t*t) + (p[4]*t*t*t*t) + (p[3]*t*t*t) + (p[2]*t*t) + (p[1]*t) + p[0]);
}

double My_Fit_Function8(double t, double* p)
{
	vCallsToSquareFunction = vCallsToSquareFunction + 1;
	return ((p[8]*t*t*t*t*t*t*t*t) + (p[7]*t*t*t*t*t*t*t) + (p[6]*t*t*t*t*t*t) + (p[5]*t*t*t*t*t) + (p[4]*t*t*t*t) + (p[3]*t*t*t) + (p[2]*t*t) + (p[1]*t) + p[0]);
}

double My_Fit_Function7(double t, double* p)
{
	vCallsToSquareFunction = vCallsToSquareFunction + 1;
	return ((p[7]*t*t*t*t*t*t*t) + (p[6]*t*t*t*t*t*t) + (p[5]*t*t*t*t*t) + (p[4]*t*t*t*t) + (p[3]*t*t*t) + (p[2]*t*t) + (p[1]*t) + p[0]);
}

double My_Fit_Function6(double t, double* p)
{
	vCallsToSquareFunction = vCallsToSquareFunction + 1;
	return ((p[6]*t*t*t*t*t*t) + (p[5]*t*t*t*t*t) + (p[4]*t*t*t*t) + (p[3]*t*t*t) + (p[2]*t*t) + (p[1]*t) + p[0]);
}

double My_Fit_Function5(double t, double* p)
{
	vCallsToSquareFunction = vCallsToSquareFunction + 1;
	return ((p[5]*t*t*t*t*t) + (p[4]*t*t*t*t) + (p[3]*t*t*t) + (p[2]*t*t) + (p[1]*t) + p[0]);
}

double My_Fit_Function4(double t, double* p)
{
	vCallsToSquareFunction = vCallsToSquareFunction + 1;
	return ((p[4]*t*t*t*t) + (p[3]*t*t*t) + (p[2]*t*t) + (p[1]*t) + p[0]);
}

double My_Cubic_Fit_Function(double t, double* p)
{
	//return ( p[0]*t + (1-p[0]+p[1]+p[2])*t*t ) / ( 1 + p[1]*t + p[2]*t*t );  //example that came with library
	vCallsToSquareFunction = vCallsToSquareFunction + 1;
	return ((p[3]*t*t*t) + (p[2]*t*t) + (p[1]*t) + p[0] ); //for cubic function
}

double My_Square_Fit_Function(double t, double* p)
{
	vCallsToSquareFunction = vCallsToSquareFunction + 1;
	return ((p[2]*t*t) + (p[1]*t) + p[0]); //for square function
}

double My_Line_Fit_Function(double t, double* p)
{
	vCallsToSquareFunction = vCallsToSquareFunction + 1;
	return ((p[1]*t) + p[0]); //for line function
}

//double CInspection::CurveFitFillInLightSpots(BYTE *TempImage, WORD TempSizeX, WORD TempSizeY)
//{
//	if (vEnable)
//	{
//		WORD TempBottom = vROIPixelBottom;
//		if (TempBottom >= TempSizeX)
//			TempBottom = 0;
//		WORD TempLeft = vROIPixelLeft;
//		WORD TempRight = vROIPixelRight;
//		//if (TempLeft < 5)
//		//	TempLeft = 5;
//		//if (TempRight + 5 > TempSizeY)
//		//	TempRight = TempSizeY - 5;
//
//		WORD TempHeight = vROIPixelTop - TempBottom;
//		if (TempHeight + TempBottom > TempSizeX)
//			TempHeight = TempSizeX - TempBottom;
//
//		WORD TempWidth = TempRight - vROIPixelLeft;
//		if (TempWidth + TempLeft > TempSizeY)
//			TempWidth = TempSizeY - TempLeft;
//
//		//if (TempWidth + 10 > TempSizeY)
//		//	TempWidth = TempSizeY - 10;
//
//		WORD TempHalfWidth = TempWidth / 2;
//		WORD TempCenter = TempLeft + TempHalfWidth;
//
//		if (TempImage)
//		{
//			BYTE *TempCurrentPixel = NULL;
//			BYTE *TempMaximumPointer = TempImage + (TempSizeY * TempSizeX);
//			//scanning from left to right, eliminate white spots on each line
//			for (WORD TempLineLoop = 0; TempLineLoop <= TempWidth; TempLineLoop++)
//			{
//				//start at the bottom ROI line and study every line up to the top ROI line
//				TempCurrentPixel = TempImage + TempBottom + ((TempLineLoop + TempLeft) * TempSizeX);
//				BYTE TempMedianValueOfLine = FindMedianValue(TempCurrentPixel, TempHeight);
//				TempCurrentPixel = TempImage + TempBottom + ((TempLineLoop + TempLeft) * TempSizeX);
//
//				for (WORD TempPixelLoop = 0; TempPixelLoop <= TempHeight; TempPixelLoop++)
//				{
//					if (TempCurrentPixel < TempMaximumPointer)
//					{
//						if (*TempCurrentPixel > TempMedianValueOfLine)
//							*TempCurrentPixel = TempMedianValueOfLine;
//
//						TempCurrentPixel++;
//					}
//					else
//						ReportErrorMessage("Bad Pointer 1w", cEMailInspx,32000);
//				}
//			}
//		}
//	}
///*	double TempCurveFitTime = 0;
//	if (vEnable)
//	{
//		WORD TempBottom = vROIPixelBottom;
//		if (TempBottom >= TempSizeX)
//			TempBottom = 0;
//		WORD TempLeft = vROIPixelLeft;
//		WORD TempRight = vROIPixelRight;
//		if (TempRight + 5 > TempSizeY)
//			TempRight = TempSizeY - 5;
//		if (TempLeft >= TempRight)
//			TempLeft = 5;
//		if (TempLeft < 5)
//			TempLeft = 5;
//
//		WORD TempHeight = vROIPixelTop - TempBottom;
//		if (TempHeight + TempBottom > TempSizeX)
//			TempHeight = TempSizeX - TempBottom;
//
//		WORD TempWidth = TempRight - vROIPixelLeft;
//		if (TempWidth + TempLeft > TempSizeY)
//			TempWidth = TempSizeY - TempLeft;
//
//		if (TempWidth + 10 > TempSizeY)
//			TempWidth = TempSizeY - 10;
//
//		WORD TempHalfWidth = TempWidth / 2;
//
//		BYTE *TempMaxImage = TempImage + ((DWORD)TempSizeX * (DWORD)TempSizeY);
//
//		if (TempImage)
//		{
//			//start at the bottom ROI line and study every line up to the top ROI line
//			for (WORD TempLineLoop = 0; TempLineLoop < TempHeight; TempLineLoop++)
//			{
//				BYTE *TempLineData = NULL;
//				if (ThereIsEnoughMemory(TempWidth, "Fill In White Spots Line Buffer"))
//					TempLineData = (BYTE *)malloc(TempWidth);
//
//				if (TempLineData)
//				{
//					//copy this line out of the image to the temporary line holder
//					BYTE *TempImagePointer = TempImage + TempLineLoop + TempBottom + (TempLeft * TempSizeX);
//					BYTE *TempLinePointer = TempLineData;
//					for (WORD TempLoop = 0; TempLoop < TempWidth; TempLoop++)
//					{
//						if (TempImagePointer < TempMaxImage)
//						{
//							*TempLinePointer = *TempImagePointer;
//							TempLinePointer++;
//							TempImagePointer = TempImagePointer + TempSizeX;
//						}
//						//else
//						//	ReportErrorMessage("CurveFitFillInLightSpots Pointer Error 1", cEMailInspx,0);
//					}
//
//					//replace the line data with the fitted curve to the line data and ends set to white
//					TempCurveFitTime = TempCurveFitTime + ReturnCurveFitLine(TempLineData, TempWidth, NULL, vGlobalCurveFitEquationOrder);
//
//					//scan down the line that you have fit if any data point is higer than the fitted line, set it to the fitted line 
//					TempImagePointer = TempImage + TempLineLoop + TempBottom + (TempLeft * TempSizeX);
//					TempLinePointer = TempLineData;
//					for (WORD TempLoop = 0; TempLoop < TempWidth; TempLoop++)
//					{
//						if (TempImagePointer < TempMaxImage)
//						{
//							if (*TempLinePointer == 255)
//								*TempImagePointer = 255;
//							else
//							if (*TempImagePointer > *TempLinePointer)
//								*TempImagePointer = *TempLinePointer;
//							TempLinePointer++;
//							TempImagePointer = TempImagePointer + TempSizeX;
//						}
//						//else
//						//	ReportErrorMessage("CurveFitFillInLightSpots Pointer Error 2", cEMailInspx,0);
//					}
//				}
//				if (TempLineData)
//					free(TempLineData);
//			}
//			BYTE *TempImagePointer = NULL;
//			//fill in to left of ROI
//			if (TempLeft)
//			{
//				DWORD TempNumberOfPixelsToFill = TempLeft * TempSizeX;
//				if (TempLeft < TempSizeY)
//					FillMemory(TempImage, TempNumberOfPixelsToFill, 255);
//				//else
//				//	ReportErrorMessage("CurveFitFillInLightSpots Pointer Error 3", cEMailInspx,0);
//
//			}
//
//			//fill in to right of ROI
//			if (TempRight < TempSizeY)
//			{
//				int TempNumberOfLinesToFill = TempSizeY - TempRight;
//				DWORD TempNumberOfPixelsToFill = TempNumberOfLinesToFill * TempSizeX;
//				if (TempImage + (TempRight * TempSizeX) + TempNumberOfPixelsToFill < TempMaxImage)
//					FillMemory(TempImage + (TempRight * TempSizeX), TempNumberOfPixelsToFill, 255);
//				//else
//				//	ReportErrorMessage("CurveFitFillInLightSpots Pointer Error 4", cEMailInspx,0);
//
//				//TempImagePointer = TempImage + (TempRight * TempSizeX);  //start at bottom below Right ROI Line
//			}
//
//			//fill in below ROI
//			if (TempBottom)
//			{
//				int TempNumberOfLinesToFill = TempRight - TempLeft;
//				for (WORD TempLineLoop = 0; TempLineLoop < TempNumberOfLinesToFill; TempLineLoop++)
//				{
//					//FillMemory(TempImage + ((TempLineLoop  + TempLeft) * TempSizeX), TempBottom, 255);
//					if ((BYTE *)(TempImage + ((TempLineLoop  + TempLeft) * TempSizeX + TempBottom)) < TempMaxImage)
//					{
//						TempImagePointer = TempImage + ((TempLineLoop  + TempLeft) * TempSizeX);
//						for (WORD TempPixelLoop = 0; TempPixelLoop < TempBottom; TempPixelLoop++)
//							*TempImagePointer++ = 255;
//					}
//					//else
//					//	ReportErrorMessage("CurveFitFillInLightSpots Pointer Error 5", cEMailInspx,0);
//				}
//			}
//			//fill in above ROI
//			if (vROIPixelTop < TempSizeX)
//			{
//				int TempNumberOfLinesToFill = TempRight - TempLeft;
//				int TempNumberOfPixelsToFill = TempSizeX - vROIPixelTop;
//				for (WORD TempLineLoop = 0; TempLineLoop < TempNumberOfLinesToFill; TempLineLoop++)
//				{
//					if (TempImage + ((TempLineLoop  + TempLeft) * TempSizeX) + vROIPixelTop + TempNumberOfPixelsToFill < TempMaxImage)
//						FillMemory(TempImage + ((TempLineLoop  + TempLeft) * TempSizeX) + vROIPixelTop, TempNumberOfPixelsToFill, 255);
//					//else
//					//	ReportErrorMessage("CurveFitFillInLightSpots Pointer Error 6", cEMailInspx,0);
//					//TempImagePointer = TempImage + ((TempLineLoop  + TempRight) * TempSizeX);
//					//for (WORD TempPixelLoop = 0; TempPixelLoop < TempNumberOfPixelsToFill; TempPixelLoop++)
//					//	*TempImagePointer++ = 255;
//				}
//			}
//		}
//	}
//	*/
//	return 0;
//}

double CInspection::ReturnCurveFitLine(BYTE *TempImage, WORD TempSizeY, tCurveFitAnalysisData *vCurveFitAnalysisData, 
																			 int TempNumberOfTerms)
{
	double TempCurveFitTime = 0;
	vCallsToSquareFunction = 0;
	WORD TempHalfWidth = TempSizeY / 2;
	//must have trimmed edges to jar in image before this

	BYTE *TempMaxImage = TempImage + (DWORD)TempSizeY;

	if (TempSizeY <= 10 + TempNumberOfTerms)
		ReportErrorMessage("Ignore Curve Fit, array too small", cWriteToLog,0);
	else
	if (TempImage)
	{
		BYTE *TempCurrentPixel = TempImage;
		WORD TempLeftToCurveFit = 0;
		//scanning from left to center find left edge of data to curve fit
		//start with white near edge so look and see if has dark edge
		//scan in until find first non saturated pixel
		while ((*TempCurrentPixel == 255) && (TempLeftToCurveFit < TempHalfWidth - 2))
		{
			if (TempCurrentPixel < TempMaxImage - 1)
			{
				TempCurrentPixel++;
			}
			//else
			//	ReportErrorMessage("ReturnCurveFitLine Pointer Error 1", cEMailInspx,0);
			TempLeftToCurveFit++;
		}
		//found TempLeftToCurveFit

		//scanning from Right to center find left edge of data to curve fit
		TempCurrentPixel = TempImage + TempSizeY - 1;
		WORD TempRightToCurveFit = TempSizeY;  //??? am I off by one?
		//start with white near edge so look and see if has dark edge
		//scan in until find first non saturated pixel
		while ((*TempCurrentPixel == 255) && (TempRightToCurveFit > TempHalfWidth + 2))
		{
			if ((TempCurrentPixel < TempMaxImage) && (TempCurrentPixel > TempImage))
			{
				TempCurrentPixel--;
			}
			//else
			//	ReportErrorMessage("ReturnCurveFitLine Pointer Error 2", cEMailInspx,0);
			TempRightToCurveFit--;
		}
		//found TempRightToCurveFit

		//if any white pixels inside curve fit range, reduce curve fit range to exclude white pixel
		if (TempRightToCurveFit > TempLeftToCurveFit + 10)
		{
			//check left side
			TempCurrentPixel = TempImage + TempHalfWidth;
			bool TempFound255 = false;
			WORD TempNewLeftToCurveFit = TempLeftToCurveFit;
			for (int TempLoop = TempHalfWidth; TempLoop >= TempLeftToCurveFit; TempLoop--)
			{
				if ((TempCurrentPixel < TempMaxImage) && (TempCurrentPixel >= TempImage))
				{
					if (TempFound255)
						*TempCurrentPixel = 255;
					else
					if (*TempCurrentPixel == 255)
					{
						TempFound255 = true;
						TempNewLeftToCurveFit = TempLoop + 1;
					}
					TempCurrentPixel--;
				}
				//else
				//	ReportErrorMessage("ReturnCurveFitLine Pointer Error 3", cEMailInspx,0);
			}
			TempLeftToCurveFit = TempNewLeftToCurveFit;
			//check right side
			TempCurrentPixel = TempImage + TempHalfWidth;
			TempFound255 = false;
			WORD TempNewRightToCurveFit = TempRightToCurveFit;
			for (WORD TempLoop = TempHalfWidth; TempLoop < TempRightToCurveFit; TempLoop++)
			{
				if ((TempCurrentPixel < TempMaxImage) && (TempCurrentPixel >= TempImage))
				{
					if (TempFound255)
						*TempCurrentPixel = 255;
					else
					if (*TempCurrentPixel == 255)
					{
						TempFound255 = true;
						TempNewRightToCurveFit = TempLoop;
					}
					TempCurrentPixel++;
				}
				//else
				//	ReportErrorMessage("ReturnCurveFitLine Pointer Error 4", cEMailInspx,0);
			}
			TempRightToCurveFit = TempNewRightToCurveFit;
		}

		/*
		if (TempRightToCurveFit <= TempLeftToCurveFit)
			ReportErrorMessage("Glass2C Did Not Find Edges", cWriteToLog,32000);
		else
		if (TempRightToCurveFit <= TempLeftToCurveFit + 10)
			ReportErrorMessage("Glass2C Less Than 10 Points", cWriteToLog,32000);
		else
			*/
		if (TempRightToCurveFit > TempLeftToCurveFit + 10)
		{
			int TempDataSize = TempRightToCurveFit - TempLeftToCurveFit;

			BYTE TempNumberOfDataPointsDivideFactor = 1;
			if (TempDataSize > 200) //b rh
				TempNumberOfDataPointsDivideFactor = 2;
			int TempCurveFitSize = TempDataSize / TempNumberOfDataPointsDivideFactor;

			//for cubic equation
			//int TempNumberOfTerms = 4;
			//double TempEquationTerms[4] = { 148, -1.7, .035, -.00016};//use any starting value, but not { 0,0,0 } //initial guess at function terms

			//for square equation
			double TempEquationTerms[10] = {213.5161, -3.2377, .0416, .001, .001, .001, .001, .001, .001, .001};//use any starting value, but not { 0,0,0 } //initial guess at function terms

			double *TempXData = NULL;
			double *TempYData = NULL;

			if (ThereIsEnoughMemory(TempCurveFitSize * 2 * sizeof(double), "Line Derivative Buffer"))
			{
				TempXData = (double *)malloc(TempCurveFitSize * sizeof(double));
				TempYData = (double *)malloc(TempCurveFitSize * sizeof(double));
			}

			if ((TempXData) && (TempYData))
			{
				double *TempXDataPointer = TempXData;
				double *TempYDataPointer = TempYData;
				BYTE *TempImagePointer = TempImage + TempLeftToCurveFit;
				for (WORD TempLoop = 0; TempLoop < TempCurveFitSize; TempLoop++)
				{
					if ((TempImagePointer < TempMaxImage - TempNumberOfDataPointsDivideFactor + 1) && (TempImagePointer >= TempImage))
					{
						*TempXDataPointer++ = TempLoop; //set x data
						*TempYDataPointer = *TempImagePointer++;//set Y data

						if (TempNumberOfDataPointsDivideFactor > 1)
						{
							for (WORD TempPixelLoop = 1; TempPixelLoop < TempNumberOfDataPointsDivideFactor; TempPixelLoop++)
								*TempYDataPointer = *TempYDataPointer + *TempImagePointer++; //sum up additional points

							*TempYDataPointer = *TempYDataPointer / TempNumberOfDataPointsDivideFactor;//divide number of points to get average of points
						}
						TempYDataPointer++;
					}
					//else
					//	ReportErrorMessage("ReturnCurveFitLine Pointer Error 5", cEMailInspx,0);
				}

				lm_control_type control; //curve fit parameters
				lm_initialize_control(&control);

				lm_data_type data; //curve fit data
				switch (TempNumberOfTerms)
				{
					case 10: data.user_func = My_Fit_Function9; break;
					case 9: data.user_func = My_Fit_Function8; break;
					case 8: data.user_func = My_Fit_Function7; break;
					case 7: data.user_func = My_Fit_Function6; break;
					case 6: data.user_func = My_Fit_Function5; break;
					case 5: data.user_func = My_Fit_Function4; break;
					case 4: data.user_func = My_Cubic_Fit_Function; break;//cubic
					case 3: data.user_func = My_Square_Fit_Function; break;
					case 2: data.user_func = My_Line_Fit_Function; break;
				}
				data.user_t = TempXData;
				data.user_y = TempYData;

				double TempStartTime = CIFCOS::GetSystimeMicrosecs();
				// perform the fit:
				lm_minimize(TempCurveFitSize, TempNumberOfTerms, &TempEquationTerms[0], &data, &control);
				//SetDlgItemText(IDC_InspectionThreshold,"CurveFit:" + dtoa(TempEquationTerms[3],2) + "X3 + " + dtoa(TempEquationTerms[2],2) +
				//	"X2 + " +	dtoa(TempEquationTerms[1],2) + "X + " + dtoa(TempEquationTerms[0],2));

				TempCurveFitTime = CIFCOS::GetSystimeMicrosecs() - TempStartTime;

				//scan down the line that you have fit.
				TempCurrentPixel = TempImage + TempLeftToCurveFit;
				for (WORD TempLoop = 0; TempLoop < TempDataSize; TempLoop++)
				{
					if ((TempCurrentPixel < TempMaxImage) && (TempCurrentPixel >= TempImage))
					{
						double TempX = (double)TempLoop / (double)TempNumberOfDataPointsDivideFactor;
						double TempCalcY =
							(TempEquationTerms[2] * TempX * TempX) +  (TempEquationTerms[1] * TempX) + TempEquationTerms[0]; //square
						if (TempNumberOfTerms > 3)
							TempCalcY = TempCalcY + (TempEquationTerms[3] * TempX * TempX * TempX);//cubic 
						if (TempNumberOfTerms > 4)
							TempCalcY = TempCalcY + (TempEquationTerms[4] * TempX * TempX * TempX * TempX);
						if (TempNumberOfTerms > 5)
							TempCalcY = TempCalcY + (TempEquationTerms[5] * TempX * TempX * TempX * TempX * TempX);
						if (TempNumberOfTerms > 6)
							TempCalcY = TempCalcY + (TempEquationTerms[6] * TempX * TempX * TempX * TempX * TempX * TempX);
						if (TempNumberOfTerms > 7)
							TempCalcY = TempCalcY + (TempEquationTerms[7] * TempX * TempX * TempX * TempX * TempX * TempX * TempX);
						if (TempNumberOfTerms > 8)
							TempCalcY = TempCalcY + (TempEquationTerms[8] * TempX * TempX * TempX * TempX * TempX * TempX * TempX * TempX);
						if (TempNumberOfTerms > 9)
							TempCalcY = TempCalcY + (TempEquationTerms[9] * TempX * TempX * TempX * TempX * TempX * TempX * TempX * TempX * TempX);

						if (TempCalcY > 255)
							TempCalcY = 255;
						if (TempCalcY < 0)
							TempCalcY = 0;
						*TempCurrentPixel = (BYTE)(TempCalcY + .5);
						TempCurrentPixel++;
					}
					//else
					//	ReportErrorMessage("ReturnCurveFitLine Pointer Error 6", cEMailInspx,0);
				}
				if (vCurveFitAnalysisData)
				{
					vCurveFitAnalysisData->vNumberOfPointsToPlot = TempCurveFitSize;
					vCurveFitAnalysisData->vCallsToSquare = vCallsToSquareFunction;
					vCurveFitAnalysisData->vMainCalls = control.nfev;
					for (BYTE TempLoop = 0; TempLoop < 10; TempLoop++)
						vCurveFitAnalysisData->vTerms[TempLoop] = TempEquationTerms[TempLoop];
					vCurveFitAnalysisData->vTerminationReason = control.info;
					vCurveFitAnalysisData->vFNorm = control.fnorm;
				}
			}
			if (TempXData)
				free(TempXData);
			if (TempYData)
				free(TempYData);
		}
	}
	return TempCurveFitTime;
}

double CInspection::ClipToCurveFit(BYTE *TempImage, WORD TempSizeX, WORD TempSizeY)//find contaminants below curve fit
{
	double TempCurveFitTime = 0;
	if (vEnable)
	{
		WORD TempBottom = vROIPixelBottom;
		if (TempBottom >= vROIPixelTop)
			TempBottom = 0;
		WORD TempRight = vROIPixelRight;
		if (TempRight > TempSizeY)
			TempRight = TempSizeY;
		WORD TempLeft = vROIPixelLeft;
		if (TempLeft >= TempRight)
			TempLeft = 0;
		if (TempLeft)
			TempLeft--;

		WORD TempHeight = vROIPixelTop - TempBottom;
		if (TempHeight + TempBottom > TempSizeX)
			TempHeight = TempSizeX - TempBottom;

		WORD TempWidth = TempRight - TempLeft;
		if (TempWidth + TempLeft > TempSizeY)
			TempWidth = TempSizeY - TempLeft;

		WORD TempHalfWidth = TempWidth / 2;

		BYTE *TempMaxImage = TempImage + ((DWORD)TempSizeX * (DWORD)TempSizeY);

		if (TempImage)
		{
			//start at the bottom ROI line and study every line up to the top ROI line
			for (WORD TempLineLoop = 0; TempLineLoop < TempHeight; TempLineLoop++)
			{
				BYTE *TempLineData = NULL;
				if (ThereIsEnoughMemory(TempWidth, "Fill In Light Spots Line Buffer"))
					TempLineData = (BYTE *)malloc(TempWidth);

				if (TempLineData)
				{
					//copy this line out of the image to the temporary line holder
					BYTE *TempImagePointer = TempImage + TempLineLoop + TempBottom + (TempLeft * TempSizeX);
					BYTE *TempLinePointer = TempLineData;
					for (WORD TempLoop = 0; TempLoop < TempWidth; TempLoop++)
					{
						if ((TempImagePointer < TempMaxImage) && (TempImagePointer >= TempImage))
						{
							*TempLinePointer = *TempImagePointer;
							TempLinePointer++;
							TempImagePointer = TempImagePointer + TempSizeX;
						}
						//else
						//	ReportErrorMessage("ClipToCurveFit Pointer Error 1", cEMailInspx, 32000);
					}

					//replace the line data with the fitted curve to the line data and ends set to white
					TempCurveFitTime = TempCurveFitTime + ReturnCurveFitLine(TempLineData, TempWidth, NULL, vGlobalCurveFitEquationOrder);

					//scan down the line that you have fit if any data point is higer than the fitted line, set it to the fitted line 
					TempImagePointer = TempImage + TempLineLoop + TempBottom + (TempLeft * TempSizeX);
					TempLinePointer = TempLineData;
					for (DWORD TempLoop = 0; TempLoop < TempWidth; TempLoop++)
					{
						if ((TempImagePointer < TempMaxImage) && (TempImagePointer >= TempImage))
						{
							if (*TempLinePointer == 255)//if curve fit line clipped pixel off, clip off output image pixel
								*TempImagePointer = 255;
							else
							if (*TempImagePointer < *TempLinePointer)  //if image pixel is below curve fit pixel, then set to 255-(amount below curve)
								*TempImagePointer = 255 - (*TempLinePointer - *TempImagePointer);
							else
								*TempImagePointer = 255; //image was not below curve fit, so pixel OK so set to white
							TempLinePointer++;
							TempImagePointer = TempImagePointer + TempSizeX;
						}
						//else
						//	ReportErrorMessage("ClipToCurveFit Pointer Error 2", cEMailInspx, 32000);
					}
				}
				if (TempLineData)
					free(TempLineData);
			}
		}
	}
	return TempCurveFitTime;
}

void CInspection::TrimEdgesOfJarAtROI(BYTE *TempImage, WORD TempSizeX, WORD TempSizeY, bool TempSetBackgroundWhite, 
		WORD TempLeftSide, WORD TempRightSide)
{
	if (vEnable)
	if (!vFindSpotInSubROI)
	{
		if (vROIPixelRight > TempSizeY)
			vROIPixelRight = TempSizeY - 1;
		if (vROIPixelLeft >= vROIPixelRight)
			vROIPixelLeft = 1;

		WORD TempBottom = vROIPixelBottom;
		WORD TempTop = vROIPixelTop;
		if (TempTop >= TempSizeX)
			TempTop = TempSizeX - 1;
		if (TempBottom >= vROIPixelTop)
			TempBottom = 1;
		WORD TempLeft = vROIPixelLeft;
		WORD TempRight = vROIPixelRight;
		if (TempRight > TempSizeY)
			TempRight = TempSizeY - 1;
		if (TempLeft >= TempRight)
			TempLeft = 1;

		WORD TempLeftToFill = TempLeft;
		WORD TempRightToFill = TempRight;

		//trim to the edge of the jar instead of the ROI if know the edge of the jar
		if (TempRightSide)
		{
			TempLeft = TempLeftSide;
			TempRight = TempRightSide;

			if (TempRight > TempSizeY)
				TempRight = TempSizeY - 1;
			if (TempLeft >= TempRight)
				TempLeft = 1;

			if (TempRightToFill > TempRight)
			if (TempRight > 20)
			if (TempRight < TempSizeY)
				TempRightToFill = TempRight;
			if (TempLeftToFill < TempLeft)
				TempLeftToFill = TempLeft;
		}

		WORD TempHeight = TempTop - TempBottom;
		if (TempHeight + TempBottom >= TempSizeX)
			TempHeight = TempSizeX - TempBottom - 1;

		WORD TempWidth = TempRight - TempLeft;
		if (TempWidth + TempLeft >= TempSizeY)
			TempWidth = TempSizeY - TempLeft - 1;

		WORD TempHalfWidth = TempWidth / 2;

		BYTE *TempMaximumImagePointer = TempImage + ((DWORD)TempSizeX * (DWORD)TempSizeY);

		if (TempImage)
		if (TempWidth > 10)
		{
			//start at the bottom ROI line and trim every line up to the top ROI line
			for (WORD TempLineLoop = 0; TempLineLoop < TempHeight; TempLineLoop++)
			{
				BYTE *TempCurrentPixel = NULL;
				BYTE *TempNextPixel = NULL;
				bool TempFoundDarkeningArea = false;

				//scanning from left to center find left edge of data to curve fit
				TempCurrentPixel = TempImage + TempBottom + TempLineLoop + (TempLeft * TempSizeX);
				TempNextPixel = TempCurrentPixel + TempSizeX;
				TempFoundDarkeningArea = false;
				WORD TempLeftToCurveFit = 0;

				bool TempNeedToTrim = false;

				//start with white near edge so look and see if has dark edge
				//scan in until find first non saturated pixel
				while ((*TempCurrentPixel == 255) && (TempLeftToCurveFit < TempHalfWidth - 2))
				{
					if ((TempCurrentPixel >= TempImage) && (TempCurrentPixel < TempMaximumImagePointer))
						TempCurrentPixel = TempCurrentPixel + TempSizeX;
					TempLeftToCurveFit++;
				}

				//check next 15 pixels to see if another white spot, if so, move in 
				BYTE *TempWhiteSpotCheckPixel = TempCurrentPixel + TempSizeX;
				BYTE TempFoundAnotherWhiteSpot = 0;

				BYTE TempPixelsToScan = 15;
				if (vFindSpotInSubROI)
					TempPixelsToScan = 2;

				//scan in until find first non saturated pixel
				for (BYTE TempLoop = 0; TempLoop < TempPixelsToScan; TempLoop++)  //scan the next 5 pixels for a dark edge of the jar
				if ((TempWhiteSpotCheckPixel >= TempImage) && (TempWhiteSpotCheckPixel < TempMaximumImagePointer))
				{
					if (*TempWhiteSpotCheckPixel == 255)
					{
						TempFoundAnotherWhiteSpot = TempLoop + 1;
						TempLoop = 15;
					}
					TempWhiteSpotCheckPixel = TempWhiteSpotCheckPixel + TempSizeX;
				}
				if (TempFoundAnotherWhiteSpot) //set pixels until white spot white
				{
					for (BYTE TempLoop = 0; TempLoop <= TempFoundAnotherWhiteSpot; TempLoop++)  //scan the next 5 pixels for a dark edge of the jar
					if ((TempCurrentPixel >= TempImage) && (TempCurrentPixel < TempMaximumImagePointer))
					{
						*TempCurrentPixel = 255;
						TempLeftToCurveFit++;
						TempCurrentPixel = TempCurrentPixel + TempSizeX;
					}
					//scan in until find first non saturated pixel
					while ((*TempCurrentPixel == 255) && (TempLeftToCurveFit < TempHalfWidth - 2))
					{
						TempLeftToCurveFit++;
						if ((TempCurrentPixel >= TempImage) && (TempCurrentPixel < TempMaximumImagePointer))
							TempCurrentPixel = TempCurrentPixel + TempSizeX;
					}
				}
				else
				{
					//look and see if there is a major darkening, then lightening, if so, 
					//find the peak of the lightening to start from, if not, start from TempLeftToCurveFit
					BYTE TempPixelNumberOfDarkest = 0;
					if (TempLeftToCurveFit < TempHalfWidth - 15)
					{
						BYTE TempDarkestInNextFivePixels = 255;
						BYTE *TempDarkPixelScanning = TempCurrentPixel;
						BYTE TempLightestInNextFivePixels = 0;
						BYTE TempPixelNumberOfLightest = 0;
						BYTE *TempLightPixelScanning = TempCurrentPixel;
						bool TempGettingDarker = true;

						BYTE TempPixelsToScan = 13;
						if (vFindSpotInSubROI)
							TempPixelsToScan = 2;

						for (BYTE TempLoop = 0; TempLoop < TempPixelsToScan; TempLoop++)  //scan the next only 4 (5 too much) pixels for a dark edge of the jar
						{
							if ((TempDarkPixelScanning >= TempImage) && (TempDarkPixelScanning < TempMaximumImagePointer))
							if (TempDarkestInNextFivePixels >= *TempDarkPixelScanning) //if next pixel is darker, save it
							{
								TempDarkestInNextFivePixels = *TempDarkPixelScanning;
								TempPixelNumberOfDarkest = TempLoop;
								TempLightPixelScanning = TempDarkPixelScanning;
							}
							else 
							if (TempDarkestInNextFivePixels + 2 < *TempDarkPixelScanning) //if next pixel not darker, then not getting darker
								TempLoop = 25;
							TempDarkPixelScanning = TempDarkPixelScanning + TempSizeX;
						}
						TempLightestInNextFivePixels = TempDarkestInNextFivePixels;
						TempPixelNumberOfLightest = TempPixelNumberOfDarkest;

						TempPixelsToScan = 6;
						if (vFindSpotInSubROI)
							TempPixelsToScan = 2;

						for (BYTE TempLoop = 0; TempLoop < TempPixelsToScan; TempLoop++) //scan the next 5 pixels for a lightening in the image
						{
							if ((TempLightPixelScanning >= TempImage) && (TempLightPixelScanning < TempMaximumImagePointer))
							if (TempLightestInNextFivePixels < *TempLightPixelScanning) //if next pixel is lighter, save it
							{
								TempLightestInNextFivePixels = *TempLightPixelScanning;
								TempPixelNumberOfLightest = TempLoop;
							}
							else
							if (TempLightestInNextFivePixels > *TempLightPixelScanning + 2) 
								TempLoop = 25;
							TempLightPixelScanning = TempLightPixelScanning + TempSizeX;
						}
						if (TempLightestInNextFivePixels > TempDarkestInNextFivePixels + 5) //if lightest pixel more than 15 more than darkest, need to trim
						{
							TempNeedToTrim = true;//need to trim dark edge of the jar off
						}
					}

					//if (TempNeedToTrim)
					if ((TempNeedToTrim) || (vPerformGlassAlgorithm == 1))
					{
						if (TempNeedToTrim)
						for (WORD TempPixelLoop = 0; TempPixelLoop < TempPixelNumberOfDarkest; TempPixelLoop++) //set area that is darkening to white
						if ((TempCurrentPixel >= TempImage) && (TempCurrentPixel < TempMaximumImagePointer))
						{
							*TempCurrentPixel = 255; //set current pixel white
							TempCurrentPixel = TempCurrentPixel + TempSizeX;
						}

						TempNextPixel = TempCurrentPixel + TempSizeX; //current pixel was one pixel past start point, so it is the next pixel
						TempLeftToCurveFit = TempLeftToCurveFit + TempPixelNumberOfDarkest; //set left edge to darkest pixel found

						WORD TempMaximumPixelsToScan = 15;  //only scan over maximum of 15 pixels blanking out the pixels getting lighter
						if (!TempNeedToTrim) 
							TempMaximumPixelsToScan = 6;

						if (vFindSpotInSubROI)
							TempMaximumPixelsToScan = 2;

						if (TempLeftToCurveFit + TempMaximumPixelsToScan > TempHalfWidth)
							TempMaximumPixelsToScan = TempHalfWidth - TempLeftToCurveFit;

						BYTE *TempNextPixelAfterThat = TempNextPixel + TempSizeX;
						BYTE *TempLastPixelAfterThat = TempNextPixelAfterThat + TempSizeX;
						for (WORD TempPixelLoop = 0; TempPixelLoop < TempMaximumPixelsToScan; TempPixelLoop++)
						{
							if ((TempCurrentPixel >= TempImage) && (TempCurrentPixel < TempMaximumImagePointer))
							if (TempNeedToTrim)
							{
								if ((*TempNextPixel < *TempCurrentPixel) && (*TempNextPixelAfterThat < *TempCurrentPixel) && 
									((*TempLastPixelAfterThat < *TempCurrentPixel) || (vPerformGlassAlgorithm == 2))) //found lightest spot of next 2 if derivative
									TempPixelLoop = TempMaximumPixelsToScan; //found lightest pixel of next three, so exit loop
								else
								{
									TempLeftToCurveFit++;
									*TempCurrentPixel = 255; //set this pixel white to blank it
								}
							}
							else
							{  //if did not need to trim, eliminate any rise
								if (*TempNextPixel < *TempCurrentPixel)
									TempPixelLoop = TempMaximumPixelsToScan; //found lightest pixel of next two, so exit loop
								else
								{
									TempLeftToCurveFit++;
									*TempCurrentPixel = 255; //set this pixel white to blank it
								}
							}
							TempCurrentPixel = TempNextPixel;
							TempNextPixel = TempNextPixelAfterThat;
							TempNextPixelAfterThat = TempLastPixelAfterThat;
							TempLastPixelAfterThat = TempLastPixelAfterThat + TempSizeX;
						}
					}
				}
				//scanning from Right to center find Right edge of data to curve fit
				TempCurrentPixel = TempImage + TempBottom + TempLineLoop + ((TempRight) * TempSizeX);
				WORD TempRightToCurveFit = TempRight;  // ??? am I off by one?

				TempNeedToTrim = false;

				//start with white near edge so look and see if has dark edge
				//scan in until find first non saturated pixel
				while ((*TempCurrentPixel == 255) && (TempRightToCurveFit > TempHalfWidth + 2))
				{
					if ((TempCurrentPixel >= TempImage) && (TempCurrentPixel < TempMaximumImagePointer))
						TempCurrentPixel = TempCurrentPixel - TempSizeX;
					TempRightToCurveFit--;
				}

				//check next 15 pixels to see if another white spot, if so, move in 
				TempWhiteSpotCheckPixel = TempCurrentPixel - TempSizeX;
				TempFoundAnotherWhiteSpot = 0;

				TempPixelsToScan = 15;
				if (vFindSpotInSubROI)
					TempPixelsToScan = 2;

				//scan in until find first non saturated pixel
				for (BYTE TempLoop = 0; TempLoop < TempPixelsToScan; TempLoop++)  //scan the next 5 pixels for a dark edge of the jar
				{
					if (*TempWhiteSpotCheckPixel == 255)
					{
						TempFoundAnotherWhiteSpot = TempLoop + 1;
						TempLoop = 15;
					}
					TempWhiteSpotCheckPixel = TempWhiteSpotCheckPixel - TempSizeX;
				}
				if (TempFoundAnotherWhiteSpot) //set pixels until white spot white
				{
					for (BYTE TempLoop = 0; TempLoop <= TempFoundAnotherWhiteSpot; TempLoop++)  //scan the next 5 pixels for a dark edge of the jar
					if ((TempCurrentPixel >= TempImage) && (TempCurrentPixel < TempMaximumImagePointer))
					{
						*TempCurrentPixel = 255;
						TempRightToCurveFit--;
						TempCurrentPixel = TempCurrentPixel - TempSizeX;
					}
					//scan in until find first non saturated pixel
					while ((*TempCurrentPixel == 255) && (TempRightToCurveFit > TempHalfWidth + 2))
					{
						TempRightToCurveFit--;
						if ((TempCurrentPixel >= TempImage) && (TempCurrentPixel < TempMaximumImagePointer))
							TempCurrentPixel = TempCurrentPixel - TempSizeX;
					}
				}
				else
				{
					//look and see if there is a major darkening, then lightening, if so, 
					//find the peak of the lightening to start from, if not, start from TempRightToCurveFit
					BYTE TempPixelNumberOfDarkest = 0;
					if (TempRightToCurveFit > TempHalfWidth + 15)
					{
						BYTE TempDarkestInNextFivePixels = 255;
						BYTE *TempDarkPixelScanning = TempCurrentPixel;
						BYTE TempLightestInNextFivePixels = 0;
						BYTE TempPixelNumberOfLightest = 0;
						BYTE *TempLightPixelScanning = TempCurrentPixel;
						bool TempGettingDarker = true;

						BYTE TempPixelsToScan = 13;
						if (vFindSpotInSubROI)
							TempPixelsToScan = 2;

						for (BYTE TempLoop = 0; TempLoop < TempPixelsToScan; TempLoop++)  //scan the next only 4 (5 too much) pixels for a dark edge of the jar
						if ((TempDarkPixelScanning >= TempImage) && (TempDarkPixelScanning < TempMaximumImagePointer))
						{
							if (TempDarkestInNextFivePixels >= *TempDarkPixelScanning) //if next pixel is darker, save it
							{
								TempDarkestInNextFivePixels = *TempDarkPixelScanning;
								TempPixelNumberOfDarkest = TempLoop;
								TempLightPixelScanning = TempDarkPixelScanning;
							}
							if (TempDarkestInNextFivePixels + 2 < *TempDarkPixelScanning) //if next pixel not darker, then not getting darker
								TempLoop = 25;
							TempDarkPixelScanning = TempDarkPixelScanning - TempSizeX;
						}
						TempLightestInNextFivePixels = TempDarkestInNextFivePixels;
						TempPixelNumberOfLightest = TempPixelNumberOfDarkest;

						TempPixelsToScan = 6;
						if (vFindSpotInSubROI)
							TempPixelsToScan = 2;

						for (BYTE TempLoop = 0; TempLoop < TempPixelsToScan; TempLoop++) //scan the next 5 pixels for a lightening in the image
						if ((TempLightPixelScanning >= TempImage) && (TempLightPixelScanning < TempMaximumImagePointer))
						{
							if (TempLightestInNextFivePixels < *TempLightPixelScanning) //if next pixel is lighter, save it
							{
								TempLightestInNextFivePixels = *TempLightPixelScanning;
								TempPixelNumberOfLightest = TempLoop;
							}
							else
							if (TempLightestInNextFivePixels > *TempLightPixelScanning + 2) //if next pixel is lighter, save it
								TempLoop = 25;
							TempLightPixelScanning = TempLightPixelScanning - TempSizeX;
						}
						if (TempLightestInNextFivePixels > TempDarkestInNextFivePixels + 5) //if lightest pixel more than 15 more than darkest, need to trim
						{
							TempNeedToTrim = true;//need to trim dark edge of the jar off
						}
					}

					//vPerformGlassAlgorithm
					if ((TempNeedToTrim) || (vPerformGlassAlgorithm == 1))
					{
						if (TempNeedToTrim)
						for (WORD TempPixelLoop = 0; TempPixelLoop < TempPixelNumberOfDarkest; TempPixelLoop++) //set area that is darkening to white
						if ((TempCurrentPixel >= TempImage) && (TempCurrentPixel < TempMaximumImagePointer))
						{
							*TempCurrentPixel = 255; //set current pixel white
							TempCurrentPixel = TempCurrentPixel - TempSizeX;
						}

						TempNextPixel = TempCurrentPixel - TempSizeX; //current pixel was one pixel past start point, so it is the next pixel
						TempRightToCurveFit = TempRightToCurveFit - TempPixelNumberOfDarkest; //set Right edge to darkest pixel found

						WORD TempMaximumPixelsToScan = 15;  //only scan over maximum of 15 pixels blanking out the pixels getting lighter
						if (!TempNeedToTrim) 
							TempMaximumPixelsToScan = 6;

						if (vFindSpotInSubROI)
							TempMaximumPixelsToScan = 2;

						if (TempRightToCurveFit < TempHalfWidth + TempMaximumPixelsToScan)
							TempMaximumPixelsToScan = TempRightToCurveFit - TempHalfWidth;

						BYTE *TempNextPixelAfterThat = TempNextPixel - TempSizeX;
						BYTE *TempLastPixelAfterThat = TempNextPixelAfterThat - TempSizeX;
						for (WORD TempPixelLoop = 0; TempPixelLoop < TempMaximumPixelsToScan; TempPixelLoop++)
						{
							if ((TempCurrentPixel >= TempImage) && (TempCurrentPixel < TempMaximumImagePointer))
							if (TempNeedToTrim)
							{
								if ((*TempNextPixel < *TempCurrentPixel) && (*TempNextPixelAfterThat < *TempCurrentPixel) && 
									((*TempLastPixelAfterThat < *TempCurrentPixel) || (vPerformGlassAlgorithm == 2)))//found lightest spot of next 2 if derivative
									TempPixelLoop = TempMaximumPixelsToScan; //found lightest pixel of next two, so exit loop
								else
								{
									TempRightToCurveFit--;
									*TempCurrentPixel = 255; //set this pixel white to blank it
								}
							}
							else
							{  //if did not need to trim, eliminate any rise
								if (*TempNextPixel < *TempCurrentPixel)
									TempPixelLoop = TempMaximumPixelsToScan; //found lightest pixel of next two, so exit loop
								else
								{
									TempRightToCurveFit--;
									*TempCurrentPixel = 255; //set this pixel white to blank it
								}
							}
							TempCurrentPixel = TempNextPixel;
							TempNextPixel = TempNextPixelAfterThat;
							TempNextPixelAfterThat = TempLastPixelAfterThat;
							TempLastPixelAfterThat = TempLastPixelAfterThat - TempSizeX;
						}
					}
				}
			}
			if (TempSetBackgroundWhite)
			{
				//fill white to the small area either the ROI or the edge of the jar
				BYTE *TempImagePointer = NULL;
				//fill in to left of ROI
				if (TempLeftToFill)
				{
					DWORD TempNumberOfPixelsToFill = TempLeftToFill * TempSizeX;
					if (TempImage + TempNumberOfPixelsToFill < TempMaximumImagePointer)
						FillMemory(TempImage, TempNumberOfPixelsToFill, 255);
				}

				//fill in to right of ROI
				if (TempRightToFill < TempSizeY)
				{
					int TempNumberOfLinesToFill = TempSizeY - TempRightToFill;
					DWORD TempNumberOfPixelsToFill = TempNumberOfLinesToFill * TempSizeX;
					BYTE *TempDestinationPointer = TempImage + (TempRightToFill * TempSizeX);
					if (TempDestinationPointer + TempNumberOfPixelsToFill >= TempMaximumImagePointer)
						TempNumberOfPixelsToFill = TempMaximumImagePointer - TempDestinationPointer - 1;
					FillMemory(TempDestinationPointer, TempNumberOfPixelsToFill, 255);
				}

				//fill in below ROI
				if (TempBottom)
				{
					int TempNumberOfLinesToFill = TempRightToFill - TempLeftToFill;
					for (WORD TempLineLoop = 0; TempLineLoop < TempNumberOfLinesToFill; TempLineLoop++)
					{
						BYTE *TempDestinationPointer = TempImage + ((TempLineLoop  + TempLeftToFill) * TempSizeX);
						if (TempDestinationPointer + TempBottom < TempMaximumImagePointer)
							FillMemory(TempDestinationPointer, TempBottom, 255);
					}
				}
				//fill in above ROI
				if (vROIPixelTop < TempSizeX - 1)
				{
					int TempNumberOfLinesToFill = TempRightToFill - TempLeftToFill;
					int TempNumberOfPixelsToFill = TempSizeX - vROIPixelTop - 1;
					for (WORD TempLineLoop = 0; TempLineLoop < TempNumberOfLinesToFill; TempLineLoop++)
					{
						BYTE *TempDestinationPointer = TempImage + ((TempLineLoop  + TempLeftToFill) * TempSizeX) + vROIPixelTop;
						if (TempDestinationPointer + TempNumberOfPixelsToFill >= TempMaximumImagePointer)
							TempNumberOfPixelsToFill = TempMaximumImagePointer - TempDestinationPointer - 1;
							FillMemory(TempDestinationPointer, TempNumberOfPixelsToFill, 255);
					}
				}
			}
		}
	}
}
/*
void CInspection::TrimEdgesOfJarAtROIOfRandomShape(BYTE *TempImage, WORD TempSizeX, WORD TempSizeY, bool TempSetBackgroundWhite, 
		WORD TempLeftSide, WORD TempRightSide)
{
	if (vEnable)
	{
		if (vROIPixelRight > TempSizeY)
			vROIPixelRight = TempSizeY - 1;
		if (vROIPixelLeft >= vROIPixelRight)
			vROIPixelLeft = 1;

		WORD TempBottom = vROIPixelBottom;
		WORD TempTop = vROIPixelTop;
		if (TempTop >= TempSizeX)
			TempTop = TempSizeX - 1;
		if (TempBottom >= vROIPixelTop)
			TempBottom = 1;
		WORD TempLeft = vROIPixelLeft;
		WORD TempRight = vROIPixelRight;
		if (TempRight > TempSizeY)
			TempRight = TempSizeY - 1;
		if (TempLeft >= TempRight)
			TempLeft = 1;

		WORD TempLeftToFill = TempLeft;
		WORD TempRightToFill = TempRight;

		//trim to the edge of the jar instead of the ROI if know the edge of the jar
		if (TempRightSide)
		{
			TempLeft = TempLeftSide;
			TempRight = TempRightSide;

			if (TempRight > TempSizeY)
				TempRight = TempSizeY - 1;
			if (TempLeft >= TempRight)
				TempLeft = 1;

			if (TempRightToFill > TempRight)
			if (TempRight > 20)
			if (TempRight < TempSizeY)
				TempRightToFill = TempRight;
			if (TempLeftToFill < TempLeft)
				TempLeftToFill = TempLeft;
		}

		WORD TempHeight = TempTop - TempBottom;
		if (TempHeight + TempBottom >= TempSizeX)
			TempHeight = TempSizeX - TempBottom - 1;

		WORD TempWidth = TempRight - TempLeft;
		if (TempWidth + TempLeft >= TempSizeY)
			TempWidth = TempSizeY - TempLeft - 1;

		WORD TempHalfWidth = TempWidth / 2;

		BYTE *TempMaximumImagePointer = TempImage + ((DWORD)TempSizeX * (DWORD)TempSizeY);

		if (TempImage)
		if (TempWidth > 10) 
		{
			//start at the bottom ROI line and trim every line up to the top ROI line
			for (WORD TempLineLoop = 0; TempLineLoop < TempHeight; TempLineLoop++)
			{
				BYTE *TempCurrentPixel = NULL;
				BYTE *TempNextPixel = NULL;
				bool TempFoundDarkeningArea = false;

				//scanning from left to center find left edge of data to curve fit
				TempCurrentPixel = TempImage + TempBottom + TempLineLoop + (TempLeft * TempSizeX);
				TempNextPixel = TempCurrentPixel + TempSizeX;
				TempFoundDarkeningArea = false;
				WORD TempLeftToCurveFit = 0;

				bool TempNeedToTrim = false;

				//start with white near edge so look and see if has dark edge
				//scan in until find first non saturated pixel
				while ((*TempCurrentPixel == 255) && (TempLeftToCurveFit < TempHalfWidth - 2))
				{
					if ((TempCurrentPixel >= TempImage) && (TempCurrentPixel < TempMaximumImagePointer))
						TempCurrentPixel = TempCurrentPixel + TempSizeX;
					TempLeftToCurveFit++;
				}

				//check next 5 pixels to see if another white spot, if so, move in 
				BYTE *TempWhiteSpotCheckPixel = TempCurrentPixel + TempSizeX;
				BYTE TempFoundAnotherWhiteSpot = 0;
				//scan in until find first non saturated pixel
				for (BYTE TempLoop = 0; TempLoop < 5; TempLoop++)  //scan the next 5 pixels for a dark edge of the jar
				if ((TempWhiteSpotCheckPixel >= TempImage) && (TempWhiteSpotCheckPixel < TempMaximumImagePointer))
				{
					if (*TempWhiteSpotCheckPixel == 255)
					{
						TempFoundAnotherWhiteSpot = TempLoop + 1;
						TempLoop = 15;
					}
					TempWhiteSpotCheckPixel = TempWhiteSpotCheckPixel + TempSizeX;
				}
				if (TempFoundAnotherWhiteSpot) //set pixels until white spot white
				{
					for (BYTE TempLoop = 0; TempLoop <= TempFoundAnotherWhiteSpot; TempLoop++)  //scan the next 5 pixels for a dark edge of the jar
					if ((TempCurrentPixel >= TempImage) && (TempCurrentPixel < TempMaximumImagePointer))
					{
						*TempCurrentPixel = 255;
						TempLeftToCurveFit++;
						TempCurrentPixel = TempCurrentPixel + TempSizeX;
					}
					//scan in until find first non saturated pixel
					while ((*TempCurrentPixel == 255) && (TempLeftToCurveFit < TempHalfWidth - 2))
					{
						TempLeftToCurveFit++;
						if ((TempCurrentPixel >= TempImage) && (TempCurrentPixel < TempMaximumImagePointer))
							TempCurrentPixel = TempCurrentPixel + TempSizeX;
					}
				}
				for (WORD TempPixelLoop = 0; TempPixelLoop < 2; TempPixelLoop++) //set area that is darkening to white
				if ((TempCurrentPixel >= TempImage) && (TempCurrentPixel < TempMaximumImagePointer))
				{
					*TempCurrentPixel = 255; //set current pixel white
					TempCurrentPixel = TempCurrentPixel + TempSizeX;
				}

				//scanning from Right to center find Right edge of data to curve fit
				TempCurrentPixel = TempImage + TempBottom + TempLineLoop + ((TempRight) * TempSizeX);
				WORD TempRightToCurveFit = TempRight;  // ??? am I off by one?

				TempNeedToTrim = false;

				//start with white near edge so look and see if has dark edge
				//scan in until find first non saturated pixel
				while ((*TempCurrentPixel == 255) && (TempRightToCurveFit > TempHalfWidth + 2))
				{
					if ((TempCurrentPixel >= TempImage) && (TempCurrentPixel < TempMaximumImagePointer))
						TempCurrentPixel = TempCurrentPixel - TempSizeX;
					TempRightToCurveFit--;
				}

				//check next 15 pixels to see if another white spot, if so, move in 
				TempWhiteSpotCheckPixel = TempCurrentPixel - TempSizeX;
				TempFoundAnotherWhiteSpot = 0;
				//scan in until find first non saturated pixel
				for (TempLoop = 0; TempLoop < 15; TempLoop++)  //scan the next 5 pixels for a dark edge of the jar
				{
					if (*TempWhiteSpotCheckPixel == 255)
					{
						TempFoundAnotherWhiteSpot = TempLoop + 1;
						TempLoop = 15;
					}
					TempWhiteSpotCheckPixel = TempWhiteSpotCheckPixel - TempSizeX;
				}
				if (TempFoundAnotherWhiteSpot) //set pixels until white spot white
				{
					for (BYTE TempLoop = 0; TempLoop <= TempFoundAnotherWhiteSpot; TempLoop++)  //scan the next 5 pixels for a dark edge of the jar
					if ((TempCurrentPixel >= TempImage) && (TempCurrentPixel < TempMaximumImagePointer))
					{
						*TempCurrentPixel = 255;
						TempRightToCurveFit--;
						TempCurrentPixel = TempCurrentPixel - TempSizeX;
					}
					//scan in until find first non saturated pixel
					while ((*TempCurrentPixel == 255) && (TempRightToCurveFit > TempHalfWidth + 2))
					{
						TempRightToCurveFit--;
						if ((TempCurrentPixel >= TempImage) && (TempCurrentPixel < TempMaximumImagePointer))
							TempCurrentPixel = TempCurrentPixel - TempSizeX;
					}
				}
				for (TempPixelLoop = 0; TempPixelLoop < 2; TempPixelLoop++) //set area that is darkening to white
				if ((TempCurrentPixel >= TempImage) && (TempCurrentPixel < TempMaximumImagePointer))
				{
					*TempCurrentPixel = 255; //set current pixel white
					TempCurrentPixel = TempCurrentPixel - TempSizeX;
				}
			}
			if (TempSetBackgroundWhite)
			{
				//fill white to the small area either the ROI or the edge of the jar
				BYTE *TempImagePointer = NULL;
				//fill in to left of ROI
				if (TempLeftToFill)
				{
					DWORD TempNumberOfPixelsToFill = TempLeftToFill * TempSizeX;
					if (TempImage + TempNumberOfPixelsToFill < TempMaximumImagePointer)
						FillMemory(TempImage, TempNumberOfPixelsToFill, 255);
				}

				//fill in to right of ROI
				if (TempRightToFill < TempSizeY)
				{
					int TempNumberOfLinesToFill = TempSizeY - TempRightToFill;
					DWORD TempNumberOfPixelsToFill = TempNumberOfLinesToFill * TempSizeX;
					BYTE *TempDestinationPointer = TempImage + (TempRightToFill * TempSizeX);
					if (TempDestinationPointer + TempNumberOfPixelsToFill >= TempMaximumImagePointer)
						TempNumberOfPixelsToFill = TempMaximumImagePointer - TempDestinationPointer - 1;
					FillMemory(TempDestinationPointer, TempNumberOfPixelsToFill, 255);
				}

				//fill in below ROI
				if (TempBottom)
				{
					int TempNumberOfLinesToFill = TempRightToFill - TempLeftToFill;
					for (WORD TempLineLoop = 0; TempLineLoop < TempNumberOfLinesToFill; TempLineLoop++)
					{
						BYTE *TempDestinationPointer = TempImage + ((TempLineLoop  + TempLeftToFill) * TempSizeX);
						if (TempDestinationPointer + TempBottom < TempMaximumImagePointer)
							FillMemory(TempDestinationPointer, TempBottom, 255);
					}
				}
				//fill in above ROI
				if (vROIPixelTop < TempSizeX - 1)
				{
					int TempNumberOfLinesToFill = TempRightToFill - TempLeftToFill;
					int TempNumberOfPixelsToFill = TempSizeX - vROIPixelTop - 1;
					for (WORD TempLineLoop = 0; TempLineLoop < TempNumberOfLinesToFill; TempLineLoop++)
					{
						BYTE *TempDestinationPointer = TempImage + ((TempLineLoop  + TempLeftToFill) * TempSizeX) + vROIPixelTop;
						if (TempDestinationPointer + TempNumberOfPixelsToFill >= TempMaximumImagePointer)
							TempNumberOfPixelsToFill = TempMaximumImagePointer - TempDestinationPointer - 1;
							FillMemory(TempDestinationPointer, TempNumberOfPixelsToFill, 255);
					}
				}
			}
		}
	}
}
*/
void CInspection::RemoveJarPunt(BYTE *TempImage, WORD TempSizeX, WORD TempSizeY, WORD TempLeftSide, WORD TempRightSide, 
																double TempImageBrightness, BYTE TempJustMarkPunt)
{
	if (vEnable)
	{
		BYTE *TempPointer = TempImage;
		//4/11/2011  Setting value to 2 seems to reduce odd false rejects
		for (int TempLoop = 0; TempLoop < TempSizeY; TempLoop++)
		{
			*TempPointer = 2;
			TempPointer = TempPointer + TempSizeX;
		}

		for (int TempLineLoop = 0; TempLineLoop < cMaximumProfileSize; TempLineLoop++)
			vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = 255;

		//BYTE TempThreshold = (BYTE)(TempImageBrightness * 3 / 4 );//* 5 / 6); was - 25
		BYTE TempThreshold = (BYTE)(TempImageBrightness / 1.75); //was 2.  Found 1.75 worked best for DFA Starbucks bottles  1.2 for spice bot
		BYTE TempHighThreshold = (BYTE)(TempImageBrightness - 10);

		//DWORD TempSize = TempSizeX * TempSizeY;
		BYTE *TempMaxImage = TempImage + ((DWORD)TempSizeX * (DWORD)TempSizeY);

		WORD TempBottom = 1;
		WORD TempLeft = TempLeftSide;
		WORD TempRight = TempRightSide;
		if (TempRight > TempSizeY)
			TempRight = TempSizeY;
		if (TempLeft >= TempRight)
			TempLeft = 0;
		if (TempLeft)
			TempLeft--;

		WORD TempTop = vROIPixelTop;
		if (TempTop > 30)
			TempTop = 30;

		WORD TempNumberOfLinesInROI = TempRight - TempLeft;
		if (TempNumberOfLinesInROI + TempLeft > TempSizeY)
			TempNumberOfLinesInROI = TempSizeY - TempLeft;

		WORD TempMaximumPuntHeight = 0;
		WORD TempMinimumPuntHeight = 255;

		//scan up each line from the left to right out to find min and max punt height and copy punt height into array
		FillMemory(vRecentPuntRemovalPuntHeightProfile, cMaximumProfileSize, 255);
		BYTE TempCurrentPuntHeight = 50;
		//if (TempCurrentPuntHeight > vROIPixelTop)
		//	TempCurrentPuntHeight = vROIPixelTop - 2;
		BYTE TempMaximumPuntCheck = TempCurrentPuntHeight - 10;

		if (TempNumberOfLinesInROI > cMaximumProfileSize)
			TempNumberOfLinesInROI = cMaximumProfileSize;

		int TempCenter = TempNumberOfLinesInROI / 2;

		BYTE TempUpperThreshold = (BYTE)(TempImageBrightness + 20);
			if (TempImageBrightness > 210)
				TempUpperThreshold = (BYTE)TempImageBrightness;

		bool TempFoundPunt = false;
		//find punt for each line from center to Left
		for (int TempLineLoop = TempCenter; TempLineLoop > 0; TempLineLoop--)
		{
			BYTE *TempImagePointer = TempImage + ((TempLeft + TempLineLoop) * TempSizeX);

			//find a threshold for this line as the line brighnesses increase going to the edges
			BYTE TempLineThreshold = TempThreshold;
			BYTE TempMinValue = 255;
			BYTE TempMaxValue = 0;
			BYTE *TempPixelPointer = TempImage + TempBottom + ((TempLeft + TempLineLoop) * TempSizeX);
			for (int TempPixelLoop = TempBottom; TempPixelLoop < TempTop; TempPixelLoop++)
			{
				if (*TempPixelPointer < 255)
				{
					if (TempMaxValue < *TempPixelPointer)
						TempMaxValue = *TempPixelPointer;
					if (TempMinValue > *TempPixelPointer)
						TempMinValue = *TempPixelPointer;
				}
				TempPixelPointer++;
			}
			BYTE TempMidLineThreshold = TempThreshold;
			if (TempMinValue < TempMaxValue)
			{
				TempLineThreshold =  TempMinValue + (BYTE)((TempMaxValue - TempMinValue) * .95);

				if (TempLineThreshold > TempImageBrightness + 20)
					TempLineThreshold = (BYTE)(TempImageBrightness + 20);

				TempMidLineThreshold =  TempMinValue + (BYTE)((TempMaxValue - TempMinValue) * .5);

				if (TempMidLineThreshold > TempImageBrightness - 20)
					TempMidLineThreshold = (BYTE)(TempImageBrightness - 20);
			}

			//Scan from bottom of line up to find the punt on left half of image
			//if (TempFoundPunt)
				vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempCurrentPuntHeight;
			TempFoundPunt = false;
			for (int TempPixelLoop = TempBottom; TempPixelLoop <= TempCurrentPuntHeight; TempPixelLoop++)
			{
				if ((*TempImagePointer > 1) && (*TempImagePointer < TempMidLineThreshold)) //found the punt
					TempFoundPunt = true;

				if (TempFoundPunt)
				if ((TempImagePointer < TempMaxImage) && (TempImagePointer >= TempImage))  //if pointer valid
				{
					if ((*TempImagePointer > 230) && (TempPixelLoop < 5)) //found gap under punt, so mark as not found punt yet
						TempFoundPunt = false;
					else
					if (*TempImagePointer < TempMidLineThreshold)
						vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempPixelLoop;
					else
					if ((*TempImagePointer > 230) && (TempPixelLoop > 5)) //exit loop if real bright above line 5
						TempPixelLoop = TempCurrentPuntHeight;
					else
					{
						BYTE *TempPixelAbove = TempImagePointer + 1;
						if ((*TempImagePointer < TempLineThreshold) && (*TempImagePointer + 20 < *TempPixelAbove))
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempPixelLoop;
					}
				}
				TempImagePointer++;
			}
			TempCurrentPuntHeight = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
			if (TempCurrentPuntHeight < TempMinimumPuntHeight)
				TempMinimumPuntHeight = TempCurrentPuntHeight;
			if (TempCurrentPuntHeight < 50)
			if (TempCurrentPuntHeight > TempMaximumPuntHeight)
				TempMaximumPuntHeight = TempCurrentPuntHeight;
		}
		TempCurrentPuntHeight = 50;
		TempFoundPunt = false;
		//find punt for each line from center to right
		for (DWORD TempLineLoop = (TempCenter) + 1; TempLineLoop < TempNumberOfLinesInROI; TempLineLoop++)
		{
			BYTE *TempImagePointer = TempImage + ((TempLeft + TempLineLoop) * TempSizeX);

			//find a threshold for this line as the line brighnesses increase going to the edges
			BYTE TempLineThreshold = TempThreshold;
			BYTE TempMinValue = 255;
			BYTE TempMaxValue = 0;
			BYTE *TempPixelPointer = TempImage + TempBottom + ((TempLeft + TempLineLoop) * TempSizeX);
			for (int TempPixelLoop = TempBottom; TempPixelLoop < TempTop; TempPixelLoop++)
			{
				if (*TempPixelPointer < 255)
				{
					if (TempMaxValue < *TempPixelPointer)
						TempMaxValue = *TempPixelPointer;
					if (TempMinValue > *TempPixelPointer)
						TempMinValue = *TempPixelPointer;
				}
				TempPixelPointer++;
			}
			BYTE TempMidLineThreshold = TempThreshold;
			if (TempMinValue < TempMaxValue)
			{
				TempLineThreshold =  TempMinValue + (BYTE)((TempMaxValue - TempMinValue) * .95);

				if (TempLineThreshold > TempImageBrightness + 20)
					TempLineThreshold = (BYTE)(TempImageBrightness + 20);

				TempMidLineThreshold =  TempMinValue + (BYTE)((TempMaxValue - TempMinValue) * .5);

				if (TempMidLineThreshold > TempImageBrightness - 20)
					TempMidLineThreshold = (BYTE)(TempImageBrightness - 20);
			}

			//Scan from bottom of line up to find the punt on right half of image
			//if (TempFoundPunt)
				vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempCurrentPuntHeight;
			TempFoundPunt = false;
			for (int TempPixelLoop = TempBottom; TempPixelLoop <= TempCurrentPuntHeight; TempPixelLoop++)
			{
				if ((*TempImagePointer > 1) && (*TempImagePointer < TempMidLineThreshold)) //found the punt
					TempFoundPunt = true;

				if (TempFoundPunt)
				if ((TempImagePointer < TempMaxImage) && (TempImagePointer >= TempImage))
				{
					if ((*TempImagePointer > 230) && (TempPixelLoop < 5)) //found gap under punt, so mark as not found punt yet
						TempFoundPunt = false;
					else
					if (*TempImagePointer < TempMidLineThreshold)
						vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempPixelLoop;
					else
					if ((*TempImagePointer > 230) && (TempPixelLoop > 5)) //exit loop if real bright above line 5
						TempPixelLoop = TempCurrentPuntHeight;
					else
					{
						BYTE *TempPixelAbove = TempImagePointer + 1;
						if ((*TempImagePointer < TempLineThreshold) && (*TempImagePointer + 20 < *TempPixelAbove))
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempPixelLoop;
					}
				}
				TempImagePointer++;
			}
			TempCurrentPuntHeight = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
			if (TempCurrentPuntHeight < TempMinimumPuntHeight)
				TempMinimumPuntHeight = TempCurrentPuntHeight;
			if (TempCurrentPuntHeight < 50)
			if (TempCurrentPuntHeight > TempMaximumPuntHeight)
				TempMaximumPuntHeight = TempCurrentPuntHeight;
		}

		if (TempMinimumPuntHeight < TempBottom)
			TempMinimumPuntHeight = TempBottom;

		int TempLeftMostValue = 255;
		int TempRightMostValue = 255;
		int TempLeftMaximumIndex = 0;  //is array index of left curve fit edge
		int TempRightMaximumIndex = TempRight;//is array index of right curve fit edge
		if (TempMinimumPuntHeight <= TempMaximumPuntHeight)  //found have a punt that needs eliminating
		if (TempMaximumPuntHeight > TempBottom)
		if (TempMaximumPuntHeight < 50)
		{
//should not need
	/*
			if (!vAddStructure)
			if ((!TempJustMarkPunt) || (TempJustMarkPunt > 2))
			{
				if (TempRight > cMaximumProfileSize - 4)
					TempRight = cMaximumProfileSize - 4;

				//clip any bump up or down of one to 4 pixels
				for (TempLineLoop = 0; TempLineLoop < TempRight - 3; TempLineLoop++)
				{
					if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
					{
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2])
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 3])
						{
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
						}
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 4])
						{
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 3] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
						}
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 5])
						{
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 3] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 4] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
						}
					}
				}
			}
			*/
			//find left curve height and place
			if (TempCenter > cMaximumProfileSize - 1)
				TempCenter = cMaximumProfileSize - 1;

			TempLeftMostValue = 255;
			for (int TempLineLoop = TempCenter; TempLineLoop > 1; TempLineLoop--)
			{
				//if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop] + 3 < TempLeftMostValue) && (TempLeftMostValue < 255))
				//	TempLineLoop = 0;
				//else
				if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < TempLeftMostValue) && 
					(vRecentPuntRemovalPuntHeightProfile[TempLineLoop - 1] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop]))// &&
					//(vRecentPuntRemovalPuntHeightProfile[TempLineLoop - 2] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop]))
				{
					TempLeftMostValue = vRecentPuntRemovalPuntHeightProfile[TempLineLoop]; // ? + 1
					TempLeftMaximumIndex = TempLineLoop;
				}
			}

			//find right curve height and place
			if (TempNumberOfLinesInROI > cMaximumProfileSize - 1)
				TempNumberOfLinesInROI = cMaximumProfileSize - 1;
			TempRightMostValue = 255;
			for (int TempLineLoop = TempCenter; TempLineLoop < TempNumberOfLinesInROI - 1; TempLineLoop++)
			{
				//if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop] + 3 < TempRightMostValue) && (TempRightMostValue < 255))
				//	TempLineLoop = TempNumberOfLinesInROI;
				//else
				if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < TempRightMostValue) && 
					(vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop]))// &&
					//(vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop]))
				{
					TempRightMostValue = vRecentPuntRemovalPuntHeightProfile[TempLineLoop]; // ? + 1
					TempRightMaximumIndex = TempLineLoop;
				}
			}


			//eliminate data beyond left edge of curve fit area
			//for (TempLineLoop = 0; TempLineLoop < TempLeftMaximumIndex; TempLineLoop++)
			//{
			//	vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = 255;
			//}

			//eliminate data beyond Right edge of curve fit area
			//for (TempLineLoop = TempNumberOfLinesInROI - 1; TempLineLoop > TempRightMaximumIndex; TempLineLoop--)
			//{
			//	vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = 255;
			//}

			WORD TempNumberOfLinesCurveFit = TempRightMaximumIndex - TempLeftMaximumIndex + 1;
			if (TempNumberOfLinesCurveFit + TempLeftMaximumIndex > TempSizeY)
				TempNumberOfLinesCurveFit = TempSizeY - TempLeftMaximumIndex;
			int TempCenterIndex = TempLeftMaximumIndex + ((TempRightMaximumIndex - TempLeftMaximumIndex) / 2);

			BYTE *TempBottomProfileArray = NULL;
			TempBottomProfileArray = (BYTE *)malloc(TempNumberOfLinesCurveFit);
			//BYTE *TempBottomProfileArrayPointer = TempBottomProfileArray;
			if (TempBottomProfileArray)
			{
				FillMemory(TempBottomProfileArray, TempNumberOfLinesCurveFit, 255);
//should not need
				//trim any rise after well on left side using left curve height
				if ((!TempJustMarkPunt) || (TempJustMarkPunt > 1))
				{
					if (TempLeftMaximumIndex > cMaximumProfileSize)
						TempLeftMaximumIndex = cMaximumProfileSize;
/* test should not need
					for (TempLineLoop = 0; TempLineLoop < TempLeftMaximumIndex; TempLineLoop++)
					{
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > TempLeftMostValue)
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempLeftMostValue;
					}

					if (TempNumberOfLinesInROI > cMaximumProfileSize)
						TempNumberOfLinesInROI = cMaximumProfileSize;
					//trim any rise after well on left side using right curve height
					for (TempLineLoop = TempRightMaximumIndex + 1; TempLineLoop < TempNumberOfLinesInROI; TempLineLoop++)
					{
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > TempRightMostValue)
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempRightMostValue;
					}
//should not need
					//trim any large bumps on the left side
					if (TempCenterIndex > cMaximumProfileSize - 2)
						TempCenterIndex = cMaximumProfileSize - 2;

					TempCurrentPuntHeight = vRecentPuntRemovalPuntHeightProfile[TempCenterIndex];
					for (TempLineLoop = TempCenterIndex + 1; TempLineLoop > 0; TempLineLoop--)
					{
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
						{
							// ?? limit to go up one pixel every 5 lines?
							if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > TempCurrentPuntHeight + 3)
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempCurrentPuntHeight;
							else
								TempCurrentPuntHeight = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
						}
					}

					//trim any large bumps on the right side
					if (TempRight > cMaximumProfileSize - 1)
						TempRight = cMaximumProfileSize - 1;
					TempCurrentPuntHeight = vRecentPuntRemovalPuntHeightProfile[TempCenterIndex];
					for (TempLineLoop =  TempCenterIndex- 1; TempLineLoop >= TempRight; TempLineLoop++)
					{
						if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop]) && (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255))
						{
							// ?? limit to go up one pixel every 5 lines?
							if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > TempCurrentPuntHeight + 3)
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempCurrentPuntHeight;
							else
								TempCurrentPuntHeight = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
						}
					}
					*/
					//clear any value past first 255 going center to left
					bool TempFound255 = false;
					for (int TempLineLoop = TempCenterIndex; TempLineLoop > 0; TempLineLoop--)
					{
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
						{
							if (TempFound255)
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = 255;
						}
						else
							TempFound255 = true;
					}

					//clear any value past first 255 going center to right
					if (TempRightMaximumIndex > cMaximumProfileSize)
						TempRightMaximumIndex = cMaximumProfileSize;
					TempFound255 = false;
					for (int TempLineLoop =  TempCenterIndex; TempLineLoop < TempRightMaximumIndex; TempLineLoop++)
					{
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
						{
							if (TempFound255)
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = 255;
						}
						else
							TempFound255 = true;
					}
				}

				//int TempCenterIndex = TempLeftMaximumIndex + ((TempRightMaximumIndex - TempLeftMaximumIndex) / 2);
				if ((!TempJustMarkPunt) || (TempJustMarkPunt > 2))
				{
					BYTE *TempBottomProfileArrayPointer = TempBottomProfileArray;
					for (int TempLineLoop = TempLeftMaximumIndex; TempLineLoop < TempRightMaximumIndex; TempLineLoop++)
						*TempBottomProfileArrayPointer++ = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
//curve fit punt
					ReturnCurveFitLine(TempBottomProfileArray, TempNumberOfLinesCurveFit, NULL, 4);  // use 4 terms to fit punt for swung Baffles

					// test
					//TempBottomProfileArrayPointer = TempBottomProfileArray;
					//for (int TempLineLoop = TempLeftMaximumIndex; TempLineLoop < TempRightMaximumIndex; TempLineLoop++)
					//	vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = *TempBottomProfileArrayPointer++;


					vRecentPuntRemovalBottomHeight = 0;
					vRecentPuntRemovalNumberOfPixelsCopied = 0;

					//If curve fit lower than threshold use it, raise one pixel if needed, find maximum punt height
					//check left side for curve fit being lower than original profile and move down if so
					if (vAddStructure)
						vSwungBaffleTolerance = 0;
					TempBottomProfileArrayPointer = TempBottomProfileArray;
					TempMaximumPuntHeight = 0;
					bool TempClipping = false;
					for (int TempLineLoop = TempLeftMaximumIndex; TempLineLoop < TempCenterIndex; TempLineLoop++)
					{
						//clip top of array if goes above curve
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
						{
							//if had a contaminant on the bottom, use the curve fit to trim it off
							if (*TempBottomProfileArrayPointer < 255)
							if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > *TempBottomProfileArrayPointer + vSwungBaffleTolerance)
							{
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = *TempBottomProfileArrayPointer;//use curve fit height
								TempClipping = true;
							}
							else
							if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > *TempBottomProfileArrayPointer) && (TempClipping))
							{
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = *TempBottomProfileArrayPointer;//use curve fit height
							}
							else
							{  //if curve fit not less than original threshold value
								TempClipping = false;
							}
							//record current height to compare next pixel

							if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > TempMaximumPuntHeight) //find new maximum punt height
								TempMaximumPuntHeight = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];

						}
						TempBottomProfileArrayPointer++;

					}
					//check right side for curve fit being lower than original profile and move down if so
					TempBottomProfileArrayPointer = TempBottomProfileArray + TempNumberOfLinesCurveFit - 1;
					TempClipping = false;
					for (int TempLineLoop = TempRightMaximumIndex; TempLineLoop >= TempCenterIndex; TempLineLoop--)
					{
						//clip top of array if goes above curve
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
						{
							//if had a contaminant on the bottom, use the curve fit to trim it off
							if (*TempBottomProfileArrayPointer < 255)
							if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > *TempBottomProfileArrayPointer + vSwungBaffleTolerance)
							{
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = *TempBottomProfileArrayPointer;
								TempClipping = true;
							}
							else
							if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > *TempBottomProfileArrayPointer) && (TempClipping))
							{
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = *TempBottomProfileArrayPointer;
							}
							else
							{
								TempClipping = false;
							}

							if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > TempMaximumPuntHeight) //find new maximum punt height
								TempMaximumPuntHeight = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];

						}
						TempBottomProfileArrayPointer--;

					}
					/* test remove
					if (!vAddStructure)
					if ((!TempJustMarkPunt) || (TempJustMarkPunt > 2))
					{
						//clip any bump up or down of one to 4 pixels
						for (TempLineLoop = 0; TempLineLoop < TempRight - 3; TempLineLoop++)
						{
							if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
							{
								if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 5])
								{
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 3] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 4] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
								}
								else
								if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 4])
								{
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 3] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
								}
								else
								if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 3])
								{
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
								}
								else
								if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2])
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
								else
								//if next pixel is more than one down, and pixel after is only one down, set next pixel to this one
								if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] + 1)
								if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2] + 1)
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
								else
								//if next pixel is more than one up, and pixel after is only one up, set next pixel to the one after it
								if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] + 1 < vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1])
								if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] + 1 == vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2])
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2];
							}
						}
					}
					*/
				}

				if ((!TempJustMarkPunt) || (TempJustMarkPunt > 2))
				{
					//clear any value past first 255 going center to Left
					bool TempFound255 = false;
					for (int TempLineLoop = TempCenterIndex; TempLineLoop > 0; TempLineLoop--)
					{
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
						{
							if (TempFound255)
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = 255;
						}
						else
							TempFound255 = true;
					}

					//clear any value past first 255 going center to Right
					TempFound255 = false;
					for (int TempLineLoop =  TempCenterIndex; TempLineLoop < TempRightMaximumIndex; TempLineLoop++)
					{
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
						{
							if (TempFound255)
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = 255;
						}
						else
							TempFound255 = true;
					}
					//add one more to height of punt if next pixel is dark, but one above is light
					if (!vAddStructure) 
					if ((!TempJustMarkPunt) || (TempJustMarkPunt > 4))
					for (int TempLineLoop = 0; TempLineLoop < TempNumberOfLinesInROI; TempLineLoop++)
					{
						BYTE *TempImagePointer = TempImage + vRecentPuntRemovalPuntHeightProfile[TempLineLoop] + ((TempLeft + TempLineLoop) * TempSizeX);
						BYTE *TempPixelAbove = TempImagePointer;
						BYTE *TempPixel2Above = TempPixelAbove + 1;

						if ((TempPixel2Above < TempMaxImage) && (TempImagePointer >= TempImage))
						{
							//add one if pixel above is dark, but not one above that
							if ((*TempPixelAbove < TempImageBrightness - 15) && (*TempPixel2Above > TempImageBrightness + 15))
							{
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop]++;  //increase the height one as think it is just to top of the punt
							}
						}
					}
					

					//find left curve height
					TempLeftMostValue = 255;
					for (int TempLineLoop = TempCenter; TempLineLoop > 1; TempLineLoop--)
					{
						//if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop] + 3 < TempLeftMostValue) && (TempLeftMostValue < 255))
						//	TempLineLoop = 0;
						//else
						if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < TempLeftMostValue) && 
							(vRecentPuntRemovalPuntHeightProfile[TempLineLoop - 1] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop]))// &&
							//(vRecentPuntRemovalPuntHeightProfile[TempLineLoop - 2] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop]))
						{
							TempLeftMostValue = vRecentPuntRemovalPuntHeightProfile[TempLineLoop]; // ? + 1
							TempLeftMaximumIndex = TempLineLoop;
						}
					}

					//find right curve height
					TempRightMostValue = 255;
					for (int TempLineLoop = TempCenter; TempLineLoop < TempNumberOfLinesInROI - 1; TempLineLoop++)
					{
						//if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop] + 3 < TempRightMostValue) && (TempRightMostValue < 255))
						//	TempLineLoop = TempNumberOfLinesInROI;
						//else
						if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < TempRightMostValue) && 
							(vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop]))// &&
							//(vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop]))
						{
							TempRightMostValue = vRecentPuntRemovalPuntHeightProfile[TempLineLoop]; // ? + 1
							TempRightMaximumIndex = TempLineLoop;
						}
					}
				}
/* should not need
				//trim any rise after well on left side using left curve height
				if ((!TempJustMarkPunt) || (TempJustMarkPunt > 2))
				{
					//do left side
					for (TempLineLoop = 0; TempLineLoop < TempLeftMaximumIndex; TempLineLoop++)
					{
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
						{
							//if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > TempLeftMostValue)
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempLeftMostValue;
							//else
							//	TempLineLoop = TempCenterIndex;
						}
						//else
						//	vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempLeftMostValue;
					}
					//do right side
					for (TempLineLoop = TempNumberOfLinesInROI - 1; TempLineLoop > TempRightMaximumIndex; TempLineLoop--)
					{
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
						{
							//if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > TempRightMostValue)
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempRightMostValue;
							//else
							//	TempLineLoop = TempCenterIndex;
						}
						//else
						//	vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempRightMostValue;
					}
				}
				*/

				//put the image height of the maximum height copied pixel
				vRecentPuntRemovalBottomHeight = TempRightMostValue;
				if (vRecentPuntRemovalBottomHeight > TempLeftMostValue)
					vRecentPuntRemovalBottomHeight = TempLeftMostValue;

				vRecentPuntRemovalNumberOfPixelsCopied = (BYTE)TempMaximumPuntHeight - vRecentPuntRemovalBottomHeight + 6;

				//now know min and max punt height, if found correctly, move pixels down appropriate amount
				//scan lines from left maximum to right maximum, find punt height, if taller than minimum, copy pixels to cover punt
				if (TempJustMarkPunt)
				{
					for (int TempLineLoop = 0; TempLineLoop < TempNumberOfLinesInROI; TempLineLoop++)
					{
						WORD TempCurrentPuntHeight = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
						if (TempCurrentPuntHeight > vRecentPuntRemovalBottomHeight)
						if (TempCurrentPuntHeight < 255)
						//punt is higher than minimum, so copy pixels down to cover the punt
						{
							int TempAmountPuntTooTallBy = TempCurrentPuntHeight - vRecentPuntRemovalBottomHeight;
							BYTE *TempDestinationPixel = TempImage + vRecentPuntRemovalBottomHeight + ((TempLeft + TempLineLoop) * TempSizeX);

							for (int TempCopyPixelLoop = 0; TempCopyPixelLoop < TempAmountPuntTooTallBy; TempCopyPixelLoop++)
							{
								if (TempDestinationPixel < TempMaxImage)
									*TempDestinationPixel++ = 200;
								//else
								//	ReportErrorMessage("RemoveJarPunt Pointer Error 3", cEMailInspx, 32000);
							}
						}
					}
					//if have area in punt below product area turn it white so it is not processed
					if (vRecentPuntRemovalBottomHeight > TempBottom)
					{
						int TempAmountBottomOfPuntAboveBottomROI = vRecentPuntRemovalBottomHeight - TempBottom;
						for (int TempLineLoop = TempLeft; TempLineLoop < TempRight; TempLineLoop++)
						{
							BYTE *TempDestinationPixel = TempImage + TempBottom + (TempLineLoop * TempSizeX);
							for (int TempCopyPixelLoop = 0; TempCopyPixelLoop < TempAmountBottomOfPuntAboveBottomROI; TempCopyPixelLoop++)
							{
								if (TempDestinationPixel < TempMaxImage)
									*TempDestinationPixel++ = 200;
								//else
								//	ReportErrorMessage("RemoveJarPunt Pointer Error 4", cEMailInspx, 32000);
							}
						}
					}
				}
				else
				{  //move pixels above punt down to hide punt
					vRecentPuntRemovalNumberOfPixelsCopied = (BYTE)TempMaximumPuntHeight + 5; //is from - 1 + 6;
					/*
					if (vRecentPuntRemovalBottomHeight > TempBottom)
					{
						int TempAmountBottomOfPuntAboveBottomROI = vRecentPuntRemovalBottomHeight - TempBottom;
						for (int TempLineLoop = TempLeft; TempLineLoop < TempRight; TempLineLoop++)
						{
							BYTE *TempDestinationPixel = TempImage + TempBottom + (TempLineLoop * TempSizeX);
							for (int TempCopyPixelLoop = 0; TempCopyPixelLoop < TempAmountBottomOfPuntAboveBottomROI; TempCopyPixelLoop++)
							{
								if (TempDestinationPixel < TempMaxImage)
									*TempDestinationPixel++ = 255;
								//else
								//	ReportErrorMessage("RemoveJarPunt Pointer Error 5", cEMailInspx, 32000);
							}
						}
					}
					*/
					WORD TempCurrentPuntHeight = vRecentPuntRemovalBottomHeight;
					for (int TempLineLoop = 0; TempLineLoop < TempNumberOfLinesInROI; TempLineLoop++)
					{
						TempCurrentPuntHeight = vRecentPuntRemovalBottomHeight;
						//if (TempCurrentPuntHeight > vRecentPuntRemovalBottomHeight)
						//punt is higher than minimum, so copy pixels down to cover the punt
						if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > 1) && (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255))
							TempCurrentPuntHeight = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
						//{
							int TempAmountPuntTooTallBy = TempCurrentPuntHeight - vRecentPuntRemovalBottomHeight - 1;
							//BYTE *TempDestinationPixel = TempImage + vRecentPuntRemovalBottomHeight + ((TempLeft + TempLineLoop) * TempSizeX);
							//copy punt to one pixel up from bottom of image
							BYTE *TempDestinationPixel = TempImage + 1 + ((TempLeft + TempLineLoop) * TempSizeX);

							BYTE *TempSourcePixel = TempDestinationPixel + TempAmountPuntTooTallBy + vRecentPuntRemovalBottomHeight;
							//copy pixels down to cover punt
							for (int TempCopyPixelLoop = 0; TempCopyPixelLoop < vRecentPuntRemovalNumberOfPixelsCopied; TempCopyPixelLoop++)
							{
								if ((TempDestinationPixel < TempMaxImage) && (TempSourcePixel < TempMaxImage))
									*TempDestinationPixel++ = *TempSourcePixel++;
								//else
								//	ReportErrorMessage("RemoveJarPunt Pointer Error 6", cEMailInspx, 32000);
							}
						//}
					}
				}
				if (TempBottomProfileArray)
					free(TempBottomProfileArray);
			}
		}
	}
}

/*
void CInspection::RemoveJarPunt(BYTE *TempImage, WORD TempSizeX, WORD TempSizeY, WORD TempLeftSide, WORD TempRightSide, 
																double TempImageBrightness, BYTE TempJustMarkPunt)
{
	if (vEnable)
	if (!vFindSpotInSubROI)
	{
		for (int TempLineLoop = 0; TempLineLoop < 512; TempLineLoop++)
			vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = 255;

		//BYTE TempThreshold = (BYTE)(TempImageBrightness * 3 / 4 );//* 5 / 6); was - 25
		BYTE TempThreshold = (BYTE)(TempImageBrightness / 1.75); //was 2.  Found 1.75 worked best for DFA Starbucks bottles
		BYTE TempHighThreshold = (BYTE)(TempImageBrightness - 10);

		//DWORD TempSize = TempSizeX * TempSizeY;
		BYTE *TempMaxImage = TempImage + ((DWORD)TempSizeX * (DWORD)TempSizeY);

		WORD TempBottom = 1;
		WORD TempLeft = TempLeftSide;
		WORD TempRight = TempRightSide;
		if (TempRight > TempSizeY)
			TempRight = TempSizeY;
		if (TempLeft >= TempRight)
			TempLeft = 0;
		if (TempLeft)
			TempLeft--;
		WORD TempNumberOfLinesInROI = TempRight - TempLeft;
		if (TempNumberOfLinesInROI + TempLeft > TempSizeY)
			TempNumberOfLinesInROI = TempSizeY - TempLeft;

		WORD TempMaximumPuntHeight = 0;
		WORD TempMinimumPuntHeight = 255;

		//scan down each line from the left to right out to find min and max punt height and copy punt height into array
		FillMemory(vRecentPuntRemovalPuntHeightProfile, 512,255);
		BYTE TempCurrentPuntHeight = 50;
		//if (TempCurrentPuntHeight > vROIPixelTop)
		//	TempCurrentPuntHeight = vROIPixelTop - 2;
		BYTE TempMaximumPuntCheck = TempCurrentPuntHeight - 10;
		//for (int TempLineLoop = 0; TempLineLoop < TempNumberOfLinesCurveFit; TempLineLoop++)
		for (TempLineLoop = 0; TempLineLoop < TempNumberOfLinesInROI; TempLineLoop++)
		{
			BYTE TempBackupPuntHeight = 0;
			if (TempCurrentPuntHeight < TempMaximumPuntCheck)
				TempCurrentPuntHeight = TempCurrentPuntHeight + 8;
			else
				TempCurrentPuntHeight = TempMaximumPuntCheck + 8;
			BYTE *TempImagePointer = TempImage + TempCurrentPuntHeight + ((TempLeft + TempLineLoop) * TempSizeX);
			bool TempFoundProduct = false;
			//scan from near top of bottom detector down to bottom looking for the height of the punt
			for (int TempPixelLoop = TempCurrentPuntHeight; TempPixelLoop >= TempBottom; TempPixelLoop--)
			{
				if ((TempImagePointer + 4 < TempMaxImage) && (TempImagePointer >= TempImage))
				{
					if (*TempImagePointer < TempThreshold) //found the punt
					{
						BYTE *TempPixelAbove = TempImagePointer + 1;
						BYTE *TempPixel2Above = TempPixelAbove + 1;
						BYTE *TempPixel3Above = TempPixel2Above + 1;
						BYTE *TempPixel4Above = TempPixel3Above + 1;

						BYTE TempLineBrightness = (BYTE)TempImageBrightness;
						if (*TempPixel4Above < 255)
						if ((*TempPixel4Above < TempImageBrightness + 30) && (*TempPixel4Above > TempImageBrightness - 30))
							TempLineBrightness = *TempPixel4Above;

						TempCurrentPuntHeight = TempPixelLoop + 1;
						//add one if pixel above is dark, but not one above that
						if (!vAddStructure) 
						if ((!TempJustMarkPunt) || (TempJustMarkPunt > 1))
						{
							//if ((*TempPixelAbove < TempImageBrightness - 10) && (*TempPixel2Above < *TempPixel4Above - 10))
							//{
							//	TempCurrentPuntHeight--;  //decrease the height one as think it is part of a real contaminant
							//}
							//else
							if ((*TempPixelAbove < TempLineBrightness - 35) && (*TempPixel2Above > TempLineBrightness - 25))
							{
								TempCurrentPuntHeight++;  //increase the height one as think it is just to top of the punt
							}
						}
						vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempCurrentPuntHeight; //put the height of the punt for this line in the profile array
						if (TempCurrentPuntHeight < TempMinimumPuntHeight)
							TempMinimumPuntHeight = TempCurrentPuntHeight;
						if (TempCurrentPuntHeight > TempMaximumPuntHeight)
							TempMaximumPuntHeight = TempCurrentPuntHeight;
						TempPixelLoop = 0;
					}
					else
					if (*TempImagePointer < TempHighThreshold) //found the punt but may not be the best value, so keep looking
					{
						if (!TempBackupPuntHeight)
							TempBackupPuntHeight = TempPixelLoop + 1;
					}
				}
				//else
				//	ReportErrorMessage("RemoveJarPunt Pointer Error", cEMailInspx, 32000);

				if ((TempPixelLoop == TempBottom) && (TempCurrentPuntHeight > TempBottom) && 
					(TempBackupPuntHeight) && (TempBackupPuntHeight < 40)) //if did not find punt on this line, set to backup height if have one
				{
					TempCurrentPuntHeight = TempBackupPuntHeight;
					vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempCurrentPuntHeight;
				}
				TempImagePointer--;
			}
		}

		if (TempMinimumPuntHeight < TempBottom)
			TempMinimumPuntHeight = TempBottom;

		int TempLeftMostValue = 255;
		int TempRightMostValue = 255;
		int TempLeftMaximumIndex = 0;  //is array index of left curve fit edge
		int TempRightMaximumIndex = TempRight;//is array index of right curve fit edge

		if (TempMinimumPuntHeight <= TempMaximumPuntHeight)  //found have a punt that needs eliminating
		if (TempMaximumPuntHeight > TempBottom)
		{
			if (!vAddStructure)
			if ((!TempJustMarkPunt) || (TempJustMarkPunt > 2))
			{
				//clip any bump up or down of one to 4 pixels
				for (TempLineLoop = 0; TempLineLoop < TempRight - 3; TempLineLoop++)
				{
					if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
					{
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2])
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 3])
						{
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
						}
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 4])
						{
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 3] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
						}
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 5])
						{
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 3] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 4] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
						}
					}
				}
			}
			//find left curve height and place
			int TempCenter = TempNumberOfLinesInROI / 2;

			//find left curve height
			TempLeftMostValue = 255;
			for (TempLineLoop = TempCenter; TempLineLoop > 2; TempLineLoop--)
			{
				if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop] + 3 < TempLeftMostValue) && (TempLeftMostValue < 255))
					TempLineLoop = 0;
				else
				if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < TempLeftMostValue) && 
					(vRecentPuntRemovalPuntHeightProfile[TempLineLoop - 1] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop]) &&
					(vRecentPuntRemovalPuntHeightProfile[TempLineLoop - 2] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop]))
				{
					TempLeftMostValue = vRecentPuntRemovalPuntHeightProfile[TempLineLoop]; // ? + 1
					TempLeftMaximumIndex = TempLineLoop;
				}
			}

			TempRightMostValue = 255;
			for (TempLineLoop = TempCenter; TempLineLoop < TempNumberOfLinesInROI - 2; TempLineLoop++)
			{
				if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop] + 3 < TempRightMostValue) && (TempRightMostValue < 255))
					TempLineLoop = TempNumberOfLinesInROI;
				else
				if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < TempRightMostValue) && 
					(vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop]) &&
					(vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop]))
				{
					TempRightMostValue = vRecentPuntRemovalPuntHeightProfile[TempLineLoop]; // ? + 1
					TempRightMaximumIndex = TempLineLoop;
				}
			}


			//eliminate data beyond left edge of curve fit area
			//for (TempLineLoop = 0; TempLineLoop < TempLeftMaximumIndex; TempLineLoop++)
			//{
			//	vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = 255;
			//}

			//eliminate data beyond Right edge of curve fit area
			//for (TempLineLoop = TempNumberOfLinesInROI - 1; TempLineLoop > TempRightMaximumIndex; TempLineLoop--)
			//{
			//	vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = 255;
			//}

			WORD TempNumberOfLinesCurveFit = TempRightMaximumIndex - TempLeftMaximumIndex + 1;
			if (TempNumberOfLinesCurveFit + TempLeftMaximumIndex > TempSizeY)
				TempNumberOfLinesCurveFit = TempSizeY - TempLeftMaximumIndex;
			int TempCenterIndex = TempLeftMaximumIndex + ((TempRightMaximumIndex - TempLeftMaximumIndex) / 2);

			BYTE *TempBottomProfileArray = NULL;
			TempBottomProfileArray = (BYTE *)malloc(TempNumberOfLinesCurveFit);
			//BYTE *TempBottomProfileArrayPointer = TempBottomProfileArray;
			if (TempBottomProfileArray)
			{
				FillMemory(TempBottomProfileArray, TempNumberOfLinesCurveFit, 255);

				//trim any rise after well on left side using left curve height
				if ((!TempJustMarkPunt) || (TempJustMarkPunt > 2))
				{
					for (TempLineLoop = 0; TempLineLoop < TempLeftMaximumIndex; TempLineLoop++)
					{
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > TempLeftMostValue)
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempLeftMostValue;
					}

					//trim any rise after well on left side using right curve height
					for (TempLineLoop = TempRightMaximumIndex + 1; TempLineLoop < TempNumberOfLinesInROI; TempLineLoop++)
					{
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > TempRightMostValue)
							vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempRightMostValue;
					}

					//trim any large bumps on the left side
					TempCurrentPuntHeight = vRecentPuntRemovalPuntHeightProfile[TempCenterIndex];
					for (TempLineLoop = TempCenterIndex + 1; TempLineLoop > 0; TempLineLoop--)
					{
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
						{
							if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > TempCurrentPuntHeight + 3)
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempCurrentPuntHeight;
							else
								TempCurrentPuntHeight = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
						}
					}

					//trim any large bumps on the right side
					TempCurrentPuntHeight = vRecentPuntRemovalPuntHeightProfile[TempCenterIndex];
					for (TempLineLoop =  TempCenterIndex- 1; TempLineLoop >= TempRight; TempLineLoop++)
					{
						if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop]) && (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255))
						{
							if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > TempCurrentPuntHeight + 3)
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempCurrentPuntHeight;
							else
								TempCurrentPuntHeight = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
						}
					}
					//clear any value past first 255 going center to left
					bool TempFound255 = false;
					for (TempLineLoop = TempCenterIndex; TempLineLoop > 0; TempLineLoop--)
					{
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
						{
							if (TempFound255)
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = 255;
						}
						else
							TempFound255 = true;
					}

					//clear any value past first 255 going center to right
					TempFound255 = false;
					for (TempLineLoop =  TempCenterIndex; TempLineLoop < TempRightMaximumIndex; TempLineLoop++)
					{
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
						{
							if (TempFound255)
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = 255;
						}
						else
							TempFound255 = true;
					}
				}

				//int TempCenterIndex = TempLeftMaximumIndex + ((TempRightMaximumIndex - TempLeftMaximumIndex) / 2);
				if ((!TempJustMarkPunt) || (TempJustMarkPunt > 4))
				{
					BYTE *TempBottomProfileArrayPointer = TempBottomProfileArray;
					for (TempLineLoop = TempLeftMaximumIndex; TempLineLoop < TempRightMaximumIndex; TempLineLoop++)
						*TempBottomProfileArrayPointer++ = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
					//curve fit punt
					ReturnCurveFitLine(TempBottomProfileArray, TempNumberOfLinesCurveFit, NULL, 4);  // use 4 terms to fit punt for swung Baffles

					// test
					//TempBottomProfileArrayPointer = TempBottomProfileArray;
					//for (int TempLineLoop = TempLeftMaximumIndex; TempLineLoop < TempRightMaximumIndex; TempLineLoop++)
					//	vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = *TempBottomProfileArrayPointer++;


					vRecentPuntRemovalBottomHeight = 0;
					vRecentPuntRemovalNumberOfPixelsCopied = 0;

					//If curve fit lower than threshold use it, raise one pixel if needed, find maximum punt height
					//check left side
					if (vAddStructure)
						vSwungBaffleTolerance = 0;
					TempBottomProfileArrayPointer = TempBottomProfileArray;
					TempMaximumPuntHeight = 0;
					BYTE vPreviousValue = 0;
					BYTE TempCurrentHeight = (BYTE)TempMinimumPuntHeight;
					bool TempClipping = false;
					for (TempLineLoop = TempLeftMaximumIndex; TempLineLoop < TempCenterIndex; TempLineLoop++)
					{
						//clip top of array if goes above curve
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
						{
							//if had a contaminant on the bottom, use the curve fit to trim it off
							if (*TempBottomProfileArrayPointer < 255)
							if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > *TempBottomProfileArrayPointer + vSwungBaffleTolerance)
							{
								if (vPreviousValue)
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop - 1] = vPreviousValue; //restore old value as now clipping
								vPreviousValue = 0;

								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = *TempBottomProfileArrayPointer;//use curve fit height
								TempClipping = true;
							}
							else
							if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > *TempBottomProfileArrayPointer) && (TempClipping))
							{
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = *TempBottomProfileArrayPointer;//use curve fit height
							}
							else
							{  //if curve fit not less than original threshold value

								//mark next pixel above curve fit as punt as it is dark also
								//test
								//if ((!TempJustMarkPunt) || (TempJustMarkPunt > 3))
								//if (*TempPixelAbovePunt + 20 < *TempPixel2AbovePunt) //&& (*TempPixelAbovePunt < TempImageBrightness))// &&
								//	(*TempPixel2AbovePunt > TempImageBrightness))
								//	vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop] + 1; // test add one to every height
								TempClipping = false;
							}
							//record current height to compare next pixel
							TempCurrentHeight = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];

							if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > TempMaximumPuntHeight) //find new maximum punt height
								TempMaximumPuntHeight = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];

						}
						TempBottomProfileArrayPointer++;

					}
					//check right side
					TempBottomProfileArrayPointer = TempBottomProfileArray + TempNumberOfLinesCurveFit - 1;
					TempCurrentHeight = (BYTE)TempMinimumPuntHeight;
					TempClipping = false;
					for (TempLineLoop = TempRightMaximumIndex; TempLineLoop >= TempCenterIndex; TempLineLoop--)
					{
						//clip top of array if goes above curve
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
						{
							//if had a contaminant on the bottom, use the curve fit to trim it off
							if (*TempBottomProfileArrayPointer < 255)
							if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > *TempBottomProfileArrayPointer + vSwungBaffleTolerance)
							{
								if (vPreviousValue)
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop - 1] = vPreviousValue; //restore old value as now clipping
								vPreviousValue = 0;

								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = *TempBottomProfileArrayPointer;
								TempClipping = true;
							}
							else
							if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > *TempBottomProfileArrayPointer) && (TempClipping))
							{
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = *TempBottomProfileArrayPointer;
							}
							else
							{
								//contaminant on bottom so use curve fit to trim off
								//if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > *TempBottomProfileArrayPointer)
								//	vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = *TempBottomProfileArrayPointer;

								//BYTE *TempTopPuntPixel = TempImage + vRecentPuntRemovalPuntHeightProfile[TempLineLoop] + 
								//	((TempLeft + TempLineLoop) * TempSizeX);
								//BYTE *TempPixelAbovePunt = TempTopPuntPixel + 1;
								//BYTE *TempPixel2AbovePunt = TempPixelAbovePunt + 1;
								//BYTE *TempPixel3AbovePunt = TempPixel2AbovePunt + 1;
								// mark next pixel above curve fit as punt as it is dark also
								//test
								//if ((!TempJustMarkPunt) || (TempJustMarkPunt > 3))
								//if (*TempPixelAbovePunt + 20 < *TempPixel2AbovePunt) //&& (*TempPixelAbovePunt < TempImageBrightness))// &&
									//(*TempPixel2AbovePunt > TempImageBrightness))
									//|| (*TempPixelAbovePunt + 20 < *TempPixel2AbovePunt))
								//	vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop] + 1; // test add one to every height

								TempClipping = false;
							}
							//record current height to compare next pixel
							TempCurrentHeight = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];

							if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > TempMaximumPuntHeight) //find new maximum punt height
								TempMaximumPuntHeight = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];

						}
						TempBottomProfileArrayPointer--;

					}
					if (!vAddStructure)
					if ((!TempJustMarkPunt) || (TempJustMarkPunt > 2))
					{
						//clip any bump up or down of one to 4 pixels
						for (TempLineLoop = 0; TempLineLoop < TempRight - 3; TempLineLoop++)
						{
							if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
							{
								if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 5])
								{
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 3] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 4] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
								}
								else
								if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 4])
								{
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 3] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
								}
								else
								if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 3])
								{
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
								}
								else
								if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2])
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
								else
								//if next pixel is more than one down, and pixel after is only one down, set next pixel to this one
								if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] + 1)
								if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2] + 1)
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
								else
								//if next pixel is more than one up, and pixel after is only one up, set next pixel to the one after it
								if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] + 1 < vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1])
								if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] + 1 == vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2])
									vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] = vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2];
							}
						}
					}
				}

				if ((!TempJustMarkPunt) || (TempJustMarkPunt > 3))
				{
					//clear any value past first 255 going center to Left
					bool TempFound255 = false;
					for (TempLineLoop = TempCenterIndex; TempLineLoop > 0; TempLineLoop--)
					{
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
						{
							if (TempFound255)
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = 255;
						}
						else
							TempFound255 = true;
					}

					//clear any value past first 255 going center to Right
					TempFound255 = false;
					for (TempLineLoop =  TempCenterIndex; TempLineLoop < TempRightMaximumIndex; TempLineLoop++)
					{
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
						{
							if (TempFound255)
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = 255;
						}
						else
							TempFound255 = true;
					}

					//add one more to height of punt if next pixel is dark, but one above is light
					if (!vAddStructure) 
					if ((!TempJustMarkPunt) || (TempJustMarkPunt > 5))
					for (TempLineLoop = 0; TempLineLoop < TempNumberOfLinesInROI; TempLineLoop++)
					{
						BYTE *TempImagePointer = TempImage + vRecentPuntRemovalPuntHeightProfile[TempLineLoop] + ((TempLeft + TempLineLoop) * TempSizeX);
						//scan from near top of bottom detector down to bottom looking for the height of the punt
						BYTE *TempPixelAbove = TempImagePointer;
						BYTE *TempPixel2Above = TempPixelAbove + 1;
						BYTE *TempPixel3Above = TempPixel2Above + 1;
						BYTE *TempPixel4Above = TempPixel3Above + 1;

						if ((TempPixel4Above < TempMaxImage) && (TempImagePointer >= TempImage))
						{
							BYTE TempLineBrightness = (BYTE)TempImageBrightness;
							if (*TempPixel4Above < 255)
							if ((*TempPixel4Above < TempImageBrightness + 30) && (*TempPixel4Above > TempImageBrightness - 30))
								TempLineBrightness = *TempPixel4Above;

							//add one if pixel above is dark, but not one above that
							//if (*TempPixel2Above < 255)
							if ((*TempPixelAbove < TempLineBrightness - 35) && (*TempPixel2Above > TempLineBrightness - 25))
							{
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop]++;  //increase the height one as think it is just to top of the punt
							}
						}
						//else
						//	ReportErrorMessage("RemoveJarPunt Pointer Error 2", cEMailInspx, 32000);
					}

					//find left curve height
					TempLeftMostValue = 255;
					for (TempLineLoop = TempCenter; TempLineLoop > 2; TempLineLoop--)
					{
						if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop] + 3 < TempLeftMostValue) && (TempLeftMostValue < 255))
							TempLineLoop = 0;
						else
						if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < TempLeftMostValue) && 
							(vRecentPuntRemovalPuntHeightProfile[TempLineLoop - 1] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop]) &&
							(vRecentPuntRemovalPuntHeightProfile[TempLineLoop - 2] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop]))
						{
							TempLeftMostValue = vRecentPuntRemovalPuntHeightProfile[TempLineLoop]; // ? + 1
							TempLeftMaximumIndex = TempLineLoop;
						}
					}

					TempRightMostValue = 255;
					for (TempLineLoop = TempCenter; TempLineLoop < TempNumberOfLinesInROI - 2; TempLineLoop++)
					{
						if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop] + 3 < TempRightMostValue) && (TempRightMostValue < 255))
							TempLineLoop = TempNumberOfLinesInROI;
						else
						if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < TempRightMostValue) && 
							(vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 1] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop]) &&
							(vRecentPuntRemovalPuntHeightProfile[TempLineLoop + 2] == vRecentPuntRemovalPuntHeightProfile[TempLineLoop]))
						{
							TempRightMostValue = vRecentPuntRemovalPuntHeightProfile[TempLineLoop]; // ? + 1
							TempRightMaximumIndex = TempLineLoop;
						}
					}
				}

				//trim any rise after well on left side using left curve height
				if ((!TempJustMarkPunt) || (TempJustMarkPunt > 2))
				{
					//do left side
					for (TempLineLoop = 0; TempLineLoop < TempLeftMaximumIndex; TempLineLoop++)
					{
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
						{
							//if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > TempLeftMostValue)
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempLeftMostValue;
							//else
							//	TempLineLoop = TempCenterIndex;
						}
						//else
						//	vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempLeftMostValue;
					}
					//do right side
					for (TempLineLoop = TempNumberOfLinesInROI - 1; TempLineLoop > TempRightMaximumIndex; TempLineLoop--)
					{
						if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255)
						{
							//if (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > TempRightMostValue)
								vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempRightMostValue;
							//else
							//	TempLineLoop = TempCenterIndex;
						}
						//else
						//	vRecentPuntRemovalPuntHeightProfile[TempLineLoop] = TempRightMostValue;
					}
				}

				//put the image height of the maximum height copied pixel
				vRecentPuntRemovalBottomHeight = TempRightMostValue;
				if (vRecentPuntRemovalBottomHeight > TempLeftMostValue)
					vRecentPuntRemovalBottomHeight = TempLeftMostValue;

				vRecentPuntRemovalNumberOfPixelsCopied = (BYTE)TempMaximumPuntHeight - vRecentPuntRemovalBottomHeight + 6;

				//now know min and max punt height, if found correctly, move pixels down appropriate amount
				//scan lines from left maximum to right maximum, find punt height, if taller than minimum, copy pixels to cover punt
				if (TempJustMarkPunt)
				{
					for (int TempLineLoop = 0; TempLineLoop < TempNumberOfLinesInROI; TempLineLoop++)
					{
						WORD TempCurrentPuntHeight = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
						if (TempCurrentPuntHeight > vRecentPuntRemovalBottomHeight)
						if (TempCurrentPuntHeight < 255)
						//punt is higher than minimum, so copy pixels down to cover the punt
						{
							int TempAmountPuntTooTallBy = TempCurrentPuntHeight - vRecentPuntRemovalBottomHeight;
							BYTE *TempDestinationPixel = TempImage + vRecentPuntRemovalBottomHeight + ((TempLeft + TempLineLoop) * TempSizeX);

							for (int TempCopyPixelLoop = 0; TempCopyPixelLoop < TempAmountPuntTooTallBy; TempCopyPixelLoop++)
							{
								if (TempDestinationPixel < TempMaxImage)
									*TempDestinationPixel++ = 200;
								//else
								//	ReportErrorMessage("RemoveJarPunt Pointer Error 3", cEMailInspx, 32000);
							}
						}
					}
					//if have area in punt below product area turn it white so it is not processed
					if (vRecentPuntRemovalBottomHeight > TempBottom)
					{
						int TempAmountBottomOfPuntAboveBottomROI = vRecentPuntRemovalBottomHeight - TempBottom;
						for (int TempLineLoop = TempLeft; TempLineLoop < TempRight; TempLineLoop++)
						{
							BYTE *TempDestinationPixel = TempImage + TempBottom + (TempLineLoop * TempSizeX);
							for (int TempCopyPixelLoop = 0; TempCopyPixelLoop < TempAmountBottomOfPuntAboveBottomROI; TempCopyPixelLoop++)
							{
								if (TempDestinationPixel < TempMaxImage)
									*TempDestinationPixel++ = 200;
								//else
								//	ReportErrorMessage("RemoveJarPunt Pointer Error 4", cEMailInspx, 32000);
							}
						}
					}
				}
				else
				{  //move pixels above punt down to hide punt
					WORD TempCenter = TempNumberOfLinesInROI / 2;
					vRecentPuntRemovalNumberOfPixelsCopied = (BYTE)TempMaximumPuntHeight + 5; //is from - 1 + 6;
					if (vRecentPuntRemovalBottomHeight > TempBottom)
					{
						int TempAmountBottomOfPuntAboveBottomROI = vRecentPuntRemovalBottomHeight - TempBottom;
						for (int TempLineLoop = TempLeft; TempLineLoop < TempRight; TempLineLoop++)
						{
							BYTE *TempDestinationPixel = TempImage + TempBottom + (TempLineLoop * TempSizeX);
							for (int TempCopyPixelLoop = 0; TempCopyPixelLoop < TempAmountBottomOfPuntAboveBottomROI; TempCopyPixelLoop++)
							{
								if (TempDestinationPixel < TempMaxImage)
									*TempDestinationPixel++ = 255;
								//else
								//	ReportErrorMessage("RemoveJarPunt Pointer Error 5", cEMailInspx, 32000);
							}
						}
					}
					WORD TempCurrentPuntHeight = vRecentPuntRemovalBottomHeight;
					for (int TempLineLoop = 0; TempLineLoop < TempNumberOfLinesInROI; TempLineLoop++)
					{
						TempCurrentPuntHeight = vRecentPuntRemovalBottomHeight;
						//if (TempCurrentPuntHeight > vRecentPuntRemovalBottomHeight)
						//punt is higher than minimum, so copy pixels down to cover the punt
						if ((vRecentPuntRemovalPuntHeightProfile[TempLineLoop] > 1) && (vRecentPuntRemovalPuntHeightProfile[TempLineLoop] < 255))
							TempCurrentPuntHeight = vRecentPuntRemovalPuntHeightProfile[TempLineLoop];
						//{
							int TempAmountPuntTooTallBy = TempCurrentPuntHeight - vRecentPuntRemovalBottomHeight - 1;
							//BYTE *TempDestinationPixel = TempImage + vRecentPuntRemovalBottomHeight + ((TempLeft + TempLineLoop) * TempSizeX);
							//copy punt to one pixel up from bottom of image
							BYTE *TempDestinationPixel = TempImage + 1 + ((TempLeft + TempLineLoop) * TempSizeX);

							BYTE *TempSourcePixel = TempDestinationPixel + TempAmountPuntTooTallBy + vRecentPuntRemovalBottomHeight;
							//copy pixels down to cover punt
							for (int TempCopyPixelLoop = 0; TempCopyPixelLoop < vRecentPuntRemovalNumberOfPixelsCopied; TempCopyPixelLoop++)
							{
								if ((TempDestinationPixel < TempMaxImage) && (TempSourcePixel < TempMaxImage))
									*TempDestinationPixel++ = *TempSourcePixel++;
								//else
								//	ReportErrorMessage("RemoveJarPunt Pointer Error 6", cEMailInspx, 32000);
							}
						//}
					}
				}
				if (TempBottomProfileArray)
					free(TempBottomProfileArray);
			}
		}
	}
}
*/
//test for alex
void CInspection::TakeLineDerivative(BYTE *TempImage, BYTE *TempScratchImage, WORD TempSizeX, WORD TempSizeY, 
																					double TempImageBrightness, BYTE TempInspectionThreshold)
{
	if (vEnable)
	{
		DWORD TempSize = TempSizeX * TempSizeY;

		WORD TempTop = vROIPixelTop;
		WORD TempBottom = vROIPixelBottom;
		if (TempTop >= TempSizeX)
			TempTop = TempSizeX - 1;
		if (TempBottom >= TempTop)
			TempBottom = 2;
		WORD TempLeft = vROIPixelLeft;
		WORD TempRight = vROIPixelRight;
		if (TempRight + 5 > TempSizeY)
			TempRight = TempSizeY - 5;
		if (TempLeft >= TempRight)
			TempLeft = 0;
		if (TempLeft < 5)
			TempLeft = 5;

		WORD TempHeight = vROIPixelTop - TempBottom;
		if (TempHeight + TempBottom > TempSizeX)
			TempHeight = TempSizeX - TempBottom;

		WORD TempWidth = TempRight - TempLeft;
		if (TempWidth + TempLeft + 5 > TempSizeY)
			TempWidth = TempSizeY - TempLeft - 5;

		//ReportErrorMessage("Glass On Bottom Left: " + dtoa(TempLeft,0) + " Right: " + dtoa(TempRight,0) + " Bottom: " +
		//	dtoa(TempBottom,0) + " Top: " + dtoa(TempTop,0) + " Width: " + dtoa(TempWidth,0), cWriteToLog,0);

		if (TempImage)
		if (TempScratchImage)
		if (TempWidth < 10)
		{
			ReportErrorMessage("Notice: Derivative too few pixels, Ignore Inspection ", cWriteToLog,0);
		}
		else
		{
			//copy original image to scratch image
			CopyMemory(TempScratchImage, TempImage, TempSize);

			//clear original image to white to use as the destination image for the results
			FillMemory(TempImage, TempSize, 255);

			BYTE *TempCurrentPixel = NULL;
			BYTE *TempNextPixel = NULL;
			BYTE *TempNextPixelAfterThat = NULL;
			BYTE *TempResultPixel = NULL;
			BYTE *TempMaximumScratchPointer = TempScratchImage + (TempSizeY * TempSizeX);
			BYTE *TempMaximumResultPointer = TempImage + (TempSizeY * TempSizeX);

			//start at the bottom ROI line and study every line up to the top ROI line
			for (WORD TempLineLoop = 0; TempLineLoop < TempHeight; TempLineLoop++)
			{
				//scanning from center to left, indicate where image gets darker than is
				TempCurrentPixel = TempScratchImage + TempLineLoop + TempBottom + (TempLeft * TempSizeX) ;
				TempNextPixel = TempCurrentPixel + TempSizeX;
				TempNextPixelAfterThat = TempNextPixel + TempSizeX;
				TempResultPixel = TempImage + TempLineLoop + TempBottom + (TempLeft * TempSizeX);

				//scanning from Left ROI Line to right, output derivative
				for (WORD TempPixelLoop = 0; TempPixelLoop < TempWidth; TempPixelLoop++)
				if ((TempResultPixel < TempMaximumResultPointer) && (TempNextPixel < TempMaximumScratchPointer))
				{
					if (TempCurrentPixel != TempNextPixel)
					{
						*TempResultPixel = 255 - abs((int)(*TempCurrentPixel) - (int)(*TempNextPixel));
					}
					TempCurrentPixel = TempNextPixel;
					TempNextPixel = TempNextPixel + TempSizeX;
					TempResultPixel = TempResultPixel + TempSizeX;
				}
			}
		}
	}
}
/*
void CInspection::TakeLineDerivativeOfRandomShape(BYTE *TempImage, BYTE *TempScratchImage, WORD TempSizeX, WORD TempSizeY, 
																					double TempImageBrightness)
{
	if (vEnable)
	{
		DWORD TempSize = TempSizeX * TempSizeY;

		WORD TempTop = vROIPixelTop;
		WORD TempBottom = vROIPixelBottom;
		if (TempTop >= TempSizeX)
			TempTop = TempSizeX - 1;
		if (TempBottom >= TempTop)
			TempBottom = 2;
		WORD TempLeft = vROIPixelLeft;
		WORD TempRight = vROIPixelRight;
		if (TempRight + 5 > TempSizeY)
			TempRight = TempSizeY - 5;
		if (TempLeft >= TempRight)
			TempLeft = 0;
		if (TempLeft < 5)
			TempLeft = 5;

		WORD TempHeight = vROIPixelTop - TempBottom;
		if (TempHeight + TempBottom > TempSizeX)
			TempHeight = TempSizeX - TempBottom;

		WORD TempWidth = TempRight - TempLeft;
		if (TempWidth + TempLeft + 5 > TempSizeY)
			TempWidth = TempSizeY - TempLeft - 5;

		//ReportErrorMessage("Glass On Bottom Left: " + dtoa(TempLeft,0) + " Right: " + dtoa(TempRight,0) + " Bottom: " +
		//	dtoa(TempBottom,0) + " Top: " + dtoa(TempTop,0) + " Width: " + dtoa(TempWidth,0), cWriteToLog,0);

		if (TempImage)
		if (TempScratchImage)
		if (TempWidth < 10)
		{
			ReportErrorMessage("Notice: Derivative too few pixels, Ignore Inspection ", cWriteToLog,0);
		}
		else
		{
			//copy original image to scratch image
			CopyMemory(TempScratchImage, TempImage, TempSize);

			//clear original image to white to use as the destination image for the results
			FillMemory(TempImage, TempSize, 255);

			BYTE *TempPreviousPixel = NULL;
			BYTE *TempCurrentPixel = NULL;
			BYTE *TempNextPixel = NULL;
			BYTE *TempNextPixelAfterThat = NULL;
			BYTE *TempPixelBeyondThat = NULL;
			BYTE *TempResultPixel = NULL;
			BYTE *TempMaximumScratchPointer = TempScratchImage + (TempSizeY * TempSizeX);
			BYTE *TempMaximumResultPointer = TempImage + (TempSizeY * TempSizeX);

			//start at the bottom ROI line and study every line up to the top ROI line
			for (WORD TempLineLoop = 0; TempLineLoop < TempHeight; TempLineLoop++)
			{
				//scanning from center to left, indicate where image gets darker than is
				TempPreviousPixel = TempScratchImage + TempLineLoop + TempBottom + (TempLeft * TempSizeX);
				TempCurrentPixel = TempPreviousPixel + TempSizeX;
				TempNextPixel = TempCurrentPixel + TempSizeX;
				TempNextPixelAfterThat = TempNextPixel + TempSizeX;
				TempPixelBeyondThat = TempNextPixelAfterThat + TempSizeX;
				TempResultPixel = TempImage + TempLineLoop + TempBottom + (TempLeft * TempSizeX);

				//scanning from Left ROI Line to right, output derivative
				for (WORD TempPixelLoop = 0; TempPixelLoop < TempWidth; TempPixelLoop++)
				if ((TempResultPixel < TempMaximumResultPointer) && (TempPixelBeyondThat < TempMaximumScratchPointer))
				{
					if (TempCurrentPixel != TempNextPixelAfterThat)
					{
						BYTE TempMaximum = *TempPreviousPixel;

						if (TempMaximum < *TempCurrentPixel)
							TempMaximum = *TempCurrentPixel;

						if (TempMaximum < *TempNextPixel)
							TempMaximum = *TempNextPixel;

						if (TempMaximum < *TempNextPixelAfterThat)
							TempMaximum = *TempNextPixelAfterThat;

						if (TempMaximum < *TempPixelBeyondThat)
							TempMaximum = *TempPixelBeyondThat;

						//if (TempImageBrightness + 20 > TempMaximum)
						//	TempMaximum = 1;
						//else
						//	TempMaximum = TempMaximum / 50;

						if (TempMaximum >=250)
							TempMaximum = 2;
						else
							TempMaximum = 1;

						if ((*TempCurrentPixel != 255) && (*TempNextPixelAfterThat != 255) && (*TempNextPixel != 255))
							*TempResultPixel = 255 - ((abs((int)(*TempCurrentPixel) - (int)(*TempNextPixelAfterThat))) / TempMaximum);
					}
					TempPreviousPixel = TempCurrentPixel;
					TempCurrentPixel = TempNextPixel;
					TempNextPixel = TempNextPixelAfterThat;
					TempNextPixelAfterThat = TempPixelBeyondThat;
					TempPixelBeyondThat = TempPixelBeyondThat + TempSizeX;

					TempResultPixel = TempResultPixel + TempSizeX;
				}
			}
		}
	}
}
*/
void CInspection::FindDarkSpotOnLine(BYTE *TempImage, BYTE *TempScratchImage, WORD TempSizeX, WORD TempSizeY, BYTE TempInspectionThreshold)
{
	//This algorithm looks for a dark spot on the line by seaching the next vFindDarkSpotOnLine pixels ahead.  
	//If the pixel values drop by the threshold and return by the X pixel, then all pixels below the threshold
	//are copied to the output pixel.  The value assigned them is the amount they are darker than the threshold from the threshold
	if (vEnable)
	{
		if (!vPixelsStepToNextROI)
			vPixelsStepToNextROI = 1;
		if (vPixelsStepToNextROI >= vFindDarkSpotOnLine)
			vPixelsStepToNextROI = 1;

		BYTE TempMaxSpotSize = vFindDarkSpotOnLine;
		int TempThreshold = 255 - TempInspectionThreshold;

		DWORD TempSize = TempSizeX * TempSizeY;

		WORD TempTop = vROIPixelTop;
		WORD TempBottom = vROIPixelBottom;
		if (TempTop >= TempSizeX)
			TempTop = TempSizeX - 1;
		if (TempBottom >= TempTop)
			TempBottom = 2;
		WORD TempLeft = vROIPixelLeft;
		WORD TempRight = vROIPixelRight;
		if (TempRight + 5 > TempSizeY)
			TempRight = TempSizeY - 5;
		if (TempLeft >= TempRight)
			TempLeft = 0;
		if (TempLeft < 5)
			TempLeft = 5;

		WORD TempHeight = vROIPixelTop - TempBottom;
		if (TempHeight + TempBottom > TempSizeX)
			TempHeight = TempSizeX - TempBottom;

		WORD TempWidth = TempRight - TempLeft;
		if (TempWidth + TempLeft + 5 > TempSizeY)
			TempWidth = TempSizeY - TempLeft - 5;

		//ReportErrorMessage("Glass On Bottom Left: " + dtoa(TempLeft,0) + " Right: " + dtoa(TempRight,0) + " Bottom: " +
		//	dtoa(TempBottom,0) + " Top: " + dtoa(TempTop,0) + " Width: " + dtoa(TempWidth,0), cWriteToLog,0);

		if (TempImage)
		if (TempScratchImage)
		if (TempWidth < 10)
		{
			ReportErrorMessage("Notice: Spot on Line too few pixels, Ignore Inspection ", cWriteToLog,0);
		}
		else
		{
			//copy original image to scratch image
			CopyMemory(TempScratchImage, TempImage, TempSize);

			//clear original image to white to use as the destination image for the results
			FillMemory(TempImage, TempSize, 255);

			BYTE *TempCurrentPixel = NULL;
			BYTE *TempNextPixel = NULL;
			BYTE *TempResultPixel = NULL;
			BYTE *TempMaximumScratchPointer = TempScratchImage + (TempSizeY * TempSizeX);
			BYTE *TempMaximumResultPointer = TempImage + (TempSizeY * TempSizeX);

			//start at the bottom ROI line and study every line up to the top ROI line
			for (WORD TempLineLoop = 0; TempLineLoop < TempHeight; TempLineLoop++)
			{
				//scanning from center to left, indicate where image gets darker than is
				TempCurrentPixel = TempScratchImage + TempLineLoop + TempBottom + (TempLeft * TempSizeX);
				TempResultPixel = TempImage + TempLineLoop + TempBottom + (TempLeft * TempSizeX);

				//scanning from Left ROI Line to right, output derivative
				for (WORD TempPixelLoop = 0; TempPixelLoop < TempWidth; TempPixelLoop = TempPixelLoop + vPixelsStepToNextROI)
				if ((TempResultPixel + (TempMaxSpotSize * TempSizeX) < TempMaximumResultPointer) && 
					(TempCurrentPixel + (TempMaxSpotSize * TempSizeX) < TempMaximumScratchPointer))
				{
					BYTE TempMinimum = *TempCurrentPixel;
					BYTE TempFoundDarker = false;
					BYTE TempSpotDetected = false;
					BYTE TempSpotLength = 0;
					TempNextPixel = TempCurrentPixel;

					int TempStartPixelValue = 255;
					TempStartPixelValue = *TempCurrentPixel - TempThreshold;
					if (TempStartPixelValue < 0)
						TempStartPixelValue = 0;

					//BYTE TempPixel[50];
					//for (BYTE TempSpotLoop = 0; TempSpotLoop < 50; TempSpotLoop++)
					//	TempPixel[TempSpotLoop] = 0;
					//TempPixel[0] = *TempCurrentPixel;

					if (*TempCurrentPixel < 255)
					for (BYTE TempSpotLoop = 0; TempSpotLoop < TempMaxSpotSize; TempSpotLoop++)
					{
						TempNextPixel = TempNextPixel + TempSizeX;
						//TempPixel[TempSpotLoop + 1] = *TempNextPixel;

						if (TempMinimum > *TempNextPixel)
							TempMinimum = *TempNextPixel;

						if (*TempNextPixel < TempStartPixelValue)
							TempFoundDarker = true;

						if (*TempNextPixel == 255)
							TempSpotLoop = TempMaxSpotSize;  //see saturated pixel, so exit check from this pixel
						else
						if (TempFoundDarker)
						if (*TempNextPixel > TempMinimum + TempThreshold)
						{
							TempSpotDetected = true;
							TempSpotLength = TempSpotLoop;
							TempSpotLoop = TempMaxSpotSize;
						}
					}
					if (TempSpotDetected)
					{
						BYTE TempStartValue = *TempCurrentPixel;
						BYTE TempStopValue = *TempNextPixel;
						BYTE TempLowValue = *TempCurrentPixel;
						if (TempLowValue > *TempNextPixel)
							TempLowValue = *TempNextPixel;

						if ((TempStartValue > 200) || (TempStopValue > 200))
						{
							if (TempLowValue > TempThreshold * 2)
								TempLowValue = TempLowValue - (TempThreshold * 2);
							else
							if (TempLowValue > TempThreshold)
								TempLowValue = TempLowValue - TempThreshold;
						}
						else
						{
							if (TempLowValue > TempThreshold)
								TempLowValue = TempLowValue - TempThreshold;
						}

						TempNextPixel = TempCurrentPixel;
						BYTE *TempNextResultPixel = TempResultPixel;
						for (BYTE TempSpotLoop = 0; TempSpotLoop < TempSpotLength; TempSpotLoop++)
						{
							TempNextPixel = TempNextPixel + TempSizeX;
							TempNextResultPixel = TempNextResultPixel + TempSizeX;
							if ((TempNextResultPixel < TempMaximumResultPointer) && (TempNextPixel < TempMaximumScratchPointer))
							if (*TempNextPixel < TempLowValue)
							{
								BYTE TempValue = (BYTE)(TempInspectionThreshold - (TempLowValue - *TempNextPixel));
								if (*TempNextResultPixel > TempValue)
									*TempNextResultPixel = TempValue;
							}
						}
					}
					TempCurrentPixel = TempCurrentPixel + (TempSizeX * vPixelsStepToNextROI);
					TempResultPixel = TempResultPixel + (TempSizeX * vPixelsStepToNextROI);
				} //end each pixel
			} //end each line
		}
	}
}

void CInspection::FindBonesInspection(BYTE *TempImage, Image *TempScratchImage, WORD TempSizeX, WORD TempSizeY, WORD TempLeft, WORD TempRight, WORD TempBottom, WORD TempTop)
{
	if ((TempImage) && (TempScratchImage))
	{
		if (TempTop >= TempSizeX)
			TempTop = TempSizeX - 1;
		if (TempRight >= TempSizeY)
			TempRight = TempSizeY - 1;
		DWORD TempSize = TempSizeX * TempSizeY;

		//b rh testing
		//Image *TempOriginalImage = NULL;
		//if (ThereIsEnoughMemory(TempSize + sizeof(Image), "Temporary Image for Find Bones"))
		//	TempOriginalImage = im_create(IM_BYTE, TempSizeX, TempSizeY);
		//CopyMemory(TempOriginalImage->pix, TempImage, TempSize);

		Image *TempImageToRotate = NULL;
		if (ThereIsEnoughMemory(TempSize + sizeof(Image), "Temporary Image for Find Bones"))
			TempImageToRotate = im_create(IM_BYTE, TempSizeX, TempSizeY);

		if ((TempImageToRotate) && (TempImageToRotate->pix))
		if (TempScratchImage)
		{ // two matrixes
			//scan side to side
			CopyMemory(TempImageToRotate->pix, TempImage, TempSize);
			//LookForDarkSpotsOnLineInOneDirection(TempScratchImage->pix, TempScratchImage->pix, TempSizeX, TempSizeY, TempLeft, TempRight, TempBottom, TempTop);
			RotateImage90Degrees(TempImageToRotate, 1);
			FindBonesInOneDirection(TempImageToRotate->pix, TempScratchImage->pix, TempSizeY, TempSizeX, TempBottom, TempTop, TempLeft, TempRight);
			RotateImage90Degrees(TempImageToRotate, -1);

			FindBonesInOneDirection(TempImage, TempScratchImage->pix, TempSizeX, TempSizeY, TempLeft, TempRight, TempBottom, TempTop);

			//take smaller value of two images for each pixel
			BYTE *TempSource = TempImage;
			BYTE *TempSource2 = TempImageToRotate->pix;
			for (DWORD TempLoop = 0; TempLoop < TempSize; TempLoop++)
			{
				if (*TempSource > *TempSource2)  //take the smallest value of the two
					*TempSource = *TempSource2;

				TempSource++;
				TempSource2++;
			}


			//b rh testing
//CString TempImageFileName = "C:\\ScanTracLogs\\Large Bones.csv";
//CString TempString;
//LPCTSTR TempFileName = TempImageFileName.GetBuffer(TempImageFileName.GetLength());
//CFileException TempFileException;
//CFile *vDataFile;
//vDataFile = new CFile;
//int TempFileOpened = vDataFile->Open(TempFileName, (CFile::modeCreate | CFile::modeReadWrite), &TempFileException);
////write the image data to the file
//CString TempOutputString("");
//CString TempPixelString("Pixel Number,");
//	for (WORD TempPixelLoop = 0; TempPixelLoop < TempSizeY; TempPixelLoop++)
//	{
//		TempPixelString = TempPixelString + dtoa(TempPixelLoop + 1, 0) + ',';
//	}
//	TempPixelString = TempPixelString + "Pixel Number\n";
//if (TempFileOpened)
//{
//	vDataFile->Write(TempPixelString.GetBuffer(TempPixelString.GetLength()), TempPixelString.GetLength());
//	for (WORD TempLineLoop = 0; TempLineLoop < TempSizeX; TempLineLoop++)
//	{
//		TempOutputString = "Line Number: " + dtoa(TempLineLoop, 0) + ',';
//		BYTE *TempOriginalPointer = TempOriginalImage->pix + TempLineLoop;
//		BYTE *TempResultPointer = TempImage + TempLineLoop;
//		for (WORD TempPixelLoop = 0; TempPixelLoop < TempSizeY; TempPixelLoop++)
//		{
//			TempOutputString = TempOutputString + dtoa(*TempOriginalPointer, 0) + ":" + dtoa(*TempResultPointer, 0) + ',';
//			TempOriginalPointer = TempOriginalPointer + TempSizeX;
//			TempResultPointer = TempResultPointer + TempSizeX;
//		}
//		TempOutputString = TempOutputString + "Line Number: " + dtoa(TempLineLoop, 0) + "\n";
//		vDataFile->Write(TempOutputString.GetBuffer(TempOutputString.GetLength()), TempOutputString.GetLength());
//	}
//	vDataFile->Write(TempPixelString.GetBuffer(TempPixelString.GetLength()), TempPixelString.GetLength());
//	vDataFile->Close();
//}
//delete(vDataFile);
//if (TempOriginalImage)
//	im_delete(TempOriginalImage);


		}
		if (TempImageToRotate)
			im_delete(TempImageToRotate);
	}
}

//Limited Transition Distance Bones Inspection
//new algorithm just looking for dark spots but faint with transition in limited number of pixels
void CInspection::FindBonesInOneDirection(BYTE *TempImage, BYTE *TempScratchImage, WORD TempSizeX, WORD TempSizeY, WORD TempLeft, WORD TempRight, WORD TempBottom, WORD TempTop)
{
	if (vEnable)
	{
		//br h test
		//BYTE TempPixelData[50];
		//FillMemory(&TempPixelData[0], 50, 0);

		BYTE TempTotalLookAhead = vBonesInspection;

		if ((TempTotalLookAhead < 1) || (TempTotalLookAhead > 55))
		{
			TempTotalLookAhead = 15;
			vBonesInspection = TempTotalLookAhead;
		}

		BYTE TempTransitionLookAhead = vPixelLookAhead;

		if ((TempTransitionLookAhead < 1) || (TempTransitionLookAhead > 12) || (TempTransitionLookAhead >= TempTotalLookAhead))
		{
			TempTransitionLookAhead = 3;
			vPixelLookAhead = TempTransitionLookAhead;
		}

		if (TempTransitionLookAhead >= TempTotalLookAhead)
			TempTransitionLookAhead = 1;

		BYTE TempStartToGetBrighterPoint = TempTotalLookAhead - TempTransitionLookAhead;

		DWORD TempSize = TempSizeX * TempSizeY;

		if (TempTop >= TempSizeX)
		{
			//ReportErrorMessage("Bones Inspection-ROI Wrong 8. Top: " + dtoa(TempTop, 0), cEMailInspx, 32000);
			TempTop = TempSizeX - 1;
		}
		if (TempBottom >= TempTop)
		{
			ReportErrorMessage("Bones Inspection-Error-ROI Wrong 9. Bottom: " + dtoa(TempBottom, 0), cEMailInspx, 32000);
			TempBottom = 2;
		}
		if (TempRight + TempTotalLookAhead + 2 > TempSizeY - 1)
		{
			//ReportErrorMessage("Bones Inspection-Error-ROI Wrong 10. Right: " + dtoa(TempRight, 0), cEMailInspx, 32000);
			TempRight = TempSizeY - TempTotalLookAhead - 2 - 1;
		}
		if (TempLeft >= TempRight)
		{
			ReportErrorMessage("Bones Inspection-Error-ROI Wrong 11. Left: " + dtoa(TempLeft, 0), cEMailInspx, 32000);
			TempLeft = 1;
		}
		if (TempLeft == 0)
			TempLeft = 1;

		WORD TempHeight = TempTop - TempBottom;
		if (TempHeight + TempBottom > TempSizeX)
		{
			ReportErrorMessage("Bones Inspection-Error-ROI Wrong 13. Height: " + dtoa(TempHeight, 0), cEMailInspx, 32000);
			TempHeight = TempSizeX - TempBottom;
		}

		WORD TempROIWidth = TempRight - TempLeft;
		if (TempROIWidth + TempLeft + TempTotalLookAhead + 2 > TempSizeY)
		{
			//ReportErrorMessage("Bones Inspection-Error-ROI Wrong. Width: " + dtoa(TempROIWidth, 0), cEMailInspx, 32000);
			TempROIWidth = TempSizeY - TempLeft - TempTotalLookAhead - 2;
		}

		if (TempImage)
		if (TempScratchImage)
		if ((TempROIWidth < 20) || (TempROIWidth < (TempTotalLookAhead * 2) + 4))
		{
			ReportErrorMessage("Bones Inspection too few pixels, make ROI larger", cEMailMaintenance, 0);
		}
		else
		{
			//copy original image to scratch image
			CopyMemory(TempScratchImage, TempImage, TempSize);

			//clear original image to white to use as the destination image for the results
			FillMemory(TempImage, TempSize, 255);

			BYTE *TempCurrentPixel = NULL;
			BYTE *TempNextPixel = NULL;
			BYTE *TempLastPixel = NULL;
			BYTE *TempResultPixel = NULL;
			BYTE *TempMaximumScratchPointer = TempScratchImage + TempSize;
			BYTE *TempMaximumResultPointer = TempImage + TempSize;

			//start at the bottom ROI line and study every line up to the top ROI line
			for (WORD TempLineLoop = 0; TempLineLoop < TempHeight; TempLineLoop++)
			{
				//scanning from Left ROI to Right ROI Line, indicate where image gets darker than is, output image is how much next pixel got darker
				TempCurrentPixel = TempScratchImage + TempLineLoop + TempBottom + (TempLeft * TempSizeX);
				TempNextPixel = TempCurrentPixel + TempSizeX;
				TempLastPixel = TempCurrentPixel + (TempSizeX * TempTotalLookAhead);
				TempResultPixel = TempImage + TempLineLoop + TempBottom + (TempLeft * TempSizeX);//plus 1 equals next pixel

				for (int TempPixelLoop = TempLeft; TempPixelLoop <= TempRight; TempPixelLoop++)
				{
					//b rh test
					//BYTE *TempPointer = TempCurrentPixel;
					//for (int TempPLoop = 0; TempPLoop <= TempTotalLookAhead; TempPLoop++)
					//if (TempPointer < TempMaximumScratchPointer)
					//{
					//	TempPixelData[TempPLoop] = *TempPointer;
					//	TempPointer = TempPointer + TempSizeX;
					//}
					//else
					//	TempPixelData[TempPLoop] = 255;

					BYTE TempPixelBefore = 255;
					BYTE TempPixelAfter = 255;
					if (vIgnoreEdgePixels)
					{ 
						BYTE *TempPixelBeforePointer = TempCurrentPixel - (vIgnoreEdgePixels * TempSizeX);
						if (TempPixelBeforePointer >= TempScratchImage)
							TempPixelBefore = *TempPixelBeforePointer;
						BYTE *TempPixelAfterPointer = TempCurrentPixel + ((TempTotalLookAhead + vIgnoreEdgePixels) * TempSizeX);
						if (TempPixelAfterPointer <= TempMaximumScratchPointer)
							TempPixelAfter = *TempPixelAfterPointer;
					}

					if ((TempLastPixel < TempMaximumScratchPointer) && (TempResultPixel < TempMaximumResultPointer))
					if ((*TempCurrentPixel < 255) && (*TempCurrentPixel > 10))
					if (*TempNextPixel < *TempCurrentPixel)
					if ((!vIgnoreEdgePixels) || ((TempPixelBefore < 250) && (TempPixelAfter < 250)))
					{
						//b rh test only
						//if ((TempLineLoop == 203) && (TempPixelLoop <= 187))
						//	TempPixelData[49] = 255;

						BYTE TempDarkestPixel = 255;
						BYTE TempBrightestAfterDarkestPixel = 0;
						BYTE TempDarkestPixelGoingBrighter = 255;
						BYTE TempBrightestPixelGoingBrighter = 0;
						BYTE *TempPixelToCheck = TempNextPixel;
						BYTE TempDarkestPosition = 0;
						BYTE TempBrightestPosition = 0;
						for (BYTE TempLookAheadLoop = 0; TempLookAheadLoop < TempTransitionLookAhead; TempLookAheadLoop++)
						if ((TempPixelToCheck >= TempScratchImage) && (TempPixelToCheck < TempMaximumScratchPointer))
						if (TempPixelLoop + TempLookAheadLoop <= TempRight) //don't look past the ROI
						{
							if (*TempPixelToCheck >= *TempCurrentPixel) //if pixel checking is brighter than current pixel, then stop test and see if any values should be saved
							{
								if (TempDarkestPixel < 255) //had a dark pixel, so save this is brightest after dark
								{
									TempBrightestAfterDarkestPixel = *TempPixelToCheck;
									TempBrightestPosition = TempLookAheadLoop;
								}

								TempLookAheadLoop = TempTotalLookAhead; //exit loop looking ahead as this pixel is brighter than current pixel studying
							}
							else
							{ //pixel looking at is darker than current pixel
								if ((*TempPixelToCheck < TempDarkestPixel) && (TempLookAheadLoop < TempTransitionLookAhead)) //this pixel is darker than any so far//looking for a darkening trend only within the Transition Look Ahead
								{
									TempDarkestPixel = *TempPixelToCheck;
									TempDarkestPosition = TempLookAheadLoop;
									TempBrightestAfterDarkestPixel = 0;
									TempBrightestPosition = 0;
								}
								//else //this pixel not darker than current
								//if (*TempPixelToCheck > TempBrightestAfterDarkestPixel) //so if brightest after darkest save it
								//{
								//	TempBrightestAfterDarkestPixel = *TempPixelToCheck;
								//	TempBrightestPosition = TempLookAheadLoop;
								//}
							}
							TempPixelToCheck = TempPixelToCheck + TempSizeX;
						}
						//if ((TempBrightestPosition) && (TempDarkestPosition < TempBrightestPosition)) //found a dark and light difference and have the going darker transition value, so calculate the transition of the going brighter value
						if (TempDarkestPixel < 250)
						{
							BYTE TempGoingDarkTransition = *TempCurrentPixel - TempDarkestPixel;
							if (TempGoingDarkTransition > 4)
							{
								BYTE TempGoingBrightTransition = 0;
								for (BYTE TempLookAheadLoop = TempDarkestPosition + 1; TempLookAheadLoop < TempTotalLookAhead; TempLookAheadLoop++)
								for (BYTE TempMaxTransitionLoop = 0; TempMaxTransitionLoop < TempTransitionLookAhead; TempMaxTransitionLoop++)
								if (TempLookAheadLoop + TempMaxTransitionLoop < TempTotalLookAhead)
								{
									BYTE *TempFirstPixel = TempCurrentPixel + (TempLookAheadLoop * TempSizeX);
									BYTE *TempLastPixel = TempFirstPixel + ((TempMaxTransitionLoop + 1) * TempSizeX);
									if (TempLastPixel < TempMaximumScratchPointer)
									{
										if (*TempLastPixel == 255) //if see a saturated pixel terminate the scan without saving the value
											TempMaxTransitionLoop = TempTransitionLookAhead;
										else
										if (*TempLastPixel > *TempFirstPixel)
										{
											BYTE TempBrighteningTransition = *TempLastPixel - *TempFirstPixel;
											if (TempGoingBrightTransition < TempBrighteningTransition)
											{
												TempGoingBrightTransition = TempBrighteningTransition;
												TempBrightestPosition = TempLookAheadLoop + TempMaxTransitionLoop;
												//b rh tests
												//if (TempBrighteningTransition > 20)
												//	TempGoingBrightTransition = TempBrighteningTransition;
											}
										}
									}
								}
								BYTE TempSmallestTransition = TempGoingBrightTransition;
								if (TempSmallestTransition > TempGoingDarkTransition)
									TempSmallestTransition = TempGoingDarkTransition;

								BYTE TempResult = 255 - TempSmallestTransition;
								//if (*TempResultPixel > TempResult)
								//{
								//	*TempResultPixel = TempResult;

								//	//b rh test
								//	if (*TempResultPixel < 220)
								//		*TempResultPixel = TempResult;
								//}
								BYTE *TempMarkItPixel = TempResultPixel;
								for (BYTE TempMarkItLoop = 0; TempMarkItLoop <= TempBrightestPosition + 1; TempMarkItLoop++)
								if (TempMarkItPixel < TempMaximumResultPointer)
								if (*TempMarkItPixel > TempResult)
								{
									*TempMarkItPixel = TempResult;

									//b rh test
									//if (*TempMarkItPixel < 220)
									//	*TempMarkItPixel = TempResult;

									TempMarkItPixel = TempMarkItPixel + TempSizeX;
								}
							}
						}
					}
					TempCurrentPixel = TempCurrentPixel + TempSizeX;
					TempNextPixel = TempCurrentPixel + TempSizeX;
					TempLastPixel = TempCurrentPixel + (TempSizeX * TempTotalLookAhead);
					TempResultPixel = TempResultPixel + TempSizeX;
				}
				//else
				//	ReportErrorMessage("Calculate Glass On Bottom Pointer Error 1, pixel: " + dtoa(TempPixelLoop,0), cEMailInspx, 32000); //test only
			}
		}
	}
}

void CInspection::FindSpotInSubROI(BYTE *TempImage, Image *TempScratchImage, WORD TempSizeX, WORD TempSizeY, BYTE TempInspectionThreshold, tImage *TempDesensitizeImage, 
	bool TempFindAllSizeSpots, WORD TempROIBottom, WORD TempROITop, WORD TempROILeft, WORD TempROIRight)
{
	//This algorithm looks for a dark spot in each sub roi
	if (vEnable)
	{
		WORD TempSaturatedEdgePixelCount = 0;
		if (!vPixelsStepToNextROI)
			vPixelsStepToNextROI = 1;
		if (vPixelsStepToNextROI >= vFindSpotInSubROI)
			vPixelsStepToNextROI = 1;

		BYTE TempMaxSpotSize = vFindSpotInSubROI;
		BYTE TempHalfMaxSpotSize = (vFindSpotInSubROI) / 2;

		int TempThreshold = 255 - TempInspectionThreshold;

		DWORD TempSize = TempSizeX * TempSizeY;

		WORD TempTop = TempROITop + 4;
		if (TempTop >= TempSizeX)
			TempTop = TempSizeX - 1;

		WORD TempBottom = TempROIBottom;
		if (TempBottom > 4)
			TempBottom = TempBottom - 4;
		else
			TempBottom = 1;

		if (TempBottom >= TempTop)
			TempBottom = 2;

		WORD TempRight = TempROIRight + 4;
		if (TempRight >= TempSizeY)
			TempRight = TempSizeY - 1;

		WORD TempLeft = TempROILeft;
		if (TempLeft >=4)
			TempLeft = TempLeft - 4;

		if (TempLeft >= TempRight)
			TempLeft = 1;
		if (TempLeft < 1)
			TempLeft = 1;

		WORD TempHeight = TempTop - TempBottom;
		if (TempHeight + TempBottom > TempSizeX)
			TempHeight = TempSizeX - TempBottom;

		WORD TempWidth = TempRight - TempLeft;
		if (TempWidth + TempLeft + 1 > TempSizeY)
			TempWidth = TempSizeY - TempLeft - 1;

		//ReportErrorMessage("Glass On Bottom Left: " + dtoa(TempLeft,0) + " Right: " + dtoa(TempRight,0) + " Bottom: " +
		//	dtoa(TempBottom,0) + " Top: " + dtoa(TempTop,0) + " Width: " + dtoa(TempWidth,0), cWriteToLog,0);

		if (TempImage)
		//if (TempScratchImage)
		if (TempWidth < 10)
		{
			ReportErrorMessage("Notice: Derivative too few pixels, Ignore Inspection ", cWriteToLog,0);
		}
		else
		{
			BYTE *TempMaximumResultPointer = TempImage + (TempSizeY * TempSizeX);

			Image *TempSubImage = NULL;
			if (TempScratchImage)
			{
				CopyMemory(TempScratchImage->pix, TempImage, TempSize);

				//clear original image to white to use as the destination image for the results
				FillMemory(TempImage, TempSize, 255);
			}
#ifdef UseIFCDLLs
			Erflag TempErrorFlag;
			MVT_BLOB_PARAMS *TempBlobParameters = mvt_blob_create_params(&TempErrorFlag);
			if (TempErrorFlag != IM_OK)
				ReportErrorMessage("MVTools Error 1112", cEMailInspx,32000);
			else
			{
				//if (vConnect4)
				//	TempBlobParameters->connected = CONNECT4;
				//else
				//	TempBlobParameters->connected = CONNECT8;

				TempBlobParameters->blob_color = 0x00;  //fewer xrays
				TempBlobParameters->connected = CONNECT8;
				TempBlobParameters->calc_results = AREA_BBOX;
				if (vSize > 1)
					TempBlobParameters->min_area = vSize;
				else
					TempBlobParameters->min_area = -1;

				if (TempFindAllSizeSpots)
					TempBlobParameters->min_area = -1;
			}

			MVT_BLOB_RESULTS *TempBlobResults = mvt_blob_create_results(20, true, &TempErrorFlag);
			if (TempErrorFlag != IM_OK)
				ReportErrorMessage("MVTools Error 2112", cEMailInspx,32000);

			//start at the bottom ROI line and study every line up to the top ROI line
			if ((TempScratchImage) && (TempBlobParameters) && (TempBlobResults))
			for (WORD TempLineLoop = 0; TempLineLoop < TempHeight - TempMaxSpotSize - 2; TempLineLoop = TempLineLoop + vPixelsStepToNextROI)
			{
				//scanning from Left ROI Line to right, check each sub ROI for a spot darker than lowest value along edges
				for (WORD TempPixelLoop = 0; TempPixelLoop < TempWidth - TempMaxSpotSize - 2; TempPixelLoop = TempPixelLoop + vPixelsStepToNextROI)
				{
					//don't bother doing this ROI if you have all white in the middle
					bool TempSubROIIsNotSaturated = false;

					BYTE *TempPointer = TempScratchImage->pix + TempBottom + TempLineLoop + TempHalfMaxSpotSize + ((TempLeft + TempPixelLoop + TempHalfMaxSpotSize) * TempSizeX);
					BYTE *TempMaximumPointer = TempScratchImage->pix + TempSize;

					//Check 5 pixels around centroid to see if any data in this spot

					BYTE TempSaturationZone = 3; //was 5

					for (WORD TempLoop = 0; TempLoop < TempSaturationZone; TempLoop++)
					if (TempPointer < TempMaximumPointer)
					{
						if (*TempPointer++ < 230)
						{
							TempLoop = 50;
							TempSubROIIsNotSaturated = true;
						}
					}

					//Check 5 pixels around centroid to see if any data in this spot
					if (!TempSubROIIsNotSaturated)
					{
						TempPointer = TempPointer + TempSizeX - TempSaturationZone;
						if (TempPointer >= TempScratchImage->pix)
						for (WORD TempLoop = 0; TempLoop < TempSaturationZone; TempLoop++)
						if (TempPointer < TempMaximumPointer)
						{
							if (*TempPointer++ < 230)
							{
								TempLoop = 50;
								TempSubROIIsNotSaturated = true;
							}
						}
					}
					//Check 5 pixels around centroid to see if any data in this spot
					if (!TempSubROIIsNotSaturated)
					{
						TempPointer = TempPointer + TempSizeX - TempSaturationZone;
						for (WORD TempLoop = 0; TempLoop < TempSaturationZone; TempLoop++)
						if (TempPointer < TempMaximumPointer)
						{
							if (*TempPointer++ < 230)
							{
								TempLoop = 50;
								TempSubROIIsNotSaturated = true;
							}
						}
					}

					if (TempSubROIIsNotSaturated)
					if (TempLineLoop + TempMaxSpotSize < TempHeight)
					if (TempBottom + TempLineLoop + TempMaxSpotSize < TempSizeX)
					if (TempPixelLoop + TempMaxSpotSize < TempWidth)
					if (TempLeft + TempPixelLoop + TempMaxSpotSize < TempSizeY)
					{
						Image *TempSubDesensitizeImage = NULL;
						Image *TempSubImage = NULL; 
						Image *TempSubImage1 = im_rroi(TempScratchImage, TempBottom + TempLineLoop, TempLeft + TempPixelLoop, TempMaxSpotSize, TempMaxSpotSize);
						if (TempSubImage1)
							//TempErrorFlag = im_rroi_master(TempSubImage);
							TempSubImage = im_duplicate(TempSubImage1);

						if ((TempErrorFlag != IM_OK) || (!TempSubImage))
						{
							ReportErrorMessage("Bad Sub Image 45", cEMailInspx,32000);
						}
						else
						if ((TempSubImage) && (TempSubImage->pix))
						{
							BYTE TempMinimumEdgePixelValue = 255;
							BYTE *TempTopLinePointer = TempSubImage->pix + TempMaxSpotSize - 1;
							BYTE *TempBottomLinePointer = TempSubImage->pix;
							BYTE *TempRightEdgePointer = TempSubImage->pix + ((TempMaxSpotSize - 1) * TempSubImage->dy);
							BYTE *TempLeftEdgePointer = TempSubImage->pix;
							BYTE *TempMaximumSubImage = TempSubImage->pix + (TempMaxSpotSize * TempMaxSpotSize);
							TempSaturatedEdgePixelCount = 0;

							//test
							//BYTE TempPixels[200][200];
							//BYTE *TempPointer = TempSubImage->pix;
							//for (WORD TempScanLoopX = 0; TempScanLoopX < TempMaxSpotSize; TempScanLoopX++)
							//for (WORD TempScanLoopY = 0; TempScanLoopY < TempMaxSpotSize; TempScanLoopY++)
							//	TempPixels[TempScanLoopX][TempScanLoopY] = *TempPointer++;

							for (WORD TempScanLoop = 0; TempScanLoop < TempMaxSpotSize; TempScanLoop++)
							{
								if (vDesensitizeEdgePixels)
								{
									if (TempTopLinePointer < TempMaximumSubImage)
									if (*TempTopLinePointer > 250)
										TempSaturatedEdgePixelCount++;

									if (TempBottomLinePointer < TempMaximumSubImage)
									if (*TempBottomLinePointer > 250)
										TempSaturatedEdgePixelCount++;

									if (TempLeftEdgePointer < TempMaximumSubImage)
									if (*TempLeftEdgePointer > 250)
										TempSaturatedEdgePixelCount++;

									if (TempRightEdgePointer < TempMaximumSubImage)
									if (*TempRightEdgePointer > 250)
										TempSaturatedEdgePixelCount++;
								}

								if (TempTopLinePointer < TempMaximumSubImage)
								if (*TempTopLinePointer < TempMinimumEdgePixelValue)
									TempMinimumEdgePixelValue = *TempTopLinePointer;
								TempTopLinePointer = TempTopLinePointer + TempSubImage->dy;

								if (TempBottomLinePointer < TempMaximumSubImage)
								if (*TempBottomLinePointer < TempMinimumEdgePixelValue)
									TempMinimumEdgePixelValue = *TempBottomLinePointer;
								TempBottomLinePointer = TempBottomLinePointer + TempSubImage->dy;

								if (TempLeftEdgePointer < TempMaximumSubImage)
								if (*TempLeftEdgePointer < TempMinimumEdgePixelValue)
									TempMinimumEdgePixelValue = *TempLeftEdgePointer;
								TempLeftEdgePointer++;

								if (TempRightEdgePointer < TempMaximumSubImage)
								if (*TempRightEdgePointer < TempMinimumEdgePixelValue)
									TempMinimumEdgePixelValue = *TempRightEdgePointer;
								TempRightEdgePointer++;
							}

							BYTE TempDeltaThreshold = TempThreshold;
							//if (TempMinimumEdgePixelValue < 250)
							{
								if (vDesensitizeEdgePixels)
								//if (TempMinimumEdgePixelValue >= 225)
								if (TempSaturatedEdgePixelCount > vFindSpotInSubROI * .5)//brh to cover closer to edges make 0.3 and equal //was * 2.5)  //if more than half of the perimeter pixels are saturated
								if (TempDeltaThreshold + vDesensitizeEdgePixels < 255)
									TempDeltaThreshold = TempDeltaThreshold + vDesensitizeEdgePixels;
								else
									TempDeltaThreshold = 255;

								if ((vDesensitizeIrregularArea) && (vDesensitizeIrregularArea != 10))
								if (TempDesensitizeImage)  //for Desensitize Irregular Areas
								{
									BYTE *TempDesensitizePointer = TempDesensitizeImage->pix + TempBottom + TempLineLoop + TempHalfMaxSpotSize + 
										((TempLeft + TempPixelLoop + TempHalfMaxSpotSize) * TempSizeX);
									BYTE *TempMaximumDesensitizePointer = TempDesensitizeImage->pix + TempSize; 

									//average 5 pixels up and down around centroid to get amount to desensitize this Sub ROI spot find Threshold
									DWORD TempAverage = 0;
									for (WORD TempLoop = 0; TempLoop < 5; TempLoop++)
									if (TempDesensitizePointer < TempMaximumDesensitizePointer)
									{
										TempAverage = TempAverage + *TempDesensitizePointer++;
									}
									TempAverage = TempAverage / 5;

									if (TempDeltaThreshold + TempAverage < 255)
										TempDeltaThreshold = (BYTE)(TempDeltaThreshold + TempAverage);
									else
										TempDeltaThreshold = 255;
								}

								BYTE TempBlobThreshold = 0;
								if (TempMinimumEdgePixelValue > TempDeltaThreshold)
								{  //only continue if threshold is lower then darkest edge pixel
									TempBlobThreshold = TempMinimumEdgePixelValue - TempDeltaThreshold;

									ThresholdImage(TempSubImage, TempBlobThreshold);

									Erflag TempResult = mvt_blob_find(TempSubImage, TempBlobParameters, TempBlobResults, MVT_CF_DEFAULT);

									if ((WORD)TempBlobResults->num_blobs_found)
									{
										bool TempBlobOnEdge = false;
										for (WORD TempBlobLoop = 0; TempBlobLoop < TempBlobResults->num_blobs_found; TempBlobLoop++)
										{
											MVT_BLOB *TempBlob = TempBlobResults->blobs[TempBlobLoop];

											if (TempBlob)
											{
												if (TempBlob->xmin == 0)
													TempBlobOnEdge = true;
												if (TempBlob->xmax >= (WORD)(TempMaxSpotSize - 1))
													TempBlobOnEdge = true;
												if (TempBlob->ymin == 0)
													TempBlobOnEdge = true;
												if (TempBlob->ymax >= (WORD)(TempMaxSpotSize - 1))
													TempBlobOnEdge = true;
											}
										}
										//copy dark pixels into output image
										//if (!TempBlobOnEdge)
										{
											//BYTE *TempCurrentPixel = TempSubImage1->pix;
											for (BYTE TempSpotLineLoop = 0; TempSpotLineLoop < TempMaxSpotSize; TempSpotLineLoop++)
											{
												BYTE *TempResultPixel = TempImage + TempLineLoop + TempBottom + 
													((TempLeft + TempPixelLoop + TempSpotLineLoop) * TempSizeX);
												BYTE *TempMaximumResultPixel = TempImage + TempSize;
												
												BYTE *TempCurrentPixel = TempScratchImage->pix + TempLineLoop + TempBottom + 
														((TempLeft + TempPixelLoop + TempSpotLineLoop) * TempSizeX);
												BYTE *TempMaximumCurrentPixel = TempScratchImage->pix + TempSize;

												for (BYTE TempSpotPixelLoop = 0; TempSpotPixelLoop < TempMaxSpotSize; TempSpotPixelLoop++)
												if (TempResultPixel < TempMaximumResultPointer)
												if (TempCurrentPixel < TempMaximumCurrentPixel)
												{
													if (*TempCurrentPixel < TempBlobThreshold)
													{
														BYTE TempValue = (BYTE)(TempInspectionThreshold - (TempBlobThreshold - *TempCurrentPixel));
														if (*TempResultPixel > TempValue)  //if new value is small, shows spot is darker, so save it
															*TempResultPixel = TempValue;
													}

													TempCurrentPixel++;
													TempResultPixel++;
												}
											}
										}
									}
								}
							}
						}
						if (TempSubImage)
							im_delete(TempSubImage);
						if (TempSubImage1)
							im_delete(TempSubImage1);
					}
				} //end each pixel
			} //end each line

			if (TempBlobResults)
			{
				mvt_blob_delete_results(TempBlobResults);
				TempBlobResults = NULL;
			}

			if (TempBlobParameters)
			{
				mvt_blob_delete_params(TempBlobParameters);
				TempBlobParameters = NULL;
			}
#endif
		}
	}
}

void CInspection::ErodeEdgesOfContainer(tImage *TempImage, WORD TempSizeX, WORD TempSizeY)
{
	Image *TempImageIn = NULL;
	Image *TempImageOut = NULL;
	DWORD TempSize = TempSizeX * TempSizeY;
	if (ThereIsEnoughMemory(TempSize + sizeof(Image), "Temporary Image for erode contaminant analysis"))
		TempImageIn = im_create(IM_BYTE, TempSizeX, TempSizeY);
	if (ThereIsEnoughMemory(TempSize + sizeof(Image), "Temporary Image for erode contaminant analysis"))
		TempImageOut = im_create(IM_BYTE, TempSizeX, TempSizeY);

	if ((TempImage->pix) && (TempImage) && (TempImageIn) && (TempImageIn->pix) && (TempImageOut) && (TempImageOut->pix))
	{
		CopyMemory(TempImageIn->pix, TempImage->pix, TempSize);
		int TempError = mvt_threshold(TempImageIn,254,0,255);  //this makes the product black and background white
		if (TempError != IM_OK)
			ReportErrorMessage("MVT_Threshold Error",cEMailInspx,32000);

		for (BYTE TempLoop = 0; TempLoop < vErodeCount; TempLoop++)
		{
			DilateImage(TempImageIn, TempImageOut);
		}

		//if (!vGlobalUseHalconLibrary)
		{
#ifdef UseIFCDLLs
			Erflag TempResultError = mvt_or(TempImageIn,TempImage);
			if (TempResultError != IM_OK)
				ReportErrorMessage("Error, MVT Dilate Failed", cEMailInspx,32000);
#endif
		}
//#ifdef UseHalconLibraries
//		else
//		{
//			HByteImage TempHalconImageIn = MakeHalconImage(TempImageIn);
//			HByteImage TempHalconImageIn1 = MakeHalconImage(TempImage);
//			HByteImage TempHalconImageOut = MakeHalconImage(TempImageOut);
//
//			TempHalconImageOut = TempHalconImageIn | TempHalconImageIn1;
//
//			CopyFromHalconImage(TempHalconImageOut, TempImage);
//
//		}
//#endif
	}
	if (TempImageIn)
		im_delete(TempImageIn);
	if (TempImageOut)
		im_delete(TempImageOut);
}

void CInspection::MakeVariablityImage(tImage *TempImage, Image *TempScratchImage, WORD TempSizeX, WORD TempSizeY, BYTE TempGoToStep)
{
	BYTE TempPixelsToAverageOneSide = 4;  //pixels to average on each side, so double this amount is actually averaged
	BYTE TempPixelsToAverage = 8;  //pixels to average on each side, so double this amount is actually averaged
	DWORD TempSize = TempSizeX * TempSizeY;

	if ((TempImage) && (TempImage->pix) && (TempScratchImage) && (TempScratchImage->pix))
	{
		CopyMemory(TempScratchImage->pix, TempImage->pix, TempSize);

		//for every line in the image
		for (WORD TempLoopX = 0; TempLoopX < TempSizeX; TempLoopX++)
		{
			BYTE *TempDestinationPointer = TempScratchImage->pix + TempLoopX;
			//zero the first n pixels on this line
			for (WORD TempLoopY = 0; TempLoopY < TempPixelsToAverageOneSide; TempLoopY++)
			{
				*TempDestinationPointer = 0;
				TempDestinationPointer = TempDestinationPointer + TempSizeX;
			}
			//for every pixel on this line
			//find the average for the n pixels around this one and take difference of this pixel to the average
			for (WORD TempLoopY = 0; TempLoopY < TempSizeY - TempPixelsToAverage; TempLoopY++)
			{
			/*
				BYTE *TempPointer = TempImage->pix + TempLoopX + (TempSizeX * TempLoopY);
				bool TempHaveData = false;
				for (WORD TempLoopSum = 0; TempLoopSum < TempPixelsToAverage; TempLoopSum++)
				{
					if (*TempPointer++ < 255)
					{
						TempHaveData = true;
						TempLoopSum = TempPixelsToAverage;
					}
				}

				if (TempHaveData)
				{
				*/
					BYTE *TempPointer = TempImage->pix + TempLoopX + (TempSizeX * TempLoopY);
					WORD TempSum = 0;
					//sum up n pixels on line
					for (WORD TempLoopSum = 0; TempLoopSum < TempPixelsToAverage; TempLoopSum++)
					{
						TempSum = TempSum + *TempPointer;
						TempPointer = TempPointer + TempSizeX;
						if (TempLoopSum == TempPixelsToAverageOneSide)
							TempPointer = TempPointer + TempSizeX; //skip over this pixel itself
					}
					TempSum = TempSum / TempPixelsToAverage;  //calculate average of sum of 10 pixels

					//find difference of pixel and average of n pixels around
					if (*TempDestinationPointer > TempSum)
						*TempDestinationPointer = *TempDestinationPointer - TempSum;
					else
						*TempDestinationPointer = TempSum - *TempDestinationPointer;

					//square the resulting difference
					//if (*TempDestinationPointer >= 16)
					//	*TempDestinationPointer = 255;
					//else
					//	*TempDestinationPointer = *TempDestinationPointer * *TempDestinationPointer;  //take square of difference
			//	}

				TempDestinationPointer = TempDestinationPointer + TempSizeX;
			}
			//set last few pixels to zero since don't have enough pixels to average
			for (WORD TempLoopY = 0; TempLoopY < TempPixelsToAverageOneSide; TempLoopY++)
			{
				*TempDestinationPointer = 0;
				TempDestinationPointer = TempDestinationPointer + TempSizeX;
			}
		}
			//copy resultant image back to the original or output or further analysis
		CopyMemory(TempImage->pix, TempScratchImage->pix, TempSize);
		if (TempGoToStep > 1)
		{ //step 2 or higher
			//step 2 is to average the resultant differences image
			BYTE TempPixelsToAverageOneSide = 10;  //pixels to average on each side, so double this amount is actually averaged
			BYTE TempPixelsToAverage = 20;  //pixels to average on each side, so double this amount is actually averaged

			//for every line in the image
			for (WORD TempLoopX = 0; TempLoopX < TempSizeX; TempLoopX++)
			{
				//for every pixel on the line
				for (WORD TempLoopY = 0; TempLoopY < TempSizeY - TempPixelsToAverage; TempLoopY++)
				{
					/*
					BYTE *TempPointer = TempImage->pix + TempLoopX + (TempSizeX * TempLoopY);
					bool TempHaveData = false;
					for (WORD TempLoopSum = 0; TempLoopSum < TempPixelsToAverage; TempLoopSum++)
					{
						if (*TempPointer++ < 255)
						{
							TempHaveData = true;
							TempLoopSum = TempPixelsToAverage;
						}
					}

					if (TempHaveData)
					{
					*/
						BYTE *TempDestinationPointer = TempScratchImage->pix + TempLoopX + (TempSizeX * (TempLoopY + TempPixelsToAverageOneSide));
						BYTE *TempPointer = TempImage->pix + TempLoopX + (TempSizeX * TempLoopY);

						WORD TempSum = 0;
						//sum up n pixels on line
						//find the average for the n pixels around this one and itself
						for (WORD TempLoopSum = 0; TempLoopSum <= TempPixelsToAverage; TempLoopSum++)
						{
							TempSum = TempSum + *TempPointer;
							TempPointer = TempPointer + TempSizeX;
							//if (TempLoopSum == TempPixelsToAverageOneSide)
							//	TempPointer = TempPointer + TempSizeX; //skip over this pixel
						}
						*TempDestinationPointer = (BYTE)(TempSum / TempPixelsToAverage);  //calculate average of sum of n pixels
					//}
				}
			}
			//copy averaged differences image back to the original or output or further analysis
			CopyMemory(TempImage->pix, TempScratchImage->pix, TempSize);
		}
	}
	if (TempGoToStep > 2)
	if (vDesensitizeIrregularArea != 10)//don't need to scale if scale factor is 1
	if (vDesensitizeIrregularArea)
	{ //scale image by a tenth of the amount of vDesensitizeIrregularArea
		double TempFactor = vDesensitizeIrregularArea / 10.0;
		BYTE *TempPointer = TempImage->pix;
		for (DWORD TempLoop = 0; TempLoop < TempSize; TempLoop++)
		{
			*TempPointer = (BYTE)(*TempPointer * TempFactor);
			TempPointer++;
		}
	}
}

void CInspection::FillBackground(BYTE *TempImage, WORD TempSizeX, WORD TempSizeY)
{
	if (vFillBackground)
	if ((TempImage) && (TempImage))
	{
		DWORD TempSize = TempSizeX * TempSizeY;
		BYTE *TempSourcePointer = TempImage;
		for (DWORD TempLoop = 0; TempLoop < TempSize; TempLoop++)
		{
			if (*TempSourcePointer > cBackgroundThreshold)
				*TempSourcePointer = vFillBackground;
			TempSourcePointer++;
		}
	}
}


void CInspection::TakeDifferenceFromMedianSingleMatrix(Image *TempImage, WORD TempSizeX, WORD TempSizeY, BYTE TempMatrixSizeX, BYTE TempMatrixSizeY)
{ //TakeDifferenceOfPixelToMedian //bones2 algorithm
	if (TempImage)
	{
		DWORD TempSize = TempSizeX * TempSizeY;

		Image *TempImageToProcess = NULL;
		if (ThereIsEnoughMemory(TempSize + sizeof(Image), "Temporary Image for Find Bones"))
			TempImageToProcess = im_create(IM_BYTE, TempSizeX, TempSizeY);

		if ((TempImageToProcess) && (TempImageToProcess->pix))
		{
			//fill horizontal, then fill vertical, then combine, then fill horizontal again to get a complete fill
			CopyMemory(TempImageToProcess->pix, TempImage->pix, TempSize);

			DoMedianSmooth(TempImageToProcess, TempMatrixSizeY, TempMatrixSizeX);

			//subtract the original image from the Median image
			BYTE *TempSource = TempImage->pix;
			BYTE *TempDestination = TempImageToProcess->pix;
			for (DWORD TempLoop = 0; TempLoop < TempSize; TempLoop++)
			{
				if (*TempSource < *TempDestination)  //if this pixel is darker than the median of this pixel then take the difference
				{
					*TempDestination = 255 - (BYTE)((*TempDestination - *TempSource)); // if * TempScaleFactor);
				}
				else
					*TempDestination = 255;

				TempSource++;
				TempDestination++;
			}
			CopyMemory(TempImage->pix, TempImageToProcess->pix, TempSize);
		}
		if (TempImageToProcess)
			im_delete(TempImageToProcess);
	}
}

void CInspection::FindDarkSpotSingleMatrix(Image *TempImage, WORD TempSizeX, WORD TempSizeY, BYTE TempMatrixSize, BYTE TempThreshold, BYTE *TempMaskImage)
{ //looking for bones
	BYTE TempThresholdInverted = 255 - TempThreshold;
	//if (TempThresholdInverted > 5)
	//	TempThresholdInverted = TempThresholdInverted - 4;

	//BYTE TempMinimumBoneWidth = TempMatrixSize / 4;
	//if (TempMinimumBoneWidth < 2)
	//	TempMinimumBoneWidth = 2;
	BYTE TempMinimumBoneWidth = 0;

	if ((!vPixelsStepToNextROI) || (vPixelsStepToNextROI >= TempMatrixSize))
	{
		vPixelsStepToNextROI = 2;
		if (TempMatrixSize > 5)
			vPixelsStepToNextROI = 3;
	}

	if (TempImage)
	{
		DWORD TempSize = TempSizeX * TempSizeY;

		Image *TempImageToProcess = NULL;
		if (ThereIsEnoughMemory(TempSize + sizeof(Image), "Temporary Image for Find Bones"))
			TempImageToProcess = im_create(IM_WORD, TempSizeX, TempSizeY);
		BYTE *TempMaskImageMask = NULL;
		BYTE *TempMaskImageMaskPlus1 = NULL;
		/*
//test
CTime TempTime = CTime::GetCurrentTime();
CString TempTimeString = TempTime.Format("%H-%M-%S");//24 hour time format

CString TempImageFileName = "C:\\ScanTracLogs\\Chicken Bones-" + TempTimeString + ".csv";
LPCTSTR TempFileName = TempImageFileName;
CFileException TempFileException;
CFile *vDataFile;
vDataFile = new CFile;
int TempFileOpened = vDataFile->Open(TempFileName, (CFile::modeCreate | CFile::modeReadWrite), &TempFileException);
//write the image data to the file
CString TempString("");
TempString = "";
//end test
*/
		BYTE TempOutputImageOffset = vPixelsStepToNextROI; //((vPixelsStepToNextROI) / 2) + 1;
		if ((TempImageToProcess) && (TempImageToProcess->pix))
		{
			//fill horizontal, then fill vertical, then combine, then fill horizontal again to get a complete fill
			//FillMemory(TempImageToProcess->pix, TempSize, 255);
			WORD *TempPointer = (WORD *)TempImageToProcess->pix;
			for (DWORD TempLoopD = 0; TempLoopD < TempSize; TempLoopD++) //scan all lines
				*TempPointer++ = 255;
/*
//test
for (WORD TempLoopY = 0; TempLoopY < TempSizeX; TempLoopY++) //scan all lines
	TempString = TempString + dtoa(TempLoopY,0) + ',';

TempString = TempString + "Line Number" + ',';
TempString = TempString + "\n";
vDataFile->Write(TempString.GetBuffer(TempString.GetLength()), TempString.GetLength());
//end test
*/

			for (WORD TempLoopY = 0; TempLoopY < TempSizeY; TempLoopY++) //scan all lines
			{
// test
//TempString = "Line " + dtoa(TempLoopY,0) + ',';

				//compare each pixel with next pixel and put amount darker than in original image (TempImage)
				BYTE *TempSource = TempImage->pix + (TempLoopY * TempSizeX) + 1; //skip the first pixel on the line as it is the A,B,C,D Frame Pixel
				BYTE *TempSourcePlus2 = TempSource + vPixelsStepToNextROI;
				WORD *TempDestination = (WORD *)(TempImageToProcess->pix) + (TempLoopY * TempSizeX) + TempOutputImageOffset;
				if (TempMaskImage)
				{
					TempMaskImageMask = TempMaskImage + (TempLoopY * TempSizeX);
					TempMaskImageMaskPlus1 = TempMaskImageMask + vPixelsStepToNextROI;
				}

				for (WORD TempLoopX = 1; TempLoopX < TempSizeX - vPixelsStepToNextROI; TempLoopX++) //scan each pixel in this line
				{
// test
//TempString = TempString + ByteToHex(*TempSource) + ',';

					//if next pixel is the same or is masked off, mark no delta
					if ((*TempSource == 255) || ((TempMaskImageMask) && ((*TempMaskImageMask == 255) || (*TempMaskImageMaskPlus1 == 255))))
						*TempDestination = 255;
					else
					if (*TempSource == *TempSourcePlus2)
						*TempDestination = 0;
					else
					if (*TempSource > *TempSourcePlus2)  //if there is a transition getting darker record amount darker by
					{
						int TempDelta = (*TempSource - *TempSourcePlus2);// * 5;// * 512.0 / *TempSource;  //multiply by 2 if white, multiply by 4 if 128
/*
						if (vMaximumScaleFactor > 1)
						{
							if (vMaximumScaleFactor > 10)
								vMaximumScaleFactor = 0;
							else
							{
								double TempScaleFactor = 255.0 / ((*TempSourcePlus2 + *TempSource) / 2.0);
								if (TempScaleFactor > vMaximumScaleFactor)
									TempScaleFactor = vMaximumScaleFactor;

								TempDelta = (int)((double)(TempDelta) * TempScaleFactor);
							}
						}
						*/

						if (TempDelta > TempThresholdInverted)  //if the delta is more than the threshold, save it
						{
							*TempDestination = (BYTE)TempDelta;
						}
						else
							*TempDestination = 0;
					}
					else //if there is a transition getting lighter, mark amount getting lighter by and set the ms bit
					{
						int TempDelta = (*TempSourcePlus2 - *TempSource);// * 255.0 / *TempSourcePlus2;//multiply by 1 if white, multiply by 2 if 128
/*
						if (vMaximumScaleFactor > 1)
						{
							if (vMaximumScaleFactor > 10)
								vMaximumScaleFactor = 0;
							else
							{
								double TempScaleFactor = 255.0 / ((*TempSourcePlus2 + *TempSource) / 2.0);
								if (TempScaleFactor > vMaximumScaleFactor)
									TempScaleFactor = vMaximumScaleFactor;

								TempDelta = (int)((double)(TempDelta) * TempScaleFactor);
							}
						}
						*/

						if (TempDelta > TempThresholdInverted)  //if the delta is more than the threshold, save it
						{
							*TempDestination = TempDelta + 0x8000; //mark that this is a negitive transition
						}
						else
							*TempDestination = 0;
					}
// test
//TempString = TempString + WordToHex(*TempDestination) + ',';

					TempSource++;
					TempSourcePlus2++;
					TempDestination++;
					if (TempMaskImageMask)
					{
						TempMaskImageMask++;
						TempMaskImageMaskPlus1++;
					}
				}
				//look for matching positive with negitive transitions, otherwise, blank out
				WORD *TempSourceW = (WORD *)(TempImageToProcess->pix) + (TempLoopY * TempSizeX);
				BYTE *TempDestinationB = TempImage->pix + (TempLoopY * TempSizeX);
				BYTE *TempDestinationBMaximum = TempImage->pix + TempSize;

				for (WORD TempLoopX = 0; TempLoopX < TempSizeX - vPixelsStepToNextROI; TempLoopX++)
				{
// test
//bool TempWrite = true;

					if (*TempSourceW == 255) //if it was background ignore it
						*TempDestinationB = 255;
					else
					if (*TempSourceW == 0) //if it was no change, clear output pixel
						*TempDestinationB = 255;
					else
					if (*TempSourceW & 0x8000) //if it was a transition getting brighter without a transition getting darker first, clear it
						*TempDestinationB = 255;
					else
					{//found a transition getting darker, so look for matching lightening transition
						WORD *TempPixelToCheck = TempSourceW + 1;
						WORD TempLargestUpTransition = *TempSourceW;
						WORD TempLargestDownTranstion = 0;
						WORD TempBoneLength = 0;
						WORD TempDetectedBone = 0;

						WORD TempMaximumSize = TempMatrixSize;
						if (TempMaximumSize + TempLoopX + vPixelsStepToNextROI >= TempSizeX)
						{
							TempMaximumSize = TempSizeX - TempLoopX - vPixelsStepToNextROI;
							if (TempMaximumSize > TempMatrixSize)
								TempMaximumSize = 0;
						}

						for (BYTE TempLoopPixel = 0; TempLoopPixel < TempMaximumSize; TempLoopPixel++)
						{
							TempBoneLength++;
							if (*TempPixelToCheck == 255) //if it was a background pixel
							{
								//if (TempDetectedBone)
								//	TempLoopPixel = TempMatrixSize; // detected bone, and not going down anymore, so exit and mark bone
								TempDetectedBone = false;
								TempLoopPixel = TempMatrixSize; // detected bone, and not going down anymore, so exit and mark bone
							}
							else
							if (*TempPixelToCheck & 0x8000) //found a transition getting brighter
							{
								if (TempBoneLength >= TempMinimumBoneWidth)
								{ //found bone, mark it
									if ((*TempPixelToCheck & 0xFF) > TempLargestDownTranstion)
									{ //if stronger edge, then mark it as the edge
										TempLargestDownTranstion = *TempPixelToCheck & 0xFF;
										TempDetectedBone = TempBoneLength;
									}
								}
								else
								{
									TempLoopPixel = TempMatrixSize; // going down to soon, exit did not find bone
								}
							}
							else
							if (*TempPixelToCheck) //if going up
							{
								if (TempDetectedBone)
									TempLoopPixel = TempMatrixSize; // detected bone, and not going down anymore, so exit and mark bone
								else
								if (*TempPixelToCheck > TempLargestUpTransition)
									//TempLargestUpTransition = *TempPixelToCheck & 0xFF;
									//found a stronger edge, so use it instead of the weaker one, so abort checking further on this pixel this pixel
									TempLoopPixel = TempMatrixSize; // detected bone, and not going down anymore, so exit and mark bone
							}
							TempPixelToCheck++;
						}
						if (!TempDetectedBone)
							*TempDestinationB = 255; //did not detect a bone, so clear this pixel
						else
						{//detected a bone, so mark OK to the end
							if (TempLargestUpTransition < TempLargestDownTranstion)
								TempLargestUpTransition = TempLargestDownTranstion;

							TempLargestUpTransition = 255 - TempLargestUpTransition;

							for (BYTE TempLoopPixel = 0; TempLoopPixel < TempDetectedBone; TempLoopPixel++)
							{
								if (TempDestinationB < TempDestinationBMaximum)
									*TempDestinationB = (BYTE)TempLargestUpTransition;
//test
//TempString = TempString + ByteToHex(*TempDestinationB) + ',';
								// try
								//*TempSource = 255 - (*TempSource & 0xFF);
								TempSourceW++;
								TempDestinationB++;
								TempLoopX++; //increment the X pixel counter for this line as skipping this pixel for checking as already marked it
//test
//if (TempLoopX >= TempSizeX - 1)
//TempWrite = false;
							}
							if (TempDestinationB < TempDestinationBMaximum)
								*TempDestinationB = 255; //blank the pixel after the bone
						}
					}
//test
//if (TempWrite)
//TempString = TempString + ByteToHex(*TempDestinationB) + ',';
					TempSourceW++;
					TempDestinationB++;
				}
				for (WORD TempLoopX = TempSizeX - vPixelsStepToNextROI; TempLoopX < TempSizeX; TempLoopX++)
				if (TempDestinationB < TempDestinationBMaximum)
					*TempDestinationB++ = 255;
/*		
//test
TempString = TempString + "Line " + dtoa(TempLoopY,0) + ',';
TempString = TempString + "\n";
vDataFile->Write(TempString.GetBuffer(TempString.GetLength()), TempString.GetLength());
*/			
				//end of looking for matching pairs of transitions
			}
/*		
// test
vDataFile->Close();
delete vDataFile;
vDataFile = NULL;
//end  test
*/
		}
		if (TempImageToProcess)
			im_delete(TempImageToProcess);
	}
}

void CInspection::TakeDifferenceFromMedian(BYTE *TempImage, Image *TempScrachImage, WORD TempSizeX, WORD TempSizeY, BYTE *TempOriginalImage, BYTE TempThreshold)
{ //TakeDifferenceOfPixelToMedian //bones2 algorithm using Median Filter in both directions, or new difference algorithm if MSBit is set
	BYTE TempMatrixSize = vTakeDifferenceFromMedian & 0x7F;
	BYTE TempDifferenceAlgorithm = vTakeDifferenceFromMedian & 0x80;

//keep copy of original
//erode a copy n time to fill in edges, to original, add eroded on if pixel is < 230 and eroded one is > 250, else set to default
//when done, mask off any data where pixel in original is > 230

	if (TempMatrixSize)
	if (TempImage) //TempImage already has background filled if configured, and median filtered if configured
	{
		DWORD TempSize = TempSizeX * TempSizeY;
		Image *TempImageMask = NULL;
		Image *TempImageOutput = NULL;

		Image *TempImageToProcess2 = NULL;
		if (ThereIsEnoughMemory(TempSize + sizeof(Image), "Temporary Image for Find Bones"))
			TempImageToProcess2 = im_create(IM_BYTE, TempSizeX, TempSizeY);

		if ((TempImageToProcess2) && (TempImageToProcess2->pix))
		if ((TempScrachImage) && (TempScrachImage->pix))
		{ // two matrixes
			CopyMemory(TempScrachImage->pix, TempImage, TempSize);

			if ((vFillBackground) && (!TempDifferenceAlgorithm))  //take original image, fill background if configured for median filter of higer value.
				FillBackground(TempScrachImage->pix, TempSizeX, TempSizeY);

			CopyMemory(TempImageToProcess2->pix, TempScrachImage->pix, TempSize);

			if (!TempDifferenceAlgorithm) //not used anymore
			{
				TakeDifferenceFromMedianSingleMatrix(TempScrachImage, TempSizeX, TempSizeY, TempMatrixSize, 1);

				TakeDifferenceFromMedianSingleMatrix(TempImageToProcess2, TempSizeX, TempSizeY, 1, TempMatrixSize);
			}
			else
			{
				//create mask for checking edges if need to 
				BYTE *TempMaskToUse = NULL;
				if (vIgnoreEdgePixels)
				if (TempOriginalImage)
				{
					if (ThereIsEnoughMemory(TempSize + sizeof(Image), "Temporary Image for Find Bones"))
						TempImageOutput = im_create(IM_BYTE, TempSizeX, TempSizeY);

					if (ThereIsEnoughMemory(TempSize + sizeof(Image), "Temporary Image for Find Bones"))
						TempImageMask = im_create(IM_BYTE, TempSizeX, TempSizeY);

					//trim vIgnoreEdgePixels more pixels off at edge of product
					if (TempImageOutput)
					if (TempImageMask)
					{
						CopyMemory(TempImageMask->pix, TempOriginalImage, TempSize);

						ThresholdImage(TempImageMask, 250);

						for (WORD TempLoop = 0; TempLoop < vIgnoreEdgePixels; TempLoop++)
						{
							DilateImage(TempImageMask, TempScrachImage);
						}
						TempMaskToUse = TempImageMask->pix;
					}
				}
				//scan up and down
				FindDarkSpotSingleMatrix(TempScrachImage, TempSizeX, TempSizeY, TempMatrixSize, TempThreshold, TempMaskToUse);
				//test only blank this direction FillMemory(TempScrachImage->pix, TempSize, 255);

				//scan side to side
#ifdef UseIFCDLLs
				mvt_rotate90(TempImageToProcess2, 1);
				if (TempImageMask)
				{
					RotateImage90Degrees(TempImageMask, 1);
					TempMaskToUse = TempImageMask->pix;
				}

				FindDarkSpotSingleMatrix(TempImageToProcess2, TempSizeY, TempSizeX, TempMatrixSize, TempThreshold, TempMaskToUse);
				RotateImage90Degrees(TempImageToProcess2, -1);
				//test only blank this direction FillMemory(TempImageToProcess2->pix, TempSize, 255);
#endif
			}
			BYTE *TempSource = TempScrachImage->pix;
			BYTE *TempSource2 = TempImageToProcess2->pix;
			BYTE *TempDestination = TempImage;
			for (DWORD TempLoop = 0; TempLoop < TempSize; TempLoop++)
			{
				if (*TempSource < *TempSource2)  //take the smallest value of the two
					*TempDestination = *TempSource;
				else
					*TempDestination = *TempSource2;

				TempSource++;
				TempSource2++;
				TempDestination++;
			}
			//blank out any area that was above the background threshold in the original image
			if (!TempDifferenceAlgorithm)
			if (vIgnoreEdgePixels)
			if (TempOriginalImage)
			{
				//trim more pixels off at edge of product
				CopyMemory(TempImageToProcess2->pix, TempOriginalImage, TempSize);
				for (DWORD TempLoop = 0; TempLoop < vIgnoreEdgePixels; TempLoop++)
				{
					DilateImage(TempImageToProcess2, TempScrachImage);
				}

				BYTE *TempSource = TempScrachImage->pix;
				BYTE *TempDestination = TempImage;
				for (DWORD TempLoop = 0; TempLoop < TempSize; TempLoop++)
				{
					if (*TempSource > cBackgroundThreshold)
					{
						*TempDestination = 255;
					}
					TempSource++;
					TempDestination++;
				}
			}

			//do a dilate/erode to try to connect over the gaps in the thin bone detection
			//CopyMemory(TempImageToProcess2->pix, TempImage, TempSize);

			//ErodeImage(TempImageToProcess2);

			//DilateImage(TempImageToProcess2);

			//CopyMemory(TempImage, TempImageToProcess2->pix, TempSize);
		}
		if (TempImageOutput)
			im_delete(TempImageOutput);
		if (TempImageMask)
			im_delete(TempImageMask);
		if (TempImageToProcess2)
			im_delete(TempImageToProcess2);
	}
}

//Latvian Algorithm

/**
 * Calculation of thresholds from one "Healthy Choice Good" image (pixels represented by bytes)
 * representing "Healthy Choice" package without contaminants.<p>
 * Detects minimum pixel values after background filtering and adjusts old thresholds if appropriate.<p>
 *
 * Usage scenario:
 * - set all thresholds to 0 using "Contaminant22SetThresholds";
 * - in a loop, call "...ThresholdCalculation..." for all available images without contaminants.
 *	\param Image 	pointer to input image (<code>rowsIn*columnsIn</code>), placed in memory by rows.
 *	\param SizeX	number of rows in input image 
 *	\param SizeY 	number of columns in input image 
 *  \return result code: 0: OK; -1: memory not allocated; -5: invalid size parameter   
 *  \sa InspxContaminant22ThresholdCalculationW(), InspxGetErrorText() 
 */
int CInspection::InspxContaminant22ThresholdCalculationB(BYTE *Image, WORD SizeX, WORD SizeY, int k, WORD n)
{
	int N = ((int)SizeX) * ((int)SizeY);
	
	// check memory allocation
	if (_MinimaColumnsDown==NULL) return INSPX_ERROR_MEMORY;
	
	// check arguments
	if (Image==NULL) return INSPX_ERROR_MEMORY;
	if (SizeX<_rowsEdge) return INSPX_ILLEGAL_IMAGE_SIZE;
	if (SizeY<_rowsEdge) return INSPX_ILLEGAL_IMAGE_SIZE;
	if (SizeX>_maxImageSize) return INSPX_ILLEGAL_IMAGE_SIZE;
	if (SizeY>_maxImageSize) return INSPX_ILLEGAL_IMAGE_SIZE;
	if (N>((int)_maxPixels)) return INSPX_ILLEGAL_IMAGE_SIZE;
	
	// convert to WORD image
	inspxImageB2W(ImageTmpL1[k], Image, N);
	
	// call thresholds calculation and update routine
	return inspxUpdateThresholds(ImageTmpL1[k], (int)SizeX, (int)SizeY, _processEdges, n);
}

/**
 * Calculation of thresholds from one "Healthy Choice Good" image (pixels represented by words). See InspxContaminant22ThresholdCalculationB().
 */
int CInspection::InspxContaminant22ThresholdCalculationW(WORD *Image, WORD SizeX, WORD SizeY)
{
	int N = ((int)SizeX) *((int)SizeY);

	// check memory allocation
	if (_MinimaColumnsDown==NULL) return INSPX_ERROR_MEMORY;
	
	// check arguments
	if (Image==NULL) return INSPX_ERROR_MEMORY;
	if (SizeX<_rowsEdge) return INSPX_ILLEGAL_IMAGE_SIZE;
	if (SizeY<_rowsEdge) return INSPX_ILLEGAL_IMAGE_SIZE;
	if (SizeX>_maxImageSize) return INSPX_ILLEGAL_IMAGE_SIZE;
	if (SizeY>_maxImageSize) return INSPX_ILLEGAL_IMAGE_SIZE;
	if (N>((int)_maxPixels)) return INSPX_ILLEGAL_IMAGE_SIZE;

	// call thresholds calculation and update routine
	return inspxUpdateThresholds(Image, (int)SizeX, (int)SizeY, _processEdges, (WORD)1);
}

/**
 * Detection of contaminants in "Healthy Choice" images.
 * Image pixels are represented by bytes.
 *
 * Returns number of detected contaminant pixels (if >0) or error code (0 if OK but no pixels detected, <0 - error), 
 * marks detected contaminant pixels in "Image" using values INSPX_MAX_PIXEL_VALUE/256.
 *
 *	\param Image 	pointer to input image (<code>rowsIn*columnsIn</code>), placed in memory by rows.
 *	\param SizeX	number of rows in input image 
 *	\param SizeY 	number of columns in input image 
 *  \return result code: >=0: OK, number of contaminant pixels found;  -1: memory not allocated; -5: invalid size parameter   
 */
/*
Algorithm is based on separate processing of the image lines by rows and columns and logically combining the results. Processing includes filtering out image background with different filters and performing thresholding with different thresholds for results obtained with different background filters. Thresholds are calculated in advance by applying the same procedures for images known to be "good", i.e. taken from samples without contaminants. 

Usage scenario: 

(optional) set values of variables acting as algorithm parameters: "BackgroundThreshold", "MinSizeX", "MinSizeY", "RowsEdge", "ProcessEdges" 
invoke "CreateBuffers" function to prepare memory buffers 
(optional) prepare threshold values in the following alternative ways: 
invoke "SetThresholds" with threshold values prepared before 
invoke "SetThresholds" with 0 values followed by multiple use of "...ThresholdCalculation..." for preparing new threshold values from available images without contaminants 

perform contaminant detection using "...Inspection..." routine (multiple times) 
free memory by using "FreeBuffers" function 
Default parameters of all variables and thresholds are defined in DLL so that setting their values is optional.
*/
int CInspection::InspxContaminant22InspectionB(BYTE *Image, WORD SizeX, WORD SizeY, BYTE tTh, int k)
{
	int res, nrDetected, N = ((int)SizeX) * ((int)SizeY);
	//double timer0, timer;

	//timer0 = Timer();

	// check memory allocation
	if (_MinimaColumnsDown==NULL) 
		return INSPX_ERROR_MEMORY;
	
	// check arguments
	if (Image==NULL) 
		return INSPX_ERROR_MEMORY;

	if (SizeX<_rowsEdge) 
		return INSPX_ILLEGAL_IMAGE_SIZE;

	if (SizeY<_rowsEdge) 
		return INSPX_ILLEGAL_IMAGE_SIZE;

	if (SizeX>_maxImageSize) 
		return INSPX_ILLEGAL_IMAGE_SIZE;

	if (SizeY>_maxImageSize) 
		return INSPX_ILLEGAL_IMAGE_SIZE;

	if (N>((int)_maxPixels)) 
		return INSPX_ILLEGAL_IMAGE_SIZE;
	
	BYTE *TempPointer = Image;
	for (int TempLoop = 0; TempLoop < N; TempLoop++) 
	{
		if (*TempPointer == 255) //contaminant 22 algorithm marks contaminants in white (255)
			*TempPointer = 254;

		if (*TempPointer < 2) //don't have any value less than 5
			*TempPointer = 2;	

		*TempPointer++;
	}

	// convert to WORD image
	inspxImageB2W(ImageTmpL1[k], Image, N);
	
	_processEdges = vTrimEdgesOfJar;
	// call detection routine
	res = inspxDetect(ImageTmpL2[k], &nrDetected, ImageTmpL1[k], (int)SizeX, (int)SizeY, _processEdges, tTh, k);
	
	// convert back to BYTE image
	inspxImageW2B(Image, ImageTmpL2[k], N);
	//inspxImageW2B(Image, ImageTmpL1, N);

	TempPointer = Image;
	for (int TempLoop = 0; TempLoop < N; TempLoop++) 
	{
		if (*TempPointer == 255) //contaminant 22 algorithm marks contaminants in white (255) so convert to black (0)
			*TempPointer = 0;  //marks a contaminant
		else 
			*TempPointer = 255;//marks pixel ok
		*TempPointer++;
	}

	if (res < 0) return res;
	
	//timer = Timer();
	//printf("Detection time=%6.3fs, nrDetected=%i\n",timer-timer0,nrDetected);

	//return = 0 when good image
	//return < 0 when error
	//return > is number of bad pixels found
	
	return nrDetected;
}

/**
 * Detection of contaminants in "Healthy Choice" images.
 * Image pixels are represented by words. See InspxContaminant22InspectionB(). 
 */
int CInspection::InspxContaminant22InspectionW(WORD *Image, WORD SizeX, WORD SizeY)
{
	int res, nrDetected, N=((int)SizeX)*((int)SizeY);
	//double timer0, timer;

	//timer0 = Timer();
	
	// check memory allocation
	if (_MinimaColumnsDown==NULL) return INSPX_ERROR_MEMORY;
	
	// check arguments
	if (Image==NULL) return INSPX_ERROR_MEMORY;
	if (SizeX<_rowsEdge) return INSPX_ILLEGAL_IMAGE_SIZE;
	if (SizeY<_rowsEdge) return INSPX_ILLEGAL_IMAGE_SIZE;
	if (SizeX>_maxImageSize) return INSPX_ILLEGAL_IMAGE_SIZE;
	if (SizeY>_maxImageSize) return INSPX_ILLEGAL_IMAGE_SIZE;
	if (N>((int)_maxPixels)) return INSPX_ILLEGAL_IMAGE_SIZE;

	// call detection routine
	res = inspxDetect(ImageTmpL2[0], &nrDetected, Image, (int)SizeX, (int)SizeY, _processEdges, (BYTE)254, 0);	 
	if (res<0) return res;
	
	// copy back to input image
	inspxCopyU16(Image, ImageTmpL2[0], N);

	//timer = Timer();
	//printf("Detection time=%6.3fs, nrDetected=%i\n",timer-timer0,nrDetected);
	
	return nrDetected;
}

/**
 *	Set threshold values for detection algorithm.<p>
 *
 *	ThresholdsXnew, ThresholdsYnew - threshold values calculated outside (see "detectHealthyAllM.m"):<p>
 *  <UL>
 *	<LI> ThresholdsXnew should point to array of 7 threshold values {threshold_h5and, threshold_h7and, threshold_h9and, threshold_h3, 0, threshold_h5, threshold_h5a}
 *	<LI> ThresholdsYnew should point to array of 7 threshold values {threshold_v5and, threshold_v9and, threshold_v13and, threshold_v3, 0, threshold_v5, threshold_v5a}
 *  </UL>
 */
void CInspection::InspxContaminant22SetThresholds(short *ThresholdsXnew, short *ThresholdsYnew)
{
	int i;
	
	for (i=0; i<7; i++) 
	{
		_ThresholdsH[i] = ThresholdsXnew[i];
		_MinValuesGoodH[i] = (short)((double)ThresholdsXnew[i] / _MultipliersH[i]);
		_ThresholdsV[i] = ThresholdsYnew[i];
		_MinValuesGoodV[i] = (short)((double)ThresholdsYnew[i] / _MultipliersV[i]);
	}
}
/** Get threshold values for detection algorithm. See: InspxContaminant22SetThresholds()*/
void CInspection::InspxContaminant22GetThresholds(short *ThresholdsX, short *ThresholdsY)
{
	int i;
	
	for (i = 0; i < 7; i++) 
	{
		ThresholdsX[i] = _ThresholdsH[i];
		ThresholdsY[i] = _ThresholdsV[i];
	}
}

/*
// Get max image size on one axis (used for memory allocation).
WORD InspxContaminant22GetMaxImageSize()
{
	return _maxImageSize;
}
// Set max image size on one axis (used for memory allocation). Default value = 384.
void InspxContaminant22SetMaxImageSize(WORD SizeMax)
{
	_maxImageSize = SizeMax;
}

// Get max total image size in pixels (used for memory allocation).
DWORD InspxContaminant22GetMaxPixels()
{
	return _maxPixels;
}
// Set max total image size in pixels (used for memory allocation). Default value = 147456(384*384).
void InspxContaminant22SetMaxPixels(DWORD PixelsMax)
{
	_maxPixels = PixelsMax;
}
*/

/**
 * Prepares memory buffers for further operation.<p> 
 * Defines maximum image size on one axis and maximum number of pixels in images processed by the library.
 *
 *	\param SizeX maximum size of images on X axis
 *	\param SizeY maximum size of images on Y axis 
 *	\return true, if memory allocated successfully.
 */
int CInspection::InspxContaminant22CreateBuffers(WORD SizeX, WORD SizeY)
{
	InspxContaminant22FreeBuffers();
	int N;
	
	_maxImageSize = SizeX;
	if (SizeY>_maxImageSize) _maxImageSize = SizeY;
	_maxPixels = SizeX * SizeY;
	N = (int)_maxPixels;
	
	for (int i=0; i<2; i++)
	{
	
		ImageTmpL1[i] = (WORD *)malloc(N * sizeof(WORD)); 
		if (ImageTmpL1[i]==NULL) 
			return false;
		ImageTmpL2[i] = (WORD *)malloc(N * sizeof(WORD)); 
		if (ImageTmpL2[i]==NULL) 
			return false;
		_ImageTmp1[i] 		= (WORD *)malloc(N * sizeof(WORD)); 
		if (_ImageTmp1[i]==NULL) 
			return false;
		_ImageTmp2[i] 		= (WORD *)malloc(N * sizeof(WORD)); 
		if (_ImageTmp2[i]==NULL) 
			return false;
		_ImageCroppedT[i] 	= (WORD *)malloc(N * sizeof(WORD)); 
		if (_ImageCroppedT[i]==NULL) 
			return false;
		_ImageAlog [i]		= (short *)malloc(N * sizeof(short)); 
		if (_ImageAlog[i]==NULL) 
			return false;
		_ImageAlogT[i] 	= (short *)malloc(N * sizeof(short)); 
		if (_ImageAlogT[i]==NULL) 
			return false;
		_ImageAlog0[i]	= (short *)malloc(N * sizeof(short)); 
		if (_ImageAlog0[i]==NULL) 
			return false;
		_ImageAb[i] 		= (short *)malloc(N * sizeof(short)); 
		if (_ImageAb[i]==NULL) 
			return false;
		_ImageAbh5[i] 		= (short *)malloc(N * sizeof(short)); 
		if (_ImageAbh5[i]==NULL) 
			return false;
		_ImageAbv5[i] 		= (short *)malloc(N * sizeof(short)); 
		if (_ImageAbv5[i]==NULL) 
			return false;
		_ImageAd[i] 		= (BOOL *)malloc(N * sizeof(BOOL)); 
		if (_ImageAd[i]==NULL) 
			return false;
		_ImageAd1[i] 		= (BOOL *)malloc(N * sizeof(BOOL)); 
		if (_ImageAd1[i]==NULL) 
			return false;
		_ImageAd2[i] 		= (BOOL *)malloc(N * sizeof(BOOL)); 
		if (_ImageAd2[i]==NULL) 
			return false;
		_MinIndexesRows[i]	= (int *)malloc(_maxImageSize * sizeof(int)); 
		if (_MinIndexesRows[i]==NULL) 
			return false;
		_MinIndexesColumns[i] 	= (int *)malloc(_maxImageSize * sizeof(int)); 
		if (_MinIndexesColumns[i]==NULL) 
			return false;
		_MinIndexesColumnsUp[i] 	= (int *)malloc(_maxImageSize * sizeof(int)); 
		if (_MinIndexesColumnsUp[i]==NULL) 
			return false;
		_MinIndexesColumnsDown[i] 	= (int *)malloc(_maxImageSize * sizeof(int)); 
		if (_MinIndexesColumnsDown[i]==NULL) 
			return false;
		_MinimaRows[i] 		= (WORD *)malloc(_maxImageSize * sizeof(WORD)); 
		if (_MinimaRows[i]==NULL) 
			return false;
		_MinimaColumns[i] 		= (WORD *)malloc(_maxImageSize * sizeof(WORD)); 
		if (_MinimaColumns[i]==NULL) 
			return false;
		_MinimaColumnsUp[i] 		= (WORD *)malloc(_maxImageSize * sizeof(WORD)); 
		if (_MinimaColumnsUp[i]==NULL) 
			return false;
		_MinimaColumnsDown[i] 		= (WORD *)malloc(_maxImageSize * sizeof(WORD)); 
		if (_MinimaColumnsDown[i]==NULL) 
			return false;
	}
	
	return true;
}
/** Releases memory allocated by "InspxContaminant22CreateBuffers".*/
void CInspection::InspxContaminant22FreeBuffers()
{
	for (int i = 0; i<2; i++)
	{
		if (_MinimaColumnsDown[i])
			free(_MinimaColumnsDown[i]);
		_MinimaColumnsDown[i] = NULL;

		if (_MinimaColumnsUp[i])
			free(_MinimaColumnsUp[i]);
		_MinimaColumnsUp[i] = NULL;

		if (_MinimaColumns[i])
			free(_MinimaColumns[i]);
		_MinimaColumns[i] = NULL;

		if (_MinimaRows[i])
			free(_MinimaRows[i]);
		_MinimaRows[i] = NULL;

		if (_MinIndexesColumnsDown[i])
			free(_MinIndexesColumnsDown[i]);
		_MinIndexesColumnsDown[i] = NULL;

		if (_MinIndexesColumnsUp[i])
			free(_MinIndexesColumnsUp[i]);
		_MinIndexesColumnsUp[i] = NULL;

		if (_MinIndexesColumns[i])
			free(_MinIndexesColumns[i]);
		_MinIndexesColumns[i] = NULL;

		if (_MinIndexesRows[i])
			free(_MinIndexesRows[i]);
		_MinIndexesRows[i] = NULL;

		if (_ImageAd2[i])
			free(_ImageAd2[i]);
		_ImageAd2[i] = NULL;

		if (_ImageAd1[i])
			free(_ImageAd1[i]);
		_ImageAd1[i] = NULL;

		if (_ImageAd[i])
			free(_ImageAd[i]);
		_ImageAd[i] = NULL;

		if (_ImageAbv5[i])
			free(_ImageAbv5[i]);
		_ImageAbv5[i] = NULL;

		if (_ImageAbh5[i])
			free(_ImageAbh5[i]);
		_ImageAbh5[i] = NULL;

		if (_ImageAb[i])
			free(_ImageAb[i]);
		_ImageAb[i] = NULL;

		if (_ImageAlog0[i])
			free(_ImageAlog0[i]);
		_ImageAlog0[i] = NULL;

		if (_ImageAlogT[i])
			free(_ImageAlogT[i]);
		_ImageAlogT[i] = NULL;

		if (_ImageAlog[i])
			free(_ImageAlog[i]);
		_ImageAlog[i] = NULL;

		if (_ImageCroppedT[i])
			free(_ImageCroppedT[i]);
		_ImageCroppedT[i] = NULL;

		if (_ImageTmp2[i])
			free(_ImageTmp2[i]);
		_ImageTmp2[i] = NULL;

		if (_ImageTmp1[i])
			free(_ImageTmp1[i]);
		_ImageTmp1[i] = NULL;

		if (ImageTmpL2[i])
			free(ImageTmpL2[i]);
		ImageTmpL2[i] = NULL;

		if (ImageTmpL1[i])
			free(ImageTmpL1[i]);
		ImageTmpL1[i] = NULL;
	}
}

/** Get threshold used for cropping white margins.*/
WORD CInspection::InspxContaminant22GetBackgroundThreshold()
{
	return _backgroundThreshold;
}
/** Set threshold used for cropping white margins. Default value = 45000.*/
void CInspection::InspxContaminant22SetBackgroundThreshold(WORD ThresholdNew)
{
	_backgroundThreshold = ThresholdNew;
}

/** Get min package image size on X axis (used for checking cropping results).*/
WORD CInspection::InspxContaminant22GetMinSizeX()
{
	return _minSizeX;
}
/** Set min package image size on X axis (used for checking cropping results). Default value = 230.*/
void CInspection::InspxContaminant22SetMinSizeX(WORD SizeXMinNew)
{
	_minSizeX = SizeXMinNew;
}

/** Get min package image size on Y axis (used for checking cropping results). */
WORD CInspection::InspxContaminant22GetMinSizeY()
{
	return _minSizeY;
}
/** Set min package image size on Y axis (used for checking cropping results). Default value = 280. */
void CInspection::InspxContaminant22SetMinSizeY(WORD SizeYMinNew)
{
	_minSizeY = SizeYMinNew;
}

/** Get number of rows/columns used for detection of package edges (after image cropping).*/
WORD CInspection::InspxContaminant22GetRowsEdge()
{
	return _rowsEdge;
}
/** Set number of rows/columns used for detection of package edges (after image cropping). Default value = 50.*/
void CInspection::InspxContaminant22SetRowsEdge(WORD RowsEdgeNew)
{
	_rowsEdge = RowsEdgeNew;
}

/** Get value of "ProcessEdges" flag. */
BOOL CInspection::InspxContaminant22GetProcessEdges()
{
	return _processEdges;
}
/** Set value of "ProcessEdges" flag. Default value = true. */
void CInspection::InspxContaminant22SetProcessEdges(BOOL ProcessEdgesNew)
{
	_processEdges = ProcessEdgesNew;
}

/**
 *	Obtains error text from error code.
 *	\param code error code returned by library function
 *	\return error string
 */
CString CInspection::InspxGetErrorText(int code) 
{
	int codeI = -code;
	
	return (codeI<0 || codeI>INSPX_MAX_ERROR_NR) ? inspxErrorTexts[INSPX_MAX_ERROR_NR+1] : inspxErrorTexts[codeI];
}

/**
 *	Finds and returns "Healthy Food" package limits within image.
 *
 *	\param colLeft pointer used to return leftmost edge pixel number within "imageIn"
 *	\param colRight pointer used to return rightmost edge pixel number within "imageIn"
 *	\param rowUp pointer used to return uppermost edge pixel number within "imageIn"
 *	\param rowDown pointer used to return lowermost edge pixel number within "imageIn"
 *	\param imageIn input image
 *	\param rows size of "imageIn" on X axis
 *	\param columns size of "imageIn" on Y axis
 *	\return true if package edges found and package size inside the edges is at least _minSizeY x _minSizeX pixels 
 * \sa inspxProcessEdges(), inspxImageMinRows(), inspxImageMinColumns()
 */
BOOL CInspection::inspxPackageLimits(int *colLeft, int *colRight, int *rowUp, int *rowDown, WORD *imageIn, int rows, int columns, int k) 
{
	WORD *ptrMin;
	unsigned char found;

	// get minima over image rows, with indexes
	inspxImageMinRows(_MinimaRows[k], _MinIndexesRows[k], imageIn, rows, columns);
	// get minima over image columns, with indexes
	inspxImageMinColumns(_MinimaColumns[k], _MinIndexesColumns[k], imageIn, rows, columns);
	
	// find upper minimum below the background threshold
	found = false;
	ptrMin = _MinimaRows[k];
	//*rowUp = 0;
	//ptrMin++;		// ams	// temp change to test
	for (int i = 0; i < rows; i++) 
	{
		if ((*ptrMin++) < _backgroundThreshold) 
		{
			*rowUp = i;
			found = true;
			break;
		}
	}
	if (!found) 
		return false;
	
	// find lower minimum below the background threshold
	found = false;
	ptrMin = _MinimaRows[k] + rows - 1;
	//*rowDown = rows - 1;
	for (int i = rows - 1; i >= 0; i--) 
	{
		if ((*ptrMin--) < _backgroundThreshold) 
		{
			*rowDown = i;
			found = true;
			break;
		}
	}
	if (!found) 
		return false;
	
	// find left minimum below the background threshold
	found = false;
	ptrMin = _MinimaColumns[k];
	//*colLeft = 0;
	ptrMin++;					// ams	// temp change to test
	for (int i = 1; i < columns; i++) 
	{
		if ((*ptrMin++) < _backgroundThreshold) 
		{
			*colLeft = i;
			found = true;
			break;
		}
	}
	if (!found) 
		return false;

	// find right minimum below the background threshold
	found = false;
	ptrMin = _MinimaColumns[k] + columns - 1;
	//*colRight = columns - 1;
	for (int i = columns - 1; i >= 0; i--) 
	{
		if ((*ptrMin--) < _backgroundThreshold) 
		{
			*colRight = i;
			found = true;
			break;
		}
	}
	if (!found) 
		return false;
	
	if ((*rowDown - *rowUp) < _minSizeY)
		return false;		// package should be of expected minimum size

	if ((*colRight - *colLeft) < _minSizeX) 
		return false;
	
	return true;
}

/**
 *	Processes left and right edges of "Healthy Choice" package to exclude them from contaminant detection.  
 *	Finds coordinates of left and right edge points in upper and lower parts of the package image.
 *  Masks off left and right package edges by drawing a line through these points using maximum brightness values.
 *
 *	\param imageIn input image
 *	\param rows size of "imageIn" on X axis
 *	\param columns size of "imageIn" on Y axis
 *	\param rowUp uppermost row of package within the image 
 *	\param rowDown lowermost row of package within the image 
 * \sa inspxProcessEdges(), inspxImageMinRows(), inspxImageMinColumns()
 */
void CInspection::inspxEdgePointsLeftRight(WORD *imageIn, int rows, int columns, int rowUp, int rowDown, int k) 
{
	WORD *ptrMin, minVal, *ptrRow, *ptrVal;
	int i, x11, x12, x21, x22, irow, icol;
	int x1Left, x2Left, x1Right, x2Right, y1Left, y2Left, y1Right, y2Right;
	
	double leftCoeff, rightCoeff;
	double y, x1d, x2d; 

	//printf("inspxEdgePointsLeftRight: rows=%i, columns=%i\n", rows, columns);

	// find column minima in upper part of the image
	inspxImageMinColumns(_MinimaColumnsUp[k], _MinIndexesColumnsUp[k], imageIn, rowUp+_rowsEdge, columns);
	// find column minima in lower part of the image
	inspxImageMinColumns(_MinimaColumnsDown[k], _MinIndexesColumnsDown[k], imageIn+(rowDown-_rowsEdge)*columns, rows-rowDown+_rowsEdge, columns);

	// find coordinates of leftmost edge point in upper part of the image
	ptrMin = _MinimaColumnsUp[k];
	//x1Left = 0;
	ptrMin++;
	for (i=1; i<columns; i++) {		// ams 1 as first index to get rid of wrong column
		minVal = *ptrMin++;
		//printf("min Left1: i=%i min value=%u index=%u\n", i, minVal, minIndexesColumnsUp[i]);
		if (minVal < _backgroundThreshold) {
			//printf("min Left1: x=%i y=%i\n",i,minIndexesColumnsUp[i]);
			x1Left = i;
			y1Left = _MinIndexesColumnsUp[k][i];
			break;
		}
	}
	// find coordinates of rightmost edge point in upper part of the image
	ptrMin = _MinimaColumnsUp[k]+columns-1;
	//x1Right = columns-1;
	ptrMin--;
	for (i=columns-1; i>=0; i--) {
		minVal = *ptrMin--;
		//printf("min Right1: i=%i min value=%u index=%u\n", i, minVal, minIndexesColumnsUp[i]);
		if (minVal < _backgroundThreshold) {
			//printf("min Right1: x=%i y=%i\n",i,minIndexesColumnsUp[i]);
			x1Right = i;
			y1Right = _MinIndexesColumnsUp[k][i];
			break;
		}
	}

	// find coordinates of leftmost edge point in lower part of the image
	ptrMin = _MinimaColumnsDown[k];
	//x2Left = 0;
	ptrMin++;
	for (i=1; i<columns; i++) {		// ams 1 as first index to get rid of wrong column
		minVal = *ptrMin++;
		//printf("min Left2: i=%i min value=%u index=%u\n", i, minVal, minIndexesColumnsDown[i] + rows - INSPX_NR_ROWS_FOR_EDGE_DETECT);
		if (minVal < _backgroundThreshold) {
			x2Left = i;
			y2Left = _MinIndexesColumnsDown[k][i] + rowDown - _rowsEdge;
			//printf("min Left2: x=%i y=%i\n",i,y2Left);
			break;
		}
	}
	// find coordinates of rightmost edge point in lower part of the image
	ptrMin = _MinimaColumnsDown[k]+columns-1;
	//x2Right = columns-1;
	ptrMin--;
	for (i=columns-1; i>=0; i--) {
		minVal = *ptrMin--;
		//printf("min Right2: i=%i min value=%u index=%u\n", i, minVal, minIndexesColumnsDown[i] + rows - INSPX_NR_ROWS_FOR_EDGE_DETECT);
		if (minVal < _backgroundThreshold) {
			x2Right = i;
			y2Right = _MinIndexesColumnsDown[k][i] + rowDown - _rowsEdge;
			//printf("min Right2: x=%i y=%i\n",i,y2Right);
			break;
		}
	}
	
	// calculate slope coefficients of left and right edges  
	leftCoeff  = ((double)(x2Left-x1Left))   / ((double)(y2Left-y1Left));
	rightCoeff = ((double)(x2Right-x1Right)) / ((double)(y2Right-y1Right));
	
	// mask off edge pixels
	ptrRow = imageIn;
	for (irow=0; irow<rows; irow++) {	// for each row
		y = (double)irow;
		x1d = x1Left - (y1Left-y) * leftCoeff - 1.;   // calculate left edge first column coordinate taking account the slope
	    x11 = (int)x1d; 
			if (x11<0) 
				x11 = 0;				  // calculate first pixel index of left edge, ensure that it is in limits	
			
	    x12 = (int)(x1d + 6.); 
			if (x12<0) 
				x12 = 0;			  // calculate last pixel index of left edge, ensure that it is in limits

	    ptrVal = ptrRow + x11;
	    for (icol=x11; icol<=x12; icol++) *ptrVal++ = INSPX_MAX_PIXEL_VALUE;// mask off left edge pixels by replacing them with maximum brightness

	    x2d = x1Right - (y1Right-y) * rightCoeff + 2.;// calculate right edge first column coordinate taking account the slope
	    x21 = (int)(x2d - 7.);								  // calculate first pixel index of right edge
	    x22 = (int)x2d;									  // calculate last pixel index of right edge
	    if (x21>=columns) x21 = columns-1;			  // ensure that first pixel index is in limits 
	    if (x22>=columns) x22 = columns-1;			  // ensure that last pixel index is in limits

	    ptrVal = ptrRow + x21;
	    for (icol=x21; icol<=x22; icol++) *ptrVal++ = INSPX_MAX_PIXEL_VALUE;// mask off right edge pixels by replacing them with maximum brightness

	    ptrRow += columns; // move pointer to start of next row
	}
}

/**
 *	Copies region of interest (ROI) from input image 'imageIn' of size 'rowsIn'x'columnsIn' to separate image (allocated by caller).
 *	Region is defined by 0-based coordinates of first and last pixels to include in resulting 'imageOut'.
 *	
 *	\param imageOut output image (extracted region)
 *	\param imageIn input image
 *	\param rowsIn size of "imageIn" on X axis
 *	\param columnsIn size of "imageIn" on Y axis
 *	\param x1 index of the leftmost pixel in imageIn to be extracted
 *	\param x2 index of the rightmost pixel in imageIn to be extracted  
 *	\param y1 index of the uppermost pixel in imageIn to be extracted  
 *	\param y2 index of the lowermost pixel in imageIn to be extracted  
 * \sa inspxProcessEdges(), inspxDetect()
 */
void CInspection::inspxExtractROI(WORD *imageOut, WORD *imageIn, int rowsIn, int columnsIn, int x1, int x2, int y1, int y2)
{
	int irow, icol;
	int	rowsOut = y2 - y1 + 1;
	int	columnsOut = x2 - x1 + 1;
	int columnsToSkip = columnsIn - columnsOut;
	WORD *ptrIn = imageIn + y1 * columnsIn + x1;
	WORD *ptrOut = imageOut;
	
	for (irow=0; irow<rowsOut; irow++) {
		for (icol=0; icol<columnsOut; icol++) {
			*ptrOut++ = *ptrIn++;
		}
		ptrIn += columnsToSkip; 
	}
}

/**
 *	Obtains 'imageOut' as a copy of 'imageIn' with region of interest (ROI) pixels replaced from 'imageROI'.
 *  Size of 'imageOut' and 'imageIn' is 'rows'x'columns'.
 *	'x1','x2','y1' and 'y2' are 0-based coordinates of first and last pixels of 'imageOut' to be replaced by pixels from 'imageROI'.
 *	
 *	\param imageOut output image (extracted region)
 *	\param imageIn input image
 *	\param rows size of "imageIn" on X axis
 *	\param columns size of "imageIn" on Y axis
 *	\param imageROI image to be copied into ROI of imageOut
 *	\param x1 index of the leftmost pixel in imageOut to be replaced
 *	\param x2 index of the rightmost pixel in imageOut to be replaced  
 *	\param y1 index of the uppermost pixel in imageOut to be replaced  
 *	\param y2 index of the lowermost pixel in imageOut to be replaced  
 * \sa inspxDetect()
 */
void CInspection::inspxReplaceROI(WORD *imageOut, WORD *imageIn, int rows, int columns, WORD *imageROI, int x1, int x2, int y1, int y2)
{
	int i, N=rows*columns;
	int irow, icol;
	int	rowsROI = y2 - y1 + 1;
	int	columnsROI = x2 - x1 + 1;
	int columnsToSkip = columns - columnsROI;
	WORD *ptrIn;
	WORD *ptrOut;

	ptrIn = imageIn; ptrOut = imageOut;
	for (i=0; i<N; i++) *ptrOut++ = *ptrIn++;
	
	ptrIn = imageROI;
	ptrOut = imageOut + y1 * columns + x1;
	for (irow=0; irow<rowsROI; irow++) {
		for (icol=0; icol<columnsROI; icol++) {
			*ptrOut++ = *ptrIn++;
		}
		ptrOut += columnsToSkip; 
	}
}

/**
 * Processes package edges assuming that they are straight dark lines. Finds package limits within image, masks off package edges, and crops out white margins,
 *
 *	\param imageTmp1 output (cropped) image, used also as temporary storage
 *	\param colLeft pointer used to return leftmost edge pixel number within "imageIn"
 *	\param colRight pointer used to return rightmost edge pixel number within "imageIn"
 *	\param rowUp pointer used to return uppermost edge pixel number within "imageIn"
 *	\param rowDown pointer used to return lowermost edge pixel number within "imageIn"
 *	\param imageTmp2 temporary image
 *	\param imageIn input image
 *	\param rowsIn size of "imageIn" on X axis
 *	\param columnsIn size of "imageIn" on Y axis
 *	\return INSPX_OK(), if package found; 
 *	\return INSPX_PACKAGE_NOT_FOUND() if not found.
 * \sa inspxUpdateThresholds(), inspxDetect()
 */
int CInspection::inspxProcessEdges(WORD *imageTmp1, int *colLeft, int *colRight, int *rowUp, int *rowDown, WORD *imageTmp2, 
																	 WORD *imageIn, int rowsIn, int columnsIn, int k)
{
	// find package limits in image
	if (!inspxPackageLimits(colLeft, colRight, rowUp, rowDown, imageIn, rowsIn, columnsIn, k)) 
		return INSPX_PACKAGE_NOT_FOUND;

	// transpose to process up/down sides
	inspxTransposeU16(imageTmp1, imageIn, rowsIn, columnsIn);

	// find upper and lower edges and mask edge pixels  
	inspxEdgePointsLeftRight(imageTmp1, columnsIn, rowsIn, *colLeft, *colRight, k);

	// transpose back
	inspxTransposeU16(imageTmp2, imageTmp1, columnsIn, rowsIn);

	// find left and right edges and mask edge pixels  
	inspxEdgePointsLeftRight(imageTmp2, rowsIn, columnsIn, *rowUp, *rowDown, k); 

	// crop out empty margins
	inspxExtractROI(imageTmp1, imageTmp2, rowsIn, columnsIn, *colLeft, *colRight, *rowUp, *rowDown);
	
	return INSPX_OK;
}
/**
 *	Calculates minimum pixel values found after background filtering with differnet filters. For each threshold,
 *  updates minimum and threshold values if corresponding old minimum value is larger than newly calculated one.  
 *
 *	\param imageIn input image
 *	\param rowsIn size of "imageIn" on X axis
 *	\param columnsIn size of "imageIn" on Y axis
 *	\param processEdges flag indicating whether processing of edges should be used (true= should be used)
 *	\return INSPX_OK(), if image processed successfully; 
 *	\return INSPX_PACKAGE_NOT_FOUND() if product package not found within "imageIn".
 * \sa InspxContaminant22ThresholdCalculationB(), InspxContaminant22ThresholdCalculationW()
 */
int CInspection::inspxUpdateThresholds(WORD *imageIn, int rowsIn, int columnsIn, BOOL processEdges,  WORD n)
{
	int res, N;

	//FILE* mm;

	double t;

	int colLeft, colRight, rowUp, rowDown;
	int rowsCropped, columnsCropped;
	short minValue, maxValue;

	// ams May 06, 2010, all buffers changed to index k from previous 0
	
	if (processEdges) 
	{
	   
		res = inspxProcessEdges(_ImageTmp1[0], &colLeft, &colRight, &rowUp, &rowDown, 
			_ImageTmp2[0], imageIn, rowsIn, columnsIn, 0);
		//res = inspxProcessEdges(_ImageTmp1[k], &colLeft, &colRight, &rowUp, &rowDown, 
		//	_ImageTmp2[k], imageIn, rowsIn, columnsIn, 0);
		if (res!=INSPX_OK) 
		{
			
			return res;	
		}

		rowsCropped = rowDown - rowUp + 1;	
	
		columnsCropped = colRight - colLeft + 1;
		N = rowsCropped * columnsCropped;

		// linearize
	
		inspxImageLog(_ImageAlog[0], _ImageTmp1[0], N); 
	} 
	
	else 
	{
	
		rowsCropped = rowsIn;
		columnsCropped = columnsIn;
		N = rowsIn * columnsIn;

		// linearize
	
		inspxImageLog(_ImageAlog[0], imageIn, N); 
	}

	//inspxImageMinMax(_ImageAlog[0], N, &minValue, &maxValue);

	//mm = fopen("c:\\ImMinMax.txt","a");
	//fprintf(mm, "%f, %f \n", (float)minValue, (float)maxValue);
	//fclose(mm);


	// transpose Alog
	
	inspxTransposeI16(_ImageAlogT[0], _ImageAlog[0], rowsCropped, columnsCropped);
	
	// calculate Abh3
	
	inspxRemoveBackgroundImageByRows(_ImageAb[0], _ImageAlog[0], rowsCropped, columnsCropped, 3);
	inspxImageMinMax(_ImageAb[0], N, &minValue, &maxValue);


	//mm = fopen("c:\\ImMinMax.txt","a");
	//fprintf(mm, "%f, %f \n", (float)minValue, (float)maxValue);
	//fclose(mm);


	h3*=(n-1);
	t = minValue * _MultipliersH[3];
	h3+=t;
	if (n)
		h3/=n;
	vh3*=(n-1);
	vh3+=(t*t);
	if (n)
		vh3/=n;
	_varH[3] = sqrt(vh3 - h3*h3);

	if (minValue<_MinValuesGoodH[3]) 
	{
		_MinValuesGoodH[3] = minValue;
		_ThresholdsH[3] = (short)(_MinValuesGoodH[3] * _MultipliersH[3]);
		
	}

	// calculate Abh5
	
	inspxRemoveBackgroundImageByRows(_ImageAb[0], _ImageAlog[0], rowsCropped, columnsCropped, 5);
	inspxImageMinMax(_ImageAb[0], N, &minValue, &maxValue);

	h0*=(n-1);
	t = minValue * _MultipliersH[0];
	h0+=t;
	if (n)
		h0/=n;
	vh0*=(n-1);
	vh0+=(t*t);
	if (n)
		vh0/=n;
	_varH[0] = sqrt(vh0 - h0*h0);

	h5*=(n-1);
	t = minValue * _MultipliersH[5];
	h5+=t;
	if (n)
		h5/=n;
	vh5*=(n-1);
	vh5+=(t*t);
	if (n)
		vh5/=n;
	_varH[5] = sqrt(vh5 - h5*h5);

	if (minValue<_MinValuesGoodH[5]) 
	{
		_MinValuesGoodH[0] = minValue;
		_MinValuesGoodH[5] = minValue;
		_ThresholdsH[0] = (short)(_MinValuesGoodH[0] * _MultipliersH[0]);
		_ThresholdsH[5] = (short)(_MinValuesGoodH[5] * _MultipliersH[5]);
		
	}

	// calculate Alog0_h11
	
	inspxRemoveBackgroundTrendByRows(_ImageAlog0[0], _ImageAlog[0], rowsCropped, columnsCropped, 11);
	
	// calculate Abh5a
	
	inspxRemoveBackgroundImageByRows(_ImageAb[0], _ImageAlog0[0], rowsCropped, columnsCropped, 5);
	inspxImageMinMax(_ImageAb[0], N, &minValue, &maxValue);

	h6*=(n-1);
	t = minValue * _MultipliersH[6];
	h6+=t;
	if (n)
		h6/=n;
	vh6*=(n-1);
	vh6+=(t*t);
	if (n)
		vh6/=n;
	_varH[6] = sqrt(vh6 - h6*h6);

	if (minValue<_MinValuesGoodH[6]) {
		_MinValuesGoodH[6] = minValue;
		_ThresholdsH[6] = (short)(_MinValuesGoodH[6] * _MultipliersH[6]);
		
	}

	// calculate Abh7

	inspxRemoveBackgroundImageByRows(_ImageAb[0], _ImageAlog[0], rowsCropped, columnsCropped, 7);
	inspxImageMinMax(_ImageAb[0], N, &minValue, &maxValue);

	h1*=(n-1);
	t = minValue * _MultipliersH[1];
	h1+=t;
	if (n)
		h1/=n;
	vh1*=(n-1);
	vh1+=(t*t);
	if (n)
		vh1/=n;
	_varH[1] = sqrt(vh1 - h1*h1);

	if (minValue<_MinValuesGoodH[1]) {
		_MinValuesGoodH[1] = minValue;
		_ThresholdsH[1] = (short)(_MinValuesGoodH[1] * _MultipliersH[1]);
		
	}

	// calculate Abh9

	inspxRemoveBackgroundImageByRows(_ImageAb[0], _ImageAlog[0], rowsCropped, columnsCropped, 9);
	inspxImageMinMax(_ImageAb[0], N, &minValue, &maxValue);

	h2*=(n-1);
	t = minValue * _MultipliersH[2];
	h2+=t;
	if (n)
		h2/=n;
	vh2*=(n-1);
	vh2+=(t*t);
	if (n)
		vh2/=n;
	_varH[2] = sqrt(vh2 - h2*h2);

	if (minValue<_MinValuesGoodH[2]) 
	{
		_MinValuesGoodH[2] = minValue;
		_ThresholdsH[2] = (short)(_MinValuesGoodH[2] * _MultipliersH[2]);
		
	}

	// calculate Abv3

	inspxRemoveBackgroundImageByRows(_ImageAb[0], _ImageAlogT[0], columnsCropped, rowsCropped, 3);
	inspxImageMinMax(_ImageAb[0], N, &minValue, &maxValue);

	v3*=(n-1);
	t = minValue * _MultipliersV[3];
	v3+=t;
	if (n)
		v3/=n;
	vv3*=(n-1);
	vv3+=(t*t);
	if (n)
		vv3/=n;
	_varV[3] = sqrt(vv3 - v3*v3);

	if (minValue < _MinValuesGoodV[3]) 
	{
		_MinValuesGoodV[3] = minValue;
		_ThresholdsV[3] = (short)(_MinValuesGoodV[3] * _MultipliersV[3]);
		
	}

	// calculate Abv5

	inspxRemoveBackgroundImageByRows(_ImageAb[0], _ImageAlogT[0], columnsCropped, rowsCropped, 5);
	inspxImageMinMax(_ImageAb[0], N, &minValue, &maxValue);

	v0*=(n-1);
	t = minValue * _MultipliersV[0];
	v0+=t;
	if (n)
		v0/=n;
	vv0*=(n-1);
	vv0+=(t*t);
	if (n)
		vv0/=n;
	_varV[0] = sqrt(vv0 - v0*v0);

	v5*=(n-1);
	t = minValue * _MultipliersV[5];
	v5+=t;
	if (n)
		v5/=n;
	vv5*=(n-1);
	vv5+=(t*t);
	if (n)
		vv5/=n;
	_varV[5] = sqrt(vv5 - v5*v5);

	if (minValue<_MinValuesGoodV[5]) 
	{
		_MinValuesGoodV[5] = minValue;
		_MinValuesGoodV[0] = minValue;
		_ThresholdsV[5] = (short)(_MinValuesGoodV[5] * _MultipliersV[5]);
		_ThresholdsV[0] = (short)(_MinValuesGoodV[0] * _MultipliersV[0]);
		
	}

	// calculate Alog0_v15

	inspxRemoveBackgroundTrendByRows(_ImageAlog0[0], _ImageAlogT[0], columnsCropped, rowsCropped, 15);
	
	// calculate Abv5a

	inspxRemoveBackgroundImageByRows(_ImageAb[0], _ImageAlog0[0], columnsCropped, rowsCropped, 5);
	inspxImageMinMax(_ImageAb[0], N, &minValue, &maxValue);

	v6*=(n-1);
	t = minValue * _MultipliersV[6];
	v6+=t;
	if (n)
		v6/=n;
	vv6*=(n-1);
	vv6+=(t*t);
	if (n)
		vv6/=n;
	_varV[6] = sqrt(vv6 - v6*v6);

	if (minValue<_MinValuesGoodV[6]) 
	{
		_MinValuesGoodV[6] = minValue;
		_ThresholdsV[6] = (short)(_MinValuesGoodV[6] * _MultipliersV[6]);
		
	}

	// calculate Abv9
	
	inspxRemoveBackgroundImageByRows(_ImageAb[0], _ImageAlogT[0], columnsCropped, rowsCropped, 9);
	inspxImageMinMax(_ImageAb[0], N, &minValue, &maxValue);

	v1*=(n-1);
	t = minValue * _MultipliersV[1];
	v1+=t;
	if (n)
		v1/=n;
	vv1*=(n-1);
	vv1+=(t*t);
	if (n)
		vv1/=n;
	_varV[1] = sqrt(vv1 - v1*v1);

	if (minValue<_MinValuesGoodV[1]) 
	{
		_MinValuesGoodV[1] = minValue;
		_ThresholdsV[1] = (short)(_MinValuesGoodV[1] * _MultipliersV[1]);
		
	}

	// calculate Abv13

	inspxRemoveBackgroundImageByRows(_ImageAb[0], _ImageAlogT[0], columnsCropped, rowsCropped, 13);
	inspxImageMinMax(_ImageAb[0], N, &minValue, &maxValue);

	v2*=(n-1);
	t = minValue * _MultipliersV[2];
	v2+=t;
	if (n)
		v2/=n;
	vv2*=(n-1);
	vv2+=(t*t);
	if (n)
		vv2/=n;
	_varV[2] = sqrt(vv2 - v2*v2);

	if (minValue<_MinValuesGoodV[2]) 
	{
		_MinValuesGoodV[2] = minValue;
		_ThresholdsV[2] = (short)(_MinValuesGoodV[2] * _MultipliersV[2]);
		
	}



	return INSPX_OK;
}



short ths(double a, double b, double c, BYTE t)
{
	double T = (double)t;
	short s;
	s =  (short)(a*T*T + b*T + c);
	return s;

}
/**
 *	Performs contaminant detection according to algorithm found in "detectHealthyAllM.m".
 *
 *	\param imageOut output image, formed from imageIn by marking found contaminant pixels.
 *	\param nrDetected pointer used to return number of found contaminant pixels
 *	\param imageIn input image
 *	\param rowsIn size of "imageIn" on X axis
 *	\param columnsIn size of "imageIn" on Y axis
 *	\param processEdges flag indicating whether processing of edges should be used (true= should be used)
 *	\return INSPX_OK(), if detection performed successfully
 *	\return INSPX_PACKAGE_NOT_FOUND(), if product package not found within "imageIn".
 * \sa InspxContaminant22InspectionB(), InspxContaminant22InspectionW()
 */
int CInspection::inspxDetect(WORD *imageOut, int *nrDetected, WORD *imageIn, int rowsIn, int columnsIn, 
							 BOOL processEdges, BYTE sTh, int k)
{
	int res, pixelsFound, N;

	short H[7];
	short V[7];

	for (int i = 0; i<7; i++)
	{
		H[i] = _ThresholdsH[i];
		V[i] = _ThresholdsV[i];

		double TempFactor = ((sTh + 35.0)*(sTh + 35.0))/65025.0;
		if (TempFactor)
		{
			H[i] = (short)(H[i] / TempFactor);
			V[i] = (short)(V[i] / TempFactor);
		}

		//H[i] /= (((sTh + 35.)*(sTh + 35.)*(sTh + 35.))/16581375.);
		//V[i] /= (((sTh + 35.)*(sTh + 35.)*(sTh + 35.))/16581375.);

		//H[i]-=(3*_varH[i]*(1 - sTh/255));
		//V[i]-=(3*_varV[i]*(1 - sTh/255));
	}
/*

	H[0] = ths(-0.04784, 28.4385, -4639.9967, sTh);

	H[1] = ths(-0.07377, 41.4395, -6392.3535, sTh);

	H[2] = ths(-0.08082, 42.3888, -6241.9237, sTh);

	H[3] = ths(-0.07029, 39.3324, -6133.9045, sTh);

	H[5] = ths(-0.070198, 41.7155, -6804.8225, sTh);

	H[6] = ths(-0.07777, 43.3122, -6776.6169, sTh);

	

	V[0] = ths(-0.0241, 28.6169, -6283.482, sTh);

	V[1] = ths(-0.044484, 33.5298, -6348.5367, sTh);

	V[2] = ths(-0.05415, 37.207, -6759.6815, sTh);

	V[3] = ths(-0.05505, 44.7766, -8645.5068, sTh);

	V[5] = ths(-0.029938, 35.3918, -7762.2104, sTh);

	V[6] = ths(-0.028845, 35.2157, -7787.3576, sTh);
	
*/
/*
	double te = _varH[0];
	te = _varH[1];
	te = _varH[2];
	te = _varH[3];
	te = _varH[4];
	te = _varH[5];
	te = _varH[6];
	te = _varV[0];
	te = _varV[1];
	te = _varV[2];
	te = _varV[3];
	te = _varV[4];
	te = _varV[5];
	te = _varV[6];
*/
	int colLeft, colRight, rowUp, rowDown;

	int rowsCropped, columnsCropped;

	if (processEdges) 
	{
	  res = inspxProcessEdges(_ImageTmp1[k], &colLeft, &colRight, &rowUp, &rowDown, _ImageTmp2[k], imageIn, rowsIn, columnsIn, k);
	  
		if (res!=INSPX_OK) 
		{
			////////
			
			////////////////
			return res;	
		}

		rowsCropped = rowDown - rowUp + 1;
		columnsCropped = colRight - colLeft + 1;
		N = rowsCropped * columnsCropped;

		// linearize
	
		inspxImageLog(_ImageAlog[k], _ImageTmp1[k], N); 
	} 
	else 
	{
	
		rowsCropped = rowsIn;
		////columnsCropped = columnsIn-1;
		columnsCropped = columnsIn;
		
		N = rowsIn * columnsIn;

		// linearize
		
		inspxImageLog(_ImageAlog[k], imageIn, N); 
	}

	// transpose Alog and save in AlogT for V processing
	inspxTransposeI16(_ImageAlogT[k], _ImageAlog[k], rowsCropped, columnsCropped);
	
	// calculate Abh5
	inspxRemoveBackgroundImageByRows(_ImageAbh5[k], _ImageAlog[k], rowsCropped, columnsCropped, 5);
	// calculate Adh03
	inspxThresholdNegative(_ImageAd[k], &pixelsFound, _ImageAbh5[k], N, H[0]);
	//if (pixelsFound)
	//	te = pixelsFound;

	// calculate Abh7
	inspxRemoveBackgroundImageByRows(_ImageAb[k], _ImageAlog[k], rowsCropped, columnsCropped, 7);
	// calculate Adh04
	inspxThresholdNegative(_ImageAd1[k], &pixelsFound, _ImageAb[k], N, H[1]);
	//if (pixelsFound)
	//	te = pixelsFound;
	// Ad = Adh03 | Adh04
	inspxImageOR(_ImageAd[k], _ImageAd1[k], N);

	// calculate Abh9
	inspxRemoveBackgroundImageByRows(_ImageAb[k], _ImageAlog[k], rowsCropped, columnsCropped, 9);
	// calculate Adh05
	inspxThresholdNegative(_ImageAd1[k], &pixelsFound, _ImageAb[k], N, H[2]);
	//if (pixelsFound)
	//	te = pixelsFound;
	// Ad = Adh03 | Adh04 | Adh05
	inspxImageOR(_ImageAd[k], _ImageAd1[k], N);
	
	// calculate Abv5
	inspxRemoveBackgroundImageByRows(_ImageAbv5[k], _ImageAlogT[k], columnsCropped, rowsCropped, 5);
	// calculate Adv03
	inspxThresholdNegative(_ImageAd2[k], &pixelsFound, _ImageAbv5[k], N, V[0]);
	//if (pixelsFound)
	//	te = pixelsFound;
	
	// calculate Abv9
	inspxRemoveBackgroundImageByRows(_ImageAb[k], _ImageAlogT[k], columnsCropped, rowsCropped, 9);
	// calculate Adv05
	inspxThresholdNegative(_ImageAd1[k], &pixelsFound, _ImageAb[k], N, V[1]);
	//if (pixelsFound)
	//	te = pixelsFound;
	// Ad2 = Adv03 | Adv05
	//inspxImageOR(_ImageAd2s, _ImageAd1s, N);
	inspxImageOR(_ImageAd2[k], _ImageAd1[k], N);
	
	// calculate Abv13
	inspxRemoveBackgroundImageByRows(_ImageAb[k], _ImageAlogT[k], columnsCropped, rowsCropped, 13);
	// calculate Adv06
	inspxThresholdNegative(_ImageAd1[k], &pixelsFound, _ImageAb[k], N, V[2]);
	//if (pixelsFound)
	//	te = pixelsFound;
	// Ad2 = Adv03 | Adv05 | Adv06
	inspxImageOR(_ImageAd2[k], _ImageAd1[k], N);
	
	//inspxTransposeU8((BYTE *)(_ImageAd1s), (BYTE *)(_ImageAd2s), columnsCropped, rowsCropped);	
	inspxTransposeU8((BYTE *)(_ImageAd1[k]), (BYTE *)(_ImageAd2[k]), columnsCropped, rowsCropped);	
	// Ad = (Adh03 | Adh04 | Adh05) & (Adv03 | Adv05 | Adv06)
	inspxImageAND(_ImageAd[k], _ImageAd1[k], N);
	
	// calculate Abh3
	inspxRemoveBackgroundImageByRows(_ImageAb[k], _ImageAlog[k], rowsCropped, columnsCropped, 3);
	// calculate Adh01
	inspxThresholdNegative(_ImageAd1[k], &pixelsFound, _ImageAb[k], N, H[3]);
	//if (pixelsFound)
	//	te = pixelsFound;
	// Ad = (Adh03 | Adh04 | Adh05) & (Adv03 | Adv05 | Adv06) | Adh01
	inspxImageOR(_ImageAd[k], _ImageAd1[k], N);
	
	// calculate Adh02
	//inspxThresholdNegative(_ImageAd1s, &pixelsFound, _ImageAbh5s, N, _ThresholdsH[5]);
	inspxThresholdNegative(_ImageAd1[k], &pixelsFound, _ImageAbh5[k], N, H[5]);
	//if (pixelsFound)
	//	te = pixelsFound;
	// Ad = (Adh03 | Adh04 | Adh05) & (Adv03 | Adv05 | Adv06) | Adh01 | Adh02
	inspxImageOR(_ImageAd[k], _ImageAd1[k], N);
	
	// calculate Alog0_h11
	inspxRemoveBackgroundTrendByRows(_ImageAlog0[k], _ImageAlog[k], rowsCropped, columnsCropped, 11);
	// calculate Abh5a
	inspxRemoveBackgroundImageByRows(_ImageAb[k], _ImageAlog0[k], rowsCropped, columnsCropped, 5);
	// calculate Adh02a
	inspxThresholdNegative(_ImageAd1[k], &pixelsFound, _ImageAb[k], N, H[6]);
	//if (pixelsFound)
	//	te = pixelsFound;
	// Ad = (Adh03 | Adh04 | Adh05) & (Adv03 | Adv05 | Adv06) | Adh01 | Adh02 | Adh02a
	inspxImageOR(_ImageAd[k], _ImageAd1[k], N);
	// calculate Abv3
	inspxRemoveBackgroundImageByRows(_ImageAb[k], _ImageAlogT[k], columnsCropped, rowsCropped, 3);
	// calculate Adv01
	inspxThresholdNegative(_ImageAd2[k], &pixelsFound, _ImageAb[k], N, V[3]);
	//if (pixelsFound)
	//	te = pixelsFound;
	// calculate Adv02
	inspxThresholdNegative(_ImageAd1[k], &pixelsFound, _ImageAbv5[k], N, V[5]);
	//if (pixelsFound)
	//	te = pixelsFound;
	// Ad2 = Adv01 | Adv02
	inspxImageOR(_ImageAd2[k], _ImageAd1[k], N);

	
	// calculate Alog0_v15
	inspxRemoveBackgroundTrendByRows(_ImageAlog0[k], _ImageAlogT[k], columnsCropped, rowsCropped, 15);
	// calculate Abv5a
	inspxRemoveBackgroundImageByRows(_ImageAb[k], _ImageAlog0[k], columnsCropped, rowsCropped, 5);
	// calculate Adv02a
	inspxThresholdNegative(_ImageAd1[k], &pixelsFound, _ImageAb[k], N, V[6]);
	//if (pixelsFound)
	//	te = pixelsFound;
	// Ad2 = Adv01 | Adv02 | Adv02a
	inspxImageOR(_ImageAd2[k], _ImageAd1[k], N);
	
	inspxTransposeU8((BYTE *)(_ImageAd1[k]), (BYTE *)(_ImageAd2[k]), columnsCropped, rowsCropped);
	// Ad = (Adh03 | Adh04 | Adh05) & (Adv03 | Adv05 | Adv06) | Adh01 | Adh02 | Adh02a | Adv01 | Adv02 | Adv02a
	inspxImageOR(_ImageAd[k], _ImageAd1[k], N);
	// resulting binary image calculated

	
	if (processEdges) 
	{
		
		inspxExtractROI(_ImageTmp1[k], imageIn, rowsIn, columnsIn, colLeft, colRight, rowUp, rowDown);

		// mark found contaminant pixels and count them
	
		*nrDetected = inspxMarkImage(_ImageTmp2[k], _ImageTmp1[k], _ImageAd[k], N);	

	
		// form output image from input image with package region replaced from marked image 
		inspxReplaceROI(imageOut, imageIn, rowsIn, columnsIn, _ImageTmp2[k], colLeft, colRight, rowUp, rowDown);
		
	
	} 
	else 
	{
		// mark found contaminant pixels and count them
		*nrDetected = inspxMarkImage(imageOut, imageIn, _ImageAd[k], N);
			

	}
	if (processEdges)
	{
		WORD TempCorrectBottom = colLeft;
		WORD TempCorrectLeft = rowUp + 5;// add the 5 as most packages are skewed a bit, but not needed
		WORD TempCorrectTop = colRight - 5;
		WORD TempCorrectRight = rowDown - 5;

		// blank edge areas so can see in image where it found edges
		//	int colLeft, colRight, rowUp, rowDown;
		WORD *TempPointer = imageOut;
		WORD TempLoopX = 0;

		for (TempLoopX = 0; TempLoopX < TempCorrectLeft; TempLoopX++)  //blank out left side
		{
			for (WORD TempLoopY = 0; TempLoopY < rowsIn; TempLoopY++)
				*TempPointer++ = 0xFE00;
		}
		TempPointer = imageOut + (rowsIn * TempCorrectRight);
		for (TempLoopX = TempCorrectRight; TempLoopX < columnsIn; TempLoopX++)  //blank out right side
		{
			for (WORD TempLoopY = 0; TempLoopY < rowsIn; TempLoopY++)
				*TempPointer++ = 0xFE00;
		}
		
		//blank out bottom and top
		for (TempLoopX = TempCorrectLeft; TempLoopX < TempCorrectRight; TempLoopX++)  
		{
			TempPointer = imageOut + (rowsIn * TempLoopX); //blank out bottom
			for (WORD TempLoopY = 0; TempLoopY < TempCorrectBottom; TempLoopY++)
				*TempPointer++ = 0xFE00;

			TempPointer = imageOut + (rowsIn * TempLoopX) + TempCorrectTop; //blank out top
			for (WORD TempLoopY = TempCorrectTop; TempLoopY < rowsIn; TempLoopY++)
				*TempPointer++ = 0xFE00;
		}


		/*
		{
			CString TempString("TestImageMask.bmp");
			BYTE TempLength = TempString.GetLength();
			if (PasswordOK(cTemporaryInspxPassword,false))
			if (TempLength > MAX_PATH)
			{
				CNoticeDialog TempNoticeDialog;
				CString TempString = "\n\n\nSetup Name more the MAX_PATH characters, Offset Image save aborted";
				TempNoticeDialog.vNoticeText = TempString;
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
				TempLength = MAX_PATH;
			}
			else
			{
				LPTSTR TempString1 = TempString.GetBuffer(MAX_PATH); //TempString.GetLength());

				CImgFile *TempImageFile=IfxCreateImgFile(TempString1);
				if (TempImageFile) 
				{
					TempImageFile->WriteFile((BYTE*)imageOut, 384, 384, 16);
					delete TempImageFile;
				}
			}
		
		}
	*/

	
  
	}


	

	//WriteImageToHardDisk(TempPointer, "TestImageMask.bmp", 384, 384);
	
	return INSPX_OK;
}

/**
 *	Transposes images with pixels of type <code>short</code>.
 *
 *	\param imageOut	pointer to output image (<code>columnsIn*rowsIn</code>) (allocated by caller)
 *	\param imageIn 	pointer to input image (<code>rowsIn*columnsIn</code>), placed in memory by rows
 *	\param rowsIn	number of rows in input image / number of columns in output image
 *	\param columnsIn number of columns in input image / number of rows in output image
 *  \sa inspxUpdateThresholds(), inspxDetect(), inspxTransposeU16(), inspxTransposeU8()
 */
void CInspection::inspxTransposeI16(short *imageOut, short *imageIn, int rowsIn, int columnsIn) 
{
	short *ptrRowIn=imageIn,  *ptrColumnStartOut=imageOut, *ptrColumnOut;
	int row, column, rows=rowsIn, columns=columnsIn;
	
	for (row=0; row<rows; row++) {
		ptrColumnOut = ptrColumnStartOut;
		for (column=0; column<columns; column++) {
			*ptrColumnOut = *ptrRowIn++;
			ptrColumnOut += rows;
		}
		ptrColumnStartOut++;
	}
}

/**
 *	Transposes images with pixels of type <code>WORD</code>.
 *
 *	\param imageOut	pointer to output image pixels (<code>columnsIn*rowsIn</code>) (allocated by caller)
 *	\param imageIn 	pointer to input image pixels (<code>rowsIn*columnsIn</code>), placed in memory by rows
 *	\param rowsIn	number of rows in input image / number of columns in output image
 *	\param columnsIn number of columns in input image / number of rows in output image
 *  \sa inspxProcessEdges(), inspxTransposeI16(), inspxTransposeU8()
 */
void CInspection::inspxTransposeU16(WORD *imageOut, WORD *imageIn, int rowsIn, int columnsIn) 
{
	WORD *ptrRowIn=imageIn,  *ptrColumnStartOut=imageOut, *ptrColumnOut;
	int row, column, rows=rowsIn, columns=columnsIn;

	for (row=0; row<rows; row++) {
		ptrColumnOut = ptrColumnStartOut;
		for (column=0; column<columns; column++) {
			*ptrColumnOut = *ptrRowIn++;
			ptrColumnOut += rows;
		}
		ptrColumnStartOut++;
	}
}

/**
 *	Transposes images with pixels of type <code>unsigned char</code>.
 *
 *	\param imageOut	pointer to output image pixels (<code>columnsIn*rowsIn</code>) (allocated by caller)
 *	\param imageIn 	pointer to input image pixels (<code>rowsIn*columnsIn</code>), placed in memory by rows
 *	\param rowsIn	number of rows in input image / number of columns in output image
 *	\param columnsIn number of columns in input image / number of rows in output image
 *  \sa inspxDetect(), inspxTransposeU16(), inspxTransposeI16()
 */
void CInspection::inspxTransposeU8(unsigned char *imageOut, unsigned char *imageIn, int rowsIn, int columnsIn) 
{
	BOOL *ptrRowIn = (BOOL *)(imageIn); 
	BOOL *ptrColumnStartOut = (BOOL *)imageOut;
	BOOL *ptrColumnOut;
	int row, column, rows=rowsIn, columns=columnsIn;

	for (row=0; row<rows; row++) {
		ptrColumnOut = ptrColumnStartOut;
		for (column=0; column<columns; column++) {
			*ptrColumnOut = *ptrRowIn++;
			ptrColumnOut += rows;
		}
		ptrColumnStartOut++;
	}
}

/**
 *	Copies images with pixels of type <code>WORD</code>.
 *
 *	\param imageOut			pointer to output image pixels (<code>rowsIn*columnsIn</code>) (allocated by caller)
 *	\param imageIn 			pointer to input image pixels (<code>rowsIn*columnsIn</code>)
 *	\param numberOfPixels	number of pixels in input and output images
 *
 *  \sa InspxContaminant22InspectionW(), inspxTransposeI16(), inspxTransposeU8()
 */
void CInspection::inspxCopyU16(WORD *imageOut, WORD *imageIn, int numberOfPixels)
{
	int iPixel, N = numberOfPixels;
	WORD *ptrIn = imageIn, *ptrOut = imageOut;
	
	// copy input image to output
	for (iPixel=0; iPixel<N; iPixel++) *ptrOut++ = *ptrIn++;
}

/**
 * Converts BYTE image to WORD image.
 *
 * \param  imageOut pointer to output image pixels
 * \param  imageIn 	pointer to input image pixels
 * \param  numberOfPixels number of pixels in <code>imageIn</code> and <code>imageOut</code> 
 * \sa InspxContaminant22InspectionB(), InspxContaminant22ThresholdCalculationB(), inspxImageW2B()
 */
void CInspection::inspxImageB2W(WORD *imageOut, BYTE *imageIn, int numberOfPixels)
{
	WORD *ptrOut = imageOut;
	BYTE *ptrIn = imageIn;
	
	for (int i = 0; i < numberOfPixels; i++) 
	{
		*ptrOut = ((WORD)(*ptrIn)) << 8;
		ptrOut++;
		ptrIn++;
	}
}
/* original
void CInspection::inspxImageB2W(WORD *imageOut, BYTE *imageIn, int numberOfPixels)
{
	int i;
	WORD *ptrOut = imageOut;
	BYTE *ptrIn = imageIn;
	
	for (i=0; i<numberOfPixels; i++) {
		*ptrOut++ =  ((WORD)*ptrIn++)<<8;
	}
}
*/


/**
 * Converts WORD image to BYTE image.
 *
 * \param  imageOut pointer to output image pixels
 * \param  imageIn 	pointer to input image pixels
 * \param  numberOfPixels number of pixels in <code>imageIn</code> and <code>imageOut</code> 
 * \sa InspxContaminant22InspectionB(), inspxImageB2W()
 */
void CInspection::inspxImageW2B(BYTE *imageOut, WORD *imageIn, int numberOfPixels)
{
	BYTE *ptrOut = imageOut;
	WORD *ptrIn = imageIn;
	
	for (int i = 0; i < numberOfPixels; i++) 
	{
		*ptrOut++ = (BYTE)((*ptrIn++) >> 8);
	}
}
/* original
void CInspection::inspxImageW2B(BYTE *imageOut, WORD *imageIn, int numberOfPixels)
{
	int i;
	BYTE *ptrOut = imageOut;
	WORD *ptrIn = imageIn;
	
	for (i=0; i<numberOfPixels; i++) {
		*ptrOut++ =  (BYTE)((*ptrIn++)>>8);
	}
}
*/


/**
 * Applies logarithmic operator to input image pixel by pixel to linearize X-ray response.
 * Output pixel value <code>y</code> is obtained from input pixel value <code>x</code> as follows: <code>y=log2(x)*1024</code>.
 * Output codes are stored in a look-up-table <code>lutLog2_1024_16bit</code>.
 *
 * \param imageOut	pointer to output image (allocated by caller)
 * \param imageIn 	pointer to input image 
 * \param  numberOfPixels total number of pixels in images
 */
void CInspection::inspxImageLog(short *imageOut, WORD *imageIn, int numberOfPixels)
{
	int i, n = numberOfPixels;
	short *ptrOut = imageOut;
	WORD *ptrIn = imageIn;
	short *logs = lutLog2_1024_16bit;

	for (i=0; i<n; i++) *ptrOut++ = *(logs + (*ptrIn++));
}

/**
 * Finds minimum amd maximum pixel values in input image.
 *
 * \param  imageIn 	pointer to input image pixels
 * \param  numberOfPixels total number of pixels in <code>imageIn</code>
 * \param  minValue minimum value, passed by reference
 * \param  maxValue maximum value, passed by reference
 * \sa inspxImageMinRows(), inspxImageMinColumns()
 */
void CInspection::inspxImageMinMax(short *imageIn, int numberOfPixels, short *minValue, short *maxValue)
{
	int i, n = numberOfPixels;
	short *ptrIn = imageIn, _min = 32767, _max = -32768, _curr;	//, t;

	for (i=0; i<n; i++) {
		
		_curr = *ptrIn++;
		
		
		if (_curr >= -9000)		// ams otherwise background may supress all image minima
		{
			if (_curr > _max) 
				_max = _curr;
			if (_curr < _min) 
				_min = _curr;
		}
		
	}
	*minValue = _min;
	*maxValue = _max;
}

/**
 *	Calculates vector of minimum values obtained within rows of the image. 
 *  If required, provides also vector of indices.
 *
 * \param  minVectorOut pointer to output vector containing minimum values found
 * \param  indOut pointer to output vector containing indices of minimum values found
 * \param  imageIn 	pointer to input image pixels
 * \param rows	number of rows in input image
 * \param columns number of columns in input image
 * \return number of true pixels in "imageFound"
 * \sa inspxPackageLimits()
 */
void CInspection::inspxImageMinRows(WORD *minVectorOut, int *indOut, WORD *imageIn, int rows, int columns)
{
	int *ptrInd = indOut;
	short *ptrIn = (short *)imageIn;
	short *ptrOut = (short *)minVectorOut;
	short _min, _curr, _ind;

	for (int irow = 0; irow < rows; irow++) 
	{
		_min = *ptrIn;
		_ind = 0;
		for (int icolumn = 0; icolumn < columns; icolumn++) 
		{
			_curr = *ptrIn++;
			if (_curr < _min) 
			{ 
				_min = _curr;
				_ind = icolumn;
			}
		}
		*ptrOut++ = _min;
		if (indOut != NULL) 
			*ptrInd++ = _ind;
	}
}

/**
 *	Calculates vector of minimum values obtained within columns of the image. 
 *  If required, provides also vector of indices.
 *
 * \param  minVectorOut pointer to output vector containing minimum values found
 * \param  indOut pointer to output vector containing indices of minimum values found
 * \param  imageIn 	pointer to input image pixels
 * \param rows	number of rows in input image
 * \param columns number of columns in input image
 * \return number of true pixels in "imageFound"
 * \sa inspxPackageLimits(), inspxEdgePointsLeftRight()
 */
void CInspection::inspxImageMinColumns(WORD *minVectorOut, int *indOut, WORD *imageIn, int rows, int columns)
{
	int irow, icolumn, *ptrInd = indOut;
	WORD *ptrIn = imageIn, *ptrOut=minVectorOut, _min, _curr, _ind, *ptrCol;

	
	for (icolumn=0; icolumn<columns; icolumn++) {
		ptrCol = ptrIn;
		//ptrIn++;
		_min = *ptrIn++;
		_ind = 0;
		for (irow=1; irow<rows; irow++) {	// ams 1 as first index the same 
			_curr = *ptrCol;
			if (_curr < _min) { 
				_min = _curr;
				_ind = irow;
			}
			ptrCol += columns;
		}
		*ptrOut++ = _min;
		if (indOut!=NULL) *ptrInd++ = _ind;
	}
}

/**
 *	Apply threshold to input image to obtain binary image and count the number of pixels found below threshold. 
 *	Pixel in output image is set to 'true' if corresponding pixel's value in input image was below the threshold.
 *
 *	\param imageOut	pointer to output image. 
 *			If NULL, output image is not used and only number of found are counted. 
 *			If needed, must be allocated by caller 
 *	\param numberOfPixelsFound number of pixels found below threshold (passed by reference)
 *	\param imageIn 	pointer to input image pixels (size = numberOfPixels)
 *	\param numberOfPixels	number of pixels in input and output images
 *	\param threshold threshold value to compare with
 *  \sa inspxDetect()
 */
void CInspection::inspxThresholdNegative(BOOL *imageOut, int *numberOfPixelsFound, short *imageIn, int numberOfPixels, short threshold) 
{
	short *ptrIn = imageIn;
	BOOL res, *ptrOut = imageOut;
	int i, found = 0, n = numberOfPixels;
	
	if (ptrOut==NULL) {
		for (i=0; i<n; i++) {
			res = ((*ptrIn++) < threshold);
			if (res) found++;
		}
	} else {
		for (i=0; i<n; i++) {
			res = ((*ptrIn++) < threshold);
			*ptrOut++ = res;
			if (res) found++;
		}
	}
	*numberOfPixelsFound = found;
}

/**
 * ANDs two images pixel by pixel. Result is returned in the first image,
 *
 * \param  image1  	pointer to first image to AND (
 * \param  image2	pointer to second image to AND
 * \param  numberOfPixels total number of pixels in images
 * \sa inspxImageOR(), inspxDetect()
 */
void CInspection::inspxImageAND(BOOL *image1, BOOL *image2, int numberOfPixels) 
{
	BOOL *ptr1 = image1, *ptr2 = image2, tmp;
	int i, n = numberOfPixels;
	
	for (i=0; i<n; i++) {
		tmp = (*ptr1) & (*ptr2++);
		*ptr1++ = tmp;	
	}
}

/**
 * ORs two images pixel by pixel. Result is returned in the first image,
 *
 * \param  image1  	pointer to first image to OR 
 * \param  image2	pointer to second image to OR
 * \param  numberOfPixels total number of pixels in images
 * \sa inspxImageAND(), inspxDetect()
 */
void CInspection::inspxImageOR(BOOL *image1, BOOL *image2, int numberOfPixels) 
{
	BOOL *ptr1 = image1, *ptr2 = image2, tmp;
	int i, n = numberOfPixels;
	
	for (i=0; i<n; i++) {
		tmp = (*ptr1) | (*ptr2++);
		*ptr1++ = tmp;	
	}
}

/**
 *	Marks found contaminant pixels with maximum brightness (INSPX_MAX_PIXEL_VALUE).
 *
 * \param  imageOut pointer to output image (marked) pixels
 * \param  imageIn 	pointer to input image pixels
 * \param  imageFound 	pointer to boolean image pixels where found contaminant pixels are true
 * \param  numberOfPixels number of pixels in <code>imageIn</code>, <code>imageFound</code> and <code>imageOut</code> 
 * \return number of true pixels in "imageFound"
 * \sa inspxDetect()
 */
int CInspection::inspxMarkImage(WORD *imageOut, WORD *imageIn, BOOL *imageFound, int numberOfPixels)
{
	int i, pixelsFound=0;
	BOOL *ptrFound = imageFound;
	WORD *ptrIn = imageIn; 
	WORD *ptrOut = imageOut;
	WORD tmp;
	
	for (i=0; i<numberOfPixels; i++) {
		tmp = *ptrIn++;
		if (*ptrFound++) {
			tmp = INSPX_MAX_PIXEL_VALUE;
			pixelsFound++;
		}
		*ptrOut++ = tmp;
	}
	return pixelsFound;
}


/**
 *	Apply averaging filter to one line of the image to roughly remove background trend.
 *	Value of the output vector is calculated by subtracting average value of pixels surrounding the corresponding value in input vector from the value of the input vector.
 *	Number of values taken to obtain one output value (<code>filterLength</code>) must be even so that the pixel for which the output value is calculated is in the center.
 *	To calculate average values at the start (or end) of the line, it is padded with values of the first (or last) pixel of the line.
 *
 *	\param outputVector	pointer to output line pixels (allocated by caller, size = <code>numberOfElements</code>). 
 *	\param inputVector	pointer to input line pixels (size = <code>numberOfElements</code>). 
 *	\param numberOfElements	number of pixels in input and output lines (must be >=<code>filterLength</code>)
 *	\param filterLength length of the averaging filter (must be even, >2 and <= <code>INSPX_MAX_FILTER_LENGTH</code>)
 *  \sa inspxRemoveBackgroundLine(), inspxRemoveBackgroundTrendByRows(), INSPX_MAX_FILTER_LENGTH
 */
void CInspection::inspxRemoveTrendLine(short *outputVector, short *inputVector, int numberOfElements, int filterLength)
{
	int i,last;
	int width2 = (filterLength-1)/2;
	short *pOldValue, *ptrNewValue, *ptrMiddleVal;
	int filterLength1 = filterLength - 1;
	int tmpVal;
	int filterLengthI = filterLength;

	// prepare sum of pixel values to calculate mean from
	// assume pixel values left to the first pixel to be equal to first pixel value 
	tmpVal = ((int)*inputVector)*(width2+1);
	
	// add width2 pixel values to tmpVal without writing new value
	ptrNewValue = inputVector+1;
	for (i=0; i<width2; i++) { 
		 tmpVal += (int)(*ptrNewValue++);
	}
	
	ptrMiddleVal = inputVector;		// pointer to currently processed pixel in input line
	pOldValue = inputVector;
	for (i=0; i<width2; i++) { 
    	*outputVector++ =  (*ptrMiddleVal++) - (short)(tmpVal/filterLengthI);
		tmpVal += (int)((*ptrNewValue++) - (*pOldValue));
	}

	// go through the line calculating new values. In each step:
	// calculate new value as difference between old and mean=tmpVal/filterLength
	// add new value and subtract old values to/from tmpVal
	last = numberOfElements-filterLength;
    for (i=0; i<last; i++) {
    	*outputVector++ =  (*ptrMiddleVal++) - (short)(tmpVal/filterLengthI);
    	if (i==(last-1)) break;
		tmpVal += (int)((*ptrNewValue++) - (*pOldValue++));
	}
	
	// process last values assuming new value equal to value of the last pixel 
	for (i=0; i<=width2; i++) { 
    	*outputVector++ =  (*ptrMiddleVal++) - (short)(tmpVal/filterLengthI);
    	if (i==width2) break;
		tmpVal += (int)((*ptrNewValue) - (*pOldValue++));
	}
}

/**
 *	Apply median filter to one line of the image to extract values related to small foreground objects and remove background trend.
 *	Value of the output vector is calculated by subtracting median value of pixels surrounding the corresponding value in input vector from the value of the input vector.
 *	Number of values taken to obtain one output value (<code>filterLength</code>) must be even so that the pixel for which the output value is calculated is in the center.
 *	To calculate median values at the start (or end) of the line, it is padded with values of the first (or last) pixel of the line.
 *
 *	\param outputVector	pointer to output line pixels (allocated by caller, size = <code>numberOfElements</code>). 
 *	\param inputVector	pointer to input line pixels (size = <code>numberOfElements</code>). 
 *	\param numberOfElements	number of pixels in input and output lines (must be >=<code>filterLength</code>)
 *	\param filterLength length of the median filter (must be even, >2 and <= <code>INSPX_MAX_FILTER_LENGTH</code>)
 *  \sa inspxRemoveTrendLine(), inspxRemoveBackgroundImageByRows(), INSPX_MAX_FILTER_LENGTH
 */
void CInspection::inspxRemoveBackgroundLine(short *outputVector, short *inputVector, int numberOfElements, int filterLength)
{
	int i,last;
	int width2 = (filterLength-1)/2;
	short value1, valueLast;
	short sortedArray[INSPX_MAX_FILTER_LENGTH];
	short *ptmp, *ptrNewValue, *ptrMed, *ptrMiddleVal;
	register short oldVal, newVal;
	int filterLength1 = filterLength - 1;

	// form initial sorted list from 'filterLength' values = the first pixel value in line
	ptmp = sortedArray; 
	value1 = *inputVector;
	valueLast = *(inputVector+numberOfElements-1);
	for (i=0; i<filterLength; i++) *ptmp++ = value1;
	
	// shift sorted list width2 times without writing new value
	ptrNewValue = inputVector+1;
	for (i=0; i<width2; i++) { 
    	oldVal = value1;
		newVal = *ptrNewValue++;
		#include "shiftSortedArray.h"
	}
	
	ptrMed = sortedArray + width2;	// pointer to median value in sorted array
	ptrMiddleVal = inputVector;		// pointer to currently processed pixel in input line

	// shift sorted list width2 times with writing new value but taking old values as = value1
	for (i=0; i<width2; i++) { 
    	*outputVector++ =  *ptrMiddleVal++ - *ptrMed;
    	oldVal = value1;
		newVal = *ptrNewValue++;
		#include "shiftSortedArray.h"
	}

	// shift sorted list with writing new value and taking old values from line
	last = numberOfElements-filterLength;
    for (i=0; i<last; i++) {
    	*outputVector++ =  *ptrMiddleVal++ - *ptrMed;
    	if (i==last) break;

    	oldVal = *inputVector++;
		newVal = *ptrNewValue++;
		#include "shiftSortedArray.h"
	}
	
	// shift sorted list width2+1 times with writing new value but taking new values as = last pixel value
	for (i=0; i<=width2; i++) { 
    	*outputVector++ =  *ptrMiddleVal++ - *ptrMed;
    	if (i==width2) break;
    	oldVal = *inputVector++;
		newVal = valueLast;
		#include "shiftSortedArray.h"
	}
}

/**
 *	Apply mean filter to the image row by row to remove background trend.
 *  For that, <code>inspxRemoveTrendLine</code> is applied to all rows of the image. Image structure by columns is not analyzed.  
 *
 *	\param imageOut	pointer to output image pixels placed in memory by rows (allocated by caller, size = <code>numberOfRows x numberOfColumns</code>) 
 *	\param imageIn	pointer to input image pixels placed in memory by rows (size = <code>numberOfRows x numberOfColumns</code>) 
 *	\param numberOfRows	number of rows in input and output images 
 *	\param numberOfColumns	number of columns in input and output images (must be >=<code>filterLength</code>)
 *	\param filterLength length of the median filter (must be even, >2 and <= <code>INSPX_MAX_FILTER_LENGTH</code>)
 *  \sa inspxDetect(), inspxRemoveTrendLine(), inspxRemoveBackgroundImageByRows(), INSPX_MAX_FILTER_LENGTH
 */
void CInspection::inspxRemoveBackgroundTrendByRows(short *imageOut, short *imageIn, int numberOfRows, int numberOfColumns, int filterLength)
{
	int row;
	short *ptrIn = imageIn;
	short *ptrOut = imageOut;

	for (row=0; row<numberOfRows; row++) {
		inspxRemoveTrendLine(ptrOut, ptrIn, numberOfColumns, filterLength);
		ptrIn += numberOfColumns;	// go to next line
		ptrOut += numberOfColumns;
	}
}

/**
 *	Apply median filter to the image row by row to extract values related to small foreground objects and remove background trend.
 *  For that, <code>inspxRemoveBackgroundLine</code> is applied to all rows of the image. Image structure by columns is not analyzed. Small vertical objects are extracted. 
 *
 *	\param imageOut	pointer to output image pixels placed in memory by rows (allocated by caller, size = <code>numberOfRows x numberOfColumns</code>) 
 *	\param imageIn	pointer to input image pixels placed in memory by rows (size = <code>numberOfRows x numberOfColumns</code>) 
 *	\param numberOfRows	number of rows in input and output images 
 *	\param numberOfColumns	number of columns in input and output images (must be >=<code>filterLength</code>)
 *	\param filterLength length of the median filter (must be even, >2 and <= <code>INSPX_MAX_FILTER_LENGTH</code>)
 *  \sa inspxDetect(), inspxRemoveBackgroundLine(), inspxRemoveBackgroundTrendByRows(), INSPX_MAX_FILTER_LENGTH
 */
void CInspection::inspxRemoveBackgroundImageByRows(short *imageOut, short *imageIn, int numberOfRows, int numberOfColumns, int filterLength)
{
	int row;
	short *ptrIn = imageIn;
	short *ptrOut = imageOut;

	for (row=0; row<numberOfRows; row++) 
	{
		inspxRemoveBackgroundLine(ptrOut, ptrIn, numberOfColumns, filterLength);
		ptrIn += numberOfColumns;	// go to next line
		ptrOut += numberOfColumns;
	}
}

double CInspection::ConvertToWeight(double TempDensity)
{
	double TempWeight = TempDensity;
	if ((vInspectionType == cUnderfillByWeightInspection) || (vInspectionType == cOverfillByWeightInspection) || (vInspectionType == cCheckWeighInspection))
	//if (TempDensity > 5000)  //must be over 5000 as under 5000 must be weight in ounces or grams already, not a pixel value
	if (((vCheckWeighMFactor != 1) || (vCheckWeighBFactor != 0)) && (vCheckWeighMFactor)) //must have calibration factors
	{
		TempWeight = (TempWeight * vCheckWeighMFactor) + vCheckWeighBFactor;
		if (TempWeight < 0)
			TempWeight = 0;
	}
	return TempWeight;
}

double CInspection::ConvertWeightToBrightness(double TempWeight)
{
	double TempBrightness = TempWeight;
	if ((TempWeight < 5000) && (TempWeight))  //must be under 5000 as under 5000 must be weight in ounces or grams already, not a pixel value
	if (((vCheckWeighMFactor != 1) || (vCheckWeighBFactor != 0)) && (vCheckWeighMFactor)) //must have calibration factors
	if (TempWeight > vCheckWeighBFactor) //weight entered is less than offset, so can not convert
	{
		TempBrightness = ((TempWeight - vCheckWeighBFactor) / vCheckWeighMFactor);
		if (TempBrightness < 0)
			TempBrightness = 0;
	}
	return TempBrightness;
}

double CInspection::GetSensitivity(BOOL TempUsePassedThreshold, double TempPassedThreshold)
{
	//this->vName
	if (!TempUsePassedThreshold)
		TempPassedThreshold = vThreshold;

	if (vAddStructure) //using learned algorithm
	{
		if ((TempPassedThreshold == 0) || (TempPassedThreshold == 255))
			return 0;
		else
		if (TempPassedThreshold >= 245)
			return 100;
		else
		if (TempPassedThreshold <= 140)
			return 0;
		else
		{
			double TempSensitivity = (TempPassedThreshold - 175.0) * 100.0 / 70.0;

			if (vInspectionType == cTopContaminantInspection)
				TempSensitivity = (TempPassedThreshold - 135.0) * 100.0 / 110.0;

			if (TempSensitivity > 100)
				TempSensitivity = 100;
			if (TempSensitivity < 0)
				TempSensitivity = 0;
			return  TempSensitivity; // * 100 to make percent, / 70 as 70 is the range to make range 1
		}
	}
	else
	if (vHasDensityUpperThreshold)//density inspection, missing lid, underfill
	{
		if ((vDensityThresholdUpper == 0) || (vDensityThresholdUpper == 255))
			return 0;
		else
			return 255;
			/*
		if (!TempUsePassedThreshold)
			TempPassedThreshold = vDensityThresholdUpper;
		BYTE TempReferenceThreshold = 128;
		if (vGlobalCurrentProduct)
		if (vGlobalCurrentProduct->vSetupHistoryAverageDensity)
			TempReferenceThreshold = (BYTE)vGlobalCurrentProduct->vSetupHistoryAverageDensity;
		BYTE TempRange = 255 - TempReferenceThreshold;
		BYTE TempPlotMargin = (BYTE)(TempRange * .3);
		BYTE TempPlotReference = TempReferenceThreshold + TempPlotMargin;
		BYTE TempPlotRange = (BYTE)(TempRange * .7);

		if (TempPassedThreshold <= TempPlotReference)
			return 100;
		else
		if (TempPassedThreshold >= 250)
			return 2;
		else
		{
			double TempAmountOverReference = TempPassedThreshold - TempPlotReference;
			double TempPercentRangeOverReference = TempAmountOverReference / TempPlotRange * 100;
			if (TempPercentRangeOverReference < 0)
				TempPercentRangeOverReference = 0;
			if (TempPercentRangeOverReference > 100)
				TempPercentRangeOverReference = 100;

			return (100 - TempPercentRangeOverReference); //invert as closer to reference is more sensitive
		}
		*/
	}
	else
	if (vHasDensityLowerThreshold)//density inspection, overfill
	{
		if ((vDensityThresholdLower == 0) || (vDensityThresholdLower == 255))
			return 0;
		else
			return 255;
			/*
		if (!TempUsePassedThreshold)
			TempPassedThreshold = TempPassedThreshold;
		BYTE TempReferenceThreshold = 128;
		if (vGlobalCurrentProduct)
		if (vGlobalCurrentProduct->vSetupHistoryAverageDensity)
			TempReferenceThreshold = (BYTE)(vGlobalCurrentProduct->vSetupHistoryAverageDensity);
		BYTE TempRange = TempReferenceThreshold;
		BYTE TempPlotMargin = (BYTE)(TempRange * .3);
		BYTE TempPlotReference = TempReferenceThreshold - TempPlotMargin;
		BYTE TempPlotRange = (BYTE)(TempRange * .7);

		if (TempPassedThreshold >= TempPlotReference)
			return 100;
		else
		if (TempPassedThreshold <= 10)
			return 2;
		else
			//return (BYTE)((TempPassedThreshold - 125) * 100 / 120);  // * 100 to make percent, / 120 as 120 is the range to make range 1
		{
			double TempAmountUnderReference = TempPlotReference - TempPassedThreshold;
			double TempPercentRangeUnderReference = TempAmountUnderReference / TempPlotRange * 100;
			if (TempPercentRangeUnderReference < 0)
				TempPercentRangeUnderReference = 0;
			if (TempPercentRangeUnderReference > 100)
				TempPercentRangeUnderReference = 100;

			return (100 - TempPercentRangeUnderReference); //invert as closer to reference is more sensitive
		}
		*/
	}
	else
	{ //using mathematical algorithm
		if (vPerformGlassAlgorithm)
		{ //Glass in Body
			if ((TempPassedThreshold == 0) || (TempPassedThreshold == 255))
				return 0;
			else
			if (TempPassedThreshold >= 245)
				return 100;
			else
			if (TempPassedThreshold <= 150) //was 215
				return 2;
			else
				return (BYTE)((TempPassedThreshold - 150) * 100 / 95);  // * 100 to make percent, / 105 as 105 is the range to make range 1
		}
		else 
		if ((vPerformGlassAlgorithm) || (vTakeDifferenceFromMedian) || (vFindSpotInSubROI) || (vBonesInspection) || (vLookForDarkSpots))
		{ //contaminant P, Q, R, Spot in ROI, Bones, Glass On Bottom
			if ((TempPassedThreshold == 0) || (TempPassedThreshold == 255))
				return 0;
			else
			if (TempPassedThreshold >= 245)
				return 100;
			else
			if (TempPassedThreshold <= 215) //was 215
				return 2;
			else
				return (BYTE)((TempPassedThreshold - 215) * 100 / 30);  // * 100 to make percent, / 30 as 30 is the range to make range 1
		}
		else 
		{ //Contaminant T Inspection
			if ((TempPassedThreshold == 0) || (TempPassedThreshold == 255))
				return 0;
			else
				return 255;
			/*
			//10 is least sensitive
			//reference brightness is most sensitive

			BYTE TempReferenceThreshold = 128;
			if (vGlobalCurrentProduct)
			if (vGlobalCurrentProduct->vSetupHistoryAverageDensity)
				TempReferenceThreshold = (BYTE)(vGlobalCurrentProduct->vSetupHistoryAverageDensity);
			BYTE TempRange = TempReferenceThreshold;
			BYTE TempPlotMargin = (BYTE)(TempRange * .3);
			BYTE TempPlotReference = TempReferenceThreshold - TempPlotMargin;
			BYTE TempPlotRange = (BYTE)(TempRange * .7);

			if (TempPassedThreshold >= TempPlotReference)
				return 100;
			else
			if (TempPassedThreshold <= 10)
				return 2;
			else
			{
				double TempAmountUnderReference = TempPlotReference - TempPassedThreshold;
				double TempPercentRangeUnderReference = TempAmountUnderReference / TempPlotRange * 100;
				if (TempPercentRangeUnderReference < 0)
					TempPercentRangeUnderReference = 0;
				if (TempPercentRangeUnderReference > 100)
					TempPercentRangeUnderReference = 100;

				return (100 - TempPercentRangeUnderReference); //invert as closer to reference is more sensitive
			}
			*/
		}
	}
}

double CInspection::AnalyzeImage(BYTE *TempImageIn, BYTE *TempImageReturn, BYTE TempThreshold)
{
	double TempEjectIt = 0;

	CopyMemory(TempImageReturn, TempImageIn, vGlobalITIPCDig->vOriginalBufferSize);

	CopyMemory(vGlobalITIPCDig->vPQLIScratchImage->pix, TempImageIn, vGlobalITIPCDig->vOriginalBufferSize);

	WORD TempBottom = 0;
	WORD TempLeft = 0;
	WORD TempHeight = 0;
	WORD TempWidth = 0;

	if ((!vAddStructure) || (vGlobalITIPCDig->vProductContaminant2StructureImage)) //don't do inspection if no structure and using
	if (vEnable)
	//don't do if Top contaminant, and learned and learn state above 3
	{
		BYTE TempAutoImproveState = vGlobalLearnState;
		if (!vAddStructure)
			TempAutoImproveState = 0;
		else
		if ((vGlobalConfigurationData->vContinuousFeedContainerRate) || (vGlobalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) || (vGlobalScanTracType == cAtempoScanTrac) || 
			(vGlobalScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) || (vGlobalScanTracType == cCaseInspectorScanTrac)) //Continuous Feed, or a Pipeline or TD
			TempAutoImproveState = 0;
		else
		if (!vGlobalConfigurationData->vAutoImproveEnabled)
			TempAutoImproveState = 0;

		vGlobalITIPCDig->GetInspectionROIBounds(this, &TempBottom, &TempLeft, &TempHeight, &TempWidth, 0,//already referenced vGlobalITIPCDig->vLastLeftReferenceAdjust, 
			vGlobalConfigurationData->vReferenceROIsToEdges, TempAutoImproveState);

		if (vFillBackground)
			FillBackground(vGlobalITIPCDig->vPQLIScratchImage->pix, vGlobalITIPCDig->vOriginalBufferSizeX, vGlobalITIPCDig->vOriginalBufferSizeY);

		if (vFillInLightSpots)
		{
			FillInLightSpots(vGlobalITIPCDig->vPQLIScratchImage->pix, vGlobalITIPCDig->vOriginalBufferSizeX, vGlobalITIPCDig->vOriginalBufferSizeY, vFillInLightSpots);
		}

		if (vPreSmoothImage)
			vGlobalITIPCDig->MedianSmoothImage(vGlobalITIPCDig->vPQLIScratchImage, vPreSmoothImage, TempLeft, TempWidth, TempBottom, TempHeight);

		if (vErodeCount)  //is variable to hold erode times count
		if ((vGlobalITIPCDig->vPQLIScratchImage2) && (vGlobalITIPCDig->vPQLIScratchImage2->pix))
		for (BYTE TempLoop = 0; TempLoop < vErodeCount; TempLoop++)
			ErodeImage(vGlobalITIPCDig->vPQLIScratchImage, vGlobalITIPCDig->vPQLIScratchImage2);

		if (vDilationCount)
		if ((vGlobalITIPCDig->vPQLIScratchImage2) && (vGlobalITIPCDig->vPQLIScratchImage2->pix))
		for (BYTE TempLoop = 0; TempLoop < vDilationCount; TempLoop++)
			DilateImage(vGlobalITIPCDig->vPQLIScratchImage, vGlobalITIPCDig->vPQLIScratchImage2);

		if (vRemoveJarPunt)
			RemoveJarPunt(vGlobalITIPCDig->vPQLIScratchImage->pix,vGlobalITIPCDig->vOriginalBufferSizeX, vGlobalITIPCDig->vOriginalBufferSizeY, 
				vGlobalITIPCDig->vLastLeft - vGlobalITIPCDig->vLastLeftReferenceAdjust, vGlobalITIPCDig->vLastRight - vGlobalITIPCDig->vLastLeftReferenceAdjust, vGlobalCurrentProduct->vSetupHistoryAverageDensity, 0);

		if (vTrimEdgesOfJar)
		if (!vDoContaminant22Algorithm)
		{  //trim from edge of jar, not edge of ROI
			WORD TempLeft = vGlobalITIPCDig->vLastLeft;
			WORD TempRight = vGlobalITIPCDig->vLastRight;

			if (TempRight >= vGlobalITIPCDig->vOriginalBufferSizeY)
				TempRight = vGlobalITIPCDig->vOriginalBufferSizeY - 1;
			if (TempLeft >= TempRight)
				TempLeft = 1;

			TrimEdgesOfJarAtROI(vGlobalITIPCDig->vPQLIScratchImage->pix, vGlobalITIPCDig->vOriginalBufferSizeX, vGlobalITIPCDig->vOriginalBufferSizeY, true,
				vGlobalITIPCDig->vLastLeft - vGlobalITIPCDig->vLastLeftReferenceAdjust, vGlobalITIPCDig->vLastRight - vGlobalITIPCDig->vLastLeftReferenceAdjust);
		}

		//if (vFillInLightSpots)
		//	CurveFitFillInLightSpots(vGlobalITIPCDig->vPQLIScratchImage->pix, vGlobalITIPCDig->vOriginalBufferSizeX, vGlobalITIPCDig->vOriginalBufferSizeY);

		if (vTakeDerivative)
			TakeLineDerivative(vGlobalITIPCDig->vPQLIScratchImage->pix, vGlobalITIPCDig->vPQLIScratchImage2->pix, vGlobalITIPCDig->vOriginalBufferSizeX, vGlobalITIPCDig->vOriginalBufferSizeY, 
				vGlobalCurrentProduct->vSetupHistoryAverageDensity, vThreshold);

		if (vTakeDifferenceFromMedian)
			TakeDifferenceFromMedian(vGlobalITIPCDig->vPQLIScratchImage->pix,  vGlobalITIPCDig->vPQLIScratchImage2, vGlobalITIPCDig->vOriginalBufferSizeX, vGlobalITIPCDig->vOriginalBufferSizeY, vGlobalITIPCDig->vLatestImage->pix, vThreshold);

		if (vFindDarkSpotOnLine)
			FindDarkSpotOnLine(vGlobalITIPCDig->vPQLIScratchImage->pix, vGlobalITIPCDig->vPQLIScratchImage2->pix, vGlobalITIPCDig->vOriginalBufferSizeX, vGlobalITIPCDig->vOriginalBufferSizeY, vThreshold);

		if (vFindSpotInSubROI)
		{
			if (vDesensitizeIrregularArea)
			{
				CopyMemory(vGlobalITIPCDig->vDesensitizeImage->pix, vGlobalITIPCDig->vPQLIScratchImage->pix, vGlobalITIPCDig->vOriginalBufferSize);

				MakeVariablityImage(vGlobalITIPCDig->vDesensitizeImage, vGlobalITIPCDig->vPQLIScratchImage2, vGlobalITIPCDig->vOriginalBufferSizeX, vGlobalITIPCDig->vOriginalBufferSizeY, 4);
			}

			FindSpotInSubROI(vGlobalITIPCDig->vPQLIScratchImage->pix, vGlobalITIPCDig->vPQLIScratchImage2, vGlobalITIPCDig->vOriginalBufferSizeX, vGlobalITIPCDig->vOriginalBufferSizeY, 
				vThreshold, vGlobalITIPCDig->vDesensitizeImage, false, TempBottom, TempBottom + TempHeight, TempLeft, TempLeft + TempWidth);
		}

		if (vPerformGlassAlgorithm == 1)
		{
			ClipToCurveFit(vGlobalITIPCDig->vPQLIScratchImage->pix, vGlobalITIPCDig->vOriginalBufferSizeX, vGlobalITIPCDig->vOriginalBufferSizeY);
		}

		if (vPerformGlassAlgorithm == 2)
		if ((vGlobalITIPCDig->vPQLIScratchImage2) && (vGlobalITIPCDig->vPQLIScratchImage2->pix))
		{
			CalculateLineDerivative(vGlobalITIPCDig->vPQLIScratchImage->pix, vGlobalITIPCDig->vPQLIScratchImage2->pix,
				vGlobalITIPCDig->vOriginalBufferSizeX, vGlobalITIPCDig->vOriginalBufferSizeY, vGlobalCurrentProduct->vSetupHistoryAverageDensity);
		}

		if (vDoContaminant22Algorithm)
		{
			BYTE TempThreshold = vThreshold;
			int TempResult22 = InspxContaminant22InspectionB(vGlobalITIPCDig->vPQLIScratchImage->pix, vGlobalITIPCDig->vOriginalBufferSizeX, vGlobalITIPCDig->vOriginalBufferSizeY, TempThreshold, 0);
		}

		if (vSubtractAverage)
		if (vGlobalITIPCDig->vProductAverageImage)
			vGlobalITIPCDig->SubtractProductAverageImage(
			vGlobalITIPCDig->vPQLIScratchImage->pix,
			vGlobalITIPCDig->vProductAverageImage);

		//add the product's structure image to this one if enabled
		if ((vAddStructure) && (vSubtractAverage))
		{
			if (vGlobalITIPCDig->vProductStructureImage)
				vGlobalITIPCDig->AddProductStructureImage(vGlobalITIPCDig->vPQLIScratchImage->pix);
		}
		else
		if ((vAddStructure) && (!vSubtractAverage))
		if (vGlobalITIPCDig->vProductContaminant2StructureImage)
				vGlobalITIPCDig->AddProductContaminant2StructureImage(vGlobalITIPCDig->vPQLIScratchImage->pix);

		if (vEnhanceMethod)
			vGlobalITIPCDig->PerformEnhanceOnImage(vGlobalITIPCDig->vPQLIScratchImage, vGlobalITIPCDig->vPQLIScratchImage2, vEnhanceMethod);

		//if (vDoImageFactors)
		//	vGlobalITIPCDig->CombineImages( vGlobalITIPCDig->vPQLIScratchImage->pix, vEnhancedImageFactor, vGlobalITIPCDig->vLatestImage->pix, vOriginalImageFactor);

		if (vErodeCountAtEnd)  //is variable to hold erode times count
		for (BYTE TempLoop = 0; TempLoop < vErodeCountAtEnd; TempLoop++)
			ErodeImage(vGlobalITIPCDig->vPQLIScratchImage, vGlobalITIPCDig->vPQLIScratchImage2);

		if (vDilationCountAtEnd)
		if ((vGlobalITIPCDig->vPQLIScratchImage2) && (vGlobalITIPCDig->vPQLIScratchImage2->pix))
		for (BYTE TempLoop = 0; TempLoop < vDilationCountAtEnd; TempLoop++)
			DilateImage(vGlobalITIPCDig->vPQLIScratchImage, vGlobalITIPCDig->vPQLIScratchImage2);

		//Add Product's Histogram image to this one if enabled
		if ((vAddHistogram) && (vGlobalITIPCDig->vProductHistogramImage))
			vGlobalITIPCDig->AddProductHistogramImage(vGlobalITIPCDig->vPQLIScratchImage->pix);

		//if (vGlobalLearnState)
		//if (vGlobalConfigurationData->vAutoImproveEnabled)
		//if (vAddStructure)
		//{
		//	if (vROIPixelBottom < vGlobalCurrentProduct->vReferenceTop / 2)//if bottom contaminant
		//	{
		//		if (TempThreshold > (vGlobalLearnState * vGlobalConfigurationData->vAutoImproveReduceThresholdFactor))
		//			TempThreshold = (BYTE)(TempThreshold - (vGlobalLearnState * vGlobalConfigurationData->vAutoImproveReduceThresholdFactor)); 
		//	}
		//	else//if top contaminant
		//	{
		//		if (TempThreshold > (vGlobalLearnState * vGlobalConfigurationData->vAutoImproveReduceThresholdFactor * 3))
		//			TempThreshold = (BYTE)(TempThreshold - (vGlobalLearnState * vGlobalConfigurationData->vAutoImproveReduceThresholdFactor * 3)); 
		//	}
		//}

		if (!vDoContaminant22Algorithm)// dont threshold if c22
		{
			ThresholdImage(vGlobalITIPCDig->vPQLIScratchImage, TempThreshold);
		}

		//check for contaminants
		Image *TempSubImage = im_rroi(vGlobalITIPCDig->vPQLIScratchImage, TempBottom, TempLeft, TempHeight, TempWidth);
		if (TempSubImage)
		{
			if (vSize > 1)
				vGlobalITIPCDig->vBlobParameters->min_area = vSize;
			else
				vGlobalITIPCDig->vBlobParameters->min_area = -1;
			vGlobalITIPCDig->vBlobParameters->blob_color = 0x00;  //fewer xrays
			vGlobalITIPCDig->vBlobParameters->min_height = -1;
			vGlobalITIPCDig->vBlobParameters->max_area = -1;

			if ((vInspectionType == cDripInspection) || (vInspectionType == cDripBySpoutInspection))
			{
				if (vMaximumSize) //for Drip Inspection for IV Bag
					vGlobalITIPCDig->vBlobParameters->max_area = vMaximumSize;
				if (vMinimumWidth) //for Drip By Spout Inspection for IV Bag
					vGlobalITIPCDig->vBlobParameters->min_height = vMinimumWidth;
			}

			if (vInspectionType == cBadHeartInspection)
				vGlobalITIPCDig->vBlobParameters->calc_results = AREA_BBOX + AREA_BBOX_CENT_PERIM;
			else
				vGlobalITIPCDig->vBlobParameters->calc_results = AREA_BBOX;

			Erflag TempResult = mvt_blob_find(TempSubImage, vGlobalITIPCDig->vBlobParameters, vGlobalITIPCDig->vBlobResults, MVT_CF_DEFAULT);
			// area of blob calculation

			if ((WORD)vGlobalITIPCDig->vBlobResults->num_blobs_found)
			{
				double TempArea = 0;
				double TempLargestBlob = 0;
				double TempLargestPerimeter = 0;
				if ((WORD)vGlobalITIPCDig->vBlobResults->num_blobs_found)
				{
					for (WORD TempLoop = 0; TempLoop < (WORD)vGlobalITIPCDig->vBlobResults->num_blobs_found; TempLoop++)
					{
						DWORD TempBlobArea = vGlobalITIPCDig->vBlobResults->blobs[TempLoop]->area;

						if ((vInspectionType == cDripInspection) && (vMaximumHeightAboveBottom))
						{
							//if the height of the centroid is higher than the setting above the bottom, then ignore this blob
							if (vGlobalITIPCDig->vBlobResults->blobs[TempLoop]->xcent + vGlobalITIPCDig->vLastBottomReferenceAdjust > 
								vGlobalITIPCDig->vLastBottom + vMaximumHeightAboveBottom) //zero out this blobs area so it does not count if it is above the limit
							{
								TempBlobArea = 0;  //ignore this blob
								vGlobalITIPCDig->vBlobResults->blobs[TempLoop]->area = 0; //set the area of this blob to zero to clear it as it is too high above the bottom of the container
							}
						}

						TempArea = TempArea + TempBlobArea;
					}
				}

				if (TempArea)
					TempEjectIt = 1;
			}

			im_delete(TempSubImage);
			TempSubImage = NULL;
		}
		else
		if (vGlobalRunningData->vLogFile.vLogSerialData)
			vGlobalRunningData->vLogFile.WriteToLogFile(vGlobalRunningData->vFrameCountString + " Error, MVT Could not create Image for Blobs",cDebugMessage);
	}
	return TempEjectIt;
}
/*
void CInspection::DilateImage(Image *TempInputImage)
{
	if ((TempInputImage) && (TempInputImage->pix))
	{
//#ifdef UseHalconLibraries
//		if (vGlobalUseHalconLibrary)
//		{//using HALCON
//			HByteImage TempHalconImageIn = MakeHalconImage(TempInputImage);
//			
//			HByteImage TempResultImage = TempHalconImageIn.GrayOpeningRect(3, 3);
//			CopyFromHalconImage(TempResultImage, TempInputImage);
//		}
//		else
//#endif
		{ //MVTools
			Image *TempOutputImage;
			TempOutputImage = im_create(IM_BYTE, TempInputImage->dx, TempInputImage->dy);

			if ((TempOutputImage) && (TempOutputImage->pix))
			{
				Erflag TempResultError = mvt_dilate(TempInputImage, TempOutputImage);
				if (TempResultError != IM_OK)
					ReportErrorMessage("Error, MVT Dilate Failed", cEMailInspx,32000);
				else
					CopyMemory(TempInputImage->pix, TempOutputImage->pix, TempInputImage->dx * TempInputImage->dy);
			}
			if (TempOutputImage)
				im_delete(TempOutputImage);
		}
	}
}

void CInspection::DilateImage(Image *TempInputImage, Image *TempScratchImage)
{
	if ((TempInputImage) && (TempInputImage->pix))
	if ((TempScratchImage) && (TempScratchImage->pix))
	{
		//ZeroMemory(TempScratchImage->pix, TempInputImage->dx * TempInputImage->dy);
		Erflag TempResultError = mvt_dilate(TempInputImage, TempScratchImage);
		if (TempResultError != IM_OK)
			ReportErrorMessage("Error, MVT Dilate Failed", cEMailInspx,32000);
		else
			CopyMemory(TempInputImage->pix, TempScratchImage->pix, TempInputImage->dx * TempInputImage->dy);
	}
}
*/
bool CInspection::ThereAreAnyBlobsAtValue(BYTE TempThreshold, Image *TempImage, WORD TempBottom, WORD TempLeft, WORD TempHeight, WORD TempWidth)
{
	bool TempFoundBlobs = false;
#ifdef UseIFCDLLs
	Erflag TempErrorFlag;
	MVT_BLOB_PARAMS *TempBlobParameters = mvt_blob_create_params(&TempErrorFlag);
	MVT_BLOB_RESULTS *TempBlobResults = mvt_blob_create_results(20, true, &TempErrorFlag);
	if ((TempBlobParameters) && (TempBlobResults))
	{
		TempBlobParameters->min_area = vSize;
		TempBlobParameters->connected = CONNECT8;
		TempBlobParameters->blob_color = 0x00;  //fewer xrays
		TempBlobParameters->calc_results = AREA_BBOX;

		if ((vInspectionType == cDripInspection) || (vInspectionType == cDripBySpoutInspection))
		{
			if (vMaximumSize) //for Drip Inspection
				TempBlobParameters->max_area = vMaximumSize;
			if (vMinimumWidth) //for Drip By Spout Inspection for IV Bag
				vGlobalITIPCDig->vBlobParameters->min_height = vMinimumWidth;
		}

		if (vInspectionType == cBadHeartInspection)
			vGlobalITIPCDig->vBlobParameters->calc_results = AREA_BBOX + AREA_BBOX_CENT_PERIM;
		else
			vGlobalITIPCDig->vBlobParameters->calc_results = AREA_BBOX;

		Image *TempDuplicateImage = im_duplicate(TempImage);
		Image *TempSubImage = NULL;
		if (TempDuplicateImage)
		{
			TempSubImage = im_rroi(TempDuplicateImage, TempBottom, TempLeft, TempHeight, TempWidth - 1);
			if (TempSubImage)
			{
				ThresholdImage(TempSubImage, TempThreshold);

				Erflag TempResult = mvt_blob_find(TempSubImage, TempBlobParameters, TempBlobResults, MVT_CF_DEFAULT);
				if (TempResult != IM_OK)
					ReportErrorMessage("MVT_Blob Error 5",cEMailInspx,32000);

				im_delete(TempSubImage);
			}
			im_delete(TempDuplicateImage);
		}
		else
		if (vGlobalRunningData->vLogFile.vLogSerialData)
			vGlobalRunningData->vLogFile.WriteToLogFile(" Error, MVT Could not create Image for Blobs",cDebugMessage);

		if (TempBlobResults->num_blobs_found)
		{
				double TempArea = 0;
				double TempLargestBlob = 0;
				double TempLargestPerimeter = 0;
				if ((WORD)TempBlobResults->num_blobs_found)
				{
					for (WORD TempLoop = 0; TempLoop < (WORD)TempBlobResults->num_blobs_found; TempLoop++)
					{
						DWORD TempBlobArea = TempBlobResults->blobs[TempLoop]->area;

						if ((vInspectionType == cDripInspection) && (vMaximumHeightAboveBottom))
						{
							//if the height of the centroid is higher than the setting above the bottom, then ignore this blob
							if (TempBlobResults->blobs[TempLoop]->xcent + vGlobalITIPCDig->vLastBottomReferenceAdjust > vGlobalITIPCDig->vLastBottom + vMaximumHeightAboveBottom) //zero out this blobs area so it does not count if it is above the limit
							{
								TempBlobArea = 0;  //ignore this blob
								TempBlobResults->blobs[TempLoop]->area = 0; //set the area of this blob to zero to clear it as it is too high above the bottom of the container
							}
						}

						TempArea = TempArea + TempBlobArea;
					}
				}
				if (TempArea)
					TempFoundBlobs = true;
		}
		else
			TempFoundBlobs = false;

		mvt_blob_delete_params(TempBlobParameters);
		mvt_blob_delete_results(TempBlobResults);
	}
#endif
	return TempFoundBlobs;
}

void CInspection::LookForDarkSpotsOnLine(BYTE *TempImage, Image *TempScratchImage, WORD TempSizeX, WORD TempSizeY, WORD TempLeft, WORD TempRight, WORD TempBottom, WORD TempTop, BYTE TempThreshold)
{ // Wire algorithm
	if ((TempImage) && (TempScratchImage))
	{
		if (TempTop >= TempSizeX)
			TempTop = TempSizeX - 1;
		if (TempRight >= TempSizeY)
			TempRight = TempSizeY - 1;
		DWORD TempSize = TempSizeX * TempSizeY;

		Image *TempImageToRotate = NULL;
		if (ThereIsEnoughMemory(TempSize + sizeof(Image), "Temporary Image for Find Bones"))
			TempImageToRotate = im_create(IM_BYTE, TempSizeX, TempSizeY);

		if ((TempImageToRotate) && (TempImageToRotate->pix))
		if (TempScratchImage)
		{ // two matrixes
			//scan side to side
			CopyMemory(TempImageToRotate->pix, TempImage, TempSize);
			//LookForDarkSpotsOnLineInOneDirection(TempScratchImage->pix, TempScratchImage->pix, TempSizeX, TempSizeY, TempLeft, TempRight, TempBottom, TempTop);
#ifdef UseIFCDLLs
			mvt_rotate90(TempImageToRotate, -1);
			//top to bottom looking
			LookForDarkSpotsOnLineInOneDirection(TempImageToRotate->pix, TempScratchImage->pix, TempSizeY, TempSizeX, TempBottom, TempTop, TempLeft, TempRight, TempThreshold);

			RotateImage90Degrees(TempImageToRotate, 1);
			//test only blank this direction FillMemory(TempImageToRotate->pix, TempSize, 255);
#endif
			//side to side looking
			LookForDarkSpotsOnLineInOneDirection(TempImage, TempScratchImage->pix, TempSizeX, TempSizeY, TempLeft, TempRight, TempBottom, TempTop, TempThreshold);
			//test only blank this direction FillMemory(TempImage, TempSize, 255);

			//take smaller value of two images for each pixel
			BYTE *TempSource = TempImage;
			BYTE *TempSource2 = TempImageToRotate->pix;
			for (DWORD TempLoop = 0; TempLoop < TempSize; TempLoop++)
			{
				if (*TempSource > *TempSource2)  //take the smallest value of the two
					*TempSource = *TempSource2;

				TempSource++;
				TempSource2++;
			}
		}

		if (TempImageToRotate)
			im_delete(TempImageToRotate);
	}
}

//wire inspection
//new algorithm just looking for dark spots but faint
void CInspection::LookForDarkSpotsOnLineInOneDirection(BYTE *TempImage, BYTE *TempScratchImage, WORD TempSizeX, WORD TempSizeY, WORD TempLeft, WORD TempRight, WORD TempBottom, WORD TempTop, BYTE TempThreshold)
{
	if (vEnable)
	{
		BYTE TempMinimumDipForAContaminant = 255 - TempThreshold;
		if (TempMinimumDipForAContaminant == 0)
			TempMinimumDipForAContaminant = 1;
		DWORD TempSize = TempSizeX * TempSizeY;

		if (TempTop >= TempSizeX)
		{
			//ReportErrorMessage("CalculateLineDerivative-Error-ROI Wrong 8. Top: " + dtoa(TempTop, 0), cEMailInspx, 32000);
			TempTop = TempSizeX - 1;
		}
		if (TempBottom >= TempTop)
		{
			ReportErrorMessage("CalculateLineDerivative-Error-ROI Wrong 9. Bottom: " + dtoa(TempBottom, 0), cEMailInspx, 32000);
			TempBottom = 2;
		}
		if (TempRight + vPixelLookAhead + 2 > TempSizeY - 1)
		{
			//ReportErrorMessage("CalculateLineDerivative-Error-ROI Wrong 10. Right: " + dtoa(TempRight, 0), cEMailInspx, 32000);
			TempRight = TempSizeY - vPixelLookAhead - 2 - 1;
		}
		if (TempLeft >= TempRight)
		{
			ReportErrorMessage("CalculateLineDerivative-Error-ROI Wrong 11. Left: " + dtoa(TempLeft, 0), cEMailInspx, 32000);
			TempLeft = vPixelLookAhead + 2;
		}
		if (TempLeft < vPixelLookAhead + 2)
		{
			//ReportErrorMessage("CalculateLineDerivative-Error-ROI Wrong 12. Left: " + dtoa(TempLeft, 0), cEMailInspx, 32000);
			TempLeft = vPixelLookAhead + 2;
		}
		if (TempLeft == 0)
			TempLeft = 1;

		WORD TempHeight = TempTop - TempBottom;
		if (TempHeight + TempBottom > TempSizeX)
		{
			ReportErrorMessage("CalculateLineDerivative-Error-ROI Wrong 13. Height: " + dtoa(TempHeight, 0), cEMailInspx, 32000);
			TempHeight = TempSizeX - TempBottom;
		}

		WORD TempROIWidth = TempRight - TempLeft;
		if (TempROIWidth + TempLeft + vPixelLookAhead + 2 > TempSizeY)
		{
			//ReportErrorMessage("CalculateLineDerivative-Error-ROI Wrong. Width: " + dtoa(TempROIWidth, 0), cEMailInspx, 32000);
			TempROIWidth = TempSizeY - TempLeft - vPixelLookAhead - 2;
		}

		WORD TempROIHalfWidth = TempROIWidth / 2;
		WORD TempCenter = TempLeft + TempROIHalfWidth;

		if (TempImage)
		if (TempScratchImage)
		if ((TempROIWidth < 20) || (TempROIWidth < (vPixelLookAhead * 2) + 4))
		{
			ReportErrorMessage("Wire Inspection too few pixels, make ROI larger", cEMailMaintenance, 0);
		}
		else
		{
			//copy original image to scratch image
			CopyMemory(TempScratchImage, TempImage, TempSize);

			//clear original image to white to use as the destination image for the results
			FillMemory(TempImage, TempSize, 255);

			BYTE *TempCurrentPixel = NULL;
			BYTE *TempNextPixel = NULL;
			BYTE *TempNextPixelAfterThat = NULL;
			BYTE *TempResultPixel = NULL;
			BYTE *TempMaximumScratchPointer = TempScratchImage + TempSize;
			BYTE *TempMaximumResultPointer = TempImage + TempSize;

			//start at the bottom ROI line and study every line up to the top ROI line
			for (WORD TempLineLoop = 0; TempLineLoop < TempHeight; TempLineLoop++)
			{
				TempCurrentPixel = TempScratchImage + TempLineLoop + TempBottom + (TempLeft * TempSizeX); //start at left ROI pixel on this line
				TempResultPixel = TempImage + TempLineLoop + TempBottom + ((TempLeft + 2) * TempSizeX); //result at left ROI pixel on this line
				BYTE *TempPreviousPixel = TempCurrentPixel - TempSizeX; //for seeing if near background
				if (TempPreviousPixel < TempScratchImage)
					TempPreviousPixel = TempCurrentPixel;
				BYTE *Temp2PreviousPixel = TempPreviousPixel - TempSizeX; //for seeing if near background
				if (Temp2PreviousPixel < TempScratchImage)
					TempPreviousPixel = TempPreviousPixel;

				//scanning from Left to Right ROI Line, indicate where image gets darker than is over 2 pixels, then gets stronger over 2 pixels, output image is lower of them painted over span between them
				for (int TempPixelLoop = TempLeft; TempPixelLoop < TempRight; TempPixelLoop++)
				{
					TempNextPixel = TempCurrentPixel + TempSizeX;
					if ((TempCurrentPixel >= TempScratchImage) && (TempCurrentPixel < TempMaximumScratchPointer) && (TempResultPixel  < TempMaximumResultPointer) && (TempResultPixel >= TempImage)) //check pointers are in images
					if (*TempCurrentPixel < 255) //skip white pixels
					if (*TempCurrentPixel > TempMinimumDipForAContaminant) //skip pixels cant drop the threshold amount after
					if (*TempNextPixel < *TempCurrentPixel) //skip pixels if the next pixel is brighter
					{
						BYTE *TempPixelToCheck = TempNextPixel;
						TempNextPixel = TempNextPixel + TempSizeX;
						BYTE *TempPixelAfterNext = TempNextPixel + TempSizeX;
						BYTE TempAmountGotBrighterIn2Pixels = 0;
						BYTE TempLookAheadLoopPositionFoundRiseAt = 0;
						BYTE TempAmountGotDarkerIn2Pixels = 0;
						BYTE TempDarkestPixel = 255;
						BYTE TempBrightestAfterDarkestPixel = 0;

						bool TempWhiteNearBy = ((*TempCurrentPixel > 240) || (*TempPreviousPixel > 240) || (*Temp2PreviousPixel > 240)); //flag if should desensitize the result because you are near a white background

						for (BYTE TempLookAheadLoop = 0; TempLookAheadLoop < vPixelLookAhead; TempLookAheadLoop++)  //for the current pixel, scan ahead the lookahead amount and see if there is a dip and return (2 pixel dip, 2 pixel return)
						if (TempPixelAfterNext < TempMaximumScratchPointer) //check pointer is in image
						if (TempPixelLoop + TempLookAheadLoop < TempRight) //don't look past the Right ROI Line
						{
							if (*TempPixelToCheck > 240) //flag if should desensitize the result because you are near a white background
								TempWhiteNearBy = true;
							if (*TempNextPixel > 240) //flag if should desensitize the result because you are near a white background
								TempWhiteNearBy = true;
							
							//if this pixel is a bigger drop from the current pixel than we have seen so far (only looking a 2 pixel drops)
							if ((TempLookAheadLoop < 2) && (*TempPixelToCheck < *TempCurrentPixel) && (*TempCurrentPixel - *TempPixelToCheck > TempAmountGotDarkerIn2Pixels))
									TempAmountGotDarkerIn2Pixels = *TempCurrentPixel - *TempPixelToCheck; //save how much it got darker
							else
							if (TempAmountGotDarkerIn2Pixels > TempMinimumDipForAContaminant) //if saw a drop greater than the Threshold, then look for a rise
							{ //pixel looking at is darker than current pixel
								if (*TempPixelToCheck < *TempCurrentPixel)  //only look at rise if pixel starting at is less than the current pixel (so can go through whole lookahead to see if near white edge)
								if (*TempNextPixel < *TempPixelAfterNext) //if next pixel is brighter
								{
									if (*TempPixelToCheck < *TempPixelAfterNext) //if 2 pixels farther is brighter
									{
										if (*TempPixelAfterNext - *TempPixelToCheck > TempAmountGotBrighterIn2Pixels) //if next pixel is brighter than seen so far, save it
										{
											TempAmountGotBrighterIn2Pixels = *TempPixelAfterNext - *TempPixelToCheck;
											TempLookAheadLoopPositionFoundRiseAt = TempLookAheadLoop;
										}
									}
								}
								else
								{
									if (*TempPixelToCheck < *TempNextPixel) //if next pixel is brighter
									{
										if (*TempNextPixel - *TempPixelToCheck > TempAmountGotBrighterIn2Pixels) //if next pixel is brighter than seen so far, save it
										{
											TempAmountGotBrighterIn2Pixels = *TempNextPixel - *TempPixelToCheck;
											TempLookAheadLoopPositionFoundRiseAt = TempLookAheadLoop;
										}
									}
								}
							}

							Temp2PreviousPixel = TempPreviousPixel;
							TempPreviousPixel = TempPixelToCheck;
							TempPixelToCheck = TempPixelToCheck + TempSizeX;
							TempNextPixel = TempNextPixel + TempSizeX;
							TempPixelAfterNext = TempPixelAfterNext + TempSizeX;

							if ((TempLookAheadLoop == 1) && (TempAmountGotDarkerIn2Pixels < TempMinimumDipForAContaminant)) //if after two pixels, did not see a drop darker than the Threshold, then give up on the current pixel
								TempLookAheadLoop = vPixelLookAhead;
						}
						//if found a dip and rise in brightness both greater than the threshold then pick
						if (TempAmountGotDarkerIn2Pixels >= TempMinimumDipForAContaminant)
						if (TempAmountGotBrighterIn2Pixels >= TempMinimumDipForAContaminant)
						{//pick the smallest of the differences
							BYTE TempDesensitizeAmount = 0;
							if (TempWhiteNearBy)
								TempDesensitizeAmount = vDesensitizeEdgePixels; //if near white around, then lower the sensitivity if configured

							if (TempAmountGotDarkerIn2Pixels <= TempAmountGotBrighterIn2Pixels)
							{
								if (TempDesensitizeAmount < TempAmountGotDarkerIn2Pixels)
								if (*TempResultPixel > 255 - TempAmountGotDarkerIn2Pixels + TempDesensitizeAmount) //only save the result if darker than any previous result
									*TempResultPixel = 255 - TempAmountGotDarkerIn2Pixels + TempDesensitizeAmount;
							}
							else
							//if (TempAmountGotDarkerIn2Pixels > TempAmountGotBrighterIn2Pixels)
							{
								if (TempDesensitizeAmount < TempAmountGotBrighterIn2Pixels)
								if (*TempResultPixel > 255 - TempAmountGotBrighterIn2Pixels + TempDesensitizeAmount) //only save the result if darker than any previous result
									*TempResultPixel = 255 - TempAmountGotBrighterIn2Pixels + TempDesensitizeAmount;
							}

							//now paint in the pixels between the dip and the rise with this value
							//if (TempLookAheadLoopPositionFoundRiseAt > 5)
							//	TempLookAheadLoopPositionFoundRiseAt = 5;

							BYTE *TempNextResultPixel = TempResultPixel + TempSizeX;
							for (BYTE TempPaintLoop = 1; TempPaintLoop < TempLookAheadLoopPositionFoundRiseAt + 1; TempPaintLoop++)
							if (TempNextResultPixel < TempMaximumResultPointer)
							{
								if (*TempNextResultPixel > *TempResultPixel)
								{
									*TempNextResultPixel = *TempResultPixel;
								}
								TempNextResultPixel++;
							}
						}
					}
					TempCurrentPixel = TempCurrentPixel + TempSizeX;
					TempResultPixel = TempResultPixel + TempSizeX;
				}
			}
		}
	}
}


void CInspection::DilateImage(Image *TempImageIn, Image *TempScratchImage)
{
	if (TempImageIn)
	if (TempScratchImage)
	//if (vGlobalUseHalconLibrary)
	//{
	//	try
	//	{
	//		HalconCpp::HRegion TempHalconRegionIn = MakeHalconRegion(TempImageIn);
	//		//HalconCpp::Rectangle TempRectangle = GenRectangle1(0,0,5,5);
	//		HalconCpp::HRegion TempStructureElement;
	//		//TempStructureElement.GenRectangle1(0, 0, 1, 1);
	//		TempStructureElement.GenCircle(5, 5, 2);
	//		HalconCpp::HRegion TempHalconRegionOut = TempHalconRegionIn.Dilation1(TempStructureElement, 1);

	//		//CopyFromHalconRegion((HalconCpp::HObject &)TempHalconRegionOut, TempImageIn);
	//		HalconCpp::HImage TempHalconImage = TempHalconRegionOut.RegionToBin(255, 0, TempImageIn->dx, TempImageIn->dy);
	//		TempHalconImage = TempHalconImage.InvertImage();
	//		CopyFromHalconImage((HalconCpp::HObject &)TempHalconImage, TempImageIn);


	//		//TempHalconImageIn.WriteImage("bmp", 0, "c:\\ScanTracImages\\Honest Tea Halcon Median 5 DFM.BMP");
	//	}
	//	catch(HalconCpp::HException &TempException)
	//	{
	//		Hlong TempErrorNumber = TempException.ErrorNumber();
	//		if ((TempErrorNumber < 2003) && (TempErrorNumber > 2099)) //reported no license error when booted if that is the problem
	//			ReportErrorMessage("InvertImageA-Halcon Error: " + dtoa(TempErrorNumber, 0), cEMailInspx, 0);
	//	}
	//}
	//else
	{//MVTools way
		//Erflag TempResultError = mvt_dilate(TempImageIn, TempScratchImage);

		//if (TempResultError != IM_OK)
		//	ReportErrorMessage("Error, MVT Dilate Failed", cEMailInspx,32000);
		//CopyMemory(TempImageIn->pix, TempScratchImage->pix, TempImageIn->dx * TempImageIn->dy);
		Erflag TempResultError = mvt_dilate_rect(TempImageIn, 2, 2, EDGE_IGNORE);

		if (TempResultError != IM_OK)
			ReportErrorMessage("Error, MVT Dilate Failed", cEMailInspx,32000);
	}
}

void CInspection::ErodeImage(Image *TempImageIn, Image *TempScratchImage)
{
	if (TempImageIn)
	if (TempScratchImage)
	//if (vGlobalUseHalconLibrary)
	//{
	//	try
	//	{
	//		HalconCpp::HRegion TempHalconRegionIn = MakeHalconRegion(TempImageIn);
	//		//HalconCpp::Rectangle TempRectangle = GenRectangle1(0,0,5,5);
	//		HalconCpp::HRegion TempStructureElement;
	//		//TempStructureElement.GenRectangle1(0, 0, 1, 1);
	//		TempStructureElement.GenCircle(5, 5, 2);
	//		HalconCpp::HRegion TempHalconRegionOut = TempHalconRegionIn.Erosion1(TempStructureElement, 1);

	//		//CopyFromHalconRegion((HalconCpp::HObject &)TempHalconRegionOut, TempImageIn);
	//		HalconCpp::HImage TempHalconImage = TempHalconRegionOut.RegionToBin(255, 0, TempImageIn->dx, TempImageIn->dy);
	//		TempHalconImage = TempHalconImage.InvertImage();
	//		CopyFromHalconImage((HalconCpp::HObject &)TempHalconImage, TempImageIn);
	//		//TempHalconImageIn.WriteImage("bmp", 0, "c:\\ScanTracImages\\Honest Tea Halcon Median 5 DFM.BMP");
	//	}
	//	catch(HalconCpp::HException &TempException)
	//	{
	//		Hlong TempErrorNumber = TempException.ErrorNumber();
	//		if ((TempErrorNumber < 2003) && (TempErrorNumber > 2099)) //reported no license error when booted if that is the problem
	//			ReportErrorMessage("InvertImageA-Halcon Error: " + dtoa(TempErrorNumber, 0), cEMailInspx, 0);
	//	}
	//}
	//else
	{//MVTools way
		//Erflag TempResultError = mvt_erode(TempImageIn, TempScratchImage);

		//if (TempResultError != IM_OK)
		//	ReportErrorMessage("Error, MVT Dilate Failed", cEMailInspx,32000);
		//CopyMemory(TempImageIn->pix, TempScratchImage->pix, TempImageIn->dx * TempImageIn->dy);
		Erflag TempResultError = mvt_erode_rect(TempImageIn, 2, 2, EDGE_IGNORE);

		if (TempResultError != IM_OK)
			ReportErrorMessage("Error, MVT erode_rect Failed", cEMailInspx,32000);
	}
}


bool CInspection::IsIVBagFlooded(Image *TempImage, WORD *TempDelta, WORD *TempBottom, WORD TempSizeX, WORD TempSizeY)
{
	//Threshold the image at the Threshold to get rid of the faint water drops.
	//then find the height difference between the lowest point and the highest point on the bottom

	*TempDelta = 0;

	//int TempError = mvt_threshold(TempImage, vThreshold, 0, 255);  //this makes the product black and background white
	/*
	//must blank off area outside ROI
	//blank left edge
	if (vROIPixelLeft)
	{
		DWORD TempCount = vROIPixelLeft * TempSizeX;
		FillMemory(TempImage, TempCount, 255);
	}

	//blank Right edge
	if (vROIPixelRight < TempSizeY - 1)
	{
		DWORD TempStart = vROIPixelRight * TempSizeX;
		DWORD TempCount = (TempSizeX * TempSizeY) - TempStart - 2;
		FillMemory(TempImage + TempStart, TempCount, 255);
	}

	//blank Bottom edge
	if (vROIPixelBottom)
	{
		DWORD TempStart = vROIPixelLeft * TempSizeX;
		DWORD TempCount = vROIPixelBottom;
		for (WORD TempLoop = 0; TempLoop < vROIPixelRight - vROIPixelLeft; TempLoop++)
			FillMemory(TempImage + TempStart + (TempLoop * TempSizeX), TempCount, 255);
	}

	//blank Top edge
	if (vROIPixelTop < TempSizeX - 1)
	{
		DWORD TempStart = (vROIPixelLeft * TempSizeX) + vROIPixelTop;
		DWORD TempCount = TempSizeX - vROIPixelTop;
		for (WORD TempLoop = 0; TempLoop < vROIPixelRight - vROIPixelLeft; TempLoop++)
			FillMemory(TempImage + TempStart + (TempLoop * TempSizeX), TempCount, 255);
	}
	*/
	WORD TempIVBagMinimum = TempSizeX;
	WORD TempIVBagMaximum = 0;

	WORD TempQuarterWidth = (vROIPixelRight - vROIPixelLeft) / 4;
	WORD TempThreeQuarterWidth = TempQuarterWidth * 3;

	for (WORD TempLoopY = 0; TempLoopY < vROIPixelRight - vROIPixelLeft; TempLoopY++) //for each vertical line in the ROI
	{
		BYTE *TempPointer = TempImage->pix + vROIPixelBottom + ((vROIPixelLeft + TempLoopY) * TempSizeX);
		for (WORD TempLoopX = 0; TempLoopX < vROIPixelTop - vROIPixelBottom; TempLoopX++) //for each vertical line in the ROI
		{
			if (*TempPointer < vThreshold) //found bag, so this is height of this line
			{
				if ((TempLoopY > TempQuarterWidth) && (TempLoopY < TempThreeQuarterWidth)) //only check for spout minimum in center half of image
				if (TempIVBagMinimum > TempLoopX)
					TempIVBagMinimum = TempLoopX;

				if (TempIVBagMaximum < TempLoopX)
					TempIVBagMaximum = TempLoopX;

				TempLoopX = TempSizeX; //go to next line since found minimum of this line
			}
			TempPointer++;
		}
	}
	if (TempIVBagMaximum > TempIVBagMinimum)
	{
		*TempDelta = TempIVBagMaximum - TempIVBagMinimum;
		*TempBottom = TempIVBagMinimum;
	}
	if ((TempIVBagMaximum) && (*TempDelta < vSize))  //if saw the bag, and if not enough difference, then did not see the spout, so IV bag not flooded 
		return true;
	else
		return false;
}
void CInspection::WhiteOutAnythingTouchingTheTop(BYTE *TempImage, WORD TempSizeX, WORD TempSizeY)
{
	BYTE *TempPointer = TempImage + TempSizeX - 1;  //point at first pixel on top
	for (WORD TempLoopY = 0; TempLoopY < TempSizeY; TempLoopY++)
	{
		if (*TempPointer < 255)
			WhiteOutThisPixelAndOnesTouchingBelow(TempImage, TempSizeX - 1, TempLoopY, TempSizeX, TempSizeY);

		TempPointer = TempPointer + TempSizeX;
	}
}

void CInspection::WhiteOutThisPixelAndOnesTouchingBelow(BYTE *TempImage, WORD TempPixelX, WORD TempPixelY, WORD TempSizeX, WORD TempSizeY)
{
	BYTE *TempPointer = TempImage + TempPixelX + (TempPixelY * TempSizeX);  //point at first pixel on top
	if (*TempPointer < 255) //if this pixel is not white, delete anything touching below, and to the sides
	{
		if (TempPixelX > 1)
			WhiteOutThisPixelAndOnesTouchingBelow(TempImage, TempPixelX - 1, TempPixelY, TempSizeX, TempSizeY);

		if (TempPixelY > TempSizeY / 2)  //if in the right half of the image, check the right pixel
		if (TempPixelY < TempSizeY - 2)
			WhiteOutThisPixelAndOnesTouchingBelow(TempImage, TempPixelX, TempPixelY + 1, TempSizeX, TempSizeY);

		if (TempPixelY < TempSizeY / 2)  //if in the left half of the image, check the left pixel
		if (TempPixelY > 2)
			WhiteOutThisPixelAndOnesTouchingBelow(TempImage, TempPixelX, TempPixelY - 1, TempSizeX, TempSizeY);

		*TempPointer = 255; //white out this pixel

		if (TempPixelX < TempSizeX - 2) //check pixel above in case came from the side and bag is higher
			WhiteOutThisPixelAndOnesTouchingBelow(TempImage, TempPixelX + 1, TempPixelY, TempSizeX, TempSizeY);
	}
}

