//ScanTrac Side View Source File
// ITIPCDig.cpp: implementation of the ITIPCDig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "math.h"
#include "ScanTrac.h"
#include "NoticeDialog.h"
#include "YesNoDialog.h"
#include "DisplayDialogThread.h"
#include "ImageAquisitionThread.h"
#include "ITIPCDig.h"

#ifdef UseIFCDLLs
	#include "PCDig.h"  //for PC Dig constants
	#include <PC2Link.h>
	#include "mvtools.h"
#endif
//#include "HalconCpp.h"
//#include "HImage.h"

const BYTE cBlackIntensity = 0x01;
const BYTE cWhiteIntensity = 0xFF;
const BYTE cWidthTolerance = 5;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern BYTE vGlobalNormalLearnDilateTimes;
extern BYTE vGlobalQuickLearnDilateTimes;
extern BYTE vGlobalAllowVoidInspection;
extern bool vGlobalFPGAVersion11Point0OrHigher;
//extern bool vGlobalUseHalconLibrary;
extern BYTE vGlobalScanTracType;
extern BYTE vGlobalLearnState;
extern BYTE vGlobalInAutoSize;
//extern BYTE vGlobalUseMetric;
extern BYTE vGlobalDemoMode;
extern CProduct *vGlobalCurrentProduct;
extern CDisplayDialogThread *vGlobalDisplayDialogThread;
extern CImageAquisitionThread *vGlobalImageAquisitionThread;
//extern bool vGlobalShowDebugWindows;
extern CString vGlobalCurrentDirectory;
extern BYTE vGlobalFrameGrabberType;
extern CScanTracSystemRunningData *vGlobalRunningData;
extern const WORD cOldResolutionX;
extern const WORD cOldResolutionY;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CITIPCDig,CObject,VERSIONABLE_SCHEMA|cCurrentArchiveVersionNumber)

CITIPCDig::CITIPCDig()
{
	vDoBottomReferencing = false;
	vAutoImproveStatisticsCount = 0;
	vAutoImproveStatisticsCountTo100 = 0;
	vAutoImproveStatisticsAddedToLearnCount = 0;
	vAutoImproveStatisticsCountEjectedIn100 = 0;

	vImageTypeString = "Unknown Type";
	vALearnedImageHasChanged = false;
	vDriftCompensationLocked = true;
	vNumberOfImagesInLastAddToLearn = 0;
	vUsingOriginalLearnTemporarily = false;

	Kernel3X3[0] = 0;Kernel3X3[1] = 1;Kernel3X3[2] = 0;
	Kernel3X3[3] = 1;Kernel3X3[4] = 1;Kernel3X3[5] = 1;
	Kernel3X3[6] = 0;Kernel3X3[7] = 1;Kernel3X3[8] = 0;

	Kernel5X5[0] = 0;Kernel5X5[1] = 1;Kernel5X5[2] = 2;Kernel5X5[3] = 1;Kernel5X5[4] = 0;
	Kernel5X5[5] = 1;Kernel5X5[6] = 4;Kernel5X5[7] = 8;Kernel5X5[8] = 4;Kernel5X5[9] = 1;
	Kernel5X5[10]= 2;Kernel5X5[11]= 8;Kernel5X5[12]=16;Kernel5X5[13]= 8;Kernel5X5[14]= 2;
	Kernel5X5[15]= 1;Kernel5X5[16]= 4;Kernel5X5[17]= 8;Kernel5X5[18]= 4;Kernel5X5[19]= 1;
	Kernel5X5[20]= 0;Kernel5X5[21]= 1;Kernel5X5[22]= 2;Kernel5X5[23]= 1;Kernel5X5[24]= 0;
	Kernel5X5[25]= 6;

	Kernel7X7[0] = 0;Kernel7X7[1] = 1;Kernel7X7[2] = 2;Kernel7X7[3] = 4;Kernel7X7[4] = 2;Kernel7X7[5] = 1;Kernel7X7[6] = 0;
	Kernel7X7[7] = 1;Kernel7X7[8] = 2;Kernel7X7[9] = 4;Kernel7X7[10]= 8;Kernel7X7[11]= 4;Kernel7X7[12]= 2;Kernel7X7[13]= 1;
	Kernel7X7[14]= 2;Kernel7X7[15]= 4;Kernel7X7[16]= 8;Kernel7X7[17]=16;Kernel7X7[18]= 8;Kernel7X7[19]= 4;Kernel7X7[20]= 2;
	Kernel7X7[21]= 4;Kernel7X7[22]= 8;Kernel7X7[23]=16;Kernel7X7[24]=32;Kernel7X7[25]=16;Kernel7X7[26]= 8;Kernel7X7[27]= 4;
	Kernel7X7[28]= 2;Kernel7X7[29]= 4;Kernel7X7[30]= 8;Kernel7X7[31]=16;Kernel7X7[32]= 8;Kernel7X7[33]= 4;Kernel7X7[34]= 2;
	Kernel7X7[35]= 1;Kernel7X7[36]= 2;Kernel7X7[37]= 4;Kernel7X7[38]= 8;Kernel7X7[39]= 4;Kernel7X7[40]= 2;Kernel7X7[41]= 1;
	Kernel7X7[42]= 0;Kernel7X7[43]= 1;Kernel7X7[44]= 2;Kernel7X7[45]= 4;Kernel7X7[46]= 2;Kernel7X7[47]= 1;Kernel7X7[48]= 0;
/*
	Kernel7X7[0] = 1;Kernel7X7[1] = 1;Kernel7X7[2] = 1;Kernel7X7[3] = 1;Kernel7X7[4] = 1;Kernel7X7[5] = 1;Kernel7X7[6] = 1;
	Kernel7X7[7] = 1;Kernel7X7[8] = 1;Kernel7X7[9] = 1;Kernel7X7[10]= 1;Kernel7X7[11]= 1;Kernel7X7[12]= 1;Kernel7X7[13]= 1;
	Kernel7X7[14]= 1;Kernel7X7[15]= 1;Kernel7X7[16]= 1;Kernel7X7[17]= 1;Kernel7X7[18]= 1;Kernel7X7[19]= 1;Kernel7X7[20]= 1;
	Kernel7X7[21]= 1;Kernel7X7[22]= 1;Kernel7X7[23]= 1;Kernel7X7[24]= 1;Kernel7X7[25]= 1;Kernel7X7[26]= 1;Kernel7X7[27]= 1;
	Kernel7X7[28]= 1;Kernel7X7[29]= 1;Kernel7X7[30]= 1;Kernel7X7[31]= 1;Kernel7X7[32]= 1;Kernel7X7[33]= 1;Kernel7X7[34]= 1;
	Kernel7X7[35]= 1;Kernel7X7[36]= 1;Kernel7X7[37]= 1;Kernel7X7[38]= 1;Kernel7X7[39]= 1;Kernel7X7[40]= 1;Kernel7X7[41]= 1;
	Kernel7X7[42]= 1;Kernel7X7[43]= 1;Kernel7X7[44]= 1;Kernel7X7[45]= 1;Kernel7X7[46]= 1;Kernel7X7[47]= 1;Kernel7X7[48]= 1;
*/
	vContainerWidthMaximum = 0;
	vContainerWidthMinimum = 0;
	vContainerWidthAverage = 0;
	vContainerWidthStandardDeviation = 0;
	vContainerWidthIndex = 0;
	vContainerWidthSamples = 0;
	vShowAverageMaskedWithHistogram = false;
	vNumberOfLastRejectImages = 50;
	vUsingLocalSetupDataFile = false;
	vFrameFlag = 1;
	vShowOnlyRealImages = 1;
	vMainDisplayIsDefaultImage = false;
	vMaximumImageOverlap = 1;
	vCalibrateBodyTriggerReferenceCount = 0;
	vLastLeftReferenceAdjust = 0;
	vLastRightReferenceAdjust = 0;

	vReferenceRightStandardDeviation = 0;
	vReferenceRightAverage = 0;
	vReferenceLeftStandardDeviation = 0; 
	vReferenceLeftAverage = 0;
	vReferenceTopStandardDeviation = 0; 
	vReferenceTopAverage = 0;
	vReferenceBottomStandardDeviation = 0; 
	vReferenceBottomAverage = 0;

	vLearnedAverageDensity = 0;
	vLearnedProductAverageImage = NULL;

	vOriginalBufferSize = 0;
	vOriginalBufferSizeX = 0;
	vOriginalBufferSizeY = 0;
	vImageDisplayOnlyRejects = false;
	vDisplayImageSizeX = (WORD) ResizeXCoor(416);
	//416 is normal background so can see buttons
	//768 is too big to fit screen, but one pixel per 2 feet
	//632 fits full screen
	vDisplayImageSizeY = (WORD) ResizeYCoor(360);
	//336 is auto setup image display
	//360 is normal background so can see buttons used in image display
	//512 is too big to fit screen, but one pixel per camera bit
	//476 fits full screen
	vMainDisplayImageSizeX = (WORD) ResizeXCoor(235);
	vMainDisplayImageSizeY = (WORD) ResizeYCoor(207);
	vMainDisplayImageSize = vMainDisplayImageSizeY * vMainDisplayImageSizeX;
	vMainDisplayImageOffsetX = (WORD) ResizeXCoor(111);
	vMainDisplayImageOffsetY = (WORD) ResizeYCoor(202);
	vMainImageDisplayBuffer = NULL;
	vMainImagePseudoColorDisplayBuffer = NULL;

	if (ThereIsEnoughMemory(vMainDisplayImageSize, "Main Display Buffer"))
		vMainImageDisplayBuffer = (BYTE *)malloc( vMainDisplayImageSize);

	if (ThereIsEnoughMemory(vMainDisplayImageSize, "Main Display Buffer"))
		vMainImagePseudoColorDisplayBuffer = (BYTE *)malloc( vMainDisplayImageSize);

	vAverageImage = NULL;
	vStructureImage = NULL;
	vImproveLearnStructureImage = NULL;
	vImproveLearnCount = 0;
	vImproveLearnContaminant2StructureImage = NULL;
	vContaminant2StructureImage = NULL;
	vVoidStructureImage = NULL;

	vProductBackgroundImage = NULL;
	vProductAverageImage = NULL;
	vProductHistogramImage = NULL;
	vProductStructureImage = NULL;
	vProductContaminant2StructureImage = NULL;
	vProductContaminant2StructureImageWithNoDilation = NULL;
	//vProductGlassStructureImage = NULL;
	//vProductGlassHistogramImage = NULL;
	//vProductFactoredStructureImage = NULL;
	vVoidProductStructureImage = NULL;
	//vVoidProductFactoredStructureImage = NULL;
	vProcessManipulationImage = NULL;

	vNextImageToDisplay = NULL;
	InitializeLastRejectsBuffer(true);

	vAverageCount = 0;
	vStructureCount = 0;

	vCalibrateBodyTriggerReferenceEnabled = false;
	vAverageEnabled = false;
	vStructureEnabled = false;

	vHaveRealImage = false;
	vProcessNextImage = false;
	vLastBufferFilled = NULL;
	vReferenceAdjustBuffer = NULL;
	
	vImageShowCrossHair = 0;
	vGridLineDistanceX = 0;
	vGridLineDistanceY = 0;
	vImageAquisitionTime = 1500;
	vLatestImage = NULL;
	vLatestImageCopy = NULL;
	vDesensitizeImage = NULL;
	vImageForPseudoColor = NULL;
	vIATScratchImage = NULL;
	vIATScratchImage2 = NULL;
	vASIDDScratchImage = NULL;
	vNIDDScratchImage = NULL;
	vNIDDScratchImage2 = NULL;
	vASIDDScratchImage2 = NULL;
	vASIDDScratchImage3 = NULL;
	vPQLIScratchImage = NULL;
	vPQLIScratchImage2 = NULL;
	//vImageDisplayImageBuffer1 = NULL;
	//vProcessingImageStepB = NULL;
	//vProcessingImageStepC = NULL;
	//vProcessingImageStepD = NULL;
	//vProcessingImageStepE = NULL;
	//vProcessingImageStepE2 = NULL;
	//vProcessingImageStepH = NULL;
	//vProcessingImageStepV = NULL;

	//vDisplayProcessingImageStepB = NULL;
	//vDisplayProcessingImageStepC = NULL;
	//vDisplayProcessingImageStepD = NULL;
	//vDisplayProcessingImageStepE = NULL;
	//vDisplayProcessingImageStepE2 = NULL;
	//vDisplayProcessingImageStepH = NULL;
	//vDisplayProcessingImageStepV = NULL;
	vLatestSubImage = NULL;
	//vContainerThresholdImage = NULL;
	//vHeightLine = NULL;
	vWindowHandle = NULL;
	vMemoryLock = NULL;
	vImageBuffer = NULL;	// Memory space which is the destination of the ITX grab operation
	vImageBufferWithNoLines = NULL;
	//vImageSizeX = 416;
	//416 is normal background so can see buttons
	//768 is too big to fit screen, but one pixel per 2 feet
	//632 fits full screen
	//vImageSizeY = 360;
	//360 is normal background so can see buttons
	//512 is too big to fit screen, but one pixel per camera bit
	//476 fits full screen
	vOldImageSizeX = 416;
	vOldImageSizeY = 360;
	vBitsPerPixel = cBitsPerPixel;
	vNumberOfFrames = 1;
	vBytesPerPixel = 1;

	//vOriginalPixels = NULL;
	vOriginalImage = NULL;
	vImageToDisplayOnMainMenu = NULL;
	vClipboardImage = NULL;
	vImageBufferSize = 0;
	vImage = NULL;
	vCaptureModule = NULL;
	//vPCDigBoard = NULL;
	vCamera = NULL;
	vAquisitionBuffer = NULL;
	vAquisitionSizeX = 0;
	vAquisitionSizeXUpTo12Detectors = 0;
	vAquisitionSizeY = 0;
	vInterlaced = 1;
	vAquisitionBufferSize = 0;
	//vPixelsInOneAquisitionFrame = 0;
	vAquisitionMemoryLock = 0;

	//for Backups of learn files
	vIndexOfBackupFileDataBufferEnter = 0;
	vIndexOfBackupFileDataBufferExit = 0;
	vBackupFileDataFlag = 0;	//0 - nothing to back up, 1 - waiting, 2 - waiting, 3 - backup now

	//vCameraLive = false;
	vHaveImage = false;

	Erflag TempErrorFlag;
	vBlobParameters = mvt_blob_create_params(&TempErrorFlag);
	if (TempErrorFlag != IM_OK)
		ReportErrorMessage("MVTools Error 1", cEMailInspx,32000);
	else
	{
		vBlobParameters->calc_results = 0;
		vBlobParameters->min_area = 10;
#ifdef UseIFCDLLs
		vBlobParameters->connected = CONNECT8;
		vBlobResults = mvt_blob_create_results(20, true, &TempErrorFlag);
		if (TempErrorFlag != IM_OK)
			ReportErrorMessage("MVTools Error 2", cEMailInspx,32000);
#endif
	}
	vAquisitionROITop = 0;//these are in pixels where product ones are in units
	vAquisitionROIBottom = 0;

	FillMainDisplayBuffer();
	ZeroLastValues();
	vLastScreenPaintTime = 0;
}

void CITIPCDig::ZeroLastValues()
{
	vLastFineReferenceAdjust = 0;
	vLastMinimumGoodMaximumBad = 3;
	vLastTop = 0;
	vLastBottom = 0;
	vLastRight = 0;
	vLastLeft = 0;
	vLastHeight = 0;
	vLastWidth = 0;
	vLastFineReferenceAdjust = 0;
	for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfInspections; TempLoop++)
	{
		vLastNumberOfBlobs[TempLoop] = 0;
		vLastDensityValue[TempLoop] = 0;
		vLastInspectonProcessingTime[TempLoop] = 0;
		vLastLargestBlobPerimeter[TempLoop] = 0;
	}
	vLastMinimumGoodMaximumBad = 255; 
	vLastCurveFitTime = 0;
	vLastGlassInspectionTime = 0;
	vLastSmoothTime = 0;
	vLastProcessTime = 0;
	vLastEnhanceTime = 0;
	vLastEdgeAdjustTime = 0;
	vLastFindEdgesTime = 0;
	vLastReferencePositionTime = 0;
	vLastCopyBufferTime = 0;
	vLastCopyToMainTime = 0;
	vLastFindContaminantsTime = 0;
	vLastFindVoidsTime = 0;
	vLastFindMedianTime = 0;
	vLastFindDensityTime = 0;
	vLastFindBlobsTime = 0;
	vLastFindTime = 0;
	vLastOriginalImageIntensity = 0;
	vLastImageIntensityAdjust = 0;
	vLastImageIntensityTooFarOff = false;

	vLastTopReferenceAdjust = 0;
	vLastDisplayTime = 0;
	for (BYTE TempLoop = 0; TempLoop < cMaximumRedBoxes; TempLoop++)
	{
		vLastRedBoxCornerX[TempLoop] = 0;
		vLastRedBoxCornerY[TempLoop] = 0;
		vLastRedBoxWidth[TempLoop] = 0;
		vLastRedBoxHeight[TempLoop] = 0;
		vLastRedBoxInspection[TempLoop] = 0;
	}
}

/*
	CCriticalSection vLastNRejectImagesLock;
	CCriticalSection vNextImageToDisplayLock;
	CAM_ATTR vCameraAttributes;
*/
CITIPCDig::~CITIPCDig()
{
	if (vMemoryLock != NULL)
	if (vCaptureModule != NULL)
	{
		vCaptureModule->UnLockMem(vMemoryLock);
		vMemoryLock=NULL;
	}
	if (vImageBuffer)
		free(vImageBuffer);
	if (vImageBufferWithNoLines)
		free(vImageBufferWithNoLines);
		
	if (vMainImageDisplayBuffer)
		free(vMainImageDisplayBuffer);
		
	if (vMainImagePseudoColorDisplayBuffer)
		free(vMainImagePseudoColorDisplayBuffer);

	if (vImage) 
	{
		delete vImage;
		vImage = NULL;
	}
	if (vCamera)
	{
		vCamera->Freeze();
		delete vCamera;
		vCamera = NULL;
	}

	/*
#ifdef UseIFCDLLs
	if (vPCDigBoard)
	{
		delete vPCDigBoard;
		vPCDigBoard = NULL;
	}
#endif
	*/
	if (vBlobParameters)
	{
		mvt_blob_delete_params(vBlobParameters);
		vBlobParameters = NULL;
	}
	if (vBlobResults)
	{
		mvt_blob_delete_results(vBlobResults);
		vBlobResults = NULL;
	}

	//if (vCaptureModule != NULL)
	//	delete vCaptureModule;
}

//IMPLEMENT_DYNCREATE(CITIPCDig, CObject)

void CITIPCDig::Serialize( CArchive& TempArchive )
{
  // call base class function first
  // base class is CObject in this case
  CObject::Serialize( TempArchive );

  // now do the stuff for our specific class
}

bool CITIPCDig::InitializeFrameGrabberBoard()
{
	bool TempOK = true;
#ifdef UsePCDigBoard

#ifndef TestModeCode
#ifdef UseIFCDLLs
	IfxSetConsoleHandler(NULL);  //prevents the IFC Message Console from displaying on screen 
#endif
#endif
															//with Freeze Timed Out Message when stop acquisition
	LPTSTR TempBoardName = _T("PCD");
#ifdef UseIFCDLLs
	CW2A TempBoardNameString(TempBoardName);			
	if (vGlobalDemoMode)
	{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\nDemo Mode\nno Frame Grabber access\nno real images";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
	}
	else
	if (!(vCaptureModule = IfxCreateCaptureModule(TempBoardNameString, 0, NULL)))
	{
		ReportErrorMessage("PCDig Frame Grabber not found, looking for PC2-CamLink", cWriteToLog, 0);
	}
	else
		vGlobalFrameGrabberType = cPCDigFrameGrabber;

	if (!vCaptureModule)
	{
		TempBoardName = _T("CL2");
		CW2A TempBoardNameString(TempBoardName);			
			
		if(!(vCaptureModule = IfxCreateCaptureModule(TempBoardNameString, 0, NULL)))
			ReportErrorMessage("Error, could not find Frame Grabber (PCDig or PC2-CamLink", cEMailInspx,32000);
		else
		{
			vGlobalFrameGrabberType = cPC2CamLinkFrameGrabber;
			ReportErrorMessage("PC2-CamLink Frame Grabber Initialized", cWriteToLog, 0);
		}
	}

	if (vCaptureModule)
	{  
		vCaptureModule->InportMode(false, IFC_RISING_EDGE, 0); //make input port not latched, just buffered
		vCaptureModule->InportInterruptPolarity(IFC_RISING_EDGE, 0); //turn off input port interrupts
		vCaptureModule->OutportVal(0xFF, LNK_PARALLEL_IO_CHAN);
	}
#endif
#endif
	return TempOK;
}

void CITIPCDig::InitializeCamera(BYTE TempPixelsPerDetector)
{
#ifdef UseIFCDLLs
	if (vCaptureModule != NULL)
	{
		vCamera = vCaptureModule->GetCam(0);


		CAM_ATTR TempCameraAttributes;
		vCamera->GetAttr(&TempCameraAttributes, false);

		if (vCamera == NULL)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\nCould Not Find Frame Grabber Board\nSystem Will Not Run";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		else 
		{
			//must be divisible by 4
			//if (vGlobalScanTracType == cBriosoScanTrac) //on Brioso, uController board is mounted on its side, so must swap X and Y from side view ScanTracs.
			//{
			//	vCamera->SetAcqParam(P_WIDTH_PIXELS , vAquisitionSizeY * vInterlaced, 0);  //number of pixels per line
			//	//must be divisible by 8
			//	vCamera->SetAcqParam(P_HEIGHT_PIXELS , vAquisitionSizeX, 0); //Number of lines per frame
			//}
			//else
			{
				vCamera->SetAcqParam(P_WIDTH_PIXELS , vAquisitionSizeX * vInterlaced, 0);  //number of pixels per line
				//must be divisible by 8
				vCamera->SetAcqParam(P_HEIGHT_PIXELS , vAquisitionSizeY, 0); //Number of lines per frame
			}

			if (vGlobalScanTracType == cCaseInspectorScanTrac)
			{
				vCamera->SetAcqParam(P_PIXEL_SIZE , 12 , 0);  //Number of bits per pixel
				vCamera->SetAcqParam(P_CAM_PIXEL_SIZE , 0 , 0);  //Number of bits per pixel
			}
			else
			{
				vCamera->SetAcqParam(P_PIXEL_SIZE , vBytesPerPixel * vBitsPerPixel , 0);  //Number of bits per pixel
			}
			/*
			CString TempCameraName = TempCameraAttributes.camName;
			int TempPosition = TempCameraName.Find("CL2");
			if (TempPosition < 0)
				TempPosition = TempCameraName.Find("CamLink");
			if (TempPosition >= 0) //have a PC2-CamLink Frame Grabber
			*/
			//SOLO CODE
			if (vGlobalFrameGrabberType == cPC2CamLinkFrameGrabber)
			{
				vCamera->SetAcqParam(CL2_DVAL_ENABLE, IFC_DISABLE, 0);
				//vCamera->SetAcqParam(CL2_DVAL_INPUT_MODE, CL2_DVAL_INPUT_VALID_DATA , 0);

				//CL2_INTR_ERROR_PIXEL_CLOCK //NO PIXEL CLOCK GOING INT FRAME GRABBER
				//CL2_INTR_FRAME_SKIPPED //A FRAME WAS LOST DUE TO SLOW READOUT

				if (vGlobalScanTracType == cCaseInspectorScanTrac) //need interleaved to get 3 channels or 3 images
					//vCamera->SetAcqParam(CL2_IMAGE_GEOMETRY , CL2_2CHAN_SEP_TAP_L2R , 0); //pixel read in order format  //don't need for simulated images to work.
					vCamera->SetAcqParam(CL2_IMAGE_GEOMETRY , CL2_2CHAN_INTERLEAVED , 0); //pixel read in order format  //don't need for simulated images to work.
				else
					vCamera->SetAcqParam(CL2_IMAGE_GEOMETRY , CL2_1CHAN_L2R , 0); //pixel read in order format  //don't need for simulated images to work.

				//maybe duo vCamera->SetAcqParam(CL2_IMAGE_GEOMETRY , CL2_2CHAN_SEP_TAP_L2R , 0); //pixel read in order format  //don't need for simulated images to work.
				vCamera->SetAcqParam(P_FEN_ENABLE,IFC_ENABLE , 0); //enable IFC_ENABLE 
				vCamera->SetAcqParam(P_LEN_SYNC, IFC_SYNC, 0); //IFC_SYNC  
				vCamera->SetAcqParam(P_LEN_POLARITY, IFC_RISING_EDGE, 0); //IFC_FALLING_EDGE  //don't need for simulated images to work.
				vCamera->SetAcqParam(PCD_PCLK_POLARITY , IFC_RISING_EDGE , 0); // IFC_FALLING_EDGE  //don't need for simulated images to work.
				//vCamera->SetAcqParam(PCD_MULTI_ENABLE, IFC_DISABLE, 0);  //don't need for simulated images to work.
				//vCamera->SetAcqParam(PCD_MULTI_INPUT_MODE, PCD_MULTI_INPUT_VALID_DATA , 0);  //don't need for simulated images to work.

				vCamera->SetAcqParam(PCD_IMAGE_GEOMETRY , PCD_1CHAN_L2R , 0); //pixel read in order format
 				vCamera->SetAcqParam(P_SCAN_MODE_STAT , IFC_NILACED , 0); //Non Interlaced video data

 				vCamera->SetAcqParam(P_HORZ_INACTIVE , 0 , 0); //Non Interlaced video data
 				vCamera->SetAcqParam(P_HORZ_ACTIVE , vAquisitionSizeY , vAquisitionSizeY); //Non Interlaced video data
#ifdef IFC59Driver
				//vCamera->SetAcqParam(PCD_TRIG_START_EVENT, PCD_TMODE_DISABLE , 0);  //don't need for simulated images to work.
				vCamera->SetAcqParam(PCD_HI_SPEED_ACQ_ENABLE,IFC_ENABLE);  //don't need for simulated images to work.
#endif

			}
			else
			{// have a PC-Dig frame grabber
				vCamera->SetAcqParam(P_FEN_ENABLE,IFC_ENABLE , 0); //enable IFC_ENABLE
				vCamera->SetAcqParam(P_LEN_SYNC, IFC_SYNC, 0); //IFC_SYNC
				vCamera->SetAcqParam(P_LEN_POLARITY, IFC_RISING_EDGE, 0); //IFC_FALLING_EDGE
				vCamera->SetAcqParam(PCD_PCLK_POLARITY , IFC_RISING_EDGE , 0); // IFC_FALLING_EDGE
				vCamera->SetAcqParam(PCD_MULTI_ENABLE, IFC_DISABLE, 0);
				vCamera->SetAcqParam(PCD_MULTI_INPUT_MODE, PCD_MULTI_INPUT_VALID_DATA , 0);

				vCamera->SetAcqParam(PCD_IMAGE_GEOMETRY , PCD_1CHAN_L2R , 0); //pixel read in order format

#ifdef IFC59Driver
				vCamera->SetAcqParam(PCD_TRIG_START_EVENT, PCD_TMODE_DISABLE , 0);
				vCamera->SetAcqParam(PCD_HI_SPEED_ACQ_ENABLE,IFC_ENABLE);
#endif

				vCamera->SetAcqParam(PCD_TRIG_GLITCH_DETECTOR , IFC_ENABLE , 0); //requires 150nsec for trigger
				//br h test
				//vCamera->SetAcqParam(PCD_TRIG_GLITCH_DETECTOR , IFC_DISABLE , 0); //requires 150nsec for trigger
				vCamera->SetAcqParam(PCD_TRIG_DIVIDE_FACTOR , 1 , 0); //Trigger divide factor
				vCamera->SetAcqParam(PCD_TRIG_DIVIDE_CONTROL , PCD_DISABLE_TRIG_DIVIDE , 0); //disable trigger divider
				vCamera->SetAcqParam(PCD_NUM_CAMERAS , 1 , 0); //Number of cameras

				vCamera->SetAcqParam(P_HORZ_OFF , 0 , 0); //Number of pixels after LEN before first pixel
				vCamera->SetAcqParam(P_VERT_OFF , 0 , 0); //Number of lines after FEN before first line
 				vCamera->SetAcqParam(P_SCAN_MODE_STAT , IFC_NILACED , 0); //Non Interlaced video data
				
				vCamera->SetAcqParam(P_TRIGGER_SRC , PCD_DIFFERENTIAL_TRIG , 0); //Trigger 0 differential
				vCamera->SetAcqParam(P_TRIGGER_SRC , PCD_DIFFERENTIAL_TRIG , 1); //Trigger 1 differential
				vCamera->SetAcqParam(P_TRIGGER_STATE , IFC_LEVEL_HIGH , 0); //Active level of trigger
			}
			
			//vCamera->GetAttr(&vCameraAttributes);
			//vBytesPerPixel = vCameraAttributes.dwBytesPerPixel;
		}
	}
#endif
}

bool CITIPCDig::SetupAquisitionBuffer(WORD TempNewYSize, WORD TempNewFrameCount, WORD TempNewXROI, BYTE TempPixelsPerDetector)
{
//initialize frame grabber board if not done
	bool TempOK = true;
	if (vGlobalDisplayDialogThread)
		vGlobalDisplayDialogThread->NextStep(); //step4

	vHaveImage = false;

	vBytesPerPixel = 1;
	vAquisitionSizeX = 384;	//pixels in 12 inches at 32 pixels per inch // vAquisitionSizeX is always number of pixels in 12 inches
	vAquisitionSizeXUpTo12Detectors = 384;	//pixels in 12 inches at 32 pixels per inch // vAquisitionSizeXUpTo12Detectors is always number of pixels image is tall
	if (TempPixelsPerDetector == 128)
	{
		vAquisitionSizeX = 768; //pixels in 12 inches at 64 pixels per inch
		vAquisitionSizeXUpTo12Detectors = 768; //pixels in 12 inches at 64 pixels per inch
	}

	if (vGlobalScanTracType == cForteScanTrac)
	{
		vBytesPerPixel = 2;
		if (vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors)
		{
			vAquisitionSizeXUpTo12Detectors = 768 * 2; //768 pixels in 24 inches at 32 pixels per inch, but this is 64 pixels per inch, so double
			vAquisitionSizeX = 768; //in 12 inches at 64 pixels per inch
		}
		else
		{
			vAquisitionSizeXUpTo12Detectors = 768; //768 pixels in 24 inches by 32 pixels per inch
		}
	}

	if ((vGlobalScanTracType == cAllegroScanTrac) || (vGlobalScanTracType == cSoloPlusScanTrac))
	{
		vBytesPerPixel = 2;
		vAquisitionSizeXUpTo12Detectors = 576;  //image comes in 2 halves, 384 and 192, but must act like both 384 as both inputs to frame grabber must be same size
		if (vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors)
		{
			vAquisitionSizeXUpTo12Detectors = 576 * 2; //768 pixels in 24 inches at 32 pixels per inch, but this is 64 pixels per inch, so double
			vAquisitionSizeX = 768; //in 12 inches at 64 pixels per inch
		}
	}

	if (vGlobalScanTracType == cCaseInspectorScanTrac)
	{
		vBytesPerPixel = 2;
		vAquisitionSizeXUpTo12Detectors = 576; //case inspector up to 18 detectors
		vInterlaced = 2;
		vAquisitionSizeX = 192;
	}
	else
		vInterlaced = 1;

	vAquisitionSizeY = TempNewYSize;
	vAquisitionBufferSize = vAquisitionSizeX * vAquisitionSizeY * vBytesPerPixel * vInterlaced; //this is the buffer the Frame Grabber Reads Into

	vOriginalBufferSize = TempNewXROI * TempNewYSize;  //this is the buffer we put the usable image into
	vOriginalBufferSizeX = TempNewXROI;//can be up to 768 on Forte 24 inch TD, can be up to 576 on Case Inspector
	vOriginalBufferSizeY = TempNewYSize;

	vAquisitionSizeY = TempNewYSize;

//if have an original buffer, remove it so a new one will be made
//even if aquisition size stays the same, because ROI may change
	DisposeOriginalBuffer();

#ifdef UseIFCDLLs
	if (vImage) 
	{
		if (vCamera != NULL)
			vCamera->Freeze();
	}
	if (vAquisitionMemoryLock != NULL)
	{
		if (vCaptureModule != NULL)
			vCaptureModule->UnLockMem(vAquisitionMemoryLock);
		vAquisitionMemoryLock=NULL;
	}
#endif
	if (vAquisitionBuffer != NULL)
		free(vAquisitionBuffer);
	vAquisitionBuffer = NULL;

	DWORD TempExtendedBufferSize = vAquisitionBufferSize * TempNewFrameCount * vBytesPerPixel * vInterlaced;

	// allocate space for image ring buffer to be the destination of the host seq snap operation
	if (ThereIsEnoughMemory(TempExtendedBufferSize, "Acquisition Buffer"))
	{
		while ((vAquisitionBufferSize * TempNewFrameCount * vBytesPerPixel * vInterlaced > 0x1000000) && (TempNewFrameCount >= 2))
		{
			ReportErrorMessage("Notice - " + dtoa(TempNewFrameCount, 0) + " Aquisition Buffers are too large, using half", cEMailInspx, 32000);
			TempNewFrameCount = TempNewFrameCount / 2;
		}
		TempExtendedBufferSize = vAquisitionBufferSize * TempNewFrameCount * vBytesPerPixel * vInterlaced;

		if (TempExtendedBufferSize > 0x1000000)
			ReportErrorMessage("vAquisitionBuffer > 16 mega bytes", cEMailInspx, 32000);

		vAquisitionBuffer = (BYTE *)malloc(TempExtendedBufferSize);
		vAquisitionBufferMaximum = vAquisitionBuffer + TempExtendedBufferSize;

		if (vCaptureModule)
			delete vCaptureModule;
		vCaptureModule = NULL;

		Sleep(10);

		if (!vGlobalDemoMode)
			TempOK = InitializeFrameGrabberBoard();
		if (vGlobalDisplayDialogThread)
			vGlobalDisplayDialogThread->NextStep(); //step5

		Sleep(10);

	// Pre-Lock memory so that swapping will not take place when grab is started
#ifdef UseIFCDLLs
		if (vAquisitionBuffer)
		if (vCaptureModule != NULL)
		{ //5/16/2006 only locked first frame of buffer before, now lock all 64 frames
			//DWORD TempMemoryAvailable = vCaptureModule->ContiguousMemAvailable();

			vAquisitionMemoryLock=vCaptureModule->LockMem(vAquisitionBuffer,TempExtendedBufferSize);
			//br htest vAquisitionMemoryLock=vCaptureModule->LockMem(vAquisitionBuffer,TempAquisitionBufferSize);
			DWORD TempError = IfxGetLastError();

			if (!vAquisitionMemoryLock)
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nThe destination memory for SeqSnap was to large to be locked into memory";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
		}
#endif
	}
	if (!vAquisitionBuffer)
		TempOK = false;

	Erflag TempFlag = IM_OK;
	
	if (vImageForPseudoColor)
	{
		TempFlag = im_delete(vImageForPseudoColor);
		if (TempFlag == IM_OK)
			vImageForPseudoColor = NULL;
	}
	if (vLatestImageCopy)
	{
		TempFlag = im_delete(vLatestImageCopy);
		if (TempFlag == IM_OK)
			vLatestImageCopy = NULL;
	}
	if (vDesensitizeImage)
	{
		TempFlag = im_delete(vDesensitizeImage);
		if (TempFlag == IM_OK)
			vDesensitizeImage = NULL;
	}
	if (vIATScratchImage)
	{
		TempFlag = im_delete(vIATScratchImage);
		if (TempFlag == IM_OK)
			vIATScratchImage = NULL;
	}
	if (vIATScratchImage2)
	{
		TempFlag = im_delete(vIATScratchImage2);
		if (TempFlag == IM_OK)
			vIATScratchImage2 = NULL;
	}
	if (vASIDDScratchImage)
	{
		TempFlag = im_delete(vASIDDScratchImage);
		if (TempFlag == IM_OK)
			vASIDDScratchImage = NULL;
	}
	if (vASIDDScratchImage2)
	{
		TempFlag = im_delete(vASIDDScratchImage2);
		if (TempFlag == IM_OK)
			vASIDDScratchImage2 = NULL;
	}
	if (vASIDDScratchImage3)
	{
		TempFlag = im_delete(vASIDDScratchImage3);
		if (TempFlag == IM_OK)
			vASIDDScratchImage3 = NULL;
	}
	if (vNIDDScratchImage)
	{
		TempFlag = im_delete(vNIDDScratchImage);
		if (TempFlag == IM_OK)
			vNIDDScratchImage = NULL;
	}
	if (vNIDDScratchImage2)
	{
		TempFlag = im_delete(vNIDDScratchImage2);
		if (TempFlag == IM_OK)
			vNIDDScratchImage2 = NULL;
	}
	if (vPQLIScratchImage)
	{
		TempFlag = im_delete(vPQLIScratchImage);
		if (TempFlag == IM_OK)
			vPQLIScratchImage = NULL;
	}
	
	if (vPQLIScratchImage2)
	{
		TempFlag = im_delete(vPQLIScratchImage2);
		if (TempFlag == IM_OK)
			vPQLIScratchImage2 = NULL;
	}
	
	if (ThereIsEnoughMemory(vOriginalBufferSizeX * vAquisitionSizeY + sizeof(Image), "Latest Image CopyBuffer"))
		vIATScratchImage = im_create(IM_BYTE, vOriginalBufferSizeX, vAquisitionSizeY);
	if (!vIATScratchImage)
		TempOK = false;

	if (ThereIsEnoughMemory(vOriginalBufferSizeX * vAquisitionSizeY + sizeof(Image), "Latest Image CopyBuffer"))
		vIATScratchImage2 = im_create(IM_BYTE, vOriginalBufferSizeX, vAquisitionSizeY);
	if (!vIATScratchImage2)
		TempOK = false;

	if (ThereIsEnoughMemory(vOriginalBufferSizeX * vAquisitionSizeY + sizeof(Image), "vASIDDScratchImage"))
		vASIDDScratchImage = im_create(IM_BYTE, vOriginalBufferSizeX, vAquisitionSizeY);
	if (!vASIDDScratchImage)
		TempOK = false;

	if (ThereIsEnoughMemory(vOriginalBufferSizeX * vAquisitionSizeY + sizeof(Image), "vASIDDScratchImage2"))
		vASIDDScratchImage2 = im_create(IM_BYTE, vOriginalBufferSizeX, vAquisitionSizeY);
	if (!vASIDDScratchImage2)
		TempOK = false;

	if (ThereIsEnoughMemory(vOriginalBufferSizeX * vAquisitionSizeY + sizeof(Image), "vASIDDScratchImage3"))
		vASIDDScratchImage3 = im_create(IM_BYTE, vOriginalBufferSizeX, vAquisitionSizeY);
	if (!vASIDDScratchImage3)
		TempOK = false;

	if (ThereIsEnoughMemory(vOriginalBufferSizeX * vAquisitionSizeY + sizeof(Image), "vPQLIScratchImage"))
		vPQLIScratchImage = im_create(IM_BYTE, vOriginalBufferSizeX, vAquisitionSizeY);
	if (!vPQLIScratchImage)
		TempOK = false;
	
	if (ThereIsEnoughMemory(vOriginalBufferSizeX * vAquisitionSizeY + sizeof(Image), "vPQLIScratchImage2"))
		vPQLIScratchImage2 = im_create(IM_BYTE, vOriginalBufferSizeX, vAquisitionSizeY);
	if (!vPQLIScratchImage2)
		TempOK = false;
	
	if (ThereIsEnoughMemory(vOriginalBufferSizeX * vAquisitionSizeY + sizeof(Image), "vNIDDScratchImage"))
		vNIDDScratchImage = im_create(IM_BYTE, vOriginalBufferSizeX, vAquisitionSizeY);
	if (!vNIDDScratchImage)
		TempOK = false;

	if (ThereIsEnoughMemory(vOriginalBufferSizeX * vAquisitionSizeY + sizeof(Image), "vNIDDScratchImage2"))
		vNIDDScratchImage2 = im_create(IM_BYTE, vOriginalBufferSizeX, vAquisitionSizeY);
	if (!vNIDDScratchImage2)
		TempOK = false;

	if (ThereIsEnoughMemory(vOriginalBufferSizeX * vAquisitionSizeY + sizeof(Image), "vImageForPseudoColor"))
		vImageForPseudoColor = im_create(IM_BYTE, vOriginalBufferSizeX, vAquisitionSizeY);
	if (!vImageForPseudoColor)
		TempOK = false;

	if (ThereIsEnoughMemory(vOriginalBufferSizeX * vAquisitionSizeY + sizeof(Image), "Latest Image CopyBuffer"))
		vLatestImageCopy = im_create(IM_BYTE, vOriginalBufferSizeX, vAquisitionSizeY);
	if (!vLatestImageCopy)
		TempOK = false;

	if (ThereIsEnoughMemory(vOriginalBufferSizeX * vAquisitionSizeY + sizeof(Image), "vDesensitizeImage"))
		vDesensitizeImage = im_create(IM_BYTE, vOriginalBufferSizeX, vAquisitionSizeY);
	if (!vDesensitizeImage)
		TempOK = false;

	if (vLatestImage)
		im_delete(vLatestImage);
	vLatestImage = NULL;

	if (ThereIsEnoughMemory(vOriginalBufferSizeX * vAquisitionSizeY + sizeof(Image), "Latest Image Buffer"))
		vLatestImage = im_create(IM_BYTE, vOriginalBufferSizeX, vAquisitionSizeY);
	if (!vLatestImage)
		TempOK = false;

	DWORD TempSize = sizeof(Image);

	if (vReferenceAdjustBuffer)
		free(vReferenceAdjustBuffer);
	vReferenceAdjustBuffer = NULL;

	if (ThereIsEnoughMemory(vOriginalBufferSize, "Reference Adjust Buffer"))
		vReferenceAdjustBuffer = (BYTE *)malloc(vOriginalBufferSize);
	if (!vReferenceAdjustBuffer)
		TempOK = false;

	if (vClipboardImage != NULL)
	{
		delete vClipboardImage;
		vClipboardImage = NULL;
	}
	InitializeLastRejectsBuffer(false);
	MakeOriginalBuffer();
	return TempOK;
}

void CITIPCDig::SetupImageBuffer(WORD TempNewXSize, WORD TempNewYSize)
{
	DWORD TempBufferSize = TempNewXSize * TempNewYSize;
	if (vImageBufferSize != TempBufferSize) 
	{
		// Pre-Lock memory so that swapping will not take place when grab is started
		if (vMemoryLock != NULL)
		{
			if (vCaptureModule != NULL)
				vCaptureModule->UnLockMem(vMemoryLock);
			vMemoryLock=NULL;
		}
		if (vImageBuffer)
			free(vImageBuffer);
		vImageBuffer = NULL;
		if (vImageBufferWithNoLines)
			free(vImageBufferWithNoLines);
		vImageBufferWithNoLines = NULL;

		// allocate space for image ring buffer to be the destination of the host seq snap operation
		if (ThereIsEnoughMemory(TempBufferSize * 2, "Display Buffer"))
		{
			vImageBuffer = (BYTE*)malloc( TempBufferSize);
			vImageBufferWithNoLines = (BYTE*)malloc( TempBufferSize);

			if (vImageBufferWithNoLines)
				FillMemory(vImageBufferWithNoLines, TempBufferSize, 255);

			if (vImageBuffer)
				FillMemory(vImageBuffer, TempBufferSize, 255);

#ifdef UseIFCDLLs
			if (vCaptureModule != NULL)
			if (vImageBuffer)
			{
				vMemoryLock=vCaptureModule->LockMem(vImageBuffer,TempBufferSize);
				DWORD TempError = IfxGetLastError();
			}

			if (vImage)
			if (vImageBuffer)
				vImage->GetSrc()->SetBufferAddr(vImageBuffer);
#endif
		}

		vOldImageSizeX = vDisplayImageSizeX;
		vOldImageSizeY = vDisplayImageSizeY;

		vImageBufferSize = TempBufferSize;
		vDisplayImageSizeX = TempNewXSize;
		vDisplayImageSizeY = TempNewYSize;
	}
}

void CITIPCDig::DisposeOriginalBuffer()
{
	if (vOriginalImage)
	{
		delete vOriginalImage;
		vOriginalImage = NULL;
	}

	if (vImageToDisplayOnMainMenu)
	{
		delete vImageToDisplayOnMainMenu;
		vImageToDisplayOnMainMenu = NULL;
	}
	
	if (vNextImageToDisplay)
	{
		delete vNextImageToDisplay;
		vNextImageToDisplay = NULL;
	}
	if (vProcessManipulationImage)
	{
		delete vProcessManipulationImage;
		vProcessManipulationImage = NULL;
	}
}

void CITIPCDig::MakeReferenceImageAdjustment(BYTE *TempImage, BYTE *TempBuffer, int TempChangeTop, int TempChangeLeft)
{
	if (TempImage)
	if (TempBuffer)
	{
		int TempAdjustAmount = TempChangeTop + (TempChangeLeft * vOriginalBufferSizeX);
		BYTE *TempFromBuffer;
		BYTE *TempToBuffer = TempBuffer;
		WORD TempX = 0;
		for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
		{
			TempX = (WORD)(TempLoop % vOriginalBufferSizeX); //mod function
			//if left edge with no image data
			if ((TempAdjustAmount < 0) && ((-TempAdjustAmount) > (int)TempLoop))
				*TempToBuffer = 255;
			else
			//if right edge with no image data
			if (TempAdjustAmount + TempLoop >= vOriginalBufferSize)
				*TempToBuffer = 255;
			else
			//if bottom edge with no image data
			if ((TempChangeTop < 0) && (-TempChangeTop > TempX))
				*TempToBuffer = 255;
			else
			//if top edge with no image data
			if ((TempChangeTop + TempX >= vOriginalBufferSizeX))
				*TempToBuffer = 255;
			else
			{
				TempFromBuffer = TempImage + TempLoop + TempAdjustAmount;
				*TempToBuffer = *TempFromBuffer;
			}
			TempToBuffer++;
		}
		//copy adjusted image back to original image buffer
		CopyMemory(TempImage,TempBuffer,vOriginalBufferSize);
	}
}

void CITIPCDig::MakeOriginalBuffer()
{
	if (vOriginalBufferSize)
	if (vOriginalImage == NULL)
	{
		if (ThereIsEnoughMemory(sizeof(CImageData), "vOriginalImage"))
		{
			vOriginalImage = new CImageData;
			vOriginalBufferSizeX = vOriginalBufferSizeX;
			vOriginalBufferSizeY = vAquisitionSizeY;
			if (vOriginalImage)
			{
				vOriginalImage->MakeGreyImage(vOriginalBufferSize);
				vOriginalImage->MakeRedImage(vOriginalBufferSize);
			}
		}
		if (ThereIsEnoughMemory(sizeof(CImageData), "vNextImageToDisplay"))
		{
			vNextImageToDisplay = new CImageData;
			//fill image with grey scale pattern
			if (vNextImageToDisplay)
			{
				vNextImageToDisplay->MakeGreyImage(vOriginalBufferSize);
				if (vNextImageToDisplay->vGreyImage)
					FillMemory(vNextImageToDisplay->vGreyImage, vOriginalBufferSize, 10);

				vNextImageToDisplay->MakeRedImage(vOriginalBufferSize);
				if (vNextImageToDisplay->vRedImage)
					ZeroMemory(vNextImageToDisplay->vRedImage, vOriginalBufferSize);
			}
		}
		if (ThereIsEnoughMemory(sizeof(CImageData), "vImageToDisplayOnMainMenu"))
		{
			vImageToDisplayOnMainMenu = new CImageData;
			//fill image with grey scale pattern
			if (vImageToDisplayOnMainMenu)
			{
				vImageToDisplayOnMainMenu->MakeGreyImage(vOriginalBufferSize);
				if (vImageToDisplayOnMainMenu->vGreyImage)
					FillMemory(vImageToDisplayOnMainMenu->vGreyImage, vOriginalBufferSize, 10);

				vImageToDisplayOnMainMenu->MakeRedImage(vOriginalBufferSize);
				if (vImageToDisplayOnMainMenu->vRedImage)
					ZeroMemory(vImageToDisplayOnMainMenu->vRedImage, vOriginalBufferSize);
			}
		}
	}
	
	if (vProcessManipulationImage == NULL)
	if (ThereIsEnoughMemory(sizeof(CImageData), "vProcessManipulationImage"))
	{
		vProcessManipulationImage = new CImageData;
		vProcessManipulationImage->MakeGreyImage(vOriginalBufferSize);
		vProcessManipulationImage->MakeRedImage(vOriginalBufferSize);
	}

	//new improve learn created when reset counters and have loaded learned images
	if (vImproveLearnStructureImage != NULL)
	{
		delete vImproveLearnStructureImage;
		vImproveLearnStructureImage = NULL;
		vImproveLearnCount = 0;
	}

	if (vImproveLearnContaminant2StructureImage != NULL)
	{
		delete vImproveLearnContaminant2StructureImage;
		vImproveLearnContaminant2StructureImage = NULL;
	}
}
/*
void CITIPCDig::ScaleImage(WORD TempNewXSize, WORD TempNewYSize, float TempZoom, WORD TempOffsetX, WORD TempOffsetY, BYTE *TempInputBuffer)
{
	//if have an image
	if (vHaveImage)
	{
		//scale from original buffer to desired size
		double TempScaleX = ((double)vOriginalBufferSizeX / (double)TempNewXSize) / (double)TempZoom;
		double TempScaleY = ((double)vOriginalBufferSizeY / (double)TempNewYSize) / (double)TempZoom;

		//SetupImageBuffer(TempNewXSize, TempNewYSize);
		//need to set buffer in vImage to correct size
		if (TempInputBuffer != NULL)
		if (vImageBuffer != NULL)
		{

			BYTE *TempBuffer = vImageBuffer;
			BYTE *TempOriginalBuffer = TempInputBuffer;
			DWORD TempDelta = 0;

			for(WORD TempLoopY = 0; TempLoopY < TempNewYSize; TempLoopY++)
			{
				for(WORD TempLoopX = 0; TempLoopX < TempNewXSize; TempLoopX++)
				{
					TempDelta = ((DWORD)((TempScaleX * TempLoopX)) + //number of pixels up this line
						TempOffsetX + //number of pixels offset in line
						(TempOffsetY * vOriginalBufferSizeX) + //number of lines offset
						(vOriginalBufferSizeX * (DWORD)((TempLoopY * TempScaleY)))); //number of lines up the frame
					
					if (TempDelta >= vOriginalBufferSize)
						TempDelta = vOriginalBufferSize - 1;
					TempOriginalBuffer = TempDelta + TempInputBuffer;

					*TempBuffer = *TempOriginalBuffer;
					if (vImageShowCrossHair)
					{
						if ((TempDelta == (vOriginalBufferSize - vOriginalBufferSizeX) / 2) ||
							(TempDelta == (vOriginalBufferSize - vOriginalBufferSizeX) / 2 
							+ vOriginalBufferSizeX) ||
							(TempDelta == (vOriginalBufferSize - vOriginalBufferSizeX) / 2 
							- vOriginalBufferSizeX) ||
							(TempDelta == (vOriginalBufferSize - vOriginalBufferSizeX) / 2 + 1) ||
							(TempDelta == (vOriginalBufferSize - vOriginalBufferSizeX) / 2 - 1))
							*TempBuffer = cBlackIntensity;

						double TempX = TempDelta / vOriginalBufferSizeX;
						double TempY = (TempDelta % vOriginalBufferSizeX) + vAquisitionROIBottom;
						if (((TempY == 0) && (TempX < 20))  ||
							((TempY == 63) && (TempX < 20)) ||
							((TempY == 64) && (TempX < 20)) ||
							((TempY == 127) && (TempX < 20)) ||
							((TempY == 128) && (TempX < 20)) ||
							((TempY == 191) && (TempX < 20)) ||
							((TempY == 192) && (TempX < 20)) ||
							((TempY == 255) && (TempX < 20)) ||
							((TempY == 256) && (TempX < 20)) ||
							((TempY == 319) && (TempX < 20)) ||
							((TempY == 320) && (TempX < 20)) ||
							((TempY == 383) && (TempX < 20)) ||
							((TempY == 384) && (TempX < 20)) ||
							((TempY == 447) && (TempX < 20)) ||
							((TempY == 448) && (TempX < 20)) ||
							((TempY == 511) && (TempX < 20)))
							*TempBuffer = cBlackIntensity;
					}
					TempBuffer++;  
				}
			}
		}
	}
}
*/

void CITIPCDig::ScaleImageSideways(WORD TempNewXSize, WORD TempNewYSize, float TempZoomX, float TempZoomY, WORD TempOffsetX, 
		WORD TempOffsetY, BYTE *TempInputBuffer, WORD TempNumberOfPixelsPerDetector, BYTE TempShowDetectorEdgeLines)
{
	if (!TempZoomX)
		TempZoomX = 1;
	if (!TempZoomY)
		TempZoomY = 1;
	//if have an image
	//if (vHaveImage)
	{
		//scale from original buffer to desired size
		double TempScaleX = ((double)vOriginalBufferSizeY / (double)TempNewXSize) / (double)TempZoomX;
		double TempScaleY = ((double)vOriginalBufferSizeX / (double)TempNewYSize) / (double)TempZoomY;

		//SetupImageBuffer(TempNewXSize, TempNewYSize);
		if (TempInputBuffer)
		if (vImageBuffer)
		{
			BYTE *TempBuffer;
			BYTE *TempOriginalBuffer;
			TempBuffer = vImageBuffer;
			TempOriginalBuffer = TempInputBuffer;
			DWORD TempDelta = 0;

			for(WORD TempLoopY = 0; TempLoopY < TempNewYSize; TempLoopY++)
			{
				for(WORD TempLoopX = 0; TempLoopX < TempNewXSize; TempLoopX++)
				{
					TempDelta = ((DWORD)(((double)TempScaleY * 
							(double)(TempNewYSize - 1 - TempLoopY))) + //number of pixels up this line
						TempOffsetX + //number of pixels offset in line
						(TempOffsetY * vOriginalBufferSizeX) + //number of lines offset
						(vOriginalBufferSizeX * (DWORD)(((double)( TempLoopX) * 
							(double)TempScaleX)))); //number of lines up the frame
					
					if (TempDelta >= vOriginalBufferSize)
					{
						TempDelta = vOriginalBufferSize - 1;
						//ReportErrorMessage("Error-Scaling Problem 1", cEMailInspx,32000);
					}
					TempOriginalBuffer = TempInputBuffer + TempDelta;

					*TempBuffer = *TempOriginalBuffer;  //copy scalled image data

					if ((vImageShowCrossHair) || (TempShowDetectorEdgeLines))
					{
						if (vImageShowCrossHair & 4)
						if ((TempDelta == (vOriginalBufferSize - vOriginalBufferSizeX) / 2.0) ||
							(TempDelta == (vOriginalBufferSize - vOriginalBufferSizeX) / 2.0 + vOriginalBufferSizeX) ||
							(TempDelta == (vOriginalBufferSize - vOriginalBufferSizeX) / 2.0 - vOriginalBufferSizeX) ||
							(TempDelta == (vOriginalBufferSize - vOriginalBufferSizeX) / 2.0 + 1.0) ||
							(TempDelta == (vOriginalBufferSize - vOriginalBufferSizeX) / 2.0 - 1.0))
							*TempBuffer = cBlackIntensity;


						WORD TempBottomOffset = 0;
						if (vGlobalCurrentProduct->vProductImageHeightBottom)
							TempBottomOffset = (WORD)(vGlobalCurrentProduct->vProductImageHeightBottom * vGlobalPixelsPerUnitInHeight);

						double TempX = TempDelta / vOriginalBufferSizeX;
						WORD TempY = (WORD)(TempDelta % vOriginalBufferSizeX);

						if ((vImageShowCrossHair & 1) || ((TempShowDetectorEdgeLines) && ((TempLoopX < 20) || (TempLoopX > TempNewXSize - 20))))
						{
							if (TempNumberOfPixelsPerDetector == 64)
							{
								if ((TempY + TempBottomOffset == 0) || (TempY + TempBottomOffset == 64) || (TempY + TempBottomOffset == 128) || 
									(TempY + TempBottomOffset == 192) || (TempY + TempBottomOffset == 256) || (TempY + TempBottomOffset == 320) || 
									(TempY + TempBottomOffset == 384) || (TempY + TempBottomOffset == 448) || (TempY + TempBottomOffset == 512) || 
									(TempY + TempBottomOffset == 576) || (TempY + TempBottomOffset == 640) || (TempY + TempBottomOffset == 704) ||
									(TempY + TempBottomOffset == 768) || (TempY + TempBottomOffset == 832) || (TempY + TempBottomOffset == 896) || 
									(TempY + TempBottomOffset == 960) || (TempY + TempBottomOffset == 1024) || (TempY + TempBottomOffset == 1088))
									*TempBuffer = cBlackIntensity;
							}
							else
							if (TempNumberOfPixelsPerDetector == 128)
							{
								if ((TempY + TempBottomOffset == 0) || (TempY + TempBottomOffset == 128) || (TempY + TempBottomOffset == 256) || 
									(TempY + TempBottomOffset == 384) || (TempY + TempBottomOffset == 512) || (TempY + TempBottomOffset == 640) || 
									(TempY + TempBottomOffset == 768) || (TempY + TempBottomOffset == 896))
									*TempBuffer = cBlackIntensity;
							}
						}

						if ((vImageShowCrossHair & 2) || ((TempShowDetectorEdgeLines) && ((TempLoopX < 20) || (TempLoopX > TempNewXSize - 20))))
						{
							if (TempNumberOfPixelsPerDetector == 64)
							{  //in rejects display
								if ((vOriginalBufferSizeX > 380) && (vDisplayImageSizeY == 360))  //show line below cause this line is pixilated out
								{
									if ((TempY + TempBottomOffset == 62) || (TempY + TempBottomOffset == 126) || (TempY + TempBottomOffset == 190) || 
										(TempY + TempBottomOffset == 254) || (TempY + TempBottomOffset == 318) || (TempY + TempBottomOffset == 382) || 
										(TempY + TempBottomOffset == 446) || (TempY + TempBottomOffset == 510) || (TempY + TempBottomOffset == 574) || 
										(TempY + TempBottomOffset == 638) || (TempY + TempBottomOffset == 702) || (TempY + TempBottomOffset == 765) ||
										(TempY + TempBottomOffset == 831) || (TempY + TempBottomOffset == 895) || (TempY + TempBottomOffset == 959) || 
										(TempY + TempBottomOffset == 1023) || (TempY + TempBottomOffset == 1087) || (TempY + TempBottomOffset == 1151))
										*TempBuffer = cBlackIntensity;
								}
								else  //in setup size window
								if ((vOriginalBufferSizeX > 380) && (vDisplayImageSizeY == 338))  //show line below cause this line is pixilated out in autosetup
								{
									if ((TempY + TempBottomOffset == 63) || (TempY + TempBottomOffset == 127) || (TempY + TempBottomOffset == 190) || 
										(TempY + TempBottomOffset == 255) || (TempY + TempBottomOffset == 319) || (TempY + TempBottomOffset == 383) || 
										(TempY + TempBottomOffset == 447) || (TempY + TempBottomOffset == 511) || (TempY + TempBottomOffset == 574) || 
										(TempY + TempBottomOffset == 639) || (TempY + TempBottomOffset == 703) || (TempY + TempBottomOffset == 765) ||
										(TempY + TempBottomOffset == 831) || (TempY + TempBottomOffset == 895) || (TempY + TempBottomOffset == 959) || 
										(TempY + TempBottomOffset == 1023) || (TempY + TempBottomOffset == 1087) || (TempY + TempBottomOffset == 1151))
										*TempBuffer = cBlackIntensity;
								}
								else
								{
									if ((TempY + TempBottomOffset == 63) || (TempY + TempBottomOffset == 127) || (TempY + TempBottomOffset == 191) || 
										(TempY + TempBottomOffset == 255) || (TempY + TempBottomOffset == 319) || (TempY + TempBottomOffset == 383) || 
										(TempY + TempBottomOffset == 447) || (TempY + TempBottomOffset == 511) || (TempY + TempBottomOffset == 575) || 
										(TempY + TempBottomOffset == 639) || (TempY + TempBottomOffset == 703) || (TempY + TempBottomOffset == 767) ||
										(TempY + TempBottomOffset == 831) || (TempY + TempBottomOffset == 895) || (TempY + TempBottomOffset == 959) || 
										(TempY + TempBottomOffset == 1023) || (TempY + TempBottomOffset == 1087) || (TempY + TempBottomOffset == 1151))
										*TempBuffer = cBlackIntensity;
								}
							}
							else
							if (TempNumberOfPixelsPerDetector == 128)
							{
								if ((TempY + TempBottomOffset == 127) || (TempY + TempBottomOffset == 255) || (TempY + TempBottomOffset == 383) || 
									(TempY + TempBottomOffset == 511) || (TempY + TempBottomOffset == 639) || (TempY + TempBottomOffset == 767) || 
									(TempY + TempBottomOffset == 895) || (TempY + TempBottomOffset == 1023))
									*TempBuffer = cBlackIntensity;
							}
						}
					}
					TempBuffer++;  
				}
			}
		}
	}
}

void CITIPCDig::AddLinesToImageSideways(WORD TempNewXSize, WORD TempNewYSize, bool TempDrawIfBlack, WORD TempOffsetX, WORD TempOffsetY, 
	BYTE TempShowReferenceAdjustedImage, CImageData *TempImage, CInspection *TempInspection, BYTE TempScanTracType)
{
	//if (!TempZoom)
	//	TempZoom = 1;
	//if have an image
	//if (vHaveImage)
	{
		double TempScaleX = ((double)TempNewXSize / (double)vOriginalBufferSizeY);// * (double)TempZoom;
		double TempScaleY = ((double)TempNewYSize / (double)vOriginalBufferSizeX);// * (double)TempZoom;
	
		WORD TempMinY = (WORD)(TempOffsetX);
		//WORD TempMaxY = (WORD)(TempOffsetX + (vOriginalBufferSizeY / TempZoom) + .5); 
		WORD TempMaxY = (WORD)(TempOffsetX + (vOriginalBufferSizeY) + .5); 
		WORD TempMinX = (WORD)(TempOffsetY);
		//WORD TempMaxX = (WORD)(TempOffsetY + (vOriginalBufferSizeX / TempZoom) + .5);
		WORD TempMaxX = (WORD)(TempOffsetY + (vOriginalBufferSizeX) + .5);
		//WORD TempPixelsInXDirection = (WORD)(vOriginalBufferSizeX / TempZoom);
		WORD TempPixelsInXDirection = (WORD)(vOriginalBufferSizeX);
		WORD TempYOffsetY = (vOriginalBufferSizeX - TempPixelsInXDirection) - TempOffsetY;
		if (TempYOffsetY > 0)
			TempYOffsetY--;

		//TempYOffsetY = 0;
		int TempDisplayLastTop = 0;
		int TempDisplayLastBottom = 0;
		if (vImageBuffer)
		{
			BYTE *TempBuffer;
			TempBuffer = vImageBuffer;
			DWORD TempDelta = 0;
			BYTE TempBlackCount = 0;
			bool TempBlack = false;

			TempDisplayLastTop = TempImage->vTop;
			TempDisplayLastBottom = TempImage->vBottom;
			//int TempDisplayLastBottom = TempImage->vBottom;
			int TempDisplayLastLeft = TempImage->vLeft;
			int TempDisplayLastRight = TempImage->vRight;
			if (TempShowReferenceAdjustedImage)
			{
				if (TempInspection)
				{
					if (TempInspection->vReferenceToTop)
					{
						TempDisplayLastTop = TempDisplayLastTop - TempImage->vTopReferenceAdjust;
						TempDisplayLastBottom = TempDisplayLastBottom - TempImage->vTopReferenceAdjust;
					}
					if (TempInspection->vReferenceToBottom)
					{
						TempDisplayLastTop = TempDisplayLastTop - TempImage->vTopReferenceAdjust;
						TempDisplayLastBottom = TempDisplayLastBottom - TempImage->vTopReferenceAdjust;

						if (TempDisplayLastBottom < 2)
							TempDisplayLastBottom = 2;

						if (TempDisplayLastBottom >= TempDisplayLastTop)
							TempDisplayLastTop = TempDisplayLastBottom + 1;
					}
				}
				TempDisplayLastLeft = TempDisplayLastLeft - TempImage->vLeftReferenceAdjust;
				TempDisplayLastRight = TempDisplayLastRight - TempImage->vLeftReferenceAdjust;
			}
			if (TempDisplayLastTop >= (int)vOriginalBufferSizeX)
			{
				TempDisplayLastTop = vOriginalBufferSizeX - 1;
				//ReportErrorMessage("Error-Overflow 5", cEMailInspx,32000);
			}
			if (TempDisplayLastTop <= 0)
			{
				if (TempDisplayLastTop < 0)
					ReportErrorMessage("Error-Overflow 6", cEMailInspx,32000);
				TempDisplayLastTop = vOriginalBufferSizeX - 1;
			}
			if (TempDisplayLastBottom < 0)
			{
				if (TempDisplayLastTop < 0)
					ReportErrorMessage("Error-Overflow 61", cEMailInspx,32000);
				TempDisplayLastBottom = 0;
			}
			if (TempDisplayLastBottom >= TempDisplayLastTop)
			{
				ReportErrorMessage("Error-Overflow 62", cEMailInspx,32000);
				TempDisplayLastBottom = 0;
			}

			//if (TempDisplayLastBottom >= (int)vOriginalBufferSizeX)
			//	TempDisplayLastBottom = vOriginalBufferSizeX;
			//if (TempDisplayLastBottom < 0)
			//	TempDisplayLastBottom = vOriginalBufferSizeX;

			if (TempDisplayLastRight >= (int)vOriginalBufferSizeY - 1)
			{
				//ReportErrorMessage("Error-Overflow 1: " + dtoa(TempDisplayLastRight,0), cEMailInspx,32000);
				TempDisplayLastRight = vOriginalBufferSizeY - 1;
			}
			if (TempDisplayLastRight <= 0)
			{
				//ReportErrorMessage("Error-Overflow 2: " + dtoa(TempDisplayLastRight,0), cEMailInspx,32000);
				TempDisplayLastRight = vOriginalBufferSizeY - 1;
			}

			if (TempDisplayLastLeft >= (int)TempDisplayLastRight)
			{
				//ReportErrorMessage("Error-Overflow 4", cEMailInspx,32000);
				TempDisplayLastLeft = 1;
			}
			if (TempDisplayLastLeft < 0)
			{
				TempDisplayLastLeft = 1;
				//ReportErrorMessage("Error-Overflow 3", cEMailInspx,32000);
			}
			//draw line for Top of Container
			if ((TempDisplayLastTop) >= TempMinX)
			if ((TempDisplayLastTop) <= TempMaxX)
			for (WORD TempLoopX = 0; TempLoopX < TempNewXSize; TempLoopX++)
			{
				TempDelta = (((DWORD)(TempLoopX)) - //pixel in line
					(DWORD)(TempNewXSize * (DWORD)(TempYOffsetY * TempScaleY + .5)) + //distance from top of image to be taken off
					(TempNewXSize * (DWORD)(TempScaleY * (vOriginalBufferSizeX - TempDisplayLastTop)))); //how many lines down
				if (TempDelta >= vImageBufferSize)
				{
					TempDelta = vImageBufferSize - 1;
					//ReportErrorMessage("Error-Scaling Problem 2", cEMailInspx,32000);
				}
				TempBuffer = TempDelta + vImageBuffer;

				if (TempBlack)
					*TempBuffer = cBlackIntensity;  //draw line pixel
				else
				if ((*TempBuffer) || (TempDrawIfBlack))
					*TempBuffer = cWhiteIntensity;  //draw line pixel

				TempBlackCount++;
				if (TempBlackCount > 10)
				{
					if (TempBlack)
						TempBlack = false;
					else
						TempBlack = true;
					TempBlackCount = 0;
				}
					//*TempBuffer = cBlackIntensity;  //draw line pixel
			}
			
			//draw line for Bottom of Container
			if (TempDisplayLastBottom > 1)
			if (TempDisplayLastBottom >= TempMinX)
			if (TempDisplayLastBottom <= TempMaxX)
			for (WORD TempLoopX = 0; TempLoopX < TempNewXSize; TempLoopX++)
			{
				TempDelta = (((DWORD)(TempLoopX)) - //pixel in line
					(DWORD)(TempNewXSize * (DWORD)(TempYOffsetY * TempScaleY + .5)) + //distance from top of image to be taken off
					(TempNewXSize * (DWORD)(TempScaleY * (vOriginalBufferSizeX - TempDisplayLastBottom)))); //how many lines down
				if (TempDelta >= vImageBufferSize)
				{
					TempDelta = vImageBufferSize - 1;
					ReportErrorMessage("Error-Scaling Problem 5", cEMailInspx,32000);
				}
				TempBuffer = TempDelta + vImageBuffer;
				//*TempBuffer = cWhiteIntensity;  //draw line pixel

				if (TempBlack)
					*TempBuffer = cBlackIntensity;  //draw line pixel
				else
				if ((*TempBuffer) || (TempDrawIfBlack))
					*TempBuffer = cWhiteIntensity;  //draw line pixel

				TempBlackCount++;
				if (TempBlackCount > 10)
				{
					if (TempBlack)
						TempBlack = false;
					else
						TempBlack = true;
					TempBlackCount = 0;
				}
			}

			//draw line for Right Side of Container
			if (TempDisplayLastRight == TempMaxY)
			{
				for (WORD TempLoopY = 0; TempLoopY < TempNewYSize; TempLoopY++)
				{
					TempDelta = (TempNewXSize * (DWORD)(TempLoopY)); //how many lines down
					if (TempDelta > 0)
						TempDelta = TempDelta - 1;
					if (TempDelta >= vImageBufferSize)
					{
						TempDelta = vImageBufferSize - 1;
						//ReportErrorMessage("Error-Scaling Problem 6", cEMailInspx,32000);
					}
					TempBuffer = TempDelta + vImageBuffer;

					if (TempBlack)
						*TempBuffer = cBlackIntensity;  //draw line pixel
					else
					if ((*TempBuffer) || (TempDrawIfBlack))
						*TempBuffer = cWhiteIntensity;  //draw line pixel

					TempBlackCount++;
					if (TempBlackCount > 10)
					{
						if (TempBlack)
							TempBlack = false;
						else
							TempBlack = true;
						TempBlackCount = 0;
					}
				}
			}
			else
			if ((TempDisplayLastRight >= TempMinY) && (TempDisplayLastRight < TempMaxY))
			for (WORD TempLoopY = 0; TempLoopY < TempNewYSize; TempLoopY++)
			{
				TempDelta = (((DWORD)(TempScaleX * TempDisplayLastRight)) - //pixel in line
					((DWORD)((TempScaleX * TempOffsetX + .5))) +  //distance from top of image to top of ROI
					(TempNewXSize * (DWORD)(TempLoopY))); //how many lines down
				if (TempDelta >= vImageBufferSize)
				{
					TempDelta = vImageBufferSize - 1;
					//ReportErrorMessage("Error-Scaling Problem 7", cEMailInspx,32000);
				}
				TempBuffer = TempDelta + vImageBuffer;

				if (TempBlack)
					*TempBuffer = cBlackIntensity;  //draw line pixel
				else
				if ((*TempBuffer) || (TempDrawIfBlack))
					*TempBuffer = cWhiteIntensity;  //draw line pixel

				TempBlackCount++;
				if (TempBlackCount > 10)
				{
					if (TempBlack)
						TempBlack = false;
					else
						TempBlack = true;
					TempBlackCount = 0;
				}
			}
			else
				ReportErrorMessage("Error-Overflow 7: " + dtoa(TempDisplayLastRight,0), cEMailInspx,32000);

			//ReportErrorMessage("Draw Left Line at: " + dtoa(TempDisplayLastLeft,0) + " pixel: " + dtoa(TempImage->vLeft,0), cWriteToLog,32000);

			//draw line for Left Side of Container
			if ((TempDisplayLastLeft >= TempMinY) && (TempDisplayLastLeft <= TempMaxY))
			{
				for(WORD TempLoopY = 0; TempLoopY < TempNewYSize; TempLoopY++)
				{
					TempDelta = (((DWORD)(TempScaleX * TempDisplayLastLeft)) - //pixel in line
						((DWORD)((TempScaleX * TempOffsetX + .5))) +  //distance from top of image to top of ROI
						(TempNewXSize * (DWORD)(TempLoopY))); //how many lines down
					if (TempDelta >= vImageBufferSize)
					{
						TempDelta = vImageBufferSize - 1;
						//ReportErrorMessage("Error-Scaling Problem 8", cEMailInspx,32000);
					}
					TempBuffer = TempDelta + vImageBuffer;

					if (TempBlack)
						*TempBuffer = cBlackIntensity;  //draw line pixel
					else
					if ((*TempBuffer) || (TempDrawIfBlack))
						*TempBuffer = cWhiteIntensity;  //draw line pixel

					TempBlackCount++;
					if (TempBlackCount > 10)
					{
						if (TempBlack)
							TempBlack = false;
						else
							TempBlack = true;
						TempBlackCount = 0;
					}
				}
			}
			else
				ReportErrorMessage("Error-Overflow 8: " + dtoa(TempDisplayLastLeft,0), cEMailInspx,32000);
		}


		//lower rim height detection

		if (TempImage)
		if ((vGlobalScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloPlusScanTrac))
		{
			if (vGlobalCurrentProduct->CalculateLowerRimHeight(NULL, TempImage))
			{
				if (TempShowReferenceAdjustedImage)
				if (TempInspection)
				{
					if (TempInspection->vReferenceToTop)
						vGlobalCurrentProduct->vLowerRimHeightInPixels = vGlobalCurrentProduct->vLowerRimHeightInPixels - TempImage->vTopReferenceAdjust;

					if (TempInspection->vReferenceToBottom)
						vGlobalCurrentProduct->vLowerRimHeightInPixels = vGlobalCurrentProduct->vLowerRimHeightInPixels - TempImage->vTopReferenceAdjust;
				}
				//draw line for vLowerRimHeightInPixels
				BYTE *TempBuffer;
				DWORD TempDelta = 0;
				BYTE TempBlackCount = 0;
				bool TempBlack = false;
				if ((vGlobalCurrentProduct->vLowerRimHeightInPixels) >= TempMinX)
				if ((vGlobalCurrentProduct->vLowerRimHeightInPixels) <= TempMaxX)
				if ((vGlobalCurrentProduct->vLowerRimHeightInPixels) >= TempDisplayLastTop * .2)
				if ((vGlobalCurrentProduct->vLowerRimHeightInPixels) < TempDisplayLastTop - 4)
				for (WORD TempLoopX = 0; TempLoopX < TempNewXSize; TempLoopX++)
				{
					TempDelta = (((DWORD)(TempLoopX)) - //pixel in line
						(DWORD)(TempNewXSize * (DWORD)(TempYOffsetY * TempScaleY + .5)) + //distance from top of image to be taken off
						(TempNewXSize * (DWORD)(TempScaleY * (vOriginalBufferSizeX - vGlobalCurrentProduct->vLowerRimHeightInPixels)))); //how many lines down

					if (TempDelta >= vImageBufferSize)
					{
						TempDelta = vImageBufferSize - 1;
						//ReportErrorMessage("Error-Scaling Problem 2", cEMailInspx,32000);
					}
					TempBuffer = TempDelta + vImageBuffer;

					if (TempBlack)
						*TempBuffer = cBlackIntensity;  //draw line pixel
					else
					if ((*TempBuffer) || (TempDrawIfBlack))
						*TempBuffer = cWhiteIntensity;  //draw line pixel

					TempBlackCount++;
					if (TempBlackCount > 30)
					{
						if (TempBlack)
							TempBlack = false;
						else
							TempBlack = true;
						TempBlackCount = 0;
					}
						//*TempBuffer = cBlackIntensity;  //draw line pixel
				}
			}
		}
	} 
}

void CITIPCDig::AddScaleToImageSideways(WORD TempNewXSize, WORD TempNewYSize, float TempZoom, WORD TempOffsetX, WORD TempOffsetY)
{
	if (!TempZoom)
		TempZoom = 1;
	//if have an image
	//if (vHaveImage)
	{
		if (vImageBuffer)
		{
			BYTE *TempBuffer;
			TempBuffer = vImageBuffer;
			DWORD TempDelta = 0;
			BYTE TempBlackCount = 0;
			bool TempBlack = false;

			WORD TempGridLengthX = (WORD)(TempNewXSize / 20);
			WORD TempGridLengthY = (WORD)(TempNewYSize / 20);

			vGridLineDistanceX = (double)TempNewXSize / 10.0;
			vGridLineDistanceY = (double)TempNewYSize / 10.0;

			//draw scale lines at top or along x axis
			for(WORD TempLoopY = 0; TempLoopY < TempGridLengthX; TempLoopY++)
			for(WORD TempLoopX = 0; TempLoopX < 10 - 1; TempLoopX++)
			{
				TempDelta = (((DWORD)((TempLoopX + 1) * vGridLineDistanceX + .5)) + //pixel in line
					(TempNewXSize * (DWORD)(TempLoopY))); //how many lines down
				if (TempDelta < vImageBufferSize)
				{
					TempBuffer = TempDelta + vImageBuffer;
					*TempBuffer = cBlackIntensity;  //draw line pixel
				}
			}
			
			//draw scale lines at side or along y axis
			for(WORD TempLoopY = 0; TempLoopY < 10 - 1; TempLoopY++)
			for(WORD TempLoopX = 0; TempLoopX < TempGridLengthY; TempLoopX++)
			{
				TempDelta = (((DWORD)(TempLoopX)) + //pixel in line
					(TempNewXSize * (DWORD)((TempLoopY + 1) * vGridLineDistanceY + .5))); //how many lines down
				if (TempDelta < vImageBufferSize)
				{
					TempBuffer = TempDelta + vImageBuffer;
					*TempBuffer = cBlackIntensity;  //draw line pixel
				}
			}
			vGridLineDistanceX = (double)((double)vOriginalBufferSizeX / 10 / TempZoom);
			vGridLineDistanceY = (double)((double)vOriginalBufferSizeY / 10 / TempZoom);
		}
	} 
}

void CITIPCDig::AddEdgeDetectLineToImageSideways(WORD TempNewXSize, 
	WORD TempNewYSize, float TempZoom, 
	WORD TempOffsetX, WORD TempOffsetY)
{
	if (vImageBuffer)
	if (vGlobalCurrentProduct)
	{
		BYTE *TempBuffer;
		TempBuffer = vImageBuffer;
	//	DWORD TempDelta = 0;
		BYTE TempBlackCount = 0;
		bool TempBlack = true;

		WORD TempTop = (WORD)(( vGlobalCurrentProduct->vEdgeLocationBottom + 
			vGlobalCurrentProduct->vEdgeLocationHeight) * vGlobalPixelsPerUnitInHeight);
		TempTop = TempTop * TempNewYSize / vOriginalBufferSizeX;
		TempTop = TempNewYSize - TempTop;
		WORD TempBottom = (WORD)(vGlobalCurrentProduct->vEdgeLocationBottom * vGlobalPixelsPerUnitInHeight);
		WORD TempHeight = (WORD)((vGlobalCurrentProduct->vEdgeLocationHeight) * vGlobalPixelsPerUnitInHeight);
		TempHeight = TempHeight * TempNewYSize / vOriginalBufferSizeX;
		//draw scale lines at top or along x axis
		//TempBuffer = vImageBuffer + (TempNewXSize * TempTop) + TempNewXSize - 8;
		TempBuffer = vImageBuffer + (TempNewXSize * TempTop);
		BYTE *TempPointer = 0;
		for(WORD TempLoopX = 0; TempLoopX < TempHeight; TempLoopX++)
		{
			if (TempBlack)
				*TempBuffer = cBlackIntensity;  //draw line pixel
			else
				*TempBuffer = cWhiteIntensity;  //draw line pixel
					
			TempPointer = TempBuffer + 1;
			for(WORD TempLoop = 0; TempLoop < 7; TempLoop++)
			{
				*TempPointer++ = *TempBuffer;
			}
			TempBuffer = TempBuffer + TempNewXSize;
			TempBlackCount++;
			if (TempBlackCount > 5)
			{
				if (TempBlack)
					TempBlack = false;
				else
					TempBlack = true;
				TempBlackCount = 0;
			}
		}
	}
}
/*
void CITIPCDig::AddScaleToImage(WORD TempNewXSize, 
	WORD TempNewYSize, float TempZoom, 
	WORD TempOffsetX, WORD TempOffsetY)
{
	//if have an image
	if (vHaveImage)
	{
		double TempScaleX = ((double)TempNewXSize / (double)vOriginalBufferSizeX) * (double)TempZoom;
		double TempScaleY = ((double)TempNewYSize / (double)vOriginalBufferSizeY) * (double)TempZoom;

		if (vImageBuffer != NULL)
		{
			BYTE *TempBuffer;
			TempBuffer = vImageBuffer;
			DWORD TempDelta = 0;
			BYTE TempBlackCount = 0;
			bool TempBlack = false;

			TempScaleX = ((double)TempNewXSize / (double)vOriginalBufferSizeX);
			TempScaleY = ((double)TempNewYSize / (double)vOriginalBufferSizeY);

			WORD TempGridLengthX = (WORD)(TempNewXSize / 20);
			WORD TempGridLengthY = (WORD)(TempNewYSize / 20);

			vGridLineDistanceX = (double)TempNewXSize / 10;
			vGridLineDistanceY = (double)TempNewYSize / 10;
			
			//draw scale lines at top or along x axis
			for(WORD TempLoopY = 0; TempLoopY < TempGridLengthX; TempLoopY++)
			for(WORD TempLoopX = 0; TempLoopX < 10 - 1; TempLoopX++)
			{
				TempDelta = (((DWORD)((TempLoopX + 1) * vGridLineDistanceX + .5)) + //pixel in line
					(TempNewXSize * (DWORD)(TempLoopY))); //how many lines down
				if (TempDelta >= vImageBufferSize)
					TempDelta = vImageBufferSize - 1;
				TempBuffer = TempDelta + vImageBuffer;
				*TempBuffer = cBlackIntensity;  //draw line pixel
			}
			
			//draw scale lines at side or along y axis
			for(TempLoopY = 0; TempLoopY < 10 - 1; TempLoopY++)
			for(WORD TempLoopX = 0; TempLoopX < TempGridLengthY; TempLoopX++)
			{
				TempDelta = (((DWORD)(TempLoopX)) + //pixel in line
					(TempNewXSize * (DWORD)((TempLoopY + 1) * vGridLineDistanceY + .5))); //how many lines down
				if (TempDelta >= vImageBufferSize)
					TempDelta = vImageBufferSize - 1;
				TempBuffer = TempDelta + vImageBuffer;
				*TempBuffer = cBlackIntensity;  //draw line pixel
			}
			vGridLineDistanceX = (double)(vOriginalBufferSizeX / 10 / TempZoom);
			vGridLineDistanceY = (double)(vOriginalBufferSizeY / 10 / TempZoom);
		}
	} 
}
*/
void CITIPCDig::AddFixedROILinesToImageSideways(WORD TempNewXSize, 
	WORD TempNewYSize, bool TempDrawIfBlack, 
	WORD TempOffsetX, WORD TempOffsetY, 
	BYTE TempShowReferenceAdjustedImage, CImageData *TempImage, 
	CInspection *TempInspection, BYTE TempOverScanMultiplier, BYTE TempReferenceToEdges,
	WORD TempPlotLine)
{
	//if (!TempZoom)
	//	TempZoom = 1;
	//if have an image
	//if (vHaveImage)
	if (vGlobalCurrentProduct)
	{
		double TempScaleX = ((double)TempNewXSize / (double)vOriginalBufferSizeY);// * (double)TempZoom);
		double TempScaleY = ((double)TempNewYSize / (double)vOriginalBufferSizeX);// * (double)TempZoom);
		WORD TempMinY = (WORD)(TempOffsetX);
		//WORD TempMaxY = (WORD)(TempOffsetX + (vOriginalBufferSizeY / TempZoom) + .5); 
		WORD TempMaxY = (WORD)(TempOffsetX + (vOriginalBufferSizeY) + .5); 
		WORD TempMinX = (WORD)(TempOffsetY);
		//WORD TempMaxX = (WORD)(TempOffsetY + (vOriginalBufferSizeX / TempZoom) + .5);
		WORD TempMaxX = (WORD)(TempOffsetY + (vOriginalBufferSizeX) + .5);
		//WORD TempPixelsInXDirection = (WORD)(vOriginalBufferSizeY / TempZoom);
		WORD TempPixelsInXDirection = (WORD)(vOriginalBufferSizeY);
		WORD TempYOffsetY = (vOriginalBufferSizeY - TempPixelsInXDirection) - TempOffsetY;
		if (TempYOffsetY > 0)
			TempYOffsetY--;

		if (vImageBuffer)
		{
			BYTE *TempBuffer;
			TempBuffer = vImageBuffer;
			DWORD TempDelta = 0;
			BYTE TempBlackCount = 0;
			bool TempBlack = false;

			int TempROIBottom = 0;
			int TempROILeft = 0;
			int TempROITop = 0;
			int TempROIRight = 0;

			if (TempInspection)
			{  
				TempROITop = TempInspection->vROIPixelTop;
				TempROIBottom = TempInspection->vROIPixelBottom;
				TempROILeft = TempInspection->vROIPixelLeft;
				TempROIRight = TempInspection->vROIPixelRight;
			}

			if (TempReferenceToEdges)
			if (TempImage)
			if (TempInspection)
			if (vGlobalCurrentProduct->vReferenceRight)
			{
				int TempLeftPixels = TempInspection->vROIPixelLeft - vGlobalCurrentProduct->vReferenceLeft;

				TempROILeft = TempImage->vLeft + TempLeftPixels;

				int TempRightPixels = TempInspection->vROIPixelRight - vGlobalCurrentProduct->vReferenceRight;

				TempROIRight = TempImage->vRight + TempRightPixels;

				if ((TempROIRight > vOriginalBufferSizeY) || (TempROIRight < 5))
				{
					//ReportErrorMessage("A Right Was: " + dtoa(TempROIRight, 0), cEMailInspx,32000);
					TempROIRight = vOriginalBufferSizeY - 1;
				}

				if (TempROIRight <= TempROILeft)
				{
					//ReportErrorMessage("A Left Was: " + dtoa(TempROILeft, 0), cEMailInspx,32000);
					TempROILeft = 1;
				}
			}

			if (TempROITop)
			{
				if (!(TempShowReferenceAdjustedImage & 1))
				if (!TempReferenceToEdges)// ||
					//(TempInspection == vGlobalCurrentProduct->vDriftDensityInspection))
				if (TempImage)
				{
					TempROILeft = TempROILeft + TempImage->vLeftReferenceAdjust;

					TempROIRight = TempROIRight + TempImage->vLeftReferenceAdjust;

					if ((TempROIRight >= vOriginalBufferSizeY) || (TempROIRight < 5))
					{
						//ReportErrorMessage("B Right Was: " + dtoa(TempROIRight, 0), cEMailInspx,32000);
						TempROIRight = vOriginalBufferSizeY - 1;
					}

					if (TempROIRight <= TempROILeft)
					{
						//ReportErrorMessage("B Left Was: " + dtoa(TempROILeft, 0), cEMailInspx,32000);
						TempROILeft = 1;
					}
				}
				if (!(TempShowReferenceAdjustedImage & 1))
				if (!TempReferenceToEdges)
				if (TempImage)
				{
					if (TempInspection)
					{
						if (TempInspection->vReferenceToTop)
						{
							TempROITop = TempROITop + TempImage->vTopReferenceAdjust;
							if (TempROIBottom > 1)
								TempROIBottom = TempROIBottom + TempImage->vTopReferenceAdjust;
							if (TempPlotLine)
								TempPlotLine = TempPlotLine + TempImage->vTopReferenceAdjust;
						}
						if (TempInspection->vReferenceToBottom)
						{
							if (TempROITop + 1 < vOriginalBufferSizeX)
								TempROITop = TempROITop + TempImage->vTopReferenceAdjust;
							TempROIBottom = TempROIBottom + TempImage->vTopReferenceAdjust;
							if (TempPlotLine)
								TempPlotLine = TempPlotLine + TempImage->vTopReferenceAdjust;
						}

						if ((TempROITop > vOriginalBufferSizeX) || (TempROITop < 1))
						{
							ReportErrorMessage("ROI Top Was above image: " + dtoa(TempROITop, 0), cEMailInspx,32000);
							TempROITop = vOriginalBufferSizeX - 1;
						}
						if (TempROIBottom >= TempROITop)
						{
							ReportErrorMessage("ROI Bottom was above top: " + dtoa(TempROITop, 0), cEMailInspx,32000);
							TempROIBottom = 1;
						}
					}
				}
				if ((TempShowReferenceAdjustedImage & 1))
				if (TempReferenceToEdges) //&&
					//(TempInspection != vGlobalCurrentProduct->vDriftDensityInspection))
				if (TempImage)
				{
					TempROILeft = TempROILeft - TempImage->vLeftReferenceAdjust;

					TempROIRight = TempROIRight - TempImage->vLeftReferenceAdjust;

					if ((TempROIRight >= vOriginalBufferSizeY) || (TempROIRight < 5))
					{
						//ReportErrorMessage("C Right Was: " + dtoa(TempROIRight, 0), cEMailInspx,32000);
						TempROIRight = vOriginalBufferSizeY - 1;
					}

					if (TempROIRight <= TempROILeft)
					{
						//ReportErrorMessage("C Left Was: " + dtoa(TempROILeft, 0), cEMailInspx,32000);
						TempROILeft = 1;
					}
				}
				if (TempROITop >= (int)vOriginalBufferSizeX)
					TempROITop = vOriginalBufferSizeX - 1;
				if (TempROITop <= 0)
					TempROITop = vOriginalBufferSizeX - 1;

				if (TempROIBottom >= (int)TempROITop)
					TempROIBottom = 0;
				if (TempROIBottom < 0)
					TempROIBottom = 0;

				if (TempROIRight >= (int)vOriginalBufferSizeY)
					TempROIRight = vOriginalBufferSizeY - 1;
				if (TempROIRight <= 0)
					TempROIRight = vOriginalBufferSizeY - 1;

				if (TempROILeft >= (int)TempROIRight)
					TempROILeft = 0;
				if (TempROILeft < 0)
					TempROILeft = 0;

				//int TempTopLine = (int)(TempScaleY * (vOriginalBufferSizeX - TempROIBottom));
				int TempTopLine = (int)((TempScaleY * (vOriginalBufferSizeX - TempROIBottom)));
				if (TempTopLine < 0)
					TempTopLine = 0;
				if (TempTopLine >= TempNewYSize)
					TempTopLine = TempNewYSize - 1;

				//int TempBottomLine = (int)(TempScaleY * (vOriginalBufferSizeX - TempROITop));
				int TempBottomLine = (int)((TempScaleY * (vOriginalBufferSizeX - TempROITop)));
				if (TempBottomLine < 0)
					TempBottomLine = 0;
				if (TempBottomLine > TempTopLine)
					TempBottomLine = 1;

				int TempRightLine = (int)(TempScaleX * TempROIRight);
				if (TempRightLine >= TempNewXSize)
					TempRightLine = TempNewXSize - 1;

				int TempLeftLine = (int)(TempScaleX * TempROILeft);
				if (TempLeftLine > TempRightLine)
				{
					//ReportErrorMessage("Line Left Was: " + dtoa(TempLeftLine, 0), cEMailInspx,32000);
					TempLeftLine = 1;
				}

				//draw line for Top of Contaminant ROI
				if (TempROITop >= TempMaxX)
					TempROITop = TempMaxX - 1;

				if (TempROITop <= TempMinX)
					TempMinX = 1;

				//draw line for Top of Contaminant ROI
				if (TempROITop >= TempMinX)
				if (TempROITop <= TempMaxX)
				//for(WORD TempLoopX = 0; TempLoopX < TempNewXSize; TempLoopX++)
				for (WORD TempLoopX = TempLeftLine; TempLoopX < TempRightLine; TempLoopX++)
				{
					TempDelta = (((DWORD)(TempLoopX)) - //pixel in line
						(DWORD)(TempNewXSize * (DWORD)(TempYOffsetY * TempScaleY + .5)) + //distance from top of image to be taken off
						(TempNewXSize * (DWORD)(TempScaleY * (vOriginalBufferSizeX - TempROITop)))); //how many lines down
					if (TempROITop < TempMaxX)
					if (TempDelta >= TempNewXSize)
						TempDelta = TempDelta - TempNewXSize;
					if (TempDelta >= vImageBufferSize)
					{
						TempDelta = vImageBufferSize - 1;
						//ReportErrorMessage("Error-Scaling Problem 11", cEMailInspx,32000);
					}
					TempBuffer = vImageBuffer + TempDelta;

					if (TempBlack)
						*TempBuffer = cBlackIntensity;  //draw line pixel
					else
					if ((*TempBuffer) || (TempDrawIfBlack))
						*TempBuffer = cWhiteIntensity;  //draw line pixel

					TempBlackCount++;
					if (TempBlackCount > 1)
					{
						if (TempBlack)
							TempBlack = false;
						else
							TempBlack = true;
						TempBlackCount = 0;
					}
					//*TempBuffer = cBlackIntensity;  //draw line pixel
				}

				//draw line for Bottom of Contaminant ROI
				if (TempROIBottom >= TempMinX)
				if (TempROIBottom < TempMaxX)
				for (WORD TempLoopX = TempLeftLine; TempLoopX < TempRightLine; TempLoopX++)
				{
					TempDelta = (((DWORD)(TempLoopX)) - //pixel in line
						(DWORD)(TempNewXSize * (DWORD)(TempYOffsetY * TempScaleY + .5)) + //distance from top of image to be taken off
						(TempNewXSize * (DWORD)(TempScaleY * (vOriginalBufferSizeX - TempROIBottom)))); //how many lines down

					if (TempROIBottom == TempMinX)
						TempDelta = (((DWORD)(TempLoopX)) + //pixel in line
							(DWORD)(TempNewXSize * (DWORD)(TempNewYSize - 1))); //how many lines down

					if (TempDelta >= vImageBufferSize)
					{
						TempDelta = vImageBufferSize - 1;
						//ReportErrorMessage("Error-Scaling Problem 12", cEMailInspx,32000);
					}
					TempBuffer = vImageBuffer + TempDelta;

					if (TempBlack)
						*TempBuffer = cBlackIntensity;  //draw line pixel
					else
					if ((*TempBuffer) || (TempDrawIfBlack))
						*TempBuffer = cWhiteIntensity;  //draw line pixel

					TempBlackCount++;
					if (TempBlackCount > 1)
					{
						if (TempBlack)
							TempBlack = false;
						else
							TempBlack = true;
						TempBlackCount = 0;
					}
					//*TempBuffer = cBlackIntensity;  //draw line pixel
				}

				if (TempPlotLine)
				if (TempPlotLine >= TempMinX)
				if (TempPlotLine < TempMaxX)
				for (WORD TempLoopX = TempLeftLine; TempLoopX < TempRightLine; TempLoopX++)
				{
					TempDelta = (((DWORD)(TempLoopX)) - //pixel in line
						(DWORD)(TempNewXSize * (DWORD)(TempYOffsetY * TempScaleY + .5)) + //distance from top of image to be taken off
						(TempNewXSize * (DWORD)(TempScaleY * (vOriginalBufferSizeX - TempPlotLine)))); //how many lines down

					if (TempPlotLine == TempMinX)
						TempDelta = (((DWORD)(TempLoopX)) + //pixel in line
							(DWORD)(TempNewXSize * (DWORD)(TempNewYSize - 1))); //how many lines down

					if (TempDelta >= vImageBufferSize)
					{
						TempDelta = vImageBufferSize - 1;
						//ReportErrorMessage("Error-Scaling Problem 12", cEMailInspx,32000);
					}
					TempBuffer = TempDelta + vImageBuffer;

					if (TempBlack)
						*TempBuffer = cBlackIntensity;  //draw line pixel
					else
					if ((*TempBuffer) || (TempDrawIfBlack))
						*TempBuffer = cWhiteIntensity;  //draw line pixel

					TempBlackCount++;
					if (TempBlackCount > 10)
					{
						if (TempBlack)
							TempBlack = false;
						else
							TempBlack = true;
						TempBlackCount = 0;
					}
					//*TempBuffer = cBlackIntensity;  //draw line pixel
				}

					//draw line for Right ROI
				if (TempROIRight >= TempMinY)
				if (TempROIRight <= TempMaxY)
				for(WORD TempLoopY = TempBottomLine; TempLoopY < TempTopLine; TempLoopY++)
				{
					TempDelta = (((DWORD)(TempScaleX * TempROIRight)) - //pixel in line
						((DWORD)((TempScaleX * TempOffsetX + .5))) +  //distance from top of image to top of ROI
						(TempNewXSize * (DWORD)(TempLoopY))); //how many lines down

					if (TempROIRight == TempMaxY)
					TempDelta = (((DWORD)(TempNewXSize - 1)) +  //end of the line
						(TempNewXSize * (DWORD)(TempLoopY))); //how many lines down

					if (TempDelta >= vImageBufferSize)
					{
						TempDelta = vImageBufferSize - 1;
						//ReportErrorMessage("Error-Scaling Problem 13", cEMailInspx,32000);
					}
					TempBuffer = vImageBuffer + TempDelta;

					if (TempBlack)
						*TempBuffer = cBlackIntensity;  //draw line pixel
					else
					if ((*TempBuffer) || (TempDrawIfBlack))
						*TempBuffer = cWhiteIntensity;  //draw line pixel

					TempBlackCount++;
					if (TempBlackCount > 1)
					{
						if (TempBlack)
							TempBlack = false;
						else
							TempBlack = true;
						TempBlackCount = 0;
					}
					//*TempBuffer = cBlackIntensity;  //draw line pixel
				}

				//draw line for Left ROI
				WORD TempLeftEdge = (WORD)(TempROILeft * TempScaleX); //make sure can subtract one
				if (TempROILeft >= TempMinY)
				if (TempROILeft < TempMaxY)
				for(WORD TempLoopY = TempBottomLine; TempLoopY < TempTopLine; TempLoopY++)
				{
					TempDelta = (((DWORD)(TempScaleX * TempROILeft)) - //pixel in line
						((DWORD)((TempScaleX * TempOffsetX + .5))) +  //distance from top of image to top of ROI
						(TempNewXSize * (DWORD)(TempLoopY))); //how many lines down
					if (TempDelta >= vImageBufferSize)
					{
						TempDelta = vImageBufferSize - 1;
						//ReportErrorMessage("Error-Scaling Problem 14", cEMailInspx,32000);
					}
					if (TempROILeft)
					if (TempLeftEdge)
						TempDelta--;
					TempBuffer = vImageBuffer + TempDelta;

					if (TempBlack)
						*TempBuffer = cBlackIntensity;  //draw line pixel
					else
					if ((*TempBuffer) || (TempDrawIfBlack))
						*TempBuffer = cWhiteIntensity;  //draw line pixel

					TempBlackCount++;
					if (TempBlackCount > 1)
					{
						if (TempBlack)
							TempBlack = false;
						else
							TempBlack = true;
						TempBlackCount = 0;
					}
				}
			}
		}
	} 
}
/*
void CITIPCDig::AddFixedROILinesToImage(WORD TempNewXSize, 
	WORD TempNewYSize, float TempZoom, 
	WORD TempOffsetX, WORD TempOffsetY, CImageData *TempImage)
{
	//if have an image
	if (vHaveImage)
	{
		double TempScaleX = ((double)TempNewXSize / (double)vOriginalBufferSizeX) * (double)TempZoom;
		double TempScaleY = ((double)TempNewYSize / (double)vOriginalBufferSizeY) * (double)TempZoom;
		WORD TempMinX = (WORD)(TempOffsetX);
		WORD TempMaxX = (WORD)(TempOffsetX + (vOriginalBufferSizeX / TempZoom) + .5); 
		WORD TempMinY = (WORD)(TempOffsetY);
		WORD TempMaxY = (WORD)(TempOffsetY + (vOriginalBufferSizeY / TempZoom) + .5);

		if (vImageBuffer != NULL)
		{
			BYTE *TempBuffer;
			TempBuffer = vImageBuffer;
			DWORD TempDelta = 0;
			BYTE TempBlackCount = 0;
			bool TempBlack = false;

			if ((vContaminantROITop) && (vContaminantROIRight))
			{
				//draw line for Fixed ROI Top
				if (vContaminantROITop >= TempMinX)
				if (vContaminantROITop <= TempMaxX)
				for(WORD TempLoopY = 0; TempLoopY < TempNewYSize; TempLoopY++)
				{
					TempDelta = (((DWORD)(TempScaleX * vContaminantROITop)) - //pixel in line
						((DWORD)((TempScaleX * (TempOffsetX)) + .5)) +  //distance from top of image to top of ROI
						(TempNewXSize * (DWORD)(TempLoopY))); //how many lines down
					if (TempDelta >= vImageBufferSize)
						TempDelta = vImageBufferSize - 1;
					TempBuffer = TempDelta + vImageBuffer;
					if (TempBlack)
						*TempBuffer = cBlackIntensity;  //draw line pixel
					else
						*TempBuffer = cWhiteIntensity;  //draw line pixel
					TempBlackCount++;
					if (TempBlackCount > 15)
					{
						if (TempBlack)
							TempBlack = false;
						else
							TempBlack = true;
						TempBlackCount = 0;
					}
					//*TempBuffer = cBlackIntensity;  //draw line pixel
				}

				//draw line for Fixed ROI Bottom
				if (vContaminantROIBottom >= TempMinX)
				if (vContaminantROIBottom <= TempMaxX)
				for(WORD TempLoopY = 0; TempLoopY < TempNewYSize; TempLoopY++)
				{
					TempDelta = (((DWORD)(TempScaleX * vContaminantROIBottom)) - //pixel in line
						((DWORD)((TempScaleX * (TempOffsetX)) + .5)) +  //distance from top of image to top of ROI
						(TempNewXSize * (DWORD)(TempLoopY))); //how many lines down
					TempBuffer = TempDelta + vImageBuffer;
					if (TempBlack)
						*TempBuffer = cBlackIntensity;  //draw line pixel
					else
						*TempBuffer = cWhiteIntensity;  //draw line pixel
					TempBlackCount++;
					if (TempBlackCount > 15)
					{
						if (TempBlack)
							TempBlack = false;
						else
							TempBlack = true;
						TempBlackCount = 0;
					}
					//*TempBuffer = cBlackIntensity;  //draw line pixel
				}

				//draw line for Fixed ROI Right
				if (vContaminantROIRight >= TempMinY)
				if (vContaminantROIRight <= TempMaxY)
				for(WORD TempLoopX = 0; TempLoopX < TempNewXSize; TempLoopX++)
				{
					TempDelta = (((DWORD)(TempLoopX)) - //pixel in line
						((((DWORD)(TempScaleY * TempOffsetY + .5)) * TempNewXSize)) +  //distance from top of image to top of ROI
						(TempNewXSize * (DWORD)(TempScaleY * vContaminantROIRight))); //how many lines down
					if (TempDelta >= vImageBufferSize)
						TempDelta = vImageBufferSize - 1;
					TempBuffer = TempDelta + vImageBuffer;
					if (TempBlack)
						*TempBuffer = cBlackIntensity;  //draw line pixel
					else
						*TempBuffer = cWhiteIntensity;  //draw line pixel
					TempBlackCount++;
					if (TempBlackCount > 15)
					{
						if (TempBlack)
							TempBlack = false;
						else
							TempBlack = true;
						TempBlackCount = 0;
					}
					//*TempBuffer = cBlackIntensity;  //draw line pixel
				}

				//draw line for Fixed ROI Left
				if (vContaminantROILeft >= TempMinY)
				if (vContaminantROILeft <= TempMaxY)
				for(WORD TempLoopX = 0; TempLoopX < TempNewXSize; TempLoopX++)
				{
					TempDelta = (((DWORD)(TempLoopX)) - //pixel in line
						((((DWORD)(TempScaleY * TempOffsetY + .5)) * TempNewXSize)) +  //distance from top of image to top of ROI
						(TempNewXSize * (DWORD)(TempScaleY * vContaminantROILeft))); //how many lines down
					if (TempDelta >= vImageBufferSize)
						TempDelta = vImageBufferSize - 1;
					TempBuffer = TempDelta + vImageBuffer;
					if (TempBlack)
						*TempBuffer = cBlackIntensity;  //draw line pixel
					else
						*TempBuffer = cWhiteIntensity;  //draw line pixel
					TempBlackCount++;
					if (TempBlackCount > 15)
					{
						if (TempBlack)
							TempBlack = false;
						else
							TempBlack = true;
						TempBlackCount = 0;
					}
					//*TempBuffer = cBlackIntensity;  //draw line pixel
				}
			}
		}
	} 
}
*/
//fill on its side
void CITIPCDig::FillBufferWithPattern(BYTE *TempPointerToBuffer, 
	DWORD TempNumberOfPixels, DWORD TempPatternNumber, double TempDriftCompensationCurrent)
{
	FillMemory(TempPointerToBuffer,vOriginalBufferSize,222);

	WORD TempMaxPatternHeight = (WORD)(vOriginalBufferSizeX * .9);
	//if (TempMaxPatternHeight > 130)
	//	TempMaxPatternHeight = 130;
	WORD SmallStepX = (WORD)((double)TempMaxPatternHeight / 100 + .5);
	WORD SmallStepY = (WORD)((double)vAquisitionSizeY / 100 + .5);
	if (SmallStepX == 0)
		SmallStepX = 1;
	if (SmallStepY == 0)
		SmallStepY = 1;
	WORD TempHeightStep = TempMaxPatternHeight / 10;
	WORD TempWidthStep = vAquisitionSizeY / 10;
	int TempBeltJitter = 0;
	DWORD TempRightSide = vAquisitionSizeY - 8;//(TempWidthStep * 2);
	DWORD TempLeftSide = 0; //TempWidthStep * 2;
	DWORD TempBottom = 2; //TempHeightStep;

	DWORD TempLidTop = TempMaxPatternHeight - 10; //TempHeightStep;
	//limit size so can have small image in auto setup with large aquistion buffer
	DWORD TempLidBottom = TempLidTop - SmallStepX;
	DWORD TempNormalLidBottom = TempLidTop + SmallStepX;
	DWORD TempLidWidth = TempWidthStep * 6;
	DWORD TempSidewaysJitter = 0;
	DWORD TempFuzzy = 0;
	DWORD TempPixelInLine = 0;
	int TempLidWidthAdjust = 0;
	WORD TempLidHeightAdjust = 0;
	WORD TempWidthAdjust = 0;
	DWORD TempMaxBottleLidWidth = TempWidthStep * 2;
	DWORD TempLine = 0;
	DWORD TempCockedCap = 0;
	int TempBottleNeckAdjust = 0;
	BYTE *TempBuffer;
	TempBuffer = TempPointerToBuffer;
	DWORD TempFoodLine = (DWORD)(TempLidTop - (3.5 * TempHeightStep));
	WORD TempFoodAdjust = 0;

	DWORD TempStep = TempPatternNumber % cNumberOfPatterns;

	switch (TempStep)
	{
		case 0: TempFoodAdjust = 0; TempBeltJitter = 0; 
			TempSidewaysJitter = 6; TempWidthAdjust = 1; 
		break;
		case 1: TempFoodAdjust = SmallStepX; TempBeltJitter = 2; 
			TempSidewaysJitter = 3; TempWidthAdjust = 5; TempLidHeightAdjust = 4;
		break;
		case 2: TempFoodAdjust = SmallStepX * 2; TempBeltJitter = 0; 
			TempSidewaysJitter = 3; TempWidthAdjust = 2; TempLidWidthAdjust = + 2;
		break;
		case 3: TempFoodAdjust = SmallStepX * 3; TempBeltJitter = 1; 
			TempSidewaysJitter = 4; TempWidthAdjust = 4; 
		break;
		case 4: TempFoodAdjust = SmallStepX * 5; TempBeltJitter = 0; 
			TempSidewaysJitter = 2; TempWidthAdjust = 2; 
		break;
		case 5: TempFoodAdjust = SmallStepX * 6; TempBeltJitter = 2; 
			TempSidewaysJitter = 0; TempWidthAdjust = 0; TempLidWidthAdjust = - 3; 
		break;
		case 6: TempFoodAdjust = SmallStepX * 6; TempBeltJitter = 1; 
			TempSidewaysJitter = 0; TempWidthAdjust = 0; TempLidWidthAdjust = + 1;
		break;
		case 7: TempFoodAdjust = SmallStepX * 7; TempBeltJitter = 1; 
			TempSidewaysJitter = 0; TempWidthAdjust = 0; TempLidWidthAdjust = - 1;
		break;
	}
	TempLeftSide = TempLeftSide + (TempWidthAdjust / 2);
	TempRightSide = TempRightSide - (TempWidthAdjust / 2);

	TempBeltJitter = (TempSidewaysJitter / 2) - 2;

	//if wide image, limit container to 4 inches wide
	double TempMaximumSize = 5; //inches
	//if (vGlobalUseMetric)
	//	TempMaximumSize = TempMaximumSize * 25.4;
	double TempNominalSize = 5; //inches
	//if (vGlobalUseMetric)
	//	TempNominalSize = TempNominalSize * 25.4;
	if (vAquisitionSizeY > TempMaximumSize * vGlobalPixelsPerUnit )
	{
		WORD TempReductionAmount = (WORD)((vAquisitionSizeY - (TempNominalSize * vGlobalPixelsPerUnit)) / 2);
		TempLeftSide = TempLeftSide + TempReductionAmount;
		TempRightSide = TempRightSide - TempReductionAmount;
	}

	TempLidWidth = TempWidthStep * 2;
	TempLidWidth = (TempRightSide - TempLeftSide) / 5;
	TempLidBottom = TempLidTop - TempHeightStep;
	TempNormalLidBottom = TempLidTop - TempHeightStep;
	if (TempPointerToBuffer)
	for (DWORD TempLoop = 0; TempLoop < TempNumberOfPixels / vBytesPerPixel; TempLoop++)
	{
		//TempFuzzy = (TempLoop % 20);
		//br ht
		TempFuzzy = (TempLoop % 10);
		//TempFuzzy = 0;
		
		TempPixelInLine = TempLoop % vAquisitionSizeX;
		TempPixelInLine = vAquisitionSizeX - TempPixelInLine - 1;
		TempLine = TempLoop / vAquisitionSizeX;
		if (TempStep == 4)
		if (TempLine >= ((vAquisitionSizeY / 2) - (TempLidWidth / 2) - TempLidWidthAdjust + TempSidewaysJitter))
			TempCockedCap = (TempLine - ((vAquisitionSizeY / 2) - (TempLidWidth / 2) - 
				TempLidWidthAdjust + TempSidewaysJitter)) / 15;

		if ((TempPixelInLine >= TempFoodLine - (1.0 * TempHeightStep) - TempBeltJitter) &&
			(TempPixelInLine <= TempNormalLidBottom - TempBeltJitter))
		{
			TempBottleNeckAdjust = (int)(TempPixelInLine - (TempFoodLine - 
				(1.0 * TempHeightStep) - TempBeltJitter + SmallStepX));

			if (TempBottleNeckAdjust < 0)
				TempBottleNeckAdjust = 0;
			//have the maximum value in for bottle neck
			if (TempBottleNeckAdjust >= (int)
				(((TempRightSide - TempLeftSide) - (TempLidWidth - 2)) / 2))
				TempBottleNeckAdjust = 
				(((TempRightSide - TempLeftSide) - (TempLidWidth - 2)) / 2);
			//else
			//	TempBottleNeckAdjust = 0;
		}
		else
		if (TempPixelInLine > TempNormalLidBottom - TempBeltJitter)
		{  //cap width
			TempBottleNeckAdjust = (((TempRightSide - TempLeftSide) - (TempLidWidth - 2)) / 2);
		}
		else
		{  //width of container
			TempBottleNeckAdjust = 0;//- TempWidthAdjust;//(((TempRightSide - TempLeftSide) - (TempLidWidth - 2)) / 2);
			//have the maximum value in for bottle neck
		}

		//br ht test only
		/*
		if ((TempLine == 2) ||
			(TempLine == 510) ||
			(TempPixelInLine == 2) ||
			(TempPixelInLine ==510))
			*TempBuffer = (BYTE)(0x80);//top and bottom line
		else
		*/
		/*
		//Bump on Left Side
		if (((TempStep == 2) || (TempStep == 4)) &&
			(TempLine > TempLeftSide  -4) && 
			(TempLine < TempLeftSide  + (TempWidthStep + 2)) &&
			(TempPixelInLine > TempBottom + (TempHeightStep * 2 - 8)) && 
			(TempPixelInLine < TempBottom + (TempHeightStep * 2)))
		{
			*TempBuffer = (BYTE)(0x80 + TempFuzzy);//can with food
		}
		else
		*/
		if ((TempPixelInLine == 191) && (TempLine == 0))
		{
			if (vFrameFlag == 1)
				vFrameFlag = 2;//254;
			else
				vFrameFlag = 1;

			//TempHardwareFrameCount = *TempPointerToExtraPixel >> 4;
		}
		//if (TempPixelInLine == 0)
		//	*TempBuffer = 128;
		//else
		//if (TempPixelInLine == 0) 
		//	*TempBuffer = 0;
		//else

		//br ht
		//if (TempPixelInLine) 
		//{
		//	*TempBuffer = (BYTE)(128 + TempFuzzy);
		//}
		//else


		if (TempPixelInLine == 0) 
		{
			*TempBuffer = vFrameFlag;
		}
		else
		if (((TempStep == 5) || (TempStep == 7)) &&
			(TempLine > TempLeftSide  + (TempWidthStep)) && 
			(TempLine < TempLeftSide  + (TempWidthStep + 2)) &&
			//(TempLine < TempLeftSide  + (TempWidthStep + 6)) &&
			(TempPixelInLine > TempBottom + (TempHeightStep * 2 - 4)) && 
			//(TempPixelInLine > TempBottom + (TempHeightStep * 2 - 8)) && 
			(TempPixelInLine < TempBottom + (TempHeightStep * 2)))
		{
			*TempBuffer = (BYTE)(20 + TempFuzzy);  //Blob //cBlackIntensity br ht
			//*TempBuffer = (BYTE)(110);// + TempFuzzy);  //Blob //cBlackIntensity br ht
			//*TempBuffer = (BYTE)(101);// + TempFuzzy);  //Blob //cBlackIntensity br ht
		}
		else
		if ((TempStep == 5) &&
			(TempLine > TempLeftSide  + (2 * TempWidthStep)) && 
			//(TempLine < TempLeftSide  + (2 * TempWidthStep + 6)) &&
			(TempLine < TempLeftSide  + (2 * TempWidthStep + 3)) &&
			//(TempPixelInLine > TempBottom + (TempHeightStep * 3 - 6)) && 
			(TempPixelInLine > TempBottom + (TempHeightStep * 3 - 2)) && 
			(TempPixelInLine < TempBottom + (TempHeightStep * 3)))
		{
			*TempBuffer = (BYTE)(40 + TempFuzzy);  //Blob //cBlackIntensity br ht
			//*TempBuffer = (BYTE)(120);// + TempFuzzy);  //Blob //cBlackIntensity br ht
			//*TempBuffer = (BYTE)(102 + TempFuzzy);  //Blob //cBlackIntensity br ht
		}
		else
			/*
			//br ht test tracer line
		if (TempPixelInLine == ((TempStep * 10) + 20))
		{
			*TempBuffer = 255;  //Tracer Line
		}
		else
		*/
		if ((TempStep == 4) &&
			(TempLine > TempLeftSide  + (5 * TempWidthStep)) && 
			(TempLine < TempLeftSide  + (5 * TempWidthStep + 6)) &&
			(TempPixelInLine > TempBottom + (TempHeightStep * 2 - 5)) && 
			(TempPixelInLine < TempBottom + (TempHeightStep * 2)))
		{
			*TempBuffer = (BYTE)(cWhiteIntensity);  //void
		}
		else
		if ((TempStep == 2) &&
			(TempLine > ((vAquisitionSizeY - 7) / 2) - (TempLidWidth / 2) - TempLidWidthAdjust + TempSidewaysJitter) && 
			(TempLine < ((vAquisitionSizeY - 7) / 2) + (TempLidWidth / 2) + TempLidWidthAdjust + TempSidewaysJitter) && 
			((TempPixelInLine == TempLidBottom + 2 - TempBeltJitter))) 
			//|| (TempPixelInLine == TempLidBottom - 4) ))
		{
			*TempBuffer = (BYTE)(0x90 + TempFuzzy);  //crack in cap seal
		}
		else
		if ((TempLine > ((vAquisitionSizeY - 7) / 2) - (TempLidWidth / 2) - TempLidWidthAdjust + TempSidewaysJitter) && 
			(TempLine < ((vAquisitionSizeY - 7) / 2) + (TempLidWidth / 2) + TempLidWidthAdjust + TempSidewaysJitter) && 
			(TempPixelInLine <= TempLidTop - TempBeltJitter + TempCockedCap - TempLidHeightAdjust) && 
			(TempPixelInLine >= TempLidBottom - TempBeltJitter + TempCockedCap))
		{
			if (TempStep == 6)
				*TempBuffer = (BYTE)(0xb0 + TempFuzzy);  //middle of cap - missing cap
			else
				*TempBuffer = (BYTE)(0x50 + TempFuzzy);  //middle of cap
		}
		else
		if ((TempLine > TempLeftSide + TempBottleNeckAdjust + TempSidewaysJitter) && 
			(TempLine < TempRightSide - TempBottleNeckAdjust + TempSidewaysJitter) &&  
			(TempPixelInLine <= TempLidBottom - TempBeltJitter + TempCockedCap) && 
			(TempPixelInLine >= TempFoodLine + TempFoodAdjust - TempBeltJitter + 1 + (TempLine % 8)))
		{
			*TempBuffer = (BYTE)(0xb0 + TempFuzzy);//air space at top
		}
		else
		if (((TempStep == 4) || (TempStep == 5)) &&
			(TempLine > TempRightSide + 1 - TempBottleNeckAdjust + TempSidewaysJitter) &&  
			(TempPixelInLine <= TempFoodLine - (2 *TempHeightStep) + TempFoodAdjust - TempBeltJitter + (TempLine % 8)) && 
			(TempPixelInLine > TempBottom - TempBeltJitter))
		{
			*TempBuffer = (BYTE)(0x80 + TempFuzzy);//can with food for can in next image
		}
		else
		if ((TempStep == 7) && (TempLine == 4))
		{
			*TempBuffer = 255; //(BYTE)(0xe0 + TempFuzzy);//space between can and can in front so show can in front
		}
		else
		if ((TempLine >= TempLeftSide + TempBottleNeckAdjust + TempSidewaysJitter) && 
			(TempLine < TempRightSide - TempBottleNeckAdjust + TempSidewaysJitter) &&  
			(TempPixelInLine <= TempFoodLine + TempFoodAdjust - TempBeltJitter + (TempLine % 8)) && 
			(TempPixelInLine > TempBottom - TempBeltJitter))
		{
			*TempBuffer = (BYTE)(0x80 + TempFuzzy);//can with food
		}
		else
		if ((TempStep == 6) && 
			(TempLine >= TempLeftSide + TempBottleNeckAdjust  + TempSidewaysJitter) && 
			(TempLine <= TempRightSide - TempBottleNeckAdjust + TempSidewaysJitter) &&
			(TempPixelInLine >= TempBottom - 4 - TempBeltJitter) && 
			(TempPixelInLine <= TempBottom - TempBeltJitter))
		{
			//*TempBuffer = (BYTE)(40 + TempFuzzy);  //Blob //cBlackIntensity br ht
			*TempBuffer = (BYTE)(103 + TempFuzzy);  //Blob //cBlackIntensity br ht
		}
		else
		{
			*TempBuffer = 255; //TempPixelInLine;//(BYTE)(0xe0 + TempFuzzy);  //background
		}
		//br ht
		//if (*TempBuffer > 10)
		//	*TempBuffer = *TempBuffer - 20;
		TempBuffer++; 
		if (vBytesPerPixel == 2)
		{
			BYTE *TempPointerToBufferBefore = TempBuffer - 1;
			*TempBuffer++ = *TempPointerToBufferBefore;
		}
	}

	for (DWORD TempLoop = 0; TempLoop < vAquisitionSizeY; TempLoop++)
	{
		TempBuffer = TempPointerToBuffer - vBytesPerPixel + (vAquisitionSizeX * vBytesPerPixel * (TempLoop + 1));
		*TempBuffer = 1; //is an A Frame
	}
}
/*
void CITIPCDig::SetFENEnable(bool TempEnable)
{
#ifdef UseIFCDLLs
	if (vCaptureModule != NULL)
	{
		vCamera = vCaptureModule->GetCam(0);
		if (vCamera != NULL)
		{
			if (TempEnable)
				vCamera->SetAcqParam(P_FEN_ENABLE, IFC_ENABLE, 0); 
			else
				vCamera->SetAcqParam(P_FEN_ENABLE, IFC_DISABLE, 0); 
		}
	}
#endif
}

void CITIPCDig::SetFENRising(bool TempRising)
{
#ifdef UseIFCDLLs
	if (vCaptureModule != NULL)
	{
		vCamera = vCaptureModule->GetCam(0);
		if (vCamera != NULL)
		{
			if (TempRising)
				vCamera->SetAcqParam(P_FEN_POLARITY, IFC_RISING_EDGE, 0);
			else
				vCamera->SetAcqParam(P_FEN_POLARITY, IFC_FALLING_EDGE, 0);
		}
	}
#endif
}

void CITIPCDig::SetLENSync(bool TempSync)
{
#ifdef UseIFCDLLs
	if (vCaptureModule != NULL)
	{
		vCamera = vCaptureModule->GetCam(0);
		if (vCamera != NULL)
		{
			if (TempSync)
				vCamera->SetAcqParam(P_LEN_SYNC, IFC_SYNC, 0);
			else
				vCamera->SetAcqParam(P_LEN_SYNC, IFC_ASYNC, 0);
		}
	}
#endif
}

void CITIPCDig::SetLENRising(bool TempRising)
{
#ifdef UseIFCDLLs
	if (vCaptureModule != NULL)
	{
		vCamera = vCaptureModule->GetCam(0);
		if (vCamera != NULL)
		{
			if (TempRising)
				vCamera->SetAcqParam(P_LEN_POLARITY, IFC_RISING_EDGE, 0);
			else
				vCamera->SetAcqParam(P_LEN_POLARITY, IFC_FALLING_EDGE, 0);
		}
	}
#endif
}

void CITIPCDig::SetPClockRising(bool TempRising)
{
#ifdef UseIFCDLLs
	if (vCaptureModule != NULL)
	{
		vCamera = vCaptureModule->GetCam(0);
		if (vCamera != NULL)
		{
			if (TempRising)
				vCamera->SetAcqParam(PCD_PCLK_POLARITY , IFC_RISING_EDGE , 0);
			else
				vCamera->SetAcqParam(PCD_PCLK_POLARITY , IFC_FALLING_EDGE , 0);
		}
	}
#endif
}
*/
/*
void CITIPCDig::MakeClipboard()
{
	if (vClipboardImage == NULL)
	if (ThereIsEnoughMemory(sizeof(CImageData), "vClipboardImage"))
	{
		vClipboardImage = new CImageData;
		if (vClipboardImage)
		{
			vClipboardImage->vImageTypeString = "Clipboard Image";
			vClipboardImage->vImageType = cClipboardImageType;
			vClipboardImage->MakeGreyImage(vOriginalBufferSize);
			vClipboardImage->MakeRedImage(vOriginalBufferSize);
			if (vClipboardImage->vRedImage)
			if (vClipboardImage->vGreyImage)
			{
				BYTE *TempPointer = vClipboardImage->vGreyImage;
				BYTE *TempRedPointer = vClipboardImage->vRedImage;
				BYTE TempX = 0;
				for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
				{
					TempX = (BYTE)((TempLoop % vOriginalBufferSizeX) * 4);
					*TempPointer++ = TempX;
					*TempRedPointer++ = 1;
				}
			}
		}
	}
}
*/
bool CITIPCDig::MakeProductAverageImage()
{
	if (vProductAverageImage != NULL)
	{
		delete vProductAverageImage;
		vProductAverageImage = NULL;
	}
	if (vAverageImage)
	if (vAverageCount)
	{
		DWORD *TempSourcePointer = vAverageImage;
		BYTE *TempPointer;
		if (ThereIsEnoughMemory(sizeof(CImageData), "vProductAverageImage"))
			vProductAverageImage = new CImageData;
		if (vProductAverageImage)
		{
			vProductAverageImage->vImageTypeString = "Product Process 1";
			vProductAverageImage->vImageType = cDataImageType;

			vProductAverageImage->MakeGreyImage(vOriginalBufferSize);

			if (vProductAverageImage->vGreyImage)
			{
				TempPointer = vProductAverageImage->vGreyImage;
				for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
				{
					*TempPointer++ = (BYTE)(((*TempSourcePointer++ / (double)vAverageCount)) + .5);
				}
				/*
				FindContainerDimensions(vProductAverageImage->vGreyImage, 
					&vProductAverageImage->vTop, 
					&vProductAverageImage->vBottom,
					&vProductAverageImage->vRight, 
					&vProductAverageImage->vLeft, 
					&vProductAverageImage->vHeight, 
					&vProductAverageImage->vWidth, false, 10);
					*/
			}
			/*
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nMade current Average Image, image to subtract out";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
			*/
			vAverageCount = 0;  
			free(vAverageImage);
			vAverageImage = NULL;
		}
		return true;
	}
	return false;
}

bool CITIPCDig::MakeProductStructureImage()
{
	if (vProductStructureImage != NULL)
	{
		delete vProductStructureImage;
		vProductStructureImage = NULL;
	}
	
	if ((vStructureImage) && (vStructureImage->vGreyImage))
	if (vStructureCount)
	{
		BYTE *TempSourcePointer;
		TempSourcePointer = vStructureImage->vGreyImage;
		BYTE *TempPointer;
		if (ThereIsEnoughMemory(sizeof(CImageData), "vProductStructureImage"))
			vProductStructureImage = new CImageData; 
		if (vProductStructureImage)
		{
			vProductStructureImage->vImageTypeString = "Product Process 2";
			vProductStructureImage->vImageType = cDataImageType;
			vProductStructureImage->MakeGreyImage(vOriginalBufferSize);

			if (vProductStructureImage->vGreyImage)
			{
				TempPointer = vProductStructureImage->vGreyImage;
				for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
				{
					if (*TempSourcePointer < 128)
						*TempPointer = 128 - *TempSourcePointer;
					else
						*TempPointer = 0;
					if (*TempPointer > 128)
						*TempPointer = 128;

					TempPointer++;
					TempSourcePointer++;
				}
			}
			vStructureCount = 0;
		}
		return true;
	}
	return false;
}

bool CITIPCDig::MakeVoidProductStructureImage()
{
	if (vVoidProductStructureImage != NULL)
	{
		delete vVoidProductStructureImage;
		vVoidProductStructureImage = NULL;
	}
	if ((vVoidStructureImage) && (vVoidStructureImage->vGreyImage))
	{
		BYTE *TempSourcePointer;
		TempSourcePointer = vVoidStructureImage->vGreyImage;
		BYTE *TempPointer;
		if (ThereIsEnoughMemory(sizeof(CImageData), "vVoidProductStructureImage"))
			vVoidProductStructureImage = new CImageData; 
		if (vVoidProductStructureImage)
		{
			vVoidProductStructureImage->vImageTypeString = "Void Product Process 2";
			vVoidProductStructureImage->vImageType = cDataImageType;
			vVoidProductStructureImage->MakeGreyImage(vOriginalBufferSize);

			if (vVoidProductStructureImage->vGreyImage)
			{
				TempPointer = vVoidProductStructureImage->vGreyImage;
				for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
				{
					if (*TempSourcePointer < 128)
						*TempPointer = 128 - *TempSourcePointer;
					else
						*TempPointer = 0;

					TempPointer++;
					TempSourcePointer++;
				}
			}
			//MakeVoidProductFactoredStructureImage();
		}
		vStructureCount = 0;
		//delete vVoidStructureImage;
		//vVoidStructureImage = NULL;
		return true;
	}
	return false;
}

void CITIPCDig::LoadStructureImagesFromFiles(bool TempKeepMemoryBuffer, bool TempUseOriginal)
{
	if (!TempKeepMemoryBuffer)
	{
		if (vProductStructureImage != NULL)
		{
			delete vProductStructureImage;
			vProductStructureImage = NULL;
		}

		if (vVoidProductStructureImage != NULL)
		{
			delete vVoidProductStructureImage;
			vVoidProductStructureImage = NULL;
		}

		if (vProductContaminant2StructureImage != NULL)
		{
			delete vProductContaminant2StructureImage;
			vProductContaminant2StructureImage = NULL;
		}
		
		if (vProductContaminant2StructureImageWithNoDilation != NULL)
		{
			delete vProductContaminant2StructureImageWithNoDilation;
			vProductContaminant2StructureImageWithNoDilation = NULL;
		}
	}

	if (vGlobalCurrentProduct)
	{
		CString TempProcessType = "=Process 2";
		vUsingOriginalLearnTemporarily = false;
		if (TempUseOriginal)
		if (HaveLearnImageFile("=Process 2o"))
		{
			vUsingOriginalLearnTemporarily = true;
			TempProcessType = TempProcessType + "o";
		}

		vProductStructureImage = LoadLearnImageFromFile(TempProcessType, vGlobalCurrentProduct,
			vProductStructureImage, "Product Process 2");

		if (vGlobalAllowVoidInspection)
		{
			TempProcessType = "=Process 2v";
			if (TempUseOriginal)
			if (HaveLearnImageFile("=Process 2vo"))
				TempProcessType = TempProcessType + "o";
			if (!(vVoidProductStructureImage = LoadLearnImageFromFile(TempProcessType, vGlobalCurrentProduct, vVoidProductStructureImage, 
				"Product Process 2")))	//if this process type doesn't exist
			{
				TempProcessType = "=VoidProcess 2";	//try old style void process type
				if (TempUseOriginal)
				if (HaveLearnImageFile("=VoidProcess 2o"))
					TempProcessType = TempProcessType + "o";

				vVoidProductStructureImage = LoadLearnImageFromFile(TempProcessType, vGlobalCurrentProduct,
					vVoidProductStructureImage, "Product Process 2");
			}
		}

		TempProcessType = "=Process 2T";
		if (TempUseOriginal)
		if (HaveLearnImageFile("=Process 2To"))
			TempProcessType = "=Process 2To";

		vProductContaminant2StructureImage = LoadLearnImageFromFile(TempProcessType, vGlobalCurrentProduct, vProductContaminant2StructureImage, "Product Process 2T");

		if (vProductContaminant2StructureImage)
		if ((vGlobalLearnState) && (((vGlobalNormalLearnDilateTimes) && (vGlobalCurrentProduct->vSetupHistoryStructureCount > 19) &&
			(vGlobalCurrentProduct->vSetupHistoryStructureCount != vGlobalCurrentProduct->vSetupHistoryAverageCount)) || //if the last learn was a normal learn, and not a quick learn
			((vGlobalQuickLearnDilateTimes) && (vGlobalCurrentProduct->vSetupHistoryStructureCount > 1) &&
			(vGlobalCurrentProduct->vSetupHistoryStructureCount == vGlobalCurrentProduct->vSetupHistoryAverageCount)))) //if the last learn was a quick learn, and not a normal learn
		{
			if (!vProductContaminant2StructureImageWithNoDilation)
			{
				if (ThereIsEnoughMemory(sizeof(CImageData), "vProductContaminant2StructureImageWithNoDilation"))
					vProductContaminant2StructureImageWithNoDilation = new CImageData;

				if (vProductContaminant2StructureImageWithNoDilation)
				{
					vProductContaminant2StructureImageWithNoDilation->vImageTypeString = "Structure 2 No Dilation";
					vProductContaminant2StructureImageWithNoDilation->vImageType = cDataImageType;
					vProductContaminant2StructureImageWithNoDilation->MakeGreyImage(vOriginalBufferSize);
				}
			}

			BYTE TempDilateAmount = vGlobalNormalLearnDilateTimes;
			if (vGlobalCurrentProduct->vSetupHistoryStructureCount == vGlobalCurrentProduct->vSetupHistoryAverageCount)
				TempDilateAmount = vGlobalQuickLearnDilateTimes;

			if ((vProductContaminant2StructureImageWithNoDilation) && (vProductContaminant2StructureImageWithNoDilation->vGreyImage))
			if ((vProductContaminant2StructureImage) && (vProductContaminant2StructureImage->vGreyImage))
			{ //copy the original lean with no dilation into a holder for use later to update and save to file, but not to use in image processing
				CopyMemory(vProductContaminant2StructureImageWithNoDilation->vGreyImage, vProductContaminant2StructureImage->vGreyImage, vOriginalBufferSize);

				//normal learn dilate when read in if still auto improving
				Image *TempScratchImage = NULL;
				if (ThereIsEnoughMemory(vOriginalBufferSize + sizeof(Image), "TempScratchImage"))
					TempScratchImage = im_create(IM_BYTE, vOriginalBufferSizeX, vAquisitionSizeY);
				if ((TempScratchImage) && (TempScratchImage->pix))
				{
					CopyMemory(TempScratchImage->pix, vProductContaminant2StructureImage->vGreyImage, vOriginalBufferSize);

					mvt_erode_rect(TempScratchImage, TempDilateAmount, TempDilateAmount, EDGE_IGNORE);

					CopyMemory(vProductContaminant2StructureImage->vGreyImage, TempScratchImage->pix, vOriginalBufferSize);

					int TempStart = vGlobalCurrentProduct->vReferenceTop / 4 * 3;

					if (!vGlobalCurrentProduct->vLowerRimHeightInPixels)
					if ((vProductAverageImage) && (vProductAverageImage->vGreyImage))
						vGlobalCurrentProduct->CalculateLowerRimHeight(vProductAverageImage->vGreyImage, NULL);

					if (vGlobalCurrentProduct->vLowerRimHeightInPixels)
						TempStart = (int)(vGlobalCurrentProduct->vLowerRimHeightInPixels - (vGlobalPixelsPerUnitInHeight / 2));

					//fill the variable fill area in so don't get false rejects
					if (vGlobalCurrentProduct->vSetupHistoryAverageDensity)
					if ((vGlobalCurrentProduct->vReferenceLeft) && (vGlobalCurrentProduct->vReferenceRight) && (vGlobalCurrentProduct->vReferenceTop))
					if ((vGlobalCurrentProduct->vLowerRimHeightInPixels > (vGlobalPixelsPerUnitInHeight / 2)) && (vGlobalCurrentProduct->vLowerRimHeightInPixels < vOriginalBufferSizeX - (vGlobalPixelsPerUnitInHeight / 2)))
					{
						BYTE TempStartFade = (BYTE)vGlobalCurrentProduct->vLowerRimHeightInPixels;
						if (vGlobalCurrentProduct->vReferenceTop > (BYTE)(vGlobalCurrentProduct->vLowerRimHeightInPixels + (vGlobalPixelsPerUnitInHeight / 2)))
							TempStartFade = (BYTE)(vGlobalCurrentProduct->vLowerRimHeightInPixels + (vGlobalPixelsPerUnitInHeight / 2));

						for (WORD TempLineLoop = vGlobalCurrentProduct->vReferenceLeft; TempLineLoop < vGlobalCurrentProduct->vReferenceRight; TempLineLoop++)
						{
							BYTE *TempPointer = vProductContaminant2StructureImage->vGreyImage + (TempLineLoop * vOriginalBufferSizeX) + TempStart;
							for (WORD TempPixelLoop = TempStart; TempPixelLoop < vGlobalCurrentProduct->vReferenceTop; TempPixelLoop++)
							{
								double TempFillValue = vGlobalCurrentProduct->vSetupHistoryAverageDensity;
								if (TempPixelLoop > TempStartFade)
								{ //want 255 at top of container, want average density 1/2 inch above rim
									double TempPercentTheWayToTop = 0;
									if (vGlobalCurrentProduct->vReferenceTop - TempStartFade)
										TempPercentTheWayToTop = (double)(TempPixelLoop - TempStartFade) / (double)(vGlobalCurrentProduct->vReferenceTop - TempStartFade);
									double TempRangeToCover = 255 - vGlobalCurrentProduct->vSetupHistoryAverageDensity;
									if (TempFillValue + (TempRangeToCover * TempPercentTheWayToTop) < 255)
										TempFillValue = TempFillValue + (TempRangeToCover * TempPercentTheWayToTop);
									else
										TempFillValue = 255;
								}

								BYTE TempFillValueB = (BYTE)TempFillValue;
								if (*TempPointer > TempFillValueB)
									*TempPointer = TempFillValueB;

								TempPointer++;
							}
						}
					}

					im_delete(TempScratchImage);
				}
			}
		}
	}
}
/*
void CITIPCDig::MakeProductFactoredStructureImage(bool TempKeepMemoryBuffer)
{
	if (!TempKeepMemoryBuffer)
	{
		if (vProductFactoredStructureImage != NULL)
		{
			delete vProductFactoredStructureImage;
			vProductFactoredStructureImage = NULL;
		}
	}
	if (vGlobalCurrentProduct)
	if (vProductAverageImage)
	if (vProductStructureImage)
	{
		if (!vProductFactoredStructureImage)
		if (ThereIsEnoughMemory(sizeof(CImageData), "vProductFactoredStructureImage"))
			vProductFactoredStructureImage = new CImageData;
		if (vProductFactoredStructureImage)
		{
			vProductFactoredStructureImage->
				vImageTypeString = "Product Process 2";
			vProductFactoredStructureImage->vImageType = cDataImageType;
			vProductFactoredStructureImage->MakeGreyImage(vOriginalBufferSize);
			if (vProductFactoredStructureImage->vGreyImage)
				CopyMemory(
					vProductFactoredStructureImage->vGreyImage,
					vProductStructureImage->vGreyImage,
					vOriginalBufferSize);

			double TempFactor = 1.0;
			if (vGlobalCurrentProduct->vStructureAdjustFactor)
				TempFactor = vGlobalCurrentProduct->vStructureAdjustFactor;

			BYTE *TempPointer = vProductFactoredStructureImage->vGreyImage;
			for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
			{
				if (*TempPointer)
				{
					double TempDouble = ((double)*TempPointer * 
						TempFactor) -
						vGlobalCurrentProduct->vStructureAdjustSubtractiveFactor + .5;
					if (TempDouble > 128)
						TempDouble = 128;
					if (TempDouble < 0)
						TempDouble = 0;
					*TempPointer = (BYTE)TempDouble;
				}
				TempPointer++;
			}
		}
	}
}

void CITIPCDig::MakeVoidProductFactoredStructureImage()
{
	if (vVoidProductFactoredStructureImage != NULL)
	{
		delete vVoidProductFactoredStructureImage;
		vVoidProductFactoredStructureImage = NULL;
	}
	if (vGlobalCurrentProduct)
	if (vVoidProductStructureImage)
	if (vVoidProductStructureImage->vGreyImage)
	{
		if (ThereIsEnoughMemory(sizeof(CImageData), "vVoidProductFactoredStructureImage"))
			vVoidProductFactoredStructureImage = new CImageData;
		if (vVoidProductFactoredStructureImage)
		{
			vVoidProductFactoredStructureImage->
				vImageTypeString = "Void Product Process 2";
			vVoidProductFactoredStructureImage->vImageType = cDataImageType;
			vVoidProductFactoredStructureImage->MakeGreyImage(vOriginalBufferSize);
			if (vVoidProductFactoredStructureImage->vGreyImage)
				CopyMemory(
					vVoidProductFactoredStructureImage->vGreyImage,
					vVoidProductStructureImage->vGreyImage,
					vOriginalBufferSize);

			double TempFactor = 1.0;
			if (vGlobalCurrentProduct->vStructureAdjustFactor)
				TempFactor = vGlobalCurrentProduct->vStructureAdjustFactor;

			BYTE *TempPointer = vVoidProductFactoredStructureImage->vGreyImage;
			for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
			{
				double TempDouble = (double)*TempPointer * 
					TempFactor - 
					vGlobalCurrentProduct->vStructureAdjustSubtractiveFactor + .5;
				if (TempDouble > 128)
					TempDouble = 128;
				if (TempDouble < 0)
					TempDouble = 0;
				*TempPointer = (BYTE)TempDouble;
				TempPointer++;
			}
		}
	}
}
*/
void CITIPCDig::MaskOffBottomOfImage(BYTE *TempBufferPointer, double TempBottom)
{
	BYTE *TempImagePointer;
	//mask off bottom of image as conveyor can give problems
	if (TempBufferPointer)
	if (TempBottom > 0)
	{
		int TempPixelsX = (WORD)(TempBottom * vGlobalPixelsPerUnitInHeight);
		if (TempPixelsX > 0)
		for (DWORD TempLoopY = 0; TempLoopY < vAquisitionSizeY; TempLoopY++)
		{
			TempImagePointer = TempBufferPointer + (TempLoopY * vOriginalBufferSizeX);
			for (int TempLoopX = 0; TempLoopX < TempPixelsX; TempLoopX++)
				*TempImagePointer++ = 128;
		}
	}
}

void CITIPCDig::SubtractProductAverageImage(BYTE *TempBufferPointer,
																						CImageData *TempAverageImage)
{
	if ((TempAverageImage) && (TempAverageImage->vGreyImage))
	if (TempBufferPointer)
	{
		BYTE *TempImagePointer;
		TempImagePointer = TempBufferPointer;
		BYTE *TempAverageImagePointer;
		TempAverageImagePointer = TempAverageImage->vGreyImage;
		for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
		{
			if (*TempImagePointer >= *TempAverageImagePointer)
				*TempImagePointer = 128;
			else
			{
				//make sure does not wrap around and become large
				//problem, if this value should go negitive, it gets clipped
				if (*TempAverageImagePointer < ((*TempImagePointer) + 128))
					*TempImagePointer = (*TempImagePointer) + 128 - (*TempAverageImagePointer);
				else
					*TempImagePointer = 0;  //if would have wrapped around, set to zero
				if (*TempImagePointer > 128)  //remove plain background by setting to average
					*TempImagePointer = 128;
			}
			TempImagePointer++;
			TempAverageImagePointer++;
		}
	}
}
/*
void CITIPCDig::SubtractAverageImage(BYTE *TempBufferPointer)
{
	if (vAverageCount)
	if (vAverageImage)
	if (TempBufferPointer)
	{
		BYTE *TempImagePointer;
		TempImagePointer = TempBufferPointer;
		DWORD *TempAverageImagePointer = vAverageImage;
		for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
		{
			BYTE TempAverageValue = (BYTE)((*TempAverageImagePointer / (double)vAverageCount) + .5);
			if (*TempImagePointer >= TempAverageValue)
				*TempImagePointer = 128;
			else
			{
				//make sure does not wrap around and become large
				//problem, if this value should go negitive, it gets clipped
				if (TempAverageValue < ((*TempImagePointer) + 128))
					*TempImagePointer = (*TempImagePointer) + 128 - (TempAverageValue);
				else
					*TempImagePointer = 0;  //if would have wrapped around, set to zero
				if (*TempImagePointer > 128)  //remove plain background by setting to average
					*TempImagePointer = 128;
			}
			TempImagePointer++;
			TempAverageImagePointer++;
		}
	}
}
*/
void CITIPCDig::SubtractVoidProductAverageImage(BYTE *TempBufferPointer)
{
	if ((vProductAverageImage) && (vProductAverageImage->vGreyImage))
	if (TempBufferPointer)
	{
		BYTE *TempImagePointer;
		TempImagePointer = TempBufferPointer;
		BYTE *TempAverageImagePointer;
		TempAverageImagePointer = vProductAverageImage->vGreyImage;
		for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
		{
			if (*TempImagePointer <= *TempAverageImagePointer)
				*TempImagePointer = 128;
			else
			{
				int TempInt = 128 - (*TempImagePointer - *TempAverageImagePointer);
				if (TempInt > 128)
					TempInt = 128;
				if (TempInt < 0)
					TempInt = 0;
				*TempImagePointer = (BYTE)TempInt;
			}
			TempImagePointer++;
			TempAverageImagePointer++;
		}
	}
}
/*
void CITIPCDig::SubtractVoidAverageImage(BYTE *TempBufferPointer)
{
	if (vAverageCount)
	if (vAverageImage)
	if (TempBufferPointer)
	{
		BYTE *TempImagePointer;
		TempImagePointer = TempBufferPointer;
		DWORD *TempAverageImagePointer;
		TempAverageImagePointer = vAverageImage;
		for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
		{
			BYTE TempAverageValue = (BYTE)(*TempAverageImagePointer / vAverageCount);
			if (*TempImagePointer <= TempAverageValue)
				*TempImagePointer = 128;
			else
			{
				*TempImagePointer = (*TempImagePointer) - (TempAverageValue);
				if (*TempImagePointer < 128)
					*TempImagePointer = *TempImagePointer + 128;
				else
					*TempImagePointer = 255;
			}
			TempImagePointer++;
			TempAverageImagePointer++;
		}
	}
}
*/
void CITIPCDig::AddProductHistogramImage(BYTE *TempBufferPointer)
{
	if (TempBufferPointer)
	if ((vProductHistogramImage) && (vProductHistogramImage->vGreyImage))
	{
		BYTE *TempImagePointer;
		TempImagePointer = TempBufferPointer;
		BYTE *TempHistogramImagePointer;
		TempHistogramImagePointer = vProductHistogramImage->vGreyImage;
		for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
		{
			//5/11/2004
			if (*TempHistogramImagePointer == 255)
				*TempImagePointer = 255;
			TempImagePointer++;
			TempHistogramImagePointer++;
		}
	}
}
/*
void CITIPCDig::AddProductGlassHistogramImage(BYTE *TempBufferPointer)
{
	if (vProductGlassHistogramImage != NULL)
	{
		WORD TempWord = 0;
		BYTE *TempImagePointer = TempBufferPointer;
		BYTE *TempGlassHistogramImagePointer = vProductGlassHistogramImage->vGreyImage;
		for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
		{
			if (*TempGlassHistogramImagePointer == 255)
				*TempImagePointer = 255;

			TempImagePointer++;
			TempGlassHistogramImagePointer++;
		}
	}
}

void CITIPCDig::AddProductGlassStructureImage(BYTE *TempBufferPointer)
{
	if (vProductGlassStructureImage != NULL)
	{
		WORD TempWord = 0;
		BYTE *TempImagePointer = TempBufferPointer;
		BYTE *TempStructureImagePointer = vProductStructureImage->vGreyImage;
		for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
		{
			if (*TempStructureImagePointer)
			{
				TempWord = (WORD)*TempImagePointer + (WORD)*TempStructureImagePointer;
				if (TempWord > 255)
					*TempImagePointer = 255;
				else
					*TempImagePointer = (BYTE)TempWord;
			}
			TempImagePointer++;
			TempStructureImagePointer++;
		}
	}
}
*/
void CITIPCDig::AddProductStructureImage(BYTE *TempBufferPointer)
{
	if (TempBufferPointer)
	if ((vProductStructureImage) && (vProductStructureImage->vGreyImage))
	{
		double TempDouble = 0;
		BYTE *TempImagePointer;
		TempImagePointer = TempBufferPointer;
		BYTE *TempStructureImagePointer;
		TempStructureImagePointer = vProductStructureImage->vGreyImage;
		for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
		{
			if (*TempImagePointer != 128)
				if (*TempStructureImagePointer)
			{
				TempDouble = *TempImagePointer + *TempStructureImagePointer;
				if (TempDouble > 128)
					TempDouble = 128;
				*TempImagePointer = (BYTE)(TempDouble + .5);
			}
			TempImagePointer++;
			TempStructureImagePointer++;
		}
	}
}

void CITIPCDig::AddVoidProductStructureImage(BYTE *TempBufferPointer)
{
	if (TempBufferPointer)
	if ((vVoidProductStructureImage) && (vVoidProductStructureImage->vGreyImage))
	{
		int TempInt = 0;
		BYTE *TempImagePointer;
		TempImagePointer = TempBufferPointer;
		BYTE *TempStructureImagePointer;
		TempStructureImagePointer = vVoidProductStructureImage->vGreyImage;
		for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
		{
			if (*TempImagePointer != 128)
			{
				TempInt = *TempImagePointer + *TempStructureImagePointer;
				if (TempInt > 128)
					TempInt = 128;
				*TempImagePointer = (BYTE)TempInt;
			}
			TempImagePointer++;
			TempStructureImagePointer++;
		}
	}
}

void CITIPCDig::AddProductContaminant2StructureImage(BYTE *TempBufferPointer)
{
	if (TempBufferPointer)
	if ((vProductContaminant2StructureImage) && (vProductContaminant2StructureImage->vGreyImage))
	{
		BYTE *TempImagePointer = TempBufferPointer;
		BYTE *TempStructureImagePointer = vProductContaminant2StructureImage->vGreyImage;
		for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
		{
			if (*TempImagePointer < *TempStructureImagePointer)
			{
				*TempImagePointer = 255 - (*TempStructureImagePointer - *TempImagePointer);
			}
			else
				*TempImagePointer = 255;

			TempImagePointer++;
			TempStructureImagePointer++;
		}
	}
}

void CITIPCDig::FillMainDisplayBuffer()  //put gray scale with inspx logo
{
	if (vMainImageDisplayBuffer)
	{
		BYTE *TempDataPoint;
		//when load a product, show average image if have one
		//when hit run, don't show average image
		if ((!vMainDisplayIsDefaultImage) && (vProductAverageImage) && (vProductAverageImage->vGreyImage))
		{
			double TempScaleX = (double)vAquisitionSizeY / 
				(double)vMainDisplayImageSizeX;
			double TempScaleY = (double)vOriginalBufferSizeX / 
				(double)vMainDisplayImageSizeY;

			BYTE *TempDisplayBuffer;
			TempDisplayBuffer = vMainImageDisplayBuffer;
			BYTE *TempNewImageBuffer;
			BYTE *TempHistogramImageBuffer;
			DWORD TempDelta = 0;

			for(WORD TempLoopY = 0; TempLoopY < vMainDisplayImageSizeY; TempLoopY++)
			for(WORD TempLoopX = 0; TempLoopX < vMainDisplayImageSizeX; TempLoopX++)
			{
				TempDelta = ((DWORD)(((double)TempScaleY * 
						(double)(vMainDisplayImageSizeY - 1 - TempLoopY))) + //number of pixels up this line
					(vOriginalBufferSizeX * (DWORD)(((double)( TempLoopX) * 
						(double)TempScaleX)))); //number of lines up the frame
				if (TempDelta >= vOriginalBufferSize)
				{
					TempDelta = vOriginalBufferSize - 1;
					//ReportErrorMessage("Error-Scaling Problem 15", cEMailInspx,32000);
				}

				TempNewImageBuffer = TempDelta + vProductAverageImage->vGreyImage;
				*TempDisplayBuffer = *TempNewImageBuffer;

				if ((vProductHistogramImage) && (vProductHistogramImage->vGreyImage))
				if (PasswordOK(cTemporaryInspxPassword,false))
				if (vShowAverageMaskedWithHistogram)
				{
					TempHistogramImageBuffer = TempDelta + vProductHistogramImage->vGreyImage;
					if (*TempHistogramImageBuffer == 255)
						*TempDisplayBuffer = 255;
				}

				TempDisplayBuffer++;  
			}
			vMainDisplayIsDefaultImage = true;
		}
		else
		{
			vMainDisplayIsDefaultImage = false;
			TempDataPoint = vMainImageDisplayBuffer;
			for (WORD TempLoopY = 0; TempLoopY < vMainDisplayImageSizeY; TempLoopY++)
			{
				for (WORD TempLoopX = 0; TempLoopX < vMainDisplayImageSizeX; TempLoopX++)
				{
					if ((TempLoopX/256)%2 == 1)
						*TempDataPoint = 256 - (TempLoopX - 256);
					else
						*TempDataPoint = (BYTE)TempLoopX;
					if (*TempDataPoint == 0 && ((TempLoopX/256)%2 == 1))
						*TempDataPoint = 255;
					else if (*TempDataPoint == 0 && ((TempLoopX/256)%2 == 0))
						*TempDataPoint = 1;

					TempDataPoint++;
				}
			}

			CString TempStringFileName = "Inspx Small Red Mask.BMP";
			TempStringFileName = TempStringFileName;
			int TempHeight = 0;
			int TempWidth = 0;
			int TempBitsPerPixel = 0;
			int TempType = 0;
			LPTSTR TempString = TempStringFileName.GetBuffer(TempStringFileName.GetLength());
			CFileStatus TempFileStatus;
			if (CFile::GetStatus(TempStringFileName, TempFileStatus))
			if (ThereIsEnoughMemory((vMainDisplayImageSizeX * vMainDisplayImageSizeY) + 
				sizeof(Image), "Temporary Inspx Image Buffer"))
			{
				Image *TempImageIn = NULL;
				TempImageIn = im_create(IM_BYTE, vMainDisplayImageSizeX, 
					vMainDisplayImageSizeY);
				if ((TempImageIn) && (TempImageIn->pix))
				{
					CW2A TempFileNameString(TempString);			
					Erflag TempError = im_file_read(TempImageIn, TempFileNameString, IM_BMP_FILE);
					if (TempError == IM_OK)
					{
						BYTE *TempDataPointFile;
						TempDataPointFile = TempImageIn->pix;
						TempDataPoint = vMainImageDisplayBuffer;
						int* TempWidth = new int ;
						int* TempHeight = new int ;
						int* TempDepth = new int ;
						im_file_get_size(TempFileNameString,IM_BMP_FILE,TempWidth,TempHeight,TempDepth,NULL);
						WORD TempCenterX = (vMainDisplayImageSizeX/2) - (*TempWidth/2);
						WORD TempCenterY = (vMainDisplayImageSizeY/2) - (*TempHeight/2);
							TempDataPoint += (TempCenterX + (TempCenterY * vMainDisplayImageSizeX));
						for (WORD TempLoopY = 0; TempLoopY < vMainDisplayImageSizeY; TempLoopY++)
						{
							for (WORD TempLoopX = 0; TempLoopX < vMainDisplayImageSizeX; TempLoopX++)
							{
								if ((*TempDataPointFile++ != 14) && (TempLoopX < *TempWidth) && (TempLoopY < *TempHeight))
									*TempDataPoint = 0;

								TempDataPoint++;
							}
						}
					}
				}
				if (TempImageIn)
					im_delete(TempImageIn);
			}
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "Main Image Display Buffer NULL";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}
}


void CITIPCDig::SaveAverageImageToFile(CProduct *TempCurrentProduct)
{
	if ((vProductAverageImage) && (vProductAverageImage->vGreyImage))
	{
		ReportErrorMessage("Save New Learn", cAction,0);

		CTime TempTime;
		TempTime = CTime::GetCurrentTime();
		TempCurrentProduct->vHistoryTookAverageDate = TempTime.Format("%#I:%M:%S%p, %B %d, %Y");
		
		CString TempString1 = "=Process 1";
		TempString1 =  *TempCurrentProduct->GetProductName() + TempString1;
		if (!vUsingLocalSetupDataFile)
			TempString1 = cSetupDataDirectory + TempString1;
		else
			TempString1 = vGlobalCurrentDirectory + TempString1;

		SaveLearnFile(TempString1, vProductAverageImage->vGreyImage, vOriginalBufferSizeX, vOriginalBufferSizeY, (WORD)vBitsPerPixel, true);	//true - create backup of backup
		vALearnedImageHasChanged = true;
	}
}

void CITIPCDig::SaveStructureImageToFile(CProduct *TempCurrentProduct, bool TempMakeOriginal)
{
	if (TempCurrentProduct)
	if ((vProductStructureImage) && (vProductStructureImage->vGreyImage))
	{
		CString TempString1 = "=Process 2";
		TempString1 =  *TempCurrentProduct->GetProductName() + TempString1;
		if (!vUsingLocalSetupDataFile)
			TempString1 = cSetupDataDirectory + TempString1;
		else
			TempString1 = vGlobalCurrentDirectory + TempString1;

		SaveLearnFile(TempString1, vProductStructureImage->vGreyImage, vOriginalBufferSizeX, vOriginalBufferSizeY, (WORD)vBitsPerPixel, true);	//true make backup

		if (TempMakeOriginal)  //create new original image file on disk for later restore if needed
		{ //otherwise must have added to learn so do not create original to restore from
			vGlobalCurrentProduct->vAddToLearnCount = 0;

			TempString1 = "=Process 2o";
			TempString1 =  *TempCurrentProduct->GetProductName() + TempString1;
			if (!vUsingLocalSetupDataFile)
				TempString1 = cSetupDataDirectory + TempString1;
			else
				TempString1 = vGlobalCurrentDirectory + TempString1;
			
			SaveLearnFile(TempString1, vProductStructureImage->vGreyImage, vOriginalBufferSizeX, vOriginalBufferSizeY, (WORD)vBitsPerPixel, true);	//true make backup
		}
		PostThreadMessage(vGlobalCallingThreadID,cCheckLearnQualityMessage, 0, 0);
		vALearnedImageHasChanged = true;
	}
}

void CITIPCDig::SaveContaminant2StructureImageToFile(CProduct *TempCurrentProduct, CImageData *TempContaminant2Structure, bool TempOriginalImage)
{
	if (TempCurrentProduct)
	if ((TempContaminant2Structure) && (TempContaminant2Structure->vGreyImage))
	{
		CString TempString1 = "=Process 2T";
		TempString1 =  *TempCurrentProduct->GetProductName() + TempString1;
		if (!vUsingLocalSetupDataFile)
			TempString1 = cSetupDataDirectory + TempString1;
		else
			TempString1 = vGlobalCurrentDirectory + TempString1;
		
		SaveLearnFile(TempString1, TempContaminant2Structure->vGreyImage, vOriginalBufferSizeX, vOriginalBufferSizeY, (WORD)vBitsPerPixel, true);	//true make backup

		if (TempOriginalImage)
		{
			TempString1 = "=Process 2To";
			TempString1 =  *TempCurrentProduct->GetProductName() + TempString1;
			if (!vUsingLocalSetupDataFile)
				TempString1 = cSetupDataDirectory + TempString1;
			else
				TempString1 = vGlobalCurrentDirectory + TempString1;
			
			SaveLearnFile(TempString1, TempContaminant2Structure->vGreyImage, vOriginalBufferSizeX, vOriginalBufferSizeY, (WORD)vBitsPerPixel, true);	//true make backup			
		}
		if ((!vStructureEnabled) && (!vAverageEnabled))
		if (vContaminant2StructureImage)
		{
			delete vContaminant2StructureImage;
			vContaminant2StructureImage = NULL;
		}
		vALearnedImageHasChanged = true;
	}
}

void CITIPCDig::SaveVoidStructureImageToFile(CProduct *TempCurrentProduct, bool TempSaveOriginalAlso)
{
	if (vGlobalAllowVoidInspection)
	if ((vVoidProductStructureImage) && (vVoidProductStructureImage->vGreyImage))
	{
		CString TempString1 = "=Process 2v";
		TempString1 =  *TempCurrentProduct->GetProductName() + TempString1;
		if (!vUsingLocalSetupDataFile)
			TempString1 = cSetupDataDirectory + TempString1;
		else
			TempString1 = vGlobalCurrentDirectory + TempString1;

		SaveLearnFile(TempString1, 
					vVoidProductStructureImage->vGreyImage,
					vOriginalBufferSizeX,
					vOriginalBufferSizeY,
					(WORD)vBitsPerPixel,
					true);	//true make backup
		
		if (TempSaveOriginalAlso)
		{
			CString TempString1 = "=Process 2vo";
			TempString1 =  *TempCurrentProduct->GetProductName() + TempString1;
			if (!vUsingLocalSetupDataFile)
				TempString1 = cSetupDataDirectory + TempString1;
			else
				TempString1 = vGlobalCurrentDirectory + TempString1;
		
			SaveLearnFile(TempString1, 
					vVoidProductStructureImage->vGreyImage,
					vOriginalBufferSizeX,
					vOriginalBufferSizeY,
					(WORD)vBitsPerPixel,
					true);	//true make backup
		}
	}
}

void CITIPCDig::SaveThresholdImageInHistogram(CProduct *TempCurrentProduct, BYTE TempThreshold, BYTE *TempPointerToSourceImage, BYTE TempDilateTimes, BYTE TempLowerThreshold)
{
	CString TempString1 = "=Process 3";
	TempString1 =  *TempCurrentProduct->GetProductName() + TempString1;
	if (!vUsingLocalSetupDataFile)
		TempString1 = cSetupDataDirectory + TempString1;
	else
		TempString1 = vGlobalCurrentDirectory + TempString1;
	int TempLength = TempString1.GetLength() + 4;
	if (TempLength > MAX_PATH)
	{
		CNoticeDialog TempNoticeDialog;
		CString TempString = "\n\n\nSetup Name more the MAX_PATH characters, setup save aborted";
		TempNoticeDialog.vNoticeText = TempString;
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
		TempLength = MAX_PATH;
	}
	else
	if (TempPointerToSourceImage)
	{
		Image *TempThresholdImage = NULL;
		if (ThereIsEnoughMemory(vOriginalBufferSize + sizeof(Image), "Threshold Save Buffers"))
			TempThresholdImage = im_create(IM_BYTE, vOriginalBufferSizeX, vOriginalBufferSizeY);
		if ((TempThresholdImage) && (TempThresholdImage->pix))
		{
			CopyMemory(
				TempThresholdImage->pix,
				TempPointerToSourceImage,
				vOriginalBufferSize);

			//make any pixel white that is above threshold
			ThresholdBuffer(TempThresholdImage->pix, vOriginalBufferSize,TempThreshold);

			//after threshold white image fill in white spots in container
			WORD TempStartRight = vGlobalCurrentProduct->vReferenceRight;
			if (!TempStartRight)
				TempStartRight = vOriginalBufferSizeY - 3;
			if (TempStartRight + 10 < vOriginalBufferSizeY)
				TempStartRight = TempStartRight + 10;
			if (TempStartRight > (WORD)(vOriginalBufferSizeY - 3))
				TempStartRight = (WORD)(vOriginalBufferSizeY - 3);

			WORD TempStartLeft = vGlobalCurrentProduct->vReferenceLeft;
			if (TempStartLeft > 10)
				TempStartLeft = TempStartLeft - 10;
			if (TempStartLeft < 3)
				TempStartLeft = 3;

			for (WORD TempLoopX = 0; TempLoopX < vOriginalBufferSizeX; TempLoopX++)
			{
				WORD TempLeftTransition = 0;
				WORD TempRightTransition = 0;
				BYTE *TempPointer = TempThresholdImage->pix + TempLoopX + (TempStartLeft * vOriginalBufferSizeX);
				BYTE *TempPreviousPixelPointer = NULL;
				//Scan from the left edge towards the right and look for the first light to dark transition
				for (WORD TempLoopYL = TempStartLeft; TempLoopYL < TempStartRight; TempLoopYL++)
				{
					TempPreviousPixelPointer = TempPointer;
					TempPointer = TempPointer + vOriginalBufferSizeX; //go to next pixel to the right
					if ((*TempPreviousPixelPointer == 255) && (*TempPointer == 0)) //found light to dark transition, so mark and exit loop
					{
						TempLeftTransition = TempLoopYL + 1;
						TempLoopYL = vOriginalBufferSizeY;
					}
				}

				//Scan from the right edge towards the left and look for the first light to dark transition
				TempPointer = TempThresholdImage->pix + TempLoopX + 
					((vOriginalBufferSizeY - 1 - (vOriginalBufferSizeY - TempStartRight)) * vOriginalBufferSizeX);
				for (WORD TempLoopYR = TempStartRight; TempLoopYR > TempStartLeft; TempLoopYR--)
				{
					TempPreviousPixelPointer = TempPointer;
					TempPointer = TempPointer - vOriginalBufferSizeX; //go to next pixel to the left
					if ((*TempPreviousPixelPointer == 255) && (*TempPointer == 0)) //found light to dark transition, so mark and exit loop
					{
						TempRightTransition = TempLoopYR - 1;
						TempLoopYR = 2;
					}
				}

				//fill in line between left and right transition so it is all black
				if ((TempLeftTransition) && (TempRightTransition) && (TempLeftTransition < TempRightTransition))
				{
					TempPointer = TempThresholdImage->pix + TempLoopX + (TempLeftTransition * vOriginalBufferSizeX);
					for (WORD TempLoopX = TempLeftTransition; TempLoopX < TempRightTransition; TempLoopX++)
					{
						*TempPointer = 0;  //make this pixel black so it included in the histogram mask
						TempPointer = TempPointer + vOriginalBufferSizeX; //go to next pixel to the right
					}
				}
			}

			//remove dark aras to mask them off, by making any pixel white that is below lower threshold
			if (TempLowerThreshold)
			{//if value is below lower threshold, make 255 so will be above next threshold and white
				BYTE *TempDestinationPointer = TempThresholdImage->pix;
				BYTE *TempSourcePointer = TempPointerToSourceImage;
				for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
				{
					if (*TempSourcePointer < TempLowerThreshold)
						*TempDestinationPointer = 255;
					TempDestinationPointer++;
					TempSourcePointer++;
				}
			}

			//erode dark areas down away from edges
			if (TempThresholdImage) 
			{
				if (TempDilateTimes)
				if (vGlobalCurrentProduct)
				if (vGlobalCurrentProduct->vInspection[0])
				{
					Image *TempImageOut = NULL;
					TempImageOut = im_create(IM_BYTE, vOriginalBufferSizeX, vOriginalBufferSizeY);
					if ((TempImageOut) && (TempImageOut->pix))
					{
						for (BYTE TempLoop = 0; TempLoop < TempDilateTimes; TempLoop++)
							vGlobalCurrentProduct->vInspection[0]->DilateImage(TempThresholdImage, TempImageOut);
					}
					if (TempImageOut)
						im_delete(TempImageOut);
				}
//				TempImageFile->WriteFile(TempThresholdImage->pix, vOriginalBufferSizeX, vOriginalBufferSizeY, (WORD)vBitsPerPixel);

				SaveLearnFile(TempString1, TempThresholdImage->pix, vOriginalBufferSizeX, vOriginalBufferSizeY, (WORD)vBitsPerPixel, true);	//true make backup

				if (vProductHistogramImage != NULL)
				{
					delete vProductHistogramImage;
					vProductHistogramImage = NULL;
				}
				if (ThereIsEnoughMemory(sizeof(CImageData), "vProductHistogramImage"))
					vProductHistogramImage = new CImageData;
				if (vProductHistogramImage)
				{
					vProductHistogramImage->vImageTypeString = "Product Process 3";
					vProductHistogramImage->vImageType = cDataImageType;
					vProductHistogramImage->MakeGreyImage(vOriginalBufferSize);

					if (vProductHistogramImage->vGreyImage)
					{
						CopyMemory(vProductHistogramImage->vGreyImage, TempThresholdImage->pix, vOriginalBufferSize);
					}
				}
			}
			if (TempThresholdImage)
				im_delete(TempThresholdImage);
//			delete TempImageFile;
		}
	}
}

void CITIPCDig::CopyAverageBuffer(DWORD *TempFrom, BYTE *TempTo, DWORD TempSize)
{
	DWORD TempDivider = vAverageCount;
	if (TempDivider == 0)
		TempDivider = 1;
	if ((TempFrom) && (TempTo))
	{
		for (DWORD TempLoop = 0; TempLoop < TempSize; TempLoop++)
		{
			*TempTo++ = (BYTE)(*TempFrom++ / TempDivider);
		}
	}
	else
		ReportErrorMessage("Error-CopyAverageBuffer Pointer Null", cEMailInspx,32000);
}

void CITIPCDig::MakeAverageImageBuffer()
{
	if (vAverageImage)
	{
		free(vAverageImage);
		vAverageImage = NULL;
	}
	if (ThereIsEnoughMemory(vOriginalBufferSize * sizeof(DWORD),"Average Image Buffer"))
	{
		vAverageImage = (DWORD*)malloc(vOriginalBufferSize * sizeof(DWORD));
		//fill buffer with zeros
		if (vAverageImage)
			ZeroMemory(vAverageImage, vOriginalBufferSize * sizeof(DWORD));
	}
	vAverageCount = 0;
}

void CITIPCDig::MakeStructureImageBuffer()
{
	if ((!vStructureEnabled) && (!vAverageEnabled))
	{
	if (vStructureImage != NULL)
	{
		delete vStructureImage;
		vStructureImage = NULL;
	}
	if (ThereIsEnoughMemory(sizeof(CImageData), "vStructureImage"))
		vStructureImage = new CImageData;
	if (vStructureImage)
	{
		//fill buffer with 128 because are finding smallest value below 128
		vStructureImage->vImageTypeString = "Product Process 2";
		vStructureImage->vImageType = cDataImageType;
		vStructureImage->MakeGreyImage(vOriginalBufferSize);

		if (vStructureImage->vGreyImage)
			FillMemory(vStructureImage->vGreyImage, vOriginalBufferSize,0x80);
	}

	if (vContaminant2StructureImage != NULL)
	{
		delete vContaminant2StructureImage;
		vContaminant2StructureImage = NULL;
	}
	if (ThereIsEnoughMemory(sizeof(CImageData), "vContaminant2StructureImage"))
		vContaminant2StructureImage = new CImageData;
	if (vContaminant2StructureImage)
	{
		//fill buffer with 255 because are finding smallest value
		vContaminant2StructureImage->vImageTypeString = "Product Process 2T";
		vContaminant2StructureImage->vImageType = cDataImageType;
		vContaminant2StructureImage->MakeGreyImage(vOriginalBufferSize);

		if (vContaminant2StructureImage->vGreyImage)
			FillMemory(vContaminant2StructureImage->vGreyImage, vOriginalBufferSize,0xFF);
	}

	if (vVoidStructureImage != NULL)
	{
		delete vVoidStructureImage;
		vVoidStructureImage = NULL;
	}
	if (ThereIsEnoughMemory(sizeof(CImageData), "vVoidStructureImage"))
		vVoidStructureImage = new CImageData;
	if (vVoidStructureImage)
	{
		//fill buffer with 128 because are finding smallest value below 128
		vVoidStructureImage->vImageTypeString = "Void Product Process 2";
		vVoidStructureImage->vImageType = cDataImageType;
		vVoidStructureImage->MakeGreyImage(vOriginalBufferSize);

		if (vVoidStructureImage->vGreyImage)
			FillMemory(vVoidStructureImage->vGreyImage, vOriginalBufferSize,0x80);
	}
	vStructureCount = 0;
}
}

void CITIPCDig::SaveThisRejectInBuffer(CContainer *TempContainer, long TempFrameNumber, BYTE TempEjectedInspectionTopReferenceAdjust, BYTE TempRejectReason)
{
	if (TempRejectReason < cMaximumNumberOfCountsDisplayed)
	if (TempContainer)
	if (TempContainer->vTheImage)
	if (vLastNRejectImagesLock.Lock())
	{
		//delete last reject if reject buffer is totally full
		if (vLastNRejectImages[vNumberOfLastRejectImages - 1][TempRejectReason] != NULL)
		{
			delete vLastNRejectImages[vNumberOfLastRejectImages - 1][TempRejectReason];
			vLastNRejectImages[vNumberOfLastRejectImages - 1][TempRejectReason] = NULL;
		}
		DWORD TempMemoryMargin = 25000000;

		//create memory for new reject and copy data to it
		if (ThereIsEnoughMemory(sizeof(CImageData)	+ (2 * vOriginalBufferSize) + TempMemoryMargin, "Last Reject Buffer"))
		{
			//shift all rejects down one
			for (WORD TempLoop = vNumberOfLastRejectImages - 1; TempLoop > 0; TempLoop--)
				vLastNRejectImages[TempLoop][TempRejectReason] = vLastNRejectImages[TempLoop - 1][TempRejectReason];

			vLastNRejectImages[0][TempRejectReason] = new CImageData;
			if (vLastNRejectImages[0][TempRejectReason])
			{
				//vLastNRejectImages[0][TempRejectReason]->CopyImage(TempContainer->vTheImage);

				if (TempContainer->vTheImage->vGreyImage)
				{
					vLastNRejectImages[0][TempRejectReason]->MakeGreyImage(vOriginalBufferSize);

					if (vLastNRejectImages[0][TempRejectReason]->vGreyImage)
						CopyMemory(vLastNRejectImages[0][TempRejectReason]->vGreyImage, TempContainer->vTheImage->vGreyImage, vOriginalBufferSize);
				}

				if (TempContainer->vTheImage->vRedImage)
				{
					vLastNRejectImages[0][TempRejectReason]->MakeRedImage(vOriginalBufferSize);
					if (vLastNRejectImages[0][TempRejectReason]->vRedImage)
						CopyMemory(vLastNRejectImages[0][TempRejectReason]->vRedImage, TempContainer->vTheImage->vRedImage, vOriginalBufferSize);
				}

				for (BYTE TempLoop = 0;TempLoop < 3; TempLoop++)
					vLastNRejectImages[0][TempRejectReason]->vHeadNumber[TempLoop] = TempContainer->vHeadNumber[TempLoop]; //for filler monitoring and Filler and Seamer Sampling
				vLastNRejectImages[0][TempRejectReason]->vFillLevel = TempContainer->vFillLevel;
				vLastNRejectImages[0][TempRejectReason]->vBodyTriggerGapLength = TempContainer->vLastBodyTriggerGapLength;
				vLastNRejectImages[0][TempRejectReason]->vContainerNumber = TempFrameNumber;
				vLastNRejectImages[0][TempRejectReason]->vContainerEjected = false;
				if ((TempContainer->vToBeEjected) && (!TempContainer->vToBeEjectedButDisabled))
					vLastNRejectImages[0][TempRejectReason]->vContainerEjected = true;
				vLastNRejectImages[0][TempRejectReason]->vPassContainer = TempContainer->vTheImage->vPassContainer;

				for (BYTE TempLoop = 0;TempLoop < cMaximumNumberOfInspections; TempLoop++)
				{
					vLastNRejectImages[0][TempRejectReason]->vRejectedForThisInspection[TempLoop] = TempContainer->vTheImage->vRejectedForThisInspection[TempLoop];
				}

				
				vLastNRejectImages[0][TempRejectReason]->vContainerEjectReason = TempContainer->vTheImage->vContainerEjectReason;
				vLastNRejectImages[0][TempRejectReason]->vContainerEjectThreshold = TempContainer->vTheImage->vContainerEjectThreshold;
				vLastNRejectImages[0][TempRejectReason]->vContainerLearnState = TempContainer->vTheImage->vContainerLearnState;
				vLastNRejectImages[0][TempRejectReason]->vContainerLargestContaminant = TempContainer->vTheImage->vContainerLargestContaminant;
				vLastNRejectImages[0][TempRejectReason]->vContainerEjectInspection = TempContainer->vTheImage->vContainerEjectInspection;
				vLastNRejectImages[0][TempRejectReason]->vRejectDisabled = TempContainer->vToBeEjectedButDisabled;
				vLastNRejectImages[0][TempRejectReason]->vDriftCompensationLocked = vDriftCompensationLocked;
				vLastNRejectImages[0][TempRejectReason]->vMultiLaneLaneNumber = TempContainer->vTheImage->vMultiLaneLaneNumber;
				vLastNRejectImages[0][TempRejectReason]->vMultiLaneEjectorOffset = TempContainer->vTheImage->vMultiLaneEjectorOffset;


				vLastNRejectImages[0][TempRejectReason]->vTop = vLastTop;
				vLastNRejectImages[0][TempRejectReason]->vTopReferenceAdjust = vLastTopReferenceAdjust;
				vLastNRejectImages[0][TempRejectReason]->vBottom = vLastBottom;
				vLastNRejectImages[0][TempRejectReason]->vRight = vLastRight;
				vLastNRejectImages[0][TempRejectReason]->vLeft = vLastLeft;
				vLastNRejectImages[0][TempRejectReason]->vLeftReferenceAdjust = vLastLeftReferenceAdjust;
				vLastNRejectImages[0][TempRejectReason]->vRightReferenceAdjust = vLastRightReferenceAdjust;
				vLastNRejectImages[0][TempRejectReason]->vHeight = vLastHeight;
				vLastNRejectImages[0][TempRejectReason]->vWidth = vLastWidth;
				vLastNRejectImages[0][TempRejectReason]->vFineReferenceAdjust = vLastFineReferenceAdjust;
				for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfInspections; TempLoop++)
				{
					vLastNRejectImages[0][TempRejectReason]->vDensityValue[TempLoop] = vLastDensityValue[TempLoop];
					vLastNRejectImages[0][TempRejectReason]->vNumberOfBlobs[TempLoop] = vLastNumberOfBlobs[TempLoop];
					vLastNRejectImages[0][TempRejectReason]->vInspectonProcessingTime[TempLoop] = vLastInspectonProcessingTime[TempLoop];
					vLastNRejectImages[0][TempRejectReason]->vContainerWeightCorrection[TempLoop] = TempContainer->vTheImage->vContainerWeightCorrection[TempLoop];
					vLastNRejectImages[0][TempRejectReason]->vLargestBlobPerimeter[TempLoop] = vLastLargestBlobPerimeter[TempLoop];
				}
				vLastNRejectImages[0][TempRejectReason]->vHaveRealImage = vHaveRealImage;

				//1/28/2010
				vLastNRejectImages[0][TempRejectReason]->vImageTypeString = vImageTypeString;
				vLastNRejectImages[0][TempRejectReason]->vImageType = vImageType;

				vLastNRejectImages[0][TempRejectReason]->vCurveFitTime = vLastCurveFitTime;
				vLastNRejectImages[0][TempRejectReason]->vGlassInspectionTime = vLastGlassInspectionTime;
				vLastNRejectImages[0][TempRejectReason]->vSmoothTime = vLastSmoothTime;
				vLastNRejectImages[0][TempRejectReason]->vProcessTime = vLastProcessTime;
				vLastNRejectImages[0][TempRejectReason]->vEnhanceTime = vLastEnhanceTime;
				vLastNRejectImages[0][TempRejectReason]->vEdgeAdjustTime = vLastEdgeAdjustTime;
				vLastNRejectImages[0][TempRejectReason]->vFindEdgesTime = vLastFindEdgesTime;
				vLastNRejectImages[0][TempRejectReason]->vReferencePositionTime = vLastReferencePositionTime;
				vLastNRejectImages[0][TempRejectReason]->vCopyBufferTime = vLastCopyBufferTime;
				vLastNRejectImages[0][TempRejectReason]->vCopyToMainTime = vLastCopyToMainTime;
				vLastNRejectImages[0][TempRejectReason]->vFindContaminantsTime = vLastFindContaminantsTime;
				//vLastNRejectImages[0][TempRejectReason]->vFindGlassContaminantsTime = vLastFindGlassContaminantsTime;
				
				vLastNRejectImages[0][TempRejectReason]->vFindMedianTime = vLastFindMedianTime;
				vLastNRejectImages[0][TempRejectReason]->vFindVoidsTime = vLastFindVoidsTime;
				vLastNRejectImages[0][TempRejectReason]->vFindDensityTime = vLastFindDensityTime;
				vLastNRejectImages[0][TempRejectReason]->vFindBlobsTime = vLastFindBlobsTime;
				vLastNRejectImages[0][TempRejectReason]->vFindTime = vLastFindTime;
				vLastNRejectImages[0][TempRejectReason]->vMinimumGoodMaximumBad = vLastMinimumGoodMaximumBad;
				vLastNRejectImages[0][TempRejectReason]->vOriginalImageIntensity = vLastOriginalImageIntensity;
				vLastNRejectImages[0][TempRejectReason]->vImageIntensityAdjust = vLastImageIntensityAdjust;
				//vLastNRejectImages[0][TempRejectReason]->vFinalImageIntensity = vLastFinalImageIntensity;
				vLastNRejectImages[0][TempRejectReason]->vImageIntensityTooFarOff = vLastImageIntensityTooFarOff;
				//vLastNRejectImages[0][TempRejectReason]->vThresholdCorrection = vLastThresholdCorrection;
				//vLastNRejectImages[0][TempRejectReason]->vNegitiveThresholdCorrection = vLastNegitiveThresholdCorrection;

				for (BYTE TempLoop = 0; TempLoop < cMaximumRedBoxes; TempLoop++)
				{
					vLastNRejectImages[0][TempRejectReason]->vRedBoxCornerX[TempLoop] = vLastRedBoxCornerX[TempLoop];
					vLastNRejectImages[0][TempRejectReason]->vRedBoxCornerY[TempLoop] = vLastRedBoxCornerY[TempLoop];
					vLastNRejectImages[0][TempRejectReason]->vRedBoxWidth[TempLoop] = vLastRedBoxWidth[TempLoop];
					vLastNRejectImages[0][TempRejectReason]->vRedBoxHeight[TempLoop] = vLastRedBoxHeight[TempLoop];
					vLastNRejectImages[0][TempRejectReason]->vRedBoxInspection[TempLoop] = vLastRedBoxInspection[TempLoop];
				}
			}
		}
		vLastNRejectImagesLock.Unlock();
	}
}

void CITIPCDig::Save10RedBoxesForDisplay(CInspection *TempInspection, MVT_BLOB_RESULTS *TempBlobResults, int TempSideReferenceAdjust)
{
	if (TempInspection)
	if (TempBlobResults)
	if ((!vShowOnlyRealImages) || (vHaveRealImage) || (vProcessNextImage))
	{
		if ((WORD)TempBlobResults->num_blobs_found)
		{
#ifdef UseIFCDLLs
			MVT_BLOB *TempBiggestBlobs[cMaximumRedBoxes];
			for (DWORD TempLoop = 0; TempLoop < cMaximumRedBoxes; TempLoop++)
				TempBiggestBlobs[TempLoop] = NULL;

			//sort all the blobs by size to find the 10 biggest
			for (DWORD TempLoop = 0; TempLoop < TempBlobResults->num_blobs_found; TempLoop++)
			{
				MVT_BLOB *TempBlob = TempBlobResults->blobs[TempLoop];
				// find the 10 largest blobs
				if (TempBlob)
				if ((TempBlob->area >= TempInspection->vSize) || (TempInspection->vInspectionType == cItemCountInspection) || (TempInspection->vInspectionType == cFloodedInspection) || 
					(TempInspection->vInspectionType == cMissingLidInspectionOnSize) || (TempInspection->vInspectionType == cSizeInspection))
				{
					for (WORD TempBlobLoop = 0; TempBlobLoop < cMaximumRedBoxes; TempBlobLoop++)
					{
						if (!TempBiggestBlobs[TempBlobLoop])
						{
							TempBiggestBlobs[TempBlobLoop] = TempBlob;
							TempBlobLoop = cMaximumRedBoxes;
						}
						else
						if (TempBlob->area > TempBiggestBlobs[TempBlobLoop]->area)
						{
							WORD TempTopSpotTaken = TempBlobLoop;
							for (int TempMoveBlobLoop = TempBlobLoop + 1; TempMoveBlobLoop < cMaximumRedBoxes - 1; TempMoveBlobLoop++)
							if (TempBiggestBlobs[TempMoveBlobLoop])
								TempTopSpotTaken = TempMoveBlobLoop;
							else
								TempMoveBlobLoop = cMaximumRedBoxes;

							for (int TempMoveBlobLoop = TempTopSpotTaken; TempMoveBlobLoop >= TempBlobLoop; TempMoveBlobLoop--)
							if (TempMoveBlobLoop + 1 < cMaximumRedBoxes)
								TempBiggestBlobs[TempMoveBlobLoop + 1] = TempBiggestBlobs[TempMoveBlobLoop];

							TempBiggestBlobs[TempBlobLoop] = TempBlob;
							TempBlobLoop = cMaximumRedBoxes;
						}
					}
				}
			}

			for (BYTE TempLoop = 0; TempLoop < cMaximumRedBoxes; TempLoop++)
			if (TempBiggestBlobs[TempLoop])
			if (TempBiggestBlobs[TempLoop]->height)
			if (TempBiggestBlobs[TempLoop]->width)
			{
				WORD TempWidth = (WORD)(TempBiggestBlobs[TempLoop]->height + 12);
				WORD TempHeight = (WORD)(TempBiggestBlobs[TempLoop]->width + 12);
				if (TempHeight > vOriginalBufferSizeX)
					TempHeight = vOriginalBufferSizeX;
				if (TempWidth > vOriginalBufferSizeY)
					TempWidth = vOriginalBufferSizeY;

				WORD TempHalfHeight = TempHeight / 2;
				WORD TempHalfWidth = TempWidth / 2;
				int TempCornerY = (int)(TempBiggestBlobs[TempLoop]->ycent - TempHalfWidth);
				int TempCornerX = (int)(TempBiggestBlobs[TempLoop]->xcent - TempHalfHeight);

				//if this spot lowered by punt removal, raise back up
				if ((TempInspection->vRemoveJarPunt) && 
					(TempBiggestBlobs[TempLoop]->xcent < TempInspection->vRecentPuntRemovalBottomHeight + TempInspection->vRecentPuntRemovalNumberOfPixelsCopied))
				{
					int TempIndex = (WORD)TempBiggestBlobs[TempLoop]->ycent - TempInspection->vROIPixelLeft ;//- TempSideReferenceAdjust;
					if ((TempIndex > 0) && (TempIndex < 512))
					if (TempInspection->vRecentPuntRemovalPuntHeightProfile[TempIndex] > 0)
					if (TempInspection->vRecentPuntRemovalPuntHeightProfile[TempIndex] < 255)
						TempCornerX = TempCornerX + TempInspection->vRecentPuntRemovalPuntHeightProfile[TempIndex];

					if (TempCornerX > 2)
						TempCornerX = TempCornerX - 2;
					else
						TempCornerX = 0;
				}
				else
				{ //if not needing to raise back up, lower by 6 to center

					if (TempBiggestBlobs[TempLoop]->xmin > 6)
						TempCornerX = TempBiggestBlobs[TempLoop]->xmin - 6;
					else
						TempCornerX = 0;
				}

				if (TempBiggestBlobs[TempLoop]->ymin > 6)
					TempCornerY = TempBiggestBlobs[TempLoop]->ymin - 6;
				else
					TempCornerY = 0;

				if (TempCornerX < 0)
					TempCornerX = 0;
				if (TempCornerX + TempHeight >= vOriginalBufferSizeX)
					TempCornerX = vOriginalBufferSizeX - TempHeight - 1;
				if (TempCornerY + TempWidth >= vOriginalBufferSizeY)
					TempCornerY = vOriginalBufferSizeY - TempWidth - 1;
				if (TempCornerY < 0)  
					TempCornerY = 0;
				if (TempCornerX < 0)  
					TempCornerX = 0;

				if (TempCornerX + TempHeight > vOriginalBufferSizeX)
					TempHeight = vOriginalBufferSizeX - TempCornerX;

				if (TempCornerY + TempWidth > vOriginalBufferSizeY)
					TempWidth = vOriginalBufferSizeY - TempCornerY;
/*
				if (TempSideReferenceAdjust < 0)//if left side will be reference adjusted off screen, set so will be displayed
				{
					if (TempCornerY < -TempSideReferenceAdjust)
						TempCornerY = -TempSideReferenceAdjust;
				}
*/
				if (TempCornerX + TempHeight >= vOriginalBufferSizeX)
				{
					TempHeight = vOriginalBufferSizeX - TempCornerX - 1;
				}

				if (TempHeight < 0)  
					TempHeight = 0;
/*
				if (TempSideReferenceAdjust > 0)
				{
					if (TempCornerY + TempWidth + TempSideReferenceAdjust >= vOriginalBufferSizeY)
					{
						TempWidth = vOriginalBufferSizeY - TempCornerY - TempSideReferenceAdjust;
					}
				}
				else
				*/
				{
					if (TempCornerY + TempWidth >= vOriginalBufferSizeY)
					{
						TempWidth = vOriginalBufferSizeY - TempCornerY - 1;
					}
				}
				if (TempWidth < 0) 
					TempWidth = 0;
				//vLastRedBoxCornerX[TempLoop] = TempCornerX;
				//vLastRedBoxCornerY[TempLoop] = TempCornerY;
				//vLastRedBoxWidth[TempLoop] = TempWidth;
				//vLastRedBoxHeight[TempLoop] = TempHeight;
				//vLastRedBoxInspection[TempLoop] = 0;

				//look for duplicates, if find a duplicate, make it bigger if this one is bigger
				bool TempThisBlobIsADuplicate = false;
				for (BYTE TempBoxLoop = 0; TempBoxLoop < cMaximumRedBoxes; TempBoxLoop++)
				if (vLastRedBoxCornerX[TempBoxLoop])  //if have a box value at this array place
				{
					BYTE TempSameBoxTollerance = 6;
					
					if ((vLastRedBoxCornerX[TempBoxLoop] <= TempCornerX + TempSameBoxTollerance) && (vLastRedBoxCornerX[TempBoxLoop] + TempSameBoxTollerance >= TempCornerX) &&
						(vLastRedBoxCornerY[TempBoxLoop] <= TempCornerY + TempSameBoxTollerance) && (vLastRedBoxCornerY[TempBoxLoop] + TempSameBoxTollerance >= TempCornerY))
					{ //if this corner is within 4 pixels of one already in array
						TempThisBlobIsADuplicate = true;
						//if new box is a little bigger, make original box bigger
						int TempAmountToMakeWider = 0;
						if (vLastRedBoxCornerX[TempBoxLoop] > TempCornerX)
						{
							TempAmountToMakeWider = vLastRedBoxCornerX[TempBoxLoop] - TempCornerX;
							vLastRedBoxCornerX[TempBoxLoop] = TempCornerX;
						}
						int TempAmountToMakeTaller = 0;
						if (vLastRedBoxCornerY[TempBoxLoop] > TempCornerY)
						{
							TempAmountToMakeTaller = vLastRedBoxCornerY[TempBoxLoop] - TempCornerY;
							vLastRedBoxCornerY[TempBoxLoop] = TempCornerY;
						}
						if (vLastRedBoxWidth[TempBoxLoop] < TempWidth + 1)
							vLastRedBoxWidth[TempBoxLoop] = TempWidth + 1;
						if (TempAmountToMakeWider)
							vLastRedBoxWidth[TempBoxLoop] = vLastRedBoxWidth[TempBoxLoop] + TempAmountToMakeWider;

						if (vLastRedBoxHeight[TempBoxLoop] < TempHeight + 1)
							vLastRedBoxHeight[TempBoxLoop] = TempHeight + 1;
						if (TempAmountToMakeTaller)
							vLastRedBoxHeight[TempBoxLoop] = vLastRedBoxHeight[TempBoxLoop] + TempAmountToMakeTaller;

						if (TempInspection)
						{
							BYTE TempInspectionNumber = 0;
							for (BYTE TempLoopI = 0; TempLoopI < vGlobalCurrentProduct->vNumberOfInspections; TempLoopI++)
							if (vGlobalCurrentProduct->vInspection[TempLoopI] == TempInspection)
							{
								TempInspectionNumber = TempLoopI;
								TempLoopI = vGlobalCurrentProduct->vNumberOfInspections;
							}

							vLastRedBoxInspection[TempBoxLoop] = vLastRedBoxInspection[TempBoxLoop] | (1 << TempInspectionNumber);
						}

						TempBoxLoop = cMaximumRedBoxes;
					}
				}

				if (!TempThisBlobIsADuplicate)
				{
					BYTE TempFirstEmptyRedBoxSpot = 0;
					while ((TempFirstEmptyRedBoxSpot < cMaximumRedBoxes) && (vLastRedBoxCornerX[TempFirstEmptyRedBoxSpot] > 0))
						TempFirstEmptyRedBoxSpot++;

					if (TempFirstEmptyRedBoxSpot < cMaximumRedBoxes)
					{
						vLastRedBoxCornerX[TempFirstEmptyRedBoxSpot] = TempCornerX;
						vLastRedBoxCornerY[TempFirstEmptyRedBoxSpot] = TempCornerY;
						vLastRedBoxWidth[TempFirstEmptyRedBoxSpot] = TempWidth + 1;
						vLastRedBoxHeight[TempFirstEmptyRedBoxSpot] = TempHeight + 1;

						if (TempInspection)
						{
							BYTE TempInspectionNumber = 0;
							for (BYTE TempLoopI = 0; TempLoopI < vGlobalCurrentProduct->vNumberOfInspections; TempLoopI++)
							if (vGlobalCurrentProduct->vInspection[TempLoopI] == TempInspection)
							{
								TempInspectionNumber = TempLoopI;
								TempLoopI = vGlobalCurrentProduct->vNumberOfInspections;
							}

							vLastRedBoxInspection[TempFirstEmptyRedBoxSpot] = vLastRedBoxInspection[TempFirstEmptyRedBoxSpot] | (1 << TempInspectionNumber);
						}
					}
				}
			}
#endif
		}
	}
}

void CITIPCDig::SaveRedImageForDisplay(Image *TempImage, CContainer *TempContainer)
{
	if ((!vShowOnlyRealImages) || (vHaveRealImage) || (vProcessNextImage))
	if ((TempImage) && (TempImage->pix))
	if (TempContainer)
	if (TempContainer->vTheImage)
	{
		TempContainer->vTheImage->MakeRedImage(vOriginalBufferSize);
		if (TempContainer->vTheImage->vRedImage)
			CopyMemory(TempContainer->vTheImage->vRedImage, TempImage->pix, vOriginalBufferSize);
	}
}

void CITIPCDig::SaveImageInHolderForDisplay(CContainer *TempContainer)
{
	//copy image data to main display buffer
	double TempStartCopyToMainTime = CIFCOS::GetSystimeMicrosecs();
	if ((!vShowOnlyRealImages) || (vHaveRealImage) || (vProcessNextImage))
	if (TempContainer)
	if (TempContainer->vTheImage)
	if (TempContainer->vTheImage->vGreyImage)
	{
		CopyMemory(TempContainer->vTheImage->vGreyImage, vLatestImage->pix, vOriginalBufferSize);
	}
	double TempEndTime = CIFCOS::GetSystimeMicrosecs();
	vLastCopyToMainTime = vLastCopyToMainTime + (TempEndTime - TempStartCopyToMainTime);
}

void CITIPCDig::FillImageData(CImageData *TempImagePointer, long TempFrameNumber, CContainer *TempContainer)
{
	//copy image data to main display buffer
	double TempStartCopyToMainTime = CIFCOS::GetSystimeMicrosecs();
	if (TempContainer)
	if (TempContainer->vTheImage)
	if (TempImagePointer)
	{
		TempImagePointer->vContainerNumber = TempFrameNumber;
		TempImagePointer->vBodyTriggerGapLength = TempContainer->vLastBodyTriggerGapLength;
		TempImagePointer->vFillLevel = TempContainer->vFillLevel;

		{
			if (TempContainer->vTheImage->vGreyImage)
			{
				TempImagePointer->MakeGreyImage(vOriginalBufferSize);
				if (TempImagePointer->vGreyImage)
					CopyMemory(TempImagePointer->vGreyImage, TempContainer->vTheImage->vGreyImage, vOriginalBufferSize);
			}

			if (((TempContainer->vToBeEjected) || (TempContainer->vTheImage->vContainerEjectReason < 255)) && (TempContainer->vTheImage->vRedImage))
			{
				TempImagePointer->MakeRedImage(vOriginalBufferSize);
				if (TempImagePointer->vRedImage)
					CopyMemory(TempImagePointer->vRedImage, TempContainer->vTheImage->vRedImage, vOriginalBufferSize);
			}
			else
			if (TempImagePointer->vRedImage)
			{ //clear out
				BYTE *TempDestinationPointer = TempImagePointer->vRedImage;
				for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
					*TempDestinationPointer++ = 1;
			}
		}
		CTime TempTime;
		TempTime = CTime::GetCurrentTime();
		//vTime = TempTime.Format("%d-%B-%Y-%H-%M-%S");
		TempImagePointer->vTime = TempTime.Format("%H:%M:%S");

		for (BYTE TempLoop = 0;TempLoop < 3; TempLoop++)
			TempImagePointer->vHeadNumber[TempLoop] = TempContainer->vHeadNumber[TempLoop]; //for filler monitoring and Filler and Seamer Sampling
		TempImagePointer->vFillLevel = TempContainer->vFillLevel;
		TempImagePointer->vContainerEjected = false;
		if ((TempContainer->vToBeEjected) && (!TempContainer->vToBeEjectedButDisabled))
			TempImagePointer->vContainerEjected = true;
		TempImagePointer->vPassContainer = TempContainer->vTheImage->vPassContainer;
		
		TempImagePointer->vContainerEjectReason = TempContainer->vTheImage->vContainerEjectReason;
		TempImagePointer->vContainerLargestContaminant = TempContainer->vTheImage->vContainerLargestContaminant;
		TempImagePointer->vContainerEjectThreshold = TempContainer->vTheImage->vContainerEjectThreshold;
		TempImagePointer->vContainerLearnState = TempContainer->vTheImage->vContainerLearnState;
		TempImagePointer->vContainerEjectInspection = TempContainer->vTheImage->vContainerEjectInspection;
		TempImagePointer->vRejectDisabled = TempContainer->vToBeEjectedButDisabled;
		TempImagePointer->vDriftCompensationLocked = vDriftCompensationLocked;
		TempImagePointer->vMultiLaneLaneNumber = TempContainer->vTheImage->vMultiLaneLaneNumber;
		TempImagePointer->vMultiLaneEjectorOffset = TempContainer->vTheImage->vMultiLaneEjectorOffset;
	

		for(BYTE TempLoop = 0;TempLoop < cMaximumNumberOfInspections; TempLoop++)
			TempImagePointer->vRejectedForThisInspection[TempLoop] = TempContainer->vTheImage->vRejectedForThisInspection[TempLoop];

		TempImagePointer->vFineReferenceAdjust = vLastFineReferenceAdjust;
		TempImagePointer->vTop = vLastTop;
		TempImagePointer->vTopReferenceAdjust = vLastTopReferenceAdjust;
		TempImagePointer->vBottom = vLastBottom;
		TempImagePointer->vRight = vLastRight;
		TempImagePointer->vLeft = vLastLeft;
		TempImagePointer->vLeftReferenceAdjust = vLastLeftReferenceAdjust;
		TempImagePointer->vRightReferenceAdjust = vLastRightReferenceAdjust;
		TempImagePointer->vHeight = vLastHeight;
		TempImagePointer->vWidth = vLastWidth;
		for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfInspections; TempLoop++)
		{
			TempImagePointer->vDensityValue[TempLoop] = vLastDensityValue[TempLoop];
			TempImagePointer->vNumberOfBlobs[TempLoop] = vLastNumberOfBlobs[TempLoop];
			TempImagePointer->vInspectonProcessingTime[TempLoop] = vLastInspectonProcessingTime[TempLoop];
			TempImagePointer->vContainerWeightCorrection[TempLoop] = TempContainer->vTheImage->vContainerWeightCorrection[TempLoop];
			TempImagePointer->vLargestBlobPerimeter[TempLoop] = vLastLargestBlobPerimeter[TempLoop];
		}
		TempImagePointer->vHaveRealImage = vHaveRealImage;
		TempImagePointer->vMinimumGoodMaximumBad = vLastMinimumGoodMaximumBad;

				//1/28/2010
		TempImagePointer->vImageTypeString = vImageTypeString;
		TempImagePointer->vImageType = vImageType;

		TempImagePointer->vCurveFitTime = vLastCurveFitTime;
		TempImagePointer->vGlassInspectionTime = vLastGlassInspectionTime;
		TempImagePointer->vSmoothTime = vLastSmoothTime;
		TempImagePointer->vProcessTime = vLastProcessTime;
		TempImagePointer->vEnhanceTime = vLastEnhanceTime;
		TempImagePointer->vEdgeAdjustTime = vLastEdgeAdjustTime;
		TempImagePointer->vFindEdgesTime = vLastFindEdgesTime;
		TempImagePointer->vReferencePositionTime = vLastReferencePositionTime;
		TempImagePointer->vCopyBufferTime = vLastCopyBufferTime;
		TempImagePointer->vCopyToMainTime = vLastCopyToMainTime;
		TempImagePointer->vFindContaminantsTime = vLastFindContaminantsTime;
		//TempImagePointer->vFindGlassContaminantsTime = vLastFindGlassContaminantsTime;
		TempImagePointer->vFindMedianTime = vLastFindMedianTime;
		TempImagePointer->vFindVoidsTime = vLastFindVoidsTime;
		TempImagePointer->vFindDensityTime = vLastFindDensityTime;
		TempImagePointer->vFindBlobsTime = vLastFindBlobsTime;
		TempImagePointer->vFindTime = vLastFindTime;
		TempImagePointer->vOriginalImageIntensity = vLastOriginalImageIntensity;
		TempImagePointer->vImageIntensityAdjust = vLastImageIntensityAdjust;
		//TempImagePointer->vFinalImageIntensity = vLastFinalImageIntensity;
		TempImagePointer->vImageIntensityTooFarOff = vLastImageIntensityTooFarOff;
		//TempImagePointer->vThresholdCorrection = vLastThresholdCorrection;
		//TempImagePointer->vNegitiveThresholdCorrection = vLastNegitiveThresholdCorrection;
		for (BYTE TempLoop = 0; TempLoop < cMaximumRedBoxes; TempLoop++)
		{
			TempImagePointer->vRedBoxCornerX[TempLoop] = vLastRedBoxCornerX[TempLoop];
			TempImagePointer->vRedBoxCornerY[TempLoop] = vLastRedBoxCornerY[TempLoop];
			TempImagePointer->vRedBoxWidth[TempLoop] = vLastRedBoxWidth[TempLoop];
			TempImagePointer->vRedBoxHeight[TempLoop] = vLastRedBoxHeight[TempLoop];
			TempImagePointer->vRedBoxInspection[TempLoop] = vLastRedBoxInspection[TempLoop];
		}
	}
	double TempEndTime = CIFCOS::GetSystimeMicrosecs();
	vLastCopyToMainTime = TempEndTime - TempStartCopyToMainTime;
}

void CITIPCDig::CopyImageData(CImageData *TempSourceImagePointer, CImageData *TempDestinationImagePointer, DWORD TempSize)
{
	if ((TempSourceImagePointer) && (TempSourceImagePointer->vGreyImage))
	{
		if (TempDestinationImagePointer)
		{
			TempDestinationImagePointer->MakeGreyImage(TempSize);
			TempDestinationImagePointer->MakeRedImage(TempSize);

			if (TempDestinationImagePointer->vGreyImage)
				CopyMemory(TempDestinationImagePointer->vGreyImage, TempSourceImagePointer->vGreyImage, TempSize);

			if (TempDestinationImagePointer->vRedImage)
			if (TempSourceImagePointer->vRedImage)
				CopyMemory(TempDestinationImagePointer->vRedImage, TempSourceImagePointer->vRedImage, TempSize);

				for (BYTE TempLoop = 0;TempLoop < 3; TempLoop++)
					TempDestinationImagePointer->vHeadNumber[TempLoop] = TempSourceImagePointer->vHeadNumber[TempLoop]; //for filler monitoring and Filler and Seamer Sampling
			TempDestinationImagePointer->vFillLevel = TempSourceImagePointer->vFillLevel;
			TempDestinationImagePointer->vTime = TempSourceImagePointer->vTime;
			TempDestinationImagePointer->vContainerEjected = TempSourceImagePointer->vContainerEjected;
			TempDestinationImagePointer->vPassContainer = TempSourceImagePointer->vPassContainer;
			TempDestinationImagePointer->vContainerEjectReason = TempSourceImagePointer->vContainerEjectReason;
			TempDestinationImagePointer->vContainerLargestContaminant = TempSourceImagePointer->vContainerLargestContaminant;
			TempDestinationImagePointer->vContainerEjectThreshold = TempSourceImagePointer->vContainerEjectThreshold;
			TempDestinationImagePointer->vContainerLearnState = TempSourceImagePointer->vContainerLearnState;
			TempDestinationImagePointer->vContainerEjectInspection = TempSourceImagePointer->vContainerEjectInspection;
			TempDestinationImagePointer->vMultiLaneLaneNumber = TempSourceImagePointer->vMultiLaneLaneNumber;
			TempDestinationImagePointer->vMultiLaneEjectorOffset = TempSourceImagePointer->vMultiLaneEjectorOffset;
			
			TempDestinationImagePointer->vRejectDisabled = TempSourceImagePointer->vRejectDisabled;
			TempDestinationImagePointer->vDriftCompensationLocked = TempSourceImagePointer->vDriftCompensationLocked;

			for(BYTE TempLoop = 0;TempLoop < cMaximumNumberOfInspections; TempLoop++)
				TempDestinationImagePointer->vRejectedForThisInspection[TempLoop] = TempSourceImagePointer->vRejectedForThisInspection[TempLoop];

			TempDestinationImagePointer->vTop = TempSourceImagePointer->vTop;
			TempDestinationImagePointer->vTopReferenceAdjust = TempSourceImagePointer->vTopReferenceAdjust;
			TempDestinationImagePointer->vBottom = TempSourceImagePointer->vBottom;
			TempDestinationImagePointer->vRight = TempSourceImagePointer->vRight;
			TempDestinationImagePointer->vLeft = TempSourceImagePointer->vLeft;
			TempDestinationImagePointer->vLeftReferenceAdjust = TempSourceImagePointer->vLeftReferenceAdjust;
			TempDestinationImagePointer->vRightReferenceAdjust = TempSourceImagePointer->vRightReferenceAdjust;
			TempDestinationImagePointer->vHeight = TempSourceImagePointer->vHeight;
			TempDestinationImagePointer->vWidth = TempSourceImagePointer->vWidth;
			TempDestinationImagePointer->vFineReferenceAdjust = TempSourceImagePointer->vFineReferenceAdjust;
			for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfInspections; TempLoop++)
			{
				TempDestinationImagePointer->vDensityValue[TempLoop] = TempSourceImagePointer->vDensityValue[TempLoop];
				TempDestinationImagePointer->vNumberOfBlobs[TempLoop] = TempSourceImagePointer->vNumberOfBlobs[TempLoop];
				TempDestinationImagePointer->vInspectonProcessingTime[TempLoop] = TempSourceImagePointer->vInspectonProcessingTime[TempLoop];
				TempDestinationImagePointer->vContainerWeightCorrection[TempLoop] = TempSourceImagePointer->vContainerWeightCorrection[TempLoop];
				TempDestinationImagePointer->vLargestBlobPerimeter[TempLoop] = TempSourceImagePointer->vLargestBlobPerimeter[TempLoop];
			}
			TempDestinationImagePointer->vHaveRealImage = TempSourceImagePointer->vHaveRealImage;
			TempDestinationImagePointer->vImageTypeString = TempSourceImagePointer->vImageTypeString;
			TempDestinationImagePointer->vBodyTriggerGapLength = TempSourceImagePointer->vBodyTriggerGapLength;
			TempDestinationImagePointer->vContainerNumber = TempSourceImagePointer->vContainerNumber;
			TempDestinationImagePointer->vImageType = TempSourceImagePointer->vImageType;
			
			TempDestinationImagePointer->vCurveFitTime = TempSourceImagePointer->vCurveFitTime;
			TempDestinationImagePointer->vGlassInspectionTime = TempSourceImagePointer->vGlassInspectionTime;
			TempDestinationImagePointer->vSmoothTime = TempSourceImagePointer->vSmoothTime;

			TempDestinationImagePointer->vProcessTime = TempSourceImagePointer->vProcessTime;
			TempDestinationImagePointer->vEnhanceTime = TempSourceImagePointer->vEnhanceTime;
			TempDestinationImagePointer->vEdgeAdjustTime = TempSourceImagePointer->vEdgeAdjustTime;
			TempDestinationImagePointer->vFindEdgesTime = TempSourceImagePointer->vFindEdgesTime;
			TempDestinationImagePointer->vReferencePositionTime = TempSourceImagePointer->vReferencePositionTime;
			TempDestinationImagePointer->vCopyBufferTime = TempSourceImagePointer->vCopyBufferTime;
			TempDestinationImagePointer->vCopyToMainTime = TempSourceImagePointer->vCopyToMainTime;
			TempDestinationImagePointer->vFindContaminantsTime = TempSourceImagePointer->vFindContaminantsTime;
			//TempDestinationImagePointer->vFindGlassContaminantsTime = TempSourceImagePointer->vFindGlassContaminantsTime;
			TempDestinationImagePointer->vFindMedianTime = TempSourceImagePointer->vFindMedianTime;
			TempDestinationImagePointer->vFindVoidsTime = TempSourceImagePointer->vFindVoidsTime;
			TempDestinationImagePointer->vFindDensityTime = TempSourceImagePointer->vFindDensityTime;
			TempDestinationImagePointer->vFindBlobsTime = TempSourceImagePointer->vFindBlobsTime;
			TempDestinationImagePointer->vFindTime = TempSourceImagePointer->vFindTime;
			TempDestinationImagePointer->vMinimumGoodMaximumBad = TempSourceImagePointer->vMinimumGoodMaximumBad;
			TempDestinationImagePointer->vOriginalImageIntensity = TempSourceImagePointer->vOriginalImageIntensity;
			TempDestinationImagePointer->vImageIntensityAdjust = TempSourceImagePointer->vImageIntensityAdjust;
			TempDestinationImagePointer->vFinalImageIntensity = TempSourceImagePointer->vFinalImageIntensity;
			TempDestinationImagePointer->vImageIntensityTooFarOff = TempSourceImagePointer->vImageIntensityTooFarOff;
			
			//TempDestinationImagePointer->vThresholdCorrection = TempSourceImagePointer->vThresholdCorrection;
			//TempDestinationImagePointer->vNegitiveThresholdCorrection = TempSourceImagePointer->vNegitiveThresholdCorrection;
			for (BYTE TempLoop = 0; TempLoop < cMaximumRedBoxes; TempLoop++)
			{
				TempDestinationImagePointer->vRedBoxCornerX[TempLoop] = TempSourceImagePointer->vRedBoxCornerX[TempLoop];
				TempDestinationImagePointer->vRedBoxCornerY[TempLoop] = TempSourceImagePointer->vRedBoxCornerY[TempLoop];
				TempDestinationImagePointer->vRedBoxWidth[TempLoop] = TempSourceImagePointer->vRedBoxWidth[TempLoop];
				TempDestinationImagePointer->vRedBoxHeight[TempLoop] = TempSourceImagePointer->vRedBoxHeight[TempLoop];
				TempDestinationImagePointer->vRedBoxInspection[TempLoop] = TempSourceImagePointer->vRedBoxInspection[TempLoop];
			}
		}
	}
}

void CITIPCDig::InitializeLastRejectsBuffer(bool TempJustCreatedMemory)
{
	if (vLastNRejectImagesLock.Lock())
	{
		vNextRejectBufferToFill = 0;
		vRejectBufferShowing = -1;
		for (WORD TempOuterLoop = 0; TempOuterLoop < 
			cMaximumNumberOfCountsDisplayed; TempOuterLoop++)
		{
			for (WORD TempLoop = 0; TempLoop < cMaxNumberOfLastRejectImages + 1; TempLoop++)
			{
				if (vLastNRejectImages[TempLoop][TempOuterLoop])
				{
					if (!TempJustCreatedMemory)
						delete vLastNRejectImages[TempLoop][TempOuterLoop];
				}
				vLastNRejectImages[TempLoop][TempOuterLoop] = NULL;
			}
		}
		vLastNRejectImagesLock.Unlock();
	}
}

void CITIPCDig::MaskImageWithDialatedImage(Image *TempImage, Image *TempDialatedImage)
{
	if ((TempImage) && (TempDialatedImage) && (TempDialatedImage->pix) && (TempImage->pix))
	{
		BYTE *TempDilatePointer = TempDialatedImage->pix;
		BYTE *TempDisplayPointer = TempImage->pix;
		for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
		{
			if (*TempDilatePointer++ == 255)
				*TempDisplayPointer = 255;
			TempDisplayPointer++;
		}
	}
}

void CITIPCDig::CombineImages(BYTE *TempMainImage, double TempMainImageFactor,
	BYTE *TempSecondaryImage, double TempSecondaryImageFactor)
{
	if ((TempMainImage) && (TempSecondaryImageFactor))
	{
		double TempDouble = 0;
		double TempFactorSum = TempMainImageFactor + TempSecondaryImageFactor;
		BYTE *TempMainImagePointer = TempMainImage;
		BYTE *TempSecondaryImagePointer = TempSecondaryImage;
		if (TempFactorSum)
		for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
		{
			TempDouble = ((*TempMainImagePointer * TempMainImageFactor) +
				(*TempSecondaryImagePointer++ * TempSecondaryImageFactor)) / TempFactorSum;
			if (TempDouble >= 255)
				*TempMainImagePointer = 255;
			*TempMainImagePointer++ = (BYTE)TempDouble;
		}
	}
}

void CITIPCDig::ThresholdBuffer(BYTE *TempBufferPointer, DWORD TempSize, BYTE TempThreshold)
{
	if (TempBufferPointer)
	{
		BYTE *TempPointer = TempBufferPointer;
		for (DWORD TempLoop = 0; TempLoop < TempSize; TempLoop++)
		{
			if (*TempPointer < TempThreshold)
				*TempPointer = 0;
			else
				*TempPointer = 255;
			TempPointer++;
		}
	}
}

void CITIPCDig::ThresholdBufferDown(BYTE *TempBufferPointer, DWORD TempSize, BYTE TempThreshold)
{
	if (TempBufferPointer)
	{
		BYTE *TempPointer = TempBufferPointer;
		for (DWORD TempLoop = 0; TempLoop < TempSize; TempLoop++)
		{
			if (*TempPointer < TempThreshold)
				*TempPointer = 0;
			TempPointer++;
		}
	}
}

//delete pointers and data in memory
void CITIPCDig::DeleteLearnImages()
{
	//if they are the same, clear one of the pointers
	if (vProductHistogramImage != NULL)
	{
		delete vProductHistogramImage;
		vProductHistogramImage = NULL;
	}
	/*
	if (vProductGlassHistogramImage != NULL)
	{
		delete vProductGlassHistogramImage;
		vProductGlassHistogramImage = NULL;
	}
	if (vProductGlassStructureImage != NULL)
	{
		delete vProductGlassStructureImage;
		vProductGlassStructureImage = NULL;
	}
	*/
	if (vStructureImage != NULL)
	{
		delete vStructureImage;
		vStructureImage = NULL;
	}

	if (vContaminant2StructureImage != NULL)
	{
		delete vContaminant2StructureImage;
		vContaminant2StructureImage = NULL;
	}
	
	if (vProductContaminant2StructureImage != NULL)
	{
		delete vProductContaminant2StructureImage;
		vProductContaminant2StructureImage = NULL;
	}

	if (vProductContaminant2StructureImageWithNoDilation != NULL)
	{
		delete vProductContaminant2StructureImageWithNoDilation;
		vProductContaminant2StructureImageWithNoDilation = NULL;
	}
	
	if (vProductStructureImage != NULL)
	{
		delete vProductStructureImage;
		vProductStructureImage = NULL;
	}

	if (vVoidStructureImage != NULL)
	{
		delete vVoidStructureImage;
		vVoidStructureImage = NULL;
	}
	if (vVoidProductStructureImage != NULL)
	{
		delete vVoidProductStructureImage;
		vVoidProductStructureImage = NULL;
	}
	//if (vVoidProductFactoredStructureImage != NULL)
	//{
	//	delete vVoidProductFactoredStructureImage;
	//	vVoidProductFactoredStructureImage = NULL;
	//}

	if (vAverageImage != NULL)
	{
		free(vAverageImage);
		vAverageImage = NULL;
	}
	if (vProductAverageImage != NULL)
	{
		delete vProductAverageImage;
		vProductAverageImage = NULL;
	}
	if (vProductBackgroundImage != NULL)
	{
		im_delete(vProductBackgroundImage);
		vProductBackgroundImage = NULL;
	}
}

void CITIPCDig::ChangedSizeOfProduct(CProduct *TempProduct)
{
	DeleteLearnImages();
	DeleteLearnFiles(TempProduct);
}

void CITIPCDig::DeleteABackupLearnFile(CString TempLearnName)
{
	// delete any image files			
	CString TempString1 =  "TempBackup" + TempLearnName;
	if (!vUsingLocalSetupDataFile)
		TempString1 = cSetupDataDirectory + TempString1;
	else
		TempString1 = vGlobalCurrentDirectory + TempString1;
	TempString1 = TempString1 + ".BMP";
	LPTSTR TempString = TempString1.GetBuffer(TempString1.GetLength());
	//CFile TempFile;
	CFileStatus TempFileStatus;
	if (CFile::GetStatus(TempString, TempFileStatus))
	if (!DeleteFile(TempString))
		ReportErrorMessage("Failed to Delete File: " + TempString1, cEMailInspx, 32000);

	TempString1 =  "TempBackup" + TempLearnName;
	if (!vUsingLocalSetupDataFile)
		TempString1 = cSetupDataDirectory + TempString1;
	else
		TempString1 = vGlobalCurrentDirectory + TempString1;
	TempString1 = TempString1 + "B.BMP";
	TempString = TempString1.GetBuffer(TempString1.GetLength());
	if (CFile::GetStatus(TempString, TempFileStatus))
	if (!DeleteFile(TempString))
		ReportErrorMessage("Failed to Delete File: " + TempString1, cEMailInspx, 32000);
}

void CITIPCDig::DeleteBackupLearnFiles()
{
	DeleteABackupLearnFile("=Process 0"); //background learn
	DeleteABackupLearnFile("=Process 1"); //average learn

	DeleteABackupLearnFile("=Process 2"); //Structure learn
	DeleteABackupLearnFile("=Process 2o"); //original Structure learn
	DeleteABackupLearnFile("=Process 2u"); //undo Structure learn

	DeleteABackupLearnFile("=Process 2T"); //Contaminant2 Structure learn
	DeleteABackupLearnFile("=Process 2To"); //original Contaminant2 Structure learn
	DeleteABackupLearnFile("=Process 2Tu"); //undo Contaminant2 Structure learn

	DeleteABackupLearnFile("=Process 2g"); //Glass Structure learn
	DeleteABackupLearnFile("=Process 2go"); //Original Glass Structure learn
	DeleteABackupLearnFile("=Process 2gu"); //Undo Glass Structure learn

	if (vGlobalAllowVoidInspection)
	{
		DeleteABackupLearnFile("=Process 2v"); //Void Structure learn
		DeleteABackupLearnFile("=Process 2vo"); //Original Void Structure learn
		DeleteABackupLearnFile("=Process 2vu"); //Undo Void Structure learn
	}

	DeleteABackupLearnFile("=Process 3"); //Histogram learn
}

void CITIPCDig::DeleteALearnFile(CString TempLearnName, CString TempProductName)
{
	CString TempString1 =  TempProductName + TempLearnName;
	if (!vUsingLocalSetupDataFile)
		TempString1 = cSetupDataDirectory + TempString1;
	else
		TempString1 = vGlobalCurrentDirectory + TempString1;
	TempString1 = TempString1 + ".BMP";

	//delete file
	LPTSTR TempString = TempString1.GetBuffer(TempString1.GetLength());
	CFileStatus TempFileStatus;
	if (CFile::GetStatus(TempString, TempFileStatus))
	if (!DeleteFile(TempString))
		ReportErrorMessage("Failed to Delete File: " + TempString1, cEMailInspx, 32000);
		
	//delete the file's backup
 	TempString1.Insert(TempString1.GetLength() - 4, 'B');
	TempString = TempString1.GetBuffer(TempString1.GetLength());
	if (CFile::GetStatus(TempString, TempFileStatus))
	if (!DeleteFile(TempString))
		ReportErrorMessage("Failed to Delete File: " + TempString1, cEMailInspx, 32000);

	vALearnedImageHasChanged = true;
}

void CITIPCDig::DeleteLearnFiles(CProduct *TempProduct)
{
	if (TempProduct)
	{
		CString TempProductName = *TempProduct->GetProductName();
		DeleteALearnFile("=Process 0", TempProductName); //background learn
		DeleteALearnFile("=Process 1", TempProductName); //average learn

		DeleteALearnFile("=Process 2", TempProductName); //Structure learn
		DeleteALearnFile("=Process 2o", TempProductName); //original Structure learn
		DeleteALearnFile("=Process 2u", TempProductName); //undo Structure learn

		DeleteALearnFile("=Process 2T", TempProductName); //Contaminant2 Structure learn
		DeleteALearnFile("=Process 2To", TempProductName); //original Contaminant2 Structure learn
		DeleteALearnFile("=Process 2Tu", TempProductName); //undo Contaminant2 Structure learn

		DeleteALearnFile("=Process 2g", TempProductName); //Glass Structure learn
		DeleteALearnFile("=Process 2go", TempProductName); //Original Glass Structure learn
		DeleteALearnFile("=Process 2gu", TempProductName); //Undo Glass Structure learn

		if (vGlobalAllowVoidInspection)
		{
			DeleteALearnFile("=Process 2v", TempProductName); //Void Structure learn
			DeleteALearnFile("=Process 2vo", TempProductName); //Original Void Structure learn
			DeleteALearnFile("=Process 2vu", TempProductName); //Undo Void Structure learn
		}

		DeleteALearnFile("=Process 3", TempProductName); //Histogram learn

		//DeleteUndoStructureLearnFiles(TempProductName);
		DeleteCSVFiles(TempProductName);

		//delete checkweigh calibrations since deleting learn

		for (BYTE TempLoop = 0; TempLoop < TempProduct->vNumberOfInspections; TempLoop++)
		if (TempProduct->vInspection[TempLoop])
		if ((TempProduct->vInspection[TempLoop]->vInspectionType == cCheckWeighInspection) || (TempProduct->vInspection[TempLoop]->vInspectionType == cUnderfillByWeightInspection) || 
			(TempProduct->vInspection[TempLoop]->vInspectionType == cOverfillByWeightInspection))
		{
			TempProduct->vInspection[TempLoop]->vSetupHistoryMaximumBadCount = 0;
			TempProduct->vInspection[TempLoop]->vSetupHistoryMinimumGoodCount = 0;
			TempProduct->vInspection[TempLoop]->vCheckWeighBFactor = 0;
			TempProduct->vInspection[TempLoop]->vCheckWeighMFactor = 1;
			TempProduct->vInspection[TempLoop]->vCheckWeighOffset = 0;
		}
	}
}

void CITIPCDig::DeleteLearnFiles(CString TempProductName)
{
	DeleteALearnFile("=Process 0", TempProductName); //background learn
	DeleteALearnFile("=Process 1", TempProductName); //average learn

	DeleteALearnFile("=Process 2", TempProductName); //Structure learn
	DeleteALearnFile("=Process 2o", TempProductName); //original Structure learn
	DeleteALearnFile("=Process 2u", TempProductName); //undo Structure learn

	DeleteALearnFile("=Process 2T", TempProductName); //Contaminant2 Structure learn
	DeleteALearnFile("=Process 2To", TempProductName); //original Contaminant2 Structure learn
	DeleteALearnFile("=Process 2Tu", TempProductName); //undo Contaminant2 Structure learn

	DeleteALearnFile("=Process 2g", TempProductName); //Glass Structure learn
	DeleteALearnFile("=Process 2go", TempProductName); //Original Glass Structure learn
	DeleteALearnFile("=Process 2gu", TempProductName); //Undo Glass Structure learn

	if (vGlobalAllowVoidInspection)
	{
		DeleteALearnFile("=Process 2v", TempProductName); //Void Structure learn
		DeleteALearnFile("=Process 2vo", TempProductName); //Original Void Structure learn
		DeleteALearnFile("=Process 2vu", TempProductName); //Undo Void Structure learn
	}

	DeleteALearnFile("=Process 3", TempProductName); //Histogram learn

	//DeleteUndoStructureLearnFiles(TempProductName);
	DeleteCSVFiles(TempProductName);
}

void CITIPCDig::DeleteCSVFiles(CString TempProductName)
{
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	CString TempDirectory = cSetupDataDirectory;
	if (vUsingLocalSetupDataFile)
		TempDirectory = vGlobalCurrentDirectory;
	CString TempString1 = "*.csv";
	TempString1 =  TempProductName + TempString1;
	TempString1 =  TempDirectory + TempString1;
	LPTSTR TempDirectoryName = TempString1.GetBuffer(TempDirectory.GetLength());
	hFind = FindFirstFile(TempDirectoryName, &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		CString TempFileNameString = TempDirectory + FindFileData.cFileName;
		if (TempFileNameString.GetAt(TempFileNameString.GetLength() - 1) != '.')
		if (TempFileNameString.GetAt(TempFileNameString.GetLength() - 1) != '\\')
		if (!DeleteFile(TempFileNameString))
			ReportErrorMessage("Failed to Delete File: " + TempFileNameString, cEMailInspx, 32000);
		while (FindNextFile(hFind,&FindFileData))
		{
			TempFileNameString = TempDirectory + FindFileData.cFileName;
			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			if (!DeleteFile(TempFileNameString))
				ReportErrorMessage("Failed to Delete File: " + TempFileNameString, cEMailInspx, 32000);
		}
	}
	FindClose(hFind);
}

void CITIPCDig::ChangeNameOfALearnFile(CString OriginalName, CString NewName, CString TempLearnName)
{
	CString	TempString1 =  OriginalName + TempLearnName;
	if (!vUsingLocalSetupDataFile)
		TempString1 = cSetupDataDirectory + TempString1;
	else
		TempString1 = vGlobalCurrentDirectory + TempString1;
	TempString1 = TempString1 + ".BMP";
	LPTSTR TempString = TempString1.GetBuffer(TempString1.GetLength());

	CString TempString2 =  NewName + TempLearnName;
	if (!vUsingLocalSetupDataFile)
		TempString2 = cSetupDataDirectory + TempString2;
	else
		TempString2 = vGlobalCurrentDirectory + TempString2;
	TempString2 = TempString2 + ".BMP";
	LPTSTR TempNewFileNameString = TempString2.GetBuffer(TempString2.GetLength());

	CFile TempFile;
	CFileStatus TempFileStatus;

	//rename file
	if (CFile::GetStatus(TempNewFileNameString, TempFileStatus))
	if (!DeleteFile(TempNewFileNameString))
		ReportErrorMessage("Failed to Delete File: " + TempString2, cEMailInspx, 32000);
	if (CFile::GetStatus(TempString, TempFileStatus))
	if (!CFile::GetStatus(TempNewFileNameString, TempFileStatus))
			TempFile.Rename(TempString,TempNewFileNameString);

	//rename backup file
	TempString1.Insert(TempString1.GetLength() - 4,'B');
	TempString = TempString1.GetBuffer(TempString1.GetLength());
	TempString2.Insert(TempString2.GetLength() - 4,'B');
	TempNewFileNameString = TempString2.GetBuffer(TempString2.GetLength());

	if (CFile::GetStatus(TempNewFileNameString, TempFileStatus))
	if (!DeleteFile(TempNewFileNameString))
		ReportErrorMessage("Failed to Delete File: " + TempString2, cEMailInspx, 32000);
	if (CFile::GetStatus(TempString, TempFileStatus))
	if (!CFile::GetStatus(TempNewFileNameString, TempFileStatus))
			TempFile.Rename(TempString,TempNewFileNameString);

	vALearnedImageHasChanged = true;
}

void CITIPCDig::CopyALearnFile(CString OriginalName, CString NewName, CString TempLearnName)
{
	CString	TempString1 =  OriginalName + TempLearnName;
	if (!vUsingLocalSetupDataFile)
		TempString1 = cSetupDataDirectory + TempString1;
	else
		TempString1 = vGlobalCurrentDirectory + TempString1;
	TempString1 = TempString1 + ".BMP";
	LPTSTR TempString = TempString1.GetBuffer(TempString1.GetLength());

	CString TempString2 =  NewName + TempLearnName;
	if (!vUsingLocalSetupDataFile)
		TempString2 = cSetupDataDirectory + TempString2;
	else
		TempString2 = vGlobalCurrentDirectory + TempString2;
	TempString2 = TempString2 + ".BMP";
	LPTSTR TempNewFileNameString = TempString2.GetBuffer(TempString2.GetLength());

	CFile TempFile;
	CFileStatus TempFileStatus;
	//Copy file
	if (CFile::GetStatus(TempNewFileNameString, TempFileStatus))
	if (!DeleteFile(TempNewFileNameString))
		ReportErrorMessage("Failed to Delete File Before A Copy To It: " + TempString2, cEMailInspx, 32000);
	if (CFile::GetStatus(TempString, TempFileStatus))
	if (!CFile::GetStatus(TempNewFileNameString, TempFileStatus))
	if (!CopyFile(TempString,TempNewFileNameString, false))
		ReportErrorMessage("Failed to Copy File: " + TempString1 + " to " + TempNewFileNameString, cEMailInspx, 32000);

	//rename backup file
	TempString1.Insert(TempString1.GetLength() - 4,'B');
	TempString = TempString1.GetBuffer(TempString1.GetLength());
	TempString2.Insert(TempString2.GetLength() - 4,'B');
	TempNewFileNameString = TempString2.GetBuffer(TempString2.GetLength());

	if (CFile::GetStatus(TempNewFileNameString, TempFileStatus))
	if (!DeleteFile(TempNewFileNameString))
		ReportErrorMessage("Failed to Delete File: " + TempString2, cEMailInspx, 32000);
	if (CFile::GetStatus(TempString, TempFileStatus))
	if (!CFile::GetStatus(TempNewFileNameString, TempFileStatus))
	if (!CopyFile(TempString,TempNewFileNameString, false))
		ReportErrorMessage("Failed to Copy File: " + TempString1 + " to " + TempNewFileNameString, cEMailInspx, 32000);

	vALearnedImageHasChanged = true;
}

void CITIPCDig::CopyLearnFiles(CString TempFromName, CString TempToName)
{
	if (TempFromName != "None")
	{
		CopyALearnFile(TempFromName, TempToName, "=Process 0"); //background learn
		CopyALearnFile(TempFromName, TempToName, "=Process 1"); //average learn

		CopyALearnFile(TempFromName, TempToName, "=Process 2"); //Structure learn
		CopyALearnFile(TempFromName, TempToName, "=Process 2o"); //original Structure learn
		CopyALearnFile(TempFromName, TempToName, "=Process 2u"); //undo Structure learn

		CopyALearnFile(TempFromName, TempToName, "=Process 2T"); //Contaminant2 Structure learn
		CopyALearnFile(TempFromName, TempToName, "=Process 2To"); //original Contaminant2 Structure learn
		CopyALearnFile(TempFromName, TempToName, "=Process 2Tu"); //undo Contaminant2 Structure learn

		if (vGlobalAllowVoidInspection)
		{
			CopyALearnFile(TempFromName, TempToName, "=Process 2v"); //Void Structure learn
			CopyALearnFile(TempFromName, TempToName, "=Process 2vo"); //Original Void Structure learn
			CopyALearnFile(TempFromName, TempToName, "=Process 2vu"); //UndoVoid Structure learn
		}

		CopyALearnFile(TempFromName, TempToName, "=Process 3"); //Histogram learn
	}
}

void CITIPCDig::ChangeNameOfLearnFiles(CString OriginalName, CString NewName)
{
	//copy all files, then delete in case power goes off in middle, then don't have corrupt images in middle of renaming
	CopyLearnFiles(OriginalName, NewName);

	//all files have now been copied, so next delete the old ones
	DeleteLearnFiles(OriginalName);
}

void CITIPCDig::RenameLearnFilesToBackup(CProduct *TempProduct)
{
	if (TempProduct)
	{
		CString TempProductName = *TempProduct->GetProductName();
		CopyLearnFiles(TempProductName, "TempBackup");

		//all files have now been copied, so next delete the old ones
		DeleteLearnFiles(TempProductName);
	}
	//RenameAverageFileToBackup(TempProduct);
	//RenameStructureFilesToBackup(TempProduct);
	//RenameBackgroundFileToBackup(TempProduct);
}

void CITIPCDig::RenameALearnFileToBackup(CString TempLearnName, CProduct *TempProduct)
{
	// rename any image files to the backup image files
	CString *TempProductName = TempProduct->GetProductName();
	CString TempString1 =  *TempProductName + TempLearnName;
	if (!vUsingLocalSetupDataFile)
		TempString1 = cSetupDataDirectory + TempString1;
	else
		TempString1 = vGlobalCurrentDirectory + TempString1;
	TempString1 = TempString1 + ".BMP";
	LPTSTR TempString = TempString1.GetBuffer(TempString1.GetLength());

	CString TempString2 =  "TempBackup" + TempLearnName;
	if (!vUsingLocalSetupDataFile)
		TempString2 = cSetupDataDirectory + TempString2;
	else
		TempString2 = vGlobalCurrentDirectory + TempString2;
	TempString2 = TempString2 + ".BMP";
	LPTSTR TempNewFileNameString = TempString2.GetBuffer(TempString2.GetLength());

	CFile TempFile;
	CFileStatus TempFileStatus;
	if (CFile::GetStatus(TempString, TempFileStatus))
	{//if you have an original file
		if (!CFile::GetStatus(TempNewFileNameString, TempFileStatus))
			TempFile.Rename(TempString,TempNewFileNameString);
	}
	
	// rename any image file's backup "B" to the "TempBackup" image files
	TempProductName = TempProduct->GetProductName();
	TempString1 =  *TempProductName + TempLearnName;
	if (!vUsingLocalSetupDataFile)
		TempString1 = cSetupDataDirectory + TempString1;
	else
		TempString1 = vGlobalCurrentDirectory + TempString1;
	TempString1 = TempString1 + "B.BMP";
	TempString = TempString1.GetBuffer(TempString1.GetLength());

	TempString2 =  "TempBackup" + TempLearnName;
	if (!vUsingLocalSetupDataFile)
		TempString2 = cSetupDataDirectory + TempString2;
	else
		TempString2 = vGlobalCurrentDirectory + TempString2;
	TempString2 = TempString2 + "B.BMP";
	TempNewFileNameString = TempString2.GetBuffer(TempString2.GetLength());

	TempFile;
	TempFileStatus;
	if (CFile::GetStatus(TempString, TempFileStatus))
	{//if you have an original file
		if (!CFile::GetStatus(TempNewFileNameString, TempFileStatus))
			TempFile.Rename(TempString,TempNewFileNameString);
	}
}

void CITIPCDig::RenameAverageFileToBackup(CProduct *TempProduct)
{
	RenameALearnFileToBackup("=Process 1", TempProduct); //average learn
	RenameALearnFileToBackup("=Process 3", TempProduct); //Histogram learn
}

void CITIPCDig::RenameStructureFilesToBackup(CProduct *TempProduct)
{
	RenameALearnFileToBackup("=Process 2", TempProduct); //Structure learn
	RenameALearnFileToBackup("=Process 2o", TempProduct); //original Structure learn
	RenameALearnFileToBackup("=Process 2u", TempProduct); //undo Structure learn

	RenameALearnFileToBackup("=Process 2T", TempProduct); //Contaminant2 Structure learn
	RenameALearnFileToBackup("=Process 2To", TempProduct); //original Contaminant2 Structure learn
	RenameALearnFileToBackup("=Process 2Tu", TempProduct); //undo Contaminant2 Structure learn

	//RenameALearnFileToBackup("=Process 2g", TempProduct); //Glass Structure learn
	//RenameALearnFileToBackup("=Process 2go", TempProduct); //Original Glass Structure learn
	//RenameALearnFileToBackup("=Process 2gu", TempProduct); //Undo Glass Structure learn

	if (vGlobalAllowVoidInspection)
	{
		RenameALearnFileToBackup("=Process 2v", TempProduct); //Void Structure learn
		RenameALearnFileToBackup("=Process 2vo", TempProduct); //Original Void Structure learn
		RenameALearnFileToBackup("=Process 2vu", TempProduct); //Undo Void Structure learn
	}
}

void CITIPCDig::LoadProductDataImages(CProduct *TempProduct)
{
	if (TempProduct)
	{
		CString TempProcessType = "=Process 1";
		vProductAverageImage = LoadLearnImageFromFile(TempProcessType, vGlobalCurrentProduct, vProductAverageImage, "Product Process 1");
		if (vGlobalDisplayDialogThread)
			vGlobalDisplayDialogThread->NextStep();//step8

		if (vContainerCollectionLock.Lock())
		{
			LoadStructureImagesFromFiles(false, false);
			vContainerCollectionLock.Unlock();
		}

		if (vGlobalDisplayDialogThread)
			vGlobalDisplayDialogThread->NextStep();//step9

		//if (vGlobalDisplayDialogThread)
		//	vGlobalDisplayDialogThread->NextStep();//step10

		if (ReadBackgroundImageFromFile(TempProduct))
			MakeProductBackgroundImageCorrectionFactors();

		//problem in demo program with histogram but do not know what the problem is
		//gives loading ITEX DLL Problem
		if (!vGlobalDemoMode)
		{
			CString TempProcessType = "=Process 3";
			vProductHistogramImage = LoadLearnImageFromFile(TempProcessType, vGlobalCurrentProduct, vProductHistogramImage, "Product Process 3");	
		}
	}
}

void CITIPCDig::RestoreALearnFileFromBackup(CString TempLearnName, CProduct *TempProduct)
{
	//restore file
	CString *TempProductName = TempProduct->GetProductName();
	CString TempString1 =  *TempProductName + TempLearnName;
	if (!vUsingLocalSetupDataFile)
		TempString1 = cSetupDataDirectory + TempString1;
	else
		TempString1 = vGlobalCurrentDirectory + TempString1;
	TempString1 = TempString1 + ".BMP";
	LPTSTR TempString = TempString1.GetBuffer(TempString1.GetLength());

	CString TempString2 =  "TempBackup" + TempLearnName;
	if (!vUsingLocalSetupDataFile)
		TempString2 = cSetupDataDirectory + TempString2;
	else
		TempString2 = vGlobalCurrentDirectory + TempString2;
	TempString2 = TempString2 + ".BMP";
	LPTSTR TempNewFileNameString = TempString2.GetBuffer(TempString2.GetLength());

	CFile TempFile;
	CFileStatus TempFileStatus;
	if (CFile::GetStatus(TempNewFileNameString, TempFileStatus))
	{
		if (CFile::GetStatus(TempString, TempFileStatus))
		if (!DeleteFile(TempString))
			ReportErrorMessage("Failed to Delete File: " + TempString1, cEMailInspx, 32000);
		if (!CFile::GetStatus(TempString, TempFileStatus))
			TempFile.Rename(TempNewFileNameString,TempString);
	}
	vALearnedImageHasChanged = true;

	//restore 'B'
	TempProductName = TempProduct->GetProductName();
	TempString1 =  *TempProductName + TempLearnName;
	if (!vUsingLocalSetupDataFile)
		TempString1 = cSetupDataDirectory + TempString1;
	else
		TempString1 = vGlobalCurrentDirectory + TempString1;
	TempString1 = TempString1 + "B.BMP";
	TempString = TempString1.GetBuffer(TempString1.GetLength());

	TempString2 =  "TempBackup" + TempLearnName;
	if (!vUsingLocalSetupDataFile)
		TempString2 = cSetupDataDirectory + TempString2;
	else
		TempString2 = vGlobalCurrentDirectory + TempString2;
	TempString2 = TempString2 + "B.BMP";
	TempNewFileNameString = TempString2.GetBuffer(TempString2.GetLength());

	TempFile;
	TempFileStatus;
	if (CFile::GetStatus(TempNewFileNameString, TempFileStatus))
	{
		if (CFile::GetStatus(TempString, TempFileStatus))
		if (!DeleteFile(TempString))
			ReportErrorMessage("Failed to Delete File: " + TempString1, cEMailInspx, 32000);
		if (!CFile::GetStatus(TempString, TempFileStatus))
			TempFile.Rename(TempNewFileNameString,TempString);
	}
	vALearnedImageHasChanged = true;
}

void CITIPCDig::RestoreLearnFilesFromBackups(CProduct *TempProduct)
{
	RestoreALearnFileFromBackup("=Process 0", TempProduct); //background learn
	RestoreALearnFileFromBackup("=Process 1", TempProduct); //average learn

	RestoreALearnFileFromBackup("=Process 2", TempProduct); //Structure learn
	RestoreALearnFileFromBackup("=Process 2o", TempProduct); //original Structure learn
	RestoreALearnFileFromBackup("=Process 2u", TempProduct); //undo Structure learn

	RestoreALearnFileFromBackup("=Process 2T", TempProduct); //Contaminant2 Structure learn
	RestoreALearnFileFromBackup("=Process 2To", TempProduct); //original Contaminant2 Structure learn
	RestoreALearnFileFromBackup("=Process 2Tu", TempProduct); //undo Contaminant2 Structure learn

	//RestoreALearnFileFromBackup("=Process 2g", TempProduct); //Glass Structure learn
	//RestoreALearnFileFromBackup("=Process 2go", TempProduct); //Original Glass Structure learn
	//RestoreALearnFileFromBackup("=Process 2gu", TempProduct); //Undo Glass Structure learn

	if (vGlobalAllowVoidInspection)
	{
		RestoreALearnFileFromBackup("=Process 2v", TempProduct); //void Structure learn
		RestoreALearnFileFromBackup("=Process 2vo", TempProduct); //original void Structure learn
		RestoreALearnFileFromBackup("=Process 2vu", TempProduct); //undo void Structure learn
	}
	RestoreALearnFileFromBackup("=Process 3", TempProduct); //Histogram learn
}

#ifndef UseIFCDLLs

Erflag mvt_mirror_y(Image *TempImageIn)
{
	return 0;
}

Erflag mvt_blob_get_perim(MVT_BLOB_RESULTS *TempResults, WORD TempBlobNumber)
{
	return 0;
}

Erflag mvt_dilate_rect(Image *TempImageIn, WORD TempDilateTimesX, WORD TempDilateTimesY, WORD TempHandleEdges)
{
	return 0;
}
					
Erflag mvt_erode_rect(Image *TempImageIn, WORD TempErodeTimesX, WORD TempErodeTimesY, WORD TempHandleEdges)
{
	return 0;
}

Erflag mvt_average(Image *TempImageIn)
{
	return 0;
}

MVT_BLOB::MVT_BLOB()
{
}
MVT_BLOB::~MVT_BLOB()
{
}

MVT_LINE_RESULT::MVT_LINE_RESULT()
{
}
MVT_LINE_RESULT::~MVT_LINE_RESULT()
{
}
void MVT_LINE_RESULT::WriteFile(BYTE *TempPointer,
						DWORD TempHeigth,
						DWORD TempWidth,
						WORD TempBitsPerPixel)
{
}
	
MVT_BLOB_RESULTS::MVT_BLOB_RESULTS()
{
}
MVT_BLOB_RESULTS::~MVT_BLOB_RESULTS()
{
}
void MVT_BLOB_RESULTS::DoWork()
{
}

MVT_BLOB_PARAMS::MVT_BLOB_PARAMS()
{
}
MVT_BLOB_PARAMS::~MVT_BLOB_PARAMS()
{
}
void MVT_BLOB_PARAMS::DoWork()
{
}
	Erflag mvt_extrema(Image *TempImage,
		int TempPixelIntensity,
		int TempLevel,
		int TempMinimumPixelRunLengthX,
		int TempEdgeFlag[4],
		double *TempX, 
		double *TempY)
	{
		return 0;
	}

	Erflag mvt_sobelx(Image *TempImageIn,
		Image *TempImageOut,BYTE TempStle)
	{
		return 0;
	}

	Erflag mvt_sobely(Image *TempImageIn,
		Image *TempImageOut,BYTE TempStle)
	{
		return 0;
	}

	Erflag mvt_blur(Image *TempImageIn,
		DWORD Temp1, DWORD Temp2, float Temp3, DWORD Temp4)
	{
		return 0;
	}
	Erflag mvt_dilate(Image *TempImageIn, Image *TempImageOut)
	{
		return 0;
	}

	Erflag mvt_sobelxy(Image *TempImageIn, Image *TempImageOut,BYTE TempStle)
	{
		return 0;
	}

	Erflag mvt_prewittx(Image *TempImageIn,
		Image *TempImageOut,BYTE TempStle)
	{
		return 0;
	}

	Erflag mvt_prewitty(Image *TempImageIn,
		Image *TempImageOut,BYTE TempStle)
	{
		return 0;
	}

	Erflag mvt_prewittxy(Image *TempImageIn,
		Image *TempImageOut,BYTE TempStle)
	{
		return 0;
	}

	Erflag mvt_erode(Image *TempImageIn,
		Image *TempImageOut)
	{
		return 0;
	}

	Erflag mvt_convolve_byte(Image *TempImageIn,
		int TempRows, int TempColumns, int *TempKernel, 
		int TempShift, int TempOffset,int TempEdgeFlag)
	{
		return 0;
	}

	Erflag mvt_invert(Image *TempImage)
	{
		return 0;
	}

	Erflag mvt_threshold(Image *TempImage, BYTE TempThreshold, BYTE TempSetUp,BYTE TempSetDown)
	{
		return 0;
	}

	Erflag mvt_threshold_down(Image *TempImage, BYTE TempThreshold, BYTE TempSetUp)
	{
		return 0;
	}

	Erflag mvt_caliper_od_array(
		Array *TempLineIn,
		int TempThreshold,
		double *TempWidth,
		double *TempXIn,
		double *TempYIn)
	{
		return 0;
	}

	int mvt_edge_find_image(Image *TempImageIn,
			int TempDirection,
			int TempThreshold,
			bool TempColor,
			DWORD TempReference,
			MVT_LINE_RESULT *TempLineIn)
	{
		return 0;
	}

	Erflag mvt_blob_find(Image *TempImageIn,
		MVT_BLOB_PARAMS *TempBlobParams, 
		MVT_BLOB_RESULTS *TempBlobResults, 
		DWORD coord_flag)
	{
		return 0;
	}

	double mvt_blob_get_area(MVT_BLOB_RESULTS *TempBlobResults, int TempIndex)
	{
		if (TempBlobResults)
		{
			return TempBlobResults->num_blobs_found;		
		}
		else
			return 0;
	}

	MVT_BLOB_PARAMS *mvt_blob_create_params(Erflag *TempErrorFlag)
	{
		*TempErrorFlag = 0;
		return new MVT_BLOB_PARAMS;
	}

	MVT_BLOB_RESULTS *mvt_blob_create_results(
		DWORD TempSize, 
		bool TempCanGrow, 
		Erflag *TempErrorFlag)
	{
		*TempErrorFlag = 0;
		return new MVT_BLOB_RESULTS;
	}

	void mvt_blob_delete_params(MVT_BLOB_PARAMS *TempBlobParameters)
	{
	}

	void mvt_blob_delete_results(MVT_BLOB_RESULTS *TempBlobResults)
	{
	}

#endif

double CITIPCDig::CalculateDensity(BYTE *TempImage, CInspection *TempInspection, 
																	 WORD TempMaximumHeight, WORD TempMaximumWidth)
{ //is called on product setup summary, so may no be current product
	double TempDensity = 0;

	if (TempImage)
	{
		//cant use on other products than current
		//GetInspectionROIBounds(TempInspection, &TempBottom, &TempLeft, &TempHeight, &TempWidth, 0, false, 0);
		WORD TempBottom = 1;
		WORD TempLeft = 1;
		WORD TempHeight = TempMaximumHeight - 2;
		WORD TempWidth = TempMaximumWidth - 2;

		if (TempInspection)
		if ((TempInspection->vROIPixelTop) && (TempInspection->vROIPixelRight))
		{
			if (TempInspection->vROIPixelTop > TempMaximumHeight)
				TempInspection->vROIPixelTop = TempMaximumHeight - 1;

			TempBottom = TempInspection->vROIPixelBottom;
			if (TempBottom >= TempInspection->vROIPixelTop)
				TempBottom = 0;
			TempHeight = TempInspection->vROIPixelTop - TempBottom;
			if (TempHeight + TempBottom > TempMaximumHeight)
				TempHeight = TempMaximumHeight - TempBottom;

			if (TempInspection->vROIPixelRight > TempMaximumWidth)
				TempInspection->vROIPixelRight = TempMaximumWidth - 1;

			TempLeft = TempInspection->vROIPixelLeft;
			if (TempLeft >= TempInspection->vROIPixelRight)
				TempLeft = 0;
			TempWidth = TempInspection->vROIPixelRight - TempLeft;

			if (TempWidth + TempLeft > TempMaximumWidth)
				TempWidth = TempMaximumWidth - TempLeft;
		}

		/*
		WORD TempTop = (WORD)(TempHeight - 1);
		WORD TempBottom = 1;
		WORD TempLeft = 1;
		WORD TempRight = (WORD)(TempWidth - 1);
		if (TempInspection)
		if (TempInspection->vROIPixelTop)  //if ROI is configured
		if (TempInspection->vROIPixelRight) //if ROI is configured
		if (TempInspection->vROIPixelTop <= TempHeight)  
		if (TempInspection->vROIPixelRight <= TempWidth)
		if (TempInspection->vROIPixelTop >
						TempInspection->vROIPixelBottom)
		{
			TempTop = TempInspection->vROIPixelTop;
			TempBottom = TempInspection->vROIPixelBottom;
			TempLeft = TempInspection->vROIPixelLeft;
			TempRight = TempInspection->vROIPixelRight;
		}
		if (TempTop > TempHeight - 1)
			TempTop = TempHeight - 1;
		if (TempBottom >= TempTop - 1)
			TempBottom = 1;
		if (TempRight > TempWidth - 1)
			TempRight = TempWidth - 1;
		if (TempLeft >= TempRight - 1)
			TempLeft = 1;
		*/

		double TempSum = 0;
		BYTE *TempPointer = NULL;
		DWORD TempCount = 0;
		//BYTE *TempHistogramPointer = NULL;

		WORD TempRight = TempLeft + TempWidth;
		if (TempRight >= TempMaximumWidth)
			TempRight = TempMaximumWidth - 1;

		for (WORD TempLoopY = TempLeft; TempLoopY < TempRight; TempLoopY++)
		{
			//if (vProductHistogramImage)
			//	TempHistogramPointer = vProductHistogramImage->vGreyImage + TempBottom + 
			//	(TempLoopY * vOriginalBufferSizeX);
			TempPointer = TempImage + TempBottom + (TempLoopY * TempMaximumHeight);

			for (WORD TempLoopX = 0; TempLoopX < TempHeight; TempLoopX++)
			{
				//if ((!vProductHistogramImage) || (*TempHistogramPointer == 0))
				//if (*TempPointer < 254)
				{
					TempSum = TempSum + *TempPointer;
					TempCount++;
				}
				//TempHistogramPointer++;
				TempPointer++;
			}
		}
		if (TempCount)
			TempDensity = TempSum / TempCount;
	}
	return TempDensity;
}

double CITIPCDig::CalculateDensityInImage(BYTE *TempImage, WORD TempLeft, WORD TempRight, WORD TempTop)
{ 
	double TempDensity = 0;

	if (TempImage)
	{
		//cant use on other products than current
		//GetInspectionROIBounds(TempInspection, &TempBottom, &TempLeft, &TempHeight, &TempWidth, 0, false, 0);
		WORD TempBottom = (WORD)(TempTop * 0.1);

		double TempSum = 0;
		BYTE *TempPointer = NULL;
		BYTE *TempMaximumPointer = TempImage + vOriginalBufferSize;
		DWORD TempCount = 0;

		for (WORD TempLoopY = TempLeft; TempLoopY < TempRight; TempLoopY++)
		{
			TempPointer = TempImage + TempBottom + (TempLoopY * vOriginalBufferSizeX);

			for (WORD TempLoopX = TempBottom; TempLoopX < TempTop; TempLoopX++)
			if (TempPointer < TempMaximumPointer)
			{
				TempSum = TempSum + *TempPointer++;
				TempCount++;
			}
		}
		if (TempCount)
			TempDensity = TempSum / TempCount;
	}
	return TempDensity;
}

double CITIPCDig::CalculateInspectionStandardDeviation(BYTE *TempImagePointer, CInspection *TempInspection, double *TempAverage)
{
	double TempReturn = 0;
	if (TempInspection->vROIPixelTop)  //if ROI is configured
	if (TempInspection->vROIPixelRight) //if ROI is configured
	if (TempInspection->vROIPixelTop <= vOriginalBufferSizeX)  
	if (TempInspection->vROIPixelRight <= vAquisitionSizeY)
	if (TempInspection->vROIPixelTop > TempInspection->vROIPixelBottom)
	if (TempImagePointer)
	{
		WORD TempBottom = 0;
		WORD TempLeft = 0;
		WORD TempHeight = 0;
		WORD TempWidth = 0;
		GetInspectionROIBounds(TempInspection, &TempBottom, &TempLeft, &TempHeight, &TempWidth, 0, false, 0);

		DWORD TempNumberOfPixelsInROI = TempWidth * TempHeight;
		BYTE *TempData = NULL;
		if (TempNumberOfPixelsInROI)
		if (ThereIsEnoughMemory(TempNumberOfPixelsInROI, "Temp Image Buffer"))
			TempData = (BYTE *)malloc(TempNumberOfPixelsInROI);

		if (TempData)
		{
			BYTE *TempSourcePointer = TempImagePointer;
			BYTE *TempDestination = (BYTE *)TempData;
			for (WORD TempLoopY = TempLeft; TempLoopY < TempLeft + TempWidth; TempLoopY++)
			{
				TempSourcePointer = TempImagePointer + TempBottom + (TempLoopY * vOriginalBufferSizeX);
				for (WORD TempLoopX = 0; TempLoopX < TempHeight; TempLoopX++)
					*TempDestination++ = *TempSourcePointer++;
			}
			TempReturn = CalculateStandardDeviation(TempData, TempNumberOfPixelsInROI, TempAverage);
			free(TempData);
		}
	}
	return TempReturn;
}

double CITIPCDig::CalculateInspectionStandardDeviationAfterReference(BYTE *TempImagePointer, CInspection *TempInspection, int TempSideReference)
{
	double TempReturn = -1;
	if (TempInspection->vROIPixelTop)  //if ROI is configured
	if (TempInspection->vROIPixelRight) //if ROI is configured
	if (TempInspection->vROIPixelTop <= vOriginalBufferSizeX)  
	if (TempInspection->vROIPixelRight <= vAquisitionSizeY)
	if (TempInspection->vROIPixelTop > TempInspection->vROIPixelBottom)
	if (TempImagePointer)
	{
		WORD TempBottom = 0;
		WORD TempLeft = 0;
		WORD TempHeight = 0;
		WORD TempWidth = 0;
		GetInspectionROIBounds(TempInspection, &TempBottom, &TempLeft, &TempHeight, &TempWidth, 0, false, 0);

		TempLeft = TempLeft + TempSideReference;
		WORD TempRight = TempLeft + TempWidth; //reference is already included in left edge
		if (TempRight >= vOriginalBufferSizeY)
			TempRight = vOriginalBufferSizeY - 1;
		if (TempLeft >= TempRight) 
			TempLeft = 0;

		DWORD TempNumberOfPixelsInROI = TempWidth * TempHeight;
		BYTE *TempData = NULL;
		if (TempNumberOfPixelsInROI)
		if (ThereIsEnoughMemory(TempNumberOfPixelsInROI, "Temp Image Buffer"))
			TempData = (BYTE *)malloc(TempNumberOfPixelsInROI);

		if (TempData)
		{
			BYTE *TempSourcePointer = TempImagePointer;
			BYTE *TempDestination = TempData;
			for (WORD TempLoopY = TempLeft; TempLoopY < TempRight; TempLoopY++)
			{
				TempSourcePointer = TempImagePointer + TempBottom + (TempLoopY * vOriginalBufferSizeX);
				for (WORD TempLoopX = 0; TempLoopX < TempHeight; TempLoopX++)
					*TempDestination++ = *TempSourcePointer++;
			}
			double TempAverage = 0;
			TempReturn = CalculateStandardDeviation(TempData, TempNumberOfPixelsInROI, &TempAverage);
			free(TempData);
		}
	}
	return TempReturn;
}

double CITIPCDig::CalculateInspectionDensityAfterReference(BYTE *TempImagePointer, CInspection *TempInspection, int TempSideReference, int TempTopReferenceAdjust,
	BYTE TempReferenceToEdges, WORD TempContainerLeft, WORD TempContainerRight)
{
	double TempReturn = -1;
	if (TempImagePointer)
	if (TempInspection)
	if (TempInspection->vROIPixelTop)  //if ROI is configured
	if (TempInspection->vROIPixelRight) //if ROI is configured
	if (TempInspection->vROIPixelTop <= vOriginalBufferSizeX)  
	if (TempInspection->vROIPixelRight <= vAquisitionSizeY)
	if (TempInspection->vROIPixelTop > TempInspection->vROIPixelBottom)
	{
		double TempSum = 0;
		BYTE *TempPointer = NULL;
		DWORD TempCount = 0;

		WORD TempBottom = 0;
		WORD TempLeft = 0;
		WORD TempHeight = 0;
		WORD TempWidth = 0;
		GetInspectionROIBounds(TempInspection, &TempBottom, &TempLeft, &TempHeight, &TempWidth, 0, false, 0);

		int TempRight = TempLeft + TempWidth + TempSideReference;
		TempLeft = TempLeft + TempSideReference;

		if (TempRight >= vOriginalBufferSizeY)
			TempRight = vOriginalBufferSizeY - 1;
		if (TempLeft >= TempRight)
			TempLeft = 1;
		if (TempTopReferenceAdjust)
		{
			if ((TempTopReferenceAdjust < 0) && (-TempTopReferenceAdjust > TempBottom))
				TempBottom = -TempTopReferenceAdjust;
			if ((TempTopReferenceAdjust > 0) && (TempTopReferenceAdjust > vOriginalBufferSizeX - TempBottom - TempHeight))
				TempHeight = vOriginalBufferSizeX - TempTopReferenceAdjust - TempBottom;
		}

		if (TempHeight >= vOriginalBufferSizeX - 1)
			TempHeight = vOriginalBufferSizeX - 2;
		if (TempBottom + TempHeight >= vOriginalBufferSizeX)
			TempBottom = vOriginalBufferSizeX - 2 - TempHeight;

		BYTE *TempImagePointerMaximum = TempImagePointer + vOriginalBufferSize;
		for (WORD TempLoopY = TempLeft; TempLoopY < TempRight; TempLoopY++)
		{
			if (TempInspection->vReferenceToTop)
				TempPointer = TempImagePointer + TempBottom + TempTopReferenceAdjust + (TempLoopY * vOriginalBufferSizeX);
			else
				TempPointer = TempImagePointer + TempBottom + (TempLoopY * vOriginalBufferSizeX);

			for (WORD TempLoopX = 0; TempLoopX < TempHeight; TempLoopX++)
			{
				if ((TempPointer >= TempImagePointerMaximum) || (TempPointer < TempImagePointer))
					ReportErrorMessage("Pointer Error CalculateInspectionDensityAfterReference L:" + 
					dtoa(TempLeft,0) + ", R:" + dtoa(TempRight,0) + ", H:" + dtoa(TempHeight,0),cEMailInspx,32000);
				else
					TempSum = TempSum + *TempPointer++;
				TempCount++;
			}
		}
		if (TempCount)
			TempReturn = TempSum / TempCount;
	}
	return TempReturn;
}

bool CITIPCDig::AreThereAnyAirBubbles(BYTE *TempImagePointer, CInspection *TempInspection)
{
	bool TempReturn = false;
	if (TempImagePointer)
	if (TempInspection)
	if (TempInspection->vROIPixelTop)  //if ROI is configured
	if (TempInspection->vROIPixelTop <= vOriginalBufferSizeX)  
	if (TempInspection->vROIPixelTop > TempInspection->vROIPixelBottom)
	{
		double TempSum = 0;
		BYTE *TempPointer = NULL;
		DWORD TempCount = 0;

		WORD TempHeight = TempInspection->vROIPixelTop - TempInspection->vROIPixelBottom;

		BYTE *TempImagePointerMaximum = TempImagePointer + vOriginalBufferSize;
		for (WORD TempLoopY = 0; TempLoopY < vOriginalBufferSizeY; TempLoopY++)
		{
			TempPointer = TempImagePointer + TempInspection->vROIPixelBottom + (TempLoopY * vOriginalBufferSizeX);

			for (WORD TempLoopX = 0; TempLoopX < TempHeight; TempLoopX++)
			{
				if ((TempPointer >= TempImagePointerMaximum) || (TempPointer < TempImagePointer))
					ReportErrorMessage("Pointer Error AreThereAnyAirBubbles H:" + dtoa(TempHeight,0),cEMailInspx,32000);
				else
				if (*TempPointer++ >= cImageBrightnessThreshold)
					TempCount++;
			}
		}
		if (TempCount > (DWORD)TempHeight)
			TempReturn = true;
	}
	return TempReturn;
}

double CITIPCDig::CalculatePipelineProductDensity(BYTE *TempImagePointer, CInspection *TempInspection)
{
	double TempReturn = 255; //indicates pipe is empty
	if (TempImagePointer)
	if (TempInspection)
	if (TempInspection->vROIPixelTop)  //if ROI is configured
	if (TempInspection->vROIPixelTop <= vOriginalBufferSizeX)  
	if (TempInspection->vROIPixelTop > TempInspection->vROIPixelBottom)
	{
		double TempSum = 0;
		DWORD TempCount = 0;

		WORD TempHeight = TempInspection->vROIPixelTop - TempInspection->vROIPixelBottom;
		BYTE *TempMinimumValuesArray = NULL;
		if (ThereIsEnoughMemory(TempHeight + 2, "TempMinimumValuesArray"))
			TempMinimumValuesArray = (BYTE*)malloc(TempHeight + 2);

		if (TempMinimumValuesArray)
		{
			FillMemory(TempMinimumValuesArray, TempHeight, 255);

			//find the minimum value of each pixel (across the horizontal lines)
			for (WORD TempPixelLoop = 0; TempPixelLoop < TempHeight; TempPixelLoop++)
			{
				BYTE *TempMinimumValuePointer = TempMinimumValuesArray + TempPixelLoop;
				BYTE *TempPixelPointer = TempImagePointer + TempPixelLoop + TempInspection->vROIPixelBottom;
				for (WORD TempLineLoop = 0; TempLineLoop < vOriginalBufferSizeY; TempLineLoop++)
				{
					if (*TempPixelPointer < *TempMinimumValuePointer)
						*TempMinimumValuePointer = *TempPixelPointer;

					TempPixelPointer = TempPixelPointer + vOriginalBufferSizeX;
				}
			}
			//take the average of the minimum values of each pixel
			BYTE *TempMinimumValuePointer = TempMinimumValuesArray;
			for (WORD TempPixelLoop = 0; TempPixelLoop < TempHeight; TempPixelLoop++)
			{
				if ((*TempMinimumValuePointer > 15) && (*TempMinimumValuePointer < 230))
				{
					TempSum = TempSum + *TempMinimumValuePointer;
					TempCount++;
					TempMinimumValuePointer++;
				}
			}
			if (TempCount > (DWORD)TempHeight * 3 / 4) //if enough pixels had a resonable value
				TempReturn = TempSum / TempCount; //average of minimum values

			if (TempMinimumValuesArray)
				free(TempMinimumValuesArray);
		}

		//TempSum = 0;
		//TempCount = 0;

		//double *TempAverageValuesArray = NULL;
		//if (ThereIsEnoughMemory((TempHeight + 2) * sizeof(double), "TempMinimumValuesArray"))
		//	TempAverageValuesArray = (double*)malloc((TempHeight + 2) * sizeof(double));

		//double *TempAveragePointer = TempAverageValuesArray;
		//for (WORD TempPixelLoop = 0; TempPixelLoop < TempHeight; TempPixelLoop++)
		//	*TempAveragePointer++ = 0;

		////take the average of each pixel across the horizontal lines of the pixels within 50 of the minimum (others too bright like they have airbubbles)
		//for (WORD TempPixelLoop = 0; TempPixelLoop < TempHeight; TempPixelLoop++)
		//{
		//	TempCount = 0;
		//	double *TempAveragePointer = TempAverageValuesArray + TempPixelLoop;
		//	BYTE *TempMinimumPointer = TempMinimumValuesArray + TempPixelLoop;
		//	BYTE *TempPixelPointer = TempImagePointer + TempPixelLoop + TempInspection->vROIPixelBottom;
		//	for (WORD TempLineLoop = 0; TempLineLoop < vOriginalBufferSizeY; TempLineLoop++)
		//	{
		//		if (*TempPixelPointer < *TempMinimumPointer + 50)
		//		{
		//			*TempAveragePointer = *TempAveragePointer + *TempPixelPointer;
		//			TempCount++;
		//		}
		//		TempPixelPointer = TempPixelPointer + vOriginalBufferSizeX;
		//	}
		//	//if atleast 3/4 of the pixels on the line were within 50 of the minimum value, then take the average of them to get the average of that pixel
		//	if (TempCount > (WORD)(vOriginalBufferSizeY * 3 / 4))
		//		*TempAveragePointer = *TempAveragePointer / TempCount;
		//	else
		//		*TempAveragePointer = 0;
		//}
		//TempSum = 0;
		//TempCount = 0;
		//TempAveragePointer = TempAverageValuesArray;
		//for (WORD TempPixelLoop = 0; TempPixelLoop < TempHeight; TempPixelLoop++)
		//{
		//	if ((*TempAveragePointer > 25) && (*TempAveragePointer < 230))
		//	{
		//		TempSum = TempSum + *TempAveragePointer;
		//		TempCount++;
		//		TempAveragePointer++;
		//	}
		//}
		////if at least 3/4 the pixels had average values, then return the brightness
		//if (TempCount > (WORD)(TempHeight * 3 / 4))
		//	TempReturn = TempSum / TempCount;
	}
	return TempReturn;
}

DWORD CITIPCDig::CalculateUnderfillByWeight(BYTE *TempImagePointer, CInspection *TempInspection, int TempReferenceAmount)
{
	DWORD TempReturn = 0;
	if (TempInspection)
	if (TempInspection->vROIPixelTop)  //if ROI is configured
	if (TempInspection->vROIPixelRight) //if ROI is configured
	if (TempInspection->vROIPixelTop <= vOriginalBufferSizeX)  
	if (TempInspection->vROIPixelRight <= vAquisitionSizeY)
	if (TempInspection->vROIPixelTop > TempInspection->vROIPixelBottom)
	if (TempInspection->vROIPixelRight > TempInspection->vROIPixelLeft)
	if (TempImagePointer)
	{
		BYTE *TempPointer = NULL;

		WORD TempBottom = 0;
		WORD TempLeft = 0;
		WORD TempHeight = 0;
		WORD TempWidth = 0;
		GetInspectionROIBounds(TempInspection, &TempBottom, &TempLeft, &TempHeight, &TempWidth, TempReferenceAmount, false, 0);

		if (TempLeft > -TempReferenceAmount)
			TempLeft = TempLeft + TempReferenceAmount;

		for (WORD TempLoopY = TempLeft; TempLoopY < TempLeft + TempWidth; TempLoopY++)
		{
			TempPointer = TempImagePointer + TempBottom + (TempLoopY * vOriginalBufferSizeX);

			for (WORD TempLoopX = 0; TempLoopX < TempHeight; TempLoopX++)
			{
				TempReturn = TempReturn + (255 - *TempPointer++);
			}
		}
	}
	//if (TempReturn < 5001)
	//	if (((TempInspection->vCheckWeighMFactor != 1) || (TempInspection->vCheckWeighBFactor != 0)) && (TempInspection->vCheckWeighMFactor)) //must have calibration factors
	//		TempReturn = 5001;
	return TempReturn;
}

double CITIPCDig::CalculateInspectionDensity(BYTE *TempImagePointer, CInspection *TempInspection)
{
	double TempReturn = -1;
	if (TempInspection->vROIPixelTop)  //if ROI is configured
	if (TempInspection->vROIPixelRight) //if ROI is configured
	if (TempInspection->vROIPixelTop <= vOriginalBufferSizeX)  
	if (TempInspection->vROIPixelRight <= vAquisitionSizeY)
	if (TempInspection->vROIPixelTop > TempInspection->vROIPixelBottom)
	if (TempImagePointer)
	{
		double TempSum = 0;
		BYTE *TempPointer = NULL;
		DWORD TempCount = 0;

		WORD TempBottom = 0;
		WORD TempLeft = 0;
		WORD TempHeight = 0;
		WORD TempWidth = 0;
		GetInspectionROIBounds(TempInspection, &TempBottom, &TempLeft, &TempHeight, &TempWidth, 0, false, 0);

		for (WORD TempLoopY = TempLeft; TempLoopY < TempLeft + TempWidth; TempLoopY++)
		{
			TempPointer = TempImagePointer + TempBottom + (TempLoopY * vOriginalBufferSizeX);

			for (WORD TempLoopX = 0; TempLoopX < TempHeight; TempLoopX++)
			{
				TempSum = TempSum + *TempPointer++;
				TempCount++;
			}
		}
		if (TempCount)
			TempReturn = TempSum / TempCount;
	}
	return TempReturn;
}

double CITIPCDig::CalculateFullDensity(BYTE *TempPointer, DWORD TempSize)
{
	//ReportErrorMessage("Error-110 CalculateFullDensity",cEMailInspx,32000);
	double TempReturn = 0;
	if (TempPointer)
	if (TempSize)
	{
		DWORD TempCount = 0;
		double TempSum = 0;
		BYTE *TempCheckPointer = TempPointer;
		for (DWORD TempLoop = 0; TempLoop < TempSize; TempLoop++)
		{
			if (*TempPointer < 250)
			{
				TempSum = TempSum + *TempPointer;
				TempCount++;
			}
			TempPointer++;
		}
		if (TempCount)
			TempReturn =  TempSum / TempCount;
	}
	return TempReturn;
}

void CITIPCDig::CalculateAverageDriftDensity(CProduct *TempProduct) 
{
	if ((vProductAverageImage) && (vProductAverageImage->vGreyImage))
	{
		if (TempProduct->vDriftDensityInspection)
		{
			TempProduct->vSetupHistoryAverageDensity = CalculateInspectionDensity(vProductAverageImage->vGreyImage, TempProduct->vDriftDensityInspection); 
		}
		else
		{
			TempProduct->vSetupHistoryAverageDensity = CalculateFullDensity(vProductAverageImage->vGreyImage, vOriginalBufferSize);
		}
	}
}


void CITIPCDig::SaveProcess4ImageData(CProduct *TempProduct) 
{
	if ((vProductAverageImage) && (vProductAverageImage->vGreyImage))
	{
		SaveThresholdImageInHistogram(TempProduct, TempProduct->vHistogramThreshold, vProductAverageImage->vGreyImage, TempProduct->vHistogramDilateTimes, TempProduct->vHistogramLowerThreshold);

		TempProduct->vSetupHistoryHistogramThreshold = TempProduct->vHistogramThreshold;
		vALearnedImageHasChanged = true;
	}
}

/*
void CITIPCDig::AddImageToGlassStructure(BYTE *TempImageBufferPointer, 
																				 int TempSideReferenceAmount, bool TempSaveToFile)
{
	bool TempWorked = false;
	if (vGlobalCurrentProduct)
	if (vProductGlassStructureImage)
		//ReportErrorMessage("No Glass Structure, Cant Add To Learn",cEMailInspx,32000);
	//else
	if (!TempImageBufferPointer)
		ReportErrorMessage("Nill Pointer, Cant Add To Learn",cEMailInspx,32000);
	else
	if (!ThereIsEnoughMemory(vOriginalBufferSize, "Display Buffer"))
		ReportErrorMessage("No Memory, Cant Add To Learn",cEMailInspx,32000);
	else
	{
		Image *TempWorkingImage = NULL;
		if (ThereIsEnoughMemory(vOriginalBufferSize + sizeof(Image), "Enhance Image Buffer"))
			TempWorkingImage = im_create(IM_BYTE, vOriginalBufferSizeX,vAquisitionSizeY);

		if (TempWorkingImage)
		{
			CopyMemory(
				TempWorkingImage->pix,
				TempImageBufferPointer,
				vOriginalBufferSize);

			//must reference the image
			if (TempSideReferenceAmount)
			{
				MakeReferenceImageAdjustment(
					TempWorkingImage->pix,
					0,//TempTopReferenceAmount,
					TempSideReferenceAmount);
			}
			//ProcessGlassContaminants(TempWorkingImage, NULL);

			BYTE *TempSourcePointer = TempWorkingImage->pix;
			BYTE *TempStructurePointer = vProductGlassStructureImage->vGreyImage;
			double TempValue = 0;
			for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
			{
				if (255 - *TempSourcePointer > *TempStructurePointer)
					*TempStructurePointer = 255 - *TempSourcePointer;

				TempStructurePointer++;
				TempSourcePointer++;
			}
			// need to write new structure to disk
			if (TempSaveToFile)
				SaveGlassStructureImageToFile(vGlobalCurrentProduct,
					vProductGlassStructureImage, false);

			im_delete(TempWorkingImage);
		}
	}
}
*/
bool CITIPCDig::AddImageToStructure(CImageData *TempImageBufferPointer, CInspection *TempInspection)
{
	bool TempWorked = false;
	if (vGlobalCurrentProduct)
	if (!vProductStructureImage)
		ReportErrorMessage("No Structure, Cant Add To Learn",cEMailInspx,32000);
	else
	if (!TempImageBufferPointer) 
		ReportErrorMessage("Nill Pointer, Cant Add To Learn",cEMailInspx,32000);
	else
	if (!ThereIsEnoughMemory(vOriginalBufferSize, "Display Buffer"))
		ReportErrorMessage("No Memory, Cant Add To Learn",cEMailInspx,32000);
	else
	if (TempImageBufferPointer->vGreyImage)
	{
		BYTE *TempBuffer = NULL;
		if (ThereIsEnoughMemory(vOriginalBufferSize, "Add To Learn Buffer"))
			TempBuffer = (BYTE*)malloc(vOriginalBufferSize);
		BYTE *TempBuffer1 = NULL;
		if (ThereIsEnoughMemory(vOriginalBufferSize, "Add To Learn Buffer"))
			TempBuffer1 = (BYTE*)malloc(vOriginalBufferSize);
		if ((TempBuffer) && (TempBuffer1))
		{
			int TempTopReferenceAmount = 0;
			if (TempInspection)
			if (TempInspection->vReferenceToTop)
				TempTopReferenceAmount = TempImageBufferPointer->vTopReferenceAdjust;
			WORD TempBottom = 0;
			if (TempInspection)
				TempBottom = TempInspection->vROIPixelBottom;

			CopyMemory(
				TempBuffer,
				TempImageBufferPointer->vGreyImage,
				vOriginalBufferSize);

			//must reference the image
			if ((TempImageBufferPointer->vLeftReferenceAdjust) ||
				(TempTopReferenceAmount))
			{
				MakeReferenceImageAdjustment(
					TempBuffer, TempBuffer1,
					TempTopReferenceAmount,
					TempImageBufferPointer->vLeftReferenceAdjust);
			}
			if (TempTopReferenceAmount)
			{ //must blank out part below inspection ROI so don't add it to structure because moved off reference point
				BYTE *TempPointer = TempBuffer;
				for (DWORD TempLineLoop = 0; TempLineLoop < vOriginalBufferSizeY; TempLineLoop++)
				{
					TempPointer = TempBuffer + (TempLineLoop * vOriginalBufferSizeX);
					for (DWORD TempPixelLoop = 0; TempPixelLoop < TempBottom; TempPixelLoop++)
					{
						*TempPointer = 255;
						TempPointer++;
					}
				}
			}
			//Add image to Contaminant2 Structure
			if ((vProductContaminant2StructureImage) && (vProductContaminant2StructureImage->vGreyImage))
			{
				BYTE *TempPointerImageStructure = vProductContaminant2StructureImage->vGreyImage;
				BYTE *TempPointerAquisitionImage = TempBuffer;

				for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
				{//Take Minimum pixel to Structure
					if (*TempPointerImageStructure > *TempPointerAquisitionImage)
						*TempPointerImageStructure = *TempPointerAquisitionImage;  
					TempPointerImageStructure++;
					TempPointerAquisitionImage++;  
				}

				if ((vProductContaminant2StructureImageWithNoDilation) && (vProductContaminant2StructureImageWithNoDilation->vGreyImage))
				{ //current one has dilations in it, so must add to undilated one, then write to file as we only dilate when read from file which is undilated
					BYTE *TempPointerImageStructure = vProductContaminant2StructureImageWithNoDilation->vGreyImage;
					BYTE *TempPointerAquisitionImage = TempBuffer;

					for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
					{//Take Minimum pixel to Structure
						if (*TempPointerImageStructure > *TempPointerAquisitionImage)
							*TempPointerImageStructure = *TempPointerAquisitionImage;  
						TempPointerImageStructure++;
						TempPointerAquisitionImage++;  
					}
					SaveContaminant2StructureImageToFile(vGlobalCurrentProduct, vProductContaminant2StructureImageWithNoDilation, false);
				}
				else
					SaveContaminant2StructureImageToFile(vGlobalCurrentProduct, vProductContaminant2StructureImage, false);

				//add to Contaminant2 improve learn if have one
				if ((vImproveLearnContaminant2StructureImage) && (vImproveLearnContaminant2StructureImage->vGreyImage))
				{
					BYTE *TempPointerImageStructure = vImproveLearnContaminant2StructureImage->vGreyImage;
					BYTE *TempPointerAquisitionImage = TempBuffer;

					for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
					{//Take Minimum pixel to Structure
						if (*TempPointerImageStructure > *TempPointerAquisitionImage)
							*TempPointerImageStructure = *TempPointerAquisitionImage;  
						TempPointerImageStructure++;
						TempPointerAquisitionImage++;  
					}
				}
			}
						
			//add image to standard learn structure
			//subtract the product's average image from this one if enabled
			if (vProductAverageImage)
				SubtractProductAverageImage(TempBuffer, vProductAverageImage);
			//else
			//if (vAverageImage)
			//	SubtractAverageImage(TempBuffer);

			if ((vProductStructureImage) && (vProductStructureImage->vGreyImage))
			{
				ReportErrorMessage("Added Reject to Learn", cUserChanges,0);

				BYTE *TempSourcePointer = TempBuffer;
				BYTE *TempStructurePointer = vProductStructureImage->vGreyImage;
				double TempValue = 0;
				for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
				{
					if (*TempSourcePointer < 128)
					{
						
						TempValue = 128 - *TempSourcePointer;

						if (TempValue > 128)
							TempValue = 128;
						if (TempValue < 0)
							TempValue = 0;
						if ((BYTE)TempValue > *TempStructurePointer)
							*TempStructurePointer = (BYTE)TempValue;
					}

					TempStructurePointer++;
					TempSourcePointer++;
				}
			}
			//add image to improve learn standard structure
			if ((vImproveLearnStructureImage) && (vImproveLearnStructureImage->vGreyImage))
			{
				BYTE *TempSourcePointer = TempBuffer;
				BYTE *TempStructurePointer = vImproveLearnStructureImage->vGreyImage;
				double TempValue = 0;
				for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
				{
					if (*TempSourcePointer < 128)
					{
						
						TempValue = 128 - *TempSourcePointer;

						if (TempValue > 128)
							TempValue = 128;
						if (TempValue < 0)
							TempValue = 0;
						if ((BYTE)TempValue > *TempStructurePointer)
							*TempStructurePointer = (BYTE)TempValue;
					}

					TempStructurePointer++;
					TempSourcePointer++;
				}
			}

			vNumberOfImagesInLastAddToLearn = 1;
			TempWorked = true;
			//MakeUndoBackupOfLearnStructureFiles(vGlobalCurrentProduct);

			// need to write new structure to disk
			SaveStructureImageToFile(vGlobalCurrentProduct, false);
			ClearImproveLearn(false);

			free(TempBuffer);
			free(TempBuffer1);
		}
		vALearnedImageHasChanged = true;
	}
	return TempWorked;
}

void CITIPCDig::AddImageToVoidStructure(CImageData *TempImageBufferPointer,
																		CInspection *TempInspection)
{
	if ((vVoidProductStructureImage) && (vVoidProductStructureImage->vGreyImage))
	if ((TempImageBufferPointer) && (TempImageBufferPointer->vGreyImage))
	if (ThereIsEnoughMemory(vOriginalBufferSize, "Display Buffer"))
	if (TempInspection)
	{
		BYTE *TempBuffer = (BYTE*)malloc(vOriginalBufferSize);
		BYTE *TempBuffer1 = NULL;
		if (ThereIsEnoughMemory(vOriginalBufferSize, "Add To Learn Buffer"))
			TempBuffer1 = (BYTE*)malloc(vOriginalBufferSize);
		if ((TempBuffer) && (TempBuffer1))
		{
			int TempTopReferenceAmount = 0;
			if (TempInspection->vReferenceToTop)
				TempTopReferenceAmount = TempImageBufferPointer->vTopReferenceAdjust;

			CopyMemory(
				TempBuffer,
				TempImageBufferPointer->vGreyImage,
				vOriginalBufferSize);

			//must reference the image
			if ((TempImageBufferPointer->vLeftReferenceAdjust) ||
				(TempTopReferenceAmount))
			{
				MakeReferenceImageAdjustment(
					TempBuffer, TempBuffer1,
					TempTopReferenceAmount,
					TempImageBufferPointer->vLeftReferenceAdjust);
			}
			if (TempTopReferenceAmount)
			{ //must blank out part below inspection ROI so don't add it to structure because moved off reference point
				BYTE *TempPointer = TempBuffer;
				for (DWORD TempLineLoop = 0; TempLineLoop < vOriginalBufferSizeY; TempLineLoop++)
				{
					TempPointer = TempBuffer + (TempLineLoop * vOriginalBufferSizeX);
					for (DWORD TempPixelLoop = 0; TempPixelLoop < TempInspection->vROIPixelBottom; 
						TempPixelLoop++)
					{
						*TempPointer = 255;
						TempPointer++;
					}
				}
			}
			
			//subtract the product's average image from this one if enabled
			if (vProductAverageImage)
				SubtractVoidProductAverageImage(TempBuffer);

			BYTE *TempSourcePointer = TempBuffer;
			BYTE *TempStructurePointer = vVoidProductStructureImage->vGreyImage;
			for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
			{
				if (*TempSourcePointer < 128)
				{
					BYTE TempByte = 128 - *TempSourcePointer;
					if (TempByte > *TempStructurePointer)
						*TempStructurePointer = TempByte;
				}

				TempStructurePointer++;
				TempSourcePointer++;
			}

			// need to write new structure to disk
			SaveVoidStructureImageToFile(vGlobalCurrentProduct, false);

		}
		if (TempBuffer)
			free(TempBuffer);
		if (TempBuffer1)
			free(TempBuffer1);
		vALearnedImageHasChanged = true;
	}
}

void CITIPCDig::FindContainerDimensions(BYTE *TempImage, Image *TempScratchImage, WORD *TempTop, WORD *TempBottom,
		WORD *TempRight, WORD *TempLeft, WORD *TempHeight, WORD *TempWidth, bool TempUseRelativeThreshold, WORD TempShapeHandlingWidthTolerancePixels)
{
	*TempTop = 0;
	*TempBottom = 0;
	*TempRight = 0;
	*TempLeft = 0;
	*TempHeight = 0;
	*TempWidth = 0;

	if (TempShapeHandlingWidthTolerancePixels == 0)
		TempShapeHandlingWidthTolerancePixels = 5;

	if (TempImage)
	if (vGlobalCurrentProduct)
	if ((TempScratchImage) && (TempScratchImage->pix))
	{
		CopyMemory(TempScratchImage->pix, TempImage, vOriginalBufferSize);
		if (vGlobalCurrentProduct->vThresholdForContainerBounds > 0)
		{
			if (vGlobalDemoMode)
			{
				ThresholdBuffer(TempScratchImage->pix,vOriginalBufferSize,vGlobalCurrentProduct->vThresholdForContainerBounds);
			}
			else
			{
				ThresholdImage(TempScratchImage, vGlobalCurrentProduct->vThresholdForContainerBounds);
			}
		}

		if (vGlobalCurrentProduct->vEdgeLocationHeightPixel)
		{
			//FindContainerEdgesCheckingFromCenter(TempScratchImage->pix, TempTop, TempBottom,	TempRight, TempLeft);
			WORD TempFromCenterTop = 0;
			WORD TempFromCenterBottom = 0;
			WORD TempFromCenterRight = 0;
			WORD TempFromCenterLeft = 0;
			WORD TempFromCenterWidth = 0;
			if (vGlobalCurrentProduct->vEdgeLocationHeightPixel)
			{
				FindContainerEdgesCheckingFromCenter(TempScratchImage->pix, &TempFromCenterTop, &TempFromCenterBottom, &TempFromCenterRight, &TempFromCenterLeft);
				TempFromCenterWidth = TempFromCenterRight - TempFromCenterLeft;
				int TempDelta = TempFromCenterWidth - vGlobalCurrentProduct->vReferenceWidth;
				if (TempDelta < 0)
					TempDelta = -TempDelta;

				//or no learned reference, and not at edges
				if (((TempFromCenterLeft > 1)  && (TempFromCenterRight < vOriginalBufferSizeY - 1) && (TempFromCenterLeft + 10 < TempFromCenterRight)) &&
					(((TempDelta <= TempShapeHandlingWidthTolerancePixels) && (vGlobalCurrentProduct->vReferenceWidth)) || 
					((vGlobalCurrentProduct->vReferenceWidth == 0) && (TempFromCenterLeft > 4) && (TempFromCenterRight < vOriginalBufferSizeY - 3))))
				{
					*TempTop = TempFromCenterTop;
					*TempBottom = TempFromCenterBottom;
					*TempRight = TempFromCenterRight;
					*TempLeft = TempFromCenterLeft;
				}
				else
				{
					WORD TempFromEdgesTop = 0;
					WORD TempFromEdgesBottom = 0;
					WORD TempFromEdgesRight = 0;
					WORD TempFromEdgesLeft = 0;
					WORD TempFromEdgesWidth = 0;

					//finds edges looking from edge of picture working to center
					FindContainerEdges(TempScratchImage->pix, &TempFromEdgesTop, &TempFromEdgesBottom, &TempFromEdgesRight, &TempFromEdgesLeft);
					TempFromEdgesWidth = TempFromEdgesRight - TempFromEdgesLeft;
					int TempDelta = TempFromEdgesWidth - vGlobalCurrentProduct->vReferenceWidth;
					if (TempDelta < 0)
						TempDelta = -TempDelta;

				//or no learned reference, and not at edges
					if (((TempDelta <= TempShapeHandlingWidthTolerancePixels) && (vGlobalCurrentProduct->vReferenceWidth)) || 
						((vGlobalCurrentProduct->vReferenceWidth == 0) && (TempFromEdgesLeft > 4)  && (TempFromEdgesRight < vOriginalBufferSizeY - 3)))
					{
						*TempTop = TempFromEdgesTop;
						*TempBottom = TempFromEdgesBottom;
						*TempRight = TempFromEdgesRight;
						*TempLeft = TempFromEdgesLeft;
					}
					else
					{  //neither from center, or from edges found both edges correctly, now see if one found one edge correctly, and the other found the other edge correctly
						int TempRightDelta = TempFromEdgesRight - TempFromCenterLeft - vGlobalCurrentProduct->vReferenceWidth;
						if (TempRightDelta < 0)
							TempRightDelta = -TempRightDelta;

						int TempLeftDelta = TempFromEdgesLeft - TempFromCenterRight - vGlobalCurrentProduct->vReferenceWidth;
						if (TempLeftDelta < 0)
							TempLeftDelta = -TempLeftDelta;

						if ((TempRightDelta <= 5) && (TempFromCenterRight < vOriginalBufferSizeY - 1) && (TempLeftDelta <= 5) && (TempFromCenterLeft > 1))  
						{
							//if both left and right are close to each other, must be container width
							*TempRight = TempFromCenterRight;
							*TempLeft = TempFromCenterLeft;
							*TempTop = TempFromCenterTop;
						}
						else
						if ((TempRightDelta <= TempShapeHandlingWidthTolerancePixels) && (TempFromEdgesRight < vOriginalBufferSizeY - 1) && (TempFromCenterLeft > 1))
						{
							//found edge are correct width and not at edges of images, so set left from right and width reference
							*TempRight = TempFromEdgesRight;
							*TempLeft = TempFromCenterLeft;
							*TempTop = TempFromCenterTop;
						}
						else
						if ((TempLeftDelta <= TempShapeHandlingWidthTolerancePixels) && (TempFromCenterRight < vOriginalBufferSizeY - 1) && (TempFromEdgesLeft > 1))
						{
							//Left edge is correct, so set Right from Left and width reference
							*TempLeft = TempFromEdgesLeft;
							*TempRight = TempFromCenterRight;
							*TempTop = TempFromCenterTop;
							*TempBottom = TempFromCenterBottom;
						}
						else
						{
							//did not find edges so use edges found from searching from edges 
							*TempLeft = TempFromEdgesLeft;
							*TempRight = TempFromCenterRight;
							*TempTop = TempFromCenterTop;
							*TempBottom = TempFromCenterBottom;
						}
					}
				}
			}
		}
		else
		{
			BOOL TempEdgeFlag[4];
			double TempX[4];
			double TempY[4];
			for (BYTE TempLoop = 0; TempLoop < 4; TempLoop++)
			{
				TempX[TempLoop] = 0;
				TempY[TempLoop] = 0;
				TempEdgeFlag[TempLoop] = 0;
			}
			int TempPixelIntensity = 0;  //look for black pixels to define object
			WORD TempMinimumPixelRunLengthX = vOriginalBufferSizeX / 10;
			WORD TempMinimumPixelRunLengthY = vAquisitionSizeY / 10;
			if (TempMinimumPixelRunLengthX > TempMinimumPixelRunLengthY)
				TempMinimumPixelRunLengthX = TempMinimumPixelRunLengthY;
			if (TempMinimumPixelRunLengthX < 5)
				TempMinimumPixelRunLengthX = 5;

			Erflag TempErrorFlag = mvt_extrema(TempScratchImage, TempPixelIntensity,0x10,TempMinimumPixelRunLengthX, TempEdgeFlag,TempX, TempY);

			if (TempErrorFlag == IM_OK)
			{
				*TempBottom = (WORD)(TempX[0] + 1.5);
				*TempTop = (WORD)(TempX[1] + 1.5);
				if (*TempTop >= vOriginalBufferSizeX)
					*TempTop = vOriginalBufferSizeX - 1;
				if (*TempTop < 5)
					*TempTop = vOriginalBufferSizeX - 1;
				if (*TempBottom >= *TempTop)
					*TempBottom = 0;

				//add one so pixel after edge, add .5 to round off
				*TempLeft = (WORD)(TempY[2] + .5);
				if (*TempLeft >= vAquisitionSizeY)
					*TempLeft = 0;
				*TempRight = (WORD)(TempY[3] + 1.5);
			}
		}

		if (vMaximumImageOverlap)
			VerifyContainerEdges(TempScratchImage, TempTop, TempBottom,	TempRight, TempLeft);

		if (*TempRight > vAquisitionSizeY)
			*TempRight = vAquisitionSizeY;

		if (*TempRight < 3)
			*TempRight = vAquisitionSizeY;

		if (*TempLeft >= *TempRight)
			*TempLeft = *TempRight - 1;

		*TempWidth = *TempRight - *TempLeft;

		//ReportErrorMessage("VerifyContainerEdges Right: " + dtoa(*TempRight,0) + ", Left: " +
		//	dtoa(*TempLeft,0), cWriteToLog,32000);
		
		if ((vGlobalInAutoSize) || (TempUseRelativeThreshold))
		{
			//if ((vGlobalInAutoSize == 1) || (vGlobalScanTracType == cSoloScanTrac))
			if ((vGlobalCurrentProduct->vProductImageWidth > 8) || (TempUseRelativeThreshold))//only do relative edges if product width is greater than 8 inches
			if ((*TempRight > vAquisitionSizeY - 3) || (*TempLeft < 3) || (*TempRight - *TempLeft < 20))
			{
				*TempLeft = 0;
				*TempRight = vOriginalBufferSizeY;
				FindContainerEdgesWithRelativeThreshold(TempImage, TempTop, TempBottom,	TempRight, TempLeft, 0, TempUseRelativeThreshold);
			}
		}
		else
		if (!vGlobalInAutoSize)
		if (vGlobalCurrentProduct->vReferenceRight)
		if (vGlobalCurrentProduct->vReferenceWidth)
		if (vGlobalCurrentProduct->vReferenceWidth + cWidthTolerance < *TempWidth)
		{
			ContainerTooWideLookForEdgesAgain(TempScratchImage, TempTop, TempBottom,	TempRight, TempLeft);

			if (*TempRight > vAquisitionSizeY)
				*TempRight = vAquisitionSizeY;

			if (*TempRight < 3)
				*TempRight = vAquisitionSizeY;

			if (*TempLeft >= *TempRight)
				*TempLeft = *TempRight - 1;

			*TempWidth = *TempRight - *TempLeft;
		}

		vContainerWidthData[vContainerWidthIndex] = *TempWidth;

		if ((vContainerWidthData[vContainerWidthIndex] < vContainerWidthMinimum) || (!vContainerWidthMinimum))
			vContainerWidthMinimum = vContainerWidthData[vContainerWidthIndex];

		if (vContainerWidthData[vContainerWidthIndex] > vContainerWidthMaximum)
			vContainerWidthMaximum = vContainerWidthData[vContainerWidthIndex];

		if (vContainerWidthSamples < cMaximumBodyTriggerWidthData - 1)
			vContainerWidthSamples++;

		//find variance and average of Image Width
		if (vContainerWidthSamples > 3)
				vContainerWidthStandardDeviation = CalculateStandardDeviation(vContainerWidthData, vContainerWidthSamples, &vContainerWidthAverage);

		vContainerWidthIndex++;
		if (vContainerWidthIndex >= cMaximumBodyTriggerWidthData)
			vContainerWidthIndex = 0;

		if (*TempTop < 4)
			*TempTop = vOriginalBufferSizeX - 1;

		*TempHeight = *TempTop - *TempBottom;

		if (*TempHeight < 0)
			*TempHeight = 0;
	}
}

void CITIPCDig::FindContainerEdgesWithRelativeThreshold(BYTE *TempImage, WORD *TempTop,  WORD *TempBottom, WORD *TempRight, WORD *TempLeft, WORD TempBottomPixel, bool TempUseRelativeThreshold)
{
	BYTE TempBackgroundValue[1152];
	BYTE TempThresholdForContainer = 255 - vGlobalCurrentProduct->vThresholdForContainerBounds;
	FillMemory(&TempBackgroundValue[0], 1152, 240);
							
	//ReportErrorMessage("FindContainerEdgesWithRelativeThreshold Threshold: " + dtoa(vGlobalCurrentProduct->vThresholdForContainerBounds, 0), cWriteToLog, 0);

	if (TempImage)
	if (vGlobalCurrentProduct)
	{
		WORD TempCheckBottom = 0;
		WORD TempCheckHeight = 0;
		if (vGlobalCurrentProduct->vEdgeLocationHeightPixel)
		{
			TempCheckBottom = vGlobalCurrentProduct->vEdgeLocationBottomPixel;
			TempCheckHeight = vGlobalCurrentProduct->vEdgeLocationHeightPixel;
		}
		else
		{
			TempCheckBottom = (WORD)((double)vOriginalBufferSizeX * .1);
			TempCheckHeight = (WORD)((double)vOriginalBufferSizeX * .8);
		}
		if (TempCheckBottom == 0)
			TempCheckBottom = 1;

		if (TempCheckBottom > (WORD)((double)vOriginalBufferSizeX * .9))
			TempCheckBottom = (WORD)((double)vOriginalBufferSizeX * .9);

		if (TempCheckHeight + TempCheckBottom >= vOriginalBufferSizeX)
			TempCheckHeight = vOriginalBufferSizeX - TempCheckBottom - 1;

		WORD TempCheckRequired = TempCheckHeight / 20;
		if (TempCheckRequired < 2)
			TempCheckRequired = 2;

		if (vGlobalScanTracType == cUpSideDownSoloScanTrac)
		if (TempCheckRequired > 4)
			TempCheckRequired = 4;

		WORD TempStopPoint = 5;  //start at right and move left looking for container until this line
		BYTE *TempPointer;

		BYTE *TempMaximumImagePointer = TempImage + vOriginalBufferSize;
		//fill background array with background values from right edge pixels
		TempPointer = TempImage + TempCheckBottom;// + vOriginalBufferSize - (2 * vOriginalBufferSizeX);
		if (TempUseRelativeThreshold)
		for (WORD TempPixelLoop = 0; TempPixelLoop < TempCheckHeight; TempPixelLoop++)
		{
			if ((TempPointer < TempMaximumImagePointer) && (TempPointer >= TempImage))
			{
				TempBackgroundValue[TempPixelLoop] = 240;
				if (*TempPointer < 250)
					//TempBackgroundValue[TempPixelLoop] = *TempPointer / 2;
					TempBackgroundValue[TempPixelLoop] = *TempPointer - 20;
				TempPointer++;
			}
			else
			{
				ReportErrorMessage("Error-Brian ITIPCDig #15", cWriteToLog, 0);
				ReportErrorMessage("TempPixelLoop: " + dtoa(TempPixelLoop,0), cWriteToLog, 0);
				ReportErrorMessage("TempLeft: " + dtoa(*TempLeft,0), cWriteToLog, 0);
				ReportErrorMessage("TempRight: " + dtoa(*TempRight,0), cWriteToLog, 0);
				ReportErrorMessage("TempStopPoint: " + dtoa(TempStopPoint,0), cWriteToLog, 0);
				ReportErrorMessage("TempCheckBottom: " + dtoa(TempCheckBottom,0), cWriteToLog, 0);
				ReportErrorMessage("TempCheckHeight: " + dtoa(TempCheckHeight,0), cWriteToLog, 0);
				TempPixelLoop = TempCheckHeight;
			}
		}

		//look for container Right edge
		for (WORD TempLineLoop = vOriginalBufferSizeY - 1; TempLineLoop > TempStopPoint; TempLineLoop--)
		{
			TempPointer = TempImage + (TempLineLoop * vOriginalBufferSizeX) + TempCheckBottom;
			WORD TempContainerPixelCount = 0;
			for (WORD TempPixelLoop = 0; TempPixelLoop < TempCheckHeight; TempPixelLoop++)
			{
				if (TempPointer < TempMaximumImagePointer)
				{
					if (*TempPointer++ < TempBackgroundValue[TempPixelLoop])
						TempContainerPixelCount++;  //found a pixel darker than the background by enough
				}
				else
				{
					ReportErrorMessage("Error-Brian ITIPCDig #5", cWriteToLog, 0);
					ReportErrorMessage("TempLineLoop: " + dtoa(TempLineLoop,0), cWriteToLog, 0);
					ReportErrorMessage("TempPixelLoop: " + dtoa(TempPixelLoop,0), cWriteToLog, 0);
					ReportErrorMessage("TempLeft: " + dtoa(*TempLeft,0), cWriteToLog, 0);
					ReportErrorMessage("TempRight: " + dtoa(*TempRight,0), cWriteToLog, 0);
					ReportErrorMessage("TempStopPoint: " + dtoa(TempStopPoint,0), cWriteToLog, 0);
					ReportErrorMessage("TempCheckBottom: " + dtoa(TempCheckBottom,0), cWriteToLog, 0);
					ReportErrorMessage("TempCheckHeight: " + dtoa(TempCheckHeight,0), cWriteToLog, 0);
					TempPixelLoop = TempCheckHeight;
					TempLineLoop = TempStopPoint;
				}
			}
			//if more than enough of the pixels are darker , must be a container line
			if (TempContainerPixelCount >= TempCheckRequired)
			if (*TempRight > TempLineLoop)
			{
				*TempRight = TempLineLoop + 1;
				//set loop variable so will exit
				TempLineLoop = 1;
			}
		}
		if ((*TempRight == 0) || (*TempRight >= vOriginalBufferSizeY))
		{
			*TempRight = vOriginalBufferSizeY - 1;
		}

		//find left side of container
		bool TempFoundLeftEdge = false;
		if ((*TempRight > 5) && (*TempRight < vOriginalBufferSizeY - 2))
		{
			if ((vGlobalScanTracType == cAtempoScanTrac) || (vGlobalScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) || (vGlobalScanTracType == cCaseInspectorScanTrac))
			{ //look for the left edge from the left side as boxes and many packages have several features or parts so must assume only one package in the image
				WORD TempStopPoint = *TempRight - 12;
				bool TempFoundBackground = false;
				for (int TempLineLoop = 0; TempLineLoop < TempStopPoint; TempLineLoop++)
				{
					TempPointer = TempImage + (TempLineLoop * vOriginalBufferSizeX) + TempCheckBottom;
					WORD TempContainerPixelCount = 0;
					for (WORD TempPixelLoop = 0; TempPixelLoop < TempCheckHeight; TempPixelLoop++)
					{
						if (TempPointer < TempMaximumImagePointer)
						{
							//2010 05 03 need white to decide gap between containers when sizing
							//if (253 < *TempPointer++)
							if (*TempPointer++ < TempBackgroundValue[TempPixelLoop])
								TempContainerPixelCount++;  //found a pixel brighter than container by enough
						}
						else
						{
							ReportErrorMessage("Error-Brian ITIPCDig #6", cWriteToLog, 0);
							ReportErrorMessage("TempLineLoop: " + dtoa(TempLineLoop,0), cWriteToLog, 0);
							ReportErrorMessage("TempPixelLoop: " + dtoa(TempPixelLoop,0), cWriteToLog, 0);
							ReportErrorMessage("TempLeft: " + dtoa(*TempLeft,0), cWriteToLog, 0);
							ReportErrorMessage("TempRight: " + dtoa(*TempRight,0), cWriteToLog, 0);
							ReportErrorMessage("TempStopPoint: " + dtoa(TempStopPoint,0), cWriteToLog, 0);
							ReportErrorMessage("TempCheckBottom: " + dtoa(TempCheckBottom,0), cWriteToLog, 0);
							ReportErrorMessage("TempCheckHeight: " + dtoa(TempCheckHeight,0), cWriteToLog, 0);
							TempPixelLoop = TempCheckHeight;
							TempLineLoop = 1;
						}
					}
					//if fewer of the pixels are dark enough, must be a background line
					//2010 05 03 need white to decide gap between containers when sizing
					if (TempContainerPixelCount == 0)
						//If no pixels are part of the container, then must all be background, so white before container container
						TempFoundBackground = true;
					else
					if (TempContainerPixelCount >= TempCheckRequired)
					if (TempFoundBackground)
					{
						*TempLeft = TempLineLoop;
						TempFoundLeftEdge = true;
						//set loop variable so will exit
						TempLineLoop = TempStopPoint;
					}
				}
			}
			else //solo type ScanTrac
			{ //keep searching from the right to the left to find the right most container of many containers on the screen.
				WORD TempStartPoint = *TempRight - 12;
				for (int TempLineLoop = TempStartPoint; TempLineLoop > 0; TempLineLoop--)
				{
					TempPointer = TempImage + (TempLineLoop * vOriginalBufferSizeX) + TempCheckBottom;
					WORD TempContainerPixelCount = 0;
					for (WORD TempPixelLoop = 0; TempPixelLoop < TempCheckHeight; TempPixelLoop++)
					{
						if (TempPointer < TempMaximumImagePointer)
						{
							//2010 05 03 need white to decide gap between containers when sizing
							//if (253 < *TempPointer++)
							if (*TempPointer++ < TempBackgroundValue[TempPixelLoop])
								TempContainerPixelCount++;  //found a pixel brighter than container by enough
						}
						else
						{
							ReportErrorMessage("Error-Brian ITIPCDig #6", cWriteToLog, 0);
							ReportErrorMessage("TempLineLoop: " + dtoa(TempLineLoop,0), cWriteToLog, 0);
							ReportErrorMessage("TempPixelLoop: " + dtoa(TempPixelLoop,0), cWriteToLog, 0);
							ReportErrorMessage("TempLeft: " + dtoa(*TempLeft,0), cWriteToLog, 0);
							ReportErrorMessage("TempRight: " + dtoa(*TempRight,0), cWriteToLog, 0);
							ReportErrorMessage("TempStopPoint: " + dtoa(TempStopPoint,0), cWriteToLog, 0);
							ReportErrorMessage("TempCheckBottom: " + dtoa(TempCheckBottom,0), cWriteToLog, 0);
							ReportErrorMessage("TempCheckHeight: " + dtoa(TempCheckHeight,0), cWriteToLog, 0);
							TempPixelLoop = TempCheckHeight;
							TempLineLoop = 1;
						}
					}
					//if fewer of the pixels are dark enough, must be a background line
					//2010 05 03 need white to decide gap between containers when sizing
					if (TempContainerPixelCount < 2)

					//If no pixels are part of the container, then must all be background, so found left edge of container
					//if (TempContainerPixelCount == 0)
					{
						*TempLeft = TempLineLoop + 1;
						TempFoundLeftEdge = true;
						//set loop variable so will exit
						TempLineLoop = 1;
					}
				}
			}
			if (!TempFoundLeftEdge)
			{
				*TempLeft = 1;
			}
		}

		if (*TempRight >= vOriginalBufferSizeY)
			TempStopPoint = vOriginalBufferSizeY - 1;
		//find top and bottom of container
		if ((*TempRight) && (*TempLeft) && (*TempRight < vOriginalBufferSizeY - 1) && (*TempLeft < *TempRight) && (*TempRight - *TempLeft > 11))
		{
			WORD TempStartOfBottomPurpleLine = *TempLeft; //for side views that don't adjust the bottom purple line, look from left to right for top
			WORD TempEndOfBottomPurpleLine = *TempRight;
						
			if ((vDoBottomReferencing) && (vGlobalCurrentProduct->vBottomLocationLengthPixel)) //for top downs and inverted solo, look at bottom purple line
			{
				TempStartOfBottomPurpleLine = *TempLeft + vGlobalCurrentProduct->vBottomLocationLeftPixel;
				TempEndOfBottomPurpleLine = *TempLeft + vGlobalCurrentProduct->vBottomLocationLeftPixel + vGlobalCurrentProduct->vBottomLocationLengthPixel;
			}

			if (TempStartOfBottomPurpleLine >= vOriginalBufferSizeY - 1)
				TempStartOfBottomPurpleLine = vOriginalBufferSizeY - 2;

			if (TempEndOfBottomPurpleLine > *TempRight)
				TempEndOfBottomPurpleLine = *TempRight;
			if (TempEndOfBottomPurpleLine >= vOriginalBufferSizeY)
				TempEndOfBottomPurpleLine = vOriginalBufferSizeY - 1;

			TempCheckRequired = (WORD)((TempEndOfBottomPurpleLine - TempStartOfBottomPurpleLine) * 0.1);
			if (TempCheckRequired < 2)
				TempCheckRequired = 2;

			if (vGlobalScanTracType == cUpSideDownSoloScanTrac)
			if (TempCheckRequired > 4)
				TempCheckRequired = 4;

			//find top of container
			for (int TempLineLoop = vOriginalBufferSizeX; TempLineLoop > 10; TempLineLoop--)
			{
				TempPointer = TempImage + TempLineLoop - 1 + (TempStartOfBottomPurpleLine * vOriginalBufferSizeX); 
				BYTE *TempEdgePixelPointer = TempImage + TempLineLoop - 1; 

				WORD TempContainerPixelCount = 0;
				for (WORD TempPixelLoop = TempStartOfBottomPurpleLine; TempPixelLoop < TempEndOfBottomPurpleLine; TempPixelLoop++)
				{	//go across this line and add up how many dark pixels there are
					if (TempPointer < TempMaximumImagePointer)
					{
						BYTE TempEdgeValue = 0;
						if (*TempEdgePixelPointer > 20)
							TempEdgeValue = *TempEdgePixelPointer - 20;
						if (*TempPointer < TempEdgeValue)
							TempContainerPixelCount++;//found a pixel darker than the background by enough
						TempPointer = TempPointer + vOriginalBufferSizeX;
					}
					else
					{
						ReportErrorMessage("Error-Brian ITIPCDig #8", cWriteToLog, 0);
						ReportErrorMessage("TempLineLoop: " + dtoa(TempLineLoop,0), cWriteToLog, 0);
						ReportErrorMessage("TempPixelLoop: " + dtoa(TempPixelLoop,0), cWriteToLog, 0);
						ReportErrorMessage("TempLeft: " + dtoa(*TempLeft,0), cWriteToLog, 0);
						ReportErrorMessage("TempRight: " + dtoa(*TempRight,0), cWriteToLog, 0);
						ReportErrorMessage("TempStopPoint: " + dtoa(TempStopPoint,0), cWriteToLog, 0);
						ReportErrorMessage("TempCheckBottom: " + dtoa(TempCheckBottom,0), cWriteToLog, 0);
						ReportErrorMessage("TempCheckHeight: " + dtoa(TempCheckHeight,0), cWriteToLog, 0);
						TempPixelLoop = TempStopPoint;
						TempLineLoop = 1;
					}
				}
				//if some pixels are dark, must be a container line
				if (TempContainerPixelCount > TempCheckRequired)
				{
					*TempTop = TempLineLoop;
					//set loop variable so will exit
					TempLineLoop = 1;
				}
			}
			//find Bottom of container
			if (vDoBottomReferencing)
			for (int TempLineLoop = 5; TempLineLoop < *TempTop; TempLineLoop++)
			{
				TempPointer = TempImage + TempLineLoop + (TempStartOfBottomPurpleLine * vOriginalBufferSizeX); 
				BYTE *TempEdgePixelPointer = TempImage + TempLineLoop; 

				WORD TempContainerPixelCount = 0;
				for (WORD TempPixelLoop = TempStartOfBottomPurpleLine; TempPixelLoop < TempEndOfBottomPurpleLine; TempPixelLoop++)
				{	//go across this line and add up how many dark pixels there are
					if (TempPointer < TempMaximumImagePointer)
					{
						BYTE TempEdgeValue = 0;
						if (*TempEdgePixelPointer > 20)
							TempEdgeValue = *TempEdgePixelPointer - 20;
						if (*TempPointer < TempEdgeValue)
							TempContainerPixelCount++;//found a pixel darker than the background by enough
						TempPointer = TempPointer + vOriginalBufferSizeX;
					}
					else
					{
						ReportErrorMessage("Error-Brian ITIPCDig #8", cWriteToLog, 0);
						ReportErrorMessage("TempLineLoop: " + dtoa(TempLineLoop,0), cWriteToLog, 0);
						ReportErrorMessage("TempPixelLoop: " + dtoa(TempPixelLoop,0), cWriteToLog, 0);
						ReportErrorMessage("TempLeft: " + dtoa(*TempLeft,0), cWriteToLog, 0);
						ReportErrorMessage("TempRight: " + dtoa(*TempRight,0), cWriteToLog, 0);
						ReportErrorMessage("TempStopPoint: " + dtoa(TempStopPoint,0), cWriteToLog, 0);
						ReportErrorMessage("TempCheckBottom: " + dtoa(TempCheckBottom,0), cWriteToLog, 0);
						ReportErrorMessage("TempCheckHeight: " + dtoa(TempCheckHeight,0), cWriteToLog, 0);
						TempPixelLoop = TempStopPoint;
						TempLineLoop = 1;
					}
				}
				//if some pixels are dark, must be a container line
				if (TempContainerPixelCount > TempCheckRequired)
				{
					*TempBottom = TempLineLoop;
					//set loop variable so will exit
					TempLineLoop = *TempTop;
				}
			}
		}
		else
		{
			*TempTop = vOriginalBufferSizeX - 2;
		}
	}
}//end of finding edges FindContainerEdgesWithRelativeThreshold

void CITIPCDig::FindContainerEdges(BYTE *TempImage, WORD *TempTop, WORD *TempBottom, WORD *TempRight, WORD *TempLeft)
{  //TempImage is the thresholded image
	//look for container Right edge
	if (TempImage)
	if (vGlobalCurrentProduct)
	{
		WORD TempCheckBottom = 0;
		WORD TempCheckHeight = 0;
		if (vGlobalCurrentProduct->vEdgeLocationHeightPixel)
		{
			TempCheckBottom = vGlobalCurrentProduct->vEdgeLocationBottomPixel;
			TempCheckHeight = vGlobalCurrentProduct->vEdgeLocationHeightPixel;
		}
		else
		{
			TempCheckBottom = (WORD)((double)vOriginalBufferSizeX * .1);
			TempCheckHeight = (WORD)((double)vOriginalBufferSizeX * .5);
		}

		if (TempCheckBottom == 0)
			TempCheckBottom = 1;

		if (TempCheckBottom > (WORD)((double)vOriginalBufferSizeX * .9))
			TempCheckBottom = (WORD)((double)vOriginalBufferSizeX * .9);

		if (TempCheckHeight + TempCheckBottom >= vOriginalBufferSizeX)
			TempCheckHeight = vOriginalBufferSizeX - TempCheckBottom - 1;

		WORD TempCheckRequired = TempCheckHeight / 4;
		if (TempCheckRequired < 2)
			TempCheckRequired = 2;

		if (vGlobalScanTracType == cUpSideDownSoloScanTrac)
		if (TempCheckRequired > 4)
			TempCheckRequired = 4;

		//only search from right to 40% of image width
		WORD TempMidPoint = (WORD)((double)(vOriginalBufferSizeY) * 0.4); //5;  //start at right and move left looking for container until this line

		if (vGlobalInAutoSize)  //if in auto size, look across entire image
			TempMidPoint = 5;

		BYTE *TempMaximumPointer = TempImage + vOriginalBufferSize;
		BYTE *TempPointer;
		for (WORD TempLineLoop = vOriginalBufferSizeY - 1; TempLineLoop > TempMidPoint; TempLineLoop--)
		{
			TempPointer = TempImage + (TempLineLoop * vOriginalBufferSizeX) + TempCheckBottom;
			WORD TempContainerPixelCount = 0;
			for (WORD TempPixelLoop = 0; TempPixelLoop < TempCheckHeight; TempPixelLoop++)
			if (TempPointer < TempMaximumPointer)
			{
				if (*TempPointer++ == 0)
					TempContainerPixelCount++;
			}
			//if more than half the pixels are dark, must be a container line
			if (TempContainerPixelCount >= TempCheckRequired)
			{
				*TempRight = TempLineLoop + 1;
				//set loop variable so will exit
				TempLineLoop = 1;
			}
		}
		if ((*TempRight == 0) || (*TempRight > vOriginalBufferSizeY))
		{
			*TempRight = vOriginalBufferSizeY - 1;
			//ReportErrorMessage("Error-101", cEMailInspx, 32000);
		}
			
		//find left side of container
		bool TempFoundLeftEdge = false;
		if (*TempRight > 5)
			TempMidPoint = *TempRight - 3;
		else
			TempMidPoint = vOriginalBufferSizeY - 3;
		for (WORD TempLineLoop = 0; TempLineLoop < TempMidPoint; TempLineLoop++)
		{
			TempPointer = TempImage + (TempLineLoop * vOriginalBufferSizeX) + TempCheckBottom;
			WORD TempContainerPixelCount = 0;
			for (WORD TempPixelLoop = 0; TempPixelLoop < TempCheckHeight; TempPixelLoop++)
			if (TempPointer < TempMaximumPointer)
			{
				if (*TempPointer++ == 0)
					TempContainerPixelCount++;
			}
			//if more than half the pixels are dark, must be a container line
			if (TempContainerPixelCount >= TempCheckRequired)
			{
				*TempLeft = TempLineLoop;
				TempFoundLeftEdge = true;
				//set loop variable so will exit
				TempLineLoop = TempMidPoint;
			}
		}
		if (!TempFoundLeftEdge)
			*TempLeft = 0;

		//find top and bottom of container
		if (*TempRight)
		if (*TempLeft < *TempRight)
		if (*TempRight - *TempLeft > 11)
		{
			WORD TempStartOfBottomPurpleLine = *TempLeft; //for side views that don't adjust the bottom purple line, look from left to right for top
			WORD TempEndOfBottomPurpleLine = *TempRight;
						
			if ((vDoBottomReferencing) && (vGlobalCurrentProduct->vBottomLocationLengthPixel)) //for top downs and inverted solo, look at bottom purple line
			{
				TempStartOfBottomPurpleLine = *TempLeft + vGlobalCurrentProduct->vBottomLocationLeftPixel;
				TempEndOfBottomPurpleLine = *TempLeft + vGlobalCurrentProduct->vBottomLocationLeftPixel + vGlobalCurrentProduct->vBottomLocationLengthPixel;
			}

			if (TempStartOfBottomPurpleLine >= vOriginalBufferSizeY - 1)
				TempStartOfBottomPurpleLine = vOriginalBufferSizeY - 2;

			if (TempEndOfBottomPurpleLine > *TempRight)
				TempEndOfBottomPurpleLine = *TempRight;
			if (TempEndOfBottomPurpleLine >= vOriginalBufferSizeY)
				TempEndOfBottomPurpleLine = vOriginalBufferSizeY - 1;

			TempCheckRequired = (WORD)((TempEndOfBottomPurpleLine - TempStartOfBottomPurpleLine) * 0.1);
			if (TempCheckRequired < 2)
				TempCheckRequired = 2;

			if (vGlobalScanTracType == cUpSideDownSoloScanTrac)
			if (TempCheckRequired > 4)
				TempCheckRequired = 4;

			//find top of container
			for (int TempLineLoop = vOriginalBufferSizeX; TempLineLoop > 1; TempLineLoop--)
			{
				TempPointer = TempImage + TempLineLoop - 1 + (TempStartOfBottomPurpleLine * vOriginalBufferSizeX); 
				WORD TempContainerPixelCount = 0;

				for (WORD TempPixelLoop = TempStartOfBottomPurpleLine; TempPixelLoop < TempEndOfBottomPurpleLine; TempPixelLoop++)
				{
					//go across this line and see how many dark pixels there are
					if (*TempPointer == 0)
						TempContainerPixelCount++;
					TempPointer = TempPointer + vOriginalBufferSizeX;
				}
				//if some pixels are dark, must be a container line
				if (TempContainerPixelCount > TempCheckRequired)
				{
					*TempTop = TempLineLoop;
					//set loop variable so will exit
					TempLineLoop = 1;
				}
			}
			//find Bottom of container
			if (vDoBottomReferencing)
			for (WORD TempLineLoop = 5; TempLineLoop < *TempTop; TempLineLoop++)
			{
				TempPointer = TempImage + TempLineLoop + (TempStartOfBottomPurpleLine * vOriginalBufferSizeX); 
				WORD TempContainerPixelCount = 0;
				for (WORD TempPixelLoop = TempStartOfBottomPurpleLine; TempPixelLoop < TempEndOfBottomPurpleLine; TempPixelLoop++)
				{
					//go across this line and see how many dark pixels there are
					if (*TempPointer == 0)
						TempContainerPixelCount++;
					TempPointer = TempPointer + vOriginalBufferSizeX;
				}
				//if some pixels are dark, must be a container line
				if (TempContainerPixelCount > TempCheckRequired)
				{
					*TempBottom = TempLineLoop;
					//set loop variable so will exit
					TempLineLoop = *TempTop;
				}
			}
		}
		else
			*TempTop = vOriginalBufferSizeX;

		//ReportErrorMessage("FindContainerEdges Right: " + dtoa(*TempRight,0) + ", Left: " +
		//	dtoa(*TempLeft,0), cWriteToLog,32000);
	}

	// temp for test
	//ReportErrorMessage("Find Edges Right:" + dtoa(*TempRight, 0), cWriteToLog,0);
	//ReportErrorMessage("Find Edges Left:" + dtoa(*TempLeft, 0), cWriteToLog,0);
}//end of finding edges

void CITIPCDig::FindContainerEdgesCheckingFromCenter(BYTE *TempImage, WORD *TempTop, WORD *TempBottom, WORD *TempRight, WORD *TempLeft)
{  //TempImage is the thresholded image
	//look for container left edge
	*TempLeft = 0;
	*TempBottom = 0;
	*TempRight = vOriginalBufferSizeY - 1;
	*TempTop = vOriginalBufferSizeX - 1;

	if (TempImage)
	if (vGlobalCurrentProduct)
	{
		WORD TempCheckBottom = 0;
		WORD TempCheckHeight = 0;
		if (vGlobalCurrentProduct->vEdgeLocationHeightPixel)
		{
			TempCheckBottom = vGlobalCurrentProduct->vEdgeLocationBottomPixel;
			TempCheckHeight = vGlobalCurrentProduct->vEdgeLocationHeightPixel;
		}
		else
		{
			TempCheckBottom = (WORD)((double)vOriginalBufferSizeX * .1);
			TempCheckHeight = (WORD)((double)vOriginalBufferSizeX * .5);
		}
		if (TempCheckBottom == 0)
			TempCheckBottom = 1;

		if (TempCheckBottom > (WORD)((double)vOriginalBufferSizeX * .9))
			TempCheckBottom = (WORD)((double)vOriginalBufferSizeX * .9);

		if (TempCheckHeight + TempCheckBottom >= vOriginalBufferSizeX)
			TempCheckHeight = vOriginalBufferSizeX - TempCheckBottom - 1;

		WORD TempCheckRequired = TempCheckHeight;

		if (TempCheckRequired > 3)
			TempCheckRequired = TempCheckRequired - 1;
		if (TempCheckRequired > 10)
			TempCheckRequired = TempCheckRequired - 1;

		if (vGlobalScanTracType == cUpSideDownSoloScanTrac)
		if (TempCheckRequired > 4)
			TempCheckRequired = 4;

		WORD TempMidPoint = (WORD)((double)(vOriginalBufferSizeY) * 0.4);  //start at right and move left looking for container until this line

		BYTE *TempMaximumPointer = TempImage + vOriginalBufferSize;
		BYTE *TempPointer;
		BYTE TempStartCenter = vOriginalBufferSizeY / 2;
		for (int TempLineLoop = TempStartCenter; TempLineLoop > 0; TempLineLoop--)
		{
			TempPointer = TempImage + (TempLineLoop * vOriginalBufferSizeX) + TempCheckBottom;
			WORD TempBackgroundPixelCount = 0;
			for (WORD TempPixelLoop = 0; TempPixelLoop < TempCheckHeight; TempPixelLoop++)
			if (TempPointer < TempMaximumPointer)
			{
				if (*TempPointer++ != 0) //Image was Thresholded, so container is 0, background is 1
					TempBackgroundPixelCount++;
			}
			//if more than half the pixels are bright, must not be a container line
			if (TempBackgroundPixelCount >= TempCheckRequired)
			{
				if (TempLineLoop == TempStartCenter) //first loop through is background, so can not find edges
					*TempLeft = 0;
				else
					*TempLeft = TempLineLoop + 1;
				//set loop variable so will exit
				TempLineLoop = 0;
			}
		}

		//find right side of container
		for (int TempLineLoop = TempStartCenter; TempLineLoop < vOriginalBufferSizeY; TempLineLoop++)
		{
			TempPointer = TempImage + (TempLineLoop * vOriginalBufferSizeX) + TempCheckBottom;
			WORD TempBackgroundPixelCount = 0;
			for (WORD TempPixelLoop = 0; TempPixelLoop < TempCheckHeight; TempPixelLoop++)
			if (TempPointer < TempMaximumPointer)
			{
				if (*TempPointer++ != 0)
					TempBackgroundPixelCount++;
			}
			//if more than half the pixels are bright, must not be a container line
			if (TempBackgroundPixelCount >= TempCheckRequired)
			{
				if (TempLineLoop == TempStartCenter) //first loop through is background, so can not find edges
					*TempRight = 0;
				else
					*TempRight = TempLineLoop;
					//set loop variable so will exit
				TempLineLoop = vOriginalBufferSizeY;
			}
		}

		if ((*TempRight == 0) || (*TempRight >= vOriginalBufferSizeY))
		{
			*TempRight = vOriginalBufferSizeY - 1;
			//ReportErrorMessage("Error-101", cEMailInspx, 32000);
		}
			
		//find top and bottom of container
		if (*TempRight)
		if (*TempLeft < *TempRight)
		if (*TempRight - *TempLeft > 11)
		{
			WORD TempStartOfBottomPurpleLine = *TempLeft; //for side views that don't adjust the bottom purple line, look from left to right for top
			WORD TempEndOfBottomPurpleLine = *TempRight;
						
			if ((vDoBottomReferencing) && (vGlobalCurrentProduct->vBottomLocationLengthPixel)) //for top downs and inverted solo, look at bottom purple line
			{
				TempStartOfBottomPurpleLine = *TempLeft + vGlobalCurrentProduct->vBottomLocationLeftPixel;
				TempEndOfBottomPurpleLine = *TempLeft + vGlobalCurrentProduct->vBottomLocationLeftPixel + vGlobalCurrentProduct->vBottomLocationLengthPixel;
			}

			if (TempStartOfBottomPurpleLine >= vOriginalBufferSizeY - 1)
				TempStartOfBottomPurpleLine = vOriginalBufferSizeY - 2;

			if (TempEndOfBottomPurpleLine > *TempRight)
				TempEndOfBottomPurpleLine = *TempRight;
			if (TempEndOfBottomPurpleLine >= vOriginalBufferSizeY)
				TempEndOfBottomPurpleLine = vOriginalBufferSizeY - 1;

			TempCheckRequired = (WORD)((TempEndOfBottomPurpleLine - TempStartOfBottomPurpleLine) * 0.1);
			if (TempCheckRequired < 2)
				TempCheckRequired = 2;

			if (vGlobalScanTracType == cUpSideDownSoloScanTrac)
			if (TempCheckRequired > 4)
				TempCheckRequired = 4;

			//find top of container
			for (WORD TempLineLoop = vOriginalBufferSizeX -1; TempLineLoop > 1; TempLineLoop--)
			{
				TempPointer = TempImage + TempLineLoop - 1 + (TempStartOfBottomPurpleLine * vOriginalBufferSizeX); 
				WORD TempContainerPixelCount = 0;
				for (WORD TempPixelLoop = TempStartOfBottomPurpleLine; TempPixelLoop < TempEndOfBottomPurpleLine; TempPixelLoop++)
				{
					//go across this line and see how many dark pixels there are
					if (*TempPointer == 0)
						TempContainerPixelCount++;
					TempPointer = TempPointer + vOriginalBufferSizeX;
				}
				//if some pixels are dark, must be a container line
				if (TempContainerPixelCount > TempCheckRequired)
				{
					*TempTop = TempLineLoop;
					//set loop variable so will exit
					TempLineLoop = 1;
				}
			}
			//find Bottom of container
			if (vDoBottomReferencing)
			for (WORD TempLineLoop = 5; TempLineLoop < *TempTop; TempLineLoop++)
			{
				TempPointer = TempImage + TempLineLoop + (TempStartOfBottomPurpleLine * vOriginalBufferSizeX); 
				WORD TempContainerPixelCount = 0;
				for (WORD TempPixelLoop = TempStartOfBottomPurpleLine; TempPixelLoop < TempEndOfBottomPurpleLine; TempPixelLoop++)
				{
					//go across this line and see how many dark pixels there are
					if (*TempPointer == 0)
						TempContainerPixelCount++;
					TempPointer = TempPointer + vOriginalBufferSizeX;
				}
				//if some pixels are dark, must be a container line
				if (TempContainerPixelCount > TempCheckRequired)
				{
					*TempBottom = TempLineLoop;
					//set loop variable so will exit
					TempLineLoop = *TempTop;
				}
			}
		}
		else
			*TempTop = vOriginalBufferSizeX;

		//ReportErrorMessage("FindContainerEdges Right: " + dtoa(*TempRight,0) + ", Left: " +
		//	dtoa(*TempLeft,0), cWriteToLog,32000);
	}
}//end of finding edges

void CITIPCDig::ContainerTooWideLookForEdgesAgain(Image *TempImage, WORD *TempTop, WORD *TempBottom, WORD *TempRight, WORD *TempLeft)
{
	if (*TempRight > vOriginalBufferSizeY)
		*TempRight = vOriginalBufferSizeY;
	if (*TempLeft >= *TempRight)
		*TempLeft = 0;

	if ((TempImage) && (TempImage->pix))
	if (vGlobalCurrentProduct)
	if (vGlobalCurrentProduct->vReferenceWidth)
	{
		bool TempTooWide = true;
		bool TempCheckedRight = false;
		bool TempCheckedLeft = false;
		while (TempTooWide)
		{
			int TempDistanceFromRight = vGlobalCurrentProduct->vImageWidthPosition - *TempRight;
			int TempDistanceFromLeft = *TempLeft;
			bool TempMovedRightIn = false;
			bool TempMovedLeftIn = false;
			WORD TempCheckHeight = vGlobalCurrentProduct->vEdgeLocationHeightPixel;

			WORD TempCheckRequired = TempCheckHeight / 4;
			if (TempCheckRequired < 2)
				TempCheckRequired = 2;

			if (vGlobalScanTracType == cUpSideDownSoloScanTrac)
			if (TempCheckRequired > 4)
				TempCheckRequired = 4;

			//look at bringing in just the edge that is closest to the edge of the image
			if (((TempDistanceFromRight < TempDistanceFromLeft) || (TempCheckedLeft)) &&
				(!TempCheckedRight))
			{
				//look look at bringing in Right edge
				TempCheckedRight = true;
				WORD TempCheckBottom = 10;
				WORD TempCheckHeight = *TempTop;
				if (vGlobalCurrentProduct->vEdgeLocationHeightPixel)
				{
					TempCheckBottom = vGlobalCurrentProduct->vEdgeLocationBottomPixel;
					TempCheckHeight = vGlobalCurrentProduct->vEdgeLocationHeightPixel;
				}
				if (TempCheckBottom == 0)
					TempCheckBottom = 1;

				if (TempCheckBottom > (WORD)((double)vOriginalBufferSizeX * .9))
					TempCheckBottom = (WORD)((double)vOriginalBufferSizeX * .9);

				if (TempCheckHeight + TempCheckBottom >= vOriginalBufferSizeX)
					TempCheckHeight = vOriginalBufferSizeX - TempCheckBottom - 1;

				BYTE *TempMaximumPointer = TempImage->pix + vOriginalBufferSize;

				BYTE *TempPointer;
				WORD TempStart = *TempRight - 1;
				//WORD TempStop = vOriginalBufferSizeY - vMaximumImageOverlap - 1;
				WORD TempStop = *TempLeft + vGlobalCurrentProduct->vReferenceWidth - (2 * cWidthTolerance);
				bool TempFoundBackGround = false;
				if (TempStop <= *TempLeft)
					TempStop = *TempLeft + 2;
				for (WORD TempLineLoop = TempStart; TempLineLoop > TempStop; TempLineLoop--)
				{
					TempPointer = TempImage->pix + (TempLineLoop * vOriginalBufferSizeX) + TempCheckBottom;
					WORD TempContainerPixelCount = 0;
					for (WORD TempPixelLoop = 0; TempPixelLoop < TempCheckHeight; TempPixelLoop++)
					if (TempPointer < TempMaximumPointer)
					{
						if (*TempPointer++ == 255)  //if 255, then not container
							TempContainerPixelCount++;
					}
					//if less than x of y pixels are dark, must be a background line

					//2010 05 03 need white to decide gap between containers when sizing
					if (TempContainerPixelCount >= TempCheckHeight)
						TempFoundBackGround = true;
					else
					{
						if (TempFoundBackGround)
						{
							//found a background line sooner than extrema, so use it as the new right side
							*TempRight = TempLineLoop + 1;
							//set loop variable so will exit
							TempLineLoop = TempStop;
							TempMovedRightIn = true;
							TempCheckedRight = false;
							TempCheckedLeft = false;
						}
					}//end found a background line
				}
			}
			else //look at bringing in left edge
			{
				//if (vLocalSystemData->vLogFile.vLogSerialData)
				//	vLocalSystemData->vLogFile.WriteToLogFile("Container near Left Edge",cDebugMessage);
				//look for container Left edge
				TempCheckedLeft = true;
				WORD TempCheckBottom = 10;
				WORD TempCheckHeight = *TempTop;
				if (vGlobalCurrentProduct->vEdgeLocationHeightPixel)
				{
					TempCheckBottom = vGlobalCurrentProduct->vEdgeLocationBottomPixel;
					TempCheckHeight = vGlobalCurrentProduct->vEdgeLocationHeightPixel;
				}
				if (TempCheckBottom == 0)
					TempCheckBottom = 1;

				if (TempCheckBottom > (WORD)((double)vOriginalBufferSizeX * .9))
					TempCheckBottom = (WORD)((double)vOriginalBufferSizeX * .9);

				if (TempCheckHeight + TempCheckBottom >= vOriginalBufferSizeX)
					TempCheckHeight = vOriginalBufferSizeX - TempCheckBottom - 1;

				BYTE *TempPointer;
				bool TempFoundBackGround = false;
				WORD TempStart = *TempLeft + 1;
				//WORD TempStop = vMaximumImageOverlap;
				WORD TempStop = *TempRight - vGlobalCurrentProduct->vReferenceWidth + (2 * cWidthTolerance);
				BYTE *TempMaximumPointer = TempImage->pix + vOriginalBufferSize;

				if (TempStop >= vOriginalBufferSizeY)
					TempStop = vOriginalBufferSizeY;
				if (TempStart < TempStop)
				for (int TempLineLoop = TempStart; TempLineLoop < TempStop; TempLineLoop++)
				{
					TempPointer = TempImage->pix + (TempLineLoop * vOriginalBufferSizeX) + TempCheckBottom;
					WORD TempContainerPixelCount = 0;
					for (WORD TempPixelLoop = 0; TempPixelLoop < TempCheckHeight; TempPixelLoop++)
					if (TempPointer < TempMaximumPointer)
					{
							//crashed here 8/23/2006
						if (*TempPointer++ == 255)  //if 255, then not container
							TempContainerPixelCount++;
					}
					//if less than x of the y pixels are dark, must be a background line
					//2010 05 03 need white to decide gap between containers when sizing
					if (TempContainerPixelCount >= TempCheckHeight)
						TempFoundBackGround = true;
					else
					{
						if (TempFoundBackGround)
						{
							*TempLeft = TempLineLoop;
							TempLineLoop = TempStop;
							TempMovedLeftIn = true;
							TempCheckedRight = false;
							TempCheckedLeft = false;
						}
					}
				}
			}
			if ((!TempMovedRightIn) && (!TempMovedLeftIn) && (TempCheckedRight) && (TempCheckedLeft))
				TempTooWide = false;
			else
			{
				double TempWidth = *TempRight - *TempLeft;
				if ((vGlobalCurrentProduct->vReferenceWidth + cWidthTolerance) > ((WORD)(TempWidth)))
					TempTooWide = false;
			}
		}
	}//end of wide container checking for right sized edges
}

void CITIPCDig::VerifyContainerEdges(Image *TempImage, WORD *TempTop, WORD *TempBottom, WORD *TempRight, WORD *TempLeft)
{
	if ((TempImage) && (TempImage->pix))
	if (*TempRight > vMaximumImageOverlap + 1)
	if (vGlobalCurrentProduct)
	{
		WORD TempCheckHeight = vGlobalCurrentProduct->vEdgeLocationHeightPixel;

		BYTE *TempMaximumPointer = TempImage->pix + vOriginalBufferSize;
		WORD TempCheckRequired = TempCheckHeight / 4;
		if (TempCheckRequired < 2)
			TempCheckRequired = 2;

		if (vGlobalScanTracType == cUpSideDownSoloScanTrac)
		if (TempCheckRequired > 4)
			TempCheckRequired = 4;

		if (*TempRight > (vAquisitionSizeY - 2))
		{
			//look for container Right edge
			WORD TempCheckBottom = 10;
			WORD TempCheckHeight = *TempTop;
			if (vGlobalCurrentProduct->vEdgeLocationHeightPixel)
			{
				TempCheckBottom = vGlobalCurrentProduct->vEdgeLocationBottomPixel;
				TempCheckHeight = vGlobalCurrentProduct->vEdgeLocationHeightPixel;
			}
			if (TempCheckBottom == 0)
				TempCheckBottom = 1;

			if (TempCheckBottom > (WORD)((double)vOriginalBufferSizeX * .9))
				TempCheckBottom = (WORD)((double)vOriginalBufferSizeX * .9);

			if (TempCheckHeight + TempCheckBottom >= vOriginalBufferSizeX)
				TempCheckHeight = vOriginalBufferSizeX - TempCheckBottom - 1;

			BYTE *TempPointer;
			WORD TempStart = *TempRight - 1;
			//WORD TempStop = *TempRight - vMaximumImageOverlap - 1;
			WORD TempStop = vOriginalBufferSizeY - vMaximumImageOverlap - 1;

			if (*TempLeft + vGlobalCurrentProduct->vReferenceWidth - 10 > TempStop)
				TempStop = *TempLeft + vGlobalCurrentProduct->vReferenceWidth - 10;

			bool TempFoundBackGround = false;
			if (TempStop <= *TempLeft)
				TempStop = *TempLeft + 2;
			for (WORD TempLineLoop = TempStart; TempLineLoop > TempStop; TempLineLoop--)
			{
				TempPointer = TempImage->pix + (TempLineLoop * vOriginalBufferSizeX) + TempCheckBottom;
				WORD TempContainerPixelCount = 0;
				for (WORD TempPixelLoop = 0; TempPixelLoop < TempCheckHeight; TempPixelLoop++)
				if (TempPointer < TempMaximumPointer)
				{
					if (*TempPointer++ == 255)  //if 255, then not container
						TempContainerPixelCount++;
				}
				//if less than x of y pixels are dark, must be a background line
				//2010 05 03 need white to decide gap between containers when sizing
				if (TempContainerPixelCount >= TempCheckHeight)
					TempFoundBackGround = true;
				else
				{
					if (TempFoundBackGround)
					{
						//found a background line sooner than extrema, so use it as the new right side
						*TempRight = TempLineLoop + 1;
						//set loop variable so will exit
						TempLineLoop = TempStop;
					}
				}//end found a background line
			}
		}
		//if (vLastLeft < vMaximumImageOverlap)
		if (*TempLeft < 2)
		{
			//if (vLocalSystemData->vLogFile.vLogSerialData)
			//	vLocalSystemData->vLogFile.WriteToLogFile("Container near Left Edge",cDebugMessage);
			//look for container Left edge
			WORD TempCheckBottom = 10;
			WORD TempCheckHeight = *TempTop;
			if (vGlobalCurrentProduct->vEdgeLocationHeightPixel)
			{
				TempCheckBottom = vGlobalCurrentProduct->vEdgeLocationBottomPixel;
				TempCheckHeight = vGlobalCurrentProduct->vEdgeLocationHeightPixel;
			}
			if (TempCheckBottom == 0)
				TempCheckBottom = 1;

			if (TempCheckBottom > (WORD)((double)vOriginalBufferSizeX * .9))
				TempCheckBottom = (WORD)((double)vOriginalBufferSizeX * .9);

			if (TempCheckHeight + TempCheckBottom >= vOriginalBufferSizeX)
				TempCheckHeight = vOriginalBufferSizeX - TempCheckBottom - 1;

			BYTE *TempPointer;
			bool TempFoundBackGround = false;
			WORD TempStart = *TempLeft + 1;
			WORD TempStop = vMaximumImageOverlap;
			for (int TempLineLoop = TempStart; TempLineLoop < TempStop; TempLineLoop++)
			{
				TempPointer = TempImage->pix + (TempLineLoop * vOriginalBufferSizeX) + TempCheckBottom;
				WORD TempContainerPixelCount = 0;
				for (WORD TempPixelLoop = 0; TempPixelLoop < TempCheckHeight; TempPixelLoop++)
				if (TempPointer < TempMaximumPointer)
				{
					if (*TempPointer++ == 255)  //if 255, then not container
						TempContainerPixelCount++;
				}
				//if less than x of the y pixels are dark, must be a background line
				//2010 05 03 need white to decide gap between containers when sizing
				if (TempContainerPixelCount >= TempCheckHeight)
					TempFoundBackGround = true;
				else
				{
					if (TempFoundBackGround)
					{
						*TempLeft = TempLineLoop;
						TempLineLoop = TempStop;
					}
				}
			}
		}
	}//end of checking for image overlap
}

int CITIPCDig::CalculateSideToSideReference(WORD TempRightReference, WORD TempLeftReference, WORD *TempRight, WORD *TempLeft)
{
	//if doing side to side (left right) reference adjustments
	int TempLeftReferenceAdjustInt = 0;
	int TempRightReferenceAdjustInt = 0;
	if (TempRightReference)
	if (((WORD)(*TempLeft) != TempLeftReference) || ((WORD)(*TempRight) != TempRightReference))
	{
		double TempLeftReferenceAdjustDouble = *TempLeft - TempLeftReference;
		if (TempLeftReferenceAdjustDouble < 0)
			TempLeftReferenceAdjustInt = (int)(TempLeftReferenceAdjustDouble - .5);
		else
			TempLeftReferenceAdjustInt = (int)(TempLeftReferenceAdjustDouble + .5);

		double TempRightReferenceAdjustDouble = *TempRight - TempRightReference;
		if (TempRightReferenceAdjustDouble < 0)
			TempRightReferenceAdjustInt = (int)(TempRightReferenceAdjustDouble - .5);
		else
			TempRightReferenceAdjustInt = (int)(TempRightReferenceAdjustDouble + .5);

		if (TempRightReferenceAdjustInt != TempLeftReferenceAdjustInt)
		{
			if (((int)*TempLeft == 0) &&
				((int)(*TempRight) != vAquisitionSizeY))
				//if container was on left wall, use the right reference
				TempLeftReferenceAdjustInt = TempLeftReferenceAdjustInt;
			else
			if (((int)(*TempRight) == vAquisitionSizeY) &&
				((int)*TempLeft != 0))
			{ //if container was on the right wall, use the left reference
				TempLeftReferenceAdjustInt = TempLeftReferenceAdjustInt;
			}
			else
			{//split the difference in the reference adjust
				double TempDouble = TempLeftReferenceAdjustDouble + TempRightReferenceAdjustDouble;
				if (TempDouble < 0)
					TempLeftReferenceAdjustInt = (int)((TempDouble / 2) - .49);
				else
					TempLeftReferenceAdjustInt = (int)((TempDouble / 2) + .49);
			}
		}
	}
	return TempLeftReferenceAdjustInt;
}

//void CITIPCDig::FillInLightSpots(BYTE *TempBufferPointer)
//{
//	if (vProductAverageImage)
//	if (vProductAverageImage->vGreyImage)
//	if (TempBufferPointer)
//	{
//		BYTE *TempImagePointer;
//		TempImagePointer = TempBufferPointer;
//		BYTE *TempAverageImagePointer;
//		TempAverageImagePointer = vProductAverageImage->vGreyImage;
//		for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
//		{
//			if (*TempAverageImagePointer < 220)
//			if (*TempImagePointer > *TempAverageImagePointer)
//				*TempImagePointer = *TempAverageImagePointer;
//			TempImagePointer++;
//			TempAverageImagePointer++;
//		}
//	}
//}
//
void CITIPCDig::PerformEnhanceOnImage(Image *TempInputImage, Image *TempScratchImage, BYTE TempEnhanceMethod)
{
	//Enhance Image
	if (TempEnhanceMethod)
	if ((TempInputImage) && (TempInputImage->pix))
	if ((TempScratchImage) && (TempScratchImage->pix))
	{
		Erflag TempError = 0;
		double TempStartFindTime = 0;
		double TempEndTime = 0;
		switch (TempEnhanceMethod)
		{
			case 1:
			//if (!vGlobalUseHalconLibrary)
			{
				TempError = mvt_sobelx(TempInputImage, TempScratchImage,OL_ABS);
				if (TempError == IM_OK) //0
					TempError = mvt_invert(TempScratchImage);
				CopyMemory(TempInputImage->pix,TempScratchImage->pix,vOriginalBufferSize);
			}
			break;
			case 2:
			//if (!vGlobalUseHalconLibrary)
			{
				TempError = mvt_sobely(TempInputImage,TempScratchImage,OL_ABS);
				if (TempError == IM_OK) //0
					TempError = mvt_invert(TempScratchImage);
				CopyMemory(TempInputImage->pix,TempScratchImage->pix,vOriginalBufferSize);
			}
			break;
			case 3:
				DoSobelXY(TempInputImage, TempScratchImage);
			break;
			case 4:
			//if (!vGlobalUseHalconLibrary)
			{
				TempError = mvt_prewittx(TempInputImage,TempScratchImage,OL_ABS);
				if (TempError == IM_OK) //0
					TempError = mvt_invert(TempScratchImage);
				CopyMemory(TempInputImage->pix,TempScratchImage->pix,vOriginalBufferSize);
			}
			break;
			case 5:
			//if (!vGlobalUseHalconLibrary)
			{
				TempError = mvt_prewitty(TempInputImage,TempScratchImage,OL_ABS);
				if (TempError == IM_OK) //0
					TempError = mvt_invert(TempScratchImage);
				CopyMemory(TempInputImage->pix,TempScratchImage->pix,vOriginalBufferSize);
			}
			break;
			case 6:
			//if (!vGlobalUseHalconLibrary)
			{
				TempError = mvt_prewittxy(TempInputImage,TempScratchImage,OL_ABS);
				if (TempError == IM_OK) //0
					TempError = mvt_invert(TempScratchImage);
				CopyMemory(TempInputImage->pix,TempScratchImage->pix,vOriginalBufferSize);
			}
			break;
			case 7:
			//if (!vGlobalUseHalconLibrary)
			{
				TempError = mvt_erode(TempInputImage,TempScratchImage);
				if (TempError == IM_OK) //0
					TempError = mvt_invert(TempScratchImage);
				CopyMemory(TempInputImage->pix,TempScratchImage->pix,vOriginalBufferSize);
			}
			break;
			case 8:
				DoConvolveImage3x3(TempInputImage);
			break;
			case 9: //convolve 5 x 5
				DoConvolveImage5x5(TempInputImage,TempScratchImage);
				CopyMemory(TempInputImage->pix,TempScratchImage->pix,vOriginalBufferSize);
			break;
			case 10:
				DoConvolveImage5x5(TempInputImage,TempScratchImage);
				CopyMemory(TempInputImage->pix,TempScratchImage->pix,vOriginalBufferSize);
				DoConvolveImage5x5(TempInputImage,TempScratchImage);
				CopyMemory(TempInputImage->pix,TempScratchImage->pix,vOriginalBufferSize);
			break;
			case 11:
				DoConvolveImage7x7(TempInputImage);
			break;
			case 12:
				DoConvolveImage3x3(TempInputImage);
				DoSobelXY(TempInputImage, TempScratchImage);
			break;
		}
		if (TempError == IM_OK) //0
		{		
		}
		else
		if (TempError == 105)
		{
			//if (vLocalSystemData->vLogFile.vLogSerialData)
			//	vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString +
			//" Error, MVT NO Lic.",cDebugMessage);
		}
		else
		if (TempError == IM_TOO_SMALL)
		{
			//if (vLocalSystemData->vLogFile.vLogSerialData)
			//	vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString +
			//" Error, MVT Image Too Small",cDebugMessage);
		}
		else
		{
			//if (vLocalSystemData->vLogFile.vLogSerialData)
			//	vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString +
			//" Error, MVT Byte Only",cDebugMessage);
		}
	}
}

bool CITIPCDig::ReadBackgroundImageFromFile(CProduct *TempProduct)
{
	bool TempRead = false;
	CString TempString1 = "=Process 0";
	TempString1 =  *TempProduct->GetProductName() + TempString1;
	if (!vUsingLocalSetupDataFile)
		TempString1 = cSetupDataDirectory + TempString1;
	else
		TempString1 = vGlobalCurrentDirectory + TempString1;
	TempString1 = TempString1 + ".BMP";
	LPTSTR TempString = TempString1.GetBuffer(TempString1.GetLength());
	int TempHeight = 0;
	int TempWidth = 0;
	int TempBitsPerPixel = 0;
	int TempType = 0;

	if (vProductBackgroundImage != NULL)
	{
		im_delete(vProductBackgroundImage);
		vProductBackgroundImage = NULL;
	}
	CFileStatus TempFileStatus;
	if (CFile::GetStatus(TempString, TempFileStatus))
	{
		CW2A TempFileNameString(TempString);			
		Erflag TempError = im_file_get_size(TempFileNameString, IM_BMP_FILE, &TempHeight, &TempWidth, &TempBitsPerPixel, &TempType);
		if (TempError == IM_OK)
		//if (((TempWidth != vOriginalBufferSizeY) ||
		//	(TempHeight != vOriginalBufferSizeX)) &&
		//	(!vGlobalDemoMode))
		//{
		//	ReportErrorMessage("Error-Process 0 Learned Image wrong size, ignored", cEMailInspx, 0);
		//}
		//else
		if (ThereIsEnoughMemory(vOriginalBufferSize + sizeof(Image), "Temp Image In"))
		{
			Image *TempImageIn = NULL;
			//TempImageIn = im_create(IM_BYTE, vOriginalBufferSizeX, 
			//	vOriginalBufferSizeY);
			TempImageIn = im_create(IM_BYTE, TempHeight, TempWidth);
			if ((TempImageIn) && (TempImageIn->pix))
			{

				TempError = im_file_read(TempImageIn, TempFileNameString, IM_AUTO_FILE);
				if (TempError == IM_OK)
				{
					if (ThereIsEnoughMemory(vOriginalBufferSizeX * 
						vOriginalBufferSizeY + sizeof(Image), "Background Image"))
						vProductBackgroundImage = im_create(IM_BYTE, vOriginalBufferSizeX, 
							vOriginalBufferSizeY);

					if ((vProductBackgroundImage) && (vProductBackgroundImage->pix))
					{
						FillMemory(vProductBackgroundImage->pix, vOriginalBufferSize,255);
						WORD TempMaxHeight = vOriginalBufferSizeX;
						if (TempMaxHeight > TempHeight)
							TempMaxHeight = TempHeight;
						for (WORD TempLoopX = 0; TempLoopX < TempMaxHeight; TempLoopX++)
						{
							WORD TempLineCount = 0;
							BYTE *TempSourcePointer = TempImageIn->pix + TempLoopX +
								(WORD)(vGlobalCurrentProduct->vProductImageHeightBottom * 
								vGlobalPixelsPerUnitInHeight);
							BYTE *TempDestinationPointer = vProductBackgroundImage->pix + TempLoopX;
							for (WORD TempLoopY = 0; TempLoopY < vOriginalBufferSizeY; TempLoopY++)
							{
								*TempDestinationPointer = *TempSourcePointer;
								TempSourcePointer = TempSourcePointer + TempHeight;
								TempDestinationPointer = TempDestinationPointer + vOriginalBufferSizeX;
								TempLineCount++;
								if (TempLineCount >= TempWidth)
								{
									//go back to the first pixel in the source line as hit end of line
									TempSourcePointer = TempImageIn->pix + TempLoopX +
										(WORD)(vGlobalCurrentProduct->vProductImageHeightBottom * 
										vGlobalPixelsPerUnitInHeight);
									TempLineCount = 0;
								}
							}
						}
						TempRead = true;
					}
				}
			}
			if (TempImageIn)
				im_delete(TempImageIn);
		}
	}
	return TempRead;
}

//new background
void CITIPCDig::MakeProductBackgroundImageCorrectionFactors()
{
	if ((vProductBackgroundImage) && (vProductBackgroundImage->pix))
	{
		double TempSum = 0;
		BYTE *TempPointer = NULL;
		for (WORD TempLoopY = 0; TempLoopY < vOriginalBufferSizeX; TempLoopY++)
		{
			TempSum = 0;
			TempPointer = vProductBackgroundImage->pix + TempLoopY;
			for (WORD TempLoopX = 0; TempLoopX < vOriginalBufferSizeY; TempLoopX++)
			{
				//add up all the lines taken for this detector pixel
				TempSum = TempSum + *TempPointer;
				TempPointer = TempPointer + vOriginalBufferSizeX;
			}
			BYTE TempResultByte = 255 -
				(BYTE)(TempSum / (double)vOriginalBufferSizeY);
			TempPointer = vProductBackgroundImage->pix + TempLoopY;
			for (WORD TempLoopX = 0; TempLoopX < vOriginalBufferSizeY; TempLoopX++)
			{
				//put the value in this pixel for every line of the image
				*TempPointer = TempResultByte;
				TempPointer = TempPointer + vOriginalBufferSizeX;
			}
		}
	}
}

void CITIPCDig::SaveBackgroundImageToFile(CProduct *TempCurrentProduct)
{
	if ((vProductBackgroundImage) && (vProductBackgroundImage->pix))
	{
		CString TempString1 = "=Process 0";
		TempString1 =  *TempCurrentProduct->GetProductName() + TempString1;
		if (!vUsingLocalSetupDataFile)
			TempString1 = cSetupDataDirectory + TempString1;
		else
			TempString1 = vGlobalCurrentDirectory + TempString1;
		TempString1 = TempString1 + ".BMP";
		int TempLength = TempString1.GetLength();
		if (TempLength > MAX_PATH)
		{
			CNoticeDialog TempNoticeDialog;
			CString TempString = "\n\n\nSetup Name more the MAX_PATH characters, setup save aborted";
			TempNoticeDialog.vNoticeText = TempString;
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
			TempLength = MAX_PATH;
		}
		else
		{
			CW2A TempFileNameString(TempString1);			
			CImgFile *TempImageFile=IfxCreateImgFile(TempFileNameString);
			if (TempImageFile) 
			{
				TempImageFile->WriteFile(vProductBackgroundImage->pix,
					vOriginalBufferSizeX,
					vOriginalBufferSizeY,
					(WORD)vBitsPerPixel);
				delete TempImageFile;
			}
		}
	}
}

void CITIPCDig::RemoveBackgroundFromImage(Image *TempImage)
{
	if ((vProductBackgroundImage) && (vProductBackgroundImage->pix))
	{
//#ifdef UseHalconLibraries
//		if (vGlobalUseHalconLibrary)
//		{
//			HByteImage TempHalconImageIn = MakeHalconImage(vProductBackgroundImage);
//			HByteImage TempHalconImageIn1 = MakeHalconImage(TempImage);
//			
//			HByteImage TempHalconImageOut = TempHalconImageIn + TempHalconImageIn1;
//			CopyFromHalconImage(TempHalconImageOut, TempImage);
//		}
//		else
//#endif
#ifdef UseIFCDLLs
			mvt_add_byte(vProductBackgroundImage,TempImage,0);
#endif
	}
}

void CITIPCDig::RenameBackgroundFileToBackup(CProduct *TempProduct)
{
	//RenameALearnFileToBackup("=Process 0", TempProduct); //background learn

	CString *TempProductName = TempProduct->GetProductName();
	CString TempString1 = "=Process 0";
	TempString1 =  *TempProductName + TempString1;
	if (!vUsingLocalSetupDataFile)
		TempString1 = cSetupDataDirectory + TempString1;
	else
		TempString1 = vGlobalCurrentDirectory + TempString1;
	TempString1 = TempString1 + ".BMP";
	LPTSTR TempString = TempString1.GetBuffer(TempString1.GetLength());

	CString TempString2 = "=Process 0";
	TempString2 =  "TempBackup" + TempString2;
	if (!vUsingLocalSetupDataFile)
		TempString2 = cSetupDataDirectory + TempString2;
	else
		TempString2 = vGlobalCurrentDirectory + TempString2;
	TempString2 = TempString2 + ".BMP";
	LPTSTR TempNewFileNameString = TempString2.GetBuffer(TempString2.GetLength());
	//instead of renaming it, copy it as quick learn does not replace it
	CFileStatus TempFileStatus;
	if (CFile::GetStatus(TempString, TempFileStatus))
	if (!CopyFile(TempString,TempNewFileNameString,false))
	{
		DWORD ErrorNow = GetLastError();		// system error codes http://msdn.microsoft.com/en-us/library/ms681381(v=vs.85)
		ReportErrorMessage("Copy File Error 1: "+ dtoa(ErrorNow) +", File: " + TempString1 + " to " + TempNewFileNameString, cEMailInspx, 32000);
	}
}

bool CITIPCDig::HaveLearnImageFile(CString TempLearnName)
{
	// does product have original Structure file?
	bool TempHaveUndoAddToLearnImage = false;
	if (vGlobalCurrentProduct)
	{
		CString TempDirectory = cSetupDataDirectory;
		if (vUsingLocalSetupDataFile)
			TempDirectory = vGlobalCurrentDirectory;

		CString *TempProductName = vGlobalCurrentProduct->GetProductName();
		CString TempString1 =  *TempProductName + TempLearnName;
		TempString1 = TempDirectory + TempString1;
		TempString1 = TempString1 + ".BMP";
		LPTSTR TempString = TempString1.GetBuffer(TempString1.GetLength());
		int TempHeight = 0;
		int TempWidth = 0;
		int TempBitsPerPixel = 0;
		int TempType = 0;

		CFileStatus TempFileStatus;
		if (CFile::GetStatus(TempString, TempFileStatus))	//if have file
			TempHaveUndoAddToLearnImage = true;
		else												//if not have file, make replace file with backup
		{
			if (TempString1.GetLength() > 4)
				TempString1.Insert(TempString1.GetLength()-4, 'B');
			TempString = TempString1.GetBuffer(TempString1.GetLength());
			CFileStatus TempFileStatus;
			if (CFile::GetStatus(TempString, TempFileStatus))
			{
				CW2A TempFileNameString(TempString);			
			
				Erflag TempError = im_file_get_size(TempFileNameString, IM_BMP_FILE, &TempHeight, &TempWidth, &TempBitsPerPixel, &TempType);
				DWORD TempImageSize = TempHeight * TempWidth;
				if (TempError == IM_OK)
				if (ThereIsEnoughMemory(TempImageSize + sizeof(Image), "Copy Product Learn Image"))
				{
					Image *TempImageIn = NULL;
					TempImageIn = im_create(IM_BYTE, TempHeight, TempWidth);
					if (TempImageIn)
					{
						TempError = im_file_read(TempImageIn, TempFileNameString, IM_BMP_FILE);
						if (TempError == IM_OK)
						{
							ReportErrorMessage("Learn file, " + TempString1 + " was missing, replacing with backup.",cEMailInspx,32000);
							//now write the image file to the new name
							if (TempString1.GetLength() > 5)
								TempString1.Delete(TempString1.GetLength()-5, 1);
							int TempLength = TempString1.GetLength();
							CW2A TempFileNameString(TempString1);			
							CImgFile *TempImageFile=IfxCreateImgFile(TempFileNameString);
							if ((TempImageFile) && (TempImageIn->pix))
							{
								TempImageFile->WriteFile(TempImageIn->pix,
									TempHeight,TempWidth,
									vBitsPerPixel);
								delete TempImageFile;
							}
						}
					}
					if (TempImageIn)
						im_delete(TempImageIn);
				}
				TempHaveUndoAddToLearnImage = true;
			}
			//else
			//	ReportErrorMessage("Learn file, " + TempString1 + " was missing, load failed.",cWriteToLog,0);
		}
	}
	return TempHaveUndoAddToLearnImage;
}

bool CITIPCDig::HaveUndoAddToLearnImage()
{
	return HaveLearnImageFile("=Process 2u");
}
	// does product have undo structure file?
/*	bool TempHaveUndoAddToLearnImage = false;
	if (vGlobalCurrentProduct)
	{
		CString TempDirectory = cSetupDataDirectory;
		if (vUsingLocalSetupDataFile)
			TempDirectory = vGlobalCurrentDirectory;

		CString *TempProductName = vGlobalCurrentProduct->GetProductName();
		CString TempString1 = "=Process 2u";
		TempString1 =  *TempProductName + TempString1;
		TempString1 = TempDirectory + TempString1;
		TempString1 = TempString1 + ".BMP";
		LPTSTR TempString = TempString1.GetBuffer(TempString1.GetLength());
		int TempHeight = 0;
		int TempWidth = 0;
		int TempBitsPerPixel = 0;
		int TempType = 0;

		CFileStatus TempFileStatus;
		if (CFile::GetStatus(TempString, TempFileStatus))
			TempHaveUndoAddToLearnImage = true;
	}
	return TempHaveUndoAddToLearnImage;
}
*/

void CITIPCDig::DeleteUndoStructureLearnFiles(CProduct *TempProduct)
{
	if (TempProduct)
	{
		CString TempProductName = *TempProduct->GetProductName();
		DeleteALearnFile("=Process 2u", TempProductName); //undo Structure learn
		DeleteALearnFile("=Process 2Tu", TempProductName); //undo Contaminant2 Structure learn
		DeleteALearnFile("=Process 2gu", TempProductName); //Undo Glass Structure learn
		if (vGlobalAllowVoidInspection)
			DeleteALearnFile("=Process 2vu", TempProductName); //Undo Void Structure learn
		//DeleteALearnFile("=Process 2Cu", TempProductName); //Undo Void Structure learn
	}
}

void CITIPCDig::UndoLastAddToLearnFile(CString TempLearnName)
{
	CString *TempProductName = vGlobalCurrentProduct->GetProductName();
	CString TempString1 = TempLearnName + "u";
	TempString1 =  *TempProductName + TempString1;
	if (!vUsingLocalSetupDataFile)
		TempString1 = cSetupDataDirectory + TempString1;
	else
		TempString1 = vGlobalCurrentDirectory + TempString1;
	TempString1 = TempString1 + ".BMP";
	LPTSTR TempString = TempString1.GetBuffer(TempString1.GetLength());

	CString TempString2 =  *TempProductName + TempLearnName;
	if (!vUsingLocalSetupDataFile)
		TempString2 = cSetupDataDirectory + TempString2;
	else
		TempString2 = vGlobalCurrentDirectory + TempString2;
	TempString2 = TempString2 + ".BMP";
	LPTSTR TempNewFileNameString = TempString2.GetBuffer(TempString2.GetLength());

	CFile TempFile;
	CFileStatus TempFileStatus;
	if (CFile::GetStatus(TempString, TempFileStatus))
	{//if you have an original file
		if (!DeleteFile(TempNewFileNameString))
			ReportErrorMessage("Failed to Delete File: " + TempString2, cEMailInspx, 32000);
		if (!CFile::GetStatus(TempNewFileNameString, TempFileStatus))
			TempFile.Rename(TempString,TempNewFileNameString);
	}
	vALearnedImageHasChanged = true;
}

void CITIPCDig::UndoLastAddToLearn()
{
	//rename structure image file
	if (vGlobalCurrentProduct)
	{
		UndoLastAddToLearnFile("=Process 2");
		UndoLastAddToLearnFile("=Process 2T");
		//UndoLastAddToLearnFile("=Process 2g");
		if (vGlobalAllowVoidInspection)
			UndoLastAddToLearnFile("=Process 2v");
				
		ReportErrorMessage("Un did Last Reject Added to Learn", cUserAction,0);

		if (vGlobalCurrentProduct->vAddToLearnCount)
			vGlobalCurrentProduct->vAddToLearnCount = vGlobalCurrentProduct->vAddToLearnCount - vNumberOfImagesInLastAddToLearn;
		if (vContainerCollectionLock.Lock())
		{
			LoadStructureImagesFromFiles(true, false);//loads normal structure and Contaminant2 structure
			vContainerCollectionLock.Unlock();
		}
		DeleteUndoStructureLearnFiles(vGlobalCurrentProduct);
		vALearnedImageHasChanged = true;

		ClearImproveLearn(false);
	}
}

void CITIPCDig::MakeUndoBackupOfAStructureFile(CString TempLearnName, CProduct *TempProduct)
{
	CString *TempProductName = TempProduct->GetProductName();
	CString TempString1 =  *TempProductName + TempLearnName;
	if (!vUsingLocalSetupDataFile)
		TempString1 = cSetupDataDirectory + TempString1;
	else
		TempString1 = vGlobalCurrentDirectory + TempString1;
	TempString1 = TempString1 + ".BMP";
	LPTSTR TempString = TempString1.GetBuffer(TempString1.GetLength());

	CString TempString2 = TempLearnName + "u";
	TempString2 =  *TempProductName + TempString2;
	if (!vUsingLocalSetupDataFile)
		TempString2 = cSetupDataDirectory + TempString2;
	else
		TempString2 = vGlobalCurrentDirectory + TempString2;
	TempString2 = TempString2 + ".BMP";
	LPTSTR TempNewFileNameString = TempString2.GetBuffer(TempString2.GetLength());

	CFileStatus TempFileStatus;
	if (CFile::GetStatus(TempString, TempFileStatus))
	{
		//copy to undo file
		if (!CopyFile(TempString,TempNewFileNameString,false))
		{
			DWORD ErrorNow = GetLastError();		// system error codes http://msdn.microsoft.com/en-us/library/ms681381(v=vs.85)
			ReportErrorMessage("Copy File Error 2: "+ dtoa(ErrorNow) +" File: " + TempString1 + " to " + TempString2, cEMailInspx, 32000);
		}
		//copy to backup undo file
		TempString2.Insert(TempString2.GetLength() - 4, 'B');
		TempNewFileNameString = TempString2.GetBuffer(TempString2.GetLength());
		if (!CopyFile(TempString,TempNewFileNameString,false))
		{
			DWORD ErrorNow = GetLastError();		// system error codes http://msdn.microsoft.com/en-us/library/ms681381(v=vs.85)
			ReportErrorMessage("Copy File Error 3: "+ dtoa(ErrorNow) +" File3: " + TempString1 + " to " + TempString2, cEMailInspx, 32000);
		}
	}
}

void CITIPCDig::MakeUndoBackupOfLearnStructureFiles(CProduct *TempProduct)
{
	MakeUndoBackupOfAStructureFile("=Process 2", TempProduct); //undo Structure learn
	MakeUndoBackupOfAStructureFile("=Process 2T", TempProduct); //undo Contaminant2 Structure learn
	//MakeUndoBackupOfAStructureFile("=Process 2g", TempProduct); //Undo Glass Structure learn
	if (vGlobalAllowVoidInspection)
		MakeUndoBackupOfAStructureFile("=Process 2v", TempProduct); //Undo Void Structure learn
}

void CITIPCDig::RestoreAnOriginalStructureFilePermanently(CString TempLearnName)
{
	CString *TempProductName = vGlobalCurrentProduct->GetProductName();
	CString TempString1 = TempLearnName + "o";
	TempString1 =  *TempProductName + TempString1;
	if (!vUsingLocalSetupDataFile)
		TempString1 = cSetupDataDirectory + TempString1;
	else
		TempString1 = vGlobalCurrentDirectory + TempString1;
	TempString1 = TempString1 + ".BMP";
	LPTSTR TempString = TempString1.GetBuffer(TempString1.GetLength());

	CString TempString2 =  *TempProductName + TempLearnName;
	if (!vUsingLocalSetupDataFile)
		TempString2 = cSetupDataDirectory + TempString2;
	else
		TempString2 = vGlobalCurrentDirectory + TempString2;
	TempString2 = TempString2 + ".BMP";
	LPTSTR TempNewFileNameString = TempString2.GetBuffer(TempString2.GetLength());

	CFile TempFile;
	CFileStatus TempFileStatus;
	if (CFile::GetStatus(TempString, TempFileStatus))
	{//if you have an original file
		if (!CopyFile(TempString,TempNewFileNameString,false))
		{
			DWORD ErrorNow = GetLastError();		// system error codes http://msdn.microsoft.com/en-us/library/ms681381(v=vs.85)
			ReportErrorMessage("Copy File Error 4: "+ dtoa(ErrorNow) +" File: " + TempString1 + " to " + TempString2, cEMailInspx, 32000);
		}
	}
	vALearnedImageHasChanged = true;
}

void CITIPCDig::RestoreOriginalStructurePermanently()
{
	//rename structure image file
	if (vGlobalCurrentProduct)
	{
		RestoreAnOriginalStructureFilePermanently("=Process 2");
		RestoreAnOriginalStructureFilePermanently("=Process 2T");
		//RestoreAnOriginalStructureFilePermanently("=Process 2g");
		if (vGlobalAllowVoidInspection)
			RestoreAnOriginalStructureFilePermanently("=Process 2v");

		vGlobalCurrentProduct->vAddToLearnCount = 0;
		if (vContainerCollectionLock.Lock())
		{
			LoadStructureImagesFromFiles(true, false); //loads normal structure and Contaminant2 structure
			vContainerCollectionLock.Unlock();
		}

		DeleteUndoStructureLearnFiles(vGlobalCurrentProduct);
		vALearnedImageHasChanged = true;
	}
}

void CITIPCDig::RestoreOriginalStructureTemporarily()
{
	if (vContainerCollectionLock.Lock())
	{
		LoadStructureImagesFromFiles(true, true);//loads normal structure and Contaminant2 structure
		vContainerCollectionLock.Unlock();
		vALearnedImageHasChanged = true;
	}
}

void CITIPCDig::RestoreToCurrentStructure()
{
	LoadStructureImagesFromFiles(true, false);//loads normal structure and Contaminant2 structure
	vALearnedImageHasChanged = true;
}

void CITIPCDig::DeleteUndoStructureImages(CProduct *TempProduct)
{
	if (TempProduct)
	{
		CString TempProductName = *TempProduct->GetProductName();
		DeleteALearnFile("=Process 2Tu", TempProductName);
		DeleteALearnFile("=Process 2gu", TempProductName);
		DeleteALearnFile("=Process 2u", TempProductName);
	}
}

/*
double CITIPCDig::CalculateStructureDensityInDriftDensityROI(BYTE TempRight)
{
	double TempDensity = 0;
	if (vGlobalCurrentProduct)
	if (vProductStructureImage)
	if (vGlobalCurrentProduct->vDriftDensityInspection)
	{
		TempDensity = CalculateDensity(vProductStructureImage->vGreyImage[TempRight], 
			vGlobalCurrentProduct->vDriftDensityInspection);
	}
	return TempDensity;
}

double CITIPCDig::CalculateStructureDensityInProduct(BYTE TempRight)
{
	double TempDensity = 0;
	if (vGlobalCurrentProduct)
	if (vProductStructureImage)
	if (vGlobalCurrentProduct->vReferenceTop)
	if (vGlobalCurrentProduct->vReferenceRight)
	{
		CInspection TempInspection;
		TempInspection.SetDefaultROI(
			vGlobalCurrentProduct->vReferenceTop[TempRight] / vGlobalPixelsPerUnitInHeight * .7,
			10 / vGlobalPixelsPerUnitInHeight,
			(vGlobalCurrentProduct->vReferenceLeft[TempRight] + 3) / vGlobalPixelsPerUnit,
			(vGlobalCurrentProduct->vReferenceRight[TempRight] - 3) / vGlobalPixelsPerUnitInHeight, 
			vGlobalCurrentProduct->vOverScanMultiplier);

		TempDensity = CalculateDensity(vProductStructureImage->vGreyImage[TempRight], 
			&TempInspection);
	}
	return TempDensity;
}
*/
double CITIPCDig::CalculateStructureDensityInDriftDensityROI(CProduct *TempProduct)
{
	double TempDensity = 0;
	if (TempProduct)
	if (!TempProduct->vDriftDensityInspection)
	{
		if (HaveLearnImageFile("=Process 2"))
			ReportErrorMessage("No Drift Density Inspection, Can't find learn quality", cEMailInspx,32000);
	}
	else
	if (TempProduct == vGlobalCurrentProduct)
	{
		if ((vProductStructureImage) && (vProductStructureImage->vGreyImage))
		{
			TempDensity = CalculateDensity(vProductStructureImage->vGreyImage, vGlobalCurrentProduct->vDriftDensityInspection, vOriginalBufferSizeX, vOriginalBufferSizeY);
		}
	}
	else
	{
		CImageData *TempProductStructureImage = NULL;

		CString TempString1 = "=Process 2";
		TempString1 =  *TempProduct->GetProductName() + TempString1;
		if (!vUsingLocalSetupDataFile)
			TempString1 = cSetupDataDirectory + TempString1;
		else
			TempString1 = vGlobalCurrentDirectory + TempString1;
		TempString1 = TempString1 + ".BMP";
		LPTSTR TempString = TempString1.GetBuffer(TempString1.GetLength());
		int TempHeight = 0;
		int TempWidth = 0;
		int TempBitsPerPixel = 0;
		int TempType = 0;

		CFileStatus TempFileStatus;
		if (CFile::GetStatus(TempString, TempFileStatus))
		{
			CW2A TempFileNameString(TempString);			
			Erflag TempError = im_file_get_size(TempFileNameString, IM_BMP_FILE, &TempHeight, &TempWidth, &TempBitsPerPixel, &TempType);

			DWORD TempSize = TempHeight * TempWidth;

			if (TempError == IM_OK)
			if ((TempWidth != (WORD)(TempProduct->vProductImageWidth * vGlobalPixelsPerUnit)) ||
				(TempHeight != (WORD)((TempProduct->vProductImageHeightTop -
				TempProduct->vProductImageHeightBottom) * vGlobalPixelsPerUnitInHeight)))
			{
				ReportErrorMessage("Learned Image wrong size, Can't find learn quality", cWriteToLog,0);
			}
			else
			if (ThereIsEnoughMemory(TempSize + sizeof(Image), "Temp Image"))
			{
				Image *TempImageIn = NULL;
				TempImageIn = im_create(IM_BYTE, TempHeight,TempWidth);
				if ((TempImageIn) && (TempImageIn->pix))
				{
					TempError = im_file_read(TempImageIn, TempFileNameString, IM_AUTO_FILE);
					if (TempError == IM_OK)
					{
						if (!TempProductStructureImage)
						if (ThereIsEnoughMemory(sizeof(CImageData), "TempProductStructureImage"))
							TempProductStructureImage = new CImageData;

						if (TempProductStructureImage)
						{
							TempProductStructureImage->MakeGreyImage(TempSize);

							if (TempProductStructureImage->vGreyImage)
								CopyMemory(
									TempProductStructureImage->vGreyImage,
									TempImageIn->pix,
									TempSize);
						}
					}
				}
				if (TempImageIn)
					im_delete(TempImageIn);
			}
		}

		if ((TempProductStructureImage) && (TempProductStructureImage->vGreyImage))
		{
			TempDensity = CalculateDensity(TempProductStructureImage->vGreyImage, 
				TempProduct->vDriftDensityInspection, TempHeight,TempWidth);

			delete TempProductStructureImage;
		}
	}
	return TempDensity;
}

double CITIPCDig::CalculateStructureDensityInProduct(CProduct *TempProduct)
{
	double TempDensity = 0;
	if (TempProduct)
	if (TempProduct->vReferenceTop)
	if (TempProduct->vReferenceRight)
	{
		WORD TempBottom = 10;
		WORD TempTop = (WORD)((double)TempProduct->vReferenceTop  * .7);

		if (TempProduct->vReferenceBottom)
			TempBottom = TempBottom + TempProduct->vReferenceBottom;

		if (TempBottom >= TempTop)
		{
			if (TempTop > 10)
				TempBottom = 10;
			else
				TempBottom = 0;
		}

		CInspection TempInspection;
		TempInspection.SetDefaultROI(TempTop, TempBottom, (TempProduct->vReferenceLeft + 3), (TempProduct->vReferenceRight - 3), TempProduct->vOverScanMultiplier);

		if (TempProduct == vGlobalCurrentProduct)
		{
			if ((vProductStructureImage) && (vProductStructureImage->vGreyImage))
			{
				TempDensity = CalculateDensity(vProductStructureImage->vGreyImage, &TempInspection, vOriginalBufferSizeX, vOriginalBufferSizeY);
			}
		}
		else
		{
			CImageData *TempProductStructureImage = NULL;

			CString TempString1 = "=Process 2";
			TempString1 =  *TempProduct->GetProductName() + TempString1;
			if (!vUsingLocalSetupDataFile)
				TempString1 = cSetupDataDirectory + TempString1;
			else
				TempString1 = vGlobalCurrentDirectory + TempString1;
			TempString1 = TempString1 + ".BMP";
			LPTSTR TempString = TempString1.GetBuffer(TempString1.GetLength());
			int TempHeight = 0;
			int TempWidth = 0;
			int TempBitsPerPixel = 0;
			int TempType = 0;

			CFileStatus TempFileStatus;
			if (CFile::GetStatus(TempString, TempFileStatus))
			{
				CW2A TempFileNameString(TempString);			
				Erflag TempError = im_file_get_size(TempFileNameString, IM_BMP_FILE, &TempHeight, &TempWidth, &TempBitsPerPixel, &TempType);
				DWORD TempSize = TempHeight * TempWidth;

				if (TempError == IM_OK)
				if ((TempWidth != (WORD)(TempProduct->vProductImageWidth * vGlobalPixelsPerUnit)) ||
					(TempHeight != (WORD)((TempProduct->vProductImageHeightTop -
					TempProduct->vProductImageHeightBottom) * vGlobalPixelsPerUnitInHeight)))
				{
					ReportErrorMessage("Learned Image wrong size, Can't find learn quality", cWriteToLog,0);
				}
				else
				if (ThereIsEnoughMemory(TempHeight * TempWidth + sizeof(Image), "Temp Image"))
				{
					Image *TempImageIn = NULL;
					TempImageIn = im_create(IM_BYTE, TempHeight, TempWidth);
					if ((TempImageIn) && (TempImageIn->pix))
					{
						TempError = im_file_read(TempImageIn, TempFileNameString, IM_AUTO_FILE);
						if (TempError == IM_OK)
						{
							if (!TempProductStructureImage)
							if (ThereIsEnoughMemory(sizeof(CImageData), "TempProductStructureImage"))
								TempProductStructureImage = new CImageData;

							if (TempProductStructureImage)
							{
								TempProductStructureImage->MakeGreyImage(TempSize);

								if (TempProductStructureImage->vGreyImage)
									CopyMemory(
										TempProductStructureImage->vGreyImage,
										TempImageIn->pix,
										TempSize);
							}
						}
					}
					if (TempImageIn)
						im_delete(TempImageIn);
				}
			}

			if ((TempProductStructureImage) && (TempProductStructureImage->vGreyImage))
			{
				TempDensity = CalculateDensity(TempProductStructureImage->vGreyImage, 
					&TempInspection, TempHeight,TempWidth);

				delete TempProductStructureImage;
			}
		}
	}
	return TempDensity;
}

double CITIPCDig::CalculateStructureDensityForInspection(CInspection *TempInspection)
{
	double TempDensity = 0;
	if (vGlobalCurrentProduct)
	if (vGlobalCurrentProduct->vReferenceTop)
	if (vGlobalCurrentProduct->vReferenceRight)
	{
		if ((vProductStructureImage) && (vProductStructureImage->vGreyImage))
		{
			TempDensity = CalculateDensity(vProductStructureImage->vGreyImage, TempInspection, vOriginalBufferSizeX, vOriginalBufferSizeY);
		}
	}
	return TempDensity;
}

void CITIPCDig::AddImageToImproveLearn(BYTE *TempImageBufferPointer, BYTE *TempScratchImagePointer)
{
	if (vGlobalCurrentProduct)
	if (!vImproveLearnStructureImage)
		ReportErrorMessage("No Improve Learn Structure, Cant Add To Improve Learn",cEMailInspx,32000);
	else
	if (!TempImageBufferPointer)
		ReportErrorMessage("Nill Pointer, Cant Add To Improve Learn",cEMailInspx,32000);
	else
	if (!ThereIsEnoughMemory(vOriginalBufferSize, "Display Buffer"))
		ReportErrorMessage("No Memory, Cant Add To Improve Learn",cEMailInspx,32000);
	else
	{
		if (TempScratchImagePointer)
		{
			bool TempImprovedLearn = false;
			CopyMemory(TempScratchImagePointer,TempImageBufferPointer,vOriginalBufferSize);

			//Add image to Improve learn for Contaminant2 Structure
			if ((vImproveLearnContaminant2StructureImage) && (vImproveLearnContaminant2StructureImage->vGreyImage))
			{
				BYTE *TempPointerImageStructure = vImproveLearnContaminant2StructureImage->vGreyImage;
				BYTE *TempPointerAquisitionImage = TempScratchImagePointer;

				for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
				{//Take Minimum pixel to Structure
					if (*TempPointerImageStructure > *TempPointerAquisitionImage)
					{
						*TempPointerImageStructure = *TempPointerAquisitionImage;  
						TempImprovedLearn = true;
					}
					TempPointerImageStructure++;
					TempPointerAquisitionImage++;  
				}
				vALearnedImageHasChanged = true;
			}
			
			//add image to improve learn for standard learned contaminant
			//subtract the product's average image from this one if enabled
			if (vProductAverageImage)
				SubtractProductAverageImage(TempScratchImagePointer, vProductAverageImage);

			if ((vImproveLearnStructureImage) && (vImproveLearnStructureImage->vGreyImage))
			{

				BYTE *TempSourcePointer = TempScratchImagePointer;
				BYTE *TempStructurePointer = vImproveLearnStructureImage->vGreyImage;
				double TempValue = 0;
				for (DWORD TempLoop = 0; TempLoop < vOriginalBufferSize; TempLoop++)
				{
					if (*TempSourcePointer < 128)
					{
						TempValue = 128 - *TempSourcePointer;

						if (TempValue > 128)
							TempValue = 128;
						if (TempValue < 0)
							TempValue = 0;
						if ((BYTE)TempValue > *TempStructurePointer)
						{
							*TempStructurePointer = (BYTE)TempValue;
							TempImprovedLearn = true;
						}
					}

					TempStructurePointer++;
					TempSourcePointer++;
				}
				vALearnedImageHasChanged = true;
				if (TempImprovedLearn)
				{
					if (vImproveLearnCount < 0xFFFF)
						vImproveLearnCount++;
					if (vAutoImproveStatisticsAddedToLearnCount < 64000)
						vAutoImproveStatisticsAddedToLearnCount++;
				}
			}
		}
	}
}

void CITIPCDig::ClearImproveLearn(bool TempMakeNewBuffers)
{
	if (vOriginalBufferSize)
	{
		if (TempMakeNewBuffers)
		if (vImproveLearnStructureImage)
		{
			delete vImproveLearnStructureImage;
			vImproveLearnStructureImage = NULL;
			vImproveLearnCount = 0;
		}
		if (!vImproveLearnStructureImage)
		{
			if (ThereIsEnoughMemory(sizeof(CImageData), "vImproveLearnStructureImage"))
				vImproveLearnStructureImage = new CImageData;
			if (vImproveLearnStructureImage)
			{
				vImproveLearnStructureImage->vImageTypeString = 
					"Improve Learn";
				vImproveLearnStructureImage->vImageType = cDataImageType;
				vImproveLearnStructureImage->MakeGreyImage(vOriginalBufferSize);
			}
		}
		if ((vImproveLearnStructureImage) && (vImproveLearnStructureImage->vGreyImage))
		{
			if ((vProductStructureImage) && (vProductStructureImage->vGreyImage))
				CopyMemory(vImproveLearnStructureImage->vGreyImage, vProductStructureImage->vGreyImage, vOriginalBufferSize);
			else
				ZeroMemory(vImproveLearnStructureImage->vGreyImage, vOriginalBufferSize);
		}

		if (TempMakeNewBuffers)
		if (vImproveLearnContaminant2StructureImage)
		{
			delete vImproveLearnContaminant2StructureImage;
			vImproveLearnContaminant2StructureImage = NULL;
		}
		if (!vImproveLearnContaminant2StructureImage)
		{
			if (ThereIsEnoughMemory(sizeof(CImageData), "vImproveLearnContaminant2StructureImage"))
				vImproveLearnContaminant2StructureImage = new CImageData;
			if (vImproveLearnContaminant2StructureImage)
			{
				vImproveLearnContaminant2StructureImage->vImageTypeString = "Improve Learn2";
				vImproveLearnContaminant2StructureImage->vImageType = cDataImageType;
				vImproveLearnContaminant2StructureImage->MakeGreyImage(vOriginalBufferSize);
			}
		}
		if ((vImproveLearnContaminant2StructureImage) && (vImproveLearnContaminant2StructureImage->vGreyImage))
		{
			
			if ((vProductContaminant2StructureImageWithNoDilation) && (vProductContaminant2StructureImageWithNoDilation->vGreyImage))
				CopyMemory(vImproveLearnContaminant2StructureImage->vGreyImage, vProductContaminant2StructureImageWithNoDilation->vGreyImage, vOriginalBufferSize);
			else
			if ((vProductContaminant2StructureImage) && (vProductContaminant2StructureImage->vGreyImage))
				CopyMemory(vImproveLearnContaminant2StructureImage->vGreyImage, vProductContaminant2StructureImage->vGreyImage, vOriginalBufferSize);
			else
				FillMemory(vImproveLearnContaminant2StructureImage->vGreyImage, vOriginalBufferSize,255);
		}
	}
	vImproveLearnCount = 0;
}

void CITIPCDig::GetInspectionROIBounds(CInspection *TempInspection, WORD *TempBottom, WORD *TempLeft, WORD *TempHeight, WORD *TempWidth, int TempSideReferenceAdjust, BYTE TempReferenceToEdges, BYTE TempAutoImproveState)
{
	*TempBottom = 1;
	*TempLeft = 1;
	*TempHeight = vOriginalBufferSizeX - 2;
	*TempWidth = vOriginalBufferSizeY - 2;

	if (TempInspection)
	if ((TempInspection->vROIPixelTop) && (TempInspection->vROIPixelRight))
	{
		if (TempInspection->vROIPixelTop > vOriginalBufferSizeX)
			TempInspection->vROIPixelTop = vOriginalBufferSizeX - 1;

		*TempBottom = TempInspection->vROIPixelBottom;
		if (*TempBottom >= TempInspection->vROIPixelTop)
			*TempBottom = 0;
		*TempHeight = TempInspection->vROIPixelTop - *TempBottom;
		if (*TempHeight + *TempBottom >= vOriginalBufferSizeX)
			*TempHeight = vOriginalBufferSizeX - *TempBottom - 1;

		if (TempInspection->vROIPixelRight > vOriginalBufferSizeY)
			TempInspection->vROIPixelRight = vOriginalBufferSizeY - 1;

		if (TempReferenceToEdges)
		{
			int TempLeftPixels = TempInspection->vROIPixelLeft - vGlobalCurrentProduct->vReferenceLeft;

			*TempLeft = vLastLeft + TempLeftPixels - TempSideReferenceAdjust;

			int TempRightPixels = TempInspection->vROIPixelRight - vGlobalCurrentProduct->vReferenceRight;

			int TempRight = vLastRight + TempRightPixels - TempSideReferenceAdjust;

			if (TempRight >= vOriginalBufferSizeY)
				TempRight = vOriginalBufferSizeY - 1;

			if (*TempLeft >= TempRight)
				*TempLeft = 1;

			if (*TempLeft <= 0)
				*TempLeft = 1;

			*TempWidth = TempRight - *TempLeft;
		}
		else
		{
			*TempLeft = TempInspection->vROIPixelLeft;// - TempSideReferenceAdjust;
			int TempRight = TempInspection->vROIPixelRight;// - TempSideReferenceAdjust;

			if (TempRight >= vOriginalBufferSizeY)
				TempRight = vOriginalBufferSizeY - 1;

			if (*TempLeft >= TempRight)
				*TempLeft = 1;

			if (*TempLeft <= 0)
				*TempLeft = 1;

			*TempWidth = TempRight - *TempLeft;
		}
		if (*TempWidth + *TempLeft >= vOriginalBufferSizeY)
			*TempWidth = vOriginalBufferSizeY - *TempLeft - 1;
	}
	//if (TempAutoImproveState) //not 
	//if (TempInspection->vAddStructure)
	//{
	//	if (*TempWidth > (2 * TempAutoImproveState))
	//		*TempWidth = *TempWidth - (2 * TempAutoImproveState);

	//	if (*TempLeft + TempAutoImproveState + *TempWidth < vOriginalBufferSizeY)
	//		*TempLeft = *TempLeft + TempAutoImproveState;
	//}
}

BYTE CITIPCDig::FindDarkestPixelInROI(CInspection *TempInspection, BYTE *TempImage, int TempSideReferenceAdjust, BYTE TempReferenceToEdges)
{
	//find the darkest pixel in the ROI
	BYTE TempDarkestPixel = 255;
	WORD TempBottom = 0;
	WORD TempLeft = 0;
	WORD TempHeight = 0;
	WORD TempWidth = 0;

	BYTE TempAutoImproveState = 0;

	GetInspectionROIBounds(TempInspection, &TempBottom, &TempLeft, &TempHeight, &TempWidth, TempSideReferenceAdjust, TempReferenceToEdges, TempAutoImproveState);

	for (WORD TempXLoop = 0; TempXLoop < TempWidth; TempXLoop++)
	{
		BYTE *TempPointer = TempImage + TempBottom + ((TempLeft + TempXLoop) * vOriginalBufferSizeX);

		for (WORD TempYLoop = 0; TempYLoop < TempHeight; TempYLoop++)
		{
			if (*TempPointer < TempDarkestPixel)
				TempDarkestPixel = *TempPointer;
			TempPointer++;
		}
	}
	return TempDarkestPixel;
}


double CITIPCDig::FindNumberOfPixelsBelowThreshold(CInspection *TempInspection, BYTE *TempImage, int TempSideReferenceAdjust, BYTE TempReferenceToEdges)
{
	//find the darkest pixel in the ROI
	double TempNumberOfPixelsBelowThreshold = 0;
	WORD TempBottom = 0;
	WORD TempLeft = 0;
	WORD TempHeight = 0;
	WORD TempWidth = 0;

	BYTE TempAutoImproveState = 0;

	GetInspectionROIBounds(TempInspection, &TempBottom, &TempLeft, &TempHeight, &TempWidth, TempSideReferenceAdjust, TempReferenceToEdges, TempAutoImproveState);

	for (WORD TempXLoop = 0; TempXLoop < TempWidth; TempXLoop++)
	{
		BYTE *TempPointer = TempImage + TempBottom + ((TempLeft + TempXLoop) * vOriginalBufferSizeX);

		for (WORD TempYLoop = 0; TempYLoop < TempHeight; TempYLoop++)
		{
			if (*TempPointer < TempInspection->vThreshold)
				TempNumberOfPixelsBelowThreshold++;
			TempPointer++;
		}
	}
	return TempNumberOfPixelsBelowThreshold;
}

void CITIPCDig::MedianSmoothImage(Image *TempImage, BYTE TempSmoothAmount, WORD TempLeft, WORD TempWidth, WORD TempBottom, WORD TempHeight)
{
	if ((TempSmoothAmount) && (TempImage))
	{	//add some around ROI so can median the edges correctly
		//if (vGlobalUseHalconLibrary)
		//{//using HALCON
		//	//DoMedianSmooth(TempImage, TempSmoothAmount, TempSmoothAmount);
		//}
		//else
		{
			WORD TempExtra = (TempSmoothAmount + 1) / 2;
			if (TempLeft >= TempExtra)
			{
				TempLeft = TempLeft - TempExtra;
				if (TempWidth + TempLeft + TempExtra < TempImage->dy)
					TempWidth = TempWidth + TempExtra;
			}
			if (TempWidth + TempLeft + TempExtra < TempImage->dy)
				TempWidth = TempWidth + TempExtra; //extra for right side 

			if (TempWidth + TempLeft > TempImage->dy)  //if too wide, narrow it up
				TempWidth = TempImage->dy + TempLeft; 

			if (TempBottom >= TempExtra)
			{
				TempBottom = TempBottom - TempExtra;
				if (TempHeight + TempBottom + TempExtra < TempImage->dx)
					TempHeight = TempHeight + TempExtra;
			}
			if (TempHeight + TempBottom + TempExtra < TempImage->dx)
				TempHeight = TempHeight + TempExtra;  //extra for top

			if (TempHeight + TempBottom > TempImage->dx)  //if too tall, shorten it up
				TempHeight = TempImage->dx + TempBottom; 

			Image *TempSubImage = im_rroi(TempImage, TempBottom, TempLeft, TempHeight, TempWidth);
			if (TempSubImage)
			{
				DoMedianSmooth(TempSubImage, TempSmoothAmount, TempSmoothAmount);
				im_delete(TempSubImage);
			}
		}
	}
}

void CITIPCDig::SaveLearnFile(CString TempNameOfFile, BYTE *TempDataPointer, int TempOriginalBufferSizeX,
		int TempOriginalBufferSizeY, WORD TempBitsPerPixel, bool TempCreateBackup)
{
	CString TempNameOfFile1 = TempNameOfFile + ".BMP";
	
	int TempLength = TempNameOfFile1.GetLength();
	
	if (TempLength > MAX_PATH)
	{
		CNoticeDialog TempNoticeDialog;
		CString TempString = "\n\n\nSetup Name more than MAX_PATH characters, setup save aborted";
		TempNoticeDialog.vNoticeText = TempString;
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
		TempLength = MAX_PATH;
	}
	else
	{
		CW2A TempFileNameString(TempNameOfFile1);			
		CImgFile *TempImageFile = IfxCreateImgFile(TempFileNameString);
		if (TempImageFile) 
		{
			TRY
			{
				TempImageFile->WriteFile(TempDataPointer,
				TempOriginalBufferSizeX,
				TempOriginalBufferSizeY,
				TempBitsPerPixel);
			}
			CATCH_ALL(TempFileException)
			{
				int x = 0;//write file fail
			}
			END_CATCH_ALL
		}
		else
		{
			//ReportErrorMessage("Learn File: " + TempNameOfFile + " was unable to be written.", cEMailInspx, 32000);
		}
		
		if (TempCreateBackup)
		{
			BYTE * TempImageBuffer;
			
			if (ThereIsEnoughMemory(TempOriginalBufferSizeX * TempOriginalBufferSizeY, "Store Learn data temporarily for backup delay"))
				TempImageBuffer = (BYTE*)malloc(vOriginalBufferSize);

			CopyMemory(
				TempImageBuffer,
				TempDataPointer,
				TempOriginalBufferSizeX * TempOriginalBufferSizeY);
			
			if (TempImageBuffer)
			{
				if (vBackupFileDataFlag == 0)
					vBackupFileDataFlag = 1;
				vNamesOfFilesToBackup[vIndexOfBackupFileDataBufferEnter] = TempNameOfFile + "B";
				vDataPointersBackupFile[vIndexOfBackupFileDataBufferEnter] = TempImageBuffer;
				vOriginalBufferSizeXBackupFile[vIndexOfBackupFileDataBufferEnter] = TempOriginalBufferSizeX;
				vOriginalBufferSizeYBackupFile[vIndexOfBackupFileDataBufferEnter] = TempOriginalBufferSizeY;
				vBitsPerPixelBackupFile[vIndexOfBackupFileDataBufferEnter] = TempBitsPerPixel;
				vIndexOfBackupFileDataBufferEnter = (vIndexOfBackupFileDataBufferEnter + 1) % cSizeOfBackupBuffers;
				if (vIndexOfBackupFileDataBufferEnter == vIndexOfBackupFileDataBufferExit)
				{
					int x = 0;
					//ReportErrorMessage("Learn backup file data buffers lost some backup data, increase size of cSizeOfBackupBuffers", cEMailInspx, 32000);
				}
			}			
		}
		else
		{
			free(TempDataPointer);// free data copy buffer
			TempDataPointer = NULL;
		}
	}
}

void CITIPCDig::CheckWriteBackupLearnFile()
{
	switch (vBackupFileDataFlag)
	{
		case 0:
			return;
		case 1:
		case 2:
			vBackupFileDataFlag++;
			return;
		case 3:
			SaveLearnFile(vNamesOfFilesToBackup[vIndexOfBackupFileDataBufferExit], 
					vDataPointersBackupFile[vIndexOfBackupFileDataBufferExit],
					vOriginalBufferSizeXBackupFile[vIndexOfBackupFileDataBufferExit],
					vOriginalBufferSizeYBackupFile[vIndexOfBackupFileDataBufferExit],
					vBitsPerPixelBackupFile[vIndexOfBackupFileDataBufferExit],
					0);	//0 - don't create backup of backup
			vIndexOfBackupFileDataBufferExit = (vIndexOfBackupFileDataBufferExit+1) % cSizeOfBackupBuffers;
			
			if (vIndexOfBackupFileDataBufferExit == vIndexOfBackupFileDataBufferEnter)	//nothing left in buffer
				vBackupFileDataFlag = 0;
			else
				vBackupFileDataFlag = 2;	//something left in buffer, take care of it next call of this function CheckWriteBackupLearnFile
			return;
	}
}

CImageData * CITIPCDig::LoadLearnImageFromFile(CString TempProcessType, CProduct *TempProduct, CImageData *TempImageToReturn, CString TempImageTypeString)
{
	bool TempRead = false;
	if (HaveLearnImageFile(TempProcessType))
	{
		CString TempString1 =  *TempProduct->GetProductName() + TempProcessType;
		if (!vUsingLocalSetupDataFile)
			TempString1 = cSetupDataDirectory + TempString1;
		else
			TempString1 = vGlobalCurrentDirectory + TempString1;
		TempString1 = TempString1 + ".BMP";
		LPTSTR TempString = TempString1.GetBuffer(TempString1.GetLength());
		int TempHeight = 0;
		int TempWidth = 0;
		int TempBitsPerPixel = 0;
		int TempType = 0;

		if (vProductBackgroundImage != NULL)
		{
			im_delete(vProductBackgroundImage);
			vProductBackgroundImage = NULL;
		}
		CFileStatus TempFileStatus;
		if (!CFile::GetStatus(TempString, TempFileStatus))
		{
			ReportErrorMessage("Failed to Open File: " + TempString1, cEMailInspx, 32000);
			TempString1.Insert(TempString1.GetLength()-4, 'B');
			TempString = TempString1.GetBuffer(TempString1.GetLength());
		}
		if (CFile::GetStatus(TempString, TempFileStatus))
		{
			CW2A TempFileNameString(TempString);			
			Erflag TempError = im_file_get_size(TempFileNameString, IM_BMP_FILE, &TempHeight, &TempWidth, &TempBitsPerPixel, &TempType);
			if (TempError == IM_OK)
			if (((TempWidth != vOriginalBufferSizeY) || (TempHeight != vOriginalBufferSizeX)) && (!vGlobalDemoMode) && (_wtoi(TempProcessType.Right(1))!=0))	//if it is process 0, it can handle wrong size
			{
				//ReportErrorMessage("Error" + TempProcessType +" Learned Image wrong size, ignored", cEMailInspx, 32000);
				//ReportErrorMessage("Notice-This Product Must Be Relearned-Image Size Changed", cError, 0);
			}
			else
			if (ThereIsEnoughMemory(vOriginalBufferSize + sizeof(Image), "Temp Image In"))
			{
				Image *TempImageIn = NULL;
				TempImageIn = im_create(IM_BYTE, vOriginalBufferSizeX, vOriginalBufferSizeY);
				if ((TempImageIn) && (TempImageIn->pix))
				{
					TempError = im_file_read(TempImageIn, TempFileNameString, IM_AUTO_FILE);
					if (TempError != IM_OK)	//if did not return file properly, try to read Backup file
					{
						TempString1.Insert(TempString1.GetLength()-4, 'B');
						TempString = TempString1.GetBuffer(TempString1.GetLength());
						CW2A TempFileNameString(TempString);			
						TempError = im_file_read(TempImageIn, TempFileNameString, IM_AUTO_FILE);
					}
					if (TempError == IM_OK)
					{
						TempRead = true;
						
						if (!TempImageToReturn)
						if (ThereIsEnoughMemory(sizeof(CImageData), "vProductStructureImage"))
							TempImageToReturn = new CImageData;

						if (TempImageToReturn)
						{
							TempImageToReturn->vImageTypeString = TempImageTypeString;
							TempImageToReturn->vImageType = cDataImageType;
							TempImageToReturn->MakeGreyImage(vOriginalBufferSize);

							if (TempImageToReturn->vGreyImage)
								CopyMemory(TempImageToReturn->vGreyImage, TempImageIn->pix, vOriginalBufferSize);
						}
					}
				}
				if (TempImageIn)
					im_delete(TempImageIn);
			}
		}
		else
		{
			ReportErrorMessage("Failed to Open File: " + TempString1, cEMailInspx, 32000);
		}
	}
	if (TempRead)
		return TempImageToReturn;
	else
		return NULL;
}

void CITIPCDig::DoSobelXY(Image *TempInputImage, Image *TempScratchImage)
{
	if ((TempInputImage) && (TempInputImage->pix))
	{
//#ifdef UseHalconLibraries
//		if (vGlobalUseHalconLibrary)
//		{//using HALCON
//			HByteImage TempHalconImageIn = MakeHalconImage(TempInputImage);
//			HByteImage TempSobelResultImage = TempHalconImageIn.SobelAmp("sum_abs", 3); //options {’sum_abs’, ’sum_sqrt’, ’sum_abs_binomial’, ’sum_sqrt_binomial’
//
//			HByteImage TempSobelInvertedResultImage = TempSobelResultImage.InvertImage();
//			CopyFromHalconImage(TempSobelInvertedResultImage, TempInputImage);
//		}
//		else
//#endif
		{ //MVTools
			if ((TempScratchImage) && (TempScratchImage->pix))
			{
				Erflag TempError = mvt_sobelxy(TempInputImage,TempScratchImage,OL_ABS);
				if (TempError == IM_OK) //0
				{
					TempError = mvt_invert(TempScratchImage);
				}
				CopyMemory(TempInputImage->pix,TempScratchImage->pix,vOriginalBufferSize);
			}
		}
	}
}

void CITIPCDig::DoConvolveImage3x3(tImage *TempImage) 
{
	if ((TempImage) && (TempImage->pix))
	{
//#ifdef UseHalconLibraries
//		if (vGlobalUseHalconLibrary)
//		{//using HALCON
//			HByteImage TempHalconImageIn = MakeHalconImage(TempImage);
//			HByteImage TempConvolveResultImage = TempHalconImageIn.DerivateGauss(3, "xy");
//			//derivate_gauss ( Image : DerivGauss : Sigma, Component : )
//
//			CopyFromHalconImage(TempConvolveResultImage, TempImage);
//		}
//		else
//#endif
		{//MVTools
#ifdef UseIFCDLLs
			Erflag TempError = mvt_convolve_byte(TempImage, 3, 3, &Kernel3X3[0], -2, 0, MVT_POSITIVE);
#endif
		}
	}
}

void CITIPCDig::DoConvolveImage5x5(tImage *TempImageIn,tImage *TempImageOut) 
{
	if ((TempImageIn) && (TempImageIn->pix))
	{
//#ifdef UseHalconLibraries
//		if (vGlobalUseHalconLibrary)
//		{//using HALCON
//			HByteImage TempHalconImageIn = MakeHalconImage(TempImageIn);
//			HByteImage TempConvolveResultImage = TempHalconImageIn.DerivateGauss(5, "xy");
//			//derivate_gauss ( Image : DerivGauss : Sigma, Component : )
//
//			CopyFromHalconImage(TempConvolveResultImage, TempImageIn);
//		}
//		else
//#endif
		{//MVTools
			Image *TempOutputImage = NULL;

			if (ThereIsEnoughMemory(vOriginalBufferSize + sizeof(Image), "Enhance Image Buffer"))
				TempOutputImage = im_create(IM_BYTE, vOriginalBufferSizeX, vAquisitionSizeY);

			if ((TempOutputImage) && (TempOutputImage->pix))
			{

#ifdef UseIFCDLLs
				Erflag TempError = mvt_convolve_5x5(TempImageIn, TempImageOut, &Kernel5X5[0], OL_CLIP);
#endif
				CopyMemory(TempImageIn->pix, TempOutputImage->pix, vOriginalBufferSize);
			}
			if (TempOutputImage)
				im_delete(TempOutputImage);
		}
	}
}

void CITIPCDig::DoConvolveImage7x7(tImage *TempImage) 
{
	if ((TempImage) && (TempImage->pix))
	{
//#ifdef UseHalconLibraries
//		if (vGlobalUseHalconLibrary)
//		{//using HALCON
//			HByteImage TempHalconImageIn = MakeHalconImage(TempImage);
//			HByteImage TempConvolveResultImage = TempHalconImageIn.DerivateGauss(7, "xy");
//
//			CopyFromHalconImage(TempConvolveResultImage, TempImage);
//		}
//		else
//#endif
		{//MVTools
			Erflag TempError = mvt_convolve_byte(TempImage, 7,7,&Kernel7X7[0],-8,0,MVT_NOCHANGE);
		}
	}
}

void CITIPCDig::SortImage(BYTE *TempImage, DWORD TempSize)
{
	for (DWORD TempMainLoop = 0; TempMainLoop < TempSize; TempMainLoop++)
	{
		bool TempMadeASwap = false;
		BYTE *TempPointer = TempImage;
		BYTE *TempPointerToNext = TempImage + 1;
		for (DWORD TempSortLoop = 0; TempSortLoop < TempSize - 1; TempSortLoop++)
		{
			if (*TempPointer < *TempPointerToNext)
			{
				BYTE TempByte = *TempPointer;
				*TempPointer = *TempPointerToNext;
				*TempPointerToNext = TempByte;
				TempMadeASwap = true;
			}
			TempPointer++;
			TempPointerToNext++;
		}
		if (!TempMadeASwap)
			TempMainLoop = TempSize;
	}
}

void CITIPCDig::AndImages(BYTE *TempImageIn, BYTE *TempImageOut, DWORD TempSize)
{
	if (TempImageIn)
	if (TempImageOut)
	{
		BYTE *TempInPointer = TempImageIn;
		BYTE *TempOutPointer = TempImageOut;
		for (DWORD TempLoop = 0; TempLoop < TempSize; TempLoop++)
		{
			if ((*TempInPointer == 0) && (*TempOutPointer == 0))
				*TempOutPointer = 0;
			else
				*TempOutPointer = 255;

			TempInPointer++;
			TempOutPointer++;
		}
	}
}
