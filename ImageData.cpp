//ScanTrac Side View Source File
// ImageData.cpp: implementation of the CImageData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScanTrac.h"
#include "ImageData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImageData::CImageData()
{
	vGreyImage = NULL;
	vRedImage = NULL;

	vTopReferenceAdjust = 0;
	vLeftReferenceAdjust = 0;
	vRightReferenceAdjust = 0;
	vTop = 0;
	vBottom = 0;
	vRight = 0;
	vLeft = 0;
	vHeight = 0;
	vWidth = 0;
	for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfInspections; TempLoop++)
	{
		vNumberOfBlobs[TempLoop] = 0;
		vRejectedForThisInspection[TempLoop] = 0;
		vDensityValue[TempLoop] = 0;
		vInspectonProcessingTime[TempLoop] = 0;
		vContainerWeightCorrection[TempLoop] = 0;
		vLargestBlobPerimeter[TempLoop] = 0;
	}
	vFineReferenceAdjust = 0;
	for (BYTE TempLoop = 0;TempLoop < 3; TempLoop++)
		vHeadNumber[TempLoop] = 0; //for filler monitoring and Filler and Seamer Sampling
	vFillLevel = 0;
	vContainerEjected = 0;
	vPassContainer = 0;
	vContainerEjectReason = 255;
	vContainerEjectInspection = 255;
	vContainerEjectThreshold = 0;
	vContainerLearnState = 0;
	vContainerLargestContaminant = 0;
	vMultiLaneLaneNumber = 0;
	vMultiLaneEjectorOffset = 0;
	vRejectDisabled = false;
	vDriftCompensationLocked = true;
	//vImproveLearn = false;
	vHaveRealImage = 0;
	vMinimumGoodMaximumBad = 255;
	vImageTypeString = "Unknown Type";
	vContainerNumber = 0;
	vContainerNumberToEject = 0;
	vBodyTriggerGapLength = 0;
	vImageType = cNoImageType;

	vCurveFitTime = 0;
	vGlassInspectionTime = 0;
	vSmoothTime = 0;
	vProcessTime = 0;
	vEnhanceTime = 0;
	vEdgeAdjustTime = 0;
	vFindEdgesTime = 0;
	vReferencePositionTime = 0;
	vCopyBufferTime = 0;
	vCopyToMainTime = 0;
	vFindContaminantsTime = 0;
	//vFindGlassContaminantsTime = 0;
	vFindVoidsTime = 0;
	vFindMedianTime = 0;
	vFindDensityTime = 0;
	vFindBlobsTime = 0;
	vFindTime = 0;
	vOriginalImageIntensity = 0;
	vImageIntensityAdjust = 0;
	vFinalImageIntensity = 0;
	vImageIntensityTooFarOff = false;
	//vThresholdCorrection = 0;
	//vNegitiveThresholdCorrection = 0;
	for (BYTE TempLoop = 0; TempLoop < cMaximumRedBoxes; TempLoop++)
	{
		vRedBoxCornerX[TempLoop] = 0;
		vRedBoxCornerY[TempLoop] = 0;
		vRedBoxWidth[TempLoop] = 0;
		vRedBoxHeight[TempLoop] = 0;
		vRedBoxInspection[TempLoop] = 0;
	}

	CTime TempTime;
	TempTime = CTime::GetCurrentTime();
	//vTime = TempTime.Format("%d-%B-%Y-%H-%M-%S");
	//vTime = TempTime.Format("%Y-%m-%d %H:%M:%S");
	vTime = TempTime. Format("%Y-%b-%d %H:%M:%S");
	//vTime = TempTime.Format("%c");
}

CImageData::~CImageData()
{
	if (vGreyImage != NULL) 
	{
		free(vGreyImage);
		vGreyImage = NULL;
	}
	if (vRedImage != NULL) 
	{
		free(vRedImage);
		vRedImage = NULL;
	}
}

void CImageData::MakeGreyImage(DWORD TempSize)
{
	if (vGreyImage == NULL)
	if (ThereIsEnoughMemory(TempSize, "Grey Image Data"))
	{
		vGreyImage = (BYTE *)malloc(TempSize);
	}
}

void CImageData::MakeRedImage(DWORD TempSize)
{
	if (vRedImage == NULL)
	if (ThereIsEnoughMemory(TempSize, "Red Image Data"))
	{
		vRedImage = (BYTE *)malloc(TempSize);
		if (vRedImage)
			FillMemory(vRedImage,TempSize,255);
	}
}

void CImageData::CopyImage(CImageData *TempSourceImage, DWORD TempSize)
{
	if (TempSourceImage)
	{
		if (TempSourceImage->vRedImage)
		if (!vRedImage)
			MakeRedImage(TempSize);

		if (TempSourceImage->vGreyImage)
		if (!vGreyImage)
			MakeGreyImage(TempSize);

		BYTE *TempRedSourcePointer = TempSourceImage->vRedImage;
		BYTE *TempGreySourcePointer = TempSourceImage->vGreyImage;
		BYTE *TempRedDestinationPointer = vRedImage;
		BYTE *TempGreyDestinationPointer = vGreyImage;
		if ((TempRedSourcePointer) && (TempRedDestinationPointer) && (TempGreyDestinationPointer) && (TempGreySourcePointer))
		{
			for (DWORD TempLoop = 0; TempLoop < TempSize; TempLoop++)
			{
				*TempRedDestinationPointer++ = *TempRedSourcePointer++;
				*TempGreyDestinationPointer++ = *TempGreySourcePointer++;
			}
		}
		else
		if ((TempGreyDestinationPointer) && (TempGreySourcePointer))
		{
			for (DWORD TempLoop = 0; TempLoop < TempSize; TempLoop++)
				*TempGreyDestinationPointer++ = *TempGreySourcePointer++;
		}

		vTime = TempSourceImage->vTime;
		vContainerEjected = TempSourceImage->vContainerEjected;
		vPassContainer = TempSourceImage->vPassContainer;
		vContainerEjectReason = TempSourceImage->vContainerEjectReason;
		vContainerEjectInspection = TempSourceImage->vContainerEjectInspection;
		vContainerEjectThreshold = TempSourceImage->vContainerEjectThreshold;
		vContainerLearnState = TempSourceImage->vContainerLearnState;
		vContainerLargestContaminant = TempSourceImage->vContainerLargestContaminant;
		vMultiLaneLaneNumber = TempSourceImage->vMultiLaneLaneNumber;
		vMultiLaneEjectorOffset = TempSourceImage->vMultiLaneEjectorOffset;

		for(BYTE TempLoop = 0;TempLoop < cMaximumNumberOfInspections; TempLoop++)
			vRejectedForThisInspection[TempLoop] = TempSourceImage->vRejectedForThisInspection[TempLoop];

		vRejectDisabled = TempSourceImage->vRejectDisabled;
		vDriftCompensationLocked = TempSourceImage->vDriftCompensationLocked;
		for (BYTE TempLoop = 0;TempLoop < 3; TempLoop++)
			vHeadNumber[TempLoop] = TempSourceImage->vHeadNumber[TempLoop]; //for filler monitoring and Filler and Seamer Sampling
		vFillLevel = TempSourceImage->vFillLevel;
		vTopReferenceAdjust = TempSourceImage->vTopReferenceAdjust;
		vLeftReferenceAdjust = TempSourceImage->vLeftReferenceAdjust;
		vRightReferenceAdjust = TempSourceImage->vRightReferenceAdjust;
		vTop = TempSourceImage->vTop;
		vBottom = TempSourceImage->vBottom;
		vRight = TempSourceImage->vRight;
		vLeft = TempSourceImage->vLeft;
		vHeight = TempSourceImage->vHeight;
		vWidth = TempSourceImage->vWidth;
		vFineReferenceAdjust = TempSourceImage->vFineReferenceAdjust;
		for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfInspections; TempLoop++)
		{
			vDensityValue[TempLoop] = TempSourceImage->vDensityValue[TempLoop];
			vNumberOfBlobs[TempLoop] = TempSourceImage->vNumberOfBlobs[TempLoop];
			vRejectedForThisInspection[TempLoop] = TempSourceImage->vRejectedForThisInspection[TempLoop];
			vInspectonProcessingTime[TempLoop] = TempSourceImage->vInspectonProcessingTime[TempLoop];
			vContainerWeightCorrection[TempLoop] = TempSourceImage->vContainerWeightCorrection[TempLoop];
			vLargestBlobPerimeter[TempLoop] = TempSourceImage->vLargestBlobPerimeter[TempLoop];

		}
		vHaveRealImage = TempSourceImage->vHaveRealImage;
		vMinimumGoodMaximumBad = TempSourceImage->vMinimumGoodMaximumBad;
		vImageTypeString = TempSourceImage->vImageTypeString;
		vBodyTriggerGapLength = TempSourceImage->vBodyTriggerGapLength;
		vContainerNumber = TempSourceImage->vContainerNumber;
		vContainerNumberToEject = TempSourceImage->vContainerNumberToEject;
		vImageType = TempSourceImage->vImageType;


		vCurveFitTime = TempSourceImage->vCurveFitTime;
		vGlassInspectionTime = TempSourceImage->vGlassInspectionTime;
		vSmoothTime = TempSourceImage->vSmoothTime;
		vProcessTime = TempSourceImage->vProcessTime;
		vEnhanceTime = TempSourceImage->vEnhanceTime;
		vEdgeAdjustTime = TempSourceImage->vEdgeAdjustTime;
		vFindEdgesTime = TempSourceImage->vFindEdgesTime;
		vReferencePositionTime = TempSourceImage->vReferencePositionTime;
		vCopyBufferTime = TempSourceImage->vCopyBufferTime;
		vCopyToMainTime = TempSourceImage->vCopyToMainTime;
		vFindContaminantsTime = TempSourceImage->vFindContaminantsTime;
		//vFindGlassContaminantsTime = TempSourceImage->vFindGlassContaminantsTime;
		vFindMedianTime = TempSourceImage->vFindMedianTime;
		vFindVoidsTime = TempSourceImage->vFindVoidsTime;
		vFindDensityTime = TempSourceImage->vFindDensityTime;
		vFindBlobsTime = TempSourceImage->vFindBlobsTime;
		vFindTime = TempSourceImage->vFindTime;
		vOriginalImageIntensity = TempSourceImage->vOriginalImageIntensity;
		vImageIntensityAdjust = TempSourceImage->vImageIntensityAdjust;
		vFinalImageIntensity = TempSourceImage->vFinalImageIntensity;
		vImageIntensityTooFarOff = TempSourceImage->vImageIntensityTooFarOff;
		//vThresholdCorrection = TempSourceImage->vThresholdCorrection;
		//vNegitiveThresholdCorrection = TempSourceImage->vNegitiveThresholdCorrection;
		for (BYTE TempLoop = 0; TempLoop < cMaximumRedBoxes; TempLoop++)
		{
			vRedBoxCornerX[TempLoop] = TempSourceImage->vRedBoxCornerX[TempLoop];
			vRedBoxCornerY[TempLoop] = TempSourceImage->vRedBoxCornerY[TempLoop];
			vRedBoxWidth[TempLoop] = TempSourceImage->vRedBoxWidth[TempLoop];
			vRedBoxHeight[TempLoop] = TempSourceImage->vRedBoxHeight[TempLoop];
			vRedBoxInspection[TempLoop] = TempSourceImage->vRedBoxInspection[TempLoop];
		}
	}
}