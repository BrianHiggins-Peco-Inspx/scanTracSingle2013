//ScanTrac Side View Source File
// ITIPCDig.h: interface for the CITIPCDig class.
//
//////////////////////////////////////////////////////////////////////
	#include "Vixni.h"  //ifc camera structure defines
	#include "imgutil.h"
#ifdef UseIFCDLLs
	//#include "Camera.h"
	#include "mvtools.h"
#endif

#include "Product.h"
#include "ImageData.h"
#include "Inspection.h"
#include "InspectionCheckWeigh.h"
#include "Container.h"

#include <afxmt.h>  //for syncronization objects like critical sections

#if !defined(AFX_ITIPCDIG_H__A511D620_05C2_11D4_ABE2_00500466E305__INCLUDED_)
#define AFX_ITIPCDIG_H__A511D620_05C2_11D4_ABE2_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#ifdef IFC59Driver
const BYTE cFramesInAquisitionBuffer = 4;  //was 8  allows an image to be 1 meg pixels, as 4 meg image buffer in PC-Dig
//#else
//const BYTE cFramesInAquisitionBuffer = 64;
//#endif

//const BYTE cFramesToGrabBufferSize = 1;
const BYTE cBitsPerPixel = 8;
const BYTE cNumberOfPatterns = 8;
const BYTE cMaxNumberOfLastRejectImages = 200;
const WORD cMaximumBodyTriggerWidthData = 500;
const BYTE cSizeOfBackupBuffers = 20;

#ifndef UseIFCDLLs
	typedef int Erflag;
	const BYTE OL_ABS = 0;
	const BYTE EDGE_IGNORE = 0;
/*
	typedef int HIFCGRAB;
	const BYTE IM_BYTE = 0;
	const BYTE IM_OK = 0;
	const BYTE IM_BMP_FILE = 0;
	const BYTE IM_AUTO_FILE = 0;
	const BYTE IFC_INFINITE_FRAMES = 0;
	const BYTE IFC_WAIT_NEWER_FRAME = 0;
	const BYTE IM_TOO_SMALL = 1;
	const BYTE IM_FLOAT = 1;
	*/
	const BYTE MVT_VERTICAL = 0;
	const BYTE MVT_CF_PARENT_REF = 0;
	const BYTE MVT_CF_DEFAULT = 0;
	const BYTE MVT_NOCHANGE = 0;
	const BYTE AREA_BBOX = 0;
	const BYTE AREA_BBOX_CENT_PERIM = 1;

	class MVT_LINE_RESULT : public CObject  
	{
		public:
		//variables
		BYTE p1x;
		BYTE p2x;
		BYTE p3x;
		BYTE p1y;
		BYTE p2y;
		BYTE p3y;

		//Contructor Destructor
		MVT_LINE_RESULT();
		virtual ~MVT_LINE_RESULT();
		
		//methods
		void WriteFile(BYTE *TempPointer,
						DWORD TempHeigth,
						DWORD TempWidth,
						WORD TempBitsPerPixel);
		//	DECLARE_DYNCREATE(MVT_LINE_RESULT)
	};
	/*
	class CImgFile : public CObject  
	{
		public:
		//variables
		BYTE vHolder;

		//Contructor Destructor
		CImgFile();
		virtual ~CImgFile();
		
		//methods
		void WriteFile(BYTE *TempPointer,
						DWORD TempHeigth,
						DWORD TempWidth,
						WORD TempBitsPerPixel);
		//	DECLARE_DYNCREATE(CImgFile)
	};
	class Array : public CObject  
	{
		public:
		//variables
		BYTE vHolder;

		//Contructor Destructor
		Array();
		virtual ~Array();
		
		//methods
		void DoWork();
		//	DECLARE_DYNCREATE(Array)
	};
	class CAM_ATTR : public CObject  
	{
		public:
		//variables
		BYTE vHolder;

		//Contructor Destructor
		CAM_ATTR();
		virtual ~CAM_ATTR();
		
		//methods
		void DoWork();
		//	DECLARE_DYNCREATE(CAM_ATTR)
	};
	class CImgConn : public CObject  
	{
		public:
		//variables
		BYTE vHolder;

		//Contructor Destructor
		CImgConn(BYTE *vBufferPointer,
			WORD vImageSizeX,
			WORD vImageSizeY,
			WORD vBitsPerPixel, 
			HANDLE vWindowHandle);
		virtual ~CImgConn();
		
		//methods
		void DoWork();
		//	DECLARE_DYNCREATE(CImgConn)
	};
	class CPCDig : public CObject  
	{
		public:
		//variables
		BYTE vHolder;

		//Contructor Destructor
		CPCDig();
		virtual ~CPCDig();
		
		//methods
		void DoWork();
		//	DECLARE_DYNCREATE(CPCDig)
	};
	class CICapMod : public CObject  
	{
		public:
		//variables
		BYTE vHolder;

		//Contructor Destructor
		CICapMod();
		virtual ~CICapMod();
		
		//methods
		BYTE InportVal(BYTE TempDirection);
		void UnLockMem(int TempHandle);
		//	DECLARE_DYNCREATE(CICapMod)
	};
	class CICamera : public CObject  
	{
		public:
		//variables
		BYTE vHolder;

		//Contructor Destructor
		CICamera();
		virtual ~CICamera();
		
		//methods
		int Grab(BYTE TempLock, //IFC_GOPT_AUTO_FRAME_LOCK,
				BYTE *TempFrameBuffer,
				WORD TempNumberOfFramesInBuffer,
				WORD TempNumberOfFramesToTake);
		int GrabWaitFrameEx(HIFCGRAB TempID, 
				BYTE **TempFrameBuffer,
				int TempWaitMode,
				unsigned long  TempWaitTime,
				BOOL TempSomething,
				unsigned long *TempNumberOfLinesGot);
		void Freeze(HIFCGRAB TempGrabID);
		void Freeze();
		//	DECLARE_DYNCREATE(CICamera)
	};
	class Image : public CObject  
	{
		public:
		//variables
		BYTE *pix;
		WORD x;
		WORD y;
		WORD dx;
		WORD dy;
		WORD ypitch;

		//Contructor Destructor
		vImage(BYTE vType, WORD vSizeX, 
					WORD vSizeY);
		virtual ~Image();
		
		//methods
		void DoWork();
		//	DECLARE_DYNCREATE(Image)
	};
	Image *im_create(int vType, int vSizeX, 
					int vSizeY);
	//Array *im_create(int vType, int vSizeX, 
	//				int vSizeY);
	Erflag im_delete(Image *TempImage);
	Erflag im_delete(Array *TempArray);

	Erflag im_file_get_size(CString TempNameString, 
		DWORD TempFileType, int *TempWidth, int *TempHeight, 
		int *TempBytesPerPixel, int *TempType);
	
	Erflag im_file_read(Image *TempImageIn, CString TempNameString, int TempType);

	CImgFile *IfxCreateImgFile(CString TempNameString);
	double GetSystimeMicrosecs();
	double GetSystimeMillisecs();

	Erflag im_get_line_from_img(
		Image *TempImageIn,
		int TempX1,
		int TempX2, 
		int TempY1,
		int TempY2, 
		bool TempMode,
		Array *TempLineOut);

	Image *im_rroi(
		Image *TempImageIn,
		int TempBottom,
		int TempLeftSide,
		int TempHeight,
		int TempWidth);

	Image *im_duplicate(Image *ImageIn);

	*/
	class MVT_BLOB : public CObject  
	{
		public:
		//variables
		DWORD area;
		WORD xcent;
		WORD ycent;
		WORD height;
		WORD width;
		WORD xmin;
		WORD ymin;

		//Contructor Destructor
		MVT_BLOB();
		virtual ~MVT_BLOB();
		
		//methods
		//void DoWork();
		//	DECLARE_DYNCREATE(MVT_BLOB_RESULTS)
	};

	class MVT_BLOB_RESULTS : public CObject  
	{
		public:
		//variables
		double num_blobs_found;
		MVT_BLOB *blobs[5];

		//Contructor Destructor
		MVT_BLOB_RESULTS();
		virtual ~MVT_BLOB_RESULTS();
		
		//methods
		void DoWork();
		//	DECLARE_DYNCREATE(MVT_BLOB_RESULTS)
	};
	const BYTE CONNECT8 = 1;
	class MVT_BLOB_PARAMS : public CObject  
	{
		public:
		//variables
		double min_area;
		BYTE blob_color;
		double calc_results;
		WORD min_height;
		DWORD max_area;
		BYTE connected;

		//Contructor Destructor
		MVT_BLOB_PARAMS();
		virtual ~MVT_BLOB_PARAMS();
		
		//methods
		void DoWork();
		//	DECLARE_DYNCREATE(MVT_BLOB_PARAMS)
	};
	Erflag mvt_extrema(Image *TempImage,
		int TempPixelIntensity,
		int TempLevel,
		int TempMinimumPixelRunLengthX,
		int TempEdgeFlag[4],
		double *TempX, 
		double *TempY);

	Erflag mvt_mirror_y(Image *TempImageIn);

	Erflag mvt_blob_get_perim(MVT_BLOB_RESULTS *TempResults, WORD TempBlobNumber);

	Erflag mvt_dilate_rect(Image *TempImageIn, WORD TempDilateTimesX, WORD TempDilateTimesY, WORD TempHandleEdges);
					
	Erflag mvt_erode_rect(Image *TempImageIn, WORD TempErodeTimesX, WORD TempErodeTimesY, WORD TempHandleEdges);

	Erflag mvt_average(Image *TempImageIn);

	Erflag mvt_sobelx(Image *TempImageIn,
		Image *TempImageOut,BYTE TempStle);

	Erflag mvt_dilate(Image *TempImageIn,
		Image *TempImageOut);

	Erflag mvt_sobely(Image *TempImageIn,
		Image *TempImageOut,BYTE TempStle);

	Erflag mvt_blur(Image *TempImageIn,
		DWORD Temp1, DWORD Temp2, float Temp3, DWORD Temp4);

	Erflag mvt_sobelxy(Image *TempImageIn,
		Image *TempImageOut,BYTE TempStle);

	Erflag mvt_prewittx(Image *TempImageIn,
		Image *TempImageOut,BYTE TempStle);

	Erflag mvt_prewitty(Image *TempImageIn,
		Image *TempImageOut,BYTE TempStle);

	Erflag mvt_prewittxy(Image *TempImageIn,
		Image *TempImageOut,BYTE TempStle);

	Erflag mvt_erode(Image *TempImageIn,
		Image *TempImageOut);

	Erflag mvt_convolve_byte(Image *TempImageIn,
		int TempRows, int TempColumns, int *TempKernel, 
		int TempShift, int TempOffset,int TempEdgeFlag);

	Erflag mvt_invert(Image *TempImage);

	Erflag mvt_threshold(Image *TempImage, BYTE TempThreshold,
		BYTE TempSetUp,BYTE TempSetDown);

	Erflag mvt_threshold_down(Image *TempImage, BYTE TempThreshold,
		BYTE TempSetUp);

	Erflag mvt_caliper_od_array(
		Array *TempLineIn,
		int TempThreshold,
		double *TempWidth,
		double *TempXIn,
		double *TempYIn);

	int mvt_edge_find_image(Image *TempImageIn,
			int TempDirection,
			int TempThreshold,
			bool TempColor,
			DWORD TempReference,
			MVT_LINE_RESULT *TempLineIn);  //false gave bottom of cap

	Erflag mvt_blob_find(Image *TempImageIn,
		MVT_BLOB_PARAMS *TempBlobParams, 
		MVT_BLOB_RESULTS *TempBlobResults, 
		DWORD coord_flag);

	double mvt_blob_get_area(MVT_BLOB_RESULTS *TempBlobResults, int TempIndex);

	MVT_BLOB_PARAMS *mvt_blob_create_params(Erflag *TempErrorFlag);

	MVT_BLOB_RESULTS *mvt_blob_create_results(
		DWORD TempSize, 
		bool TempCanGrow, 
		Erflag *TempErrorFlag);

	void mvt_blob_delete_params(MVT_BLOB_PARAMS *TempBlobParameters);
	void mvt_blob_delete_results(MVT_BLOB_RESULTS *TempBlobResults);
#endif

class CITIPCDig : public CObject  
{
	DECLARE_SERIAL( CITIPCDig )
public:
	//variables
				//1/28/2010
	bool vDoBottomReferencing;
	CString vImageTypeString;
	BYTE vImageType;
	bool vALearnedImageHasChanged;
	WORD vNumberOfImagesInLastAddToLearn;
	WORD vAutoImproveStatisticsCount;
	WORD vAutoImproveStatisticsCountTo100;
	WORD vAutoImproveStatisticsCountEjectedIn100;
	WORD vAutoImproveStatisticsAddedToLearnCount;

	bool vUsingOriginalLearnTemporarily;
	int Kernel3X3[9];
	int Kernel5X5[26];
	int Kernel7X7[49];
	double vContainerWidthMaximum;
	double vContainerWidthMinimum;
	double vContainerWidthAverage;
	double vContainerWidthStandardDeviation;
	WORD vContainerWidthIndex;
	WORD vContainerWidthSamples;
	double vContainerWidthData[cMaximumBodyTriggerWidthData];

	bool vUsingLocalSetupDataFile;
	BYTE vFrameFlag;  //this is used in simulating data to simulate a frame or b frame pixel data
	//BYTE vUseMetric;  //copy of configration data one for local use
	BYTE vNumberOfLastRejectImages;//copy of configration data one for local use

	bool vMainDisplayIsDefaultImage;
	WORD vMaximumImageOverlap; //value in lines setup when load a product, this is used everywhere

	CImageData *vLastNRejectImages[cMaxNumberOfLastRejectImages + 1][cMaximumNumberOfCountsDisplayed];
	CCriticalSection vLastNRejectImagesLock;
	BYTE vNextRejectBufferToFill;
	int vRejectBufferShowing;
	WORD vDisplayImageSizeX;
	WORD vDisplayImageSizeY;

	double vReferenceRightStandardDeviation;
	double vReferenceRightAverage;
	double vReferenceLeftStandardDeviation; 
	double vReferenceLeftAverage;
	double vReferenceTopStandardDeviation; 
	double vReferenceTopAverage;
	double vReferenceBottomStandardDeviation; 
	double vReferenceBottomAverage;

	double vLearnedAverageDensity;
	CImageData *vLearnedProductAverageImage;

	WORD vCalibrateBodyTriggerReferenceCount;
	DWORD vAverageCount;
	DWORD vStructureCount;

	bool vCalibrateBodyTriggerReferenceEnabled;
	bool vAverageEnabled;
	bool vStructureEnabled;

	DWORD *vAverageImage;
	CImageData *vStructureImage;
	CImageData *vImproveLearnStructureImage;
	WORD vImproveLearnCount;
	CImageData *vImproveLearnContaminant2StructureImage;
	CImageData *vVoidStructureImage;
	CCriticalSection vContainerCollectionLock;

	BYTE vShowOnlyRealImages;  //local copy of configuration data for local use
	BYTE vImageDisplayOnlyRejects;

	BYTE vPreviousContainerEjected;

	DWORD vLastContainerLargestContaminant;
	WORD vLastTop;
	WORD vLastBottom;
	WORD vLastRight;
	WORD vLastLeft;
	WORD vLastHeight;
	WORD vLastWidth;
	int vLastTopReferenceAdjust;
	int vLastBottomReferenceAdjust;
	int vLastLeftReferenceAdjust;
	int vLastRightReferenceAdjust;
	BYTE vLastFineReferenceAdjust;
	DWORD vLastNumberOfBlobs[cMaximumNumberOfInspections];
	DWORD vLastLargestBlobPerimeter[cMaximumNumberOfInspections];
	double vLastInspectonProcessingTime[cMaximumNumberOfInspections];
	double vLastDensityValue[cMaximumNumberOfInspections + 1];
	double vLastMinimumGoodMaximumBad;
	double vDensityMinimumValue;
	double vDensityMaximumValue;
	double vDensityAverage;
	double vDensityStandardDeviation;
	double vLastCurveFitTime;
	double vLastGlassInspectionTime;
	double vLastSmoothTime;
	double vLastProcessTime;
	double vLastEdgeAdjustTime;
	double vLastFindEdgesTime;
	double vLastReferencePositionTime;
	double vLastEnhanceTime;
	double vLastDisplayTime;
	double vLastScreenPaintTime;
	double vLastCopyBufferTime;
	double vLastCopyToMainTime;
	double vLastFindContaminantsTime;
	//double vLastFindGlassContaminantsTime;
	double vLastFindVoidsTime;
	double vLastFindMedianTime;
	double vLastFindDensityTime;
	double vLastFindBlobsTime;
	double vLastFindTime;
	double vLastOriginalImageIntensity;
	double vLastImageIntensityAdjust;
	//double vLastFinalImageIntensity;
	bool vLastImageIntensityTooFarOff;
	//9/30/2009
	//BYTE vNumberOfEjectors;
	WORD vLastRedBoxCornerX[cMaximumRedBoxes];
	WORD vLastRedBoxCornerY[cMaximumRedBoxes];
	WORD vLastRedBoxWidth[cMaximumRedBoxes];
	WORD vLastRedBoxHeight[cMaximumRedBoxes];
	WORD vLastRedBoxInspection[cMaximumRedBoxes];

	bool vDriftCompensationLocked;
	//double vLastThresholdCorrection;
	//double vLastNegitiveThresholdCorrection;

	double vGridLineDistanceX;
	double vGridLineDistanceY;
	MVT_BLOB_PARAMS *vBlobParameters;
	MVT_BLOB_RESULTS *vBlobResults;
	DWORD vImageAquisitionTime;
	bool vHaveRealImage;
	bool vProcessNextImage;
	BYTE *vLastBufferFilled;
	CImageData *vNextImageToDisplay;

	CCriticalSection vNextImageToDisplayLock;
	BYTE *vReferenceAdjustBuffer;

	BYTE *vMainImageDisplayBuffer;
	BYTE *vMainImagePseudoColorDisplayBuffer;
	WORD vMainDisplayImageSizeX;
	WORD vMainDisplayImageSizeY;
	WORD vMainDisplayImageOffsetX;
	WORD vMainDisplayImageOffsetY;
	DWORD vMainDisplayImageSize;

	bool vHaveImage;
	CICamera *vCamera;
	CICapMod *vCaptureModule;
	//CPCDig *vPCDigBoard;
	CImgConn *vImage;
	CAM_ATTR vCameraAttributes;
	HWND  vWindowHandle;
	int vMemoryLock;
	BYTE *vImageBuffer;	
	BYTE *vImageBufferWithNoLines;	
	CImageData *vOriginalImage;
	CImageData *vImageToDisplayOnMainMenu;
	CImageData *vProcessManipulationImage;
	CImageData *vProductAverageImage;
	CImageData *vProductAverageGImage;
	Image *vProductBackgroundImage;
	CImageData *vProductHistogramImage;
	//CImageData *vProductGlassHistogramImage;
	CImageData *vProductStructureImage;
	CImageData *vProductContaminant2StructureImage;
	CImageData *vProductContaminant2StructureImageWithNoDilation;
	CImageData *vContaminant2StructureImage;
	//CImageData *vProductGlassStructureImage;
	//CImageData *vProductFactoredStructureImage;
	CImageData *vVoidProductStructureImage;
	//CImageData *vVoidProductFactoredStructureImage;
	CImageData *vClipboardImage;
	
	bool vShowAverageMaskedWithHistogram;
	WORD vOriginalBufferSizeX;
	WORD vOriginalBufferSizeY;
	WORD vOldImageSizeX;
	WORD vOldImageSizeY;
	BYTE vBitsPerPixel;
	BYTE vBytesPerPixel;
	BYTE vNumberOfFrames;
	DWORD vImageBufferSize;
	DWORD vOriginalBufferSize;

	BYTE *vAquisitionBuffer;	// Memory space which is the destination of the ITX grab operation
	BYTE *vAquisitionBufferMaximum;

	WORD vAquisitionSizeX;
	WORD vAquisitionSizeXUpTo12Detectors;
	WORD vAquisitionSizeY;
	BYTE vInterlaced;  //1 = not interlaced, 2 = interlaced.  Must have twice the buffer in the X dimension to read in the image as an interlaced image is twice as many lines

	WORD vAquisitionROITop;//these are in pixels where product ones are in units
	WORD vAquisitionROIBottom;

	DWORD vAquisitionBufferSize;
	//DWORD vPixelsInOneAquisitionFrame;
	int vAquisitionMemoryLock;

	Image *vLatestImage;
	Image *vLatestSubImage;
	//Image *vContainerThresholdImage;
	Image *vLatestImageCopy;
	Image *vDesensitizeImage;
	Image *vImageForPseudoColor;

	BYTE *vImageBeforeBrightnessCorrection;
	//Image *vProcessingImageStepB;
	//Image *vProcessingImageStepC;
	//Image *vProcessingImageStepD;
	//Image *vProcessingImageStepE;
	//Image *vProcessingImageStepE2;
	//Image *vProcessingImageStepH;
	//Image *vProcessingImageStepV;
	//Image *vDisplayProcessingImageStepB;
	//Image *vDisplayProcessingImageStepC;
	//Image *vDisplayProcessingImageStepD;
	//Image *vDisplayProcessingImageStepE;
	//Image *vDisplayProcessingImageStepE2;
	//Image *vDisplayProcessingImageStepH;
	//Image *vDisplayProcessingImageStepV;
	Image *vIATScratchImage;
	Image *vIATScratchImage2;
	Image *vASIDDScratchImage;
	Image *vASIDDScratchImage2;
	Image *vASIDDScratchImage3;
	Image *vNIDDScratchImage;
	Image *vNIDDScratchImage2;
	Image *vPQLIScratchImage;
	Image *vPQLIScratchImage2;
	//Image *vImageDisplayImageBuffer1;
	//Array *vHeightLine;

	BYTE vImageShowCrossHair;

	//variables for learned file backup	
	CString vNamesOfFilesToBackup[cSizeOfBackupBuffers];
	byte * vDataPointersBackupFile[cSizeOfBackupBuffers];
	int vOriginalBufferSizeXBackupFile[cSizeOfBackupBuffers];
	int vOriginalBufferSizeYBackupFile[cSizeOfBackupBuffers];
	WORD vBitsPerPixelBackupFile[cSizeOfBackupBuffers];

	int vIndexOfBackupFileData;
	int vIndexOfBackupFileDataBufferExit;
	int vIndexOfBackupFileDataBufferEnter;
	int vBackupFileDataFlag;

	// Construction
	CITIPCDig();

	//destructor
	virtual ~CITIPCDig();

	//methods
	virtual void Serialize(CArchive& TempArchive);
	bool InitializeFrameGrabberBoard();
	void InitializeCamera(BYTE TempPixelsPerDetector);
	void SetupImageBuffer(WORD TempNewXSize, WORD TempNewYSize);
	void DisposeOriginalBuffer();
	void MakeOriginalBuffer();

	void ScaleImageSideways(WORD TempNewXSize, WORD TempNewYSize, float TempZoomX, float TempZoomY, WORD TempOffsetX, 
		WORD TempOffsetY, BYTE *TempInputBuffer, WORD TempNumberOfPixelsPerDetector, BYTE TempShowDetectorEdgeLines);

	void AddLinesToImageSideways(WORD TempNewXSize, WORD TempNewYSize, bool TempDrawIfBlack, 
			WORD TempOffsetX, WORD TempOffsetY, BYTE TempShowReferenceAdjustedImage, CImageData *TempImage, CInspection *TempInspection, BYTE TempScanTracType);

	void AddFixedROILinesToImageSideways(WORD TempNewXSize, WORD TempNewYSize, bool TempDrawIfBlack, WORD TempOffsetX, WORD TempOffsetY, 
		BYTE TempShowReferenceAdjustedImage, CImageData *TempImage, 
		CInspection *TempInspection, BYTE TempOverScanMultiplier, BYTE TempReferenceToEdges, WORD TempPlotLine);

	void AddScaleToImageSideways(WORD TempNewXSize, WORD TempNewYSize, float TempZoom, WORD TempOffsetX, WORD TempOffsetY);

	void AddEdgeDetectLineToImageSideways(WORD TempNewXSize, WORD TempNewYSize, float TempZoom, WORD TempOffsetX, WORD TempOffsetY);
	bool SetupAquisitionBuffer(WORD TempNewYSize, WORD TempNewFrameCount, WORD TempNewXROI, BYTE TempPixelsPerDetector);
	void FillBufferWithPattern(BYTE *TempPointerToBuffer, DWORD TempBufferSize, 
		DWORD TempPatternNumber, double TempDriftCompensationCurrent);
	/*
	void SetFENEnable(bool TempEnable);
	void SetFENRising(bool TempRising);
	void SetLENSync(bool TempSync);
	void SetLENRising(bool TempRising);
	void SetPClockRising(bool TempRising);
	*/
	void MakeReferenceImageAdjustment(BYTE *TempImage, BYTE *TempBuffer,
		int TempChangeTop, int TempChangeLeft);
	//void MakeClipboard();
	void SubtractProductAverageImage(BYTE *TempBufferPointer,
																						CImageData *TempAverageImage);
	//void SubtractAverageImage(BYTE *TempBufferPointer);
	void SubtractVoidProductAverageImage(BYTE *TempBufferPointer);
	//void SubtractVoidAverageImage(BYTE *TempBufferPointer);
	void AddProductHistogramImage(BYTE *TempBufferPointer);
	void AddFactoredProductStructureImage(BYTE *TempBufferPointer);
	void AddVoidFactoredProductStructureImage(BYTE *TempBufferPointer);
	//void AddProductGlassStructureImage(BYTE *TempBufferPointer);
	//void AddProductGlassHistogramImage(BYTE *TempBufferPointer);
	void AddProductStructureImage(BYTE *TempBufferPointer);
	void AddVoidProductStructureImage(BYTE *TempBufferPointer);
	bool MakeProductAverageImage();
	//bool MakeProductAverageGImage();
	bool MakeProductStructureImage();
	void AddProductContaminant2StructureImage(BYTE *TempBufferPointer);

	//void CreateProductGlassStructureImage();
	//void CreateProductGlassHistogramImage();
	//void MakeProductFactoredStructureImage(bool TempKeepMemoryBuffer);
	bool MakeVoidProductStructureImage();
	//void MakeVoidProductFactoredStructureImage();
	void FillMainDisplayBuffer();
	void SaveAverageImageToFile(CProduct *TempCurrentProduct);
	//void SaveAverageGImageToFile(CProduct *TempCurrentProduct);
	//void SetTopReferenceFromAverage(CProduct *TempCurrentProduct);
	void SaveStructureImageToFile(CProduct *TempCurrentProduct, bool TempMakeOriginal);
	void SaveContaminant2StructureImageToFile(CProduct *TempCurrentProduct, CImageData *TempContaminant2Structure, bool TempOriginalImage);
	//void SaveGlassStructureImageToFile(CProduct *TempCurrentProduct,
	//	CImageData *TempGlassStructure, bool TempOriginalImage);
	void SaveVoidStructureImageToFile(CProduct *TempCurrentProduct, bool TempSaveOriginalAlso);
	//void SaveHistogramImageToFile(CProduct *TempCurrentProduct);
	void SaveThresholdImageInHistogram(CProduct *TempCurrentProduct, BYTE TempThreshold, BYTE *TempPointerToSourceImage, BYTE TempDilateTimes, BYTE TempLowerThreshold);
	void CopyAverageBuffer(DWORD *TempFrom, BYTE *TempTo, DWORD TempSize);
	void MaskOffBottomOfImage(BYTE *TempBufferPointer, double TempBottom);
	void MakeAverageImageBuffer();
	void MakeStructureImageBuffer();
	void SaveThisRejectInBuffer(CContainer *TempContainer, long TempFrameNumber, BYTE TempEjectedInspectionTopReferenceAdjust, BYTE TempRejectReason);
	//void SaveRedBoxesForDisplay(CInspection *TempInspection, MVT_BLOB_RESULTS *TempBlobResults, int TempSideReferenceAdjust);
//	void SaveRedBoxForDisplay(CInspection *TempInspection, MVT_BLOB_RESULTS *TempBlobResults, int TempSideReferenceAdjust);
	void Save10RedBoxesForDisplay(CInspection *TempInspection, MVT_BLOB_RESULTS *TempBlobResults, int TempSideReferenceAdjust);
	void SaveRedImageForDisplay(Image *TempImage, CContainer *TempContainer);
	void SaveImageInHolderForDisplay(CContainer *TempContainer);
	void InitializeLastRejectsBuffer(bool TempJustCreatedMemory);
	void ZeroLastValues();
	void FillImageData(CImageData *TempImagePointer, long TempFrameNumber, CContainer *TempContainer);
	void MaskImageWithDialatedImage(Image *TempImage, Image *TempDialatedImage);
	void CombineImages(BYTE *TempMainImage, double TempMainImageFactor, BYTE *TempSecondaryImage, double TempSecondaryImageFactor);
	void ThresholdBuffer(BYTE *TempBufferPointer, DWORD TempSize, BYTE TempThreshold);
	void ThresholdBufferDown(BYTE *TempBufferPointer, DWORD TempSize, BYTE TempThreshold);
	void ChangedSizeOfProduct(CProduct *TempProduct);
	void RenameAverageFileToBackup(CProduct *TempProduct);
	void RenameStructureFilesToBackup(CProduct *TempProduct);
	void DeleteLearnImages();
	void LoadProductDataImages(CProduct *TempProduct);
	void DeleteALearnFile(CString TempLearnName, CString TempProductName);
	void DeleteLearnFiles(CProduct *TempProduct);
	void DeleteLearnFiles(CString TempProductName);
	void RenameLearnFilesToBackup(CProduct *TempProduct);
	void RenameALearnFileToBackup(CString TempLearnName, CProduct *TempProduct);
	void RestoreLearnFilesFromBackups(CProduct *TempProduct);
	void RestoreALearnFileFromBackup(CString TempLearnName, CProduct *TempProduct);
	void DeleteABackupLearnFile(CString TempLearnName);
	void DeleteBackupLearnFiles();
	void CopyImageData(CImageData *TempSourceImagePointer, CImageData *TempDestinationImagePointer, DWORD TempSize);
	double CalculateDensity(BYTE *TempAverageImage, CInspection *TempInspection, WORD TempMaximumHeight, WORD TempMaximumWidth);
	double CalculateFullDensity(BYTE *TempPointer, DWORD TempCount);
	void DeleteCSVFiles(CString TempProductName);
	void ChangeNameOfALearnFile(CString OriginalName, CString NewName, CString TempLearnName);
	void CopyALearnFile(CString OriginalName, CString NewName, CString TempLearnName);
	void CopyLearnFiles(CString TempFromName, CString TempToName);
	void ChangeNameOfLearnFiles(CString OriginalName, CString NewName);
	double CalculateInspectionDensity(BYTE *TempImagePointer, CInspection *TempInspection);
	double CalculateInspectionDensityAfterReference(BYTE *TempImagePointer, CInspection *TempInspection, int TempSideReference, int TempTopReferenceAdjust, BYTE TempReferenceToEdges, WORD TempContainerLeft, WORD TempContainerRight);
	bool AreThereAnyAirBubbles(BYTE *TempImagePointer, CInspection *TempInspection);
	double CalculatePipelineProductDensity(BYTE *TempImagePointer, CInspection *TempInspection);
	DWORD CalculateUnderfillByWeight(BYTE *TempImagePointer, CInspection *TempInspection, int TempReferenceAmount);
	double CalculateDensityInImage(BYTE *TempImage, WORD TempLeft, WORD TempRight, WORD TempTop);
	double CalculateInspectionStandardDeviation(BYTE *TempImagePointer, CInspection *TempInspection, double *TempAverage);
	double CalculateInspectionStandardDeviationAfterReference(BYTE *TempImagePointer, CInspection *TempInspection, int TempSideReference);
	void CalculateAverageDriftDensity(CProduct *TempProduct);
	void SaveProcess4ImageData(CProduct *TempProduct);
	void DoConvolveImage3x3(tImage *TempImage);
	void DoConvolveImage5x5(tImage *TempImage,tImage *TempImageOut);
	void DoConvolveImage7x7(tImage *TempImage);
	bool AddImageToStructure(CImageData *TempImageBufferPointer,CInspection *TempInspection);
	//void AddImageToGlassStructure(BYTE *TempImageBufferPointer, int TempSideReferenceAmount, bool TempSaveToFile);
	void AddImageToVoidStructure(CImageData *TempImageBufferPointer,CInspection *TempInspection);
	void FindContainerDimensions(BYTE *TempImage, Image *TempScratchImage, WORD *TempTop, WORD *TempBottom, WORD *TempRight, WORD *TempLeft, WORD *TempHeight, WORD *TempWidth, bool TempUseRelativeThreshold, WORD TempShapeHandlingWidthTolerancePixels);
	void FindContainerEdgesWithRelativeThreshold(BYTE *TempImage, WORD *TempTop, WORD *TempBottom, WORD *TempRight, WORD *TempLeft, WORD TempBottomPixel, bool TempUseRelativeThreshold);
	void VerifyContainerEdges(Image *TempImage, WORD *TempTop, WORD *TempBottom, WORD *TempRight, WORD *TempLeft);
	void ContainerTooWideLookForEdgesAgain(Image *TempImage, WORD *TempTop, WORD *TempBottom, WORD *TempRight, WORD *TempLeft);
	void FindContainerEdges(BYTE *TempImage, WORD *TempTop, WORD *TempBottom, WORD *TempRight, WORD *TempLeft);
	void FindContainerEdgesCheckingFromCenter(BYTE *TempImage, WORD *TempTop, WORD *TempBottom, WORD *TempRight, WORD *TempLeft);
	int CalculateSideToSideReference(WORD TempRightReference, WORD TempLeftReference, WORD *TempRight, WORD *TempLeft);
	//void FillInLightSpots(BYTE *TempBufferPointer);
	void PerformEnhanceOnImage(Image *TempInputImage, Image *TempScratchImage, BYTE TempEnhanceMethod);
	void RemoveBackgroundFromImage(Image *TempImage);
	void MakeProductBackgroundImageCorrectionFactors();
	void SaveBackgroundImageToFile(CProduct *TempCurrentProduct);
	bool ReadBackgroundImageFromFile(CProduct *TempProduct);
	void RenameBackgroundFileToBackup(CProduct *TempProduct);
	bool HaveUndoAddToLearnImage();
	bool HaveLearnImageFile(CString TempLearnName);
	void UndoLastAddToLearnFile(CString TempLearnName);
	void UndoLastAddToLearn();
	void MakeUndoBackupOfAStructureFile(CString TempLearnName, CProduct *TempProduct);
	void MakeUndoBackupOfLearnStructureFiles(CProduct *TempProduct);
	void LoadStructureImagesFromFiles(bool TempKeepMemoryBuffer, bool TempUseOriginal);
	void RestoreAnOriginalStructureFilePermanently(CString TempLearnName);
	void RestoreOriginalStructurePermanently();
	void RestoreOriginalStructureTemporarily();
	void RestoreToCurrentStructure();
	void DeleteUndoStructureLearnFiles(CProduct *TempProduct);
	//void ProcessGlassContaminants(Image *TempImage, CInspection *TempInspection);
	void DeleteUndoStructureImages(CProduct *TempProduct);
	double CalculateStructureDensityInDriftDensityROI(CProduct *TempProduct);
	double CalculateStructureDensityInProduct(CProduct *TempProduct);
	double CalculateStructureDensityForInspection(CInspection *TempInspection);
	void AddImageToImproveLearn(BYTE *TempImageBufferPointer, BYTE *TempScratchImagePointer);
	void ClearImproveLearn(bool TempMakeNewBuffers);
	void GetInspectionROIBounds(CInspection *TempInspection, WORD *TempBottom, WORD *TempLeft, WORD *TempHeight, WORD *TempWidth, int TempSideReferenceAdjust, BYTE TempReferenceToEdges, BYTE TempAutoImproveState);
	BYTE FindDarkestPixelInROI(CInspection *TempInspection, BYTE *TempImage, int TempSideReferenceAdjust, BYTE TempReferenceToEdges);
	double FindNumberOfPixelsBelowThreshold(CInspection *TempInspection, BYTE *TempImage, int TempSideReferenceAdjust, BYTE TempReferenceToEdges);
	void MedianSmoothImage(Image *TempImage, BYTE TempSmoothAmount, WORD TempLeft, WORD TempWidth, WORD TempBottom, WORD TempHeight);
	//void FindJarWells(CProduct *TempProduct);
	void SaveLearnFile(CString TempNameOfFile, BYTE *TempDataPointer, int TempOriginalBufferSizeX, int TempOriginalBufferSizeY, WORD TemBitsPerPixel, bool TempCreateBackup);
	void CheckWriteBackupLearnFile();
	CImageData * CITIPCDig::LoadLearnImageFromFile(CString TempProcessType, CProduct *TempProduct, CImageData *TempImageToReturn, CString TempImageTypeString);
	void DoSobelXY(Image *TempInputImage, Image *TempScratchImage);
	void SortImage(BYTE *TempImage, DWORD TempSize);
	void AndImages(BYTE *TempImageIn, BYTE *TempImageOut, DWORD TempSize);
};

#endif // !defined(AFX_ITIPCDIG_H__A511D620_05C2_11D4_ABE2_00500466E305__INCLUDED_)
