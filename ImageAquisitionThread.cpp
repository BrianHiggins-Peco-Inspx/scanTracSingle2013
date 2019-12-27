//ScanTrac Side View Source File
// ImageAquisitionThread.cpp : implementation file
//
#include "stdafx.h"
//#include "math.h"
#include "ImageAquisitionThread.h"
#include "ScanTrac.h"
//#ifdef UseIFCDLLs
//	#include "PCDig.h"  //for PC Dig constants
//#endif
#include "NoticeDialog.h"
#include "SerialPort.h"
#include "ProcessQuickLearnImagesThread.h"
/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/
extern BYTE vGlobalScanTracType;
extern bool vGlobalFPGAVersion11Point0OrHigher;
extern BYTE vGlobalErrorCount;
extern 	CSerialPort *vGlobaluCSerialPort;
extern BYTE vGlobalFrameGrabberType;
extern BYTE vGlobalInAutoSize;
extern bool vGlobalInAutoExposure;
extern BYTE vGlobalLearnState;
extern CProcessQuickLearnImagesThread *vGlobalProcessQuickLearnImagesThread;
extern bool vGlobalFPGAVersion10Point0OrHigher;
extern CImageAquisitionThread *vGlobalImageAquisitionThread;
extern CProduct *vGlobalCurrentProduct;
//extern bool vGlobalShowDebugWindows;

// kjh added for product slip
extern unsigned char vGlobalProductSlipArray[cSlipArraySize];
extern int vGlobalProductSlipErrorSent;
extern int vGlobalProductSlipCount;
extern int vGlobalProductSlipArrayLocation;

const BYTE cReferenceTries = 3;
const BYTE cAutoImproveDesensitizeFactor = 5;

/////////////////////////////////////////////////////////////////////////////
// CImageAquisitionThread

IMPLEMENT_DYNCREATE(CImageAquisitionThread, CWinThread)

CImageAquisitionThread::CImageAquisitionThread()
{
	vSavedFillerMonitorWeight = false;
	vCurrentImageBrightnessOK = true;
	vLastEvaluateSecond = 0;
	vNumberOfWrongSizeInARow = 0;
	vStartFreeTime = 0;
	vCurrentDemoImage = 0;
	//9/30/2009
	//vReferenceTries = 1;
	vSizeOK = true;
	vMyLock = false;
	vEndVirtualFrame = 1;
	vShutDownNow = false;
	m_bAutoDelete = true;
	vAFramePointer = NULL;
	vBFramePointer = NULL;
	vFinalOutPutBuffer = NULL;
	vAFrameCounter = 0;
	vBFrameCounter = 0;

	for (BYTE TempLoop = 0; TempLoop < cMaximumNumberForMultiLane; TempLoop++)
	{
		vMultiLaneImage[TempLoop] = NULL;
		vMultiLaneImageMaximum[TempLoop] = NULL;
		vMultiLaneLineCounter[TempLoop] = 0;
		vMultiLaneLineCounterNotInImage[TempLoop] = 0;
		vMultiLaneLastImageStartLine[TempLoop] = 0;
	}
	vMultiLaneSingleLinePointer = NULL;

	vCorrectDetectorEdges = true;
	vLocalSystemData = NULL;
	vLocalConfigurationData = NULL;
	vLoadedThisImageFromFile = false;
	vMainWindowPointer = NULL;
	vPreviousHardwareFrameCount = - 1;
}

CImageAquisitionThread::~CImageAquisitionThread()
{
	//vShutDownNow = true;

	// Wait for the thread to terminate
	//if (vLocalSystemData->vITIPCDig->vCamera != NULL)
	//	WaitForSingleObject (m_hThread,INFINITE);
	//CloseHandle(m_hThread);
	if (vMyLock)
	{
		//vLocalSystemData->vLastContainerImagedLock.Unlock();
		ReportErrorMessage("Error-IPT - Abnormal Aquisition Thread end - Lock still on", cWriteToLog, 0);
	}
	if (vGlobalImageAquisitionThread)
	{
		vGlobalImageAquisitionThread = NULL;
		ReportErrorMessage("Error-IPT - Abnormal Aquisition Thread end - Pointer not NULL", cWriteToLog, 0);
	}
	if (vMultiLaneSingleLinePointer) //did not create memory if 6 detectors used, just pointed at input image
	{
		if ((vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac) || 
			(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
			free(vMultiLaneSingleLinePointer);
	}
	if (vGlobalCurrentProduct)
	if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
	for (BYTE TempLoop = 0; TempLoop < vGlobalCurrentProduct->vMultiLaneNumberOfLanes; TempLoop++)
	if (vMultiLaneImage[TempLoop])
	{
		free(vMultiLaneImage[TempLoop]);
		vMultiLaneImage[TempLoop] = NULL;
	}
}

BOOL CImageAquisitionThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return true;
}

void CImageAquisitionThread::ExitImageAquisition()
{
	vShutDownNow = true;
}

int CImageAquisitionThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

void CImageAquisitionThread::KillThread()
{
	//AfxEndThread(0);
}


BEGIN_MESSAGE_MAP(CImageAquisitionThread, CWinThread)
	//{{AFX_MSG_MAP(CImageAquisitionThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageAquisitionThread message handlers

int CImageAquisitionThread::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	int TempSequenceNumber = 0;
	vLocalSystemData->vFrameCount = 0;
	WORD TempMinimumFrameSize = vLocalSystemData->vITIPCDig->vAquisitionSizeY / 2;

	vLocalSystemData->vFreeTimeMaximum = 0;
	vLocalSystemData->vFreeTimeMinimum = 0;
	vLocalSystemData->vFreeTimeAverage = 0;
	vLocalSystemData->vFreeTimeStandardDeviation = 0;
	vLocalSystemData->vFreeTimeIndex = 0;
	vLocalSystemData->vFreeTimeSamples = 0;
	for (WORD TempLoop = 0; TempLoop < cMaximumBodyTriggerWidthData; TempLoop++)
		vLocalSystemData->vFreeTimeData[TempLoop] = 0;
	if (!vLocalSystemData)
	{
		ReportErrorMessage("Error-Image Aquisition Thread, vLocalSystemData = NULL",cWriteToLog, 0);
		vShutDownNow = true;
	}
	else
	if (!vLocalSystemData->vITIPCDig)
	{
		ReportErrorMessage("Error-Image Aquisition Thread, vITIPCDig = NULL",cWriteToLog, 0);
		vShutDownNow = true;
	}
	else
	if (!vLocalSystemData->vITIPCDig->vAquisitionBuffer)
	{
		ReportErrorMessage("Error-Image Aquisition Thread, vAquisitionBuffer = NULL",cWriteToLog, 0);
		vShutDownNow = true;
	}
	else
	{
		BYTE *TempFrameBuffer = NULL;

#ifdef UseIFCDLLs
		if (vLocalSystemData->vITIPCDig->vCaptureModule != NULL)
			CInterrupt intr_eot(vLocalSystemData->vITIPCDig->vCaptureModule,PCD_INTR_EOACQ);
#endif
#ifndef IFC59Driver
		HIFCGRAB grabID;
#endif

		if (!vShutDownNow)
		{
			if (ThereIsEnoughMemory(vLocalSystemData->vITIPCDig->vAquisitionBufferSize, "A Frame Buffer"))
			{
				vAFramePointer = (BYTE *)malloc(vLocalSystemData->vITIPCDig->vAquisitionBufferSize);
			}
			if (!vAFramePointer)
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nCannot run, Out Of Memory A\nNeeded: " + dtoa(vLocalSystemData->vITIPCDig->vAquisitionBufferSize,0);
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
				vShutDownNow = true;
			}
		}
		if (!vShutDownNow)
		{
			if (ThereIsEnoughMemory(vLocalSystemData->vITIPCDig->vAquisitionBufferSize,"B Frame Buffer"))
			{
				vBFramePointer = (BYTE *)malloc(vLocalSystemData->vITIPCDig->vAquisitionBufferSize);
			}
			if (!vBFramePointer)
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nCannot run, Out Of Memory B\nNeeded: " + dtoa(vLocalSystemData->vITIPCDig->vAquisitionBufferSize,0);
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
				vShutDownNow = true;
			}
		}
		if (!vShutDownNow)
		{
			if (ThereIsEnoughMemory(vLocalSystemData->vITIPCDig->vAquisitionBufferSize, "Output Frame Buffer"))
			{
				vFinalOutPutBuffer = (BYTE *)malloc(vLocalSystemData->vITIPCDig->vAquisitionBufferSize);
			}

			if (!vFinalOutPutBuffer)
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nCannot run, Out Of Memory F\nNeeded: " + dtoa(vLocalSystemData->vITIPCDig->vAquisitionBufferSize,0);
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
				vShutDownNow = true;
			}

			if (vGlobalCurrentProduct)
			if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
			for (BYTE TempLoop = 0; TempLoop < vGlobalCurrentProduct->vMultiLaneNumberOfLanes; TempLoop++)
			{
				vMultiLaneImage[TempLoop] = (BYTE *)malloc(vLocalSystemData->vITIPCDig->vOriginalBufferSize + 1);
				if (vMultiLaneImage[TempLoop])
					vMultiLaneImageMaximum[TempLoop] = vMultiLaneImage[TempLoop] + vLocalSystemData->vITIPCDig->vOriginalBufferSize;
				if (vMultiLaneImage[TempLoop])
					FillMemory(vMultiLaneImage[TempLoop], vLocalSystemData->vITIPCDig->vOriginalBufferSize, 255);
				else
				{
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nCannot run, Out Of Memory B\nNeeded: " + dtoa(vLocalSystemData->vITIPCDig->vOriginalBufferSize,0);
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
					vShutDownNow = true;
				}
			}
		}
		if ((vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac) || 
			(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
		{
			vMultiLaneSingleLinePointer = (BYTE *)malloc(vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors);
			if (!vMultiLaneSingleLinePointer)
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nCannot run, Out Of Memory B\nNeeded: " + dtoa(vLocalSystemData->vITIPCDig->vOriginalBufferSize,0);
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
				vShutDownNow = true;
			}
		}

		TempFrameBuffer = vLocalSystemData->vITIPCDig->vAquisitionBuffer;

		// set board in mode: grab into host circular buffer
		if ((vLocalSystemData->vITIPCDig->vCamera) && (TempFrameBuffer))
		{
#ifdef IFC59Driver
			vLocalSystemData->vITIPCDig->vCamera->SetAcqParam(PCD_HI_SPEED_ACQ_ENABLE,IFC_ENABLE);

			TempFrameBuffer = vLocalSystemData->vITIPCDig->vCamera->SeqSnapPhys(cFramesInAquisitionBuffer,0,0,0,0,0,NULL);
		
			vLocalSystemData->vITIPCDig->vCamera->SetAcqParam(PCD_HI_SPEED_ACQ_ENABLE,IFC_ENABLE);
#else
			grabID=vLocalSystemData->vITIPCDig->vCamera->
				Grab(0, //IFC_GOPT_AUTO_FRAME_LOCK,
				TempFrameBuffer,
				cFramesInAquisitionBuffer,
				IFC_INFINITE_FRAMES); 
#endif
		}
		else
		if (!vLocalConfigurationData->vDemoMode)
		{
			if (!vLocalSystemData->vITIPCDig->vCamera)
			{
				ReportErrorMessage("Error-No Frame Grabber",cEMailInspx,32000);
				ReportErrorMessage("Frame Grabber not connected, Call Service",cError,0);

				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nError-No Frame Grabber detected in the computer.\nTurn off power and re-seat, or replace Frame Grabber";
				TempNoticeDialog.vType = cErrorMessage;
				//TempNoticeDialog.vAutoClose = 0xFFFF;  //force customer to shut down by not closing message
				TempNoticeDialog.DoModal();
			}
			else
			{
				ReportErrorMessage("Error-No Aquisition Frame Buffer",cEMailInspx,32000);

				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nError-No Frame Grabber Buffer detected in the computer.\nTurn off power and re-seat, or replace Frame Grabber";
				TempNoticeDialog.vType = cErrorMessage;
				//TempNoticeDialog.vAutoClose = 0xFFFF;  //force customer to shut down by not closing message
				TempNoticeDialog.DoModal();
			}
			vShutDownNow = true;
		}
		int TempNextFrameToGet = 0;
		DWORD TempFileNumber = 0;
		WORD TempPixelsUsedAbove12Inches = 0;

		if ((vGlobalCurrentProduct->vMultiLaneNumberOfLanes) &&//if Multi Lane
			((vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac) || 
			(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)))
				TempPixelsUsedAbove12Inches = vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors - vLocalSystemData->vITIPCDig->vAquisitionSizeX;  //total height minus 12 inches
		else
		if ((((vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)) && 
			(vLocalSystemData->vITIPCDig->vAquisitionROIBottom + vLocalSystemData->vITIPCDig->vOriginalBufferSizeX > 384)) ||
			((vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) && (vLocalSystemData->vITIPCDig->vAquisitionROIBottom + vLocalSystemData->vITIPCDig->vOriginalBufferSizeX > vLocalSystemData->vITIPCDig->vAquisitionSizeX)))
				TempPixelsUsedAbove12Inches = vLocalSystemData->vITIPCDig->vAquisitionROIBottom + vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - vLocalSystemData->vITIPCDig->vAquisitionSizeX;  //total height minus 192

		//continue until get flag to shut down
		for (;!vShutDownNow;) 
		{
				//1/28/2010
			vLocalSystemData->vITIPCDig->vImageTypeString = "Unknown";
			vLocalSystemData->vITIPCDig->vImageType = 0;
			DWORD TempAcquiredDy = 0;

			DWORD TempFrameWaitTime = 1800;

			if ((!vLocalSystemData->vITIPCDig->vShowOnlyRealImages) && (vLocalSystemData->vSimulateContainersRate))
			{
				TempFrameWaitTime = 60 * 1000 / vLocalSystemData->vSimulateContainersRate; 
			}
			else
			if ((!vLocalSystemData->vITIPCDig->vShowOnlyRealImages) && (vLocalConfigurationData->vContinuousFeedContainerRate))
			{
				TempFrameWaitTime = 60 * 1000 / vLocalConfigurationData->vContinuousFeedContainerRate; 
			}

			if ((vLocalSystemData->vSystemRunMode == cCalibratingDetectorOffsetMode) && (!vLocalSystemData->vITIPCDig->vShowOnlyRealImages))
				TempFrameWaitTime = 5;
			else
			if ((vLocalSystemData->vSystemRunMode == cTestShutterMode) && (!vLocalSystemData->vITIPCDig->vShowOnlyRealImages))
				TempFrameWaitTime = 5;

			vLocalSystemData->vITIPCDig->vHaveRealImage = false;

			vCorrectDetectorEdges = vLocalConfigurationData->vCorrectDetectorEdges;
			vLoadedThisImageFromFile = false;

			//if loaded image from a file, put it in as it is first in processing order
			if ((vLocalSystemData->vLoadedFileAvailable) && (!vLocalSystemData->vPostedNewMainImageReadyToDisplayMessage) &&
				(vLocalSystemData->vShowImages) && (vLocalSystemData->vLoadedFileImage) && (vLocalSystemData->vLoadedFileImage->pix))
			{
				if (vFinalOutPutBuffer)
				{
					int TempOriginalSequenceNumber = TempSequenceNumber;
					TempSequenceNumber = 252;
					vLocalSystemData->vITIPCDig->vHaveRealImage = false;
					//copy pixels from vLoadedFileImage to vFinalOutPutBuffer
					BYTE *TempSourceBuffer = vLocalSystemData->vLoadedFileImage->pix; 
					BYTE *TempDestinationBuffer;
					for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vAquisitionSizeY; TempLoopY++)
					{
						TempSourceBuffer = vLocalSystemData->vLoadedFileImage->pix + (vLocalSystemData->vITIPCDig->vOriginalBufferSizeX * TempLoopY);
						TempDestinationBuffer = vFinalOutPutBuffer + vLocalSystemData->vITIPCDig->vAquisitionROIBottom + (vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors * TempLoopY); 

						for (DWORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX; TempLoopX++)
						{
							*TempDestinationBuffer++ = *TempSourceBuffer++;  //copy data
						}
					}
					vLoadedThisImageFromFile = true;
					vCorrectDetectorEdges = false;
					vLocalSystemData->vITIPCDig->vImageTypeString = "File";
					vLocalSystemData->vITIPCDig->vImageType = cLoadedFromFileImageType;

					if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))//Continuous Feed, or a Pipeline
						ReportErrorMessage("++Notice, Loaded Image From File", cEMailInspx,32000);

					HandleCompletedImage(vFinalOutPutBuffer,TempSequenceNumber);
					TempSequenceNumber = TempOriginalSequenceNumber;
					vLoadedThisImageFromFile = false;
					vCorrectDetectorEdges = vLocalConfigurationData->vCorrectDetectorEdges;
				}
			}  //end of loading an image from a file
			else
			{  //no image from file, so get image from frame grabber 2nd in processing order
				if (vLocalSystemData->vITIPCDig->vCamera != NULL)
				{
					TempFrameBuffer = vLocalSystemData->vITIPCDig->vAquisitionBuffer;

#ifdef IFC59Driver
					TempSequenceNumber = vLocalSystemData->vITIPCDig->vCamera->WaitPhysFrame(
						&TempFrameBuffer,
						IFC_WAIT_NEWER_FRAME,
						TempFrameWaitTime,
						NULL);

					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("*IAT-PI-Wait for new image: " + dtoa(TempFrameWaitTime,0),cDebugMessage);

#else
					TempSequenceNumber =
						vLocalSystemData->vITIPCDig->vCamera->
						GrabWaitFrameEx(grabID,
						&TempFrameBuffer,
						IFC_WAIT_NEWER_FRAME,
						TempFrameWaitTime,
						false, //true,
						&TempAcquiredDy);
#endif

					//for only for testing
					
					//if (TempSequenceNumber >= 0)
					//if (PasswordOK(cTemporaryInspxPassword,false))
					//if (vLocalSystemData->vLogFile.vLogSerialData)
					//{
					//	TempFileNumber++;
					//	OutputImageToCSVFile("C:\\ScanTracSerialLogs\\FrameGrabber " + dtoa(TempFileNumber,0) + " ", TempFrameBuffer, vLocalSystemData->vITIPCDig->vAquisitionSizeX, vLocalSystemData->vITIPCDig->vAquisitionSizeY);
					//}
					
					//if (vLocalSystemData->vTestingOnlyStopImages)
					//	TempSequenceNumber = -1;

					//if showing demo images, throw away real images
					//2/1/2012 comment out for speed tests
					if (TempSequenceNumber >= 0)
					if ((!vLocalSystemData->vITIPCDig->vShowOnlyRealImages) && 
						((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem)) && (!vLocalSystemData->vSimulatingCamLinkImage))
					{
						TempSequenceNumber = -1;
						if (vLocalSystemData->vLogFile.vLogSerialData)
							vLocalSystemData->vLogFile.WriteToLogFile("*IAT-PI-Got Real Image From Grabber, but throw away as simulating images",cDebugMessage);
					}
					//if not running in any mode, throw away any image
					if (TempSequenceNumber >= 0)
					if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
					{
						//ClearDataInThisBuffer(TempFrameBuffer);
						TempSequenceNumber = -1;
						if (vLocalSystemData->vLogFile.vLogSerialData)
							vLocalSystemData->vLogFile.WriteToLogFile(
								"*IAT-PI-Got Real Image From Grabber, but throw away as not running",cDebugMessage);
					}
					
					//if got a timed out frame, then set seq to -1
					if (TempSequenceNumber >= 0)
					if (!vLocalConfigurationData->vSimulateLightControlBoard)  //if not simulating X-Rays and interlocks, must be valid hardware frame
					if ((!vAFrameCounter) && (!vBFrameCounter))
					{
						if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
						{ //case inspector
							//if got a new frame not back to back no counters running
							BYTE *TempPointerToExtraPixel = NULL;

							if (vLocalSystemData->vABPixelAtOtherSide)
								TempPointerToExtraPixel = TempFrameBuffer;
							else
								TempPointerToExtraPixel = TempFrameBuffer - (vLocalSystemData->vITIPCDig->vBytesPerPixel * vLocalSystemData->vITIPCDig->vInterlaced) + 
								//(vLocalSystemData->vITIPCDig->vAquisitionSizeX * vLocalSystemData->vITIPCDig->vBytesPerPixel * 2 * vLocalSystemData->vITIPCDig->vInterlaced); //2 Skips first line as bug
								(vLocalSystemData->vITIPCDig->vAquisitionSizeX * vLocalSystemData->vITIPCDig->vBytesPerPixel * vLocalSystemData->vITIPCDig->vInterlaced); //looks at end of first line

							if (vLocalSystemData->vLogFile.vLogSerialData)
								vLocalSystemData->vLogFile.WriteToLogFile("New Frame. A/B/C/D/E/F Pixel: " + ByteToHex(*TempPointerToExtraPixel) + " Seq #: " + dtoa(TempSequenceNumber,0),cDebugMessage);

							if (TempSequenceNumber >= 0)
							if (*TempPointerToExtraPixel == 0xFF)
								ReportErrorMessage("Notice, Frame Pixel = FF, possible problem?", cWriteToLog,32000);
						}
						else
						{
							BYTE *TempPointerToExtraPixel = NULL;
							
							if (vLocalSystemData->vABPixelAtOtherSide)
								TempPointerToExtraPixel = TempFrameBuffer;
							else//go to end of first line of image to check a/b frame pixel
								TempPointerToExtraPixel = TempFrameBuffer - vLocalSystemData->vITIPCDig->vBytesPerPixel + (vLocalSystemData->vITIPCDig->vAquisitionSizeX * vLocalSystemData->vITIPCDig->vBytesPerPixel);

							if (vLocalSystemData->vLogFile.vLogSerialData)
							{
								vLocalSystemData->vLogFile.WriteToLogFile("New Frame A/B Pixel: " + ByteToHex(*TempPointerToExtraPixel),cDebugMessage);
							}
							if (*TempPointerToExtraPixel == 0xFF)
							{
								if (vLocalSystemData->vRunningDiagnosticsMode)
									ReportErrorMessage("Error-AB Pixel is FF, Normally image would be ignored.  In Diagnostic Run Mode",cEMailInspx, 32000);
								else
								{
									TempSequenceNumber = -1;
									if (vLocalSystemData->vLogFile.vLogSerialData)
										vLocalSystemData->vLogFile.WriteToLogFile("Error-Ignore Frame",cDebugMessage);
								}
							}
						}
					}

					//if got an image from frame grabber, mark it as a real image
					if (TempSequenceNumber >= 0)
					{
						vLocalSystemData->vITIPCDig->vHaveRealImage = true;
						//5/7/2011
						vLocalSystemData->vITIPCDig->vImageTypeString = ""; //real image, but do not want to show anything on display as is normal
						vLocalSystemData->vITIPCDig->vImageType = cRealImageType;

						if (vLocalSystemData->vLogFile.vLogSerialData)
						{
							vLocalSystemData->vLogFile.WriteToLogFile(
								"Grab Returned Frame:" + dtoa(TempSequenceNumber,0) + " Address: " +
								dtoa((DWORD)TempFrameBuffer,0),cDebugMessage);
						}
					}
				}
				else
				{   //did not create pc-dig board
					vLocalSystemData->vITIPCDig->vLastBufferFilled = TempFrameBuffer;
					TempSequenceNumber = -2;//flag that no frame because no PC-Dig board in computer
					Sleep(TempFrameWaitTime);
				}

				/* for debugging only write all images to file to debug
				if (TempSequenceNumber >= 0)
				if (vLocalSystemData->vLogFile.vLogSerialData)
				if (PasswordOK(cTemporaryInspxPassword,false))
				{
					CString TempString("ImageRaw");
					TempString = TempString + vLocalSystemData->vFrameCountString + ".BMP";
					TempString = cLogDirectory + TempString;
					int TempLength = TempString.GetLength();
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
							TempImageFile->WriteFile(TempFrameBuffer,
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeX * vLocalSystemData->vITIPCDig->vBytesPerPixel * vLocalSystemData->vITIPCDig->vInterlaced,
								vLocalSystemData->vITIPCDig->vAquisitionSizeY,
								(WORD)vLocalSystemData->vITIPCDig->vBitsPerPixel);
							delete TempImageFile;
						}
					}
				}
				*/

				//if did not get a real image, and can show demo images, load demo images
				//comment out for testing high speed images if (TempSequenceNumber < 0)//don't have real data
				if ((!vLocalSystemData->vLoadedFileAvailable) && (vLocalSystemData->vNumberOfDemoImages) && (!vLocalSystemData->vLoadedFileAvailable) && 
					((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) && (!vShutDownNow) && (vLocalSystemData->vCurrentBodyTriggerMask)))
				{
					if ((vLocalSystemData->vDemoImage[vCurrentDemoImage]) && (vLocalSystemData->vDemoImage[vCurrentDemoImage]->pix))
					{
						//demo image
						TempFrameBuffer = vLocalSystemData->vITIPCDig->vAquisitionBuffer;
						BYTE *TempSourcePointer = NULL;
						BYTE *TempDestinationPointer = NULL;
						//copy data from demo image to Final Output buffer
						if ((vGlobalCurrentProduct->vMultiLaneNumberOfLanes) && ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem)))
						{
							for (DWORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY; TempLoopY++)  //each line in the image in the travel direction, width
							{
								TempSourcePointer = vLocalSystemData->vDemoImage[vCurrentDemoImage]->pix + (vLocalSystemData->vDemoImage[vCurrentDemoImage]->dx * TempLoopY);
								TempDestinationPointer = TempFrameBuffer + vLocalSystemData->vITIPCDig->vAquisitionROIBottom + (vLocalSystemData->vDemoImage[vCurrentDemoImage]->dx * (vLocalSystemData->vDemoImage[vCurrentDemoImage]->dy - TempLoopY - 1)); 

								for (int TempLoopX = 0; TempLoopX < vLocalSystemData->vDemoImage[vCurrentDemoImage]->dx; TempLoopX++)
								{//copy each pixel in the line to this buffer
									*TempDestinationPointer++ = *TempSourcePointer++;
								}
							}
						}
						else
						{
							for (DWORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY; TempLoopY++)  //each line in the image in the travel direction, width
							{
								TempSourcePointer = vLocalSystemData->vDemoImage[vCurrentDemoImage]->pix + (vLocalSystemData->vDemoImage[vCurrentDemoImage]->dx * TempLoopY);
								TempDestinationPointer = vFinalOutPutBuffer + vLocalSystemData->vITIPCDig->vAquisitionROIBottom + (vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors * (vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - TempLoopY - 1)); 

								for (int TempLoopX = 0; TempLoopX < vLocalSystemData->vDemoImage[vCurrentDemoImage]->dx; TempLoopX++)
								{//copy each pixel in the line to this buffer
									*TempDestinationPointer++ = *TempSourcePointer++;
								}
							}
						}
						CString TempText = "";
						TempText.LoadString(IDS_Demo);
						vLocalSystemData->vITIPCDig->vImageTypeString = TempText + dtoa(vCurrentDemoImage + 1,0);
						vLocalSystemData->vITIPCDig->vImageType = cDemoImageType;
							
						if (!((vGlobalCurrentProduct->vMultiLaneNumberOfLanes) && ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))))
						{
							HandleCompletedImage(vFinalOutPutBuffer,255);
							TempSequenceNumber = -2;
						}
						else
							TempSequenceNumber = 1;
					}
					vCurrentDemoImage++;
					if (vCurrentDemoImage >= vLocalSystemData->vNumberOfDemoImages)
						vCurrentDemoImage = 0;
				}//end of demo image
				else
				//if have not got an image yet, and can show demo images, put in a stick figure image
				if (TempSequenceNumber <= -1)//don't have real data
				if (!vLocalSystemData->vLoadedFileAvailable) 
				if ((vLocalSystemData->vITIPCDig->vProcessNextImage) || ((((!vLocalSystemData->vITIPCDig->vShowOnlyRealImages) && (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)))
					&& (!vShutDownNow) && (vLocalSystemData->vCurrentBodyTriggerMask)))
				if (vLocalSystemData->vITIPCDig->vAquisitionBuffer)
				{
					//  simulate receive new image data because no frame grabber in computer
					TempFrameBuffer = vLocalSystemData->vITIPCDig->vAquisitionBuffer;
					vLocalSystemData->vITIPCDig->FillBufferWithPattern(TempFrameBuffer, vLocalSystemData->vITIPCDig->vAquisitionBufferSize,
						vLocalSystemData->vFrameCount, vLocalSystemData->vDriftCompensationADCOffset);
					TempSequenceNumber = 1;
				//1/28/2010
					vLocalSystemData->vITIPCDig->vImageTypeString = "Sim";
					vLocalSystemData->vITIPCDig->vImageType = cSimulatedImageType;
				}//end of software simulating an image

				//if got a frame while running but have no containers, ignore the frame
				if (TempSequenceNumber >= 0)
				if ((vLocalSystemData->vContainerCollection->GetCount() == 0) && (!vGlobalCurrentProduct->vMultiLaneNumberOfLanes))
				if (vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
				if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
				if (!vLoadedThisImageFromFile)
				if (!vLocalSystemData->vAutoCalibrateWhileRunning)
				if (!vLocalSystemData->vSimulatingCamLinkImage)
				{ //got a frame, but have no containers, so ignore it
					TempSequenceNumber = -1;
					ReportErrorMessage("Error-IAT-ImageLoop Got Frame From Grabber, but no containers, so ignoring it", cWriteToLog,0);
				}

				//if got a frame while calibrating, but have not simulated first frame, ignore it
				if ((vLocalSystemData->vSystemRunMode == cCalibratingDetectorOffsetMode) || (vLocalSystemData->vSystemRunMode == cTestShutterMode))
				if (TempSequenceNumber >= 0)
				if (vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
				if (!vLocalSystemData->vCalibrationSimulationStarted)
				if (!vLocalSystemData->vSimulatingCamLinkImage)
				{ //got a frame, but have not simulated first calibration image yet, so ignore it
					ReportErrorMessage("Got Frame From Grabber before simulate Cal Image, so ignoring it", cWriteToLog,0);
					//ClearDataInThisBuffer(TempFrameBuffer);
					TempSequenceNumber = -1;
					vPreviousHardwareFrameCount = -1;
					//vLocalSystemData->vITIPCDig->vCamera->GrabRelease(grabID, TempSequenceNumber);
				}
			}
			{
				if (TempSequenceNumber >= 0)
				if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
				{//check each line if belongs to A or B Frame or both, copy if does, if complete
					//stop time beteen frames timer as got a new frame to process
					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("*IAT-PI-Something from Grabber:" + dtoa(vLocalSystemData->vImageCounter,0),cDebugMessage);

					if (vStartFreeTime)
					{
						double TempEndFreeTime = CIFCOS::GetSystimeMicrosecs();
						vLocalSystemData->vFreeTimeData[vLocalSystemData->vFreeTimeIndex] = (float)((TempEndFreeTime - vStartFreeTime) / 1000.0);

						if ((vLocalSystemData->vFreeTimeData[vLocalSystemData->vFreeTimeIndex] < vLocalSystemData->vFreeTimeMinimum) || (!vLocalSystemData->vFreeTimeMinimum))
							vLocalSystemData->vFreeTimeMinimum = vLocalSystemData->vFreeTimeData[vLocalSystemData->vFreeTimeIndex];

						if (vLocalSystemData->vFreeTimeData[vLocalSystemData->vFreeTimeIndex] > vLocalSystemData->vFreeTimeMaximum)
							vLocalSystemData->vFreeTimeMaximum = vLocalSystemData->vFreeTimeData[vLocalSystemData->vFreeTimeIndex];

						if (vLocalSystemData->vFreeTimeSamples < cMaximumBodyTriggerWidthData - 1)
							vLocalSystemData->vFreeTimeSamples++;

						vLocalSystemData->vFreeTimeIndex++;
						if (vLocalSystemData->vFreeTimeIndex >= cMaximumBodyTriggerWidthData)
							vLocalSystemData->vFreeTimeIndex = 0;
					}
					//a frame copy to output buffer.
					if (vLocalSystemData->vLogFile.vLogSerialData)
					{
						vLocalSystemData->vLogFile.WriteToLogFile("IAT-ImageLoop Got Frame From Grabber",cDebugMessage);

						vLocalSystemData->vLogFile.WriteToLogFile("vAquistionBuffer Address: " + dtoa(DWORD(vLocalSystemData->vITIPCDig->vAquisitionBuffer),0) + " to " +
							dtoa(DWORD(vLocalSystemData->vITIPCDig->vAquisitionBuffer + vLocalSystemData->vITIPCDig->vAquisitionBufferSize * cFramesInAquisitionBuffer),0), cDebugMessage);
					}

					if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes) 
					{
						if (!vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
							vLocalSystemData->vMultiLaneLastTriggerPosition = BPSubtract(vLocalSystemData->vCurrentBeltPosition, 200);

						vLocalSystemData->vMultiLaneLastTriggerPositionForThisImage = vLocalSystemData->vMultiLaneLastTriggerPosition;
						if (vLocalSystemData->vLogFile.vLogSerialData)
								vGlobaluCSerialPort->WriteToLogFileWithBeltPosition("Multi Lane-Save This Image Trigger Location: " + dtoa(vLocalSystemData->vMultiLaneLastTriggerPositionForThisImage, 0), vLocalSystemData->vCurrentBeltPosition);
					}

					// br h for debugging images
					//WriteImageToHardDisk(TempFrameBuffer, "C:\\ScanTracSerialLogs\\RawImage " + dtoa(vLocalSystemData->vFrameCount, 0) + ".bmp", vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors, vLocalSystemData->vITIPCDig->vAquisitionSizeY);
					//vLocalSystemData->vFrameCount++;
					//if (PasswordOK(cInspxPassword,false))
					//	OutputImageToCSVFile("C:\\ScanTracSerialLogs\\RawImage.csv", TempFrameBuffer, vLocalSystemData->vITIPCDig->vAquisitionSizeX * vLocalSystemData->vITIPCDig->vBytesPerPixel * vLocalSystemData->vITIPCDig->vInterlaced, vLocalSystemData->vITIPCDig->vAquisitionSizeY);

					BYTE *TempPointerToExtraPixel = NULL;
					BYTE *TempSourcePointer = NULL;
					BYTE *TempDestinationPointer = NULL;
					//check every line to see if an A frame or a B frame or both
					for (DWORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vAquisitionSizeY; TempLoopY++)
					{//check the last byte of each line, this is the extra pixel with a/b bits

						if ((vLocalSystemData->vABPixelAtOtherSide) && (vLocalSystemData->vITIPCDig->vShowOnlyRealImages))
							TempPointerToExtraPixel = TempFrameBuffer + (vLocalSystemData->vITIPCDig->vAquisitionSizeX * vLocalSystemData->vITIPCDig->vBytesPerPixel * TempLoopY * vLocalSystemData->vITIPCDig->vInterlaced);
						else
							TempPointerToExtraPixel = TempFrameBuffer - (vLocalSystemData->vITIPCDig->vBytesPerPixel * vLocalSystemData->vITIPCDig->vInterlaced) + 
								(vLocalSystemData->vITIPCDig->vAquisitionSizeX * vLocalSystemData->vITIPCDig->vBytesPerPixel * (TempLoopY + 1) * vLocalSystemData->vITIPCDig->vInterlaced);

							// br h temp for testing
							//if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
							//{
							//	*TempPointerToExtraPixel = *TempPointerToExtraPixel & 0xC0 | 0x01;
							//}

						//if (vLocalSystemData->vFPGAVersion9Point2OrHigher)
						////if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
						//if (vLocalSystemData->vITIPCDig->vHaveRealImage)
						////temp for testing to avoid bug
						//if (TempLoopY < 2)
						//{
						//	if (vLocalSystemData->vABPixelAtOtherSide)
						//		TempPointerToExtraPixel = TempFrameBuffer + (vLocalSystemData->vITIPCDig->vAquisitionSizeX * vLocalSystemData->vITIPCDig->vBytesPerPixel * 2 * vLocalSystemData->vITIPCDig->vInterlaced);
						//	else
						//	TempPointerToExtraPixel = TempFrameBuffer - (vLocalSystemData->vITIPCDig->vBytesPerPixel * vLocalSystemData->vITIPCDig->vInterlaced) + 
						//		(vLocalSystemData->vITIPCDig->vAquisitionSizeX * vLocalSystemData->vITIPCDig->vBytesPerPixel * (TempLoopY + 1) * vLocalSystemData->vITIPCDig->vInterlaced);//if first line, go to next line to avoid bug

						//	//br h temp for testing
						//	if (vLocalSystemData->vLogFile.vLogSerialData)
						//	if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
						//		vLocalSystemData->vLogFile.WriteToLogFile("Case Inspector2: Hardware Frame, TempPointerToExtraPixel: " + ByteToHex(*TempPointerToExtraPixel), cDebugMessage);
						//	//br h temp for testing
						//	//if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
						//	//{
						//	//	*TempPointerToExtraPixel = *TempPointerToExtraPixel & 0xC0 | 0x01;
						//	//}
						//}

						//if (vLocalSystemData->vFPGAVersion9Point2OrHigher)
						if ((vLocalSystemData->vITIPCDig->vHaveRealImage) && (TempLoopY == 0))
						{
							BYTE TempHardwareFrameCount = 0;
							if (vGlobalFPGAVersion11Point0OrHigher)
								TempHardwareFrameCount = *TempPointerToExtraPixel >> 6; //if version 11 or 15 uController, only two upper bits are frame counter.
							else
								TempHardwareFrameCount = *TempPointerToExtraPixel >> 4;

							if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
							if (vPreviousHardwareFrameCount != -1)
							if (vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
							{
								BYTE TempThisHardwareFrameCountShouldBe = vPreviousHardwareFrameCount + 1;

								if (TempThisHardwareFrameCountShouldBe > 15)
									TempThisHardwareFrameCountShouldBe = 0;

								if (vGlobalFPGAVersion11Point0OrHigher)
									if (TempThisHardwareFrameCountShouldBe > 3)
										TempThisHardwareFrameCountShouldBe = 0;
							}
							vPreviousHardwareFrameCount = TempHardwareFrameCount;
						}
									
						if ((vGlobalCurrentProduct->vMultiLaneNumberOfLanes) && (!vLocalSystemData->vMultiLaneInSetupLanePositionsMenu) && 
							((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem)))
						{ //Multi Lane
							//for each line, start or construct image for each lane.  If any lane completes its image, then process it
							if (vLocalSystemData->vLogFile.vLogSerialData)
								vLocalSystemData->vLogFile.WriteToLogFile("Multi Lane Line: " + dtoa(TempLoopY,0) + " EP: " + ByteToHex(*TempPointerToExtraPixel), cDebugMessage);

							//if just 12 inches, point at the line in the buffer
							if (vLocalConfigurationData->vNumberOfXRayDetectors == 6)
								vMultiLaneSingleLinePointer = TempFrameBuffer + (TempLoopY * vLocalSystemData->vITIPCDig->vBytesPerPixel * vLocalSystemData->vITIPCDig->vAquisitionSizeX);
							else
							if (vLocalConfigurationData->vScanTracType == cForteScanTrac)
							{ 
								if (vLocalSystemData->vITIPCDig->vImageType == cDemoImageType)
								{
									CopyMemory(vMultiLaneSingleLinePointer, TempFrameBuffer + (TempLoopY * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors), 
										vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors);
								}
								else
								{//get front 6 (3 on allegro) detectors data.  On Allegro, don't have front 3 detectors, but must read in black pixels, so once read in, reset input counter to overwrite
									// get first 12 inches, which is very back, flipped over image
									//flipped over image
									TempSourcePointer = TempFrameBuffer + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors) - 1 ; //go to the last pixel of the line

									TempDestinationPointer = vMultiLaneSingleLinePointer; //start at beginning of memory for image

									for (DWORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vAquisitionSizeX; TempLoopX++)
									{//copy each pixel in the line to this buffer
										*TempDestinationPointer++ = *TempSourcePointer--; //goes into front of image on a TD
										TempSourcePointer--;//skip every other byte as it was for the first 12 inches
									}

									//if have a Forte, get 24 inch image as 2 - 12 inch images must combine
									TempSourcePointer = TempFrameBuffer + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors) - vLocalSystemData->vITIPCDig->vBytesPerPixel;

									TempPixelsUsedAbove12Inches = vLocalSystemData->vITIPCDig->vAquisitionSizeX; //must copy all pixels to calibrate

									for (DWORD TempLoopX = 0; TempLoopX < TempPixelsUsedAbove12Inches; TempLoopX++)
									{//copy each pixel in the line to this buffer
										*TempDestinationPointer++ = *TempSourcePointer--;
										TempSourcePointer--;//skip every other byte as it was for the first 12 inches
									}
									TempDestinationPointer = vMultiLaneSingleLinePointer + vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors - 1; //point to last pixel, which is A/B Pixel from uC
									*TempDestinationPointer = 255;
								}
							}
							else
							if (vLocalConfigurationData->vScanTracType == cAllegroScanTrac)
							{ //get front 6 (3 on allegro) detectors data.  On Allegro, don't have front 3 detectors, but must read in black pixels, so once read in, reset input counter to overwrite, flipped over image
								//flipped over image
								TempSourcePointer = TempFrameBuffer + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeX * 2) - 1- (192 * 2); //go to the last pixel of the line then skip the first 192 pixels
								if (vLocalConfigurationData->vPixelsPerDetector == 128)
									TempSourcePointer = TempSourcePointer - (192 * 2);

								TempDestinationPointer = vMultiLaneSingleLinePointer; //go to the first pixel of the line

								WORD TempNumberOfPixelsToCopy = 192; //only 3 detectors at 64 pixels per detector
								if (vLocalConfigurationData->vPixelsPerDetector == 128)
									TempNumberOfPixelsToCopy = 384; //only 3 detectors at 128 pixels per detector 
								for (DWORD TempLoopX = 0; TempLoopX < TempNumberOfPixelsToCopy; TempLoopX++)
								{//copy each pixel in the line to this buffer
									*TempDestinationPointer++ = *TempSourcePointer--; //goes into front of image on a TD, bottom of image on a solo
									TempSourcePointer--;//skip every other byte as it was for the last 12 inches
								}

								//get back 12 inches on Allegro
								TempSourcePointer = TempFrameBuffer + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeX * 2) - vLocalSystemData->vITIPCDig->vBytesPerPixel;

								TempPixelsUsedAbove12Inches = 384; //must copy all pixels to calibrate
								if (vLocalConfigurationData->vPixelsPerDetector == 128)
									TempPixelsUsedAbove12Inches = TempPixelsUsedAbove12Inches * 2;

								for (DWORD TempLoopX = 0; TempLoopX < TempPixelsUsedAbove12Inches; TempLoopX++)
								{//copy each pixel in the line to this buffer
									*TempDestinationPointer++ = *TempSourcePointer--;
									TempSourcePointer--;//skip every other byte as it was for the first 12 inches
								}
							}
							else
							if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
							{
								//if (vLocalSystemData->vITIPCDig->vAquisitionROITop > 384) //only copy this part if image uses back 12 inches, from uC copy to back to top part of image
								{
									// get first 12 inches, which is very back, flipped over image
									//this gets the uC part of the image and puts it in the top (pixel 384 to 575)
									//flip over image, read it out backwards
									TempSourcePointer = TempFrameBuffer + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeX * 4) - 4; //4 bytes per pixel

									TempDestinationPointer = vMultiLaneSingleLinePointer + 384;

									//copy 12 inches to bottom of image, which is front of case inspector
									for (DWORD TempLoopX = 0; TempLoopX < 192; TempLoopX++)
									{//copy each pixel in the line to this buffer
										if ((TempDestinationPointer > vAFramePointer + vLocalSystemData->vITIPCDig->vAquisitionBufferSize) || (TempDestinationPointer < vAFramePointer) ||
											(TempSourcePointer > TempFrameBuffer + vLocalSystemData->vITIPCDig->vAquisitionBufferSize) || (TempSourcePointer < TempFrameBuffer))
											ReportErrorMessage("TempFrameBuffer Pointer Error 1", cEMailInspx, 32000);
										else
											*TempDestinationPointer++ = *TempSourcePointer;

										TempSourcePointer = TempSourcePointer - 4; //4 bytes per pixel
									}
								}

								//second section goes in middle
								//if ((vLocalSystemData->vITIPCDig->vAquisitionROITop > 192) && (vLocalSystemData->vITIPCDig->vAquisitionROIBottom < 385)) //only copy if image uses middle 12 inches
								//if (TempPixelsUsedAbove12Inches)  //need to copy pixels from daughter card from other image in Trio
								{//the middle 12 inches of the image is equivelent to the C and D frame of a trio image
									//flip over image, read it out backwards
									TempSourcePointer = TempFrameBuffer + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeX * 4) - 2;

									TempDestinationPointer = vMultiLaneSingleLinePointer + 192;

									//copy 12 inches to center of image
									for (DWORD TempLoopX = 0; TempLoopX < 192; TempLoopX++)
									{//copy each pixel in the line to this buffer
										if ((TempDestinationPointer > vAFramePointer + vLocalSystemData->vITIPCDig->vAquisitionBufferSize) || (TempDestinationPointer < vAFramePointer) ||
											(TempSourcePointer > TempFrameBuffer + vLocalSystemData->vITIPCDig->vAquisitionBufferSize) || (TempSourcePointer < TempFrameBuffer))
											ReportErrorMessage("TempFrameBuffer Pointer Error 2", cEMailInspx, 32000);
										else
											*TempDestinationPointer++ = *TempSourcePointer;
										TempSourcePointer = TempSourcePointer - 4; //4 bytes per pixel
									}
								}

								//need to copy pixels from center image of Trio, equivelent to an e or f frame in a trio and put at bottom or front of image
								//if (vLocalSystemData->vITIPCDig->vAquisitionROIBottom < 192) //only copy if image uses bottom or front 12 inches
								{
									TempSourcePointer = TempFrameBuffer - 3 + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeX * 4);

									BYTE *TempSourcePointerIL = TempFrameBuffer - 1 + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeX * 4);

									TempDestinationPointer = vMultiLaneSingleLinePointer;

									//copy 12 inches to top of image which is back of case inspector
									for (DWORD TempLoopX = 0; TempLoopX < 192; TempLoopX++)
									{//copy each pixel in the line to this buffer
										if ((TempDestinationPointer > vAFramePointer + vLocalSystemData->vITIPCDig->vAquisitionBufferSize) || (TempDestinationPointer < vAFramePointer) ||
											(TempSourcePointer > TempFrameBuffer + vLocalSystemData->vITIPCDig->vAquisitionBufferSize) || (TempSourcePointer < TempFrameBuffer))
											ReportErrorMessage("TempFrameBuffer Pointer Error 3", cEMailInspx, 32000);
										else
											*TempDestinationPointer++ = *TempSourcePointer + (*TempSourcePointerIL << 4);  //get 4 bits from each of 2 image pixels as PC2-CamLinK

										TempSourcePointer = TempSourcePointer - 4;
										TempSourcePointerIL = TempSourcePointerIL - 4;
									}
								}
							}
							
							for (BYTE TempLaneLoop = 0; TempLaneLoop < vGlobalCurrentProduct->vMultiLaneNumberOfLanes; TempLaneLoop++)
							if (vGlobalCurrentProduct->vMultiLaneWidthInPixels[TempLaneLoop])
							{
								//NOW have the current line in vMultiLaneSingleLinePointer so can copy the appropriate lanes out
								//if not in the middle of an image for this lane, see if image is dark enough so should start a new image for this lane
								bool TempStartNewImage = false;
								double TempAverageOfPixelsInThisLane = 0;
								//WORD TempContainerPixelCount = 0;
								if (vMultiLaneLineCounter[TempLaneLoop] == 0) //not in middle of building an image for this lane, so see if should start one
								{

									if (vGlobalCurrentProduct->vEdgeLocationHeightPixel >= vGlobalCurrentProduct->vMultiLaneWidthInPixels[TempLaneLoop] - 1)
										vGlobalCurrentProduct->SetEdgeLocationHeight((float)((vGlobalCurrentProduct->vMultiLaneWidthInPixels[TempLaneLoop] / 2) / vGlobalPixelsPerUnitInHeight));

									if ((vGlobalCurrentProduct->vEdgeLocationBottomPixel + vGlobalCurrentProduct->vEdgeLocationHeightPixel >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeX) ||
										(vGlobalCurrentProduct->vEdgeLocationBottomPixel + vGlobalCurrentProduct->vEdgeLocationHeightPixel >= vGlobalCurrentProduct->vMultiLaneWidthInPixels[TempLaneLoop]))
									{
										vGlobalCurrentProduct->vEdgeLocationBottomPixel = 2;
										if (vGlobalCurrentProduct->vEdgeLocationHeightPixel + 4 >= vGlobalCurrentProduct->vMultiLaneWidthInPixels[TempLaneLoop])
											vGlobalCurrentProduct->SetEdgeLocationHeight((float)((vGlobalCurrentProduct->vMultiLaneWidthInPixels[TempLaneLoop] / 2) / vGlobalPixelsPerUnitInHeight));
									}

									TempSourcePointer = vMultiLaneSingleLinePointer + vGlobalCurrentProduct->vMultiLaneStartPixel[TempLaneLoop] + vGlobalCurrentProduct->vEdgeLocationBottomPixel;

									////average method of finding container
									//for (WORD TempPixelLoop = 0; TempPixelLoop < vGlobalCurrentProduct->vEdgeLocationHeightPixel; TempPixelLoop++)
									//{
									//	TempAverageOfPixelsInThisLane = TempAverageOfPixelsInThisLane + *TempSourcePointer;
									//	TempSourcePointer = TempSourcePointer + 1;
									//}

									//TempAverageOfPixelsInThisLane = TempAverageOfPixelsInThisLane / vGlobalCurrentProduct->vEdgeLocationHeightPixel;

									//if (TempAverageOfPixelsInThisLane < vGlobalCurrentProduct->vThresholdForContainerBounds)
									//{
									//	TempStartNewImage = true;
									//	vMultiLaneLastImageStartLine[TempLaneLoop] = (WORD)TempLoopY;
									//	vMultiLaneLineCounter[TempLaneLoop] = vGlobalCurrentProduct->vMultiLaneNumberOfLinesBeforeContainer;

									//	if (vLocalSystemData->vLogFile.vLogSerialData)
									//		vLocalSystemData->vLogFile.WriteToLogFile("----Start Of New Container Lane: " + dtoa(TempLaneLoop + 1, 0) + " Line Average: " + dtoa(TempAverageOfPixelsInThisLane, 0), cDebugMessage);
									//}
									//3 pixels darker than threshold
									WORD TempNumberOfPixelsNeeded = vGlobalCurrentProduct->vEdgeLocationHeightPixel / 16; //2 pixels per inch needed
									if (TempNumberOfPixelsNeeded < 3)
										TempNumberOfPixelsNeeded = 3;

									WORD TempContainerPixelCount = 0;
									for (WORD TempPixelLoop = 0; TempPixelLoop < vGlobalCurrentProduct->vEdgeLocationHeightPixel; TempPixelLoop++)
									if (vGlobalCurrentProduct->vThresholdForContainerBounds > *TempSourcePointer++)
										TempContainerPixelCount++;

									if (TempNumberOfPixelsNeeded < TempContainerPixelCount)  //if have more than 3 darker than the treshold, then indicate new container
									{
										TempStartNewImage = true;
										vMultiLaneLastImageStartLine[TempLaneLoop] = (WORD)TempLoopY;
										vMultiLaneLineCounter[TempLaneLoop] = vGlobalCurrentProduct->vMultiLaneNumberOfLinesBeforeContainer;

										if (vLocalSystemData->vLogFile.vLogSerialData)
											vLocalSystemData->vLogFile.WriteToLogFile("----Start Of New Container Lane: " + dtoa(TempLaneLoop + 1, 0) + " Container Pixels: " + dtoa(TempContainerPixelCount, 0), cDebugMessage);
									}
								}

								if (vMultiLaneImage[TempLaneLoop])
								if ((vMultiLaneLineCounter[TempLaneLoop]) || (TempStartNewImage)) //copy the part of this line into the image for this lane
								{
									TempSourcePointer = vMultiLaneSingleLinePointer + vGlobalCurrentProduct->vMultiLaneStartPixel[TempLaneLoop];

									TempDestinationPointer = vMultiLaneImage[TempLaneLoop] + (vMultiLaneLineCounter[TempLaneLoop] * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);

									if (vMultiLaneImageMaximum[TempLaneLoop] < TempDestinationPointer + vGlobalCurrentProduct->vMultiLaneWidthInPixels[TempLaneLoop])
									{
										ReportErrorMessage("Programming Error in Multi Lane would crash from pointer beyond image ML4, Line: " + dtoa(vMultiLaneLineCounter[TempLaneLoop], 0), cEMailInspx, 32000);
									}
									else
										CopyMemory(TempDestinationPointer, TempSourcePointer, vGlobalCurrentProduct->vMultiLaneWidthInPixels[TempLaneLoop]);

									if (vLocalSystemData->vLogFile.vLogSerialData)
										vLocalSystemData->vLogFile.WriteToLogFile("----Copy Lane: " + dtoa(TempLaneLoop + 1, 0) + " Line: " + dtoa(vMultiLaneLineCounter[TempLaneLoop], 0), cDebugMessage);

									vMultiLaneLineCounter[TempLaneLoop]++;

									if (vMultiLaneLineCounter[TempLaneLoop] >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeY) //this lane has completed an image, so process it
									{
										CopyMemory(vFinalOutPutBuffer, vMultiLaneImage[TempLaneLoop], vLocalSystemData->vITIPCDig->vOriginalBufferSize);

										// test code
										//WriteImageToHardDisk(vMultiLaneImage[TempLaneLoop], "C:\\ScanTracSerialLogs\\vMultiLaneImage" + dtoa(TempLaneLoop + 1, 0) + dtoa(vLocalSystemData->vFrameCount, 0) + ".bmp", 
										//	vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

										vMultiLaneLineCounter[TempLaneLoop] = 0;
										vMultiLaneLineCounterNotInImage[TempLaneLoop] = 0;
										vMultiLaneEjectorOffset = vMultiLaneLastImageStartLine[TempLaneLoop]; //save what line in the raw image from the frame grabber you found the container at for the ejector offset
										vMultiLaneLastImageStartLine[TempLaneLoop] = 0;
										if (vLocalSystemData->vLogFile.vLogSerialData)
											vLocalSystemData->vLogFile.WriteToLogFile("Finished Image from Lane: " + dtoa(TempLaneLoop + 1, 0) + " and Processed, mode: " + dtoa(vLocalSystemData->vSystemRunMode, 0), cDebugMessage);

										vLocalSystemData->vMultiLaneTotalCounts[TempLaneLoop]++;
										HandleCompletedImage(vFinalOutPutBuffer, TempLaneLoop);

										//if have white space before container, shift image over so have the end of this image at the start of the next
										if (vGlobalCurrentProduct->vMultiLaneNumberOfLinesBeforeContainer)
										{
											//push the first N lines of the image down one
											TempSourcePointer= vMultiLaneImage[TempLaneLoop] + 
												((vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - vGlobalCurrentProduct->vMultiLaneNumberOfLinesBeforeContainer) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);

											TempDestinationPointer = vMultiLaneImage[TempLaneLoop];

											DWORD TempBytesToMove = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX * vGlobalCurrentProduct->vMultiLaneNumberOfLinesBeforeContainer;

											if (TempSourcePointer + TempBytesToMove > vMultiLaneImageMaximum[TempLaneLoop])
											{
												ReportErrorMessage("Programming Error in Multi Lane would crash from pointer beyond image ML1", cEMailInspx, 32000);
											}
											else
												CopyMemory(TempDestinationPointer, TempSourcePointer, TempBytesToMove);

											vMultiLaneLineCounterNotInImage[TempLaneLoop] = vGlobalCurrentProduct->vMultiLaneNumberOfLinesBeforeContainer;

											BYTE *TempStartPointer = vMultiLaneImage[TempLaneLoop] + (vGlobalCurrentProduct->vMultiLaneNumberOfLinesBeforeContainer * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
											DWORD TempLength = vLocalSystemData->vITIPCDig->vOriginalBufferSize - 
												(vGlobalCurrentProduct->vMultiLaneNumberOfLinesBeforeContainer * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
											FillMemory(TempStartPointer, TempLength, 255); //blank image memory after part just shifted to the front 
										}
										else
											FillMemory(vMultiLaneImage[TempLaneLoop], vLocalSystemData->vITIPCDig->vOriginalBufferSize, 255); //blank image
									}
								}
								else
								{
									//did not start an image, and not building an image, so want to put preconfigured number of lines into the image so container not up against edge of image
									if (vGlobalCurrentProduct->vMultiLaneNumberOfLinesBeforeContainer)
									{
										//push the first N lines of the image down one
										for (int TempShiftLoop = 0; TempShiftLoop < vGlobalCurrentProduct->vMultiLaneNumberOfLinesBeforeContainer - 1; TempShiftLoop++)
										{
											TempSourcePointer = vMultiLaneImage[TempLaneLoop] + ((TempShiftLoop + 1) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);

											TempDestinationPointer = vMultiLaneImage[TempLaneLoop] + (TempShiftLoop * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX); //put this line in the 4th line of the image

											if (vMultiLaneImageMaximum[TempLaneLoop] <= TempDestinationPointer + vLocalSystemData->vITIPCDig->vOriginalBufferSizeX)
											{
												ReportErrorMessage("Programming Error in Multi Lane would crash from pointer beyond image ML2", cEMailInspx, 32000);
											}
											else
												CopyMemory(TempDestinationPointer, TempSourcePointer, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);

										}

										//copy the new line to the Nth line of the image
										TempSourcePointer = vMultiLaneSingleLinePointer + vGlobalCurrentProduct->vMultiLaneStartPixel[TempLaneLoop];

										TempDestinationPointer = vMultiLaneImage[TempLaneLoop] + ((vGlobalCurrentProduct->vMultiLaneNumberOfLinesBeforeContainer - 1) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX); //put this line in the 4th line of the image

										if (vMultiLaneImageMaximum[TempLaneLoop] <= TempDestinationPointer + vGlobalCurrentProduct->vMultiLaneWidthInPixels[TempLaneLoop])
										{
											ReportErrorMessage("Programming Error in Multi Lane would crash from pointer beyond image ML3", cEMailInspx, 32000);
										}
										else
										{
											CopyMemory(TempDestinationPointer, TempSourcePointer, vGlobalCurrentProduct->vMultiLaneWidthInPixels[TempLaneLoop]);
											vMultiLaneLineCounter[TempLaneLoop] = 0;
											vMultiLaneLineCounterNotInImage[TempLaneLoop] = vGlobalCurrentProduct->vMultiLaneNumberOfLinesBeforeContainer + 1;

											if (vLocalSystemData->vLogFile.vLogSerialData)
												vLocalSystemData->vLogFile.WriteToLogFile("----Copy Background to Lane: " + dtoa(TempLaneLoop + 1, 0) + " Line: " + 
													dtoa(vGlobalCurrentProduct->vMultiLaneNumberOfLinesBeforeContainer, 0), cDebugMessage);
										}
									}
								}
							}
						}
						else
						{ //NOT MultiLane
							if ((*TempPointerToExtraPixel & 1) == 1)
							{//this is an A Frame line, so copy it to the A Frame Buffer
								if (vLocalSystemData->vLogFile.vLogSerialData)
									vLocalSystemData->vLogFile.WriteToLogFile("Line: " + dtoa(TempLoopY,0) + " Got A Line: " + 
										dtoa(vAFrameCounter,0) + " HW Frame: " + dtoa(vPreviousHardwareFrameCount,0) + " EP: " + ByteToHex(*TempPointerToExtraPixel), cDebugMessage);

								if (vLocalConfigurationData->vNumberOfXRayDetectors == 6)
								{//flip over as you copy pixels
									TempSourcePointer = TempFrameBuffer + (TempLoopY * vLocalSystemData->vITIPCDig->vBytesPerPixel * vLocalSystemData->vITIPCDig->vAquisitionSizeX);
									TempDestinationPointer = vAFramePointer - 1 + ((vAFrameCounter + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors);

									for (DWORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vAquisitionSizeX; TempLoopX++)
									{//copy each pixel in the line to this buffer
										*TempDestinationPointer-- = *TempSourcePointer++;//skip every other byte as it was for the last 12 inches
									}
								}
								else
								if (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)
								{ //get lower 6 detectors data (don't flip over as flipped from solo.
									TempDestinationPointer = vAFramePointer + (vAFrameCounter * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors); //go to the first pixel of the line

									TempSourcePointer = TempFrameBuffer + (TempLoopY * vLocalSystemData->vITIPCDig->vAquisitionSizeX * 2); //go to the last pixel of the line then skip the first 192 pixels

									WORD TempNumberOfPixelsToCopy1 = 384;
									if (vLocalConfigurationData->vPixelsPerDetector == 128)
										TempNumberOfPixelsToCopy1 = 768;

									for (DWORD TempLoopX = 0; TempLoopX < TempNumberOfPixelsToCopy1; TempLoopX++)
									{//copy each pixel in the line to this buffer
										*TempDestinationPointer++ = *TempSourcePointer++; //read straight in
										TempSourcePointer++;//skip every other byte as it was for the last 12 inches
									}

									WORD TempNumberOfPixelsToCopy2 = 192;
									if (vLocalConfigurationData->vPixelsPerDetector == 128)
										TempNumberOfPixelsToCopy2 = 384;

									//get top 3 detectors worth of the image
									TempSourcePointer = TempFrameBuffer + 1 + (TempLoopY * vLocalSystemData->vITIPCDig->vAquisitionSizeX * 2); //gets to first pixel of Grand daughter board on this line

									for (DWORD TempLoopX = 0; TempLoopX < TempNumberOfPixelsToCopy2; TempLoopX++)
									{//copy each pixel in the line to this buffer
										*TempDestinationPointer++ = *TempSourcePointer++;
										TempSourcePointer++;//skip every other byte as it was for the first 12 inches
									}
								}
								else
								//if have a Forte, get 24 inch image as 2 - 12 inch images must combine
								if (vLocalConfigurationData->vScanTracType == cForteScanTrac)
								{ //get front 6 (3 on allegro) detectors data.  On Allegro, don't have front 3 detectors, but must read in black pixels, so once read in, reset input counter to overwrite
									// get first 12 inches, which is very back, flipped over image
									//flipped over image
									TempSourcePointer = TempFrameBuffer + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors) - 1 ; //go to the last pixel of the line

									TempDestinationPointer = vAFramePointer + (vAFrameCounter * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors); //start at beginning of memory for image
					
									for (DWORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vAquisitionSizeX; TempLoopX++)
									{//copy each pixel in the line to this buffer
										*TempDestinationPointer++ = *TempSourcePointer--; //goes into front of image on a TD
										TempSourcePointer--;//skip every other byte as it was for the first 12 inches
									}

									//if have a Forte, get 24 inch image as 2 - 12 inch images must combine
									if (TempPixelsUsedAbove12Inches)
									{
										TempSourcePointer = TempFrameBuffer + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors) - vLocalSystemData->vITIPCDig->vBytesPerPixel;

										TempPixelsUsedAbove12Inches = 384; //must copy all pixels to calibrate

										for (DWORD TempLoopX = 0; TempLoopX < TempPixelsUsedAbove12Inches; TempLoopX++)
										{//copy each pixel in the line to this buffer
											*TempDestinationPointer++ = *TempSourcePointer--;
											TempSourcePointer--;//skip every other byte as it was for the first 12 inches
										}
									}
								}
								else
								if (vLocalConfigurationData->vScanTracType == cAllegroScanTrac)
								{ //get front 6 (3 on allegro) detectors data.  On Allegro, don't have front 3 detectors, but must read in black pixels, so once read in, reset input counter to overwrite, flipped over image
									//flipped over image
									TempSourcePointer = TempFrameBuffer + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeX * 2) - 1- (192 * 2); //go to the last pixel of the line then skip the first 192 pixels
									if (vLocalConfigurationData->vPixelsPerDetector == 128)
										TempSourcePointer = TempSourcePointer - (192 * 2);

									TempDestinationPointer = vAFramePointer + (vAFrameCounter * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors); //go to the first pixel of the line

									WORD TempNumberOfPixelsToCopy = 192; //only 3 detectors at 64 pixels per detector
									if (vLocalConfigurationData->vPixelsPerDetector == 128)
										TempNumberOfPixelsToCopy = 384; //only 3 detectors at 128 pixels per detector 
									for (DWORD TempLoopX = 0; TempLoopX < TempNumberOfPixelsToCopy; TempLoopX++)
									{//copy each pixel in the line to this buffer
										*TempDestinationPointer++ = *TempSourcePointer--; //goes into front of image on a TD, bottom of image on a solo
										TempSourcePointer--;//skip every other byte as it was for the last 12 inches
									}

									//get back 12 inches on Allegro
									if ((TempPixelsUsedAbove12Inches) || (vLocalSystemData->vITIPCDig->vAquisitionSizeX + vLocalSystemData->vITIPCDig->vAquisitionROIBottom > TempNumberOfPixelsToCopy))
									{
										TempSourcePointer = TempFrameBuffer + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeX * 2) - vLocalSystemData->vITIPCDig->vBytesPerPixel;

										TempPixelsUsedAbove12Inches = 384; //must copy all pixels to calibrate
										if (vLocalConfigurationData->vPixelsPerDetector == 128)
											TempPixelsUsedAbove12Inches = TempPixelsUsedAbove12Inches * 2;

										for (DWORD TempLoopX = 0; TempLoopX < TempPixelsUsedAbove12Inches; TempLoopX++)
										{//copy each pixel in the line to this buffer
											*TempDestinationPointer++ = *TempSourcePointer--;
											TempSourcePointer--;//skip every other byte as it was for the first 12 inches
										}
									}
								}
								else
								if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
								{
									//if (vLocalSystemData->vITIPCDig->vAquisitionROITop > 384) //only copy this part if image uses back 12 inches, from uC copy to back to top part of image
									{
										// get first 12 inches, which is very back, flipped over image
										//this gets the uC part of the image and puts it in the top (pixel 384 to 575)
										//flip over image, read it out backwards
										TempSourcePointer = TempFrameBuffer + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeX * 4) - 4; //4 bytes per pixel

										TempDestinationPointer = vAFramePointer + (vAFrameCounter * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors) + 384;

										//copy 12 inches to bottom of image, which is front of case inspector
										for (DWORD TempLoopX = 0; TempLoopX < 192; TempLoopX++)
										{//copy each pixel in the line to this buffer
											if ((TempDestinationPointer > vAFramePointer + vLocalSystemData->vITIPCDig->vAquisitionBufferSize) || (TempDestinationPointer < vAFramePointer) ||
												(TempSourcePointer > TempFrameBuffer + vLocalSystemData->vITIPCDig->vAquisitionBufferSize) || (TempSourcePointer < TempFrameBuffer))
												ReportErrorMessage("TempFrameBuffer Pointer Error 1", cEMailInspx, 32000);
											else
												*TempDestinationPointer++ = *TempSourcePointer;

											TempSourcePointer = TempSourcePointer - 4; //4 bytes per pixel
										}
									}

									//second section goes in middle
									//if ((vLocalSystemData->vITIPCDig->vAquisitionROITop > 192) && (vLocalSystemData->vITIPCDig->vAquisitionROIBottom < 385)) //only copy if image uses middle 12 inches
									//if (TempPixelsUsedAbove12Inches)  //need to copy pixels from daughter card from other image in Trio
									{//the middle 12 inches of the image is equivelent to the C and D frame of a trio image
										//flip over image, read it out backwards
										TempSourcePointer = TempFrameBuffer + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeX * 4) - 2;

										TempDestinationPointer = vAFramePointer + (vAFrameCounter * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors) + 192;

										//copy 12 inches to center of image
										for (DWORD TempLoopX = 0; TempLoopX < 192; TempLoopX++)
										{//copy each pixel in the line to this buffer
											if ((TempDestinationPointer > vAFramePointer + vLocalSystemData->vITIPCDig->vAquisitionBufferSize) || (TempDestinationPointer < vAFramePointer) ||
												(TempSourcePointer > TempFrameBuffer + vLocalSystemData->vITIPCDig->vAquisitionBufferSize) || (TempSourcePointer < TempFrameBuffer))
												ReportErrorMessage("TempFrameBuffer Pointer Error 2", cEMailInspx, 32000);
											else
												*TempDestinationPointer++ = *TempSourcePointer;
											TempSourcePointer = TempSourcePointer - 4; //4 bytes per pixel
										}
									}

									//need to copy pixels from center image of Trio, equivelent to an e or f frame in a trio and put at bottom or front of image
									//if (vLocalSystemData->vITIPCDig->vAquisitionROIBottom < 192) //only copy if image uses bottom or front 12 inches
									{
										TempSourcePointer = TempFrameBuffer - 3 + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeX * 4);

										BYTE *TempSourcePointerIL = TempFrameBuffer - 1 + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeX * 4);

										TempDestinationPointer = vAFramePointer + (vAFrameCounter * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors);

										//copy 12 inches to top of image which is back of case inspector
										for (DWORD TempLoopX = 0; TempLoopX < 192; TempLoopX++)
										{//copy each pixel in the line to this buffer
											if ((TempDestinationPointer > vAFramePointer + vLocalSystemData->vITIPCDig->vAquisitionBufferSize) || (TempDestinationPointer < vAFramePointer) ||
												(TempSourcePointer > TempFrameBuffer + vLocalSystemData->vITIPCDig->vAquisitionBufferSize) || (TempSourcePointer < TempFrameBuffer))
												ReportErrorMessage("TempFrameBuffer Pointer Error 3", cEMailInspx, 32000);
											else
												*TempDestinationPointer++ = *TempSourcePointer + (*TempSourcePointerIL << 4);  //get 4 bits from each of 2 image pixels as PC2-CamLinK

											TempSourcePointer = TempSourcePointer - 4;
											TempSourcePointerIL = TempSourcePointerIL - 4;
										}
									}
								}

								vAFrameCounter++;
								//check to see if completed a frame
								if (vAFrameCounter >= vLocalSystemData->vITIPCDig->vAquisitionSizeY)
								{
									CopyMemory(vFinalOutPutBuffer,vAFramePointer, vLocalSystemData->vITIPCDig->vAquisitionBufferSize);
									vAFrameCounter = 0;
									HandleCompletedImage(vFinalOutPutBuffer,TempSequenceNumber);

									if (vLocalSystemData->vLogFile.vLogSerialData)
										vLocalSystemData->vLogFile.WriteToLogFile("Finished an A Frame and Processed, mode: " + dtoa(vLocalSystemData->vSystemRunMode, 0), cDebugMessage);
								}
							}
							else
							{
								//problem if were in middle of an A Frame, but this line is not an A Frame
								if (vAFrameCounter)
								{
									CString TempString = dtoa(vAFrameCounter,0);
									if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
									if (vLocalSystemData->vFPGASimulateEncoderRate == 0)
									if (!vLocalSystemData->vShuttingDownXRays)
									if (vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
									{
										ReportErrorMessage("Line Byte = " + ByteToHex(*TempPointerToExtraPixel), cWriteToLog,0);
										ReportErrorMessage("Error Not an A Frame but Line Counter = " + TempString + " of " + dtoa(vLocalSystemData->vITIPCDig->vAquisitionSizeY,0), cWriteToLog,0);

										if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
										if (!vMainWindowPointer->CurrentProductIsInspxProduct())
										{
											if (!vMainWindowPointer->CurrentProductIsInspxProduct())
											if (vGlobalCurrentProduct->vReferenceWidth)
											if (vGlobalCurrentProduct->vReferenceWidth * 1.9 < vGlobalCurrentProduct->vImageWidthPosition)
												//ReportErrorMessage("Problem because Image Width is more than two containers wide", cEMailInspx,32000);
												ReportErrorMessage("Setup Problem-Frame too wide for container, use Autosize or check re-trigger lockout", cEMailMaintenance,0);
											else
												ReportErrorMessage("Error Not an A Frame but Line Counter = " + TempString + " of " + dtoa(vLocalSystemData->vITIPCDig->vAquisitionSizeY,0) + ", Frame width OK", cEMailInspx,32000);
										}
										if (PasswordOK(cTemporaryInspxPassword,false))
										if (vLocalSystemData->vLogFile.vLogSerialData)
											OutputImageToCSVFile("C:\\ScanTracSerialLogs\\FrameErrorA", TempFrameBuffer, 
												vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors, vLocalSystemData->vITIPCDig->vAquisitionSizeY);

										if (vLocalSystemData->vSystemRunMode == cCalibratingDetectorOffsetMode)
										{
											vGlobalErrorCount++;
											if (vGlobalErrorCount < 5)
											{
												vMainWindowPointer->StopRunning(true);
												vMainWindowPointer->vSelectProductAgain = true;
												ReportErrorMessage("Error, Corrupt image, Initializing and trying again.", cEMailInspx,32000);
											}
											else
											{
												vMainWindowPointer->StopRunning(true);
												ReportErrorMessage("Error, Corrupt image, reseat frame grabber and U cable", cError, 0);
												vGlobalErrorCount = 0;
											}

										}
										else
										if (vAFrameCounter > vGlobalCurrentProduct->vImageWidthPosition / 2.5)
										{
											//clear any lines you have not recieved
											TempDestinationPointer = vAFramePointer + (vAFrameCounter * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors);

											BYTE TempValue = 255;
											if (PasswordOK(cTemporaryInspxPassword,false))
												TempValue = 180;

											FillMemory(TempDestinationPointer, vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors * 
												(vLocalSystemData->vITIPCDig->vAquisitionSizeY - vAFrameCounter), TempValue);

											CopyMemory(vFinalOutPutBuffer,vAFramePointer, vLocalSystemData->vITIPCDig->vAquisitionBufferSize);

											ReportErrorMessage("Process Partial A Frame but Line Counter = " + TempString, cWriteToLog,0);

											HandleCompletedImage(vFinalOutPutBuffer,TempSequenceNumber);
										}
										else
											ReportErrorMessage("Ignore Partial A Frame but Line Counter = " + TempString, cWriteToLog,0);
									}
									vAFrameCounter = 0;
								}
							}

							//if ((TempPointerToExtraPixel < vLocalSystemData->vITIPCDig->vAquisitionBuffer) || (TempPointerToExtraPixel >= vLocalSystemData->vITIPCDig->vAquisitionBufferMaximum))
							//	ReportErrorMessage("Aquisition Buffer Pointer Error A: " + dtoa(vAFrameCounter,0), cEMailInspx,32000);
							//else
							if ((*TempPointerToExtraPixel & 2) == 2)
							{//this is an B Frame line, so copy it to the B Frame Buffer
								if (vLocalSystemData->vLogFile.vLogSerialData)
									vLocalSystemData->vLogFile.WriteToLogFile("Line: " + dtoa(TempLoopY,0) + " Got B Line: " + 
										dtoa(vBFrameCounter,0) + " HW Frame: " + dtoa(vPreviousHardwareFrameCount,0) + " EP: " + ByteToHex(*TempPointerToExtraPixel), cDebugMessage);

								if (vLocalConfigurationData->vNumberOfXRayDetectors == 6)
								{
									TempDestinationPointer = vBFramePointer - 1 + ((vBFrameCounter + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors);
									TempSourcePointer = TempFrameBuffer + (TempLoopY * vLocalSystemData->vITIPCDig->vBytesPerPixel * vLocalSystemData->vITIPCDig->vAquisitionSizeX);

									for (DWORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vAquisitionSizeX; TempLoopX++)
									{//copy each pixel in the line to this buffer
										*TempDestinationPointer-- = *TempSourcePointer++;
									}
								}
								else
								if (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)
								{ //get lower 6 detectors data (don't flip over as flipped from solo.
									TempDestinationPointer = vBFramePointer + (vBFrameCounter * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors); //go to the first pixel of the line

									TempSourcePointer = TempFrameBuffer + (TempLoopY * vLocalSystemData->vITIPCDig->vAquisitionSizeX * 2); //go to the last pixel of the line then skip the first 192 pixels
									WORD TempNumberOfPixelsToCopy1 = 384;
									if (vLocalConfigurationData->vPixelsPerDetector == 128)
										TempNumberOfPixelsToCopy1 = 768;


									for (DWORD TempLoopX = 0; TempLoopX < TempNumberOfPixelsToCopy1; TempLoopX++)
									{//copy each pixel in the line to this buffer
										*TempDestinationPointer++ = *TempSourcePointer++; //read straight in
										TempSourcePointer++;//skip every other byte as it was for the last 12 inches
									}

									WORD TempNumberOfPixelsToCopy2 = 192;
									if (vLocalConfigurationData->vPixelsPerDetector == 128)
										TempNumberOfPixelsToCopy2 = 384;

									//get top 3 detectors worth of the image
									TempSourcePointer = TempFrameBuffer + 1 + (TempLoopY * vLocalSystemData->vITIPCDig->vAquisitionSizeX * 2); //gets to first pixel of Grand daughter board on this line

									for (DWORD TempLoopX = 0; TempLoopX < TempNumberOfPixelsToCopy2; TempLoopX++)
									{//copy each pixel in the line to this buffer
										*TempDestinationPointer++ = *TempSourcePointer++;
										TempSourcePointer++;//skip every other byte as it was for the first 12 inches
									}
								}
								else
								//if have a Forte, get 24 inch image as 2 - 12 inch images must combine
								if (vLocalConfigurationData->vScanTracType == cForteScanTrac)
								{ //get front 6 (3 on allegro) detectors data.  On Allegro, don't have front 3 detectors, but must read in black pixels, so once read in, reset input counter to overwrite
									// get first 12 inches, which is very back, flipped over image
									//flipped over image
									TempSourcePointer = TempFrameBuffer + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors) - 1; //start at the end of this line

									TempDestinationPointer = vBFramePointer + (vBFrameCounter * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors); //start at beginning of memory for image

									for (DWORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vAquisitionSizeX; TempLoopX++)
									{//copy each pixel in the line to this buffer
										*TempDestinationPointer++ = *TempSourcePointer--; //goes into front of image on a TD
										TempSourcePointer--;//skip every other byte as it was for the first 12 inches
									}

									//if have a Forte, get 24 inch image as 2 - 12 inch images must combine
									if (TempPixelsUsedAbove12Inches)
									{
										TempSourcePointer = TempFrameBuffer + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors) - vLocalSystemData->vITIPCDig->vBytesPerPixel;

										TempPixelsUsedAbove12Inches = 384; //must copy all pixels to calibrate

										for (DWORD TempLoopX = 0; TempLoopX < TempPixelsUsedAbove12Inches; TempLoopX++)
										{//copy each pixel in the line to this buffer
											*TempDestinationPointer++ = *TempSourcePointer--;
											TempSourcePointer--;//skip every other byte as it was for the first 12 inches
										}
									}
								}
								else
								if (vLocalConfigurationData->vScanTracType == cAllegroScanTrac)
								{ //get front 6 (3 on allegro) detectors data.  On Allegro, don't have front 3 detectors, but must read in black pixels, so once read in, reset input counter to overwrite, flipped over image
									//flipped over image
									TempSourcePointer = TempFrameBuffer + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeX * 2) - 1 - (192 * 2); //go to the last pixel of the line
									if (vLocalConfigurationData->vPixelsPerDetector == 128)
										TempSourcePointer = TempSourcePointer - (192 * 2);

									TempDestinationPointer = vBFramePointer + (vBFrameCounter * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors); //go to the first pixel of the line

									WORD TempNumberOfPixelsToCopy = 192; //only 3 detectors at 64 pixels per detector
									if (vLocalConfigurationData->vPixelsPerDetector == 128)
										TempNumberOfPixelsToCopy = 384; //only 3 detectors at 128 pixels per detector 
									for (DWORD TempLoopX = 0; TempLoopX < TempNumberOfPixelsToCopy; TempLoopX++)
									{//copy each pixel in the line to this buffer
										*TempDestinationPointer++ = *TempSourcePointer--; //goes into front of image on a TD, bottom of image on a solo
										TempSourcePointer--;//skip every other byte as it was for the last 12 inches
									}

									//get back 12 inches on Allegro
									if ((TempPixelsUsedAbove12Inches) || (vLocalSystemData->vITIPCDig->vAquisitionSizeX + vLocalSystemData->vITIPCDig->vAquisitionROIBottom > TempNumberOfPixelsToCopy))
									{
										TempSourcePointer = TempFrameBuffer + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeX * 2) - vLocalSystemData->vITIPCDig->vBytesPerPixel;

										TempPixelsUsedAbove12Inches = 384; //must copy all pixels to calibrate
										if (vLocalConfigurationData->vPixelsPerDetector == 128)
											TempPixelsUsedAbove12Inches = TempPixelsUsedAbove12Inches * 2;

										for (DWORD TempLoopX = 0; TempLoopX < TempPixelsUsedAbove12Inches; TempLoopX++)
										{//copy each pixel in the line to this buffer
											*TempDestinationPointer++ = *TempSourcePointer--;
											TempSourcePointer--;//skip every other byte as it was for the first 12 inches
										}
									}
								}
								else
								if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
								{
									//if (vLocalSystemData->vITIPCDig->vAquisitionROITop > 384) //only copy this part if image uses back 12 inches, from uC copy to back to top part of image
									{
										// get first 12 inches, which is very back, flipped over image
										//this gets the uC part of the image and puts it in the top (pixel 384 to 575)
										//flip over image, read it out backwards
										TempSourcePointer = TempFrameBuffer + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeX * 4) - 4;//4 bytes per pixel

										TempDestinationPointer = vBFramePointer + (vBFrameCounter * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors) + 384;

										//copy 12 inches to bottom of image, which is front of case inspector
										for (DWORD TempLoopX = 0; TempLoopX < 192; TempLoopX++)
										{//copy each pixel in the line to this buffer
											if ((TempDestinationPointer > vBFramePointer + vLocalSystemData->vITIPCDig->vAquisitionBufferSize) || (TempDestinationPointer < vBFramePointer) ||
												(TempSourcePointer > TempFrameBuffer + vLocalSystemData->vITIPCDig->vAquisitionBufferSize) || (TempSourcePointer < TempFrameBuffer))
												ReportErrorMessage("TempFrameBuffer Pointer Error 1", cEMailInspx, 32000);
											else
												*TempDestinationPointer++ = *TempSourcePointer;

											TempSourcePointer = TempSourcePointer - 4; //4 bytes per pixel
										}
									}

									//second section goes in middle
									//if ((vLocalSystemData->vITIPCDig->vAquisitionROITop > 192) && (vLocalSystemData->vITIPCDig->vAquisitionROIBottom < 385)) //only copy if image uses middle 12 inches
									//if (TempPixelsUsedAbove12Inches)  //need to copy pixels from daughter card from other image in Trio
									{//the middle 12 inches of the image is equivelent to the C and D frame of a trio image
										//flip over image, read it out backwards
										TempSourcePointer = TempFrameBuffer + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeX * 4) - 2;

										TempDestinationPointer = vBFramePointer + (vBFrameCounter * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors) + 192;
									
										//copy 12 inches to center of image
										for (DWORD TempLoopX = 0; TempLoopX < 192; TempLoopX++)
										{//copy each pixel in the line to this buffer
											if ((TempDestinationPointer > vBFramePointer + vLocalSystemData->vITIPCDig->vAquisitionBufferSize) || (TempDestinationPointer < vBFramePointer) ||
												(TempSourcePointer > TempFrameBuffer + vLocalSystemData->vITIPCDig->vAquisitionBufferSize) || (TempSourcePointer < TempFrameBuffer))
												ReportErrorMessage("TempFrameBuffer Pointer Error 2", cEMailInspx, 32000);
											else
												*TempDestinationPointer++ = *TempSourcePointer;
											TempSourcePointer = TempSourcePointer - 4; //4 bytes per pixel
										}
									}

									//need to copy pixels from center image of Trio, equivelent to an e or f frame in a trio and put at bottom or front of image
									//if (vLocalSystemData->vITIPCDig->vAquisitionROIBottom < 192) //only copy if image uses bottom or front 12 inches
									{
										TempSourcePointer = TempFrameBuffer - 3 + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeX * 4);

										BYTE *TempSourcePointerIL = TempFrameBuffer - 1 + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeX * 4);

										TempDestinationPointer = vBFramePointer + (vBFrameCounter * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors);

										//copy 12 inches to top of image which is back of case inspector
										for (DWORD TempLoopX = 0; TempLoopX < 192; TempLoopX++)
										{//copy each pixel in the line to this buffer
											if ((TempDestinationPointer > vBFramePointer + vLocalSystemData->vITIPCDig->vAquisitionBufferSize) || (TempDestinationPointer < vBFramePointer) ||
												(TempSourcePointer > TempFrameBuffer + vLocalSystemData->vITIPCDig->vAquisitionBufferSize) || (TempSourcePointer < TempFrameBuffer))
												ReportErrorMessage("TempFrameBuffer Pointer Error 3", cEMailInspx, 32000);
											else
												*TempDestinationPointer++ = *TempSourcePointer + (*TempSourcePointerIL << 4);  //get 4 bits from each of 2 image pixels as PC2-CamLinK

											TempSourcePointer = TempSourcePointer - 4;
											TempSourcePointerIL = TempSourcePointerIL - 4;
										}
									}
								}

								vBFrameCounter++;
								//check to see if completed a frame
								if (vBFrameCounter >= vLocalSystemData->vITIPCDig->vAquisitionSizeY)
								{
									CopyMemory(vFinalOutPutBuffer,vBFramePointer, vLocalSystemData->vITIPCDig->vAquisitionBufferSize);
									vBFrameCounter = 0;
									HandleCompletedImage(vFinalOutPutBuffer, TempSequenceNumber);

									if (vLocalSystemData->vLogFile.vLogSerialData)
										vLocalSystemData->vLogFile.WriteToLogFile("Finished a B Frame and Processed, mode: " + dtoa(vLocalSystemData->vSystemRunMode, 0),cDebugMessage);
								}
							}
							else
							{
								if (vBFrameCounter)
								{  //this line is not a B Frame, but were counting into a B Frame so lost it
									CString TempString = dtoa(vBFrameCounter,0);
									if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
									if (vLocalSystemData->vFPGASimulateEncoderRate == 0)
									if (!vLocalSystemData->vShuttingDownXRays)
									if (vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
									{
										ReportErrorMessage("Line Byte = " + ByteToHex(*TempPointerToExtraPixel), cWriteToLog,0);
										ReportErrorMessage("Error Not a B Frame but Line Counter = " + TempString + " of " + dtoa(vLocalSystemData->vITIPCDig->vAquisitionSizeY,0), cWriteToLog,0);

										if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
										if (!vMainWindowPointer->CurrentProductIsInspxProduct())
										{
											if (!vMainWindowPointer->CurrentProductIsInspxProduct())
											if (vGlobalCurrentProduct->vReferenceWidth)
											if (vGlobalCurrentProduct->vReferenceWidth * 1.9 < vGlobalCurrentProduct->vImageWidthPosition)
												//ReportErrorMessage("Problem because Image Width is more than two containers wide", cEMailInspx,32000);
												ReportErrorMessage("Setup Problem-Frame too wide for container, use Autosize or check re-trigger lockout", cEMailMaintenance,0);
											else
												ReportErrorMessage("Error Not a B Frame but Line Counter = " + TempString + " of " + dtoa(vLocalSystemData->vITIPCDig->vAquisitionSizeY,0) + ", Frame width OK", cEMailInspx,32000);
										}
										if (PasswordOK(cTemporaryInspxPassword,false))
										if (vLocalSystemData->vLogFile.vLogSerialData)
											OutputImageToCSVFile("C:\\ScanTracSerialLogs\\FrameErrorB", TempFrameBuffer, 
												vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors, vLocalSystemData->vITIPCDig->vAquisitionSizeY);

										if (vLocalSystemData->vSystemRunMode == cCalibratingDetectorOffsetMode)
										{
											vMainWindowPointer->StopRunning(true);
											vMainWindowPointer->vSelectProductAgain = true;
											ReportErrorMessage("Error, Corrupt image, Initializing and trying again.", cEMailInspx,32000);
										}
										else
										if (vBFrameCounter > vGlobalCurrentProduct->vImageWidthPosition / 2.5)
										{
											//clear any lines you have not recieved
											TempDestinationPointer = vBFramePointer + ((vBFrameCounter) * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors);

											BYTE TempValue = 255;
											if (PasswordOK(cTemporaryInspxPassword,false))
												TempValue = 180;

											FillMemory(TempDestinationPointer, vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors * (vLocalSystemData->vITIPCDig->vAquisitionSizeY - vBFrameCounter), TempValue);

											CopyMemory(vFinalOutPutBuffer,vBFramePointer, vLocalSystemData->vITIPCDig->vAquisitionBufferSize);

											ReportErrorMessage("Process Partial B Frame but Line Counter = " + TempString, cWriteToLog,0);

											HandleCompletedImage(vFinalOutPutBuffer,TempSequenceNumber);
										}
										else
											ReportErrorMessage("Ignore Partial B Frame but Line Counter = " + TempString, cWriteToLog,0);
									}
									vBFrameCounter = 0;
								}
							}
							if (vLocalSystemData->vLogFile.vLogSerialData)
							//if ((!TempLoopY) || 
							//	(TempLoopY == (DWORD)(vLocalSystemData->vITIPCDig->vAquisitionSizeY - 1)))
							if ((*TempPointerToExtraPixel & 3) == 0)
									vLocalSystemData->vLogFile.WriteToLogFile("Line: " + dtoa(TempLoopY,0) + " Got Neither Line A: " + dtoa(vAFrameCounter,0) + ", B: " +
										dtoa(vBFrameCounter,0) + " HW Frame: " + dtoa(vPreviousHardwareFrameCount,0) + " EP: " + ByteToHex(*TempPointerToExtraPixel), cDebugMessage);
						}
					}
					//processed any frames done, so clear seqenceNumber flag
					TempSequenceNumber = -1;
				}
			}
			//if not doing back to back, process any frame got.
			//this should never happen
			if (TempSequenceNumber >= 0)
			{
				if (vLocalSystemData->vLogFile.vLogSerialData)
				{
					vLocalSystemData->vLogFile.WriteToLogFile("Error-162A  In Non Back To Back Code",cDebugMessage);
				}
				if (vLoadedThisImageFromFile)
				{//don't flip the image if from file
					for (DWORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vAquisitionSizeY; TempLoopY++)
					{//check the last byte of each line, this is the extra pixel
						CopyMemory(vFinalOutPutBuffer + (TempLoopY * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX),
							TempFrameBuffer + (TempLoopY * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX), vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
					}
				}
				else
				{//flip the image if from detector
					for (DWORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vAquisitionSizeY; TempLoopY++)
					{//check the last byte of each line, this is the extra pixel
						BYTE *TempDestinationPointer = vFinalOutPutBuffer - 1 + ((TempLoopY + 1) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
						BYTE *TempSourcePointer = TempFrameBuffer + (TempLoopY * vLocalSystemData->vITIPCDig->vBytesPerPixel * vLocalSystemData->vITIPCDig->vAquisitionSizeX);
						for (DWORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vAquisitionSizeX; TempLoopX++)
						{//copy each pixel in the line to this buffer
							*TempDestinationPointer-- = *TempSourcePointer++;
						}
					}
				}//NON BACK TO BACK MODE, SHOULD NOT HAPPEN
				HandleCompletedImage(vFinalOutPutBuffer,TempSequenceNumber);
			}
			//start timer to measure how long until start processing next image
			vStartFreeTime = CIFCOS::GetSystimeMicrosecs();
		}  //end of loop that keep taking images
#ifndef IFC59Driver
		if (vLocalSystemData->vITIPCDig->vCamera != NULL)
			vLocalSystemData->vITIPCDig->vCamera->Freeze(grabID);
#endif
	}
	//this signals to the main program that the thread exited

	if (vAFramePointer)
		free(vAFramePointer);
	vAFramePointer = NULL;
	if (vBFramePointer)
		free(vBFramePointer);
	vBFramePointer = NULL;
	if (vFinalOutPutBuffer)
		free(vFinalOutPutBuffer);
	vFinalOutPutBuffer = NULL;

	vShutDownNow = true;
	vGlobalImageAquisitionThread = NULL;

	return 0;
}

void CImageAquisitionThread::HandleCompletedImage(BYTE *TempFrameBuffer, int TempSequenceNumber)
{
	//if (!vLocalSystemData->vITIPCDig->vShowOnlyRealImages) //if simulating images, pretend got a Seamer Object Pulse if using FIFO in sampling
	//if (vLocalSystemData->vUseSamplingFIFO)
	//if (vGlobaluCSerialPort)
	//	vGlobaluCSerialPort->HandleSeamerObjectPulse();

	if (vLocalSystemData->vLogFile.vLogSerialData)
	{
		vLocalSystemData->vLogFile.WriteToLogFile("*IAT-PI-HandleCompletedImage:" + dtoa(vLocalSystemData->vImageCounter,0),cDebugMessage);
	}
	if (vLocalSystemData->vReCalibrationTimerRunning)
	if (!vLocalSystemData->vAutoCalibrateWhileRunning)
		::PostThreadMessage(vGlobalCallingThreadID,cKillCalibrationTimerMessage,0,0);
	vLocalSystemData->vITIPCDig->vHaveImage = true;
	if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem) || (vLoadedThisImageFromFile))
		vLocalSystemData->vFrameCount++;

	if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
	{
		if (vLocalSystemData->vLastContainerImaged)
			vLocalSystemData->vFrameCountString = dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0);
		else
			vLocalSystemData->vFrameCountString = "f=" + dtoa(vLocalSystemData->vFrameCount,0);
		vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
	}

	if ((vLocalConfigurationData->vScanTracType == cForteScanTrac) && (!vLocalSystemData->vABPixelAtOtherSide))
	{
		if (vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors > vLocalSystemData->vITIPCDig->vAquisitionSizeX)
		{
			BYTE* TempPointer0 = TempFrameBuffer + vLocalSystemData->vITIPCDig->vAquisitionSizeX; //is last pixel on line of first 6 detectors
			BYTE *TempPointer1 = TempPointer0 + 1;
			for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vAquisitionSizeY; TempLoopY++)
			{
				*TempPointer0 = *TempPointer1;
				TempPointer0 = TempPointer0 + vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors;
				TempPointer1 = TempPointer1 + vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors;
			}
		}
	}
	else
	if ((vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) && (!vLocalSystemData->vABPixelAtOtherSide))
	{
		if (vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors > vLocalSystemData->vITIPCDig->vAquisitionSizeX)
		{
			BYTE* TempPointer0 = TempFrameBuffer + 384; //is last pixel on line of first 3 detectors
			BYTE *TempPointer1 = TempPointer0 - 1;
			for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vAquisitionSizeY; TempLoopY++)
			{
				*TempPointer0 = *TempPointer1;
				TempPointer0 = TempPointer0 + vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors;
				TempPointer1 = TempPointer1 + vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors;
			}
		}
	}

	vLocalSystemData->vITIPCDig->vLastBufferFilled = TempFrameBuffer;
	vLocalSystemData->vSeqNum = TempSequenceNumber; //if the lane number in MultiLane

	//b rh test code write all images to file to debug
	
	//{
//	CString TempString("ImageToProcess");
//	TempString = TempString + dtoa(TempSequenceNumber, 0) + ".BMP";
//	TempString = "C:\\ScanTracSerialLogs\\" + TempString;
//	BYTE TempLength = TempString.GetLength();
//	if (TempLength > MAX_PATH)
//	{
//		CNoticeDialog TempNoticeDialog;
//		CString TempString = "\n\n\nSetup Name more the MAX_PATH characters, Offset Image save aborted";
//		TempNoticeDialog.vNoticeText = TempString;
//		TempNoticeDialog.vType = cErrorMessage;
//		TempNoticeDialog.DoModal();
//		TempLength = (BYTE)MAX_PATH;
//	}
//	else
//	{
//		LPTSTR TempString1 = TempString.GetBuffer(MAX_PATH); //TempString.GetLength());

//		CImgFile *TempImageFile=IfxCreateImgFile(TempString1);
//		if (TempImageFile) 
//		{
//			TempImageFile->WriteFile(vLocalSystemData->vITIPCDig->vLastBufferFilled,
//				vLocalSystemData->vITIPCDig->vOriginalBufferSizeX,
//				vLocalSystemData->vITIPCDig->vAquisitionSizeY,
//				(WORD)vLocalSystemData->vITIPCDig->vBitsPerPixel);
//			delete TempImageFile;
//		}
//	}
	//}
	
	if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem) || (vLocalSystemData->vLoadedFileImage))
	{
		ProcessImage();
	}
	else
	if ((vLocalSystemData->vSystemRunMode == cCalibratingDetectorEdgesMode) ||
		(vLocalSystemData->vSystemRunMode == cMeasureSourceStrengthMode) ||
		(vLocalSystemData->vSystemRunMode == cCalibratingDetectorGainMode) ||
		(vLocalSystemData->vSystemRunMode == cTestShutterMode) ||
		(vLocalSystemData->vSystemRunMode == cAlignTubeMode) ||
		(vLocalSystemData->vSystemRunMode == cCalibratingDetectorOffsetMode) ||
		(vLocalSystemData->vSystemRunMode == cCalibrateEncoderMode))
	{
		ProcessCalibrationImage();
	}

	Sleep(1); //is the equivelent of a yeild statement;
}

int CImageAquisitionThread::ProcessImage()
{
	double TempStartProcessTime = CIFCOS::GetSystimeMicrosecs();
	vSizeOK = true;
	int TempInspectionTopReferenceAdjust = 0;
  // newframe
	vLocalSystemData->vImageCounter++;
	if (vLocalSystemData->vLogFile.vLogSerialData)
	{
		vLocalSystemData->vLogFile.WriteToLogFile("*IAT-PI-New Image:" + dtoa(vLocalSystemData->vImageCounter,0),cDebugMessage);
	}
	if (!vLocalSystemData->vITIPCDig->vLatestImage)
		ReportErrorMessage("Error - vLatestImage->pix is null before adjust image",cEMailInspx,32000);
	else
	if (!vLocalSystemData->vITIPCDig->vLatestImage->pix)
		ReportErrorMessage("Error - vLatestImage->pix is null before adjust image",cEMailInspx,32000);

	vMyLock = true;
	if (!vLocalConfigurationData->vAutoImproveEnabled)
		vGlobalLearnState = 0;

	//The Lock prevents this serial thread from changing the pointer while
	//this thread is processing the image for it
	if (vLocalSystemData->vITIPCDig->vLatestImageCopy)
	if (vLocalSystemData->vITIPCDig->vLatestImage)
	if (vLocalSystemData->vITIPCDig->vDesensitizeImage)
	if (vLocalSystemData->vITIPCDig->vDesensitizeImage->pix)
	if (vLocalSystemData->vITIPCDig->vLatestImageCopy->pix)
	if (vLocalSystemData->vITIPCDig->vLatestImage->pix)
	if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
	{
		if ((vLocalSystemData->vITIPCDig->vHaveRealImage) || (!vLocalSystemData->vITIPCDig->vShowOnlyRealImages) || (vLocalSystemData->vITIPCDig->vProcessNextImage) || 
			(vLocalSystemData->vSimulatingCamLinkImage))
		{
			//if don't have the pointer to the container yet, and it is a real image
			//then find the next container it should be
			if (vGlobalCurrentProduct)
			if (((vLoadedThisImageFromFile) || (!vLocalSystemData->vITIPCDig->vShowOnlyRealImages)) && (!vGlobalCurrentProduct->vMultiLaneNumberOfLanes))
			{  //simulating images mode
				vMyLock = true;
				if ((vLoadedThisImageFromFile) || (vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
				{
					//WORD TempContainerCount = vLocalSystemData->vContainerCollection->GetCount();

					if (vGlobaluCSerialPort)
						vGlobaluCSerialPort->AddNewContainer(vLocalSystemData->vCurrentBeltPosition - 100);

					if (vLocalSystemData->vContainerCollection->GetCount() > 0)
					{
						vLocalSystemData->vLastContainerImaged = vLocalSystemData->vContainerCollection->GetTail();

						if (vLocalSystemData->vLastContainerImaged)
						{
							vLocalSystemData->vLastContainerImaged->vContainerNumber = vLocalSystemData->vFrameCount;
							vLocalSystemData->vLastBodyTriggerGapLength = vLocalSystemData->vLastContainerImaged->vLastBodyTriggerGapLength;

							if (vGlobaluCSerialPort)
								vGlobaluCSerialPort->AddedNewContainer_FillInData(vLocalSystemData->vLastContainerImaged, vLocalSystemData->vCurrentBeltPosition - 100);
						}
					}
					else
						ReportErrorMessage("Error-Add Container Failed",cEMailInspx,32000);

					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("IAT-ImageLoop Add Simulated Container Object",cDebugMessage);

					if (vLocalSystemData->vLastContainerImaged)
					{
						vLocalSystemData->vLastContainerImaged->vImageReceived = true;
						vLocalSystemData->vLastBodyTriggerGapLength = vLocalSystemData->vLastContainerImaged->vLastBodyTriggerGapLength;
					}
				}
			}
			else
			if ((!vLocalSystemData->vLastContainerImaged) && ((vLocalSystemData->vITIPCDig->vHaveRealImage) || (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)))
			{
				vLocalSystemData->vLastContainerImaged = GetNextContainerForImageProcessing();

				if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes) 
				if (!vLocalSystemData->vLastContainerImaged)
				{
					if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
					{
						vLocalSystemData->vValidBodyTriggersCount++;
						if (vGlobaluCSerialPort)
						{
							vLocalSystemData->vFrameCount++;
							vGlobaluCSerialPort->AddNewContainer(vLocalSystemData->vMultiLaneLastTriggerPositionForThisImage);

							if (vLocalSystemData->vLogFile.vLogSerialData)
								vGlobaluCSerialPort->WriteToLogFileWithBeltPosition("Multi Lane-Add Container at Trigger Location: " + dtoa(vLocalSystemData->vMultiLaneLastTriggerPositionForThisImage, 0), 
									vLocalSystemData->vCurrentBeltPosition);

							vLocalSystemData->vLastContainerImaged = vLocalSystemData->vContainerCollection->GetTail();
							vGlobaluCSerialPort->vPreviousContainer = vGlobaluCSerialPort->vCurrentContainer;
							vGlobaluCSerialPort->vCurrentContainer = vLocalSystemData->vLastContainerImaged;
							vLocalSystemData->vLastBodyTriggerGapLength = 11;
						}
						vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();

						if (vLocalSystemData->vReCalibrationTimerRunning)
						if (!vLocalSystemData->vAutoCalibrateWhileRunning)
							::PostThreadMessage(vGlobalCallingThreadID,cKillCalibrationTimerMessage,0,0);
					}
					else
						ReportErrorMessage("Error-Serial Thread Lock Failed 7", cEMailInspx,32000);
				}

				if (vLocalSystemData->vLastContainerImaged)
				{
					vLocalSystemData->vLastBodyTriggerGapLength = vLocalSystemData->vLastContainerImaged->vLastBodyTriggerGapLength;
					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("IAT-PI-Picked Next Container to Analyze c: " + 
							dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0) + " : " + vLocalSystemData->vFrameCountString,cDebugMessage);

					vLocalSystemData->vLastContainerImaged->vImageReceived = true;
				}
				else
				//if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
				if (!vLocalSystemData->vShuttingDownXRays)
				{
					if ((vLocalSystemData->vSystemRunMode != cRunningSystemMode) || (vLocalSystemData->vSimulatingCamLinkImage))
					{  //simulating images mode
						vMyLock = true;
						if ((vLoadedThisImageFromFile) ||(vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
						{
							//WORD TempContainerCount = vLocalSystemData->vContainerCollection->GetCount();

							if (vGlobaluCSerialPort)
								vGlobaluCSerialPort->AddNewContainer(vLocalSystemData->vCurrentBeltPosition - 100);

							if (vLocalSystemData->vContainerCollection->GetCount() > 0)
							{
								vLocalSystemData->vLastContainerImaged = vLocalSystemData->vContainerCollection->GetTail();
								if (vLocalSystemData->vLastContainerImaged)
								{
									vLocalSystemData->vLastContainerImaged->vContainerNumber = vLocalSystemData->vFrameCount;
									vLocalSystemData->vLastBodyTriggerGapLength = vLocalSystemData->vLastContainerImaged->vLastBodyTriggerGapLength;

									if (vGlobaluCSerialPort)
										vGlobaluCSerialPort->AddedNewContainer_FillInData(vLocalSystemData->vLastContainerImaged, vLocalSystemData->vCurrentBeltPosition - 100);
								}
							}
							else
								ReportErrorMessage("Error-Add Container Failed",cEMailInspx,32000);

							if (vLocalSystemData->vLogFile.vLogSerialData)
								vLocalSystemData->vLogFile.WriteToLogFile("IAT-ImageLoop Add Simulated Container Object",cDebugMessage);

							if (vLocalSystemData->vLastContainerImaged)
							{
								vLocalSystemData->vLastContainerImaged->vImageReceived = true;
								vLocalSystemData->vLastBodyTriggerGapLength = vLocalSystemData->vLastContainerImaged->vLastBodyTriggerGapLength;
							}
						}
					}
					else
					{
						ReportErrorMessage("Error - Can't find object for container", cEMailInspx,32000);
						if (vLocalSystemData->vLogFile.vLogSerialData)
							vLocalSystemData->vLogFile.WriteToLogFile("Error - Can't find object for container: " + vLocalSystemData->vFrameCountString,cDebugMessage);
					}
				}
			}
			if (vLocalSystemData->vLastContainerImaged)
				vLocalSystemData->vLastContainerImaged->vRealImage = vLocalSystemData->vITIPCDig->vHaveRealImage;

			if (vLocalSystemData->vLogFile.vLogSerialData)
			if (vLocalSystemData->vLastContainerImaged)
			if (vLocalSystemData->vLastContainerImaged->vRealImage)
			{
				vLocalSystemData->vLogFile.WriteToLogFile("IAT-PI-New Real Image: " + vLocalSystemData->vFrameCountString,cDebugMessage);
			}
			//double TempStartProcessTime = CIFCOS::GetSystimeMicrosecs();
			if (vLocalSystemData->vLastContainerImaged)
				vLocalSystemData->vLastContainerImaged->vToBeEjected = false;
			vLocalSystemData->vITIPCDig->ZeroLastValues();

			vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity = -1;


										//b rh test code
										//WriteImageToHardDisk(vLocalSystemData->vITIPCDig->vLastBufferFilled, "C:\\ScanTracSerialLogs\\ImageProcessing" + dtoa(vLocalSystemData->vSeqNum, 0) + ".bmp", 
										//	vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

			//adjust pixels to linearize image
			double TempStartEdgeAdjustTime = CIFCOS::GetSystimeMicrosecs();
			if (vLocalSystemData->vITIPCDig->vHaveRealImage)
			if (vCorrectDetectorEdges)
			{
				if (vLocalSystemData->vSystemRunMode != cCalibratingDetectorOffsetMode)
				if ((vLocalSystemData->vSystemRunMode != cCalibratingDetectorEdgesMode) ||
					(vLocalSystemData->vApplyGainAndOffsetDuringCalibration))
				if ((vLocalSystemData->vSystemRunMode != cMeasureSourceStrengthMode) ||
					(vLocalSystemData->vApplyGainAndOffsetDuringCalibration))
				if (vLocalSystemData->vSystemRunMode != cAlignTubeMode)
				{
					if (((vGlobalCurrentProduct->vMultiLaneNumberOfLanes) && ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))) && 
						(!vLocalSystemData->vMultiLaneInSetupLanePositionsMenu))
						AdjustEdgeDetectorPixels(true, vLocalSystemData->vSeqNum); //multi lane image
					else
						AdjustEdgeDetectorPixels(true);
				}
			}

			if (vLocalConfigurationData->vOverWriteBadPixel[0][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors] == 0xAAAB)
				ReplaceDeadPixels();

			//clear the A/B Pixel at the bottom of the image
			if ((vGlobalCurrentProduct->vMultiLaneNumberOfLanes == 0) || (vLocalSystemData->vMultiLaneInSetupLanePositionsMenu))
			{
				BYTE *TempPointer = vLocalSystemData->vITIPCDig->vLastBufferFilled;
				for (WORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vAquisitionSizeY; TempLoop++)
				{
					*TempPointer = 255;
					TempPointer = TempPointer + vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors;
				}
			}

			double TempEndTime = CIFCOS::GetSystimeMicrosecs();
			vLocalSystemData->vITIPCDig->vLastEdgeAdjustTime = TempEndTime - TempStartEdgeAdjustTime;
			
			vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust = 0;
			vLocalSystemData->vITIPCDig->vLastRightReferenceAdjust = 0;
			vLocalSystemData->vITIPCDig->vLastTopReferenceAdjust = 0;
			if (!vLocalSystemData->vLastContainerImaged)
			{
				vLocalSystemData->vITIPCDig->vLastTop = 1;
				vLocalSystemData->vITIPCDig->vLastBottom = 2;
				vLocalSystemData->vITIPCDig->vLastRight = 3;
				vLocalSystemData->vITIPCDig->vLastLeft = 4;
			}

			//process container if real, or flag to process
			if (!(vLocalSystemData->vLastContainerImaged))
			if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem) || (vLoadedThisImageFromFile))
			if (vLocalSystemData->vActualEncoderRate)
			if (!vLocalSystemData->vShuttingDownXRays)
			{
				if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
					ReportErrorMessage("Error-IAT-PI-vLastContainerImaged == NULL: " + vLocalSystemData->vFrameCountString, cEMailInspx,32000);
				if (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem)
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("Error-IAT-PI-InSetup-vLastContainerImaged == NULL: " + vLocalSystemData->vFrameCountString,cDebugMessage);
			}

			if ((vLocalSystemData->vLastContainerImaged) || 
				(vLocalSystemData->vSystemRunMode == cCalibratingDetectorEdgesMode) ||
				(vLocalSystemData->vSystemRunMode == cMeasureSourceStrengthMode) ||
				(vLocalSystemData->vSystemRunMode == cCalibratingDetectorGainMode) ||
				(vLocalSystemData->vSystemRunMode == cTestShutterMode) ||
				(vLocalSystemData->vSystemRunMode == cCalibratingDetectorOffsetMode) ||
				(!vLocalSystemData->vITIPCDig->vShowOnlyRealImages) ||
				(vLocalSystemData->vITIPCDig->vProcessNextImage))
			if (vGlobalCurrentProduct)
			{
				if (vLocalSystemData->vLogFile.vLogSerialData)
				{
					if (vLocalSystemData->vLastContainerImaged)
						vLocalSystemData->vLogFile.WriteToLogFile("IAT-PI-Got New Image to Analyze c: " + dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0) + " LBT: " + vLocalSystemData->vFrameCountString,cDebugMessage);
					else
						vLocalSystemData->vLogFile.WriteToLogFile("IAT-PI-Got New Image to Analyze c: " + vLocalSystemData->vFrameCountString,cDebugMessage);
				}

				double TempStartCopyBufferTime = CIFCOS::GetSystimeMicrosecs();

				BYTE *TempSourceBuffer; 
				BYTE *TempDestinationBuffer = vLocalSystemData->vITIPCDig->vLatestImage->pix;

				//copy image to LatestImage
				//rotate Brioso Image 90 degrees
				if ((vGlobalScanTracType == cBriosoScanTrac) && (vLocalSystemData->vLastContainerImaged->vRealImage) && //on Brioso, uController board is mounted on its side, so must swap X and Y from side view ScanTracs.
					(vLocalSystemData->vITIPCDig->vOriginalBufferSizeY == vLocalSystemData->vITIPCDig->vOriginalBufferSizeX))//don't rotate demo images or loaded images
				{ 
					for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY; TempLoopY++)
					{
						TempSourceBuffer = vLocalSystemData->vITIPCDig->vLastBufferFilled + vLocalSystemData->vITIPCDig->vAquisitionROIBottom +
							((vLocalSystemData->vITIPCDig->vAquisitionSizeY - TempLoopY - 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors);

						TempDestinationBuffer = vLocalSystemData->vITIPCDig->vLatestImage->pix + TempLoopY;
						for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX; TempLoopX++)
						{
							*TempDestinationBuffer = *TempSourceBuffer++;  //copy data
							TempDestinationBuffer = TempDestinationBuffer + vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
						}
					}
				}
				else
				{
					if ((vGlobalCurrentProduct->vMultiLaneNumberOfLanes) && ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem)))
					{ //multi lane
						for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY; TempLoopY++)
						{
							CopyMemory(TempDestinationBuffer, vLocalSystemData->vITIPCDig->vLastBufferFilled, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						}
					}
					else
					{ //normal single lane
						for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY; TempLoopY++)
						{
							TempSourceBuffer = vLocalSystemData->vITIPCDig->vLastBufferFilled + vLocalSystemData->vITIPCDig->vAquisitionROIBottom +
								((vLocalSystemData->vITIPCDig->vAquisitionSizeY - TempLoopY - 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors);

							for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX; TempLoopX++)
								*TempDestinationBuffer++ = *TempSourceBuffer++;  //copy data
						}
					}
				}

				if (vLocalConfigurationData->vDemoMode)
				if (!vLocalSystemData->vLastContainerImaged)
					vLocalSystemData->vLastContainerImaged = new CContainer;

				//copy image to object
				if (vLocalSystemData->vLastContainerImaged)
				{
					if (!vLocalSystemData->vLastContainerImaged->vTheImage)
						vLocalSystemData->vLastContainerImaged->vTheImage = new CImageData;
					if (vLocalSystemData->vLastContainerImaged->vTheImage)
					if (ThereIsEnoughMemory(vLocalSystemData->vITIPCDig->vOriginalBufferSize,"Copy Image"))
					{
						vLocalSystemData->vLastContainerImaged->vTheImage->MakeGreyImage(vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						if (vLocalSystemData->vLastContainerImaged->vTheImage->vGreyImage)
							CopyMemory(vLocalSystemData->vLastContainerImaged->vTheImage->vGreyImage, vLocalSystemData->vITIPCDig->vLatestImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						else
						{
							DWORD TempContainersInTunnel = vLocalSystemData->vContainerCollection->GetCount();
							ReportErrorMessage("Error allocating memory for image: " + dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber, 0) + 
								", In Tunnel: " + dtoa(TempContainersInTunnel,0), cEMailInspx, 32000);
						}
						if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
							vLocalSystemData->vLastContainerImaged->vTheImage->vMultiLaneLaneNumber = vLocalSystemData->vSeqNum + 1;
						else
							vLocalSystemData->vLastContainerImaged->vTheImage->vMultiLaneLaneNumber = 0;

						vLocalSystemData->vLastContainerImaged->vTheImage->vMultiLaneEjectorOffset = vMultiLaneEjectorOffset;
					}
				}

				double TempEndTime = CIFCOS::GetSystimeMicrosecs();
				vLocalSystemData->vITIPCDig->vLastCopyBufferTime = TempEndTime - TempStartCopyBufferTime;
				
				if ((vLocalConfigurationData->vRemoveBackgroundFromImage) || (vLocalSystemData->vRemoveBackgroundInAutoSize))
				if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))
				if (!vLoadedThisImageFromFile)
				//if (!vLocalSystemData->vInLowExposureMode)  //will have SizeExposure products'
					vLocalSystemData->vITIPCDig->RemoveBackgroundFromImage(vLocalSystemData->vITIPCDig->vLatestImage);

				//add debugging tracers in the image if enabled
				if (vLocalSystemData->vImageTracerLineOn)
				{
					BYTE *TempPointer = vLocalSystemData->vITIPCDig->vLatestImage->pix + (((vLocalSystemData->vFrameCount % 10) * 10) + 20);
					for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vAquisitionSizeY; TempLoopY++)
					{
						if (TempLoopY % 2)
							*TempPointer = 255;
						else
							*TempPointer  = 0;
						TempPointer = TempPointer + vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
					}
				}

				if (vLocalSystemData->vLastContainerImaged)
				if (vLocalSystemData->vLastContainerImaged->vTheImage)
				if (vLocalSystemData->vLastContainerImaged->vTheImage->vRedImage)
				{
					vLocalSystemData->vLastContainerImaged->vTheImage->MakeRedImage(vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					FillMemory(vLocalSystemData->vLastContainerImaged->vTheImage->vRedImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize, 1);
				}

				//need to blank out pixel 192 or 384 as it is the a / b frame pixel
				//BYTE *TempBuffer = vLocalSystemData->vITIPCDig->vLatestImage->pix;
				//for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vAquisitionSizeY; TempLoopY++)
				//{
				//	*TempBuffer = 0xFF;  //set the pixel white so does not interfear with finding edges
				//	TempBuffer = TempBuffer + vLocalSystemData->vITIPCDig->vLatestImage->dx;
				//}
				vLocalSystemData->vITIPCDig->SaveImageInHolderForDisplay(vLocalSystemData->vLastContainerImaged);

				if (vLocalSystemData->vLastContainerImaged)
				if (vLocalSystemData->vLastContainerImaged->vTheImage)
				if ((vLocalSystemData->vSystemRunMode != cCalibratingDetectorEdgesMode) &&
					(vLocalSystemData->vSystemRunMode != cMeasureSourceStrengthMode) &&
					(vLocalSystemData->vSystemRunMode != cAlignTubeMode) ||
					(vLocalSystemData->vSystemRunMode != cCalibratingDetectorGainMode) &&
					(vLocalSystemData->vSystemRunMode != cTestShutterMode) &&
					(vLocalSystemData->vSystemRunMode != cCalibratingDetectorOffsetMode))
				{
					double TempStartTime = CIFCOS::GetSystimeMicrosecs();
					//continuous feed  or bulk set size to whole image
					//TD when not sizing, set size to whole image
					if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))//Continuous Feed, or a Pipeline
					//if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) || 
					//	((vLocalConfigurationData->vReferenceContainersSideToSide == cDontReferenceSideToSide) && (!vGlobalInAutoSize)))//Continuous Feed, or a Pipeline
					{
						vLocalSystemData->vITIPCDig->vLastTop = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1;
						vLocalSystemData->vITIPCDig->vLastLeft = 0;
						vLocalSystemData->vITIPCDig->vLastRight = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1;
						vLocalSystemData->vITIPCDig->vLastBottom = 0;
						vLocalSystemData->vITIPCDig->vLastHeight = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1;
						vLocalSystemData->vITIPCDig->vLastWidth = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1;
					}
					else
					{
						bool TempFindContainerEdgesRelativeToImageEdges = ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || 
							(vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
							(vLocalConfigurationData->vScanTracType == cBriosoScanTrac) || (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac));

						vLocalSystemData->vITIPCDig->FindContainerDimensions(vLocalSystemData->vITIPCDig->vLatestImage->pix, vLocalSystemData->vITIPCDig->vIATScratchImage,
							&vLocalSystemData->vITIPCDig->vLastTop,&vLocalSystemData->vITIPCDig->vLastBottom, &vLocalSystemData->vITIPCDig->vLastRight,&vLocalSystemData->vITIPCDig->vLastLeft,
							&vLocalSystemData->vITIPCDig->vLastHeight,&vLocalSystemData->vITIPCDig->vLastWidth, TempFindContainerEdgesRelativeToImageEdges, vLocalConfigurationData->vShapeHandlingWidthTolerancePixels);

						//ReportErrorMessage("IAT1-Found Edges Left: " + dtoa(vLocalSystemData->vITIPCDig->vLastLeft, 2) + ", Right: " + dtoa(vLocalSystemData->vITIPCDig->vLastRight, 2), cWriteToLog, 0);
					}

					double TempEndTime = CIFCOS::GetSystimeMicrosecs();
					vLocalSystemData->vITIPCDig->vLastFindEdgesTime = TempEndTime - TempStartTime;

					//if have no reference information, set it from this image
					if ((!vGlobalCurrentProduct->vReferenceWidth) || (vGlobalCurrentProduct->vReferenceRight == vLocalSystemData->vITIPCDig->vOriginalBufferSizeY) ||
						(!vGlobalCurrentProduct->vReferenceLeft) || (!vGlobalCurrentProduct->vReferenceRight))
					{
						if ((vLocalSystemData->vITIPCDig->vProductAverageImage) && (vLocalSystemData->vITIPCDig->vProductAverageImage->vGreyImage))
						{
							WORD TempBottom = 0;
							WORD TempHeight = 0;
							BYTE TempOverlapped = 0;
							if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
							{
								vGlobalCurrentProduct->vReferenceTop = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1;
								vGlobalCurrentProduct->vReferenceLeft = 0;
								vGlobalCurrentProduct->vReferenceBottom = 0;
								vGlobalCurrentProduct->vReferenceRight = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1;
								TempHeight = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1;
								vGlobalCurrentProduct->vReferenceWidth = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1;
								if (vLocalSystemData->vLogFile.vLogSerialData)
									vLocalSystemData->vLogFile.WriteToLogFile("Set Reference B Right: " + dtoa(vGlobalCurrentProduct->vReferenceRight, 2) + ", Left: " + dtoa(vGlobalCurrentProduct->vReferenceLeft, 2) +
									", Width: " + dtoa(vGlobalCurrentProduct->vReferenceWidth, 2), cDebugMessage);
							}
							else
							{
								vLocalSystemData->vITIPCDig->FindContainerDimensions(vLocalSystemData->vITIPCDig->vProductAverageImage->vGreyImage, vLocalSystemData->vITIPCDig->vIATScratchImage, 
									&vGlobalCurrentProduct->vReferenceTop, &vGlobalCurrentProduct->vReferenceBottom, &vGlobalCurrentProduct->vReferenceRight, &vGlobalCurrentProduct->vReferenceLeft, &TempHeight,
									&vGlobalCurrentProduct->vReferenceWidth, 0, 10000); //vLocalConfigurationData->vShapeHandlingWidthTolerancePixels); //10000 = Finding new reference so ignore old one
								//ReportErrorMessage("IAT2-Found Edges Left: " + dtoa(vLocalSystemData->vITIPCDig->vLastLeft, 2) + ", Right: " + dtoa(vLocalSystemData->vITIPCDig->vLastRight, 2), cWriteToLog, 0);
							}
						}
						else
						{
							if (vLocalSystemData->vITIPCDig->vLastRight)
							if (vLocalSystemData->vITIPCDig->vLastRight < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY)
							if (!vLocalSystemData->vInAutoSetup)
							{
								vGlobalCurrentProduct->vReferenceRight = vLocalSystemData->vITIPCDig->vLastRight;
								vGlobalCurrentProduct->vReferenceLeft =vLocalSystemData->vITIPCDig->vLastLeft;
								vGlobalCurrentProduct->vReferenceTop = vLocalSystemData->vITIPCDig->vLastTop;
								vGlobalCurrentProduct->vReferenceBottom = vLocalSystemData->vITIPCDig->vLastBottom;
								vGlobalCurrentProduct->vReferenceWidth = vGlobalCurrentProduct->vReferenceRight - vGlobalCurrentProduct->vReferenceLeft;
								if (vLocalSystemData->vLogFile.vLogSerialData)
									vLocalSystemData->vLogFile.WriteToLogFile("Set Reference C Right: " + dtoa(vGlobalCurrentProduct->vReferenceRight, 2) + ", Left: " + dtoa(vGlobalCurrentProduct->vReferenceLeft, 2) +
										", Width: " + dtoa(vGlobalCurrentProduct->vReferenceWidth, 2), cDebugMessage);
							}
						}
					}
					else  //if purple line too low so can not find the edge of the container
					if ((vGlobalInAutoSize) && (vGlobalCurrentProduct->vEdgeLocationHeightPixel) &&
						(vLocalSystemData->vITIPCDig->vLastRight >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 3) &&
						(vLocalSystemData->vITIPCDig->vLastLeft <= 3) * (vGlobalCurrentProduct->vEdgeLocationBottomPixel < 32))
					{
						vGlobalCurrentProduct->SetEdgeLocationBottom((float)((vGlobalCurrentProduct->vEdgeLocationBottomPixel + 15) / vGlobalPixelsPerUnitInHeight));
					}

					//check for conveyor stops and image size
					if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem) || (vLoadedThisImageFromFile))
					if (vGlobalCurrentProduct->vReferenceWidth)
					if (vLocalSystemData->vLastContainerImaged)
					if ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode)) //Not Continuous Feed, and Not a Pipeline
					if (!vLocalSystemData->vSimulatingCamLinkImage)
					{
						bool TempWidthOK = true;
						bool TempHeightOK = true;

						//if this container is marked as a conveyor stop
						if ((vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cPassContainerOnConveyorStop) || (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cEjectContainerOnConveyorStop))
						{
							{
								if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cPassContainerOnConveyorStop)
								{
									vLocalSystemData->vLastContainerImaged->vTheImage->vPassContainer = true;
									vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason = cPassContainerOnConveyorStop;
									vLocalSystemData->vXRayCriteriaCount[vLocalSystemData->vCountToUseForEjectReason[cPassContainerOnConveyorStop]]++;//ConveyorStop
									vLocalSystemData->vXRayCriteriaCountTotal[vLocalSystemData->vCountToUseForEjectReason[cPassContainerOnConveyorStop]]++;//ConveyorStop
									vSizeOK = false;
									ReportErrorMessage("Pass Container From Conveyor Stop, c: " + dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0), cWrongSizeNotice,0);
								}

								if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cEjectContainerOnConveyorStop)
								{
									if (vGlobaluCSerialPort)
										vGlobaluCSerialPort->EjectContainer(cEjectContainerOnConveyorStop, vLocalSystemData->vLastContainerImaged,NULL);
									ReportErrorMessage("Eject Container From Conveyor Stop, c: " + dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0), cWrongSizeNotice,0);

									vLocalSystemData->vLastContainerImaged->vToBeEjected = true;
									vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason = cEjectContainerOnConveyorStop;
									vSizeOK = false;
								}
							}
						}

						//check if slipped off image
						if (vSizeOK)
						if (vLocalConfigurationData->vShapeHandlingProcessIfAtSides)
						{
							// initially clear, no slip, if there is a slip, set below
							vGlobalProductSlipArray[vGlobalProductSlipArrayLocation] = cProductDidNotSlipHere;
							if ((vLocalSystemData->vITIPCDig->vLastLeft <= 1) || (vLocalSystemData->vITIPCDig->vLastRight >= vLocalSystemData->vITIPCDig->vAquisitionSizeY - 1))
							{
								vSizeOK = false;
								if (!vGlobalProcessQuickLearnImagesThread)
								{
									if (vLocalConfigurationData->vSlippedIndicatorTime)
										::PostThreadMessage(vGlobalCallingThreadID,	cShowSlippedMessage, 0,0);
								}
								//if should eject slipps
								if ((vLocalConfigurationData->vShapeHandlingProcessIfAtSides == 1) && (vLocalConfigurationData->vShapeHandlingEjectors) && (!vLocalSystemData->vLastContainerImaged->vToBeEjected))
								{  //eject container on Edge
									if (vGlobaluCSerialPort)
										vGlobaluCSerialPort->EjectContainer(cEjectSlippedContainer, vLocalSystemData->vLastContainerImaged,NULL);
									ReportErrorMessage("Eject Slipped Container, c: " + dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0),cWrongSizeNotice,0);
								}
								else
								//if (vLocalConfigurationData->vShapeHandlingProcessIfAtSides == 2)
								{  //pass slips
									double TempContainterBrightness = 0;

									if (!vLocalConfigurationData->vSlipsEjectEmptyContainers)  //inverted control, is zero to eject
									{
										TempContainterBrightness = vLocalSystemData->vITIPCDig->CalculateDensityInImage(
											vLocalSystemData->vITIPCDig->vLatestImage->pix, vGlobalCurrentProduct->vReferenceLeft, vGlobalCurrentProduct->vReferenceRight,vGlobalCurrentProduct->vReferenceTop);
									}
									//ReportErrorMessage("Slip Brightness: " + dtoa(TempContainterBrightness,2),cError,0);

									//if container is white, assume empty and try to eject
									if ((!vLocalConfigurationData->vSlipsEjectEmptyContainers) && (TempContainterBrightness > 230) &&
										(vLocalConfigurationData->vShapeHandlingEjectors) && (!vLocalSystemData->vLastContainerImaged->vToBeEjected))
									{
										vLocalSystemData->vXRayCriteriaCount[vLocalSystemData->vCountToUseForEjectReason[cEjectEmptySlippedContainer]]++;//slipped
										vLocalSystemData->vXRayCriteriaCountTotal[vLocalSystemData->vCountToUseForEjectReason[cEjectEmptySlippedContainer]]++;//slipped
										vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason = cEjectEmptySlippedContainer;//slipped
										if (vGlobaluCSerialPort)
											vGlobaluCSerialPort->EjectContainer(cEjectEmptySlippedContainer, vLocalSystemData->vLastContainerImaged,NULL);
										ReportErrorMessage("Eject Empty (Slipped) Container, c: " + dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0),cWrongSizeNotice,0);
									}
									else
									{
										vLocalSystemData->vLastContainerImaged->vTheImage->vPassContainer = true;
										vLocalSystemData->vXRayCriteriaCount[vLocalSystemData->vCountToUseForEjectReason[cPassSlippedContainer]]++;//slipped
										vLocalSystemData->vXRayCriteriaCountTotal[vLocalSystemData->vCountToUseForEjectReason[cPassSlippedContainer]]++;//slipped
										vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason = cPassSlippedContainer;//slipped
										ReportErrorMessage("Container Slipped, c: " + dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0),cWrongSizeNotice,0);

										if (vLocalConfigurationData->vProductSlipPercentValue)
										if ((vLocalConfigurationData->vShapeHandlingProcessIfAtSides == 2) && (vGlobalProductSlipErrorSent == 0)) //2013 06 18
										if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
										{
											// kjh count the number of slips and flag, if warning for slip count is set
											vGlobalProductSlipArray[vGlobalProductSlipArrayLocation] = cProductSlippedHere ;
											vGlobalProductSlipCount = 0;
											for (int i=0; i < cSlipArraySize; i++)
											{
											if (vGlobalProductSlipArray[i] == cProductSlippedHere)
												vGlobalProductSlipCount++;
											}
											if (vGlobalProductSlipCount > ((vLocalConfigurationData->vProductSlipPercentValue) * 10))
											{	// send error message
												ReportErrorMessage("Containers slipping. Slipped containers are not inspected!", cCritical, 0); 
												ReportErrorMessage("Containers slipping. Slipped containers are not inspected!", cEMailMaintenance, 0); 
												vGlobalProductSlipErrorSent = 1;
											}									
										}
									}
								}
							}
							if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
							{
								vGlobalProductSlipArrayLocation++;
								if (vGlobalProductSlipArrayLocation >= cSlipArraySize)
									vGlobalProductSlipArrayLocation = 0;
							}
						}
						//check if width OK
						if (vSizeOK)
						if ((vLocalConfigurationData->vShapeHandlingProcessIfWrongWidth) ||
							//if learning find out if width OK if not a TD
							(((vLocalSystemData->vITIPCDig->vStructureEnabled) || (vLocalSystemData->vITIPCDig->vAverageEnabled) || (vLocalSystemData->vTakingQuickLearnImages)) && 
							(vLocalConfigurationData->vScanTracType != cBriosoScanTrac) && (vLocalConfigurationData->vScanTracType != cAtempoScanTrac) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && 
							(vLocalConfigurationData->vScanTracType != cAllegroScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac)))
						if (vLocalConfigurationData->vShapeHandlingWidthTolerancePixels)
						if (vGlobalCurrentProduct->vReferenceWidth)
						if ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode)) //Not Continuous Feed, and Not a Pipeline
						{
							int TempWidthDelta = vLocalSystemData->vITIPCDig->vLastWidth - vGlobalCurrentProduct->vReferenceWidth;
							if (TempWidthDelta < 0)
								TempWidthDelta = -TempWidthDelta;
							if (TempWidthDelta > vLocalConfigurationData->vShapeHandlingWidthTolerancePixels)
							{
								vSizeOK = false;
								double TempContainterBrightness = 0;
								ReportErrorMessage("Container Wrong Width, c: " + dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0),cWriteToLog,0);
								if (vLocalSystemData->vLogFile.vLogSerialData)
								{
									vLocalSystemData->vLogFile.WriteToLogFile("Container Wrong Width, c: " + dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0), cDebugMessage);
									vLocalSystemData->vLogFile.WriteToLogFile("Width delta: " + dtoa(TempWidthDelta,2) + ", Tolerance: " + dtoa(vLocalConfigurationData->vShapeHandlingWidthTolerancePixels), cDebugMessage);
									vLocalSystemData->vLogFile.WriteToLogFile("Cont Width: " + dtoa(vLocalSystemData->vITIPCDig->vLastWidth,2) + ", Ref Width: " + dtoa(vGlobalCurrentProduct->vReferenceWidth,2), cDebugMessage);
									vLocalSystemData->vLogFile.WriteToLogFile("Ref Right: " + dtoa(vGlobalCurrentProduct->vReferenceRight,2) + ", Ref Left: " + dtoa(vGlobalCurrentProduct->vReferenceLeft,2), cDebugMessage);
									vLocalSystemData->vLogFile.WriteToLogFile("Cont Right: " + dtoa(vLocalSystemData->vITIPCDig->vLastRight,2) + ", Cont Left: " + dtoa(vLocalSystemData->vITIPCDig->vLastLeft,2), cDebugMessage);
								}

								if (!vLocalConfigurationData->vSlipsEjectEmptyContainers)  //inverted control, is zero to eject
								{
									TempContainterBrightness = vLocalSystemData->vITIPCDig->CalculateDensityInImage(vLocalSystemData->vITIPCDig->vLatestImage->pix,
										vGlobalCurrentProduct->vReferenceLeft, vGlobalCurrentProduct->vReferenceRight,vGlobalCurrentProduct->vReferenceTop);
								}

								//ReportErrorMessage("Wrong Width Brightness: " + dtoa(TempContainterBrightness,2),cError,0);

								//if container is white, assume empty and try to eject
								if ((!vLocalConfigurationData->vSlipsEjectEmptyContainers) && (TempContainterBrightness > 230) &&
									(vLocalConfigurationData->vShapeHandlingEjectors) && (!vLocalSystemData->vLastContainerImaged->vToBeEjected))
								{
									if (vGlobaluCSerialPort)
										vGlobaluCSerialPort->EjectContainer(cEjectEmptySlippedContainer, vLocalSystemData->vLastContainerImaged,NULL);
									ReportErrorMessage("Eject Empty (Slipped) Container, c: " + dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0),cWrongSizeNotice,0);
								}
								else
								if ((vLocalConfigurationData->vShapeHandlingProcessIfWrongWidth == 1) &&
									(vLocalConfigurationData->vShapeHandlingEjectors) && (!vLocalSystemData->vLastContainerImaged->vToBeEjected))
								{  //eject wrong width container
									if (vGlobaluCSerialPort)
										vGlobaluCSerialPort->EjectContainer(cEjectContainerWrongWidth, vLocalSystemData->vLastContainerImaged,NULL);
									ReportErrorMessage("Eject Container Wrong Width, c: " + 
										dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0),cWrongSizeNotice,0);
								}
								else
								if (vLocalConfigurationData->vShapeHandlingProcessIfWrongWidth == 2)//pass wrong width container
								{//wrong width
									vLocalSystemData->vLastContainerImaged->vTheImage->vPassContainer = true;
									vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason = cPassContainerWrongWidth;//mark if learning
									vLocalSystemData->vXRayCriteriaCount[vLocalSystemData->vCountToUseForEjectReason[cPassContainerWrongWidth]]++;
									vLocalSystemData->vXRayCriteriaCountTotal[ vLocalSystemData->vCountToUseForEjectReason[cPassContainerWrongWidth]]++;
									ReportErrorMessage("Pass Container Wrong Width, c: " + dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0),cWrongSizeNotice,0);
								}
							}
							else
							if (vLocalSystemData->vLogFile.vLogSerialData)
							{
								vLocalSystemData->vLogFile.WriteToLogFile("Container Width OK, c: " + dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0),cDebugMessage);
								vLocalSystemData->vLogFile.WriteToLogFile("Width delta: " + dtoa(TempWidthDelta,2) + ", Tolerance: " + dtoa(vLocalConfigurationData->vShapeHandlingWidthTolerancePixels),cDebugMessage);
								vLocalSystemData->vLogFile.WriteToLogFile("Cont Width: " + dtoa(vLocalSystemData->vITIPCDig->vLastWidth,2) + ", Ref Width: " + dtoa(vGlobalCurrentProduct->vReferenceWidth,2),cDebugMessage);
								vLocalSystemData->vLogFile.WriteToLogFile("Ref Right: " + dtoa(vGlobalCurrentProduct->vReferenceRight,2) + ", Ref Left: " + dtoa(vGlobalCurrentProduct->vReferenceLeft,2),cDebugMessage);
								vLocalSystemData->vLogFile.WriteToLogFile("Cont Right: " + dtoa(vLocalSystemData->vITIPCDig->vLastRight,2) + ", Cont Left: " + dtoa(vLocalSystemData->vITIPCDig->vLastLeft,2),cDebugMessage);
							}
						}

						//check if wrong height
						if (vSizeOK)
						if (vLocalConfigurationData->vShapeHandlingProcessIfWrongHeight)
						if (vLocalConfigurationData->vShapeHandlingHeightTolerancePixels)
						{
							int TempHeightDelta = (int)vLocalSystemData->vITIPCDig->vLastHeight - (int)(vGlobalCurrentProduct->vReferenceTop - vGlobalCurrentProduct->vReferenceBottom);
							if (TempHeightDelta < 0)
								TempHeightDelta = -TempHeightDelta;
							if (TempHeightDelta > vLocalConfigurationData->vShapeHandlingHeightTolerancePixels)
							{
								vSizeOK = false;
								if ((vLocalConfigurationData->vShapeHandlingProcessIfWrongHeight == 1) && (vLocalConfigurationData->vShapeHandlingEjectors) && (!vLocalSystemData->vLastContainerImaged->vToBeEjected))
								{  //eject container on Edge
									if (vGlobaluCSerialPort)
										vGlobaluCSerialPort->EjectContainer(cEjectContainerWrongHeight, vLocalSystemData->vLastContainerImaged,NULL);
									ReportErrorMessage("Eject Container Wrong Height, c: " + dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0),cWrongSizeNotice,0);
								}
								else
								//if (vLocalConfigurationData->vShapeHandlingProcessIfWrongHeight == 2)
								{  //accept container on Edge
									vLocalSystemData->vLastContainerImaged->vTheImage->vPassContainer = true;
									vLocalSystemData->vXRayCriteriaCount[vLocalSystemData->vCountToUseForEjectReason[cPassContainerWrongHeight]]++;//wrong width
									vLocalSystemData->vXRayCriteriaCountTotal[vLocalSystemData->vCountToUseForEjectReason[cPassContainerWrongHeight]]++;//wrong width
									vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason = cPassContainerWrongHeight;//wrong width
									ReportErrorMessage("Pass Container Wrong Height, c: " + dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0),cWrongSizeNotice,0);
								}
							}
						}
						//if container is wider than it should be, and diverting containers not inspected
						//then turn off the diverter as there are back to back containers
						if (vLocalConfigurationData->vEjectContainersNotInspected)
						if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason > 0)
						if ((vLocalConfigurationData->vEjector[vLocalConfigurationData->vDiverterEjector - 1].vEnabled) &&
							(vLocalConfigurationData->vEjector[vLocalConfigurationData->vDiverterEjector - 1].vDigitalInputLine))
						if (vLocalSystemData->vDiverterOn)
						{ //turn diverter off if got a wide container as cannot inspect back to back boxes
							if (!vGlobalProcessQuickLearnImagesThread)
								::PostThreadMessage(vGlobalCallingThreadID,	cTurnDiverterOffMessage, 0,0);
							vLocalSystemData->vDiverterOn = false;

							if (vLocalSystemData->vLogFile.vLogSerialData)
								vLocalSystemData->vLogFile.WriteToLogFile("Turn Diverter Off",cDebugMessage);
						/*
							if (vLocalSystemData->vLogFile.vLogSerialData)
							if (vGlobaluCSerialPort)
								vGlobaluCSerialPort->WriteToLogFileWithBeltPosition("Turn Diverter Off",
								vLocalSystemData->vCurrentBeltPosition);
								*/
						}
						/*
						//check if container touching top
						if (vSizeOK)
						if (vLocalConfigurationData->vShapeHandlingProcessIfAtTop)
						{
							if (vLocalSystemData->vITIPCDig->vLastTop == 
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeX)
							{
								vSizeOK = false;
								if ((vLocalConfigurationData->vShapeHandlingProcessIfAtTop == 1) &&
									(vLocalConfigurationData->vShapeHandlingEjectors) &&
									(!vLocalSystemData->vLastContainerImaged->vToBeEjected))
								{  //eject container on Edge
									if (vGlobaluCSerialPort)
										vGlobaluCSerialPort->EjectContainer(cEjectContainerOnTop,
											vLocalSystemData->vLastContainerImaged,NULL);
									ReportErrorMessage("Eject Container On Top, c: " + 
										dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0),cWrongSizeNotice,0);
								}
								else
								//if (vLocalConfigurationData->vShapeHandlingProcessIfAtTop == 2)
								{  //accept container on Edge
									vLocalSystemData->vXRayCriteriaCount[
										vLocalSystemData->vCountToUseForEjectReason[cPassContainerOnTop]]++;//wrong width
									vLocalSystemData->vXRayCriteriaCountTotal[
										vLocalSystemData->vCountToUseForEjectReason[cPassContainerOnTop]]++;//wrong width
									vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason = cPassContainerOnTop;//wrong width
									ReportErrorMessage("Pass Container On Top, c: " + 
										dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0),cWrongSizeNotice,0);
								}
							}
						}
						*/

					}//end of checking for size

					vSavedFillerMonitorWeight = false;

					//filler monitor, save weight if seperate inspection
					if (vGlobalCurrentProduct->vFillerMonitorInspection) //if using the separate filler monitor inspection not being used to eject product, then run it
					{
						//double TempFillLevel = vLocalSystemData->vITIPCDig->CalculateUnderfillByWeight(vLocalSystemData->vITIPCDig->vLatestImage->pix, vGlobalCurrentProduct->vFillerMonitorInspection, 0);
						if (vGlobalCurrentProduct->vFillerMonitorInspection->vCollectingDensityAverage)
						if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
							AccumulateAverageDensity(vGlobalCurrentProduct->vFillerMonitorInspection, cMaximumNumberOfInspections);

						CheckDensity(vLocalSystemData->vLastContainerImaged, vGlobalCurrentProduct->vFillerMonitorInspection, cMaximumNumberOfInspections);
						SaveDataInValveMonitor(vLocalSystemData->vLastContainerImaged, vLocalSystemData->vITIPCDig->vLastDensityValue[cMaximumNumberOfInspections]);
					}

					if (vSizeOK)
						vNumberOfWrongSizeInARow = 0;
					else
					if (vNumberOfWrongSizeInARow < 250)
					{
						vNumberOfWrongSizeInARow++;
						if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
						if (vNumberOfWrongSizeInARow == 20)
							ReportErrorMessage("Warning, 20 wrong size in a row, check for slipping", cAction,32000);
					}

					if (!SampleThisContainer(vLocalSystemData->vLastContainerImaged)) //sample container if should
					if ((vLocalSystemData->vTestEjectNextContainer) && (!vLocalSystemData->vTestEjectWaitCount))
						TestEjectThisContainer();
					else
					//decide if should eject container based on the image
					{
						if (vLocalSystemData->vTestEjectWaitCount)
							vLocalSystemData->vTestEjectWaitCount--;

						//accumulate reference average if enabled during autosetup image Learn
						if (vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceEnabled) 
						if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cPassSlippedContainer)//slipped
							vLocalSystemData->vWrongWidthsDuringLearn++;
						else
						if ((vSizeOK) || (vNumberOfWrongSizeInARow > 50))
						{
							AccumulateBodyTriggerReference();
							ReportErrorMessage("Accumulate Learn Reference Container Size OK: " + dtoa(vLocalSystemData->vFrameCount,0), cAction,0);
						}

						//set reference values first time through if dont have any when running
						if (vGlobalCurrentProduct->vReferenceRight == 0)
						if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
						//if (vLocalConfigurationData->vReferenceImagePosition)
						{
							if (!vGlobalCurrentProduct->vReferenceTop)
							{
								vGlobalCurrentProduct->vReferenceTop = vLocalSystemData->vITIPCDig->vLastTop;
								vGlobalCurrentProduct->vReferenceBottom = vLocalSystemData->vITIPCDig->vLastBottom;
							}
							if (!vGlobalCurrentProduct->vReferenceRight)
							{
								//if ((vLocalConfigurationData->vReferenceImagePosition & 1) == 1)
								{
									vGlobalCurrentProduct->vReferenceLeft = vLocalSystemData->vITIPCDig->vLastLeft;
									vGlobalCurrentProduct->vReferenceRight = vLocalSystemData->vITIPCDig->vLastRight;
									vGlobalCurrentProduct->vReferenceWidth = vGlobalCurrentProduct->vReferenceRight - vGlobalCurrentProduct->vReferenceLeft;
									if (vLocalSystemData->vLogFile.vLogSerialData)
										vLocalSystemData->vLogFile.WriteToLogFile("Set Reference D Right: " + dtoa(vGlobalCurrentProduct->vReferenceRight, 2) + ", Left: " + dtoa(vGlobalCurrentProduct->vReferenceLeft, 2) +
											", Width: " + dtoa(vGlobalCurrentProduct->vReferenceWidth, 2), cDebugMessage);
									//if (vGlobalCurrentProduct->vProductLockOutWidth > 
									//	vGlobalCurrentProduct->vReferenceWidth / vGlobalPixelsPerUnit)
									//	vGlobalCurrentProduct->SetProductLockOutWidth( 
									//		(float)(vGlobalCurrentProduct->vReferenceWidth / 
									//		(vGlobalPixelsPerUnit * 
									//		vGlobalCurrentProduct->vOverScanMultiplier)));
								}
							}
						}
						else
						{
							//system not running, may be timing a shuttle worth ejector, and test ejected previous container, so need to extend shuttleworth ejector
						}

						if (((!vLocalSystemData->vLastContainerImaged->vToBeEjected) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem)) || 
							(!vLocalSystemData->vITIPCDig->vShowOnlyRealImages))
						if ((vSizeOK) || (vNumberOfWrongSizeInARow > 240))
						{
							int TempReferenceAdjustAmountTop = 0;
							int TempReferenceSideAdjustAmount = 0;

							double TempStartReferencePositionTime = CIFCOS::GetSystimeMicrosecs();

							//find amount need to move image to reference container
							if ((vLocalConfigurationData->vReferenceContainersSideToSide == cReferenceSideToSide) && (!vGlobalCurrentProduct->vBulkProductMode))
							{
								//TempReferenceAdjustAmountTop = CalculateTopReference();
								//TempReferenceAdjustAmountTop = CalculateBottomReference();

								//brh to do add TD Referencing up/down
								TempReferenceSideAdjustAmount = vLocalSystemData->vITIPCDig->CalculateSideToSideReference(vGlobalCurrentProduct->vReferenceRight,
										vGlobalCurrentProduct->vReferenceLeft, &vLocalSystemData->vITIPCDig->vLastRight, &vLocalSystemData->vITIPCDig->vLastLeft);
							}

							vLocalSystemData->vITIPCDig->vLastRightReferenceAdjust = (int)(vLocalSystemData->vITIPCDig->vLastRight - vGlobalCurrentProduct->vReferenceRight);
							vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust = TempReferenceSideAdjustAmount;

							double TempEndTime = CIFCOS::GetSystimeMicrosecs();
							vLocalSystemData->vITIPCDig->vLastReferencePositionTime = TempEndTime - TempStartReferencePositionTime;

							if (vLocalSystemData->vLogFile.vLogSerialData)
							{
								vLocalSystemData->vLogFile.WriteToLogFile("Side Reference: " + dtoa(TempReferenceSideAdjustAmount,0) + " R: " + 
									dtoa(vLocalSystemData->vITIPCDig->vLastRight,0) + " L: " + dtoa(vLocalSystemData->vITIPCDig->vLastLeft,0),cDebugMessage);
							}

							vCurrentImageBrightnessOK = true;
							//calculate image brightness
							//if (!vLocalSystemData->vInLowExposureMode)
							vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity = vLocalSystemData->vITIPCDig->CalculateInspectionDensityAfterReference(
								vLocalSystemData->vITIPCDig->vLatestImage->pix, vGlobalCurrentProduct->vDriftDensityInspection, vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust, 0,  //no top referencing
								vLocalConfigurationData->vReferenceROIsToEdges, vLocalSystemData->vITIPCDig->vLastLeft, vLocalSystemData->vITIPCDig->vLastRight); 

							vLocalSystemData->vLastContainerImaged->vImageBrightness = (BYTE)vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity;
							vLocalSystemData->vITIPCDig->vNextImageToDisplay->vFinalImageIntensity = (BYTE)vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity;



							if (vLocalSystemData->vLastContainerImaged)
							if (((vLocalConfigurationData->vScanTracType == cFermataScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac) || 
								(vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) &&  //if bulk and running, don't show all white images
								((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem)) &&
								(vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity > cImageBrightnessThreshold))
							{//this image will be hidden as pipe or conveyor empty, so notify operator
								if (vLocalSystemData->vPipeIsEmpty < 6)
									vLocalSystemData->vPipeIsEmpty++;
								//ReportErrorMessage("White Image ", cAction,0);
							}
							else
								if (vLocalSystemData->vPipeIsEmpty)
									vLocalSystemData->vPipeIsEmpty--;

							if (vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity > cImageBrightnessThreshold)
									vCurrentImageBrightnessOK = false;

							if (vLocalSystemData->vLogFile.vLogSerialData)
								vLocalSystemData->vLogFile.WriteToLogFile("Original Image Density: " + dtoa(vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity,2), cDebugMessage);

							bool TempCorrectThisImage = true;
							if (vLocalSystemData->vLastContainerImaged)
							if (((vLocalConfigurationData->vScanTracType == cFermataScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac)
							//	|| (vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)
								) &&  //if bulk and running, don't show all white images
								((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem)))
							{//use algorithm to find brightness that can ignore bubbles by finding darkest value of each pixel
								if (vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity < 0)
									vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity =  255;

								vLocalSystemData->vLastContainerImaged->vImageBrightness = (BYTE)(mvt_average(vLocalSystemData->vITIPCDig->vLatestImage));

								double TempPipeBrightness = vLocalSystemData->vITIPCDig->CalculatePipelineProductDensity(vLocalSystemData->vITIPCDig->vLatestImage->pix, vGlobalCurrentProduct->vDriftDensityInspection);
								double TempOriginal = vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity;
								if ((TempPipeBrightness > 25) && (TempPipeBrightness < 230))
								if (vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity > TempPipeBrightness)
								{
									if (vLocalSystemData->vPipeBrightnessCount < 200)
									{
										vLocalSystemData->vPipeBrightnessCount++;
										if (vLocalSystemData->vSmallestDifferenceBetweenAverageAndMinimum > vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity - TempPipeBrightness)
										{
											vLocalSystemData->vSmallestDifferenceBetweenAverageAndMinimum = vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity - TempPipeBrightness;
// for debugging and verifying next line only
//ReportErrorMessage("**" + dtoa(vLocalSystemData->vFrameCount, 0) + " Pipe Brightness Min to Ave Correction = " + dtoa(vLocalSystemData->vSmallestDifferenceBetweenAverageAndMinimum,3), cAction,0);
										}
									}

									if (vLocalSystemData->vSmallestDifferenceBetweenAverageAndMinimum < 40)
									if (TempPipeBrightness + vLocalSystemData->vSmallestDifferenceBetweenAverageAndMinimum < 255)
									{
										vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity = TempPipeBrightness + vLocalSystemData->vSmallestDifferenceBetweenAverageAndMinimum;
										vLocalSystemData->vLastContainerImaged->vImageBrightness = (BYTE)vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity;
										vLocalSystemData->vITIPCDig->vNextImageToDisplay->vFinalImageIntensity = (BYTE)vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity;
									}

// for debugging and verifying next line only
//ReportErrorMessage(dtoa(vLocalSystemData->vFrameCount, 0) + " Original Brightness: " + dtoa(TempOriginal,1) + ", Pipeline: " + dtoa(TempPipeBrightness,1) + ", Final: " + dtoa(vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity,1), cAction,0);
								}
								else
								{
									double TempImageAverage = vLocalSystemData->vLastContainerImaged->vImageBrightness;
									if (TempImageAverage < 254)
										vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity = vGlobalCurrentProduct->vSetupHistoryAverageDensity; //255; //cant calculate an amount, so don't make a correction
									vCurrentImageBrightnessOK = false;
//for debugging and verifying next line only
//ReportErrorMessage("**Pipe Brightness OFF: " + dtoa(TempPipeBrightness,3) + ", Image Ave: " + dtoa(TempImageAverage,3), cAction,0);
								}

								//if (vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity >= cImageBrightnessThreshold) //if not bright enough, check for dark pixels
								//{
								//	BYTE *TempPointer = vLocalSystemData->vITIPCDig->vLatestImage->pix;
								//	DWORD TempDarkPixelCount = 0;
								//	for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
								//	{
								//		if (*TempPointer++ < 240)
								//			TempDarkPixelCount++;
								//	}

								//	if (TempDarkPixelCount > (DWORD)(vLocalSystemData->vITIPCDig->vOriginalBufferSizeY)) //don't show white images
								//		vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity = cImageBrightnessThreshold - 1;  //make it so it shows the image
								//	vCurrentImageBrightnessOK = false;
								//}
								//else
								if (vCurrentImageBrightnessOK)
								{
									if ((vLocalSystemData->vITIPCDig->AreThereAnyAirBubbles(vLocalSystemData->vITIPCDig->vLatestImage->pix, vGlobalCurrentProduct->vDriftDensityInspection)) &&
										(vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity > vGlobalCurrentProduct->vSetupHistoryAverageDensity + 3))
									{
										vCurrentImageBrightnessOK = false;
										ReportErrorMessage(dtoa(vLocalSystemData->vFrameCount, 0) + "-Pipe has air bubbles", cWriteToLog, 0);
										if (vLocalSystemData->vAirBubblesInPipe < 6)
											vLocalSystemData->vAirBubblesInPipe++;
										vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity = vGlobalCurrentProduct->vSetupHistoryAverageDensity; //255; //cant calculate an amount, so don't make a correction
										vLocalSystemData->vITIPCDig->vNextImageToDisplay->vFinalImageIntensity = vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity;
										vLocalSystemData->vLastContainerImaged->vImageBrightness = (BYTE)vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity;
									}
									else
									if (vLocalSystemData->vAirBubblesInPipe)
										vLocalSystemData->vAirBubblesInPipe--;
								}

								if (vCurrentImageBrightnessOK)
								if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
								if (vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity - vGlobalCurrentProduct->vSetupHistoryAverageDensity > 10) //on a pipe, don't correct brightness to make darker if would make darker more than 5
								{
									TempCorrectThisImage = false;
		////for debugging and verifying next 2 lines only
		////ReportErrorMessage("**" + dtoa(vLocalSystemData->vFrameCount, 0) + "Pipe Brightness ignored, Reduce amount: " + 
		////	dtoa(vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity - vGlobalCurrentProduct->vSetupHistoryAverageDensity,1), cAction,0);
								}
							}

							//if ((vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity < 10) || (vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity > cImageBrightnessThreshold))
							//	vCurrentImageBrightnessOK = false;
							vLocalSystemData->vITIPCDig->vNextImageToDisplay->vFinalImageIntensity = vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity;

					//Adjust Image Brightness if you need
							if (vLocalConfigurationData->vAdjustImageIntensity)
							if ((vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem) || (vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLoadedThisImageFromFile))
							if (vGlobalCurrentProduct->vSetupHistoryAverageDensity)
							if (vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity)
							//if (!vLocalSystemData->vInLowExposureMode)
							if (vCurrentImageBrightnessOK)
							if ((vLocalSystemData->vKeepDensityFeedbackLoopLocked) || (!vLocalConfigurationData->vDriftCompensationEnabled))
							if (TempCorrectThisImage)
							{
								vLocalSystemData->vITIPCDig->vLastImageIntensityAdjust = 
									AdjustImageIntensity(vLocalSystemData->vITIPCDig->vLatestImage->pix, vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity, vGlobalCurrentProduct->vSetupHistoryAverageDensity);

//for debugging and verifying next 2 lines only
//if (vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity > vGlobalCurrentProduct->vSetupHistoryAverageDensity + 2)
//ReportErrorMessage("**" + dtoa(vLocalSystemData->vFrameCount, 0) + "Pipe Brightness Reduced: " + dtoa(vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity - vGlobalCurrentProduct->vSetupHistoryAverageDensity,3), cAction,0);

								if (vLocalSystemData->vLogFile.vLogSerialData)
								{
									vLocalSystemData->vLogFile.WriteToLogFile( "Adjusted Image Density: " + 
										dtoa(vLocalSystemData->vITIPCDig->CalculateInspectionDensityAfterReference( vLocalSystemData->vITIPCDig->vLatestImage->pix, vGlobalCurrentProduct->vDriftDensityInspection, vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust,
										0, vLocalConfigurationData->vReferenceROIsToEdges, vLocalSystemData->vITIPCDig->vLastLeft, vLocalSystemData->vITIPCDig->vLastRight) ,2) + ", Target: " + dtoa(vGlobalCurrentProduct->vSetupHistoryAverageDensity ,2),cDebugMessage);
								}

								if ((vLocalConfigurationData->vScanTracType == cFermataScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac)) 
								{
									vLocalSystemData->vITIPCDig->vNextImageToDisplay->vFinalImageIntensity = 
										vLocalSystemData->vITIPCDig->CalculatePipelineProductDensity(vLocalSystemData->vITIPCDig->vLatestImage->pix, vGlobalCurrentProduct->vDriftDensityInspection);
													
									if ((vLocalSystemData->vITIPCDig->vNextImageToDisplay->vFinalImageIntensity > 25) && (vLocalSystemData->vITIPCDig->vNextImageToDisplay->vFinalImageIntensity < 230))
									if (vLocalSystemData->vSmallestDifferenceBetweenAverageAndMinimum < 40)
									if (vLocalSystemData->vITIPCDig->vNextImageToDisplay->vFinalImageIntensity + vLocalSystemData->vSmallestDifferenceBetweenAverageAndMinimum < 255)
										vLocalSystemData->vITIPCDig->vNextImageToDisplay->vFinalImageIntensity = vLocalSystemData->vITIPCDig->vNextImageToDisplay->vFinalImageIntensity + vLocalSystemData->vSmallestDifferenceBetweenAverageAndMinimum;
								}
								else
									vLocalSystemData->vITIPCDig->vNextImageToDisplay->vFinalImageIntensity = vLocalSystemData->vITIPCDig->CalculateInspectionDensityAfterReference(
										vLocalSystemData->vITIPCDig->vLatestImage->pix, vGlobalCurrentProduct->vDriftDensityInspection, vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust, 0,  //no top referencing
										vLocalConfigurationData->vReferenceROIsToEdges, vLocalSystemData->vITIPCDig->vLastLeft, vLocalSystemData->vITIPCDig->vLastRight); 
							}
							if (vLocalSystemData->vITIPCDig->vNextImageToDisplay->vFinalImageIntensity == 0)
								vLocalSystemData->vITIPCDig->vNextImageToDisplay->vFinalImageIntensity = vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity;

							//vLocalSystemData->vLastContainerImaged->vImageBrightness = (BYTE)vLocalSystemData->vITIPCDig->CalculateInspectionDensityAfterReference(vLocalSystemData->vITIPCDig->vLatestImage->pix,
							//			vGlobalCurrentProduct->vDriftDensityInspection, vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust, 0, //vLocalSystemData->vITIPCDig->vLastTopReferenceAdjust,
							//			vLocalConfigurationData->vReferenceROIsToEdges, vLocalSystemData->vITIPCDig->vLastLeft, vLocalSystemData->vITIPCDig->vLastRight); 
							//vCurrentImageBrightnessOK = true;

							//if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode))
							//{
							//	if (vLocalSystemData->vLastContainerImaged->vImageBrightness >= cImageBrightnessThreshold)
							//	{
							//		vCurrentImageBrightnessOK = false;
							//		ReportErrorMessage("Pipe Empty", cWriteToLog, 0);
							//		BYTE *TempPointer = vLocalSystemData->vITIPCDig->vLatestImage->pix;
							//		DWORD TempDarkPixelCount = 0;
							//		for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
							//		{
							//			if (*TempPointer++ < 240)
							//				TempDarkPixelCount++;
							//		}

							//		if (TempDarkPixelCount > (DWORD)(2 * vLocalSystemData->vITIPCDig->vOriginalBufferSizeY)) //don't show white images
							//			vLocalSystemData->vLastContainerImaged->vImageBrightness = cImageBrightnessThreshold - 1;  //make it so it shows the image
							//	}
							//	else
							//	if (vLocalSystemData->vITIPCDig->AreThereAnyAirBubbles(vLocalSystemData->vITIPCDig->vLatestImage->pix, vGlobalCurrentProduct->vDriftDensityInspection))
							//	{
							//		vLocalSystemData->vLastContainerImaged->vImageBrightness = 254;
							//		vCurrentImageBrightnessOK = false;
							//		ReportErrorMessage("Pipe has air bubble", cWriteToLog, 0);
							//	}
							//}
								
							vLocalSystemData->vITIPCDig->SaveImageInHolderForDisplay(vLocalSystemData->vLastContainerImaged);

							//reference this image for the image processing routines
							if (vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust)
							{
								vLocalSystemData->vITIPCDig->MakeReferenceImageAdjustment(vLocalSystemData->vITIPCDig->vLatestImage->pix, vLocalSystemData->vITIPCDig->vReferenceAdjustBuffer, 0,// dont reference top TempReferenceAdjustAmountTop,
									vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust);
							}

							double TempStartFindTime1 = CIFCOS::GetSystimeMicrosecs();

							TempInspectionTopReferenceAdjust = 0;
							vLocalSystemData->vThisContainerShouldBeEjected = false;
							vLocalSystemData->vOldThisContainerShouldBeEjected = false;

							FillMemory(vLocalSystemData->vITIPCDig->vImageForPseudoColor->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize, 255);

							//Major Inspections 
							if (!((vLocalSystemData->vLastContainerImaged->vTheImage->vPassContainer) || (!vSizeOK) || (vGlobalInAutoExposure) || (vGlobalInAutoSize)))  //if good image, check inspections
							if (vLocalSystemData->vLastContainerImaged)
							//if (!vLocalSystemData->vLastContainerImaged->vTheImage->vPassContainer)
							//if (!vGlobalInAutoExposure)
							//if (!vGlobalInAutoSize)
							//if (vSizeOK)
							//if (vLocalSystemData->vLastContainerImaged)
							for (BYTE TempInspectionLoop = 0; TempInspectionLoop < vGlobalCurrentProduct->vNumberOfInspections; TempInspectionLoop++)
							//2/5/2016 Do all inspections all the time
							//don't continue if ejected and in normal run or should have ejected because ejectors disabled, if configured to only process till first eject inspection
							if ((!vLocalConfigurationData->vStopAfterAnInspectionRejects) ||
								((!vLocalSystemData->vLastContainerImaged->vToBeEjected) ||
								(vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vFindGoodIntensity) ||
								(vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vFindBadIntensity) ||
								(vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vInspectionType == cCheckWeighInspection) ||
								(vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vInspectionType == cUnderfillByWeightInspection) ||
								(vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vInspectionType == cOverfillByWeightInspection) ||
								(vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vCollectingDensityAverage) ||
								((vLocalSystemData->vValveData) && (vLocalSystemData->vValveMonitorInspection == TempInspectionLoop))))
							//if ((vGlobalLearnState < 3) ||//don't inspect top contaminant if Improve state 3 or higher and adding structure
							//	(!vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vAddStructure) ||
							//	(vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vROIPixelBottom < vGlobalCurrentProduct->vReferenceTop / 2) ||
							//	(!vLocalConfigurationData->vAutoImproveEnabled) ||
							//	(vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vFindGoodIntensity) ||
							//	(vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vFindBadIntensity))
							{//check this image for each inspection if have not been ejected already
								double TempStartInspectionTime = CIFCOS::GetSystimeMicrosecs();

								if (vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vReferenceToTop)
								if (TempInspectionTopReferenceAdjust == 0)
								{
									TempInspectionTopReferenceAdjust = CalculateTopReference();

									if (vLocalSystemData->vLogFile.vLogSerialData)
										vLocalSystemData->vLogFile.WriteToLogFile("Top Reference: " + dtoa(TempInspectionTopReferenceAdjust,0),cDebugMessage);
									vLocalSystemData->vITIPCDig->vLastTopReferenceAdjust = TempInspectionTopReferenceAdjust;
								}

								if (vLocalSystemData->vITIPCDig->vDoBottomReferencing)
								if (vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vReferenceToBottom)
								if (TempInspectionTopReferenceAdjust == 0)
								{
									TempInspectionTopReferenceAdjust = CalculateBottomReference();

									if (vLocalSystemData->vLogFile.vLogSerialData)
										vLocalSystemData->vLogFile.WriteToLogFile("Bottom Reference: " + dtoa(TempInspectionTopReferenceAdjust,0),cDebugMessage);
									vLocalSystemData->vITIPCDig->vLastTopReferenceAdjust = TempInspectionTopReferenceAdjust;
								}

								//copy the side to side referenced image so can process next inspection
								CopyMemory(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vLatestImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

								if ((vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vReferenceToBottom) || (vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vReferenceToTop))
								if (vLocalSystemData->vITIPCDig->vLastTopReferenceAdjust)
								{
									vLocalSystemData->vITIPCDig->MakeReferenceImageAdjustment(
										vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vReferenceAdjustBuffer,
										vLocalSystemData->vITIPCDig->vLastTopReferenceAdjust,
										0);// already referenced side amount, so don't do again vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust);
								}

								bool TempDoAllLoops = false;
								int TempFineReferenceAdjust = 0;
	//9/30/2009
								//vReferenceTries = 0; //(vLocalConfigurationData->vNumberOfJitterPositions * 2) + 1;//try n different reference postions
								switch (vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vInspectionType)
								{
									case cStandardDeviationInspection:
									{
										double TempStartFindTime = CIFCOS::GetSystimeMicrosecs();

										CheckStandardDeviation(vLocalSystemData->vLastContainerImaged, vGlobalCurrentProduct->vInspection[TempInspectionLoop], TempInspectionLoop);

										if (vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vCollectingDensityAverage)
										if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
											AccumulateAverageStandardDeviation(vGlobalCurrentProduct->vInspection[TempInspectionLoop], TempInspectionLoop);

										double TempEndTime = CIFCOS::GetSystimeMicrosecs();
										vLocalSystemData->vITIPCDig->vLastFindDensityTime = TempEndTime - TempStartFindTime;
									}
									break;
									case cFloodedInspection:
										CheckForFloodedIVBag(vLocalSystemData->vLastContainerImaged, vGlobalCurrentProduct->vInspection[TempInspectionLoop], TempInspectionLoop);
									break;
									case cDensityInspection:
									//case cCheckWeighInspection:
									case cMissingLidInspection:
									case cUnderfillInspection:
									case cOverfillInspection:
									{
										//if (vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vEnable)
										//if ((vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vDensityThresholdUpper) || 
										//	(vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vDensityThresholdLower))
										{
											double TempStartFindTime = CIFCOS::GetSystimeMicrosecs();
											if (vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vCollectingDensityAverage)
											if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
												AccumulateAverageDensity(vGlobalCurrentProduct->vInspection[TempInspectionLoop], TempInspectionLoop);

											CheckDensity(vLocalSystemData->vLastContainerImaged, vGlobalCurrentProduct->vInspection[TempInspectionLoop], TempInspectionLoop);

											double TempEndTime = CIFCOS::GetSystimeMicrosecs();
											vLocalSystemData->vITIPCDig->vLastFindDensityTime = TempEndTime - TempStartFindTime;
										}
									}
									break;
									case cCheckWeighInspection:
									case cUnderfillByWeightInspection:
									case cOverfillByWeightInspection:
										CheckUnderfillByWeight(vLocalSystemData->vLastContainerImaged, vGlobalCurrentProduct->vInspection[TempInspectionLoop], TempInspectionLoop);
									break;
									case cContaminantInspection:
									case cIPContaminantInspection:
									case cWireContaminantInspection:
									case cSpotInROIContaminantInspection:
									case cGlass13mmContaminantInspection:
									case cContaminantBonesInspection:
									case cContaminant22Inspection:
									case cThresholdContaminantInspection:
									case cBadHeartInspection:
									case cDripBySpoutInspection:
									case cDripInspection:
									case cTopContaminantInspection:
									case cBottomContaminantInspection:
									case cContaminant2Inspection:
									case cAdvancedContaminant2Inspection:
									//don't even do this inspection if using the learn and don't have a learned image
									if ((!vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vAddStructure) || (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage))
									if ((vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vThreshold) ||//if Threshold is not zero
										(vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vFindGoodIntensity) || (vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vFindBadIntensity))
									{ 
										if ((vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vFindGoodIntensity) || (vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vFindBadIntensity))
											vProcessMinimumGood = true;
										else
											vProcessMinimumGood = false;

										double TempStartFindTime = CIFCOS::GetSystimeMicrosecs();

										FindContaminants(vLocalSystemData->vLastContainerImaged, 0, 0, vGlobalCurrentProduct->vInspection[TempInspectionLoop], TempInspectionLoop);

										TempEndTime = CIFCOS::GetSystimeMicrosecs();
										vLocalSystemData->vITIPCDig->vLastFindContaminantsTime = TempEndTime - TempStartFindTime;
									} //end case contaminant inspection
									break;
									case cJarBottomContaminantInspection:
									{
										CheckForJarBottomContaminants(vLocalSystemData->vLastContainerImaged,
												vGlobalCurrentProduct->vInspection[TempInspectionLoop], 
												TempInspectionLoop);
									} //end case contaminant inspection
									break;
									case cItemCountInspection:
									case cSizeInspection:
									case cMissingLidInspectionOnSize:
									{ 
										FindSizeInspection(vLocalSystemData->vLastContainerImaged, vGlobalCurrentProduct->vInspection[TempInspectionLoop], TempInspectionLoop);

										if (vLocalSystemData->vThisContainerShouldBeEjected)
											InspectionFoundContaminant(TempInspectionLoop);
									} //end case size inspection
									break;

									case cKernelInspection:
									{
										FindKernelContaminants(vLocalSystemData->vLastContainerImaged, 0, 0, vGlobalCurrentProduct->vInspection[TempInspectionLoop], TempInspectionLoop);

										if (vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vInvertEjectLogic)
											vLocalSystemData->vThisContainerShouldBeEjected = !vLocalSystemData->vThisContainerShouldBeEjected;

										if (vLocalSystemData->vThisContainerShouldBeEjected)
											InspectionFoundContaminant(TempInspectionLoop);
									} //end case Kernel inspection
									break;

									case cVoidInspection:
									{
										double TempStartFindTime = CIFCOS::GetSystimeMicrosecs();

										FindVoids(vLocalSystemData->vLastContainerImaged, 0, 0, vGlobalCurrentProduct->vInspection[TempInspectionLoop], TempInspectionLoop);

										TempEndTime = CIFCOS::GetSystimeMicrosecs();
										vLocalSystemData->vITIPCDig->vLastFindVoidsTime = TempEndTime - TempStartFindTime;

										if (vGlobalCurrentProduct->vInspection[TempInspectionLoop]->vInvertEjectLogic)
											vLocalSystemData->vThisContainerShouldBeEjected = !vLocalSystemData->vThisContainerShouldBeEjected;

										if (vLocalSystemData->vThisContainerShouldBeEjected)
											InspectionFoundContaminant(TempInspectionLoop);
									} //end case void inspection
									break;
								} //end switch inspection type
								double TempEndTime = CIFCOS::GetSystimeMicrosecs();
								vLocalSystemData->vITIPCDig->vLastInspectonProcessingTime[TempInspectionLoop] = TempEndTime - TempStartInspectionTime;
							}//end of number of inspections loop for this image

							//add image to AutoImprove if was good, manual improve learn
							if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
							if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
							if (vSizeOK)
							if (vCurrentImageBrightnessOK)
								vLocalSystemData->vITIPCDig->AddImageToImproveLearn(vLocalSystemData->vITIPCDig->vLatestImage->pix,vLocalSystemData->vITIPCDig->vIATScratchImage->pix);

							//time for all major inspections
							double TempEndTime1 = CIFCOS::GetSystimeMicrosecs();
							vLocalSystemData->vITIPCDig->vLastFindTime = TempEndTime1 - TempStartFindTime1;
						}

						//1/27/2016 do all inspections
						BYTE TempNumberOfFailedInspections = 0;
						BYTE TempFailedImagesBitMap = 0;
						for (BYTE TempLoopI = 0; TempLoopI < vGlobalCurrentProduct->vNumberOfInspections; TempLoopI++)
						if (vLocalSystemData->vLastContainerImaged->vTheImage->vRejectedForThisInspection[TempLoopI])
							TempNumberOfFailedInspections++;

						//eject container if found contaminants in required number of inspections
						if (TempNumberOfFailedInspections >= vGlobalCurrentProduct->vNumberOfInspectionsRequiredToEject)
						{
							if (vGlobaluCSerialPort)
							if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == 255)
							{
								if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
									ReportErrorMessage("Programming Error, Reject Reason = 255 before eject", cEMailInspx, 32000);
							}
							else
							{
								if ((vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason >= cEjectInspectionOffset) && (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason <= vGlobalCurrentProduct->vNumberOfInspections))
									vGlobaluCSerialPort->EjectContainer(vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason, vLocalSystemData->vLastContainerImaged,
										vGlobalCurrentProduct->vInspection[vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason - cEjectInspectionOffset]);
								else
								if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason > cMaximumRejectReason)
								if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
									ReportErrorMessage("Programming Error, Reject Reason wrong: " + dtoa(vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason, 0), cEMailInspx, 32000);
							}
							vLocalSystemData->vLastContainerImaged->vToBeEjected = true;
						}
						else 
						{ //not ejecting, so clear reject reason
							if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason < cEjectSlippedContainer)
								vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason = 255;
							vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectInspection = 255;
							vLocalSystemData->vLastContainerImaged->vToBeEjected = false;
						}

						//if container is good, and a diverter is active, then de-activate at correct position
						if (!vLocalSystemData->vLastContainerImaged->vToBeEjected) // if container is good
							//if fpga version high enough
						{
							for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
							if (vLocalSystemData->vDiverterActive[TempLoop])
							{
								BYTE TempEjector = vLocalConfigurationData->vEjector[TempLoop].vDigitalInputLine; // note: software (TempLoop) is zero based but command is one based
								BYTE TempStopPulseEjector = TempLoop + 1; // use next ejector output to
								vLocalSystemData->vDiverterActive[TempLoop] = false;
								vLocalSystemData->vLastContainerImaged->vEjectPosition[TempStopPulseEjector] = BPAdd(vGlobalCurrentProduct->vEjectorDelayBeltPosition[TempStopPulseEjector], vLocalSystemData->vLastContainerImaged->vBodyTriggerPosition);
								ReportErrorMessage("De-activate S Diverter use timing from ejector: " + dtoa(TempStopPulseEjector, 0) + " value: " + dtoa(vGlobalCurrentProduct->vEjectorDelayBeltPosition[TempStopPulseEjector], 0),cAction,0);
				
								if (vGlobalCurrentProduct->vEjectorResponseTime[TempStopPulseEjector])
								{
									vLocalSystemData->vLastContainerImaged->vEjectPosition[TempStopPulseEjector] = BPSubtract(vLocalSystemData->vLastContainerImaged->vEjectPosition[TempStopPulseEjector], (WORD)(vLocalSystemData->vActualEncoderRate * 
										vGlobalCurrentProduct->vEjectorResponseTime[TempStopPulseEjector]) + vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust);
								}
								else
								{
									vLocalSystemData->vLastContainerImaged->vEjectPosition[TempStopPulseEjector] = BPSubtract(vLocalSystemData->vLastContainerImaged->vEjectPosition[TempStopPulseEjector], vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust);
								}
								vLocalSystemData->vDiverterDeactivatePosition[TempEjector] = vLocalSystemData->vLastContainerImaged->vEjectPosition[TempStopPulseEjector];
								if (vLocalSystemData->vDiverterDeactivatePosition[TempEjector] == 0)
									vLocalSystemData->vDiverterDeactivatePosition[TempEjector] = 0xFFFF;
								ReportErrorMessage("De-activate S Diverter: " + dtoa(TempEjector, 0) + " at BP: " + dtoa(vLocalSystemData->vLastContainerImaged->vEjectPosition[TempStopPulseEjector], 0),cAction,0);

								//use if diverter is level controled or on = divert TempEjector = TempEjector | 0x40; //turn Diverter Off as done ejecting containers in a row 

								//for pulse driven extend/retract Shuttleworth ejector
								vGlobaluCSerialPort->SendEjectCommand(TempEjector + 1, vLocalSystemData->vLastContainerImaged->vEjectPosition[TempStopPulseEjector]); // note: software is zero based but command is one based
							}
						}

						//Save image for quick learn if needed
						if (vLocalSystemData->vTakingQuickLearnImages)
						if (vLocalSystemData->vQuickLearnImageIndex < vLocalConfigurationData->vNumberOfQuickLearnImagesToTake)
						if ((!vLocalSystemData->vITIPCDig->vShowOnlyRealImages) || (vLocalSystemData->vITIPCDig->vHaveRealImage) || (vLocalSystemData->vITIPCDig->vProcessNextImage))
						if ((!vLocalSystemData->vLastContainerImaged->vToBeEjected) || (vLocalSystemData->vIncludeRejectsInAverageAndStructure))
						if ((vLocalSystemData->vITIPCDig->vDriftCompensationLocked) || (!vGlobalCurrentProduct->vSetupHistoryAverageDensity))
						if (vLocalSystemData->vLastContainerImaged)
						if (((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode)) || //if bulk mode, must be dark enough
							(vCurrentImageBrightnessOK))
						if (((vLocalSystemData->vITIPCDig->vLastImageIntensityTooFarOff) && (vLocalConfigurationData->vMaxImageIntensityAdjust)) && (!vGlobalCurrentProduct->vBulkProductMode))
							vLocalSystemData->vWrongBrightnessDuringLearn++;
						else
						if ((vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cPassSlippedContainer) && (!vGlobalCurrentProduct->vBulkProductMode))//slipped
							vLocalSystemData->vWrongWidthsDuringLearn++;
						else
						if (((!((vSizeOK) || (vNumberOfWrongSizeInARow > 50))) && (!vLocalSystemData->vInAutoSetup)) && (!vGlobalCurrentProduct->vBulkProductMode))
							vLocalSystemData->vWrongWidthsDuringLearn++;
						else
						{
							if (!vSizeOK)
								vLocalSystemData->vWrongWidthsDuringLearn++;
							if (vLocalSystemData->vQuickLearnImages[vLocalSystemData->vQuickLearnImageIndex])
							{
								free(vLocalSystemData->vQuickLearnImages[vLocalSystemData->vQuickLearnImageIndex]);
								vLocalSystemData->vQuickLearnImages[vLocalSystemData->vQuickLearnImageIndex] = NULL;
							}
							if (vLocalSystemData->vQuickLearnWorkingImages[vLocalSystemData->vQuickLearnImageIndex])
							{
								free(vLocalSystemData->vQuickLearnWorkingImages[vLocalSystemData->vQuickLearnImageIndex]);
								vLocalSystemData->vQuickLearnWorkingImages[vLocalSystemData->vQuickLearnImageIndex] = NULL;
							}

							if (ThereIsEnoughMemory(vLocalSystemData->vITIPCDig->vOriginalBufferSize,"Quick Learn Image"))
								vLocalSystemData->vQuickLearnImages[vLocalSystemData->vQuickLearnImageIndex] = (BYTE *)malloc(vLocalSystemData->vITIPCDig->vOriginalBufferSize);

							//copy image data to quick learn image buffer
							if ((vLocalSystemData->vITIPCDig->vLatestImage->pix) && (vLocalSystemData->vQuickLearnImages[vLocalSystemData->vQuickLearnImageIndex]))
							if (vLocalSystemData->vLastContainerImaged)
							if (vLocalSystemData->vLastContainerImaged->vTheImage)
							if (vLocalSystemData->vLastContainerImaged->vTheImage->vGreyImage)
							{
								CopyMemory(vLocalSystemData->vQuickLearnImages[vLocalSystemData->vQuickLearnImageIndex],
									vLocalSystemData->vLastContainerImaged->vTheImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

								vLocalSystemData->vQuickLearnImageIndex++;

								ReportErrorMessage("Save Image for Quick Learn: " + dtoa(vLocalSystemData->vQuickLearnImageIndex, 0), cWriteToLog,0);
							}
							else
							{
								vLocalSystemData->vTakingQuickLearnImages = false;
								ReportErrorMessage("Quick Learn Aborted, Out of Memory, Use Standard Learn", cEMailMaintenance,0);
								vLocalSystemData->FreeQuickLearnImages();
							}
						}

						if (vLocalSystemData->vITIPCDig->vAverageEnabled)
						if ((!vLocalSystemData->vLastContainerImaged->vToBeEjected) ||
							(vLocalSystemData->vIncludeRejectsInAverageAndStructure))
						if ((vLocalSystemData->vITIPCDig->vDriftCompensationLocked) || 
							(vLocalSystemData->vInAutoSetup) ||
							(!vGlobalCurrentProduct->vSetupHistoryAverageDensity))
						if ((vLocalSystemData->vITIPCDig->vLastImageIntensityTooFarOff) && (vLocalConfigurationData->vMaxImageIntensityAdjust))
							vLocalSystemData->vWrongBrightnessDuringLearn++;
						else
						if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cPassSlippedContainer)//slipped
							vLocalSystemData->vWrongWidthsDuringLearn++;
						else
						if (!((vSizeOK) || (vNumberOfWrongSizeInARow > 50)))
							vLocalSystemData->vWrongWidthsDuringLearn++;
						else
						if (vCurrentImageBrightnessOK)
						{
							if (!vSizeOK)
								vLocalSystemData->vWrongWidthsDuringLearn++;
							if (vLocalSystemData->vInAutoSetup)
								AccumulateAverageImage(vLocalSystemData->vITIPCDig->vLatestImage->pix);
							else
							{  //if normal re-learn, must reference image to new reference location
								if (vLocalSystemData->vLastContainerImaged)
								if (vLocalSystemData->vLastContainerImaged->vTheImage)
								if (vLocalSystemData->vLastContainerImaged->vTheImage->vGreyImage)
									CopyMemory(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vLastContainerImaged->vTheImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

								int TempLearnReferenceSideAdjustAmount = 0;

								if ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode)) //Not Continuous Feed, or a Pipeline
								if ((vLocalConfigurationData->vScanTracType != cAtempoScanTrac) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac) &&
									(vLocalConfigurationData->vScanTracType != cAllegroScanTrac) && (vLocalConfigurationData->vScanTracType != cBriosoScanTrac)) //Continuous Feed, or a Pipeline or TD
									TempLearnReferenceSideAdjustAmount = vLocalSystemData->vITIPCDig->CalculateSideToSideReference(vGlobalCurrentProduct->vReferenceRight, 
										vGlobalCurrentProduct->vReferenceLeft, &vLocalSystemData->vITIPCDig->vLastRight, &vLocalSystemData->vITIPCDig->vLastLeft);

								if (TempLearnReferenceSideAdjustAmount)
								{
									vLocalSystemData->vITIPCDig->MakeReferenceImageAdjustment(
										vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vReferenceAdjustBuffer,
										0,// dont reference top TempReferenceAdjustAmountTop,
										TempLearnReferenceSideAdjustAmount);
								}

								AccumulateAverageImage(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix);
							}
						}

						//accumulate structure image if enabled
						if (vLocalSystemData->vITIPCDig->vStructureEnabled) 
						if ((!vLocalSystemData->vLastContainerImaged->vToBeEjected) || (vLocalSystemData->vIncludeRejectsInAverageAndStructure))
						if ((vLocalSystemData->vITIPCDig->vDriftCompensationLocked) || (vLocalSystemData->vInAutoSetup) || (!vGlobalCurrentProduct->vSetupHistoryAverageDensity))
						if (((vLocalSystemData->vITIPCDig->vLastImageIntensityTooFarOff) && (vLocalConfigurationData->vMaxImageIntensityAdjust)) || (!vCurrentImageBrightnessOK))
							vLocalSystemData->vWrongBrightnessDuringLearn++;
						else
						if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cPassSlippedContainer)//slipped
							vLocalSystemData->vWrongWidthsDuringLearn++;
						else
						if (!((vSizeOK) || (vNumberOfWrongSizeInARow > 50)))
							vLocalSystemData->vWrongWidthsDuringLearn++;
						else
						if (vCurrentImageBrightnessOK)
						{
							if (!vSizeOK)
								vLocalSystemData->vWrongWidthsDuringLearn++;
							//don't find best reference for demo as will reference demo images differenetly
							//if relearning, use the new average to find the new structure, then use the
							//old average to decide if should reject
							if (!vLocalSystemData->vInAutoSetup)
							{
								if (vLocalSystemData->vLastContainerImaged)
								if (vLocalSystemData->vLastContainerImaged->vTheImage)
								if (vLocalSystemData->vLastContainerImaged->vTheImage->vGreyImage)
									CopyMemory(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vLastContainerImaged->vTheImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

								int TempReferenceSideAdjustAmount = 0;

								if ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode)) //Not Continuous Feed, or a Pipeline
								if ((vLocalConfigurationData->vScanTracType != cAtempoScanTrac) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac) &&
									(vLocalConfigurationData->vScanTracType != cAllegroScanTrac) && (vLocalConfigurationData->vScanTracType != cBriosoScanTrac)) //Continuous Feed, or a Pipeline or TD
									TempReferenceSideAdjustAmount = vLocalSystemData->vITIPCDig->CalculateSideToSideReference(
										vGlobalCurrentProduct->vReferenceRight, vGlobalCurrentProduct->vReferenceLeft, &vLocalSystemData->vITIPCDig->vLastRight, &vLocalSystemData->vITIPCDig->vLastLeft);

								if (TempReferenceSideAdjustAmount)
								{
									vLocalSystemData->vITIPCDig->MakeReferenceImageAdjustment(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vReferenceAdjustBuffer, 0,// dont reference top TempReferenceAdjustAmountTop,
										TempReferenceSideAdjustAmount);
								}

								double TempImageDensity = vLocalSystemData->vITIPCDig->CalculateInspectionDensity(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vGlobalCurrentProduct->vDriftDensityInspection); 

								AdjustImageIntensity(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, TempImageDensity, vLocalSystemData->vITIPCDig->vLearnedAverageDensity);

								AccumulateContaminant2Structure(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix);

								if (vLocalSystemData->vLastContainerImaged)
								if (vLocalSystemData->vLastContainerImaged->vTheImage)
								if (vLocalSystemData->vLastContainerImaged->vTheImage->vGreyImage)
									CopyMemory(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vLastContainerImaged->vTheImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

								if (TempReferenceSideAdjustAmount)
								{
									vLocalSystemData->vITIPCDig->MakeReferenceImageAdjustment(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vReferenceAdjustBuffer, 0,// dont reference top TempReferenceAdjustAmountTop,
										TempReferenceSideAdjustAmount);
								}

								AdjustImageIntensity(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, TempImageDensity, vLocalSystemData->vITIPCDig->vLearnedAverageDensity);

								//subtract the product's average image from this one
								if (vLocalSystemData->vITIPCDig->vLearnedProductAverageImage)
									vLocalSystemData->vITIPCDig->SubtractProductAverageImage(
									vLocalSystemData->vITIPCDig->vLatestImageCopy->pix,
									vLocalSystemData->vITIPCDig->vLearnedProductAverageImage);
								else
								if (vLocalSystemData->vITIPCDig->vProductAverageImage)
									vLocalSystemData->vITIPCDig->SubtractProductAverageImage(
									vLocalSystemData->vITIPCDig->vLatestImageCopy->pix,
									vLocalSystemData->vITIPCDig->vProductAverageImage);

								AccumulateStructure(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix);

								if (vLocalSystemData->vLastContainerImaged)
								if (vLocalSystemData->vLastContainerImaged->vTheImage)
								if (vLocalSystemData->vLastContainerImaged->vTheImage->vGreyImage)
									CopyMemory(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vLastContainerImaged->vTheImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

								if (TempReferenceSideAdjustAmount)
								{
									vLocalSystemData->vITIPCDig->MakeReferenceImageAdjustment(
										vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vReferenceAdjustBuffer,
										0,// dont reference top TempReferenceAdjustAmountTop,
										TempReferenceSideAdjustAmount);
								}

								AdjustImageIntensity(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, TempImageDensity, vLocalSystemData->vITIPCDig->vLearnedAverageDensity);

								if (vLocalSystemData->vITIPCDig->vProductAverageImage)
									vLocalSystemData->vITIPCDig->SubtractVoidProductAverageImage(
									vLocalSystemData->vITIPCDig->vLatestImageCopy->pix);
								//subtract the product's average image from this one if enabled
								//if (vLocalSystemData->vITIPCDig->vAverageImage)
								//	vLocalSystemData->vITIPCDig->SubtractVoidAverageImage(
								//	vLocalSystemData->vITIPCDig->vLatestImageCopy->pix);

								AccumulateVoidStructure(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix);

							}
							else
							{ //in auto setup
								/*
								CopyMemory(
									vLocalSystemData->vITIPCDig->vLatestImageCopy->pix,
									vLocalSystemData->vITIPCDig->vLatestImage->pix,
									vLocalSystemData->vITIPCDig->vOriginalBufferSize);

								AccumulateGlassStructure(vLocalSystemData->vITIPCDig->vLatestImage->pix);
*/
								CopyMemory(
									vLocalSystemData->vITIPCDig->vLatestImageCopy->pix,
									vLocalSystemData->vITIPCDig->vLatestImage->pix,
									vLocalSystemData->vITIPCDig->vOriginalBufferSize);

								AccumulateContaminant2Structure(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix);

								CopyMemory(
									vLocalSystemData->vITIPCDig->vLatestImageCopy->pix,
									vLocalSystemData->vITIPCDig->vLatestImage->pix,
									vLocalSystemData->vITIPCDig->vOriginalBufferSize);

								//subtract the product's average image from this one if enabled
								if (vLocalSystemData->vITIPCDig->vProductAverageImage)
									vLocalSystemData->vITIPCDig->SubtractProductAverageImage(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vProductAverageImage);

								if (vLocalSystemData->vITIPCDig->vStructureEnabled)
								if (vLocalSystemData->vInAutoSetup)
									AccumulateStructure(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix);

								CopyMemory(
									vLocalSystemData->vITIPCDig->vLatestImageCopy->pix,
									vLocalSystemData->vITIPCDig->vLatestImage->pix,
									vLocalSystemData->vITIPCDig->vOriginalBufferSize);

								//subtract the product's average image from this one if enabled
								if (vLocalSystemData->vITIPCDig->vProductAverageImage)
									vLocalSystemData->vITIPCDig->SubtractVoidProductAverageImage(
									vLocalSystemData->vITIPCDig->vLatestImageCopy->pix);

								if (vLocalSystemData->vITIPCDig->vStructureEnabled)
									AccumulateVoidStructure(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix);
							}
						}

						//did not eject container, so mark complete
						if (vLocalSystemData->vLogFile.vLogSerialData)
						if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
						if (vLocalSystemData->vLastContainerImaged)
						if (vGlobaluCSerialPort)
						{
							if (vLocalSystemData->vLogFile.vLogSerialData)
								vLocalSystemData->vLogFile.WriteToLogFile("IAT-PI-Image Analyzed c:" + 
								dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0) + " Good Container",cDebugMessage);
							/*
							vGlobaluCSerialPort->WriteToLogFileWithBeltPosition(
								"IAT-PI-Image Analyzed c:" + 
								dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0) + 
								" Good Container",
								vLocalSystemData->vLastContainerImaged->vBodyTriggerPosition);
								*/
						}
						if (vLocalSystemData->vLastContainerImaged)
							vLocalSystemData->vLastContainerImaged->vImageAnalyzed = true;
					} //end of MVTools license OK

					//temp for testing, only should have one above, just tempoarary for camlink boards without MVTools
					//if (vGlobalFrameGrabberType == cPC2CamLinkFrameGrabber)
					//if (vLocalSystemData->vLastContainerImaged)
					//	vLocalSystemData->vLastContainerImaged->vImageAnalyzed = true;
				} //end of not calibrating detectors and not measing source strength

				//filler monitor save 0 for weight if have not saved weight
				if (!vSavedFillerMonitorWeight)
				{//if wrong size, save a zero in the weight
					//valve monitoring
					if ((vLocalSystemData->vValveData) && (!vGlobalCurrentProduct->vFillerMonitorInspection))
					if (vLocalSystemData->vValveMonitorInspection != 0xFF)//need to save this valve value in temporary holder
						SaveDataInValveMonitor(vLocalSystemData->vLastContainerImaged, 0);
				}
			}  //end of should process this image
			else
			{
				if (vLocalSystemData->vLogFile.vLogSerialData)
				if (vLocalSystemData->vITIPCDig->vHaveRealImage)  //not simulated image 
				if (vLocalSystemData->vContainerCollection->GetCount() > 0)
				if (vLocalSystemData->vLastContainerImaged)
					vLocalSystemData->vLogFile.WriteToLogFile(dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0) + " Error - Did not analyze image as container was not found",cDebugMessage);
				vSizeOK = false;
				vLocalSystemData->vITIPCDig->vHaveRealImage = false;
			}
			
			if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) ||
				(vLocalConfigurationData->vScanTracType == cLegatoScanTrac) || (vLocalConfigurationData->vScanTracType == cFermataScanTrac)) //2 = 50W pipe Legato, 3= 600W Pipe
			{
				CTime TempTime;
				TempTime = CTime::GetCurrentTime();
				vLocalSystemData->vLastEjectTime = TempTime.Format("%H:%M:%S   %d-%b-%y");
			}

			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				if (vLocalSystemData->vLastContainerImaged)
					vLocalSystemData->vLogFile.WriteToLogFile("IAT-PI-Display New Image Notice: " + dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0),cDebugMessage);
				else
					vLocalSystemData->vLogFile.WriteToLogFile("IAT-PI-Display New Image Notice: " + vLocalSystemData->vFrameCountString,cDebugMessage);
			}

			//if got back to back containers and started diverting to non inspected area, and now
			//got correct size, start diverting containers to the inspected area.
			vLocalSystemData->vProcessImageCounter++;
			vLocalSystemData->vLogFile.WriteToLogFile("*Processed Image: " + dtoa(vLocalSystemData->vProcessImageCounter,0),cDebugMessage);

			if (vLocalConfigurationData->vDemoMode)
			if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
			if (vSizeOK)
			{
				vLocalSystemData->vGoodCountTotal++;
				vLocalSystemData->vTotalContainers++;
				vLocalSystemData->vImproveLearnTotalContainers++;
			}

			//Turn Diverter on if diverting uninspected containers
			if (vLocalConfigurationData->vEjectContainersNotInspected)
			if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
			if (!vLocalSystemData->vDiverterOn)
			if (vSizeOK)
			if (vLocalSystemData->vLastContainerImaged)
			{
				//::PostThreadMessage(vGlobalCallingThreadID,	cTurnDiverterOnMessage, 0,0);
				//vLocalSystemData->vDiverterOn = true;
				WORD TempDiverterOnPosition = (WORD)(vLocalSystemData->vLastContainerImaged->vBodyTriggerPosition + (vLocalConfigurationData->vDiverterMargin * vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier) +
					((vGlobalCurrentProduct->vEjectorDelayBeltPosition[vLocalConfigurationData->vDiverterEjector - 1] - vGlobalCurrentProduct->vProductImageWidth)));

				if (vGlobaluCSerialPort)
					vGlobaluCSerialPort->SendEjectCommand(vLocalConfigurationData->vDiverterEjector, TempDiverterOnPosition);

				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("Turn Diverter On Eject, Image Size OK",cDebugMessage);
				/*
				if (vLocalSystemData->vLogFile.vLogSerialData)
				if (vGlobaluCSerialPort)
					vGlobaluCSerialPort->WriteToLogFileWithBeltPosition("Turn Diverter On Eject, Image Size OK",
					TempDiverterOnPosition);
					*/
				vLocalSystemData->vDiverterOn = true;
			}

			if (!vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
			if (!vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
			{
				//vLocalSystemData->vDriftCompensationThresholdReductionAmount = 0;
				//vLocalSystemData->vDriftCompensationThresholdIncreaseAmount = 0;
				vLocalSystemData->vITIPCDig->vDriftCompensationLocked = true;
			}

			//do density compensation calcualtion and adjust source or offset if needed
			if (vLocalSystemData->vLastContainerImaged)
			if (vLocalSystemData->vLastContainerImaged->vTheImage)
			if ((!vLocalSystemData->vSendDACValues) && (!vLocalSystemData->vSendADCValues))
			if ((!vLocalSystemData->vLastContainerImaged->vToBeEjected) || //ignore container if ejected
				//but can't ignore all containers if all containers ejected, so if previous container
				//was ejected, count this container even if it was ejected
				//6/2/04
				((vLocalSystemData->vITIPCDig->vPreviousContainerEjected > 10) &&
				(vLocalSystemData->vInAutoSetup)) ||
				(vLocalSystemData->vITIPCDig->vPreviousContainerEjected > 100) ||
				(!vLocalConfigurationData->vEnableEjectors) ||
				(!vLocalSystemData->vITIPCDig->vDriftCompensationLocked))
			if (vGlobalCurrentProduct->vDriftDensityInspection)
			if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || ((vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem) && 
				(vLocalSystemData->vITIPCDig->vProductAverageImage)))
			if (vLocalSystemData->vITIPCDig->vHaveRealImage)
			if (vLocalSystemData->vLastXRayCurrentSetTime)
			if (vLocalSystemData->vKeepDensityFeedbackLoopLocked)
			if (vLocalConfigurationData->vDriftCompensationEnabled)
			if (vGlobalCurrentProduct->vSetupHistoryAverageDensity)
			if ((vLocalSystemData->vLastContainerImaged) && (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cPassSlippedContainer))//slipped
				vLocalSystemData->vWrongWidthsDuringLearn++;
			else
			if ((vSizeOK) || (vNumberOfWrongSizeInARow > 240))
			if (((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode)) || //if bulk mode, must be dark enough
				(vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity < cImageBrightnessThreshold))
			if (vCurrentImageBrightnessOK)
			{
				double TempTime = CIFCOS::GetSystimeMillisecs();
				TempTime = TempTime - vLocalSystemData->vLastXRayCurrentSetTime;
				if (TempTime > vLocalConfigurationData->vDriftCompensationWaitTime)
				{
					{
						vLocalSystemData->DensityOfLastNImages[vLocalSystemData->vIndexForDriftDensityOfLastNImages] = vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity; 
						
						vLocalSystemData->vIndexForDriftDensityOfLastNImages++;

						if (vLocalSystemData->vIndexForDriftDensityOfLastNImages >= vLocalConfigurationData->vNumberOfImagesForDriftDensity)
						{//if you have taken enough images you should now analyze them and correct if needed
							double TempAverage = 0;
							double TempCompensationOffset = vLocalSystemData->vDriftCompensationADCOffset;

							for (WORD TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfImagesForDriftDensity; TempLoop++)
								TempAverage = TempAverage + vLocalSystemData->DensityOfLastNImages[TempLoop];

							TempAverage = TempAverage / vLocalConfigurationData->vNumberOfImagesForDriftDensity;
							double TempDelta = TempAverage - vGlobalCurrentProduct->vSetupHistoryAverageDensity;
							vLocalSystemData->vDriftCompensationImageBrightnessDelta = TempDelta;

							//br ht
							//	ReportErrorMessage("Running Product Density: " + dtoa(TempAverage,3), cAction,0);
							if (vLocalSystemData->vLogFile.vLogSerialData)
								vLocalSystemData->vLogFile.WriteToLogFile("Running Product Density: " + dtoa(TempAverage,3),cDebugMessage);

							//on Legato pipeline systems, do not want to change brightness if change too much as pipe may be empty
							//so change offset brighness if not a Legato, or delta brighness is smaller than setting
							if (((vLocalConfigurationData->vScanTracType != cLegatoScanTrac) && (vLocalConfigurationData->vScanTracType != cFermataScanTrac)) || 
								((vLocalConfigurationData->vMaxImageIntensityAdjust) && (TempDelta < vLocalConfigurationData->vMaxImageIntensityAdjust)) || 
								((!vLocalSystemData->vITIPCDig->vDriftCompensationLocked) && (TempDelta < vLocalConfigurationData->vMaxImageIntensityAdjustBeforeLocked)))
							if (TempDelta > vLocalConfigurationData->vDriftCompensationVeryFastThreshold)
							{  //way over normal density, so reduce current quickly
								if (-vLocalConfigurationData->vDriftCompensationMaximumAdjust < vLocalSystemData->vDriftCompensationADCOffset - vLocalConfigurationData->vDriftCompensationVeryFastAdjust)
								{
									vLocalSystemData->vDriftCompensationADCOffset = vLocalSystemData->vDriftCompensationADCOffset - (int)vLocalConfigurationData->vDriftCompensationVeryFastAdjust;
								}
								else
								{
									vLocalSystemData->vDriftCompensationADCOffset = -(int)vLocalConfigurationData->vDriftCompensationMaximumAdjust;
								}
								if (vLocalSystemData->vLogFile.vLogSerialData)
									vLocalSystemData->vLogFile.WriteToLogFile("Very Fast Adjust-: " + dtoa(vLocalSystemData->vDriftCompensationADCOffset,2),cDebugMessage);

								//ReportErrorMessage("Density Compensation Adjustment By: -" + dtoa(vLocalConfigurationData->vDriftCompensationVeryFastAdjust, 2), cWriteToLog,0); //for testing

								double TempSteps = TempDelta / vLocalConfigurationData->vDriftCompensationSlowThreshold;
								if (vLocalSystemData->vLogFile.vLogSerialData)
									vLocalSystemData->vLogFile.WriteToLogFile( "Delta: " + dtoa(TempDelta,2),cDebugMessage);							

								if ((TempSteps > 3) && (vLocalSystemData->vSystemRunMode == cRunningSystemMode))
								{
									//vLocalSystemData->vDriftCompensationThresholdIncreaseAmount = (TempSteps - 1) * vLocalConfigurationData->vDriftCompensationThresholdReductionFactor;

									//if (vLocalSystemData->vDriftCompensationThresholdIncreaseAmount > vLocalConfigurationData->vDriftCompensationThresholdReductionMaximum)
									//	vLocalSystemData->vDriftCompensationThresholdIncreaseAmount = vLocalConfigurationData->vDriftCompensationThresholdReductionMaximum;

									//if (vLocalSystemData->vDriftCompensationThresholdIncreaseAmount < 2)
										vLocalSystemData->vDriftCompensationThresholdIncreaseAmount = 0;
								}
								else
								{
									//vLocalSystemData->vDriftCompensationThresholdIncreaseAmount = 0;
									if (!vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
									{
										vLocalSystemData->vITIPCDig->vDriftCompensationLocked = true;
										::PostThreadMessage(vGlobalCallingThreadID, cDensityLoopLockedMessage, 0,0);
										::PostThreadMessage(vGlobalCallingThreadID, cDensityLoopLockedClearMessage, 0,0);
									}
								}
								//vLocalSystemData->vDriftCompensationThresholdReductionAmount = 0;
							}
							else
							if (TempDelta > vLocalConfigurationData->vDriftCompensationFastThreshold)
							{  //way over normal density, so reduce current quickly
								if (-vLocalConfigurationData->vDriftCompensationMaximumAdjust < vLocalSystemData->vDriftCompensationADCOffset - vLocalConfigurationData->vDriftCompensationFastAdjust)
								{
									vLocalSystemData->vDriftCompensationADCOffset = vLocalSystemData->vDriftCompensationADCOffset - (int)vLocalConfigurationData->vDriftCompensationFastAdjust;
								}
								else
								{
									vLocalSystemData->vDriftCompensationADCOffset = -(int)vLocalConfigurationData->vDriftCompensationMaximumAdjust;
								}
								if (vLocalSystemData->vLogFile.vLogSerialData)
									vLocalSystemData->vLogFile.WriteToLogFile("Fast Adjust-: " + dtoa(vLocalSystemData->vDriftCompensationADCOffset,2),cDebugMessage);

								//ReportErrorMessage("Density Compensation Adjustment By: -" + dtoa(vLocalConfigurationData->vDriftCompensationFastAdjust, 2), cWriteToLog,0); //for testing

								double TempSteps = TempDelta / vLocalConfigurationData->vDriftCompensationSlowThreshold;

								if (vLocalSystemData->vLogFile.vLogSerialData)
									vLocalSystemData->vLogFile.WriteToLogFile("Delta: " + dtoa(TempDelta,2),cDebugMessage);							

								if ((TempSteps > 3) && (vLocalSystemData->vSystemRunMode == cRunningSystemMode))
								{
									//vLocalSystemData->vDriftCompensationThresholdIncreaseAmount = (TempSteps - 1) * vLocalConfigurationData->vDriftCompensationThresholdReductionFactor;

									//if (vLocalSystemData->vDriftCompensationThresholdIncreaseAmount > vLocalConfigurationData->vDriftCompensationThresholdReductionMaximum)
									//	vLocalSystemData->vDriftCompensationThresholdIncreaseAmount = vLocalConfigurationData->vDriftCompensationThresholdReductionMaximum;

									//if (vLocalSystemData->vDriftCompensationThresholdIncreaseAmount < 2)
										vLocalSystemData->vDriftCompensationThresholdIncreaseAmount = 0;
								}
								else
								{
									//vLocalSystemData->vDriftCompensationThresholdIncreaseAmount = 0;
									if (!vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
									{
										vLocalSystemData->vITIPCDig->vDriftCompensationLocked = true;
										::PostThreadMessage(vGlobalCallingThreadID, cDensityLoopLockedMessage, 0,0);
										::PostThreadMessage(vGlobalCallingThreadID, cDensityLoopLockedClearMessage, 0,0);
									}
								}
								//vLocalSystemData->vDriftCompensationThresholdReductionAmount = 0;
							}
							else
							if (TempDelta > vLocalConfigurationData->vDriftCompensationSlowThreshold)
							{//slightly over normal density, so reduce current slowly
								if (-vLocalConfigurationData->vDriftCompensationMaximumAdjust < vLocalSystemData->vDriftCompensationADCOffset - vLocalConfigurationData->vDriftCompensationSlowAdjust)
								{
									vLocalSystemData->vDriftCompensationADCOffset = vLocalSystemData->vDriftCompensationADCOffset - (int)vLocalConfigurationData->vDriftCompensationSlowAdjust;
								}
								else
								{
									vLocalSystemData->vDriftCompensationADCOffset = -(int)vLocalConfigurationData->vDriftCompensationMaximumAdjust;
								}
								if (vLocalSystemData->vLogFile.vLogSerialData)
									vLocalSystemData->vLogFile.WriteToLogFile("Slow Adjust-: " + dtoa(vLocalSystemData->vDriftCompensationADCOffset,2),cDebugMessage);

								double TempSteps = TempDelta / vLocalConfigurationData->vDriftCompensationSlowThreshold;

								if (vLocalSystemData->vLogFile.vLogSerialData)
									vLocalSystemData->vLogFile.WriteToLogFile("Delta: " + dtoa(TempDelta,2),cDebugMessage);

								ReportErrorMessage("Decreased Density Offset: " + dtoa(vLocalConfigurationData->vDriftCompensationSlowAdjust,2), cWriteToLog,0);
								//ReportErrorMessage("Density Compensation Adjustment By: -" + dtoa(vLocalConfigurationData->vDriftCompensationSlowAdjust, 2), cWriteToLog,0); //for testing

	/*
								if ((TempSteps > 3) && (vLocalSystemData->vSystemRunMode == cRunningSystemMode))
								{
									vLocalSystemData->vDriftCompensationThresholdIncreaseAmount = 
										(TempSteps - 1) *
										vLocalConfigurationData->vDriftCompensationThresholdReductionFactor;
									if (vLocalSystemData->vDriftCompensationThresholdIncreaseAmount >
										vLocalConfigurationData->vDriftCompensationThresholdReductionMaximum)
										vLocalSystemData->vDriftCompensationThresholdIncreaseAmount =
											vLocalConfigurationData->vDriftCompensationThresholdReductionMaximum;
									if (vLocalSystemData->vDriftCompensationThresholdIncreaseAmount < 2)
										vLocalSystemData->vDriftCompensationThresholdIncreaseAmount = 0;
								}
								else
								{
								*/
									//vLocalSystemData->vDriftCompensationThresholdIncreaseAmount = 0;
									if (!vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
									{
										vLocalSystemData->vITIPCDig->vDriftCompensationLocked = true;
										::PostThreadMessage(vGlobalCallingThreadID, cDensityLoopLockedMessage, 0,0);
										::PostThreadMessage(vGlobalCallingThreadID, cDensityLoopLockedClearMessage, 0,0);
									}
								//}
								//vLocalSystemData->vDriftCompensationThresholdReductionAmount = 0;
							}
							else
							if (TempDelta < -vLocalConfigurationData->vDriftCompensationVeryFastThreshold)
							{//way under normal density, so increase current quickly
								if (vLocalConfigurationData->vDriftCompensationMaximumAdjust > vLocalSystemData->vDriftCompensationADCOffset + vLocalConfigurationData->vDriftCompensationVeryFastAdjust)
								{
									vLocalSystemData->vDriftCompensationADCOffset = vLocalSystemData->vDriftCompensationADCOffset + (int)vLocalConfigurationData->vDriftCompensationVeryFastAdjust;
								}
								else
								{
									vLocalSystemData->vDriftCompensationADCOffset = (int)vLocalConfigurationData->vDriftCompensationMaximumAdjust;
								}
								if (vLocalSystemData->vLogFile.vLogSerialData)
									vLocalSystemData->vLogFile.WriteToLogFile("Very Fast Adjust+: " + dtoa(vLocalSystemData->vDriftCompensationADCOffset,2),cDebugMessage);

								ReportErrorMessage("Increased Density Offset: " + dtoa(vLocalConfigurationData->vDriftCompensationVeryFastAdjust,2), cWriteToLog,0);

								double TempSteps = TempDelta / vLocalConfigurationData->vDriftCompensationSlowThreshold;
								if (vLocalSystemData->vLogFile.vLogSerialData)
									vLocalSystemData->vLogFile.WriteToLogFile("Delta: " + dtoa(TempDelta,2),cDebugMessage);
									
								if ((TempSteps < 3) && (vLocalSystemData->vSystemRunMode == cRunningSystemMode))
								{
									//vLocalSystemData->vDriftCompensationThresholdReductionAmount = -(TempSteps + 1) * vLocalConfigurationData->vDriftCompensationThresholdReductionFactor;

									//if (vLocalSystemData->vDriftCompensationThresholdReductionAmount > vLocalConfigurationData->vDriftCompensationThresholdReductionMaximum)
									//	vLocalSystemData->vDriftCompensationThresholdReductionAmount = vLocalConfigurationData->vDriftCompensationThresholdReductionMaximum;

									//if (vLocalSystemData->vDriftCompensationThresholdReductionAmount < 2)
										vLocalSystemData->vDriftCompensationThresholdReductionAmount = 0;
								}
								else
								{
									//vLocalSystemData->vDriftCompensationThresholdReductionAmount = 0;
									if (!vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
									{
										vLocalSystemData->vITIPCDig->vDriftCompensationLocked = true;
										::PostThreadMessage(vGlobalCallingThreadID, cDensityLoopLockedMessage, 0,0);
										::PostThreadMessage(vGlobalCallingThreadID, cDensityLoopLockedClearMessage, 0,0);
									}
								}
								//vLocalSystemData->vDriftCompensationThresholdIncreaseAmount = 0;
								//if (vLocalSystemData->vLogFile.vLogSerialData)
								//	vLocalSystemData->vLogFile.WriteToLogFile("Threshold Comp: " + dtoa(vLocalSystemData-> vDriftCompensationThresholdReductionAmount,2),cDebugMessage);

								//ReportErrorMessage("Density Compensation Adjustment By: " + dtoa(vLocalConfigurationData->vDriftCompensationVeryFastAdjust, 2), cWriteToLog,0); //for testing
							}
							else
							if (TempDelta < -vLocalConfigurationData->vDriftCompensationFastThreshold)
							{//way under normal density, so increase current quickly
								if (vLocalConfigurationData->vDriftCompensationMaximumAdjust > vLocalSystemData->vDriftCompensationADCOffset + vLocalConfigurationData->vDriftCompensationFastAdjust)
								{
									vLocalSystemData->vDriftCompensationADCOffset = vLocalSystemData->vDriftCompensationADCOffset + (int)vLocalConfigurationData->vDriftCompensationFastAdjust;
								}
								else
								{
									vLocalSystemData->vDriftCompensationADCOffset = (int)vLocalConfigurationData->vDriftCompensationMaximumAdjust;
								}
								if (vLocalSystemData->vLogFile.vLogSerialData)
									vLocalSystemData->vLogFile.WriteToLogFile("Fast Adjust+: " + dtoa(vLocalSystemData->vDriftCompensationADCOffset,2),cDebugMessage);

								ReportErrorMessage("Increased Density Offset: " + dtoa(vLocalConfigurationData->vDriftCompensationFastAdjust,2), cWriteToLog,0);

								double TempSteps = TempDelta / vLocalConfigurationData->vDriftCompensationSlowThreshold;
								if (vLocalSystemData->vLogFile.vLogSerialData)
									vLocalSystemData->vLogFile.WriteToLogFile("Delta: " + dtoa(TempDelta,2),cDebugMessage);
									
								if ((TempSteps < 3) && (vLocalSystemData->vSystemRunMode == cRunningSystemMode))
								{
									//vLocalSystemData->vDriftCompensationThresholdReductionAmount = -(TempSteps + 1) * vLocalConfigurationData->vDriftCompensationThresholdReductionFactor;

									//if (vLocalSystemData->vDriftCompensationThresholdReductionAmount > vLocalConfigurationData->vDriftCompensationThresholdReductionMaximum)
									//	vLocalSystemData->vDriftCompensationThresholdReductionAmount = vLocalConfigurationData->vDriftCompensationThresholdReductionMaximum;

									//if (vLocalSystemData->vDriftCompensationThresholdReductionAmount < 2)
										vLocalSystemData->vDriftCompensationThresholdReductionAmount = 0;
								}
								else
								{
									//vLocalSystemData->vDriftCompensationThresholdReductionAmount = 0;
									if (!vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
									{
										vLocalSystemData->vITIPCDig->vDriftCompensationLocked = true;
										::PostThreadMessage(vGlobalCallingThreadID, cDensityLoopLockedMessage, 0,0);
										::PostThreadMessage(vGlobalCallingThreadID, cDensityLoopLockedClearMessage, 0,0);
									}
								}
								//vLocalSystemData->vDriftCompensationThresholdIncreaseAmount = 0;
								//if (vLocalSystemData->vLogFile.vLogSerialData)
								//	vLocalSystemData->vLogFile.WriteToLogFile("Threshold Comp: " + dtoa(vLocalSystemData->vDriftCompensationThresholdReductionAmount,2),cDebugMessage);

								//ReportErrorMessage("Density Compensation Adjustment By: " + dtoa(vLocalConfigurationData->vDriftCompensationFastAdjust, 2), cWriteToLog,0); //for testing
							}
							else
							if (TempDelta < -vLocalConfigurationData->vDriftCompensationSlowThreshold)
							{//just under normal density, so increase current slowly
								if (vLocalConfigurationData->vDriftCompensationMaximumAdjust > vLocalSystemData->vDriftCompensationADCOffset + vLocalConfigurationData->vDriftCompensationSlowAdjust)
								{
									vLocalSystemData->vDriftCompensationADCOffset = vLocalSystemData->vDriftCompensationADCOffset + (int)vLocalConfigurationData->vDriftCompensationSlowAdjust;
								}
								else
								{
									vLocalSystemData->vDriftCompensationADCOffset = (int)vLocalConfigurationData->vDriftCompensationMaximumAdjust;
								}
								if (vLocalSystemData->vLogFile.vLogSerialData)
									vLocalSystemData->vLogFile.WriteToLogFile("Slow Adjust+: " + dtoa(vLocalSystemData->vDriftCompensationADCOffset,2),cDebugMessage);

								double TempSteps = TempDelta / vLocalConfigurationData->vDriftCompensationSlowThreshold;

								if (vLocalSystemData->vLogFile.vLogSerialData)
									vLocalSystemData->vLogFile.WriteToLogFile("Delta: " + dtoa(TempDelta,2),cDebugMessage);

								//vLocalSystemData->vDriftCompensationThresholdReductionAmount = 0;
								if (!vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
								{
									vLocalSystemData->vITIPCDig->vDriftCompensationLocked = true;
									::PostThreadMessage(vGlobalCallingThreadID, cDensityLoopLockedMessage, 0,0);
									::PostThreadMessage(vGlobalCallingThreadID, cDensityLoopLockedClearMessage, 0,0);
								}

								ReportErrorMessage("Increased Density Offset: " + dtoa(vLocalConfigurationData->vDriftCompensationSlowAdjust,2), cWriteToLog,0);
							//vLocalSystemData->vDriftCompensationThresholdIncreaseAmount = 0;
								//if (vLocalSystemData->vLogFile.vLogSerialData)
								//	vLocalSystemData->vLogFile.WriteToLogFile("Threshold Comp: " + dtoa(vLocalSystemData->vDriftCompensationThresholdReductionAmount,2),cDebugMessage);

								//ReportErrorMessage("Density Compensation Adjustment By: " + dtoa(vLocalConfigurationData->vDriftCompensationSlowAdjust, 2), cWriteToLog,0); //for testing
							}
							else
							{
								//vLocalSystemData->vDriftCompensationThresholdReductionAmount = 0;
								//vLocalSystemData->vDriftCompensationThresholdIncreaseAmount = 0;
								if (!vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
								{
									vLocalSystemData->vITIPCDig->vDriftCompensationLocked = true;
									::PostThreadMessage(vGlobalCallingThreadID, cDensityLoopLockedClearMessage, 0,0);
									::PostThreadMessage(vGlobalCallingThreadID, cDensityLoopLockedMessage, 0,0);
								}
							}
							vLocalSystemData->vIndexForDriftDensityOfLastNImages = 0;

							//if you changed the drift compensation, send commands to uController
							if (TempCompensationOffset != vLocalSystemData->vDriftCompensationADCOffset)
							if (vGlobaluCSerialPort)
							{
								if (vLocalConfigurationData->vUseDACAndADCinDensityCompensation)
								{
									//if hit or exceeded limit on ADC then change DAC by one and adjust ADC by limit
									int TempMaximumADCValue = 7; //was 12 , 7 is calculated correct conversion factor worked pretty well
									//int TempADCStepValue = 4; //was 12   7 worked pretty well
									if (vLocalSystemData->vDriftCompensationADCOffset >= TempMaximumADCValue)
									{
										//ReportErrorMessage("Density Compensation-ADC at high limit increase DAC", cError,0);// temp for testing
										bool TempAtMaximumValue = false;
										while ((vLocalSystemData->vDriftCompensationADCOffset >= TempMaximumADCValue) && (!TempAtMaximumValue))
										{
											//DAC works opposite on version 10 board as it does before
											if (vGlobalFPGAVersion10Point0OrHigher)
											{
												int TempMaximumOffsetValue = 2048;
												if (cNumberOfOffsetBits == 12)
													TempMaximumOffsetValue = 4096;
												if (cNumberOfOffsetBits == 10)
													TempMaximumOffsetValue = 1024;
												if (cNumberOfOffsetBits == 9)
													TempMaximumOffsetValue = 512;

												double TempMinimumOffsetOfDetectors = 999999;
												for (BYTE TempDetectorLoop = 0; TempDetectorLoop < vLocalSystemData->vNumberOfXRayDetectorsUsing; TempDetectorLoop++)
												if (TempMinimumOffsetOfDetectors > vLocalSystemData->vDACDetectorOffset[TempDetectorLoop])
													TempMinimumOffsetOfDetectors = vLocalSystemData->vDACDetectorOffset[TempDetectorLoop];

												int TempMinimumOffsetWord = (WORD)((TempMinimumOffsetOfDetectors /  100.0) * TempMaximumOffsetValue);

												if (TempMinimumOffsetWord + vLocalSystemData->vDriftCompensationDACOffset > 0)  //if DAC at limit, just let ADC do the change
												{
													vLocalSystemData->vDriftCompensationDACOffset = vLocalSystemData->vDriftCompensationDACOffset--;
													vLocalSystemData->vDriftCompensationADCOffset = vLocalSystemData->vDriftCompensationADCOffset - TempMaximumADCValue;
												}
												else
													TempAtMaximumValue = true;
											}
											else
											{
												int TempMaximumOffsetValue = 2048; //FPGA 9.2 Uses 11 bits by default, can't be less than 11 bits
												if (cNumberOfOffsetBits == 12)
													TempMaximumOffsetValue = 4096;

												double TempMaximumOffsetOfDetectors = 0;
												for (BYTE TempDetectorLoop = 0; TempDetectorLoop < vLocalSystemData->vNumberOfXRayDetectorsUsing; TempDetectorLoop++)
												if (TempMaximumOffsetOfDetectors < vLocalSystemData->vDACDetectorOffset[TempDetectorLoop])
													TempMaximumOffsetOfDetectors = vLocalSystemData->vDACDetectorOffset[TempDetectorLoop];

												int TempMaximumOffsetWord = (WORD)((TempMaximumOffsetOfDetectors / 100.0) * TempMaximumOffsetValue);

												if (TempMaximumOffsetWord + vLocalSystemData->vDriftCompensationDACOffset < TempMaximumOffsetValue - 2)  //if DAC at limit, just let ADC do the change
												{
													vLocalSystemData->vDriftCompensationDACOffset = vLocalSystemData->vDriftCompensationDACOffset++;
													vLocalSystemData->vDriftCompensationADCOffset = vLocalSystemData->vDriftCompensationADCOffset - TempMaximumADCValue;
												}
											}
										}
										vMainWindowPointer->SendAllDACDetectorOffsets();
										//ReportErrorMessage("Density Compensation-Adjust DACs: " + dtoa(vLocalSystemData->vDriftCompensationDACOffset), cWriteToLog,0); // for testing
									}
									else
									if (vLocalSystemData->vDriftCompensationADCOffset < -TempMaximumADCValue)
									{
										bool TempAtMaximumValue = false;
										//ReportErrorMessage("Density Compensation-ADC at Low limit decrease DAC", cError,0);// temp for testing
										while ((vLocalSystemData->vDriftCompensationADCOffset < -TempMaximumADCValue) && (!TempAtMaximumValue))
										{
											//DAC works opposite on version 10 board as it does before
											if (vGlobalFPGAVersion10Point0OrHigher)
											{
												int TempMaximumOffsetValue = 2048;
												if (cNumberOfOffsetBits == 12)
													TempMaximumOffsetValue = 4096;
												if (cNumberOfOffsetBits == 10)
													TempMaximumOffsetValue = 1024;
												if (cNumberOfOffsetBits == 9)
													TempMaximumOffsetValue = 512;

												double TempMaximumOffsetOfDetectors = 0;
												for (BYTE TempDetectorLoop = 0; TempDetectorLoop < vLocalSystemData->vNumberOfXRayDetectorsUsing; TempDetectorLoop++)
												if (TempMaximumOffsetOfDetectors < vLocalSystemData->vDACDetectorOffset[TempDetectorLoop])
													TempMaximumOffsetOfDetectors = vLocalSystemData->vDACDetectorOffset[TempDetectorLoop];

												int TempMaximumOffsetWord = (WORD)((TempMaximumOffsetOfDetectors /  100.0) * TempMaximumOffsetValue);

												if (TempMaximumOffsetWord + vLocalSystemData->vDriftCompensationDACOffset < TempMaximumOffsetValue - 2)  //if DAC at limit, just let ADC do the change
												{
													vLocalSystemData->vDriftCompensationDACOffset = vLocalSystemData->vDriftCompensationDACOffset++;
													vLocalSystemData->vDriftCompensationADCOffset = vLocalSystemData->vDriftCompensationADCOffset + TempMaximumADCValue;
												}
												else
													TempAtMaximumValue = true;
											}
											else
											{
												int TempMaximumOffsetValue = 2048; //FPGA 9.2 Uses 11 bits by default, can't be less than 11 bits
												if (cNumberOfOffsetBits == 12)
													TempMaximumOffsetValue = 4096;

												double TempMinimumOffsetOfDetectors = 999999;
												for (BYTE TempDetectorLoop = 0; TempDetectorLoop < vLocalSystemData->vNumberOfXRayDetectorsUsing; TempDetectorLoop++)
													if (TempMinimumOffsetOfDetectors > vLocalSystemData->vDACDetectorOffset[TempDetectorLoop])
														TempMinimumOffsetOfDetectors = vLocalSystemData->vDACDetectorOffset[TempDetectorLoop];

												int TempMinimumOffsetWord = (WORD)((TempMinimumOffsetOfDetectors / 100.0) * TempMaximumOffsetValue);

												if (TempMinimumOffsetWord + vLocalSystemData->vDriftCompensationDACOffset > 0)  //if DAC at limit, just let ADC do the change
												{
													vLocalSystemData->vDriftCompensationDACOffset = vLocalSystemData->vDriftCompensationDACOffset--;
													vLocalSystemData->vDriftCompensationADCOffset = vLocalSystemData->vDriftCompensationADCOffset + TempMaximumADCValue;
												}
											}
										}
										vMainWindowPointer->SendAllDACDetectorOffsets();
										//ReportErrorMessage("Density Compensation-Adjust DACs: " + dtoa(vLocalSystemData->vDriftCompensationDACOffset), cWriteToLog,0); // for testing
									}
								}
								vMainWindowPointer->SendAllADCOffsets();
								//ReportErrorMessage("Density Compensation-Adjust ADCs: " + dtoa(vLocalSystemData->vDriftCompensationADCOffset), cWriteToLog,0); // for testing
							}

							//ver 10 DAC is opposite so subtract
							if (vGlobalFPGAVersion10Point0OrHigher)
								vLocalSystemData->vCurrentBrightnessAdjustment = (vLocalSystemData->vDriftCompensationADCOffset * .13) - (vLocalSystemData->vDriftCompensationDACOffset * 0.91);//was 1.22
									//(vLocalSystemData->vDriftCompensationADCOffset / 7.7) - 
									//(vLocalSystemData->vDriftCompensationDACOffset * 1.5);//was 2
							else
								vLocalSystemData->vCurrentBrightnessAdjustment = (vLocalSystemData->vDriftCompensationADCOffset / 7.7) + (vLocalSystemData->vDriftCompensationDACOffset * 1.5);//was 2

							//ReportErrorMessage("Brightness Adjust, DAC: " + dtoa(vLocalSystemData->vDriftCompensationDACOffset,3) + 
							//	" ADC: " + dtoa(vLocalSystemData->vDriftCompensationADCOffset,3) + 
							//	" Corr: " + dtoa(vLocalSystemData->vCurrentBrightnessAdjustment,3), cAction,0);

							//adjust brightness before relearn
							if (vLocalSystemData->vAdjustingBrightnessBeforeLearning)
							if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
							{
								double TempTotalBrightnessCompensation = vLocalSystemData->vCurrentBrightnessAdjustment - vLocalSystemData->vDriftCompensationImageBrightnessDelta;

								//ReportErrorMessage("Brightness Off: " + dtoa(vLocalSystemData->vDriftCompensationImageBrightnessDelta,3) + 
								//	" Total Compensation: " + dtoa(TempTotalBrightnessCompensation,3), cAction,0);

								//if ScanTrac has compensated and off in brightness by too much then tweak the current or voltage to get to learn level 
								if ((TempTotalBrightnessCompensation > 2) || (TempTotalBrightnessCompensation < -2))
								{
									double TempAdjustAmount = TempTotalBrightnessCompensation / 200.0;
									if ((TempAdjustAmount > 0) && (TempAdjustAmount < .01))
										TempAdjustAmount = .01;
									if ((TempAdjustAmount < 0) && (TempAdjustAmount > -.01))
										TempAdjustAmount = -.01;
									if (TempTotalBrightnessCompensation < 0)
									{
										//make image darker
										//SetStatusLine(" Send Containers, Adjusting Image Brightness-");
										if (vGlobalCurrentProduct->vXRaySourceVoltage + TempAdjustAmount > 20 / 6)
										{
											vGlobalCurrentProduct->vXRaySourceVoltage = (float)(vGlobalCurrentProduct->vXRaySourceVoltage + TempAdjustAmount);
											vLocalSystemData->vRampXRayVoltageAmount = vGlobalCurrentProduct->vXRaySourceVoltage;
											vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount,vLocalSystemData->vRampXRayCurrentAmount);
											ReportErrorMessage("Adjust Exposure Before Relearn Voltage: " + dtoa(TempAdjustAmount,2),cAction,0);
										}
										else
										if (vGlobalCurrentProduct->vXRaySourceCurrent + TempAdjustAmount > .5)
										{
											vGlobalCurrentProduct->vXRaySourceCurrent = (float)(vGlobalCurrentProduct->vXRaySourceCurrent + TempAdjustAmount);
											vLocalSystemData->vRampXRayCurrentAmount = vGlobalCurrentProduct->vXRaySourceCurrent;
											vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount,vLocalSystemData->vRampXRayCurrentAmount);
											ReportErrorMessage("Adjust Exposure Before Relearn Current: " + dtoa(TempAdjustAmount,2),cAction,0);
										}
										else
										{
											//SetStatusLine(" Adjusting Image Brightness To Minimum");
											ReportErrorMessage("Adjust Exposure Before Relearn To Minimum",cAction,0);
											vLocalSystemData->vAdjustingBrightnessBeforeLearning = false;
											//StartLearning();
										}
									}
									else
									{
										//make image brighter
										//SetStatusLine(" Send Containers, Adjusting Image Brightness+");
										if (vGlobalCurrentProduct->vXRaySourceVoltage + TempAdjustAmount <= vLocalConfigurationData->vMaximumVoltage / 6)
										{
											vGlobalCurrentProduct->vXRaySourceVoltage = (float)(vGlobalCurrentProduct->vXRaySourceVoltage + TempAdjustAmount);
											vLocalSystemData->vRampXRayVoltageAmount = vGlobalCurrentProduct->vXRaySourceVoltage;
											vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount,vLocalSystemData->vRampXRayCurrentAmount);
											ReportErrorMessage("Adjust Exposure Before Relearn Voltage: " + dtoa(TempAdjustAmount,2),cAction,0);
										}
										else
										if (vGlobalCurrentProduct->vXRaySourceCurrent + TempAdjustAmount <= vLocalConfigurationData->vMaximumCurrent)
										{
											vGlobalCurrentProduct->vXRaySourceCurrent = (float)(vGlobalCurrentProduct->vXRaySourceCurrent + TempAdjustAmount);
											vLocalSystemData->vRampXRayCurrentAmount = vGlobalCurrentProduct->vXRaySourceCurrent;
											vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount,vLocalSystemData->vRampXRayCurrentAmount);
											ReportErrorMessage("Adjust Exposure Before Relearn Current: " + dtoa(TempAdjustAmount,2),cAction,0);
										}
										else
										{
											//SetStatusLine(" Adjusting Image Brightness To Maximum");
											ReportErrorMessage("Adjust Exposure Before Relearn To Maximum",cAction,0);
											vLocalSystemData->vAdjustingBrightnessBeforeLearning = false;
											//StartLearning();
										}
									}
								}
								else
								{
									//SetStatusLine(" Adjusting Image Brightness Complete");
									ReportErrorMessage("Adjust Exposure Before Relearn Completed, Start Relearn",cAction,0);
									vLocalSystemData->vAdjustingBrightnessBeforeLearning = false;
									//StartLearning();
								}
							}
						}
					}
				}
			}

			if (vLocalSystemData->vLastContainerImaged)
			if (vLocalSystemData->vLastContainerImaged->vToBeEjected)
			{
				if (vLocalSystemData->vITIPCDig->vPreviousContainerEjected < 250)
					vLocalSystemData->vITIPCDig->vPreviousContainerEjected++;
			}
			else
				vLocalSystemData->vITIPCDig->vPreviousContainerEjected = 0;

			if (vLocalSystemData->vITIPCDig->vAutoImproveStatisticsCount < 65000)
				vLocalSystemData->vITIPCDig->vAutoImproveStatisticsCount++;

			if (vLocalSystemData->vITIPCDig->vAutoImproveStatisticsCountTo100 < 65000)
				vLocalSystemData->vITIPCDig->vAutoImproveStatisticsCountTo100++;

			if (vLocalSystemData->vLastContainerImaged)
			if (vLocalSystemData->vLastContainerImaged->vToBeEjected)
			if (vLocalSystemData->vITIPCDig->vAutoImproveStatisticsCountEjectedIn100 < 65000)
				vLocalSystemData->vITIPCDig->vAutoImproveStatisticsCountEjectedIn100++;

			TempEndTime = CIFCOS::GetSystimeMicrosecs();
			vLocalSystemData->vITIPCDig->vLastProcessTime = TempEndTime - TempStartProcessTime;

			//save good images in reject buffer if in Inspx Password Mode
			if (vLocalSystemData->vLastContainerImaged)
			if (vLocalSystemData->vLastContainerImaged->vTheImage)
			if ((PasswordOK(cTemporaryInspxPassword,false)) || (vLocalSystemData->vGoodCountInBufferTotal < vLocalSystemData->vITIPCDig->vNumberOfLastRejectImages))
			if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == 255)
			if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem) || (vLoadedThisImageFromFile))
			if ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode)) //if bulk mode, must be dark enough
			{
				if (vLocalSystemData->vGoodCountInBufferTotal < vLocalSystemData->vITIPCDig->vNumberOfLastRejectImages)
				vLocalSystemData->vGoodCountInBufferTotal++;

				if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason != cTestEject)
					vLocalSystemData->vITIPCDig->SaveThisRejectInBuffer(vLocalSystemData->vLastContainerImaged, vLocalSystemData->vLastContainerImaged->vContainerNumber,
						TempInspectionTopReferenceAdjust, vLocalSystemData->vGoodImagesIndex - 1);
			}

			vLocalSystemData->vImageProcessingTimeData[vLocalSystemData->vImageProcessingTimeIndex] = (float)(vLocalSystemData->vITIPCDig->vLastProcessTime);

			if ((vLocalSystemData->vImageProcessingTimeData[vLocalSystemData->vImageProcessingTimeIndex] < vLocalSystemData->vImageProcessingTimeMinimum) || (!vLocalSystemData->vImageProcessingTimeMinimum))
				vLocalSystemData->vImageProcessingTimeMinimum = vLocalSystemData->vImageProcessingTimeData[vLocalSystemData->vImageProcessingTimeIndex];

			if (vLocalSystemData->vImageProcessingTimeData[vLocalSystemData->vImageProcessingTimeIndex] > vLocalSystemData->vImageProcessingTimeMaximum)
				vLocalSystemData->vImageProcessingTimeMaximum = vLocalSystemData->vImageProcessingTimeData[vLocalSystemData->vImageProcessingTimeIndex];

			if (vLocalSystemData->vImageProcessingTimeSamples < cImageProcessingTimeMaximum - 1)
				vLocalSystemData->vImageProcessingTimeSamples++;

			vLocalSystemData->vImageProcessingTimeIndex++;
			if (vLocalSystemData->vImageProcessingTimeIndex >= cImageProcessingTimeMaximum)
				vLocalSystemData->vImageProcessingTimeIndex = 0;

			//if (vLocalSystemData->vSystemRunMode == cCalibratingDetectorOffsetMode)
			//{
			//	if (vLocalSystemData->vCalibrateUsingDAC)
			//		ProcessCalibrateDetectorOffsetFrameDAC();
			//	else
			//		ProcessCalibrateDetectorOffsetFrameADC();
			//}

			//write rejects to Hard Disk
			//save reject for write to file
			if (vLocalSystemData->vLastContainerImaged)
			if (vLocalSystemData->vLastContainerImaged->vTheImage)
			if (vLocalConfigurationData->vWriteRejectsToFiles)
			if ((vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason > 0) || (vLocalConfigurationData->vSaveAllImagesToRejectsFolder))
			if ((vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason <= vGlobalCurrentProduct->vNumberOfInspections) || (vLocalConfigurationData->vSaveAllImagesToRejectsFolder))
//b rh comment out next three lines to use demo data 8/30/12			
			if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
			if ((!vLocalSystemData->vITIPCDig->vShowOnlyRealImages) || (vLocalSystemData->vITIPCDig->vHaveRealImage))
			if (vLocalSystemData->vLastContainerImaged)
			//if (((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode)) || //if bulk mode, must be dark enough
			//	(vLocalSystemData->vLastContainerImaged->vImageBrightness < cImageBrightnessThreshold))
			{
				if (!vLocalSystemData->vRejectImageBuffer[vLocalSystemData->vNextRejectToWrite].vLogThisReject)
				{
					if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason < 255)
						vLocalSystemData->vRejectImageBuffer[vLocalSystemData->vNextRejectToWrite].vContainerEjectInspection = vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason - 1;

					vLocalSystemData->vRejectImageBuffer[vLocalSystemData->vNextRejectToWrite].vEjectorsDisabled = vLocalSystemData->vLastContainerImaged->vToBeEjectedButDisabled;

					vLocalSystemData->vRejectImageBuffer[vLocalSystemData->vNextRejectToWrite].vTime = CTime::GetCurrentTime();

					if (vLocalSystemData->vRejectImageBuffer[vLocalSystemData->vNextRejectToWrite].vGreyImage)
						free(vLocalSystemData->vRejectImageBuffer[vLocalSystemData->vNextRejectToWrite].vGreyImage);

					if (ThereIsEnoughMemory(vLocalSystemData->vITIPCDig->vOriginalBufferSize, "Image to Write To Disk Buffer"))
					{
						vLocalSystemData->vRejectImageBuffer[vLocalSystemData->vNextRejectToWrite].vGreyImage = (BYTE *)malloc(vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					}

					if (vLocalSystemData->vRejectImageBuffer[vLocalSystemData->vNextRejectToWrite].vGreyImage)
					if (vLocalSystemData->vLastContainerImaged)
					if (vLocalSystemData->vLastContainerImaged->vTheImage)
					if (vLocalSystemData->vLastContainerImaged->vTheImage->vGreyImage)
					{
						CopyMemory(vLocalSystemData->vRejectImageBuffer[vLocalSystemData->vNextRejectToWrite].vGreyImage, vLocalSystemData->vLastContainerImaged->vTheImage->vGreyImage, 
							vLocalSystemData->vITIPCDig->vOriginalBufferSize);

						vLocalSystemData->vRejectImageBuffer[vLocalSystemData->vNextRejectToWrite].vRedBoxCornerX = vLocalSystemData->vITIPCDig->vLastRedBoxCornerX[0];

						vLocalSystemData->vRejectImageBuffer[vLocalSystemData->vNextRejectToWrite].vRedBoxCornerY = vLocalSystemData->vITIPCDig->vLastRedBoxCornerY[0] + vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust;

						vLocalSystemData->vRejectImageBuffer[vLocalSystemData->vNextRejectToWrite].vRedBoxWidth = vLocalSystemData->vITIPCDig->vLastRedBoxWidth[0];

						vLocalSystemData->vRejectImageBuffer[vLocalSystemData->vNextRejectToWrite].vRedBoxHeight = vLocalSystemData->vITIPCDig->vLastRedBoxHeight[0];

						vLocalSystemData->vRejectImageBuffer[vLocalSystemData->vNextRejectToWrite].vLogThisReject = true;

						vLocalSystemData->vNextRejectToWrite++;
						if (vLocalSystemData->vNextRejectToWrite >= cNumberOfRejectImageBuffers)
							vLocalSystemData->vNextRejectToWrite = 0;
					}
				}
				else
						ReportErrorMessage("Can not save Image: " + dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber, 0) + " to File as all buffers are full", cEMailInspx, 32000);
			}

			//save reject for display and tell Main Menu have a new image to display
			if ((!vLocalSystemData->vITIPCDig->vShowOnlyRealImages) || (vLocalSystemData->vITIPCDig->vHaveRealImage) || (vLocalSystemData->vITIPCDig->vProcessNextImage))
			if (((vLocalSystemData->vTakingQuickLearnImages) && 
				(vLocalSystemData->vQuickLearnImageIndex >= vLocalConfigurationData->vNumberOfQuickLearnImagesToTake)) || //end of quick learn, so must show that image
				(!vLocalSystemData->vITIPCDig->vImageDisplayOnlyRejects) ||
				(!vLocalSystemData->vLastContainerImaged) ||
				(vLocalSystemData->vLastContainerImaged->vToBeEjected) ||
				((vLocalSystemData->vLastContainerImaged->vTheImage) && (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason >= 0)) ||
				(vLocalSystemData->vITIPCDig->vProcessNextImage) ||
				(vLocalSystemData->vSystemRunMode == cTestShutterMode) ||
				(vLocalSystemData->vSystemRunMode == cCalibratingDetectorEdgesMode) ||
				(vLocalSystemData->vSystemRunMode == cCalibratingDetectorGainMode) ||
				(vLocalSystemData->vSystemRunMode == cAlignTubeMode) ||
				//(vLocalSystemData->vSystemRunMode == cCalibratingDetectorOffsetMode) ||
				(vLocalSystemData->vSystemRunMode == cMeasureSourceStrengthMode))
			if ((vLocalSystemData->vSystemRunMode != cCalibratingDetectorOffsetMode) ||
				((PasswordOK(cTemporaryInspxPassword,false)) && (!vLocalSystemData->vAutoCalibrate) &&
				(!vLocalSystemData->vAutoCalibrateWhileRunning) &&
				(!vLocalSystemData->vCalibrateADCAfterDAC))) 
				//don't show calibrating offset images unless Inspx password
			//if (((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode)) || //if bulk mode, must be dark enough
			//	(vLocalSystemData->vITIPCDig->vLastOriginalImageIntensity < cImageBrightnessThreshold))
			if (vLocalSystemData->vLastContainerImaged)
			if (vLocalSystemData->vLastContainerImaged->vTheImage)
			{
				if ((vLocalSystemData->vLastContainerImaged->vToBeEjected) || (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason < cMaximumRejectReason))
				if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem) || (vLoadedThisImageFromFile))
				//Save image as reject if rejected
					vLocalSystemData->vITIPCDig->SaveThisRejectInBuffer(vLocalSystemData->vLastContainerImaged, vLocalSystemData->vLastContainerImaged->vContainerNumber, 
						TempInspectionTopReferenceAdjust, vLocalSystemData->vCountToUseForEjectReason[vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason]);

				//tell main menu there is a new image to display
				if (((!vLocalSystemData->vPostedNewMainImageReadyToDisplayMessage) || (vLocalSystemData->vSystemRunMode == cAlignTubeMode) || (vLoadedThisImageFromFile)) && (vLocalSystemData->vShowImages)) 
					//show all offset calibrations
				if ((!vLocalSystemData->vLoadedFileAvailable) || (vLoadedThisImageFromFile))
				if (vLocalSystemData->vITIPCDig->vNextImageToDisplay)
				if (((vLocalSystemData->vTakingQuickLearnImages) && 
					(vLocalSystemData->vQuickLearnImageIndex >= vLocalConfigurationData->vNumberOfQuickLearnImagesToTake)) ||//end of quick learn, so must show that image
					(vLocalSystemData->vSystemRunMode == cAlignTubeMode) || ((!vLocalSystemData->vITIPCDig->vImageDisplayOnlyRejects) ||
					((vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason < cMaximumRejectReason) &&
					(vGlobalCurrentProduct->vTypesOfRejectsToView & (0x0001 << vLocalSystemData->vCountToUseForEjectReason[vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason]))) ||
					(vLocalSystemData->vITIPCDig->vProcessNextImage) || (vLocalSystemData->vSystemRunMode != cRunningSystemMode)))
					//exclude learn and any inspections above 16 from showing when show rejects only
				if ((!vLocalSystemData->vITIPCDig->vImageDisplayOnlyRejects) || (vLocalSystemData->vCountToUseForEjectReason[vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason] < 16))
				if (((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode) && 
					(!vLocalSystemData->vMultiLaneInSetupLanePositionsMenu)) || //if bulk mode, must be dark enough
					((vLocalSystemData->vLastContainerImaged->vImageBrightness < cImageBrightnessThreshold) || (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)))  //vImageBrightness is average over whole image so image must not be white to be shown
				{
					if (vLocalSystemData->vITIPCDig->vNextImageToDisplayLock.Lock())
					{
						if (vLocalSystemData->vLastContainerImaged)
							vLocalSystemData->vITIPCDig->FillImageData(vLocalSystemData->vITIPCDig->vNextImageToDisplay, vLocalSystemData->vLastContainerImaged->vContainerNumber, vLocalSystemData->vLastContainerImaged);
						else
							vLocalSystemData->vITIPCDig->FillImageData(vLocalSystemData->vITIPCDig->vNextImageToDisplay, vLocalSystemData->vFrameCount, vLocalSystemData->vLastContainerImaged);

						vLocalSystemData->vITIPCDig->vNextImageToDisplayLock.Unlock();

										//b rh test code
										//WriteImageToHardDisk(vLocalSystemData->vITIPCDig->vNextImageToDisplay->vGreyImage, "C:\\ScanTracSerialLogs\\ImageForMainMenu" + dtoa(vLocalSystemData->vSeqNum, 0) + ".bmp", 
										//	vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);


						//if (vLocalSystemData->vITIPCDig->vNextImageToDisplay->vFinalImageIntensity < 6.0)
						//if (vLocalConfigurationData->vScanTracType != cSoloScanTrac)
						//{
						//	//vLocalSystemData->vITIPCDig->vNextImageToDisplay->vFinalImageIntensity = vLocalSystemData->vITIPCDig->CalculateFullDensity(
						//	//	vLocalSystemData->vITIPCDig->vNextImageToDisplay->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize); 
						//	vLocalSystemData->vITIPCDig->vNextImageToDisplay->vFinalImageIntensity = vLocalSystemData->vITIPCDig->CalculateFullDensity(
						//		vLocalSystemData->vITIPCDig->vLatestImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize); 
						//	
						//	vLocalSystemData->vLogFile.WriteToLogFile("Final Brightness: " + dtoa(vLocalSystemData->vITIPCDig->vNextImageToDisplay->vFinalImageIntensity,2),cDebugMessage);
						//}

						if (vLocalSystemData->vLastContainerImaged)
							vLocalSystemData->vLogFile.WriteToLogFile("IAT-Filled Image Data: " + dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0),cDebugMessage);
						else
							vLocalSystemData->vLogFile.WriteToLogFile("IAT-Filled Image Data FCS: " + vLocalSystemData->vFrameCountString,cDebugMessage);
					}
						/*
					}
					else
					{
						if (vLocalSystemData->vLogFile.vLogSerialData)
							vLocalSystemData->vLogFile.WriteToLogFile("3IAT-Pre vNextImageToDisplayLock.Lock",cDebugMessage);
						if (vLocalSystemData->vITIPCDig->vNextImageToDisplayLock.Lock())
						{
							if (vLocalSystemData->vLogFile.vLogSerialData)
								vLocalSystemData->vLogFile.WriteToLogFile("4IAT-Post vNextImageToDisplayLock.Lock",cDebugMessage);
							if (vLocalSystemData->vLastContainerImaged)
								vLocalSystemData->vITIPCDig->FillImageData(
									vLocalSystemData->vITIPCDig->vNextImageToDisplay, 
									vLocalSystemData->vLastContainerImaged->vContainerNumber);
							else
								vLocalSystemData->vITIPCDig->FillImageData(
									vLocalSystemData->vITIPCDig->vNextImageToDisplay, 
									vLocalSystemData->vFrameCount);

							if (vLocalSystemData->vLogFile.vLogSerialData)
								vLocalSystemData->vLogFile.WriteToLogFile("5IAT-Pre vNextImageToDisplayLock.Unlock",cDebugMessage);
							vLocalSystemData->vITIPCDig->vNextImageToDisplayLock.Unlock();
							if (vLocalSystemData->vLogFile.vLogSerialData)
								vLocalSystemData->vLogFile.WriteToLogFile("6IAT-Post vNextImageToDisplayLock.Unlock",cDebugMessage);

							if (vLocalSystemData->vLogFile.vLogSerialData)
							{
								if (vLocalSystemData->vLastContainerImaged)
									vLocalSystemData->vLogFile.WriteToLogFile("IAT-Filled Image Data With Lock: " + 
										dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0),cDebugMessage);
								else
									vLocalSystemData->vLogFile.WriteToLogFile("IAT-Filled Image Data With Lock: " + 
											vLocalSystemData->vFrameCountString,cDebugMessage);
							}
						}
					}
					*/

					if (vLoadedThisImageFromFile)
					{
						vLocalSystemData->vLoadedFileAvailable = false;
						if (vLocalSystemData->vLoadedFileImage)
							im_delete(vLocalSystemData->vLoadedFileImage);
						vLocalSystemData->vLoadedFileImage = NULL;
					}
					//if (vLocalSystemData->vLogFile.vLogSerialData)
					//	vLocalSystemData->vLogFile.WriteToLogFile("60IAT-Post Message cNewMainImageReadyToDisplay",cDebugMessage);

					vLocalSystemData->vPostedNewMainImageReadyToDisplayMessage = true;

					//posting message hangs so can not do during quick learn processing
					//if (!vGlobalProcessQuickLearnImagesThread)
					//::PostThreadMessage(vGlobalCallingThreadID,
					//	cNewMainImageReadyToDisplay, vLocalSystemData->vSeqNum, 
					//	vLocalSystemData->vFrameCount);
					vLocalSystemData->vITIPCDig->vProcessNextImage = false;
					
					//if (vLocalSystemData->vLogFile.vLogSerialData)
					//	vLocalSystemData->vLogFile.WriteToLogFile("61IAT-After Post Message cNewMainImageReadyToDisplay",cDebugMessage);
				}

				//give reject hidden indicator
				if (vLocalSystemData->vLastContainerImaged)
				if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
				if (vLocalSystemData->vITIPCDig->vImageDisplayOnlyRejects)
				if ((vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason <= vGlobalCurrentProduct->vNumberOfInspections) &&
					(!(vGlobalCurrentProduct->vTypesOfRejectsToView & (0x0001 << vLocalSystemData->vCountToUseForEjectReason[vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason]))))
				if (((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode)) || //if bulk mode, must be dark enough
					(vCurrentImageBrightnessOK))
				if (vLocalConfigurationData->vSlippedIndicatorTime)
				if (!vGlobalProcessQuickLearnImagesThread)
					::PostThreadMessage(vGlobalCallingThreadID, cDisplayRejectHiddenMessage, vLocalSystemData->vCountToUseForEjectReason[vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason], vLocalSystemData->vFrameCount);
			}
			
			if (vLocalSystemData->vLastContainerImaged)
			if ((vLocalSystemData->vLastContainerImaged->vContainerNumber % 100) == 0)  //every 100 container see if should improve learn
			if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
			{
				::PostThreadMessage(vGlobalCallingThreadID, cDensityLoopLockedMessage, 1,1);
				::PostThreadMessage(vGlobalCallingThreadID, cDensityLoopLockedClearMessage, 1,1);
			}

			vLocalSystemData->vLastContainerImaged = NULL;
		}
		vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
	}
	else
	{
		vLocalSystemData->vProcessingErrorCount++;
		ReportErrorMessage("Error-Lock failed, Did not get container for image",cWriteToLog,0);
	}
	if (!vLocalSystemData->vITIPCDig->vShowOnlyRealImages) //if simulating images, pretend got a Filler Index Pulse if using FIFO in sampling and number of containers is number of valves 
	if (vLocalSystemData->vUseSamplingFIFO)
	if (vGlobaluCSerialPort)
	{
		if (vLocalSystemData->vFrameCount % vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] == 0)  
			vGlobaluCSerialPort->HandleFillerIndexPulse();
		if (vLocalSystemData->vFrameCount % vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType] == 0)  
			vGlobaluCSerialPort->HandleSeamerIndexPulse();
	}

	vMyLock = false;
	return 0;
}

void CImageAquisitionThread::TestEjectThisContainer()
{
	//if (vGlobalCurrentProduct->vInspection[0])
	if (vLocalSystemData->vLastContainerImaged)
	{
		if (vLocalSystemData->vLastContainerImaged)
			vLocalSystemData->vLastContainerImaged->vImageAnalyzed = true;

		BYTE TempSystemRunMode = vLocalSystemData->vSystemRunMode;
		vLocalSystemData->vSystemRunMode = cRunningSystemMode;

		if (!vLocalConfigurationData->vEnableEjectors)
		{
			vMainWindowPointer->EnableDisableEjectors(true);

			BYTE TempEjector = vLocalSystemData->vTestEjectNextContainer;
			double TempDistance = vGlobalCurrentProduct->vEjectorDelayPosition[TempEjector - 1] * vGlobalPixelsPerUnit;
			double TempDelay = TempDistance / vLocalSystemData->vActualEncoderRate;
			TempDelay = (TempDelay + 1) * 1000;  //add 1 scond, then convert to milliSeconds
			::PostThreadMessage(vGlobalCallingThreadID,cDisableEjectorsMessage,(long)TempDelay,(long)TempDelay);//send message to Disable ejectors in X seconds
		}

		//if evaluating, don't eject this container if already ejected
		if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
			vGlobaluCSerialPort->EjectContainer(cTestEject, vLocalSystemData->vLastContainerImaged,NULL);

		vLocalSystemData->vSystemRunMode = TempSystemRunMode;
		if (!vLocalSystemData->vEject4InARowCount)
			vLocalSystemData->vTestEjectNextContainer = 0;

		ReportErrorMessage("Test Eject Container: " + vLocalSystemData->vContainerString, cUserAction,0);
	}
}

bool CImageAquisitionThread::SampleThisContainer(CContainer *TempContainer)
{
	if ((vLocalConfigurationData->vSimulateLightControlBoard) && (!vLocalSystemData->vUseSamplingFIFO)) //simulating when not in FIFO mode
	{
		vLocalSystemData->vLastIndexPulseWasCorrect[cSeamerSamplingType] = true;
		
		vLocalSystemData->vNextHeadToProcess[cSeamerSamplingType]++;
		if (vLocalSystemData->vNextHeadToProcess[cSeamerSamplingType] >= vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType])
			vLocalSystemData->vNextHeadToProcess[cSeamerSamplingType] = 0; //set to zero as took full set, and when get an index pulse, should be at zero

		TempContainer->vHeadNumber[cSeamerSamplingType] = vLocalSystemData->vNextHeadToProcess[cSeamerSamplingType];

		vLocalSystemData->vLastIndexPulseWasCorrect[cFillerSamplingType] = true;
		
		if (vLocalSystemData->vSamplingIndexErrorData[cSeamerSamplingType])
		{
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("*IAT-Valve Monitor Increment to Next Head: " + dtoa(vLocalSystemData->vNextHeadToProcess[cSeamerSamplingType], 0),cDebugMessage);
			if (vLocalSystemData->vNextHeadToProcess[cSeamerSamplingType] >= vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType])
			{
				vLocalSystemData->vLastIndexPulseWasCorrect[cSeamerSamplingType] = true;

				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("*IAT-Seamer Monitor Simulate End Of Valve Group: " + dtoa(vLocalSystemData->vNumberValveRowDataHave, 0),cDebugMessage);
				//got the Index pulse, so reset the counter
				if (vLocalSystemData->vRecievedFirstIndexPulse[cSeamerSamplingType])
				{
					//do error checking to be sure index pulse is in correct place to be sure you got entire set of containers (one from each valve)
					int *TempPointerToValveError = vLocalSystemData->vSamplingIndexErrorData[cSeamerSamplingType] + vLocalSystemData->vSamplingIndexErrorDataIndex[cSeamerSamplingType];
					if (vLocalSystemData->vNextHeadToProcess[cSeamerSamplingType] >= vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType]) //Index should be zero when get the sync pulse as should have written current data, and now point at zeroth position
					{
						vLocalSystemData->vNextHeadToProcess[cSeamerSamplingType] = 0; //set to zero as took full set, and when get an index pulse, should be at zero
						*TempPointerToValveError = 0;
						vLocalSystemData->vSamplingIndexErrorDataIndex[cSeamerSamplingType]++;
						if (vLocalSystemData->vSamplingIndexErrorDataIndex[cSeamerSamplingType] >= cMaximumValveDataRows)
							vLocalSystemData->vSamplingIndexErrorDataIndex[cSeamerSamplingType] = 0;
						if (vLocalSystemData->vSamplingIndexErrorDataHave[cSeamerSamplingType] < cMaximumValveDataRows)
							vLocalSystemData->vSamplingIndexErrorDataHave[cSeamerSamplingType]++;
					}	
				}
			}
		}
		TempContainer->vHeadNumber[cFillerSamplingType] = vLocalSystemData->vNextHeadToProcess[cFillerSamplingType];
	}

	//sample this container?
	BYTE TempSampleIndexAuxilaryDetector = 0;
	bool TempSampledThisContainer = false;
	if (vLocalSystemData->vLastContainerImaged)
	for (BYTE TempLoop = cRandomSamplingType; TempLoop <= cSeamerSamplingType; TempLoop++)
	if ((TempLoop == cRandomSamplingType) || (vLocalSystemData->vLastIndexPulseWasCorrect[TempLoop]))
	if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)  //already marked for eject
	{
		if (vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSeries[TempLoop])
		{
			if ((TempLoop == cRandomSamplingType) || (vLocalSystemData->vSamplingNowWhichHeadToSample[TempLoop] == vMainWindowPointer->CalculateHeadNumber(((int)TempContainer->vHeadNumber[TempLoop]), TempLoop)))
			if ((vLocalConfigurationData->vStaggerSampling) && (vLocalSystemData->vSamplingSkipNextSample))
			{
				vLocalSystemData->vSamplingSkipNextSample--;
				ReportErrorMessage("Staggered Sampling, Skip this one", cUserAction,0);
			}
			else
			{
				CString TempString = 	vLocalSystemData->vSamplingName[TempLoop];

				if (TempLoop == cRandomSamplingType)
					ReportErrorMessage(TempString + " sample # " + dtoa(vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSeries[TempLoop], 0) + " Eject container: " + 
						dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0), cUserAction,0);
				else
				{
					CString TempText = "valve";
					if (TempLoop == cSeamerSamplingType)
						TempText = "head";

					ReportErrorMessage(TempString + " sample # " + dtoa(vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSeries[TempLoop], 0) + " Eject container: " + 
						dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0) + " in Series, " + TempText + ": " + dtoa(vLocalSystemData->vSamplingNowWhichHeadToSample[TempLoop], 0), cUserAction,0);
				}

				TempSampledThisContainer = true;
				if (TempLoop != cRandomSamplingType)
					vLocalSystemData->vSampledOnLastRevolution[TempLoop] = true;
				vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSeries[TempLoop]--;

				TempSampleIndexAuxilaryDetector = vLocalSystemData->vSamplingAuxiliaryDetector[TempLoop];
				if ((vLocalConfigurationData->vNumberOfHeadsToMonitor[cRandomSamplingType]) || (TempLoop != cRandomSamplingType))  //for sampling from aux detector, just use the Aux Detector ejector, but for sampling through the sample menu, put the sample menu ejector in for the Aux Det
						vGlobalCurrentProduct->vExternalDetectorEjector[TempSampleIndexAuxilaryDetector] = 	vGlobalCurrentProduct->vSamplingEjector[TempLoop];

				TempContainer->vImageAnalyzed = true;
				//TempContainer->vRejectReason = Sample

				vLocalSystemData->vSamplesEjectedCount[TempLoop]++;

				vLocalSystemData->vSamplingNowWhichHeadToSample[TempLoop]++;
				if (vLocalSystemData->vSamplingNowWhichHeadToSample[TempLoop] > vLocalConfigurationData->vNumberOfHeadsToMonitor[TempLoop])
					vLocalSystemData->vSamplingNowWhichHeadToSample[TempLoop] = 1;

				if (vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSeries[TempLoop]) //if more samples to take
				if (vLocalConfigurationData->vStaggerSampling)
				{
					if (TempLoop == cRandomSamplingType)
						vLocalSystemData->vSamplingSkipNextSample = 9; //wait 10 containers to eject next random one
					else
						vLocalSystemData->vSamplingSkipNextSample = 1; //need to skip the next valve so get it on the next revolution
				}
			}
		}
		else
		if (vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSameHead[TempLoop])
 		{
			if (vLocalSystemData->vSamplingNowWhichHeadToSample[TempLoop] == vMainWindowPointer->CalculateHeadNumber(((int)TempContainer->vHeadNumber[TempLoop]), TempLoop))
			{
				CString TempString = vLocalSystemData->vSamplingName[TempLoop];

				CString TempText = "valve";
				if (TempLoop == cSeamerSamplingType)
					TempText = "head";

				ReportErrorMessage(TempString + " sample # " + dtoa(vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSameHead[TempLoop], 0) + " Eject Container: " + 
					dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber, 0) + " same " + TempText + ": " + dtoa(vLocalSystemData->vSamplingNowWhichHeadToSample[TempLoop], 0), cUserAction,0);

				TempSampledThisContainer = true;
				vLocalSystemData->vSampledOnLastRevolution[TempLoop] = true;

				vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSameHead[TempLoop]--;
				vLocalSystemData->vSamplesEjectedCount[TempLoop]++;

				TempSampleIndexAuxilaryDetector = vLocalSystemData->vSamplingAuxiliaryDetector[TempLoop];
				vGlobalCurrentProduct->vExternalDetectorEjector[TempSampleIndexAuxilaryDetector] = vGlobalCurrentProduct->vSamplingEjector[TempLoop];
			}
		}
	}

	if (TempSampledThisContainer)
	{
		if (vLocalSystemData->vValveMonitorInspection < 30)
		if (vLocalSystemData->vValveData)
		if ((vLocalSystemData->vValveMonitorInspection < vGlobalCurrentProduct->vNumberOfInspections) && (vGlobalCurrentProduct) && (vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection]))
		{
			vLocalSystemData->vLastContainerImaged->vToBeEjected = true;
			switch (vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection]->vInspectionType)
			{
				case cCheckWeighInspection:
				case cUnderfillByWeightInspection:
				case cOverfillByWeightInspection:
					CheckUnderfillByWeight(vLocalSystemData->vLastContainerImaged, vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection], vLocalSystemData->vValveMonitorInspection);
				break;
				case cDensityInspection:
				case cUnderfillInspection:
				case cOverfillInspection:
				{
					CheckDensity(vLocalSystemData->vLastContainerImaged, vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection], vLocalSystemData->vValveMonitorInspection);
				}
			}
		}

		if (vLocalSystemData->vLastContainerImaged)
			vLocalSystemData->vLastContainerImaged->vImageAnalyzed = true;

		BYTE TempSystemRunMode = vLocalSystemData->vSystemRunMode;
		vLocalSystemData->vSystemRunMode = cRunningSystemMode;

		if (!vLocalConfigurationData->vEnableEjectors)
			//ejector numbers are in 1 to 32, so must subtract one
		if (vGlobalCurrentProduct->vEjectorDwellTime[vGlobalCurrentProduct->vExternalDetectorEjector[TempSampleIndexAuxilaryDetector] - 1] > 0) //don't enable ejectors if using a diverter as ejector disable does not effect diverters
		{
			vMainWindowPointer->EnableDisableEjectors(true);
			vLocalSystemData->vNeedToDisableEjectorsAfterSampleAll = true;
			ReportErrorMessage("Enable Ejectors to Sample containers", cAction,0);
		}

		vGlobaluCSerialPort->EjectContainer(TempSampleIndexAuxilaryDetector + cEjectAuxilaryDetectorOffset, vLocalSystemData->vLastContainerImaged,NULL);

		vLocalSystemData->vSystemRunMode = TempSystemRunMode;
		if (!vLocalSystemData->vEject4InARowCount)
			vLocalSystemData->vTestEjectNextContainer = 0;

		ReportErrorMessage("Sample Container: " + dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0) + " Ejector: " + 
			vLocalConfigurationData->GetEjectorNames(vGlobalCurrentProduct->vExternalDetectorEjector[TempSampleIndexAuxilaryDetector]), cWriteToLog,0);

		if (vLocalSystemData->vNeedToDisableEjectorsAfterSampleAll)
		{
			BOOL TempHaveMoreToSample = false;
			for (BYTE TempLoop = 0; TempLoop < 3; TempLoop++)
			{
				if (vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSameHead[TempLoop]) //if all done sampling then setup to disable ejector after timer
					TempHaveMoreToSample = true;
				if (vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSeries[TempLoop])
					TempHaveMoreToSample = true;
			}

			if (!TempHaveMoreToSample)
			{
				vLocalSystemData->vNeedToDisableEjectorsAfterSampleAll = false;
				//setup to disable ejectors after eject this sample
				BYTE TempEjector = vLocalSystemData->vTestEjectNextContainer;
				double TempDistance = vGlobalCurrentProduct->vEjectorDelayPosition[TempEjector - 1] * vGlobalPixelsPerUnit;
				double TempDelay = TempDistance / vLocalSystemData->vActualEncoderRate;
				TempDelay = (TempDelay + 1) * 1000;  //add 1 scond, then convert to milliSeconds
				::PostThreadMessage(vGlobalCallingThreadID,cDisableEjectorsMessage,(long)TempDelay,(long)TempDelay);//send message to Disable ejectors in X seconds
				ReportErrorMessage("Disable Ejectors after Sampling containers", cAction,0);
			}
		}
	}
	else
	{ //don't sample this container, so de-activate diverter if is active from sampling previous container
		for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
		if (vLocalSystemData->vDiverterActive[TempLoop])
		{
			vLocalSystemData->vDiverterActive[TempLoop] = false;
			vLocalSystemData->vLastContainerImaged->vEjectPosition[TempLoop] = BPAdd(vGlobalCurrentProduct->vEjectorDelayBeltPosition[TempLoop], vLocalSystemData->vLastContainerImaged->vBodyTriggerPosition);
				
			if (vGlobalCurrentProduct->vEjectorResponseTime[TempLoop])
			{
				vLocalSystemData->vLastContainerImaged->vEjectPosition[TempLoop] = BPSubtract(vLocalSystemData->vLastContainerImaged->vEjectPosition[TempLoop], (WORD)(vLocalSystemData->vActualEncoderRate * 
					vGlobalCurrentProduct->vEjectorResponseTime[TempLoop]) + vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust);
			}
			else
			{
				vLocalSystemData->vLastContainerImaged->vEjectPosition[TempLoop] = BPSubtract(vLocalSystemData->vLastContainerImaged->vEjectPosition[TempLoop], vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust);
			}
			//use if diverter is level controled or on = divert BYTE TempEjector = vLocalConfigurationData->vEjector[TempLoop].vDigitalInputLine;

			//for pulse driven extend/retract Shuttleworth ejector
			BYTE TempEjector = vLocalConfigurationData->vEjector[TempLoop].vDigitalInputLine + 1;
			vLocalSystemData->vDiverterDeactivatePosition[TempEjector] = vLocalSystemData->vLastContainerImaged->vEjectPosition[TempLoop];
			if (vLocalSystemData->vDiverterDeactivatePosition[TempEjector] == 0)
				vLocalSystemData->vDiverterDeactivatePosition[TempEjector] = 0xFFFF;
			ReportErrorMessage("De-activate SW Diverter: " + dtoa(TempEjector, 0) + " at BP: " + dtoa(vLocalSystemData->vLastContainerImaged->vEjectPosition[TempLoop], 0),cAction,0);

			//TempEjector = TempEjector | 0x40; //turn Diverter Off as done Sampling 
			vGlobaluCSerialPort->SendEjectCommand(TempEjector, vLocalSystemData->vLastContainerImaged->vEjectPosition[TempLoop]);
		}
	}
	return TempSampledThisContainer;
}


int CImageAquisitionThread::CalculateTopReference()
{
	vLocalSystemData->vITIPCDig->vLastTopReferenceAdjust = 0;
	if (vGlobalCurrentProduct->vReferenceTop)
	{
		vLocalSystemData->vITIPCDig->vLastTopReferenceAdjust = vLocalSystemData->vITIPCDig->vLastTop - vGlobalCurrentProduct->vReferenceTop;
	}
	return vLocalSystemData->vITIPCDig->vLastTopReferenceAdjust;
}

int CImageAquisitionThread::CalculateBottomReference()
{
	vLocalSystemData->vITIPCDig->vLastBottomReferenceAdjust = 0;
	if (vGlobalCurrentProduct->vReferenceBottom)
		vLocalSystemData->vITIPCDig->vLastBottomReferenceAdjust = vLocalSystemData->vITIPCDig->vLastBottom - vGlobalCurrentProduct->vReferenceBottom;

	return vLocalSystemData->vITIPCDig->vLastBottomReferenceAdjust;
}

void CImageAquisitionThread::AdjustEdgeDetectorPixels(bool TempCorrectJustVisiblePicture)
{
	//adjacent pixels factor
	if (vLocalConfigurationData->vCorrectDetectorEdges == 2)
	{//correct all pixels
		BYTE *TempBuffer; 
		double TempCheck = 0;
		WORD TempStart = 0;
		WORD TempStop = vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors;
		if (TempCorrectJustVisiblePicture)
		{
			TempStart = vLocalSystemData->vITIPCDig->vAquisitionROIBottom;
			TempStop = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX + vLocalSystemData->vITIPCDig->vAquisitionROIBottom;
		}
		for (WORD TempLoop = TempStart; TempLoop < TempStop; TempLoop++) //loop for each pixel going up the image
		{
			if (vLocalConfigurationData->vDetectorEdgeAdjustGain[TempLoop][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors])
			{
				WORD TempLoopY = 0;
				TempBuffer = vLocalSystemData->vITIPCDig->vLastBufferFilled + TempLoop; 
				for (TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vAquisitionSizeY; TempLoopY++)
				{
					if ((*TempBuffer > 0) && (*TempBuffer < 255)) // only correct if not zero intensity
					{
						TempCheck = (*TempBuffer + vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempLoop][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors]) * 
							vLocalConfigurationData->vDetectorEdgeAdjustGain[TempLoop][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors];

						if (TempCheck >= 255)
							*TempBuffer = 0xFF;
						else
						if (TempCheck < 1)
							*TempBuffer = 0x00;
						else
							*TempBuffer = (BYTE)(TempCheck + .5); //adjusted pixel data
					}
					TempBuffer = TempBuffer + vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors;
				}
			}
		}
	}
	else
	if (vLocalConfigurationData->vCorrectDetectorEdges == 1)
	{//correct only edge pixels
		//adjacent pixels factor
		BYTE *TempBuffer; 
		double TempCheck = 0;
		BYTE TempNumberOfEdgesToCorrect = cNumberOfEdgePixelsToAdjust * 2 * vLocalConfigurationData->vNumberOfXRayDetectors;
		for (WORD TempLoop = 0; TempLoop < TempNumberOfEdgesToCorrect; TempLoop++)
		{
			if (vLocalConfigurationData->vDetectorEdgeAdjustGain[vLocalConfigurationData->vAdjustOffset[TempLoop]][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors] > 0)
			//if ((vLocalSystemData->vITIPCDig->vAquisitionROIBottom <= vLocalConfigurationData->vAdjustOffset[TempLoop]) &&
			//	(vLocalSystemData->vITIPCDig->vAquisitionROITop > vLocalConfigurationData->vAdjustOffset[TempLoop]))
			{
				WORD TempLoopY = 0;
				TempBuffer = vLocalSystemData->vITIPCDig->vLastBufferFilled + vLocalConfigurationData->vAdjustOffset[TempLoop]; 
				for (TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vAquisitionSizeY; TempLoopY++)
				{
					if ((*TempBuffer > 0) && (*TempBuffer < 255)) // only correct if not zero intensity
					{
						TempCheck = *TempBuffer * vLocalConfigurationData->vDetectorEdgeAdjustGain[vLocalConfigurationData->vAdjustOffset[TempLoop]][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors] + 
							vLocalConfigurationData->vDetectorEdgeAdjustOffset[vLocalConfigurationData->vAdjustOffset[TempLoop]][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors];

						if (TempCheck >= 255)
							*TempBuffer = 0xFF;
						else
						if (TempCheck < 1)
							*TempBuffer = 0x00;
						else
							*TempBuffer = (BYTE)TempCheck; //adjusted edge data
					}
					TempBuffer = TempBuffer + vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors;
				}
			}
		}
	}
}

void CImageAquisitionThread::AdjustEdgeDetectorPixels(bool TempCorrectJustVisiblePicture, BYTE TempLaneNumber)
{
	//adjacent pixels factor
	if (vLocalConfigurationData->vCorrectDetectorEdges == 2)
	{//correct all pixels
		BYTE *TempBuffer; 
		double TempCheck = 0;
		WORD TempStart = vGlobalCurrentProduct->vMultiLaneStartPixel[TempLaneNumber];
		//WORD TempStop = vGlobalCurrentProduct->vMultiLaneWidthInPixels[TempLaneNumber];

		for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX; TempLoopX++) //loop for each pixel going up the image
		{
			if (vLocalConfigurationData->vDetectorEdgeAdjustGain[TempLoopX + TempStart][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors]) //if have a calibration factor for this pixel
			{
				WORD TempLoopY = 0;
				TempBuffer = vLocalSystemData->vITIPCDig->vLastBufferFilled + TempLoopX; 
				for (TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vAquisitionSizeY; TempLoopY++)
				{
					if ((*TempBuffer > 0) && (*TempBuffer < 255)) // only correct if not zero intensity
					{
						TempCheck = (*TempBuffer + vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempLoopX + TempStart][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors]) * 
							vLocalConfigurationData->vDetectorEdgeAdjustGain[TempLoopX + TempStart][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors];

						if (TempCheck >= 255)
							*TempBuffer = 0xFF;
						else
						if (TempCheck < 1)
							*TempBuffer = 0x00;
						else
							*TempBuffer = (BYTE)(TempCheck + .5); //adjusted pixel data
					}
					TempBuffer = TempBuffer + vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
				}
			}
		}
	}
	else
	if (vLocalConfigurationData->vCorrectDetectorEdges == 1)
	{//correct only edge pixels
		//adjacent pixels factor
		BYTE *TempBuffer; 
		double TempCheck = 0;
		BYTE TempNumberOfEdgesToCorrect = cNumberOfEdgePixelsToAdjust * 2 * vLocalConfigurationData->vNumberOfXRayDetectors;
		for (WORD TempLoop = 0; TempLoop < TempNumberOfEdgesToCorrect; TempLoop++)
		{
			if (vLocalConfigurationData->vDetectorEdgeAdjustGain[vLocalConfigurationData->vAdjustOffset[TempLoop]][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors] > 0)
			//if ((vLocalSystemData->vITIPCDig->vAquisitionROIBottom <= vLocalConfigurationData->vAdjustOffset[TempLoop]) &&
			//	(vLocalSystemData->vITIPCDig->vAquisitionROITop > vLocalConfigurationData->vAdjustOffset[TempLoop]))
			{
				WORD TempLoopY = 0;
				TempBuffer = vLocalSystemData->vITIPCDig->vLastBufferFilled + vLocalConfigurationData->vAdjustOffset[TempLoop]; 
				for (TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vAquisitionSizeY; TempLoopY++)
				{
					if ((*TempBuffer > 0) && (*TempBuffer < 255)) // only correct if not zero intensity
					{
						TempCheck = *TempBuffer * vLocalConfigurationData->vDetectorEdgeAdjustGain[vLocalConfigurationData->vAdjustOffset[TempLoop]][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors] + 
							vLocalConfigurationData->vDetectorEdgeAdjustOffset[vLocalConfigurationData->vAdjustOffset[TempLoop]][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors];

						if (TempCheck >= 255)
							*TempBuffer = 0xFF;
						else
						if (TempCheck < 1)
							*TempBuffer = 0x00;
						else
							*TempBuffer = (BYTE)TempCheck; //adjusted edge data
					}
					TempBuffer = TempBuffer + vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors;
				}
			}
		}
	}
}

void CImageAquisitionThread::ReplaceDeadPixels()
{
	if (vLocalConfigurationData->vOverWriteBadPixel[0][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors] == 0xAAAB)
	{
		for (WORD TempLoop = 1; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX; TempLoop++)
		if (vLocalConfigurationData->vOverWriteBadPixel[TempLoop][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors] != 0)
		{
			BYTE *TempSourceBuffer = vLocalSystemData->vITIPCDig->vLastBufferFilled + vLocalConfigurationData->vOverWriteBadPixel[TempLoop][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors]; 
			BYTE *TempDestinationBuffer = vLocalSystemData->vITIPCDig->vLastBufferFilled + TempLoop; 
			for (WORD TempLoopPixel = 0; TempLoopPixel < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY; TempLoopPixel++)
			{
				*TempDestinationBuffer = *TempSourceBuffer;
				TempSourceBuffer = TempSourceBuffer + vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
				TempDestinationBuffer = TempDestinationBuffer + vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
			}
		}
	}
}

double CImageAquisitionThread::AdjustImageIntensity(BYTE *TempImage, double TempImageDensity, double TempDensityTarget)
{
	double TempResult = 0;
	if (TempImageDensity > 0)
	if (TempDensityTarget > 0)
	if ((vLocalConfigurationData->vMaxImageIntensityAdjust) || (vLocalConfigurationData->vMaxImageIntensityAdjustBeforeLocked))
	{
		double TempDelta = TempDensityTarget - TempImageDensity;

		int TempDeltaCurrent = 0;
		if (TempDelta < 0)
			TempDeltaCurrent = (int)(TempDelta - .5);
		else
			TempDeltaCurrent = (int)(TempDelta + .5);

		BYTE TempMax = vLocalConfigurationData->vMaxImageIntensityAdjust;
		if (!vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
			TempMax = vLocalConfigurationData->vMaxImageIntensityAdjustBeforeLocked;
		//if learning, allow any amount of adjustment
		//if ((vLocalSystemData->vITIPCDig->vStructureEnabled) || (vLocalSystemData->vITIPCDig->vReferenceEnabled) ||
		//	(vLocalSystemData->vITIPCDig->vAverageEnabled) || (vLocalSystemData->vITIPCDig->vTakingQuickLearnImages))
		//	TempMax = vLocalConfigurationData->vMaxImageIntensityAdjustBeforeLocked;

		if (TempMax)
		if ((TempDeltaCurrent > TempMax) || (TempDeltaCurrent < -TempMax))
		{
			vLocalSystemData->vLogFile.WriteToLogFile("Image Adjust Intensity too much: " + dtoa(TempDeltaCurrent,0) + " Ignored",cDebugMessage);
			TempDeltaCurrent = 0;
			vLocalSystemData->vITIPCDig->vLastImageIntensityTooFarOff = true;
		}
		else
		if (TempDeltaCurrent != 0)
		{
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				vLocalSystemData->vLogFile.WriteToLogFile("Adjust Image Intensity by: " + dtoa(TempDeltaCurrent,0),cDebugMessage);
			}
			TempResult = TempDeltaCurrent;
			int TempInt = 0;
			BYTE *TempBuffer = TempImage; 
			BYTE *TempHistogrmPointer = NULL;
			if ((vLocalConfigurationData->vAdjustAllAreaOfHistogram) && (vLocalSystemData->vITIPCDig->vProductHistogramImage) && (vLocalSystemData->vITIPCDig->vProductHistogramImage->vGreyImage))
				TempHistogrmPointer = vLocalSystemData->vITIPCDig->vProductHistogramImage->vGreyImage;
			//does not need TempHistogramPointer
			for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
			{
				if ((*TempBuffer < 255) || ((TempHistogrmPointer) && (*TempHistogrmPointer == 0)))
				{
					TempInt = *TempBuffer + TempDeltaCurrent;
					if (TempInt > 254)
						*TempBuffer = 255;
					else
					if (TempInt < 1) 
						*TempBuffer = 0;
					else
						*TempBuffer = TempInt;
				}
				TempBuffer++;
				if (TempHistogrmPointer)
					TempHistogrmPointer++;
			}
		}
	}
	return TempResult;
}

/*
void CImageAquisitionThread::FindDemoEdges()
{
	if (vLocalSystemData->vITIPCDig->vContainerThresholdImage)
	{
		if (vLocalSystemData->vLogFile.vLogSerialData)
		{
			vLocalSystemData->vLogFile.WriteToLogFile("Find Edges",cDebugMessage);
		}
		//look for container Right edge
		BYTE *TempPointer;
		for (WORD TempLineLoop = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 
			vLocalSystemData->vITIPCDig->vMaximumImageOverlap - 1; 
			TempLineLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY; TempLineLoop++)
		{
			TempPointer = vLocalSystemData->vITIPCDig->vContainerThresholdImage->pix +
				(TempLineLoop * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
			WORD TempContainerPixelCount = 0;
			for (WORD TempPixelLoop = vLocalSystemData->vITIPCDig->vLastBottom + 8; 
				TempPixelLoop < vLocalSystemData->vITIPCDig->vLastBottom + 32; TempPixelLoop++)
			{
				if (*TempPointer++ == 0)
					TempContainerPixelCount++;
			}
			//if less than 10 of the 32 pixels are dark, must be a background line
			if (TempContainerPixelCount < 10)
			{
				vLocalSystemData->vITIPCDig->vLastRight = TempLineLoop;
				vLocalSystemData->vITIPCDig->vLastRightDouble = TempLineLoop;
				//set loop variable so will exit
				TempLineLoop = TempLineLoop + vLocalSystemData->vITIPCDig->vMaximumImageOverlap;
			}//end found a background line
		}
		if (vLocalSystemData->vITIPCDig->vLastRight == 0)
		{
			vLocalSystemData->vITIPCDig->vLastRight = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY;
			vLocalSystemData->vITIPCDig->vLastRightDouble = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY;
		}
		
		if (vLocalSystemData->vITIPCDig->vLastLeft < 4)
		{
			//look for container Left edge
			BYTE *TempPointer;
			for (int TempLineLoop = vLocalSystemData->vITIPCDig->vMaximumImageOverlap; 
				TempLineLoop >= 0; TempLineLoop--)
			{
				TempPointer = vLocalSystemData->vITIPCDig->vContainerThresholdImage->pix +
					(TempLineLoop * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
				WORD TempContainerPixelCount = 0;
				for (WORD TempPixelLoop = vLocalSystemData->vITIPCDig->vLastBottom + 8; 
					TempPixelLoop < vLocalSystemData->vITIPCDig->vLastBottom + 32; TempPixelLoop++)
				{
					if (*TempPointer++ == 0)
						TempContainerPixelCount++;
				}
				//if less than 10 of the 32 pixels are dark, must be a background line
				if (TempContainerPixelCount < 10)
				{
					vLocalSystemData->vITIPCDig->vLastLeft = TempLineLoop + 1;
					vLocalSystemData->vITIPCDig->vLastLeftDouble = TempLineLoop + 1;
					TempLineLoop = -1;
				}
			}
		}
	}//end of checking for image overlap
}
*/
void CImageAquisitionThread::FindContaminants(CContainer *TempContainer, BYTE TempReferenceLoop, int TempTopReferenceLoop, CInspection *TempInspection, BYTE TempInspectionNumber)
{
	BYTE TempThreshold = TempInspection->vThreshold;

	WORD TempBottom = 0;
	WORD TempLeft = 0;
	WORD TempHeight = 0;
	WORD TempWidth = 0;

	if (TempInspection)
	//don't do inspection if no structure and set to add structure
	if ((!TempInspection->vAddStructure) || (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage)) //don't do inspection if no structure and using
	//don't do if Tom contaminant, and learned and learn state above 3
	//if ((TempInspection->vROIPixelBottom < vGlobalCurrentProduct->vReferenceTop / 2) || (!TempInspection->vAddStructure) ||
	//	(vGlobalLearnState < 4))
	{
		BYTE TempAutoImproveState = vGlobalLearnState;
		if (!TempInspection->vAddStructure)
			TempAutoImproveState = 0;
		else
		if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) || (vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || 
			(vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
			(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)) //Continuous Feed, or a Pipeline or TD
			TempAutoImproveState = 0;
		else
		if (!vLocalConfigurationData->vAutoImproveEnabled)
			TempAutoImproveState = 0;

		vLocalSystemData->vITIPCDig->GetInspectionROIBounds(TempInspection, 
			&TempBottom, &TempLeft, &TempHeight, &TempWidth, 0,//already referenced vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust, 
			vLocalConfigurationData->vReferenceROIsToEdges, TempAutoImproveState);

		if (TempInspection->vFillBackground)
			TempInspection->FillBackground(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

		//fill in light spots if enabled
		if (TempInspection->vFillInLightSpots)
			TempInspection->FillInLightSpots(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, 
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, TempInspection->vFillInLightSpots);

		//if (TempInspection->vFillInLightSpots)
		//	TempContainer->vTheImages->vCurveFitTime = TempContainer->vTheImages->vCurveFitTime +
		//		TempInspection->CurveFitFillInLightSpots(vLocalSystemData->vITIPCDig->vImageProcessingNowCopy->pix,
		//		vLocalSystemData->vITIPCDig->vDisplayBufferHeightInPixels, vLocalSystemData->vITIPCDig->vDisplayBufferWidthInPixels);

		if (TempInspection->vPreSmoothImage)
		{
			double TempStartTime = CIFCOS::GetSystimeMicrosecs();
			vLocalSystemData->vITIPCDig->MedianSmoothImage(vLocalSystemData->vITIPCDig->vLatestImageCopy, TempInspection->vPreSmoothImage, TempLeft, TempWidth, TempBottom, TempHeight);

			double TempEndTime = CIFCOS::GetSystimeMicrosecs();
			vLocalSystemData->vITIPCDig->vLastSmoothTime = vLocalSystemData->vITIPCDig->vLastSmoothTime + TempEndTime - TempStartTime;
		}

		if (TempInspection->vErodeCount)  //is variable to hold erode times count
		if ((vLocalSystemData->vITIPCDig->vIATScratchImage) && (vLocalSystemData->vITIPCDig->vIATScratchImage->pix))
		for (BYTE TempLoop = 0; TempLoop < TempInspection->vErodeCount; TempLoop++)
		{
			TempInspection->ErodeImage(vLocalSystemData->vITIPCDig->vLatestImageCopy, vLocalSystemData->vITIPCDig->vIATScratchImage);
		}

		if (TempInspection->vDilationCount)
		if ((vLocalSystemData->vITIPCDig->vIATScratchImage) && (vLocalSystemData->vITIPCDig->vIATScratchImage->pix))
		for (BYTE TempLoop = 0; TempLoop < TempInspection->vDilationCount; TempLoop++)
		{
			TempInspection->DilateImage(vLocalSystemData->vITIPCDig->vLatestImageCopy, vLocalSystemData->vITIPCDig->vIATScratchImage);
		}

		double TempStartTime = CIFCOS::GetSystimeMicrosecs();

		if (TempInspection->vRemoveJarPunt)
			TempInspection->RemoveJarPunt(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix,
				vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 
				vLocalSystemData->vITIPCDig->vLastLeft - vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust,
				vLocalSystemData->vITIPCDig->vLastRight - vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust, 
				vGlobalCurrentProduct->vSetupHistoryAverageDensity, 0);

		if (TempInspection->vTrimEdgesOfJar)
		if (!TempInspection->vDoContaminant22Algorithm)
		{  //trim from edge of jar, not edge of ROI
			WORD TempLeft = vLocalSystemData->vITIPCDig->vLastLeft;

			WORD TempRight = vLocalSystemData->vITIPCDig->vLastRight;

			if (TempRight >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeY)
				TempRight = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1;
			if (TempLeft >= TempRight)
				TempLeft = 1;

			TempInspection->TrimEdgesOfJarAtROI(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix,
				vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, true,
				vLocalSystemData->vITIPCDig->vLastLeft - vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust,
				vLocalSystemData->vITIPCDig->vLastRight - vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust);
		}

		if (TempInspection->vTakeDerivative)
			TempInspection->TakeLineDerivative(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vIATScratchImage->pix,
				vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vGlobalCurrentProduct->vSetupHistoryAverageDensity, TempInspection->vThreshold);

		if (TempInspection->vBonesInspection)
		{
			TempInspection->FindBonesInspection(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vIATScratchImage,
				vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 
				TempInspection->vROIPixelLeft, TempInspection->vROIPixelRight, TempInspection->vROIPixelBottom, TempInspection->vROIPixelTop);
		}

		if (TempInspection->vTakeDifferenceFromMedianFilter)
		{
			CopyMemory(vLocalSystemData->vITIPCDig->vIATScratchImage->pix, vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

			BYTE TempMedian = TempInspection->vTakeDifferenceFromMedianFilter & 0x3F;
			DoMedianSmooth(vLocalSystemData->vITIPCDig->vIATScratchImage, TempMedian, TempMedian);

			if (TempInspection->vTakeDifferenceFromMedianFilter < 128)
			{
#ifdef UseIFCDLLs
				mvt_subtract(vLocalSystemData->vITIPCDig->vIATScratchImage, vLocalSystemData->vITIPCDig->vLatestImageCopy);
				mvt_invert(vLocalSystemData->vITIPCDig->vLatestImageCopy);
#endif
			}
			else
				CopyMemory(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vIATScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
		}

		if (TempInspection->vTakeDifferenceFromMedian)
		{
			TempInspection->TakeDifferenceFromMedian(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix,  vLocalSystemData->vITIPCDig->vIATScratchImage,
				vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vLocalSystemData->vITIPCDig->vLatestImage->pix, TempInspection->vThreshold);
		}

		if (TempInspection->vFindDarkSpotOnLine)
			TempInspection->FindDarkSpotOnLine(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix,
			vLocalSystemData->vITIPCDig->vIATScratchImage->pix,
			vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 
			TempInspection->vThreshold);

		if (TempInspection->vFindSpotInSubROI)
		{
			//9/23/2009
			//double TempStartTime = CIFCOS::GetSystimeMicrosecs();

			if (TempInspection->vDesensitizeIrregularArea)
			{
				CopyMemory(vLocalSystemData->vITIPCDig->vDesensitizeImage->pix, vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

				TempInspection->MakeVariablityImage( vLocalSystemData->vITIPCDig->vDesensitizeImage, vLocalSystemData->vITIPCDig->vIATScratchImage, 
					vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 4);

				//double TempEndTime = CIFCOS::GetSystimeMicrosecs();
				//vLocalSystemData->vImageToProcess->vTheImage->vMakeIrregularImageTime = 
				//	vLocalSystemData->vImageToProcess->vTheImage->vMakeIrregularImageTime + TempEndTime - TempStartTime;
			}

			TempInspection->FindSpotInSubROI(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vIATScratchImage,
				vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 
				TempInspection->vThreshold, vLocalSystemData->vITIPCDig->vDesensitizeImage, false, TempBottom, TempBottom + TempHeight, TempLeft, TempLeft + TempWidth);
		}

		if (TempInspection->vPerformGlassAlgorithm == 1)
		{
			vLocalSystemData->vITIPCDig->vLastCurveFitTime = vLocalSystemData->vITIPCDig->vLastCurveFitTime + TempInspection->ClipToCurveFit(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix,
				vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);
		}

		if (TempInspection->vPerformGlassAlgorithm == 2)
		if ((vLocalSystemData->vITIPCDig->vIATScratchImage) && (vLocalSystemData->vITIPCDig->vIATScratchImage->pix))
		{
			TempInspection->CalculateLineDerivative(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vIATScratchImage->pix,
				vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vGlobalCurrentProduct->vSetupHistoryAverageDensity);
		}
		double TempEndTime = CIFCOS::GetSystimeMicrosecs();
		vLocalSystemData->vITIPCDig->vLastGlassInspectionTime = vLocalSystemData->vITIPCDig->vLastGlassInspectionTime + TempEndTime - TempStartTime;

		if (TempInspection->vLookForDarkSpots)
		{
			TempInspection->LookForDarkSpotsOnLine(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vIATScratchImage,
				vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, TempLeft, TempLeft + TempWidth, TempBottom, TempBottom + TempHeight, TempInspection->vThreshold);
		}

		if (TempInspection->vDoContaminant22Algorithm)
		{
			BYTE TempThreshold = TempInspection->vThreshold;	//ams 0407 TH scaling begins
			int TempResult22 = TempInspection->InspxContaminant22InspectionB(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix,
				vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY,
				TempThreshold, 0);
		}

		if (TempInspection->vSubtractAverage)
		if (vLocalSystemData->vITIPCDig->vProductAverageImage)
			vLocalSystemData->vITIPCDig->SubtractProductAverageImage(
			vLocalSystemData->vITIPCDig->vLatestImageCopy->pix,
			vLocalSystemData->vITIPCDig->vProductAverageImage);

		//add the product's structure image to this one if enabled
		if ((TempInspection->vAddStructure) && (TempInspection->vSubtractAverage))
		{
			if (vLocalSystemData->vITIPCDig->vProductStructureImage)
				vLocalSystemData->vITIPCDig->AddProductStructureImage(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix);
		}
		else
		if ((TempInspection->vAddStructure) && (!TempInspection->vSubtractAverage))
		{
			if (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage)
				vLocalSystemData->vITIPCDig->AddProductContaminant2StructureImage(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix);
		}

		if (TempInspection->vIPContaminant)
		if (vLocalSystemData->vITIPCDig->vIATScratchImage2)
		if (vLocalSystemData->vITIPCDig->vIATScratchImage2->pix)
		{
			CopyMemory(vLocalSystemData->vITIPCDig->vIATScratchImage2->pix, vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
			ThresholdImage(vLocalSystemData->vITIPCDig->vIATScratchImage2, TempInspection->vIPContaminant); //create a mask of what is below the threshold
			//test WriteImageToHardDisk(vLocalSystemData->vITIPCDig->vIATScratchImage2->pix, "C:\\ScanTracImages\\BrianTestMask.bmp", vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);
		}

		if (TempInspection->vEnhanceMethod)
		{
			double TempStartFindTime = CIFCOS::GetSystimeMicrosecs();
			vLocalSystemData->vITIPCDig->PerformEnhanceOnImage(vLocalSystemData->vITIPCDig->vLatestImageCopy, vLocalSystemData->vITIPCDig->vIATScratchImage, TempInspection->vEnhanceMethod);
			double 	TempEndTime = CIFCOS::GetSystimeMicrosecs();
			vLocalSystemData->vITIPCDig->vLastEnhanceTime = TempEndTime - TempStartFindTime;
		}

		if (TempInspection->vErodeCountAtEnd)  //is variable to hold erode times count
		if ((vLocalSystemData->vITIPCDig->vIATScratchImage) && (vLocalSystemData->vITIPCDig->vIATScratchImage->pix))
		for (BYTE TempLoop = 0; TempLoop < TempInspection->vErodeCountAtEnd; TempLoop++)
		{
			TempInspection->ErodeImage(vLocalSystemData->vITIPCDig->vLatestImageCopy, vLocalSystemData->vITIPCDig->vIATScratchImage);
		}

		if (TempInspection->vDilationCountAtEnd)
		if ((vLocalSystemData->vITIPCDig->vIATScratchImage) && (vLocalSystemData->vITIPCDig->vIATScratchImage->pix))
		for (BYTE TempLoop = 0; TempLoop < TempInspection->vDilationCountAtEnd; TempLoop++)
		{
			TempInspection->DilateImage(vLocalSystemData->vITIPCDig->vLatestImageCopy, vLocalSystemData->vITIPCDig->vIATScratchImage);
		}

		//if (TempInspection->vDoImageFactors)
		//	vLocalSystemData->vITIPCDig->CombineImages( vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, TempInspection->vEnhancedImageFactor, vLocalSystemData->vITIPCDig->vLatestImage->pix, TempInspection->vOriginalImageFactor);
/*
		if (vLocalSystemData->vITIPCDig->vIATScratchImage)
		{
			vLocalSystemData->vITIPCDig->MaskImageWithDialatedImage(
				vLocalSystemData->vITIPCDig->vLatestImageCopy,
				vLocalSystemData->vITIPCDig->vIATScratchImage);
		}
		*/

		//Add Product's Histogram image to this one if enabled
		if ((TempInspection->vAddHistogram) && (vLocalSystemData->vITIPCDig->vProductHistogramImage))
			vLocalSystemData->vITIPCDig->AddProductHistogramImage(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix);

		//evaluate good in find contaminants
		if (TempInspection->vFindGoodIntensity)
		if (vProcessMinimumGood)
		if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
		if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cPassSlippedContainer)//slipped
			vLocalSystemData->vWrongWidthsDuringLearn++;
		else
		if ((vSizeOK) || (vNumberOfWrongSizeInARow > 50))
		if (vLocalSystemData->vLastContainerImaged)
		if (((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode)) || //if not bulk mode, must be dark enough
			(vCurrentImageBrightnessOK))
		{
			CTime TempTime;
			TempTime = CTime::GetCurrentTime();
			int TempLastEvaluateSecond = TempTime.GetSecond();
			if ((vLastEvaluateSecond != TempLastEvaluateSecond) || (TempInspection->vInspectionType == cThresholdContaminantInspection) || (TempInspection->vInspectionType == cBadHeartInspection))
			{
				vLastEvaluateSecond = TempLastEvaluateSecond;
				if (!vSizeOK)
				{
					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("Autosetup, Find Minimum Good, Size Bad", cDebugMessage);
				}

				if (TempInspection->vDoContaminant22Algorithm)
				{
					TempInspection->InspxContaminant22ThresholdCalculationB(vLocalSystemData->vITIPCDig->vLatestImage->pix, 
						vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 0,
						vLocalSystemData->vMinimumGoodIndex + 1);

					if (vLocalSystemData->vMinimumGoodIndex < cStandardDeviationSampleSize - 1)
						vLocalSystemData->vMinimumGoodIndex++;
					else
						TempInspection->vFindGoodIntensity = false;

				}
				else
				{
					//find minimum good for this frame
					BYTE TempMinimumGood = 255;
					BYTE *TempPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix;
					WORD TempStartX = 0;
					WORD TempEndX = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
					WORD TempStartY = 0;
					WORD TempEndY = vLocalSystemData->vITIPCDig->vAquisitionSizeY;
					if ((TempLeft) || (TempInspection->vROIPixelBottom))
					{
						TempStartX = TempInspection->vROIPixelBottom;
						TempEndX = TempInspection->vROIPixelTop;
						TempStartY = TempLeft; //TempInspection->vROIPixelLeft;
						TempEndY = TempLeft + TempWidth; //TempInspection->vROIPixelRight;
					}
					//first time through, initialize data structures
					if (!vLocalSystemData->vMinimumGoodIndex)
					{
						if (vLocalSystemData->vMinimumGoodData)
						{
							free(vLocalSystemData->vMinimumGoodData);
							vLocalSystemData->vMinimumGoodData = NULL;
						}
						if (ThereIsEnoughMemory(cStandardDeviationSampleSize,"Minimum Good Sample Array"))
							vLocalSystemData->vMinimumGoodData = (BYTE*)malloc(cStandardDeviationSampleSize);

						vLocalSystemData->vMinimumGoodStandardDeviation = 0;
						vLocalSystemData->vMinimumGoodAverage = 0;
						vLocalSystemData->vMinimumGoodIntensity = 256;
					}

					BYTE *TempMinimumGoodDataPointer = vLocalSystemData->vMinimumGoodData + vLocalSystemData->vMinimumGoodIndex; //place to store this data point in data collection array
					if (vLocalSystemData->vMinimumGoodData)
					if (vLocalSystemData->vMinimumGoodIndex < cStandardDeviationSampleSize)
					//if (TempInspection->vSize == 1)
					//{//go through each pixel in the ROI and find lowest value
					//	for (WORD TempLoopY = TempStartY; TempLoopY < TempEndY; TempLoopY++)
					//	{
					//		TempPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix + TempStartX + (TempLoopY * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
					//		for (WORD TempLoopX = TempStartX; TempLoopX < TempEndX; TempLoopX++)
					//		{
					//			if (*TempPointer < TempMinimumGood)
					//			if (*TempPointer > 1)  //br ht change to 125
					//				TempMinimumGood = *TempPointer;
					//			TempPointer++;
					//		}
					//	}
					//}
					//else
					{//spec size greater than one, so must search for minimum good value
						BYTE TempTimesToLoop = 8;
						BYTE TempTryValue = 128;
						BYTE TempRange = 128;
						for (BYTE TempLoop = 0; TempLoop < TempTimesToLoop; TempLoop++)
						{
							//to evaluate the Threshold, must do these inspections each time as the threshold is used in the mathematical analysis
							if (TempInspection->vFindDarkSpotOnLine)
							{
								CopyMemory(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vLatestImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

								if (TempInspection->vPreSmoothImage)
								{
									DoMedianSmooth(vLocalSystemData->vITIPCDig->vLatestImageCopy, TempInspection->vPreSmoothImage,TempInspection->vPreSmoothImage);
								}

								TempInspection->FindDarkSpotOnLine(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix,
								vLocalSystemData->vITIPCDig->vIATScratchImage->pix,
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 
								TempTryValue);
							}

							if (TempInspection->vFindSpotInSubROI)
							{
								CopyMemory(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vLatestImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

								if (TempInspection->vPreSmoothImage)
								{
									DoMedianSmooth(vLocalSystemData->vITIPCDig->vLatestImageCopy, TempInspection->vPreSmoothImage, TempInspection->vPreSmoothImage);
								}

								TempInspection->FindSpotInSubROI(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vIATScratchImage,
									vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 
									TempTryValue, vLocalSystemData->vITIPCDig->vDesensitizeImage, false, TempBottom, TempBottom + TempHeight, TempLeft, TempLeft + TempWidth);
							}

							if (TempInspection->vTakeDifferenceFromMedian)  //bones algorithm
							{
								CopyMemory(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vLatestImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

								if (TempInspection->vPreSmoothImage)
									DoMedianSmooth(vLocalSystemData->vITIPCDig->vLatestImageCopy, TempInspection->vPreSmoothImage,TempInspection->vPreSmoothImage);

								TempInspection->TakeDifferenceFromMedian(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix,  vLocalSystemData->vITIPCDig->vIATScratchImage,
									vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vLocalSystemData->vITIPCDig->vLatestImage->pix, TempTryValue);
							}

							// if any blobs at this value then may go lower, if not, then done
							if (TempInspection->ThereAreAnyBlobsAtValue(TempTryValue, vLocalSystemData->vITIPCDig->vLatestImageCopy, TempBottom, TempLeft, TempHeight, TempWidth))
								TempTryValue = TempTryValue - TempRange; //if have a contaminant at this value, then it was too high, so remove that range trial, 

							TempRange = TempRange / 2; //test next bit lower
							TempTryValue = TempTryValue + TempRange; //add that amount to test next bit lower to try
						}
						TempMinimumGood = TempTryValue;
					}
					//br ht
					//if (!vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
						//TempMinimumGood = 150 - (vLocalSystemData->vMinimumGoodIndex % 20);
					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("Autosetup, Find Minimum Good: " + dtoa(TempMinimumGood,0), cDebugMessage);

					if (vLocalSystemData->vMinimumGoodData)
					if (vLocalSystemData->vMinimumGoodIndex < cStandardDeviationSampleSize)
					{
						*TempMinimumGoodDataPointer = TempMinimumGood; //store this images lowest good threshold in data array
						//save the best value for displaying on the screen
						vLocalSystemData->vITIPCDig->vLastMinimumGoodMaximumBad = *TempMinimumGoodDataPointer;
						if (vLocalSystemData->vLogFile.vLogSerialData)
							vLocalSystemData->vLogFile.WriteToLogFile("Final Minimum Good: " + dtoa(*TempMinimumGoodDataPointer,0),	cDebugMessage);

						if (vLocalSystemData->vMinimumGoodIntensity > *TempMinimumGoodDataPointer)
							vLocalSystemData->vMinimumGoodIntensity = *TempMinimumGoodDataPointer;

						if (vLocalSystemData->vMinimumGoodIndex < cStandardDeviationSampleSize - 1)
							vLocalSystemData->vMinimumGoodIndex++;
						else
							TempInspection->vFindGoodIntensity = false;

						if (vLocalSystemData->vMinimumGoodIndex > 3)
						{
							vLocalSystemData->vMinimumGoodStandardDeviation = CalculateStandardDeviation(vLocalSystemData->vMinimumGoodData, vLocalSystemData->vMinimumGoodIndex, &vLocalSystemData->vMinimumGoodAverage);
							if (vLocalSystemData->vMinimumGoodStandardDeviation < .5)
								vLocalSystemData->vMinimumGoodStandardDeviation = .5;

							if (vLocalConfigurationData->vDemoMode)
							{
								vLocalSystemData->vMinimumGoodStandardDeviation = (1.0 +	((vLocalSystemData->vMinimumGoodIndex % 5) / 10.0));
								vLocalSystemData->vMinimumGoodAverage = (145.0 + ((vLocalSystemData->vMinimumGoodIndex % 10) / 10.0));
							}
							vLocalSystemData->CalculateQualityFactor();
						}
					}
				}
			}
		}
		//evaluate bad
		if (TempInspection->vFindBadIntensity)
		if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
		if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cPassSlippedContainer)//slipped
			vLocalSystemData->vWrongWidthsDuringLearn++;
		else
		if ((vSizeOK) || (vNumberOfWrongSizeInARow > 50))
		if (vLocalSystemData->vLastContainerImaged)
		if (((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode)) || //if bulk mode, must be dark enough
			(vCurrentImageBrightnessOK))
		{
			CTime TempTime;
			TempTime = CTime::GetCurrentTime();
			int TempLastEvaluateSecond = TempTime.GetSecond();
			if ((vLastEvaluateSecond != TempLastEvaluateSecond) || (TempInspection->vInspectionType == cThresholdContaminantInspection) || (TempInspection->vInspectionType == cBadHeartInspection))
			{
				vLastEvaluateSecond = TempLastEvaluateSecond;
				//find minimum bad for this frame
				BYTE TempMaximumBad = 255;
				BYTE *TempPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix;
				WORD TempStartX = 0;
				WORD TempEndX = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
				WORD TempStartY = 0;
				WORD TempEndY = vLocalSystemData->vITIPCDig->vAquisitionSizeY;
				if ((TempLeft) || (TempInspection->vROIPixelBottom))
				{
					TempStartX = TempInspection->vROIPixelBottom;
					TempEndX = TempInspection->vROIPixelTop;
					TempStartY = TempLeft; //TempInspection->vROIPixelLeft;
					TempEndY = TempLeft + TempWidth; //TempInspection->vROIPixelRight;
				}

				if ((!vLocalSystemData->vSendingGoodWithBad) || (TempMaximumBad < vLocalSystemData->vMinimumGoodIntensity))
				{  // if the bad intensity is below the good level
					//first time through, initialize data structures
					if (!vLocalSystemData->vMaximumBadIndex)
					{
						vLocalSystemData->vMaximumBadCount = 0;
						if (vLocalSystemData->vMaximumBadData)
						{
							free(vLocalSystemData->vMaximumBadData);
							vLocalSystemData->vMaximumBadData = NULL;
						}
						if (ThereIsEnoughMemory(cStandardDeviationSampleSize,"Maximum Bad Sample Array"))
							vLocalSystemData->vMaximumBadData = (BYTE*)malloc(cStandardDeviationSampleSize);
						vLocalSystemData->vMaximumBadStandardDeviation = 0;
						vLocalSystemData->vMaximumBadAverage = 0;
						vLocalSystemData->vMaximumBadIntensity = 0;
						vLocalSystemData->vQualityFactor = 0;
					}

					BYTE *TempMaximumBadDataPointer = vLocalSystemData->vMaximumBadData + vLocalSystemData->vMaximumBadIndex;
					if (vLocalSystemData->vMaximumBadData)
					{
						//if (TempInspection->vSize == 1)
						//{
						//	for (WORD TempLoopY = TempStartY; TempLoopY < TempEndY; TempLoopY++)
						//	{
						//		TempPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix + TempStartX + (TempLoopY * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
						//		for (WORD TempLoopX = TempStartX; TempLoopX < TempEndX; TempLoopX++)
						//		{
						//			if (*TempPointer < TempMaximumBad)  //find the smallest value in the processed image
						//				TempMaximumBad = *TempPointer;
						//			TempPointer++;
						//		}
						//	}
						//}
						//else
						{//spec size greater than one, so must search for maximum bad value
							BYTE TempTimesToLoop = 8;
							BYTE TempTryValue = 128;
							BYTE TempRange = 128;
							for (BYTE TempLoop = 0; TempLoop < TempTimesToLoop; TempLoop++)
							{
								//to evaluate the Threshold, must do these inspections each time as the threshold is used in the mathematical analysis
								if (TempInspection->vFindDarkSpotOnLine)
								{
									CopyMemory(
										vLocalSystemData->vITIPCDig->vLatestImageCopy->pix,
										vLocalSystemData->vITIPCDig->vLatestImage->pix,
										vLocalSystemData->vITIPCDig->vOriginalBufferSize);

									if (TempInspection->vPreSmoothImage)
									{
										DoMedianSmooth(vLocalSystemData->vITIPCDig->vLatestImageCopy, TempInspection->vPreSmoothImage,TempInspection->vPreSmoothImage);
									}

									TempInspection->FindDarkSpotOnLine(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix,
									vLocalSystemData->vITIPCDig->vIATScratchImage->pix,
									vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 
									TempTryValue);
								}

								if (TempInspection->vFindSpotInSubROI)
								{
									CopyMemory(
										vLocalSystemData->vITIPCDig->vLatestImageCopy->pix,
										vLocalSystemData->vITIPCDig->vLatestImage->pix,
										vLocalSystemData->vITIPCDig->vOriginalBufferSize);

									if (TempInspection->vPreSmoothImage)
									{
										DoMedianSmooth(vLocalSystemData->vITIPCDig->vLatestImageCopy, TempInspection->vPreSmoothImage,TempInspection->vPreSmoothImage);
									}

									TempInspection->FindSpotInSubROI(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vIATScratchImage,
										vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 
										TempTryValue, vLocalSystemData->vITIPCDig->vDesensitizeImage, false, TempBottom, TempBottom + TempHeight, TempLeft, TempLeft + TempWidth);
								}

								if (TempInspection->vTakeDifferenceFromMedian)//bones algorithm
								{
									CopyMemory(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vLatestImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

									if (TempInspection->vPreSmoothImage)
										DoMedianSmooth(vLocalSystemData->vITIPCDig->vLatestImageCopy, TempInspection->vPreSmoothImage,TempInspection->vPreSmoothImage);

									TempInspection->TakeDifferenceFromMedian(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix,  vLocalSystemData->vITIPCDig->vIATScratchImage,
										vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vLocalSystemData->vITIPCDig->vLatestImage->pix, TempTryValue);
								}

								if (TempInspection->ThereAreAnyBlobsAtValue(TempTryValue, vLocalSystemData->vITIPCDig->vLatestImageCopy, TempBottom, TempLeft, TempHeight, TempWidth))
									TempTryValue = TempTryValue - TempRange; //if have a contaminant at this value, then it was too high, so remove that range trial, 

								TempRange = TempRange / 2; //test next bit lower
								TempTryValue = TempTryValue + TempRange; //add that amount to test next bit lower to try
							}
							TempMaximumBad = TempTryValue;
						}
						if (vLocalSystemData->vLogFile.vLogSerialData)
							vLocalSystemData->vLogFile.WriteToLogFile("Autosetup, Find Maximum Bad: " + dtoa(TempMaximumBad,0) + ", Reference Loop: " + dtoa(TempReferenceLoop,2), cDebugMessage);
					}

					if (vLocalSystemData->vMaximumBadData)
					{
						*TempMaximumBadDataPointer = TempMaximumBad;
						//save the best value for displaying on the screen
						vLocalSystemData->vITIPCDig->vLastMinimumGoodMaximumBad = *TempMaximumBadDataPointer;

						if (vLocalSystemData->vMaximumBadIntensity < *TempMaximumBadDataPointer)
							vLocalSystemData->vMaximumBadIntensity = *TempMaximumBadDataPointer;

						if (vLocalSystemData->vMaximumBadIndex < cStandardDeviationSampleSize - 1)
						{
							vLocalSystemData->vMaximumBadIndex++;
							vLocalSystemData->vMaximumBadCount++;
						}
						else
							TempInspection->vFindBadIntensity = false;

						if (vLocalSystemData->vMaximumBadIndex > 3)
						{
							vLocalSystemData->vMaximumBadStandardDeviation = CalculateStandardDeviation(vLocalSystemData->vMaximumBadData, vLocalSystemData->vMaximumBadIndex, &vLocalSystemData->vMaximumBadAverage);
							if (!vLocalSystemData->vMaximumBadStandardDeviation)
								vLocalSystemData->vMaximumBadStandardDeviation = .5;

							if (!vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
							if (vLocalConfigurationData->vDemoMode)
							{
								vLocalSystemData->vMaximumBadStandardDeviation = (float)(1.0 +	((vLocalSystemData->vMaximumBadIndex % 5) / 10.0));
								vLocalSystemData->vMaximumBadAverage = (float)(7.0 + ((vLocalSystemData->vMaximumBadIndex % 10) / 10.0));
							}
							vLocalSystemData->CalculateQualityFactor();
						}
					}
				}  //end if (TempMaximumBad < vLocalSystemData->vMinimumGoodIntensity)
			}
		} //end evaluate bad containers

		if (vLocalConfigurationData->vDemoMode)
		{
			vLocalSystemData->vITIPCDig->ThresholdBuffer(
				vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, 
				vLocalSystemData->vITIPCDig->vOriginalBufferSize,
				TempInspection->vThreshold);
		}
		else
		{
			//if (vLocalConfigurationData->vDriftCompensationEnabled)
			//if (TempThreshold > vLocalSystemData->vDriftCompensationThresholdReductionAmount + 1)
			//if (TempInspection->vEnhanceMethod)
			//	TempThreshold = (BYTE)(TempThreshold - vLocalSystemData->vDriftCompensationThresholdReductionAmount + .49);

			//if (vGlobalLearnState)
			//if (vLocalConfigurationData->vAutoImproveEnabled)
			//if (TempInspection->vAddStructure)
			//{
			//	if (TempInspection->vROIPixelBottom < vGlobalCurrentProduct->vReferenceTop / 2)//if bottom contaminant
			//	{
			//		if (TempThreshold > (vGlobalLearnState * vLocalConfigurationData->vAutoImproveReduceThresholdFactor))
			//			TempThreshold = (BYTE)(TempThreshold - (vGlobalLearnState * vLocalConfigurationData->vAutoImproveReduceThresholdFactor)); 
			//	}
			//	else//if top contaminant
			//	{
			//		if (TempThreshold > (vGlobalLearnState * vLocalConfigurationData->vAutoImproveReduceThresholdFactor * 3))
			//			TempThreshold = (BYTE)(TempThreshold - (vGlobalLearnState * vLocalConfigurationData->vAutoImproveReduceThresholdFactor * 3)); 
			//	}
			//}

			//if ((vLocalSystemData->vITIPCDig->vImageForPseudoColor) && (vLocalSystemData->vITIPCDig->vImageForPseudoColor->pix))
			//	CopyMemory(vLocalSystemData->vITIPCDig->vImageForPseudoColor->pix, 
			//		vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

			////must blank off area outside ROI
			////blank left edge
			//if (TempLeft)
			//{
			//	DWORD TempCount = TempLeft * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
			//	FillMemory(vLocalSystemData->vITIPCDig->vImageForPseudoColor->pix, TempCount, 255);
			//}

			////blank Right edge
			//WORD TempRight = TempLeft + TempWidth;
			//if (TempRight < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1)
			//{
			//	DWORD TempStart = TempRight * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
			//	DWORD TempCount = vLocalSystemData->vITIPCDig->vOriginalBufferSize - TempStart - 1;
			//	FillMemory(vLocalSystemData->vITIPCDig->vImageForPseudoColor->pix + TempStart, TempCount, 255);
			//}

			////blank Bottom edge
			//if (TempBottom)
			//{
			//	DWORD TempStart = TempLeft * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
			//	DWORD TempCount = TempBottom;
			//	for (WORD TempLoop = 0; TempLoop < TempWidth; TempLoop++)
			//		FillMemory(vLocalSystemData->vITIPCDig->vImageForPseudoColor->pix + TempStart + 
			//			(TempLoop * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX), TempCount, 255);
			//}

			////blank Top edge
			//WORD TempTop = TempBottom + TempHeight;
			//if (TempTop < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1)
			//{
			//	DWORD TempStart = (TempLeft * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX) + TempTop;
			//	DWORD TempCount = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - TempTop;
			//	for (WORD TempLoop = 0; TempLoop < TempWidth; TempLoop++)
			//		FillMemory(vLocalSystemData->vITIPCDig->vImageForPseudoColor->pix + TempStart + 
			//			(TempLoop * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX), TempCount, 255);
			//}
			if ((vLocalSystemData->vITIPCDig->vImageForPseudoColor) && (vLocalSystemData->vITIPCDig->vImageForPseudoColor->pix))
			{
				CopyMemory(vLocalSystemData->vITIPCDig->vIATScratchImage->pix, vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

				//must blank off area outside ROI
				//blank left edge
				if (TempLeft)
				{
					DWORD TempCount = TempLeft * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
					FillMemory(vLocalSystemData->vITIPCDig->vIATScratchImage->pix, TempCount, 255);
				}

				//blank Right edge
				WORD TempRight = TempLeft + TempWidth;
				if (TempRight < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1)
				{
					DWORD TempStart = TempRight * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
					DWORD TempCount = vLocalSystemData->vITIPCDig->vOriginalBufferSize - TempStart - 1;
					FillMemory(vLocalSystemData->vITIPCDig->vIATScratchImage->pix + TempStart, TempCount, 255);
				}

				//blank Bottom edge
				if (TempBottom)
				{
					DWORD TempStart = TempLeft * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
					DWORD TempCount = TempBottom;
					for (WORD TempLoop = 0; TempLoop < TempWidth; TempLoop++)
						FillMemory(vLocalSystemData->vITIPCDig->vIATScratchImage->pix + TempStart + 
							(TempLoop * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX), TempCount, 255);
				}

				//blank Top edge
				WORD TempTop = TempBottom + TempHeight;
				if (TempTop < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1)
				{
					DWORD TempStart = (TempLeft * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX) + TempTop;
					DWORD TempCount = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - TempTop;
					for (WORD TempLoop = 0; TempLoop < TempWidth; TempLoop++)
						FillMemory(vLocalSystemData->vITIPCDig->vIATScratchImage->pix + TempStart + 
							(TempLoop * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX), TempCount, 255);
				}

				BYTE *TempThisInspectionPseudoColor = vLocalSystemData->vITIPCDig->vIATScratchImage->pix;
				BYTE *TempThisContainerPseudoColor = vLocalSystemData->vITIPCDig->vImageForPseudoColor->pix;
				for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
				{
					if (*TempThisInspectionPseudoColor < *TempThisContainerPseudoColor)
						*TempThisContainerPseudoColor = *TempThisInspectionPseudoColor;
					TempThisInspectionPseudoColor++;
					TempThisContainerPseudoColor++;
				}
			}

			if (!TempInspection->vDoContaminant22Algorithm) 	// ams		// dont threshold if c22
			{
				ThresholdImage(vLocalSystemData->vITIPCDig->vLatestImageCopy, TempThreshold);
			}
		}

		/*
		//br ht temp for test
			TempBottom = vLatestImageCopy->x;
			TempLeft = vLatestImageCopy->y;
			TempHeight = vLatestImageCopy->dx;
			TempWidth = vLatestImageCopy->dy;
		*/


		double TempStartFindBlobsTime = CIFCOS::GetSystimeMicrosecs();
		//check for contaminants
		
		if (vLocalConfigurationData->vDemoMode)
		{
			//since cannot do ROI in demo mode, Blank out edges.
			if (TempBottom)
			{
				for (WORD TempLoopY = 0; 
					TempLoopY < vLocalSystemData->vITIPCDig->vLatestImageCopy->dy; TempLoopY++)
				{
					BYTE *TempPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix +
						(TempLoopY * vLocalSystemData->vITIPCDig->vLatestImageCopy->dx);
					for (WORD TempLoopX = 0; 
						TempLoopX < TempBottom; TempLoopX++)
					{
						*TempPointer = 255;
						TempPointer++;
					}
				}
			}

			if (TempHeight)
			{
				for (WORD TempLoopY = 0; 
					TempLoopY < vLocalSystemData->vITIPCDig->vLatestImageCopy->dy; TempLoopY++)
				{
					BYTE *TempPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix + 
						TempHeight + TempBottom +
						(TempLoopY * vLocalSystemData->vITIPCDig->vLatestImageCopy->dx);
					for (WORD TempLoopX = TempHeight + TempBottom; 
						TempLoopX < vLocalSystemData->vITIPCDig->vLatestImageCopy->dx; TempLoopX++)
					{
						*TempPointer = 255;
						TempPointer++;
					}
				}
			}

			if (TempLeft)
			{
				for (WORD TempLoopY = 0; TempLoopY < TempLeft; TempLoopY++)
				{
					BYTE *TempPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix +
						(TempLoopY * vLocalSystemData->vITIPCDig->vLatestImageCopy->dx);
					for (WORD TempLoopX = 0; 
						TempLoopX < vLocalSystemData->vITIPCDig->vLatestImageCopy->dx; TempLoopX++)
					{
						*TempPointer = 255;
						TempPointer++;
					}
				}
			}

			if (TempWidth)
			{
				for (WORD TempLoopY = TempLeft + TempWidth; TempLoopY < vLocalSystemData->vITIPCDig->vLatestImageCopy->dy; TempLoopY++)
				{
					BYTE *TempPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix +
						(TempLoopY * vLocalSystemData->vITIPCDig->vLatestImageCopy->dx);
					for (WORD TempLoopX = 0; 
						TempLoopX < vLocalSystemData->vITIPCDig->vLatestImageCopy->dx; TempLoopX++)
					{
						*TempPointer = 255;
						TempPointer++;
					}
				}
			}


			{
				BYTE *TempPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix;
				for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vLatestImageCopy->dy; TempLoopY++)
				{
					for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vLatestImageCopy->dx; TempLoopX++)
					{
						if (!*TempPointer++)
						{
							vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = 1;
							vLocalSystemData->vITIPCDig->vLastLargestBlobPerimeter[TempInspectionNumber] = 22;
							//vLocalSystemData->vLastContainerImaged->vTheImage->vRejectedForThisInspection[TempInspectionNumber] = true;
							TempLoopX = vLocalSystemData->vITIPCDig->vLatestImageCopy->dx;
							TempLoopY = vLocalSystemData->vITIPCDig->vLatestImageCopy->dy;
						}
					}
					TempPointer = TempPointer +
						vLocalSystemData->vITIPCDig->vLatestImageCopy->ypitch;
				}
				if ((vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] > 0) && (!vLocalSystemData->vLastContainerImaged->vToBeEjected))
				{
					//mark that you need to eject this for contaminant based on processing so far
					//but may need to check other reference adjustments of this image first
					vLocalSystemData->vThisContainerShouldBeEjected = true;
					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString + " Demo Contaminant Found",cDebugMessage);
				}
				else
				{
					vLocalSystemData->vThisContainerShouldBeEjected = false;
					if (vLocalSystemData->vLogFile.vLogSerialData)
					{
						if (vLocalSystemData->vLastContainerImaged)
							vLocalSystemData->vLogFile.WriteToLogFile(dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0) + " No Contaminant",cDebugMessage);
						else
							vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString + " No Contaminant",cDebugMessage);
					}
				}

				if (vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber])
				//if evaluating, don't eject this container if already ejected
				if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
				//9/30/2009
				//if ((TempReferenceLoop == 0) && (!TempTopReferenceLoop)) //3/5/2003
				if (TempInspection->vEnable)
				{
					//demo mode code
					if ((vLocalSystemData->vITIPCDig->vImageForPseudoColor) && (vLocalSystemData->vITIPCDig->vImageForPseudoColor->pix))
						vLocalSystemData->vITIPCDig->SaveRedImageForDisplay(vLocalSystemData->vITIPCDig->vImageForPseudoColor, vLocalSystemData->vLastContainerImaged);

					//vLocalSystemData->vITIPCDig->Save10RedBoxesForDisplay(TempInspection, vLocalSystemData->vITIPCDig->vBlobResults, vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust);
				}
			}
		}
		else
		{ //not demo mode
			// test WriteImageToHardDisk(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, "C:\\ScanTracImages\\BrianTestPre.bmp", vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);
			if (TempInspection->vIPContaminant)
			if (vLocalSystemData->vITIPCDig->vIATScratchImage2)
			if (vLocalSystemData->vITIPCDig->vIATScratchImage2->pix)
				vLocalSystemData->vITIPCDig->AndImages(vLocalSystemData->vITIPCDig->vIATScratchImage2->pix, vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

			vLocalSystemData->vITIPCDig->vLatestSubImage = im_rroi(vLocalSystemData->vITIPCDig->vLatestImageCopy, TempBottom, TempLeft, TempHeight, TempWidth);
			if (vLocalSystemData->vITIPCDig->vLatestSubImage)
			{
			//test WriteImageToHardDisk(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, "C:\\ScanTracImages\\BrianTestFinal.bmp", vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

				if (TempInspection->vSize > 1)
					vLocalSystemData->vITIPCDig->vBlobParameters->min_area = TempInspection->vSize;
				else
					vLocalSystemData->vITIPCDig->vBlobParameters->min_area = -1;

				vLocalSystemData->vITIPCDig->vBlobParameters->blob_color = 0x00;  //fewer xrays
				vLocalSystemData->vITIPCDig->vBlobParameters->min_height = -1;
				vLocalSystemData->vITIPCDig->vBlobParameters->max_area = -1;
				vLocalSystemData->vITIPCDig->vBlobParameters->connected = CONNECT8;

				if ((TempInspection->vInspectionType == cDripInspection) || (TempInspection->vInspectionType == cDripBySpoutInspection))
				{
					if (TempInspection->vMaximumSize) //for Drip Inspection
						vLocalSystemData->vITIPCDig->vBlobParameters->max_area = TempInspection->vMaximumSize;
					if (TempInspection->vMinimumWidth) //for Drip Inspection
						vLocalSystemData->vITIPCDig->vBlobParameters->min_height = TempInspection->vMinimumWidth;
				}

				if ((TempInspection->vInspectionType == cBadHeartInspection) || ((TempInspection->vInspectionType == cDripInspection) && (TempInspection->vMaximumHeightAboveBottom)))
					vLocalSystemData->vITIPCDig->vBlobParameters->calc_results = AREA_BBOX + AREA_BBOX_CENT_PERIM;
				else
					vLocalSystemData->vITIPCDig->vBlobParameters->calc_results = AREA_BBOX;

				Erflag TempResult = mvt_blob_find(vLocalSystemData->vITIPCDig->vLatestSubImage, vLocalSystemData->vITIPCDig->vBlobParameters, vLocalSystemData->vITIPCDig->vBlobResults, MVT_CF_DEFAULT);

				double TempArea = 0;
				double TempLargestBlob = 0;
				double TempLargestPerimeter = 0;
				if ((WORD)vLocalSystemData->vITIPCDig->vBlobResults->num_blobs_found)
				//if evaluating, don't eject this container if already ejected
				//if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
				{
					BYTE *TempMaximumRedImage = vLocalSystemData->vITIPCDig->vImageForPseudoColor->pix + vLocalSystemData->vITIPCDig->vOriginalBufferSize;

					for (WORD TempLoop = 0; TempLoop < (WORD)vLocalSystemData->vITIPCDig->vBlobResults->num_blobs_found; TempLoop++)
					{
						//WORD TempBlobArea = (WORD)mvt_blob_get_area(vLocalSystemData->vITIPCDig->vBlobResults, TempLoop);
						DWORD TempBlobArea = vLocalSystemData->vITIPCDig->vBlobResults->blobs[TempLoop]->area;

						if ((TempInspection->vInspectionType == cDripInspection) && (TempInspection->vMaximumHeightAboveBottom))
						{
							//if the height of the centroid is higher than the setting above the bottom, then ignore this blob
							if (vLocalSystemData->vITIPCDig->vBlobResults->blobs[TempLoop]->xcent + vLocalSystemData->vITIPCDig->vLastBottomReferenceAdjust > 
								vLocalSystemData->vITIPCDig->vLastBottom + TempInspection->vMaximumHeightAboveBottom) //zero out this blobs area so it does not count if it is above the limit
							{
								TempBlobArea = 0;  //ignore this blob

								//blank the area in the image for this blob
								for (WORD TempLineLoop = 0; TempLineLoop < vLocalSystemData->vITIPCDig->vBlobResults->blobs[TempLoop]->height; TempLineLoop++)
								{
									BYTE *TempRedImagePointer = vLocalSystemData->vITIPCDig->vImageForPseudoColor->pix + 
										((vLocalSystemData->vITIPCDig->vBlobResults->blobs[TempLoop]->xmin + TempLineLoop) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);

									if (TempRedImagePointer + vLocalSystemData->vITIPCDig->vBlobResults->blobs[TempLoop]->width < TempMaximumRedImage)
										FillMemory(TempRedImagePointer, vLocalSystemData->vITIPCDig->vBlobResults->blobs[TempLoop]->width, 255);
								}

								vLocalSystemData->vITIPCDig->vBlobResults->blobs[TempLoop]->area = 0; //set the area of this blob to zero to clear it as it is too high above the bottom of the container
							}
						}

						//if (TempInspection->vInspectionType == cBadHeartInspection)
						//{
						//	//if the height of the centroid is higher than the setting above the bottom, then ignore this blob
						//	if (vLocalSystemData->vITIPCDig->vBlobResults->blobs[TempLoop]->perim < TempInspection->vMaximumPerimeter) //zero out this blobs area so it does not count if it is below perimeter limit
						//	{
						//		TempBlobArea = 0;  //ignore this blob

						//		//blank the area in the image for this blob
						//		for (WORD TempLineLoop = 0; TempLineLoop < vLocalSystemData->vITIPCDig->vBlobResults->blobs[TempLoop]->height; TempLineLoop++)
						//		{
						//			BYTE *TempRedImagePointer = vLocalSystemData->vITIPCDig->vImageForPseudoColor->pix + 
						//				((vLocalSystemData->vITIPCDig->vBlobResults->blobs[TempLoop]->xmin + TempLineLoop) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);

						//			if (TempRedImagePointer + vLocalSystemData->vITIPCDig->vBlobResults->blobs[TempLoop]->width < TempMaximumRedImage)
						//				FillMemory(TempRedImagePointer, vLocalSystemData->vITIPCDig->vBlobResults->blobs[TempLoop]->width, 255);
						//		}

						//		vLocalSystemData->vITIPCDig->vBlobResults->blobs[TempLoop]->area = 0; //set the area of this blob to zero to clear it as it is too high above the bottom of the container
						//	}
						//}

						if (TempBlobArea > TempLargestBlob)
							TempLargestBlob = TempBlobArea;

						TempArea = TempArea + TempBlobArea;

						if (TempInspection->vInspectionType == cBadHeartInspection)
						{
							WORD TempBlobPerimeter = (WORD)mvt_blob_get_perim(vLocalSystemData->vITIPCDig->vBlobResults, TempLoop);
							if (TempBlobPerimeter > TempLargestPerimeter)
								TempLargestPerimeter = TempBlobPerimeter;
						}
					}
					//if (TempInspection->vSize == 1)
					//if (TempMainImageRedPixels)
					//if (TempArea < TempMainImageRedPixels)
					//	TempArea = TempMainImageRedPixels;
				}

				//save the red image
				if (TempArea)
				//if evaluating, don't eject this container if already ejected
				if ((TempInspection->vEnable) && (!TempInspection->vInvertEjectLogic))
				{
					if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
					{
						vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason = TempInspectionNumber + cEjectInspectionOffset;
						vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectInspection = TempInspectionNumber;

						if (TempInspection->vAddStructure)
						if (vLocalConfigurationData->vAutoImproveEnabled)
							TempContainer->vTheImage->vContainerLearnState = vGlobalLearnState;

						TempContainer->vTheImage->vContainerEjectThreshold = TempThreshold;
						TempContainer->vTheImage->vContainerLargestContaminant = (WORD)TempLargestBlob;
					}
					vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = (WORD)TempArea;
					vLocalSystemData->vLastContainerImaged->vTheImage->vRejectedForThisInspection[TempInspectionNumber] = true;
					if (TempInspection->vInspectionType == cBadHeartInspection)
						vLocalSystemData->vITIPCDig->vLastLargestBlobPerimeter[TempInspectionNumber] = (WORD)TempLargestPerimeter;
					else
						vLocalSystemData->vITIPCDig->vLastLargestBlobPerimeter[TempInspectionNumber] = 0;
					
					vLocalSystemData->vITIPCDig->Save10RedBoxesForDisplay(TempInspection, vLocalSystemData->vITIPCDig->vBlobResults, vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust);

					if ((vLocalSystemData->vITIPCDig->vImageForPseudoColor) && (vLocalSystemData->vITIPCDig->vImageForPseudoColor->pix))
						vLocalSystemData->vITIPCDig->SaveRedImageForDisplay(vLocalSystemData->vITIPCDig->vImageForPseudoColor, vLocalSystemData->vLastContainerImaged);

					if (TempArea < vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber])
						vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = (DWORD)TempArea;
				}

				if ((TempArea > 0) && (!vLocalSystemData->vLastContainerImaged->vToBeEjected))
				{
					if (!TempInspection->vInvertEjectLogic)
					{
						if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
							TempContainer->vTheImage->vContainerEjectThreshold = (float)TempThreshold;

						InspectionFoundContaminant(TempInspectionNumber);

						if (vLocalSystemData->vLogFile.vLogSerialData)
						{
						
							if (vLocalSystemData->vLastContainerImaged)
							if (vLocalSystemData->vLastContainerImaged->vContainerNumber)
								vLocalSystemData->vLogFile.WriteToLogFile(dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0) +
									" Contaminants Found Inspection: " + dtoa(TempInspectionNumber,0) + ", S: " + dtoa(TempReferenceLoop,0) + ", T: " + dtoa(TempTopReferenceLoop,0),cDebugMessage);
							else
								vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString + " Contaminants Found Inspection: " + dtoa(TempInspectionNumber,0) +
										", S: " + dtoa(TempReferenceLoop,0) + ", T: " + dtoa(TempTopReferenceLoop,0),cDebugMessage);
						}
					}
					else
					{
						vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = 0;

						if (vLocalSystemData->vLastContainerImaged)
						if (vLocalSystemData->vLastContainerImaged->vContainerNumber)
							vLocalSystemData->vLogFile.WriteToLogFile(dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0) +
								" Contaminants Found But Invert Logic, so accept. Inspection: " + dtoa(TempInspectionNumber,0) + ", S: " + dtoa(TempReferenceLoop,0) + ", T: " + dtoa(TempTopReferenceLoop,0),cDebugMessage);
						else
							vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString + " Contaminants Found, But Invert Logic, so accept. Inspection: " + dtoa(TempInspectionNumber,0) +
									", S: " + dtoa(TempReferenceLoop,0) + ", T: " + dtoa(TempTopReferenceLoop,0),cDebugMessage);
					}
				}
				else
				if ((TempArea == 0) && (!vLocalSystemData->vLastContainerImaged->vToBeEjected))
				{
					if (TempInspection->vInvertEjectLogic)
					{
						if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
							TempContainer->vTheImage->vContainerEjectThreshold = (float)TempThreshold;

						vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = 1;
						InspectionFoundContaminant(TempInspectionNumber);
						
						vLocalSystemData->vITIPCDig->vLastRedBoxCornerX[0] = TempBottom;
						vLocalSystemData->vITIPCDig->vLastRedBoxCornerY[0] = TempLeft;
						vLocalSystemData->vITIPCDig->vLastRedBoxHeight[0] = TempHeight;
						vLocalSystemData->vITIPCDig->vLastRedBoxWidth[0] = TempWidth;
						vLocalSystemData->vITIPCDig->vLastRedBoxInspection[0] = TempInspectionNumber;
						
						if (vLocalSystemData->vLogFile.vLogSerialData)
						{
							if (vLocalSystemData->vLastContainerImaged)
								vLocalSystemData->vLogFile.WriteToLogFile(dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0) + " No Contaminant, But Invert Logic, so Reject, Inspection: " + dtoa(TempInspectionNumber,0),cDebugMessage);
							else
								vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString +" No Contaminant, But Invert Logic, so Reject, Inspection: " + dtoa(TempInspectionNumber,0),cDebugMessage);
						}
					}
					else
					{
						vLocalSystemData->vThisContainerShouldBeEjected = false;
						if (vLocalSystemData->vLogFile.vLogSerialData)
						{
							if (vLocalSystemData->vLastContainerImaged)
								vLocalSystemData->vLogFile.WriteToLogFile(dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0) + " No Contaminant, Inspection: " + dtoa(TempInspectionNumber,0),cDebugMessage);
							else
								vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString +" No Contaminant, Inspection: " + dtoa(TempInspectionNumber,0),cDebugMessage);
						}
					}
				}

				im_delete(vLocalSystemData->vITIPCDig->vLatestSubImage);
				vLocalSystemData->vITIPCDig->vLatestSubImage = NULL;
			}
			else
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString +
							" Error, MVT Could not create Image for Blobs",cDebugMessage);
		}

		TempEndTime = CIFCOS::GetSystimeMicrosecs();
		vLocalSystemData->vITIPCDig->vLastFindBlobsTime = TempEndTime - TempStartFindBlobsTime;


		if (vLocalConfigurationData->vDemoMode)
		{
			bool TempInBlob = false;
			BYTE *TempPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix;
			for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
			{
				if (*TempPointer++ == 0)
				{
					if (!TempInBlob)
					{
						TempInBlob = true;
						vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber]++;
						vLocalSystemData->vLastContainerImaged->vTheImage->vRejectedForThisInspection[TempInspectionNumber] = true;
					}
				}
				else
					TempInBlob = false;
			}
			if ((vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] > 0) && (!vLocalSystemData->vLastContainerImaged->vToBeEjected))
			{
				vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] / 2;
				vLocalSystemData->vLastContainerImaged->vTheImage->vRejectedForThisInspection[TempInspectionNumber] = true;
				//mark that you need to eject this for contaminant based on processing so far
				//but may need to check other reference adjustments of this image first
				vLocalSystemData->vThisContainerShouldBeEjected = true;
				if (!TempInspection->vInvertEjectLogic)
				{
					if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
						TempContainer->vTheImage->vContainerEjectThreshold = (float)TempThreshold;

					InspectionFoundContaminant(TempInspectionNumber);
				}
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString + " Demo Contaminant Found",cDebugMessage);
			}
			else
			{
				vLocalSystemData->vThisContainerShouldBeEjected = false;
				if (TempInspection->vInvertEjectLogic)
				{
					if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
						TempContainer->vTheImage->vContainerEjectThreshold = (float)TempThreshold;

					InspectionFoundContaminant(TempInspectionNumber);
				}
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString + " Demo No Contaminant",cDebugMessage);
			}
		}
	}
}

void CImageAquisitionThread::FindSizeInspection(CContainer *TempContainer, CInspection *TempInspection, BYTE TempInspectionNumber)
{
	WORD TempBottom = 0;
	WORD TempLeft = 0;
	WORD TempHeight = 0;
	WORD TempWidth = 0;
	vLocalSystemData->vITIPCDig->GetInspectionROIBounds(TempInspection, &TempBottom, &TempLeft, &TempHeight, &TempWidth, 0, //already referenced vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust, 
		vLocalConfigurationData->vReferenceROIsToEdges, 0);

	BYTE *TempPointer = NULL;
	BYTE *TempRedPointer = NULL;
	if (TempContainer)
	if (TempContainer->vTheImage)
	{
		TempContainer->vTheImage->MakeRedImage(vLocalSystemData->vITIPCDig->vOriginalBufferSize);
		FillMemory(TempContainer->vTheImage->vRedImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize, 255);
		if (TempContainer->vTheImage->vRedImage)
		for (WORD TempXLoop = 0; TempXLoop < TempWidth; TempXLoop++)
		{
			TempPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix + TempBottom + (TempLeft + TempXLoop) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;

			TempRedPointer = TempContainer->vTheImage->vRedImage + TempBottom + (TempLeft + TempXLoop) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;

			for (WORD TempYLoop = 0; TempYLoop < TempHeight; TempYLoop++)
			{
				if (*TempPointer < TempInspection->vThreshold)  //if pixel is  darker than threshold
				if ((TempInspection->vMinimumBrightnessAceptable == 255) || (TempInspection->vMinimumBrightnessAceptable == 0) || (*TempPointer > TempInspection->vMinimumBrightnessAceptable))
				{
					vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber]++;
					*TempRedPointer = 0;
				}
				TempRedPointer++;
				TempPointer++;
			}
		}
		TempContainer->vTheImage->vContainerLargestContaminant = vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber];

		if (TempInspection->vInspectionType == cItemCountInspection)
		{ //now have an image of just the pixels in the ROI that are between the thresholds, so do blobl analysis
			TempContainer->vTheImage->vContainerLargestContaminant = 0;
			vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = 0;

			if (TempInspection->vSize > 1)
				vLocalSystemData->vITIPCDig->vBlobParameters->min_area = TempInspection->vSize;
			else
				vLocalSystemData->vITIPCDig->vBlobParameters->min_area = 1;

			if (TempInspection->vMaximumSize > 0)
				vLocalSystemData->vITIPCDig->vBlobParameters->max_area = TempInspection->vMaximumSize;
			else
				vLocalSystemData->vITIPCDig->vBlobParameters->max_area = -1; //for insert get blobs of any size

			vLocalSystemData->vITIPCDig->vBlobParameters->blob_color = 0x00;  //fewer xrays
			vLocalSystemData->vITIPCDig->vBlobParameters->min_height = -1;

			CopyMemory(vLocalSystemData->vITIPCDig->vIATScratchImage->pix, TempContainer->vTheImage->vRedImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

			Erflag TempResult = mvt_blob_find(vLocalSystemData->vITIPCDig->vIATScratchImage, vLocalSystemData->vITIPCDig->vBlobParameters, vLocalSystemData->vITIPCDig->vBlobResults, MVT_CF_DEFAULT);

			double TempArea = 0;
			double TempLargestBlob = 0;
			double TempLargestPerimeter = 0;
			if ((WORD)vLocalSystemData->vITIPCDig->vBlobResults->num_blobs_found)
			{
				vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = (DWORD)vLocalSystemData->vITIPCDig->vBlobResults->num_blobs_found;

				TempContainer->vTheImage->vContainerLargestContaminant = 0;
				for (BYTE TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vBlobResults->num_blobs_found; TempLoop++)
				if (vLocalSystemData->vITIPCDig->vBlobResults->blobs[TempLoop]->area > TempContainer->vTheImage->vContainerLargestContaminant)
					TempContainer->vTheImage->vContainerLargestContaminant = vLocalSystemData->vITIPCDig->vBlobResults->blobs[TempLoop]->area;
				//if evaluating, don't eject this container if already ejected
				if (vLocalSystemData->vLastContainerImaged->vToBeEjected)
				{
					BYTE *TempMaximumRedImage = vLocalSystemData->vITIPCDig->vImageForPseudoColor->pix + vLocalSystemData->vITIPCDig->vOriginalBufferSize;

					for (WORD TempLoop = 0; TempLoop < (WORD)vLocalSystemData->vITIPCDig->vBlobResults->num_blobs_found; TempLoop++)
					{
						TempContainer->vTheImage->vContainerLargestContaminant = 0;
					}
				}
			}
		}
	}
	bool TempEjectThisOne = false;

	if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
	{
		if (TempInspection->vInspectionType == cItemCountInspection)
		{
			if (((TempInspection->vMinimumNumberOfItemsRequired) && (vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] < TempInspection->vMinimumNumberOfItemsRequired)) ||
				((TempInspection->vMaximumNumberOfItemsRequired) && (vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] > TempInspection->vMaximumNumberOfItemsRequired)))
				TempEjectThisOne = true;

			if (TempInspection->vInvertEjectLogic)
				TempEjectThisOne = !TempEjectThisOne;
		}
		else
		{
			if (vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] < TempInspection->vSize)
				TempEjectThisOne = true;

			if (TempInspection->vInvertEjectLogic)
				TempEjectThisOne = !TempEjectThisOne;
		}
	}

	if (TempEjectThisOne)
	{
		//mark that you need to eject this for contaminant based on processing so far
		//but may need to check other reference adjustments of this image first
		vLocalSystemData->vThisContainerShouldBeEjected = true;

		if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
			TempContainer->vTheImage->vContainerEjectThreshold = (float)TempInspection->vSize;

		InspectionFoundContaminant(TempInspectionNumber);

		//did not find any or enough blobs big enough, so eliminate the minimum size and find all blobs of any size to show red for
		vLocalSystemData->vITIPCDig->vBlobParameters->min_area = 2;
		vLocalSystemData->vITIPCDig->vBlobParameters->max_area = -1; //for insert get blobs of any size

		vLocalSystemData->vITIPCDig->vBlobParameters->blob_color = 0x00;  //fewer xrays
		vLocalSystemData->vITIPCDig->vBlobParameters->min_height = -1;
		CopyMemory(vLocalSystemData->vITIPCDig->vIATScratchImage->pix, TempContainer->vTheImage->vRedImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
		Erflag TempResult = mvt_blob_find(vLocalSystemData->vITIPCDig->vIATScratchImage, vLocalSystemData->vITIPCDig->vBlobParameters, vLocalSystemData->vITIPCDig->vBlobResults, MVT_CF_DEFAULT);

		vLocalSystemData->vITIPCDig->Save10RedBoxesForDisplay(TempInspection, vLocalSystemData->vITIPCDig->vBlobResults, vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust);

		if (vLocalSystemData->vLogFile.vLogSerialData)
		{
			if (vLocalSystemData->vLastContainerImaged)
				vLocalSystemData->vLogFile.WriteToLogFile(dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0) + " Size Too Small",cDebugMessage);
			else
				vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString + " Size Too Small",cDebugMessage);
		}
	}
	else
	if (TempContainer)
	if (TempContainer->vTheImage)
	{
		//vLocalSystemData->vThisContainerShouldBeEjected = false;
		if (vLocalSystemData->vLogFile.vLogSerialData)
		{
			if (TempInspection->vInspectionType == cItemCountInspection)
				vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString + " Object Count OK",cDebugMessage);
			else
				vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString + " Size Large Enough",cDebugMessage);
		}

		TempContainer->vTheImage->MakeRedImage(vLocalSystemData->vITIPCDig->vOriginalBufferSize);
		if (TempContainer->vTheImage->vRedImage)
			FillMemory(TempContainer->vTheImage->vRedImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize, 1);
	}

	Image *TempImage = NULL;
	if (TempInspection->vFindGoodIntensity) //evaluate good size inspection
	if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
	if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cPassSlippedContainer)//slipped
		vLocalSystemData->vWrongWidthsDuringLearn++;
	else
	if ((vSizeOK) || (vNumberOfWrongSizeInARow > 50))
	{//Size inspection
		if (!vSizeOK)
		{
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("Autosetup, Find Minimum Good, Container width too wide/narrow", cDebugMessage);
		}
		//first time through, initialize data structures
		if (!vLocalSystemData->vMinimumGoodIndex)
		{
			if (vLocalSystemData->vMinimumGoodDataFloat)
			{
				free(vLocalSystemData->vMinimumGoodDataFloat);
				vLocalSystemData->vMinimumGoodDataFloat = NULL;
			}
			if (ThereIsEnoughMemory(cStandardDeviationSampleSize * 8,"Minimum Good Sample Array"))
				vLocalSystemData->vMinimumGoodDataFloat = (double *)malloc(cStandardDeviationSampleSize * 8);
			vLocalSystemData->vMinimumGoodStandardDeviation = 0;
			vLocalSystemData->vMinimumGoodAverage = 0;
			vLocalSystemData->vMinimumGoodIntensity = (float)999999;

			double TempThreshold = 90;
			//first time through figure out reasonable Threshold
			if (vGlobalCurrentProduct->vSetupHistoryAverageDensity)
			{
				BYTE TempDarkestPixelInROI = vLocalSystemData->vITIPCDig->FindDarkestPixelInROI(TempInspection, vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, 0, vLocalConfigurationData->vReferenceROIsToEdges);

				if (vGlobalCurrentProduct->vSetupHistoryAverageDensity > TempDarkestPixelInROI)
					TempThreshold = (BYTE)(TempDarkestPixelInROI + ((vGlobalCurrentProduct->vSetupHistoryAverageDensity - TempDarkestPixelInROI) * 0.35));
				else
					TempThreshold = (BYTE)(vGlobalCurrentProduct->vSetupHistoryAverageDensity * 0.71);
			}
			else
				TempThreshold = 90;

			if (TempThreshold > 240)
				TempThreshold = 90;
			if (TempThreshold < 10)
				TempThreshold = 90;

			if (((TempInspection->vInspectionType == cSizeInspection) || (TempInspection->vInspectionType == cMissingLidInspectionOnSize)))
			if (!vLocalSystemData->vManuallyEditedDuringEvaluate)
				TempInspection->vThreshold = (BYTE)TempThreshold;

			//TempInspection->vThreshold = TempInspection->vSetupHistoryAutoThreshold;  //must set the Threshold so can evaluate the size
			//end of first image only calculate the Threshold
		}//end of first time through evaluate
		//br ht
		//if (!vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
			//TempMinimumGood = 150 - (vLocalSystemData->vMinimumGoodIndex % 20);
		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("Autosetup, Find Minimum Good",	cDebugMessage);

		if (vLocalSystemData->vMinimumGoodDataFloat)
		{
			double *TempMinimumGoodDataPointer = vLocalSystemData->vMinimumGoodDataFloat + vLocalSystemData->vMinimumGoodIndex;

			*TempMinimumGoodDataPointer = vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber];
			//save the best value for displaying on the screen
			vLocalSystemData->vITIPCDig->vLastMinimumGoodMaximumBad = *TempMinimumGoodDataPointer;

			if (vLocalSystemData->vMinimumGoodIntensity > *TempMinimumGoodDataPointer)
				vLocalSystemData->vMinimumGoodIntensity = *TempMinimumGoodDataPointer;

			if (vLocalSystemData->vMinimumGoodIndex < cStandardDeviationSampleSize - 1)
				vLocalSystemData->vMinimumGoodIndex++;
			else
				TempInspection->vFindGoodIntensity = false;

			if (vLocalSystemData->vMinimumGoodIndex > 3)
			{
				vLocalSystemData->vMinimumGoodStandardDeviation = CalculateStandardDeviation(vLocalSystemData->vMinimumGoodDataFloat, vLocalSystemData->vMinimumGoodIndex, &vLocalSystemData->vMinimumGoodAverage);
				if (vLocalSystemData->vMinimumGoodStandardDeviation < 2)
					vLocalSystemData->vMinimumGoodStandardDeviation = 2;
			}
		}
	}

	if (TempInspection->vFindBadIntensity)
	if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
	if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cPassSlippedContainer)//slipped
		vLocalSystemData->vWrongWidthsDuringLearn++;
	else
	if ((vSizeOK) || (vNumberOfWrongSizeInARow > 50))
	{//size inspection, Insert Inspection
		{  // if the bad intensity is below the good level
			//first time through, initialize data structures
			if (!vLocalSystemData->vMaximumBadIndex)
			{
				vLocalSystemData->vMaximumBadCount = 0;
				if (vLocalSystemData->vMaximumBadDataFloat)
				{
					free(vLocalSystemData->vMaximumBadDataFloat);
					vLocalSystemData->vMaximumBadDataFloat = NULL;
				}
				if (ThereIsEnoughMemory(cStandardDeviationSampleSize * sizeof(double), "Maximum Bad Sample Array"))
					vLocalSystemData->vMaximumBadDataFloat = (double*)malloc(cStandardDeviationSampleSize * sizeof(double));
				vLocalSystemData->vMaximumBadStandardDeviation = 0;
				vLocalSystemData->vMaximumBadAverage = 0;
				vLocalSystemData->vMaximumBadIntensity = 0;
				vLocalSystemData->vQualityFactor = 0;
			}

			if (vLocalSystemData->vMaximumBadDataFloat)
			{
				double *TempMaximumBadDataPointer = vLocalSystemData->vMaximumBadDataFloat + vLocalSystemData->vMaximumBadIndex;
				*TempMaximumBadDataPointer = vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber];

				//save the best value for displaying on the screen
				vLocalSystemData->vITIPCDig->vLastMinimumGoodMaximumBad = *TempMaximumBadDataPointer;

				if (vLocalSystemData->vMaximumBadIntensity < *TempMaximumBadDataPointer)
					vLocalSystemData->vMaximumBadIntensity = *TempMaximumBadDataPointer;

				if (vLocalSystemData->vMaximumBadIndex < cStandardDeviationSampleSize - 1)
				{
					vLocalSystemData->vMaximumBadIndex++;
					vLocalSystemData->vMaximumBadCount++;
				}
				else
					TempInspection->vFindBadIntensity = false;

				if (vLocalSystemData->vMaximumBadIndex > 3)
				{
					vLocalSystemData->vMaximumBadStandardDeviation = CalculateStandardDeviation(vLocalSystemData->vMaximumBadDataFloat, vLocalSystemData->vMaximumBadIndex, &vLocalSystemData->vMaximumBadAverage);
					if (vLocalSystemData->vMaximumBadStandardDeviation < 2)
						vLocalSystemData->vMaximumBadStandardDeviation = 2;
				} 
			}
		}  //end if (TempMaximumBad < vLocalSystemData->vMinimumGoodIntensity)
	}
}

void CImageAquisitionThread::FindKernelContaminants(CContainer *TempContainer, 
	BYTE TempReferenceLoop, BYTE TempTopReferenceLoop, CInspection *TempInspection, 
	BYTE TempInspectionNumber)
{
	WORD TempBottom = 0;
	WORD TempLeft = 0;
	WORD TempHeight = 0;
	WORD TempWidth = 0;
	vLocalSystemData->vITIPCDig->GetInspectionROIBounds(TempInspection, 
		&TempBottom, &TempLeft, &TempHeight, &TempWidth, 0, //already referenced vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust, 
		vLocalConfigurationData->vReferenceROIsToEdges, 0);

	if (TempInspection)
	{
		if (TempInspection->vTrimEdgesOfJar)
		if (!TempInspection->vDoContaminant22Algorithm)
		{
			TempInspection->TrimEdgesOfJarAtROI(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix,
				vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, true,
				vLocalSystemData->vITIPCDig->vLastLeft -
				vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust,
				vLocalSystemData->vITIPCDig->vLastRight -
				vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust);
		}

		if (TempInspection->vFillInLightSpots)
			TempInspection->FillInLightSpots(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, 
			vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, TempInspection->vFillInLightSpots);

		//if (TempInspection->vFillInLightSpots)
		//	TempContainer->vTheImages->vCurveFitTime = TempContainer->vTheImages->vCurveFitTime +
		//		TempInspection->CurveFitFillInLightSpots(vLocalSystemData->vITIPCDig->vImageProcessingNowCopy->pix,
		//		vLocalSystemData->vITIPCDig->vDisplayBufferHeightInPixels, vLocalSystemData->vITIPCDig->vDisplayBufferWidthInPixels);

		//subtract the product's average image from this one if enabled
		if (TempInspection->vSubtractAverage)
		if (vLocalSystemData->vITIPCDig->vProductAverageImage)
			vLocalSystemData->vITIPCDig->SubtractProductAverageImage(
			vLocalSystemData->vITIPCDig->vLatestImageCopy->pix,
			vLocalSystemData->vITIPCDig->vProductAverageImage);

		//add the product's structure image to this one if enabled
		if ((TempInspection->vAddStructure) && (TempInspection->vSubtractAverage))
		{
			if (vLocalSystemData->vITIPCDig->vProductStructureImage)
				vLocalSystemData->vITIPCDig->AddProductStructureImage(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix);
		}
		else
		if ((TempInspection->vAddStructure) && (!TempInspection->vSubtractAverage))
		{
			if (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage)
				vLocalSystemData->vITIPCDig->AddProductContaminant2StructureImage(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix);
		}

		if (TempInspection->vEnhanceMethod)
		{
			double TempStartFindTime = CIFCOS::GetSystimeMicrosecs();
			vLocalSystemData->vITIPCDig->PerformEnhanceOnImage(vLocalSystemData->vITIPCDig->vLatestImageCopy, vLocalSystemData->vITIPCDig->vIATScratchImage, TempInspection->vEnhanceMethod);
			double 	TempEndTime = CIFCOS::GetSystimeMicrosecs();
			vLocalSystemData->vITIPCDig->vLastEnhanceTime = TempEndTime - TempStartFindTime;
		}

		//if (TempInspection->vDoImageFactors)
		//	vLocalSystemData->vITIPCDig->CombineImages(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, TempInspection->vEnhancedImageFactor, vLocalSystemData->vITIPCDig->vLatestImage->pix, TempInspection->vOriginalImageFactor);

		/*
		if (vLocalSystemData->vITIPCDig->vIATScratchImage)
		{
			vLocalSystemData->vITIPCDig->MaskImageWithDialatedImage(
				vLocalSystemData->vITIPCDig->vLatestImageCopy,vLocalSystemData->vITIPCDig->vIATScratchImage);
		}
		*/

		//need to analyze each kernel in vLatestImageCopy (manipulated image)
		if (TempInspection->vAddHistogram)
		if (vLocalSystemData->vITIPCDig->vProductHistogramImage)
			vLocalSystemData->vITIPCDig->AddProductHistogramImage(
			vLocalSystemData->vITIPCDig->vLatestImageCopy->pix);

		Image *TempKernelImage;
		//create an image to store the amount each pixel is different from the baseline
		TempKernelImage = im_create(IM_BYTE, 
			vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, 
			vLocalSystemData->vITIPCDig->vAquisitionSizeY);
		
		if ((TempKernelImage) && (TempKernelImage->pix))
		if (TempContainer)
		if (TempContainer->vTheImage)
		{
			//store the amount each pixel is different from the baseline in the pixel in the new image
			/*
			BYTE TempMax = 127;
			if (!TempInspection->vEnhanceMethod)
			if (TempInspection->vSubtractAverage)
				TempMax = 0;
				*/
			BYTE *TempDesitinationPointer = TempKernelImage->pix;
			BYTE *TempSourcePointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix;
			for (DWORD TempLoop = 0;TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; 
				TempLoop++)
			{
				*TempDesitinationPointer++ = *TempSourcePointer++;// + TempMax;
			}

			//find out how many kernels high and wide will have to analyze
			WORD TempNumberOfKernelsHigh = (WORD)((((double)TempInspection->vROIPixelTop - 
				(double)TempInspection->vROIPixelBottom) / 
				(double)TempInspection->vSize * 2.0) + .99);
			WORD TempNumberOfKernelsWide = (WORD)((((double)TempInspection->vROIPixelRight - 
				(double)TempInspection->vROIPixelLeft) / 
				(double)TempInspection->vSize * 2.0) + .99);
			bool TempRejectThisPass = false;
			
			double TempMinimumGood = 255;
			double TempMaximumBad = 255;
			WORD TempDefectSize = 0;
			BYTE *TempPointerToRedImageBuffer = NULL;

			TempContainer->vTheImage->MakeRedImage(vLocalSystemData->vITIPCDig->vOriginalBufferSize);
			if (TempContainer->vTheImage->vRedImage)
				TempPointerToRedImageBuffer = TempContainer->vTheImage->vRedImage;
			else
			{
				if (vLocalSystemData->vThisContainerShouldBeEjected)
				{
					//already have an image in the red buffer, so create a different buffer to 
					//hold the new image in so can see which is smaller
					if (ThereIsEnoughMemory(vLocalSystemData->vITIPCDig->vOriginalBufferSize, "Temp Red Image to Display Buffer"))
						TempPointerToRedImageBuffer = (BYTE *)malloc(vLocalSystemData->vITIPCDig->vOriginalBufferSize);

					if (TempPointerToRedImageBuffer)
					{
						BYTE *TempDestinationPointer = TempPointerToRedImageBuffer;
						for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
							*TempDestinationPointer++ = 1;
					}
				}
			}

			//analyze each kernel high and wide
			for (WORD TempKernelHighLoop = 0; TempKernelHighLoop < TempNumberOfKernelsHigh;
				TempKernelHighLoop++)
			for (WORD TempKernelWideLoop = 0; TempKernelWideLoop < TempNumberOfKernelsWide;
				TempKernelWideLoop++)
			{
				DWORD TempKernelSum = 0;
				//find the starting place for this kernel analysis
				WORD TempKernelWideStart = (WORD)(TempInspection->vROIPixelLeft + (TempKernelWideLoop * TempInspection->vSize / 2));
				if (TempInspection->vROIPixelLeft + TempInspection->vSize > TempInspection->vROIPixelRight)
				{
					if (TempInspection->vROIPixelLeft + TempInspection->vSize < vLocalSystemData->vITIPCDig->vAquisitionSizeY)
						TempInspection->vROIPixelRight = (WORD)(TempInspection->vROIPixelLeft + TempInspection->vSize);
					else
						TempInspection->vROIPixelLeft = 0;
				}
				if (TempKernelWideStart + TempInspection->vSize > TempInspection->vROIPixelRight)
					TempKernelWideStart = (WORD)(TempInspection->vROIPixelRight - TempInspection->vSize);

				WORD TempKernelHighStart = (WORD)(TempInspection->vROIPixelBottom + (TempKernelHighLoop * TempInspection->vSize / 2));
				if (TempKernelHighStart + TempInspection->vSize > TempInspection->vROIPixelTop)
					TempKernelHighStart = (WORD)(TempInspection->vROIPixelTop - TempInspection->vSize);
				
				//sum up all the pixels in this kernel
				WORD TempSumCount = 0;
				for (WORD TempSumWideLoop = 0; TempSumWideLoop < TempInspection->vSize;
					TempSumWideLoop++)
				{
					BYTE *TempDataPointer = TempKernelImage->pix + 
						TempKernelHighStart + 
						((TempKernelWideStart + TempSumWideLoop) * 
						vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
					for (WORD TempSumHighLoop = 0; TempSumHighLoop < TempInspection->vSize;
						TempSumHighLoop++)
					{
						if (*TempDataPointer < 255)
						{
							TempKernelSum = TempKernelSum + *TempDataPointer;
							TempSumCount++;
						}
						TempDataPointer++;
					}
				}
				double TempKernelAverage = 255;
				if (TempSumCount)
					TempKernelAverage = TempKernelSum / (double)TempSumCount;

				if (TempInspection->vFindGoodIntensity)
				if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
				{
					if (TempKernelAverage < TempMinimumGood)
						TempMinimumGood = TempKernelAverage;
				}
				if (TempInspection->vFindBadIntensity)
				if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
				{
					//find maximum bad for this frame
					if (TempKernelAverage < TempMaximumBad)
						TempMaximumBad = TempKernelAverage;  //find the lowest reading this reject gives
				}
				//if this kernel is over the average, then it is a reject
				if (TempKernelAverage < TempInspection->vThreshold)
				{
					TempRejectThisPass = true;

				//put any pixels not in the baseline in the red image buffer since going to reject
					//if (!TempReferenceLoop)
					if ((!vLocalSystemData->vITIPCDig->vShowOnlyRealImages) || 
						(vLocalSystemData->vITIPCDig->vHaveRealImage) ||
						(vLocalSystemData->vITIPCDig->vProcessNextImage))
					if (TempPointerToRedImageBuffer)
					{
						BYTE *TempDestinationPointer;
						//look at all the pixels in this kernel, any that are not at baseline set red in red image
						for (WORD TempSumWideLoop = 0; TempSumWideLoop < TempInspection->vSize; TempSumWideLoop++)
						{
							BYTE *TempDataPointer = TempKernelImage->pix + 
								TempKernelHighStart + 
								((TempKernelWideStart +	TempSumWideLoop) * 
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
							TempDestinationPointer = TempPointerToRedImageBuffer + TempKernelHighStart + ((TempKernelWideStart + TempSumWideLoop) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
							for (WORD TempSumHighLoop = 0; TempSumHighLoop < TempInspection->vSize;
								TempSumHighLoop++)
							{
								if (*TempDataPointer < TempInspection->vThreshold)
								{
									*TempDestinationPointer = 0; //red spot
									TempDefectSize++;
								}
								*TempDataPointer++;
								*TempDestinationPointer++;
							}
						}
					}

				}

			}
			if (TempRejectThisPass)
			//if evaluating, don't eject this container if already ejected
			if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
			{

				if (vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] == 0)
				{ //is the first red image, so just record how many pixels are red
					vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = TempDefectSize;
				}
				else
				{
					if (vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] > TempDefectSize)
					{ //if few red pixels in this image, then previous one, then save this one
						vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = TempDefectSize;
						//vLocalSystemData->vLastContainerImaged->vTheImage->vRejectedForThisInspection[TempInspectionNumber] = true;

						if (TempContainer)
						if (TempContainer->vTheImage)
						if (TempPointerToRedImageBuffer)
						{
							TempContainer->vTheImage->MakeRedImage(vLocalSystemData->vITIPCDig->vOriginalBufferSize);
							if (TempContainer->vTheImage->vRedImage)
								CopyMemory(TempContainer->vTheImage->vRedImage, TempPointerToRedImageBuffer, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						}
					}
				}

				vLocalSystemData->vThisContainerShouldBeEjected = true;
				//vLocalSystemData->vLastContainerImaged->vTheImage->vRejectedForThisInspection[TempInspectionNumber] = true;
			}
			else
			{
				vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = 0;
				vLocalSystemData->vThisContainerShouldBeEjected = false;
				//vLocalSystemData->vLastContainerImaged->vTheImage->vRejectedForThisInspection[TempInspectionNumber] = false;
			}

			if (TempInspection->vFindGoodIntensity)
			if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
			if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cPassSlippedContainer)//slipped
				vLocalSystemData->vWrongWidthsDuringLearn++;
			else
			if ((vSizeOK) || (vNumberOfWrongSizeInARow > 50))
			{
				//find minimum good for this frame

				//first time through, initialize data structures
				if (!vLocalSystemData->vMinimumGoodIndex)
				if (!TempReferenceLoop)
				{
					if (vLocalSystemData->vMinimumGoodData)
					{
						free(vLocalSystemData->vMinimumGoodData);
						vLocalSystemData->vMinimumGoodData = NULL;
					}
					if (ThereIsEnoughMemory(cStandardDeviationSampleSize,"Minimum Good Sample Array"))
						vLocalSystemData->vMinimumGoodData = 
						(BYTE*)malloc(cStandardDeviationSampleSize);
					vLocalSystemData->vMinimumGoodStandardDeviation = 0;
					vLocalSystemData->vMinimumGoodAverage = 0;
					vLocalSystemData->vMinimumGoodIntensity = 256;
				}

				if (vLocalSystemData->vMinimumGoodData)
				{
					BYTE *TempMinimumGoodDataPointer = vLocalSystemData->vMinimumGoodData + 
						vLocalSystemData->vMinimumGoodIndex;
					*TempMinimumGoodDataPointer = (BYTE)TempMinimumGood;
				}
			}  //end find good intensity
			if (TempInspection->vFindBadIntensity)
			if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
			if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cPassSlippedContainer)//slipped
				vLocalSystemData->vWrongWidthsDuringLearn++;
			else
			if ((vSizeOK) || (vNumberOfWrongSizeInARow > 50))
			{
				//find maximum bad for this frame

				if (TempMaximumBad == 0)
					TempMaximumBad = 1;  //br ht
				{  // if the bad intensity is below the good level
					//first time through, initialize data structures
					if (!vLocalSystemData->vMaximumBadIndex)
					if (!TempReferenceLoop)
					{
						vLocalSystemData->vMaximumBadCount = 0;
						if (vLocalSystemData->vMaximumBadData)
						{
							free(vLocalSystemData->vMaximumBadData);
							vLocalSystemData->vMaximumBadData = NULL;
						}
						if (ThereIsEnoughMemory(cStandardDeviationSampleSize,"Maximum Bad Sample Array"))
							vLocalSystemData->vMaximumBadData = 
							(BYTE*)malloc(cStandardDeviationSampleSize);
						vLocalSystemData->vMaximumBadStandardDeviation = 0;
						vLocalSystemData->vMaximumBadAverage = 0;
						vLocalSystemData->vMaximumBadIntensity = 0;
						vLocalSystemData->vQualityFactor = 0;
					}

					if (vLocalSystemData->vMaximumBadData)
					{
						BYTE *TempMaximumBadDataPointer = vLocalSystemData->vMaximumBadData + 
							vLocalSystemData->vMaximumBadIndex;
						*TempMaximumBadDataPointer = (BYTE)TempMaximumBad;
					}
				}  //end if (TempMaximumBad < vLocalSystemData->vMinimumGoodIntensity)
			}
			//if created a red temporary buffer, dispose of it
			if (TempPointerToRedImageBuffer != TempContainer->vTheImage->vRedImage)
			if (TempPointerToRedImageBuffer)
				free(TempPointerToRedImageBuffer);
		}
		if (TempKernelImage)
			im_delete(TempKernelImage);
	}
}

void CImageAquisitionThread::CheckUnderfillByWeight(CContainer *TempContainer, CInspection *TempInspection, int TempInspectionNumber)
{
	if ((TempContainer) && (TempInspection))
	{
		//remove the IV Bag if need to
		if (TempInspection->vWhiteOutAnythingTouchingTheTop)
			TempInspection->WhiteOutAnythingTouchingTheTop(vLocalSystemData->vITIPCDig->vLatestImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

		//calculate the total weight of image
		vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber] = vLocalSystemData->vITIPCDig->CalculateUnderfillByWeight(vLocalSystemData->vITIPCDig->vLatestImage->pix, TempInspection, 0);

		double TempWeight = TempInspection->ConvertToWeight(vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber]);

		if (TempInspection->vCheckWeighOffset)
		{
			TempWeight = TempWeight + TempInspection->vCheckWeighOffset;
			vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber] = TempInspection->ConvertWeightToBrightness(TempWeight);
			TempContainer->vTheImage->vContainerWeightCorrection[TempInspectionNumber] = TempInspection->vCheckWeighOffset;
		}

		//valve monitoring
		if (vLocalSystemData->vValveData) 
		if (vLocalSystemData->vValveMonitorInspection == TempInspectionNumber)//need to save this valve value in temporary holder
			SaveDataInValveMonitor(TempContainer, TempWeight);


		// for testing
		//if (vLocalConfigurationData->vSimulateLightControlBoard)
		//if (!vLocalSystemData->vITIPCDig->vHaveRealImage)
		//{
		//	int TempRandomNumber = rand();
		//	vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber] = vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber] + 5;// + (5 * TempRandomNumber);
		//}
			
		if (vLocalSystemData->vLogFile.vLogSerialData)
		{
			vLocalSystemData->vLogFile.WriteToLogFile("Inspection: " + dtoa(TempInspectionNumber,0) + " Density: " + dtoa(vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber],2), cDebugMessage);

			vLocalSystemData->vLogFile.WriteToLogFile("Inspection: " + dtoa(TempInspectionNumber,0) + " Converted to Weight: " + dtoa(TempWeight,2), cDebugMessage);
		}
		//ReportErrorMessage("Container: " + dtoa(TempContainer->vContainerNumber, 0) + " Inspection: " + dtoa(TempInspectionNumber,0) + " Converted to Weight: " + dtoa(TempWeight,2), cWriteToLog, 0); // temp for testing

		//determine if it is a reject, and eject if have not ejected for previous inspection
		if ((TempInspection->vCheckWeighMFactor != 1) || (TempInspection->vCheckWeighBFactor)) //Threshold is set and have a conversion formula
		if ((TempInspection->vInspectionType == cOverfillByWeightInspection) && (TempInspection->vDensityThresholdLower))
		{
			vLocalSystemData->vThisContainerShouldBeEjected = (TempInspection->vDensityThresholdLower < vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber]);
			CInspectionCheckWeigh *TempCWInspection = NULL;
				
			if (TempInspection->vInspectionNameForWeightTrendingPointer)
				TempCWInspection = (CInspectionCheckWeigh *)TempInspection->vInspectionNameForWeightTrendingPointer;
			else
				TempCWInspection = (CInspectionCheckWeigh *)TempInspection;

			TempCWInspection->vTotalOver++;

			if (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInvertEjectLogic)
				vLocalSystemData->vThisContainerShouldBeEjected = !vLocalSystemData->vThisContainerShouldBeEjected;

			if (vLocalSystemData->vThisContainerShouldBeEjected)
			{
				if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
					TempContainer->vTheImage->vContainerEjectThreshold = TempInspection->vDensityThresholdLower;

				InspectionFoundContaminant(TempInspectionNumber);

				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString + " Over Weight Found",cDebugMessage);
			}
		}
		else
		if ((TempInspection->vInspectionType == cUnderfillByWeightInspection) && (TempInspection->vDensityThresholdUpper))
		{
			vLocalSystemData->vThisContainerShouldBeEjected = (TempInspection->vDensityThresholdUpper > vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber]);

			CInspectionCheckWeigh *TempCWInspection = NULL;
				
			if (TempInspection->vInspectionNameForWeightTrendingPointer)
				TempCWInspection = (CInspectionCheckWeigh *)TempInspection->vInspectionNameForWeightTrendingPointer;
			else
				TempCWInspection = (CInspectionCheckWeigh *)TempInspection;

			TempCWInspection->vTotalUnder++;

			if (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInvertEjectLogic)
				vLocalSystemData->vThisContainerShouldBeEjected = !vLocalSystemData->vThisContainerShouldBeEjected;

			if (vLocalSystemData->vThisContainerShouldBeEjected)
			{
				if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
					TempContainer->vTheImage->vContainerEjectThreshold = TempInspection->vDensityThresholdUpper;

				InspectionFoundContaminant(TempInspectionNumber);

				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString + " Under Weight Found",cDebugMessage);
			}
		}

		//save weight data in data arrays
		//if (!TempEjected)  //alex decided to include all container weights in trend data
		if (vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber] >= 0) 
		//if (TempInspection->vInspectionType == cCheckWeighInspection)
		if (TempInspection->vInspectionNameForWeightTrendingName.GetLength() == 0) //don't do if pointing at another inspection for weight trending
		{
			CInspectionCheckWeigh *TempCheckWeighInspection = (CInspectionCheckWeigh *)TempInspection;
			TempCheckWeighInspection->vLastWeight = vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber]; 

			//count if was a low or high sample
			//if (TempCheckWeighInspection->vCheckWeighTrendAlarmMinimum)
			//if (vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber] < TempCheckWeighInspection->vCheckWeighTrendAlarmMinimum)
			//	TempCheckWeighInspection->vTotalUnder++;

			//if (TempCheckWeighInspection->vCheckWeighTrendAlarmMaximum)
			//if (vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber] > TempCheckWeighInspection->vCheckWeighTrendAlarmMaximum)
			//	TempCheckWeighInspection->vTotalOver++;

			//save weight in current minute data array
			if (TempCheckWeighInspection->vCheckWeighCurrentMinuteNumberOfDataSamplesTaken < cCheckWeighMaximumSamplesInOneMinute)
			{
				double *TempDataPointer = TempCheckWeighInspection->vCheckWeighCurrentMinuteData + (TempCheckWeighInspection->vCheckWeighCurrentMinuteNumberOfDataSamplesTaken);

				*TempDataPointer = vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber];
				TempCheckWeighInspection->vCheckWeighCurrentMinuteNumberOfDataSamplesTaken++;
			}

			//save weight in recent 500 array
			if (TempCheckWeighInspection->vCheckWeighDataIndex < 0)  //is set to -1 when data cleared
				TempCheckWeighInspection->vCheckWeighDataIndex = 0;

			double *TempDataPointer = TempCheckWeighInspection->vCheckWeighData + (TempCheckWeighInspection->vCheckWeighDataIndex);

			*TempDataPointer = vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber];

			if (TempCheckWeighInspection->vCheckWeighNumberOfDataSamplesTaken < TempCheckWeighInspection->vCheckWeighDataSize)
				TempCheckWeighInspection->vCheckWeighNumberOfDataSamplesTaken++;

			if (TempCheckWeighInspection->vCheckWeighNumberOfSamplesTakenForAlarm < TempCheckWeighInspection->vCheckWeighDataSize)
				TempCheckWeighInspection->vCheckWeighNumberOfSamplesTakenForAlarm++;
			
			if (TempCheckWeighInspection->vCheckWeighDataIndex < TempCheckWeighInspection->vCheckWeighDataSize - 1)
				TempCheckWeighInspection->vCheckWeighDataIndex++;
			else
			{
				TempCheckWeighInspection->vCheckWeighDataIndex = 0;
				//create trend data sample Point
				if (TempCheckWeighInspection->vCheckWeighTrendData)
				{
					if (TempCheckWeighInspection->vCheckWeighTrendDataIndex < 0)  //is set to -1 when data cleared
						TempCheckWeighInspection->vCheckWeighTrendDataIndex = 0;

					double TempAverage = 0;
					TempDataPointer = TempCheckWeighInspection->vCheckWeighData;

					for (WORD TempLoop = 0; TempLoop < TempCheckWeighInspection->vCheckWeighDataSize; TempLoop++)
						TempAverage = TempAverage + *TempDataPointer++;

					TempAverage = TempAverage / TempCheckWeighInspection->vCheckWeighDataSize;

					TempDataPointer = TempCheckWeighInspection->vCheckWeighTrendData + (TempCheckWeighInspection->vCheckWeighTrendDataIndex);

					*TempDataPointer = TempAverage;

					if (TempCheckWeighInspection->vCheckWeighTrendDataIndex < TempCheckWeighInspection->vCheckWeighTrendDataSize - 1)
						TempCheckWeighInspection->vCheckWeighTrendDataIndex++;
					else
						TempCheckWeighInspection->vCheckWeighTrendDataIndex = 0;

					if (TempCheckWeighInspection->vCheckWeighTrendNumberOfDataSamplesTaken < TempCheckWeighInspection->vCheckWeighTrendDataSize)
						TempCheckWeighInspection->vCheckWeighTrendNumberOfDataSamplesTaken++;

					double TempCheckWeight = 0;
					double TempCheckWeighAverage = 0; //for checkweighing, see if should signal an alarm on underfill
					if ((TempInspection->vCheckWeighMFactor != 1) || (TempInspection->vCheckWeighBFactor)) //Threshold is set and have a conversion formula
					{
						CInspectionCheckWeigh *TempCheckWeighInspection = (CInspectionCheckWeigh *)TempInspection;

						//Calculate the running average of the Weight Trending inspection
						double *TempPointer = TempCheckWeighInspection->vCheckWeighData;

						for (WORD TempLoop = 0; TempLoop < TempCheckWeighInspection->vCheckWeighDataSize; TempLoop++)
							TempCheckWeighAverage = TempCheckWeighAverage + *TempPointer++;

						if (TempCheckWeighInspection->vCheckWeighDataSize)
							TempCheckWeighAverage = TempCheckWeighAverage / TempCheckWeighInspection->vCheckWeighDataSize;

						if (TempCheckWeighAverage)
						{
							//ReportErrorMessage(TempCheckWeighInspection->vName + " - Trending a Point: " + dtoa(TempCheckWeighAverage, 2), cAction,0); // for testing
							TempCheckWeighAverage = TempCheckWeighInspection->ConvertToWeight(TempCheckWeighAverage);
							ReportErrorMessage("Info Only-" + TempCheckWeighInspection->vName + " - Trending Point: " + dtoa(TempCheckWeighAverage, 2), cAction,0);
							if (TempCheckWeighInspection->vCheckWeighTrendAlarmMinimum)
							{
								if (!TempCheckWeighInspection->vCheckWeighAlarm2UnderWeightOn)
								{
									if (TempCheckWeighAverage < TempCheckWeighInspection->ConvertToWeight(TempCheckWeighInspection->vCheckWeighTrendAlarmMinimum))
									{
										TempCheckWeighInspection->vNumberOfAlarms++;
										TempCheckWeighInspection->vCheckWeighAlarmLatched = true;
										TempCheckWeighInspection->vCheckWeighAlarm2UnderWeightOn = true;
										vLocalSystemData->vWeightTrendingAlarm = true;
										vMainWindowPointer->SetInterlockStatus();
										vGlobaluCSerialPort->TurnCanStopOnOff(1, true);//turn on alarm for Under Weight Check Weigh
										ReportErrorMessage(TempCheckWeighInspection->vName + " - Trending Minimum Limit: " + dtoa(TempCheckWeighInspection->ConvertToWeight(TempCheckWeighInspection->vCheckWeighTrendAlarmMinimum), 2), cEMailPlantOperations,0);
										ReportErrorMessage(TempCheckWeighInspection->vName + " - Trending Under Weight: " + dtoa(TempCheckWeighAverage, 2), cEMailPlantOperations,0);
									}
								}
								else //alarm is currently on
								{
									if (TempCheckWeighAverage > TempCheckWeighInspection->ConvertToWeight(TempCheckWeighInspection->vCheckWeighTrendAlarmMinimum))
									{
										TempCheckWeighInspection->vCheckWeighAlarm2UnderWeightOn = false;

										if (vLocalConfigurationData->vQAPassword == "0") //clear alarm automatically if set QA Password to zero
											vLocalSystemData->vWeightTrendingAlarm = false; 

										vMainWindowPointer->SetInterlockStatus();
										ReportErrorMessage(TempCheckWeighInspection->vName + " - Trending Normal after Under Weight: " + dtoa(TempCheckWeighAverage, 2), cEMailPlantOperations,0);
										vGlobaluCSerialPort->TurnCanStopOnOff(1, false);//turn off alarm for Under Weight Check Weigh
									}
								}
							}

							if (TempCheckWeighInspection->vCheckWeighTrendAlarmMaximum) //for checkweighing, see if should signal an alarm on Overfill
							if (!TempCheckWeighInspection->vCheckWeighAlarm2OverWeightOn) //if alarm not currently on
							{
								if (TempCheckWeighAverage > TempCheckWeighInspection->ConvertToWeight(TempCheckWeighInspection->vCheckWeighTrendAlarmMaximum))
								{
									TempCheckWeighInspection->vNumberOfAlarms++;
									TempCheckWeighInspection->vCheckWeighAlarmLatched = true;
									TempCheckWeighInspection->vCheckWeighAlarm2OverWeightOn = true;
									vLocalSystemData->vWeightTrendingAlarm = true;
									vMainWindowPointer->SetInterlockStatus();
									vGlobaluCSerialPort->TurnCanStopOnOff(2, true);//turn on alarm for Over Weight Check Weigh
									ReportErrorMessage(TempCheckWeighInspection->vName + " - Trending Maximum Limit: " + dtoa(TempCheckWeighInspection->ConvertToWeight(TempCheckWeighInspection->vCheckWeighTrendAlarmMaximum), 2), cEMailPlantOperations,0);
									ReportErrorMessage(TempCheckWeighInspection->vName + " - Trending Over Weight: " + dtoa(TempCheckWeighAverage, 2), cEMailPlantOperations,0);
								}
							}
							else
							if (TempCheckWeighAverage < TempCheckWeighInspection->ConvertToWeight(TempCheckWeighInspection->vCheckWeighTrendAlarmMaximum))
							{
								TempCheckWeighInspection->vCheckWeighAlarm2OverWeightOn = false;

								if (vLocalConfigurationData->vQAPassword == "0") //clear alarm automatically if set QA Password to zero
									vLocalSystemData->vWeightTrendingAlarm = false; 

								vMainWindowPointer->SetInterlockStatus();
								ReportErrorMessage(TempCheckWeighInspection->vName + " - Trending Normal after Over Weight: " + dtoa(TempCheckWeighAverage, 2), cEMailPlantOperations,0);
								vGlobaluCSerialPort->TurnCanStopOnOff(2, false);//turn off alarm for Over Weight Check Weigh
							}
						}
					}
				}
			}
		}

		if (vLocalSystemData->vLogFile.vLogSerialData)
		{
			ReportErrorMessage(" Info Only-" + TempInspection->vName + " Container: " + dtoa(TempContainer->vContainerNumber, 0) + " Weight: " + dtoa(TempWeight,2), cWriteToLog, 0);
			CString TempDirectoryString = cLogDirectory;
			TempDirectoryString = TempDirectoryString + "Container Weights.CSV";
			LPCTSTR TempFileName = TempDirectoryString;

			if (ThereIsEnoughMemory(sizeof(CFile), "Weights Log File"))
			{
				CFileException TempFileException;
				CFile *TempWeightsFile = new CFile;
				if (TempWeightsFile)
				{
					if (TempWeightsFile->Open(TempFileName, (CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyNone), &TempFileException))
					{
						if ((TempWeightsFile->m_hFile) && (TempWeightsFile->m_hFile != INVALID_HANDLE_VALUE ))
						{
							TRY
							{
								TempWeightsFile->SeekToEnd();
							}
							CATCH_ALL(TempFileException)
							{
								ReportErrorMessage("Failed SeekToEnd of Weight File", cEMailInspx, 32000);
							}
							END_CATCH_ALL

							CString TempStringForFile = dtoa(TempContainer->vContainerNumber, 0) + ",  " + dtoa(TempWeight,2) + "\n";
							WORD TempLength = TempStringForFile.GetLength();
							if (TempLength)
							{
								CW2A TempStringToWrite(TempStringForFile);
								TempWeightsFile->Write(TempStringToWrite, TempLength);
							}
							TempWeightsFile->Close();
						}
						else
							ReportErrorMessage("Failed to Write Weight to Disk",cEMailInspx, 32322);
					}
					else
						ReportErrorMessage("Failed to Write Weight to Disk2",cEMailInspx, 32322);
					delete TempWeightsFile;
				}
			}
		}
	}
	if (TempInspection->vCollectingDensityAverage)  //if collecting average
	if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
	if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cPassSlippedContainer)//slipped
		vLocalSystemData->vWrongWidthsDuringLearn++;
	else
	if ((vSizeOK) || (vNumberOfWrongSizeInARow > 50))
	{
		if (vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber] >= 0)
		{
			if (vLocalSystemData->vDensityIndex == 0)  
			{//if first time through collecting averageinitialize array
				if (vLocalSystemData->vDensityDataD)
				{
					free(vLocalSystemData->vDensityDataD);
					vLocalSystemData->vDensityDataD = NULL;
				}
				if (ThereIsEnoughMemory(cStandardDeviationSampleSize * 8,"Density Sample Array"))
				{
					vLocalSystemData->vDensityDataD = (double*)malloc(cStandardDeviationSampleSize * 8);
				}
			}

			if (vLocalSystemData->vDensityDataD)
			{
				double TempWeight = vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber];
				if (TempWeight < vLocalSystemData->vITIPCDig->vDensityMinimumValue)
					vLocalSystemData->vITIPCDig->vDensityMinimumValue = TempWeight;

				if (TempWeight > vLocalSystemData->vITIPCDig->vDensityMaximumValue)
					vLocalSystemData->vITIPCDig->vDensityMaximumValue = TempWeight;

				double *TempDataPointer = vLocalSystemData->vDensityDataD + vLocalSystemData->vDensityIndex;

				*TempDataPointer = vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber];

				if (vLocalSystemData->vDensityIndex < (cStandardDeviationSampleSize - 1))
					vLocalSystemData->vDensityIndex++;

				if (cStandardDeviationSampleSize <= vLocalSystemData->vDensityIndex)
				{
					TempInspection->vCollectingDensityAverage = 0;
				}
				//calculate Average and Standard Deviation
				if (vLocalSystemData->vDensityIndex > 3)
				{
					vLocalSystemData->vITIPCDig->vDensityStandardDeviation = 
						CalculateStandardDeviation(vLocalSystemData->vDensityDataD, vLocalSystemData->vDensityIndex, &vLocalSystemData->vITIPCDig->vDensityAverage);
					if (vLocalSystemData->vITIPCDig->vDensityStandardDeviation < 2)
						vLocalSystemData->vITIPCDig->vDensityStandardDeviation = 2;
				}
			}
		}
	}
}

void CImageAquisitionThread::CheckDensity(CContainer *TempContainer, CInspection *TempInspection, int TempInspectionNumber)
{
	vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber] = 
		(float)vLocalSystemData->vITIPCDig->CalculateInspectionDensity(vLocalSystemData->vITIPCDig->vLatestImage->pix, TempInspection);
			
	//valve monitoring
	if (vLocalSystemData->vValveData) 
	if (vLocalSystemData->vValveMonitorInspection == TempInspectionNumber)//need to save this valve value in temporary holder
		SaveDataInValveMonitor(TempContainer, vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber]);

	if (vLocalSystemData->vLogFile.vLogSerialData)
	{
		vLocalSystemData->vLogFile.WriteToLogFile("Inspection: " + dtoa(TempInspectionNumber,0) + " Density: " + 
			dtoa(vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber],2), cDebugMessage);
	}

	if (vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber] >= 0)
	{
		if (TempInspection->vDensityThresholdUpper)
		{
			double TempThreshold = TempInspection->vDensityThresholdUpper;
			if (TempInspection->vEnable)
			{
				vLocalSystemData->vThisContainerShouldBeEjected = (TempThreshold < vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber]);

				if (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInvertEjectLogic)
					vLocalSystemData->vThisContainerShouldBeEjected = !vLocalSystemData->vThisContainerShouldBeEjected;

				if (vLocalSystemData->vThisContainerShouldBeEjected)
				{
					if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
						TempContainer->vTheImage->vContainerEjectThreshold = (float)TempThreshold;

					InspectionFoundContaminant(TempInspectionNumber);

					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString + " Under Weight Found",cDebugMessage);
				}
			}
		}
		if (TempInspection->vDensityThresholdLower)
		{
			double TempThreshold = TempInspection->vDensityThresholdLower;
			if (TempInspection->vEnable)
			{
				vLocalSystemData->vThisContainerShouldBeEjected = (TempThreshold > vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber]);

				if (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInvertEjectLogic)
					vLocalSystemData->vThisContainerShouldBeEjected = !vLocalSystemData->vThisContainerShouldBeEjected;

				if (vLocalSystemData->vThisContainerShouldBeEjected)
				{
					if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
						TempContainer->vTheImage->vContainerEjectThreshold = (float)TempThreshold;

					InspectionFoundContaminant(TempInspectionNumber);

					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString + " Under Weight Found",cDebugMessage);
				}
			}
		}
		if (vLocalSystemData->vLogFile.vLogSerialData)
		{
			ReportErrorMessage(" Info Only-" + TempInspection->vName + " Container: " + dtoa(TempContainer->vContainerNumber, 0) + " Density: " + dtoa(vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber],2), cWriteToLog, 0);
			CString TempDirectoryString = cLogDirectory;
			TempDirectoryString = TempDirectoryString + "Container Densities.CSV";
			LPCTSTR TempFileName = TempDirectoryString;

			if (ThereIsEnoughMemory(sizeof(CFile), "Density Log File"))
			{
				CFileException TempFileException;
				CFile *TempWeightsFile = new CFile;
				if (TempWeightsFile)
				{
					if (TempWeightsFile->Open(TempFileName, (CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyNone), &TempFileException))
					{
						if ((TempWeightsFile->m_hFile) && (TempWeightsFile->m_hFile != INVALID_HANDLE_VALUE ))
						{
							TRY
							{
								TempWeightsFile->SeekToEnd();
							}
							CATCH_ALL(TempFileException)
							{
								ReportErrorMessage("Failed SeekToEnd of Density File", cEMailInspx, 32000);
							}
							END_CATCH_ALL

							CString TempStringForFile = dtoa(TempContainer->vContainerNumber, 0) + ",  " + dtoa(vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber],2) + "\n";
							WORD TempLength = TempStringForFile.GetLength();
							if (TempLength)
							{
								CW2A TempStringToWrite(TempStringForFile);
								TempWeightsFile->Write(TempStringToWrite, TempLength);
							}
							TempWeightsFile->Close();
						}
						else
							ReportErrorMessage("Failed to Write Density to Disk",cEMailInspx, 32322);
					}
					else
						ReportErrorMessage("Failed to Write Density to Disk2",cEMailInspx, 32322);
					delete TempWeightsFile;
				}
			}
		}
	}
}

void CImageAquisitionThread::CheckStandardDeviation(CContainer *TempContainer, CInspection *TempInspection, int TempInspectionNumber)
{
	if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
	if (TempInspection)
	if ((vLocalSystemData->vITIPCDig->vLatestImage) && (vLocalSystemData->vITIPCDig->vLatestImage->pix))
	{
		double TempAverage = 0;

		if (TempInspection->vPreSmoothImage)
		{
			CopyMemory(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vLatestImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
			DoMedianSmooth(vLocalSystemData->vITIPCDig->vLatestImageCopy, TempInspection->vPreSmoothImage,TempInspection->vPreSmoothImage);
			vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber] = vLocalSystemData->vITIPCDig->CalculateInspectionStandardDeviation(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, TempInspection, &TempAverage);
		}
		else
			vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber] = vLocalSystemData->vITIPCDig->CalculateInspectionStandardDeviation(vLocalSystemData->vITIPCDig->vLatestImage->pix, TempInspection, &TempAverage);
			
		if (vLocalSystemData->vLogFile.vLogSerialData)
		{
			vLocalSystemData->vLogFile.WriteToLogFile("Inspection: " + dtoa(TempInspectionNumber,0) + " Texture: " + dtoa(vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber],2), cDebugMessage);
		}

		if (vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber] >= 0)
		if (TempInspection->vDensityThresholdUpper)
		{
			double TempThreshold = TempInspection->vDensityThresholdUpper;
			//if (vLocalConfigurationData->vDriftCompensationEnabled)
			//if (TempThreshold + (vLocalSystemData->vDriftCompensationThresholdIncreaseAmount) + 1 < 255)
			//	TempThreshold = TempThreshold + (vLocalSystemData->vDriftCompensationThresholdIncreaseAmount);
			vLocalSystemData->vThisContainerShouldBeEjected = (TempThreshold >= vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber]);

			if (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInvertEjectLogic)
				vLocalSystemData->vThisContainerShouldBeEjected = !vLocalSystemData->vThisContainerShouldBeEjected;

			if (vLocalSystemData->vThisContainerShouldBeEjected)
			{
				if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
					TempContainer->vTheImage->vContainerEjectThreshold = (float)TempThreshold;

				InspectionFoundContaminant(TempInspectionNumber);

				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString + " Under Weight Found",cDebugMessage);
			}
		}
		if (TempInspection->vDensityThresholdLower)
		if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
		if (!vLocalSystemData->vThisContainerShouldBeEjected)
		{
			double TempThreshold = TempInspection->vDensityThresholdLower;
			vLocalSystemData->vThisContainerShouldBeEjected = (TempThreshold <= vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber]);

			if (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInvertEjectLogic)
				vLocalSystemData->vThisContainerShouldBeEjected = !vLocalSystemData->vThisContainerShouldBeEjected;

			if (vLocalSystemData->vThisContainerShouldBeEjected)
			{
				if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
					TempContainer->vTheImage->vContainerEjectThreshold = (float)TempThreshold;

				InspectionFoundContaminant(TempInspectionNumber);

				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString + " Under Weight Found",cDebugMessage);
			}
		}
	}
}

void CImageAquisitionThread::AccumulateAverageStandardDeviation(CInspection *TempInspection, int TempInspectionNumber)
{
	if (TempInspection->vCollectingDensityAverage)  //if collecting average
	{
		double TempAverage = 0;
		vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber] = 
			vLocalSystemData->vITIPCDig->CalculateInspectionStandardDeviation(vLocalSystemData->vITIPCDig->vLatestImage->pix, TempInspection, &TempAverage);
		if (vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber] >= 0)
		{
			if (vLocalSystemData->vDensityIndex == 0)  
			{//if first time through collecting averageinitialize array
				if (vLocalSystemData->vDensityDataD)
				{
					free(vLocalSystemData->vDensityDataD);
					vLocalSystemData->vDensityDataD = NULL;
				}
				if (ThereIsEnoughMemory(cStandardDeviationSampleSize * 8,"Density Sample Array"))
				{
					vLocalSystemData->vDensityDataD = (double*)malloc(cStandardDeviationSampleSize * 8);
				}
			}

			if (vLocalSystemData->vDensityDataD)
			{
				if (vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber] < vLocalSystemData->vITIPCDig->vDensityMinimumValue)
					vLocalSystemData->vITIPCDig->vDensityMinimumValue = vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber];

				if (vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber] > vLocalSystemData->vITIPCDig->vDensityMaximumValue)
					vLocalSystemData->vITIPCDig->vDensityMaximumValue = vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber];

				double *TempDataPointer = vLocalSystemData->vDensityDataD + vLocalSystemData->vDensityIndex;

				*TempDataPointer = vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber];

				//calculate Average and Standard Deviation
				if (vLocalSystemData->vDensityIndex > 3)
				{
						vLocalSystemData->vITIPCDig->vDensityStandardDeviation = CalculateStandardDeviation(vLocalSystemData->vDensityDataD, vLocalSystemData->vDensityIndex, &vLocalSystemData->vITIPCDig->vDensityAverage);
						if (vLocalSystemData->vITIPCDig->vDensityStandardDeviation < 2)
							vLocalSystemData->vITIPCDig->vDensityStandardDeviation = 2;
				}
				if (vLocalSystemData->vDensityIndex < (cStandardDeviationSampleSize - 1))
					vLocalSystemData->vDensityIndex++;

				if (cStandardDeviationSampleSize <= vLocalSystemData->vDensityIndex)
				{
					TempInspection->vCollectingDensityAverage = 0;
				}
			}
		}
	}
}

void CImageAquisitionThread::AccumulateAverageDensity(CInspection *TempInspection, int TempInspectionNumber)
{
	if (TempInspection->vCollectingDensityAverage)  //if collecting average
	{
		vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber] = vLocalSystemData->vITIPCDig->CalculateInspectionDensity(vLocalSystemData->vITIPCDig->vLatestImage->pix, TempInspection);
		if (vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber] >= 0)
		{
			if (vLocalSystemData->vDensityIndex == 0)  
			{//if first time through collecting averageinitialize array
				if (vLocalSystemData->vDensityDataD)
				{
					free(vLocalSystemData->vDensityDataD);
					vLocalSystemData->vDensityDataD = NULL;
				}
				if (ThereIsEnoughMemory(cStandardDeviationSampleSize * 8,"Density Sample Array"))
				{
					vLocalSystemData->vDensityDataD = (double*)malloc(cStandardDeviationSampleSize * 8);
				}
			}

			if (vLocalSystemData->vDensityDataD)
			{
				if (vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber] < vLocalSystemData->vITIPCDig->vDensityMinimumValue)
					vLocalSystemData->vITIPCDig->vDensityMinimumValue = vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber];

				if (vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber] > vLocalSystemData->vITIPCDig->vDensityMaximumValue)
					vLocalSystemData->vITIPCDig->vDensityMaximumValue = vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber];

				double *TempDataPointer = vLocalSystemData->vDensityDataD + vLocalSystemData->vDensityIndex;

				*TempDataPointer = vLocalSystemData->vITIPCDig->vLastDensityValue[TempInspectionNumber];

				//calculate Average and Standard Deviation
				if (vLocalSystemData->vDensityIndex > 3)
				{
					vLocalSystemData->vITIPCDig->vDensityStandardDeviation = CalculateStandardDeviation(vLocalSystemData->vDensityDataD, vLocalSystemData->vDensityIndex, &vLocalSystemData->vITIPCDig->vDensityAverage);
					if (vLocalSystemData->vITIPCDig->vDensityStandardDeviation < .5)
						vLocalSystemData->vITIPCDig->vDensityStandardDeviation = .5;

					if (vLocalSystemData->vITIPCDig->vDensityAverage < 230)
					if (vLocalSystemData->vITIPCDig->vDensityStandardDeviation < 2)
						vLocalSystemData->vITIPCDig->vDensityStandardDeviation = 2;
				}
				if (vLocalSystemData->vDensityIndex < (cStandardDeviationSampleSize - 1))
					vLocalSystemData->vDensityIndex++;

				if (cStandardDeviationSampleSize <= vLocalSystemData->vDensityIndex)
				{
					TempInspection->vCollectingDensityAverage = 0;
				}
			}
		}
	}
}

void CImageAquisitionThread::AccumulateStructure(BYTE *TempPointerToImage)
{
	if (TempPointerToImage)
	{
		vLocalSystemData->vITIPCDig->vStructureCount++;
		if (vLocalConfigurationData->vStructureDisableCount > 0)
		if (vLocalSystemData->vITIPCDig->vStructureCount >= vLocalConfigurationData->vStructureDisableCount)
		{
			vLocalSystemData->vITIPCDig->vStructureEnabled = false;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				vLocalSystemData->vLogFile.WriteToLogFile("End Accumulating Structure",cDebugMessage);
			}
			::PostThreadMessage(vGlobalCallingThreadID,cCheckLearningMessage,0,0);
		}

		//accumulate contaminant structure
		if ((vLocalSystemData->vITIPCDig->vStructureImage) && (vLocalSystemData->vITIPCDig->vStructureImage->vGreyImage))
		{
			BYTE *TempPointerImageStructure = vLocalSystemData->vITIPCDig->vStructureImage->vGreyImage;
			BYTE *TempPointerAquisitionImage = TempPointerToImage;

			for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
			{//Take Minimum pixel to Structure
				if (*TempPointerImageStructure > *TempPointerAquisitionImage)
					*TempPointerImageStructure = *TempPointerAquisitionImage;  
				TempPointerImageStructure++;
				TempPointerAquisitionImage++;  
			}
		}
	}
}

void CImageAquisitionThread::AccumulateContaminant2Structure(BYTE *TempPointerToImage)
{
	//accumulate contaminant2 structure
	if (TempPointerToImage)
	if ((vLocalSystemData->vITIPCDig->vContaminant2StructureImage) && (vLocalSystemData->vITIPCDig->vContaminant2StructureImage->vGreyImage))
	{
		BYTE *TempPointerImageStructure = vLocalSystemData->vITIPCDig->vContaminant2StructureImage->vGreyImage;
		BYTE *TempPointerAquisitionImage = TempPointerToImage;

		for (DWORD TempLoop = 0; TempLoop < 
			vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
		{//Take Minimum pixel to Structure
			if (*TempPointerImageStructure > *TempPointerAquisitionImage)
				*TempPointerImageStructure = *TempPointerAquisitionImage;  
			TempPointerImageStructure++;
			TempPointerAquisitionImage++;  
		}
	}
}

void CImageAquisitionThread::AccumulateVoidStructure(BYTE *TempPointerToImage)
{
	//accumulate void structure
	if (TempPointerToImage)
	if ((vLocalSystemData->vITIPCDig->vVoidStructureImage) && (vLocalSystemData->vITIPCDig->vVoidStructureImage->vGreyImage))
	{
		BYTE *TempPointerImageStructure = vLocalSystemData->vITIPCDig->vVoidStructureImage->vGreyImage;
		BYTE *TempPointerAquisitionImage = TempPointerToImage;
		for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
		{//Take Maximum pixel to Structure
			if (*TempPointerAquisitionImage < *TempPointerImageStructure)
				*TempPointerImageStructure = *TempPointerAquisitionImage;  
			TempPointerImageStructure++;
			TempPointerAquisitionImage++;  
		}
	}
}

void CImageAquisitionThread::FindVoids(CContainer *TempContainer,
	 BYTE TempReferenceLoop, BYTE TempTopReferenceLoop, CInspection *TempInspection, 
	 BYTE TempInspectionNumber)
{
	WORD TempBottom = 0;
	WORD TempLeft = 0;
	WORD TempHeight = 0;
	WORD TempWidth = 0;
	vLocalSystemData->vITIPCDig->GetInspectionROIBounds(TempInspection, 
		&TempBottom, &TempLeft, &TempHeight, &TempWidth, 0, //already referenced vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust, 
		vLocalConfigurationData->vReferenceROIsToEdges, 0);

	//subtract the product's average image from this one if enabled
	if (TempInspection->vSubtractAverage)
	if (vLocalSystemData->vITIPCDig->vProductAverageImage)
		vLocalSystemData->vITIPCDig->SubtractVoidProductAverageImage(
		vLocalSystemData->vITIPCDig->vLatestImageCopy->pix);

	//add the product's structure image to this one if enabled
	if (TempInspection->vAddStructure)
	if (vLocalSystemData->vITIPCDig->vVoidProductStructureImage)
		vLocalSystemData->vITIPCDig->AddVoidProductStructureImage(
		vLocalSystemData->vITIPCDig->vLatestImageCopy->pix);

	if (TempInspection->vEnhanceMethod)
	{
		double TempStartFindTime = CIFCOS::GetSystimeMicrosecs();
		vLocalSystemData->vITIPCDig->PerformEnhanceOnImage(vLocalSystemData->vITIPCDig->vLatestImageCopy, vLocalSystemData->vITIPCDig->vIATScratchImage, TempInspection->vEnhanceMethod);
		double 	TempEndTime = CIFCOS::GetSystimeMicrosecs();
		vLocalSystemData->vITIPCDig->vLastEnhanceTime = TempEndTime - TempStartFindTime;
	}

	//if (TempInspection->vDoImageFactors)
	//	vLocalSystemData->vITIPCDig->CombineImages(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, TempInspection->vEnhancedImageFactor, vLocalSystemData->vITIPCDig->vLatestImage->pix, TempInspection->vOriginalImageFactor);

	//Add Product's Histogram image to this one if enabled
	if ((TempInspection->vAddHistogram) && (vLocalSystemData->vITIPCDig->vProductHistogramImage))
		vLocalSystemData->vITIPCDig->AddProductHistogramImage(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix);

	if (TempInspection->vFindGoodIntensity)
	if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
	if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cPassSlippedContainer)//slipped
		vLocalSystemData->vWrongWidthsDuringLearn++;
	else
	if ((vSizeOK) || (vNumberOfWrongSizeInARow > 50))
	{
		//find minimum good for this frame
		BYTE TempMinimumGood = 255;
		BYTE *TempPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix;
		WORD TempStartX = 0;
		WORD TempEndX = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
		WORD TempStartY = 0;
		WORD TempEndY = vLocalSystemData->vITIPCDig->vAquisitionSizeY;
		if ((TempInspection->vROIPixelLeft) || (TempInspection->vROIPixelBottom))
		{
			TempStartX = TempInspection->vROIPixelBottom;
			TempEndX = TempInspection->vROIPixelTop;
			TempStartY = TempInspection->vROIPixelLeft;
			TempEndY = TempInspection->vROIPixelRight;
		}
		for (WORD TempLoopY = TempStartY; TempLoopY < TempEndY; TempLoopY++)
		{
			TempPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix + 
				TempStartX +
				(TempLoopY * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
			for (WORD TempLoopX = TempStartX; TempLoopX < TempEndX; TempLoopX++)
			{
				if (*TempPointer < TempMinimumGood)
				if (*TempPointer > 1)  //br ht change to 125
					TempMinimumGood = *TempPointer;
				TempPointer++;
			}
		}
		//first time throught, initialize data structures
		if (!vLocalSystemData->vMinimumGoodIndex)
		if (!TempReferenceLoop)
		{
			if (vLocalSystemData->vMinimumGoodData)
			{
				free(vLocalSystemData->vMinimumGoodData);
				vLocalSystemData->vMinimumGoodData = NULL;
			}
			if (ThereIsEnoughMemory(cStandardDeviationSampleSize,"Minimum Good Sample Array"))
				vLocalSystemData->vMinimumGoodData = (BYTE*)malloc(cStandardDeviationSampleSize);
			vLocalSystemData->vMinimumGoodStandardDeviation = 0;
			vLocalSystemData->vMinimumGoodAverage = 0;
			vLocalSystemData->vMinimumGoodIntensity = 256;
		}

		if (vLocalSystemData->vMinimumGoodData)
		{
			BYTE *TempMinimumGoodDataPointer = vLocalSystemData->vMinimumGoodData + vLocalSystemData->vMinimumGoodIndex;
			*TempMinimumGoodDataPointer = TempMinimumGood;
		}
	}
	if (TempInspection->vFindBadIntensity)
	if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
	if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cPassSlippedContainer)//slipped
		vLocalSystemData->vWrongWidthsDuringLearn++;
	else
	if ((vSizeOK) || (vNumberOfWrongSizeInARow > 50))
	{
		//find minimum bad for this frame
		BYTE TempMaximumBad = 255;
		BYTE *TempPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix;
		WORD TempStartX = 0;
		WORD TempEndX = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
		WORD TempStartY = 0;
		WORD TempEndY = vLocalSystemData->vITIPCDig->vAquisitionSizeY;
		if ((TempInspection->vROIPixelLeft) || 
			(TempInspection->vROIPixelBottom))
		{
			TempStartX = TempInspection->vROIPixelBottom;
			TempEndX = TempInspection->vROIPixelTop;
			TempStartY = TempInspection->vROIPixelLeft;
			TempEndY = TempInspection->vROIPixelRight;
		}
		for (WORD TempLoopY = TempStartY; TempLoopY < TempEndY; TempLoopY++)
		{
			TempPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix + 
				TempStartX +
				(TempLoopY * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
			for (WORD TempLoopX = TempStartX; TempLoopX < TempEndX; TempLoopX++)
			{
				if (*TempPointer < TempMaximumBad)
					TempMaximumBad = *TempPointer;
				TempPointer++;
			}
		}

		if (TempMaximumBad == 0)  //zero does not change values for screen write, so make a little different
			TempMaximumBad = 1;

		if ((!vLocalSystemData->vSendingGoodWithBad) ||
			(TempMaximumBad < vLocalSystemData->vMinimumGoodIntensity))
		{  // if the bad intensity is below the good level
			//first time throught, initialize data structures
			if (!vLocalSystemData->vMaximumBadIndex)
			if (!TempReferenceLoop)
			{
				vLocalSystemData->vMaximumBadCount = 0;
				if (vLocalSystemData->vMaximumBadData)
				{
					free(vLocalSystemData->vMaximumBadData);
					vLocalSystemData->vMaximumBadData = NULL;
				}
				if (ThereIsEnoughMemory(cStandardDeviationSampleSize,"Maximum Bad Sample Array"))
					vLocalSystemData->vMaximumBadData = (BYTE*)malloc(cStandardDeviationSampleSize);
				vLocalSystemData->vMaximumBadStandardDeviation = 0;
				vLocalSystemData->vMaximumBadAverage = 0;
				vLocalSystemData->vMaximumBadIntensity = 0;
				vLocalSystemData->vQualityFactor = 0;
			}

			if (vLocalSystemData->vMaximumBadData)
			{
				BYTE *TempMaximumBadDataPointer = vLocalSystemData->vMaximumBadData + vLocalSystemData->vMaximumBadIndex;
				*TempMaximumBadDataPointer = TempMaximumBad;
			} // end switch (TempReferenceLoop)
		}
	}

	BYTE TempThreshold = TempInspection->vThreshold;
	if (vLocalConfigurationData->vDemoMode)
	{
		vLocalSystemData->vITIPCDig->ThresholdBuffer(
			vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, 
			vLocalSystemData->vITIPCDig->vOriginalBufferSize,
			TempInspection->vThreshold);
	}
	else
	{
		if ((vLocalSystemData->vITIPCDig->vImageForPseudoColor) && (vLocalSystemData->vITIPCDig->vImageForPseudoColor->pix))
		{
			CopyMemory(vLocalSystemData->vITIPCDig->vIATScratchImage->pix, vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

			//must blank off area outside ROI
			//blank left edge
			if (TempLeft)
			{
				DWORD TempCount = TempLeft * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
				FillMemory(vLocalSystemData->vITIPCDig->vIATScratchImage->pix, TempCount, 255);
			}

			//blank Right edge
			WORD TempRight = TempLeft + TempWidth;
			if (TempRight < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1)
			{
				DWORD TempStart = TempRight * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
				DWORD TempCount = vLocalSystemData->vITIPCDig->vOriginalBufferSize - TempStart - 1;
				FillMemory(vLocalSystemData->vITIPCDig->vIATScratchImage->pix + TempStart, TempCount, 255);
			}

			//blank Bottom edge
			if (TempBottom)
			{
				DWORD TempStart = TempLeft * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
				DWORD TempCount = TempBottom;
				for (WORD TempLoop = 0; TempLoop < TempWidth; TempLoop++)
					FillMemory(vLocalSystemData->vITIPCDig->vIATScratchImage->pix + TempStart + 
						(TempLoop * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX), TempCount, 255);
			}

			//blank Top edge
			WORD TempTop = TempBottom + TempHeight;
			if (TempTop < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1)
			{
				DWORD TempStart = (TempLeft * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX) + TempTop;
				DWORD TempCount = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - TempTop;
				for (WORD TempLoop = 0; TempLoop < TempWidth; TempLoop++)
					FillMemory(vLocalSystemData->vITIPCDig->vIATScratchImage->pix + TempStart + 
						(TempLoop * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX), TempCount, 255);
			}

			BYTE *TempThisInspectionPseudoColor = vLocalSystemData->vITIPCDig->vIATScratchImage->pix;
			BYTE *TempThisContainerPseudoColor = vLocalSystemData->vITIPCDig->vImageForPseudoColor->pix;
			for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
			{
				if (*TempThisInspectionPseudoColor < *TempThisContainerPseudoColor)
					*TempThisContainerPseudoColor = *TempThisInspectionPseudoColor;
				TempThisInspectionPseudoColor++;
				TempThisContainerPseudoColor++;
			}
		}

		ThresholdImage(vLocalSystemData->vITIPCDig->vLatestImageCopy, TempThreshold);
	}

	double TempStartFindBlobsTime = CIFCOS::GetSystimeMicrosecs();
	//check for voids
	
	if (vLocalConfigurationData->vDemoMode)
	{
		//any zero value is a blob
		BYTE *TempPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix;
		for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vLatestImageCopy->dy; TempLoopY++)
		{
			for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vLatestImageCopy->dx; TempLoopX++)
			{
				if (!*TempPointer++)
				{
					vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = 1;
					//vLocalSystemData->vLastContainerImaged->vTheImage->vRejectedForThisInspection[TempInspectionNumber] = true;
					TempLoopX = vLocalSystemData->vITIPCDig->vLatestImageCopy->dx;
					TempLoopY = vLocalSystemData->vITIPCDig->vLatestImageCopy->dy;
				}
			}
			TempPointer = TempPointer +
				vLocalSystemData->vITIPCDig->vLatestImageCopy->ypitch;
		}
		//if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
		if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
		vLocalSystemData->vThisContainerShouldBeEjected = (vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] > 0);

		if (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInvertEjectLogic)
			vLocalSystemData->vThisContainerShouldBeEjected = !vLocalSystemData->vThisContainerShouldBeEjected;

		if (vLocalSystemData->vThisContainerShouldBeEjected)
		{
			if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
				TempContainer->vTheImage->vContainerEjectThreshold = (float)TempThreshold;

			InspectionFoundContaminant(TempInspectionNumber);

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString + " Void Found",cDebugMessage);
		}
		/*
		else
		{
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString +
						" No Void",cDebugMessage);
			vLocalSystemData->vThisContainerShouldBeEjected = false;
		}
		*/
		if (vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber])
		if (TempReferenceLoop == 0)
		//if evaluating, don't eject this container if already ejected
		if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
		if (TempInspection->vEnable)
		{
			if ((vLocalSystemData->vITIPCDig->vImageForPseudoColor) && (vLocalSystemData->vITIPCDig->vImageForPseudoColor->pix))
				vLocalSystemData->vITIPCDig->SaveRedImageForDisplay(vLocalSystemData->vITIPCDig->vImageForPseudoColor, vLocalSystemData->vLastContainerImaged);

			vLocalSystemData->vITIPCDig->Save10RedBoxesForDisplay(TempInspection, vLocalSystemData->vITIPCDig->vBlobResults,
				vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust);
		}
	} //end demo mode code
	else //void
	{  //real mode (not demo) code
		vLocalSystemData->vITIPCDig->vLatestSubImage = im_rroi(vLocalSystemData->vITIPCDig->vLatestImageCopy, TempBottom, TempLeft, TempHeight, TempWidth);

		if (vLocalSystemData->vITIPCDig->vLatestSubImage)
		{
			vLocalSystemData->vITIPCDig->vBlobParameters->min_area = TempInspection->vSize;
			vLocalSystemData->vITIPCDig->vBlobParameters->blob_color = 0x00;  //fewer xrays

			Erflag TempResult = mvt_blob_find(vLocalSystemData->vITIPCDig->vLatestSubImage, vLocalSystemData->vITIPCDig->vBlobParameters, vLocalSystemData->vITIPCDig->vBlobResults, MVT_CF_DEFAULT);

			if (TempResult != IM_OK)
				ReportErrorMessage("MVT_Blob Error",cEMailInspx,32000);

			double TempArea = 0;
			if ((WORD)vLocalSystemData->vITIPCDig->vBlobResults->num_blobs_found)
			{
				for (DWORD TempLoop = 0; TempLoop < (WORD)vLocalSystemData->vITIPCDig->vBlobResults->num_blobs_found; TempLoop++)
				{
					TempContainer->vTheImage->vContainerLargestContaminant = 0;
					WORD TempBlobArea = (WORD)mvt_blob_get_area(vLocalSystemData->vITIPCDig->vBlobResults, TempLoop);
					if (TempBlobArea > TempContainer->vTheImage->vContainerLargestContaminant)
						TempContainer->vTheImage->vContainerLargestContaminant = TempBlobArea;
					TempArea = TempArea + TempBlobArea;
				}
				if (TempInspection->vAddStructure)
				if (vLocalConfigurationData->vAutoImproveEnabled)
					TempContainer->vTheImage->vContainerLearnState = vGlobalLearnState;

				vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason = TempInspectionNumber + cEjectInspectionOffset;
				vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectInspection = TempInspectionNumber;

				if (TempInspection->vAddStructure)
				if (vLocalConfigurationData->vAutoImproveEnabled)
					TempContainer->vTheImage->vContainerLearnState = vGlobalLearnState;

				if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
					TempContainer->vTheImage->vContainerEjectThreshold = TempThreshold;

				vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = (WORD)TempArea;
				if (vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber])
				if (TempReferenceLoop == 0)
				//if evaluating, don't eject this container if already ejected
				if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
				if (TempInspection->vEnable)
				{
					if ((vLocalSystemData->vITIPCDig->vImageForPseudoColor->pix) && (vLocalSystemData->vITIPCDig->vImageForPseudoColor->pix))
						vLocalSystemData->vITIPCDig->SaveRedImageForDisplay(vLocalSystemData->vITIPCDig->vImageForPseudoColor, vLocalSystemData->vLastContainerImaged);

					vLocalSystemData->vITIPCDig->Save10RedBoxesForDisplay(TempInspection, vLocalSystemData->vITIPCDig->vBlobResults,
						vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust);
				}
			}

			if ((vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] > 0) && (!vLocalSystemData->vLastContainerImaged->vToBeEjected))
			{
				//mark that you need to eject this for void based on processing so far
				//but may need to check other reference adjustments of this image first
				vLocalSystemData->vThisContainerShouldBeEjected = true;
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString + " Void Found",cDebugMessage);
			}
			else
			{
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString + " No Void",cDebugMessage);
				vLocalSystemData->vThisContainerShouldBeEjected = false;
			}

			if (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vInvertEjectLogic)
				vLocalSystemData->vThisContainerShouldBeEjected = !vLocalSystemData->vThisContainerShouldBeEjected;

			if (vLocalSystemData->vThisContainerShouldBeEjected)
			{
				if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
					TempContainer->vTheImage->vContainerEjectThreshold = (float)TempThreshold;

				InspectionFoundContaminant(TempInspectionNumber);

				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString + " Void Found",cDebugMessage);
			}

			im_delete(vLocalSystemData->vITIPCDig->vLatestSubImage);
			vLocalSystemData->vITIPCDig->vLatestSubImage = NULL;
		}
		else
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString +
						" Error, MVT Could not create Image for Blobs",cDebugMessage);
	}

	double TempEndTime = CIFCOS::GetSystimeMicrosecs();
	vLocalSystemData->vITIPCDig->vLastFindBlobsTime = TempEndTime - TempStartFindBlobsTime;


	if (vLocalConfigurationData->vDemoMode)
	{
		bool TempInBlob = false;
		BYTE *TempPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix;
		for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
		{
			if (*TempPointer++ == 0)
			{
				if (!TempInBlob)
				{
					TempInBlob = true;
					vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber]++;
				}
			}
			else
				TempInBlob = false;
		}
		if ((vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] > 0) &&
			(!vLocalSystemData->vLastContainerImaged->vToBeEjected))
		{
			vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = 
				vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] / 2;
			//mark that you need to eject this for void based on processing so far
			//but may need to check other reference adjustments of this image first
			vLocalSystemData->vThisContainerShouldBeEjected = true;
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString +
						" Void Found",cDebugMessage);
		}
		else
		{
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString +
						" No Void",cDebugMessage);
			vLocalSystemData->vThisContainerShouldBeEjected = false;
		}
	}
}

CContainer *CImageAquisitionThread::GetNextContainerForImageProcessing()
{
	bool TempDone = false;
	CContainer *TempContainer = NULL;
	WORD TempCount = vLocalSystemData->vContainerCollection->GetCount();
	if (TempCount > 0)
	{
		POSITION TempIndex = vLocalSystemData->vContainerCollection->GetHeadPosition();
		TempContainer = vLocalSystemData->vContainerCollection->GetAt(TempIndex);//GetHead();
		//Check Each Containter until get one before timeout
		while ((TempContainer != NULL) && (!TempDone))
		{
			if (TempContainer->vImageReceived)
			{
				//get next container to check
				if (TempIndex)
					TempContainer = vLocalSystemData->vContainerCollection->GetNext(TempIndex);
				else
					TempContainer = NULL;
			}
			else
				TempDone = true;
		}  // while not done
	}  //end if have containers

	if (TempDone)
		return TempContainer;
	else
		return NULL;
}  //end function

void CImageAquisitionThread::AccumulateBodyTriggerReference()
{
	if (vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceCount == 0)
	{
		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("Start Accumulating Reference at frame:" + 
					vLocalSystemData->vFrameCountString,cDebugMessage);

		if (vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount == 0)
			vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount = 10;
		vLocalSystemData->CreateReferenceSamples(
			vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount);
	}
	if (vLocalSystemData->vLeftReferenceSample)
	if (vLocalSystemData->vRightReferenceSample)
	if (vLocalSystemData->vTopReferenceSample)
	if (vLocalSystemData->vBottomReferenceSample)
	{
		if (vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceCount <
			vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount)
		{
			double *TempTopSample = vLocalSystemData->vTopReferenceSample + vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceCount;
			double *TempBottomSample = vLocalSystemData->vBottomReferenceSample + vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceCount;
			double *TempRightSample = vLocalSystemData->vRightReferenceSample + vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceCount;
			double *TempLeftSample = vLocalSystemData->vLeftReferenceSample + vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceCount;

			*TempTopSample = vLocalSystemData->vITIPCDig->vLastTop;
			*TempBottomSample = vLocalSystemData->vITIPCDig->vLastBottom;
			*TempRightSample = vLocalSystemData->vITIPCDig->vLastRight;
			*TempLeftSample = vLocalSystemData->vITIPCDig->vLastLeft;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				CString TempString = dtoa(vLocalSystemData->vITIPCDig->vLastRight);
				vLocalSystemData->vLogFile.WriteToLogFile("Accumulating Right Reference: " + TempString,cDebugMessage);
				TempString = dtoa(vLocalSystemData->vITIPCDig->vLastLeft);
				vLocalSystemData->vLogFile.WriteToLogFile("Accumulating Left Reference: " + TempString,cDebugMessage);
				TempString = dtoa(vLocalSystemData->vITIPCDig->vLastTop);
				vLocalSystemData->vLogFile.WriteToLogFile("Accumulating Top Reference: " + TempString,cDebugMessage);
				TempString = dtoa(vLocalSystemData->vITIPCDig->vLastBottom);
				vLocalSystemData->vLogFile.WriteToLogFile("Accumulating Bottom Reference: " + TempString,cDebugMessage);
			}

			vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceCount++;
		}

		if (vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceCount >= vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount)
		{
			vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceEnabled = false;
			double TempAverage = 0;
			double TempVariance = 0;

			{
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("End Accumulating Reference at frame:" + vLocalSystemData->vFrameCountString,cDebugMessage);
				vLocalSystemData->vITIPCDig->vReferenceRightStandardDeviation = 
					CalculateStandardDeviation(vLocalSystemData->vRightReferenceSample, vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceCount, &vLocalSystemData->vITIPCDig->vReferenceRightAverage);

				vLocalSystemData->vITIPCDig->vReferenceLeftStandardDeviation = 
					CalculateStandardDeviation(vLocalSystemData->vLeftReferenceSample, vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceCount, &vLocalSystemData->vITIPCDig->vReferenceLeftAverage);

				vLocalSystemData->vITIPCDig->vReferenceTopStandardDeviation = 
					CalculateStandardDeviation(vLocalSystemData->vTopReferenceSample, vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceCount, &vLocalSystemData->vITIPCDig->vReferenceTopAverage);

				vLocalSystemData->vITIPCDig->vReferenceBottomStandardDeviation = 
					CalculateStandardDeviation(vLocalSystemData->vBottomReferenceSample, vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceCount, &vLocalSystemData->vITIPCDig->vReferenceBottomAverage);

				if (vLocalSystemData->vLogFile.vLogSerialData)
				{
					vLocalSystemData->vLogFile.WriteToLogFile("Right Reference: " + dtoa(vLocalSystemData->vITIPCDig->vReferenceRightAverage, 0),cDebugMessage);
					vLocalSystemData->vLogFile.WriteToLogFile("Right Reference Standard Deviation: " + dtoa(vLocalSystemData->vITIPCDig->vReferenceRightStandardDeviation, 0),cDebugMessage);
					vLocalSystemData->vLogFile.WriteToLogFile("Left Reference: " + dtoa(vLocalSystemData->vITIPCDig->vReferenceLeftAverage, 0),cDebugMessage);
					vLocalSystemData->vLogFile.WriteToLogFile("Left Reference Standard Deviation: " + dtoa(vLocalSystemData->vITIPCDig->vReferenceLeftStandardDeviation, 0),cDebugMessage);
					vLocalSystemData->vLogFile.WriteToLogFile("Top Reference: " + dtoa(vLocalSystemData->vITIPCDig->vReferenceTopAverage, 0),cDebugMessage);
					vLocalSystemData->vLogFile.WriteToLogFile("Top Reference Standard Deviation: " + dtoa(vLocalSystemData->vITIPCDig->vReferenceTopStandardDeviation, 0),cDebugMessage);
					vLocalSystemData->vLogFile.WriteToLogFile("Bottom Reference: " + dtoa(vLocalSystemData->vITIPCDig->vReferenceBottomAverage, 0),cDebugMessage);
					vLocalSystemData->vLogFile.WriteToLogFile("Bottom Reference Standard Deviation: " + dtoa(vLocalSystemData->vITIPCDig->vReferenceBottomStandardDeviation, 0),cDebugMessage);
				}

				if (vLocalSystemData->vInAutoSetup)
				{
					vGlobalCurrentProduct->vReferenceRight = (WORD)vLocalSystemData->vITIPCDig->vReferenceRightAverage;
					vGlobalCurrentProduct->vReferenceLeft = (WORD)vLocalSystemData->vITIPCDig->vReferenceLeftAverage;
					vGlobalCurrentProduct->vReferenceTop = (WORD)vLocalSystemData->vITIPCDig->vReferenceTopAverage;
					vGlobalCurrentProduct->vReferenceBottom = (WORD)vLocalSystemData->vITIPCDig->vReferenceBottomAverage;

					vGlobalCurrentProduct->vReferenceWidth = vGlobalCurrentProduct->vReferenceRight - vGlobalCurrentProduct->vReferenceLeft;
					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("Set Reference E Right: " + dtoa(vGlobalCurrentProduct->vReferenceRight, 2) + ", Left: " + dtoa(vGlobalCurrentProduct->vReferenceLeft, 2) +
						", Width: " + dtoa(vGlobalCurrentProduct->vReferenceWidth, 2), cDebugMessage);
				}
				vLocalSystemData->FreeReferenceSamples();
				::PostThreadMessage(vGlobalCallingThreadID,cCheckLearningMessage,0,0);
			}
		}
	}
}

void CImageAquisitionThread::AccumulateAverageImage(BYTE *TempPointerToImage)
{
	if (TempPointerToImage)
	if (vLocalSystemData->vITIPCDig->vAverageImage)
	{
		if (vLocalSystemData->vITIPCDig->vAverageCount == 0)
		{
			if (vLocalConfigurationData->vAverageDisableCount == 0)
				vLocalConfigurationData->vAverageDisableCount = 100;
		}
		vLocalSystemData->vITIPCDig->vAverageCount++;
		if (vLocalConfigurationData->vAverageDisableCount > 0)
		if (vLocalSystemData->vITIPCDig->vAverageCount >= vLocalConfigurationData->vAverageDisableCount)
		{
			vLocalSystemData->vITIPCDig->vAverageEnabled = false;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				vLocalSystemData->vLogFile.WriteToLogFile("End Accumulating Average",cDebugMessage);
			}
			::PostThreadMessage(vGlobalCallingThreadID,cCheckLearningMessage,0,0);
		}
		DWORD *TempPointerImageAverage = vLocalSystemData->vITIPCDig->vAverageImage;
		BYTE *TempPointerAquisitionImage = TempPointerToImage;
		for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
		{//add pixel to Average  
			*TempPointerImageAverage = *TempPointerImageAverage++ + *TempPointerAquisitionImage++;  
		}
	}
}

//bool CImageAquisitionThread::ThereAreAnyBlobsAtValue(BYTE TempThreshold, Image *TempImage, CInspection *TempInspection, WORD TempBottom, WORD TempLeft, WORD TempHeight, WORD TempWidth)
//{
//	vLocalSystemData->vITIPCDig->vBlobResults->num_blobs_found = 0;
//	Image *TempDuplicateImage = im_duplicate(TempImage);
//	if (TempDuplicateImage)
//	{
//		vLocalSystemData->vITIPCDig->vLatestSubImage = im_rroi(TempDuplicateImage, TempBottom, TempLeft, TempHeight, TempWidth);
//		if (vLocalSystemData->vITIPCDig->vLatestSubImage)
//		{
//			ThresholdImage(vLocalSystemData->vITIPCDig->vLatestSubImage, TempThreshold);
//
//			vLocalSystemData->vITIPCDig->vBlobParameters->min_area = TempInspection->vSize;
//			vLocalSystemData->vITIPCDig->vBlobParameters->blob_color = 0x00;  //fewer xrays
//
//			Erflag TempResult = mvt_blob_find(vLocalSystemData->vITIPCDig->vLatestSubImage, vLocalSystemData->vITIPCDig->vBlobParameters, vLocalSystemData->vITIPCDig->vBlobResults, MVT_CF_DEFAULT);
//			if (TempResult != IM_OK)
//				ReportErrorMessage("MVT_Blob Error",cEMailInspx,32000);
//
//			im_delete(vLocalSystemData->vITIPCDig->vLatestSubImage);
//			vLocalSystemData->vITIPCDig->vLatestSubImage = NULL;
//		}
//		im_delete(TempDuplicateImage);
//		TempDuplicateImage = NULL;
//	}
//	else
//		if (vLocalSystemData->vLogFile.vLogSerialData)
//			vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString +
//					" Error, MVT Could not create Image for Blobs",cDebugMessage);
//	if (vLocalSystemData->vITIPCDig->vBlobResults->num_blobs_found)
//		return true;
//	else
//		return false;
//}
//
void CImageAquisitionThread::ProcessCalibrateDetectorOffsetFrameADC()
{
	//if (vLocalSystemData->vCalibrationSimulationStarted)
	if (vGlobalFPGAVersion10Point0OrHigher)
	{
		if (vLocalSystemData->vLogFile.vLogSerialData)
		{
			for (BYTE TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfXRayDetectors; TempLoop++)
				vLocalSystemData->vLogFile.WriteToLogFile("Detector: " + dtoa(TempLoop,0) + " Brightness: " + dtoa(CalculateDetectorDensity(vLocalSystemData->vITIPCDig->vLastBufferFilled, TempLoop, vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors),2),cDebugMessage);
		}

		{
			vLocalSystemData->vWaitingForImage = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("IAT-ProcessCalibrateDetectorOffsetFrame",cDebugMessage);
			vLocalSystemData->vCalibrationImageNumber++;
			//vMainWindowPointer->SetCalibrateStep();
			::PostThreadMessage(vGlobalCallingThreadID,	cOffsetCalibrationNextStep,
				vLocalSystemData->vCalibrationImageNumber,0);

			//write calibration offset files to hard disk if failed to calibrate first time
			//if (vLocalSystemData->vCalibrateErrorCount == 1)Process
			{
				CString TempString = "CalibrationADCOffset";
				TempString = TempString + dtoa(vLocalSystemData->vCalibrateErrorCount,0) + "-";
				TempString = TempString + dtoa(vLocalSystemData->vCalibrationImageNumber,0);
				TempString = TempString + ".BMP";
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
						if (!TempImageFile->WriteFile(vLocalSystemData->vITIPCDig->vLastBufferFilled, vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors, vLocalSystemData->vITIPCDig->vAquisitionSizeY,
							(WORD)vLocalSystemData->vITIPCDig->vBitsPerPixel))
						{
							ReportErrorMessage("Failed to Write Calibration Image to File", cEMailInspx, 32000);
						}

						delete TempImageFile;
					}
				}
			}
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("Calibrate Offset Process Frame: " + dtoa(vLocalSystemData->vCalibrationImageNumber,0),cDebugMessage);

			CString TempCalibrationNumbersString = " ";
			for (BYTE TempDetectorLoop = 0; TempDetectorLoop < vLocalSystemData->vNumberOfXRayDetectorsUsing; TempDetectorLoop++)
			{
				if (HalfPixelsGreaterThanZero(TempDetectorLoop))  //offset is high, so try lower
				{
					vLocalSystemData->vCalibrationOffsetCurrentOffset[TempDetectorLoop] = vLocalSystemData->vCalibrationOffsetCurrentOffset[TempDetectorLoop] - vLocalSystemData->vCalibrationOffsetRange;

					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("Calibrate ADC Offset Value:" + dtoa(vLocalSystemData->vCalibrationOffsetCurrentOffset[TempDetectorLoop] - vLocalSystemData->vCalibrationADCOffsetMaximumBit,0) +
						" Detector: " + dtoa(TempDetectorLoop,0) + " Too Light",cDebugMessage);
				}
				else
				{
					if (vLocalSystemData->vCalibrationImageNumber > 9)
						vLocalSystemData->vCalibrationOffsetCurrentOffset[TempDetectorLoop] = vLocalSystemData->vCalibrationOffsetCurrentOffset[TempDetectorLoop] + vLocalSystemData->vCalibrationOffsetRange;

					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("Calibrate ADC Offset Value:" + dtoa(vLocalSystemData->vCalibrationOffsetCurrentOffset[TempDetectorLoop] - vLocalSystemData->vCalibrationADCOffsetMaximumBit,0) + " Detector: " + dtoa(TempDetectorLoop,0) + " Too Dark",cDebugMessage);
				}
			}

			if (vLocalSystemData->vCalibrationOffsetRange > 1)
				vLocalSystemData->vCalibrationOffsetRange = vLocalSystemData->vCalibrationOffsetRange >> 1;
			if (vLocalSystemData->vCalibrationImageNumber == 11)
			//if (vLocalSystemData->vCalibrationOffsetRange == 0) //done calibrating
			{//done calibration of all detectors so end calibration
				vLocalSystemData->vFrameCount = 0; 
				::PostThreadMessage(vGlobalCallingThreadID,	cOffsetCalibrationDoneMessage, 1,0); //1 = ADC
				//vLocalSystemData->vCalibrationSimulationStarted = false;
				for (BYTE TempLoop = 0; TempLoop < vLocalSystemData->vNumberOfXRayDetectorsUsing; TempLoop++)
				{
					vLocalSystemData->vADCDetectorOffset[TempLoop] = vLocalSystemData->vCalibrationOffsetCurrentOffset[TempLoop] - vLocalSystemData->vCalibrationADCOffsetMaximumBit;
				}
			}
			else  //not done, so send the next offset to test for all detectors
			{
				if (vGlobaluCSerialPort)
				for (BYTE TempLoop = 0; TempLoop < vLocalSystemData->vNumberOfXRayDetectorsUsing; TempLoop++)
				{
					if (vLocalSystemData->vCalibrationImageNumber < 10)
						vLocalSystemData->vCalibrationOffsetCurrentOffset[TempLoop] = vLocalSystemData->vCalibrationOffsetCurrentOffset[TempLoop] + vLocalSystemData->vCalibrationOffsetRange;

					vLocalSystemData->vSendADCValues = 0; //cancel sending any ADC Values as will send all now
					vGlobaluCSerialPort->SendADCOffset(TempLoop, vLocalSystemData->vCalibrationOffsetCurrentOffset[TempLoop] - vLocalSystemData->vCalibrationADCOffsetMaximumBit);
					Sleep(cSendCommandSleepTime + 10);
				}
				::PostThreadMessage(vGlobalCallingThreadID,cSimulateOneContainerIn10MSMessage,100,0);
			}
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				TempCalibrationNumbersString = "";
				for (BYTE TempLoop = 0; TempLoop < vLocalSystemData->vNumberOfXRayDetectorsUsing; TempLoop++)
					TempCalibrationNumbersString = TempCalibrationNumbersString + dtoa(vLocalSystemData->vCalibrationOffsetCurrentOffset[TempLoop] - vLocalSystemData->vCalibrationADCOffsetMaximumBit,0) + ", ";
				vLocalSystemData->vLogFile.WriteToLogFile("Calibrate ADC Offset Step:" + 
					dtoa(vLocalSystemData->vCalibrationImageNumber,0) + ": " + 
					TempCalibrationNumbersString,cDebugMessage);
			}
		}
	}
	else
	{
		{
			vLocalSystemData->vWaitingForImage = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("IAT-ProcessCalibrateDetectorOffsetFrame",cDebugMessage);
			vLocalSystemData->vCalibrationImageNumber++;
			//vMainWindowPointer->SetCalibrateStep();
			::PostThreadMessage(vGlobalCallingThreadID,	cOffsetCalibrationNextStep,
				vLocalSystemData->vCalibrationImageNumber,0);

			//write calibration offset files to hard disk if failed to calibrate first time
			//if (vLocalSystemData->vCalibrateErrorCount == 1)
			{
				CString TempString("CalibrationADCOffset");
				TempString = TempString + dtoa(vLocalSystemData->vCalibrateErrorCount,0) + "-";
				TempString = TempString + dtoa(vLocalSystemData->vCalibrationImageNumber,0);
				TempString = TempString + ".BMP";
				TempString = vLocalConfigurationData->vScanTracCalibrationImagesDirectory + TempString;
				int TempLength = TempString.GetLength();
				if ((PasswordOK(cTemporaryInspxPassword,false)) && (vLocalSystemData->vWriteCalibrationImages))
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
						if (!TempImageFile->WriteFile(vLocalSystemData->vITIPCDig->vLastBufferFilled,
							vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors,
							vLocalSystemData->vITIPCDig->vAquisitionSizeY,
							(WORD)vLocalSystemData->vITIPCDig->vBitsPerPixel))
						{
							ReportErrorMessage("Failed to Write Calibration Image to File", cEMailInspx, 32000);
						}

						delete TempImageFile;
					}
				}
			}
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile(
				"Calibrate Offset Process Frame: " + 
				dtoa(vLocalSystemData->vCalibrationImageNumber,0),cDebugMessage);

			CString TempCalibrationNumbersString = " ";
			for (BYTE TempDetectorLoop = 0; TempDetectorLoop < vLocalSystemData->vNumberOfXRayDetectorsUsing; TempDetectorLoop++)
			{
				if (HalfPixelsGreaterThanZero(TempDetectorLoop))  //offset is high, so try lower
				{
					vLocalSystemData->vCalibrationOffsetCurrentOffset[TempDetectorLoop] = vLocalSystemData->vCalibrationOffsetCurrentOffset[TempDetectorLoop] - vLocalSystemData->vCalibrationOffsetRange;

					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("Calibrate ADC Offset Value:" + dtoa(vLocalSystemData->vCalibrationOffsetCurrentOffset[TempDetectorLoop] - vLocalSystemData->vCalibrationADCOffsetMaximumBit,0) +
						" Detector: " + dtoa(TempDetectorLoop,0) + " Too Light",cDebugMessage);
				}
				else
				{
					if (vLocalSystemData->vCalibrationImageNumber > 9)
						vLocalSystemData->vCalibrationOffsetCurrentOffset[TempDetectorLoop] = vLocalSystemData->vCalibrationOffsetCurrentOffset[TempDetectorLoop] + vLocalSystemData->vCalibrationOffsetRange;

					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("Calibrate ADC Offset Value:" + dtoa(vLocalSystemData->vCalibrationOffsetCurrentOffset[TempDetectorLoop] - vLocalSystemData->vCalibrationADCOffsetMaximumBit,0) + " Detector: " + dtoa(TempDetectorLoop,0) + " Too Dark",cDebugMessage);
				}
			}

			if (vLocalSystemData->vCalibrationOffsetRange > 1)
				vLocalSystemData->vCalibrationOffsetRange = vLocalSystemData->vCalibrationOffsetRange >> 1;
			if (vLocalSystemData->vCalibrationImageNumber == 11)
			//if (vLocalSystemData->vCalibrationOffsetRange == 0) //done calibrating
			{//done calibration of all detectors so end calibration
				vLocalSystemData->vFrameCount = 0; 
				::PostThreadMessage(vGlobalCallingThreadID,	cOffsetCalibrationDoneMessage, 1,0); //1 = ADC
				//vLocalSystemData->vCalibrationSimulationStarted = false;
			}
			else  //not done, so send the next offset to test for all detectors
			{
				if (vGlobaluCSerialPort)
				for (BYTE TempLoop = 0; TempLoop < vLocalSystemData->vNumberOfXRayDetectorsUsing; TempLoop++)
				{
					if (vLocalSystemData->vCalibrationImageNumber < 10)
						vLocalSystemData->vCalibrationOffsetCurrentOffset[TempLoop] = vLocalSystemData->vCalibrationOffsetCurrentOffset[TempLoop] + vLocalSystemData->vCalibrationOffsetRange;

					vLocalSystemData->vSendADCValues = 0; //cancel sending any ADC Values as will send all now
					vGlobaluCSerialPort->SendADCOffset(TempLoop, vLocalSystemData->vCalibrationOffsetCurrentOffset[TempLoop] - vLocalSystemData->vCalibrationADCOffsetMaximumBit);
					Sleep(cSendCommandSleepTime + 10);
				}
				::PostThreadMessage(vGlobalCallingThreadID,cSimulateOneContainerIn10MSMessage,100,0);
			}
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				TempCalibrationNumbersString = "";

				for (BYTE TempLoop = 0; TempLoop < vLocalSystemData->vNumberOfXRayDetectorsUsing; TempLoop++)
					TempCalibrationNumbersString = TempCalibrationNumbersString + dtoa(vLocalSystemData->vCalibrationOffsetCurrentOffset[TempLoop] - vLocalSystemData->vCalibrationADCOffsetMaximumBit,0) + ", ";

				vLocalSystemData->vLogFile.WriteToLogFile("Calibrate ADC Offset Step:" + dtoa(vLocalSystemData->vCalibrationImageNumber,0) + ": " + TempCalibrationNumbersString,cDebugMessage);
			}
		}
	}
}

void CImageAquisitionThread::ProcessCalibrateDetectorOffsetFrameDAC()
{
	if (vLocalSystemData->vCalibrateDACPreFrames)
	{
		bool TempDetectorsNotZero = false;
		for (BYTE TempDetectorLoop = 0; TempDetectorLoop < vLocalConfigurationData->vNumberOfXRayDetectors; TempDetectorLoop++)
		{
			bool TempIsTooBright = HalfPixelsGreaterThanZero(TempDetectorLoop);

			if (TempIsTooBright) //should not be too bright, so ignore and give inspx message
			if (vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
			{
				TempDetectorsNotZero = true;
				ReportErrorMessage("Notice-DAC/Image is not black. Detector: " + dtoa(TempDetectorLoop + 1, 0) + ", PreFrame: " + dtoa(vLocalSystemData->vCalibrateDACPreFrames, 0), cWriteToLog, 0);
				TempIsTooBright = false;
			}
		}
		if (TempDetectorsNotZero)
		{
			if (vGlobaluCSerialPort)
			{
				for (BYTE TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfXRayDetectors; TempLoop++)
				{
					vLocalSystemData->vSendDACValues = 0; //cancel sending any DAC offsets that we were going to as now calibrating offset
					vLocalSystemData->vSendADCValues = 0; //cancel sending any DAC offsets that we were going to as now calibrating offset

					vGlobaluCSerialPort->SendAbsoluteDACDetectorOffset(TempLoop, vLocalSystemData->vCalibrationOffsetCurrentOffset[TempLoop]); //if too bright, reprogram DACS

					Sleep(cSendCommandSleepTime);
				}
			}
		}
		vLocalSystemData->vCalibrateDACPreFrames--;
		::PostThreadMessage(vGlobalCallingThreadID,cSimulateOneContainerIn10MSMessage,10,0);

		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("Calibrate DAC Preframe",cDebugMessage);

		CString TempString = "CalibrationDACOffset";
		TempString = TempString + dtoa(vLocalSystemData->vCalibrateErrorCount,0) + "-P";
		TempString = TempString + dtoa(vLocalSystemData->vCalibrateDACPreFrames,0);
		TempString = TempString + ".BMP";
		TempString = vLocalConfigurationData->vScanTracCalibrationImagesDirectory + TempString;
		//TempString = "C:\\ScanTracLogs\\" + TempString;
		int TempLength = TempString.GetLength();
		//if (PasswordOK(cTemporaryInspxPassword,false))// && (vLocalSystemData->vWriteCalibrationImages))
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
				if (!TempImageFile->WriteFile(vLocalSystemData->vITIPCDig->vLastBufferFilled,
					vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors,
					vLocalSystemData->vITIPCDig->vAquisitionSizeY,
					(WORD)vLocalSystemData->vITIPCDig->vBitsPerPixel))
				{
					ReportErrorMessage("Failed to Write Calibration Image to File", cEMailInspx, 32000);
				}
				delete TempImageFile;
			}
		}
	}
	else
	if (vGlobalFPGAVersion10Point0OrHigher)
	{
		if (vLocalSystemData->vLogFile.vLogSerialData)
		{
			for (BYTE TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfXRayDetectors; TempLoop++)
				vLocalSystemData->vLogFile.WriteToLogFile("Detector: " + dtoa(TempLoop,0) + " Brightness: " + dtoa(CalculateDetectorDensity(vLocalSystemData->vITIPCDig->vLastBufferFilled, 
					TempLoop, vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors),2),cDebugMessage);
		}

		if (vLocalSystemData->vCalibrationOffsetRange)
		{
			vLocalSystemData->vReceivedACalibrationImage = true;
			vLocalSystemData->vWaitingForImage = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("IAT-ProcessCalibrateDetectorOffsetFrameDAC",cDebugMessage);

			vLocalSystemData->vCalibrationImageNumber++;

			::PostThreadMessage(vGlobalCallingThreadID,	cOffsetCalibrationNextStep, vLocalSystemData->vCalibrationImageNumber,0);

			//write calibration offset files to hard disk if failed to calibrate first time
			//if (vLocalSystemData->vCalibrateErrorCount == 1)
			{
				CString TempString = "CalibrationDACOffset";
				TempString = TempString + dtoa(vLocalSystemData->vCalibrateErrorCount,0) + "-";
				TempString = TempString + dtoa(vLocalSystemData->vCalibrationImageNumber,0);
				TempString = TempString + ".BMP";
				TempString = vLocalConfigurationData->vScanTracCalibrationImagesDirectory + TempString;
				//TempString = "C:\\ScanTracLogs\\" + TempString;
				int TempLength = TempString.GetLength();
				//if (PasswordOK(cTemporaryInspxPassword,false))// && (vLocalSystemData->vWriteCalibrationImages))
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
						if (!TempImageFile->WriteFile(vLocalSystemData->vITIPCDig->vLastBufferFilled,
							vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors,
							vLocalSystemData->vITIPCDig->vAquisitionSizeY,
							(WORD)vLocalSystemData->vITIPCDig->vBitsPerPixel))
						{
							ReportErrorMessage("Failed to Write Calibration Image to File", cEMailInspx, 32000);
						}
						delete TempImageFile;
					}
				}
			}

			for (BYTE TempDetectorLoop = 0; TempDetectorLoop < vLocalSystemData->vNumberOfXRayDetectorsUsing; TempDetectorLoop++)
			{
				bool TempIsTooBright = HalfPixelsGreaterThanZero(TempDetectorLoop);

				//if (vLocalSystemData->vCalibrationImageNumber <= 3) //first three bits, 50%, 25%, 12.5%
				if (vLocalSystemData->vCalibrationImageNumber <= 1) //first one bits, 12.5%
				if (TempIsTooBright) //should not be too bright, so ignore and give inspx message
				if (vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
				{
					ReportErrorMessage("Notice-DAC/Image is not black. Detector: " + dtoa(TempDetectorLoop + 1) + ", Image: " + dtoa(vLocalSystemData->vCalibrationImageNumber), cWriteToLog, 0);
					TempIsTooBright = false;
				}

				//if (TempIsTooBright)  
				if (HalfPixelsGreaterThanZero(TempDetectorLoop))  //offset is high, so try lower
				{
					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("Calibrate DAC-Offset Step:" + WordToHex(vLocalSystemData->vCalibrationOffsetRange) + " Detector: " + dtoa(TempDetectorLoop,0) + " Too Light",cDebugMessage);
				}
				else
				{
					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("Calibrate DAC-Offset Step:" + WordToHex(vLocalSystemData->vCalibrationOffsetRange) + " Detector: " + dtoa(TempDetectorLoop,0) + " Too Dark",cDebugMessage);

					vLocalSystemData->vCalibrationOffsetCurrentOffset[TempDetectorLoop] = vLocalSystemData->vCalibrationOffsetCurrentOffset[TempDetectorLoop] - vLocalSystemData->vCalibrationOffsetRange;
				}
			}
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("Calibrate Offset Step:" + WordToHex(vLocalSystemData->vCalibrationOffsetRange),cDebugMessage);

			vLocalSystemData->vCalibrationOffsetRange = vLocalSystemData->vCalibrationOffsetRange >> 1;
			vLocalSystemData->vSendDACValues = 0; //cancel sending any DAC offsets that we were going to as now calibrating offset
			vLocalSystemData->vSendADCValues = 0; //cancel sending any DAC offsets that we were going to as now calibrating offset
			//set next bit in number to test if higher or lower

			if (vLocalSystemData->vCalibrationOffsetRange == 0)  //done with testing 12 bits, so save current offsets
			{//done calibration of all detectors so end calibration
				vLocalSystemData->vFrameCount = 0; 
				::PostThreadMessage(vGlobalCallingThreadID,	cOffsetCalibrationDoneMessage, 2,0); //2 = DAC

				for (BYTE TempLoop = 0; TempLoop < vLocalSystemData->vNumberOfXRayDetectorsUsing; TempLoop++)
				{
					vLocalSystemData->vDACDetectorOffset[TempLoop] = vLocalSystemData->vCalibrationOffsetCurrentOffset[TempLoop] * 100.0 / 0x800;
				}
			}
			else  //not done, so send the next offset to test for all detectors
			{
				//for (BYTE TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfXRayDetectors; TempLoop++)
				if (vGlobaluCSerialPort)
				for (BYTE TempLoop = 0; TempLoop < vLocalSystemData->vNumberOfXRayDetectorsUsing; TempLoop++)
				{
					vLocalSystemData->vCalibrationOffsetCurrentOffset[TempLoop] = vLocalSystemData->vCalibrationOffsetCurrentOffset[TempLoop] + vLocalSystemData->vCalibrationOffsetRange;
					vGlobaluCSerialPort->SendAbsoluteDACDetectorOffset(TempLoop, vLocalSystemData->vCalibrationOffsetCurrentOffset[TempLoop]); //set the offset of first detector mid way
				}
				::PostThreadMessage(vGlobalCallingThreadID,cSimulateOneContainerIn10MSMessage,10,0);//200 msec worked good
			}
		}
	}
	else
	{ //before version 10 FPGA
		if (vLocalSystemData->vCalibrationOffsetRange)
		{
			vLocalSystemData->vReceivedACalibrationImage = true;
			vLocalSystemData->vWaitingForImage = 0;
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("IAT-ProcessCalibrateDetectorOffsetFrameDAC",cDebugMessage);
			vLocalSystemData->vCalibrationImageNumber++;
			//vMainWindowPointer->SetCalibrateStep();
			::PostThreadMessage(vGlobalCallingThreadID,	cOffsetCalibrationNextStep,
				vLocalSystemData->vCalibrationImageNumber,0);

			//write calibration offset files to hard disk if failed to calibrate first time
			//if (vLocalSystemData->vCalibrateErrorCount == 1)
			{
				CString TempString = "CalibrationDACOffset";
				TempString = TempString + dtoa(vLocalSystemData->vCalibrateErrorCount,0) + "-";
				TempString = TempString + dtoa(vLocalSystemData->vCalibrationImageNumber,0);
				TempString = TempString + ".BMP";
				TempString = vLocalConfigurationData->vScanTracCalibrationImagesDirectory + TempString;
				int TempLength = TempString.GetLength();
				//if ((PasswordOK(cTemporaryInspxPassword,false)) && (vLocalSystemData->vWriteCalibrationImages))
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
						if (!TempImageFile->WriteFile(vLocalSystemData->vITIPCDig->vLastBufferFilled,
							vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors,
							vLocalSystemData->vITIPCDig->vAquisitionSizeY,
							(WORD)vLocalSystemData->vITIPCDig->vBitsPerPixel))
						{
							ReportErrorMessage("Failed to Write Calibration Image to File", cEMailInspx, 32000);
						}
						delete TempImageFile;
					}
				}
			}

			for (BYTE TempDetectorLoop = 0; TempDetectorLoop < 
				vLocalSystemData->vNumberOfXRayDetectorsUsing; TempDetectorLoop++)
			{
				if (HalfPixelsGreaterThanZero(TempDetectorLoop))  //offset is high, so try lower
				{
					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("Calibrate DAC Offset Step:" + WordToHex(vLocalSystemData->vCalibrationOffsetRange) +
						" Detector: " + dtoa(TempDetectorLoop,0) + " Too Light",cDebugMessage);
					vLocalSystemData->vCalibrationOffsetCurrentOffset[TempDetectorLoop] = vLocalSystemData->vCalibrationOffsetCurrentOffset[TempDetectorLoop] - vLocalSystemData->vCalibrationOffsetRange;
				}
				else
				{
					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("Calibrate DAC Offset Step:" + WordToHex(vLocalSystemData->vCalibrationOffsetRange) + " Detector: " + dtoa(TempDetectorLoop,0) + " Too Dark",cDebugMessage);
				}
			}
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile(
				"Calibrate Offset Step:" + WordToHex(vLocalSystemData->vCalibrationOffsetRange),cDebugMessage);
			vLocalSystemData->vCalibrationOffsetRange = vLocalSystemData->vCalibrationOffsetRange >> 1;
			//set next bit in number to test if higher or lower

			if (vLocalSystemData->vCalibrationOffsetRange == 0)  //done with testing 12 bits, so save current offsets
			{//done calibration of all detectors so end calibration
				vLocalSystemData->vFrameCount = 0; 
				::PostThreadMessage(vGlobalCallingThreadID,	cOffsetCalibrationDoneMessage, 2,0); //2 = DAC
			}
			else  //not done, so send the next offset to test for all detectors
			{
				//for (BYTE TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfXRayDetectors; TempLoop++)
				if (vGlobaluCSerialPort)
				for (BYTE TempLoop = 0; TempLoop < vLocalSystemData->vNumberOfXRayDetectorsUsing; TempLoop++)
				{
					vLocalSystemData->vCalibrationOffsetCurrentOffset[TempLoop] = vLocalSystemData->vCalibrationOffsetCurrentOffset[TempLoop] + vLocalSystemData->vCalibrationOffsetRange;
					vGlobaluCSerialPort->SendAbsoluteDACDetectorOffset(TempLoop, vLocalSystemData->vCalibrationOffsetCurrentOffset[TempLoop]); //set the offset of first detector mid way
				}
				::PostThreadMessage(vGlobalCallingThreadID,cSimulateOneContainerIn10MSMessage,10,0);//200 msec worked good
			}
		}
	}
}

bool CImageAquisitionThread::HalfPixelsGreaterThanZero(BYTE TempDetectorNumber)
{
	bool TempReturn = false;
	WORD TempCounter = 0;
	double TempSum = 0;
	if (vLocalSystemData->vITIPCDig->vLastBufferFilled)
	{
		for (BYTE TempLineLoop = 3; TempLineLoop < vLocalConfigurationData->vPixelsPerDetector - 3; TempLineLoop++)
		{
			BYTE *TempSourcePointer = (BYTE *)vLocalSystemData->vITIPCDig->vLastBufferFilled + (TempLineLoop + (TempDetectorNumber * vLocalConfigurationData->vPixelsPerDetector));
			TempSum = 0;
			for (WORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vAquisitionSizeY; TempLoop++)
			{
				TempSum = TempSum + *TempSourcePointer;
				TempSourcePointer = TempSourcePointer + vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors;
			}
			TempSum = TempSum / vLocalSystemData->vITIPCDig->vAquisitionSizeY;
			if (TempSum > 1)
				TempCounter++;
		}
		if (TempCounter > vLocalConfigurationData->vPixelsPerDetector / 2)
			TempReturn = true;
	}
	return TempReturn;
}

void CImageAquisitionThread::CheckForJarBottomContaminants(CContainer *TempContainer, CInspection *TempInspection, BYTE TempInspectionNumber)
{
	if (TempContainer)
	if (TempContainer->vTheImage)
		TempContainer->vTheImage->MakeRedImage(vLocalSystemData->vITIPCDig->vOriginalBufferSize);

	if (TempContainer)
	if (TempContainer->vTheImage)
	if (TempContainer->vTheImage->vRedImage)
	if (TempInspection)
	{
		WORD TempBottom = 0;
		WORD TempLeft = 0;
		WORD TempHeight = 0;
		WORD TempWidth = 0;
		vLocalSystemData->vITIPCDig->GetInspectionROIBounds(TempInspection, 
			&TempBottom, &TempLeft, &TempHeight, &TempWidth, 0, //already referenced vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust, 
			vLocalConfigurationData->vReferenceROIsToEdges, 0);

		WORD TempRight = TempLeft + TempWidth;

		BYTE TempThreshold = TempInspection->vThreshold;
		//if (vLocalConfigurationData->vDriftCompensationEnabled)
		//if (TempThreshold > vLocalSystemData->vDriftCompensationThresholdReductionAmount + 1)
		//if (TempInspection->vEnhanceMethod)
		//	TempThreshold = (BYTE)(TempThreshold - vLocalSystemData->vDriftCompensationThresholdReductionAmount + .49);

		CopyMemory(
			vLocalSystemData->vITIPCDig->vLatestImageCopy->pix,
			vLocalSystemData->vITIPCDig->vLatestImage->pix,
			vLocalSystemData->vITIPCDig->vOriginalBufferSize);

		if (TempInspection->vPreSmoothImage)
			DoMedianSmooth(vLocalSystemData->vITIPCDig->vLatestImageCopy, TempInspection->vPreSmoothImage,TempInspection->vPreSmoothImage);

		if (TempInspection->vEnhanceMethod)
		{
			double TempStartFindTime = CIFCOS::GetSystimeMicrosecs();
			vLocalSystemData->vITIPCDig->PerformEnhanceOnImage(vLocalSystemData->vITIPCDig->vLatestImageCopy, vLocalSystemData->vITIPCDig->vIATScratchImage, TempInspection->vEnhanceMethod);
			double 	TempEndTime = CIFCOS::GetSystimeMicrosecs();
			vLocalSystemData->vITIPCDig->vLastEnhanceTime = TempEndTime - TempStartFindTime;
		}
			vLocalSystemData->vITIPCDig->PerformEnhanceOnImage(vLocalSystemData->vITIPCDig->vLatestImageCopy, vLocalSystemData->vITIPCDig->vIATScratchImage,TempInspection->vEnhanceMethod);
	
		WORD TempHalfWidth = TempWidth / 2;
		WORD TempMiddleLine = TempLeft + TempHalfWidth;
		WORD TempCurrentBottomHeight = TempHeight;
		WORD TempCurrentBottomHeightFollowing = TempHeight;

		//first time through evaluate get first cut on threshold
		if (TempInspection->vFindGoodIntensity)
		//if (!vLocalSystemData->vMinimumGoodIndex)
		if (vLocalSystemData->vMinimumGoodIntensity == 255)
		{
			vLocalSystemData->vITIPCDig->vLastMinimumGoodMaximumBad = 255;
			//check left side of bottom of container
			for (int TempLineLoop = TempMiddleLine + 5; TempLineLoop >= TempMiddleLine - 5; TempLineLoop--)
			{ //check each line near top to see what the brightness is
				WORD TempOffset = 56 + (TempLineLoop * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
				BYTE *TempPixelPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix + TempOffset;

				if (TempOffset < vLocalSystemData->vITIPCDig->vOriginalBufferSize)
				if (TempOffset > 9)
				for (int TempPixelLoop = 9; TempPixelLoop >= 0; TempPixelLoop--)
				{  //find darkest pixel value in product area of ROI.
					if (*TempPixelPointer < vLocalSystemData->vITIPCDig->vLastMinimumGoodMaximumBad) 
						vLocalSystemData->vITIPCDig->vLastMinimumGoodMaximumBad = *TempPixelPointer;

					TempPixelPointer--;
				}  
			}
			TempThreshold = (BYTE)(vLocalSystemData->vITIPCDig->vLastMinimumGoodMaximumBad - 20);
			if (vLocalSystemData->vInAutoSetup)  
				TempInspection->vThreshold = TempThreshold;
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile(
				"Evaluate Jar Bottom, Initial Pass Set Threshold: " + 
				dtoa(TempThreshold,0),cDebugMessage);
			
		} //end first time through evaluate get first cut on threshold

		WORD TempOffset = TempInspection->vROIPixelTop + (TempMiddleLine * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
		BYTE *TempPixelPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix + TempOffset;

		if (TempOffset < vLocalSystemData->vITIPCDig->vOriginalBufferSize)
		if (TempOffset >= TempHeight)
		for (int TempPixelLoop = TempHeight; TempPixelLoop > 0; TempPixelLoop--)
		{
			if (*TempPixelPointer < TempThreshold) //found bottom of container so mark it as current bottom
			{
				BYTE *TempPixelPointer2 = TempPixelPointer - 1;
				if ((TempPixelLoop == 1) || 
					(*TempPixelPointer2 < TempThreshold)) 
				{	//found bottom of container so mark it as current bottom
					TempCurrentBottomHeight = TempPixelLoop;
					TempCurrentBottomHeightFollowing = TempPixelLoop;
					TempPixelLoop = 0; //end loop if found bottom
				}
			}
			TempPixelPointer--;
		}
		WORD TempMiddleBottomHeight = TempCurrentBottomHeight;

		bool TempEvaluateMovedROI = false;
		bool TempDone = false;
		while (!TempDone)
		{
			//check left side of bottom of container
			TempCurrentBottomHeight = TempMiddleBottomHeight;
			WORD TempWentUp = 0;
			for (int TempLineLoop = TempMiddleLine - 1; TempLineLoop >= TempLeft; TempLineLoop--)
			{ //check each line to the left of center for a bump on the bottom
				WORD TempOffset = TempInspection->vROIPixelTop + (TempLineLoop * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
				BYTE *TempPixelPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix + TempOffset;

				if (TempOffset < vLocalSystemData->vITIPCDig->vOriginalBufferSize)
				if (TempOffset >= TempHeight)
				for (int TempPixelLoop = TempHeight; TempPixelLoop >= 0; TempPixelLoop--)
				{  //go down each pixel on the line until you find the bottom.
					if (*TempPixelPointer < TempThreshold) //found bottom of container so mark it as current bottom
					{
						BYTE *TempPixelPointer1 = TempPixelPointer + 1;
						if (TempPixelLoop > (long)(TempInspection->vSize + TempCurrentBottomHeight))
						{  //if bottom bumps up too much, then reject it
							//if (vLocalSystemData->vInAutoSetup)  //evaluating and reject close to left
							if (TempInspection->vFindGoodIntensity) //so move left ROI to the right
							if ((TempLineLoop < vGlobalCurrentProduct->vReferenceLeft + 
								(vGlobalCurrentProduct->vReferenceWidth / 8)) ||
								((vLocalSystemData->vMinimumGoodIndex > 20) &&
								(TempLineLoop < vGlobalCurrentProduct->vReferenceLeft + 
								(vGlobalCurrentProduct->vReferenceWidth / 4))))
							{
								TempEvaluateMovedROI = true;
								TempInspection->SetROILeft((TempLineLoop + 2) / 
									(vGlobalPixelsPerUnit *	vGlobalCurrentProduct->vOverScanMultiplier),
									vGlobalCurrentProduct->vOverScanMultiplier);
								if (vLocalSystemData->vLogFile.vLogSerialData)
									vLocalSystemData->vLogFile.WriteToLogFile(
									"Evaluate Jar Bottom, Move Left ROI Line In to: " +
									dtoa((TempInspection->vROIPixelLeft),0),cDebugMessage);

								int TempDeltaLeft = TempInspection->vROIPixelLeft - 
									vGlobalCurrentProduct->vReferenceLeft;
								int TempDeltaRight = vGlobalCurrentProduct->vReferenceRight - 
									TempInspection->vROIPixelRight;
								if (TempDeltaLeft > 0)
								if (TempDeltaLeft > TempDeltaRight)
								{
									TempInspection->SetROIRight((vGlobalCurrentProduct->vReferenceRight - TempDeltaLeft) / (vGlobalPixelsPerUnit *	vGlobalCurrentProduct->vOverScanMultiplier), vGlobalCurrentProduct->vOverScanMultiplier);
								}
							}
							else  //reduce threshold by one if still rejecting after 50
							if (vLocalSystemData->vMinimumGoodIndex > 50)
							if (TempInspection->vSize < 2)
								TempInspection->vSize = 2;
							else
							if (vLocalSystemData->vMinimumGoodIntensity > 40)
								vLocalSystemData->vMinimumGoodIntensity =
								vLocalSystemData->vMinimumGoodIntensity - 5;

							//if evaluating, don't eject this container if already ejected
							if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
							if (vGlobaluCSerialPort)
							if (TempInspection->vEnable)
								vGlobaluCSerialPort->EjectContainer(cEjectInspectionOffset + TempInspectionNumber,TempContainer, TempInspection);

							//vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = 1;
							//vLocalSystemData->vITIPCDig->vLastThresholdCorrection = TempThreshold;
							if (TempCurrentBottomHeightFollowing > TempPixelLoop)
								TempContainer->vTheImage->vContainerLargestContaminant = TempCurrentBottomHeightFollowing - TempPixelLoop;
							else
								TempContainer->vTheImage->vContainerLargestContaminant = TempPixelLoop - TempCurrentBottomHeightFollowing;

							vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = 
								TempContainer->vTheImage->vContainerLargestContaminant;

							BYTE *TempRedPointer = TempBottom + 1 + TempContainer->vTheImage->vRedImage + ((TempLineLoop) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
							for (WORD TempLoop = 0; TempLoop < TempPixelLoop; TempLoop++)
								*TempRedPointer++ = 0;

							TempLineLoop = TempLeft; //exit looking at the lines on the left side of container
						}  //end bottom bumped up to much so rejected
						else  //if drop too much, then reject
						if (TempInspection->vMaximumDrop)
						if (TempPixelLoop < TempCurrentBottomHeightFollowing - TempInspection->vMaximumDrop)
						//if evaluating, don't eject this container if already ejected
						if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
						{
							if (vGlobaluCSerialPort)
							if (TempInspection->vEnable)
								vGlobaluCSerialPort->EjectContainer(cEjectInspectionOffset + TempInspectionNumber,TempContainer, TempInspection);

							TempContainer->vTheImage->vContainerEjectThreshold = TempThreshold;
							WORD TempStart = 0;
							if (TempCurrentBottomHeightFollowing > TempPixelLoop)
							{
								TempContainer->vTheImage->vContainerLargestContaminant = TempCurrentBottomHeightFollowing - TempPixelLoop;
								TempStart = TempPixelLoop;
							}
							else
							{
								TempContainer->vTheImage->vContainerLargestContaminant = TempPixelLoop - TempCurrentBottomHeightFollowing;
								TempStart = TempCurrentBottomHeightFollowing;
							}

							vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = 
								TempContainer->vTheImage->vContainerLargestContaminant;

							if (TempContainer)
							if (TempContainer->vTheImage)
							if (TempContainer->vTheImage->vRedImage)
							{
								BYTE *TempRedPointer = TempContainer->vTheImage->vRedImage + TempStart + ((TempLineLoop) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
								for (WORD TempLoop = TempStart; TempLoop < TempInspection->vROIPixelTop + 2; TempLoop++)
									*TempRedPointer++ = 0;
							}

							TempLineLoop = TempLeft; //exit looking at the lines on the left side of container
						}  //end bottom dropped too much so reject it

						//found bottom so if went down, set current bottom
						TempCurrentBottomHeightFollowing = TempPixelLoop;
						if (TempPixelLoop < TempCurrentBottomHeight)
							TempCurrentBottomHeight = TempPixelLoop;
						else
						//if found bottom, if went up, and following bottom up and down, 
						//then set current bottom
						if ((TempInspection->vFollowBottomUpAndDown) && 
							(!TempInspection->vFindGoodIntensity))
						if (TempPixelLoop > TempCurrentBottomHeight)
						{
							if (TempWentUp > TempInspection->vSize)
							{
								TempCurrentBottomHeight++;
								TempWentUp = 0;
							}
							else
								TempWentUp++;
						}
						TempPixelLoop = 0; //end loop if found bottom
					}//end of found bottom
					TempPixelPointer--;
				}  //end checking each pixel down this line
			}//end checking left side

			//do right side next!!!
			if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
			{
				WORD TempWentUp = 0;
				TempCurrentBottomHeight = TempMiddleBottomHeight;
				TempCurrentBottomHeightFollowing = TempMiddleBottomHeight;
				for (WORD TempLineLoop = TempMiddleLine + 1; TempLineLoop < TempRight; TempLineLoop++)
				{
					WORD TempOffset = TempInspection->vROIPixelTop +
							(TempLineLoop * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
					BYTE *TempPixelPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix + TempOffset;

					if (TempOffset < vLocalSystemData->vITIPCDig->vOriginalBufferSize)
					if (TempOffset >= TempHeight)
					for (int TempPixelLoop = TempHeight; TempPixelLoop >= 0; TempPixelLoop--)
					{//check each pixel down this line to find bottom
						if (*TempPixelPointer < TempThreshold) 
						{//found bottom of container so check it and mark it as current bottom
							BYTE *TempPixelPointer1 = TempPixelPointer + 1;
							if (TempPixelLoop > (long)(TempInspection->vSize + TempCurrentBottomHeight))
							{  //found the bottom bumped up too much, so reject
								//if (vLocalSystemData->vInAutoSetup)  //evaluating and reject close to right
								if (TempInspection->vFindGoodIntensity) //so move right ROI to the left
								if ((TempLineLoop + (vGlobalCurrentProduct->vReferenceWidth / 8) > 
									vGlobalCurrentProduct->vReferenceRight) ||
									((vLocalSystemData->vMinimumGoodIndex > 20) &&
									(TempLineLoop < vGlobalCurrentProduct->vReferenceLeft + 
									(vGlobalCurrentProduct->vReferenceWidth / 4))))
								{
									TempEvaluateMovedROI = true;
									TempInspection->SetROIRight((TempLineLoop - 2) / 
										(vGlobalPixelsPerUnit *	vGlobalCurrentProduct->vOverScanMultiplier),
										vGlobalCurrentProduct->vOverScanMultiplier);
									if (vLocalSystemData->vLogFile.vLogSerialData)
										vLocalSystemData->vLogFile.WriteToLogFile(
										"Evaluate Jar Bottom, Move Right ROI Line In to: " +
										dtoa(TempInspection->vROIPixelRight,0),cDebugMessage);

									int TempDeltaRight = vGlobalCurrentProduct->vReferenceRight - 
										TempInspection->vROIPixelRight;
									int TempDeltaLeft = TempInspection->vROIPixelLeft - 
										vGlobalCurrentProduct->vReferenceLeft;
									if (TempDeltaRight > 0)
									if (TempDeltaRight > TempDeltaLeft)
									{
										TempInspection->SetROILeft((vGlobalCurrentProduct->vReferenceLeft +
											TempDeltaRight) / 
											(vGlobalPixelsPerUnit *	vGlobalCurrentProduct->vOverScanMultiplier),
											vGlobalCurrentProduct->vOverScanMultiplier);
									}
								}
								else
								if (vLocalSystemData->vMinimumGoodIndex > 50)
								if (TempInspection->vSize < 2)
									TempInspection->vSize = 2;
								else
								if (vLocalSystemData->vMinimumGoodIntensity > 40)
									vLocalSystemData->vMinimumGoodIntensity = 
									vLocalSystemData->vMinimumGoodIntensity -5;

								//if evaluating, don't eject this container if already ejected
								if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
								if (TempInspection->vEnable)
								if (vGlobaluCSerialPort)
								{
									vGlobaluCSerialPort->EjectContainer(
										cEjectInspectionOffset + TempInspectionNumber,TempContainer, TempInspection);

									//vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = 1;
									//vLocalSystemData->vITIPCDig->vLastThresholdCorrection = TempThreshold;
									TempContainer->vTheImage->vContainerEjectThreshold = TempThreshold;
									if (TempCurrentBottomHeightFollowing > TempPixelLoop)
										TempContainer->vTheImage->vContainerLargestContaminant = TempCurrentBottomHeightFollowing - TempPixelLoop;
									else
										TempContainer->vTheImage->vContainerLargestContaminant = TempPixelLoop - TempCurrentBottomHeightFollowing;

									vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = 
										TempContainer->vTheImage->vContainerLargestContaminant;

									if (TempContainer)
									if (TempContainer->vTheImage)
									if (TempContainer->vTheImage->vRedImage)
									{
										BYTE *TempRedPointer = TempBottom + 1 + TempContainer->vTheImage->vRedImage + ((TempLineLoop) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
										for (WORD TempLoop = 0; TempLoop < TempPixelLoop; TempLoop++)
											*TempRedPointer++ = 0;
									}
								}
								
								TempLineLoop = TempRight; //exit looking at the lines on the left side of container
							}// end of found the bottom bumped up too much, so reject
							else  //if drop too much, then reject
							if (TempInspection->vMaximumDrop)
							if (TempPixelLoop < TempCurrentBottomHeightFollowing - 
								TempInspection->vMaximumDrop)
							//if evaluating, don't eject this container if already ejected
							if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
							{//found bottom fell too much, so eject
								if (vGlobaluCSerialPort)
								if (TempInspection->vEnable)
									vGlobaluCSerialPort->EjectContainer(cEjectInspectionOffset + 
										TempInspectionNumber,TempContainer, TempInspection);

								//vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = 1;
								//vLocalSystemData->vITIPCDig->vLastThresholdCorrection = TempThreshold;
								TempContainer->vTheImage->vContainerEjectThreshold = TempThreshold;
								WORD TempStart = 0;
								if (TempCurrentBottomHeightFollowing > TempPixelLoop)
								{
									TempContainer->vTheImage->vContainerLargestContaminant = TempCurrentBottomHeightFollowing - TempPixelLoop;
									TempStart = TempPixelLoop;
								}
								else
								{
									TempContainer->vTheImage->vContainerLargestContaminant = TempPixelLoop - TempCurrentBottomHeightFollowing;
									TempStart = TempCurrentBottomHeightFollowing;
								}

								vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = TempContainer->vTheImage->vContainerLargestContaminant;

								if (TempContainer)
								if (TempContainer->vTheImage)
								if (TempContainer->vTheImage->vRedImage)
								{
									BYTE *TempRedPointer = TempContainer->vTheImage->vRedImage + TempStart + ((TempLineLoop) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
									for (WORD TempLoop = TempStart; TempLoop < TempInspection->vROIPixelTop + 2; TempLoop++)
										*TempRedPointer++ = 0;
								}

								TempLineLoop = TempRight; //exit looking at the lines on the left side of container
							}//end of found bottom fell too much, so eject
							TempCurrentBottomHeightFollowing = TempPixelLoop;
							if (TempPixelLoop < TempCurrentBottomHeight)
								TempCurrentBottomHeight = TempPixelLoop;
							else
							if ((TempInspection->vFollowBottomUpAndDown) && 
								(!TempInspection->vFindGoodIntensity))
							if (TempPixelLoop > TempCurrentBottomHeight)
							{
								if (TempWentUp > TempInspection->vSize)
								{
									TempCurrentBottomHeight++;
									TempWentUp = 0;
								}
								else
									TempWentUp++;
							}
							TempPixelLoop = 0; //end loop if found bottom
						}//end of found bottom
						TempPixelPointer--;
					}//end of check each pixel down this line to find bottom
				}

			}
			if (vLocalSystemData->vLastContainerImaged->vToBeEjected)
				TempDone = true;
			if (!TempInspection->vTryThresholdsDownToThreshold)
				TempDone = true;
			else
			if (TempThreshold >= TempInspection->vTryThresholdsDownToThreshold)
				TempThreshold--;
			else
				TempDone = true;
		}  //end while not done
		//evaluate
		if (TempInspection->vFindGoodIntensity)//vProcessMinimumGood
		{
			vLocalSystemData->vMinimumGoodIndex++;
			if (!TempEvaluateMovedROI)
			{
				vLocalSystemData->vITIPCDig->vLastMinimumGoodMaximumBad = 255;
				TempHeight = (WORD)(TempHeight * .5);  //look at top 50% of ROI for Product area
				//check left side of bottom of container
				TempRight = TempRight - (TempHalfWidth / 2);
				TempLeft = TempLeft + (TempHalfWidth / 2);
				for (int TempLineLoop = TempRight; TempLineLoop >= TempLeft;TempLineLoop--)
				{ //check each line near top to see what the brightness is
					WORD TempOffset = TempInspection->vROIPixelTop + (TempLineLoop * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
					BYTE *TempPixelPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix + TempOffset;

					if (TempOffset < vLocalSystemData->vITIPCDig->vOriginalBufferSize)
					if (TempOffset >= TempHeight)
					for (int TempPixelLoop = TempHeight; TempPixelLoop >= 0; TempPixelLoop--)
					{  //find darkest pixel value in product area of ROI.
						if (*TempPixelPointer < vLocalSystemData->vITIPCDig->vLastMinimumGoodMaximumBad) 
							vLocalSystemData->vITIPCDig->vLastMinimumGoodMaximumBad = *TempPixelPointer;

						if (*TempPixelPointer < vLocalSystemData->vMinimumGoodIntensity) 
							vLocalSystemData->vMinimumGoodIntensity = *TempPixelPointer;

						TempPixelPointer--;
					}  //end checking each pixel down this line
				}
			} //end of evaluate
		}
	}
}

void CImageAquisitionThread::WriteImageToHardDisk(BYTE *TempPointer, CString TempName, WORD TempSizeX, WORD TempSizeY)
{
	//if (vLocalSystemData->vLogFile.vLogSerialData)
	{
		CString TempString = TempName;
		int TempLength = TempString.GetLength();
		//if (PasswordOK(cTemporaryInspxPassword,false))
		if (TempLength > MAX_PATH)
		{
			CNoticeDialog TempNoticeDialog;
			CString TempString = "\n\n\nSetup Name more the MAX_PATH characters, Image: " + TempName + " save aborted";
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
				TempImageFile->WriteFile(TempPointer, TempSizeX, TempSizeY, (WORD)vLocalSystemData->vITIPCDig->vBitsPerPixel);
				delete TempImageFile;
			}
		}
	}
}

/*
void CImageAquisitionThread::ClearDataInThisBuffer(BYTE *TempFrameBuffer)
{
	for (DWORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vAquisitionSizeY;
		TempLoopY++)
	{//check the last byte of each line, this is the extra pixel
		BYTE TempByte = 0xFF;
		BYTE *TempPointerToExtraPixel = NULL;
		TempPointerToExtraPixel = TempFrameBuffer - 1 + 
			(vLocalSystemData->vITIPCDig->vOriginalBufferSizeX * (TempLoopY + 1));
		for (DWORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
			TempLoopX++)
		{//check the last byte of each line, this is the extra pixel
			*TempPointerToExtraPixel = TempByte;
			TempByte--;
			TempPointerToExtraPixel--;
		}
	}
}
*/
void CImageAquisitionThread::ProcessCalibrationImage()
{
	if (vLocalSystemData->vLogFile.vLogSerialData)
		vLocalSystemData->vLogFile.WriteToLogFile("Start Process Calibration Image", cDebugMessage);

	if ((vLocalSystemData->vCalibrationSimulationStarted) || (!vLocalSystemData->vITIPCDig->vShowOnlyRealImages) || (vLocalSystemData->vSystemRunMode == cCalibrateEncoderMode))
	if ((vLocalSystemData->vITIPCDig->vNextImageToDisplay) && (vLocalSystemData->vITIPCDig->vNextImageToDisplay->vGreyImage))
	if (vLocalSystemData->vITIPCDig->vNextImageToDisplay->vRedImage)
	{
		if (vLocalSystemData->vITIPCDig->vNextImageToDisplayLock.Lock())
		{
			vLocalSystemData->vCalibrationSimulationStarted = false;
			if (vLocalSystemData->vSystemRunMode == cCalibratingDetectorEdgesMode)
			if (vLocalConfigurationData->vSimulateLightControlBoard)
			{
				if (vLocalSystemData->vFindingProductBrightness)
					FillMemory(vLocalSystemData->vITIPCDig->vLastBufferFilled, vLocalSystemData->vITIPCDig->vOriginalBufferSize, 254);
				else
				{
					FillMemory(vLocalSystemData->vITIPCDig->vLastBufferFilled, vLocalSystemData->vITIPCDig->vOriginalBufferSize, vLocalSystemData->vCalibrationImageNumber * 10);
					for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors;  TempLoopX = TempLoopX + 10)
					{
						BYTE *TempPointer = vLocalSystemData->vITIPCDig->vLastBufferFilled + TempLoopX;
						for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY;TempLoopY++)
						{
							*TempPointer = *TempPointer + 12;
							TempPointer = TempPointer + vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors;
						}
					}
					for (WORD TempLoopX = 5; TempLoopX < vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors;  TempLoopX = TempLoopX + 10)
					{
						BYTE *TempPointer = vLocalSystemData->vITIPCDig->vLastBufferFilled + TempLoopX;
						for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY;TempLoopY++)
						{
							if (*TempPointer > 35 + (2 * vLocalSystemData->vCalibrationImageNumber))
								*TempPointer = *TempPointer - 35 - (vLocalSystemData->vCalibrationImageNumber * 2);
							else
								*TempPointer = 0;
							TempPointer = TempPointer + vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors;
						}
					}
				}
				FillMemory(vLocalSystemData->vITIPCDig->vNextImageToDisplay->vRedImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize, 1);
			}
			vLocalSystemData->vITIPCDig->vNextImageToDisplayLock.Unlock();
		}

		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("Finished a Frame for Calibration and process", cDebugMessage);

		if ((vLocalSystemData->vITIPCDig->vHaveRealImage) || ((vLocalConfigurationData->vSimulateLightControlBoard) && (vLocalSystemData->vSystemRunMode == cCalibratingDetectorEdgesMode)))
		if (vCorrectDetectorEdges)
		{
			if (((vLocalSystemData->vSystemRunMode == cCalibratingDetectorEdgesMode) && (vLocalSystemData->vApplyGainAndOffsetDuringCalibration)) ||
				((vLocalSystemData->vSystemRunMode == cMeasureSourceStrengthMode) && (vLocalSystemData->vApplyGainAndOffsetDuringCalibration)) ||
				(vLocalSystemData->vSystemRunMode == cCalibrateEncoderMode))
			{
				AdjustEdgeDetectorPixels(false);
			}
		}

		if (vLocalSystemData->vSystemRunMode == cMeasureSourceStrengthMode)
		{
			WriteImageToHardDisk(vLocalSystemData->vITIPCDig->vLastBufferFilled, vLocalConfigurationData->vScanTracCalibrationImagesDirectory + "MSS" + dtoa(vLocalSystemData->vCalibrationImageNumber,0) + 
				".BMP", vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);
		}

		if (vLocalSystemData->vSystemRunMode == cCalibratingDetectorOffsetMode)
		{
			if (vLocalSystemData->vCalibrateUsingDAC)
				ProcessCalibrateDetectorOffsetFrameDAC();
			else
				ProcessCalibrateDetectorOffsetFrameADC();
		}
		else
		if (vLocalSystemData->vITIPCDig->vLastBufferFilled)
		if (vLocalSystemData->vITIPCDig->vNextImageToDisplayLock.Lock())
		{
			//CopyMemory(vLocalSystemData->vITIPCDig->vNextImageToDisplay->vGreyImage,
			//	vLocalSystemData->vITIPCDig->vLastBufferFilled,
			//	vLocalSystemData->vITIPCDig->vOriginalBufferSize);
				//copy image to LatestImage
			BYTE *TempSourceBuffer; 
			BYTE *TempDestinationBuffer;                      
			TempDestinationBuffer = vLocalSystemData->vITIPCDig->vNextImageToDisplay->vGreyImage;
			for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vAquisitionSizeY; TempLoopY++)
			{
				TempSourceBuffer = vLocalSystemData->vITIPCDig->vLastBufferFilled + vLocalSystemData->vITIPCDig->vAquisitionROIBottom +
					((vLocalSystemData->vITIPCDig->vAquisitionSizeY - TempLoopY - 1) * vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors);

				for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX; TempLoopX++)
					*TempDestinationBuffer++ = *TempSourceBuffer++;  //copy data
			}
			
			vLocalSystemData->vITIPCDig->vNextImageToDisplayLock.Unlock();

			if (vLocalSystemData->vSystemRunMode == cCalibrateEncoderMode)
			{
				if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
				{
					vLocalSystemData->vITIPCDig->vNextImageToDisplay->vTop = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1;
					vLocalSystemData->vITIPCDig->vNextImageToDisplay->vLeft = 0;
					vLocalSystemData->vITIPCDig->vNextImageToDisplay->vRight = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1;
					vLocalSystemData->vITIPCDig->vNextImageToDisplay->vBottom = 0;
					vLocalSystemData->vITIPCDig->vNextImageToDisplay->vHeight = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1;
					vLocalSystemData->vITIPCDig->vNextImageToDisplay->vWidth = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1;
				}
				else
				{
					vLocalSystemData->vITIPCDig->FindContainerDimensions(
						vLocalSystemData->vITIPCDig->vNextImageToDisplay->vGreyImage, vLocalSystemData->vITIPCDig->vIATScratchImage,
						&vLocalSystemData->vITIPCDig->vNextImageToDisplay->vTop,&vLocalSystemData->vITIPCDig->vLastBottom,
						&vLocalSystemData->vITIPCDig->vNextImageToDisplay->vRight,&vLocalSystemData->vITIPCDig->vNextImageToDisplay->vLeft,
						&vLocalSystemData->vITIPCDig->vNextImageToDisplay->vHeight,&vLocalSystemData->vITIPCDig->vNextImageToDisplay->vWidth, 0, 10000); //vLocalConfigurationData->vShapeHandlingWidthTolerancePixels); //10000 = don't use reference so ignore old one

					//ReportErrorMessage("IAT3-Found Edges Left: " + dtoa(vLocalSystemData->vITIPCDig->vLastLeft, 2) + ", Right: " + dtoa(vLocalSystemData->vITIPCDig->vLastRight, 2), cWriteToLog, 0);
				}
			}

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("Post Image Ready to Display - Calibration Image", cDebugMessage);

			vLocalSystemData->vPostedNewMainImageReadyToDisplayMessage = true;
		}
	}
}

void CImageAquisitionThread::OutputImageToCSVFile(CString TempImageFileName, BYTE *TempImage, WORD TempSizeX, WORD TempSizeY)
{
	if (TempImage)
	{
		CTime TempTime = CTime::GetCurrentTime();
		CString TempTimeString = TempTime.Format("%H-%M-%S");//24 hour time format

		TempImageFileName = TempImageFileName + TempTimeString + ".csv";
		LPCTSTR TempFileName = TempImageFileName;
		CFileException TempFileException;
		if (ThereIsEnoughMemory(sizeof(CFile), "Calibration Data File"))
		{
			CFile *vDataFile;
			vDataFile = new CFile;
			if (vDataFile)
			{
				int TempFileOpened = vDataFile->Open(TempFileName, (CFile::modeCreate | CFile::modeReadWrite), &TempFileException);
				if (TempFileOpened == 0)
				{
					//could not open serial log file
					ReportErrorMessage("Error-Open File Failed: ", cEMailInspx,32000);// + TempFileException.m_cause); 
					#ifdef _DEBUG
						afxDump << "Could Not Open Data File"  << "\n";
					#endif
				}
				else
				{
					//write the image data to the file
					TRY
					{//a/b pixel should be last pixel of each line.  numbers written to file must be rotatate 90 degrees counter clock wise to be seen as shown on ScanTrac X-Ray pictures on screen.
						CString TempString = " ";
						TempString = "Pixels__,";
						BYTE *TempImageBuffer;
						for (int TempLineLoop = 0; TempLineLoop < TempSizeX; TempLineLoop++) 
							TempString = TempString + Pad(dtoa(TempLineLoop,0), 3, 0) + ',';
						
						TempString = TempString + "\n";
						WORD TempLength = TempString.GetLength();
						if (TempLength)
						{
							CW2A TempStringToWrite(TempString);
							vDataFile->Write(TempStringToWrite, TempLength);
						}

						TempImageBuffer = TempImage;
						for (WORD TempLineLoop = 0; TempLineLoop < TempSizeY; TempLineLoop++) 
						{
							TempString = "Line " + Pad(dtoa(TempLineLoop,0), 3, 0) + ',';

							for (WORD TempPixelLoop = 0; TempPixelLoop < TempSizeX; TempPixelLoop++) 
							{
								TempString = TempString + Pad(ByteToHex(*TempImageBuffer), 3, 0) + ',';
								TempImageBuffer++;
							}

							TempString = TempString + "Line " + dtoa(TempLineLoop,0) + "\n";

							WORD TempLength = TempString.GetLength();
							if (TempLength)
							{
								CW2A TempStringToWrite(TempString);
								vDataFile->Write(TempStringToWrite, TempLength);
							}
						}
					}
					CATCH_ALL(TempFileException)
					{
					 //#ifdef _DEBUG
					//		afxDump << "Could Not Write Calibration File"  << "\n";
					 //#endif
					}
					END_CATCH_ALL
					TRY
					{
						vDataFile->Close();
					}
					CATCH_ALL(TempFileException)
					{
					 #ifdef _DEBUG
							afxDump << "Could Not Close Calibration Image File"  << "\n";
					 #endif
					}
					END_CATCH_ALL
				}
				delete vDataFile;
				vDataFile = NULL;
			}
		}
	}
}

double CImageAquisitionThread::CalculateDetectorDensity(BYTE *TempImagePointer, BYTE TempDetectorNumber, WORD TempSizeX)
{
	double TempReturn = -1;
	WORD TempROIBottom = (TempDetectorNumber * vLocalConfigurationData->vPixelsPerDetector) + 4;
	WORD TempROITop = ((TempDetectorNumber + 1) * vLocalConfigurationData->vPixelsPerDetector) - 4;
	WORD TempROILeft = 1;
	WORD TempROIRight = vLocalSystemData->vITIPCDig->vAquisitionSizeY - 2;

	float TempSum = 0;
	BYTE *TempPointer = NULL;
	DWORD TempCount = 0;

	for (WORD TempLoopY = TempROILeft; TempLoopY < TempROIRight; TempLoopY++)
	{
		TempPointer = TempImagePointer + TempROIBottom + (TempLoopY * TempSizeX);
		for (WORD TempLoopX = TempROIBottom; TempLoopX < TempROITop; TempLoopX++)
		if (vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors > TempLoopX)
		{
			TempSum = TempSum + *TempPointer++;
			TempCount++;
		}
	}
	if (TempCount)
		TempReturn = TempSum / TempCount;
	return TempReturn;
}

void CImageAquisitionThread::CheckForFloodedIVBag(CContainer *TempContainer, CInspection *TempInspection, BYTE TempInspectionNumber)
{
	//Threshold the image at the Threshold to get rid of the faint water drops.
	//then find the height difference between the lowest point and the highest point on the bottom

	if (TempInspection)
	{
		//CopyMemory(vLocalSystemData->vITIPCDig->vLatestImageCopy->pix, vLocalSystemData->vITIPCDig->vLatestImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

		WORD TempDelta = 0;
		WORD TempBottom = 0;
		if (TempInspection->vEnable)
		if (TempInspection->IsIVBagFlooded(vLocalSystemData->vITIPCDig->vLatestImageCopy, &TempDelta, &TempBottom,
			vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY))
		{
			vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = TempDelta;
			if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
			{
				TempContainer->vTheImage->vContainerEjectThreshold = TempInspection->vSize;
				TempContainer->vTheImage->vContainerLearnState = TempInspection->vThreshold;
				TempContainer->vTheImage->vContainerLargestContaminant = TempDelta;

				//save the red image
				vLocalSystemData->vITIPCDig->SaveRedImageForDisplay(vLocalSystemData->vITIPCDig->vLatestImageCopy, vLocalSystemData->vLastContainerImaged);

				//mark that you need to eject this
				vLocalSystemData->vThisContainerShouldBeEjected = true;

				//if evaluating, don't eject this container if already ejected
				if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
				{
					if (vGlobaluCSerialPort)
						vGlobaluCSerialPort->EjectContainer(cEjectInspectionOffset + TempInspectionNumber, vLocalSystemData->vLastContainerImaged, vGlobalCurrentProduct->vInspection[TempInspectionNumber]);

					if (vLocalSystemData->vITIPCDig->vBlobResults)
					{
						vLocalSystemData->vITIPCDig->vBlobResults->num_blobs_found = 1;
						if (!vLocalSystemData->vITIPCDig->vBlobResults->blobs[0])
						{
							MVT_BLOB *TempBlob = new(MVT_BLOB);
							vLocalSystemData->vITIPCDig->vBlobResults->blobs[0] = TempBlob;
						}

						if (vLocalSystemData->vITIPCDig->vBlobResults->blobs[0])
						{
							vLocalSystemData->vITIPCDig->vBlobResults->blobs[0]->height = TempInspection->vROIPixelRight - TempInspection->vROIPixelLeft;
							//vLocalSystemData->vITIPCDig->vBlobResults->blobs[0]->height = vLocalSystemData->vITIPCDig->vLastRight - vLocalSystemData->vITIPCDig->vLastLeft;
							vLocalSystemData->vITIPCDig->vBlobResults->blobs[0]->width = TempDelta;
							vLocalSystemData->vITIPCDig->vBlobResults->blobs[0]->ymin = TempInspection->vROIPixelLeft;
							//vLocalSystemData->vITIPCDig->vBlobResults->blobs[0]->ymin = vLocalSystemData->vITIPCDig->vLastLeft - vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust;
							vLocalSystemData->vITIPCDig->vBlobResults->blobs[0]->xmin = TempBottom + TempInspection->vROIPixelBottom;
							vLocalSystemData->vITIPCDig->vBlobResults->blobs[0]->area = TempDelta * (TempInspection->vROIPixelRight - TempInspection->vROIPixelLeft);
							vLocalSystemData->vITIPCDig->vBlobResults->blobs[0]->ycent = vLocalSystemData->vITIPCDig->vLastLeft + 
								((TempInspection->vROIPixelRight - TempInspection->vROIPixelLeft) / 2);
							vLocalSystemData->vITIPCDig->vBlobResults->blobs[0]->xcent = TempBottom + TempInspection->vROIPixelBottom + (TempDelta / 2);

							vLocalSystemData->vITIPCDig->Save10RedBoxesForDisplay(TempInspection, vLocalSystemData->vITIPCDig->vBlobResults, vLocalSystemData->vITIPCDig->vLastLeftReferenceAdjust);
						}
					}
				}

				if (vLocalSystemData->vLogFile.vLogSerialData)
				{
					if (vLocalSystemData->vLastContainerImaged)
					if (vLocalSystemData->vLastContainerImaged->vContainerNumber)
						vLocalSystemData->vLogFile.WriteToLogFile(dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0) + " IV Bag Flooded, Inspection: " + dtoa(TempInspectionNumber,0),cDebugMessage);
					else
						vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString + " IV Bag Flooded, Inspection: " + dtoa(TempInspectionNumber,0),cDebugMessage);
				}
			}
		}
		else
		{
			vLocalSystemData->vITIPCDig->vLastNumberOfBlobs[TempInspectionNumber] = TempDelta;
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				if (vLocalSystemData->vLastContainerImaged)
					vLocalSystemData->vLogFile.WriteToLogFile(dtoa(vLocalSystemData->vLastContainerImaged->vContainerNumber,0) + " IV Bag NOT Flooded, Inspection: " + dtoa(TempInspectionNumber,0),cDebugMessage);
				else
					vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString +" IV Bag NOT Flooded, Inspection: " + dtoa(TempInspectionNumber,0),cDebugMessage);
			}
		}

		//evaluate good in find contaminants
		if (TempInspection->vFindGoodIntensity)
		if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
		if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cPassSlippedContainer)//slipped
			vLocalSystemData->vWrongWidthsDuringLearn++;
		else
		if ((vSizeOK) || (vNumberOfWrongSizeInARow > 50))
		if (vLocalSystemData->vLastContainerImaged)
		if (((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode)) || //if not bulk mode, must be dark enough
			(vCurrentImageBrightnessOK))
		{
			//find minimum good for this frame
			BYTE TempMinimumGood = 255;
			//first time through, initialize data structures
			if (!vLocalSystemData->vMinimumGoodIndex)
			{
				if (vLocalSystemData->vMinimumGoodData)
				{
					free(vLocalSystemData->vMinimumGoodData);
					vLocalSystemData->vMinimumGoodData = NULL;
				}
				if (ThereIsEnoughMemory(cStandardDeviationSampleSize,"Minimum Good Sample Array"))
					vLocalSystemData->vMinimumGoodData = (BYTE*)malloc(cStandardDeviationSampleSize);

				vLocalSystemData->vMinimumGoodStandardDeviation = 0;
				vLocalSystemData->vMinimumGoodAverage = 0;
				vLocalSystemData->vMinimumGoodIntensity = 256;
			}

			BYTE *TempMinimumGoodDataPointer = vLocalSystemData->vMinimumGoodData + vLocalSystemData->vMinimumGoodIndex; //place to store this data point in data collection array
			if (vLocalSystemData->vMinimumGoodData)
			if (vLocalSystemData->vMinimumGoodIndex < cStandardDeviationSampleSize)
			{
				TempMinimumGood = (BYTE)TempDelta;
			}
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("Autosetup, Find Minimum Good: " + dtoa(TempMinimumGood,0), cDebugMessage);

			if (vLocalSystemData->vMinimumGoodData)
			if (vLocalSystemData->vMinimumGoodIndex < cStandardDeviationSampleSize)
			{
				*TempMinimumGoodDataPointer = (BYTE)TempMinimumGood; //store this images lowest good threshold in data array
				//save the best value for displaying on the screen
				vLocalSystemData->vITIPCDig->vLastMinimumGoodMaximumBad = *TempMinimumGoodDataPointer;
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("Final Minimum Good: " + dtoa(*TempMinimumGoodDataPointer,0),	cDebugMessage);

				if (vLocalSystemData->vMinimumGoodIntensity > *TempMinimumGoodDataPointer)
					vLocalSystemData->vMinimumGoodIntensity = *TempMinimumGoodDataPointer;

				if (vLocalSystemData->vMinimumGoodIndex < cStandardDeviationSampleSize - 1)
					vLocalSystemData->vMinimumGoodIndex++;
				else
					TempInspection->vFindGoodIntensity = false;

				if (vLocalSystemData->vMinimumGoodIndex > 3)
				{
					vLocalSystemData->vMinimumGoodStandardDeviation = CalculateStandardDeviation(vLocalSystemData->vMinimumGoodData, vLocalSystemData->vMinimumGoodIndex, &vLocalSystemData->vMinimumGoodAverage);
					if (vLocalSystemData->vMinimumGoodStandardDeviation < .5)
						vLocalSystemData->vMinimumGoodStandardDeviation = .5;

					if (vLocalConfigurationData->vDemoMode)
					{
						vLocalSystemData->vMinimumGoodStandardDeviation = (1.0 +	((vLocalSystemData->vMinimumGoodIndex % 5) / 10.0));
						vLocalSystemData->vMinimumGoodAverage = (145.0 + ((vLocalSystemData->vMinimumGoodIndex % 10) / 10.0));
					}
					vLocalSystemData->CalculateQualityFactor();
				}
			}
		}
		//evaluate bad
		if (TempInspection->vFindBadIntensity)
		if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
		if (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cPassSlippedContainer)//slipped
			vLocalSystemData->vWrongWidthsDuringLearn++;
		else
		if ((vSizeOK) || (vNumberOfWrongSizeInARow > 50))
		if (vLocalSystemData->vLastContainerImaged)
		if (((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode)) || //if bulk mode, must be dark enough
			(vCurrentImageBrightnessOK))
		{
			//find minimum bad for this frame
			BYTE TempMaximumBad = 255;
			BYTE *TempPointer = vLocalSystemData->vITIPCDig->vLatestImageCopy->pix;

			if ((!vLocalSystemData->vSendingGoodWithBad) || (TempMaximumBad < vLocalSystemData->vMinimumGoodIntensity))
			{  // if the bad intensity is below the good level
				//first time through, initialize data structures
				if (!vLocalSystemData->vMaximumBadIndex)
				{
					vLocalSystemData->vMaximumBadCount = 0;
					if (vLocalSystemData->vMaximumBadData)
					{
						free(vLocalSystemData->vMaximumBadData);
						vLocalSystemData->vMaximumBadData = NULL;
					}
					if (ThereIsEnoughMemory(cStandardDeviationSampleSize,"Maximum Bad Sample Array"))
						vLocalSystemData->vMaximumBadData = (BYTE*)malloc(cStandardDeviationSampleSize);
					vLocalSystemData->vMaximumBadStandardDeviation = 0;
					vLocalSystemData->vMaximumBadAverage = 0;
					vLocalSystemData->vMaximumBadIntensity = 0;
					vLocalSystemData->vQualityFactor = 0;
				}

				BYTE *TempMaximumBadDataPointer = vLocalSystemData->vMaximumBadData + vLocalSystemData->vMaximumBadIndex;
				if (vLocalSystemData->vMaximumBadData)
				{
					TempMaximumBad = (BYTE)TempDelta;

					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("Autosetup, Find Maximum Bad: " + dtoa(TempMaximumBad,0), cDebugMessage);
				}

				if (vLocalSystemData->vMaximumBadData)
				{
					*TempMaximumBadDataPointer = TempMaximumBad;
					//save the best value for displaying on the screen
					vLocalSystemData->vITIPCDig->vLastMinimumGoodMaximumBad = *TempMaximumBadDataPointer;

					if (vLocalSystemData->vMaximumBadIntensity < *TempMaximumBadDataPointer)
						vLocalSystemData->vMaximumBadIntensity = *TempMaximumBadDataPointer;

					if (vLocalSystemData->vMaximumBadIndex < cStandardDeviationSampleSize - 1)
					{
						vLocalSystemData->vMaximumBadIndex++;
						vLocalSystemData->vMaximumBadCount++;
					}
					else
						TempInspection->vFindBadIntensity = false;

					if (vLocalSystemData->vMaximumBadIndex > 3)
					{
						vLocalSystemData->vMaximumBadStandardDeviation = CalculateStandardDeviation(vLocalSystemData->vMaximumBadData, vLocalSystemData->vMaximumBadIndex, &vLocalSystemData->vMaximumBadAverage);
						if (!vLocalSystemData->vMaximumBadStandardDeviation)
							vLocalSystemData->vMaximumBadStandardDeviation = .5;

						if (!vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
						if (vLocalConfigurationData->vDemoMode)
						{
							vLocalSystemData->vMaximumBadStandardDeviation = (float)(1.0 +	((vLocalSystemData->vMaximumBadIndex % 5) / 10.0));
							vLocalSystemData->vMaximumBadAverage = (float)(7.0 + ((vLocalSystemData->vMaximumBadIndex % 10) / 10.0));
						}
						vLocalSystemData->CalculateQualityFactor();
					}
				}
			}  //end if (TempMaximumBad < vLocalSystemData->vMinimumGoodIntensity)
		}//end of evaluate
	}
}

void CImageAquisitionThread::SaveDataInValveMonitor(CContainer *TempContainer, double TempWeight)
{
	vSavedFillerMonitorWeight = true;
	if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
	//if (vLocalSystemData->vRecievedFirstIndexPulse[cFillerSamplingType])
	//if (vLocalSystemData->vNextHeadToProcess[cFillerSamplingType] < vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
	//if (TempContainer->vHeadNumber[cFillerSamplingType])
	if (TempContainer->vHeadNumber[cFillerSamplingType] > 0)
	if (vLocalSystemData->vCurrentValveData)
	{
		//when using FIFO, if head number wraps around to 1, or low value, then completed last set of data, save if complete set, or throw away if too many or too few
		vLocalSystemData->vNextHeadToProcess[cFillerSamplingType] = TempContainer->vHeadNumber[cFillerSamplingType];
		if (vLocalSystemData->vUseSamplingFIFO) //In FIFO mode, each index pulse sees if last group was complete and marks error if not, then zeros counters that put head numbers into FIFOs
		if (vLocalSystemData->vCurrentSampleDataCount[cFillerSamplingType]) //had at least one container in last filler rotation

		//went from a higher head number to a lower head number, so copy data from temporary memory to permanent memory
		if ((vLocalSystemData->vNextHeadToProcess[cFillerSamplingType]  < (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] / 2)) && 
			(vLocalSystemData->vPreviousHeadProcessed[cFillerSamplingType] > (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] / 2)))
		{
			if (vGlobaluCSerialPort)
				vGlobaluCSerialPort->HandleCompleteFillerMonitorDataSet();
		}
		vLocalSystemData->vPreviousHeadProcessed[cFillerSamplingType] = vLocalSystemData->vNextHeadToProcess[cFillerSamplingType];

		//Now Save this data value for its head and the in the current set
		if (TempContainer->vHeadNumber[cFillerSamplingType] <= vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
		{
			double *TempPointer = vLocalSystemData->vHoldValveDataForNextIndexPulse + TempContainer->vHeadNumber[cFillerSamplingType] - 1;
			*TempPointer = TempWeight;

			//if low fill, then count that head as having a low fill
			if ((vLocalSystemData->vValveMonitorInspection < vGlobalCurrentProduct->vNumberOfInspections) && (vGlobalCurrentProduct) && 
				(vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection]))
			{
				if (vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection]->vInspectionType == cUnderfillByWeightInspection)
				{
					double TempThreshold = vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection]->ConvertToWeight(vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection]->vDensityThresholdUpper);
					if (TempWeight < TempThreshold)
					{
						BYTE TempActualHeadNumber = vMainWindowPointer->CalculateHeadNumber(((int)TempContainer->vHeadNumber[cFillerSamplingType]), cFillerSamplingType);
						vLocalSystemData->vFillerLowFillHeadCounts[TempActualHeadNumber - 1]++; //subtract 1 because array is zero based, but head number is one based
					}
				}
				else
				if (vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection]->vInspectionType == cUnderfillInspection)
				if (TempWeight > vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection]->vDensityThresholdUpper)
				{
					BYTE TempActualHeadNumber = vMainWindowPointer->CalculateHeadNumber(((int)TempContainer->vHeadNumber[cFillerSamplingType]), cFillerSamplingType);
					vLocalSystemData->vFillerLowFillHeadCounts[TempActualHeadNumber - 1]++; //subtract 1 because array is zero based, but head number is one based
				}
			}
			else
			if (vGlobalCurrentProduct->vFillerMonitorInspection)
			{
				//if (vGlobalCurrentProduct->vFillerMonitorInspection->vInspectionType == cDensityInspection)
				if (TempWeight > vGlobalCurrentProduct->vFillerMonitorInspection->vDensityThresholdUpper)
				{
					BYTE TempActualHeadNumber = vMainWindowPointer->CalculateHeadNumber(((int)TempContainer->vHeadNumber[cFillerSamplingType]), cFillerSamplingType);
					vLocalSystemData->vFillerLowFillHeadCounts[TempActualHeadNumber - 1]++; //subtract 1 because array is zero based, but head number is one based
				}
			}
		}

		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("*IAT-Valve monitor count: " + dtoa(vLocalSystemData->vCurrentSampleDataCount[cFillerSamplingType], 0) + ", valve: " +
			dtoa(vLocalSystemData->vNextHeadToProcess[cFillerSamplingType],0) + ", Fill: " + dtoa(TempWeight, 2),cDebugMessage);

		TempContainer->vFillLevel = TempWeight;
	}
	vLocalSystemData->vCurrentSampleDataCount[cFillerSamplingType]++;

	if ((!vLocalSystemData->vITIPCDig->vShowOnlyRealImages) && (!vLocalSystemData->vUseSamplingFIFO))
  //simulating image without FIFO, so pretend there is an index pulse-This is the same code if you get Aux Det Input
	{
		if (vLocalSystemData->vSamplingIndexErrorData[cFillerSamplingType])
		{
			vLocalSystemData->vNextHeadToProcess[cFillerSamplingType]++;
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("*IAT-Simulate sampling increment to next valve: " + dtoa(vLocalSystemData->vNextHeadToProcess[cFillerSamplingType], 0),cDebugMessage);
			if (vLocalSystemData->vNextHeadToProcess[cFillerSamplingType] >= vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
			{
				vLocalSystemData->vLastIndexPulseWasCorrect[cFillerSamplingType] = true;

				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("*IAT-Simulate Sampling End Of Valve Group: " + dtoa(vLocalSystemData->vNumberValveRowDataHave, 0),cDebugMessage);
				//got the Index pulse, so reset the counter
				if (vLocalSystemData->vRecievedFirstIndexPulse[cFillerSamplingType])
				{
					//do error checking to be sure index pulse is in correct place to be sure you got entire set of containers (one from each valve)
					int *TempPointerToValveError = vLocalSystemData->vSamplingIndexErrorData[cFillerSamplingType] + vLocalSystemData->vSamplingIndexErrorDataIndex[cFillerSamplingType];
					if (vLocalSystemData->vNextHeadToProcess[cFillerSamplingType] == vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]) //Index should be zero when get the sync pulse as should have written current data, and now point at zeroth position
						*TempPointerToValveError = 0;
					else
					{	//positive value is number of extra packages, negative value is number of packages missed
						*TempPointerToValveError = vLocalSystemData->vCurrentSampleDataCount[cFillerSamplingType] - vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; 
					}
					vLocalSystemData->vSamplingIndexErrorDataIndex[cFillerSamplingType]++;
					if (vLocalSystemData->vSamplingIndexErrorDataIndex[cFillerSamplingType] >= cMaximumValveDataRows)
						vLocalSystemData->vSamplingIndexErrorDataIndex[cFillerSamplingType] = 0;
					if (vLocalSystemData->vSamplingIndexErrorDataHave[cFillerSamplingType] < cMaximumValveDataRows)
						vLocalSystemData->vSamplingIndexErrorDataHave[cFillerSamplingType]++;
						
					//copy the set of data from current to long term data if no index pulse error, current data has offset of Index pulse, so copy over so valve 1 is in first position of permanent data
					if (*TempPointerToValveError == 0)
					{
						double *TempPointerToDataRowToWriteInto = vLocalSystemData->vValveData + (vLocalSystemData->vValveDataRowIndex * vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]); 
						double *TempPointerToCurrentDataJustTook = vLocalSystemData->vCurrentValveData + vMainWindowPointer->GetCurrentFillerIndexOffset();
						for (BYTE TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; TempLoop++)
						if (TempPointerToDataRowToWriteInto < vLocalSystemData->vValveDataMaximum)
						{
							*TempPointerToDataRowToWriteInto++ = *TempPointerToCurrentDataJustTook++;// + TempRandomNumber;
							if (TempPointerToCurrentDataJustTook >= vLocalSystemData->vCurrentValveDataMaximum) //have read to end of valve set as started at offset, so start back at 0th location
								TempPointerToCurrentDataJustTook = vLocalSystemData->vCurrentValveData;
						}
						else
							ReportErrorMessage("Valve Monitor Pointer Error 2", cEMailInspx, 32000);

						if (vLocalSystemData->vNumberValveRowDataHave < cMaximumValveDataRows)
							vLocalSystemData->vNumberValveRowDataHave++;

						vLocalSystemData->vValveDataRowIndex++;
						if (vLocalSystemData->vValveDataRowIndex >= cMaximumValveDataRows)
							vLocalSystemData->vValveDataRowIndex = 0;

						BYTE TempFindLowestValue = 0;
						BYTE TempSpeedFindingAt = 0;
						for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
						if (vLocalSystemData->vFindFillerOffset[TempLoop])
						{
							TempFindLowestValue = vLocalSystemData->vFindFillerOffset[TempLoop];
							TempSpeedFindingAt = TempLoop;
						}

						//find head 1 offset from Index pulse
						if (TempFindLowestValue)
						{
							double *TempPointerToMostRecentDataJustTook = vLocalSystemData->vCurrentValveData;

							if (vLocalSystemData->vValveMonitorInspection < vGlobalCurrentProduct->vNumberOfInspections)
							if ((vLocalSystemData->vValveMonitorInspection < vGlobalCurrentProduct->vNumberOfInspections) && (vGlobalCurrentProduct) && (vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection]))
							if ((vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection]->vInspectionType == cUnderfillInspection) ||
								(vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection]->vInspectionType == cOverfillInspection))
							{//must swap highest to lowest as fill is opposite in values as weight
								if (TempFindLowestValue == 1) //finding highest value
									TempFindLowestValue = 2; //find lowest value as fill values are opposite of weight values
								else
								if (TempFindLowestValue == 2) //finding highest value
									TempFindLowestValue = 1; //find lowest value as fill values are opposite of weight values
							}
							//find heaviest container (lowest brightness number)
							if (TempFindLowestValue == 1)
							{
								double TempLowestValue = 0xffffff;
								BYTE TempLowestPosition = 0;

								for (BYTE TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; TempLoop++)
								{
									if (*TempPointerToMostRecentDataJustTook < TempLowestValue)
									{
										TempLowestValue = *TempPointerToMostRecentDataJustTook;
										TempLowestPosition = TempLoop;
									}
									TempPointerToMostRecentDataJustTook++;
								}

								vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[TempSpeedFindingAt] = (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] - 
										TempLowestPosition) % vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType];  //the modulus affects the case where highest position is 0, because x % x = 0, offset is 0 not numberOfHeads
											
								vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[TempSpeedFindingAt] = vLocalSystemData->vActualEncoderRate;

								ReportErrorMessage("Info Only-Filler Speed: " + dtoa(TempSpeedFindingAt + 1, 0) + " Offset found to be: " + dtoa(vMainWindowPointer->GetCurrentFillerIndexOffset(), 0) +
									" Encoder Rate: " + dtoa(vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[TempSpeedFindingAt], 0), cUserChanges, 0);
							}
							else
							if (TempFindLowestValue == 2)
							{ //find highest value or lightest container
								double TempHighesttValue = 0;
								BYTE TempHighestPosition = 0;

								for (BYTE TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; TempLoop++)
								{
									if (*TempPointerToMostRecentDataJustTook > TempHighesttValue)
									{
										TempHighesttValue = *TempPointerToMostRecentDataJustTook;
										TempHighestPosition = TempLoop;
									}
									TempPointerToMostRecentDataJustTook++;
								}
								vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[TempSpeedFindingAt] = (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] - 
										TempHighestPosition) % vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType];  //the modulus affects the case where highest position is 0, because x % x = 0, offset is 0 not numberOfHeads
											
								vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[TempSpeedFindingAt] = vLocalSystemData->vActualEncoderRate;

								ReportErrorMessage("Info Only--->Filler Speed: " + dtoa(TempSpeedFindingAt + 1, 0) + " Offset found to be: " + dtoa(vMainWindowPointer->GetCurrentFillerIndexOffset(), 0) +
									" Encoder Rate: " + dtoa(vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[TempSpeedFindingAt], 0), cUserChanges, 0);
							}
							else
							if (TempFindLowestValue == 3)
							{ //find most variable value
								double TempMostVariableValue = 0;
								BYTE TempMostVariablePosition = 0;
								double *TempStandardDieviationValues = vLocalSystemData->vValveStandardDeviations;

								for (BYTE TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]; TempLoop++)
								{
									if (*TempStandardDieviationValues > TempMostVariableValue)
									{
										TempMostVariableValue = *TempStandardDieviationValues;
										TempMostVariablePosition = TempLoop;
									}
									TempStandardDieviationValues++;
								}
								vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[TempSpeedFindingAt] = (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] - 
										TempMostVariablePosition) % vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType];  //the modulus affects the case where highest position is 0, because x % x = 0, offset is 0 not numberOfHeads
											
								vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[TempSpeedFindingAt] = vLocalSystemData->vActualEncoderRate;

								ReportErrorMessage("Info Only--->Filler Speed: " + dtoa(TempSpeedFindingAt + 1, 0) + " Offset found to be: " + dtoa(vMainWindowPointer->GetCurrentFillerIndexOffset(), 0) +
									" Encoder Rate: " + dtoa(vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[TempSpeedFindingAt], 0), cUserChanges, 0);
							}
							for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
								vLocalSystemData->vFindFillerOffset[TempLoop] = 0; //clear the flag to find the filler offset as you are done
						}
					}
					else
					{
						::PostThreadMessage(vGlobalCallingThreadID, cIndexPulseErrorMessage, cFillerSamplingType, cFillerSamplingType);
						 ZeroMemory(vLocalSystemData->vHoldValveDataForNextIndexPulse, vMainWindowPointer->GetCurrentFillerIndexOffset() * sizeof(double));

						if (vLocalSystemData->vSampledOnLastRevolution[cFillerSamplingType])
						{
							ReportErrorMessage("Filler Index ERROR\nLast sample may be from wrong valve\nTake sample over", cCritical,0);
							vLocalSystemData->vSampledOnLastRevolution[cFillerSamplingType] = false;
						}
					}
				}

				//start collecting a new round of valve data
				vLocalSystemData->vNextHeadToProcess[cFillerSamplingType] = 0;
				vLocalSystemData->vCurrentSampleDataCount[cFillerSamplingType] = 0;
				vLocalSystemData->vRecievedFirstIndexPulse[cFillerSamplingType] = true;
				vLocalSystemData->vNextHeadToProcess[cFillerSamplingType] = 0;
				vLocalSystemData->vTimeToGetIndexPulse = true;
			}
			else
			if ((vLocalSystemData->vNextHeadToProcess[cFillerSamplingType] == 1) && (vLocalSystemData->vTimeToGetIndexPulse))
			if (vLocalSystemData->vITIPCDig->vShowOnlyRealImages)
			{
				vLocalSystemData->vTimeToGetIndexPulse = false;
				if (vLocalSystemData->vMissedIndexCount < 253)
					vLocalSystemData->vMissedIndexCount++;
				if (vLocalSystemData->vMissedIndexCount == 3)
					ReportErrorMessage("Filler Monitor Error-Missed Index Pulse", cWriteToLog, 0);
				if (vLocalSystemData->vMissedIndexCount == 250)
					ReportErrorMessage("Filler Monitor Error-Missed Many/All Index Pulses", cWriteToLog, 0);
				if (vLocalConfigurationData->vShowIndexErrors)
					::PostThreadMessage(vGlobalCallingThreadID, cIndexPulseErrorMessage, cSeamerSamplingType, cFillerSamplingType);

				if (vLocalSystemData->vSampledOnLastRevolution[cFillerSamplingType])
				{
					ReportErrorMessage("Filler Index ERROR,\nLast sample may be from wrong valve\nTake sample over", cCritical,0);
					vLocalSystemData->vSampledOnLastRevolution[cFillerSamplingType] = false;
				}
			}
		}
	}
}
						
void CImageAquisitionThread::InspectionFoundContaminant(BYTE TempInspectionNumber)
{
	if (TempInspectionNumber < vGlobalCurrentProduct->vNumberOfInspections)
	if (vGlobalCurrentProduct->vInspection[TempInspectionNumber]->vEnable)
	if ((!vLocalConfigurationData->vStopAfterAnInspectionRejects) || (!vLocalSystemData->vLastContainerImaged->vToBeEjected)) //if processing all inspection, or have not decided to eject before
	{
		vLocalSystemData->vLastContainerImaged->vTheImage->vRejectedForThisInspection[TempInspectionNumber] = true;
		//if (vLocalConfigurationData->vEnableEjectors)
		//	vLocalSystemData->vXRayCriteriaCount[vLocalSystemData->vCountToUseForEjectReason[cEjectInspectionOffset + TempInspectionNumber]]++;
		//else
		//	vLocalSystemData->vXRayCriteriaCountButDisabled[vLocalSystemData->vCountToUseForEjectReason[cEjectInspectionOffset + TempInspectionNumber]]++;

		//vLocalSystemData->vXRayCriteriaCountTotal[vLocalSystemData->vCountToUseForEjectReason[cEjectInspectionOffset + TempInspectionNumber]]++;

		//if evaluating, don't eject this container if already ejected
		if (!vLocalSystemData->vLastContainerImaged->vToBeEjected)
		if ((vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason > cTestEject) || 
			(vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason = cPassSlippedContainer) || (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cPassContainerWrongWidth) ||
			(vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cPassContainerOnTop) || (vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason == cPassContainerWrongHeight))
		{
			//if (vGlobaluCSerialPort)
			//	vGlobaluCSerialPort->EjectContainer(cEjectInspectionOffset + TempInspectionLoop, vLocalSystemData->vLastContainerImaged, 
			//		vGlobalCurrentProduct->vInspection[TempInspectionLoop], vCurrentFrameIsARightImage);
			vLocalSystemData->vLastContainerImaged->vTheImage->vContainerEjectReason = cEjectInspectionOffset + TempInspectionNumber;  //255 = not ejected.  Any other value is reason for eject
			vLocalSystemData->vLastContainerImaged->vToBeEjected = true;
		}
	}
}
