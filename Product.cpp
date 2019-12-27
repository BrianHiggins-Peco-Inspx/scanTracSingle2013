//ScanTrac Side View Source File
// Product.cpp: implementation of the CProduct class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScanTrac.h"
#include "Product.h"
#include "SelectMultipleItemDialog.h"
#include "Inspection.h"
#include "InspectionBottomContaminant.h"
//#include "InspectionGlassContaminant.h"
#include "InspectionContaminant.h"
#include "InspectionVoid.h"
#include "InspectionDensity.h"
#include "InspectionKernel.h"
#include "InspectionSize.h"
#include "InspectionStandardDeviation.h"
#include "InspectionCheckWeigh.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//IMPLEMENT_DYNCREATE(CProduct, CObject)

extern BYTE vGlobalEndOfLineTimeOutMargin;
extern BYTE vGlobalInAutoSize;
extern BYTE vGlobalAllowVoidInspection;
extern BYTE vGlobalScanTracType;
//extern BYTE vGlobalUseMetric;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProduct::CProduct()
{
	vLowerRimHeightInPixels = 0;
	vNumberOfInspectionsRequiredToEject = 1;
	vProductCodeSelectionCString = "";  // was   vSpareCString1 = "";		
	vSpareCString1 = "";
	vSpareCString2 = "";
	vSpareCString3 = "";
	vSpareCString4 = "";
	vSpareCString5 = "";

	vSpareDoubleWasStructureAdjustSubtractiveFactor = 0;
	vSpareDoubleWasStructureAdjustFactor = 0;
	vHistogramThreshold = 250;
	vHistogramLowerThreshold = 0;
	vHistogramDilateTimes = 0;
	vTypesOfRejectsToView = 0xFFFF;
	vMaxConveyorSpeedAsEntered = 0;
	vNumberOfInspections = 0;
	vNumberOfAuxiliaryDetectors = 0;
	for (BYTE TempLoop = 0;TempLoop < cMaximumNumberOfInspections; TempLoop++)
		vInspection[TempLoop] = NULL;

	vDriftDensityInspection = NULL;
	vFillerMonitorInspection = NULL;

	for (BYTE TempLoop = 0;TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
		vSpareDACDetectorOffset[TempLoop] = 0;
	vSpareElectronicOffset = 0;

	vHeadOffsetFromIndexPulse[7] = 0;
	vSpareByteVSHAT1 = 0;
	vSpareFloatSHDATU = 0;
	vSpareFloatSHDATL = 0;

	vNotes = "None";
	SetOverScanFactor(0);
	vTooManyRejectsAlarmX[0] = 0;
	vTooManyRejectsAlarmY[0] = 0;
	vTooManyRejectsAlarmX[1] = 0;
	vTooManyRejectsAlarmY[1] = 0;
	vTooManyRejectsAlarmDisableEjectors[0] = false;
	vTooManyRejectsAlarmDisableEjectors[1] = false;
	vInspectionAlarmSettings[0] = 0;
	vInspectionAlarmSettings[1] = 0;
	vProductName = "Not Assigned";
	vHistoryTookAverageDate = "";
	vMaximumImageOverlap = 1;
	vReferenceTop = 0;
	vReferenceBottom = 0;
	vReferenceLeft = 0;
	vReferenceRight = 0;
	vReferenceWidth = 0;
	vXRayIntegrationTime = 750; //code for .3 msec integration time
	vXRayIntegrationTimeByte = 0xFF;  //indicates it is not used now, use the word value
	vSpareByteCanImageBottomOfContainer = false;
	vThresholdForContainerBounds = 240;
	vProductImageWidth = 0;
	vImageWidthPosition = 0;
	vProductLockOutWidth = 0;
	vLockoutPosition = 0;
	vProductBodyTriggerToImageBeltPositionOffset = 0;
	vBTToImagePosition = 0;
	vProductImageHeightTop = 6;
	vProductImageHeightBottom = 0;
	vEndOfLineTimeOut = 0;
	vEndOfLineBeltPosition = 0;
	vNeedToRelearn = 0;
	vReferenceROIsToEdges = true;
	vUsingXScanPoint4mmDetectors = 0;

	vHaveManuallySetRetriggerLockout = 0;
	vAddToLearnCount = 0;
	vMultiLaneNumberOfLanes = 0;
	vMultiLaneNumberOfLinesBeforeContainer = 4;
	vMultiLaneEjectAdjacentLanes = 0;
	for (BYTE TempLoop = 0; TempLoop < cMaximumNumberForMultiLane; TempLoop++)
	{
		vMultiLaneStartPixel[TempLoop] = 0;
		vMultiLaneWidthInPixels[TempLoop] = 0;
	}

	//vLeftJarWell = 0;
	//vRightJarWell = 0;
	vFillerTolerance = 0;
	vSpareDoubleCB = 0;
	vSpareDoubleCL = 0;
	vSpareDoubleCR = 0;
	vSpareByteDT = 0;
	vBulkProductMode = 0;
	vTempScreenShotDirectoryName = " ";

	vSetupHistoryAverageDensity = 0;

	vSpareByteSHMG = 255;
	vSpareByteSHMB = 0;
	vSpareFloatSHMBSD = 0;
	vSetupHistoryStructureCount = 0;
	vSetupHistoryHistogramThreshold = 0;
	vSetupHistoryAverageCount = 0;
	vSetupHistoryContainerTriggerStandardDeviation = 0;
	vSpareDWordVSMGC = 0;
	vSpareDWordVSMBC = 0;
	vSpareDWordVSHAC = 0;
	vSpareDWordVSHSC = 0;

	vSpareByteVSHMG = 0;
	vSpareFloatVSHMGSD = 0;
	vSpareFloatVSHMBA = 0;
	vSpareFloatVSHMBSD = 0;
	vSpareByteVSHHT = 0;

	vSpareStringHDN = "";
	vSpareStringLDN = "";
	vDriftROITop = 0;
	vDriftROIBottom = 0;
	vDriftROILeft = 0;
	vDriftROIRight = 0;
	vSpareFloatDTU = 0;
	vSpareFloatDTL = 0;
	vSpareDWordDAC = 0;
	vHeadOffsetFromIndexPulse[6] = 0;
	vSpareFloatDMiV = 0;
	vSpareFloatDAV = 0;
	vSpareFloatDSD = 0;

	vHeadOffsetFromIndexPulse[3] = 0;
	vSpareStringVN = "";
	vStructureROIStandardDievation = 0;
	vStructureROIAverage = 0;
	vSpareDoubleVT1 = 0;
	vSpareDoubleVL = 0;
	vSpareDoubleVR = 0;
	vHeadOffsetFromIndexPulse[2] = 0;
	vSpareWordVS = 0;
	vHeadOffsetFromIndexPulse[1] = 0;
	vHeadOffsetFromIndexPulse[4] = 0;
	vHeadOffsetFromIndexPulse[0] = 0;
	vHeadOffsetFromIndexPulse[5] = 0;
	vSpareByteVAS = 0;
	vSpareByteVDIF = 0;
	vSpareDoubleVOIF = 0;
	vSpareDoubleVEIF = 0;
	vSpareByteVDD = 0;
	vSpareByteVDC = 0;
	vSpareByteVDT = 0;

	vEncoderRateIndexesForFillerSeamerSampling[0] = 0;
	vEncoderRateIndexesForFillerSeamerSampling[1] = 0;
	vEncoderRateIndexesForFillerSeamerSampling[2] = 0;
	vEncoderRateIndexesForFillerSeamerSampling[3] = 0;
	vSpareWordP1 = 0;
	vSpareWordP2 = 0;
	vSpareWordP3 = 0;
	vSpareWordP4 = 0;
	vSpareWordP5 = 0;

	vSpareIntP1 = 0;
	vSpareIntP2 = 0;
	vSpareIntP3 = 0;
	vSpareIntP4 = 0;
	vSpareIntP5 = 0;

	vEdgeLocationBottom = 0;
	vEdgeLocationBottomPixel = 0;
	vEdgeLocationHeight = 0;
	vEdgeLocationHeightPixel = 0;

	vBottomLocationLeft = 0;
	vBottomLocationLength = 0;
	vBottomLocationLeftPixel = 0;
	vBottomLocationLengthPixel = 0;

	//x ray source data
	for (BYTE TempLoop = 0;TempLoop < cNumberOfXRayImageCriteriaInArchive; TempLoop++)
	{
		vSpareByteXRCE[TempLoop] = 0;
		vSpareByteXRCE1[TempLoop] = 0;
		vSpareFloatXRCP[TempLoop] = 0;
		vSpareFloatXRCP2[TempLoop] = 0;
	}
	vXRaySourceVoltage = 40 / 6;
	vXRaySourceCurrent = 8;
	vSpareByteXRSE = 1;
	vBodyTrigger = 1;
	vBackupBodyTrigger = 0;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
	{
		vExternalDetectorEnable[TempLoop] = 0;
		SetExternalDetectorWindowStart(TempLoop, 0);
		SetExternalDetectorWindowEnd(TempLoop, 0);
		vExternalDetectorMode[TempLoop] = 0;
		vExternalDetectorEjector[TempLoop] = 1;
	}
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
	{
		vEjectorDwellPosition[TempLoop] = 60;//not used
		vEjectorDwellTime[TempLoop] = 35;
		SetEjectorBeltPositionOffset(TempLoop, 64);
		vEjectorResponseTime[TempLoop] = (float).004;
	}
	for (BYTE TempLoop = 0; TempLoop < 3; TempLoop++)
		vSamplingEjector[TempLoop] = 0;
}

void CProduct::CopyProduct(CProduct *TempProduct)
{
	for (int TempLoop = 0;TempLoop < cMaximumNumberOfInspections; TempLoop++)
	if (vInspection[TempLoop])
	{
		delete vInspection[TempLoop];
		vInspection[TempLoop] = NULL;
	}

	if (vDriftDensityInspection)
	{
		delete vDriftDensityInspection;
		vDriftDensityInspection = NULL;
	}

	if (vFillerMonitorInspection)
	{
		delete vFillerMonitorInspection;
		vFillerMonitorInspection = NULL;
	}

	SetProductName(*(TempProduct->GetProductName()));
	vHistoryTookAverageDate = TempProduct->vHistoryTookAverageDate;
	vMaximumImageOverlap = TempProduct->vMaximumImageOverlap;
	vReferenceTop = TempProduct->vReferenceTop;
	vReferenceBottom = TempProduct->vReferenceBottom;
	vReferenceLeft = TempProduct->vReferenceLeft;
	vReferenceRight = TempProduct->vReferenceRight;
	vReferenceWidth = TempProduct->vReferenceWidth;
	vProductCodeSelectionCString = TempProduct->vProductCodeSelectionCString;
	vUsingXScanPoint4mmDetectors = TempProduct->vUsingXScanPoint4mmDetectors;
	vNumberOfInspectionsRequiredToEject = TempProduct->vNumberOfInspectionsRequiredToEject;
	vMultiLaneNumberOfLanes = TempProduct->vMultiLaneNumberOfLanes;
	vMultiLaneNumberOfLinesBeforeContainer = TempProduct->vMultiLaneNumberOfLinesBeforeContainer;
	for (BYTE TempLoop = 0; TempLoop < cMaximumNumberForMultiLane; TempLoop++)
	{
		vMultiLaneStartPixel[TempLoop] = TempProduct->vMultiLaneStartPixel[TempLoop];
		vMultiLaneWidthInPixels[TempLoop] = TempProduct->vMultiLaneWidthInPixels[TempLoop];
	}

	//ReportErrorMessage("Set Reference Copy Right: " + dtoa(vReferenceRight, 2) + ", Left: " + dtoa(vReferenceLeft, 2) + ", Width: " + dtoa(vReferenceWidth, 2), cAction, 0);

	for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
		vEncoderRateIndexesForFillerSeamerSampling[TempLoop] = TempProduct->vEncoderRateIndexesForFillerSeamerSampling[TempLoop];

	for (BYTE TempLoop = 0; TempLoop < 8; TempLoop++)
	vHeadOffsetFromIndexPulse[TempLoop] = TempProduct->vHeadOffsetFromIndexPulse[TempLoop];

	//vLeftJarWell = TempProduct->vLeftJarWell;
	//vRightJarWell = TempProduct->vRightJarWell;
	vXRayIntegrationTime = TempProduct->vXRayIntegrationTime;
	vXRayIntegrationTimeByte = TempProduct->vXRayIntegrationTimeByte;
	vReferenceROIsToEdges = TempProduct->vReferenceROIsToEdges;
	SetOverScanFactor(TempProduct->vOverScanFactor);
	vNotes = TempProduct->vNotes;
	vTooManyRejectsAlarmX[0] = TempProduct->vTooManyRejectsAlarmX[0];
	vTooManyRejectsAlarmY[0] = TempProduct->vTooManyRejectsAlarmY[0];
	vTooManyRejectsAlarmX[1] = TempProduct->vTooManyRejectsAlarmX[1];
	vTooManyRejectsAlarmY[1] = TempProduct->vTooManyRejectsAlarmY[1];
	vTooManyRejectsAlarmDisableEjectors[0] = TempProduct->vTooManyRejectsAlarmDisableEjectors[0];
	vTooManyRejectsAlarmDisableEjectors[1] = TempProduct->vTooManyRejectsAlarmDisableEjectors[1];
	vInspectionAlarmSettings[0] = TempProduct->vInspectionAlarmSettings[0];
	vInspectionAlarmSettings[1] = TempProduct->vInspectionAlarmSettings[1];

	vHaveManuallySetRetriggerLockout = TempProduct->vHaveManuallySetRetriggerLockout;
	vSetupHistoryStructureCount = TempProduct->vSetupHistoryStructureCount;
	vSetupHistoryHistogramThreshold = TempProduct->vSetupHistoryHistogramThreshold;
	vSetupHistoryAverageCount = TempProduct->vSetupHistoryAverageCount;
	vNeedToRelearn = TempProduct->vNeedToRelearn;
	vAddToLearnCount = TempProduct->vAddToLearnCount;
	
	vDriftROITop = TempProduct->vDriftROITop;
	vDriftROIBottom = TempProduct->vDriftROIBottom;
	vDriftROILeft = TempProduct->vDriftROILeft;
	vDriftROIRight = TempProduct->vDriftROIRight;

	vSpareByteCanImageBottomOfContainer = TempProduct->vSpareByteCanImageBottomOfContainer;
	vThresholdForContainerBounds = TempProduct->vThresholdForContainerBounds;
	vProductImageHeightTop = TempProduct->vProductImageHeightTop;
	vProductImageHeightBottom = TempProduct->vProductImageHeightBottom;
	vProductImageWidth = TempProduct->vProductImageWidth;
	vImageWidthPosition = TempProduct->vImageWidthPosition;
	vProductLockOutWidth = TempProduct->vProductLockOutWidth;
	vLockoutPosition = TempProduct->vLockoutPosition;
	vProductBodyTriggerToImageBeltPositionOffset = TempProduct->vProductBodyTriggerToImageBeltPositionOffset;
	vBulkProductMode = TempProduct->vBulkProductMode;
	vBTToImagePosition = TempProduct->vBTToImagePosition;
	vEndOfLineTimeOut = TempProduct->vEndOfLineTimeOut;
	vEndOfLineBeltPosition = TempProduct->vEndOfLineBeltPosition;

	vEdgeLocationBottom = TempProduct->vEdgeLocationBottom;
	vEdgeLocationBottomPixel = TempProduct->vEdgeLocationBottomPixel;
	vEdgeLocationHeight = TempProduct->vEdgeLocationHeight;
	vEdgeLocationHeightPixel = TempProduct->vEdgeLocationHeightPixel;

	vBottomLocationLeft = TempProduct->vBottomLocationLeft;
	vBottomLocationLength = TempProduct->vBottomLocationLength;
	vBottomLocationLeftPixel = TempProduct->vBottomLocationLeftPixel;
	vBottomLocationLengthPixel = TempProduct->vBottomLocationLengthPixel;

	// main x ray detector configuration
	for (BYTE TempLoop = 0;TempLoop < cNumberOfXRayImageCriteriaInArchive; TempLoop++)
	{
		vSpareByteXRCE[TempLoop] = TempProduct->vSpareByteXRCE[TempLoop];
		vSpareByteXRCE1[TempLoop] = TempProduct->vSpareByteXRCE1[TempLoop];
		vSpareFloatXRCP[TempLoop] = TempProduct->vSpareFloatXRCP[TempLoop];
		vSpareFloatXRCP2[TempLoop] = TempProduct->vSpareFloatXRCP2[TempLoop];
	}
	vXRaySourceVoltage = TempProduct->vXRaySourceVoltage;
	vXRaySourceCurrent = TempProduct->vXRaySourceCurrent;
	vSpareByteXRSE = TempProduct->vSpareByteXRSE;
	vBodyTrigger = TempProduct->vBodyTrigger;
	vBackupBodyTrigger = TempProduct->vBackupBodyTrigger;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
	{
		vExternalDetectorEnable[TempLoop] = TempProduct->vExternalDetectorEnable[TempLoop];
		vExternalDetectorMode[TempLoop] = TempProduct->vExternalDetectorMode[TempLoop];
		SetExternalDetectorWindowStart(TempLoop, TempProduct->vExternalDetectorWindowStart[TempLoop]);
		SetExternalDetectorWindowEnd(TempLoop, TempProduct->vExternalDetectorWindowEnd[TempLoop]);
		vExternalDetectorEjector[TempLoop] = TempProduct->vExternalDetectorEjector[TempLoop];
	}
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
	{
		//vEjectorDwellPosition[TempLoop] = TempProduct->vEjectorDwellPosition[TempLoop];
		vEjectorDwellTime[TempLoop] = TempProduct->vEjectorDwellTime[TempLoop];
		SetEjectorBeltPositionOffset(TempLoop,TempProduct->vEjectorDelayPosition[TempLoop]);
		vEjectorResponseTime[TempLoop] = TempProduct->vEjectorResponseTime[TempLoop];
	}

	for (BYTE TempLoop = 0;TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
		vSpareDACDetectorOffset[TempLoop] = TempProduct->vSpareDACDetectorOffset[TempLoop];
	vSpareElectronicOffset = TempProduct->vSpareElectronicOffset;
	
	vDriftDensityInspection = NULL;
	if (TempProduct->vDriftDensityInspection)
	{
		if (ThereIsEnoughMemory(sizeof(CInspectionDensity), "New Inspection"))
			vDriftDensityInspection = new CInspectionDensity;
		CInspectionDensity *TempInspectionToCopyPointer = (CInspectionDensity *)TempProduct->vDriftDensityInspection;
		vDriftDensityInspection->CopyInspection(TempInspectionToCopyPointer, vOverScanMultiplier);
	}
	
	vFillerMonitorInspection = NULL;
	if (TempProduct->vFillerMonitorInspection)
	{
		if (ThereIsEnoughMemory(sizeof(CInspectionDensity), "New Inspection"))
			vFillerMonitorInspection = new CInspectionDensity;
		CInspectionDensity *TempInspectionToCopyPointer = (CInspectionDensity *)TempProduct->vFillerMonitorInspection;
		vFillerMonitorInspection->CopyInspection(TempInspectionToCopyPointer, vOverScanMultiplier);
	}
	
	vMaxConveyorSpeedAsEntered = TempProduct->vMaxConveyorSpeedAsEntered;
	vHistogramThreshold = TempProduct->vHistogramThreshold;
	vHistogramLowerThreshold = TempProduct->vHistogramLowerThreshold;
	vHistogramDilateTimes = TempProduct->vHistogramDilateTimes;
	//vGlassHistogramDilateTimes = TempProduct->vGlassHistogramDilateTimes;
	vTypesOfRejectsToView = TempProduct->vTypesOfRejectsToView;
	vNumberOfInspections = TempProduct->vNumberOfInspections;
	vNumberOfAuxiliaryDetectors = TempProduct->vNumberOfAuxiliaryDetectors;
	for (BYTE TempLoop = 0; TempLoop < vNumberOfInspections; TempLoop++)
	if (TempProduct->vInspection[TempLoop])
	{
		switch (TempProduct->vInspection[TempLoop]->vInspectionType)
		{
			case cJarBottomContaminantInspection:
			{
				CInspectionBottomContaminant *TempInspectionToEditPointer = NULL;
				if (ThereIsEnoughMemory(sizeof(CInspectionBottomContaminant), "New Inspection"))
					TempInspectionToEditPointer = new CInspectionBottomContaminant;
				TempInspectionToEditPointer->CopyInspection(TempProduct->vInspection[TempLoop],
					vOverScanMultiplier);
				vInspection[TempLoop] = TempInspectionToEditPointer;
			}
			break;
			case cContaminantInspection:
			case cWireContaminantInspection:
			case cSpotInROIContaminantInspection:
			case cThresholdContaminantInspection:
			case cBadHeartInspection:
			case cDripBySpoutInspection:
			case cDripInspection:
			case cFloodedInspection:
			case cTopContaminantInspection:
			case cBottomContaminantInspection:
			case cContaminant2Inspection:
			case cAdvancedContaminant2Inspection:
			case cContaminantBonesInspection:
			case cContaminant22Inspection:
			case cGlass13mmContaminantInspection:
			case cIPContaminantInspection:
			{
				CInspectionContaminant *TempInspectionToEditPointer = NULL;
				if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
					TempInspectionToEditPointer = new CInspectionContaminant;
				TempInspectionToEditPointer->CopyInspection(TempProduct->vInspection[TempLoop], vOverScanMultiplier);
				vInspection[TempLoop] = TempInspectionToEditPointer;

				//if ((TempProduct->vInspection[TempLoop]->vInspectionType == cContaminant22Inspection) || (TempProduct->vInspection[TempLoop]->vDoContaminant22Algorithm))
				//	vInspection[TempLoop]->InspxContaminant22CreateBuffers(TempSizeX, TempSizeY);
			}
			break;
			case cKernelInspection:
			{
				CInspectionKernel *TempInspectionToEditPointer = NULL;
				if (ThereIsEnoughMemory(sizeof(CInspectionKernel), "New Inspection"))
					TempInspectionToEditPointer = new CInspectionKernel;
				TempInspectionToEditPointer->CopyInspection(TempProduct->vInspection[TempLoop],
					vOverScanMultiplier);
				vInspection[TempLoop] = TempInspectionToEditPointer;
			}
			break;
			case cVoidInspection:
			{
				CInspectionVoid *TempInspectionToEditPointer = NULL;
				if (ThereIsEnoughMemory(sizeof(CInspectionVoid), "New Inspection"))
					TempInspectionToEditPointer = new CInspectionVoid;
				TempInspectionToEditPointer->CopyInspection(TempProduct->vInspection[TempLoop],
					vOverScanMultiplier);
				vInspection[TempLoop] = TempInspectionToEditPointer;
			}
			break;
			case cDensityInspection:
			case cMissingLidInspection:
			case cUnderfillInspection:
			//case cUnderfillByWeightInspection:
			//case cOverfillByWeightInspection:
			case cOverfillInspection:
			{
				CInspectionDensity *TempInspectionToEditPointer = NULL;

				if (ThereIsEnoughMemory(sizeof(CInspectionDensity), "New Inspection"))
					TempInspectionToEditPointer = new CInspectionDensity;

				CInspectionDensity *TempInspectionToCopyPointer = (CInspectionDensity *)TempProduct->vInspection[TempLoop];
				TempInspectionToEditPointer->CopyInspection(TempInspectionToCopyPointer, vOverScanMultiplier);
				vInspection[TempLoop] = TempInspectionToEditPointer;
			}
			break;
			case cCheckWeighInspection:
			case cUnderfillByWeightInspection:
			case cOverfillByWeightInspection:
			{
				CInspectionCheckWeigh *TempInspectionToEditPointer = NULL;

				if (ThereIsEnoughMemory(sizeof(CInspectionCheckWeigh), "New Inspection"))
					TempInspectionToEditPointer = new CInspectionCheckWeigh;

				CInspectionCheckWeigh *TempInspectionToCopyPointer = (CInspectionCheckWeigh *)TempProduct->vInspection[TempLoop];
				TempInspectionToEditPointer->CopyInspection(TempInspectionToCopyPointer, vOverScanMultiplier);
				vInspection[TempLoop] = TempInspectionToEditPointer;
			}
			break;
			case cItemCountInspection:
			case cSizeInspection:
			case cMissingLidInspectionOnSize:
			{
				CInspectionSize *TempInspectionToEditPointer = NULL;
				if (ThereIsEnoughMemory(sizeof(CInspectionSize), "New Inspection"))
					TempInspectionToEditPointer = new CInspectionSize;
				CInspectionSize *TempInspectionToCopyPointer = (CInspectionSize *)TempProduct->vInspection[TempLoop];
				TempInspectionToEditPointer->CopyInspection(TempInspectionToCopyPointer, vOverScanMultiplier);
				vInspection[TempLoop] = TempInspectionToEditPointer;
			}
			break;
			case cStandardDeviationInspection:
			{
				CInspectionStandardDeviation *TempInspectionToEditPointer = NULL;
				if (ThereIsEnoughMemory(sizeof(CInspectionStandardDeviation), "New Inspection"))
					TempInspectionToEditPointer = new CInspectionStandardDeviation;
				CInspectionStandardDeviation *TempInspectionToCopyPointer = (CInspectionStandardDeviation *)TempProduct->vInspection[TempLoop];
				TempInspectionToEditPointer->CopyInspection(TempInspectionToCopyPointer, vOverScanMultiplier);
				vInspection[TempLoop] = TempInspectionToEditPointer;
			}
			break;
		}
	}
}

IMPLEMENT_SERIAL(CProduct, CObject, VERSIONABLE_SCHEMA | cCurrentArchiveVersionNumber)

void CProduct::Serialize(CArchive& TempArchive)
{
  // call base class function first
  // base class is CObject in this case
  CObject::Serialize(TempArchive);

	float TempHaveAFillerMonitorInspection = 0;
  // now do the stuff for our specific class
  if (TempArchive.IsStoring())
	{
		CheckInspectionsAreValid();

		if (vFillerMonitorInspection)
			TempHaveAFillerMonitorInspection = 0xAA;

		TempArchive << vProductName 
			<< vHistoryTookAverageDate
			<< vProductImageWidth 
			<< vProductLockOutWidth
			<< vProductBodyTriggerToImageBeltPositionOffset
			<< vProductImageHeightTop
			<< vProductImageHeightBottom
			<< vEndOfLineTimeOut
			<< vXRaySourceVoltage
			<< vXRaySourceCurrent
			<< vXRayIntegrationTimeByte
			<< vSpareByteXRSE
			<< vBodyTrigger
			<< vThresholdForContainerBounds
			<< vSpareByteCanImageBottomOfContainer
			<< vSamplingEjector[0]
			<< vFillerTolerance
			<< vSpareDoubleCB
			<< vSpareDoubleCL
			<< vSpareDoubleCR
			<< vBackupBodyTrigger
			<< vReferenceTop
			<< vReferenceLeft
			<< vReferenceRight
			<< vXRayIntegrationTime
			<< vUsingXScanPoint4mmDetectors
			<< vBulkProductMode
			<< vNumberOfInspectionsRequiredToEject
			<< vSpareDoubleWasStructureAdjustFactor
			<< vMultiLaneEjectAdjacentLanes
			<< vSpareByteDT
			<< vSpareDoubleWasStructureAdjustSubtractiveFactor
			<< vStructureROIAverage
			<< vMultiLaneNumberOfLanes
			<< vMultiLaneNumberOfLinesBeforeContainer
			<< vSamplingEjector[1]
			<< vTempScreenShotDirectoryName
			<< vHaveManuallySetRetriggerLockout
			<< vReferenceROIsToEdges
			<< vAddToLearnCount
			<< vMaximumImageOverlap
			<< vNeedToRelearn
			<< vEdgeLocationBottom
			<< vEdgeLocationBottomPixel
			<< vEdgeLocationHeight
			<< vEdgeLocationHeightPixel
			<< vSpareByteSHMG
			<< vBottomLocationLeft
			<< vBottomLocationLength
			<< vSpareByteSHMB
			<< TempHaveAFillerMonitorInspection
			<< vSpareFloatSHMBSD
			<< vSetupHistoryHistogramThreshold
			<< vSetupHistoryAverageCount
			<< vSetupHistoryStructureCount

			<< vHeadOffsetFromIndexPulse[6]
			<< vSpareStringHDN
			<< vSpareStringLDN
			<< vDriftROITop
			<< vDriftROIBottom
			<< vDriftROILeft
			<< vDriftROIRight
			<< vSpareFloatDTU
			<< vSpareFloatDTL
			<< vSpareDWordDAC
			<< vReferenceBottom
			<< vHeadOffsetFromIndexPulse[6]
			<< vSpareFloatDMV
			<< vSpareFloatDMiV
			<< vSpareFloatDAV
			<< vSpareFloatDSD

			<< vHeadOffsetFromIndexPulse[3]
			<< vSpareStringVN
			<< vStructureROIStandardDievation
			<< vSpareDoubleVT1
			<< vSpareDoubleVL
			<< vSpareDoubleVR
			<< vHeadOffsetFromIndexPulse[2]
			<< vSpareWordVS
			<< vHeadOffsetFromIndexPulse[cFillerSamplingType]
			<< vHeadOffsetFromIndexPulse[4]
			<< vHeadOffsetFromIndexPulse[0]
			<< vHeadOffsetFromIndexPulse[5]
			<< vSpareByteVAS
			<< vSetupHistoryContainerTriggerStandardDeviation
			<< vSpareByteVDIF
			<< vSpareDoubleVOIF
			<< vSpareDoubleVEIF
			<< vSpareByteVDD
			<< vSpareByteVDC
			<< vSpareByteVDT

			<< vSpareByte
			<< vSpareFloat1
			<< vSpareFloat2
			<< vSpareByteVSHMB
			<< vSpareFloatVSHMBA
			<< vSpareFloatVSHMBSD
			<< vSpareByteVSHHT
			<< vBottomLocationLeftPixel
			<< vBottomLocationLengthPixel
			<< vSpareDWordVSHSC
			<< vSamplingEjector[2]
			<< vTooManyRejectsAlarmX[0]
			<< vTooManyRejectsAlarmY[0]
			<< vTooManyRejectsAlarmDisableEjectors[0]
			<< vTooManyRejectsAlarmX[1]
			<< vTooManyRejectsAlarmY[1]
			<< vTooManyRejectsAlarmDisableEjectors[1]
			<< vSpareByteVSHMG
			<< vSpareFloatVSHMGA
			<< vSpareFloatVSHMGSD
			<< vOverScanFactor
			<< vNotes
			<< vInspectionAlarmSettings[0]
			<< vInspectionAlarmSettings[1]
			<< vSpareDWordVSMGC
			<< vSpareDWordVSMBC
			<< vSpareDWordVSHAC
			<< vSpareDWordVSHSC
			<< vHeadOffsetFromIndexPulse[7]
			<< vSpareByteVSHAT1
			<< vSpareFloatSHDATU
			<< vSpareFloatSHDATL
			<< vNumberOfInspections
			<< vSetupHistoryAverageDensity
			<< vSpareElectronicOffset
			<< vDriftDensityInspection
			<< vMaxConveyorSpeedAsEntered
			<< vTypesOfRejectsToView
			<< vNumberOfAuxiliaryDetectors
			<< vHistogramThreshold
			<< vHistogramLowerThreshold
			<< vHistogramDilateTimes
			<< vProductCodeSelectionCString
			<< vSpareCString1
			<< vSpareCString2
			<< vSpareCString3
			<< vSpareCString4
			<< vSpareCString5
			<< vEncoderRateIndexesForFillerSeamerSampling[0]
			<< vEncoderRateIndexesForFillerSeamerSampling[1]
			<< vEncoderRateIndexesForFillerSeamerSampling[2]
			<< vEncoderRateIndexesForFillerSeamerSampling[3]
			<< vSpareWordP1
			<< vSpareWordP2
			<< vSpareWordP3
			<< vSpareWordP4
			<< vSpareWordP5
			<< vSpareIntP1
			<< vSpareIntP2
			<< vSpareIntP3
			<< vSpareIntP4
			<< vSpareIntP5;

		if (TempHaveAFillerMonitorInspection == 0xAA)
			TempArchive << vFillerMonitorInspection;

		if (vMultiLaneNumberOfLanes)
		for (BYTE TempLoop = 0; TempLoop < cMaximumNumberForMultiLane; TempLoop++)
		{
			TempArchive << vMultiLaneStartPixel[TempLoop]
				<< vMultiLaneWidthInPixels[TempLoop];
		}

		for (int TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
		{
			TempArchive << vExternalDetectorEnable[TempLoop]
				<< vExternalDetectorWindowStart[TempLoop]
				<< vExternalDetectorWindowEnd[TempLoop]
				<< vExternalDetectorMode[TempLoop]
				<< vExternalDetectorEjector[TempLoop];
		}
		for (int TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
		{
			TempArchive //<< vEjectorDwellPosition[TempLoop]
				<< vEjectorDwellTime[TempLoop]
				<< vEjectorDelayPosition[TempLoop]
				<< vEjectorResponseTime[TempLoop];
		}
		for (int TempLoop = 0; TempLoop < vNumberOfInspections; TempLoop++)
		{
			TempArchive << vInspection[TempLoop];
		}
	}
  else  //reading archive data
	{
		// may read different versions of object if have them
		// int vVersion = archive.GetObjectSchema();
    int TempVersion = TempArchive.GetObjectSchema();

		if (TempVersion > 156)
		{  // version 157 and up
			TempArchive >> vProductName 
				>> vHistoryTookAverageDate
				>> vProductImageWidth 
				>> vProductLockOutWidth
				>> vProductBodyTriggerToImageBeltPositionOffset
				>> vProductImageHeightTop
				>> vProductImageHeightBottom
				>> vEndOfLineTimeOut
				>> vXRaySourceVoltage
				>> vXRaySourceCurrent
				>> vXRayIntegrationTimeByte
				>> vSpareByteXRSE
				>> vBodyTrigger
				>> vThresholdForContainerBounds
				>> vSpareByteCanImageBottomOfContainer
				>> vSamplingEjector[0]
				>> vFillerTolerance
				>> vSpareDoubleCB
				>> vSpareDoubleCL
				>> vSpareDoubleCR
				>> vBackupBodyTrigger
				>> vReferenceTop
				>> vReferenceLeft
				>> vReferenceRight
				>> vXRayIntegrationTime
				>> vUsingXScanPoint4mmDetectors
				>> vBulkProductMode
				>> vNumberOfInspectionsRequiredToEject
				>> vSpareDoubleWasStructureAdjustFactor
				>> vMultiLaneEjectAdjacentLanes
				>> vSpareByteDT
				>> vSpareDoubleWasStructureAdjustSubtractiveFactor
				>> vStructureROIAverage
				>> vMultiLaneNumberOfLanes
				>> vMultiLaneNumberOfLinesBeforeContainer
				>> vSamplingEjector[1]
				>> vTempScreenShotDirectoryName
				>> vHaveManuallySetRetriggerLockout
				>> vReferenceROIsToEdges
				>> vAddToLearnCount
				>> vMaximumImageOverlap
				>> vNeedToRelearn
				>> vEdgeLocationBottom
				>> vEdgeLocationBottomPixel
				>> vEdgeLocationHeight
				>> vEdgeLocationHeightPixel
				>> vSpareByteSHMG
				>> vBottomLocationLeft
				>> vBottomLocationLength
				>> vSpareByteSHMB
				>> TempHaveAFillerMonitorInspection
				>> vSpareFloatSHMBSD
				>> vSetupHistoryHistogramThreshold
				>> vSetupHistoryAverageCount
				>> vSetupHistoryStructureCount

				>> vHeadOffsetFromIndexPulse[6]
				>> vSpareStringHDN
				>> vSpareStringLDN
				>> vDriftROITop
				>> vDriftROIBottom
				>> vDriftROILeft
				>> vDriftROIRight
				>> vSpareFloatDTU
				>> vSpareFloatDTL
				>> vSpareDWordDAC
				>> vReferenceBottom
				>> vHeadOffsetFromIndexPulse[6]
				>> vSpareFloatDMV
				>> vSpareFloatDMiV
				>> vSpareFloatDAV
				>> vSpareFloatDSD

				>> vHeadOffsetFromIndexPulse[3]
				>> vSpareStringVN
				>> vStructureROIStandardDievation
				>> vSpareDoubleVT1
				>> vSpareDoubleVL
				>> vSpareDoubleVR
				>> vHeadOffsetFromIndexPulse[2]
				>> vSpareWordVS
				>> vHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vHeadOffsetFromIndexPulse[4]
				>> vHeadOffsetFromIndexPulse[0]
				>> vHeadOffsetFromIndexPulse[5]
				>> vSpareByteVAS
				>> vSetupHistoryContainerTriggerStandardDeviation
				>> vSpareByteVDIF
				>> vSpareDoubleVOIF
				>> vSpareDoubleVEIF
				>> vSpareByteVDD
				>> vSpareByteVDC
				>> vSpareByteVDT

				>> vSpareByte
				>> vSpareFloat1
				>> vSpareFloat2
				>> vSpareByteVSHMB
				>> vSpareFloatVSHMBA
				>> vSpareFloatVSHMBSD
				>> vSpareByteVSHHT
				>> vBottomLocationLeftPixel
				>> vBottomLocationLengthPixel
				>> vSpareDWordVSHSC
				>> vSamplingEjector[2]
				>> vTooManyRejectsAlarmX[0]
				>> vTooManyRejectsAlarmY[0]
				>> vTooManyRejectsAlarmDisableEjectors[0]
				>> vTooManyRejectsAlarmX[1]
				>> vTooManyRejectsAlarmY[1]
				>> vTooManyRejectsAlarmDisableEjectors[1]
				>> vSpareByteVSHMG
				>> vSpareFloatVSHMGA
				>> vSpareFloatVSHMGSD
				>> vOverScanFactor
				>> vNotes
				>> vInspectionAlarmSettings[0]
				>> vInspectionAlarmSettings[1]
				>> vSpareDWordVSMGC
				>> vSpareDWordVSMBC
				>> vSpareDWordVSHAC
				>> vSpareDWordVSHSC
				>> vHeadOffsetFromIndexPulse[7]
				>> vSpareByteVSHAT1
				>> vSpareFloatSHDATU
				>> vSpareFloatSHDATL
				>> vNumberOfInspections
				>> vSetupHistoryAverageDensity
				>> vSpareElectronicOffset
				>> vDriftDensityInspection
				>> vMaxConveyorSpeedAsEntered
				>> vTypesOfRejectsToView
				>> vNumberOfAuxiliaryDetectors
				>> vHistogramThreshold
				>> vHistogramLowerThreshold
				>> vHistogramDilateTimes
				>> vProductCodeSelectionCString
				>> vSpareCString1
				>> vSpareCString2
				>> vSpareCString3
				>> vSpareCString4
				>> vSpareCString5
				>> vEncoderRateIndexesForFillerSeamerSampling[0]
				>> vEncoderRateIndexesForFillerSeamerSampling[1]
				>> vEncoderRateIndexesForFillerSeamerSampling[2]
				>> vEncoderRateIndexesForFillerSeamerSampling[3]
				>> vSpareWordP1
				>> vSpareWordP2
				>> vSpareWordP3
				>> vSpareWordP4
				>> vSpareWordP5
				>> vSpareIntP1
				>> vSpareIntP2
				>> vSpareIntP3
				>> vSpareIntP4
				>> vSpareIntP5;

			if (TempHaveAFillerMonitorInspection == 0xAA)
				TempArchive >> vFillerMonitorInspection;

			if (vMultiLaneNumberOfLanes)
			for (BYTE TempLoop = 0; TempLoop < cMaximumNumberForMultiLane; TempLoop++)
			{
				TempArchive >> vMultiLaneStartPixel[TempLoop]
					>> vMultiLaneWidthInPixels[TempLoop];
			}

			for (int TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
			{
				TempArchive >> vExternalDetectorEnable[TempLoop]
					>> vExternalDetectorWindowStart[TempLoop]
					>> vExternalDetectorWindowEnd[TempLoop]
					>> vExternalDetectorMode[TempLoop]
					>> vExternalDetectorEjector[TempLoop];
			}
			for (int TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
			{
				TempArchive //>> vEjectorDwellPosition[TempLoop]
					>> vEjectorDwellTime[TempLoop]
					>> vEjectorDelayPosition[TempLoop]
					>> vEjectorResponseTime[TempLoop];
			}
			BYTE TempGoodInspectionCount = 0;
			for (int TempLoop = 0; TempLoop < vNumberOfInspections; TempLoop++)
			{
				TempArchive >> vInspection[TempGoodInspectionCount];

				//must convert Under Weight and Over Weight from density to CheckWeigh type
				if (vInspection[TempGoodInspectionCount])
				{
					if ((vInspection[TempGoodInspectionCount]->vInspectionType == cUnderfillByWeightInspection) || (vInspection[TempGoodInspectionCount]->vInspectionType == cOverfillByWeightInspection))
					{
						CInspectionCheckWeigh *TempObjectPointer = new CInspectionCheckWeigh;
						CRuntimeClass *TempCheckWeighInspectionRunTimeClass = TempObjectPointer->GetRuntimeClass();
						if (!vInspection[TempGoodInspectionCount]->IsKindOf(TempCheckWeighInspectionRunTimeClass))
						{
							TempObjectPointer->CopyDensityInspection((CInspectionDensity * )vInspection[TempLoop], vOverScanMultiplier);
							delete vInspection[TempGoodInspectionCount];
							vInspection[TempGoodInspectionCount] = (CInspectionCheckWeigh *)TempObjectPointer;
						}
						else
							delete TempObjectPointer;
					}
					TempGoodInspectionCount++;  //got a valid inspection, so increase count
				}
				else
					ReportErrorMessage("Invalid Inspection Reading Stream, Product: " + vProductName + ", Inspection: " + dtoa(TempLoop + 1, 0), cEMailInspx, 32000);
			}
			vNumberOfInspections = TempGoodInspectionCount;

			//for (TempLoop = 0;TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
			//	TempArchive >> vSpareDACDetectorOffset[TempLoop];
		} //end version 157 and up
		else
		if (TempVersion > 155)
		{  // version 156
			TempArchive >> vProductName 
				>> vHistoryTookAverageDate
				>> vProductImageWidth 
				>> vProductLockOutWidth
				>> vProductBodyTriggerToImageBeltPositionOffset
				>> vProductImageHeightTop
				>> vProductImageHeightBottom
				>> vEndOfLineTimeOut
				>> vXRaySourceVoltage
				>> vXRaySourceCurrent
				>> vXRayIntegrationTimeByte
				>> vSpareByteXRSE
				>> vBodyTrigger
				>> vThresholdForContainerBounds
				>> vSpareByteCanImageBottomOfContainer
				>> vSamplingEjectorVariableToReadInOldByte[0]
				>> vFillerTolerance
				>> vSpareDoubleCB
				>> vSpareDoubleCL
				>> vSpareDoubleCR
				>> vBackupBodyTrigger
				>> vReferenceTop
				>> vReferenceLeft
				>> vReferenceRight
				>> vXRayIntegrationTime
				>> vUsingXScanPoint4mmDetectors
				>> vBulkProductMode
				>> vNumberOfInspectionsRequiredToEject
				>> vSpareDoubleWasStructureAdjustFactor
				>> vMultiLaneEjectAdjacentLanes
				>> vSpareByteDT
				>> vSpareDoubleWasStructureAdjustSubtractiveFactor
				>> vStructureROIAverage
				>> vMultiLaneNumberOfLanes
				>> vMultiLaneNumberOfLinesBeforeContainer
				>> vSamplingEjectorVariableToReadInOldByte[1]
				>> vTempScreenShotDirectoryName
				>> vHaveManuallySetRetriggerLockout
				>> vReferenceROIsToEdges
				>> vAddToLearnCount
				>> vMaximumImageOverlap
				>> vNeedToRelearn
				>> vEdgeLocationBottom
				>> vEdgeLocationBottomPixel
				>> vEdgeLocationHeight
				>> vEdgeLocationHeightPixel
				>> vSpareByteSHMG
				>> vBottomLocationLeft
				>> vBottomLocationLength
				>> vSpareByteSHMB
				>> TempHaveAFillerMonitorInspection
				>> vSpareFloatSHMBSD
				>> vSetupHistoryHistogramThreshold
				>> vSetupHistoryAverageCount
				>> vSetupHistoryStructureCount

				>> vHeadOffsetFromIndexPulse[6]
				>> vSpareStringHDN
				>> vSpareStringLDN
				>> vDriftROITop
				>> vDriftROIBottom
				>> vDriftROILeft
				>> vDriftROIRight
				>> vSpareFloatDTU
				>> vSpareFloatDTL
				>> vSpareDWordDAC
				>> vReferenceBottom
				>> vHeadOffsetFromIndexPulse[6]
				>> vSpareFloatDMV
				>> vSpareFloatDMiV
				>> vSpareFloatDAV
				>> vSpareFloatDSD

				>> vHeadOffsetFromIndexPulse[3]
				>> vSpareStringVN
				>> vStructureROIStandardDievation
				>> vSpareDoubleVT1
				>> vSpareDoubleVL
				>> vSpareDoubleVR
				>> vHeadOffsetFromIndexPulse[2]
				>> vSpareWordVS
				>> vHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vHeadOffsetFromIndexPulse[4]
				>> vHeadOffsetFromIndexPulse[0]
				>> vHeadOffsetFromIndexPulse[5]
				>> vSpareByteVAS
				>> vSetupHistoryContainerTriggerStandardDeviation
				>> vSpareByteVDIF
				>> vSpareDoubleVOIF
				>> vSpareDoubleVEIF
				>> vSpareByteVDD
				>> vSpareByteVDC
				>> vSpareByteVDT

				>> vSpareByte
				>> vSpareFloat1
				>> vSpareFloat2
				>> vSpareByteVSHMB
				>> vSpareFloatVSHMBA
				>> vSpareFloatVSHMBSD
				>> vSpareByteVSHHT
				>> vBottomLocationLeftPixel
				>> vBottomLocationLengthPixel
				>> vSpareDWordVSHSC
				>> vSamplingEjectorVariableToReadInOldByte[2]
				>> vTooManyRejectsAlarmX[0]
				>> vTooManyRejectsAlarmY[0]
				>> vTooManyRejectsAlarmDisableEjectors[0]
				>> vTooManyRejectsAlarmX[1]
				>> vTooManyRejectsAlarmY[1]
				>> vTooManyRejectsAlarmDisableEjectors[1]
				>> vSpareByteVSHMG
				>> vSpareFloatVSHMGA
				>> vSpareFloatVSHMGSD
				>> vOverScanFactor
				>> vNotes
				>> vInspectionAlarmSettings[0]
				>> vInspectionAlarmSettings[1]
				>> vSpareDWordVSMGC
				>> vSpareDWordVSMBC
				>> vSpareDWordVSHAC
				>> vSpareDWordVSHSC
				>> vHeadOffsetFromIndexPulse[7]
				>> vSpareByteVSHAT1
				>> vSpareFloatSHDATU
				>> vSpareFloatSHDATL
				>> vNumberOfInspections
				>> vSetupHistoryAverageDensity
				>> vSpareElectronicOffset
				>> vDriftDensityInspection
				>> vMaxConveyorSpeedAsEntered
				>> vTypesOfRejectsToView
				>> vNumberOfAuxiliaryDetectors
				>> vHistogramThreshold
				>> vHistogramLowerThreshold
				>> vHistogramDilateTimes
				>> vProductCodeSelectionCString
				>> vSpareCString1
				>> vSpareCString2
				>> vSpareCString3
				>> vSpareCString4
				>> vSpareCString5
				>> vEncoderRateIndexesForFillerSeamerSampling[0]
				>> vEncoderRateIndexesForFillerSeamerSampling[1]
				>> vEncoderRateIndexesForFillerSeamerSampling[2]
				>> vEncoderRateIndexesForFillerSeamerSampling[3]
				>> vSpareWordP1
				>> vSpareWordP2
				>> vSpareWordP3
				>> vSpareWordP4
				>> vSpareWordP5
				>> vSpareIntP1
				>> vSpareIntP2
				>> vSpareIntP3
				>> vSpareIntP4
				>> vSpareIntP5;

			if (TempHaveAFillerMonitorInspection == 0xAA)
				TempArchive >> vFillerMonitorInspection;

			if (vMultiLaneNumberOfLanes)
			for (BYTE TempLoop = 0; TempLoop < cMaximumNumberForMultiLane; TempLoop++)
			{
				TempArchive >> vMultiLaneStartPixel[TempLoop]
					>> vMultiLaneWidthInPixels[TempLoop];
			}

			for (int TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
			{
				TempArchive >> vExternalDetectorEnable[TempLoop]
					>> vExternalDetectorWindowStart[TempLoop]
					>> vExternalDetectorWindowEnd[TempLoop]
					>> vExternalDetectorMode[TempLoop]
					>> vExternalDetectorEjectorVariableToReadInOldByte[TempLoop];
			}
			for (int TempLoop = 0; TempLoop < cOldNumberOfEjectorsInStream6; TempLoop++)
			{
				TempArchive >> vEjectorDwellPosition[TempLoop]
					>> vEjectorDwellTime[TempLoop]
					>> vEjectorDelayPosition[TempLoop]
					>> vEjectorResponseTime[TempLoop];
			}
			BYTE TempGoodInspectionCount = 0;
			for (int TempLoop = 0; TempLoop < vNumberOfInspections; TempLoop++)
			{
				TempArchive >> vInspection[TempGoodInspectionCount];

				//must convert Under Weight and Over Weight from density to CheckWeigh type
				if (vInspection[TempGoodInspectionCount])
				{
					if ((vInspection[TempGoodInspectionCount]->vInspectionType == cUnderfillByWeightInspection) || (vInspection[TempGoodInspectionCount]->vInspectionType == cOverfillByWeightInspection))
					{
						CInspectionCheckWeigh *TempObjectPointer = new CInspectionCheckWeigh;
						CRuntimeClass *TempCheckWeighInspectionRunTimeClass = TempObjectPointer->GetRuntimeClass();
						if (!vInspection[TempGoodInspectionCount]->IsKindOf(TempCheckWeighInspectionRunTimeClass))
						{
							TempObjectPointer->CopyDensityInspection((CInspectionDensity * )vInspection[TempLoop], vOverScanMultiplier);
							delete vInspection[TempGoodInspectionCount];
							vInspection[TempGoodInspectionCount] = (CInspectionCheckWeigh *)TempObjectPointer;
						}
						else
							delete TempObjectPointer;
					}
					TempGoodInspectionCount++;  //got a valid inspection, so increase count
				}
				else
					ReportErrorMessage("Invalid Inspection Reading Stream, Product: " + vProductName + ", Inspection: " + dtoa(TempLoop + 1, 0), cEMailInspx, 32000);
			}
			vNumberOfInspections = TempGoodInspectionCount;

			//for (TempLoop = 0;TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
			//	TempArchive >> vSpareDACDetectorOffset[TempLoop];
		} //end version 156
		else
		if (TempVersion > 152)
		{  // version 153 to 155
			TempArchive >> vProductName 
				>> vHistoryTookAverageDate
				>> vProductImageWidth 
				>> vProductLockOutWidth
				>> vProductBodyTriggerToImageBeltPositionOffset
				>> vProductImageHeightTop
				>> vProductImageHeightBottom
				>> vEndOfLineTimeOut
				>> vXRaySourceVoltage
				>> vXRaySourceCurrent
				>> vXRayIntegrationTimeByte
				>> vSpareByteXRSE
				>> vBodyTrigger
				>> vThresholdForContainerBounds
				>> vSpareByteCanImageBottomOfContainer
				>> vSamplingEjectorVariableToReadInOldByte[0]
				>> vFillerTolerance
				>> vSpareDoubleCB
				>> vSpareDoubleCL
				>> vSpareDoubleCR
				>> vBackupBodyTrigger
				>> vReferenceTop
				>> vReferenceLeft
				>> vReferenceRight
				>> vXRayIntegrationTime
				>> vUsingXScanPoint4mmDetectors
				>> vBulkProductMode
				>> vNumberOfInspectionsRequiredToEject
				>> vSpareDoubleWasStructureAdjustFactor
				>> vMultiLaneEjectAdjacentLanes
				>> vSpareByteDT
				>> vSpareDoubleWasStructureAdjustSubtractiveFactor
				>> vStructureROIAverage
				>> vMultiLaneNumberOfLanes
				>> vMultiLaneNumberOfLinesBeforeContainer
				>> vSamplingEjectorVariableToReadInOldByte[1]
				>> vTempScreenShotDirectoryName
				>> vHaveManuallySetRetriggerLockout
				>> vReferenceROIsToEdges
				>> vAddToLearnCount
				>> vMaximumImageOverlap
				>> vNeedToRelearn
				>> vEdgeLocationBottom
				>> vEdgeLocationBottomPixel
				>> vEdgeLocationHeight
				>> vEdgeLocationHeightPixel
				>> vSpareByteSHMG
				>> vBottomLocationLeft
				>> vBottomLocationLength
				>> vSpareByteSHMB
				>> TempHaveAFillerMonitorInspection
				>> vSpareFloatSHMBSD
				>> vSetupHistoryHistogramThreshold
				>> vSetupHistoryAverageCount
				>> vSetupHistoryStructureCount

				>> vHeadOffsetFromIndexPulse[6]
				>> vSpareStringHDN
				>> vSpareStringLDN
				>> vDriftROITop
				>> vDriftROIBottom
				>> vDriftROILeft
				>> vDriftROIRight
				>> vSpareFloatDTU
				>> vSpareFloatDTL
				>> vSpareDWordDAC
				>> vReferenceBottom
				>> vHeadOffsetFromIndexPulse[6]
				>> vSpareFloatDMV
				>> vSpareFloatDMiV
				>> vSpareFloatDAV
				>> vSpareFloatDSD

				>> vHeadOffsetFromIndexPulse[3]
				>> vSpareStringVN
				>> vStructureROIStandardDievation
				>> vSpareDoubleVT1
				>> vSpareDoubleVL
				>> vSpareDoubleVR
				>> vHeadOffsetFromIndexPulse[2]
				>> vSpareWordVS
				>> vHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vHeadOffsetFromIndexPulse[4]
				>> vHeadOffsetFromIndexPulse[0]
				>> vHeadOffsetFromIndexPulse[5]
				>> vSpareByteVAS
				>> vSetupHistoryContainerTriggerStandardDeviation
				>> vSpareByteVDIF
				>> vSpareDoubleVOIF
				>> vSpareDoubleVEIF
				>> vSpareByteVDD
				>> vSpareByteVDC
				>> vSpareByteVDT

				>> vSpareByte
				>> vSpareFloat1
				>> vSpareFloat2
				>> vSpareByteVSHMB
				>> vSpareFloatVSHMBA
				>> vSpareFloatVSHMBSD
				>> vSpareByteVSHHT
				>> vBottomLocationLeftPixel
				>> vBottomLocationLengthPixel
				>> vSpareDWordVSHSC
				>> vSamplingEjectorVariableToReadInOldByte[2]
				>> vTooManyRejectsAlarmX[0]
				>> vTooManyRejectsAlarmY[0]
				>> vTooManyRejectsAlarmDisableEjectors[0]
				>> vTooManyRejectsAlarmX[1]
				>> vTooManyRejectsAlarmY[1]
				>> vTooManyRejectsAlarmDisableEjectors[1]
				>> vSpareByteVSHMG
				>> vSpareFloatVSHMGA
				>> vSpareFloatVSHMGSD
				>> vOverScanFactor
				>> vNotes
				>> vInspectionAlarmSettings[0]
				>> vInspectionAlarmSettings[1]
				>> vSpareDWordVSMGC
				>> vSpareDWordVSMBC
				>> vSpareDWordVSHAC
				>> vSpareDWordVSHSC
				>> vHeadOffsetFromIndexPulse[7]
				>> vSpareByteVSHAT1
				>> vSpareFloatSHDATU
				>> vSpareFloatSHDATL
				>> vNumberOfInspections
				>> vSetupHistoryAverageDensity
				>> vSpareElectronicOffset
				>> vDriftDensityInspection
				>> vMaxConveyorSpeedAsEntered
				>> vTypesOfRejectsToView
				>> vNumberOfAuxiliaryDetectors
				>> vHistogramThreshold
				>> vHistogramLowerThreshold
				>> vHistogramDilateTimes
				>> vProductCodeSelectionCString
				>> vSpareCString1
				>> vSpareCString2
				>> vSpareCString3
				>> vSpareCString4
				>> vSpareCString5
				>> vEncoderRateIndexesForFillerSeamerSampling[0]
				>> vEncoderRateIndexesForFillerSeamerSampling[1]
				>> vEncoderRateIndexesForFillerSeamerSampling[2]
				>> vEncoderRateIndexesForFillerSeamerSampling[3]
				>> vSpareWordP5;


			if (TempHaveAFillerMonitorInspection == 0xAA)
				TempArchive >> vFillerMonitorInspection;

			int TempLoop = 0;
			//for (; TempLoop < cNumberOfXRayImageCriteriaInArchive; TempLoop++)
			//{
			//	TempArchive >> vSpareByteXRCE[TempLoop]
			//	>> vSpareByteXRCE1[TempLoop]
			//	>> vSpareFloatXRCP[TempLoop]
			//	>> vSpareFloatXRCP2[TempLoop];
			//}
			
			for (TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
			{
				TempArchive >> vExternalDetectorEnable[TempLoop]
					>> vExternalDetectorWindowStart[TempLoop]
					>> vExternalDetectorWindowEnd[TempLoop]
					>> vExternalDetectorMode[TempLoop]
					>> vExternalDetectorEjectorVariableToReadInOldByte[TempLoop];
			}
			for (TempLoop = 0; TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
			{
				TempArchive >> vEjectorDwellPosition[TempLoop]
					>> vEjectorDwellTime[TempLoop]
					>> vEjectorDelayPosition[TempLoop]
					>> vEjectorResponseTime[TempLoop];
			}
			BYTE TempGoodInspectionCount = 0;
			for (TempLoop = 0; TempLoop < vNumberOfInspections; TempLoop++)
			{
				TempArchive >> vInspection[TempGoodInspectionCount];

				//must convert Under Weight and Over Weight from density to CheckWeigh type
				if (vInspection[TempGoodInspectionCount])
				{
					if ((vInspection[TempGoodInspectionCount]->vInspectionType == cUnderfillByWeightInspection) || (vInspection[TempGoodInspectionCount]->vInspectionType == cOverfillByWeightInspection))
					{
						CInspectionCheckWeigh *TempObjectPointer = new CInspectionCheckWeigh;
						CRuntimeClass *TempCheckWeighInspectionRunTimeClass = TempObjectPointer->GetRuntimeClass();
						if (!vInspection[TempGoodInspectionCount]->IsKindOf(TempCheckWeighInspectionRunTimeClass))
						{
							TempObjectPointer->CopyDensityInspection((CInspectionDensity * )vInspection[TempLoop], vOverScanMultiplier);
							delete vInspection[TempGoodInspectionCount];
							vInspection[TempGoodInspectionCount] = (CInspectionCheckWeigh *)TempObjectPointer;
						}
						else
							delete TempObjectPointer;
					}
					TempGoodInspectionCount++;  //got a valid inspection, so increase count
				}
				else
					ReportErrorMessage("Invalid Inspection Reading Stream, Product: " + vProductName + ", Inspection: " + dtoa(TempLoop + 1, 0), cEMailInspx, 32000);
			}
			vNumberOfInspections = TempGoodInspectionCount;

			//for (TempLoop = 0;TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
			//	TempArchive >> vSpareDACDetectorOffset[TempLoop];
		} //end version 153 to 155
		else
		if (TempVersion > 149)
		{  // version 150 to 152
			TempArchive >> vProductName 
				>> vHistoryTookAverageDate
				>> vProductImageWidth 
				>> vProductLockOutWidth
				>> vProductBodyTriggerToImageBeltPositionOffset
				>> vProductImageHeightTop
				>> vProductImageHeightBottom
				>> vEndOfLineTimeOut
				>> vXRaySourceVoltage
				>> vXRaySourceCurrent
				>> vXRayIntegrationTimeByte
				>> vSpareByteXRSE
				>> vBodyTrigger
				>> vThresholdForContainerBounds
				>> vSpareByteCanImageBottomOfContainer
				>> vSamplingEjectorVariableToReadInOldByte[0]
				>> vFillerTolerance
				>> vSpareDoubleCB
				>> vSpareDoubleCL
				>> vSpareDoubleCR
				>> vBackupBodyTrigger
				>> vReferenceTop
				>> vReferenceLeft
				>> vReferenceRight
				>> vXRayIntegrationTime
				>> vUsingXScanPoint4mmDetectors
				>> vBulkProductMode
				>> vNumberOfInspectionsRequiredToEject
				>> vSpareDoubleWasStructureAdjustFactor
				>> vMultiLaneEjectAdjacentLanes
				>> vSpareByteDT
				>> vSpareDoubleWasStructureAdjustSubtractiveFactor
				>> vStructureROIAverage
				>> vMultiLaneNumberOfLanes
				>> vMultiLaneNumberOfLinesBeforeContainer
				>> vSamplingEjectorVariableToReadInOldByte[1]
				>> vTempScreenShotDirectoryName
				>> vHaveManuallySetRetriggerLockout
				>> vReferenceROIsToEdges
				>> vAddToLearnCount
				>> vMaximumImageOverlap
				>> vNeedToRelearn
				>> vEdgeLocationBottom
				>> vEdgeLocationBottomPixel
				>> vEdgeLocationHeight
				>> vEdgeLocationHeightPixel
				>> vSpareByteSHMG
				>> vBottomLocationLeft
				>> vBottomLocationLength
				>> vSpareByteSHMB
				>> TempHaveAFillerMonitorInspection
				>> vSpareFloatSHMBSD
				>> vSetupHistoryHistogramThreshold
				>> vSetupHistoryAverageCount
				>> vSetupHistoryStructureCount

				>> vHeadOffsetFromIndexPulse[6]
				>> vSpareStringHDN
				>> vSpareStringLDN
				>> vDriftROITop
				>> vDriftROIBottom
				>> vDriftROILeft
				>> vDriftROIRight
				>> vSpareFloatDTU
				>> vSpareFloatDTL
				>> vSpareDWordDAC
				>> vReferenceBottom
				>> vHeadOffsetFromIndexPulse[6]
				>> vSpareFloatDMV
				>> vSpareFloatDMiV
				>> vSpareFloatDAV
				>> vSpareFloatDSD

				>> vHeadOffsetFromIndexPulse[3]
				>> vSpareStringVN
				>> vStructureROIStandardDievation
				>> vSpareDoubleVT1
				>> vSpareDoubleVL
				>> vSpareDoubleVR
				>> vHeadOffsetFromIndexPulse[2]
				>> vSpareWordVS
				>> vHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vHeadOffsetFromIndexPulse[4]
				>> vHeadOffsetFromIndexPulse[0]
				>> vHeadOffsetFromIndexPulse[5]
				>> vSpareByteVAS
				>> vSetupHistoryContainerTriggerStandardDeviation
				>> vSpareByteVDIF
				>> vSpareDoubleVOIF
				>> vSpareDoubleVEIF
				>> vSpareByteVDD
				>> vSpareByteVDC
				>> vSpareByteVDT

				>> vSpareByte
				>> vSpareFloat1
				>> vSpareFloat2
				>> vSpareByteVSHMB
				>> vSpareFloatVSHMBA
				>> vSpareFloatVSHMBSD
				>> vSpareByteVSHHT
				>> vBottomLocationLeftPixel
				>> vBottomLocationLengthPixel
				>> vSpareDWordVSHSC
				>> vSamplingEjectorVariableToReadInOldByte[2]
				>> vTooManyRejectsAlarmX[0]
				>> vTooManyRejectsAlarmY[0]
				>> vTooManyRejectsAlarmDisableEjectors[0]
				>> vTooManyRejectsAlarmX[1]
				>> vTooManyRejectsAlarmY[1]
				>> vTooManyRejectsAlarmDisableEjectors[1]
				>> vSpareByteVSHMG
				>> vSpareFloatVSHMGA
				>> vSpareFloatVSHMGSD
				>> vOverScanFactor
				>> vNotes
				>> vInspectionAlarmSettings[0]
				>> vInspectionAlarmSettings[1]
				>> vSpareDWordVSMGC
				>> vSpareDWordVSMBC
				>> vSpareDWordVSHAC
				>> vSpareDWordVSHSC
				>> vHeadOffsetFromIndexPulse[7]
				>> vSpareByteVSHAT1
				>> vSpareFloatSHDATU
				>> vSpareFloatSHDATL
				>> vNumberOfInspections
				>> vSetupHistoryAverageDensity
				>> vSpareElectronicOffset
				>> vDriftDensityInspection
				>> vMaxConveyorSpeedAsEntered
				>> vTypesOfRejectsToView
				>> vNumberOfAuxiliaryDetectors
				>> vHistogramThreshold
				>> vHistogramLowerThreshold
				>> vHistogramDilateTimes
				>> vProductCodeSelectionCString
				>> vSpareCString1
				>> vSpareCString2
				>> vSpareCString3
				>> vSpareCString4
				>> vSpareCString5;

			if (TempHaveAFillerMonitorInspection == 0xAA)
				TempArchive >> vFillerMonitorInspection;

			int TempLoop = 0;
			for (; TempLoop < cNumberOfXRayImageCriteriaInArchive; TempLoop++)
			{
				TempArchive >> vSpareByteXRCE[TempLoop]
				>> vSpareByteXRCE1[TempLoop]
				>> vSpareFloatXRCP[TempLoop]
				>> vSpareFloatXRCP2[TempLoop];
			}
			
			for (TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
			{
				TempArchive >> vExternalDetectorEnable[TempLoop]
					>> vExternalDetectorWindowStart[TempLoop]
					>> vExternalDetectorWindowEnd[TempLoop]
					>> vExternalDetectorMode[TempLoop]
					>> vExternalDetectorEjectorVariableToReadInOldByte[TempLoop];
			}
			for (TempLoop = 0; TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
			{
				TempArchive >> vEjectorDwellPosition[TempLoop]
					>> vEjectorDwellTime[TempLoop]
					>> vEjectorDelayPosition[TempLoop]
					>> vEjectorResponseTime[TempLoop];
			}
			BYTE TempGoodInspectionCount = 0;
			for (TempLoop = 0; TempLoop < vNumberOfInspections; TempLoop++)
			{
				TempArchive >> vInspection[TempGoodInspectionCount];

				//must convert Under Weight and Over Weight from density to CheckWeigh type
				if (vInspection[TempGoodInspectionCount])
				{
					if ((vInspection[TempGoodInspectionCount]->vInspectionType == cUnderfillByWeightInspection) || (vInspection[TempGoodInspectionCount]->vInspectionType == cOverfillByWeightInspection))
					{
						CInspectionCheckWeigh *TempObjectPointer = new CInspectionCheckWeigh;
						CRuntimeClass *TempCheckWeighInspectionRunTimeClass = TempObjectPointer->GetRuntimeClass();
						if (!vInspection[TempGoodInspectionCount]->IsKindOf(TempCheckWeighInspectionRunTimeClass))
						{
							TempObjectPointer->CopyDensityInspection((CInspectionDensity * )vInspection[TempLoop], vOverScanMultiplier);
							delete vInspection[TempGoodInspectionCount];
							vInspection[TempGoodInspectionCount] = (CInspectionCheckWeigh *)TempObjectPointer;
						}
						else
							delete TempObjectPointer;
					}
					TempGoodInspectionCount++;  //got a valid inspection, so increase count
				}
				else
					ReportErrorMessage("Invalid Inspection Reading Stream, Product: " + vProductName + ", Inspection: " + dtoa(TempLoop + 1, 0), cEMailInspx, 32000);
			}
			vNumberOfInspections = TempGoodInspectionCount;

			for (TempLoop = 0;TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
				TempArchive >> vSpareDACDetectorOffset[TempLoop];
		} //end version 150 to 152
		else
		if (TempVersion > 147)
		{  // version 148 and 149
			TempArchive >> vProductName 
				>> vHistoryTookAverageDate
				>> vProductImageWidth 
				>> vProductLockOutWidth
				>> vProductBodyTriggerToImageBeltPositionOffset
				>> vProductImageHeightTop
				>> vProductImageHeightBottom
				>> vEndOfLineTimeOut
				>> vXRaySourceVoltage
				>> vXRaySourceCurrent
				>> vXRayIntegrationTimeByte
				>> vSpareByteXRSE
				>> vBodyTrigger
				>> vThresholdForContainerBounds
				>> vSpareByteCanImageBottomOfContainer
				>> vSamplingEjectorVariableToReadInOldByte[0]
				>> vFillerTolerance
				>> vSpareDoubleCB
				>> vSpareDoubleCL
				>> vSpareDoubleCR
				>> vBackupBodyTrigger
				>> vReferenceTop
				>> vReferenceLeft
				>> vReferenceRight
				>> vXRayIntegrationTime
				>> vUsingXScanPoint4mmDetectors
				>> vBulkProductMode
				>> vNumberOfInspectionsRequiredToEject
				>> vSpareDoubleWasStructureAdjustFactor
				>> vMultiLaneEjectAdjacentLanes
				>> vSpareByteDT
				>> vSpareDoubleWasStructureAdjustSubtractiveFactor
				>> vStructureROIAverage
				>> vMultiLaneNumberOfLanes
				>> vMultiLaneNumberOfLinesBeforeContainer
				>> vSamplingEjectorVariableToReadInOldByte[1]
				>> vTempScreenShotDirectoryName
				>> vHaveManuallySetRetriggerLockout
				>> vReferenceROIsToEdges
				>> vAddToLearnCount
				>> vMaximumImageOverlap
				>> vNeedToRelearn
				>> vEdgeLocationBottom
				>> vEdgeLocationBottomPixel
				>> vEdgeLocationHeight
				>> vEdgeLocationHeightPixel
				>> vSpareByteSHMG
				>> vBottomLocationLeft
				>> vBottomLocationLength
				>> vSpareByteSHMB
				>> TempHaveAFillerMonitorInspection
				>> vSpareFloatSHMBSD
				>> vSetupHistoryHistogramThreshold
				>> vSetupHistoryAverageCount
				>> vSetupHistoryStructureCount

				>> vHeadOffsetFromIndexPulse[6]
				>> vSpareStringHDN
				>> vSpareStringLDN
				>> vDriftROITop
				>> vDriftROIBottom
				>> vDriftROILeft
				>> vDriftROIRight
				>> vSpareFloatDTU
				>> vSpareFloatDTL
				>> vSpareDWordDAC
				>> vReferenceBottom
				>> vHeadOffsetFromIndexPulse[6]
				>> vSpareFloatDMV
				>> vSpareFloatDMiV
				>> vSpareFloatDAV
				>> vSpareFloatDSD

				>> vHeadOffsetFromIndexPulse[3]
				>> vSpareStringVN
				>> vStructureROIStandardDievation
				>> vSpareDoubleVT1
				>> vSpareDoubleVL
				>> vSpareDoubleVR
				>> vHeadOffsetFromIndexPulse[2]
				>> vSpareWordVS
				>> vHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vHeadOffsetFromIndexPulse[4]
				>> vHeadOffsetFromIndexPulse[0]
				>> vHeadOffsetFromIndexPulse[5]
				>> vSpareByteVAS
				>> vSetupHistoryContainerTriggerStandardDeviation
				>> vSpareByteVDIF
				>> vSpareDoubleVOIF
				>> vSpareDoubleVEIF
				>> vSpareByteVDD
				>> vSpareByteVDC
				>> vSpareByteVDT

				>> vSpareByte
				>> vSpareFloat1
				>> vSpareFloat2
				>> vSpareByteVSHMB
				>> vSpareFloatVSHMBA
				>> vSpareFloatVSHMBSD
				>> vSpareByteVSHHT
				>> vBottomLocationLeftPixel
				>> vBottomLocationLengthPixel
				>> vSpareDWordVSHSC
				>> vSamplingEjectorVariableToReadInOldByte[2]
				>> vTooManyRejectsAlarmX[0]
				>> vTooManyRejectsAlarmY[0]
				>> vTooManyRejectsAlarmDisableEjectors[0]
				>> vTooManyRejectsAlarmX[1]
				>> vTooManyRejectsAlarmY[1]
				>> vTooManyRejectsAlarmDisableEjectors[1]
				>> vSpareByteVSHMG
				>> vSpareFloatVSHMGA
				>> vSpareFloatVSHMGSD
				>> vOverScanFactor
				>> vNotes
				>> vInspectionAlarmSettings[0]
				>> vInspectionAlarmSettings[1]
				>> vSpareDWordVSMGC
				>> vSpareDWordVSMBC
				>> vSpareDWordVSHAC
				>> vSpareDWordVSHSC
				>> vHeadOffsetFromIndexPulse[7]
				>> vSpareByteVSHAT1
				>> vSpareFloatSHDATU
				>> vSpareFloatSHDATL
				>> vNumberOfInspections
				>> vSetupHistoryAverageDensity
				>> vSpareElectronicOffset
				>> vDriftDensityInspection
				>> vMaxConveyorSpeedAsEntered
				>> vTypesOfRejectsToView
				>> vNumberOfAuxiliaryDetectors
				>> vHistogramThreshold
				>> vHistogramLowerThreshold
				>> vHistogramDilateTimes
				>> vProductCodeSelectionCString
				>> vSpareCString2
				>> vSpareCString3
				>> vSpareCString4;

			int TempLoop = 0;
			for (; TempLoop < cNumberOfXRayImageCriteriaInArchive; TempLoop++)
			{
				TempArchive >> vSpareByteXRCE[TempLoop]
				>> vSpareByteXRCE1[TempLoop]
				>> vSpareFloatXRCP[TempLoop]
				>> vSpareFloatXRCP2[TempLoop];
			}
			
			for (TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
			{
				TempArchive >> vExternalDetectorEnable[TempLoop]
					>> vExternalDetectorWindowStart[TempLoop]
					>> vExternalDetectorWindowEnd[TempLoop]
					>> vExternalDetectorMode[TempLoop]
					>> vExternalDetectorEjectorVariableToReadInOldByte[TempLoop];
			}
			for (TempLoop = 0; TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
			{
				TempArchive >> vEjectorDwellPosition[TempLoop]
					>> vEjectorDwellTime[TempLoop]
					>> vEjectorDelayPosition[TempLoop]
					>> vEjectorResponseTime[TempLoop];
			}
			BYTE TempGoodInspectionCount = 0;
			for (TempLoop = 0; TempLoop < vNumberOfInspections; TempLoop++)
			{
				TempArchive >> vInspection[TempGoodInspectionCount];

				//must convert Under Weight and Over Weight from density to CheckWeigh type
				if (vInspection[TempGoodInspectionCount])
				{
					if ((vInspection[TempGoodInspectionCount]->vInspectionType == cUnderfillByWeightInspection) || (vInspection[TempGoodInspectionCount]->vInspectionType == cOverfillByWeightInspection))
					{
						CInspectionCheckWeigh *TempObjectPointer = new CInspectionCheckWeigh;
						CRuntimeClass *TempCheckWeighInspectionRunTimeClass = TempObjectPointer->GetRuntimeClass();
						if (!vInspection[TempGoodInspectionCount]->IsKindOf(TempCheckWeighInspectionRunTimeClass))
						{
							TempObjectPointer->CopyDensityInspection((CInspectionDensity * )vInspection[TempLoop], vOverScanMultiplier);
							delete vInspection[TempGoodInspectionCount];
							vInspection[TempGoodInspectionCount] = (CInspectionCheckWeigh *)TempObjectPointer;
						}
						else
							delete TempObjectPointer;
					}
					TempGoodInspectionCount++;  //got a valid inspection, so increase count
				}
				else
					ReportErrorMessage("Invalid Inspection Reading Stream, Product: " + vProductName + ", Inspection: " + dtoa(TempLoop + 1, 0), cEMailInspx, 32000);
			}
			vNumberOfInspections = TempGoodInspectionCount;
			for (TempLoop = 0;TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
				TempArchive >> vSpareDACDetectorOffset[TempLoop];
		} //end version 148 and 149
		else
		if (TempVersion > 130)
		{  // version 131 - 247
			TempArchive >> vProductName 
				>> vHistoryTookAverageDate
				>> vProductImageWidth 
				>> vProductLockOutWidth
				>> vProductBodyTriggerToImageBeltPositionOffset
				>> vProductImageHeightTop
				>> vProductImageHeightBottom
				>> vEndOfLineTimeOut
				>> vXRaySourceVoltage
				>> vXRaySourceCurrent
				>> vXRayIntegrationTimeByte
				>> vSpareByteXRSE
				>> vBodyTrigger
				>> vThresholdForContainerBounds
				>> vSpareByteCanImageBottomOfContainer
				>> vSamplingEjectorVariableToReadInOldByte[0]
				>> vFillerTolerance
				>> vSpareDoubleCB
				>> vSpareDoubleCL
				>> vSpareDoubleCR
				>> vBackupBodyTrigger
				>> vReferenceTop
				>> vReferenceLeft
				>> vReferenceRight
				>> vXRayIntegrationTime
				>> vUsingXScanPoint4mmDetectors
				>> vBulkProductMode
				>> vNumberOfInspectionsRequiredToEject
				>> vSpareDoubleWasStructureAdjustFactor
				>> vMultiLaneEjectAdjacentLanes
				>> vSpareByteDT
				>> vSpareDoubleWasStructureAdjustSubtractiveFactor
				>> vStructureROIAverage
				>> vMultiLaneNumberOfLanes
				>> vMultiLaneNumberOfLinesBeforeContainer
				>> vSamplingEjectorVariableToReadInOldByte[1]
				>> vTempScreenShotDirectoryName
				>> vHaveManuallySetRetriggerLockout
				>> vReferenceROIsToEdges
				>> vAddToLearnCount
				>> vMaximumImageOverlap
				>> vNeedToRelearn
				>> vEdgeLocationBottom
				>> vEdgeLocationBottomPixel
				>> vEdgeLocationHeight
				>> vEdgeLocationHeightPixel
				>> vSpareByteSHMG
				>> vBottomLocationLeft
				>> vBottomLocationLength
				>> vSpareByteSHMB
				>> TempHaveAFillerMonitorInspection
				>> vSpareFloatSHMBSD
				>> vSetupHistoryHistogramThreshold
				>> vSetupHistoryAverageCount
				>> vSetupHistoryStructureCount

				>> vHeadOffsetFromIndexPulse[6]
				>> vSpareStringHDN
				>> vSpareStringLDN
				>> vDriftROITop
				>> vDriftROIBottom
				>> vDriftROILeft
				>> vDriftROIRight
				>> vSpareFloatDTU
				>> vSpareFloatDTL
				>> vSpareDWordDAC
				>> vReferenceBottom
				>> vHeadOffsetFromIndexPulse[6]
				>> vSpareFloatDMV
				>> vSpareFloatDMiV
				>> vSpareFloatDAV
				>> vSpareFloatDSD

				>> vHeadOffsetFromIndexPulse[3]
				>> vSpareStringVN
				>> vStructureROIStandardDievation
				>> vSpareDoubleVT1
				>> vSpareDoubleVL
				>> vSpareDoubleVR
				>> vHeadOffsetFromIndexPulse[2]
				>> vSpareWordVS
				>> vHeadOffsetFromIndexPulse[3]
				>> vHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vHeadOffsetFromIndexPulse[4]
				>> vHeadOffsetFromIndexPulse[0]
				>> vHeadOffsetFromIndexPulse[5]
				>> vSetupHistoryContainerTriggerStandardDeviation
				>> vSpareByteVDIF
				>> vSpareDoubleVOIF
				>> vSpareDoubleVEIF
				>> vSpareByteVDD
				>> vSpareByteVDC
				>> vSpareByteVDT

				>> vSpareByte
				>> vSpareFloat1
				>> vSpareFloat2
				>> vSpareByteVSHMB
				>> vSpareFloatVSHMBA
				>> vSpareFloatVSHMBSD
				>> vSpareByteVSHHT
				>> vBottomLocationLeftPixel
				>> vBottomLocationLengthPixel
				>> vSpareDWordVSHSC
				>> vSamplingEjectorVariableToReadInOldByte[2]
				>> vTooManyRejectsAlarmX[0]
				>> vTooManyRejectsAlarmY[0]
				>> vTooManyRejectsAlarmDisableEjectors[0]
				>> vTooManyRejectsAlarmX[1]
				>> vTooManyRejectsAlarmY[1]
				>> vTooManyRejectsAlarmDisableEjectors[1]
				>> vSpareByteVSHMG
				>> vSpareFloatVSHMGA
				>> vSpareFloatVSHMGSD
				>> vOverScanFactor
				>> vNotes
				>> vInspectionAlarmSettings[0]
				>> vInspectionAlarmSettings[1]
				>> vSpareDWordVSMGC
				>> vSpareDWordVSMBC
				>> vSpareDWordVSHAC
				>> vSpareDWordVSHSC
				>> vHeadOffsetFromIndexPulse[7]
				>> vSpareByteVSHAT1
				>> vSpareFloatSHDATU
				>> vSpareFloatSHDATL
				>> vNumberOfInspections
				>> vSetupHistoryAverageDensity
				>> vSpareElectronicOffset
				>> vDriftDensityInspection
				>> vMaxConveyorSpeedAsEntered
				>> vTypesOfRejectsToView
				>> vNumberOfAuxiliaryDetectors
				>> vHistogramThreshold
				>> vHistogramLowerThreshold
				>> vHistogramDilateTimes;

			int TempLoop = 0;
			for (; TempLoop < cNumberOfXRayImageCriteriaInArchive; TempLoop++)
			{
				TempArchive >> vSpareByteXRCE[TempLoop]
				>> vSpareByteXRCE1[TempLoop]
				>> vSpareFloatXRCP[TempLoop]
				>> vSpareFloatXRCP2[TempLoop];
			}
			
			for (TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
			{
				TempArchive >> vExternalDetectorEnable[TempLoop]
					>> vExternalDetectorWindowStart[TempLoop]
					>> vExternalDetectorWindowEnd[TempLoop]
					>> vExternalDetectorMode[TempLoop]
					>> vExternalDetectorEjectorVariableToReadInOldByte[TempLoop];
			}
			for (TempLoop = 0; TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
			{
				TempArchive >> vEjectorDwellPosition[TempLoop]
					>> vEjectorDwellTime[TempLoop]
					>> vEjectorDelayPosition[TempLoop]
					>> vEjectorResponseTime[TempLoop];
			}
			BYTE TempGoodInspectionCount = 0;
			for (TempLoop = 0; TempLoop < vNumberOfInspections; TempLoop++)
			{
				TempArchive >> vInspection[TempGoodInspectionCount];

				//must convert Under Weight and Over Weight from density to CheckWeigh type
				if (vInspection[TempGoodInspectionCount])
				{
					if ((vInspection[TempGoodInspectionCount]->vInspectionType == cUnderfillByWeightInspection) || (vInspection[TempGoodInspectionCount]->vInspectionType == cOverfillByWeightInspection))
					{
						CInspectionCheckWeigh *TempObjectPointer = new CInspectionCheckWeigh;
						CRuntimeClass *TempCheckWeighInspectionRunTimeClass = TempObjectPointer->GetRuntimeClass();
						if (!vInspection[TempGoodInspectionCount]->IsKindOf(TempCheckWeighInspectionRunTimeClass))
						{
							TempObjectPointer->CopyDensityInspection((CInspectionDensity * )vInspection[TempLoop], vOverScanMultiplier);
							delete vInspection[TempGoodInspectionCount];
							vInspection[TempGoodInspectionCount] = (CInspectionCheckWeigh *)TempObjectPointer;
						}
						else
							delete TempObjectPointer;
					}
					TempGoodInspectionCount++;  //got a valid inspection, so increase count
				}
				else
					ReportErrorMessage("Invalid Inspection Reading Stream, Product: " + vProductName + ", Inspection: " + dtoa(TempLoop + 1, 0), cEMailInspx, 32000);
			}
			vNumberOfInspections = TempGoodInspectionCount;
			for (TempLoop = 0;TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
				TempArchive >> vSpareDACDetectorOffset[TempLoop];
		} //end version 131 - 147
		else
		if ((TempVersion > 123) && (TempVersion < 131))
		{  // version 124 - 130
			TempArchive >> vProductName 
				>> vHistoryTookAverageDate
				>> vProductImageWidth 
				>> vProductLockOutWidth
				>> vProductBodyTriggerToImageBeltPositionOffset
				>> vProductImageHeightTop
				>> vProductImageHeightBottom
				>> vEndOfLineTimeOut
				>> vXRaySourceVoltage
				>> vXRaySourceCurrent
				>> vXRayIntegrationTimeByte
				>> vSpareByteXRSE
				>> vBodyTrigger
				>> vThresholdForContainerBounds
				>> vSpareByteCanImageBottomOfContainer
				>> vSamplingEjectorVariableToReadInOldByte[0]
				>> vFillerTolerance
				>> vSpareDoubleCB
				>> vSpareDoubleCL
				>> vSpareDoubleCR
				>> vBackupBodyTrigger
				>> vReferenceTop
				>> vReferenceLeft
				>> vReferenceRight
				>> vXRayIntegrationTime
				>> vUsingXScanPoint4mmDetectors
				>> vBulkProductMode
				>> vNumberOfInspectionsRequiredToEject
				>> vSpareDoubleWasStructureAdjustFactor
				>> vMultiLaneEjectAdjacentLanes
				>> vSpareByteDT
				>> vSpareDoubleWasStructureAdjustSubtractiveFactor
				>> vStructureROIAverage
				>> vMultiLaneNumberOfLanes
				>> vMultiLaneNumberOfLinesBeforeContainer
				>> vSamplingEjectorVariableToReadInOldByte[1]
				>> vTempScreenShotDirectoryName
				>> vHaveManuallySetRetriggerLockout
				>> vReferenceROIsToEdges
				>> vAddToLearnCount
				>> vMaximumImageOverlap
				>> vNeedToRelearn
				>> vEdgeLocationBottom
				>> vEdgeLocationBottomPixel
				>> vEdgeLocationHeight
				>> vEdgeLocationHeightPixel
				>> vSpareByteSHMG
				>> vBottomLocationLeft
				>> vBottomLocationLength
				>> vSpareByteSHMB
				>> TempHaveAFillerMonitorInspection
				>> vSpareFloatSHMBSD
				>> vSetupHistoryHistogramThreshold
				>> vSetupHistoryAverageCount
				>> vSetupHistoryStructureCount

				>> vHeadOffsetFromIndexPulse[6]
				>> vSpareStringHDN
				>> vSpareStringLDN
				>> vDriftROITop
				>> vDriftROIBottom
				>> vDriftROILeft
				>> vDriftROIRight
				>> vSpareFloatDTU
				>> vSpareFloatDTL
				>> vSpareDWordDAC
				>> vReferenceBottom
				>> vHeadOffsetFromIndexPulse[6]
				>> vSpareFloatDMV
				>> vSpareFloatDMiV
				>> vSpareFloatDAV
				>> vSpareFloatDSD

				>> vHeadOffsetFromIndexPulse[3]
				>> vSpareStringVN
				>> vStructureROIStandardDievation
				>> vSpareDoubleVT1
				>> vSpareDoubleVL
				>> vSpareDoubleVR
				>> vHeadOffsetFromIndexPulse[2]
				>> vSpareWordVS
				>> vHeadOffsetFromIndexPulse[cFillerSamplingType]
				>> vHeadOffsetFromIndexPulse[4]
				>> vHeadOffsetFromIndexPulse[0]
				>> vHeadOffsetFromIndexPulse[5]
				>> vSpareByteVAS
				>> vSetupHistoryContainerTriggerStandardDeviation
				>> vSpareByteVDIF
				>> vSpareDoubleVOIF
				>> vSpareDoubleVEIF
				>> vSpareByteVDD
				>> vSpareByteVDC
				>> vSpareByteVDT

				>> vSpareByte
				>> vSpareFloat1
				>> vSpareFloat2
				>> vSpareByteVSHMB
				>> vSpareFloatVSHMBA
				>> vSpareFloatVSHMBSD
				>> vSpareByteVSHHT
				>> vBottomLocationLeftPixel
				>> vBottomLocationLengthPixel
				>> vSpareDWordVSHSC
				>> vSamplingEjectorVariableToReadInOldByte[2]
				>> vTooManyRejectsAlarmX[0]
				>> vTooManyRejectsAlarmY[0]
				>> vTooManyRejectsAlarmDisableEjectors[0]
				>> vTooManyRejectsAlarmX[1]
				>> vTooManyRejectsAlarmY[1]
				>> vTooManyRejectsAlarmDisableEjectors[1]
				>> vSpareByteVSHMG
				>> vSpareFloatVSHMGA
				>> vSpareFloatVSHMGSD
				>> vOverScanFactor
				>> vNotes
				>> vInspectionAlarmSettings[0]
				>> vInspectionAlarmSettings[1]
				>> vSpareDWordVSMGC
				>> vSpareDWordVSMBC
				>> vSpareDWordVSHAC
				>> vSpareDWordVSHSC
				>> vHeadOffsetFromIndexPulse[7]
				>> vSpareByteVSHAT1
				>> vSpareFloatSHDATU
				>> vSpareFloatSHDATL
				>> vNumberOfInspections
				>> vSetupHistoryAverageDensity
				>> vSpareElectronicOffset
				>> vDriftDensityInspection
				>> vMaxConveyorSpeedAsEntered
				>> vTypesOfRejectsToView
				>> vNumberOfAuxiliaryDetectors;

			int TempLoop = 0;
			for (; TempLoop < cNumberOfXRayImageCriteriaInArchive; TempLoop++)
			{
				TempArchive >> vSpareByteXRCE[TempLoop]
				>> vSpareByteXRCE1[TempLoop]
				>> vSpareFloatXRCP[TempLoop]
				>> vSpareFloatXRCP2[TempLoop];
			}
			
			for (TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
			{
				TempArchive >> vExternalDetectorEnable[TempLoop]
					>> vExternalDetectorWindowStart[TempLoop]
					>> vExternalDetectorWindowEnd[TempLoop]
					>> vExternalDetectorMode[TempLoop]
					>> vExternalDetectorEjectorVariableToReadInOldByte[TempLoop];
			}
			for (TempLoop = 0; TempLoop < cOldNumberOfEjectorsInStream3; TempLoop++)
			{
				TempArchive >> vEjectorDwellPosition[TempLoop]
					>> vEjectorDwellTime[TempLoop]
					>> vEjectorDelayPosition[TempLoop]
					>> vEjectorResponseTime[TempLoop];
			}
			BYTE TempGoodInspectionCount = 0;
			for (TempLoop = 0; TempLoop < vNumberOfInspections; TempLoop++)
			{
				TempArchive >> vInspection[TempGoodInspectionCount];

				//must convert Under Weight and Over Weight from density to CheckWeigh type
				if (vInspection[TempGoodInspectionCount])
				{
					if ((vInspection[TempGoodInspectionCount]->vInspectionType == cUnderfillByWeightInspection) || (vInspection[TempGoodInspectionCount]->vInspectionType == cOverfillByWeightInspection))
					{
						CInspectionCheckWeigh *TempObjectPointer = new CInspectionCheckWeigh;
						CRuntimeClass *TempCheckWeighInspectionRunTimeClass = TempObjectPointer->GetRuntimeClass();
						if (!vInspection[TempGoodInspectionCount]->IsKindOf(TempCheckWeighInspectionRunTimeClass))
						{
							TempObjectPointer->CopyDensityInspection((CInspectionDensity * )vInspection[TempLoop], vOverScanMultiplier);
							delete vInspection[TempGoodInspectionCount];
							vInspection[TempGoodInspectionCount] = (CInspectionCheckWeigh *)TempObjectPointer;
						}
						else
							delete TempObjectPointer;
					}
					TempGoodInspectionCount++;  //got a valid inspection, so increase count
				}
				else
					ReportErrorMessage("Invalid Inspection Reading Stream, Product: " + vProductName + ", Inspection: " + dtoa(TempLoop + 1, 0), cEMailInspx, 32000);
			}
			vNumberOfInspections = TempGoodInspectionCount;
			for (TempLoop = 0;TempLoop < cMaximumNumberOfDetectors8; TempLoop++)
				TempArchive >> vSpareDACDetectorOffset[TempLoop];
		} //end version 124 - 130
		//net to set all items that have a Conveyor position word so it will be initialized

		if (TempVersion < 157)
		{
			for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
				vSamplingEjector[TempLoop] = vSamplingEjectorVariableToReadInOldByte[TempLoop]; //Changed to DWORD in version 157, so if reading 156 or lower, copy BYTE var to DWORD var
		
			for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
				vExternalDetectorEjector[TempLoop] = vExternalDetectorEjectorVariableToReadInOldByte[TempLoop]; //Changed to DWORD in version 157, so if reading 156 or lower, copy BYTE var to DWORD var

			for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
			if (vEjectorResponseTime[TempLoop] == 0)
			{
				vEjectorDwellTime[TempLoop] = 35;
				vEjectorResponseTime[TempLoop] = (float)0.004;
				SetEjectorBeltPositionOffset(TempLoop, 64);  //in inches or mm
			}
		}

		if (TempVersion < 153)
			vMultiLaneNumberOfLanes = 0; //Added during version 153, so if reading 152 or lower, make zero lanes as it was not supported then

		if (vMultiLaneNumberOfLanes > cMaximumNumberForMultiLane)
			vMultiLaneNumberOfLanes = 0; 

		SetOverScanFactor(vOverScanFactor);

		if ((vBodyTrigger == 0) || (vBodyTrigger > cNumberOfBodyTriggers))
			vBodyTrigger = 1;

		if (!vReferenceWidth)
		if (vReferenceRight > vReferenceLeft)
			vReferenceWidth = vReferenceRight - vReferenceLeft;

		CheckInspectionsAreValid();
		if (TempVersion < 137)
			vSpareDoubleWasStructureAdjustFactor = 0;
		if (vSpareDoubleWasStructureAdjustFactor == 1.00)
			vSpareDoubleWasStructureAdjustFactor = 0;

		if (vXRaySourceVoltage > 11.66)
			vXRaySourceVoltage = (float)11.66;

		for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
		{
			if ((vEjectorResponseTime[TempLoop] > .5) || (vEjectorResponseTime[TempLoop] < 0))
				vEjectorResponseTime[TempLoop] = (float).004;
		}

		if (vUsingXScanPoint4mmDetectors > 1)
			vUsingXScanPoint4mmDetectors = 0;

		if ((vGlobalScanTracType == cForteScanTrac) || (vGlobalScanTracType == cCaseInspectorScanTrac))
			vUsingXScanPoint4mmDetectors = 0;

		vSpareDoubleWasStructureAdjustSubtractiveFactor = 0;
		vSpareDoubleWasStructureAdjustFactor = 0;
		if ((vNumberOfInspectionsRequiredToEject == 0) || (vNumberOfInspectionsRequiredToEject > vNumberOfInspections))
			vNumberOfInspectionsRequiredToEject = 1;

		vTempScreenShotDirectoryName = " ";
		vSpareByteCanImageBottomOfContainer = 0;
		if (vBulkProductMode > 1)
			vBulkProductMode = 0;

		vSpareByteXRSE = 0;
		vSpareDoubleCB = 0;
		vSpareDoubleCL = 0;
		vSpareDoubleCR = 0;
		vSpareByteDT = 0;
		vSpareByteSHMG = 0;
		vSpareByteSHMB = 0;
		vSpareFloatSHMBSD = 0;
//		vHeadOffsetFromIndexPulse[6] = 0;
		vSpareStringHDN = "";
		vSpareStringLDN = "";
		vSpareFloatDTU = 0;
		vSpareFloatDTL = 0;
		vSpareDWordDAC = 0;
//		vHeadOffsetFromIndexPulse[6] = 0;
		vSpareFloatDMiV = 0;
		vSpareFloatDAV = 0;
		vSpareFloatDSD = 0;
		//vHeadOffsetFromIndexPulse[cFillerSamplingType] = 0;
		vSpareStringVN = "";
		vSpareDoubleVT1 = 0;
		vSpareDoubleVL = 0;
		vSpareDoubleVR = 0;
//		vHeadOffsetFromIndexPulse[2] = 0;
		vSpareWordVS = 0;
		//vHeadOffsetFromIndexPulse[cFillerSamplingType] = 0;
		//vHeadOffsetFromIndexPulse[cSeamerSamplingType] = 0;
		//vHeadOffsetFromIndexPulse[0] = 0;
		//vHeadOffsetFromIndexPulse[3] = 0;
		vSpareByteVAS = 0;
		vSpareByteVDIF = 0;
		vSpareDoubleVOIF = 0;
		vSpareDoubleVEIF = 0;
		vSpareByteVDD = 0;
		vSpareByteVDC = 0;
		vSpareByteVDT = 0;
		vSpareByte = 0;
		vSpareFloat1 = 0;
		vSpareFloat2 = 0;
		vSpareByteVSHMB = 0;
		vSpareFloatVSHMBA = 0;
		vSpareFloatVSHMBSD = 0;
		vSpareByteVSHHT = 0;
		vSpareDWordVSHAC = 0;
		vSpareDWordVSHSC = 0;
		vSpareByteVSHMG = 0;
		vSpareFloatVSHMGA = 0;
		vSpareFloatVSHMGSD = 0;
		vSpareDWordVSMGC = 0;
		vSpareDWordVSMBC = 0;
//		vHeadOffsetFromIndexPulse[7] = 0;
		vSpareByteVSHAT1 = 0;
		vSpareFloatSHDATU = 0;
		vSpareFloatSHDATL = 0;
		for (BYTE TempLoop = 0; TempLoop < cNumberOfXRayImageCriteriaInArchive; TempLoop++)
		{
			vSpareByteXRCE[TempLoop] = 0;
			vSpareByteXRCE1[TempLoop] = 0;
			vSpareFloatXRCP[TempLoop] = 0;
			vSpareFloatXRCP2[TempLoop] = 0;
		}

		if ((vProductLockOutWidth > cMaximumImageWidth * 5) || (vProductLockOutWidth < 0))
		{
			vProductLockOutWidth = 5;
			ReportErrorMessage("Bad float in stream corrected: vProductLockOutWidth", cEMailInspx, 32000);
		}
		if ((vProductBodyTriggerToImageBeltPositionOffset > cMaximumImageWidth * 5) || (vProductBodyTriggerToImageBeltPositionOffset < 0))
		{
			vProductBodyTriggerToImageBeltPositionOffset = 5;
			ReportErrorMessage("Bad float in stream corrected: vProductBodyTriggerToImageBeltPositionOffset", cEMailInspx, 32000);
		}
		if ((vProductImageHeightTop > 36) || (vProductImageHeightTop < 0))
		{
			vProductImageHeightTop = 12;
			ReportErrorMessage("Bad float in stream corrected: vProductImageHeightTop", cEMailInspx, 32000);
		}
		if (vTooManyRejectsAlarmX[0] > 20000)
		{
			ReportErrorMessage("Bad word in stream corrected: vTooManyRejectsAlarmX[0]", cEMailInspx, 32000);
			vTooManyRejectsAlarmX[0] = 0;
		}

		if (vTooManyRejectsAlarmY[0] > 20000)
		{
			ReportErrorMessage("Bad word in stream corrected: vTooManyRejectsAlarmY[0]", cEMailInspx, 32000);
			vTooManyRejectsAlarmY[0] = 0;
		}

		if (vTooManyRejectsAlarmX[1] > 20000)
		{
			ReportErrorMessage("Bad word in stream corrected: vTooManyRejectsAlarmX[1]", cEMailInspx, 32000);
			vTooManyRejectsAlarmX[1] = 0;
		}

		if (vTooManyRejectsAlarmY[1] > 20000)
		{
			ReportErrorMessage("Bad word in stream corrected: vTooManyRejectsAlarmY[1]", cEMailInspx, 32000);
			vTooManyRejectsAlarmY[1] = 0;
		}

		if ((vProductImageHeightBottom > vProductImageHeightTop) || (vProductImageHeightBottom < 0))
		{
			vProductImageHeightBottom = 0;
			ReportErrorMessage("Bad float in stream corrected: vProductImageHeightBottom", cEMailInspx, 32000);
		}
		if ((vEndOfLineTimeOut > 10000) || (vEndOfLineTimeOut < 0))
		{
			vEndOfLineTimeOut = 100;
			ReportErrorMessage("Bad float in stream corrected: vEndOfLineTimeOut", cEMailInspx, 32000);
		}
		if ((vXRaySourceVoltage > 100) || (vXRaySourceVoltage < 0))
		{
			vXRaySourceVoltage = 10;
			ReportErrorMessage("Bad float in stream corrected: vXRaySourceVoltage", cEMailInspx, 32000);
		}
		if ((vXRaySourceCurrent > 100) || (vXRaySourceCurrent < 0))
		{
			vXRaySourceCurrent = 6;
			ReportErrorMessage("Bad float in stream corrected: vXRaySourceCurrent", cEMailInspx, 32000);
		}
		if ((vEdgeLocationBottom > cMaximumNumberOfDetectors12 * 2) || (vEdgeLocationBottom < 0))
		{
			vEdgeLocationBottom = 6;
			ReportErrorMessage("Bad float in stream corrected: vEdgeLocationBottom", cEMailInspx, 32000);
		}
		if ((vEdgeLocationHeight > cMaximumNumberOfDetectors12 * 2) || (vEdgeLocationHeight < 0))
		{
			vEdgeLocationHeight = 6;
			ReportErrorMessage("Bad float in stream corrected: vEdgeLocationBottom", cEMailInspx, 32000);
		}

		if ((vDriftROITop > 36) || (vDriftROITop < 0))
		{
			vDriftROITop = 5;
			ReportErrorMessage("Bad float in stream corrected: vDriftROITop", cEMailInspx, 32000);
		}
		if ((vDriftROIBottom > vDriftROITop) || (vDriftROIBottom < 0))
		{
			ReportErrorMessage("Bad float in stream " + *GetProductName() + " corrected: vDriftROIBottom was: " + dtoa(vDriftROIBottom,2), cEMailInspx, 32000);
			vDriftROIBottom = 1;
			if (vDriftROITop <= 1)
				vDriftROITop = 1.5;
		}
		if ((vDriftROILeft > cMaximumImageWidth) || (vDriftROILeft < 0))
		{
			ReportErrorMessage("Bad float in stream " + *GetProductName() + " corrected: vDriftROILeft was: " + dtoa(vDriftROILeft,2), cEMailInspx, 32000);
			vDriftROILeft = 0;
		}
		if ((vDriftROIRight > vProductImageWidth) || (vDriftROIRight < 0))
		{
			ReportErrorMessage("Bad float in stream " + *GetProductName() + " corrected: vDriftROIRight was: " + dtoa(vDriftROIRight,2), cEMailInspx, 32000);
			vDriftROIRight = vProductImageWidth;
		}
		if ((vMaximumImageOverlap > vProductImageWidth) || (vMaximumImageOverlap < 0))
		{
			vMaximumImageOverlap = vProductImageWidth;
			ReportErrorMessage("Bad float in stream corrected: vMaximumImageOverlap", cEMailInspx, 32000);
		}
		if ((vStructureROIAverage > 255) || (vStructureROIAverage < 0))
		{
			vStructureROIAverage = 0;
			ReportErrorMessage("Bad float in stream corrected: vStructureROIAverage", cEMailInspx, 32000);
		}
		if ((vStructureROIStandardDievation > 10000) || (vStructureROIStandardDievation < 0))
		{
			vStructureROIStandardDievation = 0;
			ReportErrorMessage("Bad float in stream corrected: vStructureROIStandardDievation", cEMailInspx, 32000);
		}
		if ((vSetupHistoryContainerTriggerStandardDeviation > 10000) || (vSetupHistoryContainerTriggerStandardDeviation < 0))
		{
			vSetupHistoryContainerTriggerStandardDeviation = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryContainerTriggerStandardDeviation", cEMailInspx, 32000);
		}
		if ((vSetupHistoryAverageDensity > 255) || (vSetupHistoryAverageDensity < 0))
		{
			vSetupHistoryAverageDensity = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryAverageDensity", cEMailInspx, 32000);
		}
		if ((vMaxConveyorSpeedAsEntered > 10000) || (vMaxConveyorSpeedAsEntered < 0))
		{
			vMaxConveyorSpeedAsEntered = 0;
			ReportErrorMessage("Bad float in stream corrected: vMaxConveyorSpeedAsEntered", cEMailInspx, 32000);
		}
		
		for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
		{
			if ((vExternalDetectorWindowStart[TempLoop] > 100) || (vExternalDetectorWindowStart[TempLoop] < 0))
			{
				vExternalDetectorWindowStart[TempLoop] = 6;
				ReportErrorMessage("Bad float in stream corrected: vExternalDetectorWindowStart[TempLoop]", cEMailInspx, 32000);
			}
			if ((vExternalDetectorWindowEnd[TempLoop] > 100) || (vExternalDetectorWindowEnd[TempLoop] < 0))
			{
				vExternalDetectorWindowEnd[TempLoop] = 6;
				ReportErrorMessage("Bad float in stream corrected: vExternalDetectorWindowEnd[TempLoop]", cEMailInspx, 32000);
			}
		}
		for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
		{
			//if ((vEjectorDwellPosition[TempLoop] > 10000) || (vEjectorDwellPosition[TempLoop] < 0))
			//{
			//	vEjectorDwellPosition[TempLoop] = 100;
			//	ReportErrorMessage("Bad float in stream corrected: vEjectorDwellPosition[TempLoop]", cEMailInspx, 32000);
			//}
			if ((vEjectorDwellTime[TempLoop] > 10000) || (vEjectorDwellTime[TempLoop] < 0))
			{
				vEjectorDwellTime[TempLoop] = 100;
				ReportErrorMessage("Bad float in stream corrected: vEjectorDwellTime[TempLoop]", cEMailInspx, 32000);
			}
			if ((vEjectorResponseTime[TempLoop] > 10000) || (vEjectorResponseTime[TempLoop] < 0))
			{
				vEjectorResponseTime[TempLoop] = 100;
				ReportErrorMessage("Bad float in stream corrected: vEjectorResponseTime[TempLoop]", cEMailInspx, 32000);
			}
			if ((vEjectorDelayPosition[TempLoop] > 10000) || (vEjectorDelayPosition[TempLoop] < 0))
			{
				vEjectorDelayPosition[TempLoop] = 100;
				ReportErrorMessage("Bad float in stream corrected: vEjectorDelayPosition[TempLoop]", cEMailInspx, 32000);
			}
		}

		if (vInspectionAlarmSettings[0] == 0)
			vInspectionAlarmSettings[0] = ~vInspectionAlarmSettings[0];

		if (vInspectionAlarmSettings[1] == 0)
			vInspectionAlarmSettings[1] = ~vInspectionAlarmSettings[1];
	}  //end reading in configuration data

	for (BYTE TempLoop = 0; TempLoop < 2; TempLoop++)
	{
		if (vTooManyRejectsAlarmX[TempLoop] > cMaximumEjectDataSize)
			vTooManyRejectsAlarmX[TempLoop] = cMaximumEjectDataSize;
		
		if (vTooManyRejectsAlarmY[TempLoop] > cMaximumEjectDataSize)
			vTooManyRejectsAlarmY[TempLoop] = cMaximumEjectDataSize;
	}
}
                                      
CString *CProduct::GetProductName()
{               
  return &vProductName;
}
	

void CProduct::SetProductName(CString TempProductName)
{
	vProductName = TempProductName;
}

CProduct::~CProduct()
{
	for (int TempLoop = 0;TempLoop < cMaximumNumberOfInspections; TempLoop++)
	if (vInspection[TempLoop])
	{
		delete vInspection[TempLoop];
		vInspection[TempLoop] = NULL;
	}

	if (vDriftDensityInspection)
	{
		delete vDriftDensityInspection;
		vDriftDensityInspection = NULL;
	}

	if (vFillerMonitorInspection)
	{
		delete vFillerMonitorInspection;
		vFillerMonitorInspection = NULL;
	}
}

void CProduct::SetEdgeLocationBottom(float TempValue)
{
	vEdgeLocationBottom = TempValue;
	vEdgeLocationBottomPixel = (WORD)((TempValue * vGlobalPixelsPerUnitInHeight) + .5);
}

void CProduct::SetEdgeLocationHeight(float TempValue)
{
	vEdgeLocationHeight = TempValue;
	vEdgeLocationHeightPixel = (WORD)((TempValue * vGlobalPixelsPerUnitInHeight) + .5);
}

void CProduct::SetBottomLocationLeft(float TempValue)
{
	vBottomLocationLeft = TempValue;
	vBottomLocationLeftPixel = (WORD)((TempValue * vGlobalPixelsPerUnit) + .5);
}

void CProduct::SetBottomLocationLength(float TempValue)
{
	vBottomLocationLength = TempValue;
	vBottomLocationLengthPixel = (WORD)((TempValue * vGlobalPixelsPerUnit) + .5);
}

void CProduct::SetEndOfLineTimeOut(float TempValue)
{
	vEndOfLineTimeOut = TempValue;
	vEndOfLineBeltPosition = (WORD)(TempValue * vGlobalPixelsPerUnit * vOverScanMultiplier);
}

void CProduct::SetProductImageWidth(float TempValue)
{
	//float TempMax = (float)(24.0 / vOverScanMultiplier);
	//if (vGlobalUseMetric)
	//	TempMax = (float)(TempMax * 25.4);

	//if (TempValue > TempMax)
	//	TempValue = TempMax;

	vProductImageWidth = TempValue;

	vImageWidthPosition = (WORD)(TempValue * vGlobalPixelsPerUnit * vOverScanMultiplier);
  double TempRoundMargin = .5 / (vGlobalPixelsPerUnit * vOverScanMultiplier);
	for (BYTE TempLoop = 0; TempLoop < 4; TempLoop++)
	{  //width must be divisible by 8
		int TempMod = vImageWidthPosition % 8; //mod 8
		if (TempMod > 4)
			vImageWidthPosition++;
		else
		if (TempMod > 0)
			vImageWidthPosition--;
	}
	if (vGlobalPixelsPerUnit * vOverScanMultiplier)
		vProductImageWidth = (float)(((double)vImageWidthPosition + TempRoundMargin) / (vGlobalPixelsPerUnit * vOverScanMultiplier));
}

void CProduct::SetProductLockOutWidth(float TempValue)
{
	vProductLockOutWidth = TempValue;
	vLockoutPosition = (WORD)(TempValue * vGlobalPixelsPerUnit * vOverScanMultiplier);
}

void CProduct::SetProductBodyTriggerToImageBeltPositionOffset(float TempValue)
{
	if (TempValue < 4)
		TempValue = 4;

	vProductBodyTriggerToImageBeltPositionOffset = TempValue;
	vBTToImagePosition = (WORD)(TempValue * vGlobalPixelsPerUnit * vOverScanMultiplier);
}

void CProduct::SetEjectorBeltPositionOffset(BYTE TempEjectorNumber, float TempValue)
{
	if (TempValue < 14)
		TempValue = 14;

	vEjectorDelayPosition[TempEjectorNumber] = TempValue;
	vEjectorDelayBeltPosition[TempEjectorNumber] = (WORD)(TempValue * vGlobalPixelsPerUnit * vOverScanMultiplier);
}

void CProduct::SetExternalDetectorWindowStart(BYTE TempDetectorNumber, float TempValue)
{
	vExternalDetectorWindowStart[TempDetectorNumber] = TempValue;
	vExternalDetectorWindowStartPosition[TempDetectorNumber] = (WORD)(TempValue * vGlobalPixelsPerUnit * vOverScanMultiplier);
}

void CProduct::SetExternalDetectorWindowEnd(BYTE TempDetectorNumber, float TempValue)
{
	vExternalDetectorWindowEnd[TempDetectorNumber] = TempValue;
	vExternalDetectorWindowEndPosition[TempDetectorNumber] = (WORD)(TempValue * vGlobalPixelsPerUnit * vOverScanMultiplier);
}

void CProduct::CalculateEndOfLineTimeOut()
{
	bool TempEjectorUsed[cNumberOfEjectors];

	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
		TempEjectorUsed[TempLoop] = false;

	for (BYTE TempLoop = 0; TempLoop < vNumberOfInspections; TempLoop++)
	{
		if (vInspection[TempLoop])
		if (vInspection[TempLoop]->vEjector)
		{
			for (BYTE TempEjectorLoop = 0; TempEjectorLoop < cNumberOfEjectors; TempEjectorLoop++)
			if (vInspection[TempLoop]->vEjector & (1 << TempEjectorLoop))
				TempEjectorUsed[TempEjectorLoop] = true;
		}
	}

	for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
	{
		if (vExternalDetectorEnable[TempLoop])
		if (vExternalDetectorMode[TempLoop])
		if (vExternalDetectorEjector[TempLoop])
		{
			for (BYTE TempEjectorLoop = 0; TempEjectorLoop < cNumberOfEjectors; TempEjectorLoop++)
			if (vExternalDetectorEjector[TempLoop] & (1 << TempEjectorLoop))
				TempEjectorUsed[TempEjectorLoop] = true;
		}
	}

	for (BYTE TempLoop = cFillerSamplingType; TempLoop <= cSeamerSamplingType; TempLoop++)
	//if (vLocalConfigurationData->vNumberOfHeadsToMonitor[TempLoop])
	if (vSamplingEjector[TempLoop])
	{
		for (BYTE TempEjectorLoop = 0; TempEjectorLoop < cNumberOfEjectors; TempEjectorLoop++)
		if (vSamplingEjector[TempLoop] & (1 << TempEjectorLoop))
			TempEjectorUsed[TempEjectorLoop] = true;
	}

	BYTE TempNumberOfEjectorsUsed = 0;
	vEndOfLineTimeOut = (float)(60.0 + (float)vGlobalEndOfLineTimeOutMargin);
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
	{
		if (TempEjectorUsed[TempLoop])
		{
			TempNumberOfEjectorsUsed++;
			if (vEndOfLineTimeOut < vEjectorDelayPosition[TempLoop] + vGlobalEndOfLineTimeOutMargin)
			{																														
				//4/1/04 changed extra delay from 60 to 104 = 4 inches after Last ejector
				vEndOfLineTimeOut = vEjectorDelayPosition[TempLoop] + vGlobalEndOfLineTimeOutMargin;
			}
		}
	}

	if (!TempNumberOfEjectorsUsed)
		vEndOfLineTimeOut = (float)((60 * vGlobalPixelsPerUnit * vOverScanMultiplier) / (32 * vOverScanMultiplier)); //sets to 60 inches by scaling by scale factor
	SetEndOfLineTimeOut(vEndOfLineTimeOut);
}

void CProduct::ClearInspectionHistory()
{
	//have removed or changed the Image Learned data or change product size
	vSetupHistoryHistogramThreshold = 0;
	vSetupHistoryAverageCount = 0;
	vSetupHistoryStructureCount = 0;
	vSetupHistoryAverageDensity = 0;

	vHistoryTookAverageDate = "";

	for (BYTE TempLoop = 0; TempLoop < vNumberOfInspections; TempLoop++)
		vInspection[TempLoop]->ClearInspectionHistory();
}

void CProduct::SetOverScanFactor(BYTE TempOverScanFactor)
{
	vOverScanFactor = TempOverScanFactor;
	switch (TempOverScanFactor)
	{
		case 0:
			vOverScanMultiplier = 1;
		break;
		case 1:
			vOverScanMultiplier = 2;
		break;
		case 2:
			vOverScanMultiplier = 4;
		break;
	}
}

bool CProduct::ProductHasImageFiles(CString TempDirectory)
{
	// does product have any image files?
	bool TempProductHasImageFiles = false;

	CString *TempProductName = GetProductName();
	CString TempString1 = _T("=Process 1");
	TempString1 =  *TempProductName + TempString1;
	TempString1 = TempDirectory + TempString1;
	TempString1 = TempString1 + ".BMP";
	LPTSTR TempString = TempString1.GetBuffer(TempString1.GetLength());
	int TempHeight = 0;
	int TempWidth = 0;
	int TempBytesPerPixel = 0;
	int TempType = 0;

	CFileStatus TempFileStatus;
	if (CFile::GetStatus(TempString, TempFileStatus))
		TempProductHasImageFiles = true;

	TempString1 = "=Process 2";
	TempString1 =  *TempProductName + TempString1;
	TempString1 = TempDirectory + TempString1;
	TempString1 = TempString1 + ".BMP";
	TempString = TempString1.GetBuffer(TempString1.GetLength());

	if (CFile::GetStatus(TempString, TempFileStatus))
		TempProductHasImageFiles = true;

	if (vGlobalAllowVoidInspection)
	{
		TempString1 = "=Process 2v";
		TempString1 =  *TempProductName + TempString1;
		TempString1 = TempDirectory + TempString1;
		TempString1 = TempString1 + ".BMP";
		TempString = TempString1.GetBuffer(TempString1.GetLength());

		if (CFile::GetStatus(TempString, TempFileStatus))
			TempProductHasImageFiles = true;
	}

	TempString1 = "=Process 3";
	TempString1 =  *TempProductName + TempString1;
	TempString1 = TempDirectory + TempString1;
	TempString1 = TempString1 + ".BMP";
	TempString = TempString1.GetBuffer(TempString1.GetLength());

	if (CFile::GetStatus(TempString, TempFileStatus))
		TempProductHasImageFiles = true;
	return TempProductHasImageFiles;
}

BYTE CProduct::InspectionNumber(CInspection *TempInspection)
{
	BYTE TempInspectionNumber = 0xFF;
	if (vNumberOfInspections)
	for (BYTE TempLoop = 0; TempLoop < vNumberOfInspections;TempLoop++)
	{
		if (TempInspection->vName == vInspection[TempLoop]->vName)
			TempInspectionNumber = TempLoop;
	}
	return TempInspectionNumber;
}

bool CProduct::HaveInspection(CInspection *TempInspection)
{
	bool TempTempInspection = false;
	if (vNumberOfInspections)
	if (!TempInspection->vName.IsEmpty())
	for (BYTE TempLoop = 0; TempLoop < vNumberOfInspections;TempLoop++)
	{
		if (TempInspection->vName == vInspection[TempLoop]->vName)
			TempTempInspection = true;
	}
	return TempTempInspection;
}

bool CProduct::ProductHasBackgroundFile(CString TempDirectory)
{
	// does product have any image files?
	bool TempProductHasImageFiles = false;

	CString *TempProductName = GetProductName();
	CString TempString1 = "=Process 1";
	TempString1 =  *TempProductName + TempString1;
	TempString1 = TempDirectory + TempString1;
	TempString1 = TempString1 + ".BMP";
	LPTSTR TempString = TempString1.GetBuffer(TempString1.GetLength());
	int TempHeight = 0;
	int TempWidth = 0;
	int TempBytesPerPixel = 0;
	int TempType = 0;

	CFileStatus TempFileStatus;

	TempString1 = "=Process 0";
	TempString1 =  *TempProductName + TempString1;
	TempString1 = TempDirectory + TempString1;
	TempString1 = TempString1 + ".BMP";
	TempString = TempString1.GetBuffer(TempString1.GetLength());

	if (CFile::GetStatus(TempString, TempFileStatus))
		TempProductHasImageFiles = true;
	return TempProductHasImageFiles;
}

void CProduct::SetupAllInspectionROIsFromReference(CImageData *TempContainer, BYTE TempSetProductDensityROI, bool TempMakeROIFullImage, CImageData *TempAverageImage)
{
	if (TempAverageImage)
		CalculateLowerRimHeight(TempAverageImage, NULL);
	if (TempMakeROIFullImage)
	{
		int TempHeight = (int)((vProductImageHeightTop - vProductImageHeightBottom) * vGlobalPixelsPerUnitInHeight);
		if (vNumberOfInspections)
		for (BYTE TempLoop = 0; TempLoop < vNumberOfInspections; TempLoop++)
		if (vInspection[TempLoop])
		{
			vInspection[TempLoop]->SetupInspectionInPixels(TempHeight, 1, 0, (WORD)((double)vProductImageWidth * vGlobalPixelsPerUnit * vOverScanMultiplier), vOverScanMultiplier);
		}

		//Product Density Inspection
		if (!vMultiLaneNumberOfLanes)
		if (TempSetProductDensityROI)
		if (vDriftDensityInspection)
		{
			if ((vGlobalScanTracType == cFermataScanTrac) || (vGlobalScanTracType == cLegatoScanTrac))
				vDriftDensityInspection->SetupInspectionInPixels((int)((double)TempHeight * 0.5), (int)((double)TempHeight * 0.25), 2, vImageWidthPosition - 2, vOverScanMultiplier);
			else
				vDriftDensityInspection->SetupInspectionInPixels((int)((double)TempHeight * 0.65), (int)((double)TempHeight * 0.35), (int)((double)vImageWidthPosition * .35), (int)((double)vImageWidthPosition * .65), vOverScanMultiplier);
		}

		//vFillerMonitorInspection
		if (TempSetProductDensityROI)
		if (vFillerMonitorInspection)
		{
			vFillerMonitorInspection->SetupInspectionInPixels((int)((double)TempHeight), (int)((double)TempHeight * 0.5), 2, vImageWidthPosition - 2, vOverScanMultiplier);
		}
	}
	else
	if ((vReferenceRight) && (vReferenceTop))
	{
		if (TempSetProductDensityROI < 2)
		if (vNumberOfInspections)
		for (BYTE TempLoop = 0; TempLoop < vNumberOfInspections; TempLoop++)
		if (vInspection[TempLoop])
		{
			if (vInspection[TempLoop]->vInspectionType == cTopContaminantInspection)
				vInspection[TempLoop]->SetupInspectionROIFromReference((WORD)(vProductImageHeightTop * vGlobalPixelsPerUnitInHeight), 0, vReferenceLeft, vReferenceRight, vLowerRimHeightInPixels, vOverScanMultiplier);
			else
				vInspection[TempLoop]->SetupInspectionROIFromReference(vReferenceTop, 0, vReferenceLeft, vReferenceRight, vLowerRimHeightInPixels, vOverScanMultiplier);
		}

		//Product Density Inspection
		if (!vMultiLaneNumberOfLanes)
		if (TempSetProductDensityROI)
		if (vDriftDensityInspection)
		{
			WORD TempQuarterWidth = 0;

			if (vReferenceRight > vReferenceLeft + 3)
				TempQuarterWidth = (vReferenceRight - vReferenceLeft) / 4;

			vDriftDensityInspection->SetupInspectionInPixels((WORD)((double)vReferenceTop * 0.25), (WORD)((double)vReferenceTop * 0.1), vReferenceLeft + TempQuarterWidth, vReferenceRight - TempQuarterWidth, vOverScanMultiplier);
		}

		if (TempSetProductDensityROI)
		if (vFillerMonitorInspection)
		{
			vFillerMonitorInspection->SetupInspectionInPixels((WORD)((double)vReferenceTop), (WORD)((double)vReferenceTop * 0.5), vReferenceLeft, vReferenceRight, vOverScanMultiplier);
		}
	}
	else
	if (TempContainer)
	if ((TempContainer->vRight) && (TempContainer->vTop))
	{
		if (TempSetProductDensityROI < 2)
		if (vNumberOfInspections)
		for (BYTE TempLoop = 0; TempLoop < vNumberOfInspections; TempLoop++)
		if (vInspection[TempLoop])
		{
			if (vInspection[TempLoop]->vInspectionType == cTopContaminantInspection)
				vInspection[TempLoop]->SetupInspectionROIFromReference((WORD)(vProductImageHeightTop * vGlobalPixelsPerUnitInHeight), 0, TempContainer->vLeft, TempContainer->vRight, vLowerRimHeightInPixels, vOverScanMultiplier);
			else
				vInspection[TempLoop]->SetupInspectionROIFromReference(TempContainer->vTop, 0, TempContainer->vLeft, TempContainer->vRight, vLowerRimHeightInPixels, vOverScanMultiplier);
		}

		//Product Density Inspection
		if (!vMultiLaneNumberOfLanes)
		if (TempSetProductDensityROI)
		if (vDriftDensityInspection)
		{
			WORD TempQuarterWidth = 1;

			if (TempContainer->vRight > TempContainer->vLeft + 3)
				TempQuarterWidth = (TempContainer->vRight - TempContainer->vLeft) / 4;

			vDriftDensityInspection->SetupInspectionInPixels((WORD)((double)TempContainer->vTop * 0.25), (WORD)((double)TempContainer->vTop * 0.1), 
				TempContainer->vLeft + TempQuarterWidth, TempContainer->vRight - TempQuarterWidth, vOverScanMultiplier);
		}

		//vFillerMonitorInspection
		if (TempSetProductDensityROI)
		if (vFillerMonitorInspection)
		{
			vFillerMonitorInspection->SetupInspectionInPixels((WORD)((double)TempContainer->vTop), (WORD)((double)TempContainer->vTop * 0.5), 
				TempContainer->vLeft, TempContainer->vRight, vOverScanMultiplier);
		}
	}
}

void CProduct::CheckAllROIs(CImageData *TempAverageImage)
{
	double TempImageMargin = 0;
	//if (vGlobalUseMetric)
	//	TempImageMargin = (WORD)(25.4 * 0.40);
	//else
		TempImageMargin = (WORD)(0.40);

	WORD TempImageMarginPixels = (WORD)(TempImageMargin * vGlobalPixelsPerUnit * vOverScanMultiplier);

	CalculateLowerRimHeight(TempAverageImage, NULL);

	if (vNumberOfInspections)
	for (BYTE TempLoop = 0; TempLoop < vNumberOfInspections; TempLoop++)
	if (vInspection[TempLoop])
	{  //if you don't have a good top or right ROI, set all ROIs
		if ((!vInspection[TempLoop]->vROITop) || (vInspection[TempLoop]->vROITop > vProductImageHeightTop) || (!vInspection[TempLoop]->vROIPixelRight) || (vInspection[TempLoop]->vROIPixelRight > vImageWidthPosition))
		{
			ReportErrorMessage("Inspection: " + vInspection[TempLoop]->vName + " Top or Right ROI Invalid, setting to default. Top ROI : " + dtoa(vInspection[TempLoop]->vROITop, 2) +
				" Image Height: " + dtoa(vProductImageHeightTop, 2) + " Right ROI: " + dtoa(vInspection[TempLoop]->vROIPixelRight, 2) + " Image Width: " + dtoa(vImageWidthPosition) , cWriteToLog, 0);

			if (vReferenceRight > vImageWidthPosition)
				vReferenceRight = 0;
			if (vReferenceTop > vProductImageHeightTop * vGlobalPixelsPerUnit * vOverScanMultiplier)
				vReferenceTop = 0;

			if (vReferenceBottom >= vReferenceTop)
				vReferenceBottom = 0;

			if ((vReferenceRight) && (vReferenceTop))
				vInspection[TempLoop]->SetupInspectionROIFromReference(vReferenceTop, 0, vReferenceLeft, vReferenceRight, vLowerRimHeightInPixels, vOverScanMultiplier);
			else
				vInspection[TempLoop]->SetupInspectionROIFromReference((WORD)(vProductImageHeightTop * vGlobalPixelsPerUnit * vOverScanMultiplier) - TempImageMarginPixels, 0, 
				TempImageMarginPixels, vImageWidthPosition - TempImageMarginPixels, vLowerRimHeightInPixels, vOverScanMultiplier);
		}

		if (vInspection[TempLoop]->vROIPixelLeft >= vInspection[TempLoop]->vROIPixelRight)
		{
			ReportErrorMessage("Left ROI Invalid, setting to default", cWriteToLog, 0);
			if (vInspection[TempLoop]->vROIRight > TempImageMargin)
				vInspection[TempLoop]->SetROILeft(TempImageMargin, vOverScanMultiplier);
			else
				vInspection[TempLoop]->SetROILeft(0, vOverScanMultiplier);
		}

		if (vInspection[TempLoop]->vROIPixelBottom >= vInspection[TempLoop]->vROIPixelTop)
		{
			ReportErrorMessage("Bottom ROI Invalid, setting to default", cWriteToLog, 0);
			if (vInspection[TempLoop]->vROIPixelTop > 6)
				vInspection[TempLoop]->SetROIBottom(6);
			else
				vInspection[TempLoop]->SetROIBottom(0);
		}
	}

	if (vMultiLaneNumberOfLanes == 0)
	if (vDriftDensityInspection)
	{  //if the density ROI has any bad values, set all 4 ROI lines
		if ((!vDriftDensityInspection->vROITop) || (vDriftDensityInspection->vROIBottom < 0) || (vDriftDensityInspection->vROITop > vProductImageHeightTop) || 
			(vDriftDensityInspection->vROITop <= vDriftDensityInspection->vROIBottom) || (!vDriftDensityInspection->vROIRight) || (vDriftDensityInspection->vROILeft < 0) ||
			(vDriftDensityInspection->vROILeft >= vDriftDensityInspection->vROIRight) || (vDriftDensityInspection->vROIRight > vProductImageWidth))
		{
			ReportErrorMessage("Product Density ROI Invalid, setting to default", cWriteToLog, 0);
			if ((vReferenceRight) && (vReferenceTop))
			{
				if ((vReferenceTop > 36 * vGlobalPixelsPerUnitInHeight) || (vReferenceRight > 48 * vGlobalPixelsPerUnit))
				{
					vReferenceTop = 0;
					vReferenceBottom = 0;
					vReferenceRight = 0;
					vReferenceLeft = 0;
				}
				else
				{
					WORD TempQuarterWidth = (vReferenceRight - vReferenceLeft) / 4;

					if (vGlobalScanTracType == cBriosoScanTrac)
					{
						WORD TempTop = 27;
						WORD TempBottom = 5;
						vDriftDensityInspection->SetDefaultROI(TempTop, TempBottom, vReferenceLeft + TempQuarterWidth, vReferenceRight - TempQuarterWidth, vOverScanMultiplier);
					}
					else
					if ((vGlobalScanTracType == cAtempoScanTrac) || (vGlobalScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) || (vGlobalScanTracType == cBriosoScanTrac) ||
						(vGlobalScanTracType == cLegatoScanTrac) || (vGlobalScanTracType == cFermataScanTrac) || (vGlobalScanTracType == cCaseInspectorScanTrac))
					{
						WORD TempTop = (WORD)(vReferenceTop - 2);
						WORD TempBottom = (WORD)(2);
						vDriftDensityInspection->SetDefaultROI(TempTop, TempBottom, 2, vReferenceRight - 2, vOverScanMultiplier);
					}
					else
					{
						WORD TempTop = (WORD)(vReferenceTop / 4);
						WORD TempBottom = (WORD)(vReferenceTop * 0.1);
						vDriftDensityInspection->SetDefaultROI(TempTop, TempBottom, vReferenceLeft + TempQuarterWidth, vReferenceRight - TempQuarterWidth, vOverScanMultiplier);
					}
				}
			}
			else
			{
				if ((vGlobalScanTracType == cAtempoScanTrac) || (vGlobalScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) || (vGlobalScanTracType == cBriosoScanTrac) ||
					(vGlobalScanTracType == cLegatoScanTrac) || (vGlobalScanTracType == cFermataScanTrac) || (vGlobalScanTracType == cCaseInspectorScanTrac))
				{
					vDriftDensityInspection->SetDefaultROI((WORD)((vProductImageHeightTop * vGlobalPixelsPerUnitInHeight) - 2), (WORD)(2), 2, (WORD)(vImageWidthPosition - 2), vOverScanMultiplier);
				}
				else
				{
					vDriftDensityInspection->SetDefaultROI((WORD)(vProductImageHeightTop * vGlobalPixelsPerUnitInHeight / 4), (WORD)(vProductImageHeightTop * vGlobalPixelsPerUnitInHeight / 10), 
						vImageWidthPosition * 4 / 10, vImageWidthPosition * 7 / 10, vOverScanMultiplier);
				}
			}
		}
		else
		if (TempAverageImage) //if have a learn for this product
		if ((vReferenceRight) && (vReferenceTop))
		if ((vDriftDensityInspection->vROIPixelRight >= vReferenceRight) || (vDriftDensityInspection->vROIPixelLeft <= vReferenceLeft))
		if ((vGlobalScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloPlusScanTrac) || (vGlobalScanTracType == cPiccoloScanTrac))
		if (!vGlobalInAutoSize)
		{
			ReportErrorMessage("Product Density ROI very wide, consider making narrower", cEMailMaintenance, 0);
			//WORD TempQuarterWidth = (vReferenceRight - vReferenceLeft) / 4;
			////9/9/2009
			//WORD TempTop = vReferenceTop / 5;
			//WORD TempBottom = vReferenceTop / 10;
			//vDriftDensityInspection->SetDefaultROI(TempTop, TempBottom, vReferenceLeft + TempQuarterWidth, vReferenceRight - TempQuarterWidth, vOverScanMultiplier);
		}
	}
	
	if (vFillerMonitorInspection)
	{  //if the density ROI has any bad values, set all 4 ROI lines
		if ((!vFillerMonitorInspection->vROITop) || (vFillerMonitorInspection->vROIBottom < 0) || (vFillerMonitorInspection->vROITop > vProductImageHeightTop) || 
			(vFillerMonitorInspection->vROITop <= vFillerMonitorInspection->vROIBottom) || (!vFillerMonitorInspection->vROIRight) || (vFillerMonitorInspection->vROILeft < 0) ||
			(vFillerMonitorInspection->vROILeft >= vFillerMonitorInspection->vROIRight) || (vFillerMonitorInspection->vROIRight > vProductImageWidth))
		{
			ReportErrorMessage("Filler MonitorInspection ROI Invalid, setting to default", cWriteToLog, 0);
			if ((vReferenceRight) && (vReferenceTop))
			{
				if ((vReferenceTop > 36) || (vReferenceRight > 48))
				{
					vReferenceTop = 0;
					vReferenceBottom = 0;
					vReferenceRight = 0;
					vReferenceLeft = 0;
				}
				else
				{
					if (vGlobalScanTracType == cBriosoScanTrac)
					{
						WORD TempTop = 27;
						WORD TempBottom = 5;
						vFillerMonitorInspection->SetDefaultROI(TempTop, TempBottom, vReferenceLeft, vReferenceRight, vOverScanMultiplier);
					}
					else
					if ((vGlobalScanTracType == cAtempoScanTrac) || (vGlobalScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) || (vGlobalScanTracType == cBriosoScanTrac) ||
						(vGlobalScanTracType == cLegatoScanTrac) || (vGlobalScanTracType == cFermataScanTrac) || (vGlobalScanTracType == cCaseInspectorScanTrac))
					{
						WORD TempTop = (WORD)(vReferenceTop - 2);
						WORD TempBottom = (WORD)(2);
						vFillerMonitorInspection->SetDefaultROI(TempTop, TempBottom, 2, vReferenceRight - 2, vOverScanMultiplier);
					}
					else
					{
						WORD TempTop = (WORD)(vReferenceTop);
						WORD TempBottom = (WORD)(vReferenceTop * 0.5);
						vFillerMonitorInspection->SetDefaultROI(TempTop, TempBottom, vReferenceLeft, vReferenceRight, vOverScanMultiplier);
					}
				}
			}
			else
			{
				if ((vGlobalScanTracType == cAtempoScanTrac) || (vGlobalScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) || (vGlobalScanTracType == cBriosoScanTrac) ||
					(vGlobalScanTracType == cLegatoScanTrac) || (vGlobalScanTracType == cFermataScanTrac) || (vGlobalScanTracType == cCaseInspectorScanTrac))
				{
					vFillerMonitorInspection->SetDefaultROI((WORD)((vProductImageHeightTop * vGlobalPixelsPerUnitInHeight) - 2), (WORD)(2), 2, (WORD)(vImageWidthPosition - 2), vOverScanMultiplier);
				}
				else
				{
					vFillerMonitorInspection->SetDefaultROI((WORD)(vProductImageHeightTop * vGlobalPixelsPerUnitInHeight), (WORD)(vProductImageHeightTop * vGlobalPixelsPerUnitInHeight), 
						vImageWidthPosition * 1 / 10, vImageWidthPosition * 9 / 10, vOverScanMultiplier);
				}
			}
		}
	}
}

void CProduct::MatchInspectionsForWeightTrending()
{
	if (vNumberOfInspections)
	for (BYTE TempLoop = 0; TempLoop < vNumberOfInspections; TempLoop++)
	if (vInspection[TempLoop])
	{
		vInspection[TempLoop]->vInspectionNameForWeightTrendingPointer = NULL;
		if (vInspection[TempLoop]->vInspectionNameForWeightTrendingName.GetLength())
		{
			for (BYTE TempLoopI = 0; TempLoopI < vNumberOfInspections; TempLoopI++) //go through each inspection looking for the matching name
			if (TempLoopI != TempLoop) //ignore this inspection
			{
				if (vInspection[TempLoop]->vInspectionNameForWeightTrendingName == vInspection[TempLoopI]->vName)
				if ((vInspection[TempLoopI]->vInspectionType == cUnderfillByWeightInspection) || (vInspection[TempLoopI]->vInspectionType == cOverfillByWeightInspection))
				{ //name matches, so this is the inspection that is the weight trending inspection
					vInspection[TempLoop]->vInspectionNameForWeightTrendingPointer = vInspection[TempLoopI];
					vInspection[TempLoop]->vCheckWeighMFactor = vInspection[TempLoopI]->vCheckWeighMFactor;  //copy weight calibration formula y=mx+b calibration of density to weight
					vInspection[TempLoop]->vCheckWeighBFactor = vInspection[TempLoopI]->vCheckWeighBFactor;  //copy weight calibration formula y=mx+b calibration of density to weight
					vInspection[TempLoop]->vCheckWeighOffset = vInspection[TempLoopI]->vCheckWeighOffset;
					vInspection[TempLoop]->vCheckWeighUnits = vInspection[TempLoopI]->vCheckWeighUnits;  //copy weight units
					vInspection[TempLoop]->SetROITop(vInspection[TempLoopI]->vROITop);  //copy ROI
					vInspection[TempLoop]->SetROIBottom(vInspection[TempLoopI]->vROIBottom);  //copy ROI
					vInspection[TempLoop]->SetROILeft(vInspection[TempLoopI]->vROILeft, vOverScanMultiplier);  //copy ROI
					vInspection[TempLoop]->SetROIRight(vInspection[TempLoopI]->vROIRight, vOverScanMultiplier);  //copy ROI

					TempLoopI = vNumberOfInspections; //exit loop
				}
			}
			if (!vInspection[TempLoop]->vInspectionNameForWeightTrendingPointer) //if can not find inspection that was named, generate yellow message
				ReportErrorMessage("Lost Weight Trending Inspection for Inspection: " + vInspection[TempLoop]->vName, cEMailMaintenance, 0);
		}
	}
}

BYTE CProduct::AreThereAnyOtherWeightTrendingInspections(BYTE TempThisInspection)
{
	BYTE TempReturn = 0;
	if (vNumberOfInspections > 1)
	if (vInspection[TempThisInspection])
	if ((vInspection[TempThisInspection]->vInspectionType == cUnderfillByWeightInspection) || (vInspection[TempThisInspection]->vInspectionType == cOverfillByWeightInspection)) //only yes if this one is weight trending
	for (BYTE TempLoop = 0; TempLoop < vNumberOfInspections; TempLoop++)
	if (vInspection[TempLoop])
	if (TempThisInspection != TempLoop) //ignore this inspection
	if ((vInspection[TempLoop]->vInspectionType == cUnderfillByWeightInspection) || (vInspection[TempLoop]->vInspectionType == cOverfillByWeightInspection))
	if (vInspection[TempLoop]->vInspectionNameForWeightTrendingName.GetLength() == 0) //can not be pointing at another inspection
		TempReturn++;
	return TempReturn;
}

BYTE CProduct::AreThereAnyWeightTrendingInspections()
{
	BYTE TempReturn = 0;
	if (vNumberOfInspections > 1)
	for (BYTE TempLoop = 0; TempLoop < vNumberOfInspections; TempLoop++)
	if (vInspection[TempLoop])
	if ((vInspection[TempLoop]->vInspectionType == cUnderfillByWeightInspection) || (vInspection[TempLoop]->vInspectionType == cOverfillByWeightInspection))
	if (vInspection[TempLoop]->vInspectionNameForWeightTrendingName.GetLength() == 0) //can not be pointing at another inspection
		TempReturn++;
	return TempReturn;
}

bool CProduct::CalculateLowerRimHeight(BYTE *TempAverageImage, BYTE *TempCurrentImage)
{
	vLowerRimHeightInPixels = 0;
	BYTE *TempImageToUse = TempAverageImage;
	if (!TempAverageImage)
		TempImageToUse = TempCurrentImage;

	WORD TempImageHeightInPixels = (WORD)(vProductImageHeightTop * vGlobalPixelsPerUnitInHeight);

	if (TempImageToUse)
	{
		WORD TempPixelToStartLooking = TempImageHeightInPixels / 4;
		if (vReferenceTop)
			TempPixelToStartLooking = vReferenceTop / 3;

		WORD TempMaximumHeightToLookAt = TempImageHeightInPixels - 5;
		if (vReferenceTop)
			TempMaximumHeightToLookAt = vReferenceTop - 5;

		WORD TempCenterOfImage = vImageWidthPosition / 2;
		if (vReferenceRight)
			TempCenterOfImage = (vReferenceRight - vReferenceLeft) / 2;

		BYTE *TempPointer = TempImageToUse + (TempCenterOfImage * TempImageHeightInPixels) + TempPixelToStartLooking;

		WORD TempMinimumBrightness = 255;
		WORD TempHeightFoundMinimumBrightnessAt = 0;

		for (WORD TempLoop = TempPixelToStartLooking; TempLoop < TempMaximumHeightToLookAt; TempLoop++)
		{
			if (*TempPointer < TempMinimumBrightness)
			{
				TempMinimumBrightness = *TempPointer;
				TempHeightFoundMinimumBrightnessAt = TempLoop;
			}
			*TempPointer++;
		}

		if (TempHeightFoundMinimumBrightnessAt)
		{

			if (vSetupHistoryAverageDensity)
			{
				if (TempMinimumBrightness < vSetupHistoryAverageDensity * .75)
				{
					vLowerRimHeightInPixels = TempHeightFoundMinimumBrightnessAt;
					return true;
				}
				else
					return false;
			}
			else
			{
				vLowerRimHeightInPixels = TempHeightFoundMinimumBrightnessAt;
				return true;
			}
		}
		else
			return false;
	}
	else
		return false;
}

bool CProduct::CalculateLowerRimHeight(CImageData *TempAverageImage, CImageData *TempCurrentImage)
{
	if ((vGlobalScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloPlusScanTrac) || (vGlobalScanTracType == cPiccoloScanTrac))
	{
		WORD TempLeft = vReferenceLeft;
		WORD TempRight = vReferenceRight;

		vLowerRimHeightInPixels = 0;
		BYTE *TempImageToUse = NULL;

		if (TempAverageImage)
		{
			TempImageToUse = TempAverageImage->vGreyImage;
			if (TempAverageImage->vLeft)
			{
				TempLeft = TempAverageImage->vRight;
				TempRight = TempAverageImage->vLeft;
			}
		}
		if (!TempAverageImage)
		if (TempCurrentImage)
		if (TempCurrentImage->vTop)
		{
			TempImageToUse = TempCurrentImage->vGreyImage;
			if (TempCurrentImage->vRight)
			{
				TempLeft = TempCurrentImage->vLeft;
				TempRight = TempCurrentImage->vRight;
			}
		}

		WORD TempImageHeightInPixels = (WORD)(vProductImageHeightTop * vGlobalPixelsPerUnitInHeight);

		if (TempImageToUse)
		{

			WORD TempPixelToStartLooking = TempImageHeightInPixels / 4;
			if (vReferenceTop)
				TempPixelToStartLooking = vReferenceTop / 3;

			WORD TempMaximumHeightToLookAt = TempImageHeightInPixels - 5;
			if (vReferenceTop)
				TempMaximumHeightToLookAt = vReferenceTop - 5;

			WORD TempCenterOfImage = vImageWidthPosition / 2;
			if (TempRight)
				TempCenterOfImage = TempLeft + ((TempRight - TempLeft) / 2);

			BYTE *TempPointer = TempImageToUse + (TempCenterOfImage * TempImageHeightInPixels) + TempPixelToStartLooking;

			WORD TempMinimumBrightness = 255;
			WORD TempHeightFoundMinimumBrightnessAt = 0;

			for (WORD TempLoop = TempPixelToStartLooking; TempLoop < TempMaximumHeightToLookAt; TempLoop++)
			{
				// crashed here 12/4/2014 when changed product height
				if (*TempPointer < TempMinimumBrightness)
				{
					TempMinimumBrightness = *TempPointer;
					TempHeightFoundMinimumBrightnessAt = TempLoop;
				}
				*TempPointer++;
			}

			if (TempHeightFoundMinimumBrightnessAt)
			{

				if (vSetupHistoryAverageDensity)
				{
					if (TempMinimumBrightness < vSetupHistoryAverageDensity * .75)
					{
						vLowerRimHeightInPixels = TempHeightFoundMinimumBrightnessAt;
						return true;
					}
					else
						return false;
				}
				else
				{
					vLowerRimHeightInPixels = TempHeightFoundMinimumBrightnessAt;
					return true;
				}
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}

void CProduct::CheckInspectionsAreValid()
{
	BYTE TempGoodInspectionCount = 0;
	for (BYTE TempLoop = 0; TempLoop < vNumberOfInspections; TempLoop++)
	{
		if (vInspection[TempLoop])
			TempGoodInspectionCount++;  //good inspection, so increment count
		else
		{ //invalid inspection, so move the rest of the inspections down one in the pointers
			ReportErrorMessage("Invalid Inspection before Stream read or write, Product: " + vProductName + ", Inspection: " + dtoa(TempLoop + 1, 0), cEMailInspx, 32000);
			for (int TempMoveLoop = 0; TempMoveLoop < vNumberOfInspections - 1 - TempLoop; TempMoveLoop++)
				vInspection[TempGoodInspectionCount + TempMoveLoop] = vInspection[TempLoop + 1 + TempMoveLoop];
		}
	}
	vNumberOfInspections = TempGoodInspectionCount;
}

void CProduct::SetAllDistances()
{
		for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
			SetEjectorBeltPositionOffset(TempLoop,vEjectorDelayPosition[TempLoop]);

		SetProductBodyTriggerToImageBeltPositionOffset(vProductBodyTriggerToImageBeltPositionOffset);
		SetProductLockOutWidth(vProductLockOutWidth);
		SetProductImageWidth(vProductImageWidth);
		SetEndOfLineTimeOut(vEndOfLineTimeOut);
		SetEdgeLocationBottom(vEdgeLocationBottom);
		SetEdgeLocationHeight(vEdgeLocationHeight);

		SetBottomLocationLeft(vBottomLocationLeft);
		SetBottomLocationLength(vBottomLocationLength);

		for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
		{
			SetExternalDetectorWindowStart(TempLoop, vExternalDetectorWindowStart[TempLoop]);
			SetExternalDetectorWindowEnd(TempLoop, vExternalDetectorWindowEnd[TempLoop]);
		}

		for (BYTE TempLoop = 0; TempLoop < vNumberOfInspections; TempLoop++)
		{
			if (vInspection[TempLoop])
			{
				vInspection[TempLoop]->SetROITop(vInspection[TempLoop]->vROITop);
				vInspection[TempLoop]->SetROIBottom(vInspection[TempLoop]->vROIBottom);
				vInspection[TempLoop]->SetROILeft(vInspection[TempLoop]->vROILeft, vOverScanMultiplier);
				vInspection[TempLoop]->SetROIRight(vInspection[TempLoop]->vROIRight,vOverScanMultiplier);
			}
			else
				vNumberOfInspections = TempLoop;
		}

		if (vDriftDensityInspection)
		{
			CString TempText;
			TempText.LoadString(IDS_DensityCompensation);
			vDriftDensityInspection->vName = TempText;
			vDriftDensityInspection->SetROITop(vDriftDensityInspection->vROITop);
			vDriftDensityInspection->SetROIBottom(vDriftDensityInspection->vROIBottom);
			vDriftDensityInspection->SetROILeft(vDriftDensityInspection->vROILeft, vOverScanMultiplier);
			vDriftDensityInspection->SetROIRight(vDriftDensityInspection->vROIRight,vOverScanMultiplier);
		}

		if (vFillerMonitorInspection)
		{
			vFillerMonitorInspection->vName = "Filler Monitor";
			vFillerMonitorInspection->SetROITop(vFillerMonitorInspection->vROITop);
			vFillerMonitorInspection->SetROIBottom(vFillerMonitorInspection->vROIBottom);
			vFillerMonitorInspection->SetROILeft(vFillerMonitorInspection->vROILeft, vOverScanMultiplier);
			vFillerMonitorInspection->SetROIRight(vFillerMonitorInspection->vROIRight,vOverScanMultiplier);
		}

		CheckAllROIs(NULL);
		if ((vProductImageWidth > cMaximumImageWidth * 3) || (vProductImageWidth < 0))
		{
			vProductImageWidth = 5;
			SetProductImageWidth(vProductImageWidth);
			ReportErrorMessage("Bad float in stream corrected: vProductImageWidth", cEMailInspx, 32000);
		}
}


void CProduct::ChangeInspectionNames()
{
	for (BYTE TempLoop = 0; TempLoop < vNumberOfInspections; TempLoop++)
	if (vInspection[TempLoop])
	{
		//int TempPosition = vInspection[TempLoop]->vName.Find("Contaminant P");
		//if (TempPosition >= 0)
		vInspection[TempLoop]->vName.Replace( _T("Contaminant P"), _T("Small Contaminant"));
		vInspection[TempLoop]->vName.Replace( _T("Contaminant Q"), _T("Medium Contaminant"));
		vInspection[TempLoop]->vName.Replace( _T("Contaminant R"), _T("Large Contaminant"));
		vInspection[TempLoop]->vName.Replace( _T("Contaminant T"), _T("Metal Contaminant"));
	}
}
