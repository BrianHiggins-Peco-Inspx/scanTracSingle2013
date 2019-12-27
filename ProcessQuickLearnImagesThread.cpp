//ScanTrac Side View Source File
// ProcessQuickLearnImagesThread.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "scantrac.h"
#include "ProcessQuickLearnImagesThread.h"
#include "ImageAquisitionThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CImageAquisitionThread *vGlobalImageAquisitionThread;
extern BYTE vGlobalQuickLearnDilateTimes;
extern CString vGlobalCurrentDirectory;
extern CProduct *vGlobalCurrentProduct;
extern CProcessQuickLearnImagesThread *vGlobalProcessQuickLearnImagesThread;
/////////////////////////////////////////////////////////////////////////////
// CProcessQuickLearnImagesThread

IMPLEMENT_DYNCREATE(CProcessQuickLearnImagesThread, CWinThread)

CProcessQuickLearnImagesThread::CProcessQuickLearnImagesThread()
{
	vLocalSystemData = NULL;
	vLocalConfigurationData = NULL;
	vMainWindowPointer = NULL;
}

CProcessQuickLearnImagesThread::~CProcessQuickLearnImagesThread()
{
}

BOOL CProcessQuickLearnImagesThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return true;
}

int CProcessQuickLearnImagesThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CProcessQuickLearnImagesThread, CWinThread)
	//{{AFX_MSG_MAP(CProcessQuickLearnImagesThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessQuickLearnImagesThread message handlers

int CProcessQuickLearnImagesThread::Run() 
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Finished Quick Learn at frame:" + vLocalSystemData->vFrameCountString,cDebugMessage);

	ReportErrorMessage("Started processing Quick Learn 2: " + dtoa(vLocalConfigurationData->vNumberOfQuickLearnImagesToTake,0),cAction,0);


	if (vLocalSystemData->vInAutoSetup)  //backup files if in autosetup so can abort if needed
		vLocalSystemData->vITIPCDig->RenameLearnFilesToBackup(vGlobalCurrentProduct);

	Image *TempScratchImage = NULL;
	if (ThereIsEnoughMemory(vLocalSystemData->vITIPCDig->vOriginalBufferSize + sizeof(Image), "TempScratchImage"))
		TempScratchImage = im_create(IM_BYTE, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vAquisitionSizeY);

	Image *TempScratchImage2 = NULL;
	if (ThereIsEnoughMemory(vLocalSystemData->vITIPCDig->vOriginalBufferSize + sizeof(Image), "TempScratchImage2"))
		TempScratchImage2 = im_create(IM_BYTE, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vAquisitionSizeY);

	//calculate Reference so can check width is OK, or put in reference info if have none
	vLocalSystemData->CreateReferenceSamples(vLocalSystemData->vQuickLearnImageIndex);
	if (vLocalSystemData->vLeftReferenceSample)
	if (vLocalSystemData->vBottomReferenceSample)
	if (vLocalSystemData->vRightReferenceSample)
	if (vLocalSystemData->vTopReferenceSample)
	if ((TempScratchImage) && (TempScratchImage->pix))
	if ((TempScratchImage2) && (TempScratchImage2->pix))
	{
		WORD TempTop = 0;
		WORD TempBottom = 0;
		WORD TempHeight = 0;
		WORD TempWidth = 0;
		WORD TempLeft = 0;
		WORD TempRight = 0;
		
		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("Quick Learn Index:" + dtoa(vLocalSystemData->vQuickLearnImageIndex,0),cDebugMessage);

		for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vQuickLearnImageIndex; TempLoop++)
		if (vLocalSystemData->vQuickLearnImages[TempLoop])
		{
			// for testing save original structure to compare with dilated structure
			// test of new quick learn with expanded structure
			CString TempString1 = "-QuickLearnImage-";
			TempString1 =  *vGlobalCurrentProduct->GetProductName() + TempString1 + dtoa(TempLoop,0);
			TempString1 = "C:\\ScanTracCalibrationImages\\" + TempString1;
			TempString1 = TempString1 + ".BMP";

			vGlobalImageAquisitionThread->WriteImageToHardDisk(vLocalSystemData->vQuickLearnImages[TempLoop], TempString1, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

			if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
			{
				TempTop = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1;
				TempLeft = 0;
				TempRight = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1;
				TempBottom = 0;
				TempHeight = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1;
				TempWidth = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1;
			}
			else
			{
				vLocalSystemData->vITIPCDig->FindContainerDimensions(vLocalSystemData->vQuickLearnImages[TempLoop], TempScratchImage, &TempTop, &TempBottom,&TempRight,&TempLeft,&TempHeight,&TempWidth, false, 
					10000); //vLocalConfigurationData->vShapeHandlingWidthTolerancePixels); //10000 = Finding new reference so ignore old one
			}

			double *TempTopSample = vLocalSystemData->vTopReferenceSample + TempLoop;
			double *TempBottomSample = vLocalSystemData->vBottomReferenceSample + TempLoop;
			double *TempRightSample = vLocalSystemData->vRightReferenceSample + TempLoop;
			double *TempLeftSample = vLocalSystemData->vLeftReferenceSample + TempLoop;

			*TempRightSample = (float)TempRight;
			*TempLeftSample = (float)TempLeft;
			*TempTopSample = (float)TempTop;
			*TempBottomSample = (float)TempBottom;
		}
		//if (vLocalSystemData->vLogFile.vLogSerialData)
		//	vLocalSystemData->vLogFile.WriteToLogFile("Quick Learn Start Find Right Reference---",cDebugMessage);
		vLocalSystemData->vITIPCDig->vReferenceRightStandardDeviation = CalculateStandardDeviation(vLocalSystemData->vRightReferenceSample, vLocalSystemData->vQuickLearnImageIndex, &vLocalSystemData->vITIPCDig->vReferenceRightAverage);
			
		if (vLocalSystemData->vLogFile.vLogSerialData)
		{
			vLocalSystemData->vLogFile.WriteToLogFile("Quick Learn Found Right Reference: " + dtoa(vLocalSystemData->vITIPCDig->vReferenceRightAverage, 2)+ 
				", SD: " + dtoa(vLocalSystemData->vITIPCDig->vReferenceRightStandardDeviation,2),cDebugMessage);
		}

		//find variance and average of Left side samples
		vLocalSystemData->vITIPCDig->vReferenceLeftStandardDeviation = CalculateStandardDeviation(vLocalSystemData->vLeftReferenceSample, vLocalSystemData->vQuickLearnImageIndex, &vLocalSystemData->vITIPCDig->vReferenceLeftAverage);
		//Sleep(10);

		if (vLocalSystemData->vLogFile.vLogSerialData)
		{
			vLocalSystemData->vLogFile.WriteToLogFile("Quick Learn Found Left Reference: " + dtoa(vLocalSystemData->vITIPCDig->vReferenceLeftAverage, 2) + 
				", SD: " + dtoa(vLocalSystemData->vITIPCDig->vReferenceLeftStandardDeviation,2),cDebugMessage);
		}

		//find SD and average of Top samples
		vLocalSystemData->vITIPCDig->vReferenceTopStandardDeviation = CalculateStandardDeviation(vLocalSystemData->vTopReferenceSample, vLocalSystemData->vQuickLearnImageIndex, &vLocalSystemData->vITIPCDig->vReferenceTopAverage);

		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("Quick Learn Found Top Reference: " + dtoa(vLocalSystemData->vITIPCDig->vReferenceTopAverage, 2) + ", " +
				+ dtoa(vLocalSystemData->vITIPCDig->vReferenceTopStandardDeviation), cDebugMessage);

		//find SD and average of Bottom samples
		vLocalSystemData->vITIPCDig->vReferenceBottomStandardDeviation = CalculateStandardDeviation(vLocalSystemData->vBottomReferenceSample, vLocalSystemData->vQuickLearnImageIndex, &vLocalSystemData->vITIPCDig->vReferenceBottomAverage);

		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("Quick Learn Found Bottom Reference: " + dtoa(vLocalSystemData->vITIPCDig->vReferenceBottomAverage, 2) + ", " +
				+ dtoa(vLocalSystemData->vITIPCDig->vReferenceBottomStandardDeviation), cDebugMessage);

		TempWidth = (int)(vLocalSystemData->vITIPCDig->vReferenceRightAverage - vLocalSystemData->vITIPCDig->vReferenceLeftAverage);
		int TempWidthDelta = (int)vGlobalCurrentProduct->vReferenceWidth - TempWidth;
			
		if ((vGlobalCurrentProduct->vReferenceRight == 0) || (vGlobalCurrentProduct->vReferenceTop == 0) || (TempWidthDelta > 6) || (TempWidthDelta < -6) || (vLocalSystemData->vChangedSizeOfProduct))
		{
			int TempDifferenceInRightReference = (int)(vGlobalCurrentProduct->vReferenceRight - vLocalSystemData->vITIPCDig->vReferenceRightAverage);
			int TempDifferenceInLeftReference = (int)(vGlobalCurrentProduct->vReferenceLeft - vLocalSystemData->vITIPCDig->vReferenceLeftAverage);
			int TempOriginalRightReference = vGlobalCurrentProduct->vReferenceRight;

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("Use/Save new Reference Values, Width: " + dtoa(TempWidth), cDebugMessage);

			vGlobalCurrentProduct->vReferenceTop = (WORD)vLocalSystemData->vITIPCDig->vReferenceTopAverage;
			vGlobalCurrentProduct->vReferenceBottom = (WORD)vLocalSystemData->vITIPCDig->vReferenceBottomAverage;
			vGlobalCurrentProduct->vReferenceRight = (WORD)vLocalSystemData->vITIPCDig->vReferenceRightAverage;
			vGlobalCurrentProduct->vReferenceLeft = (WORD)vLocalSystemData->vITIPCDig->vReferenceLeftAverage;
			vGlobalCurrentProduct->vReferenceWidth = vGlobalCurrentProduct->vReferenceRight - vGlobalCurrentProduct->vReferenceLeft;
			//correct all inspection ROIs since moved reference
	//		if (TempOriginalRightReference > 10)
	//		for (BYTE TempLoopA = 0; TempLoopA < vGlobalCurrentProduct->vNumberOfInspections; TempLoopA++)
	//		if (vGlobalCurrentProduct->vInspection[TempLoopA])
	//		{
	//			ReportErrorMessage(vGlobalCurrentProduct->vInspection[TempLoopA]->vName + " Inspection Left ROI Was: " + dtoa(vGlobalCurrentProduct->vInspection[TempLoopA]->vROIPixelLeft, 0), cAction, 0);

	//			vGlobalCurrentProduct->vInspection[TempLoopA]->SetROILeft(vGlobalCurrentProduct->vInspection[TempLoopA]->vROILeft + 
	//				(TempDifferenceInLeftReference / vGlobalPixelsPerUnit / vGlobalCurrentProduct->vOverScanMultiplier), vGlobalCurrentProduct->vOverScanMultiplier);

	//			ReportErrorMessage(vGlobalCurrentProduct->vInspection[TempLoopA]->vName + " Inspection Left ROI Is: " + dtoa(vGlobalCurrentProduct->vInspection[TempLoopA]->vROIPixelLeft, 0), cAction, 0);
	//			ReportErrorMessage(vGlobalCurrentProduct->vInspection[TempLoopA]->vName + " Inspection Right ROI Was: " + dtoa(vGlobalCurrentProduct->vInspection[TempLoopA]->vROIPixelRight, 0), cAction, 0);

	//			vGlobalCurrentProduct->vInspection[TempLoopA]->SetROIRight(vGlobalCurrentProduct->vInspection[TempLoopA]->vROIRight + 
	//				(TempDifferenceInRightReference / vGlobalPixelsPerUnit / vGlobalCurrentProduct->vOverScanMultiplier), vGlobalCurrentProduct->vOverScanMultiplier);

	//			ReportErrorMessage(vGlobalCurrentProduct->vInspection[TempLoopA]->vName + " Inspection Right ROI Is: " + dtoa(vGlobalCurrentProduct->vInspection[TempLoopA]->vROIPixelRight, 0), cAction, 0);
	//		}
	//		if (!vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
	//		if (vGlobalCurrentProduct->vDriftDensityInspection)
	//		{
	//			ReportErrorMessage("Product Density Left ROI Was: " + dtoa(vGlobalCurrentProduct->vDriftDensityInspection->vROIPixelLeft, 0), cAction, 0);

	//			vGlobalCurrentProduct->vDriftDensityInspection->SetROILeft(vGlobalCurrentProduct->vDriftDensityInspection->vROILeft + 
	//				(TempDifferenceInLeftReference / vGlobalPixelsPerUnit / vGlobalCurrentProduct->vOverScanMultiplier), vGlobalCurrentProduct->vOverScanMultiplier);

	//			ReportErrorMessage("Product Density Left ROI Is: " + dtoa(vGlobalCurrentProduct->vDriftDensityInspection->vROIPixelLeft, 0), cAction, 0);
	//			ReportErrorMessage("Product Density Right ROI Was: " + dtoa(vGlobalCurrentProduct->vDriftDensityInspection->vROIPixelRight, 0), cAction, 0);

	//			vGlobalCurrentProduct->vDriftDensityInspection->SetROIRight(vGlobalCurrentProduct->vDriftDensityInspection->vROIRight + 
	//				(TempDifferenceInRightReference / vGlobalPixelsPerUnit / vGlobalCurrentProduct->vOverScanMultiplier), vGlobalCurrentProduct->vOverScanMultiplier);

	//			ReportErrorMessage("Product Density Right ROI Is: " + dtoa(vGlobalCurrentProduct->vDriftDensityInspection->vROIPixelRight, 0), cAction, 0);
	//		}
	//		if (vLocalSystemData->vLogFile.vLogSerialData)
	//			vLocalSystemData->vLogFile.WriteToLogFile("Set Reference F Right: " + dtoa(vGlobalCurrentProduct->vReferenceRight, 2) + ", Left: " + dtoa(vGlobalCurrentProduct->vReferenceLeft, 2) +
	//					", Width: " + dtoa(vGlobalCurrentProduct->vReferenceWidth, 2), cDebugMessage);
		}
		else
		if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("Ignore new Reference Values, they are close to old values", cDebugMessage);
	}

	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Quick Learn Find Average",cDebugMessage);

	//calculate the Average
	bool TempMemoryOK = true;

	vLocalSystemData->vITIPCDig->vAverageCount = 0;
	vLocalSystemData->vITIPCDig->MakeAverageImageBuffer();

	//Sleep(10);

	if (vLocalSystemData->vITIPCDig->vAverageImage)
	{
		DWORD *TempPointerImageAverage;
		BYTE *TempPointerAquisitionImage;
		//sum up all the images taken into one sum image
		for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vQuickLearnImageIndex; TempLoop++)
		if (TempMemoryOK)
		{
			if (vLocalSystemData->vQuickLearnWorkingImages[TempLoop])
			{
				free(vLocalSystemData->vQuickLearnWorkingImages[TempLoop]);
				vLocalSystemData->vQuickLearnWorkingImages[TempLoop] = NULL;
			}
			if (ThereIsEnoughMemory(vLocalSystemData->vITIPCDig->vOriginalBufferSize,"Quick Learn Working Image"))
				vLocalSystemData->vQuickLearnWorkingImages[TempLoop] =
				(BYTE *)malloc(vLocalSystemData->vITIPCDig->vOriginalBufferSize);
			if (!vLocalSystemData->vQuickLearnWorkingImages[TempLoop])
				TempMemoryOK = false;
			else
			{
				//todo: reference image
				int TempReferenceSideAdjustAmount = 0;
				WORD TempTop = 0;
				WORD TempBottom = 0;
				WORD TempHeight = 0;
				WORD TempWidth = 0;
				WORD TempLeft = 0;
				WORD TempRight = 0;
				//if (vLocalConfigurationData->vReferenceImagePosition)
				{
					if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) || //Continuous Feed, or a Pipeline
						(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
						(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)) //Continuous Feed, or a Pipeline or TD
					{
						TempTop = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1;
						TempLeft = 0;
						TempRight = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1;
						TempBottom = 0;
						TempHeight = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1;
						TempWidth = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1;
					}
					else
					{
						vLocalSystemData->vITIPCDig->FindContainerDimensions(vLocalSystemData->vQuickLearnImages[TempLoop], TempScratchImage, &TempTop, &TempBottom, &TempRight, &TempLeft, &TempHeight, &TempWidth, false, 
							vLocalConfigurationData->vShapeHandlingWidthTolerancePixels);

						TempReferenceSideAdjustAmount = vLocalSystemData->vITIPCDig->CalculateSideToSideReference(vGlobalCurrentProduct->vReferenceRight, vGlobalCurrentProduct->vReferenceLeft, &TempRight, &TempLeft);
					}
				}
				//Only images within reference tolerance and width tolerance were saved
				
				if (TempReferenceSideAdjustAmount)
				{
					BYTE *TempBuffer1 = NULL;
					if (ThereIsEnoughMemory(vLocalSystemData->vITIPCDig->vOriginalBufferSize, "Add To Learn Buffer"))
						TempBuffer1 = (BYTE*)malloc(vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					if (TempBuffer1)
					{
						vLocalSystemData->vITIPCDig->MakeReferenceImageAdjustment(
							vLocalSystemData->vQuickLearnImages[TempLoop], TempBuffer1,
							0,
							TempReferenceSideAdjustAmount);
						free(TempBuffer1);
					}
				}
			//	CopyMemory(
			//		vLocalSystemData->vQuickLearnWorkingImages[TempLoop],
			//		vLocalSystemData->vQuickLearnImages[TempLoop],
			//		vLocalSystemData->vITIPCDig->vOriginalBufferSize);

				TempPointerImageAverage = vLocalSystemData->vITIPCDig->vAverageImage;
				TempPointerAquisitionImage = vLocalSystemData->vQuickLearnImages[TempLoop];
				if (TempPointerAquisitionImage)
				{
					vLocalSystemData->vITIPCDig->vAverageCount++;
					for (DWORD TempLoopP = 0; TempLoopP < vLocalSystemData->vITIPCDig->vOriginalBufferSize; 
						TempLoopP++)
					{//add pixel to Average  
						*TempPointerImageAverage = *TempPointerImageAverage + *TempPointerAquisitionImage;  
						TempPointerImageAverage++;
						TempPointerAquisitionImage++;
					}
				}
			}
			//if (!(TempLoop % 20)) Sleep(10); //new
		}
/*
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nFinished Average Quick Learn Images";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
*/
	}
	if (TempMemoryOK)
	{
		if (vLocalSystemData->vLogFile.vLogSerialData)
		{
			vLocalSystemData->vLogFile.WriteToLogFile("Make Product Average2, Average Count: " +
				dtoa(vLocalSystemData->vITIPCDig->vAverageCount,0),cDebugMessage);
		}
		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("Quick Learn Save Average",cDebugMessage);
			
		DWORD TempAverageCount = vLocalSystemData->vITIPCDig->vAverageCount;
		if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
		{
			vLocalSystemData->vITIPCDig->MakeProductAverageImage();
			vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();		
		}
		//Sleep(10);
		vLocalSystemData->vITIPCDig->SaveAverageImageToFile(vGlobalCurrentProduct);
		vGlobalCurrentProduct->vSetupHistoryAverageDensity = 0;

		if (vGlobalCurrentProduct->vHistogramThreshold > 0)
		if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
		{
			vLocalSystemData->vITIPCDig->SaveProcess4ImageData(vGlobalCurrentProduct);
			vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();		
		}
		vGlobalCurrentProduct->vSetupHistoryAverageCount = TempAverageCount;
		vLocalSystemData->vITIPCDig->CalculateAverageDriftDensity(vGlobalCurrentProduct);
		vLocalSystemData->vIndexForDriftDensityOfLastNImages = 0;
		if (vGlobalCurrentProduct->vSetupHistoryAverageDensity > 255)
			vGlobalCurrentProduct->vSetupHistoryAverageDensity = 0;
		vGlobalCurrentProduct->vSetupHistoryContainerTriggerStandardDeviation = vLocalSystemData->vBodyTriggerWidthStandardDeviation;
		//Sleep(10);
		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("Quick Learn Make Structure",cDebugMessage);

		vLocalSystemData->vITIPCDig->DeleteUndoStructureImages(vGlobalCurrentProduct);
		vLocalSystemData->vITIPCDig->MakeStructureImageBuffer();

		//add Contaminant2 structure
		if (vLocalSystemData->vITIPCDig->vContaminant2StructureImage != NULL)
		{
			delete vLocalSystemData->vITIPCDig->vContaminant2StructureImage;
			vLocalSystemData->vITIPCDig->vContaminant2StructureImage = NULL;
		}
		if (ThereIsEnoughMemory(sizeof(CImageData), "vContaminant2StructureImage"))
			vLocalSystemData->vITIPCDig->vContaminant2StructureImage = new CImageData;
		if (vLocalSystemData->vITIPCDig->vContaminant2StructureImage)
		{
			vLocalSystemData->vITIPCDig->vContaminant2StructureImage->vImageTypeString = "Product Process 2T";
			vLocalSystemData->vITIPCDig->vContaminant2StructureImage->vImageType = cDataImageType;
			vLocalSystemData->vITIPCDig->vContaminant2StructureImage->MakeGreyImage(vLocalSystemData->vITIPCDig->vOriginalBufferSize);

			//accumulate Contaminant2 structure
			if (vLocalSystemData->vITIPCDig->vContaminant2StructureImage->vGreyImage)
			{
				//statistical Structure2 method
				//clear the structure image data
				ZeroMemory(vLocalSystemData->vITIPCDig->vContaminant2StructureImage->vGreyImage,vLocalSystemData->vITIPCDig->vOriginalBufferSize);
				BYTE *TempImageSourcePointer = NULL;
				BYTE *TempImageDestinationPointer = NULL;
				double TempAverage = 0;
				double TempStandardDeviation = 0;

				//calculate contaminant 2 structure
				//allocate memory to hold the same pixel location from every image collected
				BYTE *TempSinglePixelData = NULL;
				if (ThereIsEnoughMemory(vLocalSystemData->vQuickLearnImageIndex * sizeof(BYTE) * 2, "Temp Image Buffer"))
					TempSinglePixelData = (BYTE *)malloc(vLocalSystemData->vQuickLearnImageIndex * sizeof(BYTE) * 2); //each sample has a reflection of it for Std Dev calculation

				if (TempSinglePixelData)
				{
					for (DWORD TempPixelLoop = 0; TempPixelLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempPixelLoop++)
					{
						//for this pixel, go through every image and copy to array to find Ave and SD.
						BYTE *TempDestination = TempSinglePixelData;
						for (DWORD TempLoopImage = 0; TempLoopImage < vLocalSystemData->vQuickLearnImageIndex; TempLoopImage++)
						if (vLocalSystemData->vQuickLearnImages[TempLoopImage])
						{
							TempImageSourcePointer = vLocalSystemData->vQuickLearnImages[TempLoopImage] + TempPixelLoop; //get the pointer to the current pixel location in this image
							*TempDestination = *TempImageSourcePointer;
							TempDestination++;
						}
						TempStandardDeviation = CalculateStandardDeviation(TempSinglePixelData, vLocalSystemData->vQuickLearnImageIndex, &TempAverage);

						//calculate the structure value for this pixel and put in structure array
						TempImageDestinationPointer = vLocalSystemData->vITIPCDig->vContaminant2StructureImage->vGreyImage + TempPixelLoop;//get the pointer to the final structure image location for this pixel
							
						if (TempAverage > (vLocalConfigurationData->vQuickLearnNumberOfStructureDeltas * TempStandardDeviation))
							*TempImageDestinationPointer = (BYTE)(TempAverage - (vLocalConfigurationData->vQuickLearnNumberOfStructureDeltas * TempStandardDeviation));
						else
							*TempImageDestinationPointer = 0;
						//if (!(TempPixelLoop % 1000)) Sleep(10); //new
					}
				}
				if (TempSinglePixelData)
					free(TempSinglePixelData);
				//Non statistical Contaminant2 Structure just found darkest value of every pixel over all learned images
				/*
				for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vQuickLearnImageIndex; TempLoop++)
				if (vLocalSystemData->vQuickLearnImages[TempLoop])
				{
					BYTE *TempPointerImageStructure = 
						vLocalSystemData->vITIPCDig->vContaminant2StructureImage->vGreyImage;
					BYTE *TempPointerAquisitionImage = vLocalSystemData->vQuickLearnImages[TempLoop];

					for (DWORD TempLoopP = 0; TempLoopP < 
						vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoopP++)
					{//Take Minimum pixel to Structure
						if (*TempPointerImageStructure > *TempPointerAquisitionImage)
							*TempPointerImageStructure = *TempPointerAquisitionImage;  
						TempPointerImageStructure++;
						TempPointerAquisitionImage++;  
					}
					if (!(TempLoop % 5)) Sleep(10);
				}
				*/
				if (vGlobalQuickLearnDilateTimes)
				{
					CopyMemory(TempScratchImage->pix, vLocalSystemData->vITIPCDig->vContaminant2StructureImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

					//mvt_erode(TempScratchImage, TempScratchImage2);
					mvt_erode_rect(TempScratchImage, vGlobalQuickLearnDilateTimes, vGlobalQuickLearnDilateTimes, EDGE_IGNORE);

					CopyMemory(vLocalSystemData->vITIPCDig->vContaminant2StructureImage->vGreyImage, TempScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
				}

				if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
				{ 
					vLocalSystemData->vITIPCDig->SaveContaminant2StructureImageToFile(vGlobalCurrentProduct,vLocalSystemData->vITIPCDig->vContaminant2StructureImage, true);
					ReportErrorMessage("Save Quick Learn For Product: " + vLocalSystemData->vCurrentProductName, cUserChanges,0);

					vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();		
				}
			}
		}

		//calculate the structure

		//new statistical structure method
		if (!vLocalSystemData->vITIPCDig->vProductAverageImage)
			ReportErrorMessage("Quick Learn Did Not Create Average Image", cEMailInspx,32000);
		else
		if (!vLocalSystemData->vITIPCDig->vStructureImage)
			ReportErrorMessage("Quick Learn Could Not make Structure Buffer", cEMailInspx,32000);
		else
		{
			BYTE *TempSinglePixelData = NULL;
			if (ThereIsEnoughMemory(vLocalSystemData->vQuickLearnImageIndex * sizeof(BYTE) * 2, "Temp Image Buffer"))
				TempSinglePixelData = (BYTE *)malloc(vLocalSystemData->vQuickLearnImageIndex * sizeof(BYTE) * 2); //each sample has a reflection of it for Std Dev calculation

			if (TempSinglePixelData)
			{
				//for each image, subtract average
				for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vQuickLearnImageIndex; TempLoop++)
				if (vLocalSystemData->vQuickLearnImages[TempLoop])
				if (vLocalSystemData->vQuickLearnWorkingImages[TempLoop])
				{
					// vQuickLearnWorkingImages are referenced from doing average
					CopyMemory(vLocalSystemData->vQuickLearnWorkingImages[TempLoop], vLocalSystemData->vQuickLearnImages[TempLoop], vLocalSystemData->vITIPCDig->vOriginalBufferSize);

					//must subtract the average
					if (vLocalSystemData->vITIPCDig->vProductAverageImage)
						vLocalSystemData->vITIPCDig->SubtractProductAverageImage(vLocalSystemData->vQuickLearnWorkingImages[TempLoop], vLocalSystemData->vITIPCDig->vProductAverageImage);
					//if (!(TempLoop % 100)) Sleep(10); //new
				}

				BYTE *TempImageSourcePointer = NULL;
				BYTE *TempImageDestinationPointer = NULL;
				double TempAverage = 0;
				double TempStandardDeviation = 0;
				WORD TempNumberOfSamples = 0;

				//calculate contaminant structure
				if ((vLocalSystemData->vITIPCDig->vStructureImage) && (vLocalSystemData->vITIPCDig->vStructureImage->vGreyImage))
				for (DWORD TempPixelLoop = 0; TempPixelLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempPixelLoop++)
				{
					//for this pixel, go through every image and copy to array to find SD.
					TempNumberOfSamples = 0;
					TempImageDestinationPointer = TempSinglePixelData;
					for (DWORD TempLoopImage = 0; TempLoopImage < vLocalSystemData->vQuickLearnImageIndex; TempLoopImage++)
					if (vLocalSystemData->vQuickLearnWorkingImages[TempLoopImage])
					{
						TempImageSourcePointer = vLocalSystemData->vQuickLearnWorkingImages[TempLoopImage] + TempPixelLoop;
						if (*TempImageSourcePointer < 128)
						{
							//put in this sample
							*TempImageDestinationPointer++ = *TempImageSourcePointer;
							TempNumberOfSamples++;
							//put in a reflection of this sample for the other side of the bell curve
							*TempImageDestinationPointer++ = 256 - *TempImageSourcePointer;
							TempNumberOfSamples++;
						}
					}
					//find variance and average this pixel for every image
					TempImageDestinationPointer = vLocalSystemData->vITIPCDig->vStructureImage->vGreyImage + TempPixelLoop;//get pointer to this pixel in the structure image to set value
					if (TempNumberOfSamples == 0)
						*TempImageDestinationPointer = 128;
					else
					{
						TempStandardDeviation = CalculateStandardDeviation(TempSinglePixelData, TempNumberOfSamples, &TempAverage);

						if (TempStandardDeviation == 0)
							*TempImageDestinationPointer = 128;
						else
						{
							double TempDouble = 128.0;
								TempDouble = 128.0 - (vLocalConfigurationData->vQuickLearnNumberOfStructureDeltas * TempStandardDeviation);
							if (TempDouble > 0)
								*TempImageDestinationPointer = (BYTE)TempDouble;
							else
								*TempImageDestinationPointer = 0;
						}
					}
					//if (!(TempPixelLoop % 1000)) Sleep(10); //new
				}
				//calculate void structure
				if (vLocalConfigurationData->vAllowVoidInspection)
				{
					//must take reference and subtract void average
					for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vQuickLearnImageIndex; TempLoop++)
					if (vLocalSystemData->vQuickLearnImages[TempLoop])
					if (vLocalSystemData->vQuickLearnWorkingImages[TempLoop])
					{
						CopyMemory(vLocalSystemData->vQuickLearnWorkingImages[TempLoop], vLocalSystemData->vQuickLearnImages[TempLoop], vLocalSystemData->vITIPCDig->vOriginalBufferSize);

						//reference image
						int TempReferenceSideAdjustAmount = 0;
						WORD TempTop = 0;
						WORD TempBottom = 0;
						WORD TempHeight = 0;
						WORD TempWidth = 0;
						WORD TempLeft = 0;
						WORD TempRight = 0;
						//if (vLocalConfigurationData->vReferenceImagePosition)
						{
							if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) || //Continuous Feed, or a Pipeline
								(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
								(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)) //Continuous Feed, or a Pipeline or TD
							{
								TempTop = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1;
								TempLeft = 0;
								TempRight = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1;
								TempBottom = 0;
								TempHeight = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1;
								TempWidth = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1;
							}
							else
							{
								vLocalSystemData->vITIPCDig->FindContainerDimensions(vLocalSystemData->vQuickLearnWorkingImages[TempLoop], TempScratchImage, &TempTop, &TempBottom, &TempRight, &TempLeft, &TempHeight,&TempWidth, false, 
									vLocalConfigurationData->vShapeHandlingWidthTolerancePixels);
								TempReferenceSideAdjustAmount = vLocalSystemData->vITIPCDig->CalculateSideToSideReference(vGlobalCurrentProduct->vReferenceRight, vGlobalCurrentProduct->vReferenceLeft, &TempRight, &TempLeft);
							}
						}
						//Only images within reference tolerance and width tolerance were saved
						if (TempReferenceSideAdjustAmount)
						{
							BYTE *TempBuffer1 = NULL;
							if (ThereIsEnoughMemory(vLocalSystemData->vITIPCDig->vOriginalBufferSize, "Add To Learn Buffer"))
								TempBuffer1 = (BYTE*)malloc(vLocalSystemData->vITIPCDig->vOriginalBufferSize);
							if (TempBuffer1)
							{
								vLocalSystemData->vITIPCDig->MakeReferenceImageAdjustment(vLocalSystemData->vQuickLearnWorkingImages[TempLoop], TempBuffer1, 0, TempReferenceSideAdjustAmount);
								free(TempBuffer1);
							}
						}
						//if (!(TempLoop % 20)) Sleep(10); //new
					}

					//for each image, subtract average
					for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vQuickLearnImageIndex; TempLoop++)
					if (vLocalSystemData->vQuickLearnWorkingImages[TempLoop])
					{ // vQuickLearnWorkingImages are referenced from doing average, must subtract the average
							vLocalSystemData->vITIPCDig->SubtractVoidProductAverageImage(vLocalSystemData->vQuickLearnWorkingImages[TempLoop]);
						//if (!(TempLoop % 20)) Sleep(10); //new
					}

					if ((vLocalSystemData->vITIPCDig->vVoidStructureImage) && (vLocalSystemData->vITIPCDig->vVoidStructureImage->vGreyImage))
					for (DWORD TempPixelLoop = 0; TempPixelLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempPixelLoop++)
					{
						//for this pixel, go through every image and copy to array to find SD.
						TempNumberOfSamples = 0;
						TempImageDestinationPointer = TempSinglePixelData;
						for (DWORD TempLoopImage = 0; TempLoopImage < vLocalSystemData->vQuickLearnImageIndex; TempLoopImage++)
						if (vLocalSystemData->vQuickLearnWorkingImages[TempLoopImage])
						{
							TempImageSourcePointer = vLocalSystemData->vQuickLearnWorkingImages[TempLoopImage] + TempPixelLoop;
							if (*TempImageSourcePointer < 128)
							{
								//put in this sample
								*TempImageDestinationPointer++ = 128 + *TempImageSourcePointer;
								TempNumberOfSamples++;
								//put in a reflection of this sample for the other side of the bell curve
								*TempImageDestinationPointer++ = 128 - *TempImageSourcePointer;
								TempNumberOfSamples++;
							}
						}
						//find variance and average this pixel for every image
						TempImageDestinationPointer = vLocalSystemData->vITIPCDig->vVoidStructureImage->vGreyImage + TempPixelLoop;
						if (TempNumberOfSamples == 0)
							*TempImageDestinationPointer = 128;
						else
						{
							TempStandardDeviation = CalculateStandardDeviation(TempSinglePixelData, TempNumberOfSamples, &TempAverage);
							//calculate the structure value for this pixel and put in structure array
							if (TempStandardDeviation == 0)
								*TempImageDestinationPointer = 128;
							else
							{
								double TempDouble = 128.0 - (vLocalConfigurationData->vQuickLearnNumberOfStructureDeltas * TempStandardDeviation);
								if (TempDouble < 0)
									*TempImageDestinationPointer = 0;
								else
									*TempImageDestinationPointer = (BYTE)TempDouble;
							}
						}
						//if (!(TempPixelLoop % 1000)) Sleep(10); //new
					}
				} //end of void enabled
			}
			else
				ReportErrorMessage("Quick Learn Could Not make Structure Working Buffers", cEMailInspx,32000);
			if (TempSinglePixelData)
				free(TempSinglePixelData);

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("Quick Learn Save Structure",cDebugMessage);

			//make final product structure image from accumulation
			if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
			{
				vLocalSystemData->vITIPCDig->vStructureCount = vLocalSystemData->vQuickLearnImageIndex;
				if (vLocalSystemData->vITIPCDig->MakeProductStructureImage())
				{
					vGlobalCurrentProduct->vSetupHistoryStructureCount = vLocalSystemData->vQuickLearnImageIndex;
					vGlobalCurrentProduct->vNeedToRelearn = 0;
				}

				
				// test of new quick learn with expanded structure
				//CString TempString1 = "=Process 2 before dilate";
				//TempString1 =  *vGlobalCurrentProduct->GetProductName() + TempString1;
				//if (!vLocalSystemData->vITIPCDig->vUsingLocalSetupDataFile)
				//	TempString1 = cSetupDataDirectory + TempString1;
				//else
				//	TempString1 = vGlobalCurrentDirectory + TempString1;

				//if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
				//{ 
				//	vLocalSystemData->vITIPCDig->SaveLearnFile(TempString1, vLocalSystemData->vITIPCDig->vProductStructureImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY,
				//			(WORD)vLocalSystemData->vITIPCDig->vBitsPerPixel, true);	//true make backup
				//	vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();		
				//}

				if (vGlobalQuickLearnDilateTimes)
				{
					CopyMemory(TempScratchImage->pix, vLocalSystemData->vITIPCDig->vProductStructureImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

					mvt_dilate_rect(TempScratchImage, vGlobalQuickLearnDilateTimes, vGlobalQuickLearnDilateTimes, EDGE_IGNORE);

					CopyMemory(vLocalSystemData->vITIPCDig->vProductStructureImage->vGreyImage, TempScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
				}
				vLocalSystemData->vITIPCDig->SaveStructureImageToFile(vGlobalCurrentProduct, true);
				vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();		
				//	if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
				//	{ 
				//		vLocalSystemData->vITIPCDig->SaveContaminant2StructureImageToFile(vGlobalCurrentProduct, vLocalSystemData->vITIPCDig->vContaminant2StructureImage, true);
				//		vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();		
				//	}

				//done making variance structure that has been dilated, now make Contamiant2, or ContaminantT Structure that has been dilated, by subtracting the variance from the average

				//CopyMemory(TempScratchImage->pix, vLocalSystemData->vITIPCDig->vProductAverageImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

				//mvt_multiply_const(TempScratchImage2, 2); //must multiply the variance image that has been expanded with dilate by 2 as it only goes up to 128

				//mvt_subtract(TempScratchImage, TempScratchImage2);

				//if (vLocalSystemData->vITIPCDig->vContaminant2StructureImage != NULL)
				//{
				//	delete vLocalSystemData->vITIPCDig->vContaminant2StructureImage;
				//	vLocalSystemData->vITIPCDig->vContaminant2StructureImage = NULL;
				//}
				//if (ThereIsEnoughMemory(sizeof(CImageData), "vContaminant2StructureImage"))
				//	vLocalSystemData->vITIPCDig->vContaminant2StructureImage = new CImageData;
				//if (vLocalSystemData->vITIPCDig->vContaminant2StructureImage)
				//{
				//	vLocalSystemData->vITIPCDig->vContaminant2StructureImage->MakeGreyImage(vLocalSystemData->vITIPCDig->vOriginalBufferSize);
				//	//crashed?????
				//	CopyMemory(vLocalSystemData->vITIPCDig->vContaminant2StructureImage->vGreyImage, TempScratchImage2->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

				//	if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
				//	{ 
				//		vLocalSystemData->vITIPCDig->SaveContaminant2StructureImageToFile(vGlobalCurrentProduct, vLocalSystemData->vITIPCDig->vContaminant2StructureImage, true);
				//		vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();		
				//	}

				//	if (vLocalSystemData->vITIPCDig->vContaminant2StructureImage)
				//	{
				//		delete vLocalSystemData->vITIPCDig->vContaminant2StructureImage;
				//		vLocalSystemData->vITIPCDig->vContaminant2StructureImage = NULL;
				//	}
				//}
			}
			//Sleep(10);
			if (vLocalConfigurationData->vAllowVoidInspection)
			{
				if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
				{ 
					if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
					{
						vLocalSystemData->vITIPCDig->MakeVoidProductStructureImage();
						vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();		
					}
					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("Pre Save Void To File",cDebugMessage);
					vLocalSystemData->vITIPCDig->SaveVoidStructureImageToFile(vGlobalCurrentProduct, true);
					vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();		
				}
			}
		}
		if (vLocalSystemData->vInAutoSetup)
			ReportErrorMessage("Finished Quick Learn, Used Learn Data: " + dtoa(vLocalSystemData->vWrongWidthsDuringLearn,0) + 
				" Wrong Widths, " + dtoa(vLocalSystemData->vWrongBrightnessDuringLearn,0) + " Wrong Brightness", cAction,0);
		else
		{
			ReportErrorMessage("Finished Quick ReLearn, Used Learn Data: " + dtoa(vLocalSystemData->vWrongWidthsDuringLearn,0) + " Wrong Widths, " +
				dtoa(vLocalSystemData->vWrongBrightnessDuringLearn,0) + " Wrong Brightness, " + dtoa(vLocalSystemData->vRejectsDuringLearn,0) + " Rejects", cAction,0);

			vLocalSystemData->vITIPCDig->ClearImproveLearn(false);
			vLocalSystemData->vImproveLearnTotalContainers = 0;
		}

		if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
		{  //end of collecting structure, After Save structure image, load back from files
			vLocalSystemData->vITIPCDig->LoadStructureImagesFromFiles(false, false);
			vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();		
		}
	}
	else
		ReportErrorMessage("Error-Memory Low, Quick Learn Aborted, Use Standard Learn", cEMailMaintenance,0);

	vLocalSystemData->vQuickLearnImageIndex = 0;
	if (vLocalSystemData->vEvaluateAndOrQuickLearn == cQuickLearnOnly)
	{
		ReportErrorMessage("Processing Quick Learn, Deleting Images",cAction,0);

		for (DWORD TempLoop = 0; TempLoop < cMaximumQuickLearnImages; TempLoop++)
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
				//if (!(TempLoop % 5)) Sleep(10);
			}
		}
	}
	//if (vLocalSystemData->vChangedSizeOfProduct)
	//{
	//	bool TempMakeInspectionsFullImage = false;
	//	if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) ||
	//		(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
	//		(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)) //Continuous Feed, or a Pipeline or TD
	//		TempMakeInspectionsFullImage = true;

	//	vGlobalCurrentProduct->SetupAllInspectionROIsFromReference(vLocalSystemData->vITIPCDig->vOriginalImage, vLocalSystemData->vChangedSizeOfProductCheckDensityROI, TempMakeInspectionsFullImage, vLocalSystemData->vITIPCDig->vProductAverageImage);
	//}
	vGlobalCurrentProduct->CheckAllROIs(vLocalSystemData->vITIPCDig->vProductAverageImage);
	vLocalSystemData->vChangedSizeOfProduct = false;
	vLocalSystemData->vChangedSizeOfProductCheckDensityROI = 0;

	if ((TempScratchImage) && (TempScratchImage->pix))
		im_delete(TempScratchImage);

	if ((TempScratchImage2) && (TempScratchImage2->pix))
		im_delete(TempScratchImage2);

	::PostThreadMessage(vGlobalCallingThreadID,cQuickLearnProcessingFinishedMessage,0,0);

	ReportErrorMessage("Finished processing Quick Learn",cAction,0);

	vGlobalProcessQuickLearnImagesThread = NULL;

	//if (vLocalSystemData->vInAutoSetup)
	//if (!vLocalSystemData->vImprovingLearn)
	//if (!vLocalSystemData->vInExpressSetup)
	//{
	//	CNoticeDialog TempNoticeDialog;
	//	TempNoticeDialog.vNoticeText = "\n\nQuick Learn Complete:\n" + dtoa(vLocalSystemData->vWrongWidthsDuringLearn,0) + " Wrong Widths during learn (excluded)\n" +
	//		dtoa(vLocalSystemData->vWrongBrightnessDuringLearn,0) + " Wrong Brightness during Learn (excluded)\n";
	//	TempNoticeDialog.vType = cNoticeMessage;
	//	TempNoticeDialog.DoModal();
	//}

	return 0;
}
