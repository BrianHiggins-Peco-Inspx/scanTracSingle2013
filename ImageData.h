//ScanTrac Side View Source File
// ImageData.h: interface for the CImageData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEDATA_H__8004A6B0_F7A2_11D4_AD44_00500466E305__INCLUDED_)
#define AFX_IMAGEDATA_H__8004A6B0_F7A2_11D4_AD44_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const BYTE cMaximumRedBoxes = 10;

enum {cNoImageType,cRealImageType, cSimulatedImageType, cClipboardImageType, 
	cDataImageType, cLoadedFromFileImageType, cDemoImageType};
class CImageData : public CObject  
{
public:
	//variables
	BYTE *vGreyImage;
	BYTE *vRedImage;
	CString vTime;

	WORD vBodyTriggerGapLength;
	DWORD vContainerNumber;
	BYTE vContainerNumberToEject;
	BYTE vHeadNumber[3]; //for filler monitoring and Filler and Seamer Sampling
	double vFillLevel;
	BYTE vContainerEjected;
	bool vPassContainer;
	BYTE vContainerEjectReason;
	BYTE vContainerEjectInspection;
	double vContainerEjectThreshold;
	BYTE vContainerLearnState;
	DWORD vContainerLargestContaminant;
	BYTE vRejectDisabled;
	bool vDriftCompensationLocked;
	bool vHaveRealImage;
	double vMinimumGoodMaximumBad;
	CString vImageTypeString;
	BYTE vImageType;
	BYTE vMultiLaneLaneNumber;
	WORD vMultiLaneEjectorOffset;

	int vTopReferenceAdjust;
	int vLeftReferenceAdjust;
	int vRightReferenceAdjust;
	WORD vTop;
	WORD vBottom;
	WORD vRight;
	WORD vLeft;
	WORD vHeight;
	WORD vWidth;
	DWORD vNumberOfBlobs[cMaximumNumberOfInspections];
	DWORD vLargestBlobPerimeter[cMaximumNumberOfInspections];
	bool vRejectedForThisInspection[cMaximumNumberOfInspections];
	double vInspectonProcessingTime[cMaximumNumberOfInspections];
	double vContainerWeightCorrection[cMaximumNumberOfInspections];
	
	BYTE vFineReferenceAdjust;
	double vDensityValue[cMaximumNumberOfInspections];

	double vCurveFitTime;
	double vGlassInspectionTime;
	double vSmoothTime;
	double vProcessTime;
	double vEnhanceTime;
	double vEdgeAdjustTime;
	double vFindEdgesTime;
	double vReferencePositionTime;
	double vCopyBufferTime;
	double vCopyToMainTime;
	double vFindContaminantsTime;
	//double vFindGlassContaminantsTime;
	double vFindVoidsTime;
	double vFindMedianTime;
	double vFindDensityTime;
	double vFindBlobsTime;
	double vFindTime;
	double vOriginalImageIntensity;
	double vImageIntensityAdjust;
	double vFinalImageIntensity;
	bool vImageIntensityTooFarOff;
	//double vThresholdCorrection;
	//double vNegitiveThresholdCorrection;
	WORD vRedBoxCornerX[cMaximumRedBoxes];
	WORD vRedBoxCornerY[cMaximumRedBoxes];
	WORD vRedBoxWidth[cMaximumRedBoxes];
	WORD vRedBoxHeight[cMaximumRedBoxes];
	WORD vRedBoxInspection[cMaximumRedBoxes]; //this is a bit map, one bit for each inspection that found this red box.  On the Rejects display, it can just show the red boxes that were found by an inspection

	//constructor
	CImageData();
	virtual ~CImageData();

	//methods
	void MakeGreyImage(DWORD TempSize);
	void MakeRedImage(DWORD TempSize);
	void CopyImage(CImageData *TempSourceImage,DWORD TempSize);
};

#endif // !defined(AFX_IMAGEDATA_H__8004A6B0_F7A2_11D4_AD44_00500466E305__INCLUDED_)
