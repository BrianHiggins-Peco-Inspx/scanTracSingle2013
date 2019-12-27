//ScanTrac Side View Source File
// Inspection.h: interface for the CInspection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSPECTION_H__B28B9521_4BEE_11D6_AFBD_00500466E305__INCLUDED_)
#define AFX_INSPECTION_H__B28B9521_4BEE_11D6_AFBD_00500466E305__INCLUDED_

#include "imgutil.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//declarations for curve fitting algorithm
typedef struct {
        double ftol;       // relative error desired in the sum of squares.
        double xtol;       // relative error between last two approximations.
        double gtol;       // orthogonality desired between fvec and its derivs.
        double epsilon;    // step used to calculate the jacobian.
        double stepbound;  // initial bound to steps in the outer loop.
        double fnorm;      // norm of the residue vector fvec.
        int maxcall;       // maximum number of iterations.
        int nfev;          // actual number of iterations.
        int info;          // status of minimization.
} lm_control_type;

typedef struct
{
	double vTime;
	DWORD vCallsToSquare;
	DWORD vMainCalls;
	DWORD vNumberOfPointsToPlot;
	double vTerms[10];
	int vTerminationReason;
	double vFNorm;
} tCurveFitAnalysisData;


// the subroutine that calculates fvec:
typedef void (lm_evaluate_ftype) (
    double* par, int m_dat, double* fvec, void *data, int *info );

typedef struct {
        double* user_t;
        double* user_y;
        double (*user_func)( double user_t_point, double* par );
} lm_data_type;
//end of declarations for curve fitting algorithm

enum {cSelectInspection,cSelectInspectionForEdit,	cSelectInspectionForCopy, 
	cSelectInspectionForDelete, cSelectInspectionForDisplay, cSelectInspectionForEvaluate, 
	cSelectInspectionForBounds, cSelectInspectionForEjector, cSelectInspectionForThreshold,
	cSelectInspectionForLastPriority};

const WORD cMaximumProfileSize = 512;
const WORD cCheckWeighMaximumSamplesInOneMinute = 2500;

//constants for Latvian Algorithm
// return codes
#define INSPX_OK 						0
#define INSPX_ERROR_MEMORY				-1 
#define INSPX_ERROR_ARG					-2
#define INSPX_ERROR_IN_PLACE			-3
#define INSPX_PACKAGE_NOT_FOUND			-4
#define INSPX_ILLEGAL_IMAGE_SIZE		-5

// number of errors in list excluding "OK" 
#define INSPX_MAX_ERROR_NR	5
// Constants
/// Maximum median and averaging filter length used for background processing 
#define INSPX_MAX_FILTER_LENGTH	15
/// Pixel value used to indicate found contaminant pixels 
#define INSPX_MAX_PIXEL_VALUE 65535
//end of constants for Latvian Algorithm


class CInspection : public CObject  
{
	//For serializing, writing to disk product info
DECLARE_SERIAL( CInspection )
//DECLARE_DYNCREATE(CInspection)
public:
	//variables
	BYTE vTakeDifferenceFromMedian; //Bones2 algorithm
	BYTE vBonesInspection;
	BYTE vFillBackground;
	BYTE vDoContaminant22Algorithm;
	BYTE vSwungBaffleTolerance;
	BYTE vReferenceToTop;
	WORD vReferenceToBottom;
	BYTE vInvertEjectLogic;
	BYTE vInspectionType;
	BYTE vEnable;
	CString vName;
	double vROITop;
	double vROIBottom;
	double vROILeft;
	double vROIRight;
	WORD vROIPixelTop;
	WORD vROIPixelBottom;
	WORD vROIPixelLeft;
	WORD vROIPixelRight;
	BYTE vThreshold;
	DWORD vSize;
	WORD vSizeToReadInOldStream;
	WORD vMaximumSize;
	WORD vMinimumNumberOfItemsRequired;
	WORD vMaximumNumberOfItemsRequired;
	WORD vMinimumWidth;
	WORD vMaximumHeightAboveBottom;
	BYTE vEjectorVariableToReadInOldByte;
	DWORD vEjector;
	BYTE vEnhanceMethod;
	BYTE vSubtractAverage;
	BYTE vPerformGlassAlgorithm;

	BYTE vAddHistogram;
	BYTE vAddStructure;
	BYTE vFillInLightSpots;
	BYTE vMaximumDrop;
	BYTE vFollowBottomUpAndDown;
	BYTE vTryThresholdsDownToThreshold;
	BYTE vHasDensityLowerThreshold;
	BYTE vHasDensityUpperThreshold;
	BYTE vAllowChangingAdvancedOptions;
	BYTE vHasAMinimumSize;
	BYTE vShowRejects;
	BYTE vSaveRejects;

	BYTE vTrimEdgesOfJar;
	BYTE vTakeDerivative;
	BYTE vFindSpotInSubROI;
	BYTE vPixelsStepToNextROI;
	BYTE vFindDarkSpotOnLine;

	BYTE vDesensitizeEdgePixels;
	BYTE vDesensitizeIrregularArea;

	BYTE vDoImageFactors;
	BYTE vDilationCount;
	BYTE vErodeCount;
	BYTE vIPContaminant;
	BYTE vErodeCountAtStart;
	BYTE vDilationCountAtEnd;
	BYTE vErodeCountAtEnd;
	BYTE vPreSmoothImage;
	BYTE vTakeDifferenceFromMedianFilter;

	float vSetupHistoryMinimumGood;
	DWORD vSetupHistoryMinimumGoodCount;
	float vSetupHistoryMinimumGoodAverage;
	float vSetupHistoryMinimumGoodStandardDeviation;

	float vSetupHistoryMaximumBad;
	DWORD vSetupHistoryMaximumBadCount;
	float vSetupHistoryMaximumBadAverage;
	float vSetupHistoryMaximumBadStandardDeviation;
	double vSetupHistoryQualityFactor;

	BYTE vSetupHistoryAutoThreshold;
	bool vCollectingDensityAverage;
	bool vFindBadIntensity;
	bool vFindGoodIntensity;

	BYTE vRecentPuntRemovalBottomHeight;
	BYTE vRecentPuntRemovalNumberOfPixelsCopied;
	BYTE vRecentPuntRemovalPuntHeightProfile[512];
	//density variables
	float vDensityThresholdUpper;
	float vDensityThresholdLower; 
	float vSetupHistoryDensityAutoThresholdUpper;
	float vSetupHistoryDensityAutoThresholdLower;
	float vSetupHistoryMinimumValue;
	float vSetupHistoryMaximumValue;
	float vSetupHistoryBadMinimumValue;
	float vSetupHistoryBadMaximumValue;

	BYTE vRemoveJarPunt;

	double vCheckWeighMFactor;  //for y=mx+b calibration of density to weight
	double vCheckWeighBFactor;
	CString vCheckWeighUnits;
	double vCheckWeighMaximumGoodWeight; //heavy sample weight
	double vCheckWeighMinimumGoodWeight; //light sample weight
	double vCheckWeighNominalWeight;
	double vCheckWeighTrendAlarmMaximum;
	double vCheckWeighTrendAlarmMinimum;
	CString vInspectionNameForWeightTrendingName;
	CInspection *vInspectionNameForWeightTrendingPointer;

	//variables for Latvian Algorithm
	WORD *_ImageTmp1[2];
	WORD *_ImageTmp2[2];
	WORD *_ImageCroppedT[2];
	short *_ImageAlog[2]; 
	short *_ImageAlogT[2]; 
	short *_ImageAlog0[2]; 
	short *_ImageAb[2];
	short *_ImageAbh5[2];
	short *_ImageAbv5[2];
	BOOL  *_ImageAd[2];
	BOOL  *_ImageAd1[2];
	BOOL  *_ImageAd2[2];

	int *_MinIndexesRows[2];
	int *_MinIndexesColumns[2];
	int *_MinIndexesColumnsUp[2];
	int *_MinIndexesColumnsDown[2];
	WORD *_MinimaRows[2];
	WORD *_MinimaColumns[2];
	WORD *_MinimaColumnsUp[2];
	WORD *_MinimaColumnsDown[2];

	// thresholds calculated from sample 'Healthy Choice Good' images
	short _MinValuesGoodH[7]; 		//  filter lengths 5,7,9,3,-,5,5a
	double _MultipliersH[7];
	short _ThresholdsH[7];
	double _varH[7];	// statistical variances of _ThresholdsH[i];
													 
	short _MinValuesGoodV[7];			//  filter lengths 5,9,13,3,-,5,5a
	double _MultipliersV[7]; 	
	short _ThresholdsV[7];
	double _varV[7];	// statistical variances of _ThresholdsV[i];

	/// Maximum image size on one axis
	WORD _maxImageSize;	

	/// Maximum number of pixels in image
	DWORD _maxPixels;

	/// Threshold used to separate package from empty background (for the WORD range)
	WORD _backgroundThreshold;

	/// Minimum X size of the package image after trimming white margins 
	WORD _minSizeX;

	/// Minimum Y size of the package image after trimming white margins 
	WORD _minSizeY;

	/// Number of side rows in trimmed image where the package edges are searched for 
	WORD _rowsEdge;

	/// Flag indicating whether package edges should be found and masked before dealing with package contents
	BOOL _processEdges;
	// ----------------------------

	// local buffers
	WORD *ImageTmpL1[2];
	WORD *ImageTmpL2[2];
	CString inspxErrorTexts[7];
	//end of Latvian algorithm variables

	BYTE vIgnoreEdgePixels;
	BYTE vPixelLookAhead;
	BYTE vLookForDarkSpots;
	BYTE vMinimumLength;
	BYTE vOverlap;
	double vCheckWeighOffset; //uses location vSpareDoubleA[0] in stream before version 153
	double vMinimumBrightnessAceptable;

	BYTE vSpareByteA[13]; //not in stream anymore, just to read in old stream formats and throw away
	DWORD vSpareDWordA[14]; //not in stream anymore, just to read in old stream formats and throw away
	int vSpareIntA[14]; //not in stream anymore, just to read in old stream formats and throw away
	double vSpareDoubleA[14]; //not in stream anymore, just to read in old stream formats and throw away
	BYTE vWhiteOutAnythingTouchingTheTop;

// SPARES	
	CString vSpareStringName; 
	double vOriginalImageFactor; //vSpare not used anymore
	double vEnhancedImageFactor; //vSpare not used anymore
	double vSpareDoubleI2;
	double vSpareDoubleI3;
	double vSpareDoubleI4;
	double vSpareDoubleI5;

	BYTE vSpareByteI3;
	BYTE vSpareByteI4;
	BYTE vSpareByteI5;
	BYTE vSpareByteI6;
	BYTE vSpareByteI7;
	BYTE vSpareByteI8;
	BYTE vSpareByteI9;
	BYTE vSpareByteI10;

	WORD vSpareWordI2;
	WORD vSpareWordI3;
	WORD vSpareWordI4;
	WORD vSpareWordI5;
	WORD vSpareWordI6;

	DWORD vSpareDWordI2;
	DWORD vSpareDWordI3;
	DWORD vSpareDWordI4;
	DWORD vSpareDWordI5;
	DWORD vSpareDWordI6;

	int vSpareIntI1;
	int vSpareIntI2;
	int vSpareIntI3;
	int vSpareIntI4;
	int vSpareIntI5;

	CString vSpareCStringI1;
	CString vSpareCStringI2;
	CString vSpareCStringI3;
	CString vSpareCStringI4;
	CString vSpareCStringI5;

	//constructor
	CInspection();
	virtual ~CInspection();

	//methods
	virtual void Serialize(CArchive& TempArchive);
	virtual void CopyInspection(CInspection *TempInspectionToCopy, BYTE TempOverScanMultiplier);
	virtual CString MakeSetupDisplayString(bool TempReferenceEnabled, BYTE TempOverScanFactor, 
		CString TempEjectorNameString, CString TempUnitsString);
	void SetupInspection(double TempTop, double TempBottom, double TempLeft, double TempRight,BYTE TempOverScanMultiplier); //parameters passed in inches, not pixels
	void SetupInspectionInPixels(WORD TempTop, WORD TempBottom, WORD TempLeft, WORD TempRight,BYTE TempOverScanMultiplier); //parameters passed in pixels
	void SetROITop(double TempTop);
	void SetROIBottom(double TempBottom);
	void SetROILeft(double TempLeft,BYTE vOverScanMultiplier);
	void SetROIRight(double TempRight,BYTE vOverScanMultiplier);
	void ClearInspectionHistory();
	void SetDefaultROI(WORD TempTop, WORD TempBottom, WORD TempLeft, WORD TempRight, BYTE TempOverScanMultiplier);
	void SetDensityLowerThreshold(double TempThreshold);
	CString GetInspectionTypeString();
	void CalculateLineDerivative(BYTE *TempImage, BYTE *TempScratchImage, WORD TempSizeX, WORD TempSizeY, double TempImageBrightness);
	void LookForDarkSpotsOnLine(BYTE *TempImage, Image *TempScratchImage, WORD TempSizeX, WORD TempSizeY, WORD TempLeft, WORD TempRight, WORD TempBottom, WORD TempTop, BYTE TempThreshold);
	void LookForDarkSpotsOnLineInOneDirection(BYTE *TempImage, BYTE *TempScratchImage, WORD TempSizeX, WORD TempSizeY, WORD TempLeft, WORD TempRight, WORD TempBottom, WORD TempTop, BYTE TempThreshold);
	void FillInLightSpots(BYTE *TempImage, WORD TempSizeX, WORD TempSizeY, BYTE TempLightSpotsType);
	//void CalculateLineDerivative(BYTE *TempImage, BYTE *TempScratchImage, WORD TempSizeX, WORD TempSizeY, BYTE TempRequire2InARow, BYTE TempLookAhead);
 void GenerateLinearBestFit(BYTE *TempData, WORD TempDataCount);
	void SetupInspectionROIFromReference(WORD TempTop, WORD TempBottom, WORD TempLeft, WORD TempRight, WORD TempLowerRimHeight, BYTE TempOverScanMultiplier);
	void LeastSquaresCurveFit( int m, int n, double* x, double* fvec, double ftol, double xtol,
               double gtol, int maxfev, double epsfcn, double* diag, int mode,
               double factor, int *info, int *nfev, 
               double* fjac, int* ipvt, double* qtf,
               double* wa1, double* wa2, double* wa3, double* wa4,
               //lm_evaluate_ftype *evaluate, //lm_print_ftype *printout,
               void *data );
	void EvaluateLeastSquaresFitFunction( double* par, int m_dat, double* fvec, void *data, int *info );
	double lm_enorm( int n, double *x );
	void lm_qrfac(int m, int n, double* a, int pivot, int* ipvt, double* rdiag, double* acnorm, double* wa);
	void lm_lmpar(int n, double* r, int ldr, int* ipvt, double* diag, double* qtb,
              double delta, double* par, double* x, double* sdiag, double* wa1, double* wa2);
	void lm_qrsolv(int n, double* r, int ldr, int* ipvt, double* diag, double* qtb, double* x, double* sdiag, double* wa);
	void lm_initialize_control( lm_control_type *control );
	void lm_minimize( int m_dat, int n_par, double* par, void *data, lm_control_type *control);
	//double CurveFitFillInLightSpots(BYTE *TempImage, WORD TempSizeX, WORD TempSizeY);
	double ReturnCurveFitLine(BYTE *TempImage, WORD TempSizeY, tCurveFitAnalysisData *vCurveFitAnalysisData, int TempNumberOfTerms);
	double ClipToCurveFit(BYTE *TempImage, WORD TempSizeX, WORD TempSizeY);
	void TrimEdgesOfJarAtROI(BYTE *TempImage, WORD TempSizeX, WORD TempSizeY, bool TempSetBackgroundWhite, 
		WORD TempLeftSide, WORD TempRightSide);
	void RemoveJarPunt(BYTE *TempImage, WORD TempSizeX, WORD TempSizeY, WORD TempLeftSide, WORD TempRightSide, 
		double TempImageBrightness, BYTE TempJustMarkPunt);
	void TakeLineDerivative(BYTE *TempImage, BYTE *TempScratchImage, WORD TempSizeX, WORD TempSizeY, double TempImageBrightness, 
		BYTE TempInspectionThreshold);
	void FindSpotInSubROI(BYTE *TempImage, Image *TempScrachImage, WORD TempSizeX, WORD TempSizeY, BYTE TempInspectionThreshold, tImage *TempDesensitizeImage, bool TempFindAllSizeSpots,
																	 WORD TempROIBottom, WORD TempROITop, WORD TempROILeft, WORD TempROIRight);
	//void TrimEdgesOfJarAtROIOfRandomShape(BYTE *TempImage, WORD TempSizeX, WORD TempSizeY, bool TempSetBackgroundWhite, 
	//	WORD TempLeftSide, WORD TempRightSide);
	void FindDarkSpotOnLine(BYTE *TempImage, BYTE *TempScratchImage, WORD TempSizeX, WORD TempSizeY, BYTE TempInspectionThreshold);
	void FindBonesInspection(BYTE *TempImage, Image *TempScratchImage, WORD TempSizeX, WORD TempSizeY, WORD TempLeft, WORD TempRight, WORD TempBottom, WORD TempTop);
	void FindBonesInOneDirection(BYTE *TempImage, BYTE *TempScratchImage, WORD TempSizeX, WORD TempSizeY, WORD TempLeft, WORD TempRight, WORD TempBottom, WORD TempTop);
	void ErodeEdgesOfContainer(tImage *TempImage, WORD TempSizeX, WORD TempSizeY);
	void MakeVariablityImage(tImage *TempImage, Image *TempScratchImage, WORD TempSizeX, WORD TempSizeY, BYTE TempGoToStep);
	//void FillBackground(BYTE *TempImage, WORD TempSizeX, WORD TempSizeY);
	void FillBackground(BYTE *TempImage, WORD TempSizeX, WORD TempSizeY);
	void TakeDifferenceFromMedianSingleMatrix(Image *TempImage, WORD TempSizeX, WORD TempSizeY, BYTE TempMatrixSizeX, BYTE TempMatrixSizeY);
	void TakeDifferenceFromMedian(BYTE *TempImage, Image *TempScrachImage, WORD TempSizeX, WORD TempSizeY, BYTE *TempOriginalImage, BYTE TempThreshold);
	void FindDarkSpotSingleMatrix(Image *TempImage, WORD TempSizeX, WORD TempSizeY, BYTE TempMatrixSize, BYTE TempThreshold, BYTE *TempMaskImage);
	//Latvian Algorithm functions
	// In all declarations, "Image" is represented in memory by columns (SizeX related to height=number of rows, SizeY related to width=number of columns).
	// 'Horizontal' direction mentioned in comments, names and descriptions is related to X axis and SizeX.
	// Images passed as BYTE values are converted to WORD values before further internal processing and returned to BYTEs before returning. 

	/**
	 * Calculation of thresholds from one "Healthy Choice Good" image (pixels represented by bytes)
	 * of "Healthy Choice" package without contaminants.
	 */
	int InspxContaminant22ThresholdCalculationB(BYTE *Image, WORD SizeX, WORD SizeY, int k, WORD n);
	/**
	 * Calculation of thresholds from one "Healthy Choice Good" image (pixels represented by words)
	 * of "Healthy Choice" package without contaminants.
	 */
	int InspxContaminant22ThresholdCalculationW(WORD *Image, WORD SizeX, WORD SizeY);

	/**
	 * Detection of contaminants in "Healthy Choice" images.
	 * Image pixels are represented by bytes.
	 */
	int InspxContaminant22InspectionB(BYTE *Image, WORD SizeX, WORD SizeY, BYTE tTh, int k);
	/**
	 * Detection of contaminants in "Healthy Choice" images.
	 * Image pixels are represented by words.
	 */
	int InspxContaminant22InspectionW(WORD *Image, WORD SizeX, WORD SizeY);

	// get/set threshold values used by the library (vectors of 7 thresholds for each axis).
	void InspxContaminant22GetThresholds(short *ThresholdsX, short *ThresholdsY);
	void InspxContaminant22SetThresholds(short *ThresholdsXnew, short *ThresholdsYnew);
	/*
	// get/set max image size on one axis (used for memory allocation). Default value = 384.
	WORD InspxContaminant22GetMaxImageSize();
	void InspxContaminant22SetMaxImageSize(WORD SizeMax);

	// get/set max total image size in pixels (used for memory allocation). Default value = 384x384.
	DWORD InspxContaminant22GetMaxPixels();
	void InspxContaminant22SetMaxPixels(DWORD PixelsMax);
	*/
	// defines used image sizes and allocates memory buffers for library operation. 
	int InspxContaminant22CreateBuffers(WORD SizeX, WORD SizeY);
	// frees memory buffers after use of the library. 
	void InspxContaminant22FreeBuffers();

	// get/set threshold used for cropping white margins. Default value = 45000.
	WORD InspxContaminant22GetBackgroundThreshold();
	void InspxContaminant22SetBackgroundThreshold(WORD ThresholdNew);

	// get/set min package image size on X axis (used for checking cropping results). Default value = 230.
	WORD InspxContaminant22GetMinSizeX();
	void InspxContaminant22SetMinSizeX(WORD SizeXMinNew);

	// get/set min package image size on Y axis (used for checking cropping results). Default value = 280.
	WORD InspxContaminant22GetMinSizeY();
	void InspxContaminant22SetMinSizeY(WORD SizeYMinNew);

	// get/set number of rows/columns used for detection of package edges (after image cropping). Default value = 50.
	WORD InspxContaminant22GetRowsEdge();
	void InspxContaminant22SetRowsEdge(WORD RowsEdgeNew);

	// get/set ProcessEdges flag. Default is true(1).
	BOOL InspxContaminant22GetProcessEdges();
	void InspxContaminant22SetProcessEdges(BOOL ProcessEdgesNew);

	// get description of error returned from 'ThresholdCalculation' or 'Contaminant22Inspection' routines. 
	CString InspxGetErrorText(int ErrorCode);

	//inspxImageServices.c
	void inspxTransposeI16(short *imageOut, short *imageIn, int rowsIn, int columnsIn); 
	void inspxTransposeU16(WORD *imageOut, WORD *imageIn, int rowsIn, int columnsIn); 
	void inspxTransposeU8(unsigned char *imageOut, unsigned char *imageIn, int rowsIn, int columnsIn);

	void inspxCopyU16(WORD *imageOut, WORD *imageIn, int numberOfPixels);
	void inspxImageB2W(WORD *imageOut, BYTE *imageIn, int numberOfPixels);
	void inspxImageW2B(BYTE *imageOut, WORD *imageIn, int numberOfPixels);
	void inspxImageLog(short *imageOut, WORD *imageIn, int numberOfElements);
	void inspxImageMinMax(short *imageIn, int numberOfPixels, short *minValue, short *maxValue);
	void inspxImageMinRows(WORD *minVectorOut, int *indOut, WORD *imageIn, int rows, int columns);
	void inspxImageMinColumns(WORD *minVectorOut, int *indOut, WORD *imageIn, int rows, int columns);
	void inspxThresholdNegative(BOOL *imageOut, int *numberPixelsFound, short *imageIn, int numberOfPixels, short threshold);
	void inspxImageAND(BOOL *image1, BOOL *image2, int numberOfPixels);
	void inspxImageOR(BOOL *image1, BOOL *image2, int numberOfPixels);
	int inspxMarkImage(WORD *imageOut, WORD *imageIn, BOOL *imageFound, int numberOfPixels);

	//inspxBackgroundFilters.c
	void inspxRemoveTrendLine(short *outputVector, short *inputVector, int numberOfElements, int filterLength);
	void inspxRemoveBackgroundLine(short *outputVector, short *inputVector, int numberOfElements, int filterLength);
	void inspxRemoveBackgroundTrendByRows(short *imageOut, short *imageIn, int numberOfRows, int numberOfColumns, int filterLength);
	void inspxRemoveBackgroundImageByRows(short *imageOut, short *imageIn, int numberOfRows, int numberOfColumns, int filterLength);

	//inspxMaskPackage.c
	BOOL inspxPackageLimits(int *colLeft, int *colRight, int *rowUp, int *rowDown, WORD *imageIn, int rows, int columns, int k);
	void inspxEdgePointsLeftRight(WORD *imageIn, int rows, int columns, int rowUp, int rowDown, int k);
	void inspxExtractROI(WORD *imageOut, WORD *imageIn, int rowsIn, int columnsIn, int x1, int x2, int y1, int y2);
	void inspxReplaceROI(WORD *imageOut, WORD *imageIn, int rows, int columns, WORD *imageROI, int x1, int x2, int y1, int y2);

	//inspxDetect.c
	int inspxProcessEdges(WORD *imageTmp1, int *colLeft, int *colRight, int *rowUp, int *rowDown, WORD *imageTmp2, WORD *imageIn, int rowsIn, int columnsIn, int k);
	int inspxUpdateThresholds(WORD *imageIn, int rowsIn, int columnsIn, BOOL processEdges, WORD n);
	int inspxDetect(WORD *imageOut, int *nrDetected, WORD *imageIn, int rowsIn, int columnsIn, BOOL maskEdges, BYTE sTh, int k);

	//end of Latvian algorithm functions
	double ConvertToWeight(double TempDensity);
	double ConvertWeightToBrightness(double TempWeight);
	virtual double GetSensitivity(BOOL TempUsePassedThreshold, double TempPassedThreshold);
	virtual double AnalyzeImage(BYTE *TempImageIn, BYTE *TempImageReturn, BYTE TempThreshold);
	//virtual void DilateImage(Image *TempInputImage);
	//void DilateImage(Image *TempInputImage, Image *TempScratchImage);
	bool ThereAreAnyBlobsAtValue(BYTE TempThreshold, Image *TempImage, WORD TempBottom, WORD TempLeft, WORD TempHeight, WORD TempWidth);
	void DilateImage(Image *TempImageIn, Image *TempScratchImage);
	void ErodeImage(Image *TempImageIn, Image *TempScratchImage);
	bool IsIVBagFlooded(Image *TempImage, WORD *TempDelta, WORD *TempBottom, WORD TempSizeX, WORD TempSizeY);
	void WhiteOutAnythingTouchingTheTop(BYTE *TempImage, WORD TempSizeX, WORD TempSizeY);
	void WhiteOutThisPixelAndOnesTouchingBelow(BYTE *TempImage, WORD TempPixelX, WORD TempPixelY, WORD TempSizeX, WORD TempSizeY);

};

#endif // !defined(AFX_INSPECTION_H__B28B9521_4BEE_11D6_AFBD_00500466E305__INCLUDED_)
