//ScanTrac Side View Source File
// ProcessEvaluateImagesThread.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "scantrac.h"
#include "ProcessEvaluateImagesThread.h"
#include "ImageAquisitionThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CImageAquisitionThread *vGlobalImageAquisitionThread;
extern CITIPCDig *vGlobalITIPCDig;
extern CProduct *vGlobalCurrentProduct;
extern CProcessEvaluateImagesThread *vGlobalProcessEvaluateImagesThread;
/////////////////////////////////////////////////////////////////////////////
// CProcessEvaluateImagesThread

IMPLEMENT_DYNCREATE(CProcessEvaluateImagesThread, CWinThread)

CProcessEvaluateImagesThread::CProcessEvaluateImagesThread()
{
	vLocalSystemData = NULL;
	vLocalConfigurationData = NULL;
	vMainWindowPointer = NULL;
}

CProcessEvaluateImagesThread::~CProcessEvaluateImagesThread()
{
}

BOOL CProcessEvaluateImagesThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return true;
}

int CProcessEvaluateImagesThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CProcessEvaluateImagesThread, CWinThread)
	//{{AFX_MSG_MAP(CProcessEvaluateImagesThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessEvaluateImagesThread message handlers

int CProcessEvaluateImagesThread::Run() 
{
	ReportErrorMessage("Started processing Evaluate: " + dtoa(cNumberOfImagesToEvaluate, 0), cWriteToLog, 0);

	Image *TempScratchImage = NULL;
	if (ThereIsEnoughMemory(vLocalSystemData->vITIPCDig->vOriginalBufferSize + sizeof(Image), "TempScratchImage"))
		TempScratchImage = im_create(IM_BYTE, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vAquisitionSizeY);

	Image *TempWorkingImage = NULL;
	if (ThereIsEnoughMemory(vLocalSystemData->vITIPCDig->vOriginalBufferSize + sizeof(Image), "TempWorkingImage"))
		TempWorkingImage = im_create(IM_BYTE, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vAquisitionSizeY);

	//first write images to calibration images directory
	for (BYTE TempLoop = 0; TempLoop < cNumberOfImagesToEvaluate; TempLoop++)
	if (vLocalSystemData->vQuickLearnImages[TempLoop])
	{
		CString TempString = "EvaluateInTDAutoSetup";
		TempString = TempString + dtoa(TempLoop, 0);
		TempString = TempString + "xteamviewr.BMP";
		TempString = vLocalConfigurationData->vScanTracCalibrationImagesDirectory + TempString;
		//TempString = cLogDirectory + TempString;
		int TempLength = TempString.GetLength();
		if (PasswordOK(cTemporaryInspxPassword,false))// && (vLocalSystemData->vWriteCalibrationImages))
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
			CW2A TempFileNameString(TempString);
			CImgFile *TempImageFile=IfxCreateImgFile(TempFileNameString);
			if (TempImageFile) 
			{
				if (!TempImageFile->WriteFile(vLocalSystemData->vQuickLearnImages[TempLoop], vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 8))
				{
					ReportErrorMessage("Failed to Write TD Evaluate Image to File", cEMailInspx, 32000);
				}

				delete TempImageFile;
			}
		}
	}

	//save one image for the Average for the learn on TD ScanTracs.

	if ((vLocalConfigurationData->vScanTracType != cSoloScanTrac) && (vLocalConfigurationData->vScanTracType != cSoloPlusScanTrac))
	{
		vLocalSystemData->vITIPCDig->DeleteLearnImages();
		vLocalSystemData->vITIPCDig->DeleteLearnFiles(vGlobalCurrentProduct);
		vLocalSystemData->vITIPCDig->MakeAverageImageBuffer();
		vLocalSystemData->vITIPCDig->vAverageCount = 1;

		//for testing
		//vGlobalImageAquisitionThread->WriteImageToHardDisk(vLocalSystemData->vQuickLearnImages[5], "c:\\ScanTracSetupData\\Learned Image.bmp", vGlobalITIPCDig->vOriginalBufferSizeX, vGlobalITIPCDig->vOriginalBufferSizeY);

		if (vLocalSystemData->vITIPCDig->vAverageImage != NULL)
		{
			BYTE *TempSourcePointer = vLocalSystemData->vQuickLearnImages[5];
			DWORD *TempDestinationPointer = vGlobalITIPCDig->vAverageImage;

			for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
				*TempDestinationPointer++ = *TempSourcePointer++;

			if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
			{
				if (vLocalSystemData->vITIPCDig->MakeProductAverageImage())
				{
					vLocalSystemData->vITIPCDig->SaveAverageImageToFile(vGlobalCurrentProduct);

					vGlobalCurrentProduct->vSetupHistoryAverageDensity = 0;

					if (vGlobalCurrentProduct->vHistogramThreshold > 0)
						vLocalSystemData->vITIPCDig->SaveProcess4ImageData(vGlobalCurrentProduct);

					vGlobalCurrentProduct->vSetupHistoryAverageCount = 1;
					vLocalSystemData->vITIPCDig->CalculateAverageDriftDensity(vGlobalCurrentProduct);
					vLocalSystemData->vIndexForDriftDensityOfLastNImages = 0;
					if (vGlobalCurrentProduct->vSetupHistoryAverageDensity > 255)
						vGlobalCurrentProduct->vSetupHistoryAverageDensity = 0;
					vGlobalCurrentProduct->vSetupHistoryContainerTriggerStandardDeviation = vLocalSystemData->vBodyTriggerWidthStandardDeviation;
				}
				vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();		
			}
		}
	}
	if (vLocalSystemData->vITIPCDig->vAverageImage != NULL)
	{
		free(vLocalSystemData->vITIPCDig->vAverageImage);
		vLocalSystemData->vITIPCDig->vAverageImage = NULL;
	}

	//analyze evaluate images here

	//for each inspection, evaluate the images to calculate the Threshold
	for (DWORD TempLoopI = 0; TempLoopI < vGlobalCurrentProduct->vNumberOfInspections; TempLoopI++)
	if (vGlobalCurrentProduct->vInspection[TempLoopI])
	if (!vGlobalCurrentProduct->vInspection[TempLoopI]->vAddStructure) //don't evaluate learned inspections as they do not evaluate well after quick learn of 10
	{
		//initialize data structures
		if (!TempLoopI)
		{
			if (vLocalSystemData->vMinimumGoodData)
			{
				free(vLocalSystemData->vMinimumGoodData);
				vLocalSystemData->vMinimumGoodData = NULL;
			}
			if (ThereIsEnoughMemory(cStandardDeviationSampleSize,"Minimum Good Sample Array"))
				vLocalSystemData->vMinimumGoodData = (BYTE*)malloc(cStandardDeviationSampleSize);
			
			if (vLocalSystemData->vMinimumGoodDataFloat)
			{
				free(vLocalSystemData->vMinimumGoodDataFloat);
				vLocalSystemData->vMinimumGoodDataFloat = NULL;
			}
			if (ThereIsEnoughMemory(cStandardDeviationSampleSize * sizeof(double),"Minimum Good Sample Array"))
				vLocalSystemData->vMinimumGoodDataFloat = (double*)malloc(cStandardDeviationSampleSize * sizeof(double));
		}
		double TempMinimumGoodStandardDeviation = 0;
		double TempMinimumGoodAverage = 0;
		double TempMinimumGoodIntensity = 255;
		double TempMaximumGoodIntensity = 0;

		//for each evaluation image, find the numbers
		for (DWORD TempLoop = 0; TempLoop < cNumberOfImagesToEvaluate; TempLoop++)
		if (vLocalSystemData->vQuickLearnImages[TempLoop])
		{
			//find minimum good for this image
			BYTE TempMinimumGood = 255;
			BYTE *TempPointer = vLocalSystemData->vQuickLearnImages[TempLoop];
			WORD TempStartX = 0;
			WORD TempEndX = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
			WORD TempStartY = 0;
			WORD TempEndY = vLocalSystemData->vITIPCDig->vAquisitionSizeY;
			WORD TempLeft = 0;
			WORD TempRight = vLocalSystemData->vITIPCDig->vAquisitionSizeY;

			WORD TempBottom = vGlobalCurrentProduct->vInspection[TempLoopI]->vROIPixelBottom;
			WORD TempTop = TempRight - TempLeft;

			vLocalSystemData->vITIPCDig->FindContainerEdgesCheckingFromCenter(TempScratchImage->pix, &TempTop, &TempBottom,	&TempRight, &TempLeft);

			WORD TempHeight = vGlobalCurrentProduct->vInspection[TempLoopI]->vROIPixelTop - vGlobalCurrentProduct->vInspection[TempLoopI]->vROIPixelBottom;
			WORD TempWidth = vLocalSystemData->vITIPCDig->vAquisitionSizeY;

			if ((TempLeft) || (vGlobalCurrentProduct->vInspection[TempLoopI]->vROIPixelBottom))
			{
				TempStartX = vGlobalCurrentProduct->vInspection[TempLoopI]->vROIPixelBottom;
				TempEndX = vGlobalCurrentProduct->vInspection[TempLoopI]->vROIPixelTop;
				TempStartY = TempLeft; //TempInspection->vROIPixelLeft;
				TempEndY = TempLeft + TempWidth; //TempInspection->vROIPixelRight;
			}

			//Analyze Image
			BYTE TempThreshold = vGlobalCurrentProduct->vInspection[TempLoopI]->vThreshold;

			double TempResult = vGlobalCurrentProduct->vInspection[TempLoopI]->AnalyzeImage(vLocalSystemData->vQuickLearnImages[TempLoop], TempScratchImage->pix, TempThreshold);

			if ((vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cContaminantInspection) || (vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cJarBottomContaminantInspection) ||
				(vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cWireContaminantInspection) || (vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cDripInspection) ||
				(vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cDripBySpoutInspection) || (vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cBadHeartInspection) ||
				(vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cIPContaminantInspection) || (vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cThresholdContaminantInspection) ||
				(vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cContaminant2Inspection) || (vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cAdvancedContaminant2Inspection) ||
				(vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cGlass13mmContaminantInspection) || (vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cSpotInROIContaminantInspection) ||
				(vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cContaminantBonesInspection) || (vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cContaminant22Inspection) ||
				(vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cTopContaminantInspection) || (vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cBottomContaminantInspection))
			{
				BYTE *TempMinimumGoodDataPointer = vLocalSystemData->vMinimumGoodData + TempLoop; //place to store this data point in data collection array

				if (vLocalSystemData->vMinimumGoodData)
				if (vGlobalCurrentProduct->vInspection[TempLoopI]->vSize == 1)
				{//go through each pixel in the ROI and find lowest value
					for (WORD TempLoopY = TempStartY; TempLoopY < TempEndY; TempLoopY++)
					{
						TempPointer = TempWorkingImage->pix + TempStartX + (TempLoopY * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
						for (WORD TempLoopX = TempStartX; TempLoopX < TempEndX; TempLoopX++)
						{
							if (*TempPointer < TempMinimumGood)
							if (*TempPointer > 1)  //br ht change to 125
								TempMinimumGood = *TempPointer;
							TempPointer++;
						}
					}
				}
				else
				{//spec size greater than one, so must search for minimum good value
					BYTE TempTimesToLoop = 8;
					BYTE TempTryValue = 128;
					BYTE TempRange = 128;

					//for testing
					//vGlobalImageAquisitionThread->WriteImageToHardDisk(vLocalSystemData->vQuickLearnImages[TempLoop], "c:\\ScanTracSetupData\\PreProcess In=" + dtoa(TempLoopI,0) + " Im-" + dtoa(TempLoop,0) + ".bmp", 
					//	vGlobalITIPCDig->vOriginalBufferSizeX, vGlobalITIPCDig->vOriginalBufferSizeY);

					for (BYTE TempLoopTry = 0; TempLoopTry < TempTimesToLoop; TempLoopTry++)
					{
						//to evaluate the Threshold, must do these inspections each time as the threshold is used in the mathematical analysis
						if (vGlobalCurrentProduct->vInspection[TempLoopI]->vFindDarkSpotOnLine)
						{
							CopyMemory(TempWorkingImage->pix, vLocalSystemData->vQuickLearnImages[TempLoop], vLocalSystemData->vITIPCDig->vOriginalBufferSize);

							if (vGlobalCurrentProduct->vInspection[TempLoopI]->vPreSmoothImage)
								DoMedianSmooth(TempWorkingImage, vGlobalCurrentProduct->vInspection[TempLoopI]->vPreSmoothImage, vGlobalCurrentProduct->vInspection[TempLoopI]->vPreSmoothImage);

							vGlobalCurrentProduct->vInspection[TempLoopI]->FindDarkSpotOnLine(TempWorkingImage->pix, TempScratchImage->pix,
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, TempTryValue);
						}
						else
						if (vGlobalCurrentProduct->vInspection[TempLoopI]->vFindSpotInSubROI)
						{
							CopyMemory(TempWorkingImage->pix, vLocalSystemData->vQuickLearnImages[TempLoop], vLocalSystemData->vITIPCDig->vOriginalBufferSize);

							if (vGlobalCurrentProduct->vInspection[TempLoopI]->vPreSmoothImage)
								DoMedianSmooth(TempWorkingImage, vGlobalCurrentProduct->vInspection[TempLoopI]->vPreSmoothImage, vGlobalCurrentProduct->vInspection[TempLoopI]->vPreSmoothImage);

							vGlobalCurrentProduct->vInspection[TempLoopI]->FindSpotInSubROI(TempWorkingImage->pix, TempScratchImage,
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 
								TempTryValue, vLocalSystemData->vITIPCDig->vDesensitizeImage, false, TempBottom, TempBottom + TempHeight, TempLeft, TempLeft + TempWidth);
						}
						else
						if (vGlobalCurrentProduct->vInspection[TempLoopI]->vTakeDifferenceFromMedian)  //bones algorithm
						{
							CopyMemory(TempWorkingImage->pix, vLocalSystemData->vQuickLearnImages[TempLoop], vLocalSystemData->vITIPCDig->vOriginalBufferSize);

							if (vGlobalCurrentProduct->vInspection[TempLoopI]->vPreSmoothImage)
								DoMedianSmooth(TempWorkingImage, vGlobalCurrentProduct->vInspection[TempLoopI]->vPreSmoothImage,vGlobalCurrentProduct->vInspection[TempLoopI]->vPreSmoothImage);

							vGlobalCurrentProduct->vInspection[TempLoopI]->TakeDifferenceFromMedian(TempWorkingImage->pix,  TempScratchImage,
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vLocalSystemData->vQuickLearnImages[TempLoop], TempTryValue);
						}
						else
						{
							if (TempLoopTry == 0)  //only need to process image first time through as does not change as Threshold changes
							{
								int TempReferenceSideAdjustAmount = 0;
								if ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode)) //Not Continuous Feed, and Not a Pipeline
								if ((vLocalConfigurationData->vScanTracType != cAtempoScanTrac) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cAllegroScanTrac) &&
									(vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac)) //Continuous Feed, or a Pipeline or TD
									TempReferenceSideAdjustAmount = vLocalSystemData->vITIPCDig->CalculateSideToSideReference(vGlobalCurrentProduct->vReferenceRight, vGlobalCurrentProduct->vReferenceLeft, &TempRight, &TempLeft);

								CopyMemory(TempWorkingImage->pix, vLocalSystemData->vQuickLearnImages[TempLoop], vLocalSystemData->vITIPCDig->vOriginalBufferSize);
								if ((!vGlobalCurrentProduct->vInspection[TempLoopI]->vAddStructure) || (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage)) //don't do inspection if no structure and using
								{
									vLocalSystemData->vITIPCDig->GetInspectionROIBounds(vGlobalCurrentProduct->vInspection[TempLoopI], &TempBottom, &TempLeft, &TempHeight, &TempWidth, 0, vLocalConfigurationData->vReferenceROIsToEdges, 0);

									if (vGlobalCurrentProduct->vInspection[TempLoopI]->vFillBackground)
										vGlobalCurrentProduct->vInspection[TempLoopI]->FillBackground(TempWorkingImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

									if (vGlobalCurrentProduct->vInspection[TempLoopI]->vFillInLightSpots)
										vGlobalCurrentProduct->vInspection[TempLoopI]->FillInLightSpots(TempWorkingImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, 
										vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vGlobalCurrentProduct->vInspection[TempLoopI]->vFillInLightSpots);

									//if (vGlobalCurrentProduct->vInspection[TempLoopI]->vFillInLightSpots)
									//	vGlobalCurrentProduct->vInspection[TempLoopI]->CurveFitFillInLightSpots(TempWorkingImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

									if (vGlobalCurrentProduct->vInspection[TempLoopI]->vPreSmoothImage)
										vLocalSystemData->vITIPCDig->MedianSmoothImage(TempWorkingImage, vGlobalCurrentProduct->vInspection[TempLoopI]->vPreSmoothImage, TempLeft, TempWidth, TempBottom, TempHeight);

									if (vGlobalCurrentProduct->vInspection[TempLoopI]->vDilationCount)
									for (BYTE TempLoopD = 0; TempLoopD < vGlobalCurrentProduct->vInspection[TempLoopI]->vDilationCount; TempLoopD++)
										vGlobalCurrentProduct->vInspection[TempLoopI]->DilateImage(TempWorkingImage, TempScratchImage);

									if (vGlobalCurrentProduct->vInspection[TempLoopI]->vErodeCount)  //is variable to hold erode times count
										vGlobalCurrentProduct->vInspection[TempLoopI]->ErodeEdgesOfContainer(TempWorkingImage, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

									if (vGlobalCurrentProduct->vInspection[TempLoopI]->vRemoveJarPunt)
										vGlobalCurrentProduct->vInspection[TempLoopI]->RemoveJarPunt(TempWorkingImage->pix,
											vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 
											vLocalSystemData->vITIPCDig->vLastLeft - vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust,
											vLocalSystemData->vITIPCDig->vLastRight - vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust, 
											vGlobalCurrentProduct->vSetupHistoryAverageDensity, 0);

									if (vGlobalCurrentProduct->vInspection[TempLoopI]->vTrimEdgesOfJar)
									if (!vGlobalCurrentProduct->vInspection[TempLoopI]->vDoContaminant22Algorithm)
									{  //trim from edge of jar, not edge of ROI
										WORD TempLeft = vLocalSystemData->vITIPCDig->vLastLeft;

										WORD TempRight = vLocalSystemData->vITIPCDig->vLastRight;

										if (TempRight >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeY)
											TempRight = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1;

										if (TempLeft >= TempRight)
											TempLeft = 1;

										vGlobalCurrentProduct->vInspection[TempLoopI]->TrimEdgesOfJarAtROI(TempWorkingImage->pix,
											vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, true,
											vLocalSystemData->vITIPCDig->vLastLeft - vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust,
											vLocalSystemData->vITIPCDig->vLastRight - vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust);
									}

									if (vGlobalCurrentProduct->vInspection[TempLoopI]->vTakeDifferenceFromMedianFilter)
									{
										CopyMemory(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, TempWorkingImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

										DoMedianSmooth(vLocalSystemData->vITIPCDig->vNIDDScratchImage, vGlobalCurrentProduct->vInspection[TempLoopI]->vTakeDifferenceFromMedianFilter, vGlobalCurrentProduct->vInspection[TempLoopI]->vTakeDifferenceFromMedianFilter);

#ifdef UseIFCDLLs
										mvt_subtract(TempWorkingImage, vLocalSystemData->vITIPCDig->vNIDDScratchImage);
#endif
										CopyMemory(TempWorkingImage->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
									}

									if (vGlobalCurrentProduct->vInspection[TempLoopI]->vTakeDerivative)
										vGlobalCurrentProduct->vInspection[TempLoopI]->TakeLineDerivative(TempWorkingImage->pix, TempScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 
											vGlobalCurrentProduct->vSetupHistoryAverageDensity, vGlobalCurrentProduct->vInspection[TempLoopI]->vThreshold);

									if (vGlobalCurrentProduct->vInspection[TempLoopI]->vFindDarkSpotOnLine)
										vGlobalCurrentProduct->vInspection[TempLoopI]->FindDarkSpotOnLine(TempWorkingImage->pix, TempScratchImage->pix,
										vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vGlobalCurrentProduct->vInspection[TempLoopI]->vThreshold);

									if (vGlobalCurrentProduct->vInspection[TempLoopI]->vPerformGlassAlgorithm == 1)
									{
										vGlobalCurrentProduct->vInspection[TempLoopI]->ClipToCurveFit(TempWorkingImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);
									}

									if (vGlobalCurrentProduct->vInspection[TempLoopI]->vPerformGlassAlgorithm == 2)
									if ((TempScratchImage) && (TempScratchImage->pix))
									{
										vGlobalCurrentProduct->vInspection[TempLoopI]->CalculateLineDerivative(TempWorkingImage->pix, TempScratchImage->pix,
											vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 
											//1, 0); //vGlobalCurrentProduct->vInspection[TempLoopI]->vRequire2InARow, vGlobalCurrentProduct->vInspection[TempLoopI]->vOverlap);
											vGlobalCurrentProduct->vSetupHistoryAverageDensity);
									}

									if (vGlobalCurrentProduct->vInspection[TempLoopI]->vDoContaminant22Algorithm)
									{
										BYTE TempThreshold = vGlobalCurrentProduct->vInspection[TempLoopI]->vThreshold;	//ams 0407 TH scaling begins
										int TempResult22 = vGlobalCurrentProduct->vInspection[TempLoopI]->InspxContaminant22InspectionB(TempWorkingImage->pix,
											vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, TempThreshold, 0);
									}

									if (vGlobalCurrentProduct->vInspection[TempLoopI]->vSubtractAverage)
									if (vLocalSystemData->vITIPCDig->vProductAverageImage)
										vLocalSystemData->vITIPCDig->SubtractProductAverageImage(TempWorkingImage->pix, vLocalSystemData->vITIPCDig->vProductAverageImage);

									//add the product's structure image to this one if enabled
									if ((vGlobalCurrentProduct->vInspection[TempLoopI]->vAddStructure) && (vGlobalCurrentProduct->vInspection[TempLoopI]->vSubtractAverage))
									{
										if (vLocalSystemData->vITIPCDig->vProductStructureImage)
											vLocalSystemData->vITIPCDig->AddProductStructureImage(TempWorkingImage->pix);
									}
									else
									if ((vGlobalCurrentProduct->vInspection[TempLoopI]->vAddStructure) && (!vGlobalCurrentProduct->vInspection[TempLoopI]->vSubtractAverage))
									{
										if (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage)
											vLocalSystemData->vITIPCDig->AddProductContaminant2StructureImage(TempWorkingImage->pix);
									}

									if (vGlobalCurrentProduct->vInspection[TempLoopI]->vEnhanceMethod)
										vLocalSystemData->vITIPCDig->PerformEnhanceOnImage(TempWorkingImage, TempScratchImage, vGlobalCurrentProduct->vInspection[TempLoopI]->vEnhanceMethod);

									//if (vGlobalCurrentProduct->vInspection[TempLoopI]->vDoImageFactors)
									//	vLocalSystemData->vITIPCDig->CombineImages(TempWorkingImage->pix, vGlobalCurrentProduct->vInspection[TempLoopI]->vEnhancedImageFactor,
									//		vLocalSystemData->vQuickLearnImages[TempLoop], vGlobalCurrentProduct->vInspection[TempLoopI]->vOriginalImageFactor);
								}
							}
						}
						//for testing
						//vGlobalImageAquisitionThread->WriteImageToHardDisk(TempWorkingImage->pix, "c:\\ScanTracSetupData\\PreBlob In=" + dtoa(TempLoopI,0) + " Im-" + dtoa(TempLoop,0) + " Step-" + dtoa(TempLoopTry,0) + " Th-" + dtoa(TempTryValue,0) + ".bmp", 
						//	vGlobalITIPCDig->vOriginalBufferSizeX, vGlobalITIPCDig->vOriginalBufferSizeY);

						if (vGlobalCurrentProduct->vInspection[TempLoopI]->ThereAreAnyBlobsAtValue(TempTryValue, TempWorkingImage, TempBottom, TempLeft, TempHeight, TempWidth))
							TempTryValue = TempTryValue - TempRange; //if have a contaminant at this value, then it was too high, so remove that range trial, 

						TempRange = TempRange / 2; //test next bit lower
						TempTryValue = TempTryValue + TempRange; //add that amount to test next bit lower to try
					}
					//if (TempTryValue)  //sometimes contaminant T might be zero if have an item in it.
						TempMinimumGood = TempTryValue;
				} //end spec size greater than 1

				if (vLocalSystemData->vMinimumGoodData)
				{
					*TempMinimumGoodDataPointer = TempMinimumGood; //store this images lowest good threshold in data array
					ReportErrorMessage("Eval Insp: " + dtoa(TempLoopI, 0) + "-" + vGlobalCurrentProduct->vInspection[TempLoopI]->vName + " Image: " + dtoa(TempLoop, 0) + " Eval Thresh: " + dtoa(TempMinimumGood,0), cWriteToLog, 0);

					if (TempMinimumGoodIntensity > *TempMinimumGoodDataPointer)
						TempMinimumGoodIntensity = *TempMinimumGoodDataPointer;

					if (TempLoop == cNumberOfImagesToEvaluate - 1) //on last loop calculate Standard Deviation
					{
						TempMinimumGoodStandardDeviation = CalculateStandardDeviation(vLocalSystemData->vMinimumGoodData, cNumberOfImagesToEvaluate, &TempMinimumGoodAverage);
						if (TempMinimumGoodStandardDeviation < .5)
							TempMinimumGoodStandardDeviation = .5;

						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryMinimumGood = (float)TempMinimumGoodIntensity;
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryMinimumGoodAverage = (float)TempMinimumGoodAverage;
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryMinimumGoodStandardDeviation = (float)TempMinimumGoodStandardDeviation;
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold = 0;
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryMinimumGoodCount = cNumberOfImagesToEvaluate;

						if (TempMinimumGoodAverage > (5 * TempMinimumGoodStandardDeviation))
							vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold = (BYTE)(TempMinimumGoodAverage - (5.0 * TempMinimumGoodStandardDeviation));
						else
							vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold = 0;

						if (vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cSpotInROIContaminantInspection)
						{
							double TempMargin = TempMinimumGoodStandardDeviation;
							if (TempMargin < 2)
								TempMargin = 2; 

							if (TempMinimumGoodIntensity > TempMargin)
								vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold = (BYTE)(TempMinimumGoodIntensity - TempMargin);//Set Threshold to Minimum minus one Standard Dieviation
							else
								vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold = 0;
						}
						if (vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cThresholdContaminantInspection)
						{
							double TempMargin = 5.0 * TempMinimumGoodStandardDeviation;
							if (TempMargin > 20)
								TempMargin = 20;
							if (TempMargin < 5)
								TempMargin = 5;

							if (TempMinimumGoodIntensity > TempMargin)
								vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold = (BYTE)(TempMinimumGoodIntensity - TempMargin);
							else
								vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold = 0;
						}

						ReportErrorMessage("Eval Insp: " + vGlobalCurrentProduct->vInspection[TempLoopI]->vName + " Preliminary Threshold: " + dtoa(vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold, 1), cWriteToLog, 0);

						if (vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cSpotInROIContaminantInspection)
						{
							if (vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold > 245)
								vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold = 245;
						}
						else
						{
							if (vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold > 240)
								vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold = 240;
						}

						//if (vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cThresholdContaminantInspection)
						//{
						//	if (vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold < 20)
						//		vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold = 20;
						//}
						//else
						//{
						//	if (vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold < 190)
						//		vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold = 190;
						//}
						vGlobalCurrentProduct->vInspection[TempLoopI]->vThreshold = vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold;

						ReportErrorMessage("Eval Insp: " + vGlobalCurrentProduct->vInspection[TempLoopI]->vName + " Average: " + dtoa(TempMinimumGoodAverage, 1) + 
							" SD: " + dtoa(TempMinimumGoodStandardDeviation,2) + " Min: " + dtoa(TempMinimumGoodIntensity,2), cWriteToLog, 0);

						ReportErrorMessage("Eval Insp: " + vGlobalCurrentProduct->vInspection[TempLoopI]->vName + " Threshold: " + dtoa(vGlobalCurrentProduct->vInspection[TempLoopI]->vThreshold,2),cAction,0);
					} //end of evaluated last image
				}
			} //end of contaminant types of inspections

			if ((vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cDensityInspection) || (vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cMissingLidInspection) ||
				(vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cUnderfillInspection) || (vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cOverfillInspection) ||
				(vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cCheckWeighInspection) || (vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cUnderfillByWeightInspection) ||
				(vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cOverfillByWeightInspection))
			{
				if (vLocalSystemData->vMinimumGoodDataFloat)
				{
					double *TempMinimumGoodDataPointer = vLocalSystemData->vMinimumGoodDataFloat + TempLoop; //place to store this data point in data collection array
			
					*TempMinimumGoodDataPointer = TempResult; //store this images lowest good threshold in data array
					ReportErrorMessage("Eval Insp: " + dtoa(TempLoopI, 0) + "-" + vGlobalCurrentProduct->vInspection[TempLoopI]->vName + " Image: " + dtoa(TempLoop, 0) + " Eval Thresh: " + dtoa(*TempMinimumGoodDataPointer,2), cWriteToLog, 0);

					if (TempMinimumGoodIntensity > TempResult)
						TempMinimumGoodIntensity = TempResult;

					if (TempMaximumGoodIntensity < TempResult)
						TempMaximumGoodIntensity = TempResult;

					if (TempLoop == cNumberOfImagesToEvaluate - 1) //on last loop calculate Standard Deviation
					{
						TempMinimumGoodStandardDeviation = CalculateStandardDeviation(vLocalSystemData->vMinimumGoodDataFloat, cNumberOfImagesToEvaluate, &TempMinimumGoodAverage);
						if (TempMinimumGoodStandardDeviation < .5)
							TempMinimumGoodStandardDeviation = .5;

						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryMinimumValue = (float)TempMinimumGoodIntensity;
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryMaximumValue = (float)TempMaximumGoodIntensity;
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryMinimumGood = (float)TempMinimumGoodIntensity;
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryMinimumGoodAverage = (float)TempMinimumGoodAverage;
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryMinimumGoodStandardDeviation = (float)TempMinimumGoodStandardDeviation;
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold = 0;
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryMinimumGoodCount = cNumberOfImagesToEvaluate;
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryMaximumBad = (float)TempMaximumGoodIntensity;
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryDensityAutoThresholdUpper = 0;
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryDensityAutoThresholdLower = 0;

						if (TempMinimumGoodAverage > (5 * TempMinimumGoodStandardDeviation))
							vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold = (BYTE)(TempMinimumGoodAverage - (5 * TempMinimumGoodStandardDeviation));

						if (vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold > 240)
							vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold = 240;
						if (vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold < 190)
							vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold = 190;

						vGlobalCurrentProduct->vInspection[TempLoopI]->vThreshold = vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold;

						if ((vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cDensityInspection) || (vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cOverfillInspection) ||
							(vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cCheckWeighInspection) || (vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cOverfillByWeightInspection))
						{
							vGlobalCurrentProduct->vInspection[TempLoopI]->SetDensityLowerThreshold((float)(TempMinimumGoodAverage - (5 * TempMinimumGoodStandardDeviation)));
							vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryDensityAutoThresholdLower = vGlobalCurrentProduct->vInspection[TempLoopI]->vDensityThresholdLower;

							ReportErrorMessage("Auto Evaluate Inspection: " + vGlobalCurrentProduct->vInspection[TempLoopI]->vName + " Threshold Set To: " + 
								dtoa(vGlobalCurrentProduct->vInspection[TempLoopI]->vDensityThresholdLower,2),cAction,0);
							ReportErrorMessage("Auto Evaluate Inspection: " + vGlobalCurrentProduct->vInspection[TempLoopI]->vName + " Ave: " + dtoa(TempMinimumGoodAverage,2) + " SD: " + dtoa(TempMinimumGoodStandardDeviation,2),cAction,0);
						}
						else
						if ((vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cDensityInspection) || (vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cMissingLidInspection) ||
							(vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cUnderfillInspection) || (vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cCheckWeighInspection) || 
							(vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cUnderfillByWeightInspection))
						{
							vGlobalCurrentProduct->vInspection[TempLoopI]->vDensityThresholdUpper = (float)(TempMinimumGoodAverage + (5 * TempMinimumGoodStandardDeviation));
							vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryDensityAutoThresholdUpper = vGlobalCurrentProduct->vInspection[TempLoopI]->vDensityThresholdUpper;

							ReportErrorMessage("Auto Evaluate Inspection: " + vGlobalCurrentProduct->vInspection[TempLoopI]->vName + " Threshold Set To: " + 
								dtoa(vGlobalCurrentProduct->vInspection[TempLoopI]->vDensityThresholdUpper,2),cAction,0);
							ReportErrorMessage("Auto Evaluate Inspection: " + vGlobalCurrentProduct->vInspection[TempLoopI]->vName + " Ave: " + dtoa(TempMinimumGoodAverage,2) + " SD: " + dtoa(TempMinimumGoodStandardDeviation,2),cAction,0);
						}
					}//end of just evaluated last image
				}
			} //end of missing lid or under fill types of inspections
			if ((vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cItemCountInspection) || (vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cSizeInspection) || (vGlobalCurrentProduct->vInspection[TempLoopI]->vInspectionType == cMissingLidInspectionOnSize))
			{
				if (vLocalSystemData->vMinimumGoodDataFloat)
				{
					vLocalSystemData->vITIPCDig->GetInspectionROIBounds(vGlobalCurrentProduct->vInspection[TempLoopI], &TempBottom, &TempLeft, &TempHeight, &TempWidth, 0, vLocalConfigurationData->vReferenceROIsToEdges, 0);
					int TempReferenceAdjust = TempLeft - vGlobalCurrentProduct->vReferenceLeft;
					if (TempLoop == 0) //Calculate Threshold on first image only
					{
						if (vGlobalCurrentProduct->vSetupHistoryAverageDensity)
						{

							BYTE TempDarkestPixelInROI = vLocalSystemData->vITIPCDig->FindDarkestPixelInROI(vGlobalCurrentProduct->vInspection[TempLoopI], vLocalSystemData->vQuickLearnImages[TempLoop], 
								TempReferenceAdjust, vLocalConfigurationData->vReferenceROIsToEdges);

							if (vGlobalCurrentProduct->vSetupHistoryAverageDensity > TempDarkestPixelInROI)
								vGlobalCurrentProduct->vInspection[TempLoopI]->vThreshold = (BYTE)(TempDarkestPixelInROI + ((vGlobalCurrentProduct->vSetupHistoryAverageDensity - TempDarkestPixelInROI) * 0.35));
							else
								vGlobalCurrentProduct->vInspection[TempLoopI]->vThreshold = (BYTE)(vGlobalCurrentProduct->vSetupHistoryAverageDensity * 0.71);
						}
						else
							vGlobalCurrentProduct->vInspection[TempLoopI]->vThreshold = 90;

						if (vGlobalCurrentProduct->vInspection[TempLoopI]->vThreshold > 240)
							vGlobalCurrentProduct->vInspection[TempLoopI]->vThreshold = 90;
						if (vGlobalCurrentProduct->vInspection[TempLoopI]->vThreshold < 10)
							vGlobalCurrentProduct->vInspection[TempLoopI]->vThreshold = 90;
					} //end of first image only calculate the Threshold

					double *TempMinimumGoodDataPointer = vLocalSystemData->vMinimumGoodDataFloat + TempLoop; //place to store this data point in data collection array
			
					*TempMinimumGoodDataPointer = vLocalSystemData->vITIPCDig->FindNumberOfPixelsBelowThreshold(vGlobalCurrentProduct->vInspection[TempLoopI], vLocalSystemData->vQuickLearnImages[TempLoop], 
								TempReferenceAdjust, vLocalConfigurationData->vReferenceROIsToEdges); //store this images number of pixels darker than threshold

					ReportErrorMessage("Eval Insp: " + dtoa(TempLoopI, 0) + "-" + vGlobalCurrentProduct->vInspection[TempLoopI]->vName + " Image: " + dtoa(TempLoop, 0) + " Eval Thresh: " + dtoa(vGlobalCurrentProduct->vInspection[TempLoopI]->vThreshold,0) +
						", Size: " + dtoa(*TempMinimumGoodDataPointer, 0), cWriteToLog, 0);

					if (TempMinimumGoodIntensity > *TempMinimumGoodDataPointer)
						TempMinimumGoodIntensity = *TempMinimumGoodDataPointer;

					if (TempMaximumGoodIntensity < *TempMinimumGoodDataPointer)
						TempMaximumGoodIntensity = *TempMinimumGoodDataPointer;

					if (TempLoop == cNumberOfImagesToEvaluate - 1) //on last loop calculate Standard Deviation
					{
						TempMinimumGoodStandardDeviation = CalculateStandardDeviation(vLocalSystemData->vMinimumGoodDataFloat, cNumberOfImagesToEvaluate, &TempMinimumGoodAverage);
						if (TempMinimumGoodStandardDeviation < .5)
							TempMinimumGoodStandardDeviation = .5;
						if (TempMinimumGoodStandardDeviation > 100)
							TempMinimumGoodStandardDeviation = 100;
						if (TempMinimumGoodStandardDeviation > TempMinimumGoodIntensity)
							TempMinimumGoodStandardDeviation = TempMinimumGoodIntensity / 2;

						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryMinimumValue = (float)TempMinimumGoodIntensity;  //this statisitics are on the size calculated at the Threshold for the images
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryMaximumValue = (float)TempMaximumGoodIntensity;
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryMinimumGood = (float)TempMinimumGoodIntensity;
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryMinimumGoodAverage = (float)TempMinimumGoodAverage;
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryMinimumGoodStandardDeviation = (float)TempMinimumGoodStandardDeviation;
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryAutoThreshold = vGlobalCurrentProduct->vInspection[TempLoopI]->vThreshold;
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryMinimumGoodCount = cNumberOfImagesToEvaluate;
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryMaximumBad = (float)TempMaximumGoodIntensity;
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryDensityAutoThresholdUpper = 0;
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryDensityAutoThresholdLower = 0;

						vGlobalCurrentProduct->vInspection[TempLoopI]->vSize = (WORD)(TempMinimumGoodIntensity / 4);
						vGlobalCurrentProduct->vInspection[TempLoopI]->vSetupHistoryDensityAutoThresholdUpper = (float)vGlobalCurrentProduct->vInspection[TempLoopI]->vSize;

						ReportErrorMessage("Auto Evaluate Inspection: " + vGlobalCurrentProduct->vInspection[TempLoopI]->vName + " Threshold Set To: " + 
							dtoa(vGlobalCurrentProduct->vInspection[TempLoopI]->vThreshold,2),cAction,0);
						ReportErrorMessage("Auto Evaluate Inspection: " + vGlobalCurrentProduct->vInspection[TempLoopI]->vName + " Size Set To: " + 
							dtoa(vGlobalCurrentProduct->vInspection[TempLoopI]->vSize,2),cAction,0);
						ReportErrorMessage("Auto Evaluate Inspection: " + vGlobalCurrentProduct->vInspection[TempLoopI]->vName + " Min: " + dtoa(TempMinimumGoodIntensity,2) + " SD: " + dtoa(TempMinimumGoodStandardDeviation,2),cAction,0);
					}//end of just evaluated last image
				}
			} //end of missing lid or under fill types of inspections
		} //end of For Each Image to Evaluate
	} //end of For Each Inspection
	
	if ((TempScratchImage) && (TempScratchImage->pix))
		im_delete(TempScratchImage);

	if ((TempWorkingImage) && (TempWorkingImage->pix))
		im_delete(TempWorkingImage);

	if (vLocalSystemData->vMinimumGoodData)
	{
		free(vLocalSystemData->vMinimumGoodData);
		vLocalSystemData->vMinimumGoodData = NULL;
	}

	if (vLocalSystemData->vMinimumGoodDataFloat)
	{
		free(vLocalSystemData->vMinimumGoodDataFloat);
		vLocalSystemData->vMinimumGoodDataFloat = NULL;
	}

	vLocalSystemData->vQuickLearnImageIndex = 0;

	//free all the image memory
	for (DWORD TempLoop = 0; TempLoop < cNumberOfImagesToEvaluate; TempLoop++)
	{
		if (vLocalSystemData->vQuickLearnImages[TempLoop])
		{
			free(vLocalSystemData->vQuickLearnImages[TempLoop]);
			vLocalSystemData->vQuickLearnImages[TempLoop] = NULL;
		}
		if (vLocalSystemData->vQuickLearnWorkingImages[TempLoop])
		{
			free(vLocalSystemData->vQuickLearnWorkingImages[TempLoop]);
			vLocalSystemData->vQuickLearnWorkingImages[TempLoop] = NULL;
		}
	}

	ReportErrorMessage("Finished processing Evaluate Images",cAction,0);

	vGlobalProcessEvaluateImagesThread = NULL;

	::PostThreadMessage(vGlobalCallingThreadID,	cEvaluateImagesProcessingFinishedMessage, 0,0);

	return 0;
}
